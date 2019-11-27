/*******************************************************************************
*
*  FILE NAME   : asn_dec_wrappers.c
*
*  DESCRIPTION : This file contains the wrapper function of ASN Decoders.
*
*  DATE 24-June-2010  NAME Ashutosh Gupta  REFERENCE  36.423_880(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/

#include "rtxsrc/rtxCommon.h"
#include "x2ap_asn_enc_dec_3gpp.h"
#include "x2ap_tracing.h"
#include "x2ap_utils.h"
#include "rrc_common_utils.h"
#include "asn_dec_wrappers.h"
#include "x2ap_uecc.h"
#include "rrc_logging.h"

#ifdef ENDC_ENABLED
/*******************************************************************************
 * Function Name  : x2ap_parse_nr_neigh_info_endc_x2_management_list
 * Inputs         : x2ap_NRNeighbour_Information             *p_nr_neigh_info_src,
 *                  x2ap_nr_cell_info_t                      *p_nr_cell_info
 *
 * Outputs        : x2ap_nr_cell_info_t                       nr_neigh_cell_arr[]
 *
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode NR Neighbour information
 *******************************************************************************/

x2ap_return_et
x2ap_parse_nr_neigh_info_endc_x2_management_list
(
     x2ap_NRNeighbour_Information             *p_nr_neigh_info_src,
     x2ap_nr_cell_info_t                      *p_nr_cell_info,
     x2ap_bool_et                             *p_send_tsc
);

/*****************************************************************************
 * Function Name  : x2ap_internal_decode_meas_timing_config 
 * Inputs         : OSDynOctStr - pointer to ASN measurement timing config
 *                  string
 *                  nr_rrc_MeasurementTimingConfiguration - pointer to
 *                  ASN measurement timing config
 *                  p_asn1_ctx - pointer to OSCTXT
 * Outputs        : meas_timing_config
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE 
 * Description    : This function used to decode measurement timing configuration
 *                  for EN DC X2 setup response.
 ********************************************************************************/
x2ap_return_et
x2ap_internal_decode_meas_timing_config
(
    OSDynOctStr                              *p_meas_timing_config,
    nr_rrc_MeasurementTimingConfiguration    *p_asn_meas_timing_config,
    OSCTXT                                   *p_asn1_ctx
);

x2ap_return_et
x2ap_parse_nr_svrd_info_endc_x2_management_list
(
     x2ap_ServedNRCell_Information             *p_served_nr_cell_Info_src,
     x2ap_srvd_nr_cell_info_t                      *p_served_nr_cell_info,
     x2ap_bool_et                             *p_send_tsc,
     OSCTXT                                  *p_asn1_ctx
);

#endif
/* GDB Fix - compiler warning */
extern void x2ap_compose_cell_id_from_ecgi(x2_gb_enb_id_t global_enb_id, U8* eutran_cell_id, U8* p_cell_id);


x2ap_return_et  x2ap_parse_ServedCellsToModify(
        U16                            *p_num_srv_cell_to_mod,
        x2ap_served_cells_to_modify_list_t *p_served_cells_to_mod_info,
        x2ap_ServedCellsToModify        *p_ServedCellsToModify,
        x2ap_peer_enb_context_t* p_enb_cntx);



x2ap_return_et  x2ap_parse_ServedCellsToDelete(
        U16                            *p_num_srv_cell_to_del,
        U8                   *p_old_cell_id,
        x2ap_Old_ECGIs                 *p_Old_ECGIs,
        x2ap_peer_enb_context_t* p_enb_cntx);

x2ap_return_et  x2ap_parse_CriticalityDiagnostics(
        x2ap_crit_diagno_t            *p_CriticalityDiagno,
        x2ap_CriticalityDiagnostics    *p_CriticalityDiagnostics);

x2ap_return_et  x2ap_parse_GUGroupIDList(
        U8                          *p_num_GUGroupID,
        x2ap_gu_group_id_t         *p_GUGroupIDList,
        x2ap_GUGroupIDList          *p_GU_Group_ID_List,
        x2ap_message_data_t *p_ie_order_map,
        U8 id,
        x2ap_bool_et *p_send_tse);

x2ap_return_et  x2ap_parse_ServedCells(
        U16                          *p_num_srv_cell,
        x2ap_srvd_cell_arry_t       *p_served_cells_info,
        x2ap_ServedCells             *p_ServedCells,
        x2ap_message_data_t *p_ie_order_map,
        U8 order_index,
        U8 id,
        x2ap_bool_et *p_send_tse);

x2ap_return_et  x2ap_parse_GlobalENB_ID(
        x2_gb_enb_id_t         *p_glb_enbId,
        x2ap_GlobalENB_ID    *p_glb_enb_id,
        x2ap_message_data_t *p_ie_order_map,
        U8 order_index,
        U8 id);

extern void x2ap_reset_parse_asn_x2ap_cause(
        x2ap_Cause asn_cause,
        x2ap_cause_t* p_cause);

static void rrc_x2ap_internal_decode_handover_command(
        x2ap_TargeteNBtoSource_eNBTransparentContainer *p_container,
        rrc_x2ap_handover_req_ack_t *p_intrl,
        OSCTXT      *p_asn1_ctx,
        x2ap_Cause *cause);

static void rrc_x2ap_internal_decode_handover_preparation_information(
        /*SPR_18125_START*/
        OSCTXT           *p_asn1_ctx, 
        /*SPR_18125_END*/
        x2ap_RRC_Context *p_rRC_Context,
        HandoverPreparationInformation_r8_IEs  *p_intrl,
        x2ap_Cause       *cause);
/*valgrind_fix_start*/
/* Pointer to ASN Context */
/*valgrind_fix_stop*/

static x2ap_return_et partially_decode_X2AP_PDU (
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t   *p_enb_cntxt_arr,
        OSCTXT* pctxt,
        x2ap_EP_et * p_x2ap_EP,
        x2ap_ProcedureCode  * p_proc_code,
        x2ap_UE_X2AP_ID     * p_oldEnbUeX2apId,
        x2ap_UE_X2AP_ID     * p_newEnbUeX2apId
#ifdef ENDC_ENABLED
        /*NR_DC Code Change Start*/
        ,U32  *p_mEnbUeX2apId,
        U32  *p_sgnbUeX2apId
        /*NR_DC Code Change Stop*/
#endif
        );

/*****************************************************************************
 * Function Name  : x2ap_partially_decode_asn_msg
 * Inputs         : p_x2ap_gb_ctx - pointer to X2ap global context 
 *                  p_enb_cntxt_arr - pointer to x2ap_peer_enb_context_t
 *                  p_asn_msg - Pointer to Asn message to be partially
 *                              decoded.
 *                  asn_msg_len - The length of the Asn Message.
 *                  p_x2ap_EP - pointer to x2ap_EP_et
 *                  p_proc_code - pointer to x2ap_ProcedureCode
 *                  p_oldEnbUeX2apId - pointer to x2ap_UE_X2AP_ID
 *                  p_newEnbUeX2apId -  pointer to x2ap_UE_X2AP_ID
 * Outputs        : p_x2ap_EP- The X2AP EP received.
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE 
 * Description    : This function when called partially decodes a Asn message
 *                  and returns X2AP EP type.
 *****************************************************************************/
x2ap_return_et x2ap_partially_decode_asn_msg (
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t   *p_enb_cntxt_arr,
        U8 * p_asn_msg,
        U16 asn_msg_len,
        x2ap_EP_et  *p_x2ap_EP,
        x2ap_ProcedureCode       *p_proc_code,
        x2ap_UE_X2AP_ID     * p_oldEnbUeX2apId,
        x2ap_UE_X2AP_ID     * p_newEnbUeX2apId
#ifdef ENDC_ENABLED
        /*NR_DC Code Change Start*/
        ,U32                       *p_mEnbUeX2apId,
        U32                        *p_sgnbUeX2apId
        /*NR_DC Code Change Stop*/
#endif
        )
{
    OSCTXT asn1_ctx;
    /* SPR 19838 : Coverity 11397 fix start */
    /* Code Deleted */
    /* SPR 19838 : Coverity 11397 fix end */
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    U32                         encoded_msg_len = 0;
    X2AP_UT_TRACE_ENTER();

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        /* Drop message */
        X2AP_TRACE(X2AP_ERROR, "x2ap_partially_decoded_t: ASN "\
                "context initialization failed.");
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(&asn1_ctx, p_asn_msg, asn_msg_len, TRUE);

    if( X2AP_FAILURE == partially_decode_X2AP_PDU(
                p_x2ap_gb_ctx,
                p_enb_cntxt_arr,
                &asn1_ctx,
                p_x2ap_EP, p_proc_code,
                p_oldEnbUeX2apId, p_newEnbUeX2apId
#ifdef ENDC_ENABLED
                /*NR_DC Code Change Start*/
                ,p_mEnbUeX2apId, p_sgnbUeX2apId
                /*NR_DC Code Change Stop*/
#endif
                ))
    {
        X2AP_TRACE(X2AP_ERROR, "ASN X2AP_PDU partial decode failed");
        /*SPR_18125_START*/
        rtFreeContext(&asn1_ctx);
        /*SPR_18125_END*/
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    if ((*p_proc_code) == ASN1V_x2ap_id_privateMessage)
    {
        encoded_msg_len = (U32)pe_GetMsgLen(&asn1_ctx);

        /* SPR 19838 : Coverity 11397 fix start */
        /* Code Deleted */
        /* SPR 19838 : Coverity 11397 fix end */
        p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                encoded_msg_len);
        if (PNULL != p_event_header)
        {
            x2ap_fill_protocol_event_params(p_event_header, encoded_msg_len,
                    LOCAL_EVENT_X2_PRIVATE_MESSAGE, 
                    MESSAGE_DIRECTION_RECEIVED);

            l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                        sizeof(EVENT_EXTERNAL_HEADER)),
                    (const void*)p_asn_msg,
                    encoded_msg_len);

            rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
        }
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;
}

/*****************************************************************************
 * Function Name  : decode_EnbUeX2apid
 * Inputs         : pctxt - Pointer to OSCTXT.
 *                  p_oldEnbUeX2apId - pointer to x2ap_UE_X2AP_ID
 *                  p_newEnbUeX2apId - pointer to x2ap_UE_X2AP_ID
 * Outputs        : decodes p_oldEnbUeX2apId and
 *                  p_newEnbUeX2apId
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function when called decodes Enb_Ue_X2apId
 *****************************************************************************/
static x2ap_return_et decode_EnbUeX2apid (
        OSCTXT              * pctxt,
        x2ap_UE_X2AP_ID     *p_oldEnbUeX2apId,
        x2ap_UE_X2AP_ID     *p_newEnbUeX2apId)
{

    OSCTXT lctxt;
    ASN1OpenType openType;
    S32 stat = 0;
    OSBOOL extbit = FALSE;
    U32 count = 0;
    U32 xx1 = 0;
    U32 ui;

    x2ap_ProtocolIE_ID    protocol_id = 0;

    X2AP_UT_TRACE_ENTER();

    *p_oldEnbUeX2apId = INVALID_UE_X2AP_ID;
    *p_newEnbUeX2apId = INVALID_UE_X2AP_ID;
    stat = pd_OpenType (pctxt, &openType.data, &openType.numocts);
    if (stat != 0) 
    {
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    /* decode open type data into specific type field */

    rtxCopyContext (&lctxt, pctxt);

    stat = pd_setp (pctxt, (OSOCTET*)openType.data,
            openType.numocts, pctxt->buffer.aligned);

    /* extension bit */

    stat = PD_BIT (pctxt, &extbit);

    if (stat != 0) 
    {
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    /* decode length determinant */
    PU_SETSIZECONSTRAINT (pctxt, OSUINTCONST(0), OSUINTCONST(65535), 0, 0);

    stat = pd_Length (pctxt,((OSUINT32*)&count));
    if (stat != 0) 
    {
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    /* decode elements */
    count = count < MAX_ELEM_TO_PARTIAL_DECODE ? count :
        MAX_ELEM_TO_PARTIAL_DECODE;

    for (xx1 = 0; xx1 < count; xx1++) 
    {
        /* Decode Protocol Id */
        stat = asn1PD_x2ap_ProtocolIE_ID (pctxt, &protocol_id);
        if (stat != 0) 
        {
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }

        /* decode criticality */
        stat = asn1PD_x2ap_Criticality (pctxt, ((x2ap_Criticality*)&ui));
        if (stat != 0) 
        {
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }

        {
            OSCTXT lctxt1;
            ASN1OpenType openType1;

            stat = pd_OpenType (pctxt, &openType1.data, &openType1.numocts);
            if (stat != 0) 
            {
                X2AP_UT_TRACE_EXIT();
                return X2AP_FAILURE;
            }

            /* decode open type data into specific type field */

            rtxCopyContext (&lctxt1, pctxt);

            stat = pd_setp (pctxt, (OSOCTET*)openType1.data,
                    openType1.numocts, pctxt->buffer.aligned);
            if (stat != 0) 
            {
                X2AP_UT_TRACE_EXIT();
                return X2AP_FAILURE;
            }

            switch (protocol_id) {
                case ASN1V_x2ap_id_Old_eNB_UE_X2AP_ID:
                stat = asn1PD_x2ap_UE_X2AP_ID (pctxt, p_oldEnbUeX2apId);
                if (stat != 0) 
                {
                    X2AP_UT_TRACE_EXIT();
                    return X2AP_FAILURE;
                }
                break;

                case ASN1V_x2ap_id_New_eNB_UE_X2AP_ID:
                stat = asn1PD_x2ap_UE_X2AP_ID (pctxt, p_newEnbUeX2apId);
                if (stat != 0) 
                {
                    X2AP_UT_TRACE_EXIT();
                    return X2AP_FAILURE;
                }
                break;

                default :
                break;
            }
            rtxCopyContext (pctxt, &lctxt1);

        }
    }
    rtxCopyContext (pctxt, &lctxt);

    X2AP_TRACE(X2AP_INFO, "Decoded OldEnbUeX2apId : 0X%x NewEnbUeX2apId : 0X%x",
            *p_oldEnbUeX2apId, *p_newEnbUeX2apId);

    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;

}

#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
/*****************************************************************************
 * Function Name  : decode_dcUeX2apid
 * Inputs         : pctxt - Pointer to OSCTXT.
 *                  p_mEnbUeX2apId - pointer to x2ap_UE_X2AP_ID
 *                  p_sgnbUeX2apId - pointer to x2ap_UE_X2AP_ID
 * Outputs        : decodes p_mEnbUeX2apId and
 *                  p_sgnbUeX2apId
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function when called decodes mEnb_Ue_X2apId
 *****************************************************************************/
static x2ap_return_et decode_dcUeX2apid (
        OSCTXT              *pctxt,
        U32                 *p_mEnbUeX2apId,
        U32                 *p_sgnbUeX2apId
)
{

    OSCTXT lctxt;
    ASN1OpenType openType;
    S32 stat = 0;
    OSBOOL extbit = FALSE;
    U32 count = 0;
    U32 xx1 = 0;
    U32 ui;

    x2ap_ProtocolIE_ID    protocol_id = 0;

    X2AP_UT_TRACE_ENTER();

    *p_mEnbUeX2apId = RRC_NULL;
    *p_sgnbUeX2apId = RRC_NULL;
    stat = pd_OpenType (pctxt, &openType.data, &openType.numocts);
    if (stat != 0) 
    {
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    /* decode open type data into specific type field */

    rtxCopyContext (&lctxt, pctxt);

    stat = pd_setp (pctxt, (OSOCTET*)openType.data,
            openType.numocts, pctxt->buffer.aligned);

    /* extension bit */

    stat = PD_BIT (pctxt, &extbit);

    if (stat != 0) 
    {
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    /* decode length determinant */
    PU_SETSIZECONSTRAINT (pctxt, OSUINTCONST(0), OSUINTCONST(65535), 0, 0);

    stat = pd_Length (pctxt,((OSUINT32*)&count));
    if (stat != 0) 
    {
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    for (xx1 = 0; xx1 < count; xx1++) 
    {
        /* Decode Protocol Id */
        stat = asn1PD_x2ap_ProtocolIE_ID (pctxt, &protocol_id);
        if (stat != 0) 
        {
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }

        /* decode criticality */
        stat = asn1PD_x2ap_Criticality (pctxt, ((x2ap_Criticality*)&ui));
        if (stat != 0) 
        {
            X2AP_UT_TRACE_EXIT();
            return X2AP_FAILURE;
        }

        {
            OSCTXT lctxt1;
            ASN1OpenType openType1;

            stat = pd_OpenType (pctxt, &openType1.data, &openType1.numocts);
            if (stat != 0) 
            {
                X2AP_UT_TRACE_EXIT();
                return X2AP_FAILURE;
            }

            /* decode open type data into specific type field */

            rtxCopyContext (&lctxt1, pctxt);

            stat = pd_setp (pctxt, (OSOCTET*)openType1.data,
                    openType1.numocts, pctxt->buffer.aligned);
            if (stat != 0) 
            {
                X2AP_UT_TRACE_EXIT();
                return X2AP_FAILURE;
            }

            switch (protocol_id) {
                case ASN1V_x2ap_id_MeNB_UE_X2AP_ID:
                stat = asn1PD_x2ap_UE_X2AP_ID (pctxt, (x2ap_UE_X2AP_ID *)p_mEnbUeX2apId);
                if (stat != 0) 
                {
                    X2AP_UT_TRACE_EXIT();
                    return X2AP_FAILURE;
                }
                break;
                case ASN1V_x2ap_id_SgNB_UE_X2AP_ID:
                stat = asn1PD_x2ap_SgNB_UE_X2AP_ID(pctxt, 
                        (x2ap_SgNB_UE_X2AP_ID *)p_sgnbUeX2apId);
                if (stat != 0) 
                {
                    X2AP_UT_TRACE_EXIT();
                    return X2AP_FAILURE;
                }
                break;

                default :
                break;
            }
            rtxCopyContext (pctxt, &lctxt1);

        }
    }


    X2AP_TRACE(X2AP_INFO, "Decoded mEnbUeX2apId : 0X%x sgnbUeX2apId : 0X%x",
            *p_mEnbUeX2apId, *p_sgnbUeX2apId);

    rtxCopyContext (pctxt, &lctxt);

    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;
}
/*NR_DC Code Change Stop*/
#endif

/*****************************************************************************
 * Function Name  : determine_unsuccessfulOutcome_proc_code
 * Inputs         : p_x2ap_gb_ctx
 *                  p_enb_cntxt_arr
 *                  pctxt
 *                  p_proc_code
 *                  p_oldEnbUeX2apId
 *                  p_newEnbUeX2apId
 *                  criticality
 * Outputs        : p_x2ap_EP
 * Returns        : X2AP_SUCCESS 
 *                  X2AP_FAILURE 
 * Description    : This function is used to process the ProcedureCode for
 *                  UnsuccessOutcome messages
 *****************************************************************************/
static x2ap_return_et determine_unsuccessfulOutcome_proc_code(
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t   *p_enb_cntxt_arr,
        OSCTXT              * pctxt,
        x2ap_EP_et          * p_x2ap_EP,
        x2ap_ProcedureCode  * p_proc_code,
        x2ap_UE_X2AP_ID     * p_oldEnbUeX2apId,
        x2ap_UE_X2AP_ID     * p_newEnbUeX2apId,
#ifdef ENDC_ENABLED
        /*NR_DC Code Change Start*/
        U32                        * p_mEnbUeX2apId,
        U32                        * p_sgnbUeX2apId,
        /*NR_DC Code Change Stop*/
#endif
        x2ap_Criticality      criticality)
{
    x2ap_return_et stat = X2AP_FAILURE; 
    rrc_x2ap_error_indication_t error_ind;

    X2AP_UT_TRACE_ENTER();
    X2AP_MEMSET (&error_ind, X2AP_NULL, sizeof(rrc_x2ap_error_indication_t));

    if (!pctxt || !p_x2ap_EP )
    {
        return X2AP_FAILURE;
    }

    switch (*p_proc_code)
    {
#ifdef ENDC_ENABLED
        /* MENB CHNAGES - START */
        case ASN1V_x2ap_id_endcX2Setup:
        *p_x2ap_EP = ENDC_X2SETUP_FAIL;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode Success for"
                "EN_DC_X2_SETUP_FAIL",__FUNCTION__);
        break;
        case ASN1V_x2ap_id_sgNBAdditionPreparation:                  
        *p_x2ap_EP = X2AP_UE_ASSO_SIG_MSG_IND_SGNB_ADD_REQ_REJ;
        X2AP_TRACE(X2AP_DETAILED, "%s: Partial Decode Success for"
                "procedure code %u",__FUNCTION__, *p_proc_code);
        break;                    
        /* MENB CHNAGES - END */
#endif

        case ASN1V_x2ap_id_x2Setup:
        *p_x2ap_EP = X2_SETUP_FAIL;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Success for X2_SETUP_FAIL",__FUNCTION__);
        break;
        case ASN1V_x2ap_id_eNBConfigurationUpdate:
        *p_x2ap_EP = ENB_CONFIGURE_UPDATE_FAIL;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Success for ENB_CONFIGURE_UPDATE_FAIL"
                ,__FUNCTION__);
        break;
        case ASN1V_x2ap_id_handoverPreparation:
        *p_x2ap_EP = X2AP_UE_ASSO_SIG_MSG_IND_OTHERS;
        stat = decode_EnbUeX2apid (pctxt, p_oldEnbUeX2apId, p_newEnbUeX2apId);
        if(stat != X2AP_SUCCESS )
        {
            X2AP_TRACE(X2AP_INFO, "%s: Decode decode_EnbUeX2apid"
                    "failed for handover Prepration failure"
                    ,__FUNCTION__); 
            return stat;
        }
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Success for UE Asso Sig Msg",
                __FUNCTION__);
        break;

        case ASN1V_x2ap_id_resourceStatusReportingInitiation:
        *p_x2ap_EP = RESOURCE_STATUS_FAILURE;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Success for RESOURCE_STATUS_FAILURE"
                ,__FUNCTION__); 
        break;
        case ASN1V_x2ap_id_cellActivation:
        *p_x2ap_EP = CELL_ACTIVATION_FAILURE;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Success for CELL_ACTIVATION_FAILURE",__FUNCTION__);
        break;
        case ASN1V_x2ap_id_mobilitySettingsChange:
        *p_x2ap_EP = MOBILITY_CHANGE_FAILURE;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                ,__FUNCTION__); 
        break;
#ifdef ENDC_ENABLED
        /*NR_DC Code Change Start*/
        case ASN1V_x2ap_id_endcConfigurationUpdate:
        *p_x2ap_EP = ENDC_CONFIGURATION_UPDATE_FAIL;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                ,__FUNCTION__);
        break;
        case ASN1V_x2ap_id_meNBinitiatedSgNBRelease:
        *p_x2ap_EP = X2AP_UE_ASSO_SIG_MSG_IND_OTHERS;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                ,__FUNCTION__);
        break;
        /*NR_DC Code Change Stop*/
#endif

        default:
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Unwanted Procedure code",
                __FUNCTION__);
        if ((x2ap_reject == criticality) ||
                (x2ap_notify == criticality))
        {
            error_ind.bitmask |= RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT;
            error_ind.cause.t = T_x2ap_Cause_protocol;
            error_ind.cause.u.protocol = 
                x2ap_message_not_compatible_with_receiver_state;

            x2ap_build_and_send_err_indication(
                    p_x2ap_gb_ctx,
                    p_enb_cntxt_arr,
                    &error_ind);
        }
    }
    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;
}

/*****************************************************************************
 * Function Name  : determine_successfulOutcome_proc_code
 * Inputs         : p_x2ap_gb_ctx
 *                  p_enb_cntxt_arr
 *                  pctxt
 *                  p_proc_code
 *                  p_oldEnbUeX2apId
 *                  p_newEnbUeX2apId
 *                  criticality
 * Outputs        : p_x2ap_EP
 * Returns        : X2AP_SUCCESS 
 *                  X2AP_FAILURE 
 * Description    : This function is used to process the ProcedureCode
 *                  SuccessOutcome messages
 *****************************************************************************/
static x2ap_return_et determine_successfulOutcome_proc_code(
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t   *p_enb_cntxt_arr,
        OSCTXT              * pctxt,
        x2ap_EP_et          * p_x2ap_EP,
        x2ap_ProcedureCode  * p_proc_code,
        x2ap_UE_X2AP_ID     * p_oldEnbUeX2apId,
        x2ap_UE_X2AP_ID     * p_newEnbUeX2apId,
#ifdef ENDC_ENABLED
        /*NR_DC Code Change Start*/
        U32                 * p_mEnbUeX2apId,
        U32                 * p_sgnbUeX2apId,
        /*NR_DC Code Change Stop*/
#endif
        x2ap_Criticality      criticality)
{
    x2ap_return_et stat = X2AP_FAILURE; 
    rrc_x2ap_error_indication_t error_ind;

    X2AP_UT_TRACE_ENTER();
    X2AP_MEMSET (&error_ind, X2AP_NULL, sizeof(rrc_x2ap_error_indication_t));

    if (!pctxt || !p_x2ap_EP )
    {
        return X2AP_FAILURE;
    }

    switch (*p_proc_code)
    {
#ifdef ENDC_ENABLED
        /* MENB CHANGES - START */
        case ASN1V_x2ap_id_endcX2Setup:
        *p_x2ap_EP = ENDC_X2SETUP_RESP;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Success for ENDC_X2_SETUP_RESP",__FUNCTION__);
        break;
        /* MENB CHANGES - END */
#endif

        case ASN1V_x2ap_id_x2Setup:
        *p_x2ap_EP = X2_SETUP_RESP;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Success for X2_SETUP_RESP",__FUNCTION__);
        break;

        case ASN1V_x2ap_id_eNBConfigurationUpdate:
        *p_x2ap_EP = ENB_CONFIGURE_UPDATE_RESP;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Success for ENB_CONFIGURE_UPDATE_RESP"
                ,__FUNCTION__);
        break;

        case ASN1V_x2ap_id_handoverPreparation:
        *p_x2ap_EP = X2AP_UE_ASSO_SIG_MSG_IND_HO_REQ;
        stat = decode_EnbUeX2apid (pctxt, p_oldEnbUeX2apId, p_newEnbUeX2apId);
        if(stat != X2AP_SUCCESS )
        {
            X2AP_TRACE(X2AP_INFO, "%s: decode_EnbUeX2apid"
                    "failed for Handver Request Ack"
                    ,__FUNCTION__); 
            return stat;
        }

        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "success for Handver Request Ack"
                ,__FUNCTION__); 
        break;

        case ASN1V_x2ap_id_resourceStatusReportingInitiation:
        *p_x2ap_EP = RESOURCE_STATUS_RESPONSE;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Success for RESOURCE_STATUS_RESPONSE"
                ,__FUNCTION__); 
        break;
        case ASN1V_x2ap_id_reset:
        *p_x2ap_EP = RESET_RESPONSE;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Success for RESET_RESPONSE"
                ,__FUNCTION__);
        break;
        case ASN1V_x2ap_id_mobilitySettingsChange:
        *p_x2ap_EP = MOBILITY_CHANGE_RESPONSE;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Success for MOBILITY_CHANGE_RESPONSE"
                ,__FUNCTION__);
        break;
        case ASN1V_x2ap_id_cellActivation:    
        *p_x2ap_EP = CELL_ACTIVATION_RESPONSE;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Success for CELL_ACTIVATION_RESPONSE",__FUNCTION__);
        break;                    
#ifdef ENDC_ENABLED
        /*NR_DC Code Change Start*/
        case ASN1V_x2ap_id_endcConfigurationUpdate:
        *p_x2ap_EP = ENDC_CONFIGURATION_UPDATE_RESP;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Success for ENDC_CONFIGURATION_UPDATE_RESP"
                ,__FUNCTION__);
        break;
        case ASN1V_x2ap_id_sgNBAdditionPreparation:                  
        *p_x2ap_EP = X2AP_UE_ASSO_SIG_MSG_IND_SGNB_ADD_REQ_ACK;
        stat = decode_dcUeX2apid (pctxt, p_mEnbUeX2apId, p_sgnbUeX2apId);
        if (stat != X2AP_SUCCESS)
        {
            X2AP_TRACE(X2AP_INFO, "%s: decode_SenbUeX2apid failed"
                    "for procedure code %u",
                    __FUNCTION__, *p_proc_code);
            return X2AP_FAILURE;
        }
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode Success for"
                "procedure code %u",__FUNCTION__, *p_proc_code);
        break;                    
        case ASN1V_x2ap_id_meNBinitiatedSgNBRelease:                  
        *p_x2ap_EP = X2AP_UE_ASSO_SIG_MSG_IND_SGNB_REL_REQ_ACK;
        stat = decode_dcUeX2apid (pctxt, p_mEnbUeX2apId, p_sgnbUeX2apId);
        if (stat != X2AP_SUCCESS)
        {
            X2AP_TRACE(X2AP_INFO, "%s: decode_SenbUeX2apid failed"
                    "for procedure code %u",
                    __FUNCTION__, *p_proc_code);
            return X2AP_FAILURE;
        }
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode Success for"
                "procedure code %u",__FUNCTION__, *p_proc_code);
        break;                    
        /*NR_DC Code Change Stop*/
#endif
        default:
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Unwanted Procedure code",
                __FUNCTION__);
        if ((x2ap_reject == criticality) ||
                (x2ap_notify == criticality))
        {
            error_ind.bitmask |= RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT;
            error_ind.cause.t = T_x2ap_Cause_protocol;
            error_ind.cause.u.protocol = 
                x2ap_message_not_compatible_with_receiver_state;

            x2ap_build_and_send_err_indication(
                    p_x2ap_gb_ctx,
                    p_enb_cntxt_arr,
                    &error_ind);
        }
        return X2AP_FAILURE;
    }
    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;
}

/*****************************************************************************
 * Function Name  : determine_initiatingMessage_proc_code
 * Inputs         : p_x2ap_gb_ctx
 *                  p_enb_cntxt_arr
 *                  pctxt
 *                  p_proc_code
 *                  p_oldEnbUeX2apId
 *                  p_newEnbUeX2apId
 *                  criticality
 * Outputs        : p_x2ap_EP
 * Returns        : X2AP_SUCCESS 
 *                  X2AP_FAILURE 
 * Description    : This function is used to process the ProcedureCode
 *                  Initiating messages
 *****************************************************************************/
static x2ap_return_et determine_initiatingMessage_proc_code(
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t   *p_enb_cntxt_arr,
        OSCTXT              * pctxt,
        x2ap_EP_et          * p_x2ap_EP,
        x2ap_ProcedureCode  * p_proc_code,
        x2ap_UE_X2AP_ID     * p_oldEnbUeX2apId,
        x2ap_UE_X2AP_ID     * p_newEnbUeX2apId,
#ifdef ENDC_ENABLED
        /*NR_DC Code Change Start*/
        U32                 * p_mEnbUeX2apId,
        U32                 * p_sgnbUeX2apId,
        /*NR_DC Code Change Stop*/
#endif
        x2ap_Criticality      criticality)
{
    x2ap_return_et stat = X2AP_FAILURE; 
    rrc_x2ap_error_indication_t error_ind;

    X2AP_UT_TRACE_ENTER();
    X2AP_MEMSET (&error_ind, X2AP_NULL, sizeof(rrc_x2ap_error_indication_t));

    if (!pctxt || !p_x2ap_EP )
    {
        return X2AP_FAILURE;
    }

    switch (*p_proc_code)
    {
#ifdef ENDC_ENABLED
        /* MENB CHANGES - START */
        case ASN1V_x2ap_id_sgNBAdditionPreparation:

        X2AP_TRACE(X2AP_INFO, "%s: Unsupported message"
                "recieved",__FUNCTION__);
        return X2AP_FAILURE;
        case ASN1V_x2ap_id_sgNBinitiatedSgNBRelease:
        stat = decode_dcUeX2apid (pctxt, p_mEnbUeX2apId, p_sgnbUeX2apId);
        if (stat != X2AP_SUCCESS)
        {
            X2AP_TRACE(X2AP_INFO, "%s: decode_SenbUeX2apid failed"
                    "for procedure code %u",
                    __FUNCTION__, *p_proc_code);
            return X2AP_FAILURE;
        }
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode Success for"
                "procedure code %u",__FUNCTION__, *p_proc_code);
        *p_x2ap_EP = X2AP_UE_ASSO_SIG_MSG_IND_SGNB_REL_REQ;
        break;

        case ASN1V_x2ap_id_endcX2Setup:
        *p_x2ap_EP = ENDC_X2SETUP_REQ;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Success for ENDC_X2SETUP_REQ",__FUNCTION__);
        break;
        
        case ASN1V_x2ap_id_endcConfigurationUpdate:
        *p_x2ap_EP = ENDC_CONFIGURATION_UPDATE_REQ;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Success for ENDC_CONFIGURATION_UPDATE_REQ"
                ,__FUNCTION__);
        break;
        
        case ASN1V_x2ap_id_secondaryRATDataUsageReport:
        stat = decode_dcUeX2apid (pctxt, p_mEnbUeX2apId, p_sgnbUeX2apId);
        if (stat != X2AP_SUCCESS)
        {
            X2AP_TRACE(X2AP_INFO, "%s: decode_MenbUeX2apid/SgnbUeX2apid failed"
                    "for procedure code %u SECONDARY_RAT_DATA_USAGE_REPORT",
                    __FUNCTION__, *p_proc_code);
            return X2AP_FAILURE;
        }
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode Success for "
                "procedure code %u SECONDARY_RAT_DATA_USAGE_REPORT",__FUNCTION__, *p_proc_code);
        *p_x2ap_EP = X2AP_UE_ASSO_SIG_MSG_IND_SECONDARY_RAT_DATA_USAGE_REPORT;
        break;
        /* MENB CHANGES - END */
#endif

        case ASN1V_x2ap_id_x2Setup:
        *p_x2ap_EP = X2SETUP_REQ;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Success for X2SETUP_REQ",__FUNCTION__);
        break;
        case ASN1V_x2ap_id_eNBConfigurationUpdate:
        *p_x2ap_EP = ENB_CONFIGURE_UPDATE_REQ;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Success for ENB_CONFIGURE_UPDATE_REQ"
                ,__FUNCTION__);
        break;
        case ASN1V_x2ap_id_handoverPreparation:
        *p_x2ap_EP = X2AP_UE_ASSO_SIG_MSG_IND_HO_REQ;

        stat = decode_EnbUeX2apid (pctxt, p_oldEnbUeX2apId, p_newEnbUeX2apId);
        if(stat != X2AP_SUCCESS )
        {
            X2AP_TRACE(X2AP_INFO, "%s: decode_EnbUeX2apid"
                    "failed for Handver Request"
                    ,__FUNCTION__); 
            return stat;
        }

        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Success for Handver Request"
                ,__FUNCTION__); 
        break;
        case ASN1V_x2ap_id_handoverCancel:
        case ASN1V_x2ap_id_snStatusTransfer:
        case ASN1V_x2ap_id_uEContextRelease:
        *p_x2ap_EP = X2AP_UE_ASSO_SIG_MSG_IND_OTHERS;
        stat = decode_EnbUeX2apid (pctxt, p_oldEnbUeX2apId, p_newEnbUeX2apId);
        if(stat != X2AP_SUCCESS )
        {
            X2AP_TRACE(X2AP_INFO, "%s: decode_EnbUeX2apid"
                    "failed for handover Cancel or sn_status_transfer of ue_context_release"
                    ,__FUNCTION__); 
            return stat;
        }
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Success for UE Associated Signalling " 
                "Message",__FUNCTION__);
        break;
        case ASN1V_x2ap_id_resourceStatusReportingInitiation:
        *p_x2ap_EP = RESOURCE_STATUS_REQUEST;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Success for RESOURCE_STATUS_REQUEST",__FUNCTION__);
        break;
        case ASN1V_x2ap_id_loadIndication:
        *p_x2ap_EP = LOAD_INFORMATION;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Success for LOAD_INFORMATION",__FUNCTION__);
        break;
        case ASN1V_x2ap_id_resourceStatusReporting:
        *p_x2ap_EP = RESOURCE_STATUS_UPDATE;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Success for RESOURCE_STATUS_UPDATE",__FUNCTION__);
        break;
        case ASN1V_x2ap_id_reset:
        *p_x2ap_EP = RESET_REQUEST;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Success for RESET_REQUEST",__FUNCTION__);
        break;
        case ASN1V_x2ap_id_errorIndication:
        *p_x2ap_EP = ERROR_INDICATION;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Success for ERROR_INDICATION",__FUNCTION__);
        break;

        case ASN1V_x2ap_id_rLFIndication:
        *p_x2ap_EP = RADIO_LINK_FAILURE_INDICATION;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Success for RADIO_LINK_FAILURE_INDICATION",__FUNCTION__);
        break;

        case ASN1V_x2ap_id_cellActivation:
        *p_x2ap_EP = CELL_ACTIVATION_REQUEST;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Success for CELL_ACTIVATION_REQUEST",__FUNCTION__);
        break;

        case ASN1V_x2ap_id_handoverReport:
        *p_x2ap_EP = HANDOVER_REPORT;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                " Success for HANDOVER_REPORT",__FUNCTION__);
        break;

        case ASN1V_x2ap_id_mobilitySettingsChange:                  
        *p_x2ap_EP = MOBILITY_CHANGE_REQUEST;
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Success for MOBILITY_CHANGE_REQUEST",__FUNCTION__);
        break;
        default:
        X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                "Unwanted Procedure code",
                __FUNCTION__);
        if ((x2ap_reject == criticality) ||
                (x2ap_notify == criticality))
        {
            error_ind.bitmask |= RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT;
            error_ind.cause.t = T_x2ap_Cause_protocol;
            error_ind.cause.u.protocol = 
                x2ap_message_not_compatible_with_receiver_state;

            x2ap_build_and_send_err_indication(
                    p_x2ap_gb_ctx,
                    p_enb_cntxt_arr,
                    &error_ind);
        }
        return X2AP_FAILURE;
    }
    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;
}

/*****************************************************************************
 * Function Name  : partially_decode_X2AP_PDU
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context 
 *                  p_enb_cntxt_arr - pointer to x2ap_peer_enb_context_t
 *                  pctxt - Pointer to OSCTXT.
 *                  p_x2ap_EP - pointer to x2ap_EP_et
 *                  p_proc_code - pointer to x2ap_ProcedureCode
 *                  p_oldEnbUeX2apId - pointer to x2ap_UE_X2AP_ID
 *                  p_newEnbUeX2apId - pointer to x2ap_UE_X2AP_ID
 * Outputs        : p_x2ap_EP - The X2AP EP received.
 * Returns        : X2AP_SUCCESS - Asn Encoding was successful.
 *                  X2AP_FAILURE - Asn Encoding was not successful.
 * Description    : This function when called partially decodes an Asn message
 *                  and returns X2AP EP type. This function decodes the 
 *                  procedureCode and fill p_x2ap_EP by calling 
 *                  determine_x2ap_EP_type. 
 * Error          : This function report error to the caller function, If decoder
 *                  fails to decode message.
 *****************************************************************************/
static x2ap_return_et partially_decode_X2AP_PDU (
        x2ap_gb_context_t   *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t   *p_enb_cntxt_arr,
        OSCTXT              * pctxt,
        x2ap_EP_et          * p_x2ap_EP,
        x2ap_ProcedureCode  * p_proc_code,
        x2ap_UE_X2AP_ID     * p_oldEnbUeX2apId,
        x2ap_UE_X2AP_ID     * p_newEnbUeX2apId
#ifdef ENDC_ENABLED
        /*NR_DC Code Change Start*/
        ,U32                * p_mEnbUeX2apId,
        U32                 * p_sgnbUeX2apId
        /*NR_DC Code Change Stop*/
#endif
        )
{
    S32 stat = 0;
    OSBOOL extbit = FALSE;
    OSUINT32 message_type = 0;
    x2ap_Criticality criticality;
    rrc_x2ap_error_indication_t error_ind;

    X2AP_UT_TRACE_ENTER();
    X2AP_MEMSET (&error_ind, X2AP_NULL, sizeof(rrc_x2ap_error_indication_t));
    X2AP_MEMSET(&criticality, X2AP_NULL, sizeof(x2ap_Criticality));

    if (!pctxt || !p_x2ap_EP )
    {
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    do 
    {
        /* Partially Decode ASN Message */

        /* extension bit */
        stat = PD_BIT (pctxt, &extbit);
        if (stat != 0) return X2AP_FAILURE;

        if (extbit)
        {
            return X2AP_FAILURE;
        }
        /* Decode Message Type */
        stat = pd_ConsUnsigned (pctxt, &message_type, 0, OSUINTCONST(2));
        if (stat != 0) return X2AP_FAILURE;
        else message_type++;

        /* Decode Procedure Code */
        stat = asn1PD_x2ap_ProcedureCode (pctxt, p_proc_code);
        if (stat != 0) return X2AP_FAILURE;

        /* Decode Procedure Code */
        stat = asn1PD_x2ap_Criticality (pctxt, &criticality);
        if (stat != 0) return X2AP_FAILURE;

        /* decode criticality */
        stat = asn1PD_x2ap_Criticality (pctxt, &criticality);
        if (stat != 0) return X2AP_FAILURE;


        /* Determine EP based on Message Type and Procedure Code */    
        switch (message_type)
        {
            case T_x2ap_X2AP_PDU_initiatingMessage:
            /* initiatingMessage */
            X2AP_TRACE(X2AP_DETAILED, "%s: Partial Decode"
                    "Success for initiating Message Request",
                    __FUNCTION__);
            if (X2AP_SUCCESS != determine_initiatingMessage_proc_code(
                        p_x2ap_gb_ctx, p_enb_cntxt_arr, pctxt, p_x2ap_EP, 
                        p_proc_code, p_oldEnbUeX2apId,p_newEnbUeX2apId,
#ifdef ENDC_ENABLED
                        /*NR_DC Code Change Start*/
                        p_mEnbUeX2apId, p_sgnbUeX2apId, 
                        /*NR_DC Code Change Stop*/
#endif
                        criticality))
            {
                return X2AP_FAILURE;
            }
            break;
            /* Successful outcome */
            case T_x2ap_X2AP_PDU_successfulOutcome:
            X2AP_TRACE(X2AP_DETAILED, "%s: Partial Decode"
                    "Success for Successful Out come",
                    __FUNCTION__);
            if (X2AP_SUCCESS != determine_successfulOutcome_proc_code(
                        p_x2ap_gb_ctx, p_enb_cntxt_arr, pctxt, p_x2ap_EP, 
                        p_proc_code, p_oldEnbUeX2apId,p_newEnbUeX2apId, 
#ifdef ENDC_ENABLED
                        /*NR_DC Code Change Start*/
                        p_mEnbUeX2apId, p_sgnbUeX2apId, 
                        /*NR_DC Code Change Stop*/
#endif
                        criticality))
            {
                return X2AP_FAILURE;
            }
            break;
            /* unsuccessfulOutcome */
            case T_x2ap_X2AP_PDU_unsuccessfulOutcome:

            X2AP_TRACE(X2AP_DETAILED, "%s: Partial Decode"
                    "Success for unsuccessful Out come",
                    __FUNCTION__);
            if (X2AP_SUCCESS != determine_unsuccessfulOutcome_proc_code(
                        p_x2ap_gb_ctx, p_enb_cntxt_arr, pctxt, p_x2ap_EP, 
                        p_proc_code, p_oldEnbUeX2apId,p_newEnbUeX2apId, 
#ifdef ENDC_ENABLED
                        /*NR_DC Code Change Start*/
                        p_mEnbUeX2apId, p_sgnbUeX2apId, 
                        /*NR_DC Code Change Stop*/
#endif
                        criticality))
            {
                return X2AP_FAILURE;
            }
            break;
            default:
            X2AP_TRACE(X2AP_INFO, "%s: Partial Decode"
                    "Unwanted message Type received",
                    __FUNCTION__);

            if ((x2ap_reject == criticality) ||
                    (x2ap_notify == criticality))
            {
                error_ind.bitmask |= RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT;
                error_ind.cause.t = T_x2ap_Cause_protocol;

                error_ind.cause.u.protocol = 
                    x2ap_message_not_compatible_with_receiver_state;

                x2ap_build_and_send_err_indication(
                        p_x2ap_gb_ctx,
                        p_enb_cntxt_arr,
                        &error_ind);
            }
            return X2AP_FAILURE;
        }

    }while(0);

    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;
}


/*****************************************************************************
 * Function Name  : x2ap_decode_eNB_config_update_resp_fail 
 * Inputs         : p_enb_conf_resp_fail - pointer to x2ap_enb_conf_upd_fail_t
 *                  msg_result - pointer to msg result
 *                  p_asn_msg - pointer to ASN message
 *                  p_asn_msg_len - pointer to ASN message length 
 * Outputs        : p_asn_msg,p_asn_msg_len
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode eNB Configuration Update Failure
 *                   Response.
 * Error          : This function report error to the caller function, If decoder
 *                  fails to decode message.
 ********************************************************************************/
x2ap_return_et x2ap_decode_eNB_config_update_resp_fail(
        x2ap_enb_conf_upd_fail_t   *p_enb_conf_resp_fail,
        U32                         *msg_result,
        U8                          *p_asn_msg,
        U32                         *p_asn_msg_len, /*SPR_19067_FIX_START*/
        x2ap_gb_context_t	    *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t	    *p_enb_ctx,
        rrc_x2ap_error_indication_t *p_error_indication,
        x2ap_error_ind_bool_t       *p_send_err_ind,
        OSCTXT                	    *p_asn1_ctx) /*SPR_19067_FIX_STOP*/
{
    x2ap_return_et       retVal = X2AP_FAILURE;

    U32                  count = 0;
    x2ap_X2AP_PDU        x2ap_pdu;
    /*SPR_19067_FIX_START*/
    /* Code Removed */
    /*SPR_19067_FIX_STOP*/
    x2ap_ENBConfigurationUpdateFailure  *p_ENBConfigurationUpdateFailure;
    OSRTDListNode        *p_node = X2AP_P_NULL;
    x2ap_ENBConfigurationUpdateFailure_protocolIEs_element
        *p_ENBConfigUpdRespFail_protocol_IEs = X2AP_P_NULL;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    U32                         encoded_msg_len = 0;
    /*SPR_19067_FIX_START*/
    x2ap_error_ind_ie_list_t   iE_list;
    /* coverity 108694 fix start */
    iE_list.ie_cnt = RRC_ZERO;
    /* coverity 108694 fix end */
    U16 ie_list_index = 0;

    x2ap_message_data_t message_map =
    {3, X2AP_TRUE,X2AP_TRUE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_Cause, x2ap_mandatory, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {1, ASN1V_x2ap_id_TimeToWait, x2ap_optional, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {2, ASN1V_x2ap_id_CriticalityDiagnostics, x2ap_optional, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL}
        }};
    /*SPR_19067_FIX_STOP*/

    if (!p_enb_conf_resp_fail)
    {
        X2AP_TRACE(X2AP_ERROR,  "%s: ASN "\
                "p_enb_conf_ack is NULL",__FUNCTION__);
        return X2AP_FAILURE;
    }

    /*SPR_19067_FIX_START*/
    /* Init ASN1 context */
    if ( 0 != rtInitContext(p_asn1_ctx) )
    {
        /* Drop message */
        X2AP_TRACE(X2AP_ERROR,  "%s: ASN "\
                "context initialization failed.",__FUNCTION__);
        return X2AP_FAILURE;
    }

    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(p_asn1_ctx, p_asn_msg, *p_asn_msg_len, TRUE);

    do
    {
        if (0 != asn1PD_x2ap_X2AP_PDU(p_asn1_ctx, &x2ap_pdu) )
        {
            /* Drop message */
            X2AP_TRACE(X2AP_ERROR,  "%s: ASN "\
                    "decode failed.",__FUNCTION__);
            retVal = X2AP_FAILURE;
            break;
        }
        else
        {
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
            encoded_msg_len = (U32)pe_GetMsgLen(p_asn1_ctx); 
            /*SPR_19067_FIX_STOP*/

            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    encoded_msg_len);
            if (PNULL != p_event_header)
            {
                x2ap_fill_protocol_event_params(p_event_header, encoded_msg_len,
                        LOCAL_X2_ENB_CONFIGURATION_UPDATE_FAILURE, 
                        MESSAGE_DIRECTION_RECEIVED);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        encoded_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

        /* check whether the message is not successful */
        if ((T_x2ap_X2AP_PDU_unsuccessfulOutcome == x2ap_pdu.t) ||
                (ASN1V_x2ap_id_eNBConfigurationUpdate ==
                 x2ap_pdu.u.unsuccessfulOutcome->procedureCode) ||
                ((X2AP_P_NULL != x2ap_pdu.u.unsuccessfulOutcome->value.u.
                  eNBConfigurationUpdate) &&
                 (T1x2ap__eNBConfigurationUpdate == 
                  x2ap_pdu.u.unsuccessfulOutcome->value.t)))
        {
            *msg_result = T_x2ap_X2AP_PDU_unsuccessfulOutcome;
            p_ENBConfigurationUpdateFailure = 
                x2ap_pdu.u.unsuccessfulOutcome->value.u.eNBConfigurationUpdate;
            p_node = p_ENBConfigurationUpdateFailure->protocolIEs.head;

            for (count =0; count < p_ENBConfigurationUpdateFailure->
                    protocolIEs.count; count++)
            {
                if (!p_node)
                {
                    X2AP_TRACE(X2AP_ERROR, "%s: ""next node returned NULL",
                            __FUNCTION__);
                    retVal = X2AP_FAILURE;
                    break;
                }
                p_ENBConfigUpdRespFail_protocol_IEs =
                    (x2ap_ENBConfigurationUpdateFailure_protocolIEs_element*)
                    p_node->data;
                switch (p_ENBConfigUpdRespFail_protocol_IEs->id)
                {
                    case ASN1V_x2ap_id_Cause:
                    X2AP_TRACE(X2AP_INFO, "%s: "\
                            "ASN1V_x2ap_id_Cause",__FUNCTION__);
                    retVal = x2ap_parse_cause(
                            &(p_enb_conf_resp_fail->cause),
                            p_ENBConfigUpdRespFail_protocol_IEs->value.u.
                            _x2ap_ENBConfigurationUpdateFailure_IEs_1,
                            PNULL,
                            X2AP_NULL,
                            X2AP_NULL);
                    break;

                    case ASN1V_x2ap_id_TimeToWait:
                    X2AP_TRACE(X2AP_INFO, "%s: "\
                            "ASN1V_x2ap_id_TimeToWait",__FUNCTION__);
                    p_enb_conf_resp_fail->time_to_wait =
                        (x2ap_time_to_wait_et)p_ENBConfigUpdRespFail_protocol_IEs->value.u.
                        _x2ap_ENBConfigurationUpdateFailure_IEs_2;
                    p_enb_conf_resp_fail->bitmask = X2AP_TIME_TO_WAIT_INDEX_PRESENT;
                    retVal = X2AP_SUCCESS;
                    break;

                    case ASN1V_x2ap_id_CriticalityDiagnostics:
                    X2AP_TRACE(X2AP_INFO, "%s: "\
                            "ASN1V_x2ap_id_CriticalityDiagnostics",__FUNCTION__);
                    retVal = x2ap_parse_CriticalityDiagnostics(
                            &(p_enb_conf_resp_fail->criticality_diagnostics),
                            p_ENBConfigUpdRespFail_protocol_IEs->value.u.
                            _x2ap_ENBConfigurationUpdateFailure_IEs_3);
                    p_enb_conf_resp_fail->bitmask = X2AP_CRITICALITY_DIAGNOSTICS_PRESENT;
                    break;

                    default:
                    X2AP_TRACE(X2AP_ERROR, "%s:"\
                            "invalid Protocol IE id",__FUNCTION__);
                    /*SPR_19067_FIX_START*/
                    x2ap_add_to_err_ind_ie_list(&iE_list,
                            p_ENBConfigUpdRespFail_protocol_IEs->criticality,
                            p_ENBConfigUpdRespFail_protocol_IEs->id,
                            &ie_list_index,
                            p_send_err_ind,
                            X2AP_FALSE,
                            X2AP_FALSE);

                    if (x2ap_reject == p_ENBConfigUpdRespFail_protocol_IEs->criticality) 
                    {
                        retVal = X2AP_FAILURE;
                    }
                    if (X2AP_FAILURE == retVal)
                    {
                        X2AP_TRACE(X2AP_ERROR,  "%s: ASN "\
                                "decode failed while decoding parameter id [%d].",
                                __FUNCTION__,p_ENBConfigUpdRespFail_protocol_IEs->id);
                        break;
                    }
                    /*SPR_19067_FIX_STOP*/ 
                    /* SPR 21578 +- */
                }
                /* SPR 21578 Start */
                p_node = p_node->next;
                /* SPR 21578 End */
            }
        }
        else
        {
            /* Drop message */
            X2AP_TRACE(X2AP_ERROR,  "%s: ASN "\
                    "decode failed Unknown Message type received.",
                    __FUNCTION__);
            retVal = X2AP_FAILURE;
            break;
        }


    }while(0);

    /*SPR_19067_FIX_START*/
    /* Parse the map for Error Indication */
    if ((X2AP_SUCCESS == x2ap_parse_message_map(
                    p_asn1_ctx,
                    p_x2ap_gb_ctx,
                    p_enb_ctx, 
                    &message_map,
                    &iE_list,
                    &ie_list_index,
                    p_send_err_ind,
                    /* SPR 23762/SES-459 Fix Start */
                    ASN1V_x2ap_id_eNBConfigurationUpdate,
                    T_x2ap_X2AP_PDU_unsuccessfulOutcome,
                    /* SPR 23762/SES-459 Fix End */
                    x2ap_reject,
                    p_error_indication)))
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED ");

        retVal = X2AP_FAILURE;
    } 
    /*SPR_19067_FIX_STOP*/

    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_decode_eNB_config_update_ack 
 * Inputs         : p_enb_conf_ack - poiner to x2ap_enb_conf_ack_t
 *                  msg_result - msg result
 *                  p_asn_msg - pointer to ASN buffer
 *                  p_asn_msg_len - pointer to ASN buffer length  
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode eNB Configuration Update ACK.
 * Error          : This function report error to the caller function, If decoder
 *                  fails to decode message.
 ********************************************************************************/
x2ap_return_et x2ap_decode_eNB_config_update_ack (
        x2ap_enb_conf_ack_t   *p_enb_conf_ack,
        U32                    *msg_result,
        U8                     *p_asn_msg,
        /*SPR_19067_FIX_START*/
        U32                    	*p_asn_msg_len,
        rrc_x2ap_error_indication_t  *p_error_indication,
        x2ap_error_ind_bool_t        *p_send_err_ind,
        OSCTXT                 	*p_asn1_ctx,
        x2ap_gb_context_t		*p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t	*p_enb_ctx)
/*SPR_19067_FIX_STOP*/
{
    x2ap_return_et       retVal = X2AP_SUCCESS;

    x2ap_X2AP_PDU        x2ap_pdu;
    /*SPR_19067_FIX_START*/
    /* Code Removed */
    /*SPR_19067_FIX_STOP*/
    OSRTDListNode        *p_node = X2AP_P_NULL;
    x2ap_ENBConfigurationUpdateAcknowledge  *p_ENBConfigurationUpdateAcknowledge;
    x2ap_ENBConfigurationUpdateAcknowledge_protocolIEs_element
        *p_ENBConfigUpdAck_protocol_IEs = X2AP_P_NULL;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    U32                         encoded_msg_len = 0;
    U32                         count = 0;

    /*SPR_19067_FIX_START*/
    x2ap_error_ind_ie_list_t   iE_list;
    /* coverity 108696 fix start */
    iE_list.ie_cnt = RRC_ZERO;
    /* coverity 108696 fix end */
    U16 ie_list_index = 0;

    x2ap_message_data_t message_map =
    {1, X2AP_TRUE,X2AP_TRUE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_CriticalityDiagnostics, x2ap_optional, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL}
        }};

    if (!p_enb_conf_ack)
    {
        X2AP_TRACE(X2AP_ERROR,  "%s: ASN "\
                "p_enb_conf_ack is NULL",__FUNCTION__);
        return X2AP_FAILURE;
    }

    /* Init ASN1 context */
    if ( 0 != rtInitContext(p_asn1_ctx) )
    {
        /* Drop message */
        X2AP_TRACE(X2AP_ERROR,  "%s: ASN "\
                "context initialization failed.",__FUNCTION__);
        return X2AP_FAILURE;
    }

    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(p_asn1_ctx, p_asn_msg, *p_asn_msg_len, TRUE);

    do
    {
        if (0 != asn1PD_x2ap_X2AP_PDU(p_asn1_ctx, &x2ap_pdu) )
        {
            /* Drop message */
            X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                    "decode failed.",__FUNCTION__);
            retVal = X2AP_FAILURE;
            break;
        }
        else
        {
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);

            encoded_msg_len = (U32)pe_GetMsgLen(p_asn1_ctx);

            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    encoded_msg_len);
            if (PNULL != p_event_header)
            {
                x2ap_fill_protocol_event_params(p_event_header, encoded_msg_len,
                        LOCAL_X2_ENB_CONFIGURATION_UPDATE_ACKNOWLEDGE, 
                        MESSAGE_DIRECTION_RECEIVED);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        encoded_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

        if ((T_x2ap_X2AP_PDU_successfulOutcome == x2ap_pdu.t) ||
                (ASN1V_x2ap_id_eNBConfigurationUpdate ==
                 x2ap_pdu.u.successfulOutcome->procedureCode) ||
                ((X2AP_P_NULL != x2ap_pdu.u.successfulOutcome->value.u.
                  eNBConfigurationUpdate) &&
                 (T1x2ap__eNBConfigurationUpdate == x2ap_pdu.u.successfulOutcome->value.t)))
        {
            *msg_result = T_x2ap_X2AP_PDU_successfulOutcome;
            p_ENBConfigurationUpdateAcknowledge = 
                x2ap_pdu.u.successfulOutcome->value.u.
                eNBConfigurationUpdate;

            p_node = p_ENBConfigurationUpdateAcknowledge->protocolIEs.head;

            for (count =0;
                    count < p_ENBConfigurationUpdateAcknowledge->protocolIEs.count; count++)
            {
                if (!p_node)
                {
                    X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                            __FUNCTION__);
                    retVal = X2AP_FAILURE;
                    break;
                }

                p_ENBConfigUpdAck_protocol_IEs =
                    (x2ap_ENBConfigurationUpdateAcknowledge_protocolIEs_element *)p_node->data;

                switch (p_ENBConfigUpdAck_protocol_IEs->id)
                {

                    case ASN1V_x2ap_id_CriticalityDiagnostics:
                    {
                        p_enb_conf_ack->bitmask = X2AP_CRITICALITY_DIAGNOS_PRESENT;

                        X2AP_TRACE(X2AP_INFO,"%s: "
                                "ASN1V_x2ap_id_CriticalityDiagnostics",__FUNCTION__);
                        if(  X2AP_P_NULL != p_ENBConfigUpdAck_protocol_IEs->
                                value.u._x2ap_ENBConfigurationUpdateAcknowledge_IEs_1 )
                        {
                            retVal = x2ap_parse_CriticalityDiagnostics(
                                    &(p_enb_conf_ack->criticality_diagnostics),
                                    p_ENBConfigUpdAck_protocol_IEs->value.u.
                                    _x2ap_ENBConfigurationUpdateAcknowledge_IEs_1 );
                        }

                    }break;

                    default: 
                    {
                        X2AP_TRACE(X2AP_ERROR, "%s: invalid Protocol IE id",__FUNCTION__);
                        x2ap_add_to_err_ind_ie_list(&iE_list,
                                p_ENBConfigUpdAck_protocol_IEs->criticality,
                                p_ENBConfigUpdAck_protocol_IEs->id,
                                &ie_list_index,
                                p_send_err_ind,
                                X2AP_FALSE,
                                X2AP_FALSE);
                        if (x2ap_reject == p_ENBConfigUpdAck_protocol_IEs->criticality) 
                        {
                            retVal = X2AP_FAILURE;
                        }
                    }
                    break;
                }

                p_node = p_node->next;
            }
        }
        else
        {
            X2AP_TRACE(X2AP_ERROR,  "%s: ASN "\
                    "decode failed Unknown Message type received.",
                    __FUNCTION__);
            retVal = X2AP_FAILURE;
            break; 
        }
    }while(0);

    /* Parse the map for Error Indication */
    if ((X2AP_SUCCESS == x2ap_parse_message_map(
                    p_asn1_ctx,
                    p_x2ap_gb_ctx,
                    p_enb_ctx, 
                    &message_map,
                    &iE_list,
                    &ie_list_index,
                    p_send_err_ind,
                    /* SPR 23762/SES-459 Fix Start */
                    ASN1V_x2ap_id_eNBConfigurationUpdate,
                    T_x2ap_X2AP_PDU_successfulOutcome,
                    /* SPR 23762/SES-459 Fix End */
                    x2ap_reject,
                    p_error_indication)))
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED ");

        retVal = X2AP_FAILURE;
    }
    /*SPR_19067_FIX_STOP*/

    return retVal;
}


/*****************************************************************************
 * Function Name  : x2ap_parse_ServedCellsToDelete 
 * Inputs         : p_num_srv_cell_to_del - pointer to num of cells to delete
 *                  p_old_cell_id - old cell id
 *                  p_Old_ECGIs - pointer to x2ap_Old_ECGIs
 *                  p_enb_cntx - pointer to x2ap_peer_enb_context_t
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode Served Cells to delete Information.
 * Error          : This function report error to the caller function, If decoder
 *                  fails to decode message.
 ********************************************************************************/
x2ap_return_et  x2ap_parse_ServedCellsToDelete(
        U16                            *p_num_srv_cell_to_del,
        U8                   *p_old_cell_id,
        x2ap_Old_ECGIs                 *p_Old_ECGIs,
        x2ap_peer_enb_context_t* p_enb_cntx)
{
    x2ap_return_et                  retVal = X2AP_SUCCESS;
    OSRTDListNode                   *p_node = X2AP_P_NULL;
    U8                              old_ecgi_count = 0;

    p_node = p_Old_ECGIs->head;
    *p_num_srv_cell_to_del = (U16)(p_Old_ECGIs->count);

    /* SPR 13041 Fix Start */
    if(256 < *p_num_srv_cell_to_del)
    {
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE; 
    }

    if (MAX_SERVED_CELLS < *p_num_srv_cell_to_del)
    {
        X2AP_TRACE(X2AP_WARNING,"%s: ""Served Cells greater than 8 are not supported ",
                __FUNCTION__);
    }

    for (old_ecgi_count = 0; (old_ecgi_count < p_Old_ECGIs->count) &&
            (old_ecgi_count < MAX_SERVED_CELLS) ; old_ecgi_count++)/* SPR 13041 Fix Stop */
    {
        x2ap_compose_cell_id_from_ecgi(p_enb_cntx->gb_enb_id, 
                ((x2ap_ECGI *)p_node->data)->eUTRANcellIdentifier.data, 
                &p_old_cell_id[old_ecgi_count]);
        p_node = p_node->next;
    }

    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_parse_ServedCellsToModify 
 * Inputs         : p_num_srv_cell_to_mod - pointer to the served cell to midify 
 *                  p_served_cells_to_mod_info - pointer to 
 *                                          x2ap_served_cells_to_modify_list_t
 *                  p_ServedCellsToModify - pointer to x2ap_ServedCellsToModify
 *                  p_enb_cntx - pointer to x2ap_peer_enb_context_t
 * Outputs        : p_served_cells_to_mod_info
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode Served Cells Information. 
 * Error          : This function report error to the caller function, If decoder
 *                  fails to decode message.
 ********************************************************************************/
x2ap_return_et  x2ap_parse_ServedCellsToModify(
        U16                                 *p_num_srv_cell_to_mod,
        x2ap_served_cells_to_modify_list_t  *p_served_cells_to_mod_info,
        x2ap_ServedCellsToModify            *p_ServedCellsToModify,
        x2ap_peer_enb_context_t             *p_enb_cntx)
{
    OSRTDListNode   *p_node = X2AP_P_NULL;
    x2ap_return_et  retVal = X2AP_SUCCESS;
    OSRTDListNode   *p_neigh_cell_info = X2AP_P_NULL;
    U8              count_cell_to_mod = 0;
    U32             num_neigh_cell = 0;
    U8              num_bplmn = 0;

    p_node = p_ServedCellsToModify->head;
    *p_num_srv_cell_to_mod = (U16)p_ServedCellsToModify->count;

    /* SPR 13041 Fix Start */
    if(256 < *p_num_srv_cell_to_mod)
    {
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    if (MAX_SERVED_CELLS < *p_num_srv_cell_to_mod)
    {
        X2AP_TRACE(X2AP_WARNING,"%s: Served Cells greater than 8 are not supported ", __FUNCTION__);
    }

    for (count_cell_to_mod = 0;
            (count_cell_to_mod < p_ServedCellsToModify->count) &&
            (count_cell_to_mod < MAX_SERVED_CELLS);
            count_cell_to_mod++)
        /* SPR 13041 Fix Stop */
    {
        if (!p_node)
        {
            X2AP_TRACE(X2AP_ERROR, "%s: next node returned NULL", __FUNCTION__);
            retVal = X2AP_FAILURE;
            break;
        }

        x2ap_compose_cell_id_from_ecgi(
                p_enb_cntx->gb_enb_id,
                ((x2ap_ServedCellsToModify_Item *)p_node->data)->old_ecgi.eUTRANcellIdentifier.data,
                &(p_served_cells_to_mod_info->cell_list[count_cell_to_mod].old_cell_id));

        /*decode served cell to modify */
        /*PCI*/
        p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.pci =
            ((x2ap_ServedCellsToModify_Item *)p_node->data)->servedCellInfo.pCI;

        /* cellId */
        x2ap_compose_cell_id_from_ecgi(
                p_enb_cntx->gb_enb_id,
                ((x2ap_ServedCellsToModify_Item *)p_node->data)->servedCellInfo.cellId.eUTRANcellIdentifier.data,
                &p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.cell_id);

        /* tac */
        X2AP_MEMCPY(p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.tac,
                ((x2ap_ServedCellsToModify_Item*)p_node->data)->servedCellInfo.tAC.data,
                ((x2ap_ServedCellsToModify_Item*)p_node->data)->servedCellInfo.tAC.numocts);

        /* bcast_plmn_id */
        p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.bcast_plmn_info.num_bcast_plmn_id =
            (U8)(((x2ap_ServedCellsToModify_Item*)p_node->data)->servedCellInfo.broadcastPLMNs.n);

        for (num_bplmn = 0; num_bplmn < ((x2ap_ServedCellsToModify_Item*)p_node->data)->servedCellInfo.broadcastPLMNs.n; num_bplmn++)
        {
            X2AP_MEMCPY(p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.bcast_plmn_info.plmn_identity[num_bplmn].plmn_id,
                    ((x2ap_ServedCellsToModify_Item *)p_node->data)->servedCellInfo.broadcastPLMNs.elem[num_bplmn].data,
                    ((x2ap_ServedCellsToModify_Item *)p_node->data)->servedCellInfo.broadcastPLMNs.elem[num_bplmn].numocts);
        }

        /* choice eutra mode */
        p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.choice_eutra_mode.choice_eutra_mode.bitmask =
            (U8)(((x2ap_ServedCellsToModify_Item*)p_node->data)->servedCellInfo.eUTRA_Mode_Info.t);

        if (T_x2ap_EUTRA_Mode_Info_fDD == ((x2ap_ServedCellsToModify_Item*)p_node->data)->servedCellInfo.eUTRA_Mode_Info.t)
        {
            p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.choice_eutra_mode.choice_eutra_mode.fdd_info.ul_earfcn.earfcn =
                ((x2ap_ServedCellsToModify_Item*)p_node->data)->servedCellInfo.eUTRA_Mode_Info.u.fDD->uL_EARFCN;

            p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.choice_eutra_mode.choice_eutra_mode.fdd_info.dl_earfcn.earfcn =
                ((x2ap_ServedCellsToModify_Item*)p_node->data)->servedCellInfo.eUTRA_Mode_Info.u.fDD->dL_EARFCN;

            p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.choice_eutra_mode.choice_eutra_mode.fdd_info.ul_trnsmission_bdwidth =
                (U8)(((x2ap_ServedCellsToModify_Item*)p_node->data)->servedCellInfo.eUTRA_Mode_Info.u.fDD->uL_Transmission_Bandwidth);

            p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.choice_eutra_mode.choice_eutra_mode.fdd_info.dl_trnsmission_bdwidth =
                (U8)(((x2ap_ServedCellsToModify_Item*)p_node->data)->servedCellInfo.eUTRA_Mode_Info.u.fDD->dL_Transmission_Bandwidth);
        }
        else if (T_x2ap_EUTRA_Mode_Info_tDD == ((x2ap_ServedCellsToModify_Item*)p_node->data)->servedCellInfo.eUTRA_Mode_Info.t)
        {
            p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.choice_eutra_mode.choice_eutra_mode.tdd_info.earfcn.earfcn =
                ((x2ap_ServedCellsToModify_Item*)p_node->data)->servedCellInfo.eUTRA_Mode_Info.u.tDD->eARFCN;

            p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.choice_eutra_mode.choice_eutra_mode.tdd_info.trnsmission_bdwidth =
                (U8)(((x2ap_ServedCellsToModify_Item*)p_node->data)->servedCellInfo.eUTRA_Mode_Info.u.tDD->transmission_Bandwidth);

            p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.choice_eutra_mode.choice_eutra_mode.tdd_info.subframe_assn =
                (U8)(((x2ap_ServedCellsToModify_Item*)p_node->data)->servedCellInfo.eUTRA_Mode_Info.u.tDD->subframeAssignment);

            p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.choice_eutra_mode.choice_eutra_mode.tdd_info.special_subframe_info.special_subframe_patterns =
                (U8)(((x2ap_ServedCellsToModify_Item*)p_node->data)->servedCellInfo.eUTRA_Mode_Info.u.tDD->specialSubframe_Info.specialSubframePatterns);

            p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.choice_eutra_mode.choice_eutra_mode.tdd_info.special_subframe_info.dl_cp =
                (U8)(((x2ap_ServedCellsToModify_Item*)p_node->data)->servedCellInfo.eUTRA_Mode_Info.u.tDD->specialSubframe_Info.cyclicPrefixDL);

            p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.choice_eutra_mode.choice_eutra_mode.tdd_info.special_subframe_info.ul_cp =
                (U8)(((x2ap_ServedCellsToModify_Item*)p_node->data)->servedCellInfo.eUTRA_Mode_Info.u.tDD->specialSubframe_Info.cyclicPrefixUL);
        }
        else
        {
            X2AP_TRACE(X2AP_ERROR, "%s: Eutran mode is not present", __FUNCTION__);
            retVal = X2AP_FAILURE;
            break;
        }

        if(((x2ap_ServedCellsToModify_Item *)p_node->data)->m.iE_ExtensionsPresent)
        {
            U8 extensions_count = 0;
            U8 extensions_counter =0;
            OSRTDListNode* p_node_1 = X2AP_P_NULL;

            p_node_1 = ((x2ap_ServedCellsToModify_Item *)p_node->data)->iE_Extensions.head;
            extensions_count = ((x2ap_ServedCellsToModify_Item *)p_node->data)->iE_Extensions.count;

            for(extensions_counter=0;extensions_counter<extensions_count; extensions_counter++)
            {
                if(((x2ap_ServedCellsToModify_Item_iE_Extensions_element*)p_node_1->data)->id == ASN1V_x2ap_id_DeactivationIndication)
                {
                    p_served_cells_to_mod_info->cell_list[count_cell_to_mod].bitmask |= X2AP_DEACTIVATION_IND_PRESENT;
                    p_served_cells_to_mod_info->cell_list[count_cell_to_mod].deactivation_ind =
                        ((x2ap_ServedCellsToModify_Item_iE_Extensions_element*)p_node_1->data)->extensionValue.u._x2ap_ServedCellsToModify_Item_ExtIEs_1;
                }
            }
        }
        if (((x2ap_ServedCellsToModify_Item *)p_node->data)->m.neighbour_InfoPresent)  
        {
            p_neigh_cell_info = ((x2ap_ServedCellsToModify_Item*)p_node->data)->
                neighbour_Info.head;
            p_served_cells_to_mod_info->cell_list[count_cell_to_mod].
                served_cell_info.nbour_cell_info.num_neighbour_cell =
                (U16)(((x2ap_ServedCellsToModify_Item*)p_node->data)->
                        neighbour_Info.count);
            for (num_neigh_cell = 0;
                    num_neigh_cell < ((x2ap_ServedCellsToModify_Item*)p_node->
                        data)->neighbour_Info.count;
                    num_neigh_cell++)
            {
                X2AP_MEMCPY(p_served_cells_to_mod_info->cell_list[count_cell_to_mod].
                        served_cell_info.nbour_cell_info.
                        neighbour_info_arr[num_neigh_cell].ecgi.plmn_identity.plmn_id,
                        ((x2ap_Neighbour_Information_element *)
                         p_neigh_cell_info->data)->eCGI.pLMN_Identity.data,
                        ((x2ap_Neighbour_Information_element *)
                         p_neigh_cell_info->data)->eCGI.pLMN_Identity.numocts);

                X2AP_MEMCPY(&(p_served_cells_to_mod_info->cell_list[count_cell_to_mod].
                            served_cell_info.nbour_cell_info.
                            neighbour_info_arr[num_neigh_cell].ecgi.eutran_cell_id),
                        ((x2ap_Neighbour_Information_element *)
                         p_neigh_cell_info->data)->eCGI.eUTRANcellIdentifier.data,
                        sizeof (U32));

                p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.
                    nbour_cell_info.neighbour_info_arr[num_neigh_cell].pci =
                    ((x2ap_Neighbour_Information_element *)
                     p_neigh_cell_info->data)->pCI;

                p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.
                    nbour_cell_info.neighbour_info_arr[num_neigh_cell].earfcn.earfcn =
                    ((x2ap_Neighbour_Information_element *)
                     p_neigh_cell_info->data)->eARFCN;

                p_neigh_cell_info = p_neigh_cell_info->next;
            }/* end of for neighbour info */
        }/* end of if neighbour present */

        if(((x2ap_ServedCellsToModify_Item *)p_node->data)->servedCellInfo.m.iE_ExtensionsPresent)
        {
            U8 extensions_count = 0;
            U8 extensions_counter =0;
            OSRTDListNode* p_node_1 = X2AP_P_NULL;

            p_node_1 = ((x2ap_ServedCellsToModify_Item *)p_node->data)->servedCellInfo.iE_Extensions.head;
            extensions_count = ((x2ap_ServedCellsToModify_Item *)p_node->data)->servedCellInfo.iE_Extensions.count;

            for(extensions_counter=0;extensions_counter<extensions_count; extensions_counter++)
            {
                if(((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.t==T341x2ap___x2ap_ServedCell_Information_ExtIEs_1)
                {
                    p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.bitmask |= X2AP_NUM_ANTENNA_INFO_PORT_PRESENT;
                    p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.num_antenna_port =
                        ((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_1;
                }
                else if(((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.t==T341x2ap___x2ap_ServedCell_Information_ExtIEs_2)
                {
                    p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.bitmask |= X2AP_PRACH_CONFIG_INFO_PRESENT;
                    p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.prach_config.high_speed_flag =
                        ((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_2->highSpeedFlag;
                    p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.prach_config.prach_frequency_offset =
                        ((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_2->prach_FreqOffset;
                    p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.prach_config.root_sequence_index =
                        ((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_2->rootSequenceIndex;
                    p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.prach_config.zero_correlation_zone_config  =
                        ((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_2->zeroCorrelationIndex;

                    if(((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_2->m.prach_ConfigIndexPresent)
                    {
                        p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.
                            cell_info.prach_config.bitmask |= X2AP_PRACH_CONFIG_INDEX_PRESENT;
                        p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.prach_config.prach_config_index =
                            ((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_2->prach_ConfigIndex;
                    }
                }
#ifdef LTE_EMBMS_SUPPORTED
                /*SPR 18605 FIX START*/
                else if(((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.t==T341x2ap___x2ap_ServedCell_Information_ExtIEs_3)
                {
                    U8 mbsfn_area_count = X2AP_ZERO,index = X2AP_ZERO;
                    x2ap_MBSFN_Subframe_Info* p_mbsfn_subframe_info = X2AP_P_NULL;
                    OSRTDListNode* p_mbsfn_area_node = X2AP_P_NULL;
                    p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.bitmask |= X2AP_MBSFN_SUBFRAME_INFO_PRESENT;
                    p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.mbsfn_subframe_info.num_mbsfn =
                        ((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_3->count;
                    mbsfn_area_count = p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.mbsfn_subframe_info.num_mbsfn;

                    p_mbsfn_area_node = ((x2ap_ServedCell_Information_iE_Extensions_element*)
                            p_node_1->data)->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_3->head;

                    X2AP_TRACE(X2AP_INFO, "Num MBSFN Areas:%u %s",
                            p_served_cells_to_mod_info->cell_list[count_cell_to_mod].
                            served_cell_info.cell_info.mbsfn_subframe_info.num_mbsfn,
                            __FUNCTION__);

                    for(index = X2AP_ZERO;index < mbsfn_area_count ; index++)
                    {
                        if(!p_mbsfn_area_node)
                        {
                            X2AP_TRACE(X2AP_ERROR, "%s: ""next node returned NULL",
                                    __FUNCTION__);
                            retVal = X2AP_FAILURE;
                            break;
                        }
                        p_mbsfn_subframe_info = (x2ap_MBSFN_Subframe_Info*)p_mbsfn_area_node->data;

                        p_served_cells_to_mod_info->cell_list[count_cell_to_mod].
                            served_cell_info.cell_info.mbsfn_subframe_info.mbsfn_info[index].radioframe_alloc_period =
                            p_mbsfn_subframe_info->radioframeAllocationPeriod;

                        X2AP_TRACE(X2AP_INFO, "radioframe_alloc_period:%u %s",
                                p_served_cells_to_mod_info->cell_list[count_cell_to_mod].
                                served_cell_info.cell_info.mbsfn_subframe_info.mbsfn_info[index].radioframe_alloc_period,
                                __FUNCTION__);

                        p_served_cells_to_mod_info->cell_list[count_cell_to_mod].
                            served_cell_info.cell_info.mbsfn_subframe_info.mbsfn_info[index].radioframe_alloc_offset =
                            p_mbsfn_subframe_info->radioframeAllocationOffset;

                        X2AP_TRACE(X2AP_INFO, "radioframeAllocationOffset:%u %s",
                                p_served_cells_to_mod_info->cell_list[count_cell_to_mod].
                                served_cell_info.cell_info.mbsfn_subframe_info.mbsfn_info[index].radioframe_alloc_offset,
                                __FUNCTION__);
                        if(T_MBSFN_SubframeConfig_subframeAllocation_oneFrame == p_mbsfn_subframe_info->subframeAllocation.t)
                        {
                            p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.
                                mbsfn_subframe_info.mbsfn_info[index].subframe_alloc.bitmask |=
                                X2AP_SUBFRAME_ALLOC_ONE_FRAME_INFO_PRESENT;

                            X2AP_MEMCPY(p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.
                                    mbsfn_subframe_info.mbsfn_info[index].subframe_alloc.oneframe,
                                    p_mbsfn_subframe_info->subframeAllocation.u.oneframe->data,
                                    X2AP_SUBFRAME_ALLOC_ONE_FRAME_OCTET_SIZE);
                        }
                        else if(T_MBSFN_SubframeConfig_subframeAllocation_fourFrames == p_mbsfn_subframe_info->subframeAllocation.t)
                        {
                            p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.
                                mbsfn_subframe_info.mbsfn_info[index].subframe_alloc.bitmask |=
                                X2AP_SUBFRAME_ALLOC_FOUR_FRAME_INFO_PRESENT;

                            X2AP_MEMCPY(p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.
                                    mbsfn_subframe_info.mbsfn_info[index].subframe_alloc.fourframe,
                                    p_mbsfn_subframe_info->subframeAllocation.u.fourframes->data,
                                    X2AP_SUBFRAME_ALLOC_FOUR_FRAME_OCTET_SIZE);
                        }
                        p_mbsfn_area_node = p_mbsfn_area_node->next;
                    }
                }
                /*SPR 18605 FIX STOP*/
#endif
                /* SES-460 changes start */
                if(((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.t==T341x2ap___x2ap_ServedCell_Information_ExtIEs_4)
                {
                    p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.bitmask |= X2AP_CSG_ID_INFO_PRESENT;
                    l3_memcpy_wrapper(p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.csg_identity,
                            ((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_4->data,
                            sizeof(p_served_cells_to_mod_info->cell_list[count_cell_to_mod].served_cell_info.cell_info.csg_identity));
                }
                /* SES-460 changes end */

                p_node_1 = p_node_1->next;
            }
        }
        p_node = p_node->next;
    }/* end of for served cells to modify */
    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_decode_eNB_config_update_req 
 * Inputs         : p_x2ap_gb_ctx - poiter to X2AP global context
 *                  p_enb_conf_req - pointer to x2ap_enb_config_update_ind_t
 *                  p_asn_msg - pointer to ASN msg
 *                  p_asn_msg_len - pointer to ASN msg length 
 *                  p_enb_cntx - pointer to x2ap_peer_enb_context_t
 * Outputs        : p_asn_msg,p_asn_msg_len 
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode eNB Configuration Update request.
 * Error          : This function report error to the caller function, If decoder
 *                  fails to decode message.
 ********************************************************************************/
x2ap_return_et x2ap_decode_eNB_config_update_req (
        /*SPR_19067_FIX_START*/
        x2ap_gb_context_t* p_x2ap_gb_ctx,
        /*SPR_19067_FIX_STOP*/
        x2ap_enb_config_update_ind_t   *p_enb_conf_req,
        U8                     *p_asn_msg,
        U32                    *p_asn_msg_len,
        x2ap_peer_enb_context_t* p_enb_cntx, 
        /*SPR_19067_FIX_START*/ 
        rrc_x2ap_error_indication_t  *p_error_indication,
        x2ap_error_ind_bool_t        *p_send_err_ind,
        OSCTXT                	*p_asn1_ctx) 
/*SPR_19067_FIX_STOP*/
{
    x2ap_return_et                 retVal = X2AP_FAILURE;

    U32                            count = 0;
    x2ap_X2AP_PDU                  x2ap_pdu;
    /*SPR_19067_FIX_START*/
    /* Code Removed */
    /*SPR_19067_FIX_STOP*/
    x2ap_ENBConfigurationUpdate    *p_ENBConfigurationUpdate;
    OSRTDListNode                  *p_node = X2AP_P_NULL;
    x2ap_ENBConfigurationUpdate_protocolIEs_element
        *p_ENBConfigurationUpdate_protocol_IEs = X2AP_P_NULL;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    U32                         encoded_msg_len = 0;

    /*SPR_19067_FIX_START*/
    x2ap_error_ind_ie_list_t   iE_list;
    /* coverity 108695 fix start */
    iE_list.ie_cnt = RRC_ZERO;
    /* coverity 108695 fix end */
    U16 ie_list_index = 0;

    x2ap_message_data_t message_map =
    {5, X2AP_TRUE,X2AP_TRUE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_ServedCellsToAdd, x2ap_optional, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {1, ASN1V_x2ap_id_ServedCellsToModify, x2ap_optional, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {2, ASN1V_x2ap_id_ServedCellsToDelete, x2ap_optional, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {3, ASN1V_x2ap_id_GUGroupIDToAddList, x2ap_optional,  x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {4, ASN1V_x2ap_id_GUGroupIDToDeleteList, x2ap_optional, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL}
        }};

    if (!p_enb_conf_req)
    {
        X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                "p_enb_conf_req is NULL",__FUNCTION__);
        return X2AP_FAILURE;
    }

    /* Init ASN1 context */
    if ( 0 != rtInitContext(p_asn1_ctx) )
    {
        /* Drop message */
        X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                "context initialization failed.",__FUNCTION__);
        return X2AP_FAILURE;
    }
    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(p_asn1_ctx, p_asn_msg, *p_asn_msg_len, TRUE);

    do
    {
        if (0 != asn1PD_x2ap_X2AP_PDU(p_asn1_ctx, &x2ap_pdu) )
        {
            /* Drop message */
            X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                    "decode failed.",__FUNCTION__);
            retVal = X2AP_FAILURE;
            break;
        }
        else
        {
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
            encoded_msg_len = (U32)pe_GetMsgLen(p_asn1_ctx);
            /*SPR_19067_FIX_STOP*/

            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    encoded_msg_len);
            if (PNULL != p_event_header)
            {
                x2ap_fill_protocol_event_params(p_event_header, encoded_msg_len,
                        LOCAL_X2_ENB_CONFIGURATION_UPDATE, 
                        MESSAGE_DIRECTION_RECEIVED);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        encoded_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

        /* check whether the message is not successful */
        if ((T_x2ap_X2AP_PDU_initiatingMessage == x2ap_pdu.t) ||
                (ASN1V_x2ap_id_eNBConfigurationUpdate ==
                 x2ap_pdu.u.initiatingMessage->procedureCode) ||
                ((X2AP_P_NULL != x2ap_pdu.u.initiatingMessage->value.u.
                  eNBConfigurationUpdate) &&
                 (T1x2ap__eNBConfigurationUpdate == x2ap_pdu.u.initiatingMessage->
                  value.t)))
        {
            p_ENBConfigurationUpdate = x2ap_pdu.u.initiatingMessage->value.u.
                eNBConfigurationUpdate;
            p_node = p_ENBConfigurationUpdate->protocolIEs.head;

            for (count =0;
                    count < p_ENBConfigurationUpdate->protocolIEs.count; 
                    count++)
            {
                if (!p_node)
                {
                    X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                            __FUNCTION__);
                    retVal = X2AP_FAILURE;
                    break;
                }
                p_ENBConfigurationUpdate_protocol_IEs =
                    (x2ap_ENBConfigurationUpdate_protocolIEs_element *)p_node->data;

                switch (p_ENBConfigurationUpdate_protocol_IEs->id)
                {
                    case ASN1V_x2ap_id_ServedCellsToAdd:
                    X2AP_TRACE(X2AP_INFO,"%s: "\
                            "ASN1V_x2ap_id_ServedCellsToAdd\n",__FUNCTION__);
                    p_enb_conf_req->bitmask |= X2AP_SERVED_CELLS_TO_ADD_LIST_PRESENT;

                    retVal = x2ap_parse_peer_served_cells_info (
                            &(p_enb_conf_req->add_served_cell.num_served_cell),
                            p_enb_conf_req->add_served_cell.cell_list,
                            p_ENBConfigurationUpdate_protocol_IEs->value.u.
                            _x2ap_ENBConfigurationUpdate_IEs_1,
                            p_enb_cntx);
                    break;

                    case ASN1V_x2ap_id_ServedCellsToModify:
                    X2AP_TRACE(X2AP_INFO,"%s: "\
                            "ASN1V_x2ap_id_ServedCellsToModify\n",__FUNCTION__);
                    p_enb_conf_req->bitmask |= X2AP_SERVED_CELLS_TO_MODIFY_LIST_PRESENT;

                    retVal = x2ap_parse_ServedCellsToModify (
                            &(p_enb_conf_req->mod_served_cell.num_served_cell),
                            &p_enb_conf_req->mod_served_cell,
                            p_ENBConfigurationUpdate_protocol_IEs->value.u.
                            _x2ap_ENBConfigurationUpdate_IEs_2,
                            p_enb_cntx);
                    break;

                    case ASN1V_x2ap_id_ServedCellsToDelete:
                    X2AP_TRACE(X2AP_INFO,"%s: "\
                            "ASN1V_x2ap_id_ServedCellsToDelete\n",__FUNCTION__);
                    p_enb_conf_req->bitmask |= X2AP_SERVED_CELLS_TO_DELETE_LIST_PRESENT;

                    retVal = x2ap_parse_ServedCellsToDelete (
                            &(p_enb_conf_req->del_served_cell.num_served_cell),
                            p_enb_conf_req->del_served_cell.cell_id_list,
                            p_ENBConfigurationUpdate_protocol_IEs->value.u.
                            _x2ap_ENBConfigurationUpdate_IEs_3,
                            p_enb_cntx);
                    break;

                    case ASN1V_x2ap_id_GUGroupIDToAddList:
                    X2AP_TRACE(X2AP_INFO,"%s: "\
                            "ASN1V_x2ap_id_GUGroupIDToAddList\n",__FUNCTION__);
                    p_enb_conf_req->bitmask |= X2AP_GU_GROUP_ID_TO_ADD_LIST_PRESENT;

                    retVal = x2ap_parse_GUGroupIDList (
                            &(p_enb_conf_req->add_gu_id.num_gu_group_id),
                            p_enb_conf_req->add_gu_id.gu_id_list,
                            p_ENBConfigurationUpdate_protocol_IEs->value.u.
                            _x2ap_ENBConfigurationUpdate_IEs_4,
                            X2AP_NULL,
                            X2AP_NULL,
                            X2AP_P_NULL);
                    break;

                    case ASN1V_x2ap_id_GUGroupIDToDeleteList:
                    X2AP_TRACE(X2AP_INFO,"%s: "\
                            "ASN1V_x2ap_id_GUGroupIDToDeleteList\n",__FUNCTION__);
                    p_enb_conf_req->bitmask |= X2AP_GU_GROUP_ID_TO_DELETE_LIST_PRESENT;

                    retVal = x2ap_parse_GUGroupIDList (
                            &(p_enb_conf_req->delete_gu_id.num_gu_group_id),
                            p_enb_conf_req->delete_gu_id.gu_id_list,
                            p_ENBConfigurationUpdate_protocol_IEs->value.u.
                            _x2ap_ENBConfigurationUpdate_IEs_5,
                            X2AP_NULL,
                            X2AP_NULL,
                            X2AP_P_NULL);
                    break;

                    default :
                    X2AP_TRACE(X2AP_ERROR,"%s:"\
                            "invalid Protocol IE id",__FUNCTION__);
                    /*SPR_19067_FIX_START*/
                    x2ap_add_to_err_ind_ie_list(&iE_list,
                            p_ENBConfigurationUpdate_protocol_IEs->criticality,
                            p_ENBConfigurationUpdate_protocol_IEs->id,
                            &ie_list_index,
                            p_send_err_ind,
                            X2AP_FALSE,
                            X2AP_FALSE);

                    if (x2ap_reject == p_ENBConfigurationUpdate_protocol_IEs->criticality) 
                    {
                        retVal = X2AP_FAILURE;
                    }	
                    /*SPR_19067_FIX_STOP*/ 

                }
                if (X2AP_FAILURE == retVal)
                {
                    X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                            "decode failed while decoding parameter id [%d].",
                            __FUNCTION__,p_ENBConfigurationUpdate_protocol_IEs->id);
                    break;
                }
                p_node = p_node->next;
            }
        }
        else
        {
            /* Drop message */
            X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                    "decode failed Unknown Message type received.",
                    __FUNCTION__);
            retVal = X2AP_FAILURE;
            break;
        }


    }while(0);

    /*SPR_19067_FIX_START*/
    /* Parse the map for Error Indication */
    if ((X2AP_SUCCESS == x2ap_parse_message_map(
                    p_asn1_ctx,
                    p_x2ap_gb_ctx,
                    p_enb_cntx, 
                    &message_map,
                    &iE_list,
                    &ie_list_index,
                    p_send_err_ind,
                    /* SPR 23762/SES-459 Fix Start */
                    ASN1V_x2ap_id_eNBConfigurationUpdate,
                    /* SPR 23762/SES-459 Fix End */
                    T_x2ap_X2AP_PDU_initiatingMessage,
                    x2ap_reject,
                    p_error_indication)))
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED ");

        retVal = X2AP_FAILURE;
    } 
    /*SPR_19067_FIX_STOP*/

    return retVal;
}
/*****************************************************************************
 * Function Name  : x2ap_decode_x2setup_req 
 * Inputs         : p_x2ap_gl_ctx - pointer to X2Ap global context 
 *                  p_enb_ctx - pointer to x2ap_peer_enb_context_t
 *                  p_x2_setup_req - pointer to x2_setup_req_t
 *                  p_asn_msg - pointer to ASN message 
 *                  p_asn_msg_len - pointer to ASN message length 
 * Outputs        : p_asn_msg,p_asn_msg_len 
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode X2 Setup request.
 * Error          : This function report error to the caller function, If decoder
 *                  fails to decode message.
 ********************************************************************************/
x2ap_return_et x2ap_decode_x2setup_req (
        x2ap_gb_context_t  *p_x2ap_gl_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx,
        x2_setup_req_t   *p_x2_setup_req,
        U8                *p_asn_msg,
        U32               *p_asn_msg_len,
        rrc_x2ap_error_indication_t *p_error_indication,
        /*SPR_19067_FIX_START*/
        x2ap_error_ind_bool_t       *p_send_err_ind,
        OSCTXT                *p_asn1_ctx
        /*SPR_19067_FIX_STOP*/
        )
{
    x2ap_return_et        retVal = X2AP_SUCCESS;

    U32                   count = 0;
    x2ap_X2AP_PDU         x2ap_pdu;
    /*SPR_19067_FIX_START*/
    /* code removed */
    /*SPR_19067_FIX_STOP*/
    x2ap_X2SetupRequest  *p_x2Setup_req;
    OSRTDListNode        *p_node = X2AP_P_NULL;
    x2ap_X2SetupResponse_protocolIEs_element
        *p_x2ap_protocol_IEs = X2AP_P_NULL;
    x2ap_error_ind_ie_list_t   iE_list;
    U16 ie_list_index = 0;
    x2ap_bool_et send_tse = X2AP_FALSE;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    U32                         encoded_msg_len = 0;

    x2ap_message_data_t message_map =
    {3, X2AP_TRUE,X2AP_TRUE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_GlobalENB_ID, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {1, ASN1V_x2ap_id_ServedCells, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {2, ASN1V_x2ap_id_GUGroupIDList, x2ap_optional, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL}
        }};

    X2AP_UT_TRACE_ENTER();

    X2AP_MEMSET(&iE_list, X2AP_NULL, sizeof(x2ap_error_ind_ie_list_t));
    if (!p_x2_setup_req)
    {
        X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                "p_x2_setup_req is NULL",__FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    /* Init ASN1 context */
    /*SPR_19067_FIX_START*/
    if ( 0 != rtInitContext(p_asn1_ctx) )
    {
        /* Drop message */
        X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                "context initialization failed.",__FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }
    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(p_asn1_ctx, p_asn_msg, *p_asn_msg_len, TRUE);

    do
    {
        if (0 != asn1PD_x2ap_X2AP_PDU(p_asn1_ctx, &x2ap_pdu) )
        {

            /* Drop message */
            X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                    "decode failed.",__FUNCTION__);

            /* Send Transfer Syntax Error */
            x2ap_build_and_send_err_indication(p_x2ap_gl_ctx, p_enb_ctx, PNULL);

            retVal = X2AP_FAILURE;
            break;
        }
        else
        {
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);

            encoded_msg_len = (U32)pe_GetMsgLen(p_asn1_ctx);
            /*SPR_19067_FIX_STOP*/

            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    encoded_msg_len);
            if (PNULL != p_event_header)
            {
                x2ap_fill_protocol_event_params(p_event_header, encoded_msg_len,
                        LOCAL_X2_SETUP_REQUEST, 
                        MESSAGE_DIRECTION_RECEIVED);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        encoded_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

        /* check whether the message is not successful */
        if ((T_x2ap_X2AP_PDU_initiatingMessage == x2ap_pdu.t) ||
                (ASN1V_x2ap_id_x2Setup ==
                 x2ap_pdu.u.initiatingMessage->procedureCode) ||
                ((X2AP_P_NULL != x2ap_pdu.u.initiatingMessage->value.u.x2Setup) &&
                 (T1x2ap__x2Setup == x2ap_pdu.u.initiatingMessage->value.t)))
        {
            p_x2Setup_req = x2ap_pdu.u.initiatingMessage->value.u.x2Setup;
            p_node = p_x2Setup_req->protocolIEs.head;

            for (count =0; count < p_x2Setup_req->protocolIEs.count; count++)
            {
                if (!p_node)
                {
                    X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                            __FUNCTION__);
                    retVal = X2AP_FAILURE;
                    break;
                }
                p_x2ap_protocol_IEs =
                    (x2ap_X2SetupResponse_protocolIEs_element *)p_node->data;
                switch (p_x2ap_protocol_IEs->id)
                {
                    case ASN1V_x2ap_id_GlobalENB_ID:
                    X2AP_TRACE(X2AP_INFO,"%s: "\
                            "ASN1V_x2ap_id_GlobalENB_ID",__FUNCTION__);
                    if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                count,
                                p_x2ap_protocol_IEs->id,
                                p_x2ap_protocol_IEs->value.u._x2ap_X2SetupResponse_IEs_1))
                    {

                        if(X2AP_FAILURE == x2ap_parse_GlobalENB_ID(
                                    &(p_x2_setup_req->global_enb_id),
                                    p_x2ap_protocol_IEs->value.u.
                                    _x2ap_X2SetupResponse_IEs_1,
                                    &message_map,
                                    (U8)count,
                                    (U8)p_x2ap_protocol_IEs->id))
                        {
                            X2AP_TRACE(X2AP_INFO,"%s: x2ap_parse_GlobalENB_ID"
                                    "failed",__FUNCTION__);
                            break;
                        }
                    }
                    break;

                    case ASN1V_x2ap_id_ServedCells:
                    X2AP_TRACE(X2AP_INFO,"%s: "\
                            "ASN1V_x2ap_id_ServedCells",__FUNCTION__);
                    if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                count,
                                p_x2ap_protocol_IEs->id,
                                p_x2ap_protocol_IEs->value.u._x2ap_X2SetupResponse_IEs_2))
                    {

                        if(X2AP_FAILURE == x2ap_parse_ServedCells (
                                    &(p_x2_setup_req->num_served_cell),
                                    p_x2_setup_req->served_cell_arr,
                                    p_x2ap_protocol_IEs->value.u.
                                    _x2ap_X2SetupResponse_IEs_2,
                                    &message_map,
                                    (U8)count,
                                    (U8)p_x2ap_protocol_IEs->id,
                                    &send_tse))
                        {
                            X2AP_TRACE(X2AP_INFO,"%s: x2ap_parse_ServedCells"
                                    "failed",__FUNCTION__);
                            break;
                        }
                        /* This will only happen when the element list exceeds
                         * the max number */
                        if (X2AP_TRUE == send_tse)
                        {
                            X2AP_TRACE(X2AP_WARNING, "Error is Detected" 
                                    "in ASN.1 Sequence, Sending Transfer Syntax Error");
                            x2ap_build_and_send_err_indication(p_x2ap_gl_ctx, p_enb_ctx, PNULL);
                            retVal = X2AP_FAILURE;
                            break;
                        }
                    }
                    break;

                    case ASN1V_x2ap_id_GUGroupIDList:
                    X2AP_TRACE(X2AP_INFO,"%s: "\
                            "ASN1V_x2ap_id_GUGroupIDList",__FUNCTION__);
                    if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                count,
                                p_x2ap_protocol_IEs->id,
                                p_x2ap_protocol_IEs->value.u._x2ap_X2SetupResponse_IEs_3))
                    {

                        if(X2AP_FAILURE == x2ap_parse_GUGroupIDList(
                                    &(p_x2_setup_req->num_gu_group_id),
                                    p_x2_setup_req->group_id_arr,
                                    p_x2ap_protocol_IEs->value.u.
                                    _x2ap_X2SetupResponse_IEs_3,
                                    &message_map,  
                                    (U8)p_x2ap_protocol_IEs->id,
                                    &send_tse))
                        {
                            X2AP_TRACE(X2AP_INFO,"%s: x2ap_parse_GUGroupIDList"
                                    "failed",__FUNCTION__);
                            break;
                        }
                        /* This will only happen when the element list exceeds
                         * the max number */
                        if (X2AP_TRUE == send_tse)
                        {
                            X2AP_TRACE(X2AP_WARNING, "Error is Detected" 
                                    "in ASN.1 Sequence, Sending Transfer Syntax Error");
                            x2ap_build_and_send_err_indication(p_x2ap_gl_ctx, p_enb_ctx, PNULL);
                            retVal = X2AP_FAILURE;
                            break;
                        }
                    }
                    break;

                    default :
                    /* Class - 1 Error - Unknown IE */
                    /* Base on Criticality fill the value */
                    X2AP_TRACE(X2AP_ERROR,"%s:"\
                            "invalid Protocol IE id",__FUNCTION__);
                    x2ap_add_to_err_ind_ie_list(&iE_list,
                            p_x2ap_protocol_IEs->criticality,
                            p_x2ap_protocol_IEs->id,
                            &ie_list_index,
                            p_send_err_ind,
                            X2AP_FALSE,
                            X2AP_FALSE);

                    /*SPR_19067_FIX_START*/
                    if (x2ap_reject == p_x2ap_protocol_IEs->criticality) 
                    {
                        retVal = X2AP_FAILURE;
                    }
                    /*SPR_19067_FIX_STOP*/
                }

                p_node = p_node->next;
            }
        }
        else
        {
            /* Drop message */
            X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                    "decode failed Unknown Message type received.",
                    __FUNCTION__);
            retVal = X2AP_FAILURE;
            break;
        }
    }while(0);
    /* Parse the map for Error Indication */
    if ((X2AP_SUCCESS == x2ap_parse_message_map(/*SPR_19067_FIX_START*/
                    p_asn1_ctx, /*SPR_19067_FIX_STOP*/
                    p_x2ap_gl_ctx,
                    p_enb_ctx, 
                    &message_map,
                    &iE_list,
                    &ie_list_index,
                    p_send_err_ind,
                    ASN1V_x2ap_id_x2Setup,
                    T_x2ap_X2AP_PDU_initiatingMessage,
                    x2ap_reject,
                    p_error_indication))
       )
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED ");

        retVal = X2AP_FAILURE;
    }

    /*SPR_19067_FIX_START*/
    /*code removed */
    /*SPR_19067_FIX_STOP*/

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_decode_x2setup_failure_resp 
 * Inputs         : p_x2ap_gl_ctx - pointer to X2AP global context
 *                  p_enb_ctx - pointer to x2ap_peer_enb_context_t
 *                  p_setup_fail - pointer to x2_setup_fail_t
 *                  p_asn_msg - pointer to ASN message
 *                  p_asn_msg_len - pointer to ASN message length 
 *                  p_asn_msg_len - pointer to ASN message length length 
 * Outputs        : None 
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode X2 Setup failure response.
 * Error          : This function report error to the caller function, If decoder
 *                  fails to decode message.
 ********************************************************************************/
x2ap_return_et x2ap_decode_x2setup_failure_resp (
        x2ap_gb_context_t  *p_x2ap_gl_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx,
        x2_setup_fail_t  *p_setup_fail,
        U8               *p_asn_msg,
        /*SPR_19067_FIX_START*/
        U32              *p_asn_msg_len,
        OSCTXT 	         *p_asn1_ctx)
/*SPR_19067_FIX_STOP*/
{
    x2ap_return_et       retVal = X2AP_SUCCESS;

    U32                  count = 0;
    x2ap_X2AP_PDU        x2ap_pdu;
    /*SPR_19067_FIX_START*/
    /* Code Removed */
    /*SPR_19067_FIX_STOP*/
    x2ap_X2SetupFailure  *p_x2Setup_fail;
    OSRTDListNode        *p_node = X2AP_P_NULL;
    x2ap_X2SetupFailure_protocolIEs_element
        *p_x2ap_ptcl_fail_IEs = X2AP_P_NULL;
    x2ap_error_ind_ie_list_t   iE_list;
    x2ap_error_ind_bool_t send_err_ind;
    U16 ie_list_index = 0;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    U32                         encoded_msg_len = 0;

    x2ap_message_data_t message_map =
    {3, X2AP_FALSE,X2AP_FALSE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_Cause, x2ap_mandatory, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {1, ASN1V_x2ap_id_TimeToWait, x2ap_optional, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {2, ASN1V_x2ap_id_CriticalityDiagnostics, x2ap_optional, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL}
        }};

    X2AP_UT_TRACE_ENTER();

    X2AP_MEMSET(&iE_list, X2AP_NULL, sizeof(x2ap_error_ind_ie_list_t));
    X2AP_MEMSET(&send_err_ind, X2AP_FALSE, sizeof(x2ap_error_ind_bool_t));
    if (!p_setup_fail)
    {
        X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                "p_setup_fail is NULL",__FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    /* Init ASN1 context */
    /*SPR_19067_FIX_START*/
    if ( 0 != rtInitContext(p_asn1_ctx) )
    {
        /* Drop message */
        X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                "context initialization failed.",__FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }
    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(p_asn1_ctx, p_asn_msg, *p_asn_msg_len, TRUE);

    do
    {
        if (0 != asn1PD_x2ap_X2AP_PDU(p_asn1_ctx, &x2ap_pdu) )
        {
            /* Drop message */
            X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                    "decode failed.",__FUNCTION__);
            /* Send Transfer Syntax Error */
            x2ap_build_and_send_err_indication(p_x2ap_gl_ctx, p_enb_ctx, PNULL);
            retVal = X2AP_FAILURE;
            break;
        }
        else
        {
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);

            encoded_msg_len = (U32)pe_GetMsgLen(p_asn1_ctx);
            /*SPR_19067_FIX_STOP*/

            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    encoded_msg_len);
            if (PNULL != p_event_header)
            {
                x2ap_fill_protocol_event_params(p_event_header, encoded_msg_len,
                        LOCAL_X2_SETUP_FAILURE, 
                        MESSAGE_DIRECTION_RECEIVED);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        encoded_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

        /* check whether the message is not successful */
        if ((T_x2ap_X2AP_PDU_unsuccessfulOutcome == x2ap_pdu.t) ||
                (ASN1V_x2ap_id_x2Setup ==
                 x2ap_pdu.u.unsuccessfulOutcome->procedureCode) ||
                ((X2AP_P_NULL != x2ap_pdu.u.unsuccessfulOutcome->value.u.x2Setup) &&
                 (T1x2ap__x2Setup == x2ap_pdu.u.unsuccessfulOutcome->value.t)))
        {
            p_x2Setup_fail = x2ap_pdu.u.unsuccessfulOutcome->value.u.x2Setup;
            p_node = p_x2Setup_fail->protocolIEs.head;

            for (count =0; count < p_x2Setup_fail->protocolIEs.count; count++)
            {
                if (!p_node)
                {
                    X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                            __FUNCTION__);
                    retVal = X2AP_FAILURE;
                    break;
                }
                p_x2ap_ptcl_fail_IEs =
                    (x2ap_X2SetupFailure_protocolIEs_element*)p_node->data;
                switch (p_x2ap_ptcl_fail_IEs->id)
                {
                    case ASN1V_x2ap_id_Cause:
                    X2AP_TRACE(X2AP_INFO,"%s: "\
                            "ASN1V_x2ap_id_Cause",__FUNCTION__);
                    if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                count,
                                p_x2ap_ptcl_fail_IEs->id,
                                p_x2ap_ptcl_fail_IEs->value.u._x2ap_X2SetupFailure_IEs_1))
                    {
                        if(X2AP_FAILURE == x2ap_parse_cause(
                                    &(p_setup_fail->cause),
                                    p_x2ap_ptcl_fail_IEs->value.u.
                                    _x2ap_X2SetupFailure_IEs_1,
                                    &message_map,
                                    (U8)count,
                                    (U8)p_x2ap_ptcl_fail_IEs->id))
                        {
                            X2AP_TRACE(X2AP_INFO,
                                    "%s:x2ap_parse_cause failed",__FUNCTION__);
                            break;
                        }
                    }
                    break;

                    case ASN1V_x2ap_id_TimeToWait:
                    X2AP_TRACE(X2AP_INFO,"%s: "\
                            "ASN1V_x2ap_id_TimeToWait",__FUNCTION__);
                    if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                count,
                                p_x2ap_ptcl_fail_IEs->id,
                                &p_x2ap_ptcl_fail_IEs->value.u._x2ap_X2SetupFailure_IEs_2))
                    {
                        /* SPR 10858 Fix Start */
                        p_setup_fail->bitmask = X2_SETUP_FAIL_TIME_TO_WAIT_PRESENT;
                        /* SPR 10858 Fix Stop */
                        /* BUG_7951 START */
                        p_setup_fail->time_to_wait =
                            (x2ap_time_to_wait_et)p_x2ap_ptcl_fail_IEs->value.u.
                            _x2ap_X2SetupFailure_IEs_2;
                        /* BUG_7951 STOP */
                        retVal = X2AP_SUCCESS;
                    }
                    break;

                    case ASN1V_x2ap_id_CriticalityDiagnostics:
                    X2AP_TRACE(X2AP_INFO,"%s: "\
                            "ASN1V_x2ap_id_CriticalityDiagnostics",__FUNCTION__);
                    if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                count,
                                p_x2ap_ptcl_fail_IEs->id,
                                p_x2ap_ptcl_fail_IEs->value.u._x2ap_X2SetupFailure_IEs_3))
                    {
                        retVal = x2ap_parse_CriticalityDiagnostics(
                                &(p_setup_fail->critc_diagno),
                                p_x2ap_ptcl_fail_IEs->value.u.
                                _x2ap_X2SetupFailure_IEs_3);
                        x2ap_log_criticality_diagostics(p_x2ap_ptcl_fail_IEs->value.u._x2ap_X2SetupFailure_IEs_3);
                    }
                    break;

                    default:
                    /* Class - 1 Error - Unknown IE */
                    /* Base on Criticality fill the value */
                    x2ap_add_to_err_ind_ie_list(&iE_list,
                            p_x2ap_ptcl_fail_IEs->criticality,
                            p_x2ap_ptcl_fail_IEs->id,
                            &ie_list_index,
                            &send_err_ind,
                            X2AP_FALSE,
                            X2AP_FALSE);
                    X2AP_TRACE(X2AP_ERROR,"%s:"\
                            "invalid Protocol IE id",__FUNCTION__);
                    /*SPR_19067_FIX_START*/
                    if (x2ap_reject == p_x2ap_ptcl_fail_IEs->criticality) 
                    {
                        retVal = X2AP_FAILURE;
                    }
                    /*SPR_19067_FIX_STOP*/
                }

                p_node = p_node->next;
            }
            break;
        }
        else
        {
            /* Drop message */
            X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                    "decode failed Unknown Message type received.",
                    __FUNCTION__);
            retVal = X2AP_FAILURE;
            break;
        }
    }while(0);
    /* Parse the map for Error Indication */
    /*SPR_19067_FIX_START*/
    if ((X2AP_SUCCESS == x2ap_parse_message_map(
                    p_asn1_ctx,
                    p_x2ap_gl_ctx,
                    p_enb_ctx, 
                    &message_map,
                    &iE_list,
                    &ie_list_index,
                    &send_err_ind,
                    ASN1V_x2ap_id_x2Setup,
                    T_x2ap_X2AP_PDU_unsuccessfulOutcome,
                    x2ap_reject,
                    (rrc_x2ap_error_indication_t *)PNULL))
       )
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED ");

        retVal = X2AP_FAILURE;
    }
    /*SPR_19067_FIX_STOP*/

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_decode_x2setup_resp 
 * Inputs         : p_x2ap_gl_ctx - pointer to X2AP contexttttttt
 *                  p_enb_ctx - pointer to x2ap_peer_enb_context_t
 *                  p_setup_resp - pointer to x2_setup_resp_t
 *                  p_asn_msg - pointer to ASN message
 *                  p_asn_msg_len - pointer to ASN message length 
 * Outputs        : None 
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE_
 * Description    : This function used to decode X2 Setup response.
 * Error          : This function report error to the caller function, If decoder
 *                  fails to decode message.
 ********************************************************************************/
x2ap_return_et x2ap_decode_x2setup_resp (
        x2ap_gb_context_t  *p_x2ap_gl_ctx,
        x2ap_peer_enb_context_t *p_enb_ctx,
        x2_setup_resp_t  *p_setup_resp,
        U8                *p_asn_msg,
        /*SPR_19067_FIX_START*/
        U32               *p_asn_msg_len,
        OSCTXT            *p_asn1_ctx)
/*SPR_19067_FIX_STOP*/
{
    x2ap_return_et       retVal = X2AP_SUCCESS;

    U32                  count = 0;
    x2ap_X2AP_PDU        x2ap_pdu;
    /*SPR_19067_FIX_START*/
    /* Code Removed */
    /*SPR_19067_FIX_STOP*/
    x2ap_X2SetupResponse *p_x2Setup_resp;
    OSRTDListNode        *p_node = X2AP_P_NULL;
    x2ap_X2SetupResponse_protocolIEs_element  
        *p_x2ap_protocol_IEs = X2AP_P_NULL;
    x2ap_error_ind_ie_list_t   iE_list;
    x2ap_error_ind_bool_t send_err_ind;
    U16 ie_list_index = 0;
    x2ap_bool_et send_tse = X2AP_FALSE;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    U32                         encoded_msg_len = 0;

    x2ap_message_data_t message_map =
    {4, X2AP_FALSE,X2AP_FALSE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_GlobalENB_ID, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {1, ASN1V_x2ap_id_ServedCells, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {2, ASN1V_x2ap_id_GUGroupIDList, x2ap_optional, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {3, ASN1V_x2ap_id_CriticalityDiagnostics, x2ap_optional, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL}
        }};

    X2AP_UT_TRACE_ENTER();

    X2AP_MEMSET(&iE_list, X2AP_NULL, sizeof(x2ap_error_ind_ie_list_t));
    X2AP_MEMSET(&send_err_ind, X2AP_FALSE, sizeof(x2ap_error_ind_bool_t));
    if (!p_setup_resp)
    {
        X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                "p_setup_resp is NULL",__FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    /* Init ASN1 context */
    /*SPR_19067_FIX_START*/
    if ( 0 != rtInitContext(p_asn1_ctx) )
    {
        /* Drop message */
        X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                "context initialization failed.",__FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }
    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(p_asn1_ctx, p_asn_msg, *p_asn_msg_len, TRUE);

    do
    {
        if (0 != asn1PD_x2ap_X2AP_PDU(p_asn1_ctx, &x2ap_pdu) )
        {
            /* Drop message */
            X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                    "decode failed.",__FUNCTION__);
            /* Send Transfer Syntax Error */
            x2ap_build_and_send_err_indication(p_x2ap_gl_ctx, p_enb_ctx, PNULL);
            retVal = X2AP_FAILURE;
            break;
        }
        else
        {
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);

            encoded_msg_len = (U32)pe_GetMsgLen(p_asn1_ctx);
            /*SPR_19067_FIX_STOP*/

            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    encoded_msg_len);
            if (PNULL != p_event_header)
            {
                x2ap_fill_protocol_event_params(p_event_header, encoded_msg_len,
                        LOCAL_X2_SETUP_RESPONSE, 
                        MESSAGE_DIRECTION_RECEIVED);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        encoded_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

        /* check whether the message is not successful */
        if ((T_x2ap_X2AP_PDU_successfulOutcome == x2ap_pdu.t) ||
                (ASN1V_x2ap_id_x2Setup ==
                 x2ap_pdu.u.successfulOutcome->procedureCode) ||
                ((X2AP_P_NULL != x2ap_pdu.u.successfulOutcome->value.u.x2Setup) &&
                 (T1x2ap__x2Setup == x2ap_pdu.u.successfulOutcome->value.t)))
        {
            p_x2Setup_resp = x2ap_pdu.u.successfulOutcome->value.u.x2Setup;
            p_node = p_x2Setup_resp->protocolIEs.head;

            for (count =0; count < p_x2Setup_resp->protocolIEs.count; count++)
            {
                if (!p_node)
                {
                    X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                            __FUNCTION__);
                    retVal = X2AP_FAILURE;
                    break;
                }
                p_x2ap_protocol_IEs = 
                    (x2ap_X2SetupResponse_protocolIEs_element *)p_node->data;
                switch (p_x2ap_protocol_IEs->id)
                {
                    case ASN1V_x2ap_id_GlobalENB_ID:
                    X2AP_TRACE(X2AP_DETAILED, "ASN1V_x2ap_id_GlobalENB_ID");

                    if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                count,
                                p_x2ap_protocol_IEs->id,
                                p_x2ap_protocol_IEs->value.u._x2ap_X2SetupResponse_IEs_1))
                    {
                        if(X2AP_FAILURE == x2ap_parse_GlobalENB_ID(
                                    &(p_setup_resp->global_enb_id),
                                    p_x2ap_protocol_IEs->value.u.
                                    _x2ap_X2SetupResponse_IEs_1,
                                    &message_map,
                                    (U8)count,
                                    (U8)p_x2ap_protocol_IEs->id))
                        {
                            X2AP_TRACE(X2AP_INFO,"%s: x2ap_parse_GlobalENB_ID"
                                    "failed",__FUNCTION__);
                            break;
                        }
                    }
                    break;

                    case ASN1V_x2ap_id_ServedCells:
                    X2AP_TRACE(X2AP_DETAILED, "ASN1V_x2ap_id_ServedCells");
                    if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                count,
                                p_x2ap_protocol_IEs->id,
                                p_x2ap_protocol_IEs->value.u._x2ap_X2SetupResponse_IEs_2))
                    {
                        if(X2AP_FAILURE == x2ap_parse_ServedCells (
                                    &(p_setup_resp->num_served_cell),
                                    p_setup_resp->served_cell_arr,
                                    p_x2ap_protocol_IEs->value.u.
                                    _x2ap_X2SetupResponse_IEs_2,
                                    &message_map,
                                    (U8)count,
                                    (U8)p_x2ap_protocol_IEs->id,
                                    &send_tse))
                        {
                            X2AP_TRACE(X2AP_INFO,"x2ap_parse_ServedCells failed");
                            break;
                        }
                        /* This will only happen when the element list exceeds
                         * the max number */
                        if (X2AP_TRUE == send_tse)
                        {
                            X2AP_TRACE(X2AP_WARNING, "Error is Detected" 
                                    "in ASN.1 Sequence, Sending Transfer Syntax Error");
                            x2ap_build_and_send_err_indication(p_x2ap_gl_ctx, p_enb_ctx, PNULL);
                            retVal = X2AP_FAILURE;
                            break;
                        }
                    }
                    break;

                    case ASN1V_x2ap_id_GUGroupIDList:
                    X2AP_TRACE(X2AP_DETAILED, "ASN1V_x2ap_id_GUGroupIDList");
                    if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                count,
                                p_x2ap_protocol_IEs->id,
                                p_x2ap_protocol_IEs->value.u._x2ap_X2SetupResponse_IEs_3))
                    {
//#ifndef RRC_UNIT_TEST_FRAMEWORK
                        if(X2AP_FAILURE == x2ap_parse_GUGroupIDList (
                                    &(p_setup_resp->num_gu_group_id),
                                    p_setup_resp->group_id_arr,
                                    p_x2ap_protocol_IEs->value.u.
                                    _x2ap_X2SetupResponse_IEs_3,
                                    &message_map, 
                                    (U8)p_x2ap_protocol_IEs->id,
                                    &send_tse))
//#endif
                        {
                            X2AP_TRACE(X2AP_INFO,"%s: x2ap_parse_GUGroupIDList"
                                    "failed",__FUNCTION__);
                            break;
                        }
                        /* This will only happen when the element list exceeds
                         * the max number */
                        if (X2AP_TRUE == send_tse)
                        {
                            X2AP_TRACE(X2AP_WARNING, "Error is Detected" 
                                    "in ASN.1 Sequence, Sending Transfer Syntax Error");
                            x2ap_build_and_send_err_indication(p_x2ap_gl_ctx, p_enb_ctx, PNULL);
                            retVal = X2AP_FAILURE;
                            break;
                        }
                    }
                    break;

                    case ASN1V_x2ap_id_CriticalityDiagnostics:
                    X2AP_TRACE(X2AP_DETAILED, "ASN1V_x2ap_id_CriticalityDiagnostics");
                    if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                count,
                                p_x2ap_protocol_IEs->id,
                                p_x2ap_protocol_IEs->value.u._x2ap_X2SetupResponse_IEs_4))
                    {
                        retVal = x2ap_parse_CriticalityDiagnostics(
                                &(p_setup_resp->critc_diagno),
                                p_x2ap_protocol_IEs->value.u.
                                _x2ap_X2SetupResponse_IEs_4);
                    }
                    break;

                    default :
                    /* Class - 1 Error - Unknown IE */
                    /* Base on Criticality fill the value */
                    x2ap_add_to_err_ind_ie_list(&iE_list,
                            p_x2ap_protocol_IEs->criticality,
                            p_x2ap_protocol_IEs->id,
                            &ie_list_index,
                            &send_err_ind,
                            X2AP_FALSE,
                            X2AP_FALSE);
                    X2AP_TRACE(X2AP_ERROR,"%s:"\
                            "invalid Protocol IE id",__FUNCTION__);
                    /*SPR_19067_FIX_START*/
                    if (x2ap_reject == p_x2ap_protocol_IEs->criticality) 
                    {
                        retVal = X2AP_FAILURE;
                    }
                    /*SPR_19067_FIX_STOP*/
                }

                p_node = p_node->next;
            }
        }
        else
        {
            /* Drop message */
            X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                    "decode failed Unknown Message type received.",
                    __FUNCTION__);
            retVal = X2AP_FAILURE;
            break;
        }
    }while(0);
    /* Parse the map for Error Indication */
    /*SPR_19067_FIX_START*/
    if ((X2AP_SUCCESS == x2ap_parse_message_map(
                    p_asn1_ctx,
                    p_x2ap_gl_ctx,
                    p_enb_ctx, 
                    &message_map,
                    &iE_list,
                    &ie_list_index,
                    &send_err_ind,
                    ASN1V_x2ap_id_x2Setup,
                    T_x2ap_X2AP_PDU_successfulOutcome,
                    x2ap_reject,
                    (rrc_x2ap_error_indication_t *)PNULL))
       )
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED ");

        retVal = X2AP_FAILURE;
    }
    /*SPR_19067_FIX_STOP*/

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_parse_CriticalityDiagnostics 
 * Inputs         : p_CriticalityDiagno - pointer to x2ap_crit_diagno_t
 *                  p_CriticalityDiagnostics - pointer to x2ap_CriticalityDiagnostics
 * Outputs        : p_CriticalityDiagno
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode Criticality Diagnostics
 * Error          : This function report error to the caller function, If decoder
 *                  fails to decode message.
 ********************************************************************************/
x2ap_return_et  x2ap_parse_CriticalityDiagnostics(
        x2ap_crit_diagno_t            *p_CriticalityDiagno,
        x2ap_CriticalityDiagnostics    *p_CriticalityDiagnostics)
{
    x2ap_return_et                   retVal = X2AP_SUCCESS;
    OSRTDListNode                    *p_cric_diagno_info = X2AP_P_NULL;
    U32                              count_cric_diag_info = 0;
    X2AP_UT_TRACE_ENTER();

    if (p_CriticalityDiagnostics->m.procedureCodePresent)
    {
        p_CriticalityDiagno->procedure_code = 
            p_CriticalityDiagnostics->procedureCode;
    }

    if (p_CriticalityDiagnostics->m.triggeringMessagePresent)
    {
        p_CriticalityDiagno->trigger_msg = 
            (x2ap_trigg_msg_en)p_CriticalityDiagnostics->triggeringMessage;
    }

    if (p_CriticalityDiagnostics->m.procedureCriticalityPresent)
    {
        p_CriticalityDiagno->proc_critic = 
            (x2ap_proc_critical_en)p_CriticalityDiagnostics->procedureCriticality;
    }

    if (p_CriticalityDiagnostics->m.iEsCriticalityDiagnosticsPresent)
    {
        p_cric_diagno_info = 
            p_CriticalityDiagnostics->iEsCriticalityDiagnostics.head;
        for (count_cric_diag_info = 0; count_cric_diag_info < 
                p_CriticalityDiagnostics->iEsCriticalityDiagnostics.count;
                count_cric_diag_info++)
        {
            p_CriticalityDiagno->critic_daigno_info[count_cric_diag_info].
                ie_ctiticality = (x2ap_proc_critical_en)((x2ap_CriticalityDiagnostics_IE_List_element *)
                        p_cric_diagno_info->data)->iECriticality;

            p_CriticalityDiagno->critic_daigno_info[count_cric_diag_info].
                ie_id = ((x2ap_CriticalityDiagnostics_IE_List_element *)
                        p_cric_diagno_info->data)->iE_ID;

            p_CriticalityDiagno->critic_daigno_info[count_cric_diag_info].
                type_of_error = (x2ap_type_of_err_en)((x2ap_CriticalityDiagnostics_IE_List_element *)
                        p_cric_diagno_info->data)->typeOfError;

            p_cric_diagno_info = p_cric_diagno_info->next;
        }
    }
    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_parse_GUGroupIDList 
 * Inputs         : p_num_GUGroupID - pointer to Gu group id
 *                  p_GUGroupIDList - pointer to x2ap_gu_group_id_t
 *                  p_GU_Group_ID_List - pointer to x2ap_GUGroupIDList
 *                  p_ie_order_map - pointer to x2ap_message_data_t
 *                  id - U8
 *                  p_send_tse - pointer to x2ap_bool_et
 * Outputs        : p_GUGroupIDList 
 * Returns        : X2AP_SUCCESSSSSSS/X2AP_FAILURE 
 * Description    : This function used to decode GU Group ID List.
 * Error          : This function report error to the caller function,If decoder 
 *                  fails to decode message.
 ********************************************************************************/
x2ap_return_et  x2ap_parse_GUGroupIDList(
        U8                          *p_num_GUGroupID,
        x2ap_gu_group_id_t         *p_GUGroupIDList,
        x2ap_GUGroupIDList          *p_GU_Group_ID_List,
        x2ap_message_data_t *p_ie_order_map,
        U8 id,
        x2ap_bool_et *p_send_tse)
{
    x2ap_return_et                   retVal = X2AP_SUCCESS;
    OSRTDListNode                   *p_node = X2AP_P_NULL;
    U8                               count = 0;

    X2AP_UT_TRACE_ENTER();

    p_node = p_GU_Group_ID_List->head;
    *p_num_GUGroupID = (U8)(p_GU_Group_ID_List->count);
    /* SPR 13041 Fix Start */ 
    if(16 < *p_num_GUGroupID)
    {
        /* coverity_fix_66592_start */
        if(X2AP_P_NULL != p_send_tse)
        {
            *p_send_tse = X2AP_TRUE;
        }    
        /* coverity_fix_66592_stop */
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE; 
        /* SPR 13041 Fix Stop */
    }

    for (count = 0; count < *p_num_GUGroupID; count++)
    {
        if (!p_node)
        {
            X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                    __FUNCTION__);
            if(p_ie_order_map){x2ap_update_message_map(p_ie_order_map,
                    DATA_MISSING, count, id);}
            retVal = X2AP_FAILURE;
            break;
        }
        X2AP_MEMCPY(p_GUGroupIDList[count].plmn_id.plmn_id,
                ((x2ap_GU_Group_ID*)p_node->data)->
                pLMN_Identity.data,
                ((x2ap_GU_Group_ID*)p_node->data)->
                pLMN_Identity.numocts);

        X2AP_MEMCPY(p_GUGroupIDList[count].mme_group_id,
                ((x2ap_GU_Group_ID*)p_node->data)->
                mME_Group_ID.data,
                MAX_MME_GRP_BYTES); 

        p_node = p_node->next;
    }
    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_parse_ServedCells 
 * Inputs         : p_num_srv_cell - pointer to num of served cell
 *                  p_served_cells_info - pointer to x2ap_srvd_cell_arry_t
 *                  p_ServedCells - pointer to x2ap_ServedCells
 *                  p_ie_order_map - pointer to x2ap_message_data_t
 *                  order_index - order index
 *                  U8 id - unsigned char 
 *                  p_send_tse - pointer to x2ap_bool_et
 * Outputs        : p_served_cells_info
 * Returns        : X2AP_SUCCESS/X2AP_FAILUREE
 * Description    : This function used to decode Served Cells Information.
 ********************************************************************************/
x2ap_return_et  x2ap_parse_ServedCells(
        U16                          *p_num_srv_cell,
        x2ap_srvd_cell_arry_t       *p_served_cells_info,
        x2ap_ServedCells             *p_ServedCells,
        x2ap_message_data_t *p_ie_order_map,
        U8 order_index,
        U8 id,
        x2ap_bool_et *p_send_tse)
{
    x2ap_return_et                  retVal = X2AP_SUCCESS;
    OSRTDListNode                   *p_neigh_cell_info = X2AP_P_NULL;
    OSRTDListNode                   *p_node = X2AP_P_NULL;
    U8                               count = 0;
    U32                              num_neigh_cell = 0;
    U8                               num_bplmn = 0;
    X2AP_UT_TRACE_ENTER();

    p_node = p_ServedCells->head;
    *p_num_srv_cell = (U16)(p_ServedCells->count);
    /* SPR 13041 Fix Start */
    if(256 < *p_num_srv_cell)
    {
        *p_send_tse = X2AP_TRUE;
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE; 
        /* SPR 13041 Fix Stop */
    }

    /* SPR 13041 Fix Start */
    if (MAX_SERVED_CELLS < *p_num_srv_cell)
    {
        X2AP_TRACE(X2AP_WARNING,"%s: ""Served Cells greater than 8 are not supported ",
                __FUNCTION__);
    }

    for (count = 0; (count < p_ServedCells->count) &&
            (count < MAX_SERVED_CELLS) ; count++)/* SPR 13041 Fix Stop */
    {
        if (!p_node)
        {
            X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                    __FUNCTION__);
            x2ap_update_message_map(p_ie_order_map, DATA_MISSING, order_index, id);
            retVal = X2AP_FAILURE;
            break;
        }
        p_served_cells_info[count].served_cell_info.pci =
            ((x2ap_ServedCells_element*)p_node->data)->servedCellInfo.pCI;

        X2AP_MEMCPY(p_served_cells_info[count].
                served_cell_info.cell_id.plmn_identity.plmn_id, 
                ((x2ap_ServedCells_element*)p_node->data)->
                servedCellInfo.cellId.pLMN_Identity.data,
                ((x2ap_ServedCells_element*)p_node->data)->
                servedCellInfo.cellId.pLMN_Identity.numocts);
        X2AP_MEMCPY(&(p_served_cells_info[count].
                    served_cell_info.cell_id.eutran_cell_id),
                ((x2ap_ServedCells_element*)p_node->data)->
                servedCellInfo.cellId.eUTRANcellIdentifier.data,
                sizeof(U32));

        X2AP_MEMCPY(p_served_cells_info[count].
                served_cell_info.track_area_code.tac,
                ((x2ap_ServedCells_element*)p_node->data)->
                servedCellInfo.tAC.data,
                ((x2ap_ServedCells_element*)p_node->data)->
                servedCellInfo.tAC.numocts);

        p_served_cells_info[count].
            served_cell_info.broadcast_plmn_info.num_bcast_plmn_id = 
            (U8)(((x2ap_ServedCells_element*)p_node->data)->
                    servedCellInfo.broadcastPLMNs.n);
        for (num_bplmn = 0;
                num_bplmn < ((x2ap_ServedCells_element*)p_node->
                    data)->servedCellInfo.broadcastPLMNs.n;
                num_bplmn++)
        {
            X2AP_MEMCPY(p_served_cells_info[count].
                    served_cell_info.broadcast_plmn_info.
                    plmn_identity[num_bplmn].plmn_id,
                    ((x2ap_ServedCells_element *)p_node->data)->servedCellInfo.
                    broadcastPLMNs.elem[num_bplmn].data,
                    ((x2ap_ServedCells_element *)p_node->data)->servedCellInfo.
                    broadcastPLMNs.elem[num_bplmn].numocts);     
        }
        p_served_cells_info[count].served_cell_info.choice_eutra_mode.
            choice_eutra_mode.bitmask = 
            (U8)(((x2ap_ServedCells_element*)p_node->data)->
                    servedCellInfo.eUTRA_Mode_Info.t);

        if (T_x2ap_EUTRA_Mode_Info_fDD ==
                ((x2ap_ServedCells_element*)p_node->data)->
                servedCellInfo.eUTRA_Mode_Info.t)
        {
            p_served_cells_info[count].served_cell_info.
                choice_eutra_mode.choice_eutra_mode.fdd_info.ul_earfcn.earfcn =
                ((x2ap_ServedCells_element*)p_node->data)->servedCellInfo.
                eUTRA_Mode_Info.u.fDD->uL_EARFCN;
            p_served_cells_info[count].served_cell_info.
                choice_eutra_mode.choice_eutra_mode.fdd_info.dl_earfcn.earfcn =
                ((x2ap_ServedCells_element*)p_node->data)->servedCellInfo.
                eUTRA_Mode_Info.u.fDD->dL_EARFCN;

            p_served_cells_info[count].served_cell_info.
                choice_eutra_mode.choice_eutra_mode.fdd_info.
                ul_trnsmission_bdwidth =
                (U8)(((x2ap_ServedCells_element*)p_node->data)->servedCellInfo.
                        eUTRA_Mode_Info.u.fDD->uL_Transmission_Bandwidth);
            p_served_cells_info[count].served_cell_info.
                choice_eutra_mode.choice_eutra_mode.fdd_info.
                dl_trnsmission_bdwidth =
                (U8)(((x2ap_ServedCells_element*)p_node->data)->servedCellInfo.
                        eUTRA_Mode_Info.u.fDD->dL_Transmission_Bandwidth);

        }
        else if (T_x2ap_EUTRA_Mode_Info_tDD ==
                ((x2ap_ServedCells_element*)p_node->data)->
                servedCellInfo.eUTRA_Mode_Info.t)
        {
            p_served_cells_info[count].served_cell_info.
                choice_eutra_mode.choice_eutra_mode.tdd_info.earfcn.earfcn =
                ((x2ap_ServedCells_element*)p_node->data)->servedCellInfo.
                eUTRA_Mode_Info.u.tDD->eARFCN;
            p_served_cells_info[count].served_cell_info.
                choice_eutra_mode.choice_eutra_mode.tdd_info.trnsmission_bdwidth = 
                (U8)(((x2ap_ServedCells_element*)p_node->data)->servedCellInfo.
                        eUTRA_Mode_Info.u.tDD->transmission_Bandwidth);
            p_served_cells_info[count].served_cell_info.
                choice_eutra_mode.choice_eutra_mode.tdd_info.subframe_assn =
                (U8)(((x2ap_ServedCells_element*)p_node->data)->servedCellInfo.
                        eUTRA_Mode_Info.u.tDD->subframeAssignment);
            p_served_cells_info[count].served_cell_info.
                choice_eutra_mode.choice_eutra_mode.tdd_info.
                special_subframe_info.special_subframe_patterns =
                (U8)(((x2ap_ServedCells_element*)p_node->data)->servedCellInfo.
                        eUTRA_Mode_Info.u.tDD->specialSubframe_Info.
                        specialSubframePatterns);
            p_served_cells_info[count].served_cell_info.
                choice_eutra_mode.choice_eutra_mode.tdd_info.
                special_subframe_info.dl_cp = 
                (U8)(((x2ap_ServedCells_element*)p_node->data)->
                        servedCellInfo.eUTRA_Mode_Info.u.tDD->
                        specialSubframe_Info.cyclicPrefixDL);
            p_served_cells_info[count].served_cell_info.
                choice_eutra_mode.choice_eutra_mode.tdd_info.
                special_subframe_info.ul_cp =
                (U8)(((x2ap_ServedCells_element*)p_node->data)->
                        servedCellInfo.eUTRA_Mode_Info.
                        u.tDD->specialSubframe_Info.cyclicPrefixUL);
        }
        else
        {
            X2AP_TRACE(X2AP_ERROR,"%s: ""Eutran mode is not present",
                    __FUNCTION__);
            x2ap_update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
            retVal = X2AP_FAILURE;
            break;
        }
        if (((x2ap_ServedCells_element *)p_node->data)->m.neighbour_InfoPresent)
        {
            p_neigh_cell_info = ((x2ap_ServedCells_element*)p_node->data)->
                neighbour_Info.head;
            p_served_cells_info[count].nbour_cell_info.num_neighbour_cell =
                (U16)(((x2ap_ServedCells_element*)p_node->data)->
                        neighbour_Info.count); 
            for (num_neigh_cell = 0; 
                    num_neigh_cell < ((x2ap_ServedCells_element*)p_node->
                        data)->neighbour_Info.count; 
                    num_neigh_cell++) 
            {
                X2AP_MEMCPY(p_served_cells_info[count].nbour_cell_info.
                        neighbour_info_arr[num_neigh_cell].ecgi.
                        plmn_identity.plmn_id,
                        ((x2ap_Neighbour_Information_element *)
                         p_neigh_cell_info->data)->eCGI.pLMN_Identity.data,
                        ((x2ap_Neighbour_Information_element *)
                         p_neigh_cell_info->data)->eCGI.pLMN_Identity.numocts);
                X2AP_MEMCPY(&(p_served_cells_info[count].nbour_cell_info.
                            neighbour_info_arr[num_neigh_cell].ecgi.
                            eutran_cell_id),
                        ((x2ap_Neighbour_Information_element *)
                         p_neigh_cell_info->data)->eCGI.eUTRANcellIdentifier
                        .data,
                        sizeof (U32));
                p_served_cells_info[count].nbour_cell_info.
                    neighbour_info_arr[num_neigh_cell].pci =
                    ((x2ap_Neighbour_Information_element *)
                     p_neigh_cell_info->data)->pCI;
                p_served_cells_info[count].nbour_cell_info.
                    neighbour_info_arr[num_neigh_cell].earfcn.earfcn =
                    ((x2ap_Neighbour_Information_element *)
                     p_neigh_cell_info->data)->eARFCN;
                p_neigh_cell_info = p_neigh_cell_info->next;
            }
        }

        if(((x2ap_ServedCells_element *)p_node->data)->servedCellInfo.m.iE_ExtensionsPresent)
        {
            U8 extensions_count = 0;
            U8 extensions_counter =0;
            OSRTDListNode* p_node_1 = X2AP_P_NULL;

            p_node_1 = ((x2ap_ServedCells_element *)p_node->data)->servedCellInfo.iE_Extensions.head;
            extensions_count = ((x2ap_ServedCells_element *)p_node->data)->servedCellInfo.iE_Extensions.count;

            for(extensions_counter=0;extensions_counter<extensions_count; extensions_counter++)
            {
                if(((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.t==T341x2ap___x2ap_ServedCell_Information_ExtIEs_1)
                {
                    p_served_cells_info[count].served_cell_info.bitmask |= X2AP_NUM_ANTENNA_INFO_PORT_PRESENT;
                    p_served_cells_info[count].served_cell_info.num_antenna_port =
                        (x2ap_num_antenna_port_et)((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->
                        extensionValue.u._x2ap_ServedCell_Information_ExtIEs_1;
                }
                else if(((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.t==T341x2ap___x2ap_ServedCell_Information_ExtIEs_2)
                {
                    p_served_cells_info[count].served_cell_info.bitmask |= X2AP_PRACH_CONFIG_INFO_PRESENT;
                    p_served_cells_info[count].served_cell_info.prach_config.high_speed_flag =
                        ((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_2->highSpeedFlag;
                    p_served_cells_info[count].served_cell_info.prach_config.prach_frequency_offset =
                        ((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_2->prach_FreqOffset;
                    p_served_cells_info[count].served_cell_info.prach_config.root_sequence_index =
                        ((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_2->rootSequenceIndex;
                    p_served_cells_info[count].served_cell_info.prach_config.zero_correlation_zone_config  =
                        ((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_2->zeroCorrelationIndex;

                    if(((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_2->m.prach_ConfigIndexPresent)
                    {
                        p_served_cells_info[count].served_cell_info.prach_config.bitmask |= X2AP_PRACH_CONFIG_INDEX_PRESENT;
                        p_served_cells_info[count].served_cell_info.prach_config.prach_config_index =
                            ((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_2->prach_ConfigIndex;
                    }
                }
                /* SPR 18793 changes start */ 
#ifdef LTE_EMBMS_SUPPORTED
                else if(((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.t==T341x2ap___x2ap_ServedCell_Information_ExtIEs_3)
                {
                    U8 mbsfn_area_count = X2AP_ZERO,index = X2AP_ZERO;
                    x2ap_MBSFN_Subframe_Info* p_mbsfn_subframe_info = X2AP_P_NULL;
                    OSRTDListNode* p_mbsfn_area_node = X2AP_P_NULL;
                    p_served_cells_info[count].served_cell_info.bitmask |= X2AP_MBSFN_SUBFRAME_INFO_PRESENT;
                    p_served_cells_info[count].served_cell_info.mbsfn_subframe_info.num_mbsfn =
                        ((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_3->count;

                    mbsfn_area_count = p_served_cells_info[count].served_cell_info.mbsfn_subframe_info.num_mbsfn;

                    p_mbsfn_area_node = ((x2ap_ServedCell_Information_iE_Extensions_element*)
                            p_node_1->data)->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_3->head;

                    X2AP_TRACE(X2AP_INFO, "Num MBSFN Areas:%u %s",
                            p_served_cells_info[count].served_cell_info.mbsfn_subframe_info.num_mbsfn,
                            __FUNCTION__);

                    for(index = X2AP_ZERO;index < mbsfn_area_count ; index++)
                    {
                        if(!p_mbsfn_area_node)
                        {
                            X2AP_TRACE(X2AP_ERROR, "%s: ""next node returned NULL",
                                    __FUNCTION__);
                            retVal = X2AP_FAILURE;
                            break;

                        }
                        p_mbsfn_subframe_info = (x2ap_MBSFN_Subframe_Info*)p_mbsfn_area_node->data;

                        p_served_cells_info[count].served_cell_info.mbsfn_subframe_info.mbsfn_info[index].radioframe_alloc_period =
                            p_mbsfn_subframe_info->radioframeAllocationPeriod;

                        X2AP_TRACE(X2AP_INFO, "radioframe_alloc_period:%u %s",
                                p_served_cells_info[count].served_cell_info.mbsfn_subframe_info.mbsfn_info[index].radioframe_alloc_period,
                                __FUNCTION__);

                        p_served_cells_info[count].served_cell_info.mbsfn_subframe_info.mbsfn_info[index].radioframe_alloc_offset =
                            p_mbsfn_subframe_info->radioframeAllocationOffset;

                        X2AP_TRACE(X2AP_INFO, "radioframeAllocationOffset:%u %s",
                                p_served_cells_info[count].served_cell_info.mbsfn_subframe_info.mbsfn_info[index].radioframe_alloc_offset,
                                __FUNCTION__);
                        if(T_MBSFN_SubframeConfig_subframeAllocation_oneFrame == p_mbsfn_subframe_info->subframeAllocation.t)
                        {
                            p_served_cells_info[count].served_cell_info.
                                mbsfn_subframe_info.mbsfn_info[index].subframe_alloc.bitmask |=
                                X2AP_SUBFRAME_ALLOC_ONE_FRAME_INFO_PRESENT;

                            X2AP_MEMCPY(p_served_cells_info[count].served_cell_info.
                                    mbsfn_subframe_info.mbsfn_info[index].subframe_alloc.oneframe,
                                    p_mbsfn_subframe_info->subframeAllocation.u.oneframe->data,
                                    X2AP_SUBFRAME_ALLOC_ONE_FRAME_OCTET_SIZE);

                        }
                        else if(T_MBSFN_SubframeConfig_subframeAllocation_fourFrames == p_mbsfn_subframe_info->subframeAllocation.t)
                        {
                            p_served_cells_info[count].served_cell_info.
                                mbsfn_subframe_info.mbsfn_info[index].subframe_alloc.bitmask |=
                                X2AP_SUBFRAME_ALLOC_FOUR_FRAME_INFO_PRESENT;

                            X2AP_MEMCPY(p_served_cells_info[count].served_cell_info.
                                    mbsfn_subframe_info.mbsfn_info[index].subframe_alloc.fourframe,
                                    p_mbsfn_subframe_info->subframeAllocation.u.fourframes->data,
                                    X2AP_SUBFRAME_ALLOC_FOUR_FRAME_OCTET_SIZE);

                        }
                        p_mbsfn_area_node = p_mbsfn_area_node->next;
                    }
                }
#endif
                /* SPR 18793 changes end */ 
                /*36423_CR0407_start*/
                if(((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.t==T341x2ap___x2ap_ServedCell_Information_ExtIEs_4)
                {
                    p_served_cells_info[count].served_cell_info.bitmask |= X2AP_CSG_ID_INFO_PRESENT;
                    l3_memcpy_wrapper(p_served_cells_info[count].served_cell_info.csg_identity,
                            ((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_4->data,sizeof(p_served_cells_info[count].served_cell_info.csg_identity));
                }
                /*36423_CR0407_stop*/
                p_node_1 = p_node_1->next;
            }
        }
        p_node = p_node->next;
    }
    /*SPR_17901_FIX_START */
    *p_num_srv_cell = count;
    /*SPR_17901_FIX_END */
    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_parse_cause 
 * Inputs         : p_cause - pointer to x2_cause_t
 *                  p_Cause - pointer to x2ap_Cause
 *                  p_ie_order_map - pointer to x2ap_message_data_t
 *                  order_index - index
 *                  id - ID
 * Outputs        : p_cause
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function report error to the caller function, If decoder
 *                  fails  to decode message.  
 ********************************************************************************/
x2ap_return_et  x2ap_parse_cause (x2_cause_t    *p_cause,
        x2ap_Cause     *p_Cause,
        x2ap_message_data_t *p_ie_order_map,
        U8 order_index,
        U8 id)
{
    x2ap_return_et                  retVal = X2AP_SUCCESS;
    X2AP_UT_TRACE_ENTER();

    p_cause->cause_type = p_Cause->t;

    if (RADIO_NW_LAYER_CAUSE == p_cause->cause_type)
    {
        p_cause->cause_info.radio_nw_cause =
            ((x2ap_Cause*)p_Cause)->u.radioNetwork; 
    }
    else if (TRANSPORT_LAYER_CAUSE == p_cause->cause_type)
    {
        p_cause->cause_info.transport_cause =
            ((x2ap_Cause*)p_Cause)->u.transport;
    }
    else if (PROTOCOL_CAUSE == p_cause->cause_type)
    {
        p_cause->cause_info.protocol_cause =
            ((x2ap_Cause*)p_Cause)->u.protocol;
    }
    else if (MISC_CAUSE == p_cause->cause_type)
    {
        p_cause->cause_info.misc_cause =
            ((x2ap_Cause*)p_Cause)->u.misc;
    }
    else
    {
        X2AP_TRACE(X2AP_ERROR,"%s: ""Unknown cause type",
                __FUNCTION__);
        if(p_ie_order_map){x2ap_update_message_map(p_ie_order_map, DATA_MISSING,
                order_index, id);}
        retVal = X2AP_FAILURE;
    }
    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_parse_GlobalENB_ID
 * Inputs         : p_glb_enbId - pointer to x2_gb_enb_id_t
 *                  p_glb_enb_id - pointer to x2ap_GlobalENB_ID
 *                  p_ie_order_map - pointer to x2ap_message_data_t
 *                  order_index order index
 *                  id - ID
 * Outputs        : p_glb_enb_id
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode GlobalENB ID
 ********************************************************************************/
x2ap_return_et  x2ap_parse_GlobalENB_ID(x2_gb_enb_id_t         *p_glb_enbId,   
        x2ap_GlobalENB_ID    *p_glb_enb_id,
        x2ap_message_data_t *p_ie_order_map,
        U8 order_index,
        U8 id)
{
    x2ap_return_et                  retVal = X2AP_SUCCESS;
    X2AP_UT_TRACE_ENTER();

    X2AP_MEMCPY(p_glb_enbId->plmn_identity.plmn_id,
            ((x2ap_GlobalENB_ID *)p_glb_enb_id)->pLMN_Identity.data,
            MAX_PLMN_ID_BYTES);

    p_glb_enbId->enb_id.presence_bitmask = 
        (U8)((x2ap_GlobalENB_ID *)p_glb_enb_id)->eNB_ID.t;

    if (T_x2ap_ENB_ID_macro_eNB_ID == 
            ((x2ap_GlobalENB_ID *)p_glb_enb_id)->eNB_ID.t)
    {
        X2AP_MEMCPY(p_glb_enbId->enb_id.macro_enb_id.eNB_id,
                ((x2ap_GlobalENB_ID *)p_glb_enb_id)->
                eNB_ID.u.macro_eNB_ID->data,
                MACRO_ENB_ID_OCTET_SIZE);
    }
    else if (T_x2ap_ENB_ID_home_eNB_ID ==
            ((x2ap_GlobalENB_ID *)p_glb_enb_id)->eNB_ID.t)
    {
        X2AP_MEMCPY(p_glb_enbId->enb_id.home_enb_id.eNB_id,
                ((x2ap_GlobalENB_ID *)p_glb_enb_id)->
                eNB_ID.u.home_eNB_ID->data,
                HOME_ENB_ID_OCTET_SIZE); 
    } 
    else
    {
        X2AP_TRACE(X2AP_ERROR,"%s: ""Enb Id is not present",
                __FUNCTION__);
        x2ap_update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
        retVal = X2AP_FAILURE;
    }
    X2AP_UT_TRACE_EXIT();
    return retVal;
}


#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
/*****************************************************************************
 * Function Name  : x2ap_internal_decode_nr_cg_config 
 * Inputs         : p_container - pointer to x2ap_SgNBtoMeNBContainer
 *                  sgnb_to_menb_container - SgNB to MeNB Container
 *                  p_asn1_ctx - pointer to OSCTXT
 *                  cause - pointer to x2ap_Cause
 * Outputs        : sgnb_to_menb_container
 * Returns        : None
 * Description    : This function used to decode SgNB To MeNB Container for nr
 *                  cg config.
 ********************************************************************************/
void x2ap_internal_decode_nr_cg_config(
        x2ap_SgNBtoMeNBContainer    *p_container,
        nr_rrc_CG_Config            *p_sgnb_to_menb_container,
        OSCTXT                      *p_asn1_ctx,
        x2ap_Cause                  *cause)
{
    X2AP_UT_TRACE_ENTER();

    cause->t = T_x2ap_Cause_protocol;
    U8  rrc_result =0;

    X2AP_ASSERT(PNULL != p_sgnb_to_menb_container);
    X2AP_MEMSET(p_sgnb_to_menb_container, RRC_NULL, sizeof(nr_rrc_CG_Config));

    do
    {
        if ( 0 != rtInitContext(p_asn1_ctx) )
        {
            /* Drop message */
            X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                    "context initialization failed.",__FUNCTION__);
            X2AP_UT_TRACE_EXIT();
            break;
        }

        pu_setBuffer(p_asn1_ctx,
                (OSOCTET *)p_container->data,
                p_container->numocts,
                ASN_RRC_BUF_TYPE);

        if ( RT_OK != asn1PD_nr_rrc_CG_Config(p_asn1_ctx,
                    p_sgnb_to_menb_container))
        {
            /* Drop message */
            rtxErrPrint(p_asn1_ctx); 
            X2AP_TRACE(X2AP_WARNING,
                    "[%s] ASN decode failed.",__FUNCTION__);
            cause->u.protocol = x2ap_abstract_syntax_error_reject;
            break;
        }
        else
        {
            if(RRC_ONE == rrc_check_logger_type(RRC_ASN))
            {
                if (T_nr_rrc_CG_Config_criticalExtensions_c1 ==
                        p_sgnb_to_menb_container->criticalExtensions.t &&
                        PNULL != p_sgnb_to_menb_container->criticalExtensions.u.c1 &&
                        T_nr_rrc_CG_Config_criticalExtensions_c1_cg_Config ==
                        p_sgnb_to_menb_container->criticalExtensions.u.c1->t)
                {
                    if(p_sgnb_to_menb_container->criticalExtensions.u.c1->u.cg_Config->m.scg_CellGroupConfigPresent)
                    {
                        pu_setBuffer(p_asn1_ctx,
                                (OSOCTET *)p_sgnb_to_menb_container->criticalExtensions.u.c1->u.cg_Config->scg_CellGroupConfig.data,
                                p_sgnb_to_menb_container->criticalExtensions.u.c1->u.cg_Config->scg_CellGroupConfig.numocts,
                                ASN_RRC_BUF_TYPE);
                        nr_rrc_RRCReconfiguration scg_cellgroupconfig;
                        if ( RT_OK != asn1PD_nr_rrc_RRCReconfiguration(p_asn1_ctx,
                                    &scg_cellgroupconfig))
                        {
                            rtxErrPrint(p_asn1_ctx);
                            X2AP_TRACE(X2AP_WARNING,
                                    "[%s] ASN decode failed.",__FUNCTION__);
                        }
                        else
                        {
                            if(T_nr_rrc_RRCReconfiguration_criticalExtensions_rrcReconfiguration ==
                                    scg_cellgroupconfig.criticalExtensions.t &&
                                    PNULL != scg_cellgroupconfig.criticalExtensions.u.rrcReconfiguration &&
                                    scg_cellgroupconfig.criticalExtensions.u.rrcReconfiguration->m.secondaryCellGroupPresent)
                            {
                                pu_setBuffer(p_asn1_ctx,
                                        (OSOCTET *)scg_cellgroupconfig.criticalExtensions.u.rrcReconfiguration->secondaryCellGroup.data,
                                        scg_cellgroupconfig.criticalExtensions.u.rrcReconfiguration->secondaryCellGroup.numocts,
                                        ASN_RRC_BUF_TYPE);
                                nr_rrc_CellGroupConfig secondarycellgroup;
                                if ( RT_OK != asn1PD_nr_rrc_CellGroupConfig(p_asn1_ctx,
                                            &secondarycellgroup))
                                {
                                    rtxErrPrint(p_asn1_ctx);
                                    X2AP_TRACE(X2AP_WARNING,
                                            "[%s] ASN decode failed.",__FUNCTION__);
                                }
                                else
                                {
                                    S8* pBuff = rrc_mem_get(MAX_ASN_PRINT_BUFF_SIZE);

                                    if(pBuff != NULL)
                                    {
                                        memset_wrapper(pBuff,0, MAX_ASN_PRINT_BUFF_SIZE);

                                        /* Converted to char* as required by ASN function */
                                        rrc_result = asn1PrtToStr_nr_rrc_CellGroupConfig(
                                                "secondaryCellGroup --> nr_rrc_CellGroupConfig",
                                                &secondarycellgroup,
                                                (char*)pBuff,
                                                MAX_ASN_PRINT_BUFF_SIZE);

                                        if(rrc_result == 0)
                                        {
                                            RRC_TRACE_ASN((S32)RRC_ASN, "%s\n", pBuff);
                                        }
                                        else
                                        {
                                            RRC_TRACE(RRC_WARNING,"Buffer size for ASN needs to be increased");
                                        }
                                        rrc_mem_free(pBuff);
                                    }

                                }
                                S8* pBuff = rrc_mem_get(MAX_ASN_PRINT_BUFF_SIZE);

                                if(pBuff != NULL)
                                {
                                    memset_wrapper(pBuff,0, MAX_ASN_PRINT_BUFF_SIZE);

                                    /* Converted to char* as required by ASN function */
                                    rrc_result = asn1PrtToStr_nr_rrc_RRCReconfiguration(
                                            "scg_CellGroupConfig --> nr_rrc_RRCReconfiguration",
                                            &scg_cellgroupconfig,
                                            (char*)pBuff,
                                            MAX_ASN_PRINT_BUFF_SIZE);

                                    if(rrc_result == 0)
                                    {
                                        RRC_TRACE_ASN((S32)RRC_ASN, "%s\n", pBuff);
                                    }
                                    else
                                    {
                                        RRC_TRACE(RRC_WARNING,"Buffer size for ASN needs to be increased");
                                    }
                                    rrc_mem_free(pBuff);

                                }
                            }
                        }
                    }
                    if(p_sgnb_to_menb_container->criticalExtensions.u.c1->u.cg_Config->m.scg_RB_ConfigPresent)
                    {
                        pu_setBuffer(p_asn1_ctx,
                                (OSOCTET *)p_sgnb_to_menb_container->criticalExtensions.u.c1->u.cg_Config->scg_RB_Config.data,
                                p_sgnb_to_menb_container->criticalExtensions.u.c1->u.cg_Config->scg_RB_Config.numocts,
                                ASN_RRC_BUF_TYPE);
                        nr_rrc_RadioBearerConfig scg_rbconfig;
                        if ( RT_OK != asn1PD_nr_rrc_RadioBearerConfig(p_asn1_ctx,
                                    &scg_rbconfig))
                        {
                            rtxErrPrint(p_asn1_ctx);
                            X2AP_TRACE(X2AP_WARNING,
                                    "[%s] ASN decode failed.",__FUNCTION__);
                        }
                        else
                        {
                            S8* pBuff = rrc_mem_get(MAX_ASN_PRINT_BUFF_SIZE);

                            if(pBuff != NULL)
                            {
                                memset_wrapper(pBuff,0, MAX_ASN_PRINT_BUFF_SIZE);

                                /* Converted to char* as required by ASN function */
                                rrc_result = asn1PrtToStr_nr_rrc_RadioBearerConfig(
                                        "scg_RB_Config --> nr_rrc_RadioBearerConfig",
                                        &scg_rbconfig,
                                        (char*)pBuff,
                                        MAX_ASN_PRINT_BUFF_SIZE);

                                if(rrc_result == 0)
                                {
                                    RRC_TRACE_ASN((S32)RRC_ASN, "%s\n", pBuff);
                                }
                                else
                                {
                                    RRC_TRACE(RRC_WARNING,"Buffer size for ASN needs to be increased");
                                }
                                rrc_mem_free(pBuff);
                            }
                        }
                    }
                }
            }
            if(RRC_SUCCESS == rrc_check_logger_type(RRC_ASN))
            {
                S8* pBuff = rrc_mem_get(MAX_ASN_PRINT_BUFF_SIZE);

                if(pBuff != NULL)
                {
                    memset_wrapper(pBuff,0, MAX_ASN_PRINT_BUFF_SIZE);

                    /* Converted to char* as required by ASN function */
                    rrc_result = asn1PrtToStr_nr_rrc_CG_Config(
                            "nr-CgConfig",
                            p_sgnb_to_menb_container,
                            (char*)pBuff,
                            MAX_ASN_PRINT_BUFF_SIZE);

                    if(rrc_result == 0)
                    {
                        RRC_TRACE_ASN((S32)RRC_ASN, "%s\n", pBuff);
                    }
                    else
                    {
                        RRC_TRACE(RRC_WARNING,"Buffer size for ASN needs to be increased");
                    }
                    rrc_mem_free(pBuff);
                }
            }
        }

        if ((cause->u.protocol != x2ap_abstract_syntax_error_ignore_and_notify)&&
                (cause->u.protocol != x2ap_abstract_syntax_error_reject))
        {
            cause->t = X2AP_CAUSE_SUCCESS;
        }
    } while(0);

    X2AP_UT_TRACE_EXIT();
}
/*****************************************************************************
 * Function Name  : x2ap_internal_decode_sgnb_addition_request_ack 
 * Inputs         : pasn1_ctx - pointer to ASN1 OSCTXT
 *                  p_3gpp - pointer to x2ap_SgNBAdditionRequestAck 3GPP
 *                  p_intrl - pointer to internal msg rrc_x2ap_sgnb_addition_req_ack_t
 *                  peer_enodeb_id - Peer EnodeB ID
 * Outputs        : p_intrl - x2ap_sgnb_addition_request_ack_t filled msg
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode sgnb addition request ack
 ********************************************************************************/
x2ap_return_et x2ap_internal_decode_sgnb_addition_request_ack(
        OSCTXT                           *p_asn1_ctx,
        x2ap_SgNBAdditionRequestAcknowledge
        *p_3gpp,
        rrc_x2ap_sgnb_addition_req_ack_t
        *p_intrl,
        U8                               peer_enodeb_id
)
{

    x2ap_return_et                   result          = X2AP_SUCCESS;
    U32                              index           = X2AP_NULL;
    OSRTDListNode                    *p_node         = PNULL;
    x2ap_Cause                       cause;
    uecc_x2ap_error_ind_bool_t       send_err_ind;

    x2ap_SgNBAdditionRequestAcknowledge_protocolIEs_element
        *p_protocol_IE  = X2AP_P_NULL;
    uecc_x2ap_error_ind_ie_list_t    iE_list;
    U16                              ie_list_index   = 0;

    uecc_x2ap_message_data_t message_map =
    {10, X2AP_TRUE, X2AP_TRUE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_MeNB_UE_X2AP_ID, 
             x2ap_mandatory, x2ap_reject, X2AP_NULL, X2AP_NULL, X2AP_NULL, X2AP_NULL},
        {1, ASN1V_x2ap_id_SgNB_UE_X2AP_ID, 
            x2ap_mandatory, x2ap_reject, X2AP_NULL, X2AP_NULL, X2AP_NULL, X2AP_NULL},
        {2, ASN1V_x2ap_id_E_RABs_Admitted_ToBeAdded_SgNBAddReqAckList, 
            x2ap_mandatory, x2ap_ignore, X2AP_NULL, X2AP_NULL, X2AP_NULL, X2AP_NULL},
        {3, ASN1V_x2ap_id_E_RABs_NotAdmitted_List, 
            x2ap_optional,  x2ap_ignore, X2AP_NULL, X2AP_NULL, X2AP_NULL, X2AP_NULL},
        {4, ASN1V_x2ap_id_SgNBtoMeNBContainer, 
            x2ap_mandatory, x2ap_reject, X2AP_NULL, X2AP_NULL, X2AP_NULL, X2AP_NULL},
        {5, ASN1V_x2ap_id_CriticalityDiagnostics, 
            x2ap_optional,  x2ap_ignore, X2AP_NULL, X2AP_NULL, X2AP_NULL, X2AP_NULL},
        {6, ASN1V_x2ap_id_MeNB_UE_X2AP_ID_Extension, 
            x2ap_optional,  x2ap_reject, X2AP_NULL, X2AP_NULL, X2AP_NULL, X2AP_NULL},
        {7, ASN1V_x2ap_id_AdmittedSplitSRBs, 
            x2ap_optional,  x2ap_reject, X2AP_NULL, X2AP_NULL, X2AP_NULL, X2AP_NULL},
        {8, ASN1V_x2ap_id_SgNBResourceCoordinationInformation, 
            x2ap_optional,  x2ap_ignore, X2AP_NULL, X2AP_NULL, X2AP_NULL, X2AP_NULL},
        {9, ASN1V_x2ap_id_RRCConfigIndication, 
            x2ap_optional,  x2ap_reject, X2AP_NULL, X2AP_NULL, X2AP_NULL, X2AP_NULL},
        }};

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != p_3gpp);
    X2AP_ASSERT(PNULL != p_intrl);

    X2AP_MEMSET(p_intrl, RRC_NULL, sizeof(rrc_x2ap_sgnb_addition_req_ack_t));
    X2AP_MEMSET(&iE_list, RRC_NULL, sizeof(uecc_x2ap_error_ind_ie_list_t));
    X2AP_MEMSET(&cause, RRC_NULL, sizeof(x2ap_Cause));
    X2AP_MEMSET(&send_err_ind, 0, sizeof(uecc_x2ap_error_ind_bool_t));

    do
    {
        p_node = p_3gpp->protocolIEs.head;
        /* Save the decoded Protocol IEs */
        for (index = RRC_NULL;
                index < p_3gpp->protocolIEs.count;
                index++)
        {
            if (!p_node)
            {
                X2AP_TRACE(X2AP_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }

            p_protocol_IE =
                (x2ap_SgNBAdditionRequestAcknowledge_protocolIEs_element *)p_node->data;
            switch (p_protocol_IE->id)
            {
                case ASN1V_x2ap_id_MeNB_UE_X2AP_ID:
                {
                    /* Fill MeNB_UE_X2AP_ID */
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(
                                &message_map,
                                index,
                                ASN1V_x2ap_id_MeNB_UE_X2AP_ID,
                                &p_protocol_IE->value.
                                u._x2ap_SgNBAdditionRequestAcknowledge_IEs_1))
                    {
                        p_intrl->menb_ue_x2ap_id =
                            p_protocol_IE->value.
                            u._x2ap_SgNBAdditionRequestAcknowledge_IEs_1;
                    }
                    break;
                }

                case ASN1V_x2ap_id_SgNB_UE_X2AP_ID:
                {
                    /* Fill SgNB_UE_X2AP_ID */
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(
                                &message_map,
                                index,
                                ASN1V_x2ap_id_SgNB_UE_X2AP_ID,
                                &p_protocol_IE->value.
                                u._x2ap_SgNBAdditionRequestAcknowledge_IEs_2))
                    {
                        p_intrl->sgnb_ue_x2ap_id =
                            p_protocol_IE->value.
                            u._x2ap_SgNBAdditionRequestAcknowledge_IEs_2;
                    }
                    break;
                }

                case ASN1V_x2ap_id_E_RABs_Admitted_ToBeAdded_SgNBAddReqAckList:
                {
                    /* Fill E_RABs_Admitted_ToBeAdded_SgNBAddReqAckList */
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(
                                &message_map,
                                index,
                                ASN1V_x2ap_id_E_RABs_Admitted_ToBeAdded_SgNBAddReqAckList,
                                p_protocol_IE->value.
                                u._x2ap_SgNBAdditionRequestAcknowledge_IEs_3))
                    {
                        p_intrl->erabs_admittied_list =
                            *p_protocol_IE->value.
                            u._x2ap_SgNBAdditionRequestAcknowledge_IEs_3;
                    }
                    break;
                }

                case ASN1V_x2ap_id_E_RABs_NotAdmitted_List:
                {
                    /* Fill E_RABs_NotAdmitted_List */
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(
                                &message_map,
                                index,
                                ASN1V_x2ap_id_E_RABs_NotAdmitted_List,
                                p_protocol_IE->value.
                                u._x2ap_SgNBAdditionRequestAcknowledge_IEs_4))
                    {
                        p_intrl->bitmask |= 
                            X2AP_SGNB_ADD_REQ_ACK_ERABS_NOT_ADMITTED_LIST_PRESENT;
                        p_intrl->erabs_not_admitted_list =
                            *p_protocol_IE->value.
                            u._x2ap_SgNBAdditionRequestAcknowledge_IEs_4;
                    }
                    break;
                }

                case ASN1V_x2ap_id_SgNBtoMeNBContainer:
                {
                    /* Fill SgNBtoMeNBContainer */
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(
                                &message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                p_protocol_IE->value.
                                u._x2ap_SgNBAdditionRequestAcknowledge_IEs_5))
                    {
                        x2ap_internal_decode_nr_cg_config(
                                p_protocol_IE->value.
                                u._x2ap_SgNBAdditionRequestAcknowledge_IEs_5,
                                &p_intrl->sgnb_to_menb_container,
                                p_asn1_ctx,
                                &cause);

                        if (X2AP_CAUSE_SUCCESS != cause.t)
                        {
                            /* Message parsing error */
                            X2AP_TRACE(X2AP_WARNING,
                                    "[SGNB ADDITION REQUEST ACKNOWLEDGE] Parsing error");
                            uecc_x2ap_update_message_map(&message_map,
                                    UECC_X2AP_INVALID_VALUE,
                                    index,
                                    ASN1V_x2ap_id_SgNBtoMeNBContainer);
                        }
                    }
                    break;
                }

                case ASN1V_x2ap_id_CriticalityDiagnostics:
                {
                    /* Fill CriticalityDiagnostics */
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(
                                &message_map,
                                index,
                                ASN1V_x2ap_id_CriticalityDiagnostics,
                                p_protocol_IE->value.
                                u._x2ap_SgNBAdditionRequestAcknowledge_IEs_6))
                    {
                        p_intrl->bitmask |= 
                            X2AP_SGNB_ADD_REQ_ACK_CRITICALITY_DIAGNOSTICS_PRESENT;
                        p_intrl->criticality_diagnostics =
                            *p_protocol_IE->value.
                            u._x2ap_SgNBAdditionRequestAcknowledge_IEs_6;
                    }
                    break;
                }

                case ASN1V_x2ap_id_MeNB_UE_X2AP_ID_Extension:
                {
                    /* Fill MeNB_UE_X2AP_ID_Extension */
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(
                                &message_map,
                                index,
                                ASN1V_x2ap_id_MeNB_UE_X2AP_ID_Extension,
                                &p_protocol_IE->value.
                                u._x2ap_SgNBAdditionRequestAcknowledge_IEs_7))
                    {
                        p_intrl->bitmask |= 
                            X2AP_SGNB_ADD_REQ_ACK_MENB_UE_X2AP_ID_EXT_PRESENT;
                        p_intrl->menb_ue_x2ap_id_ext =
                            p_protocol_IE->value.
                            u._x2ap_SgNBAdditionRequestAcknowledge_IEs_7;
                    }
                    break;
                }

                case ASN1V_x2ap_id_AdmittedSplitSRBs:
                {
                    /* Fill AdmittedSplitSRBs */
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(
                                &message_map,
                                index,
                                ASN1V_x2ap_id_AdmittedSplitSRBs,
                                &p_protocol_IE->value.
                                u._x2ap_SgNBAdditionRequestAcknowledge_IEs_8))
                    {
                        p_intrl->bitmask |= 
                            X2AP_SGNB_ADD_REQ_ACK_ADMITTED_SPLIT_SRBS_PRESENT;
                        p_intrl->admitted_split_srbs =
                            p_protocol_IE->value.
                            u._x2ap_SgNBAdditionRequestAcknowledge_IEs_8;
                    }
                    break;
                }

                case ASN1V_x2ap_id_SgNBResourceCoordinationInformation:
                {
                    /* Fill SgNBResourceCoordinationInformation */
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(
                                &message_map,
                                index,
                                ASN1V_x2ap_id_SgNBResourceCoordinationInformation,
                                p_protocol_IE->value.
                                u._x2ap_SgNBAdditionRequestAcknowledge_IEs_9))
                    {
                        p_intrl->bitmask |= 
                            X2AP_SGNB_ADD_REQ_ACK_SGNB_RESOURCE_COORDINATION_INFO_PRESENT;
                        p_intrl->sgnb_resource_coordination_info =
                            *p_protocol_IE->value.
                            u._x2ap_SgNBAdditionRequestAcknowledge_IEs_9;
                    }
                    break;
                }

                case ASN1V_x2ap_id_RRCConfigIndication:
                {
                    /* Fill RRCConfigIndication */
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(
                                &message_map,
                                index,
                                ASN1V_x2ap_id_RRCConfigIndication,
                                &p_protocol_IE->value.
                                u._x2ap_SgNBAdditionRequestAcknowledge_IEs_10))
                    {
                        p_intrl->bitmask |= 
                            X2AP_SGNB_ADD_REQ_ACK_RRC_CONFIG_INDICATION_PRESENT;
                        p_intrl->rrc_config_indication =
                            p_protocol_IE->value.
                            u._x2ap_SgNBAdditionRequestAcknowledge_IEs_10;
                    }
                    break;
                }

                default:
                X2AP_TRACE(X2AP_ERROR, "%s:"\
                        "Invalid Protocol IE id",__FUNCTION__);
                uecc_x2ap_add_to_err_ind_ie_list(&iE_list, 
                        p_protocol_IE->criticality,
                        p_protocol_IE->id,
                        &ie_list_index,
                        &send_err_ind,
                        X2AP_FALSE,
                        X2AP_FALSE);
                if (x2ap_reject == p_protocol_IE->criticality) 
                {
                    result = X2AP_FAILURE;
                }
            }
            p_node= p_node->next;
        }
    } while(RRC_NULL);

    if (X2AP_SUCCESS == uecc_x2ap_parse_message_map(
                p_asn1_ctx,
                peer_enodeb_id,
                &message_map,
                &iE_list,
                &ie_list_index,
                &send_err_ind,
                ASN1V_x2ap_id_sgNBAdditionPreparation,
                T_x2ap_X2AP_PDU_successfulOutcome,
                x2ap_reject,
                (rrc_x2ap_error_indication_t *)PNULL))
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED,"
                "SgNB Addition Request Acknowledge");
        result = X2AP_FAILURE;
    }

    X2AP_UT_TRACE_EXIT();

    return result;
}

/*****************************************************************************
 * Function Name  : x2ap_internal_decode_secondary_rat_data_usage_report 
 * Inputs         : p_asn1_ctx - pointer to OSCTXT
 *                  p_3gpp - pointer to x2ap_SNStatusTransfer
 *                  p_intrl - pointer to rrc_x2ap_sn_status_transfer_t
 *                  peer_enodeb_id - Peer EnodeB ID
 * Outputs        : p_intrl 
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode SECONDARY RAT DATA USAGE REPORT
 ********************************************************************************/
x2ap_return_et x2ap_internal_decode_secondary_rat_data_usage_report
(
 OSCTXT      *p_asn1_ctx,
 x2ap_SecondaryRATDataUsageReport   *p_3gpp,    /* unpacked src */
 rrc_x2ap_secondary_rat_data_usage_report_t    *p_intrl,    /* dest */
 U8    peer_enodeb_id /* Peer EnodeB ID */
)
{
    U32  index = X2AP_NULL;
    OSRTDListNode  *p_node     = PNULL;
    x2ap_SecondaryRATDataUsageReport_protocolIEs_element *p_protocol_IE = PNULL;
    x2ap_return_et       retVal = X2AP_SUCCESS;
    uecc_x2ap_error_ind_bool_t send_err_ind;
    uecc_x2ap_error_ind_ie_list_t   iE_list;
    U16 ie_list_index = 0;

    uecc_x2ap_message_data_t message_map =
    {4, X2AP_FALSE,X2AP_FALSE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_MeNB_UE_X2AP_ID, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {1, ASN1V_x2ap_id_SgNB_UE_X2AP_ID, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {2, ASN1V_x2ap_id_SecondaryRATUsageReportList, x2ap_mandatory, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {3, ASN1V_x2ap_id_MeNB_UE_X2AP_ID_Extension, x2ap_optional, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL}
        }};

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != p_3gpp);
    X2AP_ASSERT(PNULL != p_intrl);
    X2AP_MEMSET(&iE_list, 0, sizeof(uecc_x2ap_error_ind_ie_list_t));
    X2AP_MEMSET(&send_err_ind, 0, sizeof(uecc_x2ap_error_ind_bool_t));

    do
    {
        p_node = p_3gpp->protocolIEs.head;
        /* Save the decoded Protocol IEs */
        for (index =0; index < p_3gpp->protocolIEs.count; index++)
        {
            if (!p_node)
            {
                X2AP_TRACE(X2AP_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }
            p_protocol_IE =
                (x2ap_SecondaryRATDataUsageReport_protocolIEs_element *)
                p_node->data;
            switch (p_protocol_IE->id)
            {
                case ASN1V_x2ap_id_MeNB_UE_X2AP_ID:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(&message_map,
                                index,
                                p_protocol_IE->id,
                                &p_protocol_IE->value.u._x2ap_SecondaryRATDataUsageReport_IEs_1))
                    {
                        p_intrl->menb_ue_x2ap_id =
                            p_protocol_IE->value.u._x2ap_SecondaryRATDataUsageReport_IEs_1;
                    }
                }
                break;

                case ASN1V_x2ap_id_SgNB_UE_X2AP_ID:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(&message_map,
                                index,
                                p_protocol_IE->id,
                                &p_protocol_IE->value.u._x2ap_SecondaryRATDataUsageReport_IEs_2))
                    {
                        p_intrl->sgnb_ue_x2ap_id =
                            p_protocol_IE->value.u._x2ap_SecondaryRATDataUsageReport_IEs_2;
                    }
                }
                break;

                case ASN1V_x2ap_id_SecondaryRATUsageReportList:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(&message_map,
                                index,
                                p_protocol_IE->id,
                                p_protocol_IE->value.u._x2ap_SecondaryRATDataUsageReport_IEs_3))
                    {
                        p_intrl->secondary_rat_usage_rep_list =
                            *p_protocol_IE->value.u._x2ap_SecondaryRATDataUsageReport_IEs_3;
                    }
                }
                break;

                case ASN1V_x2ap_id_MeNB_UE_X2AP_ID_Extension:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(&message_map,
                                index,
                                p_protocol_IE->id,
                                &p_protocol_IE->value.u._x2ap_SecondaryRATDataUsageReport_IEs_4))
                    {
                        p_intrl->menb_ue_x2ap_id_ext =
                            p_protocol_IE->value.u._x2ap_SecondaryRATDataUsageReport_IEs_4;
                        p_intrl->bitmask |= X2AP_SECONDARY_RAT_DATA_USAGE_REP_MENB_UE_X2AP_ID_EXT_PRESENT;
                    }
                }
                break;

                default:
                X2AP_TRACE(X2AP_ERROR, "%s:"\
                        "invalid Protocol IE id",__FUNCTION__);
                uecc_x2ap_add_to_err_ind_ie_list(&iE_list, 
                        p_protocol_IE->criticality,
                        p_protocol_IE->id,
                        &ie_list_index,
                        &send_err_ind,
                        X2AP_FALSE,
                        X2AP_FALSE);

                if (x2ap_reject == p_protocol_IE->criticality) 
                {
                    retVal = X2AP_FAILURE;
                }
            }
            p_node= p_node->next;
        }
    }while(0);
    if (X2AP_SUCCESS == uecc_x2ap_parse_message_map(
                p_asn1_ctx,
                peer_enodeb_id,
                &message_map,
                &iE_list,
                &ie_list_index,
                &send_err_ind,
                ASN1V_x2ap_id_secondaryRATDataUsageReport,
                T_x2ap_X2AP_PDU_initiatingMessage,
                x2ap_ignore,
                (rrc_x2ap_error_indication_t *)PNULL))
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED,"
                " SecondaryRatDataUsageReport");
        retVal = X2AP_FAILURE;
    }

    X2AP_UT_TRACE_EXIT();

    return retVal;

}
/*NR_DC Code Change Stop*/
#endif


/*****************************************************************************
 * Function Name  : x2ap_internal_decode_handover_request 
 * Inputs         : p_asn1_ctx -  Pointer to ASN Context
 *                  p_3gpp - pointer to x2ap_HandoverRequest
 *                  p_intrl - pointer to rrc_x2ap_handover_request_t
 *                  p_error_ind_content - pointer to rrc_x2ap_error_indication_t
 *                  p_send_err_ind - pointer to rrc_x2ap_error_indication_t
 * Outputs        : p_intrl 
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode handover request
 ********************************************************************************/
x2ap_return_et x2ap_internal_decode_handover_request
(
 OSCTXT      *p_asn1_ctx,   /* Pointer to ASN Context */
 x2ap_HandoverRequest        *p_3gpp,    /* unpacked src */
 rrc_x2ap_handover_request_t *p_intrl,    /* dest */
 U8                  peer_enodeb_id,      /* Peer EnodeB ID */
 rrc_x2ap_error_indication_t *p_error_ind_content,
 uecc_x2ap_error_ind_bool_t *p_send_err_ind
)
{

    U32  index = X2AP_NULL;
    OSRTDListNode  *p_node     = PNULL;
    x2ap_Cause     cause;
    x2ap_HandoverRequest_protocolIEs_element *p_protocol_IE = PNULL;
    x2ap_return_et result = X2AP_SUCCESS;
    uecc_x2ap_error_ind_ie_list_t   iE_list;
    U16 ie_list_index = 0;

    uecc_x2ap_message_data_t message_map =
    {9, X2AP_TRUE,X2AP_TRUE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_Old_eNB_UE_X2AP_ID, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {1, ASN1V_x2ap_id_Cause, x2ap_mandatory, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {2, ASN1V_x2ap_id_TargetCell_ID, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {3, ASN1V_x2ap_id_GUMMEI_ID, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {4, ASN1V_x2ap_id_UE_ContextInformation, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {5, ASN1V_x2ap_id_UE_HistoryInformation, x2ap_mandatory, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {6, ASN1V_x2ap_id_TraceActivation, x2ap_optional, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {7, ASN1V_x2ap_id_SRVCCOperationPossible, x2ap_optional, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {8, ASN1V_x2ap_id_CSGMembershipStatus, x2ap_optional, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL}
        }};
    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != p_3gpp);
    X2AP_ASSERT(PNULL != p_intrl);

    X2AP_MEMSET(&iE_list, 0, sizeof(uecc_x2ap_error_ind_ie_list_t));
    X2AP_MEMSET(&cause, 0, sizeof(x2ap_Cause));

    p_intrl->bitmask = 0;
    do
    {
        p_node = p_3gpp->protocolIEs.head;
        /* Save the decoded Protocol IEs */
        for (index =0; index < p_3gpp->protocolIEs.count; index++)
        {
            if (!p_node)
            {
                X2AP_TRACE(X2AP_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }
            p_protocol_IE =
                (x2ap_HandoverRequest_protocolIEs_element *)
                p_node->data;
            switch (p_protocol_IE->id)
            {
                case ASN1V_x2ap_id_Old_eNB_UE_X2AP_ID:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(
                                &message_map,
                                index,
                                ASN1V_x2ap_id_Old_eNB_UE_X2AP_ID,
                                &p_protocol_IE->value.u._x2ap_HandoverRequest_IEs_1))
                    {
                        p_intrl->x2ap_Old_UE_X2AP_ID =
                            p_protocol_IE->value.u._x2ap_HandoverRequest_IEs_1;
                    }
                }
                break;

                case ASN1V_x2ap_id_Cause:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(
                                &message_map,
                                index,
                                ASN1V_x2ap_id_Cause,
                                p_protocol_IE->value.u._x2ap_HandoverRequest_IEs_2))
                    {
                        p_intrl->cause =
                            *p_protocol_IE->value.u._x2ap_HandoverRequest_IEs_2;
                    }
                }
                break;

                case ASN1V_x2ap_id_TargetCell_ID:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(
                                &message_map,
                                index,
                                ASN1V_x2ap_id_TargetCell_ID,
                                p_protocol_IE->value.u._x2ap_HandoverRequest_IEs_3))
                    {
                        p_intrl->target_cell_id = 
                            *p_protocol_IE->value.u._x2ap_HandoverRequest_IEs_3;
                    }
                }
                break;

                case ASN1V_x2ap_id_GUMMEI_ID:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(
                                &message_map,
                                index,
                                ASN1V_x2ap_id_GUMMEI_ID,
                                p_protocol_IE->value.u._x2ap_HandoverRequest_IEs_4))
                    {
                        p_intrl->gummei = 
                            *p_protocol_IE->value.u._x2ap_HandoverRequest_IEs_4;
                    }
                }
                break;

                case ASN1V_x2ap_id_UE_ContextInformation:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(
                                &message_map,
                                index,
                                ASN1V_x2ap_id_UE_ContextInformation,
                                p_protocol_IE->value.u._x2ap_HandoverRequest_IEs_5))
                    {
                        asn1Copy_x2ap_UE_ContextInformation(p_asn1_ctx,
                                p_protocol_IE->value.u._x2ap_HandoverRequest_IEs_5,
                                &(p_intrl->rrc_x2ap_ue_ctx_info.ue_context_info)); 

                        rrc_x2ap_internal_decode_handover_preparation_information(
                                /*SPR_18125_START*/
                                p_asn1_ctx,
                                /*SPR_18125_END*/
                                &p_intrl->rrc_x2ap_ue_ctx_info.ue_context_info.rRC_Context,
                                &p_intrl->rrc_x2ap_ue_ctx_info.rRC_Container,
                                &cause);
                        /*valgrind_fix_start*/
                        // p_asn1_ctx);
                        /*valgrind_fix_stop*/
                    }
                    if (X2AP_CAUSE_SUCCESS != cause.t)
                    {
                        /* Message parsing error */
                        X2AP_TRACE(X2AP_WARNING,
                                "[X2AP Handover Request ] Parsing error");
                        uecc_x2ap_update_message_map(&message_map,
                                UECC_X2AP_INVALID_VALUE,
                                index,
                                ASN1V_x2ap_id_UE_ContextInformation);
                        /* Drop. */
                    }
                }
                break;
                case ASN1V_x2ap_id_UE_HistoryInformation:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(
                                &message_map,
                                index,
                                ASN1V_x2ap_id_UE_HistoryInformation,
                                p_protocol_IE->value.u._x2ap_HandoverRequest_IEs_6))
                    {
                        p_intrl->ue_history_info = 
                            *p_protocol_IE->value.u._x2ap_HandoverRequest_IEs_6;
                    }
                }
                break;
                case ASN1V_x2ap_id_TraceActivation:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(
                                &message_map,
                                index,
                                ASN1V_x2ap_id_TraceActivation,
                                p_protocol_IE->value.u._x2ap_HandoverRequest_IEs_7))
                    {
                        p_intrl->trace_activation = 
                            *p_protocol_IE->value.u._x2ap_HandoverRequest_IEs_7;
                        p_intrl->bitmask |=
                            X2AP_HANDOVER_REQ_TRACE_ACTIVATION_PRESENT;
                    }
                }
                break;
                case ASN1V_x2ap_id_SRVCCOperationPossible:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(
                                &message_map,
                                index,
                                ASN1V_x2ap_id_SRVCCOperationPossible,
                                &p_protocol_IE->value.u._x2ap_HandoverRequest_IEs_8))
                    {
                        p_intrl->srv_operation_possible = 
                            p_protocol_IE->value.u._x2ap_HandoverRequest_IEs_8;
                        p_intrl->bitmask |=
                            X2AP_HANDOVER_REQ_SRVCC_OPERATION_POSSIBLE_PRESENT;
                    }

                }
                break;
                /*36423_CR0407_start*/                               
                case ASN1V_x2ap_id_CSGMembershipStatus: 
                {         
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(
                                &message_map,
                                index,
                                ASN1V_x2ap_id_CSGMembershipStatus,
                                &p_protocol_IE->value.u._x2ap_HandoverRequest_IEs_9))
                    {
                        p_intrl->csg_membership_status = 
                            p_protocol_IE->value.u._x2ap_HandoverRequest_IEs_9;
                        p_intrl->bitmask |=
                            X2AP_HANDOVER_REQ_CSG_MEMBERSHIP_STATUS_PRESENT;
                    }
                }
                /*36423_CR0407_stop*/
                break;
                default:
                X2AP_TRACE(X2AP_ERROR, "%s:"\
                        "invalid Protocol IE id",__FUNCTION__);
                uecc_x2ap_add_to_err_ind_ie_list(&iE_list, 
                        p_protocol_IE->criticality,
                        p_protocol_IE->id,
                        &ie_list_index,
                        p_send_err_ind,
                        X2AP_FALSE,
                        X2AP_FALSE);
                /*SPR_19067_FIX_START*/
                if (x2ap_reject == p_protocol_IE->criticality) 
                {
                    result = X2AP_FAILURE;
                }
                /*SPR_19067_FIX_STOP*/
            }
            p_node= p_node->next;
        }
    }while(0);
    if (X2AP_SUCCESS == uecc_x2ap_parse_message_map(
                p_asn1_ctx,
                peer_enodeb_id,
                &message_map,
                &iE_list,
                &ie_list_index,
                p_send_err_ind,
                ASN1V_x2ap_id_handoverPreparation,
                T_x2ap_X2AP_PDU_initiatingMessage,
                x2ap_reject,
                p_error_ind_content))
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED,"
                "Handover Request");
        result = X2AP_FAILURE;
    }


    X2AP_UT_TRACE_EXIT();

    return result;

}

/*****************************************************************************
 * Function Name  : x2ap_internal_decode_handover_request_ack 
 * Inputs         : p_asn1_ctx - pointer to OSCTXT
 *                  p_ho_cmd_ctxt - pointer to OSCTXT
 *                  p_3gpp - pointer to x2ap_HandoverRequestAcknowledge
 *                  p_intrl - pointer to rrc_x2ap_handover_req_ack_t
 *                  peer_enodeb_id - Peer EnodeB ID
 * Outputs        : p_intrl
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode handover request
 ********************************************************************************/
x2ap_return_et x2ap_internal_decode_handover_request_ack
(
 OSCTXT      *p_asn1_ctx,
 OSCTXT  *p_ho_cmd_ctxt,                /* for memory allocation */
 x2ap_HandoverRequestAcknowledge     *p_3gpp,    /* unpacked src */
 rrc_x2ap_handover_req_ack_t         *p_intrl,    /* dest */
 U8  peer_enodeb_id /* Peer EnodeB ID */
)
{

    U32  index = X2AP_NULL;
    OSRTDListNode  *p_node     = PNULL;
    x2ap_Cause     cause;
    x2ap_TargeteNBtoSource_eNBTransparentContainer* p_container;
    x2ap_HandoverRequestAcknowledge_protocolIEs_element *p_protocol_IE = PNULL;
    x2ap_return_et       retVal = X2AP_SUCCESS;
    uecc_x2ap_error_ind_bool_t send_err_ind;
    uecc_x2ap_error_ind_ie_list_t   iE_list;
    U16 ie_list_index = 0;

    uecc_x2ap_message_data_t message_map =
    {6, X2AP_FALSE,X2AP_FALSE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_Old_eNB_UE_X2AP_ID, x2ap_mandatory, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {1, ASN1V_x2ap_id_New_eNB_UE_X2AP_ID, x2ap_mandatory, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {2, ASN1V_x2ap_id_E_RABs_Admitted_List, x2ap_mandatory, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {3, ASN1V_x2ap_id_E_RABs_NotAdmitted_List, x2ap_optional, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {4, ASN1V_x2ap_id_TargeteNBtoSource_eNBTransparentContainer,
                x2ap_mandatory, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {5, ASN1V_x2ap_id_CriticalityDiagnostics, x2ap_optional, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL}
        }};

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != p_3gpp);
    X2AP_ASSERT(PNULL != p_intrl);
    X2AP_ASSERT(PNULL != p_ho_cmd_ctxt);
    X2AP_MEMSET(&iE_list, 0, sizeof(uecc_x2ap_error_ind_ie_list_t));
    X2AP_MEMSET(&send_err_ind, 0, sizeof(uecc_x2ap_error_ind_bool_t));
    X2AP_MEMSET(&cause, 0, sizeof(x2ap_Cause));

    p_intrl->bitmask = 0;
    do
    {
        p_node = p_3gpp->protocolIEs.head;
        /* Save the decoded Protocol IEs */
        for (index =0; index < p_3gpp->protocolIEs.count; index++)
        {
            if (!p_node)
            {
                X2AP_TRACE(X2AP_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }
            p_protocol_IE =
                (x2ap_HandoverRequestAcknowledge_protocolIEs_element *)
                p_node->data;
            switch (p_protocol_IE->id)
            {
                case ASN1V_x2ap_id_Old_eNB_UE_X2AP_ID:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(&message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                &p_protocol_IE->value.u._x2ap_HandoverRequestAcknowledge_IEs_1))
                    {
                        p_intrl->old_ue_x2ap_id =
                            p_protocol_IE->value.u._x2ap_HandoverRequestAcknowledge_IEs_1;
                    }
                }
                break;

                case ASN1V_x2ap_id_New_eNB_UE_X2AP_ID:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(&message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                &p_protocol_IE->value.u._x2ap_HandoverRequestAcknowledge_IEs_2))
                    {
                        p_intrl->new_ue_x2ap_id =
                            p_protocol_IE->value.u._x2ap_HandoverRequestAcknowledge_IEs_2;
                    }
                }
                break;

                case ASN1V_x2ap_id_E_RABs_Admitted_List:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(&message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                p_protocol_IE->value.u._x2ap_HandoverRequestAcknowledge_IEs_3))
                    {
                        p_intrl->e_rab_admitted_list = 
                            *p_protocol_IE->value.u._x2ap_HandoverRequestAcknowledge_IEs_3;
                    }
                }
                break;

                case ASN1V_x2ap_id_E_RABs_NotAdmitted_List:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(&message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                p_protocol_IE->value.u._x2ap_HandoverRequestAcknowledge_IEs_4))
                    {
                        p_intrl->e_rab_not_admitted_list = 
                            *p_protocol_IE->value.u._x2ap_HandoverRequestAcknowledge_IEs_4;
                        p_intrl->bitmask |=
                            X2AP_HANDOVER_REQ_ACK_ERAB_NOT_ADMITTED_LIST_PRESENT;
                    }
                }
                break;

                case ASN1V_x2ap_id_TargeteNBtoSource_eNBTransparentContainer:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(&message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                p_protocol_IE->value.u._x2ap_HandoverRequestAcknowledge_IEs_5))
                    {
                        p_container = &p_intrl->target_to_src_enb_transparent_cont.
                            target_to_source_transparent_container;
                        asn1Copy_x2ap_TargeteNBtoSource_eNBTransparentContainer(
                                p_asn1_ctx,
                                p_protocol_IE->value.u._x2ap_HandoverRequestAcknowledge_IEs_5,
                                p_container);

                        rrc_x2ap_internal_decode_handover_command(
                                p_container,
                                p_intrl,
                                p_ho_cmd_ctxt,
                                &cause);
                    }
                    if (X2AP_CAUSE_SUCCESS != cause.t)
                    {
                        /* Message parsing error */
                        X2AP_TRACE(X2AP_WARNING,
                                "[X2AP Handover Request Acknowledge] Parsing error");
                        uecc_x2ap_update_message_map(&message_map,
                                UECC_X2AP_INVALID_VALUE,
                                index,
                                ASN1V_x2ap_id_TargeteNBtoSource_eNBTransparentContainer);
                        /* Drop. */
                    }
                }
                break;
                case ASN1V_x2ap_id_CriticalityDiagnostics:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(&message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                p_protocol_IE->value.u._x2ap_HandoverRequestAcknowledge_IEs_6))
                    {
                        p_intrl->criticality_diagnostics = 
                            *p_protocol_IE->value.u._x2ap_HandoverRequestAcknowledge_IEs_6;
                        p_intrl->bitmask |=
                            X2AP_HANDOVER_REQ_ACK_CRIT_DIAGNO_PRESENT;
                    }
                }    
                break;

                default:
                X2AP_TRACE(X2AP_ERROR, "%s:"\
                        "invalid Protocol IE id",__FUNCTION__);
                uecc_x2ap_add_to_err_ind_ie_list(&iE_list, 
                        p_protocol_IE->criticality,
                        p_protocol_IE->id,
                        &ie_list_index,
                        &send_err_ind,
                        X2AP_FALSE,
                        X2AP_FALSE);
                /*SPR_19067_FIX_START*/
                if (x2ap_reject == p_protocol_IE->criticality) 
                {
                    retVal = X2AP_FAILURE;
                }
                /*SPR_19067_FIX_STOP*/
            }

            p_node= p_node->next;
        }
    }while(0);
    if (X2AP_SUCCESS == uecc_x2ap_parse_message_map(
                p_asn1_ctx,
                peer_enodeb_id,
                &message_map,
                &iE_list,
                &ie_list_index,
                &send_err_ind,
                ASN1V_x2ap_id_handoverPreparation,
                T_x2ap_X2AP_PDU_successfulOutcome,
                x2ap_reject,
                (rrc_x2ap_error_indication_t *)PNULL))
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED,"
                " Handover Request Acknowledge");
        retVal = X2AP_FAILURE;
    }

    X2AP_UT_TRACE_EXIT();

    return retVal;

}

/*****************************************************************************
 * Function Name  : x2ap_internal_decode_handover_prep_fail 
 * Inputs         : p_asn1_ctx - pointer to OSCTXT
 *                  p_3gpp - pointer to x2ap_HandoverPreparationFailure
 *                  p_intrl - pointer to rrc_x2ap_handover_prep_fail_t
 *                  peer_enodeb_id - Peer EnodeB ID
 * Outputs        : p_intrl
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode handover preparation failure
 ********************************************************************************/
x2ap_return_et x2ap_internal_decode_handover_prep_fail
(
 OSCTXT      *p_asn1_ctx,
 x2ap_HandoverPreparationFailure     *p_3gpp,    /* unpacked src */
 rrc_x2ap_handover_prep_fail_t    *p_intrl,    /* dest */
 U8  peer_enodeb_id /* Peer EnodeB ID */
)
{

    U32  index = X2AP_NULL;
    OSRTDListNode  *p_node     = PNULL;
    x2ap_HandoverPreparationFailure_protocolIEs_element *p_protocol_IE = PNULL;
    x2ap_return_et       retVal = X2AP_SUCCESS;
    uecc_x2ap_error_ind_bool_t send_err_ind;
    uecc_x2ap_error_ind_ie_list_t   iE_list;
    U16 ie_list_index = 0;

    uecc_x2ap_message_data_t message_map =
    {3, X2AP_FALSE,X2AP_FALSE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_Old_eNB_UE_X2AP_ID, x2ap_mandatory, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {1, ASN1V_x2ap_id_Cause, x2ap_mandatory, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {2, ASN1V_x2ap_id_CriticalityDiagnostics, x2ap_optional, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL}
        }};

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != p_3gpp);
    X2AP_ASSERT(PNULL != p_intrl);
    X2AP_MEMSET(&iE_list, 0, sizeof(uecc_x2ap_error_ind_ie_list_t));
    X2AP_MEMSET(&send_err_ind, 0, sizeof(uecc_x2ap_error_ind_bool_t));

    p_intrl->bitmask = 0;
    do
    {
        p_node = p_3gpp->protocolIEs.head;
        /* Save the decoded Protocol IEs */
        for (index =0; index < p_3gpp->protocolIEs.count; index++)
        {
            if (!p_node)
            {
                X2AP_TRACE(X2AP_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }
            p_protocol_IE =
                (x2ap_HandoverPreparationFailure_protocolIEs_element *)
                p_node->data;
            switch (p_protocol_IE->id)
            {
                case ASN1V_x2ap_id_Old_eNB_UE_X2AP_ID:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(&message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                &p_protocol_IE->value.u._x2ap_HandoverPreparationFailure_IEs_1))
                    {
                        p_intrl->x2ap_Old_UE_X2AP_ID =
                            p_protocol_IE->value.u._x2ap_HandoverPreparationFailure_IEs_1;
                    }
                }
                break;

                case ASN1V_x2ap_id_Cause:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(&message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                p_protocol_IE->value.u._x2ap_HandoverPreparationFailure_IEs_2))
                    {
                        p_intrl->cause =
                            *p_protocol_IE->value.u._x2ap_HandoverPreparationFailure_IEs_2;
                    }
                }
                break;

                case ASN1V_x2ap_id_CriticalityDiagnostics:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(&message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                p_protocol_IE->value.u._x2ap_HandoverPreparationFailure_IEs_3))
                    {
                        p_intrl->criticality_diagnostics = 
                            *p_protocol_IE->value.u._x2ap_HandoverPreparationFailure_IEs_3;
                        p_intrl->bitmask |=
                            X2AP_HANDOVER_PREP_FAIL_CRIT_DIAGNO_PRESENT;
                    }
                }
                break;

                default:
                X2AP_TRACE(X2AP_ERROR, "%s:"\
                        "invalid Protocol IE id",__FUNCTION__);
                uecc_x2ap_add_to_err_ind_ie_list(&iE_list, 
                        p_protocol_IE->criticality,
                        p_protocol_IE->id,
                        &ie_list_index,
                        &send_err_ind,
                        X2AP_FALSE,
                        X2AP_FALSE);
                /*SPR_19067_FIX_START*/
                if (x2ap_reject == p_protocol_IE->criticality) 
                {
                    retVal = X2AP_FAILURE;
                }
                /*SPR_19067_FIX_STOP*/
            }
            p_node= p_node->next;
        }
    }while(0);
    if (X2AP_SUCCESS == uecc_x2ap_parse_message_map(
                p_asn1_ctx,
                peer_enodeb_id,
                &message_map,
                &iE_list,
                &ie_list_index,
                &send_err_ind,
                ASN1V_x2ap_id_handoverPreparation,
                T_x2ap_X2AP_PDU_unsuccessfulOutcome,
                x2ap_reject,
                (rrc_x2ap_error_indication_t *)PNULL))
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED,"
                " Handover Prep Failure");
        retVal = X2AP_FAILURE;
    }

    X2AP_UT_TRACE_EXIT();

    return retVal;

}

/*****************************************************************************
 * Function Name  : x2ap_internal_decode_sn_status_transfer 
 * Inputs         : p_asn1_ctx - pointer to OSCTXT
 *                  p_3gpp - pointer to x2ap_SNStatusTransfer
 *                  p_intrl - pointer to rrc_x2ap_sn_status_transfer_t
 *                  peer_enodeb_id - Peer EnodeB ID
 * Outputs        : p_intrl 
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode SN Status Transfer
 ********************************************************************************/
x2ap_return_et x2ap_internal_decode_sn_status_transfer
(
 OSCTXT      *p_asn1_ctx,
 x2ap_SNStatusTransfer            *p_3gpp,    /* unpacked src */
 rrc_x2ap_sn_status_transfer_t    *p_intrl,    /* dest */
 U8    peer_enodeb_id /* Peer EnodeB ID */
)
{

    U32  index = X2AP_NULL;
    OSRTDListNode  *p_node     = PNULL;
    x2ap_SNStatusTransfer_protocolIEs_element *p_protocol_IE = PNULL;
    x2ap_return_et       retVal = X2AP_SUCCESS;
    uecc_x2ap_error_ind_bool_t send_err_ind;
    uecc_x2ap_error_ind_ie_list_t   iE_list;
    U16 ie_list_index = 0;

    uecc_x2ap_message_data_t message_map =
    {3, X2AP_FALSE,X2AP_FALSE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_Old_eNB_UE_X2AP_ID, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {1, ASN1V_x2ap_id_New_eNB_UE_X2AP_ID, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {2, ASN1V_x2ap_id_E_RABs_SubjectToStatusTransfer_List, x2ap_mandatory, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL}
        }};

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != p_3gpp);
    X2AP_ASSERT(PNULL != p_intrl);
    X2AP_MEMSET(&iE_list, 0, sizeof(uecc_x2ap_error_ind_ie_list_t));
    X2AP_MEMSET(&send_err_ind, 0, sizeof(uecc_x2ap_error_ind_bool_t));

    do
    {
        p_node = p_3gpp->protocolIEs.head;
        /* Save the decoded Protocol IEs */
        for (index =0; index < p_3gpp->protocolIEs.count; index++)
        {
            if (!p_node)
            {
                X2AP_TRACE(X2AP_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }
            p_protocol_IE =
                (x2ap_SNStatusTransfer_protocolIEs_element *)
                p_node->data;
            switch (p_protocol_IE->id)
            {
                case ASN1V_x2ap_id_Old_eNB_UE_X2AP_ID:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(&message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                &p_protocol_IE->value.u._x2ap_SNStatusTransfer_IEs_1))
                    {
                        p_intrl->x2ap_Old_UE_X2AP_ID =
                            p_protocol_IE->value.u._x2ap_SNStatusTransfer_IEs_1;
                    }
                }
                break;

                case ASN1V_x2ap_id_New_eNB_UE_X2AP_ID:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(&message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                &p_protocol_IE->value.u._x2ap_SNStatusTransfer_IEs_2))
                    {
                        p_intrl->x2ap_New_UE_X2AP_ID =
                            p_protocol_IE->value.u._x2ap_SNStatusTransfer_IEs_2;
                    }
                }
                break;

                case ASN1V_x2ap_id_E_RABs_SubjectToStatusTransfer_List:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(&message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                p_protocol_IE->value.u._x2ap_SNStatusTransfer_IEs_3))
                    {
                        p_intrl->erab_subject_to_status_transfer_list =
                            *p_protocol_IE->value.u._x2ap_SNStatusTransfer_IEs_3;
                    }
                }
                break;

                default:
                X2AP_TRACE(X2AP_ERROR, "%s:"\
                        "invalid Protocol IE id",__FUNCTION__);
                uecc_x2ap_add_to_err_ind_ie_list(&iE_list, 
                        p_protocol_IE->criticality,
                        p_protocol_IE->id,
                        &ie_list_index,
                        &send_err_ind,
                        X2AP_FALSE,
                        X2AP_FALSE);
                /*SPR_19067_FIX_START*/
                if (x2ap_reject == p_protocol_IE->criticality) 
                {
                    retVal = X2AP_FAILURE;
                }
                /*SPR_19067_FIX_STOP*/
            }
            p_node= p_node->next;
        }
    }while(0);
    if (X2AP_SUCCESS == uecc_x2ap_parse_message_map(
                p_asn1_ctx,
                peer_enodeb_id,
                &message_map,
                &iE_list,
                &ie_list_index,
                &send_err_ind,
                ASN1V_x2ap_id_snStatusTransfer,
                T_x2ap_X2AP_PDU_initiatingMessage,
                x2ap_ignore,
                (rrc_x2ap_error_indication_t *)PNULL))
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED,"
                " SNStatusTransfer");
        retVal = X2AP_FAILURE;
    }

    X2AP_UT_TRACE_EXIT();

    return retVal;

}

/*****************************************************************************
 * Function Name  : x2ap_internal_decode_ue_context_release 
 * Inputs         : p_asn1_ctx - pointer to OSCTXT
 *                  p_3gpp - pointer to x2ap_UEContextRelease
 *                  p_intrl - pointer to rrc_x2ap_ue_context_release_t
 *                  peer_enodeb_id -  Peer EnodeB ID 
 * Outputs        : p_intrl
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode ue context release
 ********************************************************************************/
x2ap_return_et x2ap_internal_decode_ue_context_release
(
 OSCTXT      *p_asn1_ctx,
 x2ap_UEContextRelease            *p_3gpp,     /* unpacked src */
 rrc_x2ap_ue_context_release_t    *p_intrl,    /* dest */
 U8  peer_enodeb_id /* Peer EnodeB ID */
)
{

    U32  index = X2AP_NULL;
    OSRTDListNode  *p_node     = PNULL;
    x2ap_UEContextRelease_protocolIEs_element *p_protocol_IE = PNULL;
    x2ap_return_et       retVal = X2AP_SUCCESS;
    uecc_x2ap_error_ind_bool_t send_err_ind;
    uecc_x2ap_error_ind_ie_list_t   iE_list;
    U16 ie_list_index = 0;

    uecc_x2ap_message_data_t message_map =
    {2, X2AP_FALSE,X2AP_FALSE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_Old_eNB_UE_X2AP_ID, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {1, ASN1V_x2ap_id_New_eNB_UE_X2AP_ID, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL}
        }};

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != p_3gpp);
    X2AP_ASSERT(PNULL != p_intrl);
    X2AP_MEMSET(&iE_list, 0, sizeof(uecc_x2ap_error_ind_ie_list_t));
    X2AP_MEMSET(&send_err_ind, 0, sizeof(uecc_x2ap_error_ind_bool_t));

    do
    {
        p_node = p_3gpp->protocolIEs.head;
        /* Save the decoded Protocol IEs */
        for (index =0; index < p_3gpp->protocolIEs.count; index++)
        {
            if (!p_node)
            {
                X2AP_TRACE(X2AP_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }
            p_protocol_IE =
                (x2ap_UEContextRelease_protocolIEs_element *)
                p_node->data;
            switch (p_protocol_IE->id)
            {
                case ASN1V_x2ap_id_Old_eNB_UE_X2AP_ID:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(&message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                &p_protocol_IE->value.u._x2ap_UEContextRelease_IEs_1))
                    {
                        p_intrl->x2ap_Old_UE_X2AP_ID =
                            p_protocol_IE->value.u._x2ap_UEContextRelease_IEs_1;
                    }
                }
                break;

                case ASN1V_x2ap_id_New_eNB_UE_X2AP_ID:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(&message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                &p_protocol_IE->value.u._x2ap_UEContextRelease_IEs_2))
                    {
                        p_intrl->x2ap_New_UE_X2AP_ID =
                            p_protocol_IE->value.u._x2ap_UEContextRelease_IEs_2;
                    }
                }
                break;

                default:
                X2AP_TRACE(X2AP_ERROR, "%s:"\
                        "invalid Protocol IE id",__FUNCTION__);
                uecc_x2ap_add_to_err_ind_ie_list(&iE_list, 
                        p_protocol_IE->criticality,
                        p_protocol_IE->id,
                        &ie_list_index,
                        &send_err_ind,
                        X2AP_FALSE,
                        X2AP_FALSE);
                /*SPR_19067_FIX_START*/
                if (x2ap_reject == p_protocol_IE->criticality) 
                {
                    retVal = X2AP_FAILURE;
                }
                /*SPR_19067_FIX_STOP*/
            }
            p_node= p_node->next;
        }
    }while(0);
    if (X2AP_SUCCESS == uecc_x2ap_parse_message_map(
                p_asn1_ctx,
                peer_enodeb_id,
                &message_map,
                &iE_list,
                &ie_list_index,
                &send_err_ind,
                ASN1V_x2ap_id_uEContextRelease,
                T_x2ap_X2AP_PDU_initiatingMessage,
                x2ap_ignore,
                (rrc_x2ap_error_indication_t *)PNULL))
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED,"
                " UEContextRelease");
        retVal = X2AP_FAILURE;
    }

    X2AP_UT_TRACE_EXIT();

    return retVal;

}

/*****************************************************************************
 * Function Name  : x2ap_internal_decode_handover_cancel 
 * Inputs         : p_asn1_ctx - pointer to OSCTXT
 *                  p_3gpp - pointer to x2ap_HandoverCancel
 *                  p_intrl - ponter to rrc_x2ap_handover_cancel_t
 *                  peer_enodeb_id - eNB id
 * Outputs        : p_intrl
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode handover cancel.
 ********************************************************************************/
x2ap_return_et x2ap_internal_decode_handover_cancel     
(
 OSCTXT      *p_asn1_ctx,
 x2ap_HandoverCancel           *p_3gpp,    /* unpacked src */
 rrc_x2ap_handover_cancel_t    *p_intrl,    /* dest */
 U8    peer_enodeb_id /* Peer EnodeB ID */
)
{

    U32  index = X2AP_NULL;
    OSRTDListNode  *p_node     = PNULL;
    x2ap_HandoverCancel_protocolIEs_element *p_protocol_IE = PNULL;
    x2ap_return_et       retVal = X2AP_SUCCESS;
    uecc_x2ap_error_ind_bool_t send_err_ind;
    uecc_x2ap_error_ind_ie_list_t   iE_list;
    U16 ie_list_index = 0;

    uecc_x2ap_message_data_t message_map =
    {3, X2AP_FALSE,X2AP_FALSE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_Old_eNB_UE_X2AP_ID, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {1, ASN1V_x2ap_id_New_eNB_UE_X2AP_ID, x2ap_optional, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {2, ASN1V_x2ap_id_Cause, x2ap_mandatory, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL}
        }};

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != p_3gpp);
    X2AP_ASSERT(PNULL != p_intrl);
    X2AP_MEMSET(&iE_list, 0, sizeof(uecc_x2ap_error_ind_ie_list_t));
    X2AP_MEMSET(&send_err_ind, 0, sizeof(uecc_x2ap_error_ind_bool_t));

    p_intrl->bitmask = 0;
    do
    {
        p_node = p_3gpp->protocolIEs.head;
        /* Save the decoded Protocol IEs */
        for (index =0; index < p_3gpp->protocolIEs.count; index++)
        {
            if (!p_node)
            {
                X2AP_TRACE(X2AP_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }
            p_protocol_IE =
                (x2ap_HandoverCancel_protocolIEs_element *)
                (p_node->data);
            switch (p_protocol_IE->id)
            {
                case ASN1V_x2ap_id_Old_eNB_UE_X2AP_ID:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(&message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                &p_protocol_IE->value.u._x2ap_HandoverCancel_IEs_1))
                    {
                        p_intrl->x2ap_Old_UE_X2AP_ID =
                            p_protocol_IE->value.u._x2ap_HandoverCancel_IEs_1;
                    }
                }
                break;

                case ASN1V_x2ap_id_New_eNB_UE_X2AP_ID:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(&message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                &p_protocol_IE->value.u._x2ap_HandoverCancel_IEs_2))
                    {
                        p_intrl->bitmask |= 
                            X2AP_HANDOVER_CANCEL_NEW_UE_X2AP_ID_PRESENT;
                        p_intrl->x2ap_New_UE_X2AP_ID =
                            p_protocol_IE->value.u._x2ap_HandoverCancel_IEs_2;
                    }
                }
                break;

                case ASN1V_x2ap_id_Cause:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(&message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                p_protocol_IE->value.u._x2ap_HandoverCancel_IEs_3))
                    {
                        p_intrl->cause =
                            *p_protocol_IE->value.u._x2ap_HandoverCancel_IEs_3;
                    }
                }
                break;

                default:
                X2AP_TRACE(X2AP_ERROR, "%s:"\
                        "invalid Protocol IE id",__FUNCTION__);
                uecc_x2ap_add_to_err_ind_ie_list(&iE_list, 
                        p_protocol_IE->criticality,
                        p_protocol_IE->id,
                        &ie_list_index,
                        &send_err_ind,
                        X2AP_FALSE,
                        X2AP_FALSE);
                /*SPR_19067_FIX_START*/
                if (x2ap_reject == p_protocol_IE->criticality) 
                {
                    retVal = X2AP_FAILURE;
                }
                /*SPR_19067_FIX_STOP*/
            }
            p_node= p_node->next;
        }
    }while(0);
    if (X2AP_SUCCESS == uecc_x2ap_parse_message_map(
                p_asn1_ctx,
                peer_enodeb_id,
                &message_map,
                &iE_list,
                &ie_list_index,
                &send_err_ind,
                ASN1V_x2ap_id_handoverCancel,
                T_x2ap_X2AP_PDU_initiatingMessage,
                x2ap_ignore,
                (rrc_x2ap_error_indication_t *)PNULL))
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED,"
                " Handover Cancel");
        retVal = X2AP_FAILURE;
    }

    X2AP_UT_TRACE_EXIT();

    return retVal;

}

/*****************************************************************************
 * Function Name  : rrc_x2ap_internal_decode_handover_command 
 * Inputs         : p_container - pointer to 
 *                              x2ap_TargeteNBtoSource_eNBTransparentContainer
 *                  p_intrl - pointer to rrc_x2ap_handover_req_ack_t
 *                  p_asn1_ctx - pointer to OSCTXT
 *                  cause - pointer to x2ap_Cause
 * Outputs        : p_intrl
 * Returns        : None
 * Description    : This function used to decode handover command
 ********************************************************************************/
void rrc_x2ap_internal_decode_handover_command(
        x2ap_TargeteNBtoSource_eNBTransparentContainer *p_container,
        rrc_x2ap_handover_req_ack_t *p_intrl,
        OSCTXT      *p_asn1_ctx,
        x2ap_Cause *cause)
{
    OSOCTET msg[X2AP_MAX_ASN1_BUF_LEN];

    X2AP_UT_TRACE_ENTER();

    cause->t = T_x2ap_Cause_protocol;

    X2AP_ASSERT(PNULL != p_intrl);
    X2AP_MEMSET(msg, 0, sizeof(msg));
    X2AP_MEMCPY(msg,p_container->data,p_container->numocts); 

    do
    {
        /*get pointer on LLIM message body*/
        pu_setBuffer(p_asn1_ctx,
                msg,
                p_container->numocts,
                ASN_RRC_BUF_TYPE);

        if ( RT_OK != asn1PD_HandoverCommand(p_asn1_ctx,
                    &p_intrl->target_to_src_enb_transparent_cont.ho_command))
        {
            /* Drop message */
            X2AP_TRACE(X2AP_WARNING,
                    "[%s] ASN decode failed.",__FUNCTION__);
            cause->u.protocol = x2ap_abstract_syntax_error_reject;
            break;
        }
        else
        {
            rrc_asn1PrtToStr_handover_command(X2AP_ASN, "X2AP_PDU",
                    &p_intrl->target_to_src_enb_transparent_cont.ho_command,
                    "HandoverCommand", msg,
                    p_container->numocts);

        }

        if ((cause->u.protocol != x2ap_abstract_syntax_error_ignore_and_notify)&&
                (cause->u.protocol != x2ap_abstract_syntax_error_reject))
        {
            cause->t = X2AP_CAUSE_SUCCESS;
        }
    } while(0);

    X2AP_UT_TRACE_EXIT();
}

/*SPR_18125_START*/
/****************************************************************************
 * Function Name  : rrc_x2ap_internal_decode_handover_preparation_information
 * Inputs         : p_asn1_ctx - pointer to OSCTXT
 *                  p_rRC_Context - unpacked s1ap_RRC_Container
 *                  p_intrl - pointer to 
 *                                  HandoverPreparationInformation_r8_IEs
 *                  cause - pointer to x2ap_Cause
 * Outputs        : p_intrl (internal HandoverPreparationInformation_r8_IEs)
 *                : s1ap_Cause (cause)
 * Returns        : X2AP_FAILURE/X2AP_SUCCESS 
 * Description    : makes HandoverPreparationInformation_r8_IEs from
 *                  p_rRC_Context
 ****************************************************************************/
void  rrc_x2ap_internal_decode_handover_preparation_information(
        OSCTXT* p_asn1_ctx, 
        x2ap_RRC_Context *p_rRC_Context,
        HandoverPreparationInformation_r8_IEs  *p_intrl,
        x2ap_Cause       *cause)
{
    /*SPR_18125_END*/
    HandoverPreparationInformation value;
    OSOCTET msg[X2AP_MAX_ASN1_BUF_LEN];

    X2AP_UT_TRACE_ENTER();
    cause->t = T_x2ap_Cause_protocol;

    X2AP_ASSERT(PNULL != p_rRC_Context);
    X2AP_ASSERT(PNULL != p_intrl);

    /*SPR_18125_START*/
    do
    {
        X2AP_MEMSET(msg, 0, sizeof(msg));
        X2AP_MEMCPY(msg,p_rRC_Context->data,p_rRC_Context->numocts);
        /*valgrind_fix_start*/
        pu_setBuffer(p_asn1_ctx,
                msg,
                p_rRC_Context->numocts,
                ASN_RRC_BUF_TYPE);
        /*valgrind_fix_stop*/

        if ( RT_OK != asn1PD_HandoverPreparationInformation(
                    p_asn1_ctx,
                    &value))
            /*SPR_18125_END*/
        {
            /* Drop message */
            X2AP_TRACE(X2AP_WARNING,
                    "%s: asn1PD_HandoverPreparationInformation failed",__FUNCTION__);
            cause->u.protocol = x2ap_transfer_syntax_error;
            break;
        }
        else
        {
            rrc_asn1PrtToStr_handover_preparation(X2AP_ASN, "X2AP_PDU",
                    &value, "HandoverPreparation", msg,
                    p_rRC_Context->numocts);
        }

        if ((1 == value.criticalExtensions.t) &&
                (PNULL != value.criticalExtensions.u.c1) &&
                (1 == value.criticalExtensions.u.c1->t) &&
                (PNULL != value.criticalExtensions.u.c1->
                 u.handoverPreparationInformation_r8))
        {
            *p_intrl =
                *(value.criticalExtensions.u.c1->
                        u.handoverPreparationInformation_r8);
        }
        else
        {
            X2AP_TRACE(X2AP_WARNING,
                    "HandoverPreparationInformation decoding failed.");
            cause->u.protocol = x2ap_transfer_syntax_error;
            break;
        }
        if ((cause->u.protocol !=
                    x2ap_abstract_syntax_error_ignore_and_notify)&&
                (cause->u.protocol != x2ap_abstract_syntax_error_reject))
        {
            cause->t = X2AP_CAUSE_SUCCESS;
        }

    } while(0);

    X2AP_UT_TRACE_EXIT();
}



/****************************************************************************
 * Function Name  : rrc_x2ap_pdu_intrl_dec
 * Inputs         : pctxt - pointer to OSCTXT
 *                  p_buff - pointer to ASN.1 encoded message
 *                  buff_size - pointer to encoded buf size
 *                  p_x2ap_pdu - pointer to x2ap_X2AP_PDU
 * Outputs        : p_x2ap_pdu (unpacked x2ap_X2AP_PDU)
 * Returns        : x2AP_SUCCESS/X2AP_FAILURE
 * Description    : unpacks x2ap_X2AP_PDU from ASN
 ****************************************************************************/
x2ap_return_et rrc_x2ap_pdu_intrl_dec
(
 OSCTXT         *pctxt,         /* for memory allocation */
 U8             *p_buff,        /* ASN.1 encoded message */
 U16            buff_size,      /* encoded buf size - just for check*/
 x2ap_X2AP_PDU  *p_x2ap_pdu     /* unpacked */
 )
{
    x2ap_return_et result = X2AP_SUCCESS;

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != pctxt);
    X2AP_ASSERT(PNULL != p_buff);
    X2AP_ASSERT(PNULL != p_x2ap_pdu);

    do
    {
        pu_setBuffer(pctxt,(OSOCTET *)p_buff,buff_size,ASN_X2AP_BUF_TYPE);

        if (RT_OK != asn1PD_x2ap_X2AP_PDU(pctxt,p_x2ap_pdu))
        {
            /* Drop message */
            X2AP_TRACE(X2AP_ERROR, "%s: ASN "
                    "decode failed.",__FUNCTION__);
            result = X2AP_FAILURE;
            break;
        }
        else
        {
            /*SPR_17976_START*/
            if(!((T_x2ap_X2AP_PDU_initiatingMessage == p_x2ap_pdu->t) &&
                        (ASN1V_x2ap_id_snStatusTransfer ==
                         p_x2ap_pdu->u.initiatingMessage->procedureCode)))
            {
                rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", p_x2ap_pdu);
            }
            /*SPR_17976_END*/
        }
    } while (0);

    X2AP_UT_TRACE_EXIT();
    return result;
}

/****************************************************************************
 * Function Name  : rrc_x2ap_error_indication_intrl_dec 
 * Inputs         : pctxt - pointer to OSCTXT
 *                  p_3gpp - unpacked rrc_x2ap_error_indication_t
 *                  p_intrl - pointer to rrc_x2ap_error_indication_t
 * Outputs        : p_intrl (internal x2ap_ErrorIndication)
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : makes rrc_x2ap_error_indication_t from
 *                  x2ap_ErrorIndication and fills to the p_intrl
 ****************************************************************************/
x2ap_return_et rrc_x2ap_error_indication_intrl_dec
(
 x2ap_ErrorIndication    *p_3gpp,    /* unpacked src */
 rrc_x2ap_error_indication_t *p_intrl/* dest */
 )
{
    x2ap_return_et result = X2AP_SUCCESS;
    OSRTDListNode  *p_node     = PNULL;
    x2ap_ErrorIndication_protocolIEs_element 
        *p_protocol_IE = PNULL;
    U32  index = X2AP_NULL;
    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != p_3gpp);
    X2AP_ASSERT(PNULL != p_intrl);

    do
    {
        p_node = p_3gpp->protocolIEs.head;
        /* Save the decoded Protocol IEs */
        for (index =0; index < p_3gpp->protocolIEs.count; index++)
        {
            if (!p_node)
            {
                X2AP_TRACE(X2AP_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                result = X2AP_FAILURE;
                break;
            }

            p_protocol_IE =
                (x2ap_ErrorIndication_protocolIEs_element *)p_node->data;

            switch (p_protocol_IE->id)
            {
                case ASN1V_x2ap_id_Old_eNB_UE_X2AP_ID:
                {
                    X2AP_TRACE(X2AP_INFO,"%s:"
                            "ASN1V_x2ap_id_Old_eNB_UE_X2AP_ID",__FUNCTION__);

                    p_intrl->old_enb_ue_x2ap_id = 
                        p_protocol_IE->value.u._x2ap_ErrorIndication_IEs_1;
                    p_intrl->bitmask |=
                        RRC_X2AP_ERROR_IND_OLD_ENB_UE_X2AP_ID_PRESENT;
                }
                break;
                case ASN1V_x2ap_id_New_eNB_UE_X2AP_ID:
                {
                    X2AP_TRACE(X2AP_INFO,"%s:"
                            "ASN1V_x2ap_id_New_eNB_UE_X2AP_ID",__FUNCTION__);

                    p_intrl->new_enb_ue_x2ap_id = 
                        p_protocol_IE->value.u._x2ap_ErrorIndication_IEs_2;
                    p_intrl->bitmask |=
                        RRC_X2AP_ERROR_IND_NEW_ENB_UE_X2AP_ID_PRESENT;
                }
                break;

                case ASN1V_x2ap_id_Cause:
                {
                    X2AP_TRACE(X2AP_INFO,"%s:"
                            "ASN1V_x2ap_id_Cause",__FUNCTION__);

                    p_intrl->cause = 
                        *p_protocol_IE->value.u._x2ap_ErrorIndication_IEs_3;
                    p_intrl->bitmask |=
                        RRC_X2AP_ERROR_IND_X2AP_CAUSE_PRESENT;
                }
                break;
                case ASN1V_x2ap_id_CriticalityDiagnostics:
                {
                    X2AP_TRACE(X2AP_INFO,"%s:"
                            "ASN1V_x2ap_id_CriticalityDiagnostics",__FUNCTION__);

                    p_intrl->criticality_diagnostics = 
                        *p_protocol_IE->value.u._x2ap_ErrorIndication_IEs_4;
                    p_intrl->bitmask |=
                        RRC_X2AP_ERROR_IND_CRITICALITY_DIAGNOSTICS_PRESENT;
                }
                break;
/*Error_Handling_changes*/
#ifdef ENDC_ENABLED
                case ASN1V_x2ap_id_New_eNB_UE_X2AP_ID_Extension:
                {
                        X2AP_TRACE(X2AP_INFO,"%s:"
                                            "ASN1V_x2ap_id_New_eNB_UE_X2AP_ID_Extension",__FUNCTION__);
                            break;
                }
                case ASN1V_x2ap_id_Old_eNB_UE_X2AP_ID_Extension:
                {
                        X2AP_TRACE(X2AP_INFO,"%s:"
                                            "ASN1V_x2ap_id_Old_eNB_UE_X2AP_ID_Extension",__FUNCTION__);
                            break;
                }
                case ASN1V_x2ap_id_Old_SgNB_UE_X2AP_ID:
                {
                    X2AP_TRACE(X2AP_INFO,"%s:"
                            "ASN1V_x2ap_id_Old_SgNB_UE_X2AP_ID",__FUNCTION__);

                    p_intrl->old_sgnb_ue_x2ap_id = 
                        p_protocol_IE->value.u._x2ap_ErrorIndication_IEs_7;
                    p_intrl->bitmask |=
                        RRC_X2AP_ERROR_IND_OLD_SGNB_UE_X2AP_ID_PRESENT;
                }
                break;
#endif
/*Error_Handling_changes*/

                default:
                X2AP_TRACE(X2AP_WARNING,"%s:"
                        " invalid ProtocolElement IE id",__FUNCTION__);
                result = X2AP_FAILURE;
                break;                  
            }
            p_node= p_node->next;            
        }

    } while (0);

    X2AP_UT_TRACE_EXIT();
    return result;
}

/****************************************************************************
 * Function Name  : rrc_x2ap_last_visited_utran_cell_information_intrl_dec
 * Inputs         : p_asn1_ctxt - pointer to OSCTXT
 *                  p_utran_cell -packed x2ap_LastVisitedUTRANCellInformation
 *                  p_intrl - pointer to ranap_LastVisitedUTRANCell_Item
 *                  cause - pointer to x2ap_Cause 
 * Outputs        : p_intrl (internal ranap_LastVisitedUTRANCell_Item)
 *                : cause (s1ap_Cause)
 * Returns        : None
 * Description    : makes ranap_LastVisitedUTRANCell_Item from
 *                  x2ap_LastVisitedUTRANCellInformation
 ****************************************************************************/
void rrc_x2ap_last_visited_utran_cell_information_intrl_dec
(
 OSCTXT                                 *p_asn1_ctxt,
 x2ap_LastVisitedUTRANCellInformation   *p_utran_cell,
 ranap_LastVisitedUTRANCell_Item        *p_intrl,
 x2ap_Cause                             *cause
 )
{
    OSOCTET msg[X2AP_MAX_ASN1_BUF_LEN];

    RRC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_asn1_ctxt);
    RRC_ASSERT(PNULL != p_utran_cell);
    RRC_ASSERT(PNULL != p_intrl);
    do
    {
        l3_memcpy_wrapper(msg, p_utran_cell->data, p_utran_cell->numocts);
        pu_setBuffer(p_asn1_ctxt,
                msg,
                p_utran_cell->numocts,
                ASN_X2AP_BUF_TYPE);

        if ( RT_OK != asn1PD_ranap_LastVisitedUTRANCell_Item (
                    p_asn1_ctxt,
                    p_intrl))
        {
            RRC_TRACE(RRC_WARNING,
                    "asn1PD_ranap_LastVisitedUTRANCell_Item failed.");
            cause->u.protocol = x2ap_transfer_syntax_error;
            break;
        }
    } while(0);

    RRC_UT_TRACE_EXIT();
    return;
}


/*
 *  The encoding decoding functions below can be used in case ASN Enc /Dec
 *  Library is to be prepared seperately
 */


/*****************************************************************************
 * Function Name  : x2ap_parse_peer_served_cells_info 
 * Inputs         : 
 *                  p_num_srv_cell - num of served cell
 *                  p_served_cells_info - pointer to x2ap_served_cell_info_arr_t
 *                  p_ServedCells - pointer to x2ap_ServedCells
 *                  p_enb_cntx - pointer to x2ap_peer_enb_context_t
 * Outputs        : p_served_cells_info
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function parses the peer served cells information 
 ********************************************************************************/
x2ap_return_et  x2ap_parse_peer_served_cells_info(
        U16                         *p_num_srv_cell,
        x2ap_served_cell_info_arr_t *p_served_cells_info,
        x2ap_ServedCells            *p_ServedCells,
        x2ap_peer_enb_context_t     *p_enb_cntx)
{
    x2ap_return_et  retVal = X2AP_SUCCESS;
    OSRTDListNode   *p_neigh_cell_info = X2AP_P_NULL;
    OSRTDListNode   *p_node = X2AP_P_NULL;
    U8              count = 0;
    U32             num_neigh_cell = 0;
    U8              num_bplmn = 0;

    X2AP_UT_TRACE_ENTER();

    p_node = p_ServedCells->head;
    *p_num_srv_cell = (U16)(p_ServedCells->count);

    /* SPR 13041 Fix Start */
    if(256 < *p_num_srv_cell)
    {
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    if (MAX_SERVED_CELLS < *p_num_srv_cell)
    {
        X2AP_TRACE(X2AP_WARNING,"%s: Served Cells greater than 8 are not supported ", __FUNCTION__);
    }

    for (count = 0; (count < p_ServedCells->count) &&
            (count < MAX_SERVED_CELLS);
            count++)
        /* SPR 13041 Fix Stop */
    {
        if (!p_node)
        {
            X2AP_TRACE(X2AP_ERROR,"%s: next node returned NULL", __FUNCTION__);
            retVal = X2AP_FAILURE;
            break;
        }
        p_served_cells_info[count].cell_info.pci =
            ((x2ap_ServedCells_element*)p_node->data)->servedCellInfo.pCI;

        x2ap_compose_cell_id_from_ecgi(p_enb_cntx->gb_enb_id,
                ((x2ap_ServedCells_element*)p_node->data)->servedCellInfo.cellId.eUTRANcellIdentifier.data,
                &p_served_cells_info[count].cell_info.cell_id);

        X2AP_MEMCPY(p_served_cells_info[count].cell_info.tac,
                ((x2ap_ServedCells_element*)p_node->data)->servedCellInfo.tAC.data,
                ((x2ap_ServedCells_element*)p_node->data)->servedCellInfo.tAC.numocts);

        p_served_cells_info[count].cell_info.bcast_plmn_info.num_bcast_plmn_id =
            (U8)(((x2ap_ServedCells_element*)p_node->data)->servedCellInfo.broadcastPLMNs.n);

        for (num_bplmn = 0; num_bplmn < ((x2ap_ServedCells_element*)p_node->data)->servedCellInfo.broadcastPLMNs.n; num_bplmn++)
        {
            X2AP_MEMCPY(p_served_cells_info[count].cell_info.bcast_plmn_info.plmn_identity[num_bplmn].plmn_id,
                    ((x2ap_ServedCells_element *)p_node->data)->servedCellInfo.broadcastPLMNs.elem[num_bplmn].data,
                    ((x2ap_ServedCells_element *)p_node->data)->servedCellInfo.broadcastPLMNs.elem[num_bplmn].numocts);
        }

        p_served_cells_info[count].cell_info.choice_eutra_mode.choice_eutra_mode.bitmask =
            (U8)(((x2ap_ServedCells_element*)p_node->data)->servedCellInfo.eUTRA_Mode_Info.t);

        if (T_x2ap_EUTRA_Mode_Info_fDD == ((x2ap_ServedCells_element*)p_node->data)->servedCellInfo.eUTRA_Mode_Info.t)
        {
            p_served_cells_info[count].cell_info.choice_eutra_mode.choice_eutra_mode.fdd_info.ul_earfcn.earfcn =
                ((x2ap_ServedCells_element*)p_node->data)->servedCellInfo.eUTRA_Mode_Info.u.fDD->uL_EARFCN;

            p_served_cells_info[count].cell_info.choice_eutra_mode.choice_eutra_mode.fdd_info.dl_earfcn.earfcn =
                ((x2ap_ServedCells_element*)p_node->data)->servedCellInfo.eUTRA_Mode_Info.u.fDD->dL_EARFCN;

            p_served_cells_info[count].cell_info.choice_eutra_mode.choice_eutra_mode.fdd_info.ul_trnsmission_bdwidth =
                (U8)(((x2ap_ServedCells_element*)p_node->data)->servedCellInfo.eUTRA_Mode_Info.u.fDD->uL_Transmission_Bandwidth);

            p_served_cells_info[count].cell_info.choice_eutra_mode.choice_eutra_mode.fdd_info.dl_trnsmission_bdwidth =
                (U8)(((x2ap_ServedCells_element*)p_node->data)->servedCellInfo.eUTRA_Mode_Info.u.fDD->dL_Transmission_Bandwidth);
        }
        else if (T_x2ap_EUTRA_Mode_Info_tDD == ((x2ap_ServedCells_element*)p_node->data)->servedCellInfo.eUTRA_Mode_Info.t)
        {
            p_served_cells_info[count].cell_info.choice_eutra_mode.choice_eutra_mode.tdd_info.earfcn.earfcn =
                ((x2ap_ServedCells_element*)p_node->data)->servedCellInfo.eUTRA_Mode_Info.u.tDD->eARFCN;

            p_served_cells_info[count].cell_info.choice_eutra_mode.choice_eutra_mode.tdd_info.trnsmission_bdwidth =
                (U8)(((x2ap_ServedCells_element*)p_node->data)->servedCellInfo.eUTRA_Mode_Info.u.tDD->transmission_Bandwidth);

            p_served_cells_info[count].cell_info.choice_eutra_mode.choice_eutra_mode.tdd_info.subframe_assn =
                (U8)(((x2ap_ServedCells_element*)p_node->data)->servedCellInfo.eUTRA_Mode_Info.u.tDD->subframeAssignment);

            p_served_cells_info[count].cell_info.choice_eutra_mode.choice_eutra_mode.tdd_info.special_subframe_info.special_subframe_patterns =
                (U8)(((x2ap_ServedCells_element*)p_node->data)->servedCellInfo.eUTRA_Mode_Info.u.tDD->specialSubframe_Info.specialSubframePatterns);

            p_served_cells_info[count].cell_info.choice_eutra_mode.choice_eutra_mode.tdd_info.special_subframe_info.dl_cp =
                (U8)(((x2ap_ServedCells_element*)p_node->data)->servedCellInfo.eUTRA_Mode_Info.u.tDD->specialSubframe_Info.cyclicPrefixDL);

            p_served_cells_info[count].cell_info.choice_eutra_mode.choice_eutra_mode.tdd_info.special_subframe_info.ul_cp =
                (U8)(((x2ap_ServedCells_element*)p_node->data)->servedCellInfo.eUTRA_Mode_Info.u.tDD->specialSubframe_Info.cyclicPrefixUL);
        }
        else
        {
            X2AP_TRACE(X2AP_ERROR,"%s: Eutran mode is not present", __FUNCTION__);
            retVal = X2AP_FAILURE;
            break;
        }
        if (((x2ap_ServedCells_element *)p_node->data)->m.neighbour_InfoPresent)
        {
            p_neigh_cell_info = ((x2ap_ServedCells_element*)p_node->data)->neighbour_Info.head;

            p_served_cells_info[count].nbour_cell_info.num_neighbour_cell =
                (U16)(((x2ap_ServedCells_element*)p_node->data)->neighbour_Info.count);

            for (num_neigh_cell = 0;num_neigh_cell < ((x2ap_ServedCells_element*)p_node->data)->neighbour_Info.count; num_neigh_cell++)
            {
                X2AP_MEMCPY(p_served_cells_info[count].nbour_cell_info.neighbour_info_arr[num_neigh_cell].ecgi.plmn_identity.plmn_id,
                        ((x2ap_Neighbour_Information_element *)p_neigh_cell_info->data)->eCGI.pLMN_Identity.data,
                        ((x2ap_Neighbour_Information_element *)p_neigh_cell_info->data)->eCGI.pLMN_Identity.numocts);

                X2AP_MEMCPY(&(p_served_cells_info[count].nbour_cell_info.neighbour_info_arr[num_neigh_cell].ecgi.eutran_cell_id),
                        ((x2ap_Neighbour_Information_element *)p_neigh_cell_info->data)->eCGI.eUTRANcellIdentifier.data,
                        sizeof (U32));

                p_served_cells_info[count].nbour_cell_info.neighbour_info_arr[num_neigh_cell].pci =
                    ((x2ap_Neighbour_Information_element *)p_neigh_cell_info->data)->pCI;

                p_served_cells_info[count].nbour_cell_info.neighbour_info_arr[num_neigh_cell].earfcn.earfcn =
                    ((x2ap_Neighbour_Information_element *)p_neigh_cell_info->data)->eARFCN;

                p_neigh_cell_info = p_neigh_cell_info->next;
            }
        }

        if(((x2ap_ServedCells_element *)p_node->data)->servedCellInfo.m.iE_ExtensionsPresent)
        {
            U8 extensions_count = 0;
            U8 extensions_counter =0;
            OSRTDListNode* p_node_1 = X2AP_P_NULL;

            p_node_1 = ((x2ap_ServedCells_element *)p_node->data)->servedCellInfo.iE_Extensions.head;
            extensions_count = ((x2ap_ServedCells_element *)p_node->data)->servedCellInfo.iE_Extensions.count;

            for(extensions_counter=0;extensions_counter<extensions_count; extensions_counter++)
            {
                if(((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.t==T341x2ap___x2ap_ServedCell_Information_ExtIEs_1)
                {
                    p_served_cells_info[count].cell_info.bitmask |= X2AP_NUM_ANTENNA_INFO_PORT_PRESENT;
                    p_served_cells_info[count].cell_info.num_antenna_port =
                        ((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_1;
                }
                else if(((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.t==T341x2ap___x2ap_ServedCell_Information_ExtIEs_2)
                {
                    p_served_cells_info[count].cell_info.bitmask |= X2AP_PRACH_CONFIG_INFO_PRESENT;
                    p_served_cells_info[count].cell_info.prach_config.high_speed_flag =
                        ((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_2->highSpeedFlag;
                    p_served_cells_info[count].cell_info.prach_config.prach_frequency_offset =
                        ((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_2->prach_FreqOffset;
                    p_served_cells_info[count].cell_info.prach_config.root_sequence_index =
                        ((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_2->rootSequenceIndex;
                    p_served_cells_info[count].cell_info.prach_config.zero_correlation_zone_config  =
                        ((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_2->zeroCorrelationIndex;

                    if(((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_2->m.prach_ConfigIndexPresent)
                    {
                        p_served_cells_info[count].cell_info.prach_config.bitmask |= X2AP_PRACH_CONFIG_INDEX_PRESENT;
                        p_served_cells_info[count].cell_info.prach_config.prach_config_index =
                            ((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_2->prach_ConfigIndex;
                    }
                }

                /* SES-460 changes start */
                if(((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.t==T341x2ap___x2ap_ServedCell_Information_ExtIEs_4)
                {
                    p_served_cells_info[count].cell_info.bitmask |= X2AP_CSG_ID_INFO_PRESENT;
                    l3_memcpy_wrapper(p_served_cells_info[count].cell_info.csg_identity,
                            ((x2ap_ServedCell_Information_iE_Extensions_element*)p_node_1->data)->extensionValue.u._x2ap_ServedCell_Information_ExtIEs_4->data,
                            sizeof(p_served_cells_info[count].cell_info.csg_identity));
                }
                /* SES-460 changes end */
                p_node_1 = p_node_1->next;
            }
        }
        p_node = p_node->next;
    }
    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_decode_rlf_indication 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_rlf_ind - pointer to x2ap_rlf_ind_info_t
 *                  p_asn_msg - pointer to  ASN buffer 
 *                  p_asn_msg_len - pointer to  ASN buffer length
 *                  p_enb_cntx - pointer to x2ap_peer_enb_context_t
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode RLF Indication
 * ERROR          : This function report error to the caller function, If decoder
 *                  fails to decode message.
 ********************************************************************************/
x2ap_return_et x2ap_decode_rlf_indication(
        x2ap_gb_context_t *p_x2ap_gb_ctx,
        x2ap_rlf_ind_info_t    *p_rlf_ind,
        U8                     *p_asn_msg,
        U32                    *p_asn_msg_len,
        x2ap_peer_enb_context_t* p_enb_cntx)
{
    x2ap_return_et                 retVal = X2AP_SUCCESS;

    U32                            count = 0;
    x2ap_X2AP_PDU                  x2ap_pdu;
    OSCTXT                         asn1_ctx;
    OSRTDListNode                  *p_node = X2AP_P_NULL;
    x2ap_RLFIndication             *p_RLFIndication = X2AP_P_NULL;
    x2ap_RLFIndication_protocolIEs_element
        *p_protocol_IEs_element = X2AP_P_NULL;
    x2ap_error_ind_ie_list_t   iE_list;
    x2ap_error_ind_bool_t send_err_ind;
    U16 ie_list_index = 0;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    U32                         encoded_msg_len = 0;

    x2ap_message_data_t message_map =
    {6, X2AP_FALSE,X2AP_FALSE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_FailureCellPCI, x2ap_mandatory, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {1, ASN1V_x2ap_id_Re_establishmentCellECGI, x2ap_mandatory, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {2, ASN1V_x2ap_id_FailureCellCRNTI, x2ap_mandatory, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {3, ASN1V_x2ap_id_ShortMAC_I, x2ap_optional, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {4, ASN1V_x2ap_id_UE_RLF_Report_Container, x2ap_optional, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {5, ASN1V_x2ap_id_RRCConnSetupIndicator, x2ap_optional, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL}
        }};

    X2AP_MEMSET(&iE_list, X2AP_NULL, sizeof(x2ap_error_ind_ie_list_t));
    X2AP_MEMSET(&send_err_ind, X2AP_FALSE, sizeof(x2ap_error_ind_bool_t));

    if (!p_rlf_ind)
    {
        X2AP_TRACE(X2AP_ERROR, "%s: ASN p_rlf_ind is NULL",__FUNCTION__);
        return X2AP_FAILURE;
    }

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        /* Drop message */
        X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                "context initialization failed.",__FUNCTION__);
        return X2AP_FAILURE;
    }
    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(&asn1_ctx, p_asn_msg, *p_asn_msg_len, TRUE);

    do
    {
        if (0 != asn1PD_x2ap_X2AP_PDU(&asn1_ctx, &x2ap_pdu) )
        {
            /* Drop message */
            X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                    "decode failed.",__FUNCTION__);
            /* Send Transfer Syntax Error */
            x2ap_build_and_send_err_indication(p_x2ap_gb_ctx, p_enb_cntx, PNULL);
            retVal = X2AP_FAILURE;
            break;
        }
        else
        {
            encoded_msg_len = (U32)pe_GetMsgLen(&asn1_ctx);


            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);

            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    encoded_msg_len);
            if (PNULL != p_event_header)
            {
                x2ap_fill_protocol_event_params(p_event_header, encoded_msg_len,
                        LOCAL_X2_RLF_INDICATION, 
                        MESSAGE_DIRECTION_RECEIVED);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        encoded_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

        /* check whether the message is not successful */
        if ((T_x2ap_X2AP_PDU_initiatingMessage == x2ap_pdu.t) ||
                (ASN1V_x2ap_id_rLFIndication == 
                 x2ap_pdu.u.initiatingMessage->procedureCode) ||
                ((X2AP_P_NULL != x2ap_pdu.u.initiatingMessage->value.u.
                  rLFIndication) &&
                 (T1x2ap__rLFIndication == 
                  x2ap_pdu.u.initiatingMessage->value.t)))
        {
            p_RLFIndication = x2ap_pdu.u.initiatingMessage->value.u.
                rLFIndication;

            p_node = p_RLFIndication->protocolIEs.head;

            for (count =0;
                    count < p_RLFIndication->protocolIEs.count;
                    count++)
            {
                if (!p_node)
                {
                    X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                            __FUNCTION__);
                    retVal = X2AP_FAILURE;
                    break;
                }
                p_protocol_IEs_element = 
                    (x2ap_RLFIndication_protocolIEs_element *)p_node->data;

                switch (p_protocol_IEs_element->id)
                {
                    case ASN1V_x2ap_id_FailureCellPCI:
                    {
                        X2AP_TRACE(X2AP_INFO,"%s: "\
                                "ASN1V_x2ap_id_FailureCellPCI\n",__FUNCTION__);
                        if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                    &message_map,
                                    count,
                                    p_protocol_IEs_element->id,
                                    &p_protocol_IEs_element->value.u._x2ap_RLFIndication_IEs_1))
                        {
                            p_rlf_ind->failure_cell_pci = 
                                p_protocol_IEs_element->value.u._x2ap_RLFIndication_IEs_1;
                        }
                    }
                    break;

                    case ASN1V_x2ap_id_Re_establishmentCellECGI:
                    {
                        X2AP_TRACE(X2AP_INFO,"%s: "\
                                "ASN1V_x2ap_id_Re_establishmentCellECGI\n",__FUNCTION__);
                        if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                    &message_map,
                                    count,
                                    p_protocol_IEs_element->id,
                                    p_protocol_IEs_element->value.u._x2ap_RLFIndication_IEs_2))
                        {
                            X2AP_MEMCPY(p_rlf_ind->re_est_cell_ecgi.plmn_identity.plmn_id,
                                    p_protocol_IEs_element->value.u.
                                    _x2ap_RLFIndication_IEs_2->pLMN_Identity.data,
                                    p_protocol_IEs_element->value.u.
                                    _x2ap_RLFIndication_IEs_2->pLMN_Identity.numocts);
                            X2AP_MEMCPY(p_rlf_ind->re_est_cell_ecgi.eutran_cell_id,
                                    p_protocol_IEs_element->value.u.
                                    _x2ap_RLFIndication_IEs_2->eUTRANcellIdentifier.data,
                                    sizeof (U32));
                        }
                    }
                    break;

                    case ASN1V_x2ap_id_FailureCellCRNTI:
                    {
                        X2AP_TRACE(X2AP_INFO,"%s: "\
                                "ASN1V_x2ap_id_FailureCellCRNTI\n",__FUNCTION__);
                        if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                    &message_map,
                                    count,
                                    p_protocol_IEs_element->id,
                                    p_protocol_IEs_element->value.u._x2ap_RLFIndication_IEs_3))
                        {
                            p_rlf_ind->crnti = 
                                p_protocol_IEs_element->value.u._x2ap_RLFIndication_IEs_3->data[0];
                            p_rlf_ind->crnti = (U16)(p_rlf_ind->crnti << 8);
                            p_rlf_ind->crnti = 
                                (U16)(p_rlf_ind->crnti | p_protocol_IEs_element->
                                        value.u._x2ap_RLFIndication_IEs_3->data[1]);
                        }
                    }
                    break;

                    case ASN1V_x2ap_id_ShortMAC_I:
                    {    
                        X2AP_TRACE(X2AP_INFO,"%s: "\
                                "ASN1V_x2ap_id_ShortMAC_I\n",__FUNCTION__);
                        if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                    &message_map,
                                    count,
                                    p_protocol_IEs_element->id,
                                    p_protocol_IEs_element->value.u._x2ap_RLFIndication_IEs_4))
                        {
                            p_rlf_ind->bitmask |= X2AP_SHORT_MAC_I_PRESENT;
                            p_rlf_ind->short_mac_i = 
                                p_protocol_IEs_element->value.u._x2ap_RLFIndication_IEs_4->data[0];
                            p_rlf_ind->short_mac_i = (U16)(p_rlf_ind->short_mac_i << 8);
                            p_rlf_ind->short_mac_i = 
                                (U16)(p_rlf_ind->short_mac_i | p_protocol_IEs_element->
                                        value.u._x2ap_RLFIndication_IEs_4->data[1]);
                        }
                    }
                    break;

                    case ASN1V_x2ap_id_UE_RLF_Report_Container:
                    {
                        X2AP_TRACE(X2AP_INFO,"%s: "\
                                "ASN1V_x2ap_id_UE_RLF_Report_Container\n",__FUNCTION__);
                        if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                    &message_map,
                                    count,
                                    p_protocol_IEs_element->id,
                                    p_protocol_IEs_element->value.u._x2ap_RLFIndication_IEs_5))
                        {
                            p_rlf_ind->bitmask |= X2AP_RLF_REPORT_PRESENT;
                            p_rlf_ind->rlf_report.message_buff_size = 
                                p_protocol_IEs_element->value.u._x2ap_RLFIndication_IEs_5->numocts;

                            X2AP_MEMCPY(&p_rlf_ind->rlf_report.message_buff_p[0],
                                    p_protocol_IEs_element->value.u._x2ap_RLFIndication_IEs_5->data,
                                    p_protocol_IEs_element->value.u._x2ap_RLFIndication_IEs_5->numocts);

                        }
                    }
                    break;

                    case ASN1V_x2ap_id_RRCConnSetupIndicator: 
                    {         
                        if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                    &message_map,
                                    count,
                                    p_protocol_IEs_element->id,
                                    &p_protocol_IEs_element->value.u._x2ap_RLFIndication_IEs_6))
                        {
                            p_rlf_ind->bitmask |= X2AP_RLF_CONN_SETUP_INDICATOR_PRESENT;
                            p_rlf_ind->conn_setup_indicator =
                                p_protocol_IEs_element->value.u._x2ap_RLFIndication_IEs_6;
                        }
                    }
                    break;

                    default:
                    X2AP_TRACE(X2AP_ERROR,"%s: "\
                            "Invalid Protocol IE ID\n",__FUNCTION__);
                    x2ap_add_to_err_ind_ie_list(&iE_list,
                            p_protocol_IEs_element->criticality,
                            p_protocol_IEs_element->id,
                            &ie_list_index,
                            &send_err_ind,
                            X2AP_FALSE,
                            X2AP_FALSE);
                    /*SPR_19067_FIX_START*/
                    if (x2ap_reject == p_protocol_IEs_element->criticality) 
                    {
                        retVal = X2AP_FAILURE;
                    }
                    /*SPR_19067_FIX_STOP*/
                }

                p_node = p_node->next;
            }
        }
        else
        {
            /* Drop message */
            X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                    "decode failed Unknown Message type received.",
                    __FUNCTION__);
            retVal = X2AP_FAILURE;
            break;
        }
    }while(0);
    /* Parse the map for Error Indication */
    if ((X2AP_SUCCESS == x2ap_parse_message_map(
                    &asn1_ctx,
                    p_x2ap_gb_ctx,
                    p_enb_cntx,
                    &message_map,
                    &iE_list,
                    &ie_list_index,
                    &send_err_ind,
                    ASN1V_x2ap_id_rLFIndication,
                    T_x2ap_X2AP_PDU_initiatingMessage,
                    x2ap_ignore,
                    (rrc_x2ap_error_indication_t *)PNULL)))
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED in RLF Indication");

        retVal = X2AP_FAILURE;
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    return retVal;
}


/*****************************************************************************
 * Function Name  : x2ap_decode_handover_report 
 * Inputs         : p_x2ap_gb_ctx - pointer to X2AP global context
 *                  p_ho_report_ind -  pointer to x2ap_ho_report_ind_t
 *                  p_asn_msg - pointer to ASN encoded msg 
 *                  p_asn_msg_len -  pointer to ASN encoded msg length 
 *                  p_enb_cntx - pointer to x2ap_peer_enb_context_t
 * Outputs        : None 
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode Handover Report
 * ERROR          : This function report error to the caller function, If decoder
 *                  fails  to decode message.
 ********************************************************************************/
x2ap_return_et x2ap_decode_handover_report(
        x2ap_gb_context_t *p_x2ap_gb_ctx,
        x2ap_ho_report_ind_t   *p_ho_report_ind,
        U8                     *p_asn_msg,
        U32                    *p_asn_msg_len,
        x2ap_peer_enb_context_t* p_enb_cntx)
{
    x2ap_return_et                 retVal = X2AP_SUCCESS;

    U32                            count = 0;
    x2ap_X2AP_PDU                  x2ap_pdu;
    OSCTXT                         asn1_ctx;
    OSRTDListNode                  *p_node = X2AP_P_NULL;
    x2ap_HandoverReport            *p_ho_report = X2AP_P_NULL;
    x2ap_HandoverReport_protocolIEs_element
        *p_protocol_IEs_element = X2AP_P_NULL;
    x2ap_error_ind_ie_list_t   iE_list;
    x2ap_error_ind_bool_t send_err_ind;
    U16 ie_list_index = 0;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    U32                         encoded_msg_len = 0;
    x2ap_message_data_t message_map =
    {5, X2AP_FALSE,X2AP_FALSE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_HandoverReportType, x2ap_mandatory, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {1, ASN1V_x2ap_id_Cause, x2ap_mandatory, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {2, ASN1V_x2ap_id_SourceCellECGI, x2ap_mandatory, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {3, ASN1V_x2ap_id_FailureCellECGI, x2ap_mandatory, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {4, ASN1V_x2ap_id_Re_establishmentCellECGI, x2ap_conditional, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL}
        }};

    X2AP_MEMSET(&iE_list, X2AP_NULL, sizeof(x2ap_error_ind_ie_list_t));
    X2AP_MEMSET(&send_err_ind, X2AP_FALSE, sizeof(x2ap_error_ind_bool_t));

    if (p_ho_report_ind == X2AP_P_NULL)
    {
        X2AP_TRACE(X2AP_ERROR, "%s: ho_report_ind pointer is NULL",__FUNCTION__);
        return X2AP_FAILURE;
    }

    /* Init ASN1 context */
    if ( 0 != rtInitContext(&asn1_ctx) )
    {
        /* Drop message */
        X2AP_TRACE(X2AP_ERROR, "%s: ASN context initialization failed.",__FUNCTION__);
        return X2AP_FAILURE;
    }
    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(&asn1_ctx, p_asn_msg, *p_asn_msg_len, TRUE);

    do
    {
        if (0 != asn1PD_x2ap_X2AP_PDU(&asn1_ctx, &x2ap_pdu) )
        {
            /* Drop message */
            X2AP_TRACE(X2AP_ERROR, "%s: ASN decode failed.",__FUNCTION__);
            /* Send Transfer Syntax Error */
            x2ap_build_and_send_err_indication(p_x2ap_gb_ctx, p_enb_cntx, PNULL);
            retVal = X2AP_FAILURE;
            break;
        }
        else
        {
            encoded_msg_len = (U32)pe_GetMsgLen(&asn1_ctx);


            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);

            /*coverity_fix_531*/
            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    encoded_msg_len);
            if (PNULL != p_event_header)
            {
                x2ap_fill_protocol_event_params(p_event_header, encoded_msg_len,
                        LOCAL_X2_HANDOVER_REPORT,
                        MESSAGE_DIRECTION_RECEIVED);

                l3_memcpy_wrapper((void*)((U8*)p_event_header +
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        encoded_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
            /*coverity_fix_531*/ 
        }

        /* check whether the message is not successful */
        if ((T_x2ap_X2AP_PDU_initiatingMessage == x2ap_pdu.t) ||
                (ASN1V_x2ap_id_handoverReport ==
                 x2ap_pdu.u.initiatingMessage->procedureCode) ||
                ((X2AP_P_NULL != x2ap_pdu.u.initiatingMessage->value.u.
                  handoverReport) &&
                 (T1x2ap__handoverReport == x2ap_pdu.u.initiatingMessage->value.t)))
        {
            p_ho_report = x2ap_pdu.u.initiatingMessage->value.u.handoverReport;

            p_node = p_ho_report->protocolIEs.head;

            for (count =0;
                    count < p_ho_report->protocolIEs.count; count++)
            {
                if (!p_node)
                {
                    X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                            __FUNCTION__);
                    retVal = X2AP_FAILURE;
                    break;
                }
                p_protocol_IEs_element =
                    (x2ap_HandoverReport_protocolIEs_element *)p_node->data;

                switch (p_protocol_IEs_element->id)
                {
                    case ASN1V_x2ap_id_HandoverReportType:
                    {
                        X2AP_TRACE(X2AP_INFO,"%s: "\
                                "ASN1V_x2ap_id_HandoverReportType\n",__FUNCTION__);
                        if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                    &message_map,
                                    count,
                                    p_protocol_IEs_element->id,
                                    &p_protocol_IEs_element->value.u._x2ap_HandoverReport_IEs_1))
                        {
                            p_ho_report_ind->ho_report_type = 
                                p_protocol_IEs_element->value.u._x2ap_HandoverReport_IEs_1;
                        }
                    }
                    break;

                    case ASN1V_x2ap_id_Cause:
                    {
                        X2AP_TRACE(X2AP_INFO, "%s: "\
                                "ASN1V_x2ap_id_Cause",__FUNCTION__);
                        if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                    &message_map,
                                    count,
                                    p_protocol_IEs_element->id,
                                    p_protocol_IEs_element->value.u._x2ap_HandoverReport_IEs_2))
                        {
                            x2ap_reset_parse_asn_x2ap_cause(
                                    *(p_protocol_IEs_element->value.u._x2ap_HandoverReport_IEs_2),
                                    &(p_ho_report_ind->ho_cause));
                        }
                    }
                    break;

                    case ASN1V_x2ap_id_SourceCellECGI:
                    {
                        X2AP_TRACE(X2AP_INFO,"%s: "\
                                "ASN1V_x2ap_id_SourceCellECGI\n",__FUNCTION__);
                        if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                    &message_map,
                                    count,
                                    p_protocol_IEs_element->id,
                                    p_protocol_IEs_element->value.u._x2ap_HandoverReport_IEs_3))
                        {
                            X2AP_MEMCPY(
                                    p_ho_report_ind->src_ecgi.plmn_identity.plmn_id,
                                    p_protocol_IEs_element->value.u.
                                    _x2ap_HandoverReport_IEs_3->pLMN_Identity.data,
                                    p_protocol_IEs_element->value.u.
                                    _x2ap_HandoverReport_IEs_3->pLMN_Identity.numocts);

                            X2AP_MEMCPY(p_ho_report_ind->src_ecgi.eutran_cell_id,
                                    p_protocol_IEs_element->value.u.
                                    _x2ap_HandoverReport_IEs_3->eUTRANcellIdentifier.data,
                                    sizeof (U32));
                        }
                    }
                    break;

                    case ASN1V_x2ap_id_FailureCellECGI:
                    {
                        X2AP_TRACE(X2AP_INFO,"%s: "\
                                "ASN1V_x2ap_id_FailureCellECGI\n",__FUNCTION__);
                        if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                    &message_map,
                                    count,
                                    p_protocol_IEs_element->id,
                                    p_protocol_IEs_element->value.u._x2ap_HandoverReport_IEs_4))
                        {
                            X2AP_MEMCPY(
                                    p_ho_report_ind->fail_cell_ecgi.plmn_identity.plmn_id,
                                    p_protocol_IEs_element->value.u.
                                    _x2ap_HandoverReport_IEs_4->pLMN_Identity.data,
                                    p_protocol_IEs_element->value.u.
                                    _x2ap_HandoverReport_IEs_4->pLMN_Identity.numocts);

                            X2AP_MEMCPY(p_ho_report_ind->fail_cell_ecgi.eutran_cell_id,
                                    p_protocol_IEs_element->value.u.
                                    _x2ap_HandoverReport_IEs_4->eUTRANcellIdentifier.data,
                                    sizeof (U32));
                        }
                    }
                    break;

                    case ASN1V_x2ap_id_Re_establishmentCellECGI:
                    {
                        X2AP_TRACE(X2AP_INFO,"%s: "\
                                "ASN1V_x2ap_id_Re_establishmentCellECGI\n",__FUNCTION__);
                        if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                    &message_map,
                                    count,
                                    p_protocol_IEs_element->id,
                                    p_protocol_IEs_element->value.u._x2ap_HandoverReport_IEs_5))
                        {
                            p_ho_report_ind->bitmask |= X2AP_HO_REPORT_REEST_CELL_ECGI_PRESENT; 
                            X2AP_MEMCPY(
                                    p_ho_report_ind->re_est_cell_ecgi.plmn_identity.plmn_id,
                                    p_protocol_IEs_element->value.u.
                                    _x2ap_HandoverReport_IEs_5->pLMN_Identity.data,
                                    p_protocol_IEs_element->value.u.
                                    _x2ap_HandoverReport_IEs_5->pLMN_Identity.numocts);

                            X2AP_MEMCPY(p_ho_report_ind->re_est_cell_ecgi.eutran_cell_id,
                                    p_protocol_IEs_element->value.u.
                                    _x2ap_HandoverReport_IEs_5->eUTRANcellIdentifier.data,
                                    sizeof (U32));
                        }
                    }
                    break;

                    default:
                    X2AP_TRACE(X2AP_ERROR,"%s: Invalid Protocol IE ID\n",__FUNCTION__);
                    x2ap_add_to_err_ind_ie_list(&iE_list,
                            p_protocol_IEs_element->criticality,
                            p_protocol_IEs_element->id,
                            &ie_list_index,
                            &send_err_ind,
                            X2AP_FALSE,
                            X2AP_FALSE);
                    /*SPR_19067_FIX_START*/
                    if (x2ap_reject == p_protocol_IEs_element->criticality) 
                    {
                        retVal = X2AP_FAILURE;
                    }
                    /*SPR_19067_FIX_STOP*/
                }
                p_node = p_node->next;
            }
        }
        else
        {
            /* Drop message */
            X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                    "decode failed Unknown Message type received.",
                    __FUNCTION__);
            retVal = X2AP_FAILURE;
            break;
        }
    }while(0);

    /* Parse the map for Error Indication */
    if ((X2AP_SUCCESS == x2ap_parse_message_map(
                    &asn1_ctx,
                    p_x2ap_gb_ctx,
                    p_enb_cntx,
                    &message_map,
                    &iE_list,
                    &ie_list_index,
                    &send_err_ind,
                    ASN1V_x2ap_id_handoverReport,
                    T_x2ap_X2AP_PDU_initiatingMessage,
                    x2ap_ignore,
                    (rrc_x2ap_error_indication_t *)PNULL)))
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED in Handover Report");

        retVal = X2AP_FAILURE;
    }

    /* Free ASN Context */
    rtFreeContext(&asn1_ctx);

    return retVal;
}

#ifdef ENDC_ENABLED
/* MENB CHANGES - START */
/*****************************************************************************
 * Function Name  : x2ap_internal_decode_meas_timing_config 
 * Inputs         : OSDynOctStr - pointer to ASN measurement timing config
 *                  string
 *                  nr_rrc_MeasurementTimingConfiguration - pointer to
 *                  ASN measurement timing config
 *                  p_asn1_ctx - pointer to OSCTXT
 * Outputs        : meas_timing_config 
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE 
 * Description    : This function used to decode measurement timing configuration
 *                  for EN DC X2 setup response.
 ********************************************************************************/
x2ap_return_et
x2ap_internal_decode_meas_timing_config
(
    OSDynOctStr                              *p_meas_timing_config,
    nr_rrc_MeasurementTimingConfiguration    *p_asn_meas_timing_config,
    OSCTXT                                   *p_asn1_ctx
)
{
    x2ap_return_et           result = X2AP_FAILURE;
    /* coverity_<280096>_fix_start */
    S8                      ret_val = X2AP_NULL;
    /* coverity_<280096>_fix_end */

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != p_asn_meas_timing_config);

    X2AP_MEMSET(p_asn_meas_timing_config, RRC_NULL,
        sizeof(nr_rrc_MeasurementTimingConfiguration));

    do
    {
        if ( X2AP_NULL != rtInitContext(p_asn1_ctx) )
        {
            /* Drop message */
            X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                    "context initialization failed.",__FUNCTION__);
            X2AP_UT_TRACE_EXIT();
            break;
        }

        pu_setBuffer(p_asn1_ctx,
                (OSOCTET *)p_meas_timing_config->data,
                p_meas_timing_config->numocts,
                ASN_RRC_BUF_TYPE);

        if ( RT_OK != asn1PD_nr_rrc_MeasurementTimingConfiguration(p_asn1_ctx,
                    p_asn_meas_timing_config))
        {
            /* Drop message */
            rtxErrPrint(p_asn1_ctx); 
            X2AP_TRACE(X2AP_WARNING,
                    "[%s] ASN decode failed.",__FUNCTION__);
            break;
        }
        else
        {
            S8* pBuff = rrc_mem_get(MAX_ASN_PRINT_BUFF_SIZE);

            if(pBuff != NULL)
            {
                memset_wrapper(pBuff, X2AP_NULL, MAX_ASN_PRINT_BUFF_SIZE);

                /* Converted to char* as required by ASN function */
                /* coverity_<280096>_fix_start */
                ret_val = asn1PrtToStr_nr_rrc_MeasurementTimingConfiguration(
                        "MeasurementTimingConfiguration",
                        p_asn_meas_timing_config,
                        (char*)pBuff,
                        MAX_ASN_PRINT_BUFF_SIZE);
                if ( ret_val == X2AP_NULL )
                 /* coverity_<280096>_fix_end */
                {
                    RRC_TRACE_ASN((S32)RRC_ASN, "%s\n", pBuff);
                    result = X2AP_SUCCESS;
                }
                else
                {
                    RRC_TRACE(RRC_WARNING,"Buffer size for ASN needs to be increased");
                }
                rrc_mem_free(pBuff);
            }
        }
    }while(X2AP_NULL);

    X2AP_UT_TRACE_EXIT();
    return result;
}

/*******************************************************************************
 * Function Name  : x2ap_parse_nr_svrd_info_endc_x2_management_list
 * Inputs         : x2ap_ServedNRCell_Information             *p_served_nr_cell_Info_src,
 *                  x2ap_srvd_nr_cell_info_t                      *p_served_nr_cell_info,
 *                  x2ap_message_data_t                     *p_ie_order_map,
 *                  U8                                      order_index,
 *                 U8                                      id,
 *                  x2ap_bool_et                             *p_send_tsc
 *
 * Outputs        : x2ap_srvd_nr_cell_info_t                       *p_served_nr_cell_info
 *
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode NR Neighbour information
 *******************************************************************************/
x2ap_return_et
x2ap_parse_nr_svrd_info_endc_x2_management_list
(
    x2ap_ServedNRCell_Information             *p_served_nr_cell_Info_src,
    x2ap_srvd_nr_cell_info_t                      *p_served_nr_cell_info,
    x2ap_bool_et                             *p_send_tsc,
    OSCTXT                                  *p_asn1_ctx
)
{
    x2ap_return_et                  retVal = X2AP_SUCCESS;
    OSRTDListNode                   *p_node_1 = X2AP_P_NULL;
    OSRTDListNode                   *p_node_2 = X2AP_P_NULL;
    OSRTDListNode                   *p_node_3 = X2AP_P_NULL;
    OSRTDListNode                   *p_node_4 = X2AP_P_NULL;
    U8                              freq_count = X2AP_NULL;
    U8                              sul_count = X2AP_NULL;
    U16                             num_bplmn = X2AP_NULL;//
    x2ap_FDD_InfoServedNRCell_Information
                                    *p_fdd_served_nr_cell_info = X2AP_P_NULL;
    x2ap_TDD_InfoServedNRCell_Information
                                    *p_tdd_served_nr_cell_info = X2AP_P_NULL;

    X2AP_UT_TRACE_ENTER();                                
                                    
     p_served_nr_cell_info->nr_pci =
         p_served_nr_cell_Info_src->nrpCI;

     X2AP_MEMCPY(p_served_nr_cell_info->nr_cell_id.plmn_identity.plmn_id,
             p_served_nr_cell_Info_src->nrCellID.pLMN_Identity.data,
             p_served_nr_cell_Info_src->nrCellID.pLMN_Identity.numocts);

     X2AP_MEMCPY(p_served_nr_cell_info->nr_cell_id.nr_cell_id,
             p_served_nr_cell_Info_src->nrCellID.nRcellIdentifier.data,
             NR_CELL_ID_OCTET_SIZE);

     if( RRC_ONE == (p_served_nr_cell_Info_src->m.fiveGS_TACPresent) )
     {
         p_served_nr_cell_info->bitmask |=
             X2AP_NR_CELL_INFO_FIVE_GS_TAC_PRESENT;

         X2AP_MEMCPY(p_served_nr_cell_info->fiveGS_track_area_code.tac,
                 p_served_nr_cell_Info_src->fiveGS_TAC.data,
                 p_served_nr_cell_Info_src->fiveGS_TAC.numocts);
     }

     if( RRC_ONE == (p_served_nr_cell_Info_src->m.configured_TACPresent) )
     {
         p_served_nr_cell_info->bitmask |=
             X2AP_NR_CELL_INFO_CONFIGURED_TAC_PRESENT;

         X2AP_MEMCPY(p_served_nr_cell_info->configured_tac.tac,
                 p_served_nr_cell_Info_src->configured_TAC.data,
                 p_served_nr_cell_Info_src->configured_TAC.numocts);
     }

     p_served_nr_cell_info->broadcast_plmn_info.num_bcast_plmn_id =
         p_served_nr_cell_Info_src->broadcastPLMNs.n;

     for ( num_bplmn = X2AP_NULL; num_bplmn < p_served_nr_cell_info->
             broadcast_plmn_info.num_bcast_plmn_id; num_bplmn++ )
     {
         X2AP_MEMCPY(p_served_nr_cell_info->broadcast_plmn_info.
                 plmn_identity[num_bplmn].plmn_id,
                 p_served_nr_cell_Info_src->broadcastPLMNs.elem[num_bplmn].data,
                 p_served_nr_cell_Info_src->broadcastPLMNs.elem[num_bplmn].numocts);
     }

     switch( p_served_nr_cell_Info_src->nrModeInfo.t )
    {
        case T_x2ap_ServedNRCell_Information_nrModeInfo_fdd:
        {
              p_fdd_served_nr_cell_info = p_served_nr_cell_Info_src->nrModeInfo.u.fdd;

              if ( X2AP_P_NULL == p_fdd_served_nr_cell_info )
              {
                  X2AP_TRACE(X2AP_ERROR,"%s: ""p_fdd_served_nr_cell_info is NULL",
                          __FUNCTION__);
                  retVal = X2AP_FAILURE;
                  break;
              }

              p_served_nr_cell_info->choice_nr_mode.bitmask |=
                 X2AP_NR_MODE_FDD_PRESENT;

              p_served_nr_cell_info->choice_nr_mode.nr_fdd_info.
                 nr_ul_freq_info.nr_earfcn =
                 p_fdd_served_nr_cell_info->ul_NRFreqInfo.nRARFCN;
              p_served_nr_cell_info->choice_nr_mode.nr_fdd_info.
                 nr_dl_freq_info.nr_earfcn =
                 p_fdd_served_nr_cell_info->dl_NRFreqInfo.nRARFCN;

              {
                 p_node_1 = p_fdd_served_nr_cell_info->dl_NRFreqInfo.freqBandListNr.head;

                 p_served_nr_cell_info->choice_nr_mode.nr_fdd_info.
                     nr_dl_freq_info.num_freq_bands =
                     p_fdd_served_nr_cell_info->dl_NRFreqInfo.freqBandListNr.count;

                 for ( freq_count = X2AP_NULL; freq_count < p_fdd_served_nr_cell_info->dl_NRFreqInfo.
                         freqBandListNr.count; freq_count++ )
                 {
                    if ( !p_node_1 )
                    {
                        X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                                __FUNCTION__);
                        retVal = X2AP_FAILURE;
                        break;
                    }

                     p_served_nr_cell_info->choice_nr_mode.nr_fdd_info.
                         nr_dl_freq_info.freq_band_info[freq_count].freq_band_indicator_nr =
                         ((x2ap_FreqBandNrItem*)p_node_1->data)->freqBandIndicatorNr;

                     p_node_2 = ((x2ap_FreqBandNrItem*)p_node_1->data)->supportedSULBandList.head;

                     p_served_nr_cell_info->choice_nr_mode.nr_fdd_info.
                         nr_dl_freq_info.freq_band_info[freq_count].num_supported_sul_band =
                         ((x2ap_FreqBandNrItem*)p_node_1->data)->supportedSULBandList.count;

                     for ( sul_count = X2AP_NULL; sul_count < ((x2ap_FreqBandNrItem*)p_node_1->data)->
                             supportedSULBandList.count; sul_count++ )
                     {
                         if ( !p_node_2 )
                         {
                             X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                                     __FUNCTION__);
                             retVal = X2AP_FAILURE;
                             break;
                         }

                         p_served_nr_cell_info->choice_nr_mode.nr_fdd_info.
                             nr_dl_freq_info.freq_band_info[freq_count].supported_sul_freq_band_indicator_nr[sul_count] =
                             ((x2ap_SupportedSULFreqBandItem*)p_node_2->data)->freqBandIndicatorNr;

                         p_node_2 = p_node_2->next;
                     }
                     p_node_1 = p_node_1->next;
                }
            }
            {
                p_node_3 = p_fdd_served_nr_cell_info->ul_NRFreqInfo.freqBandListNr.head;

                p_served_nr_cell_info->choice_nr_mode.nr_fdd_info.
                    nr_ul_freq_info.num_freq_bands =
                    p_fdd_served_nr_cell_info->ul_NRFreqInfo.freqBandListNr.count;

                for ( freq_count = X2AP_NULL; freq_count < p_fdd_served_nr_cell_info->ul_NRFreqInfo.
                        freqBandListNr.count; freq_count++ )
                {
                    if ( !p_node_3 )
                    {
                        X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                                __FUNCTION__);
                        retVal = X2AP_FAILURE;
                           break;
                    }

                    p_served_nr_cell_info->choice_nr_mode.nr_fdd_info.
                       nr_ul_freq_info.freq_band_info[freq_count].freq_band_indicator_nr =
                       ((x2ap_FreqBandNrItem*)p_node_3->data)->freqBandIndicatorNr;

                    p_node_4 = ((x2ap_FreqBandNrItem*)p_node_3->data)->supportedSULBandList.head;

                    p_served_nr_cell_info->choice_nr_mode.nr_fdd_info.
                       nr_ul_freq_info.freq_band_info[freq_count].num_supported_sul_band =
                       ((x2ap_FreqBandNrItem*)p_node_3->data)->supportedSULBandList.count;

                    for ( sul_count = X2AP_NULL; sul_count < ((x2ap_FreqBandNrItem*)p_node_3->data)->
                            supportedSULBandList.count; sul_count++ )
                    {
                        if ( !p_node_4 )
                        {
                            X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                                    __FUNCTION__);
                            retVal = X2AP_FAILURE;
                            break;
                        }

                        p_served_nr_cell_info->choice_nr_mode.nr_fdd_info.
                            nr_ul_freq_info.freq_band_info[freq_count].supported_sul_freq_band_indicator_nr[sul_count] =
                            ((x2ap_SupportedSULFreqBandItem*)p_node_4->data)->freqBandIndicatorNr;

                        p_node_4 = p_node_4->next;
                    }
                    p_node_3 = p_node_3->next;
               }
            }

            p_served_nr_cell_info->choice_nr_mode.nr_fdd_info.
                nr_ul_tx_bdwidth.nr_scs =
                p_fdd_served_nr_cell_info->ul_NR_TxBW.nRSCS;
            p_served_nr_cell_info->choice_nr_mode.nr_fdd_info.
                nr_ul_tx_bdwidth.nr_nrb =
                p_fdd_served_nr_cell_info->ul_NR_TxBW.nRNRB;

            p_served_nr_cell_info->choice_nr_mode.nr_fdd_info.
                nr_dl_tx_bdwidth.nr_scs =
                p_fdd_served_nr_cell_info->dl_NR_TxBW.nRSCS;
            p_served_nr_cell_info->choice_nr_mode.nr_fdd_info.
                nr_dl_tx_bdwidth.nr_nrb =
                p_fdd_served_nr_cell_info->dl_NR_TxBW.nRNRB;

        }
        break;

        case T_x2ap_ServedNRCell_Information_nrModeInfo_tdd:
        {
            p_tdd_served_nr_cell_info = p_served_nr_cell_Info_src->nrModeInfo.u.tdd;

            if ( X2AP_P_NULL == p_tdd_served_nr_cell_info )
            {
                X2AP_TRACE(X2AP_ERROR,"%s: p_tdd_served_nr_cell_info is NULL",
                        __FUNCTION__);
                retVal = X2AP_FAILURE;
                break;
            }

            p_served_nr_cell_info->choice_nr_mode.bitmask |=
                X2AP_NR_MODE_TDD_PRESENT;

            p_served_nr_cell_info->choice_nr_mode.nr_tdd_info.nr_freq_info.nr_earfcn =
                p_tdd_served_nr_cell_info->nRFreqInfo.nRARFCN;

            {
                p_served_nr_cell_info->choice_nr_mode.nr_tdd_info.
                    nr_freq_info.num_freq_bands =
                     p_tdd_served_nr_cell_info->nRFreqInfo.freqBandListNr.count;

                if(RRC_ZERO != p_tdd_served_nr_cell_info->nRFreqInfo.freqBandListNr.count)
               {
                    p_node_1 = p_tdd_served_nr_cell_info->nRFreqInfo.freqBandListNr.head;

                    for ( freq_count = X2AP_NULL; freq_count < p_tdd_served_nr_cell_info->nRFreqInfo.
                            freqBandListNr.count; freq_count++ )
                    {
                        if ( !p_node_1 )
                        {
                            X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                                    __FUNCTION__);
                            retVal = X2AP_FAILURE;
                            break;
                        }

                        p_served_nr_cell_info->choice_nr_mode.nr_tdd_info.
                            nr_freq_info.freq_band_info[freq_count].freq_band_indicator_nr =
                            ((x2ap_FreqBandNrItem*)p_node_1->data)->freqBandIndicatorNr;

                        p_served_nr_cell_info->choice_nr_mode.nr_tdd_info.
                            nr_freq_info.freq_band_info[freq_count].num_supported_sul_band =
                            ((x2ap_FreqBandNrItem*)p_node_1->data)->supportedSULBandList.count;

                        if(RRC_ZERO !=
                                ((x2ap_FreqBandNrItem*)p_node_1->data)->supportedSULBandList.count)
                        {
                            p_node_2 = ((x2ap_FreqBandNrItem*)p_node_1->data)->supportedSULBandList.head;

                            for ( sul_count = X2AP_NULL; sul_count < ((x2ap_FreqBandNrItem*)p_node_1->data)->
                                    supportedSULBandList.count; sul_count++ )
                            {
                                if ( !p_node_2 )
                                {
                                    X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                                            __FUNCTION__);
                                    retVal = X2AP_FAILURE;
                                    break;
                                }
                                /* coverity fix 277503 */
                                p_served_nr_cell_info->choice_nr_mode.nr_tdd_info.
                                    nr_freq_info.freq_band_info[freq_count].supported_sul_freq_band_indicator_nr[sul_count] =
                                    ((x2ap_SupportedSULFreqBandItem*)p_node_2->data)->freqBandIndicatorNr;
                                /* coverity fix 277503 */

                                p_node_2 = p_node_2->next;
                            }
                        }
                        p_node_1 = p_node_1->next;
                    }
                }
            }

            p_served_nr_cell_info->choice_nr_mode.nr_tdd_info.
                nr_tx_bandwidth.nr_scs = p_tdd_served_nr_cell_info->nR_TxBW.nRSCS;
            p_served_nr_cell_info->choice_nr_mode.nr_tdd_info.
                nr_tx_bandwidth.nr_nrb = p_tdd_served_nr_cell_info->nR_TxBW.nRNRB;
        }
        break;

        default:
        {
           X2AP_TRACE(X2AP_ERROR,"%s: Eutran mode is not present",
                   __FUNCTION__);
           *p_send_tsc = X2AP_TRUE;
           retVal = X2AP_FAILURE;
           break;
        }
    }

#ifndef LINUX_PC_TEST
            if ( X2AP_FAILURE == x2ap_internal_decode_meas_timing_config(
                        &(p_served_nr_cell_Info_src->measurementTimingConfiguration),
                        &(p_served_nr_cell_info->meas_timing_config),
                        p_asn1_ctx) )
            {
                X2AP_TRACE(X2AP_DETAILED,"%s: Decoding of Measurement timing config failed",
                        __FUNCTION__);
                X2AP_UT_TRACE_EXIT();        
                return X2AP_FAILURE;
            }
#endif
    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/* MENB CHANGES - START */
/*******************************************************************************
 * Function Name  : x2ap_parse_srerved_nr_cells_endc_x2_management_list
 * Inputs         : x2ap_nr_cell_info_t                    *p_nr_cell_info 
 *                  x2ap_ServedNRcellsENDCX2ManagementList *p_asn_server_nr_cells
 *                  x2ap_message_data_t                    *p_ie_order_map
 *                  U8                                     order_index
 *                  U8                                     id
 *                  x2ap_bool_et                           *p_send_tse
 *                  OSCTXT                                 *p_asn1_ctx 
 * Outputs        : x2ap_srvd_nr_cell_info_t               *p_nr_served_cell[]
 *                  
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode served NR cells
 *******************************************************************************/
x2ap_return_et
x2ap_parse_srerved_nr_cells_endc_x2_management_list
(
    x2ap_nr_cell_info_t                     *p_nr_cell_info,
    x2ap_ServedNRcellsENDCX2ManagementList  *p_asn_server_nr_cells,
    x2ap_message_data_t                     *p_ie_order_map,
    U8                                      order_index,
    U8                                      id,
    x2ap_bool_et                            *p_send_tse,
    OSCTXT                                  *p_asn1_ctx
)
{
    x2ap_return_et                  retVal = X2AP_SUCCESS;
    x2ap_bool_et                    send_tsc = X2AP_FALSE; 
    OSRTDListNode                   *p_node = X2AP_P_NULL;
    U8                              nr_cell_count = X2AP_NULL;

    X2AP_UT_TRACE_ENTER();

    p_node = p_asn_server_nr_cells->head;
    p_nr_cell_info->num_served_nr_cell = p_asn_server_nr_cells->count;

    if ( MAX_SERVED_CELLS <  p_nr_cell_info->num_served_nr_cell )
    {
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    for ( nr_cell_count = X2AP_NULL; nr_cell_count <  p_nr_cell_info->num_served_nr_cell;
        nr_cell_count++ )
    {
        if ( !p_node )
        {
            if(p_ie_order_map)
            {
                x2ap_update_message_map(p_ie_order_map,
                        DATA_MISSING, order_index, id);
            }
            X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                       __FUNCTION__);
            retVal = X2AP_FAILURE;
            break;
        }

        /*Populate servedNRCellInfo*/
        retVal = x2ap_parse_nr_svrd_info_endc_x2_management_list(
                    (&((x2ap_ServedNRcellsENDCX2ManagementList_element*)p_node->data)->
                     servedNRCellInfo),
                     &p_nr_cell_info->served_nr_cell_arr[nr_cell_count],
                    &send_tsc,
                    p_asn1_ctx);
                    
        if (X2AP_FAILURE == retVal && X2AP_TRUE == send_tsc)
        {
            if(p_ie_order_map)
            {
               x2ap_update_message_map(p_ie_order_map,
                       DATA_MISSING, order_index, id);
            }
        }   

        send_tsc = X2AP_FALSE;

        /* populate nRNeighbourInfo */
        if(RRC_ONE ==
                (((x2ap_ServedNRcellsENDCX2ManagementList_element*)p_node->data)->m.
                 nRNeighbourInfoPresent))
        {
            p_nr_cell_info->bitmask |= NR_NEIGHBOUR_INFO_PRESENT;
            x2ap_parse_nr_neigh_info_endc_x2_management_list(
                    (&((x2ap_ServedNRcellsENDCX2ManagementList_element*)p_node->data)->
                     nRNeighbourInfo),
                    p_nr_cell_info,&send_tsc);
        }
        p_node = p_node->next;
    }

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/*******************************************************************************
 * Function Name  : x2ap_parse_nr_neigh_info_endc_x2_management_list
 * Inputs         : x2ap_NRNeighbour_Information             *p_nr_neigh_info_src,
 *                  x2ap_nr_cell_info_t                      *p_nr_cell_info
 *
 * Outputs        : x2ap_nr_cell_info_t                       *p_nr_cell_info
 *
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode NR Neighbour information
 *******************************************************************************/
x2ap_return_et
x2ap_parse_nr_neigh_info_endc_x2_management_list
(
     x2ap_NRNeighbour_Information             *p_nr_neigh_info_src,
     x2ap_nr_cell_info_t                      *p_nr_cell_info,
     x2ap_bool_et                             *p_send_tsc
)
{
    x2ap_return_et                  retVal = X2AP_SUCCESS;
    OSRTDListNode                   *p_node = X2AP_P_NULL;
    OSRTDListNode                   *p_node_1 = X2AP_P_NULL;
    OSRTDListNode                   *p_node_2 = X2AP_P_NULL;
    OSRTDListNode                   *p_node_3 = X2AP_P_NULL;
    OSRTDListNode                   *p_node_4 = X2AP_P_NULL;
    OSRTDListNode                   *p_node_5 = X2AP_P_NULL;
    OSRTDListNode                   *p_node_6 = X2AP_P_NULL;
    U8                              nr_cell_count = X2AP_NULL;
    U8                              freq_count = X2AP_NULL;
    U8                              sul_count = X2AP_NULL;

    x2ap_NRFreqInfo_freqBandListNr          *p_nr_freq_band_list = X2AP_P_NULL;
    x2ap_FreqBandNrItem_supportedSULBandList 
                                            *p_nr_freq_sul_band = X2AP_P_NULL;
    x2ap_FDD_InfoNeighbourServedNRCell_Information
                                            *p_fdd_served_nr_cell_info = X2AP_P_NULL;
    x2ap_TDD_InfoNeighbourServedNRCell_Information
                                            *p_tdd_served_nr_cell_info = X2AP_P_NULL;

    x2ap_nr_neigh_info_t                    *p_nr_neigh_info = X2AP_P_NULL; 
    
    x2ap_NRNeighbour_Information_element    *p_NRNeighbour_Info_ele = X2AP_P_NULL;
    x2ap_FreqBandNrItem                     *p_Freq_Band_Nr_Ite = X2AP_P_NULL;
    x2ap_SupportedSULFreqBandItem           *p_Supp_SUL_Freq_band = X2AP_P_NULL;

    X2AP_UT_TRACE_ENTER();
     
    p_node = p_nr_neigh_info_src->head;
    p_nr_cell_info->num_neigh_cell = p_nr_neigh_info_src->count;
    
    if ( ASN1V_x2ap_maxofNRNeighbours < p_nr_cell_info->num_neigh_cell)
    {
        X2AP_UT_TRACE_EXIT();
        *p_send_tsc = X2AP_TRUE;
        return X2AP_FAILURE;
    }
        
    if ( MAX_SERVED_CELLS < p_nr_cell_info->num_neigh_cell)
    {
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

     for ( nr_cell_count = X2AP_NULL; nr_cell_count < p_nr_cell_info->num_neigh_cell;
                     nr_cell_count++ )
     {
         if ( !p_node )
         {
             X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                     __FUNCTION__);
             retVal = X2AP_FAILURE;
             break;
         }

         p_nr_neigh_info = &(p_nr_cell_info->nr_neigh_cell_arr[nr_cell_count].nr_neigh_info);

         p_NRNeighbour_Info_ele = (x2ap_NRNeighbour_Information_element*)p_node->data;
         /*filling nr_pci*/
         p_nr_neigh_info->nr_pci = p_NRNeighbour_Info_ele->nrpCI;

         /* filling nr_cell_id */
         X2AP_MEMCPY(p_nr_neigh_info-> nr_cell_id.plmn_identity.plmn_id,
                 p_NRNeighbour_Info_ele->nrCellID.pLMN_Identity.data,
                 p_NRNeighbour_Info_ele->nrCellID.pLMN_Identity.numocts);

         X2AP_MEMCPY(p_nr_neigh_info->nr_cell_id.nr_cell_id,
                 p_NRNeighbour_Info_ele->nrCellID.nRcellIdentifier.data,
                 NR_CELL_ID_OCTET_SIZE);


         /*NR Frequency Info*/
         p_nr_neigh_info->nr_freq_info.nr_earfcn =
             p_NRNeighbour_Info_ele->nrFreqInfo.nRARFCN;

           if(RRC_ONE ==
                        (p_NRNeighbour_Info_ele->nrFreqInfo.m.nRARFCN_SULPresent))
           {
                 p_nr_neigh_info->nr_freq_info.bitmask |= X2AP_NR_ARFCN_SUL_PRESENT; 

                 p_nr_neigh_info->nr_freq_info.nr_earfcn_sul =
                     p_NRNeighbour_Info_ele->nrFreqInfo.nRARFCN_SUL;
           }

           if (RRC_ONE ==
                         (p_NRNeighbour_Info_ele->nrFreqInfo.m.sULInformationPresent))
           {
               p_nr_neigh_info->nr_freq_info.bitmask |= X2AP_SUL_INFORMATION_PRESENT;

               p_nr_neigh_info->nr_freq_info.nr_sul_information.nr_sul_arfcn =
                   p_NRNeighbour_Info_ele->nrFreqInfo.sULInformation.sUL_ARFCN;

               p_nr_neigh_info->nr_freq_info.nr_sul_information.nr_tx_band_width.nr_scs =
                                      p_NRNeighbour_Info_ele->nrFreqInfo.sULInformation.sUL_TxBW.nRSCS;

               p_nr_neigh_info->nr_freq_info.nr_sul_information.nr_tx_band_width.nr_nrb =
                                      p_NRNeighbour_Info_ele->nrFreqInfo.sULInformation.sUL_TxBW.nRNRB;
           }
           
           /*x2ap_nr_freq_band_info_t filling from  x2ap_NRFreqInfo_freqBandListNr*/
           p_nr_freq_band_list = &(p_NRNeighbour_Info_ele->nrFreqInfo.freqBandListNr);

           p_node_1 = p_nr_freq_band_list->head;
           p_nr_neigh_info->nr_freq_info.num_freq_bands = p_nr_freq_band_list->count;

           for ( freq_count = X2AP_NULL; freq_count <
                       p_nr_neigh_info->nr_freq_info.num_freq_bands;
                            freq_count++)
           {
               if ( !p_node_1 )
               {
                   X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                           __FUNCTION__);
                   retVal = X2AP_FAILURE;
                   break;
               }
               
               p_Freq_Band_Nr_Ite = (x2ap_FreqBandNrItem*)p_node_1->data;

               p_nr_neigh_info->nr_freq_info.freq_band_info[freq_count].freq_band_indicator_nr =
                   p_Freq_Band_Nr_Ite->freqBandIndicatorNr;

               p_nr_freq_sul_band = &(p_Freq_Band_Nr_Ite->supportedSULBandList);
               p_node_2 = p_nr_freq_sul_band->head;
               p_nr_neigh_info-> nr_freq_info.freq_band_info[freq_count].num_supported_sul_band = 
                   p_nr_freq_sul_band->count;

               for ( sul_count = X2AP_NULL; sul_count <
                     p_nr_neigh_info->nr_freq_info.freq_band_info[freq_count].num_supported_sul_band;
                     sul_count++ ) 
               {
                   if ( !p_node_2 )
                   {
                       X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                               __FUNCTION__);
                       retVal = X2AP_FAILURE;
                       break;
                   }
                 p_Supp_SUL_Freq_band =  (x2ap_SupportedSULFreqBandItem*)p_node_2->data;
                 p_nr_neigh_info->nr_freq_info.freq_band_info[freq_count].
                     supported_sul_freq_band_indicator_nr[sul_count] = 
                       p_Supp_SUL_Freq_band->freqBandIndicatorNr;

                   p_node_2 = p_node_2->next;
               }
               p_node_1 = p_node_1->next;
           }

           /*filling fivegs_tac and configured_tac*/
           if(RRC_ONE == p_NRNeighbour_Info_ele->m.fiveGS_TACPresent)
           {
               p_nr_neigh_info->bitmask |= X2AP_FIVE_GS_TAC_PRESENT;

               X2AP_MEMCPY(p_nr_neigh_info->fivegs_tac.tac,
                       p_NRNeighbour_Info_ele->fiveGS_TAC.data,
                       p_NRNeighbour_Info_ele->fiveGS_TAC.numocts);
           }
           if(RRC_ONE == p_NRNeighbour_Info_ele->m.configured_TACPresent)
           {

               p_nr_neigh_info->bitmask |= X2AP_CONFIGURED_TAC_PRESENT;
               X2AP_MEMCPY(p_nr_neigh_info->configured_tac.tac,
                       p_NRNeighbour_Info_ele->configured_TAC.data,
                       p_NRNeighbour_Info_ele->configured_TAC.numocts);
           }

           /*Filling nr_neigh_mode info */
           switch( p_NRNeighbour_Info_ele->nRNeighbourModeInfo.t )
           {
               case T_x2ap_NRNeighbour_Information_element_nRNeighbourModeInfo_fdd:
               {
                   p_fdd_served_nr_cell_info = p_NRNeighbour_Info_ele->nRNeighbourModeInfo.u.fdd;

                   if ( X2AP_P_NULL == p_fdd_served_nr_cell_info )
                   {
                        X2AP_TRACE(X2AP_ERROR,"%s: ""p_fdd_served_nr_cell_info is NULL",
                               __FUNCTION__);
                       retVal = X2AP_FAILURE;
                       break;
                   }
                   /*filling Bitmask */
                   p_nr_neigh_info->nr_neigh_Mode_Info.bitmask |=
                       X2AP_NR_NEIGHBOUR_FDD_PRESENT;

                   /* filling nr_earfcn ul_nr_freq_Info */
                   p_nr_neigh_info->nr_neigh_Mode_Info.fdd_info.ul_nr_freq_Info.nr_earfcn =
                       p_fdd_served_nr_cell_info->ul_NRFreqInfo.nRARFCN;

                   /*filling nr_earfcn for dl_nr_freq_Info */
                   p_nr_neigh_info->nr_neigh_Mode_Info.fdd_info.dl_nr_freq_Info.nr_earfcn =
                       p_fdd_served_nr_cell_info->dl_NRFreqInfo.nRARFCN;

                   /*dl_nr_frequency_info*/
                   p_node_3 = p_fdd_served_nr_cell_info->dl_NRFreqInfo.freqBandListNr.head;
                   p_nr_neigh_info->nr_neigh_Mode_Info.fdd_info.dl_nr_freq_Info.num_freq_bands = 
                       p_fdd_served_nr_cell_info->dl_NRFreqInfo.freqBandListNr.count;

                   for ( freq_count = X2AP_NULL; freq_count <
                           p_nr_neigh_info->nr_neigh_Mode_Info.fdd_info.dl_nr_freq_Info.num_freq_bands;
                           freq_count++)
                   {
                       if ( !p_node_3 )
                       {
                           X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                                   __FUNCTION__);
                           retVal = X2AP_FAILURE;
                           break;
                       }
                       /*filling freq_band_indicator */
                       p_Freq_Band_Nr_Ite = (x2ap_FreqBandNrItem*)p_node_3->data;
                       p_nr_neigh_info->nr_neigh_Mode_Info.fdd_info.
                           dl_nr_freq_Info.freq_band_info[freq_count].freq_band_indicator_nr =
                           p_Freq_Band_Nr_Ite->freqBandIndicatorNr;

                       /*filling num_supported_sul_band */
                       p_node_4 = p_Freq_Band_Nr_Ite->supportedSULBandList.head;
                       p_nr_neigh_info->nr_neigh_Mode_Info.fdd_info.dl_nr_freq_Info.
                           freq_band_info[freq_count].num_supported_sul_band =
                           p_Freq_Band_Nr_Ite->supportedSULBandList.count;

                       for ( sul_count = X2AP_NULL; sul_count < p_nr_neigh_info->nr_neigh_Mode_Info.
                               fdd_info.dl_nr_freq_Info.freq_band_info[freq_count].
                                                 num_supported_sul_band;sul_count++ )
                       {
                           if ( !p_node_4 )
                           {
                               X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                                       __FUNCTION__);
                               retVal = X2AP_FAILURE;
                               break;
                           }
                           /*filling supported_sul_freq_band_indicator_info*/
                           p_Supp_SUL_Freq_band = (x2ap_SupportedSULFreqBandItem*)p_node_4->data;
                           p_nr_neigh_info->nr_neigh_Mode_Info.fdd_info.dl_nr_freq_Info.
                               freq_band_info[freq_count].
                               supported_sul_freq_band_indicator_nr[sul_count] =
                               p_Supp_SUL_Freq_band->freqBandIndicatorNr;

                           p_node_4 = p_node_4->next;
                       }
                       p_node_3 = p_node_3->next;
                   }
                   
                   /*filling nr_earfcn_sul,nr_sul_arfcn, nr_scs, nr_nrb */
                   if(RRC_ONE == p_fdd_served_nr_cell_info->dl_NRFreqInfo.m.nRARFCN_SULPresent)
                   {
                       p_nr_neigh_info->nr_neigh_Mode_Info.fdd_info.dl_nr_freq_Info.bitmask |=
                           X2AP_NR_ARFCN_SUL_PRESENT;

                       p_nr_neigh_info->nr_neigh_Mode_Info.fdd_info.dl_nr_freq_Info.nr_earfcn_sul =
                           p_fdd_served_nr_cell_info->dl_NRFreqInfo.nRARFCN_SUL;
                   }

                   if(RRC_ONE == p_fdd_served_nr_cell_info->dl_NRFreqInfo.m.sULInformationPresent)
                   {
                       p_nr_neigh_info->nr_neigh_Mode_Info.fdd_info.dl_nr_freq_Info.bitmask |=
                           X2AP_SUL_INFORMATION_PRESENT;

                       p_nr_neigh_info->nr_neigh_Mode_Info.fdd_info.dl_nr_freq_Info.nr_sul_information.
                           nr_sul_arfcn =
                           p_fdd_served_nr_cell_info->dl_NRFreqInfo.sULInformation.sUL_ARFCN;

                       p_nr_neigh_info->nr_neigh_Mode_Info.fdd_info.dl_nr_freq_Info.
                           nr_sul_information.nr_tx_band_width.nr_scs =
                           p_fdd_served_nr_cell_info->dl_NRFreqInfo.sULInformation.sUL_TxBW.nRSCS;

                       p_nr_neigh_info->nr_neigh_Mode_Info.fdd_info.dl_nr_freq_Info.
                           nr_sul_information.nr_tx_band_width.nr_nrb =
                           p_fdd_served_nr_cell_info->dl_NRFreqInfo.sULInformation.sUL_TxBW.nRNRB;
                   }

                   /*ul_nr_freq_info*/
                   p_node_5 = p_fdd_served_nr_cell_info->ul_NRFreqInfo.freqBandListNr.head;

                   p_nr_neigh_info->nr_neigh_Mode_Info.fdd_info.ul_nr_freq_Info.num_freq_bands = 
                       p_fdd_served_nr_cell_info->ul_NRFreqInfo.freqBandListNr.count;

                   for ( freq_count = X2AP_NULL; freq_count <
                           p_nr_neigh_info->nr_neigh_Mode_Info.fdd_info.ul_nr_freq_Info.num_freq_bands;
                           freq_count++)
                   {
                       if ( !p_node_5 )
                       {
                           X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                                   __FUNCTION__);
                           retVal = X2AP_FAILURE;
                           break;
                       }
                       /*filling ul freq_band_indicator_nr */
                       p_Freq_Band_Nr_Ite = (x2ap_FreqBandNrItem*)p_node_5->data;
                       p_nr_neigh_info->nr_neigh_Mode_Info.fdd_info.
                           ul_nr_freq_Info.freq_band_info[freq_count].freq_band_indicator_nr =
                           p_Freq_Band_Nr_Ite->freqBandIndicatorNr;
                       
                       /*pointing to the list of supportedSULBandList */
                       p_node_6 = p_Freq_Band_Nr_Ite->supportedSULBandList.head;
                       p_nr_neigh_info->nr_neigh_Mode_Info.fdd_info.ul_nr_freq_Info.
                               freq_band_info[freq_count].num_supported_sul_band =
                           p_Freq_Band_Nr_Ite->supportedSULBandList.count;

                       for ( sul_count = X2AP_NULL; sul_count < p_nr_neigh_info->nr_neigh_Mode_Info.
                               fdd_info.ul_nr_freq_Info.freq_band_info[freq_count].
                                           num_supported_sul_band;sul_count++ )
                       {
                           if ( !p_node_6 )
                           {
                               X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                                       __FUNCTION__);
                               retVal = X2AP_FAILURE;
                               break;
                           }
                           /*filling ul supported_sul_freq_indicator_nr*/
                           p_Supp_SUL_Freq_band = (x2ap_SupportedSULFreqBandItem*)p_node_6->data;
                           p_nr_neigh_info->nr_neigh_Mode_Info.fdd_info.ul_nr_freq_Info.
                               freq_band_info[freq_count].
                               supported_sul_freq_band_indicator_nr[sul_count] =
                               p_Supp_SUL_Freq_band->freqBandIndicatorNr;

                           p_node_6 = p_node_6->next;
                       }

                       p_node_5 = p_node_5->next;
                   }

                    /*filling nr_earfcn_sul, nr_sul_arfcn, nr_scs, nr_nrb for ul*/
                   if(RRC_ONE == p_fdd_served_nr_cell_info->ul_NRFreqInfo.m.nRARFCN_SULPresent)
                   {
                       /*filling Bitmask */
                       p_nr_neigh_info->nr_neigh_Mode_Info.fdd_info.ul_nr_freq_Info.bitmask |=
                           X2AP_NR_ARFCN_SUL_PRESENT;

                       /*filling nr_earfcn_sul for ul*/
                       p_nr_neigh_info->nr_neigh_Mode_Info.fdd_info.ul_nr_freq_Info.nr_earfcn_sul =
                           p_fdd_served_nr_cell_info->ul_NRFreqInfo.nRARFCN_SUL;
                   }

                   if (RRC_ONE == p_fdd_served_nr_cell_info->ul_NRFreqInfo.m.sULInformationPresent )
                   {
                       p_nr_neigh_info->nr_neigh_Mode_Info.fdd_info.ul_nr_freq_Info.bitmask |=
                           X2AP_SUL_INFORMATION_PRESENT;

                       p_nr_neigh_info->nr_neigh_Mode_Info.fdd_info.ul_nr_freq_Info.nr_sul_information.
                           nr_sul_arfcn =
                           p_fdd_served_nr_cell_info->ul_NRFreqInfo.sULInformation.sUL_ARFCN;

                       p_nr_neigh_info->nr_neigh_Mode_Info.fdd_info.ul_nr_freq_Info.
                           nr_sul_information.nr_tx_band_width.nr_scs =
                           p_fdd_served_nr_cell_info->ul_NRFreqInfo.sULInformation.sUL_TxBW.nRSCS;

                       p_nr_neigh_info->nr_neigh_Mode_Info.fdd_info.ul_nr_freq_Info.
                           nr_sul_information.nr_tx_band_width.nr_nrb =
                           p_fdd_served_nr_cell_info->ul_NRFreqInfo.sULInformation.sUL_TxBW.nRNRB;
                   }
               }    
               break;

               case T_x2ap_NRNeighbour_Information_element_nRNeighbourModeInfo_tdd:
               {
                   p_tdd_served_nr_cell_info = p_NRNeighbour_Info_ele->nRNeighbourModeInfo.u.tdd;

                   if ( X2AP_P_NULL == p_tdd_served_nr_cell_info )
                   {
                       X2AP_TRACE(X2AP_ERROR,"%s: ""p_tdd_served_nr_cell_info is NULL",
                               __FUNCTION__);
                       retVal = X2AP_FAILURE;
                       break;
                   }
               
                   /*filling the bit mask for tdd*/
                   p_nr_neigh_info->nr_neigh_Mode_Info.bitmask |=
                        X2AP_NR_NEIGHBOUR_TDD_PRESENT;

                   /*filling nr_earfcn */
                   p_nr_neigh_info->nr_neigh_Mode_Info.tdd_info.nr_freq_info.nr_earfcn =
                       p_tdd_served_nr_cell_info->nRFreqInfo.nRARFCN;

                   /*nRFreqInfo*/
                   p_node_3 = p_tdd_served_nr_cell_info->nRFreqInfo.freqBandListNr.head;
                   p_nr_neigh_info->nr_neigh_Mode_Info.tdd_info.nr_freq_info.num_freq_bands = 
                       p_tdd_served_nr_cell_info->nRFreqInfo.freqBandListNr.count;

                   for ( freq_count = X2AP_NULL; freq_count <
                           p_nr_neigh_info->nr_neigh_Mode_Info.tdd_info.nr_freq_info.num_freq_bands;
                           freq_count++)
                   {
                       if ( !p_node_3 )
                       {
                           X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                                   __FUNCTION__);
                           retVal = X2AP_FAILURE;
                           break;
                       }
                       p_Freq_Band_Nr_Ite = (x2ap_FreqBandNrItem*)p_node_3->data; 
                       p_nr_neigh_info->nr_neigh_Mode_Info.
                           tdd_info.nr_freq_info.freq_band_info[freq_count].freq_band_indicator_nr =
                           p_Freq_Band_Nr_Ite->freqBandIndicatorNr;

                       p_node_4 = p_Freq_Band_Nr_Ite->supportedSULBandList.head;
                       p_nr_neigh_info->nr_neigh_Mode_Info.tdd_info.nr_freq_info.
                                    freq_band_info[freq_count].num_supported_sul_band =
                           p_Freq_Band_Nr_Ite->supportedSULBandList.count;

                       for ( sul_count = X2AP_NULL; sul_count < p_nr_neigh_info->nr_neigh_Mode_Info.
                                     tdd_info.nr_freq_info.freq_band_info[freq_count].
                                           num_supported_sul_band;sul_count++ )
                       {
                           if ( !p_node_4 )
                           {
                               X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                                       __FUNCTION__);
                               retVal = X2AP_FAILURE;
                               break;
                           }
                         p_Supp_SUL_Freq_band = (x2ap_SupportedSULFreqBandItem*)p_node_4->data;
                         p_nr_neigh_info->nr_neigh_Mode_Info.tdd_info.nr_freq_info.
                             freq_band_info[freq_count].
                               supported_sul_freq_band_indicator_nr[sul_count] =
                               p_Supp_SUL_Freq_band->freqBandIndicatorNr;

                           p_node_4 = p_node_4->next;
                       }

                       p_node_3 = p_node_3->next;
                   }

                   /*filling nr_earfcn_sul, nr_sul_arfcn, nr_scs, nr_nrb for Tdd nr_freq_info*/
                   if(RRC_ONE == p_tdd_served_nr_cell_info->nRFreqInfo.m.nRARFCN_SULPresent)
                   {
                       p_nr_neigh_info->nr_neigh_Mode_Info.tdd_info.nr_freq_info.bitmask |=
                           X2AP_NR_ARFCN_SUL_PRESENT;

                       p_nr_neigh_info->nr_neigh_Mode_Info.tdd_info.nr_freq_info.nr_earfcn_sul =
                           p_tdd_served_nr_cell_info->nRFreqInfo.nRARFCN_SUL;
                   }

                   if (RRC_ONE == p_tdd_served_nr_cell_info->nRFreqInfo.m.sULInformationPresent )
                   {
                       p_nr_neigh_info->nr_neigh_Mode_Info.tdd_info.nr_freq_info.bitmask |=
                           X2AP_SUL_INFORMATION_PRESENT;

                       p_nr_neigh_info->nr_neigh_Mode_Info.tdd_info.nr_freq_info.nr_sul_information.
                           nr_sul_arfcn =
                           p_tdd_served_nr_cell_info->nRFreqInfo.sULInformation.sUL_ARFCN;

                       p_nr_neigh_info->nr_neigh_Mode_Info.tdd_info.nr_freq_info.nr_sul_information.
                           nr_tx_band_width.nr_scs =
                           p_tdd_served_nr_cell_info->nRFreqInfo.sULInformation.sUL_TxBW.nRSCS;

                       p_nr_neigh_info->nr_neigh_Mode_Info.tdd_info.nr_freq_info.nr_sul_information.
                           nr_tx_band_width.nr_nrb =
                           p_tdd_served_nr_cell_info->nRFreqInfo.sULInformation.sUL_TxBW.nRNRB;
                   }
               }
               break;

               default:
               {
                   X2AP_TRACE(X2AP_ERROR,"%s: Eutran mode is not present",
                           __FUNCTION__);
                   retVal = X2AP_FAILURE;
                   break;
               }
           }
         p_node = p_node->next;
       }

      X2AP_UT_TRACE_EXIT();
      return retVal;
}

/*******************************************************************************
 * Function Name  : x2ap_parse_srerved_nr_cells_endc_x2_modify_list
 * Inputs         : x2ap_nr_srvd_cell_to_modify_t                    *p_nr_cell_info 
 *                  x2ap_ServedNRcellsToModifyENDCConfUpdList *p_asn_server_nr_cells
 *                  x2ap_message_data_t                    *p_ie_order_map
 *                  U8                                     order_index
 *                  U8                                     id
 *                  x2ap_bool_et                           *p_send_tse
 *                  OSCTXT                                 *p_asn1_ctx 
 * Outputs        : x2ap_nr_srvd_cell_to_modify_t               *p_nr_cell_info
 *                  
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode served NR cells
 *******************************************************************************/
x2ap_return_et
x2ap_parse_srerved_nr_cells_endc_x2_modify_list
(
    x2ap_nr_srvd_cell_to_modify_t                     *p_nr_cell_info,
    x2ap_ServedNRcellsToModifyENDCConfUpdList  *p_asn_server_nr_cells,
    x2ap_message_data_t                     *p_ie_order_map,
    U8                                      order_index,
    U8                                      id,
    x2ap_bool_et                            *p_send_tse,
    OSCTXT                                  *p_asn1_ctx
)
{
    x2ap_return_et                  retVal = X2AP_SUCCESS;
    x2ap_bool_et                    send_tsc; 
    OSRTDListNode                   *p_node = X2AP_P_NULL;
    U8                              nr_cell_count = X2AP_NULL;

    X2AP_UT_TRACE_ENTER();

    p_node = p_asn_server_nr_cells->head;
    p_nr_cell_info->num_nr_srvd_cell_modify = p_asn_server_nr_cells->count;

    if ( MAX_SERVED_CELLS <  p_nr_cell_info->num_nr_srvd_cell_modify )
    {
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    for ( nr_cell_count = X2AP_NULL; nr_cell_count <  p_nr_cell_info->num_nr_srvd_cell_modify;
        nr_cell_count++ )
    {
        if ( !p_node )
        {
            if(p_ie_order_map)
            {
                x2ap_update_message_map(p_ie_order_map,
                        DATA_MISSING, order_index, id);
            }
            X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                       __FUNCTION__);
            retVal = X2AP_FAILURE;
            break;
        }

        /*Populate servedNRCellInfo*/
        x2ap_parse_nr_svrd_info_endc_x2_management_list(
                    (&((x2ap_ServedNRCellsToModify_Item*)p_node->data)->
                     servedNRCellInformation),
                    &p_nr_cell_info->nr_cell_infos[nr_cell_count].srvd_nr_cell_info,
                    &send_tsc,
                    p_asn1_ctx);
                    
        if (X2AP_TRUE == send_tsc)
        {
            if(p_ie_order_map)
            {
               x2ap_update_message_map(p_ie_order_map,
                       DATA_MISSING, order_index, id);
            }
        }   

        //send_tsc = X2AP_FALSE;

        /* populate nRNeighbourInfo */
        /*if(RRC_ONE ==
                (((x2ap_ServedNRCellsToModify_Item*)p_node->data)->m.
                 nrNeighbourInformationPresent))
        {
            p_nr_cell_info->nr_cell_infos[nr_cell_count].bitmask |= NR_NEIGHBOUR_INFO_PRESENT;
            x2ap_parse_nr_neigh_info_endc_x2_management_list(
                    (&((x2ap_ServedNRCellsToModify_Item*)p_node->data)->
                     nrNeighbourInformation),
                    &p_nr_cell_info->eutra_nr_cell_info,&send_tsc);
        }*/

        /* populate nr_cgi */
        X2AP_MEMCPY(p_nr_cell_info->nr_cell_infos[nr_cell_count].old_nr_cgi.plmn_identity.plmn_id,
                    ((x2ap_ServedNRCellsToModify_Item*)p_node->data)->old_nrcgi.pLMN_Identity.data,
                    ((x2ap_ServedNRCellsToModify_Item*)p_node->data)->old_nrcgi.pLMN_Identity.numocts);

       X2AP_MEMCPY(p_nr_cell_info->nr_cell_infos[nr_cell_count].old_nr_cgi.nr_cell_id,
               ((x2ap_ServedNRCellsToModify_Item*)p_node->data)->
               old_nrcgi.nRcellIdentifier.data,
               NR_CELL_ID_OCTET_SIZE);

        /* populate deact ind */
        if (RRC_ONE == ((x2ap_ServedNRCellsToModify_Item*)p_node->data)->m.nrDeactivationIndicationPresent)
        {
            p_nr_cell_info->nr_cell_infos[nr_cell_count].bitmask |= X2AP_ENDC_X2_NR_DEACT_IND_PRESENT;
            p_nr_cell_info->nr_cell_infos[nr_cell_count].deact_ind 
                = (x2ap_deact_et)((x2ap_ServedNRCellsToModify_Item*)p_node->data)->
                            nrDeactivationIndication;
        }
               
        p_node = p_node->next;
    }            

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/*******************************************************************************
 * Function Name  : x2ap_parse_srerved_nr_cells_endc_x2_delete_list
 * Inputs         : x2ap_nr_srvd_cell_to_delete_t                    *p_nr_cell_info 
 *                  x2ap_ServedNRcellsToDeleteENDCConfUpdList *p_asn_server_nr_cells
 *                  x2ap_message_data_t                    *p_ie_order_map
 *                  U8                                     order_index
 *                  U8                                     id
 *                  x2ap_bool_et                           *p_send_tse
 *                  OSCTXT                                 *p_asn1_ctx 
 * Outputs        : x2ap_nr_srvd_cell_to_delete_t               *p_nr_cell_info
 *                  
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode deleted served NR cells
 *******************************************************************************/
x2ap_return_et
x2ap_parse_srerved_nr_cells_endc_x2_delete_list
(
    x2ap_nr_srvd_cell_to_delete_t                     *p_nr_cell_info,
    x2ap_ServedNRcellsToDeleteENDCConfUpdList  *p_asn_server_nr_cells,
    x2ap_message_data_t                     *p_ie_order_map,
    U8                                      order_index,
    U8                                      id,
    x2ap_bool_et                            *p_send_tse,
    OSCTXT                                  *p_asn1_ctx
)
{
    x2ap_return_et                  retVal = X2AP_SUCCESS;
    OSRTDListNode                   *p_node = X2AP_P_NULL;
    U8                              nr_cell_count = X2AP_NULL;

    X2AP_UT_TRACE_ENTER();

    p_node = p_asn_server_nr_cells->head;
    p_nr_cell_info->num_delete_nrcells_endc = p_asn_server_nr_cells->count;

    if ( MAX_SERVED_CELLS <  p_nr_cell_info->num_delete_nrcells_endc )
    {
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    for ( nr_cell_count = X2AP_NULL; nr_cell_count <  p_nr_cell_info->num_delete_nrcells_endc;
        nr_cell_count++ )
    {
        if ( !p_node )
        {
            if(p_ie_order_map)
            {
                x2ap_update_message_map(p_ie_order_map,
                        DATA_MISSING, order_index, id);
            }
            X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                       __FUNCTION__);
            retVal = X2AP_FAILURE;
            break;
        }
        
        X2AP_MEMCPY(p_nr_cell_info->eutra_nr_cell_delete[nr_cell_count].plmn_identity.plmn_id,
                ((x2ap_NRCGI*)p_node->data)->pLMN_Identity.data, 
                ((x2ap_NRCGI*)p_node->data)->pLMN_Identity.numocts); 

        X2AP_MEMCPY(p_nr_cell_info->eutra_nr_cell_delete[nr_cell_count].nr_cell_id,
                ((x2ap_NRCGI*)p_node->data)->nRcellIdentifier.data, 
                NR_CELL_ID_OCTET_SIZE); 
        
        p_node = p_node->next;
    }    

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_parse_GlobalGNB_ID
 * Inputs         : x2_gb_gnb_id_t              *p_glb_en_gnbId
 *                  x2ap_GlobalGNB_ID           *p_asn_en_gnb_id
 *                  x2ap_message_data_t         *p_ie_order_map
 *                  U8                          order_index
 *                  U8                          id
 * Outputs        : x2_gb_gnb_id_t              *p_glb_en_gnbId
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode Global GNB ID
 ********************************************************************************/
x2ap_return_et
    x2ap_parse_GlobalGNB_ID
(
 x2_gb_gnb_id_t              *p_glb_en_gnbId,
 x2ap_GlobalGNB_ID           *p_asn_en_gnb_id,
 x2ap_message_data_t         *p_ie_order_map,
 U8                          order_index,
 U8                          id
 )
{
    x2ap_return_et    retVal = X2AP_SUCCESS;

    X2AP_UT_TRACE_ENTER();

    X2AP_MEMCPY(p_glb_en_gnbId->plmn_identity.plmn_id,
            p_asn_en_gnb_id->pLMN_Identity.data,
            MAX_PLMN_ID_BYTES);

    if (T_x2ap_GNB_ID_gNB_ID ==
            p_asn_en_gnb_id->gNB_ID.t)
    {
        X2AP_MEMCPY(p_glb_en_gnbId->x2_en_gnb_id.x2_gnb_id,
                p_asn_en_gnb_id->gNB_ID.u.gNB_ID->data,
                EN_GNB_ID_SIZE);

    }
    else
    {
        X2AP_TRACE(X2AP_ERROR,"%s: ""gNB_ID is not present",
                __FUNCTION__);
        x2ap_update_message_map(p_ie_order_map, INVALID_VALUE, order_index, id);
        retVal = X2AP_FAILURE;
    }

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_decode_endc_x2setup_resp
 * Inputs         : x2ap_gb_context_t          *p_x2ap_gl_ctx
 *                  x2ap_peer_enb_context_t    *p_enb_ctx
 *                  en_dc_x2_setup_resp_t      *p_setup_resp
 *                  U8                         *p_asn_msg
 *                  U32                        *p_asn_msg_len
 *                  OSCTXT                     *p_asn1_ctx
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode EN DC X2 Setup response.
 ********************************************************************************/
x2ap_return_et
x2ap_decode_endc_x2setup_resp
(
 x2ap_gb_context_t          *p_x2ap_gl_ctx,
 x2ap_peer_enb_context_t    *p_enb_ctx,
 en_dc_x2_setup_resp_t      *p_setup_resp,
 U8                         *p_asn_msg,
 U32                        *p_asn_msg_len,
 OSCTXT                     *p_asn1_ctx
)
{
    x2ap_X2AP_PDU               x2ap_pdu;
    x2ap_error_ind_ie_list_t    iE_list;
    x2ap_error_ind_bool_t       send_err_ind;
    OSRTDListNode               *p_node         = X2AP_P_NULL;
    OSRTDListNode               *p_node_genb    = X2AP_P_NULL;
    x2ap_return_et              retVal          = X2AP_SUCCESS;
    U32                         ie_count        = X2AP_NULL;
    U32                         genb_index      = X2AP_NULL;
    U16                         ie_list_index   = X2AP_NULL;
    x2ap_ENDCX2SetupResponse    *p_x2Setup_resp = X2AP_P_NULL;
    x2ap_ENDCX2SetupResponse_protocolIEs_element  
        *p_x2ap_endc_protocol_IEs = X2AP_P_NULL;
    x2ap_RespondingNodeType_EndcX2Setup_respond_en_gNB_element
        *p_resp_genb_element = X2AP_P_NULL;
    x2ap_RespondingNodeType_EndcX2Setup_respond_en_gNB
        *p_resp_genb = X2AP_P_NULL;

    x2ap_message_data_t message_map =
    {1, X2AP_FALSE,X2AP_FALSE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_RespondingNodeType_EndcX2Setup, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
        }};

    X2AP_UT_TRACE_ENTER();

    X2AP_MEMSET(&iE_list, X2AP_NULL, sizeof(x2ap_error_ind_ie_list_t));
    X2AP_MEMSET(&send_err_ind, X2AP_FALSE, sizeof(x2ap_error_ind_bool_t));

    if ( !p_setup_resp )
    {
        X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                "p_setup_resp is NULL",__FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    /* Init ASN1 context */
    if ( 0 != rtInitContext(p_asn1_ctx) )
    {
        /* Drop message */
        X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                "context initialization failed.",__FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }
    /* Set pointer of asn buffer in asn context */
    if(RT_OK != pu_setBuffer(p_asn1_ctx, p_asn_msg, *p_asn_msg_len, TRUE))
    {
        X2AP_TRACE(X2AP_ERROR,"pu_setBuffer failed.",__FUNCTION__);
        return X2AP_FAILURE;
    }

    do
    {
        if ( X2AP_NULL != asn1PD_x2ap_X2AP_PDU(p_asn1_ctx, &x2ap_pdu) )
        {
            /* Drop message*/
            X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                    "decode failed.",__FUNCTION__);
            /* Send Transfer Syntax Error */
            x2ap_build_and_send_err_indication(p_x2ap_gl_ctx, p_enb_ctx, PNULL);
            rtxErrPrint(p_asn1_ctx);
            break;
        }
        else
        {
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
        }

        /* Check whether the message is not successful*/
        if ((T_x2ap_X2AP_PDU_successfulOutcome == x2ap_pdu.t ) &&
                (ASN1V_x2ap_id_endcX2Setup ==
                 x2ap_pdu.u.successfulOutcome->procedureCode ) &&
                ((X2AP_P_NULL != x2ap_pdu.u.successfulOutcome->value.u.endcX2Setup) &&
                 (T1x2ap__endcX2Setup == x2ap_pdu.u.successfulOutcome->value.t)))
        {
            p_x2Setup_resp = x2ap_pdu.u.successfulOutcome->value.u.endcX2Setup;
            p_node = p_x2Setup_resp->protocolIEs.head;

            for ( ie_count = X2AP_NULL; ie_count < p_x2Setup_resp->protocolIEs.count;
                    ie_count++ )
            {
                if ( !p_node )
                {
                    X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                            __FUNCTION__);
                    retVal = X2AP_FAILURE;                    
                    break;
                }

                p_x2ap_endc_protocol_IEs = 
                    (x2ap_ENDCX2SetupResponse_protocolIEs_element *)p_node->data;
                if ( PNULL == p_x2ap_endc_protocol_IEs )
                {
                    X2AP_TRACE(X2AP_ERROR,"%s: p_x2ap_endc_protocol_IEs is NULL",
                            __FUNCTION__);
                    retVal = X2AP_FAILURE;                    
                    break;
                }

                switch ( p_x2ap_endc_protocol_IEs->id )
                {
                    case ASN1V_x2ap_id_RespondingNodeType_EndcX2Setup:
                    {
                        X2AP_TRACE(X2AP_DETAILED, "ASN1V_x2ap_id_RespondingNode"
                                "Type_EndcX2Setup");

                        if ( PNULL == p_x2ap_endc_protocol_IEs->value.u.
                                _x2ap_ENDCX2SetupResponse_IEs_1 )
                        {
                            X2AP_TRACE(X2AP_ERROR,"%s: _x2ap_ENDCX2SetupResponse_IEs_1"
                                    "is NULL",
                                    __FUNCTION__);
                            retVal = X2AP_FAILURE;                    
                            break;
                        }

                        if (X2AP_SUCCESS == x2ap_validate_ie_value(&message_map,
                                    ie_count,
                                    (U8)p_x2ap_endc_protocol_IEs->id,
                                    &p_x2ap_endc_protocol_IEs->value.u.
                                    _x2ap_ENDCX2SetupResponse_IEs_1))
                        {
                            X2AP_TRACE(X2AP_DETAILED, "ASN1V_x2ap_id_RespondingNode"
                                    "Type_EndcX2Setup");
                        }
                        else
                        {
                            X2AP_TRACE(X2AP_ERROR,"%s: ASN1V_x2ap_id_RespondingNode"
                                    "Type_EndcX2Setup validation failed",
                                    __FUNCTION__);
                            retVal = X2AP_FAILURE;                    
                            break;
                        }

                        switch ( p_x2ap_endc_protocol_IEs->value.u.
                                _x2ap_ENDCX2SetupResponse_IEs_1->t )
                        {
                            case T_x2ap_RespondingNodeType_EndcX2Setup_respond_en_gNB:
                            {
                                x2ap_message_data_t message_map =
                                {2, X2AP_TRUE,X2AP_TRUE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
                                    {{0, ASN1V_x2ap_id_Globalen_gNB_ID, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
                                        {1, ASN1V_x2ap_id_ServedNRcellsENDCX2ManagementList,x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
                                    }};

                                x2ap_bool_et send_tse = X2AP_FALSE;

                                p_resp_genb = p_x2ap_endc_protocol_IEs->value.
                                    u._x2ap_ENDCX2SetupResponse_IEs_1->u.
                                    respond_en_gNB;
                                if ( PNULL == p_resp_genb )
                                {
                                    X2AP_TRACE(X2AP_ERROR,"%s: p_resp_genb "
                                            "is NULL",
                                            __FUNCTION__);
                                    retVal = X2AP_FAILURE;                    
                                    break;
                                }

                                p_node_genb = p_resp_genb->head;
                                for ( genb_index = X2AP_NULL; genb_index <
                                        p_resp_genb->count; genb_index++ )
                                {
                                    if ( !p_node_genb )
                                    {
                                        X2AP_TRACE(X2AP_ERROR,"%s: next node "
                                                "returned NULL",__FUNCTION__);
                                        retVal = X2AP_FAILURE;                    
                                        break;
                                    }

                                    p_resp_genb_element = 
                                        (x2ap_RespondingNodeType_EndcX2Setup_respond_en_gNB_element *)
                                        p_node_genb->data;
                                    if ( PNULL == p_resp_genb_element )
                                    {
                                        X2AP_TRACE(X2AP_ERROR,"%s: "
                                                "p_resp_en_gnb_element is NULL",
                                                __FUNCTION__);
                                        retVal = X2AP_FAILURE;                    
                                        break;
                                    }

                                    switch ( p_resp_genb_element->id )
                                    {
                                        case ASN1V_x2ap_id_Globalen_gNB_ID:
                                        {
                                            X2AP_TRACE(X2AP_DETAILED,
                                                    "ASN1V_x2ap_id_Globalen_gNB_ID");
                                            if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                                        &message_map,
                                                        genb_index,
                                                        (U8)p_resp_genb_element->id,
                                                        p_resp_genb_element->value.u.
                                                        _x2ap_En_gNB_ENDCX2SetupReqAckIEs_1))
                                            {

                                                if ( X2AP_FAILURE == x2ap_parse_GlobalGNB_ID(
                                                            &(p_setup_resp->nr_cell_info.
                                                                global_gnb_id),
                                                            p_resp_genb_element->value.u.
                                                            _x2ap_En_gNB_ENDCX2SetupReqAckIEs_1,
                                                            &message_map,
                                                            (U8)ie_count,
                                                            (U8)p_resp_genb_element->id) )
                                                {
                                                    X2AP_TRACE(X2AP_INFO,"%s: x2ap_parse_GlobalGNB_ID"
                                                            "failed",__FUNCTION__);
                                                    retVal = X2AP_FAILURE;                    
                                                    break;
                                                }
                                                l3_memcpy_wrapper(&(p_x2ap_gl_ctx->global_gnb_id),
                                                        &(p_setup_resp->nr_cell_info.global_gnb_id),
                                                        sizeof(x2_gb_gnb_id_t));
                                            }
                                        }
                                        break;

                                        case ASN1V_x2ap_id_ServedNRcellsENDCX2ManagementList:
                                        {
                                            X2AP_TRACE(X2AP_DETAILED,
                                                    "ASN1V_x2ap_id_ServedNRcellsENDCX2ManagementList");
                                            if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                                        &message_map,
                                                        genb_index,
                                                        (U8)p_resp_genb_element->id,
                                                        p_resp_genb_element->value.u.
                                                        _x2ap_En_gNB_ENDCX2SetupReqAckIEs_2))
                                            {
                                                if ( X2AP_FAILURE ==
                                                    x2ap_parse_srerved_nr_cells_endc_x2_management_list(
                                                            &p_setup_resp->nr_cell_info,
                                                            p_resp_genb_element->value.u.
                                                            _x2ap_En_gNB_ENDCX2SetupReqAckIEs_2,
                                                            &message_map,
                                                            (U8)ie_count,
                                                            (U8)p_resp_genb_element->id,
                                                            &send_tse,
                                                            p_asn1_ctx
                                                            ))
                                                {
                                                    X2AP_TRACE(X2AP_INFO,"%s: "
                                                            "x2ap_parse_srerved_nr_cells_endc_x2_management_list"
                                                            "failed",__FUNCTION__);
                                                    retVal = X2AP_FAILURE;                    
                                                    break;
                                                }
                                                /* This will only happen when the element list exceeds
                                                 * the max number */
                                                if (X2AP_TRUE == send_tse)
                                                {
                                                    X2AP_TRACE(X2AP_WARNING, "Error is Detected" 
                                                            "in ASN.1 Sequence, Sending Transfer Syntax Error");
                                                    x2ap_build_and_send_err_indication(p_x2ap_gl_ctx,
                                                        p_enb_ctx, PNULL);
                                                    retVal = X2AP_FAILURE;
                                                    break;
                                                }
                                            }
                                        }
                                        break;

                                        default:
                                        {
                                            /*
                                             * Class - 1 Error - Unknown IE
                                             */
                                            /*
                                             * Base on Criticality fill the value
                                             */
                                            x2ap_add_to_err_ind_ie_list(&iE_list,
                                                    p_resp_genb_element->criticality,
                                                    p_resp_genb_element->id,
                                                    &ie_list_index,
                                                    &send_err_ind,
                                                    X2AP_FALSE,
                                                    X2AP_FALSE);
                                            X2AP_TRACE(X2AP_ERROR,"%s:"\
                                                    "invalid Protocol IE id",__FUNCTION__);
                                            retVal = X2AP_FAILURE;
                                        }
                                        break;
                                    }
                                    p_node_genb = p_node_genb->next;
                                }
                                /*
                                 * Parse the map for Error Indication
                                 */
                                if ( ( X2AP_SUCCESS == x2ap_parse_message_map(
                                                p_asn1_ctx,
                                                p_x2ap_gl_ctx,
                                                p_enb_ctx, 
                                                &message_map,
                                                &iE_list,
                                                &ie_list_index,
                                                &send_err_ind,
                                                ASN1V_x2ap_id_endcX2Setup,
                                                T_x2ap_X2AP_PDU_successfulOutcome,//T_x2ap_X2AP_PDU_unsuccessfulOutcome,
                                                x2ap_reject,
                                                (rrc_x2ap_error_indication_t *)PNULL) )
                                   )
                                {
                                    X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED ");
                                    retVal = X2AP_FAILURE;
                                }
                                break;
                            }

                            default:
                            X2AP_TRACE(X2AP_WARNING,"%s:"
                                    " invalid x2ap_RespondingNodeType_EndcX2Setup",__FUNCTION__);
                            retVal = X2AP_FAILURE;
                            break;
                        }
                    }
                    break;

                    default:
                    {
                        /*
                         * Class - 1 Error - Unknown IE
                         */
                        /*
                         * Base on Criticality fill the value
                         */
                        x2ap_add_to_err_ind_ie_list(&iE_list,
                                p_x2ap_endc_protocol_IEs->criticality,
                                p_x2ap_endc_protocol_IEs->id,
                                &ie_list_index,
                                &send_err_ind,
                                X2AP_FALSE,
                                X2AP_FALSE);

                        X2AP_TRACE(X2AP_ERROR,"%s:"\
                                "invalid Protocol IE id",__FUNCTION__);

                        retVal = X2AP_FAILURE;
                    } 
                }
                if ( X2AP_FAILURE == retVal )
                {
                    break;
                }
                else
                {
                    p_node = p_node->next;
                }
            }
            break;
        }
        else
        {
            /*
             * Drop message
             */
            X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                    "decode failed Unknown Message type received.",
                    __FUNCTION__);
            break;
        }
    }while(X2AP_NULL);

    /*
     * Parse the map for Error Indication
     */
    if ( ( X2AP_SUCCESS == x2ap_parse_message_map(
                    p_asn1_ctx,
                    p_x2ap_gl_ctx,
                    p_enb_ctx, 
                    &message_map,
                    &iE_list,
                    &ie_list_index,
                    &send_err_ind,
                    ASN1V_x2ap_id_endcX2Setup,
                    T_x2ap_X2AP_PDU_successfulOutcome,//T_x2ap_X2AP_PDU_unsuccessfulOutcome,
                    x2ap_reject,
                    (rrc_x2ap_error_indication_t *)PNULL) )
       )
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED ");
        retVal = X2AP_FAILURE;
    }

    X2AP_UT_TRACE_EXIT();
    return retVal;
}
/* MENB CHANGES - END */

/*EN-DC_changes_start*/
/*****************************************************************************
 * Function Name  : x2ap_internal_decode_menb_initiated_sgnb_release_ack 
 * Inputs         : p_asn1_ctx - pointer to OSCTXT
 *                  p_3gpp - x2ap_SgNBReleaseRequestAcknowledge
 *                  p_intrl - pointer to rrc_x2ap_sgnb_release_request_ack_t
 *                  peer_enodeb_id -  Peer EnodeB ID 
 * Outputs        : p_intrl
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode ue context release ack
 ********************************************************************************/
x2ap_return_et x2ap_internal_decode_menb_initiated_sgnb_release_ack
(
 OSCTXT      *p_asn1_ctx,
 x2ap_SgNBReleaseRequestAcknowledge            *p_3gpp,     /* unpacked src */
 rrc_x2ap_sgnb_release_request_ack_t    *p_intrl,    /* dest */
 U8  peer_enodeb_id /* Peer EnodeB ID */
)
{
    U32  index = X2AP_NULL;
    OSRTDListNode*  p_node     = PNULL;
    x2ap_SgNBReleaseRequestAcknowledge_protocolIEs_element *p_protocol_IE = PNULL;
    x2ap_return_et       retVal = X2AP_SUCCESS;
    uecc_x2ap_error_ind_bool_t send_err_ind;
    uecc_x2ap_error_ind_ie_list_t   iE_list;
    U16 ie_list_index = 0;

    uecc_x2ap_message_data_t message_map =
    {2, X2AP_FALSE,X2AP_FALSE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_MeNB_UE_X2AP_ID, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
        {1, ASN1V_x2ap_id_SgNB_UE_X2AP_ID, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
        {2, ASN1V_x2ap_id_CriticalityDiagnostics, x2ap_optional,  x2ap_ignore, X2AP_NULL, X2AP_NULL, X2AP_NULL, X2AP_NULL},
        {3, ASN1V_x2ap_id_MeNB_UE_X2AP_ID_Extension, x2ap_optional,  x2ap_reject, X2AP_NULL, X2AP_NULL, X2AP_NULL, X2AP_NULL}
        }};

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != p_3gpp);
    X2AP_ASSERT(PNULL != p_intrl);
    X2AP_MEMSET(&iE_list, 0, sizeof(uecc_x2ap_error_ind_ie_list_t));
    X2AP_MEMSET(&send_err_ind, 0, sizeof(uecc_x2ap_error_ind_bool_t));

    do
    {
        p_node = p_3gpp->protocolIEs.head;
        /* Save the decoded Protocol IEs */
        for (index =0; index < p_3gpp->protocolIEs.count; index++)
        {
            if (!p_node)
            {
                X2AP_TRACE(X2AP_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }
            p_protocol_IE =
                (x2ap_SgNBReleaseRequestAcknowledge_protocolIEs_element *)
                p_node->data;
            switch (p_protocol_IE->id)
            {
                case ASN1V_x2ap_id_MeNB_UE_X2AP_ID:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(
                                &message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                &p_protocol_IE->value.u._x2ap_SgNBReleaseRequestAcknowledge_IEs_1))
                    {
                        p_intrl->menb_ue_x2ap_id =
                            p_protocol_IE->value.u._x2ap_SgNBReleaseRequestAcknowledge_IEs_1;
                    }
                }
                break;

                case ASN1V_x2ap_id_SgNB_UE_X2AP_ID:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(&message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                &p_protocol_IE->value.u._x2ap_SgNBReleaseRequestAcknowledge_IEs_2))
                    {
                        p_intrl->sgnb_ue_x2ap_id =
                            p_protocol_IE->value.u._x2ap_SgNBReleaseRequestAcknowledge_IEs_2;
                    }
                }
                break;

                case ASN1V_x2ap_id_CriticalityDiagnostics:
                {
                    /* Fill CriticalityDiagnostics */
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(
                                &message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                p_protocol_IE->value.
                                u._x2ap_SgNBReleaseRequestAcknowledge_IEs_3))
                    {
                        p_intrl->bitmask |= 
                            RRC_X2AP_SGNB_REL_ACK_CRITICAL_DIAGNOSTIC_PRESENT;
                        p_intrl->criticality_diagnostics =
                            *p_protocol_IE->value.
                            u._x2ap_SgNBReleaseRequestAcknowledge_IEs_3;
                    }
                    break;
                }

                case ASN1V_x2ap_id_MeNB_UE_X2AP_ID_Extension:
                {
                    /* Fill MeNB_UE_X2AP_ID_Extension */
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(
                                &message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                &p_protocol_IE->value.
                                u._x2ap_SgNBReleaseRequestAcknowledge_IEs_4))
                    {
                        p_intrl->bitmask |= 
                            RRC_X2AP_SGNB_REL_ACK_MENB_UE_X2AP_ID_EXTN_PRESENT;
                        p_intrl->menb_ue_x2ap_id_ext =
                            p_protocol_IE->value.
                            u._x2ap_SgNBReleaseRequestAcknowledge_IEs_4;
                    }
                    break;
                }

                default:
                X2AP_TRACE(X2AP_ERROR, "%s:"\
                        "invalid Protocol IE id",__FUNCTION__);
                uecc_x2ap_add_to_err_ind_ie_list(&iE_list, 
                        p_protocol_IE->criticality,
                        p_protocol_IE->id,
                        &ie_list_index,
                        &send_err_ind,
                        X2AP_FALSE,
                        X2AP_FALSE);
                if (x2ap_reject == p_protocol_IE->criticality) 
                {
                    retVal = X2AP_FAILURE;
                }
            }
            p_node= p_node->next;
        }
    }while(0);

    if (X2AP_SUCCESS == uecc_x2ap_parse_message_map(
                p_asn1_ctx,
                peer_enodeb_id,
                &message_map,
                &iE_list,
                &ie_list_index,
                &send_err_ind,
                ASN1V_x2ap_id_meNBinitiatedSgNBRelease,
                T_x2ap_X2AP_PDU_successfulOutcome,
                x2ap_ignore,
                (rrc_x2ap_error_indication_t *)PNULL))
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED,"
                " MeNB Initiated SgNB Rel Ack");
        retVal = X2AP_FAILURE;
    }

    X2AP_UT_TRACE_EXIT();

    return retVal;

}

/*****************************************************************************
 * Function Name  : x2ap_decode_endc_x2setup_failure_resp 
 * Inputs         : p_x2ap_gl_ctx - pointer to X2AP global context
 *                  p_enb_ctx     - pointer to x2ap_peer_enb_context_t
 *                  p_setup_fail  - pointer to x2_setup_fail_t
 *                  p_asn_msg     - pointer to ASN message
 *                  p_asn_msg_len - pointer to ASN message length 
 * Outputs        : None 
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode ENDC X2 Setup failure response.
 * Error          : This function report error to the caller function, If decoder
 *                  fails to decode message.
 ********************************************************************************/
x2ap_return_et
x2ap_decode_endc_x2setup_failure_resp
(
 x2ap_gb_context_t          *p_x2ap_gl_ctx,
 x2ap_peer_enb_context_t    *p_enb_ctx,
 x2_setup_fail_t            *p_setup_fail,
 U8                         *p_asn_msg,
 U32                        *p_asn_msg_len,
 OSCTXT 	                   *p_asn1_ctx
)
{
    x2ap_return_et              retVal = X2AP_SUCCESS;
    U32                         count = X2AP_NULL;
    x2ap_X2AP_PDU               x2ap_pdu;
    x2ap_ENDCX2SetupFailure     *p_x2Setup_fail;
    OSRTDListNode               *p_node = X2AP_P_NULL;
    x2ap_ENDCX2SetupFailure_protocolIEs_element
        *p_x2ap_ptcl_fail_IEs = X2AP_P_NULL;
    x2ap_error_ind_ie_list_t    iE_list;
    x2ap_error_ind_bool_t       send_err_ind;
    U16                         ie_list_index = X2AP_NULL;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    U32                         encoded_msg_len = X2AP_NULL;

    x2ap_message_data_t message_map =
    {3, X2AP_FALSE,X2AP_FALSE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_Cause, x2ap_mandatory, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {1, ASN1V_x2ap_id_TimeToWait, x2ap_optional, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {2, ASN1V_x2ap_id_CriticalityDiagnostics, x2ap_optional, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL}
        }};

    X2AP_UT_TRACE_ENTER();

    X2AP_MEMSET(&iE_list, X2AP_NULL, sizeof(x2ap_error_ind_ie_list_t));
    X2AP_MEMSET(&send_err_ind, X2AP_FALSE, sizeof(x2ap_error_ind_bool_t));

    if ( !p_setup_fail )
    {
        X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                "x2ap_ENDCX2SetupFailure is NULL",__FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    /*
     * Init ASN1 context
     */
    if ( X2AP_NULL != rtInitContext(p_asn1_ctx) )
    {
        /*
         * Drop message
         */
        X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                "context initialization failed in x2ap_ENDCX2SetupFailure.",
                __FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    /*
     * Set pointer of asn buffer in asn context
     */
    pu_setBuffer(p_asn1_ctx, p_asn_msg, *p_asn_msg_len, TRUE);

    do
    {
        if ( X2AP_NULL != asn1PD_x2ap_X2AP_PDU(p_asn1_ctx, &x2ap_pdu) )
        {
            /*
             * Drop message
             */
            X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                    "decode failed in x2ap_ENDCX2SetupFailure.",__FUNCTION__);

            /*
             * Send Transfer Syntax Error
             */
            x2ap_build_and_send_err_indication(p_x2ap_gl_ctx, p_enb_ctx, PNULL);
            retVal = X2AP_FAILURE;
            break;
        }
        else
        {
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);

            encoded_msg_len = (U32)pe_GetMsgLen(p_asn1_ctx);

            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    encoded_msg_len);
            if (PNULL != p_event_header)
            {
                x2ap_fill_protocol_event_params(p_event_header, encoded_msg_len,
                        LOCAL_X2_SETUP_FAILURE, 
                        MESSAGE_DIRECTION_RECEIVED);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        encoded_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

        /*
         * check whether the message is not successful
         */
        if ( ( T_x2ap_X2AP_PDU_unsuccessfulOutcome == x2ap_pdu.t ) ||
                ( ASN1V_x2ap_id_x2Setup ==
                  x2ap_pdu.u.unsuccessfulOutcome->procedureCode ) ||
                ( ( X2AP_P_NULL != x2ap_pdu.u.unsuccessfulOutcome->value.u.endcX2Setup ) &&
                  ( T1x2ap__endcX2Setup == x2ap_pdu.u.unsuccessfulOutcome->value.t ) ) )
        {
            p_x2Setup_fail = x2ap_pdu.u.unsuccessfulOutcome->value.u.endcX2Setup;
            p_node = p_x2Setup_fail->protocolIEs.head;

            for ( count = X2AP_NULL; count < p_x2Setup_fail->protocolIEs.count; count++ )
            {
                if ( !p_node )
                {
                    X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL in "
                            "x2ap_ENDCX2SetupFailure",
                            __FUNCTION__);
                    retVal = X2AP_FAILURE;
                    break;
                }

                p_x2ap_ptcl_fail_IEs =
                    (x2ap_ENDCX2SetupFailure_protocolIEs_element*)p_node->data;

                switch ( p_x2ap_ptcl_fail_IEs->id )
                {
                    case ASN1V_x2ap_id_Cause:
                    X2AP_TRACE(X2AP_INFO,"%s: "\
                            "ASN1V_x2ap_id_Cause in x2ap_ENDCX2SetupFailure",__FUNCTION__);

                    if ( X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                count,
                                p_x2ap_ptcl_fail_IEs->id,
                                p_x2ap_ptcl_fail_IEs->value.u._x2ap_ENDCX2SetupFailure_IEs_1) )
                    {
                        if(X2AP_FAILURE == x2ap_parse_cause(
                                    &(p_setup_fail->cause),
                                    p_x2ap_ptcl_fail_IEs->value.u.
                                    _x2ap_ENDCX2SetupFailure_IEs_1,
                                    &message_map,
                                    (U8)count,
                                    (U8)p_x2ap_ptcl_fail_IEs->id))
                        {
                            X2AP_TRACE(X2AP_INFO,
                                    "%s:x2ap_parse_cause failed in x2ap_ENDCX2SetupFailure",__FUNCTION__);
                            break;
                        }
                    }
                    break;

                    case ASN1V_x2ap_id_CriticalityDiagnostics:
                    X2AP_TRACE(X2AP_INFO,"%s: "\
                            "ASN1V_x2ap_id_CriticalityDiagnostics in x2ap_ENDCX2SetupFailure",__FUNCTION__);

                    if ( X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                count,
                                p_x2ap_ptcl_fail_IEs->id,
                                p_x2ap_ptcl_fail_IEs->value.u._x2ap_ENDCX2SetupFailure_IEs_2) )
                    {
                        retVal = x2ap_parse_CriticalityDiagnostics(
                                &(p_setup_fail->critc_diagno),
                                p_x2ap_ptcl_fail_IEs->value.u.
                                _x2ap_ENDCX2SetupFailure_IEs_2);
                        x2ap_log_criticality_diagostics(p_x2ap_ptcl_fail_IEs->value.u.
                                _x2ap_ENDCX2SetupFailure_IEs_2);
                    }
                    break;

                    case ASN1V_x2ap_id_TimeToWait:
                    X2AP_TRACE(X2AP_INFO,"%s: "\
                            "ASN1V_x2ap_id_TimeToWait in x2ap_ENDCX2SetupFailure",__FUNCTION__);

                    if ( X2AP_SUCCESS == x2ap_validate_ie_value(
                                &message_map,
                                count,
                                p_x2ap_ptcl_fail_IEs->id,
                                &p_x2ap_ptcl_fail_IEs->value.u._x2ap_ENDCX2SetupFailure_IEs_3) )
                    {
                        p_setup_fail->bitmask = X2_SETUP_FAIL_TIME_TO_WAIT_PRESENT;
                        p_setup_fail->time_to_wait =
                            (x2ap_time_to_wait_et)p_x2ap_ptcl_fail_IEs->value.u.
                            _x2ap_ENDCX2SetupFailure_IEs_3;
                        retVal = X2AP_SUCCESS;
                    }
                    break;

                    default:
                    /*
                     * Class - 1 Error - Unknown IE
                     */
                    /*
                     * Base on Criticality fill the value
                     */
                    x2ap_add_to_err_ind_ie_list(&iE_list,
                            p_x2ap_ptcl_fail_IEs->criticality,
                            p_x2ap_ptcl_fail_IEs->id,
                            &ie_list_index,
                            &send_err_ind,
                            X2AP_FALSE,
                            X2AP_FALSE);

                    X2AP_TRACE(X2AP_ERROR,"%s:"\
                            "invalid Protocol IE id in x2ap_ENDCX2SetupFailure",__FUNCTION__);

                    if ( x2ap_reject == p_x2ap_ptcl_fail_IEs->criticality ) 
                    {
                        retVal = X2AP_FAILURE;
                    }
                }

                p_node = p_node->next;
            }
            break;
        }
        else
        {
            /*
             * Drop message
             */
            X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                    "decode failed Unknown Message type received during x2ap_ENDCX2SetupFailure.",
                    __FUNCTION__);
            retVal = X2AP_FAILURE;
            break;
        }
    }while(X2AP_NULL);

    /*
     * Parse the map for Error Indication
     */
    if ( ( X2AP_SUCCESS == x2ap_parse_message_map(
                    p_asn1_ctx,
                    p_x2ap_gl_ctx,
                    p_enb_ctx, 
                    &message_map,
                    &iE_list,
                    &ie_list_index,
                    &send_err_ind,
                    ASN1V_x2ap_id_endcX2Setup,
                    T_x2ap_X2AP_PDU_unsuccessfulOutcome,
                    x2ap_reject,
                    (rrc_x2ap_error_indication_t *)PNULL) )
       )
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED in x2ap_ENDCX2SetupFailure");
        retVal = X2AP_FAILURE;
    }

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_decode_endc_x2setup_req
 * Inputs         : p_x2ap_gl_ctx  - pointer to X2Ap global context
 *                  p_enb_ctx      - pointer to x2ap_peer_enb_context_t
 *                  p_x2_setup_req - pointer to x2_setup_req_t
 *                  p_asn_msg      - pointer to ASN message
 *                  p_asn_msg_len  - pointer to ASN message length
 * Outputs        : p_asn_msg,p_asn_msg_len
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode EN DC X2 Setup request.
 * Error          : This function report error to the caller function, If decoder
 *                  fails to decode message.
 ********************************************************************************/
x2ap_return_et x2ap_decode_endc_x2setup_req
(
 x2ap_gb_context_t              *p_x2ap_gl_ctx,
 x2ap_peer_enb_context_t        *p_enb_ctx,
 en_dc_x2_setup_req_t           *p_endc_x2_setup_req,
 U8                             *p_asn_msg,
 U32                            *p_asn_msg_len,
 rrc_x2ap_error_indication_t    *p_error_indication,
 x2ap_error_ind_bool_t          *p_send_err_ind,
 OSCTXT                         *p_asn1_ctx
)
{
    x2ap_return_et                      retVal = X2AP_SUCCESS;
    U32                                 count = X2AP_NULL;
    U32                                 count_1 = X2AP_NULL;
    x2ap_X2AP_PDU                       x2ap_pdu;
    x2ap_ENDCX2SetupRequest             *p_x2Setup_req = X2AP_P_NULL;
    OSRTDListNode                       *p_node = X2AP_P_NULL;
    OSRTDListNode                       *p_node_1 = X2AP_P_NULL;
    x2ap_ENDCX2SetupRequest_protocolIEs_element
        *p_x2ap_protocol_IEs = X2AP_P_NULL;
    x2ap_InitiatingNodeType_EndcX2Setup_init_en_gNB_element
        *p_x2ap_en_gNB_element = X2AP_P_NULL;
    x2ap_InitiatingNodeType_EndcX2Setup_init_en_gNB
        *p_resp_genb = X2AP_P_NULL;
    x2ap_error_ind_ie_list_t            iE_list;
    U16                                 ie_list_index = X2AP_NULL;
    EVENT_EXTERNAL_HEADER               *p_event_header = PNULL;
    U32                                 encoded_msg_len = X2AP_NULL;
    uecc_x2ap_error_ind_bool_t          send_err_ind;

    x2ap_message_data_t message_map =
    {1, X2AP_TRUE,X2AP_TRUE, X2AP_NULL, X2AP_NULL,  X2AP_NULL,
        {{0, ASN1V_x2ap_id_InitiatingNodeType_EndcX2Setup, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
        }};

    X2AP_UT_TRACE_ENTER();

    X2AP_MEMSET(&iE_list, X2AP_NULL, sizeof(x2ap_error_ind_ie_list_t));
    X2AP_MEMSET(&send_err_ind, X2AP_FALSE, sizeof(x2ap_error_ind_bool_t));

    if (!p_endc_x2_setup_req)
    {
        X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                "p_endc_x2_setup_req is NULL",__FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    /* Init ASN1 context */
    if ( 0 != rtInitContext(p_asn1_ctx) )
    {
        /* Drop message */
        X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                "context initialization failed.",__FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }
    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(p_asn1_ctx, p_asn_msg, *p_asn_msg_len, TRUE);

    do
    {
        if (0 != asn1PD_x2ap_X2AP_PDU(p_asn1_ctx, &x2ap_pdu) )
        {

            /* Drop message */
            X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                    "decode failed.",__FUNCTION__);

            /* Send Transfer Syntax Error */
            x2ap_build_and_send_err_indication(p_x2ap_gl_ctx, p_enb_ctx, PNULL);
            retVal = X2AP_FAILURE;
            break;
        }
        else
        {
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);

            encoded_msg_len = (U32)pe_GetMsgLen(p_asn1_ctx);

            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    encoded_msg_len);
            if (PNULL != p_event_header)
            {
                x2ap_fill_protocol_event_params(p_event_header, encoded_msg_len,
                        LOCAL_X2_SETUP_REQUEST,
                        MESSAGE_DIRECTION_RECEIVED);

                l3_memcpy_wrapper((void*)((U8*)p_event_header +
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        encoded_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

        /* check whether the message is not successful */
        if ((T_x2ap_X2AP_PDU_initiatingMessage == x2ap_pdu.t) ||
                (ASN1V_x2ap_id_endcX2Setup ==
                 x2ap_pdu.u.initiatingMessage->procedureCode) ||
                ((X2AP_P_NULL != x2ap_pdu.u.initiatingMessage->value.u.endcX2Setup) &&
                 (T1x2ap__endcX2Setup == x2ap_pdu.u.initiatingMessage->value.t)))
        {
            p_x2Setup_req = x2ap_pdu.u.initiatingMessage->value.u.endcX2Setup;
            p_node = p_x2Setup_req->protocolIEs.head;

            for ( count = X2AP_NULL; count < p_x2Setup_req->protocolIEs.count; count++ )
            {
                if (!p_node)
                {
                    X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                            __FUNCTION__);
                    retVal = X2AP_FAILURE;
                    break;
                }
                p_x2ap_protocol_IEs =
                    (x2ap_ENDCX2SetupRequest_protocolIEs_element *)p_node->data;
                switch ( p_x2ap_protocol_IEs->id )
                {
                    case ASN1V_x2ap_id_InitiatingNodeType_EndcX2Setup:
                    {
                        X2AP_TRACE(X2AP_INFO,"%s: ASN1V_x2ap_id_InitiatingNodeType_EndcX2Setup",
                                __FUNCTION__);

                        if ( PNULL == p_x2ap_protocol_IEs->value.u.
                                _x2ap_ENDCX2SetupRequest_IEs_1 )
                        {
                            X2AP_TRACE(X2AP_ERROR,"%s: _x2ap_ENDCX2SetupRequest_IEs_1"
                                    "is NULL",
                                    __FUNCTION__);
                            retVal = X2AP_FAILURE;                    
                            break;
                        }

                        if (X2AP_SUCCESS == x2ap_validate_ie_value(&message_map,
                                    count,
                                    (U8)p_x2ap_protocol_IEs->id,
                                    &p_x2ap_protocol_IEs->value.u._x2ap_ENDCX2SetupRequest_IEs_1))
                        {
                            X2AP_TRACE(X2AP_DETAILED, "ASN1V_x2ap_id_InitiatingNode"
                                    "Type_EndcX2Setup");
                        }
                        else
                        {
                            X2AP_TRACE(X2AP_ERROR,"%s: ASN1V_x2ap_id_InitiatingNode"
                                    "Type_EndcX2Setup validation failed",
                                    __FUNCTION__);
                            retVal = X2AP_FAILURE;                    
                            break;
                        }

                        if ((T_x2ap_InitiatingNodeType_EndcX2Setup_init_en_gNB ==
                                    p_x2ap_protocol_IEs->value.u._x2ap_ENDCX2SetupRequest_IEs_1->t))
                        {
                            x2ap_message_data_t message_map =
                            {2, X2AP_TRUE,X2AP_TRUE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
                                {{0, ASN1V_x2ap_id_Globalen_gNB_ID, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
                                    {1, ASN1V_x2ap_id_ServedNRcellsENDCX2ManagementList,x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
                                }};

                            x2ap_bool_et send_tse = X2AP_FALSE;
                            p_resp_genb = p_x2ap_protocol_IEs->value.
                                u._x2ap_ENDCX2SetupRequest_IEs_1->u.
                                init_en_gNB;
                            if ( PNULL == p_resp_genb )
                            {
                                X2AP_TRACE(X2AP_ERROR,"%s: p_resp_genb "
                                        "is NULL",
                                        __FUNCTION__);
                                retVal = X2AP_FAILURE;                    
                                break;
                            }
                            p_node_1 = p_resp_genb->head;

                            for (count_1 = X2AP_NULL; count_1 < p_resp_genb->count; count_1++)
                            {
                                if (!p_node_1)
                                {
                                    X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                                            __FUNCTION__);
                                    retVal = X2AP_FAILURE;
                                    break;
                                }
                                p_x2ap_en_gNB_element =
                                    (x2ap_InitiatingNodeType_EndcX2Setup_init_en_gNB_element *)p_node_1->data;
                                if ( PNULL == p_x2ap_en_gNB_element )
                                {
                                    X2AP_TRACE(X2AP_ERROR,"%s: "
                                            "p_resp_en_gnb_element is NULL",
                                            __FUNCTION__);
                                    retVal = X2AP_FAILURE;                    
                                    break;
                                }
                                switch (p_x2ap_en_gNB_element->id)
                                {
                                    case ASN1V_x2ap_id_Globalen_gNB_ID:
                                    {
                                        X2AP_TRACE(X2AP_DETAILED,
                                                "ASN1V_x2ap_id_Globalen_gNB_ID");

                                        if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                                    &message_map,
                                                    count_1,
                                                    (U8)p_x2ap_en_gNB_element->id,
                                                    p_x2ap_en_gNB_element->value.u._x2ap_En_gNB_ENDCX2SetupReqIEs_1))
                                        {

                                            if ( X2AP_FAILURE == x2ap_parse_GlobalGNB_ID(
                                                        &(p_endc_x2_setup_req->nr_cell_info.global_gnb_id),
                                                        p_x2ap_en_gNB_element->value.u.
                                                        _x2ap_En_gNB_ENDCX2SetupReqIEs_1,
                                                        &message_map,
                                                        (U8)count_1,
                                                        (U8)p_x2ap_en_gNB_element->id) )
                                            {
                                                X2AP_TRACE(X2AP_INFO,"%s: x2ap_parse_GlobalGNB_ID"
                                                        "failed",__FUNCTION__);
                                                retVal = X2AP_FAILURE;
                                                break;
                                            }
                                            l3_memcpy_wrapper(&(p_x2ap_gl_ctx->global_gnb_id),
                                                    &(p_endc_x2_setup_req->nr_cell_info.global_gnb_id),
                                                    sizeof(x2_gb_gnb_id_t));
                                        }
                                    }
                                    break;

                                    case ASN1V_x2ap_id_ServedNRcellsENDCX2ManagementList:
                                    {
                                        X2AP_TRACE(X2AP_DETAILED,
                                                "ASN1V_x2ap_id_ServedNRcellsENDCX2ManagementList");
                                        if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                                    &message_map,
                                                    count_1,
                                                    (U8)p_x2ap_en_gNB_element->id,
                                                    p_x2ap_en_gNB_element->value.u._x2ap_En_gNB_ENDCX2SetupReqIEs_2))
                                        {
                                            if ( X2AP_FAILURE == x2ap_parse_srerved_nr_cells_endc_x2_management_list(
                                                        &p_endc_x2_setup_req->nr_cell_info,
                                                        p_x2ap_en_gNB_element->value.u.
                                                        _x2ap_En_gNB_ENDCX2SetupReqIEs_2,
                                                        &message_map,
                                                        (U8)count_1,
                                                        (U8)p_x2ap_en_gNB_element->id,
                                                        &send_tse,
                                                        p_asn1_ctx
                                                        ))
                                            {
                                                X2AP_TRACE(X2AP_INFO,"%s: "
                                                        "x2ap_parse_srerved_nr_cells_endc_x2_management_list"
                                                        "failed",__FUNCTION__);
                                                retVal = X2AP_FAILURE;
                                                break;
                                            }
                                            /* This will only happen when the element list exceeds
                                             * the max number */
                                            if (X2AP_TRUE == send_tse)
                                            {
                                                X2AP_TRACE(X2AP_WARNING, "Error is Detected" 
                                                        "in ASN.1 Sequence, Sending Transfer Syntax Error");
                                                x2ap_build_and_send_err_indication(p_x2ap_gl_ctx, p_enb_ctx, PNULL);
                                                retVal = X2AP_FAILURE;
                                                break;
                                            }
                                        }
                                    }
                                    break;
                                    default :
                                    /* Base on Criticality fill the value */
                                    X2AP_TRACE(X2AP_ERROR,"%s:"\
                                            "invalid Protocol IE id",__FUNCTION__);
                                    x2ap_add_to_err_ind_ie_list(&iE_list,
                                            p_x2ap_en_gNB_element->criticality,
                                            p_x2ap_en_gNB_element->id,
                                            &ie_list_index,
                                            p_send_err_ind,
                                            X2AP_FALSE,
                                            X2AP_FALSE);

                                    if (x2ap_reject == p_x2ap_en_gNB_element->criticality)
                                    {
                                        retVal = X2AP_FAILURE;
                                    }
                                }
                                p_node_1 = p_node_1->next;
                            }
                            /* Parse the map for Error Indication */
                            if ((X2AP_SUCCESS == x2ap_parse_message_map(
                                            p_asn1_ctx,
                                            p_x2ap_gl_ctx,
                                            p_enb_ctx,
                                            &message_map,
                                            &iE_list,
                                            &ie_list_index,
                                            p_send_err_ind,
                                            ASN1V_x2ap_id_x2Setup,
                                            T_x2ap_X2AP_PDU_initiatingMessage,
                                            x2ap_reject,
                                            p_error_indication))
                               )
                            {
                                X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED ");

                                retVal = X2AP_FAILURE;
                            }
                        }
                        else
                        {
                            //todo error handling
/* coverity_277499_fix_start */
                            X2AP_TRACE(X2AP_ERROR,"%s:"\
                                    "invalid Protocol IE id(%d)",__FUNCTION__, p_x2ap_protocol_IEs->id);

                            x2ap_add_to_err_ind_ie_list(&iE_list,
                                    p_x2ap_protocol_IEs->criticality,
                                    p_x2ap_protocol_IEs->id,
                                    &ie_list_index,
                                    p_send_err_ind,
                                    X2AP_FALSE,
                                    X2AP_FALSE);

                            if (x2ap_reject == p_x2ap_protocol_IEs->criticality)
                            {
                                retVal = X2AP_FAILURE;
                            }
/* coverity_277499_fix_end */
                        }
                        break;
                    }
                    default :
                    /* Base on Criticality fill the value */
                    X2AP_TRACE(X2AP_ERROR,"%s:"\
                            "invalid Protocol IE id",__FUNCTION__);
                    x2ap_add_to_err_ind_ie_list(&iE_list,
                            p_x2ap_protocol_IEs->criticality,
                            p_x2ap_protocol_IEs->id,
                            &ie_list_index,
                            p_send_err_ind,
                            X2AP_FALSE,
                            X2AP_FALSE);

                    if (x2ap_reject == p_x2ap_protocol_IEs->criticality)
                    {
                        retVal = X2AP_FAILURE;
                    }
                }
                p_node = p_node->next;
            }
        }
        else
        {
            /* Drop message */
            X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                    "decode failed Unknown Message type received.",
                    __FUNCTION__);
            retVal = X2AP_FAILURE;
            break;
        }
    }while(0);
    /* Parse the map for Error Indication */
    if ((X2AP_SUCCESS == x2ap_parse_message_map(
                    p_asn1_ctx,
                    p_x2ap_gl_ctx,
                    p_enb_ctx,
                    &message_map,
                    &iE_list,
                    &ie_list_index,
                    p_send_err_ind,
                    ASN1V_x2ap_id_endcX2Setup,
                    T_x2ap_X2AP_PDU_initiatingMessage,
                    x2ap_reject,
                    p_error_indication))
       )
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED ");

        retVal = X2AP_FAILURE;
    }

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_internal_decode_sgnb_initiated_sgnb_release
 * Inputs         : p_asn1_ctx     - pointer to OSCTXT
 *                  p_3gpp         - x2ap_SgNBReleaseRequired
 *                  p_intrl        - pointer to rrc_x2ap_sgnb_release_required_t
 *                  peer_enodeb_id - Peer EnodeB ID
 * Outputs        : p_intrl
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode SgNB Initiated SgNB release req
 ********************************************************************************/
x2ap_return_et x2ap_internal_decode_sgnb_initiated_sgnb_release
(
 OSCTXT                              *p_asn1_ctx,
 x2ap_SgNBReleaseRequired            *p_3gpp,
 rrc_x2ap_sgnb_release_required_t    *p_intrl,
 U8                                  peer_enodeb_id
)
{
    U32                                          index = X2AP_NULL;
    OSRTDListNode*                               p_node     = PNULL;
    x2ap_SgNBReleaseRequired_protocolIEs_element *p_protocol_IE = PNULL;
    x2ap_return_et                               retVal = X2AP_SUCCESS;
    uecc_x2ap_error_ind_bool_t                   send_err_ind;
    uecc_x2ap_error_ind_ie_list_t                iE_list;
    U16                                          ie_list_index = X2AP_NULL;

    uecc_x2ap_message_data_t message_map =
    {3, X2AP_FALSE,X2AP_FALSE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_MeNB_UE_X2AP_ID, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {1, ASN1V_x2ap_id_SgNB_UE_X2AP_ID, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {2, ASN1V_x2ap_id_Cause, x2ap_mandatory, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL}}};

    X2AP_UT_TRACE_ENTER();

    X2AP_ASSERT(PNULL != p_3gpp);
    X2AP_ASSERT(PNULL != p_intrl);

    X2AP_MEMSET(&iE_list, RRC_NULL, sizeof(uecc_x2ap_error_ind_ie_list_t));
    X2AP_MEMSET(&send_err_ind, RRC_NULL, sizeof(uecc_x2ap_error_ind_bool_t));

    do
    {
        p_node = p_3gpp->protocolIEs.head;

        /*
         * Save the decoded Protocol IEs
         */
        for ( index = X2AP_NULL; index < p_3gpp->protocolIEs.count; index++ )
        {
            if (!p_node)
            {
                X2AP_TRACE(X2AP_ERROR,"%s: "
                        "next node returned NULL",
                        __FUNCTION__);
                break;
            }
            p_protocol_IE =
                (x2ap_SgNBReleaseRequired_protocolIEs_element *)
                p_node->data;
            switch (p_protocol_IE->id)
            {
                case ASN1V_x2ap_id_MeNB_UE_X2AP_ID:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(
                                &message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                &p_protocol_IE->value.u._x2ap_SgNBReleaseRequired_IEs_1))
                    {
                        p_intrl->menb_ue_x2ap_id =
                            p_protocol_IE->value.u._x2ap_SgNBReleaseRequired_IEs_1;
                    }
                }
                break;

                case ASN1V_x2ap_id_SgNB_UE_X2AP_ID:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(&message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                &p_protocol_IE->value.u._x2ap_SgNBReleaseRequired_IEs_2))
                    {
                        p_intrl->sgnb_ue_x2ap_id =
                            p_protocol_IE->value.u._x2ap_SgNBReleaseRequired_IEs_2;
                    }
                }
                break;

                case ASN1V_x2ap_id_Cause:
                {
                    if (X2AP_SUCCESS == uecc_x2ap_validate_ie_value(&message_map,
                                index,
                                (U8)p_protocol_IE->id,
                                p_protocol_IE->value.u._x2ap_SgNBReleaseRequired_IEs_3))
                    {
                        p_intrl->cause =
                            *p_protocol_IE->value.u._x2ap_SgNBReleaseRequired_IEs_3;
                    }
                }
                break;

                default:
                X2AP_TRACE(X2AP_ERROR, "%s:"\
                        "invalid Protocol IE id",__FUNCTION__);
                uecc_x2ap_add_to_err_ind_ie_list(&iE_list,
                        p_protocol_IE->criticality,
                        p_protocol_IE->id,
                        &ie_list_index,
                        &send_err_ind,
                        X2AP_FALSE,
                        X2AP_FALSE);
                if (x2ap_reject == p_protocol_IE->criticality)
                {
                    retVal = X2AP_FAILURE;
                }
            }
            p_node= p_node->next;
        }
    } while(X2AP_NULL);

    if (X2AP_SUCCESS == uecc_x2ap_parse_message_map(
                p_asn1_ctx,
                peer_enodeb_id,
                &message_map,
                &iE_list,
                &ie_list_index,
                &send_err_ind,
                ASN1V_x2ap_id_sgNBinitiatedSgNBRelease,
                T_x2ap_X2AP_PDU_initiatingMessage,
                x2ap_ignore,
                (rrc_x2ap_error_indication_t *)PNULL))
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED,"
                " SgNB Initiated SgNB Rel Req");
        retVal = X2AP_FAILURE;
    }

    X2AP_UT_TRACE_EXIT();

    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_decode_endc_config_update_en_gnb_req
 * Inputs         : x2ap_gb_context_t          *p_x2ap_gl_ctx
 *                  x2ap_peer_enb_context_t    *p_enb_ctx
 *                  x2ap_endc_config_update_en_gnb_req_t      *p_endc_config_update_req
 *                  U8                         *p_asn_msg
 *                  U32                        *p_asn_msg_len
 *                  OSCTXT                     *p_asn1_ctx
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode ENDC config update req from en gnb.
 ********************************************************************************/
x2ap_return_et x2ap_decode_endc_config_update_en_gnb_req
(
 x2ap_gb_context_t          *p_x2ap_gl_ctx,
 x2ap_peer_enb_context_t    *p_enb_ctx,
 x2ap_endc_config_update_en_gnb_req_t      *p_endc_config_update_req,
 U8                         *p_asn_msg,
 U32                        *p_asn_msg_len,
 OSCTXT                     *p_asn1_ctx
)
{
    x2ap_X2AP_PDU               x2ap_pdu;
    x2ap_error_ind_ie_list_t    iE_list;
    x2ap_error_ind_bool_t       send_err_ind;
    x2ap_return_et              retVal          = X2AP_SUCCESS;
    U32                         ie_count        = X2AP_NULL;
    U32                         genb_index      = X2AP_NULL;
    U16                         ie_list_index   = X2AP_NULL;
    x2ap_ENDCConfigurationUpdate     *p_endc_config_upd_req = X2AP_P_NULL;
    OSRTDListNode               *p_node = X2AP_P_NULL;
    OSRTDListNode               *p_node_genb = X2AP_P_NULL;
    OSRTDListNode               *p_node_3 = X2AP_P_NULL;
    x2ap_InitiatingNodeType_EndcConfigUpdate_init_en_gNB
                                *p_endc_configupdate_init_en_gnb = X2AP_P_NULL;
    x2ap_InitiatingNodeType_EndcConfigUpdate_init_en_gNB_element
                                *p_endc_configupdate_init_en_gnb_elem = X2AP_P_NULL;
    x2ap_ENDCConfigurationUpdate_protocolIEs_element
                                *p_x2ap_endc_protocol_IEs = X2AP_P_NULL;

    x2ap_message_data_t message_map =
    {1, X2AP_FALSE,X2AP_FALSE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_InitiatingNodeType_EndcConfigUpdate, x2ap_mandatory, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
        }};

    X2AP_UT_TRACE_ENTER();

    X2AP_MEMSET(&iE_list, X2AP_NULL, sizeof(x2ap_error_ind_ie_list_t));
    X2AP_MEMSET(&send_err_ind, X2AP_FALSE, sizeof(x2ap_error_ind_bool_t));

    if ( !p_endc_config_update_req )
    {
        X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                "p_endc_config_update_req is NULL",__FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }

    /* Init ASN1 context */
    if ( 0 != rtInitContext(p_asn1_ctx) )
    {
        /* Drop message */
        X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                "context initialization failed.",__FUNCTION__);
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }
    /* Set pointer of asn buffer in asn context */
    if(RT_OK != pu_setBuffer(p_asn1_ctx, p_asn_msg, *p_asn_msg_len, TRUE))
    {
        X2AP_TRACE(X2AP_ERROR,"pu_setBuffer failed.",__FUNCTION__);
        return X2AP_FAILURE;
    }

    do
    {
        if ( X2AP_NULL != asn1PD_x2ap_X2AP_PDU(p_asn1_ctx, &x2ap_pdu) )
        {
            /* Drop message*/
            X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                    "decode failed.",__FUNCTION__);
            /* Send Transfer Syntax Error */
            x2ap_build_and_send_err_indication(p_x2ap_gl_ctx, p_enb_ctx, PNULL);
            rtxErrPrint(p_asn1_ctx);
            break;
        }
        else
        {
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
        }

        /* Check whether the message is not successful*/
        if ((T_x2ap_X2AP_PDU_initiatingMessage == x2ap_pdu.t ) &&
                (ASN1V_x2ap_id_endcConfigurationUpdate ==
                 x2ap_pdu.u.initiatingMessage->procedureCode ) &&
                ((X2AP_P_NULL != x2ap_pdu.u.initiatingMessage->value.u.endcConfigurationUpdate) &&
                 (T1x2ap__endcConfigurationUpdate == x2ap_pdu.u.initiatingMessage->value.t)))
        {
            p_endc_config_upd_req = x2ap_pdu.u.initiatingMessage->value.u.endcConfigurationUpdate;
            p_node = p_endc_config_upd_req->protocolIEs.head;

            for ( ie_count = X2AP_NULL; ie_count < p_endc_config_upd_req->protocolIEs.count;
                    ie_count++ )
            {
                if ( !p_node )
                {
                    X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                            __FUNCTION__);
                    retVal = X2AP_FAILURE;                    
                    break;
                }

                p_x2ap_endc_protocol_IEs = 
                    (x2ap_ENDCConfigurationUpdate_protocolIEs_element *)p_node->data;
                if ( PNULL == p_x2ap_endc_protocol_IEs )
                {
                    X2AP_TRACE(X2AP_ERROR,"%s: p_x2ap_endc_protocol_IEs is NULL",
                            __FUNCTION__);
                    retVal = X2AP_FAILURE;                    
                    break;
                }

                switch ( p_x2ap_endc_protocol_IEs->id )
                {
                    case ASN1V_x2ap_id_InitiatingNodeType_EndcConfigUpdate:
                    {
                        X2AP_TRACE(X2AP_DETAILED, "ASN1V_x2ap_id_InitiatingNodeType_EndcConfigUpdate");

                        if ( PNULL == p_x2ap_endc_protocol_IEs->value.u._x2ap_ENDCConfigurationUpdate_IEs_1)
                        {
                            X2AP_TRACE(X2AP_ERROR,"%s: _x2ap_ENDCConfigurationUpdate_IEs_1"
                                    "is NULL",
                                    __FUNCTION__);
                            retVal = X2AP_FAILURE;                    
                            break;
                        }

                        if (X2AP_SUCCESS == x2ap_validate_ie_value(&message_map,
                                    ie_count,
                                    (U8)p_x2ap_endc_protocol_IEs->id,
                                    &p_x2ap_endc_protocol_IEs->value.u.
                                    _x2ap_ENDCConfigurationUpdate_IEs_1))
                        {
                            X2AP_TRACE(X2AP_DETAILED, "_x2ap_ENDCConfigurationUpdate_IEs_1");
                        }
                        else
                        {
                            X2AP_TRACE(X2AP_ERROR,"%s: ASN1V_x2ap_id_InitiatingNode"
                                    "Type_EndcConfigUpdate validation failed",
                                    __FUNCTION__);
                            retVal = X2AP_FAILURE;                    
                            break;
                        }

                        switch ( p_x2ap_endc_protocol_IEs->value.u.
                                _x2ap_ENDCConfigurationUpdate_IEs_1->t )
                        {
                            case T_x2ap_InitiatingNodeType_EndcConfigUpdate_init_en_gNB:
                            {
                                x2ap_message_data_t message_map =
                                {3, X2AP_TRUE,X2AP_TRUE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
                                    {{0, ASN1V_x2ap_id_ServedNRcellsENDCX2ManagementList, x2ap_optional, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
                                        {1, ASN1V_x2ap_id_ServedNRcellsToModifyListENDCConfUpd,x2ap_optional, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
                                        {2, ASN1V_x2ap_id_ServedNRcellsToDeleteListENDCConfUpd,x2ap_optional, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
                                    }};

                                x2ap_bool_et send_tse = X2AP_FALSE;

                                p_endc_config_update_req->bitmask |= X2AP_ENDC_X2_CONFIG_UPDATE_INIT_EN_GNB_PRESENT;

                                p_endc_configupdate_init_en_gnb = p_x2ap_endc_protocol_IEs->value.
                                    u._x2ap_ENDCConfigurationUpdate_IEs_1->u.init_en_gNB;
                                if ( PNULL == p_endc_configupdate_init_en_gnb )
                                {
                                    X2AP_TRACE(X2AP_ERROR,"%s: p_endc_configupdate_init_en_gnb "
                                            "is NULL",
                                            __FUNCTION__);
                                    retVal = X2AP_FAILURE;                    
                                    break;
                                }

                                p_node_genb = p_endc_configupdate_init_en_gnb->head;
                                for ( genb_index = X2AP_NULL; genb_index <
                                        p_endc_configupdate_init_en_gnb->count; genb_index++ )
                                {
                                    if ( !p_node_genb )
                                    {
                                        X2AP_TRACE(X2AP_ERROR,"%s: next node "
                                                "returned NULL",__FUNCTION__);
                                        retVal = X2AP_FAILURE;                    
                                        break;
                                    }

                                    p_endc_configupdate_init_en_gnb_elem = 
                                        (x2ap_InitiatingNodeType_EndcConfigUpdate_init_en_gNB_element *)
                                        p_node_genb->data;
                                    if ( PNULL == p_endc_configupdate_init_en_gnb_elem )
                                    {
                                        X2AP_TRACE(X2AP_ERROR,"%s: "
                                                "x2ap_InitiatingNodeType_EndcConfigUpdate_init_en_gNB_element is NULL",
                                                __FUNCTION__);
                                        retVal = X2AP_FAILURE;                    
                                        break;
                                    }

                                    switch ( p_endc_configupdate_init_en_gnb_elem->id )
                                    {
                                        case ASN1V_x2ap_id_ServedNRcellsENDCX2ManagementList:
                                        {
                                            X2AP_TRACE(X2AP_DETAILED,
                                                    "ASN1V_x2ap_id_ServedNRcellsENDCX2ManagementList");
                                            if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                                        &message_map,
                                                        genb_index,
                                                        (U8)p_endc_configupdate_init_en_gnb_elem->id,
                                                        p_endc_configupdate_init_en_gnb_elem->value.u.
                                                        _x2ap_En_gNB_ENDCConfigUpdateIEs_1))
                                            {
                                                if ( X2AP_FAILURE ==
                                                    x2ap_parse_srerved_nr_cells_endc_x2_management_list(
                                                            &p_endc_config_update_req->en_gNB.eutra_nr_cell_management,
                                                            p_endc_configupdate_init_en_gnb_elem->value.u.
                                                            _x2ap_En_gNB_ENDCConfigUpdateIEs_1,
                                                            &message_map,
                                                            (U8)ie_count,
                                                            (U8)p_endc_configupdate_init_en_gnb_elem->id,
                                                            &send_tse,
                                                            p_asn1_ctx
                                                            ))
                                                {
                                                    X2AP_TRACE(X2AP_INFO,"%s: "
                                                            "x2ap_parse_srerved_nr_cells_endc_x2_management_list"
                                                            "failed",__FUNCTION__);
                                                    retVal = X2AP_FAILURE;                    
                                                    break;
                                                }

                                                p_endc_config_update_req->en_gNB.bitmask |= 
                                                    X2AP_ENDC_CONFIGUPDATE_GNB_SERVEDNRCELL_MANAGEMENTLIST_PRESENT;
                                                    
                                                /* This will only happen when the element list exceeds
                                                 * the max number */
                                                if (X2AP_TRUE == send_tse)
                                                {
                                                    X2AP_TRACE(X2AP_WARNING, "Error is Detected" 
                                                            "in ASN.1 Sequence, Sending Transfer Syntax Error");
                                                    x2ap_build_and_send_err_indication(p_x2ap_gl_ctx,
                                                        p_enb_ctx, PNULL);
                                                    retVal = X2AP_FAILURE;
                                                    break;
                                                }
                                            }
                                        }
                                        break;

                                        case ASN1V_x2ap_id_ServedNRcellsToModifyListENDCConfUpd:
                                        {
                                            X2AP_TRACE(X2AP_DETAILED,
                                                    "ASN1V_x2ap_id_ServedNRcellsToModifyListENDCConfUpd");
                                            if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                                        &message_map,
                                                        genb_index,
                                                        (U8)p_endc_configupdate_init_en_gnb_elem->id,
                                                        p_endc_configupdate_init_en_gnb_elem->value.u.
                                                        _x2ap_En_gNB_ENDCConfigUpdateIEs_2))
                                            {
                                                if ( X2AP_FAILURE ==
                                                    x2ap_parse_srerved_nr_cells_endc_x2_modify_list(
                                                            &p_endc_config_update_req->en_gNB.eutra_nr_cell_modify,
                                                            p_endc_configupdate_init_en_gnb_elem->value.u.
                                                            _x2ap_En_gNB_ENDCConfigUpdateIEs_2,
                                                            &message_map,
                                                            (U8)ie_count,
                                                            (U8)p_endc_configupdate_init_en_gnb_elem->id,
                                                            &send_tse,
                                                            p_asn1_ctx
                                                            ))
                                                {
                                                    X2AP_TRACE(X2AP_INFO,"%s: "
                                                            "x2ap_parse_srerved_nr_cells_endc_x2_modify_list"
                                                            "failed",__FUNCTION__);
                                                    retVal = X2AP_FAILURE;                    
                                                    break;
                                                }
                                                
                                                p_endc_config_update_req->en_gNB.bitmask |= 
                                                    X2AP_ENDC_CONFIGUPDATE_GNB_SERVEDNRCELL_MODIFYLIST_PRESENT;
                                                    
                                                /* This will only happen when the element list exceeds
                                                 * the max number */
                                                if (X2AP_TRUE == send_tse)
                                                {
                                                    X2AP_TRACE(X2AP_WARNING, "Error is Detected" 
                                                            "in ASN.1 Sequence, Sending Transfer Syntax Error");
                                                    x2ap_build_and_send_err_indication(p_x2ap_gl_ctx,
                                                        p_enb_ctx, PNULL);
                                                    retVal = X2AP_FAILURE;
                                                    break;
                                                }
                                            }
                                        }
                                        break;

                                        case ASN1V_x2ap_id_ServedNRcellsToDeleteListENDCConfUpd:
                                        {
                                            X2AP_TRACE(X2AP_DETAILED,
                                                    "ASN1V_x2ap_id_ServedNRcellsToDeleteListENDCConfUpd");
                                            if (X2AP_SUCCESS == x2ap_validate_ie_value(
                                                        &message_map,
                                                        genb_index,
                                                        (U8)p_endc_configupdate_init_en_gnb_elem->id,
                                                        p_endc_configupdate_init_en_gnb_elem->value.u.
                                                        _x2ap_En_gNB_ENDCConfigUpdateIEs_3))
                                            {
                                                if ( X2AP_FAILURE ==
                                                    x2ap_parse_srerved_nr_cells_endc_x2_delete_list(
                                                            &p_endc_config_update_req->en_gNB.eutra_nr_cell_delete,
                                                            p_endc_configupdate_init_en_gnb_elem->value.u.
                                                            _x2ap_En_gNB_ENDCConfigUpdateIEs_3,
                                                            &message_map,
                                                            (U8)ie_count,
                                                            (U8)p_endc_configupdate_init_en_gnb_elem->id,
                                                            &send_tse,
                                                            p_asn1_ctx
                                                            ))
                                                {
                                                    X2AP_TRACE(X2AP_INFO,"%s: "
                                                            "x2ap_parse_srerved_nr_cells_endc_x2_delete_list"
                                                            "failed",__FUNCTION__);
                                                    retVal = X2AP_FAILURE;                    
                                                    break;
                                                }

                                                p_endc_config_update_req->en_gNB.bitmask |= 
                                                    X2AP_ENDC_CONFIGUPDATE_GNB_SERVEDNRCELL_DELETELIST_PRESENT;
                                                    
                                                /* This will only happen when the element list exceeds
                                                 * the max number */
                                                if (X2AP_TRUE == send_tse)
                                                {
                                                    X2AP_TRACE(X2AP_WARNING, "Error is Detected" 
                                                            "in ASN.1 Sequence, Sending Transfer Syntax Error");
                                                    x2ap_build_and_send_err_indication(p_x2ap_gl_ctx,
                                                        p_enb_ctx, PNULL);
                                                    retVal = X2AP_FAILURE;
                                                    break;
                                                }
                                            }
                                        }
                                        break;

                                        default:
                                        {
                                            /*
                                             * Class - 1 Error - Unknown IE
                                             */
                                            /*
                                             * Base on Criticality fill the value
                                             */
                                            x2ap_add_to_err_ind_ie_list(&iE_list,
                                                    p_endc_configupdate_init_en_gnb_elem->criticality,
                                                    p_endc_configupdate_init_en_gnb_elem->id,
                                                    &ie_list_index,
                                                    &send_err_ind,
                                                    X2AP_FALSE,
                                                    X2AP_FALSE);
                                            X2AP_TRACE(X2AP_ERROR,"%s:"\
                                                    "invalid Protocol IE id",__FUNCTION__);
                                            retVal = X2AP_FAILURE;
                                        }
                                        break;
                                    }
                                    p_node_genb = p_node_genb->next;
                                }
                                /*
                                 * Parse the map for Error Indication
                                 */
                                if ( ( X2AP_SUCCESS == x2ap_parse_message_map(
                                                p_asn1_ctx,
                                                p_x2ap_gl_ctx,
                                                p_enb_ctx, 
                                                &message_map,
                                                &iE_list,
                                                &ie_list_index,
                                                &send_err_ind,
                                                ASN1V_x2ap_id_endcConfigurationUpdate,
                                                T_x2ap_X2AP_PDU_initiatingMessage,
                                                x2ap_reject,
                                                (rrc_x2ap_error_indication_t *)PNULL) )
                                   )
                                {
                                    X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED ");
                                    retVal = X2AP_FAILURE;
                                }
                                break;
                            }

                            default:
                            X2AP_TRACE(X2AP_WARNING,"%s:"
                                    " invalid x2ap_InitiatingNodeType_EndcConfigUpdate",__FUNCTION__);
                            retVal = X2AP_FAILURE;
                            break;
                        }
                    }
                    break;

                    default:
                    {
                        /*
                         * Class - 1 Error - Unknown IE
                         */
                        /*
                         * Base on Criticality fill the value
                         */
                        x2ap_add_to_err_ind_ie_list(&iE_list,
                                p_x2ap_endc_protocol_IEs->criticality,
                                p_x2ap_endc_protocol_IEs->id,
                                &ie_list_index,
                                &send_err_ind,
                                X2AP_FALSE,
                                X2AP_FALSE);        

                        X2AP_TRACE(X2AP_ERROR,"%s:"\
                                "invalid Protocol IE id",__FUNCTION__);

                        retVal = X2AP_FAILURE;
                    } 
                }
                if ( X2AP_FAILURE == retVal )
                {
                    break;
                }
                else
                {
                    p_node = p_node->next;
                }
            }
            break;
        }
        else
        {
            /*
             * Drop message
             */
            X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                    "decode failed Unknown Message type received.",
                    __FUNCTION__);
            break;
        }
    }while(X2AP_NULL);

    /*
     * Parse the map for Error Indication
     */
    if ( ( X2AP_SUCCESS == x2ap_parse_message_map(
                    p_asn1_ctx,
                    p_x2ap_gl_ctx,
                    p_enb_ctx, 
                    &message_map,
                    &iE_list,
                    &ie_list_index,
                    &send_err_ind,
                    ASN1V_x2ap_id_endcConfigurationUpdate,
                    T_x2ap_X2AP_PDU_initiatingMessage,
                    x2ap_reject,
                    (rrc_x2ap_error_indication_t *)PNULL) )
       )
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED ");
        retVal = X2AP_FAILURE;
    }

    X2AP_UT_TRACE_EXIT();
    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_decode_endc_config_update_ack 
 * Inputs         : p_endc_conf_ack - poiner to x2ap_endc_config_update_ack_t
 *                  msg_result - msg result
 *                  p_asn_msg - pointer to ASN buffer
 *                  p_asn_msg_len - pointer to ASN buffer length  
 * Outputs        : None
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode eNB Configuration Update ACK.
 * Error          : This function report error to the caller function, If decoder
 *                  fails to decode message.
 ********************************************************************************/
x2ap_return_et x2ap_decode_endc_config_update_ack (
        x2ap_endc_config_update_ack_t   *p_endc_conf_ack,
        U32                    *msg_result,
        U8                     *p_asn_msg,
        U32                    	*p_asn_msg_len,
        rrc_x2ap_error_indication_t  *p_error_indication,
        x2ap_error_ind_bool_t        *p_send_err_ind,
        OSCTXT                 	*p_asn1_ctx,
        x2ap_gb_context_t		*p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t	*p_enb_ctx)
{
    x2ap_return_et       retVal = X2AP_SUCCESS;

    x2ap_X2AP_PDU        x2ap_pdu;
    OSRTDListNode        *p_node = X2AP_P_NULL;
    OSRTDListNode        *p_node_2 = X2AP_P_NULL;
    
    x2ap_ENDCConfigurationUpdateAcknowledge
        *p_x2ap_ENDCConfigurationUpdateAcknowledge = X2AP_P_NULL;
        
    x2ap_ENDCConfigurationUpdateAcknowledge_protocolIEs_element
        *p_ENDC_config_update_ack_protocolIEs_elem = X2AP_P_NULL;
        
    x2ap_RespondingNodeType_EndcConfigUpdate_respond_en_gNB_element
        *p_ENDC_ConfigUpdateAck_en_gnb_elem = X2AP_P_NULL;
        
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    U32                         encoded_msg_len = 0;
    U32                         count = 0;
    U16                         index = 0;

    x2ap_error_ind_ie_list_t   iE_list;
    iE_list.ie_cnt = RRC_ZERO;
    U16 ie_list_index = 0;

    x2ap_message_data_t message_map =
    {1, X2AP_FALSE,X2AP_FALSE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_RespondingNodeType_EndcConfigUpdate, x2ap_mandatory, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
        }};

    if (!p_endc_conf_ack)
    {
        X2AP_TRACE(X2AP_ERROR,  "%s: ASN "\
                "p_enb_conf_ack is NULL",__FUNCTION__);
        return X2AP_FAILURE;
    }

    /* Init ASN1 context */
    if ( 0 != rtInitContext(p_asn1_ctx) )
    {
        /* Drop message */
        X2AP_TRACE(X2AP_ERROR,  "%s: ASN "\
                "context initialization failed.",__FUNCTION__);
        return X2AP_FAILURE;
    }

    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(p_asn1_ctx, p_asn_msg, *p_asn_msg_len, TRUE);

    do
    {
        if (0 != asn1PD_x2ap_X2AP_PDU(p_asn1_ctx, &x2ap_pdu) )
        {
            /* Drop message */
            X2AP_TRACE(X2AP_ERROR, "%s: ASN "\
                    "decode failed.",__FUNCTION__);
            retVal = X2AP_FAILURE;
            break;
        }
        else
        {
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);

            encoded_msg_len = (U32)pe_GetMsgLen(p_asn1_ctx);

            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    encoded_msg_len);
                    
            if (PNULL != p_event_header)
            {
                x2ap_fill_protocol_event_params(p_event_header, encoded_msg_len,
                        LOCAL_X2_ENB_CONFIGURATION_UPDATE_ACKNOWLEDGE, 
                        MESSAGE_DIRECTION_RECEIVED);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        encoded_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

        if ((T_x2ap_X2AP_PDU_successfulOutcome == x2ap_pdu.t) ||
                (ASN1V_x2ap_id_endcConfigurationUpdate == x2ap_pdu.u.successfulOutcome->procedureCode) ||
                ((X2AP_P_NULL != x2ap_pdu.u.successfulOutcome->value.u.endcConfigurationUpdate) &&
                 (T1x2ap__endcConfigurationUpdate == x2ap_pdu.u.successfulOutcome->value.t)))
        {
            *msg_result = T_x2ap_X2AP_PDU_successfulOutcome;
            
            p_x2ap_ENDCConfigurationUpdateAcknowledge = 
                x2ap_pdu.u.successfulOutcome->value.u.endcConfigurationUpdate;

            p_node = p_x2ap_ENDCConfigurationUpdateAcknowledge->protocolIEs.head;

            for (count =0;
                    count < p_x2ap_ENDCConfigurationUpdateAcknowledge->protocolIEs.count; count++)
            {
                if (!p_node)
                {
                    X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                            __FUNCTION__);
                    retVal = X2AP_FAILURE;
                    break;
                }

                p_ENDC_config_update_ack_protocolIEs_elem =
                    (x2ap_ENDCConfigurationUpdateAcknowledge_protocolIEs_element *)p_node->data;

                switch (p_ENDC_config_update_ack_protocolIEs_elem->id)
                {
                    case ASN1V_x2ap_id_RespondingNodeType_EndcConfigUpdate:
                    {
                        X2AP_TRACE(X2AP_INFO,"%s: "
                                "ASN1V_x2ap_id_RespondingNodeType_EndcConfigUpdate",__FUNCTION__);
                                
                        if (p_ENDC_config_update_ack_protocolIEs_elem->value.u.
                                    _x2ap_ENDCConfigurationUpdateAcknowledge_IEs_1->t == 
                                    T_x2ap_RespondingNodeType_EndcConfigUpdate_respond_en_gNB)
                        {
                            x2ap_message_data_t message_map =
                                {1, X2AP_TRUE,X2AP_TRUE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
                                    {{0, ASN1V_x2ap_id_ServedNRcellsENDCX2ManagementList, x2ap_optional, x2ap_reject,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL}
                                    }};

                            x2ap_bool_et send_tse = X2AP_FALSE;
                                
                            //p_endc_conf_ack->bitmask |= X2AP_ENDC_CONFIG_UPDATE_RESPOND_EN_GNB_PRESENT;

                            p_node_2 = p_ENDC_config_update_ack_protocolIEs_elem->value.u.
                                    _x2ap_ENDCConfigurationUpdateAcknowledge_IEs_1->u.respond_en_gNB->head;    

                            for ( index = X2AP_NULL; index < p_ENDC_config_update_ack_protocolIEs_elem->
                                        value.u._x2ap_ENDCConfigurationUpdateAcknowledge_IEs_1->u.respond_en_gNB->count;
                                        index++ )
                            {
                                if (!p_node_2)
                                {
                                    X2AP_TRACE(X2AP_ERROR,"%s: ""next node returned NULL",
                                            __FUNCTION__);
                                    retVal = X2AP_FAILURE;
                                    break;
                                 }

                                 p_ENDC_ConfigUpdateAck_en_gnb_elem = 
                                        (x2ap_RespondingNodeType_EndcConfigUpdate_respond_en_gNB_element *)p_node_2->data;

                                 switch(p_ENDC_ConfigUpdateAck_en_gnb_elem->id)
                                 {
                                     case ASN1V_x2ap_id_ServedNRcellsENDCX2ManagementList:
                                     {
                                         X2AP_TRACE(X2AP_INFO,"%s: "\
                                                 "ASN1V_x2ap_id_ServedNRcellsENDCX2ManagementList",__FUNCTION__);   

                                         p_endc_conf_ack->bitmask |= X2AP_ENDC_CONFIG_UPDATE_RESPOND_EN_GNB_PRESENT;        

                                         if(X2AP_FAILURE == x2ap_parse_srerved_nr_cells_endc_x2_management_list(
                                                    &p_endc_conf_ack->respond_en_gnb,
                                                    p_ENDC_ConfigUpdateAck_en_gnb_elem->value.u._x2ap_En_gNB_ENDCConfigUpdateAckIEs_1,
                                                    &message_map,
                                                    (U8)count,
                                                    (U8)p_ENDC_ConfigUpdateAck_en_gnb_elem->id,
                                                    &send_tse,
                                                    p_asn1_ctx))
                                         {
                                             X2AP_TRACE(X2AP_INFO,"%s: x2ap_parse_ServedNRcellsENDCX2ManagementList"
                                                     "failed",__FUNCTION__);
                                             break;

                                         }

                                     }
                                     break;
                                     default:
                                         X2AP_TRACE(X2AP_ERROR,"%s: invalid Protocol IE id",__FUNCTION__);
                                         break;
                                 }   
                                 p_node_2 = p_node_2->next;
                            } 
                        }

                    }break;
                    
                    default: 
                    {
                        X2AP_TRACE(X2AP_ERROR, "%s: invalid Protocol IE id",__FUNCTION__);
                        x2ap_add_to_err_ind_ie_list(&iE_list,
                                p_ENDC_config_update_ack_protocolIEs_elem->criticality,
                                p_ENDC_config_update_ack_protocolIEs_elem->id,
                                &ie_list_index,
                                p_send_err_ind,
                                X2AP_FALSE,
                                X2AP_FALSE);
                                
                        if (x2ap_reject == p_ENDC_config_update_ack_protocolIEs_elem->criticality) 
                        {
                            retVal = X2AP_FAILURE;
                        }
                    }
                    break;
                }
                p_node = p_node->next;
            }
        }
        else
        {
            X2AP_TRACE(X2AP_ERROR,  "%s: ASN "\
                    "decode failed Unknown Message type received.",
                    __FUNCTION__);
            retVal = X2AP_FAILURE;
            break; 
        }
    }while(0);

    /* Parse the map for Error Indication */
    if ((X2AP_SUCCESS == x2ap_parse_message_map(
                    p_asn1_ctx,
                    p_x2ap_gb_ctx,
                    p_enb_ctx, 
                    &message_map,
                    &iE_list,
                    &ie_list_index,
                    p_send_err_ind,
                    ASN1V_x2ap_id_endcConfigurationUpdate,
                    T_x2ap_X2AP_PDU_successfulOutcome,
                    x2ap_reject,
                    p_error_indication)))
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED ");

        retVal = X2AP_FAILURE;
    }

    return retVal;
}

/*****************************************************************************
 * Function Name  : x2ap_decode_endc_config_update_resp_fail 
 * Inputs         : p_endc_conf_resp_fail - pointer to x2ap_endc_config_update_failure_t
 *                  msg_result - pointer to msg result
 *                  p_asn_msg - pointer to ASN message
 *                  p_asn_msg_len - pointer to ASN message length 
 * Outputs        : p_asn_msg,p_asn_msg_len
 * Returns        : X2AP_SUCCESS/X2AP_FAILURE
 * Description    : This function used to decode ENDC Configuration Update Failure
 *                   Response.
 * Error          : This function report error to the caller function, If decoder
 *                  fails to decode message.
 ********************************************************************************/
x2ap_return_et x2ap_decode_endc_config_update_resp_fail(
        x2ap_endc_config_update_failure_t   *p_endc_conf_resp_fail,
        U32                         *msg_result,
        U8                          *p_asn_msg,
        U32                         *p_asn_msg_len, 
        x2ap_gb_context_t	    *p_x2ap_gb_ctx,
        x2ap_peer_enb_context_t	    *p_enb_ctx,
        rrc_x2ap_error_indication_t *p_error_indication,
        x2ap_error_ind_bool_t       *p_send_err_ind,
        OSCTXT                	    *p_asn1_ctx) 
{
    x2ap_return_et       retVal = X2AP_FAILURE;

    U32                  count = 0;
    x2ap_X2AP_PDU        x2ap_pdu;
    x2ap_ENDCConfigurationUpdateFailure  *p_ENDCConfigurationUpdateFailure;
    OSRTDListNode        *p_node = X2AP_P_NULL;
    x2ap_ENDCConfigurationUpdateFailure_protocolIEs_element
        *p_ENDCConfigUpdRespFail_protocol_IEs = X2AP_P_NULL;
    EVENT_EXTERNAL_HEADER       *p_event_header = PNULL;
    U32                         encoded_msg_len = 0;
    x2ap_error_ind_ie_list_t   iE_list;
    iE_list.ie_cnt = RRC_ZERO;
    U16 ie_list_index = 0;

    x2ap_message_data_t message_map =
    {3, X2AP_TRUE,X2AP_TRUE, X2AP_NULL, X2AP_NULL, X2AP_NULL,
        {{0, ASN1V_x2ap_id_Cause, x2ap_mandatory, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {1, ASN1V_x2ap_id_CriticalityDiagnostics, x2ap_optional, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL},
            {2, ASN1V_x2ap_id_TimeToWait, x2ap_optional, x2ap_ignore,X2AP_NULL,X2AP_NULL, X2AP_NULL, X2AP_NULL}
        }};

    if (!p_endc_conf_resp_fail)
    {
        X2AP_TRACE(X2AP_ERROR,  "%s: ASN "\
                "p_enb_conf_ack is NULL",__FUNCTION__);
        return X2AP_FAILURE;
    }

    if ( 0 != rtInitContext(p_asn1_ctx) )
    {
        /* Drop message */
        X2AP_TRACE(X2AP_ERROR,  "%s: ASN "\
                "context initialization failed.",__FUNCTION__);
        return X2AP_FAILURE;
    }

    /*Set pointer of asn buffer in asn context*/
    pu_setBuffer(p_asn1_ctx, p_asn_msg, *p_asn_msg_len, TRUE);

    do
    {
        if (0 != asn1PD_x2ap_X2AP_PDU(p_asn1_ctx, &x2ap_pdu) )
        {
            /* Drop message */
            X2AP_TRACE(X2AP_ERROR,  "%s: ASN "\
                    "decode failed.",__FUNCTION__);
            retVal = X2AP_FAILURE;
            break;
        }
        else
        {
            rrc_asn1PrtToStr_X2AP_PDU(X2AP_ASN, "X2AP_PDU", &x2ap_pdu);
            encoded_msg_len = (U32)pe_GetMsgLen(p_asn1_ctx); 
            /*SPR_19067_FIX_STOP*/

            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    encoded_msg_len);
            if (PNULL != p_event_header)
            {
                x2ap_fill_protocol_event_params(p_event_header, encoded_msg_len,
                        LOCAL_X2_ENB_CONFIGURATION_UPDATE_FAILURE, 
                        MESSAGE_DIRECTION_RECEIVED);

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_asn_msg,
                        encoded_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
        }

        /* check whether the message is not successful */
        if ((T_x2ap_X2AP_PDU_unsuccessfulOutcome == x2ap_pdu.t) ||
                (ASN1V_x2ap_id_endcConfigurationUpdate ==
                 x2ap_pdu.u.unsuccessfulOutcome->procedureCode) ||
                ((X2AP_P_NULL != x2ap_pdu.u.unsuccessfulOutcome->value.u.endcConfigurationUpdate) &&
                 (T1x2ap__endcConfigurationUpdate == 
                  x2ap_pdu.u.unsuccessfulOutcome->value.t)))
        {
            *msg_result = T_x2ap_X2AP_PDU_unsuccessfulOutcome;
            
            p_ENDCConfigurationUpdateFailure = 
                x2ap_pdu.u.unsuccessfulOutcome->value.u.endcConfigurationUpdate;
                
            p_node = p_ENDCConfigurationUpdateFailure->protocolIEs.head;

            for (count =0; count < p_ENDCConfigurationUpdateFailure->protocolIEs.count; count++)
            {
                if (!p_node)
                {
                    X2AP_TRACE(X2AP_ERROR, "%s: ""next node returned NULL",
                            __FUNCTION__);
                    retVal = X2AP_FAILURE;
                    break;
                }
                
                p_ENDCConfigUpdRespFail_protocol_IEs =
                    (x2ap_ENDCConfigurationUpdateFailure_protocolIEs_element*)p_node->data;
                    
                switch (p_ENDCConfigUpdRespFail_protocol_IEs->id)
                {
                    case ASN1V_x2ap_id_Cause:
                    {
                        X2AP_TRACE(X2AP_INFO, "%s: "\
                                "ASN1V_x2ap_id_Cause",__FUNCTION__);
                                
                        retVal = x2ap_parse_cause(
                                &(p_endc_conf_resp_fail->cause),
                                p_ENDCConfigUpdRespFail_protocol_IEs->value.u._x2ap_ENDCConfigurationUpdateFailure_IEs_1,
                                PNULL,
                                X2AP_NULL,
                                X2AP_NULL);
                    }            
                    break;

                    case ASN1V_x2ap_id_CriticalityDiagnostics:
                    {
                        X2AP_TRACE(X2AP_INFO, "%s: "\
                                "ASN1V_x2ap_id_CriticalityDiagnostics",__FUNCTION__);
                                
                        retVal = x2ap_parse_CriticalityDiagnostics(
                                &(p_endc_conf_resp_fail->criticality_diagnostics),
                                p_ENDCConfigUpdRespFail_protocol_IEs->value.u._x2ap_ENDCConfigurationUpdateFailure_IEs_2);
                                
                        p_endc_conf_resp_fail->bitmask |= X2AP_ENDC_CONFIG_UPDATE_FAILURE_CRITICALITY_DIAGNOSTICS_PRESENT;
                    }
                    break;
    
                    case ASN1V_x2ap_id_TimeToWait:
                    {
                        X2AP_TRACE(X2AP_INFO, "%s: "\
                                "ASN1V_x2ap_id_TimeToWait",__FUNCTION__);
                                
                        p_endc_conf_resp_fail->time_to_wait =
                            (x2ap_time_to_wait_et)p_ENDCConfigUpdRespFail_protocol_IEs->value.u._x2ap_ENDCConfigurationUpdateFailure_IEs_3;

                        p_endc_conf_resp_fail->bitmask = X2AP_TIME_TO_WAIT_INDEX_PRESENT;
                        retVal = X2AP_SUCCESS;
                    }
                    break;
                    
                    default:
                    X2AP_TRACE(X2AP_ERROR, "%s:"\
                            "invalid Protocol IE id",__FUNCTION__);

                    x2ap_add_to_err_ind_ie_list(&iE_list,
                            p_ENDCConfigUpdRespFail_protocol_IEs->criticality,
                            p_ENDCConfigUpdRespFail_protocol_IEs->id,
                            &ie_list_index,
                            p_send_err_ind,
                            X2AP_FALSE,
                            X2AP_FALSE);

                    if (x2ap_reject == p_ENDCConfigUpdRespFail_protocol_IEs->criticality) 
                    {
                        retVal = X2AP_FAILURE;
                    }
                    
                    if (X2AP_FAILURE == retVal)
                    {
                        X2AP_TRACE(X2AP_ERROR,  "%s: ASN "\
                                "decode failed while decoding parameter id [%d].",
                                __FUNCTION__,p_ENDCConfigUpdRespFail_protocol_IEs->id);
                        break;
                    }
                }

                p_node = p_node->next;
            }
        }
        else
        {
            /* Drop message */
            X2AP_TRACE(X2AP_ERROR,  "%s: ASN "\
                    "decode failed Unknown Message type received.",
                    __FUNCTION__);
            retVal = X2AP_FAILURE;
            break;
        }


    }while(0);

    /* Parse the map for Error Indication */
    if ((X2AP_SUCCESS == x2ap_parse_message_map(
                    p_asn1_ctx,
                    p_x2ap_gb_ctx,
                    p_enb_ctx, 
                    &message_map,
                    &iE_list,
                    &ie_list_index,
                    p_send_err_ind,
                    ASN1V_x2ap_id_endcConfigurationUpdate,
                    T_x2ap_X2AP_PDU_unsuccessfulOutcome,
                    x2ap_reject,
                    p_error_indication)))
    {
        X2AP_TRACE(X2AP_INFO, "ERROR INDICATION DETECTED ");

        retVal = X2AP_FAILURE;
    } 

    return retVal;
}

/*EN-DC_changes_end*/
#endif
