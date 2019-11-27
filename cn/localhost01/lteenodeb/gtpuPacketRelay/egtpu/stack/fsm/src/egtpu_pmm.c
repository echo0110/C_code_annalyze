/***********************************************************************
**  FUNCTION:
**	GTP-U Path Management Routines
************************************************************************
**
**  FILE NAME:
**	egtpu_pmm.c
**
**  DESCRIPTION:
**	Contains routines to perform echo and reorder mechanism
**
**  DATE	    NAME			REFERENCE		REASON
**  ----	    ----			---------		------
**  Oct 2009    Vipul Aggarwal          		Initial
**
**   Copyright (C) 2009 Aricent Inc . All Rights Reserved
***********************************************************************/

#include <ylib.h>
#include <egtpu_glob.h>
#include <egtpu_apihdr.h>

#include <egtpu_msgtypes.h>
#include <egtpu_ietypes.h>
#include <egtpu_stat.h>
#include <egtpu_glob_struct.h>
#include <egtpu_cdb.h>
#include <egtpu_os_proto.h>
#include <egtpu_error.h>
#include <egtpu_trace.h>
#include <egtpu_util.h>
#include <egtpu_tmr.h>
#include <egtpu_pmm.h>
#include <egtpu_udi.h>


unsigned char echo_timer_peer_addr_g[EGTPU_MAX_IP6_ADDR_SIZE] = {0};
/***********************************************************************************
 * Function Name  : __send_path_failure_ind_to_cp__
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  p_addr - Pointer to Peer's address
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This function prepares and sends PATH FAILURE indication to CP,
 *                  CP shall deactivate all the RABs associate with the given
 *                  peer address.
 ***********************************************************************************/
static egtpu_return_t __send_path_failure_ind_to_cp__(egtpu_global_t *p_global,
														U8bit *p_addr)
{
    U8bit         *p_buf, *p_trav;
    egtpu_return_t ret_val;

    EGTPU_TRACE_FN_ENTR (p_global,"__send_path_failure_ind_to_cp__");

    EGTPU_GET_MSG_BUF (p_global,U8bit, p_buf, EGTPU_API_MSG_SIZE);

    /* Rel 5.3: Coverity 24408 Fix Start */
    if (EGTPU_NULL == p_buf)
    {
        /* Not able to allocate memory */
        EGTPU_TRACE_FN_EXIT (p_global,"__send_path_failure_ind_to_cp__");
        return EGTPU_FAILURE;
    }
    /* Rel 5.3: Coverity Fix End */

    p_trav = p_buf + EGTPU_API_HDR_LEN;

    *p_trav++ = EGTPU_IE_GSN_ADDR;
    /* Coverity 73548 Fix Start*/
    if((EGTPU_GET_U16BIT(p_addr) + EGTPU_SIZE_OF_LENGTH) > (EGTPU_API_MSG_SIZE - EGTPU_API_HDR_LEN))
    {
        EGTPU_FREE_MSG_BUF (p_buf);
        return EGTPU_FAILURE;
    }
    /* Coverity 73548 Fix End*/

    egtpu_memcpy(p_trav, p_addr,
						EGTPU_GET_U16BIT(p_addr) + EGTPU_SIZE_OF_LENGTH);
    p_trav += (EGTPU_GET_U16BIT(p_addr) + EGTPU_SIZE_OF_LENGTH);

    EGTPU_SET_API_HDR(p_buf, EGTPU_EI_GTPU, EGTPU_EI_CP,
            EGTPU_CP_PATH_FAILURE_IND, (p_trav - p_buf),
            EGTPU_NO_PAYLOAD, EGTPU_NO_PAYLOAD, (p_trav - p_buf - 1));
/*+SPR #3456*/
    EGTPU_SEND_FROM_GTPU (p_global,p_buf, (p_trav - p_buf), EGTPU_EI_RELAY,\
							ret_val, NULL);
/*-SPR #3456*/
    /*SPR #2716 fix Start*/
    EGTPU_FREE_MSG_BUF (p_buf);
    /*SPR #2716 fix End*/
    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_CP_PATH_FAILURE_IND);

    EGTPU_TRACE_FN_EXIT (p_global,"__send_path_failure_ind_to_cp__");
    return ret_val;
}

/*+SPR #3456*/
/********************************************************************************
 * Function Name  : __send_path_success_ind_to_cp__
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  p_addr - Pointer to Peer's address
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This function prepares and sends PATH SUCCESS indication to CP,
 *                  CP shall activate all the RABs associate with the given
 *                  peer address.
 ********************************************************************************/
egtpu_return_t __send_path_success_ind_to_cp__(egtpu_global_t *p_global,
														U8bit *p_addr,
                                                        U8bit alarm_flag)
{
    U8bit         *p_buf, *p_trav;
    egtpu_return_t ret_val;

    EGTPU_TRACE_FN_ENTR (p_global,"__send_path_success_ind_to_cp__");

    EGTPU_GET_MSG_BUF (p_global,U8bit, p_buf, EGTPU_API_MSG_SIZE);

    /* Rel 5.3: Coverity 24409 Fix Start */
    if (EGTPU_NULL == p_buf)
    {
        /* Not able to allocate memory */
        EGTPU_TRACE_FN_EXIT (p_global,"__send_path_success_ind_to_cp__");
        return EGTPU_FAILURE;
    }
    /* Rel 5.3: Coverity Fix End */

    p_trav = p_buf + EGTPU_API_HDR_LEN;
/* SPR 13765 fix START */
	if(EGTPU_INVALID == alarm_flag)
	{
        *p_trav++ = EGTPU_IE_GSN_ADDR;
	}
	else if(alarm_flag)
	{
        *p_trav++ = EGTPU_IE_GSN_ADDR_FLAG;
	    *p_trav++ = alarm_flag ;
	}
/* SPR 13765 fix END */
    /* Coverity 72786 Fix Start*/
    if((EGTPU_GET_U16BIT(p_addr) + EGTPU_SIZE_OF_LENGTH) >
            ( EGTPU_API_MSG_SIZE - EGTPU_API_HDR_LEN))
    {
        EGTPU_TRACE_FN_EXIT (p_global,"__send_path_success_ind_to_cp__");
        EGTPU_FREE_MSG_BUF (p_buf);
        p_buf = EGTPU_NULL;
        p_trav = EGTPU_NULL;
        return EGTPU_FAILURE;
    }
    /* Coverity 72786 Fix End*/

    egtpu_memcpy(p_trav, p_addr,
						EGTPU_GET_U16BIT(p_addr) + EGTPU_SIZE_OF_LENGTH);
    p_trav += (EGTPU_GET_U16BIT(p_addr) + EGTPU_SIZE_OF_LENGTH);

    EGTPU_SET_API_HDR(p_buf, EGTPU_EI_GTPU, EGTPU_EI_CP,
            /* PATH_SUCCESS Fix Start */
            EGTPU_CP_PATH_SUCCESS_IND, (p_trav - p_buf),
            /* PATH_SUCCESS Fix End */
            EGTPU_NO_PAYLOAD, EGTPU_NO_PAYLOAD, (p_trav - p_buf - 1));
    EGTPU_SEND_FROM_GTPU (p_global,p_buf, (p_trav - p_buf), EGTPU_EI_RELAY,\
							ret_val, NULL);
    /*SPR #2716 fix Start*/
    EGTPU_FREE_MSG_BUF (p_buf);
    /*SPR #2716 fix End*/
    /* Correct Message Updation Fix*/
    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_CP_PATH_SUCCESS_IND);

    EGTPU_TRACE_FN_EXIT (p_global,"__send_path_success_ind_to_cp__");
    return ret_val;
}
/*-SPR #3456*/
/********************************************************************************
 * Function Name  : egtpu_echo_timeout
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  p_ctxt - Pointer to egtpu_peer_ctxt_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This function handles the Echo Request timeout, Increment
 *                  ath counter, resend echo request message to peer On N3-EXPIRY,
 *                  path failure shall be declared, continues with pmm.
 ********************************************************************************/
/*+SPR #3456*/
egtpu_return_t egtpu_echo_timeout(egtpu_global_t *p_global,
									egtpu_peer_ctxt_t *p_ctxt)
/*-SPR #3456*/
{
    /* + coverity 32075 */
    egtpu_return_t    ret_val = EGTPU_SUCCESS;
    /* - coverity 32075 */

    /*+SPR #3456*/
    /*
       egtpu_peer_ctxt_t *p_ctxt = (egtpu_peer_ctxt_t *)
       EGTPU_GET_U32BIT(p_egtpu_info->p_buff + EGTPU_API_HDR_LEN);
       */
    /*-SPR #3456*/

    EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_ECHO_TIMER_EXPIRY);

    if (egtpu_db_validate_peer_ctxt (p_ctxt))
    {
        if (!p_ctxt->path_struct.tmr_flag)
        {
            EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_UNEXPECTED_TIMER_EXP,
                        egtpu_entity_name_arr[EGTPU_EI_GTPU],
                        "ECHO TIMER"));
            /* SPR 17747 FIX START */
            LOG_MSG(GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON,
                    relayGtpuCurrentTick_g, __LINE__,
                    EGTPU_PERR_UNEXPECTED_TIMER_EXP,0,0,0,0,0,
                     __func__, egtpu_entity_name_arr[EGTPU_EI_GTPU] );
            /* SPR 17747 FIX END */
        }
        else
        {
            EGTPU_STACK_TRACE(p_global,EGTPU_BRIEF_TRACE_LEVEL,EGTPU_FSM_TRACE,
                    (EGTPU_TS_PERR_ECHO_TIMER_EXP,
                     egtpu_entity_name_arr[EGTPU_EI_GTPU],
                     p_ctxt->path_struct.path_cntr + 1));

            p_ctxt->path_struct.tmr_flag = EGTPU_TIMER_OFF;

            /* if path counter is less than max retries increment path counter 
             * by one and send echo request message to peer and if path counter 
             * is greater or equal to max retries then send path failure
             *  indication to CP */
            if (p_ctxt->path_struct.path_cntr <
                    p_global->tmr_info[EGTPU_ECHO_RSP_TMR].max_retries)
            {
                p_ctxt->path_struct.path_cntr++;
                /*+SPR #3456*/
                egtpu_send_echo_req (p_global,p_ctxt->entity_addr);
                EGTPU_START_ECHO_TIMER (p_global,p_ctxt,
                        &p_ctxt->path_struct, ret_val);
                /*-SPR #3456*/
            }
            else
            {
                /*+SPR #3456*/
                if( p_ctxt->path_struct.path_status != EGTPU_PATH_STATUS_NOK)
                {
                    __send_path_failure_ind_to_cp__ (p_global,p_ctxt->entity_addr);
                    p_ctxt->path_struct.path_status = EGTPU_PATH_STATUS_NOK;
					/* SPR 13765 fix START */
						p_ctxt->is_alarm_raised = EGTPU_TRUE;
					/* SPR 13765 fix END */
                }
                /*-SPR #3456*/
                p_ctxt->path_struct.path_cntr = 0;

                /* + SPR_17858_667_CHANGES */
                /*egtpu_send_alarm_notification(p_global,EGTPU_CERR_PATH_FAILURE,
                      p_ctxt->entity_addr);*/
                /* - SPR_17858_667_CHANGES */

            }

            /*KAR:HORROR
              egtpu_send_echo_req (p_global,p_ctxt->entity_addr);*/
            /*EGTPU_START_ECHO_TIMER (p_global,p_ctxt,
              &p_ctxt->path_struct, ret_val);*/
        }
    }
    else
    {
        EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_DBERR_KEY_NOT_PRESENT,
                    egtpu_entity_name_arr[EGTPU_EI_GTPU]));

        /* SPR 17747 FIX START */
        LOG_MSG(GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON,
                relayGtpuCurrentTick_g, __LINE__,
                EGTPU_DBERR_KEY_NOT_PRESENT,0,0,0,0,0,
                __func__,egtpu_entity_name_arr[EGTPU_EI_GTPU] );
        /* SPR 17747 FIX END */

        EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_KEY_NOT_PRESENT);
    }

    /*EGTPU_FREE_TIMER_BLOCK(p_egtpu_info->p_buff);*/
    /* + coverity 32075 */
    return ret_val;
    /* - coverity 32075 */
}

/********************************************************************************
 * Function Name  : egtpu_reorder_timeout
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This function handles Reorder Timer Expiry.
 ********************************************************************************/
egtpu_return_t egtpu_reorder_timeout(egtpu_global_t *p_global,
									egtpu_info_t *p_egtpu_info)
{
    egtpu_info_t          egtpu_info_reorder;
    egtpu_rab_ctxt_t		*p_rab_ctxt = (egtpu_rab_ctxt_t *)
        /* SPR 20430 Changes Start*/
#ifdef __x86_64__
        EGTPU_GET_U64BIT(p_egtpu_info->p_buff + EGTPU_API_HDR_LEN);
#else
    EGTPU_GET_U32BIT(p_egtpu_info->p_buff + EGTPU_API_HDR_LEN);
#endif
    /* SPR 20430 Changes End*/
    YHNODE *FOUND = EGTPU_NULL;

    /* + coverity 11454 */
    egtpu_info_reorder.ie_presence = 0;
    /* - coverity 11454 */
    p_rab_ctxt->guard_arr[0] = p_rab_ctxt->guard_arr[1];           
    p_rab_ctxt->guard_arr[1] = p_rab_ctxt->guard_arr[2];           
    p_rab_ctxt->guard_arr[2] = p_rab_ctxt->guard_arr[3];           
    p_rab_ctxt->guard_arr[3] = p_rab_ctxt->max_seq_num;            

    p_rab_ctxt->max_seq_num  =  EGTPU_INV_SEQ_NUM;                                 

    /* The re-ordering timer has expired. Setting the timer id to invalid */
    p_rab_ctxt->p_tid = EGTPU_INVALID_TIMER_ID;

    /* Calling off the wait for the  elusive sequence number */
    /* Taking care of wrap around */
    if(p_rab_ctxt->nxt_seq == EGTPU_MAX_SEQ_NUM)
        p_rab_ctxt->nxt_seq = EGTPU_MIN_SEQ_NUM;
    else
        p_rab_ctxt->nxt_seq++;

    /* The next_seq is searched in the hash table. Packets succeding it are also 
     * forwarded in the while loop till we have consecutive packets
     * or the hash table has been exhausted
     */
    while(egtpu_find_in_hash_table(p_rab_ctxt->H,
                p_rab_ctxt->nxt_seq,&FOUND) == EGTPU_SUCCESS)
    {
        egtpu_info_reorder.transaction_id = p_rab_ctxt->transaction_id;
        if(FOUND) {
            egtpu_info_reorder.p_buff = 
                YMEMBEROF(egtpu_hash_tbl, lnode, FOUND)->p_buff;

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

    /* The guard array is checked whether we meet its requirement or not */
    egtpu_chk_starvation(p_rab_ctxt, p_global);

    /* Check if the hash table is empty or not. If it is not empty then
     * start the timer
     */
    if(EGTPU_COUNT_PDU_IN_HASH_TABLE(p_rab_ctxt->H ))
    {	
        /*EGTPU_START_REORDER_TIMER(p_global, p_rab_ctxt);*/
    }

    return EGTPU_SUCCESS;
} 

/********************************************************************************
 * Function Name  : egtpu_echo_response
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This function processes Echo Response Message,
 *                  If peer information is found, make path counter as zero.
 ********************************************************************************/
egtpu_return_t egtpu_echo_response (egtpu_global_t *p_global,
									egtpu_info_t *p_egtpu_info)
{
	UInt8 alarm_flag;   /* SPR 13765 fix */
    egtpu_peer_ctxt_t *p_ctxt =
        egtpu_db_sel_addr_key_peer_ctxt (p_egtpu_info->p_peer_addr);

    if (!p_ctxt)
	{
        EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_UNEXPECTED_MSG,
                    egtpu_entity_name_arr[EGTPU_EI_GTPU],
                    egtpu_get_msg_name (EGTPU_MSGT_ECHO_RESPONSE)));
    /* SPR 2030 fix Start*/
    LOG_GTPU_MSG(GTPU_ECHO_RESP_RECVD_FROM_UNKNOWN_PEER, LOGWARNING, PR_GTPU_DL_DATA,
             relayGtpuCurrentTick_g, __LINE__, 
             0,0,0,0, 0,0, __func__,"");
     /* SPR 2030 fix End*/
	
 	EGTPU_UPDATE_STATS(p_global,\
				EGTPU_STAT_ERR_UNEXPECTED_GTP_SIGNALLING_MESSAGE);			
        EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_KEY_NOT_PRESENT);
    }
	else
/*+SPR #3456*/
        /*PATH_SUCCESS_IND shall be sent only first time when ECHO_RESP is
         * received from the peer or whenever the path status changes from NOK
         * to OK*/
    {
        p_ctxt->path_struct.path_cntr = 0;
        if(p_ctxt->path_struct.path_status != EGTPU_PATH_STATUS_OK)
        {
		/* SPR 13765 fix START */
			if(p_ctxt->is_alarm_raised)            /* check if alarm was raised */
			{
				alarm_flag = EGTPU_SEND_ALARM_WITH_PATH_SUCCESS;
			    p_ctxt->is_alarm_raised = EGTPU_FALSE;
			}
			else
			{
				alarm_flag = EGTPU_INVALID;
			}
             __send_path_success_ind_to_cp__ (p_global,p_ctxt->entity_addr,alarm_flag);
             p_ctxt->path_struct.path_status = EGTPU_PATH_STATUS_OK;
		/* SPR 13765 fix END */
		}
    }
/*+SPR #3456*/
     /* SPR 2007 Fix - Part2 Start*/
    /* EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);*/
    /* SPR 2007 Fix - Part2 End*/

    /* SPR 2030 fix Start*/
    LOG_GTPU_MSG(GTPU_ECHO_RESP_RECVD_FROM_PEER, LOGINFO, PR_GTPU_DL_DATA,
             relayGtpuCurrentTick_g, __LINE__, 
             0,0,0,0, 0,0, __func__,"");
     /* SPR 2030 fix End*/


    return EGTPU_SUCCESS;
}

/********************************************************************************
 * Function Name  : egtpu_echo_request
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This function processes Echo Request Message from peer,
 *                  Always respond with Echo Response message.
 ********************************************************************************/
egtpu_return_t egtpu_echo_request (egtpu_global_t *p_global,
									egtpu_info_t *p_egtpu_info)
{

    U8bit         *p_buf, *p_trav, addr_len;
    U16bit        st_payld;
    egtpu_return_t ret_val;
    /* + SPR 19022 */
    /* coverity fix 108671 ++ */
    U32bit dummy_id = 0xff;
    /* coverity fix 108671 -- */
    /* - SPR 19022 */

/*+ SPR 21550 fix start*/
    EGTPU_GET_MSG_BUF (p_global,U8bit, p_buf, EGTPU_API_MSG_SIZE);
/*+ SPR 21550 fix end*/
    /* Rel 5.3: Coverity 24410 Fix Start */
    if (EGTPU_NULL == p_buf)
    {
        /* Not able to allocate memory */
        return EGTPU_FAILURE;
    }
    /* Rel 5.3: Coverity Fix End */
    /* Coverity 72766 Fix Start*/
    if(p_egtpu_info->p_peer_addr == EGTPU_NULL )
    {
/*+ SPR 21550 fix start*/
        EGTPU_FREE_MSG_BUF (p_buf);
/*+ SPR 21550 fix end*/
        return EGTPU_FAILURE;
    }
    /* Coverity 72766 Fix End*/

    p_trav = p_buf + EGTPU_API_HDR_LEN;

    addr_len = EGTPU_GET_U16BIT(p_egtpu_info->p_peer_addr);

    /* Coverity 72839 Fix Start*/
    if((EGTPU_GET_U16BIT (p_egtpu_info->p_peer_addr) + 2) > (EGTPU_API_MSG_SIZE
                - EGTPU_API_HDR_LEN))
    {
/*+ SPR 21550 fix start*/
        EGTPU_FREE_MSG_BUF (p_buf);
/*+ SPR 21550 fix end*/
        return EGTPU_FAILURE;
    }
    /* Coverity 72839 Fix End*/

    EGTPU_FILL_PEER_INFO (p_trav,
            p_egtpu_info->port_num, p_egtpu_info->p_peer_addr);
    /* + SPR 19022 */
    EGTPU_SET_U32BIT (p_trav, dummy_id);
    p_trav += EGTPU_SIZE_OF_QOS_ID;
    /* - SPR 19022 */
    st_payld = p_trav - p_buf; /* offset to start of payload */

    EGTPU_SET_GTP_HDR(p_trav, EGTPU_UNSET_EXT_HDR_FLAG,
            EGTPU_SET_SEQ_NUM_FLAG, EGTPU_UNSET_PN_FLAG,
            EGTPU_MSGT_ECHO_RESPONSE, EGTPU_MIN_ECHO_RSP_LENGTH,
            EGTPU_INV_TEID_VALUE, p_egtpu_info->seq_num_dnlk,
            EGTPU_SPARE_BYTE, EGTPU_SPARE_BYTE);
    p_trav += EGTPU_HDR_LEN_WITH_SEQ_NUM;

    *p_trav++ = EGTPU_IE_RECOVERY;
    *p_trav++ = EGTPU_RECOVERY_VALUE;


    /** Send the echo response from the primary source address
      (i.e. first my_addr configured during initialization) **/

    if(addr_len==EGTPU_SIZE_OF_IPV4_ADDR)
    {
        EGTPU_SET_API_HDR (p_buf, EGTPU_EI_GTPU, EGTPU_EI_UDP,
                EGTPU_UDP_UNITDATA_REQ, (p_trav - p_buf),
                (p_trav - p_buf - 1), st_payld, (st_payld - 1));

        EGTPU_SEND_FROM_GTPU(p_global,p_buf, (p_trav - p_buf), EGTPU_EI_UDP,\


                ret_val, p_global->initdb_info.my_addr[0]);
    }
    else
    {
        EGTPU_SET_API_HDR (p_buf, EGTPU_EI_GTPU, EGTPU_EI_UDP6,
                EGTPU_UDP_UNITDATA_REQ, (p_trav - p_buf),
                (p_trav - p_buf - 1), st_payld, (st_payld - 1));

        EGTPU_SEND_FROM_GTPU(p_global,p_buf, (p_trav - p_buf), EGTPU_EI_UDP6,\
                ret_val, p_global->initdb_info.my_addr6[0]);
    }


    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_UDP_UNITDATA_REQ);
    EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_ECHO_RES_MSG_SENT);
    /* SPR 2030 fix Start*/
    LOG_GTPU_MSG(GTPU_ECHO_RESP_SENT_TO_PEER, LOGINFO, PR_GTPU_DL_DATA,
            relayGtpuCurrentTick_g, __LINE__, 
            0,0,0,0, 0,0, __func__,"");
    /* SPR 2030 fix End*/

    /* free the incoming buffer */
    /* SPR 2007 Fix Start */
    /* EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);*/
/*+ SPR 21550 fix start*/
    EGTPU_FREE_MSG_BUF (p_buf);
/*+ SPR 21550 fix end*/

    /* SPR 2007 Fix End */ 
    /* + coverity 32076 */ 
    return ret_val;
    /* - coverity 32076 */ 
}

/********************************************************************************
 * Function Name  : egtpu_send_echo_req
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  p_addr - Pointer to Peer's address
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This function sends an Echo request to the Peer Entity.
 ********************************************************************************/
egtpu_return_t  egtpu_send_echo_req(egtpu_global_t *p_global,U8bit *p_addr)
{

    U8bit         *p_buf = NULL , *p_trav, addr_len;
    U16bit        st_payld;
    /* SPR  10015 Fix Start */
    U32bit dummy_id = 0xff;
    /* SPR  10015 Fix End */
    egtpu_return_t ret_val;

    EGTPU_TRACE_FN_ENTR (p_global,"egtpu_send_echo_req");

    addr_len = EGTPU_GET_U16BIT(p_addr);

/*+ SPR 21550 fix start*/
    EGTPU_GET_MSG_BUF (p_global,U8bit, p_buf, EGTPU_API_MSG_SIZE);
/*+ SPR 21550 fix end*/

    /* Rel 5.3: Coverity 24411 Fix Start */
    if (EGTPU_NULL == p_buf)
    {
        /* Not able to allocate memory */
        EGTPU_TRACE_FN_EXIT (p_global,"egtpu_send_echo_req");
        return EGTPU_FAILURE;
    }
    /* Rel 5.3: Coverity Fix End */

    p_trav = p_buf + EGTPU_API_HDR_LEN;

    /* Coverity 73268 Fix Start*/
    if((EGTPU_GET_U16BIT (p_addr) + 2) > ( EGTPU_API_MSG_SIZE - EGTPU_API_HDR_LEN ))
    {
/*+ SPR 21550 fix start*/
        EGTPU_FREE_MSG_BUF (p_buf);
/*+ SPR 21550 fix end*/
        return EGTPU_FAILURE;
    }
    /* Coverity 73268 Fix End*/


    EGTPU_FILL_PEER_INFO (p_trav, EGTPU_UDP_PORT, p_addr);
    /* SPR  10015 Fix Start */
    EGTPU_SET_U32BIT (p_trav, dummy_id);
    p_trav += EGTPU_SIZE_OF_QOS_ID;
    /* SPR  10015 Fix End */
    st_payld = p_trav - p_buf; /* offset to start of payload */

    EGTPU_SET_GTP_HDR(p_trav, EGTPU_UNSET_EXT_HDR_FLAG, EGTPU_SET_SEQ_NUM_FLAG,
            EGTPU_UNSET_PN_FLAG, EGTPU_MSGT_ECHO_REQUEST,
            EGTPU_MIN_ECHO_REQ_LENGTH, EGTPU_INV_TEID_VALUE,
            EGTPU_MIN_SEQ_NUM, EGTPU_SPARE_BYTE, EGTPU_SPARE_BYTE);
    p_trav += EGTPU_HDR_LEN_WITH_SEQ_NUM;



    if(addr_len==EGTPU_SIZE_OF_IPV4_ADDR)
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
        EGTPU_SEND_FROM_GTPU(p_global, p_buf, (p_trav - p_buf), EGTPU_EI_UDP6,\
                ret_val, p_global->initdb_info.my_addr6[0]);
    }
/*+ SPR 21550 fix start*/
    /*SPR #2716 fix Start*/
    EGTPU_FREE_MSG_BUF (p_buf);
    /*SPR #2716 fix End*/
/*+ SPR 21550 fix end*/

    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_UDP_UNITDATA_REQ);
    EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_ECHO_REQ_MSG_SENT);

    EGTPU_TRACE_FN_EXIT (p_global,"egtpu_send_echo_req");
    return ret_val;
}
