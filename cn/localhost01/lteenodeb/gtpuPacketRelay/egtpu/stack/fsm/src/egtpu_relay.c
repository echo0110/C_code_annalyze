/***********************************************************************
** FILE
**     egtpu_relay.c
**
************************************************************************
**
** DESCRIPTION
**     This file has all relay entity related API handling functions
**
**  DATE      NAME        	REFERENCE    	REASON
**  ---       ----        	---------    	------
**  Oct,2009 Vipul Aggarwal					Initial
**
**  Copyright (C) 2009 Aricent Inc . All Rights Reserved
***********************************************************************/

#include <egtpu_glob.h>
#include <egtpu_apihdr.h>

#include <egtpu_msgtypes.h>
#include <egtpu_ietypes.h>
#include <egtpu_error.h>
#include <egtpu_stat.h>
#include <egtpu_glob_struct.h>
#include <egtpu_cdb.h>
#include <egtpu_os_proto.h>
#include <egtpu_trace.h>
#include <egtpu_util.h>


/********************************************************************************
 * Function Name  : egtpu_reloc_set_gtp_header
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t,
 *                  p_rab_ctxt - Pointer to egtpu_rab_ctxt_t,
 *                  p_msg,
 *                  pdcp_flag,
 *                  seq_flag,
 *                  len
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This function sets the GTP Header during relocation.
 ********************************************************************************/
STATIC inline egtpu_return_t egtpu_reloc_set_gtp_header (
                                    egtpu_global_t   *p_global,
                                    egtpu_info_t     *p_egtpu_info,
                                    egtpu_rab_ctxt_t *p_rab_ctxt,
                                    U8bit            *p_msg,
                                    U8bit             pdcp_flag,
                                    U8bit             seq_flag,
                                    U16bit            len )
{
    if (pdcp_flag)
    {
        /*As Y tunneling is disabled, changing forward_addr to transport_addr*/
        if (!(p_rab_ctxt->p_transport_addr->ext_hdr &
                    EGTPU_EXT_HDR_PDCP))
        { /* peer dont supp pdcp# hdr */
            EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_EXT_HDR_NOT_SUPP,
                        egtpu_entity_name_arr[EGTPU_EI_GTPU],
                        EGTPU_EXT_HDR_PDCP));

            /* SPR 2030 fix Start*/
            LOG_GTPU_MSG(GTP_RELOC_DATA_REQ_NO_EXT_HDR_SUPP, LOGWARNING, PR_GTPU_UL_DATA,
                    relayGtpuCurrentTick_g, __LINE__, 
                    0,0,0,0, 0,0, __func__, "");
            /* SPR 2030 fix End*/
            /* SPR 2007 Fix - Part2 Start*/
            /* EGTPU_FREE_MSG_BUF (p_msg);*/
            /* SPR 2007 Fix - Part2 End*/
            return EGTPU_FAILURE;
        }

        EGTPU_API_SET_START_PAYLOAD (p_msg,
                (EGTPU_API_GET_START_PAYLOAD (p_msg) - EGTPU_HDR_LEN_WITH_PDCP_NUM));
        /* SPR 18822 Fix Start */
        /* GTP Header will be filled with PDCP PDU Exension,Seq No and TEID*/
        EGTPU_SET_GTP_HDR (EGTPU_API_GET_PAYLOAD_STRT_PTR (p_msg),
                EGTPU_PDCP_PDU_EXT_HDR, seq_flag, EGTPU_UNSET_PN_FLAG,
                EGTPU_MSGT_TPDU, (len + 8), 
                /* SPR 10787 fix start */
                p_rab_ctxt->teid_cell.teid_peer, 
                /* SPR 10787 fix end */
                p_egtpu_info->seq_num_dnlk, EGTPU_SPARE_BYTE,
                p_egtpu_info->pdcp_num);
        /* SPR 18822 Fix End */
    }
    else
    {
        EGTPU_API_SET_START_PAYLOAD (p_msg,
                (EGTPU_API_GET_START_PAYLOAD (p_msg) -
                 ((seq_flag) ? EGTPU_HDR_LEN_WITH_SEQ_NUM : EGTPU_MIN_HDR_LEN)));

        EGTPU_SET_GTP_HDR (EGTPU_API_GET_PAYLOAD_STRT_PTR(p_msg),
                EGTPU_UNSET_EXT_HDR_FLAG, seq_flag, EGTPU_UNSET_PN_FLAG,
                EGTPU_MSGT_TPDU, ((seq_flag) ? len + 4 : len),
                /* SPR 10787 fix start */
                p_rab_ctxt->teid_cell.teid_peer,  
                /* SPR 10787 fix end */
                p_egtpu_info->seq_num_dnlk,
                EGTPU_SPARE_BYTE, EGTPU_SPARE_BYTE);
    }
    return EGTPU_SUCCESS;
}
/* Cyclomatic Complexity changes - ends here */

/********************************************************************************
 * Function Name  : egtpu_data_req
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This function Processes EGTPU_DATA_REQ API from relay.
 *                  This function forwards the PDUs received to peer.
 ********************************************************************************/
egtpu_return_t egtpu_data_req(egtpu_global_t *p_global,
								egtpu_info_t *p_egtpu_info)
{
    egtpu_rab_ctxt_t *p_rab_ctxt;
    U8bit           *p_msg = p_egtpu_info->p_buff,
                    *p_param = p_msg + EGTPU_API_HDR_LEN,addr_len;
    U16bit          msg_len = EGTPU_API_GET_PAYLOAD_LEN (p_msg);
    egtpu_return_t   ret_val;

    EGTPU_UPDATE_STATS(p_global,EGTPU_STATS_API_DATA_REQ);

    p_rab_ctxt = egtpu_db_sel_teid_key_rab_ctxt
        (p_global,EGTPU_GET_U32BIT (p_param + EGTPU_SIZE_OF_TYPE));
    if (!p_rab_ctxt)
	{   /* teid not found */
        EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_DBERR_KEY_NOT_PRESENT,
                    egtpu_entity_name_arr[EGTPU_EI_GTPU]));

        EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_KEY_NOT_PRESENT);
        /* SPR 2030 fix Start*/
/* SPR 20430 Changes Start*/
        LOG_GTPU_MSG(GTP_UL_DATA_FOR_UNKNOWN_RAB, LOGWARNING, PR_GTPU_UL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                EGTPU_GET_ADDRESS (p_param + EGTPU_SIZE_OF_TYPE),
                0,0,0, 0,0, __func__,"");
/* SPR 20430 Changes End*/
        /* SPR 2030 fix End*/

       /* EGTPU_FREE_MSG_BUF (p_msg);*/
        return EGTPU_FAILURE;
    }

    if (!(p_rab_ctxt->sap_flag & EGTPU_FROM_RELAY_SAP) ||
            !(p_rab_ctxt->sap_flag & EGTPU_TX_SAP) ||
            !p_rab_ctxt->p_transport_addr)
	{
        EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_UNEXPECTED_API,
                    egtpu_entity_name_arr[EGTPU_EI_GTPU],
                    egtpu_in_api_name_arr[EGTPU_EI_RELAY][EGTPU_DATA_REQ -
                    egtpu_in_api_base_arr[EGTPU_EI_RELAY]]));

        /* SPR 2007 Fix - Part2 Start*/
       /* EGTPU_FREE_MSG_BUF (p_msg);*/
       /* SPR 2007 Fix - Part2 End*/

        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTP_UL_TUNNEL_NO_TX, LOGWARNING, PR_GTPU_UL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                0,0,0,0, 0,0, __func__, "");
        /* SPR 2030 fix End*/
        return EGTPU_FAILURE;
    }
    /* Coverity 73008 Fix Start*/
    if(EGTPU_GET_U16BIT (p_rab_ctxt->p_transport_addr->entity_addr) > EGTPU_SIZE_OF_IPV6_ADDR)
    {
        return EGTPU_FAILURE;
    }
    /* Coverity 73008 Fix End*/

	/** Here, we are making use of zero copy buffer **/
	/** For the same, the payload always starts at an offset of 64 **/
	/** So the spare bytes between the API header and the payload are used
	as below to copy the parameters and GTP header **/
    EGTPU_FILL_PEER_INFO (p_param,
            EGTPU_UDP_PORT, p_rab_ctxt->p_transport_addr->entity_addr);

    addr_len = EGTPU_GET_U16BIT(p_rab_ctxt->p_transport_addr->entity_addr);
    
    EGTPU_SET_U32BIT (p_param, p_rab_ctxt->qos_id);
    p_param += EGTPU_SIZE_OF_QOS_ID;

    /* Set API Header, msg-len and end-payload shall be same */
    EGTPU_API_SET_SRC_ID (p_msg, EGTPU_EI_GTPU);

    if (addr_len == EGTPU_SIZE_OF_IPV4_ADDR)
	{
		EGTPU_API_SET_DST_ID (p_msg, EGTPU_EI_UDP);
	}
	else
	{
		EGTPU_API_SET_DST_ID (p_msg, EGTPU_EI_UDP6);

    }
    
    EGTPU_API_SET_API_ID (p_msg, EGTPU_UDP_UNITDATA_REQ);

    if (p_rab_ctxt->seq_disable_flag) {
        EGTPU_API_SET_START_PAYLOAD (p_msg,
                (EGTPU_API_GET_START_PAYLOAD (p_msg) - EGTPU_MIN_HDR_LEN));
    }
    else
	{
        EGTPU_API_SET_START_PAYLOAD (p_msg,
            (EGTPU_API_GET_START_PAYLOAD(p_msg) - EGTPU_HDR_LEN_WITH_SEQ_NUM));
		msg_len += EGTPU_SIZE_OF_SEQ_NUM + 2;
	}
                
    EGTPU_API_SET_END_PARAM_LIST (p_msg, (p_param - p_msg - 1));

    /* Filling the GTP Header; */
    EGTPU_SET_GTP_HDR(EGTPU_API_GET_PAYLOAD_STRT_PTR(p_msg),
            EGTPU_UNSET_EXT_HDR_FLAG, !(p_rab_ctxt->seq_disable_flag),
			EGTPU_UNSET_PN_FLAG, EGTPU_MSGT_TPDU, msg_len,
            /* SPR 10787 fix start */
			p_rab_ctxt->teid_cell.teid_peer, p_rab_ctxt->seq_num_uplk,
            /* SPR 10787 fix end */
			EGTPU_SPARE_BYTE, EGTPU_SPARE_BYTE);

    if (!(p_rab_ctxt->seq_disable_flag))
	{
        p_rab_ctxt->seq_num_uplk++;
    }
    

    if(addr_len == EGTPU_SIZE_OF_IPV4_ADDR)
	{
	    EGTPU_SEND_FROM_GTPU (p_global,p_msg,
            EGTPU_API_GET_MSG_LEN (p_msg), EGTPU_EI_UDP, ret_val,
									p_rab_ctxt->src_addr);
	}
	else
	{
		EGTPU_SEND_FROM_GTPU (p_global,p_msg,
            EGTPU_API_GET_MSG_LEN (p_msg), EGTPU_EI_UDP6, ret_val,
									p_rab_ctxt->src_addr);
    }

    /* SPR 2030 fix Start*/
     LOG_UT(GTP_UL_DATA_SENT_TO_PEER, LOGINFO, PR_GTPU_UL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                0,0,0,0, 0,0, __func__, "");
     /* SPR 2030 fix End*/
    
    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_UDP_UNITDATA_REQ);
    EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_PDU_SENT);

    return ret_val;
}

/********************************************************************************
 * Function Name  : egtpu_reloc_data_req
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This function forwards dnlk pdus to peer during relocation,
 *                  This function forwards the PDUs of inter sgw ctxt tfr case.
 ********************************************************************************/
egtpu_return_t egtpu_reloc_data_req(egtpu_global_t *p_global,
									egtpu_info_t *p_egtpu_info)
{
    egtpu_rab_ctxt_t *p_rab_ctxt;
    U8bit           *p_msg = p_egtpu_info->p_buff,
                    *p_param = p_msg + EGTPU_API_HDR_LEN, 
                    *p_start = p_msg + EGTPU_API_HDR_LEN,
                    *p_end = EGTPU_API_GET_END_PARAM_LIST_PTR (p_msg),
                    seq_flag= EGTPU_FALSE, 
                    pdcp_flag = EGTPU_FALSE,addr_len;

    U16bit          len = EGTPU_API_GET_PAYLOAD_LEN (p_msg);
    egtpu_return_t   ret_val;

    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_RELOC_DATA_REQ);

    while (p_start < p_end)
        switch (*p_start++) {
            case EGTPU_IE_TEID_DATA_I:
                p_egtpu_info->teid_self = EGTPU_GET_U32BIT (p_start);
                p_start += EGTPU_SIZE_OF_TEID;
                break;

            case EGTPU_IE_SEQ_NUM:
                p_egtpu_info->seq_num_dnlk = EGTPU_GET_U16BIT (p_start);
                seq_flag = EGTPU_TRUE;
                p_start += EGTPU_SIZE_OF_SEQ_NUM;
                break;

            case EGTPU_IE_PDCP_NUM:
                p_egtpu_info->pdcp_num = EGTPU_GET_U16BIT (p_start);
                pdcp_flag = EGTPU_TRUE;
                p_start += EGTPU_SIZE_OF_PDCP_NUM;
                break;
        }

    p_rab_ctxt = egtpu_db_sel_teid_key_rab_ctxt
        (p_global,p_egtpu_info->teid_self);
    if (!p_rab_ctxt)
    {   /* teid not found */
        EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_DBERR_KEY_NOT_PRESENT,
                    egtpu_entity_name_arr[EGTPU_EI_GTPU]));

        EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_KEY_NOT_PRESENT);
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTP_RELOC_DATA_REQ_FOR_UNKNOWN_RAB, LOGWARNING, PR_GTPU_UL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                p_egtpu_info->teid_self,
                0,0,0, 0,0, __func__,"");
        /* SPR 2030 fix End*/

        /* SPR 2007 Fix - Part2 Start*/
        /* EGTPU_FREE_MSG_BUF (p_msg);*/
        /* SPR 2007 Fix - Part2 End*/
        return EGTPU_FAILURE;
    }
    /* Handover Coding - Prashant Changes Start */

    /*  As Y tunnel concept is disabled as part of setup SAP req 
     *  for S1-HO so EGTPU_RELOC_DATA_SAP and EGTPU_FWD_SAP
     *  checks are not required to forward the data over 
     *  indirect tunnels. 
     *  we need to add checks for EGTPU_FROM_RELAY_SAP and 
     *  EGTPU_TX_SAP as doing in egtpu_data_req. 
     */

    if (!(p_rab_ctxt->sap_flag & EGTPU_FROM_RELAY_SAP) ||
            !(p_rab_ctxt->sap_flag & EGTPU_TX_SAP) ||
            !p_rab_ctxt->p_transport_addr)
    {
        EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_UNEXPECTED_API,
                    egtpu_entity_name_arr[EGTPU_EI_GTPU],
                    egtpu_in_api_name_arr[EGTPU_EI_RELAY][EGTPU_DATA_REQ -
                    egtpu_in_api_base_arr[EGTPU_EI_RELAY]]));

        LOG_GTPU_MSG(GTP_UL_TUNNEL_NO_TX, LOGWARNING, PR_GTPU_UL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                0,0,0,0, 0,0, __func__, "");

        return EGTPU_FAILURE;
    }
    /* Handover Coding - Prashant Changes End */

    /* Cyclomatic Complexity changes - starts here */
    if (EGTPU_SUCCESS != egtpu_reloc_set_gtp_header (p_global, p_egtpu_info, 
                p_rab_ctxt, p_msg, pdcp_flag, seq_flag, len))
    {
        return EGTPU_FAILURE;
    }
    /* Cyclomatic Complexity changes - ends here */

    /* Handover Coding - Prashant Changes Start */

    /* As Y Tunnel is disabled in S1- HO so eGTPU shall fill the 
     * peer address from p_transport_addrp_transport_addr */
    
    /* Coverity 73368 Fix Start*/
    if(EGTPU_GET_U16BIT (p_rab_ctxt->p_transport_addr->entity_addr) > EGTPU_SIZE_OF_IPV6_ADDR)
    {
        return EGTPU_FAILURE;
    }
    /* Coverity 73368 Fix End*/

    /* EGTPU_FILL_PEER_INFO (p_param,
     *       EGTPU_UDP_PORT, p_rab_ctxt->p_forward_addr->entity_addr);
     * addr_len = EGTPU_GET_U16BIT(p_rab_ctxt->p_forward_addr->entity_addr);*/

    EGTPU_FILL_PEER_INFO (p_param,
            EGTPU_UDP_PORT, p_rab_ctxt->p_transport_addr->entity_addr);

    addr_len = EGTPU_GET_U16BIT(p_rab_ctxt->p_transport_addr->entity_addr);

    /* SPR 14469 Changes Start */
    EGTPU_SET_U32BIT (p_param, p_rab_ctxt->qos_id);
    p_param += EGTPU_SIZE_OF_QOS_ID;
    /* SPR 14469 Changes End */
    
    /* Handover Coding - Prashant Changes End */

    /* Set API Header */
    EGTPU_API_SET_SRC_ID (p_msg, EGTPU_EI_GTPU);

    if(addr_len == EGTPU_SIZE_OF_IPV4_ADDR)
    {
        EGTPU_API_SET_DST_ID (p_msg, EGTPU_EI_UDP);
    }
    else
    {
        EGTPU_API_SET_DST_ID (p_msg, EGTPU_EI_UDP6);
    }


    EGTPU_API_SET_API_ID (p_msg, EGTPU_UDP_UNITDATA_REQ);

    EGTPU_API_SET_END_PARAM_LIST (p_msg, (p_param - p_msg - 1));

    if(addr_len == EGTPU_SIZE_OF_IPV4_ADDR)
    {
        EGTPU_SEND_FROM_GTPU (p_global,p_msg, EGTPU_API_GET_MSG_LEN(p_msg),
                EGTPU_EI_UDP, ret_val, p_rab_ctxt->src_addr);
    }
    else
    {
        EGTPU_SEND_FROM_GTPU (p_global,p_msg, EGTPU_API_GET_MSG_LEN(p_msg),
                EGTPU_EI_UDP6, ret_val, p_rab_ctxt->src_addr);
    }

    /* SPR 2030 fix Start*/
    LOG_GTPU_MSG(GTP_RELOC_DATA_REQ_PROCESSED, LOGINFO, PR_GTPU_UL_DATA,
            relayGtpuCurrentTick_g, __LINE__, 
            0,0,0,0, 0,0, __func__, "");
    /* SPR 2030 fix End*/

    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_UDP_UNITDATA_REQ);
    EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_PDU_SENT);

    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_LM_INITDB_CNF);
    /* + coverity 32081 */
    return ret_val;
    /* - coverity 32081 */
}

/********************************************************************************
 * Function Name  : egtpu_send_emi_to_peer_req
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This function sends End Marker Indication to Peer during HO,
 *                  by finding peer info based on teif_self received in request.
 ********************************************************************************/
egtpu_return_t egtpu_send_emi_to_peer_req(egtpu_global_t *p_global,
									egtpu_info_t *p_egtpu_info)
{
    egtpu_rab_ctxt_t *p_rab_ctxt;
    U8bit           *p_param = p_egtpu_info->p_buff + EGTPU_API_HDR_LEN,
    addr_len = 0;
    egtpu_return_t   ret_val = EGTPU_FAILURE;
    U8bit *p_buf = EGTPU_NULL;
    U8bit         *p_trav = EGTPU_NULL;
    U16bit        st_payld;

    /*Search the RAB context based on teid-Self received*/
    p_rab_ctxt = egtpu_db_sel_teid_key_rab_ctxt
        (p_global,EGTPU_GET_U32BIT (p_param));

    if (!p_rab_ctxt)
	{   /* teid not found */
        EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_DBERR_KEY_NOT_PRESENT,
                    egtpu_entity_name_arr[EGTPU_EI_GTPU]));

        EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_KEY_NOT_PRESENT);
/* SPR 20430 Changes Start*/

        LOG_GTPU_MSG(GTPU_SEND_EMI_TO_PEER_REQ_FOR_UNKNOWN_RAB, LOGWARNING, 
                PR_GTPU_CONTROL, relayGtpuCurrentTick_g, __LINE__, 
                EGTPU_GET_ADDRESS (p_param),0,0,0, 0,0, __func__,"");
/* SPR 20430 Changes End*/

        return EGTPU_FAILURE;
    }
    else
    {
        /*GET msg buffer for creating End Marker Ind*/
        EGTPU_GET_MSG_BUF (p_global,U8bit, p_buf, EGTPU_API_MSG_SIZE);
        /* Rel 5.3: Coverity 24412 Fix Start */
        if (EGTPU_NULL == p_buf)
        {
            /* Not able to allocate Memory */
            return EGTPU_FAILURE;
        }
        /* Rel 5.3: Coverity Fix End */

        p_trav = p_buf + EGTPU_API_HDR_LEN;
        
        /* SPR 4161 Fix Start */
        /* st_payld = p_trav - p_buf; *//* offset to start of payload */
        /* SPR 4161 Fix End */
        /* Coverity 73346 Fix Start*/
        if((EGTPU_GET_U16BIT (p_rab_ctxt->p_transport_addr->entity_addr)+2)  > (EGTPU_API_MSG_SIZE - EGTPU_API_HDR_LEN))
        {
            EGTPU_FREE_MSG_BUF (p_buf);
            p_buf = EGTPU_NULL;
            p_trav = EGTPU_NULL;
            return EGTPU_FAILURE;
        }
        /* Coverity 73346 Fix End*/

        /* Set Parameters */
        EGTPU_FILL_PEER_INFO (p_trav,
                EGTPU_UDP_PORT, p_rab_ctxt->p_transport_addr->entity_addr);
        
        /* SPR 4161 Fix Start */
        st_payld = p_trav - p_buf; /* offset to start of payload */
        /* SPR 4161 Fix End */

        addr_len = EGTPU_GET_U16BIT(p_rab_ctxt->p_transport_addr->entity_addr);


        /* Set API Header */
        if(addr_len == EGTPU_SIZE_OF_IPV4_ADDR)
        {
          EGTPU_SET_API_HDR (p_buf, EGTPU_EI_GTPU, EGTPU_EI_UDP,
            EGTPU_UDP_UNITDATA_REQ, (p_trav - p_buf),
            /* SPR 4161 Fix Start */
            (p_trav - p_buf - 1 + GTP_MSG_HDR_MIN_LEN), st_payld, (st_payld - 1));
            /* SPR 4161 Fix End */
        }
        else
        {
          EGTPU_SET_API_HDR (p_buf, EGTPU_EI_GTPU, EGTPU_EI_UDP6,
            EGTPU_UDP_UNITDATA_REQ, (p_trav - p_buf),
            /* SPR 4161 Fix Start */
            (p_trav - p_buf - 1 + GTP_MSG_HDR_MIN_LEN), st_payld, (st_payld - 1));
            /* SPR 4161 Fix End */
        }



       EGTPU_SET_GTP_HDR (p_buf + st_payld, EGTPU_UNSET_EXT_HDR_FLAG,
					EGTPU_UNSET_SEQ_NUM_FLAG, EGTPU_UNSET_PN_FLAG,
            		EGTPU_MSGT_END_MARKER,
                    /* SPR 4161 Fix Start */
            		(p_trav - (p_buf + st_payld )),
                    /* SPR 4161 Fix End */
                    /* SPR 10787 fix start */
            		p_rab_ctxt->teid_cell.teid_peer, EGTPU_INV_SEQ_NUM,
                    /* SPR 10787 fix end */
		     		 EGTPU_SPARE_BYTE, EGTPU_SPARE_BYTE);

        if(addr_len == EGTPU_SIZE_OF_IPV4_ADDR)
        {
            EGTPU_SEND_FROM_GTPU (p_global,p_buf,
                    EGTPU_API_GET_MSG_LEN (p_buf),
                    EGTPU_EI_UDP, ret_val, p_rab_ctxt->src_addr);
        }
        else
        {
            EGTPU_SEND_FROM_GTPU (p_global,p_buf,
                    EGTPU_API_GET_MSG_LEN (p_buf),
                    EGTPU_EI_UDP6, ret_val, p_rab_ctxt->src_addr);
        }
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_END_MARKER_SENT_TO_PEER, LOGINFO, PR_GTPU_DL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                /* SPR 10787 fix start */
                p_rab_ctxt->teid_cell.teid_peer,0,0,0, 0,0, __func__,"");
                /* SPR 10787 fix end */

        /* SPR 2030 fix End*/

        EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_UDP_UNITDATA_REQ);
        EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_END_MARKER_MSG_SENT);
    
        /*Free the allocated local buffer*/
        EGTPU_FREE_MSG_BUF (p_buf);
        /* + coverity 32077 */
        return ret_val;
        /* - coverity 32077 */
    }
}

#ifdef ENDC_ENABLED
/********************************************************************************
 * Function Name  : egtpu_x2u_delivery_status
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This function processes EGTPU_X2U_DELIVERY_STATUS API from relay.
 *                  This function forwards the received PDU to peer.
 ********************************************************************************/
egtpu_return_t egtpu_x2u_delivery_status(
               egtpu_global_t*  p_global,
               egtpu_info_t*    p_egtpu_info)
{
    U8bit*            p_msg      = p_egtpu_info->p_buff;
    U8bit*            p_param    = p_msg + EGTPU_API_HDR_LEN;
    U8bit             addr_len   = 0;
    U16bit            msg_len    = EGTPU_API_GET_PAYLOAD_LEN (p_msg);
    egtpu_return_t    ret_val                       = EGTPU_FAILURE;
    egtpu_rab_ctxt_t* p_rab_ctxt = EGTPU_NULL;
    EGTPU_UPDATE_STATS(p_global, EGTPU_STATS_X2U_DELIVERY_STATUS);

    /* Fetch Self tunnel Id */
    p_egtpu_info->teid_self = EGTPU_GET_U32BIT(p_param + EGTPU_SIZE_OF_TYPE);

    /* Fetch RAB context based on self tunnel Id */
    p_rab_ctxt = egtpu_db_sel_teid_key_rab_ctxt(p_global, p_egtpu_info->teid_self);
    if (!p_rab_ctxt)
    {
        /* RAB context not found */
        EGTPU_ERROR_REPORTER(p_global, (EGTPU_TS_DBERR_KEY_NOT_PRESENT,
                    egtpu_entity_name_arr[EGTPU_EI_GTPU]));

        EGTPU_UPDATE_STATS(p_global, EGTPU_STAT_KEY_NOT_PRESENT);
        LOG_GTPUPR_WARNING(X2U_RELAY,
                "X2U Delivery Status received for unknown TEID [%u]\n",
                EGTPU_GET_U32BIT (p_param + EGTPU_SIZE_OF_TYPE));
        return EGTPU_FAILURE;
    }

    /* Ensure that required flags are set for this tunnel prior
     * to processing this message. */
    if (!(p_rab_ctxt->sap_flag & EGTPU_FROM_RELAY_SAP) ||
            !(p_rab_ctxt->sap_flag & EGTPU_TX_SAP) ||
            !p_rab_ctxt->p_transport_addr)
    {
        EGTPU_ERROR_REPORTER(p_global,(EGTPU_TS_PERR_UNEXPECTED_API,
                    egtpu_entity_name_arr[EGTPU_EI_GTPU],
                    egtpu_in_api_name_arr[EGTPU_EI_RELAY][EGTPU_X2U_DELIVERY_STATUS -
                    egtpu_in_api_base_arr[EGTPU_EI_RELAY]]));

	/* abhijeet */
        /*LOG_GTPUPR_WARNING(X2U_RELAY,"TEID not configured with SAP flag either"
			 "FROM_RELAY_SAP/TX_SAP or PEER Transp Addr not Available\n");*/

        return EGTPU_FAILURE;
    }

    /* Ensure that size of transport address is valid */
    if (EGTPU_GET_U16BIT(p_rab_ctxt->p_transport_addr->entity_addr) > EGTPU_SIZE_OF_IPV6_ADDR)
    {
        return EGTPU_FAILURE;
    }

    /* Since the payload always starts at an offset of 64, fill the info
     * used for forwarding the packet to peer in the spare bytes between
     * the API header and the payload. */
    {
        /* Set the peer port number and ip address */
        EGTPU_FILL_PEER_INFO (p_param,
                EGTPU_UDP_PORT, p_rab_ctxt->p_transport_addr->entity_addr);

        /* Set the QoS Id */
        EGTPU_SET_U32BIT (p_param, p_rab_ctxt->qos_id);
        p_param += EGTPU_SIZE_OF_QOS_ID;
    }

    /* Update parameters in API header */
    /* Set source module ID */
    EGTPU_API_SET_SRC_ID (p_msg, EGTPU_EI_GTPU);

    addr_len = EGTPU_GET_U16BIT(p_rab_ctxt->p_transport_addr->entity_addr);

    /* Set destination module ID based on IP address length */
    if (addr_len == EGTPU_SIZE_OF_IPV4_ADDR)
    {
        EGTPU_API_SET_DST_ID (p_msg, EGTPU_EI_UDP);
    }
    else
    {
        EGTPU_API_SET_DST_ID (p_msg, EGTPU_EI_UDP6);
    }

    /* Set API ID */
    EGTPU_API_SET_API_ID (p_msg, EGTPU_UDP_UNITDATA_REQ);

    /* Set START payload to start payload minus GTPU header including
     * RAN container extension header containing DL data delivery status */
    EGTPU_API_SET_START_PAYLOAD (p_msg,
            (EGTPU_API_GET_START_PAYLOAD(p_msg) - (EGTPU_MIN_HDR_LEN + 4)));

    /* Update the message length to include GTPU header */
    msg_len += (EGTPU_MIN_HDR_LEN + 4);

    /* Set end param list */
    EGTPU_API_SET_END_PARAM_LIST (p_msg, (p_param - p_msg - 1));

    /* Fill the GTP Header */
    EGTPU_SET_GTP_HDR_WITH_NRU_RAN_CONTAINER(EGTPU_API_GET_PAYLOAD_STRT_PTR(p_msg),
            EGTPU_MSGT_TPDU, msg_len,
            p_rab_ctxt->teid_cell.teid_peer);

    /* Forward the message back to Relay module. */
    if (addr_len == EGTPU_SIZE_OF_IPV4_ADDR)
    {
        EGTPU_SEND_FROM_GTPU (p_global, p_msg,
                EGTPU_API_GET_MSG_LEN (p_msg),
                EGTPU_EI_UDP, ret_val,
                p_rab_ctxt->src_addr);
    }
    else
    {
        EGTPU_SEND_FROM_GTPU (p_global, p_msg,
                EGTPU_API_GET_MSG_LEN (p_msg),
                EGTPU_EI_UDP6, ret_val,
                p_rab_ctxt->src_addr);
    }


    LOG_GTPUPR_INFO(X2U_RELAY,
            "DL Data Delivery Status Sent to Peer\n"
            );
			
    EGTPU_UPDATE_STATS (p_global, EGTPU_STATS_API_UDP_UNITDATA_REQ);
    EGTPU_UPDATE_STATS (p_global, EGTPU_STAT_PDU_SENT);

    return ret_val;
}
#endif
      
