/*******************************************************************************
*
*  FILE NAME   : x2ap_rsu_sm.h
*
*  DESCRIPTION : This file contains the state machine and the state machine Entry
*                function declaration.
*
*  DATE           NAME                REFERENCE                  REASON
*  02-March-2012   Rajneesh Walia       36.423_960(3GPP Spec)      Initial
*
*  Copyright 2012, Aricent Inc.
*
*******************************************************************************/

#ifndef _X2AP_RSU_SM_
#define _X2AP_RSU_SM_

#include "stacklayer.h"
#include "x2ap_types.h"
#include "x2ap_search_tree.h"
#include "x2ap_timer.h"
#include "x2ap_db.h"
#include "rrc_x2apRrm_intf.h"
#include "x2ap_utils.h"
#include "sm_entry.h"
#include "x2ap_utility.h"
#include "rrc_cp_common.h"
#include "rrc_x2apRrm_il_composer.h"
#include "rrc_x2apRrm_il_parser.h"
#include "rrc_msg_mgmt.h"
#include "rrc_x2ap_uecc_intf.h"
#include "x2ap_error_indication.h"

/* RSU SM States */
typedef enum
{
    X2AP_RSU_STATE_IDLE,           /* Idle state*/
    X2AP_RSU_STATE_SEND_RSU,       /* Sending resource status update */
    X2AP_RSU_STATE_STOP_ONGOING,   /* Stop command ongoing */
    X2AP_RSU_STATE_START_ONGOING,  /* Start command ongoing */
    X2AP_RSU_STATE_LISTEN_RSU, /* Listening for resource status update */
    X2AP_RSU_MAX_STATE
}x2ap_rsu_state_en;

/* RSU SM Events*/
typedef enum
{
    X2AP_RSU_EVENT_X2_CONNECTION_DOWN,             /* X2 connection down */
    X2AP_RSU_EVENT_RESOURCE_STATUS_INFO,           /* Info recvd from RRM */
    X2AP_RSU_EVENT_RESOURCE_STATUS_UPDATE,         /* Update from peer eNB */
    X2AP_RSU_EVENT_RESOURCE_STATUS_FAILURE,        /* Failure from peer eNB */
    X2AP_RSU_EVENT_RESOURCE_STATUS_REQUEST,        /* Request from peer eNB */
    X2AP_RSU_EVENT_RESOURCE_STATUS_RESPONSE,       /* Response from peer eNB */
    X2AP_RSU_EVENT_RESOURCE_STATUS_RRM_STOP_REQ,   /* Stop request   */
    X2AP_RSU_EVENT_RESOURCE_STATUS_RRM_STOP_RES,   /* Stop response  */
    X2AP_RSU_EVENT_RESOURCE_STATUS_RRM_START_REQ,  /* Start request  */
    X2AP_RSU_EVENT_RESOURCE_STATUS_RRM_START_RES,   /* Start response */
    X2AP_RSU_EVENT_PEER_ENB_RESPONSE_TIMER_EXPIRY, /* Peer eNB response Timer */
    X2AP_RSU_EVENT_RRM_RESPONSE_TIMER_EXPIRY,       /* RRM response timer */
    X2AP_RSU_MAX_EVENT 
}x2ap_rsu_event_en;

/* SPR_5966 */
typedef enum
{
    SRC_MEAS_ID = 1,
    TRG_MEAS_ID
}meas_id_type_en;
/* SPR_5966 */

/* RSU context */
typedef struct
{
   x2ap_rsu_state_en  state;                          /* RSU current state */
   U8                 peer_enb_id;                    /* Peer EnodeB ID */
   /* SPR_5966 */
   U32                self_meas_id;
   U8                 src_or_trg_meas_id;             /* meas_id_type_en */
   /* SPR_5966 */
   U32                enb1_meas_id;                   /* Self measurement id */
   U32                enb2_meas_id;                   /* Peer measurement id */
   U32                rrm_transaction_id;
   x2ap_timer_t       timer_id;                       /* Timer id */
   x2ap_timer_id_en   timer_type;                      /* Timer ype */
   U8                 partial_success_indicator_present;
   U8                 rrm_rsu_measurement_res_list_present;
   x2ap_rrm_rsu_measurement_res_t x2ap_rsu_measurement_res_list;
}x2ap_rsu_context_t;

typedef struct 
{
    /*CSPL nodeAnchor*/
    x2ap_search_tree_node_t nodeAnchor;
    x2ap_rsu_context_t     x2ap_rsu_ctx;

}rsu_enb_meas_id_search_node_t;

#define X2AP_RSU_MEASUREMENT_ID_INVALID 0
#define X2AP_RSU_MEASUREMENT_ID_IS_ALLOCATED 1
#define X2AP_RSU_MEASUREMENT_ID_MAX 4095
#define X2AP_RSU_MEASUREMENT_ID_INDEX_INVALID 4096

/**************************************************************************
*           FUNCTIONS PROTOTYPES
**************************************************************************/           
x2ap_return_et
x2ap_rsu_res_status_rrm_start_req_evt_hdl(x2ap_gb_context_t            *p_x2ap_gb_ctx,
                                          U32                          transaction_Id,
                                          x2ap_rrm_rsu_rrm_start_req_t *p_x2ap_rsu_start_req,
                                          U8                            peer_enodeb_id);

x2ap_return_et 
x2ap_rsu_build_and_send_start_req(x2ap_gb_context_t             *p_x2ap_gb_ctx,
                                  x2ap_peer_enb_context_t       *p_enb_ctx,
                                  x2ap_rrm_rsu_rrm_start_req_t  *p_x2ap_rsu_start_req,
                                  rsu_enb_meas_id_search_node_t *p_insert_node);

x2ap_return_et 
x2ap_rsu_build_start_req(rrc_x2ap_rsu_start_req_t      *p_x2ap_rsu_start_req, 
                               x2ap_gb_context_t             *p_x2ap_gb_ctx, 
                               x2ap_peer_enb_context_t       *p_enb_ctx,
                               x2ap_rrm_rsu_rrm_start_req_t  *p_rrm_rsu_start_req,
                               rsu_enb_meas_id_search_node_t *p_insert_node);

x2ap_return_et 
x2ap_rsu_encode_start_req(rrc_x2ap_rsu_start_req_t  *p_rsu_start_req,  /* for memory allocation */
                               U8                        *p_buff,           /* ASN.1 encoded message */
                               U16                       *p_buff_size,      /* size of result buffer */
                               /* SPR 16118 Fix Start */
                               OSCTXT      *p_asn1_ctx
                               /* SPR 16118 Fix End */);

x2ap_return_et
x2ap_rsu_build_report_characterstics_bitmask(rrc_x2ap_rsu_start_req_t *p_rsu_start_req,
					 x2ap_ReportCharacteristics* p_report_characteristics);

x2ap_return_et
x2ap_rsu_build_cell_report_list(OSCTXT                                  *pctxt,
                            x2ap_CellToReport_List* plist,
                            rrc_x2ap_rsu_start_req_t                *p_rsu_start_req);

x2ap_return_et 
x2ap_rsu_process_start_msg(OSCTXT      *p_asn1_ctx,
                           x2ap_ResourceStatusRequest       *p_3gpp,    /* unpacked src */
                           rrc_x2ap_rsu_start_req_t    *p_trgt,    /* dest */
                           x2ap_gb_context_t  *p_x2ap_gb_ctx,      /* X2ap global context */
                           x2ap_peer_enb_context_t *p_enb_ctx);    /* peer EnodeB context */

x2ap_return_et 
x2ap_rsu_process_failure_msg(OSCTXT      *p_asn1_ctx,
                         x2ap_ResourceStatusFailure    *p_3gpp,    /* unpacked src */
                         rrc_x2ap_rsu_failure_t       *p_trgt,    /* dest */
                         x2ap_gb_context_t            *p_x2ap_gb_ctx,      /* X2ap global context*/
                         x2ap_peer_enb_context_t      *p_enb_ctx);

x2ap_return_et 
x2ap_rsu_process_response_msg(OSCTXT      *p_asn1_ctx,
                              x2ap_ResourceStatusResponse    *p_3gpp,    /* unpacked src */
                              rrc_x2ap_rsu_response_t       *p_trgt,    /* dest */
                              x2ap_gb_context_t            *p_x2ap_gb_ctx,   /* X2ap global context*/
                              x2ap_peer_enb_context_t      *p_enb_ctx);

x2ap_return_et 
x2ap_rsu_build_and_send_failure_msg(U32                         enb1_meas_id,
                                    U32                         enb2_meas_id,
                                    x2ap_gb_context_t           *p_x2ap_gb_ctx,
                                    x2ap_peer_enb_context_t     *p_enb_ctx,
                                    x2ap_CriticalityDiagnostics *p_criticality_diagnostics,
                                    U8                          cause,
                                    x2ap_rsu_context_t *p_x2ap_rsu_ctx);

x2ap_return_et 
x2ap_rsu_decode_start_msg(
                          x2ap_gb_context_t  *p_x2ap_gl_ctx,
                          x2ap_peer_enb_context_t *p_enb_ctx,
                          OSCTXT                     *p_asn1_ctx,
                          x2ap_ResourceStatusRequest *p_3gpp,    /* unpacked src */
                          rrc_x2ap_rsu_start_req_t   *p_intrl,    /* dest */
                          U8                         *rsu_periodicity_fg,
                          rrc_x2ap_error_indication_t *p_error_indication);

x2ap_return_et 
x2ap_rsu_build_and_send_rrm_start_req(rrc_x2ap_rsu_start_req_t *p_x2ap_start_req,    /* dest */
                                      x2ap_gb_context_t        *p_x2ap_gb_ctx,
                                      x2ap_peer_enb_context_t  *p_enb_ctx);      /* X2ap global context*/



x2ap_return_et 
x2ap_rsu_build_and_send_rrm_stop_req(x2ap_gb_context_t        *p_x2ap_gb_ctx,      /* X2ap global context*/
                                     U32                      meas_ID);

x2ap_return_et 
x2ap_rsu_internal_encode_failure_msg(OSCTXT                  *pctxt,         /* for memory allocation */
                                     U8                      *p_buff,        /* ASN.1 encoded message */
                                     U16                     *p_buff_size,   /* size of result buffer */
                                     rrc_x2ap_rsu_failure_t  *p_resource_status_failure);

x2ap_return_et
x2ap_rsu_decode_cell_to_report(rrc_x2ap_rsu_start_req_t               *p_x2ap_rsu_start_req , 
                               x2ap_ResourceStatusRequest_protocolIEs_element *p_protocol_IE,
                               x2ap_bool_et *p_send_tse);

void x2ap_generate_plmn_identity_to_int(plmn_identity_t    *p_plmn_identity,
                                        const OSOCTET      *p_s1ap_plmn_identity);

x2ap_return_et 
x2ap_rsu_rrm_timer_expiry_evt_hdl(x2ap_timer_buffer_t *p_timer_buf,
                                  x2ap_gb_context_t *p_x2ap_gb_ctx);

x2ap_return_et 
x2ap_rsu_build_and_send_rrm_stop_resp(U32                                transaction_id,
                                      x2ap_return_et                     response,
                                      U32                                meas_id,
                                      x2_gb_enb_id_t                     gb_enb_id,
                                      U8                                 casue);

void 
x2ap_rsu_process_msg(U8                  *p_msg_ptr, 
                     x2ap_gb_context_t   *p_x2ap_gb_ctx, 
                     x2ap_peer_enb_context_t *p_enb_ctx);

x2ap_return_et 
x2ap_rsu_enb_timer_expiry_evt_hdl(x2ap_timer_buffer_t *p_timer_buf,
                                              x2ap_gb_context_t *p_x2ap_gb_ctx);


x2ap_return_et 
x2ap_rsu_build_and_send_response_msg(x2ap_gb_context_t           *p_x2ap_gb_ctx,
                                     x2ap_rsu_context_t          *p_x2ap_rsu_ctx,
                                     x2ap_peer_enb_context_t     *p_enb_ctx,
                                     x2ap_CriticalityDiagnostics *p_criticality_diagnostics);

x2ap_return_et 
x2ap_rsu_internal_encode_response_msg(OSCTXT                   *pctxt,         /* for memory allocation */
                                      U8                       *p_buff,        /* ASN.1 encoded message */
                                      U16                      *p_buff_size,   /* size of result buffer */
                                      rrc_x2ap_rsu_response_t  *p_resource_status_response);/* source */

x2ap_return_et 
x2ap_rsu_decode_faliure_msg(
                            x2ap_gb_context_t  *p_x2ap_gl_ctx,
                            x2ap_peer_enb_context_t *p_enb_ctx,
                            OSCTXT                     *p_asn1_ctx,
                            x2ap_ResourceStatusFailure *p_3gpp,    /* unpacked src */
                            rrc_x2ap_rsu_failure_t     *p_msg);    /* dest */

x2ap_return_et 
x2ap_rsu_decode_response_msg(
                             x2ap_gb_context_t  *p_x2ap_gl_ctx,
                             x2ap_peer_enb_context_t *p_enb_ctx,
                             OSCTXT      *p_asn1_ctx,
                             x2ap_ResourceStatusResponse *p_3gpp,    /* unpacked src */
                             rrc_x2ap_rsu_response_t    *p_msg);   /* dest */


x2ap_return_et 
x2ap_rsu_build_and_send_update_msg(x2ap_gb_context_t          *p_x2ap_gb_ctx,
                                   x2ap_rsu_context_t         *p_x2ap_rsu_ctx,
                                   x2ap_rrm_rsu_rrm_update_t  *p_x2ap_rrm_rsu_rrm_update,
                                   x2ap_peer_enb_context_t    *p_enb_ctx);

x2ap_return_et 
x2ap_rsu_build_update_list(rrc_x2ap_rsu_update_t     *p_x2ap_res_status_update, 
                           x2ap_rrm_rsu_rrm_update_t *p_x2ap_rrm_rsu_update, 
                           OSCTXT                    *p_asn1_ctx);

x2ap_return_et 
x2ap_rsu_internal_encode_update_msg(OSCTXT                 *pctxt,         /* for memory allocation */
                                    U8                     *p_buff,        /* ASN.1 encoded message */
                                    U16                    *p_buff_size,   /* size of result buffer */
                                    rrc_x2ap_rsu_update_t  *p_resource_status_update); /* source */

void 
x2ap_rsu_process_update_msg(U8                      *p_msg_ptr, 
                            x2ap_gb_context_t   *p_x2ap_gb_ctx, 
                            x2ap_peer_enb_context_t *p_enb_ctx);


x2ap_return_et 
x2ap_rsu_decode_cacg_list(x2ap_rrm_rsu_cacg_t                       *p_x2ap_rsu_cacg,
                      x2ap_CellMeasurementResult_Item_iE_Extensions *p_cacg_list);

/* eICIC changes start */
x2ap_return_et 
x2ap_rsu_decode_abs_status_list(x2ap_rrm_abs_status_t     *p_x2ap_rsu_aspr,
                            x2ap_CellMeasurementResult_Item_iE_Extensions *p_aspr_list);
/* eICIC changes stop */

x2ap_return_et 
x2ap_rsu_decode_update_msg(
                           x2ap_gb_context_t  *p_x2ap_gl_ctx,
                           x2ap_peer_enb_context_t *p_enb_ctx,
                           OSCTXT                        *p_asn1_ctx,
                           x2ap_ResourceStatusUpdate     *p_3gpp,    /* unpacked src */
                           rrc_x2ap_rsu_update_t         *p_msg,    /* dest */
                           x2ap_rrm_rsu_enb_update_ind_t *p_x2ap_rrm_rsu_rrm_update
                           );


x2ap_return_et 
x2ap_rsu_decode_measurement_result_list(
        x2ap_CellMeasurementResult_List *p_cell_meas_result_list,
        x2ap_rrm_rsu_enb_update_ind_t *p_rsu_update_list,
        x2ap_message_data_t *p_ie_order_map,
        U8 order_index,
        U8 id,
        x2ap_bool_et *p_send_tse);

x2ap_return_et x2ap_rsu_process_update(OSCTXT                        *p_asn1_ctx,
                                       x2ap_ResourceStatusUpdate     *p_3gpp,    /* unpacked src */
                                       rrc_x2ap_rsu_update_t         *p_trgt,    /* dest */
                                       x2ap_gb_context_t             *p_x2ap_gb_ctx,      /* X2ap global context*/
                                       x2ap_peer_enb_context_t          *p_enb_ctx);

x2ap_return_et 
x2ap_rsu_encode_stop_req(OSCTXT                    *p_asn1_ctx,
                         rrc_x2ap_rsu_start_req_t  *p_rsu_start_req,         /* for memory allocation */
                         U8                        *p_buff,        /* ASN.1 encoded message */
                         U16                       *p_buff_size);  /* size of result buffer */

x2ap_return_et 
x2ap_rsu_build_and_send_stop_msg(x2ap_gb_context_t           *p_x2ap_gb_ctx,
                                 x2ap_rsu_context_t          *p_x2ap_rsu_ctx,
                                 x2ap_peer_enb_context_t      *p_enb_ctx);


x2ap_return_et 
x2ap_rsu_send_rrm_start_resp(U32                                transaction_id,
                             x2ap_return_et                     response,
                             U32                                meas_id,
                             U8                                 cause,
                             x2ap_peer_enb_context_t            *p_peer_enb_ctx,
                             x2ap_rrm_rsu_measurement_res_t     *x2ap_rsu_measurement_res_list,
                             U8                                 partial_success_present);

x2ap_return_et 
x2ap_rsu_start_timer(x2ap_timer_id_en             timer_type,
                     x2ap_peer_enb_context_t      *p_enb_ctx,
                     U32                          timer_value,
                     x2ap_timer_t                 *p_timer_id,
                     U32                           meas_id);

x2ap_return_et
x2ap_rsu_event_x2_conn_down_evt_hdl(x2ap_gb_context_t  *p_x2ap_gb_ctx,
                                    x2ap_peer_enb_context_t *p_enb_ctx);

x2ap_return_et 
x2ap_process_rrm_rsu_start_res( void             *p_api,
                                x2ap_gb_context_t   *p_x2ap_gb_ctx);

x2ap_return_et 
x2ap_process_rrm_rsu_stop_res( void             *p_api,
                               x2ap_gb_context_t   *p_x2ap_gb_ctx);
x2ap_return_et 
x2ap_process_rrm_rsu_update( void             *p_api,
                             x2ap_gb_context_t   *p_x2ap_gb_ctx);

void 
x2ap_process_rrm_rsu_start_req(void             *p_api,
                               x2ap_gb_context_t   *p_x2ap_gb_ctx);

x2ap_return_et 
x2ap_process_rrm_rsu_stop_req(void             *p_api,
                              x2ap_gb_context_t   *p_x2ap_gb_ctx);
x2ap_return_et
x2ap_rsu_build_measurement_initiation_result_list(rrc_x2ap_rsu_response_t* p_x2ap_rsu_response,
        x2ap_rrm_rsu_measurement_res_t *p_x2ap_rrm_rsu_measurement,
        OSCTXT *p_asn1_ctx);

x2ap_return_et
x2ap_rsu_complete_failure_cause_measurement_list(rrc_x2ap_rsu_failure_t* p_x2ap_rsu_failure,
        x2ap_rrm_rsu_measurement_res_t *p_x2ap_rrm_rsu_measurement,
        OSCTXT *p_asn1_ctx);

x2ap_return_et 
x2ap_rsu_decode_measurement_intiation_list(x2ap_MeasurementInitiationResult_List *p_measurement_initiation_result_list,
        x2ap_rrm_rsu_measurement_res_t *p_x2ap_rsu_measurement_res_list);

x2ap_return_et 
x2ap_rsu_decode_complete_failure_list(x2ap_CompleteFailureCauseInformation_List *p_complete_failure_cause_inf_list,
        x2ap_rrm_rsu_measurement_res_t *p_x2ap_rsu_measurement_res_list);

#endif/*_X2AP_RSU_SM_*/

