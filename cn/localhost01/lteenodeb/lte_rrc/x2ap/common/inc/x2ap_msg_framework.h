/*******************************************************************************
*
*  FILE NAME   : x2ap_msg_fw.h
*
*  DESCRIPTION : This file contains the structure and function declaration
*                required for x2ap messaging framwork.
*
*  DATE 24-June-2010 NAME Ashutosh Gupta REFERENCE 36.423_880(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/
#ifndef _X2AP_MSG_FW_H_
#define _X2AP_MSG_FW_H_

#include "commoninc.h"
/*SPR 20172 FIX START*/
#define x2ap_send_message(p_msg,dst_module) do {\
	x2ap_send_message_ex(p_msg,dst_module);\
	p_msg = PNULL;\
}while(0)

void x2ap_send_message_ex(
/*SPR 20172 FIX End*/
    void            *p_msg,     
    x2ap_module_id_t dst_module);

x2ap_return_et x2ap_check_cspl_header( U8 *p_api);

#endif

