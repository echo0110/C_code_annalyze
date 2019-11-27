
/*! \file    : rrm_mif_msg_handler.h
 *  \brief   : This file contains the message handlers of RRM Mgmt IfH.
 *  \date    : January 24, 2012 
 *  \author  : Hargovind Sharma
*/
/*!
 *  \copyright {Copyright (c) 2009, Aricent Inc. All Rights Reserved}
*/

#ifndef _RRM_MIF_MSG_HANDLER_H_
#define _RRM_MIF_MSG_HANDLER_H_
/*! \headerfile rrm_mif_db.h <>
 */
#include <rrm_mif_db.h>
/*! \headerfile son_rrm_intf.h <> 
 */
#include "son_rrm_intf.h"   

/*! \fn rrm_mif_rrmcm_init_ind_handler(rrm_mif_gb_context_t *p_gl_ctxt,void *p_msg) 
 *  \brief  This function is rrmcm init indication handler
 *  \param p_gl_ctxt Pointer to global context
 *  \param p_msg Pointer to message
*/
void rrm_mif_rrmcm_init_ind_handler
(
 rrm_mif_gb_context_t   *p_gl_ctxt
     /*SPR 17777 +-*/
 );

/* Configuration filename */
#define RRM_OAM_CONFIG_PARAM_FILE     "rrm_oam.cfg"

#define RRM_NW_SCAN_UNSUCCESSFUL  1

/* BUG_857 start*/
/*KPI changes*/
typedef rrm_void_t (*rrm_oam_bitmap_func_ptr)(rrmcm_kpi_data_t * ,rrm_oam_kpi_data_t *);
extern rrm_oam_bitmap_func_ptr g_rrm_oam_map_bit[RRM_MAX_KPI][RRM_EIGHT];
/* BUG_857 end*/

/*! \fn rrm_check_for_rmif_init_state(rrm_mif_gb_context_t *p_gl_ctxt)
 *  \brief  This function is for rmif state initialization
 *  \param p_gl_ctxt rrm_mif_gb_context_t
*/
void rrm_check_for_rmif_init_state
(
 rrm_mif_gb_context_t*  p_gl_ctxt
);

/*! \fn rrm_mif_rrmuem_init_ind_handler(rrm_mif_gb_context_t *p_gl_ctxt, void  *p_msg)
 *  \brief  This function is for init indication handler
 *  \param p_gl_ctxt 
 *  \param p_msg 
*/
void rrm_mif_rrmuem_init_ind_handler
(
 rrm_mif_gb_context_t   *p_gl_ctxt
     /*SPR 17777 +-*/
);

/*! \fn rrm_mif_rrmcm_init_config_resp_handler(rrm_mif_gb_context_t *p_gl_ctxt,void *p_msg)
 *  \brief this function is used for init config response
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_msg void
*/
void rrm_mif_rrmcm_init_config_resp_handler
(
 rrm_mif_gb_context_t   *p_gl_ctxt,
 void                    *p_msg
);

/*! \fn rrm_mif_rrmuem_init_config_resp_handler(rrm_mif_gb_context_t *p_rmif_gb_context, void *p_msg)
 *  \brief This function is used for rrm ue init config response
 *  \param p_rmif_gb_context  MIF global context
 *  \param p_msg  pointer to message
*/
void rrm_mif_rrmuem_init_config_resp_handler
(
 rrm_mif_gb_context_t   *p_rmif_gb_context,
 void                    *p_msg
);

/*! \fn rrm_check_for_rmif_init_state(rrm_mif_gb_context_t *  p_gl_ctxt) 
 *  \brief This function is for checking the mif initial state
 *  \param p_gl_ctxt rrm_mif_gb_context_t
*/
void rrm_check_for_rmif_init_state
(
 rrm_mif_gb_context_t*  p_gl_ctxt
);

/*! \fn rrm_check_for_rmif_active_state(rrm_mif_gb_context_t* p_gl_ctxt, U16 trans_id)
 *  \brief This function is used for checking mif active state 
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param trans_id U16
*/
void rrm_check_for_rmif_active_state
(
 rrm_mif_gb_context_t*  p_gl_ctxt,
 U16                    trans_id
);

/*! \fn void build_and_send_cell_config_req_to_cellm(rrm_mif_gb_context_t *p_gl_ctxt, rrm_mif_context *p_mif_context, rrm_oam_cell_config_req_t *p_in_cell_config_req_from_oam, rrmcm_rmif_cell_config_req_t *p_ut_cell_config_req_to_cellm)
 *  \brief This function is used to build and send config to cellm
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context  rrm_mif_context
 *  \param p_in_cell_config_req_from_oam  rrm_oam_cell_config_req_t
 *  \param p_ut_cell_config_req_to_cellm  rrmcm_rmif_cell_config_req_t
*/
void build_and_send_cell_config_req_to_cellm
(
   rrm_mif_gb_context_t  *p_gl_ctxt,
   rrm_mif_context             *p_mif_context, 
   rrm_oam_cell_config_req_t *p_in_cell_config_req_from_oam, 
   rrmcm_rmif_cell_config_req_t    *p_ut_cell_config_req_to_cellm
);

/*! \fn void build_and_send_cell_start_adm_req_for_cell_unblock_to_cellm(rrm_mif_gb_context_t *p_gl_ctxt, rrm_mif_context *p_mif_context, rrm_oam_cell_config_req_t *            p_in_cell_config_req_from_oam, rrmcm_rmif_cell_config_req_t *p_ut_cell_config_req_to_cellm)
 *  \brief This function is used to build and send config to cellm
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context  rrm_mif_context
 *  \param p_in_cell_config_req_from_oam  rrm_oam_cell_config_req_t
 *  \param p_ut_cell_config_req_to_cellm  rrmcm_rmif_cell_config_req_t
 */
void build_and_send_cell_start_adm_req_for_cell_unblock_to_cellm
(
     /*SPR 17777 +-*/
 rrm_mif_context             *p_mif_context,
     /*SPR 17777 +-*/
 rrmcm_rmif_cell_start_adm_req_for_cell_unblock_t    *p_out_cell_start_adm_req_for_cell_unblock_to_cellm
 );

/*! \fn void build_and_send_cell_stop_adm_req_for_cell_block_to_cellm(rrm_mif_gb_context_t *p_gl_ctxt, rrm_mif_context *p_mif_context, rrm_oam_cell_config_req_t *            p_in_cell_config_req_from_oam, rrmcm_rmif_cell_config_req_t *p_ut_cell_config_req_to_cellm)
 *  \brief This function is used to build and send config to cellm
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context  rrm_mif_context
 *  \param p_in_cell_config_req_from_oam  rrm_oam_cell_config_req_t
 *  \param p_ut_cell_config_req_to_cellm  rrmcm_rmif_cell_config_req_t
 */
void build_and_send_cell_stop_adm_req_for_cell_block_to_cellm
(
     /*SPR 17777 +-*/
 rrm_mif_context             *p_mif_context,
 rrmcm_rmif_cell_stop_adm_req_for_cell_block_t
 *p_out_cell_stop_adm_req_to_cellm
 );

/*! \fn void build_and_send_cell_reconfig_req_to_cellm(rrm_mif_gb_context_t *p_gl_ctxt, rrm_mif_context *p_mif_context, rrm_oam_cell_reconfig_req_t *p_in_cell_reconfig_req_from_oam, rrmcm_rmif_cell_reconfig_req_t *p_ut_cell_config_req_to_cellm)
 *  \brief This function is used to build and send reconfig to cellm
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param p_in_cell_reconfig_req_from_oam rrm_oam_cell_reconfig_req_t
 *  \param p_ut_cell_config_req_to_cellm rrmcm_rmif_cell_reconfig_req_t
*/
void build_and_send_cell_reconfig_req_to_cellm
(
 rrm_mif_gb_context_t  *p_gl_ctxt,
 rrm_mif_context             *p_mif_context, 
 rrm_oam_cell_reconfig_req_t *p_in_cell_reconfig_req_from_oam, 
 rrmcm_rmif_cell_reconfig_req_t    *p_ut_cell_config_req_to_cellm
);

/*! \fn void build_and_send_cell_del_req_to_cellm(rrm_mif_gb_context_t *p_gl_ctxt,rrm_mif_context *p_mif_context,rrmcm_rmif_cell_delete_req_t *p_ut_cell_del_req_to_cellm)
 *  \brief This function is used to  build and send delete request to cellm
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param p_ut_cell_del_req_to_cellm rrmcm_rmif_cell_delete_req_t
*/
void build_and_send_cell_del_req_to_cellm
(
     /*SPR 17777 +-*/
  rrm_mif_context       *p_mif_context,
  /*SPR 21369 Start*/
  rrm_oam_cell_delete_req_t  *p_cell_delete_req_msg_from_oam, 
  /*SPR 21369 End*/
  rrmcm_rmif_cell_delete_req_t    *p_ut_cell_del_req_to_cellm
);

/*! \fn void build_and_send_cell_stop_req_to_cellm(rrm_mif_gb_context_t *p_gl_ctxt,rrm_mif_context *p_mif_context,rrmcm_rmif_cell_stop_req_t *p_ut_cell_stop_req_to_cellm)
 *  \brief this function build and send stop req to cellm
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param p_ut_cell_stop_req_to_cellm rrmcm_rmif_cell_stop_req_t
*/
void build_and_send_cell_stop_req_to_cellm
(
     /*SPR 17777 +-*/
  rrm_mif_context             *p_mif_context, 
  rrmcm_rmif_cell_stop_req_t    *p_ut_cell_stop_req_to_cellm);

/*! \fn void build_and_send_cell_update_req_to_cellm(rrm_mif_gb_context_t *p_gl_ctxt,rrm_mif_context *p_mif_context,rrm_oam_cell_update_req_t *p_in_cell_update_req_from_oam,rrmcm_rmif_cell_update_req_t *p_out_cell_update_req_to_cellm)
 *  \brief This function is used to  build and send update to cellm
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param p_in_cell_update_req_from_oam rrm_oam_cell_update_req_t
 *  \param p_out_cell_update_req_to_cellm rrmcm_rmif_cell_update_req_t
*/
void build_and_send_cell_update_req_to_cellm
(
     /*SPR 17777 +-*/
 rrm_mif_context                *p_mif_context,
 rrm_oam_cell_update_req_t      *p_in_cell_update_req_from_oam,
 rrmcm_rmif_cell_update_req_t   *p_out_cell_update_req_to_cellm
);

/*! \fn void fill_plmn_info(rrm_oam_cell_plmn_info_t *p_cell_plmn_info_to_cellm,rrm_oam_cell_plmn_info_t *p_cell_plmn_info_from_oam)
 *  \brief Fill PLMN Information.
 *  \param p_cell_plmn_info_to_cellm rrm_oam_cell_plmn_info_t
 *  \param p_cell_plmn_info_from_oam rrm_oam_cell_plmn_info_t
 */
void fill_plmn_info(rrm_oam_cell_plmn_info_t     *p_cell_plmn_info_to_cellm,
                    rrm_oam_cell_plmn_info_t     *p_cell_plmn_info_from_oam);

/*! \fn void build_and_send_cell_start_req_to_cellm(rrm_mif_gb_context_t *p_gl_ctxt,rrm_mif_context *p_mif_context,rrmcm_rmif_cell_start_req_t  *p_ut_cell_start_req_to_cellm)
 *  \brief This function is used to build and send start req to cellm
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param p_ut_cell_start_req_to_cellm rrmcm_rmif_cell_start_req_t
*/
void build_and_send_cell_start_req_to_cellm
(
     /*SPR 17777 +-*/
   rrm_mif_context             *p_mif_context, 
   rrmcm_rmif_cell_start_req_t    *p_ut_cell_start_req_to_cellm);


/*! \fn void build_and_send_cell_stop_adm_req_to_cellm(rrm_mif_gb_context_t *p_gl_ctxt,rrm_mif_context *p_mif_context,rrmcm_rmif_cell_stop_adm_req_t  *to_cellm)
 *  \brief This function is used to build and send stop admission req to cellm
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param to_cellm rrmcm_rmif_cell_stop_adm_req_t
*/
void build_and_send_cell_stop_adm_req_to_cellm
(
     /*SPR 17777 +-*/
   rrm_mif_context             *p_mif_context, 
   rrmcm_rmif_cell_stop_adm_req_t  *to_cellm);
 
/*! \fn void build_and_send_cell_start_adm_req_to_cellm(rrm_mif_gb_context_t *p_gl_ctxt,rrm_mif_context *p_mif_context,rrm_son_cell_switch_on_req_t *p_in_cell_switch_on_from_son,rrmcm_rmif_cell_start_adm_req_t  *p_out_cell_start_adm_req_to_cellm)
 *  \brief This function is used to  build and send start admission req to cellm
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param p_in_cell_switch_on_from_son rrm_son_cell_switch_on_req_t
 *  \param p_out_cell_start_adm_req_to_cellm rrmcm_rmif_cell_start_adm_req_t
*/
void build_and_send_cell_start_adm_req_to_cellm
(
     /*SPR 17777 +-*/
  rrm_mif_context             *p_mif_context, 
     /*SPR 17777 +-*/
  rrmcm_rmif_cell_start_adm_req_t  *p_out_cell_start_adm_req_to_cellm);

/*! \fn void build_and_send_son_cell_register_req_to_cellm(rrm_mif_gb_context_t *p_gl_ctxt,rrm_mif_context *p_mif_context,rrm_son_register_req_t *p_in_cell_register_req_msg,U16 src_id)
 *  \brief This function is used build and send son register req to cellm
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param p_in_cell_register_req_msg  rrm_son_register_req_t
 *  \param src_id source identifier
 */
void build_and_send_son_cell_register_req_to_cellm
(
     /*SPR 17777 +-*/
    rrm_mif_context             *p_mif_context, 
    rrm_son_register_req_t *p_in_cell_register_req_msg,
    U16                     src_id);

/*! \fn void build_and_send_son_cell_deregister_req_to_cellm(rrm_mif_gb_context_t *p_gl_ctxt,rrm_mif_context *p_mif_context,rrm_son_deregister_req_t *p_in_cell_deregister_req_msg,U16 src_id)
 *  \brief This function is used to build and send son deregister req to cellm
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param p_in_cell_deregister_req_msg rrm_son_deregister_req_t
 *  \param src_id source identifier
 */
void build_and_send_son_cell_deregister_req_to_cellm
(
     /*SPR 17777 +-*/
   rrm_mif_context             *p_mif_context, 
   rrm_son_deregister_req_t *p_in_cell_deregister_req_msg,
   U16                     src_id);

/*! \fn void build_and_send_son_cell_set_attributes_req_to_cellm(rrm_mif_gb_context_t  *p_gl_ctxt,rrm_mif_context  *p_mif_context,rrm_son_set_attribute_value_req_t *p_in_cell_set_attributes_req_msg,U16 src_id)
 *  \brief This function is used to build and send set attribute req to cellm
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param p_in_cell_set_attributes_req_msg rrm_son_set_attribute_value_req_t
 *  \param src_id U16
 */
void build_and_send_son_cell_set_attributes_req_to_cellm
(
     /*SPR 17777 +-*/
   rrm_mif_context             *p_mif_context, 
   rrm_son_set_attribute_value_req_t *p_in_cell_set_attributes_req_msg,
   U16                     src_id);


/*! \fn void build_and_send_log_enable_disable_req(rrm_mif_gb_context_t *p_gl_ctxt,rrm_oam_log_enable_disable_req_t *p_in_log_en_dis_req_from_oam,U16 trans_id)
 *  \brief This function is used to build and send log enable disable req to cellm
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_in_log_en_dis_req_from_oam rrm_oam_log_enable_disable_req_t
 *  \param trans_id transaction identifier
 */
void build_and_send_log_enable_disable_req
(
     /*SPR 17777 +-*/
    rrm_oam_log_enable_disable_req_t *p_in_log_en_dis_req_from_oam,
    U16                        trans_id);

/*! \fn void build_and_send_init_config_request(rrm_mif_gb_context_t *p_gl_ctxt,rrm_oam_init_config_req_t *p_in_init_config_req_from_oam, U16 trans_id)
 *  \brief This function is used to build and send init config req to cellm
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_in_init_config_req_from_oam rrm_oam_init_config_req_t
 *  \param trans_id transaction identifier
 */
void build_and_send_init_config_request
(
   rrm_mif_gb_context_t      *p_gl_ctxt,
   rrm_oam_init_config_req_t *p_in_init_config_req_from_oam,
   U16                        trans_id);

/*! \fn void build_and_send_non_registration_based_resp_to_son(rrm_mif_gb_context_t *p_gl_ctxt,rrm_mif_context *p_mif_context,rrmcm_rmif_generic_resp_t *p_rmif_generic_resp,U16 apiId)
 *  \brief this function is used to build and send non registration based response to son
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param p_rmif_generic_resp rrmcm_rmif_generic_resp_t
 *  \param apiId api identifier
 */
void build_and_send_non_registration_based_resp_to_son
(
  rrm_mif_gb_context_t  *p_gl_ctxt,
  rrm_mif_context    *p_mif_context,
  rrmcm_rmif_generic_resp_t *p_rmif_generic_resp,
  U16        apiId);

/*! \fn void build_and_send_registration_based_resp_to_son(rrm_mif_gb_context_t *p_gl_ctxt,rrm_mif_context *p_mif_context,rrmcm_rmif_generic_resp_t *p_rmif_generic_resp,U16 api_id)
 *  \brief This function is used to build and send registration based response to son
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param p_rmif_generic_resp rrmcm_rmif_generic_resp_t
 *  \param api_id api identifier
 */
void build_and_send_registration_based_resp_to_son
(
   rrm_mif_gb_context_t  *p_gl_ctxt,
   rrm_mif_context              *p_mif_context, 
   rrmcm_rmif_generic_resp_t *p_rmif_generic_resp,
   U16                        api_id);

/*! \fn void build_and_send_config_resp_to_oam(rrm_mif_gb_context_t *p_gl_ctxt,rrm_mif_context *p_mif_context,rrmcm_rmif_cell_config_resp *p_in_resp_from_cellm,rrm_oam_cell_config_resp_t *p_ut_response_to_oam)
 *  \brief build and send config response to son
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param p_in_resp_from_cellm rrmcm_rmif_cell_config_resp
 *  \param p_ut_response_to_oam rrm_oam_cell_config_resp_t
 */
void build_and_send_config_resp_to_oam
(
    rrm_mif_gb_context_t             *p_gl_ctxt,
    rrm_mif_context                  *p_mif_context,
    rrmcm_rmif_cell_config_resp      *p_in_resp_from_cellm,
    rrm_oam_cell_config_resp_t       *p_ut_response_to_oam);


/*! \fn void build_and_send_reconfig_resp_to_oam(rrm_mif_gb_context_t *p_gl_ctxt,rrm_mif_context  *p_mif_context,rrmcm_rmif_generic_resp_t *p_in_resp_from_cellm,rrm_oam_cell_reconfig_resp_t *p_ut_response_to_oam)
 *  \brief build and send reconfig response to son
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param p_in_resp_from_cellm rrmcm_rmif_generic_resp_t
 *  \param p_ut_response_to_oam rrm_oam_cell_reconfig_resp_t
 */
void build_and_send_reconfig_resp_to_oam
(
   rrm_mif_gb_context_t         *p_gl_ctxt,
   rrm_mif_context              *p_mif_context,
   rrmcm_rmif_generic_resp_t    *p_in_resp_from_cellm,
   rrm_oam_cell_reconfig_resp_t *p_ut_response_to_oam);

/*! \fn void build_and_send_del_resp_to_oam(rrm_mif_gb_context_t *p_gl_ctxt,rrm_mif_context *p_mif_context,rrmcm_rmif_generic_resp_t *p_in_resp_from_cellm,rrm_oam_delete_resp_t *p_ut_response_to_oam)
 *  \brief This function is used to build and send delete response to son
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param p_in_resp_from_cellm rrmcm_rmif_generic_resp_t
 *  \param p_ut_response_to_oam rrm_oam_delete_resp_t
 */
void build_and_send_del_resp_to_oam
(
   rrm_mif_gb_context_t  *p_gl_ctxt,
   rrm_mif_context             *p_mif_context,
   rrmcm_rmif_generic_resp_t   *p_in_resp_from_cellm,
   rrm_oam_delete_resp_t            *p_ut_response_to_oam);

/*! \fn void build_and_send_start_resp_to_oam(rrm_mif_gb_context_t *p_gl_ctxt,rrm_mif_context *p_mif_context,rrmcm_rmif_generic_resp_t *p_in_resp_from_cellm,rrm_oam_cell_start_resp_t*p_ut_response_to_oam)
 *  \brief build and send start response to oam
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param p_in_resp_from_cellm rrmcm_rmif_generic_resp_t
 *  \param p_ut_response_to_oam rrm_oam_cell_start_resp_t
 */
void build_and_send_start_resp_to_oam
(
   rrm_mif_gb_context_t        *p_gl_ctxt,
   rrm_mif_context             *p_mif_context,
   rrmcm_rmif_generic_resp_t   *p_in_resp_from_cellm,
   rrm_oam_cell_start_resp_t   *p_ut_response_to_oam);

/*! \fn void build_and_send_stop_resp_to_oam(rrm_mif_gb_context_t *p_gl_ctxt,rrm_mif_context *p_mif_context,rrmcm_rmif_generic_resp_t *p_in_resp_from_cellm,rrm_oam_cell_stop_resp_t *p_out_response_to_oam)
 *  \brief This function is used to build and send stop delete response to son
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param p_in_resp_from_cellm rrmcm_rmif_generic_resp_t
 *  \param p_out_response_to_oam rrm_oam_cell_stop_resp_t
 */
void build_and_send_stop_resp_to_oam(
                                       rrm_mif_gb_context_t  *p_gl_ctxt,
                                       rrm_mif_context             *p_mif_context,
                                       rrmcm_rmif_generic_resp_t   *p_in_resp_from_cellm,
                                       rrm_oam_cell_stop_resp_t   *p_out_response_to_oam);

/*! \fn void build_and_send_update_resp_to_oam(rrm_mif_gb_context_t *p_gl_ctxt,rrm_mif_context *p_mif_context,rrmcm_rmif_generic_resp_t *p_in_resp_from_cellm,rrm_oam_cell_update_resp_t *p_out_response_to_oam)
 *  \brief This function is used to build and send update response to son
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param p_in_resp_from_cellm rrmcm_rmif_generic_resp_t
 *  \param p_out_response_to_oam rrm_oam_cell_update_resp_t
 */
void build_and_send_update_resp_to_oam
(
   rrm_mif_gb_context_t         *p_gl_ctxt,
   rrm_mif_context              *p_mif_context,
   rrmcm_rmif_generic_resp_t    *p_in_resp_from_cellm,
   rrm_oam_cell_update_resp_t   *p_out_response_to_oam);

/*! \fn void build_and_send_emerg_call_active_req_to_uem(rrm_mif_gb_context_t *p_gl_ctxt,rrm_mif_context *p_mif_context,rrmuem_rmif_emergency_call_active_req_t *p_ergency_call_active_req,U16 trans_id)
 *  \brief build and send emergency call active req to ue
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param p_ergency_call_active_req rrmuem_rmif_emergency_call_active_req_t
 *  \param trans_id trasaction id
 */
/*SPR 15993 Fix Start*/
void build_and_send_emerg_call_active_req_to_uem
(rrm_mif_gb_context_t  *p_gl_ctxt,
    rrm_mif_context             *p_mif_context,
 rrmuem_rmif_emergency_call_active_req_t *p_ergency_call_active_req,
 U16                        trans_id);
/*SPR 15993 Fix End*/

/*! \fn void build_and_send_active_ue_count_ind_to_son(rrm_mif_gb_context_t  *p_gl_ctxt,rrm_mif_context *p_mif_context,rrmcm_rmif_cell_active_ue_report_ind_t *p_cell_active_ue_report_ind,U16 api_id)
 *  \brief This function is used to send active UE count to SON
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param p_cell_active_ue_report_ind rrmcm_rmif_cell_active_ue_report_ind_t
 *  \param api_id api identifier
 */
void build_and_send_active_ue_count_ind_to_son(
                                       rrm_mif_gb_context_t  *p_gl_ctxt,
                                       rrm_mif_context             *p_mif_context,
                                       rrmcm_rmif_cell_active_ue_report_ind_t 
                                             *p_cell_active_ue_report_ind,
                                       U16    api_id);

/*! \fn void build_and_send_active_threashold_reached_indto_son(rrm_mif_gb_context_t *p_gl_ctxt,rrm_mif_context *p_mif_context,rrm_ue_count_threshold_reached_ind_t *p_ue_cnt_threshold_ind,U16 api_id)
 *  \brief This function is used to send active threshold indication to SON
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param p_ue_cnt_threshold_ind rrm_ue_count_threshold_reached_ind_t
 *  \param api_id api id
 */
void build_and_send_active_threashold_reached_indto_son(
                                       rrm_mif_gb_context_t  *p_gl_ctxt,
                                       rrm_mif_context             *p_mif_context,
                                       rrm_ue_count_threshold_reached_ind_t          
                                               *p_ue_cnt_threshold_ind,
                                       U16    api_id);

/*! \fn void build_and_send_cell_state_change_ind_to_son(rrm_mif_gb_context_t  *p_gl_ctxt,rrm_mif_context *p_mif_context,rrmcm_rmif_cell_state_change_ind_t *p_cell_state_change_ind,U16 api_id)
 *  \brief This function is used to build and send cell state change indication to son
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param p_cell_state_change_ind rrmcm_rmif_cell_state_change_ind_t
 *  \param api_id api id
 */
void build_and_send_cell_state_change_ind_to_son(
                                       rrm_mif_gb_context_t  *p_gl_ctxt,
                                       rrm_mif_context             *p_mif_context,
                                       rrmcm_rmif_cell_state_change_ind_t
                                                  *p_cell_state_change_ind,
                                       U16    api_id);



/*! \fn void fill_cell_plmn_info(rrm_cell_plmn_info_t *cellPlmnInfoToCellM_p,rrm_oam_cell_plmn_info_t *cellPlmnInfoFromOam_p)
 *  \brief This function Fill Cell PLMN Information.
 *  \param cellPlmnInfoToCellM_p rrm_cell_plmn_info_t
 *  \param cellPlmnInfoFromOam_p  rrm_oam_cell_plmn_info_t
 */
void fill_cell_plmn_info(rrm_cell_plmn_info_t     *cellPlmnInfoToCellM_p,
                         rrm_oam_cell_plmn_info_t *cellPlmnInfoFromOam_p);

/* SPR 20653 Fix Start */
/*! \fn void fill_phy_layer_info_for_cell_config(rrm_physical_layer_params_t *p_phy_info_to_cellm ,rrm_oam_physical_layer_params_t *p_phy_info_from_oam) 
 *  \brief This function fills Physical Layer Information 
 *  \param p_phy_info_to_cellm rrm_physical_layer_params_t
 *  \param p_operator_info_from_oam rrm_oam_operator_info_t 
 */
void fill_phy_layer_info_for_cell_config
                        (rrm_physical_layer_params_t       *p_phy_info_to_cellm,
                         rrm_oam_physical_layer_params_t   *p_phy_info_from_oam);
                         /*SPR 17777 +-*/

/*! \fn  void fill_phy_layer_info_for_cell_reconfig(rrm_physical_layer_params_t *p_phy_info_to_cellm,rrm_oam_physical_layer_params_t *p_phy_info_from_oam)
 *  \brief This function fills Physical Layer Information 
 *  \param p_phy_info_to_cellm rrm_physical_layer_params_t
 *  \param p_operator_info_from_oam rrm_oam_operator_info_t
 */
void fill_phy_layer_info_for_cell_reconfig(rrm_physical_layer_params_t       *p_phy_info_to_cellm,
                         rrm_oam_physical_layer_params_t   *p_phy_info_from_oam);
                        /*SPR 17777 +-*/
/* SPR 20653 Fix End */

/* BUG_371 FIX START */ 
/*! \fn void fill_mac_layer_info_for_cell_config(mac_layer_params_t *macInfoToCellM_p,rrm_oam_mac_layer_params_t  *macInfoFromOam_p) 
 *  \brief This function is to fill MAC Layer Information.
 *  \param macInfoToCellM_p  mac_layer_params_t
 */
void fill_mac_layer_info_for_cell_config(mac_layer_params_t          *macInfoToCellM_p,
                         rrm_oam_mac_layer_params_t  *macInfoFromOam_p);
                         /*SPR 17777 +-*/

/*! \fn void fill_mac_layer_info_for_cell_reconfig(mac_layer_params_t *macInfoToCellM_p,rrm_oam_mac_layer_params_t *macInfoFromOam_p)
 *  \brief this function is to fill MAC Layer Information.
 *  \param macInfoToCellM_p mac_layer_params_t
 *  \param macInfoFromOam_p rrm_oam_mac_layer_params_t
 */
void fill_mac_layer_info_for_cell_reconfig(mac_layer_params_t          *macInfoToCellM_p,
                         rrm_oam_mac_layer_params_t  *macInfoFromOam_p
                         );
/* BUG_371 FIX END */ 

/*! \fn void fill_rlc_layer_info(rlc_layer_params_t *rlcInfoToCellM_p,rrm_oam_rlc_layer_params_t *rlcInfoFromOam_p)
 *  \brief This funtcion is to fill RLC Layer Information.
 *  \param rlcInfoToCellM_p rlc_layer_params_t
 *  \param rlcInfoFromOam_p rrm_oam_rlc_layer_params_t
 */
void fill_rlc_layer_info(/*SPR 17777 +-*/
                         rlc_layer_params_t          *rlcInfoToCellM_p,
                         rrm_oam_rlc_layer_params_t  *rlcInfoFromOam_p);

/*! \fn void fill_cell_restriction_info(cell_restriction_params_t *p_cell_restrict_info_to_cellm,rrm_oam_cell_access_restriction_params_t *p_cell_restrict_info_from_oam,rrm_oam_epc_t *p_epc_info_from_oam)
 *  \brief This function is to fill Cell Restriction Information.
 *  \param p_cell_restrict_info_to_cellm cell_restriction_params_t
 *  \param p_cell_restrict_info_from_oam rrm_oam_cell_access_restriction_params_t
 *  \param p_epc_info_from_oam rrm_oam_epc_t
 */
void fill_cell_restriction_info(cell_restriction_params_t                *p_cell_restrict_info_to_cellm,
                                rrm_oam_cell_access_restriction_params_t *p_cell_restrict_info_from_oam,
                                rrm_oam_epc_t                            *p_epc_info_from_oam);


/*! \fn void fill_mobility_info_for_cell_config(mobility_params_t  *mobInfoToCellM_p,rrm_oam_mobility_params_t *mobInfoFromOam_p) 
 *  \brief this function is used to fill Mobility Information.
 *  \param mobInfoToCellM_p mobility_params_t
 *  \param mobInfoFromOam_p rrm_oam_mobility_params_t
 */
void fill_mobility_info_for_cell_config(mobility_params_t          *mobInfoToCellM_p, 
 /* CSR_00057814_CHANGES_START */
                        rrm_oam_mobility_params_t  *mobInfoFromOam_p,
                        rrm_oam_ran_t               *p_ran_info_from_oam);
 /* CSR_00057814_CHANGES_END */


/*! \fn void fill_mobility_info_for_cell_reconfig(mobility_params_t *mobInfoToCellM_p,rrm_oam_mobility_params_t *mobInfoFromOam_p)
 *  \brief This function is to fill Mobility Information.
 *  \param mobInfoToCellM_p mobility_params_t
 *  \param mobInfoFromOam_p rrm_oam_mobility_params_t
 */
void fill_mobility_info_for_cell_reconfig(mobility_params_t          *mobInfoToCellM_p, 
 /* CSR_00057814_CHANGES_START */
                        rrm_oam_mobility_params_t  *mobInfoFromOam_p,
                        rrm_oam_ran_t               *p_ran_info_from_oam);
 /* CSR_00057814_CHANGES_END */


/*! \fn void fill_rrc_timer_constant_info(rrc_timers_and_constants_t *timerInfoToCellM_p,rrm_oam_rrc_timers_and_constants_t *timerInfoFromOam_p)
 *  \brief This funtcion is to fill RRC Timer_ConstantInformation.
 *  \param timerInfoToCellM_p rrc_timers_and_constants_t
 *  \param timerInfoFromOam_p rrm_oam_rrc_timers_and_constants_t
 */
void fill_rrc_timer_constant_info(rrc_timers_and_constants_t         *timerInfoToCellM_p, 
                                  rrm_oam_rrc_timers_and_constants_t *timerInfoFromOam_p);

/* SPR 20653 Fix Start */
/*! \fn void fill_rf_info(rf_params_t *rfInfoToCellM_p,rrm_oam_rf_params_t *rfInfoFromOam_p)
 *  \brief This function Fill RF Information.
 *  \param rfInfoToCellM_p rrc_timers_and_constants_t
 *  \param rfInfoFromOam_p rrm_oam_rrc_timers_and_constants_t
 */
void fill_rf_info(rf_params_t         *rfInfoToCellM_p,
                  rrm_oam_rf_params_t *rfInfoFromOam_p
                  );
/* SPR 20653 Fix End */

/*! \fn void fill_ncl_info(ncl_params_t *nclInfoToCellM_p,rrm_oam_ncl_params_t *nclInfoFromOam_p)
 *  \brief this function is to Fill NCL Information.
 *  \param nclInfoToCellM_p ncl_params_t
 *  \param nclInfoFromOam_p rrm_oam_ncl_params_t
 */
/* SPR 5193: Added p_ncl_cfg_params to get information from cfg file */
void fill_ncl_info(/*SPR 17777 +-*/
                   ncl_params_t               *nclInfoToCellM_p,
                   operator_info_t            *p_operator_info_to_cellm,
                   rrm_oam_ncl_params_t       *nclInfoFromOam_p);

/* SPR 20653 Fix Start */
/*! \fn void fill_ran_info_for_cell_config(ran_t *p_ran_info_to_cellm,rrm_oam_ran_t *p_ran_info_from_oam,rrm_oam_cell_access_restriction_params_t *p_car_params_from_oam,rrm_oam_eutran_global_cell_id_t *p_cell_id_from_oam,rrm_oam_epc_t *p_epc_info_from_oam) 
 *  \brief  this function is to Fill RAN Information.
 *  \param  p_ran_info_to_cellm ran_t
 *  \param  p_ran_info_from_oam rrm_oam_ran_t
 *  \param  p_car_params_from_oam rrm_oam_cell_access_restriction_params_t
 *  \param  p_cell_id_from_oam rrm_oam_eutran_global_cell_id_t
 *  \param  p_epc_info_from_oam rrm_oam_epc_t
 *  \param  p_operator_info_from_oam rrm_oam_operator_info
 */
void fill_ran_info_for_cell_config(ran_t                       *p_ran_info_to_cellm,
                   operator_info_t             *p_operator_info_to_cellm,
                   rrm_oam_ran_t               *p_ran_info_from_oam,
                   rrm_oam_cell_access_restriction_params_t *p_car_params_from_oam,
                   rrm_oam_eutran_global_cell_id_t    *p_cell_id_from_oam,
                   /*SPR 17777 +-*/
                   rrm_oam_operator_info_t      *p_operator_info_from_oam);

/*! \fn fill_ran_info_for_cell_reconfig(ran_t *p_ran_info_to_cellm,rrm_oam_ran_t *p_ran_info_from_oam,rrm_oam_cell_access_restriction_params_t *p_car_params_from_oam,rrm_oam_eutran_global_cell_id_t *p_cell_id_from_oam,rrm_oam_epc_t *p_epc_info_from_oam) 
 *  \brief This funtcion id to Fill RAN Information for cell reconfig
 *  \param  p_ran_info_to_cellm ran_t
 *  \param  p_ran_info_from_oam rrm_oam_ran_t
 *  \param  p_car_params_from_oam rrm_oam_cell_access_restriction_params_t
 *  \param  p_cell_id_from_oam rrm_oam_eutran_global_cell_id_t
 *  \param  p_epc_info_from_oam rrm_oam_epc_t
 *  \param  p_operator_info_from_oam rrm_oam_operator_info_t
 */
void fill_ran_info_for_cell_reconfig(ran_t                       *p_ran_info_to_cellm,
                   operator_info_t             *p_operator_info_to_cellm,
                   rrm_oam_ran_t               *p_ran_info_from_oam,
                   rrm_oam_eutran_global_cell_id_t    *p_cell_id_from_oam
                   /*SPR 17777 +-*/
/*BUG_866_CHANGES_START*/
                   //rrm_oam_epc_t                *p_epc_info_from_oam,
/*BUG_866_CHANGES_END*/
        );
     /*SPR 17777 +-*/

/*! \fn void fill_operator_info_for_cell_config(operator_info_t *p_operator_info_to_cellm,rrm_oam_operator_info_t *p_operator_info_from_oam,rrm_oam_cell_access_restriction_params_t *p_car_params_from_oam,rrm_oam_rf_params_t *p_rf_params_from_oam,rrm_oam_physical_layer_params_t *p_plp_params_from_oam)
 *  \brief This funtcion is used to Fill Operator Information.
 *  \param p_operator_info_to_cellm operator_info_t
 *  \param p_operator_info_from_oam rrm_oam_operator_info_t
 *  \param p_car_params_from_oam rrm_oam_cell_access_restriction_params_t
 *  \param p_rf_params_from_oam rrm_oam_rf_params_t
 *  \param p_plp_params_from_oam rrm_oam_physical_layer_params_t
 */
void fill_operator_info_for_cell_config(operator_info_t            *p_operator_info_to_cellm,
    rrm_oam_operator_info_t    *p_operator_info_from_oam,
     /*SPR 17777 +-*/
    rrm_oam_rf_params_t    *p_rf_params_from_oam,
    rrm_oam_physical_layer_params_t    *p_plp_params_from_oam,
/* SPR 18641_18843 fix start */
    rrm_oam_mac_layer_params_t      *p_mac_layer_params_from_oam,
    U8 tti_bundling_info_present);
/* SPR 18641_18843 fix end */

/*! \fn void fill_operator_info_for_cell_reconfig(operator_info_t *p_operator_info_to_cellm,rrm_oam_operator_info_t *p_operator_info_from_oam,rrm_oam_cell_access_restriction_params_t *p_car_params_from_oam,rrm_oam_rf_params_t *p_rf_params_from_oam,rrm_oam_physical_layer_params_t *p_plp_params_from_oam)
 *  \brief This function is to Fill Operator Information for cell reconfig
 *  \param p_operator_info_to_cellm operator_info_t
 *  \param p_operator_info_from_oam rrm_oam_operator_info_t
 *  \param p_car_params_from_oam rrm_oam_cell_access_restriction_params_t
 *  \param p_rf_params_from_oam rrm_oam_rf_params_t
 *  \param p_plp_params_from_oam rrm_oam_physical_layer_params_t
 */
void fill_operator_info_for_cell_reconfig(operator_info_t            *p_operator_info_to_cellm,
    rrm_oam_operator_info_t    *p_operator_info_from_oam,
     /*SPR 17777 +-*/
    rrm_oam_rf_params_t    *p_rf_params_from_oam,
    rrm_oam_physical_layer_params_t    *p_plp_params_from_oam,
/* SPR 18641_18843 fix start */
    rrm_oam_mac_layer_params_t      *p_mac_layer_params_from_oam,
    U8 tti_bundling_info_present);
/* SPR 18641_18843 fix end */

/*! \fn void fill_epc_info_for_cell_config(epc_t *p_epc_info_to_cellm,rrm_oam_epc_t *p_epc_info_from_oam,rrm_oam_ran_t *p_ran_info_from_oam) 
 *  \brief This funtcion is used to Fill EPC Information.
 *  \param  p_epc_info_to_cellm epc_t
 *  \param  p_epc_info_from_oam rrm_oam_epc_t
 *  \param  p_ran_info_from_oam rrm_oam_ran_t
 */
void fill_epc_info_for_cell_config(epc_t                        *p_epc_info_to_cellm,
    rrm_oam_epc_t                *p_epc_info_from_oam,
/* SPR 18641_18843 fix start */
    rrm_oam_ran_t                *p_ran_info_from_oam,
    U8                           *p_tti_bundling_info_present);
/* SPR 18641_18843 fix end */
/*! \fn void fill_epc_info_for_cell_reconfig(epc_t *p_epc_info_to_cellm,rrm_oam_epc_t *p_epc_info_from_oam,rrm_oam_ran_t *p_ran_info_from_oam) 
 *  \brief this function is used to Fill EPC Information.
 *  \param  p_epc_info_to_cellm epc_t
 *  \param  p_epc_info_from_oam rrm_oam_epc_t
 *  \param  p_ran_info_from_oam rrm_oam_ran_t
 */
void 
fill_epc_info_for_cell_reconfig
(
    epc_t                        *p_epc_info_to_cellm,
    rrm_oam_epc_t                *p_epc_info_from_oam,
/* SPR 18641_18843 fix start */
    rrm_oam_ran_t                *p_ran_info_from_oam,
    U8                           *p_tti_bundling_info_present);
/* SPR 18641_18843 fix end */

/*! \fn void fill_cell_config_request_for_cellm(rrmcm_rmif_cell_config_req_t *OutCellConfigReqToCellM_p,rrm_oam_cell_config_req_t *InCellConfigReqFromOam_p)
 *  \brief This function is used to Fill Cell Config Info
 *  \param OutCellConfigReqToCellM_p  rrmcm_rmif_cell_config_req_t
 *  \param InCellConfigReqFromOam_p rrm_oam_cell_config_req_t
 */
void fill_cell_config_request_for_cellm(rrmcm_rmif_cell_config_req_t   *OutCellConfigReqToCellM_p,
                                        rrm_oam_cell_config_req_t      *InCellConfigReqFromOam_p);

/*! \fn void fill_cell_reconfig_request_for_cellm(rrmcm_rmif_cell_reconfig_req_t *p_cell_reconf_req_to_cellm, rrm_oam_cell_reconfig_req_t *p_cell_reconf_req_from_oam)
 *  \brief This function is to Fill Cell Reconf Information.
 *  \param p_cell_reconf_req_to_cellm rrmcm_rmif_cell_reconfig_req_t
 *  \param p_cell_reconf_req_from_oam rrm_oam_cell_reconfig_req_t
*/
void fill_cell_reconfig_request_for_cellm(rrmcm_rmif_cell_reconfig_req_t   *p_cell_reconf_req_to_cellm,
                                          rrm_oam_cell_reconfig_req_t      *p_cell_reconf_req_from_oam);
/* SPR 20653 Fix End */

/*! \fn void build_and_send_set_log_level_req(rrm_mif_gb_context_t *p_gl_ctxt,rrm_oam_set_log_level_req_t *p_in_log_en_dis_req_from_oam,U16 trans_id) 
 *  \brief This function is used to build and send set log level req to cellm
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_in_log_en_dis_req_from_oam rrm_oam_set_log_level_req_t
 *  \param trans_id U16
 */

void build_and_send_set_log_level_req(/*SPR 17777 +-*/
                                    rrm_oam_set_log_level_req_t 
                   *p_in_log_en_dis_req_from_oam,
                                     U16                        trans_id);
/*! \fn void fill_cell_update_request_for_cellm(rrmcm_rmif_cell_update_req_t *p_cell_update_req_to_cellm,rrm_oam_cell_update_req_t *p_cell_update_req_from_oam)
 *  \brief this function is used to Fill Cell Update Information.
 *  \param p_cell_update_req_to_cellm rrmcm_rmif_cell_update_req_t
 *  \param p_cell_update_req_from_oam rrm_oam_cell_update_req_t
*/
void fill_cell_update_request_for_cellm(rrmcm_rmif_cell_update_req_t   *p_cell_update_req_to_cellm,
                                          rrm_oam_cell_update_req_t      *p_cell_update_req_from_oam);

/*! \fn void rrm_mif_log_enable_disable_cmd(rrm_mif_gb_context_t *p_gl_ctxt,rrm_oam_log_on_off_et log_on_off,rrm_oam_log_level_et log_level)
 *  \brief this funtcion is used for mif log enable disable
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param log_on_off rrm_oam_log_on_off_et
 *  \param log_level rrm_oam_log_level_et
*/

void rrm_mif_log_enable_disable_cmd (
                                rrm_mif_gb_context_t      *p_gl_ctxt,
                                rrm_oam_log_on_off_et log_on_off,
                                rrm_oam_log_level_et log_level);

/*! \fn void build_and_send_rac_enable_disable_req_to_cellm(rrm_mif_gb_context_t *p_gl_ctxt,rrm_mif_context *p_mif_context,rrm_oam_rac_enable_disable_req_t *p_rac_enable_disable_req_from_oam,rrmcm_rmif_rac_enable_disable_req_t *p_rac_enable_disable_req_to_cellm)
 *  \brief This function prepares and sends RAC enable/disable request to UEM
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param p_rac_enable_disable_req_from_oam rrm_oam_rac_enable_disable_req_t
 *  \param p_rac_enable_disable_req_to_cellm rrmcm_rmif_rac_enable_disable_req_t
 */
void build_and_send_rac_enable_disable_req_to_cellm
(
     /*SPR 17777 +-*/
    rrm_mif_context                      *p_mif_context,
    rrm_oam_rac_enable_disable_req_t     *p_rac_enable_disable_req_from_oam,
    rrmcm_rmif_rac_enable_disable_req_t  *p_rac_enable_disable_req_to_cellm
);

/*! \fn void build_and_send_rac_enable_disable_resp_to_oam(rrm_mif_gb_context_t *p_gl_ctxt,rrm_mif_context *p_mif_context,rrmcm_rmif_rac_enable_disable_res_t *p_in_rac_enable_disable_resp_from_cellm,rrm_oam_rac_enable_disable_resp_t *p_out_rac_enable_disable_resp_to_oam)
 *  \brief This function prepares and sends RAC enable/disable response to OAMH
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param p_in_rac_enable_disable_resp_from_cellm rrmcm_rmif_rac_enable_disable_res_t
 *  \param p_out_rac_enable_disable_resp_to_oam rrm_oam_rac_enable_disable_resp_t
 */
void build_and_send_rac_enable_disable_resp_to_oam
(
    rrm_mif_gb_context_t                 *p_gl_ctxt,
    rrm_mif_context                      *p_mif_context,
    rrmcm_rmif_rac_enable_disable_res_t  *p_in_rac_enable_disable_resp_from_cellm,
    rrm_oam_rac_enable_disable_resp_t    *p_out_rac_enable_disable_resp_to_oam
);





/*! \fn void build_and_send_init_ind_to_oam(void )
 *  \brief  This funtcion prepares and sends an INIT_IND to OAM
 */
void build_and_send_init_ind_to_oam(void);

/*! \fn void fill_antenna_information(U8 snr_index,rrc_phy_antenna_information_ue_service_profile_t *p_antenna_information)
 *  \brief This funtcion fills in the antenna information from config
 *  \param  snr_index U8
 *  \param  p_antenna_information rrc_phy_antenna_information_ue_service_profile_t
 */
/* SPR 20653 Fix Start */
void fill_antenna_information(rrc_phy_antenna_information_ue_service_profile_t *p_antenna_information);
/* SPR 20653 Fix End */

/*! \fn void build_and_send_context_print_req(U8 cell_index)
 *  \brief This function is used to build and send context print request 
 *  \param cell_index cell index
 */
void build_and_send_context_print_req(U8 cell_index);

/*! \fn rrm_error_et rrm_mif_send_event_to_all_cells(U16 event_id,U8 *p_msg_info,rrm_mif_gb_context_t *p_gl_ctxt,U16 transaction_id,U16 source)
 *  \brief This function is used to send event to all cells
 *  \param event_id event identifier
 *  \param p_msg_info pointer to msg information
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param transaction_id transaction id
 *  \param source src id
 */
rrm_error_et rrm_mif_send_event_to_all_cells
     (
      U16                   event_id,
      U8                   *p_msg_info,
      rrm_mif_gb_context_t *p_gl_ctxt,
      U16                   transaction_id,
      U16                   source
      );

/* UE MEAS CHANGES : STARTS */
/*! \fn void build_and_send_son_ue_meas_config_req_to_cellm(rrm_mif_gb_context_t *p_gl_ctxt,rrm_mif_context *p_mif_context,rrm_son_meas_config_req_t *p_in_meas_config_req_msg,U16 src_id) 
 *  \brief  This function is used to build and send son meas config req to cellm
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param p_in_meas_config_req_msg rrm_son_meas_config_req_t
 *  \param src_id U16
 */
void build_and_send_son_ue_meas_config_req_to_cellm 
(
     /*SPR 17777 +-*/
    rrm_mif_context             *p_mif_context, 
    rrm_son_meas_config_req_t   *p_in_meas_config_req_msg
     /*SPR 17777 +-*/
);
/* UE MEAS CHANGES : ENDS */

/* UPDATED NRT INFO CHANGES START */ 

/*! \fn void build_and_send_son_updated_nrt_info_to_cellm(rrm_mif_gb_context_t  *p_gl_ctxt,rrm_mif_context *p_mif_context,son_anr_rrm_updated_nrt_info_t  *p_in_updated_nrt_info_req_msg,U16 src_id)
 *  \brief This function is to build and send son updated nrt info to cellM
 *  \param p_gl_ctxt  rrm_mif_gb_context_t
 *  \param p_mif_context  rrm_mif_context
 *  \param p_in_updated_nrt_info_req_msg  son_anr_rrm_updated_nrt_info_t 
 *  \param src_id  U16
 */
void build_and_send_son_updated_nrt_info_to_cellm
(
     /*SPR 17777 +-*/
    rrm_mif_context             *p_mif_context, 
    son_anr_rrm_updated_nrt_info_t *p_in_updated_nrt_info_req_msg,
    U16                     src_id);

/* UPDATED NRT INFO CHANGES END */ 
/*! \fn void build_and_send_ue_release_req(rrm_ue_index_t ue_index , U16 transaction_id)
 *  \brief This function is used to build and send UE release request 
 *  \param ue_index rrm_ue_index_t
 *  \param transaction_id transaction id
 *  \param cell_index  cell index
 */
void build_and_send_ue_release_req
(
    rrm_ue_index_t ue_index, 
     /*SPR 17777 +-*/
	/*SPR_11336_FIX_START */
	U8 cell_index
	/*SPR_11336_FIX_END */
);

/*! \fn void build_and_send_release_all_existing_ue_forcefully_req(rrm_cell_index_t cell_index,U16 trans_id)
 *  \brief This function is to build and send all existing ue release forcefully req to UEM
 *  \param cell_index cell index
 *  \param trans_id transaction identifier
 */
void build_and_send_release_all_existing_ue_forcefully_req 
(   rrm_cell_index_t cell_index,
    U16  trans_id
);

/*! \fn void build_and_send_non_emrgncy_active_calls_ho_req(rrm_cell_index_t cell_index,U16 trans_id)
 *  \brief This function builds and sends non emrgncy active calls ho request to UEM
 *  \param cell_index cell index
 *  \param trans_id transaction identifier
 */

void build_and_send_non_emrgncy_active_calls_ho_req
(   rrm_cell_index_t cell_index,
    U16  trans_id
);
/*! \fn void build_and_send_son_tnl_discovery_to_cellm(U16 transaction_id,rrm_son_tnl_discovery_req_t *p_in_tnl_discovery_req_msg,U16 src)
 *  \brief This function is used to build and send son tnl discovery to cell mgr
 *  \param transaction_id transaction identifier
 *  \param p_in_tnl_discovery_req_msg tnl discovery request message
 *  \param src source
 */
void build_and_send_son_tnl_discovery_to_cellm
(
 U16                         transaction_id,
 rrm_son_tnl_discovery_req_t *p_in_tnl_discovery_req_msg
     /*SPR 17777 +-*/
 );

/*! \fn void build_and_send_son_ue_meas_config_obj_remove_req_to_cellm(rrm_mif_gb_context_t *p_gl_ctxt,rrm_mif_context            *p_mif_context, rrm_son_meas_config_obj_remove_req_t  *p_in_meas_config_obj_remove_req_msg,U16 src)
 *  \brief This function is used to build and send son meas obj remove req to cell mgr
 *  \param p_gl_ctxt    mif global context
 *  \param p_mif_context  mif context
 *  \param p_in_meas_config_obj_remove_req_msg meas obj remove request message
 *  \param src source
 */
void build_and_send_son_ue_meas_config_obj_remove_req_to_cellm
(
     /*SPR 17777 +-*/
    rrm_mif_context            *p_mif_context,
    rrm_son_meas_config_obj_remove_req_t  *p_in_meas_config_obj_remove_req_msg
     /*SPR 17777 +-*/
);


/* SPS related changes start */
/*! \fn void populate_sps_config_data(rrm_oam_sps_config_data_t *sps_config_data_from_oam,rrm_sps_config_data_t *sps_config_data_to_cellm,U8 qci_val,S8 p_0_nominal_pusch)
 *  \brief   This function populates sps_config data for CellM
 *  \param   sps_config_data_from_oam rrm_oam_sps_config_data_t
 *  \param   sps_config_data_to_cellm rrm_sps_config_data_t
 *  \param   qci_val U8
 *  \param   p_0_nominal_pusch S8
 */

void populate_sps_config_data(
        rrm_oam_sps_config_data_t     *sps_config_data_from_oam,
        rrm_sps_config_data_t         *sps_config_data_to_cellm,
        U8                             qci_val,
        S8                             p_0_nominal_pusch
        );
/* SPS related changes end */
/*! \fn rrm_void_t get_spcl_dwpts_length_from_spcl_sf_pattern( rrm_oam_special_sub_frame_patterns_et spcl_sf_pattrn,U16 *spcl_dwpts_length)
 *  \brief This function is used is used to get spcl dwpts length from spcl sf pattern
 *  \param spcl_sf_pattrn rrm_oam_special_sub_frame_patterns_et
 *  \param spcl_dwpts_length U16
 */
rrm_void_t
get_spcl_dwpts_length_from_spcl_sf_pattern
(
  rrm_oam_special_sub_frame_patterns_et spcl_sf_pattrn,
  U16 *spcl_dwpts_length
);
/*! \fn rrm_void_t get_num_of_rb_frm_chanl_bw(rrm_oam_band_width_et channel_bandwith,U8 *num_of_avail_rb)
 *  \brief This function is used to get number of rb frm channel
 *  \param channel_bandwith rrm_oam_band_width_et
 *  \param num_of_avail_rb U8
 */
rrm_void_t
get_num_of_rb_frm_chanl_bw
( 
 rrm_oam_band_width_et channel_bandwith , 
 U8 *num_of_avail_rb
);



/*! \fn rrm_void_t fill_ncl_info_cdma2000(ncl_params_t *p_ncl_info_to_cellm,rrm_oam_ncl_params_t *p_ncl_info_from_oam)
*   \brief This function fill the cdma2000 NCL Info for CellM module
*   \param p_ncl_info_to_cellm ncl_params_t
*   \param p_ncl_info_from_oam rrm_oam_ncl_params_t
*/
rrm_void_t
fill_ncl_info_cdma2000(ncl_params_t          *p_ncl_info_to_cellm,
                       rrm_oam_ncl_params_t  *p_ncl_info_from_oam);


 /*! \fn rrm_void_t fill_cdma2000_band_class_params(U8 *p_cellm_cdma2000_band_class,U8 *p_oam_cdma2000_band_class)
 *   \brief This function fills the cdma2000 band class Info for CellM module
     \param p_cellm_cdma2000_band_class
     \param p_oam_cdma2000_band_class
 */                       
rrm_void_t
fill_cdma2000_band_class_params(U8 *p_cellm_cdma2000_band_class,
                                U8 *p_oam_cdma2000_band_class);


/*! \fn rrm_void_t fill_cdma2000_cell_specific_params(rrmcm_rmif_cell_specific_params_t *p_cellm_cdma2000_cell_spec_param,rrm_oam_cell_specific_params_t   *p_oam_cdma2000_cell_spec_param)
*   \brief This function fill the cdma2000 NCL Info for CellM module
*   \param p_cellm_cdma2000_cell_spec_param
*   \param p_oam_cdma2000_cell_spec_param
*/
rrm_void_t
fill_cdma2000_cell_specific_params(
     rrmcm_rmif_cell_specific_params_t  *p_cellm_cdma2000_cell_spec_param,
     rrm_oam_cell_specific_params_t   *p_oam_cdma2000_cell_spec_param);


/*! \fn rrm_void_t   fill_cdma2000_fpc_fch_params(rrmcm_rmif_fpc_fch_included_t *p_cellm_fpc_fch_params,rrm_oam_fpc_fch_included_t     *p_oam_fpc_fch_params)
*   \brief This function fill the cdma2000 fpc fch Info for CellM module
*   \param p_cellm_fpc_fch_params rrmcm_rmif_fpc_fch_included_t
*   \param p_oam_fpc_fch_params rrm_oam_fpc_fch_included_t
*/                           
rrm_void_t
fill_cdma2000_fpc_fch_params(
     rrmcm_rmif_fpc_fch_included_t  *p_cellm_fpc_fch_params,
     rrm_oam_fpc_fch_included_t     *p_oam_fpc_fch_params);


/*! \fn rrm_void_t fill_cdma2000_pz_hyst_params(rrmcm_rmif_pz_hyst_parameters_included_t  *p_cellm_pz_hyst_params,rrm_oam_pz_hyst_parameters_included_t *p_oam_pz_hyst_params)
*   \brief  This function fill the cdma2000 PZ HYST Info for CellM module   
*   \param  p_cellm_pz_hyst_params rrmcm_rmif_pz_hyst_parameters_included_t
*   \param  p_oam_pz_hyst_params rrm_oam_pz_hyst_parameters_included_t
*/
rrm_void_t
fill_cdma2000_pz_hyst_params(
     rrmcm_rmif_pz_hyst_parameters_included_t  *p_cellm_pz_hyst_params,
     rrm_oam_pz_hyst_parameters_included_t *p_oam_pz_hyst_params);



/*! \fn rrm_void_t fill_irat_cdma2000_ac_barring_info(rrmcm_rmif_ac_barring_config_1_xrtt_r9_t *p_ac_barring_info_to_cellm,rrm_oam_ac_barring_config_1_xrtt_r9_t  *p_ac_barring_info_from_oam)
*   \brief This function fill the cdma2000 idle mode mobility params for 
           CellM module
*   \param p_ac_barring_info_to_cellm rrmcm_rmif_ac_barring_config_1_xrtt_r9_t
*   \param p_ac_barring_info_from_oam rrm_oam_ac_barring_config_1_xrtt_r9_t
*/       
rrm_void_t
fill_irat_cdma2000_ac_barring_info(
     rrmcm_rmif_ac_barring_config_1_xrtt_r9_t  *p_ac_barring_info_to_cellm,
     rrm_oam_ac_barring_config_1_xrtt_r9_t  *p_ac_barring_info_from_oam);


/*! \fn rrm_void_t fill_irat_cdma2000_speed_scale_factors(speed_scale_factors_t *p_sf_info_to_cellm,rrm_oam_speed_scale_factors_t *p_sf_info_from_oam)
*   \brief  This function fill the cdma2000 speed scale factors params for CellM
            Module
*   \param p_sf_info_to_cellm speed_scale_factors_t
*   \param p_sf_info_from_oam rrm_oam_speed_scale_factors_t
*/                           
rrm_void_t
fill_irat_cdma2000_speed_scale_factors(
     speed_scale_factors_t  *p_sf_info_to_cellm,
     rrm_oam_speed_scale_factors_t  *p_sf_info_from_oam);

/*! \fn  rrm_void_t  fill_irat_cdma2000_pre_reg_hrpd_info(rrmcm_rmif_pre_reg_info_hrpd_t  *p_pre_reg_hrpd_info_to_cellm,rrm_oam_pre_reg_info_hrpd_t  *p_pre_reg_hrpd_info_from_oam)
*   \brief  This function fill the cdma2000 hrpd pre-registration info
            params for CellM module
*   \param  p_pre_reg_hrpd_info_to_cellm rrmcm_rmif_pre_reg_info_hrpd_t
*   \param  p_pre_reg_hrpd_info_from_oam rrm_oam_pre_reg_info_hrpd_t
*   \returns  void
*/
rrm_void_t
fill_irat_cdma2000_pre_reg_hrpd_info(
        rrmcm_rmif_pre_reg_info_hrpd_t  *p_pre_reg_hrpd_info_to_cellm,
        rrm_oam_pre_reg_info_hrpd_t  *p_pre_reg_hrpd_info_from_oam);

/*! \fn rrm_void_t fill_irat_cdma2000_sib8_mobility_info(rrmcm_rmif_mobility_sib_8_params_t *p_sib8_mob_info_to_cellm,rrm_oam_mobility_sib_8_params_t  *p_sib8_mob_info_from_oam)
*   \brief This function fill the cdma2000 sib8 mobility params for CellM module
*   \param p_sib8_mob_info_to_cellm rrmcm_rmif_mobility_sib_8_params_t
*   \param p_sib8_mob_info_from_oam rrm_oam_mobility_sib_8_params_t
 */
rrm_void_t
fill_irat_cdma2000_sib8_mobility_info(
    rrmcm_rmif_mobility_sib_8_params_t  *p_sib8_mob_info_to_cellm,
    rrm_oam_mobility_sib_8_params_t  *p_sib8_mob_info_from_oam);

/*! \fn rrm_void_t fill_irat_cdma2000_cell_params_info(rrmcm_rmif_cdma2000_cell_param_t *p_cell_param_info_to_cellm,rrm_oam_cdma2000_cell_param_t  *p_cell_param_info_from_oam)
*   \brief  This function fill the cdma2000 cell id and rand params for 
           CellM module
*   \param p_cell_param_info_to_cellm  rrmcm_rmif_cdma2000_cell_param_t
*   \param p_cell_param_info_from_oam  rrm_oam_cdma2000_cell_param_t  
*/
rrm_void_t
fill_irat_cdma2000_cell_params_info(
        rrmcm_rmif_cdma2000_cell_param_t  *p_cell_param_info_to_cellm,
        rrm_oam_cdma2000_cell_param_t  *p_cell_param_info_from_oam);


/*! \fn rrm_void_t fill_irat_cdma2000_v920_info(rrmcm_rmif_irat_parameters_cdma2000_v920_t  *p_v920_info_to_cellm,rrm_oam_irat_parameters_cdma2000_v920_t  *p_v920_info_from_oam)
*   \brief This function fill the cdma2000 v920 params for
           CellM module
*   \param p_v920_info_to_cellm rrmcm_rmif_irat_parameters_cdma2000_v920_t
*   \param p_v920_info_from_oam rrm_oam_irat_parameters_cdma2000_v920_t
*/
rrm_void_t
fill_irat_cdma2000_v920_info(
        rrmcm_rmif_irat_parameters_cdma2000_v920_t  *p_v920_info_to_cellm,
        rrm_oam_irat_parameters_cdma2000_v920_t  *p_v920_info_from_oam);


/*! \fn rrm_void_t fill_idle_mode_mobility_irat_cdma2000_info(rrmcm_idle_mode_mobility_irat_cdma_params_t *p_irat_cdma2000_info_to_cellm,rrm_oam_idle_mode_mobility_inter_rat_cdma2000_params_t *p_irat_cdma2000_info_from_oam)
*   \brief This function fill the cdma2000 idle mode mobility params for
           CellM module
*   \param p_irat_cdma2000_info_to_cellm  rrmcm_idle_mode_mobility_irat_cdma_params_t
*   \param p_irat_cdma2000_info_from_oam rrm_oam_idle_mode_mobility_inter_rat_cdma2000_params_t
*/
rrm_void_t
fill_idle_mode_mobility_irat_cdma2000_info(
    rrmcm_idle_mode_mobility_irat_cdma_params_t 
        *p_irat_cdma2000_info_to_cellm,
    rrm_oam_idle_mode_mobility_inter_rat_cdma2000_params_t 
        *p_irat_cdma2000_info_from_oam);


/*! \fn rrm_void_t rrm_oam_fill_ue_gen_cdma2000_params(rrmc_ue_generic_cdma2000_params_t *p_ue_gen_cdma2000_info_to_cellm,rrm_oam_ue_generic_cdma2000_params_t *p_ue_gen_cdma2000_info_from_oam)
 *  \brief This function fill connected mode mobility info for CellM module 
 *  \param p_ue_gen_cdma2000_info_to_cellm rrmc_ue_generic_cdma2000_params_t
 *  \param p_ue_gen_cdma2000_info_from_oam rrm_oam_ue_generic_cdma2000_params_t
*/
rrm_void_t
rrm_oam_fill_ue_gen_cdma2000_params(
        rrmc_ue_generic_cdma2000_params_t     *p_ue_gen_cdma2000_info_to_cellm,
        rrm_oam_ue_generic_cdma2000_params_t  *p_ue_gen_cdma2000_info_from_oam);

/*! \fn  rrm_return_et build_and_send_load_config_req_to_cellm(rrm_mif_gb_context_t *p_gl_ctxt,rrm_mif_context **p_mif_context,U16 mif_count,rrm_oam_load_config_req_t *p_in_message,U32 trans_id)
 *  \brief This function is used to build and send load config request to cell mgr
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param mif_count U16
 *  \param p_in_message rrm_oam_load_config_req_t
 *  \param trans_id transaction identifier
 */
rrm_return_et 
build_and_send_load_config_req_to_cellm(
     /*SPR 17777 +-*/
rrm_mif_context             **p_mif_context,
U16                         mif_count,
rrm_oam_load_config_req_t   *p_in_message,
U32                         trans_id,
rrm_error_et                *p_fail_cause);
/*! \fn void build_and_send_cell_ecn_capacity_enhance_req_to_cellm(rrm_mif_gb_context_t *p_gl_ctxt,rrm_mif_context  *p_mif_context,rrm_ecn_configure_cell_list_t *p_in_cell_ecn_confgiure_bitrate,rrmcm_rmif_cell_ecn_config_req_t *p_out_cell_ecn_config_req)
 *  \brief This function is used to build and send cell ecn capacity request to cell manager
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param p_in_cell_ecn_confgiure_bitrate rrm_ecn_configure_cell_list_t
 *  \param p_out_cell_ecn_config_req rrmcm_rmif_cell_ecn_config_req_t
 */
void build_and_send_cell_ecn_capacity_enhance_req_to_cellm
(
     /*SPR 17777 +-*/
 rrm_mif_context             *p_mif_context,
 rrm_ecn_configure_cell_list_t *p_in_cell_ecn_confgiure_bitrate,
 rrmcm_rmif_cell_ecn_config_req_t *p_out_cell_ecn_config_req
 );
/*! \fn rrm_void_t build_and_send_reconfig_schedule_params(rrm_mif_gb_context_t *p_gl_ctxt,rrm_mif_context *p_mif_context)
 *  \brief This function is used to build and send reconfig request schedule parameters
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 */
 /* SPR 20653 Fix Start */
 /* Code Removed */
 /* SPR 20653 Fix End */

void build_and_send_cell_platform_to_cellm
(
     /*SPR 17777 +-*/
 rrm_mif_context             *p_mif_context,
 rrm_plat_cell_load_t   *p_in_cell_plat_ind,
 rrmcm_rmif_cell_platform_t *p_out_cell_platform
 );

/*! \fn rrm_return_et build_and_send_son_ttt_update_ind_to_cellm(
                          rrm_mif_gb_context_t     *p_gl_ctxt,
                          rrm_mif_context          *p_mif_context,
                          son_rrm_ttt_update_ind_t *p_in_ttt_update_ind,
                          U16                       src)
 *  \brief This function is used to build and send ttt update to cell manager
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param p_in_ttt_update_ind son_rrm_ttt_update_ind_t
 *  \param src U16
 */
rrm_return_et
build_and_send_son_ttt_update_ind_to_cellm(
    rrm_mif_gb_context_t     *p_gl_ctxt,
    rrm_mif_context          *p_mif_context,
    son_rrm_ttt_update_ind_t *p_in_ttt_update_ind,
    U16                       src
);

rrm_void_t
get_ul_prb_bdgt_frm_total_bw_in_tdd
(
    rrm_oam_band_width_et channel_bandwith,
    rrm_oam_tdd_frame_structure_t *plp_tdd_frame_str_from_oam,
    U8 *ul_prb_budget
);

rrm_void_t build_and_send_get_kpi_request_to_cellm(
        rrm_oam_get_kpi_req_t *p_rrm_oam_get_kpi_req,
        rrm_mif_context *p_mif_context);

rrm_void_t build_and_send_kpi_config_req_to_cellm(
        rrm_oam_config_kpi_req_t *p_in_oam_config_kpi_req,
        rrm_mif_context          *p_mif_context);

rrm_void_t map_kpi_from_cellm( rrmcm_kpi_data_t *p_kpi_from_cellm,
            rrm_oam_kpi_data_t *p_kpi_stat);

void build_and_send_kpi_ind_to_oam (
        rrm_mif_gb_context_t        *p_gl_ctxt,
        rrm_mif_context *p_mif_context,
        rrmcm_kpi_ind_t *p_kpi_ind
        );


/* UE positioning start */
/*! \fn rrm_void_t populate_eutran_access_point_pos_params(
        rrm_oam_eutran_access_point_pos_t  *eutran_access_point_pos_from_oam,
        rrm_eutran_access_point_pos_t      *eutran_access_point_pos_to_cellm)
 *  \brief This function is used to populate EUTRAN Access point position params to CELLM
 *  \param rrm_oam_eutran_access_point_pos_t *eutran_access_point_pos_from_oam
 *  \param rrm_eutran_access_point_pos_t     *eutran_access_point_pos_to_cellm
 */

rrm_void_t
populate_eutran_access_point_pos_params(
        rrm_oam_eutran_access_point_pos_t *eutran_access_point_pos_from_oam,
        rrm_eutran_access_point_pos_t     *eutran_access_point_pos_to_cellm
        );
/* UE positioning end */

/*! \fn rrm_void_t rrm_fill_beamforming_info(rrm_oam_operator_info_t *, operator_info_t  * )
*   \brief This function fill the beamforming vector 
*   \param p_operator_cfg_params 
*   \param p_operator_info_to_cellm 
*/
/* SPR 20653 Fix Start */
rrm_void_t
rrm_fill_beamforming_info(rrm_oam_operator_info_t *p_operator_info_from_oam,
                                operator_info_t           *p_operator_info_to_cellm );

rrm_void_t
rrm_fill_tm_mode_additional_info( rrm_oam_tm_mode_additional_info_t *p_tm_mode_add_info_from_oam,
                                  operator_info_t *p_operator_info_to_cellm);
/* SPR 20653 Fix End */
/* RACH_OPTIMIZATION_CHANGES_START */
void 
build_and_send_rach_report_to_son(
        rrm_mif_gb_context_t        *p_gl_ctxt,
        rrm_mif_context             *p_mif_context,
        rrmcm_rmif_rach_l2_report_t *p_rach_l2_report
        );

void
build_and_send_rach_info_req_to_uem
(
 rrm_mif_context             *p_mif_context,
 rrm_son_rach_config_req_t   *p_in_rach_info_req_msg
     /*SPR 17777 +-*/
 );

void 
build_and_send_rach_info_req_to_cellm
(
 rrm_mif_context             *p_mif_context,
 rrm_son_rach_config_req_t   *p_in_rach_info_req_msg
     /*SPR 17777 +-*/
 );

rrm_void_t
rrm_fill_dynamic_ue_scheduling
( 
 rrm_oam_max_ue_schedule_dl_ul_t *p_ue_schedule_info_oam,
 rrm_max_ue_schedule_dl_ul_t     *p_ue_schedule_info_cellm 
 );

/* SPR 10972 Fix Start */
rrm_void_t
get_dl_prb_bdgt_frm_total_bw_in_tdd
(
 rrm_oam_band_width_et           channel_bandwith,
 rrm_oam_tdd_frame_structure_t   *plp_tdd_frame_str_from_oam,
 U8                              *dl_prb_budget
 );
/* SPR 10972 Fix End */

rrm_void_t
get_dl_prb_bdgt_frm_total_bw_in_tdd
(
 rrm_oam_band_width_et channel_bandwith,
 rrm_oam_tdd_frame_structure_t *plp_tdd_frame_str_from_oam,
 U8 *dl_prb_budget
);


/* RACH_OPTIMIZATION_CHANGES_END */
/* SPR 12603_12618 Fix start */
/* SPR 20653 Fix Start */
rrm_void_t
fill_ran_antenna_info_from_cfg
(
 ran_t                       *p_ran_info_to_cellm
 );
/* SPR 20653 Fix End */
/* SPR 12603_12618 Fix start */

/* Bug 13021 fix start */
#define RRM_DEFAULT_VALUE_FOR_B2_THRESHOLD1_RSRP   50
#define RRM_DEFAULT_VALUE_FOR_B2_THRESHOLD1_RSRQ   20
/* Bug 13021 fix end */

/* SPR_13117_FIX_START */
rrm_void_t
rrm_build_and_get_log_level_resp(rrm_oam_get_log_level_resp_t *p_out_message,
                                  rrmuem_rmif_get_log_level_resp_t *log_level_of_cellm_uem,
                                  rrm_mif_gb_context_t *p_gl_ctxt);

rrm_void_t
rrm_build_and_send_get_log_level_req_to_cellm_uem(rrm_internal_module_id_et module_id,
                                              U16 trans_id);

/* SPR_13117_FIX_END */

/* Spr 16142 Fix Start */
void build_and_send_son_stop_tnl_discovery_to_cellm
(
 son_rrm_stop_tnl_discovery_ind_t *p_in_stop_tnl_discovery_ind
 );
/* Spr 16142 Fix End */

/* Coverity 88477-80 Fix Start */
rrm_q_offsetrange_et
rrm_map_q_offset_from_oam_to_cellm
(
  rrm_oam_q_offset_range_et oam_q_offset
);
/* Coverity 88477-80 Fix End */

/*eMTC changes start*/
/*eMTC changes stop*/

#endif /* _RRM_MIF_MSG_HANDLER_H_ */

