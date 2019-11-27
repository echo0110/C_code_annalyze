/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2017 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: oam_agnt_parser.c $
 *
 *******************************************************************************
 *
 * File Description: OAM AGENT Parser defination all OAM AGENT APIs.
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#include "oam_agnt_utils.h"
#include "oam_agnt_port.h"
#include "oam_agnt_intf.h"
#include "oam_mgmt_intf_api.h"


#define ARRSIZE(array_name) (sizeof(array_name) / sizeof(array_name[0]))


/*****************************************************************************/
/*                      Functions forward declarations                       */
/*****************************************************************************/

static
oam_agnt_return_et
oam_agnt_parse_oam_agent_process_name
(
    oam_agent_process_name_t *p_oam_agent_process_name,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_agnt_return_et
oam_agnt_parse_oam_agent_process_info_list
(
    oam_agent_process_info_list_t *p_oam_agent_process_info_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_agnt_return_et
oam_agnt_parse_oam_agent_dir_path
(
    oam_agent_dir_path_t *p_oam_agent_dir_path,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_agnt_return_et
oam_agnt_parse_oam_agent_process_args
(
    oam_agent_process_args_t *p_oam_agent_process_args,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_agnt_return_et
oam_agnt_parse_oam_agent_process_spawn_list
(
    oam_agent_process_spawn_list_t *p_oam_agent_process_spawn_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_agnt_return_et
oam_agnt_parse_oam_agent_process_spawn_status_list
(
    oam_agent_process_spawn_status_list_t *p_oam_agent_process_spawn_status_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_agnt_return_et
oam_agnt_parse_oam_agent_kill_process_list
(
    oam_agent_kill_process_list_t *p_oam_agent_kill_process_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_agnt_return_et
oam_agnt_parse_oam_agent_kill_process_status_list
(
    oam_agent_kill_process_status_list_t *p_oam_agent_kill_process_status_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

static
oam_agnt_return_et
oam_agnt_parse_oam_agent_file_name
(
    oam_agent_file_name_t *p_oam_agent_file_name,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

oam_agnt_return_et
oam_agnt_parse_agent_oam_init_ind
(
    agent_oam_init_ind_t *p_agent_oam_init_ind,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_agnt_memset(p_agent_oam_init_ind, 0, sizeof(agent_oam_init_ind_t));

    /* This function parses agent_oam_init_ind */
    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "Parsing agent_oam_init_ind");

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    oam_agnt_cp_unpack_UInt8(&p_agent_oam_init_ind->num_process, p_src + *p_length_read, "num_process");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_agent_oam_init_ind->num_process > OAM_AGENT_MAX_NUM_PROCESS))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Parameter [p_agent_oam_init_ind->num_process] should be in range [%d <= OAM_AGENT_MAX_NUM_PROCESS] !", p_agent_oam_init_ind->num_process);
        return OAM_AGNT_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        oam_agnt_counter_t loop;
        for (loop = 0; loop < p_agent_oam_init_ind->num_process; loop++)
        {
            if (OAM_AGNT_FAILURE == oam_agnt_parse_oam_agent_process_info_list(
                &p_agent_oam_init_ind->process_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_AGNT_FAILURE;
            }

            *p_length_read += length_read;
        }
    }


    if (OAM_AGNT_FAILURE == oam_agnt_parse_oam_agent_dir_path(
        &p_agent_oam_init_ind->bin_path,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_AGNT_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    return OAM_AGNT_SUCCESS;
}

static
oam_agnt_return_et
oam_agnt_parse_oam_agent_process_name
(
    oam_agent_process_name_t *p_oam_agent_process_name,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_agnt_memset(p_oam_agent_process_name, 0, sizeof(oam_agent_process_name_t));

    /* This function parses oam_agent_process_name */
    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "Parsing oam_agent_process_name");


    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    oam_agnt_cp_unpack_UInt8(&p_oam_agent_process_name->len, p_src + *p_length_read, "len");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_oam_agent_process_name->len > OAM_AGENT_MAX_PROCESS_NAME))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Parameter [p_oam_agent_process_name->len] should be in range [%d <= OAM_AGENT_MAX_PROCESS_NAME] !", p_oam_agent_process_name->len);
        return OAM_AGNT_FAILURE;
    }

    if (*p_length_read + (SInt32)(p_oam_agent_process_name->len * sizeof(p_oam_agent_process_name->name[0])) > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        oam_agnt_counter_t loop;
        for (loop = 0; loop < p_oam_agent_process_name->len; loop++)
        {
            oam_agnt_cp_unpack_Char8((void*)&p_oam_agent_process_name->name[loop], (void*)(p_src + *p_length_read), "name[]");
            *p_length_read += sizeof(Char8);
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    return OAM_AGNT_SUCCESS;
}

static
oam_agnt_return_et
oam_agnt_parse_oam_agent_process_info_list
(
    oam_agent_process_info_list_t *p_oam_agent_process_info_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_agnt_memset(p_oam_agent_process_info_list, 0, sizeof(oam_agent_process_info_list_t));

    /* This function parses oam_agent_process_info_list */
    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "Parsing oam_agent_process_info_list");


    if (OAM_AGNT_FAILURE == oam_agnt_parse_oam_agent_process_name(
        &p_oam_agent_process_info_list->process_name,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_AGNT_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt64) > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    oam_agnt_cp_unpack_UInt64(&p_oam_agent_process_info_list->cksum, p_src + *p_length_read, "cksum");
    *p_length_read += sizeof(UInt64);

    if (*p_length_read > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    return OAM_AGNT_SUCCESS;
}

static
oam_agnt_return_et
oam_agnt_parse_oam_agent_dir_path
(
    oam_agent_dir_path_t *p_oam_agent_dir_path,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_agnt_memset(p_oam_agent_dir_path, 0, sizeof(oam_agent_dir_path_t));

    /* This function parses oam_agent_dir_path */
    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "Parsing oam_agent_dir_path");


    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    oam_agnt_cp_unpack_UInt16(&p_oam_agent_dir_path->len, p_src + *p_length_read, "len");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [H - higher boundary] */
    if ((p_oam_agent_dir_path->len > OAM_AGENT_MAX_DIR_PATH))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Parameter [p_oam_agent_dir_path->len] should be in range [%d <= OAM_AGENT_MAX_DIR_PATH] !", p_oam_agent_dir_path->len);
        return OAM_AGNT_FAILURE;
    }

    if (*p_length_read + (SInt32)(p_oam_agent_dir_path->len * sizeof(p_oam_agent_dir_path->name[0])) > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        oam_agnt_counter_t loop;
        for (loop = 0; loop < p_oam_agent_dir_path->len; loop++)
        {
            oam_agnt_cp_unpack_Char8((void*)&p_oam_agent_dir_path->name[loop], (void*)(p_src + *p_length_read), "name[]");
            *p_length_read += sizeof(Char8);
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    return OAM_AGNT_SUCCESS;
}

oam_agnt_return_et
oam_agnt_parse_oam_agent_spawn_process_req
(
    oam_agent_spawn_process_req_t *p_oam_agent_spawn_process_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_agnt_memset(p_oam_agent_spawn_process_req, 0, sizeof(oam_agent_spawn_process_req_t));

    /* This function parses oam_agent_spawn_process_req */
    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "Parsing oam_agent_spawn_process_req");

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    oam_agnt_cp_unpack_UInt8(&p_oam_agent_spawn_process_req->num_process, p_src + *p_length_read, "num_process");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_oam_agent_spawn_process_req->num_process > OAM_AGENT_MAX_NUM_PROCESS))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Parameter [p_oam_agent_spawn_process_req->num_process] should be in range [%d <= OAM_AGENT_MAX_NUM_PROCESS] !", p_oam_agent_spawn_process_req->num_process);
        return OAM_AGNT_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        oam_agnt_counter_t loop;
        for (loop = 0; loop < p_oam_agent_spawn_process_req->num_process; loop++)
        {
            if (OAM_AGNT_FAILURE == oam_agnt_parse_oam_agent_process_spawn_list(
                &p_oam_agent_spawn_process_req->process_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_AGNT_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    oam_agnt_cp_unpack_UInt16(&p_oam_agent_spawn_process_req->interval, p_src + *p_length_read, "interval");
    *p_length_read += sizeof(UInt16);

    if (*p_length_read > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    return OAM_AGNT_SUCCESS;
}

static
oam_agnt_return_et
oam_agnt_parse_oam_agent_process_args
(
    oam_agent_process_args_t *p_oam_agent_process_args,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_agnt_memset(p_oam_agent_process_args, 0, sizeof(oam_agent_process_args_t));

    /* This function parses oam_agent_process_args */
    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "Parsing oam_agent_process_args");


    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    oam_agnt_cp_unpack_UInt8(&p_oam_agent_process_args->len, p_src + *p_length_read, "len");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_oam_agent_process_args->len > OAM_AGENT_PROCESS_ARG_LIST))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Parameter [p_oam_agent_process_args->len] should be in range [%d <= OAM_AGENT_PROCESS_ARG_LIST] !", p_oam_agent_process_args->len);
        return OAM_AGNT_FAILURE;
    }

    if (*p_length_read + (SInt32)(p_oam_agent_process_args->len * sizeof(p_oam_agent_process_args->name[0])) > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        oam_agnt_counter_t loop;
        for (loop = 0; loop < p_oam_agent_process_args->len; loop++)
        {
            oam_agnt_cp_unpack_Char8((void*)&p_oam_agent_process_args->name[loop], (void*)(p_src + *p_length_read), "name[]");
            *p_length_read += sizeof(Char8);
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    return OAM_AGNT_SUCCESS;
}

static
oam_agnt_return_et
oam_agnt_parse_oam_agent_process_spawn_list
(
    oam_agent_process_spawn_list_t *p_oam_agent_process_spawn_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_agnt_memset(p_oam_agent_process_spawn_list, 0, sizeof(oam_agent_process_spawn_list_t));

    /* This function parses oam_agent_process_spawn_list */
    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "Parsing oam_agent_process_spawn_list");


    if (OAM_AGNT_FAILURE == oam_agnt_parse_oam_agent_process_name(
        &p_oam_agent_process_spawn_list->process_name,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_AGNT_FAILURE;
    }

    *p_length_read += length_read;


    if (OAM_AGNT_FAILURE == oam_agnt_parse_oam_agent_process_args(
        &p_oam_agent_process_spawn_list->arg_list,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_AGNT_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    return OAM_AGNT_SUCCESS;
}

oam_agnt_return_et
oam_agnt_parse_agent_oam_spawn_process_resp
(
    agent_oam_spawn_process_resp_t *p_agent_oam_spawn_process_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_agnt_memset(p_agent_oam_spawn_process_resp, 0, sizeof(agent_oam_spawn_process_resp_t));

    /* This function parses agent_oam_spawn_process_resp */
    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "Parsing agent_oam_spawn_process_resp");

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    oam_agnt_cp_unpack_UInt8(&p_agent_oam_spawn_process_resp->num_failed_process, p_src + *p_length_read, "num_failed_process");
    *p_length_read += sizeof(UInt8);


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        oam_agnt_counter_t loop;
        for (loop = 0; loop < p_agent_oam_spawn_process_resp->num_failed_process; loop++)
        {
            if (OAM_AGNT_FAILURE == oam_agnt_parse_oam_agent_process_spawn_status_list(
                &p_agent_oam_spawn_process_resp->process_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_AGNT_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    return OAM_AGNT_SUCCESS;
}

static
oam_agnt_return_et
oam_agnt_parse_oam_agent_process_spawn_status_list
(
    oam_agent_process_spawn_status_list_t *p_oam_agent_process_spawn_status_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_agnt_memset(p_oam_agent_process_spawn_status_list, 0, sizeof(oam_agent_process_spawn_status_list_t));

    /* This function parses oam_agent_process_spawn_status_list */
    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "Parsing oam_agent_process_spawn_status_list");


    if (OAM_AGNT_FAILURE == oam_agnt_parse_oam_agent_process_name(
        &p_oam_agent_process_spawn_status_list->failed_process_name,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_AGNT_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    oam_agnt_cp_unpack_UInt32(&p_oam_agent_process_spawn_status_list->err, p_src + *p_length_read, "err");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    return OAM_AGNT_SUCCESS;
}

oam_agnt_return_et
oam_agnt_parse_oam_agent_kill_process_req
(
    oam_agent_kill_process_req_t *p_oam_agent_kill_process_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_agnt_memset(p_oam_agent_kill_process_req, 0, sizeof(oam_agent_kill_process_req_t));

    /* This function parses oam_agent_kill_process_req */
    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "Parsing oam_agent_kill_process_req");

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    oam_agnt_cp_unpack_UInt8(&p_oam_agent_kill_process_req->num_process, p_src + *p_length_read, "num_process");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_oam_agent_kill_process_req->num_process > OAM_AGENT_MAX_NUM_PROCESS))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Parameter [p_oam_agent_kill_process_req->num_process] should be in range [%d <= OAM_AGENT_MAX_NUM_PROCESS] !", p_oam_agent_kill_process_req->num_process);
        return OAM_AGNT_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        oam_agnt_counter_t loop;
        for (loop = 0; loop < p_oam_agent_kill_process_req->num_process; loop++)
        {
            if (OAM_AGNT_FAILURE == oam_agnt_parse_oam_agent_kill_process_list(
                &p_oam_agent_kill_process_req->process_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_AGNT_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    return OAM_AGNT_SUCCESS;
}

static
oam_agnt_return_et
oam_agnt_parse_oam_agent_kill_process_list
(
    oam_agent_kill_process_list_t *p_oam_agent_kill_process_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_agnt_memset(p_oam_agent_kill_process_list, 0, sizeof(oam_agent_kill_process_list_t));

    /* This function parses oam_agent_kill_process_list */
    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "Parsing oam_agent_kill_process_list");


    if (OAM_AGNT_FAILURE == oam_agnt_parse_oam_agent_process_name(
        &p_oam_agent_kill_process_list->process_name,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_AGNT_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    return OAM_AGNT_SUCCESS;
}

oam_agnt_return_et
oam_agnt_parse_agent_oam_kill_process_resp
(
    agent_oam_kill_process_resp_t *p_agent_oam_kill_process_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_agnt_memset(p_agent_oam_kill_process_resp, 0, sizeof(agent_oam_kill_process_resp_t));

    /* This function parses agent_oam_kill_process_resp */
    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "Parsing agent_oam_kill_process_resp");

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    oam_agnt_cp_unpack_UInt8(&p_agent_oam_kill_process_resp->num_process, p_src + *p_length_read, "num_process");
    *p_length_read += sizeof(UInt8);

    /* Check for correct range [H - higher boundary] */
    if ((p_agent_oam_kill_process_resp->num_process > OAM_AGENT_MAX_NUM_PROCESS))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Parameter [p_agent_oam_kill_process_resp->num_process] should be in range [%d <= OAM_AGENT_MAX_NUM_PROCESS] !", p_agent_oam_kill_process_resp->num_process);
        return OAM_AGNT_FAILURE;
    }


    /* Parse OCTET_STRING VARIABLE of IEs */
    {
        oam_agnt_counter_t loop;
        for (loop = 0; loop < p_agent_oam_kill_process_resp->num_process; loop++)
        {
            if (OAM_AGNT_FAILURE == oam_agnt_parse_oam_agent_kill_process_status_list(
                &p_agent_oam_kill_process_resp->fail_process_list[loop],
                p_src + *p_length_read,
                length_left - *p_length_read,
                &length_read))
            {
                return OAM_AGNT_FAILURE;
            }

            *p_length_read += length_read;
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    return OAM_AGNT_SUCCESS;
}

static
oam_agnt_return_et
oam_agnt_parse_oam_agent_kill_process_status_list
(
    oam_agent_kill_process_status_list_t *p_oam_agent_kill_process_status_list,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_agnt_memset(p_oam_agent_kill_process_status_list, 0, sizeof(oam_agent_kill_process_status_list_t));

    /* This function parses oam_agent_kill_process_status_list */
    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "Parsing oam_agent_kill_process_status_list");


    if (OAM_AGNT_FAILURE == oam_agnt_parse_oam_agent_process_name(
        &p_oam_agent_kill_process_status_list->failed_process_name,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_AGNT_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    oam_agnt_cp_unpack_UInt32(&p_oam_agent_kill_process_status_list->err, p_src + *p_length_read, "err");
    *p_length_read += sizeof(UInt32);

    if (*p_length_read > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    return OAM_AGNT_SUCCESS;
}

oam_agnt_return_et
oam_agnt_parse_agent_oam_get_logs_resp
(
    agent_oam_get_logs_resp_t *p_agent_oam_get_logs_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_agnt_memset(p_agent_oam_get_logs_resp, 0, sizeof(agent_oam_get_logs_resp_t));

    /* This function parses agent_oam_get_logs_resp */
    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "Parsing agent_oam_get_logs_resp");


    if (OAM_AGNT_FAILURE == oam_agnt_parse_oam_agent_file_name(
        &p_agent_oam_get_logs_resp->filename,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_AGNT_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    return OAM_AGNT_SUCCESS;
}

static
oam_agnt_return_et
oam_agnt_parse_oam_agent_file_name
(
    oam_agent_file_name_t *p_oam_agent_file_name,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{

    *p_length_read = 0;

    oam_agnt_memset(p_oam_agent_file_name, 0, sizeof(oam_agent_file_name_t));

    /* This function parses oam_agent_file_name */
    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "Parsing oam_agent_file_name");


    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    oam_agnt_cp_unpack_UInt16(&p_oam_agent_file_name->len, p_src + *p_length_read, "len");
    *p_length_read += sizeof(UInt16);

    /* Check for correct range [H - higher boundary] */
    if ((p_oam_agent_file_name->len > OAM_AGENT_MAX_FILE_NAME))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Parameter [p_oam_agent_file_name->len] should be in range [%d <= OAM_AGENT_MAX_FILE_NAME] !", p_oam_agent_file_name->len);
        return OAM_AGNT_FAILURE;
    }

    if (*p_length_read + (SInt32)(p_oam_agent_file_name->len * sizeof(p_oam_agent_file_name->name[0])) > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    /* Parse OCTET_STRING VARIABLE of basic type elements */
    {
        oam_agnt_counter_t loop;
        for (loop = 0; loop < p_oam_agent_file_name->len; loop++)
        {
            oam_agnt_cp_unpack_Char8((void*)&p_oam_agent_file_name->name[loop], (void*)(p_src + *p_length_read), "name[]");
            *p_length_read += sizeof(Char8);
        }
    }

    if (*p_length_read > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    return OAM_AGNT_SUCCESS;
}

oam_agnt_return_et
oam_agnt_parse_oam_agent_get_logs_cnf
(
    oam_agent_get_logs_cnf_t *p_oam_agent_get_logs_cnf,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_agnt_memset(p_oam_agent_get_logs_cnf, 0, sizeof(oam_agent_get_logs_cnf_t));

    /* This function parses oam_agent_get_logs_cnf */
    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "Parsing oam_agent_get_logs_cnf");


    if (OAM_AGNT_FAILURE == oam_agnt_parse_oam_agent_file_name(
        &p_oam_agent_get_logs_cnf->filename,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_AGNT_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    return OAM_AGNT_SUCCESS;
}

oam_agnt_return_et
oam_agnt_parse_agent_oam_im_ok_ind
(
    agent_oam_im_ok_ind_t *p_agent_oam_im_ok_ind,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
)
{
    SInt32 length_read = 0;

    *p_length_read = 0;

    oam_agnt_memset(p_agent_oam_im_ok_ind, 0, sizeof(agent_oam_im_ok_ind_t));

    /* This function parses agent_oam_im_ok_ind */
    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "Parsing agent_oam_im_ok_ind");

    if (*p_length_read + (SInt32)sizeof(UInt64) > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    oam_agnt_cp_unpack_UInt64(&p_agent_oam_im_ok_ind->cksum, p_src + *p_length_read, "cksum");
    *p_length_read += sizeof(UInt64);


    if (OAM_AGNT_FAILURE == oam_agnt_parse_oam_agent_dir_path(
        &p_agent_oam_im_ok_ind->bin_path,
        p_src + *p_length_read,
        length_left - *p_length_read,
        &length_read))
    {
        return OAM_AGNT_FAILURE;
    }

    *p_length_read += length_read;

    if (*p_length_read > length_left)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Incoming message damaged!");
        return OAM_AGNT_FAILURE;
    }

    return OAM_AGNT_SUCCESS;
}

