/* vim:ts=4:et:ai:ci:sw=4
*/
/********************************************************************************
 * File Name   : oam_sim_utils.c
 * Description : This file contains procedures to detect the ethernet adaptor ,
 *               and creates the virtual interfaces for OAM.
 * Revision History :
 * Date          Author                Reference                  Comments
 * May,2012      Brijesh madolia       OAM Design Document        Initial Version
 * November,2012 Sunita	              Rel 1.0 Fault Management   Thread Added
 * January,2013  Vinod Vyas            OAM Design Doc             Changes for SDM
 Release 2.0
 * Sept,2013     Raveen Sharma         CSR 51606 Fix              Code Changes for 
 *                                                                CSR Fix
 *Sept,2013      Vinod Vyas            OAM Design Document        Ipv6 Support                                                                
 *April,2014     Ankita Jain           OAM Design Document        Cell Traffic Trace                                                               
 * June, 2014    Vivek K Gupta         Release 3.0         Changes for xml
 *                                                      add/delete frwk code
 * June, 2014    Ekta Jain             Release 3.0                BUG 11488 FIX
 * June,2014        Ravi Gupta         SPR 10567 Fix
 * June 2014     Shalu Sagar             SPR 12225 FIX   
 * June,2014     Avinash Kumar 	      SPR 12175 FIx
 * July,2014     Aditi Singh           SPR 9620 Fix                  Guard timer Added 
 * July,2014     Ankita Jain           SPR 9485 Fix                  GU_GROUP_ID related enhancement
 * Aug 2014      Aditi Singh           SPR 13116
 * Oct 2014      Aditi Singh            SPR 13617
 * Oct,2014      Ankita Jain           SPR 13586 Fix                 
 * Dec  2014     Vivek Gupta           SPR 14152
 * Dec  2014     Vivek Gupta           SPR 14153
 * Sep 2014      Vinod Vyas        Changes Related to eICIC 
 * Nov,2014      Karuna Bhardwaj   EICIC Enhancement
 * Copyright (c) 2014, Aricent Inc. All Rights Reserved
 *********************************************************************************/
#include <oam_defines.h>
#include <oam_sim.h>
#include <oam_sim_utils.h>
#include <oam_config_tcb.h>
#include <oam_types.h>
#include <oam_proto.h>
#include<oam_config_mgmt.h>
#include<oam_fault_mgmt.h>
#include <cspl.h>
#include <stacklayer.h>
#ifdef OAM_RADIO_HEAD_SUPPORT
#ifdef OAM_RRH_TYPE_1_SUPPORT
#include<oam_rcm_proto.h>
#include <oam_hm.h>
#endif
#endif
#ifdef OAM_EVM_TARGET
#include <oam_vendor_svcs.h>
#endif
/* SPR 16567 START */
#include <oam_s1ap_il_composer.h>
/* SPR 16567 END */

/* CSR 57877 FIx Start */
#include <oam_mgmt_intf_api.h>
/* CSR 57877 FIx End*/


#include <oam_son_composer.h>
#include <oam_rrm_composer.h>
/* SPR 14082 START */
#include<oam_fm_alarm_defs.h>
/* SPR 14082 END */


/* SPR 16200 FIX START */
#include<oam_xml_header.h>
/* SPR 16200 FIX END */

extern void
            /*SPR 17777 fix*/
oam_hm_timer_expiry(void);
extern void
oam_hm_non_responsive_timer_expiry(void);
            /*SPR 17777 fix*/
/* + SPR 17439 */
extern oam_return_et oam_start_hm(void);
/* - SPR 17439 */
extern oam_prov_req_t oam_prov_req;
/* Start: SPR 10567 */
extern oam_prov_req_t local_prov_req;
/*End: SPR 10567 */
oam_return_et
oam_cm_fsm_init(void);
oam_return_et oam_isValidIp4 (UInt8 *p_str);
/* + SPR 17439 */
extern  void oam_CoM_init_modules(void);
/* - SPR 17439 */
extern oam_ext_com_config_t comm_data;
extern UInt16 global_trans_flag;
extern UInt8 trace_start;
extern const QSYSOP* os;
extern const QWAIT* waitstruct;
extern const QSHELL* operations;
extern oam_cm_cntxt_t *p_cm_cntxt;
extern int g_no_of_arguments;
extern Char8 **g_list_of_init_arguments; 
extern UInt8 oam_sys_log;
prov_request_t prov_req;
/*SPR 21369 Start*/
extern lower_layer_init_info_t l2_comm_info[OAM_MAX_NUM_L2_INSTANCE];
/*SPR 21369 End */
/* SPR 16567 START */
extern UInt16 current_enb_config_count;
/* SPR 16567 END */
QLOCK pm_mutex_lock;                                         
/* SPR 21496 +- */ 

U8 num_active_threads=0;
thread_arg_t logger_thd_arg;

/* SPR_20467_FIX_START */
extern void oam_update_enb_config_info_from_current_mme_list(void);
extern oam_s1ap_ecu_resp_data_t *oam_process_tcb_for_ecu_resp(oam_s1ap_ecu_resp_data_t *p_oam_s1ap_ecu_resp_data,
                                                              UInt8* p_api_buf,UInt16 trans_id);
extern void * oam_maintain_tcb_for_ecu(UInt16 trans_id,
                                       oam_trans_type_et trans_type,
                                       UInt16 msg_size,
                                       void *p_api_buf,
                                       UInt8 procedure_specific_buf_len,
                                       void *p_oam_s1ap_ecu_resp_data,
                                       UInt8 old_ecu_sent_flag);
/* SPR_20467_FIX_END */

extern parameter_map_element_t *g_parameter_map_element;
extern oam_bool_et oam_is_sdm_alive;

rrm_void_t *oam_logger_init(oam_void_t *);
S32 oam_logger_process_msg( oam_void_t *, oam_void_t *p_gl_ctx);
oam_void_t oam_logger_process_timer_msg (QTIMER, oam_void_t *, oam_void_t *);

#ifdef OAM_SON_ENABLE
oam_layer_intf_resp_code_t oam_son_resp_code_tbl[] = {
	{ SON_ERR_ALREADY_ENABLED, "Already enabled!" },
	{ SON_ERR_ALREADY_DISABLED, "Already disabled!" },
	/* SPR 11156 FIX START */
	{ SON_ERR_ANR_NR_NOT_FOUND, "Neighbor not found" },
	{ SON_ERR_NMM_INTERNAL_FAILURE, "NMM internal failure" },
	{ SON_ERR_NMM_TIMER_EXPIRED, "NMM timer expired" },
	{ SON_ERR_NMM_STOP_MSG_RECVD, "NMM stop message received" },
	{ SON_ERR_NMM_MSG_SEND_FAILED, "NMM message send failed" },
	{ SON_ERR_NMM_INVALID_MSG_REC, "NMM invalid message received" },
	{ SON_ERR_NMM_PHY_NULL_DATA, "Irrelevant data received from PHY" },
	{ SON_ERR_NMM_FREQ_BAND_NOT_SUPPORTED_BY_HW,
		"Frequency band not supported by hardware" },
	{ SON_ERR_NMM_INVALID_EARFCN, "Invalid EARFCN" },
	{ SON_ERR_NMM_INVALID_NR_CARRIER_LIST, "Invalid NR Carrier List" },
	{ SON_ERR_NMM_INVALID_CARRIER_LIST, "Invalid Carrier List" },
	{ SON_ERR_NMM_STOP_SCAN_RUNNING,
		"A stop scan is running for the requested entity" },
	{ SON_ERR_NMM_START_SCAN_NOT_FOUND,
		"Start scan not found for the corresponding requested stop scan" },
	{ SON_ERR_NMM_SCAN_PREPAIR_FAILURE, "NMM preparation failure" },
	{ SON_ERR_NMM_MAX_ALLOWED_SCAN_REACHED, "Maximum allowed scan reached" },
	{ SON_ERR_NMM_UNKNOWN_FAILURE, "Unknown cause" },
	{ SON_ERR_INVALID_PARAMS, "Invalid parameter set received" },
	/* SPR 11156 FIX END */
	{ SON_ERR_UNUSED_PCI_NOT_FOUND, "Unused PCI not found" }
};
#endif
oam_layer_intf_resp_code_t oam_mac_resp_code_tbl[] = {
	/* CA TDD Changes Start */
	/* Code Removed */
	/* CA TDD Changes End */
	{ MAC_INIT_LAYER_REQ_MULTI_CELL_WITHOUT_DL_UL_SPLIT,
		"Multi cell is configured without DL UL split" },
	{ MAC_INIT_LAYER_REQ_CA_INVALID_SCH_STRATEGY,
		"CA is configured with strategy set to PFS or RR" },
	{ MAC_INIT_LAYER_REQ_INVALID_NUM_OF_CELLS,
		"Number of cells configured is greater than the maximum allowed" },
	{ MAC_LAYER_ALREADY_INITIALIZED, "Cell already configured" },
	{ MAC_LAYER_CELL_IGNORE, "Cell configuration ignored at MAC" }
}; 

oam_layer_intf_resp_code_t oam_rrc_resp_code_tbl[] = {
	{ RRC_OAM_COMM_INFO_FAILURE, "RRC communication info failure" },
	{ RRC_OAM_CELL_INDEX_MISMATCH, "Cell index mismatch" }
};

oam_layer_intf_resp_code_t oam_rrm_resp_code_tbl[] = {
	{ RRM_ERR_LOG_ENABLE_FAILURE, "Failed to enable log" },
	{ RRM_ERR_INIT_CONFIG_FAILURE, "layer initial configuration failed" },
	{ RRM_ERR_CELL_ALREADY_CONFIGURED, "Cell already configured" },
	{ RRM_ERR_CELL_START_FAILURE, "Cell start failed" },
	{ RRM_ERR_CELL_CONFIG_FAILURE, "Cell configuration failed" },
	{ RRM_ERR_UNEXPECTED_MSG, "Unexpected message received" },
	/* SPR 13024 FIX START */
	{ RRM_ERR_CELL_UNCONFIGURED, "Cell data not found" },
	{ RRM_ERR_CELL_DELETION_EXPECTED, "Cell Deleteion is possible" }
	/* SPR 13024 FIX END */
};

/****************************************************************************
 * Function Name  : oam_handle_invalid_startup_config
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function handles the invalid startup configuration
 ****************************************************************************/

/* + SPR 17439 */
void oam_handle_invalid_startup_config(void)
/* - SPR 17439 */
{
	oam_prov_req.fm_cntxt.isRaiseConfigAlarm = OAM_ONE;
}


/****************************************************************************
 * Function Name  : Pack
 * Inputs         : module : Module
buffer :Pointer to void
size :Pointer to buffer size 
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used for pack.
 ****************************************************************************/

static void *Pack(QMODULE module, void *buffer, U32 *size)
{
    /* SPR 17777 fix */
    OAM_LOG(OAM,OAM_DETAILEDALL,"unused parameter %p and %p", size, module);
	return buffer;
}


/****************************************************************************
 * Function Name  : Alloc
 * Inputs         : module :Module
attrsize :attr size
 * Outputs        : None
 * Returns        : None
 * Description    : This function is used for alloc purpose.
 ****************************************************************************/
/* Compose stack */
static void *Alloc(QMODULE module, U32 attrsize)
{
    /* SPR 17777 fix */
    OAM_LOG(OAM,OAM_DETAILEDALL,"unused parameter %p", module);
	return qvMsgAllocBySize( attrsize );
}
/********************************PM_CODE**************************************/
LOCAL const unsigned long PMDestList[] =
{
	MAC_MODULE_ID,
	RLC_MODULE_ID,
	PDCP_MODULE_ID,
	EGTPU_MODULE_ID,
	QCLI_MODULE_ID,
	OAM_ZERO
};

const   QMANIFEST  PMEntity =
{
	"PM_MODULE_ID",
	PM_MODULE_ID,
	{ OAM_ZERO, OAM_ZERO },
	{(S32 (*)(void *, void *))oam_PM_intf_process_msg,(void (*)(QTIMER, void *, void *))oam_PM_intf_process_timer},
	{(void *(*)(QMODULE, void *, U32 *))Pack,OAM_ZERO,(void *(*)(QMODULE, U32))Alloc},
	PMDestList
};

#ifdef OAM_SHARED_MEM_LOGGER
static const unsigned long oamLoggerDestinationList[] =
{
        0    
};

const   QMANIFEST oamLoggerEntity =
{
	"OAM_LOGGER_MODULE_ID",
	OAM_LOGGER_MODULE_ID,

	{ OAM_PNULL, oam_logger_init },
	{ oam_logger_process_msg, oam_logger_process_timer_msg },
	{(void *(*)(QMODULE, void *, U32 *))Pack,OAM_ZERO,(void *(*)(QMODULE, U32))Alloc},
	oamLoggerDestinationList /* Does not send message to any module */
};
#endif

/********************************PM_CODE**************************************/
LOCAL const unsigned long LayerDestList[] =
{
	RRM_MODULE_ID,
	SON_MODULE_ID,
	MAC_MODULE_ID,
	RLC_MODULE_ID,
	PDCP_MODULE_ID,
	EGTPU_MODULE_ID,
	QCLI_MODULE_ID,
	TR069_MODULE_ID,
	OAM_SNMP_INTF_MODULE_ID,
#ifdef OAM_RADIO_HEAD_SUPPORT
#ifdef OAM_RRH_TYPE_1_SUPPORT
	RCM_MODULE_ID,
#endif
#endif
	OAM_ZERO
};

LOCAL const unsigned long FMDestList[] =
{
	OAM_QCLI_TRAP_MODULE_ID,
	TR069_MODULE_ID,
	OAM_ZERO
};


const   QMANIFEST  LayerEntity =
{

	"OAM_MODULE_ID",
	OAM_MODULE_ID,
	{ OAM_ZERO, OAM_ZERO },
	{(S32 (*)(void *, void *))oam_layer_intf_process_msg,(void (*)(QTIMER, void *, void *))oam_layer_intf_process_timer},
	{(void *(*)(QMODULE, void *, U32 *))Pack,OAM_ZERO,(void *(*)(QMODULE, U32))Alloc},
	LayerDestList
};

/* Fault Management Change Start */
const   QMANIFEST  FMEntity =
{

	"FM_MODULE_ID",
	FM_MODULE_ID,
	{ OAM_ZERO, OAM_ZERO },
	{(S32 (*)(void *, void *))oam_fm_intf_process_msg,(void (*)(QTIMER, void *, void *))oam_fm_intf_process_timer},
	{(void *(*)(QMODULE, void *, U32 *))Pack,OAM_ZERO,(void *(*)(QMODULE, U32))Alloc},
	FMDestList
};

/* Fault Management Change End */

LOCAL const unsigned long QCLIDestList[] =
{
	QCLI_MODULE_ID,
	TR069_MODULE_ID,
	OAM_SDM_MODULE_ID, 
	OAM_SNMP_MODULE_ID,
	OAM_SNMP_INTF_MODULE_ID,
	OAM_ZERO
};

const   QMANIFEST  QCLIEntity =
{
	"QCL_INTERFACE_MODULE_ID",
	QCLI_INTERFACE_MODULE_ID,
	{ OAM_ZERO, OAM_ZERO },
	{(S32 (*)(void *, void *))oam_user_intf_process_msg,(void (*)(QTIMER, void *, void *))oam_user_intf_process_timer},
	{(void *(*)(QMODULE, void *, U32 *))Pack,OAM_ZERO,(void *(*)(QMODULE, U32))Alloc},

	QCLIDestList
};

extern oam_sim_table_t g_oam_sim_t;
oam_return_et
oam_il_parse_proc_sup_resp
(
 S32                         *p_response,
 U8                          *p_src,
 S32                         length_left,
 S32                         *p_length_read
 );

/* SPR_13116_CHANGE_START */
/*SPR 13617 START*/
/****************************************************************************
 * Function Name  : oam_l3_get_loglevel_resp
 * Description    : This function sends L3 current loglevel of response to oam
 * Inputs         : p_api_buf :pointer to SON get loglevel response
 * Outputs        : p_error_code :Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_l3_get_loglevel_resp
(
 oam_rrc_get_log_level_resp_t *p_rrc_get_log_level_resp,
 void *p_api_buf

 )
{
	OAM_FUN_ENTRY_TRACE();
	UInt16 tcb_detail = get_word_from_header(p_api_buf + 0);

	OAM_LOG(OAM,OAM_DETAILED,"  GET RRC LOG LEVEL INFORMATION ");

	UInt8 *p_buffer=OAM_NULL;
	UInt8 *p_buf=OAM_NULL;
	oam_error_code_et error_code;
	UInt32 tcb_id =tcb_detail;
	UInt8 *p_req_msg=OAM_NULL;
	UInt16 dest_id = OAM_ZERO;
	UInt16 msg_size=sizeof(oam_rrc_get_log_level_resp_t);

	oam_struct_t *node = oam_tcb_get_detail(&tcb_id);
	if(node)
	{
		p_req_msg = node->oam_tcb_context;
		dest_id = get_word_from_header((UInt8*)(p_req_msg) + OAM_TWO);
		if(node->timer_id!=OAM_ZERO)
		{
			if(qvTimerRunning((QTIMER)(node->timer_id))==OAM_ONE)
			{
				OAM_LOG(OAM,OAM_BRIEF,"Stopping Timer: Id(%p)",
                        (QTIMER)(node->timer_id));
				qvTimerStop((QTIMER)(node->timer_id),OAM_NULL);
			}
			else
			{
				OAM_LOG(OAM,OAM_WARNING," Trying to stop inavlid timer");
			}
		}
	}
	else
	{
		OAM_LOG(OAM,OAM_WARNING," Failure in fetching TCB entry");
		return OAM_FAILURE;
	}
	oam_tcb_delete(&tcb_id);
	if(OAM_FAILURE == oam_mem_alloc(msg_size+ OAM_MGMT_INTF_HEADER_SIZE +OAM_CSPL_HDR_SIZE,
				(void *)&p_buffer,
				&error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_buffer failed"
                " with Error Code:%d",
                error_code);
		OAM_FUN_EXIT_TRACE();
		return (OAM_FAILURE);
	}
	p_buf=p_buffer;
	oam_construct_api_header(p_buffer,OAM_VERSION,OAM_MODULE_ID,
			dest_id,
			OAM_GET_LOG_LEVEL_L3_RESP, msg_size+ OAM_MGMT_INTF_HEADER_SIZE +OAM_CSPL_HDR_SIZE);

	p_buffer += OAM_CSPL_HDR_SIZE;



	oam_construct_mgmt_api_hdr((UInt8 *)p_buffer,
			tcb_detail,
			OAM_MODULE_ID,
			dest_id,
			OAM_GET_LOG_LEVEL_L3_RESP,
			msg_size + OAM_MGMT_INTF_HEADER_SIZE,
		 OAM_ZERO);
            /*SPR 17777 fix*/
	p_buffer += OAM_MGMT_INTF_HEADER_SIZE;
	oam_memcpy(p_buffer,p_rrc_get_log_level_resp,msg_size );
	if(OAM_SUCCESS == oam_send_message(p_buf, dest_id,&error_code))
	{
		OAM_LOG(OAM,OAM_DETAILED,"Message Sending is successful");


	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/*SPR 13617 END*/

/***********************************************************************************
 * Function Name  : oam_l3_s1ap_get_loglevel_resp
 * Description    : This function sends L3 S1AP current loglevel of response to oam
 * Inputs         : p_api_buf :pointer to SON get loglevel response
 * Outputs        : p_error_code :Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ***********************************************************************************/
oam_return_et
oam_l3_s1ap_get_loglevel_resp
(
 oam_s1ap_get_log_level_resp_t *p_s1ap_get_log_level_resp,
 void *p_api_buf

 )
{
	OAM_FUN_ENTRY_TRACE();
	UInt16 tcb_detail = get_word_from_header(p_api_buf + 0);

	OAM_LOG(OAM,OAM_DETAILED,"  GET S1AP LOG LEVEL INFORMATION ");

	UInt8 *p_buffer=OAM_NULL;
	UInt8 *p_buf=OAM_NULL;
	oam_error_code_et error_code;
	UInt32 tcb_id =tcb_detail;
	UInt8 *p_req_msg=OAM_NULL;
	UInt16 dest_id = OAM_ZERO;
	UInt16 msg_size=sizeof(oam_s1ap_get_log_level_resp_t);

	oam_struct_t *node = oam_tcb_get_detail(&tcb_id);
	if(node)
	{
		p_req_msg = node->oam_tcb_context;
		dest_id = get_word_from_header((UInt8*)(p_req_msg) + OAM_TWO);
		if(node->timer_id!=OAM_ZERO)
		{
			if(qvTimerRunning((QTIMER)(node->timer_id))==OAM_ONE)
			{
				OAM_LOG(OAM,OAM_BRIEF,"Stopping Timer: Id(%p)",
                        (QTIMER)(node->timer_id));
				qvTimerStop((QTIMER)(node->timer_id),OAM_NULL);
			}
			else
			{
				OAM_LOG(OAM,OAM_WARNING," Trying to stop inavlid timer");
			}
		}
	}
	else
	{
		OAM_LOG(OAM,OAM_WARNING," Failure in fetching TCB entry");
		return OAM_FAILURE;
	}
	oam_tcb_delete(&tcb_id);
	if(OAM_FAILURE == oam_mem_alloc(msg_size+ OAM_MGMT_INTF_HEADER_SIZE +OAM_CSPL_HDR_SIZE,
				(void *)&p_buffer,
				&error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_buffer failed"
                " with Error Code:%d",
                error_code);
		OAM_FUN_EXIT_TRACE();
		return (OAM_FAILURE);
	}
	p_buf=p_buffer;
	oam_construct_api_header(p_buffer,OAM_VERSION,OAM_MODULE_ID,
			dest_id,
			OAM_GET_LOG_LEVEL_L3_S1AP_RESP, msg_size+ OAM_MGMT_INTF_HEADER_SIZE +OAM_CSPL_HDR_SIZE);

	p_buffer += OAM_CSPL_HDR_SIZE;



	oam_construct_mgmt_api_hdr((UInt8 *)p_buffer,
			tcb_detail,
			OAM_MODULE_ID,
			dest_id,
			OAM_GET_LOG_LEVEL_L3_S1AP_RESP,
			msg_size + OAM_MGMT_INTF_HEADER_SIZE,
		 OAM_ZERO);
            /*SPR 17777 fix*/
	p_buffer += OAM_MGMT_INTF_HEADER_SIZE;
	oam_memcpy(p_buffer,p_s1ap_get_log_level_resp,msg_size );
	if(OAM_SUCCESS == oam_send_message(p_buf, dest_id,&error_code))
	{
		OAM_LOG(OAM,OAM_DETAILED,"Message Sending is successful");


	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_l3_x2ap_get_loglevel_resp
 * Description    : This function sends L3 X2AP current loglevel of response to oam
 * Inputs         : p_api_buf :pointer to SON get loglevel response
 * Outputs        : p_error_code :Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_l3_x2ap_get_loglevel_resp
(
 oam_x2ap_get_log_level_resp_t *p_x2ap_get_log_level_resp,
 void *p_api_buf

 )
{
	OAM_FUN_ENTRY_TRACE();
	UInt16 tcb_detail = get_word_from_header(p_api_buf + 0);

	OAM_LOG(OAM,OAM_DETAILED,"  GET X2AP LOG LEVEL INFORMATION ");

	UInt8 *p_buffer=OAM_NULL;
	UInt8 *p_buf=OAM_NULL;
	oam_error_code_et error_code;
	UInt32 tcb_id =tcb_detail;
	UInt8 *p_req_msg=OAM_NULL;
	UInt16 dest_id = OAM_ZERO;
	UInt16 msg_size=sizeof(oam_x2ap_get_log_level_resp_t);

	oam_struct_t *node = oam_tcb_get_detail(&tcb_id);
	if(node)
	{
		p_req_msg = node->oam_tcb_context;
		dest_id = get_word_from_header((UInt8*)(p_req_msg) + OAM_TWO);
		if(node->timer_id!=OAM_ZERO)
		{
			if(qvTimerRunning((QTIMER)(node->timer_id))==OAM_ONE)
			{
				OAM_LOG(OAM,OAM_BRIEF,"Stopping Timer: Id(%p)",
                        (QTIMER)(node->timer_id));
				qvTimerStop((QTIMER)(node->timer_id),OAM_NULL);
			}
			else
			{
				OAM_LOG(OAM,OAM_WARNING," Trying to stop inavlid timer");
			}
		}
	}
	else
	{
		OAM_LOG(OAM,OAM_WARNING," Failure in fetching TCB entry");
		return OAM_FAILURE;
	}
	oam_tcb_delete(&tcb_id);
	if(OAM_FAILURE == oam_mem_alloc(msg_size+ OAM_MGMT_INTF_HEADER_SIZE +OAM_CSPL_HDR_SIZE,
				(void *)&p_buffer,
				&error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_buffer failed"
                " with Error Code:%d",
                error_code);
		OAM_FUN_EXIT_TRACE();
		return (OAM_FAILURE);
	}
	p_buf=p_buffer;
	oam_construct_api_header(p_buffer,OAM_VERSION,OAM_MODULE_ID,
			dest_id,
			OAM_GET_LOG_LEVEL_L3_X2AP_RESP, msg_size+ OAM_MGMT_INTF_HEADER_SIZE +OAM_CSPL_HDR_SIZE);

	p_buffer += OAM_CSPL_HDR_SIZE;



	oam_construct_mgmt_api_hdr((UInt8 *)p_buffer,
			tcb_detail,
			OAM_MODULE_ID,
			dest_id,
			OAM_GET_LOG_LEVEL_L3_X2AP_RESP,
			msg_size + OAM_MGMT_INTF_HEADER_SIZE,
            /*SPR 17777 fix*/
			OAM_ZERO);
	p_buffer += OAM_MGMT_INTF_HEADER_SIZE;
	oam_memcpy(p_buffer,p_x2ap_get_log_level_resp,msg_size );
	if(OAM_SUCCESS == oam_send_message(p_buf, dest_id,&error_code))
	{
		OAM_LOG(OAM,OAM_DETAILED,"Message Sending is successful");


	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}


/****************************************************************************
 * Function Name  : oam_rrm_get_loglevel_resp
 * Description    : This function provide RRM current loglevel response to CLI
 * Inputs         : p_api_buf :pointer to RRM get loglevel response
 * Outputs        : p_error_code :Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_rrm_get_loglevel_resp
(
 oam_rrm_get_log_level_resp_t   *p_oam_get_loglevel_rrm,
 void* p_api_buf

 )
{
	OAM_FUN_ENTRY_TRACE();
	UInt16 tcb_detail = get_word_from_header(p_api_buf + 0);

	OAM_LOG(OAM,OAM_DETAILED,"  GET RRM LOG LEVEL INFORMATION ");

	UInt8 *p_buffer=OAM_NULL;
	UInt8 *p_buf=OAM_NULL;
	oam_error_code_et error_code;
	UInt32 tcb_id =tcb_detail;
	UInt8 *p_req_msg=OAM_NULL;
	UInt16 dest_id = OAM_ZERO;
	UInt16 msg_size=sizeof(oam_rrm_get_log_level_resp_t);



	oam_struct_t *node = oam_tcb_get_detail(&tcb_id);
	if(node)
	{
		p_req_msg = node->oam_tcb_context;
		dest_id = get_word_from_header((UInt8*)(p_req_msg) + OAM_TWO);
		if(node->timer_id!=OAM_ZERO)
		{
			if(qvTimerRunning((QTIMER)(node->timer_id))==OAM_ONE)
			{
				OAM_LOG(OAM,OAM_BRIEF,"Stopping Timer: Id(%p)",
                        (QTIMER)(node->timer_id));
				qvTimerStop((QTIMER)(node->timer_id),OAM_NULL);
			}
			else
			{
				OAM_LOG(OAM,OAM_WARNING,"Trying to stop inavlid timer");
			}
		}
	}
	else
	{
		OAM_LOG(OAM,OAM_WARNING,"Failure in fetching TCB entry");
		return OAM_FAILURE;
	}
	oam_tcb_delete(&tcb_id);
	if(OAM_FAILURE == oam_mem_alloc(msg_size+ OAM_MGMT_INTF_HEADER_SIZE +OAM_CSPL_HDR_SIZE,
				(void *)&p_buffer,
				&error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_buffer failed"
                " with Error Code:%d",
                error_code);
		OAM_FUN_EXIT_TRACE();
		return (OAM_FAILURE);
	}
	p_buf=p_buffer;
	oam_construct_api_header(p_buffer,OAM_VERSION,OAM_MODULE_ID,
			dest_id,
			OAM_GET_LOG_LEVEL_RRM_RESP, msg_size+ OAM_MGMT_INTF_HEADER_SIZE +OAM_CSPL_HDR_SIZE);

	p_buffer += OAM_CSPL_HDR_SIZE;

	oam_construct_mgmt_api_hdr((UInt8 *)p_buffer,
			tcb_detail,
			OAM_MODULE_ID,
			dest_id,
			OAM_GET_LOG_LEVEL_RRM_RESP,
			msg_size + OAM_MGMT_INTF_HEADER_SIZE,
			OAM_ZERO);
            /*SPR 17777 fix*/
	p_buffer += OAM_MGMT_INTF_HEADER_SIZE;
	oam_memcpy(p_buffer,p_oam_get_loglevel_rrm ,msg_size );
	if(OAM_SUCCESS == oam_send_message(p_buf, dest_id,&error_code))
	{
		OAM_LOG(OAM,OAM_DETAILED,"Message Sending is successful");


	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}


#ifdef OAM_SON_ENABLE
/****************************************************************************
 * Function Name  : oam_son_get_loglevel_resp
 * Description    : This function sends SON current loglevel of response to oam
 * Inputs         : p_api_buf :pointer to SON get loglevel response
 * Outputs        : p_error_code :Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
/* SPR 17777 fix */
oam_return_et
oam_son_get_loglevel_resp
(
 oam_son_get_log_level_resp_t *p_oam_get_loglevel_son,
 void *p_api_buf
 )
{
	OAM_FUN_ENTRY_TRACE();
	UInt16 tcb_detail = get_word_from_header(p_api_buf + 0);

	OAM_LOG(OAM,OAM_DETAILED,"  GET SON LOG LEVEL INFORMATION ");

	UInt8 *p_buffer=OAM_NULL;
	UInt8 *p_buf=OAM_NULL;
	oam_error_code_et error_code;
	UInt32 tcb_id =tcb_detail;
	UInt8 *p_req_msg=OAM_NULL;
	UInt16 dest_id = OAM_ZERO;
	UInt16 msg_size=sizeof(oam_son_get_log_level_resp_t);

	oam_struct_t *node = oam_tcb_get_detail(&tcb_id);
	if(node)
	{
		p_req_msg = node->oam_tcb_context;
		dest_id = get_word_from_header((UInt8*)(p_req_msg) + OAM_TWO);
		if(node->timer_id!=OAM_ZERO)
		{
			if(qvTimerRunning((QTIMER)(node->timer_id))==OAM_ONE)
			{
				OAM_LOG(OAM,OAM_BRIEF,"Stopping Timer: Id(%p)",
                        (QTIMER)(node->timer_id));
				qvTimerStop((QTIMER)(node->timer_id),OAM_NULL);
			}
			else
			{
				OAM_LOG(OAM,OAM_WARNING,"Trying to stop inavlid timer");
			}
		}
	}
	else
	{
		OAM_LOG(OAM,OAM_WARNING,"Failure in fetching TCB entry");
		return OAM_FAILURE;
	}
	oam_tcb_delete(&tcb_id);
	if(OAM_FAILURE == oam_mem_alloc(msg_size+ OAM_MGMT_INTF_HEADER_SIZE +OAM_CSPL_HDR_SIZE,
				(void *)&p_buffer,
				&error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_buffer failed"
                " with Error Code:%d",
                error_code);
		OAM_FUN_EXIT_TRACE();
		return (OAM_FAILURE);
	}
	p_buf=p_buffer;
	oam_construct_api_header(p_buffer,OAM_VERSION,OAM_MODULE_ID,
			dest_id,
			OAM_GET_LOG_LEVEL_SON_RESP, msg_size+ OAM_MGMT_INTF_HEADER_SIZE +OAM_CSPL_HDR_SIZE);

	p_buffer += OAM_CSPL_HDR_SIZE;



	oam_construct_mgmt_api_hdr((UInt8 *)p_buffer,
			tcb_detail,
			OAM_MODULE_ID,
			dest_id,
			OAM_GET_LOG_LEVEL_SON_RESP,
			msg_size + OAM_MGMT_INTF_HEADER_SIZE,
			OAM_ZERO);
            /*SPR 17777 fix*/
	p_buffer += OAM_MGMT_INTF_HEADER_SIZE;
	oam_memcpy(p_buffer,p_oam_get_loglevel_son ,msg_size );
	if(OAM_SUCCESS == oam_send_message(p_buf, dest_id,&error_code))
	{
		OAM_LOG(OAM,OAM_DETAILED,"Message Sending is successful");


	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}


/****************************************************************************
 * Function Name  : oam_son_send_get_log_level_req
 * Description    : This function sends request to MAC to get loglevel info
 * Inputs         : src_module_id   :Source module identifier
 *                : transaction_id  :transaction identifier
 * Outputs        : p_error_code    :pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_son_send_get_log_level_req
(
 oam_counter_t src_module_id,
 oam_counter_t transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t msg_length     = OAM_ZERO;
	oam_counter_t msg_api_length = OAM_ZERO;
	UInt8 *p_msg;
	UInt8 *p_mac_msg;
	p_msg          = OAM_NULL;
	p_mac_msg      = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	/* Allocate buffer */
	if(OAM_FAILURE == oam_mem_alloc(msg_api_length,(void*)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	oam_memset((void *)p_msg, OAM_ZERO, msg_api_length);
	p_mac_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_mac_msg, OAM_VERSION_ID, src_module_id,
			SON_MODULE_ID, SON_OAM_GET_LOG_LEVEL_REQ, msg_api_length);

	/* Fill interface header */
	p_mac_msg = p_mac_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_mac_msg,
			transaction_id,
			src_module_id,
			SON_MODULE_ID,
			SON_OAM_GET_LOG_LEVEL_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Send message to MAC */
	if(OAM_FAILURE == oam_send_message(p_msg, SON_MODULE_ID,p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending message to SON with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
#endif
/****************************************************************************
 * Function Name  : oam_send_get_log_level_resp
 * Inputs         : response :Response
transaction_id :transaction id
 * Outputs        : p_error_code :Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function sends the success/failure response
 ****************************************************************************/

oam_return_et
oam_send_get_log_level_resp(
		UInt16 transaction_id,
		UInt16 dest_id,
		oam_error_code_et *p_error_code
		)
{

	UInt8 * p_msg;
	UInt8 * p_resp_msg;
	UInt8 msg_len = OAM_ZERO;
	msg_len = (sizeof(UInt8)) + LOWER_LAYER_INTERFACE_HEADER_SIZE + OAM_CSPL_HDR_SIZE;

	if (OAM_FAILURE == oam_mem_alloc(msg_len,(void *)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		/* Not Enough Memory */
		return OAM_FAILURE;
	}

	p_resp_msg = p_msg;

	oam_construct_api_header(p_resp_msg, OAM_VERSION_ID,OAM_MODULE_ID,
			dest_id, OAM_GET_LOG_LEVEL_RESP,
			msg_len);

	p_resp_msg = p_resp_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_resp_msg,
			transaction_id,
			OAM_MODULE_ID,
			dest_id, OAM_GET_LOG_LEVEL_RESP,
			(msg_len - OAM_CSPL_HDR_SIZE),
			oam_prov_req.cell_cntxt.curr_cell_cid);

	p_resp_msg = p_resp_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	UInt8 resp_send;
	resp_send=prov_req.log_level;
	oam_memcpy(p_resp_msg,&resp_send,sizeof(UInt8));

	if(OAM_SUCCESS == oam_send_message(p_msg, dest_id,p_error_code))
	{
		OAM_LOG(OAM,OAM_DETAILED,"Sending message to Management Interface");
	}
	else
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending message with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	return OAM_SUCCESS;
}

/* SPR_13116_CHANGE_END */




/****************************************************************************
 * Function Name  : oam_send_success_failure_resp
 * Inputs         : response :Response
transaction_id :transaction id                     
 * Outputs        : p_error_code :Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function sends the success/failure response 
 ****************************************************************************/

oam_return_et
oam_send_success_failure_resp(
		oam_return_et response,
		UInt16 transaction_id,
		UInt16 dest_id,
		oam_error_code_et *p_error_code
		)
{

	UInt8 * p_msg;
	UInt8 * p_resp_msg;
	UInt8 msg_len = OAM_ZERO;
	msg_len = (sizeof(UInt8)) + LOWER_LAYER_INTERFACE_HEADER_SIZE + OAM_CSPL_HDR_SIZE;

	if (OAM_FAILURE == oam_mem_alloc(msg_len,(void *)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		/* Not Enough Memory */
		return OAM_FAILURE;
	}

	p_resp_msg = p_msg;

	oam_construct_api_header(p_resp_msg, OAM_VERSION_ID,OAM_MODULE_ID,
			dest_id, OAM_SET_LOG_LEVEL_RESP,
			msg_len);

	p_resp_msg = p_resp_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_resp_msg,
			transaction_id,
			OAM_MODULE_ID,
			dest_id, OAM_SET_LOG_LEVEL_RESP,
			(msg_len - OAM_CSPL_HDR_SIZE),
			oam_prov_req.cell_cntxt.curr_cell_cid);

	p_resp_msg = p_resp_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	UInt8 resp_send;
	if(OAM_SUCCESS == response){
		resp_send = OAM_ONE;
		oam_memcpy(p_resp_msg,&resp_send,sizeof(UInt8));
	}
	else{
		resp_send = OAM_ZERO;
		oam_memcpy(p_resp_msg,&resp_send,sizeof(UInt8));
	}

	if(OAM_SUCCESS == oam_send_message(p_msg, dest_id,p_error_code))
	{
		OAM_LOG(OAM,OAM_DETAILED,"Sending message to Management Interface");
	}
	else
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending message with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	return OAM_SUCCESS;
}

/* CSR_59405_FIX_START */
/****************************************************************************
 * Function Name  : oam_send_call_trace_start_resp
 * Inputs         : response :Response
transaction_id :transaction id
 * Outputs        : p_error_code :Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function sends the success/failure response
 ****************************************************************************/

oam_return_et
oam_send_call_trace_start_resp(
		oam_return_et response,
		UInt16 transaction_id,
		UInt16 dest_id,
		oam_error_code_et *p_error_code
		)
{

	UInt8 * p_msg;
	UInt8 * p_resp_msg;
	UInt8 msg_len = OAM_ZERO;
	msg_len = (sizeof(UInt8)) + LOWER_LAYER_INTERFACE_HEADER_SIZE + OAM_CSPL_HDR_SIZE;

	if (OAM_FAILURE == oam_mem_alloc(msg_len,(void *)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		/* Not Enough Memory */
		return OAM_FAILURE;
	}

	p_resp_msg = p_msg;

	oam_construct_api_header(p_resp_msg, OAM_VERSION_ID,OAM_MODULE_ID,
			dest_id, OAM_SET_CALL_TRACE_START_RESP,
			msg_len);

	p_resp_msg = p_resp_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_resp_msg,
			transaction_id,
			OAM_MODULE_ID,
			dest_id,
			OAM_SET_CALL_TRACE_START_RESP,
			(msg_len - OAM_CSPL_HDR_SIZE),
			oam_prov_req.cell_cntxt.curr_cell_cid);

	p_resp_msg = p_resp_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	UInt8 resp_send;
	if(OAM_SUCCESS == response){
		resp_send = OAM_ONE;
		oam_memcpy(p_resp_msg,&resp_send,sizeof(UInt8));
	}
	else{
		resp_send = OAM_ZERO;
		oam_memcpy(p_resp_msg,&resp_send,sizeof(UInt8));
	}

	if(OAM_SUCCESS == oam_send_message(p_msg, dest_id,p_error_code))
	{
		OAM_LOG(OAM,OAM_DETAILED,"Sending message to Management Interface");
	}
	else
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending message with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	return OAM_SUCCESS;
}
/****************************************************************************
 * Function Name  : oam_send_call_trace_stop_resp
 * Inputs         : response :Response
transaction_id :transaction id
 * Outputs        : p_error_code :Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function sends the success/failure response
 ****************************************************************************/

oam_return_et
oam_send_call_trace_stop_resp(
		oam_return_et response,
		UInt16 transaction_id,
		UInt16 dest_id,
		oam_error_code_et *p_error_code
		)
{

	UInt8 * p_msg;
	UInt8 * p_resp_msg;
	UInt8 msg_len = OAM_ZERO;
	msg_len = (sizeof(UInt8)) + LOWER_LAYER_INTERFACE_HEADER_SIZE + OAM_CSPL_HDR_SIZE;

	if (OAM_FAILURE == oam_mem_alloc(msg_len,(void *)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		/* Not Enough Memory */
		return OAM_FAILURE;
	}

	p_resp_msg = p_msg;

	oam_construct_api_header(p_resp_msg, OAM_VERSION_ID,OAM_MODULE_ID,
			dest_id, OAM_SET_CALL_TRACE_STOP_RESP,
			msg_len);

	p_resp_msg = p_resp_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_resp_msg,
			transaction_id,
			OAM_MODULE_ID,
			dest_id,
			OAM_SET_CALL_TRACE_STOP_RESP,
			(msg_len - OAM_CSPL_HDR_SIZE),
			oam_prov_req.cell_cntxt.curr_cell_cid);

	p_resp_msg = p_resp_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	UInt8 resp_send;
	if(OAM_SUCCESS == response){
		resp_send = OAM_ONE;
		oam_memcpy(p_resp_msg,&resp_send,sizeof(UInt8));
	}
	else{
		resp_send = OAM_ZERO;
		oam_memcpy(p_resp_msg,&resp_send,sizeof(UInt8));
	}

	if(OAM_SUCCESS == oam_send_message(p_msg, dest_id,p_error_code))
	{
		OAM_LOG(OAM,OAM_DETAILED,"Sending message to Management Interface");
	}
	else
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending message with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	return OAM_SUCCESS;
}
/* CSR_59405_FIX_END */

/***************************************************************************
 * Function Name  : oam_shut_down_single
 * Inputs         : p_proc_name :name of the process
 * Variables      : None
 * Description    : shut down particular process of oam
 ***************************************************************************/
oam_return_et
oam_shut_down_single
(
 const char *p_proc_name
 )
{
	UInt16 loop=OAM_ZERO;
	OAM_FUN_ENTRY_TRACE();
	for(loop=OAM_ZERO; loop < g_oam_sim_t.max_no_process; loop++)
	{
		if(oam_strcmp(g_oam_sim_t.p_info[loop].name,p_proc_name) == OAM_ZERO)
		{
			if(OAM_SUCCESS == oam_kill(g_oam_sim_t.p_info[loop].pid,OAM_SIG_KILL))
			{
				OAM_LOG(OAM, OAM_DETAILED,
						"process having pid : %d killed successfully",
						g_oam_sim_t.p_info[loop].pid);
				OAM_FUN_EXIT_TRACE();
				return OAM_SUCCESS;
			}
			else
			{
				OAM_LOG(OAM,OAM_WARNING,
						"process having pid : %d kill Fail",
						g_oam_sim_t.p_info[loop].pid);
				OAM_FUN_EXIT_TRACE();
				return OAM_FAILURE;
			}
		}
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : change_address_format
 * Inputs         : addr_str :Pointer to address string
addr :Pointer to address
 * Outputs        : address in desired form
 * Returns        : void
 * Description    : converts string address array into decimal form.
 ****************************************************************************/
void 
change_address_format(
		U8* addr_str,
		U8* addr
		)
{
	UInt8 index = OAM_ZERO;
	UInt8 index_for_decimal = OAM_ZERO;
	unsigned char value = OAM_ZERO;
	for(index = OAM_ZERO; addr_str[index] != '\0'; index ++)
	{
		if (addr_str[index] == '.')
		{
			addr[index_for_decimal] = value;
			value = OAM_ZERO;
			++index_for_decimal;
		}
		else
		{
			value = value * OAM_TEN + (addr_str[index] - '0' );
		}
	}
	addr[index_for_decimal] = value;
}

/******************************************************************************
 * Function Name  : get_config_info
 * Inputs         : None
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This funtion read config information for OAM.
 *****************************************************************************/

/* + SPR 17439 */
U32 get_config_info(void)
	/* + SPR 17439 */
{
	Char8   read_buf[OAM_CONFIG_FILE_LINE_MAX_SIZE] = {0};
	Char8   tempStr[STR_LEN_16] = {OAM_ZERO};
	/* SPR_17377_Warning_fix + */
	UInt32  flag          = OAM_ZERO;
	/* SPR_17377_Warning_fix - */ 
	FILE *fp           = PNULL;
	UInt8 ipaddr[MAX_IP_ADD_LEN_STR] = {OAM_ZERO};
    /* SPR 20908 Start */
    /* Code Deleted */
    /* SPR 20908 End */
	fp = oam_fopen(OAM_ENB_CONFIG_FILE_NAME, "r");
	if (PNULL == fp)    
	{
		OAM_LOG(OAM,OAM_ERROR,"Not able to open cfg fileShutting down eNodeB");
		oam_handle_invalid_startup_config();
		return OAM_FAILURE;
	}    
	while((PNULL != fp) && (OAM_FGETS(read_buf,STR_LEN_100,fp)))
	{
		if(read_buf[0] != OAM_CFG_COMMENT_START_CHAR &&
				read_buf[0] != OAM_CFG_NEW_LINE_CHAR) {
			if(snscanf(read_buf,sizeof("ENB_OAM_IP_ADDRESS=") + MAX_IP_ADD_LEN_STR,"ENB_OAM_IP_ADDRESS=%s",comm_data.oam_com_params.ipaddr) > OAM_ZERO )
			{
				if( !oam_isValidIp4(comm_data.oam_com_params.ipaddr) )
				{
					OAM_LOG(OAM,OAM_ERROR,"ENB_OAM_IP_ADDRESS is INVALID. Shutting down eNodeB");
					oam_handle_invalid_startup_config();
					if(fp != OAM_NULL)
						oam_fclose(fp);
					return OAM_FAILURE;
				}
				change_address_format(comm_data.oam_com_params.ipaddr,prov_req.commn_info.ext_comm_info_upper_layer[OAM_INDEX].ip_addr);
				OAM_LOG(OAM,OAM_DETAILED,"--->comm_data.oam_com_params.ipaddr = %s."
				    " ext_comm_info[OAM_INDEX].ip_addri[0] %d ip_addri[1] %d ip_addri[2] ip_addri[3] %d.",
				    comm_data.oam_com_params.ipaddr,
				    prov_req.commn_info.ext_comm_info_upper_layer[OAM_INDEX].ip_addr[OAM_ZERO],
				    prov_req.commn_info.ext_comm_info_upper_layer[OAM_INDEX].ip_addr[OAM_ONE],
				    prov_req.commn_info.ext_comm_info_upper_layer[OAM_INDEX].ip_addr[OAM_TWO],
				    prov_req.commn_info.ext_comm_info_upper_layer[OAM_INDEX].ip_addr[OAM_THREE]);
				flag = (flag << OAM_ONE) | OAM_ONE;
			}
			else if (snscanf(read_buf,sizeof("ENB_OAM_RX_PORT=")+MAX_PORT_LENGTH,"ENB_OAM_RX_PORT=%s",comm_data.oam_com_params.port) > OAM_ZERO )
			{
				prov_req.commn_info.ext_comm_info_upper_layer[OAM_INDEX].ext_module_id = RRC_OAM_MODULE_ID;
				prov_req.commn_info.ext_comm_info_upper_layer[OAM_INDEX].transport_mode = OAM_TRANSPORT_MODE_UDP;
				prov_req.commn_info.ext_comm_info_upper_layer[OAM_INDEX].bitmask = RRC_OAM_IP_ADDR_PRESENT;
				prov_req.commn_info.ext_comm_info_upper_layer[OAM_INDEX].port = oam_atoi((const char *)comm_data.oam_com_params.port);
				OAM_LOG(OAM,OAM_DETAILED,"--->read_buf = %s. --->after atoi = %d",
                    read_buf, prov_req.commn_info.ext_comm_info_upper_layer[OAM_INDEX].port);

				if(prov_req.commn_info.ext_comm_info_upper_layer[OAM_INDEX].port <= EXT_COMM_INFO_PORT )
				{
					OAM_LOG(OAM,OAM_ERROR,"ENB_OAM_RX_PORT is INVALID using System PORT. Shutting down eNodeB");
					oam_handle_invalid_startup_config();
					if(fp != OAM_NULL)
						oam_fclose(fp);
					return OAM_FAILURE;
				}

				flag = (flag << OAM_ONE) | OAM_ONE;
			}
/*SPR 20908+- */
			else if(snscanf(read_buf,sizeof("ENB_RRM_IP_ADDRESS=")+MAX_IP_ADD_LEN_STR,"ENB_RRM_IP_ADDRESS=%s",ipaddr) > OAM_ZERO )
			{
				if( !oam_isValidIp4(comm_data.oam_com_params.ipaddr) )
				{
					OAM_LOG(OAM,OAM_ERROR,"ENB_RRM_IP_ADDRESS is INVALID. Shutting down eNodeB");
					oam_handle_invalid_startup_config();
					if(fp != OAM_NULL)
						oam_fclose(fp);
					return OAM_FAILURE;
				}             
				change_address_format(ipaddr,prov_req.commn_info.ext_comm_info_upper_layer[RRM_INDEX].ip_addr);
				flag = (flag << OAM_ONE) | OAM_ONE;
			}
			else if (snscanf(read_buf,sizeof("ENB_RRM_RX_PORT=")+MAX_PORT_LENGTH,"ENB_RRM_RX_PORT=%s",tempStr) > OAM_ZERO )
			{
				prov_req.commn_info.ext_comm_info_upper_layer[RRM_INDEX].port = oam_atoi((const char *)tempStr);
				prov_req.commn_info.ext_comm_info_upper_layer[RRM_INDEX].ext_module_id = RRC_RRM_MODULE_ID;
				prov_req.commn_info.ext_comm_info_upper_layer[RRM_INDEX].transport_mode = OAM_TRANSPORT_MODE_UDP;
				prov_req.commn_info.ext_comm_info_upper_layer[RRM_INDEX].bitmask = RRC_OAM_IP_ADDR_PRESENT;
				if(  prov_req.commn_info.ext_comm_info_upper_layer[RRM_INDEX].port <= EXT_COMM_INFO_PORT )
				{
					OAM_LOG(OAM,OAM_ERROR,"ENB_RRM_RX_PORT is INVALID using System PORT. Shutting down eNodeB");
					oam_handle_invalid_startup_config();
					if(fp != OAM_NULL)
						oam_fclose(fp);
					return OAM_FAILURE;
				}     
				flag = (flag << OAM_ONE) | OAM_ONE;
			}
			else if(snscanf(read_buf,
						sizeof("ENB_L3_IP_ADDRESS=") + MAX_IP_ADD_LEN_STR,
						"ENB_L3_IP_ADDRESS=%s",
						comm_data.rrc_s1ap_com_params.ipaddr) > OAM_ZERO)
			{
				if(!oam_isValidIp4(comm_data.oam_com_params.ipaddr)) {
					OAM_LOG(OAM, OAM_ERROR,
							"ENB_L3_IP_ADDRESS is INVALID."
							" Shutting down eNodeB");
					oam_handle_invalid_startup_config();
					if(fp != OAM_NULL)
						oam_fclose(fp);
					return OAM_FAILURE;
				}
				change_address_format(comm_data.rrc_s1ap_com_params.ipaddr,
						prov_req.commn_info.ext_comm_info_upper_layer[RRC_INDEX].ip_addr);
				flag = (flag << OAM_ONE) | OAM_ONE;
			}
			else if(snscanf(read_buf,sizeof("ENB_L3_PORT=")+MAX_PORT_LENGTH,"ENB_L3_PORT=%s",comm_data.rrc_s1ap_com_params.port) > OAM_ZERO )
			{
				prov_req.commn_info.ext_comm_info_upper_layer[RRC_INDEX].ext_module_id = RRC_MODULE_ID;
				prov_req.commn_info.ext_comm_info_upper_layer[RRC_INDEX].transport_mode = OAM_TRANSPORT_MODE_UDP;
				prov_req.commn_info.ext_comm_info_upper_layer[RRC_INDEX].bitmask = RRC_OAM_IP_ADDR_PRESENT;
				prov_req.commn_info.ext_comm_info_upper_layer[RRC_INDEX].port = oam_atoi((const char *)comm_data.rrc_s1ap_com_params.port);
				if(oam_atoi((Char8*)comm_data.rrc_s1ap_com_params.port) <= EXT_COMM_INFO_PORT)
				{
					OAM_LOG(OAM,OAM_ERROR,"ENB_RRC_RX_PORT is INVALID using System PORT. Shutting down eNodeB");
					oam_handle_invalid_startup_config();
					if(fp != OAM_NULL)
						oam_fclose(fp);
					return OAM_FAILURE;
				}     
				flag = (flag << OAM_ONE) | OAM_ONE;
			}
			/* NMM_AB+ */
			else if(snscanf(read_buf,sizeof("ENB_NMM_IP_ADDRESS=")+MAX_IP_ADD_LEN_STR,"ENB_NMM_IP_ADDRESS=%s",comm_data.nmm_com_params.ipaddr) > OAM_ZERO )
			{
				if( !oam_isValidIp4(comm_data.oam_com_params.ipaddr) )
				{
					OAM_LOG(OAM,OAM_ERROR,"ENB_NMM_IP_ADDRESS is INVALID. Shutting down eNodeB");
					oam_handle_invalid_startup_config();
					if(fp != OAM_NULL)
						oam_fclose(fp);
					return OAM_FAILURE;
				}             
				flag = (flag << OAM_ONE) | OAM_ONE;
			}
			else if (snscanf(read_buf,sizeof("ENB_NMM_RX_PORT=")+MAX_PORT_LENGTH,"ENB_NMM_RX_PORT=%s",comm_data.nmm_com_params.port) > OAM_ZERO )
			{
				flag = (flag << OAM_ONE) | OAM_ONE;
			}
			else if(snscanf(read_buf,sizeof("ENB_SDM_IP_ADDRESS=")+MAX_IP_ADD_LEN_STR,"ENB_SDM_IP_ADDRESS=%s",comm_data.sdm_com_params.ipaddr) > OAM_ZERO )
			{
				if( !oam_isValidIp4(comm_data.sdm_com_params.ipaddr) )
				{
					OAM_LOG(OAM,OAM_ERROR,"LTE_SDM_IP_ADDRESS is INVALID. Shutting down eNodeB");
					oam_handle_invalid_startup_config();
					if(fp != OAM_NULL)
						oam_fclose(fp);
					return OAM_FAILURE;
				}
				flag = (flag << OAM_ONE) | OAM_ONE;
			}
			else if (snscanf(read_buf,sizeof("ENB_SDM_RX_PORT=")+MAX_PORT_LENGTH,"ENB_SDM_RX_PORT=%s",comm_data.sdm_com_params.port) > OAM_ZERO )
			{
				flag = (flag << OAM_ONE) | OAM_ONE;
			}
#ifdef OAM_SON_ENABLE
			else if(snscanf(read_buf,
		    /*SPR 21881 Start*/
						sizeof("ENB_SON_IP_ADDRESS=") + MAX_IP_ADD_LEN_STR,
						"ENB_SON_IP_ADDRESS=%s",
						comm_data.son_com_params.ipaddr) > OAM_ZERO)
			{
				if(!oam_isValidIp4(comm_data.son_com_params.ipaddr)) {
					OAM_LOG(OAM, OAM_ERROR, "ENB_SON_IP_ADDRESS is INVALID."
							" Shutting down eNodeB");
		        /*SPR 21881 End*/
					oam_handle_invalid_startup_config();
					if(fp != OAM_NULL)
						oam_fclose(fp);
					return OAM_FAILURE;
				}

				change_address_format(comm_data.son_com_params.ipaddr,
						X2AP_PROV_REQ.son_comm_info.ip_addr);
				OAM_LOG(OAM, OAM_DETAILED,
						"--->comm_data.son_com_params.ipaddr = %s",
						comm_data.son_com_params.ipaddr);
				X2AP_PROV_REQ.bitmask |= SON_MODULE_INFO_PRESENT;
				flag = (flag << OAM_ONE) | OAM_ONE;
			}
            /*SPR 21881 Start*/
			else if(snscanf(read_buf,
						sizeof("ENB_SON_RRC_RX_PORT=") + MAX_PORT_LENGTH,
						"ENB_SON_RRC_RX_PORT=%s",
						comm_data.son_com_params.port) > OAM_ZERO)
            /*SPR 21881 End*/
			{
				X2AP_PROV_REQ.son_comm_info.port =
					(UInt16)oam_atoi((Char8*)comm_data.son_com_params.port);
				OAM_LOG(OAM, OAM_DETAILED,
						"--->comm_data.son_com_params.port = %d",
						X2AP_PROV_REQ.son_comm_info.port);
				flag = (flag << OAM_ONE) | OAM_ONE;
			}
#endif
		}
	}
	if(fp != OAM_NULL)
		oam_fclose(fp);
#ifdef OAM_SON_ENABLE
	if (0x1FFFF == flag) 
	{
		return OAM_SUCCESS;
	}
	else
	{
		OAM_LOG(OAM,OAM_ERROR,"Missing Ip and/or Port required by OAM in Config file");
		return OAM_FAILURE;
	}
#else
	if (0x7FFF == flag) /* CSR 00048098 Fix */
	{
		return OAM_SUCCESS;
	}
	else
	{
		OAM_LOG(OAM,OAM_ERROR,"Missing Ip and/or Port required by OAM in Config file");
		return OAM_FAILURE;
	}
#endif

}

#ifdef OAM_SON_ENABLE
/****************************************************************************
 * Function Name  : oam_get_son_inter_freq_carrier_idx
 * Inputs         : cell_index  : cid
 * Outputs        : None
 * Returns        : OAM_ERROR_RETURN_VAL(-1) / Array Index of the configured cell
 * Description    : This function returns the array index of the configured cell
 *		    taking input the cell index .
 ****************************************************************************/
	SInt8
oam_get_son_inter_freq_carrier_idx(UInt8 cid)
{
	UInt8 cell_count = OAM_ZERO;
	UInt8 tmp_cid = OAM_ZERO;
	SInt8 retVal = OAM_ERROR_RETURN_VAL;

	while(cell_count < oam_prov_req.num_cells_configured) {
		tmp_cid = SON_INTER_FREQ_CARR_PARAMS(cell_count).cid;

		if(tmp_cid == cid) {
			retVal = cell_count;
			break;
		}
		else {
			++cell_count;
		}
	}

	return retVal;
}


/* SPR 13024 FIX START */
/****************************************************************************
 * Function Name  : oam_get_earfcn_pci_select_arr_idx
 * Inputs         : cell_index  : cid
 * Outputs        : None
 * Returns        : OAM_ERROR_RETURN_VAL(-1) /Array Index of the configured cell
 * Description    : This function returns the array index of the configured
 cell taking input the cell index.
 ****************************************************************************/
	SInt8
oam_get_earfcn_pci_select_arr_idx(UInt8 cid)
{
	UInt8 cell_count = OAM_ZERO;
	UInt8 tmp_cid = OAM_ZERO;
	SInt8 retVal = OAM_ERROR_RETURN_VAL;

	while(cell_count < oam_prov_req.num_cells_configured) {
		tmp_cid = SON_PROV_REQ.earfcn_pci_selection[cell_count].cid;

		if(tmp_cid == cid) {
			retVal = cell_count;
			break;
		}
		else {
			++cell_count;
		}
	}

	return retVal;
}
/* SPR 13024 FIX END */


/****************************************************************************
 * Function Name  : oam_get_son_serving_cell_idx
 * Inputs         : cell_index  : cid
 * Outputs        : None
 * Returns        : OAM_ERROR_RETURN_VAL(-1) / Array Index of the configured cell
 * Description    : This function returns the array index of the configured cell
 *                  taking input the cell index .
 ****************************************************************************/
	SInt8
oam_get_son_serving_cell_idx(UInt8 cid)
{
	UInt8 cell_count = OAM_ZERO;
	UInt8 tmp_cid = OAM_ZERO;
	SInt8 retVal = OAM_ERROR_RETURN_VAL;
	UInt8 *p_cgi = OAM_NULL;

	while(cell_count < oam_prov_req.num_cells_configured) {
		p_cgi =
			SON_CELL_INFO_IND_SERVING_CELL(cell_count).src_cgi.cell_identity;

		tmp_cid = oam_extract_cid_from_eutran_cellidentity(p_cgi);

		if(tmp_cid == cid) {
			retVal = cell_count;
			break;
		}
		else {
			++cell_count;
		}
	}

	return retVal;
}

/****************************************************************************
 * Function Name  : oam_get_son_cell_nr_list_idx
 * Inputs         : cid
 * Outputs        : None
 * Returns        : OAM_ERROR_RETURN_VAL(-1) OR array index value
 * Description    : This function returns the array index of the array
 *                  oam_son_nr_add_req
 ****************************************************************************/
	SInt8
oam_get_son_cell_nr_list_idx(UInt8 cid)
{
	UInt8 cell_count = OAM_ZERO;
	UInt8 tmp_cid = OAM_ZERO;
	SInt8 retVal = OAM_ERROR_RETURN_VAL;

	while(cell_count < oam_prov_req.num_cells_configured) {
		tmp_cid = SON_NEIGHBORLIST_INFO(cell_count).cid;

		if(tmp_cid == cid) {
			retVal = cell_count;
			break;
		}
		else {
			++cell_count;
		}
	}

	return retVal;
}
/* Start: SPR 10567 */
/****************************************************************************
 * Function Name  : oam_get_cell_idx_from_cell_identity
 * Inputs         : p_cgi
 * Outputs        : None
 * Returns        : OAM_ERROR_RETURN_VAL(-1) OR array index value
 * Description    : This function returns the array index 
 *****************************************************************************/
SInt8
oam_get_cell_idx_from_cell_identity
(
 UInt8 *p_cgi
 )
{
	UInt8 src_cid = OAM_ZERO,
	      resp_cid = OAM_ZERO,
	      *p_src_cgi = OAM_NULL,
	      i = OAM_ZERO;
	SInt8 retVal = OAM_ERROR_RETURN_VAL;
	resp_cid = oam_extract_cid_from_eutran_cellidentity(p_cgi);
	for (i = 0 ; i < oam_prov_req.num_cells_configured; i++)
	{
		p_src_cgi = SON_CELL_INFO_IND_SERVING_CELL(i).src_cgi.cell_identity;
		src_cid = oam_extract_cid_from_eutran_cellidentity(p_src_cgi);
		if (src_cid == resp_cid)
		{
			retVal = i;
			break;
		}
	}
	return retVal;
}
/*End: SPR 10567 */
/****************************************************************************
 * Function Name  : oam_get_son_neigh_list_in_use_idx
 * Inputs         : cid
 * Outputs        : None
 * Returns        : OAM_ERROR_RETURN_VAL(-1) OR array index value
 * Description    : This function returns the array index 
 *       ****************************************************************************/
	SInt8
oam_get_son_neigh_list_in_use_idx(UInt8 cid)
{
	UInt8 cell_count = OAM_ZERO;
	UInt8 tmp_cid = OAM_ZERO;
	SInt8 retVal = OAM_ERROR_RETURN_VAL;
	UInt8 *p_cgi = OAM_NULL;

	while(cell_count < oam_prov_req.num_cells_configured) {
		/* Start: SPR 10567 */
		p_cgi = SON_CELL_INFO_IND_SERVING_CELL(cell_count).src_cgi.cell_identity;
		/*End: SPR 10567 */
		tmp_cid = oam_extract_cid_from_eutran_cellidentity(p_cgi);
		if(tmp_cid == cid) {
			retVal = cell_count;
			break;
		}
		else {
			++cell_count;
		}
	}

	return retVal;
}

#endif

/* SPR 21412 Fix Start */
/* SPR 21738 Fix Start */
/* Code Deleted */
/* SPR 21738 Fix End */
/****************************************************************************
 * Function Name  : oam_get_min_rrm_cell_config_idx
 * Inputs         : cell_action
 * Outputs        : None
 * Returns        : OAM_ERROR_RETURN_VAL(-1) OR array index value
 * Description    : This function returns the array index of the min value of array
 *                  oam_rrm_cell_config_req
 ****************************************************************************/
	SInt8
oam_get_min_rrm_cell_config_idx(UInt8 cell_action)
{

	UInt8 cell_count = OAM_ZERO;
	UInt8 tmp_cid = OAM_ZERO;
	SInt8 retVal = OAM_ERROR_RETURN_VAL;
	UInt8 min_cid = OAM_ZERO;
    oam_cell_status_t *p_cell_status = OAM_NULL;
    UInt8 state_check = OAM_ZERO;

	while(cell_count < oam_prov_req.num_cells_configured) 
	{
		tmp_cid = oam_extract_cid_from_eutran_cellidentity(
				OWN_CELL_CGI(cell_count).cell_identity);


		p_cell_status = oam_get_cell_cntxt_status(tmp_cid);
		if(!p_cell_status) {
			OAM_LOG(OAM, OAM_ERROR, "No cell found for cell context = %d", tmp_cid);
			retVal = OAM_FAILURE;
			break;
		}

        if(cell_action == CELL_DELETE)
        {
            state_check = ((p_cell_status->cell_state & OAM_CELL_ACTIVE ) || (p_cell_status->cell_state & OAM_CELL_STOP));
        }
        else if(cell_action == CELL_CONFIG)
        {
            state_check = (p_cell_status->cell_state & OAM_CELL_IDLE);
        }
        else if (cell_action == CELL_START)
        {
            state_check = ((p_cell_status->cell_state & OAM_CELL_STOP) || (p_cell_status->cell_state & OAM_CELL_IDLE));
        }
        else if (cell_action == CELL_STOP)
        {
            state_check = (p_cell_status->cell_state & OAM_CELL_ACTIVE);
        }
        else
        {
            OAM_LOG(OAM, OAM_ERROR, "Invalid State [%d]",cell_action);
        }

        if(state_check)
		{
			if (( OAM_ERROR_RETURN_VAL == retVal) ||
                   (tmp_cid <= min_cid))
            {
			   min_cid = tmp_cid;
			   retVal = cell_count;
			}
		}
			cell_count++;
	}

	return retVal;
}
/* SPR 21738 Fix Start */
/* Code Deleted */
/* SPR 21738 Fix End */
/* SPR 21412 Fix End */

/****************************************************************************
 * Function Name  : oam_get_rrm_cell_config_idx
 * Inputs         : cid
 * Outputs        : None
 * Returns        : OAM_ERROR_RETURN_VAL(-1) OR array index value
 * Description    : This function returns the array index of the array
 *                  oam_rrm_cell_config_req
 ****************************************************************************/
	SInt8
oam_get_rrm_cell_config_idx(UInt8 cid)
{
	UInt8 cell_count = OAM_ZERO;
	UInt8 tmp_cid = OAM_ZERO;
	SInt8 retVal = OAM_ERROR_RETURN_VAL;

	while(cell_count < oam_prov_req.num_cells_configured) {
		tmp_cid = oam_extract_cid_from_eutran_cellidentity(
				OWN_CELL_CGI(cell_count).cell_identity);

		if(tmp_cid == cid) {
			retVal = cell_count;
			break;
		}
		else {
			++cell_count;
		}
	}

	return retVal;
}

/******************************************************************************
 *   FUNCTION NAME   : oam_user_intf_process_timer
 *   DESCRIPTION     : It is a timer expiry callback function
 *   INPUT           : p_timer_buf :Pointer to timer buffer
 *                     p_context :Pointer to context
 *   OUTPUT          : None                    
 *   RETURNS         : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/

oam_return_et oam_user_intf_process_timer
(
 oam_timer_t timer_id,
 void        *p_timer_buf,
 void        *p_context
 )
{      
	/*CID:54892 start*/
	oam_timer_data_t *p_data = (oam_timer_data_t*)p_timer_buf;
	oam_error_code_et error_code = NO_ERROR;
	oam_return_et retVal = OAM_SUCCESS;
	/* SPR 14082 START */
	Char8 *p_al_add_info = OAM_NULL;
	Char8 *p_al_add_txt = OAM_NULL;
	/* SPR 14082 END */

	OAM_FUN_ENTRY_TRACE();

    OAM_LOG(OAM,OAM_BRIEF,"Timer expired: Timer [%p] p_context [%p]"
            " p_timer_buf [%p]", 
            timer_id, p_context, p_timer_buf);

	if(((OAM_GUARD_TIMER_TYPE == p_data->timer_type) &&
				(OAM_SUB_MODULE_CM == p_data->module_id)) ||
			((OAM_TR069_INIT_TIMER_TYPE == p_data->timer_type) &&
			 (OAM_SUB_MODULE_MIH == p_data->module_id)))
	{
		/* SPR 13832 START */
		if (OAM_TRUE == oam_prov_req.oam_tr069_req.tr069_mode && oam_atoi((Char8 *)p_data->buf) == TR069_MODULE_ID) 
			/* SPR 13832 END */
		{
			/* SPR 14082 START */
			OAM_LOG(OAM,OAM_ERROR,
					"Setting TR069 mode to False and Aborting ");
			/* Since TR069 agent is not responding, StartUp can't continue */   
			oam_prov_req.oam_tr069_req.tr069_mode = OAM_FALSE;
			p_al_add_info = "TR069 Layer initialization failed";

			oam_handle_init_config_failure(
					CRITICAL_CONFIGURATION_FAILURE_ALARM_ID,
					p_al_add_txt,p_al_add_info);

			/* SPR 14082 END */
		}
		/* SPR 13832 START */ 
		else
		{
			OAM_LOG(OAM,OAM_WARNING,
					"INIT_CONFIG_RESP not recieved from module_id [%d]",oam_atoi((Char8 *)p_data->buf));
			p_cm_cntxt->event = OAM_CM_EV_TIMER_EXPIRY;
			oam_mem_free(p_data->buf,&error_code);
			retVal =  oam_cm_fsm_process_event(p_timer_buf, p_cm_cntxt, &error_code);
		}
		/* SPR 13832 END */ 

	}
#ifdef OAM_SON_ENABLE
	if((OAM_TR069_INIT_NMM_TIMER_TYPE == p_data->timer_type) &&
			(OAM_SUB_MODULE_MIH == p_data->module_id))
	{

		OAM_LOG(OAM,OAM_INFO,
				"Sending Message to Initiate Reboot Process");
		oam_son_execute_reboot(p_data->buf);
		g_oam_nmm_scan_first_time = OAM_FALSE;
	}
#endif          
	if((OAM_SDM_TIMER_TYPE == p_data->timer_type) &&
			(OAM_SUB_MODULE_SDM == p_data->module_id)) 
	{
		OAM_LOG(OAM,OAM_INFO,
				"Sending Message to SDM to Initiate Reboot Process");
		retVal=oam_send_sdm_start_upgrade_req(p_data->buf); /* Coverity FIX 41059 */
		if(OAM_FAILURE == retVal)
		{
			OAM_LOG(OAM,OAM_WARNING,
					"Sending Message to SDM to Initiate Reboot Process Failed");
			return retVal;
		}
	}
	oam_mem_free(p_timer_buf, &error_code);     
	OAM_FUN_EXIT_TRACE();
	return retVal;
	/*CID:54892 end*/
}
/******************************************************************************
 * *   FUNCTION NAME    : oam_fm_intf_process_timer
 * *   DESCRIPTION      : It is a callback function for timer expiry
 * *   INPUT            : p_timer_buf :Pointer to timer buffer
 * *                      p_context :Pointer to context
 * *   OUTPUT           : None                       
 * *   RETURNS          : OAM_SUCCESS/OAM_FAILURE
 * ******************************************************************************/
oam_return_et oam_fm_intf_process_timer
(
 oam_timer_t timer_id,
 void        *p_timer_buf,
 void        *p_context
 )
{
    OAM_LOG(OAM,OAM_BRIEF,"Timer expired: Timer [%p] p_context [%p]"
            " p_timer_buf [%p]", 
            timer_id, p_context, p_timer_buf);
#ifdef OAM_EVM_TARGET
	oam_ntp_handler();
#endif
	return OAM_SUCCESS;

}
/******************************************************************************
 *   FUNCTION NAME    : oam_layer_intf_process_timer
 *   DESCRIPTION      : It is a callback function for timer expiry
 *   INPUT            : p_timer_buf :Pointer to timer buffer
 *                      p_context :Pointer to context
 *   OUTPUT           : None                       
 *   RETURNS          : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/

oam_return_et oam_layer_intf_process_timer
(
 oam_timer_t timer_id,
 void        *p_timer_buf,
 void        *p_context
 )
{
	OAM_FUN_ENTRY_TRACE();
	oam_timer_data_t *p_data = (oam_timer_data_t*)p_timer_buf;
	oam_error_code_et error_code = NO_ERROR;
	oam_return_et     ret_val = OAM_SUCCESS;
	/* SPR 9620 CHANGE START */
	UInt16 trans_id_resp = OAM_ZERO;
	UInt16 dest_id = OAM_ZERO;
    UInt8  temp_count = OAM_ZERO;
    UInt8  ecu_failure_received = OAM_FALSE;
    /* SPR_20467_FIX_START */
    UInt8 cnt = OAM_ZERO;
    UInt8 mme_id = OAM_ZERO;
    UInt8 index = OAM_ZERO;
    UInt8 index1 = OAM_ZERO;
    UInt16 transid = OAM_ZERO;
    UInt8 cid = OAM_ZERO;
    UInt8 num_failure_mmeId = OAM_ZERO;
    oam_counter_t counter_1 = OAM_ZERO;
    oam_counter_t counter_2 = OAM_ZERO;
    oam_counter_t loop_counter = OAM_ZERO;
    oam_cell_status_t *p_cell_status = OAM_NULL;
    UInt8 send_s1ap_oam_close_mme_conn_req = OAM_FALSE;
    oam_rrm_cell_config_req_t *p_rrm_cell_config = OAM_NULL;
    oam_s1ap_ecu_resp_data_t *p_oam_s1ap_ecu_resp_data = OAM_NULL;
    oam_rrm_cell_reconfig_req_t  *p_cell_reconfig_req = OAM_NULL;
    oam_rrm_cell_plmnlist_data_t *p_cell_plmnlist_data = OAM_NULL;
    oam_rrm_cell_plmnlist_data_t *p_cell_local_plmnlist_data = OAM_NULL;
    /* SPR_20467_FIX_END */

    OAM_LOG(OAM,OAM_BRIEF,"Timer expired: Timer [%p] p_context [%p]", 
            timer_id, p_context);
	switch(p_data->module_id) 
	{
		case OAM_SUB_MODULE_MIH:
			/* CSR_59405_FIX_START *//* L3 does not send a response to cell traffic trace APIs in case of success, so OAM core does not start a guard timer for this */ /* CSR_59405_FIX_END */
			if(OAM_LAYERS_CONFIG_TIMER_TYPE == p_data->timer_type)
			{
				OAM_LOG(OAM, OAM_ERROR, "Layers Config Timer Expired!");

				if(OAM_SET_ADMIN_STATE_RESP_PENDING & oam_prov_req.system_status.event_ongoing)
				{
					/* Reset Event */
					oam_prov_req.system_status.event_ongoing &= ~OAM_SET_ADMIN_STATE_RESP_PENDING;

					if(OAM_NULL == oam_prov_req.system_status.p_api_buf)
					{
						OAM_LOG(OAM,OAM_ERROR,"Buffer stored is NULL");
						/* SPR 22465 Fix Start*/
						oam_mem_free(p_data, &error_code);
						/* SPR 22465 Fix End*/
						OAM_FUN_EXIT_TRACE();
						return OAM_FAILURE;
					}

					UInt16 trans_id = get_word_from_header((UInt8 *)oam_prov_req.system_status.p_api_buf);
					UInt16 dest_id = get_word_from_header((UInt8 *)oam_prov_req.system_status.p_api_buf + OAM_TWO);

					/* Send failure set response */
					OAM_LOG(OAM, OAM_WARNING, "Sending SPV failure response for Admin State");
					/* Send set parameters response from here only */
					oam_construct_n_send_set_resp_to_usr(OAM_RESULT_FAILURE,
							OAM_ERR_INTERNAL_ERROR,
							(UInt8 *)"Cell Operation Timeout",oam_strlen("Cell Operation Timeout") + OAM_ONE,
							OAM_VERSION_ID, OAM_MODULE_ID,
							dest_id, trans_id, OAM_ZERO, OAM_ZERO,
							&error_code);
				}

				if(OAM_FILE_DOWNLOAD_RESP_PENDING & oam_prov_req.system_status.event_ongoing)
				{
					/* Reset Event */
					oam_prov_req.system_status.event_ongoing &= ~OAM_FILE_DOWNLOAD_RESP_PENDING;
					OAM_LOG(OAM, OAM_WARNING, "Sending Failure File Download Response");
					ret_val = oam_send_file_download_ind_resp(OAM_RESULT_FAILURE,
							OAM_ERR_INTERNAL_ERROR,
							oam_prov_req.system_status.trans_id,
							(UInt8 *)"Layers Configuration Timeout");
				}

				/* SPR 12225 FIX START */
				/* CODE REMOVED */
				/* SPR 12225 FIX END */
			}/* End of else if(OAM_LAYERS_CONFIG_TIMER_TYPE .... */
			else if((OAM_CELL_SETUP_TIMER_TYPE == p_data->timer_type) ||
					(OAM_CELL_BLOCK_TIMER_TYPE == p_data->timer_type))
			{
				if(OAM_CELL_SETUP_TIMER_TYPE == p_data->timer_type)
				{
					OAM_LOG(OAM, OAM_INFO, "Cell Setup Timer Expired!");
				}
				else if(OAM_CELL_BLOCK_TIMER_TYPE == p_data->timer_type)
				{
					OAM_LOG(OAM, OAM_INFO, "Cell Block Timer Expired!");
				}

				/* Reset Event */
				if(OAM_RRM_CELL_BLOCK_REQ_MASK & oam_prov_req.system_status.event_ongoing) 
					oam_prov_req.system_status.event_ongoing &= ~OAM_RRM_CELL_BLOCK_REQ_MASK;

				if(OAM_SET_ADMIN_STATE_RESP_PENDING & oam_prov_req.system_status.event_ongoing) 
				{
					/* Reset Event */
					oam_prov_req.system_status.event_ongoing &= ~OAM_SET_ADMIN_STATE_RESP_PENDING;


					if(OAM_NULL == oam_prov_req.system_status.p_api_buf)
					{
						OAM_LOG(OAM,OAM_WARNING,"Buffer stored is NULL");
						/* SPR 22465 Fix Start*/
						oam_mem_free(p_data, &error_code);
						/* SPR 22465 Fix End*/
						OAM_FUN_EXIT_TRACE(); 
						return OAM_FAILURE;     
					}

					UInt16 trans_id = get_word_from_header((UInt8 *)oam_prov_req.system_status.p_api_buf);
					UInt16 dest_id = get_word_from_header((UInt8 *)oam_prov_req.system_status.p_api_buf + OAM_TWO);

					/* Send success set response */
					OAM_LOG(OAM, OAM_WARNING, "Sending SPV failure response for Admin State");
					/* Send set parameters response from here only */
					oam_construct_n_send_set_resp_to_usr(OAM_RESULT_FAILURE,
							OAM_ERR_INTERNAL_ERROR,
							(UInt8 *)"Cell Operation Timeout",oam_strlen("Cell Operation Timeout") + OAM_ONE,
							OAM_VERSION_ID, OAM_MODULE_ID,
							dest_id, trans_id, OAM_ZERO, OAM_ZERO,
							&error_code);
				}
				oam_mem_free(p_data, &error_code);
				return ret_val;
			}/* End of else if(OAM_CELL_SETUP_TIMER_TYPE .... */
			else if (OAM_SYSTEM_LOG_TIMER_TYPE == p_data->timer_type)
			{

				if(OAM_SET_LOG_LEVEL_REQ & oam_prov_req.system_status.event_ongoing)
				{
					oam_prov_req.system_status.event_ongoing &= ~OAM_SET_LOG_LEVEL_REQ;
					if(OAM_NULL == oam_prov_req.system_status.p_api_buf)
					{
						OAM_LOG(OAM,OAM_WARNING,"Buffer stored is NULL");
						/* SPR 22465 Fix Start*/
						oam_mem_free(p_data, &error_code);
						/* SPR 22465 Fix End*/
						OAM_FUN_EXIT_TRACE();
						return OAM_FAILURE;
					}

					UInt16 trans_id = get_word_from_header((UInt8 *)oam_prov_req.system_status.p_api_buf);
					UInt16 dest_id = get_word_from_header((UInt8 *)oam_prov_req.system_status.p_api_buf + OAM_TWO);
					if(OAM_FAILURE == oam_process_tcb_for_layer_resp((UInt8 *)oam_prov_req.system_status.p_api_buf,&trans_id,&dest_id))
					{
						OAM_LOG(OAM,OAM_WARNING, "Deletion from TCB failed");
					} 

					OAM_LOG(OAM, OAM_INFO, "System log level Timer Expired!");
					/* Send set parameters response from here only */
					oam_construct_n_send_set_resp_to_usr(OAM_RESULT_FAILURE,
							OAM_ERR_INTERNAL_ERROR,
							(UInt8 *)"system log command alarm timeout",oam_strlen("system log command alarm timeout") + OAM_ONE,
							OAM_VERSION_ID, OAM_MODULE_ID,
							dest_id, trans_id, OAM_ZERO, OAM_ZERO,
							&error_code);
				}


			}/* End of else of OAM_SYSTEM_LOG_TIMER_TYPE*/
            /* SPR 16567 START */
            else if (OAM_ENB_CONFIG_UPDATE_TIMER == p_data->timer_type)
            {
                OAM_LOG(OAM, OAM_INFO, "Timer for oam enb config update expired %d time", current_enb_config_count);
                /* SPR_20467_FIX_START */
                if(oam_prov_req.system_status.event_ongoing & 
                         OAM_CELL_PLMNLIST_CONFIG_ONGOING) 
                {
                    if (current_enb_config_count <= MAX_RETRY_COUNT )
                    {
                        p_oam_s1ap_ecu_resp_data = oam_process_tcb_for_ecu_resp(p_oam_s1ap_ecu_resp_data,(UInt8 *)p_data->buf,p_data->trans_id);

                        if(OAM_NULL == p_oam_s1ap_ecu_resp_data)
                        {
                            OAM_LOG(OAM, OAM_WARNING, "p_oam_s1ap_ecu_resp_data is NULL");
                            /* SPR 22465 Fix Start*/
			    oam_mem_free(p_data, &error_code);
                            /* SPR 22465 Fix End*/
                            return OAM_FAILURE;
                        }    

                        UInt16 transaction_id = oam_get_new_trans_id();

                        if(OAM_FALSE == p_oam_s1ap_ecu_resp_data->old_ecu_sent_flag)
                        {    
                            local_prov_req.oam_l3_req.oam_s1ap_req.enb_config_update.bitmask = OAM_ZERO;
                            local_prov_req.oam_l3_req.oam_s1ap_req.enb_config_update.bitmask |= S1AP_OAM_ENB_CONFIG_UPDATE_TA_LIST_PRESENT;

                            oam_memset(&local_prov_req.oam_l3_req.oam_s1ap_req.enb_config_update.s1ap_oam_mme_id_list,
                                    OAM_ZERO,
                                    sizeof(local_prov_req.oam_l3_req.oam_s1ap_req.enb_config_update.s1ap_oam_mme_id_list));
                            local_prov_req.oam_l3_req.oam_s1ap_req.enb_config_update.bitmask = OAM_ZERO;

                            local_prov_req.oam_l3_req.oam_s1ap_req.enb_config_update.bitmask |= 
                                S1AP_OAM_ENB_CONFIG_UPDATE_TA_LIST_PRESENT;
                            local_prov_req.oam_l3_req.oam_s1ap_req.enb_config_update.bitmask |= 
                                S1AP_OAM_ENB_CONFIG_UPDATE_MME_LIST_PRESENT;

                            for(index=0; index<oam_prov_req.oam_l3_req.mme_data.num_mme_id; index++)
                            {
                                mme_id = oam_prov_req.oam_l3_req.mme_data.mme_id[index];
                                if( (OAM_FALSE == p_oam_s1ap_ecu_resp_data->old_ecu_sent_flag) &&
                                        ((OAM_RESP_NOT_RECEIVED == p_oam_s1ap_ecu_resp_data->ecu_resp_received[mme_id]) ||
                                         (OAM_ECU_FAILURE_TIMER_EXPIRY == p_oam_s1ap_ecu_resp_data->ecu_resp_received[mme_id])) )
                                {
                                    local_prov_req.oam_l3_req.oam_s1ap_req.enb_config_update.s1ap_oam_mme_id_list.mme_id[index1]= mme_id;
                                    index1++;
                                }   
                            }

                            local_prov_req.oam_l3_req.oam_s1ap_req.enb_config_update.s1ap_oam_mme_id_list.num_mme_id=index1;
                            if(OAM_SUCCESS == oam_s1ap_il_send_s1ap_oam_enb_config_update(
                                        &local_prov_req.oam_l3_req.oam_s1ap_req.enb_config_update,
                                        OAM_MODULE_ID, RRC_MODULE_ID, transaction_id,
                                        oam_prov_req.cell_cntxt.curr_cell_cid))
                            {
                                if(OAM_NULL == oam_maintain_tcb_for_ecu(transaction_id, OAM_TRANS_X2AP,
                                            p_data->buf_size,p_data->buf,
                                            sizeof(oam_s1ap_ecu_resp_data_t),
                                            (void *)p_oam_s1ap_ecu_resp_data,
                                            OAM_FALSE))
                                {
                                    OAM_LOG(OAM, OAM_WARNING, "p_data->buf is NULL");
                            	     /* SPR 22465 Fix Start*/
			            oam_mem_free(p_data, &error_code);
                                     /* SPR 22465 Fix End*/
                                    return OAM_FAILURE;
                                }    

                                oam_prov_req.system_status.timer_id = oam_start_guard_timer(transaction_id,OAM_SUB_MODULE_MIH,
                                        OAM_MODULE_ID,
                                        OAM_ENB_CONFIG_UPDATE_TIMER,
                                        OAM_USER_INTF_RESP_TIMEOUT_S * OAM_THOUSAND,
                                        (UInt8 *)p_data->buf,
                                        p_data->buf_size,
                                        OAM_FALSE);
                                {
                                    OAM_LOG(OAM,OAM_DETAILED,"Layers Config Timer started successfully, TIMER Id %p",
                                            oam_prov_req.system_status.timer_id);
                                }
                                oam_tcb_delete(&(p_data->trans_id));
                                current_enb_config_count++;
                            }
                            else
                            {
                                OAM_LOG(OAM,OAM_WARNING,"Failed to send enb config update ");
                            }    
                        }
                        else if(OAM_TRUE == p_oam_s1ap_ecu_resp_data->old_ecu_sent_flag)
                        {
                            oam_prov_req.oam_l3_req.oam_s1ap_req.enb_config_update.bitmask = OAM_ZERO;
                            oam_prov_req.oam_l3_req.oam_s1ap_req.enb_config_update.bitmask |= S1AP_OAM_ENB_CONFIG_UPDATE_TA_LIST_PRESENT;

                            oam_memset(&oam_prov_req.oam_l3_req.oam_s1ap_req.enb_config_update.s1ap_oam_mme_id_list,
                                    OAM_ZERO,
                                    sizeof(oam_prov_req.oam_l3_req.oam_s1ap_req.enb_config_update.s1ap_oam_mme_id_list));
                            oam_prov_req.oam_l3_req.oam_s1ap_req.enb_config_update.bitmask = OAM_ZERO;

                            oam_prov_req.oam_l3_req.oam_s1ap_req.enb_config_update.bitmask |= 
                                S1AP_OAM_ENB_CONFIG_UPDATE_TA_LIST_PRESENT;
                            oam_prov_req.oam_l3_req.oam_s1ap_req.enb_config_update.bitmask |= 
                                S1AP_OAM_ENB_CONFIG_UPDATE_MME_LIST_PRESENT;

                            for(index=0; index<oam_prov_req.oam_l3_req.mme_data.num_mme_id; index++)
                            {
                                mme_id = oam_prov_req.oam_l3_req.mme_data.mme_id[loop_counter];
                                oam_prov_req.oam_l3_req.oam_s1ap_req.enb_config_update.s1ap_oam_mme_id_list.mme_id[mme_id] =  
                                    oam_prov_req.oam_l3_req.mme_data.mme_id[mme_id];
                            }

                            oam_prov_req.oam_l3_req.oam_s1ap_req.enb_config_update.s1ap_oam_mme_id_list.num_mme_id=mme_id;

                            if(OAM_SUCCESS == oam_s1ap_il_send_s1ap_oam_enb_config_update(
                                        &oam_prov_req.oam_l3_req.oam_s1ap_req.enb_config_update,
                                        OAM_MODULE_ID, RRC_MODULE_ID, transaction_id,
                                        oam_prov_req.cell_cntxt.curr_cell_cid))
                            {
                                if(OAM_NULL == oam_maintain_tcb_for_ecu(transaction_id, OAM_TRANS_X2AP,
                                            p_data->buf_size,p_data->buf,
                                            sizeof(oam_s1ap_ecu_resp_data_t),
                                            (void *)p_oam_s1ap_ecu_resp_data,
                                            OAM_FALSE))
                                {
                                    OAM_LOG(OAM,OAM_WARNING, "p_data->buf is NULL");
                            	    /* SPR 22465 Fix Start*/
			    	    oam_mem_free(p_data, &error_code);
                                    /* SPR 22465 Fix End*/
                                    return OAM_FAILURE;
                                }    

                                oam_prov_req.system_status.timer_id = oam_start_guard_timer(transaction_id,OAM_SUB_MODULE_MIH,
                                        OAM_MODULE_ID,
                                        OAM_ENB_CONFIG_UPDATE_TIMER,
                                        OAM_USER_INTF_RESP_TIMEOUT_S * OAM_THOUSAND,
                                        (UInt8 *)p_data->buf,
                                        p_data->buf_size,
                                        OAM_FALSE);
                                {
                                    OAM_LOG(OAM,OAM_DETAILED,"Layers Config Timer started successfully, TIMER Id %p",
                                            oam_prov_req.system_status.timer_id);
                                }
                                oam_tcb_delete(&(p_data->trans_id));
                                current_enb_config_count++;
                            }
                            else
                            {
                                OAM_LOG(OAM,OAM_WARNING,"Failed to send enb config update ");
                            }    
                        }
                    }
                    else
                    {
                        OAM_LOG(OAM, OAM_INFO, "maximum retry count reached for enb config update, mme count %d, p_data->buf %p, trans_id %d",
                                oam_prov_req.oam_l3_req.mme_data.num_mme_id,p_data->buf,p_data->trans_id);
                        current_enb_config_count = OAM_ZERO;

                        p_oam_s1ap_ecu_resp_data = oam_process_tcb_for_ecu_resp(p_oam_s1ap_ecu_resp_data,(UInt8 *)p_data->buf,p_data->trans_id);

                        if(OAM_NULL == p_oam_s1ap_ecu_resp_data)
                        {
                            OAM_LOG(OAM, OAM_WARNING, "p_oam_s1ap_ecu_resp_data is NULL");
                            /* SPR 22465 Fix Start*/
			    oam_mem_free(p_data, &error_code);
                            /* SPR 22465 Fix End*/
                            return OAM_FAILURE;
                        }    
                        p_oam_s1ap_ecu_resp_data->mme_id_response_cnt++;

                        oam_s1ap_close_mme_conn_t s1ap_close_mme_conn={OAM_ZERO};
                        transid = get_word_from_header(p_data->buf);

                        for(index=0; index<oam_prov_req.oam_l3_req.mme_data.num_mme_id; index++)
                        {
                            mme_id =  oam_prov_req.oam_l3_req.mme_data.mme_id[loop_counter];
                            if( ( (OAM_TRUE == p_oam_s1ap_ecu_resp_data->old_ecu_sent_flag) &&
                                  ((OAM_RESP_NOT_RECEIVED == p_oam_s1ap_ecu_resp_data->ecu_resp_received[mme_id]) ||
                                   (OAM_ECU_FAILURE_TIMER_EXPIRY == p_oam_s1ap_ecu_resp_data->ecu_resp_received[mme_id]) ||
                                   (OAM_ECU_FAILURE_RECEIVED == p_oam_s1ap_ecu_resp_data->ecu_resp_received[mme_id])) ) ||
                                ( (OAM_FALSE == p_oam_s1ap_ecu_resp_data->old_ecu_sent_flag) &&
                                  (OAM_ZERO < p_oam_s1ap_ecu_resp_data->ecu_success_resp_received) && 
                                  ((OAM_RESP_NOT_RECEIVED == p_oam_s1ap_ecu_resp_data->ecu_resp_received[mme_id]) ||
                                   (OAM_ECU_FAILURE_TIMER_EXPIRY == p_oam_s1ap_ecu_resp_data->ecu_resp_received[mme_id]) ||
                                   (OAM_ECU_FAILURE_RECEIVED == p_oam_s1ap_ecu_resp_data->ecu_resp_received[mme_id])) ) )
                            {
                                p_oam_s1ap_ecu_resp_data->ecu_resp_received[mme_id] = OAM_TRUE;
                                s1ap_close_mme_conn.mme_id[s1ap_close_mme_conn.mme_count]= mme_id;
                                s1ap_close_mme_conn.mme_count++;
                                send_s1ap_oam_close_mme_conn_req = OAM_TRUE;

                                for (counter_1 = OAM_ZERO; counter_1 < oam_prov_req.oam_l3_req.mme_data.num_mme_id; counter_1++)
                                {
                                    if(oam_prov_req.oam_l3_req.mme_data.mme_id[counter_1] == mme_id)
                                        break;  
                                }
                                for (counter_2=counter_1;counter_2 < oam_prov_req.oam_l3_req.mme_data.num_mme_id - OAM_ONE;counter_2++)
                                {
                                    /* Shift all present MME ID to thier left array index */
                                    oam_prov_req.oam_l3_req.mme_data.mme_id[counter_2] 
                                        = oam_prov_req.oam_l3_req.mme_data.mme_id[counter_2 + OAM_ONE]; 
                                }              
                                if (oam_prov_req.oam_l3_req.mme_data.num_mme_id >= OAM_ONE)
                                {
                                    num_failure_mmeId++;
                                }

                                OAM_LOG(OAM,OAM_DETAILED,"S1AP num active mme =%d",oam_prov_req.oam_l3_req.mme_data.num_mme_id);

                            }    
                            else if( (OAM_ONE == oam_prov_req.oam_l3_req.mme_data.num_mme_id) &&
                                     ((OAM_RESP_NOT_RECEIVED == p_oam_s1ap_ecu_resp_data->ecu_resp_received[mme_id]) ||
                                      (OAM_ECU_FAILURE_TIMER_EXPIRY == p_oam_s1ap_ecu_resp_data->ecu_resp_received[mme_id]) ||
                                      (OAM_ECU_FAILURE_RECEIVED == p_oam_s1ap_ecu_resp_data->ecu_resp_received[mme_id])) )
                            {
                                OAM_LOG(OAM,OAM_INFO,"Received failure for ECU from L3, single MME connected");
                                ecu_failure_received = OAM_TRUE;
                            }
                            else if( (oam_prov_req.oam_l3_req.mme_data.num_mme_id > OAM_ONE) &&
                                     ((OAM_ZERO == p_oam_s1ap_ecu_resp_data->ecu_success_resp_received) && 
                                      ((OAM_RESP_NOT_RECEIVED == p_oam_s1ap_ecu_resp_data->ecu_resp_received[mme_id]) ||
                                       (OAM_ECU_FAILURE_TIMER_EXPIRY == p_oam_s1ap_ecu_resp_data->ecu_resp_received[mme_id]) ||
                                       (OAM_ECU_FAILURE_RECEIVED == p_oam_s1ap_ecu_resp_data->ecu_resp_received[mme_id]))) )  
                             {
                                 temp_count++;
                             }   
                        }    
                        if(OAM_TRUE == send_s1ap_oam_close_mme_conn_req)
                        {   
                            if(OAM_FAILURE == oam_s1ap_il_send_s1ap_oam_close_mme_conn(
                                        &s1ap_close_mme_conn,
                                        OAM_MODULE_ID,
                                        RRC_MODULE_ID,
                                        p_data->trans_id,
                                        oam_prov_req.cell_cntxt.curr_cell_cid))
                            {
                                OAM_LOG(OAM,OAM_WARNING,"Error in function oam_s1ap_il_send_s1ap_oam_close_mme_conn");
                                OAM_FUN_EXIT_TRACE ();
                            }
                            else
                            {
                                OAM_LOG(OAM,OAM_DETAILED,"Succesfully send oam_s1ap_il_send_s1ap_oam_close_mme_conn");
                            }

                            if(num_failure_mmeId)
                            {
                                oam_prov_req.oam_l3_req.mme_data.num_mme_id -= num_failure_mmeId;
                            }    
                            if (oam_prov_req.oam_l3_req.mme_data.num_mme_id < OAM_ONE)
                            {
                                for(loop_counter = OAM_ZERO;
                                        loop_counter < oam_prov_req.num_cells_configured;
                                        ++loop_counter) {
                                    p_rrm_cell_config =
                                        &RRM_CELL_CONFIG(loop_counter);
                                    cid =
                                        oam_extract_cid_from_eutran_cellidentity(
                                                p_rrm_cell_config->global_cell_info.
                                                eutran_global_cell_id.cell_identity);
                                    p_cell_status = oam_get_cell_cntxt_status(cid);

                                    if(p_cell_status == OAM_NULL) /*Cov CID 62498 Fix*/
                                        continue;

                                    if ((p_cell_status->cell_state & OAM_CELL_ACTIVE))
                                    { 
                                        OAM_LOG(OAM, OAM_INFO,
                                                "Sending cell stop for CID: %d to rrm as no s1 link is active",
                                                cid);
                                        if(OAM_FAILURE ==
                                                oam_rrm_send_rrm_oam_cell_stop_req(
                                                    ((rrm_oam_cell_stop_req_t *)
                                                     &(p_rrm_cell_config->
                                                         global_cell_info.
                                                         eutran_global_cell_id)),
                                                    OAM_MODULE_ID,
                                                    RRM_MODULE_ID,
                                                    transid, cid))
                                        {
                                            OAM_LOG(OAM,OAM_WARNING,
                                                    "Error in function oam_send_cell_stop_req");						
                            		    /* SPR 22465 Fix Start*/
			    		    oam_mem_free(p_data, &error_code);
                            		    /* SPR 22465 Fix End*/
                                            OAM_FUN_EXIT_TRACE ();
                                            return OAM_FAILURE;
                                        } 
                                        else{
                                            OAM_LOG(OAM, OAM_DETAILED, "RRM_OAM_CELL_STOP_REQ Successfully sent");
#ifndef OAM_UT_FRAMEWORK
                                            if( OAM_FAILURE == oam_maintain_tcb_with_timer(transid,OAM_SUB_MODULE_RRM,
                                                        OAM_TRANS_RRM,p_data->buf_size,p_data->buf))
                                            {
                                                OAM_LOG(OAM, OAM_WARNING, " Failed to maintain TCB ");
                            			/* SPR 22465 Fix Start*/
			    			oam_mem_free(p_data, &error_code);
                            			/* SPR 22465 Fix End*/
                                                return OAM_FAILURE;
                                            }
#endif
                                        }
                                    }
                                    else 
                                    {
                                        OAM_LOG(OAM,OAM_WARNING,"Cell Stop for cell identity %d, "
                                                "Cannot be triggerd as CELL Stopped By USER",cid);
                                    }
                                }
                            }
                        }
                        if(temp_count == oam_prov_req.oam_l3_req.mme_data.num_mme_id)
                        {
                            /* OAM received failure from all the MMEs , OAM send failure to UI */
                            ecu_failure_received = OAM_TRUE;
                        }
                        if(ecu_failure_received)  
                        {    
                            dest_id = get_word_from_header((p_data->buf) +
                                    OAM_INTF_HDR_SRC_ID_OFFSET);

                            oam_construct_n_send_set_resp_to_usr(OAM_RESULT_FAILURE,
                                    OAM_ERR_INTERNAL_ERROR,
                                    (UInt8 *) "s1ap_oam_ECU_response failure",
                                    oam_strlen("s1ap_oam_ECU_response failure")+1,
                                    OAM_VERSION_ID, OAM_MODULE_ID,
                                    dest_id, transid, OAM_ZERO, OAM_ZERO,
                                    &error_code);
                        }
                        if( (OAM_FALSE == p_oam_s1ap_ecu_resp_data->old_ecu_sent_flag) &&
                            (OAM_ZERO < p_oam_s1ap_ecu_resp_data->ecu_success_resp_received) )
                        {
                            dest_id = get_word_from_header((p_data->buf) +
                                    OAM_INTF_HDR_SRC_ID_OFFSET);
                            SInt8 cell_config_idx = oam_get_rrm_cell_config_idx(
                                     oam_prov_req.cell_cntxt.curr_cell_cid);

                            if(OAM_ERROR_RETURN_VAL == cell_config_idx) {
                                OAM_LOG(OAM, OAM_WARNING, "Invalid cell context[cid=%d]",
                                       oam_prov_req.cell_cntxt.curr_cell_cid);
                            	/* SPR 22465 Fix Start*/
			    	oam_mem_free(p_data, &error_code);
                            	/* SPR 22465 Fix End*/
                                return OAM_FAILURE;
                            }

						    p_cell_local_plmnlist_data =
							           &LOCAL_RRM_PROV_REQ.plmnlist_data[cell_config_idx];
		                    p_cell_plmnlist_data =
			                           &RRM_PROV_REQ.plmnlist_data[cell_config_idx];
                            if(OAM_FAILURE ==
                                oam_mem_alloc(sizeof(oam_rrm_cell_reconfig_req_t),
                                        (void**)&p_cell_reconfig_req, &error_code)) {
                                OAM_LOG(OAM, OAM_ERROR, "Memory allocation failed "
                                        "with Error Code: %d", error_code);
                            	/* SPR 22465 Fix Start*/
			    	oam_mem_free(p_data, &error_code);
                            	/* SPR 22465 Fix End*/
                                return OAM_FAILURE; 
                            }

                            oam_copy_config_to_reconfig((oam_rrm_cell_config_req_t *)
                                    (&(LOCAL_RRM_PROV_REQ.oam_rrm_cell_config_req[cell_config_idx])),p_cell_reconfig_req);

                            for(cnt = 0; cnt < RRM_OAM_MAX_NUM_PLMNS; ++cnt) {

                                if(p_cell_local_plmnlist_data->
                                        plmn_entries[cnt].is_primary) {
                                    break;
                                }
                            }
                            if(cnt == RRM_OAM_MAX_NUM_PLMNS) 
                            {
                                OAM_LOG(OAM, OAM_WARNING,
                                        "No primary PLMNID found, request will "
                                        "not be sent to RRM!!");
                                p_cell_plmnlist_data->plmnlist_cache_idx =
                                    p_cell_local_plmnlist_data->plmnlist_query_idx + 1;
                                oam_memcpy(&p_cell_plmnlist_data->plmnlist_cache,
                                        &p_cell_local_plmnlist_data->
                                        plmn_entries[p_cell_local_plmnlist_data->plmnlist_query_idx],
                                        sizeof(oam_rrm_plmn_data_t));

                                oam_construct_n_send_set_resp_to_usr(
                                        OAM_RESULT_SUCCESS,
                                        OAM_ZERO, OAM_NULL,
                                        OAM_ZERO,
                                        OAM_VERSION_ID,
                                        OAM_MODULE_ID,
                                        dest_id,
                                        transid,
                                        OAM_ZERO, OAM_ZERO,
                                        &error_code);

                                /* Coverity 113377 Fix Start*/
                                oam_mem_free(p_cell_reconfig_req, &error_code);
                                /* Coverity 113377 Fix End*/
                                break;
                            }

                            if(OAM_SUCCESS != oam_rrm_send_rrm_oam_cell_reconfig_req(
                                        p_cell_reconfig_req,
                                        OAM_MODULE_ID,
                                        RRM_MODULE_ID,
                                        transid,
                                        oam_prov_req.cell_cntxt.curr_cell_cid))
                            {
                                OAM_LOG(OAM, OAM_ERROR,
                                        "Failed to send RRM_OAM_CELL_RECONFIG_REQ message to RRM");
                            }
                            else {
                                OAM_LOG(OAM, OAM_DETAILED,
                                        "Successfully send RRM_OAM_CELL_RECONFIG_REQ message to RRM");
                            }
                            oam_mem_free(p_cell_reconfig_req, &error_code);
                        }    
                        oam_tcb_delete(&(p_data->trans_id));

                        if(OAM_FAILURE == oam_maintain_tcb(transid, OAM_TRANS_SON,
                                   p_data->buf_size, p_data->buf))
                        { 
                            OAM_LOG(OAM, OAM_WARNING, "Failed to maintain TCB ");
                            break;
                        }
                        else
                        {
                             oam_prov_req.system_status.event_ongoing |= OAM_CELL_PLMNLIST_RRM_RECONFIG_ONGOING;
                        }    
                        /* SPR_20467_FIX_END */
                    }
                }
                else
                {    
                    /* SPR_20467_FIX_END */
                    if (current_enb_config_count <= MAX_RETRY_COUNT )
                    {
                        OAM_LOG(OAM, OAM_DETAILED, "trying %d time for sending enb_config_update", current_enb_config_count);
                        if (oam_send_enb_config_update() == OAM_SUCCESS)
                        {
                            if(OAM_NULL != (oam_prov_req.system_status.timer_id =
                                        oam_start_new_timer(OAM_SUB_MODULE_MIH,
                                            OAM_MODULE_ID,
                                            OAM_ENB_CONFIG_UPDATE_TIMER,
                                            OAM_USER_INTF_RESP_TIMEOUT_S * OAM_THOUSAND,
                                            p_data->buf,
                                            p_data->buf_size,
                                            OAM_FALSE)))
                            {
                                OAM_LOG(OAM,OAM_DETAILED,"Layers Config Timer started successfully");
                            }
                            else
                            {
                                OAM_LOG(OAM,OAM_INFO,"Error starting Timer for enb config update ");
                            }
                        }
                        else
                        {

                        }
                    }
                    else
                    {
                        OAM_LOG(OAM, OAM_INFO, "maximum retry count reached for enb config update");
                        current_enb_config_count = OAM_ZERO;
                    }
                /* SPR_20467_FIX_START */
                }
                /* SPR_20467_FIX_END */
            }
            /* SPR 16567 END */
            /*SPR 22465 Fix Start*/
            else if(OAM_GUARD_TIMER_TYPE == p_data->timer_type)
            {			
                OAM_LOG(OAM,OAM_BRIEF, "OAM GUARD TIMER Expired with trans_id : %u module_id : %u and Timer Type : %u\n",
                        p_data->trans_id, p_data->module_id, p_data->timer_type);

                if(OAM_FAILURE == oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id(p_data->trans_id, &trans_id_resp, &dest_id))
                {
                    OAM_LOG(OAM,OAM_ERROR, "Deletion from TCB failed");
                    ret_val=OAM_FAILURE;
                }

            }
            /*SPR 22465 Fix End*/

			break;

		case OAM_SUB_MODULE_CM:
			p_cm_cntxt->event = OAM_CM_EV_TIMER_EXPIRY;
			ret_val =  oam_cm_fsm_process_event(OAM_NULL, p_cm_cntxt, &error_code);
			break;

		case OAM_SUB_MODULE_HM:
			if (OAM_HM_TIMER_TYPE == p_data->timer_type)
			{    
                /*SPR 17777 fix*/
				oam_hm_timer_expiry();
				return ret_val;
			}
			else if(OAM_HM_NON_RESPONSIVE_TIMER_TYPE == p_data->timer_type)
			{
				oam_hm_non_responsive_timer_expiry();
                /*SPR 17777 fix*/
			}
			else
			{
				OAM_LOG(OAM, OAM_WARNING, "INVALID HM TIMER TYPE[%d]", p_data->timer_type);    
			}    
			break;
			/* SPR 9620 CHANGES START */
		case OAM_SUB_MODULE_SON:
            /*SPR 22465 Fix Start*/
            OAM_LOG(OAM,OAM_DETAILED, "Timer Expired with trans_id : %u module_id : %u and Timer Type : %u\n",
                    p_data->trans_id, p_data->module_id, p_data->timer_type);
#ifndef OAM_UT_FRAMEWORK
			if(OAM_FAILURE == oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id(p_data->trans_id, &trans_id_resp, &dest_id))
			{
				OAM_LOG(OAM,OAM_DETAILED, "oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id function failed in OAM_SUB_MODULE_SON");
				ret_val=OAM_FAILURE;
			}
#endif
			break;
		case OAM_SUB_MODULE_RRC:
            OAM_LOG(OAM,OAM_DETAILED, "Timer Expired with trans_id : %u module_id : %u and Timer Type : %u\n",
                    p_data->trans_id, p_data->module_id, p_data->timer_type);
#ifndef OAM_UT_FRAMEWORK
			if(OAM_FAILURE == oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id(p_data->trans_id, &trans_id_resp, &dest_id))
			{
				OAM_LOG(OAM,OAM_DETAILED, "oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id function failed in OAM_SUB_MODULE_RRC");
				ret_val=OAM_FAILURE;
			}
#endif
			break;

		case OAM_SUB_MODULE_PDCP:
            OAM_LOG(OAM,OAM_DETAILED, "Timer Expired with trans_id : %u module_id : %u and Timer Type : %u\n",
                    p_data->trans_id, p_data->module_id, p_data->timer_type);
#ifndef OAM_UT_FRAMEWORK
			if(OAM_FAILURE == oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id(p_data->trans_id, &trans_id_resp, &dest_id))
			{
				OAM_LOG(OAM,OAM_DETAILED, "oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id function failed in OAM_SUB_MODULE_PDCP");
				ret_val=OAM_FAILURE;
			}
#endif
			break;

		case OAM_SUB_MODULE_MAC:
            OAM_LOG(OAM,OAM_DETAILED, "Timer Expired with trans_id : %u module_id : %u and Timer Type : %u\n",
                    p_data->trans_id, p_data->module_id, p_data->timer_type);
#ifndef OAM_UT_FRAMEWORK
			if(OAM_FAILURE == oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id(p_data->trans_id, &trans_id_resp, &dest_id))
			{
				OAM_LOG(OAM,OAM_DETAILED, "oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id function failed in OAM_SUB_MODULE_MAC");
				ret_val=OAM_FAILURE;
			}
#endif
			break;

		case OAM_SUB_MODULE_RRM:
            OAM_LOG(OAM,OAM_DETAILED, "Timer Expired with trans_id : %u module_id : %u and Timer Type : %u\n",
                    p_data->trans_id, p_data->module_id, p_data->timer_type);
#ifndef OAM_UT_FRAMEWORK
			if(OAM_FAILURE == oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id(p_data->trans_id, &trans_id_resp, &dest_id))
			{
				OAM_LOG(OAM,OAM_DETAILED, "oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id function failed in OAM_SUB_MODULE_RRM");
				ret_val=OAM_FAILURE;
			}
#endif
			break;
		case OAM_SUB_MODULE_RLC:
            OAM_LOG(OAM,OAM_DETAILED, "Timer Expired with trans_id : %u module_id : %u and Timer Type : %u\n",
                    p_data->trans_id, p_data->module_id, p_data->timer_type);
            /*SPR 22465 Fix End*/
#ifndef OAM_UT_FRAMEWORK
			if(OAM_FAILURE == oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id(p_data->trans_id, &trans_id_resp, &dest_id))
			{
				OAM_LOG(OAM,OAM_DETAILED, "oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id function failed in OAM_SUB_MODULE_RLC");
				ret_val=OAM_FAILURE;
			}
#endif
			break;
			/* SPR 9620 CHANGES END */
			/* SPR 21889 start */
        case OAM_SUB_MODULE_AGENT:
            {
                switch(p_data->timer_type)
                {
                    case OAM_SHUTDOWN_TIMER:
                        {
                            if (OAM_FALSE != oam_is_sdm_alive)
                            {
                                oam_construct_n_send_sdm_command_req(OAM_SDM_SHUTDOWN);
                            }
                            else
                            {
                                OAM_EXIT (1);
                            }

                            oam_reset_reason_code(OPERATOR_INITIATED_SHUTDOWN);
                        }
                        break;

                    case OAM_AGENT_RESP_PENDING_TIMER:
                        oam_trigger_agent_fsm(OAM_NULL, OAM_AGENT_EVENT_PROCEDURE_TMR_EXPIRY, *((UInt8*)(p_data->buf)));
                        break;
                }
            }
            break;
			/* SPR 21889 end */
	}

	oam_mem_free(p_data, &error_code);
    /* Coverity 113377 Fix Start*/
    /* Coverity 111587 Fix Start*/
    /* Code Remove */
    /* Coverity 111587 Fix End*/
    /* Coverity 113377 Fix End*/
	OAM_FUN_EXIT_TRACE();
	return ret_val;
}
/******************************************************************************
 *   FUNCTION NAME   : oam_maintain_tcb
 *   DESCRIPTION     : It is responsible to maintaining tcb entries and starting 
 *                     Module timmer for Qcli command
 *   INPUT           : trans_id :transaction identifier
 *                     trans_type :transaction type
 *                     msg_size :size of message
 *                     p_api_buf :Pointer to Buffer 
 *   OUTPUT          : None
 *   RETURNS         : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/

oam_return_et 
oam_maintain_tcb
(
 UInt16 trans_id,
 oam_trans_type_et trans_type,
 UInt16 msg_size,
 void *p_api_buf
 )
{
	OAM_FUN_ENTRY_TRACE();
    /*SPR 22465 Fix Start */
    /* Code Deleted */
    /*SPR 22465 Fix End */
	UInt32 timer_s = OAM_LAYER_INTF_RESP_TIMEOUT_S; /* Timer value in seconds */
	QTIMER timer_id = OAM_ZERO;
	UInt16 timer_type = OAM_GUARD_TIMER_TYPE;
	/* For calling any TCB related functions, trans_id must be of 4 bytes */
	UInt32 trans_id_new = trans_id;
	oam_return_et retVal = OAM_FAILURE;

    /*SPR 22465 Fix Start */
    /* Code Deleted */
    /*SPR 22465 Fix End */

#ifdef OAM_RADIO_HEAD_SUPPORT
	if(OAM_TRANS_RRH == trans_type) {
		OAM_LOG(OAM, OAM_DETAILED, "Setting request timer value: %d s",
				oam_prov_req.oam_rcm_req.rrh_timer);
		timer_s = oam_prov_req.oam_rcm_req.rrh_timer;
	}
#endif

	if(OAM_TRANS_RRM == trans_type &&
			(oam_prov_req.system_status.event_ongoing &
			 OAM_SET_ADMIN_STATE_RESP_PENDING) &&
			(oam_prov_req.system_status.event_ongoing &
			 OAM_RRM_CELL_BLOCK_REQ_MASK)) {
		timer_type = OAM_CELL_BLOCK_TIMER_TYPE;
	}
	if(OAM_ONE == oam_sys_log)
	{
		timer_type = OAM_SYSTEM_LOG_TIMER_TYPE;
		OAM_LOG(OAM, OAM_DETAILED, "OAM_SYSTEM_LOG_TIMER_TYPE called");
		oam_sys_log = OAM_ZERO;
	}
	/*SPR 22465 Fix Start */
	timer_id = oam_start_new_timer_and_maintain_trans_id(OAM_SUB_MODULE_MIH,
			OAM_MODULE_ID,
			timer_type,
			(timer_s * 1000), /* Timer value in ms */
			OAM_NULL, OAM_ZERO,
			OAM_FALSE,
			trans_id_new);
	/*SPR 22465 Fix End */
	OAM_LOG(OAM, OAM_DETAILED, "trans_id Saved is: %d", trans_id_new);
	retVal = oam_tcb_insert(trans_id_new, timer_id, trans_type,
			msg_size, p_api_buf);
	if(OAM_FAILURE == retVal) {
		OAM_LOG(OAM, OAM_WARNING,
				"Unable to Maintain Transaction Details in TCB");
	}
    /*SPR 22465 Fix Start */
    /* Code Deleted */
    /*SPR 22465 Fix End */

	OAM_FUN_EXIT_TRACE();
	return retVal;
}

/* SPR_9620_CHANGES_START */
/******************************************************************************
 *   FUNCTION NAME   : oam_maintain_tcb_with_timer
 *   DESCRIPTION     : It is responsible to maintaining tcb entries and starting 
 *                     Module timmer for Qcli command
 *   INPUT           : trans_id :transaction identifier
 *                     trans_type :transaction type
 *                     msg_size :size of message
 *                     p_api_buf :Pointer to Buffer 
 *   OUTPUT          : None
 *   RETURNS         : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/

oam_return_et 
oam_maintain_tcb_with_timer
(
 UInt16 trans_id,
 UInt16 sub_module_id,
 oam_trans_type_et trans_type,
 UInt16 msg_size,
 UInt8 *p_api_buf
 )
{
	OAM_FUN_ENTRY_TRACE();
    /*SPR 22465 Fix Start */
    /* Code Deleted */
    /*SPR 22465 Fix End */
	/*UInt32 timer_s = OAM_LAYER_INTF_RESP_TIMEOUT_S;  Timer value in seconds */
	QTIMER timer_id = OAM_ZERO;
	UInt16 timer_type = OAM_GUARD_TIMER_TYPE;
	/* For calling any TCB related functions, trans_id must be of 4 bytes */
	UInt32 trans_id_new = trans_id;
	oam_return_et retVal = OAM_FAILURE;
    /*SPR 22465 Fix Start */
    /* Code Deleted */
    /*SPR 22465 Fix End */
#ifdef OAM_RADIO_HEAD_SUPPORT
	if(OAM_TRANS_RRH == trans_type) {
		OAM_LOG(OAM, OAM_DETAILED, "Setting request timer value: %d s",
				oam_prov_req.oam_rcm_req.rrh_timer);
		timer_s = oam_prov_req.oam_rcm_req.rrh_timer;
	}
#endif

	if(OAM_TRANS_RRM == trans_type &&
			(oam_prov_req.system_status.event_ongoing &
			 OAM_SET_ADMIN_STATE_RESP_PENDING) &&
			(oam_prov_req.system_status.event_ongoing &
			 OAM_RRM_CELL_BLOCK_REQ_MASK)) {
		timer_type = OAM_CELL_BLOCK_TIMER_TYPE;
	}
	if(OAM_ONE == oam_sys_log)
	{
		timer_type = OAM_SYSTEM_LOG_TIMER_TYPE;
		OAM_LOG(OAM, OAM_DETAILED, "OAM_SYSTEM_LOG_TIMER_TYPE called");
		oam_sys_log = OAM_ZERO;
	}
    /*SPR 22465 Fix Start */
	timer_id = oam_start_guard_timer(trans_id_new,sub_module_id,
			OAM_MODULE_ID,
			timer_type,
			OAM_GUARD_TIMER_DURATION, /* Timer value in ms */
			OAM_NULL, OAM_ZERO,
			OAM_FALSE);
    /*SPR 22465 Fix End */
	OAM_LOG(OAM, OAM_DETAILED, "trans_id Saved is: %d", trans_id_new);
	retVal = oam_tcb_insert(trans_id_new, timer_id, trans_type,
			msg_size, p_api_buf);
	if(OAM_FAILURE == retVal) {
		OAM_LOG(OAM, OAM_WARNING,
				"Unable to Maintain Transaction Details in TCB");
	}

    /*SPR 22465 Fix Start */
    /* Code Deleted */
    /*SPR 22465 Fix End */

	OAM_FUN_EXIT_TRACE();
	return retVal;
}
/* SPR_9620_CHANGES_END */

/******************************************************************************
 *   FUNCTION NAME   : oam_qcli_send_All_response
 *   DESCRIPTION     : It sends message to qcli
 *   INPUT           : p_msg :Pointer to message
 *                     trans_id :transaction id
 *                     api_id :API id
 *                     msg_size :message size   
 *   OUTPUT          : None
 *   RETURNS         : OAM_SUCCESS/OAM_FAILURE
 *****************************************************************************/
oam_return_et 
oam_qcli_send_All_response
(
 void *p_msg,
 UInt16 trans_id,
 UInt16 api_id,
 UInt16 msg_size
 )
{
	/*CID:66598 start*/
	OAM_FUN_ENTRY_TRACE();
	UInt8 *p_buffer=OAM_NULL;
	UInt8 *p_api_buf=OAM_NULL;
	oam_error_code_et error_code;
	UInt32 tcb_id =trans_id;
	UInt8 *p_req_msg=OAM_NULL;
	UInt16 dest_id = OAM_ZERO;

	oam_struct_t *node = oam_tcb_get_detail(&tcb_id);
	if(node)
	{
		p_req_msg = node->oam_tcb_context;
		dest_id = get_word_from_header((UInt8*)(p_req_msg) + OAM_TWO);
		if(node->timer_id!=OAM_ZERO)
		{
			/*SPR 13617 START*/
			if( oam_stop_timer(node->timer_id) == OAM_SUCCESS)
			{
				/*SPR 13617 END*/
				OAM_LOG(OAM,OAM_DETAILED,"Stopping Timer");
			}
			else
			{
				OAM_LOG(OAM,OAM_WARNING," Trying to stop inavlid timer[%p]",
                    node->timer_id);
			}
		}
	}

	else
	{
		OAM_LOG(OAM,OAM_WARNING,"Failure in fetching TCB entry");
		return OAM_FAILURE; 
	}
	oam_tcb_delete(&tcb_id);
	if(OAM_FAILURE == oam_mem_alloc(msg_size+ OAM_MGMT_INTF_HEADER_SIZE +OAM_CSPL_HDR_SIZE,
				(void *)&p_buffer,
				&error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_buffer failed"
                " with Error Code:%d",
                error_code);
		OAM_FUN_EXIT_TRACE();
		return (OAM_FAILURE);
	}
	p_api_buf=p_buffer;
	/* encode the CSPL header in the message */
	oam_construct_api_header(p_buffer,OAM_VERSION,OAM_MODULE_ID,
			dest_id,
			api_id, msg_size+ OAM_MGMT_INTF_HEADER_SIZE +OAM_CSPL_HDR_SIZE);

	p_buffer += OAM_CSPL_HDR_SIZE;



	/* Fill API header in message*/
	oam_construct_mgmt_api_hdr((UInt8 *)p_buffer,
			trans_id,
			OAM_MODULE_ID,
			dest_id,
			api_id,
			msg_size + OAM_MGMT_INTF_HEADER_SIZE,
			OAM_ZERO);
            /*SPR 17777 fix*/
	p_buffer += OAM_MGMT_INTF_HEADER_SIZE;

	oam_memcpy(p_buffer, p_msg, msg_size);
	/* Send message to MAC */
	if(OAM_SUCCESS == oam_send_message(p_api_buf, dest_id,&error_code))
	{
		OAM_LOG(OAM,OAM_DETAILED,"Sending message to Mangement Interface");

	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
	/*CID:66598 end*/
}


/******************************************************************************
 *   FUNCTION NAME   : oam_layer_intf_thread_func
 *   DESCRIPTION     : It is a callback function of pthread_create for "layer interface"
 *   INPUT           : thread_arg :Pointer to thread
 *   OUTPUT          : None 
 *   RETURNS         : OAM_ZERO
 ******************************************************************************/

static void * 
oam_layer_intf_thread_func(
		void * thread_arg
		)
{
	OAM_FUN_ENTRY_TRACE();
	thread_arg_t *p_thread_arg = (thread_arg_t *)thread_arg;
	OAM_LOG(OAM,OAM_DETAILED," In Layer Intf Thread having module_id --%p : %p",
			p_thread_arg->driver_id,p_thread_arg->p_qwait);
	lgr_logger_register_thread(p_thread_arg->logical_id);
	qvDriverLoop(p_thread_arg->driver_id,p_thread_arg->p_qwait);
	OAM_FUN_EXIT_TRACE();
	return OAM_ZERO;
}

#ifdef OAM_SHARED_MEM_LOGGER
/******************************************************************************
 *   FUNCTION NAME   : oam_logger_thread_func
 *   DESCRIPTION     : It is a callback function of pthread_create for Logger
 *   INPUT           : thread_arg :Pointer to thread
 *   OUTPUT          : None
 *   RETURNS         : OAM_ZERO
 ******************************************************************************/
static void * 
oam_logger_thread_func(
		void * thread_arg
		)
{
	OAM_FUN_ENTRY_TRACE();
	thread_arg_t *p_thread_arg = (thread_arg_t *)thread_arg;
	OAM_LOG(OAM,OAM_DETAILED," In Logger Thread having module_id --%p : %p",
			p_thread_arg->driver_id,p_thread_arg->p_qwait);
    lgr_logger_register_thread(p_thread_arg->logical_id);
	qvDriverLoop(p_thread_arg->driver_id,p_thread_arg->p_qwait);
	OAM_FUN_EXIT_TRACE();
	return OAM_ZERO;
}
#endif

/******************************************************************************
 *   FUNCTION NAME   : oam_user_intf_thread_func
 *   DESCRIPTION     : It is a callback function of pthread_create for "USER interface"
 *   INPUT           : thread_arg :Pointer to thread
 *   OUTPUT          : None
 *   RETURNS         : OAM_ZERO
 ******************************************************************************/
static void * 
oam_user_intf_thread_func(
		void * thread_arg
		)
{
	OAM_FUN_ENTRY_TRACE();
	thread_arg_t *p_thread_arg = (thread_arg_t *)thread_arg;
	OAM_LOG(OAM,OAM_DETAILED," In User Intf Thread having module_id --%p : %p",
			p_thread_arg->driver_id,p_thread_arg->p_qwait);
	lgr_logger_register_thread(p_thread_arg->logical_id);
	qvDriverLoop(p_thread_arg->driver_id,p_thread_arg->p_qwait);
	OAM_FUN_EXIT_TRACE();
	return OAM_ZERO;
}
/********************************PM_CODE**************************************/

/******************************************************************************
 *   FUNCTION NAME   : oam_PM_thread_func
 *   DESCRIPTION     : It is a function for PM thread
 *   INPUT           : thread_arg :Pointer to thread
 *   OUTPUT          : None
 *   RETURNS         : OAM_ZERO
 ******************************************************************************/
static void * 
oam_PM_thread_func(
		void * thread_arg
		)
{
	OAM_FUN_ENTRY_TRACE();
	thread_arg_t *p_thread_arg = (thread_arg_t *)thread_arg;
	OAM_LOG(OAM,OAM_DETAILED," In PM Thread having module_id --%p : %p",
			p_thread_arg->driver_id,p_thread_arg->p_qwait);
    lgr_logger_register_thread(p_thread_arg->logical_id);
	qvDriverLoop(p_thread_arg->driver_id,p_thread_arg->p_qwait);
	return OAM_ZERO;
}
/********************************PM_CODE**************************************/
/*******************************FM Code***************************************/

/******************************************************************************
 *   FUNCTION NAME   : oam_fm_thread_func
 *   DESCRIPTION     : It is a function for FM thread
 *   INPUT           : thread_arg :Pointer to thread
 *   OUTPUT          : None
 *   RETURNS         : OAM_ZERO
 ******************************************************************************/
static void * 
oam_fm_thread_func(
		void * thread_arg
		)
{
	OAM_FUN_ENTRY_TRACE();
	thread_arg_t *p_thread_arg = (thread_arg_t *)thread_arg;
	OAM_LOG(OAM,OAM_DETAILED," In FM Thread having module_id --%p : %p",
			p_thread_arg->driver_id,p_thread_arg->p_qwait);
    lgr_logger_register_thread(p_thread_arg->logical_id);
	qvDriverLoop(p_thread_arg->driver_id,p_thread_arg->p_qwait);
	return OAM_ZERO;
}
/****************************************************************************/
/* SPR 21889 +- */ 
/*****************************************************************************
 *   FUNCTION NAME   : oam_sdm_send_active_ind
 *   DESCRIPTION     : This function Sends Active Indication to SDM
 *   INPUT           : none
 *   OUTPUT          : none  
 *   RETURNS         : OAM_FAILURE/OAM_SUCCESS
 ******************************************************************************/
/* + SPR 17439 */
	oam_return_et
oam_sdm_send_active_ind(void)
/* + SPR 17439 */
{

	/* Sending Indication to SDM For Activation of OAM for polling functionality */
	UInt8 *p_buffer =OAM_NULL;
	UInt8 *p_buf = OAM_NULL;
	UInt16 trans_id = oam_get_new_trans_id();
	oam_error_code_et p_error_code = NO_ERROR;
	UInt16 msg_size = OAM_CSPL_HDR_SIZE+OAM_TR069_INTF_HEADER_SIZE;

	OAM_FUN_ENTRY_TRACE();
	if(OAM_FAILURE == oam_mem_alloc(msg_size,(void *)&p_buffer,&p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_buffer failed"
                " with Error Code:%d",
				p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	oam_memset(p_buffer , OAM_ZERO , msg_size);
	p_buf = p_buffer;
	oam_construct_api_header(p_buf,OAM_VERSION,OAM_MODULE_ID,OAM_SDM_MODULE_ID,
			OAM_SDM_ACTIVE_IND,msg_size);

	p_buf = p_buffer+OAM_CSPL_HDR_SIZE;

	oam_construct_mgmt_api_hdr(p_buf,trans_id,OAM_MODULE_ID,
			OAM_SDM_MODULE_ID,OAM_SDM_ACTIVE_IND,
			msg_size-OAM_CSPL_HDR_SIZE,OAM_ZERO);
            /*SPR 17777 fix*/


	if(OAM_SUCCESS == oam_send_message(p_buffer,OAM_SDM_MODULE_ID,&p_error_code))
	{
		OAM_LOG(OAM,OAM_INFO,"Sending OAM ACTIVATION Indication to SDM");
	}
	else
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending message to SDM");
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}


#ifdef OAM_SHARED_MEM_LOGGER
/******************************************************************************
 *   FUNCTION NAME: oam_create_logger_thread
 *
 *       This function creates LOGGER thread for reading the logs from shared 
 *       memory.
 *
 *   RETURNS:
 *       None
 *
 ******************************************************************************/
	oam_return_et oam_create_logger_thread
(
)
{
    cpu_set_t cpuSetThrd;
    QDRIVER logger_driver;
    pthread_t logger_tid;
    pthread_attr_t pthread_attr;
    size_t stack_size = OAM_ZERO;
/*SPR 22554 Fix Start*/
    Char8  threadName[OAM_THREAD_NAME_LEN] = "oam_logger";
/*SPR 22554 Fix End*/
    pthread_attr_init(&pthread_attr);
    if (OAM_ZERO != oam_pthread_attr_getstacksize(&pthread_attr, &stack_size))
    {
        OAM_LOG(OAM, OAM_WARNING, "Failed to get Stack Size");
    }
    else
    {
        OAM_LOG(OAM, OAM_DETAILED, "Current Stack Size:%d", stack_size);
    }
    if (OAM_ZERO != oam_pthread_attr_setstacksize(&pthread_attr, OAM_THREAD_STACK_SIZE))
    {
        OAM_LOG(OAM, OAM_WARNING, "Failed to set Stack Size");
    }
    else
    {
        stack_size = OAM_ZERO;
        oam_pthread_attr_getstacksize(&pthread_attr, &stack_size);
        OAM_LOG(OAM, OAM_DETAILED, "Updated Stack Size:%d--", stack_size);
    }
    logger_driver = qvNewDriver();
    logger_thd_arg.driver_id = logger_driver;
    logger_thd_arg.p_qwait = (QWAIT*)waitstruct;
    logger_thd_arg.logical_id = num_active_threads;
    qvRegister(logger_driver, "OAM_LOGGER_MODULE_ID", OAM_LOGGER_MODULE_ID, &oamLoggerEntity, 0);
    /* creating Driver loop for Logger Thread */
    if (oam_pthread_create(&logger_tid, &pthread_attr, oam_logger_thread_func, (void *)&logger_thd_arg))
    {
        OAM_LOG(OAM, OAM_WARNING, "Unable to create thread for Logger");
        return OAM_FAILURE;
    }
/*SPR 22554 Fix Start*/

    /* Set name of the OAM logger thread */
    if (0 != oam_pthread_setname_np(&logger_tid, threadName))
    {
        OAM_LOG(OAM,OAM_WARNING, "Failed to set name of the OAM logger thread \n");
    }

/*SPR 22554 Fix End*/
  /* Binding Core to logger Thread */
    CPU_ZERO(&cpuSetThrd);
    if(OAM_TRUE == oam_prov_req.oam_igd_params.oam_shared_mem_conf.is_shared_mem_conf_present)
    {
        CPU_SET (oam_prov_req.oam_igd_params.
                oam_shared_mem_conf.cpu_bitmap,
                &cpuSetThrd);
    }
    else
    {
        CPU_SET (SHM_LOG_CORE,
                &cpuSetThrd);
    }
    if (pthread_setaffinity_np(logger_tid, sizeof(cpu_set_t),
                &cpuSetThrd))
    {
        OAM_LOG(OAM, OAM_WARNING, "pthread_setaffinity_np Returned Error"
                "for thread %d", logger_tid);
        return OAM_FAILURE;
    }
    else
    {
        CPU_ZERO(&cpuSetThrd);
        if (0 == pthread_getaffinity_np(logger_tid, 
                    sizeof(cpu_set_t), &cpuSetThrd))
        {
            /* SPR_17377_Warning_fix + */
            OAM_LOG(OAM, OAM_INFO, "Core Binding is successful for Logger thread %d",
                    logger_tid);
            /* SPR_17377_Warning_fix - */
        }
    }
    pthread_attr_destroy(&pthread_attr);
    /*SPR 21889 Fix +- */
    return OAM_SUCCESS;
}
#endif
/******************************************************************************
 *   FUNCTION NAME   : oam_multi_thd_init
 *   DESCRIPTION     : This function creates user interface thd and layer interface thd
 *                     and also responsible for initializing the cspl library
 *   INPUT           : none
 *   OUTPUT          : none
 *   RETURNS         : OAM_FAILURE/OAM_SUCCESS
 ******************************************************************************/

 /* + SPR 17439 */
int oam_multi_thd_init(void)
{
 /* - SPR 17439 */
	pthread_attr_t pthread_attr;
	size_t stack_size = OAM_ZERO;
	pthread_attr_init(&pthread_attr);
	QDRIVER layer_intf_driver,user_intf_driver,pm_intf_driver, FM_intf_driver;

	thread_arg_t layer_intf_thd_arg,user_intf_thd_arg,pm_intf_thd_arg, FM_intf_thd_arg;

	pthread_t layer_intf_tid,user_intf_tid,pm_tid,FM_intf_tid;
#ifdef OAM_SHARED_MEM_LOGGER
    lgr_shm_logging_config_t thd_cfg[OAM_THREAD_NUM];
    U8                       loop_index;
#endif

	void * listener;

/*SPR 22554 Fix Start*/
    Char8  threadName[OAM_THREAD_NAME_LEN] = "oam_layer_intf";
/*SPR 22554 Fix End*/
	OAM_FUN_ENTRY_TRACE();


	listener = qvOpen(operations,OAM_NULL );
	pm_mutex_lock = qvNewLock();
	if (listener == OAM_NULL)
	{
		OAM_LOG(OAM,OAM_ERROR,"Unable to create Listener");
		return OAM_FAILURE;
	}
#ifdef OAM_SHARED_MEM_LOGGER
    oam_strcpy(thd_cfg[num_active_threads].thread_name, "oam_main");
    oam_strcpy(thd_cfg[num_active_threads].lgr_region_name, "OAM_MAIN");

    lgr_logger_register_thread(num_active_threads);
#endif
    num_active_threads++;

	layer_intf_driver = qvNewDriver();
	layer_intf_thd_arg.driver_id = layer_intf_driver;
    layer_intf_thd_arg.logical_id = num_active_threads;
	layer_intf_thd_arg.p_qwait = (QWAIT*)waitstruct;
#ifdef OAM_SHARED_MEM_LOGGER
    oam_strcpy(thd_cfg[num_active_threads].thread_name, "oam_layer_intf");
    oam_strcpy(thd_cfg[num_active_threads].lgr_region_name, "OAM_LAYER_INTF");
#endif
    num_active_threads++;

	user_intf_driver = qvNewDriver();
	user_intf_thd_arg.driver_id = user_intf_driver;
    user_intf_thd_arg.logical_id = num_active_threads;
	user_intf_thd_arg.p_qwait = (QWAIT*)waitstruct;
#ifdef OAM_SHARED_MEM_LOGGER
    oam_strcpy(thd_cfg[num_active_threads].thread_name, "oam_user_intf");
    oam_strcpy(thd_cfg[num_active_threads].lgr_region_name, "OAM_USER_INTF");
#endif
    num_active_threads++;

	/* Fault Management Change Start */
	FM_intf_driver = qvNewDriver();
	FM_intf_thd_arg.driver_id = FM_intf_driver; 
    FM_intf_thd_arg.logical_id = num_active_threads;
	FM_intf_thd_arg.p_qwait = (QWAIT*)waitstruct;
#ifdef OAM_SHARED_MEM_LOGGER
    oam_strcpy(thd_cfg[num_active_threads].thread_name, "oam_fm_intf");
    oam_strcpy(thd_cfg[num_active_threads].lgr_region_name, "OAM_FM_INTF");
#endif
    num_active_threads++;
	/* Fault Management Change End */

	pm_intf_driver = qvNewDriver();
	pm_intf_thd_arg.driver_id = pm_intf_driver;
    pm_intf_thd_arg.logical_id = num_active_threads;
	pm_intf_thd_arg.p_qwait = (QWAIT*)waitstruct;
#ifdef OAM_SHARED_MEM_LOGGER
    oam_strcpy(thd_cfg[num_active_threads].thread_name, "oam_pm_intf");
    oam_strcpy(thd_cfg[num_active_threads].lgr_region_name, "OAM_PM_INTF");
#endif
    num_active_threads++;

	qvRegister(layer_intf_driver, "OAM_MODULE_ID",OAM_MODULE_ID ,
			&LayerEntity,(void *)&layer_intf_thd_arg);
	qvRegister(user_intf_driver, "QCLI_INTERFACE_MODULE_ID",
			QCLI_INTERFACE_MODULE_ID, &QCLIEntity,(void *)&user_intf_thd_arg);
	qvRegister(pm_intf_driver, "PM_MODULE_ID",PM_MODULE_ID ,
			&PMEntity,(void *)&pm_intf_thd_arg);
	/* Fault Management Change Start */
	qvRegister(FM_intf_driver, "FM_MODULE_ID",FM_MODULE_ID ,
			&FMEntity,(void *)&FM_intf_thd_arg);
	/* Fault Management Change End */
	qvSetDestination(RRM_MODULE_ID, operations, listener,OAM_ZERO);
	qvSetDestination(SON_MODULE_ID,operations,listener,OAM_ZERO);
	qvSetDestination(RLC_MODULE_ID, operations, listener,OAM_ZERO);
	qvSetDestination(PDCP_MODULE_ID,operations,listener,OAM_ZERO);
	qvSetDestination(MAC_MODULE_ID, operations, listener,OAM_ZERO);
	qvSetDestination(EGTPU_MODULE_ID,operations,listener,OAM_ZERO);
	qvSetDestination(RRC_MODULE_ID, operations, listener,OAM_ZERO);
	qvSetDestination(QCLI_MODULE_ID,operations,listener,OAM_ZERO);
#ifdef OAM_RADIO_HEAD_SUPPORT
#ifdef OAM_RRH_TYPE_1_SUPPORT
	qvSetDestination(RCM_MODULE_ID, operations, listener, OAM_ZERO);
#endif
#endif
	qvSetDestination(OAM_SDM_MODULE_ID,operations,listener,OAM_ZERO);
	qvSetDestination(OAM_AGNT_MODULE_ID, operations, listener,OAM_ZERO);
	qvSetDestination(OAM_SNMP_MODULE_ID,operations,listener,OAM_ZERO);
	qvSetDestination(OAM_SNMP_INTF_MODULE_ID,operations,listener,OAM_ZERO);
	qvSetDestination(TR069_MODULE_ID,operations,listener,OAM_ZERO);
	qvSetDestination(OAM_QCLI_TRAP_MODULE_ID, operations, listener,OAM_ZERO);



	if(oam_init_hash_table()==OAM_FAILURE)
	{
		OAM_LOG(OAM,OAM_ERROR,
				"Initialization of Hash Table Fails!!");
		return OAM_FAILURE;
	}
	/*All module init will be done inside these funtion*/
	oam_CoM_init_modules();

	if (OAM_ZERO != oam_pthread_attr_getstacksize(&pthread_attr, &stack_size))
	{
        OAM_LOG(OAM, OAM_WARNING, "Failed to get Stack Size");
	}
	else
	{
		OAM_LOG(OAM, OAM_DETAILED, "Current Stack Size:%d", stack_size);
	}
	if (OAM_ZERO != oam_pthread_attr_setstacksize(&pthread_attr, OAM_THREAD_STACK_SIZE))
	{
        OAM_LOG(OAM, OAM_WARNING, "Failed to set Stack Size");
	}
	else
	{
		stack_size = OAM_ZERO;
		oam_pthread_attr_getstacksize(&pthread_attr, &stack_size);
		OAM_LOG(OAM, OAM_DETAILED, "Updated Stack Size:%d--", stack_size);
	}

	oam_pthread_create(&layer_intf_tid, &pthread_attr, oam_layer_intf_thread_func,
			(void *)&layer_intf_thd_arg);
/*SPR 22554 Fix Start*/

    /* Set name of the OAM layer interface thread */
    if (0 != oam_pthread_setname_np(&layer_intf_tid, threadName))
    {
        OAM_LOG(OAM, OAM_WARNING, "Failed to set name of the OAM layer interface thread");
    }

/*SPR 22554 Fix End*/
	oam_pthread_create(&user_intf_tid, &pthread_attr, oam_user_intf_thread_func,
			(void *)&user_intf_thd_arg);
/*SPR 22554 Fix Start*/

    /* Set name of the OAM user interface thread */
    oam_snprintf(threadName, MAX_THREAD_NAME_LEN, "oam_usr_intf");
    if (0 != oam_pthread_setname_np(&user_intf_tid, threadName))
    {
        OAM_LOG(OAM, OAM_WARNING, "Failed to set name of the OAM user interface thread");
    }

/*SPR 22554 Fix End*/
	oam_pthread_create(&pm_tid, &pthread_attr,oam_PM_thread_func,
			(void *)&pm_intf_thd_arg);
/*SPR 22554 Fix Start*/

    /* Set name of the OAM PM thread */
    oam_snprintf(threadName, MAX_THREAD_NAME_LEN, "oam_pm");
    if (0 != oam_pthread_setname_np(&pm_tid, threadName))
    {
        OAM_LOG(OAM,OAM_WARNING, "Failed to set name of the OAM PM thread ");
    }

/*SPR 22554 Fix End*/
	oam_pthread_create(&FM_intf_tid, &pthread_attr, oam_fm_thread_func,
			(void *)&FM_intf_thd_arg);
/*SPR 22554 Fix Start*/

    /* Set name of the OAM FM thread */
    oam_snprintf(threadName, MAX_THREAD_NAME_LEN, "oam_fm");
    if (0 != oam_pthread_setname_np(&FM_intf_tid, threadName))
    {
        OAM_LOG(OAM,OAM_WARNING, "Failed to set name of the OAM FM thread \n");
    }

/*SPR 22554 Fix End*/
#ifdef OAM_SHARED_MEM_LOGGER
    oam_strcpy(thd_cfg[num_active_threads].thread_name, "oam_logger");
    oam_strcpy(thd_cfg[num_active_threads].lgr_region_name, "OAM_LOGGER");

    if(OAM_TRUE == oam_prov_req.oam_igd_params.oam_shared_mem_conf.is_shared_mem_conf_present)
    {
        for(loop_index = 0; loop_index < num_active_threads + OAM_ONE; loop_index++)
        {
            thd_cfg[loop_index].p_call_back = RRC_NULL;
            thd_cfg[loop_index].p_cbk_arg = RRC_NULL; 
            thd_cfg[loop_index].shm_size = oam_prov_req.oam_igd_params.
                oam_shared_mem_conf.shm_size;
            thd_cfg[loop_index].log_file_sz = oam_prov_req.oam_igd_params.
                oam_shared_mem_conf.log_file_size;
            thd_cfg[loop_index].file_cnt = oam_prov_req.oam_igd_params.
                oam_shared_mem_conf.file_count;
            /* SPR 22071 Logging Changes Start */
            thd_cfg[loop_index].log_chunk_cnt = oam_prov_req.oam_igd_params.
                                 oam_shared_mem_conf.log_chunk_count;
            /* SPR 22071 Logging Changes End */
        }
        if(LGR_SUCCESS == lgr_in_mem_logging_init(num_active_threads + OAM_ONE,
                    thd_cfg,
                    (const char *)oam_prov_req.oam_igd_params.
                    oam_shared_mem_conf.log_path))
        {
            OAM_LOG(OAM, OAM_INFO, "Created SharedMemory");
        }
        else
        {
            OAM_LOG(OAM, OAM_WARNING,
                    "Couldn't init Shared Memory logging!");
        }
    }
    else
    {
        for(loop_index = 0; loop_index < num_active_threads + OAM_ONE; loop_index++)
        {
            thd_cfg[loop_index].p_call_back = RRC_NULL;
            thd_cfg[loop_index].p_cbk_arg = RRC_NULL; 
            thd_cfg[loop_index].shm_size = SHM_SIZE;
            thd_cfg[loop_index].log_file_sz = SHM_LOG_FILE_SIZE;
            thd_cfg[loop_index].file_cnt = SHM_LOG_FILE_COUNT;
            /* SPR 22071 Logging Changes Start */
            thd_cfg[loop_index].log_chunk_cnt = SHM_LOG_CHUNK_COUNT;
            /* SPR 22071 Logging Changes End */
        }
        if(LGR_SUCCESS == lgr_in_mem_logging_init(num_active_threads + OAM_ONE,
                    thd_cfg,
                    (char*)SHM_LOG_FILE_PATH))
        {
            OAM_LOG(OAM, OAM_INFO, "Created SharedMemory");
        }
        else
        {
            OAM_LOG(OAM, OAM_WARNING,
                    "Couldn't init Shared Memory logging!");
        }
    }
    oam_create_logger_thread();
#endif

    /* SPR 8725 */
/* SPR 8725 end */
	pthread_attr_destroy(&pthread_attr);
	qvInputLoop(operations,listener);
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME   : oam_send_l2_proc_sup_req
 *   DESCRIPTION     : This function sends OAM_PROC_SUP_REQ message to L2
 *   INPUT           : src_module_id :source module id
 *                     transaction_id :transaction id
 *   OUTPUT          : none
 *   RETURNS         : OAM_FAILURE/OAM_SUCCESS
 ******************************************************************************/
oam_return_et
oam_send_l2_proc_sup_req
(
 U16                 src_module_id,  /* Source module identifier */
 U16                 transaction_id  /* Interface transaction identifier */
 )
{
	U16 		msg_length,
	msg_api_length;
	U8 			*p_msg,
				*p_oam_msg;

	msg_length	    =  OAM_ZERO;
	msg_api_length  = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

    /* Coverity CID 115303 +*/
	oam_error_code_et p_error_code = NO_ERROR;
    /* Coverity CID 115303 -*/
	/* Allocate buffer */
	p_msg = (U8*)oam_msg_mem_get(msg_api_length);

	if (p_msg == PNULL)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}

	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_oam_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_oam_msg, OAM_VERSION_ID, src_module_id,
			MAC_MODULE_ID, OAM_PROC_SUP_REQ, msg_api_length);

	/* Fill interface header */
	p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_oam_msg,
			transaction_id,
			src_module_id,
			MAC_MODULE_ID,
			OAM_PROC_SUP_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Send message to MAC*/
	/* Coverity CID 108817 Fix Start */
	if(OAM_FAILURE == oam_send_message(p_msg,MAC_MODULE_ID,&p_error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Sendto failed with err_code %d",p_error_code);
	}
	/* Coverity CID 108817 Fix End */

	return OAM_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME   : oam_send_l3_proc_sup_req
 *   DESCRIPTION     : This function sends OAM_PROC_SUP_REQ message to L3
 *   INPUT           : src_module_id :source module id
 *                     transaction_id :transaction id
 *   OUTPUT          : none
 *   RETURNS         : OAM_FAILURE/OAM_SUCCESS
 ******************************************************************************/
oam_return_et
oam_send_l3_proc_sup_req
(
 U16                 src_module_id,  /* Source module identifier */
 U16                 transaction_id  /* Interface transaction identifier */
 )
{
	U16 		msg_length,
	msg_api_length;
	U8 			*p_msg,
				*p_oam_msg;
	/* Coverity CID 115305 +*/
	oam_error_code_et error_code = NO_ERROR;
	/* Coverity CID 115305 -*/
	msg_length		=  OAM_ZERO;
	msg_api_length 	= msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;


	/* Allocate buffer */
	p_msg = (U8*)oam_msg_mem_get(msg_api_length);

	if (p_msg == PNULL)
	{
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed");
		/* Not enough memory */
		return OAM_FAILURE;
	}

	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_oam_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_oam_msg, OAM_VERSION_ID, src_module_id,
			RRC_MODULE_ID, OAM_PROC_SUP_REQ, msg_api_length);

	/* Fill interface header */
	p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_oam_msg,
			transaction_id,
			src_module_id,
			RRC_MODULE_ID,
			OAM_PROC_SUP_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Send message to RRC*/
	/* Coverity CID 108815 Fix Start */
	if(OAM_FAILURE == oam_send_message(p_msg, RRC_MODULE_ID,&error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Sendto failed with err_code %d",error_code);
	}
	/* Coverity CID 108815 Fix End */

	return OAM_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME   : oam_send_rrm_proc_sup_req
 *   DESCRIPTION     : This function sends OAM_PROC_SUP_REQ message to RRM
 *   INPUT           : src_module_id :source module id
 *                     transaction_id :transaction id
 *   OUTPUT          : none
 *   RETURNS         : OAM_FAILURE/OAM_SUCCESS
 ******************************************************************************/
oam_return_et
oam_send_rrm_proc_sup_req
(
 U16                 src_module_id,  /* Source module identifier */
 U16                 transaction_id  /* Interface transaction identifier */
 )
{
	U16 		msg_length,
	msg_api_length;
	U8 			*p_msg,
				*p_oam_msg;
	/* Coverity CID 115306 +*/
	oam_error_code_et error_code = NO_ERROR;
	/* Coverity CID 115306 -*/
	msg_length		=  OAM_ZERO;
	msg_api_length 	= msg_length + OAM_CSPL_HDR_SIZE + RRM_INTERFACE_API_HEADER_SIZE;

	/* Allocate buffer */
	p_msg = (U8*)oam_msg_mem_get(msg_api_length);

	if (p_msg == PNULL)
	{
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed");
		/* Not enough memory */
		return OAM_FAILURE;
	}

	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_oam_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_oam_msg, OAM_VERSION_ID, src_module_id,
			RRM_MODULE_ID, OAM_PROC_SUP_REQ, msg_api_length);

	/* Fill interface header */
	p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_oam_msg,
			transaction_id,
			src_module_id,
			RRM_MODULE_ID,
			OAM_PROC_SUP_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Send message to RRM */
	/* Coverity CID 108818 Fix Start */
	if(OAM_FAILURE == oam_send_message(p_msg, RRM_MODULE_ID,&error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Sendto failed with err_code %d",error_code);
	}
	/* Coverity CID 108818 Fix End */

	return OAM_SUCCESS;
}
#ifdef OAM_SON_ENABLE
/******************************************************************************
 *   FUNCTION NAME   : oam_send_son_proc_sup_req
 *   DESCRIPTION     : This function sends OAM_PROC_SUP_REQ message to L3
 *   INPUT           : src_module_id :source module id
 *                     transaction_id :transaction id
 *   OUTPUT          : none
 *   RETURNS         : OAM_FAILURE/OAM_SUCCESS
 ******************************************************************************/
oam_return_et
oam_send_son_proc_sup_req
(
 U16                 src_module_id,  /* Source module identifier */
 U16                 transaction_id  /* Interface transaction identifier */
 )
{
	U16 		msg_length,
	msg_api_length;
	U8 			*p_msg,
				*p_oam_msg;
	/* Coverity CID 115304 +*/
	oam_error_code_et error_code = NO_ERROR;
	/* Coverity CID 115304 -*/
	msg_length		=  OAM_ZERO;
	msg_api_length 	= msg_length + OAM_CSPL_HDR_SIZE + SON_INTERFACE_API_HEADER_SIZE;

	/* Allocate buffer */
	p_msg = (U8*)oam_msg_mem_get(msg_api_length);

	if (p_msg == PNULL)
	{
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed");
		/* Not enough memory */
		return OAM_FAILURE;
	}

	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_oam_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_oam_msg, OAM_VERSION_ID, src_module_id,
			SON_MODULE_ID, OAM_PROC_SUP_REQ, msg_api_length);

	/* Fill interface header */
	p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_oam_msg,
			transaction_id,
			src_module_id,
			SON_MODULE_ID,
			OAM_PROC_SUP_REQ,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Send message to SON */
	/* Coverity CID 108819 Fix Start */
	if(OAM_FAILURE == oam_send_message(p_msg,SON_MODULE_ID,&error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Sendto failed with err_code %d",error_code);
	}
	/* Coverity CID 108819 Fix End */

	return OAM_SUCCESS;
}
#endif
/******************************************************************************
 *   FUNCTION NAME   : oam_send_proc_sup_req
 *   DESCRIPTION     : This function sends OAM_PROC_SUP_REQ message to L2,L3,RRM,SON
 *   INPUT           : dest_module_id :destination module id
 *                     transaction_id :transaction id
 *   OUTPUT          : none
 *   RETURNS         : OAM_FAILURE/OAM_SUCCESS
 *******************************************************************************/
oam_return_et
oam_send_proc_sup_req
(
 U16                 dest_module_id,  /* Destination module identifier */
 U16                 transaction_id  /* Interface transaction identifier */
 )
{
	U16 		msg_length=OAM_ZERO,
	msg_api_length=OAM_ZERO;
	U8 			*p_msg=OAM_NULL,
				*p_oam_msg=OAM_NULL;
	oam_error_code_et error_code=NO_ERROR;
	UInt16 api_id = OAM_PROC_SUP_REQ;
#ifdef OAM_RADIO_HEAD_SUPPORT
#ifdef OAM_RRH_TYPE_1_SUPPORT
	if(RCM_MODULE_ID == dest_module_id && oam_prov_req.rrh_flag &&
			oam_prov_req.rcm_flag) {
		api_id = OAM_RCM_PROC_SUP_REQ; 
	}
#endif
#endif
	OAM_FUN_ENTRY_TRACE();

	msg_length		=  OAM_ZERO;
	msg_api_length 	= msg_length + OAM_CSPL_HDR_SIZE + SON_INTERFACE_API_HEADER_SIZE;

	/* Allocate buffer */
	p_msg = (U8*)oam_msg_mem_get(msg_api_length);

	if (p_msg == PNULL)
	{
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed");
		/* Not enough memory */
		return OAM_FAILURE;
	}

	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_oam_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_oam_msg, OAM_VERSION_ID, OAM_MODULE_ID,
			dest_module_id, api_id, msg_api_length);

	/* Fill interface header */
	p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_oam_msg,
			transaction_id,
			OAM_MODULE_ID,
			dest_module_id,
			api_id,
			msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Send message to SON */
	/* Coverity CID 108816 Fix Start */
	if(OAM_FAILURE == oam_send_message(p_msg, dest_module_id,&error_code))
	{
		OAM_LOG(OAM, OAM_ERROR,
				"Sendto failed with err_code %d",error_code);
	}
	/* Coverity CID 108816 Fix End */

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_il_parse_proc_sup_resp
 * Inputs         : p_response : Pointer to Response
 *                  p_src :Pointer to source
 *                  length_left :left length
 *                  p_length_read :Pointer to length read
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function handles oam_proc_sup_res  message
 ****************************************************************************/
oam_return_et
oam_il_parse_proc_sup_resp
(
 S32 			*p_response,
 U8  			*p_src,
 S32 			length_left,
 S32 			*p_length_read
 )
{
	*p_length_read 	= OAM_ZERO;

	oam_memset((void *)p_response, OAM_ZERO, sizeof(*p_response));

	/* This function parses rrc_oam_reset_cell_stats_resp */
	OAM_LOG(OAM,OAM_DETAILED, "Parsing oam_proc_sup_resp");

	if (*p_length_read + (S32)sizeof(U8) > length_left)
	{
		OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of basic type */
	oam_cp_unpack_UInt8(&p_response, p_src + *p_length_read
			, "oam_proc_sup_resp"
			);
	*p_length_read += sizeof(U8);

	if (*p_length_read > length_left)
	{
		OAM_LOG(OAM,OAM_WARNING, "Incoming message damaged!");
		return OAM_FAILURE;
	}
	return(OAM_SUCCESS);
}


/****************************************************************************
 * Function Name  : oam_send_l2All_success_resp 
 * Inputs         : response :Response 
 *                  transaction_id :transaction id
 * Outputs        : p_error_code :Pointer to Error code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function sends l2all success response
 ****************************************************************************/
oam_return_et
oam_send_l2All_success_resp(
		oam_return_et response,
		UInt16 transaction_id,
		UInt16 dest_id,
		oam_error_code_et *p_error_code
		)
{
	UInt8 * p_msg;
	UInt8 * p_resp_msg;
	UInt8 msg_len = OAM_ZERO;
	msg_len = (sizeof(UInt8)) + LOWER_LAYER_INTERFACE_HEADER_SIZE + OAM_CSPL_HDR_SIZE;

	if (OAM_FAILURE == oam_mem_alloc(msg_len,(void *)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
                *p_error_code);
		OAM_FUN_EXIT_TRACE();
		/* Not Enough Memory */
		return OAM_FAILURE;
	}

	p_resp_msg = p_msg;

	oam_construct_api_header(p_resp_msg, OAM_VERSION_ID,OAM_MODULE_ID,
			dest_id, OAM_SET_LOG_LEVEL_L2_RESP,
			msg_len);

	p_resp_msg = p_resp_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_resp_msg,
			transaction_id,
			OAM_MODULE_ID,
			dest_id,
			OAM_SET_LOG_LEVEL_L2_RESP,
			sizeof(UInt8),
			oam_prov_req.cell_cntxt.curr_cell_cid);

	p_resp_msg = p_resp_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	UInt8 resp_send;
	if(OAM_SUCCESS == response){
		resp_send = OAM_ONE;
		oam_memcpy(p_resp_msg,&resp_send,sizeof(UInt8));
	}
	else{
		resp_send = OAM_ZERO;
		oam_memcpy(p_resp_msg,&resp_send,sizeof(UInt8));
	}

	if(OAM_SUCCESS == oam_send_message(p_msg, dest_id, p_error_code))
	{
		OAM_LOG(OAM,OAM_DETAILED,"Sending message to Management Interface");
	}
	else
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending message with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	return OAM_SUCCESS;
}


/****************************************************************************
 * Function Name  : oam_compose_and_send_inform_api
 * Inputs         : oam_inform_api :Pointer to pointer to oam_inform_api_t
 *                  num_of_parent :num of parents
 *                  event_code :Pointer to UInt8
 *                  dest_id :destination id
 *                  sub_module_id :sub module id
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function is used to compose and send api inform.
 ****************************************************************************/

oam_return_et
oam_compose_and_send_inform_api
(
 oam_inform_api_t **oam_inform_api,
 UInt16 num_of_parent,
 UInt8 *event_code,
 UInt16 dest_id,
 UInt16  sub_module_id
 )
{

	UInt16 loop=OAM_ZERO,inner_loop=OAM_ZERO;
	UInt8 *p_msg=OAM_NULL;
	UInt8 *p_api_msg=OAM_NULL;
	Char8 event_info[OAM_NOTIFICATION_API_EVENT_INFO_MAX_LEN] = {0};
	UInt32 resvd_byte_count = 0;
	UInt16 event_info_cntr = 0;
	UInt8 param_list_info[OAM_TEN];
	UInt16 param_list_info_cntr=OAM_ZERO;
	UInt8 *payload = OAM_NULL;
	UInt16 payload_cntr=OAM_ZERO;
	UInt8 parent_node_info[STR_LEN_264];
	UInt16 parent_node_info_cntr=OAM_ZERO;
	UInt8 *parameter_info = OAM_NULL;
	UInt16 parameter_info_cntr=OAM_ZERO;
	UInt16 temp_len=OAM_ZERO;
	UInt16 complete_msg_len=OAM_ZERO;
	UInt16 transaction_id=OAM_ZERO;
	oam_error_code_et error_code = NO_ERROR;
	oam_return_et retVal = OAM_FAILURE;
	oam_trans_type_et trans_type = OAM_TRANS_SON;

	if(OAM_FAILURE == oam_mem_alloc(5000, (void**)&parameter_info, &error_code)) 
	{
		OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to parameter_info failed"
                " with Error Code:%d",
                error_code);
		return retVal;
	}
	if(OAM_FAILURE == oam_mem_alloc(5000, (void**)&payload, &error_code)) 
	{
		OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to payload failed"
                " with Error Code:%d",
                error_code);
		/* Coverity ID : 72288 Fix Start*/	
		oam_mem_free(parameter_info,&error_code);
		/* Coverity ID : 72288 Fix End*/
		return retVal;
	}

	for(loop=OAM_ZERO;loop<num_of_parent;loop++)
	{
		for(inner_loop=OAM_ZERO;inner_loop< oam_inform_api[loop]->num_of_param;inner_loop++)
		{
			temp_len = sizeof(UInt16)
				+sizeof(UInt16)
				+oam_strlen((char *)oam_inform_api[loop]->param_list[inner_loop])
				+ OAM_ONE;
			parameter_info[parameter_info_cntr++] = (temp_len & OAM_UPPER_EIGHT_BITS_SET) >> OAM_EIGHT;
			parameter_info[parameter_info_cntr++] = (temp_len & OAM_LOWER_EIGHT_BITS_SET);
			parameter_info[parameter_info_cntr++] = OAM_ZERO;
			parameter_info[parameter_info_cntr++] = OAM_ZERO;
			oam_strncpy(((char *)parameter_info + parameter_info_cntr),((char *)oam_inform_api[loop]->param_list[inner_loop]),sizeof(oam_inform_api[loop]->param_list[inner_loop]));
			parameter_info_cntr += temp_len
				-(sizeof(UInt16)
						+sizeof(UInt16)+ OAM_ONE);
			parameter_info[parameter_info_cntr++] = '\0'; 

		}
		temp_len = sizeof(UInt16)
			+sizeof(UInt8)
			+sizeof(UInt8)
			+sizeof(UInt16)
			+sizeof(UInt16)
			+oam_strlen(((char *)oam_inform_api[loop]->parent_name))
			+ OAM_ONE;
		UInt16 temp_len_1 = temp_len + parameter_info_cntr;  
		parent_node_info[parent_node_info_cntr++] = (temp_len_1 & OAM_UPPER_EIGHT_BITS_SET) >> OAM_EIGHT;
		parent_node_info[parent_node_info_cntr++] = (temp_len_1 & OAM_LOWER_EIGHT_BITS_SET);
		parent_node_info[parent_node_info_cntr++] = oam_inform_api[loop]->num_of_param;
		parent_node_info[parent_node_info_cntr++] = OAM_ZERO;
		parent_node_info[parent_node_info_cntr++] = OAM_ZERO;
		parent_node_info[parent_node_info_cntr++] = OAM_ZERO;
		parent_node_info[parent_node_info_cntr++] = ((oam_strlen(((char *)oam_inform_api[loop]->parent_name))+OAM_ONE) & OAM_UPPER_EIGHT_BITS_SET) >> OAM_EIGHT;
		parent_node_info[parent_node_info_cntr++] = ((oam_strlen(((char *)oam_inform_api[loop]->parent_name))+OAM_ONE) & OAM_LOWER_EIGHT_BITS_SET);
		oam_strncpy(((char *)parent_node_info + parent_node_info_cntr),((char *)oam_inform_api[loop]->parent_name),sizeof(oam_inform_api[loop]->parent_name));

		parent_node_info_cntr += temp_len
			-(sizeof(UInt16)
					+sizeof(UInt8)
					+sizeof(UInt8)
					+sizeof(UInt16)
					+sizeof(UInt16)
					+OAM_ONE);
		parent_node_info[parent_node_info_cntr++] = '\0';
		oam_memcpy(payload + payload_cntr,parent_node_info,parent_node_info_cntr);
		payload_cntr += parent_node_info_cntr;
		oam_memcpy(payload + payload_cntr,parameter_info,parameter_info_cntr);
		payload_cntr += parameter_info_cntr;
		parent_node_info_cntr = OAM_ZERO;
		parameter_info_cntr   = OAM_ZERO;
	}
	temp_len =  payload_cntr 
		+sizeof(UInt16)
		+sizeof(UInt8)
		+sizeof(UInt8)
		+sizeof(UInt16)
		+sizeof(UInt16);
	param_list_info[param_list_info_cntr++] = (temp_len & OAM_UPPER_EIGHT_BITS_SET) >> OAM_EIGHT;
	param_list_info[param_list_info_cntr++] = (temp_len & OAM_LOWER_EIGHT_BITS_SET);
	param_list_info[param_list_info_cntr++] = num_of_parent;
	param_list_info[param_list_info_cntr++] = OAM_ZERO;
	param_list_info[param_list_info_cntr++] = OAM_ZERO;
	param_list_info[param_list_info_cntr++] = OAM_ZERO;
	param_list_info[param_list_info_cntr++] = OAM_ZERO;
	param_list_info[param_list_info_cntr++] = OAM_ZERO;

	temp_len = param_list_info_cntr
		+payload_cntr
		+sizeof(UInt16)
		+sizeof(UInt16)
		+sizeof(UInt16)
		+sizeof(UInt16)
		+sizeof(UInt8)
		+sizeof(UInt8)
		+sizeof(UInt16)
		+(event_code==OAM_NULL?OAM_ZERO:oam_strlen(((char *)event_code))+ OAM_ONE)
		+sizeof(UInt16)
		+sizeof(UInt16);
	event_info[event_info_cntr++] = (temp_len & OAM_UPPER_EIGHT_BITS_SET) >> OAM_EIGHT;
	event_info[event_info_cntr++] = (temp_len & OAM_LOWER_EIGHT_BITS_SET);

	resvd_byte_count = 6;
	oam_memset(event_info + event_info_cntr, 0, resvd_byte_count);
	event_info_cntr += resvd_byte_count;


	event_info[event_info_cntr++] = ((OAM_NULL == event_code) ? OAM_ZERO : (oam_strlen(((char *)event_code))+OAM_ONE));

	resvd_byte_count = 3;
	oam_memset(event_info + event_info_cntr, 0, resvd_byte_count);
	event_info_cntr += resvd_byte_count;


	if(OAM_NULL != event_code)
	{
		oam_strncpy(event_info + event_info_cntr,
				(Char8*)event_code,
				sizeof(event_info) - event_info_cntr - 1);
		event_info_cntr += oam_strlen((char *)event_code)+ OAM_ONE;
	}
	else
	{
		event_info_cntr += OAM_ONE;
	}

	resvd_byte_count = 4;
	if(sizeof(event_info) > (event_info_cntr + resvd_byte_count)) {
		oam_memset(event_info + event_info_cntr, 0, resvd_byte_count);
		event_info_cntr += resvd_byte_count;
	}
	else {
		OAM_LOG(OAM, OAM_WARNING, "Buffer overflow error.");
		retVal = OAM_FAILURE;
		/* Coverity ID : 72289 Fix Start*/
		oam_mem_free(payload,&error_code);
		/* Coverity ID : 72289 Fix End*/
		/* Coverity ID : 72288 Fix Start*/	
		oam_mem_free(parameter_info,&error_code);
		/* Coverity ID : 72288 Fix End*/
		return retVal;
	}

	complete_msg_len = OAM_CSPL_HDR_SIZE + OAM_MGMT_INTF_HEADER_SIZE +event_info_cntr+param_list_info_cntr+payload_cntr;
	if (OAM_FAILURE == oam_mem_alloc(complete_msg_len,
				(void *)&p_msg,&error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
                error_code);
		/* Not Enough Memory */
		/* Coverity ID : 72289 Fix Start*/
		oam_mem_free(payload,&error_code);
		/* Coverity ID : 72289 Fix End*/
		/* Coverity ID : 72288 Fix Start*/	
		oam_mem_free(parameter_info,&error_code);
		/* Coverity ID : 72288 Fix End*/
		return retVal;
	}
	transaction_id = oam_get_new_trans_id();
	p_api_msg = p_msg;
	oam_construct_api_header(p_api_msg, OAM_VERSION_ID,OAM_MODULE_ID,
			dest_id, OAM_NOTIFICATION_IND,
			complete_msg_len);

	p_api_msg += OAM_CSPL_HDR_SIZE;

	/* Fill API header in message*/
	oam_construct_mgmt_api_hdr(p_api_msg,
			transaction_id,
			OAM_MODULE_ID,
			dest_id,
			OAM_NOTIFICATION_IND,
			complete_msg_len - OAM_CSPL_HDR_SIZE,
		 OAM_ZERO);
            /*SPR 17777 fix*/
	p_api_msg = p_api_msg + OAM_MGMT_INTF_HEADER_SIZE;
	oam_memcpy(p_api_msg,event_info,event_info_cntr);
	p_api_msg +=event_info_cntr;
	oam_memcpy(p_api_msg,param_list_info,param_list_info_cntr);
	p_api_msg += param_list_info_cntr;
	oam_memcpy(p_api_msg,payload,payload_cntr);


	if( OAM_ZERO  !=  sub_module_id)
	{
		if(OAM_SUB_MODULE_PM == sub_module_id)
		{
			trans_type = OAM_TRANS_PM;
		}
		else if(OAM_SUB_MODULE_FM == sub_module_id)
		{
			trans_type =  OAM_TRANS_FM; 
		}
		else if(OAM_SUB_MODULE_SM == sub_module_id)
		{
			trans_type = OAM_TRANS_SM;
		}

		OAM_LOG(OAM,OAM_DETAILED,
				"in inform value change event trans_id:%d",transaction_id);
		oam_maintain_internal_tcb(transaction_id,trans_type,complete_msg_len,p_msg);  /* vishwa changes */

	}


	if(OAM_FAILURE == oam_send_message(p_msg, dest_id,&error_code))
	{
		OAM_LOG(OAM,OAM_DETAILED,"Sending message to Management Interface Failed");
	}

	oam_mem_free(parameter_info,&error_code);
	oam_mem_free(payload,&error_code);

	retVal = OAM_SUCCESS;
	OAM_FUN_EXIT_TRACE();

	return retVal;
}



/****************************************************************************
 * Function Name  : oam_inform_value_change_event
 * Description    : Oam Notification indicator 
 * Inputs         : p_input_inform_api :Pointer to oam_inform_api_t
 *                  sub_module_id :sub module id
 *                  dest_module_id :dest moduleid
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/

oam_return_et
oam_inform_value_change_event( 
		oam_inform_api_t   *p_input_inform_api, 
		UInt16              sub_module_id,
		UInt16              dest_module_id
		)
{
	oam_return_et    ret_val = OAM_SUCCESS;
	UInt8 notification_flag = OAM_ZERO;
	UInt8 num_of_parents; 
	UInt8 counter=OAM_ZERO; 

	if(OAM_NULL ==  p_input_inform_api)
	{
		num_of_parents = OAM_THREE;
	}
	else
	{
		num_of_parents = OAM_FOUR;
		notification_flag  = OAM_ONE;
	}
#ifndef OAM_SON_ENABLE
	num_of_parents -= OAM_TWO;
#else
	SInt8 cell_config_idx = OAM_ZERO;
#endif

	UInt16 param_len = OAM_ZERO;
	UInt8  temp_param[STR_LEN_128] = {OAM_ZERO}, str[OAM_FIVE] = {OAM_ZERO},index = OAM_ZERO ;
	char temp_str [OAM_TWENTY_FOUR];
	oam_memset(temp_str,'\0',OAM_TWENTY_FOUR);
	oam_inform_api_t *oam_inform_api[num_of_parents];
	oam_error_code_et err_code = NO_ERROR;


	OAM_FUN_ENTRY_TRACE();

	for( index=OAM_ZERO ; index < num_of_parents ; index ++ )
	{
		if(OAM_FAILURE == oam_mem_alloc(sizeof(oam_inform_api_t), (void**)&oam_inform_api[index],
					&err_code)) 
		{
			OAM_LOG(OAM, OAM_ERROR,
                    "Memory allocation to oam_inform_api failed"
                    " with Error Code:%d",
                    err_code);
			return OAM_FAILURE;
		}

	}
    /* SPR  14568 Fix Start */
    oam_strncpy((Char8*)oam_inform_api[counter]->parent_name,
    /* spr_21013_fix_start */
            "InternetGatewayDevice.Services.FAPService.FAPControl.LTE",
            sizeof("InternetGatewayDevice.Services.FAPService.FAPControl.LTE"));
    /* spr_21013_fix_end */
    /* SPR  14568 Fix End*/

	oam_inform_api[counter]->num_of_param = OAM_TWO;

	snprintf((char *)str,OAM_FIVE , "%d",oam_prov_req.system_status.op_state); 
	oam_strncpy((char *)temp_param,"OpState=" ,sizeof(temp_param ) );
	oam_strncat((char *)temp_param,(char *)str , sizeof(temp_param ) - OAM_ONE);
	param_len = oam_strlen((char *)temp_param);
	temp_param[param_len] = '\0';
	oam_strncpy((char *)oam_inform_api[counter]->param_list[OAM_ZERO],(char *)temp_param , sizeof(oam_inform_api[counter]->param_list[OAM_ZERO]));

	snprintf((char *)str,OAM_FIVE,"%d",oam_prov_req.system_status.rftx_status); 
	oam_strncpy((char *)temp_param,"RFTxStatus=" , sizeof(temp_param ));
	oam_strncat((char *)temp_param,(char *)str , sizeof(temp_param ) - OAM_ONE );
	param_len = oam_strlen((char *)temp_param);
	temp_param[param_len] = '\0';
	oam_strncpy((char *)oam_inform_api[counter]->param_list[OAM_ONE],(char *)temp_param , sizeof( oam_inform_api[counter]->param_list[OAM_ONE]) );
	counter++;
#ifdef OAM_SON_ENABLE
    /* spr_21013_fix_start */
	oam_strncpy((Char8*)oam_inform_api[counter]->parent_name,
			"InternetGatewayDevice.Services.FAPService.REM.LTE", 
			sizeof("InternetGatewayDevice.Services.FAPService.REM.LTE"));
    /* spr_21013_fix_end */

	oam_inform_api[counter]->num_of_param = OAM_ONE;
	oam_memset(oam_inform_api[counter]->param_list[OAM_ZERO], OAM_ZERO,
			sizeof(oam_inform_api[counter]->param_list[OAM_ZERO]));
	oam_memset(temp_str, OAM_ZERO, sizeof(temp_str));
	snprintf(temp_str, sizeof(temp_str), "%s",
			oam_prov_req.oam_igd_params.nw_scan_status.last_scan_time); 
	oam_strncpy(oam_inform_api[counter]->param_list[OAM_ZERO],
			"LastScanTime=",
			sizeof(oam_inform_api[counter]->param_list[0])  - 1);
	oam_strncat((Char8*)oam_inform_api[counter]->param_list[OAM_ZERO],
			(Char8*)oam_prov_req.oam_igd_params.nw_scan_status.last_scan_time,
			sizeof(oam_inform_api[counter]->param_list[0]) - oam_strlen("LastScanTime=") - 1);
	counter++;
    /* spr_21013_fix_start */
	oam_strncpy((Char8*)oam_inform_api[counter]->parent_name,
			"InternetGatewayDevice.Services.FAPService.CellConfig."
			"LTE.RAN.NeighborListInUse",
			sizeof("InternetGatewayDevice.Services.FAPService.CellConfig."
				"LTE.RAN.NeighborListInUse"));
    /* spr_21013_fix_end */

	oam_inform_api[counter]->num_of_param = OAM_ONE;
	oam_memset(oam_inform_api[counter]->param_list[OAM_ZERO], OAM_ZERO,
			sizeof(oam_inform_api[counter]->param_list[OAM_ZERO]));
	oam_memset(temp_str, OAM_ZERO, sizeof(temp_str));

	cell_config_idx = oam_get_son_neigh_list_in_use_idx(
			oam_prov_req.cell_cntxt.curr_cell_cid);
	if(OAM_ERROR_RETURN_VAL == cell_config_idx) {
		OAM_LOG(OAM, OAM_WARNING, "Invalid cell context[cid=%d]", oam_prov_req.cell_cntxt.curr_cell_cid);
		return OAM_FAILURE;
	}
	snprintf(temp_str, sizeof(temp_str), "%s",
			oam_prov_req.oam_son_req.neigh_list_in_use[cell_config_idx].last_nrt_upd_time); 
	oam_strncpy(oam_inform_api[counter]->param_list[0],
			"X_VENDOR_LastNRTUpdateTime=",
			sizeof(oam_inform_api[counter]->param_list[0]) - 1);
	oam_strncat((Char8*)oam_inform_api[counter]->param_list[0],
			oam_prov_req.oam_son_req.neigh_list_in_use[cell_config_idx].last_nrt_upd_time,
			sizeof(oam_inform_api[counter]->param_list[0]) - oam_strlen("X_VENDOR_LastNRTUpdateTime=") - 1);
	++counter;
#endif

	if(notification_flag)
	{
		oam_memcpy((Char8*)oam_inform_api[counter]->parent_name,
				p_input_inform_api->parent_name,
				sizeof(p_input_inform_api->parent_name));


		oam_inform_api[counter]->num_of_param =
			p_input_inform_api->num_of_param;

		oam_memcpy((Char8*)oam_inform_api[counter]->param_list,
				p_input_inform_api->param_list,
				sizeof(p_input_inform_api->param_list));
	}

	ret_val = oam_compose_and_send_inform_api((oam_inform_api_t **)&oam_inform_api,
			num_of_parents,(UInt8 *)"Value Change\0",dest_module_id , sub_module_id); 

	/*SPR 16027 Fixed Start */
	for( index=OAM_ZERO ; index < num_of_parents ; index ++ ){

		ret_val = oam_mem_free(oam_inform_api[index],&err_code);
		if (OAM_FAILURE == ret_val) {
			OAM_LOG(OAM, OAM_WARNING,
					"Error in memory free with error code: %d", err_code);
		}

	}
	/*SPR 16027 Fixed End*/

	OAM_FUN_EXIT_TRACE();
	return ret_val;
}


/******************************************************************************
 *   FUNCTION NAME   : oam_maintain_internal_tcb
 *   DESCRIPTION     : It is responsible to maintaining tcb entries and starting Module timmer
 *                     for Qcli command
 *   INPUT           : trans_id :transaction id
 *                     trans_type :transaction type
 *                     msg_size :message size
 *                     p_api_buf :Pointer to api buffer
 *   OUTPUT          : none
 *   RETURNS         : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/

oam_return_et 
oam_maintain_internal_tcb
(
 UInt16 trans_id,
 oam_trans_type_et trans_type,
 UInt16 msg_size,
 UInt8 *p_api_buf
 )
{
	OAM_FUN_ENTRY_TRACE();
	/* For calling any TCB related functions, trans_id must be of 4 bytes */
	UInt32 trans_id_new = trans_id;

	OAM_LOG(OAM,OAM_INFO,
			"trans_id Saved is: %d",trans_id_new);
	if(OAM_FAILURE==oam_tcb_insert(trans_id_new,OAM_ZERO,trans_type,msg_size,p_api_buf)) 
	{
		OAM_LOG(OAM,OAM_WARNING,
				"Unable to Maintain Transaction Detail in TCB");
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/******************************************************************************
 * Function Name  : oam_handle_notification_ind_resp 
 * Description    : This function handles notification response api from management interface 
 * Inputs         : p_buf : Pointer to message Buffer
 * Output         : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE  

 ******************************************************************************/
oam_return_et
oam_handle_notification_ind_resp
(
 void *p_buf 
 )
{
	UInt8 status = OAM_ZERO; 
	UInt16 error_code = OAM_ZERO;
	UInt16 counter = OAM_ZERO;
	UInt8 len_str = OAM_ZERO; 
	UInt8 err_string[STR_LEN_256]; 

	OAM_FUN_ENTRY_TRACE();


	UInt16 trans_id = OAM_ZERO;
	trans_id = get_word_from_header((UInt8*)(p_buf) + OAM_ZERO);
	/* For calling any TCB related functions, trans_id must be of 4 bytes */
	UInt32 trans_id_new = trans_id;

	oam_struct_t * p_node = oam_tcb_get_detail(&trans_id_new);
	if(!p_node)
	{
		OAM_LOG(OAM, OAM_WARNING, "Pointer to oam_struct returned : NULL");
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	if(p_node->bitmask == TCB_BITMASK_FM)
	{
		OAM_LOG(OAM, OAM_DETAILED,"Expect FM function");
	}
	else if(p_node->bitmask == TCB_BITMASK_SM)
	{
		OAM_LOG(OAM, OAM_DETAILED,"Expect SM function");
	}
	else
	{
		OAM_LOG(OAM, OAM_DETAILED,"Module not match");
	}


	p_buf += OAM_TR069_INTF_HEADER_SIZE;  

	oam_memcpy(&status,p_buf,sizeof(UInt8)); 
	counter += sizeof(UInt8);

	error_code = get_word_from_header((UInt8*)(p_buf) + OAM_ONE);   

	counter += sizeof(UInt16);

	counter += OAM_FOUR *(sizeof(UInt8)); 

	oam_memcpy(&len_str,p_buf+counter,sizeof(UInt8));    

	counter += sizeof(UInt8); 
	oam_memcpy(err_string,p_buf+counter,len_str); 

	OAM_LOG(OAM,OAM_INFO,"Status : %d. "
	    "Error Code : %d. "
	    "Error String : %s",
	    status,
	    error_code,
	    err_string); 

	oam_fm_handle_alarm_acknowledge_resp(trans_id_new); 

	/*SPR 16027 Fixed Start */
	oam_tcb_delete(&trans_id_new);
	/*SPR 16027 Fixed End */

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS; 

}

/****************************************************************************
 * Function Name  : oam_fill_common_resp_info
 * Description    : This function will return the repsonse of any request API
 * Inputs         : des_arr :pointer to des_arr
 *                  status :status 
 *                  error_string :Error String
 *                  ret_val_len :Pointer to UInt16
 * Outputs        : array of string conversion
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
oam_return_et
oam_fill_common_resp_info
(
 UInt8 *des_arr,
 UInt8 status,
 UInt8 error_string[],
 UInt16 *ret_val_len
 )
{
	OAM_FUN_ENTRY_TRACE();
	OAM_NULL_CHECK(OAM_NULL != ret_val_len);
	UInt8 curr_pos=OAM_ZERO, str_len=OAM_ZERO;
	if(des_arr==OAM_NULL)
		return OAM_FAILURE;		 

	des_arr[OAM_ZERO] = status & OAM_ALL_8_BIT_SET;
	curr_pos++;

	des_arr[OAM_ONE] = (OAM_ZERO & OAM_UPPER_EIGHT_BITS_SET) >> OAM_EIGHT;
	des_arr[OAM_TWO] =  OAM_ZERO & OAM_ALL_8_BIT_SET;
	curr_pos += OAM_TWO;

	des_arr[OAM_THREE] = OAM_ZERO;
	curr_pos++;

	des_arr[OAM_FOUR] = OAM_ZERO;
	des_arr[OAM_FIVE] = OAM_ZERO;
	des_arr[OAM_SIX] = OAM_ZERO;
	curr_pos += OAM_THREE;

	if(OAM_ZERO == oam_strlen((char *)error_string))
	{ 
		des_arr[OAM_SEVEN] = (oam_strlen((char *)error_string)) & OAM_ALL_8_BIT_SET;
		curr_pos += OAM_ONE;
	}
	else
	{
		str_len = oam_strlen((char *)error_string);
		error_string[str_len]='\0';
		str_len++;
		oam_memcpy(des_arr+OAM_SEVEN, (char *)error_string, str_len);
		curr_pos += str_len;
	} 
	*ret_val_len = curr_pos;

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}


/****************************************************************************
 * Function Name  : fill_parent_node_info
 * Inputs         : des_arr :Pointer to des arr
 *                  total_len_child_nodes :total length of child nodes
 *                  total_num_param :total number of params
 *                  par_name :parameter name
 *                  ret_val_len :Pointer to UInt16
 *                  start_index :start index
 *                  end_index :end index
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function fill the parent node info
 ****************************************************************************/

oam_return_et
fill_parent_node_info
(
 UInt8 *des_arr,
 UInt16 total_len_child_nodes,
 UInt8 total_num_param,
 UInt8 par_name[],
 UInt16 *ret_val_len , 
 UInt8 start_index ,
 UInt8 end_index
 )
{
	OAM_FUN_ENTRY_TRACE();
	OAM_ASSERT(OAM_NULL != des_arr);
	OAM_NULL_CHECK(OAM_NULL != ret_val_len); 
	UInt8 temp_arr[OAM_THOUSAND] = {OAM_ZERO};
	UInt16 par_len = OAM_ZERO;
	UInt16 parent_n_child_nodes_len  = OAM_ZERO;
	if(OAM_NULL==des_arr)
		return OAM_FAILURE;

	oam_sprintf((char *)temp_arr,"%s",par_name);

	par_len = OAM_TWO + OAM_ONE + OAM_ONE + OAM_TWO + OAM_TWO + oam_strlen((char *)temp_arr) + OAM_ONE;
	parent_n_child_nodes_len =  par_len + total_len_child_nodes;

	des_arr[OAM_ZERO] = (parent_n_child_nodes_len & OAM_UPPER_EIGHT_BITS_SET) >> OAM_EIGHT;
	des_arr[OAM_ONE] =  parent_n_child_nodes_len & OAM_ALL_8_BIT_SET;

	des_arr[OAM_TWO] =  total_num_param  & OAM_ALL_8_BIT_SET;

	des_arr[OAM_THREE] =  start_index & OAM_ALL_8_BIT_SET ;  
	des_arr[OAM_FOUR] =  end_index & OAM_ALL_8_BIT_SET ; 
	des_arr[OAM_FIVE] =  OAM_ZERO;

	des_arr[OAM_SIX] = ((oam_strlen((char *)temp_arr) + OAM_ONE) & OAM_UPPER_EIGHT_BITS_SET) >> OAM_EIGHT;
	des_arr[OAM_SEVEN] = ((oam_strlen((char *)temp_arr) + OAM_ONE) & OAM_ALL_8_BIT_SET);

	oam_sprintf((char *)&des_arr[OAM_EIGHT],"%s",par_name);

	*ret_val_len = par_len;
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}



/****************************************************************************
 * Function Name  : oam_fill_name_value_pair
 * Inputs         : des_arr :Pointer to des_arr
 *                  ret_val_len :Pointer to UInt16
 *                  p_child_node_name_value_pair :Pointer to child node name value
 *                  pair 
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function fills the name value pair
 ****************************************************************************/
oam_return_et
oam_fill_name_value_pair
(
 UInt8 *des_arr,
 UInt16 *ret_val_len,
 UInt8 *p_child_node_name_value_pair
 )
{
	UInt16  name_val_len = OAM_ZERO;
	OAM_FUN_ENTRY_TRACE();
	OAM_ASSERT(OAM_NULL != des_arr);
	OAM_NULL_CHECK(OAM_NULL != ret_val_len);
	OAM_NULL_CHECK(OAM_NULL != p_child_node_name_value_pair);
	if(OAM_NULL==des_arr)
		return OAM_FAILURE;

	name_val_len  =  OAM_TWO + OAM_TWO + oam_strlen((char *)p_child_node_name_value_pair) + OAM_ONE;

	des_arr[OAM_ZERO] = (name_val_len & OAM_UPPER_EIGHT_BITS_SET) >> OAM_EIGHT;
	des_arr[OAM_ONE] =  name_val_len & OAM_ALL_8_BIT_SET;

	des_arr[OAM_TWO]= OAM_ZERO;
	des_arr[OAM_THREE]= OAM_ZERO;

	oam_strncpy((char *)&des_arr[OAM_FOUR],(char *)p_child_node_name_value_pair , oam_strlen((char *)p_child_node_name_value_pair));

	*ret_val_len = name_val_len;

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}


/****************************************************************************
 * Function Name  : cal_len_of_common_n_parent_node_info
 * Inputs         : p_parent_node_name :Pointer to parent node name
 * Outputs        : None
 * Returns        : Length of common and parent node info
 * Description    : This function calculates the length of common and parent node info
 ****************************************************************************/

UInt16
cal_len_of_common_n_parent_node_info
(
 UInt8  *p_parent_node_name
 )
{
	UInt16 common_info_length  = OAM_ZERO;
	UInt16 parent_info_length  = OAM_ZERO;
	common_info_length = OAM_ONE + OAM_TWO + OAM_ONE + OAM_THREE + OAM_ONE;

	parent_info_length = OAM_TWO + OAM_ONE + OAM_ONE +OAM_TWO + OAM_TWO + oam_strlen((char *)p_parent_node_name) + OAM_ONE;

	return (common_info_length + parent_info_length);
}


/****************************************************************************
 * Function Name  : cal_len_of_child_nodes
 * Inputs         : p_child_node_name_value_pair :Pointer to array of child node
 *                  name value pair 
 *                  no_of_parameter :numbers of parameters
 * Outputs        : None
 * Returns        : Length of child nodes
 * Description    : This function calculates the length of child nodes
 ****************************************************************************/
UInt16
cal_len_of_child_nodes
(
 UInt8 *p_child_node_name_value_pair[255],
 UInt8 no_of_parameter
 )
{
	UInt16 child_node_length = OAM_ZERO;

	oam_counter_t loop_counter = OAM_ZERO;
	for(loop_counter = OAM_ZERO;loop_counter<no_of_parameter;loop_counter++)
	{
		OAM_ASSERT(OAM_NULL != p_child_node_name_value_pair[loop_counter]);
		child_node_length += oam_strlen((char *)p_child_node_name_value_pair[loop_counter]);
		child_node_length +=OAM_ONE;
	}
	return child_node_length;
}


/******************************************************************************
 * Function Name  : oam_construct_n_send_gpv_response
 * Inputs         : p_parent_node_name :Pointer to parent node name
 *                : p_child_node_name_value_pair :Pointer to child node name
 *                  value pair
 *                : no_of_parameter :number of parameters
 *                : transaction_id :transaction id
 *                : p_error_code : Pointer to Error code
 *                : des_id : dest id
 *                : start index :start index
 *                : end index :end index
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : this function handles the GET param response 
 *******************************************************************************/

oam_return_et
oam_construct_n_send_gpv_response
(
 UInt8 *p_parent_node_name,    
 UInt8 *p_child_node_name_value_pair[255],
 UInt8 no_of_parameter,
 UInt16 transaction_id,
 oam_error_code_et *p_error_code,
 UInt16 des_id , 
 UInt8 start_index ,
 UInt8 end_index
 )
{
	oam_counter_t loop_counter   = OAM_ZERO;
	UInt8 *p_msg;
	UInt8 *p_api_buf;
	UInt16 api_buf_len;
	UInt16 com_par_len;
	UInt16 ret_len = OAM_ZERO;  
	UInt16 length_of_child_parameters = OAM_ZERO;  

	com_par_len = cal_len_of_common_n_parent_node_info(p_parent_node_name);
	length_of_child_parameters = cal_len_of_child_nodes(p_child_node_name_value_pair,no_of_parameter);
	length_of_child_parameters = length_of_child_parameters + (no_of_parameter * OAM_FOUR);

	api_buf_len = length_of_child_parameters + com_par_len + (OAM_CSPL_HDR_SIZE) + (OAM_MGMT_INTF_HEADER_SIZE);

	if (OAM_FAILURE == oam_mem_alloc(api_buf_len,(void *)&p_msg,p_error_code))
	{    
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		/* Not Enough Memory */
		return OAM_FAILURE;
	}
	p_api_buf = p_msg;
	oam_memset( p_msg, OAM_ZERO , api_buf_len);

	oam_construct_api_header(p_api_buf, OAM_VERSION_ID,OAM_MODULE_ID,
			des_id,OAM_GET_PARAMETER_VALUES_RESP,
			api_buf_len);

	p_api_buf = p_api_buf + OAM_CSPL_HDR_SIZE;

	oam_construct_mgmt_api_hdr(p_api_buf,transaction_id,OAM_MODULE_ID,
			des_id,OAM_GET_PARAMETER_VALUES_RESP,
			api_buf_len - (OAM_CSPL_HDR_SIZE),OAM_ZERO);
            /*SPR 17777 fix*/

	p_api_buf = p_api_buf  + OAM_MGMT_INTF_HEADER_SIZE; 

	/* encode common response information*/
	oam_fill_common_resp_info(p_api_buf,OAM_ZERO,(UInt8 *)"",&ret_len);
	p_api_buf = p_api_buf + ret_len;
	ret_len = OAM_ZERO; 

	/* encode parent information*/ 
	fill_parent_node_info(p_api_buf,length_of_child_parameters,
			no_of_parameter,p_parent_node_name,&ret_len , start_index ,end_index );
	p_api_buf = p_api_buf + ret_len; 
	ret_len = OAM_ZERO; 

	for(loop_counter = OAM_ZERO;loop_counter<no_of_parameter;loop_counter++)
	{
		oam_fill_name_value_pair(p_api_buf,&ret_len,(UInt8*)p_child_node_name_value_pair[loop_counter]);  
		p_api_buf+=ret_len;  
	}
	if(OAM_SUCCESS == oam_send_message(p_msg,des_id,p_error_code))
	{
		OAM_LOG(OAM,OAM_DETAILED,"Sending message");
	}
	else
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in Sending message with error code %d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_populate_plmnid
 * Inputs         : temp :Pointer to Char8
 *                  plmnid :Pointer to void
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function populates the plmnid
 ****************************************************************************/
	oam_return_et
oam_populate_plmnid(Char8 *temp, void *p_plmnid)
{
	OAM_FUN_ENTRY_TRACE();
	rrm_oam_cell_plmn_info_t *plmnid = p_plmnid;
	Char8 new_plmn[OAM_SEVEN] = {OAM_ZERO};
	Char8 final_plmn[OAM_TWO] = {OAM_ZERO};
	UInt16 length = OAM_ZERO;
	oam_return_et retVal = OAM_FAILURE;

	if(OAM_NULL == temp || OAM_NULL == plmnid) {
		OAM_LOG(OAM, OAM_WARNING, "Null pointer passed.");
		OAM_FUN_EXIT_TRACE();
		return retVal;
	}

	OAM_LOG(OAM, OAM_DETAILED,
			" {global}Content of node is PLMN ID and parameter name is %s",temp);

	length = oam_strlen(temp);

	if(OAM_SIX == length) {
		oam_memcpy(new_plmn, temp, OAM_SIX);
	}
	else
	{
		OAM_LOG(OAM, OAM_DETAILED,
				" Enter Valid PLMN ID");
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	snprintf(final_plmn, OAM_ONE + sizeof(new_plmn[OAM_ZERO]), "%c", new_plmn[OAM_ZERO]);
	plmnid->mcc[OAM_ONE] = oam_atoi(final_plmn);


	snprintf(final_plmn, OAM_ONE + sizeof(new_plmn[OAM_ONE]), "%c", new_plmn[OAM_ONE]);
	plmnid->mcc[OAM_ZERO] = oam_atoi(final_plmn);

	snprintf(final_plmn, OAM_ONE + sizeof(new_plmn[OAM_THREE]), "%c", new_plmn[OAM_THREE]);
	plmnid->mcc[OAM_TWO] = oam_atoi(final_plmn);


	snprintf(final_plmn, OAM_ONE + sizeof(new_plmn[OAM_TWO]), "%c", new_plmn[OAM_TWO]);
	if ((new_plmn[OAM_TWO] == 'f') || new_plmn[OAM_TWO] == 'F')
	{

		snprintf(final_plmn, OAM_ONE + sizeof(new_plmn[OAM_FOUR]), "%c",
				new_plmn[OAM_FOUR]);
		plmnid->mnc[OAM_ONE] = oam_atoi(final_plmn);

		snprintf(final_plmn, OAM_ONE + sizeof(new_plmn[OAM_FIVE]), "%c",
				new_plmn[OAM_FIVE]);
		plmnid->mnc[OAM_ZERO] = oam_atoi(final_plmn);
	}
	else
	{
		plmnid->mnc[OAM_ZERO] = oam_atoi(final_plmn);

		snprintf(final_plmn, OAM_ONE + sizeof(new_plmn[OAM_FOUR]), "%c",
				new_plmn[OAM_FOUR]);

		plmnid->mnc[OAM_TWO] = oam_atoi(final_plmn);

		snprintf(final_plmn, OAM_ONE + sizeof(new_plmn[OAM_FIVE]), "%c",
				new_plmn[OAM_FIVE]);

		plmnid->mnc[OAM_ONE] = oam_atoi(final_plmn);
	}

	if (new_plmn[OAM_TWO] == 'f' || new_plmn[OAM_TWO] == 'F') {
		plmnid->num_mnc_digit = MIN_NUM_MNC_DIGIT;
	}
	else {
		plmnid->num_mnc_digit = MAX_NUM_MNC_DIGIT;
	}




	retVal = OAM_SUCCESS;
	OAM_FUN_EXIT_TRACE();
	return retVal;
}
/*SPR 11315 FIX START*/

/****************************************************************************
 * Function Name  : oam_generate_plmnid
 * Inputs         : out_plmn :Pointer to out plmn
 *                  input_plmn :Pointer to void
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function generates the plmnid
 *****************************************************************************/
	oam_return_et
oam_generate_plmnid(Char8 *out_plmn, void *p_input_plmn)
{
	OAM_FUN_ENTRY_TRACE();
	Char8  temp[OAM_SIX]={OAM_ZERO};
	rrm_oam_cell_plmn_info_t *input_plmn = p_input_plmn;


	if(OAM_TWO == input_plmn->num_mnc_digit)
	{
		temp[OAM_ZERO] = input_plmn->mcc[OAM_ZERO];
		temp[OAM_ONE] = input_plmn->mcc[OAM_ONE];
		temp[OAM_TWO] = input_plmn->mcc[OAM_TWO];
		temp[OAM_THREE] = input_plmn->mnc[OAM_ZERO];
		temp[OAM_FOUR] = input_plmn->mnc[OAM_ONE];
		temp[OAM_FIVE] = '\0';
	}
	else
	{
		temp[OAM_ZERO] = input_plmn->mcc[OAM_ZERO];
		temp[OAM_ONE] = input_plmn->mcc[OAM_ONE];
		temp[OAM_TWO] = input_plmn->mcc[OAM_TWO]; 
		temp[OAM_THREE] = input_plmn->mnc[OAM_ZERO];
		temp[OAM_FOUR] = input_plmn->mnc[OAM_ONE];
		temp[OAM_FIVE] = input_plmn->mnc[OAM_TWO];

	}

	if(OAM_THREE == input_plmn->num_mnc_digit)
		snprintf((char *)out_plmn,OAM_ONE + OAM_SIX,"%d%d%d%d%d%d",temp[OAM_ZERO],temp[OAM_ONE],temp[OAM_TWO],temp[OAM_THREE],temp[OAM_FOUR],temp[OAM_FIVE]);
	else
		/* SPR-10788 Fix Start */
		snprintf((char *)out_plmn,OAM_ONE + OAM_FIVE,"%d%d%d%d%d",temp[OAM_ZERO],temp[OAM_ONE],temp[OAM_TWO],temp[OAM_THREE],temp[OAM_FOUR]);
	/* SPR-10788 Fix End */

	OAM_FUN_EXIT_TRACE();

	return OAM_SUCCESS;
}
/*SPR 11315 FIX END*/
/* SPR 12175 FIX START */
/****************************************************************************
 * Function Name  : oam_conversion_from_int64_to_byte_string
 * Inputs         : dest :Pointer to dest
 *                  source :Source
 * Outputs        : None
 * Returns        : None
 * Description    : This function converts int into byte string
 ****************************************************************************/
void 
oam_conversion_from_int64_to_byte_string(
		UInt8* dest, 
		UInt64 source,
		UInt8 bitstring_size
		)
{
	if (OAM_NULL == dest)
	{
		OAM_LOG(OAM, OAM_WARNING, "Null Argument Passed!!");
		return;
	}
	source = (source << (OAM_MAX_CDMA_1XRTT_INT_BITSTRING_SIZE - bitstring_size));
	dest[OAM_FIVE] = (UInt8) (source & 0x000000FF);
	dest[OAM_FOUR] = (UInt8) ((source >> OAM_EIGHT)& 0x000000FF);
	dest[OAM_THREE] = (UInt8) ((source >> OAM_SIXTEEN)& 0x000000FF);
	dest[OAM_TWO] = (UInt8) ((source >> OAM_TWENTY_FOUR)& 0x000000FF);
	dest[OAM_ONE] = (UInt8) ((source >> OAM_THIRTY_TWO)& 0x000000FF);
	dest[OAM_ZERO] = (UInt8) ((source >> OAM_FOURTY)& 0x000000FF);
}
/* SPR 12175 FIX END */
/* SPR 13283  START */
/****************************************************************************
 * Function Name  : oam_conversion_from_int128_to_byte_string
 * Inputs         : dest :Pointer to dest
 *                  source :Source
 * Outputs        : None
 * Returns        : None
 * Description    : This function converts int into byte string
 ****************************************************************************/
void 
oam_conversion_from_int128_to_byte_string(
		UInt8* dest, 
		UInt64 source,
		UInt8 bitstring_size
		)
{
	if (OAM_NULL == dest)
	{
		OAM_LOG(OAM, OAM_WARNING, "Null Argument Passed!!");
		return;
	}
	source = (source << (OAM_MAX_CDMA_HRPD_INT_BITSTRING_SIZE - bitstring_size));
	dest[OAM_FIFTEEN] = (UInt8) (source  & 0x000000FF);
	dest[OAM_FOURTEEN] = (UInt8) ((source >> OAM_EIGHT) & 0x000000FF);
	dest[OAM_THIRTEEN] = (UInt8) ((source >> OAM_SIXTEEN) & 0x000000FF);
	dest[OAM_TWELVE] = (UInt8) ((source  >> OAM_TWENTY_FOUR) & 0x000000FF);
	dest[OAM_ELEVEN] = (UInt8) ((source >> OAM_THIRTY_TWO) & 0x000000FF);
	dest[OAM_TEN] = (UInt8) ((source >> OAM_FOURTY) & 0x000000FF);
	dest[OAM_NINE] = (UInt8) ((source >> OAM_FOURTY_EIGHT) & 0x000000FF);
	dest[OAM_EIGHT] = (UInt8) ((source >> OAM_FIFTY_SIX) & 0x000000FF);
}
/* SPR 13283 END */

/*SPR 11650 fix */
/****************************************************************************
 * Function Name  : oam_conversion_from_macro_enb_to_byte_string
 * Inputs         : dest :Pointer to dest
 *                  source :Source
 * Outputs        : None
 * Returns        : None
 * Description    : This function converts int into byte string
 ****************************************************************************/
void 
oam_conversion_from_macro_enb_to_byte_string(
		UInt8* dest, 
		UInt32 source,
		UInt8 bitstring_size
		)
{
	if (OAM_NULL == dest)
	{
		OAM_LOG(OAM, OAM_WARNING, "Null Argument Passed!!");
		return;
	}
	source = (source << (bitstring_size));
	dest[OAM_THREE] = (UInt8) (source & 0x000000FF);
	dest[OAM_TWO] = (UInt8) ((source >> OAM_EIGHT)& 0x000000FF);
	dest[OAM_ONE] = (UInt8) ((source >> OAM_SIXTEEN)& 0x000000FF);
	dest[OAM_ZERO] = (UInt8) ((source >> OAM_TWENTY_FOUR)& 0x000000FF);
}

/*SPR 11650 fix */



/* SPR 11650 FIX START */
/****************************************************************************
 * Function Name  : oam_conversion_from_int_to_byte_string
 * Inputs         : dest :Pointer to dest
 *                  source :Source
 * Outputs        : None
 * Returns        : None
 * Description    : This function converts int into byte string
 ****************************************************************************/
void 
oam_conversion_from_int_to_byte_string(
		UInt8* dest, 
		UInt32 source,
		UInt8 bitstring_size
		)
{
	if (OAM_NULL == dest)
	{
		OAM_LOG(OAM, OAM_WARNING, "Null Argument Passed!!");
		return;
	}
	source = (source << (OAM_MAX_INT_BITSTRING_SIZE - bitstring_size));
	dest[OAM_THREE] = (UInt8) (source & 0x000000FF);
	dest[OAM_TWO] = (UInt8) ((source >> OAM_EIGHT)& 0x000000FF);
	dest[OAM_ONE] = (UInt8) ((source >> OAM_SIXTEEN)& 0x000000FF);
	dest[OAM_ZERO] = (UInt8) ((source >> OAM_TWENTY_FOUR)& 0x000000FF);
}
/* BITSTRING FIX START */
/****************************************************************************
 * Function Name  : oam_conversion_from_short_to_byte_string
 * Inputs         : dest :Pointer to dest
 *                  source :Source
 * Outputs        : None
 * Returns        : None
 * Description    : This function converts short into byte string
 ****************************************************************************/
void
oam_conversion_from_short_to_byte_string(
		UInt8* dest,
		UInt32 source,
		UInt8 bitstring_size
		)
{
	if (OAM_NULL == dest)
	{
		OAM_LOG(OAM, OAM_WARNING, "Null Argument Passed!!");
		return;
	}
	source = (source << (OAM_MAX_SHORT_BITSTRING_SIZE - bitstring_size));
	dest[OAM_ONE]  = (UInt8) (source & 0x000000FF) ;
	dest[OAM_ZERO] = (UInt8) ((source >> OAM_EIGHT)& 0x000000FF);
}
/* BITSTRING FIX END */
/****************************************************************************
 * Function Name  : oam_conversion_for_macro_enb_from_int_to_byte_string
 * Inputs         : dest :Pointer to dest
 *                  source :Source
 * Outputs        : None
 * Returns        : None
 * Description    : This function converts int into byte string
 ****************************************************************************/
void 
oam_conversion_for_macro_enb_from_int_to_byte_string(
		UInt8* dest, 
		UInt32 source,
		UInt8 bitstring_size
		)
{
	if (OAM_NULL == dest)
	{
		OAM_LOG(OAM, OAM_WARNING, "Null Argument Passed!!");
		return;
	}
	source = (source << (OAM_MAX_INT_BITSTRING_SIZE - bitstring_size));
	dest[OAM_TWO] = (UInt8) ((source >> OAM_EIGHT)& 0x000000F0);
	dest[OAM_ONE] = (UInt8) ((source >> OAM_SIXTEEN)& 0x000000FF);
	dest[OAM_ZERO] = (UInt8) ((source >> OAM_TWENTY_FOUR)& 0x000000FF);
}


/* SPR 11650 FIX END */


/****************************************************************************
 * Function Name  : oam_convert_int_to_byte_string
 * Inputs         : dest :Pointer to dest
 *                  source :Source
 * Outputs        : None
 * Returns        : None
 * Description    : This function converts int into byte string
 ****************************************************************************/
void 
oam_convert_int_to_byte_string(
		UInt8* dest, 
		UInt32 source
		)
{
	if (OAM_NULL == dest)
	{
		OAM_LOG(OAM, OAM_WARNING, "Null Argument Passed!!");
		return;
	}
	dest[OAM_ZERO] = (UInt8) (source & 0x000000FF);
	dest[OAM_ONE] = (UInt8) ((source >> OAM_EIGHT)& 0x000000FF);
	dest[OAM_TWO] = (UInt8) ((source >> OAM_SIXTEEN)& 0x000000FF);
	dest[OAM_THREE] = (UInt8) ((source >> OAM_TWENTY_FOUR)& 0x000000FF);
}

/* BITSTRING FIX START */
/****************************************************************************
 * Function Name  : oam_conversion_from_char_to_byte_string
 * Inputs         : dest :Pointer to dest
 *                  source :Source
 * Outputs        : None
 * Returns        : None
 * Description    : This function converts char into byte string
 ****************************************************************************/
void
oam_conversion_from_char_to_byte_string(
		UInt8* dest,
		UInt32 source,
		UInt8 bitstring_size
		)
{
	source = (source << (OAM_MAX_CHAR_BITSTRING_SIZE - bitstring_size));
	dest[OAM_ZERO]  = (UInt8) (source & 0x000000FF) ;
}
/* BITSTRING FIX END */

/****************************************************************************
 * Function Name  : oam_convert_short_to_byte_string
 * Inputs         : dest :Pointer to dest
 *                  source :Source
 * Outputs        : None
 * Returns        : None
 * Description    : This function converts short into byte string
 ****************************************************************************/
void 
oam_convert_short_to_byte_string(
		UInt8* dest, 
		UInt32 source
		)
{
	if (OAM_NULL == dest)
	{
		OAM_LOG(OAM, OAM_WARNING, "Null Argument Passed!!");
		return;
	}
	dest[OAM_ZERO] = (UInt8) (source & 0x000000FF);
	dest[OAM_ONE] = (UInt8) ((source >> OAM_EIGHT)& 0x000000FF);
}
/*SPR 11315 FIX START*/
/****************************************************************************
 * Function Name  : oam_conversion_bitstring_short_to_byte_string
 * Inputs         : dest :Pointer to dest
 *                  source :Source
 * Outputs        : None
 * Returns        : None
 * Description    : This function converts short into byte string
 ****************************************************************************/
void
oam_conversion_bitstring_short_to_byte_string(
		UInt8* dest,
		UInt32 source
		)
{
	if (OAM_NULL == dest)
	{
		OAM_LOG(OAM, OAM_WARNING, "Null Argument Passed!!");
		return;
	}
	dest[OAM_ONE] = (UInt8) (source & 0x000000FF);
	dest[OAM_ZERO] = (UInt8) ((source >> OAM_EIGHT)& 0x000000FF);
}
/*SPR 11315 FIX END*/



/****************************************************************************
 * Function Name  : oam_populate_rrm_plmnid
 * Inputs         : input_plmn :Pointer to input plmn
 *                  out_plmn :Pointer to rrm_oam_cell_plmn_info_t
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function populates the rrm plmnid
 ****************************************************************************/

oam_return_et
oam_populate_rrm_plmnid(
		Char8 *input_plmn,
		rrm_oam_cell_plmn_info_t *out_plmn)
{
	OAM_FUN_ENTRY_TRACE();
	Char8 new_plmn[OAM_SEVEN],final_plmn[OAM_TWO];
	UInt16 length = OAM_ZERO;

	oam_memset(new_plmn,OAM_ZERO,OAM_SEVEN);
	oam_memset(final_plmn,OAM_ZERO,OAM_TWO);

	length = oam_strlen(input_plmn);

	if (OAM_SIX == length)
	{
		oam_memcpy(new_plmn,input_plmn,OAM_SIX);
	}
	else if(OAM_FIVE == length)
	{
		new_plmn[OAM_ZERO] = input_plmn[OAM_ZERO];
		new_plmn[OAM_ONE] = input_plmn[OAM_ONE];
		new_plmn[OAM_TWO] = 'f';
		new_plmn[OAM_THREE] = input_plmn[OAM_TWO];
		new_plmn[OAM_FOUR] = input_plmn[OAM_THREE];
		new_plmn[OAM_FIVE] = input_plmn[OAM_FOUR];
		new_plmn[OAM_SIX] =  '\0';
	}
	else
	{
		OAM_LOG(OAM, OAM_WARNING," Enter Valid PLMNID");
		return OAM_FAILURE;
	}

	snprintf(final_plmn,OAM_ONE +sizeof(new_plmn[OAM_ZERO]),"%c",new_plmn[OAM_ZERO]);
	out_plmn->mcc[OAM_ONE] = oam_atoi(final_plmn);

	snprintf(final_plmn,OAM_ONE +sizeof(new_plmn[OAM_ONE]),"%c",new_plmn[OAM_ONE]);
	out_plmn->mcc[OAM_ZERO] = oam_atoi(final_plmn);

	if (new_plmn[OAM_TWO] == 'f')
	{
		out_plmn->mnc[OAM_TWO] = OAM_ZERO;
		out_plmn->num_mnc_digit = MIN_NUM_MNC_DIGIT;
	}
	else
	{
		out_plmn->num_mnc_digit = MAX_NUM_MNC_DIGIT;
		snprintf(final_plmn,OAM_ONE +sizeof(new_plmn[OAM_TWO]),"%c",new_plmn[OAM_TWO]);
		out_plmn->mnc[OAM_TWO] = oam_atoi(final_plmn);
	}

	snprintf(final_plmn,OAM_ONE +sizeof(new_plmn[OAM_THREE]),"%c",new_plmn[OAM_THREE]);
	out_plmn->mcc[OAM_TWO] = oam_atoi(final_plmn);

	snprintf(final_plmn,OAM_ONE +sizeof(new_plmn[OAM_FOUR]),"%c",new_plmn[OAM_FOUR]);
	out_plmn->mnc[OAM_ONE] = oam_atoi(final_plmn);

	snprintf(final_plmn,OAM_ONE +sizeof(new_plmn[OAM_FIVE]),"%c",new_plmn[OAM_FIVE]);
	out_plmn->mnc[OAM_ZERO] = oam_atoi(final_plmn);

	oam_memset(new_plmn,OAM_ZERO,OAM_SEVEN);
	oam_memset(final_plmn,OAM_ZERO, OAM_TWO);
	/*  Coverity ID : 79062 Fix Start */
	/*  length = OAM_ZERO;  */
	/*  Coverity ID : 79062 Fix End */

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}



/****************************************************************************
 * Function Name  : oam_stop_timer_and_delete_tcb_entry
 * Inputs         : p_api_buf :Pointer to api buffer
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function is used to stop the timer and delete tcb entry
 ****************************************************************************/
oam_return_et 
oam_stop_timer_and_delete_tcb_entry(
		UInt8* p_api_buf
		)
{
	/*-------------Deleting Entry In TCB --------------*/
	UInt32 transaction_id;

	OAM_FUN_ENTRY_TRACE();

	transaction_id = get_word_from_header((UInt8*)(p_api_buf) + OAM_ZERO);
	oam_struct_t *node;
	node = oam_tcb_get_detail(&transaction_id);
	if(node)
	{
		if(node->timer_id!=OAM_ZERO)
		{
			if(qvTimerRunning((QTIMER)&(node->timer_id))==OAM_ONE)
			{
				OAM_LOG(OAM,OAM_DETAILED,"Stopping Timer");
				if (OAM_SUCCESS != oam_stop_timer(node->timer_id))
				{
					OAM_LOG(OAM, OAM_WARNING, "Error returned while stopping the timer");
					return OAM_FAILURE;
				}
				else
				{
					OAM_LOG(OAM, OAM_INFO, "Cell Setup Timer Stopped");
				}
			}
			else
			{
				OAM_LOG(OAM,OAM_WARNING,"Trying to stop inavlid timer");
			}
		}
	}
	oam_tcb_delete(&transaction_id);
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
	/*-------------Deleting Entry In TCB --------------*/

}

/****************************************************************************
 * Function Name  : oam_process_tcb_for_layer_resp
 * Inputs         : p_api_buf :Pointer to api buffer
 * Outputs        : tid :Transaction id	fetched from TCB
 *                  dest_id: Destination id fetched from TCB
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function is used to stop the timer, delete tcb entry
 *                  and get transaction/destination id.
 ****************************************************************************/

oam_return_et
oam_process_tcb_for_layer_resp
(
 UInt8* p_api_buf,
 UInt16 *tid,
 UInt16 *dest_id
 )
{
    /* SPR 20430 Changes Start*/
#if defined(__x86_64__)
    ULong32 transaction_id; 
#else
	UInt32 transaction_id;
#endif
    /* SPR 40430 Changes End*/
	oam_return_et retVal = OAM_SUCCESS;
	oam_struct_t *node;
	UInt8 *p_req_msg;

	OAM_FUN_ENTRY_TRACE();

	transaction_id = get_word_from_header(p_api_buf);

	OAM_LOG(OAM, OAM_INFO,
			"trans_id received from lower layer: %d", transaction_id);
	node = oam_tcb_get_detail(&transaction_id);
	if(node) {
		if(node->timer_id) {
			p_req_msg = node->oam_tcb_context;
			*tid = get_word_from_header(p_req_msg);
			*dest_id = get_word_from_header(p_req_msg +
					OAM_INTF_HDR_SRC_ID_OFFSET);

			OAM_LOG(OAM, OAM_INFO,
					"trans_id[%u] and Dest Id[%u] of request message in TCB", 
					*tid, *dest_id);

			if(qvTimerRunning(node->timer_id)) {
				OAM_LOG(OAM, OAM_DETAILED, "Stopping Timer");
				retVal = oam_stop_timer(node->timer_id);
				if (OAM_SUCCESS != retVal) {
					OAM_LOG(OAM, OAM_WARNING,
							"Error returned while stopping the timer");
				}
				else {
					OAM_LOG(OAM, OAM_INFO, "Timer Stopped");
				}
			}
			else {
				OAM_LOG(OAM, OAM_INFO,
						"Timer not running for the response");
			}
		}

		oam_tcb_delete(&transaction_id);
	}
	else {
		OAM_LOG(OAM, OAM_WARNING,
				"Node fetched from TCB is NULL, Returning Failure");
		retVal = OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return retVal;
}

/****************************************************************************
 * Function Name  : oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id
 * Inputs         : trans_id :Transaction id as received from lower layer
 * Outputs        : tid :Transaction id	fetched from TCB
 *                  dest_id: Destination id fetched from TCB
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function is used to stop the timer, delete tcb entry
 *                  and get transaction/destination id.
 ****************************************************************************/

oam_return_et
oam_stop_timer_and_delete_tcb_entry_get_tid_and_dest_id_via_trans_id(
		UInt16 trans_id,
		UInt16 *tid,
		UInt16 *dest_id
		)
{
/* SPR 20430 Changes Start*/
#if defined(__x86_64__)
    ULong32 transaction_id;   
#else
	UInt32 transaction_id;
#endif

/* SPR 20430 Changes End*/
	OAM_FUN_ENTRY_TRACE();

	transaction_id = trans_id;
	oam_struct_t *node;
	UInt8 *p_req_msg;

	OAM_LOG(OAM, OAM_INFO, "trans_id received from lower layer:%d",transaction_id);
	node = oam_tcb_get_detail(&transaction_id);
	if(node)
	{
		if(node->timer_id!=OAM_ZERO)
		{
			p_req_msg = node->oam_tcb_context;
			*tid = get_word_from_header((UInt8 *)p_req_msg);
			*dest_id = get_word_from_header((UInt8*)(p_req_msg) + OAM_TWO);

			OAM_LOG(OAM, OAM_INFO, "trans_id[%u] and Dest Id[%u] of request "
                "message in TCB:%d",*tid, *dest_id);

			if(qvTimerRunning((QTIMER)&(node->timer_id))==OAM_ONE)
			{
				OAM_LOG(OAM,OAM_DETAILED,"Stopping Timer");
				if (OAM_SUCCESS != oam_stop_timer(node->timer_id))
				{
					OAM_LOG(OAM, OAM_WARNING, "Error returned while stopping the timer");
					OAM_FUN_EXIT_TRACE();
					return OAM_FAILURE;
				}
				else
				{
					OAM_LOG(OAM, OAM_INFO, "Timer Stopped");
				}
			}
			else
			{
				OAM_LOG(OAM,OAM_WARNING,"Trying to stop inavlid timer");
			}
		}
	}
	else
	{
		OAM_FUN_EXIT_TRACE();
		OAM_LOG(OAM, OAM_WARNING, "Node fetched from TCB is NULL, Returning Failure");
		return OAM_FAILURE;
	}
	oam_tcb_delete(&transaction_id);
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;

}
/****************************************************************************
 * Function Name  : oam_maintain_system_state
 * Inputs         : oam_event
 * Outputs        : None
 *                  
 * Returns        : None
 * Description    : This function is used to stop the timer, delete tcb entry
 *                  and get transaction/destination id.
 ****************************************************************************/

void oam_maintain_system_state(oam_system_state_event_et oam_event)
{
	switch(oam_event)
	{
		case OAM_INITIALIZATION_ONGOING:
			{
				oam_prov_req.system_status.admin_state = ENODEB_ADMIN_STATE_LOCKED;   
				oam_prov_req.system_status.op_state = ENODEB_OP_STATE_DISABLED;   
				oam_prov_req.system_status.availability_status = ENODEB_AVAIL_STATE_INITIALIZING;   
				oam_prov_req.system_status.rftx_status = OAM_FALSE;   
				break;
			}
		case OAM_INITIALIZATION_COMPLETE:
			{
				oam_prov_req.system_status.availability_status = ENODEB_AVAIL_STATE_AVAILABLE;
				oam_prov_req.system_status.rftx_status = OAM_FALSE;   
				break;
			}
		case OAM_CHANGE_ADMIN_TO_UNLOCK:
			{
				break;
			}
		case OAM_CHANGE_ADMIN_TO_LOCK:
			{
				oam_prov_req.system_status.op_state = ENODEB_OP_STATE_DISABLED;   
				oam_prov_req.system_status.availability_status = ENODEB_AVAIL_STATE_FAILED;
				break;
			}
		case OAM_CHANGE_SYSTEM_STATE_FOR_MAJOR_ALARM:
			{
				if (oam_prov_req.system_status.availability_status != ENODEB_AVAIL_STATE_DEGRADED)
				{
					oam_prov_req.system_status.availability_status_bak = 
						oam_prov_req.system_status.availability_status;
				}
				oam_prov_req.system_status.availability_status = ENODEB_AVAIL_STATE_DEGRADED;
				break;
			}
		case OAM_CHANGE_SYSTEM_STATE_FOR_CRITICAL_ALARM:
			{
				oam_prov_req.system_status.op_state = ENODEB_OP_STATE_DISABLED;   
				if (oam_prov_req.system_status.availability_status != ENODEB_AVAIL_STATE_FAILED)
				{
					oam_prov_req.system_status.availability_status_bak = 
						oam_prov_req.system_status.availability_status;
				}
				oam_prov_req.system_status.availability_status = ENODEB_AVAIL_STATE_FAILED;
				break;
			}
		case OAM_CHANGE_SYSTEM_STATE_FOR_CLEAR_MAJOR_ALARM:
			{
				oam_prov_req.system_status.availability_status = 
					oam_prov_req.system_status.availability_status_bak;
				break;
			}
		case OAM_CHANGE_SYSTEM_STATE_FOR_CLEAR_CRITICAL_ALARM:
			{
				oam_prov_req.system_status.op_state = ENODEB_OP_STATE_ENABLED;   
				oam_prov_req.system_status.availability_status = 
					oam_prov_req.system_status.availability_status_bak;
				break;
			}
	}
}


#ifdef OAM_SON_ENABLE
/******************************************************************************
 * Function Name  : are_plmns_same
 * Description    : This function compares two PLMNs
 * Inputs         : p1 :Pointer to son_cell_plmn_info_t
 *                : p2 : Pointer to son_cell_plmn_info_t
 * Outputs        : none
 * Returns        : Non-zero if PLMNs match, zero otherwise
 *******************************************************************************/
UInt8 
are_plmns_same(
		son_cell_plmn_info_t *p1, 
		son_cell_plmn_info_t *p2
	      )
{
	UInt8 retVal = OAM_ZERO;
    int i = 0;
	OAM_FUN_ENTRY_TRACE();
    /* SPR 17748 START */
    if ((p1->mcc[0] == p2->mcc[0]) && (p1->mcc[1] == p2->mcc[1]) && (p1->mcc[2] == p2->mcc[2]) && (p1->num_mnc_digit == p2->num_mnc_digit))
    {
        for (i = 0; i < p1->num_mnc_digit; i++)
        {
            if (p1->mnc[i] != p2->mnc[i])
                break;
        }

	/* SPR 19224 Fix Start */
	retVal=OAM_ONE;
	/* SPR 19224 Fix End */
    }
    /* SPR 17748 END */
	OAM_FUN_EXIT_TRACE();
	return retVal;
}


/****************************************************************************
 * Function Name  : oam_cgi_search_in_neigh_list
 * Description    : This function searches the given CGI in the given neighbor
 *                  list
 * Inputs         : CGI to search
 *                : Neighbor list to CGI to search from
 *                : Neighbor list type
 * Outputs        : none
 * Returns        : Index of neighbor list for matched CGI, -1 if the CGI is
 *                  not found
 ****************************************************************************/
SInt16 oam_cgi_search_in_neigh_list(void *cgi, void *nr_list,
		UInt16 list_size,
		UInt32 *valid_bitmap,
		oam_son_neigh_type_et neigh_type)
{
	UInt16 idx = OAM_ZERO;
	UInt16 valid_neigh = OAM_ZERO;
	OAM_FUN_ENTRY_TRACE();
	SInt16 retVal = OAM_INVALID_RETURN;
	son_intra_rat_neighbor_info_t *intra_rat_nr_list = OAM_NULL;
	son_intra_rat_global_cell_id_t *intra_rat_cgi = OAM_NULL;
	son_inter_rat_neighbor_info_t *inter_rat_utran_nr_list = OAM_NULL;
	son_utran_geran_global_cell_id_t *utran_cgi = OAM_NULL;
	/*  SPR 13586 START */
	son_inter_rat_neighbor_info_t *inter_rat_geran_nr_list = OAM_NULL;
	son_utran_geran_global_cell_id_t *geran_cgi = OAM_NULL;
	/*  SPR 13586 END */


	switch(neigh_type) {
		case OAM_SON_NEIGH_TYPE_INTRA_RAT:
			intra_rat_nr_list = nr_list;
			intra_rat_cgi = cgi;
			for( ; idx < OAM_SON_MAX_NUM_NEIGHBOR && valid_neigh < list_size;
					++idx) {
				if(!valid_bitmap || *valid_bitmap & (OAM_ONE << idx)) {
					++valid_neigh;
					if(are_plmns_same(&intra_rat_nr_list[idx].nbr_cgi.plmn_id,
								&intra_rat_cgi->plmn_id) &&
							!oam_memcmp(intra_rat_nr_list[idx].nbr_cgi.
								cell_identity,
								intra_rat_cgi->cell_identity,
								SON_CELL_ID_OCTET_SIZE))
					{
						retVal = idx;
						break;
					}
				}
			}
			break;

		case OAM_SON_NEIGH_TYPE_INTER_RAT_UTRAN:
			inter_rat_utran_nr_list = nr_list;
			utran_cgi = cgi;
			for( ; idx < OAM_SON_MAX_NUM_NEIGHBOR && valid_neigh < list_size;
					++idx) {
				if(inter_rat_utran_nr_list[idx].bitmask &
						SON_INTER_UTRAN_NEIGHBOR) {
					if(!valid_bitmap || *valid_bitmap & (OAM_ONE << idx)) {
						++valid_neigh;
						if(are_plmns_same(&inter_rat_utran_nr_list[idx].
									utran_neighbor.nbr_cgi.plmn_id,
									&utran_cgi->plmn_id) &&
								!oam_memcmp(inter_rat_utran_nr_list[idx].
									utran_neighbor.nbr_cgi.cell_identity,
									utran_cgi->cell_identity,
									SON_CELL_ID_OCTET_SIZE))
						{
							retVal = idx;
							break;
						}
					}
				}
			}
			break;
			/*  SPR 13586 START */
		case OAM_SON_NEIGH_TYPE_INTER_RAT_GERAN:
			inter_rat_geran_nr_list = nr_list;
			geran_cgi = cgi;
			for( ; idx < OAM_SON_MAX_NUM_NEIGHBOR && valid_neigh < list_size;
					++idx) {
				if(inter_rat_geran_nr_list[idx].bitmask &
						SON_INTER_GERAN_NEIGHBOR) {
					if(!valid_bitmap || *valid_bitmap & (OAM_ONE << idx)) {
						++valid_neigh;
						if(are_plmns_same(&inter_rat_geran_nr_list[idx].
									geran_neighbor.nbr_cgi.plmn_id,
									&geran_cgi->plmn_id) &&
								!oam_memcmp(inter_rat_geran_nr_list[idx].
									geran_neighbor.nbr_cgi.cell_identity,
									geran_cgi->cell_identity,
									SON_CELL_ID_OCTET_SIZE))
						{
							retVal = idx;
							break;
						}
					}
				}
			}
			break;
			/*  SPR 13586 END */


		default:
			OAM_LOG(OAM, OAM_INFO, "CGI doesn't match.");
	}
	OAM_FUN_EXIT_TRACE();
	return retVal;
}


/****************************************************************************
 * Function Name  : oam_arfcn_search_in_meas_config_list
 * Description    : This function searches the given   arfcn(earfcn of uarfcn)
 *                  in the given measurement configuration list
 * Inputs         : arfcn to search
 *                : measurement configuration list to  arfcn  to  search  from
 *                : meas config list type
 * Outputs        : none
 * Returns        : Index of meas config list for matched arfcn, -1 if   arfcn
 *                  is not found
 ****************************************************************************/
SInt16 oam_arfcn_search_in_meas_config_list
(
 void* p_arfcn,
 void *meas_config_list,
 UInt16 list_size,
 UInt32 *valid_bitmap,
 oam_son_meas_config_type_et meas_type
 )
{
	OAM_FUN_ENTRY_TRACE();
	UInt16 idx = OAM_ZERO;
	UInt16 valid_meas_config = OAM_ZERO;
	SInt16 retVal = OAM_INVALID_RETURN;
	son_meas_config_eutran_t *eutran_meas_config_list = OAM_NULL;
	son_meas_config_utran_fdd_t *utran_fdd_meas_config_list = OAM_NULL;
	son_meas_config_utran_tdd_t *utran_tdd_meas_config_list = OAM_NULL;
	son_geran_carrier_freq_info_t *p_geran_carrier_freqs_list = OAM_NULL;
	son_geran_carrier_freqs_t *p_geran_arfcn = OAM_NULL;
	UInt16 arfcn = OAM_ZERO;

	switch(meas_type) {
		case OAM_SON_MEAS_CONFIG_TYPE_EUTRAN:
			eutran_meas_config_list = meas_config_list;
			arfcn = *(UInt16*)p_arfcn;

			for(idx = OAM_ZERO; idx < SON_MAX_CONFIG_OBJECT && valid_meas_config < list_size; ++idx) {
				if(!valid_bitmap || *valid_bitmap & (OAM_ONE << idx)) {
					++valid_meas_config;
					if(arfcn == eutran_meas_config_list[idx].earfcn) {
						OAM_LOG(OAM, OAM_DETAILED, "EARFCN found at index: %d", idx);
						retVal = idx;
						break;
					}
				}
			}
			break;

		case OAM_SON_MEAS_CONFIG_TYPE_UTRAN_FDD:
			utran_fdd_meas_config_list = meas_config_list;
			arfcn = *(UInt16*)p_arfcn;

			for(idx = OAM_ZERO; idx < SON_MAX_CONFIG_OBJECT && valid_meas_config < list_size; ++idx) {
				if(!valid_bitmap || *valid_bitmap & (OAM_ONE << idx)) {
					++valid_meas_config;
					if(arfcn == utran_fdd_meas_config_list[idx].uarfcn) {
						OAM_LOG(OAM, OAM_DETAILED, "UARFCN found at index: %d", idx);
						retVal = idx;
						break;
					}
				}
			}
			break;
		case OAM_SON_MEAS_CONFIG_TYPE_UTRAN_TDD:
			utran_tdd_meas_config_list = meas_config_list;
			arfcn = *(UInt16*)p_arfcn;
			for(idx = OAM_ZERO; idx < SON_MAX_CONFIG_OBJECT && valid_meas_config < list_size; ++idx) {
				if(!valid_bitmap || *valid_bitmap & (OAM_ONE << idx)) {
					++valid_meas_config;
					if(arfcn == utran_tdd_meas_config_list[idx].uarfcn) {
						OAM_LOG(OAM, OAM_DETAILED, "UARFCN found at index: %d", idx);
						retVal = idx;
						break;
					}
				}
			}

			break;

		case OAM_SON_MEAS_CONFIG_TYPE_GERAN:
			p_geran_carrier_freqs_list = meas_config_list;
			p_geran_arfcn = p_arfcn;

			for(idx = OAM_ZERO;
					idx < SON_MAX_CONFIG_OBJECT && valid_meas_config < list_size;
					++idx) {
				if(!valid_bitmap || *valid_bitmap & (OAM_ONE << idx)) {
					++valid_meas_config;
					if((p_geran_carrier_freqs_list[idx].geran_car_freqs.starting_arfcn ==
								p_geran_arfcn->starting_arfcn) &&
							p_geran_carrier_freqs_list[idx].geran_car_freqs.band_ind ==
							p_geran_arfcn->band_ind) {
						OAM_LOG(OAM, OAM_DETAILED, "UARFCN found at index: %d", idx);
						retVal = idx;
						break;
					}
				}
			}
			break;

		default:
			OAM_LOG(OAM, OAM_INFO, "ARFCN doesn't match.");
	}

	OAM_FUN_EXIT_TRACE();
	return retVal;
}
/* SPR-13586 START */
void oam_extract_ci_from_cell_identity(UInt16 *out_ci,
		UInt8 *p_cell_identity)
{
	*out_ci = (p_cell_identity[3] & 0xFF);
	*out_ci <<= OAM_BYTE_SIZE_IN_BITS;
	*out_ci |= (p_cell_identity[2] & 0xFF);

	OAM_LOG(OAM, OAM_DETAILED, "Extraced CI: %d", *out_ci);


}

/*SPR 21793 FIXED START*/
/*SPR 22398 Fix Start*/
/*While encoding, cell_identity is filled in this way
 * ie at index 0 and 1, lac is filled and at index 2 and 3
 * CI is filled. So decoding is also done in this way*/
/*SPR 22398 Fix End*/
void oam_extract_lac_from_cell_identity(UInt16 *out_lac,
		UInt8 *p_cell_identity)
{
    /*SPR 22398 Fix Start*/
   *out_lac = oam_ntohs(*((UInt16 *)p_cell_identity));
    /*SPR 22398 Fix End*/

	OAM_LOG(OAM, OAM_DETAILED, "Extraced LAC: %d", *out_lac);


}
/*SPR 21793 FIXED END*/
void oam_extract_value_from_bsic(UInt8 *out_bsic,
		son_geran_physical_cell_id_t *p_cell_identity)
{
	*out_bsic = (p_cell_identity->ncc & 0x7);
	*out_bsic <<= 3;
	*out_bsic |= (p_cell_identity->bscc & 0x7);

	OAM_LOG(OAM, OAM_DETAILED, "Extraced BSIC: %d", *out_bsic);


}
/* SPR-13586 END */
#endif

/* SPR 15604 Fix Start */
void oam_extract_cid_from_cell_identity(UInt16 *out_cid,
                                        UInt8 *p_cell_identity)
{
    UInt32 msb_byte_mask = OAM_EXTENDED_RNCID_BITMASK;
    UInt32 offset = 1;
    UInt32 msb_byte_offset = 3;
    *out_cid = 0;

    msb_byte_mask >>= msb_byte_offset * OAM_BYTE_SIZE_IN_BITS;
    msb_byte_mask >>= OAM_NIBBLE_SIZE_IN_BITS;

    if(!(p_cell_identity[msb_byte_offset] & msb_byte_mask)) {
        *out_cid = (p_cell_identity[offset] & 0xF);
        *out_cid <<= OAM_NIBBLE_SIZE_IN_BITS;
    }
    else {
        OAM_LOG(OAM, OAM_DETAILED, "Extended-RNCID bit is found set");
    }

    ++offset;

    *out_cid |= (p_cell_identity[offset] & 0xF0) >> OAM_NIBBLE_SIZE_IN_BITS;
    *out_cid <<= OAM_NIBBLE_SIZE_IN_BITS;
    *out_cid |= p_cell_identity[offset] & 0xF;

    ++offset;
    *out_cid <<= OAM_NIBBLE_SIZE_IN_BITS;
    *out_cid |= (p_cell_identity[offset] & 0xF0) >> OAM_NIBBLE_SIZE_IN_BITS;

    OAM_LOG(OAM, OAM_DETAILED, "Extraced CID: %d", *out_cid);
}

void oam_extract_rncid_from_cell_identity(UInt16 *out_rncid,
                                          UInt8 *p_cell_identity)
{
    UInt32 msb_byte_mask = OAM_EXTENDED_RNCID_BITMASK;
    UInt32 offset = 0;
    UInt32 msb_byte_offset = 3;

    msb_byte_mask >>= msb_byte_offset * OAM_BYTE_SIZE_IN_BITS;
    msb_byte_mask >>= OAM_NIBBLE_SIZE_IN_BITS;

    *out_rncid = (p_cell_identity[offset] & 0xF0) >> OAM_NIBBLE_SIZE_IN_BITS;
    *out_rncid <<= OAM_NIBBLE_SIZE_IN_BITS;

    *out_rncid |= p_cell_identity[offset] & 0xF;

    ++offset;
    *out_rncid <<= OAM_NIBBLE_SIZE_IN_BITS;
    *out_rncid |= (p_cell_identity[offset] & 0xF0) >> OAM_NIBBLE_SIZE_IN_BITS;

    if(p_cell_identity[msb_byte_offset] & msb_byte_mask) {
        OAM_LOG(OAM, OAM_DETAILED, "Extended-RNCID bit is found set");

        *out_rncid <<= OAM_NIBBLE_SIZE_IN_BITS;
        *out_rncid |= p_cell_identity[offset] & 0xF;
    }

    OAM_LOG(OAM, OAM_DETAILED, "Extraced RNCID: %d", *out_rncid);
}

/* SPR 15604 Fix End */



/****************************************************************************
 * Function Name  : oam_extract_rncid_from_utran_cell_identity
 * Description    : This function extract rncid from utran cell identity
 *                  
 * Inputs         : p_cell_identity 
 *                               
 * Outputs        : out_rncid 
 * Returns        : none
 *****************************************************************************/

void oam_extract_rncid_from_utran_cell_identity(UInt16 *out_rncid,
		UInt8 *p_cell_identity)
{
	UInt32 msb_byte_mask = OAM_EXTENDED_RNCID_BITMASK;
	UInt32 offset = 0;
	UInt32 msb_byte_offset = 3;

	msb_byte_mask >>= msb_byte_offset * OAM_BYTE_SIZE_IN_BITS;
	msb_byte_mask >>= OAM_NIBBLE_SIZE_IN_BITS;

	*out_rncid = (p_cell_identity[offset] & 0xF0) >> OAM_NIBBLE_SIZE_IN_BITS;
	*out_rncid <<= OAM_NIBBLE_SIZE_IN_BITS;

	*out_rncid |= p_cell_identity[offset] & 0xF;

	++offset;
	*out_rncid <<= OAM_NIBBLE_SIZE_IN_BITS;
	*out_rncid |= (p_cell_identity[offset] & 0xF0) >> OAM_NIBBLE_SIZE_IN_BITS;

	if(p_cell_identity[msb_byte_offset] & msb_byte_mask) {
		OAM_LOG(OAM, OAM_DETAILED, "Extended-RNCID bit is found set");

		*out_rncid <<= OAM_NIBBLE_SIZE_IN_BITS;
		*out_rncid |= p_cell_identity[offset] & 0xF;
	}

	OAM_LOG(OAM, OAM_DETAILED, "Extraced RNCID: %d", *out_rncid);
}

/****************************************************************************
 * Function Name  : oam_extract_cid_from_utran_cell_identity
 * Description    : This function extract cid from utran cell identity
 *
 * Inputs         : p_cell_identity
 *
 * Outputs        : out_cid
 * Returns        : none
 *****************************************************************************/

void oam_extract_cid_from_utran_cell_identity(UInt16 *out_cid,
		UInt8 *p_cell_identity)
{
	UInt32 msb_byte_mask = OAM_EXTENDED_RNCID_BITMASK;
	UInt32 offset = 1;
	UInt32 msb_byte_offset = 3;
	*out_cid = 0;

	msb_byte_mask >>= msb_byte_offset * OAM_BYTE_SIZE_IN_BITS;
	msb_byte_mask >>= OAM_NIBBLE_SIZE_IN_BITS;

	if(!(p_cell_identity[msb_byte_offset] & msb_byte_mask)) {
		*out_cid = (p_cell_identity[offset] & 0xF);
		*out_cid <<= OAM_NIBBLE_SIZE_IN_BITS;
	}
	else {
		OAM_LOG(OAM, OAM_DETAILED, "Extended-RNCID bit is found set");
	}

	++offset;

	*out_cid |= (p_cell_identity[offset] & 0xF0) >> OAM_NIBBLE_SIZE_IN_BITS;
	*out_cid <<= OAM_NIBBLE_SIZE_IN_BITS;
	*out_cid |= p_cell_identity[offset] & 0xF;

	++offset;
	*out_cid <<= OAM_NIBBLE_SIZE_IN_BITS;
	*out_cid |= (p_cell_identity[offset] & 0xF0) >> OAM_NIBBLE_SIZE_IN_BITS;

	OAM_LOG(OAM, OAM_DETAILED, "Extraced CID: %d", *out_cid);
}


/****************************************************************************
 * Function Name  : oam_encode_cid_to_utran_cell_identity
 * Description    : This function encode cid to utran cell identity
 *
 * Inputs         : cid
 *
 * Outputs        : p_cell_identity
 * Returns        : none
 *****************************************************************************/
void oam_encode_cid_to_utran_cell_identity(UInt8 *p_cell_identity,
		UInt16 cid)
{
	UInt32 offset = 1;
	UInt32 msb_byte_mask = OAM_EXTENDED_RNCID_BITMASK;
	UInt32 msb_byte_offset = 3;

	if(!p_cell_identity) {
		OAM_LOG(OAM, OAM_WARNING, "NULL POINTER ERROR!!!");
		return;
	}

	OAM_LOG(OAM, OAM_DETAILED, "Encoding CID: %d into cell identity", cid);

	msb_byte_mask >>= msb_byte_offset * OAM_BYTE_SIZE_IN_BITS;
	msb_byte_mask >>= OAM_NIBBLE_SIZE_IN_BITS;

	if(!(p_cell_identity[msb_byte_offset] & msb_byte_mask)) {
		p_cell_identity[offset] |= (cid & 0xF000) >> OAM_UMTS_CID_MIN_NUM_BITS;
		OAM_LOG(OAM, OAM_DETAILED, "Cell Identity[%d]: %d", offset,
				p_cell_identity[offset]);
	}
	else {
		OAM_LOG(OAM, OAM_DETAILED, "Extended-RNCID bit is found set");
	}

	++offset;
	p_cell_identity[offset] = (cid & 0xF00) >> OAM_NIBBLE_SIZE_IN_BITS;
	p_cell_identity[offset] |= (cid & 0xF0) >> OAM_NIBBLE_SIZE_IN_BITS;
	OAM_LOG(OAM, OAM_DETAILED, "Cell Identity[%d]: %d", offset,
			p_cell_identity[offset]);

	++offset;
	p_cell_identity[offset] |= (cid & 0xF) << OAM_NIBBLE_SIZE_IN_BITS;
	OAM_LOG(OAM, OAM_DETAILED, "Cell Identity[%d]: %d", offset,
			p_cell_identity[offset]);
}



/****************************************************************************
 * Function Name  : oam_encode_rncid_to_utran_cell_identity
 * Description    : This function encode rncid to utran cell identity
 *
 * Inputs         : rncid 
 *
 * Outputs        : p_cell_identity
 * Returns        : none
 *****************************************************************************/
void oam_encode_rncid_to_utran_cell_identity(UInt8 *p_cell_identity,
		UInt16 rncid)
{
	UInt32 offset = 0;
	UInt32 msb_byte_mask = OAM_EXTENDED_RNCID_BITMASK;
	UInt32 msb_byte_offset = 3;

	if(!p_cell_identity) {
		OAM_LOG(OAM, OAM_WARNING, "NULL POINTER ERROR!!!");
		return;
	}

	OAM_LOG(OAM, OAM_DETAILED, "Encoding RNCID: %d into cell identity",
			rncid);

	if(rncid > OAM_12BIT_MAX_VALUE) {
		p_cell_identity[offset] = (rncid & 0xF000) >> OAM_BYTE_SIZE_IN_BITS;
		p_cell_identity[offset] |= (rncid & 0xF00) >> OAM_BYTE_SIZE_IN_BITS;
		OAM_LOG(OAM, OAM_DETAILED, "Cell Identity[%d]: %d", offset,
				p_cell_identity[offset]);

		++offset;
		p_cell_identity[offset] = rncid & 0xF0;
		p_cell_identity[offset] |= rncid & 0xF;
		OAM_LOG(OAM, OAM_DETAILED, "Cell Identity[%d]: %d", offset,
				p_cell_identity[offset]);

		msb_byte_mask >>= msb_byte_offset * OAM_BYTE_SIZE_IN_BITS;
		msb_byte_mask >>= OAM_NIBBLE_SIZE_IN_BITS;
		p_cell_identity[msb_byte_offset] |= msb_byte_mask;
		OAM_LOG(OAM, OAM_DETAILED,
				"Setting the extended-RNCID indicator bit");
	}
	else {
		p_cell_identity[offset] = (rncid & 0xF00) >> OAM_NIBBLE_SIZE_IN_BITS;
		p_cell_identity[offset] |= (rncid & 0xF0) >> OAM_NIBBLE_SIZE_IN_BITS;
		OAM_LOG(OAM, OAM_DETAILED, "Cell Identity[%d]: %d", offset,
				p_cell_identity[offset]);

		++offset;
		p_cell_identity[offset] |= (rncid & 0xF) << OAM_NIBBLE_SIZE_IN_BITS;
		OAM_LOG(OAM, OAM_DETAILED, "Cell Identity[%d]: %d", offset,
				p_cell_identity[offset]);
	}
}

/****************************************************************************
 * Function Name  : oam_find_arfcn_in_available_freqs
 * Description    : This function searches the given   arfcn(earfcn of uarfcn)
 *                  in the given measurement configuration list
 * Inputs         : arfcn to search
 *                : measurement configuration list to  arfcn  to  search  from
 *                : meas config list type
 * Outputs        : none
 * Returns        : Index of meas config list for matched arfcn, -1 if   arfcn
 *                  is not found
 ****************************************************************************/


	oam_freq_type_et
oam_find_arfcn_in_available_freqs(UInt32 arfcn,
		oam_arfcn_type_et arfcn_type, oam_rrm_cell_config_req_t *p_rrm_cell_config)
{
	UInt16 cnt = 0;
	UInt32 avail_freq = 0;
	UInt16 max_cnt = 0;
	oam_freq_type_et retVal = OAM_FREQ_NOT_AVAILABLE;
	switch(arfcn_type) {
		case OAM_ARFCN_EUTRAN:
			if(arfcn == p_rrm_cell_config->ran_info.rf_params.
					rf_configurations.dl_earfcn) 
                        {
				retVal = OAM_EUTRAN_FREQ;
				break;
			}
			max_cnt = p_rrm_cell_config->ran_info.
				mobility_params.idle_mode_mobility_params.
				idle_mode_inter_freq_params_list.
				num_valid_inter_freq_list;

			for(cnt = 0; cnt < max_cnt; ++cnt) {
				avail_freq = p_rrm_cell_config->ran_info.
					mobility_params.idle_mode_mobility_params.
					idle_mode_inter_freq_params_list.
					idle_mode_mobility_inter_freq_params[cnt].
					eutra_carrier_arfcn;

				if(arfcn == avail_freq) {
					retVal = OAM_EUTRAN_FREQ;
					break;
				}
			}
			break;

		case OAM_ARFCN_UTRAN:
			max_cnt = p_rrm_cell_config->ran_info.
				mobility_params.idle_mode_mobility_params.
				idle_mode_mobility_inter_rat_utra_params.
				irat_eutran_to_utran_fdd_list.
				num_irat_eutran_to_utran_fdd_carriers;

			for(cnt = 0; cnt < max_cnt; ++cnt) {
				avail_freq = p_rrm_cell_config->ran_info.
					mobility_params.idle_mode_mobility_params.
					idle_mode_mobility_inter_rat_utra_params.
					irat_eutran_to_utran_fdd_list.
					irat_eutran_to_utran_fdd_carriers[cnt].
					utra_carrier_arfcn;

				if(arfcn == avail_freq) {
					retVal = OAM_UTRAN_FREQ_FDD;
					break;
				}
			}

			if(OAM_FREQ_NOT_AVAILABLE == retVal) {
				max_cnt = p_rrm_cell_config->ran_info.
					mobility_params.idle_mode_mobility_params.
					idle_mode_mobility_inter_rat_utra_params.
					irat_eutran_to_utran_tdd_list.
					num_irat_eutran_to_utran_tdd_carriers;

				for(cnt = 0; cnt < max_cnt; ++cnt) {
					avail_freq = p_rrm_cell_config->ran_info.
						mobility_params.idle_mode_mobility_params.
						idle_mode_mobility_inter_rat_utra_params.
						irat_eutran_to_utran_tdd_list.
						irat_eutran_to_utran_tdd_carriers[cnt].
						utra_carrier_arfcn;

					if(arfcn == avail_freq) {
						retVal = OAM_UTRAN_FREQ_TDD;
						break;
					}
				}
			}
			break;
		case OAM_ARFCN_GERAN:
			max_cnt = p_rrm_cell_config->ran_info.
				mobility_params.idle_mode_mobility_params.
				idle_mode_mobility_inter_rat_geran_params.
				carrier_freq_info_list.count_geran_carrier;

			for(cnt = 0; cnt < max_cnt; ++cnt) {
				avail_freq = p_rrm_cell_config->ran_info.
					mobility_params.idle_mode_mobility_params.
					idle_mode_mobility_inter_rat_geran_params.carrier_freq_info_list.
					carrier_list[cnt].carrier_freq.starting_arfcn;

				if(arfcn == avail_freq) {
					retVal = OAM_GERAN_FREQ;
					break;
				}
			}
			/* SPR 14298 FIX START */
			break; /* Coverity 69685 Fix */
			/* SPR 14298 FIX END */

		default:
			OAM_LOG(OAM, OAM_WARNING, "Unknown frequency input type: %u",
                arfcn_type);
	}

	return retVal;
}


/****************************************************************************
 * Function Name  : oam_encode_plmnid
 * Description    : This function encode plmnid
 *                  
 * Inputs         :input 
 * Outputs        : out_plmnid
 * Returns        : OAM_SUCCESS/OAM_FAILURE                  
 ****************************************************************************/
	oam_return_et
oam_encode_plmnid(Char8* input, UInt8 *out_plmnid)
{
	OAM_FUN_ENTRY_TRACE();
	UInt8 temp_plmn[OAM_PLMNID_MAX_LENGTH] = {OAM_ZERO};
	Char8 final[OAM_THREE] = {OAM_ZERO};
	UInt16 index = OAM_ZERO;
	UInt16 offset = OAM_ZERO;
	SInt32 len = OAM_ZERO;

	if(!input || !out_plmnid) {
		OAM_LOG(OAM, OAM_WARNING, "Null pointer error !!!");
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	len = oam_strlen(input);

	if (OAM_PLMNID_MAX_LENGTH == len) {
		oam_memcpy(temp_plmn, input, OAM_PLMNID_MAX_LENGTH);
	}
	else if(OAM_PLMNID_MIN_LENGTH == len) {
		temp_plmn[index] = input[index];
		++index;
		temp_plmn[index] = input[index];
		++index;
		temp_plmn[index] = 0xF;
		++index;
		temp_plmn[index] = input[index];
		++index;
		temp_plmn[index] = input[index];
		++index;
		temp_plmn[index] = input[index];
	}

	index = OAM_ZERO;
	for(offset = OAM_ZERO; offset < sizeof(final); ++offset) {
		snprintf(final, sizeof(final), "%c%c", temp_plmn[index],
				temp_plmn[index + 1]);
		out_plmnid[offset] = oam_atoh(final);
		index += 2;
	}

	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/* SPR-9485 START */
/****************************************************************************
 * Function Name  : oam_decode_plmnid
 * Inputs         : plmnid with three octets 
 * Outputs        : plmnid string
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function decodes the bit stream into cell Identity
 ****************************************************************************/
	oam_return_et
oam_decode_plmnid(UInt8 *in_plmnid,Char8 *out_temp)
{
	UInt8 offset = 0;
	UInt8 output,loop;
	Char8 temp[OAM_EIGHT] = {0};

	if (!in_plmnid) {
		OAM_LOG(OAM, OAM_WARNING, "Null pointer error");
		return OAM_FAILURE;
	}
	output = 0;

	for(loop = 0; loop < OAM_THREE; loop++)
	{
		output = (((in_plmnid[offset] & 0xF0) >> OAM_NIBBLE_SIZE_IN_BITS) | 0x00);
        /* SPR17739 */
		oam_sprintf((char *)temp, (const char *)"%x", output);
		oam_strncat((Char8 *)out_temp,(const Char8 *)temp,oam_strlen(temp));
		output = in_plmnid[offset] & 0x0F;
        /* SPR17739 */
		oam_sprintf((char *)temp, (const char *)"%x", output);
		oam_strncat((Char8 *)out_temp,(const Char8 *)temp,oam_strlen(temp));
		++offset;
	}

	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_plmnid_from_mcc_mnc
 * Inputs         : out_plmn :Pointer to out plmn
 *                  input_plmn :Pointer to void
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function converts the mnc,mcc plmn format to TR specific format of plmnid
 *****************************************************************************/

	oam_return_et
oam_plmnid_from_mcc_mnc(Char8 *out_plmn, void *p_input_plmn)
{
	OAM_FUN_ENTRY_TRACE();
	Char8  temp[OAM_SIX]={OAM_ZERO};
	rrm_oam_cell_plmn_info_t *input_plmn = p_input_plmn;


	if(OAM_TWO == input_plmn->num_mnc_digit)
	{
		temp[OAM_ZERO] = input_plmn->mcc[OAM_ONE];
		temp[OAM_ONE] = input_plmn->mcc[OAM_ZERO];
		temp[OAM_TWO] = 'F';
		temp[OAM_THREE] = input_plmn->mcc[OAM_TWO];
		temp[OAM_FOUR] = input_plmn->mnc[OAM_ONE];
		temp[OAM_FIVE] = input_plmn->mnc[OAM_ZERO];
	}
	else
	{
		temp[OAM_ZERO] = input_plmn->mcc[OAM_ONE];
		temp[OAM_ONE] = input_plmn->mcc[OAM_ZERO];
		temp[OAM_TWO] = input_plmn->mnc[OAM_ZERO]; 
		temp[OAM_THREE] = input_plmn->mcc[OAM_TWO];
		temp[OAM_FOUR] = input_plmn->mnc[OAM_TWO];
		temp[OAM_FIVE] = input_plmn->mnc[OAM_ONE];

	}

	if(OAM_THREE == input_plmn->num_mnc_digit)
		snprintf((char *)out_plmn,OAM_ONE + OAM_SIX,"%d%d%d%d%d%d",temp[OAM_ZERO],temp[OAM_ONE],temp[OAM_TWO],temp[OAM_THREE],temp[OAM_FOUR],temp[OAM_FIVE]);
	else
		/* SPR-10788 Fix Start */
		snprintf(out_plmn, OAM_ONE + OAM_SIX, "%d%d%s%d%d%d",
				temp[OAM_ZERO], temp[OAM_ONE], "f", temp[OAM_THREE],
				temp[OAM_FOUR], temp[OAM_FIVE]);
	/* SPR-10788 Fix End */

	OAM_FUN_EXIT_TRACE();

	return OAM_SUCCESS;
}
/* SPR Fix 76829 Start */
#ifdef OAM_SON_ENABLE
/*  coverity 94871/83280 fix start */
/*******************************************************************************
 *  FUNCTION NAME    : oam_map_pci_range 
 *  DESCRIPTION      : This function maps the pci range enums to calculate 
 *                     actual ranges to determine pci end value
 *  PARAMETERS
 *      IN           : pci_range
 *
 *      OUT          : NONE
 *
 *  RETURNS          : pci_range 
*******************************************************************************/
U16
son_oam_map_pci_range(
        son_oam_cell_id_range_et pci_range)
{
    U16   ret_value = OAM_ZERO;

    OAM_FUN_ENTRY_TRACE();

    switch(pci_range)
    {
        /* Coverity 96707 fix start */
        case  SON_OAM_N4:
            {
                ret_value = 4;
                break;
            }
        case  SON_OAM_N8:
            {
                ret_value = 8;
                break;
            }
        case  SON_OAM_N12:
            {
                ret_value = 12;
                break;
            }
        case  SON_OAM_N16:
            {
                ret_value = 16;
                break;
            }
        case  SON_OAM_N24:
            {
                ret_value = 24;
                break;
            }
        case  SON_OAM_N32:
            {
                ret_value = 32;
                break;
            }
        case  SON_OAM_N48:
            {
                ret_value = 48;
                break;
            }
        case  SON_OAM_N64:
            {
                ret_value = 64;
                break;
            }
        case  SON_OAM_N84:
            {
                ret_value = 84;
                break;
            }
        case  SON_OAM_N96:
            {
                ret_value = 96;
                break;
            }
        case  SON_OAM_N128:
            {
                ret_value = 128;
                break;
            }
        case  SON_OAM_N168:
            {
                ret_value = 168;
                break;
            }
        case  SON_OAM_N252:
            {
                ret_value = 252;
                break;
            }
        case  SON_OAM_N504:
            {
                ret_value = 504;
                break;
            }
        /* Coverity 96707 fix end */
        default:
            {
                OAM_LOG(OAM,OAM_ERROR,"unexpected pci range[%u] received", 
                    pci_range);
            }
    }

    OAM_FUN_EXIT_TRACE(); 
    return ret_value;
}
/*  coverity 94871/83280 fix end */
#endif
/* SPR Fix 76829 End */
/* SPR 15238 START */
/*******************************************************************************
 *  FUNCTION NAME    : oam_map_pci_range 
 *  DESCRIPTION      : This function maps the pci range enums to calculate 
 *                     actual ranges to determine pci end value
 *  PARAMETERS
 *      IN           : pci_range
 *
 *      OUT          : NONE
 *
 *  RETURNS          : pci_range 
*******************************************************************************/
U16
oam_map_pci_range(
        rrm_oam_cell_id_range_et pci_range)
{
    U16   ret_value = OAM_ZERO;

    OAM_FUN_ENTRY_TRACE();

    switch(pci_range)
    {
        case  RRM_OAM_N4:
            {
                ret_value = 4;
                break;
            }
        case  RRM_OAM_N8:
            {
                ret_value = 8;
                break;
            }
        case  RRM_OAM_N12:
            {
                ret_value = 12;
                break;
            }
        case  RRM_OAM_N16:
            {
                ret_value = 16;
                break;
            }
        case  RRM_OAM_N24:
            {
                ret_value = 24;
                break;
            }
        case  RRM_OAM_N32:
            {
                ret_value = 32;
                break;
            }
        case  RRM_OAM_N48:
            {
                ret_value = 48;
                break;
            }
        case  RRM_OAM_N64:
            {
                ret_value = 64;
                break;
            }
        case  RRM_OAM_N84:
            {
                ret_value = 84;
                break;
            }
        case  RRM_OAM_N96:
            {
                ret_value = 96;
                break;
            }
        case  RRM_OAM_N128:
            {
                ret_value = 128;
                break;
            }
        case  RRM_OAM_N168:
            {
                ret_value = 168;
                break;
            }
        case  RRM_OAM_N252:
            {
                ret_value = 252;
                break;
            }
        case  RRM_OAM_N504:
            {
                ret_value = 504;
                break;
            }
        default:
            {
                OAM_LOG(OAM,OAM_WARNING,"unexpected pci range received");
            }
    }

    OAM_FUN_EXIT_TRACE(); 
    return ret_value;
}

/* SPR 15238 END */

/* SPR 16567 START */
oam_return_et oam_send_enb_config_update(void)
{
    oam_return_et ret_val = OAM_FAILURE;
    OAM_FUN_ENTRY_TRACE();
	UInt8 cid = oam_prov_req.cell_cntxt.curr_cell_cid;
    ret_val = oam_s1ap_il_send_s1ap_oam_enb_config_update(
            &oam_prov_req.oam_l3_req.oam_s1ap_req.enb_config_update,
            OAM_MODULE_ID,
            RRC_MODULE_ID,
            oam_get_new_trans_id(),cid);
    if (ret_val == OAM_FAILURE)
    {
        OAM_LOG(OAM, OAM_ERROR, "Failed to send enb_config_update to RRC");
    }
    else
    {
        current_enb_config_count++;
        OAM_LOG(OAM, OAM_DETAILED, "successfully send enb_config_update to RRC %d", current_enb_config_count);
        ret_val = OAM_SUCCESS;
    }

    OAM_FUN_EXIT_TRACE();
    return ret_val;
}
/* SPR 16567 START */

/* SPR-9485 END */
oam_cell_status_t *oam_get_cell_cntxt_status(UInt8 cid)
{

	UInt8 loop = OAM_ZERO;
	oam_cell_status_t *p_cell_status = OAM_NULL;

	for(loop = OAM_ZERO; loop < oam_prov_req.num_cells_configured; ++loop) {
		if(cid == OAM_CELL_CNTXT_STATUS(loop).cid) {
			p_cell_status = &OAM_CELL_CNTXT_STATUS(loop);
		}
	}

	return p_cell_status;
}

/****************************************************************************
 * Function Name  : oam_mac_resp_code_to_str
 * Description    : This function is used to convert the response code
 *                  returned from MAC layer to equivalent char string
 * Inputs         : response code
 * Outputs        : NONE
 * Returns        : char string
 ***************************************************************************/
Char8* oam_mac_resp_code_to_str(UInt16 resp_code)
{
	UInt16 loop = OAM_ZERO;
	Char8 *err_string = "Internal error";
	UInt16 max_loop = sizeof(oam_mac_resp_code_tbl) /
		sizeof(oam_mac_resp_code_tbl[OAM_ZERO]);

	for(loop = OAM_ZERO; loop < max_loop; ++loop) {
		if(resp_code == oam_mac_resp_code_tbl[loop].resp_code) {
			err_string = oam_mac_resp_code_tbl[loop].resp_code_str;
		}
	}

	return err_string;
}

#ifdef OAM_SON_ENABLE
/****************************************************************************
 * Function Name  : oam_son_resp_code_to_str
 * Description    : This function is used to convert the response code
 *                  returned from SON layer to equivalent char string
 * Inputs         : response code
 * Outputs        : NONE
 * Returns        : char string
 ***************************************************************************/
Char8* oam_son_resp_code_to_str(son_error_et err_code)
{
	UInt16 loop = OAM_ZERO;
	Char8 *err_string = "Internal error";
	UInt16 max_loop = sizeof(oam_son_resp_code_tbl) /
		sizeof(oam_son_resp_code_tbl[OAM_ZERO]);

	for(loop = OAM_ZERO; loop < max_loop; ++loop) {
		if(err_code == oam_son_resp_code_tbl[loop].resp_code) {
			err_string = oam_son_resp_code_tbl[loop].resp_code_str;
		}
	}

	return err_string;
}
#endif

/****************************************************************************
 * Function Name  : oam_rrc_resp_code_to_str
 * Description    : This function is used to convert the response code
 *                  returned from RRC layer to equivalent char string
 * Inputs         : response code
 * Outputs        : NONE
 * Returns        : char string
 ***************************************************************************/
Char8* oam_rrc_resp_code_to_str(UInt16 resp_code)
{
	UInt16 loop = OAM_ZERO;
	Char8 *err_string = "Internal error";
	UInt16 max_loop = sizeof(oam_rrc_resp_code_tbl) /
		sizeof(oam_rrc_resp_code_tbl[OAM_ZERO]);

	for(loop = OAM_ZERO; loop < max_loop; ++loop) {
		if(resp_code == oam_rrc_resp_code_tbl[loop].resp_code) {
			err_string = oam_rrc_resp_code_tbl[loop].resp_code_str;
		}
	}

	return err_string;
}

/****************************************************************************
 * Function Name  : oam_rrm_resp_code_to_str
 * Description    : This function is used to convert the response code
 *                  returned from RRM layer to equivalent char string
 * Inputs         : response code
 * Outputs        : NONE
 * Returns        : char string
 ***************************************************************************/
Char8* oam_rrm_resp_code_to_str(SInt32 resp_code)
{
	UInt16 loop = OAM_ZERO;
	Char8 *err_string = "Internal error";
	UInt16 max_loop = sizeof(oam_rrm_resp_code_tbl) /
		sizeof(oam_rrm_resp_code_tbl[OAM_ZERO]);

	for(loop = OAM_ZERO; loop < max_loop; ++loop) {
		if(resp_code == oam_rrm_resp_code_tbl[loop].resp_code) {
			err_string = oam_rrm_resp_code_tbl[loop].resp_code_str;
		}
	}

	return err_string;
}



/****************************************************************************
 * Function Name  : oam_handle_init_config_failure
 * Inputs         : Internal alarm ID to be raised, 
 *                  Additional text,
 *                  Additional Information
 * Outputs        : None
 * Returns        : NONE
 * Description    : This function handles critical errors encountered during
 *                  system startup, raises critical alarm, and aborts
 ****************************************************************************/
void oam_handle_init_config_failure(UInt16 alarm_id,
		Char8 *p_add_txt,
		Char8 *p_add_info)
{
	UInt16 loop = OAM_ZERO;
	/* Spr 12880 Fix Start*/
	UInt8 CellId = OAM_INVALID_CELL_ID;
	/* Spr 12880 Fix End*/
	for(loop = OAM_ZERO;
			loop < FM_CNTXT.SupportedAlarmNumberOfEntries;
			++loop) {
		if(alarm_id == SUPP_ALARM_LIST(loop).alarm_identifier) {
			OAM_ALARM(alarm_id,
					SUPP_ALARM_LIST(loop).perceived_severity,
					NOTIFY_NEW_ALARM,
					SUPP_ALARM_LIST(loop).event_type,
					SUPP_ALARM_LIST(loop).probable_cause,
					SUPP_ALARM_LIST(loop).reporting_mechanism,
					/* Spr 12880 Fix Start*/
					p_add_txt, p_add_info,CellId);
			/* Spr 12880 Fix End*/
			break;
		}
	}

	if(loop == FM_CNTXT.SupportedAlarmNumberOfEntries) {
		OAM_LOG(OAM, OAM_WARNING, "Invalid alarm id received");
	}

	oam_killall();
}


/****************************************************************************
 * Function Name  : oam_is_duplicate_cell_identity
 * Inputs         : pointer to cell identity 
 * Outputs        : None
 * Returns        : a truth value
 * Description    : This function checks if the given cell identity is
 *                  already present in rrm_oam_cell_config structure
 ****************************************************************************/
SInt32 oam_is_duplicate_cell_identity(UInt8 *p_cell_identity)
{
	UInt8 loop = OAM_ZERO;
	SInt32 dup_found = OAM_ZERO;

	for(loop = OAM_ZERO; loop < OAM_MAX_CELL_SUPPORTED; ++loop) {
		dup_found = !oam_memcmp(p_cell_identity,
				OWN_CELL_CGI(loop).cell_identity,
				sizeof(OWN_CELL_CGI(loop).cell_identity));
		if(dup_found) {
			break;
		}
	}

	return dup_found;
}

/****************************************************************************
 * Function Name  : oam_get_cell_plmnlist_data_idx
 * Inputs         : cid
 * Outputs        : None
 * Returns        : OAM_ERROR_RETURN_VAL(-1) OR array index value
 * Description    : This function returns the array index of the array
 *                  of PLMNList
 ****************************************************************************/
	SInt8
oam_get_cell_plmnlist_data_idx(UInt8 cid)
{
	UInt8 cell_count = OAM_ZERO;
	SInt8 retVal = OAM_ERROR_RETURN_VAL;

	while(cell_count < oam_prov_req.num_cells_configured) {
		if(RRM_PROV_REQ.plmnlist_data[cell_count].cid == cid) {
			retVal = cell_count;
			break;
		}
		else {
			++cell_count;
		}
	}

	return retVal;
}

/*SPR 11488 FIX START*/
/******************************************************************************
 *   FUNCTION NAME  : oam_rrc_send_cleanup_req
 *   DESCRIPTION    : This function constructs and sends RRC OAM
 *            Cleanup Request
 *   INPUTS     : src_module_id :Source module identifier
 *          : transaction_id :Interface transaction identifier
 *   OUTPUTS    : p_error_code :Error Code
 *   RETURNS    : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et oam_rrc_send_cleanup_req
(
 UInt16     src_module_id,
 UInt16     transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_length_t msg_length = OAM_ZERO, msg_api_length = OAM_ZERO;
	UInt8 *p_msg = OAM_NULL, *p_oam_msg = OAM_NULL;
	OAM_FUN_ENTRY_TRACE();
	/* Get API length */
	msg_length = OAM_ZERO;

	OAM_LOG(OAM,OAM_DETAILED, "Clean Up Request: length %u", msg_length);

	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;

	if (OAM_FAILURE == oam_mem_alloc(msg_api_length,(void *)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		/* Not Enough Memory */
		return OAM_FAILURE;
	}

	p_oam_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_oam_msg, RRC_VERSION_ID, src_module_id,
			RRC_MODULE_ID, RRC_OAM_CLEANUP_REQ, msg_api_length);

	/* Fill interface header */
	p_oam_msg = p_oam_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_oam_msg, transaction_id,
			src_module_id, RRC_MODULE_ID, RRC_OAM_CLEANUP_REQ, msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Fill RRC_OAM_CLEANUP_REQ message */

	/* Coverity ID : 81843 Fix Start*/	
	/*  p_s1apOam_msg = p_s1apOam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;    */
	/* Coverity ID : 81843 Fix End*/

	/* Send message to */
	if(OAM_FAILURE == oam_send_message(p_msg, RRC_MODULE_ID, p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in sending Message to RRC with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME  : oam_s1ap_send_cleanup_req
 *   DESCRIPTION    : This function constructs and sends S1AP OAM
 *            Cleanup Request
 *   INPUTS     : src_module_id :Source module identifier
 *          : transaction_id :Interface transaction identifier
 *   OUTPUTS    : p_error_code :Error Code
 *   RETURNS    : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et oam_s1ap_send_cleanup_req
(
 UInt16     src_module_id,
 UInt16     transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_length_t msg_length = OAM_ZERO, msg_api_length = OAM_ZERO;
	UInt8 *p_msg = OAM_NULL, *p_s1apOam_msg = OAM_NULL;
	OAM_FUN_ENTRY_TRACE();
	/* Get API length */
	msg_length = OAM_ZERO;

	OAM_LOG(OAM,OAM_DETAILED, "Clean Up Request: length %u", msg_length);

	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	if (OAM_FAILURE == oam_mem_alloc(msg_api_length,(void *)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		/* Not Enough Memory */
		return OAM_FAILURE;
	}
	p_s1apOam_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_s1apOam_msg, RRC_VERSION_ID, src_module_id,
			RRC_MODULE_ID, S1AP_OAM_CLEANUP_REQ, msg_api_length);

	/* Fill interface header */
	p_s1apOam_msg = p_s1apOam_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_s1apOam_msg, transaction_id,
			src_module_id, RRC_MODULE_ID, S1AP_OAM_CLEANUP_REQ, msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);

	/* Fill S1AP_OAM_RESET_REQ message */

	/* Coverity ID : 81819 Fix Start*/	
	/*  p_s1apOam_msg = p_s1apOam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;    */
	/* Coverity ID : 81819 Fix End*/

	/* Send message to */
	if(OAM_FAILURE == oam_send_message(p_msg, RRC_MODULE_ID, p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in sending Message to RRC with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME  : oam_x2ap_send_cleanup_req
 *   DESCRIPTION    : This function constructs and sends X2AP OAM
 *            Cleanup Request
 *   INPUTS     : src_module_id :Source module identifier
 *          : transaction_id :Interface transaction identifier
 *   OUTPUTS    : p_error_code :Error Code
 *   RETURNS    : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et oam_x2ap_send_cleanup_req
(
 UInt16     src_module_id,
 UInt16     transaction_id,
 oam_error_code_et *p_error_code
 )
{
	oam_length_t msg_length = OAM_ZERO, msg_api_length = OAM_ZERO;
	UInt8 *p_msg = OAM_NULL, *p_s1apOam_msg = OAM_NULL;
	OAM_FUN_ENTRY_TRACE();
	/* Get API length */
	msg_length = OAM_ZERO;

	OAM_LOG(OAM,OAM_DETAILED, "Clean Up Request: length %u", msg_length);

	msg_api_length = msg_length + OAM_CSPL_HDR_SIZE + LOWER_LAYER_INTERFACE_HEADER_SIZE;
	if (OAM_FAILURE == oam_mem_alloc(msg_api_length,(void *)&p_msg,p_error_code))
	{
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		/* Not Enough Memory */
		return OAM_FAILURE;
	}
	p_s1apOam_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_s1apOam_msg, RRC_VERSION_ID, src_module_id,
			RRC_MODULE_ID, X2AP_OAM_CLEANUP_REQ, msg_api_length);

	/* Fill interface header */
	p_s1apOam_msg = p_s1apOam_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_interface_api_header(p_s1apOam_msg, transaction_id,
			src_module_id, RRC_MODULE_ID, X2AP_OAM_CLEANUP_REQ, msg_length,
			oam_prov_req.cell_cntxt.curr_cell_cid);
    /*  Coverity ID : 81844 Fix Start */
    /*  p_s1apOam_msg = p_s1apOam_msg + LOWER_LAYER_INTERFACE_HEADER_SIZE;  */
    /*  Coverity ID : 81844 Fix End */

	/* Send message to */
	if(OAM_FAILURE == oam_send_message(p_msg, RRC_MODULE_ID, p_error_code))
	{
		OAM_LOG(OAM,OAM_ERROR,"Error in sending Message to RRC with Error Code:%d",
				*p_error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}
/*SPR 11488 FIX END*/

/****************************************************************************
 *   FUNCTION NAME  : oam_maintain_tcb_ext
 *   DESCRIPTION     : This function is an extenstion of the existing function   
 *                     oam_maintain_tcb(), besides the user request buffer
 *                     coming from user management interface, it also saves
 *                     a temporary copy of the xmlNode representaion of the
 *                     user request inputs
 *   INPUT           : trans_id :transaction identifier
 *                     trans_type :transaction type
 *                     msg_size :size of message
 *                     p_api_buf :Pointer to Buffer 
 *                     p_req_parent:  the xmlNode representation of user
 *                                    request
 *   OUTPUT          : None
 *   RETURNS         : OAM_SUCCESS/OAM_FAILURE
 ****************************************************************************/
	oam_return_et 
oam_maintain_tcb_ext(UInt16 trans_id,
		oam_trans_type_et trans_type,
		UInt16 msg_size,
		UInt8 *p_api_buf,
		xmlNode *p_orig_parent,
		xmlNode *p_req_parent,
		xmlNode **p_p_xml_struct_addr)
{
	oam_tcb_cntxt_data_t tcb_data = {OAM_ZERO};
    /* SPR 17777 fix */
    OAM_LOG(OAM, OAM_DETAILEDALL, "unused parameter %d", msg_size);
	tcb_data.usr_req_trans_id = get_word_from_header(p_api_buf +
			OAM_INTF_HDR_TRANS_ID_OFFSET);
	tcb_data.usr_intf_src_id = get_word_from_header(p_api_buf +
			OAM_INTF_HDR_SRC_ID_OFFSET);
	tcb_data.p_usr_msg_xml_parent = p_req_parent;
	tcb_data.p_orig_xml_parent = p_orig_parent;
	tcb_data.p_parent_xml_struct_addr = p_p_xml_struct_addr;

	return oam_maintain_tcb(trans_id, trans_type,
			sizeof(tcb_data), &tcb_data);
}

/****************************************************************************
 * Function Name  : oam_process_tcb_for_layer_resp_ext
 * Inputs         : p_api_buf :Pointer to api buffer
 * Outputs        : tid :Transaction id	fetched from TCB
 *                  dest_id: Destination id fetched from TCB
 *                  p_p_orig_xml_parent: the xmlNode representation of the
 *                  original parent of the parameters being updated by user.
 *                  p_p_req_xml_parent: the xmlNode representation of the
 *                  user request for which response is received. This is
 *                  used in the persistence of xml DOM tree, if layer
 *                  response is success
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function is used to stop the timer, delete tcb entry
 *                  and get transaction/destination id and the xmlNode
 *                  representation of the user request
 ****************************************************************************/

	oam_return_et
oam_process_tcb_for_layer_resp_ext(UInt8* p_resp,
		UInt16 *tid,
		UInt16 *dest_id,
		xmlNode **p_p_orig_xml_parent,
		xmlNode **p_p_req_xml_parent,
		xmlNode ***p_p_p_xml_struct_addr)
{
	UInt32 trans_id;
	oam_return_et retVal = OAM_SUCCESS;
	oam_struct_t *node = OAM_NULL;
	oam_tcb_cntxt_data_t *p_tcb_data = OAM_NULL;

	OAM_FUN_ENTRY_TRACE();

	trans_id = get_word_from_header(p_resp + OAM_INTF_HDR_TRANS_ID_OFFSET);

	node = oam_tcb_get_detail(&trans_id);

	if(node) {
        /* SPR 19236 Fix + */
		p_tcb_data = (oam_tcb_cntxt_data_t *)node->oam_tcb_context;
        /* SPR 19236 Fix - */
		*tid = p_tcb_data->usr_req_trans_id;
		*dest_id = p_tcb_data->usr_intf_src_id;
		*p_p_req_xml_parent = p_tcb_data->p_usr_msg_xml_parent;
		*p_p_orig_xml_parent = p_tcb_data->p_orig_xml_parent;
		*p_p_p_xml_struct_addr = p_tcb_data->p_parent_xml_struct_addr;

		OAM_LOG(OAM, OAM_DETAILED, "User request Trans_id: %d, "
				"user SRC ID: %d", *tid, *dest_id);

		if (node->timer_id) {
			OAM_LOG(OAM, OAM_DETAILED, "Stopping user request timer");
			retVal = oam_stop_timer(node->timer_id);
			if (OAM_SUCCESS != retVal) {
				OAM_LOG(OAM, OAM_WARNING,
						"Error returned while stopping the timer[%u]", node->timer_id);

				retVal = OAM_SUCCESS;
			}
			else {
				OAM_LOG(OAM, OAM_DETAILED, "Timer Stopped[%p]", node->timer_id);
			}
		}

		oam_tcb_delete(&trans_id);
	}
	else {
		OAM_LOG(OAM, OAM_WARNING,
				"Node fetched from TCB is NULL, Returning Failure");
		retVal = OAM_FAILURE;
	}

	OAM_FUN_EXIT_TRACE();
	return retVal;
}


/***************************************************************************
 * FUNCTION NAME:      oam_add_new_xsd_validated_xml_child 
 * DESCRIPTION  :      This function implicitly uses the global variable
 *                     g_parameter_map_element and adds a new child
 *                     described at the index "loop" of the parameter
 *                     table g_parameter_map_element to the parent pointed
 *                     by p_parent. It adds the child according to the
 *                     xsd file so that it can also be validated successfully
 * Inputs       :      p_parent: the pointer to the xml parent
 *                     of the parameter being added
 *                     loop : index of the parameter in parameter table
 *                            g_parameter_map_element
 *              :      p_value: value to be assigned to the newly added child
 * Output       :      NONE
 * RETURNS      :      pointer to the new child
 ***************************************************************************/
	xmlNode*
oam_add_new_xsd_validated_xml_child(xmlNode *p_parent,
		UInt16 loop,
		UInt8 *p_value)
{
	SInt16 idx = OAM_ZERO;
	parameter_map_element_t *p_param_tbl = g_parameter_map_element;
	xmlNode *p_sibling = OAM_NULL;
	xmlNode *p_last_child = OAM_NULL;
	xmlNode *p_child = OAM_NULL;
	xmlNode *p_tmp = OAM_NULL;
	UInt16 max_element = oam_find_global_table_size(p_param_tbl);

	p_tmp = p_parent->children;

	if (!loop) {
		if (p_tmp) {
			/* SPR 14153 FIX START */
			p_child = oam_xml_add_prev_sibling(p_tmp,
					p_param_tbl[loop].tag_name,
					(Char8*)p_value);
			if (!p_child) {
				/* SPR 14153 FIX END */
				OAM_LOG(OAM, OAM_WARNING, "Failed to create new node");
			}
		}
		else {
			p_child = xmlNewChild(p_parent, OAM_NULL,
					(xmlChar*)p_param_tbl[idx].tag_name,
					p_value);
			if (!p_child) {
				OAM_LOG(OAM, OAM_WARNING, "Failed to create new node");
			}
		}
	}
	else {
		for (idx = loop - 1; idx >= 0; --idx) {
			p_sibling = oam_get_xml_leaf_child_node(p_parent,
					p_param_tbl[idx].tag_name);

			if (p_sibling) {
				/* SPR 14153 FIX START */
				p_child = oam_xml_add_next_sibling(p_sibling,
						p_param_tbl[loop].tag_name,
						(Char8*)p_value);
				/* SPR 14153 FIX END */

				break;
			}
		}

		if (idx < 0) {
			for (idx = loop + 1; idx < max_element; ++idx) {
				p_sibling = oam_get_xml_leaf_child_node(p_parent,
						p_param_tbl[idx].tag_name);

				if (p_sibling) {
					/* SPR 14153 FIX START */
					p_child = oam_xml_add_prev_sibling(p_sibling,
							p_param_tbl[loop].tag_name,
							(Char8*)p_value);
					/* SPR 14153 FIX END */

					break;
				}
			}
		}

		if (idx == max_element) {
			p_last_child = oam_get_xml_leaf_child_node(p_parent,
					"X_VENDOR_INSTANCE_ID");
			if (p_last_child) {
				/* SPR 14153 FIX START */
				p_child = oam_xml_add_prev_sibling(p_last_child,
						p_param_tbl[loop].tag_name,
						(Char8*)p_value);
				/* SPR 14153 FIX END */
			}
		}
	}

	if (p_child) {
		p_child->doc = p_parent->doc;
	}

	return p_child;
}

/***************************************************************************
 * FUNCTION NAME:      oam_update_xml_dom 
 * DESCRIPTION  :      This function updates parameter values in the xml dom
 *                     tree
 * Inputs       :      layer_resp: OAM_SUCCESS, OAM_FAILURE
 *              :      p_orig_xml_parent: the pointer to the xml original
 *                     parent of the parameters being updated
 *                     p_req_xml_parent : pointer to the xml updated parent
 *                     of the parameters having updated values
 *              :      p_parent_xml_struct_addr: address of the field in the
 *                     structure XML_struct_cell_config which is pointing to
 *                     the parent of the parameters in question
 * Output       :      NONE
 * RETURNS      :      NONE
 ****************************************************************************/
void oam_update_xml_dom(oam_return_et layer_resp,
		xmlNode *p_orig_parent,
		xmlNode *p_updated_parent,
		xmlNode **p_parent_xml_struct_addr)
{
    /* SPR 18930  FIX START */
    /* Code Removed */
    /* SPR 18930  FIX END */
	xmlChar* curr_num_entries_str = OAM_NULL;
	UInt16 curr_num_entries = OAM_ZERO;
	xmlChar   buff[OAM_OBJ_INSTANCE_NUM_STR_MAX_SIZE] = {OAM_ZERO};

	if (!p_updated_parent) {
		OAM_LOG(OAM, OAM_WARNING,
				"Null pointer received, nothing to do...");
		return;
	}


	if (OAM_SUCCESS == layer_resp) {
		/* Deleting existing xml object */
		if (oam_prov_req.system_status.event_ongoing &
				OAM_DELETE_OBJ_CELL_RECONFIG_ONGOING) {
			/* SPR 13120 FIX START */
			OAM_LOG(OAM, OAM_DETAILED, "Deleting xml object: %s",
					p_orig_parent->name);
			/* SPR 13120 FIX END */
			curr_num_entries_str = xmlNodeGetContent(p_updated_parent);
			curr_num_entries = oam_atoi((Char8*)curr_num_entries_str);

			/* Coverity 93923 FIX START */
			if(curr_num_entries > 0)
			{
				--curr_num_entries;
			}
			else
			{
				return;
			}
			/* Coverity 93923 FIX END */

			/*Convert the interger to string*/
			oam_snprintf((Char8*)buff, sizeof(buff), "%d", curr_num_entries);
			xmlNodeSetContent(p_updated_parent, buff);

			if (p_updated_parent->next == p_orig_parent) {
				if (*p_parent_xml_struct_addr == p_orig_parent) {
					if (curr_num_entries) {
						*p_parent_xml_struct_addr = p_orig_parent->next;
					}
					else {
						*p_parent_xml_struct_addr = OAM_NULL;
					}
				}
			}

			xmlUnlinkNode(p_orig_parent);
			xmlFreeNode(p_orig_parent);
			xmlFree(curr_num_entries_str);
			return;
		}

    /* SPR 18930  FIX START */
        /* Code removed */
    /* SPR 18930  FIX END */
        /* Simple updation of some parameters in an xml subtree */
        OAM_LOG(OAM, OAM_DETAILED,
                "Updating child parameters of xml object: %s",
                p_orig_parent->name);

        oam_update_xml_subtree(p_updated_parent, p_orig_parent);

        /* Free the memory allocated for temporary xml object */
    /* SPR_19303 Fix Start */
	//    xmlFreeNode(p_updated_parent);
    /* SPR_19303 Fix End */
    /* SPR 18930  FIX START */
        /* Braces Removed */
    /* SPR 18930  FIX END */
        /* SPR 13120 FIX END */
    }
	else {
		/* SPR 13120 FIX START */
		OAM_LOG(OAM, OAM_DETAILED, "Nothing to update, just free-up the "
				"temporary allocated memory");
		/* SPR 13120 FIX END */

		if (!(oam_prov_req.system_status.
/* SPR 16603 Fix start */
                    event_ongoing & OAM_DELETE_OBJ_CELL_RECONFIG_ONGOING)
            && !(oam_prov_req.system_status.event_ongoing & OAM_ADD_OBJ_ONGOING )) {
/* SPR 16603 Fix end */
			p_updated_parent->prev = OAM_NULL;
			p_updated_parent->doc = OAM_NULL;
			/*SPR 21619 Start*/
			/* Code Removed */
			/*SPR 21619 End*/
		}
	}
}

/* SPR 17777 fix */
oam_return_et oam_cleanup_layers(UInt16 trans_id)
{
	oam_return_et retVal = OAM_FAILURE;
	UInt8 curr_cid = oam_prov_req.cell_cntxt.curr_cell_cid;

#ifdef OAM_SON_ENABLE
	oam_son_shutdown_req_t son_shutdown_req;
#endif

	oam_rrm_shutdown_req_t rrm_shutdown_req;

	OAM_FUN_ENTRY_TRACE();

#ifdef OAM_CLEANUP_FLAG
    /*SPR 21369 Start*/
    oam_error_code_et *p_err =NO_ERROR;
    UInt8 l2_instance = OAM_ZERO;
    for(l2_instance =OAM_ZERO;(l2_instance <OAM_MAX_NUM_L2_INSTANCE);l2_instance++)
    {
        if(PDCP_OAM_INIT_CNF & l2_comm_info[l2_instance].init_flag)
        {
            OAM_LOG(OAM, OAM_INFO, "Sending clean-up request to MAC");
            retVal = oam_send_mac_cleanup_req(OAM_MODULE_ID, trans_id,
                    L2_INSTANCE_CELL_INFO(l2_instance, OAM_ZERO).cell_idx,p_err);
            if (OAM_FAILURE == retVal) {
                OAM_LOG(OAM, OAM_ERROR, "Sending Cleanup request to MAC failed"
                        " with error code: %d", *p_err);
            }

            OAM_LOG(OAM, OAM_INFO, "Sending clean-up request to PDCP");
            retVal = oam_send_pdcp_cleanup_req(OAM_MODULE_ID, trans_id,
                    L2_INSTANCE_CELL_INFO(l2_instance, OAM_ZERO).cell_idx,p_err);
            if (OAM_FAILURE == retVal) {
                OAM_LOG(OAM, OAM_ERROR, "Sending Cleanup request to PDCP failed"
                        " with error code: %d", *p_err);
            }
        }
    }
    /*SPR 21369 End*/
	OAM_LOG(OAM, OAM_INFO, "Sending clean-up request to RRC");
	retVal = oam_rrc_send_cleanup_req(OAM_MODULE_ID, trans_id, p_err);
	if (OAM_FAILURE == retVal) {
		OAM_LOG(OAM, OAM_ERROR, "Sending Cleanup request to RRC failed"
				" with error code: %d", *p_err);
	}

	OAM_LOG(OAM, OAM_INFO, "Sending clean-up request to S1AP");
	retVal = oam_s1ap_send_cleanup_req(OAM_MODULE_ID, trans_id, p_err);
	if (OAM_FAILURE == retVal) {
		OAM_LOG(OAM, OAM_ERROR, "Sending Cleanup request to S1AP failed"
				" with error code: %d", *p_err);
	}

	OAM_LOG(OAM, OAM_INFO, "Sending clean-up request to X2AP");
	retVal = oam_x2ap_send_cleanup_req(OAM_MODULE_ID, trans_id, p_err);
	if (OAM_FAILURE == retVal) {
		OAM_LOG(OAM, OAM_ERROR, "Sending Cleanup request to X2AP failed"
				" with error code: %d", *p_err);
	}
#endif

#ifdef OAM_SON_ENABLE
	/* coverity 41331 */
	OAM_LOG(OAM, OAM_INFO, "Sending clean-up request to SON");
	son_shutdown_req.shutdown_type = SON_FORCED_SHUTDOWN;
	son_shutdown_req.time_to_shutdown = 1;

	retVal = oam_son_send_son_oam_shutdown_req(&son_shutdown_req,
			OAM_MODULE_ID,
			SON_MODULE_ID,
			trans_id,
			curr_cid);
	if (OAM_FAILURE == retVal) {
		OAM_LOG(OAM, OAM_ERROR,
				"Sending clean-up request to SON failed");
	}
#endif

	OAM_LOG(OAM, OAM_INFO, "Sending clean-up request to RRM");
	rrm_shutdown_req.shutdown_mode = RRM_SHUTDOWN_MODE_FORCED; 
	rrm_shutdown_req.time_to_shutdown = 1;

	retVal = oam_rrm_send_rrm_oam_shutdown_req(&rrm_shutdown_req,
			OAM_MODULE_ID,
			RRM_MODULE_ID,
			trans_id,
			curr_cid);
	if (OAM_FAILURE == retVal) {
		OAM_LOG(OAM, OAM_ERROR,
				"Sending clean-up request to RRM failed");
	}

	OAM_FUN_EXIT_TRACE();
	return retVal;
}
/* SPR 11351 PLMNID FIX */
	oam_return_et
oam_convert_plmnid(Char8 *out_plmn, void *p_input_plmn)
{
	OAM_FUN_ENTRY_TRACE();
	char *ip_plmnid = (char*)p_input_plmn;
	int len = 0;

	oam_return_et retCode = OAM_SUCCESS;
	len = oam_strlen(ip_plmnid);
	OAM_LOG(OAM, OAM_DETAILED, "length %d",len);
	if(OAM_SIX == len)
	{
		out_plmn[1] = *ip_plmnid;
		out_plmn[0] = *(ip_plmnid + 1);
		out_plmn[3] = *(ip_plmnid + 2);
		out_plmn[2] = *(ip_plmnid + 3);
		out_plmn[5] = *(ip_plmnid + 4);
		out_plmn[4] = *(ip_plmnid + 5);
		out_plmn[6] = '\0';
	}
	else if(OAM_FIVE == len)
	{
		out_plmn[1] = *ip_plmnid;
		out_plmn[0] = *(ip_plmnid + 1);
		out_plmn[2] = 'F';
		out_plmn[3] = *(ip_plmnid + 2);
		out_plmn[5] = *(ip_plmnid + 3);
		out_plmn[4] = *(ip_plmnid + 4);
		out_plmn[6] = '\0';
	}
	else
	{
		OAM_LOG(OAM, OAM_DETAILED, "Wrong PLMNID");
		out_plmn[0] = '\0';
		retCode = OAM_FAILURE; 
	}
	OAM_FUN_EXIT_TRACE();
	return retCode;
}

/* SPR 13120 FIX START */
/***************************************************************************
 * FUNCTION NAME:      oam_update_xml_subtree 
 * DESCRIPTION  :      This function updates some parameter values in the xml
 *                     subtree
 * Inputs       :      p_new_subtree: pointer to xmlNode subtree having
 *                     updated values
 *              :      p_orig_subtree: pointer to the original xml subtree
 *                     which should be updated
 * Output       :      NONE
 * RETURNS      :      NONE
 ****************************************************************************/
void oam_update_xml_subtree(xmlNode *p_new_subtree, xmlNode *p_orig_subtree)
{
	xmlNode *p_new_node = OAM_NULL;
	xmlNode *p_orig_node = OAM_NULL;
	xmlNode *p_new_sibling = OAM_NULL;

	if (!p_new_subtree || !p_orig_subtree) {
		OAM_LOG(OAM, OAM_WARNING, "%s is NULL",
				p_new_node ? "p_orig_node" : "p_new_node");
		return;
	}

	p_new_node = p_new_subtree->children;
	p_orig_node = p_orig_subtree->children;

	while (p_new_node) {
    /* SPR 18930  FIX START */
						OAM_LOG(OAM, OAM_DETAILED,
								"name  of parameter: %s "
								"  p_new_node->name: %s",
								p_orig_node->name,
                                p_new_node->name);
    /* SPR 18930  FIX END */
		if ((XML_ELEMENT_NODE == p_new_node->type) && p_new_node->children) {
			if ((XML_TEXT_NODE == (p_new_node->children)->type)) {
				if (!xmlStrcmp(p_orig_node->name, p_new_node->name)) {
				/*SPR 23390 Fix Start*/
					if ((p_orig_node->children->content != OAM_PNULL) && 
						(p_new_node->children->content != OAM_PNULL)&&
						(xmlStrcmp(p_orig_node->children->content,
								p_new_node->children->content))) {
				/*SPR 23390 Fix End*/
						OAM_LOG(OAM, OAM_DETAILED,
								"Updating old value[%s] of parameter: %s "
								"with new value[%s]",
								p_orig_node->children->content,
								p_orig_node->name,
								p_new_node->children->content);

						xmlNodeSetContent(p_orig_node,
								(p_new_node->children)->content);
					}
				}
				else {
					p_new_sibling = oam_xmlCopyNode(p_new_node, 1);

					if (p_new_sibling) {
						p_orig_node = xmlAddPrevSibling(p_orig_node,
								p_new_sibling);

						if (!p_new_sibling) {
							OAM_LOG(OAM, OAM_WARNING, "Adding new xml node "
									"sibling failed");
							break;
						}
						else {
							OAM_LOG(OAM, OAM_DETAILED,
									"Adding new previous sibling[%s] with "
									"value[%s] to %s",
									p_new_sibling->name,
									p_new_sibling->children->content,
									p_orig_node->next->name);
						}
					}
					else {
						OAM_LOG(OAM, OAM_WARNING, "xmlNode copy failed");
						break;
					}
				}
			}
			else {
				oam_update_xml_subtree(p_new_node, p_orig_node);
			}
		}

		p_new_node = p_new_node->next;
		p_orig_node = p_orig_node->next;
	}
}
/* SPR 13120 FIX END */
/*SPR 8725 end*/

/* SPR 13542 START */
	void
oam_update_plmnlist_from_mcc_mnc (void *p_input_plmnid, UInt8 index, bc_plmn_list_t *p_bc_plmnList)
{
	OAM_FUN_ENTRY_TRACE();

	/* SPR 13542 REVIEW COMMENTS START */
	Char8  temp_plmn[OAM_SEVEN] = {OAM_ZERO};
	UInt8 offset = OAM_ZERO;
	UInt8 loop_index = OAM_ZERO;
	rrm_oam_cell_plmn_info_t *input_plmn =
		(rrm_oam_cell_plmn_info_t*)p_input_plmnid;
	Char8 final[MAX_PLMN_ID_BYTES] = {OAM_ZERO};

	if(OAM_TWO == input_plmn->num_mnc_digit) {
		temp_plmn[offset] = input_plmn->mcc[OAM_ONE];
		++offset;
		temp_plmn[offset] = input_plmn->mcc[OAM_ZERO];
		++offset;
		temp_plmn[offset] = 0x0F;
		++offset;
		temp_plmn[offset] = input_plmn->mcc[OAM_TWO];
		++offset;
		temp_plmn[offset] = input_plmn->mnc[OAM_ONE];
		++offset;
		temp_plmn[offset] = input_plmn->mnc[OAM_ZERO];
	}
	else {
		temp_plmn[offset] = input_plmn->mcc[OAM_ONE];
		++offset;
		temp_plmn[offset] = input_plmn->mcc[OAM_ZERO];
		++offset;
		temp_plmn[offset] = input_plmn->mnc[OAM_ZERO];
		++offset;
		temp_plmn[offset] = input_plmn->mcc[OAM_TWO];
		++offset;
		temp_plmn[offset] = input_plmn->mnc[OAM_TWO];
		++offset;
		temp_plmn[offset] = input_plmn->mnc[OAM_ONE];

	}

	for(offset = OAM_ZERO; offset < MAX_PLMN_ID_BYTES; ++offset) {
		snprintf(final, sizeof(final), "%c%c",
				temp_plmn[loop_index], temp_plmn[loop_index + 1]);
		p_bc_plmnList->plmn_identity[index].plmn_id[offset] |= final[OAM_ZERO];
		p_bc_plmnList->plmn_identity[index].
			plmn_id[offset] <<= OAM_NIBBLE_SIZE_IN_BITS;
		p_bc_plmnList->plmn_identity[index].plmn_id[offset] |= final[OAM_ONE];
		loop_index += 2;
	}

	OAM_FUN_EXIT_TRACE();
	/* SPR 13542 REVIEW COMMENTS END */
}
/* SPR 13542 END */

/* SPR 14016 START */
/****************************************************************************
 * Function Name  : oam_extract_eutran_cid_from_macro_enb_cellidentity
 * Inputs         : p_cellidentity :Pointer to cell identity
 * Outputs        : None
 * Returns        : CID of the cell
 * Description    : This function extracts the CID of an EUTRAN cell with Matched CellIdentiy 
 ****************************************************************************/
	UInt8
oam_extract_cid_from_eutran_cellidentity(UInt8 *p_cellidentity)
{
	/*SPR 21240 Start*/
	UInt8 cid = OAM_ZERO;
	oam_return_et retVal;
	SInt32 lclTemp_value;
	UInt8 cell_count= OAM_ZERO;
	UInt8 cell_identity_cfg[MAX_CELL_IDENTITY_OCTETS] = {OAM_ZERO};
	xmlChar *tag_value = OAM_NULL;
	/*SPR 21240 End*/

	/* Cell Index Modification for HeNB Start */
	if(RRM_ENB_TYPE_HOME == RRM_PROV_REQ.oam_rrm_enb_config_req.enb_id.enb_type) 
	{
		return OAM_ZERO;
	}
	else
	{
		/*SPR 21240 Start*/
		for(cell_count =OAM_ZERO;cell_count < oam_prov_req.num_cells_configured;cell_count++)
		{
			tag_value = oam_xmlNodeGetContent( xml_struct_tr196_g.FAPService[cell_count].CellConfig.Common_1,
					"CellIdentity");
			lclTemp_value = oam_atoi((Char8*)tag_value); 

			retVal = oam_encode_eutran_cellidentity_into_bit_stream(
					cell_identity_cfg,
					lclTemp_value);
			if(OAM_FAILURE == retVal) {
				OAM_FUN_EXIT_TRACE();
				return OAM_ZERO;
			}

			OAM_LOG(OAM, OAM_DETAILED, "Reading CellIdentity: %s", tag_value);

			if((cell_identity_cfg[OAM_THREE]==
						p_cellidentity[OAM_THREE])&&
					(cell_identity_cfg[OAM_TWO]==
					 p_cellidentity[OAM_TWO])&&          
					(cell_identity_cfg[OAM_ONE]==
					 p_cellidentity[OAM_ONE])&&               
					(cell_identity_cfg[OAM_ZERO]==
					 p_cellidentity[OAM_ZERO])) 
			{
				cid =cell_count;
				/*SPR 12260 FIX START*/
				OAM_LOG(OAM, OAM_DETAILED,"CellIdentity [%d %d %d %d] of Cell_Index : %d Matched with  EUTRAN CellIdentity [%d %d %d %d]", 
						cell_identity_cfg[OAM_THREE],
						cell_identity_cfg[OAM_TWO],
						cell_identity_cfg[OAM_ONE],
						cell_identity_cfg[OAM_ZERO],
						cid,
						p_cellidentity[OAM_THREE],
						p_cellidentity[OAM_TWO],
						p_cellidentity[OAM_ONE],
						p_cellidentity[OAM_ZERO]
				       );
				/*SPR 12260 FIX END*/
				return cid;
			}
		}
		OAM_LOG(OAM, OAM_DETAILED,"CellIdentity [%d %d %d %d]  NOT  Matched with any EUTRAN CellIdentity [%d %d %d %d] ",
				cell_identity_cfg[OAM_THREE],
				cell_identity_cfg[OAM_TWO],
				cell_identity_cfg[OAM_ONE],
				cell_identity_cfg[OAM_ZERO],
				cid,
				p_cellidentity[OAM_THREE],
				p_cellidentity[OAM_TWO],
				p_cellidentity[OAM_ONE],
				p_cellidentity[OAM_ZERO]
		       );
		return OAM_ZERO;
		/*SPR 21240 End*/
	}/* Cell Index Modification for HeNB Start */
}
/* SPR 14016 END */

/* SPR 14153 FIX START */
/***************************************************************************
 * FUNCTION NAME:      oam_xml_add_first_child 
 * DESCRIPTION  :      This function adds a new immediate child to given
 *                     parent
 * Inputs       :      p_parent: pointer to parent
 *                     child_name: name of the child to be added
 *              :      content: pointer to contents of the new child, if any
 * Output       :      NONE
 * RETURNS      :      pointer to new child
 ****************************************************************************/
xmlNode* oam_xml_add_first_child(xmlNode *p_parent, Char8* child_name,
		Char8* content)
{
	xmlNode *p_child = OAM_NULL;
	xmlNode *p_first = OAM_NULL;

	if (!p_parent || !child_name) {
		return p_child;
	}

	p_first = p_parent->children;

	if (p_first) {
		p_child = oam_xml_add_prev_sibling(p_first, child_name, content);
	}
	else {
		p_child = xmlNewChild(p_parent, OAM_NULL,
				(xmlChar*)child_name, (xmlChar*)content);
	}

	return p_child;
}


/***************************************************************************
 * FUNCTION NAME:      oam_xml_add_prev_sibling 
 * DESCRIPTION  :      This function adds a new sibling node just before the
 *                     given node in subtree
 * Inputs       :      p_node: pointer to node
 *                     sibling_name: name of the sibling to be added
 *              :      content: pointer to contents of the new sibling, if any
 * Output       :      NONE
 * RETURNS      :      pointer to new child
 ****************************************************************************/
xmlNode* oam_xml_add_prev_sibling(xmlNode *p_node, Char8* sibling_name,
		Char8* content)
{
	xmlNode *p_sibling = OAM_NULL;
	xmlNode *p_tmp = OAM_NULL;

	if (!p_node || !sibling_name) {
		return p_sibling;
	}

	p_tmp = xmlNewNode(OAM_NULL, (xmlChar*)sibling_name);
	if (p_tmp) {
		if (content) {
			xmlNodeSetContent(p_tmp, (xmlChar*)content);
		}

		p_sibling = xmlAddPrevSibling(p_node, p_tmp);

		if (!p_sibling) {
			xmlFreeNode(p_tmp);
		}
	}

	return p_sibling;
}


/***************************************************************************
 * FUNCTION NAME:      oam_xml_add_next_sibling 
 * DESCRIPTION  :      This function adds a new sibling node just after the
 *                     given node in subtree
 * Inputs       :      p_node: pointer to node
 *                     sibling_name: name of the sibling to be added
 *              :      content: pointer to contents of the new sibling, if any
 * Output       :      NONE
 * RETURNS      :      pointer to new child
 ****************************************************************************/
xmlNode* oam_xml_add_next_sibling(xmlNode *p_node, Char8* sibling_name,
		Char8* content)
{
	xmlNode *p_sibling = OAM_NULL;
	xmlNode *p_tmp = OAM_NULL;

	if (!p_node || !sibling_name) {
		return p_sibling;
	}

	p_tmp = xmlNewNode(OAM_NULL, (xmlChar*)sibling_name);

	if (p_tmp) {
		if (content) {
			xmlNodeSetContent(p_tmp, (xmlChar*)content);
		}

		p_sibling = xmlAddNextSibling(p_node, p_tmp);

		if (!p_sibling) {
			xmlFreeNode(p_tmp);
		}
	}

	return p_sibling;
}
/* SPR 14153 FIX END */
/*eICIC_PHASE_1_2_CHANGES_START*/
/************************************************************************
 *Function Name  : oam_conversion_abs_pattern_byte_string_to_bitstring 
 *Inputs         : p_byte_string,p_bit_string
 *Outputs        : p_bit_string: encoded byte array
 *Returns        : OAM_SUCCESS/OAM_FAILURE
 *Description    : This function encodes the byte pattern string into byte array
 ****************************************************************************/
oam_return_et 
	oam_conversion_abs_pattern_byte_string_to_bitstring
(Char8 *p_byte_string , UInt8* p_bit_string)
{
	OAM_FUN_ENTRY_TRACE();
	UInt8 temp_array[OAM_TWO] = {OAM_ZERO};
	UInt8 index=OAM_ZERO, index1 =OAM_ZERO ,
	      or_mask = OAM_ZERO ,bit_count = OAM_ZERO,
	      value = OAM_ZERO; 
	SInt8  byte_pattern_length = oam_strlen(p_byte_string);

	/*SPR 14354 start */
	/*bug_14899_start*/
	oam_memset((void *)p_bit_string, OAM_ZERO, (RRM_MAX_PATTERN_BYTE));
	/*bug_14899_end*/

	if (byte_pattern_length< OAM_RRM_eICIC_MIN_ABS_PATTERN_LENGTH ||
			byte_pattern_length > OAM_RRM_eICIC_MAX_ABS_PATTERN_LENGTH)
		/*SPR 14354 end */          
	{
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	} 


	while(byte_pattern_length > OAM_ZERO)
	{
		or_mask = OAM_BYTE_SIZE_IN_BITS - OAM_ONE; 
		for(index =OAM_ZERO; index< (byte_pattern_length < OAM_BYTE_SIZE_IN_BITS 
					? byte_pattern_length : OAM_BYTE_SIZE_IN_BITS); index++)
		{
			oam_strncpy((Char8*)temp_array,
					p_byte_string+bit_count+index,OAM_ONE);

			value = (oam_atoi((Char8 *)temp_array));
			if(value != OAM_ZERO)
			{
				p_bit_string[index1] |= value * (OAM_ONE << or_mask);
			}
			or_mask--;
		}
		index1 ++;
		bit_count += OAM_BYTE_SIZE_IN_BITS;
		byte_pattern_length -= OAM_BYTE_SIZE_IN_BITS;
	}
	OAM_FUN_EXIT_TRACE();
	return OAM_SUCCESS;
}

/*******************************************************************************
 *Function Name  : oam_conversion_abs_pattern_bit_string_to_bytestring 
 *Inputs         : p_byte_string,p_bit_string
 *Outputs        : p_byte_string: decoded byte array
 *Returns        : OAM_SUCCESS/OAM_FAILURE
 *Description    : This function decodes the byte pattern string into byte array
 *******************************************************************************/
oam_return_et 
	oam_conversion_abs_pattern_bit_string_to_bytestring
(Char8 *p_byte_string , UInt8* p_bit_string)
{
	OAM_FUN_ENTRY_TRACE();

	UInt8 byte_pos = OAM_ZERO; 
	UInt8 temp_val = OAM_ZERO; 
	/*bug_13968_start */
	SInt8 index = OAM_ZERO; 
	/*bug_13968_end */
	UInt8 count = OAM_ZERO; 
	UInt8 total_bytes_size = OAM_ZERO;

	if(OAM_ZERO == oam_strncmp((const char *)oam_prov_req.oam_tr069_req.tr069_init_params.DuplexMode
				,"TDDMode",oam_strlen("TDDMode")+1))
	{
		total_bytes_size = OAM_RRM_MAX_ABS_PATTERN_SIZE_TDD;
	}
	else
	{
		total_bytes_size = OAM_RRM_MAX_ABS_PATTERN_SIZE_FDD;
	}
	while(byte_pos < total_bytes_size )
	{
		for (index = OAM_SEVEN; index >= OAM_ZERO; index--)
		{
			temp_val = p_bit_string[byte_pos] >> index;

			if (temp_val & OAM_ONE)
				*(p_byte_string + count) = '1';
			else
				*(p_byte_string + count) = '0';

			count++;
		}
		byte_pos++;
	}
	*(p_byte_string + count) = '\0';

	/*CID:69237 start*/
	OAM_FUN_EXIT_TRACE();
	/*CID:69237 end*/
	return OAM_SUCCESS;
} 

/*eICIC_PHASE_1_2_CHANGES_END */



/* SPR 16200 FIX START */
/*******************************************************************************
 *Function Name  : oam_validate_abs_pattern 
 *Inputs         : abs_pattern: abs pattern,
                   dup_mode: duplex mode TDD/FDD
                   tdd_config: UL/DL configuraton if duplex mode is TDD
 *Outputs        : NONE
 *Returns        : OAM_SUCCESS/OAM_FAILURE
 *Description    : This function validates the ABS pattern length according to
                   duplex mode
 ******************************************************************************/
oam_return_et oam_validate_abs_pattern(Char8* abs_pattern,
                                       Char8* dup_mode,
                                       rrm_oam_sub_frame_asgmnt_et tdd_config)
{
    OAM_FUN_ENTRY_TRACE();
    oam_return_et retVal = OAM_SUCCESS;
    /*SPR 16544 START */
    OAM_LOG(OAM, OAM_INFO, "ABS pattern length: %d", oam_strlen(abs_pattern)); 
    /*SPR 16544 END*/
    if (!oam_strcmp(dup_mode, "FDDMode")) {
        if (oam_strlen(abs_pattern) > OAM_FDD_MODE_ABS_PATTERN_MAX_LEN) {
            retVal = OAM_FAILURE;
        }
    }
    else if (!oam_strcmp(dup_mode, "TDDMode")) {
        if ((RRM_OAM_SA0 == tdd_config) &&
             (oam_strlen(abs_pattern) >
                         OAM_TDD_MODE_SF_CONFIG0_ABS_PATTERN_MAX_LEN)) {
            OAM_LOG(OAM, OAM_WARNING,
                         "TDD ABS pattern value must not be "
                         "greater than %d-bit long for UL/DL config-0",
                         OAM_TDD_MODE_SF_CONFIG0_ABS_PATTERN_MAX_LEN);

            retVal = OAM_FAILURE;
        }
        else if ((RRM_OAM_SA1 <= tdd_config && RRM_OAM_SA5 >= tdd_config) &&
                 (oam_strlen(abs_pattern) >
                           OAM_TDD_MODE_SF_CONFIG1_5_ABS_PATTERN_MAX_LEN)) {
            OAM_LOG(OAM, OAM_WARNING,
                         "TDD ABS pattern value must not be greater "
                         "than %d-bit long for UL/DL config-1 to config-5",
                         OAM_TDD_MODE_SF_CONFIG1_5_ABS_PATTERN_MAX_LEN);

            retVal = OAM_FAILURE;
        }
        else if ((RRM_OAM_SA6 == tdd_config) &&
                 (oam_strlen(abs_pattern) >
                             OAM_TDD_MODE_SF_CONFIG6_ABS_PATTERN_MAX_LEN)) {
            OAM_LOG(OAM, OAM_WARNING,
                         "TDD ABS pattern value must not be "
                         "greater than %d-bit long for UL/DL config-6",
                         OAM_TDD_MODE_SF_CONFIG6_ABS_PATTERN_MAX_LEN);

            retVal = OAM_FAILURE;
        }
    }
    else {
        OAM_LOG(OAM, OAM_WARNING, "Invalid dup_mode: %s", dup_mode);
        retVal = OAM_FAILURE;
    }

    OAM_FUN_EXIT_TRACE();
    return retVal;
}
/* SPR 16200 FIX END */
/* SPR 15604 Fix Start */
void oam_encode_rncid_to_cell_identity(UInt8 *p_cell_identity,
                                       UInt16 rncid)
{
    UInt32 offset = 0;
    UInt32 msb_byte_mask = OAM_EXTENDED_RNCID_BITMASK;
    UInt32 msb_byte_offset = 3;

    if(!p_cell_identity) {
        OAM_LOG(OAM, OAM_WARNING, "NULL POINTER ERROR!!!");
        return;
    }

    OAM_LOG(OAM, OAM_DETAILED, "Encoding RNCID: %d into cell identity",
            rncid);

    if(rncid > OAM_12BIT_MAX_VALUE) {
        p_cell_identity[offset] = (rncid & 0xF000) >> OAM_BYTE_SIZE_IN_BITS;
        p_cell_identity[offset] |= (rncid & 0xF00) >> OAM_BYTE_SIZE_IN_BITS;
        OAM_LOG(OAM, OAM_DETAILED, "Cell Identity[%d]: %d", offset,
                p_cell_identity[offset]);

        ++offset;
        p_cell_identity[offset] = rncid & 0xF0;
        p_cell_identity[offset] |= rncid & 0xF;
        OAM_LOG(OAM, OAM_DETAILED, "Cell Identity[%d]: %d", offset,
                p_cell_identity[offset]);

        msb_byte_mask >>= msb_byte_offset * OAM_BYTE_SIZE_IN_BITS;
        msb_byte_mask >>= OAM_NIBBLE_SIZE_IN_BITS;
        p_cell_identity[msb_byte_offset] |= msb_byte_mask;
        OAM_LOG(OAM, OAM_DETAILED,
                "Setting the extended-RNCID indicator bit");
    }
    else {
        p_cell_identity[offset] = (rncid & 0xF00) >> OAM_NIBBLE_SIZE_IN_BITS;
        p_cell_identity[offset] |= (rncid & 0xF0) >> OAM_NIBBLE_SIZE_IN_BITS;
        OAM_LOG(OAM, OAM_DETAILED, "Cell Identity[%d]: %d", offset,
                p_cell_identity[offset]);
        ++offset;
        p_cell_identity[offset] |= (rncid & 0xF) << OAM_NIBBLE_SIZE_IN_BITS;
        OAM_LOG(OAM, OAM_DETAILED, "Cell Identity[%d]: %d", offset,
                p_cell_identity[offset]);
    }
}
void oam_encode_cid_to_cell_identity(UInt8 *p_cell_identity,
                                     UInt16 cid)
{
    UInt32 offset = 1;
    UInt32 msb_byte_mask = OAM_EXTENDED_RNCID_BITMASK;
    UInt32 msb_byte_offset = 3;

    if(!p_cell_identity) {
        OAM_LOG(OAM, OAM_WARNING, "NULL POINTER ERROR!!!");
        return;
    }

    OAM_LOG(OAM, OAM_DETAILED, "Encoding CID: %d into cell identity", cid);

    msb_byte_mask >>= msb_byte_offset * OAM_BYTE_SIZE_IN_BITS;
    msb_byte_mask >>= OAM_NIBBLE_SIZE_IN_BITS;

    if(!(p_cell_identity[msb_byte_offset] & msb_byte_mask)) {
        p_cell_identity[offset] |= (cid & 0xF000) >> OAM_UMTS_CID_MIN_NUM_BITS;
        OAM_LOG(OAM, OAM_DETAILED, "Cell Identity[%d]: %d", offset,
                p_cell_identity[offset]);

         }
    else {
        OAM_LOG(OAM, OAM_DETAILED, "Extended-RNCID bit is found set");
    }

    ++offset;
    p_cell_identity[offset] = (cid & 0xF00) >> OAM_NIBBLE_SIZE_IN_BITS;
    p_cell_identity[offset] |= (cid & 0xF0) >> OAM_NIBBLE_SIZE_IN_BITS;
    OAM_LOG(OAM, OAM_DETAILED, "Cell Identity[%d]: %d", offset,
            p_cell_identity[offset]);

    ++offset;
    p_cell_identity[offset] |= (cid & 0xF) << OAM_NIBBLE_SIZE_IN_BITS;
    OAM_LOG(OAM, OAM_DETAILED, "Cell Identity[%d]: %d", offset,
            p_cell_identity[offset]);
}
/* SPR 15604 Fix Start */

/* SPR-16334 START */

/******************************************************************************
 * Function Name  : oam_print_plmn_id
 * Inputs         : p_plmn_id    Pointer to the plmn Id
 * Outputs        : None
 * Returns        : None
 * Description    : This function prints the value of Plmn Id
 ****************************************************************************/
oam_void_t
oam_print_plmn_id
(
    oam_cell_plmn_info_t *p_plmn_id
)
{
    UInt8 index = 0;

    OAM_FUN_ENTRY_TRACE();

    if (OAM_NULL == p_plmn_id)
    {
        OAM_LOG(OAM,OAM_WARNING,
                "oam_print_plmn_id: Could not "
                "print PLMN Id, p_plmn_id is NULL");
    }
    else
    {
        for (index = 0; index < OAM_MCC_OCTET_SIZE; index++)
        {
            OAM_LOG(OAM, OAM_DETAILED,
                    "mcc[%u]: %u",
                    index,
                    (UInt16)p_plmn_id->mcc[index]);
        }

        OAM_LOG(OAM, OAM_DETAILED,
                "Number of mnc digits: %u",
                (UInt16)p_plmn_id->num_mnc_digit);
        for (index = 0; index < p_plmn_id->num_mnc_digit; index++)
        {
            OAM_LOG(OAM, OAM_DETAILED,
                    "mnc[%u]: %u",
                    index,
                    (UInt16)p_plmn_id->mnc[index]);
        }
    }
    OAM_FUN_EXIT_TRACE();

} /* oam_print_plmn_id */



/* BCOM Compiltaion Fix */
#ifndef OAM_SON_ENABLE

/******************************************************************************
 * Function Name  : oam_print_enb_id
 * Inputs         : p_enb_id    Pointer to the eNB Id
 * Outputs        : None
 * Returns        : None
 * Description    : This function prints the value of eNB Identifier
 ****************************************************************************/
oam_void_t
oam_print_enb_id
(
    oam_global_enb_id_t *p_enb_id
)
{
    UInt8 index = 0;

    OAM_FUN_ENTRY_TRACE();

    if (OAM_NULL == p_enb_id)
    {
        OAM_LOG(OAM, OAM_WARNING,
                "oam_print_enb_id: Could not "
                "print eNB Id, p_enb_id is NULL");
    }
    else
    {
        OAM_LOG(OAM, OAM_DETAILED,
                "--- eNB Id value ---");
        OAM_LOG(OAM, OAM_DETAILED,
                "eNB Type: %u", p_enb_id->enb_type);
        oam_print_plmn_id(&p_enb_id->plmn_id);
        for (index = 0; index < OAM_ENB_ID_OCTET_SIZE; index++)
        {
            OAM_LOG(OAM, OAM_DETAILED,
                    "eNB Id[%u]: %u",
                    index,
                    (UInt16)p_enb_id->enb_id[index]);
        }
    }

    OAM_FUN_EXIT_TRACE();
} /* oam_print_enb_id */
#endif
/* BCOM Compiltaion Fix */
/*****************************************************************************
 * Function Name  : oam_x2_convert_l3_plmn_id_to_oam_plmn_id
 * Inputs         : p_l3_plmn_id    - Pointer to plmn id for l3
 *                  p_oam_plmn_id   - Pointer to plmn id for OAM
 * Outputs        : None
 * Returns        : None
 * Description    : This is the function which is used to convert PLMN value
 *                  provided by L3 X2AP to the PLMN value required by OAM
 ****************************************************************************/
oam_void_t
oam_x2_convert_l3_plmn_id_to_oam_plmn_id
(
    x2ap_plmn_identity_t *p_l3_plmn_id,
    oam_cell_plmn_info_t *p_oam_plmn_id
)
{
    /* As per TS 36.423 :
     * PLMN Id consists of 3 bit string where
     * - digits 0 to 9, encoded 0000 to 1001,
     * - 1111 used as filler digit, two digits per octet,
     * - bits 4 to 1 of octet n encoding digit 2n-1
     * - bits 8 to 5 of octet n encoding digit 2n
     *
     * -The Selected PLMN identity consists of 3 digits from MCC followed by either
     *  -a filler digit plus 2 digits from MNC (in case of 2 digit MNC) or
     *  -3 digits from MNC (in case of a 3 digit MNC). */

    UInt8 temp =  p_l3_plmn_id->plmn_id[1] >> 4;

    p_oam_plmn_id->mcc[0] = 0x0F & p_l3_plmn_id->plmn_id[0];
    p_oam_plmn_id->mcc[1] = p_l3_plmn_id->plmn_id[0] >> 4;
    p_oam_plmn_id->mcc[2] = 0x0F & p_l3_plmn_id->plmn_id[1];

    /* Check if filler digit is present */
    if (0x0F == temp)
    {
        p_oam_plmn_id->num_mnc_digit = 2;
        p_oam_plmn_id->mnc[0] = 0x0F & p_l3_plmn_id->plmn_id[2];
        p_oam_plmn_id->mnc[1] = p_l3_plmn_id->plmn_id[2] >> 4;
    }
    else
    {
        p_oam_plmn_id->num_mnc_digit = 3;
        p_oam_plmn_id->mnc[0] = temp;
        p_oam_plmn_id->mnc[1] = 0x0F & p_l3_plmn_id->plmn_id[2];
        p_oam_plmn_id->mnc[2] = p_l3_plmn_id->plmn_id[2] >> 4;
    }
} /* oam_x2_convert_l3_plmn_id_to_oam_plmn_id */

/* BCOM Compiltaion Fix */
#ifndef OAM_SON_ENABLE

/*****************************************************************************
 * Function Name  : oam_x2_convert_l3_enbid_to_oam_enbid
 * Inputs         : p_l3_enbid - Pointer to the eNB Id value defined by L3 X2AP
 * Outputs        : p_oam_enbid - Pointer to the eNB Id value defined by OAM
 * Returns        : None
 * Description    : This is the function which is used to convert eNB Id value
 *                  provided by L3 X2AP to the eNB Id value required by OAM
 ****************************************************************************/
oam_void_t  
oam_x2_convert_l3_enbid_to_oam_enbid
(
    x2_gb_enb_id_t      *p_l3_enbid, 
    oam_global_enb_id_t *p_oam_enbid
)           
{           
    oam_x2_convert_l3_plmn_id_to_oam_plmn_id(
        &p_l3_enbid->plmn_identity, &p_oam_enbid->plmn_id);
        
    if (X2AP_OAM_MACRO_ENB_ID_PRESENT & p_l3_enbid->enb_id.presence_bitmask)
    {       
        p_oam_enbid->enb_type = OAM_MACRO_ENB;
        oam_memcpy(p_oam_enbid->enb_id,
            p_l3_enbid->enb_id.macro_enb_id.eNB_id,
            MACRO_ENB_ID_OCTET_SIZE);
    }       
    else if (X2AP_OAM_HOME_ENB_ID_PRESENT & p_l3_enbid->enb_id.presence_bitmask)
    {
        p_oam_enbid->enb_type = OAM_HOME_ENB;
        oam_memcpy(p_oam_enbid->enb_id, 
            p_l3_enbid->enb_id.home_enb_id.eNB_id,
            HOME_ENB_ID_OCTET_SIZE);
    }

} /* oam_x2_convert_l3_enbid_to_oam_enbid */
#endif
/* BCOM Compiltaion Fix */
/******************************************************************************
 * Function Name  : oam_x2_convert_ip_addr_to_l3_ip_addr
 * Inputs         : p_oam_ip_addr - Pointer to IP address in the notation used by OAM.
 * Outputs        : p_l3_ip_addr - Pointer to the IP address in the notation 
 *                  used by L3 X2AP
 * Returns        : None
 * Description    : This function converts the IP address from the notation used by 
 *                  OAM to the notation used by L3 X2AP.
 *****************************************************************************/
oam_void_t      
oam_x2_convert_ip_addr_to_l3_ip_addr
(               
    UInt8  *p_oam_ip_addr,
    UInt8  *p_l3_ip_addr
)                       
{                               
    UInt8 index = 0, num_of_digits_read = 0;
                        
    OAM_FUN_ENTRY_TRACE();
    if (OAM_NULL == p_oam_ip_addr ||
        OAM_NULL == p_l3_ip_addr)
    {                       
        OAM_LOG(OAM, OAM_WARNING,  
                "oam_x2_convert_ip_addr_to_l3_ip_addr: Could not "
                "convert IP address, p_oam_ip_addr or "
                "p_l3_ip_addr is NULL");
        OAM_FUN_EXIT_TRACE();
        return;             
    }                       
                        
    oam_memset(p_l3_ip_addr, 0, MAX_X2AP_IP_ADDRESS_LENGTH);
    for (index = 0; index < OAM_IPV4_SIZE &&
        num_of_digits_read < MAX_X2AP_IP_ADDRESS_LENGTH; index++)
    {           
        /* Use 4 as the size for snprintf as IP address octet can have
         *  maximum of 3 digits + 1 for storing NULL character */
        if (0 < oam_snprintf((char *)p_l3_ip_addr + num_of_digits_read, 4,
            "%u", (UInt16)(p_oam_ip_addr[index])))
        {
            num_of_digits_read = oam_strlen((const char *)p_l3_ip_addr);
            p_l3_ip_addr[num_of_digits_read] = '.';
            num_of_digits_read++;
            p_l3_ip_addr[num_of_digits_read] = '\0';
        }
    }

    p_l3_ip_addr[num_of_digits_read - 1] = '\0';

    OAM_FUN_EXIT_TRACE();
} /* oam_x2_convert_ip_addr_to_l3_ip_addr */

/******************************************************************************
 * Function Name  : oam_x2_convert_ipv6_addr_to_l3_ipv6_addr
 * Inputs         : p_oam_ipv6_addr - Pointer to IP address in the notation used by OAM.
 * Outputs        : p_l3_ipv6_addr - Pointer to the IP address in the notation
 *                  used by L3 X2AP
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function converts the IP address from the notation used by
 *                  OAM to the notation used by L3 X2AP.
 *****************************************************************************/
oam_return_et   
oam_x2_convert_ipv6_addr_to_l3_ipv6_addr
(                       
    UInt8  *p_oam_ipv6_addr,
    UInt8  *p_l3_ipv6_addr
)                   
{               
    oam_return_et   ret_val = OAM_SUCCESS;
    UInt8           index   = OAM_ZERO;
                    
    OAM_FUN_ENTRY_TRACE();
                        
    if (OAM_NULL == p_oam_ipv6_addr ||
        OAM_NULL == p_l3_ipv6_addr)
    {                   
        OAM_LOG(OAM, OAM_WARNING,
            "oam_x2_convert_ipv6_addr_to_l3_ipv6_addr: Could not "
            "convert IP address, p_oam_ip_addr or "
            "p_l3_ipv6_addr is NULL");
        OAM_FUN_EXIT_TRACE();
        ret_val = OAM_FAILURE;
    }
    else
    {                       
        for (index = 0; index < OAM_IPV6_SIZE; index++)
        {                   
            OAM_LOG(OAM, OAM_DETAILED,
                    "OAM IPv6 addr[%u]: %u", index, p_oam_ipv6_addr[index]);
        }           
                
        oam_memset(p_l3_ipv6_addr, '\0', MAX_X2AP_IPV6_ADDRESS_LENGTH);

        if(OAM_NULL == oam_inet_ntop(AF_INET6, (const oam_void_t *)p_oam_ipv6_addr,
                             (char *)p_l3_ipv6_addr, MAX_X2AP_IPV6_ADDRESS_LENGTH))
        {
            OAM_LOG(OAM, OAM_WARNING,
                    "oam_x2_convert_ipv6_addr_to_l3_ipv6_addr: Failed to convert "
                    "OAM IPv6 address to L3 IPv6 address :: errno = %d", errno);
            ret_val = OAM_FAILURE;
        }
        else
        {
            OAM_LOG(OAM, OAM_DETAILED,
                    "oam_x2_convert_ipv6_addr_to_l3_ipv6_addr: Converted "
                    "to L3 IPv6 address = %s", p_l3_ipv6_addr);
            ret_val = OAM_SUCCESS;
        }
    }
    OAM_FUN_EXIT_TRACE();
    return ret_val;
} /* oam_x2_convert_ipv6_addr_to_l3_ipv6_addr */

/******************************************************************************
 * Function Name  : oam_x2_convert_l3_ipv6_addr_to_oam_ipv6_addr
 * Inputs         : p_l3_ipv6_addr - Pointer to IP address received from L3 X2AP
 * Outputs        : p_oam_ipv6_addr - Pointer to the IP address in the notation
 *                  used by OAM.
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function converts the IP address from the notation used by
 *                  L3 X2AP to the notation used by OAM.
 *****************************************************************************/
oam_return_et
oam_x2_convert_l3_ipv6_addr_to_oam_ipv6_addr
(                   
    UInt8  *p_l3_ipv6_addr,
    UInt8  *p_oam_ipv6_addr
)               
{               
    oam_return_et   ret_val         = OAM_SUCCESS;
    UInt8           convert_ret_val = OAM_ZERO;
    UInt8           index           = OAM_ZERO;
    
        
    OAM_FUN_ENTRY_TRACE();
    if (OAM_NULL == p_oam_ipv6_addr ||
        OAM_NULL == p_l3_ipv6_addr)
    {   
        OAM_LOG(OAM, OAM_WARNING,
            "oam_x2_convert_l3_ipv6_addr_to_oam_ipv6_addr: Could not "
            "convert IP address, p_oam_ip_addr or p_l3_ipv6_addr is NULL");
                    
        ret_val = OAM_FAILURE;
    }       
    else if(OAM_SUCCESS != (convert_ret_val = inet_pton(AF_INET6, (const char *)p_l3_ipv6_addr, p_oam_ipv6_addr)))
    {
        if(OAM_ZERO == convert_ret_val)
        {
            OAM_LOG(OAM, OAM_WARNING,
                    "oam_x2_convert_l3_ipv6_addr_to_oam_ipv6_addr: Failed to convert "
                    "L3 IPv6 address = %s to OAM format.Invalid presentation format"
                    "received from L3", p_l3_ipv6_addr);
        }
        else
        {
            OAM_LOG(OAM,OAM_WARNING,
                "oam_x2_convert_l3_ipv6_addr_to_oam_ipv6_addr: Failed to convert "
                "L3 IPv6 address = %s to OAM format:: errno = %d", p_l3_ipv6_addr, errno);
        }
        ret_val = OAM_FAILURE;
    }
    else
    {
        OAM_LOG(OAM, OAM_DETAILED,
                "oam_x2_convert_l3_ipv6_addr_to_oam_ipv6_addr: Converted "
                "L3 IPv6 address = %s to OAM IPv6 address format = ", p_l3_ipv6_addr);

        for (index = 0; index < OAM_IPV6_SIZE; index++)
        {
            OAM_LOG(OAM,OAM_DETAILED,
                    "IPv6 addr[%u]: %u", index, p_oam_ipv6_addr[index]);
        }

        ret_val = OAM_SUCCESS;
    }

    OAM_FUN_EXIT_TRACE();
    return ret_val;
} /* oam_x2_convert_l3_ipv6_addr_to_oam_ipv6_addr */

/******************************************************************************
 * Function Name  : oam_x2_convert_l3_ip_addr_to_oam_ip_addr
 * Inputs         : p_l3_ip_addr - Pointer to IP address received from L3 X2AP
 * Outputs        : p_oam_ip_addr - Pointer to the IP address in the notation 
 *                  used by OAM.
 * Returns        : None
 * Description    : This function converts the IP address from the notation used by 
 *                  L3 X2AP to the notation used by OAM.
 *****************************************************************************/
oam_void_t
oam_x2_convert_l3_ip_addr_to_oam_ip_addr
(
    UInt8  *p_l3_ip_addr,
    UInt8  *p_oam_ip_addr
)
{
    UInt8 *p_ip_addr_octet = OAM_NULL;
    UInt8 index = 0;


    OAM_FUN_ENTRY_TRACE();

    if (OAM_NULL == p_oam_ip_addr ||
        OAM_NULL == p_l3_ip_addr)
    {
        OAM_LOG(OAM, OAM_WARNING,
                "oam_x2_convert_l3_ip_addr_to_oam_ip_addr: Could not "
                "convert IP address, p_oam_ip_addr or "
                "p_l3_ip_addr is NULL");
        OAM_FUN_EXIT_TRACE();
        return;
    }

    p_ip_addr_octet = (UInt8 *)oam_strtok((char *)p_l3_ip_addr, ".");
    while (OAM_NULL != p_ip_addr_octet)
    {
        p_oam_ip_addr[index] = oam_atoi((char *)p_ip_addr_octet);
        index++;
        p_ip_addr_octet = (UInt8 *)oam_strtok(OAM_NULL, ".");
    }
    OAM_FUN_EXIT_TRACE();

} /* oam_x2_convert_l3_ip_addr_to_oam_ip_addr */

/* SPR-16334 END */

/*************************************************************************
 * Function Name  : oam_popen
 * Description    : This is a wrapper function to popen
 * Inputs         : Command, read/write mode
 * Outputs        : none
 * Returns        : Return type to system call
 ******************************************************************************/
FILE *oam_popen(const Char8 *p_cmd, const Char8 *p_mode)
{
    return popen(p_cmd, p_mode);
}

/*************************************************************************
 * Function Name  : oam_pclose
 * Description    : This is a wrapper function for pclose
 * Inputs         : File stream
 * Outputs        : none
 * Returns        : Return type to system call
 ******************************************************************************/
int oam_pclose(FILE *p_stream)
{
    return pclose(p_stream);
}

/*************************************************************************
 * Function Name  : oam_get_cksum
 * Description    : This is get the checksum of the file name passed
 * Inputs         : Filename
 * Outputs        : Cksum
 * Returns        : OAM_AGNT_SUCCESS/OAM_AGNT_FAILURE
 ******************************************************************************/
oam_return_et
oam_fill_cksum(Char8 *p_filename, UInt64 *p_cksum)
{
    FILE *fp;
    Char8 line[OAM_MAX_CMD_LENGTH_NAME];
    Char8 cmd[OAM_MAX_CMD_LENGTH_NAME];
    /* SPR 22704 Fix Start */
    *p_cksum = OAM_ZERO;
    /* SPR 22704 Fix End */

    snprintf(cmd, OAM_MAX_CMD_LENGTH_NAME, "cksum %s", p_filename);

    fp = oam_popen(cmd, "r");
    /* SPR 21889: CID 115183 Start */
    if (OAM_NULL != fp)
    {
	    if (fgets(line, sizeof(line), fp) != OAM_NULL)
	    {
		    *p_cksum = oam_atoi(oam_strtok(line, " "));
	    }
	    oam_pclose(fp);
        return OAM_SUCCESS;
    }
    /* SPR 22704 Fix +- */
    return OAM_FAILURE;
    /* SPR 21889: CID 115183 End */
}
/*SPR 22465 Fix Start*/
/****************************************************************************
 * Function Name  : oam_start_new_timer_and_maintain_trans_id
 * Inputs         : timer_duration,p_timer_data,timer_data_size,is_repeated,
 *                  trans_id
 * Outputs        : none
 * Returns        : oam_timer_t
 * Description    : This function strats the timer and maintains the transaction
 *                  id.
 ****************************************************************************/
	oam_timer_t
oam_start_new_timer_and_maintain_trans_id(UInt16       sub_module_id,
		UInt16       expiry_thread_id,
		UInt16       timer_type,
		UInt32       timer_duration, /* unit: milliseconds */
		void         *buf,
		UInt16       buf_size,
		oam_bool_et  is_repeated,
        UInt16       trans_id)
{
	QTIME qtime = {OAM_ZERO};
	QMODULE module = {OAM_ZERO};
	oam_timer_t timer  = PNULL;
	oam_timer_data_t  *p_timer_data;
	oam_error_code_et error_code = NO_ERROR;

	p_timer_data = oam_msg_mem_get(sizeof(oam_timer_data_t));
	if(p_timer_data == PNULL) {
		OAM_LOG(OAM,OAM_ERROR, "Not enough memory");
		return PNULL;
	}

	p_timer_data->module_id = sub_module_id;
	p_timer_data->timer_type = timer_type;
	p_timer_data->buf = buf;
	p_timer_data->buf_size = buf_size;
	p_timer_data->trans_id = trans_id;

	qtime.s = timer_duration / OAM_MSEC_IN_ONE_SEC;
	qtime.us = (timer_duration % OAM_MSEC_IN_ONE_SEC) * OAM_MSEC_IN_ONE_SEC;

	if(OAM_ZERO == expiry_thread_id) {
		timer = qvTimerStart(&qtime, p_timer_data, is_repeated);
	}
	else {
		module = qvGetService(expiry_thread_id);
		timer = qvModuleTimerStart(module, &qtime, (void*)p_timer_data,
				is_repeated, OAM_NULL);
	}

	if(timer == PNULL) {
		OAM_LOG(OAM, OAM_WARNING, "Timer Start failed");
		/* Release the memory allocated for storing the timer data */
		oam_mem_free(p_timer_data, &error_code);
		oam_reset_reason_code(OAM_INTERNAL_FAILURE);
		oam_abort();
	}
	else {
		OAM_LOG(OAM, OAM_BRIEF, "Timer Id [%p]: Duration: [%d]msec",
				timer, timer_duration);
	}

	return timer;
}
/*SPR 22465 Fix End*/








