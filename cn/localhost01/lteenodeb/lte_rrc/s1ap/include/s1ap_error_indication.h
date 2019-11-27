/****************************************************************************
*
*  ARICENT -
*
*  Copyright (C) 2011 Aricent Inc. All Rights Reserved.
*
****************************************************************************
*
*  $Id: s1ap_error_indication.h,v 1.6 2011/02/28 07:04:17 Jitender Kumar Exp $
*
****************************************************************************
*
*  File Description : 
*
****************************************************************************/
#ifndef _S1AP_ERROR_INDICATION_H_
#define _S1AP_ERROR_INDICATION_H_

/****************************************************************************
* Project Includes
****************************************************************************/

/****************************************************************************
* Exported Includes
****************************************************************************/
#include "rrc_defines.h"
#include "s1ap_global_ctx.h"
#include "rrc_s1ap_asn_intrl_3gpp.h"
#include "rrc_common_utils.h"
#include "rrc_s1ap_asn_intrl_dec_3gpp.h"

/****************************************************************************
* Exported Definitions
****************************************************************************/
/********************************************************************
*                        EXTERNAL APIs
*******************************************************************/
/********************************************************************
* S1AP - OAM APIs
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
#define S1AP_MAX_IES 32
#define S1AP_IE_PRESENT 1
#define MAX_MME_UE_S1AP_ID 0xFFFFFFFF
#define MAX_ENB_UE_S1AP_ID 0xFFFFFF
#define ENUM_NOT_VALID 0x7FFFFFFF
typedef struct
{
    U8 order_num;
    U16 ie_id;
    s1ap_Presence_Root presence;
    s1ap_Criticality_Root criticality;
    U8 occurances;
    U8 invalid_value_present;
    U8 wrong_order;
    U8 data_missing;
} s1ap_message_map_t;

typedef struct
{
    U8 order_num;
    U16 ie_id;
    lppa_Presence_Root presence;
    lppa_Criticality_Root criticality;
    U8 occurances;
    U8 invalid_value_present;
    U8 wrong_order;
    U8 data_missing;
} lppa_message_map_t;

typedef struct  
{
    U8 max_count;
    rrc_bool_t unsuccessful_outcome_present;
    rrc_bool_t successful_outcome_present;
    rrc_bitmask_t        bitmask;
    s1ap_MME_UE_S1AP_ID  mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID  enb_ue_s1ap_id;
    s1ap_message_map_t msg_map[S1AP_MAX_IES];
}s1ap_message_data_t;

typedef struct  
{
    U8 max_count;
    rrc_bool_t unsuccessful_outcome_present;
    rrc_bool_t successful_outcome_present;
    rrc_bitmask_t        bitmask;
    s1ap_MME_UE_S1AP_ID  mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID  enb_ue_s1ap_id;
    U8                   routing_id;
    lppa_message_map_t msg_map[S1AP_MAX_IES];
}lppa_message_data_t;

typedef enum
{
   ASN_NOT_UNDERSTOOD = 0,
   ASN_MISSING = 1
} s1ap_asn_type_of_error_et;

typedef enum 
{
    OPTIONAL,
    CONDITIONAL,
    MANDATORY
} s1ap_presence_et;

typedef enum
{
    OCCURANCE,
    INVALID_VALUE,
    WRONG_ORDER,
    DATA_MISSING
} map_updation_const_et;

typedef struct
{
    rrc_bool_et send_err_indication_reject_ie_present;
    rrc_bool_et send_err_indication_notify_ie_present;
} s1ap_error_ind_bool_t;

rrc_return_et s1ap_build_and_send_err_indication
(
  mme_context_t           *p_mme_context,
  rrc_s1ap_error_indication_t *p_error_indication,
  U16 *p_stream_id
);


rrc_return_et update_message_map
(
 s1ap_message_data_t *p_ie_order_map,
 map_updation_const_et update_type,
 U32 order_index,
 U16 id
);

rrc_return_et lppa_update_message_map
(
 lppa_message_data_t *p_ie_order_map,
 map_updation_const_et update_type,
 U32 order_index,
 U16 id
);

rrc_return_et validate_ie_value(
        s1ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U16 id,
        void *p_value);
void add_to_err_ind_ie_list(
        s1ap_error_ind_ie_list_t *p_ie_list,
        s1ap_Criticality iECriticality,
        s1ap_ProtocolIE_ID iE_ID,
        U16 *p_index,
        s1ap_error_ind_bool_t *p_send_error_indication,
        rrc_bool_et ismissing,
        rrc_bool_et bypass_ignore_check);
rrc_return_et parse_message_map(
        OSCTXT * p_asn1_ctx,
        mme_context_t *p_mme_context,
        s1ap_message_data_t *p_msg_map,
        s1ap_error_ind_ie_list_t *p_ie_list,
        U16* p_index_to_update,
        s1ap_error_ind_bool_t *p_send_error_indication,
        U16 proc_code,
        U8 triggering_msg,
        U8 proc_criticality,
        rrc_s1ap_error_indication_t *p_error_indication);

rrc_return_et lppa_validate_ie_value(
        lppa_message_data_t *p_ie_order_map,
        U32 order_index,
        U16 id,
        void *p_value);

void lppa_add_to_err_ind_ie_list(
        lppa_error_ind_ie_list_t  *p_ie_list,
        lppa_Criticality  iECriticality,
        lppa_ProtocolIE_ID  iE_ID,
        U16  *p_index,
        s1ap_error_ind_bool_t  *p_send_error_indication,
        rrc_bool_et  ismissing,
        rrc_bool_et  bypass_ignore_check);

rrc_return_et lppa_parse_message_map(
        OSCTXT  *p_asn1_ctx,
        mme_context_t *p_mme_context,
        lppa_message_data_t *p_msg_map,
        lppa_error_ind_ie_list_t *p_ie_list,
        U16* p_index_to_update,
        s1ap_error_ind_bool_t *p_send_error_indication,
        U16 proc_code,
        U8 triggering_msg,
        U8 proc_criticality,
        rrc_lppa_error_indication_t *p_error_indication);

void  lppa_fill_error_indication_values(
        rrc_lppa_error_indication_t *p_err_ind,
        lppa_ProcedureCode proc_code,
        lppa_TriggeringMessage triggering_msg,
        lppa_Criticality proc_criticality,
        lppa_LPPATransactionID lppa_trans_id);

rrc_return_et lppa_build_and_send_err_indication(
  mme_context_t           *p_mme_context,
  rrc_lppa_error_indication_t  *p_error_indication,
  U16                     *p_stream_id);

rrc_return_et validate_lppa_cause(lppa_Cause *p_value);

void log_error_indication_message
(
        rrc_s1ap_error_indication_t *p_err_ind
);
void log_criticality_diagostics(
       s1ap_CriticalityDiagnostics *p_criticality_diagostics);

#endif  /* _S1AP_ERROR_INDICATION_H_ */
