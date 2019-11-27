/*******************************************************************************
*
*  FILE NAME   : x2ap_s1ap.c
*
*  DESCRIPTION : This file contains the function definition of 
*
*  DATE 29-Jan-2013  NAME Simarjot Kaur  REFERENCE  REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/

#include "commoninc.h"
#include "x2ap_db.h"
#include "rrc_x2apOam_intf.h"
#include "x2ap_utils.h"
#include "x2ap_api.h"
#include "sm_entry.h"
#include "rrc_x2apS1ap_il_parser.h"
#include "rrc_x2apOam_il_parser.h"
#include "rrc_msg_mgmt.h"
#include "rrc_intrl_api.h"
#include "rrc_common_utils.h"

/*****************************************************************************
 * Function Name  : x2ap_build_and_send_s1ap_comm_info 
 * Inputs         : x2ap_gb_context_t - pointer to X2AP global context 
 * Outputs        : None
 * Returns        : X2AP_FAILURE/X2AP_SUCCESS 
 * Description    : This function builds and sends x2ap_s1ap_comm_info to S1AP 
 *      
 ****************************************************************************/
x2ap_return_et x2ap_build_and_send_s1ap_comm_info(
        x2ap_gb_context_t *p_x2ap_gb_ctx)
{
    x2ap_s1ap_comm_info_t    *msg_p = PNULL;
    void *p_intrl_msg = PNULL;

    X2AP_UT_TRACE_ENTER();
    X2AP_ASSERT(p_x2ap_gb_ctx != X2AP_P_NULL);

    p_intrl_msg = x2ap_alloc_intrl_msg(
            RRC_S1AP_MODULE_ID,
            X2AP_S1AP_COMM_INFO,
            (U16)(sizeof(x2ap_s1ap_comm_info_t)));

    if (PNULL == p_intrl_msg)
    {
        /*out of memory*/
        X2AP_UT_TRACE_EXIT();
        return X2AP_FAILURE;
    }
    msg_p = (x2ap_s1ap_comm_info_t *)((U8 *)p_intrl_msg +
            RRC_API_HEADER_SIZE);

    /* Save local ENB Communication Info in Global DB */
    X2AP_MEMCPY ((void *)(&(msg_p->local_enb_info)),
                (void*)(&(p_x2ap_gb_ctx->local_enb_info)),
                sizeof(x2_enb_comm_info_t) );

    X2AP_TRACE(X2AP_DETAILED, "Forwarding message to S1AP");

    /* Send Request to S1AP */
    rrc_send_message(
            p_intrl_msg,
            RRC_S1AP_MODULE_ID);

    X2AP_UT_TRACE_EXIT();
    return X2AP_SUCCESS;

}

