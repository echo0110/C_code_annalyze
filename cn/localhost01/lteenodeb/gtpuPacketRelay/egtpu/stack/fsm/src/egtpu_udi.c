/***********************************************************************
************************************************************************
** FILE NAME
**     egtpu_udi.c
**
************************************************************************
************************************************************************
** DESCRIPTION
**     This file handles incoming G-PDUs from the peer
**     Handles: T-PDU, supp-ext-hdr, err-ind, echo-req, echo-rsp
**
**  DATE      NAME        			REFERENCE    REASON
**  ---       ----        			---------    ------
**  Oct,2009 Vipul Aggarwal						Initial
**  14Jan,10 Vipul Aggarwal						SPR FIX 7446
**
** Copyright (C) 2009 Aricent Inc. All Rights Reserved.
************************************************************************
***********************************************************************/

#include <ylib.h>
#include <egtpu_glob.h>
#include <egtpu_apihdr.h>

#include <egtpu_msgtypes.h>
#include <egtpu_ietypes.h>
#include <egtpu_error.h>
#include <egtpu_stat.h>
#include <egtpu_glob_struct.h>
#include <egtpu_cdb.h>
#include <egtpu_os_proto.h>
#include <egtpu_udi.h>
#include <egtpu_trace.h>
#include <egtpu_util.h>

#include <egtpu_pmm.h>

/***********************************************************************************
 * Function Name  : __send_err_ind__ 
 * Inputs         : p_global,p_egtpu_info 
 * Outputs        : None
 * Returns        : EGTPU_FAILURE/EGTPU_SUCCESS 
 * Description    : Prepares and sends Error Indication message to peer
 *                  Always sends seq-no even if it wasn't received in corresponding
 *                  tpdu... Doesn't matters as the peer could do no harm with this
 *                  As a matter of fact he is not supposed to interpret it anyway
 **********************************************************************************/
static egtpu_return_t __send_err_ind__ (egtpu_global_t *p_global,
										egtpu_info_t *p_egtpu_info)
{
    U8bit         *p_buf = EGTPU_NULL, addr_len=0;
    U8bit         *p_trav = EGTPU_NULL;
    U16bit        st_payld;
    egtpu_return_t ret_val;

    /* SPR 18822 Fix Start */
    /* Get the peer context to know UDP Extensnion Header supported or not*/
    egtpu_peer_ctxt_t *p_ctxt =
        egtpu_db_sel_addr_key_peer_ctxt
        (p_egtpu_info->p_peer_addr);
    /* SPR 18822 Fix End */

    EGTPU_TRACE_FN_ENTR (p_global,"__send_err_ind__");

    EGTPU_GET_MSG_BUF (p_global,U8bit, p_buf, EGTPU_API_MSG_SIZE);

    /* Rel 5.3: Coverity 24416 Fix Start */
    if (EGTPU_NULL == p_buf)
    {
        /* Not able to allocate Memory */
        EGTPU_TRACE_FN_EXIT (p_global,"__send_err_ind__");
        return EGTPU_FAILURE;
    }
    /* Rel 5.3: Coverity Fix End */
    p_trav = p_buf + EGTPU_API_HDR_LEN;
    /* coverity 72839 fix start */
    addr_len = EGTPU_GET_U16BIT(p_egtpu_info->p_peer_addr);
    /* SPR 18822 Fix Start */
    if((addr_len != EGTPU_SIZE_OF_IPV6_ADDR) && (addr_len != EGTPU_SIZE_OF_IPV4_ADDR)) 
    /* SPR 18822 Fix End */
    {
        /*Coverity 83246 Fix Start*/
        EGTPU_FREE_MSG_BUF (p_buf);
        /*Coverity 83246 Fix End*/
        return EGTPU_FAILURE;
    }
    /* coverity 72839 fix end */

    EGTPU_FILL_PEER_INFO (p_trav,
            EGTPU_UDP_PORT, p_egtpu_info->p_peer_addr);

	
    /* SPR 18822 Fix Start */
    st_payld = p_trav - p_buf; /* offset to start of payload */

    /*Target address may not be present in database if this is 
     * forwarding tunnel (which is unidirectional).If present we will check
     * wheather peer is supporting UDP Extension Header or not */
    if((p_ctxt && (p_ctxt->ext_hdr & EGTPU_EXT_HDR_UDP))||(p_ctxt == EGTPU_NULL))
    {
        p_trav += EGTPU_HDR_LEN_WITH_UDP_PORT_NUM;  /* fill IEs first; put hdr later */
    }
    else
    {
    p_trav += EGTPU_HDR_LEN_WITH_SEQ_NUM; /* fill IEs first; put hdr later */
    }
    /* SPR 18822 Fix End */

    *p_trav++ = EGTPU_IE_TEID_DATA_I;
    EGTPU_SET_U32BIT (p_trav, p_egtpu_info->teid_self);
    p_trav += EGTPU_SIZE_OF_TEID;

    *p_trav++ = EGTPU_IE_GSN_ADDR;
	
	if (addr_len == EGTPU_SIZE_OF_IPV4_ADDR)											 
	{
     EGTPU_SET_U16BIT (p_trav,  EGTPU_SIZE_OF_IPV4_ADDR);
     p_trav +=  EGTPU_SIZE_OF_LENGTH;
     /** SPR FIX 7446 Start **/
     egtpu_memcpy (p_trav, p_global->initdb_info.my_addr[0],
           EGTPU_SIZE_OF_IPV4_ADDR);
     p_trav += EGTPU_SIZE_OF_IPV4_ADDR;
     /** SPR FIX 7446 Stop **/
	}
	else
	{
	EGTPU_SET_U16BIT (p_trav,  EGTPU_SIZE_OF_IPV6_ADDR);
     p_trav +=  EGTPU_SIZE_OF_LENGTH;
     egtpu_memcpy (p_trav, p_global->initdb_info.my_addr6[0],
           EGTPU_SIZE_OF_IPV6_ADDR);
     p_trav += EGTPU_SIZE_OF_IPV6_ADDR;
	}
	/* SPR 3945 Fix Start */	
    /* SPR 18822 Fix Start */
    /*Target address may not be present in database if this is 
     * forwarding tunnel (which is unidirectional).If present we will check
     * wheather peer is supporting UDP Extension Header or not */
    if((p_ctxt && (p_ctxt->ext_hdr & EGTPU_EXT_HDR_UDP))||(p_ctxt == EGTPU_NULL))
    {
        /* GTP Hdr will be filled with UDP port details if Peer is supported 
         * UDP Port extension as per spec 29.281 */
        EGTPU_SET_GTP_HDR (p_buf + st_payld, EGTPU_UDP_EXT_HDR,
                EGTPU_SET_SEQ_NUM_FLAG, EGTPU_UNSET_PN_FLAG,
                EGTPU_MSGT_ERROR_INDICATION,
                (p_trav - (p_buf + st_payld + GTP_MSG_HDR_MIN_LEN)),
                EGTPU_INV_TEID_VALUE, EGTPU_MIN_SEQ_NUM,
                EGTPU_SPARE_BYTE,p_egtpu_info->port_num);
    }
    else
    {
        /* GTP Hdr will be filled without UDP port details if Peer is not supported */
        EGTPU_SET_GTP_HDR (p_buf + st_payld,EGTPU_UNSET_EXT_HDR_FLAG,
					EGTPU_SET_SEQ_NUM_FLAG, EGTPU_UNSET_PN_FLAG,
            		EGTPU_MSGT_ERROR_INDICATION,
            		(p_trav - (p_buf + st_payld + GTP_MSG_HDR_MIN_LEN)),
            		EGTPU_INV_TEID_VALUE, EGTPU_MIN_SEQ_NUM,
                EGTPU_SPARE_BYTE,EGTPU_SPARE_BYTE);
    }
    /* SPR 18822 Fix Start */
	/* SPR 3945 Fix End */	

    if (addr_len == EGTPU_SIZE_OF_IPV4_ADDR)
	{
        EGTPU_SET_API_HDR (p_buf, EGTPU_EI_GTPU, EGTPU_EI_UDP,
            EGTPU_UDP_UNITDATA_REQ, (p_trav - p_buf),
            (p_trav - p_buf - 1), st_payld, (st_payld - 1));
	

		EGTPU_SEND_FROM_GTPU (p_global,p_buf, (p_trav - p_buf), EGTPU_EI_UDP,\
							ret_val, p_global->initdb_info.my_addr[0]);
	}
	else
	{
		 EGTPU_SET_API_HDR (p_buf, EGTPU_EI_GTPU, EGTPU_EI_UDP6,
				EGTPU_UDP_UNITDATA_REQ, (p_trav - p_buf),
				(p_trav - p_buf - 1), st_payld, (st_payld - 1));
	 
		EGTPU_SEND_FROM_GTPU (p_global,p_buf, (p_trav - p_buf), EGTPU_EI_UDP6,\
							ret_val, p_global->initdb_info.my_addr6[0]);
    }
    /*SPR #2700 fix Start*/
    EGTPU_FREE_MSG_BUF (p_buf);
    /*SPR #2700 fix End*/

    /* SPR 2030 fix Start*/
    LOG_GTPU_MSG(GTPU_SENT_ERR_IND_TO_PEER, LOGINFO, PR_GTPU_DL_DATA,
             relayGtpuCurrentTick_g, __LINE__, 
             0,0,0,0, 0,0, __func__,"");
     /* SPR 2030 fix End*/

    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_UDP_UNITDATA_REQ);
    EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_ERROR_IND_MSG_SENT);

    EGTPU_TRACE_FN_EXIT (p_global,"__send_err_ind__");
    return ret_val;
}
  
/***********************************************************************************
 * Function Name  : egtpu_send_gpdu_relay 
 * Inputs         : p_global,p_egtpu_info 
 * Outputs        : None
 * Returns        : None 
 * Description    : This function send gppu to relay entity
 **********************************************************************************/
egtpu_void_t egtpu_send_gpdu_relay(egtpu_global_t *p_global,
									egtpu_info_t *p_egtpu_info)
{
    egtpu_return_t   ret_val = EGTPU_FAILURE;
    U8bit           *p_param = p_egtpu_info->p_buff + EGTPU_API_HDR_LEN;

	/* Set Parameters */
    *p_param++ = EGTPU_IE_TRANSACTION_ID;
    EGTPU_SET_U32BIT (p_param, p_egtpu_info->transaction_id);
    p_param += EGTPU_SIZE_OF_TRANSACTION_ID;
        
    if (p_egtpu_info->ie_presence & EGTPU_IE_SEQ_NUM_FLAG)
	{
           *p_param++ = EGTPU_IE_SEQ_NUM;
           EGTPU_SET_U16BIT (p_param, p_egtpu_info->seq_num_dnlk);
           p_param += EGTPU_SIZE_OF_SEQ_NUM;
    }

    if (p_egtpu_info->ie_presence & EGTPU_IE_NPDU_NUM_FLAG)
	{
           *p_param++ = EGTPU_IE_NPDU_NUM;
           *p_param++ = p_egtpu_info->n_pdu_num;
        }

    if (p_egtpu_info->ie_presence & EGTPU_IE_PDCP_NUM_FLAG)
	{
           *p_param++ = EGTPU_IE_PDCP_NUM;
                
           EGTPU_SET_U16BIT (p_param, p_egtpu_info->pdcp_num);
           p_param += EGTPU_SIZE_OF_PDCP_NUM;
        }

    /* Set API Header */
    EGTPU_API_SET_SRC_ID (p_egtpu_info->p_buff, EGTPU_EI_GTPU);
    EGTPU_API_SET_DST_ID (p_egtpu_info->p_buff, EGTPU_EI_RELAY);
    EGTPU_API_SET_API_ID (p_egtpu_info->p_buff, EGTPU_RELAY_DATA_IND);

    EGTPU_API_SET_END_PARAM_LIST (p_egtpu_info->p_buff,
            (p_param - p_egtpu_info->p_buff - 1));

    EGTPU_API_SET_START_PAYLOAD (p_egtpu_info->p_buff,
            (EGTPU_API_GET_START_PAYLOAD (p_egtpu_info->p_buff) +
                    p_egtpu_info->hdr_len));

    EGTPU_SEND_FROM_GTPU (p_global,p_egtpu_info->p_buff,
             EGTPU_API_GET_MSG_LEN (p_egtpu_info->p_buff),
             EGTPU_EI_RELAY, ret_val, NULL);

    /* + coverity 32080 */
    if (EGTPU_FAILURE == ret_val)
    {
        LOG_GTPU_MSG(GTPU_SEND_FAILURE, LOGWARNING, PR_GTPU_DL_DATA,
                relayGtpuCurrentTick_g, __LINE__,
                0,0,0,0, 0,0, __func__, "");
    }
    /* + coverity 32080 */
 
    /* SPR 2030 fix Start*/
    LOG_UT(GTPU_GPDU_SENT_TO_RELAY, LOGDEBUG, PR_GTPU_DL_DATA,
             relayGtpuCurrentTick_g, __LINE__, 
             0,0,0,0, 0,0, __func__,"");
     /* SPR 2030 fix End*/

    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_RELAY_DATA_IND);
}

/***********************************************************************************
 * Function Name  : egtpu_send_hash_node_relay 
 * Inputs         : p_global,p_egtpu_info_reorder
 * Outputs        : None
 * Returns        : None
 * Description    : This function send gppu to relay entity
 **********************************************************************************/
egtpu_void_t egtpu_send_hash_node_relay(egtpu_global_t *p_global,
									egtpu_info_t *p_egtpu_info_reorder)
{
    U8bit *p_tmp = p_egtpu_info_reorder->p_buff + EGTPU_API_HDR_LEN;
	U8bit ext_hdr;
	U8bit *p_start = 
			EGTPU_API_GET_PAYLOAD_STRT_PTR(p_egtpu_info_reorder->p_buff);
	/*goes to start of payload*/ 
	egtpu_return_t ret_val;

	if (*p_start & 0x07) 
	 p_egtpu_info_reorder->hdr_len = EGTPU_HDR_LEN_WITH_SEQ_NUM;
	else
	  p_egtpu_info_reorder->hdr_len = EGTPU_MIN_HDR_LEN;   

	/*Inserting the IE's of EGTPU_RELAY_DATA_IND*/
    *p_tmp++ = EGTPU_IE_TRANSACTION_ID;

    EGTPU_SET_U32BIT (p_tmp, p_egtpu_info_reorder->transaction_id);
    p_tmp += EGTPU_SIZE_OF_TRANSACTION_ID;

    if (EGTPU_GET_SEQ_FLAG (p_start))
	{
        *p_tmp++ = EGTPU_IE_SEQ_NUM;
        p_egtpu_info_reorder->seq_num_dnlk = EGTPU_GET_SEQ_NUM (p_start);
        p_egtpu_info_reorder->ie_presence |= EGTPU_IE_SEQ_NUM_FLAG;
        EGTPU_SET_U16BIT (p_tmp, p_egtpu_info_reorder->seq_num_dnlk);
        p_tmp += EGTPU_SIZE_OF_SEQ_NUM;
    }

    if (EGTPU_GET_PN_FLAG (p_start))
	{
        *p_tmp++ = EGTPU_IE_NPDU_NUM;
        p_egtpu_info_reorder->n_pdu_num = EGTPU_GET_NPDU_NUM (p_start);
        p_egtpu_info_reorder->ie_presence |= EGTPU_IE_NPDU_NUM_FLAG;
        *p_tmp = p_egtpu_info_reorder->n_pdu_num;
        p_tmp += EGTPU_SIZE_OF_NPDU_NUM;
    }
    
    ext_hdr=*(p_start + EGTPU_HDR_LEN_WITH_SEQ_NUM - 1); 
    if (ext_hdr == EGTPU_PDCP_PDU_EXT_HDR)
	{
         p_egtpu_info_reorder->ie_presence |= EGTPU_IE_PDCP_NUM_FLAG;
         p_egtpu_info_reorder->pdcp_num =
		 			EGTPU_GET_U16BIT(p_start + EGTPU_HDR_LEN_WITH_SEQ_NUM);               
         *p_tmp++ = EGTPU_IE_PDCP_NUM;
         *p_tmp = p_egtpu_info_reorder->pdcp_num;
         p_tmp += EGTPU_SIZE_OF_PDCP_NUM;
    }
    /* Set API Header */
    EGTPU_API_SET_SRC_ID (p_egtpu_info_reorder->p_buff, EGTPU_EI_GTPU);
    EGTPU_API_SET_DST_ID (p_egtpu_info_reorder->p_buff, EGTPU_EI_RELAY);
    EGTPU_API_SET_API_ID (p_egtpu_info_reorder->p_buff, EGTPU_RELAY_DATA_IND);

    EGTPU_API_SET_END_PARAM_LIST (p_egtpu_info_reorder->p_buff,
                (p_tmp - p_egtpu_info_reorder->p_buff - 1));

    EGTPU_API_SET_START_PAYLOAD (p_egtpu_info_reorder->p_buff,
                (EGTPU_API_GET_START_PAYLOAD (p_egtpu_info_reorder->p_buff) +
                 p_egtpu_info_reorder->hdr_len));

    EGTPU_SEND_FROM_GTPU (p_global,p_egtpu_info_reorder->p_buff,
                EGTPU_API_GET_MSG_LEN (p_egtpu_info_reorder->p_buff),
                EGTPU_EI_RELAY, ret_val, NULL);

     /* + coverity 32079 */
     if (EGTPU_FAILURE == ret_val)
     {
         LOG_GTPU_MSG(GTPU_SEND_FAILURE, LOGWARNING, PR_GTPU_DL_DATA,
                 relayGtpuCurrentTick_g, __LINE__,
                 0,0,0,0, 0,0, __func__, "");
     }
     /* + coverity 32079 */

    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_RELAY_DATA_IND);
}

/***********************************************************************************
 * Function Name  : __tpdu_msg__ 
 * Inputs         : p_global,p_egtpu_info
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE 
 * Description    : This function processes the incoming T-PDU
 *                  Checked if RAB exists for the incoming PDU
 *                  PDU is then fwded either to relay or transferred to target eNodeB
 **********************************************************************************/
static egtpu_return_t __tpdu_msg__ (egtpu_global_t *p_global,
									egtpu_info_t *p_egtpu_info)
{
    egtpu_rab_ctxt_t *p_rab_ctxt = EGTPU_NULL;
    U8bit           *p_param = p_egtpu_info->p_buff + EGTPU_API_HDR_LEN,
                    addr_len = 0;
    egtpu_return_t   ret_val = EGTPU_FAILURE;
	
    p_rab_ctxt = 
			egtpu_db_sel_teid_key_rab_ctxt(p_global,p_egtpu_info->teid_self);

    if (!p_rab_ctxt)
	{
        EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_DBERR_KEY_NOT_PRESENT,
                    egtpu_entity_name_arr[EGTPU_EI_GTPU]));

        EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_KEY_NOT_PRESENT);

        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_DATA_RCVD_FOR_UNKNOWN_RAB, LOGWARNING , PR_GTPU_DL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                0,0,0,0, 0,0, __func__,"");

        /* SPR 2030 fix End*/
        __send_err_ind__ (p_global,p_egtpu_info);

        /* On receiving dl traffic data in pdcp adapter, a static memory is 
        allocated for buffer, so can't free it from memory pool*/

        /*EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);*/
        return EGTPU_SUCCESS;
    }
     
   /*if end_marker_flag is true then silently discard all downlink packets on that
	 tunnel */
	
   if (	p_rab_ctxt->end_marker_flag == EGTPU_TRUE)
	{
        EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_PDU_DISCARDED); 
        /* SPR 2007 Fix - Part2 Start*/
        /* EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);*/
        /* SPR 2007 Fix - Part2 End*/
        LOG_GTPU_MSG(GTPU_DL_DATA_DISCARDED_AFTER_EMI, LOGINFO , PR_GTPU_DL_DATA,
                relayGtpuCurrentTick_g, __LINE__,
                0,0,0,0, 0,0, __func__,"");

         return EGTPU_SUCCESS;
    }

    if (!(p_rab_ctxt->sap_flag & EGTPU_RX_SAP))
	{
        EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_UNEXPECTED_MSG,
                    egtpu_entity_name_arr[EGTPU_EI_GTPU],
                    egtpu_get_msg_name (p_egtpu_info->pdu_type)));

        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_NO_RX_TUNNEL, LOGWARNING, PR_GTPU_DL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                0,0,0,0, 0,0, __func__,"");
        /* SPR 2030 fix End*/

        /* SPR 2007 Fix - Part2 Start*/
        /*EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);*/
        /* SPR 2007 Fix - Part2 End*/
        return EGTPU_FAILURE;
    }

    if (p_rab_ctxt->sap_flag & EGTPU_TO_RELAY_SAP)
	{
        /*reordering starts*/
        if(p_rab_ctxt->reordering_reqd)
		{
			egtpu_reordering(p_rab_ctxt, p_global, p_egtpu_info);
        }
        else
		{ 
		    p_egtpu_info->transaction_id = p_rab_ctxt->transaction_id;
	  		egtpu_send_gpdu_relay(p_global ,p_egtpu_info);
            p_rab_ctxt->seq_num_dnlk = p_egtpu_info->seq_num_dnlk;
		}
	}
    else if ((p_rab_ctxt->sap_flag & EGTPU_FWD_SAP) &&
               p_rab_ctxt->p_forward_addr)
	{
        /* Coverity 73724 Fix Start*/
        if( EGTPU_GET_U16BIT(p_rab_ctxt->p_forward_addr->entity_addr) > EGTPU_SIZE_OF_IPV6_ADDR )
        {
            return EGTPU_FAILURE;
        }
        /* Coverity 73724 Fix End*/

        /*Coverity 86865 Fix Start*/
        /* Set Parameters */
        EGTPU_FILL_PEER_INFO (p_param,
                EGTPU_UDP_PORT, p_rab_ctxt->p_forward_addr->entity_addr);

        addr_len = EGTPU_GET_U16BIT(p_rab_ctxt->p_forward_addr->entity_addr);

        /* Set API Header */
        EGTPU_API_SET_SRC_ID (p_egtpu_info->p_buff, EGTPU_EI_GTPU);

        if(addr_len == EGTPU_SIZE_OF_IPV4_ADDR)
        {
            EGTPU_API_SET_DST_ID (p_egtpu_info->p_buff, EGTPU_EI_UDP);
        }
        else
        {
            EGTPU_API_SET_DST_ID (p_egtpu_info->p_buff, EGTPU_EI_UDP6);
        }

        EGTPU_API_SET_API_ID (p_egtpu_info->p_buff,
                EGTPU_UDP_UNITDATA_REQ);

        EGTPU_API_SET_END_PARAM_LIST (p_egtpu_info->p_buff,
                (p_param - p_egtpu_info->p_buff - 1));

        /* only teid peer needs to be changed in gtp-hdr */
        /* teid is after 4 bytes in the header */
        EGTPU_SET_U32BIT
            ((EGTPU_API_GET_PAYLOAD_STRT_PTR(p_egtpu_info->p_buff) + 4),
             p_rab_ctxt->teid_peer_fwd);

        if(addr_len == EGTPU_SIZE_OF_IPV4_ADDR)
        {

            EGTPU_SEND_FROM_GTPU (p_global,p_egtpu_info->p_buff,
                    EGTPU_API_GET_MSG_LEN (p_egtpu_info->p_buff),
                    EGTPU_EI_UDP, ret_val, p_rab_ctxt->src_addr);
        }
        else
        {
            EGTPU_SEND_FROM_GTPU (p_global,p_egtpu_info->p_buff,
                    EGTPU_API_GET_MSG_LEN (p_egtpu_info->p_buff),
                    EGTPU_EI_UDP6, ret_val, p_rab_ctxt->src_addr);
        }

        EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_UDP_UNITDATA_REQ);
        EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_PDU_SENT);
    }   
    else
    {
        EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_UNEXPECTED_MSG,
                    egtpu_entity_name_arr[EGTPU_EI_GTPU],
                    egtpu_get_msg_name (p_egtpu_info->pdu_type)));
        /* SPR 2007 Fix - Part2 Start*/
        /* EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);*/
        /* SPR 2007 Fix - Part2 End*/
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_NO_TORELAY_NO_FWD_SAP_SET, LOGWARNING, PR_GTPU_DL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                0,0,0,0, 0,0, __func__,"");
        /* SPR 2030 fix End*/

        /* SPR 2007 Fix - Part2 Start*/
        /*EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);*/
        /* SPR 2007 Fix - Part2 End*/
        return EGTPU_FAILURE;
    }
    /*Coverity 86865 Fix End*/
    return ret_val;
}


/***********************************************************************************
 * Function Name  : __error_ind_msg__ 
 * Inputs         : p_global
 *                  p_egtpu_info->teid_peer    peer transport association
 *                  p_egtpu_info->p_peer_addr  peer transport address
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE 
 * Description    : This function  Processes the error indication message from peer
 *                  Searches in database for RAB ctxt corresponding to peer info
 *                  If found, error indication noti is sent to CP
 *                  CP shall release the RAB
 **********************************************************************************/
static egtpu_return_t __error_ind_msg__ (egtpu_global_t *p_global,
										egtpu_info_t *p_egtpu_info)
{
    /*SPR_20405_START*/
    YTNODE            *p_node = EGTPU_NULL;
    /*SPR_20405_END*/
    U8bit            *p_param = p_egtpu_info->p_buff + EGTPU_API_HDR_LEN;
    egtpu_rab_ctxt_t  *p_rab_ctxt = EGTPU_NULL;
    egtpu_peer_ctxt_t *p_peer_ctxt = EGTPU_NULL;
    /* + coverity 108948 */
    egtpu_return_t    ret_val = EGTPU_SUCCESS;
    /* - coverity 108948 */
	U16bit			  pvt_extn_len;
    egtpu_peer_teid_cell_ctxt_t   teid_cell;

    teid_cell.cellIndex = p_egtpu_info->cellIndex;
    teid_cell.teid_peer = p_egtpu_info->teid_peer;
    /*SPR_20405_START*/

    if ((p_peer_ctxt = egtpu_db_sel_addr_key_peer_ctxt
			    (p_egtpu_info->p_peer_addr)) &&
		    (p_rab_ctxt = egtpu_db_sel_teid_peer_key_peer_map
		     /* SPR 10787 fix start */
		     (p_peer_ctxt, teid_cell)))
	    /* SPR 10787 fix end */
    {
        while((p_node = egtpu_db_check_teid_peer_key_peer_map
                    (p_peer_ctxt, p_egtpu_info->teid_peer, p_node)) &&
                (p_rab_ctxt = &((EGTPU_GET_STR_FROM_ELMT (struct __rab_ctxt_rec__,p_node,__node__))->ctxt)))
        {
            /*SPR_20405_END*/
            *p_param++ = EGTPU_IE_TRANSACTION_ID;
            EGTPU_SET_U32BIT (p_param, p_rab_ctxt->transaction_id);
            p_param += EGTPU_SIZE_OF_TRANSACTION_ID;

            if (p_egtpu_info->ie_presence & EGTPU_IE_PVT_EXTN_FLAG)
            {
                *p_param++ = EGTPU_IE_PRIVATE_EXTENSION;
                pvt_extn_len = EGTPU_GET_U16BIT(p_egtpu_info->p_pvt_extn);
                egtpu_memcpy(p_param, p_egtpu_info->p_pvt_extn,
                        EGTPU_SIZE_OF_LENGTH + pvt_extn_len);
                p_param += EGTPU_SIZE_OF_LENGTH + pvt_extn_len;
            }
            EGTPU_SET_API_HDR (p_egtpu_info->p_buff, EGTPU_EI_GTPU, EGTPU_EI_CP,
                    EGTPU_CP_ERROR_IND, (p_param - p_egtpu_info->p_buff),
                    EGTPU_NO_PAYLOAD, EGTPU_NO_PAYLOAD, (p_param - p_egtpu_info->p_buff - 1));
            /*SPR_20405_START*/
            LOG_GTPUPR_INFO(PR_GTPU_DL_DATA,"ErrorIndication being sent for Peer TEID [%u] and transactionId:[%u] ",
                    p_egtpu_info->teid_peer,p_rab_ctxt->transaction_id);

            EGTPU_SEND_FROM_GTPU (p_global,p_egtpu_info->p_buff,
                    (p_param - p_egtpu_info->p_buff), EGTPU_EI_CP, ret_val,NULL);

            /* SPR 2030 fix Start*/
            LOG_GTPU_MSG(GTPU_ERR_IND_SENT_TO_CP, LOGINFO, PR_GTPU_DL_DATA,
                    relayGtpuCurrentTick_g, __LINE__, 
                    0,0,0,0, 0,0, __func__,"");
            /* SPR 2030 fix End*/
            EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_CP_ERROR_IND);
            p_param = p_egtpu_info->p_buff + EGTPU_API_HDR_LEN;
        }
        /*SPR_20405_END*/
    }
	else
	{
        EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_UNEXPECTED_MSG,
                    egtpu_entity_name_arr[EGTPU_EI_GTPU],
                    egtpu_get_msg_name (p_egtpu_info->pdu_type)));

        EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_KEY_NOT_PRESENT);
		EGTPU_UPDATE_STATS (p_global,\
						EGTPU_STAT_ERR_UNEXPECTED_GTP_SIGNALLING_MESSAGE);	
                        
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_ERR_IND_RECVD_FROM_UNKNOWN_PEER, LOGWARNING, PR_GTPU_DL_DATA,
               relayGtpuCurrentTick_g, __LINE__, 
               0,0,0,0, 0,0, __func__,"");
     /* SPR 2030 fix End*/

        /* SPR 2007 Fix - Part2 Start*/
        /* EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);*/
        /* SPR 2007 Fix - Part2 End*/
        return EGTPU_FAILURE;
    }

    return ret_val;
}

/***********************************************************************************
 * Function Name  : __end_marker_msg__ 
 * Inputs         : p_global
 *                  p_egtpu_info
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE 
 * Description    : This function Processes the incoming End Marker Message
 *                  Checked if RAB exists for the incoming End Marker Message
 *                  End Marker Indication is then fwded to control plane and 
 *                  the msg is transferred to target eNodeB
 **********************************************************************************/
static egtpu_return_t __end_marker_msg__ (egtpu_global_t *p_global,
											egtpu_info_t *p_egtpu_info)
{
    egtpu_rab_ctxt_t *p_rab_ctxt = EGTPU_NULL;
    U8bit           *p_param = p_egtpu_info->p_buff + EGTPU_API_HDR_LEN,
    addr_len = 0;
    egtpu_return_t   ret_val = EGTPU_FAILURE;
    U16bit pvt_extn_len=0, msg_len;
    U8bit *p_buf;

    p_rab_ctxt = 
        egtpu_db_sel_teid_key_rab_ctxt(p_global,p_egtpu_info->teid_self);

    if (!p_rab_ctxt)
    {
        EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_DBERR_KEY_NOT_PRESENT,
                    egtpu_entity_name_arr[EGTPU_EI_GTPU]));

        EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_KEY_NOT_PRESENT);
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_END_MARKER_RCVD_FOR_UNKNOWN_RAB, LOGWARNING, PR_GTPU_DL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                0,0,0,0, 0,0, __func__,"");

        /* SPR 2030 fix End*/

        __send_err_ind__ (p_global,p_egtpu_info);
        /* SPR 2007 Fix - Part2 Start*/
        /* EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);*/
        /* SPR 2007 Fix - Part2 End*/
        return EGTPU_SUCCESS;
    }

    if (!(p_rab_ctxt->sap_flag & EGTPU_RX_SAP))
    {
        EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_UNEXPECTED_MSG,
                    egtpu_entity_name_arr[EGTPU_EI_GTPU],
                    egtpu_get_msg_name (p_egtpu_info->pdu_type)));
        
        /* SPR 17747 FIX START */
        LOG_MSG(GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON,
                relayGtpuCurrentTick_g, __LINE__, EGTPU_PERR_UNEXPECTED_MSG,
                p_egtpu_info->pdu_type,0,0,0,0, __func__,
                egtpu_entity_name_arr[EGTPU_EI_GTPU] );
        /* SPR 17747 FIX END */
        
        /* SPR 2007 Fix - Part2 Start*/
        /* EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);*/
        /* SPR 2007 Fix - Part2 End*/
        return EGTPU_FAILURE;
    }
    p_rab_ctxt->end_marker_flag = EGTPU_TRUE;

    if ((p_rab_ctxt->sap_flag & EGTPU_FWD_SAP) &&
            p_rab_ctxt->p_forward_addr)
    {
        /* This condition shall not hit on source eNB as EMI is received 
         * over normal tunnel */
        if(EGTPU_GET_U16BIT (p_rab_ctxt->p_forward_addr->entity_addr)  > EGTPU_SIZE_OF_IPV6_ADDR)
        {
            return EGTPU_FAILURE;
        }

        /* Set Parameters */
        EGTPU_FILL_PEER_INFO (p_param,
                EGTPU_UDP_PORT, p_rab_ctxt->p_forward_addr->entity_addr);


        addr_len = EGTPU_GET_U16BIT(p_rab_ctxt->p_forward_addr->entity_addr);


        /* Set API Header */
        EGTPU_API_SET_SRC_ID (p_egtpu_info->p_buff, EGTPU_EI_GTPU);

        if(addr_len == EGTPU_SIZE_OF_IPV4_ADDR)
        {
            EGTPU_API_SET_DST_ID (p_egtpu_info->p_buff, EGTPU_EI_UDP);
        }
        else
        {
            EGTPU_API_SET_DST_ID (p_egtpu_info->p_buff, EGTPU_EI_UDP6);
        }


        EGTPU_API_SET_API_ID (p_egtpu_info->p_buff,EGTPU_UDP_UNITDATA_REQ);


        EGTPU_API_SET_END_PARAM_LIST (p_egtpu_info->p_buff,
                (p_param - p_egtpu_info->p_buff - 1));

        /* only teid peer needs to be changed in gtp-hdr */
        /* teid is after 4 bytes in the header */
        EGTPU_SET_U32BIT
            ((EGTPU_API_GET_PAYLOAD_STRT_PTR (p_egtpu_info->p_buff) + 4),
             p_rab_ctxt->teid_peer_fwd);

        if(addr_len == EGTPU_SIZE_OF_IPV4_ADDR)
        {
            EGTPU_SEND_FROM_GTPU (p_global,p_egtpu_info->p_buff,
                    EGTPU_API_GET_MSG_LEN (p_egtpu_info->p_buff),
                    EGTPU_EI_UDP, ret_val, p_rab_ctxt->src_addr);
        }
        else
        {
            EGTPU_SEND_FROM_GTPU (p_global,p_egtpu_info->p_buff,
                    EGTPU_API_GET_MSG_LEN (p_egtpu_info->p_buff),
                    EGTPU_EI_UDP6, ret_val, p_rab_ctxt->src_addr);
        }
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_END_MARKER_SENT_TO_PEER, LOGINFO, PR_GTPU_DL_DATA,
                /* + SPR 16766 */
                relayGtpuCurrentTick_g, __LINE__, p_egtpu_info->teid_self, 
                /* - SPR 16766 */
                0,0,0, 0,0, __func__,"");
        /* SPR 2030 fix End*/


        EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_UDP_UNITDATA_REQ);
        EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_END_MARKER_MSG_SENT);

        /** Send the End Marker Indication to the CP entity **/
        if (p_egtpu_info->ie_presence & EGTPU_IE_PVT_EXTN_FLAG)
        {
            pvt_extn_len = EGTPU_GET_U16BIT(p_egtpu_info->p_pvt_extn);
        }
        msg_len = EGTPU_API_HDR_LEN + EGTPU_SIZE_OF_TYPE +
            EGTPU_SIZE_OF_TRANSACTION_ID + EGTPU_SIZE_OF_TYPE +
            EGTPU_SIZE_OF_LENGTH + pvt_extn_len;

        EGTPU_GET_MSG_BUF (p_global,U8bit, p_buf, msg_len);
        
        /* Rel 5.3: Coverity 24415 Fix Start */
        if (EGTPU_NULL == p_buf)
        {
            /* Not able to allocate Memory */
            return EGTPU_FAILURE;
        }
        /* Rel 5.3: Coverity Fix End */

        p_param = p_buf + EGTPU_API_HDR_LEN;
        *p_param++ = EGTPU_IE_TRANSACTION_ID;
        EGTPU_SET_U32BIT (p_param, p_rab_ctxt->transaction_id);
        p_param += EGTPU_SIZE_OF_TRANSACTION_ID;

        if (p_egtpu_info->ie_presence & EGTPU_IE_PVT_EXTN_FLAG)
        {
            *p_param++ = EGTPU_IE_PRIVATE_EXTENSION;
            egtpu_memcpy(p_param, p_egtpu_info->p_pvt_extn,
                    EGTPU_SIZE_OF_LENGTH + pvt_extn_len);
            p_param += EGTPU_SIZE_OF_LENGTH + pvt_extn_len;
        }

        EGTPU_SET_API_HDR (p_buf, EGTPU_EI_GTPU, EGTPU_EI_CP,
                EGTPU_CP_END_MARKER_IND, (p_param - p_buf),
                EGTPU_NO_PAYLOAD, EGTPU_NO_PAYLOAD, (p_param - p_buf - 1));
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_END_MARKER_SENT_TO_CP, LOGINFO, PR_GTPU_DL_DATA,
                /* + SPR 16766 */
                relayGtpuCurrentTick_g, __LINE__, p_egtpu_info->teid_self, 
                /* - SPR 16766 */
                0,0,0, 0,0, __func__,"");

        /* SPR 2030 fix End*/

        EGTPU_SEND_FROM_GTPU (p_global,p_buf,
                (p_param - p_buf), EGTPU_EI_CP, ret_val,NULL);


        /*SPR #2700 fix Start*/
        EGTPU_FREE_MSG_BUF (p_buf);
        /*SPR #2700 fix End*/


        EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_CP_END_MARKER_IND);
    }   
    else
    {
        /* Send the End Marker Indication to the CP entity as well as PDCP
         * which will forward local EMI */
        *p_param++ = EGTPU_IE_TRANSACTION_ID;
        EGTPU_SET_U32BIT (p_param, p_rab_ctxt->transaction_id);
        p_param += EGTPU_SIZE_OF_TRANSACTION_ID;

        if (p_egtpu_info->ie_presence & EGTPU_IE_PVT_EXTN_FLAG)
        {
            *p_param++ = EGTPU_IE_PRIVATE_EXTENSION;
            pvt_extn_len = EGTPU_GET_U16BIT(p_egtpu_info->p_pvt_extn);
            egtpu_memcpy(p_param, p_egtpu_info->p_pvt_extn,
                    EGTPU_SIZE_OF_LENGTH + pvt_extn_len);
            p_param += EGTPU_SIZE_OF_LENGTH + pvt_extn_len;
        }

        EGTPU_SET_API_HDR (p_egtpu_info->p_buff, EGTPU_EI_GTPU, EGTPU_EI_CP,
                EGTPU_CP_END_MARKER_IND, (p_param - p_egtpu_info->p_buff),
                EGTPU_NO_PAYLOAD, EGTPU_NO_PAYLOAD, (p_param - p_egtpu_info->p_buff - 1));
        
        LOG_GTPU_MSG(GTPU_END_MARKER_SENT_TO_CP, LOGINFO, PR_GTPU_DL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                0,0,0,0, 0,0, __func__,"");

        EGTPU_SEND_FROM_GTPU (p_global,p_egtpu_info->p_buff,
                (p_param - p_egtpu_info->p_buff), EGTPU_EI_CP, ret_val, NULL);


        EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_CP_END_MARKER_IND);
    }

    return ret_val;
}

       
/***********************************************************************************
 * Function Name  : egtpu_unitdata_ind 
 * Inputs         : p_global
 *                  p_egtpu_info
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE 
 * Description    : This function processes indication from UDP
 *                  calls respective message handlers for GTP messages
 *                  also records statistics
 **********************************************************************************/
egtpu_return_t egtpu_unitdata_ind (egtpu_global_t *p_global,
									egtpu_info_t *p_egtpu_info)
{
    EGTPU_TRACE_HLR_ENTR(p_global,egtpu_get_msg_name(p_egtpu_info->pdu_type));
    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_UNITDATA_IND);

    switch (p_egtpu_info->pdu_type)
	{
        case EGTPU_MSGT_TPDU:
            EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_PDU_RECEIVED);
            __tpdu_msg__ (p_global,p_egtpu_info);
            break;

        case EGTPU_MSGT_ECHO_REQUEST:
            EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_ECHO_REQ_MSG_RECEIVED);
            egtpu_echo_request (p_global,p_egtpu_info);
            break;

        case EGTPU_MSGT_ECHO_RESPONSE:
            EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_ECHO_RES_MSG_RECEIVED);
            egtpu_echo_response (p_global,p_egtpu_info);
            break;

        case EGTPU_MSGT_ERROR_INDICATION:
            EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_ERROR_IND_MSG_RECEIVED);
            __error_ind_msg__ (p_global,p_egtpu_info);
            break;

        case EGTPU_MSGT_END_MARKER:
            EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_END_MARKER_MSG_RECEIVED);
            __end_marker_msg__ (p_global,p_egtpu_info);
            break;
    }

    EGTPU_TRACE_HLR_EXIT(p_global,egtpu_get_msg_name(p_egtpu_info->pdu_type));
    return EGTPU_SUCCESS;
}

/***********************************************************************************
 * Function Name  : egtpu_set_max_seq_num_in_hash_tbl 
 * Inputs         : p_rab_ctxt - pointer of structure egtpu_rab_ctxt_t
 *                  p_hash_rec - pointer of type egtpu_hash_tbl
 * Outputs        : None 
 * Returns        : min/max seq no 
 * Description    : This function set max seq no and min seq no
 **********************************************************************************/
egtpu_seq_num_t egtpu_set_max_seq_num_in_hash_tbl(egtpu_rab_ctxt_t *p_rab_ctxt,
												egtpu_hash_tbl *p_hash_rec )
{
	egtpu_seq_num_t max, min;

	if(p_rab_ctxt->max_seq_num == EGTPU_INV_SEQ_NUM)
		return p_hash_rec->seq_num;

	if( p_rab_ctxt->max_seq_num < p_hash_rec->seq_num)
	{
		max =  p_hash_rec->seq_num;
	    min =  p_rab_ctxt->max_seq_num;
	}
	else
	{
	    max =  p_rab_ctxt->max_seq_num;
		min =  p_hash_rec->seq_num;
	}

    /* SPR 18122 Changes Start*/
    if( (max - min) > (DIVIDE_BY_TWO(EGTPU_MAX_SEQ_NUM)) )
        /* SPR 18122 Changes End*/
		return min;
	else
	  	return max;
}

/***********************************************************************************
 * Function Name  : egtpu_chk_starvation 
 * Inputs         : p_rab_ctxt - pointer of structure egtpu_rab_ctxt_t
 *                  p_global - pointer of structure egtpu_global_t
 * Outputs        : None 
 * Returns        : EGTP_SUCCESS/EGTP_FAILURE
 * Description    : This function check the starvation. 
 **********************************************************************************/
egtpu_return_t egtpu_chk_starvation(egtpu_rab_ctxt_t *p_rab_ctxt,
										egtpu_global_t *p_global)
{
    YHNODE 				 *FOUND =EGTPU_NULL;
    egtpu_info_t 		 egtpu_info_reorder;
	egtpu_seq_num_t 		 end;
	egtpu_seq_num_t		 j = EGTPU_MIN_SEQ_NUM;
    egtpu_return_t		 result	= EGTPU_FAILURE ;
	
	egtpu_bzero(&egtpu_info_reorder,sizeof(egtpu_info_t));

    if(p_rab_ctxt->guard_arr[0] != EGTPU_INV_SEQ_NUM &&
					EGTPU_COUNT_PDU_IN_HASH_TABLE(p_rab_ctxt->H) != 0)
    {
  		/* If minimum sequence number gpdu present in the hash table has
		 * a smaller sequence no.value than  the guard_array[0] then we 
		 * forward all the gpdu's which have sequence number less than 
		 * or equal to the guard_arr[0] and present in the hash table.
   		 */
        /* SPR 18122 Changes Start*/
	  	 if( (p_rab_ctxt->nxt_seq <= p_rab_ctxt->guard_arr[0] ||
            (p_rab_ctxt->nxt_seq - p_rab_ctxt->guard_arr[0] > (DIVIDE_BY_TWO(EGTPU_MAX_SEQ_NUM)))) &&
            !(p_rab_ctxt->guard_arr[0] - p_rab_ctxt->nxt_seq > (DIVIDE_BY_TWO(EGTPU_MAX_SEQ_NUM)))) 
            /* SPR 18122 Changes End*/
      {
  	      /* We do not have the correct value in the 
	       * p_rab_ctxt->min_seq_num so need to find it
	       */
	      j = p_rab_ctxt->nxt_seq ; 

		  if(p_rab_ctxt->guard_arr[0] == EGTPU_MAX_SEQ_NUM)
				end = 0;
		  else
		  		end = p_rab_ctxt->guard_arr[0] + 1;

		  while(EGTPU_COUNT_PDU_IN_HASH_TABLE(p_rab_ctxt->H))
	      {
			 if((result = egtpu_find_in_hash_table
				(p_rab_ctxt->H,(egtpu_seq_num_t) j,&FOUND)) == EGTPU_SUCCESS)
           	 {
  			    egtpu_info_reorder.transaction_id = p_rab_ctxt->transaction_id;
                /* Rel 5.3: Coverity 24417 Fix Start */
				egtpu_info_reorder.p_buff = 
				(U8bit *)(YMEMBEROFWO(egtpu_hash_tbl, lnode, FOUND)->p_buff);
                /* Rel 5.3: Coverity 24417 Fix End */
				
				if(egtpu_info_reorder.p_buff)
                	egtpu_send_hash_node_relay(p_global, &egtpu_info_reorder);
             	p_rab_ctxt->seq_num_dnlk = egtpu_info_reorder.seq_num_dnlk;

		        EGTPU_FREE_HASH_REC(p_rab_ctxt->H ,FOUND);

				if(j == EGTPU_MAX_SEQ_NUM)
  						p_rab_ctxt->nxt_seq = EGTPU_MIN_SEQ_NUM;
			    else
				  		p_rab_ctxt->nxt_seq = j + 1;
			 }
			
			 if(j == EGTPU_MAX_SEQ_NUM)
			  	 j = 0;
			 else
				 j++;
                /* SPR 18122 Changes Start*/
                if(!(j - end > (DIVIDE_BY_TWO(EGTPU_MAX_SEQ_NUM))) && j >= end && result == EGTPU_FAILURE)
                    /* SPR 18122 Changes End*/
			 	break;
		 }
	  }
	  else 
            /* SPR 18122 Changes Start*/
            if(p_rab_ctxt->guard_arr[0] - p_rab_ctxt->nxt_seq > DIVIDE_BY_TWO(EGTPU_MAX_SEQ_NUM))
                /* SPR 18122 Changes End*/
	  {
			 if(egtpu_find_in_hash_table
				 (p_rab_ctxt->H,
				 (egtpu_seq_num_t)p_rab_ctxt->guard_arr[0],
				 &FOUND) == EGTPU_SUCCESS)
		     		EGTPU_FREE_HASH_REC(p_rab_ctxt->H ,FOUND);
	  
	  }
	}
	return EGTPU_SUCCESS;
}	

/***********************************************************************************
 * Function Name  : egtpu_reordering 
 * Inputs         : p_rab_ctxt - pointer of structure egtpu_rab_ctxt_t
 *                  p_global - pointer of structure egtpu_global_t
 *                  p_egtpu_info - pointer of structure egtpu_info_t
 * Outputs        : None 
 * Returns        : EGTP_SUCCESS/EGTP_FAILURE
 * Description    : This function is used to reorder the out of order PDU
 *                  (out of order seq no).
 **********************************************************************************/
egtpu_return_t egtpu_reordering(egtpu_rab_ctxt_t *p_rab_ctxt,
								egtpu_global_t *p_global,
								egtpu_info_t *p_egtpu_info)
{
    YHNODE 			*FOUND 					= EGTPU_NULL;
	egtpu_hash_tbl   *p_hash_rec 			= EGTPU_NULL;
	egtpu_info_t     egtpu_info_reorder;
    egtpu_return_t	result					= EGTPU_FAILURE;
	egtpu_seq_num_t  j						= EGTPU_MIN_SEQ_NUM;
	
	egtpu_bzero(&egtpu_info_reorder,sizeof(egtpu_info_t));
	if(p_egtpu_info->seq_num_dnlk == p_rab_ctxt->nxt_seq)
	{
      p_egtpu_info->transaction_id = p_rab_ctxt->transaction_id;
	  /* Send the GPDU and increment the expected sequence number */
	  egtpu_send_gpdu_relay(p_global ,p_egtpu_info);
      p_rab_ctxt->seq_num_dnlk = p_egtpu_info->seq_num_dnlk;

	  /* Taking care of wrap around */
	  if(p_rab_ctxt->nxt_seq == EGTPU_MAX_SEQ_NUM)
  			p_rab_ctxt->nxt_seq = EGTPU_MIN_SEQ_NUM;
	  else
			p_rab_ctxt->nxt_seq++;

	  /* Check if the timer is running */
	  if( p_rab_ctxt->p_tid != EGTPU_INVALID_TIMER_ID )
	  {
	 	   /* We have received the expected sequence number,
	    	* we will have to stop the timer
	    	*/
		   /*EGTPU_STOP_REORDER_TIMER(p_global, p_rab_ctxt);*/

		   while(egtpu_find_in_hash_table
			   		(p_rab_ctxt->H,
					p_rab_ctxt->nxt_seq ,&FOUND) == EGTPU_SUCCESS)
		   {
		     egtpu_info_reorder.transaction_id = p_rab_ctxt->transaction_id;
                if(FOUND) {
			        egtpu_info_reorder.p_buff = 
			        (U8bit *)(YMEMBEROF(egtpu_hash_tbl, lnode, FOUND)->p_buff);

		     	    egtpu_send_hash_node_relay(p_global, &egtpu_info_reorder);
                    p_rab_ctxt->seq_num_dnlk = egtpu_info_reorder.seq_num_dnlk;

		            EGTPU_FREE_HASH_REC(p_rab_ctxt->H ,FOUND);
                }

			  /* Taking care of wrap around */
			  if(p_rab_ctxt->nxt_seq == EGTPU_MAX_SEQ_NUM)
  					p_rab_ctxt->nxt_seq = EGTPU_MIN_SEQ_NUM;
			  else
	 			    p_rab_ctxt->nxt_seq++;
		   }

		   /* The guard array is checked whether we meet its requirement
		    *  or not
		    */
		    egtpu_chk_starvation(p_rab_ctxt, p_global);
			
		   if( EGTPU_COUNT_PDU_IN_HASH_TABLE(p_rab_ctxt->H ))
		   {	
			  /*EGTPU_START_REORDER_TIMER(p_global, p_rab_ctxt);*/						
		   }
   	}
	return EGTPU_SUCCESS;
   }
    /* SPR 18122 Changes Start*/
   if( ((p_egtpu_info->seq_num_dnlk > p_rab_ctxt->nxt_seq) &&
         !(p_egtpu_info->seq_num_dnlk - p_rab_ctxt->nxt_seq > (DIVIDE_BY_TWO(EGTPU_MAX_SEQ_NUM)))) ||
         p_rab_ctxt->nxt_seq - p_egtpu_info->seq_num_dnlk > (DIVIDE_BY_TWO(EGTPU_MAX_SEQ_NUM)))
        /* SPR 18122 Changes End*/
   {
  		/* If this is a duplicate packet then discard the pdu and return */
	   if(egtpu_find_in_hash_table
		   (p_rab_ctxt->H,
		   p_egtpu_info->seq_num_dnlk ,&FOUND) == EGTPU_SUCCESS)
	   {
			EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_PDU_DISCARDED);
                /* SPR 2007 Fix - Part2 Start*/
                /* EGTPU_FREE_MSG_BUF(p_egtpu_info->p_buff);*/
                /* SPR 2007 Fix - Part2 End*/
    		return EGTPU_FAILURE;
 	   }
	  
	   /* Creating a hash record for storage */
	   EGTPU_GET_HASH_REC(p_global,p_global->hash_ctxt_pool, p_hash_rec); 

	   /* Storing the recevied info in the hash record */ 
	   p_hash_rec->seq_num = p_egtpu_info->seq_num_dnlk;
	   p_hash_rec->p_buff  = p_egtpu_info->p_buff;
	 
	   /* Setting the max and min seq_num values received during a timer interval */
	   p_rab_ctxt->max_seq_num = 
	   				egtpu_set_max_seq_num_in_hash_tbl(p_rab_ctxt, p_hash_rec);

	   /* Insert the hash record in the hash table */
       egtpu_insert_in_hash_table (p_hash_rec , p_rab_ctxt->H);

	   /* Check if the hash table has space or not */
	   if(EGTPU_COUNT_PDU_IN_HASH_TABLE(p_rab_ctxt->H) >=
		   									p_rab_ctxt->max_window_size)
	   {
		 /* the hash table has no space and has become full so the timer 
		  *	has to be stopped.
		  */
	      /*EGTPU_STOP_REORDER_TIMER(p_global, p_rab_ctxt);*/
	  
		 /* Now we will have to forward the stored packets to relay to create
		  * space. We set the nxt_seq_num to the min seq_num value packet in 
		  * the hash table and forward it. The packets succeding it are also 
		  * forwarded in the while loop till we have consecutive packets
          * or the hash table has been exhausted
          */
  	     /* Taking care of wrap around */
		  if(p_rab_ctxt->nxt_seq == EGTPU_MAX_SEQ_NUM)
  				p_rab_ctxt->nxt_seq = EGTPU_MIN_SEQ_NUM;
		  else
	 		    p_rab_ctxt->nxt_seq++;

		  j = p_rab_ctxt->nxt_seq ; 

		  while(EGTPU_COUNT_PDU_IN_HASH_TABLE(p_rab_ctxt->H))
	      {
			 if(egtpu_find_in_hash_table
				 	(p_rab_ctxt->H,
						(egtpu_seq_num_t) j,&FOUND) == EGTPU_SUCCESS)
           	 {
  			    egtpu_info_reorder.transaction_id = p_rab_ctxt->transaction_id;
                if(FOUND) {
				    egtpu_info_reorder.p_buff = 
				    (U8bit *)(YMEMBEROF(egtpu_hash_tbl, lnode, FOUND)->p_buff);

                	egtpu_send_hash_node_relay(p_global, &egtpu_info_reorder);
	                p_rab_ctxt->seq_num_dnlk = egtpu_info_reorder.seq_num_dnlk;

		            EGTPU_FREE_HASH_REC(p_rab_ctxt->H ,FOUND);
                }

				if(j == EGTPU_MAX_SEQ_NUM)
  						p_rab_ctxt->nxt_seq = EGTPU_MIN_SEQ_NUM;
			    else
				  		p_rab_ctxt->nxt_seq = j + 1;

			 	result = EGTPU_SUCCESS;
			 }
			 else if(result == EGTPU_SUCCESS)
			   			break;
			
			 if(j == EGTPU_MAX_SEQ_NUM)
			  	 j = EGTPU_MIN_SEQ_NUM;
			 else
				 j++;
		 }

	     /* The guard array is checked whether we meet its requirement or not */
		 egtpu_chk_starvation(p_rab_ctxt , p_global);
        }

	    /* Check if the hash table is not empty and a previous is already not 
		 * running then start the timer
         */
	    if( p_rab_ctxt->p_tid == EGTPU_INVALID_TIMER_ID &&
				EGTPU_COUNT_PDU_IN_HASH_TABLE(p_rab_ctxt->H ))
		{
			;
   			/*EGTPU_START_REORDER_TIMER(p_global, p_rab_ctxt);*/
		}
		return EGTPU_SUCCESS;
 	}

	EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_PDU_DISCARDED);
       /* SPR 2007 Fix - Part2 Start*/
       /* EGTPU_FREE_MSG_BUF(p_egtpu_info->p_buff);*/
       /* SPR 2007 Fix - Part2 End*/
	return EGTPU_SUCCESS;
}
