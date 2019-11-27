/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc . All Rights Reserved.
 *
 ******************************************************************************
 *
 * $$Id: ipr_cwmp_sig_handler.h $
 *
 *******************************************************************************/

#ifndef _SIG_HANDLE_
#define _SIG_HANDLE_

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <ipr_cwmp_read_config.h>
#include <ManagementServerModel.h>

extern struct sigaction g_act;
extern struct itimerval g_tout_val;
void sig_hdl(void);
void send_tunnel_error_oam(void);

#endif
