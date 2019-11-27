/****************************************************************************
*
*  ARICENT -
*
*  Copyright (C) 2011 Aricent Inc. All Rights Reserved.
*
****************************************************************************
*
*  $Id: x2ap_cell_activation.h,v 1.6 2012/10/26 07:04:17 Hari Oum Sharan Exp $
*
****************************************************************************
*
*  File Description : 
*
****************************************************************************/
#ifndef _X2AP_CELL_ACTIVATION_H_
#define _X2AP_CELL_ACTIVATION_H_

/****************************************************************************
* Project Includes
****************************************************************************/

/****************************************************************************
* Exported Includes
****************************************************************************/
#include "rrc_defines.h"
#include "x2ap_db.h"
#include "rrc_common_utils.h"
#include "x2ap_asn_enc_dec_3gpp.h"
#include "rrc_x2apEnb_intf.h"

/****************************************************************************
* Exported Definitions
****************************************************************************/
/********************************************************************
*                        EXTERNAL APIs
*******************************************************************/

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
x2ap_return_et x2ap_process_peer_cell_activation_req( 
            void                *p_api,
            x2ap_gb_context_t   *p_x2ap_gb_ctx); 

x2ap_return_et x2ap_enb_process_cell_activation_request_msg(
            U8 *p_msg_ptr,
            x2ap_gb_context_t  *p_x2ap_gl_ctx,
            x2ap_peer_enb_context_t *p_enb_ctx);

x2ap_return_et x2ap_process_cell_activation_resp( 
            void                *p_api,
            x2ap_gb_context_t   *p_x2ap_gb_ctx); 


x2ap_return_et x2ap_enb_process_cell_activation_response_msg(
            U8 *p_msg_ptr,
            x2ap_gb_context_t  *p_x2ap_gl_ctx,
            x2ap_peer_enb_context_t *p_enb_ctx);

x2ap_return_et x2ap_enb_process_cell_activation_failure_msg(
            U8 *p_msg_ptr,
            x2ap_gb_context_t  *p_x2ap_gl_ctx,
            x2ap_peer_enb_context_t *p_enb_ctx);

x2ap_return_et x2ap_cell_activation_timer_expiry_hdl(
        x2ap_timer_buffer_t *p_timer_buf, 
        x2ap_gb_context_t *p_x2ap_gb_ctx);

/*spr_fix*/
x2ap_return_et x2ap_cell_activation_rrm_guard_timer_expiry_hdl(
        x2ap_timer_buffer_t *p_timer_buf, 
        x2ap_gb_context_t *p_x2ap_gb_ctx);
/*spr_fix*/

#endif  /* _X2AP_CELL_ACTIVATION_H_ */
