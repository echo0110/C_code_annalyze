/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: s1ap_csc.h,v 1.3 2010/02/18 04:25:20 gur10248 Exp $
 ****************************************************************************
 *
 *  File Description : 
 *  This is the header file of s1ap_csc module which includes 
 *  common header files and various data structures exchanged.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: s1ap_csc.h,v $
 * Revision 1.3  2010/02/18 04:25:20  gur10248
 * LTE_RRC_Rel1.1_18feb
 *
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.2.3  2009/12/28 05:09:42  gur18569
 * fixed indentation
 *
 * Revision 1.1.2.2  2009/12/27 08:32:51  gur18569
 * removed tabs
 *
 * Revision 1.1.2.1  2009/11/25 13:11:02  gur18569
 * Shifted to level of rrc dir
 *
 * Revision 1.1.2.1  2009/10/23 17:10:41  gur18569
 * Initial version
 *
 *
 *
 ****************************************************************************/

#ifndef _S1AP_CSC_H_
#define _S1AP_CSC_H_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "s1ap_logger.h"
#include "rrc_common_utils.h"
#include "rrc_defines.h"
#include "rrc_msg_mgmt.h"
#include "s1ap_utils.h"
#include "s1ap_global_ctx.h"

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
void s1ap_process_paging_proc(    
        mme_context_t *p_mme_context,
        void            *p_asn_msg,                
        U16             asn_msg_len);

void s1ap_process_csc_messages(    
        void            *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context);

void process_s1ap_cell_register_req(    
        void            *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context);

void process_s1ap_cell_deregister_req(    
        void            *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context);

void send_enb_reset_part_to_mme(
        mme_context_t * p_mme_context);

void process_s1ap_cell_area_info_update_ind(    
        void            *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context);

void process_s1ap_pws_resp(    
    void            *p_api,                
    s1ap_gb_context_t    *p_s1ap_gb_context);

void process_s1ap_pws_failure_resp(    
    void            *p_api,                
    s1ap_gb_context_t    *p_s1ap_gb_context);

void process_s1ap_kill_resp(    
    void            *p_api,                
    s1ap_gb_context_t    *p_s1ap_gb_context);

 
#endif /* _S1AP_CSC_H_ */
