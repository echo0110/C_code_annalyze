/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: uecc_global_ctx.h,v 1.8 2010/10/13 07:03:17 gur18569 Exp $
 ****************************************************************************
 *
 *   DESCRIPTION:
 *       This is the UECC global context data definition file.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: uecc_global_ctx.h,v $
 * Revision 1.8  2010/10/13 07:03:17  gur18569
 * Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 * Revision 1.6.4.1  2010/08/05 08:05:54  gur15697
 * merging round 2
 *
 * Revision 1.6  2010/03/24 09:50:42  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.5  2010/02/24 04:25:53  gur10248
 * comments_dpisposed_cell_del_cell_setup_1_1
 *
 * Revision 1.4  2010/02/18 04:19:02  gur10248
 * LTE_RRC_Rel1.1_18feb
 *
 * Revision 1.3  2010/01/11 11:48:29  gur10248
 * *** empty log message ***
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.4.7  2009/12/08 21:17:02  ukr15916
 * LG_6 fixed
 *
 * Revision 1.1.4.6  2009/12/08 11:22:51  ukr16032
 * UECC - inspection log items LG_20, LG_23, LG_24, LG_26, LG_27, LG_28, LG_29, LG_34, LG_40, LG_41 fixed
 *
 * Revision 1.1.4.5  2009/11/26 18:33:48  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.4.3.2.12  2009/11/24 12:12:51  ukr15916
 * tabs expanded, lineending updated for Unix format
 *
 * Revision 1.1.4.3.2.11  2009/11/24 11:32:34  ukr15916
 * updated for dev_rel_1_0
 *
 * Revision 1.1.4.3.2.9.2.1  2009/11/23 16:43:50  ukr15916
 * beautification
 *
 * Revision 1.1.4.3.2.9  2009/11/19 09:52:17  ukr15916
 * tabs -> spaces
 *
 * Revision 1.1.4.3.2.8  2009/11/13 13:14:05  ukr18880
 * comments ordering
 *
 * Revision 1.1.4.3.2.7  2009/11/13 12:36:55  ukr18880
 * *** empty log message ***
 *
 * Revision 1.1.4.3.2.6  2009/11/13 12:23:05  ukr18880
 * comments ordering
 *
 * Revision 1.1.4.3.2.5  2009/11/12 12:25:49  ukr18880
 * New parameter to global context was added from oamh provision request
 *
 * Revision 1.1.4.3.2.4  2009/11/10 20:02:17  ukr16032
 * UECC S1AP_STATUS_MSG_IND handling added
 *
 *
 ****************************************************************************/

#ifndef UECC_GLOBAL_CTX_H
#define UECC_GLOBAL_CTX_H

/****************************************************************************
 * Local includes
 ****************************************************************************/

#include "rrc_defines.h"
#include "rrc_csc_uecc_intf.h"
#include "uecc_ue_ctx.h"
#include "uecc_statistic.h"
#include "uecc_ue_timer_utils.h"
#include "rrc_oam_intf.h"
#include "rrc_oamh_intf.h"
#include "search_tree_wrap.h"
#include "rrc_x2ap_uecc_intf.h"
#include "rrc_s1apOam_intf.h"


/****************************************************************************
 * Public definitions
 ****************************************************************************/

#define UECC_READY_OAMH_FLAG     0x1



/****************************************************************************
* Public types definitions
****************************************************************************/

typedef struct
{
    
    uecc_ue_context_t**     ue_contexts; /*UE contexts DB*/

    /*should be U32 to avoid overflow if idx==0xFFFF*/
    U32                     not_allocated_ue_ids_count;
    U32                     not_allocated_ue_ids_cur;

    rrc_ue_index_t*         not_allocated_ue_ids; /*UE IDs ready to allocate*/
} uecc_ue_contexts_db_t;

typedef struct _ho_ongoing_ue_identity_search_node_t
{
    /*CSPL nodeAnchor */
    search_tree_node_t nodeAnchor;

    /* The value of UeIdentity(phyCellId+C-RNTI) . */
    U32    ue_identity;

    /*This ueIndex of the Ue whose corresponding ue identity(phyCellId+C-RNTI) */
    U16    ue_index;

}ho_ongoing_ue_identity_search_node_t;

typedef struct
{
    x2ap_link_info_t    x2ap_link_info[MAX_PEER_ENB];                  
    U32                 not_allocated_ue_ids_count;
    U32                 not_allocated_ue_ids_cur;

    U16                 not_allocated_ue_ids[MAX_NUM_SUPPORTED_X2AP_UEID]; /*X2AP UEIDs ready to allocate*/
} uecc_x2ap_gb_context_t;

typedef struct
{
    U32             mme_id;
    rrc_bool_t      mme_connection_status;
    s1ap_gummei_t   gummei;
    //Trace Change Start
    mme_comm_info_t connection_info;
    //Trace Change End
} mme_ctx_t;


typedef struct
{
    U8                      erab_id;
    U8                      qci;
    rrc_timer_stats_t       rrc_erab_setup_time;
}rrc_erab_setup_qci_id_t;

typedef struct
{
    U32                     rrc_erab_setup_time_per_qci[MAX_MME_ERAB_LIST_COUNT];
    rrc_erab_setup_qci_id_t rrc_erab_setup_qci_id;
    U8                      rrc_erab_setup_count;
}rrc_erab_setup_meas_t;


typedef struct _rrc_csc_context_t
{
    rrc_csc_uecc_init_setup_ind_t *p_csc_init_setup_ind;

    /* global statistics */
    rrc_uecc_statistic_t          *p_rrc_uecc_statistic;
    /* this del_cell_ongoing is set when UECC receive CSC cell Del req */
    U8                            del_cell_ongoing;
    U16                           count_use_ue_del_cell_req;
    rrc_bool_et                   is_cell_stop_ongoing;
    /* this cell_trace_ongoing is set when UECCC receives OAMH Cell traffic
    * trace start */
    U8                            cell_trace_ongoing; /* RRC_TRUE or RRC_FALSE */
    rrc_uecc_cell_traffic_trace_info_t* p_cell_traffic_trace_info;
    /* this is used to generate trace id for trace reference which
     * in turn used in cell traffic trace */
    // Trace Change Start
    U8        traceRef[TRACE_REF_LENGTH];
    // Trace Change End
   /* CSR 56323/Bug8834 FIX start */
    U8     cdma2000_rand_val_in_use_flag; 
    /* This flag is set when a rand value is sent to UE in 
     * HandoverFromEutraPreparartionRequest message and 
     * the value is stored in  cdma2000_rand_val_used */
    U32    cdma2000_rand_val_used; 
   /* CSR 56323/Bug8834 FIX stop */
 
} rrc_csc_context_t;

typedef struct
{
    U16  ue_index;
    U16  pci;
    U16  crnti;
    U16  short_mac_i;
    U8   ue_context_released;
}ue_context_info_t;

typedef struct _ue_context_info_list_node_t
{
    /* List node Anchor */
    list_node_t node;

    /* ue context information */
    ue_context_info_t   ue_ctx_info;
}ue_context_info_list_node_t;

typedef struct _uecc_gb_context_t
{
    /* bitmask with info regarding ready status */
    rrc_bitmask_t           ready_condition;

    /* timers info */
    rrc_timer_duration_t    oamh_init_ind_duration;
    rrc_timer_t             oamh_init_ind_timer;
    uecc_ue_global_timer_data_t ue_timers_data;

    /*CSC Context*/
    rrc_csc_context_t **p_p_csc_context;

    /* ue contexts */
    uecc_ue_contexts_db_t   ue_db;

    /* Security capabilities */
    rrc_oam_supported_security_algo_per_cell_t      supported_security_algo_per_cell;

    mme_ctx_t        mme_ctx[MAX_NUM_MME];
    /* MME connection status information */
    rrc_bool_t              mme_connection_status;


    void*                   p_event_queue;

    /* Store UeIdentity whose Handover ongoing at target side*/
    search_tree_t           ho_ongoing_ue_identity_search_tree;

    U8         len_vendor_name;
    U8         vendor_name[RRC_VENDOR_NAME_LEN];
    uecc_x2ap_gb_context_t  x2ap_ctx;
    U16                     total_ue_supported;
    U16                     context_index;
    const S8*               facility_name;
    U8                      uecc_module_id;
    S8                      log_file_name[MAX_UECC_FILE_NAME_SIZE];
    U8                      offset_uecc_mod_id;

    rrc_erab_setup_meas_t   rrc_erab_setup_meas;


    /* SPR 4118 Start */
    U16    pdcp_integrity_threshold_counter;
    U16    uecc_integrity_threshold_timer_value;
    /* SPR 4118 Stop */
    //Trace Change Start
    enb_type_et enbType; 
    U8     macro_enb_id[MACRO_ENB_ID_OCTET_SIZE];
    U8     home_enb_id[HOME_ENB_ID_OCTET_SIZE];
    U16    traceRecordSessionRef;
    U8     enbName[MAX_ENB_NAME_STR_SIZE];
    //Trace Change End
    /* cell_index for INTER_CELL_HO */
    U8                  inter_cell_ho_cell_index;  
    U32    max_ue_context_list_size;
    list_t ue_context_info_list;
    /* lipa start */
    U8 lgw_configured_status;
    U8 lgw_data_length;
    U8 lgw_ip_addr[MAX_TRANSPORT_LAYER_ADDR_SIZE];
    /* lipa end */
    /* BUG_11901 start */
    U8 reset_ongoing;// will be set as TRUE once reset is ongoing
    /* BUG_11901 stop */
    /* BUG 12722 Fix Start */
    U16 count_use_ue_reset_req;
    /* BUG 12722 Fix Stop */
    /* SPR 21458 Start */
    U8                          l2_instance_reset_flag; /* Indicates the Layer2 presence during cell delete */ 
    /* SPR 21458 End */

} uecc_gb_context_t;

/*SPR 5253 Start*/
uecc_ue_context_t* uecc_ue_ctx_get_by_x2ap_id(
           uecc_gb_context_t*  p_uecc_gb_context,
           /*SPR_16618_START*/
           U8                  peer_enodeb_id,   /* Peer EnodeB ID */
           /*SPR_16618_END*/
           U16                 enb_ue_x2ap_id,
           U32                 x2ap_id_type
           );
/*SPR 5253 Stop*/

/*BUG 604 changes start*/
uecc_ue_context_t* uecc_ue_ctx_get_by_x2ap_id_ho_cancel(
    uecc_gb_context_t*  p_uecc_gb_context,  /* UECC global context */
    /*SPR_16618_START*/
    U8                  peer_enodeb_id,      /* Peer EnodeB ID */
    /*SPR_16618_END*/
    U16                 enb_ue_x2ap_id,     /* X2AP id */
    U32                 x2ap_id_type
);

/*SPR_17792_start*/
uecc_ue_context_t* uecc_ue_ctx_get_by_x2_id_intra_cell_ho_cancel(
    uecc_gb_context_t*  p_uecc_gb_context,  /* UECC global context */
    U16                 enb_ue_x2ap_id,     /* X2AP id */
    U32                 x2ap_id_type
);

/*SPR_17792_end*/

uecc_ue_context_t* uecc_ue_ctx_get_by_x2ap_id_src_enb(
    uecc_gb_context_t*  p_uecc_gb_context,  /* UECC global context */
    /*SPR_16618_START*/
    U8                  peer_enodeb_id,      /* Peer EnodeB ID */
    /*SPR_16618_END*/
    U16                 enb_ue_x2ap_id,     /* X2AP id */
    U32                 x2ap_id_type
);

/*SPR_17792_start*/
uecc_ue_context_t* uecc_ue_ctx_get_by_x2_id_src_intra_cell_ho(
    uecc_gb_context_t*  p_uecc_gb_context,  /* UECC global context */
    U16                 enb_ue_x2ap_id,     /* X2AP id */
    U32                 x2ap_id_type
);

/*SPR_17792_end*/

/*BUG 604 changes stop*/
#endif /* UECC_GLOBAL_CTX_H */
