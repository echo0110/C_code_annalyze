/****************************************************************************
*
*  ARICENT -
*
*  Copyright (C) 2011 Aricent Inc. All Rights Reserved.
*
****************************************************************************
*
*  $Id: x2ap_error_indication.h,v 1.6 2012/08/02 07:04:17 Hari Oum Sharan Exp $
*
****************************************************************************
*
*  File Description : 
*
****************************************************************************/
#ifndef _X2AP_ERROR_INDICATION_H_
#define _X2AP_ERROR_INDICATION_H_

/****************************************************************************
* Project Includes
****************************************************************************/

/****************************************************************************
* Exported Includes
****************************************************************************/
#include "rrc_defines.h"
#include "x2ap_db.h"
//#include "rrc_s1ap_asn_intrl_3gpp.h"
#include "rrc_common_utils.h"
#include "x2ap_asn_enc_dec_3gpp.h"
#include "rrc_x2apEnb_intf.h"

/****************************************************************************
* Exported Definitions
****************************************************************************/
/********************************************************************
*                        EXTERNAL APIs
*******************************************************************/

/****************************************************************************
* Exported Types
****************************************************************************/

/****************************************************************************
* Exported Constants
****************************************************************************/

/****************************************************************************
* Exported Variables
****************************************************************************/

/****************************************************************************
* Exported Functions
****************************************************************************/
#define X2AP_MAX_IES 32
#define X2AP_IE_PRESENT 1
#define MAX_OLD_ENB_UE_X2AP_ID 0xFFF
#define MAX_NEW_ENB_UE_X2AP_ID 0xFFF
#define ENUM_NOT_VALID 0x7FFFFFFF
typedef struct
{
    U8 order_num;
    U16 ie_id;
    x2ap_Presence_Root presence;
    x2ap_Criticality_Root criticality;
    U8 occurances;
    U8 invalid_value_present;
    U8 wrong_order;
    U8 data_missing;
} x2ap_message_map_t;

typedef struct  
{
    U8                  max_count;
    x2ap_bool_t          unsuccessful_outcome_present;
    x2ap_bool_t          successful_outcome_present;
    rrc_bitmask_t       bitmask;
    x2ap_UE_X2AP_ID     old_enb_ue_x2ap_id;
    x2ap_UE_X2AP_ID     new_enb_ue_x2ap_id;
    x2ap_message_map_t  msg_map[X2AP_MAX_IES];
}x2ap_message_data_t;

typedef struct
{
    x2ap_CriticalityDiagnostics_IE_List_element iE_list[X2AP_MAX_IES];
    U8                                  ie_cnt;
}x2ap_error_ind_ie_list_t;

typedef struct
{
    x2ap_bool_et send_err_indication_reject_ie_present;
    x2ap_bool_et send_err_indication_notify_ie_present;
} x2ap_error_ind_bool_t;

typedef enum
{
    ASN_NOT_UNDERSTOOD = 0,
    ASN_MISSING = 1
}x2ap_asn_type_of_error_et;

typedef enum 
{
    OPTIONAL,
    CONDITIONAL,
    MANDATORY
} x2ap_presence_et;

typedef enum
{
    OCCURANCE,
    INVALID_VALUE,
    WRONG_ORDER,
    DATA_MISSING
} map_updation_const_et;

x2ap_return_et x2ap_update_message_map
(
 x2ap_message_data_t *p_ie_order_map,
 map_updation_const_et update_type,
 U32 order_index,
 U16 id
);

x2ap_return_et x2ap_validate_ie_value(
        x2ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U16 id,
        void *p_value);

void x2ap_add_to_err_ind_ie_list(
        x2ap_error_ind_ie_list_t *p_ie_list,
        x2ap_Criticality iECriticality,
        x2ap_ProtocolIE_ID iE_ID,
        U16 *p_index,
        x2ap_error_ind_bool_t *p_send_error_indication,
        x2ap_bool_et ismissing,
        x2ap_bool_et bypass_ignore_check);

x2ap_return_et x2ap_parse_message_map(
        OSCTXT *p_asn1_ctx,
        x2ap_gb_context_t  *p_x2ap_gl_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx,
        x2ap_message_data_t *p_msg_map,
        x2ap_error_ind_ie_list_t *p_ie_list,
        U16* p_index_to_update,
        x2ap_error_ind_bool_t *p_send_error_indication,
        U16 proc_code,
        U8 triggering_msg,
        U8 proc_criticality,
        rrc_x2ap_error_indication_t *p_error_indication
        );

x2ap_return_et x2ap_process_error_indication_msg(
        U8 *p_msg_ptr,
        x2ap_gb_context_t  *p_x2ap_gl_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx
        );

void x2ap_log_error_indication_message(
        rrc_x2ap_error_indication_t *p_err_ind
        );

void x2ap_log_criticality_diagostics(
       x2ap_CriticalityDiagnostics *p_criticality_diagostics);

x2ap_return_et x2ap_send_msg_to_peer_enb(
        x2ap_gb_context_t *p_x2ap_gb_ctx, 
        x2ap_peer_enb_context_t* p_peer_enb_ctx,
        U8* p_message,
        U32 message_buff_len);
void x2ap_fill_error_indication_values(rrc_x2ap_error_indication_t *p_err_ind,
        x2ap_ProcedureCode proc_code,
        x2ap_TriggeringMessage triggering_msg,
        x2ap_Criticality proc_criticality);
void  x2ap_build_criticality_diagnostics_list(
        OSCTXT                              *p_asn1_ctx,
        x2ap_CriticalityDiagnostics_IE_List *p_list,
        x2ap_error_ind_ie_list_t              *p_iE_list
        );

x2ap_return_et x2ap_build_and_send_err_indication(
        x2ap_gb_context_t *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t* p_peer_enb_ctx,
        rrc_x2ap_error_indication_t *p_error_indication
        );

x2ap_return_et x2ap_parse_and_decode_error_indication( 
        rrc_x2ap_error_indication_t *p_parsed_error_indication,
        void                      *p_asn_msg,
        U16                        asn_msg_len);
#endif  /* _X2AP_ERROR_INDICATION_H_ */
