/*******************************************************************************
*
*  FILE NAME   : x2ap_api.h
*
*  DESCRIPTION : This file contains the api definition.
*
*  DATE 5-July-2010  NAME Sudhanshu Kumar Srivastav 
*                    REFERENCE 36.423_880(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/


#ifndef _X2AP_API_H_
#define _X2AP_API_H_

/****************************************************************************
 * Project Includes
 ****************************************************************************/

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
 * X2AP - OAM APIs
 *******************************************************************/
#define X2AP_OAM_API_BASE                      0x0430

#define X2AP_OAM_INIT_IND                      (X2AP_OAM_API_BASE + 1)
#define X2AP_OAM_INIT_CNF                      (X2AP_OAM_API_BASE + 2)
#define X2AP_OAM_PROVISION_REQ                 (X2AP_OAM_API_BASE + 3)
#define X2AP_OAM_PROVISION_RESP                (X2AP_OAM_API_BASE + 4)
#define X2AP_OAM_CLEANUP_REQ                   (X2AP_OAM_API_BASE + 5)
#define X2AP_OAM_CLEANUP_RESP                  (X2AP_OAM_API_BASE + 6)
#define X2AP_OAM_LINK_DOWN_IND                 (X2AP_OAM_API_BASE + 7)
#define X2AP_OAM_LINK_UP_IND                   (X2AP_OAM_API_BASE + 8)
#define X2AP_OAM_LINK_SETUP_REQ                (X2AP_OAM_API_BASE + 9)
#define X2AP_OAM_LINK_SETUP_RESP               (X2AP_OAM_API_BASE + 10)
#define X2AP_ADD_ENB_REQ                       (X2AP_OAM_API_BASE + 11)
#define X2AP_ADD_ENB_RES                       (X2AP_OAM_API_BASE + 12)
#define X2AP_DEL_ENB_REQ                       (X2AP_OAM_API_BASE + 13)
#define X2AP_DEL_ENB_RES                       (X2AP_OAM_API_BASE + 14)
/*sctp parameters start */
#define X2AP_OAM_GET_SCTP_STATUS_REQ           (X2AP_OAM_API_BASE + 15)
#define X2AP_OAM_GET_SCTP_STATUS_RESP          (X2AP_OAM_API_BASE + 16)
/*sctp parameters stop */
#define X2AP_OAM_PROC_SUPP_REQ                      (X2AP_OAM_API_BASE + 17)
#define X2AP_OAM_PROC_SUPP_RESP                      (X2AP_OAM_API_BASE + 18)
#define X2AP_OAM_SET_LOG_LEVEL_REQ             (X2AP_OAM_API_BASE + 19)
#define X2AP_OAM_SET_LOG_LEVEL_RESP             (X2AP_OAM_API_BASE + 20)
#define X2AP_OAM_GET_LOG_LEVEL_REQ             (X2AP_OAM_API_BASE + 21)
#define X2AP_OAM_GET_LOG_LEVEL_RESP             (X2AP_OAM_API_BASE + 22)

#define X2AP_OAM_LOG_ENABLE_REQ             (X2AP_OAM_API_BASE + 23)
#define X2AP_OAM_LOG_ENABLE_RESP             (X2AP_OAM_API_BASE + 24)
#define X2AP_OAM_LOG_DISABLE_REQ             (X2AP_OAM_API_BASE + 25)

#ifdef LINUX_PC_TEST
#define X2AP_OAM_CLOSE_SCTP_IND		(X2AP_OAM_API_BASE + 26)
#define TEST_X2AP_ADD_ENB_REQ				X2AP_ADD_ENB_REQ
#define TEST_X2AP_ADD_ENB_RES				X2AP_ADD_ENB_RES
#define TEST_X2AP_DEL_ENB_REQ				X2AP_DEL_ENB_REQ
#define TEST_X2AP_DEL_ENB_RES				X2AP_DEL_ENB_RES
#endif
/********************************************************************
 * X2AP - SCTP APIs
 *******************************************************************/
#define X2AP_EXTERNAL_API_BASE                  0x0450                            

#define X2AP_ENB_MESSAGE                        X2AP_EXTERNAL_API_BASE + 1     /* message from ENB */
#define X2AP_SCTP_CONN_FAILURE_IND              X2AP_EXTERNAL_API_BASE + 2     /* Connecttion Lost */
#define X2AP_SCTP_CONN_RESTART_IND              X2AP_EXTERNAL_API_BASE + 3     /* Connection restart */
#define X2AP_SCTP_CLOSED_IND                    X2AP_EXTERNAL_API_BASE + 4     /* Shut down complete */
#define X2AP_SCTP_COMM_UP_IND                   X2AP_EXTERNAL_API_BASE + 5     /* Communication up */
#define X2AP_SCTP_PEND_CONN_IND                 X2AP_EXTERNAL_API_BASE + 6     /* Pending */
#define X2AP_SCTP_SHUT_DOWN_INIT_IND            X2AP_EXTERNAL_API_BASE + 7     /* Shut down initiated */
#ifdef LINUX_PC_TEST
#define X2AP_ENB_MESSAGE_PS_TO_TEST			X2AP_EXTERNAL_API_BASE + 8
#endif


/********************************************************************
 * X2AP - RRM APIs 
 *******************************************************************/
#define X2AP_RRM_IF_API_BASE                   0x0460

#define X2AP_RRM_LINK_DOWN_IND                 (X2AP_RRM_IF_API_BASE + 1)	
#define X2AP_RRM_LINK_UP_IND                   (X2AP_RRM_IF_API_BASE + 2)	
#define X2AP_RRM_CELL_UPDATED_INDICATION       (X2AP_RRM_IF_API_BASE + 3)
#define X2AP_RRM_RSU_RRM_START_REQ             (X2AP_RRM_IF_API_BASE + 4)
#define X2AP_RRM_RSU_ENB_START_RES             (X2AP_RRM_IF_API_BASE + 5)
#define X2AP_RRM_RSU_RRM_STOP_REQ              (X2AP_RRM_IF_API_BASE + 6)
#define X2AP_RRM_RSU_ENB_STOP_RES              (X2AP_RRM_IF_API_BASE + 7)
#define X2AP_RRM_RSU_ENB_START_REQ             (X2AP_RRM_IF_API_BASE + 8)
#define X2AP_RRM_RSU_RRM_START_RES             (X2AP_RRM_IF_API_BASE + 9)
#define X2AP_RRM_RSU_ENB_STOP_REQ              (X2AP_RRM_IF_API_BASE + 10)
#define X2AP_RRM_RSU_RRM_STOP_RES              (X2AP_RRM_IF_API_BASE + 11)
#define X2AP_RRM_RSU_RRM_UPDATE                (X2AP_RRM_IF_API_BASE + 12)
#define X2AP_RRM_RSU_ENB_UPDATE_IND            (X2AP_RRM_IF_API_BASE + 13)
#define X2AP_RRM_LI_RRM_LOAD_INFORMATION_REQ   (X2AP_RRM_IF_API_BASE + 14)
#define X2AP_RRM_LI_ENB_LOAD_INFORMATION_RES   (X2AP_RRM_IF_API_BASE + 15)
#define X2AP_RRM_LI_ENB_LOAD_INFORMATION_IND   (X2AP_RRM_IF_API_BASE + 16)
#define X2AP_ENB_CONFIG_UPDATE_REQ 	           (X2AP_RRM_IF_API_BASE + 17)
#define X2AP_ENB_CONFIG_UPDATE_RES 	           (X2AP_RRM_IF_API_BASE + 18)
#define X2AP_ENB_CONFIG_UPDATE_IND 	           (X2AP_RRM_IF_API_BASE + 19)
#define X2AP_RESET_REQ 			               (X2AP_RRM_IF_API_BASE + 20)
#define X2AP_RESET_RES 			               (X2AP_RRM_IF_API_BASE + 21)
#define X2AP_RESET_IND 			               (X2AP_RRM_IF_API_BASE + 22)
#define X2AP_RLF_IND 			               (X2AP_RRM_IF_API_BASE + 23)
#define X2AP_ENB_LINK_UP_IND		           (X2AP_RRM_IF_API_BASE + 24)
#define X2AP_ENB_LINK_DOWN_IND		           (X2AP_RRM_IF_API_BASE + 25)
#define X2AP_ENB_CONFIG_UPDATE_WAIT_IND	       (X2AP_RRM_IF_API_BASE + 26)
#define X2AP_HO_REPORT_IND                     (X2AP_RRM_IF_API_BASE + 27)
#define X2AP_PEER_CELL_ACTIVATION_REQ	       (X2AP_RRM_IF_API_BASE + 28)
#define X2AP_PEER_CELL_ACTIVATION_RESP	       (X2AP_RRM_IF_API_BASE + 29)
#define X2AP_CELL_ACTIVATION_REQ	           (X2AP_RRM_IF_API_BASE + 30)
#define X2AP_CELL_ACTIVATION_RESP	           (X2AP_RRM_IF_API_BASE + 31)
#define X2AP_MOBILITY_CHANGE_REQ	           (X2AP_RRM_IF_API_BASE + 32)
#define X2AP_MOBILITY_CHANGE_RESP	           (X2AP_RRM_IF_API_BASE + 33)
#define X2AP_MOBILITY_SETTING_CHANGE_REQ       (X2AP_RRM_IF_API_BASE + 34)
#define X2AP_MOBILITY_SETTING_CHANGE_RESP	   (X2AP_RRM_IF_API_BASE + 35)
#define X2AP_RRM_UPDATED_MBMS_SF_INFO_IND	   (X2AP_RRM_IF_API_BASE + 36)
#define X2AP_ENDC_CONFIG_UPDATE_REQ 	       (X2AP_RRM_IF_API_BASE + 37)
#define X2AP_ENDC_CONFIG_UPDATE_RES 	       (X2AP_RRM_IF_API_BASE + 38)
#define X2AP_ENDC_CONFIG_UPDATE_IND 	       (X2AP_RRM_IF_API_BASE + 39)
#define X2AP_ENDC_CONFIG_UPDATE_WAIT_IND       (X2AP_RRM_IF_API_BASE + 40)

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

#endif  /* _X2AP_API_H_ */
