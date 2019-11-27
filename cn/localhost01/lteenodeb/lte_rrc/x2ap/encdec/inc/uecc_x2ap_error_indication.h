/*******************************************************************************
*
*  FILE NAME   : uecc_x2ap_error_indication.h
*
*  DESCRIPTION : This file contains the UE Associated X2AP Error-Indication
*              : Procedure.
*
*  DATE 30-Aug-2012  NAME Sudhansu Sekhar Nanda  REFERENCE  36.423_960(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/
#include "x2ap_types.h"
#include "x2ap_asn_enc_dec_3gpp.h"
#include "rrc_defines.h"


#define X2AP_MAX_IES 32
#define X2AP_IE_PRESENT 1
#define X2AP_CAUSE_SUCCESS 0
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
} uecc_x2ap_message_map_t;

typedef struct  
{
    U8                  max_count;
    x2ap_bool_t         unsuccessful_outcome_present;
    x2ap_bool_t         successful_outcome_present;
    rrc_bitmask_t       bitmask;
    x2ap_UE_X2AP_ID     old_enb_ue_x2ap_id;
    x2ap_UE_X2AP_ID     new_enb_ue_x2ap_id;
    uecc_x2ap_message_map_t  msg_map[X2AP_MAX_IES];
}uecc_x2ap_message_data_t;

typedef enum
{
    UECC_X2AP_ASN_NOT_UNDERSTOOD = 0,
    UECC_X2AP_ASN_MISSING = 1
}uecc_x2ap_asn_type_of_error_et;

typedef enum 
{
    UECC_X2AP_OPTIONAL,
    UECC_X2AP_CONDITIONAL,
    UECC_X2AP_MANDATORY
} uecc_x2ap_presence_et;

typedef enum
{
    UECC_X2AP_OCCURANCE,
    UECC_X2AP_INVALID_VALUE,
    UECC_X2AP_WRONG_ORDER,
    UECC_X2AP_DATA_MISSING
} uecc_x2ap_map_updation_const_et;

typedef struct
{
    x2ap_bool_et send_err_indication_reject_ie_present;
    x2ap_bool_et send_err_indication_notify_ie_present;
} uecc_x2ap_error_ind_bool_t;

typedef struct
{
    x2ap_CriticalityDiagnostics_IE_List_element iE_list[X2AP_MAX_IES];
    U8                                  ie_cnt;
}uecc_x2ap_error_ind_ie_list_t;

void uecc_x2ap_fill_error_indication_values(
        rrc_x2ap_error_indication_t *p_err_ind,
        x2ap_ProcedureCode          proc_code,
        x2ap_TriggeringMessage      triggering_msg,
        x2ap_Criticality            proc_criticality);

void uecc_x2ap_fill_cause_for_error_indication_message(
        x2ap_TriggeringMessage triggering_msg,
        U16 procedure_code,
        rrc_x2ap_error_indication_t *p_error_indication);

rrc_return_et uecc_x2ap_build_and_send_err_indication
(
 U8                  peer_enodeb_id,      /* Peer EnodeB ID */
 rrc_x2ap_error_indication_t *p_error_indication
);

x2ap_return_et uecc_x2ap_validate_ie_value(
        uecc_x2ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U16 id,
        void *p_value);

x2ap_return_et uecc_x2ap_update_message_map
(
 uecc_x2ap_message_data_t *p_ie_order_map,
 uecc_x2ap_map_updation_const_et update_type,
 U32 order_index,
 U16 id
 );


void uecc_x2ap_add_to_err_ind_ie_list(
        uecc_x2ap_error_ind_ie_list_t *p_ie_list,
        x2ap_Criticality iECriticality,
        x2ap_ProtocolIE_ID iE_ID,
        U16 *p_index,
        uecc_x2ap_error_ind_bool_t *p_send_error_indication,
        x2ap_bool_et ismissing,
        x2ap_bool_et bypass_ignore_check);

x2ap_return_et uecc_x2ap_parse_message_map(
        OSCTXT *p_asn1_ctx,
        U8 peer_enodeb_id,
        uecc_x2ap_message_data_t *p_msg_map,
        uecc_x2ap_error_ind_ie_list_t *p_ie_list,
        U16* p_index_to_update,
        uecc_x2ap_error_ind_bool_t *p_send_error_indication,
        U16 proc_code,
        U8 triggering_msg,
        U8 proc_criticality,
        rrc_x2ap_error_indication_t *p_error_indication);
