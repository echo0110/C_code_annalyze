/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: uecc_oamh.h,v 1.3 2010/03/24 09:50:42 gur18569 Exp $
 ****************************************************************************
 *
 *   DESCRIPTION:
 *       This is the header file of the UECC OAMH.
 *       UECC interact with RRC OAMH module.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: uecc_oamh.h,v $
 * Revision 1.3  2010/03/24 09:50:42  gur18569
 * Merged files from Rel 1.0 support branch
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.4.6  2009/12/10 11:45:33  ukr15916
 * _H macros fixed
 *
 * Revision 1.1.4.5  2009/11/26 18:33:49  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.4.3.2.7  2009/11/24 11:32:35  ukr15916
 * updated for dev_rel_1_0
 *
 * Revision 1.1.4.3.2.5.2.1  2009/11/23 17:04:05  ukr15916
 * beautification
 *
 * Revision 1.1.4.3.2.5  2009/11/19 09:52:17  ukr15916
 * tabs -> spaces
 *
 * Revision 1.1.4.3.2.4  2009/10/30 12:47:44  ukr15916
 * UECC-OAMH interface -- Get Cell Status and Get UE Status
 * requests handling added
 *
 *
 ****************************************************************************/

#ifndef UECC_OAMH_H
#define UECC_OAMH_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "uecc_global_ctx.h"
#include "uecc_ue_timer_utils.h"

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

void uecc_oamh_process_timer_msg
(
    rrc_timer_t             timer_id,               /* Identity of the expired timer */
    uecc_gb_context_t       *p_uecc_gb_context      /* Pointer to UECC global context */
);


void uecc_oamh_prov_req_processs_msg
(
    void                *p_api,             /* Pointer to input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* Pointer to UECC global context */
);

void uecc_oamh_clear_req_processs_msg
(
    void                *p_api,             /* Pointer to input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* Pointer to UECC global context */
);

void uecc_oamh_get_cell_status_req_processs_msg
(
    void                *p_api,             /* Pointer to input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* Pointer to UECC global context */
);

void uecc_oamh_get_ue_status_req_processs_msg
(
    void                *p_api,             /* Pointer to input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* Pointer to UECC global context */
);
void uecc_oamh_cell_traffic_trace_start_processs_msg
(
    void                *p_api,             /* Pointer to input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* Pointer to UECC global context */
);
void uecc_oamh_cell_traffic_trace_stop_processs_msg
(
    void                *p_api,             /* Pointer to input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* Pointer to UECC global context */
);
void uecc_oamh_trace_file_transfer_complete_ind_processs_msg
(
    void                *p_api,             /* Pointer to input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* Pointer to UECC global context */
);
/* lipa start */
void uecc_oamh_add_del_lgw_ind_processs_msg
(
    void                *p_api,             /* Input API buffer */
    uecc_gb_context_t   *p_uecc_gb_context  /* UECC global context */
);
/* lipa end */

rrc_return_t create_adv_logger_file( uecc_gb_context_t   *p_uecc_gb_context);
#endif /* UECC_OAMH_H */
