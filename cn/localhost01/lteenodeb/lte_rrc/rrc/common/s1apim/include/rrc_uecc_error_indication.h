/****************************************************************************
*
*  ARICENT -
*
*  Copyright (C) 2011 Aricent Inc. All Rights Reserved.
*
****************************************************************************
*
*  $Id: UECC_ERROR_INDICATION.h,v 1.6 2011/02/28 07:04:17 Jitender Kumar Exp $
*
****************************************************************************
*
*  File Description : 
*
****************************************************************************/
#ifndef _UECC_ERROR_INDICATION_H_
#define _UECC_ERROR_INDICATION_H_

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
#include "uecc_ue_ctx.h"
#include "uecc_global_ctx.h"
#include "lppa_asn_enc_dec_3gpp.h"

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
} uecc_s1ap_message_map_t;

typedef struct  
{
    U8 max_count;
    rrc_bool_t unsuccessful_outcome_present;
    rrc_bool_t successful_outcome_present;
    rrc_bitmask_t        bitmask;
    s1ap_MME_UE_S1AP_ID  mme_ue_s1ap_id;
    s1ap_ENB_UE_S1AP_ID  enb_ue_s1ap_id;
    uecc_s1ap_message_map_t msg_map[S1AP_MAX_IES];
} uecc_s1ap_message_data_t;

typedef enum
{
    UECC_ASN_NOT_UNDERSTOOD = 0,
    UECC_ASN_MISSING = 1
} uecc_s1ap_asn_type_of_error_et;

typedef enum 
{
    UECC_OPTIONAL,
    UECC_CONDITIONAL,
    UECC_MANDATORY
} uecc_s1ap_presence_et;

typedef enum
{
    UECC_OCCURANCE,
    UECC_INVALID_VALUE,
    UECC_WRONG_ORDER,
    UECC_DATA_MISSING
} uecc_map_updation_const_et;

typedef struct
{
    rrc_bool_et send_err_indication_reject_ie_present;
    rrc_bool_et send_err_indication_notify_ie_present;
} uecc_s1ap_error_ind_bool_t;

typedef struct
{
    s1ap_CriticalityDiagnostics_IE_Item iE_list[S1AP_MAX_IES];
    U8                                  ie_cnt;
}s1ap_error_ind_ie_list_t;

typedef struct
{
    lppa_CriticalityDiagnostics_IE_List_element iE_list[S1AP_MAX_IES];
    U8                                          ie_cnt;
}lppa_error_ind_ie_list_t;

rrc_return_et uecc_validate_ie_value(
        uecc_gb_context_t          *p_uecc_gb_context,/* UECC global context */
        uecc_s1ap_message_data_t *p_ie_order_map,
        U32 order_index,
        U8 id,
        void *p_value,
        uecc_ue_context_t             *p_ue_context);

rrc_return_et build_and_send_error_indication_unknown_proc(
        uecc_gb_context_t          *p_uecc_gb_context,/* UECC global context */
        U8 mme_id, 
        s1ap_ProcedureCode proc_code,
        s1ap_Criticality proc_criticality,
        s1ap_TriggeringMessage triggering_msg
);
rrc_return_et uecc_s1ap_build_and_send_err_indication
(
 uecc_gb_context_t          *p_uecc_gb_context,/* UECC global context */
 rrc_module_id_t             dest_module_id,
 U8                          mme_id,
 rrc_s1ap_error_indication_t *p_error_indication
);
void log_uecc_error_indication_message
(
     uecc_gb_context_t          *p_uecc_gb_context,/* UECC global context */
     rrc_s1ap_error_indication_t *p_err_ind
);
void uecc_add_to_err_ind_ie_list
(
        uecc_gb_context_t          *p_uecc_gb_context,/* UECC global context */
        s1ap_error_ind_ie_list_t *p_ie_list,
        s1ap_Criticality iECriticality,
        s1ap_ProtocolIE_ID iE_ID,
        U16 *p_index,
        uecc_s1ap_error_ind_bool_t *p_send_error_indication,
        rrc_bool_et ismissing,
        rrc_bool_et bypass_ignore_check
);

rrc_return_et uecc_parse_message_map(
        uecc_gb_context_t          *p_uecc_gb_context,/* UECC global context */
        OSCTXT *p_asn1_ctx,
        U8 mme_id,
        uecc_s1ap_message_data_t *p_msg_map,
        s1ap_error_ind_ie_list_t *p_ie_list,
        U16* p_index_to_update,
        uecc_s1ap_error_ind_bool_t *p_send_error_indication,
        U16 proc_code,
        U8 triggering_msg,
        U8 proc_criticality,
        rrc_s1ap_error_indication_t *p_error_indication
);
rrc_return_et uecc_update_message_map
(
 uecc_gb_context_t          *p_uecc_gb_context,/* UECC global context */
 uecc_s1ap_message_data_t *p_ie_order_map,
 uecc_map_updation_const_et update_type,
 U32 order_index,
 U16 id
);
void log_uecc_criticality_diagostics(
        uecc_gb_context_t          *p_uecc_gb_context,/* UECC global context */
        s1ap_CriticalityDiagnostics *p_criticality_diagostics
);

#endif /* _UECC_ERROR_INDICATION_H_ */
