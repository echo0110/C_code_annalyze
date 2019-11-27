/*! \file son_mro_intf.h
 *  \brief This file contains the  structures specific to MRO interface with other modules(RRM, SMIF, ANR)
 *  \date  Dec, 2012
 *  \author Shilpi/Ravi
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */

#ifndef _SON_MRO_INTF_H_
#define _SON_MRO_INTF_H_

/*! \headerfile son_types.h <>
 */
#include <son_types.h>

/*! \headerfile son_rrm_intf.h <>
 */
#include <son_rrm_intf.h>

/*! \  struct son_mro_enable_req_t
 *  \brief MRO Enable Request Structure for CSPL Communication
 *  \param transaction_id                 Transaction Identity
 *  \param enable_req                     Enable request message from OAM
 */
typedef struct _son_mro_enable_req_t
{
    son_u16 transaction_id;
    son_oam_mro_enable_req_t enable_req;
}son_mro_enable_req_t;

/*! \  struct son_mro_disable_req_t
 *  \brief MRO Disable Request Structure  for CSPL Communication
 *  \param transaction_id                   Transaction Identity
 *  \param disable_req                      Disable Request from OAM
 */
typedef struct _son_mro_disable_req_t
{
    son_u16 transaction_id;
    son_oam_mro_disable_req_t disable_req;
}son_mro_disable_req_t;

/*! \  struct son_mro_enable_res_t
 *  \brief MRO Enable response Structure for CSPL Communication
 *  \param transaction_id                 Transaction Identity
 *  \param son_oam_mro_enable_resp_t      Enable response message to OAM
 */
typedef struct _son_mro_enable_res_t
{
    son_u16 transaction_id;
    son_oam_mro_enable_resp_t enable_res;
}son_mro_enable_res_t;

/*! \  struct son_mro_disable_res_t
 *  \brief MRO disable response Structure for CSPL Communication
 *  \param transaction_id                 Transaction Identity
 *  \param son_oam_mro_disable_resp_t     Disable response message to OAM
 */
typedef struct _son_mro_disable_res_t
{
    son_u16 transaction_id;
    son_oam_mro_disable_resp_t disable_res;
}son_mro_disable_res_t;

/*! \  struct son_mro_modify_config_params_req_t
 *  \brief MRO modify config param request structure for CSPL Communication
 *  \param transaction_id                              Transaction Identity
 *  \param son_oam_mro_modify_config_params_req_t      Config param request
 *                                                     message to OAM
 */
typedef struct _son_mro_modify_config_params_req_t
{
    son_u16 transaction_id;
    son_oam_mro_modify_config_params_req_t config_param_req;
}son_mro_modify_config_params_req_t;

/*! \struct son_mro_modify_config_params_res_t
 *  \brief MRO modify config param response structure for CSPL Communication
 *  \param transaction_id                              Transaction Identity
 *  \param son_oam_mro_modify_config_params_res_t      Config param response
 *                                                     message to OAM
 */
typedef struct _son_mro_modify_config_params_res_t
{
    son_u16 transaction_id;
    son_oam_mro_modify_config_params_resp_t config_param_res;
}son_mro_modify_config_params_res_t;

/*! \struct son_mro_feature_state_change_ind_t
 *  \brief  Typedef son_mro_feature_state_change_ind_t from
 *          son_oam_feature_state_change_ind_t
 */
typedef son_oam_feature_state_change_ind_t son_mro_feature_state_change_ind_t;

/*! \struct son_mro_ttt_update_ind_t
 *  \brief  Typedef son_mro_eutran_ttt_update_ind_t from son_oam_mro_eutran_ttt_update_ind_t
 */
typedef son_oam_mro_eutran_ttt_update_ind_t son_mro_eutran_ttt_update_ind_t;

/*! \struct son_mro_ho_attempts_ind_t
 *  \brief  Typedef son_mro_ho_attempts_ind_t from rrm_son_ho_attempt_ind_t
 */
typedef rrm_son_ho_attempt_ind_t son_mro_ho_attempts_ind_t;

/*! \struct son_mro_ho_counter_report_t
 *  \brief  Typedef son_mro_ho_counter_report_t from son_oam_mro_ho_counter_report_t
 */
typedef son_oam_mro_ho_counter_report_t son_mro_ho_counter_report_t;

/*! \struct son_mro_optimization_notification_t
 *  \brief  Typedef son_mro_optimization_notification_t from son_oam_mro_optimization_notification_t
 */
typedef son_oam_mro_optimization_notification_t son_mro_optimization_notification_t;

#endif /* _SON_MRO_INTF_H_ */
