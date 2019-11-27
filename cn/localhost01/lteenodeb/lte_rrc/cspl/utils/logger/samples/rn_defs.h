/******************************************************************************
**FILE :
**	This file contains hash-defines that can be used by the RANAP User.
*******************************************************************************
**
**FILE NAME:
**	rn_defs.h
**
**DESCRIPTION
**	This file contains hash defintions and API Identifiers  
**	provided to the RANAP User. 
**
** DATE				AUTHOR				REF			REASON
** ----   			------              ---		    ------
** 17 Apr 2002		reaggarwal			 - 			Original
**
**  Copyright (C) 2006 Aricent Inc . All Rights Reserved
******************************************************************************/

#ifndef	_RN_DEFS_H_
#define	_RN_DEFS_H_

/******************************************************************************
 * 																			  *	
 *							API IDENTIFIERS									  *	
 * 																			  *	
 ******************************************************************************/

#define RRN_API_BASE					0

				/***************************************
				 * API(s) from RANAP to the RANAP User *
				 ***************************************/

#define	RRN_MIN_RN_TO_USER_API_ID			RRN_API_BASE
#define RRN_MIN_RN_TO_USER_CO_API_ID		RRN_MIN_RN_TO_USER_API_ID

#define RRN_CN_DEACTIVATE_TRACE_IND			(RRN_MIN_RN_TO_USER_CO_API_ID)
#define RRN_CN_INVOKE_TRACE_IND				(RRN_MIN_RN_TO_USER_CO_API_ID + 1 )
#define RRN_COMMON_ID_IND					(RRN_MIN_RN_TO_USER_CO_API_ID + 2 )
#define RRN_DIRECT_TRANSFER_IND				(RRN_MIN_RN_TO_USER_CO_API_ID + 3 )
#define RRN_CONN_CONF_IND					(RRN_MIN_RN_TO_USER_CO_API_ID + 4 )
#define RRN_CONN_REFUSAL_IND				(RRN_MIN_RN_TO_USER_CO_API_ID + 5 )
#define RRN_IU_REL_IND						(RRN_MIN_RN_TO_USER_CO_API_ID + 6 )
#define RRN_LOC_REPORTING_CTRL_IND			(RRN_MIN_RN_TO_USER_CO_API_ID + 7 )
#define RRN_RAB_ASSIGN_REQ_IND				(RRN_MIN_RN_TO_USER_CO_API_ID + 8 )
#define RRN_RELOC_REQ_IND					(RRN_MIN_RN_TO_USER_CO_API_ID + 9 )
#define RRN_RELOC_CANCEL_ACK_IND			(RRN_MIN_RN_TO_USER_CO_API_ID + 10)
#define RRN_RELOC_CANCEL_FAIL_IND			(RRN_MIN_RN_TO_USER_CO_API_ID + 11)
#define RRN_RELOC_CMD_IND					(RRN_MIN_RN_TO_USER_CO_API_ID + 12)
#define RRN_RELOC_PREP_FAILURE_IND			(RRN_MIN_RN_TO_USER_CO_API_ID + 13)
#define RRN_SEC_MODE_CMD_IND				(RRN_MIN_RN_TO_USER_CO_API_ID + 14)
#define RRN_RELOC_OVERALL_EXP_IND			(RRN_MIN_RN_TO_USER_CO_API_ID + 15)
#define RRN_LOCAL_ERROR_IND					(RRN_MIN_RN_TO_USER_CO_API_ID + 16)
#define RRN_PEER_ERROR_IND					(RRN_MIN_RN_TO_USER_CO_API_ID + 17)
#define RRN_CANCEL_RELOC_IND 				(RRN_MIN_RN_TO_USER_CO_API_ID + 18)
#define RRN_SRNS_CTXT_REQ_IND				(RRN_MIN_RN_TO_USER_CO_API_ID + 19)
#define RRN_SRNS_DATA_FWD_CMD_IND			(RRN_MIN_RN_TO_USER_CO_API_ID + 20)
#define RRN_FWD_SRNS_CTXT_IND				(RRN_MIN_RN_TO_USER_CO_API_ID + 21)
#define RRN_DATA_VOL_REPORT_REQ_IND			(RRN_MIN_RN_TO_USER_CO_API_ID + 22)
#define RRN_LOCN_RELATED_DATA_REQ_IND		(RRN_MIN_RN_TO_USER_CO_API_ID + 23)

#define RRN_MAX_RN_TO_USER_CO_API_ID		RRN_LOCN_RELATED_DATA_REQ_IND

#define RRN_MIN_RN_TO_USER_CL_API_ID		(RRN_MAX_RN_TO_USER_CO_API_ID + 1)

#define RRN_OVERLOAD_IND					 RRN_MIN_RN_TO_USER_CL_API_ID
#define RRN_PAGING_IND						(RRN_MIN_RN_TO_USER_CL_API_ID + 1)
#define RRN_RESET_RESOURCE_IND				(RRN_MIN_RN_TO_USER_CL_API_ID + 2)
#define RRN_RESET_RESOURCE_ACK_IND			(RRN_MIN_RN_TO_USER_CL_API_ID + 3)
#define RRN_RESET_RESOURCE_FAIL_IND			(RRN_MIN_RN_TO_USER_CL_API_ID + 4)
#define RRN_RESET_IND						(RRN_MIN_RN_TO_USER_CL_API_ID + 5)
#define RRN_RESET_ACK_IND					(RRN_MIN_RN_TO_USER_CL_API_ID + 6)
#define RRN_RESET_FAIL_IND					(RRN_MIN_RN_TO_USER_CL_API_ID + 7)

#define RRN_MAX_RN_TO_USER_CL_API_ID		RRN_RESET_FAIL_IND

/* API to inform the Application about the Status Indication receieved 
 * from the SCCP */
#define RRN_LOWER_LAYER_STATUS_IND			(RRN_MAX_RN_TO_USER_CL_API_ID + 1)
/* APIs to Application to give the encoded and decoded buffer of RANAP
 * Relocation Information Message This message is between RNC-RNC. We 
 * are providing an interface for encoding and decoding of the message */
#define RRN_RELOC_INFO_ENC_IND				(RRN_MAX_RN_TO_USER_CL_API_ID + 2)
#define RRN_RELOC_INFO_DEC_IND				(RRN_MAX_RN_TO_USER_CL_API_ID + 3)
#define RRN_PEER_RANAP_UP_IND				(RRN_MAX_RN_TO_USER_CL_API_ID + 4)

#define	RRN_MAX_RN_TO_USER_API_ID			RRN_PEER_RANAP_UP_IND

				/***************************************
				 * API(s) from the RANAP user to RANAP *
				 ***************************************/


#define	RRN_MIN_USER_TO_RN_API_ID		(RRN_MAX_RN_TO_USER_API_ID + 1)
#define RRN_MIN_USER_TO_RN_CO_API_ID	 RRN_MIN_USER_TO_RN_API_ID

#define RRN_DIRECT_TRANS_REQ			 RRN_MIN_USER_TO_RN_CO_API_ID
#define RRN_INITIAL_UE_MSG_REQ			(RRN_MIN_USER_TO_RN_CO_API_ID + 1)
#define RRN_IU_REL_REQ					(RRN_MIN_USER_TO_RN_CO_API_ID + 2)
#define RRN_IU_REL_COMP_REQ				(RRN_MIN_USER_TO_RN_CO_API_ID + 3)
#define RRN_LOC_REPORT_REQ				(RRN_MIN_USER_TO_RN_CO_API_ID + 4)
#define RRN_RAB_ASSIGN_RESP_REQ			(RRN_MIN_USER_TO_RN_CO_API_ID + 5)
#define RRN_RAB_REL_REQ					(RRN_MIN_USER_TO_RN_CO_API_ID + 6)
#define RRN_RELOC_REQUIRED_REQ			(RRN_MIN_USER_TO_RN_CO_API_ID + 7)
#define RRN_RELOC_REQ_ACK_REQ			(RRN_MIN_USER_TO_RN_CO_API_ID + 8)
#define RRN_RELOC_CANCEL_REQ			(RRN_MIN_USER_TO_RN_CO_API_ID + 9)
#define RRN_RELOC_COMPLETE_REQ			(RRN_MIN_USER_TO_RN_CO_API_ID + 10)
#define RRN_RELOC_DETECT_REQ			(RRN_MIN_USER_TO_RN_CO_API_ID + 11)
#define RRN_RELOC_FAILURE_REQ			(RRN_MIN_USER_TO_RN_CO_API_ID + 12)
#define RRN_SEC_MODE_COMPLETE_REQ		(RRN_MIN_USER_TO_RN_CO_API_ID + 13)
#define RRN_SEC_MODE_REJECT_REQ			(RRN_MIN_USER_TO_RN_CO_API_ID + 14)
#define RRN_ERROR_REQ					(RRN_MIN_USER_TO_RN_CO_API_ID + 15)
#define RRN_DATA_VOL_REPORT_REQ			(RRN_MIN_USER_TO_RN_CO_API_ID + 16)
#define RRN_SRNS_CTXT_RESP_REQ			(RRN_MIN_USER_TO_RN_CO_API_ID + 17)
#define RRN_FWD_SRNS_CTXT_REQ			(RRN_MIN_USER_TO_RN_CO_API_ID + 18)
#define RRN_RAB_MODIFY_REQ				(RRN_MIN_USER_TO_RN_CO_API_ID + 19)
#define RRN_LOCN_RELATED_DATA_RESP_REQ	(RRN_MIN_USER_TO_RN_CO_API_ID + 20)
#define RRN_LOCN_RELATED_DATA_FAIL_REQ	(RRN_MIN_USER_TO_RN_CO_API_ID + 21)

#define RRN_MAX_USER_TO_RN_CO_API_ID	RRN_LOCN_RELATED_DATA_FAIL_REQ

#define RRN_MIN_USER_TO_RN_CL_API_ID	(RRN_MAX_USER_TO_RN_CO_API_ID + 1)

#define RRN_OVERLOAD_REQ				RRN_MIN_USER_TO_RN_CL_API_ID
#define RRN_RESET_RESOURCE_REQ			(RRN_MIN_USER_TO_RN_CL_API_ID + 1)
#define RRN_RESET_RESOURCE_ACK_REQ		(RRN_MIN_USER_TO_RN_CL_API_ID + 2)
#define RRN_RESET_REQ					(RRN_MIN_USER_TO_RN_CL_API_ID + 3)
#define RRN_RESET_ACK_REQ				(RRN_MIN_USER_TO_RN_CL_API_ID + 4)

#define RRN_MAX_USER_TO_RN_CL_API_ID	RRN_RESET_ACK_REQ

/* APIs to Application to give the encoded and decoded buffer of RANAP
 * Relocation Information Message This message is between RNC-RNC. We 
 * are providing an interface for encoding and decoding of the message */
#define RRN_RELOC_INFO_ENC_REQ			(RRN_MAX_USER_TO_RN_CL_API_ID + 1)
#define RRN_RELOC_INFO_DEC_REQ			(RRN_MAX_USER_TO_RN_CL_API_ID + 2)

#define RRN_MAX_USER_TO_RN_API_ID		RRN_RELOC_INFO_DEC_REQ

#endif
