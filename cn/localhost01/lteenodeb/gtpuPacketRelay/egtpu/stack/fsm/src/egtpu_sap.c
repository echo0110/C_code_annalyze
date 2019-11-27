/**********************************************************************
** FILE
**     GTP-U CONTROL SAP FILE
************************************************************************
**
** DESCRIPTION
**     Handles setup and release of SAPs
**     Data behaviour of RABs is enabled/disabled here
**
** DATE      NAME        		REFERENCE    REASON
** ---       ----        		---------    ------
** Oct,2009  Vipul Aggarwal 				Original
** 14Jan 10  Vipul Aggarwal 				SPR FIX 7445
**
** Copyright 2009,Aricent Technolgies Ltd.
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
#include <egtpu_tmr.h>
#include <egtpu_util.h>

#include <egtpu_pmm.h>
#include <egtpu_sap.h>

#define __FWD_SAP_SET__(flag) ((flag) & EGTPU_FWD_SAP)
/* SPR 3546 changes starts */
extern unsigned char echo_timer_peer_addr_g[EGTPU_MAX_IP6_ADDR_SIZE];
/* SPR 3546 changes ends */

/* Cyclomatic Complexity changes - starts here */
STATIC inline egtpu_return_t egtpu_fill_create_sap_src_addr_info (
        egtpu_global_t    *p_global, 
        egtpu_info_t      *p_egtpu_info, 
        egtpu_rab_ctxt_t  *p_rab_ctxt);

STATIC inline egtpu_return_t egtpu_fill_create_sap_reordering_info (
        egtpu_global_t    *p_global, 
        egtpu_info_t      *p_egtpu_info, 
        egtpu_rab_ctxt_t  *p_rab_ctxt);

STATIC inline egtpu_return_t egtpu_fill_modify_sap_reordering_info (
        egtpu_global_t    *p_global, 
        egtpu_info_t      *p_egtpu_info, 
        egtpu_rab_ctxt_t  *p_rab_ctxt);

/* Cyclomatic Complexity changes - ends here */

/********************************************************************************
 * Function Name  : __enc_send_to_cp__
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t,
 *                  api_id - api id which will be sent to CP
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This function is called to send confirmation to control plane
 *                  api_id determines if cnf is for setup or release.
 ********************************************************************************/
STATIC egtpu_return_t __enc_send_to_cp__(egtpu_global_t *p_global,
					egtpu_info_t *p_egtpu_info,
					egtpu_api_id_t api_id)
{
    U8bit         *p_msg = EGTPU_NULL;
    U8bit         *p_param = EGTPU_NULL;
    egtpu_return_t ret_val;
    EGTPU_TRACE_FN_ENTR (p_global,"__enc_send_to_cp__");

    /* Get a msg Buffer to send outgoing cnf */
    EGTPU_GET_MSG_BUF (p_global,U8bit, p_msg, EGTPU_API_MSG_SIZE);

    /* Rel 5.3: Coverity 24414 Fix Start */
    if (EGTPU_NULL == p_msg)
    {
        /* Not able to allocate Memory */
        /* free the buffer for request */
        if(p_egtpu_info->mem_flag)
        {
            EGTPU_FREE_MSG_BUF(p_egtpu_info->p_src_addr);
        }
        EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);
        EGTPU_TRACE_FN_EXIT (p_global,"__enc_send_to_cp__");

        return EGTPU_FAILURE;
    }
    /* Rel 5.3: Coverity Fix End */

    /* p_param points to start parameter */
    p_param = p_msg + EGTPU_API_HDR_LEN;

    /* Set the cause parameter */
    *p_param++ = EGTPU_IE_CAUSE;
    *p_param++ = p_egtpu_info->cause;

    /* Set the transaction-id parameter; cause:success | failure */
    if (p_egtpu_info->ie_presence & EGTPU_IE_TRANSACTION_ID_FLAG)
	{
        *p_param++ = EGTPU_IE_TRANSACTION_ID;
        EGTPU_SET_U32BIT (p_param, p_egtpu_info->transaction_id);
        p_param += EGTPU_SIZE_OF_TRANSACTION_ID;
    }

    /* sent for first time for this ctxt;setup-first | ctxt-not-found */
    if (p_egtpu_info->ie_presence & EGTPU_IE_TEID_SELF_FLAG)
	{
        *p_param++ = EGTPU_IE_TEID_DATA_I;
        EGTPU_SET_U32BIT (p_param, p_egtpu_info->teid_self);
        p_param += EGTPU_SIZE_OF_TEID;
    }

    if (p_egtpu_info->ie_presence & EGTPU_IE_SEQ_NUM_UPLK_FLAG)
	{
        *p_param++ = EGTPU_IE_SEQ_NUM;
        EGTPU_SET_U16BIT (p_param, p_egtpu_info->seq_num_uplk);
        p_param += EGTPU_SIZE_OF_SEQ_NUM;

        *p_param++ = EGTPU_IE_SEQ_NUM;
        EGTPU_SET_U16BIT (p_param, p_egtpu_info->seq_num_dnlk);
        p_param += EGTPU_SIZE_OF_SEQ_NUM;
    }

    /* + Coverity 40837*/
    if ( (p_egtpu_info->ie_presence & EGTPU_IE_SOURCE_TRANSPORT_ADDR_FLAG) &&
          (PNULL != p_egtpu_info->p_src_addr) )
    /* - Coverity 40837*/
    {
        *p_param++ = EGTPU_IE_GSN_ADDR;	    
        {
            /* Coverity 73461 Fix Start*/
            if((EGTPU_GET_U16BIT(p_egtpu_info->p_src_addr)+EGTPU_SIZE_OF_LENGTH) >
                    (EGTPU_API_MSG_SIZE - EGTPU_API_HDR_LEN ))
            {
                if(p_egtpu_info->mem_flag)
                {
                    EGTPU_FREE_MSG_BUF(p_egtpu_info->p_src_addr);
                }
                EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);
                EGTPU_FREE_MSG_BUF (p_msg);
                EGTPU_TRACE_FN_EXIT (p_global,"__enc_send_to_cp__");
                return EGTPU_FAILURE;
            }
            /* Coverity 73461 Fix End*/

            egtpu_memcpy(p_param , p_egtpu_info->p_src_addr , \
                    EGTPU_GET_U16BIT(p_egtpu_info->p_src_addr)+EGTPU_SIZE_OF_LENGTH);
            p_param += (EGTPU_GET_U16BIT(p_egtpu_info->p_src_addr) + EGTPU_SIZE_OF_LENGTH);	
        }
    }


    EGTPU_SET_API_HDR (p_msg, EGTPU_EI_GTPU, EGTPU_EI_CP,
            api_id, (p_param - p_msg), EGTPU_NO_PAYLOAD, EGTPU_NO_PAYLOAD, (p_param - p_msg - 1));

    /* SPR 2030 fix Start*/
    LOG_GTPU_MSG(GTPU_SEND_CNF_TO_CP, LOGINFO, PR_GTPU_CONTROL,
            relayGtpuCurrentTick_g, __LINE__, 
            api_id,0,0,0, 0,0, __func__,"");
    /* SPR 2030 fix End*/

    EGTPU_SEND_FROM_GTPU (p_global,p_msg, (p_param - p_msg), EGTPU_EI_CP,
							ret_val,NULL);



    /* free the buffer for request */
    if(p_egtpu_info->mem_flag)
	{
	    EGTPU_FREE_MSG_BUF(p_egtpu_info->p_src_addr);
    }
    EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);
    EGTPU_FREE_MSG_BUF (p_msg);
    EGTPU_TRACE_FN_EXIT (p_global,"__enc_send_to_cp__");
    return ret_val;
}

/********************************************************************************
 * Function Name  : __get_peer_ctxt__
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  p_addr - Pointer to Peer's address
 * Outputs        : None
 * Returns        : p_ctxt - pointer of structure egtpu_peer_ctxt_t
 * Description    : This function obtains a peer ctxt record given a peer
 *                  transport address.
 ********************************************************************************/
/* imp: ref_cntr, tmr_flag has to be taken care of later */
STATIC egtpu_peer_ctxt_t *__get_peer_ctxt__ (egtpu_global_t *p_global,
					U8bit *p_addr)
{
    /*+SPR #3456*/
    UInt8  peer_count = p_global->initdb_info.num_active_peer_rec;
    /*-SPR #3456*/
    egtpu_peer_ctxt_t *p_ctxt = egtpu_db_sel_addr_key_peer_ctxt (p_addr);

    /* Spr 15913 Fix Start */
    if (!p_ctxt && (peer_count  < EGTPU_MAX_PEER_CTXT) && (p_ctxt = egtpu_db_get_peer_ctxt (p_global)))
	{
    /* Spr 15913 Fix End */
        /* Coverity 73583 Fix Start*/
        if((EGTPU_GET_U16BIT (p_addr) + EGTPU_SIZE_OF_LENGTH) > EGTPU_MAX_IP6_ADDR_SIZE)
        {
            return PNULL;
        }
        /* Coverity 73583 Fix End*/

        egtpu_memcpy (p_ctxt->entity_addr, p_addr,
                EGTPU_GET_U16BIT (p_addr) + EGTPU_SIZE_OF_LENGTH);
        /*+SPR #3456*/
        /*Copy the address of peer context record in the global peer ctxt address table*/
        egtpu_memcpy(&p_global->initdb_info.peer_ctxt_addr[peer_count],&p_ctxt,sizeof(UInt32*));
        p_global->initdb_info.num_active_peer_rec++;
        /*-SPR #3456*/
        /* SPR 18822 Fix Start */
        /* Intialise the supported Extension Header of Peer to 0x03
         * i.e PDCP SN and UDP port Extension supported value */
        p_ctxt->ext_hdr = (( EGTPU_EXT_HDR_PDCP ) | ( EGTPU_EXT_HDR_UDP )) ;
        /* SPR 18822 Fix End */
        p_ctxt->ref_cntr = 0;

        p_ctxt->path_struct.tmr_flag = EGTPU_TIMER_OFF;
        p_ctxt->path_struct.path_cntr = 0;
    }

    return p_ctxt;
}

/********************************************************************************
 * Function Name  : __incr_ref_cntr__
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  p_ctxt - Pointer to egtpu_peer_ctxt_t,
 *                  path - is a flag that determines if echo mechanism has,
 *                         to be done
 * Outputs        : None
 * Returns        : None
 * Description    : This function is a wrapper to increment ref_cntr.
 ********************************************************************************/
STATIC egtpu_void_t __incr_ref_cntr__ (egtpu_global_t *p_global,
				egtpu_peer_ctxt_t *p_ctxt, U8bit path)
{
    if (path && (p_ctxt->path_struct.tmr_flag == EGTPU_TIMER_OFF))
	{
        egtpu_return_t ret_val;
       /*+SPR #3456*/
        egtpu_send_echo_req (p_global,p_ctxt->entity_addr);
        EGTPU_START_ECHO_TIMER(p_global,p_ctxt, &p_ctxt->path_struct,ret_val);
  
        /* + coverity 32078 */
        if (EGTPU_FAILURE == ret_val)
        {
         LOG_GTPU_MSG(GTPU_ECHO_TIMER_START_FAILURE, LOGWARNING, PR_GTPU_DL_DATA,
                 relayGtpuCurrentTick_g, __LINE__,
                 0,0,0,0, 0,0, __func__, "");
        }
        /* - coverity 32078 */

        p_ctxt->path_struct.path_status = EGTPU_PATH_STATUS_UNKNOWN;
       /*-SPR #3456*/
    }

    p_ctxt->ref_cntr++;
}

/********************************************************************************
 * Function Name  : __decr_ref_cntr__
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  p_ctxt - Pointer to egtpu_peer_ctxt_t
 * Outputs        : None
 * Returns        : None
 * Description    : This function is a wrapper to decrement ref_cntr.
 ********************************************************************************/
STATIC egtpu_void_t __decr_ref_cntr__ (egtpu_global_t *p_global,
				egtpu_peer_ctxt_t *p_ctxt)
{
	U8bit alarm_flag = EGTPU_SEND_ALARM_ONLY ;   /* SPR 13765 fix */
    /* checking ref_cntr equalto zero is a redundant step */
    /* rather, can be considered as a safe step */
    if (!p_ctxt->ref_cntr || !(--p_ctxt->ref_cntr))
	{
        if (p_ctxt->path_struct.tmr_flag == EGTPU_TIMER_ON)
	{
        /*+SPR #3456*/
            EGTPU_STOP_ECHO_TIMER (p_global,&p_ctxt->path_struct);
        /*-SPR #3456*/
	}
		 /*+SPR #3456*/
		/* SPR 13765 fix start */	
		if(p_ctxt->is_alarm_raised)
		{
			__send_path_success_ind_to_cp__ (p_global,p_ctxt->entity_addr,alarm_flag); 
		}
 		/* SPR 13765 fix END */
        egtpu_free_peer_ctxt_addr_tbl(p_ctxt,p_global);
        /*-SPR #3456*/
        egtpu_db_free_peer_ctxt (p_ctxt, p_global);
    }
}

/********************************************************************************
 * Function Name  : release_rab_ctxt
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  p_rab_ctxt - Pointer to egtpu_rab_ctxt_t
 * Outputs        : None
 * Returns        : None
 * Description    : This function does the clean up of a RAB ctxt.
 ********************************************************************************/
STATIC egtpu_void_t release_rab_ctxt (egtpu_global_t *p_global,
				egtpu_rab_ctxt_t *p_rab_ctxt)
{
    if (p_rab_ctxt->p_transport_addr)
    {
        /* remove the entry from teid-peer map */
        egtpu_db_del_teid_peer_key_peer_map
            (p_rab_ctxt->p_transport_addr, p_rab_ctxt);

        __decr_ref_cntr__ (p_global,p_rab_ctxt->p_transport_addr);
    }
    /* LIPA changes start */
    /*
     *  Releasing the secondary transport addr for secondary tunnel
    */
    if(p_rab_ctxt->p_secondary_transport_addr)
    {
        __decr_ref_cntr__ (p_global, p_rab_ctxt->p_secondary_transport_addr);
    }
    /* LIPA changes end */

    if (p_rab_ctxt->p_forward_addr)
        __decr_ref_cntr__ (p_global, p_rab_ctxt->p_forward_addr);

    /* If we are running the re-ordering timer then stop it */
    if( p_rab_ctxt->p_tid != EGTPU_INVALID_TIMER_ID )
    {
        /*EGTPU_STOP_REORDER_TIMER(p_global, p_rab_ctxt);*/
        p_rab_ctxt->p_tid=EGTPU_INVALID_TIMER_ID ;
    }

    /* If timer buffer has been allocated then then free it */
    if( p_rab_ctxt->p_buf != EGTPU_NULL)
    {
        /*EGTPU_FREE_TIMER_BLOCK(p_rab_ctxt->p_buf);*/
        p_rab_ctxt->p_buf =EGTPU_NULL;
    }

    /* Deallocate the memory for hash table*/
    if( p_rab_ctxt->H != EGTPU_NULL)
    {
        EGTPU_FREE_SAP_HASH_REC(p_rab_ctxt->H);
	p_rab_ctxt->H=EGTPU_NULL;
    }

    egtpu_db_free_rab_ctxt (p_rab_ctxt);
}

/* error response preparation is done here */
/* used when RAB establishment has failed */
#define __ERR_RSP_1__(info_v, cause_v)                                 \
/* Cyclomatic Complexity changes */                                    \
    {                                                               \
        (info_v)->ie_presence = EGTPU_IE_CAUSE_FLAG |                   \
            EGTPU_IE_TRANSACTION_ID_FLAG;                               \
                                                                       \
        (info_v)->cause = cause_v;                                     \
    } 

/********************************************************************************
 * Function Name  : egtpu_init_reorder_timer_buf
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  p_rab_ctxt - Pointer to egtpu_rab_ctxt_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS
 * Description    : This function initializes the Reorder Timer.
 ********************************************************************************/
egtpu_return_t egtpu_init_reorder_timer_buf(egtpu_global_t *p_global,
					egtpu_rab_ctxt_t *p_rab_ctxt)
{
    EGTPU_GET_TIMER_BLOCK(p_global, p_global->tpool, p_rab_ctxt->p_buf);
		
    EGTPU_SET_API_HDR (p_rab_ctxt->p_buf, EGTPU_EI_TS, EGTPU_EI_GTPU,
            EGTPU_REORDER_TIMER_EXPIRY, (EGTPU_API_HDR_LEN + sizeof(U32bit)),
            EGTPU_NO_PAYLOAD, EGTPU_NO_PAYLOAD, (EGTPU_API_HDR_LEN + sizeof(U32bit) - 1));

/* SPR 20430 Changes Start*/
#ifdef __x86_64__	
    EGTPU_SET_U64BIT (p_rab_ctxt->p_buf + EGTPU_API_HDR_LEN, (U64bit)p_rab_ctxt);
#else
/* SPR 20430 Changes End*/
	EGTPU_SET_U32BIT (p_rab_ctxt->p_buf + EGTPU_API_HDR_LEN, (U32bit)p_rab_ctxt);
/* SPR 20430 Changes Start*/
#endif
/* SPR 20430 Changes End*/

	return EGTPU_SUCCESS;
}

/********************************************************************************
 * Function Name  : __create_sap_req__
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 * Outputs        : None
 * Returns        : None
 * Description    : In this function, when RAB is being setup for the first time,
 *                  transport-addr, if present, is assumed to be SGW address.
 ********************************************************************************/
egtpu_void_t __create_sap_req__(egtpu_global_t *p_global,
				egtpu_info_t *p_egtpu_info)
{
    egtpu_rab_ctxt_t *p_rab_ctxt = NULL;
    /* SPR 10787 fix start */
    egtpu_peer_teid_cell_ctxt_t   teid_cell;
    /* SPR 10787 fix end */
    U32bit 			ip_traffic;
      
    if (__FWD_SAP_SET__ (p_egtpu_info->sap_flag))
    {
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_CREATE_SAP_REQ_WITH_FWD_SAP, LOGWARNING, 
             PR_GTPU_CONTROL,
             relayGtpuCurrentTick_g, __LINE__, 
             0,0,0,0, 0,0, __func__,"");
        /* SPR 2030 fix End*/
        /* invalid SAP cfg is being attempted, no traces... */
        __ERR_RSP_1__ (p_egtpu_info, EGTPU_INV_SAP_CFG);
        return;
    }

    if (!(p_rab_ctxt = egtpu_db_get_rab_ctxt (p_global)))
    {
        /* ctxt acquisition fails... err has been reported in cdb */
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_CREATE_SAP_REQ_RSRC_NOT_AVAILABLE, LOGWARNING, 
             PR_GTPU_CONTROL,
             relayGtpuCurrentTick_g, __LINE__, 
             0,0,0,0, 0,0, __func__,"");
        /* SPR 2030 fix End*/
        __ERR_RSP_1__ (p_egtpu_info, EGTPU_RESOURCES_NOT_AVAILABLE);
        return;
    }
    
    p_rab_ctxt->sap_flag = p_egtpu_info->sap_flag;
    ip_traffic = p_egtpu_info->ip_traffic;
    p_rab_ctxt->ip_traffic = ip_traffic;
    p_rab_ctxt->transaction_id = p_egtpu_info->transaction_id;

   
    if (p_egtpu_info->ie_presence & EGTPU_IE_SEQ_FLAG)
        p_rab_ctxt->seq_disable_flag = p_egtpu_info->seq_disable;
    else
       p_rab_ctxt->seq_disable_flag = 1;  /*Disabled*/

    if (p_egtpu_info->ie_presence & EGTPU_IE_SEQ_NUM_UPLK_FLAG)
    {
        /* GPRS to UMTS handover procedure scenario */
        /* or can be re-establishment of RAB too */
        p_rab_ctxt->seq_num_uplk = p_egtpu_info->seq_num_uplk;
        p_rab_ctxt->seq_num_dnlk = p_egtpu_info->seq_num_dnlk;
    }
    else
    {
      	p_rab_ctxt->seq_num_uplk = p_rab_ctxt->seq_num_dnlk = 
				EGTPU_MIN_SEQ_NUM;
        p_egtpu_info->seq_num_uplk = p_egtpu_info->seq_num_dnlk = 
				EGTPU_MIN_SEQ_NUM;
    }

    /* Cyclomatic Complexity changes - starts here */
    if (EGTPU_SUCCESS != egtpu_fill_create_sap_src_addr_info (p_global, 
                p_egtpu_info, p_rab_ctxt))
    {
        return;
    }
    /* Cyclomatic Complexity changes - ends here */

    /* transport-addr is conditional to teid-peer */
    if (p_egtpu_info->ie_presence & EGTPU_IE_TEID_PEER_FLAG)
    {
        /* Coverity 55020 Fix Start */
	    if ( (PNULL == p_egtpu_info->p_peer_addr) || !(p_rab_ctxt->p_transport_addr =
                    __get_peer_ctxt__ (p_global, p_egtpu_info->p_peer_addr)))
	    {
        /* Coverity 55020 Fix End */
            /* ctxt acquisition fails... err has been reported in cdb */
            /* release the RAB ctxt acquired above */
            egtpu_db_free_rab_ctxt (p_rab_ctxt);

            __ERR_RSP_1__ (p_egtpu_info, EGTPU_RESOURCES_NOT_AVAILABLE);
            return;
        }

        /* search for teid-peer presence in peer map */
        /* SPR 10787 fix start */
        teid_cell.cellIndex = p_egtpu_info->cellIndex;
        teid_cell.teid_peer = p_egtpu_info->teid_peer;

	if (egtpu_db_sel_teid_peer_key_peer_map
			(p_rab_ctxt->p_transport_addr,
			 teid_cell))
		/* SPR 10787 fix end */
	{
		/* release the rab-ctxt that was acquired */
		/*SPR_20405_START*/
		/*Removed failure return for duplicate teid-peer in peer map*/
		LOG_GTPUPR_ERROR(PR_GTPU_CONTROL,"Duplicate TEID [%u] recieved ",
				p_egtpu_info->teid_peer);
		/*SPR_20405_END*/
	}

        /*+SPR #3456*/
        /*No ECHO REQ shall be generated in case of timer duration = 0*/
        if(p_global->tmr_info[0].tmr_dur == 0)
            __incr_ref_cntr__ (p_global,p_rab_ctxt->p_transport_addr, EGTPU_FALSE);

        else
        /*-SPR #3456*/

        __incr_ref_cntr__ (p_global,p_rab_ctxt->p_transport_addr, EGTPU_TRUE);

        /* SPR 10787 fix start */
        p_rab_ctxt->teid_cell.cellIndex = p_egtpu_info->cellIndex;
        p_rab_ctxt->teid_cell.teid_peer = p_egtpu_info->teid_peer;
        /* SPR 10787 fix end */
        egtpu_db_ins_teid_peer_key_peer_map
            (p_rab_ctxt->p_transport_addr, p_rab_ctxt);
    }
    else
        p_rab_ctxt->p_transport_addr = EGTPU_NULL;
	
   
    p_rab_ctxt->qos_id = p_egtpu_info->qos_id;

    if(p_egtpu_info->ie_presence & EGTPU_IE_REORDERING_FLAG) 
    {
    	/* Cyclomatic Complexity changes - starts here */
    	if (EGTPU_SUCCESS != egtpu_fill_create_sap_reordering_info (p_global, 
                p_egtpu_info, p_rab_ctxt))
    	{
    	    return;
    	}
    	/* Cyclomatic Complexity changes - ends here */
    }

    p_egtpu_info->ie_presence = EGTPU_IE_CAUSE_FLAG |
        EGTPU_IE_TRANSACTION_ID_FLAG | EGTPU_IE_TEID_SELF_FLAG |
        EGTPU_IE_SEQ_NUM_UPLK_FLAG | EGTPU_IE_SEQ_NUM_DNLK_FLAG |
        EGTPU_IE_SOURCE_TRANSPORT_ADDR_FLAG;

    p_egtpu_info->cause = EGTPU_SUCCESS;
    p_egtpu_info->teid_self = 
		egtpu_db_get_teid_key_rab_ctxt (p_global,p_rab_ctxt);
    

  
}

/* Cyclomatic Complexity changes - starts here */
/********************************************************************************
 * Function Name  : egtpu_fill_create_sap_src_addr_info
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t,
 *                  p_rab_ctxt - Pointer to egtpu_rab_ctxt_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This function fills the source transport address during
 *                  CREATE_SAP_REQUEST.
 ********************************************************************************/
STATIC inline egtpu_return_t egtpu_fill_create_sap_src_addr_info (
        egtpu_global_t    *p_global, 
        egtpu_info_t      *p_egtpu_info, 
        egtpu_rab_ctxt_t  *p_rab_ctxt)
{
    U8bit 			count=0;
    /* + Coverity_10294 */
    U16bit addr_len=0;   
    /* - Coverity_10294 */
    STATIC U8bit	index_ip4=0;
    STATIC U8bit	index_ip6=0;

    if (p_egtpu_info->ie_presence & EGTPU_IE_SOURCE_TRANSPORT_ADDR_FLAG)
    {
        /* If Control Plane entity has sent source/own address,
         *  but this address is not configured during initialization,
         *  then raise error, release rab ctxt and exit
         */
        /* Coverity 40837 Fix Start*/
        if(p_egtpu_info->p_src_addr == EGTPU_NULL)
        {
            egtpu_db_free_rab_ctxt (p_rab_ctxt);
            LOG_GTPU_MSG(GTPU_SRC_IP_NOT_CONFIGURED, LOGWARNING,
                    PR_GTPU_CONTROL,
                    relayGtpuCurrentTick_g, __LINE__,
                    0,0,0,0, 0,0, __func__,"");
            return EGTPU_FAILURE;
        }
        /* Coverity 40837 Fix End*/

        addr_len = EGTPU_GET_U16BIT(p_egtpu_info->p_src_addr);
        /* Coverity 73447 Fix Start*/
        if(addr_len > EGTPU_MAX_IP6_ADDR_SIZE)
        {
            egtpu_db_free_rab_ctxt (p_rab_ctxt);
            LOG_GTPU_MSG(GTPU_SRC_IP_NOT_CONFIGURED, LOGWARNING,
                    PR_GTPU_CONTROL,
                    relayGtpuCurrentTick_g, __LINE__,
                    0,0,0,0, 0,0, __func__,"");
            return EGTPU_FAILURE;
        }
        /* Coverity 73447 Fix End*/

        egtpu_memcpy(p_rab_ctxt->src_addr, p_egtpu_info->p_src_addr + EGTPU_SIZE_OF_LENGTH,
                addr_len);

        if(addr_len == EGTPU_SIZE_OF_IPV4_ADDR)
        {
            for(count=0; count<p_global->initdb_info.num_my_addr4; count++)
            {
                if(egtpu_memcmp((S8bit *)p_rab_ctxt->src_addr,
                            (S8bit *)p_global->initdb_info.my_addr[count],
                            EGTPU_SIZE_OF_IPV4_ADDR) == 0)
                {
                    /** Found a matching source address **/
                    break;
                }
            }

            if(count >= p_global->initdb_info.num_my_addr4)
            {
                /* release the RAB ctxt acquired above */
                egtpu_db_free_rab_ctxt (p_rab_ctxt);
                /* SPR 2030 fix Start*/
                LOG_GTPU_MSG(GTPU_SRC_IP_NOT_CONFIGURED, LOGWARNING, 
                        PR_GTPU_CONTROL,
                        relayGtpuCurrentTick_g, __LINE__, 
                        0,0,0,0, 0,0, __func__,"");
                /* SPR 2030 fix End*/

                __ERR_RSP_1__ (p_egtpu_info, EGTPU_INV_SRC_ADDR_RECEIVED);
                return EGTPU_FAILURE;
            }
        }
        else
        {
            for(count=0; count<p_global->initdb_info.num_my_addr6; count++)
            {
                if(egtpu_memcmp((S8bit *)p_rab_ctxt->src_addr,
                            (S8bit *)p_global->initdb_info.my_addr6[count],
                            EGTPU_SIZE_OF_IPV6_ADDR) == 0)

                {
                    /** Found a matching source address **/
                    break;
                }
            }

            if(count >= p_global->initdb_info.num_my_addr6)
            {
                /* release the RAB ctxt acquired above */
                egtpu_db_free_rab_ctxt (p_rab_ctxt);
                /* SPR 2030 fix Start*/
                LOG_GTPU_MSG(GTPU_SRC_IP_NOT_CONFIGURED, LOGWARNING, 
                        PR_GTPU_CONTROL,
                        relayGtpuCurrentTick_g, __LINE__, 
                        0,0,0,0, 0,0, __func__,"");
                /* SPR 2030 fix End*/

                __ERR_RSP_1__ (p_egtpu_info, EGTPU_INV_SRC_ADDR_RECEIVED);
                return EGTPU_FAILURE;
            }
        }
    }
    else
    {
        /* Choose any one of the source address from the configured ones */
        /*Coverity Id 55020 Fix Start*/
        if((p_egtpu_info->ie_presence & EGTPU_IE_TEID_PEER_FLAG) &&
                (p_egtpu_info->ie_presence & EGTPU_IE_TRANSPORT_ADDR_FLAG))
        {
            if(EGTPU_NULL == p_egtpu_info->p_peer_addr)
            {
                LOG_GTPU_MSG(GTPU_PEER_NOT_PRESENT, LOGWARNING, 
                        PR_GTPU_CONTROL,
                        relayGtpuCurrentTick_g, __LINE__, 
                        0,0,0,0, 0,0, __func__,"");
                return EGTPU_FAILURE;
            }
            /*Coverity Id 55020 Fix End*/
            addr_len = EGTPU_GET_U16BIT(p_egtpu_info->p_peer_addr);

            if (addr_len==EGTPU_SIZE_OF_IPV4_ADDR)
            {
                if(p_global->initdb_info.num_my_addr4 == 0)
                {			
                    /* src_addr list is empty, so cannot choose the src_addr */
                    /* release the RAB ctxt acquired above */
                    egtpu_db_free_rab_ctxt (p_rab_ctxt);
                    /* SPR 2030 fix Start*/
                    LOG_GTPU_MSG(GTPU_SRC_IP_NOT_PRESENT, LOGWARNING, 
                            PR_GTPU_CONTROL,
                            relayGtpuCurrentTick_g, __LINE__, 
                            0,0,0,0, 0,0, __func__,"");
                    /* SPR 2030 fix End*/

                    __ERR_RSP_1__ (p_egtpu_info, EGTPU_INV_SRC_ADDR_RECEIVED);
                    return EGTPU_FAILURE;
                }
                else
                {
                    egtpu_memcpy(p_rab_ctxt->src_addr,\
                            (S8bit *)p_global->initdb_info.my_addr[index_ip4],
                            EGTPU_SIZE_OF_IPV4_ADDR);
                    index_ip4++;
                }
            }			
            else 
            {
                if( p_global->initdb_info.num_my_addr6 == 0)
                {			
                    /* src_addr list is empty, so cannot choose the src_addr */
                    /* release the RAB ctxt acquired above */
                    egtpu_db_free_rab_ctxt (p_rab_ctxt);
                    /* SPR 2030 fix Start*/
                    LOG_GTPU_MSG(GTPU_SRC_IP_NOT_PRESENT, LOGWARNING, 
                            PR_GTPU_CONTROL,
                            relayGtpuCurrentTick_g, __LINE__, 
                            0,0,0,0, 0,0, __func__,"");
                    /* SPR 2030 fix End*/

                    __ERR_RSP_1__ (p_egtpu_info, EGTPU_INV_SRC_ADDR_RECEIVED);
                    return EGTPU_FAILURE;
                }
                else
                {
                    egtpu_memcpy(p_rab_ctxt->src_addr,\
                            (S8bit *)p_global->initdb_info.my_addr6[index_ip6],
                            EGTPU_SIZE_OF_IPV6_ADDR);
                    index_ip6++;
                }
            }

            if(index_ip4 >= p_global->initdb_info.num_my_addr4)
            {
                index_ip4 = 0;

            }
            if(index_ip6 >= p_global->initdb_info.num_my_addr6)
            {
                index_ip6 = 0;

            }

            /*	returning source address to CP */
            EGTPU_GET_MSG_BUF(p_global,U8bit,p_egtpu_info->p_src_addr ,	\
                    EGTPU_MAX_IP6_ADDR_SIZE);	
            /*coverity-530 CID-42170*/                                      
            if(EGTPU_NULL == p_egtpu_info->p_src_addr)
            {
                egtpu_db_free_rab_ctxt (p_rab_ctxt);
                return EGTPU_FAILURE; 
            }
            /*coverity-530 CID-42170*/                                      
            egtpu_bzero(p_egtpu_info->p_src_addr, EGTPU_MAX_IP6_ADDR_SIZE);
            EGTPU_SET_U16BIT(p_egtpu_info->p_src_addr, addr_len);
            /* Coverity 73447 Fix Start*/
            if(addr_len > EGTPU_MAX_IP6_ADDR_SIZE)
            {
                egtpu_db_free_rab_ctxt (p_rab_ctxt);
                LOG_GTPU_MSG(GTPU_SRC_IP_NOT_CONFIGURED, LOGWARNING,
                        PR_GTPU_CONTROL,
                        relayGtpuCurrentTick_g, __LINE__,
                        0,0,0,0, 0,0, __func__,"");
                return EGTPU_FAILURE;
            }
            /* Coverity 73447 Fix End*/

            egtpu_memcpy(p_egtpu_info->p_src_addr + EGTPU_SIZE_OF_LENGTH,\
                    p_rab_ctxt->src_addr, addr_len);
            p_egtpu_info->mem_flag = EGTPU_TRUE;

        }
#ifdef ENDC_ENABLED
        /*since the lower nibble of the SAP flag is sent to egtpu, hence
         * for X2U with sap flag 0x90, lower nibble is 0*/
        else if( S1U_SN_TERMINTED_SPLIT_BEARER_SAP_FLAG == p_egtpu_info->sap_flag)
        {
            if(p_global->initdb_info.num_my_addr4 != 0)                        
            {                                                                  
                egtpu_memcpy(p_rab_ctxt->src_addr,\
                        (S8bit *)p_global->initdb_info.my_addr[index_ip4],     
                        EGTPU_SIZE_OF_IPV4_ADDR);                              
                index_ip4++;                                                   
            }                                                                  
            else if( p_global->initdb_info.num_my_addr6 != 0)                          
            {                                                                     
                egtpu_memcpy(p_rab_ctxt->src_addr,\
                        (S8bit *)p_global->initdb_info.my_addr6[index_ip6],       
                        EGTPU_SIZE_OF_IPV6_ADDR);                                 
                index_ip6++;                                                      
            }
            else
            {
                /* src_addr list is empty, so cannot choose the src_addr */    
                /* release the RAB ctxt acquired above */                      
                egtpu_db_free_rab_ctxt (p_rab_ctxt);                           
                /* SPR 2030 fix Start*/                                        
                LOG_GTPU_MSG(GTPU_SRC_IP_NOT_PRESENT, LOGWARNING,              
                        PR_GTPU_CONTROL,                                       
                        relayGtpuCurrentTick_g, __LINE__,                      
                        0,0,0,0, 0,0, __func__,"");                            
                /* SPR 2030 fix End*/                                          

                __ERR_RSP_1__ (p_egtpu_info, EGTPU_INV_SRC_ADDR_RECEIVED);     
                return EGTPU_FAILURE;                                          
            }

            if(index_ip4 >= p_global->initdb_info.num_my_addr4)                   
            {                                                                     
                index_ip4 = 0;                                                    

            }                                                                     
            if(index_ip6 >= p_global->initdb_info.num_my_addr6)                   
            {                                                                     
                index_ip6 = 0;                                                    

            }                                                                     

            /*  returning source address to CP */                                 
            EGTPU_GET_MSG_BUF(p_global,U8bit,p_egtpu_info->p_src_addr , \
                    EGTPU_MAX_IP6_ADDR_SIZE);                                     
            /*coverity-530 CID-42170*/                                            
            if(EGTPU_NULL == p_egtpu_info->p_src_addr)                            
            {                                                                     
                egtpu_db_free_rab_ctxt (p_rab_ctxt);                               
                return EGTPU_FAILURE;                                              
            }                                                                     
            /*coverity-530 CID-42170*/                                            
            egtpu_bzero(p_egtpu_info->p_src_addr, EGTPU_MAX_IP6_ADDR_SIZE);       
            EGTPU_SET_U16BIT(p_egtpu_info->p_src_addr, addr_len);                 
            /* Coverity 73447 Fix Start*/                                         
            if(addr_len > EGTPU_MAX_IP6_ADDR_SIZE)                                
            {                                                                     
                egtpu_db_free_rab_ctxt (p_rab_ctxt);                              
                LOG_GTPU_MSG(GTPU_SRC_IP_NOT_CONFIGURED, LOGWARNING,              
                        PR_GTPU_CONTROL,                                          
                        relayGtpuCurrentTick_g, __LINE__,                         
                        0,0,0,0, 0,0, __func__,"");                               
                return EGTPU_FAILURE;                                             
            }                                                                     
            /* Coverity 73447 Fix End*/                                           

            egtpu_memcpy(p_egtpu_info->p_src_addr + EGTPU_SIZE_OF_LENGTH,\
                    p_rab_ctxt->src_addr, addr_len);                              
            p_egtpu_info->mem_flag = EGTPU_TRUE;                                  
        }
#endif
        else
        {
            /* No Dest-Addr or Peer-Addr is present, so cannot choose the src_addr */
            /* release the RAB ctxt acquired above */
            egtpu_db_free_rab_ctxt (p_rab_ctxt);
            /* SPR 2030 fix Start*/
            LOG_GTPU_MSG(GTPU_PEER_NOT_PRESENT, LOGWARNING, 
                    PR_GTPU_CONTROL,
                    relayGtpuCurrentTick_g, __LINE__, 
                    0,0,0,0, 0,0, __func__,"");
            /* SPR 2030 fix End*/
            /*SPR 21767 Start*/
            __ERR_RSP_1__ (p_egtpu_info, EGTPU_PEER_ADDRESS_MISSING);
            /*SPR 21767 End*/
            return EGTPU_FAILURE;
        }
    }

    return EGTPU_SUCCESS;
}

/********************************************************************************
 * Function Name  : egtpu_fill_create_sap_reordering_info
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t,
 *                  p_rab_ctxt - Pointer to egtpu_rab_ctxt_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This function fills the reordering information during
 *                  CREATE_SAP_REQUEST.
 ********************************************************************************/
STATIC inline egtpu_return_t egtpu_fill_create_sap_reordering_info (
        egtpu_global_t    *p_global, 
        egtpu_info_t      *p_egtpu_info, 
        egtpu_rab_ctxt_t  *p_rab_ctxt)
{
	U32bit 			ip_traffic = p_rab_ctxt->ip_traffic;
    p_rab_ctxt->reordering_reqd = p_egtpu_info->reordering_reqd;
    /* the default values pertaining to the ip_class which is parsed from 
     * the setup_sap_req_parse are placed in the 
     * p_egtpu_info->max_window_size and 
     * p_egtpu_info->max_window_size parameters*/
    if(p_egtpu_info->reordering_reqd)
    {
        EGTPU_GET_SAP_HASH_REC(p_global, p_global->hash_pool, p_rab_ctxt->H); 

        /* Allocate memory for the hash table and initialize it */
        /*Klockworks Warning Fix Start*/
        if(egtpu_init_hash(p_global, p_rab_ctxt->H) == EGTPU_SUCCESS)
        {
            /*This function will always return EGTPU_SUCCESS */
        }
        /*Klockworks Warning Fix End*/
        p_rab_ctxt->max_window_size =  
            p_global->max_window_size[ip_traffic];
        p_rab_ctxt->reorder_timer_drtn = 
            p_global->reorder_timer_drtn[ip_traffic];
        /* Allocate memory for the reorder buffer */
        egtpu_init_reorder_timer_buf(p_global,p_rab_ctxt);

    }   
    return EGTPU_SUCCESS;
}

/* Cyclomatic Complexity changes - ends here */
            
/********************************************************************************
 * Function Name  : __fwd_addr_upd__
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  new_teid - New Tunnel Endpoint ID,
 *                  p_new_peer - New peer address,
 *                  p_ctxt - Pointer to structure egtpu_rab_ctxt_t
 * Outputs        : None
 * Returns        : None
 * Description    : This function updates the new address for forwarding the data.
 ********************************************************************************/
/* new configuration has to be matched against the old one */
STATIC egtpu_void_t __fwd_addr_upd__ (egtpu_global_t *p_global,
				egtpu_teid_t new_teid,
				egtpu_peer_ctxt_t *p_new_peer,
				egtpu_rab_ctxt_t *p_ctxt)
{
    if (p_ctxt->p_forward_addr != p_new_peer)
	{
        if (p_ctxt->p_forward_addr)
            __decr_ref_cntr__ (p_global,p_ctxt->p_forward_addr);

        __incr_ref_cntr__ (p_global,p_new_peer, EGTPU_FALSE);
        p_ctxt->p_forward_addr = p_new_peer;
    }

    p_ctxt->teid_peer_fwd = new_teid;
}

/********************************************************************************
 * Function Name  : __tranport_addr_upd__
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  new_teid - New Tunnel Endpoint ID,
 *                  p_new_peer - New peer address,
 *                  p_ctxt - Pointer to structure egtpu_rab_ctxt_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This function updates the new peer address.
 ********************************************************************************/
STATIC egtpu_return_t __tranport_addr_upd__ (egtpu_global_t *p_global,
					egtpu_teid_t new_teid,
					egtpu_peer_ctxt_t *p_new_peer,
					egtpu_rab_ctxt_t *p_ctxt)
{
    egtpu_peer_ctxt_t *p_old_peer = p_ctxt->p_transport_addr;

    /* SPR 10787 fix start */
    egtpu_peer_teid_cell_ctxt_t teid_cell;
    teid_cell.cellIndex = p_ctxt->teid_cell.cellIndex;
    teid_cell.teid_peer = new_teid;

    if ((p_old_peer != p_new_peer) || (p_ctxt->teid_cell.teid_peer != new_teid))
	{
        if (egtpu_db_sel_teid_peer_key_peer_map (p_new_peer, teid_cell))
		/* SPR 10787 fix end */
		/*SPR_20405_START*/
	{
		LOG_GTPUPR_ERROR(PR_GTPU_CONTROL,"Duplicate TEID [%u] recieved ",
				new_teid);
	}
	/*SPR_20405_END*/

        if (p_old_peer)
            egtpu_db_del_teid_peer_key_peer_map (p_old_peer, p_ctxt);

        /* SPR 10787 fix start */
        p_ctxt->teid_cell.teid_peer = new_teid;
        /* SPR 10787 fix end */
        egtpu_db_ins_teid_peer_key_peer_map (p_new_peer, p_ctxt);

        if (p_old_peer != p_new_peer)
		{
            if (p_old_peer)
                __decr_ref_cntr__ (p_global,p_old_peer);

            /*+SPR #3456*/
            /*No ECHO REQ shall be generated in case of timer duration = 0*/
           if(p_global->tmr_info[0].tmr_dur == 0)
            __incr_ref_cntr__ (p_global,p_new_peer, EGTPU_FALSE);
           else
            /*-SPR #3456*/

            __incr_ref_cntr__ (p_global,p_new_peer, EGTPU_TRUE);
            p_ctxt->p_transport_addr = p_new_peer;
        }
    }

    return EGTPU_SUCCESS;
}

/* error response preparation is done here */
/* used when RAB modification has failed */
#define __ERR_RSP_2__(info_v, cause_v, xid)                            \
/* Cyclomatic Complexity changes */                                    \
    {                                                               \
        (info_v)->ie_presence = EGTPU_IE_CAUSE_FLAG |                   \
            EGTPU_IE_TRANSACTION_ID_FLAG;                               \
                                                                       \
        (info_v)->cause = cause_v;                                     \
        (info_v)->transaction_id = xid;                                \
    }

/********************************************************************************
 * Function Name  : __modify_sap_req__
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 * Outputs        : None
 * Returns        : None
 * Description    : In this function RAB ctxt is being modified.
 ********************************************************************************/
egtpu_void_t __modify_sap_req__ (egtpu_global_t *p_global,
				egtpu_info_t *p_egtpu_info)
{
    egtpu_peer_ctxt_t *p_new_peer = EGTPU_NULL;
    egtpu_rab_ctxt_t  *p_rab_ctxt = EGTPU_NULL;
    
    if (!(p_rab_ctxt = egtpu_db_sel_teid_key_rab_ctxt
                (p_global,p_egtpu_info->teid_self)))
	{
        /* modification of a non-existent RAB is being attempted */
        p_egtpu_info->ie_presence = EGTPU_IE_CAUSE_FLAG |
            EGTPU_IE_TEID_SELF_FLAG;

        p_egtpu_info->cause = EGTPU_CTXT_NOT_FOUND;
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_MOD_SAP_REQ_FOR_NONEXIST_RAB, LOGWARNING, PR_GTPU_CONTROL,
                relayGtpuCurrentTick_g, __LINE__, 
                (p_global,p_egtpu_info->teid_self),0,0,0, 0,0, __func__,"");
        /* SPR 2030 fix End*/
        return;
    }

    if (p_egtpu_info->ie_presence & EGTPU_IE_SOURCE_TRANSPORT_ADDR_FLAG)
	{
		/* Source Addr cannot be present in Modify SAP Req.
		   Raise error, release the rab ctxt and exit
		*/
		__ERR_RSP_2__ (p_egtpu_info, EGTPU_INV_SRC_ADDR_RECEIVED,
                    p_rab_ctxt->transaction_id);
            	
		/* rel RAB, rmv teid-peer entry, decr ref_cntr for peer */
        release_rab_ctxt (p_global,p_rab_ctxt);

        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_MOD_SAP_REQ_INV_SRC_ADD, LOGWARNING, PR_GTPU_CONTROL,
                relayGtpuCurrentTick_g, __LINE__, 
                0,0,0,0, 0,0, __func__,"");
        /* SPR 2030 fix End*/
		return;
	}

    if (p_egtpu_info->ie_presence & EGTPU_IE_SEQ_NUM_UPLK_FLAG)
	{
        /* relocation at target eNodeB */
        p_rab_ctxt->seq_num_uplk = p_egtpu_info->seq_num_uplk;
        p_rab_ctxt->seq_num_dnlk = p_egtpu_info->seq_num_dnlk;
    }
	else
	{
        p_egtpu_info->seq_num_uplk = p_rab_ctxt->seq_num_uplk;
        p_egtpu_info->seq_num_dnlk = p_rab_ctxt->seq_num_dnlk;
    }

    if (p_egtpu_info->ie_presence & EGTPU_IE_SEQ_FLAG)
        p_rab_ctxt->seq_disable_flag = p_egtpu_info->seq_disable;
    else
       p_rab_ctxt->seq_disable_flag = 1;  /*Disabled*/

    /* relocation cancel procedure scenario, cleanup p_forward_addr */
    /* Coverity 54565 */    
    if (__FWD_SAP_SET__ (p_rab_ctxt->sap_flag) &&
            !__FWD_SAP_SET__ (p_egtpu_info->sap_flag))
    {
        if (p_rab_ctxt->p_forward_addr)
		{
            __decr_ref_cntr__ (p_global,p_rab_ctxt->p_forward_addr);
            p_rab_ctxt->p_forward_addr = EGTPU_NULL;
        }
    }
    /* Coverity 54565 */    
    if (p_egtpu_info->ie_presence & EGTPU_IE_TEID_PEER_FLAG)
	{
        /* Coverity 55020 Fix Start */
        if ( (PNULL == p_egtpu_info->p_peer_addr) || !(p_new_peer =
                    __get_peer_ctxt__ (p_global,p_egtpu_info->p_peer_addr)))
        /* Coverity 55020 Fix End */
		{
            /* no resource available to create peer record */
            __ERR_RSP_2__ (p_egtpu_info, EGTPU_RESOURCES_NOT_AVAILABLE,
                    p_rab_ctxt->transaction_id);

            /* rel RAB, rmv teid-peer entry, decr ref_cntr for peer */
            release_rab_ctxt (p_global,p_rab_ctxt);
            return;
        }

        if (__FWD_SAP_SET__ (p_egtpu_info->sap_flag))
            __fwd_addr_upd__ (p_global,p_egtpu_info->teid_peer,
                    p_new_peer, p_rab_ctxt);
        else if (__tranport_addr_upd__ (p_global,p_egtpu_info->teid_peer,
                    p_new_peer, p_rab_ctxt))
		{
        /* search for teid-peer presence in peer map */
        /* this is clearly a mis-behaviour by peer */
        /* two endpoints can not send data to one single endpoint */
        /* note that vice versa is possible - Y tunnelling */
        /* has to be tackled, otherwise dB will be screwed */
            __ERR_RSP_2__ (p_egtpu_info, EGTPU_DUPLICATE_PEER_TEID,
                    p_rab_ctxt->transaction_id);

            /* rel RAB, rmv teid-peer entry, decr ref_cntr for peer */
            release_rab_ctxt (p_global,p_rab_ctxt);
            return;
        }
    }

	if (p_egtpu_info->ie_presence & EGTPU_IE_REORDERING_FLAG) 
	{
        /* Cyclomatic Complexity changes - starts here */
        if (EGTPU_SUCCESS != egtpu_fill_modify_sap_reordering_info (p_global, 
                    p_egtpu_info, p_rab_ctxt))
        {
            return;
        }
        /* Cyclomatic Complexity changes - ends here */
    }
    p_rab_ctxt->sap_flag = p_egtpu_info->sap_flag;
    p_egtpu_info->ie_presence = EGTPU_IE_CAUSE_FLAG |
        EGTPU_IE_TRANSACTION_ID_FLAG |
        EGTPU_IE_SEQ_NUM_UPLK_FLAG | EGTPU_IE_SEQ_NUM_DNLK_FLAG;

    p_egtpu_info->cause = EGTPU_SUCCESS;
    p_egtpu_info->transaction_id = p_rab_ctxt->transaction_id;
}

/* Cyclomatic Complexity changes - starts here */
/********************************************************************************
 * Function Name  : egtpu_fill_modify_sap_reordering_info
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t,
 *                  p_rab_ctxt - Pointer to structure egtpu_rab_ctxt_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This function fills the Reordering information
 *                  during MODIFY_SAP_REQUEST.
 ********************************************************************************/
STATIC inline egtpu_return_t egtpu_fill_modify_sap_reordering_info (
        egtpu_global_t    *p_global, 
        egtpu_info_t      *p_egtpu_info, 
        egtpu_rab_ctxt_t  *p_rab_ctxt)
{
    U32bit 			  ip_traffic;

    ip_traffic = p_egtpu_info->ip_traffic;
    p_rab_ctxt->ip_traffic = ip_traffic;

    if(p_rab_ctxt->reordering_reqd)
    {
        p_rab_ctxt->max_window_size = 
            p_global->max_window_size[ip_traffic];
        p_rab_ctxt->reorder_timer_drtn = 
            p_global->reorder_timer_drtn[ip_traffic];

        p_rab_ctxt->reordering_reqd = p_egtpu_info->reordering_reqd;

        if(!p_rab_ctxt->reordering_reqd)
        {
            /* The user has decided that re-ordering is useless. 
             * Even though its hard to guess why but we respect his/her
             * decision and henceforth destroy all traces that re-ordering
             * was ever attempted
             */

            /* If we are running the re-ordering timer then stop it */
            if( p_rab_ctxt->p_tid != EGTPU_INVALID_TIMER_ID )
            {
                /*EGTPU_STOP_REORDER_TIMER(p_global, p_rab_ctxt);*/
            }

            /* If timer buffer has been allocated then then free it */
            if( p_rab_ctxt->p_buf != EGTPU_NULL)
            {
                /*EGTPU_FREE_TIMER_BLOCK(p_rab_ctxt->p_buf);*/
                p_rab_ctxt->p_buf = EGTPU_NULL; /* SPR FIX 7445 */
            }
            /* Deallocate the memory for hash table*/
            if( p_rab_ctxt->H != EGTPU_NULL)
            {
                EGTPU_FREE_SAP_HASH_REC(p_rab_ctxt->H);
                /* SPR 4047 Fix Start */
                p_rab_ctxt->H = EGTPU_NULL;    /* SPR FIX 7445 */
                /* SPR 4047 Fix End */
            }
        }
    }
    else
    {
        p_rab_ctxt->reordering_reqd = p_egtpu_info->reordering_reqd;

        /* the default values pertaining to the ip_class which is parsed 
         * from the setup_sap_req_parse are placed in the 
         * p_egtpu_info->max_window_size and 
         * p_egtpu_info->max_window_size parameters
         */
        if(p_egtpu_info->reordering_reqd)
        {
            p_rab_ctxt->max_window_size =  
                p_global->max_window_size[ip_traffic];
            p_rab_ctxt->reorder_timer_drtn = 
                p_global->reorder_timer_drtn[ip_traffic];
            /* Set the next seq number */
            p_rab_ctxt->nxt_seq = p_rab_ctxt->seq_num_dnlk + 1;

            EGTPU_GET_SAP_HASH_REC(p_global,
                    p_global->hash_pool, p_rab_ctxt->H); 

            /* Allocate memory for the reorder buffer */
            egtpu_init_reorder_timer_buf(p_global,p_rab_ctxt);
            
            /* Allocate memory for the hash table and initialize it */
            /*Klockworks Warning Fix Start*/
            if(egtpu_init_hash(p_global, p_rab_ctxt->H) == EGTPU_SUCCESS)
            {
                /*This function will always return EGTPU_SUCCESS */
            }
            /*Klockworks Warning Fix End*/
        }
    }   

    return EGTPU_SUCCESS;
}
/* Cyclomatic Complexity changes - ends here */

/********************************************************************************
 * Function Name  : egtpu_setup_sap_req
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  p_egtpu_info - Pointer to structure egtpu_info_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This function handles setup (create/modify) of SAP.
 ********************************************************************************/
egtpu_return_t egtpu_setup_sap_req (egtpu_global_t *p_global,
									egtpu_info_t *p_egtpu_info)

{

    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_SETUP_SAP_REQ);
		
    if (p_egtpu_info->ie_presence & EGTPU_IE_TEID_SELF_FLAG)
	{
        EGTPU_TRACE_FN_ENTR (p_global,"__modify_sap_req__");
        __modify_sap_req__ (p_global,p_egtpu_info);   /* teid-self */
        EGTPU_TRACE_FN_EXIT (p_global,"__modify_sap_req__");
    }
	else
	{
        EGTPU_TRACE_FN_ENTR (p_global,"__create_sap_req__");
         __create_sap_req__ (p_global,p_egtpu_info);   /* transaction-id */
        EGTPU_TRACE_FN_EXIT (p_global,"__create_sap_req__");
    }

    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_CP_SETUP_SAP_CNF);
    
	 return __enc_send_to_cp__ (p_global,p_egtpu_info, EGTPU_CP_SETUP_SAP_CNF);


}

     
/********************************************************************************
 * Function Name  : egtpu_release_sap_req
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  p_egtpu_info - Pointer to structure egtpu_info_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This function releases the resources of a SAP/RAB context.
 ********************************************************************************/
egtpu_return_t egtpu_release_sap_req (egtpu_global_t *p_global,
										egtpu_info_t *p_egtpu_info)
{
    egtpu_rab_ctxt_t *p_rab_ctxt;
    egtpu_teid_t     teid = EGTPU_GET_U32BIT (p_egtpu_info->p_buff +
            EGTPU_API_HDR_LEN + EGTPU_SIZE_OF_TYPE);
    
    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_RELEASE_SAP_REQ);

    if ((p_rab_ctxt = egtpu_db_sel_teid_key_rab_ctxt (p_global,teid)))
	{
        p_egtpu_info->ie_presence = EGTPU_IE_CAUSE_FLAG |
            EGTPU_IE_TRANSACTION_ID_FLAG |
            EGTPU_IE_SEQ_NUM_UPLK_FLAG | EGTPU_IE_SEQ_NUM_DNLK_FLAG;

        p_egtpu_info->cause = EGTPU_SUCCESS;
        p_egtpu_info->transaction_id = p_rab_ctxt->transaction_id;
        p_egtpu_info->seq_num_uplk = p_rab_ctxt->seq_num_uplk;
        p_egtpu_info->seq_num_dnlk  = p_rab_ctxt->seq_num_dnlk;

        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTP_RAB_CTXT_RELEASED, LOGINFO, PR_GTPU_CONTROL,
                relayGtpuCurrentTick_g, __LINE__, 
                teid,0,0,0,0,0, __func__,"");
        /* SPR 2030 fix End*/
        release_rab_ctxt (p_global, p_rab_ctxt);
    }
	else
	{
        /* release of a non-existent RAB is being attempted */
        /* cnf shall contain cause and teid-self */
        p_egtpu_info->ie_presence = EGTPU_IE_CAUSE_FLAG |
            EGTPU_IE_TEID_SELF_FLAG;

        p_egtpu_info->cause = EGTPU_CTXT_NOT_FOUND;
        p_egtpu_info->teid_self = teid;

        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTP_RELEASE_REQ_FOR_NONEXIST_RAB, LOGWARNING, PR_GTPU_CONTROL,
                relayGtpuCurrentTick_g, __LINE__, 
                teid,0,0,0, 0,0, __func__,"");
        /* SPR 2030 fix End*/

        /*SPR #2714 fix Start*/
        /*rab_ctxt for non-existent RAB can never be released*/
        /*release_rab_ctxt (p_global, p_rab_ctxt);*/
        /*SPR #2714 fix End*/
    }
    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_CP_RELEASE_SAP_CNF);

    return __enc_send_to_cp__(p_global,p_egtpu_info,EGTPU_CP_RELEASE_SAP_CNF);
}


/* LIPA changes start */
/********************************************************************************
 * Function Name  : handleGtpuSecondaryTunnelId
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  p_addr - Pointer to Peer's Address,
 *                  tunnelId - Tunnel Identifier
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This function gets or updates the secondary Transport Address
 *                  for Secondary Tunnel Id in same SAP/RAB context.
 ********************************************************************************/

egtpu_return_t handleGtpuSecondaryTunnelId (
                                          egtpu_global_t *p_global, 
                                          U8bit          *p_addr,
                                          UInt32         tunnelId)
{
    egtpu_rab_ctxt_t *p_rab_ctxt = PNULL;
    /* Getting the rab_ctxt on basis of tunnelId received for Secondary Tunnel Id */
    if ((p_rab_ctxt = egtpu_db_sel_teid_key_rab_ctxt (p_global, tunnelId)))
    {
        if(p_rab_ctxt->p_secondary_transport_addr)
        {
            LOG_GTPU_MSG(PR_SECONDARY_TRANSPORT_ADDR, LOGINFO, PR_GTPU_CONTROL,
                    relayGtpuCurrentTick_g, __LINE__,
                    tunnelId,0,0,0, 0,0, __func__,"");
        }
        /* Get the peer context for the secondary transport address */ 
        if (!(p_rab_ctxt->p_secondary_transport_addr =
                            __get_peer_ctxt__ (p_global, p_addr)))
        {
            /* release the RAB ctxt acquired above */
            egtpu_db_free_rab_ctxt (p_rab_ctxt);
            
            LOG_GTPU_MSG(PR_SECONDARY_TRANSPORT_ADDR_ALLREADY_EXIST, LOGERROR, PR_GTPU_CONTROL,
                    relayGtpuCurrentTick_g, __LINE__,
                    tunnelId,0,0,0, 0,0, __func__,"");

            return EGTPU_FAILURE ;
        }
	/*No ECHO REQ shall be generated in case of timer duration = 0*/

	if(p_global->tmr_info[0].tmr_dur == 0)
        {
            __incr_ref_cntr__ (p_global,p_rab_ctxt->p_secondary_transport_addr, EGTPU_FALSE);
        }
	else
        {
            __incr_ref_cntr__ (p_global,p_rab_ctxt->p_secondary_transport_addr, EGTPU_TRUE);
        } 
    }
    else
    {
         /* This condition should not hit because the primary tunnel should 
          * already created and hence the RAB should exist */
         LOG_GTPU_MSG(PR_ATTEMPTED_NON_EXIST_RAB, LOGERROR, PR_GTPU_CONTROL,
                    relayGtpuCurrentTick_g, __LINE__,
                    tunnelId,0,0,0, 0,0, __func__,"");

         return EGTPU_FAILURE ;
    }
    return EGTPU_SUCCESS;
}
/* LIPA changes end */
