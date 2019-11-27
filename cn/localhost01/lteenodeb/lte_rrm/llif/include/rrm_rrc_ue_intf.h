/*!
 * \file        rrm_rrc_ue_intf.h
 * \author      gur26991
 * \date        2 March 2012
 * \brief       This file contains the prototypes for Response filling and 
 *              and parsing for RRC layer abstraction.
 */
/*!
 *  copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 *
 */

#ifndef _RRM_UE_RRC_INTF_H_
#define _RRM_UE_RRC_INTF_H_

/*! \headerfile cspl.h <>
 */
#include "cspl.h"

/*! \headerfile rrm_defines.h <>
 */
#include "rrm_defines.h"

/*! \headerfile rrm_ue_context.h <> 
 */
#include "rrm_ue_context.h"

/*! \headerfile rrm_cell_context.h <>
 */
#include "rrm_cell_context.h"

/*! \headerfile rrm_types.h <>
 */
#include "rrm_types.h"

/*! \fn U16 rrm_return_end_tpc_rnti_pucch(rrm_cell_index_t cell_index)
 *  \brief  This function return end tpc rnti pucch
 *  \param  cell_index Cell Index
 */
U16
rrm_return_end_tpc_rnti_pucch(
        rrm_cell_index_t cell_index
        );
/*LTE_RRM_TM_MODE_FEATURE*/
#define STANDARD_PACKET_DELAY_LOW_BOUNDARY  50
#define STANDARD_PACKET_DELAY_HIGH_BOUNDARY 100
#define STANDARD_PACKET_ERROR_LOSS_RATE	    1
#define MAX_TM_MODE_SUPPORTED               8
#define MAX_APERIODIC_MODE                  3

#define RRM_N2_TX_CODEBOOK_SUBSET_VAL_TM3   0xC0
#define RRM_N2_TX_CODEBOOK_SUBSET_VAL_TM4   0xFC
#define RRM_N2_TX_CODEBOOK_SUBSET_VAL_TM5   0xF0
#define RRM_N2_TX_CODEBOOK_SUBSET_VAL_TM6   0xF0
/*LTE_RRM_TM_MODE_FEATURE*/

/* SPR 16893 Fix Start */
#define RRM_MEAS_GP0_PERIODICITY            40
#define RRM_MEAS_GP1_PERIODICITY            80
/* SPR 16893 Fix Stop */ 


/*! \fn U16 rrm_return_end_tpc_rnti_pusch(rrm_cell_index_t cell_index)
 *  \brief to return end tpc rnti pusch
 *  \param cell_index
 */
U16
rrm_return_end_tpc_rnti_pusch(
        rrm_cell_index_t cell_index
        );
/*! \fn U16 rrm_return_start_tpc_rnti_pusch(rrm_cell_index_t cell_index)
 *  \brief This function return start tpc rnti pusch
 *  \param  cell_index Cell Index
 */
U16
rrm_return_start_tpc_rnti_pusch(
        rrm_cell_index_t cell_index
        );

/*! \fn U16 rrm_return_start_tpc_rnti_pucch(rrm_cell_index_t cell_index)
 * \brief to return start tpc rnti pucch 
 * \param cell_index Cell Index
 */
U16
rrm_return_start_tpc_rnti_pucch(
        rrm_cell_index_t cell_index);

/*SPR 21848 Start*/
rrm_void_t
rrm_release_ho_restriction_list
(rrm_ho_restriction_list_t   * p_ho_restriction_list);

rrm_void_t
rrm_ue_fill_restriction_list_from_proc_data_to_ue_context
(
rrm_ho_restriction_list_t  * p_rrm_ho_restriction_list,
rrm_ho_restriction_list_t   *p_rrc_ho_restriction_list
);
/*SPR 21848 End*/

/*! \ rrm_ue_rrc_msg_t
 *  \brief This struct is used for RRC message parameters
 *  \param  ue_index UE Index
 *  \param  cell_index Index of the Cell
 *  \param  p_msg_data Message data
 *  \param  sinr_adm_req SINR admin Request
 *  \param  num_of_inactive_ue No of inactive UE 
 *  \param  num_of_inactive_ue_list[10] Number of Inactive UE List
 */
typedef struct _rrm_ue_rrc_msg_t
{
    rrm_ue_index_t   ue_index;		/*!< Ue index for RRC messages 		*/
    rrm_cell_index_t cell_index;		/*!< Cell index for RRC messages	*/ 
    void	*p_msg_data;	/*!< Actual message data			*/
    U8       sinr_adm_req;
    /* +- SPR 18268 */
    U16       num_of_inactive_ue;
    U16       num_of_inactive_ue_list[10];
    /* +- SPR 18268 */
}rrm_ue_rrc_msg_t;


/*! \fn  rrm_return_et rrm_rrc_process_ue_transmission_mode_selection(const rrm_ue_context_t *p_rrm_ue_context,rrc_phy_antenna_information_dedicated_t *p_rrm_antenna_information)
 *  \brief          to select the transmission mode of ue.
 *  \param  p_rrm_ue_context UE Context Information
 *  \param  p_rrm_antenna_information Antenna Information
 */
rrm_return_et
rrm_rrc_process_ue_transmission_mode_selection(
        const   rrm_ue_context_t                *p_rrm_ue_context,
        rrc_phy_antenna_information_dedicated_t *p_rrm_antenna_information
        );


/*! \fn	rrm_return_et rrm_build_and_send_ue_admission_resp(rrm_cell_index_t cell_index,rrm_ue_index_t ue_index,rrm_ue_response_et response,rrm_ue_context_t *p_rrm_ue_cntxt,U16 transaction_id)
 * \brief to Build and send the UE admission response.
 * \param cell_index Cell index Information
 * \param ue_index UE Index Information 
 * \param response Enum for Response
 * \param p_rrm_ue_cntxt UE Context Information
 * \param transaction_id transaction identifier
 */
rrm_return_et
rrm_build_and_send_ue_admission_resp(
	/*SPR_15323 start*/
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg,
        /*SPR_15323 end*/
        rrm_ue_response_et response,
        rrm_ue_context_t *p_rrm_ue_cntxt,
        U16              transaction_id,
        /*SPR 15439 Fix Start*/
        rrc_ue_adm_resp_fail_cause_et ue_adm_fail_cause
        /*SPR 15439 Fix End*/
        );
/*! \fn	rrm_return_et rrm_build_and_send_erb_setup_resp(rrm_ue_response_et response,rrm_ue_context_t *p_rrm_ue_cntxt,U16 transaction_id)
 * \brief this function will build and send the UE admission response.
 * \param response Response sent
 * \param p_rrm_ue_cntxt UE context Information
 * \param transaction_id transaction Identifier
 */
rrm_return_et
rrm_build_and_send_erb_setup_resp(
        rrm_ue_response_et response,
        rrm_ue_context_t *p_rrm_ue_cntxt,
        U16              transaction_id
        );


/*! \fn	rrm_return_et rrm_build_and_send_erb_modify_resp(rrm_ue_response_et response,rrm_ue_context_t *p_rrm_ue_cntxt,U16 transaction_id)
 * \brief this function will build and send the ERAB modify response
 * \param response Response sent
 * \param p_rrm_ue_cntxt UE context Information
 * \param transaction_id transaction Identifier
 */
rrm_return_et
rrm_build_and_send_erb_modify_resp(
        rrm_ue_response_et response,
        rrm_ue_context_t *p_rrm_ue_cntxt,
        U16              transaction_id
        );

/*! \fn rrm_return_et rrm_build_and_send_erb_release_resp(rrm_ue_response_et response,rrm_ue_context_t *p_rrm_ue_cntxt,U16 transaction_id)
 * \brief this function will build and send the ERAB send release reponse.
 * \param response Response sent
 * \param p_rrm_ue_cntxt UE context Information
 * \param transaction_id transaction Identifier
 */
rrm_return_et
rrm_build_and_send_erb_release_resp(
        rrm_ue_response_et response,
        rrm_ue_context_t *p_rrm_ue_cntxt,
        U16              transaction_id
        );



/*! \fn rrm_return_et rrm_build_and_send_ue_release_resp(rrm_cell_index_t cell_index,rrm_ue_index_t ue_index,rrm_ue_response_et response,U16 transaction_id)
 *  \brief		this function build and send the UE release response.
 *  \param cell_index Cell Index
 *  \param ue_index UE Index
 *  \param response Response
 *  \param transaction_id Transaction Identifier
 */	
rrm_return_et
rrm_build_and_send_ue_release_resp(
        rrm_cell_index_t   cell_index,
        rrm_ue_index_t   ue_index,
        rrm_ue_response_et response,
        U16              transaction_id
        );
/*MC, MR & HO  START*/
/*! \fn rrm_return_et rrm_build_and_send_ho_cmd_resp(rrm_response_t response,rrm_ue_proc_ho_command_request_t *p_rrm_proc_ho_cmd_req,U8 transaction_id,rrm_ue_context_t *p_ue_context)
 *  \brief      this function build and send the HO Command response.
 *  \param  response Response Success / Failure
 *  \param  p_rrm_proc_ho_cmd_req Proc HO command Request
 *  \param  transaction_id Transaction identifier
 *  \param p_ue_context UE context Information
 */
rrm_return_et
rrm_build_and_send_ho_cmd_resp(
        rrm_response_t response,
        rrm_ue_proc_ho_command_request_t    *p_rrm_proc_ho_cmd_req,
        U8 transaction_id,
        rrm_ue_context_t *p_ue_context
        );

/*! \fn rrm_return_et rrm_build_and_send_ho_required(U16 phy_cell_id, rrm_ue_context_t *p_ue_context,U8 transaction_id)
 *  \brief      this function build and send the HO required.
 *  \param  phy_cell_id
 *  \param  p_ue_context
 *  \param  transaction_id
 */
rrm_return_et
rrm_build_and_send_ho_required(
        /* SPR 15674 Start  */
        phy_cell_id_info_t phy_cell_id_info,
        /* SPR 15674 End  */
        rrm_ue_context_t    *p_ue_context,
        U8 transaction_id
        );

/*! \fn rrm_return_et rrm_build_and_send_ho_required(U16 phy_cell_id, rrm_ue_context_t *p_ue_context,U8 transaction_id)
 *  \brief      this function build and send the HO required.
 *  \param  phy_cell_id
 *  \param  p_ue_context
 *  \param  transaction_id
 */
rrm_return_et
rrm_build_and_send_lipa_ho_required(
        rrm_ue_context_t    *p_ue_context
        );


/*! \fn rrm_return_et rrm_build_and_send_ho_cancel_req(U16 ue_index,U8 transaction_id)
 *  \brief      this function build and send the HO CANCEL REQUEST.
 *  \param  rrm_ue_context UE context pointer
 *  \param  ue_index UE Index information
 *  \param  transaction_id Transaction Identifier
 */
rrm_return_et
rrm_build_and_send_ho_cancel_req(
        rrm_ue_context_t *p_ue_context,
        U16 ue_index,
        U8 transaction_id
        );
/*MC, MR & HO  END*/


/*! \fn rrm_return_et rrm_build_and_send_ue_connection_release_ind(rrm_ue_index_t ue_index,rrm_type_et type,rrm_radio_network_cause_et cause,U16 transaction_id,rrm_ue_context_t *p_ue_context)
 *  \brief      this function build and send the UE release response.
 *  \param  ue_index UE Index
 *  \param  type 
 *  \param  cause Radio Network cause
 *  \param  transaction_id Transaction Identifier
 *  \param  p_ue_context UE Context 
 *  \param  rrm_cell_index_t  cell_index
 */
rrm_return_et
rrm_build_and_send_ue_connection_release_ind(
        rrm_ue_index_t ue_index,
        U8 type, /* change data type to U8 to remove coverity warning after SPR_8345_fix */
        U16 cause, /* change data type to U16 to remove coverity warning after SPR_8345_fix */
        U16              transaction_id,
												rrm_ue_context_t *p_ue_context,
                                                /* SPR_10729_fix */
                                                rrm_cell_index_t  cell_index
        );

/*! \fn rrm_return_et rrm_build_and_send_erb_release_ind(rrm_ue_index_t ue_index,U8 *p_erab_id,U16 count,rrm_type_et type,rrm_radio_network_cause_et cause,U16 transaction_id)
 *  \brief      this function build and send the ERAB release indication.
 *  \param  rrm_ue_context_t UE context
 *  \param  ue_index UE index
 *  \param  p_erab_id ERAB identifier
 *  \param  count Count value
 *  \param  type Enum variable 
 *  \param  cause radio network cause
 *  \param  transaction_id transaction identifier
 */
/*
// Redefinition change
rrm_return_et
rrm_build_and_send_erb_release_ind(
rrm_ue_context_t	*p_ue_context,
rrm_ue_index_t          ue_index,
U8                      *p_erab_id,
U16                     count,
rrm_type_et                 type,
rrm_radio_network_cause_et          cause,
U16                 transaction_id
);
 */

/*! \fn	rrm_return_et rrm_ue_rrc_parse_ue_admission_req(U8 *p_api_buf,U16 api_id,U16 data_len,rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg,rrm_uem_s_tmsi_t s_tmsi)
 *  \brief  this function is used to parse the UE admission request
 *  \param  p_api_buf API buffer
 *  \param  api_id API identifier
 *  \param  data_len data length
 *  \param p_rrm_ue_rrc_msg UE RRC message
 *  \param s_tmsi Sim TMSI number
 */
rrm_return_et
rrm_ue_rrc_parse_ue_admission_req(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg,
        rrm_uem_s_tmsi_t s_tmsi
        );
/*! \fn rrm_return_et rrm_ue_rrc_parse_ue_inactive_ind(U8 *p_api_buf,U16 api_id,U16 data_len,rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg)
 *  \brief  This function is used to parse the UE inactivity indication.
 *  \param  p_api_buf API buffer
 *  \param  api_id API identifier
 *  \param  data_len data length
 *  \param p_rrm_ue_rrc_msg UE RRC message
 */
rrm_return_et
rrm_ue_rrc_parse_ue_inactive_ind(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        );

/*!  \fn rrm_return_et rrm_ue_rrc_parse_ue_admission_cnf(U8 *p_api_buf,U16 api_id,U16 data_len,rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg)
 *  \brief this function will parse the UE admission confirmation.
 *  \param  p_api_buf API buffer
 *  \param  api_id API identifier
 *  \param  data_len data length
 *  \param p_rrm_ue_rrc_msg UE RRC message
 */
rrm_return_et
rrm_ue_rrc_parse_ue_admission_cnf(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        );
/*! \fn	rrm_return_et rrm_ue_rrc_parse_ue_release_req(U8 *p_api_buf,U16 api_id,U16 data_len,rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg)
 *  \brief  this function used to parse the UE release request message.
 *  \param  p_api_buf API buffer
 *  \param  api_id API identifier
 *  \param  data_len data length
 *  \param p_rrm_ue_rrc_msg UE RRC message
 */	
rrm_return_et
rrm_ue_rrc_parse_ue_release_req(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        );

/*! \fn	rrm_return_et rrm_ue_rrc_parse_ue_capability_ind(U8 *p_api_buf,U16 api_id,U16 data_len,rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg)
 *  \brief This function will parse the UE capability indication
 *  \param  p_api_buf API buffer
 *  \param  api_id API identifier
 *  \param  data_len data length
 *  \param p_rrm_ue_rrc_msg UE RRC message
 */
rrm_return_et
rrm_ue_rrc_parse_ue_capability_ind(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        );

/*! \fn	rrm_return_et rrm_ue_rrc_parse_ue_erab_setup_req(U8 *p_api_buf,U16 api_id,U16 data_len,rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg,U16 *spid)
 *  \brief This function will parse the UE erab setup request.
 *  \param  p_api_buf API buffer
 *  \param  api_id API identifier
 *  \param  data_len data length
 *  \param p_rrm_ue_rrc_msg UE RRC message
 *  \param spid SP identifier
 */
/*SPR 17438 Fix Start*/
rrm_return_et
rrm_ue_rrc_parse_ue_erab_setup_req(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        );
/*SPR 17438 Fix End*/

/*! \fn  rrm_return_et rrm_ue_rrc_parse_ue_context_mod_req(U8 *p_api_buf,U16 api_id, U16 data_len, rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg)
 *  \brief      this function will parse the UE Context Modification request.
 *  \param  p_api_buf API buffer
 *  \param  api_id API identifier
 *  \param  data_len data length
 *  \param p_rrm_ue_rrc_msg UE RRC message
 */
rrm_return_et
rrm_ue_rrc_parse_ue_context_mod_req(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        );

/*! \fn rrm_return_et rrm_ue_rrc_parse_ue_context_mod_cnf(U8 *p_api_buf,U16 api_id,U16 data_len,rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg)
 *  \brief  this function will parse the UE Context Modification CNF.
 *  \param  p_api_buf API buffer
 *  \param  api_id API identifier
 *  \param  data_len data length
 *  \param p_rrm_ue_rrc_msg UE RRC message
 */
rrm_return_et
rrm_ue_rrc_parse_ue_context_mod_cnf(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg);

/*! \fn  rrm_return_et rrm_ue_rrc_parse_ue_erab_modify_req(U8 *p_api_buf,U16 api_id,U16 data_len,rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg)
 *  \brief		this function will parse the ERAB modify request.
 *  \param  p_api_buf API buffer
 *  \param  api_id API identifier
 *  \param  data_len data length
 *  \param p_rrm_ue_rrc_msg UE RRC message
 */
rrm_return_et
rrm_ue_rrc_parse_ue_erab_modify_req(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        );

/*! \fn	rrm_return_et rrm_ue_rrc_parse_ue_erab_setup_cnf(U8 *p_api_buf,U16 api_id,U16 data_len,rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg)
 *  \brief     this function will parse the ERAB setup confirmation.
 *  \param  p_api_buf API buffer
 *  \param  api_id API identifier
 *  \param  data_len data length
 *  \param p_rrm_ue_rrc_msg UE RRC message
 */	
rrm_return_et
rrm_ue_rrc_parse_ue_erab_setup_cnf(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        );
/*! \fn	rrm_return_et rrm_ue_rrc_parse_ue_erab_modify_cnf(U8 *p_api_buf,U16 api_id,U16 data_len,rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg)
 *  \brief     this function will parse the ERAB modify confirmation.
 *  \param  p_api_buf API buffer
 *  \param  api_id API identifier
 *  \param  data_len data length
 *  \param p_rrm_ue_rrc_msg UE RRC message
 */
rrm_return_et
rrm_ue_rrc_parse_ue_erab_modify_cnf(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        );

/* SPR 15260 start */
rrm_bool_et
rrm_is_required_meas_gap_config
(
 rrm_ue_context_t  *p_ue_context,
 rrm_meas_config_t *p_meas_config
 );
/* SPR 15260 end */

/*! \fn	rrm_return_et rrm_ue_rrc_parse_ue_erab_release_req(U8 *p_api_buf,U16 api_id,U16 data_len,rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg)
 *  \brief     this function will parse the ERAB release request.
 *  \param  p_api_buf API buffer
 *  \param  api_id API identifier
 *  \param	data_len data length
 *  \param p_rrm_ue_rrc_msg UE RRC message
 */
/* SPR_15313_Fix_Start */
rrm_void_t find_cqi_offset
(
 U8 cqiPeriodicity,
 U16 cqi_config_index ,
 U16 *cqi_offset_p
 );

rrm_void_t rrm_find_drx_offset_cqi_based
(
 rrm_ue_context_t                        *p_rrm_ue_context,
 rrm_oam_long_drx_cycle_start_offset_et  long_drx_cycle,
 U16                                     *p_drx_start_offset,
 /*SPR 15713 Fix Start*/
 U16                                     *p_cqi_offset
 /*SPR 15713 Fix End*/
 );
/* SPR_15313_Fix_End */

#ifdef ENDC_ENABLED
rrm_void_t rrm_find_drx_offset_cqi_based_r15
(
    rrm_ue_context_t                            *p_rrm_ue_context,
    rrm_oam_long_drx_cycle_start_offset_r15_et  long_drx_cycle,
    U16                                         *p_drx_start_offset,
    U16                                         *p_cqi_offset
);
#endif

rrm_return_et
rrm_ue_rrc_parse_ue_erab_release_req(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        );
/*! \fn	rrm_return_et rrm_ue_rrc_parse_ue_erab_release_cnf(U8 *p_api_buf,U16 api_id,U16 data_len,rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg)
 *  \brief     this function will parse the ERAB release confirmation.
 *  \param p_api_buf  API buffer
 *  \param api_id API identifier
 *  \param data_len data length
 *  \param p_rrm_ue_rrc_msg UE RRC message
 */
rrm_return_et
rrm_ue_rrc_parse_ue_erab_release_cnf(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        ); 
/*! \fn rrm_return_et rrm_ue_rrc_parse_ho_adm_req(U8 *p_api_buf, U16 api_id, U16 data_len, rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg) 
 *  \brief      this function is used to parse the HO admission request
 *  \param  p_api_buf API buffer
 *  \param  api_id API identifier
 *  \param  data_len data length
 *  \param p_rrm_ue_rrc_msg UE RRC message
 */
rrm_return_et
/* SPR 21496 Fix Start */
rrm_ue_rrc_parse_ho_adm_req(
        rrc_rrm_ue_ho_adm_req_t *p_rrc_rrm_ho_adm_req,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        );
/* SPR 21496 Fix End */
/*!     \fn rrm_return_et rrm_ue_rrc_parse_ho_adm_cnf(U8 *p_api_buf, U16 api_id, U16 data_len, rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg)
 *      \brief          this function will parse the HO admission confirmation
 *      \param    p_api_buf API buffer
 *      \param    api_id API identifier
 *      \param    data_len data length
 *      \param    p_rrm_ue_rrc_msg UE RRC message
 */
rrm_return_et
rrm_ue_rrc_parse_ho_adm_cnf(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        );
/*! \fn rrm_return_et  rrm_build_and_send_ho_adm_resp(rrm_response_t response,rrm_ue_context_t *p_rrm_ue_cntxt,U16 transaction_id)
 *  \brief          this function will build and send the HO admission response
 *  \param response Response 
 *  \param p_rrm_ue_cntxt UE Context Information
 *  \param transaction_id transaction identifier
 */
rrm_return_et
rrm_build_and_send_ho_adm_resp(
        rrm_response_t response,
        rrm_ue_context_t *p_rrm_ue_cntxt,
        U16              transaction_id
        );

/*! \fn rrm_return_et rrm_ue_rrc_parse_inactive_ues_ind(U8 *p_api_buf,U16  api_id,U16 data_len,U8 new_trans_id)
 *  \brief      This function build and send the UE inactive indication.
 *  \param  p_api_buf API buffer 
 *  \param  api_id API identifier
 *  \param  data_len data length
 *  \param  new_trans_id new transaction identifier
 */
rrm_return_et
rrm_ue_rrc_parse_inactive_ues_ind
(
 U8      *p_api_buf,
         /*SPR 17777 +-*/
 U16     data_len,
    rrm_ue_global_context_t *p_rrm_glb_ctxt
 );
/*! \fn rrm_return_et rrm_ue_rrc_parse_meas_config_resp(U8 *p_api_buf,U16 api_id,U16 data_len,rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg)
 *  \brief This function is used for parsing meas config response
 *  \param p_api_buf API buffer
 *  \param api_id  API identifier
 *  \param data_len data length
 *  \param p_rrm_ue_rrc_msg UE RRC message
 */
rrm_return_et
rrm_ue_rrc_parse_meas_config_resp(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        );
/*! \fn rrm_return_et rrm_ue_rrc_parse_measurement_results_ind(U8 *p_api_buf,U16 api_id,U16 data_len,rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg)
 *  \brief This function is used to parse measurements results 
 *  \param p_api_buf API buffer 
 *  \param api_id API identifier
 *  \param data_len data length
 *  \param p_rrm_ue_rrc_msg UE RRC message
 */
rrm_return_et
rrm_ue_rrc_parse_measurement_results_ind(
         /*SPR 17777 +-*/
        U8 *p_api_buf,
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
         /*SPR 19071  +-*/
	/*Code removed*/
         /*SPR 19071 +-*/
        );



/*! \fn rrm_return_et rrm_build_and_send_erb_release_ind(rrm_ue_index_t ue_index,U8 *p_erab_id, U16 count,rrm_type_et type,rrm_radio_network_cause_et cause,U16 transaction_id)
 *  \brief This function build and send the erb release indication
 *  \param p_ue_context UE context
 *  \param ue_index UE Index
 *  \param p_erab_id ERAB identifier
 *  \param count Count Value
 *  \param cause radio network cause
 *  \param transaction_id transaction identifier
 *  \param type rrm_type_et
 */
rrm_return_et
rrm_build_and_send_erb_release_ind(
        rrm_ue_context_t	    *p_ue_context,
        rrm_ue_index_t              ue_index,
        U8                          *p_erab_id,
        U16                         count,
        rrm_type_et                 type,
        rrm_radio_network_cause_et  cause,
        U16                         transaction_id
        );

/*! \fn rrm_return_et rrm_build_and_send_context_mod_resp(rrm_ue_response_et response,rrm_ue_context_t *p_rrm_ue_cntxt,U16 transaction_id)
 *  \brief  this function build and send the context modification resp
 *  \param  response Response Success/failure
 *  \param  p_rrm_ue_cntxt UE Context Information
 *  \param  transaction_id Transaction Identifier
 */
rrm_return_et
rrm_build_and_send_context_mod_resp(
        rrm_ue_response_et response,
        rrm_ue_context_t *p_rrm_ue_cntxt,
        U16              transaction_id
        );

/*! \fn  rrm_return_et rrm_build_and_send_meas_config_req_for_cgi_reporting(meas_eutran_node_t *p_meas_eutran_node,meas_utran_node_t *p_meas_utran_node, meas_utran_tdd_node_t  *p_meas_utran_tdd_node, meas_geran_node_t *p_meas_geran_node,rrm_ue_context_t *p_ue_context,void *p_cgi,U16 transaction_id ,rrm_ue_meas_config_t *p_meas_config_info)
 *  \brief      This function Builds the meas config request and sends to RRC for CGI 
 *              Reporting                
 *  \param p_meas_eutran_node Measurement EUTRAN Node
 *  \param p_meas_utran_node Measurement FDD UTRAN node
 *  \param p_meas_utran_node Measurement TDD UTRAN node
 *  \param p_meas_geran_node Measurement GERAN node
 *  \param p_ue_context UE Context Information
 *  \param p_cgi CGI information
 *  \param transaction_id transaction id
 *  \param p_meas_config_info Meas Configuration Information
 */
rrm_return_et
rrm_build_and_send_meas_config_req_for_cgi_reporting(
        meas_eutran_node_t              *p_meas_eutran_node,
        meas_utran_node_t               *p_meas_utran_node,
        /* TDD Support SON Start */
        meas_utran_tdd_node_t           *p_meas_utran_tdd_node,
        /* TDD Support SON End */
        meas_geran_node_t               *p_meas_geran_node,
        meas_cdma_node_t                *p_meas_cdma_node,
        rrm_ue_context_t                *p_ue_context,
        /* Spr 18401 Changes Start */
        rrm_ue_meas_config_cgi_queue_node_t *p_report_cgi,
        /* Spr 18401 Changes End */
        U16                             transaction_id,
        rrm_ue_meas_config_t        *p_meas_config_info
        );

/*! \fn  rrm_return_et rrm_build_and_send_meas_config_req(rrm_ue_context_t *p_ue_context,meas_type_et meas_type)
 *  \brief      Builds the meas config request and sends to RRC for Strongest
 *              cell reporting
 *  \param  p_ue_context UE Context Information
 *  \param  meas_type Measurement Type
 */
rrm_return_et
rrm_build_and_send_meas_config_req(
        rrm_ue_context_t                *p_ue_context,
        meas_type_et                     meas_type
        );

/*! \fn         rrm_return_et
  rrm_ue_rrc_parse_ho_cmd_req(U8 *p_api_buf,U16 api_id,U16 data_len,rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg)
 *  \brief      this function parse the ho cmd  req.
 *  \param  p_api_buf API buffer
 *  \param  api_id API Identifier
 *  \param  data_len data length
 *  \param  p_rrm_ue_rrc_msg UE RRC msg
 */
rrm_return_et
rrm_ue_rrc_parse_ho_cmd_req(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        );

/*! \fn rrm_return_et rrm_ue_rrc_parse_ho_failure(U8 *p_api_buf,U16 api_id,U16 data_len,rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg)
 *  \brief      this function parse the ho failure.
 *  \param p_api_buf API buffer
 *  \param api_id API Identifier
 *  \param data_len data length
 *  \param p_rrm_ue_rrc_msg UE RRC msg
 */
rrm_return_et
rrm_ue_rrc_parse_ho_failure(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        );                              

/*! \fn  rrm_return_et rrm_ue_rrc_parse_ho_cancel_resp(U8 *p_api_buf, U16 api_id,U16 data_len,rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg)
 *  \brief      this function parse the ho cancel response.
 *  \param p_api_buf API buffer
 *  \param api_id API Identifier
 *  \param data_len data length
 *  \param p_rrm_ue_rrc_msg UE RRC msg
 */
rrm_return_et
rrm_ue_rrc_parse_ho_cancel_resp(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        );


/*! \fn         rrm_return_et
  rrm_ue_rrc_parse_ho_restriction_ind(U8 *p_api_buf,U16 api_id,U16 data_len,rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg)
 *  \brief  this function parse the ho restriction indication.
 *  \param  p_api_buf API buffer
 *  \param  api_id API Identifier
 *  \param  data_len data length
 *  \param  p_rrm_ue_rrc_msg UE RRC msg
 */
rrm_return_et
rrm_ue_rrc_parse_ho_restriction_ind(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        );

/* Fix for SPR 10310 start */
/*! \fn rrm_return_et rrm_ue_fill_meas_config_req_for_ho(rrm_meas_config_t *p_meas_config,rrm_ue_context_t *p_rrm_ue_ctx)
 *  \brief This function Fills the eutran configuration in meas config list with values received from OAM
 *  \param p_meas_config measurement configuration
 *  \param p_rrm_ue_ctx UE context
 */
rrm_return_et
rrm_ue_fill_meas_config_req_for_ho(
        rrm_meas_config_t             *p_meas_config,
        rrm_ue_context_t              *p_rrm_ue_ctx
        );
/* Fix for SPR 10310 end */

/*! \fn rrm_void_t rrm_ue_fill_meas_remove_list_from_ho_adm_req(rrm_meas_config_t *p_meas_config,rrm_ue_context_t *p_rrm_ue_ctx)
 *  \brief This function is used to fill the eutran configuration in meas config list with values received from OAM
 *  \param p_meas_config measurement configuration
 *  \param p_rrm_ue_ctx UE context
 */
rrm_void_t
rrm_ue_fill_meas_remove_list_from_ho_adm_req(
        rrm_meas_config_t             *p_meas_config,
        rrm_ue_context_t              *p_rrm_ue_ctx
        );




/*LTE_RRM_TM_SWITCH_START*/

/*! \fn         rrm_return_et
  rrm_switch_transmission_mode_decision(U8 port,const rrm_ue_context_t *p_ue_context,rrm_ue_category_et ue_category,transmission_mode_ue_service_profile_et *p_transmission_mode)
 *  \brief      this function switch the transmission mode of ue.
 *  \param  port Port No
 *  \param  p_ue_context UE Context Information
 *  \param  ue_category UE Category
 *  \param p_transmission_mode Transmission Mode
 */
rrm_return_et
rrm_switch_transmission_mode_decision(U8 			port,
        rrm_ue_context_t   *p_ue_context,
        rrm_ue_category_et        ue_category,
        transmission_mode_ue_service_profile_et 
        *p_transmission_mode
        );


/*! \fn         rrm_return_et
  rrm_fill_transmission_mode(const rrm_ue_context_t *p_rrm_ue_context,rrc_phy_antenna_information_dedicated_t *p_rrm_antenna_information,rrm_bool_et *p_antenna_match)
 *  \brief      this function switch the transmission mode of ue.
 *  \param p_antenna_match boolean varible
 *  \param p_rrm_ue_context UE Context Information 
 *  \param p_rrm_antenna_information Antenna Information
 */
rrm_return_et
rrm_fill_transmission_mode( rrm_ue_context_t *p_rrm_ue_context,
        rrc_phy_physical_config_dedicated_t *p_rrm_phy_information,
        rrm_bool_et *p_antenna_match);


/*! \fn         rrm_return_et
  rrm_build_and_send_ue_reconfig_req(const rrm_ue_context_t *p_rrm_ue_context)
 *  \brief      this function send the transmission mode switch req to rrc.
 *  \param  p_rrm_ue_context UE Context Information
 */
rrm_return_et
rrm_build_and_send_ue_reconfig_req(rrm_ue_context_t *p_rrm_ue_context);


/*! \fn         rrm_return_et
  rrm_ue_rrc_parse_proximity_ind(U8 *p_api_buf, U16 api_id, U16 data_len, rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg)
 *  \brief      this function is used to parse the proximity ind
 *  \param  p_api_buf API buffer
 *  \param  api_id API identifier
 *  \param  data_len data length
 *  \param p_rrm_ue_rrc_msg UE msg
 */
rrm_return_et
rrm_ue_rrc_parse_proximity_ind(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        );

/*! \fn         rrm_return_et
  rrm_ue_rrc_parse_ue_reconfig_resp(U8 *p_api_buf,U16 *api_id,U16 *data_len,rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg)
 *  \brief      this function parse the ue's reconfig response
 *  \param  p_api_buf API buffer 
 *  \param  api_id API identifier
 *  \param  data_len Data length
 *  \param p_rrm_ue_rrc_msg RRC message
 */
rrm_return_et
rrm_ue_rrc_parse_ue_reconfig_resp(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        );
/*! \fn rrm_return_et rrm_cellm_assign_new_priorities(rrm_freq_priority_list_t *p_freq_priority,rrm_priority_cfg_t *p_priority_cfg,rrm_ue_context_t *p_ue_context)
 *  \brief This function is used assign new priorities
 *  \param p_freq_priority Frequency Priority
 *  \param p_priority_cfg Priority Config
 *  \param p_ue_context UE context Information
 */
rrm_return_et 
rrm_cellm_assign_new_priorities(
        rrm_freq_priority_list_t *p_freq_priority,
        rrm_priority_cfg_t *p_priority_cfg,
        rrm_ue_context_t *p_ue_context);
    /* SPR 13205 Fix Start */
/*! \fn rrm_void_t arrange_high_eutran (rrm_eutran_freq_priority_list_info_t *p_eutran_priority_frequency_list_info,rrm_priority_cfg_t *p_priority_cfg, U8 flag_cdma)
 *  \brief This function is used to arrange high EUTRAN information
 *  \param p_eutran_priority_frequency_list_info  Priority frequency list information
 *  \param p_priority_cfg Priority Configuration
 */
rrm_void_t arrange_high_eutran (rrm_eutran_freq_priority_list_info_t *p_eutran_priority_frequency_list_info,
        rrm_priority_cfg_t *p_priority_cfg);
    /* SPR 13205 Fix End */
/*! \fn rrm_void_t arrange_medium_eutran (rrm_eutran_freq_priority_list_info_t *p_eutran_priority_frequency_list_info,rrm_priority_cfg_t *p_priority_cfg)
 *  \brief This function is used to arrange medium EUTRAN information
 *  \param p_eutran_priority_frequency_list_info Priority frequency list information
 *  \param p_priority_cfg Priority Configuration
 */
rrm_void_t arrange_medium_eutran (rrm_eutran_freq_priority_list_info_t *p_eutran_priority_frequency_list_info,
        rrm_priority_cfg_t *p_priority_cfg);
/*! \fn rrm_void_t arrange_low_eutran (rrm_eutran_freq_priority_list_info_t *p_eutran_priority_frequency_list_info,rrm_priority_cfg_t *p_priority_cfg)
 *  \brief This function is used to arrange low EUTRAN information
 *  \param p_eutran_priority_frequency_list_info Priority frequency list information
 *  \param p_priority_cfg Priority Configuration
 */
rrm_void_t arrange_low_eutran (rrm_eutran_freq_priority_list_info_t *p_eutran_priority_frequency_list_info,
        rrm_priority_cfg_t *p_priority_cfg);
    /* SPR 13205 Fix Start */
/*! \fn rrm_void_t arrange_low_2_eutran (rrm_eutran_freq_priority_list_info_t *p_eutran_priority_frequency_list_info,rrm_priority_cfg_t *p_priority_cfg)
 *  \brief This function is used to arrange low_2 EUTRAN information
 *  \param p_eutran_priority_frequency_list_info Priority frequency list information
 *  \param p_priority_cfg Priority Configuration
 */
rrm_void_t arrange_low_2_eutran (rrm_eutran_freq_priority_list_info_t *p_eutran_priority_frequency_list_info,
                             rrm_priority_cfg_t *p_priority_cfg);
/*! \fn rrm_void_t arrange_high_utran (rrm_utran_freq_priority_list_info_t *p_utran_priority_frequency_list_info,rrm_priority_cfg_t *p_priority_cfg)
 *  \brief This function is used arrange high UTRAN parameters
 *  \param p_utran_priority_frequency_list_info Priority frequency list information
 *  \param p_priority_cfg Priority Configuration
 *  \param flag_cdma CDMA flag
 */
rrm_void_t arrange_high_utran (rrm_utran_freq_priority_list_info_t *p_utran_priority_frequency_list_info,
        rrm_priority_cfg_t *p_priority_cfg);
    /* SPR 13205 Fix End */
/*! \fn rrm_void_t arrange_medium_utran (rrm_utran_freq_priority_list_info_t *p_utran_priority_frequency_list_info,rrm_priority_cfg_t *p_priority_cfg)
 *  \brief This function is used to arrange medium UTRAN parameters
 *  \param p_utran_priority_frequency_list_info Priority frequency list information
 *  \param p_priority_cfg Priority Configuration
 */
rrm_void_t arrange_medium_utran (rrm_utran_freq_priority_list_info_t *p_utran_priority_frequency_list_info,
        rrm_priority_cfg_t *p_priority_cfg);
/*! \fn rrm_void_t arrange_low_utran (rrm_utran_freq_priority_list_info_t *p_utran_priority_frequency_list_info,rrm_priority_cfg_t  *p_priority_cfg)
 *  \brief This function is used for arranging low utran
 *  \param p_utran_priority_frequency_list_info Priority frequency list information
 *  \param p_priority_cfg Priority Configuration
 */
rrm_void_t arrange_low_utran (rrm_utran_freq_priority_list_info_t *p_utran_priority_frequency_list_info,
        rrm_priority_cfg_t *p_priority_cfg);
    /* SPR 13205 Fix Start */
/*! \fn rrm_void_t arrange_low_2_utran (rrm_utran_freq_priority_list_info_t *p_utran_priority_frequency_list_info,rrm_priority_cfg_t  *p_priority_cfg)
 *  \brief This function is used for arranging low_2 utran
 *  \param p_utran_priority_frequency_list_info Priority frequency list information
 *  \param p_priority_cfg Priority Configuration
 */
rrm_void_t arrange_low_2_utran (rrm_utran_freq_priority_list_info_t *p_utran_priority_frequency_list_info,
                             rrm_priority_cfg_t *p_priority_cfg);
    /* SPR 13205 Fix End */
/*! \fn rrm_void_t arrange_high_geran (rrm_geran_freq_priority_list_info_t *p_geran_priority_frequency_list_info,rrm_priority_cfg_t *p_priority_cfg)
 *  \brief This function is used to arrange high GERAN parameters
 *  \param p_geran_priority_frequency_list_info Priority frequency list information
 *  \param p_priority_cfg Priority Configuration
 */
rrm_void_t arrange_high_geran (rrm_geran_freq_priority_list_info_t *p_geran_priority_frequency_list_info,
        rrm_priority_cfg_t *p_priority_cfg);
/*! \fn rrm_void_t arrange_medium_geran(rrm_geran_freq_priority_list_info_t *p_geran_priority_frequency_list_info,rrm_priority_cfg_t *p_priority_cfg)
 *  \brief This function is used to arrange medium GERAN information
 *  \param p_geran_priority_frequency_list_info Priority frequency list information
 *  \param p_priority_cfg Priority Configuration
 */
rrm_void_t arrange_medium_geran (rrm_geran_freq_priority_list_info_t *p_geran_priority_frequency_list_info,
        rrm_priority_cfg_t *p_priority_cfg);
/*! \fn rrm_void_t arrange_low_geran(rrm_geran_freq_priority_list_info_t *p_geran_priority_frequency_list_info,rrm_priority_cfg_t *p_priority_cfg)
 *  \brief This function is used to arrange low GERAN information
 *  \param p_geran_priority_frequency_list_info rrm_geran_freq_priority_list_info_t
 *  \param p_priority_cfg rrm_priority_cfg_t
 */
rrm_void_t arrange_low_geran (rrm_geran_freq_priority_list_info_t *p_geran_priority_frequency_list_info,
        rrm_priority_cfg_t *p_priority_cfg);
    /* SPR 13205 Fix Start */
/*! \fn rrm_void_t arrange_low_2_geran(rrm_geran_freq_priority_list_info_t *p_geran_priority_frequency_list_info,rrm_priority_cfg_t *p_priority_cfg)
 *  \brief This function is used to arrange low_2 GERAN information
 *  \param p_geran_priority_frequency_list_info rrm_geran_freq_priority_list_info_t
 *  \param p_priority_cfg rrm_priority_cfg_t
 */
rrm_void_t arrange_low_2_geran (rrm_geran_freq_priority_list_info_t *p_geran_priority_frequency_list_info,
                             rrm_priority_cfg_t *p_priority_cfg);
/*! \fn rrm_void_t arrange_high_cdma (rrm_cdma_freq_priority_list_info_t *p_cdma_priority_frequency_list_info,rrm_priority_cfg_t *p_priority_cfg)
 *  \brief This function is used to arrange high CDMA parameters
 *  \param p_cdma_priority_frequency_list_info Priority frequency list information
 *  \param p_priority_cfg Priority Configuration
 */
rrm_void_t arrange_high_cdma (rrm_cdma_freq_priority_list_info_t *p_cdma_priority_frequency_list_info,
                                     rrm_priority_cfg_t *p_priority_cfg);
/*! \fn rrm_void_t arrange_medium_cdma(rrm_cdma_freq_priority_list_info_t *p_cdma_priority_frequency_list_info,rrm_priority_cfg_t *p_priority_cfg)
 *  \brief This function is used to arrange medium CDMA information
 *  \param p_cdma_priority_frequency_list_info Priority frequency list information
 *  \param p_priority_cfg Priority Configuration
 */
rrm_void_t arrange_medium_cdma (rrm_cdma_freq_priority_list_info_t *p_cdma_priority_frequency_list_info,
                             rrm_priority_cfg_t *p_priority_cfg);
/*! \fn rrm_void_t arrange_low_cdma(rrm_cdma_freq_priority_list_info_t *p_cdma_priority_frequency_list_info,rrm_priority_cfg_t *p_priority_cfg)
 *  \brief This function is used to arrange low CDMA information
 *  \param p_cdma_priority_frequency_list_info rrm_cdma_freq_priority_list_info_t
 *  \param p_priority_cfg rrm_priority_cfg_t
 */
rrm_void_t arrange_low_cdma (rrm_cdma_freq_priority_list_info_t *p_cdma_priority_frequency_list_info,
                             rrm_priority_cfg_t *p_priority_cfg);
/*! \fn rrm_void_t arrange_low_2_cdma(rrm_cdma_freq_priority_list_info_t *p_cdma_priority_frequency_list_info,rrm_priority_cfg_t *p_priority_cfg)
 *  \brief This function is used to arrange low_2 CDMA information
 *  \param p_cdma_priority_frequency_list_info rrm_cdma_freq_priority_list_info_t
 *  \param p_priority_cfg rrm_priority_cfg_t
 */
rrm_void_t arrange_low_2_cdma (rrm_cdma_freq_priority_list_info_t *p_cdma_priority_frequency_list_info,
                             rrm_priority_cfg_t *p_priority_cfg);
    /* SPR 13205 Fix End */



/*! \fn         rrm_return_et
  rrm_transmission_mode_packet_delay_and_error_rate_constraintconst(const rrm_ue_context_t*,U8 ,rrm_bool_et *)
 *  \brief      This function check does any rab havin g qci value 3 or 7
 *  \param  p_ue_context
 *  \param qci
 *  \param p_delay_constraint
 */
rrm_void_t
rrm_transmission_mode_packet_delay_and_error_rate_constraint(const rrm_ue_context_t *p_ue_context,
        U8    	     qci,
        rrm_bool_et    *p_delay_constraint);

/*4x2 MIMO start*/
typedef void (*rrm_transmission_mode)( rrc_phy_physical_config_dedicated_t *, 
        antenna_info_t *,
        rrm_ue_context_t *);
/*4x2 MIMO end*/

/* CA_Stage_3_Scell_TM_change: Start */
typedef void (*rrm_scell_transmission_mode)(rrc_antenna_info_dedicated_r10_t *,
                              antenna_info_t *,
                              rrm_ue_context_t *,
                              rrm_cell_index_t);
/* CA_Stage_3_Scell_TM_change: End */

/* Uplink power starts */
typedef void (*rrm_ue_olpc_action)(rrm_ue_context_t *,S8 *,S8 *, U8);

/*! \fn         rrm_void_t
  rrm_olpc_inc_power(rrm_ue_context_t *,S8 *,S8 *, U8)
 *  \brief      this function set pusch and pucch to increase uplink power 
 *  \param  p_rrm_ue_context
 *  \param  p_rrm_olpc_ue_value
 *  \param  p_rrm_ue_profile_value
 *  \param  rrm_flag
 *  \param p_rrm_olpc_ue_value
 */
rrm_void_t
rrm_olpc_inc_power(rrm_ue_context_t *,S8 *,S8 *, U8);

/*! \fn         rrm_void_t
 *              rrm_olpc_inc_pow_on_high_prb_usage(rrm_ue_context_t *,S8 *,S8 *, U8)
 *  \brief      this function set pusch and pucch to increase uplink power on high prb usage
 *  \param  p_rrm_ue_context
 *  \param  p_rrm_olpc_ue_value
 *  \param  p_rrm_ue_profile_value
 *  \param  rrm_flag
 *  \param p_rrm_olpc_ue_value
 */
rrm_void_t 
rrm_olpc_inc_pow_on_high_prb_usage(rrm_ue_context_t *,S8 *,S8 *, U8);

/*! \fn         rrm_void_t
 *              rrm_olpc_unlikely_event(rrm_ue_context_t *,S8 *,S8 *, U8)
 *  \brief      This function raise allarm on unlikley events
 *  \param  p_rrm_ue_context
 *  \param  p_rrm_olpc_ue_value
 *  \param  p_rrm_ue_profile_value
 *  \param  rrm_flag
 *  \param p_rrm_olpc_ue_value
 */
rrm_void_t 
rrm_olpc_unlikely_event(rrm_ue_context_t *,S8 *,S8 *, U8);

/*! \fn         rrm_void_t
 *              rrm_olpc_inc_little_power(rrm_ue_context_t *,S8 *,S8 *, U8)
 *  \brief      This function set pusch and pucch to increase uplink power
 *  \param  p_rrm_ue_context
 *  \param  p_rrm_olpc_ue_value
 *  \param  p_rrm_ue_profile_value
 *  \param  rrm_flag
 *  \param p_rrm_olpc_ue_value
 */
rrm_void_t 
rrm_olpc_inc_little_power(rrm_ue_context_t *,S8 *,S8 *, U8);

/*! \fn         rrm_void_t
 *               rrm_olpc_dec_pow_on_low_prb_usage(rrm_ue_context_t *,S8 *,S8 *, U8)
 *  \brief      This function set pusch and pucch to decrease uplink poweron low prb usage
 *  \param  p_rrm_ue_context
 *  \param  p_rrm_olpc_ue_value
 *  \param  p_rrm_ue_profile_value
 *  \param  rrm_flag
 *  \param p_rrm_olpc_ue_value
 */
rrm_void_t 
rrm_olpc_dec_pow_on_low_prb_usage(rrm_ue_context_t *,S8 *,S8 *, U8);

/*! \fn         rrm_void_t
 *               rrm_olpc_dec_pow(rrm_ue_context_t *,S8 *,S8 *, U8)
 *  \brief      This function set pusch and pucch to increase uplink power
 *  \param  p_rrm_ue_context
 *  \param  p_rrm_olpc_ue_value
 *  \param  p_rrm_ue_profile_value
 *  \param  rrm_flag
 */
rrm_void_t 
rrm_olpc_dec_pow(rrm_ue_context_t *,S8 *,S8 *, U8);

/*! \fn         rrm_void_t
 *              rrm_olpc_no_action(rrm_ue_context_t *,S8 *,S8 *, U8)
 *  \brief      This function set pusch and pucch for default uplink power
 *  \param  p_rrm_ue_context
 *  \param  p_rrm_olpc_ue_value
 *  \param  p_rrm_ue_profile_value
 *  \param  rrm_flag
 */
rrm_void_t 
rrm_olpc_no_action(rrm_ue_context_t *,S8 *,S8 *, U8);
/* Uplink power end */

/*! \fn  rrm_return_et
 *        rrm_setting_transmission_mode(transmission_mode_ue_service_profile_et transmission_mode,rrc_phy_antenna_information_dedicated_t *p_rrm_antenna_information,rrm_cell_index_t cell_index)
 *  \brief This function fill the info of transmission_mode
 *  \param transmission_mode Transmission Mode     
 *  \param p_rrm_antenna_information Antenna Information
 *  \param cell_index Cell Index Information
 */
rrm_return_et
rrm_setting_transmission_mode(transmission_mode_ue_service_profile_et
        transmission_mode,
        rrc_phy_physical_config_dedicated_t 
        *p_rrm_phy_information,
        rrm_ue_context_t *p_rrm_ue_context);


/*! \fn rrm_fill_tm1(rrc_phy_antenna_information_dedicated_t *)
 *  \brief  This function fill the info of siso
 */

rrm_void_t rrm_fill_tm1(rrc_phy_physical_config_dedicated_t *p_rrm_phy_information,
        antenna_info_t *p_antenna_info_t,
        rrm_ue_context_t *p_ue_context);

/*! \fn  rrm_fill_tm2(rrc_phy_antenna_information_dedicated_t *)
 *  \brief  This function fill the info of TM-2 mimo
 */
rrm_void_t rrm_fill_tm2(rrc_phy_physical_config_dedicated_t *p_rrm_phy_information ,
        antenna_info_t *p_antenna_info_t,
        rrm_ue_context_t *p_ue_context);

/*! \fn rrm_fill_tm3(rrc_phy_antenna_information_dedicated_t *)
 *  \brief      This function fill the info of TM-3 mimo
 */
rrm_void_t rrm_fill_tm3( rrc_phy_physical_config_dedicated_t *p_rrm_phy_information ,
        antenna_info_t *p_antenna_info_t,
        rrm_ue_context_t *p_ue_context);
/*! \fn rrm_fill_tm4(rrc_phy_antenna_information_dedicated_t *)
 *  \brief This function fill the info of TM-4444mo
 */
rrm_void_t rrm_fill_tm4( rrc_phy_physical_config_dedicated_t *p_rrm_phy_information ,
        antenna_info_t *p_antenna_info_t,
        rrm_ue_context_t *p_ue_context);

/*! \fn rrm_fill_tm5(rrc_phy_antenna_information_dedicated_t *)
 *  \brief      This function fill the info of TM-5 mimo
 */
rrm_void_t rrm_fill_tm5( rrc_phy_physical_config_dedicated_t *p_rrm_phy_information,
        antenna_info_t *p_antenna_info_t,
        rrm_ue_context_t *p_ue_context);
/*! \fn rrm_fill_tm6(rrc_phy_antenna_information_dedicated_t *)
 *  \brief      This function fill the info of TM-6 mimo
 */
rrm_void_t rrm_fill_tm6( rrc_phy_physical_config_dedicated_t *p_rrm_phy_information,
        antenna_info_t *p_antenna_info_t,
        rrm_ue_context_t *p_ue_context);
/*! \fn rrm_fill_tm7(rrc_phy_antenna_information_dedicated_t *)
 *  \brief      This function fill the info of TM-6 mimo
 */
rrm_void_t rrm_fill_tm7( rrc_phy_physical_config_dedicated_t *p_rrm_phy_information ,
        antenna_info_t *p_antenna_info_t,
        rrm_ue_context_t *p_ue_context);
/*! \fn rrm_fill_tm8(rrc_phy_antenna_information_dedicated_t *)
 *  \brief      This function fill the info of TM-6 mimo
 */
rrm_void_t rrm_fill_tm8( rrc_phy_physical_config_dedicated_t *p_rrm_phy_information ,
        antenna_info_t *p_antenna_info_t,
        rrm_ue_context_t *p_ue_context);
/*LTE_RRM_TM_SWITCH_END*/
/*! \fn rrm_return_et rrm_get_idle_mode_ue_connection_release_ind_param(
  rrc_rrm_ue_connection_release_ind_t * p_ue_connection_release_ind,rrm_ue_context_t *p_ue_context, rrm_bool_t csfb_present )
 *  \brief This function fills the idlemodemobilityinfo in RRC Conn Release Ind msg
 *  \param  p_ue_connection_release_ind UE_CONN_RELEASE_IND structure
 *  \param  p_ue_context UE Context Information
 */
rrm_return_et rrm_get_idle_mode_ue_connection_release_ind_param(
        rrc_rrm_ue_connection_release_ind_t* p_ue_connection_release_ind,
        rrm_ue_context_t *p_ue_context);

/* Loss-less data transfer changes start */
/*! \fn rrm_void_t fill_dl_data_fwd_erab_list(rrm_ue_context_t *p_ue_context,rrc_rrm_ho_required_t *p_ho_required)
 *  \brief  This functions fills the erab_list that requires DL data
 *          forwarding based on the QCI configurations
 *  \param p_ue_context UE Context Information
 *  \param p_ho_required HO Required or not
 */
rrm_void_t fill_dl_data_fwd_erab_list(
        rrm_ue_context_t 		*p_ue_context,
        rrc_rrm_ho_required_t	*p_ho_required);
/* LTE_RRM_WARNING_REMOVAL_START*/
/*! \fn rrm_return_et rrm_handle_ue_reconfig_req(rrm_ue_context_t *)
 *  \brief  This function send the ue reconfig request
 *  \param  p_ue_context UE Context Information
 */
rrm_return_et
rrm_handle_ue_reconfig_req(
        rrm_ue_context_t *p_ue_context
        )  ;  

/* LTE_RRM_WARNING_REMOVAL_END*/

/*! \fn rrm_return_et rrm_cqi_information_reconfig_req(const rrm_ue_context_t *p_ue_context,U8 transmission_mode,rrc_phy_cqi_reporting_t * p_cqi_report)
 *  \brief This function is CQI information Reconfig Request      
 *  \param  p_ue_context Pointer to UE context
 *  \param  transmission_mode Transmission Mode
 *  \param  p_cqi_report Pointer to physical cqi reporting
 */

rrm_return_et
rrm_cqi_information_reconfig_req_r10(const rrm_ue_context_t         *p_ue_context,
				     U8			            transmission_mode, 
				     rrc_cqi_report_config_r10_t    *p_cqi_updated_info_r10);

/*! \fn rrm_return_et rrm_cqi_information_reconfig_req(const rrm_ue_context_t *p_ue_context,U8 transmission_mode,rrc_phy_cqi_reporting_t * p_cqi_report)
 *  \brief This function is CQI information Reconfig Request      
 *  \param  p_ue_context Pointer to UE context
 *  \param  transmission_mode Transmission Mode
 *  \param  p_cqi_report Pointer to physical cqi reporting
*/
rrm_return_et
rrm_cqi_information_reconfig_req(const rrm_ue_context_t     *p_ue_context,
        U8                          transmission_mode,
        rrc_phy_cqi_reporting_t    *p_cqi_report);

/*! \fn rrm_return_et rrm_rrc_ue_fill_cqi_reporting(rrc_phy_cqi_reporting_t *p_rrc_phy_cqi_reporting, 
  rrc_phy_cqi_reporting_ue_service_profile_t *p_ue_profile_phy_cqi_reporting,
  rrm_ue_context_t *p_rrm_ue_cntxt, U8 transmission_mode)
 *  \brief This function UE fill CQI Reporting 
 *  \param  p_rrc_phy_cqi_reporting Physical CQI Reporting Information
 *  \param  p_ue_profile_phy_cqi_reporting Profile Physical CQI reporting
 *  \param  p_rrm_ue_cntxt UE context Information
 *  \param  transmission_mode Transmission Mode
 */
rrm_return_et
rrm_rrc_ue_fill_cqi_reporting(
        rrc_phy_cqi_reporting_t                        *p_rrc_phy_cqi_reporting,
        rrc_phy_cqi_reporting_ue_service_profile_t     *p_ue_profile_phy_cqi_reporting,
        rrm_ue_context_t                               *p_rrm_ue_cntxt,
        U8                                              transmission_mode);
/*! \fn rrm_return_et rrm_rrc_ue_fill_cqi_reporting_periodic(rrc_phy_cqi_reporting_periodic_t *p_rrc_phy_cqi_reporting_periodic,rrc_phy_cqi_reporting_periodic_ue_service_profile_t *p_ue_profile_phy_cqi_reporting_periodic,rrm_ue_context_t *p_rrm_ue_cntxt,U8 transmission_mode ,rrm_ul_dl_freq_selective_sched_et rrm_freq_selective_sched)
 *  \brief This function is used to Fill UE CQI Reporting Periodic Information
 *  \param p_rrc_phy_cqi_reporting_periodic Physical CQI reporting periodic information
 *  \param p_ue_profile_phy_cqi_reporting_periodic CQI Reporting Periodic Info
 *  \param p_rrm_ue_cntxt RRM UE Context
 *  \param transmission_mode Transmission Mode
 *  \param rrm_freq_selective_sched Frequency Selective Scheduling
 */
rrm_return_et
rrm_rrc_ue_fill_cqi_reporting_periodic(
        rrc_phy_cqi_reporting_periodic_t
        *p_rrc_phy_cqi_reporting_periodic,
        rrc_phy_cqi_reporting_periodic_ue_service_profile_t
        *p_ue_profile_phy_cqi_reporting_periodic,
        rrm_ue_context_t
        *p_rrm_ue_cntxt,
        U8                     transmission_mode,
        rrm_ul_dl_freq_selective_sched_et
        rrm_freq_selective_sched
        );
/*! \fn rrm_void_t fill_cqi_reporting_periodic_param(rrc_phy_cqi_reporting_periodic_param_t *p_rrc_phy_cqi_reporting_periodic_param,rrc_phy_cqi_reporting_periodic_param_ue_service_profile_t *p_ue_profile_phy_cqi_reporting_periodic_param,rrm_ue_context_t *p_rrm_ue_cntxt,U8 transmission_mode,rrm_ul_dl_freq_selective_sched_et rrm_freq_selective_sched)
 *  \brief This function is used to fill CQI Reporting Periodic Parameter
 *  \param p_rrc_phy_cqi_reporting_periodic_param Physical CQI Reporting Periodic Param 
 *  \param p_ue_profile_phy_cqi_reporting_periodic_param Physical CQI Reporting Periodic Param information
 *  \param p_rrm_ue_cntxt UE context information
 *  \param transmission_mode Transmission Mode Information
 *  \param rrm_freq_selective_sched frequency Selective scheduling
 */
rrm_void_t fill_cqi_reporting_periodic_param(
        rrc_phy_cqi_reporting_periodic_param_t
        *p_rrc_phy_cqi_reporting_periodic_param,
        rrc_phy_cqi_reporting_periodic_param_ue_service_profile_t
        *p_ue_profile_phy_cqi_reporting_periodic_param,
        rrm_ue_context_t
        *p_rrm_ue_cntxt,
        U8          transmission_mode,
        rrm_ul_dl_freq_selective_sched_et
        rrm_freq_selective_sched
        );
/*! \fn rrm_return_et assign_aperiodic_mode_from_tm_mode(rrm_ue_context_t *p_rrm_ue_cntxt,U8 *p_rrc_reporting_mode_aperiodic,rrm_ul_dl_freq_selective_sched_et freq_selective_sched,U8 transmission_mode,U32 *p_aperiodic_mode)
 *  \brief This function is used to assign aperiodic mode from mode
 *  \param p_rrm_ue_cntxt UE context Information 
 *  \param p_rrc_reporting_mode_aperiodic Reporting Mode Aperiodic Information
 *  \param freq_selective_sched Frequency Selective Scheduling Information
 *  \param transmission_mode Transmission Mode 
 *  \param p_aperiodic_mode Aperiodic Mode
 */
rrm_return_et assign_aperiodic_mode_from_tm_mode(rrm_ue_context_t           *p_rrm_ue_cntxt,
        U8                           *p_rrc_reporting_mode_aperiodic,
        rrm_ul_dl_freq_selective_sched_et  freq_selective_sched,
        U8                           transmission_mode,
        U32                          *p_aperiodic_mode);
/*! \fn rrm_return_et fill_erb_setup_resp_physical_config_antenna_info_v920(rrc_phy_physical_config_dedicated_t *p_rrc_phy_physical_config_dedicated,const rrc_phy_codebook_subset_restriction_v90_ue_service_profile_t *p_codebook_subset_restriction_v90)
 *  \brief This function is used to fill ERB Setup Response Physical Configuration antenna information v920
 *  \param p_rrc_phy_physical_config_dedicated Physical Configuration Dedicated Information
 *  \param p_codebook_subset_restriction_v90 Codebook Subset restriction v90
 */
rrm_return_et
fill_erb_setup_resp_physical_config_antenna_info_v920(
        rrc_phy_physical_config_dedicated_t 
        *p_rrc_phy_physical_config_dedicated,
        const rrc_phy_codebook_subset_restriction_v90_ue_service_profile_t 
        *p_codebook_subset_restriction_v90);

/*! \fn rrm_return_et rrm_rrc_ue_adm_resp_fill_pucch_configuration_dedicated(rrc_phy_pucch_configuration_dedicated_t *p_rrc_phy_pucch_configuration_dedicated,rrm_physical_layer_params_t *p_rrm_physical_layer_params)
 *  \brief  This function fills the pucch config parameters in the ue admission response message
 *  \param p_rrc_phy_pucch_configuration_dedicated Physical PUCCH Configuration dedicated
 *  \param p_rrm_physical_layer_params Physical Layer parameters
 */
rrm_return_et
rrm_rrc_ue_adm_resp_fill_pucch_configuration_dedicated(
        rrc_phy_pucch_configuration_dedicated_t
         /*SPR 17777 +-*/
        *p_rrc_phy_pucch_configuration_dedicated
#ifdef TDD_MODE_FLAG        
        ,rrm_physical_layer_params_t
        *p_rrm_physical_layer_params
#endif        
         /*SPR 17777 +-*/
        );


/*! \fn rrm_return_et rrm_build_and_send_meas_config_req_release(rrm_ue_context_t *p_ue_context)
 *  \brief This function is used to build and send Measurement configuration request for release
 *  \param p_ue_context UE Context
 */
rrm_return_et
rrm_build_and_send_meas_config_req_release(
        rrm_ue_context_t                *p_ue_context
        );

/* COMP_WARN_1_FEB:compilation warning removal */
/*! \fn rrm_void_t fill_eutran_meas_config_to_remove_list(rrm_meas_config_t *p_meas_config,rrm_ue_context_t *p_rrm_ue_ctx)
 *  \brief This function is used Fill EUTRAN MEAS configuration to remove List
 *  \param p_meas_config Measurement Configuration
 *  \param p_rrm_ue_ctx UE Context Information
 */
rrm_void_t
fill_eutran_meas_config_to_remove_list(
        rrm_meas_config_t             *p_meas_config,
        rrm_ue_context_t              *p_rrm_ue_ctx
        );

/* SPS related changes start */
/*! \fn rrm_void_t fill_sps_config(rrc_sps_config_t *sps_config, epc_params_t *p_epc_params,U8 qci_val,S8 p0_ue_pusch,rrm_ue_context_t *p_rrm_ue_context)
 *  \brief This function is used to fill SPS Configuration Information
 *  \param sps_config SPS Configuration Information
 *  \param p_epc_params EPC parameters
 *  \param qci_val QCI value
 *  \param p0_ue_pusch p0 UE PUSCH information
 *  \param p_rrm_ue_context UE context Information
 */
rrm_return_et
fill_sps_config(
        rrc_sps_config_t          *sps_config,
        epc_params_t              *p_epc_params,
        U8                         qci_val,
        S8                         p0_ue_pusch,
        rrm_ue_context_t          *p_rrm_ue_context
        );
/*! \fn rrm_void_t fill_reqd_drx_params_for_sps(rrc_drx_config_param_t *p_rrc_drx_config_param,drx_config_t *p_drx_config,rrm_cell_context_t *p_cell_context,U8 qci_val)
 *  \brief This function is used to fill Required DRX parameters for SPS data
 *  \param p_rrc_drx_config_param DRX configuration parameters
 *  \param p_drx_config DRX configuration
 *  \param p_cell_context Cell Context Information
 *  \param qci_val QCI value
 */
rrm_void_t
fill_reqd_drx_params_for_sps(
        rrc_drx_config_param_t  *p_rrc_drx_config_param,
/* SPR 10450 Fix Start*/
        rrm_cell_drx_res_per_profile_t *p_drx_config,
/* SPR 10450 Fix End */
        rrm_cell_context_t      *p_cell_context,
        U8                       qci_val
        );
/*! \fn rrm_void_t modify_reqd_drx_params_for_sps( rrc_drx_config_param_t *p_rrc_drx_config_param,rrm_drx_config_t *drx_config,rrm_cell_context_t *p_cell_context,U8 qci_val)
 *  \brief This function is used to modify DRX parameters for SPS data
 *  \param p_rrc_drx_config_param DRX configuration parameters
 *  \param drx_config DRX configuration
 *  \param p_cell_context Cell Context Information
 *  \param qci_val QCI value
 */
rrm_void_t
modify_reqd_drx_params_for_sps(
        rrc_drx_config_param_t    *p_rrc_drx_config_param,
        rrm_drx_config_t          *drx_config,
        rrm_cell_context_t        *p_cell_context,
        U8                         qci_val
        );

/*SPR 17777 +-*/

/*! \fn rrm_void_t fill_sps_dl_pucch_list(rrc_n1_pucch_an_persist_list_t *p_n1_pucch_an_persist_list,rrm_ue_context_t *p_rrm_ue_context)
 *  \brief This function is used to fill SPS Downlink PUCCH List information
 *  \param p_n1_pucch_an_persist_list n1 PUCCH Persist list
 *  \param p_rrm_ue_context UE context Information
 */
rrm_return_et
fill_sps_dl_pucch_list(
        rrc_n1_pucch_an_persist_list_t  *p_n1_pucch_an_persist_list,
        rrm_ue_context_t                *p_rrm_ue_context
        );

/* SPS related changes end */
/*! \fn rrm_return_et rrm_ue_release_process(rrm_ue_context_t *p_ue_context,U16 trans_id)
 *  \brief This function is used for UR release Process
 *  \param p_ue_context UE context Information
 *  \param trans_id Transaction Identifier
 */
rrm_return_et
rrm_ue_release_process(
    /* eICIC_PHASE_1_2_CHANGES_START */
   rrm_ue_global_context_t *p_g_rrm_ue_ctx
    /* eICIC_PHASE_1_2_CHANGES_END */
        );

/* SRS_FREQ_HOPPING_START */
/*! \fn rrm_bool_t rrm_check_for_srs_hopping_enable_disable(rrm_ue_context_t *p_rrm_ue_context,rrc_phy_sounding_rs_ul_config_dedicated_t *p_srs_ul_config_dedicated)
 *  \brief This function is used to check whether SRS Hopping Enable / Disable
 *  \param p_rrm_ue_context UE context Information
 *  \param p_srs_ul_config_dedicated SRS Uplink Configuration
 */
rrm_bool_t
rrm_check_for_srs_hopping_enable_disable
( 
 rrm_ue_context_t *p_rrm_ue_context,
 rrc_phy_sounding_rs_ul_config_dedicated_t *p_srs_ul_config_dedicated
 );

/*! \fn rrm_void_t fill_ue_reconfig_req_for_srs_configuration(rrm_ue_context_t *p_rrm_ue_context ,rrc_phy_sounding_rs_ul_config_dedicated_param_t *p_ue_srs_params)
 *  \brief This function is used to Fill UE Reconfig Request for SRS configuration
 *  \param p_rrm_ue_context UE context Information
 *  \param p_ue_srs_params UE SRS Parameters
 */
rrm_void_t
fill_ue_reconfig_req_for_srs_configuration
(
 rrm_ue_context_t *p_rrm_ue_context , 
 rrc_phy_sounding_rs_ul_config_dedicated_param_t *p_ue_srs_params 
 );

/* SRS_FREQ_HOPPING_END */

/*  DYNAMIC ICIC CHANGES START  */

/*! \fn         rrm_return_et
  rrm_build_and_send_ue_reconfig_req_for_icic_action(const rrm_ue_context_t *p_rrm_ue_context)
 *  \brief      this function send the transmission mode switch req to rrc.
 *  \param  p_rrm_ue_context UE Context Information
 */
rrm_return_et
rrm_build_and_send_ue_reconfig_req_for_icic_action(rrm_ue_context_t *p_rrm_ue_context);

/*  DYNAMIC ICIC CHANGES END  */

rrm_void_t
rrm_fill_release_params_cdma2000
(
 rrc_rrm_ue_connection_release_ind_t     *p_rrc_connection_release_ind,
 rrm_ue_context_t                        *p_ue_context
 );
/* start spr 9064 */
rrmcm_rmif_cdma2000_freq_cells_t *
rrm_ue_match_1xrtt_daho_cell_from_ncl
(
 rrm_ue_context_t *p_rrm_ue_context,
 inter_rat_ncl_t  *p_inter_rat_ncl,
 rrmcm_idle_mode_mobility_irat_cdma_params_t  *p_irat_cdma_params
 );

rrmcm_rmif_cdma2000_freq_cells_t*
rrm_ue_match_hrpd_daho_cell_from_ncl
(
 rrm_ue_context_t *p_rrm_ue_context,
 inter_rat_ncl_t  *p_inter_rat_ncl,
 rrmcm_idle_mode_mobility_irat_cdma_params_t  *p_irat_cdma_params
 );
/* end spr 9064 */

/* SPR 15674 Start */
rrm_bool_et rrm_ue_check_for_cco
(
 /* SPR_14067_Fix_Start */
 rrm_ue_context_t        *p_ue_context,
 /* SPR 19364 start */
 phy_cell_id_info_t      *p_phy_cell_id_info
 /* SPR 19364 end */
 /* SPR_14067_Fix_End */
 );
/* SPR 15674 End */

/* LIPA SIPTO Changes start */
rrm_return_et
rrm_rrc_parse_local_error_ind
(
 U8 *p_api_buf,
         /*SPR 17777 +-*/
 U16 data_len,
 rrm_ue_global_context_t *p_rrm_ue_glb_ctxt
 );
/* LIPA SIPTO Changes end */

rrm_return_et
         /*SPR 17777 +-*/
rrm_build_and_send_daho_cdma2000_ho_required
(
 rrm_ue_context_t * p_ue_context,
 U16 transaction_id
 );
rrm_return_et
rrm_build_and_send_ue_connection_release_ind_carrier_redir
(
 rrm_ue_index_t      ue_index,
 rrm_cell_index_t    cell_index,
 U16                 earfcn,
 rrm_type_et         type,
 rrm_radio_network_cause_et      cause,
 U16                 transaction_id
 );


rrm_void_t
rrm_fill_release_params_cdma2000
(
 rrc_rrm_ue_connection_release_ind_t     *p_rrc_connection_release_ind,
 rrm_ue_context_t                        *p_ue_context
 );

rrm_void_t
rrm_rrc_fill_mobility_sib_8_params(
        rrm_cell_index_t cell_index,
         /*SPR 17777 +-*/
        mobility_parameters_t  *p_mobility_parameters,
        additional_mobility_parameters_t  *p_add_mobility_parameters);

rrm_void_t
rrm_rrc_fill_irat_ncl_cdma2000_params(
        rrm_cell_index_t cell_index,
         /*SPR 17777 +-*/
        mobility_parameters_t *p_rrcm_mobility_parameters,
        additional_mobility_parameters_t  *p_rrcm_add_mobility_parameters);

rrm_void_t
rrm_rrc_fill_ue_generic_cdma2000_params(
        rrm_cell_index_t cell_index,
         /*SPR 17777 +-*/
        mobility_parameters_t *p_rrcm_mobility_parameters,
        additional_mobility_parameters_t  *p_rrcm_add_mobility_parameters);

rrmcm_rmif_cdma2000_freq_cells_t *
rrm_ue_match_1xrtt_daho_cell_from_ncl
(
 rrm_ue_context_t *p_rrm_ue_context,
 inter_rat_ncl_t  *p_inter_rat_ncl,
 rrmcm_idle_mode_mobility_irat_cdma_params_t  *p_irat_cdma_params
 );

rrmcm_rmif_cdma2000_freq_cells_t*
rrm_ue_match_hrpd_daho_cell_from_ncl
(
 rrm_ue_context_t *p_rrm_ue_context,
 inter_rat_ncl_t  *p_inter_rat_ncl,
 rrmcm_idle_mode_mobility_irat_cdma_params_t  *p_irat_cdma_params
 );


rrm_return_et
rrm_rrc_process_cdma2000_mob_info_req
(
 U8 *p_api_buf,
         /*SPR 17777 +-*/
 U16 data_len,
 rrm_ue_global_context_t *p_rrm_ue_glb_ctxt
 );

rrm_return_et
rrm_build_and_send_daho_cdma2000_ho_required
(
 rrm_ue_context_t * p_ue_context,
 U16 transaction_id
 );

rrm_return_et
rrm_build_and_send_cdm2000_ho_required
(
 rrm_ue_context_t  *p_ue_context,
 U16                transaction_id
 );

U8
get_next_priority_rat
(
 U8                            index,
 rrm_ue_context_t              *p_rrm_ue_ctx
 );

rrm_return_et
rrm_rrc_parse_dl_nas_transport_ind_msg
(
 U8                  *p_api_buf,
         /*SPR 17777 +-*/
 U16                 data_len,
 rrm_ue_rrc_msg_t    *p_rrm_ue_rrc_msg
 );

rrm_return_et
rrm_build_and_send_hrpd_daho_ho_required
(
 rrm_ue_context_t  *p_rrm_ue_context,
 U16                transaction_id
 );

rrm_return_et
rrm_build_and_send_1xrtt_daho_ho_required
(
 rrm_ue_context_t  *p_rrm_ue_context,
 U16                transaction_id
 );

rrm_bool_et
rrm_ue_check_dual_rx_support
(
 rrm_ue_context_t  *p_ue_context
 );

rrm_return_et
rrm_build_and_send_daho_1xrtt_ho_required
(
 rrm_ue_context_t    *p_ue_context,
 U16                  trans_id
 );

rrm_return_et
rrm_rrc_parse_intra_enodeb_Ho_ind_msg(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        );

/* UE Positioning changes start */
rrm_return_et
rrm_build_and_send_ecid_meas_response
(
 rrm_ue_context_t   *p_ue_context,
 U32              esmlc_meas_id
 );

rrm_return_et
rrm_build_and_send_ecid_meas_failure (
        rrm_ue_index_t ue_index,
    rrm_cell_index_t cell_index,
        U8 mme_id,
        U8 routing_id,
        U32 lppa_trans_id,
        U32 e_smlc_meas_id,
        rrm_cause_t cause);

rrm_return_et
rrm_build_and_send_ecid_meas_failure_indication (
        rrm_ue_index_t ue_index,
    rrm_cell_index_t cell_index,
        U8 mme_id,
        U8 routing_id,
        U32 lppa_trans_id,
        U32 e_smlc_meas_id,
        U32 enb_ue_meas_id,
        rrm_cause_t cause);

rrm_return_et
rrm_build_and_send_ecid_meas_error_req (
        rrm_ue_index_t ue_index,
    rrm_cell_index_t cell_index,
        U8 mme_id,
        U8 routing_id,
        U32 lppa_trans_id,
        rrm_cause_t cause,
        lppa_criticality_diagno_t  criticality_diagnostics);

rrm_return_et
rrm_build_and_send_meas_config_req_for_ue_positioning(
        rrm_ue_context_t                *p_ue_context,
        U32                              esmlc_meas_id,
        rrm_lppa_meas_peridicity_et      report_interval,
        U32                              report_type,
        U8                               report_config_trigger_qty_bitmask,
        /* RACH_OPTIMIZATION_CHANGES_START */
        rrm_bool_et                      is_req_for_rach_info
        /* RACH_OPTIMIZATION_CHANGES_END */
        );
rrm_return_et
rrm_rrc_parse_s1ap_rrm_lppa_ecid_meas_init_req(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        );

rrm_return_et 
rrm_fill_meas_config_req_for_ue_positioning (
        rrm_meas_config_t *p_meas_config,
        U8                meas_object_id,
        rrm_ue_context_t  *p_ue_context,
        U32                esmlc_meas_id,
        rrm_lppa_meas_peridicity_et report_interval,
        U32                    report_type,
        U8                     report_config_trigger_qty_bitmask,
        /* RACH_OPTIMIZATION_CHANGES_START */
        rrm_bool_et                      is_req_for_rach_info
        /* RACH_OPTIMIZATION_CHANGES_END */
        );
rrm_void_t
rrm_uem_deallocate_meas_user_id_for_ue_positioing
(
 rrm_ue_context_t  *p_ue_context,
 U8  meas_id,
 U8  report_config_id
 );
rrm_return_et
rrm_rrc_parse_s1ap_rrm_lppa_ecid_meas_termination_cmd(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        );

rrm_return_et
rrm_rrc_parse_s1ap_rrm_lppa_error_ind(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        );

rrm_bool_et 
rrm_ue_is_ecid_meas_already_exist(
        rrm_ue_context_t               *p_ue_context,
        U32                             esmlc_meas_id,
        U8                              *p_ue_esmlc_ue_index
        );

/*BUG 680 FIX START*/
rrm_return_et rrm_map_erab_to_release(
        rrm_ue_ho_erab_to_release_t *p_rrm_ue_ho_erab_to_release ,
        erab_to_release_t *p_erab_to_release
        );
rrm_return_et rrm_map_erab_to_release_list(
        rrm_ue_ho_erab_to_release_list_t *p_rrm_ue_ho_erab_to_release_list,
        erab_to_release_list_t *p_rrc_erab_to_release_list);
rrm_return_et rrm_map_erab_sub_to_fwd_list(
        rrm_ue_ho_erab_info_list_t *p_rrm_ue_ho_erab_info_list ,
        rrc_erab_info_list_t *p_rrc_erab_info_list);
rrm_return_et rrm_map_rrm_ue_proc_ho_command_request(
        rrm_ue_proc_ho_command_request_t *p_rrm_ho_proc_cmd_req,
        rrc_rrm_ue_ho_command_request_t *p_rrc_rrm_ue_ho_command_request
        );
rrm_return_et rrm_map_forbidden_lac(
        rrm_forbidden_lac_list_t *p_rrm_forbidden_lac_list,
        forbidden_lac_list_t *p_rrc_forbidden_lac_list
        );
rrm_return_et rrm_map_rrm_forbidden_la(
        rrm_forbidden_la_t *p_rrm_forbidden_la,
        forbidden_la_t *p_rrc_forbidden_la
        );
rrm_return_et rrm_map_lac(
        rrm_forbidden_lac_t *p_rrm_forbidden_lac,
        forbidden_lac_t *p_rrc_forbidden_lac
        );

rrm_return_et rrm_map_tac(
        rrm_forbidden_tac_t *p_rrm_forbidden_tac,
        forbidden_tac_t *p_rrc_forbidden_tac
        );
rrm_return_et rrm_map_forbidden_tac(
        rrm_forbidden_tac_list_t *p_rrm_forbidden_tac_list,
        forbidden_tac_list_t *p_rrc_forbidden_tac_list
        );
rrm_return_et rrm_map_rrm_forbidden_ta(
        rrm_forbidden_ta_t *p_rrm_forbidden_ta,
        forbidden_ta_t *p_rrc_forbidden_ta
        );
rrm_return_et rrm_map_rrm_equiv_plmn_list(
        rrm_equiv_plmn_list_t *p_rrm_equiv_plmn_list,
        equiv_plmn_list_t *p_rrc_equiv_plmn_list
        );
rrm_return_et rrm_map_rrm_plmn_identity(
        rrm_plmn_identity_t *p_rrm_plmn_identity ,
        plmn_identity_t  *p_rrc_plmn_identity
        );
rrm_return_et rrm_map_ho_restriction_list(
        rrm_ho_restriction_list_t *p_ho_restriction_list,
        ho_restriction_list_t *p_rrc_ho_restriction_list
        );
rrm_return_et
rrm_map_rrm_ue_proc_ho_failure
(
 rrm_ue_proc_ho_failure_t *p_rrm_ue_proc_ho_failure,
 rrc_rrm_ho_failure_t     *p_rrm_ue_ho_failure
 );

rrm_return_et rrm_map_lai_info(
        rrm_lai_info_t *p_rrc_rrm_lai_info,
        lai_info_t     *p_rrm_lai_info);
/*BUG 680 FIX END*/

/* SPR-19276 START */
rrm_ue_position_et
rrm_get_ue_location
(
 rrm_ue_context_t            *p_ue_ctxt
 );
/* SPR-19276 END */


/*DYNAMIC ICIC START*/
rrm_bool_et
rrm_apply_dynamic_icic_schemes
(
 rrm_ue_context_t *p_rrm_ue_context,
 radio_resource_reconfig_t *radio_resource_reconfig
 );

rrm_bool_et
rrm_reconfig_ue_for_icic
(
 rrm_ue_context_t *p_rrm_ue_context,
 rrm_ue_position_et *p_ue_position_change_status 
 );

rrm_bool_et
rrm_get_updated_dl_p_a_for_ue
(
 rrm_ue_context_t *p_rrm_ue_context,
 rrm_ue_position_et ue_position_change_status
 );


/*DYNAMIC ICIC END*/
rrm_return_et
rrm_cellm_get_num_of_layer_antenna_port
(
 rrm_cell_index_t cell_index,
 U8 *num_of_layer
 );

rrm_return_et
rrm_decide_transmisson_mode(
        rrm_ue_context_t *p_rrm_ue_context,
        transmission_mode_ue_service_profile_et
        *transmission_mode
        );
/* RACH_OPTIMIZATION_CHANGES_START */
rrm_return_et
build_and_send_ue_info_req_to_rrc(
        rrm_ue_context_t *p_rrm_ue_cntxt,
        prach_cause_et               cause
        );

rrm_return_et
rrm_ue_rrc_parse_ue_info_resp(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        );

rrm_return_et
rrm_ue_rrc_parse_ue_sync_status(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        );

rrm_return_et
rrm_ue_rrc_parse_ue_reestab_config_req(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        );
/* RACH_OPTIMIZATION_CHANGES_END */

/* SPR_16053_Fix: Start */
rrm_return_et
rrm_ue_rrc_process_ue_reestab_complete_ind
(
 U8 *p_api_buf,
         /*SPR 17777 +-*/
 U16 data_len,
 rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
         /*SPR 17777 +-*/
 );

rrm_return_et
rrm_ue_rrc_parse_ue_reestab_complete_ind(
        U8 *p_api_buf,
         /*SPR 17777 +-*/
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
);
/* SPR_16053_Fix: End */

/* LIPA_SIPTO_START */

/*! \fn rrm_return_et rrm_build_and_send_lipa_erb_release_ind(
  rrc_rrm_erb_release_ind_t  *p_rrc_rrm_erb_release_ind)
 *  \brief This function build and send the erb release indication
 *  \param p_ue_context UE context
 *  \param ue_index UE Index
 *  \param p_erab_id ERAB identifier
 *  \param count Count Value
 *  \param cause radio network cause
 *  \param transaction_id transaction identifier
 *  \param type rrm_type_et
 */
rrm_return_et
rrm_build_and_send_lipa_erb_release_ind(
    rrc_rrm_erb_release_ind_t  *p_rrc_rrm_erb_release_ind,
    rrm_cell_index_t cell_index);

/* LIPA_SIPTO_END */

/* CSR 00055769  : inter-freq HO code changes start */
rrm_void_t
rrm_map_source_meas_config
(
 rrm_meas_config_t   *p_dst_meas_config,
 rrm_meas_config_t   *p_src_meas_config
 );
/* CSR 00055769  : inter-freq HO code changes end */

/*! \fn rrm_return_et  rrm_build_and_send_ho_adm_resp_failure(rrm_response_t response,rrm_ue_index_t ue_index,rrm_cell_index_t cell_index,U16 transaction_id)
 *  \brief          this function will build and send the HO admission failure response
 *  \param response Response 
 *  \param ue_index
 *  \param cell_index
 *  \param transaction_id transaction identifier
 */
rrm_return_et
rrm_build_and_send_ho_adm_resp_failure(
        rrm_response_t response,
        rrm_ue_index_t ue_index,
        rrm_cell_index_t cell_index,
        U16              transaction_id
        );

/* BUG_11317_FIX_START */
/*Start:Bug 966*/
/*! \fn U8 rrm_olpc_get_delta_sinr(rrm_ue_context_t   *p_rrm_ue_context,
 *                                 U8                 is_pusch_or_pucch_flag)
 *  \brief          this function returns the delta SINR required to reach or 
 *                   above the target sinr. 
 *  \param p_rrm_ue_context ue context
 *  \param U8               is_pusch_or_pucch_flag
 */
U8
rrm_olpc_get_delta_sinr(
    rrm_ue_context_t   *p_rrm_ue_context,
    U8                 is_pusch_or_pucch_flag
);
/* BUG_11317_FIX_END */
/*End:Bug 966*/
/* Start :SPR 9309 */
/* SPR 15674 Start */
rrm_return_et
is_x2_link_up_for_phy_cell
(
 phy_cell_id_info_t phy_cell_id_info,
 rrm_cell_index_t  cell_index
 );
/* SPR 15674 End */
/* End :SPR 9309 */
/* Start CSR: 00058590 */
rrm_bool_et
rrm_is_any_interfreq_interrat_meas_configured
(
 rrm_ue_context_t       *p_ue_context
 );
/* End CSR: 00058590 */

/*SPR-604 Fix Starts*/
/*! \fn rrm_return_et rrm_build_and_send_intra_cell_ho_required(rrm_ue_context_t *p_ue_context,U8 transaction_id)
 *  \brief      this function build and send the HO required for intra-cell HO.
 *  \param  p_ue_context
 *  \param  transaction_id
 */
rrm_return_et
rrm_build_and_send_intra_cell_ho_required(
        rrm_ue_context_t    *p_ue_context,
        U8 transaction_id
        );
/*SPR-604 Fix Ends*/


/* SPR_15259_fix: start */
/* Fix for CSR 58972 start */
/*! \fn     rrm_return_et rrm_build_and_send_meas_config_req_for_cgi(
  rrm_ue_context_t *p_ue_context,
  meas_type_et meas_type)
 *  \brief  Builds the meas config request and sends to RRC for CGI reporting
 *  \param  p_ue_context UE Context Information
 *  \param  meas_type Measurement Type
 */
rrm_return_et
rrm_build_and_send_meas_config_req_for_cgi
(
 rrm_ue_context_t *p_ue_context,
         /*SPR 17777 +-*/
 rrm_ue_rat_et     rrm_cell_rat_type
 );
/* SPR_15259_fix: end */

rrm_bool_et
rrm_is_meas_object_configured_for_frequency
(
 rrm_ue_context_t       *p_ue_context,
 U32                    carrier_earfcn,
 U8                     band_indicator,
 rrm_irat_priority_et   freq_type,
 meas_report_info_node_t  **p_meas_object_config
 );

U8 rrm_uem_allocate_free_id
(
 rrm_ue_context_t            *p_rrm_ue_context,
 rrmc_meas_id_type_et         type
 );

/* SPR_14564_fix: start */
typedef enum
{
    RRM_INTER_FREQ_NONE,
    RRM_INTER_FREQ_FOR_HO,
    RRM_INTER_FREQ_FOR_CGI,
    /* SPR 18431 Fix Start */
    RRM_INTER_FREQ_FOR_SCELL
    /* SPR 18431 Fix End */
}rrm_ue_meas_cause_et;

rrm_void_t
fill_inter_freq_meas_config_object(
        rrm_meas_config_t           *p_meas_config,
        rrm_ue_context_t            *p_rrm_ue_context,
        rrm_ue_meas_cause_et        rrm_inter_freq_meas
        );
/* SPR_14564_fix: end */

/* Fix for CSR 58972 end */

/* CSR00069389 Start */
rrm_ue_meas_config_t*
rrm_find_obj_in_ue_meas_config(
            rrm_rat_type_et              rat_type,
            U16                          arfcn,
            U8                           band_ind,
            U8                          band_class,
            rrm_ue_meas_config_t        *p_meas_config_info
);
/* CSR00069389 End */
/* SPR 12512 Fix Start */
rrm_return_et
rrm_rrc_parse_ue_sync_status_ind(
    U8 *p_api_buf,
         /*SPR 17777 +-*/
    U16 data_len,
    rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
    );
/* SPR 12512 Fix End */

/* CA related function prototype : Start */
rrm_return_et
rrm_remove_scell_data_from_ue_context(
rrm_ue_context_t     *p_rrm_ue_context,
U8                    p_num_of_scell,
rrm_cell_index_t     *p_arr_cell_index);

rrm_return_et
build_and_send_scell_release_to_ue(
rrm_ue_context_t            *p_rrm_ue_context,
U8                           scell_rel_count,
rrm_cell_index_t             *p_scell_cell_index,
/*SPR 13316:start*/
U16                          trans_id);
/*SPR 13316:end*/

rrm_return_et
rrm_fill_scell_to_remove_list(
rrm_ue_context_t            *p_rrm_ue_context,
rrm_cell_index_t             pcell_index,
U8                           p_num_of_scell,
rrm_cell_index_t            *p_arr_cell_index,
U64                          ca_bitmask,
/*SPR 13316:start*/
U16                          trans_id);
/*SPR 13316:end*/

rrm_return_et
rrm_check_cell_index_valid(rrm_cell_index_t p_cell_index);


rrm_return_et
rrm_fill_scell_non_ul_config_pdsch_config_common(
pdsch_config_common_t *p_pdsch_config_common,
rrm_cell_index_t p_scell_index);

rrm_return_et
rrm_fill_scell_non_ul_config_phich_config(
phich_config_t *p_phich_config,
rrm_cell_index_t p_scell_index);

rrm_return_et
fill_rrc_cqi_report_config_scell_r10(
rrc_cqi_report_config_scell_r10_t *p_cqi_report_config_scell,
rrm_cell_index_t                   scell_index,
rrm_ue_context_t                  *p_rrm_ue_context,
U8                                 transmission_mode);

rrm_return_et
fill_radio_res_dedicated_scell_non_ul_config(
rrc_radio_res_dedicated_scell_non_ul_config_t *p_radio_res_dedicated_scell_non_ul_config,
rrm_ue_context_t                              *p_rrm_ue_context,
rrm_cell_index_t                               scell_index,
U64                                            ca_params_bitmask);

rrm_return_et
fill_pdsch_configuration_dedicated(
rrc_phy_pdsch_configuration_dedicated_t *p_pdsch_configuration_dedicated,
rrm_ue_context_t                        *p_rrm_ue_context,
rrm_cell_index_t                        scell_index);

 rrm_return_et
rrm_fill_scell_mac_main_config_extensions(
rrc_scell_mac_main_config_extensions_t *p_mac_main_config_extensions,
rrm_cell_index_t                        p_scell_index,
rrm_ue_context_t                       *p_rrm_ue_context,
U64                                     p_ca_param_bitmask);

rrm_return_et
fill_antenna_info_dedicated_r10(
rrc_antenna_info_dedicated_r10_t *p_antenna_info_dedicated_r10,
rrm_cell_index_t                  scell_index,
rrm_ue_context_t                 *p_rrm_ue_context);

    rrm_return_et
fill_radio_res_dedicated_scell_ul_config(
rrc_radio_res_dedicated_scell_ul_config_t *p_radio_res_dedicated_scell_ul_config,
rrm_cell_index_t                           scell_index,
U64                                        ca_param_bitmask,
rrm_ue_context_t                          *p_rrm_ue_context,
U8                                         transmission_mode);

rrm_return_et
fill_radio_res_config_dedicated_scell_phy_config(
rrc_physical_config_dedicated_scell_t *p_physical_config_dedicated_scell,
rrm_cell_index_t                       p_scell_index,
U64                                    p_ca_param_bitmask,
rrm_ue_context_t                      *p_rrm_ue_context);

rrm_return_et
rrm_fill_scell_radio_res_config_dedicated(
rrc_radio_res_config_dedicated_scell_t *p_radio_res_config_dedicated_scell,
rrm_cell_index_t                        p_scell_index,
U64                                     ca_param_bitmask,
rrm_ue_context_t                       *p_rrm_ue_context);

rrm_return_et
fill_radio_res_common_scell_non_ul_config(
rrc_radio_res_common_scell_non_ul_config_t *p_radio_res_common_scell_non_ul_config,
rrm_cell_index_t                            p_scell_index);

rrm_return_et
rrm_fill_scell_radio_res_config_common(
rrc_radio_res_config_common_scell_t    *p_radio_res_config_common_scell,
rrm_cell_index_t                        p_scell_index);

rrm_return_et
rrm_fill_cell_Identification(
rrc_scell_Identification_t  *p_scell_Id,
rrm_cell_index_t             p_scell_index);
rrm_return_et
rrm_find_valid_scell_index_for_ue(
rrm_ue_context_t  *p_rrm_ue_context,
rrm_cell_index_t   p_scell_cell_index,
U8                *p_scellIndex,
U64                ca_param_bitmask);

rrm_return_et
rrm_fill_scell_to_add_mod_list(
rrm_ue_context_t                 *p_rrm_ue_context,
U8                                p_num_of_scell,
rrm_cell_index_t                 *p_arr_cell_index,
rrc_scell_to_add_mod_list_t      *p_scell_add_mod_list,
U64                               ca_param_bitmask);

rrm_return_et
fill_scell_config_params_in_ue_reconfig(
rrm_ue_context_t        *p_rrm_ue_context,
U8                       p_num_of_scell,
rrm_cell_index_t        *p_arr_cell_index,
rrm_scell_config_t      *p_scell_config,
U64                      p_ca_params_bitmask);

rrm_return_et
rrm_fill_scell_add_params(
rrm_ue_context_t *p_rrm_ue_context,
U8 p_num_of_scell,
rrm_cell_index_t *p_arr_cell_index,
U64               p_ca_params_bitmask);

rrm_return_et
rrm_add_scell_to_ue(
rrm_ue_context_t *p_rrm_ue_context, 
U8 p_num_of_scell, 
rrm_cell_index_t *p_arr_cell_index);
rrm_void_t
rrm_store_scell_list_rcv_in_ho_adm_req
(
    rrm_ue_context_t *p_rrm_ue_context,
         /*SPR 17777 +-*/
    U8               count,
    rrm_cell_index_t cell_index,
    rrm_cell_index_t scell_index
);

void
fill_init_bitmask_for_cell_addition(U64 *ca_param_bitmask);

rrm_return_et
rrm_fill_cqi_reporting_periodic_param_scell_r10(
    rrc_phy_cqi_report_periodic_r10_setup_t *p_rrc_phy_cqi_reporting_periodic_setup_param,
         /*SPR 17777 +-*/
    rrm_ue_context_t *p_rrm_ue_cntxt,
    U8          transmission_mode,
    rrm_ul_dl_freq_selective_sched_et
                    rrm_freq_selective_sched,
    rrm_cell_index_t scell_index);
					
rrm_return_et
rrm_ue_fill_cqi_reporting_periodic_scell_r10(
    rrc_phy_cqi_report_periodic_r10_t *p_rrc_phy_cqi_reporting_periodic,
    rrc_phy_cqi_reporting_periodic_ue_service_profile_t
                           *p_ue_profile_phy_cqi_reporting_periodic,
    rrm_ue_context_t
                           *p_rrm_ue_cntxt,
    U8                     transmission_mode,
    rrm_ul_dl_freq_selective_sched_et
                           rrm_freq_selective_sched,
    rrm_cell_index_t       scell_index);
						   
rrm_return_et
rrm_rrc_ue_fill_cqi_reporting_scell_r10(
    rrc_cqi_report_config_scell_r10_t              *p_rrc_phy_cqi_reporting,
    rrc_phy_cqi_reporting_ue_service_profile_t     *p_ue_profile_phy_cqi_reporting,
    rrm_ue_context_t                               *p_rrm_ue_cntxt,
    U8                                              transmission_mode,
    rrm_cell_index_t                                p_scell_index);

rrm_return_et
fill_phy_config_dedicated_for_pcell(
rrm_ue_context_t                     *p_rrm_ue_context,
rrc_phy_physical_config_dedicated_t  *p_rrc_rrm_ue_reconfig_physical_config_dedicated,
U64                                   p_ca_params_bitmask);

rrm_return_et
fill_mac_config_for_pcell(
rrm_ue_context_t             *p_rrm_ue_context,
rrm_mac_config_t             *p_mac_config);
         /*SPR 17777 +-*/

rrm_return_et
fill_pcell_physical_config_dedicated_r10(
rrc_phy_physical_config_dedicated_t            *p_rrc_phy_physical_config_dedicated,
rrm_ue_context_t                               *p_rrm_ue_context,
U64                                             p_ca_params_bitmask);

void
rrm_extract_r10_rf_params(
   rrm_rf_parameters_v1020_ies_t *p_rrm_rf_param_v1020_ies,
   rf_parameters_v1020_ies_t     *p_rf_param_v1020);
/* CA related function prototype : End */

/*  LAA Rel13 Code + */
/*  LAA Rel13 Code - */

/*INTRA_CELL_HO changes start*/
/*! \fn rrm_return_et rrm_build_and_send_intra_cell_ho_required(rrm_ue_context_t *p_ue_context,U8 transaction_id)
 *  \brief      this function build and send the HO required for intra-cell HO.
 *  \param  p_ue_context
 *  \param  transaction_id
 */
rrm_return_et
rrm_build_and_send_intra_cell_ho_required(
        rrm_ue_context_t    *p_ue_context,
        U8 transaction_id
);
/*INTRA_CELL_HO changes stop*/

/* SPR 9216:MRO enhancement start */
/*!  \fn rrm_return_et rrm_ue_rrc_parse_ue_reestab_config_cnf(U8 *p_api_buf,U16 api_id,U16 data_len,rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg)
 *  \brief this function will parse the UE reestablishment config cnf.
 *  \param  p_api_buf API buffer
 *  \param  api_id API identifier
 *  \param  data_len data length
 *  \param p_rrm_ue_rrc_msg UE RRC message
 */
rrm_return_et
rrm_ue_rrc_parse_ue_reestab_config_cnf
(
    U8 *p_api_buf,
         /*SPR 17777 +-*/
    U16 data_len,
    rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
);
/* SPR 9216:MRO enhancement end */

/*CA Stage3: start*/
rrm_return_et
rrm_fill_a1_a2_report_config(
        rrm_meas_config_t *p_meas_config,
        rrm_ue_context_t  *p_rrm_ue_context,
        U8                *index,
        rrm_cell_index_t  cell_index
        );
/*SPR 16406 Fix Satrt */
/* Code removed */
/*SPR 16406 Fix Satrt */
/*CA Stage3: end*/

/* CA_Stage_3_Scell_TM_change: Start */
rrm_bool_et
rrm_check_scell_index_report_received( rrm_ue_context_t *p_rrm_ue_context,
                                   U8 index,
                                   U8 count );

rrm_bool_et
rrm_get_dl_sinr_cw1_consider_scell( rrm_ue_context_t *p_rrm_ue_context,
         /*SPR 17777 +-*/
                                    U8 index );

rrm_return_et
rrm_get_scell_sinr_pathloss_mobility_levels( rrm_level_t      *p_sinr_level,
                                             rrm_level_t      *p_pathloss_level,
                                             rrm_level_t      *p_mobility_level,
                                             rrm_cell_index_t  scell_index,
                                             rrm_ue_context_t *p_rrm_ue_context );

transmission_mode_ue_service_profile_et 
rrm_get_tm_mode_based_on_scell_tbl(rrm_level_t       sinr_level,
                              rrm_level_t       pathloss_level,
                              rrm_level_t       mobility_level,
                              rrm_ue_context_t  *p_rrm_ue_context,
                              rrm_cell_index_t  scell_index );

rrm_bool_et
rrm_fallback_mode_for_transmission_mode_on_scell( transmission_mode_ue_service_profile_et
                                                                       transmission_mode,
                                                  rrm_ue_context_t     *p_ue_context,
                                                  rrm_cell_index_t      scell_index);

rrm_return_et
rrm_scell_select_tx_mode(rrm_ue_context_t   *p_ue_context,
                         rrm_cell_index_t    scell_index,
                         transmission_mode_ue_service_profile_et
                                            *p_transmission_mode);

rrm_return_et
rrm_scell_check_scell_tm_change_condition(U8                   port,
                                          rrm_ue_context_t    *p_ue_context,
                                          rrm_ue_category_et   ue_category,
                                          rrm_cell_index_t     scell_index,
                                          transmission_mode_ue_service_profile_et
                                                              *p_transmission_mode);

rrm_return_et
rrm_set_scell_transmission_mode(transmission_mode_ue_service_profile_et 
                                                          transmission_mode,
                        	rrc_antenna_info_dedicated_r10_t 
                                                    *p_antenna_info_dedicated_r10,
	                        rrm_ue_context_t    *p_rrm_ue_context,
                                rrm_cell_index_t     scell_index );

rrm_return_et
rrm_fill_scell_transmission_mode_r10(
                         rrc_antenna_info_dedicated_r10_t *p_antenna_info_dedicated_r10,
                         rrm_cell_index_t                  scell_index,
                         rrm_ue_context_t                 *p_rrm_ue_context,
                         rrm_bool_et                      *p_antenna_match);

rrm_void_t 
rrm_fill_scell_tm1( rrc_antenna_info_dedicated_r10_t *p_antenna_info_dedicated_r10,
                              antenna_info_t                   *p_antenna_info,
                              rrm_ue_context_t                 *p_ue_context,
                              rrm_cell_index_t                  scell_index);

rrm_void_t 
rrm_fill_scell_tm2( rrc_antenna_info_dedicated_r10_t *p_antenna_info_dedicated_r10,
                              antenna_info_t                   *p_antenna_info,
                              rrm_ue_context_t                 *p_ue_context,
                              rrm_cell_index_t                  scell_index);

rrm_void_t 
rrm_fill_scell_tm3( rrc_antenna_info_dedicated_r10_t *p_antenna_info_dedicated_r10,
                              antenna_info_t                   *p_antenna_info,
                              rrm_ue_context_t                 *p_ue_context,
                              rrm_cell_index_t                  scell_index);

rrm_void_t 
rrm_fill_scell_tm4( rrc_antenna_info_dedicated_r10_t *p_antenna_info_dedicated_r10,
                              antenna_info_t                   *p_antenna_info,
                              rrm_ue_context_t                 *p_ue_context,
                              rrm_cell_index_t                  scell_index);

rrm_void_t 
rrm_fill_scell_tm5( rrc_antenna_info_dedicated_r10_t *p_antenna_info_dedicated_r10,
                              antenna_info_t                   *p_antenna_info,
                              rrm_ue_context_t                 *p_ue_context,
                              rrm_cell_index_t                  scell_index);

rrm_void_t 
rrm_fill_scell_tm6( rrc_antenna_info_dedicated_r10_t *p_antenna_info_dedicated_r10,
                              antenna_info_t                   *p_antenna_info,
                              rrm_ue_context_t                 *p_ue_context,
                              rrm_cell_index_t                  scell_index);

rrm_void_t 
rrm_fill_scell_tm7( rrc_antenna_info_dedicated_r10_t *p_antenna_info_dedicated_r10,
                              antenna_info_t                   *p_antenna_info,
                              rrm_ue_context_t                 *p_ue_context,
                              rrm_cell_index_t                  scell_index);

rrm_void_t 
rrm_fill_scell_tm8( rrc_antenna_info_dedicated_r10_t *p_antenna_info_dedicated_r10,
                              antenna_info_t                   *p_antenna_info,
                              rrm_ue_context_t                 *p_ue_context,
                              rrm_cell_index_t                  scell_index);

rrm_return_et 
rrm_assign_aperiodic_mode_from_tm_mode_r10(rrm_ue_context_t                    *p_rrm_ue_cntxt, 
                                       rrm_ul_dl_freq_selective_sched_et    freq_selective_sched,
                                       U8                                   transmission_mode,
                                       U32                                 *p_aperiodic_mode,
                                       const rrm_tm_mode_additional_info_t *p_rrm_tm_mode_additional_info);

rrm_return_et
rrm_fill_aperiodic_csi_trigger( rrm_ue_context_t *p_rrm_ue_context,
             rrc_phy_cqi_report_aperiodic_r10_setup_csi_trigger_t *p_aperiodic_csi_trigger );

rrm_bool_et
rrm_check_scell_report_count( rrm_ue_context_t      *p_rrm_ue_context,
                              U8                    *scell_valid_rep_count,
                              rrm_cell_index_t      *p_arr_cell_index);

rrm_void_t
rrm_calc_ca_param_bitmask(U64    *ca_params_bitmask);


rrm_return_et
rrm_select_cqi_mode_for_cell( rrm_cqi_reporting_mode_et oam_config_cqi_mode,
                               rrm_ue_context_t      *p_rrm_ue_context,
                               U8                    *selected_cqi_mode);

rrm_return_et
rrm_get_scell_transmission_mode(
     rrm_ue_context_t *p_rrm_ue_context,
     rrm_cell_index_t scell_cell_index,
     transmission_mode_ue_service_profile_et *transmission_mode);

/* CA_Stage_3_Scell_TM_change: End */
/*SPR 10329:start*/
/* SPR 15674 Start */
rrm_bool_et 
is_phy_cell_support_x2_neighbour 
(
 lte_ncl_t *p_ncl_params,
 U8   ncl_count,
 phy_cell_id_info_t phy_cell_id_info,
 rrmcm_rmif_cell_x2_status_et  *p_x2_status, /*OUT :  If the target has an X2 link up */
         /*SPR 17777 +-*/
 rrm_bool_et intra_freq_cell_present
 );

/* SPR 15674 End */


/*SPR 10329:end*/
/*SPR 11535:start*/
rrm_return_et rrm_handle_ue_inactive(rrm_ue_index_t ue_index,
                                     rrm_cell_index_t cell_index,
                                     rrm_ue_global_context_t *p_rrm_glb_ctxt);
/*SPR 11535:end*/
/* SPR 12705 start */
rrm_void_t rrm_update_kpi_ho_fail_stats(rrm_ue_context_t *p_ue_context, rrm_cell_context_t  *p_cell_ctxt);
rrm_void_t rrm_update_kpi_ho_attempt_stats(rrm_ue_context_t *p_ue_context, rrm_cell_context_t  *p_cell_ctxt);
/* SPR 12705 end */

/*ho_report_change start*/
rrm_return_et rrm_fill_reestab_ho_failure_report(
         /*SPR 17777 +-*/
		rrm_ue_context_t *p_ue_context
		);
/*ho_report_change end*/

/* SSI_CHANGES_START */
    /* eICIC_PHASE_1_2_CHANGES_START */
rrm_void_t
rrm_rrc_ue_fill_periodic_sub_wide_band
(
    rrc_phy_cqi_report_periodic_r10_setup_t    *p_rrc_phy_cqi_report_periodic_r10_setup,
    rrm_ue_context_t *p_rrm_ue_cntxt,
    rrm_ul_dl_freq_selective_sched_et rrm_freq_selective_sched,
    rrm_return_et    *ret_val
);
/* SPR 14419 start */
rrm_return_et
rrm_rrc_ue_adm_resp_fill_antenna_information(
        rrm_ue_context_t *p_rrm_ue_context,
        rrc_phy_physical_config_dedicated_t    *p_phy_config_dedicated
        );
/* SPR 14419 end */
/*CID:83013 start*/
rrm_void_t rrm_copy_tdd_bits_from_src_abs_to_dest
(
 U8  abs_size,
 U8 *p_src_abs_pattern,
 U8 *p_dest_abs_pattern
 );
/*CID:83013 end*/

    /* eICIC_PHASE_1_2_CHANGES_END */
/* SSI_CHANGES_END */
/* SPR 14326 Fix Start */
rrm_bool_et
rrm_check_send_meas_gap_is_enabled
(
  U32 operating_freq,
  U32 measuring_freq,
 rrm_ue_context_t           *p_ue_context,
  rrm_meas_gap_band_type_et
      meas_gap_band_type,
rrm_meas_gap_freq_type_et
            meas_gap_freq_type,
 rrm_cdma_meas_gap_config_t *p_cdma_meas_gap_config
);
U8
rrm_ue_get_dl_freq_specific_band
(
 U32 frequency
 );
rrm_void_t
rrm_get_pos_in_supported_band_list
(
  rrm_ue_eutra_radio_capability_info_def_t *p_radio_capability_info,
  U8   operating_band,
  rrm_meas_gap_freq_type_et
            meas_gap_freq_type,
  U8   *p_operating_band_idx,
  U8    cdma_type
 );
rrm_void_t
rrm_get_freq_specific_bands
(
    U32    measuring_freq,
    rrm_meas_gap_freq_type_et
            meas_gap_freq_type,
    U8     *p_measuring_band
);

U8
rrm_ue_get_geran_freq_band
(
  U32 geran_freq
);

U32
rrm_ue_get_utra_freq_band
(
  U32 utran_freq
);
/* SPR 14326 Fix End */

/*SPR_15852_FIX:start*/
 rrm_void_t rrm_uem_deallocate_used_id
(
 rrm_ue_context_t           *p_rrm_ue_context,
 rrmc_meas_id_type_et        meas_type,
 U8                          used_id
 );
/*SPR_15852_FIX:end*/
/* SPR-16447 START */
rrm_bool_et
check_geran_carrier_freq
(
 /* Spr 18401 Changes Start */
 rrm_son_geran_following_arfcns_t  following_arfcns,
 U16         starting_arfcn,
 /* Spr 18401 Changes End */
 U16         bcch_arfcn
 );
/* SPR-16447 END */
/* SPR 16406 8SEP start */
rrm_return_et
rrm_remove_previous_cgi_from_ue(
        rrm_ue_context_t *p_ue_context,
        rrc_rrm_meas_config_req_t       *p_rrc_rrm_meas_config_req);
rrm_return_et
rrm_clear_pending_anr_cgi(
        rrm_ue_context_t *p_ue_context);
rrm_return_et
rrm_remove_previous_pending_measurement(rrm_ue_context_t *p_ue_context);
/* SPR 16406 8SEP end */
/* SPR_17797_FIX_START */
rrm_return_et
rrm_get_index_of_inter_rat_band_list(
  rrm_ue_inter_rat_Parameters_t *p_inter_rat_parameters,
  U8                            *p_count,
  rrm_meas_gap_freq_type_et     meas_gap_freq_type,
  U8                            cdma_type
 );
/* SPR_17797_FIX_END */
/* Spr 18401 Changes Start */
rrm_void_t
fill_geran_meas_object_to_add_mod_list_for_cgi(
        rrm_cell_context_t          *p_cell_ctx,
        meas_geran_node_t           *p_meas_geran_node,
        meas_object_to_add_mod_t    *p_meas_object_to_add_mod,
        U8                          meas_obj_id,
        U16                         carr_freq
        ); 
 /* Spr 18401 Changes End */
/*SPR_17893_START*/
/*! \fn	rrm_return_et rrm_ue_rrc_process_ue_capability_enq_resp(U8 *p_api_buf,U16 api_id,
 *        U16 data_len,rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg)
 *  \brief This function process the UE Capability Enquiry Response.
 *  \param  p_api_buf API Buffer
 *  \param  api_id API identifier
 *  \param  data_len data length
 *  \param p_rrm_ue_rrc_msg UE RRC message
 */
rrm_return_et
rrm_ue_rrc_process_ue_capability_enq_resp(
        U8 *p_api_buf,
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        );
/*! \fn	rrm_bool_et rrm_populate_and_send_utran_cap_enq_req(rrm_ue_context_t *p_ue_context)
 *  \brief This function populates and send the UE capability enq request for UTRAN.
 *  \param  p_ue_context
 */
rrm_bool_et
rrm_populate_and_send_utran_cap_enq_req(rrm_ue_context_t *p_ue_context);
/*! \fn	rrm_bool_et rrm_check_if_utran_cap_enq_reqd(rrm_ue_context_t *p_ue_context)
 *  \brief This function checks for the conditions if the Utran capability Enq is required.
 *  \param  p_ue_context
 */
rrm_bool_et
rrm_check_if_utran_cap_enq_reqd(rrm_ue_context_t *p_ue_context);
/*SPR_17893_END*/

rrm_void_t
store_meas_report_info
(
 rrm_meas_config_t          *p_meas_config,
 rrm_ue_context_t           *p_rrm_ue_context,
 U8                          meas_id,
 rrm_meas_func_ptr          meas_func_ptr,
 meas_type_et               meas_type
);

#ifdef ENDC_ENABLED
/* ENDC_MEAS_CHANGES_START */
rrm_return_et
fill_eutra_nr_meas_info
(
  rrm_meas_config_t*   p_meas_config,
  rrm_ue_context_t*    p_rrm_ue_ctx
);

rrm_bool_t
rrm_check_trigger_to_configure_nr_meas
(
  rrm_ue_context_t           *p_ue_context,
  rrm_nr_meas_trigger_type_et trigger_type
);
#endif
/* ENDC_MEAS_CHANGES_END */

/* SPR 21958 PUSCH RAT1 Support Start */
rrm_bool_et
rrm_check_is_ue_support_uplink_rat1(rrm_ue_context_t *p_rrm_ue_context);
/* SPR 21958 PUSCH RAT1 Support End */


#ifdef ENDC_ENABLED
/* MENB_CHANGES_START */
/***********************************************************************************
 *   FUNCTION NAME: rrm_uem_fill_ue_eutra_endc_capability_v1510_ies
 *   INPUT        : p_rrc_ue_eutra_capability_v1510_ies_t
 *   OUTPUT       : p_rrm_ue_eutra_capability_v1510_ies
 *   DESCRIPTION  : Populate the EN-DC ue capability params received UE Capability
 *                  Ind from RRC in rrm_ue_rrc_msg_t pointer.
 *   RETURNS      : none
 ***********************************************************************************/
rrm_void_t
rrm_uem_fill_ue_eutra_endc_capability_v1510_ies
(
    rrm_ue_eutra_capability_v1510_ies_t *p_rrm_ue_eutra_capability_v1510_ies,
    ue_eutra_capability_v1510_ies_t     *p_rrc_ue_eutra_capability_v1510_ies
);
/* MENB_CHANGES_END */
/* OPTION3X Changes Start */
/******************************************************************************
 *   FUNCTION NAME: rrm_ue_rrc_parse_ue_dc_bearer_change_cnf 
 *   INPUT      : p_api_buf,api_id,data_len,p_rrm_ue_rrc_msg
 *   OUTPUT     : none
 *   DESCRIPTION:
 *       Parse the UE DC Bearer Change CNF message from L3 
 *   RETURNS:
 *       RRM_SUCCESS on Success
 *       RRM_FAILURE on Failure
 *****************************************************************************/
rrm_return_et
rrm_ue_rrc_parse_ue_dc_bearer_change_cnf(
        U8 *p_api_buf,
        U16 data_len,
        rrm_ue_rrc_msg_t *p_rrm_ue_rrc_msg
        );
/******************************************************************************
 *   FUNCTION NAME: rrm_build_and_send_dc_bearer_change_req
 *   INPUT      : p_rrm_ue_cntxt,p_erb_change_list,erb_change_list_count
 *   OUTPUT     : response
 *   DESCRIPTION:
 *                 Fills and sends DC Bearer Change Request
 *   RETURNS    :
 *                 RRM_SUCCESS  on Success
 *                 RRM_FAILURE  on Failure
 ******************************************************************************/
rrm_return_et
rrm_build_and_send_dc_bearer_change_req(
    rrm_ue_context_t             *p_ue_context,
    endc_eligible_erab_list_t    *p_erb_change_list,
    U8                           erb_change_list_count,
    rrm_meas_result_nr_neighbour_list_r15_t  *p_meas_result_nr,
    rrm_x2_gb_gnb_id_t            *p_selected_gnb_id
);
/* OPTION3X Changes End */

rrm_void_t 
rrm_fill_nr_cg_config_info
(
 rrm_ue_context_t           *p_rrm_ue_context,
 rrc_rrm_sgnb_add_param_t   *p_sgnb_add_param,
 rrm_x2_gb_gnb_id_t         *p_selected_gnb_id,
 rrm_meas_result_nr_neighbour_list_r15_t  *p_meas_result_nr
);

/* NR_BAND_FIX_START */
rrm_bool_et rrm_check_is_ue_support_endc
(
    rrm_ue_context_t    *p_ue_context
);

rrm_bool_et
rrm_check_frequancy_band
(
    rrm_ue_context_t              *p_rrm_ue_context
);
/* NR_BAND_FIX_END */
rrm_return_et
fill_ssb_subcarrier_spacing_meas_obj_nr
(
  U8 *p_meas_obj_ssb_r15,
  U8 x2_ssb_spacing
);


#endif
#endif
