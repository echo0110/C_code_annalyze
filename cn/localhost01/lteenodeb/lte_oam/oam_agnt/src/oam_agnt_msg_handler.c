/* vim:ts=4:et:ai:ci:sw=4
*/
/*******************************************************************************
 * File Name       : oam_agnt_msg_handler.c
 *
 * Description     : This file contains utility based functions. 
 * Revision History:
 *
 * Date         Author                 Reference            Comments
 * -----        ------                 ---------           --------
 *
 * Copyright (c) 2013, Aricent Inc. All Rights Reserved
 *******************************************************************************/

/*******************************************************************************
 * Includes
 *** ***************************************************************************/
#include "oam_agnt_utils.h"
#include "oam_agnt_port.h"
#include "oam_agnt_intf.h"
#include "oam_agnt_msg_handler.h"
#include "oam_mgmt_intf_api.h"
#include "oam_defines.h"
#include "oam_agnt_composer.h"
#include "oam_agnt_parser.h"

/*******************************************************************************
 *  Global Declaration
 *******************************************************************************/

agnt_process_table_t g_agnt_process_tbl = {OAM_AGNT_ZERO, OAM_AGNT_NULL};
extern Char8 **g_agent_args;
extern UInt8 g_agent_args_cnt;
extern log_priority_et g_log_level;
extern LISTENER *u;

Char8 *g_known_process_list [] =
{
    OAM_AGENT_BIN_NAME,
    OAM_L2_BIN_NAME,
    OAM_PDCP_BIN_NAME,
    OAM_EGTPU_BIN_NAME,
    OAM_LIBBINREAD_BIN_NAME,
    OAM_SDM_BIN_NAME,
    OAM_TR069_BIN_NAME,
    OAM_QCLI_BIN_NAME,
    OAM_RRM_BIN_NAME,
    OAM_SON_BIN_NAME,
    OAM_L3_BIN_NAME,
    OAM_AGNT_NULL
};

LOCAL const unsigned long agntDestList[] = {
	OAM_MODULE_ID,
	OAM_AGNT_ZERO
};


/****************************************************************************
  Private Types
 ****************************************************************************/

/*************************************************************************
 * Function Name  : send_oam_agnt_init_ind
 * Description    : This is used for sending INIT indication to OAM
 * Inputs         : None
 * Outputs        : none
 * Returns        : OAM_AGNT_SUCCESS/OAM_AGNT_FAILURE
 ******************************************************************************/
oam_agnt_return_et
send_oam_agnt_init_ind(void)
{
    agent_oam_init_ind_t    *p_init_ind = OAM_AGNT_NULL;
    oam_agnt_error_code_et err          = OAM_AGNT_NO_ERROR;
    UInt8 num_process = OAM_AGNT_ZERO;
    OAM_AGNT_FUN_ENTRY_TRACE ();

    if (OAM_AGNT_SUCCESS != oam_agnt_mem_alloc(sizeof(agent_oam_init_ind_t), (void*)&p_init_ind, &err))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_ERROR, "Failed to get memory for INIT indication");
        return OAM_AGNT_FAILURE;
    }

    p_init_ind->num_process = OAM_AGNT_ZERO;

    while ((OAM_AGENT_MAX_NUM_PROCESS > num_process) &&
            (num_process <
             (sizeof(g_known_process_list)/sizeof(g_known_process_list[OAM_AGNT_ZERO]))) &&
            (OAM_AGNT_NULL != g_known_process_list[num_process]))
    {
        /* Renaming if latest image is already present */
        oam_agnt_rename_latest_img(g_known_process_list[num_process]);
        if (OAM_AGNT_SUCCESS == oam_agnt_get_cksum(g_known_process_list[num_process],
                &p_init_ind->process_list[p_init_ind->num_process].cksum))
        {
            oam_agnt_strncpy(p_init_ind->process_list[p_init_ind->num_process].process_name.name,
                    g_known_process_list[num_process], OAM_AGENT_MAX_PROCESS_NAME);
            p_init_ind->process_list[p_init_ind->num_process].process_name.name[OAM_AGENT_MAX_PROCESS_NAME - OAM_AGNT_ONE] = '\0';

            p_init_ind->process_list[p_init_ind->num_process].process_name.len =
                oam_agnt_strlen(p_init_ind->process_list[p_init_ind->num_process].process_name.name);

            p_init_ind->num_process++;
        }
        num_process++;
    }
    if (OAM_AGNT_NULL != oam_agnt_get_cwd(p_init_ind->bin_path.name, OAM_AGENT_MAX_DIR_PATH))
    {
        p_init_ind->bin_path.len = oam_agnt_strlen(p_init_ind->bin_path.name);
    }

    /* Send message */
    oam_agnt_send_agent_oam_init_ind(p_init_ind, OAM_AGNT_MODULE_ID, OAM_MODULE_ID, OAM_AGNT_ZERO, oam_agnt_get_my_id());

    OAM_AGNT_FUN_EXIT_TRACE ();
    return OAM_AGNT_SUCCESS;
}

/*************************************************************************
 * Function Name  : send_oam_agnt_im_ok_ind
 * Description    : This is used for sending IM OK message to OAM
 * Inputs         : None
 * Outputs        : none
 * Returns        : OAM_AGNT_SUCCESS/OAM_AGNT_FAILURE
 ******************************************************************************/
oam_agnt_return_et
send_oam_agnt_im_ok_ind(void)
{
    UInt8 *p_api_msg = OAM_AGNT_NULL;
    UInt8 *p_msg = OAM_AGNT_NULL;
    UInt16 msg_length = OAM_AGNT_ZERO;
    /* SPR 21889: CID 115186 Start */
    oam_agnt_error_code_et error_code = OAM_AGNT_NO_ERROR;
    /* SPR 21889: CID 115186 End */
    UInt16 transaction_id = OAM_AGNT_ZERO;
    OAM_AGNT_FUN_ENTRY_TRACE ();


    OAM_AGNT_LOG(AGNT, OAM_AGNT_BRIEF, "Src(%u)->Dst(%u):AGENT_OAM_IM_OK_IND Instance %u",
            OAM_AGNT_MODULE_ID, OAM_MODULE_ID, oam_agnt_get_my_id());
    msg_length = OAM_AGNT_API_HEADER_SIZE + OAM_AGNT_INTERFACE_API_HEADER_SIZE;

    if (OAM_AGNT_FAILURE == oam_agnt_mem_alloc (msg_length, (void *) &p_msg, &error_code))
    {
        OAM_AGNT_LOG (AGNT, OAM_AGNT_ERROR,
                "Error in Memory Allocation with Error Code:%d",
                error_code);
        OAM_AGNT_FUN_EXIT_TRACE ();
        return OAM_AGNT_FAILURE;
    }

    oam_agnt_memset (p_msg, OAM_AGNT_ZERO, msg_length);
    p_api_msg = p_msg;
    /* encode the CSPL header in the message */
    oam_agnt_construct_api_header (p_api_msg, OAM_AGNT_VERSION, OAM_AGNT_MODULE_ID,
            OAM_MODULE_ID,
            AGENT_OAM_IM_OK_IND, OAM_AGNT_ZERO);

    p_api_msg += OAM_AGNT_API_HEADER_SIZE;

    /* encode API header in message */
    /* SPR 17777 fix */
    oam_agnt_construct_interface_api_header ((UInt8 *) p_api_msg,
            transaction_id,
            OAM_AGNT_MODULE_ID,
            OAM_MODULE_ID,
            AGENT_OAM_IM_OK_IND,
            OAM_AGNT_ZERO,
            oam_agnt_get_my_id());

    OAM_AGNT_FUN_EXIT_TRACE ();
    return oam_agnt_send_message (p_msg, OAM_MODULE_ID);
}

/*************************************************************************
 * Function Name  : agent_clear_process_table
 * Description    : This function is used for clearing the agent process table
 * Inputs         : None
 * Outputs        : none
 * Returns        : OAM_AGNT_SUCCESS/OAM_AGNT_FAILURE
 ******************************************************************************/
oam_agnt_return_et
agent_clear_process_table()
{
	SInt32 status = OAM_AGNT_ZERO; 
	UInt8  i = OAM_AGNT_ZERO;
    oam_agnt_error_code_et err = OAM_AGNT_NO_ERROR;

    OAM_AGNT_FUN_ENTRY_TRACE ();

    for (i = OAM_AGNT_ZERO; i < g_agnt_process_tbl.max_no_process; i++)
    {
        if (OAM_AGNT_ZERO != g_agnt_process_tbl.p_info[i].pid)
        {
            oam_agnt_kill(g_agnt_process_tbl.p_info[i].pid, AGNT_SIGKILL);
            oam_agnt_waitpid(g_agnt_process_tbl.p_info[i].pid, &status, 0);
        }
    }

    g_agnt_process_tbl.max_no_process = OAM_AGNT_ZERO;
    if (OAM_AGNT_NULL != g_agnt_process_tbl.p_info)
    {
        oam_agnt_mem_free(g_agnt_process_tbl.p_info, &err);
        g_agnt_process_tbl.p_info = OAM_AGNT_NULL;
    }

    OAM_AGNT_FUN_EXIT_TRACE ();
    return OAM_AGNT_SUCCESS;
}

/*************************************************************************
 * Function Name  : agnt_sig_handler
 * Description    : This catch the signal passed to the process and kill all
 *                  process that has been started by agent
 * Inputs         : Signal number
 * Outputs        : none
 * Returns        : OAM_AGNT_SUCCESS/OAM_AGNT_FAILURE
 ******************************************************************************/
void agnt_sig_handler(Int signo)
{
    OAM_AGNT_LOG(AGNT, OAM_AGNT_INFO, "Agent caught signal %u", signo);
    agent_clear_process_table();
    oam_agnt_exit(OAM_AGNT_EXIT_SUCCESS);
}

/******************************************************************************
 * Function Name  : agent_handle_spawn_process_req
 * Description    : This function is used to spawn process requested by OAM
 * Inputs         : p_api
 * Outputs        : none
 * Returns        : None
 *******************************************************************************/
void
agent_handle_spawn_process_req(UInt8 *p_api_buf)
{
    oam_agent_spawn_process_req_t *p_oam_spawn_process_req = OAM_AGNT_NULL;
    agent_oam_spawn_process_resp_t *p_oam_spawn_process_resp = OAM_AGNT_NULL;
    UInt8   fail_count = OAM_AGNT_ZERO;
    Char8 **pp_args = OAM_AGNT_NULL;
    Char8 tmp_str[OAM_AGENT_MAX_PROCESS_NAME] = {OAM_AGNT_ZERO};
    UInt8 tok_cnt;
    UInt8 num_args;
    UInt8 i;
    pid_t pid = OAM_AGNT_ZERO;
    oam_agent_timer_data_t  data;
    oam_agnt_error_code_et err  = OAM_AGNT_NO_ERROR;
    SInt16  errno_err = OAM_AGNT_ZERO;
	oam_agent_context_t *p_ctxt   = OAM_AGNT_NULL;
    SInt32 data_len = OAM_AGNT_ZERO;
    SInt32 len      = OAM_AGNT_ZERO;
    UInt8 *p_api = OAM_AGNT_NULL;
    UInt8 m;
    oam_agnt_bool_et agent_upgraded = OAM_AGNT_FALSE;

    OAM_AGNT_FUN_ENTRY_TRACE ();

    data_len = oam_agnt_get_api_len(p_api_buf);
    p_api = oam_agnt_get_api_start(p_api_buf);

    if (OAM_AGNT_SUCCESS != oam_agnt_mem_alloc(sizeof(oam_agent_spawn_process_req_t), (void*)&p_oam_spawn_process_req, &err))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_ERROR, "Failed to allocate memory for SPAWN REQ");
        OAM_AGNT_FUN_EXIT_TRACE ();
        return;
    }

    /* Parse message */
    if (OAM_AGNT_SUCCESS != oam_agnt_parse_oam_agent_spawn_process_req(p_oam_spawn_process_req, p_api, data_len, &len))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_ERROR, "OAM->AGENT:OAM_AGENT_SPAWN_PROCESS_REQ parsing failed");
        oam_agnt_mem_free(p_oam_spawn_process_req, &err);
        OAM_AGNT_FUN_EXIT_TRACE ();
        return;
    }
    else
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_INFO, "OAM->AGENT:OAM_AGENT_SPAWN_PROCESS_REQ");
    }
    if (OAM_AGNT_SUCCESS != oam_agnt_mem_alloc(sizeof(agent_oam_spawn_process_resp_t), (void*)&p_oam_spawn_process_resp, &err))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_ERROR, "Failed to allocate memory for SPAWN RESP");
        oam_agnt_mem_free(p_oam_spawn_process_req, &err);
        OAM_AGNT_FUN_EXIT_TRACE ();
        return;
    }

    if (OAM_AGNT_NULL != g_agnt_process_tbl.p_info)
    {
        /* OAM has sent request again without killing something is wrong but
         * will honour the command and kill previous table process */
        agent_clear_process_table();
    }

    if ((OAM_AGNT_ONE == p_oam_spawn_process_req->num_process) &&
            (OAM_AGNT_ZERO == oam_agnt_strcmp(p_oam_spawn_process_req->process_list[OAM_AGNT_ZERO].process_name.name,
             OAM_AGENT_BIN_NAME)))
    {
        num_args = OAM_AGNT_THREE;
        oam_agnt_strcpy(tmp_str, p_oam_spawn_process_req->process_list[OAM_AGNT_ZERO].arg_list.name);
        oam_agnt_strtok(tmp_str, " ");
        while (OAM_AGNT_NULL != (oam_agnt_strtok(OAM_AGNT_NULL, " ")))
        {
            /* Traversing all the argument and saving it in args */
            num_args++;
        }

        if (OAM_AGNT_SUCCESS != oam_agnt_mem_alloc(num_args * sizeof(Char8*), (void*)&pp_args, &err))
        {
            OAM_AGNT_LOG(AGNT, OAM_AGNT_ERROR, "Failed to make argument list for process %s Args %s",
                    p_oam_spawn_process_req->process_list[OAM_AGNT_ZERO].process_name,
                    p_oam_spawn_process_req->process_list[OAM_AGNT_ZERO].arg_list);
        }
        else
        {
            oam_agnt_socket_close(u->rx_tx_sd);
            tok_cnt = OAM_AGNT_ZERO;
            pp_args[tok_cnt++] = p_oam_spawn_process_req->process_list[OAM_AGNT_ZERO].process_name.name;

            oam_agnt_strcpy(tmp_str, p_oam_spawn_process_req->process_list[OAM_AGNT_ZERO].arg_list.name);
            pp_args[tok_cnt++] = oam_agnt_strtok(tmp_str, " ");
            while (OAM_AGNT_NULL != (pp_args[tok_cnt++] = oam_agnt_strtok(OAM_AGNT_NULL, " ")))
            {
                /* Traversing all the argument and saving it in args */
            }
            pp_args[tok_cnt] = OAM_AGNT_NULL;

            oam_agnt_rename_latest_img(p_oam_spawn_process_req->process_list[OAM_AGNT_ZERO].process_name.name);
            OAM_AGNT_LOG(AGNT, OAM_AGNT_INFO, "Upgraded myself hence restarting with Args %s",
                    p_oam_spawn_process_req->process_list[OAM_AGNT_ZERO].arg_list.name);
            if(-1 == oam_agnt_execve(p_oam_spawn_process_req->process_list[OAM_AGNT_ZERO].process_name.name,
                        pp_args,environ))
            {
                OAM_AGNT_LOG(AGNT, OAM_AGNT_ERROR,"Exec to %s failed!!!. Errno %d",
                        p_oam_spawn_process_req->process_list[OAM_AGNT_ZERO].process_name.name,
                        errno);
            }
        }
    }
    else
    {
        /* Allocating process table */
        if (OAM_AGNT_SUCCESS != oam_agnt_mem_alloc(sizeof(agnt_process_info_t) *
                    p_oam_spawn_process_req->num_process,
                    (void*)&g_agnt_process_tbl.p_info, &err))
        {
            OAM_AGNT_LOG(AGNT, OAM_AGNT_ERROR, "Failed to allocate memory for Process table");
            /* Send failure for all process */
            for (i = OAM_AGNT_ZERO; i < p_oam_spawn_process_req->num_process; i++)
            {
                oam_agnt_strcpy(p_oam_spawn_process_resp->process_list[i].failed_process_name.name,
                        p_oam_spawn_process_req->process_list[i].process_name.name);

                p_oam_spawn_process_resp->process_list[i].failed_process_name.len =
                    p_oam_spawn_process_req->process_list[i].process_name.len;

                p_oam_spawn_process_resp->process_list[i].err = errno;
            }
            oam_agnt_send_agent_oam_spawn_process_resp(p_oam_spawn_process_resp,
                    OAM_AGNT_MODULE_ID, OAM_MODULE_ID, OAM_AGNT_ZERO,
                    oam_agnt_get_my_id());

            oam_agnt_mem_free(p_oam_spawn_process_resp, &err);
            oam_agnt_mem_free(p_oam_spawn_process_req, &err);
            return;
        }

        /* Blindly doing the renaming to avoid multiple combination of the application using bin reader */
        oam_agnt_rename_latest_img(OAM_LIBBINREAD_BIN_NAME);
        for (i = OAM_AGNT_ZERO; i < p_oam_spawn_process_req->num_process; i++)
        {
            oam_agnt_rename_latest_img(p_oam_spawn_process_req->process_list[i].process_name.name);
            if (OAM_AGNT_TRUE == agent_chk_file_exist(p_oam_spawn_process_req->process_list[i].process_name.name, &errno_err))
            {
                /* Starting with three because one space for null and one for binary name also needs to be filled */
                num_args = OAM_AGNT_THREE;
                oam_agnt_strcpy(tmp_str, p_oam_spawn_process_req->process_list[i].arg_list.name);
                oam_agnt_strtok(tmp_str, " ");
                while (OAM_AGNT_NULL != (oam_agnt_strtok(OAM_AGNT_NULL, " ")))
                {
                    /* Traversing all the argument and saving it in args */
                    num_args++;
                }

                if (OAM_AGNT_SUCCESS != oam_agnt_mem_alloc(num_args * sizeof(Char8*), (void*)&pp_args, &err))
                {
                    OAM_AGNT_LOG(AGNT, OAM_AGNT_ERROR, "Failed to make argument list for process %s Args %s",
                            p_oam_spawn_process_req->process_list[i].process_name,
                            p_oam_spawn_process_req->process_list[i].arg_list);
                    continue;
                }
                tok_cnt = OAM_AGNT_ZERO;
                pp_args[tok_cnt++] = p_oam_spawn_process_req->process_list[i].process_name.name;

                oam_agnt_strcpy(tmp_str, p_oam_spawn_process_req->process_list[i].arg_list.name);
                pp_args[tok_cnt++] = oam_agnt_strtok(tmp_str, " ");
                while (OAM_AGNT_NULL != (pp_args[tok_cnt++] = oam_agnt_strtok(OAM_AGNT_NULL, " ")))
                {
                    /* Traversing all the argument and saving it in args */
                }
                pp_args[tok_cnt] = OAM_AGNT_NULL;

                if (OAM_AGNT_ZERO == oam_agnt_strcmp(p_oam_spawn_process_req->process_list[i].process_name.name,
                            OAM_AGENT_BIN_NAME))
                {
                    /* Checking if the arguments are modified */
                    if ((tok_cnt - OAM_AGNT_ONE) != g_agent_args_cnt)
                    {
                        agent_upgraded = OAM_AGNT_TRUE;
                    }
                    else
                    {
                        for (m = OAM_AGNT_ONE; m < g_agent_args_cnt; m++)
                        {
                            if (OAM_AGNT_ZERO != oam_agnt_strcmp(pp_args[m], g_agent_args[m]))
                            {
                                agent_upgraded = OAM_AGNT_TRUE;
                                break;
                            }
                        }
                    }

                    if (OAM_AGNT_TRUE == agent_upgraded)
                    {
                        agent_clear_process_table();
                        oam_agnt_socket_close(u->rx_tx_sd);
                        OAM_AGNT_LOG(AGNT, OAM_AGNT_INFO, "Agent arguments changed. Restarting process %s with Args %s",
                                p_oam_spawn_process_req->process_list[i].process_name.name,
                                p_oam_spawn_process_req->process_list[i].arg_list.name);
                        oam_agnt_rename_latest_img(p_oam_spawn_process_req->process_list[i].process_name.name);
                        if(-1 == oam_agnt_execve(p_oam_spawn_process_req->process_list[i].process_name.name,
                                    pp_args,environ))
                        {
                            OAM_AGNT_LOG(AGNT, OAM_AGNT_ERROR,"Exec to %s failed!!!. Errno %s",
                                    p_oam_spawn_process_req->process_list[i].process_name.name,
                                    oam_agnt_strerror(errno));
                        }
                    }
                    else
                    {
                        continue;
                    }
                }
                OAM_AGNT_LOG(AGNT, OAM_AGNT_INFO, "Forking process %s Args %s",
                        p_oam_spawn_process_req->process_list[i].process_name.name,
                        p_oam_spawn_process_req->process_list[i].arg_list.name);
                /* Fork out image */
                pid= oam_agnt_fork();

                switch(pid)
                {
                    case FORK_ERROR:
                        {
                            oam_agnt_strcpy(p_oam_spawn_process_resp->process_list[fail_count].failed_process_name.name,
                                    p_oam_spawn_process_req->process_list[i].process_name.name);
                            p_oam_spawn_process_resp->process_list[fail_count].failed_process_name.len =
                                p_oam_spawn_process_req->process_list[i].process_name.len;

                            p_oam_spawn_process_resp->process_list[fail_count].err = errno;
                            fail_count++;
                        }
                        break;
                    case FORK_CHILD:
                        {
                            oam_agnt_fprintf(stderr,"\x1b[32m*****************************************\n\x1b[0m");
                            oam_agnt_fprintf(stderr,"\x1b[32mForking process...\x1b[0m\n");
                            oam_agnt_fprintf(stderr,"\x1b[32mProcess Name:%s\nProcess Id:%d\x1b[0m\n",
                                    p_oam_spawn_process_req->process_list[i].process_name.name, oam_agnt_getpid());
                            oam_agnt_fprintf(stderr,"\x1b[32mProcess Args:%s\x1b[0m\n",
                                    p_oam_spawn_process_req->process_list[i].arg_list.name);

                            oam_agnt_fprintf(stderr,"\x1b[32m*****************************************\n\x1b[0m");

                            if(-1 == oam_agnt_execve(p_oam_spawn_process_req->process_list[i].process_name.name,
                                        pp_args,environ))
                            {
                                oam_agnt_fprintf(stderr,"\x1b[31m**********Exec to %s failed!!!****************\n\x1b[0m",
                                        p_oam_spawn_process_req->process_list[i].process_name.name);
                                oam_agnt_exit(errno);
                            }
                            break;
                        }
                    default:
                        oam_agnt_strcpy(g_agnt_process_tbl.p_info[g_agnt_process_tbl.max_no_process].name,
                                p_oam_spawn_process_req->process_list[i].process_name.name);
                        g_agnt_process_tbl.p_info[g_agnt_process_tbl.max_no_process].pid = pid;
                        g_agnt_process_tbl.max_no_process++;
                        break;
                }
            }
            else
            {
                /* Binary not found on the agent path */
                oam_agnt_strcpy(p_oam_spawn_process_resp->process_list[fail_count].failed_process_name.name,
                        p_oam_spawn_process_req->process_list[i].process_name.name);
                p_oam_spawn_process_resp->process_list[fail_count].err = errno_err;
                fail_count++;
            }
            if (OAM_AGNT_NULL != pp_args)
            {
                oam_agnt_mem_free(pp_args, &err);
                pp_args = OAM_AGNT_NULL;
            }
        }

        p_oam_spawn_process_resp->num_failed_process = fail_count;

        if (OAM_AGNT_ZERO == p_oam_spawn_process_resp->num_failed_process)
        {
            p_ctxt = qvModuleContext(qvGetService(OAM_AGNT_MODULE_ID));
            /* All process are successful */

            if (OAM_AGNT_NULL != p_ctxt)
            {
                if (OAM_AGNT_NULL != p_ctxt->timer_id[OAM_AGNT_INIT_IND_TIMER])
                {
                    oam_agnt_stop_timer(p_ctxt->timer_id[OAM_AGNT_INIT_IND_TIMER]);
                    p_ctxt->timer_id[OAM_AGNT_INIT_IND_TIMER] = OAM_AGNT_ZERO;
                }
                data.timer_type = OAM_AGNT_IM_OK_IND_TIMER;
                data.p_timer_data = OAM_AGNT_NULL;
                p_ctxt->timer_id[OAM_AGNT_IM_OK_IND_TIMER] = oam_agnt_start_timer(p_oam_spawn_process_req->interval, &data, OAM_AGNT_TRUE);
            }
        }
        else
        {
            /* Process startup failed hence killing all process so that OAM can send the list again */
            agent_clear_process_table();
        }
    }

    /* Send response */
    oam_agnt_send_agent_oam_spawn_process_resp(p_oam_spawn_process_resp,
            OAM_AGNT_MODULE_ID, OAM_MODULE_ID, OAM_AGNT_ZERO,
            oam_agnt_get_my_id());

    oam_agnt_mem_free(p_oam_spawn_process_resp, &err);
    oam_agnt_mem_free(p_oam_spawn_process_req, &err);

    OAM_AGNT_FUN_EXIT_TRACE ();
    return;
}

/*************************************************************************
 * Function Name  : agent_handle_kill_process_req
 * Description    : This function is used for handling the kill request sent by
 *                  OAM
 * Inputs         : p_api_buf
 * Outputs        : none
 * Returns        : OAM_AGNT_SUCCESS/OAM_AGNT_FAILURE
 ******************************************************************************/
void agent_handle_kill_process_req(UInt8 *p_api_buf)
{
    oam_agent_kill_process_req_t    *p_kill_req;
    agent_oam_kill_process_resp_t   *p_kill_resp;
    oam_agent_timer_data_t          tmr_data;
    /* SPR 21889: CID 115188 Start */
    oam_agnt_error_code_et err  = OAM_AGNT_NO_ERROR;
    /* SPR 21889: CID 115188 End */
    oam_agent_context_t *p_ctxt;
    UInt8 i = OAM_AGNT_ZERO;
    pid_t pid = OAM_AGNT_ZERO;
    Int status = OAM_ZERO;

    SInt32 data_len = OAM_AGNT_ZERO;
    SInt32 len      = OAM_AGNT_ZERO;
    UInt8 *p_api = OAM_AGNT_NULL;

    OAM_AGNT_FUN_ENTRY_TRACE();

    data_len = oam_agnt_get_api_len(p_api_buf);
    p_api = oam_agnt_get_api_start(p_api_buf);


    if (OAM_AGNT_SUCCESS != oam_agnt_mem_alloc(sizeof(oam_agent_kill_process_req_t), (void*)&p_kill_req, &err))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_ERROR, "Failed to allocate memory for KILL REQ");
        OAM_AGNT_FUN_EXIT_TRACE ();
        return;
    }

    /* Parse message */
    if (OAM_AGNT_SUCCESS != oam_agnt_parse_oam_agent_kill_process_req(p_kill_req, p_api, data_len, &len))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_ERROR, "OAM->AGENT:OAM_AGENT_KILL_PROCESS_REQ parsing failed");
        oam_agnt_mem_free(p_kill_req, &err);
        OAM_AGNT_FUN_EXIT_TRACE ();
        return;
    }
    else
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_INFO, "OAM->AGENT:OAM_AGENT_KILL_PROCESS_REQ");
    }
    if (OAM_AGNT_SUCCESS != oam_agnt_mem_alloc(sizeof(agent_oam_kill_process_resp_t), (void*)&p_kill_resp, &err))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_ERROR, "Failed to allocate memory for KILL RESP");
        OAM_AGNT_FUN_EXIT_TRACE ();
        return;
    }

    if (OAM_AGNT_ZERO == p_kill_req->num_process)
    {
        /* Request is for kill all */
        for (i = OAM_AGNT_ZERO; i < g_agnt_process_tbl.max_no_process; i++)
        {
            if (OAM_AGNT_ZERO != g_agnt_process_tbl.p_info[i].pid)
            {
                oam_agnt_kill(g_agnt_process_tbl.p_info[i].pid, AGNT_SIGKILL);
                pid = oam_agnt_waitpid(g_agnt_process_tbl.p_info[i].pid, &status, 0);
                if (pid != g_agnt_process_tbl.p_info[i].pid)
                {
                    oam_agnt_strncpy(p_kill_resp->fail_process_list[p_kill_resp->num_process].failed_process_name.name,
                            g_agnt_process_tbl.p_info[i].name,
                            OAM_AGENT_MAX_PROCESS_NAME);
                    p_kill_resp->fail_process_list[p_kill_resp->num_process].failed_process_name.name[OAM_AGENT_MAX_PROCESS_NAME
                        - OAM_AGNT_ONE]
                    = '\0';
                    p_kill_resp->fail_process_list[p_kill_resp->num_process].err = errno;
                    p_kill_resp->num_process++;
                }
            }
        }
        p_ctxt = qvModuleContext(qvGetService(OAM_AGNT_MODULE_ID));
        if (OAM_AGNT_NULL != p_ctxt)
        {
            /* Starting timer so that this message can be sent to OAM and then kill itself */
            tmr_data.timer_type = OAM_AGNT_SHUT_DOWN_TIMER;
            tmr_data.p_timer_data = OAM_AGNT_NULL;
            p_ctxt->timer_id[OAM_AGNT_SHUT_DOWN_TIMER] = oam_agnt_start_timer(OAM_AGENT_SHUT_DOWN_TIME, &tmr_data, OAM_AGNT_FALSE);
        }
    }

    /* Send message */
    oam_agnt_send_agent_oam_kill_process_resp(p_kill_resp, OAM_AGNT_MODULE_ID,
            OAM_MODULE_ID, OAM_ZERO, oam_agnt_get_my_id());

    oam_agnt_mem_free(p_kill_req, &err);
    oam_agnt_mem_free(p_kill_resp, &err);

    OAM_AGNT_FUN_EXIT_TRACE ();
    return;
}

/*************************************************************************
 * Function Name  : agent_handle_get_logs_request
 * Description    : This is used for handling get log request from OAM
 * Inputs         : p_api_buf
 * Outputs        : none
 * Returns        : OAM_AGNT_SUCCESS/OAM_AGNT_FAILURE
 ******************************************************************************/
void agent_handle_get_logs_request(UInt8 *p_api_buf)
{
    Char8   cmd[OAM_AGNT_MAX_CMD_SIZE];

    agent_oam_get_logs_resp_t get_log_resp;

    OAM_AGNT_FUN_ENTRY_TRACE ();
    oam_agnt_snprintf(get_log_resp.filename.name, sizeof(get_log_resp.filename.name), "agent_%02u_logs.tgz", oam_agnt_get_my_id()); 
    get_log_resp.filename.len = oam_agnt_strlen(get_log_resp.filename.name);

    OAM_AGNT_LOG(AGNT, OAM_AGNT_INFO, "OAM->AGENT:OAM_AGENT_GET_LOGS");
    oam_agnt_snprintf(cmd, sizeof(cmd), "tar -zcf %s *.log* *REGION*bin", get_log_resp.filename.name); 
    if (OAM_AGNT_ZERO == oam_agnt_system (cmd))
    {
        /* Send response */
        oam_agnt_send_agent_oam_get_logs_resp(&get_log_resp,
                OAM_AGNT_MODULE_ID, OAM_MODULE_ID, OAM_AGNT_ZERO,
                oam_agnt_get_my_id());
    }

    OAM_AGNT_FUN_EXIT_TRACE ();
    return;
}

/*************************************************************************
 * Function Name  : agent_handle_get_logs_cnf
 * Description    : This is used to handle confirm message from OAM
 * Inputs         : p_api_buf
 * Outputs        : none
 * Returns        : OAM_AGNT_SUCCESS/OAM_AGNT_FAILURE
 ******************************************************************************/
void agent_handle_get_logs_cnf(UInt8 *p_api_buf)
{
    oam_agent_get_logs_cnf_t get_log_resp;
    SInt32 data_len = OAM_AGNT_ZERO;
    SInt32 len      = OAM_AGNT_ZERO;
    UInt8 *p_api = OAM_AGNT_NULL;

    OAM_AGNT_FUN_ENTRY_TRACE();

    data_len = oam_agnt_get_api_len(p_api_buf);
    p_api = oam_agnt_get_api_start(p_api_buf);

    /* Parse message */
    if (OAM_AGNT_SUCCESS != oam_agnt_parse_oam_agent_get_logs_cnf(&get_log_resp, p_api, data_len, &len))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_ERROR, "OAM->AGENT:OAM_AGENT_GET_LOGS parsing failed");
        OAM_AGNT_FUN_EXIT_TRACE ();
        return;
    }
    else
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_INFO,
                "OAM->AGENT:OAM_AGENT_GET_LOGS_CNF file %s",
                get_log_resp.filename.name);
    }
    /* SPR 21889: CID 115175 Start */
    if(OAM_ZERO != oam_agnt_remove(get_log_resp.filename.name))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_INFO,
                "Unable to remove file %s error code:",
                get_log_resp.filename.name, errno);
    }
    /* SPR 21889: CID 115175 End */
    OAM_AGNT_FUN_EXIT_TRACE ();
    return;
}

/*************************************************************************
 * Function Name  : agent_handle_restart_ind
 * Description    : This is used for handling restart request from OAM
 * Inputs         : p_api_buf
 * Outputs        : none
 * Returns        : OAM_AGNT_SUCCESS/OAM_AGNT_FAILURE
 ******************************************************************************/
void agent_handle_restart_ind(UInt8 *p_api_buf)
{
    OAM_AGNT_FUN_ENTRY_TRACE ();
    OAM_AGNT_LOG(AGNT, OAM_AGNT_INFO, "OAM->AGENT:OAM_AGENT_RESTART_IND");
    agent_clear_process_table();
    oam_agnt_socket_close(u->rx_tx_sd);
    OAM_AGNT_LOG(AGNT,OAM_AGNT_INFO, "Starting agent %s new image", OAM_AGENT_BIN_NAME);
    if(-1 == oam_agnt_execve(OAM_AGENT_BIN_NAME,
                g_agent_args,environ))
    {
        OAM_AGNT_LOG(AGNT,OAM_AGNT_ERROR, "Failed to fork %s", OAM_AGENT_BIN_NAME);
    }

    OAM_AGNT_FUN_EXIT_TRACE ();
    return;
}

/*************************************************************************
 * Function Name  : agent_process_oam_msg
 * Description    : This is message handling function for API received from OAM
 * Inputs         : p_api_buf
 * Outputs        : none
 * Returns        : OAM_AGNT_SUCCESS/OAM_AGNT_FAILURE
 ******************************************************************************/
void agent_process_oam_msg(UInt8 *p_api_buf)
{
    UInt16 api_id = OAM_AGNT_ZERO;
    OAM_AGNT_FUN_ENTRY_TRACE ();

    api_id = oam_agnt_get_word_from_header ((UInt8 *) (p_api_buf) + OAM_AGNT_API_ID_HDR_IDX);
    switch (api_id)
    {
        case OAM_AGENT_SPAWN_PROCESS_REQ:
            agent_handle_spawn_process_req(p_api_buf);
            break;

        case OAM_AGENT_KILL_PROCESS_REQ:
            agent_handle_kill_process_req(p_api_buf);
            break;

        case OAM_AGENT_GET_LOGS:
            agent_handle_get_logs_request(p_api_buf);
            break;

        case OAM_AGENT_GET_LOGS_CNF:
            agent_handle_get_logs_cnf(p_api_buf);
            break;

        case OAM_AGENT_RESTART_IND:
            agent_handle_restart_ind(p_api_buf);
            break;

        default:
            OAM_AGNT_LOG (AGNT, OAM_AGNT_ERROR, "Invalid API[api_id=%d] Received by AGENT", api_id);
            break;
    }

    OAM_AGNT_FUN_EXIT_TRACE ();
    return;
}

/*************************************************************************
 * Function Name  : agent_init_context
 * Description    : This is used for allocating context for AGENT module
 * Inputs         : p_data initial data 
 * Outputs        : none
 * Returns        : OAM_AGNT_SUCCESS/OAM_AGNT_FAILURE
 ******************************************************************************/
void* agent_init_context(	
		void *p_data)
{
    oam_agent_context_t *p_ctxt_data;
    /* SPR 21889: CID 115188 Start */
    oam_agnt_error_code_et err  = OAM_AGNT_NO_ERROR;
    /* SPR 21889: CID 115188 End */
    oam_agent_timer_data_t  data;
    oam_agnt_timer_type_et  i;

    if (OAM_AGNT_SUCCESS != oam_agnt_mem_alloc(sizeof(oam_agent_context_t), (void*)&p_ctxt_data, &err))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_INFO, "Failed to allocate memory for Agent Ctxt Aborting !!");
        oam_agnt_abort();
    }

    oam_agnt_memset(p_ctxt_data, OAM_AGNT_ZERO, sizeof(oam_agent_context_t));


    for (i = OAM_AGNT_INIT_IND_TIMER; i < OAM_AGNT_MAX_TIMER; i++)
    {
        p_ctxt_data->timer_id[i] = OAM_AGNT_ZERO;
    }
    data.timer_type = OAM_AGNT_INIT_IND_TIMER;
    data.p_timer_data = OAM_AGNT_NULL;
    p_ctxt_data->timer_id[OAM_AGNT_INIT_IND_TIMER] = oam_agnt_start_timer(OAM_AGENT_IND_PERIODICITY, &data, OAM_AGNT_TRUE);
    oam_agnt_signal(AGNT_SIGINT, agnt_sig_handler);
    oam_agnt_signal(AGNT_SIGKILL, agnt_sig_handler);
    oam_agnt_signal(AGNT_SIGTERM, agnt_sig_handler);
    oam_agnt_signal(AGNT_SIGABRT, agnt_sig_handler);

    /* Send INIT */
    send_oam_agnt_init_ind();
    return p_ctxt_data;
}

/*************************************************************************
 * Function Name  : agent_timer_handler
 * Description    : This is used for handling timer expiry
 * Inputs         : p_msg, dst_module
 * Outputs        : none
 * Returns        : OAM_AGNT_SUCCESS/OAM_AGNT_FAILURE
 ******************************************************************************/
void agent_timer_handler(QTIMER timer_id, void *p_buf, void *p_context)
{
	oam_agent_timer_data_t *p_timer_buf    =   (oam_agent_timer_data_t*)p_buf;
	switch (p_timer_buf->timer_type)
    {
        case OAM_AGNT_INIT_IND_TIMER:
            OAM_AGNT_LOG(AGNT,OAM_AGNT_INFO,
                    "OAM_AGNT_INIT_IND_TIMER: Timer 0x%x expired", timer_id);
            send_oam_agnt_init_ind();
            break;

        case OAM_AGNT_IM_OK_IND_TIMER:
            OAM_AGNT_LOG(AGNT,OAM_AGNT_INFO,
                    "OAM_AGNT_IM_OK_IND_TIMER: Timer 0x%x expired", timer_id);
            send_oam_agnt_im_ok_ind();
            break;

        case OAM_AGNT_SHUT_DOWN_TIMER:
            OAM_AGNT_LOG(AGNT,OAM_AGNT_INFO,
                    "OAM_AGNT_SHUT_DOWN_TIMER: Timer 0x%x expired", timer_id);
            oam_agnt_exit(OAM_AGNT_EXIT_SUCCESS);
            break;

        default:
            OAM_AGNT_LOG(AGNT,OAM_AGNT_ERROR, "Invalid timer type %u Timer 0x%x",
                    p_timer_buf->timer_type, timer_id);
            break;
    }
}

/*****************************************************************************
 *   FUNCTION NAME  : oam_agnt_process_msg
 *   DESCRIPTION    : This function is used for distribution of API based on
 *                      Source
 *   INPUT          : p_api_buf :Pointer to API Buffer
 *   RETURNS        : OAM_AGNT_FAILURE/OAM_AGNT_SUCCESS
 ******************************************************************************/
oam_agnt_return_et
oam_agnt_process_msg (
		void *p_api_buf
		)
{
	OAM_AGNT_FUN_ENTRY_TRACE ();
	UInt16 src_id = OAM_AGNT_ZERO;
	/* coverity 35998 */

	/* Get the src ID */
	src_id = oam_agnt_get_word_from_header ((UInt8 *) (p_api_buf) + OAM_AGNT_SRC_ID_HDR_IDX);

    switch(src_id)
    {
        case OAM_MODULE_ID:
            agent_process_oam_msg((UInt8*)p_api_buf);
            break;

        default:
            OAM_AGNT_LOG (AGNT, OAM_AGNT_ERROR, "Message from Invalid source %u", src_id);
    }

	OAM_AGNT_FUN_EXIT_TRACE ();
	return OAM_AGNT_SUCCESS;
}

const QMANIFEST agnt_entity = {
	"OAM_AGNT_MODULE_ID",
	OAM_AGNT_MODULE_ID,
	{OAM_AGNT_ZERO, agent_init_context},
	{(S32 (*)(void *, void *))oam_agnt_process_msg, agent_timer_handler},
	{OAM_AGNT_ZERO, OAM_AGNT_ZERO,OAM_AGNT_ZERO},
	agntDestList
};
