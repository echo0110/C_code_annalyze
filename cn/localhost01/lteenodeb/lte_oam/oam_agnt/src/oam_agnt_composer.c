/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2017 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: oam_agnt_composer.c $
 *
 *******************************************************************************
 *
 * File Description: OAM AGENT composer files for all OAM AGENT composer functions.
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
oam_agnt_length_t
oam_agnt_get_oam_agent_process_name_len
(
    oam_agent_process_name_t *p_oam_agent_process_name
);

static
oam_agnt_return_et
oam_agnt_compose_oam_agent_process_name
(
    UInt8  **pp_buffer,
    oam_agent_process_name_t *p_oam_agent_process_name
);

static
oam_agnt_length_t
oam_agnt_get_oam_agent_process_info_list_len
(
    oam_agent_process_info_list_t *p_oam_agent_process_info_list
);

static
oam_agnt_return_et
oam_agnt_compose_oam_agent_process_info_list
(
    UInt8  **pp_buffer,
    oam_agent_process_info_list_t *p_oam_agent_process_info_list
);

static
oam_agnt_length_t
oam_agnt_get_oam_agent_dir_path_len
(
    oam_agent_dir_path_t *p_oam_agent_dir_path
);

static
oam_agnt_return_et
oam_agnt_compose_oam_agent_dir_path
(
    UInt8  **pp_buffer,
    oam_agent_dir_path_t *p_oam_agent_dir_path
);

static
oam_agnt_length_t
oam_agnt_get_oam_agent_process_args_len
(
    oam_agent_process_args_t *p_oam_agent_process_args
);

static
oam_agnt_return_et
oam_agnt_compose_oam_agent_process_args
(
    UInt8  **pp_buffer,
    oam_agent_process_args_t *p_oam_agent_process_args
);

static
oam_agnt_length_t
oam_agnt_get_oam_agent_process_spawn_list_len
(
    oam_agent_process_spawn_list_t *p_oam_agent_process_spawn_list
);

static
oam_agnt_return_et
oam_agnt_compose_oam_agent_process_spawn_list
(
    UInt8  **pp_buffer,
    oam_agent_process_spawn_list_t *p_oam_agent_process_spawn_list
);

static
oam_agnt_length_t
oam_agnt_get_oam_agent_process_spawn_status_list_len
(
    oam_agent_process_spawn_status_list_t *p_oam_agent_process_spawn_status_list
);

static
oam_agnt_return_et
oam_agnt_compose_oam_agent_process_spawn_status_list
(
    UInt8  **pp_buffer,
    oam_agent_process_spawn_status_list_t *p_oam_agent_process_spawn_status_list
);

static
oam_agnt_length_t
oam_agnt_get_oam_agent_kill_process_list_len
(
    oam_agent_kill_process_list_t *p_oam_agent_kill_process_list
);

static
oam_agnt_return_et
oam_agnt_compose_oam_agent_kill_process_list
(
    UInt8  **pp_buffer,
    oam_agent_kill_process_list_t *p_oam_agent_kill_process_list
);

static
oam_agnt_length_t
oam_agnt_get_oam_agent_kill_process_status_list_len
(
    oam_agent_kill_process_status_list_t *p_oam_agent_kill_process_status_list
);

static
oam_agnt_return_et
oam_agnt_compose_oam_agent_kill_process_status_list
(
    UInt8  **pp_buffer,
    oam_agent_kill_process_status_list_t *p_oam_agent_kill_process_status_list
);

static
oam_agnt_length_t
oam_agnt_get_oam_agent_file_name_len
(
    oam_agent_file_name_t *p_oam_agent_file_name
);

static
oam_agnt_return_et
oam_agnt_compose_oam_agent_file_name
(
    UInt8  **pp_buffer,
    oam_agent_file_name_t *p_oam_agent_file_name
);


/*****************************************************************************/
/*                      Functions implementations                            */
/*****************************************************************************/

oam_agnt_length_t
oam_agnt_get_agent_oam_init_ind_len
(
    agent_oam_init_ind_t *p_agent_oam_init_ind
)
{
    oam_agnt_length_t length = 0;


    /* Get length of parameter of basic type */
    length += sizeof(p_agent_oam_init_ind->num_process);

    /* Check for correct range [H - higher boundary] */
    if ((p_agent_oam_init_ind->num_process > OAM_AGENT_MAX_NUM_PROCESS))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Parameter [p_agent_oam_init_ind->num_process] should be in range [%d <= OAM_AGENT_MAX_NUM_PROCESS] !", p_agent_oam_init_ind->num_process);
        return OAM_AGNT_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        oam_agnt_counter_t loop;
        for (loop = 0; loop < p_agent_oam_init_ind->num_process; loop++)
        {
            length += oam_agnt_get_oam_agent_process_info_list_len(&p_agent_oam_init_ind->process_list[loop]);
        }
    }

    /* Get length of IE */
    length += oam_agnt_get_oam_agent_dir_path_len(&p_agent_oam_init_ind->bin_path);

    return length;
}

oam_agnt_return_et
oam_agnt_compose_agent_oam_init_ind
(
    UInt8  **pp_buffer,
    agent_oam_init_ind_t *p_agent_oam_init_ind
)
{

    /* Check for correct range [H - higher boundary] */
    if ((p_agent_oam_init_ind->num_process > OAM_AGENT_MAX_NUM_PROCESS))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Parameter [p_agent_oam_init_ind->num_process] should be in range [%d <= OAM_AGENT_MAX_NUM_PROCESS] !", p_agent_oam_init_ind->num_process);
        return OAM_AGNT_FAILURE;
    }

    /* Compose parameter of basic type */
    oam_agnt_cp_pack_UInt8(*pp_buffer, &p_agent_oam_init_ind->num_process, "num_process");
    *pp_buffer += sizeof(p_agent_oam_init_ind->num_process);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        oam_agnt_counter_t loop;
        for (loop = 0; loop < p_agent_oam_init_ind->num_process; loop++)
        {
            if (OAM_AGNT_FAILURE == oam_agnt_compose_oam_agent_process_info_list(pp_buffer, &p_agent_oam_init_ind->process_list[loop]))
            {
                return OAM_AGNT_FAILURE;
            }
        }
    }

    /* Compose IE */
    if (OAM_AGNT_FAILURE == oam_agnt_compose_oam_agent_dir_path(pp_buffer, &p_agent_oam_init_ind->bin_path))
    {
        return OAM_AGNT_FAILURE;
    }

    return OAM_AGNT_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_agnt_send_agent_oam_init_ind
*
*   DESCRIPTION:
*       This function constructs and sends AGENT_OAM_INIT_IND message
*
*   RETURNS:
*       OAM_AGNT_FAILURE     - Indicates failed message processing
*       OAM_AGNT_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_agnt_return_et
oam_agnt_send_agent_oam_init_ind
(
    agent_oam_init_ind_t  *p_agent_oam_init_ind,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dest_module_id,  /* Destination module identifier */
    UInt16                 transaction_id,  /* Interface transaction identifier */
    UInt8                  instance_id
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_agnt_msg;
    oam_agnt_error_code_et err = OAM_AGNT_NO_ERROR;

    /* Get API length */
    msg_length = oam_agnt_get_agent_oam_init_ind_len(p_agent_oam_init_ind);

    OAM_AGNT_LOG(AGNT, OAM_AGNT_INFO, "Src(%u)->Dst(%u):AGENT_OAM_INIT_IND Instance %u", src_module_id, dest_module_id, instance_id);
	
	if(msg_length == OAM_AGNT_FAILURE)
	{
 		return OAM_AGNT_FAILURE;
	}	
    msg_api_length = msg_length + OAM_AGNT_API_HEADER_SIZE + OAM_AGNT_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    if (OAM_AGNT_FAILURE == oam_agnt_mem_alloc(msg_api_length, (void*)&p_msg, &err))
    {
        /* Not enough memory */
        return OAM_AGNT_FAILURE;
    }

    oam_agnt_memset(p_msg, 0, msg_api_length);
    p_agnt_msg = p_msg;

    /* Fill CSPL header */
    oam_agnt_construct_api_header(p_agnt_msg, OAM_AGNT_VERSION, src_module_id,
        dest_module_id, AGENT_OAM_INIT_IND, msg_api_length);

    /* Fill interface header */
    p_agnt_msg = p_agnt_msg + OAM_AGNT_API_HEADER_SIZE;
    oam_agnt_construct_interface_api_header(p_agnt_msg, transaction_id, src_module_id,
        dest_module_id, AGENT_OAM_INIT_IND, msg_length,instance_id);

    /* Fill AGENT_OAM_INIT_IND message */
    p_agnt_msg = p_agnt_msg + OAM_AGNT_INTERFACE_API_HEADER_SIZE;
    if (OAM_AGNT_FAILURE == oam_agnt_compose_agent_oam_init_ind(&p_agnt_msg, p_agent_oam_init_ind))
    {
        oam_agnt_mem_free(p_msg, &err);

        return OAM_AGNT_FAILURE;
    }

    if((p_agnt_msg - msg_api_length) != p_msg)
    {
        oam_agnt_mem_free(p_msg, &err);

        return OAM_AGNT_FAILURE;
    }

    /* Send message to */
    oam_agnt_send_message(p_msg, dest_module_id);

    return OAM_AGNT_SUCCESS;
}

static
oam_agnt_length_t
oam_agnt_get_oam_agent_process_name_len
(
    oam_agent_process_name_t *p_oam_agent_process_name
)
{
    oam_agnt_length_t length = 0;


    /* Get length of parameter of basic type */
    length += sizeof(p_oam_agent_process_name->len);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_oam_agent_process_name->len * sizeof(p_oam_agent_process_name->name[0]));

    return length;
}

static
oam_agnt_return_et
oam_agnt_compose_oam_agent_process_name
(
    UInt8  **pp_buffer,
    oam_agent_process_name_t *p_oam_agent_process_name
)
{

    /* Check for correct range [H - higher boundary] */
    if ((p_oam_agent_process_name->len > OAM_AGENT_MAX_PROCESS_NAME))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Parameter [p_oam_agent_process_name->len] should be in range [%d <= OAM_AGENT_MAX_PROCESS_NAME] !", p_oam_agent_process_name->len);
        return OAM_AGNT_FAILURE;
    }

    /* Compose parameter of basic type */
    oam_agnt_cp_pack_UInt8(*pp_buffer, &p_oam_agent_process_name->len, "len");
    *pp_buffer += sizeof(p_oam_agent_process_name->len);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        oam_agnt_counter_t loop;
        for (loop = 0; loop < p_oam_agent_process_name->len; loop++)
        {
            oam_agnt_cp_pack_Char8(*pp_buffer, &p_oam_agent_process_name->name[loop], "name[]");
            *pp_buffer += sizeof(Char8);
        }
    }

    return OAM_AGNT_SUCCESS;
}

static
oam_agnt_length_t
oam_agnt_get_oam_agent_process_info_list_len
(
    oam_agent_process_info_list_t *p_oam_agent_process_info_list
)
{
    oam_agnt_length_t length = 0;



    /* Get length of IE */
    length += oam_agnt_get_oam_agent_process_name_len(&p_oam_agent_process_info_list->process_name);
    /* Get length of parameter of basic type */
    length += sizeof(p_oam_agent_process_info_list->cksum);

    return length;
}

static
oam_agnt_return_et
oam_agnt_compose_oam_agent_process_info_list
(
    UInt8  **pp_buffer,
    oam_agent_process_info_list_t *p_oam_agent_process_info_list
)
{

    /* Compose IE */
    if (OAM_AGNT_FAILURE == oam_agnt_compose_oam_agent_process_name(pp_buffer, &p_oam_agent_process_info_list->process_name))
    {
        return OAM_AGNT_FAILURE;
    }

    /* Compose parameter of basic type */
    oam_agnt_cp_pack_UInt64(*pp_buffer, &p_oam_agent_process_info_list->cksum, "cksum");
    *pp_buffer += sizeof(p_oam_agent_process_info_list->cksum);

    return OAM_AGNT_SUCCESS;
}

static
oam_agnt_length_t
oam_agnt_get_oam_agent_dir_path_len
(
    oam_agent_dir_path_t *p_oam_agent_dir_path
)
{
    oam_agnt_length_t length = 0;


    /* Get length of parameter of basic type */
    length += sizeof(p_oam_agent_dir_path->len);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_oam_agent_dir_path->len * sizeof(p_oam_agent_dir_path->name[0]));

    return length;
}

static
oam_agnt_return_et
oam_agnt_compose_oam_agent_dir_path
(
    UInt8  **pp_buffer,
    oam_agent_dir_path_t *p_oam_agent_dir_path
)
{

    /* Check for correct range [H - higher boundary] */
    if ((p_oam_agent_dir_path->len > OAM_AGENT_MAX_DIR_PATH))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Parameter [p_oam_agent_dir_path->len] should be in range [%d <= OAM_AGENT_MAX_DIR_PATH] !", p_oam_agent_dir_path->len);
        return OAM_AGNT_FAILURE;
    }

    /* Compose parameter of basic type */
    oam_agnt_cp_pack_UInt16(*pp_buffer, &p_oam_agent_dir_path->len, "len");
    *pp_buffer += sizeof(p_oam_agent_dir_path->len);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        oam_agnt_counter_t loop;
        for (loop = 0; loop < p_oam_agent_dir_path->len; loop++)
        {
            oam_agnt_cp_pack_Char8(*pp_buffer, &p_oam_agent_dir_path->name[loop], "name[]");
            *pp_buffer += sizeof(Char8);
        }
    }

    return OAM_AGNT_SUCCESS;
}

oam_agnt_length_t
oam_agnt_get_oam_agent_spawn_process_req_len
(
    oam_agent_spawn_process_req_t *p_oam_agent_spawn_process_req
)
{
    oam_agnt_length_t length = 0;


    /* Get length of parameter of basic type */
    length += sizeof(p_oam_agent_spawn_process_req->num_process);

    /* Check for correct range [H - higher boundary] */
    if ((p_oam_agent_spawn_process_req->num_process > OAM_AGENT_MAX_NUM_PROCESS))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Parameter [p_oam_agent_spawn_process_req->num_process] should be in range [%d <= OAM_AGENT_MAX_NUM_PROCESS] !", p_oam_agent_spawn_process_req->num_process);
        return OAM_AGNT_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        oam_agnt_counter_t loop;
        for (loop = 0; loop < p_oam_agent_spawn_process_req->num_process; loop++)
        {
            length += oam_agnt_get_oam_agent_process_spawn_list_len(&p_oam_agent_spawn_process_req->process_list[loop]);
        }
    }
    /* Get length of parameter of basic type */
    length += sizeof(p_oam_agent_spawn_process_req->interval);

    return length;
}

oam_agnt_return_et
oam_agnt_compose_oam_agent_spawn_process_req
(
    UInt8  **pp_buffer,
    oam_agent_spawn_process_req_t *p_oam_agent_spawn_process_req
)
{

    /* Check for correct range [H - higher boundary] */
    if ((p_oam_agent_spawn_process_req->num_process > OAM_AGENT_MAX_NUM_PROCESS))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Parameter [p_oam_agent_spawn_process_req->num_process] should be in range [%d <= OAM_AGENT_MAX_NUM_PROCESS] !", p_oam_agent_spawn_process_req->num_process);
        return OAM_AGNT_FAILURE;
    }

    /* Compose parameter of basic type */
    oam_agnt_cp_pack_UInt8(*pp_buffer, &p_oam_agent_spawn_process_req->num_process, "num_process");
    *pp_buffer += sizeof(p_oam_agent_spawn_process_req->num_process);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        oam_agnt_counter_t loop;
        for (loop = 0; loop < p_oam_agent_spawn_process_req->num_process; loop++)
        {
            if (OAM_AGNT_FAILURE == oam_agnt_compose_oam_agent_process_spawn_list(pp_buffer, &p_oam_agent_spawn_process_req->process_list[loop]))
            {
                return OAM_AGNT_FAILURE;
            }
        }
    }

    /* Compose parameter of basic type */
    oam_agnt_cp_pack_UInt16(*pp_buffer, &p_oam_agent_spawn_process_req->interval, "interval");
    *pp_buffer += sizeof(p_oam_agent_spawn_process_req->interval);

    return OAM_AGNT_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_agnt_send_oam_agent_spawn_process_req
*
*   DESCRIPTION:
*       This function constructs and sends OAM_AGENT_SPAWN_PROCESS_REQ message
*
*   RETURNS:
*       OAM_AGNT_FAILURE     - Indicates failed message processing
*       OAM_AGNT_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_agnt_return_et
oam_agnt_send_oam_agent_spawn_process_req
(
    oam_agent_spawn_process_req_t  *p_oam_agent_spawn_process_req,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dest_module_id,  /* Destination module identifier */
    UInt16                 transaction_id,  /* Interface transaction identifier */
    UInt8                  instance_id
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_agnt_msg;
    oam_agnt_error_code_et err = OAM_AGNT_NO_ERROR;

    /* Get API length */
    msg_length = oam_agnt_get_oam_agent_spawn_process_req_len(p_oam_agent_spawn_process_req);

    OAM_AGNT_LOG(AGNT, OAM_AGNT_INFO, "Src(%u)->Dst(%u):OAM_AGENT_SPAWN_PROCESS_REQ Instance %u", src_module_id, dest_module_id, instance_id);
	
	if(msg_length == OAM_AGNT_FAILURE)
	{
 		return OAM_AGNT_FAILURE;
	}	
    msg_api_length = msg_length + OAM_AGNT_API_HEADER_SIZE + OAM_AGNT_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    if (OAM_AGNT_FAILURE == oam_agnt_mem_alloc(msg_api_length, (void*)&p_msg, &err))
    {
        /* Not enough memory */
        return OAM_AGNT_FAILURE;
    }

    oam_agnt_memset(p_msg, 0, msg_api_length);
    p_agnt_msg = p_msg;

    /* Fill CSPL header */
    oam_agnt_construct_api_header(p_agnt_msg, OAM_AGNT_VERSION, src_module_id,
        dest_module_id, OAM_AGENT_SPAWN_PROCESS_REQ, msg_api_length);

    /* Fill interface header */
    p_agnt_msg = p_agnt_msg + OAM_AGNT_API_HEADER_SIZE;
    oam_agnt_construct_interface_api_header(p_agnt_msg, transaction_id, src_module_id,
        dest_module_id, OAM_AGENT_SPAWN_PROCESS_REQ, msg_length,instance_id);

    /* Fill OAM_AGENT_SPAWN_PROCESS_REQ message */
    p_agnt_msg = p_agnt_msg + OAM_AGNT_INTERFACE_API_HEADER_SIZE;
    if (OAM_AGNT_FAILURE == oam_agnt_compose_oam_agent_spawn_process_req(&p_agnt_msg, p_oam_agent_spawn_process_req))
    {
        oam_agnt_mem_free(p_msg, &err);

        return OAM_AGNT_FAILURE;
    }

    if((p_agnt_msg - msg_api_length) != p_msg)
    {
        oam_agnt_mem_free(p_msg, &err);

        return OAM_AGNT_FAILURE;
    }

    /* Send message to */
    oam_agnt_send_message(p_msg, dest_module_id);

    return OAM_AGNT_SUCCESS;
}

static
oam_agnt_length_t
oam_agnt_get_oam_agent_process_args_len
(
    oam_agent_process_args_t *p_oam_agent_process_args
)
{
    oam_agnt_length_t length = 0;


    /* Get length of parameter of basic type */
    length += sizeof(p_oam_agent_process_args->len);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_oam_agent_process_args->len * sizeof(p_oam_agent_process_args->name[0]));

    return length;
}

static
oam_agnt_return_et
oam_agnt_compose_oam_agent_process_args
(
    UInt8  **pp_buffer,
    oam_agent_process_args_t *p_oam_agent_process_args
)
{

    /* Check for correct range [H - higher boundary] */
    if ((p_oam_agent_process_args->len > OAM_AGENT_PROCESS_ARG_LIST))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Parameter [p_oam_agent_process_args->len] should be in range [%d <= OAM_AGENT_PROCESS_ARG_LIST] !", p_oam_agent_process_args->len);
        return OAM_AGNT_FAILURE;
    }

    /* Compose parameter of basic type */
    oam_agnt_cp_pack_UInt8(*pp_buffer, &p_oam_agent_process_args->len, "len");
    *pp_buffer += sizeof(p_oam_agent_process_args->len);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        oam_agnt_counter_t loop;
        for (loop = 0; loop < p_oam_agent_process_args->len; loop++)
        {
            oam_agnt_cp_pack_Char8(*pp_buffer, &p_oam_agent_process_args->name[loop], "name[]");
            *pp_buffer += sizeof(Char8);
        }
    }

    return OAM_AGNT_SUCCESS;
}

static
oam_agnt_length_t
oam_agnt_get_oam_agent_process_spawn_list_len
(
    oam_agent_process_spawn_list_t *p_oam_agent_process_spawn_list
)
{
    oam_agnt_length_t length = 0;



    /* Get length of IE */
    length += oam_agnt_get_oam_agent_process_name_len(&p_oam_agent_process_spawn_list->process_name);

    /* Get length of IE */
    length += oam_agnt_get_oam_agent_process_args_len(&p_oam_agent_process_spawn_list->arg_list);

    return length;
}

static
oam_agnt_return_et
oam_agnt_compose_oam_agent_process_spawn_list
(
    UInt8  **pp_buffer,
    oam_agent_process_spawn_list_t *p_oam_agent_process_spawn_list
)
{

    /* Compose IE */
    if (OAM_AGNT_FAILURE == oam_agnt_compose_oam_agent_process_name(pp_buffer, &p_oam_agent_process_spawn_list->process_name))
    {
        return OAM_AGNT_FAILURE;
    }

    /* Compose IE */
    if (OAM_AGNT_FAILURE == oam_agnt_compose_oam_agent_process_args(pp_buffer, &p_oam_agent_process_spawn_list->arg_list))
    {
        return OAM_AGNT_FAILURE;
    }

    return OAM_AGNT_SUCCESS;
}

oam_agnt_length_t
oam_agnt_get_agent_oam_spawn_process_resp_len
(
    agent_oam_spawn_process_resp_t *p_agent_oam_spawn_process_resp
)
{
    oam_agnt_length_t length = 0;


    /* Get length of parameter of basic type */
    length += sizeof(p_agent_oam_spawn_process_resp->num_failed_process);

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        oam_agnt_counter_t loop;
        for (loop = 0; loop < p_agent_oam_spawn_process_resp->num_failed_process; loop++)
        {
            length += oam_agnt_get_oam_agent_process_spawn_status_list_len(&p_agent_oam_spawn_process_resp->process_list[loop]);
        }
    }

    return length;
}

oam_agnt_return_et
oam_agnt_compose_agent_oam_spawn_process_resp
(
    UInt8  **pp_buffer,
    agent_oam_spawn_process_resp_t *p_agent_oam_spawn_process_resp
)
{

    /* Compose parameter of basic type */
    oam_agnt_cp_pack_UInt8(*pp_buffer, &p_agent_oam_spawn_process_resp->num_failed_process, "num_failed_process");
    *pp_buffer += sizeof(p_agent_oam_spawn_process_resp->num_failed_process);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        oam_agnt_counter_t loop;
        for (loop = 0; loop < p_agent_oam_spawn_process_resp->num_failed_process; loop++)
        {
            if (OAM_AGNT_FAILURE == oam_agnt_compose_oam_agent_process_spawn_status_list(pp_buffer, &p_agent_oam_spawn_process_resp->process_list[loop]))
            {
                return OAM_AGNT_FAILURE;
            }
        }
    }

    return OAM_AGNT_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_agnt_send_agent_oam_spawn_process_resp
*
*   DESCRIPTION:
*       This function constructs and sends AGENT_OAM_SPAWN_PROCESS_RESP message
*
*   RETURNS:
*       OAM_AGNT_FAILURE     - Indicates failed message processing
*       OAM_AGNT_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_agnt_return_et
oam_agnt_send_agent_oam_spawn_process_resp
(
    agent_oam_spawn_process_resp_t  *p_agent_oam_spawn_process_resp,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dest_module_id,  /* Destination module identifier */
    UInt16                 transaction_id,  /* Interface transaction identifier */
    UInt8                  instance_id
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_agnt_msg;
    oam_agnt_error_code_et err = OAM_AGNT_NO_ERROR;

    /* Get API length */
    msg_length = oam_agnt_get_agent_oam_spawn_process_resp_len(p_agent_oam_spawn_process_resp);

    OAM_AGNT_LOG(AGNT, OAM_AGNT_INFO, "Src(%u)->Dst(%u):AGENT_OAM_SPAWN_PROCESS_RESP Instance %u", src_module_id, dest_module_id, instance_id);
	
	if(msg_length == OAM_AGNT_FAILURE)
	{
 		return OAM_AGNT_FAILURE;
	}	
    msg_api_length = msg_length + OAM_AGNT_API_HEADER_SIZE + OAM_AGNT_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    if (OAM_AGNT_FAILURE == oam_agnt_mem_alloc(msg_api_length, (void*)&p_msg, &err))
    {
        /* Not enough memory */
        return OAM_AGNT_FAILURE;
    }

    oam_agnt_memset(p_msg, 0, msg_api_length);
    p_agnt_msg = p_msg;

    /* Fill CSPL header */
    oam_agnt_construct_api_header(p_agnt_msg, OAM_AGNT_VERSION, src_module_id,
        dest_module_id, AGENT_OAM_SPAWN_PROCESS_RESP, msg_api_length);

    /* Fill interface header */
    p_agnt_msg = p_agnt_msg + OAM_AGNT_API_HEADER_SIZE;
    oam_agnt_construct_interface_api_header(p_agnt_msg, transaction_id, src_module_id,
        dest_module_id, AGENT_OAM_SPAWN_PROCESS_RESP, msg_length,instance_id);

    /* Fill AGENT_OAM_SPAWN_PROCESS_RESP message */
    p_agnt_msg = p_agnt_msg + OAM_AGNT_INTERFACE_API_HEADER_SIZE;
    if (OAM_AGNT_FAILURE == oam_agnt_compose_agent_oam_spawn_process_resp(&p_agnt_msg, p_agent_oam_spawn_process_resp))
    {
        oam_agnt_mem_free(p_msg, &err);

        return OAM_AGNT_FAILURE;
    }

    if((p_agnt_msg - msg_api_length) != p_msg)
    {
        oam_agnt_mem_free(p_msg, &err);

        return OAM_AGNT_FAILURE;
    }

    /* Send message to */
    oam_agnt_send_message(p_msg, dest_module_id);

    return OAM_AGNT_SUCCESS;
}

static
oam_agnt_length_t
oam_agnt_get_oam_agent_process_spawn_status_list_len
(
    oam_agent_process_spawn_status_list_t *p_oam_agent_process_spawn_status_list
)
{
    oam_agnt_length_t length = 0;



    /* Get length of IE */
    length += oam_agnt_get_oam_agent_process_name_len(&p_oam_agent_process_spawn_status_list->failed_process_name);
    /* Get length of parameter of basic type */
    length += sizeof(p_oam_agent_process_spawn_status_list->err);

    return length;
}

static
oam_agnt_return_et
oam_agnt_compose_oam_agent_process_spawn_status_list
(
    UInt8  **pp_buffer,
    oam_agent_process_spawn_status_list_t *p_oam_agent_process_spawn_status_list
)
{

    /* Compose IE */
    if (OAM_AGNT_FAILURE == oam_agnt_compose_oam_agent_process_name(pp_buffer, &p_oam_agent_process_spawn_status_list->failed_process_name))
    {
        return OAM_AGNT_FAILURE;
    }

    /* Compose parameter of basic type */
    oam_agnt_cp_pack_UInt32(*pp_buffer, &p_oam_agent_process_spawn_status_list->err, "err");
    *pp_buffer += sizeof(p_oam_agent_process_spawn_status_list->err);

    return OAM_AGNT_SUCCESS;
}

oam_agnt_length_t
oam_agnt_get_oam_agent_kill_process_req_len
(
    oam_agent_kill_process_req_t *p_oam_agent_kill_process_req
)
{
    oam_agnt_length_t length = 0;


    /* Get length of parameter of basic type */
    length += sizeof(p_oam_agent_kill_process_req->num_process);

    /* Check for correct range [H - higher boundary] */
    if ((p_oam_agent_kill_process_req->num_process > OAM_AGENT_MAX_NUM_PROCESS))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Parameter [p_oam_agent_kill_process_req->num_process] should be in range [%d <= OAM_AGENT_MAX_NUM_PROCESS] !", p_oam_agent_kill_process_req->num_process);
        return OAM_AGNT_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        oam_agnt_counter_t loop;
        for (loop = 0; loop < p_oam_agent_kill_process_req->num_process; loop++)
        {
            length += oam_agnt_get_oam_agent_kill_process_list_len(&p_oam_agent_kill_process_req->process_list[loop]);
        }
    }

    return length;
}

oam_agnt_return_et
oam_agnt_compose_oam_agent_kill_process_req
(
    UInt8  **pp_buffer,
    oam_agent_kill_process_req_t *p_oam_agent_kill_process_req
)
{

    /* Check for correct range [H - higher boundary] */
    if ((p_oam_agent_kill_process_req->num_process > OAM_AGENT_MAX_NUM_PROCESS))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Parameter [p_oam_agent_kill_process_req->num_process] should be in range [%d <= OAM_AGENT_MAX_NUM_PROCESS] !", p_oam_agent_kill_process_req->num_process);
        return OAM_AGNT_FAILURE;
    }

    /* Compose parameter of basic type */
    oam_agnt_cp_pack_UInt8(*pp_buffer, &p_oam_agent_kill_process_req->num_process, "num_process");
    *pp_buffer += sizeof(p_oam_agent_kill_process_req->num_process);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        oam_agnt_counter_t loop;
        for (loop = 0; loop < p_oam_agent_kill_process_req->num_process; loop++)
        {
            if (OAM_AGNT_FAILURE == oam_agnt_compose_oam_agent_kill_process_list(pp_buffer, &p_oam_agent_kill_process_req->process_list[loop]))
            {
                return OAM_AGNT_FAILURE;
            }
        }
    }

    return OAM_AGNT_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_agnt_send_oam_agent_kill_process_req
*
*   DESCRIPTION:
*       This function constructs and sends OAM_AGENT_KILL_PROCESS_REQ message
*
*   RETURNS:
*       OAM_AGNT_FAILURE     - Indicates failed message processing
*       OAM_AGNT_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_agnt_return_et
oam_agnt_send_oam_agent_kill_process_req
(
    oam_agent_kill_process_req_t  *p_oam_agent_kill_process_req,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dest_module_id,  /* Destination module identifier */
    UInt16                 transaction_id,  /* Interface transaction identifier */
    UInt8                  instance_id
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_agnt_msg;
    oam_agnt_error_code_et err = OAM_AGNT_NO_ERROR;

    /* Get API length */
    msg_length = oam_agnt_get_oam_agent_kill_process_req_len(p_oam_agent_kill_process_req);

    OAM_AGNT_LOG(AGNT, OAM_AGNT_INFO, "Src(%u)->Dst(%u):OAM_AGENT_KILL_PROCESS_REQ Instance %u", src_module_id, dest_module_id, instance_id);
	
	if(msg_length == OAM_AGNT_FAILURE)
	{
 		return OAM_AGNT_FAILURE;
	}	
    msg_api_length = msg_length + OAM_AGNT_API_HEADER_SIZE + OAM_AGNT_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    if (OAM_AGNT_FAILURE == oam_agnt_mem_alloc(msg_api_length, (void*)&p_msg, &err))
    {
        /* Not enough memory */
        return OAM_AGNT_FAILURE;
    }

    oam_agnt_memset(p_msg, 0, msg_api_length);
    p_agnt_msg = p_msg;

    /* Fill CSPL header */
    oam_agnt_construct_api_header(p_agnt_msg, OAM_AGNT_VERSION, src_module_id,
        dest_module_id, OAM_AGENT_KILL_PROCESS_REQ, msg_api_length);

    /* Fill interface header */
    p_agnt_msg = p_agnt_msg + OAM_AGNT_API_HEADER_SIZE;
    oam_agnt_construct_interface_api_header(p_agnt_msg, transaction_id, src_module_id,
        dest_module_id, OAM_AGENT_KILL_PROCESS_REQ, msg_length,instance_id);

    /* Fill OAM_AGENT_KILL_PROCESS_REQ message */
    p_agnt_msg = p_agnt_msg + OAM_AGNT_INTERFACE_API_HEADER_SIZE;
    if (OAM_AGNT_FAILURE == oam_agnt_compose_oam_agent_kill_process_req(&p_agnt_msg, p_oam_agent_kill_process_req))
    {
        oam_agnt_mem_free(p_msg, &err);

        return OAM_AGNT_FAILURE;
    }

    if((p_agnt_msg - msg_api_length) != p_msg)
    {
        oam_agnt_mem_free(p_msg, &err);

        return OAM_AGNT_FAILURE;
    }

    /* Send message to */
    oam_agnt_send_message(p_msg, dest_module_id);

    return OAM_AGNT_SUCCESS;
}

static
oam_agnt_length_t
oam_agnt_get_oam_agent_kill_process_list_len
(
    oam_agent_kill_process_list_t *p_oam_agent_kill_process_list
)
{
    oam_agnt_length_t length = 0;



    /* Get length of IE */
    length += oam_agnt_get_oam_agent_process_name_len(&p_oam_agent_kill_process_list->process_name);

    return length;
}

static
oam_agnt_return_et
oam_agnt_compose_oam_agent_kill_process_list
(
    UInt8  **pp_buffer,
    oam_agent_kill_process_list_t *p_oam_agent_kill_process_list
)
{

    /* Compose IE */
    if (OAM_AGNT_FAILURE == oam_agnt_compose_oam_agent_process_name(pp_buffer, &p_oam_agent_kill_process_list->process_name))
    {
        return OAM_AGNT_FAILURE;
    }

    return OAM_AGNT_SUCCESS;
}

oam_agnt_length_t
oam_agnt_get_agent_oam_kill_process_resp_len
(
    agent_oam_kill_process_resp_t *p_agent_oam_kill_process_resp
)
{
    oam_agnt_length_t length = 0;


    /* Get length of parameter of basic type */
    length += sizeof(p_agent_oam_kill_process_resp->num_process);

    /* Check for correct range [H - higher boundary] */
    if ((p_agent_oam_kill_process_resp->num_process > OAM_AGENT_MAX_NUM_PROCESS))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Parameter [p_agent_oam_kill_process_resp->num_process] should be in range [%d <= OAM_AGENT_MAX_NUM_PROCESS] !", p_agent_oam_kill_process_resp->num_process);
        return OAM_AGNT_FAILURE;
    }

    /* Get length of OCTET_STRING VARIABLE of IEs */
    {
        oam_agnt_counter_t loop;
        for (loop = 0; loop < p_agent_oam_kill_process_resp->num_process; loop++)
        {
            length += oam_agnt_get_oam_agent_kill_process_status_list_len(&p_agent_oam_kill_process_resp->fail_process_list[loop]);
        }
    }

    return length;
}

oam_agnt_return_et
oam_agnt_compose_agent_oam_kill_process_resp
(
    UInt8  **pp_buffer,
    agent_oam_kill_process_resp_t *p_agent_oam_kill_process_resp
)
{

    /* Check for correct range [H - higher boundary] */
    if ((p_agent_oam_kill_process_resp->num_process > OAM_AGENT_MAX_NUM_PROCESS))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Parameter [p_agent_oam_kill_process_resp->num_process] should be in range [%d <= OAM_AGENT_MAX_NUM_PROCESS] !", p_agent_oam_kill_process_resp->num_process);
        return OAM_AGNT_FAILURE;
    }

    /* Compose parameter of basic type */
    oam_agnt_cp_pack_UInt8(*pp_buffer, &p_agent_oam_kill_process_resp->num_process, "num_process");
    *pp_buffer += sizeof(p_agent_oam_kill_process_resp->num_process);

    /* Compose OCTET_STRING VARIABLE of IEs */
    {
        oam_agnt_counter_t loop;
        for (loop = 0; loop < p_agent_oam_kill_process_resp->num_process; loop++)
        {
            if (OAM_AGNT_FAILURE == oam_agnt_compose_oam_agent_kill_process_status_list(pp_buffer, &p_agent_oam_kill_process_resp->fail_process_list[loop]))
            {
                return OAM_AGNT_FAILURE;
            }
        }
    }

    return OAM_AGNT_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_agnt_send_agent_oam_kill_process_resp
*
*   DESCRIPTION:
*       This function constructs and sends AGENT_OAM_KILL_PROCESS_RESP message
*
*   RETURNS:
*       OAM_AGNT_FAILURE     - Indicates failed message processing
*       OAM_AGNT_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_agnt_return_et
oam_agnt_send_agent_oam_kill_process_resp
(
    agent_oam_kill_process_resp_t  *p_agent_oam_kill_process_resp,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dest_module_id,  /* Destination module identifier */
    UInt16                 transaction_id,  /* Interface transaction identifier */
    UInt8                  instance_id
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_agnt_msg;
    oam_agnt_error_code_et err = OAM_AGNT_NO_ERROR;

    /* Get API length */
    msg_length = oam_agnt_get_agent_oam_kill_process_resp_len(p_agent_oam_kill_process_resp);

    OAM_AGNT_LOG(AGNT, OAM_AGNT_INFO, "Src(%u)->Dst(%u):AGENT_OAM_KILL_PROCESS_RESP Instance %u", src_module_id, dest_module_id, instance_id);
	
	if(msg_length == OAM_AGNT_FAILURE)
	{
 		return OAM_AGNT_FAILURE;
	}	
    msg_api_length = msg_length + OAM_AGNT_API_HEADER_SIZE + OAM_AGNT_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    if (OAM_AGNT_FAILURE == oam_agnt_mem_alloc(msg_api_length, (void*)&p_msg, &err))
    {
        /* Not enough memory */
        return OAM_AGNT_FAILURE;
    }

    oam_agnt_memset(p_msg, 0, msg_api_length);
    p_agnt_msg = p_msg;

    /* Fill CSPL header */
    oam_agnt_construct_api_header(p_agnt_msg, OAM_AGNT_VERSION, src_module_id,
        dest_module_id, AGENT_OAM_KILL_PROCESS_RESP, msg_api_length);

    /* Fill interface header */
    p_agnt_msg = p_agnt_msg + OAM_AGNT_API_HEADER_SIZE;
    oam_agnt_construct_interface_api_header(p_agnt_msg, transaction_id, src_module_id,
        dest_module_id, AGENT_OAM_KILL_PROCESS_RESP, msg_length,instance_id);

    /* Fill AGENT_OAM_KILL_PROCESS_RESP message */
    p_agnt_msg = p_agnt_msg + OAM_AGNT_INTERFACE_API_HEADER_SIZE;
    if (OAM_AGNT_FAILURE == oam_agnt_compose_agent_oam_kill_process_resp(&p_agnt_msg, p_agent_oam_kill_process_resp))
    {
        oam_agnt_mem_free(p_msg, &err);

        return OAM_AGNT_FAILURE;
    }

    if((p_agnt_msg - msg_api_length) != p_msg)
    {
        oam_agnt_mem_free(p_msg, &err);

        return OAM_AGNT_FAILURE;
    }

    /* Send message to */
    oam_agnt_send_message(p_msg, dest_module_id);

    return OAM_AGNT_SUCCESS;
}

static
oam_agnt_length_t
oam_agnt_get_oam_agent_kill_process_status_list_len
(
    oam_agent_kill_process_status_list_t *p_oam_agent_kill_process_status_list
)
{
    oam_agnt_length_t length = 0;



    /* Get length of IE */
    length += oam_agnt_get_oam_agent_process_name_len(&p_oam_agent_kill_process_status_list->failed_process_name);
    /* Get length of parameter of basic type */
    length += sizeof(p_oam_agent_kill_process_status_list->err);

    return length;
}

static
oam_agnt_return_et
oam_agnt_compose_oam_agent_kill_process_status_list
(
    UInt8  **pp_buffer,
    oam_agent_kill_process_status_list_t *p_oam_agent_kill_process_status_list
)
{

    /* Compose IE */
    if (OAM_AGNT_FAILURE == oam_agnt_compose_oam_agent_process_name(pp_buffer, &p_oam_agent_kill_process_status_list->failed_process_name))
    {
        return OAM_AGNT_FAILURE;
    }

    /* Compose parameter of basic type */
    oam_agnt_cp_pack_UInt32(*pp_buffer, &p_oam_agent_kill_process_status_list->err, "err");
    *pp_buffer += sizeof(p_oam_agent_kill_process_status_list->err);

    return OAM_AGNT_SUCCESS;
}

oam_agnt_length_t
oam_agnt_get_agent_oam_get_logs_resp_len
(
    agent_oam_get_logs_resp_t *p_agent_oam_get_logs_resp
)
{
    oam_agnt_length_t length = 0;



    /* Get length of IE */
    length += oam_agnt_get_oam_agent_file_name_len(&p_agent_oam_get_logs_resp->filename);

    return length;
}

oam_agnt_return_et
oam_agnt_compose_agent_oam_get_logs_resp
(
    UInt8  **pp_buffer,
    agent_oam_get_logs_resp_t *p_agent_oam_get_logs_resp
)
{

    /* Compose IE */
    if (OAM_AGNT_FAILURE == oam_agnt_compose_oam_agent_file_name(pp_buffer, &p_agent_oam_get_logs_resp->filename))
    {
        return OAM_AGNT_FAILURE;
    }

    return OAM_AGNT_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_agnt_send_agent_oam_get_logs_resp
*
*   DESCRIPTION:
*       This function constructs and sends AGENT_OAM_GET_LOGS_RESP message
*
*   RETURNS:
*       OAM_AGNT_FAILURE     - Indicates failed message processing
*       OAM_AGNT_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_agnt_return_et
oam_agnt_send_agent_oam_get_logs_resp
(
    agent_oam_get_logs_resp_t  *p_agent_oam_get_logs_resp,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dest_module_id,  /* Destination module identifier */
    UInt16                 transaction_id,  /* Interface transaction identifier */
    UInt8                  instance_id
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_agnt_msg;
    oam_agnt_error_code_et err = OAM_AGNT_NO_ERROR;

    /* Get API length */
    msg_length = oam_agnt_get_agent_oam_get_logs_resp_len(p_agent_oam_get_logs_resp);

    OAM_AGNT_LOG(AGNT, OAM_AGNT_INFO, "Src(%u)->Dst(%u):AGENT_OAM_GET_LOGS_RESP Instance %u", src_module_id, dest_module_id, instance_id);
	
	if(msg_length == OAM_AGNT_FAILURE)
	{
 		return OAM_AGNT_FAILURE;
	}	
    msg_api_length = msg_length + OAM_AGNT_API_HEADER_SIZE + OAM_AGNT_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    if (OAM_AGNT_FAILURE == oam_agnt_mem_alloc(msg_api_length, (void*)&p_msg, &err))
    {
        /* Not enough memory */
        return OAM_AGNT_FAILURE;
    }

    oam_agnt_memset(p_msg, 0, msg_api_length);
    p_agnt_msg = p_msg;

    /* Fill CSPL header */
    oam_agnt_construct_api_header(p_agnt_msg, OAM_AGNT_VERSION, src_module_id,
        dest_module_id, AGENT_OAM_GET_LOGS_RESP, msg_api_length);

    /* Fill interface header */
    p_agnt_msg = p_agnt_msg + OAM_AGNT_API_HEADER_SIZE;
    oam_agnt_construct_interface_api_header(p_agnt_msg, transaction_id, src_module_id,
        dest_module_id, AGENT_OAM_GET_LOGS_RESP, msg_length,instance_id);

    /* Fill AGENT_OAM_GET_LOGS_RESP message */
    p_agnt_msg = p_agnt_msg + OAM_AGNT_INTERFACE_API_HEADER_SIZE;
    if (OAM_AGNT_FAILURE == oam_agnt_compose_agent_oam_get_logs_resp(&p_agnt_msg, p_agent_oam_get_logs_resp))
    {
        oam_agnt_mem_free(p_msg, &err);

        return OAM_AGNT_FAILURE;
    }

    if((p_agnt_msg - msg_api_length) != p_msg)
    {
        oam_agnt_mem_free(p_msg, &err);

        return OAM_AGNT_FAILURE;
    }

    /* Send message to */
    oam_agnt_send_message(p_msg, dest_module_id);

    return OAM_AGNT_SUCCESS;
}

static
oam_agnt_length_t
oam_agnt_get_oam_agent_file_name_len
(
    oam_agent_file_name_t *p_oam_agent_file_name
)
{
    oam_agnt_length_t length = 0;


    /* Get length of parameter of basic type */
    length += sizeof(p_oam_agent_file_name->len);
    /* Get length of OCTET_STRING VARIABLE of basic type elements */
    length += (p_oam_agent_file_name->len * sizeof(p_oam_agent_file_name->name[0]));

    return length;
}

static
oam_agnt_return_et
oam_agnt_compose_oam_agent_file_name
(
    UInt8  **pp_buffer,
    oam_agent_file_name_t *p_oam_agent_file_name
)
{

    /* Check for correct range [H - higher boundary] */
    if ((p_oam_agent_file_name->len > OAM_AGENT_MAX_FILE_NAME))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Parameter [p_oam_agent_file_name->len] should be in range [%d <= OAM_AGENT_MAX_FILE_NAME] !", p_oam_agent_file_name->len);
        return OAM_AGNT_FAILURE;
    }

    /* Compose parameter of basic type */
    oam_agnt_cp_pack_UInt16(*pp_buffer, &p_oam_agent_file_name->len, "len");
    *pp_buffer += sizeof(p_oam_agent_file_name->len);

    /* Compose OCTET_STRING VARIABLE of basic type elements */
    {
        oam_agnt_counter_t loop;
        for (loop = 0; loop < p_oam_agent_file_name->len; loop++)
        {
            oam_agnt_cp_pack_Char8(*pp_buffer, &p_oam_agent_file_name->name[loop], "name[]");
            *pp_buffer += sizeof(Char8);
        }
    }

    return OAM_AGNT_SUCCESS;
}

oam_agnt_length_t
oam_agnt_get_oam_agent_get_logs_cnf_len
(
    oam_agent_get_logs_cnf_t *p_oam_agent_get_logs_cnf
)
{
    oam_agnt_length_t length = 0;



    /* Get length of IE */
    length += oam_agnt_get_oam_agent_file_name_len(&p_oam_agent_get_logs_cnf->filename);

    return length;
}

oam_agnt_return_et
oam_agnt_compose_oam_agent_get_logs_cnf
(
    UInt8  **pp_buffer,
    oam_agent_get_logs_cnf_t *p_oam_agent_get_logs_cnf
)
{

    /* Compose IE */
    if (OAM_AGNT_FAILURE == oam_agnt_compose_oam_agent_file_name(pp_buffer, &p_oam_agent_get_logs_cnf->filename))
    {
        return OAM_AGNT_FAILURE;
    }

    return OAM_AGNT_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_agnt_send_oam_agent_get_logs_cnf
*
*   DESCRIPTION:
*       This function constructs and sends OAM_AGENT_GET_LOGS_CNF message
*
*   RETURNS:
*       OAM_AGNT_FAILURE     - Indicates failed message processing
*       OAM_AGNT_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_agnt_return_et
oam_agnt_send_oam_agent_get_logs_cnf
(
    oam_agent_get_logs_cnf_t  *p_oam_agent_get_logs_cnf,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dest_module_id,  /* Destination module identifier */
    UInt16                 transaction_id,  /* Interface transaction identifier */
    UInt8                  instance_id
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_agnt_msg;
    oam_agnt_error_code_et err = OAM_AGNT_NO_ERROR;

    /* Get API length */
    msg_length = oam_agnt_get_oam_agent_get_logs_cnf_len(p_oam_agent_get_logs_cnf);

    OAM_AGNT_LOG(AGNT, OAM_AGNT_INFO, "Src(%u)->Dst(%u):OAM_AGENT_GET_LOGS_CNF Instance %u", src_module_id, dest_module_id, instance_id);
	
	if(msg_length == OAM_AGNT_FAILURE)
	{
 		return OAM_AGNT_FAILURE;
	}	
    msg_api_length = msg_length + OAM_AGNT_API_HEADER_SIZE + OAM_AGNT_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    if (OAM_AGNT_FAILURE == oam_agnt_mem_alloc(msg_api_length, (void*)&p_msg, &err))
    {
        /* Not enough memory */
        return OAM_AGNT_FAILURE;
    }

    oam_agnt_memset(p_msg, 0, msg_api_length);
    p_agnt_msg = p_msg;

    /* Fill CSPL header */
    oam_agnt_construct_api_header(p_agnt_msg, OAM_AGNT_VERSION, src_module_id,
        dest_module_id, OAM_AGENT_GET_LOGS_CNF, msg_api_length);

    /* Fill interface header */
    p_agnt_msg = p_agnt_msg + OAM_AGNT_API_HEADER_SIZE;
    oam_agnt_construct_interface_api_header(p_agnt_msg, transaction_id, src_module_id,
        dest_module_id, OAM_AGENT_GET_LOGS_CNF, msg_length,instance_id);

    /* Fill OAM_AGENT_GET_LOGS_CNF message */
    p_agnt_msg = p_agnt_msg + OAM_AGNT_INTERFACE_API_HEADER_SIZE;
    if (OAM_AGNT_FAILURE == oam_agnt_compose_oam_agent_get_logs_cnf(&p_agnt_msg, p_oam_agent_get_logs_cnf))
    {
        oam_agnt_mem_free(p_msg, &err);

        return OAM_AGNT_FAILURE;
    }

    if((p_agnt_msg - msg_api_length) != p_msg)
    {
        oam_agnt_mem_free(p_msg, &err);

        return OAM_AGNT_FAILURE;
    }

    /* Send message to */
    oam_agnt_send_message(p_msg, dest_module_id);

    return OAM_AGNT_SUCCESS;
}

oam_agnt_length_t
oam_agnt_get_agent_oam_im_ok_ind_len
(
    agent_oam_im_ok_ind_t *p_agent_oam_im_ok_ind
)
{
    oam_agnt_length_t length = 0;


    /* Get length of parameter of basic type */
    length += sizeof(p_agent_oam_im_ok_ind->cksum);

    /* Get length of IE */
    length += oam_agnt_get_oam_agent_dir_path_len(&p_agent_oam_im_ok_ind->bin_path);

    return length;
}

oam_agnt_return_et
oam_agnt_compose_agent_oam_im_ok_ind
(
    UInt8  **pp_buffer,
    agent_oam_im_ok_ind_t *p_agent_oam_im_ok_ind
)
{

    /* Compose parameter of basic type */
    oam_agnt_cp_pack_UInt64(*pp_buffer, &p_agent_oam_im_ok_ind->cksum, "cksum");
    *pp_buffer += sizeof(p_agent_oam_im_ok_ind->cksum);

    /* Compose IE */
    if (OAM_AGNT_FAILURE == oam_agnt_compose_oam_agent_dir_path(pp_buffer, &p_agent_oam_im_ok_ind->bin_path))
    {
        return OAM_AGNT_FAILURE;
    }

    return OAM_AGNT_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: oam_agnt_send_agent_oam_im_ok_ind
*
*   DESCRIPTION:
*       This function constructs and sends AGENT_OAM_IM_OK_IND message
*
*   RETURNS:
*       OAM_AGNT_FAILURE     - Indicates failed message processing
*       OAM_AGNT_SUCCESS     - Indicates successful message processing
*
******************************************************************************/
oam_agnt_return_et
oam_agnt_send_agent_oam_im_ok_ind
(
    agent_oam_im_ok_ind_t  *p_agent_oam_im_ok_ind,           /* Pointer to API specific information. */
    UInt16                 src_module_id,  /* Source module identifier */
    UInt16                 dest_module_id,  /* Destination module identifier */
    UInt16                 transaction_id,  /* Interface transaction identifier */
    UInt8                  instance_id
)
{
    UInt16 msg_length, msg_api_length;
    UInt8 *p_msg, *p_agnt_msg;
    oam_agnt_error_code_et err = OAM_AGNT_NO_ERROR;

    /* Get API length */
    msg_length = oam_agnt_get_agent_oam_im_ok_ind_len(p_agent_oam_im_ok_ind);

    OAM_AGNT_LOG(AGNT, OAM_AGNT_INFO, "Src(%u)->Dst(%u):AGENT_OAM_IM_OK_IND Instance %u", src_module_id, dest_module_id, instance_id);
	
	if(msg_length == OAM_AGNT_FAILURE)
	{
 		return OAM_AGNT_FAILURE;
	}	
    msg_api_length = msg_length + OAM_AGNT_API_HEADER_SIZE + OAM_AGNT_INTERFACE_API_HEADER_SIZE;

    /* Allocate buffer */
    if (OAM_AGNT_FAILURE == oam_agnt_mem_alloc(msg_api_length, (void*)&p_msg, &err))
    {
        /* Not enough memory */
        return OAM_AGNT_FAILURE;
    }

    oam_agnt_memset(p_msg, 0, msg_api_length);
    p_agnt_msg = p_msg;

    /* Fill CSPL header */
    oam_agnt_construct_api_header(p_agnt_msg, OAM_AGNT_VERSION, src_module_id,
        dest_module_id, AGENT_OAM_IM_OK_IND, msg_api_length);

    /* Fill interface header */
    p_agnt_msg = p_agnt_msg + OAM_AGNT_API_HEADER_SIZE;
    oam_agnt_construct_interface_api_header(p_agnt_msg, transaction_id, src_module_id,
        dest_module_id, AGENT_OAM_IM_OK_IND, msg_length,instance_id);

    /* Fill AGENT_OAM_IM_OK_IND message */
    p_agnt_msg = p_agnt_msg + OAM_AGNT_INTERFACE_API_HEADER_SIZE;
    if (OAM_AGNT_FAILURE == oam_agnt_compose_agent_oam_im_ok_ind(&p_agnt_msg, p_agent_oam_im_ok_ind))
    {
        oam_agnt_mem_free(p_msg, &err);

        return OAM_AGNT_FAILURE;
    }

    if((p_agnt_msg - msg_api_length) != p_msg)
    {
        oam_agnt_mem_free(p_msg, &err);

        return OAM_AGNT_FAILURE;
    }

    /* Send message to */
    oam_agnt_send_message(p_msg, dest_module_id);

    return OAM_AGNT_SUCCESS;
}

