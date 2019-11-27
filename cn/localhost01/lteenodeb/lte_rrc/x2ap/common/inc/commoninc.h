/*******************************************************************************
*
*  FILE NAME   : commoninc.h
*
*  DESCRIPTION : This file include the all common required files for X2AP module
*
*  DATE 24-June-2010 NAME Ashutosh Gupta REFERENCE 36.423_880(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/
#ifndef _X2AP_COMM_INC_H_
#define _X2AP_COMM_INC_H_

#include "stacklayer.h"
#include "rrc_defines.h" 
#include "x2ap_types.h"
#include "x2ap_mem_mgr.h" 
#include "x2ap_asn_enc_dec_3gpp.h"
#if 0
#ifdef ARICENT_SCTP_IPR

#include <s_cogbl.h>
#include <s_cotyps.h>
#include <s_osmem.h>
#include <s_uisuif.h>
#include "s_uisusk.h"
#include "s_cspl.h"
#include "sctp.h"
#include "s_select.h"
#include "sig_ipc_intr.h"
#include "sig_typs.h"
#include "sig_cspl_wrap.h"
/*sctp parameters start */
#include "s_mibtyp.h"
/*sctp parameters stop */
#endif
#endif
#define X2AP_MAX_ASN1_BUF_LEN   8192 

#define MAX_NUM_IP_ADDR                 3
#define MAX_PLMN_ID_BYTES               3
#define TAC_OCTET_SIZE                  2
#define MACRO_ENB_ID_OCTET_SIZE         3
#define HOME_ENB_ID_OCTET_SIZE          4

#define X2AP_OAM_MACRO_ENB_ID_PRESENT        0x01
#define X2AP_OAM_HOME_ENB_ID_PRESENT         0x02
#define MAX_PEER_ENB                         32
#define MAX_BUFFER_SIZE                      1024
#define X2AP_SCTP_ERROR -1 
#define SA_FAMILY_T sa_family_t
#define SCTP_SD_OCTET_SIZE          4
#define MAX_NUM_BPLMN 6
#define MAX_MME_GRP_BYTES 2
#define MAX_NEIGHBOURS 256
#define MAX_ENB_POOLS 16

    #define MAX_SERVED_CELLS 8

#define X2AP_ENB_CFG_RETRY_TIMER_VAL_DEFAULT								5000
#define X2AP_ENB_CFG_TIME_TO_WAIT_RETRY_COUNT_DEFAULT			3
#define X2AP_ENB_CFG_TIME_TO_WAIT_OFFSET_TIMER_VAL_DEFAULT	1000
#define X2AP_RESET_RETRY_COUNT_DEFAULT	3
#define X2AP_RESET_RETRY_TIMER_VAL_DEFAULT	3000
#define X2AP_TIME_TO_WAIT_VALUE_DEFAULT 255
#define INVALID_UE_X2AP_ID 0XFFFF
#define MAX_ELEM_TO_PARTIAL_DECODE   2

#ifdef ENDC_ENABLED 
/*NR_DC Code Change Start*/
/* Invalid ID when ext_bit = 4096 and actual id = 4096 */
#define RRC_INVALID_DC_UE_X2AP_ID                           0x10001000
/*NR_DC Code Change Stop*/
#endif

/*L3-X2AP Drop2 Integration Start*/
/*SPR 20633 Start*/
#ifdef LKSCTP
extern U32 init_stack_flag;
S32 g_sctp_server_fd_ipv4 ;
S32 g_sctp_server_fd_ipv6 ;
#else 
extern U32 init_stack_flag_v4;
extern U32 init_stack_flag_v6;
S32 g_sctp_server_fd ;
#endif
/*SPR 20633 End*/
extern U32 is_sctp_stck_env_set;

U8 g_sctp_sd_count ;   /* Maintaining connection count */
/*L3-X2AP Drop2 Integration End */

typedef U32 sctp_assoc_id; 

typedef struct
{
    void *p_msg;
    U32  msg_len;
    x2ap_bool_et ue_ass_msg;
    void *decoded_buffer;
    x2ap_UE_X2AP_ID       oldEnbUeX2apId;
    x2ap_UE_X2AP_ID       newEnbUeX2apId;
    /* SPR_14436_Fix Start */
    U16 stream_id; 
    /* SPR_14436_Fix Stop */
#ifdef ENDC_ENABLED
    /*NR_DC Code Change Start*/
    U32                        mEnbUeX2apId;
    U32                        sgnbUeX2apId;
    /*NR_DC Code Change Stop*/
#endif
}x2ap_peer_enb_msg_t;

#endif

