/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 *  File Details
 *  ------------
 *  $Id: s1ap_mme.h,v 1.4 2010/10/13 07:04:07 gur18569 Exp $
 ****************************************************************************
 *
 *  File Description : 
 *  This is the file which contains definitions and data structures
 *  used for processing the MME messages in s1ap module.
 *
 ****************************************************************************
 *
 *  Revision Details
 *  ----------------
 *
 *  $Log: s1ap_mme.h,v $
 *  Revision 1.4  2010/10/13 07:04:07  gur18569
 *  Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 *  Revision 1.2.10.1  2010/08/05 08:10:44  gur15697
 *  merging round 2
 *
 *  Revision 1.3  2010/07/24 07:18:48  gur04733
 *  Checking On 24 July
 *
 *  Revision 1.2.8.2  2010/07/01 05:29:19  gur10248
 *  retrive old version before s1ap asn
 *
 *  Revision 1.2.8.1  2010/06/30 07:28:16  gur10248
 *  new s1ap folder
 *
 *  Revision 1.2  2010/01/04 16:10:05  ukr15916
 *  no message
 *
 *  Revision 1.1.2.6  2009/12/28 05:09:42  gur18569
 *  fixed indentation
 *
 *  Revision 1.1.2.5  2009/12/27 08:32:51  gur18569
 *  removed tabs
 *
 *  Revision 1.1.2.4  2009/12/22 14:38:53  gur18569
 *  fixed 154
 *
 *  Revision 1.1.2.3  2009/12/17 07:32:47  gur18569
 *  removed multiple definition of MAX_NUM_MME
 *
 *  Revision 1.1.2.2  2009/12/03 09:24:08  ukr18877
 *  Points 5, 10 and 3MM (CSC) from mail "Questions about S1AP" fixed. Parameter RUNNER_RRC_UECC_FT set to 0 (for SSIT testing), OAM connect and cleanup SSIT tests added
 *
 *  Revision 1.1.2.1  2009/11/25 13:11:02  gur18569
 *  Shifted to level of rrc dir
 *
 *  Revision 1.1.2.2  2009/11/09 09:02:10  gur18569
 *  removed compilation warnings
 *
 *  Revision 1.1.2.1  2009/10/23 16:11:39  gur18569
 *  Initial version
 *
 *
 *
 ****************************************************************************/

#ifndef _S1AP_MME_H_
#define _S1AP_MME_H_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "s1ap_logger.h"
#include "rrc_msg_mgmt.h"
#include "rrc_s1ap_uecc_intf.h"
#include "rrc_s1ap_csc_intf.h"
#include "s1ap_global_ctx.h"
#include "s1ap_utils.h"
#include "s1ap_asn_codec.h"
#include "s1ap_api.h"
#include "s1ap_mme_fsm.h"
#include "rrc_s1apOam_intf.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define SOCKET_DESC_SIZE    4  
#define MAX_MME_UE_S1AP_ID 0xFFFFFFFF
#define MAX_ENB_UE_S1AP_ID 0xFFFFFF
#define INVALID_S1AP_PROCEDURE_CODE 0xFF
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
void s1ap_process_mme_messages(    
        void             *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context);

void s1ap_process_mme_initiated_reset(s1ap_gb_context_t *p_s1ap_gl_ctx,
        mme_context_t *p_mme_context,
        U8 * p_asn_msg,
        U16  asn_msg_len);

rrc_return_et send_msg_to_mme(
        mme_context_t *p_mme_ctx, 
        U8* p_message, 
        U32 message_buff_len,
        U16 *p_stream_id);

mme_overload_response_et convert_overload_response(
        s1ap_OverloadAction overload_action);
U32 s1ap_decode_overload_gummei_list(
        s1ap_gummei_t *p_gummei_list,
        s1ap_GUMMEIList *p_s1ap_gummei_list);

#endif /* _S1AP_MME_H_ */
