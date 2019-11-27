/******************************************************************************
*
*   FILE NAME:
*       csc_utils.h
*
*   DESCRIPTION:
*       This header file contains declaration CSC specific utilities
*
*   DATE            AUTHOR      REFERENCE       REASON
*   02 June 2009    VasylS      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef _CSC_UTILS_H_
#define _CSC_UTILS_H_

#include "rrc_defines.h"
#include "csc_cell_ctx.h"
#include "rrc_rrm_intf.h"

rrc_return_et csc_send_rrm_cell_setup_resp
(
    rrc_transaction_id_t    transaction_id,
    U8                      cell_index,
	rrc_return_et           response,
    rrm_fail_cause_et       fail_cause
);
void* csc_alloc_intrl_msg
(
    rrc_module_id_t             dst_module_id,
    U16                         api_id,
    rrc_size_t                  msg_size
);

rrc_return_et csc_send_rrm_cell_reconfig_resp_failure
(
    rrc_transaction_id_t    transaction_id,
    U8                      cell_index,
    rrc_return_et           response,
    rrm_fail_cause_et       fail_cause
);

rrc_return_et csc_send_rrm_cell_del_resp_failure
(
    rrc_transaction_id_t    transaction_id,
    U8                      cell_index,
    rrc_return_et           response,
    rrm_fail_cause_et       fail_cause
);

rrc_return_et csc_send_rrm_updated_pws_si_list_cnf
(
    csc_cell_ctx_t          *p_cell_ctx,
    rrc_return_et           response
);

void csc_send_s1ap_pws_resp_failure
(
    rrc_cell_index_t        cell_index,
    rrc_transaction_id_t    trans_id,
    rrc_return_et           response,
    rrm_fail_cause_et       fail_cause
);

void csc_send_s1ap_kill_resp_failure
(
    rrc_cell_index_t        cell_index,
    rrc_transaction_id_t    trans_id,
    rrc_return_et           response,
/* SPR 5688 Start */
    rrc_kill_fail_cause_et  fail_cause
/* SPR 5688 End */
);

rrc_return_et csc_send_rrm_cell_start_resp
(
    rrc_transaction_id_t    transaction_id,
    U8                      cell_index,
    rrc_return_et           response,
    rrm_fail_cause_et       fail_cause
);

rrc_return_et csc_send_rrm_cell_stop_resp
(
    rrc_transaction_id_t    transaction_id,
    U8                      cell_index,
    rrc_return_et           response,
    rrm_fail_cause_et       fail_cause
);

rrc_return_et csc_send_rrm_cell_start_resp
(
    rrc_transaction_id_t    transaction_id,
    U8                      cell_index,
    rrc_return_et           response,
    rrm_fail_cause_et       fail_cause
);

rrc_return_et csc_send_rrm_cell_stop_resp
(
    rrc_transaction_id_t    transaction_id,
    U8                      cell_index,
    rrc_return_et           response,
    rrm_fail_cause_et       fail_cause
);

#endif /* _CSC_UTILS_H_ */

