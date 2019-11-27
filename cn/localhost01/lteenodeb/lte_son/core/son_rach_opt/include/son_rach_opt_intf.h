/*! \file son_rach_opt_intf.h
 *  \brief This file contains the  structures specific to RACH_OPT interface with other modules(RRM, SMIF)
 *  \date  Aug, 2013
 *  \author Shilpi
 */

/*!
 *  \copyright {Copyright (c) 2013, Aricent Inc. All Rights Reserved}
 */

#ifndef _SON_RACH_OPT_INTF_H_
#define _SON_RACH_OPT_INTF_H_

/*! \headerfile son_types.h <>
 */
#include <son_types.h>

/*! \headerfile son_rrm_intf.h <>
 */
#include <son_rrm_intf.h>

/*! \  struct son_rach_opt_enable_req_t
 *  \brief RACH_OPT Enable Request Structure for CSPL Communication
 *  \param transaction_id                 Transaction Identity
 *  \param enable_req                     Enable request message from OAM
 */
typedef struct _son_rach_opt_enable_req_t
{
    son_u16                         transaction_id;
    son_oam_rach_opt_enable_req_t   enable_req;
}son_rach_opt_enable_req_t;

/*! \  struct son_rach_opt_disable_req_t
 *  \brief RACH_OPT Disable Request Structure for CSPL Communication
 *  \param transaction_id   Transaction Identity
 *  \param disable_req      Disable Request from OAM
 */
typedef struct _son_rach_opt_disable_req_t
{
    son_u16                         transaction_id;
    son_oam_rach_opt_disable_req_t  disable_req;
}son_rach_opt_disable_req_t;

/*! \  struct son_rach_opt_enable_res_t
 *  \brief RACH_OPT Enable response Structure for CSPL Communication
 *  \param transaction_id                   Transaction Identity
 *  \param son_oam_rach_opt_enable_resp_t   Enable response message to OAM
 */
typedef struct _son_rach_opt_enable_res_t
{
    son_u16                         transaction_id;
    son_oam_rach_opt_enable_resp_t  enable_res;
}son_rach_opt_enable_res_t;

/*! \  struct son_rach_opt_disable_res_t
 *  \brief RACH_OPT disable response Structure for CSPL Communication
 *  \param transaction_id                   Transaction Identity
 *  \param son_oam_rach_opt_disable_resp_t  Disable response message to OAM
 */
typedef struct _son_rach_opt_disable_res_t
{
    son_u16                         transaction_id;
    son_oam_rach_opt_disable_resp_t disable_res;
}son_rach_opt_disable_res_t;

/*! \  struct son_rach_opt_modify_config_params_req_t
 *  \brief RACH_OPT modify config param request structure for CSPL Communication
 *  \param transaction_id                              Transaction Identity
 *  \param son_oam_rach_opt_modify_config_params_req_t Config param request
 *                                                     message to OAM
 */
typedef struct _son_rach_opt_modify_config_params_req_t
{
    son_u16                                     transaction_id;
    son_oam_rach_opt_modify_config_params_req_t modify_req;
}son_rach_opt_modify_config_params_req_t;

/*! \struct son_rach_opt_modify_config_params_res_t
 *  \brief RACH_OPT modify config param response structure for CSPL Communication
 *  \param transaction_id                               Transaction Identity
 *  \param son_oam_rach_opt_modify_config_params_res_t  Config param response
 *                                                      message to OAM
 */
typedef struct _son_rach_opt_modify_config_params_res_t
{
    son_u16                                         transaction_id;
    son_oam_rach_opt_modify_config_params_resp_t    modify_res;
}son_rach_opt_modify_config_params_res_t;

/*! \struct son_rach_opt_modify_rach_optmode_req_t
 *  \brief RACH_OPT modify RACH mode request structure for CSPL Communication
 *  \param transaction_id   Transaction Identity
 *  \param modify_mode_req  RACH OPT mode modification request                  
 */
typedef struct _son_rach_opt_modify_rach_optmode_req_t
{
    son_u16                                     transaction_id;
    son_oam_rach_opt_modify_rach_optmode_req_t  modify_mode_req;
}son_rach_opt_modify_rach_optmode_req_t;

/*! \struct son_rach_opt_modify_rach_optmode_res_t
 *  \brief RACH_OPT modify RACH mode response structure for CSPL Communication
 *  \param transaction_id   Transaction Identity
 *  \param modify_mode_res  RACH OPT mode modification response
 */
typedef struct _son_rach_opt_modify_rach_optmode_res_t
{
    son_u16                                     transaction_id;
    son_oam_rach_opt_modify_rach_optmode_resp_t modify_mode_res;
}son_rach_opt_modify_rach_optmode_res_t;

/*! \struct son_rach_opt_feature_state_change_ind_t
 *  \brief  Typedef son_rach_opt_feature_state_change_ind_t from
 *          son_oam_feature_state_change_ind_t
 */
typedef son_oam_feature_state_change_ind_t 
son_rach_opt_feature_state_change_ind_t;

/*! \struct son_rach_opt_failure_notification_t
 *  \brief  Typedef son_rach_opt_failure_notification_t from 
 son_oam_rach_opt_failure_notification_t
 */
typedef son_oam_rach_opt_failure_notification_t 
son_rach_opt_failure_notification_t;

/*! \  struct son_rrm_rach_config_req_t
 *  \brief Structure storing the RACH config request message data
 *  \param transaction_id       Transaction Identifier to map the req/res
 *                              messages
 *  \param req                  RACH config Request message data
 */
typedef struct _son_rrm_rach_config_req_t
{
    son_u16                     transaction_id;
    rrm_son_rach_config_req_t   req;
}son_rrm_rach_config_req_t;

/*! \  struct son_rrm_rach_config_resp_t
 *  \brief Structure storing the RACH info response message data
 *  \param transaction_id       Transaction Identifier to map the req/res
 *                              messages
 *  \param res                  RACH config Response from RRM
 */
typedef struct _son_rrm_rach_config_resp_t
{
    son_u16                     transaction_id;
    rrm_son_rach_config_resp_t  res;
}son_rrm_rach_config_resp_t;

/*! \struct son_rrm_rach_ue_info_ind_t
 *  \brief  Typedef son_rrm_rach_ue_info_ind_t from rrm_son_rach_ue_info_ind_t
 */
typedef rrm_son_rach_ue_info_ind_t son_rrm_rach_ue_info_ind_t;

/*! \struct son_rrm_l2_rach_perf_report_t
 *  \brief  Typedef son_rrm_rach_ue_info_ind_t from rrm_son_l2_rach_perf_report_t
 */
typedef rrm_son_l2_rach_perf_report_t son_rrm_l2_rach_perf_report_t;

/*! \struct son_rach_opt_ue_info_report_t
 *  \brief  Typedef son_rach_opt_ue_info_report_t from 
 son_oam_rach_opt_ue_info_report_t
 */
typedef son_oam_rach_opt_ue_info_report_t son_rach_opt_ue_info_report_t;

/*! \struct son_rach_opt_l2_perf_report_t
 *  \brief  Typedef son_rach_opt_l2_perf_report_t from
 son_oam_rach_opt_l2_perf_report_t
 */
typedef son_oam_rach_opt_l2_perf_report_t son_rach_opt_l2_perf_report_t;

/*! \struct son_rach_opt_parameter_update_req_t
 *  \brief  Typedef son_rach_opt_parameter_update_req_t from
 son_oam_rach_opt_parameter_update_req_t
 */
typedef son_oam_rach_opt_parameter_update_req_t 
 son_rach_opt_parameter_update_req_t;

#endif /* _SON_RACH_OPT_INTF_H_ */
