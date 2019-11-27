/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: s1ap_global_ctx.h,v 1.7 2010/10/13 07:04:06 gur18569 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains all the necessary declarations for
 *                     s1ap module global context.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: s1ap_global_ctx.h,v $
 * Revision 1.7  2010/10/13 07:04:06  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.6.4.1  2010/06/24 12:55:31  gur22047
 * internal source and s1ap code
 *
 * Revision 1.6  2010/04/06 04:02:53  gur18569
 * added is_s1ap_ctx_deleted to reset_context_t
 *
 * Revision 1.5  2010/03/25 10:06:04  gur18569
 *  added macro MAX_ENB_UE_ID_PAIR_LIST
 *
 * Revision 1.4  2010/03/24 09:50:45  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.3  2010/02/18 04:25:20  gur10248
 * LTE_RRC_Rel1.1_18feb
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.2.5  2009/12/28 05:09:42  gur18569
 * fixed indentation
 *
 * Revision 1.1.2.4  2009/12/27 08:32:51  gur18569
 * removed tabs
 *
 * Revision 1.1.2.3  2009/12/22 14:38:53  gur18569
 * fixed 154
 *
 * Revision 1.1.2.2  2009/12/17 15:08:26  gur18569
 * added num_of_mme_prov
 *
 * Revision 1.1.2.1  2009/11/25 13:11:02  gur18569
 * Shifted to level of rrc dir
 *
 * Revision 1.1.2.10  2009/11/23 15:03:56  gur18569
 * changed sctp_sd from u16 to sctp_sd_t
 *
 * Revision 1.1.2.9  2009/11/23 10:42:02  gur21006
 * Type od sctp_sd_t changed to S32
 *
 * Revision 1.1.2.8  2009/11/20 08:40:43  gur18569
 * changes during UT
 *
 * Revision 1.1.2.7  2009/11/18 11:21:01  gur20470
 * Removed macro MME_MODULE_ID
 *
 * Revision 1.1.2.6  2009/11/17 10:23:46  gur20052
 *  Review Comments are incorporated
 *
 * Revision 1.1.2.5  2009/11/11 04:38:12  gur18569
 * added max_retry_for_reset to global ctx
 *
 * Revision 1.1.2.4  2009/11/10 11:41:24  gur18569
 * review comments incorporated
 *
 * Revision 1.1.2.3  2009/11/09 09:02:10  gur18569
 * removed compilation warnings
 *
 * Revision 1.1.2.2  2009/10/23 16:11:39  gur18569
 * Initial version
 *
 * Revision 1.1.2.1  2009/10/15 08:40:00  gur18569
 * Initial revision-Gur18569
 *
 *
 *
 ****************************************************************************/

#ifndef _S1AP_GLB_CTX_H_
#define _S1AP_GLB_CTX_H_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "rrc_defines.h"
#include "rrc_s1apOam_intf.h"
#include "search_tree_wrap.h"
#include "s1ap_handlers.h"
#include "s1ap_asn_enc_dec_3gpp.h"
#include "list_wrap.h"

#include "rrc_oam_intf.h"
#include "rrc_rrm_intf.h"
#include "rrc_s1apRrm_intf.h"
#include "rrc_s1ap_csc_intf.h"
#include "rrc_s1ap_uecc_intf.h" 
/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define MAX_ENB_UE_ID_PAIR_LIST 3
//#define MAX_NUM_UE              600
#define MAX_MME_NAME_STR        150
#define MAX_NUM_SCTP_STREAM      10
/*SPR 21219 25Jan FIX START*/
#define INVALID_NUM_SCTP_STREAM      255
/*SPR 21219 25Jan FIX STOP*/

#define BITMASK_ENB_S1AP_ID_PRESENT    1
#define BITMASK_MME_INDEX_PRESENT      2
#define BITMASK_STREAM_ID_PRESENT      3

/* These are used for compare function of a search tree*/
#define FIRST_IS_LESS_THAN_SECOND         -1
#define FIRST_IS_GREATER_THAN_SECOND       1
#define FIRST_IS_EQUAL_TO_SECOND           0
#define ERROR_IN_COMPARE                   0

#define DEFAULT_MAX_RESET_RETRY_COUNT 5

#define MAX_ELEM_TO_PARTIAL_DECODE           2
#define S1AP_MAX_ASN1_BUF_LEN_6K            6144
#define MAX_UE_S1AP_ID_PAIR                256
#define MAX_CELL_NUM                         8


#define S1AP_CSC_READY_FLAG                0x01
#define MAX_NUM_MMECS                      256

/* SPR 15641 Fix Start */
#define S1AP_KILL_Q_ENQUEUE(p_s1ap_kill_queue, \
        p_s1ap_kill_queue_node)\
       ylPushTail( p_s1ap_kill_queue, \
               &(p_s1ap_kill_queue_node->node))

#define S1AP_KILL_Q_DEQUEUE(p_s1ap_kill_queue, snode) \
       snode = ylPopHead(p_s1ap_kill_queue);\

#define S1AP_KILL_Q_GET_COUNT(p_s1ap_kill_queue)\
       ylCount(p_s1ap_kill_queue)

/* SPR 15641 Fix Stop */

/* SPR 22789 Fix Start */
#define RRC_REL_S1AP_UE_CTXT(p_s1ap_gb_ctx,p_s1ap_ctx, enb_ue_s1ap_id) do{\
        set_ue_ctx_from_ue_id(p_s1ap_gb_ctx,PNULL,enb_ue_s1ap_id);\
        rrc_mem_free(p_s1ap_ctx);\
        p_s1ap_ctx = PNULL;\
    }while(RRC_ZERO)
/* SPR 22789 Fix End */

/****************************************************************************
 * Exported Types
 ****************************************************************************/

typedef enum
{
    S1AP_INIT,                   /* Initial state */
    S1AP_W_FOR_PROV_REQ,         /* Waiting for comm/provision request */
    S1AP_ACTIVE,                 /* Active state */
    S1AP_NUM_OF_STATES,          /* Number of states */
    S1AP_INVALID_STATE           /* Invalid State */
}s1ap_state_et;

typedef enum
{
    REJ_ALL_RRC_CONN_MO_DT,
    REJ_ALL_RRC_CONN_MO_DT_MO_SIG,
    PERMIT_EMERGENCY_MO_ACCESS,
    PERMIT_HIGH_PRIORITY_MT_ACCESS,
    REJ_ALL_RRC_DELAY_TOLERANCE,
    OVERLOAD_START_INVALID_VALUE
}mme_overload_response_et;

#define S1AP_SET_FSM_STATE(gb_ctx,new_state) (gb_ctx->s1ap_current_state=new_state)

typedef struct {
#define MME_UE_S1AP_ID_PRESENT                 0x01
#define ENB_UE_S1AP_ID_PRESENT                 0x02

    U8 bitmask;

    U32 mmeUeS1apId;

    U32 enbUeS1apId;
}ue_s1ap_id_pair_t;

typedef struct {
    /* Number of elements in id_pair array */
    U16 num_of_items;

    /* Array to hold enbUeS1apId and mmeUeS1apId values */
    ue_s1ap_id_pair_t id_pair[MAX_UE_S1AP_ID_PAIR];
}s1ap_reset_info_t;

typedef struct {
    /* Cause of reset */
    s1ap_Cause cause;

    s1ap_ResetType *p_reset_type;
    /* Information present request req message */
    s1ap_reset_info_t reset_info;
}reset_req_t;

typedef struct {
    /* Information present in overload start message */
    s1ap_OverloadAction overload_action;
    U16                         bitmask;
#define S1AP_S1_OVERLOAD_START_GUMMEILIST_PRESENT 0x01
#define S1AP_S1_OVERLOAD_START_TRAFFIC_LOAD_RED_IND_PRESENT 0x02
    s1ap_GUMMEIList     s1ap_gummelist;
    s1ap_TrafficLoadReductionIndication s1ap_traffic_load_red_ind;
}parsed_overload_start_t;

typedef struct {
    U16                         bitmask;
    /* Information present in overload start message */
#define S1AP_S1_OVERLOAD_STOP_GUMMEILIST_PRESENT 0x01
    s1ap_GUMMEIList     s1ap_gummelist;
}parsed_overload_stop_t;

/* This structure is filled upon parsing
 * S1 SETUP RESPONSE message 
 * */
typedef struct mme_group_id_list_node
{
    list_node_t node;
    s1ap_MME_Group_ID mme_group_id;
}mme_group_id_list_node_t;

typedef struct{
    U8                          num_served_gummeis;
    served_gummei_info_t        *p_served_gummeis;
}parsed_gummei_info;

typedef struct {
    U16                         bitmask;
#define S1AP_S1_SETUP_RESP_MME_NAME_PRESENT 0x01
#define S1AP_S1_SETUP_RESP_CRITICALITY_DIAGNOSTICS_PRESENT 0x02
    /* Pointer to MME name string */
    U8                          p_mme_name[MAX_MME_NAME_STR];
    s1ap_ServedGUMMEIs          served_gummeis;
    U8                          relative_mme_capacity;
    s1ap_CriticalityDiagnostics criticality_diagnostics;
    parsed_gummei_info          gummei_info;
}parsed_s1_setup_rsp_t;

typedef struct {
    U16                         bitmask;
#define S1AP_S1_SETUP_FAIL_TIME_TO_WAIT_PRESENT 0x01
#define S1AP_S1_SETUP_FAIL_CRITICALITY_DIAGNOSTICS_PRESENT 0x02
    s1ap_Cause                   cause;
    U32                          time_to_wait;
    s1ap_CriticalityDiagnostics criticality_diagnostics;
}parsed_s1_setup_fail_t;

typedef struct
{
    U16        bitmask;
#define S1AP_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT 0x01
#define S1AP_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT 0x02
#define S1AP_ERROR_INDICATION_CAUSE_PRESENT 0x04
#define S1AP_ERROR_INDICATION_CRITICALITY_DIAGNOSTICS_PRESENT 0x08

    s1ap_MME_UE_S1AP_ID  mme_ue_s1ap_id;
/*^ O, RRC_S1AP_ERROR_INDICATION_MME_UE_S1AP_ID_PRESENT ^*/
    s1ap_ENB_UE_S1AP_ID  enb_ue_s1ap_id;
/*^ O, RRC_S1AP_ERROR_INDICATION_ENB_UE_S1AP_ID_PRESENT ^*/
    s1ap_Cause           cause;
/*^ O, RRC_S1AP_ERROR_INDICATION_CAUSE_PRESENT ^*/
    s1ap_CriticalityDiagnostics criticality_diagnostics;
/*^ O, RRC_S1AP_ERROR_INDICATION_CRITICALITY_DIAGNOSTICS_PRESENT ^*/

} s1ap_error_indication_t;


typedef struct {
    s1ap_Inter_SystemInformationTransferType si_transfer_type;
}mme_direct_info_t;

typedef struct _stream_info_t
{
    /*The array holds the stream ids of an SCTP association.
     */
    U16 stream_ids[MAX_NUM_SCTP_STREAM];

    /*This field is used to distribute streams among UEs in a 
     *Round Robin fashion.
     */
    U8 stream_idx_returned;

    /*The field represents total number of streams present between 
     * eNB and peer MME.
     */
    U8  number_of_streams;

}stream_info_t;

typedef struct
{
    U8  ue_asso_type;
    U32 mme_ue_s1ap_id;
    U32 enb_ue_s1ap_id;
    U8  routing_id;
    U32 lppa_trans_id;
    U32 e_smlc_meas_id;
}lppa_proc_info_t;

typedef struct _mme_context_t
{
    /*It is the socket descriptor of an SCTP end point.
     */
    sctp_sd_t sctp_sd;

    /*This contains description of streams present between a 
     * particular MME and eNB.
     */
    stream_info_t stream_info;

    /*The field contains necessary information to open 
     * an SCTP association with an MME.
     */
    mme_comm_info_t connection_info;

    /* Served Gummei Information used by NNSF and is filled
     * with the information received in S1 SETUP RESP
     * */
    U8                          num_served_gummeis;       
    served_gummei_info_t        *p_served_gummeis;

    /*Timer ID of timer started for connection recovery. 
     */
    rrc_timer_t mme_timer_id;

    /*Timer ID for eNB Configuration Update 
     */
    rrc_timer_t enb_config_update_timer_id;

    /*This is a search tree whose key value 
     * is mmeUeS1apId allocated to a UE.
     */
    search_tree_t  mmeUeS1apId_search_tree;

    /*This contains MME name which may be provided by 
     * MME in S1 SETUP RESPONSE
     */
    U8 mme_name[MAX_MME_NAME_STR];

    /*It represents present state of an MME's state machine.
     */
    U8 current_mme_state;

    /* It represents present state of the eNB Configuration
     * update State Machine 
     */
    U8 current_enb_config_update_state;

    /* pws_start*/
    /*It represents present state of an enb mgr's state machine.
     */
    /* pws_end*/

    /*The field represents the number of retries done for S1 
     * SETUP Procedure.
     */
    U8 s1_config_retry_count;

    /* index of MME context in global mme context array
     */
    U8 mme_id;

    /* relative mme_capacity */
    s1ap_RelativeMMECapacity    relative_mme_capacity;

    /* OAM Transaction Identifier */
    /* Maintained for sending back the reply to OAM 
     * for the received message */
    U16 oam_transaction_id;

    /* ENB Configuration update procedure data ongoing data */
    s1ap_oam_enb_config_update_t *p_enb_configuration_update;

    /* Individual Count for eNB Configuration Update */
    U32 enb_config_updated_count;
    
    /* SPR 22789 Fix Start */
    U32 ue_connected;
    /* SPR 22789 Fix End */

    /* lppa procedure related info */
    lppa_proc_info_t lppa_info;

    /*SPR 15570 Fix Start*/
    /*Flag introduced to check EINPROGRESS error*/
    rrc_bool_et is_conn_ongoing ;

    /*Count to retry select in case of EINPROGRESS error*/
    U8 sctp_einprogress_retry_count;
   
    /*State maintained in case of EINPROGRESS error*/
    U8 sctp_einprogress_initiated_from;
    /*SPR 15570 Fix Stop*/

}mme_context_t;

/* SPR 15641 Fix Start */
typedef struct 
{
    YLNODE      node;

    U8 		mme_id;   

    U16 	asn_msg_len;

    U8          message[0];
}s1ap_kill_queue_node_t;
/* SPR 15641 Fix Stop */

typedef struct _sctp_sd_search_node_t
{
    /*CSPL nodeAnchor
     */
    search_tree_node_t nodeAnchor;

    /* The value of sctp socket descriptor.
     */
    sctp_sd_t sctp_sd;

    /*This mmeId of the MME whose corresponding sctp socket desc is
     * sctp_sd
     */
    U8 mmeId;

}sctp_sd_search_node_t;

typedef struct _mmeUeS1apId_search_node_t
{
    /*CSPL nodeAnchor
     */
    search_tree_node_t nodeAnchor;

    /*This is allocated by RRC and uniquely identifies an UE with in a eNB.
     */
    U32 enbUeS1apId;

    /*This ID is allocated by peer MME and it uniquely identifies the UE 
     * in that MME.
     */
    U32 mmeUeS1apId;

}mmeUeS1apId_search_node_t;

typedef struct _ue_s1ap_context_t
{
    U8    cell_index;
    /*This ID is allocated by peer MME and it uniquely identifies 
     * the UE in that MME.
     */
/* SPR 23260 + */
#ifndef __x86_64__
    U32 mme_ue_s1ap_id;
#else
    U64 mme_ue_s1ap_id;
#endif
/* SPR 23260 - */

    /*This ID uniquely identifies the stream in an SCTP association
     * between eNB and MME.
     */
    U16 stream_id;

    /*This ID uniquely identifies an MME context with in eNB S1AP Layer.
     */
    U8  mme_id;

    /*    This element identifies whether the UE associated logical 
     *    connection is complete or not (i.e the mme_ue_s1ap_id is valid or not)
     */
    U8  is_mme_ue_s1ap_id_valid;
    /*BUG 604 changes start*/
    U16 ue_index;

    U32 enb_ue_s1ap_id;
     /*BUG 604 changes stop*/

}ue_s1ap_context_t;

typedef struct 
{
    /* This is set whn s1ap contexts are deleted without waiting for 
     * RESET ACK , Eg: CELL DEREQ REQ */
    U8 is_s1ap_ctx_deleted;

    /* It is used to store module Id of the module to which
       a response needs to be sent and the end of RESET procedure */
    U8 send_resp_to_module;

    /*This field is incremented upon expiry of mme_timer for reset, 
     * it's max count value is same as  max_retry_for_s1_setup
     */
    U8 reset_retry_count;

    /* Number of entries present in mme_reset_info */
    /*In case of eNB initiated reset this variable is set to number 
     * of MMEs to which RESET EP is sent. The value of this variable 
     * is decremented upon reception of RESET ACK from a MME. 
     * When The value of variable becomes zero upon reception 
     * of RESET ACK , S1AP_OAMH_RESET_ACK is sent to OAMH
     */
    U8 mme_count;

    /* Timer Id of timer start for the RESET procedure */
    rrc_timer_t timer_id;

    /* Array of pointers to s1ap_reset_info_t per MME indexed with
       mme_id and holds reset_info sent to a particular MME 
       during RESET Procedure */
    reset_req_t * mme_reset_info[MAX_NUM_MME];

    /* This is set when s1ap context is deleted for a single UE  */
    U8 is_ue_ctx_deleted;
}reset_context_t;

typedef struct reset_context_node
{
    /* List node Anchor */
    list_node_t node;

    /* Reset context */
    reset_context_t reset_context;
}reset_context_node_t;

typedef struct _s1ap_cell_area_info_t
{
    s1ap_plmn_identity_t plmn_id;

    U8      cell_index;
    cell_area_info_t cell_area_info;
}s1ap_cell_area_info_t;

typedef struct _cell_s1ap_context_t 
{
    s1ap_cell_area_info_t s1ap_cell_area_info;
    U16                   max_num_supported_ue;
}cell_s1ap_context_t;
/*BUG 604 changes start*/
/*typedef struct _uecc_s1ap_context_t 
{
    ue_s1ap_context_t  ** p_p_ue_s1ap_contexts;

}uecc_s1ap_context_t;*/
/*BUG 604 changes stop*/

typedef struct _active_mme_id_t
{   
    /* This is the active mme id
     *      */
    U8 mme_id;

    /* This represents the active MME overload state */
    U8 overload_response; /* mme_overload_response_et */

    U8      gummei_list_present;

    U8      traffic_load_ind_present;

    U16      num_gummei;

    s1ap_gummei_t           gummei_list[MAX_NUM_MMECS];

    U8      traffic_load_ind;

    /* SPR 22753 Fix Start */
    /* It represents the total no. of calls of cause received in overload_response */
    U8      total_no_of_calls; 

    /* It represents the total rejected calls of cause received in overload_response */
    U8      total_rejected_calls;
    /* SPR 22753 Fix End */

} active_mme_id_t;

typedef struct _active_mme_info_t  
{
    /*This is an array of MME Ids which contains all active MMEs.
     */
    active_mme_id_t mme_id_arr[MAX_NUM_MME];

    /*The number of MMEs with which SCTP association is active.
     */
    U8 active_mme_count;

    /*Used by NNSF to distribute MMEs in a Round Robin fashion.
     */
    U8 mme_idx_to_returned;

    s1ap_gummei_t   gummei;

}active_mme_info_t;

typedef struct _s1ap_oam_transaction_info_t
{
    /* Transaction id with OAM */
    U16 s1ap_oam_transaction_id;

    /* This variable holds the response that is to be
     * sent for RESET Req from OAM*/
    rrc_return_et reset_response;

}s1ap_oam_transaction_info_t;

typedef struct {
    /* MME ID in which this enbUeS1apId's logical S1 Connection is present */
    U8 mme_id;

    /* enbUeS1apId of UE to be RESET */
    U32 enbUeS1apId;
}s1ap_enbUeS1apId_reset_input_t;

typedef struct {
    /* Cause for RESET EP encoded in ASN RESET MSG */
    s1ap_Cause cause;
    
    /* Number of elements in enbUeS1apId array */
    U16 num_of_items;

     /* CSR_00040799 Fix Start */
    /* Array to hold enbUeS1apId Info*/
    s1ap_enbUeS1apId_reset_input_t enbUeS1apId_arr[MAX_UE_S1AP_ID_PAIR];
     /* CSR_00040799 Fix End */
}s1ap_enb_reset_info_t;

/*REL 1.2:HANDOVER PROCEDURE START*/
typedef struct 
{
    /* This ID is allocated by peer MME and it uniquely identifies the UE in
     * that MME. 
     */
    U32 mmeUeS1apId;

    /* This ID uniquely identifies the stream in an SCTP association between eNB
     * and MME.
     */
    U16 stream_id;

    /* This ID uniquely identifies an MME context with in eNB S1AP Layer. */
    U8 mme_id;

/*BUG 604 changes start*/
    U32 enb_ue_s1ap_id;
/*BUG 604 changes stop*/
}ho_ongoing_ue_ctx_t;

typedef struct _ho_ongoing_ue_ctx_node_t
{
    /* List node Anchor */
    list_node_t node;

    /* ho ongoing ue context */
    ho_ongoing_ue_ctx_t ho_ongoing_ue_ctx;
}ho_ongoing_ue_ctx_node_t;

/*REL 1.2:HANDOVER PROCEDURE END*/
typedef struct
{
  U8 cell_count;
  U8 cell_index[MAX_NUM_CELLS];  
  tracking_area_identifier_t         tai;
} tai_cell_index_t;

typedef struct _tai_cell_index_list_t
{
  U8                 tai_count;
  tai_cell_index_t   tai_info[MAX_NUM_TAI];
}tai_cell_index_list_t;

typedef struct
{
  U8 cell_count;
  U8 cell_index[MAX_NUM_CELLS];  
  U8 emergency_area_id[EMERGENCY_AREA_SIZE];;
} emergency_area_with_cell_index_t;

typedef struct _emergency_cell_index_list_t
{
  U8          emer_area_count;    
  emergency_area_with_cell_index_t emer_area_info[MAX_NUM_CELLS];
}emergency_cell_index_list_t;

typedef struct
{
  U8 cell_index;  
  s1ap_cgi_eutra_t cgi;
} cgi_with_cell_index_t;

typedef struct
{
  U8 cgi_count;
  cgi_with_cell_index_t cgi_info[MAX_NUM_CELLS];
}cgi_with_cell_index_list_t;

/* pws_start*/
#define S1AP_CSC_PWS_RESP_FAIL_CAUSE_PRESENT 0X0001
typedef struct _wrw_resp_t
{
  rrc_bitmask_t           bitmask;
  U8                      cell_index;
  rrc_response_t          resp;
  s1ap_cgi_eutra_t              eCGI;
  rrm_fail_cause_et       fail_cause;
}wrw_resp_t;

typedef struct _pws_resp_t
{
  U8      count;
  wrw_resp_t  pws_resp[MAX_NUM_CELLS];
}pws_resp_t;

typedef struct _pws_info_t
{
  rrc_bitmask_t         pws_cells;
  s1ap_rrm_pws_request_t *s1ap_rrm_pws_request;
  
  U8 *p_warning_message_contents;

  tai_cell_index_list_t    *tai_list;
  
  emergency_cell_index_list_t *emergency_list;
  
  pws_resp_t             *pws_resp;

  U8 mme_id;
 
} pws_info_t;
/* pws_end*/

#define S1AP_CSC_KILL_RESP_FAIL_CAUSE_PRESENT 0X0001
typedef struct _kill_resp_data_t
{
  rrc_bitmask_t           bitmask;
  U8                      cell_index;
  U16                     num_of_broadcasts;
  rrc_response_t          resp;
  s1ap_cgi_eutra_t        eCGI;
  rrm_fail_cause_et       fail_cause;
}kill_resp_data_t;

typedef struct _kill_resp_t
{
  U8                count;
  kill_resp_data_t  kill_resp_data[MAX_NUM_CELLS];
}kill_resp_t;

typedef struct 
{
  rrc_bitmask_t         pws_cells;
  s1ap_rrm_kill_request_t *s1ap_rrm_kill_request;

  tai_cell_index_list_t    *tai_list;
  
  emergency_cell_index_list_t *emergency_list;
  
  kill_resp_t             *kill_resp;

  U8 mme_id;
}kill_data_t ;

/*BUG 604 changes start*/
typedef struct
{
    U32      not_allocated_ue_ids_count;
    U32      not_allocated_ue_ids_cur;
    U32*     not_allocated_ue_ids; /*S1AP UEIDs ready to allocate*/
} s1ap_ueid_context_t;
/*BUG 604 changes stop*/
#define RRC_S1AP_LAST_SELECTED_MMEGI_PRESENT 0x01
#define RRC_S1AP_SCTP_CONFIG_PARAMS_PRESENT  0x02
typedef struct _s1ap_gb_context_t
{

    U16                   bitmask;
    /* List of reset contexts created per ENB initiated
       RESET Procedures */
    list_t reset_context_list;

    /* The SCTP information corresponding to eNB 
     */
    enb_comm_info_t enb_comm_info;

    /*Array of pointers to Cell contexts of type cell_s1ap_context_t .
     */
    cell_s1ap_context_t * cell_s1ap_contexts[MAX_NUM_CELLS];

    /*Array of pointers to UECC contexts of type uecc_s1ap_context_t .
     */
    /*BUG 604 changes start*/
    //uecc_s1ap_context_t   * * p_p_uecc_s1ap_contexts;
    ue_s1ap_context_t  ** p_p_ue_s1ap_contexts;
    /*BUG 604 changes stop*/
    /*Array of pointers to MME contexts for each MME with which 
     * SCTP association is established.
     */
    mme_context_t * mme_contexts[MAX_NUM_MME];

    /*The information necessary to form S1 SETUP REQUEST
     */
    s1_setup_req_t s1_setup_info;

    /*The timer value for which S1AP should wait for response to 
     * be senT from MME after sending S1 SETUP REQUEST.
     */
    rrc_timer_duration_t s1_config_resp_duration;

    /* pws_start*/
    /* The timer value for which S1AP should wait for response to 
     * be sent from RRM/CSC.
     */
    rrc_timer_duration_t s1_pws_duration;
    rrc_timer_duration_t s1_kill_duration;
    /* pws_end*/

    /*The timer value for which S1AP should wait for response to 
     * be senT from MME after sending RESET Msg.
     */
    rrc_timer_duration_t reset_duration;

    /*Contains information about active MMEs. This is used by NNSF.
     */
    active_mme_info_t active_mme_info;

    /*This is a search tree whose key value is sctp_sd  
     * allocated while opening connection with an MME.
     */
    search_tree_t  sctp_socket_desc_search_tree;

    /*Timer ID of s1ap oam init ind timer.
     */
    rrc_timer_t oam_init_ind_timer;

    /*This field represents the timer value with which timer is 
     * started to wait for connection recovery in case of 
     * connection failure indication from SCTP.
     */
    rrc_timer_duration_t s1ap_conn_recovery_duration;

    /* This field stores the transaction info with OAM */
    s1ap_oam_transaction_info_t s1ap_oam_tx_info;

    /*The maximum number of Retries that must be done for S1 
     * Set up procedure in case of S1 set up failure or time out.
     */
    U8 max_retry_for_s1_config;

    /*The maximum number of Retries that must be done for Reset
     * procedure when reset Ack isn't from a MME.
     */
    U8 max_retry_for_reset;

    /* This indicates the current state of S1AP , that is whether it
     * is initialized/provisioned by OAM or not. The state type is
     * s1ap_state_et
     */
    U8 s1ap_current_state;

    /* This is set to number of MMEs whose communication information
     * is received in S1AP OAM PROVISION request */
    U8 num_of_mme_prov;
    /*REL 1.2:HANDOVER PROCEDURE START*/
    
    /* This list is a temporary list of ho_ongoing_ue_ctx_nodes created upon
     * reception of Handover Request. The UE Context nodes are deleted upon
     * reception Handover Request Ack/Handover Failuare. In case of Handover
     * Request Ack the Handover is successful and a UE context is created in
     * Cell specific S1AP Global contexts. */
    list_t ho_ongoing_ue_ctx_list;

    /* Enb Configuration updated */
    U32 enb_config_updated_gb_count;

    /* eNB Configuration Update Response */
    s1ap_oam_enb_config_update_response_t *p_oam_enb_config_update_resp;

    /* eNB Configuration update OAM Response array index to be updated */
    U8 oam_enb_config_update_resp_arr_index;

    /*It represents present state of an enb mgr's state machine.
     */
    U8 current_enb_mgr_state;

    /*Timer ID for pws 
     */
    rrc_timer_t pws_timer_id;
    rrc_timer_t kill_timer_id ;             

    /* enb mgr transaction id */
    rrc_transaction_id_t        trans_id;
    /* pws_info */
    pws_info_t *p_pws_info;
    kill_data_t  *p_kill_data ;

    rrc_timer_duration_t s1_kill_request_duration ;
    /*REL 1.2:HANDOVER PROCEDURE END*/
    U8                   no_of_uecc_instances ;

    U16                  total_ue_supported;
    
    U8                   mme_selection_algo; /* s1ap_oam_mme_selection_algo_et */

    s1ap_MME_Group_ID    last_selected_mmegi;
    /*^ O, RRC_S1AP_LAST_SELECTED_MMEGI_PRESENT ^*/

    rrc_bool_et          is_sel_mmes_overloaded;    

    x2_enb_comm_info_t x2_enb_comm_info;
    
    /*sctp parameters start*/
    rrc_timeval_t         sctp_start_time;
    s1ap_sctp_conf_info_t  s1ap_sctp_conf_param;
    /*^ O, RRC_S1AP_SCTP_CONFIG_PARAMS_PRESENT ^*/
    /*sctp parameters stop*/
    U8                    gl_s1ap_timer_exp_counter;
    /*Bug 9305 start*/
    U32                   s1ap_health_monitor_val[S1AP_SCTP_MAX_THREADS];
    /*Bug 9305 end*/
/*BUG 604 changes start*/
    s1ap_ueid_context_t   s1ap_ueid_ctx;
/*BUG 604 changes stop*/
    /* SPR 13432 Fix Start */
    U8  s1_conn_mode_one_flag;
    
    /* Most recent mme_id for which sctp association has been requested */
    U8 last_selected_mme_id;
    /* SPR 13432 Fix Stop */
/* mme grp id feature changes start */
    U8 matching_gummei_to_be_sent;
/* mme grp id feature changes stop */
    rrc_bool_et          log_enabled;  /* current state of logger(on/off) */
    U8                   log_level;

    /* SPR 15641 Fix Start */
    list_t                s1ap_kill_queue;
    /* SPR 15641 Fix Stop */
} s1ap_gb_context_t;

/* eNB Configuration Update Failiure */
typedef struct {
    U8 bitmask;
#define S1AP_TIME_TO_WAIT_PRESENT 0x01
#define S1AP_CRITICALITY_DIAGNOSTICS_PRESENT 0x02
    /* Cause sent by MME */
    s1ap_Cause cause;

    /* Time to wait sent by MME */
    s1ap_TimeToWait_Root time_to_wait;

    /* Criticality Diagnostics reported by MME */
    s1ap_CriticalityDiagnostics *p_criticality_diagonostics;
} enb_config_update_failure_t;

typedef struct
{
    U8 bitmask;
#define MME_CONFIGURATION_MME_NAME_PRESENT 0x01
#define MME_CONFIGURATION_REL_MME_CAPACITY_PRESENT 0x02
/* CSR_00059128 Fix + */
/* CSR_00070383 Fix + */
#define MME_CONFIGURATION_SERVED_GUMMEI 0x04
/* CSR_00070383 Fix + */
/* CSR_00059128 Fix + */
    U8 mme_name[MAX_MME_NAME_STR];
    parsed_gummei_info    gummei_info;
    s1ap_ServedGUMMEIs    served_gummeis;
    U8                    relative_mme_capacity;
} mme_configuration_update_t;

typedef struct
{
    s1ap_Cause              cause;
    U32 time_to_wait;
    s1ap_CriticalityDiagnostics criticality_diagnostics;
} mme_configuration_update_failure_t;

typedef struct _active_mme_pool_area_node_t  
{
    /* List node Anchor */
    list_node_t node;

    /* This is the active mme id in the mme pool */
    U8 mme_id;
    
    /* Relative MME capacity */
    U8 relative_mme_capacity;

}active_mme_pool_area_node_t;


/* LPPA Start */
typedef struct
{
   s1ap_MME_UE_S1AP_ID  mme_ue_s1ap_id;
   s1ap_ENB_UE_S1AP_ID  enb_ue_s1ap_id;
   s1ap_Routing_ID      routing_id;
   s1ap_LPPa_PDU        lppa_pdu;
}dl_ue_asso_lppa_trans_t;

typedef struct
{
   s1ap_Routing_ID      routing_id;
   s1ap_LPPa_PDU        lppa_pdu;
}dl_non_ue_asso_lppa_trans_t;


/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

#endif  /* _S1AP_GLB_CTX_H_ */


