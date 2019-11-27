/***********************************************************************
**  FUNCTION:
**	GTP-U utility file
************************************************************************
**
**  FILE NAME:
**	egtpu_util.c
**
**  DESCRIPTION:
**	Contains data definitions that will be used by FSM
**      namely global, api bases.
**      Handling of Query Interface APIs is also done here. Uses
**      database interface to answer queries on context information
**      in the stack.
**      Contains encoding/sending functionality for Alarm Notification
**      API to LM.
**
**  NOTE:
**      Queries that traverse API calls, the get_first/get_next
**      functionality,  are FSM dependent. If the FSM changes between
**      two calls, the results may not be fully correct.
**
**  DATE	    NAME			REFERENCE	REASON
**  ----	    ----			---------	------
**  Oct 2009    Vipul Aggarwal        		Initial
**
**   Copyright (C) 2009 Aricent Inc . All Rights Reserved
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
#include <egtpu_sap.h>

/***********************************************************************
** DATA FUNCTION:
**	Configuration storage
***********************************************************************/

/* incoming API-ID bases */
/* the index to this array is the source identifier of the sender */
U16bit const egtpu_in_api_base_arr[] = {
    EGTPU_NULL,                  /* self */
    EGTPU_UDP_IN_API_BASE,
    EGTPU_RELAY_IN_API_BASE,
    EGTPU_CP_IN_API_BASE,
    EGTPU_LM_IN_API_BASE,
    EGTPU_TIMEOUT_IN_API_BASE
};

/* ID of last incoming APIs */
/* the index to this array is the source identifier of the sender */
U16bit const egtpu_in_last_api[] = {
    EGTPU_NULL,                  /* self */
    EGTPU_UDP_LAST_IN_API,
    EGTPU_RELAY_LAST_IN_API,
    EGTPU_CP_LAST_IN_API,
    EGTPU_LM_LAST_IN_API,
    EGTPU_TIMEOUT_LAST_IN_API
};

/* outgoing API-ID bases */
/* the index to this array is the destination id of the reciever */
U16bit const egtpu_out_api_base_arr[] = {
    EGTPU_NULL,                  /* self */
    EGTPU_UDP_OUT_API_BASE,
    EGTPU_RELAY_OUT_API_BASE,
    EGTPU_CP_OUT_API_BASE,
    EGTPU_LM_OUT_API_BASE,
    EGTPU_UDP_OUT_API_BASE,
    EGTPU_NULL
};

/********************************************************************************
 * Function Name  : __enc_rab_ctxt__
 * Inputs         : p_global - Pointer to Global Structure egtpu_global_t,
 *                  data,
 *                  p_ctxt - Pointer to egtpu_rab_ctxt_t
 * Outputs        : None
 * Returns        : None
 * Description    : This function encodes the RAB ctxt for
 *                  EGTPU_LM_GET_CTXT_INFO_CNF API.
 ********************************************************************************/
static egtpu_void_t __enc_rab_ctxt__ (egtpu_global_t *p_global,U8bit **data,
										egtpu_rab_ctxt_t *p_ctxt)
{
    U8bit *p_tmp = *data;

    *p_tmp++ = EGTPU_RAB_CTXT_INFO;
    p_tmp += EGTPU_SIZE_OF_LENGTH; /* leave space to fill-in length later */

    EGTPU_SET_U32BIT (p_tmp, egtpu_db_get_teid_key_rab_ctxt (p_global,p_ctxt));
    p_tmp += EGTPU_SIZE_OF_TEID;

    EGTPU_SET_U32BIT (p_tmp, p_ctxt->transaction_id);
    p_tmp += EGTPU_SIZE_OF_TRANSACTION_ID;

    /* SPR 10787 fix start */
    EGTPU_SET_U32BIT (p_tmp, p_ctxt->teid_cell.teid_peer);
    /* SPR 10787 fix end */
    p_tmp += EGTPU_SIZE_OF_TEID;

    EGTPU_SET_U32BIT (p_tmp, p_ctxt->teid_peer_fwd);
    p_tmp += EGTPU_SIZE_OF_TEID;

    if (p_ctxt->p_transport_addr)
	{
        egtpu_memcpy (p_tmp, p_ctxt->p_transport_addr->entity_addr,
		EGTPU_SIZE_OF_LENGTH +
               EGTPU_GET_U16BIT(p_ctxt->p_transport_addr->entity_addr));
        p_tmp += (EGTPU_SIZE_OF_LENGTH +
               EGTPU_GET_U16BIT(p_ctxt->p_transport_addr->entity_addr));
    }
	else
	{ /* feed in length as NULL */
        *p_tmp++ = EGTPU_NULL;
        *p_tmp++ = EGTPU_NULL;
    }

    if (p_ctxt->p_forward_addr)
	{
        egtpu_memcpy (p_tmp, p_ctxt->p_forward_addr->entity_addr,
		EGTPU_SIZE_OF_LENGTH +
                EGTPU_GET_U16BIT(p_ctxt->p_forward_addr->entity_addr));
        p_tmp += (EGTPU_SIZE_OF_LENGTH +
                EGTPU_GET_U16BIT(p_ctxt->p_forward_addr->entity_addr));
    }
	else
	{ /* feed in length as NULL */
        *p_tmp++ = EGTPU_NULL;
        *p_tmp++ = EGTPU_NULL;
    }

    EGTPU_SET_U16BIT (p_tmp, p_ctxt->seq_num_uplk);
    p_tmp += EGTPU_SIZE_OF_SEQ_NUM;

    EGTPU_SET_U16BIT (p_tmp, p_ctxt->seq_num_dnlk);
    p_tmp += EGTPU_SIZE_OF_SEQ_NUM;

    *p_tmp++ = p_ctxt->sap_flag;

    EGTPU_SET_U32BIT (p_tmp, p_ctxt->qos_id);
    p_tmp += EGTPU_SIZE_OF_QOS_ID;

	*p_tmp++ = p_ctxt->reordering_reqd;
		
	if(p_ctxt->reordering_reqd)
	{
		EGTPU_SET_U16BIT (p_tmp, p_ctxt->max_window_size);
        p_tmp += EGTPU_SIZE_OF_WINDOW;

       	EGTPU_SET_U32BIT (p_tmp, p_ctxt->reorder_timer_drtn);
        p_tmp += EGTPU_SIZE_OF_REORDER_TIMER;
	}

    /* fill in length now */
    EGTPU_SET_U16BIT((*data + EGTPU_SIZE_OF_TYPE),
				(p_tmp - *data - EGTPU_SIZE_OF_TYPE - EGTPU_SIZE_OF_LENGTH));

    *data = p_tmp;
}

/********************************************************************************
 * Function Name  : __enc_peer_ctxt__
 * Inputs         : data
 *                  p_ctxt - Pointer to egtpu_peer_ctxt_t
 * Outputs        : None
 * Returns        : None
 * Description    : This function encodes the PEER ctxt for
 *                  EGTPU_LM_GET_CTXT_INFO_CNF API.
 ********************************************************************************/
static egtpu_void_t __enc_peer_ctxt__ (U8bit **data, egtpu_peer_ctxt_t *p_ctxt)
{
    U8bit *p_tmp = *data;

    *p_tmp++ = EGTPU_PEER_CTXT_INFO;
    p_tmp += EGTPU_SIZE_OF_LENGTH; /* leave space to fill-in length later */

    egtpu_memcpy (p_tmp, p_ctxt->entity_addr,
        EGTPU_GET_U16BIT (p_ctxt->entity_addr) + EGTPU_SIZE_OF_LENGTH);
    p_tmp += (EGTPU_GET_U16BIT (p_ctxt->entity_addr) + EGTPU_SIZE_OF_LENGTH);

    *p_tmp++ = p_ctxt->ext_hdr;

    EGTPU_SET_U32BIT (p_tmp, p_ctxt->ref_cntr);
    p_tmp += EGTPU_SIZE_OF_REC_COUNT_FLAG;
	if(p_ctxt->path_struct.tmr_flag == EGTPU_TIMER_ON)
    	*p_tmp++ = p_ctxt->path_struct.tmr_flag;
	else
		*p_tmp++ = EGTPU_TIMER_OFF;
    *p_tmp++ = p_ctxt->path_struct.path_cntr;

    /* fill in length now */
    EGTPU_SET_U16BIT ((*data + EGTPU_SIZE_OF_TYPE),
				(p_tmp - *data - EGTPU_SIZE_OF_TYPE - EGTPU_SIZE_OF_LENGTH));

    *data = p_tmp;
}

/********************************************************************************
 * Function Name  : __enc_rec_count__
 * Inputs         : data,
 *                  num
 * Outputs        : None
 * Returns        : None
 * Description    : This function encodes the PEER ctxt for
 *                  EGTPU_LM_GET_CTXT_INFO_CNF API.
 ********************************************************************************/
static egtpu_void_t __enc_rec_count__ (U8bit **data, U32bit num)
{
    U8bit *p_tmp = *data;

    *p_tmp++ = EGTPU_IE_REC_COUNT;

    EGTPU_SET_U32BIT (p_tmp, num);
    p_tmp += EGTPU_SIZE_OF_REC_COUNT_FLAG;

    *data = p_tmp;
}

/********************************************************************************
 * Function Name  : __enc_next_idx__
 * Inputs         : data,
 *                  next_idx - Next Index
 * Outputs        : None
 * Returns        : None
 * Description    : This function encodes the next id for
 *                  EGTPU_LM_GET_CTXT_INFO_CNF API.
 ********************************************************************************/
static egtpu_void_t __enc_next_idx__ (U8bit **data, U32bit next_idx)
{
    U8bit *p_tmp = *data;

    *p_tmp++ = EGTPU_IE_NEXT_IDX;

    EGTPU_SET_U32BIT (p_tmp, next_idx);
    p_tmp += EGTPU_SIZE_OF_NEXT_IDX_FLAG;

    *data = p_tmp;
}

/********************************************************************************
 * Function Name  : __all_rab__
 * Inputs         : p_global - Pointer to Global structure egtpu_global_t,
 *                  data,
 *                  p_qi_info - Pointer to egtpu_qi_info_t,
 *                  ie_presence
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This function 
 ********************************************************************************/
static  egtpu_return_t __all_rab__ (egtpu_global_t *p_global,U8bit **data,
								egtpu_qi_info_t *p_qi_info, U32bit ie_presence)
{
    egtpu_rab_ctxt_t *p_ctxt = EGTPU_NULL;
    U32bit          next_idx;
    egtpu_return_t  ret_val;

    next_idx = (ie_presence & EGTPU_IE_NEXT_IDX_FLAG) ? p_qi_info->next_idx : 1;

    while (p_qi_info->rec_count-- && 
				(p_ctxt = egtpu_db_get_rab_ctxt_by_idx (p_global,&next_idx)))
        __enc_rab_ctxt__ (p_global,data, p_ctxt);

    if (p_ctxt) /* more contexts to be queried */
        __enc_next_idx__ (data, next_idx);

     ret_val = EGTPU_SUCCESS;
	 return ret_val;

}

/********************************************************************************
 * Function Name  : __fltr_rab__
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  data,
 *                  p_qi_info - Pointer to egtpu_qi_info_t,
 *                  ie_presence
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This function 
 ********************************************************************************/
static  egtpu_return_t __fltr_rab__ (egtpu_global_t *p_global,U8bit **data,
								egtpu_qi_info_t *p_qi_info, U32bit ie_presence)
{
    egtpu_rab_ctxt_t *p_ctxt;
    U32bit          next_idx = 1;
    egtpu_return_t  ret_val;
    /* SPR 10787 fix start */
    egtpu_peer_teid_cell_ctxt_t teid_cell;
    /* SPR 10787 fix end */

    switch (p_qi_info->fltr.type)
	{
        case EGTPU_IE_TRANSACTION_ID:
            /* O(n), n:pdp-ctxt-tbl size */
            while((p_ctxt = egtpu_db_get_rab_ctxt_by_idx (p_global,&next_idx)))
                if (p_ctxt->transaction_id == p_qi_info->fltr.val.xid)
				{
                    __enc_rab_ctxt__ (p_global,data, p_ctxt);
                    break;
                }
            break;

        case EGTPU_IE_TEID_DATA_I:
            /* O(1) */

             if((p_ctxt = egtpu_db_sel_teid_key_rab_ctxt
									(p_global,p_qi_info->fltr.val.teid)))
			 {
                __enc_rab_ctxt__ (p_global,data, p_ctxt);
			 }
			else 
			 {
			    ret_val = EGTPU_CTXT_NOT_FOUND ;
			    return ret_val;
			 }

            break;
          
        case EGTPU_IE_GSN_ADDR:
            /* SPR 10787 fix start */
            teid_cell.teid_peer = p_qi_info->next_idx;
            teid_cell.cellIndex = INVALID_CELL_INDEX;
            /* SPR 10787 fix end */
            /* O(ln(n)), n:pdp-ctxt-tbl size */
            for (p_ctxt = egtpu_db_get_first_peer_map
						(p_qi_info->fltr.val.addr, 
                        /* SPR 10787 fix start */
                        teid_cell,
                        /* SPR 10787 fix end */
                        ie_presence & EGTPU_IE_NEXT_IDX_FLAG);
                    p_qi_info->rec_count-- && p_ctxt;
                    p_ctxt = egtpu_db_get_next_peer_map (p_ctxt))
                __enc_rab_ctxt__ (p_global,data, p_ctxt);

            if (p_ctxt) /* more contexts to be queried */
                __enc_next_idx__ (data, (p_ctxt->sap_flag & EGTPU_FWD_SAP) ?
                        /* SPR 10787 fix start */
                        p_ctxt->teid_peer_fwd : p_ctxt->teid_cell.teid_peer);
                        /* SPR 10787 fix end */
            break;
    }
    ret_val = EGTPU_SUCCESS;
	return ret_val;

}

/********************************************************************************
 * Function Name  : __num_rab__
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  data,
 *                  p_qi_info - Pointer to egtpu_qi_info_t,
 *                  ie_presence
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This function 
 ********************************************************************************/
/* O(n), n:pdp-ctxt-tbl size */
static egtpu_return_t __num_rab__ (egtpu_global_t *p_global,U8bit **data,
								egtpu_qi_info_t *p_qi_info, U32bit ie_presence)
{
    U32bit next_idx = 1;
    U32bit count = EGTPU_NULL;
    egtpu_return_t  ret_val;


    while (egtpu_db_get_rab_ctxt_by_idx (p_global,&next_idx))
        count++;

    __enc_rec_count__ (data, count);
  
    /* GCC_4.9_WARNINGS_FIX_START */
    (void)p_qi_info;
    (void)ie_presence;
    /* GCC_4.9_WARNINGS_FIX_END */
    ret_val = EGTPU_SUCCESS;
	return ret_val;

}

/********************************************************************************
 * Function Name  : __all_peer__
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  data,
 *                  p_qi_info - Pointer to egtpu_qi_info_t,
 *                  ie_presence
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This function 
 ********************************************************************************/
/* O(1) */

static egtpu_return_t  __all_peer__ (egtpu_global_t *p_global,U8bit **data,
								egtpu_qi_info_t *p_qi_info, U32bit ie_presence)
{
    egtpu_peer_ctxt_t *p_ctxt;
    egtpu_return_t  ret_val;

    for (p_ctxt = egtpu_db_get_first_peer_ctxt ();
            p_ctxt; p_ctxt = egtpu_db_get_next_peer_ctxt (p_ctxt))
        __enc_peer_ctxt__ (data, p_ctxt);
    /* GCC_4.9_WARNINGS_FIX_START */
    (void)p_global;
    (void)p_qi_info;
    (void)ie_presence;
    /* GCC_4.9_WARNINGS_FIX_END */
    ret_val = EGTPU_SUCCESS;
	return ret_val;

}

/********************************************************************************
 * Function Name  : __fltr_peer__
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  data,
 *                  p_qi_info - Pointer to egtpu_qi_info_t,
 *                  ie_presence
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This function 
 ********************************************************************************/
/* O(n), n:peer-ctxt-tbl size, a small figure */

static egtpu_return_t __fltr_peer__ (egtpu_global_t *p_global,U8bit **data,
								egtpu_qi_info_t *p_qi_info, U32bit ie_presence)
{
    egtpu_peer_ctxt_t *p_ctxt;
    egtpu_return_t  ret_val;

    if (p_qi_info->fltr.type == EGTPU_IE_GSN_ADDR)
	 {	
		if((p_ctxt = 
					egtpu_db_sel_addr_key_peer_ctxt (p_qi_info->fltr.val.addr)))
		{
		
			__enc_peer_ctxt__ (data, p_ctxt);
		}	
		else
		{
			ret_val = EGTPU_CTXT_NOT_FOUND ;
			return ret_val;
		}

	 }
     /* GCC_4.9_WARNINGS_FIX_START */
    (void)p_global;
    (void)ie_presence;
    /* GCC_4.9_WARNINGS_FIX_END */
	ret_val = EGTPU_SUCCESS;
	return ret_val;
}

/********************************************************************************
 * Function Name  : __num_peer__
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  data,
 *                  p_qi_info - Pointer to egtpu_qi_info_t,
 *                  ie_presence
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This function 
 ********************************************************************************/
/* O(n), n:peer-ctxt-tbl size, a small figure */

static  egtpu_return_t __num_peer__ (egtpu_global_t *p_global,U8bit **data,
								egtpu_qi_info_t *p_qi_info, U32bit ie_presence)
{
    egtpu_peer_ctxt_t *p_ctxt;
    U32bit           count = EGTPU_NULL;
    egtpu_return_t  ret_val;

    for (p_ctxt = egtpu_db_get_first_peer_ctxt();
            p_ctxt; p_ctxt = egtpu_db_get_next_peer_ctxt (p_ctxt))
        count++;

    __enc_rec_count__ (data, count);
    
    /* GCC_4.9_WARNINGS_FIX_START */
    (void)p_global;
    (void)p_qi_info;
    (void)ie_presence;
    /* GCC_4.9_WARNINGS_FIX_END */
    ret_val = EGTPU_SUCCESS;
	return ret_val;

}

typedef  egtpu_return_t (*qi_hlr_t) (egtpu_global_t *,U8bit **,
									egtpu_qi_info_t *, U32bit);
static qi_hlr_t qi_hlr[2][3] = {
    {__all_rab__, __fltr_rab__, __num_rab__},
    {__all_peer__, __fltr_peer__, __num_peer__}
};

/********************************************************************************
 * Function Name  : egtpu_get_ctxt_info_req
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This function processes the EGTPU_GET_CTXT_INFO_REQ API.
 ********************************************************************************/
egtpu_return_t egtpu_get_ctxt_info_req (egtpu_global_t *p_global,
										egtpu_info_t *p_egtpu_info)
{
    egtpu_qi_info_t *p_qi_info = &p_egtpu_info->qi_info;
     U8bit          *p_buf, *p_param,*p_result, idx;
    egtpu_return_t  ret_val;

    /* update incoming statistics */
    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_GET_CTXT_INFO_REQ);

    /* acquire buffer to send response */
    EGTPU_GET_MSG_BUF (p_global,U8bit, p_buf, EGTPU_MAX_MSG_SIZE);
 
    /* Rel 5.3: Coverity 24406 Fix Start */
    if (EGTPU_NULL == p_buf)
    {
        /* Free the allocated memory */
        EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);
        return EGTPU_FAILURE;
    }
    /* Rel 5.3: Coverity Fix End */

    p_param = p_buf + EGTPU_API_HDR_LEN; /* to feed in response values */
    /* result is filled later; storing the pointer */
    p_result = p_param;
    p_param++  ;

    idx = (p_qi_info->rec_type == EGTPU_RAB_CTXT_INFO) ? 0 : 1;
    ret_val = qi_hlr[idx][p_qi_info->req_code]
        (p_global,&p_param, p_qi_info, p_egtpu_info->ie_presence);
	
	if (ret_val != EGTPU_SUCCESS)	
        *p_result = ret_val;	
	else	
		*p_result = EGTPU_SUCCESS ;
	
	
    EGTPU_SET_API_HDR (p_buf, EGTPU_EI_GTPU, EGTPU_EI_LM,
            EGTPU_LM_GET_CTXT_INFO_CNF, (p_param - p_buf),
            EGTPU_NO_PAYLOAD, EGTPU_NO_PAYLOAD, (p_param - p_buf - 1));

    /* submit the response to the query and update outgoing stats */
    EGTPU_SEND_FROM_GTPU (p_global,p_buf, (p_param - p_buf), EGTPU_EI_LM,\
							ret_val,NULL);
    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_LM_GET_CTXT_INFO_CNF);

    /* free the incoming request buffer */
    /* During Covering it is found that in success scenario also 
     * it should be freed */
    EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);
    EGTPU_FREE_MSG_BUF (p_buf);

    return ret_val;
}

 
/********************************************************************************
 * Function Name  : egtpu_send_alarm_notification
 * Inputs         : p_global - Pointer to egtpu_global_t,
 *                  alarm,
 *                  info
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This function encodes and sends the EGTPU_LM_ALARM_NOTIFICATION
 *                  API.
 ********************************************************************************/
egtpu_return_t egtpu_send_alarm_notification (egtpu_global_t *p_global,
												U8bit alarm, U8bit *info)
{
    U8bit         *p_buf, *p_param;
    egtpu_return_t ret_val;

    /* acquire buffer to send alarm */
    EGTPU_GET_MSG_BUF (p_global,U8bit, p_buf, EGTPU_API_MSG_SIZE);

    /* Rel 5.3: Coverity 24407 Fix Start */
    if (EGTPU_NULL == p_buf)
    {
        /* Not able to allocate memory */
        return EGTPU_FAILURE;
    }
    /* Rel 5.3: Coverity Fix End */

    p_param = p_buf + EGTPU_API_HDR_LEN; /* to feed in elements */

    *p_param++ = alarm;

    if (alarm == EGTPU_CERR_PATH_FAILURE)
	{
        *p_param++ = EGTPU_IE_GSN_ADDR;
        /* Coverity 72813 Fix Start*/
        if ((EGTPU_GET_U16BIT(info) + EGTPU_SIZE_OF_LENGTH) > (EGTPU_API_MSG_SIZE - EGTPU_API_HDR_LEN))
        {
            EGTPU_FREE_MSG_BUF (p_buf);
            return EGTPU_FAILURE;
        }
        /* Coverity 72813 Fix End*/

        egtpu_memcpy(p_param, info,
					EGTPU_GET_U16BIT(info) + EGTPU_SIZE_OF_LENGTH);
        p_param += (EGTPU_GET_U16BIT (info) + EGTPU_SIZE_OF_LENGTH);
    }

    EGTPU_SET_API_HDR (p_buf, EGTPU_EI_GTPU, EGTPU_EI_LM,
            EGTPU_LM_ALARM_NOTIFICATION, (p_param - p_buf),
            EGTPU_NO_PAYLOAD, EGTPU_NO_PAYLOAD, (p_param - p_buf - 1));

    /* submit the alarm notification and update outgoing stats */
    EGTPU_SEND_FROM_GTPU (p_global,p_buf, (p_param - p_buf), EGTPU_EI_LM,\
							ret_val, NULL);
    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_LM_ALARM_NOTIFICATION);

    /* + coverity 36235 */
    EGTPU_FREE_MSG_BUF (p_buf);
    /* - coverity 36235 */

    return ret_val;
}
