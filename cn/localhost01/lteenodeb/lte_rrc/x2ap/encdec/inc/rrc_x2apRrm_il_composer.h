/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2014 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: $
 ****************************************************************************
 *
 *  File Description : The file rrc_x2apRrm_il_composer.h contains the prototypes 
 *                     of RRC-X2APRRM interface message composing functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $
 *
 ****************************************************************************/
#ifndef _RRC_X2APRRM_IL_COMPOSER_H_
#define _RRC_X2APRRM_IL_COMPOSER_H_

#include "rrc_defines.h"
#include "rrc_x2apRrm_intf.h"

rrc_length_t
rrc_il_get_rrm_updated_mbms_sf_info_len
(
    rrm_updated_mbms_sf_info_t *p_rrm_updated_mbms_sf_info
);

rrc_return_et
rrc_il_compose_rrm_updated_mbms_sf_info
(
    U8  **pp_buffer,
    rrm_updated_mbms_sf_info_t *p_rrm_updated_mbms_sf_info
);

rrc_length_t
rrc_il_get_x2ap_rrm_x2_link_down_ind_len
(
    x2ap_rrm_x2_link_down_ind_t *p_x2ap_rrm_x2_link_down_ind
);

rrc_return_et
rrc_il_compose_x2ap_rrm_x2_link_down_ind
(
    U8  **pp_buffer,
    x2ap_rrm_x2_link_down_ind_t *p_x2ap_rrm_x2_link_down_ind
);

rrc_length_t
rrc_il_get_x2ap_rrm_x2_link_up_ind_st_len
(
    x2ap_rrm_x2_link_up_ind_st *p_x2ap_rrm_x2_link_up_ind_st
);

rrc_return_et
rrc_il_compose_x2ap_rrm_x2_link_up_ind_st
(
    U8  **pp_buffer,
    x2ap_rrm_x2_link_up_ind_st *p_x2ap_rrm_x2_link_up_ind_st
);

rrc_length_t
rrc_il_get_x2ap_rrm_rsu_rrm_start_req_len
(
    x2ap_rrm_rsu_rrm_start_req_t *p_x2ap_rrm_rsu_rrm_start_req
);

rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_rrm_start_req
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_rrm_start_req_t *p_x2ap_rrm_rsu_rrm_start_req
);

rrc_length_t
rrc_il_get_x2ap_rrm_rsu_enb_start_res_len
(
    x2ap_rrm_rsu_enb_start_res_t *p_x2ap_rrm_rsu_enb_start_res
);

rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_enb_start_res
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_enb_start_res_t *p_x2ap_rrm_rsu_enb_start_res
);

rrc_length_t
rrc_il_get_x2ap_rrm_rsu_enb_stop_res_len
(
    x2ap_rrm_rsu_enb_stop_res_t *p_x2ap_rrm_rsu_enb_stop_res
);

rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_enb_stop_res
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_enb_stop_res_t *p_x2ap_rrm_rsu_enb_stop_res
);

rrc_length_t
rrc_il_get_x2ap_rrm_rsu_rrm_start_res_len
(
    x2ap_rrm_rsu_rrm_start_res_t *p_x2ap_rrm_rsu_rrm_start_res
);

rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_rrm_start_res
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_rrm_start_res_t *p_x2ap_rrm_rsu_rrm_start_res
);

rrc_length_t
rrc_il_get_x2ap_rrm_rsu_rrm_stop_res_len
(
    x2ap_rrm_rsu_rrm_stop_res_t *p_x2ap_rrm_rsu_rrm_stop_res
);

rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_rrm_stop_res
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_rrm_stop_res_t *p_x2ap_rrm_rsu_rrm_stop_res
);

rrc_length_t
rrc_il_get_x2ap_rrm_rsu_enb_start_req_len
(
    x2ap_rrm_rsu_enb_start_req_t *p_x2ap_rrm_rsu_enb_start_req
);

rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_enb_start_req
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_enb_start_req_t *p_x2ap_rrm_rsu_enb_start_req
);

rrc_length_t
rrc_il_get_x2ap_rrm_rsu_rrm_update_len
(
    x2ap_rrm_rsu_rrm_update_t *p_x2ap_rrm_rsu_rrm_update
);

rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_rrm_update
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_rrm_update_t *p_x2ap_rrm_rsu_rrm_update
);

rrc_length_t
rrc_il_get_x2ap_rrm_rsu_rrm_stop_req_len
(
    x2ap_rrm_rsu_rrm_stop_req_t *p_x2ap_rrm_rsu_rrm_stop_req
);

rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_rrm_stop_req
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_rrm_stop_req_t *p_x2ap_rrm_rsu_rrm_stop_req
);

rrc_length_t
rrc_il_get_x2ap_rrm_rsu_enb_stop_req_len
(
    x2ap_rrm_rsu_enb_stop_req_t *p_x2ap_rrm_rsu_enb_stop_req
);

rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_enb_stop_req
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_enb_stop_req_t *p_x2ap_rrm_rsu_enb_stop_req
);

rrc_length_t
rrc_il_get_x2ap_rrm_rsu_enb_update_ind_len
(
    x2ap_rrm_rsu_enb_update_ind_t *p_x2ap_rrm_rsu_enb_update_ind
);

rrc_return_et
rrc_il_compose_x2ap_rrm_rsu_enb_update_ind
(
    U8  **pp_buffer,
    x2ap_rrm_rsu_enb_update_ind_t *p_x2ap_rrm_rsu_enb_update_ind
);

rrc_length_t
rrc_il_get_x2ap_rrm_load_information_req_len
(
    x2ap_rrm_load_information_req_t *p_x2ap_rrm_load_information_req
);

rrc_return_et
rrc_il_compose_x2ap_rrm_load_information_req
(
    U8  **pp_buffer,
    x2ap_rrm_load_information_req_t *p_x2ap_rrm_load_information_req
);

rrc_length_t
rrc_il_get_x2ap_rrm_load_information_resp_len
(
    x2ap_rrm_load_information_resp_t *p_x2ap_rrm_load_information_resp
);

rrc_return_et
rrc_il_compose_x2ap_rrm_load_information_resp
(
    U8  **pp_buffer,
    x2ap_rrm_load_information_resp_t *p_x2ap_rrm_load_information_resp
);

rrc_length_t
rrc_il_get_x2ap_rrm_load_information_ind_len
(
    x2ap_rrm_load_information_ind_t *p_x2ap_rrm_load_information_ind
);

rrc_return_et
rrc_il_compose_x2ap_rrm_load_information_ind
(
    U8  **pp_buffer,
    x2ap_rrm_load_information_ind_t *p_x2ap_rrm_load_information_ind
);

rrc_length_t
rrc_il_get_x2ap_rlf_ind_len
(
    x2ap_rlf_ind_t *p_x2ap_rlf_ind
);

rrc_return_et
rrc_il_compose_x2ap_rlf_ind
(
    U8  **pp_buffer,
    x2ap_rlf_ind_t *p_x2ap_rlf_ind
);

#ifdef ENDC_ENABLED
rrc_length_t
rrc_il_get_x2ap_endc_config_update_en_gnb_ind_len
(
    x2ap_endc_config_update_en_gnb_ind_t *p_x2_endc_config_update_ind
);

rrc_return_et
rrc_il_compose_x2ap_endc_config_update_en_gnb_ind
(
    U8  **pp_buffer,
    x2ap_endc_config_update_en_gnb_ind_t *p_x2_endc_config_update_ind
);

rrc_length_t
rrc_il_get_x2ap_endc_config_update_req_len
(
    x2ap_endc_config_update_req_t *p_x2ap_endc_config_update_req
);

rrc_return_et
rrc_il_compose_x2ap_endc_config_update_req
(
    U8  **pp_buffer,
    x2ap_endc_config_update_req_t *p_x2ap_endc_config_update_req
);

rrc_length_t
rrc_il_get_x2ap_endc_config_update_resp_len
(
    x2ap_endc_config_update_resp_t *p_x2ap_endc_config_update_resp
);

rrc_return_et
rrc_il_compose_x2ap_endc_config_update_resp
(
    U8  **pp_buffer,
    x2ap_endc_config_update_resp_t *p_x2ap_endc_config_update_resp
);

rrc_length_t
rrc_il_get_x2ap_endc_config_update_wait_ind_len
(
    x2ap_endc_config_update_wait_ind_t *p_x2ap_endc_config_update_wait_ind
);

rrc_return_et
rrc_il_compose_x2ap_endc_config_update_wait_ind
(
    U8  **pp_buffer,
    x2ap_endc_config_update_wait_ind_t *p_x2ap_endc_config_update_wait_ind
);

rrc_return_et
rrc_x2apRrm_il_send_rrm_updated_mbms_sf_info
(
    rrm_updated_mbms_sf_info_t  *p_rrm_updated_mbms_sf_info,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_x2_link_down_ind
(
    x2ap_rrm_x2_link_down_ind_t  *p_x2ap_rrm_x2_link_down_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_x2_link_up_ind_st
(
    x2ap_rrm_x2_link_up_ind_st  *p_x2ap_rrm_x2_link_up_ind_st,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_rsu_rrm_start_req
(
    x2ap_rrm_rsu_rrm_start_req_t  *p_x2ap_rrm_rsu_rrm_start_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_rsu_enb_start_res
(
    x2ap_rrm_rsu_enb_start_res_t  *p_x2ap_rrm_rsu_enb_start_res,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_rsu_enb_stop_res
(
    x2ap_rrm_rsu_enb_stop_res_t  *p_x2ap_rrm_rsu_enb_stop_res,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_rsu_rrm_start_res
(
    x2ap_rrm_rsu_rrm_start_res_t  *p_x2ap_rrm_rsu_rrm_start_res,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_rsu_rrm_stop_res
(
    x2ap_rrm_rsu_rrm_stop_res_t  *p_x2ap_rrm_rsu_rrm_stop_res,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_rsu_enb_start_req
(
    x2ap_rrm_rsu_enb_start_req_t  *p_x2ap_rrm_rsu_enb_start_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_rsu_rrm_update
(
    x2ap_rrm_rsu_rrm_update_t  *p_x2ap_rrm_rsu_rrm_update,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_rsu_rrm_stop_req
(
    x2ap_rrm_rsu_rrm_stop_req_t  *p_x2ap_rrm_rsu_rrm_stop_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_rsu_enb_stop_req
(
    x2ap_rrm_rsu_enb_stop_req_t  *p_x2ap_rrm_rsu_enb_stop_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_rsu_enb_update_ind
(
    x2ap_rrm_rsu_enb_update_ind_t  *p_x2ap_rrm_rsu_enb_update_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_load_information_req
(
    x2ap_rrm_load_information_req_t  *p_x2ap_rrm_load_information_req,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_load_information_resp
(
    x2ap_rrm_load_information_resp_t  *p_x2ap_rrm_load_information_resp,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apRrm_il_send_x2ap_rrm_load_information_ind
(
    x2ap_rrm_load_information_ind_t  *p_x2ap_rrm_load_information_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et
rrc_x2apRrm_il_send_x2ap_rlf_ind
(
    x2ap_rlf_ind_t  *p_x2ap_rlf_ind,
    U16                 src_module_id,
    U16                 dst_module_id,
    U16                 transaction_id,
    U8                  cell_index
);

rrc_return_et 
rrc_x2apRrm_il_send_x2ap_endc_config_update_en_gnb_ind
(
    x2ap_endc_config_update_en_gnb_ind_t  *p_x2_endc_config_update_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dst_module_id,  /* Destination module identifier */
    U16                 transaction_id, /* Interface transaction identifier */
    U8                  cell_index      /* cell index */
);

rrc_return_et
rrc_x2apRrm_il_send_x2ap_endc_config_update_resp
(
    x2ap_endc_config_update_resp_t  *p_x2ap_endc_config_update_resp,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index       /* cell index */
);

rrc_return_et
rrc_x2apRrm_il_send_x2ap_endc_config_update_wait_ind
(
    x2ap_endc_config_update_wait_ind_t  *p_x2ap_endc_config_update_wait_ind,           /* Pointer to API specific information. */
    U16                 src_module_id,  /* Source module identifier */
    U16                 dest_module_id,  /* Destination module identifier */
    U16                 transaction_id,  /* Interface transaction identifier */
    U8                  cell_index       /* cell index */
);
#endif
#endif /* _RRC_X2APRRM_IL_COMPOSER_H_ */
