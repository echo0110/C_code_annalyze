/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: s1ap_api.h,v 1.2 2010/01/04 16:10:05 ukr15916 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: s1ap_api.h,v $
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.2.6  2009/12/28 05:09:42  gur18569
 * fixed indentation
 *
 * Revision 1.1.2.5  2009/12/11 04:47:50  gur21006
 * Added API for sctp shutdown message type
 *
 * Revision 1.1.2.4  2009/12/03 10:11:36  gur18569
 * integration changes
 *
 * Revision 1.1.2.3  2009/12/03 09:24:08  ukr18877
 * Points 5, 10 and 3MM (CSC) from mail "Questions about S1AP" fixed. Parameter RUNNER_RRC_UECC_FT set to 0 (for SSIT testing), OAM connect and cleanup SSIT tests added
 *
 * Revision 1.1.2.2  2009/12/01 15:07:24  gur18569
 * integration changes
 *
 * Revision 1.1.2.1  2009/11/25 13:11:02  gur18569
 * Shifted to level of rrc dir
 *
 * Revision 1.1.2.2  2009/11/17 04:30:47  gur20470
 * Modified value of S1AP_OAM_API_BASE
 *
 * Revision 1.1.2.1  2009/10/23 16:11:39  gur18569
 * Initial version
 *
 *
 *
 ****************************************************************************/

#ifndef _S1AP_API_H_
#define _S1AP_API_H_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "rrc_defines.h"
#include "rrc_ext_api.h"
#include "rrc_intrl_api.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/********************************************************************
 *                        EXTERNAL APIs
 *******************************************************************/
/********************************************************************
 * S1AP - OAM APIs
 *******************************************************************/
#define S1AP_OAM_API_BASE                      0x0500

#define S1AP_OAM_INIT_IND                      (S1AP_OAM_API_BASE + 1)
#define S1AP_OAM_INIT_CNF                      (S1AP_OAM_API_BASE + 2)
#define S1AP_OAM_PROVISION_REQ                 (S1AP_OAM_API_BASE + 3)
#define S1AP_OAM_PROVISION_RESP                (S1AP_OAM_API_BASE + 4)
#define S1AP_OAM_RESET_REQ                     (S1AP_OAM_API_BASE + 5)
#define S1AP_OAM_RESET_RESP                    (S1AP_OAM_API_BASE + 6)
#define S1AP_OAM_CLEANUP_REQ                   (S1AP_OAM_API_BASE + 7)
#define S1AP_OAM_CLEANUP_RESP                  (S1AP_OAM_API_BASE + 8)
/* CSR 00058587 Fix Start */
#define S1AP_OAM_STATUS_REQ                    (S1AP_OAM_API_BASE + 9)
#define S1AP_OAM_STATUS_RESP                   (S1AP_OAM_API_BASE + 10)
/* CSR 00058587 Fix Stop */
#define S1AP_OAM_ENB_CONFIG_UPDATE             (S1AP_OAM_API_BASE + 11)
#define S1AP_OAM_ENB_CONFIG_UPDATE_RESPONSE    (S1AP_OAM_API_BASE + 12)
#define S1AP_OAM_CLOSE_MME_CONN                (S1AP_OAM_API_BASE + 13)
#define S1AP_OAM_REESTAB_MME_CONN              (S1AP_OAM_API_BASE + 14)
#define S1AP_OAM_S1AP_LINK_STATUS_IND          (S1AP_OAM_API_BASE + 15)
#define S1AP_ADD_MME_REQ                   (S1AP_OAM_API_BASE + 16)
#define S1AP_ADD_MME_RES                  (S1AP_OAM_API_BASE + 17)
#define S1AP_OAM_MME_CONN_INFO_RESPONSE        (S1AP_OAM_API_BASE + 18)
#define S1AP_OAM_MME_INFO                      (S1AP_OAM_API_BASE + 19)
/*sctp parameters start*/
#define S1AP_OAM_GET_SCTP_STATUS_REQ           (S1AP_OAM_API_BASE + 20)
#define S1AP_OAM_GET_SCTP_STATUS_RESP          (S1AP_OAM_API_BASE + 21)
/*sctp parameters stop*/
#define S1AP_OAM_PROC_SUPP_REQ                      (S1AP_OAM_API_BASE + 22)
#define S1AP_OAM_PROC_SUPP_RESP                      (S1AP_OAM_API_BASE + 23)
#define S1AP_OAM_SET_LOG_LEVEL_REQ             (S1AP_OAM_API_BASE + 24)
#define S1AP_OAM_SET_LOG_LEVEL_RESP            (S1AP_OAM_API_BASE + 25)
#define S1AP_OAM_GET_LOG_LEVEL_REQ             (S1AP_OAM_API_BASE + 26)
#define S1AP_OAM_GET_LOG_LEVEL_RESP            (S1AP_OAM_API_BASE + 27)

#define S1AP_OAM_LOG_ENABLE_REQ             (S1AP_OAM_API_BASE + 28)
#define S1AP_OAM_LOG_ENABLE_RESP            (S1AP_OAM_API_BASE + 29)

#define S1AP_OAM_LOG_DISABLE_REQ             (S1AP_OAM_API_BASE + 30)
#ifdef LINUX_PC_TEST
#define S1AP_GB_STATUS_REQ                    (S1AP_OAM_API_BASE + 31)
#define S1AP_GB_STATUS_RESP                   (S1AP_OAM_API_BASE + 32)
#endif
/********************************************************************
* S1AP - RRM APIs
*******************************************************************/
#define S1AP_RRM_API_BASE                  0x0700
#define S1AP_RRM_ENB_DIRECT_INFO_TRANSFER      (S1AP_RRM_API_BASE + 1)
#define S1AP_RRM_MME_DIRECT_INFO_TRANSFER      (S1AP_RRM_API_BASE + 2)
#define S1AP_RRM_ENB_CONFIG_TRANSFER           (S1AP_RRM_API_BASE + 3)
#define S1AP_RRM_MME_CONFIG_TRANSFER           (S1AP_RRM_API_BASE + 4)

#define S1AP_RRM_PWS_REQ                       (S1AP_RRM_API_BASE + 5)
#define S1AP_RRM_PWS_RESP                      (S1AP_RRM_API_BASE + 6)
#define S1AP_RRM_PWS_CNF                       (S1AP_RRM_API_BASE + 7)
#define S1AP_RRM_KILL_REQ                      (S1AP_RRM_API_BASE + 8)
#define S1AP_RRM_KILL_RESP                     (S1AP_RRM_API_BASE + 9)
#define S1AP_RRM_KILL_CNF                      (S1AP_RRM_API_BASE + 10)
#define	S1AP_RRM_RIM_INFO_REQ		       (S1AP_RRM_API_BASE + 11)
#define	S1AP_RRM_RIM_INFO		       (S1AP_RRM_API_BASE + 12)
#define	S1AP_RRM_RIM_INFO_ACK		       (S1AP_RRM_API_BASE + 13)
#define	S1AP_RRM_RIM_INFO_ERROR		       (S1AP_RRM_API_BASE + 14)
#define	S1AP_RRM_RIM_INFO_APP_ERROR	       (S1AP_RRM_API_BASE + 15)
#define RRC_RRM_RAN_INFO_REQ 		       (S1AP_RRM_API_BASE + 16)
#define RRC_RRM_RAN_INFO 		       (S1AP_RRM_API_BASE + 17)
#define RRC_RRM_RAN_INFO_ACK 		       (S1AP_RRM_API_BASE + 18)
#define RRC_RRM_RAN_INFO_ERROR 		       (S1AP_RRM_API_BASE + 19)
#define RRC_RRM_RAN_INFO_APP_ERROR	       (S1AP_RRM_API_BASE + 20)
#define S1AP_RRM_MME_INFO                      (S1AP_RRM_API_BASE + 21)

#define S1AP_RRM_LPPA_ECID_MEAS_INIT_REQ       (S1AP_RRM_API_BASE + 22)
#define S1AP_RRM_LPPA_ECID_MEAS_INIT_RESP      (S1AP_RRM_API_BASE + 23)
#define S1AP_RRM_LPPA_ECID_MEAS_INIT_FAIL      (S1AP_RRM_API_BASE + 24)
#define S1AP_RRM_LPPA_ECID_MEAS_FAIL_IND       (S1AP_RRM_API_BASE + 25)
#define S1AP_RRM_LPPA_ECID_MEAS_REPORT         (S1AP_RRM_API_BASE + 26)
#define S1AP_RRM_LPPA_ECID_MEAS_TERMINATION_COMMAND (S1AP_RRM_API_BASE + 27)
#define S1AP_RRM_LPPA_OTDOA_INFO_REQ           (S1AP_RRM_API_BASE + 28)
#define S1AP_RRM_LPPA_OTDOA_INFO_RESP          (S1AP_RRM_API_BASE + 29)
#define S1AP_RRM_LPPA_OTDOA_INFO_FAIL          (S1AP_RRM_API_BASE + 30)
#define S1AP_RRM_LPPA_ERROR_IND                (S1AP_RRM_API_BASE + 31)
#define S1AP_RRM_LPPA_ERROR_IND_REQ            (S1AP_RRM_API_BASE + 32)

#define S1AP_RRM_MAX_API                       (S1AP_RRM_API_BASE + 39)

/********************************************************************
 *                        INTERNAL APIs
 *******************************************************************/
/* S1AP - UECC and S1AP - CSC API IDs defined in rrc_intrl_api.h */

/********************************************************************
 * S1AP - CMES ASSOCIATED
 *******************************************************************/
#define MME_MESSAGE                        (S1AP_RRM_API_BASE + 33)
#define SCTP_CONN_FAILURE_IND              (S1AP_RRM_API_BASE + 34)
#define SCTP_CONN_RECOVERY_IND             (S1AP_RRM_API_BASE + 35)
#define SCTP_CONN_SHUTDOWN_EVENT           (S1AP_RRM_API_BASE + 36)
	/*Bug 9050 fixes start*/
#define SCTP_ASSOC_UP_EVENT                (S1AP_RRM_API_BASE + 37)
#define SCTP_ASSOC_DOWN_EVENT              (S1AP_RRM_API_BASE + 38)
	/*Bug 9050 fixes end*/
#ifdef LINUX_PC_TEST
#define MME_MESSAGE_PS_TO_TEST			   (S1AP_RRM_API_BASE + 39)
#define SCTP_CLOSED_PS_TO_TEST			   (S1AP_RRM_API_BASE + 40)
#endif

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

#endif  /* _S1AP_API_H_ */
