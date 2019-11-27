/* vim:ts=4:et:ai:ci:sw=4
*/
/*******************************************************************************
 * File Name   : oam_agnt_msg_handler.h
 *
 * Description : 
 * Revision History:
 *
 * Date            Author          Reference             Comments
 * -----           ------          ---------             --------
 * Copyright (c) 2013, Aricent Inc. All Rights Reserved
 *******************************************************************************/
#ifndef _OAM_AGNT_MSG_HANDLER_H_
#define _OAM_AGNT_MSG_HANDLER_H_

#define OAM_AGENT_IND_PERIODICITY   5 /* Unit is in sec */
#define OAM_AGENT_SHUT_DOWN_TIME    3  /* Unit is in sec */

typedef struct _agnt_process_info
{
	char name[OAM_AGENT_MAX_PROCESS_NAME];
	pid_t pid;
}agnt_process_info_t;

typedef struct _agnt_process_table
{
  UInt8 max_no_process;
  agnt_process_info_t *p_info;
}agnt_process_table_t;

oam_agnt_return_et
oam_agnt_process_msg (void *p_api_buf);

oam_agnt_return_et 
send_oam_agnt_init_ind(void);
#endif
