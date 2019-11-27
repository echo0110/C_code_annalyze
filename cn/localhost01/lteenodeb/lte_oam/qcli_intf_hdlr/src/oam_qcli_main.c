/*******************************************************************************
 * File Name   : oam_qcli_main.c
 *
 * Description : This file contains main function and global data
 *               description.
 * Revision History:
 *
 * Date        Author              Reference              Comments
 * -----       ------              ---------              --------
 * June,2012   Vivek Kumar Dixit   OAM Design Document    Initial Version
 * April,2014  Ankita Jain         OAM Design Document    Cell Traffic Trace                                                               
 * July, 2014  Aditi Singh                                CPU_ZERO to OAM_CPU_ZERO
 *                                                        CPU_SET to OAM_CPU_SET 
 * Aug 2014     Aditi Singh         SPR 13116
 * Copyright (c) 2012, Aricent Inc. All Rights Reserved
 *******************************************************************************/
/*******************************************************************************
 * Includes
 *******************************************************************************/
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <qcli.h>
#include <qutil.h>
#include <qrl.h>
#include <qval.h>
#include <qmacro.h>
#include <oam_qcli_proto.h>
#include<oam_defines.h>
#include<oam_proto.h>
#include<oam_sim_utils.h> 
#include<oam_port_utils.h>

/* CSR 57877 FIx Start */
#include <oam_mgmt_intf_api.h>
/* CSR 57877 FIx End*/
/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern const struct command_t commands[];
extern const QSYSOP my_os;
extern const QWAIT  my_waitstruct;
extern const QSHELL my_shell;
prov_request_t prov_req;
extern mess_send_t message_send;
extern char flag_prompt_gb;
/* + SPR 17439 */
oam_return_et qcli_oam_hdle_rrc_config_resp (void *p_api);
/* - SPR 17439 */
edit_t *M;
interpret_t *L;
/* SPR 1251 FIX START */
void * cli_listener = NULL;
/* SPR 1251 FIX END */

extern UInt8 spv_after_add_status;

#if defined(OAM_MSPD_TARGET) || defined(OAM_FREESCALE_TARGET)
extern int process_signal_flag;

/******************************************************************************
 *   FUNCTION NAME   : alarm_signal_handler
 *   DESCRIPTION     : This function handle alarm signal
 *   Inputs          : signal
 *   Outputs         : NONE
 *   RETURNS         : NONE
 ******************************************************************************/

void alarm_signal_handler(int signal)
{
	if(process_signal_flag==1)
	{
		oam_print_line();
		oam_fprintf(stderr, "Operation Timed Out or EnodeB not working\n");
		oam_print_line();
		message_send.message_send = 0;
		process_signal_flag=0;
		show_prompt();
	}
}
#endif
/*SPR:9224_CHANGE_START*/
extern int timer_flag; 
/*SPR:9224_CHANGE_END*/

/****************************************************************************
  Private Types
 ****************************************************************************/
/******************************************************************************
 *   FUNCTION NAME   : Alloc
 *   DESCRIPTION     : This function alocated memory  
 *   Inputs          : attrsize
 *   Outputs         : NONE
 *   RETURNS         : Allocates the memory.
 ******************************************************************************/

static void *Alloc
(
 QMODULE module,
 unsigned int attrsize
 )
{
    /* SPR 17777 fix */
    qcliHandleUnusedParameter(module);

	return qvMsgAllocBySize(attrsize);
}

/******************************************************************************
 *   FUNCTION NAME   : oam_print_header
 *   DESCRIPTION     : oam_print_header function print header 
 *   Input           : NONE
 *   Outputs         : NONE
 *   RETURNS         : NONE
 ******************************************************************************/


void oam_print_header()
{
	oam_fprintf(stdout,"-------------------------------------------------------------------\n");
	time_t t = OAM_TIME(NULL); 
	/* Coverity FIX 35473 */
	if(OAM_LOCALTIME(&t) == NULL )
	{
                oam_fprintf(stdout,"\nFailure : LOCAL TIME is NULL\n");
		return; 
	}
    /*spr 22474 start*/
    /*+CID 147572 */
    if(gmtime(&t) != NULL)
    {
        oam_fprintf(stdout,"%s",asctime(gmtime(&t)));
    }
    /*-CID 147572 */
    /*spr 22474 end*/
	oam_fprintf(stdout,"-------------------------------------------------------------------\n");
}

/******************************************************************************
 *   FUNCTION NAME   : oam_print_line
 *   DESCRIPTION     : This function print line
 *   Inputs          : NONE
 *   Outputs         : NONE
 *   RETURNS         : NONE
 ******************************************************************************/



void oam_print_line()
{
	oam_fprintf(stdout,"-------------------------------------------------------------------\n");
}
void 
oam_qcli_encode_notif_payload
(
 char *p_ctrl_blk,
 char resp_status,
 UInt16 err_code,
 UInt8 resvd1, 
 UInt32 resvd2,
 UInt8 err_str_len,
 char *err_str
 );

oam_return_et
oam_qcli_notification_ind_resp
(
 UInt8 status,
 oam_error_code_et   error_code,
 UInt16 transaction_id,
 UInt8 *err_string
 );

/******************************************************************************
 *   FUNCTION NAME   : oam_qcli_proc_msg
 *   DESCRIPTION     : This function qcli proc msg
 *   Inputs          : p_api_buf
 *   Outputs         : NONE
 *   RETURNS         : NONE
 ******************************************************************************/
void * oam_qcli_proc_msg(void * p_api_buf)
{

    /* Cov Cid 54893 fix */
    UInt16	     api_id   = 0;
    UInt16	     msg_size = 0;
    oam_mac_get_log_level_resp_t oam_mac_log_level_resp;
#ifdef GTPU_DISABLE
    UInt8 result = OAM_ZERO;
#endif
    UInt8 ret = OAM_ZERO;
    UInt8 showPrompt = OAM_ONE;
    /*UInt8 Tracelvl,TraceFlag;*/
    UInt32 response;
    /* Get the Source ID */

    api_id   = get_word_from_header((UInt8*)(p_api_buf) + 6);
    msg_size = get_word_from_header((UInt8*)(p_api_buf) + 8);

    oam_mac_log_level_req_t oam_mac_log_level;
    Oam_LogLevel_Req_t   oam_rlc_log_level;
    oam_pdcp_log_level_req_t oam_pdcp_log_level;
    oam_relay_Gtpu_Get_Log_Lvl_Req_t oam_gtpu_log_level;
    oam_rrc_set_log_level_resp_t oam_rrc_set_log_level_resp;
    x2ap_oam_set_log_level_resp_t oam_x2ap_set_log_level_resp;
    s1ap_oam_set_log_level_resp_t oam_s1ap_set_log_level_resp;
    oam_rrm_set_log_level_resp_t oam_rrm_set_log_level_resp;

#ifdef OAM_SON_ENABLE
    oam_son_set_log_level_resp_t oam_son_set_log_level_resp;
    oam_son_log_enable_disable_resp_t oam_son_set_log_enb_disb_level_resp;
    /* SPR_13116_CHANGE_START */
    oam_son_get_log_level_resp_t oam_son_get_log_level_resp={OAM_ZERO};
#endif
	/* SPR 13617 START */
    oam_rrc_get_log_level_resp_t oam_rrc_get_log_level_resp;
	/* SPR 13617 END */
    oam_s1ap_get_log_level_resp_t oam_s1ap_get_log_level_resp;
    oam_x2ap_get_log_level_resp_t oam_x2ap_get_log_level_resp;

    oam_rrm_get_log_level_resp_t oam_rrm_get_log_level_resp;
    UInt16           count   = OAM_ZERO;
    /* SPR_13116_CHANGE_END */

    oam_rrc_log_enable_resp_t oam_rrc_log_enable_response;
    oam_x2ap_log_enable_resp_t oam_x2ap_log_enable_response;
    oam_s1ap_log_enable_resp_t oam_s1ap_log_enable_response;
    oam_rrm_cell_reconfig_resp_t oam_rrm_resp;
    /* SPR 55317 - Redundant Code Removed Here */
    oam_get_log_level_l2_all_t log_lvl_all;	
    switch (api_id)
    {
        case CONFIG_RRC_STATS_RESP:
            {
                oam_print_header();
                qcli_oam_hdle_rrc_config_resp(p_api_buf ); 
                oam_fprintf(stdout,"\n"); 
                oam_print_line();    
            }
            break;
        case RLC_GET_STATS_CNF: 
            oam_fprintf(stdout,"\tRLC Stats Recieved from OAM\n\n");
            oam_print_header();
            break;
        case MAC_GET_STATS_CNF:
            oam_fprintf(stdout,"\tMAC stats Received from OAM\n\n");
            oam_print_header();
            break;
            /* CSR_00057386 */
        case MAC_GET_STATUS_CNF:
            oam_fprintf(stdout,"\tMAC stats Received from OAM\n\n");
            oam_print_header();
            break;


        case S1AP_OAM_STATUS_RESP:
            oam_fprintf(stdout,"\tS1AP stats Received from OAM\n\n");
            oam_print_header();
            break;

        case SHOW_STATS_L2_PDCP:
            oam_fprintf(stdout,"\tPDCP stats Received from OAM\n\n");
            oam_print_header();
            break;
            /********************************PM_CODE**************************************/
        case OAM_SET_PARAMETER_VALUES_RESP : 
            ret = oam_interface_resp((UInt8*)p_api_buf);
            /*  Coverity ID : 79019 Fix Start */
            /* SPR 17952 Fix Start */
             if(OAM_SUCCESS == ret)
                showPrompt = OAM_ONE; 
            /* SPR 17952 Fix End */     
            /*  Coverity ID : 79019 Fix End */

            /* OAM BCOM Code Comment Changes Start */
            /* Show prompt only after Delete is success in case SPV fails after ADD */
            if(spv_after_add_status== OAM_ONE && ret == OAM_FAILURE)
                 showPrompt = OAM_ZERO;
            /* OAM BCOM Code Comment Changes End */
            break;

        case OAM_GET_PARAMETER_VALUES_RESP :
            ret =oam_interface_resp((U8*)p_api_buf);
            /*  Coverity ID : 79019 Fix Start */
            /* SPR 17952 Fix Start */
              if(OAM_SUCCESS == ret)
                showPrompt = OAM_ONE;
            /* SPR 17952 Fix End */     
            /*  Coverity ID : 79019 Fix End */
            break;

        case OAM_ENODEB_NOT_ACTIVE_STATE :
            oam_print_line();
            oam_fprintf(stdout, "\tEnodeB is not in working state\n\n");
            oam_print_line();
            break;
        case OAM_CELL_INFO_PARAM_MOD_RESP:
            break;
        case OAM_SIB2_PARAM_MOD_RESP:
            break;
        case OAM_CELL_REST_PARAM_MOD_RESP:
            break;
        case OAM_RRC_STATS_ALL:
            break;
        case OAM_RRC_TIMER_PARAM_MOD_RESP:
            break;
            /* SPR 55317 START */
            /* Redundant Code Removed Here*/
            /* SPR 55317 END */
        case OAM_GET_KPI_MAC_RESP:
            oam_print_header();
            oam_qcli_mac_get_kpi_stat_conf(p_api_buf);
            oam_print_line();
            break;
        case OAM_GET_KPI_PDCP_RESP:
            oam_print_header();
            oam_qcli_pdcp_get_kpi_stat_conf(p_api_buf,msg_size);
            oam_print_line();
            break;
        case OAM_CONFIG_KPI_MAC_RESP:
            oam_print_header();
            oam_qcli_mac_config_kpi_stat_conf(p_api_buf,msg_size);
            oam_print_line();
            break;
        case OAM_CONFIG_KPI_PDCP_RESP:
            oam_print_header();
            oam_qcli_pdcp_config_kpi_stat_conf(p_api_buf,msg_size);
            oam_print_line();
            break;
        case OAM_GET_LOG_CAT_MAC_RESP:
            oam_print_header();
            oam_qcli_get_log_cat_MAC_resp(p_api_buf,msg_size);
            oam_print_line();	
            break;
        case OAM_GET_LOG_CAT_RLC_RESP:
            oam_print_header();
            oam_qcli_get_log_cat_RLC_resp(p_api_buf,msg_size);
            oam_print_line();
            break;
        case OAM_GET_LOG_CAT_PDCP_RESP:
            oam_print_header();
            oam_qcli_get_log_cat_PDCP_resp(p_api_buf,msg_size);
            oam_print_line();
            break;
        case OAM_GET_LOG_CAT_EGTPU_RESP:
            oam_print_header();
            oam_qcli_get_log_cat_GTPU_resp(p_api_buf,msg_size);
            oam_print_line();
            break;
        case OAM_GET_LOG_LEVEL_MAC_RESP: /*implemented*/
            oam_print_header();
             /*SPR 13617 START */
            oam_memcpy(&oam_mac_log_level_resp.logLevel,(char*)(p_api_buf) + OAM_MGMT_INTF_HEADER_SIZE,sizeof(oam_mac_log_level_req_t));
            /*SPR 13617 END */
            /*SPR 14673 start*/
            /*deleted*/
            /*SPR 14673 end*/


            oam_fprintf(stdout, "MAC LOG LEVEL  : ");
            if(!oam_mac_log_level_resp.logLevel)
                oam_fprintf(stdout, "\tDISABLED\n");
            /*SPR 17842 START*/
            else if(oam_mac_log_level_resp.logLevel & OAM_MAC_DEBUGTTILOG)
                oam_fprintf(stdout, "\tDETAILEDALL\n");
            else if(oam_mac_log_level_resp.logLevel & OAM_MAC_DEBUGLOG)
                oam_fprintf(stdout, "\tDETAILED\n");
            else if(oam_mac_log_level_resp.logLevel & OAM_MAC_BRIEFLOG)
                oam_fprintf(stdout, "\tBRIEF\n");
            else if(oam_mac_log_level_resp.logLevel & OAM_MAC_INFOLOG)
                oam_fprintf(stdout, "\tINFO\n");
            else if(oam_mac_log_level_resp.logLevel & OAM_MAC_WARNINGLOG)
                oam_fprintf(stdout, "\tWARNING\n");
            else if(oam_mac_log_level_resp.logLevel & OAM_MAC_ERRORLOG)
                oam_fprintf(stdout, "\tERROR\n");
            else if(oam_mac_log_level_resp.logLevel & OAM_MAC_FATALLOG)
                oam_fprintf(stdout, "\tCRITICAL\n");
            /*SPR 17842 END*/
            oam_print_line();
            break;
        case OAM_GET_LOG_LEVEL_RLC_RESP: /*implemented*/
            oam_print_header();	
            oam_memcpy(&oam_rlc_log_level.logLevel,(char*)(p_api_buf) + OAM_MGMT_INTF_HEADER_SIZE + sizeof(UInt32),sizeof(Oam_LogLevel_Req_t));


            oam_fprintf(stdout, "RLC LOG LEVEL  :");
            if(oam_rlc_log_level.logLevel == OAM_RLC_DISABLED)
                oam_fprintf(stdout, "\tDISABLED\n");
            /*SPR 17842 START*/
            else if(oam_rlc_log_level.logLevel & OAM_RLC_DEBUGALLLOG)
                oam_fprintf(stdout, "\tDETAILEDALL\n");
            else if(oam_rlc_log_level.logLevel & OAM_RLC_DEBUGLOG)
                oam_fprintf(stdout, "\tDETAILED  \n");
            else if(oam_rlc_log_level.logLevel & OAM_RLC_BRIEFLOG)
                oam_fprintf(stdout, "\tBRIEF\n");
            else if(oam_rlc_log_level.logLevel & OAM_RLC_INFOLOG)
                oam_fprintf(stdout, "\tINFO\n");
            else if(oam_rlc_log_level.logLevel & OAM_RLC_WARNINGLOG)
                oam_fprintf(stdout, "\tWARNING\n");
            else if(oam_rlc_log_level.logLevel & OAM_RLC_ERRORLOG)
                oam_fprintf(stdout, "\tERROR\n");
            else if(oam_rlc_log_level.logLevel & OAM_RLC_FATALLOG)
                oam_fprintf(stdout, "\tCRITICAL\n");
            /*SPR 17842 END*/
	    oam_print_line();
            break;
        case OAM_GET_LOG_LEVEL_PDCP_RESP: /*implemented*/
            oam_print_header();
            oam_memcpy(&oam_pdcp_log_level.logLevel,
            (char*)(p_api_buf) + OAM_MGMT_INTF_HEADER_SIZE + sizeof(UInt32) + sizeof(UInt32) ,sizeof(oam_pdcp_log_level_req_t));

            oam_fprintf(stdout, "PDCP LOG LEVEL  : ");
            if(oam_pdcp_log_level.logLevel == OAM_PDCP_INVALIDLOG)
                oam_fprintf(stdout, "\tDISABLED\n");
            else if(oam_pdcp_log_level.logLevel & OAM_PDCP_DEBUGALLLOG)
            /*SPR 17842 START*/
                oam_fprintf(stdout, "\tDETAILEDALL\n");
            else if(oam_pdcp_log_level.logLevel & OAM_PDCP_DEBUGLOG)
                oam_fprintf(stdout, "\tDETAILED\n");
            else if(oam_pdcp_log_level.logLevel & OAM_PDCP_BRIEFLOG)
                oam_fprintf(stdout, "\tBRIEF\n");
            else if(oam_pdcp_log_level.logLevel & OAM_PDCP_INFOLOG)
                oam_fprintf(stdout, "\tINFO\n");
            else if(oam_pdcp_log_level.logLevel & OAM_PDCP_WARNINGLOG)
                oam_fprintf(stdout, "\tWARNING\n");
            else if(oam_pdcp_log_level.logLevel & OAM_PDCP_ERRORLOG)
                oam_fprintf(stdout, "\tERROR\n");
            else if(oam_pdcp_log_level.logLevel & OAM_PDCP_FATALLOG)
                oam_fprintf(stdout, "\tCRITICAL\n");
            /*SPR 17842 START*/
            oam_print_line();
            break;
        case OAM_GET_LOG_LEVEL_GTPU_RESP:
            oam_print_header();
            
            oam_memcpy(&oam_gtpu_log_level,
            (char*)(p_api_buf) + OAM_MGMT_INTF_HEADER_SIZE + sizeof(UInt32) + sizeof(UInt32) + sizeof(UInt32),
            sizeof(oam_relay_Gtpu_Get_Log_Lvl_Req_t));
            /*SPR 19748 FIXED START*/
            oam_fprintf(stdout, "GTPU LOG LEVEL  : ");
            /*SPR 19748 FIXED END*/
            if(oam_gtpu_log_level.logLevel == LTE_L2_LOG_INVALID)
                oam_fprintf(stdout, "\tDISABLED\n");
            /*SPR 17842 START*/
            else if(oam_gtpu_log_level.logLevel & LTE_L2_LOG_DETAILEDALL)
                oam_fprintf(stdout, "\tDETAILEDALL\n");
            else if(oam_gtpu_log_level.logLevel & LTE_L2_LOG_DETAILED)
                oam_fprintf(stdout, "\tDETAILED\n");
            else if(oam_gtpu_log_level.logLevel & LTE_L2_LOG_BRIEF)
                oam_fprintf(stdout, "\tBRIEF\n");
            else if(oam_gtpu_log_level.logLevel & LTE_L2_LOG_INFO)
                oam_fprintf(stdout, "\tINFO\n");
            else if(oam_gtpu_log_level.logLevel & LTE_L2_LOG_WARNING)
                oam_fprintf(stdout, "\tWARNING\n");
            else if(oam_gtpu_log_level.logLevel & LTE_L2_LOG_ERROR)
                oam_fprintf(stdout, "\tERROR\n");
            else if(oam_gtpu_log_level.logLevel & LTE_L2_LOG_CRITICAL)
                oam_fprintf(stdout, "\tCRITICAL\n");
            /*SPR 17842 END*/
            oam_print_line();
            break;
        case OAM_SET_ENABLE_DISABLE_RRM_LOG_RESP:
            oam_print_header();
            oam_memcpy(&oam_rrc_set_log_level_resp,(char*)(p_api_buf) + 10,sizeof(oam_rrc_set_log_level_resp_t));
            if(oam_rrc_set_log_level_resp.response == OAM_ONE){
                oam_fprintf(stdout, "\tResponse Received : SUCCESS\n");
            }
            else
                oam_fprintf(stdout, "\tResponse Received : FAILED\n");
            oam_print_line();

            break;
        case OAM_GET_LOG_LEVEL_L2_RESP: 
            oam_print_header();
            oam_memcpy(&log_lvl_all,(char*)(p_api_buf) + OAM_MGMT_INTF_HEADER_SIZE,sizeof(oam_get_log_level_l2_all_t));
            oam_memcpy(&oam_mac_log_level,(char*)(p_api_buf) + OAM_MGMT_INTF_HEADER_SIZE,sizeof(oam_mac_log_level_req_t));
            oam_fprintf(stdout, "MAC LOG LEVEL  : ");
            if(!log_lvl_all.mac_loglevel)
                oam_fprintf(stdout, "\tDISABLED\n");
            /*SPR 17842 START */
            else if(log_lvl_all.mac_loglevel & OAM_MAC_DEBUGTTILOG)
                oam_fprintf(stdout, "\tDETAILED ALL\n");
            else if(log_lvl_all.mac_loglevel & OAM_MAC_DEBUGLOG)
                oam_fprintf(stdout, "\tDETAILED\n");
            else if(log_lvl_all.mac_loglevel & OAM_MAC_BRIEFLOG)
                oam_fprintf(stdout, "\tBRIEF\n");
            else if(log_lvl_all.mac_loglevel & OAM_MAC_INFOLOG)
                oam_fprintf(stdout, "\tINFO\n");
            else if(log_lvl_all.mac_loglevel & OAM_MAC_WARNINGLOG)
                oam_fprintf(stdout, "\tWARNING\n");
            else if(log_lvl_all.mac_loglevel & OAM_MAC_ERRORLOG)
                oam_fprintf(stdout, "\tERROR\n");
            else if(log_lvl_all.mac_loglevel & OAM_MAC_FATALLOG)
                oam_fprintf(stdout, "\tCRITICAL\n");

           /*SPR 17842 END */

            oam_fprintf(stdout, "RLC LOG LEVEL  : ");
            if(log_lvl_all.rlc_loglevel == OAM_RLC_DISABLED)
                oam_fprintf(stdout, "\tDISABLED\n");
           /*SPR 17842 START */
            else if(log_lvl_all.rlc_loglevel & OAM_RLC_DEBUGALLLOG)
                oam_fprintf(stdout, "\tDETAILED ALL\n");
            else if(log_lvl_all.rlc_loglevel & OAM_RLC_DEBUGLOG)
                oam_fprintf(stdout, "\tDETAILED \n");
            else if(log_lvl_all.rlc_loglevel & OAM_RLC_BRIEFLOG)
                oam_fprintf(stdout, "\tBRIEF\n");
            else if(log_lvl_all.rlc_loglevel & OAM_RLC_INFOLOG)
                oam_fprintf(stdout, "\tINFO\n");
            else if(log_lvl_all.rlc_loglevel & OAM_RLC_WARNINGLOG)
                oam_fprintf(stdout, "\tWARNING\n");
            else if(log_lvl_all.rlc_loglevel & OAM_RLC_ERRORLOG)
                oam_fprintf(stdout, "\tERROR\n");
            else if(log_lvl_all.rlc_loglevel & OAM_RLC_FATALLOG)
                oam_fprintf(stdout, "\tCRITICAL\n");

           /*SPR 17842 END */
            oam_fprintf(stdout, "PDCP LOG LEVEL :" );
            if(log_lvl_all.pdcp_loglevel == OAM_PDCP_INVALIDLOG)
                oam_fprintf(stdout, "\tINVALID\n");
           /*SPR 17842 START */
            else if(log_lvl_all.pdcp_loglevel & OAM_PDCP_DEBUGALLLOG)
                oam_fprintf(stdout, "\tDETAILED ALL\n");
            else if(log_lvl_all.pdcp_loglevel & OAM_PDCP_DEBUGLOG)
                oam_fprintf(stdout, "\tDETAILED\n");
            else if(log_lvl_all.pdcp_loglevel & OAM_PDCP_BRIEFLOG)
                oam_fprintf(stdout, "\tBRIEF\n");
            else if(log_lvl_all.pdcp_loglevel & OAM_PDCP_INFOLOG)
                oam_fprintf(stdout, "\tINFO\n");
            else if(log_lvl_all.pdcp_loglevel & OAM_PDCP_WARNINGLOG)
                oam_fprintf(stdout, "\tWARNING\n");
            else if(log_lvl_all.pdcp_loglevel & OAM_PDCP_ERRORLOG)
                oam_fprintf(stdout, "\tERROR\n");
            else if(log_lvl_all.pdcp_loglevel & OAM_PDCP_FATALLOG)
                oam_fprintf(stdout, "\tCRITICAL\n");
           /*SPR 17842 END */
           /*SPR 17842 START */
       /* Code Deleted  #ifdef GTPU_DISABLE*/
           /*SPR 17842 END */
            oam_fprintf(stdout,"GTPU LOG LEVEL : ");
            if(log_lvl_all.gtpu_loglevel == LTE_L2_LOG_INVALID)
                oam_fprintf(stdout, "\tDISABLED\n");
           /*SPR 17842 START */
            else if(log_lvl_all.gtpu_loglevel & LTE_L2_LOG_DETAILEDALL)
              /*SPR 19748 FIXED START*/
                oam_fprintf(stdout, "\tDETAILED ALL\n");
              /*SPR 19748 FIXED END*/
            else if(log_lvl_all.gtpu_loglevel & LTE_L2_LOG_DETAILED)
                oam_fprintf(stdout, "\tDETAILED\n");
            else if(log_lvl_all.gtpu_loglevel & LTE_L2_LOG_BRIEF)
                oam_fprintf(stdout, "\tBRIEF\n");
            else if(log_lvl_all.gtpu_loglevel & LTE_L2_LOG_INFO)
                oam_fprintf(stdout, "\tINFO\n");
            else if(log_lvl_all.gtpu_loglevel & LTE_L2_LOG_WARNING)
                oam_fprintf(stdout, "\tWARNING\n");
            else if(log_lvl_all.gtpu_loglevel & LTE_L2_LOG_ERROR)
                oam_fprintf(stdout, "\tERROR\n");
            else if(log_lvl_all.gtpu_loglevel & LTE_L2_LOG_CRITICAL)
                oam_fprintf(stdout, "\tCRITICAL\n");
           /*SPR 17842 END */
           /*SPR 17842 START */
           /* Code Deleted */
           /*SPR 17842 END */
 
            oam_print_line();
            break;
        case OAM_GET_LOG_LEVEL_RRC_RESP:
            oam_print_header();
            oam_print_line();
            break;

        case OAM_SET_LOG_LEVEL_L3_RRC_RESP:
            oam_print_header();
            oam_memcpy(&oam_rrc_set_log_level_resp,(char*)(p_api_buf) + 10,sizeof(oam_rrc_set_log_level_resp_t));
            if(oam_rrc_set_log_level_resp.response == OAM_ONE){
                oam_fprintf(stdout, "\tResponse Received : SUCCESS\n");
            }
            else
                oam_fprintf(stdout, "\tResponse Received : FAILED\n");
            oam_print_line();
            break;

        case OAM_SET_LOG_LEVEL_L3_X2AP_RESP:
            oam_print_header();
            oam_memcpy(&oam_x2ap_set_log_level_resp,(char*)(p_api_buf) + 10,sizeof(x2ap_oam_set_log_level_resp_t));
            if(oam_x2ap_set_log_level_resp.response == OAM_ONE){
                oam_fprintf(stdout, "\tX2AP Response Received : SUCCESS\n");
            }
            else
                oam_fprintf(stdout, "\tX2AP Response Received : FAILED\n");
            oam_print_line();
            break;


        case OAM_SET_LOG_LEVEL_L3_S1AP_RESP:
            oam_print_header();
            oam_memcpy(&oam_s1ap_set_log_level_resp,(char*)(p_api_buf) + 10,sizeof(s1ap_oam_set_log_level_resp_t));
            if(oam_s1ap_set_log_level_resp.response == OAM_ONE){
                oam_fprintf(stdout, "\tS1AP Response Received : SUCCESS\n");
            }
            else
                oam_fprintf(stdout, "\tS1AP Response Received : FAILED\n");
            oam_print_line();
            break;

        case OAM_SET_LOG_LEVEL_RRM_RESP:
            oam_print_header();
            oam_memcpy(&oam_rrm_set_log_level_resp,(char*)(p_api_buf) + 10,sizeof(oam_rrm_set_log_level_resp_t));
            if(oam_rrm_set_log_level_resp.response == (rrm_return_et)OAM_SUCCESS){
                oam_fprintf(stdout, "\tResponse Received : SUCCESS\n");
            }
            else
                oam_fprintf(stdout, "\tResponse Received : FAILURE with Reason code %d\n",oam_rrm_set_log_level_resp.fail_cause);
            oam_print_line();

            break;
#ifdef OAM_SON_ENABLE  
        case OAM_SET_LOG_LEVEL_SON_RESP:
            oam_print_header();
            oam_memcpy(&oam_son_set_log_level_resp,(char*)(p_api_buf) + 10,sizeof(oam_son_set_log_level_resp_t));
            if(oam_son_set_log_level_resp.result == SON_SUCCESS){
                oam_fprintf(stdout, "\tResponse Received : SUCCESS\n");
            }
            else
                oam_fprintf(stdout, "\tResponse Received : FAILURE with Reason code %d\n",oam_son_set_log_level_resp.error_code);

            oam_print_line();
            break;
#endif
        case OAM_SET_CALL_TRACE_START:
            break;
        case OAM_SET_CALL_TRACE_STOP:
            break;
#ifdef OAM_SON_ENABLE
        case OAM_SET_ENABLE_DISABLE_SON_RESP:
            oam_print_header();
            oam_memcpy(&oam_son_set_log_enb_disb_level_resp,(char*)(p_api_buf) + 10,sizeof(oam_son_log_enable_disable_resp_t));

            if(oam_son_set_log_enb_disb_level_resp.result == SON_SUCCESS){
                oam_fprintf(stdout, "\tResponse Received : SUCCESS\n");
            }
            else
                oam_fprintf(stdout, "\tResponse Received : FAILURE with Reason code %d\n",oam_son_set_log_enb_disb_level_resp.error_code);
            oam_print_line();
            break;
#endif
        case OAM_SET_LOG_ENABLE_RRC_RESP:
            oam_print_header();
            oam_memcpy(&oam_rrc_log_enable_response,(char*)(p_api_buf) + 10,sizeof(oam_rrc_log_enable_resp_t));
            if(oam_rrc_log_enable_response.response == OAM_ONE){
                oam_fprintf(stdout, "\tResponse Received : SUCCESS\n");
            }
            else
                oam_fprintf(stdout, "\tResponse Received : FAILED\n");
            oam_print_line();
            break;

        case OAM_SET_LOG_ENABLE_X2AP_RESP:
            oam_print_header();
            oam_memcpy(&oam_x2ap_log_enable_response,(char*)(p_api_buf) + 10,sizeof(oam_x2ap_log_enable_resp_t));
            if(oam_x2ap_log_enable_response.response == OAM_ONE){
                oam_fprintf(stdout, "\tResponse Received : SUCCESS\n");
            }
            else
                oam_fprintf(stdout, "\tResponse Received : FAILED\n");
            oam_print_line();
            break;
            

        case OAM_SET_LOG_ENABLE_S1AP_RESP:
            oam_print_header();
            oam_memcpy(&oam_s1ap_log_enable_response,(char*)(p_api_buf) + 10,sizeof(oam_s1ap_log_enable_resp_t));
            if(oam_s1ap_log_enable_response.response == OAM_ONE){
                oam_fprintf(stdout, "\tResponse Received : SUCCESS\n");
            }
            else
                oam_fprintf(stdout, "\tResponse Received : FAILED\n");
            oam_print_line();
            break;

        case OAM_RRM_RECONFIG_RESP:

            oam_print_header();
            oam_memcpy(&oam_rrm_resp,(char*)(p_api_buf) + 10,sizeof(rrm_oam_cell_reconfig_resp_t));
            oam_print_line();
            oam_fprintf(stdout, "\tCell Identity : %d%d%d%d\n",oam_rrm_resp.global_cell_id.cell_identity[0],
                    oam_rrm_resp.global_cell_id.cell_identity[1],
                    oam_rrm_resp.global_cell_id.cell_identity[2],
                    oam_rrm_resp.global_cell_id.cell_identity[3]
                    );
            oam_fprintf(stdout, "\tmcc : %d%d%d\n",oam_rrm_resp.global_cell_id.primary_plmn_id.mcc[0],
                    oam_rrm_resp.global_cell_id.primary_plmn_id.mcc[1],
                    oam_rrm_resp.global_cell_id.primary_plmn_id.mcc[2]
                    );
            oam_fprintf(stdout, "\tmnc : %d%d%d\n",oam_rrm_resp.global_cell_id.primary_plmn_id.mnc[0],
                    oam_rrm_resp.global_cell_id.primary_plmn_id.mnc[1],
                    oam_rrm_resp.global_cell_id.primary_plmn_id.mnc[2]
                    );
            oam_fprintf(stdout, "\tNumber of mnc digits : %d\n",
                    oam_rrm_resp.global_cell_id.primary_plmn_id.num_mnc_digit);

            if(oam_rrm_resp.response == RRM_SUCCESS){
                oam_fprintf(stdout, "\tResponse Received : SUCCESS\n");
            }
            else
            {
                oam_fprintf(stdout, "\tResponse Received : FAILURE with Reason code  = %d\n",oam_rrm_resp.fail_cause);
            }
            oam_print_line();
            break;
        case OAM_SET_LOG_LEVEL_L2_RESP :
            oam_print_header();
            oam_memcpy(&response,(char*)(p_api_buf) + 10,sizeof(UInt32));
            if((UInt8)response == OAM_ONE){
                oam_fprintf(stdout, "\tResponse Received : SUCCESS\n");
            }
            else
                oam_fprintf(stdout, "\tResponse Received : FAILED\n");
            oam_print_line();
            break;
        case OAM_SET_LOG_LEVEL_RESP:
            oam_print_header();
            oam_memcpy(&response,(char*)(p_api_buf) + LOWER_LAYER_INTERFACE_HEADER_SIZE,sizeof(UInt8));   
            if((UInt8)response == OAM_ONE){
                oam_fprintf(stdout, "\tRequest Sent Successfully\n");
            }
            else
                oam_fprintf(stdout, "\tFailed To Send Request\n");
            oam_print_line();
            break;
        /* SPR_13116_CHANGE_START */
        /*SPR 13617 START*/
        case OAM_GET_LOG_LEVEL_L3_RESP:
            oam_print_header();
            oam_memcpy(&oam_rrc_get_log_level_resp,(char*)(p_api_buf) + 12 ,sizeof(oam_rrc_get_log_level_resp_t));
            if(oam_rrc_get_log_level_resp.log_on_off == OAM_ONE)
            {
                if(oam_rrc_get_log_level_resp.log_level == OAM_ZERO){
                    /*SPR 19748 FIXED START*/
                    oam_fprintf(stdout, "\tL3 Log Level : RRC_CRITICAL\n");
                    /*SPR 19748 FIXED END*/
                }
                else if(oam_rrc_get_log_level_resp.log_level == OAM_ONE){
                    oam_fprintf(stdout, "\tL3 Log Level : RRC_ERROR\n");
                }
                else if(oam_rrc_get_log_level_resp.log_level == OAM_TWO){
                    oam_fprintf(stdout, "\tL3 Log Level : RRC_WARNING\n");
                }
                else if(oam_rrc_get_log_level_resp.log_level == OAM_THREE){
                    oam_fprintf(stdout, "\tL3 Log Level : RRC_INFO\n");
                }
                else if(oam_rrc_get_log_level_resp.log_level == OAM_FOUR){
                    oam_fprintf(stdout, "\tL3 Log Level : RRC_BRIEF\n");
                }
                else if(oam_rrc_get_log_level_resp.log_level == OAM_FIVE){
                    oam_fprintf(stdout, "\tL3 Log Level : RRC_DETAILED\n");
                }
                else if(oam_rrc_get_log_level_resp.log_level == OAM_SIX){
                    oam_fprintf(stdout, "\tL3 Log Level : RRC_DETAILEDALL\n");
                }
            }
            else
            {
                oam_fprintf(stdout, "\tLogging is disabled\n");
            }
            oam_print_line();
            break;
       /*SPR 13617 END*/
        case OAM_GET_LOG_LEVEL_L3_S1AP_RESP:
            oam_print_header();
            oam_memcpy(&oam_s1ap_get_log_level_resp,(char*)(p_api_buf) + 12 ,sizeof(oam_s1ap_get_log_level_resp_t));
            if(oam_s1ap_get_log_level_resp.log_on_off == OAM_ONE)
            {
                if(oam_s1ap_get_log_level_resp.log_level == OAM_ZERO){
                    /*SPR 19748 FIXED START */
                    oam_fprintf(stdout, "\tL3 Log Level : S1AP_CRITICAL\n");
                    /*SPR 19748 FIXED END */
                }
                else if(oam_s1ap_get_log_level_resp.log_level == OAM_ONE){
                    oam_fprintf(stdout, "\tL3 Log Level : S1AP_ERROR\n");
                }
                else if(oam_s1ap_get_log_level_resp.log_level == OAM_TWO){
                    oam_fprintf(stdout, "\tL3 Log Level : S1AP_WARNING\n");
                }
                else if(oam_s1ap_get_log_level_resp.log_level == OAM_THREE){
                    oam_fprintf(stdout, "\tL3 Log Level : S1AP_INFO\n");
                }
                else if(oam_s1ap_get_log_level_resp.log_level == OAM_FOUR){
                    oam_fprintf(stdout, "\tL3 Log Level : S1AP_BRIEF\n");
                }
                else if(oam_s1ap_get_log_level_resp.log_level == OAM_FIVE){
                    oam_fprintf(stdout, "\tL3 Log Level : S1AP_DETAILED\n");
                }
                else if(oam_s1ap_get_log_level_resp.log_level == OAM_SIX){
                    oam_fprintf(stdout, "\tL3 Log Level : S1AP_DETAILEDALL\n");
                }
            }
            else
            {
                oam_fprintf(stdout, "\tLogging is disabled\n");
            }
            oam_print_line();
            break;
        case OAM_GET_LOG_LEVEL_L3_X2AP_RESP:
            oam_print_header();
            oam_memcpy(&oam_x2ap_get_log_level_resp,(char*)(p_api_buf) + 12 ,sizeof(oam_x2ap_get_log_level_resp_t));
            if(oam_x2ap_get_log_level_resp.log_on_off == OAM_ONE)
            {
                if(oam_x2ap_get_log_level_resp.log_level == OAM_ZERO){
                    /*SPR 19748 FIXED START */
                    oam_fprintf(stdout, "\tL3 Log Level : X2AP_CRITICAL\n");
                    /*SPR 19748 FIXED END */
                }
                else if(oam_x2ap_get_log_level_resp.log_level == OAM_ONE){
                    oam_fprintf(stdout, "\tL3 Log Level : X2AP_ERROR\n");
                }
                else if(oam_x2ap_get_log_level_resp.log_level == OAM_TWO){
                    oam_fprintf(stdout, "\tL3 Log Level : X2AP_WARNING\n");
                }
                else if(oam_x2ap_get_log_level_resp.log_level == OAM_THREE){
                    oam_fprintf(stdout, "\tL3 Log Level : X2AP_INFO\n");
                }
                else if(oam_x2ap_get_log_level_resp.log_level == OAM_FOUR){
                    oam_fprintf(stdout, "\tL3 Log Level : X2AP_BRIEF\n");
                }
                else if(oam_x2ap_get_log_level_resp.log_level == OAM_FIVE){
                    oam_fprintf(stdout, "\tL3 Log Level : X2AP_DETAILED\n");
                }
                else if(oam_x2ap_get_log_level_resp.log_level == OAM_SIX){
                    oam_fprintf(stdout, "\tL3 Log Level : X2AP_DETAILEDALL\n");
                }
            }
            else
            {
                oam_fprintf(stdout, "\tLogging is disabled\n");
            }
            oam_print_line();
            break;
        case OAM_GET_LOG_LEVEL_RESP:
            oam_print_header();
            oam_memcpy(&response,(char*)(p_api_buf) + LOWER_LAYER_INTERFACE_HEADER_SIZE,sizeof(UInt8));
            if((UInt8)response == OAM_ZERO){
                oam_fprintf(stdout, "\tOAM Log Level : OAM_CRITICAL\n");
            }
            else if((UInt8)response == OAM_ONE){
                oam_fprintf(stdout, "\tOAM Log Level : OAM_ERROR\n");
            }
            else if((UInt8)response == OAM_TWO){
                oam_fprintf(stdout, "\tOAM Log Level : OAM_WARNING\n");
            }
            else if((UInt8)response == OAM_THREE){
                oam_fprintf(stdout, "\tOAM Log Level : OAM_INFO\n");
            }
            else if((UInt8)response == OAM_FOUR){
                oam_fprintf(stdout, "\tOAM Log Level : OAM_BRIEF\n");
            }
            else if((UInt8)response == OAM_FIVE){
                oam_fprintf(stdout, "\tOAM Log Level : OAM_DETAILED\n");
            }
            else if((UInt8)response == OAM_SIX){
                oam_fprintf(stdout, "\tOAM Log Level : OAM_DETAILEDALL\n");
            }
            oam_print_line();
            break;
#ifdef OAM_SON_ENABLE
        case OAM_GET_LOG_LEVEL_SON_RESP :
            oam_print_header();
            oam_memcpy(&oam_son_get_log_level_resp,(char*)(p_api_buf) + 12 ,sizeof(oam_son_get_log_level_resp_t));
            if(oam_son_get_log_level_resp.error_code == SON_NO_ERROR)
            {
                if(oam_son_get_log_level_resp.num_of_modules == SON_MAX_NO_INT_MODULES)
                {
                    for(count = OAM_ZERO;count < oam_son_get_log_level_resp.num_of_modules; count ++)
                    {
                        if(oam_son_get_log_level_resp.module_log_level[count].error_code == SON_NO_ERROR)
                        {
                            if(oam_son_get_log_level_resp.module_log_level[count].module_id == SON_MIF_MODULE_ID)
                            {
                                oam_fprintf(stdout, "Module : SON_MIF_MODULE_ID \n");
                            }
                            else if(oam_son_get_log_level_resp.module_log_level[count].module_id == SON_APPS_MODULE_ID )
                            {
                                oam_fprintf(stdout, "Module : SON_APPS_MODULE_ID \n");
                            }
                            else if(oam_son_get_log_level_resp.module_log_level[count].module_id == SON_ANR_MODULE_ID )
                            {
                                oam_fprintf(stdout, "Module : SON_ANR_MODULE_ID \n");
                            }
                            else if(oam_son_get_log_level_resp.module_log_level[count].module_id == SON_ES_MODULE_ID )
                            {
                                oam_fprintf(stdout, "Module : SON_ES_MODULE_ID \n");
                            }
                            else if(oam_son_get_log_level_resp.module_log_level[count].module_id == SON_NMM_MODULE_ID )
                            {
                                oam_fprintf(stdout, "Module : SON_NMM_MODULE_ID \n");
                            }
                            else if(oam_son_get_log_level_resp.module_log_level[count].module_id == SON_MRO_MODULE_ID)
                            {
                                oam_fprintf(stdout, "Module : SON_MRO_MODULE_ID\n");
                            }
                            else if(oam_son_get_log_level_resp.module_log_level[count].module_id == SON_MLB_MODULE_ID)
                            {
                                oam_fprintf(stdout, "Module : SON_MLB_MODULE_ID\n");
                            }
                            else if(oam_son_get_log_level_resp.module_log_level[count].module_id == SON_RACH_OPT_MODULE_ID)
                            {
                                oam_fprintf(stdout, "Module : SON_RACH_OPT_MODULE_ID\n");
                            }




                            if(oam_son_get_log_level_resp.module_log_level[count].log_level == SON_OAM_LOG_LEVEL_CRITICAL)
                                oam_fprintf(stdout, "Log Level : CRITICAL  \n");
                            else if(oam_son_get_log_level_resp.module_log_level[count].log_level == SON_OAM_LOG_LEVEL_ERROR)
                                oam_fprintf(stdout, "Log Level : ERROR  \n");
                            else if(oam_son_get_log_level_resp.module_log_level[count].log_level == SON_OAM_LOG_LEVEL_WARNING )
                                oam_fprintf(stdout, "Log Level : WARNING  \n");
                            else if(oam_son_get_log_level_resp.module_log_level[count].log_level == SON_OAM_LOG_LEVEL_INFO)
                                oam_fprintf(stdout, "Log Level : INFO  \n");
                            else if(oam_son_get_log_level_resp.module_log_level[count].log_level == SON_OAM_LOG_LEVEL_BRIEF )
                                oam_fprintf(stdout, "Log Level : BRIEF  \n");
                            else if(oam_son_get_log_level_resp.module_log_level[count].log_level == SON_OAM_LOG_LEVEL_DETAILED )
                                oam_fprintf(stdout, "Log Level : DETAILED  \n");
                            else if(oam_son_get_log_level_resp.module_log_level[count].log_level == SON_OAM_LOG_LEVEL_DETAILEDALL )
                                oam_fprintf(stdout, "Log Level : DETAILEDALL  \n");
                        }
                        else
                        {
                            if(oam_son_get_log_level_resp.module_log_level[count].error_code == SON_ERR_LOGGING_DISABLED )
                            {
                                oam_fprintf(stdout, " Logging disabled for Module %d\n",
                                        oam_son_get_log_level_resp.module_log_level[count].module_id);
                            }
                            else
                            {
                                oam_fprintf(stdout, " Error From SON \n");
                            }

                        }
                    }
                }
                else if(oam_son_get_log_level_resp.num_of_modules == OAM_ONE)
                {
                    if(oam_son_get_log_level_resp.module_log_level[OAM_ZERO].error_code == SON_NO_ERROR)
                    {

                        if(oam_son_get_log_level_resp.module_log_level[OAM_ZERO].module_id == SON_MIF_MODULE_ID)
                        {
                            oam_fprintf(stdout, "Module : SON_MIF_MODULE_ID \n");
                        }
                        else if(oam_son_get_log_level_resp.module_log_level[OAM_ZERO].module_id == SON_APPS_MODULE_ID )
                        {
                            oam_fprintf(stdout, "Module : SON_APPS_MODULE_ID \n");
                        }
                        else if(oam_son_get_log_level_resp.module_log_level[OAM_ZERO].module_id == SON_ANR_MODULE_ID )
                        {
                            oam_fprintf(stdout, "Module : SON_ANR_MODULE_ID \n");
                        }
                        else if(oam_son_get_log_level_resp.module_log_level[OAM_ZERO].module_id == SON_ES_MODULE_ID )
                        {
                            oam_fprintf(stdout, "Module : SON_ES_MODULE_ID \n");
                        }
                        else if(oam_son_get_log_level_resp.module_log_level[OAM_ZERO].module_id == SON_NMM_MODULE_ID )
                        {
                            oam_fprintf(stdout, "Module : SON_NMM_MODULE_ID \n");
                        }
                        else if(oam_son_get_log_level_resp.module_log_level[OAM_ZERO].module_id == SON_MRO_MODULE_ID)
                        {
                            oam_fprintf(stdout, "Module : SON_MRO_MODULE_ID\n");
                        }
                        else if(oam_son_get_log_level_resp.module_log_level[OAM_ZERO].module_id == SON_MLB_MODULE_ID)
                        {
                            oam_fprintf(stdout, "Module : SON_MLB_MODULE_ID\n");
                        }
                        else if(oam_son_get_log_level_resp.module_log_level[OAM_ZERO].module_id == SON_RACH_OPT_MODULE_ID)
                        {
                            oam_fprintf(stdout, "Module : SON_RACH_OPT_MODULE_ID\n");
                        }




                        if(oam_son_get_log_level_resp.module_log_level[OAM_ZERO].log_level == SON_OAM_LOG_LEVEL_CRITICAL)
                            oam_fprintf(stdout, "Log Level : CRITICAL  \n");
                        else if(oam_son_get_log_level_resp.module_log_level[OAM_ZERO].log_level == SON_OAM_LOG_LEVEL_ERROR)
                            oam_fprintf(stdout, "Log Level : ERROR  \n");
                        else if(oam_son_get_log_level_resp.module_log_level[OAM_ZERO].log_level == SON_OAM_LOG_LEVEL_WARNING )
                            oam_fprintf(stdout, "Log Level : WARNING  \n");
                        else if(oam_son_get_log_level_resp.module_log_level[OAM_ZERO].log_level == SON_OAM_LOG_LEVEL_INFO )
                            oam_fprintf(stdout, "Log Level : INFO  \n");
                        else if(oam_son_get_log_level_resp.module_log_level[OAM_ZERO].log_level == SON_OAM_LOG_LEVEL_BRIEF )
                            oam_fprintf(stdout, "Log Level : BRIEF  \n");
                        else if(oam_son_get_log_level_resp.module_log_level[OAM_ZERO].log_level == SON_OAM_LOG_LEVEL_DETAILED )
                            oam_fprintf(stdout, "Log Level : DETAILED  \n");
                        else if(oam_son_get_log_level_resp.module_log_level[OAM_ZERO].log_level == SON_OAM_LOG_LEVEL_DETAILEDALL )
                            oam_fprintf(stdout, "Log Level : DETAILEDALL  \n");
                    }
                    else
                    {
                        if(oam_son_get_log_level_resp.module_log_level[count].error_code == SON_ERR_LOGGING_DISABLED )
                        {
                            oam_fprintf(stdout, " Logging disabled for Module %d\n",
                                    oam_son_get_log_level_resp.module_log_level[count].module_id);
                        }
                        else
                        {
                            oam_fprintf(stdout, " Error From SON \n");
                        }

                    }
                }    else
                {
                    oam_fprintf(stdout, "Invalid Number of Modules[%d]\n",
                    oam_son_get_log_level_resp.num_of_modules);
                }
            }
            else
            {
                if(oam_son_get_log_level_resp.error_code == SON_ERR_REQ_ALREADY_IN_PROGRESS)
                {
                    oam_fprintf(stdout, "SON_ERR_REQ_ALREADY_IN_PROGRESS \n");
                }
                else if(oam_son_get_log_level_resp.error_code == SON_ERR_UNEXPECTED_MSG)
                {
                    oam_fprintf(stdout, "SON_ERR_UNEXPECTED_MSG\n");
                }
                else if(oam_son_get_log_level_resp.error_code == SON_ERR_INVALID_PARAMS)
                {
                    oam_fprintf(stdout, "Invalid Module ID[%d] \n",
                    oam_son_get_log_level_resp.module_log_level[count].module_id);
                }
                else
                {
                    oam_fprintf(stdout, " Error Response from SON \n");
                }
            }

            oam_print_line();
            break;
#endif
        case OAM_GET_LOG_LEVEL_RRM_RESP :
            oam_print_header();

            oam_memcpy(&oam_rrm_get_log_level_resp,(char*)(p_api_buf) + 12,sizeof(oam_rrm_get_log_level_resp_t));
            if(oam_rrm_get_log_level_resp.fail_cause == RRM_NO_ERROR)
            {


                if(oam_rrm_get_log_level_resp.num_of_modules == RRM_MAX_NUM_INT_MODULES)
                {
                    for(count = OAM_ZERO;count < oam_rrm_get_log_level_resp.num_of_modules; count ++)
                    {
                        if(oam_rrm_get_log_level_resp.module_id[count].error_code == RRM_NO_ERROR)
                        {

                            if(oam_rrm_get_log_level_resp.module_id[count].module_id == RRM_OAM_MIF_MODULE_ID)
                            {
                                oam_fprintf(stdout, "Module : RRM Management Interface Handler \n");
                            }
                            else if(oam_rrm_get_log_level_resp.module_id[count].module_id == RRM_OAM_CM_MODULE_ID)
                            {
                                oam_fprintf(stdout, "Module : CELL Manager  \n");
                            }
                            else if(oam_rrm_get_log_level_resp.module_id[count].module_id == RRM_OAM_UEM_MODULE_ID)
                            {
                                oam_fprintf(stdout, "Module : UE Manager \n");
                            }

                            if(oam_rrm_get_log_level_resp.module_id[count].log_level == RRM_OAM_CRITICAL)
                                oam_fprintf(stdout, "Log Level : CRITICAL  \n");
                            else if(oam_rrm_get_log_level_resp.module_id[count].log_level == RRM_OAM_ERROR)
                                oam_fprintf(stdout, "Log Level : ERROR  \n");
                            else if(oam_rrm_get_log_level_resp.module_id[count].log_level == RRM_OAM_WARNING)
                                oam_fprintf(stdout, "Log Level : WARNING  \n");
                            else if(oam_rrm_get_log_level_resp.module_id[count].log_level == RRM_OAM_INFO)
                                oam_fprintf(stdout, "Log Level : INFO  \n");
                            else if(oam_rrm_get_log_level_resp.module_id[count].log_level ==  RRM_OAM_BRIEF )
                                oam_fprintf(stdout, "Log Level : BRIEF \n ");
                            else if(oam_rrm_get_log_level_resp.module_id[count].log_level == RRM_OAM_DETAILED )
                                oam_fprintf(stdout, "Log Level : DETAILED \n ");
                            else if(oam_rrm_get_log_level_resp.module_id[count].log_level == RRM_OAM_DETAILEDALL )
                                oam_fprintf(stdout, "Log Level : DETAILEDALL \n ");
                        }
                        else
                        {
                            if(oam_rrm_get_log_level_resp.module_id[count].error_code == RRM_ERR_LOGGING_DISABLED)
                            {
                                oam_fprintf(stdout, " Logging disabled for Module %d\n",
                                        oam_rrm_get_log_level_resp.module_id[count].module_id);
                            }
                            else
                            {
                                oam_fprintf(stdout, " Error From RRM \n");
                            }

                        }


                    }
                }
                else if(oam_rrm_get_log_level_resp.num_of_modules == OAM_ONE)
                {
                    if(oam_rrm_get_log_level_resp.module_id[OAM_ZERO].error_code == RRM_NO_ERROR)
                    {

                        if(oam_rrm_get_log_level_resp.module_id[OAM_ZERO].module_id == RRM_OAM_MIF_MODULE_ID)
                        {
                            oam_fprintf(stdout, "Module : RRM Management Interface Handler \n");
                        }
                        else if(oam_rrm_get_log_level_resp.module_id[OAM_ZERO].module_id == RRM_OAM_CM_MODULE_ID)
                        {
                            oam_fprintf(stdout, "Module : CELL Manager \n ");
                        }
                        else if(oam_rrm_get_log_level_resp.module_id[OAM_ZERO].module_id == RRM_OAM_UEM_MODULE_ID)
                        {
                            oam_fprintf(stdout, "Module : UE Manager \n");
                        }

                        if(oam_rrm_get_log_level_resp.module_id[OAM_ZERO].log_level == RRM_OAM_CRITICAL)
                            oam_fprintf(stdout, "Log Level : CRITICAL \n ");
                        else if(oam_rrm_get_log_level_resp.module_id[OAM_ZERO].log_level == RRM_OAM_ERROR)
                            oam_fprintf(stdout, "Log Level : ERROR \n ");
                        else if(oam_rrm_get_log_level_resp.module_id[OAM_ZERO].log_level == RRM_OAM_WARNING)
                            oam_fprintf(stdout, "Log Level : WARNING \n ");
                        else if(oam_rrm_get_log_level_resp.module_id[OAM_ZERO].log_level == RRM_OAM_INFO)
                            oam_fprintf(stdout, "Log Level : INFO \n ");
                        else if(oam_rrm_get_log_level_resp.module_id[OAM_ZERO].log_level ==  RRM_OAM_BRIEF )
                            oam_fprintf(stdout, "Log Level : BRIEF  \n");
                        else if(oam_rrm_get_log_level_resp.module_id[OAM_ZERO].log_level == RRM_OAM_DETAILED )
                            oam_fprintf(stdout, "Log Level : DETAILED \n ");
                        else if(oam_rrm_get_log_level_resp.module_id[OAM_ZERO].log_level == RRM_OAM_DETAILEDALL )
                            oam_fprintf(stdout, "Log Level : DETAILEDALL \n ");
                    }
                    else
                    {
                        if(oam_rrm_get_log_level_resp.module_id[OAM_ZERO].error_code == RRM_ERR_LOGGING_DISABLED)
                        {
                            oam_fprintf(stdout, " Logging disabled for Module %d\n",
                                    oam_rrm_get_log_level_resp.module_id[OAM_ZERO].module_id);
                        }
                        else
                        {
                            oam_fprintf(stdout, " Error From RRM \n");
                        }

                    }


                }
                else
                {
                    oam_fprintf(stdout, " Invalid Number[%d] of Modules \n",
                    oam_rrm_get_log_level_resp.num_of_modules);
                }
            }
            else
            {
                if(oam_rrm_get_log_level_resp.fail_cause== RRM_ERR_REQ_ALREADY_IN_PROGRESS)
                {
                    oam_fprintf(stdout, "RRM_ERR_REQ_ALREADY_IN_PROGRESS\n");
                }
                else if(oam_rrm_get_log_level_resp.fail_cause == RRM_ERR_UNEXPECTED_MSG)
                {
                    oam_fprintf(stdout, "RRM_ERR_UNEXPECTED_MSG\n");
                }
                else if(oam_rrm_get_log_level_resp.fail_cause == RRM_ERR_INVALID_PARAMS)
                {
                    oam_fprintf(stdout, "Invalid Module ID[%d] \n",
                    oam_rrm_get_log_level_resp.module_id[count].module_id);
                }
                else
                {
                    oam_fprintf(stdout, " Error Response from RRM \n");
                }
            }

            oam_print_line();
            break;
            /* SPR_13116_CHANGE_END */


            /* CSR_59405_FIX_START */
        case OAM_SET_CALL_TRACE_START_RESP:
            oam_print_header();
            oam_memcpy(&response,(char*)(p_api_buf) + LOWER_LAYER_INTERFACE_HEADER_SIZE ,sizeof(UInt8));   //FIX 748
            if((UInt8)response == OAM_ONE){
                oam_fprintf(stdout, "\tRequest Sent Successfully for CELL TRACE START\n");
            }
            else
                oam_fprintf(stdout, "\tFailed To Send Request for CELL TRACE START\n");
            oam_print_line();
            break;
        case OAM_SET_CALL_TRACE_STOP_RESP:
            oam_print_header();
            oam_memcpy(&response,(char*)(p_api_buf) + LOWER_LAYER_INTERFACE_HEADER_SIZE ,sizeof(UInt8));   //FIX 748
            if((UInt8)response == OAM_ONE){
                oam_fprintf(stdout, "\tRequest Sent Successfully for CELL TRACE STOP\n");
            }
            else
                oam_fprintf(stdout, "\tFailed To Send Request for CELL TRACE STOP\n");
            oam_print_line();
            break;
            /* CSR_59405_FIX_END */

        case OAM_CONTROL_COMMAND_RESP:
            if(oam_get_word_from_header((UChar8 *)(p_api_buf) + 12) == OAM_RESULT_SUCCESS)
            {

                oam_print_line();
                oam_fprintf(stdout, "\tSHUT DOWN/RESTART SUCCESS, Please Wait For 30 Seconds......\n");
                oam_print_line();
                OAM_EXIT(1);
            }
            else
            {
                oam_print_line();
                oam_fprintf(stdout, "\tFailed To Shutdown\n");
                oam_print_line();
            }  
            break;
        case OAM_DELETE_OBJECT_RESP : 
            oam_print_line();
            oam_fprintf(stdout, "\t Deleting object.....\n");
            ret = oam_interface_resp((U8*)p_api_buf);
            /*  Coverity ID : 79019 Fix Start */
            /* SPR 16607 Fix Start */
             if(OAM_SUCCESS == ret)
                showPrompt = OAM_ONE; 
            /* SPR 16607 Fix Start */    
            /*  Coverity ID : 79019 Fix End */
            break;
        case OAM_ADD_OBJECT_RESP : 
            ret = oam_interface_resp((U8*)p_api_buf);
            if(OAM_SUCCESS == ret)
                showPrompt = OAM_ZERO; 
            break;

        default:
            break;
    }
    if(showPrompt)
    {
        message_send.apiid_expected = 0;
        message_send.message_send = 0;
        show_prompt();
    }
    /*Coverity 79056 Fix Start*/
    /*Code Deleted*/
    /*Coverity 79056 Fix Start*/
    return OAM_NULL;
}

/******************************************************************************
 *   FUNCTION NAME   : oam_qcli_proc_msg
 *   DESCRIPTION     : This function qcli trap process msg 
 *   Inputs          : p_api_buf 
 *   Outputs         : NONE
 *   RETURNS         : NONE
 ******************************************************************************/
void oam_qcli_trap_process_msg
(
 void *p_api_buf
 )
{
    /*Cov Cid 54894 fix */
	UInt8 *p_temp_buf = OAM_NULL;
	UInt8 event_code[255];
	UInt8 num_parent_nodes = OAM_ZERO;
	UInt8 parent_node_count = OAM_ZERO;
	UInt16 event_info_len = OAM_ZERO;
	UInt8 param_num = OAM_ZERO;
	UInt16 parent_node_name_len = OAM_ZERO;
	UInt8 parent_node_name[60] = {OAM_ZERO};
	UInt8 parameter_count = OAM_ZERO;
	UInt16 parent_info_len = OAM_ZERO;
	UInt16 param_len = OAM_ZERO;	
	UInt8 name_val_pair[128] = {OAM_ZERO};
	char *name=NULL;
	char *value=NULL;
	UInt16 cur_pos=0;
	oam_fprintf(stdout, "\n");

	oam_print_line();

	p_temp_buf  = p_api_buf ;
	cur_pos += OAM_MGMT_INTF_HEADER_SIZE;


	cur_pos += OAM_TWO;

	/* Reserved Bytes */
	cur_pos += OAM_TWO;

	/* Reserved Bytes */
	cur_pos += OAM_FOUR;

	/* Extract Event Info Length */
	event_info_len = p_temp_buf[cur_pos++];

	/* Reserved Bytes */
	cur_pos += OAM_THREE;

	/* Extract Event Code */
    oam_strncpy((char *)event_code,(const char *)&p_temp_buf[cur_pos],event_info_len);
	event_code[event_info_len]='\0';

#if OAM_RADIO_HEAD_SUPPORT
	if(strstr((Char8*)event_code, "RRH")) {
        oam_fprintf(stderr, "%s\n############################\n", event_code);
	}
#endif

    cur_pos += event_info_len;

	/* Reserved Bytes */
	cur_pos += OAM_FOUR;

	/* Extract total length of parameter list information */
	cur_pos += OAM_TWO;

	/* Extract Total Number of parents */
	num_parent_nodes = p_temp_buf[cur_pos++];

	/* Reserved Bytes */
	cur_pos += OAM_ONE;

	/* Reserved Bytes */
	cur_pos += OAM_FOUR;

	/* Extract Parent Node Information */
	for(parent_node_count = 0; parent_node_count < num_parent_nodes ; parent_node_count++)
	{
		/* Extract total length of parent information */
		cur_pos += OAM_TWO;

		param_num = p_temp_buf[cur_pos++];


		/* Reserved Bytes */

		cur_pos += OAM_ONE;

		/* Reserved Bytes */

		cur_pos += OAM_TWO;

		/* Extract Length of Parent Node Name */

		parent_node_name_len = get_word_from_header((UInt8 *)(p_temp_buf+cur_pos));

		cur_pos += OAM_TWO;

		/* Extract Parent Node Name */
		if(parent_node_name_len < 60)
		{
            oam_strncpy((char *)parent_node_name,(const char *)p_temp_buf+cur_pos,parent_node_name_len);
			parent_node_name[parent_node_name_len]='\0';
		}
		else
		{
            oam_strncpy((char*)parent_node_name,(const char *)p_temp_buf+cur_pos,60);
			parent_node_name[60 - OAM_ONE]='\0';
		}

        /* spr_21013_fix_start */
		if(oam_strncasecmp((char *)parent_node_name,"InternetGatewayDevice.FAPService.FaultMgmt",
                    oam_strlen("InternetGatewayDevice.FAPService.FaultMgmt"))== OAM_ZERO)    
        /* spr_21013_fix_end */
		{

            oam_fprintf(stderr,"ALARM NOTIFICATION\n");
			oam_print_line();
		}
		else
		{
            oam_fprintf(stderr,"\nPARENT:%s\n",parent_node_name);
		}
		cur_pos += parent_node_name_len;

		for(parameter_count = 0; parameter_count < param_num; parameter_count++)
		{
			parent_info_len = get_word_from_header((UInt8 *)(p_temp_buf+cur_pos));
			cur_pos += OAM_TWO;
			/* Reserved Bytes */
			cur_pos += OAM_TWO;

			/* Extract Name Value Pair */
			param_len = parent_info_len-OAM_FOUR;
			if(param_len < STR_LEN_128)
			{
                oam_strncpy((char *)name_val_pair,(const char *)p_temp_buf+cur_pos,param_len);
				name_val_pair[param_len]='\0';
			}
			name=strtok((char *)name_val_pair,"=");
			value=strtok(NULL,"=");
			if((name != NULL) && (value != NULL))
                oam_fprintf(stdout,"%-25s\t=\t%-20s\n",name,value);
			cur_pos += param_len;
		}
	}
	oam_print_line();


}
/******************************************************************************
 *   FUNCTION NAME   : oam_qcli_cmd_intf_func
 *   DESCRIPTION     : This function qcli cmd intf func
 *   Inputs          : thread_arg
 *   Outputs         : NONE
 *   RETURNS         : NONE
 ******************************************************************************/
void * oam_qcli_cmd_intf_func(void *thread_arg)
{
	thread_arg_t *p_thread_arg = (thread_arg_t *)thread_arg;
	qvDriverLoop(p_thread_arg->driver_id,p_thread_arg->p_qwait);
	return OAM_ZERO;
}

/******************************************************************************
 *   FUNCTION NAME   : oam_qcli_cmd_intf_func
 *   DESCRIPTION     : This function qcli cmd intf func
 *   Inputs          : thread_arg
 *   Outputs         : NONE
 *   RETURNS         : None 
 ******************************************************************************/

static void * oam_qcli_trap_intf_func(void *thread_arg)
{
	thread_arg_t *p_thread_arg = (thread_arg_t *)thread_arg;
	qvDriverLoop(p_thread_arg->driver_id,p_thread_arg->p_qwait);
	return OAM_ZERO;
}   

/******************************************************************************
 *   FUNCTION NAME   : oam_qcli_cmd_send_timer_expire
 *   DESCRIPTION     : This function qcli cmd send timer expire 
 *   Inputs          : timer_id,p_timer_buf,p_context 
 *   Outputs         : NONE
 *   RETURNS         : NONE
 ******************************************************************************/
void  oam_qcli_cmd_send_timer_expire
(
 oam_timer_t timer_id,
 void   *p_timer_buf,
 void   *p_context
 )
{
	/*SPR:9224_CHANGE_START*/
	if(timer_flag)
	{
		oam_print_line();
        /* SPR 17777 fix */
        qcliHandleUnusedParameter(timer_id);
        qcliHandleUnusedParameter(p_timer_buf);
        qcliHandleUnusedParameter(p_context);
		oam_print_line();
		message_send.message_send = 0;
		show_prompt();
	}
	/*SPR:9224_CHANGE_END*/

}


static const unsigned long QCLITrapList[] =
{
	OAM_MODULE_ID,
	0
};

static const unsigned long QCLICmdList[] =
{
	OAM_MODULE_ID,
	QCLI_INTERFACE_MODULE_ID,
	0
};

const QMANIFEST QCLICmd =
{
	"QCLI_MODULE_ID", 
	QCLI_MODULE_ID, 
	{ OAM_ZERO, OAM_ZERO},
	{(S32 (*)(void *, void *))oam_qcli_proc_msg,(void (*)(QTIMER, void *, void *))oam_qcli_cmd_send_timer_expire},
	{ 0, 0, (void *(*)(QMODULE, unsigned int))Alloc},
	QCLICmdList
};
/* coverity 35582 35583 35584 */

const QMANIFEST QCLITrap = 
{
	"OAM_QCLI_TRAP_MODULE_ID",
	OAM_QCLI_TRAP_MODULE_ID,
	{ OAM_ZERO, OAM_ZERO},
	{(S32 (*)(void *, void *))oam_qcli_trap_process_msg,OAM_ZERO},
	{0, 0, (void *(*)(QMODULE, unsigned int))Alloc},
	QCLITrapList
};
/* overity 35586 35585 */
/*main function*/
/******************************************************************************
 *   FUNCTION NAME   : main
 *   DESCRIPTION     : This main function send cli information
 *   Inputs          : C,V
 *   Outputs         : NONE
 *   RETURNS         : SUCCESS/FAILURE
 ******************************************************************************/
int main
(
 int C,
 char **V
 )
{
	QDRIVER qcli_cmd_intf_driver,qcli_trap_intf_driver;

	thread_arg_t qcli_cmd_intf_thd_arg,qcli_trap_intf_thd_arg;

	pthread_t qcli_cmd_intf_tid,qcli_trap_intf_tid;

#if defined(OAM_MSPD_TARGET) || defined(OAM_FREESCALE_TARGET)
	OAM_SIGNAL(SIGALRM,alarm_signal_handler);
#endif


	cpu_set_t cpuSetSch;
	unsigned int coreNum = 0;
	/*SPR 22554 Fix Start*/
	Char8  threadName[OAM_THREAD_NAME_LEN] = "oam_qcli_intf";
	/*SPR 22554 Fix End*/
	if (C>=2) 
	{
		coreNum = oam_atoi(V[1]);
	}

	OAM_CPU_ZERO(&cpuSetSch);
	OAM_CPU_SET(coreNum ,&cpuSetSch);
		pid_t p = 0;
	if (0 != OAM_SCHED_SETAFFINITY(p, sizeof(cpu_set_t), &cpuSetSch))
		{
		oam_fprintf(stderr, " sched_setaffinity Returned Error in QCLI %s Core not Binded", V[1]);
		OAM_EXIT(1);
		}

	oam_fprintf(stdout,"\nQCLI is binding with Default Core Number 0\n");
	oam_fprintf(stdout,"\nIf you want to change the Core Number, restart QCLI and provide Core Number as Argument\n");
	oam_fprintf(stdout,"\nWelcome to LTE eNodeB Interface\nEnter your command\n");

	qvInit(0,&my_os);

	/* SPR 1251 FIX START */
	cli_listener =	qvOpen(&my_shell,NULL ); 

	if(cli_listener == NULL)
		/* SPR 1251 FIX END */
	{
		oam_fprintf(stdout,"\noam_multi_thd_init:Unable to create Listener");
		return OAM_FAILURE;
	}

	qcli_cmd_intf_driver = qvNewDriver();
	qcli_cmd_intf_thd_arg.driver_id = qcli_cmd_intf_driver;
	qcli_cmd_intf_thd_arg.p_qwait = (QWAIT *)&my_waitstruct;

	qcli_trap_intf_driver = qvNewDriver();
	qcli_trap_intf_thd_arg.driver_id = qcli_trap_intf_driver;
	qcli_trap_intf_thd_arg.p_qwait = (QWAIT *)&my_waitstruct;



	qvRegister(qcli_cmd_intf_driver,
			"QCLI_MODULE_ID", 
			QCLI_MODULE_ID, &QCLICmd,(void *)&qcli_cmd_intf_thd_arg);


	qvRegister(qcli_trap_intf_driver,
			"OAM_QCLI_TRAP_MODULE_ID",
			OAM_QCLI_TRAP_MODULE_ID, &QCLITrap,(void *)&qcli_trap_intf_thd_arg);

	/* SPR 1251 FIX START */
	qvSetDestination(QCLI_INTERFACE_MODULE_ID, &my_shell, cli_listener, OAM_ZERO);

	/* SPR 1251 FIX END */
	oam_pthread_create(&qcli_cmd_intf_tid, OAM_ZERO, oam_qcli_cmd_intf_func, 
			(void *)&qcli_cmd_intf_thd_arg);
/*SPR 22554 Fix Start*/

    /* Set name of the main thread */
    if (0 != oam_pthread_setname_np(&qcli_cmd_intf_tid, threadName))
    {
        oam_fprintf(stdout, "Failed to set name of the QCLI interface thread \n");
    }

/*SPR 22554 Fix End*/
	oam_pthread_create(&qcli_trap_intf_tid, OAM_ZERO, oam_qcli_trap_intf_func, 
			(void *)&qcli_trap_intf_thd_arg);

/*SPR 22554 Fix Start*/

    /* Set name of the main thread */
    oam_snprintf(threadName, MAX_THREAD_NAME_LEN,"oam_qcli_trap_intf");
    if (0 != oam_pthread_setname_np(&qcli_trap_intf_tid, threadName))
    {
        //oam_fprintf(stdout, "Failed to set name of the QCLI Trap interface thread \n");
    }

/*SPR 22554 Fix End*/
	/* SPR 1251 FIX START */
	qvInputLoop(&my_shell,cli_listener);
	/* SPR 1251 FIX END */
	return OAM_SUCCESS;

	/*Code ended to make the code multi threaded.*/
}

/******************************************************************************
 *   FUNCTION NAME     :  oam_qcli_notification_ind_resp
 *   DESCRIPTION       :  fucntion to send notification response to oam 
 *
 *   INPUTS            :  status,transaction_id,err_string
 *   OUTPUTS           :  NONE
 *   RETURNS           :  OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et
oam_qcli_notification_ind_resp
(
 UInt8 status,
 oam_error_code_et   error_code,
 UInt16 transaction_id,
 UInt8 *err_string
 )
{
	UInt8 *p_api_msg = OAM_NULL;
	UInt8 *p_msg = OAM_NULL;

	UInt16 msg_length = OAM_ZERO,  err_string_len=OAM_ZERO;
	UInt8  error_str[80] = {0};
	UInt16 new_trans_id = OAM_ZERO;
	UInt16 old_trans_id = OAM_ZERO;

    /* SPR 17777 fix */
    qcliHandleUnusedParameter(transaction_id);

	new_trans_id = oam_get_fresh_trans_id(old_trans_id);

	err_string_len = oam_strlen((char *)err_string);

	msg_length += OAM_CSPL_HDR_SIZE + QCLI_HEADER_SIZE;
	msg_length += sizeof(UInt8);
	msg_length += sizeof(UInt16);
	msg_length += sizeof(UInt16);
	msg_length += sizeof(UInt16);
	msg_length += sizeof(UInt8);
	msg_length += err_string_len;

	if(OAM_ZERO != err_string_len)
		msg_length +=1; 

	if(OAM_FAILURE == oam_mem_alloc(msg_length,(void*)&p_msg, &error_code))
	{
		return OAM_FAILURE;
	}

	p_api_msg = p_msg;

	/* encode the CSPL header in the message */
	oam_construct_api_header(p_api_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_NOTIFICATION_IND_RESP,msg_length);

	p_api_msg += OAM_CSPL_HDR_SIZE;

	/* encode API header in message*/
	oam_construct_qcli_header((UInt8 *)p_api_msg,
			new_trans_id,
			QCLI_MODULE_ID,
			QCLI_INTERFACE_MODULE_ID,
			OAM_NOTIFICATION_IND_RESP,
			msg_length - OAM_CSPL_HDR_SIZE
			);


	p_api_msg = p_api_msg + QCLI_HEADER_SIZE;

	/* Pack NULL Character also in error string */
	if(OAM_ZERO != err_string_len)
	{
		oam_strcpy((char *)error_str,(char *)err_string);
		error_str[err_string_len]='\0';
		err_string_len +=1;
	}

	oam_qcli_encode_notif_payload((char *)p_api_msg,
			status,error_code,0,0,err_string_len,(char *)error_str);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);

	return OAM_SUCCESS;

}

/******************************************************************************
 * Function Name  : oam_qcli_encode_notif_payload 
 * Inputs         : p_ctrl_blk
 *                : resp_status
 *                : err_code
 *                : resvd1
 *                : resvd2
 *                : err_str_len
 *                : err_str  
 * Outputs        : none
 * Returns        : Nothing
 * Description    : 
 *******************************************************************************/

void 
oam_qcli_encode_notif_payload
(
 char *p_ctrl_blk,
 char resp_status,
 UInt16 err_code,
 UInt8 resvd1, 
 UInt32 resvd2,
 UInt8 err_str_len,
 char *err_str
 )
{
	UInt32 curr_pos = OAM_ZERO;
	*(p_ctrl_blk + curr_pos++) = resp_status;/*packing response status*/
	/*packing error code*/
	*(p_ctrl_blk + curr_pos++) = (err_code & 0xff00) >> 8;
	*(p_ctrl_blk + curr_pos++) = err_code & 0xff; 
	/*packing reserved field*/
	*(p_ctrl_blk + curr_pos++) = resvd1; 
	*(p_ctrl_blk + curr_pos++) = (resvd2 & 0xff0000) >> 16; 
	*(p_ctrl_blk + curr_pos++) = (resvd2 & 0xff00) >> 8; 
	*(p_ctrl_blk + curr_pos++) = resvd2 & 0xff;
	/*packing error string length*/
	*(p_ctrl_blk + curr_pos++) = err_str_len;

	/* Encode error string only when there is failure */
	if(err_str_len != OAM_ZERO)
		oam_memcpy(p_ctrl_blk + curr_pos, err_str, err_str_len);


}













