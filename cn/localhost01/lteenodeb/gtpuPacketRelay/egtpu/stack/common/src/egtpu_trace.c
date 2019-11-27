/***********************************************************************
** FUNCTION : 
**     Source file for trace related stuff
**
************************************************************************
** DESCRIPTION :
**     This file contains the definitions of arrays and functions
**     used to perform tracing in GTP-U
**
** DATE          NAME               REFERENCE               REASON
** ----          ----               ---------               ------
** Oct,2009      Vipul Aggarwal                            Original
**
**  Copyright (C) 2009 Aricent Inc . All Rights Reserved
***********************************************************************/
#include <ylib.h>
#include <egtpu_glob.h>
#include <egtpu_apihdr.h>
#include <egtpu_error.h>

#include <egtpu_msgtypes.h>
#include <egtpu_ietypes.h>

#include <egtpu_stat.h>
#include <egtpu_glob_struct.h>
#include <egtpu_cdb.h>
#include <egtpu_os_proto.h>
#include <egtpu_util.h>

#include <egtpu_trace.h>

#if defined (EGTPU_ENABLE_TRACE)
/***********************************************************************
** DATA FUNCTION:
**    Array to contain trace flags for GTP
***********************************************************************/
S8bit const *egtpu_trace_array[] = {
    /* Incoming API */
    "%s : API recd [%s]\n",
    /* Incoming Message */
    "%s : GTP-U Message [%s] received from Peer.\n",
    /* Outgoing API */
    "%s : API sent [%s] to Destination [%s].\n",
    /* Outgoing Message */
    "%s : GTP-U Message [%s] sent to Peer.\n",
    /* Timer Started */
    "%s : Started Timer [%s] \n",
    /* Timer Stopped */
    "%s : Stopped Timer [%s] \n",
    /* Function Entered - shall be used for normal functions */
    "%s : Entering Function [%s] \n",
    /* Function Exited - shall be used for normal functions */
    "%s : Exiting Function [%s] \n",
    /* Handler Entered - shall be used for handlers */
    "%s : Entering Handler for [%s] \n",
    /* Handler Exited - shall be used for handlers */
    "%s : Exiting Handler for [%s] \n"
};


S8bit const *egtpu_error_array[] = {
    "%s : Unknown MSG received [Msg Id: %d]\n",
    "%s : Unknown API received [API Id: %d] from [SRC: %d]\n",
    "%s : Unexpected MSG received [%s]\n",
    "%s : Unexpected API received [%s]\n",
    "%s : Too short GTP-U MSG received\n",
    "%s : GTP-U MSG of invalid version received\n",
    "%s : GTP-U MSG of invalid protocol type received\n",
    "%s : GTP-U MSG with invalid format received [%s]\n",
    "%s : MSG/API with mandatory IE missing received [%s]\n",
    "%s : MSG/API with mandatory IE incorrect received [%s]\n",
    "%s : Echo response timer expired [path-cntr: %d]\n",
    "%s : Invalid Timer Type [%d]\n",
    "%s : Unexpected timer expiry [%s]\n",
    "%s : Extension header not supported [type: %d]\n",
	"%s :Invalid API Format\n",
	"%s :Invalid Opcode Received\n",

    "%s : Unable to insert record in database\n",
    "%s : Key not present in lookup data structure\n",
    "%s : Unable to allocate hash record \n",

    "%s : Unable to Allocate Static Memory of size [%d]\n",
    "%s : Unable to Allocate Dynamic Memory of size [%d]\n",
    "%s : Start Timer Failed for Timer [%s]\n",
    "%s : Invalid index Received  [%s]\n",
    "%s : Unable to Allocate Hash Table of size [%d]\n",
    "%s : Unable to Allocate Timer Pool of size [%d]\n",

    "%s : Database has not been initialised.\n",
    "%s : Traces disabled.\n",
    "%s : Invalid trace level [%d]\n",
    "%s : Statistics disabled.\n",
    "%s : Invalid statistics type [%d]\n",
    "%s : Unable to send MSG/API [%s]\n",
    "%s : path failure happened\n",
	"%s : Init failure happened\n",
    "%s : Invalid Ip Traffic Class [%d]\n",
	"%s : Destination and source address mismatch\n"
	
};


S8bit const *egtpu_entity_name_arr[] = {
    " -0- ",
    "EGTPU_EI_UDP",
    "EGTPU_EI_RELAY",
    "EGTPU_EI_CP",
    "EGTPU_EI_LM",
    "EGTPU_EI_TS",
    "EGTPU_EI_UDP6",
    " -7- ",
    " -8- ",
    "EGTPU_EI_GTPU"
    
};

S8bit const *egtpu_udp_in_api_name_arr[] = {
    "EGTPU_UNITDATA_IND"
};

S8bit const *egtpu_udp_out_api_name_arr[] = {
    "EGTPU_UDP_UNITDATA_REQ"
};

S8bit const *egtpu_relay_in_api_name_arr[] = {
    "EGTPU_DATA_REQ",
    "EGTPU_RELOC_DATA_REQ"
#ifdef ENDC_ENABLED
    ,
    "EGTPU_X2U_DELIVERY_STATUS"
#endif
};

S8bit const *egtpu_relay_out_api_name_arr[] = {
    "EGTPU_RELAY_DATA_IND"
};

S8bit const *egtpu_cp_in_api_name_arr[] = {
    "EGTPU_SETUP_SAP_REQ",
    "EGTPU_RELEASE_SAP_REQ"
};

S8bit const *egtpu_cp_out_api_name_arr[] = {
    "EGTPU_CP_SETUP_SAP_CNF",
    "EGTPU_CP_RELEASE_SAP_CNF",
    "EGTPU_CP_ERROR_IND",
    "EGTPU_CP_PATH_FAILURE_IND"
};

S8bit const *egtpu_lm_in_api_name_arr[] = {
    "EGTPU_INITDB_REQ",
    "EGTPU_SET_TIMER_REQ",
    "EGTPU_GET_TIMER_REQ",
    "EGTPU_GET_MAX_RETRIES_REQ",
    "EGTPU_SET_MAX_RETRIES_REQ",
    "EGTPU_GET_STATS_REQ",
    "EGTPU_GET_INIT_STATS_REQ",
    "EGTPU_INIT_TRACE_LEVEL_REQ",
    "EGTPU_SET_TRACE_LEVEL_REQ",
    "EGTPU_GET_TRACE_LEVEL_REQ",
    "EGTPU_GET_CTXT_INFO_REQ",
	"EGTPU_MODIFY_REORDERING_IE_REQ"
};

S8bit const *egtpu_lm_out_api_name_arr[] = {
    "EGTPU_LM_INITDB_CNF",
    "EGTPU_LM_SET_TIMER_CNF",
    "EGTPU_LM_GET_TIMER_CNF",
    "EGTPU_LM_GET_MAX_RETRIES_CNF",
    "EGTPU_LM_SET_MAX_RETRIES_CNF",
    "EGTPU_LM_GET_STATS_CNF",
    "EGTPU_LM_GET_INIT_STATS_CNF",
    "EGTPU_LM_INIT_TRACE_LEVEL_CNF",
    "EGTPU_LM_SET_TRACE_LEVEL_CNF",
    "EGTPU_LM_GET_TRACE_LEVEL_CNF",
    "EGTPU_LM_GET_CTXT_INFO_CNF",
    "EGTPU_LM_ALARM_NOTIFICATION",
	"EGTPU_LM_MODIFY_REORDERING_IE_CNF"
};

S8bit const *egtpu_ts_in_api_name_arr[] = {
    "EGTPU_ECHO_TIMER_EXPIRY",
	"EGTPU_REORDER_TIMER_EXPIRY"
};

S8bit const **egtpu_in_api_name_arr[] = {
    EGTPU_NULL,                  /* self */
    egtpu_udp_in_api_name_arr,
    egtpu_relay_in_api_name_arr,
    egtpu_cp_in_api_name_arr,
    egtpu_lm_in_api_name_arr,
    egtpu_ts_in_api_name_arr,
    egtpu_udp_in_api_name_arr
      
};

S8bit const **egtpu_out_api_name_arr[] = {
    EGTPU_NULL,                  /* self */
    egtpu_udp_out_api_name_arr,
    egtpu_relay_out_api_name_arr,
    egtpu_cp_out_api_name_arr,
    egtpu_lm_out_api_name_arr,
    EGTPU_NULL ,                  /* timer */
    egtpu_udp_out_api_name_arr
};


S8bit const *egtpu_msg_name_arr[] = {
    "EGTPU_ECHO_REQUEST_MESSAGE",
    "EGTPU_ECHO_RESPONSE_MESSAGE",
    "EGTPU_ERROR_INDICATION_MESSAGE",
    "EGTPU_SUPP_EXTN_HDR_NOTIFICATION_MESSAGE",
    "EGTPU_END_MARKER_MESSAGE",
    "EGTPU_TPDU_MESSAGE"
};
#endif

S8bit const *egtpu_null_string = "";
S8bit const *egtpu_null_string_arr[] = {
    ""
};

/********************************************************************************
 * Function Name  : egtpu_get_msg_name
 * Inputs         : msg_type
 * Outputs        : None
 * Returns        : str - message name
 * Description    : This funtion provides string name for a given msg type.
 ********************************************************************************/
S8bit *egtpu_get_msg_name (U8bit msg_type)
{
    S8bit *str = (S8bit *) egtpu_null_string;

#if defined (EGTPU_ENABLE_TRACE)
    switch (msg_type) {
        case EGTPU_MSGT_ECHO_REQUEST:
            str = (S8bit *) egtpu_msg_name_arr[0];
            break;
        case EGTPU_MSGT_ECHO_RESPONSE:
            str = (S8bit *) egtpu_msg_name_arr[1];
            break;
        case EGTPU_MSGT_ERROR_INDICATION:
            str = (S8bit *) egtpu_msg_name_arr[2];
            break;
        case EGTPU_MSGT_SUPP_EXT_HDR_NOTI:
            str = (S8bit *) egtpu_msg_name_arr[3];
            break;
        case EGTPU_MSGT_END_MARKER:
            str = (S8bit *) egtpu_msg_name_arr[4];
            break;
        case EGTPU_MSGT_TPDU:
            str = (S8bit *) egtpu_msg_name_arr[5];
            break;
    }
#endif
    /* GCC_4.9_WARNINGS_FIX_START */
  LTE_GCC_UNUSED_PARAM(msg_type)
    /* GCC_4.9_WARNINGS_FIX_END */

    return str;
}

/********************************************************************************
 * Function Name  : egtpu_init_trace_level_req
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This funtion processes the EGTPU_INIT_TRACE_LEVEL_REQ API.
 ********************************************************************************/
egtpu_return_t egtpu_init_trace_level_req (egtpu_global_t *p_global,
											egtpu_info_t *p_egtpu_info)
{
    U8bit         *p_buf;
    egtpu_return_t ret_val;

    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_INIT_TRACE_LEVEL_REQ);

    EGTPU_GET_MSG_BUF (p_global,U8bit, p_buf, EGTPU_API_MSG_SIZE);

    /* Rel 5.3: Coverity 24404 Fix Start */
    if (EGTPU_NULL == p_buf)
    {
        /* Free the allocated Memory */
        EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);
        return EGTPU_FAILURE;
    }
    /* Rel 5.3: Coverity Fix End */

#if defined (EGTPU_ENABLE_TRACE)
    p_global->egtpu_trace_arr[0] = EGTPU_NULL;
    p_global->egtpu_trace_arr[1] = EGTPU_NULL;

    *(p_buf + EGTPU_API_HDR_LEN) = EGTPU_SUCCESS;
#else
    *(p_buf + EGTPU_API_HDR_LEN) = EGTPU_CERR_TRACES_DISABLED;
#endif

    EGTPU_SET_API_HDR (p_buf, EGTPU_EI_GTPU, EGTPU_EI_LM,
            EGTPU_LM_INIT_TRACE_LEVEL_CNF,
			(EGTPU_API_HDR_LEN + EGTPU_SIZE_OF_RESULT),
            EGTPU_NO_PAYLOAD, EGTPU_NO_PAYLOAD, EGTPU_API_HDR_LEN);

    EGTPU_SEND_FROM_GTPU (p_global,p_buf,
            (EGTPU_API_HDR_LEN + EGTPU_SIZE_OF_RESULT),
			EGTPU_EI_LM, ret_val, NULL);

    /* free the incoming request buffer */
    /* During Covering it is found that in success scenario also 
     * it should be freed */
    EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);
    EGTPU_FREE_MSG_BUF (p_buf);

    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_LM_INIT_TRACE_LEVEL_CNF);
    return ret_val;
}

/********************************************************************************
 * Function Name  : egtpu_set_trace_level_req
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This funtion processes the EGTPU_SET_TRACE_LEVEL_REQ API.
 ********************************************************************************/
egtpu_return_t egtpu_set_trace_level_req (egtpu_global_t *p_global,
											egtpu_info_t *p_egtpu_info)
{
    U8bit         *p_buf, *p_trav;
    U8bit         flag = *(p_egtpu_info->p_buff + EGTPU_API_HDR_LEN);

/* SPR 5599_128826 Fix Start */
#if defined (EGTPU_ENABLE_TRACE)
    U8bit         level = *(p_egtpu_info->p_buff + EGTPU_API_HDR_LEN +
							EGTPU_SIZE_OF_TRACE_FLAG);
#endif
/* SPR 5599_128826 Fix Start */

    egtpu_return_t ret_val;

    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_SET_TRACE_LEVEL_REQ);

    EGTPU_GET_MSG_BUF (p_global,U8bit, p_buf, EGTPU_API_MSG_SIZE);

    /* Rel 5.3: Coverity 24405 Fix Start */
    if (EGTPU_NULL == p_buf)
    {
        /* Free the allocated Memory */
        EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);
        return EGTPU_FAILURE;
    }
    /* Rel 5.3: Coverity Fix End */
    p_trav = p_buf + EGTPU_API_HDR_LEN;

#if defined (EGTPU_ENABLE_TRACE)
    if (level > EGTPU_DETAILED_TRACE_LEVEL)
	{
        *p_trav++ = EGTPU_CERR_INV_TRACE_LEVEL;
        EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_CERR_INV_TRACE_LEVEL,
                    egtpu_entity_name_arr[EGTPU_EI_GTPU], level));

        /* SPR 17747 FIX START */
        LOG_MSG( GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON,
                relayGtpuCurrentTick_g, __LINE__, EGTPU_CERR_INV_TRACE_LEVEL,
                level,0,0,0,0, __func__, egtpu_entity_name_arr[EGTPU_EI_GTPU] );
        /* SPR 17747 FIX END */
    }
	else
	{
        if (level == EGTPU_NO_TRACE_LEVEL)
		{
            U8bit i;

            /* Reset trace flags for all the levels */
            for (i = 0; i < EGTPU_MAX_TRACE_LEVELS; i++)
                p_global->egtpu_trace_arr[i] &= ~flag;
        }
		else
		{ /* Set the traces for the desired level */
            p_global->egtpu_trace_arr[level - 1] |= flag;

            /* Reset the traces for the other level */
            p_global->egtpu_trace_arr [(level == EGTPU_BRIEF_TRACE_LEVEL) ?
                (EGTPU_DETAILED_TRACE_LEVEL - 1) :
                (EGTPU_BRIEF_TRACE_LEVEL - 1)] &= ~flag;
        }

        *p_trav++ = EGTPU_SUCCESS;
    }
#else
    *p_trav++ = EGTPU_CERR_TRACES_DISABLED;
#endif

    *p_trav++ = flag;

    EGTPU_SET_API_HDR (p_buf, EGTPU_EI_GTPU, EGTPU_EI_LM,
            EGTPU_LM_SET_TRACE_LEVEL_CNF, (p_trav - p_buf),
            EGTPU_NO_PAYLOAD, EGTPU_NO_PAYLOAD, (p_trav - p_buf - 1));

    EGTPU_SEND_FROM_GTPU (p_global,p_buf, (p_trav - p_buf), EGTPU_EI_LM,\
							ret_val, NULL);

    /* free the incoming API buffer */
    /* During Covering it is found that in success scenario also 
     * it should be freed */
    EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);
    EGTPU_FREE_MSG_BUF (p_buf);

    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_LM_SET_TRACE_LEVEL_CNF);
    return ret_val;
}

/********************************************************************************
 * Function Name  : egtpu_get_trace_level_req
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 * Outputs        : None
 * Returns        : EGTPU_SUCCESS/EGTPU_FAILURE
 * Description    : This funtion processes the EGTPU_GET_TRACE_LEVEL_REQ API.
 ********************************************************************************/
egtpu_return_t egtpu_get_trace_level_req (egtpu_global_t *p_global,
											egtpu_info_t *p_egtpu_info)
{
    U8bit         flag = *(p_egtpu_info->p_buff + EGTPU_API_HDR_LEN);
    U8bit         level = EGTPU_NO_TRACE_LEVEL;
    /* +COVERITY 32537 */
    U8bit         *p_buf = EGTPU_NULL;
    U8bit         *p_trav =EGTPU_NULL;
    /* +COVERITY 32537 */
    egtpu_return_t ret_val;

    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_GET_TRACE_LEVEL_REQ);

    EGTPU_GET_MSG_BUF (p_global,U8bit, p_buf, EGTPU_API_MSG_SIZE);

    /* Rel 5.3: Coverity 24403 Fix Start */
    if (EGTPU_NULL == p_buf)
    {
        /* Free the allocated Memory */
        EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);
        return EGTPU_FAILURE;
    }
    /* Rel 5.3: Coverity Fix End */

    p_trav = p_buf + EGTPU_API_HDR_LEN;

#if defined (EGTPU_ENABLE_TRACE)
    if (p_global->egtpu_trace_arr[EGTPU_DETAILED_TRACE_LEVEL - 1] & flag)
        level = EGTPU_DETAILED_TRACE_LEVEL;
    else if (p_global->egtpu_trace_arr[EGTPU_BRIEF_TRACE_LEVEL - 1] & flag)
        level = EGTPU_BRIEF_TRACE_LEVEL;

    *p_trav++ = EGTPU_SUCCESS;
#else
    *p_trav++ = EGTPU_CERR_TRACES_DISABLED;
#endif

    *p_trav++ = flag;

    if (*(p_buf + EGTPU_API_HDR_LEN) == EGTPU_SUCCESS)
        *p_trav++ = level;

    EGTPU_SET_API_HDR (p_buf, EGTPU_EI_GTPU, EGTPU_EI_LM,
            EGTPU_LM_GET_TRACE_LEVEL_CNF, (p_trav - p_buf),
            EGTPU_NO_PAYLOAD, EGTPU_NO_PAYLOAD, (p_trav - p_buf - 1));

    EGTPU_SEND_FROM_GTPU (p_global,p_buf, (p_trav - p_buf), EGTPU_EI_LM, \
							ret_val, NULL);

    /* free the incoming API buffer */
    /* During Covering it is found that in success scenario also 
     * it should be freed */ 
    EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);
    EGTPU_FREE_MSG_BUF (p_buf);


    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_LM_GET_TRACE_LEVEL_CNF);
    return ret_val;
}
