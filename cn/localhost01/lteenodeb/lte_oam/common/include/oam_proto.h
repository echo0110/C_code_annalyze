/*****************************************************************************
 * File Name   : oam_proto.h
 *
 * Description : This file contains common prototypes required by OAM modules
 *
 * Revision History:
 *
 * Date         Author           Reference              Comments
 * -----        ------           ---------              --------
 * April,2012  Nitin Agarwal   OAM Design Document      Initial Version
 * Sept,2013   Raveen Sharma   CSR 51606 Fix            Code Changes 
 *                                                      for CSR Fix
 * April,2014  Vinod Vyas    CSR 57877 FIx 
 * June,2014   Avinash Kumar SPR 12175 FIx 
 * July,2014   Avinash Kumar SPR 11315 FIx 
 * July,2014     Aditi Singh   SPR 9620 Fix              Guard timer Added
 * Copyright (c) 2012, Aricent Inc. All Rights Reserved
 ****************************************************************************/

#ifndef OAM_PROTO_H
#define OAM_PROTO_H

/****************************************************************************
 **  INCLUDES
 ****************************************************************************/
#include <lteTypes.h>
#include "oam_types.h"
/* CSR 57877 FIx Start */
#include <oam_mgmt_intf_api.h>
/* CSR 57877 FIx End */
#include <oam_port_utils.h>
#include <advlogging.h>
#include <oam_xml_header.h>
/****************************************************************************
 **  MACROS
 ****************************************************************************/
#define OAM_MAX_FILEPATH_SIZE     255 
#define OAM_MAX_APN_NAME          100 
#define OAM_MAX_NETWORK_IP_PORT    40 
#define MAX_BUF_IPADDR_SIZE        64
#define MAX_BUF_COMMAND_SIZE       64
#define MAX_INTF_LIST_SIZE        500
/* BITSTRING FIX START */
#define OAM_MAX_SHORT_BITSTRING_SIZE    16
#define OAM_MAX_INT_BITSTRING_SIZE    32
#define OAM_MAX_CHAR_BITSTRING_SIZE    8
#define OAM_CELL_ID_BITSTRING_SIZE    28
#define OAM_CSG_ID_BITSTRING_SIZE    27
#define OAM_SID_BITSTRING_SIZE    15
#define OAM_NID_BITSTRING_SIZE    16
#define OAM_REG_ZONE_BITSTRING_SIZE    12
#define OAM_TOTAL_ZONE_BITSTRING_SIZE    3
#define OAM_ZONE_TIMER_BITSTRING_SIZE    3
/* BITSTRING FIX END */

/* SPR 11650 FIX START */
#define OAM_MAX_INT_BITSTRING_SIZE    32 
#define OAM_CELL_ID_BITSTRING_SIZE    28 
/* SPR 11650 FIX END */
/* SPR 12175 FIX START */
#define OAM_CDMA_1XRTT_CELL_ID_BITSTRING_SIZE    47 
/* SPR 13283 START */
#define OAM_MAX_CDMA_1XRTT_INT_BITSTRING_SIZE    48 
#define OAM_MAX_CDMA_HRPD_INT_BITSTRING_SIZE    128 
/* SPR 13283 END */
/* SPR 15161 FIX START */
#define OAM_CDMA_1XRTT_LONG_CODE_STATE_BITSTRING_SIZE                      42
/* SPR 15161 FIX END */
/*SPR 11315 FIX START*/
#define OAM_TAC_BIT_STRING_SIZE				 16
#define OAM_MAX_SHORT_INT_BITSTRING_SIZE		 16
/*SPR 11315 FIX END*/
/* SPR 12175 FIX END */
/* SPR 11650 FIX START */
#define OAM_MACRO_ENB_ID_BITSTRING_SIZE    20 
#define OAM_HOME_ENB_ID_BITSTRING_SIZE     28 
/* SPR 11650 FIX END  */
/****************************************************************************
 **  ENUMERATIONS / STRUCTURES
 ****************************************************************************/
/*BUG_FIX_ID : 597*/
AdvL_LogLevel oam_convert_log_level (SInt32 oam_log_level);   
typedef struct oam_get_log_cat_l2_all
{
    UInt64 mac_logCategory;
    UInt64 rlc_logCategory;
    UInt64 pdcp_logCategory;
    UInt64 gtpu_logCategory;
}oam_get_log_cat_l2_all_t;

typedef struct oam_get_log_level_l2_all
{
        UInt32 mac_loglevel;
        UInt32 rlc_loglevel;
        UInt32 pdcp_loglevel;
        UInt32 gtpu_loglevel;
}oam_get_log_level_l2_all_t;

void oam_reset_reason_code
(
oam_reset_reason_code_et reset_reason_code_obj
);

lgr_log_level_et oam_map_log_level_to_lgr_log_level_type (log_priority_et log_level);

typedef enum 
{ 
    OAM_LOG_CATEGORY_FILE, 
    OAM_LOG_CATEGORY_CONSOLE, 
    OAM_LOG_CATEGORY_SYSLOG, 
    OAM_LOG_CATEGORY_NETWORK 

}oam_log_category_et; 

typedef struct oam_log_data 
{ 
    Char8   filepath[OAM_MAX_FILEPATH_SIZE]; 
    Char8   apn_name[OAM_MAX_APN_NAME]; 
    Char8   network_ip_port[OAM_MAX_NETWORK_IP_PORT]; 

}oam_log_data_st; 

#ifdef OAM_SON_ENABLE
oam_return_et
oam_send_son_shutdown_req
(
    oam_son_shutdown_req_t  *p_oam_son_shutdown_req,
    UInt16                   src_module_id,
    UInt16                   transaction_id,
    oam_error_code_et       *p_error_code
);
#endif

void 
oam_construct_n_send_set_resp_to_usr
(
    UInt8               resp_result,
    UInt32              resp_error_code,
    UInt8              *err_string,
    UInt8               err_string_size,
    UInt8               oam_version, 
    UInt16              src_id,
    UInt16              dst_id, 
    UInt16              req_trans_id,
    UInt8               api_version,
    UInt8               api_hdr_resvd_byte,
    oam_error_code_et  *p_err_code
);

void 
oam_construct_n_send_set_cell_context_resp_to_usr
(
    UInt8               resp_result,
    UInt32              resp_error_code,
    UInt8              *err_string,
    UInt8               err_string_size,
    UInt8               oam_version, 
    UInt16              src_id,
    UInt16              dst_id, 
    UInt16              req_trans_id,
    UInt8               api_version,
    UInt8               api_hdr_resvd_byte,
    oam_error_code_et  *p_err_code
);



UInt8
oam_get_alarm_probable_cause_id_by_cause
(
    char  *cause
);

UInt8
oam_get_alarm_event_id_by_event
(
    char  *event
);

UInt8
oam_get_alarm_severity_id_by_severity
(
    char  *severity
);

UInt8 
oam_get_alarm_report_id_by_report
(
	char  *report
);

oam_return_et
oam_cp_pack_SInt32
(
    void    *p_dest,
    void    *p_src
,   char    *varname
);

oam_return_et
oam_cp_pack_S8
(
    void    *p_dest,
    void    *p_src
,   char    *varname
);


oam_return_et
oam_cp_pack_U16
(
    void    *p_dest,
    void    *p_src
,  char    *varname
);

oam_return_et
oam_cp_pack_U32
(
    void    *p_dest,
    void    *p_src
 ,   char    *varname
);


/* - SPR 17439 */
oam_return_et create_time_stamp(void);
/* - SPR 17439 */

oam_return_et
oam_mem_alloc
(
    size_t              size,  /* Size of buffer which will be allocated */
    void              **p_msg, //Nitin ---check if address is required ??
    oam_error_code_et *p_error_code
);

void oam_construct_api_header
(
    UInt8  *p_header,      /* CSPL header */
    UInt8   version_id,     /* API version Id */
    UInt16  src_module_id,  /* Source module Id */
    UInt16  dst_module_id,  /* Destination module Id */
    UInt16  api_id,         /* API Id */
    UInt16  api_buf_size    /* API buffer size */
);



oam_return_et
oam_mem_free
(
    void *p_buffer, /* Pointer to buffer which will be freed */
oam_error_code_et *p_error_code

);

oam_return_et
oam_cp_unpack_SInt32
(
    void    *p_dest,
    void    *p_src
     , char    *varname
);

oam_return_et
oam_cp_pack_SInt8
(
    void    *p_dest,
    void    *p_src
,    char    *varname
);

oam_return_et
oam_cp_pack_SInt16
(
    void    *p_dest,
    void    *p_src
,    char    *varname
);

oam_return_et
oam_cp_pack_UInt16
(
    void    *p_dest,
    void    *p_src
,    char    *varname
);

oam_return_et
oam_cp_pack_UInt8
(
    void    *p_dest,
    void    *p_src
,   char    *varname
);

oam_return_et
oam_cp_pack_Char8
(
    void    *p_dest,
    void    *p_src
,   char    *varname
);

void
oam_msg_mem_free
(
    void *p_buffer /* Pointer to buffer which will be freed */
);

oam_return_et
oam_cp_pack_U64
(
    void    *p_dest,
    void    *p_src
   ,char    *varname
);

oam_return_et
oam_cp_pack_UInt32
(
    void    *p_dest,
    void    *p_src
   ,char    *varname
);

oam_return_et
oam_cp_pack_S16
(
    void    *p_dest,
    void    *p_src
   ,char    *varname
);

oam_return_et
oam_cp_pack_S32
(
    void    *p_dest,
    void    *p_src
    ,char    *varname
);

oam_return_et
oam_cp_unpack_UInt8
(
    void    *p_dest,
    void    *p_src
   ,char    *varname
);

oam_return_et
oam_cp_unpack_Char8
(
 void    *p_dest,
 void    *p_src
 ,   char    *varname
 );

oam_return_et
oam_cp_unpack_UInt16
(
    void    *p_dest,
    void    *p_src
    ,char    *varname
);

oam_return_et
oam_cp_unpack_U64
(
    void    *p_dest,
    void    *p_src
   ,char    *varname
);

oam_return_et
oam_cp_unpack_UInt32
(
    void    *p_dest,
    void    *p_src
   ,char    *varname
);

oam_return_et
oam_cp_unpack_SInt8
(
    void    *p_dest,
    void    *p_src
   ,char    *varname
);


void
oam_construct_mgmt_api_hdr
(
    UInt8           *p_header,      /* pointer to interface header */
    UInt16          transaction_id, /* Interface transaction identifier */
    oam_module_id_t src_module_id,  /* Source module identifier */
    oam_module_id_t dst_module_id,  /* Destination module identifier */
    UInt16          api_id,         /* API identifier */
    UInt16          api_buf_size,   /* API buffer size */
    /*SPR 17777 fix*/
	UInt8			resvd
);



void
oam_construct_interface_api_header
(
    UInt8              *p_header,      /* pointer to interface header */
    UInt16             transaction_id, /* Interface transaction identifier */
    oam_module_id_t    src_module_id,  /* Source module identifier */
    oam_module_id_t    dst_module_id,  /* Destination module identifier */
    UInt16             api_id,         /* API identifier */
    UInt16             api_buf_size,    /* API buffer size */
    UInt8              cell_index      /* Cell Index */
);

oam_return_et
oam_send_success_failure_resp
(
	oam_return_et 			response,
	UInt16				transaction_id,
	UInt16				dest_id,
	oam_error_code_et 		*p_error_code
);

/* CSR_59405_FIX_START */
oam_return_et
oam_send_call_trace_start_resp
(
        oam_return_et                   response,
        UInt16                          transaction_id,
        UInt16                          dest_id,
        oam_error_code_et               *p_error_code
);
oam_return_et
oam_send_call_trace_stop_resp
(
        oam_return_et                   response,
        UInt16                          transaction_id,
        UInt16                          dest_id,
        oam_error_code_et               *p_error_code
);
/* CSR_59405_FIX_END */


oam_return_et
oam_cp_unpack_S16
(
    void    *p_dest,
    void    *p_src
   ,char    *varname
);

oam_return_et
oam_cp_unpack_S32
(
    void    *p_dest,
    void    *p_src
   ,char    *varname
);


UInt32 
oam_display_command_choice
(
    void
);

UInt32 
oam_user_call_callback
(
    UInt8*
);

UInt32 
oam_user_enable_logs
(
    void
);

oam_return_et  
oam_rrc_handle_log_enable_resp
(
    void   *p_api_buf,
    UInt16  transaction_id,
    UInt16  msg_length
);


void oam_trace_message
(
    SInt32 log_level,              /* log level of this message */
    SInt8 *pFile,
    SInt8 *pFun,
    UInt16 Line,
    SInt8* format,         /* format string like in printf */
    ...                         /* optional parameters */
);

UInt16 
get_word_from_header
(
    UInt8 *
);

UInt16 
oam_get_fresh_trans_id
( 
    UInt16 
);

void* 
oam_msg_mem_get
(
    size_t 
);


oam_return_et 
oam_qcli_send_All_response
(
   void   *p_msg,
   UInt16  trans_id,
   UInt16  api_id,
   UInt16  msg_size
);
/*oam_send_message() will be used to send messages from OAM module */
oam_return_et oam_send_message
(
    void                *p_msg,     /* Message that will be passed */
    rrc_module_id_t      dst_module,  /* Module id for which message will be passed */
    oam_error_code_et   *p_error_code
);
/* oam_qcli_intf_send_message() introduced to send messages from CLI interface*/
oam_return_et oam_qcli_intf_send_message
(
 void            *p_msg,     /* Message that will be passed */
 rrc_module_id_t dst_module,  /* Module id for which message will be passed */
 oam_error_code_et *p_error_code
 );
/*SPR 20172 FIX START*/
#define oam_rrm_send_message(p_msg,dst_module) do {\
	oam_rrm_send_message_ex(p_msg,dst_module);\
	p_msg = PNULL;\
}while(0)

oam_return_et oam_rrm_send_message_ex
/*SPR 20172 FIX End*/
(
    void            *p_msg,     /* Message that will be passed */
    rrc_module_id_t  dst_module  /* Module id for which message will be passed */
);

int 
oam_multi_thd_init
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

short int
oam_eth_intf_up
(
    void
);


void
oam_construct_qcli_header
(
    unsigned char *p_header,
    UInt16 transaction_id,
    UInt16 src_module_id,
    UInt16 dst_module_id,
    UInt16 api_id,
    UInt16 api_buf_size
);


oam_return_et 
oam_qcli_send_resp
(
    void *
);


oam_return_et
oam_send_l2All_success_resp
(
    oam_return_et      response,
    UInt16             transaction_id,
    UInt16             dest_id,
    oam_error_code_et *p_error_code
);


oam_timer_t 
oam_start_new_timer
(
    UInt16       sub_module_id,
    UInt16       expiry_thread_id,
    UInt16       timer_type,
    UInt32       timer_duration, /* unit: milliseconds */
    void        *buf,
    UInt16       buf_size,
    oam_bool_et  is_repeated
);
/* SPR 22465 Fix Start */
oam_timer_t 
oam_start_new_timer_and_maintain_trans_id
(
    UInt16       sub_module_id,
    UInt16       expiry_thread_id,
    UInt16       timer_type,
    UInt32       timer_duration, /* unit: milliseconds */
    void        *buf,
    UInt16       buf_size,
    oam_bool_et  is_repeated,
    UInt16       trans_id
);
/* SPR 22465 Fix End */
/* SPR_9620_CHANGES_START */
oam_timer_t
oam_start_guard_timer(
        UInt16       trans_id,
        UInt16       sub_module_id,
        UInt16       expiry_thread_id,
        UInt16       timer_type,
        UInt32       timer_duration, /* unit: milliseconds */
        void         *buf,
        UInt16       buf_size,
        oam_bool_et  is_repeated
        );
/* SPR_9620_CHANGES_END */


UInt8
oam_get_param_value
(
    char              *full_param_name,
    char              *param_val,
    oam_error_code_et *p_error_code
);

void 
encode_get_params_resp_ctrl_blk
(
    char  *p_ctrl_blk, 
    char   resp_status,
    UInt16 err_code, 
    UInt8  resvd1, 
    UInt32 resvd2, 
    UInt8  err_str_len,
    char  *err_str
);

    /*SPR 17777 fix*/
UInt16 
oam_get_new_trans_id
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/*BUG_FIX_ID : 743*/
void oam_fetch_convert_seconds_to_timestamp
(
time_t now,
Char8 *time_buffer
);


oam_return_et
oam_send_file_download_ind_resp
(
    UInt8               status,
    UInt16   error_code,
    UInt16              transaction_id,
    UInt8               *err_string
);

void 
oam_fetch_current_timestamp
(
    Char8 *time_buffer
);

oam_return_et 
oam_isValidIp4 
(
    UInt8 *p_str
);

int 
oam_isValidIp6 
(
    UInt8 *p_str
);
/* SPR 12175 FIX START */
void 
oam_conversion_from_int64_to_byte_string
(
    UInt8* dest, 
    UInt64 source,
    UInt8 bitstring_size
);
/* SPR 12175 FIX END */
/* SPR 13283 START */
void 
oam_conversion_from_int128_to_byte_string
(
    UInt8* dest, 
    UInt64 source,
    UInt8 bitstring_size
);
/* SPR 13283 FIX END */

/* SPR 11650 FIX START */
void
oam_conversion_from_macro_enb_to_byte_string(
    UInt8* dest,
    UInt32 source,
    UInt8 bitstring_size
    );

void oam_cmn_convert_byte_string_to_integer(
		UInt8        *p_bytes,
		UInt16       num_of_bytes,
		UInt32       *p_value,
		UInt32 		  bytestring_size);

/* SPR 11650 FIX START */

void 
oam_conversion_from_int_to_byte_string
(
    UInt8* dest, 
    UInt32 source,
    UInt8 bitstring_size
);
void 
oam_conversion_for_macro_enb_from_int_to_byte_string
(
    UInt8* dest, 
    UInt32 source,
    UInt8 bitstring_size
);

/* SPR 11650 FIX END */

void 
oam_convert_int_to_byte_string
(
    UInt8* dest, 
    UInt32 source
);

void 
oam_convert_3byte_int_to_byte_string
(
    UInt8* dest, 
    UInt32 source
);

/* BITSTRING FIX START */
void
oam_conversion_from_short_to_byte_string
(
 UInt8* dest,
 UInt32 source,
 UInt8 bitstring_size
 );

void
oam_conversion_from_char_to_byte_string
(
 UInt8* dest,
 UInt32 source,
 UInt8 bitstring_size
 );

/* BITSTRING FIX END */

/*SPR 11315 FIX START*/
void
oam_conversion_bitstring_short_to_byte_string
(
    UInt8* dest,
    UInt32 source
);

void 
oam_convert_short_to_byte_string
(
    UInt8* dest, 
    UInt32 source
);

/* SPR 22398 Fix Start */
void oam_cmn_convert_bytes_to_integer_for_LAC
(
    UInt8        *p_bytes,
    UInt32       *p_value
);    
/* SPR 22398 Fix End */
void oam_cmn_convert_bytes_to_integer
(
    UInt8        *p_bytes,
    UInt16       num_of_bytes,
    UInt32       *p_value
);


#ifdef OAM_SON_ENABLE
oam_return_et
oam_parse_son_nr_add_resp

(
    UInt8  *p_src,
    oam_son_nr_add_resp_t *p_oam_son_nr_add_resp,	
    oam_error_code_et *p_error_code
);

oam_return_et
oam_parse_son_nr_update_resp
(
    UInt8  *p_src,
    oam_son_nr_update_resp_t *p_oam_son_nr_update_resp,	
    oam_error_code_et *p_error_code
);

oam_return_et
oam_parse_son_nr_delete_resp
(
    UInt8  *p_src,
    oam_son_nr_delete_resp_t *p_oam_son_nr_delete_resp,	
    oam_error_code_et *p_error_code
);

oam_return_et
oam_send_son_nr_enb_add_req
(
    oam_son_nr_enb_add_req_t  *p_nr_enb_add_req,
    UInt16                        src_module_id,
    UInt16                        transaction_id,
    oam_error_code_et          *p_error_code
);

oam_return_et
oam_send_son_nr_enb_update_req
(
    oam_son_nr_enb_update_req_t  *p_nr_enb_update_req,
    UInt16                       src_module_id,
    UInt16                       transaction_id,
    oam_error_code_et            *p_error_code
);

oam_return_et
oam_send_son_nr_enb_delete_req
(
    oam_son_nr_enb_delete_req_t  *p_nr_enb_delete_req,
    UInt16                       src_module_id,
    UInt16                       transaction_id,
    oam_error_code_et            *p_error_code
);

oam_return_et
oam_send_son_nr_enb_retrieve_req
(
    oam_son_nr_enb_retrieve_req_t  *p_nr_enb_retrieve_req,
    UInt16                         src_module_id,
    UInt16                         transaction_id,
    oam_error_code_et              *p_error_code
);

oam_return_et
oam_parse_son_nr_enb_add_resp
(
    UInt8  *p_src,
    oam_son_nr_enb_add_resp_t *p_nr_enb_add_resp,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_parse_son_nr_enb_update_resp
(
    UInt8  *p_src,
    oam_son_nr_enb_update_resp_t *p_oam_son_nr_enb_update_resp,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_parse_son_nr_enb_delete_resp
(
    UInt8  *p_src,
    oam_son_nr_enb_delete_resp_t *p_oam_son_nr_enb_delete_resp,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_parse_son_nr_enb_retrieve_resp
(
    UInt8  *p_src,
    oam_son_nr_enb_retrieve_resp_t *p_oam_son_nr_enb_retrieve_resp,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_send_son_pci_selection_req
(
    oam_son_pci_selection_req_t  *p_pci_selection_req,
    UInt16                 src_module_id,
    UInt16                 transaction_id,
    oam_error_code_et        *p_error_code
);

oam_return_et
oam_parse_son_x2_link_up_ind

(
    UInt8  *p_src,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_parse_son_x2_link_down_ind
(
    UInt8  *p_src,
    oam_error_code_et *p_error_code
);
oam_return_et
oam_parse_son_pci_selection_resp
(
    UInt8  *p_src,
    oam_son_pci_selection_resp_t *p_oam_son_pci_selection_resp,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_parse_son_pci_reselection_resp
(
    UInt8  *p_src,
    oam_son_pci_reselection_resp_t *p_oam_son_pci_reselection_resp,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_parse_son_pci_conflict_confusion_ind
(
    UInt8  *p_src,
    oam_son_pci_conflict_confusion_ind_t *p_pci_conflict_confusion_ind,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_send_son_pci_reselection_req
(
    oam_son_pci_reselection_req_t  *p_pci_reselection_req,
    UInt16                 src_module_id,
    UInt16                 transaction_id,
    oam_error_code_et      *p_error_code
);

oam_return_et
oam_parse_son_es_enable_resp
(
    oam_son_es_enable_resp_t *p_oam_son_es_enable_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_parse_son_es_disable_resp
(
    oam_son_es_disable_resp_t *p_oam_son_es_disable_resp,
    UInt8                        *p_src,
    SInt32                       length_left,
    SInt32                       *p_length_read,
    oam_error_code_et         *p_error_code
);

oam_return_et
oam_parse_son_es_cell_switch_off_resp
(
    oam_son_es_cell_switch_off_resp_t *p_oam_son_es_cell_switch_off_resp,
    UInt8                             *p_src,
    SInt32                               length_left,
    SInt32                               *p_length_read,
    oam_error_code_et                 *p_error_code
);

oam_return_et
oam_parse_son_es_cell_switch_on_resp
(
    oam_son_es_cell_switch_on_resp_t *p_oam_son_es_cell_switch_on_resp,
    UInt8                            *p_src,
    SInt32                            length_left,
    SInt32                           *p_length_read,
    oam_error_code_et                *p_error_code
);

oam_return_et
oam_send_son_cell_info_ind
(
    oam_son_cell_info_ind_t  *p_oam_son_cell_info_ind,
    UInt16                      src_module_id,
    UInt16                      transaction_id,
    oam_error_code_et        *p_error_code
);

oam_return_et
oam_send_son_es_enable_req
(
    son_oam_es_enable_req_t  *p_oam_son_es_enable_req,
    UInt16                      src_module_id,
    UInt16                      transaction_id,
    oam_error_code_et        *p_error_code
);

oam_return_et
oam_send_son_es_disable_req
(
    oam_son_es_disable_req_t  *p_oam_son_es_disable_req,
    UInt16                       src_module_id,
    UInt16                       transaction_id,
    oam_error_code_et         *p_error_code
);

oam_return_et
oam_send_son_es_cell_switch_on_req
(
    oam_son_es_cell_switch_on_req_t  *p_oam_son_es_cell_switch_on_req,
    UInt16                 src_module_id,
    UInt16                 transaction_id,
    oam_error_code_et   *p_error_code
);

oam_return_et
oam_send_son_es_cell_switch_off_req
(
    son_oam_es_cell_switch_off_req_t  *p_oam_son_es_cell_switch_off_req,
    UInt16                             src_module_id,
    UInt16                             transaction_id,
    oam_error_code_et                 *p_error_code
);


oam_return_et
oam_send_son_anr_meas_config_req
(
    oam_son_anr_meas_config_req_t *p_req,
    UInt16                         src_module_id,
    UInt16                         transaction_id,
	oam_error_code_et             *p_error_code
);

oam_return_et
oam_send_son_nr_update_req
(
    oam_son_nr_update_req_t  *p_req,
    UInt16                    src_module_id,
    UInt16                    transaction_id,
    oam_error_code_et        *p_error_code
);

oam_return_et
oam_send_son_nr_delete_req
(
    oam_son_nr_delete_req_t  *p_req,
	UInt16                    src_module_id,
	UInt16                    transaction_id,
	oam_error_code_et        *p_error_code
);

oam_return_et
oam_send_son_nr_retrieve_req
(
    oam_son_nr_retrieve_req_t  *p_req,
	UInt16              src_module_id,
	UInt16             transaction_id,
	oam_error_code_et  *p_error_code
);

oam_return_et
oam_parse_son_anr_meas_config_resp
(
    UInt8 *p_src, 
    SInt32 length_left,
    SInt32 *p_length_read,
    oam_son_anr_meas_config_res_t *p_resp,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_parse_son_nr_retrieve_resp
(
    UInt8  *p_src,
    oam_son_nr_retrieve_resp_t *p_oam_son_nr_retrieve_resp,
    oam_error_code_et *p_error_code
);

#endif
void oam_get_cli_communication_info
(
    oam_cli_comm_info_t * comm_info
);

oam_return_et
oam_search_integer_to_interger
(
    char * parmeter_value,
    int   * par_table_address,
    oam_counter_t count
);

void oam_cmn_convert_bytes_to_integer
(
     UInt8        *p_bytes,
     UInt16       num_of_bytes,
     UInt32       *p_value
);

UInt32 
oam_tokenize_string_to_u16
(
    Char8* p_str,
    const Char8* p_delim,
    UInt32 max_tokens,
    UInt16 *p_tokens
);

UInt16 
oam_trim_spaces
(
    Char8* p_src, 
    Char8* p_dest
);

/* + SPR 17665 + */
UInt32
oam_tokenize_string_to_u8
(
    Char8* p_str,
    const Char8* p_delim,
    UInt32 max_tokens,
    UInt8 *p_tokens
);
/* - SPR 17665 - */

oam_return_et
oam_sdm_send_active_ind
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);
/* SPR 21496 +- */ 
oam_return_et
oam_fetch_sctp_data_from_l3_s1ap
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

oam_return_et
oam_fetch_sctp_data_from_l3_x2ap
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
);

/* LWA changes start*/
/* LWA changes end*/

int 
oam_init_logging
(
    oam_log_category_et log_category,
    oam_log_data_st    *log_data,
    int                 log_level,
    int                 log_size
); 

oam_return_et
oam_validate_tr196_schema
(
    xmlDoc **p_doc2
);
/* SPR 15438 Start */ 
void oam_log_message(SInt32 oam_log_level,SInt8 *pFile,SInt8 *pFun,UInt32 Line,SInt8* format, ...);
/* SPR 15438 End */ 
#ifdef OAM_SON_ENABLE
oam_return_et 
oam_send_son_nr_add_upd_req
(
    UInt16            api_id,
    UInt16            transaction_id,
    oam_error_code_et *p_error_code
);


UInt8 
are_plmns_same
(
    son_cell_plmn_info_t *p1, 
    son_cell_plmn_info_t *p2
);

SInt16 
oam_cgi_search_in_neigh_list
(
    void                  *cgi, 
    void                  *nr_list,
    UInt16                list_size,
    UInt32 *valid_bitmap,
    oam_son_neigh_type_et neigh_type
);
#endif
oam_return_et
oam_cp_unpack_SInt16
(
  void    *p_dest,
  void    *p_src 
, char    *varname
 );





/*OAM_REVIEW_CHANGES*/
void set_bitmask_add_req_inter_rat_access_mode
(
    int loop,
    void *p_cell_reconfig_req
);

/*LTE_OAM_CA_PARAMETER_SUPPORT */
void set_bitmask_is_ca_eligible
(
    int loop,
    void * p_cell_reconfig_req
);


void set_bitmask_num_of_report
(
    int loop,
     void * p_cell_reconfig_req
);

void set_bitmask_ca_applicable_bitrate_dl
(
    int loop,
    void * p_cell_reconfig_req
);


void set_bitmask_is_masa
(
    int loop,
    void * p_cell_reconfig_req
);


void set_bitmask_scell_deactivation_timer
(
    int loop,
    void * p_cell_reconfig_req
);


void set_bitmask_load_calcualtion_window_size
(
    int loop,
    void * p_cell_reconfig_req
);


void set_bitmask_load_disparity_threshold
(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_mcs_diff_threshold
(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_prb_usage_water_mark
(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_dl_deact_command_thresh
(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_dl_deact_mcs_low_thresh
(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_dl_qload_up_threshold
(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_dl_qload_low_threshold
(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_act_deact_trigger_count_threshold
(
    int loop,
    void * p_cell_reconfig_req
);
/* SPR 16422 START */
void set_bitmask_cqi_validity_timer
(
	int loop,
	void * p_cell_reconfig_req
);
/* SPR 16422 END */
/*LTE_OAM_CA_PARAMETER_SUPPORT */

void set_bitmask_add_req_intra_rat_access_mode
(
    int loop,
    void *p_cell_reconfig_req
);


void set_bitmask_add_req_intra_rat_broadcast_status
(
    int loop,
    void *p_cell_reconfig_req
);


void set_bitmask_add_req_intra_rat_cm_cio
(
    int loop,
    void * p_cell_reconfig_req
    
);
void set_bitmask_add_req_intra_rat_root_sequence_index 
(
 int loop,
    void *p_cell_reconfig_req
);

void set_bitmask_add_req_intra_rat_zero_correlation_zone_config
(
 int loop,
    void *p_cell_reconfig_req
);

void set_bitmask_add_req_intra_rat_high_speed_flag 
(
 int loop,
    void *p_cell_reconfig_req
);

void set_bitmask_add_req_intra_rat_prach_frequency_offset
(
 int loop,
    void *p_cell_reconfig_req
 );

void set_bitmask_add_req_intra_rat_prach_configuration_index
(
 int loop,
    void *p_cell_reconfig_req
 );

void
set_bitmask_add_req_intra_rat_ul_earfcn
(
 int loop,
    void *p_cell_reconfig_req
 );
/*OAM_REVIEW_CHANGES*/
#ifdef OAM_SON_ENABLE
/* RT SPR 8154 FIX START */
SInt16 oam_arfcn_search_in_meas_config_list(void* arfcn,
                                      void *meas_config_list,
                                      UInt16 list_size,
				                       UInt32 *valid_bitmap,	
                                      oam_son_meas_config_type_et meas_type);
/* RT SPR 8154 FIX END */

#endif



#define OAM_CP_PACK_UINT8(ppBuff, pStruct, aString) \
oam_cp_pack_UInt8((ppBuff),(pStruct), (aString))

#define OAM_CP_PACK_CHAR8(ppBuff, pStruct, aString) \
oam_cp_pack_UInt8((ppBuff),(pStruct), (aString))

#define OAM_CP_PACK_UINT16(ppBuff, pStruct, aString) \
oam_cp_pack_UInt16((ppBuff),(pStruct), (aString))


#define OAM_CP_PACK_UINT32(ppBuff, pStruct, aString) \
oam_cp_pack_UInt32((ppBuff),(pStruct), (aString))

#define OAM_CP_PACK_UINT64(ppBuff, pStruct, aString) \
oam_cp_pack_U64((ppBuff),(pStruct), (aString))

#define OAM_CP_PACK_SINT8(ppBuff, pStruct, aString) \
oam_cp_pack_SInt8((ppBuff),(pStruct), (aString))

#define OAM_CP_PACK_SINT16(ppBuff, pStruct, aString) \
oam_cp_pack_SInt16((ppBuff),(pStruct), (aString))

#define OAM_CP_PACK_SINT32(ppBuff, pStruct, aString) \
oam_cp_pack_SInt32((ppBuff),(pStruct), (aString))

#define OAM_CP_PACK_SINT64(ppBuff, pStruct, aString) \
oam_cp_pack_SInt64((ppBuff),(pStruct), (aString))

#define OAM_CP_UNPACK_UINT8(ppBuff, pStruct, aString) \
oam_cp_unpack_UInt8((ppBuff),(pStruct), (aString))

#define OAM_CP_UNPACK_CHAR8(ppBuff, pStruct, aString) \
oam_cp_unpack_Char8((ppBuff),(pStruct), (aString))

#define OAM_CP_UNPACK_UINT16(ppBuff, pStruct, aString) \
oam_cp_unpack_UInt16((ppBuff),(pStruct), (aString))

#define OAM_CP_UNPACK_UINT32(ppBuff, pStruct, aString) \
oam_cp_unpack_UInt32((ppBuff),(pStruct), (aString))

#define OAM_CP_UNPACK_UINT64(ppBuff, pStruct, aString) \
oam_cp_unpack_U64((ppBuff),(pStruct), (aString))

#define OAM_CP_UNPACK_SINT8(ppBuff, pStruct, aString) \
oam_cp_unpack_SInt8((ppBuff),(pStruct), (aString))

#define OAM_CP_UNPACK_SINT16(ppBuff, pStruct, aString) \
oam_cp_unpack_SInt16((ppBuff),(pStruct), (aString))

#define OAM_CP_UNPACK_SINT32(ppBuff, pStruct, aString) \
oam_cp_unpack_SInt32((ppBuff),(pStruct), (aString))

#define OAM_CP_UNPACK_SINT64(ppBuff, pStruct, aString) \
oam_cp_unpack_SInt64((ppBuff),(pStruct), (aString))

void
set_bitmask_connected_mode_mob_common_params_for_eutra
(
    int loop,
    void * p_cell_reconfig_req
);


void
set_bitmask_irat_meas_quant_utra_fdd
(
    int loop,
    void * p_cell_reconfig_req
);


void
set_bitmask_irat_meas_quant_utra_tdd
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_irat_q_offset_cdma2000
(
    int loop,
    void * p_cell_reconfig_req
);


void
set_bitmask_irat_meas_quan_cdma2000
(
    int loop,
    void * p_cell_reconfig_req
);


void
set_bitmask_irat_b1_thresh_cdma2000
(
    int loop,
    void * p_cell_reconfig_req
);


void
set_bitmask_ran_info_rf_params_ul_bw
(
    int loop,
    void * p_cell_reconfig_req	
);


void 
set_bitmask_physical_layer_params_pdsch
(
    int loop,
    void * p_cell_reconfig_req
);


void 
set_bitmask_physical_layer_params_srs_max_up_pts
(
    int loop,
    void * p_cell_reconfig_req
);


void 
set_bitmask_physical_layer_params_srs
(
    int loop,
    void * p_cell_reconfig_req
);

void 
set_bitmask_physical_layer_params_prach
(
    int loop,
    void * p_cell_reconfig_req
);


void 
set_bitmask_physical_layer_params_pucch_tdd_ack_nack
(
    int loop,
    void * p_cell_reconfig_req
);


void 
set_bitmask_physical_layer_params_pucch_cqi_sinr_value
(
    int loop,
    void * p_cell_reconfig_req
);


void set_bitmask_physical_layer_params_pucch

(
    int loop,
    void * p_cell_reconfig_req
);


void set_bitmask_physical_layer_params_pusch

(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_physical_layer_params_ul_reference_signal_power

(
    int loop,
    void * p_cell_reconfig_req
);


void set_bitmask_physical_layer_params_ul_power_control

(
    int loop,
    void * p_cell_reconfig_req
);


void set_bitmask_physical_layer_params_prs

(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_physical_layer_params_addl_physical_layer_params

(
    int loop,
    void * p_cell_reconfig_req
);


void set_bitmask_physical_layer_params_addl_physical_layer_pucch_params

(
    int loop,
    void * p_cell_reconfig_req
);


void set_bitmask_physical_layer_params_addl_physical_layer_pusch_params

(
    int loop,
    void * p_cell_reconfig_req
);


void set_bitmask_physical_layer_params_addl_physical_layer_ul_reference_params

(
    int loop,
    void * p_cell_reconfig_req
);


void set_bitmask_physical_layer_params_tdd_frame_structure

(
    int loop,
    void * p_cell_reconfig_req
);


void set_bitmask_physical_layer_params

(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_mac_layer_params_ul_sync_loss_timer

(
    int loop, 
    void * p_cell_reconfig_req
);

void set_bitmask_mac_layer_params_ul_sync_loss_timer

(
    int loop, 
    void * p_cell_reconfig_req
);

void set_bitmask_rlc_layer_params

(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_mac_layer_params

(
    int loop, 
    void * p_cell_reconfig_req
);


void set_bitmask_rrc_timers_and_constants

(
    int loop,
    void * p_cell_reconfig_req
);


void set_bitmask_rrc_timers_and_constants

(
    int loop,
    void * p_cell_reconfig_req
);


void
set_bitmask_irat_b1_thresh_cdma2000
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_ran_info_rf_params
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_ran_info_s1ap_params
(
    int loop, 
    void * p_cell_reconfig_req   
);

void set_bitmask_mac_layer_params_drx

(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_rrc_timers_and_constants_timers

(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_rrc_timers_and_constants_constants

(
    int loop,
    void * p_cell_reconfig_req
);

oam_return_et
oam_file_download_from_server
(
    oam_nms_ftp_server_params_t *p_oam_nms_ftp_server_params,
	CURLcode* curl_code,
    oam_error_code_et *p_error_code
);

oam_return_et
oam_file_upload_to_server
(
        oam_nms_ftp_server_params_t *p_oam_nms_ftp_server_params,
        char * file_name,
	UInt32 *curl_code
);

#ifdef OAM_SON_ENABLE

/* OAM BCOM Code Comment Changes Start */
void
oam_nw_scan_set_bitmask_carrier_ch_width
(
    int loop,
    void * p_cell_reconfig_req
);
/* OAM BCOM Code Comment Changes Start */

void
oam_nw_scan_set_bitmask_earfcn_dl_list
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_son_anr_cm_offset_freq
(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_son_anr_utran_gap_offset_gp0
(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_son_anr_utran_gap_offset_gp1
(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_son_anr_eutran_gap_offset_gp0
(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_son_anr_eutran_gap_offset_gp1
(
    int loop,
    void * p_cell_reconfig_req
);
/* RT SPR 8154 FIX START */

void set_bitmask_son_geran_t_reselection
(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_son_geran_t_reselection_sf
(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_son_geran_qrxlevmin
(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_son_geran_arfcn_spacing
(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_son_geran_arfcn_bitmap
(
    int loop,
    void * p_cell_reconfig_req
);

void
set_bitmask_son_geran_offset_freq
(
    int                          loop,
    void *p_son_inter_freq_carr_params
);

void set_bitmask_son_geran_exp_arfcn_list
(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_son_geran_pmax_geran
(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_son_geran_ncc_permitted
(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_son_geran_cell_reselection_priority
(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_son_geran_threshX_low
(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_son_geran_threshX_high
(
    int loop,
    void * p_cell_reconfig_req
);

void set_bitmask_son_geran_cell_reselection_priority
(
    int loop,
    void * p_cell_reconfig_req
);
#endif
/* RT SPR 8154 FIX END */
/* SPR 15527 START */

void set_bitmask_tti_bundling
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_packet_error_loss_rate
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_dscp
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_sps_config_enabled
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_sps_config_dl
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_sps_config_ul
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_lossless_ho_required
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_dl_forwarding_applicability
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_ue_inactivity_timer_config
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_ul_min_bitrate
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_dl_min_bitrate
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_rlc_mode
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_sn_field_len_dl_rlc
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_sn_field_len_ul_rlc
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_t_reordering_am
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_t_reordering_um
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_ul_am_poll_byte
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_ul_am_rrc_max_retx_threshold
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_pdcp_discard_timer
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_rlc_am_status_report_required
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_rlc_am_enb_status_report_required
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_rlc_um_pdcp_sn_size 
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_enable_rohc
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_max_cid
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_rohc_profile0x0001
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_rohc_profile0x0002
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_rohc_profile0x0003
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_rohc_profile0x0003
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_rohc_profile0x0004
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_rohc_profile0x0006
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_rohc_profile0x0101
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_rohc_profile0x0102
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_rohc_profile0x0103
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_rohc_profile0x0104
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_max_harq_tx
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_max_harq_retrans
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_logical_channel_group
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_sr_configuration
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_t_status_prohibit
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_t_poll_pdu
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_t_poll_retransmit
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_srvcc_op_required
(
    int loop,
    void * p_cell_reconfig_req
);
void set_bitmask_short_drx_cycle
(
    int loop,
    void * p_cell_reconfig_req
);
/* SPR 15764 START */
   /* endif Moved */
/* SPR 15764 END */

void set_bitmask_bucket_size_duration
(
 int loop,
 void * p_cell_reconfig_req
);

void set_bitmask_prioritized_bit_rate
(
 int loop,
 void * p_cell_reconfig_req
);
/*SPR 15527 END*/

oam_return_et create_log_time_stamp
(
        UInt8 * timer_buff
);

/*SPR FIX 7851 */
void
oam_convert_int_to_byte_string_for_cell_trace
(
    UInt8  *dest,
    UInt32  source
);

oam_return_et
oam_convert_u16_arr_to_delimited_string
(
    UInt16* p_num_arr,
    UInt8 arr_size,
    Char8 delim,
    Char8 *p_str,
    UInt32 str_size
);

oam_return_et
oam_convert_bitmap_arr_to_delimited_string
(
    UInt8* p_bitmap_arr,
    UInt8 arr_size,
    Char8 delim,
    Char8 *p_str,
    UInt32 str_size
);

UInt8 oam_tokenize_string_to_bitmap_array
(
    Char8* p_str,
    const Char8* p_delim,
    UInt32 max_tokens,
    UInt8 *p_tokens
);

/* EMBMS Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
oam_return_et oam_bit_string_to_uint_array(
        Char8* p_str,
        const Char8* p_delim,
        UInt32 max_tokens,
        UInt8 sf_alloc_choice_temp,
        UInt8* p_tokens);

oam_return_et oam_uint_array_to_bit_string(
        UInt8* p_bitmap_arr,
        UInt32 max_tokens,
        const Char8* p_delim,
        UInt8* p_str,
        UInt32 str_len);
#endif
/* EMBMS Changes End */

SInt32 oam_isnumber
(
    Char8* str
);

UInt8 oam_extract_cid_from_eutran_cellidentity
(
    UInt8 *p_cellidentity
);

oam_return_et
oam_encode_eutran_cellidentity_into_bit_stream
(
    UInt8 *p_byte_arr,
    UInt32 cell_identity
);

/* SPR 11675 FIX START */
oam_return_et
oam_decode_bit_stream_into_eutran_cellidentity(UInt8 *p_byte_arr,
                                               UInt32 *cell_identity);
/* SPR 11675 FIX END */
/* BUG 1080 FIX START */
oam_return_et oam_admin_state_changed(U16,
                                      U16);

oam_cell_status_t *oam_get_cell_cntxt_status(UInt8 cid);
/* BUG 1080 FIX END */

#ifdef OAM_SON_ENABLE
void
set_bitmask_son_anr_hybrid_pci_range
(
 int loop,
 void * p_cell_reconfig_req
 );

void
set_bitmask_son_anr_closed_pci_range
(
 int loop,
 void * p_cell_reconfig_req
 );

void
set_bitmask_son_anr_open_pci_range
(
 int loop,
 void * p_cell_reconfig_req
 );


void
oam_update_son_mro_config_params(son_intra_rat_global_cell_id_t *p_srv_cgi);


oam_return_et
oam_send_es_enable_req(SInt8 cell_arr_idx,
                       UInt8 EnergySavingControl,
                       UInt16 ActiveUECountIntervalSecond,
                       UInt8 ActiveUECountThreshold,
                       void *p_api_buf,
                       UInt16 msg_len); 

oam_return_et
oam_send_es_disable_req(SInt8 cell_arr_idx,
                        UInt8 EnergySavingControl,
                        void *p_api_buf,
                        UInt16 msg_len);

void
oam_update_son_mlb_config_params(son_intra_rat_global_cell_id_t *p_srv_cgi);

void
oam_update_son_mro_config_params(son_intra_rat_global_cell_id_t *p_srv_cgi);


oam_return_et
oam_send_es_cell_switch_on_req(SInt8 cell_arr_idx,
                               UInt8 CellSwitchStatus,
                               void *p_api_buf,
                               UInt16 msg_len);

oam_return_et
oam_send_es_cell_switch_off_req(SInt8 cell_arr_idx,
				UInt8 CellSwitchStatus,
				UInt8 CellSwitchOffMode,
				void *p_api_buf,
				UInt16 msg_len);
#endif
/*SPR 11315 start*/
void oam_cmn_convert_byte_string_to_short(
        UInt8        *p_bytes,
        UInt32       *p_value,
	UInt32	      bitstring_size);
/*SPR 11315 end*/
/* BITSTRING FIX START */
void oam_cmn_convert_csgid_to_integer(
        UInt8        *p_bytes,
        UInt16       num_of_bytes,
        UInt32       *p_value);
/* BITSTRING FIX END */
/*SPR 11315 FIX START*/
oam_return_et
oam_convert_plmnid(Char8 *out_plmn, void *p_input_plmn);
/*SPR 11315 FIX END*/

/* SPR 13024 FIX START */
UInt32 oam_tokenize_string_to_u16_ext(const Char8* p_str,
                                      const Char8* item_sep,  
                                      const Char8* range_sep,
                                      UInt16 *p_tokens,
                                      UInt32 max_tokens,
                                      oam_error_code_et *p_err);
/* SPR 13024 FIX END */

/* EMBMS Changes Start */
UInt16 oam_bit_string_to_uint8_array(Char8* p_str,
                                    const Char8* p_delim,
                                    UInt16 max_tokens,
                                    UInt8* p_tokens);

UInt32 oam_uint8_array_to_bit_string(UInt8* p_uint8_arr,
                                    UInt16 num_elem,
                                    Char8* p_out_str,
                                    UInt16 max_str_size);
/* EMBMS Changes End */
/*spr 22474 start*/
#define OAM_2D_TEMP_ARR_MEM_FREE(temp_arr,loop_counter, p_error_code)\
{\
       oam_counter_t loop_counter2 = OAM_ZERO;\
       for (loop_counter2 = OAM_ZERO; loop_counter2 < loop_counter; loop_counter2++)\
       {\
                  oam_mem_free(temp_arr[loop_counter2], &p_error_code);\
              }\
       oam_mem_free(temp_arr, &p_error_code);\
}

UInt16
oam_get_debug_mem_stats_prints
(
 L2MemPoolStats* p_mem_stats,
 UInt8*** str_arr
);
UInt16
oam_get_debug_msg_stats_prints
(
 L2MsgPoolStats* p_msg_stats,
 UInt8*** str_arr
);
UInt16
oam_get_l3_debug_mem_stats_prints
(
 rrc_MemPoolStats* p_mem_stats,
 UInt8*** str_arr
);
UInt16
oam_get_oam_debug_mem_stats_prints
(
 oam_MemPoolStats* p_mem_stats,
 UInt8*** str_arr
);

/*spr 22474 end*/

 /*SPR 23209 FIXED START */

oam_return_et
oam_validate_mocn_operator_plmn_id_with_sib1
(
    rrm_oam_cell_plmn_info_t *p_mocn_plmn,
    oam_rrm_cell_config_req_t  *p_rrm_cellconfig
);
/* SPR 23504/SES-341 Fix Start */
oam_return_et
oam_update_plmnid_in_mocn_plmn_list
(
 rrm_oam_cell_plmn_info_t *p_mocn_plmn,
 UInt8                   cell_cnt
 );

oam_return_et
oam_validate_and_update_plmnid_in_rrm_oam_plmn_list
(
    rrm_oam_cell_plmn_info_t *p_rrm_oam_plmn,
    UInt8                   cell_cnt
);

oam_return_et
oam_check_epc_plmn_in_mocn_list
(
    rrm_oam_cell_plmn_info_t *p_epc_plmn,
    UInt8 *p_mocn_list_idx,
    UInt8 cell_cnt
);
/* SPR 23504/SES-341 Fix End */

oam_return_et
oam_range_validation_for_mocn_param
(
    Char8  *param,
    UInt16 lower_bound,
    UInt16 upper_bound
);



 /*SPR 23209 FIXED END */


#endif /* OAM_PROTO_H */
