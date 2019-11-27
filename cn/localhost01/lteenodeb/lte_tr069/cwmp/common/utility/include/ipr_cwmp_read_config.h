/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc . All Rights Reserved.
 *
 ******************************************************************************
 *
 * $$Id: ipr_cwmp_read_config.h $
 *
 *******************************************************************************
 *
 * File Description: This file contains functions to read config from config file. 
 *      
 *
 *******************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   2012/11/07 Anant K Sharma  Initial Version 1.0
 * 
 ******************************************************************************/

#ifndef _READ_CONFIG_H
#define _READ_CONFIG_H

/*******************************************************************************
 *
 *           STANDARD LIBRARY FILES
 *
*******************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<lteTypes.h>
#include<ipr_cwmp_enb_wrapper.h>

/*******************************************************************************
 *
 *               MACROS
 *
 *******************************************************************************/
#define CONFIG_FILE "./ipr_cwmp_client.cfg"
#define CONFIG_BUFFER_SIZE 1024
#define CONFIG_PARAM_LEN 256
#define IPSEC_ENC_KEY_LEN 50
#define IPSEC_AUTH_KEY_LEN 18
#define AUTH_TYPE 10
/*SPR 16151 Fixed Start*/
#define PARAMETER_BUFFER_SIZE 1024
/*SPR 16151 Fixed End*/
  /*SPR 19088 FIXED START*/
#define IP_PARAM_LEN 40
  /*SPR 19088 FIXED END*/
#define AUTHENTICATION_TYPE "AUTHENTICATION_TYPE"
#define AUTHENTICATION_TYPE_SIZE 19
#define NONE_AUTH "None"
#define NONE_AUTH_SIZE 4

//GLU LAYER configurations
extern UInt8 have_tunnel;
extern UInt8 have_ipsec;
extern UInt8 ipsec_net_prefix;
extern UInt8 ipsec_enc_key[IPSEC_ENC_KEY_LEN + 1];
extern UInt8 ipsec_auth_key[IPSEC_AUTH_KEY_LEN + 1];
extern UInt32 recv_port;
extern UInt32 send_port;
extern UInt8  password[CONFIG_PARAM_LEN];
extern UInt8  username[CONFIG_PARAM_LEN];
/* Periodic Inform Changes Start */
extern UInt8  PeriodicInformEnable;
/* Periodic Inform Changes End */
extern UInt32 PeriodicInformInterval;
extern UInt8  ConnectionRequestPassword[CONFIG_PARAM_LEN];
extern UInt8  ConnectionRequestUsername[CONFIG_PARAM_LEN];
extern UInt8  Manufacturer[CONFIG_PARAM_LEN];
extern UInt8  OUI[CONFIG_PARAM_LEN];
extern UInt8  ProductClass[CONFIG_PARAM_LEN];
extern UInt8  SerialNumber[CONFIG_PARAM_LEN];
extern UInt32 g_sleep_ind;
//extern UInt32 g_fact_rst_int;
extern UInt8  check_duplicates_spv_flag;

//TR069 LIB configurations
extern UInt8 log_cfg_path[CONFIG_PARAM_LEN];
extern UInt32 debugger_port;
extern UInt8 log_file_name[CONFIG_PARAM_LEN];
/*SPR 18835 fix start*/
extern UInt8 cert_file_path_shems[CONFIG_PARAM_LEN];
extern UInt8 cert_file_path_ihems[CONFIG_PARAM_LEN];
/*SPR 18835 fix end */
  /*SPR 19088 FIXED START*/

extern UInt8 ip_interface[IP_PARAM_LEN];
  /*SPR 19088 FIXED END*/

extern UInt32 http_server_port;

extern UInt8 auth_type[AUTH_TYPE];

/*********************************************************************************
 *                FUNCTION PROTOTYPES
 *
 ********************************************************************************/
UInt32 getVal(UInt8 *str);
void getValStr(UInt8 *str1, UInt8 *str2);
void read_tr69_lib_config(void);
void read_glue_config(void);

# endif
