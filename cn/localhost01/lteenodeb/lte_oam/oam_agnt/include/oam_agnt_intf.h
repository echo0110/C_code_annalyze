/* vim:ts=4:et:ai:ci:sw=4
*/
/*******************************************************************************
 * File Name   : oam_agnt_intf.h
 *
 * Description : 
 * Revision History:
 *
 * Date            Author          Reference             Comments
 * -----           ------          ---------             --------
 * Copyright (c) 2017, Aricent Inc. All Rights Reserved
 *******************************************************************************/

#ifndef OAM_AGNT_INTF_H
#define OAM_AGNT_INTF_H


/*******************************************************************************
 * Includes
 *******************************************************************************/

/*******************************************************************************
 * defines
 *****************************************************************************/
#define OAM_AGENT_MAX_PROCESS_NAME 	50 /* Maximum char for process binaries name */
#define OAM_AGENT_MAX_NUM_PROCESS 	20 /* Maximum processes which can be present at OAM agent */
#define OAM_AGENT_MAX_DIR_PATH 	512 /* maximum char for path for binaries transfer */
#define OAM_AGENT_PROCESS_ARG_LIST    255 /* Maximum char string which can be passed as argument for spawning a process */
#define OAM_AGENT_MAX_FILE_NAME 	40 /* Log File name */

/*******************************************************************************
 * structure definitions
 *****************************************************************************/

typedef struct _oam_agent_process_name
{
    UInt8 len; /*^ M, 0, H, 0, OAM_AGENT_MAX_PROCESS_NAME ^*/
    Char8 name[OAM_AGENT_MAX_PROCESS_NAME]; /*^ M ,0, OCTET_STRING, VARIABLE ^*/
}oam_agent_process_name_t;

typedef struct _oam_agent_process_args
{
    UInt8 len; /*^ M, 0, H, 0, OAM_AGENT_PROCESS_ARG_LIST ^*/
    Char8 name[OAM_AGENT_PROCESS_ARG_LIST];/*^ M ,0, OCTET_STRING, VARIABLE ^*/
}oam_agent_process_args_t;

typedef struct _oam_agent_file_name
{
    UInt16  len; /*^ M, 0, H, 0, OAM_AGENT_MAX_FILE_NAME  ^*/
    Char8   name[OAM_AGENT_MAX_FILE_NAME];/*^ M ,0, OCTET_STRING, VARIABLE ^*/
}oam_agent_file_name_t;

typedef struct _oam_agent_dir_path
{
    UInt16  len; /*^ M, 0, H, 0, OAM_AGENT_MAX_DIR_PATH  ^*/
    Char8   name[OAM_AGENT_MAX_DIR_PATH];/*^ M ,0, OCTET_STRING, VARIABLE ^*/
}oam_agent_dir_path_t;

typedef struct _oam_agent_process_info_list
{
    oam_agent_process_name_t     process_name; /*^ M, 0, N, 0, 0  ^*/
    UInt64    cksum;                           /*^ M, 0, N, 0, 0  ^*/
}oam_agent_process_info_list_t;

typedef struct _oam_agent_init_ind
{
    UInt8                           num_process;  /*^ M, 0, H, 0, OAM_AGENT_MAX_NUM_PROCESS  ^*/
    oam_agent_process_info_list_t   process_list[OAM_AGENT_MAX_NUM_PROCESS]; /*^ M ,0, OCTET_STRING, VARIABLE ^*/
    oam_agent_dir_path_t            bin_path;  /*^ M, 0, N, 0, 0  ^*/
}agent_oam_init_ind_t; /*^ API, AGENT_OAM_INIT_IND ^*/

typedef struct _oam_agent_process_spawn_list
{
  oam_agent_process_name_t     process_name;  /*^ M, 0, N, 0, 0  ^*/
  oam_agent_process_args_t       arg_list;   /*^ M, 0, N, 0, 0  ^*/
}oam_agent_process_spawn_list_t;

typedef struct _oam_agent_spawn_process_req
{
    UInt8 num_process; /*^ M, 0, H, 0, OAM_AGENT_MAX_NUM_PROCESS  ^*/ /* Number of process to be spawned by OAM-agent */
    oam_agent_process_spawn_list_t process_list[OAM_AGENT_MAX_NUM_PROCESS];/*^ M ,0, OCTET_STRING, VARIABLE ^*/
    UInt16 interval;  /*^ M, 0, N, 0, 0  ^*/ /* Periodicity at which IM_OK API is being sent by Agent */
}oam_agent_spawn_process_req_t;/*^ API, OAM_AGENT_SPAWN_PROCESS_REQ ^*/

typedef struct _oam_agent_process_spawn_status_list
{
  oam_agent_process_name_t  failed_process_name;  /*^ M, 0, N, 0, 0  ^*/
  UInt32                    err;  /*^ M, 0, N, 0, 0  ^*/  /* Errno value. Applicable only when status is failure */
}oam_agent_process_spawn_status_list_t;

typedef struct _agent_oam_spawn_process_resp
{
    /* Failed process list, In case all process started successfully then this
     * value is zero */
    UInt8 num_failed_process;  /*^ M, 0, N, 0, OAM_AGENT_MAX_NUM_PROCESS  ^*/ 
    /* Indicate list of failed process */
    oam_agent_process_spawn_status_list_t process_list[OAM_AGENT_MAX_NUM_PROCESS]; /*^ M ,0, OCTET_STRING, VARIABLE ^*/
}agent_oam_spawn_process_resp_t;/*^ API, AGENT_OAM_SPAWN_PROCESS_RESP ^*/



typedef struct _oam_agent_kill_process_list
{
  oam_agent_process_name_t     process_name;   /*^ M, 0, N, 0, 0  ^*/
}oam_agent_kill_process_list_t;

typedef struct _oam_agent_kill_process_req
{
    /* Zero value means kill all process which are being forked by OAM agent */
    UInt8 num_process;  /*^ M, 0, H, 0, OAM_AGENT_MAX_NUM_PROCESS  ^*/ 
    oam_agent_kill_process_list_t process_list[OAM_AGENT_MAX_NUM_PROCESS];/*^ M ,0, OCTET_STRING, VARIABLE ^*/
}oam_agent_kill_process_req_t;/*^ API, OAM_AGENT_KILL_PROCESS_REQ ^*/


typedef struct _oam_agent_kill_process_status_list
{
  oam_agent_process_name_t     failed_process_name;  /*^ M, 0, N, 0, 0  ^*/
  UInt32    err;  /*^ M, 0, N, 0, 0  ^*/ /* Errno value. Applicable only when status is failure */
}oam_agent_kill_process_status_list_t;

typedef struct _agent_oam_kill_process_resp
{
    UInt8 num_process;  /*^ M, 0, H, 0, OAM_AGENT_MAX_NUM_PROCESS  ^*/ /* Zero value means all process are killed successfully by OAM agent */
    oam_agent_kill_process_status_list_t fail_process_list[OAM_AGENT_MAX_NUM_PROCESS];/*^ M ,0, OCTET_STRING, VARIABLE ^*/
}agent_oam_kill_process_resp_t;/*^ API, AGENT_OAM_KILL_PROCESS_RESP ^*/


typedef struct _agent_oam_get_logs_resp
{
    oam_agent_file_name_t filename;  /*^ M, 0, N, 0, 0  ^*/
}agent_oam_get_logs_resp_t;/*^ API, AGENT_OAM_GET_LOGS_RESP ^*/


typedef struct _oam_agent_get_logs_cnf
{
    oam_agent_file_name_t filename;  /*^ M, 0, N, 0, 0  ^*/
}oam_agent_get_logs_cnf_t; /*^ API, OAM_AGENT_GET_LOGS_CNF ^*/

typedef struct _agent_oam_im_ok_ind
{
    UInt64  cksum;  /*^ M, 0, N, 0, 0  ^*/
    oam_agent_dir_path_t            bin_path;  /*^ M, 0, N, 0, 0  ^*/
}agent_oam_im_ok_ind_t; /*^ API, AGENT_OAM_IM_OK_IND ^*/
#endif
