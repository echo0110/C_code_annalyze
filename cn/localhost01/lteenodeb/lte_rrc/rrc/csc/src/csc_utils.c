/******************************************************************************
*
*   FILE NAME:
*       csc_utils.c
*
*   DESCRIPTION:
*       This header file contains implementation of CSC specific utilities
*
*   DATE            AUTHOR      REFERENCE       REASON
*   02 June 2009    VasylS      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#include "csc_utils.h"
#include "csc_logger.h"
#include "rrc_common_utils.h"
#include "rrc_msg_mgmt.h"
#include "rrc_ext_api.h"
#include "rrc_rrm_il_composer.h"
#include "rrc_s1ap_csc_intf.h"
#include "rrc_intrl_api.h"

/*global variables*/
csc_cell_ctx_t  *gp_cell_ctx = PNULL;
timespec_t timestamp;
int sfn_sf_available = RRC_FALSE;
int cdma_sys_time_available = RRC_FALSE;
int sfn = 0, sf = 0;
extern pthread_mutex_t region_mutex;
extern pthread_cond_t sfn_sf_data_available;
extern pthread_cond_t cdma_time_data_available;

sib8_sfn_list_t sib8_sfn_list;

void get_sys_time(sys_time_t *sys_time);
U64 diffInMicroSec(struct timespec *  start, struct timespec *  end);


/******************************************************************************
*   FUNCTION NAME: csc_send_rrm_cell_setup_resp
*   INPUT        : rrc_transaction_id_t    transaction_id
*                  U8                      cell_index
*                  rrc_return_et           response
*                  rrm_fail_cause_et       fail_cause
*   OUTPUT       : None
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
*   DESCRIPTION  : Send RRC_RRM_CELL_SETUP_RESP from CSC to RRM
******************************************************************************/
rrc_return_et csc_send_rrm_cell_setup_resp
(
    rrc_transaction_id_t    transaction_id,
    U8                      cell_index,
    rrc_return_et           response,
    rrm_fail_cause_et       fail_cause
)
{
    rrc_rrm_cell_setup_resp_t   cell_setup_resp;
    rrc_return_et               res = RRC_FAILURE;

    RRC_CSC_UT_TRACE_ENTER();

    memset_wrapper(&cell_setup_resp, RRC_NULL, sizeof(rrc_rrm_cell_setup_resp_t));

    /* Populate the cell setup response message */
    if (RRC_FAILURE == response)
    {
        cell_setup_resp.presence_bitmask    =
            CELL_SETUP_RESP_API_FAIL_CAUSE_PRESENCE_FLAG;
        cell_setup_resp.fail_cause          = fail_cause;
    }
    cell_setup_resp.cell_index          = cell_index;
    cell_setup_resp.response            = response;


    /* Send message */
    res = rrc_rrm_il_send_rrc_rrm_cell_setup_resp(&cell_setup_resp,
        RRC_CSC_MODULE_ID,RRC_RRM_MODULE_ID, transaction_id, cell_index);

#ifdef MEM_PROFILE_ON
    fprintf_wrapper(stderr,"\n *************** cell setup \n ");
    qvPoolStats();
#endif

    RRC_CSC_UT_TRACE_EXIT();
    return res;
}
/******************************************************************************
*   FUNCTION NAME: csc_send_rrm_cell_del_resp_failure
*   INPUT        : rrc_transaction_id_t    transaction_id
*                  U8                      cell_index
*                  rrc_return_et           response
*                  rrm_fail_cause_et       fail_cause
*   OUTPUT       : None
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
*   DESCRIPTION  : Send failure RRC_RRM_CELL_DELETE_RESP from CSC to RRM
******************************************************************************/
rrc_return_et csc_send_rrm_cell_del_resp_failure
(
    rrc_transaction_id_t    transaction_id,
    U8                      cell_index,
    rrc_return_et           response,
    rrm_fail_cause_et       fail_cause
)
{
    rrm_rrc_cell_del_resp_t   cell_del_resp;
    rrc_return_et               res = RRC_FAILURE;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(RRC_FAILURE == response);

    /* Populate the cell setup response message */
    cell_del_resp.presence_bitmask    =
        CELL_DEL_RESP_API_FAIL_CAUSE_PRESENCE_FLAG;
    cell_del_resp.cell_index          = cell_index;
    cell_del_resp.response            = response;
    cell_del_resp.fail_cause          = fail_cause;

    /* Send message */
    res = rrc_rrm_il_send_rrm_rrc_cell_del_resp(&cell_del_resp,
        RRC_CSC_MODULE_ID,RRC_RRM_MODULE_ID, transaction_id,cell_index);

    RRC_CSC_UT_TRACE_EXIT();
    return res;
}

/******************************************************************************
*   FUNCTION NAME: csc_send_rrm_updated_pws_si_list_cnf
*   INPUT        : csc_cell_ctx_t          *p_cell_ctx
*                  rrc_return_et           response
*   OUTPUT       : None
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
*   DESCRIPTION  : Send failure RRC_RRM_UPDATED_PWS_SI_LIST_CNF from CSC to 
*                  RRM
******************************************************************************/
rrc_return_et csc_send_rrm_updated_pws_si_list_cnf
(
    csc_cell_ctx_t          *p_cell_ctx,
    rrc_return_et           response
)
{
    rrc_rrm_updated_pws_si_list_cnf_t   
        updated_pws_si_list_cnf;
    rrc_return_et               res = RRC_FAILURE;

    RRC_CSC_UT_TRACE_ENTER();
    RRC_ASSERT (PNULL != p_cell_ctx);

    /* Build  the cell Reconfig response message */
    updated_pws_si_list_cnf.cell_index = p_cell_ctx->cell_index;
    updated_pws_si_list_cnf.response = response;
    updated_pws_si_list_cnf.warning_info_list = p_cell_ctx->warning_info_list;
    /* Send message */
    res = rrc_rrm_il_send_rrc_rrm_updated_pws_si_list_cnf(
            &updated_pws_si_list_cnf,
            RRC_CSC_MODULE_ID, RRC_RRM_MODULE_ID, p_cell_ctx->transaction_id,
            p_cell_ctx->cell_index);

    RRC_CSC_UT_TRACE_EXIT();
    return res;
}

/******************************************************************************
*   FUNCTION NAME: csc_send_rrm_cell_reconfig_resp_failure
*   INPUT        : rrc_transaction_id_t    transaction_id
*                  U8                      cell_index
*                  rrc_return_et           response
*                  rrm_fail_cause_et       fail_cause
*   OUTPUT       : None
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
*   DESCRIPTION  : Send failure RRC_RRM_CELL_RECONFIG_RESP from CSC to RRM
*
******************************************************************************/
rrc_return_et csc_send_rrm_cell_reconfig_resp_failure
(
    rrc_transaction_id_t    transaction_id,
    U8                      cell_index,
    rrc_return_et           response,
    rrm_fail_cause_et       fail_cause
)
{
    rrc_rrm_cell_reconfig_resp_t   cell_reconfig_resp;
    rrc_return_et               res = RRC_FAILURE;

    RRC_CSC_UT_TRACE_ENTER();

    /* Build  the cell Reconfig response message */
    cell_reconfig_resp.presence_bitmask    =
        CELL_RECONFIG_RESP_API_FAIL_CAUSE_PRESENCE_FLAG;
    cell_reconfig_resp.cell_index          = cell_index;
    cell_reconfig_resp.response            = response;
    cell_reconfig_resp.fail_cause          = fail_cause;

    /* Send message */
    res = rrc_rrm_il_send_rrc_rrm_cell_reconfig_resp(&cell_reconfig_resp,
        RRC_CSC_MODULE_ID,RRC_RRM_MODULE_ID, transaction_id, cell_index);

    RRC_CSC_UT_TRACE_EXIT();
    return res;
}
/******************************************************************************
*   FUNCTION NAME: csc_alloc_intrl_msg
*   INPUT        : rrc_module_id_t             dst_module_id
*                  U16                         api_id 
*                  rrc_size_t                  msg_size
*   OUTPUT       : None
*   RETURNS      : PNULL in failure case
*                  pointer on the very begining of message
*   DESCRIPTION  : Allocate memory and fill RRC API Header for sending message from CSC.
*                  Is used for sending messages to other RRC submodules.
******************************************************************************/
void* csc_alloc_intrl_msg
(
    /* Destination module identifier */
    rrc_module_id_t             dst_module_id,
    U16                         api_id,         /* API Id */
    rrc_size_t                  msg_size        /* Size of message (payload) */
)
{
    void *p_api = PNULL;
    rrc_size_t msg_api_length = (rrc_size_t)(msg_size + RRC_API_HEADER_SIZE);

    RRC_CSC_UT_TRACE_ENTER();

    /* Allocate buffer */
    p_api = rrc_msg_mem_get(msg_api_length);
    if (PNULL != p_api)
    {
        memset_wrapper(p_api, 0, msg_api_length);

        /* Fill RRC header */
        rrc_construct_api_header(   p_api, RRC_VERSION_ID, RRC_CSC_MODULE_ID,
                                    dst_module_id, api_id, (U16)msg_api_length);
    }

    RRC_CSC_UT_TRACE_EXIT();
    return p_api;
}

/******************************************************************************
*   FUNCTION NAME: csc_send_s1ap_pws_resp_failure
*   INPUT        : rrc_cell_index_t        cell_index
*                  rrc_transaction_id_t    trans_id
*                  rrc_return_et           response
*                  rrm_fail_cause_et       fail_cause
*   OUTPUT       : None
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
*   DESCRIPTION  : Send failure S1AP_PWS_RESP from CSC to S1AP(eNB
*                  Manager)
******************************************************************************/
void csc_send_s1ap_pws_resp_failure
(
    rrc_cell_index_t        cell_index,
    rrc_transaction_id_t    trans_id,
    rrc_return_et           response,
    rrm_fail_cause_et       fail_cause
)
{
    void *p_intrl_msg = PNULL;
    
    s1ap_pws_resp_t *p_s1ap_pws_resp = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    p_intrl_msg = csc_alloc_intrl_msg(RRC_S1AP_MODULE_ID,
            S1AP_PWS_RESP,
            sizeof(s1ap_pws_resp_t)); 
    if(PNULL == p_intrl_msg)
    {
        RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
        return;
    }

    if (PNULL == p_intrl_msg )
    {
        RRC_CSC_TRACE(RRC_WARNING, "Memory Allocation failed");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }
    
    p_s1ap_pws_resp = 
        (s1ap_pws_resp_t*)
        ((U8 *)p_intrl_msg +
         RRC_API_HEADER_SIZE);

    if (PNULL == p_s1ap_pws_resp )
    {
        RRC_CSC_TRACE(RRC_WARNING, " Invalid Response ");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }

    memset_wrapper(p_s1ap_pws_resp, 0, sizeof(s1ap_pws_resp_t));

    p_s1ap_pws_resp->transaction_id = trans_id;
    
    p_s1ap_pws_resp->cell_index = cell_index;


    p_s1ap_pws_resp->pws_response = response;
    
    p_s1ap_pws_resp->bitmask |= PWS_RESP_FAIL_CAUSE_PRESENT;
    
    p_s1ap_pws_resp->pws_fail_cause = 
        fail_cause;


    rrc_send_message(p_intrl_msg,
            RRC_S1AP_MODULE_ID);

    RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: csc_send_s1ap_kill_resp_failure
*   INPUT        : rrc_cell_index_t        cell_index
*                  rrc_transaction_id_t    trans_id
*                  rrc_return_et           response
*                  rrc_kill_fail_cause_et  fail_cause
*   OUTPUT       : None
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
*   DESCRIPTION  : Send failure S1AP_KILL_RESP from CSC to S1AP(eNB
*                  Manager)
******************************************************************************/
void csc_send_s1ap_kill_resp_failure
(
    rrc_cell_index_t        cell_index,
    rrc_transaction_id_t    trans_id,
    rrc_return_et           response,
/* SPR 5688 Start */
    rrc_kill_fail_cause_et  fail_cause
/* SPR 5688 End */
)
{
    void *p_intrl_msg = PNULL;
    
    s1ap_kill_resp_t *p_s1ap_kill_resp = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    p_intrl_msg = csc_alloc_intrl_msg(RRC_S1AP_MODULE_ID,
            S1AP_KILL_RESP,
            sizeof(s1ap_kill_resp_t)); 
    if(PNULL == p_intrl_msg)
    {
        RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
        return;
    }

    if (PNULL == p_intrl_msg )
    {
        RRC_CSC_TRACE(RRC_WARNING, "Memory Allocation failed");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }
    
    p_s1ap_kill_resp = 
        (s1ap_kill_resp_t*)
        ((U8 *)p_intrl_msg +
         RRC_API_HEADER_SIZE);
    if(PNULL == p_s1ap_kill_resp)
    {
        RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
        return;
    }

    if (PNULL == p_s1ap_kill_resp )
    {
        RRC_CSC_TRACE(RRC_WARNING, " Invalid Response ");
        RRC_CSC_UT_TRACE_EXIT();
        return;
    }

    memset_wrapper(p_s1ap_kill_resp, 0, sizeof(s1ap_kill_resp_t));

    p_s1ap_kill_resp->transaction_id = trans_id;
    
    p_s1ap_kill_resp->cell_index = cell_index;


    p_s1ap_kill_resp->kill_response = response;
    
    p_s1ap_kill_resp->bitmask |= KILL_RESP_FAIL_CAUSE_PRESENT;
    
    p_s1ap_kill_resp->kill_fail_cause = 
        fail_cause;


    rrc_send_message(p_intrl_msg,
            RRC_S1AP_MODULE_ID);

    RRC_CSC_UT_TRACE_EXIT();
}

/***************************************************************************************
 *   FUNCTION NAME : csc_send_rrm_cell_start_resp 
 *   INPUT         : rrc_transaction_id_t    transaction_id
 *                   U8                      cell_index
 *                   rrc_return_et           response
 *                   rrm_fail_cause_et       fail_cause
 *   OUTPUT        : None
 *   RETURNS       : RRC_SUCCESS/RRC_FAILURE
 *   DESCRIPTION   : Send failure RRC_RRM_CELL_START_RESP from CSC to RRM
 **************************************************************************************/
rrc_return_et csc_send_rrm_cell_start_resp
(
    rrc_transaction_id_t    transaction_id,
    U8                      cell_index,
    rrc_return_et           response,
    rrm_fail_cause_et       fail_cause
)
{
    rrc_rrm_cell_start_resp_t   *p_cell_start = PNULL; 
    rrc_return_et               result        = RRC_FAILURE;

    RRC_CSC_UT_TRACE_ENTER();
    p_cell_start = (rrc_rrm_cell_start_resp_t *)
        rrc_mem_get(sizeof(rrc_rrm_cell_start_resp_t));
    
    if (PNULL == p_cell_start)
    {
        RRC_CSC_TRACE(RRC_FATAL,"Unable to allocate Memeory");
        RRC_CSC_UT_TRACE_EXIT();
        return result;
    }

    memset_wrapper(p_cell_start, 0, sizeof(rrc_rrm_cell_start_resp_t));
    
    p_cell_start->cell_index        = cell_index;
    p_cell_start->response          = response;

    if (fail_cause != RRM_RRC_NO_ERROR)
    {
        p_cell_start->presence_bitmask |= 
            CELL_START_RESP_API_FAIL_CAUSE_PRESENCE_FLAG;
        p_cell_start->fail_cause     = fail_cause;
    }

    /* Send Message */
    result = rrc_rrm_il_send_rrc_rrm_cell_start_resp(p_cell_start,
           RRC_CSC_MODULE_ID,RRC_RRM_MODULE_ID,transaction_id,
           cell_index);

    /* coverity_fix_63309_start */
    if ( PNULL != p_cell_start )
    {
        rrc_mem_free(p_cell_start);
        p_cell_start = PNULL;
    }
    /* coverity_fix_63309_stop */
    RRC_CSC_UT_TRACE_EXIT();
    return result;
}

/************************************************************************************
 *   FUNCTION NAME : csc_send_rrm_cell_stop_resp
 *   INPUT         : rrc_transaction_id_t    transaction_id
 *                   U8                      cell_index
 *                   rrc_return_et           response
 *                   rrm_fail_cause_et       fail_cause
 *   OUTPUT        : None
 *   RETURNS       : RRC_SUCCESS/RRC_FAILURE
 *   DESCRIPTIOPN  : Send failure RRC_RRM_CELL_STOP_CNF from CSC to RRM
 *************************************************************************************/ 
rrc_return_et csc_send_rrm_cell_stop_resp
(
    rrc_transaction_id_t    transaction_id,
    U8                      cell_index,
    rrc_return_et           response,
    rrm_fail_cause_et       fail_cause
)
{
    rrc_rrm_cell_stop_resp_t   *p_cell_stop = PNULL; 
    rrc_return_et             result     = RRC_FAILURE;

    RRC_CSC_UT_TRACE_ENTER();

    p_cell_stop = (rrc_rrm_cell_stop_resp_t *)
        rrc_mem_get(sizeof( rrc_rrm_cell_stop_resp_t));
    if(PNULL == p_cell_stop)
    {
        RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
        return RRC_FAILURE;
    }

    memset_wrapper(p_cell_stop, 0, sizeof( rrc_rrm_cell_stop_resp_t));

    p_cell_stop->cell_index     = cell_index;
    p_cell_stop->response       = response;

    if (fail_cause != RRM_RRC_NO_ERROR)
    {
        p_cell_stop->presence_bitmask |= 
            CELL_STOP_RESP_API_FAIL_CAUSE_PRESENCE_FLAG;
        p_cell_stop->fail_cause     = fail_cause;
    }

    /* Send Message */
    result = rrc_rrm_il_send_rrc_rrm_cell_stop_resp(p_cell_stop,
           RRC_CSC_MODULE_ID,RRC_RRM_MODULE_ID, transaction_id,
           cell_index);
    /* Coverity Fix 63310 Start */
    if ( PNULL != p_cell_stop )
    {
        rrc_mem_free(p_cell_stop);
        p_cell_stop = PNULL;
    }
    /* Coverity Fix 63310 Stop */
    RRC_CSC_UT_TRACE_EXIT();
    return result;
}


/*****************************************************************************
 * Function Name  : diffInMicroSec
 * Inputs         : start - timespec record
 *                  end   - timespec record
 * Outputs        : none
 * Returns        : time difference in microseconds.
 * Description    : The function returs difference of start and end 
 *                  in microseconds.
 *****************************************************************************/
U64 diffInMicroSec(struct timespec *  start, struct timespec *  end)
{
    U64 diffInMicroseconds = 0;
    RRC_CSC_UT_TRACE_ENTER();

    if (end->tv_sec > start->tv_sec )
    {
        diffInMicroseconds = ((U64)(end->tv_sec) - (U64)(start->tv_sec) ) * 1000000;
    }

    if (start->tv_nsec > end->tv_nsec)
    {
        diffInMicroseconds -= ((U64)(start->tv_nsec) - (U64)(end->tv_nsec))/1000;
    }
    else
    {
        diffInMicroseconds += ((U64)(end->tv_nsec) - (U64)(start->tv_nsec))/1000;
    }

    RRC_CSC_UT_TRACE_EXIT();
    return (diffInMicroseconds);
}


/******************************************************************************
 *   FUNCTION NAME: get_next_sfn_for_sib8
 *   INPUT        : U16 curr_sfn
 *                  U16 sib8_periodicity
 *   OUTPUT       : None
 *   RETURN       :  none
 *   DESCRIPTION  : to retrieve the next sfn for sib8 to be broadcasted
 ******************************************************************************/
U16 get_next_sfn_for_sib8(U16 curr_sfn, U16 sib8_periodicity)
{
    U16 ctr = 0;
    U16 temp_curr_sfn = curr_sfn;
    U16 sfn_diff = 0;
    for(ctr=0; ctr < sib8_sfn_list.num_of_elements; ctr++)
    {
        if(curr_sfn > sib8_sfn_list.sib8_sfn[ctr])
        {
            if((curr_sfn - sib8_sfn_list.sib8_sfn[ctr]) <= sib8_periodicity)
            {
                break;
            }
        }
        else if(sib8_sfn_list.sib8_sfn[ctr] > curr_sfn)
        {
            if((sib8_sfn_list.sib8_sfn[ctr] - curr_sfn) <= sib8_periodicity)
            {
                break;
            }
        }
        else
        {
            break;
	}
    }
    ctr++; /*as indexes are started from 0 index and num of elements will be 1 extra*/

    while(temp_curr_sfn != sib8_sfn_list.sib8_sfn[ctr])
    {
	    temp_curr_sfn = ((temp_curr_sfn+1)%SFN_UPPER_LIMIT);
	    sfn_diff++;
    }

    /*take next SFN if sufficient L2 processing
     * guard period is not selected*/
    if (0 != sib8_sfn_list.num_of_elements)
    {
        if(sfn_diff < GUARD_PERIOD_IN_SFN)
            ctr = (ctr+1)% sib8_sfn_list.num_of_elements;
    }

    return sib8_sfn_list.sib8_sfn[ctr];
}


/******************************************************************************
 *   FUNCTION NAME: cdma_sys_time_caliberate_func
 *   INPUT        : void *p
 *   OUTPUT       : None
 *   RETURNS      : None
 *   DESCRIPTION  : This function is used to caliberate the CDMA System Time according to 
 *                  current SFN, SF, Timestamp and CDMA Time at current timestamp
 ******************************************************************************/
void * cdma_sys_time_caliberate_func(void *p)
{
    (void)p;/*SPR 17777 +-*/
    rrc_sfn_t l2_sfn = 0, sfn_elapsed = 0, sfn_at_curr_timestamp = 0;
    rrc_sfn_t next_sib8_sfn = 0;
    rrc_sf_t  l2_sf = 0, sf_elapsed = 0, sf_at_curr_timestamp = 0;
    sys_time_t sys_time;
    U32 time_elapsed = 0; /*unit is mili-seconds*/
    timespec_t l2_timestamp = {0};
    U64       cdma_time_microsec = 0;
    U64       timestamp_elapsed_msec = 0;
    U64       timestamp_elapsed_microsec = 0;
    U64       cdma_time_elapsed_microsec = 0;
    /* SPR 11229 Fix Start */
    rrc_sfn_t just_sent_sib8_sfn = 0;
    /* SPR 11229 Fix Stop */

    SET_CELL_AND_UE_INDEX(0xff, 0xffff);

    while(1)
    {
        /*Step1: Use SFN, SF & Timestamp reported by L2*/
        pthread_mutex_lock_wrapper(&region_mutex);
        if (sfn_sf_available == RRC_FALSE) {
            pthread_cond_wait_wrapper(&sfn_sf_data_available,&region_mutex);
        }
        l2_sfn = gp_cell_ctx->sfn;
        l2_sf = gp_cell_ctx->sf;
        l2_timestamp.tv_sec = gp_cell_ctx->timestamp.ts_sec;
        l2_timestamp.tv_nsec = gp_cell_ctx->timestamp.ts_nsec;

#ifdef RRC_DEBUG
        RRC_CSC_TRACE(RRC_DETAILED,"Used SFN:%d, SF:%d, l2_timestamp = [%llu]sec [%u]nsec",
                               gp_cell_ctx->sfn, gp_cell_ctx->sf,
                               gp_cell_ctx->timestamp.ts_sec, gp_cell_ctx->timestamp.ts_nsec);
#endif

        sfn_sf_available = RRC_FALSE;

        /* Sending signal to thread running CSC module that SFN,SF & timestamp 
         * given by L2 has been consumed used*/

        /*Step2: Generate CDMA System Time for first SIB8 segment*/

        /*Getting CDMA System Time and current timestamp from the platform using
         * ported system call get_cdma_sys_time*/
        get_sys_time(&sys_time);
        /*cdma_fix bug 10186 start*/
        /*calculate CDMA System time in msec*/
        cdma_time_microsec = (((U64)(sys_time.cdma_time.tv_sec)*1000000) + ((U64)(sys_time.cdma_time.tv_nsec)/1000));

        /*get Difference in L2 reported timestamp and current system timestamp in microseconds*/
        timestamp_elapsed_microsec = diffInMicroSec(&(l2_timestamp), &(sys_time.timestamp));
        timestamp_elapsed_msec = timestamp_elapsed_microsec/1000;
        /*cdma_fix bug 10186 stop*/

#ifdef RRC_DEBUG
        RRC_CSC_TRACE(RRC_INFO, "Received from Platform:CDMA_Sys_Time:[%llu]msec", cdma_time_microsec);
        RRC_TRACE(RRC_INFO,"Timestamp Elapsed in microseconds [%llu]microsec",timestamp_elapsed_microsec);
        RRC_TRACE(RRC_INFO,"Timestamp Elapsed in milliseconds [%llu]msec",timestamp_elapsed_msec);
#endif

        /*calculating the SFNs & SFs incremented between L2 reported timestamp and
         * current timestamp */
        sfn_elapsed = timestamp_elapsed_msec/SF_MAX_LIMIT; /*1 SFN = 10msec*/
        sf_elapsed = timestamp_elapsed_msec%SF_MAX_LIMIT;  /*10 SFs makes 1 SF*/
#ifdef RRC_DEBUG
        RRC_CSC_TRACE(RRC_ERROR,"sfn_elapsed =[%d]  sf_elapsed =[%d]",
                sfn_elapsed, sf_elapsed);
#endif

        /*Calculating the Current SFN by adding SFN_lapsed between L2 reported
         * timestamp and current timestamp*/
        sfn_at_curr_timestamp = (l2_sfn + sfn_elapsed)%SFN_UPPER_LIMIT;

        /*If SF Lapsed exceeds 10, then 1 SFN will be added */
        if((l2_sf+sf_elapsed) >= SF_MAX_LIMIT){
            sfn_at_curr_timestamp += 1;
        }

        /*Calculating the current SF by adding sf_elapsed between L2 reported
         * timestamp and current timestamp*/
        sf_at_curr_timestamp = (l2_sf + sf_elapsed)%SF_MAX_LIMIT;
#ifdef RRC_DEBUG
        RRC_CSC_TRACE(RRC_INFO,"At SFN/SF:[%d/%d] CDMA_Sys_Time:[%llu]msec",
                sfn_at_curr_timestamp, sf_at_curr_timestamp, cdma_time_microsec);
#endif

        /* SPR 11229 Fix Start */
        if ( gp_cell_ctx->sib8_periodicity > 0 )
        {
            just_sent_sib8_sfn = sfn_at_curr_timestamp - 
                   (sfn_at_curr_timestamp % gp_cell_ctx->sib8_periodicity);
            if ( gp_cell_ctx->last_sent_sib8_sfn == LAST_SENT_SIB8_SFN_UNINIT )
            {
                next_sib8_sfn = (just_sent_sib8_sfn + gp_cell_ctx->sib8_periodicity)%SFN_UPPER_LIMIT;

                RRC_CSC_TRACE(RRC_DETAILED,"Unitinilized last_sent_sib8_sfn case: " 
                                           "last_sent_sib8_sfn[%d] next_sib8_sfn[%d]",
                                       gp_cell_ctx->last_sent_sib8_sfn , next_sib8_sfn);
            }
            else
            {
                next_sib8_sfn = ( gp_cell_ctx->last_sent_sib8_sfn + (MAX_SIB8_INSTANCES * 
                                             gp_cell_ctx->sib8_periodicity) )%SFN_UPPER_LIMIT ;

                gp_cell_ctx->last_sent_sib8_sfn = next_sib8_sfn;

                RRC_CSC_TRACE(RRC_DETAILED,"NEW: Initialized last_sent_sib8_sfn[%d]" 
                                           "gp_cell_ctx->start_sib8_sfn [%d]",
                                           gp_cell_ctx->last_sent_sib8_sfn , 
                                           gp_cell_ctx->start_sib8_sfn);

            }
        }
        else
        {
            RRC_CSC_TRACE(RRC_ERROR,"SIB8 Periodicity[%d] CAN'T BE ZERO", 
                                            gp_cell_ctx->sib8_periodicity );
        }
        next_sib8_sfn++; /*Because we've to broadcast CDMA System Time present at SFN boundary*/

        next_sib8_sfn = (next_sib8_sfn + gp_cell_ctx->sib8_si_window_offset)% SFN_UPPER_LIMIT;
        /* SPR 11229 Fix Stop */

        if(next_sib8_sfn > sfn_at_curr_timestamp) /*for e.g. next_sib8_sfn is 9.0 and current is 2.3*/
        {
            sfn_elapsed = next_sib8_sfn - sfn_at_curr_timestamp;
            time_elapsed = (sfn_elapsed*SF_MAX_LIMIT) - sf_at_curr_timestamp;
        } /*cdma_fix bug 10186 start*/
        else /*SFN wraparound case*/ /*for e.g. next_sib8_sfn is 1.0 and current is 1020.3*/
        {
            sfn_elapsed = SFN_UPPER_LIMIT - sfn_at_curr_timestamp;
            time_elapsed = ((sfn_elapsed + next_sib8_sfn)*SF_MAX_LIMIT) - sf_at_curr_timestamp;
        }
        /*cdma_fix bug 10186 stop */

#ifdef RRC_DEBUG
        RRC_CSC_TRACE(RRC_ERROR,"old cdma_base_time:[%llu]msec", gp_cell_ctx->cdma_base_time);
#endif
        /*cdma_fix bug 10186 start*/
        /*adding the time_elapsed difference in cdma system time after
         * converting it to microseconds by multiplying wiht 1000*/
	/* Coverity Fix 83356 Start */
        cdma_time_elapsed_microsec = cdma_time_microsec + (((U64)time_elapsed)*1000);
	/* Coverity Fix 83356 Stop */
        /*cdma_fix bug 10186 stop*/
        /*Updating the same in cell context so that thread running csc module
         * can start processing on cdma_base_time to calculate the next 
         * MAX_SIB8_INSTANCES instances*/
        gp_cell_ctx->cdma_base_time = cdma_time_elapsed_microsec;

#ifdef RRC_DEBUG
        RRC_CSC_TRACE(RRC_INFO,"new cdma_base_time:[%llu]msec", gp_cell_ctx->cdma_base_time);
#endif
        cdma_sys_time_available = RRC_TRUE;

        /* Sending signal to thread running CSC module that CDMA Timestamp is
         * available in cell_ctx */
        pthread_cond_signal_wrapper(&cdma_time_data_available);
        pthread_mutex_unlock_wrapper(&region_mutex);
    }
}
