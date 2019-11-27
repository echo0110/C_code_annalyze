/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: x2ap_uecc.h,v 1.5 2011/09/23 12:22:10 gur22047 Hari Oum Exp $
 ****************************************************************************
 *
 *  File Description : This is the header file of the X2AP UECC.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: x2ap_uecc.h,v $
 *
 *
 ****************************************************************************/

#ifndef _X2AP_UECC_H_
#define _X2AP_UECC_H_


/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "rrc_x2ap_uecc_intf.h"
#include "x2ap_db.h"
#include "rrc_protocol_events.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/



/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
void x2ap_process_uecc_messages(
    void                *p_api,
    x2ap_gb_context_t   *p_x2ap_gb_context);

 x2ap_return_et x2ap_build_and_send_status_ind(
    x2ap_peer_enb_context_t *p_enb_ctx,
    U8                      status,
    U16                     num_served_cell,
    /* CSR_00040799 Fix Start */
    x2ap_srvd_cell_arry_t   *p_served_cell,
    U16                     total_ue_supported);
    /* CSR_00040799 Fix End */

#ifdef ENDC_ENABLED
/* MENB CHANGES - START */
x2ap_return_et
x2ap_build_and_send_nr_status_ind
(
    x2ap_peer_enb_context_t     *p_enb_ctx,
    x2ap_gb_context_t           *p_x2ap_gl_ctx,
    U8                          status,
    U16                         num_served_cell,
    x2ap_srvd_nr_cell_info_t    *p_served_cell,
    U16                         total_ue_supported
);
void x2ap_build_and_send_nr_update_info
(
    x2ap_nr_neigh_info_t* p_nbour_info
);
/* MENB CHANGES - END */
#endif

x2ap_return_et x2ap_build_and_send_reset_ind(
    x2ap_peer_enb_context_t *p_enb_ctx);

void x2ap_fill_protocol_event_params(
    EVENT_EXTERNAL_HEADER   *p_event_header,
    U32                     encoded_msg_len,
    U16                     event_id,
    U32                     message_direction);

/*CA_PHASE3_INTER_CELL_MRO_START*/
x2ap_return_et x2ap_build_and_send_internal_rlf_indication(
    x2ap_gb_context_t                       *p_x2ap_gb_ctx, /*X2AP Global context*/
    x2ap_ue_reestab_conn_setup_reconfig_t   *p_reestab_conn_setup_reconfig);
/*CA_PHASE3_INTER_CELL_MRO_STOP*/

#endif   /* _X2AP_UECC_H_ */
