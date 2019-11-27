/*! \file  rrm_ue_mgr.h
 *  \brief This file contains the informantion for Manager related prototypes
 *  \date January 30, 2012
 *  \author gur30329
 *  copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
*/
#ifndef _RRM_UE_MGR_H_
#define _RRM_UE_MGR_H_


/*! \headerfile <> rrm_defines.h 
*/
#include "rrm_defines.h"
#include "rrm_cell_context.h"
/*! \def RRM_UE_ERB_LOWEST_ARP 
*/
#define RRM_UE_ERB_LOWEST_ARP 14

/*! \def RRM_UE_ERB_HIGHEST_ARP
*/
#define RRM_UE_ERB_HIGHEST_ARP 0

/*! \def RRM_UE_ERB_NO_ARP
*/
#define RRM_UE_ERB_NO_ARP 15

/*! \def RRM_UE_NO_PRIORITY
*/
#define RRM_UE_NO_PRIORITY 30

/*! \def INVALID_PRIORITY
*/
#define INVALID_PRIORITY 8
/*! \def RRM_BLOCK_CELL_HO_RESP_WAIT_DURATION
*/
#define RRM_BLOCK_CELL_HO_RESP_WAIT_DURATION 12

/*! \def RRM_BLOCK_CELL_A4_REPORT_WAIT_DURATION
*/
#define RRM_BLOCK_CELL_A4_REPORT_WAIT_DURATION 4

/*! \macro RRM_PLMN_FILLER_DIGIT
*/
#define RRM_PLMN_FILLER_DIGIT 0x0F

/* SPR 12897 Start */
#define RRM_TDD_BAND_START 33
#define RRM_EUTRA_FDD_PRESENT 0x01
#define RRM_EUTRA_TDD_PRESENT 0x02
/* SPR 12897 End */

/* SPR 16053 start */
/* Indicates the maximum number of reports to be received from UE
 * for strongest cell both for Intra/Inter freq */
#define RRM_MAX_STRONGEST_CELL_RPT 2
/* SPR 16053 end */

/* SPR Fix 15664 Start */
#define RRM_MAX_ELIGIBLE_ACTIVE_VICTIM_UE 5
/* SPR Fix 15664 End */

/* SPR 20652 Fix Start */
#define RRM_INVALID_UE_INDEX  ((rrm_ue_index_t) -1)
/* SPR 20652 Fix End */
/* Fix for CSR: 00059124 */
/*! \fn         void
 *              rrm_checks_ue_supported_rats
 *  \brief      this fuction fill the ue context variable supported_rats.
 *  \param[in]  *p_ue_context 
 */

void rrm_checks_ue_supported_rats 
(rrm_ue_context_t *p_ue_context);

/* Fix for CSR: 00059124 */

/*! \  struct rrm_uem_global_proc_t
 *  \brief  global structure storing ue mgr Procedural info
 *  \param rrm_timer_id[RRM_UE_MAX_TIMER_ID]
 *  \param cell_index
 * \param  trans_id 
 */
typedef struct _rrm_uem_global_proc_t
{
    rrm_timer_t rrm_timer_id[RRM_UE_MAX_TIMER_ID];
    rrm_cell_index_t cell_index;
    U16              trans_id;
}rrm_uem_global_proc_t;


/* uplink mu mimo change start*/

/*! \fn         rrm_return_et
 *              rrm_is_ue_lie_on_provide_cell_region(rrm_cell_region_et,rrm_ue_context_t*,U32)
 *  \brief      this funtion tells in whicg region ue lies currently.
 *  \param[in]  cell_region 
 *  \param[in]  p_ue_context
 *  \param[in]  uplink_pathloss_threshold
 */
rrm_bool_et
rrm_does_ue_lie_on_provided_cell_region( rrm_cell_region_et  cell_region,
                                      rrm_ue_context_t   *p_ue_context,
                                      U32                 uplink_pathloss_threshold );


/*! \fn         rrm_cell_region_et
 *              rrm_which_cell_regionof_ue(rrm_ue_context_t*,U32)
 *  \brief      this funtion return the ue,s region.
 *  \param[in]  p_ue_context
 *  \param[in]  uplink_pathloss_threshold
 */
rrm_cell_region_et
rrm_which_cell_regionof_ue(
                           rrm_ue_context_t   *p_ue_context,
                           U32                 uplink_pathloss_threshold);


/*! \fn         rrm_bool_et
 *              rrm_find_eligible_ue_for_ul_mu_mimo
 *  \brief      this funtion return the ue,s region.
 *  \param[in]  uplink_pathloss_threshold
 *  \param[in]  p_cell_region
 *  \param[in]  region_count
 *  \param[in]  cell_region
 *  \param[in]  both_cell
 *  \param[in]  p_ue_context
 *  \param[in]  only_single_region
 */
rrm_bool_et
rrm_find_eligible_ue_for_ul_mu_mimo( U32                  uplink_pathloss_threshold,
                                     U32                **p_cell_region[2],
                                     U16                  region_count[2],
                                     rrm_cell_region_et   cell_region,
                                     rrm_bool_et          both_cell,
			             rrm_ue_context_t    *p_ue_context,
                                     rrm_bool_et          only_single_region );



/*! \fn         rrm_return_et
 *              rrm_populate_mu_mimo_info_enable_disable
 *  \brief      this funtion will process the ue list in mac config req.
 *  \param[in]  p_mac_ue_reconfig_req
 *  \param[in]  cell_region_enable
 *  \param[in]  cell_region_disable
 *  \param[in]  both_cell
 *  \param[in]  enable
 *  \param[in]  disable
 *  \param[in]  index
 *  \param[in]  p_ue_context
 */
rrm_return_et
rrm_populate_mu_mimo_info_enable_disable( RrmMacUeReconfigReq    *p_mac_ue_reconfig_req,
                                          rrm_cell_region_et            cell_region_enable,
                                          rrm_cell_region_et            cell_region_disable,
                                          rrm_bool_et                   both_cell,
                                          rrm_bool_et                   enable,
                                          rrm_bool_et                   disable,
                                          U16                           index,
                                          rrm_ue_context_t             *p_ue_context );

/*! \fn         rrm_return_et
 *              rrm_best_fill_ue_list_in_mac_config_req
 *  \brief      this funtion will process the ue list in mac config req.
 *  \param[in]  p_mac_ue_reconfig_req
 *  \param[in]  cell_region_enable
 *  \param[in]  cell_region_disable
 *  \param[in]  both_cell
 *  \param[in]  enable
 *  \param[in]  disable
 *  \param[in]  p_cell_region
 *  \param[in]  region_count
 */
rrm_return_et
rrm_best_fill_ue_list_in_mac_config_req(RrmMacUeReconfigReq    *p_mac_ue_reconfig_req,
                          rrm_cell_region_et            cell_region_enable,
                          rrm_cell_region_et            cell_region_disable,
                          rrm_bool_et                   both_cell,
                          rrm_bool_et                   enable,
                          rrm_bool_et                   disable,
                          U32                         **p_cell_region[2],
			  U16                           region_count[2]);



/*! \fn         rrm_return_et
 *              rrm_find_UE_eligible_for_ul_mumimo
 *  \brief      this funtion will process the ue eligible list.
 *  \param[in]  p_mac_ue_reconfig_req
 *  \param[in]  cell_index
 *  \param[in]  cell_region_enable
 *  \param[in]  cell_region_disable
 *  \param[in]  both_cell
 *  \param[in]  enable
 *  \param[in]  disable
 *  \param[in]  only_single_region
 */
rrm_return_et
rrm_find_UE_eligible_for_ul_mumimo( RrmMacUeReconfigReq  *p_mac_ue_reconfig_req,
                                    rrm_cell_index_t           cell_index,
                                    rrm_cell_region_et         cell_region_enable,
                                    rrm_cell_region_et         cell_region_disable,
                                    rrm_bool_et                both_cell,
                                    rrm_bool_et                enable,
                                    rrm_bool_et                disable,
                                    rrm_bool_et                only_single_region);


/*EICIC_TDD_CHANGES_START*/
U8
rrm_find_abs_frame_size(tdd_frame_structure_t *p_physical_layer_param_tdd_frame_structure);
/*EICIC_TDD_CHANGES_END*/


/*! \fn         rrm_return_et
 *              rrm_fill_mac_ue_reconfig_req_data
 *  \brief      this funtion will fill mac ue reconfig data for ue's.
 *  \param[in]  p_rrm_mac_ue_reconfig_req
 *  \param[in]  cell_index
 *  \param[in]  cell_center_prb_usage
 *  \param[in]  cell_edge_prb_usage
 */
rrm_return_et
rrm_fill_mac_ue_reconfig_req_data( RrmMacUeReconfigReq    *p_rrm_mac_ue_reconfig_req,
                                   rrm_cell_index_t              cell_index,
                                   U32                           cell_center_prb_usage,
                                   U32                           cell_edge_prb_usage );


/*! \fn         rrm_return_et
 *              rrm_send_mac_ue_reconfig_req
 *  \brief      this funtion will send  mac ue reconfig data for ue's.
 *  \param[in]  p_rrm_mac_ue_reconfig_req
 *  \param[in]  src_module_id
 *  \param[in]  transaction_id
 *  \param[in]  cell_index
 */
rrm_return_et
rrm_send_mac_ue_reconfig_req( RrmMacUeReconfigReq   *p_mac_ue_reconfig_req,
                              U16                          src_module_id,
                              U16                          transaction_id,
                              U8                           cell_index );

/*SPR 16364 START*/
rrm_return_et
rrm_build_and_send_ho_required_for_daho
(
 rrm_ue_context_t *p_ue_context,
 U16               transaction_id
);
/*SPR 16364 END*/

/*! \fn         rrm_return_et
 *              rrm_build_and_send_mac_ue_reconfig_req
 *  \brief      this funtion will send  mac ue reconfig data for ue's.
 *  \param[in]  cell_index
 */
//rrm_void_t
//rrm_build_and_send_mac_ue_reconfig_req( rrm_cell_index_t    cell_index );



/*! \fn         rrm_return_et
 *              rrm_update_or_rollback_bitmask_on_mac_ue_reconfig_cnf
 *  \brief      this funtion will send  mac ue reconfig data for ue's.
 *  \param[in]  cell_index
 *  \param[in]  p_mac_ue_reconf_cnf
 */
rrm_return_et
rrm_update_or_rollback_bitmask_on_mac_ue_reconfig_cnf( rrm_cell_index_t   cell_index,
                                                       RrmMacUEReConfigCnf
                                                                          *p_mac_ue_reconf_cnf );

/*! \fn         rrm_return_et
 *              rrm_handler_mac_periodic_report_event
 *  \brief      this funtion will send  mac ue reconfig data for ue's.
 *  \param[in]  cell_index
 *  \param[in]  p_ue_mac_stats
 *  \param[in]  p_gl_ctx
 */
rrm_return_et
rrm_handler_mac_periodic_report_event( rrm_cell_index_t   cell_index,
                                       rrm_ue_mac_stats_report_t
                                                          *p_ue_mac_stats,
                                       rrm_ue_global_context_t
                                                          *p_gl_ctx );

/*! \fn         rrm_return_et
 *              rrm_handler_mac_ue_reconfig_cnf_event
 *  \brief      this funtion will send  mac ue reconfig data for ue's.
 *  \param[in]  cell_index
 *  \param[in]  p_mac_ue_reconf_cnf
 */
rrm_return_et
rrm_handler_mac_ue_reconfig_cnf_event( rrm_cell_index_t           cell_index,
                                       RrmMacUEReConfigCnf  *p_mac_ue_reconf_cnf );

/* uplink mu mimo chnage end*/

/*! \fn         rrm_return_et
 *              rrm_uem_handle_rmif_emergency_call_active_req(void*, rrm_ue_global_context_t)
 *  \brief      this funtion will process the emergency call active req from cellm.
 *  \param[in]  p_data 
 *  \param[in]  p_rrm_ue_glb_ctxt
 */

/*beamforming chnages */
/*! \fn         rrm_return_et
 *              rrm_process_mac_ue_reconfig_cnf_for_beamforming( rrm_cell_index_t , RrmMacUEReConfigCnf *)
 *  \brief      this funtion will process the beamformig error .
 *  \param[in]  rrm_cell_index_t 
 *  \param[in]  RrmMacUEReConfigCnf *
 */
rrm_void_t
rrm_process_mac_ue_reconfig_cnf_for_beamforming(rrm_cell_index_t , RrmMacUEReConfigCnf *);


rrm_return_et 
rrm_uem_handle_rmif_emergency_call_active_req(
                                    rrm_void_t *p_data
                        /*SPR 17777 +-*/
                                );

rrm_return_et
rrm_ue_resource_clean_up(
            rrm_ue_context_t    *p_ue_context
            );
/*! \fn         rrm_return_et
 *              rrm_uem_send_rmif_emergency_call_active_resp(U16 trans_id, U8 cell_index, rrm_bool_et response)
 *  \brief      this funtion will process the emergency call active req from cellm.
 *  \param[in]  trans_id 
 *  \param[in]  cell_index
 *  \param[in]  response
 */
rrm_return_et
rrm_uem_send_rmif_emergency_call_active_resp(
                                                U16 trans_id,
                                                U8 cell_index,
                                                rrm_bool_et response
                                            );



                                   
/*! \fn         rrm_return_et
 *              rrm_uem_handle_rmif_init_config_req(U8 *, rrm_ue_global_context_t *)
 *  \brief      Send INIT config resp to MIF
 *  \param[in]  p_data
 *  \param[in]  p_rrm_ue_glb_ctx
 */

rrm_return_et
rrm_uem_handle_rmif_init_config_req(
                                      U8 *p_data,
                                      rrm_ue_global_context_t *p_rrm_ue_glb_ctx
                                  );

/* SPR 17564 Fix Start */
/*! \fn                 rrm_return_et
 *                              rrm_ue_emergency_establishment_operation(
								rrm_ue_context_t,
								rrm_cell_index_t)
 *  \brief              this funtion will perform operation when UE's attach with emergency cause.
 *  \param[in]  ue_context
 *  \param[in]  trans_id
 */
rrm_return_et
rrm_ue_emergency_establishment_operation(
                                          rrm_ue_context_t             *p_ue_context,
/* SPR 17564 Fix End */
					   U8                            trans_id,
                       rrm_cell_index_t              cell_index
					);


/*! \fn 		rrm_return_et 
 *				rrm_rrc_process_ue_admission_req(U8 *, U16 *, U16, rrm_ue_global_context_t *) 
 *  \brief		this funtion will process the UE admission request.
 *	\param[in] 	p_api_buf
 *  \param[in]  api_id
 *  \param[in]  data_len
 *  \paran[out] p_rrm_ue_glb_ctxt pointer to update admission req.
 */
rrm_return_et
rrm_rrc_process_ue_admission_req(
                                    U8 *p_api_buf,
                        /*SPR 17777 +-*/
									U16 data_len,
                                    rrm_ue_global_context_t *p_rrm_ue_glb_ctxt
                                );
/*! \fn         rrm_return_et
 *              rrm_rrc_process_ue_inactive_ind(U8 *, U16 *, U16, rrm_ue_global_context_t *)
 *  \brief      this funtion will process the UE inactivity indication.
 *  \param[in]  p_api_buf
 *  \param[in]  api_id
 *  \param[in]  data_len
 *  \paran[out] p_rrm_ue_glb_ctxt pointer.
 */
rrm_return_et
rrm_rrc_process_ue_inactive_ind
(
    U8 *p_api_buf,
    /*SPR 17777 +-*/
    U16 data_len,
    rrm_ue_global_context_t *p_rrm_ue_glb_ctxt
);
/*! \fn			rrm_return_et rrm_ue_mgr_msg_handler (rrm_void_t *, rrm_void_t *) 
*	\brief		this funtion is the Message handler for RRC messages.
*	\param[in]	p_api_buf
*	\param[in]  p_gl_ctx
*/
S32
rrm_ue_mgr_msg_handler (
							rrm_void_t *p_api_buf, 
							rrm_void_t *p_gl_ctx
						);

/*! \fn			rrm_return_et rrm_ue_mgr_rrc_msg_handler (rrm_void_t *, rrm_void_t *)
 *	\brief		this function used for the RRC message handler.
 *	\param[in]	p_api_buf
 *  \param[out] p_gl_ctx
 */
rrm_return_et
rrm_ue_mgr_rrc_msg_handler (
								rrm_void_t *p_api_buf, 
								rrm_void_t *p_gl_ctx
						   );

/*! \fn			rrm_return_et rrm_ue_mgr_l2_msg_handler(rrm_void_t *, rrm_void_t *)
 *	\brief		this function used for the L2 message handling.
 *	\param[in]	p_api_buf
 *  \param[out] p_gl_ctx
 */
rrm_return_et
rrm_ue_mgr_l2_msg_handler(
						  	rrm_void_t *p_api_buf,
                            rrm_void_t *p_gl_ctx
                         );

/*! \fn			rrm_return_et rrm_ue_mgr_rrm_cm_msg_handler(rrm_void_t *, rrm_void_t *)
 *	\brief		this function used for the cellm  message handling.
 *	\param[in]	p_api_buf
 *  \param[out] p_gl_ctx
 */
rrm_return_et
rrm_ue_mgr_rrm_cm_msg_handler (
						  		rrm_void_t *p_api_buf,
                          		rrm_void_t *p_gl_ctx
                       		  );
/*! \fn			rrm_return_et rrm_ue_mgr_pdcp_msg_handler(rrm_void_t *, rrm_void_t *)
 *	\brief		this function used for the cellm  message handling.
 *	\param[in]	p_api_buf
 *  \param[out] p_gl_ctx
 */
rrm_return_et
rrm_ue_mgr_pdcp_msg_handler(
						  		rrm_void_t *p_api_buf
    /*SPR 17777 +-*/
                       		  );
/*! \fn			rrm_return_et rrm_ue_handle_dequeued_msg(U16, rrm_ue_global_context_t *)
 *	\brief		this function used to handle the enqueued messages.
 *	\param[in]  p_rrm_ue_procedure_record	
 *  \param[out] p_rrm_glb_ctxt
 */
rrm_return_et
rrm_ue_handle_dequeued_msg (
                                /* CSR00069389 Start */
                                rrm_ue_procedure_record_t       *p_rrm_ue_procedure_record,
                                /* CSR00069389 End */
                            	rrm_ue_global_context_t *p_rrm_glb_ctxt,
                                rrm_cell_index_t        cell_index
                           );
                       
/*! \fn			rrm_return_et rrm_ue_mgr_rrm_if_msg_handler (rrm_void_t *, rrm_void_t *) 
 *	\brief		this function used for handling MIF messages.
 *	\param[in]	p_api_buf
 *  \param[in]	p_gl_ctx
 */
rrm_return_et
rrm_ue_mgr_rrm_if_msg_handler (
								rrm_void_t *p_api_buf, 
								rrm_void_t *p_gl_ctx
							  );

/*! \fn 		rrm_return_et rrm_ue_mgr_allocate_ue_context (rrm_cell_index_t cell_index,rrm_ue_index_t ue_index )
 *	\brief		this funtion will be used for allocate memory for UE
 *              global context.
 *	\param	cell_index
 *	\param  ue_index
 */
rrm_return_et
rrm_ue_mgr_allocate_ue_context(
								rrm_cell_index_t cell_index, 
								rrm_ue_index_t ue_index
							  );

/*! \fn	rrm_return_et rrm_ue_mgr_fetch_ue_context(void)
 *	\brief		this funtion used to fetch UE context from global.
 */
rrm_return_et
rrm_ue_mgr_fetch_ue_context (
							  	void
							);

/*! \fn	rrm_return_et rrm_ue_mgr_delete_ue_context(rrm_ue_context_t *p_ue_context)
 *	\brief 		this funtion used to delete the UE context for a particular UE index.
 *	\param	p_ue_context
 */
rrm_return_et
rrm_ue_mgr_delete_ue_context (
								rrm_ue_context_t *p_ue_context
							 );

/*! \fn	rrm_return_et rrm_ue_mgr_release_ue_resources(void)
 *	\brief		this function used to delete UE resources.
 */
rrm_return_et
rrm_ue_mgr_release_ue_resources (
									void
								);

/*! \fn  rrm_return_et rrm_ue_get_new_context(U16 ue_context_id, rrm_ue_context_t **p_ue_context)
 *	\brief	this function used to get the new UE context.
 *	\param	ue_context_id
 *	\param	p_ue_context
 */

/* SPR 20652 Fix Start */
rrm_return_et
rrm_ue_get_new_context (
						 U16 ue_context_id, 
                         rrm_cell_index_t cell_idx,
						 rrm_ue_context_t **p_ue_context
					   );
/* SPR 20652 Fix End */

/*! \fn rrm_return_et rrm_ue_delete_context(rrm_cell_index_t cell_index,rrm_ue_index_t ue_index)
 *	\brief		this function is used to delete UE context based on ue_index.
 *	\param	ue_index
 *	\param	cell_index
 */
rrm_return_et
rrm_ue_delete_context (
						rrm_cell_index_t cell_index,
						rrm_ue_index_t ue_index
					  );

/*! \fn	rrm_void_t * rrm_uem_init ( rrm_void_t *)
 *	\brief		this function initialize the UEM module.
 *	\param	p_ue_init_data		
 */
rrm_void_t *
rrm_uem_init (rrm_void_t *p_ue_init_data 
			  );

/*! \fn	rrm_ue_send_rrmim_snr_report_req (U16 src_id, rrm_ue_global_context_t *p_rrm_ue_glb_ctxt)
 *	\brief		This function will send SNR report.
 *	\param[in]	src_id
 *	\param[in]      p_rrm_ue_glb_ctxt		
 */

void
rrm_ue_send_rrmim_snr_report_req(
                                    U16 src_id,
                                    rrm_ue_global_context_t *p_rrm_ue_glb_ctxt
                                );

/*SPR 18241 START*/
/*! \fn rrm_return_et  rrm_ue_mgr_enqueue_req  (rrm_void_t *p_api_buf, rrm_ue_context_t *p_ue_context)
 *  \brief      This function will enqueue RRC messages
 *  \param[in]  p_api_buf
 *  \param[in]  p_ue_context
 */
rrm_return_et
rrm_ue_mgr_enqueue_req(
                         /* CSR00069389 Start */
                         rrm_ue_procedure_queue_t *p_api_buf,
                         /* CSR00069389 End */                         
        rrm_ue_context_t *p_ue_context,
        rrm_bool_et       rrm_push_head_flag 
                      );
/*SPR 18241 END*/

/* SPR 16456 Fix Start */
/*! \fn rrm_return_et  rrm_ue_mgr_enqueue_back_req  (rrm_void_t *p_api_buf, rrm_ue_context_t *p_ue_context)
 *  \brief      This function will enqueue back the dequeued RRC messages
 *  \param[in]  p_api_buf
 *  \param[in]  p_ue_context
 */
rrm_return_et
rrm_ue_mgr_enqueue_back_req(
        rrm_ue_procedure_queue_t *p_api_buf,
        rrm_ue_context_t *p_ue_context
        );

/*! \fn         U32  rrm_get_msg_size_through_api_id(U16 api_id)
 *  \brief      This function get msg_size through api_id
 *  \param[in]  api_id
 */
U32
rrm_get_msg_size_through_api_id
(
 U16 api_id
 );

/* SPR 16456 Fix Stop */


/*! \fn         rrm_return_et rrm_uem_send_internal_msg (U16, U16, U16, rrm_void_t *)
 *  \brief      This function will send MIF messages 
 *  \param[in]  dst_module_id 
 *  \param[in]  api_id
 *  \param[in]  msg_size
 *  \param[in]	p_msg
 */
rrm_return_et
rrm_uem_send_internal_msg(
                    U16  dst_module_id,
                    U16  api_id,
                    U16  msg_size,
                    rrm_void_t *p_msg
                );

/*! \fn         rrm_return_et rrm_ue_rac_release_cell_resources(rrm_ue_context_t *p_rrm_ue_ctxt)
 *  \brief      This function releases cell resources for a UE
 *  \param[in]  p_rrm_ue_ctxt
 */
rrm_return_et
rrm_ue_rac_release_cell_resources(
                                     rrm_ue_context_t *p_rrm_ue_ctxt
                                 );

/*! \fn    rrm_return_et rrm_ue_clean_proc_queue(rrm_ue_context_t *p_ue_context)
 *  \brief  This function cleans the procedural queue for a UE
 *  \param[in]  p_ue_context 
 */
rrm_return_et
rrm_ue_clean_proc_queue(
                            rrm_ue_context_t *p_ue_context
                       );

/*spr 17437 fix start*/
/*! \fn rrm_return_et rrm_ue_rbc_erab_setup(rrm_ue_context_t *p_rrm_ue_ctxt, rrm_ue_erab_setup_item_t erab_to_be_setup, U8                  gbr_configd_count, U8 ngbr_configd_count, U8 gbr_success_count, U8 ngbr_success_count, U64 required_bit_rate, U32 *num_temp_um_mode, U32 *  num_temp_am_mode, U32 *p_data_transfer_mode, U8 *p_out_dl_allocated_ngbr_prbs, U8 *p_out_ul_allocated_ngbr_prbs, U8 *                       p_out_dl_required_num_prbs, U8 *p_out_ul_required_num_prbs,rm_bool_et is_default_bearer,
rrm_rab_removal_ctxt_t *p_rab_removal_ctxt) 
                
 *  \brief      bearer control algorithm for erab setup
 *  \param[in]  p_rrm_ue_ctxt
 *  \param[in]  erab_to_be_setup
 *  \param[in]  ue_agg_max_bit_rate
 *  \param[in]  gbr_configd_count
 *  \param[in]  ngbr_configd_count
 *  \param[in]  gbr_success_count
 *  \param[in]  ngbr_success_count
 *  \param[in]  required_bit_rate
 *  \param[in]  num_temp_um_mode
 *  \param[in]  num_temp_am_mode
 *  \param[in]  p_data_transfer_mode
 *  \param[in]  p_out_dl_allocated_ngbr_prbs
 *  \param[in]  p_out_ul_allocated_ngbr_prbs
 *  \param[in]  p_out_dl_required_num_prbs
 *  \param[in]  p_out_ul_required_num_prbs
 *  \param[in]  is_default_bearer
 *  \param[in]  p_rab_removal_ctxt
 */
/*spr 17437 fix stop*/
/*spr 17437 fix stop*/
 rrm_return_et
 rrm_ue_rbc_erab_setup(
         rrm_ue_context_t   *p_rrm_ue_ctxt,
         /* SPR 19968 Start */
         rrm_ue_erab_setup_item_t *p_erab_to_be_setup,
         rrm_ue_ambr_t ue_agg_max_bit_rate,
         /* SPR 19968 End */
         U8 gbr_configd_count,
         U8 ngbr_configd_count,
         U8 gbr_success_count,
         U8 ngbr_success_count,
         /*SPR 18602 START*/
         /*code deleted*/
         /*SPR 18602 END*/
         U32 *num_temp_um_mode,
         U32 *num_temp_am_mode,
         U32 *p_data_transfer_mode,
		 /*SPR 18654 START*/
         U64  *p_out_dl_allocated_ngbr_prbs,
         U64  *p_out_ul_allocated_ngbr_prbs,
         U32  *p_out_dl_required_num_prbs,
         U32  *p_out_ul_required_num_prbs,
		 /*SPR 18654 END*/
         /* SPR 16853: start */
         rrm_bool_et is_default_bearer,
	     /*spr 19968 fix start*/
         rrm_rab_removal_ctxt_t *p_rab_removal_ctxt
#ifdef ENDC_ENABLED
		 ,rrm_bool_et is_endc_ue_x2_up
		 ,rrm_bool_et *is_resource_available
#endif
		 );
	                  /*spr 19968 fix stop*/
        /* SPR 16853: end */

/*spr 17437 fix start*/
/*! \fn rrm_return_et rrm_ue_rbc_erab_modify(rrm_ue_context_t *p_rrm_ue_ctxt, rrm_ue_erab_setup_item_t erab_to_modify,rrm_ue_ambr_t         ue_agg_max_bit_rate, U8 *p_out_dl_allocated_ngbr_prbs, U8 *p_out_ul_allocated_ngbr_prbs, U8 *p_out_dl_required_num_prbs, U8 *               p_out_ul_required_num_prbs,rrm_ue_cause_t   *p_erb_setup_modify_fail_cause,rrm_rab_removal_ctxt_t          *p_rab_removal_ctxt)
 *  \brief      bearer control algorithm for erab modify
 *  \param[in]  p_rrm_ue_ctxt
 *  \param[in]  erab_to_modify
 *  \param[in]  ue_agg_max_bit_rate
 *  \param[in]  p_out_dl_allocated_ngbr_prbs
 *  \param[in]  p_out_ul_allocated_ngbr_prbs
 *  \param[in]  p_out_dl_required_num_prbs
 *  \param[in]  p_out_ul_required_num_prbs
 *  \param[in]  p_erb_setup_modify_fail_cause
 *  \param[in]  p_rab_removal_ctxt
 */
/*spr 17437 fix stop*/
rrm_return_et
rrm_ue_rbc_erab_modify(
                          rrm_ue_context_t *p_rrm_ue_ctxt,
                          rrm_ue_erab_setup_item_t erab_to_modify,
                          rrm_ue_ambr_t ue_agg_max_bit_rate,
/*SPR 18654 START*/
              U64               *p_out_dl_allocated_ngbr_prbs,
              U64               *p_out_ul_allocated_ngbr_prbs,
              U32               *p_out_dl_required_num_prbs,
              /* SPR 15834 Fix Start */
              U32               *p_out_ul_required_num_prbs,
/*SPR 18654 END*/
              rrm_ue_cause_t   *p_erb_setup_modify_fail_cause,
              /* SPR 15834 Fix End */
         /*spr 17437 fix start*/
       rrm_rab_removal_ctxt_t          *p_rab_removal_ctxt
        /*spr 17437 fix stop*/
              );


/*! \fn         rrm_return_et rrm_ue_rac_srb_for_new_users(rrm_ue_context_t *p_ue_ctxt)
 *  \brief      UE admission control algorithm for new users where reserved resource
 *              factor would be applied
 *  \param[in]  p_ue_ctxt
 */
rrm_return_et
rrm_ue_rac_srb_for_new_users
(
 rrm_ue_context_t *p_ue_ctxt
 );

/*! \fn         rrm_return_et rrm_ue_rac_srb_for_existing_users(rrm_ue_context_t *p_ue_ctxt)
 *  \brief      UE admission control algorithm for existing users where reserved
 *              resource factor would not be applied
 *  \param[in]  p_ue_ctxt 
 */
rrm_return_et
rrm_ue_rac_srb_for_existing_users
(
 rrm_ue_context_t *p_ue_ctxt
 );

/*! \fn         rrm_void_t rrm_calulate_average_mac_stats(rrm_ue_context_t *p_ue_context)
 *  \brief      Calculate average mac stats reported by l2
 *  \param[in]  p_ue_context
 */
rrm_void_t
rrm_calulate_average_mac_stats
(
 /* eICIC_PHASE_1_2_CHANGES_START */
 rrm_ue_global_context_t *p_ue_glbl_ctxt
 /* eICIC_PHASE_1_2_CHANGES_END */
 );


/* eICIC_PHASE_1_2_CHANGES_START */
/*! Functions for updating victim UE list */ 

/*!  \brief     To add and delete a UE in victim list as per dlSINR reported from L2
 *  \param[in]  rrm ue context
 */
rrm_return_et 
rrm_update_victim_ue_list(
        rrm_ue_global_context_t *p_ue_glbl_ctxt,
        /*SPR 20461 START*/
        U32 dl_sinr,
        rrm_victim_state_et	prev_ue_state
        /*SPR 20461 END*/
        );

/* Functions for processing ue reconfig response */ 
rrm_return_et
rrm_uem_process_ue_reconfig_for_victim_ue
(
    rrm_ue_global_context_t   *p_rrm_glb_ctxt,
    rrm_cell_index_t cell_index,
    RrmMacUEReConfigCnf *p_api
);
    
/* Functions to handle schedule victim ue ind from cell manager */    
rrm_return_et
rrm_uem_handler_schedule_victim_ue
(
 rrm_ue_global_context_t   *p_rrm_glb_ctxt,
 rrm_void_t                *p_api,
    /*SPR 17777 +-*/
 U16                        data_len 
);

/* Functions to send ue reconfig req for victims to MAC */    
rrm_return_et
rrm_uem_send_l2_ue_reconfig_for_victim_ue
(
   rrm_ue_global_context_t     *p_rrm_glb_ctxt,
    rrm_ue_index_t              *ue_index_list,
    U8                          ue_update_count,
    rrm_cell_index_t            cell_index
);

/* Functions to send ue reconfig req for victims to RRC */    
rrm_return_et
rrm_uem_send_rrc_ue_reconfig_for_victim_ue
(
    rrm_ue_context_t *p_rrm_ue_context,
    rrm_bool_et     x2_up_flag,
    U8 request_type
);

/*SPR14140start */
rrm_return_et
rrm_uem_handle_meas_sf_pattern_reconfig
(
    /*SPR 17777 +-*/
    rrm_ue_global_context_t *p_rrm_glb_ctxt,
    /* SPR 14140 start */
    rrm_cell_index_t            cell_index
    /* SPR 14140 start */
);
/*SPR14140end */
/* SPR 14226 start */
rrm_void_t
rrm_uem_update_victim_ue_list_based_on_valid_aggressor
(
 YLIST *victim_list,
 rrm_cell_index_t cell_index,
 U16   *valid_aggressor,
 U16    num_valid_aggressor
 ); 

S32
rrm_uem_check_valid_victim_ue
(
 victim_ue_node_t *p_victim_node,
 rrm_cell_index_t cell_index,
 U16		 *aggressor_list,
 U16    num_valid_aggressor
 );

const rrm_void_t*
rrm_uem_key_of_victim_ue
(
 const YLNODE *p_input_node
 );

rrm_return_et
rrm_uem_find_num_eligible_ue_for_ue_reconfig
(
    rrm_cell_context_t *p_cell_context,
    U32                victim_ue_count,
    U16*               count_of_victim_ue_to_be_reconf 
);

SInt32
rrm_uem_compare_victim_ue_based_on_sinr
(
 const rrm_void_t * key_value1,
 const rrm_void_t * key_value2
 ) ;
/* SPR 14226 end*/
/* eICIC_PHASE_1_2_CHANGES_END */

/*CA HARDENING CHANGES 6.3.0*/

/*CA HARDENING CHANGES 6.3.0*/
U32 rrm_ue_get_utra_tdd128_freq_band (U32 utran_freq);

U32 rrm_ue_get_utra_tdd384_freq_band (U32 utran_freq);

U32 rrm_ue_get_utra_tdd768_freq_band (U32 utran_freq);

/* Uplink power control end */

/*! Functions for QUEUE mechanisms for message queuing */
/*! \fn      rrm_void_t rrm_init_queue( rrm_ue_proc_queue_t *p_procedure_queue)
 *  \brief     To start a procedure queue 
 *  \param[in]  p_procedure_queue
 */
 rrm_void_t 
rrm_init_queue
(
    rrm_ue_proc_queue_t *p_procedure_queue 
);

/*! \fn      U32 rrm_get_queue_count(rrm_ue_proc_queue_t *p_procedure_queue) 
 *  \brief      Find the number of items in queue
 *  \param[in] p_procedure_queue 
 */
 U32 
rrm_get_queue_count
(
    rrm_ue_proc_queue_t *p_procedure_queue 
);

/*SPR 21479 Fix start*/
 rrm_ue_global_context_t * rrm_uem_get_ctxt
(
    U16 module_id
);
/*SPR 21479 Fix end*/
/*! \fn      rrm_void_t rrm_request_enqueue(rrm_ue_proc_queue_t *p_procedure_queue,rrm_ue_procedure_record_t *p_procedure_record) 
 *  \brief      To enqueue a procedure in queue
 *  \param[in] p_procedure_queue 
 *  \param[in] p_procedure_record 
 */
 rrm_void_t 
rrm_request_enqueue
(
    rrm_ue_proc_queue_t *p_procedure_queue, 
    rrm_ue_procedure_record_t *p_procedure_record
);

/* CSR00069389 Start */
/*! \fn      rrm_void_t rrm_request_enqueue_before(rrm_ue_proc_queue_t *p_procedure_queue,
    rrm_ue_procedure_record_t *p_procedure_record_next,rrm_ue_procedure_record_t *p_procedure_record) 
 *  \brief      To enqueue a procedure in queue before the specified node
 *  \param[in] p_procedure_queue 
 *  \param[in] p_procedure_queue_next 
 *  \param[in] p_procedure_record 
 */
 rrm_void_t 
rrm_request_enqueue_before
(
    rrm_ue_proc_queue_t *p_procedure_queue, 
    rrm_ue_procedure_record_t *p_procedure_record_next,
    rrm_ue_procedure_record_t *p_procedure_record
);
/* CSR00069389 End */

/* SPR 15849 Start */
/*! \fn         rrm_ue_procedure_record_t * rrm_request_dequeue(rrm_ue_proc_queue_t *p_procedure_queue)
 *  \brief      To dequeue a procedure from queue
 *  \param[in]  p_procedure_queue
 */
rrm_ue_procedure_record_t *
rrm_request_dequeue
(
    rrm_ue_context_t *p_ue_context
);
rrm_ue_procedure_record_t *
rrm_request_esmlc_dequeue
(
    rrm_ue_context_t *p_ue_context
     );
/* SPR 15849 End */


/*! \fn      rrm_void_t rrm_ue_set_state( rrm_ue_context_t *p_ue_context, rrm_ue_states_et ue_state) 
 *  \brief      To set the UE state
 *  \param[in]  p_ue_context
 *  \param[in]  ue_state
 */
 rrm_void_t 
rrm_ue_set_state
(
    rrm_ue_context_t *p_ue_context,
    rrm_ue_states_et ue_state
);
/*SPR21623 Changes Start*/
/*! \fn      rrm_void_t rrm_set_ue_proc_state(rrm_ue_procedure_t *current_procedure,  rrm_ue_erb_states_et state)
 *  \brief      To set the Current Procedure State
 *  \param[in]  p_current_procedure 
 *  \param[in]  state 
 */
 rrm_void_t 
rrm_set_ue_proc_state
(
    rrm_ue_procedure_t *p_current_procedure,
    rrm_ue_erb_states_et state
);
/*SPR21623 Changes End*/
/*! \fn rrm_return_et rrm_ue_send_l2_msg( U16  dst_module_id, U16  api_id,
U16  msg_size, rrm_void_t *p_msg)
 *  \brief This function Send message to L2
 *  \param dst_module_id destination module id
 *  \param api_id api id
 *  \param msg_size message size
 *  \param p_msg msg
*/
rrm_return_et
rrm_ue_send_l2_msg(
    /*SPR 17777 +-*/
                    U16  msg_size,
                    rrm_void_t *p_msg
                );
/*! \fn rrm_ue_send_l2_snr_report_req(U16 src_id, rrm_ue_global_context_t *p_rrm_ue_glb_ctxt)
 *  \brief This function sends snr report to L2
 *  \param  src_id
 *  \param  p_rrm_ue_glb_ctxt
*/
void
rrm_ue_send_l2_snr_report_req(
                                    U16 src_id,
                                    rrm_ue_global_context_t *p_rrm_ue_glb_ctxt
                                );
/*! \fn rrm_return_et rrm_uem_handle_rmif_set_log_level_req(rrm_ue_global_context_t *p_rrm_ue_glb_ctxt, rrm_void_t*p_api, U16 api_id, U16 data_len)
 *  \brief This function Processes set log level request 
 *  \param p_rrm_ue_glb_ctxt
 *  \param p_api Input Api buffer
 *  \param api_id Incoming Api id
 *  \param data_len Incoming data length
*/
rrm_return_et
rrm_uem_handle_rmif_set_log_level_req(
    rrm_ue_global_context_t     *p_rrm_ue_glb_ctxt,
    rrm_void_t *p_api /*Input Api buffer*/
    /*SPR 17777 +-*/
);
/*! \fn rrm_return_et rrm_uem_handle_rmif_log_enb_dis_req(
rrm_ue_global_context_t *p_rrm_ue_glb_ctxt, rrm_void_t *p_api, U16 api_id, U16 data_len)
 * \brief This function processes log enable/disable request
 * \param p_rrm_ue_glb_ctxt
 * \param p_api Input Api buffer
 * \param api_id Incoming Api id
 * \param data_len Incoming data length
*/
rrm_return_et
rrm_uem_handle_rmif_log_enb_dis_req
(
    rrm_ue_global_context_t     *p_rrm_ue_glb_ctxt,
    rrm_void_t *p_api, /*Input Api buffer*/
    U16 api_id /*Incoming Api id*/
    /*SPR 17777 +-*/
);

rrm_return_et
rrm_uem_handle_cm_delete_all_ue_context_req(
    rrm_ue_global_context_t     *p_rrm_ue_glb_ctxt,
    rrm_void_t *p_api /*Input Api buffer*/
    /*SPR 17777 +-*/
);

/*! \fn         rrm_return_et rrm_ue_insert_erb_entity(rrm_ue_erb_t *ue_erab)
 *  \brief      this funtion will insert the rab entity to erb priority list.
 *  \param[in]  ue_erab  
 */
rrm_return_et 
rrm_ue_insert_erb_entity(
	rrm_ue_erb_t *ue_erab 
);

/*! \fn         rrm_return_et rrm_ue_delete_erb_entity( rrm_ue_erb_t *ue_erab )
 *  \brief      this funtion will delete the rab entity from erb priority list.
 *  \param[in]  rrm_ue_erab_t
 */
rrm_return_et
rrm_ue_delete_erb_entity(
	rrm_ue_erb_t *ue_erab /* The Incoming UE Rab */
);

/*! \fn         rrm_return_et rrm_ue_get_low_priority_erb(rrm_ue_erb_t *ue_erb, rrm_ue_context_t *ue_context, rrm_ue_erab_setup_item_t *erab_to_be_setup )
 *  \brief      this funtion will find the erb with lowest priority to be released
 *  \param[out] ue_erb
 *  \param[in]	ue_context
 *  \param[in]  erab_to_be_setup
 */
rrm_return_et
rrm_ue_get_low_priority_erb(
        rrm_ue_erb_t *ue_erb, /* The output UE ERB struct */
        rrm_ue_context_t *ue_context, /* Incoming UE context*/
        rrm_ue_erab_setup_item_t *erab_to_be_setup /* Incoming ERAB specific DATA*/
);

/*spr 17437 fix start*/
/*! \fn         rrm_return_et
 *      rrm_ue_get_low_priority_gbr_erb(rrm_ue_erb_t *ue_erb,rrm_ue_context_t *p_rrm_ue_ctxt, rrm_ue_erab_setup_item_t *erab_to_be_setup,   U8            type)
 *  \brief      this funtion will find the lowest priority gbr erab to be released
 *  \param ue_erb
 *  \param p_rrm_ue_ctxt
 *  \param erab_to_be_setup
 *  \param type
 */
/*spr 17437 fix stop*/

 
rrm_return_et
rrm_ue_get_low_priority_gbr_erb(
        rrm_ue_erb_t *ue_erb, /* The Output UE ERAB struct*/
	rrm_ue_context_t *p_rrm_ue_ctxt, /* Incoming UE context*/
    /*spr 17437 fix start*/
	rrm_ue_erab_setup_item_t *erab_to_be_setup, /* Incoming ERAB specific DATA*/
    U8            type
    /*spr 17437 fix stop*/

);

/*! \fn         rrm_return_et
 *              rrm_ue_insert_ue_to_priority_list(rrm_ue_context_t *ue_context)
 *  \brief      this funtion will insert the ue to the ue priority list.
 *  \param[in]  ue_context
 */
rrm_return_et
rrm_ue_insert_ue_to_priority_list(
        rrm_ue_context_t *ue_context /* The Incoming UE to be inserted */
);

/*! \fn         rrm_return_et
 *              rrm_ue_delete_ue_from_priority_list(rrm_ue_context_t *ue_context)
 *  \brief      this funtion will delete the ue from the ue priority list.
 *  \param[in]  ue_context
 */
rrm_return_et
rrm_ue_delete_ue_from_priority_list(
        rrm_ue_context_t *ue_context /* The Incoming UE to be Deleted */
);


/*! \fn         rrm_return_et
 *              rrm_ue_update_ue_priority_list(rrm_ue_context_t *p_ue_context)
 *  \brief      will update the ue priority list
 *  \param[in]  p_ue_context
 */
rrm_return_et
rrm_ue_update_ue_priority_list(
        rrm_ue_context_t *p_ue_context /* Incoming UE context*/
);

/*! \fn         rrm_return_et
 *              rrm_ue_update_erb_priority_list(rrm_ue_context_t *ue_context)
 *  \brief      will delete all the erbs for any ue
 *  \param[in]  p_ue_context
 */
rrm_return_et
rrm_ue_update_erb_priority_list(
        rrm_ue_context_t *p_ue_context /* Incoming  UE context*/
);

/*! \fn         rrm_return_et
 *              rrm_ue_assign_srb_params(rrm_ue_context_t *p_rrm_ue_ctxt)
 *  \brief      Will update the srb params
 *  \param[in]  p_rrm_ue_ctxt
 */
rrm_return_et
rrm_ue_assign_srb_params
(
 rrm_ue_context_t *p_rrm_ue_ctxt
 );

/* SPR 20652 Fix Start */
/* SPR 21426 Start */
/*! \fn         rrm_ue_context_t * rrm_ue_find_context(rrm_ue_index_t ue_index, rrm_cell_index_t cell_index)
 *  \brief      This function returns the pointer to the context of UE
 *              specified in the argument
 *  \param[in]  ue_index
 *  \param[in]  cell_index
 */
/* SPR 21426 End */
rrm_ue_context_t *
rrm_ue_find_context(
        rrm_ue_index_t   ue_index,
        rrm_cell_index_t cell_index
        );
/* SPR 20652 Fix End */

 /*SPR 15314 start*/
/*! \fn         rrm_bool_et
 *              rrm_uem_check_configure_auto_gap_or_cdrx_for_eutra(
                    rrm_cell_context_t              *p_cell_context,
                    rrm_ue_context_t                *p_ue_context)
 *  \brief      This function check autonomous_gap or cdrx configured for eutra
 *              for CGI Reporting purpose
 *  \param[in]  cell_context
 *  \param[in]  ue_context
 */
rrm_bool_et
rrm_uem_check_configure_auto_gap_or_cdrx_for_eutra(
        rrm_cell_context_t              *p_cell_context,
        rrm_ue_context_t                *p_ue_context
        );
 /*SPR 15314 end*/

/*! \fn         rrm_return_et
 *              rrm_uem_send_meas_config_req_for_cgi_reporting(
                    rrm_cell_context_t              *p_cell_context,
                    rrm_son_meas_config_req_t       *p_meas_config_req)
 *  \brief      This function is prepares and sends the meas config request to RRC
 *              for CGI Reporting purpose
 *  \param[in]  cell_context
 *  \param[in]  p_meas_config_req
 */
rrm_return_et
rrm_uem_send_meas_config_req_for_cgi_reporting(
        rrm_cell_context_t              *p_cell_context,
        rrm_son_meas_config_req_t       *p_meas_config_req
        );

/*! \fn         rrm_void_t rrm_uem_send_meas_config_req(rrm_cell_context_t *p_cell_context)
 *  \brief      This function is prepares and sends the meas config request to RRC
 *              for Strongest Cell Reporting purpose
 *  \param[in]  p_cell_context
 */
rrm_void_t
rrm_uem_send_meas_config_req(
        rrm_cell_context_t *p_cell_context);

/*! \fn         rrm_return_et rrm_uem_handle_rmif_ue_release_from_oam_req(rrm_void_t *p_data)
 *  \brief      This function is prepares and sends the ue release ind  request to RRC
 *  \param[in]  p_data 
 */

rrm_return_et rrm_uem_handle_rmif_ue_release_from_oam_req(rrm_void_t *p_data);

/*! \fn         rrm_return_et
 *              rrm_uem_handle_rmif_release_all_existing_ue_forcefully_req (void *p_data)
 *  \brief      This function is prepares and sends the ue release ind  request to RRC
 *              for all active ue
 *  \param[in]  p_data
 */
rrm_return_et
rrm_uem_handle_rmif_release_all_existing_ue_forcefully_req(void *p_data);

/*! \fn         rrm_return_et rrm_ue_delete_erb_entity(rrm_ue_erb_t *p_ue_erab)
 *  \brief      This function deletes the UE RAB information to the global list
 *  \param[in]  p_ue_erab
 */
rrm_return_et
rrm_ue_delete_erb_entity(
	rrm_ue_erb_t *p_ue_erab
	);

/*! \fn         rrm_return_et rrm_ue_update_ue_priority_list(rrm_ue_context_t *p_ue_context)
 *  \brief      This function will update the ue priority list
 *  \param[in]  p_ue_context
 */
rrm_return_et
rrm_ue_update_ue_priority_list(
	rrm_ue_context_t *p_ue_context
	);

/*! \fn        rrm_return_et hack_send_snr_response(rrm_ue_index_t ue_index)
*  \brief      This function used as a hack for L2 interface
*  \param[in]  ue_index
*/
rrm_return_et
hack_send_snr_response(
     rrm_ue_index_t ue_index
     );


/*! \fn        rrm_return_et rrm_uem_handle_rmif_non_emrgncy_active_calls_ho_req (void *p_data)
 *  \brief     Handle HO_REQ received from MIF to do HO of non-emergency active calls
 *  \param[in] p_data
 */
rrm_return_et
rrm_uem_handle_rmif_non_emrgncy_active_calls_ho_req (void *p_data);

/*! \fn rrm_return_et rrm_uem_send_rmif_non_emrgncy_active_calls_ho_resp (U16 trans_id, rrm_cell_index_t cell_index, rrm_bool_et response)
 *  \brief     Sends HO_RESP for non-emergency active calls to MIF
 *  \param[in] trans_id
 *  \param[in] cell_index 
 *  \param[in] response
 */
rrm_return_et
rrm_uem_send_rmif_non_emrgncy_active_calls_ho_resp(
        U16 trans_id,
        rrm_cell_index_t cell_index,
        rrm_bool_et response
        );




/*LTE_RRM_TM_SWITCH_START*/
/*! \fn rrm_return_et rrm_process_ue_l2_report(rrm_ue_events_et  current_event, rrm_ue_context_t *p_ue_context, rrm_void_t *p_ue_data_report)
*  \brief      This function used as a hack for L2 interface
*  \param[in]  current_event
*  \param[in]  p_ue_context
*  \param[in]  p_ue_data_report
*/
rrm_return_et
rrm_process_ue_l2_report(rrm_ue_events_et  current_event,
    /* eICIC_PHASE_1_2_CHANGES_START */
             rrm_ue_global_context_t *p_ue_glbl_ctxt,
    /* eICIC_PHASE_1_2_CHANGES_END */
			 rrm_void_t   	   *p_ue_data_report);



/*! \fn        rrm_return_et rrm_process_ue_mac_periodic_stats(rrm_ue_context_t *p_ue_context,PeriodicReportInfo *p_ue_mac_report)
*  \brief      This function used as a hack for L2 interface
*  \param[in]  p_ue_context
*  \param[in]  p_ue_mac_report
*/
rrm_return_et
rrm_process_ue_mac_periodic_stats(
        /* eICIC_PHASE_1_2_CHANGES_START */
        rrm_ue_global_context_t *p_ue_glbl_ctxt,
        /* eICIC_PHASE_1_2_CHANGES_END */
        /* + SPR 20078 */
        PeriodicReportInfo    *p_ue_mac_report);
        /* - SPR 20078 */




/*! \fn U16 rrm_calulate_average_sinr(const rrm_ue_context_t *p_ue_context)
*  \brief      this function find out the average sinr 
*  \param[in]  p_ue_context
*/
U16
rrm_calulate_average_sinr(const rrm_ue_context_t *p_ue_context );


/*! \fn        rrm_return_et rrm_uem_process_fsm(rrm_ue_states_et ue_state,rrm_ue_events_et ue_event, rrm_ue_global_context_t *p_rrm_ue_glb_ctxt)
 *  \brief      This function used as a hack for L2 interface
 *  \param[in]  ue_state
 *  \param[in]  ue_event
 *  \param[in]  p_rrm_ue_glb_ctxt
 */
rrm_return_et
rrm_uem_process_fsm(rrm_ue_states_et ue_state,
                    rrm_ue_events_et ue_event,
		    rrm_ue_global_context_t *p_rrm_ue_glb_ctxt);


/*! \fn        rrm_return_et rrm_ue_rac_release_cqi_res_index(rrm_ue_context_t *p_rrm_ue_ctxt)
 *  \brief      Releases the reserved CQI resources during UE admission
 *  \param[in]  p_rrm_ue_ctxt
*/
rrm_return_et
rrm_ue_rac_release_cqi_res_index
(
    rrm_ue_context_t *p_rrm_ue_ctxt
 );






/*LTE_RRM_TM_SWITCH_END*/
/*! \fn rrm_return_et rrm_ue_validate_utran_cell(rrm_ue_context_t* p_ue_context,U16  pci)
*  \brief      This function validates a UTRAN cell against  HO restriction list
*               and returns the pointer to the cell
*  \param[in]    p_ue_context
*  \param[in]    pci
*/
/* SRVCC SUPPORT START */
utran_freq_cells_t *
/* SRVCC SUPPORT END */

/*SPR_15241_Start*/
rrm_ue_validate_utran_cell(rrm_ue_context_t*   p_ue_context,U16 pci,rrm_bool_et *ue_support_srvcc);
/*SPR_15241_End*/
/* SPR_13721_fix : start */
/*Redundant code removed*/
/* SPR_13721_fix : end */

/* SPR 15336 Fix Start */
/* GERAN-HO changes start */
/*! \fn        rrm_return_et rrm_ue_validate_geran_cell(rrm_ue_context_t* p_ue_context, U16 pci, U8 *dtm_ho_supp)
*  \brief      This function validates a GERAN cell against  HO restriction list
*  \param[in]  p_ue_context
*  \param[in]  pci
*/
/* SPR 15674 Start */
/* SPR_13721_fix : start */
geran_freq_ncl_t *
/* SPR_13721_fix : end */
rrm_ue_validate_geran_cell(rrm_ue_context_t* p_ue_context, rrm_rpt_cell_info_t rrm_rpt_cell_info, U8 *dtm_ho_supp);
/* SPR 15674 End */
/* SPR 15336 Fix End*/

/* GERAN-HO changes end */
/****************************************************************************
 **** Function Name  : rrm_cellm_get_redirected_carrier_freq
 **** Inputs         : UE Context
 ****                  Physical Cell Id
 **** Outputs        : None
 **** Returns        : Carrier Frequency corresponding to the given PCI
 **** Description    : returns the Carrier Frequency of the given PCI
 *******************************************************************************/
U16 rrm_cellm_get_redirected_carrier_freq(
                rrm_ue_context_t*   p_ue_context,       
                U16                 phy_cell_id);

/****************************************************************************
 ** Function Name  : rrm_ue_get_strongest_phy_cell_id_for_ho
 ** Inputs         : UE Context
 ** Outputs        : type of the target cell(eutra,utra),
                     ue membership status presence and 
                     csg id presence for the ncl
 ** Returns        : Returns stongest neighbour cell ID 
		     (based on SPID/freq-priority list) in the list that is 
		     currently Active, otherwise INVALID_PHYSICAL_CELL_ID
 ** Description    : Retrieves the strongest cell which can be chosen for HO.
 *****************************************************************************/
/* SPR 15674 Start */
phy_cell_id_info_t rrm_ue_get_strongest_phy_cell_id_for_ho (
/* SPR 15674 End */
        rrm_ue_context_t *p_ue_ctxt,
        /*csg start*/
        rrm_bool_et *csg_membership_status_presence, /*outgoing param - csg membership presence flag for ncl*/
        rrm_bool_et *csg_id_presence /*outgoing param - csg membership presence flag for ncl*/
        /*csg end*/
        );
/****************************************************************************
 ** Function Name  : uerrm_add_ue_history_e_utran_cell_information
 ** Inputs         : UE context information
 ** Outputs        : NONE
 ** Returns        : NONE
 ** Description    : Updates the UE context with the history information
 *****************************************************************************/
rrm_void_t uerrm_add_ue_history_e_utran_cell_information(
		 rrm_ue_context_t *p_ue_context);

/**************************************************************************
 * Function Name  : rrm_handle_ue_reconfig_resp_fail
 * Inputs         : UE context information
 * Outputs        : None
 * Returns        : None
 * Description    : Handle UE reconfiguration fail response 
 **************************************************************************/
void
rrm_handle_ue_reconfig_resp_fail
(
    rrm_ue_context_t *p_ue_context
);

/* Freq-Priority Fix changes start */
/****************************************************************************
 **** Function Name  : rrm_ue_stop_ho_timers
 **** Inputs         : UE Context
 **** Outputs        : None
 **** Returns        : None
 **** Description    : This function stops all the timers started as part of HO.
 *******************************************************************************/
rrm_void_t rrm_ue_stop_ho_timers(rrm_ue_context_t   *p_ue_context);
/* Freq-Priority Fix changes end */

/*! \fn        rrm_return_et rrm_compare_cqi_information(const rrm_ue_cqi_paramas_t *p_ue_updated_cqi_info, const rrm_ue_cqi_paramas_t *p_ue_ongoing_cqi_info, rrm_bool_et *p_cqi_info_match)
*  \brief      This function compares the cqi information
*  \param[in]  p_ue_updated_cqi_info
*  \param[in]  p_ue_ongoing_cqi_info
*  \param[in]  p_cqi_info_match
*/
rrm_return_et
rrm_compare_cqi_information(const rrm_ue_cqi_paramas_t *p_ue_updated_cqi_info,
                            const rrm_ue_cqi_paramas_t *p_ue_ongoing_cqi_info,
                            rrm_bool_et                   *p_cqi_info_match);

/*! \fn        rrm_return_et rrm_backup_cqi_information(rrm_ue_context_t*p_ue_context)
*  \brief      This function takes the back up if cqi information
*  \param[in]  p_ue_context
*/
rrm_return_et
rrm_backup_cqi_information( rrm_ue_context_t    *p_ue_context);

rrm_return_et
rrm_compare_ue_antenna_info_r10(rrm_ue_index_t       ue_index,
                                rrm_cell_index_t     cell_index,
                                const rrc_antenna_info_dedicated_r10_t *p_antenna_info,
                                rrm_bool_et           *p_antenna_info_match);

/*! \fn rrm_return_et rrm_set_cqi_information_for_ue_r10(rrm_ue_context_t *p_ue_context ,rrm_ue_phy_cqi_reporting_t *p_ue_updated_cqi_info)
*  \brief      This function set the cqi information
*  \param[in]  p_ue_context
*  \param[in]  p_ue_updated_cqi_info
*/
rrm_return_et
rrm_set_cqi_information_for_ue_r10( rrm_ue_context_t   *p_ue_context,
                                     rrc_cqi_report_config_r10_t
                                                   *p_ue_updated_cqi_info);

/*! \fn rrm_return_et rrm_set_cqi_information_for_ue(rrm_ue_context_t *p_ue_context ,rrm_ue_phy_cqi_reporting_t *p_ue_updated_cqi_info)
*  \brief      This function set the cqi information
*  \param[in]  p_ue_context
*  \param[in]  p_ue_updated_cqi_info
*/
rrm_return_et
rrm_set_cqi_information_for_ue( rrm_ue_context_t   *p_ue_context,
                               rrm_ue_phy_cqi_reporting_t 
                                                   *p_ue_updated_cqi_info);

/*! \fn        rrm_return_et rrm_rollback_ue_cqi_information(rrm_ue_context_t *p_ue_context)
*  \brief      This function takes the back up if cqi information
*  \param[in]  p_ue_context
*/
rrm_return_et
rrm_rollback_ue_cqi_information( rrm_ue_context_t   *p_ue_context );

/*! \fn        rrm_return_et rrm_rollback_ue_cqi_information_r10(rrm_ue_context_t *p_ue_context)
*  \brief      This function rollback the back up of cqi information r10 if UE reconfig fails for Ue.
*  \param[in]  p_ue_context
*/
rrm_return_et
rrm_rollback_ue_cqi_information_r10( rrm_ue_context_t   *p_ue_context );

/*! \fn        rrm_return_et rrm_rollback_ue_antenna_info_r10(rrm_ue_context_t *p_ue_context)
*  \brief      This function rollback the back up of antenna info r10 if UE reconfig fails for Ue.
*  \param[in]  p_ue_context
*/
rrm_return_et
rrm_rollback_ue_antenna_info_r10( rrm_ue_context_t   *p_ue_context );

/**************************************************************************
 * Function Name  : rrm_ue_is_ue_supported_eutra_frequency
 * Inputs         : UE context information, frequency
 * Outputs        : None
 * Returns        : None
 * Description    : returns freq band of freq if it is supported else RRM_ZERO
 **************************************************************************/
/* SPR 12897 Start */
U8
/* SPR 12897 End */
rrm_ue_is_ue_supported_eutra_frequency (
    U32 frequency,
    rrm_ue_context_t *p_ue_context
    );

/*SPR_15241_Start*/
rrm_void_t
rrm_uem_is_eutra_freq_fdd_or_tdd(
        U8                              *p_eutran_fdd_tdd_bitmask,
        U8                              freq_band,
        U32                             earfcn
        );
rrm_bool_et
rrm_check_ue_capability_for_voip(
        rrm_ue_context_t *p_ue_ctxt,
        U32              serv_dl_earfcn
        );

/*SPR_15241_End*/
/* SPR 22731 Fix Start */
/**************************************************************************
 * Function Name  : rrm_ue_is_ue_supported_utra_fdd_frequency
 * Inputs         : UE context information, frequency
 * Outputs        : None
 * Returns        : None
 * Description    : returns TRUE is supported 
 **************************************************************************/
rrm_bool_et
rrm_ue_is_ue_supported_utra_fdd_frequency (
    U16 frequency,
    rrm_ue_context_t *p_ue_context
    );

/* SPR_14564_fix: start */
/**************************************************************************
 * Function Name  : rrm_ue_is_ue_supported_utra_tdd_frequency
 * Inputs         : UE context information, frequency
 * Outputs        : None
 * Returns        : None
 * Description    : returns TRUE is supported 
 **************************************************************************/
rrm_bool_et
rrm_ue_is_ue_supported_utra_tdd_frequency (
        U16 frequency,
        rrm_ue_context_t* p_ue_context
        );
/* SPR 22731 Fix End */

U32 rrm_ue_get_utra_tdd128_freq_band (U32 utran_freq);

U32 rrm_ue_get_utra_tdd384_freq_band (U32 utran_freq);

U32 rrm_ue_get_utra_tdd768_freq_band (U32 utran_freq);
U32 rrm_ue_get_utra_freq_band (U32 utran_freq);
/* SPR_14564_fix: end */
/* Start fix for SPR 14987 */
rrm_void_t
rrm_send_meas_config_request_for_anr
(
    rrm_cell_context_t *p_cell_context,
    rrm_ue_context_t   *p_ue_context
);
/* End fix for SPR 14987 */
/*! \fn        rrm_bool_et rrm_ue_is_ue_supported_geran_frequency( U16 carrier_freq, rrm_ue_context_t *p_ue_context)
*  \brief      This function checks whether UE supports a given GERAN freq
*  \param[in]  carrier_freq
*  \param[in]  p_ue_context
*/
rrm_bool_et rrm_ue_is_ue_supported_geran_frequency (
    U16 carrier_freq,
    rrm_ue_context_t *p_ue_context
    );

/*! \fn         rrm_bool_et rrm_ue_is_ue_supported_cdma2000_frequency(U8 band_class,rrm_ue_context_t* p_ue_context)
*  \brief      This function checks whether UE supports a given cdma band
*  \param[in]  band_class 
*  \param[in]  p_ue_context
*/
rrm_bool_et rrm_ue_is_ue_supported_cdma2000_frequency (
    U8 type,
    U8 band_class,
    rrm_ue_context_t* p_ue_context
    );
/*! \fn       U16 rrm_ue_get_geran_pci(rrm_ue_context_t *p_ue_ctx,U8 ncc, U8 bscc)
*  \brief     This function returns the configured PCI of geran cell
*             based on the NCC and BSCC.
*  \param[in] p_ue_ctx
*  \param[in] ncc
*  \param[in] bscc
*/
U16 rrm_ue_get_geran_pci(rrm_ue_context_t *p_ue_ctx,U8 ncc, U8 bscc);/*SPR 17777+-*/


/****************************************************************************
 * Function Name  : rrm_get_csg_non_csg_ues
 * Inputs         : none
 * Outputs        : number of connected csg and non
 csg ues
 * Returns        : void
 * Description    : This function finds the number of admitted
 csg and non csg ues
 ****************************************************************************/
void
rrm_get_csg_non_csg_ues(
        U8 *num_connected_csg_ue,
        U8 *num_connected_non_csg_ue,
        rrm_cell_index_t cell_index
        );

/****************************************************************************
 * Function Name  : rrm_get_low_priority_non_csg_ue
 * Inputs         : UE context
 * Outputs        : ue_index
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function finds the lowest priority
 non csg ue
 ****************************************************************************/
rrm_return_et
rrm_get_low_priority_non_csg_ue(
        rrm_ue_index_t *p_ue_index,
        rrm_ue_index_t ue_index_current
        );

/****************************************************************************
 * Function Name  : rrm_get_low_priority_csg_ue
 * Inputs         : UE context
 * Outputs        : ue_index
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function finds the lowest priority
 csg ue
 ****************************************************************************/
rrm_return_et
rrm_get_low_priority_csg_ue(
        rrm_ue_index_t *p_ue_index,
        rrm_ue_index_t ue_index_current
        );

/****************************************************************************
 * Function Name  : rrm_handle_csg_membership
 * Inputs         : UE context
 *                  ERAB to be setup
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function verifies the csg membership status
 for closed and hybird cell
 ****************************************************************************/
rrm_return_et
rrm_handle_csg_membership(
        rrm_ue_context_t *p_rrm_ue_ctxt
        );

/****************************************************************************
 * Function Name  : rrm_handover_ue_to_best_cell
 * Inputs         : p_ue_context_to_ho,target_cell_access_mode
 * Outputs        : none
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : This function hands over a ue to a closed/open cell
 ****************************************************************************/
rrm_return_et
rrm_handover_ue_to_best_cell(
        rrm_ue_context_t   *p_ue_context_to_ho
    /*SPR 17777 +-*/
        );


/*! \fn rrm_return_et rrm_get_low_priority_non_csg_ue(rrm_ue_index_t *p_ue_index,
rrm_ue_index_t ue_index_current)
*  \brief This function gets low priority non csg ue
*  \param p_ue_index ue index
*  \param ue_index_current current ue index
*/
rrm_return_et
rrm_get_low_priority_non_csg_ue(
        rrm_ue_index_t *p_ue_index, /*outgoing ue_index*/
        rrm_ue_index_t ue_index_current /*ue_index of current ue*/

        );

/*! \fn rrm_return_et rrm_get_low_priority_csg_ue(rrm_ue_index_t *p_ue_index,
rrm_ue_index_t ue_index_current)
*  \brief This function gets low priority  csg ue
*  \param p_ue_index ue index
*  \param ue_index_current current ue index
*/
rrm_return_et
rrm_get_low_priority_csg_ue(
        rrm_ue_index_t *p_ue_index,  /*outgoing ue index*/
        rrm_ue_index_t ue_index_current  /*ue index of curent ue */
        );
/*! \fn rrm_return_et rrm_check_target_cell_access_mode_utran(U16  phy_cell_id_target_cell, rrm_oam_access_mode_et prefered_cell_access_mode, rrm_cell_index_t cell_index, rrm_csg_membership_status_et ue_mbrshp_status_trget_cell)
 *  \brief This function checks target cell utran
 *  \param phy_cell_id_target_cell physical cell id of target cell
 *  \param prefered_cell_access_mode preferred cell access mode
 *  \param cell_index cell index
 *  \param ue_mbrshp_status_trget_cell ue membership target cell
*/

rrm_return_et
rrm_check_target_cell_access_mode_utran(
        U16  phy_cell_id_target_cell,
        rrm_oam_access_mode_et prefered_cell_access_mode,
        rrm_cell_index_t cell_index,
        rrm_csg_membership_status_et ue_mbrshp_status_trget_cell
        );

/* ! \fn rrm_return_et rrm_check_target_cell_access_mode_eutran( U16  phy_cell_id_target_cell, rrm_oam_access_mode_et prefered_cell_access_mode, rrm_cell_index_t  cell_index, rrm_csg_membership_status_et ue_mbrshp_status_trget_cell)
 * \brief This function checks the access mode of the target cell
 * \param phy_cell_id_target_cell physical id of target cell
 * \param prefered_cell_access_mode prefered cell access mode
 * \param cell_index cell index
 * \param ue_mbrshp_status_trget_cell ue membership status
*/
rrm_return_et
rrm_check_target_cell_access_mode_eutran(
        U16  phy_cell_id_target_cell,
        rrm_oam_access_mode_et prefered_cell_access_mode,
        rrm_cell_index_t  cell_index,
        rrm_csg_membership_status_et ue_mbrshp_status_trget_cell
        );


    /*SPR 17777 +-*/
/* SPR 15674 Start */
phy_cell_id_info_t
/* SPR 15674 End */
rrm_get_best_suitable_cell_for_ho(
        rrm_ue_context_t   *p_ue_context_to_ho,
        rrm_oam_access_mode_et prefered_cell_access_mode
        );

/* Fix for CSR 58972 start*/
/* SPR 15674 Start */
phy_cell_id_info_t
/* SPR 15674 End */
rrm_get_best_pci_for_ho 
(
 rrm_ue_context_t      *p_ue_context_to_ho,
 rrm_oam_access_mode_et prefered_cell_access_mode
 );
/* Fix for CSR 58972 end*/

rrm_return_et
rrm_handle_csg_membership_hybrid_for_ho(
        rrm_ue_context_t *p_rrm_ue_ctxt,
        S16                      max_csg_members,
        S16                      max_non_csg_members
        );

rrm_return_et
rrm_handle_csg_membership_hybrid(
        rrm_ue_context_t *p_rrm_ue_ctxt,
        S16                      max_csg_members,
        S16                      max_non_csg_members
        );

rrm_return_et
rrm_handle_csg_membership_closed(
        rrm_ue_context_t *p_rrm_ue_ctxt
        );

rrm_return_et
rrm_handle_csg_membership_open(
        rrm_ue_context_t *p_rrm_ue_ctxt
        );

rrm_return_et
rrm_handle_csg_membership(
        rrm_ue_context_t *p_rrm_ue_ctxt
        );

rrm_return_et
rrm_handle_max_csg_mem_minus_one_hybrid(
        rrm_ue_context_t *p_rrm_ue_context,
        access_mgmt_params_t *p_access_mgmt_params
        );

U8
rrm_get_max_allowed_user(
        rrm_cell_index_t cell_index
        );

rrm_return_et
rrm_hold_ue(
        rrm_ue_context_t *p_ue_context
        );

rrm_return_et
rrm_hold_ue_for_ho(
        rrm_ue_context_t *p_ue_context
        );

rrm_return_et
rrm_handle_max_csg_mem_minus_one_hybrid_for_ho(
        rrm_ue_context_t *p_rrm_ue_context,
        access_mgmt_params_t *p_access_mgmt_params
        );

/* SPR-18654 START */
/* SPR 21367 Fix Start */
rrm_void_t
rrm_get_admitted_bit_rate(
        rrm_cell_context_t *p_cell_context,
        U64                *p_ul_bitrate,
        U64                *p_dl_bitrate
        );

/* Code Removed */
/* SPR 21367 Fix End */
/* SPR-18654 END */

/*CA HARDENING CHANGES*/
/* For Limiting UE measurement */
/*Bug 497 Fix Start */
/******************************************************************************
 *  FUNCTION NAME: rrm_resolve_meas_gap_sr_index_conflict
 *  DESCRIPTION:
 *     This function resolves any conflict between gp_offset and sr_index
 *
 *  RETURNS:
 *     location of gp_offset
 ******************************************************************************/
U8
rrm_resolve_meas_gap_sr_index_conflict(
        U8 location,
        U8 sr_index
        );

/******************************************************************************
 *  FUNCTION NAME: rrm_ue_assign_gap_offset
 *  DESCRIPTION:
 *     This function assign gapoffset and increase count according to it and
 *     maintains offset repository as well
 *
 *  RETURNS:
 *     meas_gap_offset
 ******************************************************************************/
 U8
 rrm_ue_assign_gap_offset(
        rrm_ue_context_t    *p_rrm_ue_context,
        U8    meas_gap_type
        );
 /*Bug 497 Fix End*/
/******************************************************************************
 *  FUNCTION NAME: rrm_ue_release_gap_offset
 *  DESCRIPTION:
 *     This function release gapoffset and decrease count according to it and
 *     maintains offset repository as well
 *
 *  RETURNS:
 *      None
 ******************************************************************************/
 rrm_void_t
 rrm_ue_release_gap_offset(
         rrm_ue_context_t           *p_ue_context
         );

/* For Limiting UE measurement end */ 

/* SPS related changes start */

/*! \fn       rrm_return_et rrm_ue_sps_rab_to_be_established(rrm_ue_context_t *p_rrm_ue_ctxt,rrm_ue_erab_setup_item_t erab_to_be_setup,U8 qci_val,U8 *data_transfer_mode_cnfgd)
*  \brief     This function evaluates whether the requested RAB can be established for SPS or not
*  \param p_rrm_ue_ctxt 
*  \param erab_to_be_setup
*  \param qci_val
*  \param data_transfer_mode_cnfgd
*/
rrm_return_et
rrm_ue_sps_rab_to_be_established(
    rrm_ue_context_t          *p_rrm_ue_ctxt,
    rrm_ue_erab_setup_item_t   erab_to_be_setup,
    U8                         qci_val,
    U8                        *data_transfer_mode_cnfgd
    );

/*! \fn       rrm_ue_reset_sps_info( rrm_ue_context_t *p_rrm_ue_context)
*  \brief     This function resets the UE SPS informations
*  \param[in] p_rrm_ue_context
*/
void
rrm_ue_reset_sps_info(
    rrm_ue_context_t *p_rrm_ue_context
    );

/* SPS related changes end */

rrm_return_et
rrm_ue_ho_attempt_ind 
(
rrm_ue_context_t *p_ue_context
);

rrm_return_et
rrm_ue_ho_failure_report 
(
    rrm_ue_context_t *p_ue_context,
    rrmuem_rmif_ho_cause_et   ho_cause,
    rrm_ue_proc_rlf_ind_t    *p_ue_rlf_ind
);

rrm_void_t
rrm_uem_reset_meas_config_struct(
/* Start: Enh 317 */
        rrm_ue_meas_config_t *p_meas_config
/* End: Enh 317 */        
        );
/* SPR 16042 start */
rrm_void_t
rrm_uem_remove_cgi_report_config(
        rrm_ue_context_t *p_ue_context,
	U8 report_id);
/* SPR 16042 end */

/* Start: Enh 317 */
rrm_void_t
rrm_uem_init_meas_config_struct(
        rrm_ue_context_t *p_ue_context
        );
/* End: Enh 317 */        

/*SPR 18178 Start */
/*Code Removed */
/*SPR 18178 End */

/* SPR 16406 Start */
rrm_void_t
rrm_rank_meas_reprted_cells(
        rrm_ue_context_t *p_rrm_ue_ctxt,
        cell_selection_priority_list_t *p_cell_selection_list
        );
/* SPR 16406 End */

rrm_void_t
rrm_sort_decesnding_based_on_rank(
        cell_selection_priority_list_t  *p_cell_list
        );

rrm_void_t
rrm_sort_and_rank_meas_reported_cell(
        rrm_ue_context_t *p_rrm_ue_ctxt
        );
/*end tgt_cell_ranking*/

/* MRO changes start */
rrm_return_et
rrm_x2ap_ecgi_to_eutran_global_cell_id
(
    rrm_x2ap_ecgi_t x2ap_ecgi,
    rrm_oam_eutran_global_cell_id_t *p_eutran_global_cell_id
);
/* MRO changes end */

/* Bug_944_fix */
rrm_return_et
rrm_redirect_ue_with_release (rrmuem_cm_redirect_rel_ind_t *p_rrm_redirect_ue);

rrm_return_et
rrm_uem_redirect_ue(
        rrm_void_t *p_api /*Input Api buffer*/
    /*SPR 17777 +-*/
        );

/*  DYNAMIC ICIC CHANGES START  */
    /*SPR 17777 +-*/

rrm_return_et
rrm_uem_handle_cm_update_ue_info_req(
        rrm_ue_global_context_t     *p_rrm_ue_glb_ctxt,
        rrm_void_t *p_api /*Input Api buffer*/
    /*SPR 17777 +-*/
        );


rrm_void_t
uem_action_on_icic_reports_in_requested_cell
(
   rrm_ue_global_context_t     *p_rrm_ue_glb_ctxt
);


rrm_void_t
rrm_mac_process_ue_reconfig_cnf_event(RrmMacUEReConfigCnf  *p_ue_reconf_cnf,
                                      rrm_ue_global_context_t    *p_rrm_ue_glb_ctxt,
                                      rrm_cell_index_t             cell_index);

/*  DYNAMIC ICIC CHANGES END  */

/* SPR 15843 Start */
rrm_bool_et
rrm_uem_check_is_cgi_config_pending
(rrm_ue_context_t* p_rrm_ue_ctx
);
/* SPR 15843 End */


/*start LTE RRM Mobility-13*/
/* SPR 15674 Start */
/* code removed here */
/* SPR 15674 End */
rrm_return_et
rrm_match_with_utran_daho_pci(
        rrm_ue_context_t *p_ue_ctxt,
        U16 pci_reported_utran
        );

/* SPR 15674 Start */
rrm_return_et
rrm_match_with_eutran_daho_pci(
        rrm_ue_context_t *p_ue_ctxt,
        U16 pci_reported_eutran,
        U16 earfcn_reported_eutran
        );
/* SPR 15674 End */
/*end LTE RRM Mobility-13*/

rrm_bool_et
rrm_ue_is_valid_ecid_meas_request (
    rrm_ue_context_t *p_ue_context,
    rrm_ue_lppa_ecid_meas_init_req_t *p_rrm_lppa_ecid_meas_init_req);

rrm_void_t
rrm_fill_eutran_access_point(
    rrm_cell_index_t cell_index,
    lppa_e_cid_meas_result_t *p_ecid_meas_result);

void
rrm_clean_ue_all_ecid_meas_related_info(
    rrm_ue_context_t       *p_ue_context
    );

rrm_return_et
rrm_clean_ue_ecid_meas_related_info(
    rrm_ue_context_t       *p_ue_context,
    U8                    eslmc_meas_id
);
rrm_return_et
rrm_fill_lppa_rsrq_meas_result(rrm_cell_index_t               cell_index,
                          lppa_measured_res_t                 *p_ecid_meas_result,
                          rrm_ue_measurment_results_ind_t     *p_ue_meas_results);

rrm_return_et
rrm_fill_lppa_rsrp_meas_result(rrm_cell_index_t               cell_index,
                          lppa_measured_res_t                 *p_ecid_meas_result,
                          rrm_ue_measurment_results_ind_t     *p_ue_meas_results);

rrm_return_et
rrm_fill_serv_cell_ecid_info(
    rrm_cell_index_t                     cell_index,
    lppa_e_cid_meas_result_t             *p_lppa_ecid_meas_result
    );

rrm_return_et
rrm_convert_rrm_ecgi_into_rrc_ecgi (
        rrm_global_cell_id_t            rrm_ecgi,
        lppa_ecgi_t                     *p_rrc_ecgi
        );
/*Bug_721_changes_start*/
U8
rrm_ue_find_erb_index
(
 rrm_ue_erb_t *p_ue_erb
 );

void
rrm_ue_update_erb_priority_list_flag
(
 rrm_ue_erb_list_t * erb_release_list
 );
/*Bug_721_changes_end*/
rrm_void_t
rrm_fill_lppa_rsrp_meas_result_for_serving_cell(
        rrm_cell_context_t                  *p_cell_ctxt,
        lppa_measured_res_t                 *p_ecid_meas_result,
        rrm_ue_measurment_results_ind_t     *p_ue_meas_results
        );

rrm_void_t
rrm_fill_lppa_rsrq_meas_result_for_serving_cell(
        rrm_cell_context_t                  *p_cell_ctxt,
        lppa_measured_res_t                 *p_ecid_meas_result,
        rrm_ue_measurment_results_ind_t     *p_ue_meas_results
        );

void
rrm_ue_delete_meas_id_info_for_ecid_meas(
    rrm_ue_context_t          *p_rrm_ue_ctx,
    U8                         ecid_meas_index
    );

rrm_return_et
rrm_mac_process_ecid_meas_report_resp_event(
        rrm_cell_index_t                   cell_index,
        rrm_ue_global_context_t           *p_rrm_ue_glb_ctxt,
        MacRrmEcidMeasurementResp   *p_mac_ecid_meas_report_resp
        );

rrm_return_et
rrm_fill_lppa_rsrp_meas_result_from_mac_resp(
                rrm_cell_index_t                 cell_index,
                lppa_measured_res_t             *p_ecid_meas_result,
                rrm_ue_context_t                *p_ue_context,
                U8                               ecid_meas_req_index
                );

rrm_return_et
rrm_fill_lppa_rsrq_meas_result_from_mac_resp(
                rrm_cell_index_t                 cell_index,
                lppa_measured_res_t             *p_ecid_meas_result,
                rrm_ue_context_t                *p_ue_context,
                U8                               ecid_meas_req_index
                );

rrm_void_t
rrm_fill_rsrp_for_serving_cell_from_mac_resp(
        rrm_cell_context_t     *p_cell_ctxt,
        lppa_measured_res_t    *p_ecid_meas_result,
        rrm_ue_context_t       *p_ue_context,
        U8                      ecid_meas_req_index
        );

rrm_void_t
rrm_fill_rsrq_for_serving_cell_from_mac_resp(
        rrm_cell_context_t     *p_cell_ctxt,
        lppa_measured_res_t    *p_ecid_meas_result,
        rrm_ue_context_t       *p_ue_context,
        U8                      ecid_meas_req_index
        );

rrm_return_et
rrm_ue_meas_config_remove_for_ecid(
    rrm_ue_context_t    *p_ue_context,
    U8                   ecid_meas_req_index,
    rrm_bool_et          remove_all_ecid_ue_meas_config
);

rrm_return_et
rrm_ue_mgr_enqueue_esmlc_meas_req(
 /* SPR 20076 Fix Start */
        rrm_ue_procedure_queue_t *p_api_buf,
 /* SPR 20076 Fix End */
        rrm_ue_context_t   *p_ue_context
        );

rrm_return_et
rrm_ue_handle_dequeued_esmlc_meas_req(
        U16  api_id,
        rrm_ue_global_context_t *p_rrm_glb_ctxt
        );

rrm_return_et
rrm_ue_clean_esmlc_meas_queue(
        rrm_ue_context_t    *p_ue_context
        );

rrm_return_et
store_mac_meas_results_in_ue_context(
    rrm_ue_context_t                  *p_ue_context,
    U8                                 ecid_meas_index,
    MacRrmEcidMeasurementResp   *p_mac_ecid_meas_report_resp
    );

rrm_return_et
rrm_ue_build_and_send_combined_ecid_meas_report(
    rrm_ue_context_t                        *p_ue_context,
    s1ap_rrm_lppa_ecid_meas_report_t     *p_lppa_ecid_meas_report,
    U8                                    ecid_meas_req_index
    );

rrm_return_et
check_if_requested_quantifier_is_received(
    rrm_ue_context_t          *p_ue_cntxt,
    U8                         esmlc_meas_id
    );
/* RACH_OPTIMIZATION_CHANGES_START */
void
store_rach_info_in_global_list
(
 rrm_ue_global_context_t     *p_rrm_ue_glb_ctxt,
 rrm_ue_context_t            *p_ue_ctxt,
 rrm_bool_et                 strong_cell_info_reqd,
 /*BUG_965_CHANGES_START*/
 U8                          count
 /*BUG_965_CHANGES_END*/
 );
/* SPR 20439 Fix Start */
rrm_return_et
post_stored_data_to_son
(
 rrm_ue_global_context_t     *p_rrm_ue_glb_ctxt,
 rrm_cell_index_t    cell_index
 );
/* SPR 20439 Fix End */
rrm_return_et
rrm_ue_handle_lipa_bearer_release
(
    rrm_ue_context_t  *p_ue_context,
    rrm_ue_lipa_release_et       *is_rab_released
);

/* RACH_OPTIMIZATION_CHANGES_END */

/* LIPA_SIPTO_START */

/*! \fn         rrm_return_et rrm_ue_delete_erb_entity( rrm_ue_erb_t *ue_erab, U8 erb_index )
 *  \brief      this funtion will delete the rab entity from erb priority list for the given erab_index.
 *  \param[in]  rrm_ue_erab_t
 */

rrm_return_et
rrm_ue_delete_erb_entity_per_rab_index
(
    rrm_ue_erb_t *p_ue_erab, 
    U8 erb_index
);
/* LIPA_SIPTO_END */

/*spr 7466 start*/
void
clean_resources_for_the_bearer
(
 rrm_ue_erb_t erb_data
 );

void
rrm_ue_rac_clean_cell_prb_resources
(
 rrm_ue_context_t *p_rrm_ue_ctxt
 );
/*spr 7466 end*/

/*spr 7996 start*/
rrm_return_et
rrm_release_ue(
        rrm_ue_context_t   *p_ue_context
        );
/*spr 7996 end*/
/* CSR 55938: code changes start */
signed long
uerrm_get_ue_elapsed_time (
              rrm_ue_context_t* p_ue_context
                            );
/* CSR 55938: code changes end */

rrm_return_et
rrm_get_num_of_layer_on_tx_mode_and_ue_category_4xn( transmission_mode_ue_service_profile_et
                                                                             tx_mode,
                                                     rrm_ue_category_et      ue_category,
                                                     U8                      *num_of_layer,
                                                     rrm_antenna_mxn_et       mxn_support );
rrm_return_et
rrm_get_num_of_layer_on_tx_mode_basis_for_2x2( transmission_mode_ue_service_profile_et
                                                                   tx_mode,
                                               U8                  *num_of_layer );
rrm_return_et
rrm_get_num_of_layer_for_ue_on_tx_mode_mxn( rrm_antenna_mxn_et         mxn_support,
                                            transmission_mode_ue_service_profile_et
                                                                       transmission_mode,
                                            rrm_ue_category_et         ue_category,
                                            U8                         *num_of_layer );

rrm_return_et
rrm_select_num_of_layer_for_ue( const rrm_ue_context_t *p_ue_context,
                                U8                     *num_of_layer );

U8 rrm_get_ue_category(const rrm_ue_context_t *p_ue_context);

/* Bug_944_start */
/*! \fn rrm_find_daho_cell_freq (
             rrm_daho_cell_config_info_t *p_daho_cell,
 *           rrm_cell_context_t    *p_cell_context)
 *  \brief This function find DAHO cell frequency
 *  \param p_daho_cell DAHO cell info
 *  \param p_cell_context cell context
*/
U16
rrm_find_daho_cell_freq (
        rrm_daho_cell_config_info_t *p_daho_cell, 
        rrm_cell_context_t *p_cell_context);

/*! \fn rrm_find_load_threshold (
 *           rrm_cell_context_t    *p_cell_context)
 *  \brief This function find load threshold from load set by
 *   OAM 
 *  \param p_cell_context cell context
*/
U8
rrm_find_load_threshold (
        rrm_cell_context_t *p_cell_context);

/*! \fn rrm_sort_freq_of_neighbour_cell_on_load (
 *           rrm_ue_context_t      *p_ue_context,
 *           rrm_cell_context_t    *p_cell_context)
 *  \brief This function find less load freq from the list
 *  of neighbour cell 
 *  \param p_ue_context ue context
 *  \param p_cell_context cell context
*/

U16
rrm_sort_freq_of_neighbour_cell_on_load (
            rrm_ue_context_t      *p_ue_context,
            rrm_cell_context_t    *p_cell_context);


/*! \fn rrm_fnd_highst_frq_frm_idle_mode_prms (
 *           rrm_ue_context_t      *p_ue_context,
 *           rrm_cell_context_t    *p_cell_context)
 *  \brief This function find highest priority frequency
 *  from idle mode mobility params 
 *  \param p_ue_context ue context
 *  \param p_cell_context cell context
*/

U16
rrm_fnd_highst_frq_frm_idle_mode_prms (
    rrm_ue_context_t      *p_ue_context,    
    rrm_cell_context_t    *p_cell_context);

/*! \fn rrm_fnd_highst_frq_frm_idle_mode_prms (
 *          rrm_csg_membership_status_et ue_csg_status,
 *          inter_freq_cells_t *p_int_freq_cell
 *  \brief This function find highest priority frequency
 *  from idle mode mobility params 
 *  \param ue_csg_status CSG status
 *  \param p_int_freq_cell inter freq cell information
*/
rrm_bool_et rrm_inter_cell_valid_for_selection(
        rrm_csg_membership_status_et ue_csg_status,
        inter_freq_cells_t *p_int_freq_cell,
        rrm_bool_et        is_meas_for_masa
        );
/* Bug_944_end */

/*! \fn rrm_uem_configure_utran_srvcc(
 *  rrm_ue_context_t            *p_ue_ctxt,
    rrm_srvcc_ue_criteria_et    ue_srvcc_criteria,
    rrm_bool_et                 ps_ho_supported,
    rrm_bool_et                 voip_capable
 *  \brief Checks the UTRAN target cell capailities for HO(SRVCC/Normal HO)
 *      Also, identifies the type of HO: CS, PS
 */
rrm_return_et
rrm_uem_configure_utran_srvcc(
    rrm_ue_context_t            *p_ue_ctxt,
    rrm_srvcc_ue_criteria_et    ue_srvcc_criteria,
    rrm_bool_et                 ps_ho_supported,
    /*SPR_15241_Start*/
    rrm_bool_et                 voip_capable,
    rrm_bool_et                 ue_support_voip
    /*SPR_15241_End*/
    );


/*Start:Bug 762*/
rrm_bool_et
rrm_is_set_fgi_bit(
   rrm_ue_context_t  *p_ue_context,
   U32               bit
);
/*End:Bug 762*/
/* CA Changes : Start */
rrm_return_et
rrm_ue_set_phy_antenna_info_r10(rrm_ue_index_t      ue_index,
                                rrm_cell_index_t    cell_index,
                                const rrm_ue_phy_antenna_info_r10_t *p_ue_antenna_info);

rrm_return_et
rrm_fill_ue_scell_rollback_info_del_fail(rrm_ue_context_t *p_ue_context, U8 count);

rrm_return_et
rrm_send_ue_reconfig_on_rr_config_dedicated_param_upd(
rrm_cell_index_t        pcell_index,
rrm_cell_index_t        scell_index,
U64                     ca_bitmask);
/* CA Changes : End */

/* Carrier Aggregation start*/
S16  rrm_function_check_empty_tb_block( void );

/* BUG_11907_FIX_START */
S16 rrm_maintain_transaction_block( register_handler handler, void* cell_stop_params );
/* BUG_11907_FIEND */

void rrm_invoke_tb_handler(S16  transaction_id ,rrm_cell_index_t cell_index, void* p_api);

/* BUG_11907_FIX_START */
rrm_return_et
rrm_mac_ue_reconfig_cnf_scell_activation_or_deactivation_action( rrm_cell_index_t            cell_index,
                                                                 void                        *p_api,
                                                                 void                        *p_gb_context,
                                                                 void                        *cell_stop_params);
/* BUG_11907_FIX_END */
rrm_void_t rrm_find_ca_deactivation_activation_param_chaged( rrm_cell_index_t   pcell_index,
                                                              rrm_cell_index_t   scell_index,
                                                              U64                 ca_bitmap,
                                                              /*SPR 13316:start*/
                                                               U16                trans_id);
                                                              /*SPR 13316:end*/
/* BUG_11907_FIX_START */
rrm_return_et
rrm_perform_cell_deactivation_on_ca_configuration_change( rrm_cell_index_t            pcell_index,
                                                          rrm_cell_index_t            scell_index ,
                                                          U16                         num_user,
                                                          rrm_ue_reconfig_type_et     reconfig_type, 
                                                          /*SPR 13316:start*/
                                                          U16                         trans_id);
                                                              /*SPR 13316:end*/
/* BUG_11907_FIX_END */
rrm_return_et rrm_perform_carrier_aggregation_related_action(rrm_cell_change_notification_object_t *
                                                                        p_cell_ca_modfication_obj );


rrm_bool_et
rrm_was_any_scell_activated_for_ue( const rrm_ue_context_t    *p_ue_context,
                                    rrm_bool_et                rrm_scenario_flag);


rrm_return_et
rrm_mac_ue_reconfig_scell_reactivation_on_ho_out_failure( rrm_ue_context_t    *p_ue_context,
                                                          rrm_bool_et                rrm_scenario_flag);
/* Carrier Aggreagtion end */
/* CSR-00058598 and SPR-837-fix start*/
/*! \fn rrm_check_ue_arp_val_for_emergency_ho_admin(
 *   rrm_ue_context_t    *p_rrm_ue_context
 *
 *  \ check ARP value for emergency ho admin
 */
rrm_return_et
rrm_check_ue_arp_val_for_emergency_ho_admin (
        rrm_ue_context_t    *p_rrm_ue_context
        );
/* SPR 17564 Fix Start */
/*! \fn rrm_is_ue_special_previledge(
 *   rrm_ue_context_t    *p_rrm_ue_context
 *
 *  \ check ARP value for emergency during 
 *  \ eRAB Setup and Modify
 */
rrm_bool_et
rrm_is_ue_special_previledge (
        rrm_ue_context_t    *p_rrm_ue_context
        );
/* SPR 17564 Fix End */

/*SPR 21687 Start */
/*! \fn is_ue_restricted_for_given_rat(
 *   rrm_ue_context_t    *p_rrm_ue_context
 *
 *  \ check is UE restricted for the given rat_type 
 */
rrm_bool_et
is_ue_restricted_for_given_rat (
        rrm_ue_context_t    *p_rrm_ue_context,
        rrm_rat_type_et     rat_type
        );
/*SPR 21687 Start */

/*! \fn rrm_ue_emergency_establishment_operation_for_ho(
 *  U16               trans_id,
 *  rrm_cell_index_t   cell_index
 *
 *  \ emergency establishment operation handle for open and close cell
 */
rrm_return_et
rrm_ue_emergency_establishment_operation_for_ho (
        U16               trans_id,
        rrm_cell_index_t   cell_index
        );

/*! \fn rrm_handle_max_mem_in_hybrid_for_emergency_ho(
 *   U16               trans_id,
 *   rrm_cell_index_t   cell_index,
 *   rrm_ue_context_t *p_rrm_ue_context
 *
 *  \ emergency establishment operation handle for hybrid cell
 */
rrm_return_et
rrm_handle_max_mem_in_hybrid_for_emergency_ho(
        U16               trans_id,
        rrm_cell_index_t   cell_index,
        rrm_ue_context_t *p_rrm_ue_context
        );
/* CSR-00058598 and SPR-837-fix end */

/* CA_Stage_3_Scell_TM_change: Start */
rrm_return_et
rrm_get_scell_index_for_scell_cell_index( rrm_ue_context_t *p_rrm_ue_context,
                                          rrm_cell_index_t  scell_index,
                                          U8               *index,
                                          U8               *report_count);

rrm_return_et
rrm_backup_tm_scell_info(rrm_ue_context_t *p_rrm_ue_context,
                         rrm_cell_index_t  scell_index);

const rrm_scell_antenna_info_dedicated_r10_t*
rrm_ue_get_scell_phy_antenna_info_r10(rrm_ue_context_t *p_rrm_ue_context,
                                rrm_cell_index_t  scell_index);

rrm_return_et
rrm_compare_scell_ue_antenna_info_r10(
      rrm_ue_context_t                       *p_rrm_ue_context,
      const rrc_antenna_info_dedicated_r10_t *p_antenna_info_dedicated_r10,
      rrm_cell_index_t                        scell_index,
      rrm_bool_et                            *p_antenna_info_match);

rrm_return_et
rrm_is_scell_valid_for_ue(rrm_ue_context_t   *p_rrm_ue_context,
                          rrm_cell_index_t    scell_index,
                          U8                 *scell_arr_idx);

rrm_return_et
rrm_is_scell_index_valid_for_ue(rrm_ue_context_t   *p_rrm_ue_context,
                                U8                  scell_idx_on_ue,
                                U8                 *scell_arr_idx);

rrm_return_et
rrm_set_scell_tm_info_r10( rrm_ue_context_t            *p_rrm_ue_context,
                           rrm_cell_index_t             scell_index,
                           const rrc_antenna_info_dedicated_r10_t
                                                       *p_antenna_info_dedicated_r10);
rrm_return_et
rrm_caclulate_avg_sinr_for_scell( rrm_ue_context_t *p_ue_context,
        U16               report_index,
        U8                valid_scell_dl_sinr_cw1,
        U8                valid_scell_dl_sinr_cw0_layer2,
        U8                valid_scell_dl_sinr_cw1_layer2,
        U32               scell_total_downlinkSINRValueCW0,
        U32               scell_total_downlinkSINRValueCW1,
        U32               scell_total_downlinkSINRValueCW0Layers2,
        U32               scell_total_downlinkSINRValueCW1Layers2 );


/* CA_Stage_3_Scell_TM_change: End */
/* CA: Stage3 HO code start */
rrm_return_et
rrm_check_and_update_ue_scell_list(
            rrm_ue_context_t *p_ue_context,
            rrm_cell_context_t *p_cell_context,
            rrm_cell_index_t  *p_ret_list
            );
/* CA: Stage3 HO code end */

/* BUG_11907_FIX_START */
/*! \fn rrm_send_cell_stop_broadcast_resp
 *          rrm_cell_index_t  pcell_index , 
 *          rrm_cell_index_t  scell_index ,
 *          rrm_return_et     status
 *   
 *
 *  \ send response for cell stop broadcast api
 */

rrm_return_et
rrm_send_cell_stop_broadcast_resp( rrm_cell_index_t  pcell_index ,
        rrm_cell_index_t  scell_index ,
        rrm_return_et     status, 
        /*SPR 13316:start*/
        U16               trans_id);
       /*SPR 13316:end*/

/*! \fn rrm_send_cell_info_change_ind_resp 
 *  rrm_cell_index_t  pcell_index ,
 *  rrm_cell_index_t  scell_index ,
 *  rrm_return_et     status  
 *
 *  \ send resp for cell delete broadcast api 
 */

rrm_return_et
rrm_send_cell_info_change_ind_resp( rrm_cell_index_t  pcell_index ,
        rrm_cell_index_t  scell_index ,
        rrm_return_et     status,
/*SPR 13316:start*/
        U16               trans_id);
/*SPR 13316:end*/

/* BUG_11907_FIX_END */

/* SPR 10498 Fix Start */
rrm_void_t
rrm_uem_find_eutran_daho_cell(
        rrm_ue_context_t                *p_ue_ctxt,
        rrm_daho_cell_config_info_t     *p_daho_cfg,
        U32                             earfcn,
        U16                             *p_daho_pci
        );

rrm_eutran_inter_freq_daho_cell_config_info_t *
rrm_uem_search_inter_freq_daho_cell(
        rrm_ue_context_t                *p_ue_ctxt,
        rrm_daho_cell_config_info_t     *p_daho_cfg,
        U16                             earfcn,
        U16                             *p_daho_pci
        );
/* SPR 10498 Fix End */        


/* SPR 14672 Fix start */
rrm_void_t rrm_fill_rohc_config_ue_ctxt(rrm_ue_context_t *p_ue_context,
        rrm_ue_max_num_rohc_context_session_et  max_rohc_sessions);
/* SPR 14672 Fix End */

/* eICIC_PHASE_1_2_CHANGES_START */
rrm_return_et
rrm_uem_eicic_meas_config_ind(rrm_ue_global_context_t *p_g_rrm_ue_ctx);


rrm_return_et
/*SPR 20687 Changes Start */
rrm_uem_update_aggressor_node_list
(rrm_son_eutran_neighbor_cell_t * eutran_neighbor, rrm_ue_global_context_t *p_ue_glbl_ctxt,U8 count);
/*SPR 20687 Changes Stop */

rrm_return_et
rrm_uem_handle_cm_get_aggressor_req(
        rrm_ue_global_context_t     *p_rrm_glb_ctxt,
        rrm_void_t *p_api 
        );

rrm_return_et
rrm_uem_send_aggressor_node_list_to_cellm
(   
 YLIST *aggressor_node_list,
 UInt16 trans_id,
 rrm_cell_index_t cell_index
 );

const rrm_void_t *
rrm_uem_count_key_of_aggressor_node_list
(
    const YLNODE *p_node
);

SInt32
rrm_uem_aggressor_list_compare_count
(
    const rrm_void_t *p_val_1,
    const rrm_void_t *p_val_2
);

rrm_return_et
rrm_uem_handle_eicic_provision_req
(
 rrm_ue_global_context_t *p_rrm_glb_ctxt, 
 void *p_api_buf
 );

rrm_return_et
rrm_uem_handle_reset_scenario_for_eicic
(
 	rrm_ue_global_context_t *p_rrm_glb_ctxt 
 );

/*SPR 14960 start */
rrm_void_t
rrm_uem_flush_victim_and_aggressor_node
(
    rrm_ue_global_context_t *p_rrm_glb_ctxt
    );
/*SPR 14960 end */
/*SPR 15663 start*/
rrm_void_t rrm_calculate_sr_offset(
        U8                 cell_index,
        U16                *p_sr_offset,
        U8                 sr_config_index
        );
/*SPR 15663 end*/
/* eICIC_PHASE_1_2_CHANGES_END */
/*SPR Fix 15664 Start*/
rrm_void_t
rrm_uem_convert_abs_pattern_to_binary
(
    U8     *abs_pattern, 
    U8     *p_abs_binary_format
);
U8
rrm_uem_calculate_meas_off_set
(
    U8                      *p_meas_gap_offset,
    rrm_ue_context_t        *p_rrm_ue_ctx
);
U8 
rrm_uem_align_meas_offset_with_eicic
( 
    U8       *p_meas_offset, 
    U8      *p_abs_binary_format,
    U8        sf_status
);
/*SPR Fix 15664 end*/
rrm_bool_et
rrm_check_absPattern_is_null
(
    U8      *p_abs_pattern
);

/* SPR 15539 end */
/* SPR 16406 8SEP start */
rrmc_meas_report_event_et
rrm_ue_get_configured_meas_event (
        U8                        meas_id,         /* IN  MeasID obtained in meas res ind */
        rrm_ue_context_t*         p_ue_context);/* IN  ue Context Pointer */
/* SPR 16406 8SEP end */


/*SPR Fix 16079 fix start */
rrm_bool_et
rrm_check_is_ho_ongoing_or_meas_ongoing
(
 rrm_ue_context_t    *p_rrm_ue_context
 );
/*SPR Fix 16079 fix end */
/* SPR 19968 START */
rrm_void_t
    get_earlier_gbr_bitrate
(
 rrm_ue_erab_setup_item_list_t *p_drb_configured,
 U8 erab_id,
 U64 *ul_bitrate,
 U64 *dl_bitrate,
 U8  *old_qci
);


rrm_return_et
rac_release_gbr_bearer
(
 rrm_ue_context_t   *p_rrm_ue_ctxt,
 rrm_ue_erab_setup_item_t erab_to_modify,
 U8                 gbr_dl_prbs_released,
 U8                 gbr_ul_prbs_released
);


/* SPR-15991_2 END */
/*SPR 16060 Fix Start*/

void
rrm_ue_calc_total_prb_to_be_rlsd
(
 rrm_ue_context_t *p_rrm_ue_ctxt,
 U64          *p_total_deallocated_dl_prbs,
 U64              *p_total_deallocated_ul_prbs
 );

/*SPR 16060 Fix End*/
/*SPR 19968 Fix End*/


/* SPR 16406 Start */

rrm_void_t
    rrm_delete_entry_in_ho_cgi_pending_list
(
 rrm_ue_context_t *p_ue_context,
 U16 pci,
 U16 earfcn
 );

rrm_void_t rrm_remove_pci_from_cell_selection_list(

        rrm_ue_context_t *p_ue_context,
        cell_selection_priority_list_t *p_cell_selection,
        U16 pci_for_which_cgi_required,
        U16 carrier_earfcn
    );

/* SPR 16406 End */

/* SPR 16406 21AUG Start */
U8
rrm_get_report_id_for_configured_cgi
(
 rrm_ue_context_t *p_ue_context	
 );

U8
rrm_get_meas_id_for_configured_cgi
(
 rrm_ue_context_t *p_ue_context	
 );
/* SPR 16406 21AUG End */
/* SPR 16406 8SEP start */
rrm_bool_et
rrm_is_set_fgi_bit(
    rrm_ue_context_t  *p_ue_context,
    U32               bit);
rrm_bool_et 
rrm_is_anr_allowed_on_eutran_freq(
        rrm_ue_context_t *p_ue_context,
        U32     earfcn);
/* SPR 16406 8SEP end */


/* SPR 16406 3Aug Start */
rrm_return_et
rrm_uem_remove_rsc_and_cgi_pending_nodes
(
    rrm_ue_context_t *p_ue_context
);
/* SPR 16406 3Aug End */
/* SPR 19968 START */
rrm_void_t
    get_earlier_gbr_bitrate
(
 rrm_ue_erab_setup_item_list_t *p_drb_configured,
 U8 erab_id,
 U64 *p_ul_bitrate,
 U64 *p_dl_bitrate,
 U8  *p_old_qci
);

rrm_return_et
rac_release_ngbr_bearer(
        rrm_ue_context_t    *p_rrm_ue_ctxt,
        U8                  ngbr_dl_prbs_released,
        U8                  ngbr_ul_prbs_released,
        U64                  dl_allocated_ngbr_prbs,
        U64                 ul_allocated_ngbr_prbs
);

rrm_return_et
rac_release_gbr_bearer
(
 rrm_ue_context_t   *p_rrm_ue_ctxt,
 rrm_ue_erab_setup_item_t erab_to_modify,
 U8                 gbr_dl_prbs_released,
 U8                 gbr_ul_prbs_released
);

/* SPR 19968 END */

/* SPR 17784 Start */

cell_selection_priority_list_t*
rrm_uem_get_active_cell_selection_list
(   
 rrm_ue_context_t *p_ue_context
 );

rrm_return_et
rrm_remove_previous_cell_for_ho(
    rrm_ue_context_t *p_ue_context,
    U16 pci,
    U16 carrier_earfcn);

rrm_void_t 
rrm_remove_pci_from_cell_selection_list(
    rrm_ue_context_t *p_ue_context,
    cell_selection_priority_list_t *p_cell_selection,
    U16               pci_for_which_cgi_required,
    U16               carrier_earfcn
    );  

rrm_return_et 
rrm_ue_process_cgi_timer_expiry(
    rrm_ue_global_context_t *p_rrm_glb_ctxt);

/* SPR 17784 End */


#ifdef LTE_EMBMS_SUPPORTED
rrm_void_t
rrm_find_free_drx_offset_frm_drx_res_tree
(
  drx_res_alloc_tree_per_ue_t *p_drx_res_tree,
  U8                           total_drx_offset,
  U16                         *p_drx_start_offset
);
rrm_return_et
file_erb_setup_resp_long_drx_cycle_start_offset
(
    rrc_long_drx_cycle_start_offset_t     *p_rrc_long_drx_cycle_start_offset,
    rrm_oam_long_drx_cycle_start_offset_et long_drx_cycle,
    U16                                    drx_start_offset
);

#endif

#ifdef ENDC_ENABLED
rrm_return_et
file_erb_setup_resp_long_drx_cycle_start_offset_r15
(
    rrc_long_drx_cycle_start_offset_r15_t     *p_rrc_long_drx_cycle_start_offset,
    rrm_oam_long_drx_cycle_start_offset_r15_et    long_drx_cycle,
    U16                                       drx_start_offset
);
#endif
/* SPR 20653 Fix Start */
rrm_void_t
get_rat_priority
(
 rrm_oam_rat_priority_list_t  *p_rrm_rat_priority_list,
 rrm_oam_rat_priority_info_et *p_get_eutran_priority,
 rrm_oam_rat_priority_info_et *p_get_utran_priority,
 rrm_oam_rat_priority_info_et *p_get_geran_priority,
 rrm_oam_rat_priority_info_et *p_get_cdma_priority
);
/* SPR 20653 Fix End */
/* SPR 20652 Fix Start */
rrm_void_t
rrm_ue_get_first_node_ctxt
(
    rrm_seq_data_info_t *p_ue_node,
    rrm_ue_context_t    **p_ue_context,
    rrm_cell_index_t    cell_index
);

/* Spr 18748 Fix Start */
rrm_bool_et
rrm_check_if_sec_meas_configured
(
  rrm_ue_context_t *p_ue_context
);
/* Spr 18748 Fix End */

rrm_seq_data_info_t
rrm_ue_get_next_node_ctxt
(
    rrm_seq_data_info_t *p_ue_node,
    rrm_ue_context_t    **p_ue_context,
    rrm_cell_index_t    cell_index
);
/* SPR 20652 Fix End */
rrm_void_t rrm_print_fgi_bits(rrm_ue_context_t  *p_ue_context);

/* SPR 21496 Fix Start */
rrm_void_t
rrm_ue_release_ue_capability_params
(
    rrm_ue_context_t *p_ue_context
);

rrm_void_t
rrm_ue_rrc_parse_ho_adm_req_eutra_radio_capability
(
    rrc_rrm_ue_ho_adm_req_t     *p_rrc_rrm_ho_adm_req ,
    rrm_ue_context_t            *p_ue_context
);
/* SPR 21496 Fix End */
/* SPR 21614 start */
rrm_bool_et rrm_is_update_required_in_cell_select(
    rrm_cell_context_t    *p_cell_ctxt,
    rrm_son_utran_physical_cell_id_t *p_pci);
/* SPR 21614 end */

#endif 
