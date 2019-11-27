/*******************************************************************************
*
*  FILE NAME   : asn_enc_wrappers.c
*
*  DESCRIPTION : This file contains the wrapper function of ASN Encoders. 
*
*  DATE	24-June-2010  NAME Ashutosh Gupta  REFERENCE  36.423_880(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/

#include "rrc_defines.h"
#include "rtxsrc/rtxCommon.h"
#include "x2ap_asn_enc_dec_3gpp.h"
#include "asn_enc_wrappers.h"
#include "x2ap_tracing.h"
#include "x2ap_utils.h"
#include "rrc_x2apCommon_il_composer.h"
#include "rrc_logging.h"
#include "x2ap_uecc.h"

#define X2AP_INTERFACE_TO_TRACE_BIT_STR_LEN    8 
#define X2AP_MACRO_ENB_ID_BITSTRING_SIZE     20
#define X2AP_HOME_ENB_ID_BITSTRING_SIZE      28
#define X2AP_EUTRAN_CELL_ID_BIT_STR_LEN      28
#define X2AP_KEY_ENB_STAR_BIT_STR_LEN      256
#define X2AP_RECV_STATUS_UL_PDCP_SDU_BIT_STR_LEN   4096   
#define X2AP_CRNTI_BITSTRING_SIZE       16
#define X2AP_SHORT_MAC_I_BITSTRING_SIZE 16


x2ap_return_et x2ap_compose_serv_cell_info_new(
	x2ap_gb_context_t* p_x2ap_gb_ctx,
	OSCTXT                         *p_asn1_ctx,
	x2ap_ServedCell_Information    *p_x2ap_IE_serv_cell,
	x2ap_served_cell_info_t         *p_serv_cell);

x2ap_return_et x2ap_compose_csg_id_info_new(
	OSCTXT* p_asn1_ctx, 
	x2ap_ServedCell_Information_iE_Extensions* p_served_cell_extn, 
	x2ap_served_cell_info_t* p_serv_cell);

x2ap_return_et x2ap_compose_csg_id_info(
	OSCTXT* p_asn1_ctx, 
	x2ap_ServedCell_Information_iE_Extensions* p_served_cell_extn, 
	x2ap_srvd_cell_arry_t* p_serv_cell);

x2ap_return_et x2ap_compose_mbsfn_info(
	OSCTXT* p_asn1_ctx, 
	x2ap_ServedCell_Information_iE_Extensions* p_served_cell_extn, 
	x2ap_served_cell_info_t* p_serv_cell);

x2ap_return_et x2ap_compose_prach_config_info(
	OSCTXT* p_asn1_ctx, 
	x2ap_ServedCell_Information_iE_Extensions* p_served_cell_extn, 
        x2ap_prach_configuration_info_t* p_prach_config);

x2ap_return_et x2ap_compose_num_atenna_info(
	OSCTXT* p_asn1_ctx, 
	x2ap_ServedCell_Information_iE_Extensions* p_served_cell_extn,
/* SPR 8178 Start */ 
	x2ap_srvd_cell_arry_t* p_serv_cell);

x2ap_return_et x2ap_compose_num_atenna_info_new(
	OSCTXT* p_asn1_ctx, 
	x2ap_ServedCell_Information_iE_Extensions* p_served_cell_extn,
	x2ap_served_cell_info_t* p_serv_cell);
/* SPR 8178 End */ 

x2ap_return_et x2ap_compose_served_cell_new(
	x2ap_gb_context_t* p_x2ap_gb_ctx,
	OSCTXT                      *p_asn1_ctx,
	OSRTDList                   *p_x2ap_IE_serv_cell,
	x2ap_served_cell_info_arr_t      *p_serv_cell,
	U16                          num_served_cell);

x2ap_return_et x2ap_compose_CriticalityDiagnostics(
        OSCTXT                       *p_asn1_ctx,
        x2ap_CriticalityDiagnostics  *p_x2ap_critic_diag,
        x2ap_crit_diagno_t           *p_critic_diag);

x2ap_return_et x2ap_compose_global_enb_id(
        OSCTXT              *p_asn1_ctx,
        x2ap_GlobalENB_ID   *p_x2ap_IE_gb_enb_id,
        x2_gb_enb_id_t         *p_gb_enb_id);

x2ap_return_et x2ap_compose_GUGroupIDList(
        OSCTXT              *p_asn1_ctx,
        OSRTDList           *p_x2ap_IE_gu_grp_id_list,
        x2ap_gu_group_id_t  *p_gu_group_id,
        U8                   num_gu_group_id);

x2ap_return_et x2ap_compose_served_cells_to_modify(
		x2ap_gb_context_t* p_x2ap_gb_ctx,
		OSCTXT                         *p_asn1_ctx,
        OSRTDList                      *p_x2ap_IE_serv_cell_to_mod,
        x2ap_served_cells_to_modify_info_t  *p_serv_cell_to_mod,
        U8                              num_served_cell_to_mod);

x2ap_return_et x2ap_compose_old_ECGIs(
		x2ap_gb_context_t* p_x2ap_gb_ctx,
		OSCTXT                         *p_asn1_ctx,
        OSRTDList                      *p_x2ap_IE_serv_cell_to_del,
        x2ap_served_cells_to_delete_list_t       *p_serv_cell_to_del);

x2ap_return_et x2ap_compose_served_cell(
        OSCTXT                  *p_asn1_ctx,
        OSRTDList               *p_x2ap_IE_serv_cell,
        x2ap_srvd_cell_arry_t   *p_serv_cell,
        U16                     num_served_cell);

x2ap_return_et x2ap_compose_serv_cell_info(
        OSCTXT                         *p_asn1_ctx,
        x2ap_ServedCell_Information    *p_x2ap_IE_serv_cell,
        x2ap_srvd_cell_arry_t          *p_serv_cell);

void compose_x2ap_FDD_Info(
        x2ap_FDD_Info                  *p_x2ap_fdd_info,
        x2ap_choice_eutra_mode_info_t  *p_eutra_mode_info);

void compose_x2ap_TDD_Info(
        x2ap_TDD_Info                  *p_x2ap_tdd_info,
        x2ap_choice_eutra_mode_info_t  *p_eutra_mode_info);

x2ap_return_et x2ap_compose_cause(
        x2ap_Cause         *p_x2ap_cause,
        x2_cause_t         *p_cause);
x2ap_return_et x2ap_compose_ecgi(
        x2ap_ECGI     *p_asn_ecgi,
        x2ap_ecgi_t   *p_rrc_ecgi);

void x2ap_send_enb_config_update_response(
        x2ap_gb_context_t* p_x2ap_gb_ctx,
        x2ap_enb_config_update_resp_t* p_enb_config_response,
        U8 result, U16 dst_id, U16 trans_id);

extern void x2ap_reset_compose_asn_x2ap_cause(
        x2ap_cause_t cause, x2ap_Cause* p_asn_cause);

x2ap_return_et x2ap_compose_nr_neighbour_info_list(
        OSCTXT                         *p_asn1_ctx,
        x2ap_NRNeighbour_Information   *p_x2ap_nr_neigh_info,
        x2ap_nr_neigh_cell_arry_t       neigh_cell_arr[],
        U16                             num_neigh_cell);

 x2ap_return_et compose_x2ap_NR_Neigh_FDD_Info(
              OSCTXT                                              *p_asn1_ctx,
              x2ap_FDD_InfoNeighbourServedNRCell_Information      *p_tar_x2ap_nr_fdd_info,
              x2ap_nr_neigh_mode_info_t                           *p_src_nr_neigh_mode);

x2ap_return_et compose_x2ap_NR_Neigh_TDD_Info(
              OSCTXT                                              *p_asn1_ctx,
              x2ap_TDD_InfoNeighbourServedNRCell_Information      *p_tar_x2ap_nr_tdd_info,
              x2ap_nr_neigh_mode_info_t                           *p_src_nr_neigh_mode);





#ifdef ENDC_ENABLED
/******************************************************************************
   * Function Name  : x2ap_compose_nr_neighbour_info_dc
   * Inputs         : OSCTXT                     *p_asn1_ctx.
   *                  OSRTDList                  *p_x2ap_IE_neigh_info
   *                  x2ap_nr_neigh_cell_arry_t  *p_serv_cell
   *                  U16                         num_neigh_cell
   * Outputs        : OSRTDList                  *p_x2ap_IE_neigh_info
   * Returns        : X2AP_SUCCESS/X2AP_FAILURE
   * Description    : This function populates p_x2ap_IE_neigh_cell with
                      information necessary to encode nrneighbour Info.
*****************************************************************************/
x2ap_return_et
x2ap_compose_nr_neighbour_info_dc
(
    OSCTXT                     *p_asn1_ctx,
    x2ap_NRNeighbour_Information *p_nr_neigh_elem,
    x2ap_nr_neigh_cell_arry_t    neigh_cell_arr[],
    U16                         num_neigh_cell
);
/* MENB CHANGES - START */
/******************************************************************************
 * Function Name  : x2ap_compose_served_cell_dc
 * Inputs         : OSCTXT                   *p_asn1_ctx.
 *                  OSRTDList                *p_x2ap_IE_serv_cell
 *                  x2ap_srvd_cell_arry_t    *p_serv_cell
 *                  U16                      num_served_cell
 * Outputs        : OSRTDList                *p_x2ap_IE_serv_cell
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function populates p_x2ap_IE_serv_cell with
 *                  information necessary to encode served cell Info.
 *****************************************************************************/
x2ap_return_et
x2ap_compose_served_cell_dc
(
    OSCTXT                     *p_asn1_ctx,
    OSRTDList                  *p_x2ap_IE_serv_cell,
    en_dc_x2_setup_req_t       *p_x2_endc_setup_req
);

void x2ap_send_endc_config_update_response
(
    x2ap_gb_context_t* p_x2ap_gb_ctx, 
    x2ap_endc_config_update_resp_t* p_endc_config_response, 
    U8 result, U16 dst_id, U16 trans_id
);

x2ap_return_et x2ap_compose_partial_list
(
    x2ap_gb_context_t* p_x2ap_gb_ctx,
    OSCTXT                      *p_asn1_ctx,
    OSRTDList                   *p_x2ap_IE_serv_cell,
    x2ap_partial_list_t      *p_serv_cell
);

x2ap_return_et x2ap_compose_eutra_nr_served_cell
(
    x2ap_gb_context_t* p_x2ap_gb_ctx,
    OSCTXT                      *p_asn1_ctx,
    OSRTDList                   *p_x2ap_IE_serv_cell,
    x2ap_served_eutra_nr_cell_info_arr_t      *p_serv_cell,
    U16                          num_served_cell
);

x2ap_return_et x2ap_compose_eutra_nr_served_cell_mod_arr
(
    x2ap_gb_context_t* p_x2ap_gb_ctx,
    OSCTXT                      *p_asn1_ctx,
    OSRTDList                   *p_x2ap_IE_serv_cell,
    x2ap_served_eutra_nr_cells_to_modify_info_t      *p_serv_cell_mod,
    U16                          num_served_cell
);


/* MENB CHANGES - END */
#endif

/*****************************************************************************
 * Function Name  : x2ap_compose_CriticalityDiagnostics
 * Inputs         : p_asn1_ctx - Pointer to OSCTXT.
 *                 p_x2ap_critic_diag - pointer to x2ap_CriticalityDiagnostics
 *                  p_critic_diag - This contains the information to encode global
 *                            eNB ID.
 * Outputs        : p_x2ap_critic_diag - This structure is populated with
 *                                 information from p_x2ap_cause.
 * Returns        : X2AP_SUCCESS - Asn Encoding was successful.
 *                  X2AP_FAILURE - Asn Encoding was not successful.
 * Description    : This function populates p_x2ap_cause with
 *                  information necessary to encode global eNb ID.
 *****************************************************************************/
x2ap_return_et x2ap_compose_CriticalityDiagnostics(
        OSCTXT                       *p_asn1_ctx,
        x2ap_CriticalityDiagnostics  *p_x2ap_critic_diag,
        x2ap_crit_diagno_t          *p_critic_diag)
{
    x2ap_return_et              retVal = X2AP_SUCCESS;
    OSRTDListNode               *p_node = X2AP_P_NULL;
    U16                         critic_diagno_count = 0;
    OSRTDList                   *p_criticality_diagnostics_info = X2AP_P_NULL;
    x2ap_CriticalityDiagnostics_IE_List_element *p_critic_diag_info = X2AP_P_NULL;

    X2AP_UT_TRACE_ENTER();
    p_x2ap_critic_diag->m.procedureCodePresent = 1;
    p_x2ap_critic_diag->m.triggeringMessagePresent = 1;
    p_x2ap_critic_diag->m.procedureCriticalityPresent = 1;

    /*fill procedureCode, triggeringMessage and procedureCriticality */
    p_x2ap_critic_diag->procedureCode =  
        (x2ap_ProcedureCode)(p_critic_diag->procedure_code);
    p_x2ap_critic_diag->triggeringMessage = p_critic_diag->trigger_msg;
    p_x2ap_critic_diag->procedureCriticality = p_critic_diag->proc_critic;

    /*if there is any criticality diagnostic information */
    if (p_critic_diag->num_critic_diagno)
    {
        p_x2ap_critic_diag->m.iEsCriticalityDiagnosticsPresent = 1;
        p_criticality_diagnostics_info = 
            &p_x2ap_critic_diag->iEsCriticalityDiagnostics;
    }
    /*** compose criticality_diagnostics_info info start ***/
    for (critic_diagno_count = 0;
            critic_diagno_count < p_critic_diag->num_critic_diagno;
            critic_diagno_count++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx,
                x2ap_CriticalityDiagnostics_IE_List_element,
                &p_node,
                &p_critic_diag_info);
        if (X2AP_P_NULL == p_node)
        {
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            return X2AP_FAILURE;
        }
        asn1Init_x2ap_CriticalityDiagnostics_IE_List_element(p_critic_diag_info);

        p_critic_diag_info->iECriticality = 
            p_critic_diag->critic_daigno_info[critic_diagno_count].
                                                         ie_ctiticality;
        p_critic_diag_info->iE_ID = (x2ap_ProtocolIE_ID)(p_critic_diag->
                                    critic_daigno_info[critic_diagno_count].
                                    ie_id);
        p_critic_diag_info->typeOfError = p_critic_diag->
                                    critic_daigno_info[critic_diagno_count].
                                    type_of_error;

        rtxDListAppendNode (p_criticality_diagnostics_info, p_node);
    }
    X2AP_UT_TRACE_EXIT();
    return retVal;        
}
/*****************************************************************************
 * Function Name  : x2ap_encode_eNB_Configuration_Update_failure_resp 
 * Inputs         : p_enb_conf_upd_fail - pointer to x2ap_enb_conf_upd_fail_t
 *                  p_asn_msg - pointer to ASN encoded msg
 *                  p_asn_msg_len - pointer to ASN encoded msg length 
 * Outputs        : p_asn_msg - ASN encoded buffer
 *                  p_asn_msg_len - length of  ASN encoded buffer
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to encode eNodeB configuration failure 
 *                  responce
 * ERROR          : This function report error to the caller function, If encoder
 *                  fails to encode message.
 ********************************************************************************/
x2ap_return_et x2ap_encode_eNB_Configuration_Update_failure_resp(
        x2ap_enb_conf_upd_fail_t *p_enb_conf_upd_fail,
        U8                        *p_asn_msg,
        U16                       *p_asn_msg_len)
{
    x2ap_return_et              retVal = X2AP_FAILURE;

    OSCTXT                      asn1_ctx;
    x2ap_X2AP_PDU               x2ap_pdu;
    x2ap_ENBConfigurationUpdateFailure   
        *p_ENBConfigurationUpdateFailure = X2AP_P_NULL;
    x2ap_ENBConfigurationUpdateFailure_protocolIEs_element 
        *p_protocolIE_elem = X2AP_P_NULL;
    OSRTDListNode               *p_node = X2AP_P_NULL;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    
    X2AP_UT_TRACE_ENTER();


    /* Init ASN1 context */
    if (0 != rtInitContext(&asn1_ctx))
    {
        X2AP_TRACE(X2AP_ERROR, "%s:ASN context initialization failed.",
                   __FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return retVal;
    }
    do
    {
        X2AP_MEMSET (&(x2ap_pdu), 0, sizeof(x2ap_X2AP_PDU));
        /* Fill the values in the ASN structures that shall be encoded by
         *  ASN Encoder */
        /* Set Pdu type to Initiating message */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_unsuccessfulOutcome;
        
        /* X2AP Fix_AG_1 : Start */
        x2ap_pdu.u.unsuccessfulOutcome = rtxMemAllocType(&asn1_ctx,
                x2ap_UnsuccessfulOutcome);
        if (X2AP_P_NULL == x2ap_pdu.u.unsuccessfulOutcome)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        /* X2AP Fix_AG_1 : End */

        asn1Init_x2ap_UnsuccessfulOutcome(x2ap_pdu.u.unsuccessfulOutcome);

        x2ap_pdu.u.unsuccessfulOutcome->procedureCode =
                         ASN1V_x2ap_id_eNBConfigurationUpdate;
        x2ap_pdu.u.unsuccessfulOutcome->criticality = x2ap_reject;

         /* Set the initiating message type to X2 Setup */
        x2ap_pdu.u.unsuccessfulOutcome->value.t = T1x2ap__eNBConfigurationUpdate;

        p_ENBConfigurationUpdateFailure = rtxMemAllocType(&asn1_ctx,
                x2ap_ENBConfigurationUpdateFailure);
        if (X2AP_P_NULL == p_ENBConfigurationUpdateFailure)
        {
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_ENBConfigurationUpdateFailure (
                p_ENBConfigurationUpdateFailure);

        x2ap_pdu.u.unsuccessfulOutcome->value.u.eNBConfigurationUpdate =
            p_ENBConfigurationUpdateFailure;

        /*** Compose id-Cause start ***/
        rtxDListAllocNodeAndData(&asn1_ctx,
                x2ap_ENBConfigurationUpdateFailure_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if (X2AP_P_NULL == p_node)
        {
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_ENBConfigurationUpdateFailure_protocolIEs_element(
                p_protocolIE_elem);
        p_protocolIE_elem->id = ASN1V_x2ap_id_Cause;
        p_protocolIE_elem->criticality = x2ap_ignore;
        p_protocolIE_elem->value.t = T33x2ap___x2ap_ENBConfigurationUpdateFailure_IEs_1;
        p_protocolIE_elem->value.u._x2ap_ENBConfigurationUpdateFailure_IEs_1 =
            rtxMemAllocType(&asn1_ctx, x2ap_Cause);

        if (X2AP_P_NULL == p_protocolIE_elem->value.u.
                _x2ap_ENBConfigurationUpdateFailure_IEs_1)
        {
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_Cause (p_protocolIE_elem->value.u.
                _x2ap_ENBConfigurationUpdateFailure_IEs_1);

        if (X2AP_FAILURE == x2ap_compose_cause(
                p_protocolIE_elem->value.u._x2ap_ENBConfigurationUpdateFailure_IEs_1,
                &p_enb_conf_upd_fail->cause))
        {
            X2AP_TRACE(X2AP_ERROR,
                    "%s:x2ap_compose_cause returned Failure.",
                    __FUNCTION__);
            break;
        }
        rtxDListAppendNode(&p_ENBConfigurationUpdateFailure->protocolIEs, p_node);
        /*** Compose id-Cause end ***/

        /*** Compose id-TimeToWait start ***/
            /* BUG_7951 START */
        if (p_enb_conf_upd_fail->bitmask & X2AP_TIME_TO_WAIT_INDEX_PRESENT)
        {
            /* BUG_7951 STOP */
            rtxDListAllocNodeAndData(&asn1_ctx,
                    x2ap_ENBConfigurationUpdateFailure_protocolIEs_element,
                    &p_node,
                    &p_protocolIE_elem);
            if (X2AP_P_NULL == p_node)
            {
                X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
                break;
            }
            asn1Init_x2ap_ENBConfigurationUpdateFailure_protocolIEs_element (
                    p_protocolIE_elem);

            p_protocolIE_elem->id = ASN1V_x2ap_id_TimeToWait;
            p_protocolIE_elem->criticality = x2ap_ignore;
            p_protocolIE_elem->value.t = T33x2ap___x2ap_ENBConfigurationUpdateFailure_IEs_2;

            p_protocolIE_elem->value.u._x2ap_ENBConfigurationUpdateFailure_IEs_2 =
                p_enb_conf_upd_fail->time_to_wait;
            rtxDListAppendNode(
                    &p_ENBConfigurationUpdateFailure->protocolIEs,
                    p_node);
        }
        /*** Compose id-TimeToWait end ***/
        if(p_enb_conf_upd_fail->bitmask & X2AP_CRITICALITY_DIAGNOSTICS_PRESENT)
        {
			/*** Compose id-CriticalityDiagnostics start ***/
			rtxDListAllocNodeAndData(&asn1_ctx,
					x2ap_ENBConfigurationUpdateFailure_protocolIEs_element,
					&p_node,
					&p_protocolIE_elem);
			if (X2AP_P_NULL == p_node)
			{
				X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
				break;
			}
			asn1Init_x2ap_ENBConfigurationUpdateFailure_protocolIEs_element (
					p_protocolIE_elem);
			p_protocolIE_elem->id = ASN1V_x2ap_id_CriticalityDiagnostics;
			p_protocolIE_elem->criticality = x2ap_ignore;

			/* Fill the type of Protocol IE element */
			p_protocolIE_elem->value.t = T33x2ap___x2ap_ENBConfigurationUpdateFailure_IEs_3;

			p_protocolIE_elem->value.u._x2ap_ENBConfigurationUpdateFailure_IEs_3 =
				rtxMemAllocType (
						&asn1_ctx,x2ap_CriticalityDiagnostics);
            /* SPR 12964 Fix Start */
            if (PNULL == p_protocolIE_elem->value.u.
                    _x2ap_ENBConfigurationUpdateFailure_IEs_3)
            {
                X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
                break;
            }
            /* SPR 12964 Fix Stop */
            asn1Init_x2ap_CriticalityDiagnostics(
					p_protocolIE_elem->value.u._x2ap_ENBConfigurationUpdateFailure_IEs_3);
			if (X2AP_FAILURE == x2ap_compose_CriticalityDiagnostics(&asn1_ctx,
					p_protocolIE_elem->value.u._x2ap_ENBConfigurationUpdateFailure_IEs_3,
					&p_enb_conf_upd_fail->criticality_diagnostics))
			{
				X2AP_TRACE(X2AP_ERROR,
						   "%s:x2ap_compose_cause returned Failure.",
						   __FUNCTION__);
				break;
			}
			rtxDListAppendNode(&p_ENBConfigurationUpdateFailure->protocolIEs, p_node);
			/*** Compose id-CriticalityDiagnostics end ***/
        }

         /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_asn_msg,
                     X2AP_MAX_ASN1_BUF_LEN, TRUE);

        if (0 != asn1PE_x2ap_X2AP_PDU(&asn1_ctx, &x2ap_pdu))
        {
            X2AP_TRACE(X2AP_ERROR,  "ASN1 encoding of ENB CONFIGURATION FAILURE failed.");
            break;
        }
        else
        {
            *p_asn_msg_len= (U16)pe_GetMsgLen(&asn1_ctx);
            
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
            retVal = X2AP_SUCCESS;
            
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    (U32)*p_asn_msg_len);
            if (PNULL != p_event_header)
            {
                x2ap_fill_protocol_event_params(p_event_header, (U32)*p_asn_msg_len,
                        LOCAL_X2_ENB_CONFIGURATION_UPDATE_FAILURE, 
                        MESSAGE_DIRECTION_SEND);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        (U32)*p_asn_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }
        
     }while(0);
    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    return retVal;

}



/*****************************************************************************
 * Function Name  : x2ap_encode_eNB_Configuration_Update_ack 
 * Inputs         : p_enb_conf_ack - ointer to x2ap_enb_conf_ack_t
 *                  p_asn_msg - pointer to ASN encoded message
 *                  p_asn_msg_len - pointer to ASN encoded message length 
 * Outputs        : p_asn_msg - 
 *                  p_asn_msg_len - ASN encoded message length
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to encode eNB Configuration Update ACK.
 * ERROR          : This function report error to the caller function, If encoder
 *                  fails to encode message.
 ********************************************************************************/
x2ap_return_et x2ap_encode_eNB_Configuration_Update_ack (
                                    x2ap_enb_conf_ack_t        *p_enb_conf_ack,
                                    U8                          *p_asn_msg,
                                    U16                         *p_asn_msg_len)
{

    x2ap_return_et                retVal = X2AP_FAILURE;

    OSCTXT                        asn1_ctx;
    x2ap_X2AP_PDU                 x2ap_pdu;
    OSRTDListNode                 *p_node = X2AP_P_NULL;
    x2ap_ENBConfigurationUpdateAcknowledge 
               *p_ENBConfigurationUpdateAcknowledge = X2AP_P_NULL;
    x2ap_ENBConfigurationUpdateAcknowledge_protocolIEs_element 
                                 *p_protocolIE_elem = X2AP_P_NULL;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    X2AP_UT_TRACE_ENTER();

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        X2AP_TRACE(X2AP_ERROR, "%s:ASN context initialization failed.",
                   __FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return retVal;
    }
    do
    {
         X2AP_MEMSET ( &(x2ap_pdu), 0, sizeof(x2ap_X2AP_PDU) );
        /*
        ** Fill the values in the ASN structures that shall be encoded by ASN
        ** Encoder
        */
         /* Set Pdu type to Initiating message */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_successfulOutcome;

        x2ap_pdu.u.successfulOutcome =
            rtxMemAllocType(&asn1_ctx, x2ap_SuccessfulOutcome);
        if (PNULL == x2ap_pdu.u.successfulOutcome)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        asn1Init_x2ap_SuccessfulOutcome(x2ap_pdu.u.successfulOutcome);

        x2ap_pdu.u.successfulOutcome->procedureCode =
                                          ASN1V_x2ap_id_eNBConfigurationUpdate;
        x2ap_pdu.u.successfulOutcome->criticality = x2ap_reject;

        /* Set the initiating message type to X2 Setup */
        x2ap_pdu.u.successfulOutcome->value.t = T1x2ap__eNBConfigurationUpdate;
        
        p_ENBConfigurationUpdateAcknowledge = rtxMemAllocType(&asn1_ctx,
                                       x2ap_ENBConfigurationUpdateAcknowledge);
        if (X2AP_P_NULL == p_ENBConfigurationUpdateAcknowledge)
        {
             X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
             break;
        }
        asn1Init_x2ap_ENBConfigurationUpdateAcknowledge (
                                          p_ENBConfigurationUpdateAcknowledge);

        x2ap_pdu.u.successfulOutcome->value.u.eNBConfigurationUpdate =
                                           p_ENBConfigurationUpdateAcknowledge;

         /*** Compose id-CriticalityDiagnostics start ***/
        if(p_enb_conf_ack->bitmask)
        {
			rtxDListAllocNodeAndData(&asn1_ctx,
						x2ap_ENBConfigurationUpdateAcknowledge_protocolIEs_element,
						&p_node,
						&p_protocolIE_elem);
			if (X2AP_P_NULL == p_node)
			{
				X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
				break;
			}
			asn1Init_x2ap_ENBConfigurationUpdateAcknowledge_protocolIEs_element(
																p_protocolIE_elem);
			p_protocolIE_elem->id = ASN1V_x2ap_id_CriticalityDiagnostics;
			p_protocolIE_elem->criticality = x2ap_ignore;

            p_protocolIE_elem->value.u._x2ap_ENBConfigurationUpdateAcknowledge_IEs_1 = rtxMemAllocType(&asn1_ctx,
                    x2ap_CriticalityDiagnostics);
            if (X2AP_P_NULL == p_protocolIE_elem->value.u._x2ap_ENBConfigurationUpdateAcknowledge_IEs_1)
            {
                X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
                break;
            }
            asn1Init_x2ap_CriticalityDiagnostics (
                    p_protocolIE_elem->value.u._x2ap_ENBConfigurationUpdateAcknowledge_IEs_1);
            p_protocolIE_elem->value.t = T32x2ap___x2ap_ENBConfigurationUpdateAcknowledge_IEs_1;

			if ( X2AP_FAILURE == x2ap_compose_CriticalityDiagnostics(&asn1_ctx,
			 p_protocolIE_elem->value.u._x2ap_ENBConfigurationUpdateAcknowledge_IEs_1,
					  &p_enb_conf_ack->criticality_diagnostics))
			{
				X2AP_TRACE(X2AP_ERROR,
						   "%s:x2ap_compose_cause returned Failure.",
						   __FUNCTION__);
				break;
			}
			rtxDListAppendNode(
					&p_ENBConfigurationUpdateAcknowledge->protocolIEs,
					p_node);
        }
        /*** Compose id-CriticalityDiagnostics end ***/
 
        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_asn_msg,
                     X2AP_MAX_ASN1_BUF_LEN, TRUE);

        if (0 != asn1PE_x2ap_X2AP_PDU(&asn1_ctx, &x2ap_pdu))
        {
            X2AP_TRACE(X2AP_ERROR, "ASN1 encoding of Configuration Ack"
                    "failed.");
            break;
        }
        else
        {
            *p_asn_msg_len= (U16)pe_GetMsgLen(&asn1_ctx);
            
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
            retVal = X2AP_SUCCESS;
            
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    (U32)*p_asn_msg_len);
            if (PNULL != p_event_header)
            {
                x2ap_fill_protocol_event_params(p_event_header, (U32)*p_asn_msg_len,
                        LOCAL_X2_ENB_CONFIGURATION_UPDATE_ACKNOWLEDGE, 
                        MESSAGE_DIRECTION_SEND);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        (U32)*p_asn_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }
    }while(0);
    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/******************************************************************************
 * FUNCTION NAME : x2ap_compose_servedcells_to_add
 *
 * DESCRIPTION : This function used to compose ServedCells to add in eNB
 *               Configuration Update request. 
 *
 * RETURNS : X2AP_FAILURE/X2AP_SUCCESS
 *
 * ERROR : This function report error to the caller function, If encoder fails
 *         to encode message.
 ******************************************************************************/
static x2ap_return_et x2ap_compose_servedcells_to_add( 
		x2ap_gb_context_t* p_x2ap_gb_ctx,
		x2ap_enb_config_update_req_t   *p_eNB_config_upd_req,
        OSCTXT*                                            p_asn1_ctx,
        x2ap_ENBConfigurationUpdate_protocolIEs_element*   p_protocolIE_elem,
        x2ap_ENBConfigurationUpdate*                        p_x2ap_ENBConfigUpdate 
        )
{
    /* array of served cells to add*/
    OSRTDListNode*              p_node;
    /*Klockwork Fix*/
    x2ap_ServedCells* p_x2ap_ENBConfigurationUpdate_IEs_1 = X2AP_P_NULL ;
    /*Klockwork Fix*/

    if (p_eNB_config_upd_req->bitmask & X2AP_SERVED_CELLS_TO_ADD_LIST_PRESENT)
    { 
        rtxDListAllocNodeAndData(p_asn1_ctx,
                x2ap_ENBConfigurationUpdate_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);

        if (X2AP_P_NULL == p_node)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            return X2AP_FAILURE;
        }
        asn1Init_x2ap_ENBConfigurationUpdate_protocolIEs_element(
                p_protocolIE_elem);

        p_protocolIE_elem->id = ASN1V_x2ap_id_ServedCellsToAdd;
        p_protocolIE_elem->criticality = x2ap_reject;
        p_protocolIE_elem->value.t = T30x2ap___x2ap_ENBConfigurationUpdate_IEs_1;
        
        /*Klockwork Fix*/
        p_x2ap_ENBConfigurationUpdate_IEs_1 =  rtxMemAllocType(p_asn1_ctx,
                                               x2ap_ServedCells);
        if( X2AP_P_NULL == p_x2ap_ENBConfigurationUpdate_IEs_1 )
        {
           X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
           return X2AP_FAILURE;

        }
        asn1Init_x2ap_ServedCells(p_x2ap_ENBConfigurationUpdate_IEs_1);
                                          
        p_protocolIE_elem->value.u._x2ap_ENBConfigurationUpdate_IEs_1 =
            p_x2ap_ENBConfigurationUpdate_IEs_1;
        /*Klockwork Fix*/
 
        if ( X2AP_FAILURE == x2ap_compose_served_cell_new(p_x2ap_gb_ctx,
                    p_asn1_ctx,
                    p_protocolIE_elem->value.u._x2ap_ENBConfigurationUpdate_IEs_1,
                    p_eNB_config_upd_req->add_served_cell.cell_list,
                    p_eNB_config_upd_req->add_served_cell.num_served_cell))
        {
            X2AP_TRACE(X2AP_ERROR,
                    "%s:compose_served_cell returned Failure.",
                    __FUNCTION__);
            return X2AP_FAILURE;
        }
        if (p_eNB_config_upd_req->add_served_cell.num_served_cell)
        {
            rtxDListAppendNode(&p_x2ap_ENBConfigUpdate->protocolIEs, p_node);
        }
    }
    return X2AP_SUCCESS;
}
/******************************************************************************
 * FUNCTION NAME : x2ap_compose_servedcells_to_modify
 *
 * DESCRIPTION : This function used to compose ServedCells to modify in eNB
 *               Configuration Update request. 
 *
 * RETURNS : X2AP_FAILURE/X2AP_SUCCESS
 *
 * ERROR : This function report error to the caller function, If encoder fails
 *         to encode message.
******************************************************************************/
static x2ap_return_et x2ap_compose_servedcells_to_modify(
		x2ap_gb_context_t*                                 p_x2ap_gb_ctx,
		x2ap_enb_config_update_req_t*                      p_eNB_config_upd_req,
        OSCTXT*                                            p_asn1_ctx,
        x2ap_ENBConfigurationUpdate_protocolIEs_element*   p_protocolIE_elem, 
        x2ap_ENBConfigurationUpdate*                        p_x2ap_ENBConfigUpdate 
)
{

    /* array of served cells to modify */
    OSRTDListNode*                                    p_node;
    /*Klockwork Fix*/
    x2ap_ServedCellsToModify* p_x2ap_ENBConfigurationUpdate_IEs_2 = X2AP_P_NULL ; 
    /*Klockwork Fix*/
    
    if (p_eNB_config_upd_req->bitmask & X2AP_SERVED_CELLS_TO_MODIFY_LIST_PRESENT)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx,
                              x2ap_ENBConfigurationUpdate_protocolIEs_element,
                              &p_node,
                              &p_protocolIE_elem);
        if (X2AP_P_NULL == p_node)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            return X2AP_FAILURE;
        }
        asn1Init_x2ap_ENBConfigurationUpdate_protocolIEs_element (
                                                            p_protocolIE_elem);

        p_protocolIE_elem->id = ASN1V_x2ap_id_ServedCellsToModify;
        p_protocolIE_elem->criticality = x2ap_reject;
        p_protocolIE_elem->value.t = T30x2ap___x2ap_ENBConfigurationUpdate_IEs_2;

        /*Klockwork Fix*/
        p_x2ap_ENBConfigurationUpdate_IEs_2 = rtxMemAllocType(p_asn1_ctx,
                                              x2ap_ServedCellsToModify);
        if( X2AP_P_NULL == p_x2ap_ENBConfigurationUpdate_IEs_2 )
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            return X2AP_FAILURE;
        }
        asn1Init_x2ap_ServedCellsToModify(p_x2ap_ENBConfigurationUpdate_IEs_2);

        p_protocolIE_elem->value.u._x2ap_ENBConfigurationUpdate_IEs_2 =
                                            p_x2ap_ENBConfigurationUpdate_IEs_2;
        /*Klockwork Fix*/
        
        if ( X2AP_FAILURE == x2ap_compose_served_cells_to_modify(
        			p_x2ap_gb_ctx,
        			p_asn1_ctx,
                    p_protocolIE_elem->value.u._x2ap_ENBConfigurationUpdate_IEs_2,
                    p_eNB_config_upd_req->mod_served_cell.cell_list,
                    p_eNB_config_upd_req->mod_served_cell.num_served_cell))
        {
            X2AP_TRACE(X2AP_ERROR,
                       "%s:compose_served_cell returned Failure.",
                       __FUNCTION__);
            return X2AP_FAILURE;
        }
        if (p_eNB_config_upd_req->mod_served_cell.num_served_cell)
        {
        		rtxDListAppendNode(&p_x2ap_ENBConfigUpdate->protocolIEs, p_node);
        }
    }
    return X2AP_SUCCESS;
}
/******************************************************************************
 * FUNCTION NAME : x2ap_compose_servedcells_to_delete
 *
 * DESCRIPTION : This function used to compose ServedCells to delete from eNB
 *               Configuration Update request. 
 *
 * RETURNS : X2AP_FAILURE/X2AP_SUCCESS
 *
 * ERROR : This function report error to the caller function, If encoder fails
 *         to encode message.
******************************************************************************/
static x2ap_return_et x2ap_compose_servedcells_to_delete(
		x2ap_gb_context_t*                                 p_x2ap_gb_ctx,
		x2ap_enb_config_update_req_t*                      p_eNB_config_upd_req,
        OSCTXT*                                            p_asn1_ctx,
        x2ap_ENBConfigurationUpdate_protocolIEs_element*   p_protocolIE_elem,
        x2ap_ENBConfigurationUpdate*                        p_x2ap_ENBConfigUpdate 
)
{
    /* array of served cells to delete */
    OSRTDListNode*                                    p_node;
    /*Klockwork Fix*/
    x2ap_Old_ECGIs *p_x2ap_ENBConfigurationUpdate_IEs_3 = X2AP_P_NULL;
    /*Klockwork Fix*/
    if (p_eNB_config_upd_req->bitmask & X2AP_SERVED_CELLS_TO_DELETE_LIST_PRESENT)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx,
                x2ap_ENBConfigurationUpdate_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if (X2AP_P_NULL == p_node)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            return X2AP_FAILURE;
        }
        asn1Init_x2ap_ENBConfigurationUpdate_protocolIEs_element (
                p_protocolIE_elem);

        p_protocolIE_elem->id = ASN1V_x2ap_id_ServedCellsToDelete;
        p_protocolIE_elem->criticality = x2ap_reject;
        p_protocolIE_elem->value.t = T30x2ap___x2ap_ENBConfigurationUpdate_IEs_3;

        /*Klockwork Fix*/
        p_x2ap_ENBConfigurationUpdate_IEs_3 = 
                 rtxMemAllocType(p_asn1_ctx,x2ap_Old_ECGIs);

        if (X2AP_P_NULL == p_x2ap_ENBConfigurationUpdate_IEs_3 )
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            return X2AP_FAILURE;
        }
        asn1Init_x2ap_Old_ECGIs( p_x2ap_ENBConfigurationUpdate_IEs_3);

        p_protocolIE_elem->value.u._x2ap_ENBConfigurationUpdate_IEs_3 =
           p_x2ap_ENBConfigurationUpdate_IEs_3;
        /*Klockwork Fix*/

        if ( X2AP_FAILURE == x2ap_compose_old_ECGIs(
                    p_x2ap_gb_ctx,
                    p_asn1_ctx,
                    p_protocolIE_elem->value.u._x2ap_ENBConfigurationUpdate_IEs_3,
                    &(p_eNB_config_upd_req->del_served_cell)))
        {
            X2AP_TRACE(X2AP_ERROR,
                    "%s:compose_served_cell returned Failure.",
                    __FUNCTION__);
            return X2AP_FAILURE;
        }

        if (p_eNB_config_upd_req->del_served_cell.num_served_cell)
        {
            rtxDListAppendNode(&p_x2ap_ENBConfigUpdate->protocolIEs, p_node);
        }
    }
    return X2AP_SUCCESS;
}
/******************************************************************************
 * FUNCTION NAME : x2ap_compose_GUGroupIDList_to_add
 *
 * DESCRIPTION : This function used to compose GUGroupIDList to add from x2ap 
 *               group id list in eNB Configuration Update request.
 *
 * RETURNS : X2AP_FAILURE/X2AP_SUCCESS
 *
 * ERROR : This function report error to the caller function, If encoder fails
 *         to encode message.
******************************************************************************/
static x2ap_return_et x2ap_compose_GUGroupIDList_to_add(
        /*SPR 17777 +-*/
		x2ap_enb_config_update_req_t*                      p_eNB_config_upd_req,
        OSCTXT*                                            p_asn1_ctx,
        x2ap_ENBConfigurationUpdate_protocolIEs_element*   p_protocolIE_elem, 
        x2ap_ENBConfigurationUpdate*                        p_x2ap_ENBConfigUpdate 
)
{
    /* array of gu group ids to add */
    OSRTDListNode*                                    p_node;
    /*Klockwork Fix*/
    x2ap_GUGroupIDList*   p_x2ap_ENBConfigurationUpdateReqIEs_4 =
                                                        X2AP_P_NULL;
    /*Klockwork Fix*/
    if (p_eNB_config_upd_req->bitmask & X2AP_GU_GROUP_ID_TO_ADD_LIST_PRESENT)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx,
                x2ap_ENBConfigurationUpdate_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if (X2AP_P_NULL == p_node)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            return X2AP_FAILURE;
        }
        asn1Init_x2ap_ENBConfigurationUpdate_protocolIEs_element (
                p_protocolIE_elem);
        p_protocolIE_elem->id = ASN1V_x2ap_id_GUGroupIDToAddList;
        p_protocolIE_elem->criticality = x2ap_reject;
        p_protocolIE_elem->value.t = T30x2ap___x2ap_ENBConfigurationUpdate_IEs_4;

        /*Klockwork Fix*/
        p_x2ap_ENBConfigurationUpdateReqIEs_4 = rtxMemAllocType(p_asn1_ctx,
                                                       x2ap_GUGroupIDList);
        
        if( X2AP_P_NULL ==  p_x2ap_ENBConfigurationUpdateReqIEs_4 )
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            return X2AP_FAILURE;
        }  
        asn1Init_x2ap_GUGroupIDList(p_x2ap_ENBConfigurationUpdateReqIEs_4);

        p_protocolIE_elem->value.u._x2ap_ENBConfigurationUpdate_IEs_4 =
         p_x2ap_ENBConfigurationUpdateReqIEs_4;
        /*Klockwork Fix*/

        if ( X2AP_FAILURE == x2ap_compose_GUGroupIDList(p_asn1_ctx,
                    p_protocolIE_elem->value.u._x2ap_ENBConfigurationUpdate_IEs_4,
                    p_eNB_config_upd_req->add_gu_id.gu_id_list,
                    p_eNB_config_upd_req->add_gu_id.num_gu_group_id))
        {
            X2AP_TRACE(X2AP_ERROR,
                    "%s:x2ap_compose_GUGroupIDList returned Failure.",
                    __FUNCTION__);
            return X2AP_FAILURE;
        }
        if (p_eNB_config_upd_req->add_gu_id.num_gu_group_id)
        {
            rtxDListAppendNode(&p_x2ap_ENBConfigUpdate->protocolIEs, p_node);
        }
    }
    return X2AP_SUCCESS;
}
/******************************************************************************
 * FUNCTION NAME : x2ap_compose_GUGroupIDList_to_delete
 *
 * DESCRIPTION : This function used to compose GUGroupIDList to delete from x2ap 
 *               group id list in eNB Configuration Update request.
 *
 * RETURNS : X2AP_FAILURE/X2AP_SUCCESS
 *
 * ERROR : This function report error to the caller function, If encoder fails
 *         to encode message.
******************************************************************************/
static x2ap_return_et x2ap_compose_GUGroupIDList_to_delete(
        /*SPR 17777 +-*/
		x2ap_enb_config_update_req_t*                      p_eNB_config_upd_req,
        OSCTXT*                                            p_asn1_ctx,
        x2ap_ENBConfigurationUpdate_protocolIEs_element*   p_protocolIE_elem, 
        x2ap_ENBConfigurationUpdate*                        p_x2ap_ENBConfigUpdate 
)
{
    /* array of gu group ids to delete */
    OSRTDListNode*                                    p_node;
    /*Klockwork Fix*/
    x2ap_GUGroupIDList * p_x2ap_ENBConfigurationUpdate_IEs_5 = X2AP_P_NULL ;
    /*Klockwork Fix*/
    if (p_eNB_config_upd_req->bitmask & X2AP_GU_GROUP_ID_TO_DELETE_LIST_PRESENT)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx,
                x2ap_ENBConfigurationUpdate_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if (X2AP_P_NULL == p_node)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            return X2AP_FAILURE;
        }
        asn1Init_x2ap_ENBConfigurationUpdate_protocolIEs_element(
                p_protocolIE_elem);
        p_protocolIE_elem->id = ASN1V_x2ap_id_GUGroupIDToDeleteList;
        p_protocolIE_elem->criticality = x2ap_reject;
        p_protocolIE_elem->value.t = T30x2ap___x2ap_ENBConfigurationUpdate_IEs_5;
                          
        /*Klockwork Fix*/
        p_x2ap_ENBConfigurationUpdate_IEs_5 = 
                    rtxMemAllocType(p_asn1_ctx,x2ap_GUGroupIDList);
        
        if( X2AP_P_NULL == p_x2ap_ENBConfigurationUpdate_IEs_5 ) 
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            return X2AP_FAILURE;
        }
        asn1Init_x2ap_GUGroupIDList(p_x2ap_ENBConfigurationUpdate_IEs_5);
       
        p_protocolIE_elem->value.u._x2ap_ENBConfigurationUpdate_IEs_5 =
            p_x2ap_ENBConfigurationUpdate_IEs_5;
        /*Klockwork Fix*/

        if ( X2AP_FAILURE == x2ap_compose_GUGroupIDList(p_asn1_ctx,
                    p_protocolIE_elem->value.u._x2ap_ENBConfigurationUpdate_IEs_5,
                    p_eNB_config_upd_req->delete_gu_id.gu_id_list,
                    p_eNB_config_upd_req->delete_gu_id.num_gu_group_id))
        {
            X2AP_TRACE(X2AP_ERROR,
                    "%s:x2ap_compose_GUGroupIDList returned Failure.",
                    __FUNCTION__);
            return X2AP_FAILURE;
        }
        if (p_eNB_config_upd_req->delete_gu_id.num_gu_group_id)
        {
            rtxDListAppendNode(&p_x2ap_ENBConfigUpdate->protocolIEs, p_node);
        }
    }
    return X2AP_SUCCESS;
}

/*****************************************************************************
 * Function Name  : x2ap_encode_eNB_Configuration_Update_req 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_eNB_config_upd_req - pointer to x2ap_enb_config_update_req_t
 *                  p_asn_msg - pointer to encoded ASN msg 
 *                  p_asn_msg_len - pointer to encoded ASN msg length 
 * Outputs        : p_asn_msg - encoded ASN msg
 *                  p_asn_msg_len - encoded ASN msg length 
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to encode eNB Configuration Update request.
 * ERROR          : This function report error to the caller function, If encoder
 *                  fails to encode message. 
 ********************************************************************************/
x2ap_return_et x2ap_encode_eNB_Configuration_Update_req(
		x2ap_gb_context_t* p_x2ap_gb_ctx,
		x2ap_enb_config_update_req_t   *p_eNB_config_upd_req,
                                U8                     *p_asn_msg,
                                U16                    *p_asn_msg_len)
{

    x2ap_return_et              retVal = X2AP_FAILURE;

    OSCTXT                      asn1_ctx;
    x2ap_X2AP_PDU               x2ap_pdu;

    /* ASN generated ENBConfigurationUpdate structure*/
    x2ap_ENBConfigurationUpdate *p_x2ap_ENBConfigUpdate = X2AP_P_NULL;
    /* 
    ** ASN generated structure having ENBConfigurationUpdate information objects  
    ** and their selector.
    */
    x2ap_ENBConfigurationUpdate_protocolIEs_element *p_protocolIE_elem =
                                                                X2AP_P_NULL;

    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    X2AP_UT_TRACE_ENTER();
    
     /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        X2AP_TRACE(X2AP_ERROR,"%s:ASN context initialization failed.",
                   __FUNCTION__);
        return retVal;
    }
    
     do
    {
         X2AP_MEMSET( &(x2ap_pdu), 0, sizeof(x2ap_X2AP_PDU) );
        /*
        ** Fill the values in the ASN structures that shall be encoded by
        ** ASN Encoder
        */
        /* Set Pdu type to Initiating message */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_initiatingMessage;

        /* X2AP Fix_AG_1 : Start */
        x2ap_pdu.u.initiatingMessage = rtxMemAllocType(&asn1_ctx,
                x2ap_InitiatingMessage);
        if (X2AP_P_NULL == x2ap_pdu.u.initiatingMessage)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        /* X2AP Fix_AG_1 : End */

        asn1Init_x2ap_InitiatingMessage(x2ap_pdu.u.initiatingMessage);
        /* fill procedure code for eNBConfigurationUpdate */
        x2ap_pdu.u.initiatingMessage->procedureCode =
                                    ASN1V_x2ap_id_eNBConfigurationUpdate;
        /* fill criticality of message type */
        x2ap_pdu.u.initiatingMessage->criticality = x2ap_reject;

        /* Set the initiating message type to eNB Configuration Update */
        x2ap_pdu.u.initiatingMessage->value.t = T1x2ap__eNBConfigurationUpdate;

        p_x2ap_ENBConfigUpdate  = rtxMemAllocType(&asn1_ctx,
                                                x2ap_ENBConfigurationUpdate);
        if (X2AP_P_NULL == p_x2ap_ENBConfigUpdate)
        {
             X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
             break;
        }
         asn1Init_x2ap_ENBConfigurationUpdate(p_x2ap_ENBConfigUpdate);

        x2ap_pdu.u.initiatingMessage->value.u.eNBConfigurationUpdate =
                                                      p_x2ap_ENBConfigUpdate;

        /*** Compose id-ServedCells to add start ***/
        if(X2AP_SUCCESS != x2ap_compose_servedcells_to_add(p_x2ap_gb_ctx,
                              p_eNB_config_upd_req,&asn1_ctx,
                              p_protocolIE_elem,p_x2ap_ENBConfigUpdate))
        {
            break;
        }
        /*** Compose id-ServedCells to add end ***/

        /*** Compose id-ServedCells to modify start ***/
        if(X2AP_SUCCESS != x2ap_compose_servedcells_to_modify(p_x2ap_gb_ctx,
                              p_eNB_config_upd_req,&asn1_ctx,
                              p_protocolIE_elem,p_x2ap_ENBConfigUpdate))
        {
            break;
        }
        /*** Compose id-ServedCells to add end ***/

         /*** Compose id-ServedCells to delete start ***/
        if(X2AP_SUCCESS != x2ap_compose_servedcells_to_delete(p_x2ap_gb_ctx,
                              p_eNB_config_upd_req,&asn1_ctx,
                              p_protocolIE_elem,p_x2ap_ENBConfigUpdate))
        {
            break;
        }
        /*** Compose id-ServedCells to delete end ***/

         /*** Compose id-GUGroupIDList to add start ***/
        if(X2AP_SUCCESS != x2ap_compose_GUGroupIDList_to_add(/*SPR 17777 +-*/
                              p_eNB_config_upd_req,&asn1_ctx,
                              p_protocolIE_elem,p_x2ap_ENBConfigUpdate))
        {
            break;
        }
        /* Compose id-GUGroupIDList to add end */

        /*** Compose id-GUGroupIDList to delete start ***/
        if(X2AP_SUCCESS != x2ap_compose_GUGroupIDList_to_delete(/*SPR 17777 +-*/
                              p_eNB_config_upd_req,&asn1_ctx,
                              p_protocolIE_elem,p_x2ap_ENBConfigUpdate))
        {
            break;
        }
        /* Compose id-GUGroupIDList to delete end */

        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_asn_msg,
                     X2AP_MAX_ASN1_BUF_LEN, TRUE);

        if (0 != asn1PE_x2ap_X2AP_PDU(&asn1_ctx, &x2ap_pdu))
        {
            rtxErrPrint(&asn1_ctx);
            //*p_asn_msg_len= (U16)pe_GetMsgLen(&asn1_ctx);
            //rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU ", &x2ap_pdu,
            //        "ENBConfigurationUpdate", p_asn_msg, *p_asn_msg_len);
            X2AP_TRACE(X2AP_ERROR, "ASN1 encoding of eNB Configuration Update "
            "Request failed.");
            break;
        }
        else
        {
            *p_asn_msg_len= (U16)pe_GetMsgLen(&asn1_ctx);
            retVal = X2AP_SUCCESS;
            
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
            
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    (U32)*p_asn_msg_len);
            if (PNULL != p_event_header)
            {
                x2ap_fill_protocol_event_params(p_event_header, (U32)*p_asn_msg_len,
                        LOCAL_X2_ENB_CONFIGURATION_UPDATE, 
                        MESSAGE_DIRECTION_SEND);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        (U32)*p_asn_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

    }while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    X2AP_UT_TRACE_EXIT();
    return retVal;
}
/******************************************************************************
 * Function Name  : x2ap_compose_old_ECGIs
 * Inputs         : p_x2ap_gb_ctx - pointer to X2SP global context 
 *                  p_asn1_ctx - Pointer to OSCTXT.
 *                  p_x2ap_IE_serv_cell_to_del - poimter to OSRTDList
 *                  p_serv_cell_to_del - pointer to x2ap_served_cells_to_delete_list_t
 * Outputs        : p_x2ap_IE_serv_cell - This structure is populated with
 *                                        information from p_serv_cell.
 * Returns        : X2AP_SUCCESS - Asn Encoding was successful.
 *                  X2AP_FAILURE - Asn Encoding was not successful.
 * Description    : This function populates p_x2ap_IE_serv_cell with
 *                  information necessary to encode served cell Info.
******************************************************************************/
x2ap_return_et x2ap_compose_old_ECGIs(
		x2ap_gb_context_t* p_x2ap_gb_ctx,
		OSCTXT                         *p_asn1_ctx,
        OSRTDList                      *p_x2ap_IE_serv_cell_to_del,
        x2ap_served_cells_to_delete_list_t      *p_serv_cell_to_del)
{
    OSRTDListNode                      *p_node = X2AP_P_NULL;
    x2ap_ECGI                          *p_old_ECGI = X2AP_P_NULL;
    U16                                deleted_cells_count = 0;
    x2ap_ecgi_t old_ecgi;
     X2AP_UT_TRACE_ENTER();

    if (!(p_serv_cell_to_del->num_served_cell))
    {
        X2AP_TRACE(X2AP_INFO,  "%s:Num of supported served cell.\n",
                   __FUNCTION__);
        return X2AP_SUCCESS;
    }

    for (deleted_cells_count = 0;
            deleted_cells_count < p_serv_cell_to_del->num_served_cell;
            deleted_cells_count++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx,
                                 x2ap_ECGI,
                                 &p_node,
                                 &p_old_ECGI);
         if (X2AP_P_NULL == p_node)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }
        
        asn1Init_x2ap_ECGI(p_old_ECGI);
        x2ap_compose_ecgi_from_cell_id(p_x2ap_gb_ctx->global_enb_id,
        		p_serv_cell_to_del->cell_id_list[deleted_cells_count], &old_ecgi);

        p_old_ECGI->pLMN_Identity.numocts = MAX_PLMN_ID_BYTES;
        X2AP_MEMCPY(p_old_ECGI->pLMN_Identity.data,
                        old_ecgi.plmn_identity.plmn_id,
                        MAX_PLMN_ID_BYTES);

        p_old_ECGI->eUTRANcellIdentifier.numbits =
                        X2AP_EUTRAN_CELL_ID_BIT_STR_LEN;
        X2AP_MEMCPY(p_old_ECGI->eUTRANcellIdentifier.data,
                       old_ecgi.eutran_cell_id,
                        sizeof(U32));
         rtxDListAppendNode (p_x2ap_IE_serv_cell_to_del, p_node);
    }
    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS; 
}

/******************************************************************************
 * Function Name  : x2ap_compose_served_cells_to_modify
 * Inputs         : x2ap_gb_context_t - pointer to X2AP global context 
 *                  p_asn1_ctx - Pointer to OSCTXT.
 *                  p_x2ap_IE_serv_cell_to_mod - pointer to OSRTDList
 *                  p_serv_cell_to_mod - pointer to 
 *                                             x2ap_served_cells_to_modify_info_t
 *                  num_served_cell_to_mod - num of cell to modify
 * Outputs        : None
 * Returns        : X2AP_SUCCESS - Asn Encoding was successful.
 *                  X2AP_FAILURE - Asn Encoding was not successful.
 * Description    : This function populates p_x2ap_IE_serv_cell with
 *                  information necessary to encode served cell Info.
 ******************************************************************************/
x2ap_return_et x2ap_compose_served_cells_to_modify(
		x2ap_gb_context_t* p_x2ap_gb_ctx,
		OSCTXT                         *p_asn1_ctx,
        OSRTDList                      *p_x2ap_IE_serv_cell_to_mod,
        x2ap_served_cells_to_modify_info_t *p_serv_cell_to_mod,
        U8                              num_served_cell_to_mod)
{
    OSRTDListNode                      *p_node = X2AP_P_NULL;
    OSRTDListNode                      *p_node_serv_cell = X2AP_P_NULL;
    x2ap_ServedCellsToModify_Item      *p_srvdCellsToModify_Item = X2AP_P_NULL;
    OSRTDList                          *p_neighbour_cell_info = X2AP_P_NULL;
    x2ap_Neighbour_Information_element *p_neigh_cell_info = X2AP_P_NULL;
    U16                                 modified_cells_count = 0;
    U16                                 neighbour_cell_count = 0;
    x2ap_ecgi_t old_ecgi;
    X2AP_UT_TRACE_ENTER();

    if (!num_served_cell_to_mod)
    {
        X2AP_TRACE(X2AP_INFO,  "%s:Number of served cell to modify is 0.\n",
                   __FUNCTION__);
        return X2AP_SUCCESS;
    }

    for (modified_cells_count = 0; 
            modified_cells_count < num_served_cell_to_mod;
            modified_cells_count++)
    {
         rtxDListAllocNodeAndData(p_asn1_ctx,
                                 x2ap_ServedCellsToModify_Item,
                                 &p_node_serv_cell,
                                 &p_srvdCellsToModify_Item);
        if (X2AP_P_NULL == p_node_serv_cell)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        } 
        asn1Init_x2ap_ServedCellsToModify_Item(p_srvdCellsToModify_Item);

        if (p_serv_cell_to_mod[modified_cells_count].served_cell_info.nbour_cell_info.num_neighbour_cell)
        {
            p_srvdCellsToModify_Item->m.neighbour_InfoPresent = 1;
            p_neighbour_cell_info = &p_srvdCellsToModify_Item->neighbour_Info;
             rtxDListInit(p_neighbour_cell_info);
        }

        /*SPR 21554 Start*/
        x2ap_populate_ecgi_from_cell_id(p_x2ap_gb_ctx,
        		p_serv_cell_to_mod[modified_cells_count].served_cell_info.cell_info.cell_id, &old_ecgi);
        /*SPR 21554 End*/

        /* Compose old Ecgi start */
        p_srvdCellsToModify_Item->old_ecgi.pLMN_Identity.numocts = 
                                                      MAX_PLMN_ID_BYTES;
        X2AP_MEMCPY(p_srvdCellsToModify_Item->old_ecgi.pLMN_Identity.data,
                        old_ecgi.plmn_identity.plmn_id,
                        MAX_PLMN_ID_BYTES);

        p_srvdCellsToModify_Item->old_ecgi.eUTRANcellIdentifier.numbits =
                        X2AP_EUTRAN_CELL_ID_BIT_STR_LEN;
        X2AP_MEMCPY(
                p_srvdCellsToModify_Item->old_ecgi.eUTRANcellIdentifier.data,
                &(old_ecgi.eutran_cell_id),
                sizeof(U32));
        /* Compose old Ecgi end */

        /* Compose served cell info start */
        if (X2AP_FAILURE == x2ap_compose_serv_cell_info_new(
        			p_x2ap_gb_ctx,
        			p_asn1_ctx,
                    &p_srvdCellsToModify_Item->servedCellInfo,
                    &(p_serv_cell_to_mod[modified_cells_count].served_cell_info.cell_info)))
        {
            X2AP_TRACE(X2AP_ERROR,
                       "%s:compose_serv_cell_info returned Failure.",
                       __FUNCTION__);
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }

        /*Compose Cell deactivation*/
        if(p_serv_cell_to_mod->bitmask & X2AP_DEACTIVATION_IND_PRESENT)
        {
        	x2ap_ServedCellsToModify_Item_iE_Extensions_element *p_extn_elem = X2AP_P_NULL;

        	p_srvdCellsToModify_Item->m.iE_ExtensionsPresent = 1;

        	asn1Init_x2ap_ServedCellsToModify_Item_iE_Extensions(&p_srvdCellsToModify_Item->iE_Extensions);

        	rtxDListAllocNodeAndData(p_asn1_ctx,
			x2ap_ServedCellsToModify_Item_iE_Extensions_element,
        	        &p_node,
        	        &p_extn_elem);

        	 if (X2AP_P_NULL == p_node)
        	 {
        		 return X2AP_FAILURE;
        	 }

        	 asn1Init_x2ap_ServedCellsToModify_Item_iE_Extensions_element(p_extn_elem);
        	 p_extn_elem->id = ASN1V_x2ap_id_DeactivationIndication;
        	 p_extn_elem->criticality = x2ap_ignore;
             p_extn_elem->extensionValue.t  =  T31x2ap___x2ap_ServedCellsToModify_Item_ExtIEs_1;
        	 p_extn_elem->extensionValue.u._x2ap_ServedCellsToModify_Item_ExtIEs_1 = p_serv_cell_to_mod[modified_cells_count].deactivation_ind;;

        	if (0 == rtxDListAppend(p_asn1_ctx, &p_srvdCellsToModify_Item->iE_Extensions, (void*)p_extn_elem))
        	{
        		 return (x2ap_return_et)LOG_RTERR(p_asn1_ctx, RTERR_NOMEM);
        	}
        }

        /* Compose served cell end */

         /* compose neghbour cell info start */
        for (neighbour_cell_count = 0;
        		neighbour_cell_count<p_serv_cell_to_mod[modified_cells_count].served_cell_info.nbour_cell_info.num_neighbour_cell;
                neighbour_cell_count++)
        {
            rtxDListAllocNodeAndData(p_asn1_ctx,
                                     x2ap_Neighbour_Information_element,
                                     &p_node,
                                     &p_neigh_cell_info);
            if (X2AP_P_NULL == p_node)
            {
                X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
                X2AP_UT_TRACE_EXIT();
                return X2AP_FAILURE;
            }

            asn1Init_x2ap_Neighbour_Information_element(p_neigh_cell_info);

            /* Encode ECGI */
            /* fill pLMN_Identity numocts */

            p_neigh_cell_info->eCGI.pLMN_Identity.numocts = MAX_PLMN_ID_BYTES;

            /* fill pLMN_Identity data */
            X2AP_MEMCPY(p_neigh_cell_info->eCGI.pLMN_Identity.data,
                    p_serv_cell_to_mod[modified_cells_count].served_cell_info.nbour_cell_info.neighbour_info_arr[neighbour_cell_count].ecgi.plmn_identity.plmn_id,
                    MAX_PLMN_ID_BYTES);

            /* fill eUTRANcellIdentifier numbits */
            p_neigh_cell_info->eCGI.eUTRANcellIdentifier.numbits =
                                               X2AP_EUTRAN_CELL_ID_BIT_STR_LEN;
            /* fill eUTRANcellIdentifier data */
            X2AP_MEMCPY(p_neigh_cell_info->eCGI.eUTRANcellIdentifier.data,
            		p_serv_cell_to_mod[modified_cells_count].served_cell_info.nbour_cell_info.neighbour_info_arr[neighbour_cell_count].ecgi.eutran_cell_id,
                    sizeof(U32));

             /* Encode PCI */
            p_neigh_cell_info->pCI = p_serv_cell_to_mod[modified_cells_count].served_cell_info.nbour_cell_info.neighbour_info_arr[neighbour_cell_count].pci;

            /* Encode EARFCN */
            p_neigh_cell_info->eARFCN =p_serv_cell_to_mod[modified_cells_count].served_cell_info.nbour_cell_info.neighbour_info_arr[neighbour_cell_count].earfcn.earfcn;

            rtxDListAppendNode (p_neighbour_cell_info, p_node);
        }
        /*** compose neghbour cell info end ***/
         rtxDListAppendNode(p_x2ap_IE_serv_cell_to_mod, p_node_serv_cell);
    }
    
    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;
}

/*****************************************************************************
 * Function Name  : x2ap_encode_x2setup_req 
 * Inputs         : p_x2_setup_req - pointer to x2_setup_req_t
 *                  p_asn_msg - pointer to ASN encoded msg
 *                  p_asn_msg_len - pointer to ASN encoded msg length 
 * Outputs        : p_asn_msg - ASN encoded msg
 *                  p_asn_msg_len - ASN encoded msg length 
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to encode X2 Setup request.
 * ERROR          : This function report error to the caller function, If encoder
 *                  fails to encode message.
 ********************************************************************************/
x2ap_return_et x2ap_encode_x2setup_req (x2_setup_req_t   *p_x2_setup_req, 
                                        U8                *p_asn_msg,
                                        U16               *p_asn_msg_len)
{
    x2ap_return_et              retVal = X2AP_FAILURE;

    OSCTXT                      asn1_ctx;
    x2ap_X2AP_PDU               x2ap_pdu;
    OSRTDListNode               *p_node = PNULL;
    x2ap_X2SetupRequest_protocolIEs_element *     p_protocolIE_elem = PNULL;
    x2ap_X2SetupRequest         *p_x2ap_X2SetupRequest = X2AP_P_NULL;
    OSRTDList  x2SetupRequest_IEs_2;
    OSRTDList  x2SetupRequest_IEs_3;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    X2AP_UT_TRACE_ENTER();

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        X2AP_TRACE(X2AP_ERROR, "%s:ASN context initialization failed.",
                   __FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return retVal;
    }
    do
    {
        X2AP_MEMSET ( &(x2ap_pdu), 0, sizeof(x2ap_X2AP_PDU) );
        /* Fill the values in the ASN structures that shall be encoded by
         *  ASN Encoder */
        /* Set Pdu type to Initiating message */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_initiatingMessage; 

        x2ap_pdu.u.initiatingMessage = rtxMemAllocType(&asn1_ctx,
                x2ap_InitiatingMessage);
        if (X2AP_P_NULL == x2ap_pdu.u.initiatingMessage)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_InitiatingMessage(x2ap_pdu.u.initiatingMessage);
   
        x2ap_pdu.u.initiatingMessage->procedureCode = ASN1V_x2ap_id_x2Setup;
        x2ap_pdu.u.initiatingMessage->criticality = x2ap_reject;
        
        /* Set the initiating message type to X2 Setup */ 
        x2ap_pdu.u.initiatingMessage->value.t = T1x2ap__x2Setup;

        p_x2ap_X2SetupRequest = rtxMemAllocType(&asn1_ctx,
                                                x2ap_X2SetupRequest);  
        if (X2AP_P_NULL == p_x2ap_X2SetupRequest)
        {
             X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
             break;
        }
        else
        {
            X2AP_TRACE(X2AP_DETAILED,"Mem Alloc successful to x2ap_X2SetupRequest");
        }

        asn1Init_x2ap_X2SetupRequest (p_x2ap_X2SetupRequest);
        x2ap_pdu.u.initiatingMessage->value.u.x2Setup = p_x2ap_X2SetupRequest;


        /*** Compose id-Global-ENB-ID start ***/
        rtxDListAllocNodeAndData(&asn1_ctx,
                x2ap_X2SetupRequest_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if (X2AP_P_NULL == p_node)
        {
            /* Not enough memory */
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_X2SetupRequest_protocolIEs_element(
                p_protocolIE_elem);

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_x2ap_id_GlobalENB_ID;
        p_protocolIE_elem->criticality = x2ap_reject;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T24x2ap___x2ap_X2SetupRequest_IEs_1;

        p_protocolIE_elem->value.u._x2ap_X2SetupRequest_IEs_1 =
            rtxMemAllocType(&asn1_ctx,x2ap_GlobalENB_ID);
        if (X2AP_P_NULL == p_protocolIE_elem->value.u._x2ap_X2SetupRequest_IEs_1)
        {
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_GlobalENB_ID (p_protocolIE_elem->value.u.
                _x2ap_X2SetupRequest_IEs_1);

        if (X2AP_FAILURE == x2ap_compose_global_enb_id(&asn1_ctx,
                              p_protocolIE_elem->value.u._x2ap_X2SetupRequest_IEs_1,
                              &p_x2_setup_req->global_enb_id))
        {
            X2AP_TRACE(X2AP_ERROR,
                       "%s:compose id-Global-ENB-ID returned Failure.",
                       __FUNCTION__);
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }
        else
        {
            X2AP_TRACE(X2AP_DETAILED,
                       "compose id-Global-ENB-ID returned Success.");
        }
        rtxDListAppendNode(&p_x2ap_X2SetupRequest->protocolIEs, p_node);

        /*** Compose id-Global-ENB-ID end ***/
        ///////////////////////////////////////////////////////////
        /*** Compose id-ServedCells start ***/
        rtxDListAllocNodeAndData(&asn1_ctx,                                    
                x2ap_X2SetupRequest_protocolIEs_element,                       
                &p_node,                                                       
                &p_protocolIE_elem);        
        if (X2AP_P_NULL == p_node)
        {                                                                      
            /* Not enough memory */                                            
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",                    
                    __FUNCTION__);                                           
            break;                                                             
        }                                                                      
        asn1Init_x2ap_X2SetupRequest_protocolIEs_element(
                p_protocolIE_elem);

        /* Fill protocol IE Id and criticality */                              
        p_protocolIE_elem->id = ASN1V_x2ap_id_ServedCells;                     
        p_protocolIE_elem->criticality = x2ap_reject;  

        /* Fill the type of Protocol IE element */                             
        p_protocolIE_elem->value.t = T24x2ap___x2ap_X2SetupRequest_IEs_2;  

        rtxDListInit(&x2SetupRequest_IEs_2);                                   
        p_protocolIE_elem->value.u._x2ap_X2SetupRequest_IEs_2  =                    
            &x2SetupRequest_IEs_2;

        if (X2AP_FAILURE == x2ap_compose_served_cell(&asn1_ctx,
                              p_protocolIE_elem->value.u._x2ap_X2SetupRequest_IEs_2,
                              p_x2_setup_req->served_cell_arr,
                              p_x2_setup_req->num_served_cell) )
        {
            X2AP_TRACE(X2AP_ERROR,
                       "%s:compose_served_cell returned Failure.",
                       __FUNCTION__);
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }
        else
        {

            X2AP_TRACE(X2AP_DETAILED,
                       "compose_served_cell returned Success.");
        }
        rtxDListAppendNode(&p_x2ap_X2SetupRequest->protocolIEs, p_node);
        /*** Compose id-ServedCells end ***/
        //////////////////////////////////////////

        /*** Compose id-GUGroupIDList start ***/
        if(p_x2_setup_req->num_gu_group_id)
        {
            rtxDListAllocNodeAndData(&asn1_ctx,                                
                    x2ap_X2SetupRequest_protocolIEs_element,                   
                    &p_node,                                                   
                    &p_protocolIE_elem);                                       
            if (X2AP_P_NULL == p_node)
            {                                                                  
                /* Not enough memory */                                        
                X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",                
                        __FUNCTION__);                                         
                break;                                                         
            }                                                                  
            asn1Init_x2ap_X2SetupRequest_protocolIEs_element (
                    p_protocolIE_elem);

            /* Fill protocol IE Id and criticality */                          
            p_protocolIE_elem->id = ASN1V_x2ap_id_GUGroupIDList;               
            p_protocolIE_elem->criticality = x2ap_reject;  

            /* Fill the type of Protocol IE element */                         
            p_protocolIE_elem->value.t = T24x2ap___x2ap_X2SetupRequest_IEs_3;  

            rtxDListInit(&x2SetupRequest_IEs_3);                               
            p_protocolIE_elem->value.u._x2ap_X2SetupRequest_IEs_3  =                
                &x2SetupRequest_IEs_3;

            if ( X2AP_FAILURE == x2ap_compose_GUGroupIDList(&asn1_ctx,
                        p_protocolIE_elem->value.u._x2ap_X2SetupRequest_IEs_3,
                        p_x2_setup_req->group_id_arr,
                        p_x2_setup_req->num_gu_group_id))
            {
                X2AP_TRACE(X2AP_ERROR,
                        "%s:x2ap_compose_GUGroupIDList returned Failure.\n",
                        __FUNCTION__);
                /*SPR_18125_START*/
                rtFreeContext(&asn1_ctx);
                /*SPR_18125_END*/
                X2AP_UT_TRACE_EXIT();
                return X2AP_FAILURE;
            }
            else
            {

                X2AP_TRACE(X2AP_DETAILED,
                        "x2ap_compose_GUGroupIDList returned Success.");
            }
            rtxDListAppendNode(&p_x2ap_X2SetupRequest->protocolIEs, p_node);
            /*** Compose id-GUGroupIDList end ***/ 
        }
        //////////////////////////////////////////


        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_asn_msg,
                     X2AP_MAX_ASN1_BUF_LEN, TRUE);

        if (0 != asn1PE_x2ap_X2AP_PDU(&asn1_ctx, &x2ap_pdu))
        {
            X2AP_TRACE(X2AP_ERROR, "ASN1 encoding of X2 Setup Request "
                    "failed.");
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }
        else
        {
            *p_asn_msg_len= (U16)pe_GetMsgLen(&asn1_ctx);
            retVal = X2AP_SUCCESS;
            
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
            
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    (U32)*p_asn_msg_len);
            if (PNULL != p_event_header)
            {
                x2ap_fill_protocol_event_params(p_event_header, (U32)*p_asn_msg_len,
                        LOCAL_X2_SETUP_REQUEST, 
                        MESSAGE_DIRECTION_SEND);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        (U32)*p_asn_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

    }while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    X2AP_UT_TRACE_EXIT();
    return retVal;
}


/******************************************************************************
 * Function Name  : x2ap_compose_GUGroupIDList
 * Inputs         : p_asn1_ctx - Pointer to OSCTXT.
 *                  p_x2ap_IE_gu_grp_id_list - pointer to OSRTDList
 *                  p_gu_group_id - This contains the information to
 *                                  global unique group Id list.
 *                  num_gu_group_id - gu group id
 * Outputs        : p_x2ap_IE_gu_grp_id_list - This structure is populated with
 *                                             information from p_gu_group_id.
 * Returns        : X2AP_SUCCESS /X2AP_FAILURE 
 * Description    : This function populates p_x2ap_IE_gu_grp_id_list with
 *                  information necessary to encode global unique group Id list.
 ******************************************************************************/
x2ap_return_et x2ap_compose_GUGroupIDList(
                             OSCTXT              *p_asn1_ctx,
                             OSRTDList           *p_x2ap_IE_gu_grp_id_list,
                             x2ap_gu_group_id_t *p_gu_group_id,
                             U8                   num_gu_group_id)
{
    OSRTDListNode               *p_node = X2AP_P_NULL;
    x2ap_GU_Group_ID            *p_GU_Group_ID = X2AP_P_NULL;
    U16                          group_id_list_count = 0;

    X2AP_UT_TRACE_ENTER();

    for (group_id_list_count = 0; group_id_list_count < num_gu_group_id;
                                                    group_id_list_count++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx,
                                 x2ap_GU_Group_ID,
                                 &p_node,
                                 &p_GU_Group_ID);
        if (X2AP_P_NULL == p_node)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }
        asn1Init_x2ap_GU_Group_ID(p_GU_Group_ID);

        /* Encode PLMN ID */
        p_GU_Group_ID->pLMN_Identity.numocts = MAX_PLMN_ID_BYTES; 
        X2AP_MEMCPY(p_GU_Group_ID->pLMN_Identity.data,
                    p_gu_group_id[group_id_list_count].plmn_id.plmn_id,
                    MAX_PLMN_ID_BYTES);

        /* Encode MME Group ID */
        p_GU_Group_ID->mME_Group_ID.numocts = MAX_MME_GRP_BYTES;
        X2AP_MEMCPY(p_GU_Group_ID->mME_Group_ID.data,
                    p_gu_group_id[group_id_list_count].mme_group_id,
                    MAX_MME_GRP_BYTES); 
        rtxDListAppendNode(p_x2ap_IE_gu_grp_id_list, p_node);
    }

    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;
}

/******************************************************************************
 * Function Name  : x2ap_compose_served_cell
 * Inputs         : p_asn1_ctx - Pointer to OSCTXT.
 *                  p_x2ap_IE_serv_cell - Pointer to OSRTDList
 *                  p_serv_cell - This contains the information of served cells
 *                  num_served_cell - num of served cell
 * Outputs        : p_x2ap_IE_serv_cell - This structure is populated with
 *                                        information from p_serv_cell.
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function populates p_x2ap_IE_serv_cell with
 *                  information necessary to encode served cell Info.
 *****************************************************************************/
x2ap_return_et x2ap_compose_served_cell(
                             OSCTXT                      *p_asn1_ctx,
                             OSRTDList                   *p_x2ap_IE_serv_cell,
                             x2ap_srvd_cell_arry_t      *p_serv_cell,
                             U16                          num_served_cell)
{

    OSRTDListNode               *p_node = X2AP_P_NULL;
    OSRTDListNode               *p_node_serv_cell = NULL;
    OSRTDList                   *p_neighbour_cell_info = X2AP_P_NULL;
    x2ap_ServedCells_element    *p_servedCells_elem = X2AP_P_NULL;
    x2ap_Neighbour_Information_element *p_neigh_cell_info = X2AP_P_NULL;
    U16                         cell_count = 0;
    U16                         neighbour_cell_count = 0;
    
    X2AP_UT_TRACE_ENTER();

    if (!num_served_cell)
    {
        X2AP_TRACE(X2AP_ERROR, "%s:Num of supported served cell.",
                   __FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }
    for (cell_count = 0; cell_count < num_served_cell; cell_count++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx,
                                 x2ap_ServedCells_element,
                                 &p_node_serv_cell,
                                 &p_servedCells_elem);
        if (X2AP_P_NULL == p_node_serv_cell)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }
        asn1Init_x2ap_ServedCells_element(p_servedCells_elem);

        if (p_serv_cell->nbour_cell_info.num_neighbour_cell)
        {
            p_servedCells_elem->m.neighbour_InfoPresent = 1;
            p_neighbour_cell_info = &p_servedCells_elem->neighbour_Info;
            rtxDListInit(p_neighbour_cell_info);
        }
        /*** compose served cell Info start ***/
        if (X2AP_FAILURE == x2ap_compose_serv_cell_info(p_asn1_ctx,
                                &p_servedCells_elem->servedCellInfo,
                                                        p_serv_cell))
        {
            X2AP_TRACE(X2AP_ERROR,
                       "%s:compose_serv_cell_info returned Failure.",
                       __FUNCTION__);
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        } 
        /*** compose served cell info end ***/

        /*** compose neghbour cell info start ***/
        for (neighbour_cell_count = 0; 
                  neighbour_cell_count < 
                           p_serv_cell->nbour_cell_info.num_neighbour_cell; 
                                                       neighbour_cell_count++)
        {
            rtxDListAllocNodeAndData(p_asn1_ctx,
                                     x2ap_Neighbour_Information_element,
                                     &p_node,
                                     &p_neigh_cell_info);
            if (X2AP_P_NULL == p_node)
            {
                X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
                X2AP_UT_TRACE_EXIT();
                return X2AP_FAILURE;
            }            
            asn1Init_x2ap_Neighbour_Information_element(p_neigh_cell_info); 
            /* Encode ECGI */
            p_neigh_cell_info->eCGI.pLMN_Identity.numocts = MAX_PLMN_ID_BYTES;
            X2AP_MEMCPY(p_neigh_cell_info->eCGI.pLMN_Identity.data,
                        p_serv_cell->nbour_cell_info.neighbour_info_arr[
                         neighbour_cell_count].ecgi.plmn_identity.plmn_id,
                        MAX_PLMN_ID_BYTES); 
           
            p_neigh_cell_info->eCGI.eUTRANcellIdentifier.numbits =
                                             X2AP_EUTRAN_CELL_ID_BIT_STR_LEN;
            X2AP_MEMCPY(p_neigh_cell_info->eCGI.eUTRANcellIdentifier.data,
                        &(p_serv_cell->nbour_cell_info.neighbour_info_arr[
                         neighbour_cell_count].ecgi.eutran_cell_id),
                        sizeof(U32));

            /* Encode PCI */
            p_neigh_cell_info->pCI = p_serv_cell->nbour_cell_info.
                         neighbour_info_arr[neighbour_cell_count].pci;
            /* Encode ERFCN */
            p_neigh_cell_info->eARFCN = (x2ap_EARFCN)(p_serv_cell->
                    nbour_cell_info.neighbour_info_arr[neighbour_cell_count].
                    earfcn.earfcn);
 
            rtxDListAppendNode (p_neighbour_cell_info, p_node);        
        }
        /*** compose neghbour cell info end ***/
        rtxDListAppendNode(p_x2ap_IE_serv_cell, p_node_serv_cell);
        p_serv_cell = (x2ap_srvd_cell_arry_t *)((p_serv_cell) + 1);
    }
    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;
}

/******************************************************************************
 * Function Name  : x2ap_compose_serv_cell_info
 * Inputs         : p_asn1_ctx - Pointer to OSCTXT.
 *                  p_x2ap_IE_serv_cell - pointer to x2ap_ServedCell_Information
 *                  p_serv_cell - This contains the information of served cell.
 *                                
 * Outputs        : p_x2ap_IE_serv_cell - This structure is populated with
 *                                        information from p_serv_cell.
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function populates p_x2ap_IE_serv_cell with
 *                  information necessary to encode served cell info.
 ******************************************************************************/
x2ap_return_et x2ap_compose_serv_cell_info(
                          OSCTXT                         *p_asn1_ctx,
                          x2ap_ServedCell_Information    *p_x2ap_IE_serv_cell,
                          x2ap_srvd_cell_arry_t         *p_serv_cell)
{
    U16   brodcast_plmn_count = 0;
/*SPR 18793 changes start */
#ifdef LTE_EMBMS_SUPPORTED
    x2ap_served_cell_info_arr_t serv_cell ;
#endif
/*SPR 18793 changes end */

    X2AP_UT_TRACE_ENTER();

    /* Encode Cell ID */
    p_x2ap_IE_serv_cell->pCI = p_serv_cell->served_cell_info.pci;
 
    /* Encode Cell ID */
    p_x2ap_IE_serv_cell->cellId.pLMN_Identity.numocts = MAX_PLMN_ID_BYTES;
    X2AP_MEMCPY(p_x2ap_IE_serv_cell->cellId.pLMN_Identity.data,
                p_serv_cell->served_cell_info.cell_id
                                 .plmn_identity.plmn_id,
                MAX_PLMN_ID_BYTES);

    p_x2ap_IE_serv_cell->cellId.eUTRANcellIdentifier.numbits = 
                                               X2AP_EUTRAN_CELL_ID_BIT_STR_LEN;
    X2AP_MEMCPY(p_x2ap_IE_serv_cell->cellId.eUTRANcellIdentifier.data,
                &(p_serv_cell->served_cell_info.cell_id.eutran_cell_id),
                sizeof(U32));
    /* Encode TAC */
    p_x2ap_IE_serv_cell->tAC.numocts = MAX_TAC_BYTES;
    X2AP_MEMCPY(p_x2ap_IE_serv_cell->tAC.data,
                p_serv_cell->served_cell_info.track_area_code.tac,
                MAX_TAC_BYTES);

    /* Encode Broadcast PLMN*/
    p_x2ap_IE_serv_cell->broadcastPLMNs.n =
           p_serv_cell->served_cell_info.broadcast_plmn_info.num_bcast_plmn_id;
    for (brodcast_plmn_count = 0; brodcast_plmn_count < MAX_NUM_BPLMN; 
                                                  brodcast_plmn_count++) 
    {
        p_x2ap_IE_serv_cell->broadcastPLMNs.elem[brodcast_plmn_count].numocts =
                                                             MAX_PLMN_ID_BYTES;
        X2AP_MEMCPY(p_x2ap_IE_serv_cell->broadcastPLMNs.
                                  elem[brodcast_plmn_count].data,
                                  p_serv_cell->served_cell_info.
                                   broadcast_plmn_info.plmn_identity
                                   [brodcast_plmn_count].plmn_id,
                                  MAX_PLMN_ID_BYTES);       
    }
    /* Encode eUTRA Mode */
#if 0												  
#ifdef RRC_UNIT_TEST_FRAMEWORK
    p_serv_cell->served_cell_info.choice_eutra_mode.choice_eutra_mode.bitmask = X2AP_EUTRA_MODE_FDD;
#endif
#endif

    if (X2AP_EUTRA_MODE_FDD == p_serv_cell->served_cell_info.
                                   choice_eutra_mode.choice_eutra_mode.bitmask)
    {
         p_x2ap_IE_serv_cell->eUTRA_Mode_Info.t = T_x2ap_EUTRA_Mode_Info_fDD;
       
         p_x2ap_IE_serv_cell->eUTRA_Mode_Info.u.fDD = rtxMemAllocType(p_asn1_ctx,
                                                            x2ap_FDD_Info);
        if (X2AP_P_NULL == p_x2ap_IE_serv_cell->eUTRA_Mode_Info.u.fDD)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            return X2AP_FAILURE;
        }

        asn1Init_x2ap_FDD_Info(p_x2ap_IE_serv_cell->eUTRA_Mode_Info.u.fDD);
        /*** Compose FDD Info ***/
        compose_x2ap_FDD_Info (
                p_x2ap_IE_serv_cell->eUTRA_Mode_Info.u.fDD,
                &p_serv_cell->served_cell_info.
                choice_eutra_mode);
    }
    else if (X2AP_EUTRA_MODE_TDD == p_serv_cell->served_cell_info.
                                  choice_eutra_mode.choice_eutra_mode.bitmask)
    {
        p_x2ap_IE_serv_cell->eUTRA_Mode_Info.t = T_x2ap_EUTRA_Mode_Info_tDD;

        p_x2ap_IE_serv_cell->eUTRA_Mode_Info.u.tDD = rtxMemAllocType(p_asn1_ctx,
                                                            x2ap_TDD_Info);
        if (X2AP_P_NULL == p_x2ap_IE_serv_cell->eUTRA_Mode_Info.u.tDD)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            return X2AP_FAILURE;
        }

        asn1Init_x2ap_TDD_Info(p_x2ap_IE_serv_cell->eUTRA_Mode_Info.u.tDD);
        /*** Compose TDD Info ***/
        compose_x2ap_TDD_Info (
                p_x2ap_IE_serv_cell->eUTRA_Mode_Info.u.tDD,
                &p_serv_cell->served_cell_info.
                choice_eutra_mode);
    }
    else
    {
        /* eNB ID presence bitmask is not set */
        X2AP_TRACE(X2AP_ERROR,
                   "%s: neither FDD nor TDD mode"\
                   "Presence bitmask is set",__FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    if(p_serv_cell->served_cell_info.bitmask & X2AP_NUM_ANTENNA_INFO_PORT_PRESENT ||
    		p_serv_cell->served_cell_info.bitmask & X2AP_PRACH_CONFIG_INFO_PRESENT ||
    		p_serv_cell->served_cell_info.bitmask & X2AP_MBSFN_SUBFRAME_INFO_PRESENT ||
                p_serv_cell->served_cell_info.bitmask & X2AP_CSG_ID_INFO_PRESENT)
	{
		x2ap_return_et response = X2AP_FAILURE;
		x2ap_ServedCell_Information_iE_Extensions* p_served_cell_extn = X2AP_P_NULL;
	   	p_served_cell_extn = &p_x2ap_IE_serv_cell->iE_Extensions;
		p_x2ap_IE_serv_cell->m.iE_ExtensionsPresent = 1;

	    asn1Init_x2ap_ServedCell_Information_iE_Extensions(p_served_cell_extn);
		if(p_serv_cell->served_cell_info.bitmask & X2AP_NUM_ANTENNA_INFO_PORT_PRESENT)
		{
/* SPR 8178 Start */ 
			response = x2ap_compose_num_atenna_info(p_asn1_ctx,
                                    p_served_cell_extn, p_serv_cell);
/* SPR 8178 End */ 
		}
		if(p_serv_cell->served_cell_info.bitmask & X2AP_PRACH_CONFIG_INFO_PRESENT)
		{
			response = x2ap_compose_prach_config_info(p_asn1_ctx,
                                   p_served_cell_extn, &p_serv_cell->served_cell_info.prach_config);
		}
		if(p_serv_cell->served_cell_info.bitmask & X2AP_MBSFN_SUBFRAME_INFO_PRESENT)
		{
/*SPR 18793 changes start */
#ifdef LTE_EMBMS_SUPPORTED
            X2AP_MEMSET(&(serv_cell), X2AP_ZERO, sizeof(x2ap_served_cell_info_arr_t));
            X2AP_MEMCPY(&(serv_cell.cell_info.mbsfn_subframe_info), &(p_serv_cell->served_cell_info.mbsfn_subframe_info), 
                    sizeof(x2ap_mbsfn_subframe_info_t)); 
			response = x2ap_compose_mbsfn_info(p_asn1_ctx, p_served_cell_extn, &serv_cell.cell_info);
#endif
/*SPR 18793 changes end */
		}
		if(p_serv_cell->served_cell_info.bitmask & X2AP_CSG_ID_INFO_PRESENT)
		{
			response = x2ap_compose_csg_id_info(p_asn1_ctx, p_served_cell_extn,p_serv_cell);
		}

		if(response ==X2AP_FAILURE)
		{
			rtxDListFreeAll(p_asn1_ctx, p_served_cell_extn);
		}
	}
    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS; 
}

/*****************************************************************************
 * Function Name  : compose_x2ap_TDD_Info
 * Inputs         : 
 *                  p_x2ap_tdd_info - pointer to x2ap_TDD_Info
 *                  p_eutra_mode_info - This contains the information to
 *                                      encode eUTRA mode Info.
 * Outputs        : p_x2ap_tdd_info - This structure is populated with
 *                                        information from p_eutra_mode_info.
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE 
 * Description    : This function populates p_x2ap_tdd_info with
 *                  information necessary eUTRA mode Info.
 *****************************************************************************/
void compose_x2ap_TDD_Info(
                          x2ap_TDD_Info                  *p_x2ap_tdd_info,
                          x2ap_choice_eutra_mode_info_t *p_eutra_mode_info)
{
    X2AP_UT_TRACE_ENTER();

    /* Encode ERFCN */
    p_x2ap_tdd_info->eARFCN = (x2ap_EARFCN)(p_eutra_mode_info->
            choice_eutra_mode.tdd_info.earfcn.earfcn);
    /* Encode Tx Bandwith */
    p_x2ap_tdd_info->transmission_Bandwidth =
                         p_eutra_mode_info->choice_eutra_mode.
                                       tdd_info.trnsmission_bdwidth;

    /* Encode subframe*/
    p_x2ap_tdd_info->subframeAssignment =
                         p_eutra_mode_info->choice_eutra_mode.
                                        tdd_info.subframe_assn;

    /* Encode special sub frame */
    asn1Init_x2ap_SpecialSubframe_Info(&p_x2ap_tdd_info->specialSubframe_Info);

    p_x2ap_tdd_info->specialSubframe_Info.specialSubframePatterns
                = p_eutra_mode_info->choice_eutra_mode.
                   tdd_info.special_subframe_info.special_subframe_patterns;
    p_x2ap_tdd_info->specialSubframe_Info.cyclicPrefixDL
                = p_eutra_mode_info->choice_eutra_mode.tdd_info.
                           special_subframe_info.dl_cp;
    p_x2ap_tdd_info->specialSubframe_Info.cyclicPrefixUL
                = p_eutra_mode_info->choice_eutra_mode.tdd_info.
                           special_subframe_info.ul_cp;

    X2AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : compose_x2ap_FDD_Info
 * Inputs         : 
 *                  p_x2ap_fdd_info - pointer to x2ap_FDD_Info
 *                  p_eutra_mode_info - This contains the information to
 *                                      encode eUTRA mode Info.
 * Outputs        : p_x2ap_fdd_info - This structure is populated with
 *                                        information from p_eutra_mode_info.
 * Returns        : X2AP_SUCCESS - Asn Encoding was successful.
 *                  X2AP_FAILURE - Asn Encoding was not successful.
 * Description    : This function populates p_x2ap_fdd_info with
 *                  information necessary eUTRA mode Info.
 *****************************************************************************/
void compose_x2ap_FDD_Info(
                          x2ap_FDD_Info                  *p_x2ap_fdd_info,
                          x2ap_choice_eutra_mode_info_t *p_eutra_mode_info)
{
    X2AP_UT_TRACE_ENTER();

    /* Encode ERFCN */
    p_x2ap_fdd_info->uL_EARFCN = 
        (x2ap_EARFCN)(p_eutra_mode_info->
                      choice_eutra_mode.fdd_info.ul_earfcn.earfcn);
    /* Encode ERFCN */
    p_x2ap_fdd_info->dL_EARFCN = 
        (x2ap_EARFCN)(p_eutra_mode_info->
                      choice_eutra_mode.fdd_info.dl_earfcn.earfcn);
    /* Encode Tx Bandwith */
    p_x2ap_fdd_info->uL_Transmission_Bandwidth = 
               p_eutra_mode_info->choice_eutra_mode.fdd_info.
                                                       ul_trnsmission_bdwidth;
    /* Encode Tx Bandwith */
    p_x2ap_fdd_info->dL_Transmission_Bandwidth = 
               p_eutra_mode_info->choice_eutra_mode.fdd_info.
                                                       dl_trnsmission_bdwidth; 

    X2AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : x2ap_compose_cause
 * Inputs         : p_asn1_ctx - Pointer to OSCTXT.
 *                  p_x2ap_cause - pointer to x2ap_Cause
 *                  p_cause - This contains the information to encode global
 *                            eNB ID.
 * Outputs        : p_x2ap_cause - This structure is populated with
 *                                 information from p_x2ap_cause.
 * Returns        : X2AP_SUCCESS - Asn Encoding was successful.
 *                  X2AP_FAILURE - Asn Encoding was not successful.
 * Description    : This function populates p_x2ap_cause with
 *                  information necessary to encode global eNb ID.
 *****************************************************************************/
x2ap_return_et x2ap_compose_cause(
                             x2ap_Cause         *p_x2ap_cause,
                             x2_cause_t         *p_cause)
{
    X2AP_UT_TRACE_ENTER();

    p_x2ap_cause->t = p_cause->cause_type;

    if (T_x2ap_Cause_radioNetwork == p_x2ap_cause->t)
    {
        p_x2ap_cause->u.radioNetwork = p_cause->cause_info.radio_nw_cause; 
    }
    else if (T_x2ap_Cause_transport == p_x2ap_cause->t)
    {
        p_x2ap_cause->u.transport = p_cause->cause_info.transport_cause;
    }
    else if (T_x2ap_Cause_protocol == p_x2ap_cause->t)
    {
        p_x2ap_cause->u.protocol = p_cause->cause_info.protocol_cause;
    }
    else if (T_x2ap_Cause_misc == p_x2ap_cause->t) 
    {
        p_x2ap_cause->u.misc = p_cause->cause_info.misc_cause;
    }
    else
    {
        /* eNB ID presence bitmask is not set */
        X2AP_TRACE(X2AP_ERROR,"%s: No cause type is matched",__FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;

}

/*****************************************************************************
 * Function Name  : x2ap_compose_global_enb_id
 * Inputs         : p_asn1_ctx - Pointer to OSCTXT.
 *                  p_x2ap_IE_gb_enb_id - pointer to x2ap_GlobalENB_ID
 *                  p_gb_enb_id - This contains the information to
 *                                        encode global eNB ID.
 * Outputs        : p_x2ap_IE_gb_enb_id - This structure is populated with
 *                                        information from p_gb_enb_id.
 * Returns        : X2AP_SUCCESS - Asn Encoding was successful.
 *                  X2AP_FAILURE - Asn Encoding was not successful.
 * Description    : This function populates p_x2ap_IE_gb_enb_id with
 *                  information necessary to encode global eNb ID.
 *****************************************************************************/
x2ap_return_et x2ap_compose_global_enb_id(
        OSCTXT             *p_asn1_ctx,
        x2ap_GlobalENB_ID  *p_x2ap_IE_gb_enb_id,
        x2_gb_enb_id_t       *p_gb_enb_id)
{
	/* coverity_fix_54710_start */
	X2AP_UT_TRACE_ENTER();

	/* Set the PLMN ID field */
	p_x2ap_IE_gb_enb_id->pLMN_Identity.numocts = MAX_PLMN_ID_BYTES;

	X2AP_MEMCPY(
			p_x2ap_IE_gb_enb_id->pLMN_Identity.data,
			p_gb_enb_id->plmn_identity.plmn_id,
			MAX_PLMN_ID_BYTES); 

	/* If Macro eNB id present set the value */
#if 0			
#ifdef RRC_UNIT_TEST_FRAMEWORK
    p_gb_enb_id->enb_id.presence_bitmask = X2AP_OAM_HOME_ENB_ID_PRESENT;
#endif
#endif

	if (p_gb_enb_id->enb_id.presence_bitmask & X2AP_OAM_MACRO_ENB_ID_PRESENT)
	{
		p_x2ap_IE_gb_enb_id->eNB_ID.t = T_x2ap_ENB_ID_macro_eNB_ID;
		p_x2ap_IE_gb_enb_id->eNB_ID.u.macro_eNB_ID =  
			rtxMemAllocType(p_asn1_ctx,ASN1BitStr32);

		if (X2AP_P_NULL == p_x2ap_IE_gb_enb_id->eNB_ID.u.macro_eNB_ID)
		{
			X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
			return X2AP_FAILURE;
		}
		memset_wrapper(p_x2ap_IE_gb_enb_id->eNB_ID.u.macro_eNB_ID,X2AP_NULL,sizeof(ASN1BitStr32));

		p_x2ap_IE_gb_enb_id->eNB_ID.u.macro_eNB_ID->numbits =
			X2AP_MACRO_ENB_ID_BITSTRING_SIZE;

		X2AP_MEMCPY( 
				p_x2ap_IE_gb_enb_id->eNB_ID.u.macro_eNB_ID->data,
				p_gb_enb_id->enb_id.macro_enb_id.eNB_id,
				MACRO_ENB_ID_OCTET_SIZE);
	}
	else if (p_gb_enb_id->enb_id.presence_bitmask & 
			X2AP_OAM_HOME_ENB_ID_PRESENT)
	{
		p_x2ap_IE_gb_enb_id->eNB_ID.t = T_x2ap_ENB_ID_home_eNB_ID;

		p_x2ap_IE_gb_enb_id->eNB_ID.u.home_eNB_ID = 
			rtxMemAllocType(p_asn1_ctx,ASN1BitStr32);

		if (X2AP_P_NULL == p_x2ap_IE_gb_enb_id->eNB_ID.u.home_eNB_ID)
		{
			X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
			return X2AP_FAILURE;
		}
		memset_wrapper(p_x2ap_IE_gb_enb_id->eNB_ID.u.home_eNB_ID,X2AP_NULL,sizeof(ASN1BitStr32));

		p_x2ap_IE_gb_enb_id->eNB_ID.u.home_eNB_ID->numbits =
			X2AP_HOME_ENB_ID_BITSTRING_SIZE;

		X2AP_MEMCPY(
				p_x2ap_IE_gb_enb_id->eNB_ID.u.home_eNB_ID->data,
				p_gb_enb_id->enb_id.home_enb_id.eNB_id,
				HOME_ENB_ID_OCTET_SIZE);
	}
	else
	{
		/* eNB ID presence bitmask is not set */
		X2AP_TRACE(X2AP_ERROR,
				"%s: neither Macro or Home ENB ID"\
				"Presence bitmask not set",__FUNCTION__);
		X2AP_UT_TRACE_EXIT();
		return X2AP_FAILURE;
	}

	X2AP_UT_TRACE_EXIT();
	return X2AP_SUCCESS; 
}
/* coverity_fix_54710_stop */
/*****************************************************************************
 * Function Name  : x2ap_encode_x2setup_resp 
 * Inputs         : p_x2_setup_resp - pointer to x2_setup_resp_t
 *                  p_asn_msg - pointer to encoded ASN msg
 *                  p_asn_msg_len - pointer to encoded ASN msg length 
 * Outputs        : p_asn_msg - encoded ASN msg
 *                  p_asn_msg_len - encoded ASN msg length 
 * Returns        : X2AP_SUCCESS/X2ap_FAILURE
 * Description    : This function used to encode X2 Setup response.
 * ERROR          : This function report error to the caller function, If encoder
 *                  fails to encode message.
 ********************************************************************************/
x2ap_return_et x2ap_encode_x2setup_resp (
        x2_setup_resp_t   *p_x2_setup_resp,
        U8                 *p_asn_msg,
        U16                *p_asn_msg_len)
{
    x2ap_return_et              retVal = X2AP_FAILURE;

    OSCTXT                      asn1_ctx;
    x2ap_X2AP_PDU               x2ap_pdu;
    OSRTDListNode               *p_node = PNULL;
    x2ap_X2SetupResponse_protocolIEs_element *     p_protocolIE_elem = PNULL;
    x2ap_X2SetupResponse         *p_x2ap_X2SetupResponse = X2AP_P_NULL;
    x2ap_CriticalityDiagnostics  crit_diag;
    
    OSRTDList  x2SetupResponse_IEs_2;
    OSRTDList  x2SetupResponse_IEs_3;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    X2AP_UT_TRACE_ENTER();

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        X2AP_TRACE(X2AP_ERROR, "%s:ASN context initialization failed.",
                   __FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return retVal;
    }
    do
    {
        X2AP_MEMSET(&(x2ap_pdu), 0, sizeof(x2ap_X2AP_PDU));
        /* Fill the values in the ASN structures that shall be encoded by
         *  ASN Encoder */
        /* Set Pdu type to Initiating message */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_successfulOutcome; 

        x2ap_pdu.u.successfulOutcome = rtxMemAllocType(&asn1_ctx,
                x2ap_SuccessfulOutcome);
        if (X2AP_P_NULL == x2ap_pdu.u.successfulOutcome)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_SuccessfulOutcome(x2ap_pdu.u.successfulOutcome);
   
        x2ap_pdu.u.successfulOutcome->procedureCode = ASN1V_x2ap_id_x2Setup;
        x2ap_pdu.u.successfulOutcome->criticality = x2ap_reject;
        
        /* Set the initiating message type to X2 Setup */ 
        x2ap_pdu.u.successfulOutcome->value.t = T1x2ap__x2Setup;

        p_x2ap_X2SetupResponse = rtxMemAllocType(&asn1_ctx,
                                                x2ap_X2SetupResponse);  
        if (X2AP_P_NULL == p_x2ap_X2SetupResponse)
        {
             X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
             break;
        }
        else
        {
            X2AP_TRACE(X2AP_DETAILED,
                    "Mem Alloc successful to x2ap_X2SetupResponse");
        }

        asn1Init_x2ap_X2SetupResponse (p_x2ap_X2SetupResponse);
        x2ap_pdu.u.successfulOutcome->value.u.x2Setup = p_x2ap_X2SetupResponse;


        /*** Compose id-Global-ENB-ID start ***/
        rtxDListAllocNodeAndData(&asn1_ctx,
                x2ap_X2SetupResponse_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if (X2AP_P_NULL == p_node)
        {
            /* Not enough memory */
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_X2SetupResponse_protocolIEs_element(
                p_protocolIE_elem);

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_x2ap_id_GlobalENB_ID;
        p_protocolIE_elem->criticality = x2ap_reject;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T25x2ap___x2ap_X2SetupResponse_IEs_1;

        p_protocolIE_elem->value.u._x2ap_X2SetupResponse_IEs_1 =
            rtxMemAllocType(&asn1_ctx,x2ap_GlobalENB_ID);
        if (X2AP_P_NULL == p_protocolIE_elem->value.u._x2ap_X2SetupResponse_IEs_1)
        {
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_GlobalENB_ID (p_protocolIE_elem->value.u.
                _x2ap_X2SetupResponse_IEs_1);

        if (X2AP_FAILURE == x2ap_compose_global_enb_id(&asn1_ctx,
                             p_protocolIE_elem->value.u._x2ap_X2SetupResponse_IEs_1,
                             &p_x2_setup_resp->global_enb_id))
        {
            X2AP_TRACE(X2AP_ERROR,
                       "%s:compose id-Global-ENB-ID returned Failure.",
                       __FUNCTION__);
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }
        else
        {
            X2AP_TRACE(X2AP_INFO,
                       "%s:compose id-Global-ENB-ID returned Success.",
                       __FUNCTION__);
        }
        rtxDListAppendNode(&p_x2ap_X2SetupResponse->protocolIEs, p_node);

        /*** Compose id-Global-ENB-ID end ***/
        ///////////////////////////////////////////////////////////
        /*** Compose id-ServedCells start ***/
        rtxDListAllocNodeAndData(
                &asn1_ctx,                                  
                x2ap_X2SetupResponse_protocolIEs_element,                     
                &p_node,                                                      
                &p_protocolIE_elem);        
        if (X2AP_P_NULL == p_node)
        {                                                                     
            /* Not enough memory */                                      
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.", __FUNCTION__);    
            break;                                                         
        }                                                                   
        asn1Init_x2ap_X2SetupResponse_protocolIEs_element(
                 p_protocolIE_elem);

        /* Fill protocol IE Id and criticality */                              
        p_protocolIE_elem->id = ASN1V_x2ap_id_ServedCells;                   
        p_protocolIE_elem->criticality = x2ap_reject;  

        /* Fill the type of Protocol IE element */                            
        p_protocolIE_elem->value.t = T25x2ap___x2ap_X2SetupResponse_IEs_2;  

        rtxDListInit(&x2SetupResponse_IEs_2);                                 
        p_protocolIE_elem->value.u._x2ap_X2SetupResponse_IEs_2  =                 
                                        &x2SetupResponse_IEs_2;

        if (X2AP_FAILURE == x2ap_compose_served_cell(
                    &asn1_ctx,
                    p_protocolIE_elem->value.u._x2ap_X2SetupResponse_IEs_2,
                    p_x2_setup_resp->served_cell_arr,
                    p_x2_setup_resp->num_served_cell) )
        {
            X2AP_TRACE(X2AP_ERROR,
                       "%s:compose_served_cell returned Failure.",
                       __FUNCTION__);
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }
        else
        {

            X2AP_TRACE(X2AP_INFO,
                       "%s:compose_served_cell returned Success.",
                       __FUNCTION__);
        }
        rtxDListAppendNode(&p_x2ap_X2SetupResponse->protocolIEs, p_node);
        /*** Compose id-ServedCells end ***/
        //////////////////////////////////////////

        /*** Compose id-GUGroupIDList start ***/
        if(p_x2_setup_resp->num_gu_group_id)
        {
            rtxDListAllocNodeAndData(
                    &asn1_ctx,                                         
                    x2ap_X2SetupResponse_protocolIEs_element,                 
                    &p_node,                                                   
                    &p_protocolIE_elem);                                       
            if (X2AP_P_NULL == p_node)
            {                                                                  
                /* Not enough memory */                                        
                X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",               
                        __FUNCTION__);                                         
                break;                                                         
            }                                                                  
            asn1Init_x2ap_X2SetupResponse_protocolIEs_element(
                    p_protocolIE_elem);

            /* Fill protocol IE Id and criticality */                          
            p_protocolIE_elem->id = ASN1V_x2ap_id_GUGroupIDList;              
            p_protocolIE_elem->criticality = x2ap_reject;  

            /* Fill the type of Protocol IE element */                         
            p_protocolIE_elem->value.t = T25x2ap___x2ap_X2SetupResponse_IEs_3;  

            rtxDListInit(&x2SetupResponse_IEs_3);                              
            p_protocolIE_elem->value.u._x2ap_X2SetupResponse_IEs_3  =              
                &x2SetupResponse_IEs_3;

            if ( X2AP_FAILURE == x2ap_compose_GUGroupIDList(&asn1_ctx,
                        p_protocolIE_elem->value.u._x2ap_X2SetupResponse_IEs_3,
                        p_x2_setup_resp->group_id_arr,
                        p_x2_setup_resp->num_gu_group_id))
            {
                X2AP_TRACE(X2AP_ERROR,
                        "%s:x2ap_compose_GUGroupIDList returned Failure.\n",
                        __FUNCTION__);
                /*SPR_18125_START*/
                rtFreeContext(&asn1_ctx);
                /*SPR_18125_END*/
                X2AP_UT_TRACE_EXIT();
                return X2AP_FAILURE;
            }
            else
            {

                X2AP_TRACE(X2AP_INFO,
                        "%s:x2ap_compose_GUGroupIDList returned Success.\n",
                        __FUNCTION__);
            }
            rtxDListAppendNode(&p_x2ap_X2SetupResponse->protocolIEs, p_node);
            /*** Compose id-GUGroupIDList end ***/ 
        }
        //////////////////////////////////////////
        /****************************critical diag******/
        rtxDListAllocNodeAndData(&asn1_ctx,
                x2ap_X2SetupResponse_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if (PNULL == p_node)
        {
            /* Not enough memory */
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
        }
        if (X2AP_P_NULL == p_protocolIE_elem)
        {
            /* Not enough memory */
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",
                    __FUNCTION__);
            break;
        }

        X2AP_MEMSET(p_protocolIE_elem,0,
                sizeof(x2ap_X2SetupResponse_protocolIEs_element));

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_x2ap_id_CriticalityDiagnostics;
        p_protocolIE_elem->criticality = x2ap_ignore;


        X2AP_MEMSET(&crit_diag,0,sizeof(x2ap_CriticalityDiagnostics));
        rtxDListInit(&crit_diag.iEsCriticalityDiagnostics);
        rtxDListInit(&crit_diag.extElem1);
        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T25x2ap___x2ap_X2SetupResponse_IEs_4;
        p_protocolIE_elem->value.u._x2ap_X2SetupResponse_IEs_4 = &crit_diag;

        rtxDListAppendNode(&p_x2ap_X2SetupResponse->protocolIEs, p_node);
        /***********************************/

        rtxDListInit(&p_x2ap_X2SetupResponse->extElem1);

        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_asn_msg,
                     X2AP_MAX_ASN1_BUF_LEN, TRUE);

        if (0 != asn1PE_x2ap_X2AP_PDU(&asn1_ctx, &x2ap_pdu))
        {
/*SPR 18793 changes start */
            rtxErrPrint(&asn1_ctx);
/*SPR 18793 changes end */
            X2AP_TRACE(X2AP_ERROR, "ASN1 encoding of X2 Setup Response "
                    "failed.");
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }
        else
        {
            *p_asn_msg_len= (U16)pe_GetMsgLen(&asn1_ctx);
            retVal = X2AP_SUCCESS;
            
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
            
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    (U32)*p_asn_msg_len);
            if (PNULL != p_event_header)
            {
                x2ap_fill_protocol_event_params(p_event_header, (U32)*p_asn_msg_len,
                        LOCAL_X2_SETUP_RESPONSE, 
                        MESSAGE_DIRECTION_SEND);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        (U32)*p_asn_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

    }while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_encode_x2setup_failure_resp 
 * Inputs         : p_x2_setup_resp - pointer to x2_setup_fail_t
 *                  p_asn_msg - pointer to ASN encoded msg 
 *                  p_asn_msg_len -  pointer to ASN encoded msg length 
 *                  p_error_indication -  pointer to
 *                                             rrc_x2ap_error_indication_t
 *                  p_snd_err_ind - pointer to x2ap_error_ind_bool_t 
 * Outputs        : None 
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to encode X2 Setup failure response.
 * ERROR          : his function report error to the caller function, If encoder
 *                  fails  to encode message.
 ********************************************************************************/
x2ap_return_et x2ap_encode_x2setup_failure_resp (
                                         x2_setup_fail_t   *p_x2_setup_resp,
                                         U8                 *p_asn_msg,
                                         U16                *p_asn_msg_len,
                            rrc_x2ap_error_indication_t *p_error_indication,
                            x2ap_error_ind_bool_t *p_snd_err_ind)
{
    x2ap_return_et              retVal = X2AP_FAILURE;
    OSCTXT                      asn1_ctx;
    x2ap_X2AP_PDU               x2ap_pdu;
    OSRTDListNode               *p_node = X2AP_P_NULL;
    x2ap_X2SetupFailure         *p_x2ap_X2SetupFailure = X2AP_P_NULL;
    x2ap_X2SetupFailure_protocolIEs_element *p_protocolIE_elem =
                                                          X2AP_P_NULL;
    x2ap_CriticalityDiagnostics crit_diag;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    
    X2AP_UT_TRACE_ENTER();

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        X2AP_TRACE(X2AP_ERROR, "%s:ASN context initialization failed.",
                   __FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return retVal;
    }
    do
    {
        X2AP_MEMSET ( &(x2ap_pdu), 0, sizeof(x2ap_X2AP_PDU) );
        /* Fill the values in the ASN structures that shall be encoded by
         *  ASN Encoder */
        /* Set Pdu type to Initiating message */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_unsuccessfulOutcome;

        x2ap_pdu.u.unsuccessfulOutcome = rtxMemAllocType(&asn1_ctx,
                x2ap_UnsuccessfulOutcome);
        if (X2AP_P_NULL == x2ap_pdu.u.unsuccessfulOutcome)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_UnsuccessfulOutcome(x2ap_pdu.u.unsuccessfulOutcome);

        x2ap_pdu.u.unsuccessfulOutcome->procedureCode = ASN1V_x2ap_id_x2Setup;
        x2ap_pdu.u.unsuccessfulOutcome->criticality = x2ap_reject;

        /* Set the initiating message type to X2 Setup */
        x2ap_pdu.u.unsuccessfulOutcome->value.t = T1x2ap__x2Setup;

        p_x2ap_X2SetupFailure = rtxMemAllocType(&asn1_ctx,
                                                x2ap_X2SetupFailure);
        if (X2AP_P_NULL == p_x2ap_X2SetupFailure)
        {
             X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
             break;
        }
        asn1Init_x2ap_X2SetupFailure (p_x2ap_X2SetupFailure);

        x2ap_pdu.u.unsuccessfulOutcome->value.u.x2Setup = p_x2ap_X2SetupFailure;

        rtxDListInit(&p_x2ap_X2SetupFailure->protocolIEs);

        /*** Compose id-Cause start ***/
        rtxDListAllocNodeAndData(&asn1_ctx, 
                x2ap_X2SetupFailure_protocolIEs_element, 
                &p_node, 
                &p_protocolIE_elem);
        if (X2AP_P_NULL == p_node)
        {
            /* Not enough memory */
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        if (X2AP_P_NULL == p_protocolIE_elem)
        {
            /* Not enough memory */
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.", __FUNCTION__);
            break;
        }

        asn1Init_x2ap_X2SetupFailure_protocolIEs_element (
                p_protocolIE_elem);

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_x2ap_id_Cause;
        p_protocolIE_elem->criticality = x2ap_ignore;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T26x2ap___x2ap_X2SetupFailure_IEs_1;

        p_protocolIE_elem->value.u._x2ap_X2SetupFailure_IEs_1 =
            rtxMemAllocType(&asn1_ctx,x2ap_Cause);

        if (X2AP_P_NULL == p_protocolIE_elem->value.u._x2ap_X2SetupFailure_IEs_1)
        {
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_Cause (p_protocolIE_elem->value.u.
                _x2ap_X2SetupFailure_IEs_1);

        if (X2AP_FAILURE == x2ap_compose_cause(
                    p_protocolIE_elem->value.u._x2ap_X2SetupFailure_IEs_1,
                    &p_x2_setup_resp->cause))
        {
            X2AP_TRACE(X2AP_ERROR,
                       "%s:x2ap_compose_cause returned Failure.",
                       __FUNCTION__);
            break;
        }
        rtxDListAppendNode(&p_x2ap_X2SetupFailure->protocolIEs, p_node);  
        /*** Compose id-Cause end ***/

        /*** Compose id-TimeToWait start ***/
        if (p_x2_setup_resp->time_to_wait)
        {
        
          rtxDListAllocNodeAndData(&asn1_ctx, 
              x2ap_X2SetupFailure_protocolIEs_element, 
              &p_node, 
              &p_protocolIE_elem);
          if (X2AP_P_NULL == p_node)
          {
            /* Not enough memory */
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
          }
          asn1Init_x2ap_X2SetupFailure_protocolIEs_element (
              p_protocolIE_elem);

          /* Fill protocol IE Id and criticality */
          p_protocolIE_elem->id = ASN1V_x2ap_id_TimeToWait;
          p_protocolIE_elem->criticality = x2ap_ignore;

          /* Fill the type of Protocol IE element */
          p_protocolIE_elem->value.t = T26x2ap___x2ap_X2SetupFailure_IEs_2;
          /* BUG_7951 START */
          p_protocolIE_elem->value.u._x2ap_X2SetupFailure_IEs_2 =
                                  p_x2_setup_resp->time_to_wait;
          /* BUG_7951 STOP */

          rtxDListAppendNode(&p_x2ap_X2SetupFailure->protocolIEs, p_node);  

        }
        /*** Compose id-TimeToWait end ***/

        /*** Compose id-CriticalityDiagnostics start ***/
        if((PNULL != p_snd_err_ind) && (PNULL != p_error_indication))
        {
          if (X2AP_TRUE == p_snd_err_ind->send_err_indication_reject_ie_present ||
            X2AP_TRUE == p_snd_err_ind->send_err_indication_notify_ie_present)
          {
            rtxDListAllocNodeAndData(&asn1_ctx,
                x2ap_X2SetupFailure_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
            if (PNULL == p_node)
            {
                /* Not enough memory */
                X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                break;
            }
            X2AP_MEMSET(p_protocolIE_elem,0,
                sizeof(x2ap_X2SetupFailure_protocolIEs_element));

            /* Fill protocol IE Id and criticality */
            p_protocolIE_elem->id = ASN1V_x2ap_id_CriticalityDiagnostics;
            p_protocolIE_elem->criticality = x2ap_ignore;


            X2AP_MEMSET(&crit_diag,0,sizeof(x2ap_CriticalityDiagnostics));
            rtxDListInit(&crit_diag.iEsCriticalityDiagnostics);
            rtxDListInit(&crit_diag.extElem1);
            /* Fill the type of Protocol IE element */
            p_protocolIE_elem->value.t = T26x2ap___x2ap_X2SetupFailure_IEs_3;
            p_protocolIE_elem->value.u._x2ap_X2SetupFailure_IEs_3 = 
                        &p_error_indication->criticality_diagnostics;

            rtxDListAppendNode(&p_x2ap_X2SetupFailure->protocolIEs, p_node);
          }
        }
        else
        {
            rtxDListAllocNodeAndData(&asn1_ctx,
                x2ap_X2SetupFailure_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
            if (PNULL == p_node)
            {
                /* Not enough memory */
                X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                break;
            }
            X2AP_MEMSET(p_protocolIE_elem,0,
                sizeof(x2ap_X2SetupFailure_protocolIEs_element));

            /* Fill protocol IE Id and criticality */
            p_protocolIE_elem->id = ASN1V_x2ap_id_CriticalityDiagnostics;
            p_protocolIE_elem->criticality = x2ap_ignore;


            X2AP_MEMSET(&crit_diag,0,sizeof(x2ap_CriticalityDiagnostics));
            rtxDListInit(&crit_diag.iEsCriticalityDiagnostics);
            rtxDListInit(&crit_diag.extElem1);
            /* Fill the type of Protocol IE element */
            p_protocolIE_elem->value.t = T26x2ap___x2ap_X2SetupFailure_IEs_3;
            p_protocolIE_elem->value.u._x2ap_X2SetupFailure_IEs_3 = &crit_diag;

            rtxDListAppendNode(&p_x2ap_X2SetupFailure->protocolIEs, p_node);
        }

        /*** Compose id-CriticalityDiagnostics end ***/

        rtxDListInit(&p_x2ap_X2SetupFailure->extElem1);        

        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_asn_msg,
                     X2AP_MAX_ASN1_BUF_LEN, TRUE);

        if (0 != asn1PE_x2ap_X2AP_PDU(&asn1_ctx, &x2ap_pdu))
        {
            X2AP_TRACE(X2AP_ERROR, "ASN1 encoding of X2 Setup Response"
                    "failed.");
            break;
        }
        else
        {
            *p_asn_msg_len= (U16)pe_GetMsgLen(&asn1_ctx);
            retVal = X2AP_SUCCESS;
           
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
            
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    (U32)*p_asn_msg_len);
            if (PNULL != p_event_header)
            {
                x2ap_fill_protocol_event_params(p_event_header, (U32)*p_asn_msg_len,
                        LOCAL_X2_SETUP_FAILURE, 
                        MESSAGE_DIRECTION_SEND);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        (U32)*p_asn_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }
    }while(0);
    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/******************************************************************************
 * FUNCTION NAME : x2ap_internal_encode_handover_req
 * INPUTS        : pctxt - pointer to OSCTXT
 *                 p_buff - pointer to ASN.1 encoded message
 *                 p_buff_size - size of result buffer
 *                 p_handover_req - pointer to rrc_x2ap_handover_request_t
 * OUTPUTS     : p_buff (ASN.1 encoded message),
 *               p_buff_size (size of result buffer)
 *
 * RETURNS     : X2AP_SUCCESS / X2AP_FAILURE    
 * DESCRIPTION   : This function used to encode Handover request.
 *****************************************************************************/
x2ap_return_et x2ap_internal_encode_handover_req 
(
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_x2ap_handover_request_t  *p_handover_req
)
{
    x2ap_return_et           result = X2AP_FAILURE;
    x2ap_X2AP_PDU            x2ap_pdu;
    x2ap_HandoverRequest     *p_x2ap_msg = PNULL;
    
    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != pctxt);
    X2AP_ASSERT(PNULL != p_buff);
    X2AP_ASSERT(PNULL != p_buff_size);
    X2AP_ASSERT(PNULL != p_handover_req);

    do
    {
        /* Init X2AP PDU */
        asn1Init_x2ap_X2AP_PDU(&x2ap_pdu);
        /* Set Pdu type to Initiating message */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_initiatingMessage;

        x2ap_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, x2ap_InitiatingMessage);
        if (PNULL == x2ap_pdu.u.initiatingMessage)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        asn1Init_x2ap_InitiatingMessage(x2ap_pdu.u.initiatingMessage);

        x2ap_pdu.u.initiatingMessage->procedureCode =
            ASN1V_x2ap_id_handoverPreparation;
         
        x2ap_pdu.u.initiatingMessage->criticality = x2ap_reject;
        
        /* Set the initiating message type to handover request */
        x2ap_pdu.u.initiatingMessage->value.t = T1x2ap__handoverPreparation;
        x2ap_pdu.u.initiatingMessage->value.u.handoverPreparation = 
            rtxMemAllocType(pctxt,x2ap_HandoverRequest);
        if (PNULL == x2ap_pdu.u.initiatingMessage->value.u.handoverPreparation)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        p_x2ap_msg = x2ap_pdu.u.initiatingMessage->value.u.handoverPreparation;
        
        asn1Init_x2ap_HandoverRequest(p_x2ap_msg);
        
        if (RT_OK != asn1Append_x2ap_HandoverRequest_protocolIEs_1(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            p_handover_req->x2ap_Old_UE_X2AP_ID))
        {
            break;
        }

        if (RT_OK != asn1Append_x2ap_HandoverRequest_protocolIEs_2(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            &p_handover_req->cause))
        {
            break;
        }

        if (RT_OK != asn1Append_x2ap_HandoverRequest_protocolIEs_3(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            &p_handover_req->target_cell_id))
        {
            break;
        }

        if (RT_OK != asn1Append_x2ap_HandoverRequest_protocolIEs_4(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            &p_handover_req->gummei))
        {
            break;
        }

        if (RT_OK != asn1Append_x2ap_HandoverRequest_protocolIEs_5(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            &p_handover_req->rrc_x2ap_ue_ctx_info.ue_context_info))
        {
            break;
        }

        if (RT_OK != asn1Append_x2ap_HandoverRequest_protocolIEs_6(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            &p_handover_req->ue_history_info))
        {
            break;
        }
        

        if (
          X2AP_HANDOVER_REQ_TRACE_ACTIVATION_PRESENT
          &
          p_handover_req->bitmask)
        {

            if (RT_OK != asn1Append_x2ap_HandoverRequest_protocolIEs_7(
                pctxt,
                &p_x2ap_msg->protocolIEs,
                &p_handover_req->trace_activation))
            {
                break;
            }
        }

        if (
          X2AP_HANDOVER_REQ_SRVCC_OPERATION_POSSIBLE_PRESENT 
          &
          p_handover_req->bitmask)
        {

            if (RT_OK != asn1Append_x2ap_HandoverRequest_protocolIEs_8(
                pctxt,
                &p_x2ap_msg->protocolIEs,
                p_handover_req->srv_operation_possible))
            {
                break;
            }
        }
        
        if (
          X2AP_HANDOVER_REQ_CSG_MEMBERSHIP_STATUS_PRESENT 
          &
          p_handover_req->bitmask)
        {

            if (RT_OK != asn1Append_x2ap_HandoverRequest_protocolIEs_9(
                pctxt,
                &p_x2ap_msg->protocolIEs,
                p_handover_req->csg_membership_status))
            {
                break;
            }
        }

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, X2AP_MAX_ASN1_BUF_LEN, ASN_X2AP_BUF_TYPE);
        
        if (RT_OK != asn1PE_x2ap_X2AP_PDU(pctxt, &x2ap_pdu))
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN1 encoding failed.",__FUNCTION__);
            break;
        }
        else
        {
            *p_buff_size = (U16)pe_GetMsgLen(pctxt);
        
            
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
        
            result=X2AP_SUCCESS;
        }
    } while(0);
    X2AP_UT_TRACE_EXIT();
    return result;
}
/*******************************************************************************
 * FUNCTION NAME : x2ap_internal_encode_handover_req_ack
 * INPUTS        : pctxt - pointer to OSCTXT
 *                 p_buff - pointer to ASN.1 encoded message
 *                 p_buff_size - size of result buffer
 *                 p_handover_req_ack - pointer to rrc_x2ap_handover_req_ack_t
 * OUTPUTS     : p_buff (ASN.1 encoded message),
 *               p_buff_size (size of result buffer)
 *
 * RETURNS     : X2AP_SUCCESS / X2AP_FAILURE    
 * DESCRIPTION   : This function used to encode Handover request acknowledge.
 *******************************************************************************/
x2ap_return_et x2ap_internal_encode_handover_req_ack 
(
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_x2ap_handover_req_ack_t  *p_handover_req_ack
)
{
    x2ap_return_et                   result = X2AP_FAILURE;
    x2ap_X2AP_PDU                    x2ap_pdu;
    x2ap_HandoverRequestAcknowledge  *p_x2ap_msg = PNULL;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != pctxt);
    X2AP_ASSERT(PNULL != p_buff);
    X2AP_ASSERT(PNULL != p_buff_size);
    X2AP_ASSERT(PNULL != p_handover_req_ack);

    do
    {
        /* Init X2AP PDU */
        asn1Init_x2ap_X2AP_PDU(&x2ap_pdu);
        /* Set Pdu type to successful outcome */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_successfulOutcome;

        x2ap_pdu.u.successfulOutcome =
            rtxMemAllocType(pctxt, x2ap_SuccessfulOutcome);
        if (PNULL == x2ap_pdu.u.successfulOutcome)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        asn1Init_x2ap_SuccessfulOutcome(x2ap_pdu.u.successfulOutcome);

        x2ap_pdu.u.successfulOutcome->procedureCode =
            ASN1V_x2ap_id_handoverPreparation;
         
        x2ap_pdu.u.successfulOutcome->criticality = x2ap_reject;
        
        /* Set the initiating message type to handover request acknowledge */
        x2ap_pdu.u.successfulOutcome->value.t = T1x2ap__handoverPreparation;

        x2ap_pdu.u.successfulOutcome->value.u.handoverPreparation = 
            rtxMemAllocType(pctxt,x2ap_HandoverRequestAcknowledge);
        if (PNULL == x2ap_pdu.u.successfulOutcome->value.u.handoverPreparation)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        p_x2ap_msg = x2ap_pdu.u.successfulOutcome->value.u.handoverPreparation;
        
        asn1Init_x2ap_HandoverRequestAcknowledge(p_x2ap_msg);

        if (RT_OK != asn1Append_x2ap_HandoverRequestAcknowledge_protocolIEs_1(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            p_handover_req_ack->old_ue_x2ap_id))
        {
            break;
        }


        if (RT_OK != asn1Append_x2ap_HandoverRequestAcknowledge_protocolIEs_2(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            p_handover_req_ack->new_ue_x2ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_x2ap_HandoverRequestAcknowledge_protocolIEs_3(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            &p_handover_req_ack->e_rab_admitted_list))
        {
            break;
        }

        if (
          X2AP_HANDOVER_REQ_ACK_ERAB_NOT_ADMITTED_LIST_PRESENT
          &
          p_handover_req_ack->bitmask)
        {
            if (RT_OK != 
                    asn1Append_x2ap_HandoverRequestAcknowledge_protocolIEs_4(
                        pctxt,
                        &p_x2ap_msg->protocolIEs,
                        &p_handover_req_ack->e_rab_not_admitted_list))
            {
                break;
            }
        }

        if (RT_OK != asn1Append_x2ap_HandoverRequestAcknowledge_protocolIEs_5(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            &p_handover_req_ack->target_to_src_enb_transparent_cont.
            target_to_source_transparent_container))
        {
            break;
        }

        if (
          X2AP_HANDOVER_REQ_ACK_CRIT_DIAGNO_PRESENT
          &
          p_handover_req_ack->bitmask)
        {
            if (RT_OK != 
                    asn1Append_x2ap_HandoverRequestAcknowledge_protocolIEs_6(
                        pctxt,
                        &p_x2ap_msg->protocolIEs,
                        &p_handover_req_ack->criticality_diagnostics))
            {
                break;
            }
        }

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, X2AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);
        
        if (RT_OK != asn1PE_x2ap_X2AP_PDU(pctxt, &x2ap_pdu))
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN1 encoding failed.",__FUNCTION__);
            break;
        } 
        else
        {
          
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
        }
        
        *p_buff_size = (U16)pe_GetMsgLen(pctxt);
        
        result=X2AP_SUCCESS;
    } while(0);
    X2AP_UT_TRACE_EXIT();
    return result;
}
/*******************************************************************************
 * FUNCTION NAME : x2ap_internal_encode_handover_preparation_failure
 * INPUTS        : pctxt - poiinter to OSCTXT
 *                 p_buff - pointer to ASN.1 encoded message
 *                 p_buff_size - size of result buffer
 *                 p_handover_prep_fail - pointer to rrc_x2ap_handover_prep_fail_t
 * OUTPUTS     : p_buff (ASN.1 encoded message),
 *               p_buff_size (size of result buffer)
 * RETURNS     : X2AP_SUCCESS / X2AP_FAILURE    
 * DESCRIPTION   : This function used to encode Handover preparation failure.
 *******************************************************************************/
x2ap_return_et x2ap_internal_encode_handover_preparation_failure 
(
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_x2ap_handover_prep_fail_t  *p_handover_prep_fail
)
{
    x2ap_return_et                   result = X2AP_FAILURE;
    x2ap_X2AP_PDU                    x2ap_pdu;
    x2ap_HandoverPreparationFailure  *p_x2ap_msg = PNULL;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != pctxt);
    X2AP_ASSERT(PNULL != p_buff);
    X2AP_ASSERT(PNULL != p_buff_size);
    X2AP_ASSERT(PNULL != p_handover_prep_fail);

    do
    {
        /* Init X2AP PDU */
        asn1Init_x2ap_X2AP_PDU(&x2ap_pdu);
        /* Set Pdu type to Unsuccessful Outcome */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_unsuccessfulOutcome;

        x2ap_pdu.u.unsuccessfulOutcome =
            rtxMemAllocType(pctxt, x2ap_UnsuccessfulOutcome);
        if (PNULL == x2ap_pdu.u.unsuccessfulOutcome)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        asn1Init_x2ap_UnsuccessfulOutcome(x2ap_pdu.u.unsuccessfulOutcome);

        x2ap_pdu.u.unsuccessfulOutcome->procedureCode =
            ASN1V_x2ap_id_handoverPreparation;
         
        x2ap_pdu.u.unsuccessfulOutcome->criticality = x2ap_reject;
        
        /* Set the initiating message type to handover preparation failure */
        x2ap_pdu.u.unsuccessfulOutcome->value.t = T1x2ap__handoverPreparation;

        x2ap_pdu.u.unsuccessfulOutcome->value.u.handoverPreparation = 
            rtxMemAllocType(pctxt,x2ap_HandoverPreparationFailure);
        if (PNULL == x2ap_pdu.u.unsuccessfulOutcome->value.u.handoverPreparation)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        p_x2ap_msg = x2ap_pdu.u.unsuccessfulOutcome->value.u.handoverPreparation;
        
        asn1Init_x2ap_HandoverPreparationFailure(p_x2ap_msg);

        if (RT_OK != asn1Append_x2ap_HandoverPreparationFailure_protocolIEs_1(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            p_handover_prep_fail->x2ap_Old_UE_X2AP_ID))
        {
            break;
        }

        if (RT_OK != asn1Append_x2ap_HandoverPreparationFailure_protocolIEs_2(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            &p_handover_prep_fail->cause))
        {
            break;
        }

        if (
          X2AP_HANDOVER_PREP_FAIL_CRIT_DIAGNO_PRESENT   
          &
          p_handover_prep_fail->bitmask)
        {
          if (RT_OK != asn1Append_x2ap_HandoverPreparationFailure_protocolIEs_3(
              pctxt,
              &p_x2ap_msg->protocolIEs,
              &p_handover_prep_fail->criticality_diagnostics))
          {
              break;
          }
        }

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, X2AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);
        
        if (RT_OK != asn1PE_x2ap_X2AP_PDU(pctxt, &x2ap_pdu))
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN1 encoding failed.",__FUNCTION__);
            break;
        }
        else
        {
            
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
        }
        
        *p_buff_size = (U16)pe_GetMsgLen(pctxt);
        
        result=X2AP_SUCCESS;
    } while(0);
    X2AP_UT_TRACE_EXIT();
    return result;
}
/*******************************************************************************
 * FUNCTION NAME : x2ap_internal_encode_sn_status_transfer
 * INPUTS        : pctxt - poiinter to OSCTXT
 *                 p_buff - pointer to ASN.1 encoded message
 *                 p_buff_size - size of result buffer
 *                 p_sn_status_transfer - pointer to rrc_x2ap_sn_status_transfer_t
 * OUTPUTS     : p_buff (ASN.1 encoded message),
 *               p_buff_size (size of result buffer)
 *
 * RETURNS     : X2AP_SUCCESS / X2AP_FAILURE    
 * DESCRIPTION   : This function used to encode sn status transfer
 *******************************************************************************/
x2ap_return_et x2ap_internal_encode_sn_status_transfer 
(
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_x2ap_sn_status_transfer_t  *p_sn_status_transfer
)
{
    x2ap_return_et           result = X2AP_FAILURE;
    x2ap_X2AP_PDU            x2ap_pdu;
    x2ap_SNStatusTransfer    *p_x2ap_msg = PNULL;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != pctxt);
    X2AP_ASSERT(PNULL != p_buff);
    X2AP_ASSERT(PNULL != p_buff_size);
    X2AP_ASSERT(PNULL != p_sn_status_transfer);

    do
    {
        /* Init X2AP PDU */
        asn1Init_x2ap_X2AP_PDU(&x2ap_pdu);
        /* Set Pdu type to Initiating message */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_initiatingMessage;

        x2ap_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, x2ap_InitiatingMessage);
        if (PNULL == x2ap_pdu.u.initiatingMessage)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        asn1Init_x2ap_InitiatingMessage(x2ap_pdu.u.initiatingMessage);

        x2ap_pdu.u.initiatingMessage->procedureCode =
            ASN1V_x2ap_id_snStatusTransfer;
         
        x2ap_pdu.u.initiatingMessage->criticality = x2ap_ignore;
        
        /* Set the initiating message type to sn status transfer*/
        x2ap_pdu.u.initiatingMessage->value.t = T1x2ap__snStatusTransfer;

        x2ap_pdu.u.initiatingMessage->value.u.snStatusTransfer = 
            rtxMemAllocType(pctxt,x2ap_SNStatusTransfer);
        if (PNULL == x2ap_pdu.u.initiatingMessage->value.u.snStatusTransfer)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        p_x2ap_msg = x2ap_pdu.u.initiatingMessage->value.u.snStatusTransfer;
        
        asn1Init_x2ap_SNStatusTransfer(p_x2ap_msg);
        
        if (RT_OK != asn1Append_x2ap_SNStatusTransfer_protocolIEs_1(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            p_sn_status_transfer->x2ap_Old_UE_X2AP_ID))
        {
            break;
        }
        if (RT_OK != asn1Append_x2ap_SNStatusTransfer_protocolIEs_2(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            p_sn_status_transfer->x2ap_New_UE_X2AP_ID))
        {
            break;
        }
        if (RT_OK != asn1Append_x2ap_SNStatusTransfer_protocolIEs_3(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            &p_sn_status_transfer->erab_subject_to_status_transfer_list))
        {
            break;
        }
        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, X2AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);
        
        if (RT_OK != asn1PE_x2ap_X2AP_PDU(pctxt, &x2ap_pdu))
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN1 encoding failed.",__FUNCTION__);
            break;
        } 
        else
        {
            
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
        }
        
        *p_buff_size = (U16)pe_GetMsgLen(pctxt);
        
        result=X2AP_SUCCESS;
    } while(0);
    X2AP_UT_TRACE_EXIT();
    return result;
}
/*******************************************************************************
 * FUNCTION NAME : x2ap_internal_encode_ue_context_release
 * INPUTS        : pctxt - poiinter to OSCTXT
 *                 p_buff - pointer to ASN.1 encoded message
 *                 p_buff_size - size of result buffer
 *                 p_ue_context_release - pointer to rrc_x2ap_ue_context_release_t
 * OUTPUTS     : p_buff (ASN.1 encoded message),
 *               p_buff_size (size of result buffer)
 * RETURNS     : X2AP_SUCCESS / X2AP_FAILURE    
 * DESCRIPTION   : This function used to ue context release message
 *******************************************************************************/
x2ap_return_et x2ap_internal_encode_ue_context_release 
(
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_x2ap_ue_context_release_t  *p_ue_context_release
)
{
    x2ap_return_et           result = X2AP_FAILURE;
    x2ap_X2AP_PDU            x2ap_pdu;
    x2ap_UEContextRelease    *p_x2ap_msg = PNULL;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != pctxt);
    X2AP_ASSERT(PNULL != p_buff);
    X2AP_ASSERT(PNULL != p_buff_size);
    X2AP_ASSERT(PNULL != p_ue_context_release);

    do
    {
        /* Init X2AP PDU */
        asn1Init_x2ap_X2AP_PDU(&x2ap_pdu);
        /* Set Pdu type to Initiating message */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_initiatingMessage;

        x2ap_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, x2ap_InitiatingMessage);
        if (PNULL == x2ap_pdu.u.initiatingMessage)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        asn1Init_x2ap_InitiatingMessage(x2ap_pdu.u.initiatingMessage);

        x2ap_pdu.u.initiatingMessage->procedureCode =
            ASN1V_x2ap_id_uEContextRelease;
         
        x2ap_pdu.u.initiatingMessage->criticality = x2ap_ignore;
        
        /* Set the initiating message type to ue context release */
        x2ap_pdu.u.initiatingMessage->value.t = T1x2ap__uEContextRelease;

        x2ap_pdu.u.initiatingMessage->value.u.uEContextRelease = 
            rtxMemAllocType(pctxt,x2ap_UEContextRelease);
        if (PNULL == x2ap_pdu.u.initiatingMessage->value.u.uEContextRelease)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        p_x2ap_msg = x2ap_pdu.u.initiatingMessage->value.u.uEContextRelease;
        
        asn1Init_x2ap_UEContextRelease(p_x2ap_msg);
        
        if (RT_OK != asn1Append_x2ap_UEContextRelease_protocolIEs_1(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            p_ue_context_release->x2ap_Old_UE_X2AP_ID))
        {
            break;
        }

        if (RT_OK != asn1Append_x2ap_UEContextRelease_protocolIEs_2(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            p_ue_context_release->x2ap_New_UE_X2AP_ID))
        {
            break;
        }

#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
        if (X2AP_UE_CONTEXT_RELEASE_SGNB_UE_X2AP_ID_PRESENT &
                p_ue_context_release->bitmask)
        {
            if (RT_OK != asn1Append_x2ap_UEContextRelease_protocolIEs_6(
                        pctxt,
                        &p_x2ap_msg->protocolIEs,
                        p_ue_context_release->x2ap_SgNB_UE_X2AP_ID))
            {
                break;
            }
        }
/*NR_DC Code Change Stop*/
#endif

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, X2AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);
        
        if (RT_OK != asn1PE_x2ap_X2AP_PDU(pctxt, &x2ap_pdu))
        {
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
            rtxErrPrint(pctxt);
            X2AP_TRACE(X2AP_ERROR,"%s:ASN1 encoding failed.",__FUNCTION__);
            break;
        } 
        else
        {
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
        }
        
        *p_buff_size = (U16)pe_GetMsgLen(pctxt);
        
        result=X2AP_SUCCESS;
    } while(0);
    X2AP_UT_TRACE_EXIT();
    return result;
}

/*******************************************************************************
 * FUNCTION NAME : x2ap_internal_encode_handover_cancel
 * INPUTS        : pctxt - poiinter to OSCTXT
 *                 p_buff - pointer to ASN.1 encoded message
 *                 p_buff_size - size of result buffer
 *                 p_handover_cancel - pointer to p_handover_cancel
 * OUTPUTS     : p_buff (ASN.1 encoded message),
 *               p_buff_size (size of result buffer)
 * RETURNS     : X2AP_SUCCESS / X2AP_FAILURE    
 * DESCRIPTION   : This function used to encode handover cancel
 *******************************************************************************/
x2ap_return_et x2ap_internal_encode_handover_cancel 
(
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_x2ap_handover_cancel_t  *p_handover_cancel
)
{
    x2ap_return_et           result = X2AP_FAILURE;
    x2ap_X2AP_PDU            x2ap_pdu;
    x2ap_HandoverCancel      *p_x2ap_msg = PNULL;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != pctxt);
    X2AP_ASSERT(PNULL != p_buff);
    X2AP_ASSERT(PNULL != p_buff_size);
    X2AP_ASSERT(PNULL != p_handover_cancel);

    do
    {
        /* Init X2AP PDU */
        asn1Init_x2ap_X2AP_PDU(&x2ap_pdu);
        /* Set Pdu type to Initiating message */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_initiatingMessage;

        x2ap_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, x2ap_InitiatingMessage);
        if (PNULL == x2ap_pdu.u.initiatingMessage)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        asn1Init_x2ap_InitiatingMessage(x2ap_pdu.u.initiatingMessage);

        x2ap_pdu.u.initiatingMessage->procedureCode =
            ASN1V_x2ap_id_handoverCancel;
         
        x2ap_pdu.u.initiatingMessage->criticality = x2ap_ignore;
        
        /* Set the initiating message type to ue context release */
        x2ap_pdu.u.initiatingMessage->value.t = T1x2ap__handoverCancel;

        x2ap_pdu.u.initiatingMessage->value.u.handoverCancel = 
            rtxMemAllocType(pctxt,x2ap_HandoverCancel);
        if (PNULL == x2ap_pdu.u.initiatingMessage->value.u.handoverCancel)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        p_x2ap_msg = x2ap_pdu.u.initiatingMessage->value.u.handoverCancel;
        
        asn1Init_x2ap_HandoverCancel(p_x2ap_msg);
        
        if (RT_OK != asn1Append_x2ap_HandoverCancel_protocolIEs_1(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            p_handover_cancel->x2ap_Old_UE_X2AP_ID))
        {
            break;
        }

        if (
          X2AP_HANDOVER_CANCEL_NEW_UE_X2AP_ID_PRESENT
          &
          p_handover_cancel->bitmask)
        {
            if (RT_OK != asn1Append_x2ap_HandoverCancel_protocolIEs_2(
                pctxt,
                &p_x2ap_msg->protocolIEs,
                p_handover_cancel->x2ap_New_UE_X2AP_ID))
            {
                break;
            }
        }

        if (RT_OK != asn1Append_x2ap_HandoverCancel_protocolIEs_3(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            &p_handover_cancel->cause))
        {
            break;
        }
        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, X2AP_MAX_ASN1_BUF_LEN, ASN_S1AP_BUF_TYPE);
        
        if (RT_OK != asn1PE_x2ap_X2AP_PDU(pctxt, &x2ap_pdu))
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN1 encoding failed.",__FUNCTION__);
            break;
        } 
        else
        {
            
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
        }
        
        *p_buff_size = (U16)pe_GetMsgLen(pctxt);
        
        result = X2AP_SUCCESS;
    } while(0);
    X2AP_UT_TRACE_EXIT();
    return result;
}

/****************************************************************************
 * Function Name  : x2ap_internal_encode_rrc_context
 * Inputs         : pctxt - pointer to OSCTXT
 *                  p_rrc_context - popinter to x2ap_RRC_Context
 *                  p_ho_preparation_information - pointer to 
 *                  HandoverPreparationInformation
 * Outputs        : p_rrc_context
 * Returns        : X2AP_SUCCESS / X2AP_FAILURE
 * Description    : packs HandoverPreparationInformation
 *                  x2ap_RRC_Context
 ****************************************************************************/
rrc_return_et x2ap_internal_encode_rrc_context
(
    OSCTXT            *pctxt, /* for memory allocation */
    x2ap_RRC_Context  *p_rrc_context,

    /* source */
    HandoverPreparationInformation *p_ho_preparation_information
)
{
    OSCTXT           asn1_ctx;
    x2ap_return_et   result = X2AP_FAILURE;
    U8 msg_buf[X2AP_MAX_ASN1_BUF_LEN];
    OSOCTET *p_data = PNULL;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != pctxt);
    X2AP_ASSERT(PNULL != p_rrc_context);
    X2AP_ASSERT(PNULL != p_ho_preparation_information);

    /* Init ASN1 context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        X2AP_TRACE(X2AP_ERROR,
                "[HandoverPreparationInformation]%s:ASN1 context initialization"
                "failed.",__FUNCTION__);
        return X2AP_FAILURE;
    }

    do
    {
        pu_setBuffer(&asn1_ctx, msg_buf,
                X2AP_MAX_ASN1_BUF_LEN, ASN_RRC_BUF_TYPE);

        if (RT_OK != asn1PE_HandoverPreparationInformation(&asn1_ctx,
                    p_ho_preparation_information))
        {
            X2AP_TRACE(X2AP_ERROR,
                    "[HandoverPreparationInformation]%s: ASN1 encoding failed.",
                    __FUNCTION__);
            break;
        }

        p_rrc_context->numocts = pe_GetMsgLen(&asn1_ctx);

        p_data = (U8 *)rtxMemAlloc(pctxt, p_rrc_context->numocts);

        if (PNULL == p_data)
        {
            X2AP_TRACE(X2AP_FATAL,
                    "[HandoverPreparationInformation]%s: Memory allocation failed.",
                    __FUNCTION__);
            break;
        }

        X2AP_MEMCPY(p_data, msg_buf, p_rrc_context->numocts);

        p_rrc_context->data = p_data;
        result = X2AP_SUCCESS;
    }while(0);

    rtFreeContext(&asn1_ctx);

    X2AP_UT_TRACE_EXIT();
    return result;
}

/****************************************************************************
 * Function Name  : x2ap_internal_encode_handover_command
 * Inputs         : pctxt - pointer to OSCTXT
 *                  p_ho_cmd_msg - pointer to HandoverCommand
 *                  p_container - piinter to 
 *                            x2ap_TargeteNBtoSource_eNBTransparentContainer
 * Outputs        : p_container 
 * Returns        : X2AP_SUCCESS / X2AP_FAILURE
 * Description    : Encodes RRC Handover Command message
 ****************************************************************************/
x2ap_return_et x2ap_internal_encode_handover_command
(
    OSCTXT      *pctxt,         /* for memory allocation */
    HandoverCommand* p_ho_cmd_msg, /*Handover Command */
    x2ap_TargeteNBtoSource_eNBTransparentContainer* p_container
)
{
    rrc_return_et result = X2AP_FAILURE;
    U8 msg[X2AP_MAX_ASN1_BUF_LEN];

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != pctxt);
    X2AP_ASSERT(PNULL != p_ho_cmd_msg);

    do
    {
        /* ASN Encode message */
        pu_setBuffer(pctxt, msg, X2AP_MAX_ASN1_BUF_LEN, ASN_RRC_BUF_TYPE);

        if (RT_OK != asn1PE_HandoverCommand(pctxt, p_ho_cmd_msg))
        {
            X2AP_TRACE(X2AP_ERROR,
                "%s: ASN1 encoding for Handover Command failed.",__FUNCTION__);
            break;
        }
    
        p_container->numocts = (U16)pe_GetMsgLen(pctxt);

        p_container->data = (U8 *)rtxMemAlloc(pctxt, p_container->numocts);
        if (PNULL == p_container->data)
        {
            X2AP_TRACE(X2AP_FATAL,
            "[x2ap_internal_encode_handover_command]"
            "ASN Memory allocation"
            "failed.");
            break;
        }

        X2AP_MEMCPY((void*)p_container->data, msg,
                    p_container->numocts);
                
        result = X2AP_SUCCESS;
    } while(0);

    X2AP_UT_TRACE_EXIT();
    return result;
}

/****************************************************************************
 * Function Name  : rrc_x2ap_error_indication_tse_intrl_enc
 * Input          : pctxt - pointer to OSCTXT
 *                  p_buff - pointer to ASN.1 encoded message
                    p_buff_size -  pointer to ASN.1 encoded message lengrh 
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : X2AP_SUCCESS / X2AP_FAILURE
 * Description    : packs p_error_indication into X2AP_PDU ASN
 ****************************************************************************/
x2ap_return_et rrc_x2ap_error_indication_tse_intrl_enc
(
 OSCTXT      *pctxt,         /* for memory allocation */
 U8          *p_buff,        /* ASN.1 encoded message */
 U16         *p_buff_size   /* size of result buffer */
 )
{

    x2ap_X2AP_PDU x2ap_pdu;
    x2ap_return_et result = X2AP_FAILURE;
    x2ap_ErrorIndication *p_x2ap_ErrorIndication = PNULL;
    x2ap_Cause cause;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != pctxt); 
    X2AP_ASSERT(PNULL != p_buff);
    X2AP_ASSERT(PNULL != p_buff_size);

    do
    {
        /* X2PDU Init */
        asn1Init_x2ap_X2AP_PDU(&x2ap_pdu);
        x2ap_pdu.t = T_x2ap_X2AP_PDU_initiatingMessage;

        x2ap_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, x2ap_InitiatingMessage);

        if (PNULL == x2ap_pdu.u.initiatingMessage)
        {       
            X2AP_TRACE(X2AP_ERROR, "ASN malloc failed.");
            break;  
        }

        asn1Init_x2ap_InitiatingMessage(x2ap_pdu.u.initiatingMessage);

        x2ap_pdu.u.initiatingMessage->procedureCode = 
                    ASN1V_x2ap_id_errorIndication;

        x2ap_pdu.u.initiatingMessage->criticality = x2ap_ignore;

        x2ap_pdu.u.initiatingMessage->value.t = T1x2ap__errorIndication;
        x2ap_pdu.u.initiatingMessage->value.u.errorIndication =
            rtxMemAllocType(pctxt, x2ap_ErrorIndication);

        if (PNULL == x2ap_pdu.u.initiatingMessage->value.u.errorIndication)
        {
            X2AP_TRACE(X2AP_ERROR, "ASN malloc failed.");
            break;
        }
        p_x2ap_ErrorIndication = x2ap_pdu.u.initiatingMessage->value.u.errorIndication;

        asn1Init_x2ap_ErrorIndication(p_x2ap_ErrorIndication);

        /* ONLY CAUSE HAS TO BE SENT IN CASE OF TSE */

        /*Fill the cause */

        cause.t = T_x2ap_Cause_protocol;
        cause.u.protocol = x2ap_transfer_syntax_error;

        if (RT_OK != asn1Append_x2ap_ErrorIndication_protocolIEs_3
                (pctxt,
                 &p_x2ap_ErrorIndication->protocolIEs,
                 &cause))
        {
            break;
        }
        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, X2AP_MAX_ASN1_BUF_LEN, ASN_X2AP_BUF_TYPE);

        if (RT_OK != asn1PE_x2ap_X2AP_PDU(pctxt, &x2ap_pdu))
        {
            X2AP_TRACE(X2AP_ERROR,
                    "ASN1 encoding failed.");
            break;
        }
        else
        {
            
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
        }

        *p_buff_size = (U16)pe_GetMsgLen(pctxt);

        result=X2AP_SUCCESS;
    }
    while(0);

    return result;
}

/****************************************************************************
 * Function Name  : rrc_x2ap_error_indication_ase_intrl_enc
 * Inputs         : pctxt - pointer to OSCTXT
 *                  p_buff - pointer to  ASN.1 encoded message
 *                  p_buff_size - pointer to  size of result buffer
 *                  p_error_indication - pointer to rrc_x2ap_error_indication_t
 *                  p_ue_context_release_complete
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : X2AP_SUCCESS / X2AP_FAILURE
 * Description    : packs p_error_indication into x2ap_x2AP_PDU ASN
 ****************************************************************************/
x2ap_return_et rrc_x2ap_error_indication_ase_intrl_enc
(
 OSCTXT      *pctxt,         /* for memory allocation */
 U8          *p_buff,        /* ASN.1 encoded message */
 U16         *p_buff_size,   /* size of result buffer */
 rrc_x2ap_error_indication_t  *p_error_indication
 )
{
    x2ap_return_et result = X2AP_FAILURE;
    x2ap_X2AP_PDU x2ap_pdu;
    x2ap_ErrorIndication *p_x2ap_msg;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != pctxt);
    X2AP_ASSERT(PNULL != p_buff);
    X2AP_ASSERT(PNULL != p_buff_size);
    X2AP_ASSERT(PNULL != p_error_indication);

    do
    {
        asn1Init_x2ap_X2AP_PDU(&x2ap_pdu);
        x2ap_pdu.t = T_x2ap_X2AP_PDU_initiatingMessage;

        x2ap_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, x2ap_InitiatingMessage);
        if (PNULL == x2ap_pdu.u.initiatingMessage)
        {
            X2AP_TRACE(X2AP_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_x2ap_InitiatingMessage(x2ap_pdu.u.initiatingMessage);

        x2ap_pdu.u.initiatingMessage->procedureCode =
           ASN1V_x2ap_id_errorIndication;

        x2ap_pdu.u.initiatingMessage->criticality = x2ap_ignore;
        x2ap_pdu.u.initiatingMessage->value.t = T1x2ap__errorIndication;
        x2ap_pdu.u.initiatingMessage->value.u.errorIndication =
            rtxMemAllocType(pctxt, x2ap_ErrorIndication);
        if (PNULL == x2ap_pdu.u.initiatingMessage->value.u.errorIndication)
        {
            X2AP_TRACE(X2AP_ERROR, "ASN malloc failed.");
            break;
        }
        p_x2ap_msg = x2ap_pdu.u.initiatingMessage->value.u.errorIndication;

        asn1Init_x2ap_ErrorIndication(p_x2ap_msg);

        if (RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT
                &
                p_error_indication->bitmask)
        {
            if (RT_OK != asn1Append_x2ap_ErrorIndication_protocolIEs_1(
                        pctxt,
                        &p_x2ap_msg->protocolIEs,
                        p_error_indication->old_enb_ue_x2ap_id))
            {
                break;
            }
        }

        if (RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT
                &
                p_error_indication->bitmask)
        {
            if (RT_OK != asn1Append_x2ap_ErrorIndication_protocolIEs_2(
                        pctxt,
                        &p_x2ap_msg->protocolIEs,
                        p_error_indication->new_enb_ue_x2ap_id))
            {
                break;
            }
        }

        if (RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT
                &
                p_error_indication->bitmask)
        {
            if (RT_OK != asn1Append_x2ap_ErrorIndication_protocolIEs_3
                    (pctxt,
                     &p_x2ap_msg->protocolIEs,
                     &p_error_indication->cause))
            {
                break;
            }
        }

        if (RRC_X2AP_ERROR_IND_CRITICALITY_DIAGNOSTICS_PRESENT
                &
                p_error_indication->bitmask)
        {
            if (RT_OK != asn1Append_x2ap_ErrorIndication_protocolIEs_4
                    (pctxt,
                     &p_x2ap_msg->protocolIEs,
                     &p_error_indication->criticality_diagnostics))
            {
                break;
            }
        }
#ifdef ENDC_ENABLED
        if (RRC_X2AP_ERROR_IND_OLD_SGNB_UE_X2AP_ID_PRESENT
                &
                p_error_indication->bitmask)
        {
            if (RT_OK != asn1Append_x2ap_ErrorIndication_protocolIEs_7(
                        pctxt,
                        &p_x2ap_msg->protocolIEs,
                        p_error_indication->old_sgnb_ue_x2ap_id))
            {
                break;
            }
        }
#endif
        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, X2AP_MAX_ASN1_BUF_LEN, ASN_X2AP_BUF_TYPE);

        if (RT_OK != asn1PE_x2ap_X2AP_PDU(pctxt, &x2ap_pdu))
        {
            X2AP_TRACE(X2AP_ERROR,
                    "ASN1 encoding failed.");
            break;
        }
        else
        {
           
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
        }

        *p_buff_size = (U16)pe_GetMsgLen(pctxt);

        result=X2AP_SUCCESS;
    } while(0);
    X2AP_UT_TRACE_EXIT();
    return result;
}

/****************************************************************************
 * Function Name  : rrc_uecc_x2ap_error_indication_intrl_enc
 * Inputs         : pctxt - pointer to OSCTXT
 *                  p_buff - pointer to  ASN.1 encoded message
 *                  p_buff_size - pointer to the size of result buffer
 *                  p_error_indication - pointer to rrc_x2ap_error_indication_t
 *                  p_ue_context_release_complete
 * Outputs        : p_buff (ASN.1 encoded message),
 *                  p_buff_size (size of result buffer)
 * Returns        : X2AP_SUCCESS / X2AP_FAILURE
 * Description    : packs p_error_indication into X2AP_PDU ASN
 ****************************************************************************/
x2ap_return_et rrc_uecc_x2ap_error_indication_intrl_enc
(
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_x2ap_error_indication_t  *p_error_indication
)
{
    x2ap_return_et result = X2AP_FAILURE;
    x2ap_X2AP_PDU x2ap_pdu; 
    x2ap_ErrorIndication *p_x2ap_msg = PNULL;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != pctxt);
    X2AP_ASSERT(PNULL != p_buff);
    X2AP_ASSERT(PNULL != p_buff_size);
    X2AP_ASSERT(PNULL != p_error_indication);

    do
    {
        asn1Init_x2ap_X2AP_PDU(&x2ap_pdu);
        x2ap_pdu.t = T_x2ap_X2AP_PDU_initiatingMessage;

        x2ap_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, x2ap_InitiatingMessage);
        if (PNULL == x2ap_pdu.u.initiatingMessage)
        {
            X2AP_TRACE(X2AP_ERROR, "ASN malloc failed.");
            break;
        }

        asn1Init_x2ap_InitiatingMessage(x2ap_pdu.u.initiatingMessage);

        x2ap_pdu.u.initiatingMessage->procedureCode = 
                ASN1V_x2ap_id_errorIndication;

        x2ap_pdu.u.initiatingMessage->criticality = x2ap_ignore;

        x2ap_pdu.u.initiatingMessage->value.t = T1x2ap__errorIndication;
        x2ap_pdu.u.initiatingMessage->value.u.errorIndication =
            rtxMemAllocType(pctxt, x2ap_ErrorIndication);
        if (PNULL == x2ap_pdu.u.initiatingMessage->value.u.errorIndication)
        {
            X2AP_TRACE(X2AP_ERROR, "ASN malloc failed.");
            break;
        }
        
        p_x2ap_msg = x2ap_pdu.u.initiatingMessage->value.u.errorIndication;
        
        asn1Init_x2ap_ErrorIndication(p_x2ap_msg);


        if (RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT
            &
            p_error_indication->bitmask)
        {
            if (RT_OK != asn1Append_x2ap_ErrorIndication_protocolIEs_1(
                pctxt,
                &p_x2ap_msg->protocolIEs,
                p_error_indication->old_enb_ue_x2ap_id))
            {
                break;
            }
        }

        if (RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT
            &
            p_error_indication->bitmask)
        {
            if (RT_OK != asn1Append_x2ap_ErrorIndication_protocolIEs_2(
                pctxt,
                &p_x2ap_msg->protocolIEs,
                p_error_indication->new_enb_ue_x2ap_id))
            {
                break;
            }
        }

        if (RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT
            &
            p_error_indication->bitmask)
        {
            if (RT_OK != asn1Append_x2ap_ErrorIndication_protocolIEs_3
                (pctxt,
                &p_x2ap_msg->protocolIEs,
                &p_error_indication->cause))
            {
                break;
            }
        }

        if (RRC_X2AP_ERROR_IND_CRITICALITY_DIAGNOSTICS_PRESENT
            &
            p_error_indication->bitmask)
        {
            if (RT_OK != asn1Append_x2ap_ErrorIndication_protocolIEs_4
                (pctxt,
                &p_x2ap_msg->protocolIEs,
                &p_error_indication->criticality_diagnostics))
            {
                break;
            }
        }

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, X2AP_MAX_ASN1_BUF_LEN, ASN_X2AP_BUF_TYPE);

        if (RT_OK != asn1PE_x2ap_X2AP_PDU(pctxt, &x2ap_pdu))
        {
            X2AP_TRACE(X2AP_ERROR,
                "X2AP_ERROR_INDICATION : ASN1 encoding failed.");
            break;
        }
        else
        {
            X2AP_TRACE(X2AP_ERROR,
                "X2AP_ERROR_INDICATION : ASN1 encoding Success.");
            
           
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
        }
        *p_buff_size = (U16)pe_GetMsgLen(pctxt);

        result=X2AP_SUCCESS;
    } while(0);

    X2AP_UT_TRACE_EXIT();
    return result;
}


/* 
 *  The encoding decoding functions below can be used in case ASN Enc /Dec
 *  Library is to be prepared seperately
 */
   
/******************************************************************************
 * Function Name  : x2ap_compose_served_cell_new
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context 
 *                  p_asn1_ctx - Pointer to OSCTXT.
 *                  p_x2ap_IE_serv_cell - pointer to OSRTDList
 *                  p_serv_cell - pointer to x2ap_served_cell_info_arr_t
 *                  num_served_cell - num of served cells
 * Outputs        : p_x2ap_IE_serv_cell - This structure is populated with
 *                                        information from p_serv_cell.
 * Returns        : X2AP_SUCCESS - Asn Encoding was successful.
 *                  X2AP_FAILURE - Asn Encoding was not successful.
 * Description    : This function populates p_x2ap_IE_serv_cell with
 *                  information necessary to encode served cell Info.
 *****************************************************************************/
x2ap_return_et x2ap_compose_served_cell_new(
							x2ap_gb_context_t* p_x2ap_gb_ctx,
							 OSCTXT                      *p_asn1_ctx,
                             OSRTDList                   *p_x2ap_IE_serv_cell,
                             x2ap_served_cell_info_arr_t      *p_serv_cell,
                             U16                          num_served_cell)
{

    OSRTDListNode               *p_node = X2AP_P_NULL;
    OSRTDListNode               *p_node_serv_cell = NULL;
    OSRTDList                   *p_neighbour_cell_info = X2AP_P_NULL;
    x2ap_ServedCells_element    *p_servedCells_elem = X2AP_P_NULL;
    x2ap_Neighbour_Information_element *p_neigh_cell_info = X2AP_P_NULL;
    U16                         cell_count = 0;
    U16                         neighbour_cell_count = 0;

    X2AP_UT_TRACE_ENTER();

    if (!num_served_cell)
    {
        X2AP_TRACE(X2AP_ERROR, "%s:Num of supported served cell.",
                   __FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }
    for (cell_count = 0; cell_count < num_served_cell; cell_count++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx,
                                 x2ap_ServedCells_element,
                                 &p_node_serv_cell,
                                 &p_servedCells_elem);
        if (X2AP_P_NULL == p_node_serv_cell)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }
        asn1Init_x2ap_ServedCells_element(p_servedCells_elem);

        if (p_serv_cell->nbour_cell_info.num_neighbour_cell)
        {
            p_servedCells_elem->m.neighbour_InfoPresent = 1;
            p_neighbour_cell_info = &p_servedCells_elem->neighbour_Info;
            rtxDListInit(p_neighbour_cell_info);
        }
        /*** compose served cell Info start ***/
        if (X2AP_FAILURE == x2ap_compose_serv_cell_info_new(p_x2ap_gb_ctx,
        						p_asn1_ctx,
                                &p_servedCells_elem->servedCellInfo,
                                                        &p_serv_cell->cell_info))
        {
            X2AP_TRACE(X2AP_ERROR,
                       "%s:compose_serv_cell_info returned Failure.",
                       __FUNCTION__);
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }
        /*** compose served cell info end ***/

        /*** compose neghbour cell info start ***/
        for (neighbour_cell_count = 0;
                  neighbour_cell_count <
                           p_serv_cell->nbour_cell_info.num_neighbour_cell;
                                                       neighbour_cell_count++)
        {
            rtxDListAllocNodeAndData(p_asn1_ctx,
                                     x2ap_Neighbour_Information_element,
                                     &p_node,
                                     &p_neigh_cell_info);
            if (X2AP_P_NULL == p_node)
            {
                X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
                X2AP_UT_TRACE_EXIT();
                return X2AP_FAILURE;
            }
            asn1Init_x2ap_Neighbour_Information_element(p_neigh_cell_info);

            /* Encode ECGI */
            p_neigh_cell_info->eCGI.pLMN_Identity.numocts = MAX_PLMN_ID_BYTES;
            X2AP_MEMCPY(p_neigh_cell_info->eCGI.pLMN_Identity.data,
                        p_serv_cell->nbour_cell_info.neighbour_info_arr[
                         neighbour_cell_count].ecgi.plmn_identity.plmn_id,
                        MAX_PLMN_ID_BYTES);

            p_neigh_cell_info->eCGI.eUTRANcellIdentifier.numbits =
                                             X2AP_EUTRAN_CELL_ID_BIT_STR_LEN;
            X2AP_MEMCPY(p_neigh_cell_info->eCGI.eUTRANcellIdentifier.data,
                        &(p_serv_cell->nbour_cell_info.neighbour_info_arr[
                         neighbour_cell_count].ecgi.eutran_cell_id),
                        sizeof(U32));

            /* Encode PCI */
            p_neigh_cell_info->pCI = p_serv_cell->nbour_cell_info.
                         neighbour_info_arr[neighbour_cell_count].pci;

            /* Encode ERFCN */
            p_neigh_cell_info->eARFCN = (x2ap_EARFCN)(p_serv_cell->
                    nbour_cell_info.neighbour_info_arr[neighbour_cell_count].
                    earfcn.earfcn);

            rtxDListAppendNode (p_neighbour_cell_info, p_node);
        }
        /*** compose neghbour cell info end ***/
        rtxDListAppendNode(p_x2ap_IE_serv_cell, p_node_serv_cell);
        
        p_serv_cell++;
    }
    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;
}

/*****************************************************************************
 * Function Name  : x2ap_compose_num_atenna_info 
 * Inputs         : p_asn1_ctx - pointer to OSCTXT
 *                  p_served_cell_extn - pointer to 
 *                                   x2ap_ServedCell_Information_iE_Extensions
 *                  p_serv_cell - pointer to x2ap_served_cell_info_arr_t
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : this function composes numbers of atenna info
 ********************************************************************************/
/* SPR 8178 Start */
x2ap_return_et x2ap_compose_num_atenna_info(OSCTXT* p_asn1_ctx, x2ap_ServedCell_Information_iE_Extensions* p_served_cell_extn, x2ap_srvd_cell_arry_t* p_serv_cell)
/* SPR 8178 End */ 
{
	OSRTDListNode* p_served_cell_node = X2AP_P_NULL;
	x2ap_ServedCell_Information_iE_Extensions_element *p_served_cell_extn_elem = X2AP_P_NULL;

	rtxDListAllocNodeAndData(p_asn1_ctx,
			x2ap_ServedCell_Information_iE_Extensions_element,
		 &p_served_cell_node,
		 &p_served_cell_extn_elem);

	 if (X2AP_P_NULL == p_served_cell_node)
	 {
		 return X2AP_FAILURE;
	 }

	 asn1Init_x2ap_ServedCell_Information_iE_Extensions_element(p_served_cell_extn_elem);
	 p_served_cell_extn_elem->id = ASN1V_x2ap_id_Number_of_Antennaports;
	 p_served_cell_extn_elem->criticality = x2ap_ignore;
	 p_served_cell_extn_elem->extensionValue.t = T341x2ap___x2ap_ServedCell_Information_ExtIEs_1;
/* SPR 8178 Start */
	 p_served_cell_extn_elem->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_1 = p_serv_cell->served_cell_info.num_antenna_port;
/* SPR 8178 End */

	if (0 == rtxDListAppend(p_asn1_ctx,p_served_cell_extn, (void*)p_served_cell_extn_elem))
	{
		 return (x2ap_return_et)LOG_RTERR(p_asn1_ctx, RTERR_NOMEM);
	}
	return X2AP_SUCCESS;
}

/* SPR 8178 Start */ 
/*****************************************************************************
 * Function Name  : x2ap_compose_num_atenna_info 
 * Inputs         : p_asn1_ctx - pointer to OSCTXT
 *                  p_served_cell_extn - pointer to 
 *                                   x2ap_ServedCell_Information_iE_Extensions
 *                  p_serv_cell - pointer to x2ap_served_cell_info_t
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : this function composes numbers of atenna info
 ********************************************************************************/
x2ap_return_et x2ap_compose_num_atenna_info_new
(
    OSCTXT* p_asn1_ctx, 
    x2ap_ServedCell_Information_iE_Extensions* p_served_cell_extn, 
    x2ap_served_cell_info_t* p_serv_cell
)
{
	OSRTDListNode* p_served_cell_node = X2AP_P_NULL;
	x2ap_ServedCell_Information_iE_Extensions_element *p_served_cell_extn_elem = X2AP_P_NULL;

	rtxDListAllocNodeAndData(p_asn1_ctx,
			x2ap_ServedCell_Information_iE_Extensions_element,
		 &p_served_cell_node,
		 &p_served_cell_extn_elem);

	 if (X2AP_P_NULL == p_served_cell_node)
	 {
		 return X2AP_FAILURE;
	 }

	 asn1Init_x2ap_ServedCell_Information_iE_Extensions_element(p_served_cell_extn_elem);
	 p_served_cell_extn_elem->id = ASN1V_x2ap_id_Number_of_Antennaports;
	 p_served_cell_extn_elem->criticality = x2ap_ignore;
	 p_served_cell_extn_elem->extensionValue.t = T341x2ap___x2ap_ServedCell_Information_ExtIEs_1;
	 p_served_cell_extn_elem->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_1 = p_serv_cell->num_antenna_port;

	if (0 == rtxDListAppend(p_asn1_ctx,p_served_cell_extn, (void*)p_served_cell_extn_elem))
	{
		 return (x2ap_return_et)LOG_RTERR(p_asn1_ctx, RTERR_NOMEM);
	}
	return X2AP_SUCCESS;
}
/* SPR 8178 End */
 
/*****************************************************************************
 * Function Name  : x2ap_compose_prach_config_info 
 * Inputs         : p_asn1_ctx - pointer to OSCTXT
 *                  p_served_cell_extn - pointer to 
 *                                     x2ap_ServedCell_Information_iE_Extensions
 *                  p_prach_cfg - pointer to x2ap_prach_configuration_info_t
 * Outputs        : None 
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : this function composes prach config info
 ********************************************************************************/

x2ap_return_et x2ap_compose_prach_config_info(OSCTXT* p_asn1_ctx, x2ap_ServedCell_Information_iE_Extensions* p_served_cell_extn, 
           x2ap_prach_configuration_info_t * p_prach_cfg)
{
	OSRTDListNode* p_served_cell_node = X2AP_P_NULL;
	x2ap_ServedCell_Information_iE_Extensions_element *p_served_cell_extn_elem1 = X2AP_P_NULL;
	x2ap_PRACH_Configuration* p_prach_config = X2AP_P_NULL;

	rtxDListAllocNodeAndData(p_asn1_ctx,
			x2ap_ServedCell_Information_iE_Extensions_element,
		 &p_served_cell_node,
		 &p_served_cell_extn_elem1);

	 if (X2AP_P_NULL == p_served_cell_node)
	 {
		 return X2AP_FAILURE;
	 }

	 asn1Init_x2ap_ServedCell_Information_iE_Extensions_element(p_served_cell_extn_elem1);
	 p_served_cell_extn_elem1->id = ASN1V_x2ap_id_PRACH_Configuration;
	 p_served_cell_extn_elem1->criticality = x2ap_ignore;
	 p_served_cell_extn_elem1->extensionValue.t = T341x2ap___x2ap_ServedCell_Information_ExtIEs_2;

	 p_prach_config = p_served_cell_extn_elem1->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_2;
	 p_prach_config = rtxMemAllocType(p_asn1_ctx,x2ap_PRACH_Configuration);
	 
     if(p_prach_config ==X2AP_P_NULL)
	 {
		 X2AP_TRACE(X2AP_ERROR,"Can't allocate p_prach_config");
		 rtxDListFreeAll(p_asn1_ctx, p_served_cell_extn);
		 X2AP_UT_TRACE_EXIT();
		 return X2AP_FAILURE;
	 }
	 else
	 {
         asn1Init_x2ap_PRACH_Configuration(p_prach_config);
		 p_served_cell_extn_elem1->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_2 = p_prach_config;
	 }
	 p_prach_config->rootSequenceIndex = p_prach_cfg->root_sequence_index;
	 p_prach_config->zeroCorrelationIndex = p_prach_cfg->zero_correlation_zone_config;
	 p_prach_config-> highSpeedFlag= p_prach_cfg->high_speed_flag;
	 p_prach_config-> prach_FreqOffset= p_prach_cfg->prach_frequency_offset;

	 if(p_prach_cfg->bitmask & X2AP_PRACH_CONFIG_INDEX_PRESENT)
	 {
		 p_prach_config->m.prach_ConfigIndexPresent =1;
		 p_prach_config->prach_ConfigIndex= p_prach_cfg->prach_config_index;
	 }

	if (0 == rtxDListAppend(p_asn1_ctx,p_served_cell_extn, (void*)p_served_cell_extn_elem1))
	{
		 return (x2ap_return_et)LOG_RTERR(p_asn1_ctx, RTERR_NOMEM);
	}
	return X2AP_SUCCESS;
}

/*****************************************************************************
 * Function Name  : x2ap_compose_mbsfn_info 
 * Inputs         : p_asn1_ctx - pointer to OSCTXT
 *                  p_served_cell_extn - pointer to 
 *                                    x2ap_ServedCell_Information_iE_Extensions
 *                  p_serv_cell - pointer to x2ap_served_cell_info_t
 * Outputs        : None
 * Returns        : X2AP_FAILURE/X2AP_SUCCESS 
 * Description    : this function composes mbsfn info
 ********************************************************************************/
x2ap_return_et x2ap_compose_mbsfn_info
(
    OSCTXT* p_asn1_ctx, 
    x2ap_ServedCell_Information_iE_Extensions* p_served_cell_extn, 
    x2ap_served_cell_info_t* p_serv_cell
)
{
	OSRTDListNode* p_served_cell_node = X2AP_P_NULL;
	OSRTDListNode* p_served_cell_node1 = X2AP_P_NULL;
	x2ap_ServedCell_Information_iE_Extensions_element *p_served_cell_extn_elem2 = X2AP_P_NULL;
    /*SPR 18605 FIX START*/
    /* code Removed */
    /*SPR 18605 FIX STOP*/
	x2ap_MBSFN_Subframe_Info* p_mbsfn_subframe_info = X2AP_P_NULL;
	U8 mbsfn_info_count = 0;

	rtxDListAllocNodeAndData(p_asn1_ctx,
			x2ap_ServedCell_Information_iE_Extensions_element,
		 &p_served_cell_node,
		 &p_served_cell_extn_elem2);

	 if (X2AP_P_NULL == p_served_cell_node)
	 {
		 X2AP_TRACE(X2AP_WARNING, "Memory allocation failed");
		 return X2AP_FAILURE;
	 }

	 asn1Init_x2ap_ServedCell_Information_iE_Extensions_element(p_served_cell_extn_elem2);
	 p_served_cell_extn_elem2->id = ASN1V_x2ap_id_MBSFN_Subframe_Info;
	 p_served_cell_extn_elem2->criticality = x2ap_ignore;
	 p_served_cell_extn_elem2->extensionValue.t = T341x2ap___x2ap_ServedCell_Information_ExtIEs_3;
     
    /*SPR 18605 FIX START*/
    p_served_cell_extn_elem2->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_3 =
                rtxMemAllocType(p_asn1_ctx,x2ap_MBSFN_Subframe_Infolist);
     
    rtxDListInit(p_served_cell_extn_elem2->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_3);
    /*SPR 18605 FIX STOP*/

	 for(mbsfn_info_count=0; mbsfn_info_count < p_serv_cell->mbsfn_subframe_info.num_mbsfn; mbsfn_info_count++)
	 {
		 rtxDListAllocNodeAndData(p_asn1_ctx,
			 x2ap_MBSFN_Subframe_Info,
			 &p_served_cell_node1,
			 &p_mbsfn_subframe_info);

		 if (X2AP_P_NULL == p_served_cell_node1)
		 {
			 X2AP_TRACE(X2AP_WARNING, "Memory allocation failed");
			 return X2AP_FAILURE;
		 }

		 asn1Init_x2ap_MBSFN_Subframe_Info (p_mbsfn_subframe_info);
		 p_mbsfn_subframe_info->radioframeAllocationOffset =
				 p_serv_cell->mbsfn_subframe_info.mbsfn_info[mbsfn_info_count].radioframe_alloc_offset;
		 p_mbsfn_subframe_info->radioframeAllocationPeriod =
				 p_serv_cell->mbsfn_subframe_info.mbsfn_info[mbsfn_info_count].radioframe_alloc_period;

		 if(p_serv_cell->mbsfn_subframe_info.mbsfn_info[mbsfn_info_count].subframe_alloc.bitmask
				 & X2AP_SUBFRAME_ALLOC_ONE_FRAME_INFO_PRESENT)
		 {
			 p_mbsfn_subframe_info->subframeAllocation.t = T_MBSFN_SubframeConfig_subframeAllocation_oneFrame;

			 p_mbsfn_subframe_info->subframeAllocation.u.oneframe = rtxMemAllocType(p_asn1_ctx, x2ap_Oneframe);
			 if (X2AP_P_NULL == p_mbsfn_subframe_info->subframeAllocation.u.oneframe)
			 {
				 X2AP_TRACE(X2AP_WARNING, "Memory allocation failed");
				 return X2AP_FAILURE;
			 }

			 asn1Init_x2ap_Oneframe(p_mbsfn_subframe_info->subframeAllocation.u.oneframe);
             p_mbsfn_subframe_info->subframeAllocation.u.oneframe->numbits = X2AP_SUBFRAME_ALLOC_ONE_FRAME_NUMBITS;
			 X2AP_MEMCPY(p_mbsfn_subframe_info->subframeAllocation.u.oneframe->data,
					 p_serv_cell->mbsfn_subframe_info.mbsfn_info[mbsfn_info_count].subframe_alloc.oneframe,
					 X2AP_SUBFRAME_ALLOC_ONE_FRAME_OCTET_SIZE);
		 }
		 else if(p_serv_cell->mbsfn_subframe_info.mbsfn_info[mbsfn_info_count].subframe_alloc.bitmask
				 & X2AP_SUBFRAME_ALLOC_FOUR_FRAME_INFO_PRESENT)
		 {
			 p_mbsfn_subframe_info->subframeAllocation.t = T_MBSFN_SubframeConfig_subframeAllocation_fourFrames;;

			 p_mbsfn_subframe_info->subframeAllocation.u.fourframes = rtxMemAllocType(p_asn1_ctx, x2ap_Fourframes);
			 if (X2AP_P_NULL == p_mbsfn_subframe_info->subframeAllocation.u.fourframes)
			 {
				 X2AP_TRACE(X2AP_WARNING, "Memory allocation failed");
				 return X2AP_FAILURE;
			 }

			 asn1Init_x2ap_Fourframes(p_mbsfn_subframe_info->subframeAllocation.u.fourframes);
			 p_mbsfn_subframe_info->subframeAllocation.u.fourframes->numbits = X2AP_SUBFRAME_ALLOC_FOUR_FRAME_NUMBITS;
			 X2AP_MEMCPY(p_mbsfn_subframe_info->subframeAllocation.u.fourframes->data,
					 p_serv_cell->mbsfn_subframe_info.mbsfn_info[mbsfn_info_count].subframe_alloc.fourframe,
					 X2AP_SUBFRAME_ALLOC_FOUR_FRAME_OCTET_SIZE);
		 }

    /*SPR 18605 FIX START*/
		 if (0 == rtxDListAppendNode(p_served_cell_extn_elem2->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_3, 
                        p_served_cell_node1))
    /*SPR 18605 FIX STOP*/
		{
			 return (x2ap_return_et)LOG_RTERR(p_asn1_ctx, RTERR_NOMEM);
		}
	 }

	 if (0 == rtxDListAppendNode(p_served_cell_extn, p_served_cell_node))
	{
		 return (x2ap_return_et)LOG_RTERR(p_asn1_ctx, RTERR_NOMEM);
	}
	 return X2AP_SUCCESS;
}

/******************************************************************************
 * Function Name  : x2ap_compose_csg_id_info
 * Inputs         : p_asn1_ctx - Pointer to OSCTXT.
 *                  p_serv_cell - This contains the information of served cell.
 *
 * Outputs        : p_served_cell_extn - This structure is populated with
 *                                        information from p_serv_cell.
 * Returns        : X2AP_SUCCESS - Asn Encoding was successful.
 *                  X2AP_FAILURE - Asn Encoding was not successful.
 * Description    : This function populates p_served_cell_extn with
 *                  information necessary to encode csg id info.
 ******************************************************************************/
/*36423_CR0407_start*/
x2ap_return_et x2ap_compose_csg_id_info(OSCTXT* p_asn1_ctx, x2ap_ServedCell_Information_iE_Extensions* p_served_cell_extn, x2ap_srvd_cell_arry_t* p_serv_cell)
{
	OSRTDListNode* p_served_cell_node = X2AP_P_NULL;
	x2ap_ServedCell_Information_iE_Extensions_element *p_served_cell_extn_elem3 = X2AP_P_NULL;
	x2ap_CSG_Id * p_csg_id = X2AP_P_NULL;

	rtxDListAllocNodeAndData(p_asn1_ctx,
			x2ap_ServedCell_Information_iE_Extensions_element,
		 &p_served_cell_node,
		 &p_served_cell_extn_elem3);

	 if (X2AP_P_NULL == p_served_cell_node)
	 {
		 return X2AP_FAILURE;
	 }

	 asn1Init_x2ap_ServedCell_Information_iE_Extensions_element(p_served_cell_extn_elem3);
	 p_served_cell_extn_elem3->id = ASN1V_x2ap_id_CSG_Id;
	 p_served_cell_extn_elem3->criticality = x2ap_ignore;
	 p_served_cell_extn_elem3->extensionValue.t = T341x2ap___x2ap_ServedCell_Information_ExtIEs_4;

	 p_csg_id = p_served_cell_extn_elem3->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_4;
	 p_csg_id = rtxMemAllocType(p_asn1_ctx,x2ap_CSG_Id);
     if(p_csg_id ==X2AP_P_NULL)
	 {
		 X2AP_TRACE(X2AP_ERROR,"Can't allocate p_csg_id");
		 X2AP_UT_TRACE_EXIT();
		 return X2AP_FAILURE;
	 }
	 else
	 {
		 p_served_cell_extn_elem3->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_4 = p_csg_id;
	 }

	 p_csg_id->numbits = CSG_ID_NBITS;
         l3_memcpy_wrapper( p_csg_id->data,
                p_serv_cell->served_cell_info.csg_identity,
                sizeof(p_csg_id->data));

	if (0 == rtxDListAppend(p_asn1_ctx,p_served_cell_extn, (void*)p_served_cell_extn_elem3))
	{
		 return (x2ap_return_et)LOG_RTERR(p_asn1_ctx, RTERR_NOMEM);
	}
	return X2AP_SUCCESS;
}
/*36423_CR0407_stop*/

/* SES-460 changes start */
/******************************************************************************
 * Function Name  : x2ap_compose_csg_id_info_new
 * Inputs         : p_asn1_ctx - Pointer to OSCTXT.
 *                  p_serv_cell - This contains the information of served cell.
 *
 * Outputs        : p_served_cell_extn - This structure is populated with
 *                                        information from p_serv_cell.
 * Returns        : X2AP_SUCCESS - Asn Encoding was successful.
 *                  X2AP_FAILURE - Asn Encoding was not successful.
 * Description    : This function populates p_served_cell_extn with
 *                  information necessary to encode csg id info.
 ******************************************************************************/
x2ap_return_et x2ap_compose_csg_id_info_new(
    OSCTXT                                      *p_asn1_ctx,
    x2ap_ServedCell_Information_iE_Extensions   *p_served_cell_extn,
    x2ap_served_cell_info_t                 *p_serv_cell)
{
    OSRTDListNode                                       *p_served_cell_node = X2AP_P_NULL;
    x2ap_ServedCell_Information_iE_Extensions_element   *p_served_cell_extn_elem3 = X2AP_P_NULL;
    x2ap_CSG_Id                                         *p_csg_id = X2AP_P_NULL;

    rtxDListAllocNodeAndData(p_asn1_ctx,
        x2ap_ServedCell_Information_iE_Extensions_element,
        &p_served_cell_node,
        &p_served_cell_extn_elem3);
    if (X2AP_P_NULL == p_served_cell_node)
    {
        X2AP_TRACE(X2AP_ERROR,"Can't allocate p_served_cell_node");
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    asn1Init_x2ap_ServedCell_Information_iE_Extensions_element(p_served_cell_extn_elem3);
    p_served_cell_extn_elem3->id = ASN1V_x2ap_id_CSG_Id;
    p_served_cell_extn_elem3->criticality = x2ap_ignore;
    p_served_cell_extn_elem3->extensionValue.t = T341x2ap___x2ap_ServedCell_Information_ExtIEs_4;

    p_csg_id = p_served_cell_extn_elem3->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_4;
    p_csg_id = rtxMemAllocType(p_asn1_ctx,x2ap_CSG_Id);
    if(p_csg_id ==X2AP_P_NULL)
    {
        X2AP_TRACE(X2AP_ERROR,"Can't allocate p_csg_id");
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }
    else
    {
        p_served_cell_extn_elem3->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_4 = p_csg_id;
    }

    p_csg_id->numbits = CSG_ID_NBITS;
    l3_memcpy_wrapper(p_csg_id->data, p_serv_cell->csg_identity,
        sizeof(p_csg_id->data));

    if (0 == rtxDListAppend(p_asn1_ctx,p_served_cell_extn, (void*)p_served_cell_extn_elem3))
    {
        return (x2ap_return_et)LOG_RTERR(p_asn1_ctx, RTERR_NOMEM);
    }
    return X2AP_SUCCESS;
}
/* SES-460 changes end */

/******************************************************************************
 * Function Name  : x2ap_compose_serv_cell_info_new
 * Inputs         :p_x2ap_gb_ctx - pointer to X2AP global context 
 *                  p_asn1_ctx - Pointer to OSCTXT.
 *                  p_x2ap_IE_serv_cell - pointer to x2ap_ServedCell_Information
 *                  p_serv_cell - This contains the information of served cell.
 *
 * Outputs        : p_x2ap_IE_serv_cell - This structure is populated with
 *                                        information from p_serv_cell.
 * Returns        : X2AP_SUCCESS - Asn Encoding was successful.
 *                  X2AP_FAILURE - Asn Encoding was not successful.
 * Description    : This function populates p_x2ap_IE_serv_cell with
 *                  information necessary to encode served cell info.
 ******************************************************************************/
x2ap_return_et x2ap_compose_serv_cell_info_new(
    x2ap_gb_context_t           *p_x2ap_gb_ctx,
    OSCTXT                      *p_asn1_ctx,
    x2ap_ServedCell_Information *p_x2ap_IE_serv_cell,
    x2ap_served_cell_info_t *p_serv_cell)
{
    x2ap_return_et  response = X2AP_SUCCESS;
    U16             brodcast_plmn_count = 0;
    x2ap_ecgi_t     ecgi;
    X2AP_UT_TRACE_ENTER();

    /* Encode PCI */
    p_x2ap_IE_serv_cell->pCI = p_serv_cell->pci;

    /* Encode ECGI = PLMN_ID + CELL_ID */
    p_x2ap_IE_serv_cell->cellId.pLMN_Identity.numocts = MAX_PLMN_ID_BYTES;
    X2AP_MEMCPY(p_x2ap_IE_serv_cell->cellId.pLMN_Identity.data,
        /*SPR_19279_START*/
        p_serv_cell->bcast_plmn_info.plmn_identity[0].plmn_id,
        /*SPR_19279_END*/
        MAX_PLMN_ID_BYTES);

    p_x2ap_IE_serv_cell->cellId.eUTRANcellIdentifier.numbits =
        X2AP_EUTRAN_CELL_ID_BIT_STR_LEN;
    /*SPR 21554 Start*/
    x2ap_populate_ecgi_from_cell_id(p_x2ap_gb_ctx, p_serv_cell->cell_id, &ecgi);
    /*SPR 21554 End*/
    X2AP_MEMCPY(p_x2ap_IE_serv_cell->cellId.eUTRANcellIdentifier.data,
        &(ecgi.eutran_cell_id), sizeof(U32));

    /* Encode TAC */
    p_x2ap_IE_serv_cell->tAC.numocts = MAX_TAC_BYTES;
    X2AP_MEMCPY(p_x2ap_IE_serv_cell->tAC.data,
        p_serv_cell->tac,
        MAX_TAC_BYTES);

    /* Encode Broadcast PLMN*/
    p_x2ap_IE_serv_cell->broadcastPLMNs.n =
        p_serv_cell->bcast_plmn_info.num_bcast_plmn_id;
    for (brodcast_plmn_count = 0;
        brodcast_plmn_count < p_x2ap_IE_serv_cell->broadcastPLMNs.n;
        brodcast_plmn_count++)
    {
        p_x2ap_IE_serv_cell->broadcastPLMNs.elem[brodcast_plmn_count].numocts =
            MAX_PLMN_ID_BYTES;
        X2AP_MEMCPY(p_x2ap_IE_serv_cell->broadcastPLMNs.elem[brodcast_plmn_count].data,
            p_serv_cell->bcast_plmn_info.plmn_identity[brodcast_plmn_count].plmn_id,
            MAX_PLMN_ID_BYTES);
    }
    /* Encode eUTRA Mode */
    if (X2AP_EUTRA_MODE_FDD == p_serv_cell->choice_eutra_mode.choice_eutra_mode.bitmask)
    {
        p_x2ap_IE_serv_cell->eUTRA_Mode_Info.t = T_x2ap_EUTRA_Mode_Info_fDD;

        p_x2ap_IE_serv_cell->eUTRA_Mode_Info.u.fDD =
            rtxMemAllocType(p_asn1_ctx, x2ap_FDD_Info);
        if (X2AP_P_NULL == p_x2ap_IE_serv_cell->eUTRA_Mode_Info.u.fDD)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            return X2AP_FAILURE;
        }

        asn1Init_x2ap_FDD_Info(p_x2ap_IE_serv_cell->eUTRA_Mode_Info.u.fDD);
        /*** Compose FDD Info ***/
        compose_x2ap_FDD_Info (
            p_x2ap_IE_serv_cell->eUTRA_Mode_Info.u.fDD,
            &p_serv_cell->choice_eutra_mode);
    }
    else if (X2AP_EUTRA_MODE_TDD == p_serv_cell->choice_eutra_mode.choice_eutra_mode.bitmask)
    {
        p_x2ap_IE_serv_cell->eUTRA_Mode_Info.t = T_x2ap_EUTRA_Mode_Info_tDD;

        p_x2ap_IE_serv_cell->eUTRA_Mode_Info.u.tDD =
            rtxMemAllocType(p_asn1_ctx, x2ap_TDD_Info);
        if (X2AP_P_NULL == p_x2ap_IE_serv_cell->eUTRA_Mode_Info.u.tDD)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            return X2AP_FAILURE;
        }

        asn1Init_x2ap_TDD_Info(p_x2ap_IE_serv_cell->eUTRA_Mode_Info.u.tDD);
        /*** Compose TDD Info ***/
        compose_x2ap_TDD_Info (
            p_x2ap_IE_serv_cell->eUTRA_Mode_Info.u.tDD,
            &p_serv_cell->choice_eutra_mode);
    }
    else
    {
        /* eNB ID presence bitmask is not set */
        X2AP_TRACE(X2AP_ERROR,
            "%s: neither FDD nor TDD mode Presence bitmask is set", __FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    if(p_serv_cell->bitmask & X2AP_NUM_ANTENNA_INFO_PORT_PRESENT ||
        p_serv_cell->bitmask & X2AP_PRACH_CONFIG_INFO_PRESENT ||
        p_serv_cell->bitmask & X2AP_MBSFN_SUBFRAME_INFO_PRESENT ||
        p_serv_cell->bitmask & X2AP_CSG_ID_INFO_PRESENT)
    {
        x2ap_ServedCell_Information_iE_Extensions* p_served_cell_extn = X2AP_P_NULL;

        p_served_cell_extn = &p_x2ap_IE_serv_cell->iE_Extensions;
        p_x2ap_IE_serv_cell->m.iE_ExtensionsPresent = 1;
        asn1Init_x2ap_ServedCell_Information_iE_Extensions(p_served_cell_extn);

        if(p_serv_cell->bitmask & X2AP_NUM_ANTENNA_INFO_PORT_PRESENT)
        {
            /* SPR 8178 Start */
            response = x2ap_compose_num_atenna_info_new (p_asn1_ctx, p_served_cell_extn, p_serv_cell);
            /* SPR 8178 End */
        }
        if(p_serv_cell->bitmask & X2AP_PRACH_CONFIG_INFO_PRESENT)
        {
            response = x2ap_compose_prach_config_info(p_asn1_ctx, p_served_cell_extn,
                    &p_serv_cell->prach_config);
        }
        if(p_serv_cell->bitmask & X2AP_MBSFN_SUBFRAME_INFO_PRESENT)
        {
            /*SPR 18605 FIX START*/
            response = x2ap_compose_mbsfn_info(p_asn1_ctx,p_served_cell_extn, p_serv_cell);
            /*SPR 18605 FIX STOP*/
        }
        /* SES-460 changes start */
        if(p_serv_cell->bitmask & X2AP_CSG_ID_INFO_PRESENT)
        {
            response = x2ap_compose_csg_id_info_new(p_asn1_ctx, p_served_cell_extn, p_serv_cell);
        }
        /* SES-460 changes end */

        if(response ==X2AP_FAILURE)
        {
            rtxDListFreeAll(p_asn1_ctx, p_served_cell_extn);
        }
    }

    X2AP_UT_TRACE_EXIT();
    return response;
}

/*****************************************************************************
 * Function Name  : x2ap_send_enb_config_update_response 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context 
 *                  p_enb_config_response - poiter to 
 *                                           x2ap_enb_config_update_resp_t
 *                  result - responce
 *                  dst_id - destination module id
 *                  trans_id - transaction id
 * Outputs        : None
 * Returns        : None
 * Description    : this function sends the eNB config update responce peer eNB
 ********************************************************************************/
void x2ap_send_enb_config_update_response(x2ap_gb_context_t* p_x2ap_gb_ctx, x2ap_enb_config_update_resp_t* p_enb_config_response, U8 result, U16 dst_id, U16 trans_id)
{
	if(result == X2AP_SUCCESS)
	{
		p_enb_config_response->bitmask = 0;
		p_enb_config_response->response = X2AP_SUCCESS;
	}
	else
	{
		p_enb_config_response->bitmask = X2AP_CAUSE_PRESENT;
		p_enb_config_response->response = X2AP_FAILURE;
	}

	X2AP_MEMCPY(&p_enb_config_response->enb1_gb_id, &p_x2ap_gb_ctx->global_enb_id, sizeof(x2_gb_enb_id_t));

	if(!(rrc_x2apCommon_send_x2ap_enb_config_update_resp(p_enb_config_response,RRC_X2AP_MODULE_ID, dst_id, trans_id, RRC_NULL)))
	{
		X2AP_TRACE(X2AP_INFO,"Sending eNB Configuration Update to Peer eNB failed");
	}
}

/*****************************************************************************
 * Function Name  : x2ap_encode_rlf_indication 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2Ap global context 
 *                  p_reestab_conn_setup_reconfig - pointer to 
 *                               x2ap_ue_reestab_conn_setup_reconfig_t
 *                  p_asn_msg - pointer to ASN encoded msg
 *                  p_asn_msg_len - pointer to ASN encoded msg lenght 
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE 
 * Description    : This function used to encode RLF Indication
 ********************************************************************************/
x2ap_return_et x2ap_encode_rlf_indication(
         x2ap_gb_context_t      *p_x2ap_gb_ctx,
         x2ap_ue_reestab_conn_setup_reconfig_t  *p_reestab_conn_setup_reconfig,
         U8                     *p_asn_msg,
         U16                    *p_asn_msg_len)
{
    x2ap_return_et              retVal = X2AP_FAILURE;

    OSCTXT                      asn1_ctx;
    x2ap_X2AP_PDU               x2ap_pdu;

    OSRTDListNode               *p_node = X2AP_P_NULL;

    /* ASN generated x2ap_RLFIndication structure*/
    x2ap_RLFIndication          *p_x2ap_RLFIndication = X2AP_P_NULL;
    x2ap_RLFIndication_protocolIEs_element  *p_protocolIE_elem = X2AP_P_NULL;
    x2ap_UE_RLF_Report_Container *p_container  = X2AP_P_NULL;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;

    X2AP_UT_TRACE_ENTER();
    
     /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        X2AP_TRACE(X2AP_ERROR,"%s:ASN context initialization failed.",
                   __FUNCTION__);
        return retVal;
    }
    
     do
    {
         X2AP_MEMSET( &(x2ap_pdu), 0, sizeof(x2ap_X2AP_PDU) );
        /* Fill the values in the ASN structures that shall be encoded by
        ** ASN Encoder */
        /* Set Pdu type to Initiating message */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_initiatingMessage;

        x2ap_pdu.u.initiatingMessage = rtxMemAllocType(&asn1_ctx,
                x2ap_InitiatingMessage);
        if (X2AP_P_NULL == x2ap_pdu.u.initiatingMessage)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        asn1Init_x2ap_InitiatingMessage(x2ap_pdu.u.initiatingMessage);

        /* fill procedure code for RLFIndication*/
        x2ap_pdu.u.initiatingMessage->procedureCode =
                                    ASN1V_x2ap_id_rLFIndication;
        /* fill criticality of message type */
        x2ap_pdu.u.initiatingMessage->criticality = x2ap_ignore;

        /* Set the initiating message type to rlfIndication */
        x2ap_pdu.u.initiatingMessage->value.t = T1x2ap__rLFIndication;

        p_x2ap_RLFIndication = rtxMemAllocType(&asn1_ctx,
                                        x2ap_RLFIndication);
        if (X2AP_P_NULL == p_x2ap_RLFIndication)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        asn1Init_x2ap_RLFIndication(p_x2ap_RLFIndication);

        x2ap_pdu.u.initiatingMessage->value.u.rLFIndication = 
            p_x2ap_RLFIndication;


        /*** Compose id-FailureCellPCI ***/
        rtxDListAllocNodeAndData(&asn1_ctx,
                x2ap_RLFIndication_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);

        if (X2AP_P_NULL == p_node)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
             break;
        }
        asn1Init_x2ap_RLFIndication_protocolIEs_element(
                p_protocolIE_elem);

        p_protocolIE_elem->id = ASN1V_x2ap_id_FailureCellPCI;
        p_protocolIE_elem->criticality = x2ap_ignore;
        p_protocolIE_elem->value.t = T52x2ap___x2ap_RLFIndication_IEs_1;

        p_protocolIE_elem->value.u._x2ap_RLFIndication_IEs_1 = 
            p_reestab_conn_setup_reconfig->pci;

        rtxDListAppendNode(&p_x2ap_RLFIndication->protocolIEs, p_node);


        /*** Compose id-Re-establishmentCellECGI ***/
        rtxDListAllocNodeAndData(&asn1_ctx,
                x2ap_RLFIndication_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);

        if (X2AP_P_NULL == p_node)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
             break;
        }
        asn1Init_x2ap_RLFIndication_protocolIEs_element(
                p_protocolIE_elem);

        p_protocolIE_elem->id = ASN1V_x2ap_id_Re_establishmentCellECGI;
        p_protocolIE_elem->criticality = x2ap_ignore;
        p_protocolIE_elem->value.t = T52x2ap___x2ap_RLFIndication_IEs_2;

        p_protocolIE_elem->value.u._x2ap_RLFIndication_IEs_2 = 
            rtxMemAllocType(&asn1_ctx,x2ap_ECGI);
        if (X2AP_P_NULL == p_protocolIE_elem->value.u._x2ap_RLFIndication_IEs_2)
        {
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_ECGI(p_protocolIE_elem->value.u._x2ap_RLFIndication_IEs_2);

        if (X2AP_FAILURE == x2ap_compose_ecgi(
                         p_protocolIE_elem->value.u._x2ap_RLFIndication_IEs_2,
                         &(p_x2ap_gb_ctx->served_cell_info[p_reestab_conn_setup_reconfig->cell_index].
                                   ecgi)))
        {
            X2AP_TRACE(X2AP_ERROR,
                    "%s:compose x2ap_ECGI returned failure.",
                    __FUNCTION__);
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }

        rtxDListAppendNode(&p_x2ap_RLFIndication->protocolIEs, p_node);


        /*** Compose id-FailureCellCRNTI ***/
        rtxDListAllocNodeAndData(&asn1_ctx,
                x2ap_RLFIndication_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);

        if (X2AP_P_NULL == p_node)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_RLFIndication_protocolIEs_element(
                p_protocolIE_elem);

        p_protocolIE_elem->id = ASN1V_x2ap_id_FailureCellCRNTI;
        p_protocolIE_elem->criticality = x2ap_ignore;
        p_protocolIE_elem->value.t = T52x2ap___x2ap_RLFIndication_IEs_3;

        p_protocolIE_elem->value.u._x2ap_RLFIndication_IEs_3 = 
            rtxMemAllocType(&asn1_ctx, x2ap_CRNTI);
        if (X2AP_P_NULL == p_protocolIE_elem->value.u._x2ap_RLFIndication_IEs_3)
        {
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_CRNTI(p_protocolIE_elem->value.u._x2ap_RLFIndication_IEs_3);

        p_protocolIE_elem->value.u._x2ap_RLFIndication_IEs_3->numbits = 
                                                X2AP_CRNTI_BITSTRING_SIZE;

        p_protocolIE_elem->value.u._x2ap_RLFIndication_IEs_3->data[0] = 
                              (p_reestab_conn_setup_reconfig->crnti >> 8) & 0xff;
        p_protocolIE_elem->value.u._x2ap_RLFIndication_IEs_3->data[1] =
                              (p_reestab_conn_setup_reconfig->crnti & 0xff);

        rtxDListAppendNode(&p_x2ap_RLFIndication->protocolIEs, p_node);

        /*** Compose id-ShortMAC-I ***/
        rtxDListAllocNodeAndData(&asn1_ctx,
                x2ap_RLFIndication_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);

        if (X2AP_P_NULL == p_node)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_RLFIndication_protocolIEs_element(
                p_protocolIE_elem);

        p_protocolIE_elem->id = ASN1V_x2ap_id_ShortMAC_I;
        p_protocolIE_elem->criticality = x2ap_ignore;
        p_protocolIE_elem->value.t = T52x2ap___x2ap_RLFIndication_IEs_4;

        p_protocolIE_elem->value.u._x2ap_RLFIndication_IEs_4 = 
            rtxMemAllocType(&asn1_ctx, x2ap_ShortMAC_I);
        if (X2AP_P_NULL == p_protocolIE_elem->value.u._x2ap_RLFIndication_IEs_4)
        {
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_ShortMAC_I(p_protocolIE_elem->value.u._x2ap_RLFIndication_IEs_4);

        p_protocolIE_elem->value.u._x2ap_RLFIndication_IEs_4->numbits = 
                                                X2AP_SHORT_MAC_I_BITSTRING_SIZE;

        p_protocolIE_elem->value.u._x2ap_RLFIndication_IEs_4->data[0] = 
                                    (p_reestab_conn_setup_reconfig->short_mac_i >> 8) & 0xff;
        p_protocolIE_elem->value.u._x2ap_RLFIndication_IEs_4->data[1] =
                                    (p_reestab_conn_setup_reconfig->short_mac_i & 0xff);

        rtxDListAppendNode(&p_x2ap_RLFIndication->protocolIEs, p_node);


        /*** Compose id-UE-RLF-Report-Container if present ***/
        if (p_reestab_conn_setup_reconfig->bitmask & X2AP_RLF_REPORT_CONTAINER_PRESENT)
        {
            rtxDListAllocNodeAndData(&asn1_ctx,
                    x2ap_RLFIndication_protocolIEs_element,
                    &p_node,
                    &p_protocolIE_elem);

            if (X2AP_P_NULL == p_node)
            {
                X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
                break;
            }
            asn1Init_x2ap_RLFIndication_protocolIEs_element(
                    p_protocolIE_elem);
            p_protocolIE_elem->id = ASN1V_x2ap_id_UE_RLF_Report_Container;
            p_protocolIE_elem->criticality = x2ap_ignore;
            p_protocolIE_elem->value.t = T52x2ap___x2ap_RLFIndication_IEs_5;

            p_protocolIE_elem->value.u._x2ap_RLFIndication_IEs_5 = 
                rtxMemAllocType(&asn1_ctx, x2ap_UE_RLF_Report_Container);
            if (X2AP_P_NULL == p_protocolIE_elem->value.u._x2ap_RLFIndication_IEs_5)
            {
                X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                break;
            }
            asn1Init_x2ap_UE_RLF_Report_Container(
                    p_protocolIE_elem->value.u._x2ap_RLFIndication_IEs_5);

            p_container = p_protocolIE_elem->value.u._x2ap_RLFIndication_IEs_5;

            do
            {
                p_container->numocts = p_reestab_conn_setup_reconfig->rlf_report.message_buff_size;
                p_container->data = (U8 *)rtxMemAlloc(&asn1_ctx, p_container->numocts);
                if (PNULL == p_container->data)
                {
                    X2AP_TRACE(X2AP_FATAL, "ASN Memory allocation failed.");
                    break;
                }

                X2AP_MEMCPY((void*)p_container->data, 
                        p_reestab_conn_setup_reconfig->rlf_report.message_buff_p, p_container->numocts);


            }while(0);

            rtxDListAppendNode(&p_x2ap_RLFIndication->protocolIEs, p_node);
        }
        
        if (p_reestab_conn_setup_reconfig->bitmask & X2AP_RLF_CONN_SETUP_INDICATOR)
        {
            rtxDListAllocNodeAndData(&asn1_ctx,
                    x2ap_RLFIndication_protocolIEs_element,
                    &p_node,
                    &p_protocolIE_elem);

            if (X2AP_P_NULL == p_node)
            {
                X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
                break;
            }

            p_protocolIE_elem->id = ASN1V_x2ap_id_RRCConnSetupIndicator;
            p_protocolIE_elem->criticality = x2ap_reject;
            p_protocolIE_elem->value.t = T52x2ap___x2ap_RLFIndication_IEs_6;

            p_protocolIE_elem->value.u._x2ap_RLFIndication_IEs_6 = x2ap_rrcConnSetup;

            rtxDListAppendNode(&p_x2ap_RLFIndication->protocolIEs, p_node);

        }

         /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_asn_msg,
                     X2AP_MAX_ASN1_BUF_LEN, TRUE);

        if (0 != asn1PE_x2ap_X2AP_PDU(&asn1_ctx, &x2ap_pdu))
        {
            X2AP_TRACE(X2AP_ERROR,  "ASN1 encoding of RLF INDICATION failed.");
            break;
        }
        else
        {
            *p_asn_msg_len= (U16)pe_GetMsgLen(&asn1_ctx);
            
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
            retVal = X2AP_SUCCESS;
            
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    (U32)*p_asn_msg_len);
            if (PNULL != p_event_header)
            {
                x2ap_fill_protocol_event_params(p_event_header, (U32)*p_asn_msg_len,
                        LOCAL_X2_RLF_INDICATION, 
                        MESSAGE_DIRECTION_SEND);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        (U32)*p_asn_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }
        
     }while(0);
    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_compose_ecgi 
 * Inputs         : 
 *                  p_asn_ecgi - pointer to x2ap_ECGI
 *                  p_rrc_ecgi - pointer to x2ap_ecgi_t
 * Outputs        : p_asn_ecgi
 * Returns        : X2AP_FAILURE/X2AP_SUCCESS
 * Description    : this function composes ecgi 
 ********************************************************************************/
x2ap_return_et x2ap_compose_ecgi(
        x2ap_ECGI   *p_asn_ecgi,
        x2ap_ecgi_t *p_rrc_ecgi)
{
    X2AP_UT_TRACE_ENTER();


    p_asn_ecgi->pLMN_Identity.numocts = MAX_PLMN_ID_BYTES;

    X2AP_MEMCPY(p_asn_ecgi->pLMN_Identity.data,
                p_rrc_ecgi->plmn_identity.plmn_id,
                MAX_PLMN_ID_BYTES);

    p_asn_ecgi->eUTRANcellIdentifier.numbits = X2AP_HOME_ENB_ID_BITSTRING_SIZE;

    X2AP_MEMCPY(p_asn_ecgi->eUTRANcellIdentifier.data,
                p_rrc_ecgi->eutran_cell_id,
                HOME_ENB_ID_OCTET_SIZE);

    p_asn_ecgi->m.iE_ExtensionsPresent = 0;

    rtxDListInit(&p_asn_ecgi->iE_Extensions);
    rtxDListInit(&p_asn_ecgi->extElem1);

    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS; 
}

/*****************************************************************************
  * Function Name  : x2ap_encode_handover_report 
 * Inputs         : 
 *                  p_ho_report_ind - pointer to p_ho_report_ind
 *                  p_asn_msg - pointer to encoded ASN message
 *                  p_asn_msg_len - pointer o encoded ASN message length  
 * Outputs        : p_asn_msg - encoded ASN message
 *                  p_asn_msg_len - pointer o encoded ASN message length
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to encode Handover Report
 * ERROR          : This function report error to the caller function, If encoder
 *                  fails to encode message.
 ********************************************************************************/
x2ap_return_et x2ap_encode_handover_report(
            x2ap_ho_report_info_t   *p_ho_report_ind,
            U8                     *p_asn_msg,
            U16                    *p_asn_msg_len)
{
    x2ap_return_et              retVal = X2AP_FAILURE;

    OSCTXT                      asn1_ctx;
    x2ap_X2AP_PDU               x2ap_pdu;

    OSRTDListNode               *p_node = X2AP_P_NULL;

    x2ap_HandoverReport         *p_ho_report = X2AP_P_NULL;
    x2ap_HandoverReport_protocolIEs_element  *p_protocolIE_elem = X2AP_P_NULL;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;

    X2AP_UT_TRACE_ENTER();


    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        X2AP_TRACE(X2AP_ERROR,"%s:ASN context initialization failed.",
                __FUNCTION__);
        return retVal;
    }

    do
    {
        X2AP_MEMSET( &(x2ap_pdu), 0, sizeof(x2ap_X2AP_PDU) );
        /* Fill the values in the ASN structures that shall be encoded by
         ** ASN Encoder */
        /* Set Pdu type to Initiating message */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_initiatingMessage;

        x2ap_pdu.u.initiatingMessage = rtxMemAllocType(&asn1_ctx,
                x2ap_InitiatingMessage);
        if (X2AP_P_NULL == x2ap_pdu.u.initiatingMessage)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_InitiatingMessage(x2ap_pdu.u.initiatingMessage);

        x2ap_pdu.u.initiatingMessage->procedureCode = ASN1V_x2ap_id_handoverReport;
        x2ap_pdu.u.initiatingMessage->criticality   = x2ap_ignore;
        x2ap_pdu.u.initiatingMessage->value.t       = T1x2ap__handoverReport;

        p_ho_report = rtxMemAllocType(&asn1_ctx, x2ap_HandoverReport);
        if (X2AP_P_NULL == p_ho_report)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_HandoverReport(p_ho_report);
        x2ap_pdu.u.initiatingMessage->value.u.handoverReport = p_ho_report;

        /*** Compose id-HandoverReportType ***/
        rtxDListAllocNodeAndData(&asn1_ctx,
                x2ap_HandoverReport_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if (X2AP_P_NULL == p_node)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_HandoverReport_protocolIEs_element(
                p_protocolIE_elem);
        p_protocolIE_elem->id = ASN1V_x2ap_id_HandoverReportType;
        p_protocolIE_elem->criticality = x2ap_ignore;
        p_protocolIE_elem->value.t = T15x2ap___x2ap_HandoverReport_IEs_1;

        p_protocolIE_elem->value.u._x2ap_HandoverReport_IEs_1 = 
            p_ho_report_ind->ho_report_type;

        rtxDListAppendNode(&p_ho_report->protocolIEs, p_node);


        /*** Compose id-Cause ***/
        rtxDListAllocNodeAndData(&asn1_ctx,
                x2ap_HandoverReport_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if (X2AP_P_NULL == p_node)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_HandoverReport_protocolIEs_element(
                p_protocolIE_elem);
        p_protocolIE_elem->id = ASN1V_x2ap_id_Cause;
        p_protocolIE_elem->criticality = x2ap_ignore;
        p_protocolIE_elem->value.t = T15x2ap___x2ap_HandoverReport_IEs_2;
        p_protocolIE_elem->value.u._x2ap_HandoverReport_IEs_2 = 
            rtxMemAllocType(&asn1_ctx, x2ap_Cause);
        if (X2AP_P_NULL == p_protocolIE_elem->value.u._x2ap_HandoverReport_IEs_2)
        {
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_Cause (p_protocolIE_elem->value.u._x2ap_HandoverReport_IEs_2);

        x2ap_reset_compose_asn_x2ap_cause(p_ho_report_ind->ho_cause,
                               p_protocolIE_elem->value.u._x2ap_HandoverReport_IEs_2);

        rtxDListAppendNode(&p_ho_report->protocolIEs, p_node);


        /*** Compose id-SourceCellECGI ***/
        rtxDListAllocNodeAndData(&asn1_ctx,
                x2ap_HandoverReport_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if (X2AP_P_NULL == p_node)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_HandoverReport_protocolIEs_element(
                p_protocolIE_elem);
        p_protocolIE_elem->id = ASN1V_x2ap_id_SourceCellECGI;
        p_protocolIE_elem->criticality = x2ap_ignore;
        p_protocolIE_elem->value.t = T15x2ap___x2ap_HandoverReport_IEs_3;
        p_protocolIE_elem->value.u._x2ap_HandoverReport_IEs_3 = 
            rtxMemAllocType(&asn1_ctx, x2ap_ECGI);
        if (X2AP_P_NULL == p_protocolIE_elem->value.u._x2ap_HandoverReport_IEs_3)
        {
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_ECGI(p_protocolIE_elem->value.u._x2ap_HandoverReport_IEs_3);

        if (X2AP_FAILURE == x2ap_compose_ecgi(
                        p_protocolIE_elem->value.u._x2ap_HandoverReport_IEs_3,
                        &(p_ho_report_ind->src_ecgi)))
        {
            X2AP_TRACE(X2AP_ERROR, "%s:compose x2ap_ECGI returned failure.",
                                        __FUNCTION__);
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }
        rtxDListAppendNode(&p_ho_report->protocolIEs, p_node);


        /*** Compose id-FailureCellECGI ***/
        rtxDListAllocNodeAndData(&asn1_ctx,
                x2ap_HandoverReport_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if (X2AP_P_NULL == p_node)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_HandoverReport_protocolIEs_element(
                p_protocolIE_elem);
        p_protocolIE_elem->id = ASN1V_x2ap_id_FailureCellECGI;
        p_protocolIE_elem->criticality = x2ap_ignore;
        p_protocolIE_elem->value.t = T15x2ap___x2ap_HandoverReport_IEs_4;
        p_protocolIE_elem->value.u._x2ap_HandoverReport_IEs_4 = 
            rtxMemAllocType(&asn1_ctx, x2ap_ECGI);
        if (X2AP_P_NULL == p_protocolIE_elem->value.u._x2ap_HandoverReport_IEs_4)
        {
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_ECGI(p_protocolIE_elem->value.u._x2ap_HandoverReport_IEs_4);

        if (X2AP_FAILURE == x2ap_compose_ecgi(
                        p_protocolIE_elem->value.u._x2ap_HandoverReport_IEs_4,
                        &(p_ho_report_ind->fail_cell_ecgi)))
        {
            X2AP_TRACE(X2AP_ERROR, "%s:compose x2ap_ECGI returned failure.",
                                        __FUNCTION__);
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }
        rtxDListAppendNode(&p_ho_report->protocolIEs, p_node);


        if (p_ho_report_ind->bitmask &
                X2AP_HO_REPORT_REEST_CELL_ECGI_PRESENT)
        {
            /*** Compose id-Re-establishmentCellECGI ***/
            rtxDListAllocNodeAndData(&asn1_ctx,
                    x2ap_HandoverReport_protocolIEs_element,
                    &p_node,
                    &p_protocolIE_elem);
            if (X2AP_P_NULL == p_node)
            {
                X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
                break;
            }
            asn1Init_x2ap_HandoverReport_protocolIEs_element(
                    p_protocolIE_elem);
            p_protocolIE_elem->id = ASN1V_x2ap_id_Re_establishmentCellECGI;
            p_protocolIE_elem->criticality = x2ap_ignore;
            p_protocolIE_elem->value.t = T15x2ap___x2ap_HandoverReport_IEs_5;
            p_protocolIE_elem->value.u._x2ap_HandoverReport_IEs_5 = 
                rtxMemAllocType(&asn1_ctx, x2ap_ECGI);
            if (X2AP_P_NULL == p_protocolIE_elem->value.u._x2ap_HandoverReport_IEs_5)
            {
                X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                break;
            }
            asn1Init_x2ap_ECGI(p_protocolIE_elem->value.u._x2ap_HandoverReport_IEs_5);

            if (X2AP_FAILURE == x2ap_compose_ecgi(
                        p_protocolIE_elem->value.u._x2ap_HandoverReport_IEs_5,
                        &(p_ho_report_ind->re_est_cell_ecgi)))
            {
                X2AP_TRACE(X2AP_ERROR, "%s:compose x2ap_ECGI returned failure.",
                        __FUNCTION__);
                /*SPR_18125_START*/
                rtFreeContext(&asn1_ctx);
                /*SPR_18125_END*/
                X2AP_UT_TRACE_EXIT();
                return X2AP_FAILURE;
            }
            rtxDListAppendNode(&p_ho_report->protocolIEs, p_node);
        }

        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_asn_msg,
                X2AP_MAX_ASN1_BUF_LEN, TRUE);
        if (0 != asn1PE_x2ap_X2AP_PDU(&asn1_ctx, &x2ap_pdu))
        {
            X2AP_TRACE(X2AP_ERROR,  "ASN1 encoding of Handover Report failed.");
            break;
        }
        else
        {
            *p_asn_msg_len= (U16)pe_GetMsgLen(&asn1_ctx);

            
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
            /*coverity_fix_531*/ 
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    (U32)*p_asn_msg_len);

            if (PNULL != p_event_header)
            {

                x2ap_fill_protocol_event_params(p_event_header, (U32)*p_asn_msg_len,
                        LOCAL_X2_HANDOVER_REPORT, 
                        MESSAGE_DIRECTION_SEND);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        (U32)*p_asn_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
            /*coverity_fix_531*/ 
            retVal = X2AP_SUCCESS;
        }
    }while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);
    return retVal;
}

#ifdef ENDC_ENABLED
/*****************************************************************************
 * Function Name  : x2ap_compose_nrcgi 
 * Inputs         : 
 *                  p_asn_nrcgi - pointer to x2ap_NRCGI
 *                  p_rrc_nrcgi - pointer to x2ap_nr_ecgi_t
 * Outputs        : p_asn_nrcgi
 * Returns        : X2AP_FAILURE/X2AP_SUCCESS
 * Description    : this function composes nrcgi 
 ********************************************************************************/
x2ap_return_et x2ap_compose_nrcgi(
        x2ap_NRCGI   *p_asn_nrcgi,
        x2ap_nr_ecgi_t *p_rrc_nrcgi)
{
    X2AP_UT_TRACE_ENTER();

    p_asn_nrcgi->pLMN_Identity.numocts = MAX_PLMN_ID_BYTES;

    X2AP_MEMCPY(p_asn_nrcgi->pLMN_Identity.data,
                p_rrc_nrcgi->plmn_identity.plmn_id,
                MAX_PLMN_ID_BYTES);

    p_asn_nrcgi->nRcellIdentifier.numbits = X2AP_NR_CELL_ID_BIT_STR_LEN;

    X2AP_MEMCPY(p_asn_nrcgi->nRcellIdentifier.data,
                p_rrc_nrcgi->nr_cell_id,
                NR_CELL_ID_OCTET_SIZE);

    p_asn_nrcgi->m.iE_ExtensionsPresent = 0;

    rtxDListInit(&p_asn_nrcgi->iE_Extensions);
    rtxDListInit(&p_asn_nrcgi->extElem1);

    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS; 
}

/* OPTION3X Changes Start */       
/****************************************************************************
 * Function Name  : x2ap_internal_encode_nr_rrc_radio_bearer_config 
 * Inputs         : pctxt - pointer to OSCTXT
 *                  p_radio_bearer_config - pointer to nr_rrc_RadioBearerConfig
 *                  p_oct_str - pointer to 
 *                            OSDynOctStr
 * Outputs        : p_menb_to_sgnb_container
 * Returns        : X2AP_SUCCESS / X2AP_FAILURE
 * Description    : Encodes NR Radio Bearer Config message
 ****************************************************************************/
x2ap_return_et x2ap_internal_encode_nr_rrc_radio_bearer_config
(
    OSCTXT                   *pctxt,  /* for memory allocation */
    nr_rrc_RadioBearerConfig *p_radio_bearer_config,
    OSDynOctStr              *p_oct_str
)
{
    rrc_return_et   result = X2AP_FAILURE;
    U8              msg[X2AP_MAX_ASN1_BUF_LEN];

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != pctxt);
    X2AP_ASSERT(PNULL != p_radio_bearer_config);
    X2AP_ASSERT(PNULL != p_oct_str);

    do
    {
        /* ASN Encode message */
        pu_setBuffer(pctxt, msg, X2AP_MAX_ASN1_BUF_LEN, ASN_RRC_BUF_TYPE);

        if (RT_OK != asn1PE_nr_rrc_RadioBearerConfig(pctxt, p_radio_bearer_config))
        {
            rtxErrPrint(pctxt);
            X2AP_TRACE(X2AP_ERROR,
                "%s: ASN1 encoding for NR Radio Bearer Config failed.",__FUNCTION__);
            break;
        }
    
        p_oct_str->numocts = (U16)pe_GetMsgLen(pctxt);

        p_oct_str->data = (U8 *)rtxMemAlloc(pctxt, p_oct_str->numocts);
        if (PNULL == p_oct_str->data)
        {
            X2AP_TRACE(X2AP_FATAL,
            "[x2ap_internal_encode_nr_rrc_radio_bearer_config]"
            "ASN Memory allocation failed.");
            break;
        }

        X2AP_MEMCPY((void*)p_oct_str->data, msg,
                    p_oct_str->numocts);
                
        result = X2AP_SUCCESS;
    } while(0);
    
    X2AP_UT_TRACE_EXIT();
    return result;
}
/* OPTION3X Changes End */
/*NR_DC Code Change Start*/
/****************************************************************************
 * Function Name  : x2ap_internal_encode_menb_to_sgnb_container 
 * Inputs         : pctxt - pointer to OSCTXT
 *                  p_nr_cg_config_info - pointer to nr_rrc_CG_ConfigInfo
 *                  p_menb_to_sgnb_container - pointer to 
 *                            x2ap_MeNBtoSgNBContainer
 * Outputs        : p_menb_to_sgnb_container
 * Returns        : X2AP_SUCCESS / X2AP_FAILURE
 * Description    : Encodes  NR CG Config Info message
 ****************************************************************************/
x2ap_return_et x2ap_internal_encode_menb_to_sgnb_container
(
    OSCTXT                   *pctxt,  /* for memory allocation */
    nr_rrc_CG_ConfigInfo     *p_nr_cg_config_info,
    x2ap_MeNBtoSgNBContainer *p_container
)
{
    rrc_return_et result = X2AP_FAILURE;
    U8 msg[X2AP_MAX_ASN1_BUF_LEN];
    S8* pBuff = rrc_mem_get(MAX_ASN_PRINT_BUFF_SIZE);
    /* coverity fix 277504 */
    if ( PNULL == pBuff )
    {
        X2AP_TRACE(X2AP_FATAL,
             "ASN Memory allocation failed.");
      return X2AP_FAILURE;
    }
    /* coverity fix 277504 */

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != pctxt);
    X2AP_ASSERT(PNULL != p_nr_cg_config_info);
    X2AP_ASSERT(PNULL != p_container);

    do
    {
        /* ASN Encode message */
        pu_setBuffer(pctxt, msg, X2AP_MAX_ASN1_BUF_LEN, ASN_RRC_BUF_TYPE);

        if (RT_OK != asn1PE_nr_rrc_CG_ConfigInfo(pctxt, p_nr_cg_config_info))
        {
            rtxErrPrint(pctxt);
            X2AP_TRACE(X2AP_ERROR,
                "%s: ASN1 encoding for NR CG Config Info failed.",__FUNCTION__);
            break;
        }
    
        p_container->numocts = (U16)pe_GetMsgLen(pctxt);

        p_container->data = (U8 *)rtxMemAlloc(pctxt, p_container->numocts);
        if (PNULL == p_container->data)
        {
            X2AP_TRACE(X2AP_FATAL,
            "[x2ap_internal_encode_menb_to_sgnb_container]"
            "ASN Memory allocation failed.");
            break;
        }

        X2AP_MEMCPY((void*)p_container->data, msg,
                    p_container->numocts);
                
        result = X2AP_SUCCESS;
    } while(0);
    
    /*Print the cgConfigInfo*/
    {
        memset_wrapper(pBuff,0, MAX_ASN_PRINT_BUFF_SIZE);
        asn1Print_nr_rrc_CG_ConfigInfo("CGConfigInfo",p_nr_cg_config_info);
        rrc_mem_free(pBuff);
    }

    X2AP_UT_TRACE_EXIT();
    return result;
}

/*****************************************************************************
 * Function Name  : x2ap_encode_sgNB_addition_request
 * Inputs         : pctxt - pointer to OSCTXT
 *                  p_sgnb_addition_req - pointer to x2ap_sgnb_addition_request_t
 *                  p_asn_msg - pointer to ASN encoded message
 *                  p_asn_msg_len - pointer to ASN encoded message length
 * Outputs        : p_asn_msg -
 *                  p_asn_msg_len - encoded message length
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to encode SgNB Addition Request.
 ********************************************************************************/
x2ap_return_et x2ap_encode_sgNB_addition_request(
        OSCTXT                              *pctxt,
        U8                                  *p_asn_msg,
        U16                                 *p_asn_msg_len,
        rrc_x2ap_sgnb_addition_request_t    *p_sgnb_addition_req)
{
    x2ap_return_et              result = X2AP_FAILURE;
    x2ap_X2AP_PDU               x2ap_pdu;
    x2ap_SgNBAdditionRequest    *p_x2ap_SgNBAdditionRequest = X2AP_P_NULL;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != pctxt);
    X2AP_ASSERT(PNULL != p_sgnb_addition_req);
    X2AP_ASSERT(PNULL != p_asn_msg);
    X2AP_ASSERT(PNULL != p_asn_msg_len);
    
    
    do
    {
        X2AP_MEMSET ( &(x2ap_pdu), X2AP_NULL, sizeof(x2ap_X2AP_PDU) );
        /* Init X2AP PDU */
        asn1Init_x2ap_X2AP_PDU(&x2ap_pdu);
        /* Set Pdu type to Initiating message */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_initiatingMessage;
        x2ap_pdu.u.initiatingMessage = rtxMemAllocType(pctxt,
                x2ap_InitiatingMessage);
        if (X2AP_P_NULL == x2ap_pdu.u.initiatingMessage)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_InitiatingMessage(x2ap_pdu.u.initiatingMessage);

        x2ap_pdu.u.initiatingMessage->procedureCode = 
            ASN1V_x2ap_id_sgNBAdditionPreparation;
        x2ap_pdu.u.initiatingMessage->criticality = x2ap_reject;


        /* Set the initiating message type to SgNB Addition Preparation */ 
        x2ap_pdu.u.initiatingMessage->value.t = T1x2ap__sgNBAdditionPreparation;

        x2ap_pdu.u.initiatingMessage->value.u.sgNBAdditionPreparation = 
                                        rtxMemAllocType(pctxt, x2ap_SgNBAdditionRequest);  
        if (X2AP_P_NULL == x2ap_pdu.u.initiatingMessage->value.u.sgNBAdditionPreparation)
        {
             X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
             break;
        }
        p_x2ap_SgNBAdditionRequest = x2ap_pdu.u.initiatingMessage->value.u.sgNBAdditionPreparation;

        asn1Init_x2ap_SgNBAdditionRequest(p_x2ap_SgNBAdditionRequest);

        /* Fill the values in the ASN structures that shall be encoded by
         *  ASN Encoder */

        /* Fill MeNB-UE-X2AP-ID */
        if (RT_OK != asn1Append_x2ap_SgNBAdditionRequest_protocolIEs_1(
            pctxt,
            &p_x2ap_SgNBAdditionRequest->protocolIEs,
            p_sgnb_addition_req->menb_ue_x2ap_id))
        {
            break;
        }

        /* Fill NRUESecurityCapabilities */
        if (RT_OK != asn1Append_x2ap_SgNBAdditionRequest_protocolIEs_2(
            pctxt,
            &p_x2ap_SgNBAdditionRequest->protocolIEs,
            &p_sgnb_addition_req->nr_ue_security_capability))
        {
            break;
        }

        /* Fill SgNBSecurityKey */
        if (RT_OK != asn1Append_x2ap_SgNBAdditionRequest_protocolIEs_3(
            pctxt,
            &p_x2ap_SgNBAdditionRequest->protocolIEs,
            &p_sgnb_addition_req->sgnb_security_key))
        {
            break;
        }

        /* Fill SgNBUEAggregateMaximumBitRate */
        if (RT_OK != asn1Append_x2ap_SgNBAdditionRequest_protocolIEs_4(
            pctxt,
            &p_x2ap_SgNBAdditionRequest->protocolIEs,
            &p_sgnb_addition_req->ue_aggregate_max_bitrate))
        {
            break;
        }
        
        /* Fill SelectedPLMN */
        if (p_sgnb_addition_req->bitmask & X2AP_SGNB_ADD_REQ_SELECTED_PLMN_PRESENT)
        {
            if (RT_OK != asn1Append_x2ap_SgNBAdditionRequest_protocolIEs_5(
                pctxt,
                &p_x2ap_SgNBAdditionRequest->protocolIEs,
                &p_sgnb_addition_req->plmn_identity))
            {
                break;
            }
        }

        /* Fill HandoverRestrictionList */
        if (p_sgnb_addition_req->bitmask & X2AP_SGNB_ADD_REQ_HO_RESTRICITION_LIST_PRESENT)
        {
            if (RT_OK != asn1Append_x2ap_SgNBAdditionRequest_protocolIEs_6(
                pctxt,
                &p_x2ap_SgNBAdditionRequest->protocolIEs,
                &p_sgnb_addition_req->ho_restricition_list))
            {
                break;
            }
        }

        /* Fill E-RABs-ToBeAdded-SgNBAddReqList */
        if (RT_OK != asn1Append_x2ap_SgNBAdditionRequest_protocolIEs_7(
            pctxt,
            &p_x2ap_SgNBAdditionRequest->protocolIEs,
            &p_sgnb_addition_req->erab_to_add_list))
        {
            break;
        }

        /* Fill MeNBtoSgNBContainer */
        if (RT_OK != asn1Append_x2ap_SgNBAdditionRequest_protocolIEs_8(
            pctxt,
            &p_x2ap_SgNBAdditionRequest->protocolIEs,
            &p_sgnb_addition_req->menb_to_sgnb_container))
        {
            break;
        }

        /* Fill SgNB-UE-X2AP-ID */
        if (p_sgnb_addition_req->bitmask & X2AP_SGNB_ADD_REQ_SGNB_UE_X2AP_ID_PRESENT)
        {
            if (RT_OK != asn1Append_x2ap_SgNBAdditionRequest_protocolIEs_9(
                pctxt,
                &p_x2ap_SgNBAdditionRequest->protocolIEs,
                p_sgnb_addition_req->sgnb_ue_x2ap_id))
            {
                break;
            }
        }

        /* Fill ExpectedUEBehaviour */
        if (p_sgnb_addition_req->bitmask & X2AP_SGNB_ADD_REQ_EXPECTED_UE_BEHAVIOUR_PRESENT)
        {
            if (RT_OK != asn1Append_x2ap_SgNBAdditionRequest_protocolIEs_10(
                pctxt,
                &p_x2ap_SgNBAdditionRequest->protocolIEs,
                &p_sgnb_addition_req->expected_ue_behavior))
            {
                break;
            }
        }

        /* Fill MeNB-UE-X2AP-ID-Extension */
        if (p_sgnb_addition_req->bitmask & X2AP_SGNB_ADD_REQ_MENB_UE_X2AP_ID_EXTENSION_PRESENT)
        {
            if (RT_OK != asn1Append_x2ap_SgNBAdditionRequest_protocolIEs_11(
                pctxt,
                &p_x2ap_SgNBAdditionRequest->protocolIEs,
                p_sgnb_addition_req->menb_ue_x2ap_id_ext))
            {
                break;
            }
        }

        /* Fill RequestedSplitSRBs */
        if (p_sgnb_addition_req->bitmask & X2AP_SGNB_ADD_REQ_MCG_SPLIT_SRBs_PRESENT)
        {
            if (RT_OK != asn1Append_x2ap_SgNBAdditionRequest_protocolIEs_12(
                pctxt,
                &p_x2ap_SgNBAdditionRequest->protocolIEs,
                p_sgnb_addition_req->split_SRBs))
            {
                break;
            }
        }

        /* Fill MeNBResourceCoordinationInformation */
        if (p_sgnb_addition_req->bitmask & X2AP_SGNB_ADD_REQ_MENB_RESOURCE_COORDINATION_INFO_PRESENT)
        {
            if (RT_OK != asn1Append_x2ap_SgNBAdditionRequest_protocolIEs_13(
                pctxt,
                &p_x2ap_SgNBAdditionRequest->protocolIEs,
                &p_sgnb_addition_req->menb_resource_coordination_info))
            {
                break;
            }
        }

        /* Fill SGNB-Addition-Trigger-Ind */
        if ( X2AP_SGNB_ADD_REQ_SGNB_ADDITION_TRIGGER_IND_PRESENT &
            p_sgnb_addition_req->bitmask )
        {
            if (RT_OK != asn1Append_x2ap_SgNBAdditionRequest_protocolIEs_14(
                pctxt,
                &p_x2ap_SgNBAdditionRequest->protocolIEs,
                p_sgnb_addition_req->sgnb_addition_trigger_ind))
            {
                break;
            }
        }

        /* Fill SubscriberProfileIDforRFP */
        if ( X2AP_SGNB_ADD_REQ_SUBSCRIBER_PROFILE_ID_FOR_RFP_PRESENT &
            p_sgnb_addition_req->bitmask )
        {
            if (RT_OK != asn1Append_x2ap_SgNBAdditionRequest_protocolIEs_15(
                pctxt,
                &p_x2ap_SgNBAdditionRequest->protocolIEs,
                p_sgnb_addition_req->subscriber_profile_id_for_rfp))
            {
                break;
            }
        }

        /* Fill MeNBCell-ID */
        if (RT_OK != asn1Append_x2ap_SgNBAdditionRequest_protocolIEs_16(
            pctxt,
            &p_x2ap_SgNBAdditionRequest->protocolIEs,
            &p_sgnb_addition_req->menb_cell_id))
        {
            break;
        }

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_asn_msg, X2AP_MAX_ASN1_BUF_LEN, ASN_X2AP_BUF_TYPE);

        if (RT_OK != asn1PE_x2ap_X2AP_PDU(pctxt, &x2ap_pdu))
        {
            rtxErrPrint(pctxt);
            X2AP_TRACE(X2AP_ERROR,"%s:ASN1 encoding failed.",__FUNCTION__);
            break;
        }
        else
        {
            *p_asn_msg_len = (U16)pe_GetMsgLen(pctxt);
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
            result=X2AP_SUCCESS;
        }
    }while(0);
    
    X2AP_UT_TRACE_EXIT();
    return result;
}

/*****************************************************************************
 * Function Name  : x2ap_encode_sgNB_reconfig_complete
 * Inputs         : pctxt - pointer to OSCTXT
 *                  p_sgnb_reconfig_comp - pointer to
 *                                         rrc_x2ap_sgnb_reconfig_complete_t
 *                  p_asn_msg - pointer to ASN encoded message
 *                  p_asn_msg_len - pointer to ASN encoded message length
 * Outputs        : p_asn_msg -
 *                  p_asn_msg_len - encoded message length
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to encode SgNB Reconfiguration Complete.
 ********************************************************************************/
x2ap_return_et x2ap_encode_sgNB_reconfig_complete(
        OSCTXT                          *pctxt,
        U8                              *p_asn_msg,
        U16                             *p_asn_msg_len,
        rrc_x2ap_sgnb_reconfig_complete_t
                                        *p_sgnb_reconfig_complete)
{
    x2ap_return_et              result = X2AP_FAILURE;
    x2ap_X2AP_PDU               x2ap_pdu;
    x2ap_SgNBReconfigurationComplete
                                *p_x2ap_SgNBReconfigurationComplete = X2AP_P_NULL;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != pctxt);
    X2AP_ASSERT(PNULL != p_sgnb_reconfig_complete);
    X2AP_ASSERT(PNULL != p_asn_msg);
    X2AP_ASSERT(PNULL != p_asn_msg_len);

    do
    {
        X2AP_MEMSET ( &(x2ap_pdu), X2AP_NULL, sizeof(x2ap_X2AP_PDU) );
        /* Init X2AP PDU */
        asn1Init_x2ap_X2AP_PDU(&x2ap_pdu);
        /* Set Pdu type to Initiating message */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_initiatingMessage;
        x2ap_pdu.u.initiatingMessage = rtxMemAllocType(pctxt,
                x2ap_InitiatingMessage);
        if (X2AP_P_NULL == x2ap_pdu.u.initiatingMessage)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        asn1Init_x2ap_InitiatingMessage(x2ap_pdu.u.initiatingMessage);

        x2ap_pdu.u.initiatingMessage->procedureCode = 
            ASN1V_x2ap_id_sgNBReconfigurationCompletion;
        x2ap_pdu.u.initiatingMessage->criticality = x2ap_ignore;

        /* Set the initiating message type to SgNB Reconfiguration Completion */
        x2ap_pdu.u.initiatingMessage->value.t = 
            T1x2ap__sgNBReconfigurationCompletion;

        x2ap_pdu.u.initiatingMessage->value.u.sgNBReconfigurationCompletion = 
            rtxMemAllocType(pctxt, x2ap_SgNBReconfigurationComplete);  
        if (X2AP_P_NULL == x2ap_pdu.u.initiatingMessage->value.u.sgNBReconfigurationCompletion)
        {
             X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
             break;
        }
        p_x2ap_SgNBReconfigurationComplete = 
            x2ap_pdu.u.initiatingMessage->value.u.sgNBReconfigurationCompletion;

        asn1Init_x2ap_SgNBReconfigurationComplete(p_x2ap_SgNBReconfigurationComplete);

        /* Fill the values in the ASN structures that shall be encoded by
         *  ASN Encoder */

        if (RT_OK != asn1Append_x2ap_SgNBReconfigurationComplete_protocolIEs_1(
            pctxt,
            &p_x2ap_SgNBReconfigurationComplete->protocolIEs,
            p_sgnb_reconfig_complete->menb_ue_x2ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_x2ap_SgNBReconfigurationComplete_protocolIEs_2(
            pctxt,
            &p_x2ap_SgNBReconfigurationComplete->protocolIEs,
            p_sgnb_reconfig_complete->sgnb_ue_x2ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_x2ap_SgNBReconfigurationComplete_protocolIEs_3(
            pctxt,
            &p_x2ap_SgNBReconfigurationComplete->protocolIEs,
            &p_sgnb_reconfig_complete->resp_info_sgnb_reconfig_comp))
        {
            break;
        }

        if (p_sgnb_reconfig_complete->bitmask & X2AP_SGNB_ADD_REQ_ACK_MENB_UE_X2AP_ID_EXT_PRESENT)
        {
            if (RT_OK != asn1Append_x2ap_SgNBReconfigurationComplete_protocolIEs_4(
                pctxt,
                &p_x2ap_SgNBReconfigurationComplete->protocolIEs,
                p_sgnb_reconfig_complete->menb_ue_x2ap_id_ext))
            {
                break;
            }
        }

        /* ASN Encode message */
        pu_setBuffer(pctxt, p_asn_msg, X2AP_MAX_ASN1_BUF_LEN, ASN_X2AP_BUF_TYPE);

        if (RT_OK != asn1PE_x2ap_X2AP_PDU(pctxt, &x2ap_pdu))
        {
            rtxErrPrint(pctxt);
            X2AP_TRACE(X2AP_ERROR,"%s:ASN1 encoding failed.",__FUNCTION__);
            break;
        }
        else
        {
            *p_asn_msg_len = (U16)pe_GetMsgLen(pctxt);
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
            result=X2AP_SUCCESS;
        }
    } while(0);

    X2AP_UT_TRACE_EXIT();
    return result;
}
/*NR_DC Code Change End*/
/* MENB CHANGES - START */
/******************************************************************************
 * Function Name  : x2ap_compose_served_cell_dc
 * Inputs         : OSCTXT                   *p_asn1_ctx.
 *                  OSRTDList                *p_x2ap_IE_serv_cell
 *                  x2ap_srvd_cell_arry_t    *p_serv_cell
 *                  U16                      num_served_cell
 * Outputs        : OSRTDList                *p_x2ap_IE_serv_cell
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function populates p_x2ap_IE_serv_cell with
 *                  information necessary to encode served cell Info.
 *****************************************************************************/
x2ap_return_et
x2ap_compose_served_cell_dc
(
    OSCTXT                     *p_asn1_ctx,
    OSRTDList                  *p_x2ap_IE_serv_cell,
    en_dc_x2_setup_req_t       *p_x2_endc_setup_req
)
{
    x2ap_srvd_cell_arry_t      *p_serv_cell = PNULL;
    OSRTDListNode                                        *p_node_serv_cell = PNULL;
    x2ap_ServedEUTRAcellsENDCX2ManagementList_element    *p_servedCells_elem
        = X2AP_P_NULL;
    U16                                                  cell_count = X2AP_NULL;
    x2ap_return_et                                       retVal = X2AP_FAILURE;
    
    X2AP_UT_TRACE_ENTER();

    do
    {
        if ( !(p_x2_endc_setup_req->eutra_cell_info.num_served_cell) )
        {
            X2AP_TRACE(X2AP_ERROR, "%s:Num of supported served cell.",
                    __FUNCTION__);
            break;
        }

        for ( cell_count = X2AP_NULL; cell_count < (p_x2_endc_setup_req->
            eutra_cell_info.num_served_cell); cell_count++ )
        {
            p_serv_cell = &(p_x2_endc_setup_req->eutra_cell_info.served_cell_arr[cell_count]);
            rtxDListAllocNodeAndData(p_asn1_ctx,
                    x2ap_ServedEUTRAcellsENDCX2ManagementList_element,
                    &p_node_serv_cell,
                    &p_servedCells_elem);
            if ( X2AP_P_NULL == p_node_serv_cell )
            {
                X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
                break;
            }

            asn1Init_x2ap_ServedEUTRAcellsENDCX2ManagementList_element(
                p_servedCells_elem);

//hack
           p_serv_cell->served_cell_info.bitmask = 0;
            /*
             * Compose served cell Info
             */
            if ( X2AP_FAILURE == x2ap_compose_serv_cell_info(p_asn1_ctx,
                        &p_servedCells_elem->servedEUTRACellInfo,
                        p_serv_cell) )
            {
                X2AP_TRACE(X2AP_ERROR,
                        "%s:compose_serv_cell_info returned Failure.",
                        __FUNCTION__);
                break;
            } 
            /*
             * Populate NRNeighbour information
             */
            if ( X2_SETUP_NR_NEIGHBOUR_INFO_PRESENT & 
                    p_x2_endc_setup_req->eutra_cell_info.bitmask )
            {
                p_servedCells_elem->m.nrNeighbourInfoPresent = 1;
                if ( X2AP_FAILURE == x2ap_compose_nr_neighbour_info_dc(p_asn1_ctx,
                            &(p_servedCells_elem->nrNeighbourInfo),
                            p_x2_endc_setup_req->eutra_cell_info.nr_neigh_cell_arr,
                            p_x2_endc_setup_req->eutra_cell_info.num_neigh_cell) )
                {
                    X2AP_TRACE(X2AP_ERROR,
                            "%s:compose_NRNeighbou_cell returned Failure.",
                            __FUNCTION__);
                    rtFreeContext(p_asn1_ctx);
                    break;
                }
                else
                {
                    X2AP_TRACE(X2AP_DETAILED,
                            "compose_NRNeighbou_cell returned Success.");
                }

            }
            else
            { 
                X2AP_TRACE(X2AP_DETAILED,
                        "NR_NEIGHBOUR_INFO_PRESENT not present.");

            }

            rtxDListAppendNode(p_x2ap_IE_serv_cell, p_node_serv_cell);
            p_serv_cell = (x2ap_srvd_cell_arry_t *)((p_serv_cell) + 1);
            retVal = X2AP_SUCCESS;
        }
    }while(X2AP_NULL);

    X2AP_UT_TRACE_EXIT();
    return retVal;
}


/******************************************************************************
  * Function Name  : x2ap_compose_nr_neighbour_info_dc
  * Inputs         : OSCTXT                     *p_asn1_ctx.
  *                  OSRTDList                  *p_x2ap_IE_neigh_info
  *                  x2ap_nr_neigh_cell_arry_t  *p_serv_cell
  *                  U16                         num_neigh_cell
  * Outputs        : OSRTDList                  *p_x2ap_IE_neigh_info
  * Returns        : X2AP_SUCCESS/X2AP_FAILURE
  * Description    : This function populates p_x2ap_IE_neigh_cell with
                     information necessary to encode nrneighbour Info.
*****************************************************************************/
x2ap_return_et
x2ap_compose_nr_neighbour_info_dc
(
      OSCTXT                     *p_asn1_ctx,
      x2ap_NRNeighbour_Information *p_nr_neigh_elem,
      x2ap_nr_neigh_cell_arry_t   neigh_cell_arr[],
      U16                         num_neigh_cell
)
{
    x2ap_return_et    retVal = X2AP_FAILURE;

    X2AP_UT_TRACE_ENTER();

    do
    {
        if ( !num_neigh_cell )
        {
            X2AP_TRACE(X2AP_ERROR, "%s:Num of supported nr neighbour info.",
                    __FUNCTION__);
            break;
        }

        /*
         * Compose nr neighbour Info
         */
        if ( X2AP_FAILURE == x2ap_compose_nr_neighbour_info_list(p_asn1_ctx,
                    p_nr_neigh_elem,
                    neigh_cell_arr,num_neigh_cell) )
        {
            X2AP_TRACE(X2AP_ERROR,
                    "%s:compose_nr_neighbour_info returned Failure.",
                    __FUNCTION__);
            break;
        }
        retVal = X2AP_SUCCESS;
    }while(X2AP_NULL);

    X2AP_UT_TRACE_EXIT();
    return retVal;

}
/****************************************************************************
 *  Function Name:             x2ap_compose_nr_neighbour_info_list
 *  Description:               To compose the contents of 
 *                             x2ap_NRNeighbour_Information
 *  Return Type:               X2AP_SUCCESS
 *                             X2AP_FAILURE
 ****************************************************************************/
x2ap_return_et x2ap_compose_nr_neighbour_info_list
(
    OSCTXT                         *p_asn1_ctx,
    x2ap_NRNeighbour_Information   *p_x2ap_nr_neigh_info,
    x2ap_nr_neigh_cell_arry_t       neigh_cell_arr[],
    U16                             num_neigh_cell
)
{
    OSRTDListNode                                        *p_node_nr_neigh = PNULL;
    OSRTDListNode                                        *p_node_1 = PNULL;
    OSRTDListNode                                        *p_node_2 = PNULL;
    x2ap_NRNeighbour_Information_element                 *p_nr_neigh_elem = X2AP_P_NULL;
    x2ap_NRFreqInfo_freqBandListNr                       *p_nr_freq_band = X2AP_P_NULL;
    x2ap_FreqBandNrItem                                  *p_nr_freq_band_item = X2AP_P_NULL;
    x2ap_FreqBandNrItem_supportedSULBandList             *p_nr_freq_sul_band = X2AP_P_NULL;
    x2ap_SupportedSULFreqBandItem                        *p_nr_freq_sul_band_item = X2AP_P_NULL;
    U16                                                  cell_count = X2AP_NULL;
    U16                                                  freq_count = X2AP_NULL;
    U16                                                  sul_band_count = X2AP_NULL;

    x2ap_nr_neigh_cell_arry_t      *p_neigh_cell = NULL;

    X2AP_UT_TRACE_ENTER();

    for ( cell_count = X2AP_NULL; cell_count < num_neigh_cell; cell_count++ )
    {
        p_neigh_cell = &neigh_cell_arr[cell_count];
        
        rtxDListAllocNodeAndData(p_asn1_ctx,
                x2ap_NRNeighbour_Information_element,
                &p_node_nr_neigh,
                &p_nr_neigh_elem);
        if ( X2AP_P_NULL == p_node_nr_neigh )
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;

        }

        asn1Init_x2ap_NRNeighbour_Information_element(p_nr_neigh_elem);

        /*
         * Compose nr neighbour Info
         */
        /* Encode nrpCI */
        p_nr_neigh_elem->nrpCI = p_neigh_cell->nr_neigh_info.nr_pci;

        /* Encode nr Cell ID */
        x2ap_compose_nrcgi(&p_nr_neigh_elem->nrCellID, &p_neigh_cell->nr_neigh_info.nr_cell_id);
        
        /*Encode NR Frequency */
        p_nr_neigh_elem->nrFreqInfo.nRARFCN =
            p_neigh_cell->nr_neigh_info.nr_freq_info.nr_earfcn;

        /*Intializing the list of x2ap_NRFreqInfo_freqBandListNr*/ 
        p_nr_freq_band = &(p_nr_neigh_elem->nrFreqInfo.freqBandListNr);
        rtxDListInit(p_nr_freq_band);

        for( freq_count=X2AP_NULL; freq_count < p_neigh_cell->nr_neigh_info.
            nr_freq_info.num_freq_bands; freq_count++ )
        {
            rtxDListAllocNodeAndData(p_asn1_ctx, 
                    x2ap_FreqBandNrItem,
                    &p_node_1,
                    &p_nr_freq_band_item );
            if( X2AP_P_NULL == p_node_1 )
            {
                X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
                X2AP_UT_TRACE_EXIT();
                return X2AP_FAILURE;
            }

            asn1Init_x2ap_FreqBandNrItem(p_nr_freq_band_item);

            p_nr_freq_band_item->freqBandIndicatorNr = 
                p_neigh_cell->nr_neigh_info.nr_freq_info.freq_band_info[freq_count].
                freq_band_indicator_nr;

            /*Initailizing the list of x2ap_FreqBandNrItem_supportedSULBandList */
            p_nr_freq_sul_band = &(p_nr_freq_band_item->supportedSULBandList);
            rtxDListInit(p_nr_freq_sul_band);  

            for (sul_band_count = X2AP_NULL; sul_band_count <
                    p_neigh_cell->nr_neigh_info.nr_freq_info.freq_band_info[freq_count].
                    num_supported_sul_band;
                    sul_band_count++)
            {

                rtxDListAllocNodeAndData(p_asn1_ctx,
                        x2ap_SupportedSULFreqBandItem,
                        &p_node_2,
                        &p_nr_freq_sul_band_item );

                if( X2AP_P_NULL == p_node_2 )
                {
                    X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
                    X2AP_UT_TRACE_EXIT();
                    return X2AP_FAILURE;
                }

                asn1Init_x2ap_SupportedSULFreqBandItem(p_nr_freq_sul_band_item);

                p_nr_freq_sul_band_item->freqBandIndicatorNr = 
                    p_neigh_cell->nr_neigh_info.nr_freq_info.freq_band_info[freq_count].
                    supported_sul_freq_band_indicator_nr[sul_band_count] ;

                /* apending the x2ap_FreqBandNrItem_supportedSULBandList */
                rtxDListAppendNode(p_nr_freq_sul_band, p_node_2);
            }
            /*assigning the x2ap_FreqBandNrItem_supportedSULBandList to supportedSULBandList*/

            /*   apending the x2ap_NRFreqInfo_freqBandListNr */
            rtxDListAppendNode(p_nr_freq_band, p_node_1);

        }

        if ( X2AP_NR_ARFCN_SUL_PRESENT & p_neigh_cell->nr_neigh_info.nr_freq_info.bitmask )
        {
            p_nr_neigh_elem->nrFreqInfo.nRARFCN_SUL =
                p_neigh_cell->nr_neigh_info.nr_freq_info.nr_earfcn_sul;

        }
        if( X2AP_SUL_INFORMATION_PRESENT & p_neigh_cell->nr_neigh_info.nr_freq_info.bitmask )
        {
            p_nr_neigh_elem->nrFreqInfo.sULInformation.sUL_ARFCN =
                p_neigh_cell->nr_neigh_info.nr_freq_info.nr_sul_information.nr_sul_arfcn;

            p_nr_neigh_elem->nrFreqInfo.sULInformation.sUL_TxBW.nRSCS =
                p_neigh_cell->nr_neigh_info.nr_freq_info.nr_sul_information.
                nr_tx_band_width.nr_scs;
            p_nr_neigh_elem->nrFreqInfo.sULInformation.sUL_TxBW.nRNRB =
                p_neigh_cell->nr_neigh_info.nr_freq_info.nr_sul_information.
                nr_tx_band_width.nr_nrb;
        }

        /*Encode FiveGS TAC and Configured TAC */
        if ( X2AP_FIVE_GS_TAC_PRESENT & p_neigh_cell->nr_neigh_info.bitmask )
        {
            p_nr_neigh_elem->m.fiveGS_TACPresent = 1;
            p_nr_neigh_elem->fiveGS_TAC.numocts = 3;

            X2AP_MEMCPY(p_nr_neigh_elem->fiveGS_TAC.data,
                    p_neigh_cell->nr_neigh_info.fivegs_tac
                    .tac,3);
        }

        if( X2AP_CONFIGURED_TAC_PRESENT & p_neigh_cell->nr_neigh_info.bitmask )
        {
            p_nr_neigh_elem->m.configured_TACPresent = 1;
            p_nr_neigh_elem->configured_TAC.numocts = 2;
            X2AP_MEMCPY(p_nr_neigh_elem->configured_TAC.data,
                    p_neigh_cell->nr_neigh_info.configured_tac
                    .tac,2);
        }

#if 0
        p_nr_neigh_elem->measurementTimingConfiguration.numocts =
            p_neigh_cell->nr_neigh_info.meas_timing_config_buf_len;

        p_nr_neigh_elem->measurementTimingConfiguration.data = rtxMemAlloc(p_asn1_ctx,
                p_nr_neigh_elem->measurementTimingConfiguration.numocts);
        if (X2AP_P_NULL == p_nr_neigh_elem->measurementTimingConfiguration.data )
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            return X2AP_FAILURE;

        }
        X2AP_MEMCPY((char*)p_nr_neigh_elem->measurementTimingConfiguration.data,
                p_neigh_cell->nr_neigh_info.measurement_timing_config_asn_buff,
                p_nr_neigh_elem->measurementTimingConfiguration.numocts);
#endif
        /* Encode nRNeighbourModeInfo */
        if (X2AP_NR_NEIGHBOUR_FDD_PRESENT & p_neigh_cell->nr_neigh_info.nr_neigh_Mode_Info.bitmask)
        {

            p_nr_neigh_elem->nRNeighbourModeInfo.t =
                T_x2ap_NRNeighbour_Information_element_nRNeighbourModeInfo_fdd;
            p_nr_neigh_elem->nRNeighbourModeInfo.u.fdd = rtxMemAllocType(p_asn1_ctx,
                    x2ap_FDD_InfoNeighbourServedNRCell_Information);
            if (X2AP_P_NULL == p_nr_neigh_elem->nRNeighbourModeInfo.u.fdd )
            {
                X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
                return X2AP_FAILURE;

            }
            asn1Init_x2ap_FDD_InfoNeighbourServedNRCell_Information(p_nr_neigh_elem->
                nRNeighbourModeInfo.u.fdd);

            compose_x2ap_NR_Neigh_FDD_Info (
                    p_asn1_ctx,
                    p_nr_neigh_elem->nRNeighbourModeInfo.u.fdd,
                    &p_neigh_cell->nr_neigh_info.
                    nr_neigh_Mode_Info);
        }
        else if (X2AP_NR_NEIGHBOUR_TDD_PRESENT & p_neigh_cell->nr_neigh_info.
            nr_neigh_Mode_Info.bitmask)
        {
            p_nr_neigh_elem->nRNeighbourModeInfo.t =
                T_x2ap_NRNeighbour_Information_element_nRNeighbourModeInfo_tdd;

            p_nr_neigh_elem->nRNeighbourModeInfo.u.tdd = rtxMemAllocType(p_asn1_ctx,
                    x2ap_TDD_InfoNeighbourServedNRCell_Information);

            if (X2AP_P_NULL == p_nr_neigh_elem->nRNeighbourModeInfo.u.tdd )
            {
                X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
                return X2AP_FAILURE;

            }
            asn1Init_x2ap_TDD_InfoNeighbourServedNRCell_Information(p_nr_neigh_elem->
                nRNeighbourModeInfo.u.tdd);

            compose_x2ap_NR_Neigh_TDD_Info (
                    p_asn1_ctx, 
                    p_nr_neigh_elem->nRNeighbourModeInfo.u.tdd,
                    &p_neigh_cell->nr_neigh_info.
                    nr_neigh_Mode_Info);
        }
        else
        {
            /* eNB ID presence bitmask is not set */
            X2AP_TRACE(X2AP_ERROR,
                    "%s: neither FDD nor TDD mode"\
                    "Presence bitmask is set",__FUNCTION__);
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;

        }

        rtxDListAppendNode(p_x2ap_nr_neigh_info, p_node_nr_neigh);

    }

    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;

}

/******************************************************************************************
*  Function Name:             compose_x2ap_NR_Neigh_FDD_Info
*  Description:               To compose the contents of
*                             x2ap_FDD_InfoNeighbourServedNRCell_Information
*  Return Type:               X2AP_SUCCESS
*                             X2AP_FAILURE
*****************************************************************************************/
x2ap_return_et
compose_x2ap_NR_Neigh_FDD_Info
(
    OSCTXT                                              *p_asn1_ctx,
    x2ap_FDD_InfoNeighbourServedNRCell_Information      *p_tar_x2ap_nr_fdd_info,       
    x2ap_nr_neigh_mode_info_t                           *p_src_nr_neigh_mode
)
{

    OSRTDListNode                                        *p_node_1 = PNULL;
    OSRTDListNode                                        *p_node_2 = PNULL;
    OSRTDListNode                                        *p_node_1_dl = PNULL;
    OSRTDListNode                                        *p_node_2_dl = PNULL;
    x2ap_NRFreqInfo_freqBandListNr                       *p_nr_freq_band = X2AP_P_NULL;
    x2ap_FreqBandNrItem                                  *p_nr_freq_band_item = X2AP_P_NULL;
    x2ap_FreqBandNrItem_supportedSULBandList             *p_nr_freq_sul_band = X2AP_P_NULL;
    x2ap_SupportedSULFreqBandItem                        *p_nr_freq_sul_band_item = X2AP_P_NULL;

    x2ap_NRFreqInfo_freqBandListNr                       *p_nr_freq_band_dl = X2AP_P_NULL;
    x2ap_FreqBandNrItem                                  *p_nr_freq_band_item_dl = X2AP_P_NULL;
    x2ap_FreqBandNrItem_supportedSULBandList             *p_nr_freq_sul_band_dl = X2AP_P_NULL;
    x2ap_SupportedSULFreqBandItem                        *p_nr_freq_sul_band_item_dl = X2AP_P_NULL;

    U16                                                  freq_count = X2AP_NULL;
    U16                                                  sul_band_count = X2AP_NULL;
    X2AP_UT_TRACE_ENTER();


    /* Encode ul_NRFreqInfo */
    p_tar_x2ap_nr_fdd_info->ul_NRFreqInfo.nRARFCN =
        p_src_nr_neigh_mode->fdd_info.ul_nr_freq_Info.nr_earfcn;

    /*Intializing the list of x2ap_NRFreqInfo_freqBandListNr*/
    p_nr_freq_band = &(p_tar_x2ap_nr_fdd_info->ul_NRFreqInfo.freqBandListNr);
    rtxDListInit(p_nr_freq_band);

    for( freq_count=X2AP_NULL; freq_count < p_src_nr_neigh_mode->fdd_info.
        ul_nr_freq_Info.num_freq_bands; freq_count++ )
    {
        rtxDListAllocNodeAndData(p_asn1_ctx, 
                x2ap_FreqBandNrItem,
                &p_node_1,
                &p_nr_freq_band_item );
        if( X2AP_P_NULL == p_node_1 )
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            return X2AP_FAILURE;
        }

        asn1Init_x2ap_FreqBandNrItem(p_nr_freq_band_item);

        p_nr_freq_band_item->freqBandIndicatorNr = 
            p_src_nr_neigh_mode->fdd_info.ul_nr_freq_Info.freq_band_info[freq_count].
            freq_band_indicator_nr;

        p_nr_freq_sul_band = &(p_nr_freq_band_item->supportedSULBandList);
        /*Initailizing the list of x2ap_FreqBandNrItem_supportedSULBandList */
        rtxDListInit(p_nr_freq_sul_band);  

        for (sul_band_count = X2AP_NULL; sul_band_count <
                p_src_nr_neigh_mode->fdd_info.ul_nr_freq_Info.freq_band_info[freq_count].
                num_supported_sul_band;
                sul_band_count++)
        {

            rtxDListAllocNodeAndData(p_asn1_ctx,
                    x2ap_SupportedSULFreqBandItem,
                    &p_node_2,
                    &p_nr_freq_sul_band_item );

            if( X2AP_P_NULL == p_node_2 )
            {
                X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            return X2AP_FAILURE;
            }

            asn1Init_x2ap_SupportedSULFreqBandItem(p_nr_freq_sul_band_item);

            p_nr_freq_sul_band_item->freqBandIndicatorNr = 
                p_src_nr_neigh_mode->fdd_info.ul_nr_freq_Info.freq_band_info[freq_count].
                supported_sul_freq_band_indicator_nr[sul_band_count] ;

            /* apending the x2ap_FreqBandNrItem_supportedSULBandList */
            rtxDListAppendNode(p_nr_freq_sul_band, p_node_2);
        }
        /*   apending the x2ap_NRFreqInfo_freqBandListNr */
        rtxDListAppendNode(p_nr_freq_band, p_node_1);

    }
    if ( X2AP_NR_ARFCN_SUL_PRESENT & p_src_nr_neigh_mode->fdd_info.ul_nr_freq_Info.bitmask )
    {
        p_tar_x2ap_nr_fdd_info->ul_NRFreqInfo.nRARFCN_SUL =
            p_src_nr_neigh_mode->fdd_info.ul_nr_freq_Info.nr_earfcn_sul;

    }
    if( X2AP_SUL_INFORMATION_PRESENT & p_src_nr_neigh_mode->fdd_info.ul_nr_freq_Info.bitmask )
    {
        p_tar_x2ap_nr_fdd_info->ul_NRFreqInfo.sULInformation.sUL_ARFCN =
            p_src_nr_neigh_mode->fdd_info.ul_nr_freq_Info.nr_sul_information.nr_sul_arfcn;

        p_tar_x2ap_nr_fdd_info->ul_NRFreqInfo.sULInformation.sUL_TxBW.nRSCS =
            p_src_nr_neigh_mode->fdd_info.ul_nr_freq_Info.nr_sul_information.nr_tx_band_width.nr_scs;
        p_tar_x2ap_nr_fdd_info->ul_NRFreqInfo.sULInformation.sUL_TxBW.nRNRB =
            p_src_nr_neigh_mode->fdd_info.ul_nr_freq_Info.nr_sul_information.nr_tx_band_width.nr_nrb;

    }

    /* Encode  dl_NRFreqInfo */
    p_tar_x2ap_nr_fdd_info->dl_NRFreqInfo.nRARFCN =
        p_src_nr_neigh_mode->fdd_info.dl_nr_freq_Info.nr_earfcn;

    /*Intializing the list of x2ap_NRFreqInfo_freqBandListNr*/
    p_nr_freq_band_dl = &(p_tar_x2ap_nr_fdd_info->dl_NRFreqInfo.freqBandListNr);
    rtxDListInit(p_nr_freq_band_dl);

    for( freq_count=X2AP_NULL; freq_count < p_src_nr_neigh_mode->fdd_info.dl_nr_freq_Info.num_freq_bands; freq_count++ )
    {
        rtxDListAllocNodeAndData(p_asn1_ctx, 
                x2ap_FreqBandNrItem,
                &p_node_1_dl,
                &p_nr_freq_band_item_dl );
        if( X2AP_P_NULL == p_node_1_dl )
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            return X2AP_FAILURE;
        }

        asn1Init_x2ap_FreqBandNrItem(p_nr_freq_band_item_dl);

        p_nr_freq_band_item_dl->freqBandIndicatorNr = 
            p_src_nr_neigh_mode->fdd_info.dl_nr_freq_Info.freq_band_info[freq_count].freq_band_indicator_nr;

        /*Initailizing the list of x2ap_FreqBandNrItem_supportedSULBandList */
        p_nr_freq_sul_band_dl = &(p_nr_freq_band_item_dl->supportedSULBandList);
        rtxDListInit(p_nr_freq_sul_band_dl);  

        for (sul_band_count = X2AP_NULL; sul_band_count <
                p_src_nr_neigh_mode->fdd_info.dl_nr_freq_Info.freq_band_info[freq_count].num_supported_sul_band;
                sul_band_count++)
        {

            rtxDListAllocNodeAndData(p_asn1_ctx,
                    x2ap_SupportedSULFreqBandItem,
                    &p_node_2_dl,
                    &p_nr_freq_sul_band_item_dl);
            /* coverity_280044_fix_start*/
            if( X2AP_P_NULL == p_node_2_dl )
            /* coverity_280044_fix_end*/
            {
                X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            return X2AP_FAILURE;
            }

            asn1Init_x2ap_SupportedSULFreqBandItem(p_nr_freq_sul_band_item_dl);

            p_nr_freq_sul_band_item_dl->freqBandIndicatorNr = 
                p_src_nr_neigh_mode->fdd_info.dl_nr_freq_Info.freq_band_info[freq_count].
                supported_sul_freq_band_indicator_nr[sul_band_count] ;

            /* apending the x2ap_FreqBandNrItem_supportedSULBandList */
            rtxDListAppendNode(p_nr_freq_sul_band_dl, p_node_2_dl);
        }
        /*assigning the x2ap_FreqBandNrItem_supportedSULBandList to supportedSULBandList*/
        /*   apending the x2ap_NRFreqInfo_freqBandListNr */
        rtxDListAppendNode(p_nr_freq_band_dl, p_node_1_dl);

    }
    if ( X2AP_NR_ARFCN_SUL_PRESENT & p_src_nr_neigh_mode->fdd_info.dl_nr_freq_Info.bitmask )
    {
        p_tar_x2ap_nr_fdd_info->dl_NRFreqInfo.nRARFCN_SUL =
            p_src_nr_neigh_mode->fdd_info.dl_nr_freq_Info.nr_earfcn_sul;

    }

    if( X2AP_SUL_INFORMATION_PRESENT & p_src_nr_neigh_mode->fdd_info.dl_nr_freq_Info.bitmask )
    {
        p_tar_x2ap_nr_fdd_info->dl_NRFreqInfo.sULInformation.sUL_ARFCN =
            p_src_nr_neigh_mode->fdd_info.dl_nr_freq_Info.nr_sul_information.nr_sul_arfcn;

        p_tar_x2ap_nr_fdd_info->dl_NRFreqInfo.sULInformation.sUL_TxBW.nRSCS =
            p_src_nr_neigh_mode->fdd_info.dl_nr_freq_Info.nr_sul_information.nr_tx_band_width.nr_scs;
        p_tar_x2ap_nr_fdd_info->dl_NRFreqInfo.sULInformation.sUL_TxBW.nRNRB =
            p_src_nr_neigh_mode->fdd_info.dl_nr_freq_Info.nr_sul_information.nr_tx_band_width.nr_nrb;

    }

    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;

}
/****************************************************************************************************
*  Function Name:             compose_x2ap_NR_Neigh_TDD_Info
*  Description:               To compose the contents of
*                             x2ap_TDD_InfoNeighbourServedNRCell_Information
*  Return Type:               X2AP_SUCCESS
*                             X2AP_FAILURE 
*
*************************************************************************************************/
x2ap_return_et
compose_x2ap_NR_Neigh_TDD_Info(
        OSCTXT                                              *p_asn1_ctx,
        x2ap_TDD_InfoNeighbourServedNRCell_Information      *p_tar_x2ap_nr_tdd_info,
        x2ap_nr_neigh_mode_info_t                           *p_src_nr_neigh_mode
        )
{
    OSRTDListNode                                        *p_node_1 = PNULL;
    OSRTDListNode                                        *p_node_2 = PNULL;
    x2ap_NRFreqInfo_freqBandListNr                       *p_nr_freq_band = X2AP_P_NULL;
    x2ap_FreqBandNrItem                                  *p_nr_freq_band_item = X2AP_P_NULL;
    x2ap_FreqBandNrItem_supportedSULBandList             *p_nr_freq_sul_band = X2AP_P_NULL;
    x2ap_SupportedSULFreqBandItem                        *p_nr_freq_sul_band_item = X2AP_P_NULL;

    U16                                                  freq_count = X2AP_NULL;
    U16                                                  sul_band_count = X2AP_NULL;
    X2AP_UT_TRACE_ENTER();


    /* Encode nRFreqInfo */
    p_tar_x2ap_nr_tdd_info->nRFreqInfo.nRARFCN =
        p_src_nr_neigh_mode->tdd_info.nr_freq_info.nr_earfcn;

    /*Intializing the list of x2ap_NRFreqInfo_freqBandListNr*/
/* TB_FIX2_START */
    p_nr_freq_band = &(p_tar_x2ap_nr_tdd_info->nRFreqInfo.freqBandListNr);
/* TB_FIX2_END */
    rtxDListInit(p_nr_freq_band);

    for( freq_count=X2AP_NULL; freq_count < p_src_nr_neigh_mode->tdd_info.nr_freq_info.num_freq_bands; freq_count++ )
    {
        rtxDListAllocNodeAndData(p_asn1_ctx, 
                x2ap_FreqBandNrItem,
                &p_node_1,
                &p_nr_freq_band_item );
        if( X2AP_P_NULL == p_node_1 )
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            return X2AP_FAILURE;
        }

        asn1Init_x2ap_FreqBandNrItem(p_nr_freq_band_item);

        p_nr_freq_band_item->freqBandIndicatorNr = 
            p_src_nr_neigh_mode->tdd_info.nr_freq_info.freq_band_info[freq_count].freq_band_indicator_nr;

        /*Initailizing the list of x2ap_FreqBandNrItem_supportedSULBandList */
/* TB_FIX2_START */
        p_nr_freq_sul_band = &(p_nr_freq_band_item->supportedSULBandList);
        rtxDListInit(p_nr_freq_sul_band);  
/* TB_FIX2_END */

        for (sul_band_count = X2AP_NULL; sul_band_count <
                p_src_nr_neigh_mode->tdd_info.nr_freq_info.freq_band_info[freq_count].num_supported_sul_band;
                sul_band_count++)
        {

            rtxDListAllocNodeAndData(p_asn1_ctx,
                    x2ap_SupportedSULFreqBandItem,
                    &p_node_2,
                    &p_nr_freq_sul_band_item );

            if( X2AP_P_NULL == p_node_2 )
            {
                X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            return X2AP_FAILURE;
            }

            asn1Init_x2ap_SupportedSULFreqBandItem(p_nr_freq_sul_band_item);

            p_nr_freq_sul_band_item->freqBandIndicatorNr = 
                p_src_nr_neigh_mode->tdd_info.nr_freq_info.freq_band_info[freq_count].
                supported_sul_freq_band_indicator_nr[sul_band_count] ;

            /* apending the x2ap_FreqBandNrItem_supportedSULBandList */
            rtxDListAppendNode(p_nr_freq_sul_band, p_node_2);
        }
        /*   apending the x2ap_NRFreqInfo_freqBandListNr */
        rtxDListAppendNode(p_nr_freq_band, p_node_1);

    }
    if ( X2AP_NR_ARFCN_SUL_PRESENT & p_src_nr_neigh_mode->tdd_info.nr_freq_info.bitmask )
    {
        p_tar_x2ap_nr_tdd_info->nRFreqInfo.nRARFCN_SUL =
            p_src_nr_neigh_mode->tdd_info.nr_freq_info.nr_earfcn_sul;

    }
    if( X2AP_SUL_INFORMATION_PRESENT & p_src_nr_neigh_mode->tdd_info.nr_freq_info.bitmask )
    {
        p_tar_x2ap_nr_tdd_info->nRFreqInfo.sULInformation.sUL_ARFCN =
            p_src_nr_neigh_mode->tdd_info.nr_freq_info.nr_sul_information.nr_sul_arfcn;

        p_tar_x2ap_nr_tdd_info->nRFreqInfo.sULInformation.sUL_TxBW.nRSCS =
            p_src_nr_neigh_mode->tdd_info.nr_freq_info.nr_sul_information.nr_tx_band_width.nr_scs;
        p_tar_x2ap_nr_tdd_info->nRFreqInfo.sULInformation.sUL_TxBW.nRNRB =
            p_src_nr_neigh_mode->tdd_info.nr_freq_info.nr_sul_information.nr_tx_band_width.nr_nrb;

    }
 
   X2AP_UT_TRACE_EXIT();
   return X2AP_SUCCESS;

}



/*****************************************************************************
* FUNCTION NAME  : x2ap_encode_endc_x2setup_req
* INPUT          : x2_setup_req_t       *p_x2_endc_setup_req
*                  U8                   *p_asn_msg
*                  U16                  *p_asn_msg_len
* OUTPUT         : U8                   *p_asn_msg
*                  U16                  *p_asn_msg_len
* DESCRIPTION    : This function encode the EN DC X2 setup request
* RETURNS        : X2AP_SUCCESS - if successfully encoded the message
*                  X2AP_FAILURE - if fails to encode the message
*******************************************************************************/
x2ap_return_et
x2ap_encode_endc_x2setup_req
(
    en_dc_x2_setup_req_t    *p_x2_endc_setup_req,
    U8                      *p_asn_msg,
    U16                     *p_asn_msg_len
)
{
    x2ap_return_et             retVal = X2AP_FAILURE;
    OSCTXT                     asn1_ctx;
    x2ap_X2AP_PDU              x2ap_pdu;
    OSRTDListNode              *p_node = PNULL;
    x2ap_ENDCX2SetupRequest    *p_endc_X2Setup_asn_msg = X2AP_P_NULL;
    x2ap_ENDCX2SetupRequest_protocolIEs_element
        *p_endc_protocolIE_elem = PNULL;
    x2ap_InitiatingNodeType_EndcX2Setup_init_eNB_element
        *p_endc_x2setup_init_enb_elem = PNULL;
    x2ap_InitiatingNodeType_EndcX2Setup_init_eNB
        *p_endc_x2setup_init_enb = PNULL;
    OSRTDListNode*  p_elem_node  = PNULL;

    X2AP_UT_TRACE_ENTER();

    /*
     * Init ASN1 context
     */
    if ( X2AP_NULL != rtInitContext(&asn1_ctx) )
    {
        X2AP_TRACE(X2AP_ERROR,"%s:ASN context initialization failed !!!",
                __FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return retVal;
    }

    do
    {
        X2AP_MEMSET(&(x2ap_pdu), X2AP_NULL, sizeof(x2ap_X2AP_PDU));

        /*
         * Fill the values in the ASN structures that shall be encoded by
         * ASN Encoder
         */

        /*
         * Set Pdu type to Initiating message
         */

        x2ap_pdu.t = T_x2ap_X2AP_PDU_initiatingMessage;

        x2ap_pdu.u.initiatingMessage = rtxMemAllocType(&asn1_ctx,
                x2ap_InitiatingMessage);

        if ( X2AP_P_NULL == x2ap_pdu.u.initiatingMessage )
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        asn1Init_x2ap_InitiatingMessage(x2ap_pdu.u.initiatingMessage);

        /*
         * Fill procedure code
         */
        x2ap_pdu.u.initiatingMessage->procedureCode =
            ASN1V_x2ap_id_endcX2Setup;

        /*
         * Fill criticality of message type
         */
        x2ap_pdu.u.initiatingMessage->criticality = x2ap_reject;

        /*
         * Set the initiating message type to ENDC X2 Setup
         */
        x2ap_pdu.u.initiatingMessage->value.t = T1x2ap__endcX2Setup;

        p_endc_X2Setup_asn_msg = rtxMemAllocType(&asn1_ctx,
                x2ap_ENDCX2SetupRequest);
        if ( X2AP_P_NULL == p_endc_X2Setup_asn_msg )
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        else
        {
            X2AP_TRACE(X2AP_DETAILED,"Mem Alloc successful to "
                "_x2ap_ENDCX2SetupRequest.\n");
        }

        asn1Init_x2ap_ENDCX2SetupRequest(p_endc_X2Setup_asn_msg);

        X2AP_TRACE(X2AP_DETAILED,"asn1Init_x2ap_ENDCX2SetupRequest sucecsful\n");

        x2ap_pdu.u.initiatingMessage->value.u.endcX2Setup = p_endc_X2Setup_asn_msg;

        /*
         * Initialize list of protocol IEs
         */
        asn1Init_x2ap_ENDCX2SetupRequest_protocolIEs(
            &p_endc_X2Setup_asn_msg->protocolIEs);
        /*
         * Allocate memory for target protocolIE element
         */
        rtxDListAllocNodeAndData(&asn1_ctx,
            x2ap_ENDCX2SetupRequest_protocolIEs_element,
            &p_node,
            &p_endc_protocolIE_elem);

        if ( X2AP_P_NULL == p_node )
        {
            /*
             * Not enough memory
             */
            X2AP_TRACE(X2AP_ERROR,  "%s:ASN malloc failed !!!",__FUNCTION__);
            break;
        }

//////////////////////////////////
        /*
         *  Compose id-Global-ENB-ID
         */
        asn1Init_x2ap_ENDCX2SetupRequest_protocolIEs_element(
            p_endc_protocolIE_elem);

        /*
         * Fill protocol IE Id and criticality
         */
        p_endc_protocolIE_elem->id = ASN1V_x2ap_id_InitiatingNodeType_EndcX2Setup;
        p_endc_protocolIE_elem->criticality = x2ap_reject;

        /*
         * Fill the type of Protocol IE element
         */
        p_endc_protocolIE_elem->value.t = T200x2ap___x2ap_ENDCX2SetupRequest_IEs_1;

        p_endc_protocolIE_elem->value.u._x2ap_ENDCX2SetupRequest_IEs_1
            = rtxMemAllocType(&asn1_ctx,x2ap_InitiatingNodeType_EndcX2Setup);

        if ( X2AP_P_NULL == p_endc_protocolIE_elem->value.u.
            _x2ap_ENDCX2SetupRequest_IEs_1 )
        {
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed!!!",__FUNCTION__);
            break;
        }

        asn1Init_x2ap_InitiatingNodeType_EndcX2Setup(p_endc_protocolIE_elem->
            value.u._x2ap_ENDCX2SetupRequest_IEs_1);

        X2AP_TRACE(X2AP_DETAILED, "Populating "
            "T_x2ap_InitiatingNodeType_EndcX2Setup_init_eNB\n");

        p_endc_protocolIE_elem->value.u._x2ap_ENDCX2SetupRequest_IEs_1->t
            = T_x2ap_InitiatingNodeType_EndcX2Setup_init_eNB; 

        p_endc_protocolIE_elem->value.u._x2ap_ENDCX2SetupRequest_IEs_1->u.
            init_eNB = rtxMemAllocType(&asn1_ctx,
            x2ap_InitiatingNodeType_EndcX2Setup_init_eNB);

        if ( X2AP_P_NULL == p_endc_protocolIE_elem->value.u.
            _x2ap_ENDCX2SetupRequest_IEs_1->u.init_eNB )
        {
            X2AP_TRACE(X2AP_ERROR, "ASN Malloc Failed \n");
            break;
        }

        asn1Init_x2ap_InitiatingNodeType_EndcX2Setup_init_eNB(
            p_endc_protocolIE_elem->value.u.
            _x2ap_ENDCX2SetupRequest_IEs_1->u.init_eNB);

        p_endc_x2setup_init_enb = p_endc_protocolIE_elem->value.u.
            _x2ap_ENDCX2SetupRequest_IEs_1->u.init_eNB;

        rtxDListAllocNodeAndData(&asn1_ctx,
                    x2ap_InitiatingNodeType_EndcX2Setup_init_eNB_element,
                    &p_elem_node,
                    &p_endc_x2setup_init_enb_elem);
        if ( PNULL == p_elem_node )
        {
            /*
             * Not enough memory
             */
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed!!!!",
                    __FUNCTION__);
            break;
        }

        asn1Init_x2ap_InitiatingNodeType_EndcX2Setup_init_eNB_element(
            p_endc_x2setup_init_enb_elem);

        X2AP_TRACE(X2AP_DETAILED, "Populating p_endc_x2setup_init_enb_elem\n");

        p_endc_x2setup_init_enb_elem->id = ASN1V_x2ap_id_GlobalENB_ID;
        p_endc_x2setup_init_enb_elem->criticality = x2ap_reject;

        X2AP_TRACE(X2AP_DETAILED, "src_endc_x2setup_init_enb_elem->id[%x]",
             p_endc_x2setup_init_enb_elem->id);

        X2AP_TRACE(X2AP_DETAILED, "Encoding eNB-IDs\n");

        p_endc_x2setup_init_enb_elem->value.t =
            T201x2ap___x2ap_ENB_ENDCX2SetupReqIEs_1;

        p_endc_x2setup_init_enb_elem->value.u._x2ap_ENB_ENDCX2SetupReqIEs_1 =
            rtxMemAllocType(&asn1_ctx,x2ap_GlobalENB_ID);

        if ( PNULL == p_endc_x2setup_init_enb_elem->value.u.
             _x2ap_ENB_ENDCX2SetupReqIEs_1 )
        {
            X2AP_TRACE(X2AP_ERROR,"ASN malloc failed !!!!\n");
            break;
        }

        asn1Init_x2ap_GlobalENB_ID(p_endc_x2setup_init_enb_elem->
            value.u._x2ap_ENB_ENDCX2SetupReqIEs_1);

        /*
         * Filling global_plmn id and global enb id
         */
        if ( X2AP_FAILURE == x2ap_compose_global_enb_id(&asn1_ctx,
            p_endc_x2setup_init_enb_elem-> value.
            u._x2ap_ENB_ENDCX2SetupReqIEs_1,
            &p_x2_endc_setup_req->eutra_cell_info.global_enb_id) )
        {
            X2AP_TRACE(X2AP_ERROR,
                       "%s:compose id-Global-ENB-ID returned Failure.",
                       __FUNCTION__);
            rtFreeContext(&asn1_ctx);
            break;
        }
        else
        {
            X2AP_TRACE(X2AP_DETAILED,
                "compose id-Global-ENB-ID returned Success.");
        }
        rtxDListAppendNode(p_endc_x2setup_init_enb, p_elem_node);
        rtxDListAppendNode(&p_endc_X2Setup_asn_msg->protocolIEs, p_node);

/////////////////////
        OSRTDListNode              *p_elem_node_1 = PNULL;
        /*
         * Compose id-ServedCells
         */
        X2AP_TRACE(X2AP_DETAILED,"Encoding Served Cells \n");

        /*
         * Allocate memory for target protocolIE element
         */
        rtxDListAllocNodeAndData(&asn1_ctx,
            x2ap_ENDCX2SetupRequest_protocolIEs_element,
            &p_node,
            &p_endc_protocolIE_elem);

        if ( X2AP_P_NULL == p_node )
        {
            /*
             * Not enough memory
             */
            X2AP_TRACE(X2AP_ERROR,  "%s:ASN malloc failed !!!",__FUNCTION__);
            break;
        }

        asn1Init_x2ap_ENDCX2SetupRequest_protocolIEs_element(
            p_endc_protocolIE_elem);

        /*
         * Fill protocol IE Id and criticality
         */
        p_endc_protocolIE_elem->id = ASN1V_x2ap_id_InitiatingNodeType_EndcX2Setup;
        p_endc_protocolIE_elem->criticality = x2ap_reject;

        /*
         * Fill the type of Protocol IE element
         */
        p_endc_protocolIE_elem->value.t = T200x2ap___x2ap_ENDCX2SetupRequest_IEs_1;

        p_endc_protocolIE_elem->value.u._x2ap_ENDCX2SetupRequest_IEs_1
            = rtxMemAllocType(&asn1_ctx,x2ap_InitiatingNodeType_EndcX2Setup);

        if ( X2AP_P_NULL == p_endc_protocolIE_elem->value.u.
            _x2ap_ENDCX2SetupRequest_IEs_1 )
        {
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed!!!",__FUNCTION__);
            break;
        }

        asn1Init_x2ap_InitiatingNodeType_EndcX2Setup(p_endc_protocolIE_elem->
            value.u._x2ap_ENDCX2SetupRequest_IEs_1);

        X2AP_TRACE(X2AP_DETAILED, "Populating "
            "T_x2ap_InitiatingNodeType_EndcX2Setup_init_eNB\n");

        p_endc_protocolIE_elem->value.u._x2ap_ENDCX2SetupRequest_IEs_1->t
            = T_x2ap_InitiatingNodeType_EndcX2Setup_init_eNB; 

        p_endc_protocolIE_elem->value.u._x2ap_ENDCX2SetupRequest_IEs_1->u.
            init_eNB = rtxMemAllocType(&asn1_ctx,
            x2ap_InitiatingNodeType_EndcX2Setup_init_eNB);

        if ( X2AP_P_NULL == p_endc_protocolIE_elem->value.u.
            _x2ap_ENDCX2SetupRequest_IEs_1->u.init_eNB )
        {
            X2AP_TRACE(X2AP_ERROR, "ASN Malloc Failed \n");
            break;
        }

        asn1Init_x2ap_InitiatingNodeType_EndcX2Setup_init_eNB(
            p_endc_protocolIE_elem->value.u.
            _x2ap_ENDCX2SetupRequest_IEs_1->u.init_eNB);

        p_endc_x2setup_init_enb = p_endc_protocolIE_elem->value.u.
            _x2ap_ENDCX2SetupRequest_IEs_1->u.init_eNB;

        rtxDListAllocNodeAndData(&asn1_ctx,
                    x2ap_InitiatingNodeType_EndcX2Setup_init_eNB_element,
                    &p_elem_node_1,
                    &p_endc_x2setup_init_enb_elem);
        if ( PNULL == p_elem_node_1 )
        {
            /*
             * Not enough memory
             */
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed!!!!",
                    __FUNCTION__);
            break;
        }

        asn1Init_x2ap_InitiatingNodeType_EndcX2Setup_init_eNB_element(
            p_endc_x2setup_init_enb_elem);

        p_endc_x2setup_init_enb_elem->id =
            ASN1V_x2ap_id_ServedEUTRAcellsENDCX2ManagementList;
        p_endc_x2setup_init_enb_elem->criticality = x2ap_reject;

        /*
         * Fill the type of Protocol IE element
         */
        p_endc_x2setup_init_enb_elem->value.t = T201x2ap___x2ap_ENB_ENDCX2SetupReqIEs_2; 

        p_endc_x2setup_init_enb_elem->value.u._x2ap_ENB_ENDCX2SetupReqIEs_2
            = rtxMemAllocType(&asn1_ctx,
            x2ap_ServedEUTRAcellsENDCX2ManagementList);

        if ( PNULL == p_endc_x2setup_init_enb_elem->value.u.
             _x2ap_ENB_ENDCX2SetupReqIEs_2 )
        {
            X2AP_TRACE(X2AP_ERROR,"ASN malloc Failed\n");
            return X2AP_FAILURE;
        }

        asn1Init_x2ap_ServedEUTRAcellsENDCX2ManagementList(
            p_endc_x2setup_init_enb_elem->value.u._x2ap_ENB_ENDCX2SetupReqIEs_2);

        /*
         * Populate served cell information
         */
        if ( X2AP_FAILURE == x2ap_compose_served_cell_dc(&asn1_ctx,
                              p_endc_x2setup_init_enb_elem->value.u.
                              _x2ap_ENB_ENDCX2SetupReqIEs_2,
                              (en_dc_x2_setup_req_t *)p_x2_endc_setup_req) )
        {
            X2AP_TRACE(X2AP_ERROR,
                       "%s:compose_served_cell returned Failure.",
                       __FUNCTION__);
            rtFreeContext(&asn1_ctx);
            break;
        }
        else
        {
            X2AP_TRACE(X2AP_DETAILED,
                       "compose_served_cell returned Success.");
        }

        
        rtxDListAppendNode(p_endc_x2setup_init_enb, p_elem_node_1);
        rtxDListAppendNode(&p_endc_X2Setup_asn_msg->protocolIEs, p_node);

        /*
         * ASN Encode message
         */
        pu_setBuffer(&asn1_ctx, p_asn_msg,
                X2AP_MAX_ASN1_BUF_LEN, TRUE);

        if ( X2AP_NULL != asn1PE_x2ap_X2AP_PDU(&asn1_ctx, &x2ap_pdu) )
        {
            X2AP_TRACE(X2AP_ERROR, "ASN1 encoding of SeNB Addition failed.");
            rtxErrPrint(&asn1_ctx);
            rtFreeContext(&asn1_ctx);
            break;
        }
        else
        {
            *p_asn_msg_len = (U16)pe_GetMsgLen(&asn1_ctx);
            retVal = X2AP_SUCCESS;
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
        }
    }while(X2AP_NULL);

    /*
     * Free ASN1 context
     */
    rtFreeContext(&asn1_ctx);
    X2AP_UT_TRACE_EXIT();
    return retVal;
}
/* MENB CHANGES - END */

/*EN-DC_changes_start*/
/*******************************************************************************
 * FUNCTION NAME : x2ap_internal_encode_sgnb_release_request
 * INPUTS        : pctxt - poiinter to OSCTXT
 *                 p_buff - pointer to ASN.1 encoded message
 *                 p_buff_size - size of result buffer
 * OUTPUTS     : p_buff (ASN.1 encoded message),
 *               p_buff_size (size of result buffer)
 * RETURNS     : X2AP_SUCCESS / X2AP_FAILURE    
 * DESCRIPTION   : This function used to encode SgNB release req.
 *******************************************************************************/
x2ap_return_et x2ap_internal_encode_sgnb_release_request
(
    OSCTXT      *pctxt,         /* for memory allocation */
    U8          *p_buff,        /* ASN.1 encoded message */
    U16         *p_buff_size,   /* size of result buffer */
    /* source */
    rrc_x2ap_sgnb_release_request_t  *p_sgnb_rel_request
)
{
    x2ap_return_et                      result = X2AP_FAILURE;
    x2ap_X2AP_PDU                       x2ap_pdu;
    x2ap_SgNBReleaseRequest            *p_x2ap_msg = PNULL;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != pctxt);
    X2AP_ASSERT(PNULL != p_buff);
    X2AP_ASSERT(PNULL != p_buff_size);
    X2AP_ASSERT(PNULL != p_sgnb_rel_request);

    do
    {
        /* Init X2AP PDU */
        asn1Init_x2ap_X2AP_PDU(&x2ap_pdu);
        /* Set Pdu type to Initiating Message */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_initiatingMessage;

        x2ap_pdu.u.initiatingMessage =
            rtxMemAllocType(pctxt, x2ap_InitiatingMessage);
        if (PNULL == x2ap_pdu.u.initiatingMessage)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        asn1Init_x2ap_InitiatingMessage(x2ap_pdu.u.initiatingMessage);

        x2ap_pdu.u.initiatingMessage->procedureCode =
            ASN1V_x2ap_id_meNBinitiatedSgNBRelease;
         
        x2ap_pdu.u.initiatingMessage->criticality = x2ap_notify;
        
        x2ap_pdu.u.initiatingMessage->value.t = 
            T1x2ap__meNBinitiatedSgNBRelease;

        x2ap_pdu.u.initiatingMessage->value.u.
            meNBinitiatedSgNBRelease = 
            rtxMemAllocType(pctxt,x2ap_SgNBReleaseRequest);
        if (PNULL == x2ap_pdu.u.initiatingMessage->value.u.
                meNBinitiatedSgNBRelease)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        p_x2ap_msg = x2ap_pdu.u.initiatingMessage->value.u.
                            meNBinitiatedSgNBRelease;
        
        asn1Init_x2ap_SgNBReleaseRequest(p_x2ap_msg);

        if (RT_OK != asn1Append_x2ap_SgNBReleaseRequest_protocolIEs_1(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            p_sgnb_rel_request->menb_ue_x2ap_id))
        {
            break;
        }

/*NR_DC Code Change Start*/
        if (X2AP_SGNB_REL_REQ_SGNB_UE_X2AP_ID_PRESENT &
                p_sgnb_rel_request->bitmask)
        {
            if (RT_OK != asn1Append_x2ap_SgNBReleaseRequest_protocolIEs_2(
                        pctxt,
                        &p_x2ap_msg->protocolIEs,
                        p_sgnb_rel_request->sgnb_ue_x2ap_id))
            {
                break;
            }
        }
/*NR_DC Code Change Stop*/

        if (RT_OK != asn1Append_x2ap_SgNBReleaseRequest_protocolIEs_3(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            &p_sgnb_rel_request->cause))
        {
            break;
        }

#if 0
        if (X2AP_SGNB_REL_REQ_ERB_REL_LIST_PRESENT &
                p_sgnb_rel_request->bitmask)
        {
            if (RT_OK != asn1Append_x2ap_SgNBReleaseRequest_protocolIEs_4(
                        pctxt,
                        &p_x2ap_msg->protocolIEs,
                        &p_sgnb_rel_request->erab_to_release_list))
            {
                X2AP_TRACE(X2AP_DETAILED,"%s:ASN protocolIEs_4.",__FUNCTION__);
                break;
            }
        }
#endif
        /* ASN Encode message */
        pu_setBuffer(pctxt, p_buff, X2AP_MAX_ASN1_BUF_LEN, ASN_X2AP_BUF_TYPE);
        
        if (RT_OK != asn1PE_x2ap_X2AP_PDU(pctxt, &x2ap_pdu))
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN1 encoding failed.",__FUNCTION__);
            /*ASN DEBUG */
            char buff[500];
            rtxErrGetTextBuf(pctxt,buff ,500);
#ifdef __x86_64__
            X2AP_TRACE(X2AP_ERROR, "BUFFER[%s] %p\n",(char*)buff, buff);
#else
            X2AP_TRACE(X2AP_ERROR, "BUFFER[%s] %x\n",(char*)buff,(unsigned int)buff);
#endif 
            break;
        }
        rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
        
        *p_buff_size = (U16)pe_GetMsgLen(pctxt);
        
        result=X2AP_SUCCESS;
    } while(0);
    X2AP_UT_TRACE_EXIT();
    return result;
}

/*****************************************************************************
 * Function Name  : x2ap_encode_endc_x2setup_failure_resp 
 * Inputs         : p_x2_setup_resp    - pointer to x2_setup_fail_t
 *                  p_asn_msg          - pointer to ASN encoded msg 
 *                  p_asn_msg_len      -  pointer to ASN encoded msg length 
 *                  p_error_indication -  pointer to rrc_x2ap_error_indication_t
 *                  p_snd_err_ind      - pointer to x2ap_error_ind_bool_t 
 * Outputs        : None 
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to encode ENDC X2 Setup failure response.
 * ERROR          : his function report error to the caller function, If encoder
 *                  fails  to encode message.
 ********************************************************************************/
x2ap_return_et
x2ap_encode_endc_x2setup_failure_resp
(
    x2_setup_fail_t                *p_x2_setup_resp,
    U8                             *p_asn_msg,
    U16                            *p_asn_msg_len,
    rrc_x2ap_error_indication_t    *p_error_indication,
    x2ap_error_ind_bool_t          *p_snd_err_ind
)
{
    x2ap_return_et                              retVal = X2AP_FAILURE;
    OSCTXT                                      asn1_ctx;
    x2ap_X2AP_PDU                               x2ap_pdu;
    OSRTDListNode                               *p_node = X2AP_P_NULL;
    x2ap_ENDCX2SetupFailure                     *p_x2ap_X2SetupFailure = X2AP_P_NULL;
    x2ap_ENDCX2SetupFailure_protocolIEs_element *p_protocolIE_elem =
                                                          X2AP_P_NULL;
    x2ap_CriticalityDiagnostics                 crit_diag;
    EVENT_EXTERNAL_HEADER                       *p_event_header = PNULL;
    
    X2AP_UT_TRACE_ENTER();

    /*
     * Init ASN1 context
     */
    if ( X2AP_NULL != rtInitContext(&asn1_ctx) )
    {
        X2AP_TRACE(X2AP_ERROR, "%s:ASN context initialization failed.",
                   __FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return retVal;
    }

    do
    {
        X2AP_MEMSET(&(x2ap_pdu), X2AP_NULL, sizeof(x2ap_X2AP_PDU));

        /*
         * Fill the values in the ASN structures that shall be encoded by
         * ASN Encoder
         */
        /*
         * Set Pdu type to Initiating message
         */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_unsuccessfulOutcome;

        x2ap_pdu.u.unsuccessfulOutcome = rtxMemAllocType(&asn1_ctx,
                x2ap_UnsuccessfulOutcome);
        if ( X2AP_P_NULL == x2ap_pdu.u.unsuccessfulOutcome )
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_UnsuccessfulOutcome(x2ap_pdu.u.unsuccessfulOutcome);

        x2ap_pdu.u.unsuccessfulOutcome->procedureCode = ASN1V_x2ap_id_endcX2Setup;
        x2ap_pdu.u.unsuccessfulOutcome->criticality = x2ap_reject;

        /*
         * Set the initiating message type to ENDC X2 Setup
         */
        x2ap_pdu.u.unsuccessfulOutcome->value.t = T1x2ap__endcX2Setup;

        p_x2ap_X2SetupFailure = rtxMemAllocType(&asn1_ctx,
                                                x2ap_ENDCX2SetupFailure);
        if ( X2AP_P_NULL == p_x2ap_X2SetupFailure )
        {
             X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
             break;
        }
        asn1Init_x2ap_ENDCX2SetupFailure(p_x2ap_X2SetupFailure);

        x2ap_pdu.u.unsuccessfulOutcome->value.u.endcX2Setup = p_x2ap_X2SetupFailure;

        rtxDListInit(&p_x2ap_X2SetupFailure->protocolIEs);

        /*
         * Compose id-Cause
         */
        rtxDListAllocNodeAndData(&asn1_ctx, 
                x2ap_ENDCX2SetupFailure_protocolIEs_element, 
                &p_node, 
                &p_protocolIE_elem);
        if ( X2AP_P_NULL == p_node )
        {
            /*
             * Not enough memory
             */
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        if ( X2AP_P_NULL == p_protocolIE_elem )
        {
            /* Not enough memory
             */
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.", __FUNCTION__);
            break;
        }

        asn1Init_x2ap_ENDCX2SetupFailure_protocolIEs_element(
                p_protocolIE_elem);

        /*
         * Fill protocol IE Id and criticality
         */
        p_protocolIE_elem->id = ASN1V_x2ap_id_Cause;
        p_protocolIE_elem->criticality = x2ap_ignore;

        /*
         * Fill the type of Protocol IE element
         */
        p_protocolIE_elem->value.t = T215x2ap___x2ap_ENDCX2SetupFailure_IEs_1;

        p_protocolIE_elem->value.u._x2ap_ENDCX2SetupFailure_IEs_1 =
            rtxMemAllocType(&asn1_ctx,x2ap_Cause);

        if ( X2AP_P_NULL == p_protocolIE_elem->value.u._x2ap_ENDCX2SetupFailure_IEs_1 )
        {
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_Cause(p_protocolIE_elem->value.u.
                _x2ap_ENDCX2SetupFailure_IEs_1);

        if ( X2AP_FAILURE == x2ap_compose_cause(
                    p_protocolIE_elem->value.u._x2ap_ENDCX2SetupFailure_IEs_1,
                    &p_x2_setup_resp->cause) )
        {
            X2AP_TRACE(X2AP_ERROR,
                       "%s:x2ap_compose_cause returned Failure.",
                       __FUNCTION__);
            break;
        }
        rtxDListAppendNode(&p_x2ap_X2SetupFailure->protocolIEs, p_node);  

        /*Compose id-CriticalityDiagnostics*/
        if( X2_SETUP_FAIL_CRITICAL_DIAGNO_PRESENT & p_x2_setup_resp->bitmask)
        {

            rtxDListAllocNodeAndData(&asn1_ctx,
                    x2ap_ENDCX2SetupFailure_protocolIEs_element,
                    &p_node,
                    &p_protocolIE_elem);

            if (X2AP_P_NULL == p_node)
            {
                X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
                break;
            }
            asn1Init_x2ap_ENDCX2SetupFailure_protocolIEs_element(p_protocolIE_elem);

            p_protocolIE_elem->id = ASN1V_x2ap_id_CriticalityDiagnostics;
            p_protocolIE_elem->criticality = x2ap_ignore;

            p_protocolIE_elem->value.t = T215x2ap___x2ap_ENDCX2SetupFailure_IEs_2;

            p_protocolIE_elem->value.u._x2ap_ENDCX2SetupFailure_IEs_2 =
                rtxMemAllocType (&asn1_ctx, x2ap_CriticalityDiagnostics);
            if (X2AP_P_NULL == 
                    p_protocolIE_elem->value.u._x2ap_ENDCX2SetupFailure_IEs_2)
            {
                X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
                break;
            }

            asn1Init_x2ap_CriticalityDiagnostics(
                    p_protocolIE_elem->value.u._x2ap_ENDCX2SetupFailure_IEs_2);

            if (X2AP_FAILURE == x2ap_compose_CriticalityDiagnostics(&asn1_ctx,
                        p_protocolIE_elem->value.u._x2ap_ENDCX2SetupFailure_IEs_2,
                        &p_x2_setup_resp->critc_diagno))
            {
                X2AP_TRACE(X2AP_ERROR,
                        "%s returned Failure.",
                        __FUNCTION__);
                break;
            }
            rtxDListAppendNode(&p_x2ap_X2SetupFailure->protocolIEs, p_node);
        }

        /*
         * Compose id-TimeToWait
         */
        if ( p_x2_setup_resp->time_to_wait )
        {

            rtxDListAllocNodeAndData(&asn1_ctx, 
                    x2ap_ENDCX2SetupFailure_protocolIEs_element, 
                    &p_node, 
                    &p_protocolIE_elem);
            if ( X2AP_P_NULL == p_node )
            {
                /*
                 * Not enough memory
                 */
                X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                break;
            }
            asn1Init_x2ap_ENDCX2SetupFailure_protocolIEs_element(
                    p_protocolIE_elem);

            /*
             * Fill protocol IE Id and criticality
             */
            p_protocolIE_elem->id = ASN1V_x2ap_id_TimeToWait;
            p_protocolIE_elem->criticality = x2ap_ignore;

            /*
             * Fill the type of Protocol IE element
             */
            p_protocolIE_elem->value.t = T215x2ap___x2ap_ENDCX2SetupFailure_IEs_3;

            p_protocolIE_elem->value.u._x2ap_ENDCX2SetupFailure_IEs_3 =
                p_x2_setup_resp->time_to_wait;

            rtxDListAppendNode(&p_x2ap_X2SetupFailure->protocolIEs, p_node);  
        }
        /*
         * Compose id-CriticalityDiagnostics
         */
        if ( ( PNULL != p_snd_err_ind ) && ( PNULL != p_error_indication ) )
        {
            if ( ( X2AP_TRUE == p_snd_err_ind->send_err_indication_reject_ie_present ) ||
                    ( X2AP_TRUE == p_snd_err_ind->send_err_indication_notify_ie_present ) )
            {
                rtxDListAllocNodeAndData(&asn1_ctx,
                        x2ap_ENDCX2SetupFailure_protocolIEs_element,
                        &p_node,
                        &p_protocolIE_elem);
                if ( PNULL == p_node )
                {
                    /*
                     * Not enough memory
                     */
                    X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
                    break;
                }
                X2AP_MEMSET(p_protocolIE_elem, X2AP_NULL,
                        sizeof(x2ap_ENDCX2SetupFailure_protocolIEs_element));

                /*
                 * Fill protocol IE Id and criticality
                 */
                p_protocolIE_elem->id = ASN1V_x2ap_id_CriticalityDiagnostics;
                p_protocolIE_elem->criticality = x2ap_ignore;

                X2AP_MEMSET(&crit_diag, X2AP_NULL, sizeof(x2ap_CriticalityDiagnostics));

                rtxDListInit(&crit_diag.iEsCriticalityDiagnostics);
                rtxDListInit(&crit_diag.extElem1);

                /*
                 * Fill the type of Protocol IE element
                 */
                p_protocolIE_elem->value.t = T215x2ap___x2ap_ENDCX2SetupFailure_IEs_2;
                p_protocolIE_elem->value.u._x2ap_ENDCX2SetupFailure_IEs_2 = 
                    &p_error_indication->criticality_diagnostics;

                rtxDListAppendNode(&p_x2ap_X2SetupFailure->protocolIEs, p_node);
            }
        }

        /*
         * ASN Encode message
         */
        pu_setBuffer(&asn1_ctx, p_asn_msg,
                     X2AP_MAX_ASN1_BUF_LEN, TRUE);

        if ( X2AP_NULL != asn1PE_x2ap_X2AP_PDU(&asn1_ctx, &x2ap_pdu) )
        {
            X2AP_TRACE(X2AP_ERROR, "ASN1 encoding of ENDC X2 Setup Failure"
                    "failed.");
            break;
        }
        else
        {
            *p_asn_msg_len= (U16)pe_GetMsgLen(&asn1_ctx);
            retVal = X2AP_SUCCESS;
           
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
            
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    (U32)*p_asn_msg_len);
            if ( PNULL != p_event_header )
            {
                x2ap_fill_protocol_event_params(p_event_header, (U32)*p_asn_msg_len,
                        LOCAL_X2_SETUP_FAILURE, 
                        MESSAGE_DIRECTION_SEND);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        (U32)*p_asn_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }
    }while(X2AP_NULL);

    /*
     * Free ASN1 context
     */
    rtFreeContext(&asn1_ctx);

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/*****************************************************************************
* FUNCTION NAME  : x2ap_encode_endc_x2setup_resp
* INPUT          : x2_setup_req_t       *p_x2_endc_setup_resp
*                  U8                   *p_asn_msg
*                  U16                  *p_asn_msg_len
* OUTPUT         : U8                   *p_asn_msg
*                  U16                  *p_asn_msg_len
* DESCRIPTION    : This function encode the EN DC X2 setup response
* RETURNS        : X2AP_SUCCESS - if successfully encoded the message
*                  X2AP_FAILURE - if fails to encode the message
*******************************************************************************/
x2ap_return_et
x2ap_encode_endc_x2setup_resp
(
    en_dc_x2_setup_resp_t    *p_x2_endc_setup_resp,
    U8                       *p_asn_msg,
    U16                      *p_asn_msg_len
)
{
    x2ap_return_et              retVal = X2AP_FAILURE;
    OSCTXT                      asn1_ctx;
    x2ap_X2AP_PDU               x2ap_pdu;
    OSRTDListNode               *p_node = PNULL;
    x2ap_ENDCX2SetupResponse_protocolIEs_element
         *p_protocolIE_elem = PNULL;
    x2ap_ENDCX2SetupResponse         *p_endc_X2SetupResponse = X2AP_P_NULL;
    x2ap_RespondingNodeType_EndcX2Setup_respond_eNB_element
        *p_endc_x2setup_respond_enb_elem = PNULL;
    x2ap_RespondingNodeType_EndcX2Setup_respond_eNB
        *p_endc_x2setup_respond_enb = PNULL;
    OSRTDListNode*  p_elem_node  = PNULL;

    X2AP_UT_TRACE_ENTER();

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        X2AP_TRACE(X2AP_ERROR, "%s:ASN context initialization failed.",
                   __FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return retVal;
    }
    do
    {
        X2AP_MEMSET(&(x2ap_pdu), 0, sizeof(x2ap_X2AP_PDU));
        /* Fill the values in the ASN structures that shall be encoded by
         *  ASN Encoder */
        /* Set Pdu type to Initiating message */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_successfulOutcome;
        x2ap_pdu.u.successfulOutcome = rtxMemAllocType(&asn1_ctx,
                x2ap_SuccessfulOutcome);
        if (X2AP_P_NULL == x2ap_pdu.u.successfulOutcome)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_SuccessfulOutcome(x2ap_pdu.u.successfulOutcome);

        x2ap_pdu.u.successfulOutcome->procedureCode = ASN1V_x2ap_id_endcX2Setup;
        x2ap_pdu.u.successfulOutcome->criticality = x2ap_reject;

        /* Set the initiating message type to ENDC X2 Setup */
        x2ap_pdu.u.successfulOutcome->value.t = T1x2ap__endcX2Setup;

        p_endc_X2SetupResponse = rtxMemAllocType(&asn1_ctx,
                                                x2ap_ENDCX2SetupResponse);
        if (X2AP_P_NULL == p_endc_X2SetupResponse)
        {
             X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
             break;
        }
        else
        {
            X2AP_TRACE(X2AP_DETAILED,
                    "Mem Alloc successful to x2ap_ENDCX2SetupResponse");
        }

        asn1Init_x2ap_ENDCX2SetupResponse(p_endc_X2SetupResponse);

        x2ap_pdu.u.successfulOutcome->value.u.endcX2Setup = p_endc_X2SetupResponse;

        asn1Init_x2ap_ENDCX2SetupResponse_protocolIEs(
            &p_endc_X2SetupResponse->protocolIEs);

        rtxDListAllocNodeAndData(&asn1_ctx,
                x2ap_ENDCX2SetupResponse_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if (X2AP_P_NULL == p_node)
        {
            /* Not enough memory */
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_ENDCX2SetupResponse_protocolIEs_element(
                p_protocolIE_elem);

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_x2ap_id_RespondingNodeType_EndcX2Setup;
        p_protocolIE_elem->criticality = x2ap_reject;

        /* Fill the type of Protocol IE element */
        /* coverity_<277522>_fix_start */
        p_protocolIE_elem->value.t = T212x2ap___x2ap_ENDCX2SetupResponse_IEs_1;
        /* coverity_<277522>_fix_end */

        p_protocolIE_elem->value.u._x2ap_ENDCX2SetupResponse_IEs_1 =
            rtxMemAllocType(&asn1_ctx,x2ap_RespondingNodeType_EndcX2Setup);

        if (X2AP_P_NULL == p_protocolIE_elem->value.u._x2ap_ENDCX2SetupResponse_IEs_1)
        {
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        asn1Init_x2ap_RespondingNodeType_EndcX2Setup(p_protocolIE_elem->
            value.u._x2ap_ENDCX2SetupResponse_IEs_1);
        X2AP_TRACE(X2AP_DETAILED, "Populating "
            "T_x2ap_RespondingNodeType_EndcX2Setup_respond_eNB\n");

        p_protocolIE_elem->value.u._x2ap_ENDCX2SetupResponse_IEs_1->t
            = T_x2ap_RespondingNodeType_EndcX2Setup_respond_eNB;

        p_protocolIE_elem->value.u._x2ap_ENDCX2SetupResponse_IEs_1->u.
            respond_eNB = rtxMemAllocType(&asn1_ctx,
            x2ap_RespondingNodeType_EndcX2Setup_respond_eNB);
        if ( X2AP_P_NULL == p_protocolIE_elem->value.u.
            _x2ap_ENDCX2SetupResponse_IEs_1->u.respond_eNB )
        {
            X2AP_TRACE(X2AP_ERROR, "ASN Malloc Failed \n");
            break;
        }

        asn1Init_x2ap_RespondingNodeType_EndcX2Setup_respond_eNB(
            p_protocolIE_elem->value.u.
            _x2ap_ENDCX2SetupResponse_IEs_1->u.respond_eNB);

        p_endc_x2setup_respond_enb = p_protocolIE_elem->value.u.
            _x2ap_ENDCX2SetupResponse_IEs_1->u.respond_eNB;
        rtxDListAllocNodeAndData(&asn1_ctx,
                    x2ap_RespondingNodeType_EndcX2Setup_respond_eNB_element,
                    &p_elem_node,
                    &p_endc_x2setup_respond_enb_elem);
        if ( PNULL == p_elem_node )
        {
            /*
             * Not enough memory
             */
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed!!!!",
                    __FUNCTION__);
            break;
        }
        asn1Init_x2ap_RespondingNodeType_EndcX2Setup_respond_eNB_element(
            p_endc_x2setup_respond_enb_elem);

        X2AP_TRACE(X2AP_DETAILED, "Populating p_endc_x2setup_respond_enb_elem\n");

        p_endc_x2setup_respond_enb_elem->id = ASN1V_x2ap_id_GlobalENB_ID;
        p_endc_x2setup_respond_enb_elem->criticality = x2ap_reject;

        X2AP_TRACE(X2AP_DETAILED, "src_endc_x2setup_init_enb_elem->id[%x]",
             p_endc_x2setup_respond_enb_elem->id);

        X2AP_TRACE(X2AP_DETAILED, "Encoding eNB-IDs\n");

        p_endc_x2setup_respond_enb_elem->value.t =
            T213x2ap___x2ap_ENB_ENDCX2SetupReqAckIEs_1;
        p_endc_x2setup_respond_enb_elem->value.u._x2ap_ENB_ENDCX2SetupReqAckIEs_1 =
            rtxMemAllocType(&asn1_ctx,x2ap_GlobalENB_ID);

        if ( PNULL == p_endc_x2setup_respond_enb_elem->value.u.
             _x2ap_ENB_ENDCX2SetupReqAckIEs_1 )
        {
            X2AP_TRACE(X2AP_ERROR,"ASN malloc failed !!!!\n");
            break;
        }

        asn1Init_x2ap_GlobalENB_ID(p_endc_x2setup_respond_enb_elem->
            value.u._x2ap_ENB_ENDCX2SetupReqAckIEs_1);

        /*
         * Filling global_plmn id and global enb id
         */
        if ( X2AP_FAILURE == x2ap_compose_global_enb_id(&asn1_ctx,
            p_endc_x2setup_respond_enb_elem->value.
            u._x2ap_ENB_ENDCX2SetupReqAckIEs_1,
            &p_x2_endc_setup_resp->eutra_cell_info.global_enb_id) )
        {
            X2AP_TRACE(X2AP_ERROR,
                       "%s:compose id-Global-ENB-ID returned Failure.",
                       __FUNCTION__);
            rtFreeContext(&asn1_ctx);
            break;
        }
        else
        {
            X2AP_TRACE(X2AP_DETAILED,
                "compose id-Global-ENB-ID returned Success.");
        }
        rtxDListAppendNode(p_endc_x2setup_respond_enb, p_elem_node);
        rtxDListAppendNode(&p_endc_X2SetupResponse->protocolIEs, p_node);

/////////////////////
        OSRTDListNode              *p_elem_node_1 = PNULL;
        /*
         * Compose id-ServedCells
         */
        X2AP_TRACE(X2AP_DETAILED,"Encoding Served Cells \n");

        /*
         * Allocate memory for target protocolIE element
         */
        rtxDListAllocNodeAndData(&asn1_ctx,
            x2ap_ENDCX2SetupResponse_protocolIEs_element,
            &p_node,
            &p_protocolIE_elem);

        if ( X2AP_P_NULL == p_node )
        {
            /*
             * Not enough memory
             */
            X2AP_TRACE(X2AP_ERROR,  "%s:ASN malloc failed !!!",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_ENDCX2SetupResponse_protocolIEs_element(
            p_protocolIE_elem);

        /*
         * Fill protocol IE Id and criticality
         */
        p_protocolIE_elem->id = ASN1V_x2ap_id_RespondingNodeType_EndcX2Setup;
        p_protocolIE_elem->criticality = x2ap_reject;

        /*
         * Fill the type of Protocol IE element
         */
        /* coverity_<277522>_fix_start */
        p_protocolIE_elem->value.t = T212x2ap___x2ap_ENDCX2SetupResponse_IEs_1;
        /* coverity_<277522>_fix_start */

        p_protocolIE_elem->value.u._x2ap_ENDCX2SetupResponse_IEs_1
            = rtxMemAllocType(&asn1_ctx,x2ap_RespondingNodeType_EndcX2Setup);

        if ( X2AP_P_NULL == p_protocolIE_elem->value.u.
            _x2ap_ENDCX2SetupResponse_IEs_1 )
        {
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed!!!",__FUNCTION__);
            break;
        }

        asn1Init_x2ap_RespondingNodeType_EndcX2Setup(p_protocolIE_elem->
            value.u._x2ap_ENDCX2SetupResponse_IEs_1);

        X2AP_TRACE(X2AP_DETAILED, "Populating "
            "T_x2ap_RespondingNodeType_EndcX2Setup_respond_eNB\n");

        p_protocolIE_elem->value.u._x2ap_ENDCX2SetupResponse_IEs_1->t
            = T_x2ap_RespondingNodeType_EndcX2Setup_respond_eNB;

        p_protocolIE_elem->value.u._x2ap_ENDCX2SetupResponse_IEs_1->u.
            respond_eNB = rtxMemAllocType(&asn1_ctx,
            x2ap_RespondingNodeType_EndcX2Setup_respond_eNB);

        if ( X2AP_P_NULL == p_protocolIE_elem->value.u.
            _x2ap_ENDCX2SetupResponse_IEs_1->u.respond_eNB )
        {
            X2AP_TRACE(X2AP_ERROR, "ASN Malloc Failed \n");
            break;
        }

        asn1Init_x2ap_RespondingNodeType_EndcX2Setup_respond_eNB(
            p_protocolIE_elem->value.u.
            _x2ap_ENDCX2SetupResponse_IEs_1->u.respond_eNB);

        p_endc_x2setup_respond_enb = p_protocolIE_elem->value.u.
            _x2ap_ENDCX2SetupResponse_IEs_1->u.respond_eNB;

        rtxDListAllocNodeAndData(&asn1_ctx,
                    x2ap_RespondingNodeType_EndcX2Setup_respond_eNB_element,
                    &p_elem_node_1,
                    &p_endc_x2setup_respond_enb_elem);
        if ( PNULL == p_elem_node_1 )
        {
            /*
             * Not enough memory
             */
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed!!!!",
                    __FUNCTION__);
            break;
        }

        asn1Init_x2ap_RespondingNodeType_EndcX2Setup_respond_eNB_element(
            p_endc_x2setup_respond_enb_elem);

        p_endc_x2setup_respond_enb_elem->id =
            ASN1V_x2ap_id_ServedEUTRAcellsENDCX2ManagementList;
        p_endc_x2setup_respond_enb_elem->criticality = x2ap_reject;

        /*
         * Fill the type of Protocol IE element
         */
        p_endc_x2setup_respond_enb_elem->value.t = T213x2ap___x2ap_ENB_ENDCX2SetupReqAckIEs_2;

        p_endc_x2setup_respond_enb_elem->value.u._x2ap_ENB_ENDCX2SetupReqAckIEs_2
            = rtxMemAllocType(&asn1_ctx,
            x2ap_ServedEUTRAcellsENDCX2ManagementList);

        if ( PNULL == p_endc_x2setup_respond_enb_elem->value.u.
             _x2ap_ENB_ENDCX2SetupReqAckIEs_2 )
        {
            X2AP_TRACE(X2AP_ERROR,"ASN malloc Failed\n");
            return X2AP_FAILURE;
        }

        asn1Init_x2ap_ServedEUTRAcellsENDCX2ManagementList(
            p_endc_x2setup_respond_enb_elem->value.u._x2ap_ENB_ENDCX2SetupReqAckIEs_2);
        /*
         * Populate served cell information
         */
        if ( X2AP_FAILURE == x2ap_compose_served_cell_dc(&asn1_ctx,
                              p_endc_x2setup_respond_enb_elem->value.u.
                              _x2ap_ENB_ENDCX2SetupReqAckIEs_2,
                              (en_dc_x2_setup_req_t *)p_x2_endc_setup_resp) )
        {
            X2AP_TRACE(X2AP_ERROR,
                       "%s:compose_served_cell returned Failure.",
                       __FUNCTION__);
            rtFreeContext(&asn1_ctx);
            break;
        }
        else
        {
            X2AP_TRACE(X2AP_DETAILED,
                       "compose_served_cell returned Success.");
        }
        
        rtxDListAppendNode(p_endc_x2setup_respond_enb, p_elem_node_1);
        rtxDListAppendNode(&p_endc_X2SetupResponse->protocolIEs, p_node);

        /*
         * ASN Encode message
         */
        pu_setBuffer(&asn1_ctx, p_asn_msg,
                X2AP_MAX_ASN1_BUF_LEN, TRUE);

        if ( X2AP_NULL != asn1PE_x2ap_X2AP_PDU(&asn1_ctx, &x2ap_pdu) )
        {
            X2AP_TRACE(X2AP_ERROR, "ASN1 encoding of SeNB Addition failed.");
            rtxErrPrint(&asn1_ctx);
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
            rtFreeContext(&asn1_ctx);
            break;
        }
        else
        {
            *p_asn_msg_len = (U16)pe_GetMsgLen(&asn1_ctx);
            retVal = X2AP_SUCCESS;
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
        }
    }while(X2AP_NULL);

    /*
     * Free ASN1 context
     */
    rtFreeContext(&asn1_ctx);
    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/*******************************************************************************
 * FUNCTION NAME : x2ap_internal_encode_sgnb_release_confirm
 * INPUTS        : pctxt - pointer to OSCTXT
 *                 p_buff - pointer to ASN.1 encoded message
 *                 p_buff_size - size of result buffer
 * OUTPUTS       : p_buff (ASN.1 encoded message),
 *                 p_buff_size (size of result buffer)
 * RETURNS       : X2AP_SUCCESS / X2AP_FAILURE
 * DESCRIPTION   : This function used to encode SgNB release confirm.
 *******************************************************************************/
x2ap_return_et x2ap_internal_encode_sgnb_release_confirm
(
    OSCTXT                            *pctxt,
    U8                                *p_buff,
    U16                               *p_buff_size,
    rrc_x2ap_sgnb_release_confirm_t   *p_sgnb_rel_confirm
)
{
    x2ap_return_et                      result = X2AP_FAILURE;
    x2ap_X2AP_PDU                       x2ap_pdu;
    x2ap_SgNBReleaseConfirm             *p_x2ap_msg = PNULL;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != pctxt);
    X2AP_ASSERT(PNULL != p_buff);
    X2AP_ASSERT(PNULL != p_buff_size);
    X2AP_ASSERT(PNULL != p_sgnb_rel_confirm);

    do
    {
        /*
         * Init X2AP PDU
         */
        asn1Init_x2ap_X2AP_PDU(&x2ap_pdu);

        /*
         * Set Pdu type to Initiating Message
         */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_successfulOutcome;

        x2ap_pdu.u.successfulOutcome =
            rtxMemAllocType(pctxt, x2ap_SuccessfulOutcome);
        if (PNULL == x2ap_pdu.u.successfulOutcome)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        asn1Init_x2ap_SuccessfulOutcome(x2ap_pdu.u.successfulOutcome);

        x2ap_pdu.u.successfulOutcome->procedureCode =
            ASN1V_x2ap_id_sgNBinitiatedSgNBRelease;

        x2ap_pdu.u.successfulOutcome->criticality = x2ap_reject;

        x2ap_pdu.u.successfulOutcome->value.t =
            T1x2ap__sgNBinitiatedSgNBRelease;

        p_x2ap_msg = 
            rtxMemAllocType(pctxt,x2ap_SgNBReleaseConfirm);
        if ( X2AP_P_NULL == p_x2ap_msg )
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        x2ap_pdu.u.successfulOutcome->value.u.sgNBinitiatedSgNBRelease =
            p_x2ap_msg;

        asn1Init_x2ap_SgNBReleaseConfirm(p_x2ap_msg);

        if (RT_OK != asn1Append_x2ap_SgNBReleaseConfirm_protocolIEs_1(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            p_sgnb_rel_confirm->menb_ue_x2ap_id))
        {
            break;
        }

        if (RT_OK != asn1Append_x2ap_SgNBReleaseConfirm_protocolIEs_2(
                        pctxt,
                        &p_x2ap_msg->protocolIEs,
                        p_sgnb_rel_confirm->sgnb_ue_x2ap_id))
        {
                break;
        }
#if 0
        if (RT_OK != asn1Append_x2ap_SgNBReleaseConfirm_protocolIEs_3(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            &p_sgnb_rel_confirm->E_RABs_ToBeReleased_SgNBRelConfList))
        {
            break;
        }

        if (RT_OK != asn1Append_x2ap_SgNBReleaseConfirm_protocolIEs_4(
            pctxt,
            &p_x2ap_msg->protocolIEs,
            &p_sgnb_rel_confirm->CriticalityDiagnostics))
        {
            break;
        }
#endif

        /*
         * ASN Encode message
         */
        pu_setBuffer(pctxt, p_buff, X2AP_MAX_ASN1_BUF_LEN, ASN_X2AP_BUF_TYPE);

        if (RT_OK != asn1PE_x2ap_X2AP_PDU(pctxt, &x2ap_pdu))
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN1 encoding failed.",__FUNCTION__);
            char buff[500];
            rtxErrGetTextBuf(pctxt,buff ,500);
#ifdef __x86_64__
            X2AP_TRACE(X2AP_ERROR, "BUFFER[%s] %p\n",(char*)buff, buff);
#else
            X2AP_TRACE(X2AP_ERROR, "BUFFER[%s] %x\n",(char*)buff,(unsigned int)buff);
#endif
            break;
        }
        rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);

        *p_buff_size = (U16)pe_GetMsgLen(pctxt);

        result=X2AP_SUCCESS;
    } while(X2AP_NULL);

    X2AP_UT_TRACE_EXIT();
    return result;
}                                
   
/*****************************************************************************
 * Function Name  : x2ap_encode_ENDC_Configuration_Update_ack 
 * Inputs         : p_endc_conf_ack - pointer to x2ap_endc_config_update_ack_t 
 *                  p_asn_msg - pointer to ASN encoded message
 *                  p_asn_msg_len - pointer to ASN encoded message length 
 * Outputs        : p_asn_msg - 
 *                  p_asn_msg_len - ASN encoded message length
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to encode ENDC Configuration Update ACK.
 * ERROR          : This function report error to the caller function, If encoder
 *                  fails to encode message.
 ********************************************************************************/
x2ap_return_et x2ap_encode_ENDC_Configuration_Update_ack (
        x2ap_endc_config_update_ack_t   *p_endc_conf_ack,
        UInt8                              *p_asn_msg,
        UInt16                             *p_asn_msg_len)
{
    x2ap_return_et                retVal = X2AP_FAILURE;
    OSCTXT                        asn1_ctx;
    x2ap_X2AP_PDU                 x2ap_pdu;
    OSRTDListNode                 *p_node = X2AP_P_NULL;
    OSRTDListNode                 *p_node_2 = X2AP_P_NULL;
    x2ap_ENDCConfigurationUpdateAcknowledge
                                  *p_ENDCConfigurationUpdateAcknowledge = X2AP_P_NULL;
    x2ap_ENDCConfigurationUpdateAcknowledge_protocolIEs_element
                                  *p_protocolIE_elem = X2AP_P_NULL;

    X2AP_UT_TRACE_ENTER();

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        X2AP_TRACE(X2AP_ERROR, "%s:ASN context initialization failed.",
                   __FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return retVal;
    }
    do
    {
         X2AP_MEMSET ( &(x2ap_pdu), 0, sizeof(x2ap_X2AP_PDU) );
        /*
        ** Fill the values in the ASN structures that shall be encoded by ASN
        ** Encoder
        */
         /* Set Pdu type to Successful message */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_successfulOutcome;

        x2ap_pdu.u.successfulOutcome =
            rtxMemAllocType(&asn1_ctx, x2ap_SuccessfulOutcome);
        if (X2AP_P_NULL == x2ap_pdu.u.successfulOutcome)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        asn1Init_x2ap_SuccessfulOutcome(x2ap_pdu.u.successfulOutcome);

        x2ap_pdu.u.successfulOutcome->procedureCode =
            ASN1V_x2ap_id_endcConfigurationUpdate;

        x2ap_pdu.u.successfulOutcome->criticality = x2ap_reject;

        /* Set the initiating message type to ENDC configuration update */
        x2ap_pdu.u.successfulOutcome->value.t = T1x2ap__endcConfigurationUpdate;
        
        p_ENDCConfigurationUpdateAcknowledge = rtxMemAllocType(&asn1_ctx,
                                       x2ap_ENDCConfigurationUpdateAcknowledge);
        if (X2AP_P_NULL == p_ENDCConfigurationUpdateAcknowledge)
        {
             X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
             break;
        }
        
        asn1Init_x2ap_ENDCConfigurationUpdateAcknowledge(
                                          p_ENDCConfigurationUpdateAcknowledge);

        x2ap_pdu.u.successfulOutcome->value.u.endcConfigurationUpdate =
                                           p_ENDCConfigurationUpdateAcknowledge;

        rtxDListAllocNodeAndData(&asn1_ctx,
                x2ap_ENDCConfigurationUpdateAcknowledge_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
        if (X2AP_P_NULL == p_node)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        asn1Init_x2ap_ENDCConfigurationUpdateAcknowledge_protocolIEs_element(
                p_protocolIE_elem);
        p_protocolIE_elem->id = ASN1V_x2ap_id_RespondingNodeType_EndcConfigUpdate;
        p_protocolIE_elem->criticality = x2ap_reject;
        
        p_protocolIE_elem->value.t = T221x2ap___x2ap_ENDCConfigurationUpdateAcknowledge_IEs_1;

        p_protocolIE_elem->value.u._x2ap_ENDCConfigurationUpdateAcknowledge_IEs_1 = rtxMemAllocType(&asn1_ctx,
                x2ap_RespondingNodeType_EndcConfigUpdate);
        if (X2AP_P_NULL == p_protocolIE_elem->value.u._x2ap_ENDCConfigurationUpdateAcknowledge_IEs_1)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        
        asn1Init_x2ap_RespondingNodeType_EndcConfigUpdate(p_protocolIE_elem->
            value.u._x2ap_ENDCConfigurationUpdateAcknowledge_IEs_1);

        p_protocolIE_elem->value.u._x2ap_ENDCConfigurationUpdateAcknowledge_IEs_1->t = 
            T_x2ap_RespondingNodeType_EndcConfigUpdate_respond_eNB;

        p_protocolIE_elem->value.u._x2ap_ENDCConfigurationUpdateAcknowledge_IEs_1->u.respond_en_gNB = 
            rtxMemAllocType(&asn1_ctx,x2ap_RespondingNodeType_EndcConfigUpdate_respond_en_gNB);

        if (X2AP_P_NULL == p_protocolIE_elem->value.u._x2ap_ENDCConfigurationUpdateAcknowledge_IEs_1->u.respond_en_gNB)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        asn1Init_x2ap_RespondingNodeType_EndcConfigUpdate_respond_en_gNB(p_protocolIE_elem->
                value.u._x2ap_ENDCConfigurationUpdateAcknowledge_IEs_1->
                u.respond_en_gNB);

        /* no need to realize reponse_en_gnb */        
        
        rtxDListAppendNode(&p_ENDCConfigurationUpdateAcknowledge->protocolIEs ,p_node);
        
        /* ASN Encode message */
        if (RT_OK != pu_setBuffer(&asn1_ctx, p_asn_msg, X2AP_MAX_ASN1_BUF_LEN, TRUE))
        {
            X2AP_TRACE(X2AP_ERROR,"pu_setBuffer failed.",__FUNCTION__);
            break;
        }

        if (0 != asn1PE_x2ap_X2AP_PDU(&asn1_ctx, &x2ap_pdu))
        {
            X2AP_TRACE(X2AP_ERROR, "ASN1 encoding of ENDC Configuration Update Ack "
                    "failed.");
            break;
        }
        else
        {
            *p_asn_msg_len = (UInt16)pe_GetMsgLen(&asn1_ctx);
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
            retVal = X2AP_SUCCESS;
        }
    }while(0);
    
    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    X2AP_UT_TRACE_EXIT();
    return retVal;

}

/*****************************************************************************
 * Function Name  : x2ap_encode_endc_config_update_failure
 * Inputs         : p_endc_config_update_fail - pointer to
 *                     x2ap_endc_config_update_failure_t 
 *                  p_asn_msg - pointer to ASN encoded message
 *                  p_asn_msg_len - pointer to ASN encoded message length 
 * Outputs        : p_asn_msg - 
 *                  p_asn_msg_len - ASN encoded message length
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to encode ENDC Config Update Failure 
 * ERROR          : This function report error to the caller function, If encoder
 *                  fails to encode message.
 ********************************************************************************/
x2ap_return_et x2ap_encode_endc_config_update_failure(
        x2ap_endc_config_update_failure_t   *p_endc_config_update_fail,
        UInt8                                  *p_asn_msg,
        UInt16                                 *p_asn_msg_len)
{
    x2ap_return_et                              retVal = X2AP_FAILURE;
    OSCTXT                                      asn1_ctx;
    x2ap_X2AP_PDU                               x2ap_pdu;
    OSRTDListNode                               *p_node = X2AP_P_NULL;
    x2ap_ENDCConfigurationUpdateFailure_protocolIEs_element
                                        *p_endc_protocolIE_elem = X2AP_P_NULL;
    x2ap_ENDCConfigurationUpdateFailure *p_asn_endc_config_update_fail = X2AP_P_NULL;
    x2ap_CriticalityDiagnostics                 crit_diag;
    EVENT_EXTERNAL_HEADER                       *p_event_header = PNULL;
    
    X2AP_UT_TRACE_ENTER();

    /*
     * Init ASN1 context
     */
    if ( X2AP_NULL != rtInitContext(&asn1_ctx) )
    {
        X2AP_TRACE(X2AP_ERROR, "%s:ASN context initialization failed.",
                   __FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return retVal;
    }

    do
    {
        X2AP_MEMSET(&(x2ap_pdu), X2AP_NULL, sizeof(x2ap_X2AP_PDU));

        /*
         * Fill the values in the ASN structures that shall be encoded by
         * ASN Encoder
         */
        /*
         * Set Pdu type to Initiating message
         */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_unsuccessfulOutcome;

        x2ap_pdu.u.unsuccessfulOutcome = rtxMemAllocType(&asn1_ctx,
                x2ap_UnsuccessfulOutcome);
        if ( X2AP_P_NULL == x2ap_pdu.u.unsuccessfulOutcome )
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_UnsuccessfulOutcome(x2ap_pdu.u.unsuccessfulOutcome);

        x2ap_pdu.u.unsuccessfulOutcome->procedureCode = ASN1V_x2ap_id_endcConfigurationUpdate;
        x2ap_pdu.u.unsuccessfulOutcome->criticality = x2ap_reject;

        /*
         * Set the initiating message type to ENDC CONFIGURATION UPDATE
         */
        x2ap_pdu.u.unsuccessfulOutcome->value.t = T1x2ap__endcConfigurationUpdate;

        p_asn_endc_config_update_fail = rtxMemAllocType(&asn1_ctx,
                                                x2ap_ENDCConfigurationUpdateFailure);
        if ( X2AP_P_NULL == p_asn_endc_config_update_fail )
        {
             X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
             break;
        }

        asn1Init_x2ap_ENDCConfigurationUpdateFailure(p_asn_endc_config_update_fail);

        x2ap_pdu.u.unsuccessfulOutcome->value.u.endcConfigurationUpdate = p_asn_endc_config_update_fail;

        rtxDListInit(&p_asn_endc_config_update_fail->protocolIEs);

        /*
         * Compose id-Cause
         */
        rtxDListAllocNodeAndData(&asn1_ctx, 
                x2ap_ENDCConfigurationUpdateFailure_protocolIEs_element, 
                &p_node, 
                &p_endc_protocolIE_elem);
        if ( X2AP_P_NULL == p_node )
        {
            /*
             * Not enough memory
             */
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        if ( X2AP_P_NULL == p_endc_protocolIE_elem )
        {
            /* Not enough memory
             */
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.", __FUNCTION__);
            break;
        }

        asn1Init_x2ap_ENDCConfigurationUpdateFailure_protocolIEs_element(
                p_endc_protocolIE_elem);

        /*
         * Fill protocol IE Id and criticality
         */
        p_endc_protocolIE_elem->id = ASN1V_x2ap_id_Cause;
        p_endc_protocolIE_elem->criticality = x2ap_ignore;

        /*
         * Fill the type of Protocol IE element
         */
        p_endc_protocolIE_elem->value.t = T224x2ap___x2ap_ENDCConfigurationUpdateFailure_IEs_1;

        p_endc_protocolIE_elem->value.u._x2ap_ENDCConfigurationUpdateFailure_IEs_1 =
            rtxMemAllocType(&asn1_ctx,x2ap_Cause);

        if ( X2AP_P_NULL == p_endc_protocolIE_elem->value.u._x2ap_ENDCConfigurationUpdateFailure_IEs_1 )
        {
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        asn1Init_x2ap_Cause(p_endc_protocolIE_elem->value.u.
                _x2ap_ENDCConfigurationUpdateFailure_IEs_1);

        if ( X2AP_FAILURE == x2ap_compose_cause(
                    p_endc_protocolIE_elem->value.u._x2ap_ENDCConfigurationUpdateFailure_IEs_1,
                    &p_endc_config_update_fail->cause) )
        {
            X2AP_TRACE(X2AP_ERROR,
                       "%s:x2ap_compose_cause returned Failure.",
                       __FUNCTION__);
            break;
        }
        rtxDListAppendNode(&p_asn_endc_config_update_fail->protocolIEs, p_node);  

        /*Compose id-CriticalityDiagnostics*/
        if( X2AP_ENDC_CONFIG_UPDATE_FAILURE_CRITICALITY_DIAGNOSTICS_PRESENT & p_endc_config_update_fail->bitmask)
        {

            rtxDListAllocNodeAndData(&asn1_ctx,
                    x2ap_ENDCConfigurationUpdateFailure_protocolIEs_element,
                    &p_node,
                    &p_endc_protocolIE_elem);

            if (X2AP_P_NULL == p_node)
            {
                X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
                break;
            }
            asn1Init_x2ap_ENDCConfigurationUpdateFailure_protocolIEs_element(p_endc_protocolIE_elem);

            p_endc_protocolIE_elem->id = ASN1V_x2ap_id_CriticalityDiagnostics;
            p_endc_protocolIE_elem->criticality = x2ap_ignore;

            p_endc_protocolIE_elem->value.t = T224x2ap___x2ap_ENDCConfigurationUpdateFailure_IEs_2;

            p_endc_protocolIE_elem->value.u._x2ap_ENDCConfigurationUpdateFailure_IEs_2 =
                rtxMemAllocType (&asn1_ctx, x2ap_CriticalityDiagnostics);
            if (X2AP_P_NULL == 
                    p_endc_protocolIE_elem->value.u._x2ap_ENDCConfigurationUpdateFailure_IEs_2)
            {
                X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
                break;
            }

            asn1Init_x2ap_CriticalityDiagnostics(
                    p_endc_protocolIE_elem->value.u._x2ap_ENDCConfigurationUpdateFailure_IEs_2);

            if (X2AP_FAILURE == x2ap_compose_CriticalityDiagnostics(&asn1_ctx,
                        p_endc_protocolIE_elem->value.u._x2ap_ENDCConfigurationUpdateFailure_IEs_2,
                        &p_endc_config_update_fail->criticality_diagnostics))
            {
                X2AP_TRACE(X2AP_ERROR,
                        "%s returned Failure.",
                        __FUNCTION__);
                break;
            }
            rtxDListAppendNode(&p_asn_endc_config_update_fail->protocolIEs, p_node);
        }

        /*
         * Compose id-TimeToWait
         */
        if( X2AP_TIME_TO_WAIT_INDEX_PRESENT & p_endc_config_update_fail->bitmask)
        {

            rtxDListAllocNodeAndData(&asn1_ctx,
                    x2ap_ENDCConfigurationUpdateFailure_protocolIEs_element,
                    &p_node,
                    &p_endc_protocolIE_elem);

            if (X2AP_P_NULL == p_node)
            {
                X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
                break;
            }
            asn1Init_x2ap_ENDCConfigurationUpdateFailure_protocolIEs_element(p_endc_protocolIE_elem);

            p_endc_protocolIE_elem->id = ASN1V_x2ap_id_TimeToWait;
            p_endc_protocolIE_elem->criticality = x2ap_ignore;

            p_endc_protocolIE_elem->value.t = T224x2ap___x2ap_ENDCConfigurationUpdateFailure_IEs_3;

            p_endc_protocolIE_elem->value.u._x2ap_ENDCConfigurationUpdateFailure_IEs_3 =
                p_endc_config_update_fail->time_to_wait;

            rtxDListAppendNode(&p_asn_endc_config_update_fail->protocolIEs, p_node);
        }        

        /*
         * ASN Encode message
         */
        pu_setBuffer(&asn1_ctx, p_asn_msg,
                     X2AP_MAX_ASN1_BUF_LEN, TRUE);

        if ( X2AP_NULL != asn1PE_x2ap_X2AP_PDU(&asn1_ctx, &x2ap_pdu) )
        {
            X2AP_TRACE(X2AP_ERROR, "ASN1 encoding of ENDC Configuration Update Failure"
                    "failed.");
            break;
        }
        else
        {
            *p_asn_msg_len= (U16)pe_GetMsgLen(&asn1_ctx);
            retVal = X2AP_SUCCESS;
           
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
            
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    (U32)*p_asn_msg_len);
            if ( PNULL != p_event_header )
            {
                x2ap_fill_protocol_event_params(p_event_header, (U32)*p_asn_msg_len,
                        LOCAL_X2_SETUP_FAILURE, 
                        MESSAGE_DIRECTION_SEND);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        (U32)*p_asn_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }
    }while(X2AP_NULL);

    /*
     * Free ASN1 context
     */
    rtFreeContext(&asn1_ctx);

    X2AP_UT_TRACE_EXIT();
    return retVal;
} 

/*****************************************************************************
 * Function Name  : x2ap_send_endc_config_update_response 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context 
 *                  p_endc_config_response - poiter to 
 *                                           x2ap_endc_config_update_resp_t
 *                  result - responce
 *                  dst_id - destination module id
 *                  trans_id - transaction id
 * Outputs        : None
 * Returns        : None
 * Description    : this function sends the eNB config update responce peer eNB
 ********************************************************************************/
void x2ap_send_endc_config_update_response
(
    x2ap_gb_context_t* p_x2ap_gb_ctx, 
    x2ap_endc_config_update_resp_t* p_endc_config_response, 
    U8 result, U16 dst_id, U16 trans_id
)
{
    if(result == X2AP_SUCCESS)
    {
        p_endc_config_response->bitmask |= 0;
        p_endc_config_response->response = X2AP_SUCCESS;
    }
    else
    {
        p_endc_config_response->bitmask |= X2AP_CAUSE_PRESENT;
        p_endc_config_response->response = X2AP_FAILURE;
    }

    X2AP_MEMCPY(&p_endc_config_response->enb_gb_id, &p_x2ap_gb_ctx->global_enb_id, sizeof(x2_gb_enb_id_t));

    if(!(rrc_x2apRrm_il_send_x2ap_endc_config_update_resp(p_endc_config_response,RRC_X2AP_MODULE_ID, dst_id, trans_id, RRC_NULL)))
    {
	X2AP_TRACE(X2AP_INFO,"Sending ENDC Configuration Update to Peer gNB failed");
    }
}

/******************************************************************************
 * Function Name  : x2ap_compose_eutra_nr_served_cell
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context 
 *                  p_asn1_ctx - Pointer to OSCTXT.
 *                  p_x2ap_IE_serv_cell - pointer to OSRTDList
 *                  p_serv_cell - pointer to x2ap_served_cell_info_arr_t
 *                  num_served_cell - num of served cells
 * Outputs        : p_x2ap_IE_serv_cell - This structure is populated with
 *                                        information from p_serv_cell.
 * Returns        : X2AP_SUCCESS - Asn Encoding was successful.
 *                  X2AP_FAILURE - Asn Encoding was not successful.
 * Description    : This function populates p_x2ap_IE_serv_cell with
 *                  information necessary to encode served cell Info.
 *****************************************************************************/
x2ap_return_et x2ap_compose_eutra_nr_served_cell
(
    x2ap_gb_context_t* p_x2ap_gb_ctx,
    OSCTXT                      *p_asn1_ctx,
    OSRTDList                   *p_x2ap_IE_serv_cell,
    x2ap_served_eutra_nr_cell_info_arr_t      *p_serv_cell,
    U16                          num_served_cell
)
{
    OSRTDListNode               *p_node_serv_cell = NULL;
    x2ap_ServedEUTRAcellsENDCX2ManagementList_element
                                *p_servedCells_elem = X2AP_P_NULL;
    U16 cell_count = 0;

    X2AP_UT_TRACE_ENTER();

    if (!num_served_cell)
    {
        X2AP_TRACE(X2AP_ERROR, "%s:Num of supported served cell.",
                   __FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }
    
    for (cell_count = 0; cell_count < num_served_cell; cell_count++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx,
            x2ap_ServedEUTRAcellsENDCX2ManagementList_element,
            &p_node_serv_cell,
            &p_servedCells_elem);
            
        if (X2AP_P_NULL == p_node_serv_cell)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }

        asn1Init_x2ap_ServedEUTRAcellsENDCX2ManagementList_element(p_servedCells_elem);
        
        /*** compose served cell Info start ***/
        if (X2AP_FAILURE == x2ap_compose_serv_cell_info_new(p_x2ap_gb_ctx,
                                            p_asn1_ctx,
                                            &p_servedCells_elem->servedEUTRACellInfo,
                                            &p_serv_cell->eutra_cell_info))
        {
            X2AP_TRACE(X2AP_ERROR,
                       "%s:compose_serv_cell_info_new returned Failure.",
                       __FUNCTION__);
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }
        /*** compose served cell info end ***/

        /*** compose neghbour cell info start ***/
        if (NR_NEIGHBOUR_INFO_PRESENT & p_serv_cell->bitmask)
        {
            p_servedCells_elem->m.nrNeighbourInfoPresent = 1;

            if (X2AP_FAILURE == x2ap_compose_nr_neighbour_info_dc(p_asn1_ctx,
                                            &p_servedCells_elem->nrNeighbourInfo,
                                            p_serv_cell->nr_neighbour_cell_info.nr_neigh_cell_arr,
                                            p_serv_cell->nr_neighbour_cell_info.num_neigh_cell))
            {
                X2AP_TRACE(X2AP_ERROR,
                           "%s:x2ap_compose_nr_neighbour_info_dc returned Failure.",
                           __FUNCTION__);
                X2AP_UT_TRACE_EXIT();
                return X2AP_FAILURE;
            }
            /*** compose neghbour cell info end ***/
        }

        rtxDListAppendNode(p_x2ap_IE_serv_cell, p_node_serv_cell);
        
        p_serv_cell++;
    }
    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;
}

/******************************************************************************
 * Function Name  : x2ap_compose_eutra_nr_served_cell_mod_arr
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context 
 *                  p_asn1_ctx - Pointer to OSCTXT.
 *                  p_x2ap_IE_serv_cell - pointer to OSRTDList
 *                  p_serv_cell - pointer to x2ap_served_cell_info_arr_t
 *                  num_served_cell - num of served cells
 * Outputs        : p_x2ap_IE_serv_cell - This structure is populated with
 *                                        information from p_serv_cell.
 * Returns        : X2AP_SUCCESS - Asn Encoding was successful.
 *                  X2AP_FAILURE - Asn Encoding was not successful.
 * Description    : This function populates p_x2ap_IE_serv_cell with
 *                  information necessary to encode served cell Info.
 *****************************************************************************/
x2ap_return_et x2ap_compose_eutra_nr_served_cell_mod_arr
(
    x2ap_gb_context_t* p_x2ap_gb_ctx,
    OSCTXT                      *p_asn1_ctx,
    OSRTDList                   *p_x2ap_IE_serv_cell,
    x2ap_served_eutra_nr_cells_to_modify_info_t      *p_serv_cell_mod,
    U16                          num_served_cell
)
{
    OSRTDListNode               *p_node_serv_cell = NULL;
    OSRTDListNode               *p_node_ecgi = NULL;
    x2ap_ServedEUTRAcellsToModifyListENDCConfUpd_element  *p_protocolIE_elem = X2AP_P_NULL;
    x2ap_ECGI *p_protocolIE_ecgi_elem = X2AP_P_NULL;
    //x2ap_ServedEUTRAcellsENDCX2ManagementList_element
    //                            *p_servedCells_elem = X2AP_P_NULL;
    U16 cell_count = 0;
    x2ap_ecgi_t old_ecgi;

    X2AP_UT_TRACE_ENTER();

    if (!num_served_cell)
    {
        X2AP_TRACE(X2AP_ERROR, "%s:Num of supported served cell.",
                   __FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }
    
    for (cell_count = 0; cell_count < num_served_cell; cell_count++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx,
            x2ap_ServedEUTRAcellsToModifyListENDCConfUpd_element,
            &p_node_serv_cell,
            &p_protocolIE_elem);
            
        if (X2AP_P_NULL == p_node_serv_cell)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }

        asn1Init_x2ap_ServedEUTRAcellsToModifyListENDCConfUpd_element(p_protocolIE_elem);

        /*** compose old ecgi start ***/
        x2ap_populate_ecgi_from_cell_id(p_x2ap_gb_ctx,
            p_serv_cell_mod->old_cell_id, &old_ecgi);
        		
        if (X2AP_FAILURE == x2ap_compose_ecgi(&p_protocolIE_elem->old_ECGI, &old_ecgi))
        {
            X2AP_TRACE(X2AP_ERROR,
                       "%s:compose_ecgi returned Failure.",
                       __FUNCTION__);
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }
        /*** compose old ecgi end ***/
        
        /*** compose served cell Info start ***/
        if (X2AP_FAILURE == x2ap_compose_serv_cell_info_new(p_x2ap_gb_ctx,
                                            p_asn1_ctx,
                                            &p_protocolIE_elem->servedEUTRACellInfo,
                                            &p_serv_cell_mod->served_eutra_nr_cell_info.eutra_cell_info))
        {
            X2AP_TRACE(X2AP_ERROR,
                       "%s:compose_serv_cell_info_new returned Failure.",
                       __FUNCTION__);
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }
        /*** compose served cell info end ***/

        /*** compose neghbour cell info start ***/
        if (NR_NEIGHBOUR_INFO_PRESENT & p_serv_cell_mod->served_eutra_nr_cell_info.bitmask)
        {
            p_protocolIE_elem->m.nrNeighbourInfoPresent = 1;

            if (X2AP_FAILURE == x2ap_compose_nr_neighbour_info_dc(p_asn1_ctx,
                                            &p_protocolIE_elem->nrNeighbourInfo,
                                            p_serv_cell_mod->served_eutra_nr_cell_info.nr_neighbour_cell_info.nr_neigh_cell_arr,
                                            p_serv_cell_mod->served_eutra_nr_cell_info.nr_neighbour_cell_info.num_neigh_cell))
            {
                X2AP_TRACE(X2AP_ERROR,
                           "%s:x2ap_compose_nr_neighbour_info_dc returned Failure.",
                           __FUNCTION__);
                X2AP_UT_TRACE_EXIT();
                return X2AP_FAILURE;
            }
            /*** compose neghbour cell info end ***/
        }

        rtxDListAppendNode(p_x2ap_IE_serv_cell, p_node_serv_cell);
        
        p_serv_cell_mod++;
    }
    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;
}

/******************************************************************************
 * Function Name  : x2ap_compose_partial_list
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context 
 *                  p_asn1_ctx - Pointer to OSCTXT.
 *                  p_x2ap_IE_serv_cell - pointer to OSRTDList
 *                  p_serv_cell - pointer to x2ap_served_cell_info_arr_t
 *                  num_served_cell - num of served cells
 * Outputs        : p_x2ap_IE_serv_cell - This structure is populated with
 *                                        information from p_serv_cell.
 * Returns        : X2AP_SUCCESS - Asn Encoding was successful.
 *                  X2AP_FAILURE - Asn Encoding was not successful.
 * Description    : This function populates p_x2ap_IE_serv_cell with
 *                  information necessary to encode served cell Info.
 *****************************************************************************/
x2ap_return_et x2ap_compose_partial_list
(
    x2ap_gb_context_t* p_x2ap_gb_ctx,
    OSCTXT                      *p_asn1_ctx,
    OSRTDList                   *p_x2ap_IE_serv_cell,
    x2ap_partial_list_t      *p_serv_cell
)
{
    OSRTDListNode               *p_node = NULL;
    x2ap_Limited_list_element *p_elem = X2AP_P_NULL;
    U16 cell_count = 0;

    X2AP_UT_TRACE_ENTER();

    if (!p_serv_cell->nrcgi_count)
    {
        X2AP_TRACE(X2AP_ERROR, "%s:Num of served nr cell.",
                   __FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }
    
    for (cell_count = 0; cell_count < p_serv_cell->nrcgi_count; cell_count++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx,
            x2ap_Limited_list_element,
            &p_node,
            &p_elem);
            
        if (X2AP_P_NULL == p_node)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }

        asn1Init_x2ap_Limited_list_element(p_elem);

        /*** compose nrcgi start ***/
        if (X2AP_FAILURE == x2ap_compose_nrcgi(&p_elem->nrCellID,
                                            &p_serv_cell->nrcgi_list[cell_count]))
        {
            X2AP_TRACE(X2AP_ERROR,
                       "%s:compose_serv_cell_info_new returned Failure.",
                       __FUNCTION__);
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }
        /*** compose nrcgi end ***/

        rtxDListAppendNode(p_x2ap_IE_serv_cell, p_node);
    }
    
    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;
}

/******************************************************************************
 * FUNCTION NAME : x2ap_compose_cell_assistance_info
 *
 * DESCRIPTION : This function used to compose ServedCells to add in ENDC
 *               Configuration Update request. 
 *
 * RETURNS : X2AP_FAILURE/X2AP_SUCCESS
 *
 * ERROR : This function report error to the caller function, If encoder fails
 *         to encode message.
 ******************************************************************************/
static x2ap_return_et x2ap_compose_cell_assistance_info
( 
    x2ap_gb_context_t* p_x2ap_gb_ctx,
    x2ap_rrm_cell_assistance_info_t   *p_x2ap_cell_assis_info,
    OSCTXT*                                            p_asn1_ctx,
    x2ap_InitiatingNodeType_EndcConfigUpdate_init_eNB_element*   p_protocolIE_elem,
    x2ap_InitiatingNodeType_EndcConfigUpdate_init_eNB*                 p_x2ap_ENBConfigUpdate 
)
{
    /* array of served cells to add*/
    OSRTDListNode*              p_node;
 
    rtxDListAllocNodeAndData(p_asn1_ctx,
        x2ap_InitiatingNodeType_EndcConfigUpdate_init_eNB_element,
        &p_node,
        &p_protocolIE_elem);

    if (X2AP_P_NULL == p_node)
    {
        X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
        return X2AP_FAILURE;
    }
        
    asn1Init_x2ap_ENDCConfigurationUpdate_protocolIEs_element(
            p_protocolIE_elem);

    p_protocolIE_elem->id = ASN1V_x2ap_id_CellAssistanceInformation;
    p_protocolIE_elem->criticality = x2ap_reject;
    p_protocolIE_elem->value.t = T217x2ap___x2ap_ENB_ENDCConfigUpdateIEs_1;
    
    p_protocolIE_elem->value.u._x2ap_ENB_ENDCConfigUpdateIEs_1
                = rtxMemAllocType(p_asn1_ctx, x2ap_CellAssistanceInformation);
                    
    if( X2AP_P_NULL == p_protocolIE_elem->value.u._x2ap_ENB_ENDCConfigUpdateIEs_1)
    {
       X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
       return X2AP_FAILURE;

    }

    asn1Init_x2ap_CellAssistanceInformation(p_protocolIE_elem->value.u._x2ap_ENB_ENDCConfigUpdateIEs_1);

    if (X2AP_CELL_ASSISTANCE_FULL_LIST_PRESENT & p_x2ap_cell_assis_info->bitmask)
    {
        p_protocolIE_elem->value.u._x2ap_ENB_ENDCConfigUpdateIEs_1->t = 
            T_x2ap_CellAssistanceInformation_full_list;
        p_protocolIE_elem->value.u._x2ap_ENB_ENDCConfigUpdateIEs_1->u.full_list =
            p_x2ap_cell_assis_info->full_nr_cells_ind;
    }

    if (X2AP_CELL_ASSISTANCE_LIMITED_LIST_PRESENT & p_x2ap_cell_assis_info->bitmask)
    {
        p_protocolIE_elem->value.u._x2ap_ENB_ENDCConfigUpdateIEs_1->t = 
            T_x2ap_CellAssistanceInformation_limited_list; 

        p_protocolIE_elem->value.u._x2ap_ENB_ENDCConfigUpdateIEs_1->u.limited_list = 
            rtxMemAllocType(p_asn1_ctx, x2ap_Limited_list);

        if (X2AP_NULL == p_protocolIE_elem->value.u._x2ap_ENB_ENDCConfigUpdateIEs_1->u.limited_list)
        {
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.", __FUNCTION__);
            return X2AP_FAILURE;
        }

        rtxDListInit(p_protocolIE_elem->value.u._x2ap_ENB_ENDCConfigUpdateIEs_1->u.limited_list);

        if ( X2AP_FAILURE == x2ap_compose_partial_list(p_x2ap_gb_ctx,
                                            p_asn1_ctx,
                                            p_protocolIE_elem->value.u._x2ap_ENB_ENDCConfigUpdateIEs_1->u.limited_list,
                                            &p_x2ap_cell_assis_info->partial_nr_cells))
        {
            X2AP_TRACE(X2AP_ERROR,
                    "%s:compose_partial_list returned Failure.",
                    __FUNCTION__);
            return X2AP_FAILURE;
        }    
    }

    rtxDListAppendNode(p_x2ap_ENBConfigUpdate, p_node);

    return X2AP_SUCCESS;
}

/******************************************************************************
 * FUNCTION NAME : x2ap_compose_eutra_nr_servedcells_to_add
 *
 * DESCRIPTION : This function used to compose ServedCells to add in ENDC
 *               Configuration Update request. 
 *
 * RETURNS : X2AP_FAILURE/X2AP_SUCCESS
 *
 * ERROR : This function report error to the caller function, If encoder fails
 *         to encode message.
 ******************************************************************************/
static x2ap_return_et x2ap_compose_eutra_nr_servedcells_to_add
( 
    x2ap_gb_context_t* p_x2ap_gb_ctx,
    x2ap_served_eutra_nr_cells_to_add_list_t   *p_x2ap_served_cell_to_add_list,
    OSCTXT*                                            p_asn1_ctx,
    x2ap_InitiatingNodeType_EndcConfigUpdate_init_eNB_element*   p_protocolIE_elem,
    x2ap_InitiatingNodeType_EndcConfigUpdate_init_eNB*                 p_x2ap_ENBConfigUpdate 
)
{
    /* array of served cells to add*/
    OSRTDListNode*              p_node;
 
    rtxDListAllocNodeAndData(p_asn1_ctx,
        x2ap_InitiatingNodeType_EndcConfigUpdate_init_eNB_element,
        &p_node,
        &p_protocolIE_elem);

    if (X2AP_P_NULL == p_node)
    {
        X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
        return X2AP_FAILURE;
    }
        
    asn1Init_x2ap_ENDCConfigurationUpdate_protocolIEs_element(
            p_protocolIE_elem);

    p_protocolIE_elem->id = ASN1V_x2ap_id_ServedEUTRAcellsENDCX2ManagementList;
    p_protocolIE_elem->criticality = x2ap_reject;
    p_protocolIE_elem->value.t = T217x2ap___x2ap_ENB_ENDCConfigUpdateIEs_2;
    
    /*Klockwork Fix*/
    p_protocolIE_elem->value.u._x2ap_ENB_ENDCConfigUpdateIEs_2
                = rtxMemAllocType(p_asn1_ctx, x2ap_ServedEUTRAcellsENDCX2ManagementList);
                    
    if( X2AP_P_NULL == p_protocolIE_elem->value.u._x2ap_ENB_ENDCConfigUpdateIEs_2)
    {
       X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
       return X2AP_FAILURE;

    }

    asn1Init_x2ap_ServedEUTRAcellsENDCX2ManagementList(p_protocolIE_elem->value.u._x2ap_ENB_ENDCConfigUpdateIEs_2);
                                      
    /*Klockwork Fix*/
 
    if ( X2AP_FAILURE == x2ap_compose_eutra_nr_served_cell(p_x2ap_gb_ctx,
                                            p_asn1_ctx,
                                            p_protocolIE_elem->value.u._x2ap_ENB_ENDCConfigUpdateIEs_2,
                                            p_x2ap_served_cell_to_add_list->eutra_nr_cell_list,
                                            p_x2ap_served_cell_to_add_list->num_served_eutra_nr_cell))
    {
        X2AP_TRACE(X2AP_ERROR,
                "%s:compose_eutra_nr_served_cell returned Failure.",
                __FUNCTION__);
        return X2AP_FAILURE;
    }
    
    if (p_x2ap_served_cell_to_add_list->num_served_eutra_nr_cell)
    {
        rtxDListAppendNode(p_x2ap_ENBConfigUpdate, p_node);
    }

    return X2AP_SUCCESS;
}

/******************************************************************************
 * FUNCTION NAME : x2ap_compose_eutra_nr_servedcells_to_modify
 *
 * DESCRIPTION : This function used to compose ServedCells to add in ENDC
 *               Configuration Update request. 
 *
 * RETURNS : X2AP_FAILURE/X2AP_SUCCESS
 *
 * ERROR : This function report error to the caller function, If encoder fails
 *         to encode message.
 ******************************************************************************/
static x2ap_return_et x2ap_compose_eutra_nr_servedcells_to_modify
( 
    x2ap_gb_context_t* p_x2ap_gb_ctx,
    x2ap_served_eutra_nr_cells_to_modify_list_t   *p_x2ap_served_cell_to_mod_list,
    OSCTXT*                                            p_asn1_ctx,
    x2ap_InitiatingNodeType_EndcConfigUpdate_init_eNB_element*   p_protocolIE_elem,
    x2ap_InitiatingNodeType_EndcConfigUpdate_init_eNB*                 p_x2ap_ENBConfigUpdate 
)
{
    /* array of served cells to add*/
    OSRTDListNode*              p_node;
 
    rtxDListAllocNodeAndData(p_asn1_ctx,
        x2ap_InitiatingNodeType_EndcConfigUpdate_init_eNB_element,
        &p_node,
        &p_protocolIE_elem);

    if (X2AP_P_NULL == p_node)
    {
        X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
        return X2AP_FAILURE;
    }
        
    asn1Init_x2ap_ENDCConfigurationUpdate_protocolIEs_element(
            p_protocolIE_elem);

    p_protocolIE_elem->id = ASN1V_x2ap_id_ServedEUTRAcellsToModifyListENDCConfUpd;
    p_protocolIE_elem->criticality = x2ap_reject;
    p_protocolIE_elem->value.t = T217x2ap___x2ap_ENB_ENDCConfigUpdateIEs_3;
    
    /*Klockwork Fix*/
    p_protocolIE_elem->value.u._x2ap_ENB_ENDCConfigUpdateIEs_3
                = rtxMemAllocType(p_asn1_ctx, x2ap_ServedEUTRAcellsToModifyListENDCConfUpd);
                    
    if( X2AP_P_NULL == p_protocolIE_elem->value.u._x2ap_ENB_ENDCConfigUpdateIEs_3)
    {
       X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
       return X2AP_FAILURE;

    }

    asn1Init_x2ap_ServedEUTRAcellsToModifyListENDCConfUpd(p_protocolIE_elem->value.u._x2ap_ENB_ENDCConfigUpdateIEs_3);
                                      
    /*Klockwork Fix*/
 
    if ( X2AP_FAILURE == x2ap_compose_eutra_nr_served_cell_mod_arr(p_x2ap_gb_ctx,
                                            p_asn1_ctx,
                                            p_protocolIE_elem->value.u._x2ap_ENB_ENDCConfigUpdateIEs_3,
                                            p_x2ap_served_cell_to_mod_list->eutra_nr_cell_list,
                                            p_x2ap_served_cell_to_mod_list->num_served_eutra_nr_cell))
    {
        X2AP_TRACE(X2AP_ERROR,
                "%s:x2ap_compose_eutra_nr_served_cell_mod_arr returned Failure.",
                __FUNCTION__);
        return X2AP_FAILURE;
    }
    
    if (p_x2ap_served_cell_to_mod_list->num_served_eutra_nr_cell)
    {
        rtxDListAppendNode(p_x2ap_ENBConfigUpdate, p_node);
    }

    return X2AP_SUCCESS;
}

/******************************************************************************
 * FUNCTION NAME : x2ap_compose_eutra_nr_servedcells_to_delete
 *
 * DESCRIPTION : This function used to compose ServedCells to add in ENDC
 *               Configuration Update request. 
 *
 * RETURNS : X2AP_FAILURE/X2AP_SUCCESS
 *
 * ERROR : This function report error to the caller function, If encoder fails
 *         to encode message.
 ******************************************************************************/
static x2ap_return_et x2ap_compose_eutra_nr_servedcells_to_delete
( 
    x2ap_gb_context_t* p_x2ap_gb_ctx,
    x2ap_served_eutra_nr_cells_to_delete_list_t   *p_x2ap_served_cell_to_del_list,
    OSCTXT*                                            p_asn1_ctx,
    x2ap_InitiatingNodeType_EndcConfigUpdate_init_eNB_element*   p_protocolIE_elem,
    x2ap_InitiatingNodeType_EndcConfigUpdate_init_eNB*                 p_x2ap_ENBConfigUpdate 
)
{
    /* array of served cells to add*/
    OSRTDListNode*              p_node;
    /*Klockwork Fix*/
    x2ap_ServedEUTRAcellsToDeleteListENDCConfUpd* p_x2ap_ENDCConfigurationUpdate_IEs_4 = X2AP_P_NULL ;
    /*Klockwork Fix*/
 
    rtxDListAllocNodeAndData(p_asn1_ctx,
        x2ap_InitiatingNodeType_EndcConfigUpdate_init_eNB_element,
        &p_node,
        &p_protocolIE_elem);

    if (X2AP_P_NULL == p_node)
    {
        X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
        return X2AP_FAILURE;
    }
        
    asn1Init_x2ap_ENDCConfigurationUpdate_protocolIEs_element(
            p_protocolIE_elem);

    p_protocolIE_elem->id = ASN1V_x2ap_id_ServedEUTRAcellsToDeleteListENDCConfUpd;
    p_protocolIE_elem->criticality = x2ap_reject;
    p_protocolIE_elem->value.t = T217x2ap___x2ap_ENB_ENDCConfigUpdateIEs_4;
    
    /*Klockwork Fix*/
    p_x2ap_ENDCConfigurationUpdate_IEs_4
                = rtxMemAllocType(p_asn1_ctx, x2ap_ServedEUTRAcellsToDeleteListENDCConfUpd);
                    
    if( X2AP_P_NULL == p_x2ap_ENDCConfigurationUpdate_IEs_4)
    {
       X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
       return X2AP_FAILURE;
    }

    asn1Init_x2ap_ServedEUTRAcellsToDeleteListENDCConfUpd(p_x2ap_ENDCConfigurationUpdate_IEs_4);
                                      
    p_protocolIE_elem->value.u._x2ap_ENB_ENDCConfigUpdateIEs_4 =
        p_x2ap_ENDCConfigurationUpdate_IEs_4;
    /*Klockwork Fix*/
 
    if ( X2AP_FAILURE == x2ap_compose_old_ECGIs(p_x2ap_gb_ctx,
                                            p_asn1_ctx,
                                            p_protocolIE_elem->value.u._x2ap_ENB_ENDCConfigUpdateIEs_4,
                                            (x2ap_served_cells_to_delete_list_t *)p_x2ap_served_cell_to_del_list))
    {
        X2AP_TRACE(X2AP_ERROR,
                "%s:x2ap_compose_old_ECGIs returned Failure.",
                __FUNCTION__);
        return X2AP_FAILURE;
    }
    
    if (p_x2ap_served_cell_to_del_list->num_served_cell)
    {
        rtxDListAppendNode(p_x2ap_ENBConfigUpdate, p_node);
    }

    return X2AP_SUCCESS;
}

/*****************************************************************************
 * Function Name  : x2ap_encode_ENDC_Configuration_Update_req 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_endc_config_update_req - pointer to x2ap_endc_config_update_req_t
 *                  p_asn_msg - pointer to encoded ASN msg 
 *                  p_asn_msg_len - pointer to encoded ASN msg length 
 * Outputs        : p_asn_msg - encoded ASN msg
 *                  p_asn_msg_len - encoded ASN msg length 
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to encode ENDC Configuration Update request.
 * ERROR          : This function report error to the caller function, If encoder
 *                  fails to encode message. 
 ********************************************************************************/
x2ap_return_et x2ap_encode_ENDC_Configuration_Update_req
(
    x2ap_gb_context_t* p_x2ap_gb_ctx,
    x2ap_endc_config_update_req_t   *p_endc_config_update_req,
    U8                     *p_asn_msg,
    U16                    *p_asn_msg_len
)
{
    x2ap_return_et  retVal = X2AP_FAILURE;

    OSCTXT  asn1_ctx;
    x2ap_X2AP_PDU   x2ap_pdu;

    /* 
    ** ASN generated structure having ENDCConfigurationUpdate information objects  
    ** and their selector.
    */
    x2ap_ENDCConfigurationUpdate_protocolIEs_element *p_protocolIE_elem = X2AP_P_NULL;
    x2ap_InitiatingNodeType_EndcConfigUpdate_init_eNB_element 
        *p_endc_configupdate_init_enb_elem = X2AP_P_NULL;
    x2ap_InitiatingNodeType_EndcConfigUpdate_init_eNB 
        *p_endc_configupdate_init_enb = X2AP_P_NULL;

    OSRTDListNode               *p_node = X2AP_P_NULL;    

    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    X2AP_UT_TRACE_ENTER();
    
     /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        X2AP_TRACE(X2AP_ERROR,"%s:ASN context initialization failed.",
                   __FUNCTION__);
        return retVal;
    }
    
     do
    {
         X2AP_MEMSET( &(x2ap_pdu), 0, sizeof(x2ap_X2AP_PDU) );
        /*
        ** Fill the values in the ASN structures that shall be encoded by
        ** ASN Encoder
        */
        /* Set Pdu type to Initiating message */
        x2ap_pdu.t = T_x2ap_X2AP_PDU_initiatingMessage;

        /* X2AP Fix_AG_1 : Start */
        x2ap_pdu.u.initiatingMessage = rtxMemAllocType(&asn1_ctx, x2ap_InitiatingMessage);
        if (X2AP_P_NULL == x2ap_pdu.u.initiatingMessage)
        {
            X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        /* X2AP Fix_AG_1 : End */

        asn1Init_x2ap_InitiatingMessage(x2ap_pdu.u.initiatingMessage);
        /* fill procedure code for ENDCConfigurationUpdate */
        x2ap_pdu.u.initiatingMessage->procedureCode =
                                    ASN1V_x2ap_id_endcConfigurationUpdate;
        /* fill criticality of message type */
        x2ap_pdu.u.initiatingMessage->criticality = x2ap_reject;

        /* Set the initiating message type to eNB Configuration Update */
        x2ap_pdu.u.initiatingMessage->value.t = T1x2ap__endcConfigurationUpdate;

        x2ap_pdu.u.initiatingMessage->value.u.endcConfigurationUpdate  = rtxMemAllocType(&asn1_ctx,
                                                x2ap_ENDCConfigurationUpdate);
                                                
        if (X2AP_P_NULL == x2ap_pdu.u.initiatingMessage->value.u.endcConfigurationUpdate)
        {
             X2AP_TRACE(X2AP_ERROR,"%s:ASN malloc failed.",__FUNCTION__);
             break;
        }
        
         asn1Init_x2ap_ENDCConfigurationUpdate(x2ap_pdu.u.initiatingMessage->value.u.endcConfigurationUpdate);   

        rtxDListAllocNodeAndData(&asn1_ctx,
                x2ap_ENDCConfigurationUpdate_protocolIEs_element,
                &p_node,
                &p_protocolIE_elem);
                
        if (X2AP_P_NULL == p_node)
        {
            /* Not enough memory */
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }
        
        asn1Init_x2ap_ENDCConfigurationUpdate_protocolIEs_element(p_protocolIE_elem);

        /* Fill protocol IE Id and criticality */
        p_protocolIE_elem->id = ASN1V_x2ap_id_InitiatingNodeType_EndcConfigUpdate;
        p_protocolIE_elem->criticality = x2ap_reject;

        /* Fill the type of Protocol IE element */
        p_protocolIE_elem->value.t = T216x2ap___x2ap_ENDCConfigurationUpdate_IEs_1;

        p_protocolIE_elem->value.u._x2ap_ENDCConfigurationUpdate_IEs_1 = rtxMemAllocType(
                &asn1_ctx, x2ap_InitiatingNodeType_EndcConfigUpdate);

        if (X2AP_P_NULL == p_protocolIE_elem->value.u._x2ap_ENDCConfigurationUpdate_IEs_1)
        {
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        asn1Init_x2ap_InitiatingNodeType_EndcConfigUpdate(p_protocolIE_elem->
                value.u._x2ap_ENDCConfigurationUpdate_IEs_1);

        p_protocolIE_elem->value.u._x2ap_ENDCConfigurationUpdate_IEs_1->t = 
                T_x2ap_InitiatingNodeType_EndcConfigUpdate_init_eNB;

        p_protocolIE_elem->value.u._x2ap_ENDCConfigurationUpdate_IEs_1->u.init_eNB =
                rtxMemAllocType(&asn1_ctx, x2ap_InitiatingNodeType_EndcConfigUpdate_init_eNB);

        if (X2AP_P_NULL ==  p_protocolIE_elem->value.u._x2ap_ENDCConfigurationUpdate_IEs_1->u.init_eNB)
        {
            /* Not enough memory */
            X2AP_TRACE(X2AP_ERROR, "%s:ASN malloc failed.",__FUNCTION__);
            break;
        }

        rtxDListInit(p_protocolIE_elem->value.u._x2ap_ENDCConfigurationUpdate_IEs_1->u.init_eNB);

        p_endc_configupdate_init_enb = p_protocolIE_elem->value.u._x2ap_ENDCConfigurationUpdate_IEs_1->u.init_eNB;

        /*** Compose id-Cell assistance info start ***/
        if (X2AP_ENDC_CONFIGUPDATE_ENB_CELLASSISTANCE_INFO_PRESENT & 
            p_endc_config_update_req->bitmask)
        {
            if(X2AP_SUCCESS != x2ap_compose_cell_assistance_info(p_x2ap_gb_ctx,
                                  &p_endc_config_update_req->cell_assistance_info,&asn1_ctx,
                                  p_endc_configupdate_init_enb_elem,p_endc_configupdate_init_enb))
            {
                break;
            }
        }
        /*** Compose id-Cell assistance info end ***/
        
        /*** Compose id-ServedCells to add start ***/
        if (X2AP_SERVED_EUTRA_CELLS_TO_ADD_LIST_PRESENT & 
            p_endc_config_update_req->bitmask)
        {
            if(X2AP_SUCCESS != x2ap_compose_eutra_nr_servedcells_to_add(p_x2ap_gb_ctx,
                                  &p_endc_config_update_req->add_served_cell,&asn1_ctx,
                                  p_endc_configupdate_init_enb_elem,p_endc_configupdate_init_enb))
            {
                break;
            }
        }
        /*** Compose id-ServedCells to add end ***/

        /*** Compose id-ServedCells to modify start ***/
        if (X2AP_SERVED_EUTRA_CELLS_TO_MODIFY_LIST_PRESENT & 
            p_endc_config_update_req->bitmask)
        {
            if(X2AP_SUCCESS != x2ap_compose_eutra_nr_servedcells_to_modify(p_x2ap_gb_ctx,
                                  &p_endc_config_update_req->mod_served_cell,&asn1_ctx,
                                  p_endc_configupdate_init_enb_elem,p_endc_configupdate_init_enb))
            {
                break;
            }
        }
        /*** Compose id-ServedCells to add end ***/

         /*** Compose id-ServedCells to delete start ***/
        if (X2AP_SERVED_EUTRA_CELLS_TO_DELETE_LIST_PRESENT & 
            p_endc_config_update_req->bitmask)
        {
            if(X2AP_SUCCESS != x2ap_compose_eutra_nr_servedcells_to_delete(p_x2ap_gb_ctx,
                              &p_endc_config_update_req->del_served_cell,&asn1_ctx,
                              p_endc_configupdate_init_enb_elem,p_endc_configupdate_init_enb))
            {
                break;
            }
        }
        /*** Compose id-ServedCells to delete end ***/

        rtxDListAppendNode(&x2ap_pdu.u.initiatingMessage->value.u.endcConfigurationUpdate->protocolIEs, p_node);

        /* ASN Encode message */
        pu_setBuffer(&asn1_ctx, p_asn_msg,
                     X2AP_MAX_ASN1_BUF_LEN, TRUE);

        if (0 != asn1PE_x2ap_X2AP_PDU(&asn1_ctx, &x2ap_pdu))
        {
            rtxErrPrint(&asn1_ctx);
            X2AP_TRACE(X2AP_ERROR, "ASN1 encoding of ENDC Configuration Update "
            "Request failed.");
            break;
        }
        else
        {
            *p_asn_msg_len= (U16)pe_GetMsgLen(&asn1_ctx);
            retVal = X2AP_SUCCESS;
            
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
            
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    (U32)*p_asn_msg_len);
                    
            if (PNULL != p_event_header)
            {
                x2ap_fill_protocol_event_params(p_event_header, (U32)*p_asn_msg_len,
                        LOCAL_X2_ENB_CONFIGURATION_UPDATE, 
                        MESSAGE_DIRECTION_SEND);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        (U32)*p_asn_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

    }while(0);

    /* Free ASN1 context */
    rtFreeContext(&asn1_ctx);

    X2AP_UT_TRACE_EXIT();
    return retVal;
}	

/*EN-DC_changes_end*/
#endif
