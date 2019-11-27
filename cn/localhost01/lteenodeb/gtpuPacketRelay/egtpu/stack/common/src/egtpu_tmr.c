/***********************************************************************
**  FILE: File for handling timer related APIs
**
************************************************************************
**  FILE NAME:
**      egtpu_tmr.c
**
**  DESCRIPTION:
**      This file contains fns for Handling timer related APIs.
**
**  DATE          NAME            REFERENCE    REASON
**  ----          ----            ---------    ------
**  Oct, 2009     Vipul Aggarwal               Original
**
**   Copyright (C) 2009 Aricent Inc . All Rights Reserved
***********************************************************************/

#include <egtpu_glob.h>
#include <egtpu_apihdr.h>
#include <egtpu_error.h>

#include <egtpu_msgtypes.h>
#include <egtpu_ietypes.h>

#include <egtpu_stat.h>
#include <egtpu_glob_struct.h>
#include <egtpu_cdb.h>
#include <egtpu_os_proto.h>
#include <egtpu_trace.h>
#include <egtpu_util.h>

#include <egtpu_tmr.h>

/********************************************************************************
 * Function Name  : egtpu_get_timer_req
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This funtion processes the EGTPU_GET_TIMER_REQ API.
 *                  The only supported timer type is EGTPU_ECHO_RSP_TMR.
 *                  This function parse the request API and send cnf of it to LM.
 ********************************************************************************/
egtpu_return_t egtpu_get_timer_req (egtpu_global_t *p_global,
									egtpu_info_t *p_egtpu_info)
{
    U8bit         *p_buf, *p_trav;
    U8bit         tmr_type = *(p_egtpu_info->p_buff + EGTPU_API_HDR_LEN);
    egtpu_return_t ret_val;

    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_GET_TIMER_REQ);
    
    EGTPU_GET_MSG_BUF (p_global,U8bit, p_buf, EGTPU_API_MSG_SIZE);

    /* Rel 5.3: Coverity 24399 Fix Start */
    if (EGTPU_NULL == p_buf)
    {
        /* Free the allocated Memory */
        EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);
        return EGTPU_FAILURE;
    }
    /* Rel 5.3: Coverity Fix End */
    p_trav = p_buf + EGTPU_API_HDR_LEN;

    if (tmr_type != EGTPU_ECHO_RSP_TMR)
	{
        *p_trav++ = EGTPU_PERR_INV_TIMER_TYPE;
        *p_trav++ = tmr_type;
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_INV_TIMER_TYPE_IN_GET_TMR_REQ, LOGDEBUG, PR_GTPU_DL_DATA,
             relayGtpuCurrentTick_g, __LINE__, 
             EGTPU_ECHO_RSP_TMR,tmr_type,0,0, 0,0, __func__,"");
        /* SPR 2030 fix End*/
        EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_INV_TIMER_TYPE,
                    egtpu_entity_name_arr[EGTPU_EI_GTPU], tmr_type));
    }
	else
	{
        *p_trav++ = EGTPU_SUCCESS;
        *p_trav++ = tmr_type;
        EGTPU_SET_U32BIT (p_trav, p_global->tmr_info[0].tmr_dur);
        p_trav += EGTPU_SIZE_OF_TIMER_DURATION;
    }

    EGTPU_SET_API_HDR (p_buf, EGTPU_EI_GTPU, EGTPU_EI_LM,
            EGTPU_LM_GET_TIMER_CNF, (p_trav - p_buf),
            EGTPU_NO_PAYLOAD, EGTPU_NO_PAYLOAD, (p_trav - p_buf - 1));

    EGTPU_SEND_FROM_GTPU (p_global,p_buf, (p_trav - p_buf), EGTPU_EI_RELAY,\
							ret_val,NULL);
   /* EGTPU_SEND_FROM_GTPU (p_global,p_buf, (p_trav - p_buf), EGTPU_EI_LM,\
							ret_val,NULL);*/

    /* SPR 2030 fix Start*/
    LOG_GTPU_MSG(GTPU_SEND_GET_TIMER_CNF, LOGDEBUG, PR_GTPU_DL_DATA,
             relayGtpuCurrentTick_g, __LINE__, 
             p_global->tmr_info[0].tmr_dur,0,0,0, 0,0, __func__,"");
     /* SPR 2030 fix End*/
    /* free the incoming API buffer */
    
    /* During Covering it is found that in success scenario also 
     * it should be freed */
    EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);
    EGTPU_FREE_MSG_BUF (p_buf);

    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_LM_GET_TIMER_CNF);
    return ret_val;
}

/********************************************************************************
 * Function Name  : egtpu_set_timer_req
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This funtion processes the EGTPU_GET_TIMER_REQ API.
 *                  The only supported timer type is EGTPU_ECHO_RSP_TMR.
 *                  This function parse the request API and send cnf of it to LM.
 ********************************************************************************/
egtpu_return_t egtpu_set_timer_req (egtpu_global_t *p_global,
									egtpu_info_t *p_egtpu_info)
{
    U8bit         *p_buf, *p_trav;
    U8bit         tmr_type = *(p_egtpu_info->p_buff + EGTPU_API_HDR_LEN);
    egtpu_return_t ret_val;

    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_SET_TIMER_REQ);

    EGTPU_GET_MSG_BUF (p_global,U8bit, p_buf, EGTPU_API_MSG_SIZE);

    /* Rel 5.3: Coverity 24402 Fix Start */
    if (EGTPU_NULL == p_buf)
    {
        /* Free the allocated Memory */
        EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);
        return EGTPU_FAILURE;
    }
    /* Rel 5.3: Coverity Fix End */
    p_trav = p_buf + EGTPU_API_HDR_LEN;

    if (tmr_type != EGTPU_ECHO_RSP_TMR)
	{
        *p_trav++ = EGTPU_PERR_INV_TIMER_TYPE;
        EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_INV_TIMER_TYPE,
                    egtpu_entity_name_arr[EGTPU_EI_GTPU], tmr_type));
    /* SPR 2030 fix Start*/
    LOG_GTPU_MSG(GTPU_INV_TIMER_TYPE_IN_SET_TMR_REQ, LOGDEBUG, PR_GTPU_DL_DATA,
             relayGtpuCurrentTick_g, __LINE__, 
             EGTPU_ECHO_RSP_TMR,tmr_type,0,0, 0,0, __func__,"");
     /* SPR 2030 fix End*/
    }
	else
	{
        *p_trav++ = EGTPU_SUCCESS;
        p_global->tmr_info[0].tmr_dur = EGTPU_GET_U32BIT
            (p_egtpu_info->p_buff + EGTPU_API_HDR_LEN + 1);
        /* SPR 2030 fix Start*/
         LOG_GTPU_MSG(GTPU_ECHO_RESP_TIMER_DURATION_RCVD, LOGDEBUG, PR_GTPU_DL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                p_global->tmr_info[0].tmr_dur,0,0,0, 0,0, __func__,"");
        /* SPR 2030 fix End*/
    }

    *p_trav++ = tmr_type;

   /* EGTPU_SET_API_HDR (p_buf, EGTPU_EI_GTPU, EGTPU_EI_LM,
            EGTPU_LM_SET_TIMER_CNF, (p_trav - p_buf),
            EGTPU_NO_PAYLOAD, EGTPU_NO_PAYLOAD, (p_trav - p_buf - 1));
*/
    EGTPU_SET_API_HDR (p_buf, EGTPU_EI_GTPU, EGTPU_EI_RELAY,
            EGTPU_LM_SET_TIMER_CNF, (p_trav - p_buf),
            EGTPU_NO_PAYLOAD, EGTPU_NO_PAYLOAD, (p_trav - p_buf - 1));

    /*EGTPU_SEND_FROM_GTPU (p_global,p_buf, (p_trav - p_buf), EGTPU_EI_LM,\
							ret_val, NULL);
*/
    EGTPU_SEND_FROM_GTPU (p_global,p_buf, (p_trav - p_buf), EGTPU_EI_RELAY,\
							ret_val, NULL);

    /* SPR 2030 fix Start*/
    LOG_GTPU_MSG(GTPU_SEND_SET_TIMER_CNF, LOGDEBUG, PR_GTPU_DL_DATA,
             relayGtpuCurrentTick_g, __LINE__, 
             0,0,0,0, 0,0, __func__,"");
     /* SPR 2030 fix End*/
    /* free the incoming API buffer */
    /* During Covering it is found that in success scenario also 
     * it should be freed */ 
    EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);
    EGTPU_FREE_MSG_BUF (p_buf);

    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_LM_SET_TIMER_CNF);
    return ret_val;
}

/********************************************************************************
 * Function Name  : egtpu_get_max_retries_req
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This funtion processes the EGTPU_GET_MAX_RETRIES_REQ API.
 *                  The only supported timer type is EGTPU_ECHO_RSP_TMR.
 *                  This function parse the request API and send cnf of it to LM.
 ********************************************************************************/
egtpu_return_t egtpu_get_max_retries_req (egtpu_global_t *p_global,
										egtpu_info_t *p_egtpu_info)
{
    U8bit         *p_buf, *p_trav;
    U8bit         tmr_type = *(p_egtpu_info->p_buff + EGTPU_API_HDR_LEN);
    egtpu_return_t ret_val;

    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_GET_MAX_RETRIES_REQ);
    
    EGTPU_GET_MSG_BUF (p_global,U8bit, p_buf, EGTPU_API_MSG_SIZE);

    /* Rel 5.3: Coverity 24398 Fix Start */
    if (EGTPU_NULL == p_buf)
    {
        /* Free the allocated Memory */
        EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);
        return EGTPU_FAILURE;
    }
    /* Rel 5.3: Coverity Fix End */
    p_trav = p_buf + EGTPU_API_HDR_LEN;

    if (tmr_type != EGTPU_ECHO_RSP_TMR)
	{
        *p_trav++ = EGTPU_PERR_INV_TIMER_TYPE;
        *p_trav++ = tmr_type;
       /* SPR 2030 fix Start*/
       LOG_GTPU_MSG(GTPU_INV_TIMER_TYPE_IN_GET_MAX_RETRY_REQ, LOGDEBUG, 
              PR_GTPU_DL_DATA, relayGtpuCurrentTick_g, __LINE__,
              0,0,0,0, 0,0, __func__,"");
       /* SPR 2030 fix End*/
        EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_INV_TIMER_TYPE,
                    egtpu_entity_name_arr[EGTPU_EI_GTPU], tmr_type));
    }
	else
	{
        *p_trav++ = EGTPU_SUCCESS;
        *p_trav++ = tmr_type;
        EGTPU_SET_U16BIT (p_trav, p_global->tmr_info[0].max_retries);
        p_trav += EGTPU_SIZE_OF_RETRY_COUNT;
    }

    EGTPU_SET_API_HDR (p_buf, EGTPU_EI_GTPU, EGTPU_EI_LM,
            EGTPU_LM_GET_MAX_RETRIES_CNF, (p_trav - p_buf),
            EGTPU_NO_PAYLOAD, EGTPU_NO_PAYLOAD, (p_trav - p_buf - 1));

    EGTPU_SEND_FROM_GTPU (p_global,p_buf, (p_trav - p_buf), EGTPU_EI_RELAY,\
							ret_val, NULL);
    /*EGTPU_SEND_FROM_GTPU (p_global,p_buf, (p_trav - p_buf), EGTPU_EI_LM,\
							ret_val, NULL);*/

    /* SPR 2030 fix Start*/
    LOG_GTPU_MSG(GTPU_SEND_GET_MAX_RETRY_CNF, LOGDEBUG, PR_GTPU_DL_DATA,
             relayGtpuCurrentTick_g, __LINE__, 
             p_global->tmr_info[0].max_retries,0,0,0, 0,0, __func__,"");
     /* SPR 2030 fix End*/

    /* free the incoming API buffer */
    /* During Covering it is found that in success scenario also 
     * it should be freed */
    EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);
    EGTPU_FREE_MSG_BUF (p_buf);

    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_LM_GET_MAX_RETRIES_CNF);
    return ret_val;
}

/********************************************************************************
 * Function Name  : egtpu_set_max_retries_req
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This funtion processes the EGTPU_SET_MAX_RETRIES_REQ API.
 *                  The only supported timer type is EGTPU_ECHO_RSP_TMR.
 *                  This function parse the request API and send cnf of it to LM.
 ********************************************************************************/
egtpu_return_t egtpu_set_max_retries_req (egtpu_global_t *p_global,
											egtpu_info_t *p_egtpu_info)
{
    U8bit         *p_buf, *p_trav;
    U8bit         tmr_type = *(p_egtpu_info->p_buff + EGTPU_API_HDR_LEN);
    egtpu_return_t ret_val;

    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_SET_MAX_RETRIES_REQ);
    
    EGTPU_GET_MSG_BUF (p_global,U8bit, p_buf, EGTPU_API_MSG_SIZE);

    /* Rel 5.3: Coverity 24401 Fix Start */
    if (EGTPU_NULL == p_buf)
    {
        /* Free the allocated Memory */
        EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);
        return EGTPU_FAILURE;
    }
    /* Rel 5.3: Coverity Fix End */
    p_trav = p_buf + EGTPU_API_HDR_LEN;

    if (tmr_type != EGTPU_ECHO_RSP_TMR)
	{
        *p_trav++ = EGTPU_PERR_INV_TIMER_TYPE;
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_INV_TIMER_TYPE_IN_SET_MAX_RETRY_REQ, LOGDEBUG, PR_GTPU_DL_DATA,
             relayGtpuCurrentTick_g, __LINE__, 
             EGTPU_ECHO_RSP_TMR,tmr_type,0,0, 0,0, __func__,"");
        /* SPR 2030 fix End*/
        EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_INV_TIMER_TYPE,
                    egtpu_entity_name_arr[EGTPU_EI_GTPU], tmr_type));
    }
	else
	{
        *p_trav++ = EGTPU_SUCCESS;
        p_global->tmr_info[0].max_retries = EGTPU_GET_U16BIT
            (p_egtpu_info->p_buff + EGTPU_API_HDR_LEN + 1);
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_SET_MAX_RETRY_REQ, LOGDEBUG, PR_GTPU_DL_DATA,
             relayGtpuCurrentTick_g, __LINE__, 
             p_global->tmr_info[0].max_retries,0,0,0, 0,0, __func__,"");
        /* SPR 2030 fix End*/
    }

    *p_trav++ = tmr_type;

    EGTPU_SET_API_HDR (p_buf, EGTPU_EI_GTPU, EGTPU_EI_LM,
            EGTPU_LM_SET_MAX_RETRIES_CNF, (p_trav - p_buf),
            EGTPU_NO_PAYLOAD, EGTPU_NO_PAYLOAD, (p_trav - p_buf - 1));

    EGTPU_SEND_FROM_GTPU (p_global,p_buf, (p_trav - p_buf), EGTPU_EI_RELAY,
							ret_val,NULL);
    /*EGTPU_SEND_FROM_GTPU (p_global,p_buf, (p_trav - p_buf), EGTPU_EI_RELAY,
							ret_val,NULL);*/
    /* SPR 2030 fix Start*/
    LOG_GTPU_MSG(GTPU_SEND_SET_MAX_RETRY_CNF, LOGDEBUG, PR_GTPU_DL_DATA,
             relayGtpuCurrentTick_g, __LINE__, 
             0,0,0,0, 0,0, __func__,"");
    /* SPR 2030 fix End*/

    /* free the incoming API buffer */
    /* During Covering it is found that in success scenario also 
     * it should be freed */ 
    EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);
    EGTPU_FREE_MSG_BUF (p_buf);


    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_LM_SET_MAX_RETRIES_CNF);
    return ret_val;
}


/********************************************************************************
 * Function Name  : egtpu_modify_reordering_ie_req
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This funtion processes the EGTPU_MODIFY_REORDERING_IE_REQ API.
 *                  This function parse the request API and send cnf of it to LM.
 *                  Sets the Max Window size and Reorder Timer Duration for the
 *                  given ip traffic class.
 ********************************************************************************/
egtpu_return_t egtpu_modify_reordering_ie_req (egtpu_global_t *p_global,
												egtpu_info_t *p_egtpu_info)
{
    U8bit         *p_buf, *p_trav;
    U8bit         ip_traffic;
    egtpu_return_t ret_val;

    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_MODIFY_REORDERING_REQ);
    
    EGTPU_GET_MSG_BUF (p_global,U8bit, p_buf, EGTPU_MAX_MSG_SIZE);

    /* Rel 5.3: Coverity 24400 Fix Start */
    if (EGTPU_NULL == p_buf)
    {
        /* Free the allocated Memory */
        EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);
        return EGTPU_FAILURE;
    }
    /* Rel 5.3: Coverity Fix End */

    p_trav = p_buf + EGTPU_API_HDR_LEN;

	ip_traffic=(*((U8bit *)p_egtpu_info->p_buff + \
					EGTPU_API_HDR_LEN) & 0xE0) >> 5;

	if(ip_traffic > EGTPU_BKGROUND_TRAFFIC_CLASS)
	{ 
        *p_trav++ = EGTPU_PERR_INV_TRAFFIC_CLASS;
        EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_INV_TRAFFIC_CLASS,
                    egtpu_entity_name_arr[EGTPU_EI_GTPU], ip_traffic));

        /* SPR 17747 FIX START */
        LOG_MSG( GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON,
                relayGtpuCurrentTick_g, __LINE__, EGTPU_PERR_INV_TRAFFIC_CLASS,
                ip_traffic,0,0,0,0, __func__,
                egtpu_entity_name_arr[EGTPU_EI_GTPU] );
        /* SPR 17747 FIX END */
    }
	else
	{
        *p_trav++ = EGTPU_SUCCESS;
		p_egtpu_info->max_window_size = 
			EGTPU_GET_U16BIT(p_egtpu_info->p_buff + EGTPU_API_HDR_LEN +
								EGTPU_SIZE_OF_RESULT);

		if(p_global->max_window_size[ip_traffic] != 
								p_egtpu_info->max_window_size)
			p_global->max_window_size[ip_traffic] = 
								p_egtpu_info->max_window_size;

		p_egtpu_info->reorder_timer_drtn = 
			EGTPU_GET_U32BIT(p_egtpu_info->p_buff + EGTPU_API_HDR_LEN + \
							EGTPU_SIZE_OF_RESULT + EGTPU_SIZE_OF_WINDOW );

        if(p_global->reorder_timer_drtn[ip_traffic] != 
											p_egtpu_info->reorder_timer_drtn)
			p_global->reorder_timer_drtn[ip_traffic] = 
											p_egtpu_info->reorder_timer_drtn;
    }

    *p_trav++ = *((U8bit *)p_egtpu_info->p_buff + EGTPU_API_HDR_LEN);

    EGTPU_SET_API_HDR (p_buf, EGTPU_EI_GTPU, EGTPU_EI_LM,
            EGTPU_LM_MODIFY_REORDERING_IE_CNF, (p_trav - p_buf),
            EGTPU_NO_PAYLOAD, EGTPU_NO_PAYLOAD, (p_trav - p_buf - 1));

    EGTPU_SEND_FROM_GTPU (p_global,p_buf, (p_trav - p_buf), EGTPU_EI_LM, \
								ret_val, NULL);

    /* free the incoming API buffer */
    /* During Covering it is found that in success scenario also 
     * it should be freed */ 
    EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);
    EGTPU_FREE_MSG_BUF (p_buf);


    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_LM_MODIFY_REORDERING_CNF);

    return ret_val;
}
