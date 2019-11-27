/******************************************************************************
*
*   FILE NAME   : x2ap_utils.h
*
*   DESCRIPTION : .
*
*   DATE            AUTHOR         REFERENCE       REASON
*   24-JUNE-2010    Vaibhav Singh  ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _X2AP_UTILS_H_
#define _X2AP_UTILS_H_

#include "x2ap_db.h"
#include "x2ap_tracing.h"
/*SPR_16723_START*/
#include "rrc_x2apCsc_intf.h"
/*SPR_16723_END*/

#define INVALID_SCTP_SOCKET -1
/****************************************************************************
 * Defines
 ***************************************************************************/

#define ARRSIZE(array_name)     (sizeof(array_name) / sizeof(array_name[0]))

#ifdef X2AP_DEBUG
void
x2ap_assert
(
    void* file,
    U32 line,
    void* expression
);

#define X2AP_ASSERT(exp) (void)((exp) || (x2ap_assert(__FILE__,__LINE__,#exp),0))
#else
#define X2AP_ASSERT(exp)
#endif

#define X2AP_MEMCPY l3_memcpy_wrapper
#define X2AP_MEMSET memset_wrapper
#define X2AP_MEMCMP memcmp_wrapper

U8 x2ap_get_version(void *p_header);

x2ap_module_id_t x2ap_get_src_module_id(void *p_header);

x2ap_module_id_t x2ap_get_dst_module_id(U8 *p_header);

U16 x2ap_get_api_id(U8 *p_header);

U16 x2ap_get_api_buf_size( U8 *p_header );

void x2ap_set_api_buf_size(
    U8      *p_header,    
    U16     api_buf_size);

void x2ap_construct_api_header(
    U8                  *p_header,    
    U8                  version_id,   
    x2ap_module_id_t     src_module_id, 
    x2ap_module_id_t     dst_module_id, 
    U16                 api_id,         
    U16                 api_buf_size);

U16 x2ap_get_transaction_id(void *p_api);

void x2ap_construct_interface_api_header(
    U8                  *p_header,      
    U16                 transaction_id, 
    x2ap_module_id_t     src_module_id,  
    x2ap_module_id_t     dst_module_id,  
    U16                 api_id,         
    U16                 api_buf_size);

U32 x2ap_get_nbr_index_from_ecgi(
        x2ap_gb_context_t *p_x2ap_gb_ctxt,
        x2ap_ecgi_t nbr_ecgi,
        x2ap_bool_t  intert_flag,
        U8 cell_id);

x2ap_peer_enb_context_t * x2ap_get_ctxt_from_sd(
        x2ap_gb_context_t *p_x2ap_gb_ctxt,
        sctp_sd_t sd);

x2ap_peer_enb_context_t * x2ap_get_ctxt_from_ip(
        x2ap_gb_context_t *p_x2ap_gb_ctxt,
        U8 *ip_addr);

x2ap_peer_enb_context_t * x2ap_get_enb_ctxt_from_enbid(
        x2ap_gb_context_t *p_x2ap_gb_ctxt,
        x2_gb_enb_id_t enbid);

		/*SPR 21554 Start*/
void x2ap_populate_ecgi_from_cell_id(
        x2ap_gb_context_t* p_x2ap_gb_ctx,
        U8 cell_id,
        x2ap_ecgi_t *ecgi_val
        );
/*SPR 21554 End*/
void x2ap_compose_ecgi_from_cell_id(
        x2_gb_enb_id_t g_enb_id,
        U8 cell_id,
        x2ap_ecgi_t *ecgi);
/*SPR_19279_START*/
void x2ap_compose_ecgi_from_cell_info(
        x2_gb_enb_id_t g_enb_id,
        x2ap_served_cell_info_t* p_cell_info,
        x2ap_ecgi_t *ecgi_val
        );
/*SPR_19279_END*/
void x2ap_check_active_cell(
        U16                 *p_num_act_cell,
        U16                 *p_act_cell_id,
        x2ap_gb_context_t   *p_x2ap_gl_ctx);

void x2ap_get_ngh_cell_index(
        U16                *p_num_ngh_cell,
        U16                *p_ngh_cell_index,
        U16                 served_cell_id,
        x2ap_gb_context_t *p_x2ap_gl_ctx,
        x2ap_bool_t         is_only_active);

void x2ap_set_sctp_sd(
    U32 *p_header,
    S32 value);

U32 x2ap_get_sctp_sd(
        U32 *p_header);

void x2ap_get_gb_enbid_from_nbr_cell_index(
                             x2ap_gb_context_t *p_x2ap_gl_ctx,
                             U16                 nbr_cell_index,
                             x2_gb_enb_id_t      *p_gb_enb_id);

void x2ap_get_enbid_set_from_nbr_cell_indexes(
        x2ap_gb_context_t *p_x2ap_gl_ctx,
        U16                num_ngh_cell,
        U16                *p_ngh_cell_index_arr,
        x2_gb_enb_id_t        *p_gb_enb_id_arr);

void x2ap_get_uniq_enbid_set_from_nbr_cell_indexes(
        x2ap_gb_context_t  *p_x2ap_gl_ctx,
        U16                num_ngh_cell,
        U16                *p_ngh_cell_index_arr,
        U16                *p_num_enbid,
        x2_gb_enb_id_t        *p_gb_enb_id_arr);

S32 x2ap_get_gu_grpid_index(
        x2ap_gb_context_t  *p_x2ap_gb_ctx,
        x2ap_gu_group_id_t gu_group_id);

x2ap_return_et validate_x2setup_time_to_wait_index_range(
                U8 x2ap_time_to_wait_index);


x2ap_return_et x2ap_check_active_cell_status(
                                x2ap_gb_context_t   *p_x2ap_gl_ctx,
                                U8                  cell_id,
                                x2ap_bool_et        *cell_present);

/*SPR_16723_START*/
void x2ap_update_cell_specific_params(
                                x2ap_gb_context_t  *p_x2ap_gl_ctx,
                                x2ap_csc_cell_add_t *p_x2ap_cell_add);
/*SPR_16723_END*/
/* X2AP FIX_AG_1 : Start */
x2ap_return_et x2ap_update_active_cell_status(     
                                   x2ap_gb_context_t   *p_x2ap_gb_ctxt,
                                   U8                  cell_id,
                                   x2ap_bool_et        cell_status);
/* X2AP FIX_AG_1 : End */

x2ap_return_et x2ap_get_cell_context_by_cell_id(
        x2ap_gb_context_t   *p_x2ap_gl_ctx,
        U8                  cell_id,
        U8*                 cell_index);

x2ap_return_et validate_x2setup_time_to_wait_value_range(
        U8 x2ap_time_to_wait_index);

void *x2ap_alloc_intrl_msg(
        x2ap_module_id_t     dst_module_id,
        U16                  api_id,
        U16                  msg_size);

x2ap_peer_enb_context_t * x2ap_get_peer_enb_ctxt_from_enbid(
        x2ap_gb_context_t *p_x2ap_gb_ctxt,
        x2_gb_enb_id_t enbid);

U16 x2ap_get_ue_id(
         U32 p_header);

/* SPR_14436_Fix Start */
U16 x2ap_get_stream_id(
        U16 *p_header 
);

U16 x2ap_get_stream_id_from_header(
        U8 *p_header
);

void x2ap_set_sctp_stream(
	 U16 *p_header ,U16 value
);

void x2ap_set_stream_from_header(
    U8 *p_header ,U16 value
);
/* SPR_14436_Fix Stop */
  /*SPR_20743_FIX_START*/
x2ap_return_et x2ap_delete_peer_enb_from_ip_v4_v6_search_tree(
        x2ap_gb_context_t   *p_x2ap_gl_ctx,
        x2ap_search_tree_t  *p_search_tree,
        x2ap_peer_enb_context_t *p_peer_enb_ctxt);

void x2ap_delete_peer_enb_from_search_tree(
        x2ap_gb_context_t   *p_x2ap_gl_ctx,
        x2ap_search_tree_t  *p_search_tree,
        x2ap_peer_enb_context_t *p_peer_enb_ctxt,
        x2ap_search_tree_node_t *p_node_to_delete);
/*SPR_20743_FIX_STOP*/
#endif
