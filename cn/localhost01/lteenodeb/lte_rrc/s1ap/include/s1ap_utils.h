/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 *  File Details
 *  ------------
 *  $Id: s1ap_utils.h,v 1.4 2010/10/13 07:04:11 gur18569 Exp $
 ****************************************************************************
 *
 *  File Description : 
 *  This is the header file of s1ap module which includes 
 *  various utility functions to send messages to other RRC's
 *  CSPL modules.
 *
 ****************************************************************************
 *
 *  Revision Details
 *  ----------------
 *
 *  $Log: s1ap_utils.h,v $
 *  Revision 1.4  2010/10/13 07:04:11  gur18569
 *  Merge from LTE_L3_Rel_2_0_2 tag tag
 *
 *  Revision 1.2.10.2  2010/08/05 08:10:44  gur15697
 *  merging round 2
 *
 *  Revision 1.2.10.1  2010/06/29 06:18:07  gur22047
 *  internal source and s1ap code
 *  Revision 1.3  2010/07/24 07:18:48  gur04733
 *  Checking On 24 July
 *
 *  Revision 1.2.8.2  2010/07/01 05:29:19  gur10248
 *  retrive old version before s1ap asn
 *
 *  Revision 1.2.8.1  2010/06/30 07:28:17  gur10248
 *  new s1ap folder
 *
 *  Revision 1.2  2010/01/04 16:10:05  ukr15916
 *  no message
 *
 *  Revision 1.1.2.7  2009/12/28 05:09:42  gur18569
 *  fixed indentation
 *
 *  Revision 1.1.2.6  2009/12/27 08:32:51  gur18569
 *  removed tabs
 *
 *  Revision 1.1.2.5  2009/12/03 10:28:48  ukr18877
 *  function s1ap_set_sd_from_header moved from s1ap_utils to sctp_wrappers
 *
 *  Revision 1.1.2.4  2009/12/03 09:24:08  ukr18877
 *  Points 5, 10 and 3MM (CSC) from mail "Questions about S1AP" fixed. Parameter RUNNER_RRC_UECC_FT set to 0 (for SSIT testing), OAM connect and cleanup SSIT tests added
 *
 *  Revision 1.1.2.3  2009/12/01 15:07:24  gur18569
 *  integration changes
 *
 *  Revision 1.1.2.2  2009/11/29 09:18:34  gur18569
 *  added s1ap_set_sctp_sd
 *
 *  Revision 1.1.2.1  2009/11/25 13:11:02  gur18569
 *  Shifted to level of rrc dir
 *
 *  Revision 1.1.2.3  2009/11/17 14:04:10  gur20470
 *  Incorporated review comments
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
#ifndef _S1AP_UTILS_H_
#define _S1AP_UTILS_H_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "rrc_defines.h"
#include "rrc_msg_mgmt.h"
#include "s1ap_asn_codec.h"
#include "s1ap_logger.h"
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

/* Prototype of s1ap forming the internal message header function */
void* s1ap_alloc_intrl_msg(
        rrc_module_id_t     dst_module_id,  
        U16                 api_id,         
        U16                 msg_size);

/* Prototypes to get socket sd from the incoming msg buffer from MME */
U32 s1ap_get_sctp_sd(
        U32 *p_header 
        );
/*REL 1.2:HANDOVER PROCEDURE START*/
/* Prototypes to get stream_id from the incoming msg buffer from MME */
U16 s1ap_get_stream_id(
        U16 *p_header
        );
/*REL 1.2:HANDOVER PROCEDURE END*/
/* Prototypes of function that processes enbues1apid to return the cell id */
U8 s1ap_get_cell_id(
        s1ap_gb_context_t    *p_s1ap_gb_context,
        U32 p_header 
        );

/* Prototypes of function that processes enbues1apid to return the ue id */
U16 s1ap_get_ue_id(
        U32 p_header 
        );

rrc_transaction_id_t s1ap_enb_mgr_get_next_transaction_id(
    s1ap_gb_context_t     *p_s1ap_gb_ctx
        );

void get_ue_ctx_from_ue_id(
        s1ap_gb_context_t     *p_s1ap_gb_ctx,
        ue_s1ap_context_t     **pp_s1ap_ue_context,
        U32                   ue_index 
        );

void set_ue_ctx_from_ue_id(
        s1ap_gb_context_t     *p_s1ap_gb_ctx,
        ue_s1ap_context_t     *p_s1ap_ue_context,
        U32                   ue_index 
        );
/*BUG 604 Fixes start*/
U32 s1ap_ueid_allocate(
    s1ap_gb_context_t     *p_s1ap_gb_ctx  /* S1AP global context */
    );

void s1ap_ueid_free(
    s1ap_gb_context_t*  p_s1ap_gb_ctx,  /* S1AP global context */
    U32    s1ap_ueid                        /* S1AP_UEID  */
);

rrc_return_t s1ap_ueid_ctx_init(
   s1ap_gb_context_t*  p_s1ap_gb_ctx  /* S1AP global context */
);
/*BUG 604 Fixes stop*/
/*bugid 117989 begin*/
rrc_return_t s1ap_check_cell_index(
		rrc_cell_index_t   cell_index,
		s1ap_gb_context_t*  p_s1ap_gb_ctx  /* S1AP global context */
);
/*bugid 117989 end*/
#endif /* _S1AP_UTILS_H_ */
