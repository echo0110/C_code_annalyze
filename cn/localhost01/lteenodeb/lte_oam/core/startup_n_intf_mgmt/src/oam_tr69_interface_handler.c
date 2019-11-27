/**********************************************************************************
 * File Name    : oam_tr69_interface_handler.c
 * Description : 
 * Revision History :
 * Date            Author           Reference              Comments
 * ---------       -------------    -----------            -------------
 * February,2013   Vishal Kumar     OAM design Document    Initial Version
 * June, 2014        Vivek K Gupta  Release 3.0         Changes for xml 
 *                                                      add/delete frwk code
 * JUne 2014        Shalu Sagar                         Coverity Fix
 * June,2014        Ravi Gupta             SPR 10567 Fix
 * July 2014        Shalu Sagar            SPR 12378 Fix
 * Aug  2014        Ravi Gupta             SPR 13176 Fix
 * Oct  2014        Ankita Jain            SPR 13586
 * Dec  2014        Vivek Gupta            SPR 14152
 * Dec  2014        Vivek Gupta            SPR 14153
 * Copyright (c) 2014, Aricent Inc. All Rights Reserved
 *************************************************************************************/

#include<oam_types.h>
#include <oam_proto.h>
#include<oam_defines.h>
#include<oam_sim_utils.h>
#include<oam_sim.h>
#include<oam_config_mgmt.h>
#include<oam_xml_header.h>
#include<oam_perf_mgmt.h>
#include <oam_son_composer.h>
#include "oam_rrm_composer.h"

/* CSR 57877 FIx Start */
#include <oam_mgmt_intf_api.h>
/* CSR 57877 FIx End*/
/* SPR 14082 START */
#include <oam_fm_alarm_defs.h>
/* SPR 14082 END */
/* SPR 15604 START */
#include "oam_xml_enum.h"
/* SPR 15604 END */



extern oam_prov_req_t oam_prov_req;
extern oam_prov_req_t local_prov_req;
extern int g_no_of_arguments;
extern Char8 **g_list_of_init_arguments;
struct XML_struct_tr196 xml_struct_tr196_g;
XML_struct_cell_config XML_struct_cell_config_g;
extern parameter_map_element_t g_parameter_TR_element;
extern parameter_map_element_t g_parameter_TR_InterFreq_Carrier_element;
extern parameter_map_element_t g_parameter_TR_UTRA_FDD_element;
extern parameter_map_element_t g_parameter_son_anr_GERANFreqGroup_params;
extern parameter_map_element_t g_parameter_TR_PLMNList_element;
extern parameter_map_element_t g_parameter_lte_intra_rat_neighbor_list;
extern parameter_map_element_t g_parameter_lte_inter_rat_neighbor_list;
/*SPR 15604 START */
extern parameter_map_element_t g_param_intra_rat_neigh_list_in_use;
extern parameter_map_element_t g_param_inter_rat_neigh_list_in_use;
/*SPR 15604 END */

/* SPR-13586 START */
extern parameter_map_element_t g_parameter_lte_inter_rat_geran_neighbor_list;
/* SPR-13586 END */

/* SPR 13850 FIX START */
extern parameter_map_element_t *g_parameter_map_element;
/* SPR 13850 FIX END */
/* SPR 15527 START */                                                  
extern parameter_map_element_t g_parameter_TR_QOS_element;
void oam_find_fap_index(Char8 *p_parent_name, UInt8 *p_fap_index_value);
/* SPR 15527 END */                                                  

/*OAM CM FSM context*/
extern oam_cm_cntxt_t *p_cm_cntxt;
extern UInt16 g_upgrade_req_retry_counter;

/* spr_21013_fix_start */
/* Parameter name is changed from FAPService [1] to FAPService */
object_map_element_t g_object_list[OAM_MAX_SUPPORTED_OBJECTS] = {
    {
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE."
            "EPC.PLMNList",
/* SPR 16603 Fix Start */
        OAM_FOUR,
/* SPR 16603 Fix End */
        /* Pointer to the PLMNList parameters table */
        &g_parameter_TR_PLMNList_element,
        /* accessor function pointer to access current number of entries,
           max number of entries, pointer to parent xml node of the object,
           pointer to first sibling of the object
        */
        oam_cell_plmnlist_data_accessor,
 			/* Lower layer handler */
        oam_process_add_delete_plmn_list,
 		/* Instance ID Genertor for PLMNList */
        oam_generate_instance_id_for_plmn_list,
        "PLMNList",	/* Object tag name*/	
        "PLMNListNumberOfEntries"
/* SPR 15527 START */                                                  
    }, 
    {
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.EPC.QoS",
        OAM_SIXTY_FIVE,
        &g_parameter_TR_QOS_element,			/* Pointer to the QoS List parameters */
        oam_qos_data_accessor,                  /* Parent Node of object */
        oam_process_add_delete_qos_list, 			/* Lower layer handler */
        oam_generate_instance_id_for_qos_list, 		/* Instance ID Genertor for QoS List */
        "QoS",						/* Object Name*/	
        "QoSNumberOfEntries"
/* SPR 15527 END */                                                  
    }  
#ifdef OAM_SON_ENABLE  
    ,{
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE."
            "RAN.Mobility.IdleMode.InterFreq.Carrier",
/* SPR 16603 Fix Start */
            /* SPR 14651 START */
            OAM_TWENTY_FOUR,
            /* SPR 14651 END */
/* SPR 16603 Fix End */
 		/* Pointer to parameter table */
        &g_parameter_TR_InterFreq_Carrier_element,
        /* accessor function pointer to access current number of entries,
           max number of entries, pointer to parent xml node of the object,
           pointer to first sibling of the object
        */
        oam_interfreq_carrier_data_accessor,
		/* Lower layer handler */
        oam_process_add_delete_inter_freq_carrier_list,
 	/* Instance ID Genertor for Inter Freq Carrier List */
        oam_generate_instance_id_for_inter_freq_carrier_list,
        "Carrier",
        "CarrierNumberOfEntries"
    },
    {
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE."
            "RAN.Mobility.IdleMode.IRAT.UTRA.UTRANFDDFreq",
/* SPR 16603 Fix Start */
        /* SPR 14651 START */
        OAM_ELEVEN,
        /* SPR 14651 END */
/* SPR 16603 Fix End */
 		/* Pointer to parameter table */
        &g_parameter_TR_UTRA_FDD_element,
        /* accessor function pointer to access current number of entries,
           max number of entries, pointer to parent xml node of the object,
           pointer to first sibling of the object
        */
        oam_utran_fdd_freq_data_accessor,
		/* Lower layer handler */
        oam_process_add_delete_utran_fdd_freq_list,
        /* Instance ID Genertor for UTRANFDDFreq List */
        oam_generate_instance_id_for_utran_fdd_freq_list,
        "UTRANFDDFreq",
        "UTRANFDDFreqNumberOfEntries"
    },
    {
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE."
            "RAN.Mobility.IdleMode.IRAT.GERAN.GERANFreqGroup",
/* SPR 16603 Fix Start */
        /* SPR 14651 START */
        OAM_FOURTEEN,
        /* SPR 14651 END */
/* SPR 16603 Fix End */
 		/* Pointer to parameter table */
        &g_parameter_son_anr_GERANFreqGroup_params,
        /* accessor function pointer to access current number of entries,
           max number of entries, pointer to parent xml node of the object,
           pointer to first sibling of the object
        */
        oam_geran_freq_group_data_accessor,
		/* Lower layer handler */
        oam_process_add_delete_geran_freq_group_list,
        /* Instance ID Genertor for GERANFreqGroup List */
        oam_generate_instance_id_for_geran_freq_group_list,
        "GERANFreqGroup",
        "GERANFreqGroupNumberOfEntries"
    },
    {
        "InternetGatewayDevice.Services.FAPService.CellConfig."
        "LTE.RAN.NeighborList.LTECell",
/* SPR 16603 Fix Start */
        OAM_TWENTY_SIX,
/* SPR 16603 Fix End */
        /* Pointer to parameter table*/
        &g_parameter_lte_intra_rat_neighbor_list,
        /* accessor function pointer to access current number of entries,
           max number of entries, pointer to parent xml node of the object,
           pointer to first sibling of the object
        */
        oam_neighborlist_ltecell_data_accessor,
         /* Lower layer handler */
        oam_process_add_delete_NeighborList_LTECell,
        /* Instance ID Genertor for Inter Freq Neighborlist List */
        oam_generate_instance_id_for_NeighborList_LTECell,
        "LTECell",
        "LTECellNumberOfEntries"
    },
    {
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE."
            "RAN.NeighborList.InterRATCell.UMTS",
/* SPR 16603 Fix Start */
            OAM_INTER_RAT_UTRAN_NEIGH_PARAMS_COUNT,
/* SPR 16603 Fix End */
        /* Pointer to parameter table*/
        &g_parameter_lte_inter_rat_neighbor_list,
        /* accessor function pointer to access current number of entries,
           max number of entries, pointer to parent xml node of the object,
           pointer to first sibling of the object
        */
        oam_neighborlist_interratcell_umts_data_accessor,
         /* Lower layer handler */
        oam_process_add_delete_NeighborList_InterRATCell_UMTS,
        oam_generate_instance_id_for_NeighborList_UMTS,
        "UMTS",
        "UMTSNumberOfEntries"
    },
    /* SPR-13586 START */
    {
        "InternetGatewayDevice.Services.FAPService.CellConfig.LTE."
            "RAN.NeighborList.InterRATCell.GSM",
/* SPR 16603 Fix Start */
        OAM_INTER_RAT_GSM_NEIGH_PARAMS_COUNT,
/* SPR 16603 Fix End */
        /* Pointer to parameter table*/
        &g_parameter_lte_inter_rat_geran_neighbor_list,
        /* accessor function pointer to access current number of entries,
           max number of entries, pointer to parent xml node of the object,
           pointer to first sibling of the object
        */
        oam_neighborlist_interratcell_gsm_data_accessor,
         /* Lower layer handler */
        oam_process_add_delete_NeighborList_InterRATCell_GSM,
        oam_generate_instance_id_for_NeighborList_GSM,
        "GSM",
        "GSMNumberOfEntries"
    },
    /*SPR 15604 START*/
    {
	    "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.NeighborListInUse.LTECell",
	    /* Object Name */
/*SPR 17059 FIX START */
/* SPR 17454 changes start */
        /* SPR 17525 changes start */
        11,
        /* SPR 17525 changes ends */
/* SPR 17454 changes end */
/*SPR 17059 FIX END */
	    &g_param_intra_rat_neigh_list_in_use,  
	    /* Pointer to parameter table*/
	    oam_neighborlistInUse_ltecell_data_accessor, 
	    /* Lower layer handler */
        /* SPR 18930  FIX START */
        NULL,
        OAM_NULL,
        /* SPR 18930  FIX END */
	    /* Instance ID Genertor for Inter Freq Neighborlist List */
	    "LTECell",                                            
	    /* Object Name*/
	    "LTECellNumberOfEntries"
    },
    {
	    "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.NeighborListInUse.InterRATCell.UMTS",/* Object Name */
	    /* Number of parameter entries in the
	       g_parameter_lte_inter_rat_neighbor_list table */
        /* SPR 18930  FIX START */
	    16,
        /* SPR 18930  FIX END */
	    &g_param_inter_rat_neigh_list_in_use,
	    /* Pointer to parameter table*/
	    oam_neighborlistInUse_interratcell_umts_data_accessor,
	    /* Lower layer handler */
        /* SPR 18930  FIX START */
        NULL,
        /* SPR 18930  FIX END */
        OAM_NULL,
	    "UMTS",                                              /* Object Name*/
	    "UMTSNumberOfEntries"
    }
    /*SPR 15604 END*/

    /* SPR-13586 END */
#endif
};
/* spr_21013_fix_end */
/* SPR 15604 START */
#ifdef OAM_SON_ENABLE
enumerated_string_fields_mapping_tbl_t NeighborList_Mapping_Table[OAM_THIRTY_TWO]=
{
    {"PLMNID", PLMNID},
    {"PhyCellID",PhyCellID},
    {"CID",CID},
    {"QOffset",QOffset},
    {"CIO",CIO},
    {"EUTRACarrierARFCN",EUTRACarrierARFCN},
    {"X_VENDOR_TAC",X_VENDOR_TAC},
    {"X_VENDOR_CSGID",X_VENDOR_CSGID},
    {"Blacklisted", Blacklisted},
    {"X_VENDOR_RSRP",X_VENDOR_RSRP},
    {"X_VENDOR_RSRQ",X_VENDOR_RSRQ},
    {"X_VENDOR_NR_STATUS",X_VENDOR_NR_STATUS},
    {"X_VENDOR_BROADCAST_STATUS",X_VENDOR_BROADCAST_STATUS},
    {"X_VENDOR_PRACH_ROOT_SEQUENCE_INDEX",X_VENDOR_PRACH_ROOT_SEQUENCE_INDEX},
    {"X_VENDOR_PRACH_ZERO_CORRELATION_ZONE_CONFIG",X_VENDOR_PRACH_ZERO_CORRELATION_ZONE_CONFIG},
    {"X_VENDOR_PRACH_HIGH_SPEED_FLAG",X_VENDOR_PRACH_HIGH_SPEED_FLAG},
    {"X_VENDOR_PRACH_FREQUENCY_OFFSET",X_VENDOR_PRACH_FREQUENCY_OFFSET},
    {"X_VENDOR_PRACH_CONFIGURATION_INDEX",X_VENDOR_PRACH_CONFIGURATION_INDEX},
    {"X_VENDOR_ACCESS_MODE",X_VENDOR_ACCESS_MODE},
    {"PCPICHScramblingCode",PCPICHScramblingCode},
    {"RNCID", RNCID},
    {"LAC", LAC},
    {"RAC", RAC},
    {"X_VENDOR_RSCP", X_VENDOR_RSCP},
    {"X_VENDOR_CELL_SPECIFIC_OFFSET", X_VENDOR_CELL_SPECIFIC_OFFSET},
    {"X_VENDOR_NUMPLMNID", X_VENDOR_NUMPLMNID},
    {"X_VENDOR_PLMN_ID", X_VENDOR_PLMN_ID},
    {"X_VENDOR_ECNO", X_VENDOR_ECNO},
    {"X_VENDOR_PS_HO_SUPPORTED", X_VENDOR_PS_HO_SUPPORTED},
    {"X_VENDOR_VOIP_CAPABLE", X_VENDOR_VOIP_CAPABLE},
    {"UARFCNDL", UARFCNDL},
    {"X_VENDOR_HO_STATUS", X_VENDOR_HO_STATUS}
};
#endif
/*SPR 15604 END*/

/******************************************************************************
 * Function Name : oam_tr069_intf_msg_handler 
 * Description   : This function handles messages for TR069
 * Inputs        : p_api_buf : Pointer to API Buffer
 *                 api_id : API ID
 *                 src_module_id : Source Module Identifier
 *                 msg_len :Message Length
 * Output        : NONE 
 * Returns       : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et oam_tr069_intf_msg_handler
(
 void *p_api_buf, 
 UInt16 api_id, 
 UInt16 src_module_id, 
 UInt16 msg_len
 )
{

    UInt16    new_trans_id = OAM_ZERO;
    UInt16 trans_id = OAM_ZERO;
    UInt8 usr_resp = OAM_RESULT_SUCCESS;
    UInt16 usr_err_code = OAM_ZERO;
    Char8 *str_err = OAM_NULL;
    UInt8 str_err_len = OAM_ZERO;
    UInt8     dest_id = OAM_ZERO;
    UInt16    guard_time = OAM_INIT_TIMER_VALUE;
    /* SPR 13832 START */ 
    UInt32    timer_data   = OAM_ZERO;
    /* SPR 13832 END */ 
    oam_timer_t  timer = OAM_ZERO;
    oam_return_et  ret_val = OAM_SUCCESS;
    oam_error_code_et err= NO_ERROR;
    UInt16 out_inst_num = OAM_ZERO;

    /* SPR 15387 Start */
    UInt32 initial_event_ongoing_value =
	    oam_prov_req.system_status.event_ongoing; // Incase of SPV failure, this
    // value will be used to revert
    // oam_prov_req.system_status.event_ongoing
    /* SPR 15387 End */



    OAM_FUN_ENTRY_TRACE();
    trans_id = get_word_from_header((UInt8*)(p_api_buf) + OAM_ZERO);
    dest_id = get_word_from_header((UInt8*)(p_api_buf) + OAM_TWO);

    new_trans_id = oam_get_new_trans_id();

    switch(api_id)
    {
        case TR069_OAM_INIT_IND :
            /* SPR 13832 START */ 
            oam_sprintf((char *)&timer_data,"%d",TR069_MODULE_ID); 
            /* SPR 13832 END */ 
            timer = oam_start_timer(guard_time, &timer_data,
                    sizeof(timer_data), OAM_FALSE);
            if (OAM_NULL == timer)
            {
                ret_val = OAM_FAILURE;
                break;
            }
            p_cm_cntxt->guard_timer_id = timer;

            if(OAM_SUCCESS == oam_send_tr_069_init_config_req(&(oam_prov_req.oam_tr069_req.tr069_init_params),OAM_MODULE_ID,
                        new_trans_id,&err, &(oam_prov_req.oam_igd_params.device_info_params)))
            {
                OAM_LOG(OAM,OAM_DETAILED,"TR069_OAM_CONFIG_REQ SENT SUCCESSFULLY"); 
            }
            break;
        case TR069_OAM_CONFIG_RESP : 
            if (OAM_SUCCESS != oam_stop_timer(p_cm_cntxt->guard_timer_id))
            {
                ret_val = OAM_FAILURE;
                break;
            }

            if(OAM_SUCCESS == oam_handle_tr069_init_resp(p_api_buf))
            {
                OAM_LOG(OAM,OAM_DETAILED,"TR069_OAM_CONFIG_RESP RECEIVED SUCCESSFULLY");
#ifdef OAM_SON_ENABLE
                oam_sim_error_info_et start_error = SIM_NO_ERROR;
                if((oam_prov_req.system_status.is_nmm_enabled) &&
                   (OAM_TRUE == oam_prov_req.oam_tr069_req.connect_to_iHeMS) && 
                   (OAM_TRUE == oam_prov_req.oam_tr069_req.tr069_mode))
                {   
                    OAM_LOG(OAM, OAM_DETAILED, "Initial startup of RRM and SON");
                    g_oam_nmm_scan_first_time = OAM_TRUE;
                    oam_startup_init_rrm_son(&start_error);
                }
#endif
            } 
            break;

        case OAM_CONTROL_COMMAND_REQ :
            if((OAM_FALSE == oam_prov_req.system_status.tr069_configured) ||
                    (OAM_ZERO != oam_prov_req.system_status.event_ongoing))  
            {
                OAM_LOG(OAM,OAM_WARNING,"TR069 Agent not configured yet");   
                ret_val = oam_send_control_cmd_resp(trans_id,dest_id,
                        OAM_RESULT_FAILURE,
                        OAM_ERR_REQUEST_DENIED,
                        (UInt8 *)"Request Denied");
            } 
            else if(OAM_SUCCESS == oam_handle_control_command_req(p_api_buf))
            {
                OAM_LOG(OAM,OAM_DETAILED,"OAM_CONTROL_COMMAND_REQ PROCESSED SUCCESSFULLY");
            } 
            break;

        /* SPR 17596 fixed start */
        case OAM_GET_RPC_METHOD:
            
            if(OAM_FALSE == oam_prov_req.system_status.tr069_configured)
            {
                OAM_LOG(OAM,OAM_WARNING,"TR069 Agent not configured yet");

                oam_construct_n_send_get_resp_to_usr(OAM_RESULT_FAILURE,
                        OAM_ERR_REQUEST_DENIED, OAM_VERSION,
                        OAM_MODULE_ID, dest_id,
                        trans_id,OAM_ZERO,OAM_ZERO,
                        "Request Denied",&err);
            }
                else if(OAM_SUCCESS == oam_get_rpc_api_handler(p_api_buf, msg_len, &err))
                {
                    OAM_LOG(OAM,OAM_DETAILED,"oam_get_rpc_api_handler success");
                    ret_val = OAM_SUCCESS;
               }
               else
               {
                   OAM_LOG(OAM,OAM_WARNING,"oam_get_rpc_api_handler failed");

                   oam_construct_n_send_get_rpc_resp_to_usr(OAM_RESULT_FAILURE,
                        OAM_ERR_REQUEST_DENIED, OAM_VERSION,
                        OAM_MODULE_ID, dest_id,
                        trans_id,"Request Denied",&err);

                ret_val = OAM_FAILURE;
             }
            break;
            
        /* SPR 17596 fixed end */
        case OAM_GET_PARAMETER_VALUES:
            if((OAM_FALSE == oam_prov_req.system_status.tr069_configured) && (dest_id != OAM_SNMP_INTF_MODULE_ID))
            {
                OAM_LOG(OAM,OAM_WARNING,"TR069 Agent not configured yet");  

                oam_construct_n_send_get_resp_to_usr(OAM_RESULT_FAILURE, 
                        OAM_ERR_REQUEST_DENIED, OAM_VERSION,
                        OAM_MODULE_ID, dest_id,
                        trans_id,OAM_ZERO,OAM_ZERO,
                        "Request Denied",&err);

            }
            else if(OAM_SUCCESS==oam_get_parameter_api_handler(p_api_buf, msg_len, src_module_id,&err))
            {
                OAM_LOG(OAM,OAM_DETAILED,"oam_get_parameter_value_api_handler success");
                ret_val = OAM_SUCCESS;
            }
            else
            {
                /* SPR 21390 fix start */
                oam_get_err_string_n_err_code(err, &usr_err_code,
                        &str_err);

                usr_resp = OAM_RESULT_FAILURE;

                OAM_LOG(OAM, OAM_WARNING,
                        "oam_get_parameter_values_api_handler failed."
                        "Error: %s", str_err);

                oam_construct_n_send_get_resp_to_usr(usr_resp,
                        usr_err_code,
                        OAM_VERSION,
                        OAM_MODULE_ID,
                        dest_id,
                        trans_id,
                        OAM_ZERO,
                        OAM_ZERO,
                        str_err,
                        &err);
                /* SPR 21390 fix end */

                ret_val = OAM_FAILURE;
            }
            break;

        case OAM_SET_PARAMETER_VALUES:
        /* SPR 15527 START */
	    if(((OAM_FALSE == oam_prov_req.system_status.tr069_configured) &&
        /* SPR 15527 END */
            (OAM_ZERO != oam_prov_req.system_status.event_ongoing)) && (dest_id != OAM_SNMP_INTF_MODULE_ID ))
	    {
		    OAM_LOG(OAM,OAM_WARNING,"TR069 Agent not configured yet");   
		    oam_construct_n_send_set_resp_to_usr(OAM_RESULT_FAILURE,
                        OAM_ERR_REQUEST_DENIED,
                        (UInt8 *)"Request Denied", oam_strlen("Request Denied")+1, 
                        OAM_VERSION_ID, OAM_MODULE_ID,
                        dest_id, trans_id, OAM_ZERO, OAM_ZERO,
                        &err);

            }
            else if (OAM_FAILURE ==
                    oam_set_parameter_values_api_handler(
                        p_api_buf, msg_len, src_module_id, &err))
            {
				/*BUG 1233 FIX START*/
		    /* SPR 15387 Start */
		    oam_prov_req.system_status.event_ongoing = initial_event_ongoing_value;
		    /* SPR 15387 End */

            /* SPR 21390 fix start */
            oam_get_err_string_n_err_code(err, &usr_err_code,
                    &str_err);
            OAM_LOG(OAM, OAM_WARNING,
                    "oam_set_parameter_values_api_handler failed. "
                    "Error: %s", str_err);

            str_err_len = oam_strlen(str_err) + OAM_ONE;
            oam_construct_n_send_set_resp_to_usr(OAM_RESULT_FAILURE,
                    usr_err_code,
                    (UInt8*)str_err,
                    str_err_len,
                    OAM_VERSION_ID,
                    OAM_MODULE_ID,
                    dest_id, trans_id,
                    OAM_ZERO, OAM_ZERO,
                    &err);
            OAM_LOG(OAM, OAM_ERROR, "%s", str_err);
            /* SPR 21390 fix end */

             ret_val = OAM_FAILURE;
            }  

            break;
        case OAM_FILE_DOWNLOAD_IND :

            if((OAM_FALSE == oam_prov_req.system_status.tr069_configured) ||
                    (OAM_ZERO != oam_prov_req.system_status.event_ongoing))  
            {
                OAM_LOG(OAM,OAM_WARNING,"TR069 Agent not configured yet");   	
                ret_val = oam_send_file_download_ind_resp(OAM_RESULT_FAILURE,
                        OAM_ERR_REQUEST_DENIED,trans_id,
                        (UInt8 *)"Request Denied");
            } 
            else if(OAM_SUCCESS == oam_handle_file_download_ind(p_api_buf))
            {
                OAM_LOG(OAM,OAM_DETAILED,"File download Indication handled succesfully"); 
                ret_val = OAM_SUCCESS;

            }  

            break;

        case OAM_ADD_OBJECT_REQ:
            if((OAM_FALSE == oam_prov_req.system_status.tr069_configured) ||
                    (OAM_ZERO != oam_prov_req.system_status.event_ongoing))  
            {
                OAM_LOG(OAM, OAM_WARNING, "TR069 Agent not configured yet");
                usr_resp = OAM_RESULT_FAILURE;
                usr_err_code = OAM_ERR_REQUEST_DENIED;
                str_err = "TR069 Agent not configured yet";
                str_err_len = oam_strlen(str_err) + 1;
                out_inst_num = OAM_ZERO;
            }
            else {
                ret_val = oam_add_object_api_handler(p_api_buf, &err,
                                                     &out_inst_num);

                if (OAM_SUCCESS == ret_val) {
                    usr_resp = OAM_RESULT_SUCCESS;
                    usr_err_code = OAM_ZERO;
                    str_err_len = OAM_ZERO;

/* SPR 16603 Fix Start */
                    oam_prov_req.system_status.tr_event_ongoing |= TR069_ADD_OBJ_ONGOING;
/* SPR 16603 Fix End */
                    OAM_LOG(OAM, OAM_DETAILED,
                                 "oam_add_object_api_handler success");
                }
                else {
                    oam_get_err_string_n_err_code(err, &usr_err_code,
                                                  &str_err);
                    usr_resp = OAM_RESULT_FAILURE;
                    str_err_len = oam_strlen(str_err) + 1;

                    OAM_LOG(OAM, OAM_WARNING, "oam_add_object_api_handler failed");
                }
            }

            oam_construct_n_send_addobject_resp_to_usr(usr_resp,   
                                                       usr_err_code,
                                                       (UInt8*)str_err,
                                                       str_err_len,
                                                       OAM_VERSION_ID,
                                                       OAM_MODULE_ID,
                                                       dest_id,
                                                       trans_id,
                                                       OAM_ZERO, OAM_ZERO,
                                                       &err,
                                                       out_inst_num);
            break;

        case OAM_DELETE_OBJECT_REQ :

            if((OAM_FALSE == oam_prov_req.system_status.tr069_configured) ||
                    (OAM_ZERO != oam_prov_req.system_status.event_ongoing))  
            {
                OAM_LOG(OAM, OAM_WARNING, "TR069 Agent not configured yet");
                usr_resp = OAM_RESULT_FAILURE;
                usr_err_code = OAM_ERR_REQUEST_DENIED;
                str_err = "TR069 Agent not configured yet";
                str_err_len = oam_strlen(str_err) + 1;
            }
            else {
                ret_val = oam_delete_object_api_handler(p_api_buf, &err);

                if (OAM_SUCCESS == ret_val) {
                    /* Spr 15711 Fix Start */
                    /* Loging Removed */  
                    /* Spr 15711 Fix End */
                    usr_resp = OAM_RESULT_SUCCESS;
                } 
                else {
                    oam_get_err_string_n_err_code(err, &usr_err_code,
                                                  &str_err);
                    usr_resp = OAM_RESULT_FAILURE;
                    str_err_len = oam_strlen(str_err) + 1;

                    OAM_LOG(OAM, OAM_WARNING, "oam delete object api  failed");
                }
            }

            if ((OAM_CM_ST_ACTIVE !=  p_cm_cntxt->state) ||
                 (OAM_FAILURE == ret_val)) {
                oam_construct_n_send_addobject_resp_to_usr(usr_resp,   
                                                       usr_err_code,
                                                       (UInt8*)str_err,
                                                       str_err_len,
                                                       OAM_VERSION_ID,
                                                       OAM_MODULE_ID,
                                                       dest_id,
                                                       trans_id,
                                                       OAM_ZERO, OAM_ZERO,
                                                       &err,
                                                       out_inst_num);
            }
            break;

        case OAM_NOTIFICATION_IND_RESP :
	    /*SPR 15143 Fix Start*/
	    if((OAM_FALSE == oam_prov_req.system_status.tr069_configured) &&
			    (OAM_ZERO != oam_prov_req.system_status.event_ongoing))
            /*SPR 15143 Fix End*/
            {
                OAM_LOG(OAM,OAM_WARNING,"TR069 Agent not configured yet");   
            } 
            else if(OAM_SUCCESS == oam_handle_notification_ind_resp(p_api_buf))
            {
                OAM_LOG(OAM,OAM_DETAILED," Notification response handled succesfully"); 
                ret_val = OAM_SUCCESS;
            } 
            else
            {
                OAM_LOG(OAM,OAM_WARNING," notifictaion ack failed");
                ret_val = OAM_FAILURE;
            }
            break;
        case TR069_CONTROL_COMMAND_RESP :

            if((OAM_FALSE == oam_prov_req.system_status.tr069_configured) ||
                    (OAM_ZERO != oam_prov_req.system_status.event_ongoing))  
            {
                OAM_LOG(OAM,OAM_WARNING,"TR069 Agent not configured yet");   
            } 
            else if(OAM_SUCCESS == oam_handle_tr069_command_resp(p_api_buf))
            {
                OAM_LOG(OAM,OAM_DETAILED," Controll command response handled succesfully"); 
                ret_val = OAM_SUCCESS;

            } 
            else
            {
                OAM_LOG(OAM,OAM_WARNING,"Tr069 Control command response failed");
                ret_val = OAM_FAILURE;

            }
            break;

    }
    OAM_FUN_EXIT_TRACE();
    return ret_val; 
}

/******************************************************************************
 * Function Name  : oam_send_file_download_ind_resp  
 * Description    : This function sends message to TR069 in response to file 
 *                  download indication   
 * Inputs         : status : Indicates success and failure  
 *                  error_code : Indicates type of Error 
 *                  transaction_id : transaction identifier
 *                  err_string : Pointer to Description of Error 
 * Output         : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE 
 ******************************************************************************/
oam_return_et
oam_send_file_download_ind_resp
(
 UInt8 status,
/* coverity 35908 - 35914*/
 UInt16   err_code,
 UInt16 transaction_id,
 UInt8 *err_string
 )
{

    OAM_FUN_ENTRY_TRACE();
    UInt8 *p_api_msg = OAM_NULL;
    UInt8 *p_msg = OAM_NULL;
    UInt16 msg_length = OAM_ZERO,err_string_len=OAM_ZERO;
    Char8  error_str[OAM_USR_INTF_ERR_STR_MAX_SIZE] = {OAM_ZERO};
    oam_error_code_et   error_code = NO_ERROR; 	

    err_string_len = oam_strlen((Char8*)err_string);

    msg_length += OAM_CSPL_HDR_SIZE + OAM_TR069_INTF_HEADER_SIZE;
    msg_length += sizeof(UInt8);
    msg_length += sizeof(UInt16);
    msg_length += sizeof(UInt16);
    msg_length += sizeof(UInt16);
    msg_length += sizeof(UInt8);
    msg_length += err_string_len;

    if(OAM_ZERO != err_string_len)
        msg_length += OAM_ONE;

    if(OAM_FAILURE == oam_mem_alloc(msg_length,(void*)&p_msg, &error_code))
    {
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
                error_code);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
    p_api_msg = p_msg;
    /* encode the CSPL header in the message */
    oam_construct_api_header(p_api_msg,OAM_VERSION,OAM_MODULE_ID,
            TR069_MODULE_ID,
            OAM_FILE_DOWNLOAD_IND_RESP,msg_length);

    p_api_msg += OAM_CSPL_HDR_SIZE;

    /* encode API header in message*/
    oam_construct_mgmt_api_hdr((UInt8 *)p_api_msg,
            transaction_id,
            OAM_MODULE_ID,
            TR069_MODULE_ID,
            OAM_FILE_DOWNLOAD_IND_RESP,
            msg_length - OAM_CSPL_HDR_SIZE,
            OAM_ZERO);
            /*SPR 17777 fix*/

    p_api_msg = p_api_msg + OAM_TR069_INTF_HEADER_SIZE;

    /* Pack NULL Character also in error string */
    if(OAM_ZERO != err_string_len)
    {
        oam_strncpy((Char8*)error_str, (const Char8 *)err_string, sizeof(error_str));
        err_string_len += OAM_ONE;
    }
    encode_get_params_resp_ctrl_blk((char *)p_api_msg,
            status,err_code,OAM_ZERO,OAM_ZERO,err_string_len,error_str);

    if(OAM_FAILURE == oam_send_message(p_msg,TR069_MODULE_ID,&error_code))
    {
        OAM_LOG(OAM,OAM_WARNING,"Error in sending message : %d",error_code);
        OAM_FUN_ENTRY_TRACE();
        return OAM_FAILURE;
    }
    return OAM_SUCCESS;

}


/******************************************************************************
 * Function Name  : oam_handle_tr069_init_resp 
 * Description    : This function handles response from  TR069 in response to
 *                  initial configuration request   
 * Inputs         : p_buf : Pointer to message buffer
 *                  oam_tr069_init_config_resp
 * Output         : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE  

 ******************************************************************************/
oam_return_et
oam_handle_tr069_init_resp
(
 void *p_buf 
 )
{
    UInt8 status = OAM_ZERO; 
    UInt16 error_code = OAM_ZERO;
    UInt16 counter = OAM_ZERO;
    UInt8 len_str = OAM_ZERO; 
    UInt8 err_string[STR_LEN_255]={OAM_ZERO}; 
    oam_return_et retVal = OAM_SUCCESS;
/* SPR 14082 START */
    Char8 *p_al_add_info = OAM_NULL;
    Char8 *p_al_add_txt = OAM_NULL;
/* SPR 14082 END */

    OAM_FUN_ENTRY_TRACE();
    p_buf += OAM_TR069_INTF_HEADER_SIZE;   
    oam_memcpy(&status,p_buf,sizeof(UInt8)); 
    counter += sizeof(UInt8);
    OAM_LOG(OAM,OAM_DETAILED," Status : %d",status);  
    if(OAM_RESULT_FAILURE == status)
    {
	    error_code  |= (UInt16)((((UInt8*)p_buf)[counter++] << OAM_EIGHT));
	    error_code  |= (UInt16)(((UInt8*)p_buf)[counter++]); 
	    counter += sizeof(UInt16);
	    counter += sizeof(UInt16);

	    oam_memcpy(&len_str,p_buf+counter,sizeof(UInt8));    
	    counter += sizeof(UInt8); 
	    oam_memcpy(err_string,p_buf+counter,len_str); 

	    OAM_LOG(OAM,OAM_INFO,"Error Code : %d. "
	            "Error String : %s",error_code, err_string);

        /* SPR 14082 START */
        p_al_add_info = "TR069 Layer initialization failed";
        retVal = OAM_FAILURE;

        oam_handle_init_config_failure(
                CRITICAL_CONFIGURATION_FAILURE_ALARM_ID,
                p_al_add_txt,p_al_add_info);

        return retVal; 
        /* SPR 14082 END */

    }
    else if(OAM_RESULT_SUCCESS == status)
    {
	    /* Indicates TR069 Agent gets successfully configured */
	    oam_prov_req.system_status.tr069_configured = OAM_TRUE;
    }
    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS; 

}

/******************************************************************************
 * Function Name  : oam_add_object_api_handler 
 * Description    : This function handles add object api from user mgmt i/f
 * Inputs         : p_msg : Pointer to message Buffer
 * Output         : p_err, p_inst_num
 * Returns        : OAM_SUCCESS/OAM_FAILURE  
 ******************************************************************************/
oam_return_et
oam_add_object_api_handler(void *p_msg,
                           oam_error_code_et *p_err,
                           UInt16 *p_inst_num)
{
    UInt16 length  = OAM_ZERO; 
    Char8  obj_name[MAX_PARENT_NAME_LENGTH] = {OAM_ZERO};
    UInt8 *p_offset = p_msg;
    oam_return_et retVal = OAM_FAILURE;
        /* SPR 18930  FIX START */
    UInt16 * curr_num_entries = OAM_NULL; /*to hold the address od current entries */
        /* SPR 18930  FIX END */
    UInt16 max_num_entries = OAM_ZERO;
    /* SPR 16603 Fix Start */
    UInt8 cell_idx = OAM_ZERO;
    xmlNode *p_obj_parent = OAM_NULL; 
    xmlNode *p_obj_sibbling = OAM_NULL; 
    xmlNode **p_p_obj_struct_parent = OAM_NULL; 
    /* SPR 16603 Fix End */
   /* SPR 19977 Fix Start*/
    UInt16 trans_id = get_word_from_header((UInt8*)(p_msg) + OAM_ZERO);
    UInt16 dest_id = get_word_from_header((UInt8*)(p_msg) + OAM_TWO);
    oam_error_code_et err=NO_ERROR;
   /* SPR 19977 Fix End*/
    UInt8 loop = OAM_ZERO;
    /* SPR 15527 START */
    UInt8 fap_index_value = OAM_ZERO;
    UInt8 current_cid = OAM_ZERO;
	oam_cell_status_t *p_cell_status = OAM_NULL;
    /* SPR 15527 END */
    /* SPR 22792 Fix Starts */
    /* SPR 22248 MBMS Changes Start */
    /* Code Deleted */
    /* SPR 22248 MBMS Changes Stop */
    /* SPR 22792 Fix Ends */
    OAM_FUN_ENTRY_TRACE();

    /* Increment the pointer past the API header */
    p_offset += OAM_TR069_INTF_HEADER_SIZE;   

    /*Increment the pointer past Reserved bytes */
    p_offset += sizeof(UInt32);

    /* Length of the object name in question */
    length = get_word_from_header(p_offset);  

    /*Increment the pointer past length field read */
    p_offset += sizeof(UInt16);

    /*Increment the pointer past reserved bytes */
    p_offset += sizeof(UInt16);

    if(length < MAX_PARENT_NAME_LENGTH) {
        oam_memcpy(obj_name, (Char8*)p_offset, length);

        OAM_LOG(OAM, OAM_DETAILED, "Object name = %s", obj_name);
        /* SPR 15527 START */
        oam_find_fap_index(obj_name,&fap_index_value);
        if(fap_index_value)
        {    
            current_cid = oam_prov_req.cell_cntxt.cell_ctxt_status[fap_index_value - 1].cid; 
            p_cell_status = oam_get_cell_cntxt_status(current_cid);

            if (p_cell_status) 
            {
                oam_prov_req.cell_cntxt.curr_cell_cid = current_cid;

                OAM_LOG(OAM, OAM_INFO,"Cell Context Changed to CID: %d, fap_index_value %d", 
                        current_cid,fap_index_value);
            }
            else {
                OAM_LOG(OAM, OAM_ERROR,"Invalid Cell ID: %d Cell Context not changed, fap_index_value %d",
                        current_cid,fap_index_value);
                *p_err = OAM_INVALID_CELL_CONTEXT;
                OAM_FUN_EXIT_TRACE();
                return OAM_FAILURE;
            }
        }
        /* SPR 15527 END */

        for (loop = OAM_ZERO; loop < OAM_MAX_SUPPORTED_OBJECTS; ++loop) {
        /* Search object name in object_map_element_t */
        /* If not found, send failure add object response */
            if (!oam_strcmp(obj_name, g_object_list[loop].object_name)) {
                retVal =
                   g_object_list[loop].obj_data_accessor(&curr_num_entries,
                                                        &max_num_entries,
/* SPR 16603 Fix Start */                
                                                        &p_obj_parent, 
                                                        &p_obj_sibbling,
                                                        &p_p_obj_struct_parent,
                                                        &cell_idx,
/* SPR 16603 Fix End */                
                                                        p_err);

/* SPR 16603 Fix Start */                
                /* SPR 17526 changes start */
                /* ADD OBJECT fix start */
        /*SPR 19977 FIX START*/ 
		if(OAM_NULL!=strstr(obj_name,"NeighborList.InterRATCell"))
		{ 
                while( (p_obj_sibbling) && (p_obj_sibbling->next) && (oam_strcmp((Char8*)(((xmlNode *)p_obj_sibbling->next)->name), "InterRATCell")) != 0) 
                {
                    p_obj_sibbling = (xmlNode *)p_obj_sibbling->next;
                }
		}/* SPR 17526 changes end */
        /*SPR 19977 FIX End*/ 

                /* OAM_LOG(OAM, OAM_DETAILED, "cell_idx  : %d curr_num_entries : %d  max_num_entries : %d\
                        p_obj_parent : %s   p_obj_sibbling : %s ",
                        cell_idx,curr_num_entries,max_num_entries,(xmlNode *)p_obj_parent->name,(xmlNode *)p_obj_sibbling->name);*/
                /* ADD OBJECT fix end */

/* SPR 16603 Fix End */                
                if (OAM_FAILURE == retVal) {
/* SPR 16603 Fix Start */
                    OAM_LOG(OAM, OAM_WARNING, "obj_data_accessor failed ");
/* SPR 16603 Fix End */
                }

                /* Maximum number of instance validation */
        /* SPR 18930  FIX START */
                if (*curr_num_entries == max_num_entries) {
        /* SPR 18930  FIX END */

                    OAM_LOG(OAM, OAM_WARNING, "Add operation not allowed, "
                                 "Max limit[%u] of instances already achieved", 
                                 max_num_entries);

                    *p_err = OAM_RESOURCE_EXCCEED;
                    /* SPR 15527 START */
                    return OAM_FAILURE;
                    /* SPR 15527 END */
/* SPR 16603 Fix code removed */
                }

                /* Generate the instance number */   
                g_object_list[loop].instance_id_gen(p_inst_num,
                                                    max_num_entries,
                                                    p_err);

                if (!*p_inst_num) {
                    OAM_LOG(OAM, OAM_WARNING, "Failed to create object, "
                            "or object list size reached max limit allowed");

                    *p_err = OAM_RESOURCE_EXCCEED;
                    retVal = OAM_FAILURE;
/* SPR 16603 Fix code removed */
			/*SPR 19977 FIX START*/ 
			oam_construct_n_send_addobject_resp_to_usr(OAM_RESULT_FAILURE,   
					OAM_ERR_INVALID_PARAM_NAME,
					(UInt8 *)"Invalid param name", strlen("Invalid param name")+OAM_ONE, 
					OAM_VERSION_ID, OAM_MODULE_ID,
					dest_id, trans_id, OAM_ZERO, OAM_ZERO,
					&err ,OAM_ZERO ); // Last argument instance number 
			        return retVal;
            /*SPR 19977 FIX End*/ 
                }

                OAM_LOG(OAM, OAM_DETAILED, "Assigned instance number: %d",
                                            *p_inst_num);
/* SPR 16603 Fix Start */                
                /* SPR 17777 fix */
                if(OAM_FAILURE ==  oam_add_object(obj_name, p_msg,
                                                  p_inst_num,
                                                  p_obj_parent,
                                                  p_obj_sibbling,
        /* SPR 18930  FIX START */
                                                  p_p_obj_struct_parent,
                                                  &curr_num_entries))
        /* SPR 18930  FIX END */
                {
                    OAM_LOG(OAM,OAM_WARNING," Error while adding objects ");
                    /* SPR 17453 changes start */
        /* SPR 18930  FIX START */
                    /* Code Removed */
        /* SPR 18930  FIX END */
                    /* SPR 17453 changes end */
                    retVal = OAM_FAILURE;
                }
                break;
            }
        }
/* SPR 16603 Fix End */                

        /*If object name found, add object response to TR069*/
        if (OAM_MAX_SUPPORTED_OBJECTS == loop) {
            OAM_LOG(OAM, OAM_WARNING, "Invalid object name[%s]", obj_name);
            *p_err = OAM_PARAMETER_NOT_FOUND;
            retVal = OAM_FAILURE;
        }
    }
    else {
        OAM_LOG(OAM,OAM_WARNING, "Object name too long to process");

        *p_err = OAM_PARAM_NAME_TOO_LONG;
    }

    /* SPR 22792 Fix Starts */
    /* Code Deleted */
    /* SPR 22792 Fix Ends */

    OAM_FUN_EXIT_TRACE();
    return retVal; 
}

/* SPR 16603 Fix Start */
/******************************************************************************
 * * Function Name  : oam_add_object
 * * Description    : This function handles add object api from tr69
 * * Inputs         : obj_name : Pointer to object name
 * *                  p_msg    : Pointer to message
 * * Output         : None
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
/* SPR 17777 fix */
oam_return_et 
oam_add_object( 
    char *obj_name, 
    void *p_msg, 
    UInt16 *p_inst_num,
    xmlNode *p_obj_parent,
    xmlNode *p_obj_sibbling,
        /* SPR 18930  FIX START */
    xmlNode **p_p_obj_struct_parent,
    UInt16 **curr_num_entries 
        /* SPR 18930  FIX END */
    )
{
    OAM_FUN_ENTRY_TRACE();
    UInt8 	loop, param_loop;   	
    Char8 	*add_obj_name = OAM_NULL, *param_name = OAM_NULL, buff[OAM_FIVE]={OAM_ZERO};
    /* SPR 15527 START */
    UInt16	trans_id = OAM_ZERO;
    UInt16 instance_id_value = OAM_ZERO;
    oam_bool_et	valid_object_name= OAM_FALSE;
    /* SPR 15527 END */
    /*SPR 19977 FIX Start*/ 
    xmlChar* curr_num_entries_str = OAM_NULL;
    /*SPR 19977 FIX End*/ 
    oam_error_code_et err=NO_ERROR;
    xmlNode 	*object_node = OAM_NULL;
    trans_id = get_word_from_header((UInt8*)(p_msg) + OAM_ZERO);
   	UInt16 dest_id = get_word_from_header((UInt8*)(p_msg) + OAM_TWO);

    OAM_LOG(OAM,OAM_DETAILED,"Add object name path from TR069 : %s",obj_name);

    for(loop = OAM_ZERO; loop < OAM_MAX_SUPPORTED_OBJECTS; loop++)
    {
        /* Search object name in object_map_element_t */
        /* If not found, send failure add object response */
        if(OAM_ZERO == ( oam_strncasecmp(obj_name, g_object_list[loop].object_name, oam_strlen(g_object_list[loop].object_name))))
        {
            valid_object_name = OAM_TRUE;

            /* Get the object name*/
            add_obj_name = g_object_list[loop].tag_name;
            OAM_LOG(OAM,OAM_INFO,"\n Object Name : %s number of params", add_obj_name);

            /* Create the child object in the dom tree*/
            /* Create a new node */
            /* SPR 15527 START */
            if(OAM_ZERO == (oam_strncasecmp(
                            "QoS",
                            add_obj_name, 
                            oam_strlen(add_obj_name))))
            {
                /* The generic add object handler code does not supports creation of parent nodes inside an
                 * object and hence new handling function is called for addition of QoS object in the xml dom */
                OAM_LOG(OAM,OAM_INFO,"\n Instance Number : %d", instance_id_value);

                oam_memset(buff,OAM_ZERO,sizeof(buff));
                /*Convert the interger To string*/
                snprintf(buff, sizeof(buff), "%d", *p_inst_num);	

                /* OAM BCOM Code Comment Changes Start */
                oam_add_qos_object(add_obj_name, loop, buff);
                /* OAM BCOM Code Comment Changes End */
            }
            else
            {
                /* SPR 15527 END */
                xmlNodePtr    new_node = xmlNewNode(OAM_NULL,(xmlChar*)add_obj_name);
                object_node = xmlAddNextSibling((xmlNode *)(p_obj_sibbling),new_node);

                if (object_node) {
                    *p_p_obj_struct_parent = new_node;
                }

                /* Loop for all parameters in object */
                for(param_loop = OAM_ZERO; param_loop < g_object_list[loop].no_of_parameter; param_loop++)
                {
                    if(g_object_list[loop].parameter_table_address[param_loop].parameter_mode & OAM_WRITE_ONLY)
                    {
                        /*Get the Parameter Name*/
                        param_name = g_object_list[loop].parameter_table_address[param_loop].tag_name;
                        OAM_LOG(OAM,OAM_INFO,"\n Object Parameter Name : %s",param_name);

                        /* create child parameter in dom tree */
                         xmlNewTextChild (object_node,OAM_NULL ,
                                (const xmlChar *)param_name, (xmlChar *)OAM_NULL); 
                    }
                }
                /*Generate the instance number*/
                //g_object_list[loop].instance_id_gen(p_inst_num,*(g_object_list[loop].max_no_of_instances),&err);
                OAM_LOG(OAM,OAM_INFO," Instance Number : %d", *p_inst_num);


                oam_memset(buff,OAM_ZERO,sizeof(buff));
                /*Convert the interger To string*/
                snprintf(buff, sizeof(buff), "%d", *p_inst_num);	


                /* Also add vendor specifc parameter X_VENDOR_INSTANCE_ID */
                xmlNewChild((xmlNode *)(object_node), OAM_NULL, (xmlChar *)"X_VENDOR_INSTANCE_ID", (xmlChar *)&buff);

                /* SPR 15527 START */
            }
                /* SPR 15527 END */
            /* Increment Instance No */

            /*finding pointer to node*/
            xmlNode *p_xml_node = oam_xml_find_tag(
                    ((xmlNode *)(p_obj_parent))->children,
                    ((xmlChar *)(g_object_list[loop].current_entry)));
	    /*SPR 19977 FIX Start*/ 
        OAM_LOG(OAM,OAM_DETAILED," curr_num_entries = %d",**curr_num_entries);  
	    curr_num_entries_str = xmlNodeGetContent(p_xml_node);
	    **curr_num_entries = oam_atoi((Char8*)curr_num_entries_str);
        /*Convert the interger To string*/
        **curr_num_entries += OAM_ONE; // ADD Incrementing the current num entries 
	    OAM_LOG(OAM,OAM_DETAILED,"Add object name path from TR069 : %d",**curr_num_entries);
	    snprintf(buff, sizeof(buff), "%d", **curr_num_entries);	
	    /*SPR 19977 FIX End*/ 
            xmlNodeSetContent((xmlNode *)p_xml_node, (xmlChar *)&buff);


            /* Send add object response from here only */
            oam_construct_n_send_addobject_resp_to_usr(OAM_RESULT_SUCCESS,
                    OAM_ZERO, (UInt8 *)OAM_NULL, OAM_ZERO,
                    OAM_VERSION_ID, OAM_MODULE_ID,
                    dest_id, trans_id, OAM_ZERO, OAM_ZERO,
                    &err , *p_inst_num); // Last argument instance number	
	    /*SPR 21619 Start*/
	     xmlFree(curr_num_entries_str);	
	    /*SPR 21619 End*/	
            break;
        }
    }        
    /*If object name found, add object response to TR069*/
    if (valid_object_name == OAM_FALSE)
    {
        OAM_LOG(OAM, OAM_WARNING, "oam_add_object: Invalid param name.");
        /*coverity 109014 fix start */
        oam_construct_n_send_addobject_resp_to_usr(OAM_RESULT_FAILURE,   
                OAM_ERR_INVALID_PARAM_NAME,
                (UInt8 *)"Invalid param name", oam_strlen("Invalid param name")+OAM_ONE, 
                OAM_VERSION_ID, OAM_MODULE_ID,
                dest_id, trans_id, OAM_ZERO, OAM_ZERO,
                &err ,OAM_ZERO ); // Last argument instance number 
        /*coverity 109014 fix end */
    }		
    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}
/* SPR 16603 Fix End */


/****************************************************************************
 * * Function Name  : oam_add_parent_object
 * * Description    : This function handles add object api from user mgmt i/f
 * * Inputs         : full_param_name: full TR-196 compliable string of the
 * *                  parameter for which the parent object is being added
 * *                  inst_num: instance number to be assigned to the parent
 * * Output         : None
 * * Returns        : pointer to new object added or NULL in case of error
 ***************************************************************************/
xmlNode*
oam_add_parent_object(Char8 *full_param_name, UInt16 inst_num)
{
    OAM_FUN_ENTRY_TRACE();

    /* SPR 16603 Fix Start */
    UInt8 cell_idx = OAM_ZERO;
    /* SPR 16603 Fix End */
    UInt8 	loop = OAM_ZERO;
    Char8 	*add_obj_name = OAM_NULL;
    /* SPR 14153 FIX START */
    oam_return_et retVal = OAM_SUCCESS;
  
    xmlNode 	*object_node = OAM_NULL;
    xmlNode 	*p_obj_first_sibling = OAM_NULL;
    /* SPR 13850 FIX START */
    
    Char8 obj_name[MAX_PARENT_NAME_LENGTH] = {OAM_ZERO};
    /*CID:66602 delete*/
    /* SPR 14153 FIX END */
    /* SPR 13850 FIX END */
    if (!full_param_name) {
        OAM_LOG(OAM, OAM_WARNING, "Null pointer error");
        OAM_FUN_EXIT_TRACE();
        return object_node;
    }

    oam_strncpy(obj_name, full_param_name, MAX_PARENT_NAME_LENGTH);

    /*CID:66602 delete*/
    OAM_LOG(OAM, OAM_DETAILED, "Add object name path : %s", obj_name);

    for (loop = OAM_ZERO; loop < OAM_MAX_SUPPORTED_OBJECTS; ++loop) {
        /* Search object name in object_map_element_t */
        /* If not found, send failure add object response */
        if (!oam_strncmp(obj_name, g_object_list[loop].object_name,oam_strlen(g_object_list[loop].object_name))) {
            /* Get the object name*/
            add_obj_name = g_object_list[loop].tag_name;

            /* Create the child object in the dom tree*/
            /* Create a new node */
            /* SPR 14153 FIX START */
            object_node = oam_make_default_multi_instance_obj(add_obj_name,
                                                              inst_num);

            if (!object_node) {
                break;
           
            }
            else {
                retVal =
                    g_object_list[loop].obj_data_accessor(OAM_NULL, 
                                                          OAM_NULL,
                                                          OAM_NULL,
/* SPR 16603 Fix Start */                
                                                        &p_obj_first_sibling,
                                                        OAM_NULL,
                                                        &cell_idx,
/* SPR 16603 Fix End */                
                                                        OAM_NULL);
                if (OAM_SUCCESS == retVal) {
                   
                    object_node->prev = p_obj_first_sibling;

                    
                }
                else {
                    xmlFreeNode(object_node);
                    object_node = OAM_NULL;
                    /* SPR 14153 FIX END */
                    OAM_LOG(OAM, OAM_WARNING,
                                 "Failed to access data for object %s",
                                 add_obj_name);
                }
            }

            break;
        }
    }

    /*If object name found, add object response to TR069*/
    if (OAM_MAX_SUPPORTED_OBJECTS == loop) {
        OAM_LOG(OAM, OAM_WARNING, "Invalid object name[%s]", obj_name);
    }

    OAM_FUN_EXIT_TRACE();
    return object_node;
}

/****************************************************************************
 * * Function Name  : oam_process_add_delete_plmn_list
 * * Description    : This function is used to add or delete a PLMNList node
 * *                  in xml tree
 * * Inputs         : add_del_flag : add/delete flag
 * *                  p_msg        : Pointer to message
 * *                  instance_id  : insatnce id of the object name
 * * Output         : None
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 ***************************************************************************/

oam_return_et 
oam_process_add_delete_plmn_list(UInt8 add_del_flag,
                                 void *p_msg, 
                                 UInt16 instance_id,
                                 void* p_add_data,
                                 oam_error_code_et *p_err)
{
    OAM_FUN_ENTRY_TRACE();
    oam_return_et      ret_val = OAM_SUCCESS;
    UInt16 transaction_id = oam_get_new_trans_id();
    UInt8   *p_api_buf = p_msg;
    UInt16   msg_len = OAM_ZERO;
    UInt16 arr_idx = instance_id - 1;
    UInt8 cid = oam_prov_req.cell_cntxt.curr_cell_cid;
    oam_rrm_cell_plmnlist_data_t *p_cell_plmnlist_data = OAM_NULL;
    /* SPR 11878 FIX START */
    oam_rrm_cell_plmnlist_data_t *p_cell_local_plmnlist_data = OAM_NULL;
    /* SPR 11878 FIX END */
    oam_rrm_cell_reconfig_req_t   *p_cell_reconfig_req = OAM_NULL;
    SInt8 cell_config_idx = OAM_ERROR_RETURN_VAL;
    oam_tcb_cntxt_data_t *p_tcb_data = p_add_data;

    cell_config_idx = oam_get_cell_plmnlist_data_idx(cid);
    if(OAM_ERROR_RETURN_VAL == cell_config_idx) {
        OAM_LOG(OAM, OAM_WARNING, "PLMNList data not found");
        *p_err = OAM_INVALID_CELL_CONTEXT;
        ret_val = OAM_FAILURE;

        OAM_FUN_EXIT_TRACE();
        return ret_val;
    }

    p_cell_plmnlist_data = &RRM_PROV_REQ.plmnlist_data[cell_config_idx];
    /* SPR 11878 FIX START */
    p_cell_local_plmnlist_data =
                                 &LOCAL_RRM_PROV_REQ.plmnlist_data[cell_config_idx];
    /* SPR 11878 FIX END */


    msg_len = get_word_from_header(p_api_buf + OAM_INTF_HDR_MSG_SIZE_OFFSET);


    if (OAM_ADD_OPERATION == add_del_flag) {
        OAM_LOG(OAM, OAM_INFO, "Adding PLMNList object with instance-id: %d",
                     instance_id);

        /* SPR 11878 FIX START */
        p_cell_local_plmnlist_data->is_valid_bitmap = (1 << arr_idx);
        /* SPR 11878 FIX END */
    }
    else if (OAM_DELETE_OPERATION == add_del_flag) {
        OAM_LOG(OAM, OAM_INFO,
                     "Deleting PLMNList object with instance-id: %d",
                     instance_id);

        if (p_cell_plmnlist_data->plmn_entries[arr_idx].is_primary) {
            ret_val = OAM_FAILURE;
            OAM_LOG(OAM, OAM_WARNING, "Attempt to delete the primary PLMNID!");
            *p_err = OAM_ATTEMPT_TO_DELETE_PRIM_PLMNID; 

            OAM_FUN_EXIT_TRACE();
            return ret_val;
        }

    
        ret_val = oam_mem_alloc(sizeof(oam_rrm_cell_reconfig_req_t),
                                (void**)&p_cell_reconfig_req, p_err);

        if(OAM_FAILURE == ret_val) {
            OAM_LOG(OAM, OAM_ERROR,
                    "Memory allocation to p_cell_reconfig_req failed"
                    " with Error Code:%d",
                        *p_err);

            OAM_FUN_EXIT_TRACE();
            return ret_val;
        }

        cell_config_idx = oam_get_rrm_cell_config_idx(cid);
        if(OAM_ERROR_RETURN_VAL == cell_config_idx) {
            OAM_LOG(OAM, OAM_WARNING, "Invalid cell context[cid=%d]", cid);

            oam_mem_free(p_cell_reconfig_req, p_err);

            OAM_FUN_EXIT_TRACE();
            return OAM_FAILURE;
        }

        p_cell_plmnlist_data->plmnlist_query_idx = arr_idx;

        oam_memset(p_cell_reconfig_req, 0,
                   sizeof(oam_rrm_cell_reconfig_req_t));

        oam_prov_req.system_status.event_ongoing |=
                                    OAM_CELL_PLMNLIST_CONFIG_ONGOING;

        oam_prov_req.system_status.event_ongoing |=
                                OAM_DELETE_OBJ_CELL_RECONFIG_ONGOING;

        /* SPR_19279_START */
        p_cell_reconfig_req->bitmask |= RRM_OAM_EPC_INFO_PRESENT;
        p_cell_reconfig_req->epc_info.epc_params.bitmask |=
            RRM_OAM_GENERAL_EPC_PARAMS_PRESENT;
        /* SPR 19279 Fix Start */
        /* SPR 19977 Fix Start */
	    oam_prov_req.oam_rrm_req.plmnlist_data[cell_config_idx].is_valid_bitmap &=   
		    ~(OAM_ONE << (arr_idx));           

	    oam_prov_req.oam_rrm_req.plmnlist_data[cell_config_idx].curr_plmn_entries--;
        /* SPR 19279 Fix End */
        oam_memset(&p_cell_local_plmnlist_data->plmn_entries[arr_idx], 0,sizeof(oam_rrm_plmn_data_t));
        /* SPR 19977 Fix End */
        /* SPR_19279_END */

        oam_copy_config_to_reconfig(&RRM_CELL_CONFIG(cell_config_idx),
                                    p_cell_reconfig_req);

        ret_val = oam_rrm_send_rrm_oam_cell_reconfig_req(p_cell_reconfig_req,
                                                         OAM_MODULE_ID,
                                                         RRM_MODULE_ID,
                                                         transaction_id,
                                                         cid);


        if (OAM_FAILURE == ret_val) {
            OAM_LOG(OAM, OAM_ERROR, "Failed to send RRM_CELL_RECONFIG_REQ");

            /* Reverting changes in case of failure */
            oam_prov_req.system_status.event_ongoing &=
                                    ~OAM_CELL_PLMNLIST_CONFIG_ONGOING;
            oam_prov_req.system_status.event_ongoing &=
                                ~OAM_DELETE_OBJ_CELL_RECONFIG_ONGOING;

            *p_err = OAM_SEND_FAIL;
        }
        else {
#ifndef OAM_UT_FRAMEWORK
            oam_maintain_tcb_ext(transaction_id,
                                 OAM_TRANS_RRM, msg_len,
                                 p_api_buf,
                                 p_tcb_data->p_orig_xml_parent,
                                 p_tcb_data->p_usr_msg_xml_parent,
                                 p_tcb_data->p_parent_xml_struct_addr);
#endif
        }

        oam_mem_free(p_cell_reconfig_req, p_err);
    }

    OAM_FUN_EXIT_TRACE();
    return ret_val;
}
/* SPR 15527 START */                                                  
/******************************************************************************
 * * Function Name  : oam_process_add_delete_qos_list
 * * Description    : This function is used to indicate the existance of PLMNList 
 * * Inputs         : add_del_flag : add/delete flag
 * *                  p_msg        : Pointer to message
 * *                  instance_id  : insatnce id of the object name
 * * Output         : None
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et 
oam_process_add_delete_qos_list(UInt8 add_del_flag,
                                               void *p_msg,
                                               UInt16 instance_id,
                                               void* p_add_data,
                                               oam_error_code_et *p_err)

{
    OAM_FUN_ENTRY_TRACE();
    oam_return_et      ret_val = OAM_SUCCESS;
    oam_error_code_et  err_code = NO_ERROR;
    oam_tcb_cntxt_data_t *p_tcb_data = p_add_data;
    UInt16 transaction_id = oam_get_new_trans_id();
    UInt8   *p_api_buf = p_msg;
    UInt16   msg_len = OAM_ZERO;
    /*Coverity fix 5 Mar 2015 start : 78996 */
    if (OAM_ZERO==instance_id) {
        OAM_LOG(OAM, OAM_WARNING, "Invalid instance id: %d", instance_id);

        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
    /*Coverity fix 5 Mar 2015 End : 78996 */
    /* SPR 17777 fix */
    OAM_LOG(OAM,OAM_DETAILED," Instance id   %d , unused parameter %p and %p",instance_id, p_add_data, p_err);

    SInt8 cell_config_idx = oam_get_rrm_cell_config_idx(oam_prov_req.cell_cntxt.curr_cell_cid);
    if(OAM_ERROR_RETURN_VAL == cell_config_idx) {
        OAM_LOG(OAM, OAM_ERROR, "Invalid cell context[cid=%d]", oam_prov_req.cell_cntxt.curr_cell_cid);
        *p_err = OAM_INVALID_CELL_CONTEXT;

        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    msg_len = get_word_from_header((UInt8*)(p_api_buf) + OAM_EIGHT);

    if(OAM_ADD_OPERATION == add_del_flag)
    {
        oam_prov_req.oam_rrm_req.qos_data[cell_config_idx].qos_obj_list[instance_id - OAM_ONE].is_valid_entry = OAM_TRUE;
        local_prov_req.oam_rrm_req.qos_data[cell_config_idx].qos_obj_list[instance_id - OAM_ONE].is_valid_entry = OAM_TRUE;
        local_prov_req.oam_rrm_req.qos_data[cell_config_idx].num_qos_entries = oam_prov_req.oam_rrm_req.qos_data[cell_config_idx].num_qos_entries;
    }
    else if(OAM_DELETE_OPERATION == add_del_flag)
    {
    
        oam_rrm_cell_reconfig_req_t   *p_cell_reconfig_req; 
        RRM_PROV_REQ.qos_data[cell_config_idx].qos_obj_list[instance_id - 1].is_valid_entry = OAM_FALSE;
        LOCAL_RRM_PROV_REQ.qos_data[cell_config_idx].qos_obj_list[instance_id - 1].is_valid_entry = OAM_FALSE;
        LOCAL_RRM_PROV_REQ.qos_data[cell_config_idx].qos_obj_list[instance_id - 1].qos_enable = OAM_FALSE;
        RRM_PROV_REQ.qos_data[cell_config_idx].qos_obj_list[instance_id - 1].qos_enable = OAM_FALSE;
    
        ret_val = oam_mem_alloc(sizeof(oam_rrm_cell_reconfig_req_t),
                                (void**)&p_cell_reconfig_req, &err_code);

        if(OAM_FAILURE == ret_val) {
            OAM_LOG(OAM, OAM_ERROR,
                    "Memory allocation to p_cell_reconfig_req failed"
                    " with Error Code:%d",
                    err_code);
            OAM_FUN_EXIT_TRACE();
            return ret_val;
        }
        /* Coverity CID 85439 Fix Start */
        oam_memset(p_cell_reconfig_req,OAM_ZERO,sizeof(oam_rrm_cell_reconfig_req_t));
        /* Coverity CID 85439 Fix End */
        p_cell_reconfig_req->bitmask |= RRM_OAM_EPC_INFO_PRESENT;

        --oam_prov_req.oam_rrm_req.qos_data[cell_config_idx].num_qos_entries;
        oam_prov_req.system_status.event_ongoing |=
                                OAM_DELETE_OBJ_CELL_RECONFIG_ONGOING;  

        oam_copy_config_to_reconfig((oam_rrm_cell_config_req_t *)(
                  &(oam_prov_req.oam_rrm_req.oam_rrm_cell_config_req)), p_cell_reconfig_req);

        //oam_maintain_tcb(transaction_id,OAM_TRANS_RRM,msg_len,p_api_buf);
        ret_val = oam_rrm_send_rrm_oam_cell_reconfig_req(p_cell_reconfig_req,
                                                         OAM_MODULE_ID,
                                                         RRM_MODULE_ID,
                                                         transaction_id,
                                                         oam_prov_req.cell_cntxt.curr_cell_cid);

#ifndef OAM_UT_FRAMEWORK
            oam_maintain_tcb_ext(transaction_id,
                                 OAM_TRANS_RRM, 
                                 msg_len,
                                 p_api_buf,
                                 p_tcb_data->p_orig_xml_parent,
                                 p_tcb_data->p_usr_msg_xml_parent,
                                 p_tcb_data->p_parent_xml_struct_addr);
#endif


        if (OAM_FAILURE == ret_val)
        {
                OAM_LOG(OAM,OAM_ERROR,
                "Error in function oam_rrm_send_rrm_oam_cell_reconfig_req:"
                " error code :%d",err_code);
                /* Coverity CID 85445 Fix Start */
                oam_mem_free((void *)p_cell_reconfig_req, &err_code);
                /* Coverity CID 85445 Fix End */
        }
        else
        {
            oam_prov_req.system_status.event_ongoing |= OAM_DELETE_OBJ_CELL_RECONFIG_ONGOING;
        }
    }

    OAM_FUN_EXIT_TRACE();
    return ret_val;
}
/* SPR 15527 END */                                                  


#ifdef OAM_SON_ENABLE
/****************************************************************************
 * * Function Name  : oam_process_add_delete_inter_freq_carrier_list
 * * Description    : This function is used to add or delete a Carrier node
 * *                  in xml tree
 * * Inputs         : add_del_flag : add/delete flag
 * *                  p_msg        : Pointer to message
 * *                  instance_id  : insatnce id of the object name
 * *                  pointer to interal error code
 * * Output         : None
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 ***************************************************************************/
oam_return_et 
oam_process_add_delete_inter_freq_carrier_list(UInt8 add_del_flag,
                                               void *p_msg,
                                               UInt16 instance_id,
                                               void* p_add_data,
                                               oam_error_code_et *p_err)
{
    OAM_FUN_ENTRY_TRACE();
    oam_return_et      ret_val = OAM_SUCCESS;
    UInt16 arr_idx = instance_id - 1;
    UInt8 cid = oam_prov_req.cell_cntxt.curr_cell_cid;
    oam_son_inter_freq_carrier_t *p_interfreq_carr_params = OAM_NULL;
    /* SPR 14651 START */
    oam_son_anr_meas_config_req_t *p_anr_meas_config_req = OAM_NULL;
    /* SPR 14651 END */
    oam_son_anr_meas_config_obj_remove_req_t *p_req = OAM_NULL;
    oam_tcb_cntxt_data_t *p_tcb_data = p_add_data;
    UInt16 trans_id = OAM_ZERO;
    UInt16 dest_id = OAM_ZERO;
    UInt16 msg_len = OAM_ZERO;

    SInt8 cell_config_idx = OAM_ERROR_RETURN_VAL;
    SInt8 cell_config_idx1 = OAM_ERROR_RETURN_VAL;


    if (!p_err) {
        OAM_LOG(OAM, OAM_WARNING, "NULL pointer error");

        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    cell_config_idx = oam_get_son_inter_freq_carrier_idx(cid);
    if(OAM_ERROR_RETURN_VAL == cell_config_idx) {
        OAM_LOG(OAM, OAM_WARNING, "Invalid cell context[cid=%d]", cid);
        *p_err = OAM_INVALID_CELL_CONTEXT;

        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    cell_config_idx1 = oam_get_son_serving_cell_idx(cid);
    if(OAM_ERROR_RETURN_VAL == cell_config_idx1) {
        OAM_LOG(OAM, OAM_WARNING, "Invalid cell context[cid=%d]", cid);
        *p_err = OAM_INVALID_CELL_CONTEXT;

        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    p_interfreq_carr_params = &SON_INTER_FREQ_CARR_PARAMS(cell_config_idx);



    if (OAM_ADD_OPERATION == add_del_flag) {
        OAM_LOG(OAM, OAM_INFO, "Adding Carrier object with instance-id: %d",
                     instance_id);
    }
    else if (OAM_DELETE_OPERATION == add_del_flag) {
        OAM_LOG(OAM, OAM_INFO,
                     "Deleting Carrier object with instance-id: %d",
                     instance_id);

        if(!(p_interfreq_carr_params->eutran_meas_entry_bitmap &
                                    (1 << arr_idx))) {
            trans_id = get_word_from_header((UInt8*)p_msg +
                                    OAM_INTF_HDR_TRANS_ID_OFFSET);
            dest_id = get_word_from_header((UInt8*)p_msg +
                                    OAM_INTF_HDR_SRC_ID_OFFSET);

            OAM_LOG(OAM, OAM_DETAILED,
                    "Instance not added at SON, sending success response");
            /* Send delete object response from here only */
            oam_construct_n_send_deleteobject_resp_to_usr(OAM_RESULT_SUCCESS,
                                                          OAM_ZERO, OAM_NULL,
                                                          OAM_ZERO,
                                                          OAM_VERSION_ID,
                                                          OAM_MODULE_ID,
                                                          dest_id, trans_id,
                                                          OAM_ZERO, 
                                                          OAM_ZERO,
                                                          p_err);

            oam_prov_req.system_status.event_ongoing &=
                                ~OAM_DELETE_OBJ_CELL_RECONFIG_ONGOING;

            OAM_FUN_EXIT_TRACE();
            return OAM_SUCCESS;
        }

        p_interfreq_carr_params->query_index = arr_idx;
        p_interfreq_carr_params->config_ongoing |=
                            OAM_ANR_ACTION_DELETE_INTER_FREQ_CARRIER_ONGOING;

        /* SPR 14651 START */
        p_anr_meas_config_req = &SON_ANR_MEAS_CONFIG_REQ(cell_config_idx);

        oam_prov_req.system_status.event_ongoing |=
                                OAM_DELETE_OBJ_CELL_RECONFIG_ONGOING;  
        --p_anr_meas_config_req->eutran_config_list_size;

        p_interfreq_carr_params->eutran_meas_entry_bitmap &=
                            ~(1 << arr_idx);
        /* SPR 14651 END */

        ret_val = oam_mem_alloc(
                          sizeof(oam_son_anr_meas_config_obj_remove_req_t),
                          (void**)&p_req, p_err);

        if(OAM_FAILURE == ret_val) {
            OAM_LOG(OAM, OAM_ERROR,
                    "Memory allocation to p_req failed"
                    " with Error Code:%d",
                    *p_err);

            OAM_FUN_EXIT_TRACE();
            return ret_val;
        }

	oam_memset(p_req, OAM_ZERO,
                   sizeof(oam_son_anr_meas_config_obj_remove_req_t));


        oam_memcpy(&p_req->src_cgi,
                 &(SON_CELL_INFO_IND_SERVING_CELL(cell_config_idx1).src_cgi),
                 sizeof(son_intra_rat_global_cell_id_t));

        p_req->earfcn_remove_list_size = 1;

        p_req->earfcn_remove_list[0] =
            p_interfreq_carr_params->oam_son_anr_meas_config_req.
                eutran_config_list[arr_idx].earfcn;

        trans_id = oam_get_new_trans_id();
        ret_val =
            oam_son_send_son_oam_anr_meas_config_obj_remove_req(p_req,
                                                               OAM_MODULE_ID,
                                                               SON_MODULE_ID,
                                                               trans_id, cid);

        if (OAM_FAILURE == ret_val) {
            OAM_LOG(OAM, OAM_ERROR, "Failed to send request to lower layer");
            *p_err = OAM_SEND_FAIL;
        }
        else {
            msg_len = get_word_from_header((UInt8*)p_msg +
                                            OAM_INTF_HDR_MSG_SIZE_OFFSET);

#ifndef OAM_UT_FRAMEWORK
            oam_maintain_tcb_ext(trans_id,
                                 OAM_TRANS_SON, msg_len,
                                 p_msg,
                                 p_tcb_data->p_orig_xml_parent,
                                 p_tcb_data->p_usr_msg_xml_parent,
                                 p_tcb_data->p_parent_xml_struct_addr);
#endif
        }

        oam_mem_free(p_req, p_err);
    }

    OAM_FUN_EXIT_TRACE();
    return ret_val;
}


/****************************************************************************
 * * Function Name  : oam_process_add_delete_utran_fdd_freq_list
 * * Description    : This function is used to add or delete a Carrier node
 * *                  in xml tree
 * * Inputs         : add_del_flag : add/delete flag
 * *                  p_msg        : Pointer to message
 * *                  instance_id  : insatnce id of the object name
 * *                  pointer to interal error code
 * * Output         : None
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 ***************************************************************************/
oam_return_et 
oam_process_add_delete_utran_fdd_freq_list(UInt8 add_del_flag,
                                           void *p_msg,
                                           UInt16 instance_id,
                                           void* p_add_data,
                                           oam_error_code_et *p_err)
{
    OAM_FUN_ENTRY_TRACE();
    oam_return_et      ret_val = OAM_SUCCESS;
    UInt16 arr_idx = instance_id - 1;
    UInt8 cid = oam_prov_req.cell_cntxt.curr_cell_cid;
    oam_son_inter_freq_carrier_t *p_interfreq_carr_params = OAM_NULL;
    /* SPR 14651 START */
    oam_son_anr_meas_config_req_t *p_anr_meas_config_req = OAM_NULL;
    /* SPR 14651 END */
    oam_son_anr_meas_config_obj_remove_req_t *p_req = OAM_NULL;
    oam_tcb_cntxt_data_t *p_tcb_data = p_add_data;
    UInt16 trans_id = OAM_ZERO;
    UInt16 dest_id = OAM_ZERO;
    UInt16 msg_len = OAM_ZERO;

    SInt8 cell_config_idx = OAM_ERROR_RETURN_VAL;
    SInt8 cell_config_idx1 = OAM_ERROR_RETURN_VAL;


    if (!p_err) {
        OAM_LOG(OAM, OAM_WARNING, "NULL pointer error");

        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    cell_config_idx = oam_get_son_inter_freq_carrier_idx(cid);
    if(OAM_ERROR_RETURN_VAL == cell_config_idx) {
        OAM_LOG(OAM, OAM_WARNING, "Invalid cell context[cid=%d]", cid);
        *p_err = OAM_INVALID_CELL_CONTEXT;

        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    cell_config_idx1 = oam_get_son_serving_cell_idx(cid);
    if(OAM_ERROR_RETURN_VAL == cell_config_idx1) {
        OAM_LOG(OAM, OAM_WARNING, "Invalid cell context[cid=%d]", cid);
        *p_err = OAM_INVALID_CELL_CONTEXT;

        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    p_interfreq_carr_params = &SON_INTER_FREQ_CARR_PARAMS(cell_config_idx);



    if (OAM_ADD_OPERATION == add_del_flag) {
        OAM_LOG(OAM, OAM_INFO,
                     "Adding UTRANFDDFreq object with instance-id: %d",
                     instance_id);
    }
    else if (OAM_DELETE_OPERATION == add_del_flag) {
        OAM_LOG(OAM, OAM_INFO,
                     "Deleting UTRANFDDFreq object with instance-id: %d",
                     instance_id);

        if(!(p_interfreq_carr_params->utran_fdd_meas_entry_bitmap &
                                    (1 << arr_idx))) {
            trans_id = get_word_from_header((UInt8*)p_msg +
                                    OAM_INTF_HDR_TRANS_ID_OFFSET);
            dest_id = get_word_from_header((UInt8*)p_msg +
                                    OAM_INTF_HDR_SRC_ID_OFFSET);

            OAM_LOG(OAM, OAM_DETAILED,
                    "Instance not added at SON, sending success response");
            /* Send delete object response from here only */
            oam_construct_n_send_deleteobject_resp_to_usr(OAM_RESULT_SUCCESS,
                                                          OAM_ZERO, OAM_NULL,
                                                          OAM_ZERO,
                                                          OAM_VERSION_ID,
                                                          OAM_MODULE_ID,
                                                          dest_id, trans_id,
                                                          OAM_ZERO, 
                                                          OAM_ZERO,
                                                          p_err);

            oam_prov_req.system_status.event_ongoing &=
                                ~OAM_DELETE_OBJ_CELL_RECONFIG_ONGOING;

            OAM_FUN_EXIT_TRACE();
            return OAM_SUCCESS;
        }

        p_interfreq_carr_params->query_index = arr_idx;
        p_interfreq_carr_params->config_ongoing |=
                            OAM_ANR_ACTION_DELETE_UTRAN_FDD_FREQ_ONGOING;


        ret_val = oam_mem_alloc(
                          sizeof(oam_son_anr_meas_config_obj_remove_req_t),
                          (void**)&p_req, p_err);

        if(OAM_FAILURE == ret_val) {
            OAM_LOG(OAM, OAM_ERROR,
                    "Memory allocation to p_req failed"
                    " with Error Code:%d",
                    *p_err);

            OAM_FUN_EXIT_TRACE();
            return ret_val;
        }

	oam_memset(p_req, OAM_ZERO,
                   sizeof(oam_son_anr_meas_config_obj_remove_req_t));


        oam_memcpy(&p_req->src_cgi,
                 &(SON_CELL_INFO_IND_SERVING_CELL(cell_config_idx1).src_cgi),
                 sizeof(son_intra_rat_global_cell_id_t));

        /* SPR 14651 START */
        p_anr_meas_config_req = &SON_ANR_MEAS_CONFIG_REQ(cell_config_idx);

        --p_anr_meas_config_req->other_rat_config_list.
                  utran_config_list.utran_fdd_list_size;

        p_interfreq_carr_params->
             utran_fdd_meas_entry_bitmap &= ~(1 << arr_idx);

        oam_prov_req.system_status.event_ongoing |=
                                OAM_DELETE_OBJ_CELL_RECONFIG_ONGOING; 
        /* SPR 14651 END */

        p_req->uarfcn_fdd_remove_list_size = 1;

        p_req->uarfcn_fdd_remove_list[0] =
            p_interfreq_carr_params->oam_son_anr_meas_config_req.
                other_rat_config_list.utran_config_list.
                utran_fdd_list[arr_idx].uarfcn;

        trans_id = oam_get_new_trans_id();
        ret_val =
            oam_son_send_son_oam_anr_meas_config_obj_remove_req(p_req,
                                                               OAM_MODULE_ID,
                                                               SON_MODULE_ID,
                                                               trans_id, cid);

        if (OAM_FAILURE == ret_val) {
            OAM_LOG(OAM, OAM_ERROR, "Failed to send request to lower layer");
            *p_err = OAM_SEND_FAIL;
        }
        else {
            msg_len = get_word_from_header((UInt8*)p_msg +
                                            OAM_INTF_HDR_MSG_SIZE_OFFSET);

#ifndef OAM_UT_FRAMEWORK
            oam_maintain_tcb_ext(trans_id,
                                 OAM_TRANS_SON, msg_len,
                                 p_msg,
                                 p_tcb_data->p_orig_xml_parent,
                                 p_tcb_data->p_usr_msg_xml_parent,
                                 p_tcb_data->p_parent_xml_struct_addr);
#endif
        }

        oam_mem_free(p_req, p_err);
    }

    OAM_FUN_EXIT_TRACE();
    return ret_val;
}

/****************************************************************************
 *  Function Name  : oam_geran_freq_group_data_accessor
 *  Description    : This function returns pointers to current number of
 *                  GERANFreqGroup objects, max number of GERANFreqGroup
 *                  objects, pointer to parent of
 *                  GERANFreqGroup object node, and pointer to first sibling
 *                  of GERANFreqGroup object node
 *  Inputs         : NONE

 *  Output         : 1. p_curr_entries, (Optional, use OAM_NULL if not
 *                                      required)
 *                   2. p_max_entries, (Optional, use OAM_NULL if not
 *                                      required)
 *                   3. obj_parent (Optional, use OAM_NULL if not required),
 *                   4. obj_first_sibling (Optional, use OAM_NULL if not
 *                                         required)
 *  Returns        : return OAM_SUCCESS/OAM_FAILURE
 ***************************************************************************/
oam_return_et
        /* SPR 18930  FIX START */
oam_geran_freq_group_data_accessor(UInt16 **p_curr_entries,
        /* SPR 18930  FIX END */
                                   UInt16 *p_max_entries,
                                   xmlNode **obj_parent,
                                   xmlNode **obj_first_sibling,
/* SPR 16603 Fix Start */
                                   xmlNode *** obj_struct_parent,
                                   UInt8 *cell_idx, 
/* SPR 16603 Fix End */
                                   oam_error_code_et *p_err)
{
    UInt8 cid = oam_prov_req.cell_cntxt.curr_cell_cid;
    UInt8 idx = OAM_ZERO;
    oam_return_et retVal = OAM_SUCCESS;
    XML_struct_cell_config *p_cellconfig = OAM_NULL;

    OAM_FUN_ENTRY_TRACE();

    if (p_curr_entries || p_max_entries) {
        for ( ; idx < oam_prov_req.num_cells_configured; ++idx) {
            if (cid == SON_INTER_FREQ_CARR_PARAMS(idx).cid) {
/* SPR 16603 Fix Start */
                *cell_idx = idx;
/* SPR 16603 Fix End */
        /* SPR 18930  FIX START */
                if (p_curr_entries) {
                    *p_curr_entries = (UInt16 *)&SON_ANR_MEAS_CONFIG_REQ(idx).
                                       other_rat_config_list.
                                       geran_config_list.
                                       geran_freq_info_list_size;
                }

        /* SPR 18930  FIX END */
                if (p_max_entries) {
                    *p_max_entries = SON_INTER_FREQ_CARR_PARAMS(idx).
                                          max_geran_freq_entries;
                }
                break;
            }
        }
    }

    if (idx == oam_prov_req.num_cells_configured) {
        OAM_LOG(OAM, OAM_WARNING, "GERANFreqGroup data not found");
        if (p_err) {
            *p_err = OAM_INVALID_CELL_CONTEXT;
        }

        retVal = OAM_FAILURE;
    }
    else if (obj_parent || obj_first_sibling) {
        p_cellconfig = oam_get_xml_cellconfig_node(cid);

        if (p_cellconfig) {
            if (obj_parent) {
                *obj_parent = p_cellconfig->GERAN;
            }
            if (obj_first_sibling) {
                *obj_first_sibling =
                        oam_get_xml_leaf_child_node(p_cellconfig->GERAN,
                                            "GERANFreqGroupNumberOfEntries");
            }
/* SPR 16603 Fix Start */
            if (obj_struct_parent) {
                *obj_struct_parent = &p_cellconfig->GERANFreqGroup[0];
            }
/* SPR 16603 Fix End */
        }
        else {
            if (p_err) {
                *p_err = OAM_INVALID_CELL_CONTEXT;
            }

            retVal = OAM_FAILURE;
        }
    }

    OAM_FUN_EXIT_TRACE();
    return retVal;
}

/****************************************************************************
 * * Function Name  : oam_process_add_delete_geran_freq_group_list
 * * Description    : This function is used to add or delete a GERANFreqGroup
 * *                  node in xml tree
 * * Inputs         : add_del_flag : add/delete flag
 * *                  p_msg        : Pointer to message
 * *                  instance_id  : insatnce id of the object name
 * *                  pointer to interal error code
 * * Output         : None
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 ***************************************************************************/
oam_return_et 
oam_process_add_delete_geran_freq_group_list(UInt8 add_del_flag,
                                             void *p_msg,
                                             UInt16 instance_id,
                                             void* p_add_data,
                                             oam_error_code_et *p_err)
{
    OAM_FUN_ENTRY_TRACE();
    oam_return_et      ret_val = OAM_SUCCESS;
    UInt16 arr_idx = instance_id - 1;
    UInt8 cid = oam_prov_req.cell_cntxt.curr_cell_cid;
    oam_son_inter_freq_carrier_t *p_interfreq_carr_params = OAM_NULL;
    oam_son_anr_meas_config_req_t *p_anr_meas_config_req = OAM_NULL;
    oam_son_anr_meas_config_obj_remove_req_t *p_req = OAM_NULL;
    oam_tcb_cntxt_data_t *p_tcb_data = p_add_data;
    UInt16 trans_id = OAM_ZERO;
    UInt16 dest_id = OAM_ZERO;
    UInt16 msg_len = OAM_ZERO;

    SInt8 cell_config_idx = OAM_ERROR_RETURN_VAL;
    SInt8 cell_config_idx1 = OAM_ERROR_RETURN_VAL;


    if (!p_err) {
        OAM_LOG(OAM, OAM_WARNING, "NULL pointer error");

        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    cell_config_idx = oam_get_son_inter_freq_carrier_idx(cid);
    if(OAM_ERROR_RETURN_VAL == cell_config_idx) {
        OAM_LOG(OAM, OAM_WARNING, "Invalid cell context[cid=%d]", cid);
        *p_err = OAM_INVALID_CELL_CONTEXT;

        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    cell_config_idx1 = oam_get_son_serving_cell_idx(cid);
    if(OAM_ERROR_RETURN_VAL == cell_config_idx1) {
        OAM_LOG(OAM, OAM_WARNING, "Invalid cell context[cid=%d]", cid);
        *p_err = OAM_INVALID_CELL_CONTEXT;

        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    p_interfreq_carr_params = &SON_INTER_FREQ_CARR_PARAMS(cell_config_idx);
    p_anr_meas_config_req = &SON_ANR_MEAS_CONFIG_REQ(cell_config_idx);



    if (OAM_ADD_OPERATION == add_del_flag) {
        OAM_LOG(OAM, OAM_INFO,
                     "Adding GERANFreqGroup object with instance-id: %d",
                     instance_id);
    }
    else if (OAM_DELETE_OPERATION == add_del_flag) {
        OAM_LOG(OAM, OAM_INFO,
                     "Deleting GERANFreqGroup object with instance-id: %d",
                     instance_id);

        if(!(p_interfreq_carr_params->geran_meas_entry_bitmap &
                                    (1 << arr_idx))) {
            trans_id = get_word_from_header((UInt8*)p_msg +
                                    OAM_INTF_HDR_TRANS_ID_OFFSET);
            dest_id = get_word_from_header((UInt8*)p_msg +
                                    OAM_INTF_HDR_SRC_ID_OFFSET);

            OAM_LOG(OAM, OAM_DETAILED,
                    "Instance not added at SON, sending success response");
            /* Send delete object response from here only */
            oam_construct_n_send_deleteobject_resp_to_usr(OAM_RESULT_SUCCESS,
                                                          OAM_ZERO, OAM_NULL,
                                                          OAM_ZERO,
                                                          OAM_VERSION_ID,
                                                          OAM_MODULE_ID,
                                                          dest_id, trans_id,
                                                          OAM_ZERO, 
                                                          OAM_ZERO,
                                                          p_err);

            oam_prov_req.system_status.event_ongoing &=
                                ~OAM_DELETE_OBJ_CELL_RECONFIG_ONGOING;

            OAM_FUN_EXIT_TRACE();
            return OAM_SUCCESS;
        }

        p_interfreq_carr_params->query_index = arr_idx;
        p_interfreq_carr_params->config_ongoing |=
                            OAM_ANR_ACTION_DELETE_GERAN_FREQ_GROUP_ONGOING;


        ret_val = oam_mem_alloc(
                          sizeof(oam_son_anr_meas_config_obj_remove_req_t),
                          (void**)&p_req, p_err);

        if(OAM_FAILURE == ret_val) {
            OAM_LOG(OAM, OAM_ERROR,
                    "Memory allocation to p_req failed"
                    " with Error Code:%d",
                    *p_err);

            OAM_FUN_EXIT_TRACE();
            return ret_val;
        }

        oam_memset(p_req, OAM_ZERO,
               sizeof(oam_son_anr_meas_config_obj_remove_req_t));


        oam_memcpy(&p_req->src_cgi,
                 &(SON_CELL_INFO_IND_SERVING_CELL(cell_config_idx1).src_cgi),
                 sizeof(son_intra_rat_global_cell_id_t));

        p_req->geran_remove_list_size = 1;

        /* SPR 14651 START */
        --p_anr_meas_config_req->other_rat_config_list.
                  geran_config_list.geran_freq_info_list_size;

        p_interfreq_carr_params->geran_meas_entry_bitmap &=
                            ~(1 << arr_idx);

        oam_prov_req.system_status.event_ongoing |=
                                OAM_DELETE_OBJ_CELL_RECONFIG_ONGOING;  
        /* SPR 14651 END */

        oam_memcpy(&p_req->geran_remove_list[0],
                   &p_anr_meas_config_req->other_rat_config_list.
                       geran_config_list.geran_freq_info_list[arr_idx].
                       geran_car_freqs,
                   sizeof(son_geran_carrier_freqs_t));

        trans_id = oam_get_new_trans_id();
        ret_val =
            oam_son_send_son_oam_anr_meas_config_obj_remove_req(p_req,
                                                               OAM_MODULE_ID,
                                                               SON_MODULE_ID,
                                                               trans_id, cid);

        if (OAM_FAILURE == ret_val) {
            OAM_LOG(OAM, OAM_ERROR, "Failed to send request to lower layer");
            *p_err = OAM_SEND_FAIL;
        }
        else {
            msg_len = get_word_from_header((UInt8*)p_msg +
                                            OAM_INTF_HDR_MSG_SIZE_OFFSET);

#ifndef OAM_UT_FRAMEWORK
            oam_maintain_tcb_ext(trans_id,
                                 OAM_TRANS_SON, msg_len,
                                 p_msg,
                                 p_tcb_data->p_orig_xml_parent,
                                 p_tcb_data->p_usr_msg_xml_parent,
                                 p_tcb_data->p_parent_xml_struct_addr);
#endif
        }

        oam_mem_free(p_req, p_err);
    }

    OAM_FUN_EXIT_TRACE();
    return ret_val;
}


/****************************************************************************
*   FUNCTION NAME   : oam_generate_instance_id_for_geran_freq_group_list
*   DESCRIPTION     : This function will generate instance id for inter freq 
*                     carrier list
*   Inputs          : p_instance_id : Instance ID
*                     max_entries : Max Entries
*   Outputs         : NONE
*   RETURNS         : OAM_SUCCESS/OAM_FAILURE
****************************************************************************/
oam_return_et 
oam_generate_instance_id_for_geran_freq_group_list(UInt16 *p_instance_id,
                                                 UInt16  max_entries,
                                                 oam_error_code_et *p_err)
{
    OAM_FUN_ENTRY_TRACE();
    SInt8  i = OAM_ZERO;
    UInt8  start_idx = OAM_ZERO;
    UInt32 is_valid = OAM_ZERO;
    oam_son_inter_freq_carrier_t *p_cell_inter_freq_carr_params = OAM_NULL;

    /* COV CID: 64247 FIX START */
    if (!p_instance_id || !p_err) {
    /* COV CID: 64247 FIX END */
        OAM_LOG(OAM, OAM_WARNING, "NULL pointer error");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    *p_instance_id = OAM_ZERO;

    /* COV CID: 64246 FIX START */
    i = oam_get_son_inter_freq_carrier_idx(
                                oam_prov_req.cell_cntxt.curr_cell_cid);
    if(OAM_ERROR_RETURN_VAL == i) {
        OAM_LOG(OAM, OAM_WARNING, "Carrier data not found");
        *p_err = OAM_INVALID_CELL_CONTEXT;

        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    p_cell_inter_freq_carr_params = 
                &SON_PROV_REQ.oam_son_inter_freq_carrier_params[i];
    /* COV CID: 64246 FIX END */

    /* Exhuast list first, and then rollover to assign instance id */
    start_idx = p_cell_inter_freq_carr_params->geran_freq_group_last_inst_id;


    for (i = start_idx; i < max_entries && i < SON_MAX_CONFIG_OBJECT; ++i) {
        is_valid = p_cell_inter_freq_carr_params->
                                      geran_meas_entry_bitmap & (1 << i);

        if (!is_valid) {
            *p_instance_id = i + 1;
            /* Save the last added instance id */
            p_cell_inter_freq_carr_params->geran_freq_group_last_inst_id =
                                                            *p_instance_id;
            OAM_FUN_EXIT_TRACE();
    	    return OAM_SUCCESS; 
        }
    }
    /* No instance available from last added till end, then search from start 
     * till last added */

    if(OAM_ZERO == *p_instance_id) {
        for (i = 0;
             i < p_cell_inter_freq_carr_params->geran_freq_group_last_inst_id;
             ++i) {
            is_valid = p_cell_inter_freq_carr_params->
                                    geran_meas_entry_bitmap & (1 << i);

            if (!is_valid) {
                *p_instance_id = i + 1;
                /* Save the last added instance id */
                p_cell_inter_freq_carr_params->geran_freq_group_last_inst_id =
                                                            *p_instance_id;
                OAM_FUN_EXIT_TRACE();
                return OAM_SUCCESS;
            }
        }
    }

    *p_err = OAM_RESOURCE_EXCCEED;

    OAM_FUN_EXIT_TRACE();
    return OAM_FAILURE; 
}
#endif

/****************************************************************************
 * Function Name : oam_construct_n_send_addobject_resp_to_usr 
 * Description   : This function prepares a OAM_ADD_OBJECT_REQ and
 *                 sends it to user interface.
 * Inputs        : 1. resp_result :Result of response
 *                 2. resp_error_code :Respponse Error Code
 *                 3. err_string :Pointer to Error String
 *                 4. oam_version :OAM version
 *                 5. src_id :Source ID
 *                 6. dst_id :Destination ID
 *                 7. req_trans_id :Transaction ID
 *                 8. api_version :API verion
 *                 9. api_hdr_resvd_byte :API header reserved byte
 *                 10.instance_number :Instance number
 * Outputs       : 1.p_err_code :Pointer to Error Code
 * Returns       : Void
 ****************************************************************************/
void 
oam_construct_n_send_addobject_resp_to_usr(
        UInt8 resp_result,
        UInt32 resp_error_code,
        UInt8 *err_string,
        UInt8 err_string_size,
        UInt8 oam_version,
        UInt16 src_id,
        UInt16 dst_id,
        UInt16 req_trans_id,
        UInt8 api_version,
        UInt8 api_hdr_resvd_byte,
        oam_error_code_et *p_err_code , 
        UInt16 instance_number
        )
{
    OAM_FUN_ENTRY_TRACE();

    UInt16 msg_len = OAM_CSPL_HDR_SIZE + OAM_MGMT_INTF_HEADER_SIZE;
    UInt16 payload_len = OAM_ZERO;
    UInt8 *payload = OAM_NULL;
    UInt8 *p_msg = OAM_NULL;
    UInt32 curr_pos = OAM_ZERO;

    /*OAM_REVIEW_CHANGES*/
    if(OAM_NULL == err_string || OAM_RESULT_SUCCESS == resp_result) 
    /*OAM_REVIEW_CHANGES*/
    {
        payload_len = OAM_MAX_API_TO_LOWER_LAYER ;
    }
    else 
    {
        payload_len = OAM_MAX_API_TO_LOWER_LAYER + err_string_size;
    }

    msg_len += payload_len;

    p_msg = oam_msg_mem_get(msg_len);
    payload = p_msg; 
    if(PNULL == p_msg || PNULL == payload) 
    {
        OAM_LOG(OAM, OAM_WARNING, "memory allocation failed");
        *p_err_code = OAM_MALLOC_FAIL;
        OAM_FUN_EXIT_TRACE();
        return;
    }

    /* encode CSPL header in message*/
    oam_construct_api_header(payload, oam_version,
            src_id, dst_id, OAM_ADD_OBJECT_RESP, msg_len);
    payload += OAM_CSPL_HDR_SIZE;

    /* encode API header in message*/
    oam_construct_mgmt_api_hdr(payload, req_trans_id, src_id, dst_id,
            OAM_ADD_OBJECT_RESP, msg_len - OAM_CSPL_HDR_SIZE,
            api_hdr_resvd_byte);
            /*SPR 17777 fix*/
    payload += OAM_MGMT_INTF_HEADER_SIZE;


    /* There will be exactly 12 bytes in the response message payload */
    /* The structure of the payload will be as follows:
     * |   1 BYTE   |  2 BYTES   | 4 BYTES  | 1 BYTE   |  2 BYTE  | 2 BYTE | 
     * +-------------------------------------------------------------------+ 
     * |    Status  |            | Reserved | Length of| Instance |Reserved
     * | 0=SUCCESS  | Error Code | (all 0s) |   error  | Number   |
     * | 1=FAILURE  |            |          | string   |          |
     * +------------+------------+----------+----------+ ---------+-------*/


    /* fill the response code */
    *(payload + curr_pos++) = resp_result;


    *(payload + curr_pos++) = (resp_error_code & OAM_UPPER_EIGHT_BITS_SET) >> OAM_EIGHT;
    *(payload + curr_pos++) =  resp_error_code  & OAM_ALL_8_BIT_SET;

    /*packing reserved field*/
    *(payload + curr_pos++) = OAM_ZERO; 
    *(payload + curr_pos++) = OAM_ZERO; 
    *(payload + curr_pos++) = OAM_ZERO; 
    *(payload + curr_pos++) = OAM_ZERO; 

    /*packing error string length*/
    *(payload + curr_pos++) = err_string_size;

    /* Encode error string only when there is failure */
    if(OAM_NULL != err_string)
    {
        oam_memcpy(payload + curr_pos++, err_string, err_string_size);
        curr_pos += err_string_size;
    }

    /* filling  Instance number of the object*/
    *(payload + curr_pos++) = (instance_number & OAM_UPPER_EIGHT_BITS_SET) >> OAM_EIGHT;
    *(payload + curr_pos++) =  instance_number  & OAM_ALL_8_BIT_SET;

    /* 2 bytes Reserved */
    *(payload + curr_pos++) = OAM_ZERO; 
    *(payload + curr_pos++) = OAM_ZERO; 

    /* Coverity CID 107824 Fix Start */
    if(OAM_FAILURE == oam_send_message(p_msg, dst_id, p_err_code))
    {
        /*SPR 17777 fix*/
        OAM_LOG(OAM, OAM_ERROR,
                "Error: Sendto failed with err_code %u; Unused %d", *p_err_code,api_version);
    }
    /* Coverity CID 107824 Fix End */

    OAM_FUN_EXIT_TRACE();
}


/******************************************************************************
 * Function Name  : oam_delete_object_api_handler 
 * Description    : This function handles delete object api from tr69 
 * Inputs         : p_msg : Pointer to message Buffer
 * Output         : p_err
 * Returns        : OAM_SUCCESS/OAM_FAILURE  
 ******************************************************************************/

oam_return_et
oam_delete_object_api_handler(void *p_msg,
                              oam_error_code_et *p_err)
{
    UInt16 length  = OAM_ZERO; 
    Char8  obj_name[MAX_PARENT_NAME_LENGTH] = {OAM_ZERO};
    UInt8 *p_offset = p_msg;
    oam_return_et retVal = OAM_FAILURE;
    /* SPR 15527 START */
    UInt8 fap_index_value = OAM_ZERO;
    UInt8 current_cid = OAM_ZERO;
	SInt8 cell_arr_idx = OAM_ZERO;
	oam_cell_status_t *p_cell_status = OAM_NULL;
	UInt8 cid = oam_prov_req.cell_cntxt.curr_cell_cid;
    /* SPR 15527 END */

    OAM_FUN_ENTRY_TRACE();

    /* Increment the pointer past the API header */
    p_offset += OAM_TR069_INTF_HEADER_SIZE;   

    /*Increment the pointer past Reserved bytes */
    p_offset += sizeof(UInt32);

    /* Length of the object name in question */
    length = get_word_from_header(p_offset);  

    /*Increment the pointer past length field read */
    p_offset += sizeof(UInt16);

    /*Increment the pointer past reserved bytes */
    p_offset += sizeof(UInt16);

    if(length < MAX_PARENT_NAME_LENGTH) {
        oam_memcpy(obj_name, (Char8*)p_offset, length);

        OAM_LOG(OAM, OAM_DETAILED, "Object name = %s", obj_name);
        p_offset += length;

        /* SPR 15527 START */
        oam_find_fap_index(obj_name,&fap_index_value);
        if(fap_index_value)
        {    
            current_cid = oam_prov_req.cell_cntxt.cell_ctxt_status[fap_index_value - 1].cid; 
            p_cell_status = oam_get_cell_cntxt_status(current_cid);

            if (p_cell_status) 
            {
                oam_prov_req.cell_cntxt.curr_cell_cid = current_cid;
                cid = oam_prov_req.cell_cntxt.curr_cell_cid;
                cell_arr_idx = oam_get_rrm_cell_config_idx(cid);
                if(OAM_ERROR_RETURN_VAL == cell_arr_idx) {
                    OAM_LOG(OAM, OAM_ERROR, "Invalid cell context[cid=%d]",
                            cid);
                    *p_err = OAM_INVALID_CELL_CONTEXT;
                    OAM_FUN_EXIT_TRACE();
                    return OAM_FAILURE;
                }
                OAM_LOG(OAM, OAM_INFO,"Cell Context Changed to CID: %d, fap_index_value %d", 
                        current_cid,fap_index_value);
            }
            else {
                OAM_LOG(OAM, OAM_ERROR,"Invalid Cell ID: %d Cell Context not changed, fap_index_value %d",
                        current_cid,fap_index_value);
                *p_err = OAM_INVALID_CELL_CONTEXT;
                OAM_FUN_EXIT_TRACE();
                return OAM_FAILURE;
            }
        }
        /* SPR 15527 END */
        retVal = oam_delete_object(obj_name, p_msg, p_err);

        if(OAM_FAILURE ==  retVal) {
            OAM_LOG(OAM, OAM_WARNING, "Error while deleting object");
        }
    }
    else {
        OAM_LOG(OAM, OAM_WARNING, "Object name too long[%u] to process", length);

        *p_err = OAM_PARAM_NAME_TOO_LONG;
    }
       
    OAM_FUN_EXIT_TRACE();
    return retVal; 
}
/******************************************************************************
 * * Function Name  : oam_delete_object
 * * Description    : This function handles delete object api from tr69
 * * Inputs         : obj_name : Pointer to object name
 * *                  p_msg : Pointer to message 
 * * Output         : p_err
 * * Returns        : return OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et 
oam_delete_object(Char8 *obj_name, void *p_msg, oam_error_code_et *p_err)
{
    OAM_FUN_ENTRY_TRACE();

    /* SPR 16603 Fix Start */
    /*SPR 19977 FIX Start*/ 
    UInt8 cid = oam_prov_req.cell_cntxt.curr_cell_cid;
    UInt8 cell_idx = OAM_ZERO,idx=OAM_ZERO;
    /*SPR 19977 FIX End*/ 
    /* SPR 16603 Fix End */
    UInt8   loop = OAM_ZERO;
    xmlChar   *p_param_val = OAM_NULL;
    Char8   *token = OAM_NULL;
    Char8   buff[MAX_PARAMETER_NAME_LENGTH] = {OAM_ZERO};
    oam_bool_et	is_deleted = OAM_FALSE;
    Char8   *token_1 = OAM_NULL;
    xmlNode *p_obj_parent = OAM_NULL;
    xmlNode *p_curr_obj = OAM_NULL;
    xmlNode *p_obj_first_sibling = OAM_NULL;
    oam_tcb_cntxt_data_t tcb_data = {OAM_ZERO};

    oam_bool_et	valid_object_name = OAM_FALSE;
    oam_bool_et	obj_found = OAM_FALSE;
    oam_return_et   retVal = OAM_SUCCESS;

      /* Spr 15711 Fix Start */
    UInt8   instance_no = OAM_ZERO;
    Char8   err_str[STR_LEN_255]={OAM_ZERO};
    UInt8 inst_id_val =  OAM_ZERO;
    oam_error_code_et err = NO_ERROR;
    /*SPR 19977 FIX Start*/ 
    /*Code Deleted*/
    /*SPR 19977 FIX End*/ 
    UInt16  trans_id = OAM_ZERO, error_code = OAM_ZERO , dest_id = OAM_ZERO;
      /* Spr 15711 Fix END */
    /*SPR_18921_START*/
    UInt16 * curr_num_entries = OAM_NULL;
    UInt16 max_num_entries = OAM_ZERO;
    /*SPR_18921_END*/

    *p_err = NO_ERROR;
  /*SPR_15770 Fix start*/
  /*object deletion is initiated by OAM at Init time 
  thus check data pointer is valid or not*/
    /*SPR 19977 FIX Start*/ 
    for ( ; idx < oam_prov_req.num_cells_configured; ++idx) {
	    if (cid == oam_prov_req.oam_rrm_req.plmnlist_data[idx].cid) {
		    cell_idx = idx;
	    }
    }
    /*SPR 19977 FIX End*/ 
  if(OAM_NULL != p_msg) 
  {
      trans_id = get_word_from_header((UInt8*)(p_msg) + OAM_ZERO);
      dest_id = get_word_from_header((UInt8*)(p_msg) + OAM_TWO);
  }
  /* Cov CID 107818 Fix Start */
  else
  {
       OAM_LOG(OAM,OAM_WARNING, "NULL POINTER p_mgs is received !!!!!!");
       return OAM_FAILURE;

  }
  /* Cov CID 107818 Fix End */
  /*SPR_15770 Fix end*/

    /*Check for max no.s of obj supported*/
    for (loop = OAM_ZERO; loop < OAM_MAX_SUPPORTED_OBJECTS; ++loop) {
        /* Search object name in object_map_element_t */
        /* If not found, send failure delete object response */
        if (oam_strstr(obj_name, g_object_list[loop].object_name)) {
            retVal =
                /*SPR_18921_START*/
                g_object_list[loop].obj_data_accessor(&curr_num_entries,
                        &max_num_entries,
                        /*SPR_18921_END*/
                        /* SPR 16603 Fix Start */                
                                                        &p_obj_parent, 
                                                        &p_obj_first_sibling,
                                                        OAM_NULL,
                                                        &cell_idx,
/* SPR 16603 Fix End */                
                                                        p_err);

            if (OAM_FAILURE == retVal) {
                break;
            }

            /*Get Instance Number*/
            /* Move ahead object name + "." and "[" */
            token = obj_name + oam_strlen(g_object_list[loop].object_name)
                    + OAM_TWO;
            token_1 = oam_strtok(token, "]");

            if(OAM_NULL == token_1) {
                OAM_LOG(OAM, OAM_WARNING, "Token extracted is Null"); 
                OAM_FUN_EXIT_TRACE();
                *p_err = OAM_PARSE_MSG_DMG;
                retVal = OAM_FAILURE;
                break;
            }

            /*Convert string to integer*/  
            instance_no = oam_atoi(token_1);

  /* Cov CID 107817 Fix Start */
            if(instance_no <= OAM_ZERO)
            {
               OAM_LOG(OAM,OAM_ERROR,"Wrong Value of Instance received =%d",instance_no);
               if (!oam_strcmp(obj_name, "InternetGatewayDevice.Services.FAPService.[1]."
                           "CellConfig.LTE.RAN.Mobility.IdleMode.InterFreq.Carrier")) 
               {
                   oam_mem_free(curr_num_entries, &err);
               }
               return OAM_FAILURE;
            }
  /* Cov CID 107817 Fix End */


/* SPR 17409 changes start */
/* code removed */
/* SPR 17409 changes end */

       OAM_LOG(OAM, OAM_INFO,
                         "Delete Instance no is  = %d", instance_no);   
      /* Spr 15711 Fix Start */
      /*Cov 93921 fix start*/
            /*SPR_18921_START*/
      if ((cell_idx < OAM_MAX_CELL_SUPPORTED) && 
          (oam_strncasecmp(g_object_list[loop].tag_name, "PLMNList", oam_strlen("PLMNList")) == OAM_ZERO) &&
          ((RRM_PROV_REQ.plmnlist_data[cell_idx].plmn_entries[instance_no - 1].is_primary) || (*curr_num_entries == 1)))
      /*SPR_18921_END*/

      {
          OAM_LOG(OAM, OAM_WARNING, "Object Deletion Failed. "
                                  "Either trying to delete the last entry or Primary PLMNID.");
          break;
      }
      /*SPR_18921_START*/
      else if((oam_strncasecmp(g_object_list[loop].tag_name, "QoS", oam_strlen("QoS")) == OAM_ZERO) &&
              (*curr_num_entries == 1))
      /*SPR_18921_END*/
      /*Cov 93921 fix end*/    
      {
          OAM_LOG(OAM, OAM_WARNING,"Object Deletion Failed. "
                                 "One entry must be present for requested Object: QoS");
          break;
      }
      /* Spr 15711 Fix End */


            /* Points to first object in parent node */
            p_curr_obj = p_obj_parent->children;
           
            while(OAM_NULL != p_curr_obj) {
                p_curr_obj = oam_xml_find_tag(p_curr_obj,
                                     (xmlChar*)g_object_list[loop].tag_name);

                if(!p_curr_obj) {
                    OAM_LOG(OAM, OAM_WARNING, "Object not found");
                    *p_err = OAM_PARAMETER_NOT_FOUND;
                    retVal = OAM_FAILURE;
                    break;
                }

                p_param_val = oam_get_xml_text_node_content(p_curr_obj,
                                                  "X_VENDOR_INSTANCE_ID",
                                                  buff,
                                                  MAX_PARAMETER_NAME_LENGTH);

                if (p_param_val) {
                    inst_id_val =  oam_atoi((Char8*)p_param_val);

                    if (instance_no == inst_id_val) {
                        obj_found = OAM_TRUE; 
                        break;
                    }
                }
                else {
                    OAM_LOG(OAM, OAM_WARNING, "Object not found");
                    *p_err = OAM_PARAMETER_NOT_FOUND;
                    retVal = OAM_FAILURE;
                    break;
                }

                /* Move to next instance of object */
                p_curr_obj = p_curr_obj->next;

                /* Spr 15711 Fix Start */
                OAM_LOG(OAM,OAM_DETAILED," Object [%d] deleted Successfully",inst_id_val); 
                /* Spr 15711 Fix End */
            }   /* End of internal while loop */

            /* Coverity 110747 + */
			if ( (obj_found) && (cell_idx < OAM_MAX_CELL_SUPPORTED) ) {
            /* Coverity 110747 - */
                /* If state is not active,
                 * don't send message to lower layer
                 */
/* SPR 17409 fix start */
                valid_object_name = OAM_TRUE;
/* SPR 17409 fix end */
                if (OAM_CM_ST_ACTIVE ==  p_cm_cntxt->state) {
                    tcb_data.p_usr_msg_xml_parent = p_obj_first_sibling;
                    tcb_data.p_orig_xml_parent = p_curr_obj;
                    tcb_data.p_parent_xml_struct_addr = g_object_list[loop].
                            parameter_table_address[0].xml_parameter_parent;
                    /* send message to lower layer */
                    retVal = g_object_list[loop].lower_layer_handler(
                                                        OAM_DELETE_OPERATION,
                                                        p_msg,
                                                        inst_id_val,
                                                        &tcb_data, p_err);

                    if (OAM_SUCCESS == retVal) {
        /* SPR 18930  FIX START */
                        //oam_prov_req.system_status.event_ongoing |=
                          //      OAM_DELETE_OBJ_CELL_RECONFIG_ONGOING;
        /* SPR 18930  FIX END */
/* SPR 17409 fix start */
                        is_deleted = OAM_TRUE;

        /* SPR 18930  FIX START */
#ifdef OAM_SON_ENABLE
                        /* SPR 17678 changes start */ /*  */
                        /* SPR 17678 changes end */
#endif
        /* SPR 18930  FIX END */
/* SPR 17409 fix end */
    /*SPR 19977 FIX Start*/ 
    /*Code Deleted*/ 
    /*SPR 19977 FIX End */ 
                    }

                }
            }
            else {
                *p_err = OAM_PARAMETER_NOT_FOUND;
                retVal = OAM_FAILURE;
            }
            break;
        }/* End of if object found */
    }/* End of outer for loop */
  /*SPR_15770 Fix start*/
  if(OAM_NULL != p_msg)
  {
      /* Search object name, If not found, send failure delete object response */
      /* Spr 15711 Fix Start */
      if ((valid_object_name == OAM_FALSE)||
              ((valid_object_name == OAM_TRUE) && (is_deleted == OAM_FALSE)))  /*Checking for invalid param name path*/
      {
          if(valid_object_name == OAM_FALSE) 
          {
              OAM_LOG(OAM, OAM_WARNING, "oam_delete_object: Invalid param name.");
              oam_strcpy(err_str,"Invalid param name");
              error_code = OAM_ERR_INVALID_PARAM_NAME; 
          }
          else
          {
              oam_strcpy(err_str,"Request Denied");
              error_code = OAM_ERR_REQUEST_DENIED; 

          }
          oam_construct_n_send_deleteobject_resp_to_usr(OAM_RESULT_FAILURE,   
                  error_code,
                  (UInt8 *)err_str, oam_strlen(err_str)+1, 
                  OAM_VERSION_ID, OAM_MODULE_ID,
                  dest_id, trans_id, OAM_ZERO, OAM_ZERO,
                  &err);  
          /* Spr 15711 Fix End */
      }
    /*SPR 19977 FIX Start*/ 
    /*Code Deleted*/ 
    /*SPR 19977 FIX End */ 
  }
  /*SPR_15770 Fix end*/


    OAM_FUN_EXIT_TRACE();
    if (!oam_strcmp(obj_name, "InternetGatewayDevice.Services.FAPService.[1]."
                "CellConfig.LTE.RAN.Mobility.IdleMode.InterFreq.Carrier")) 
    {
        oam_mem_free(curr_num_entries, &err);
    }
    return retVal; 
}
/****************************************************************************
 * Function Name : oam_construct_n_send_deleteobject_resp_to_usr 
 * Description   : This function prepares a OAM_DELETE_OBJECT_REQ and
 *                 sends it to user interface.
 * Inputs        : 1. resp_result :Result of response
 *                 2. resp_error_code :Response Error Code
 *                 3. err_string : Pointer to Error String
 *                 4. err_string_size :Error String size
 *                 5. oam_version :OAM version
 *                 6. src_id :Source ID
 *                 7. dst_id :Destination ID
 *                 8. req_trans_id :Req. Transaction ID
 *                 9. api_version :API verion
 *                 10.api_hdr_resvd_byte :API header reserved byte
 * Outputs       : p_err_code :Pointer to Error code
 * Returns       : Void
 *****************************************************************************/
void 
oam_construct_n_send_deleteobject_resp_to_usr(
        UInt8 resp_result,
        UInt16 resp_error_code,
        UInt8 *err_string,
        UInt8 err_string_size,
        UInt8 oam_version, 
        UInt16 src_id,
        UInt16 dst_id, 
        UInt16 req_trans_id,
        UInt8 api_version,
        UInt8 api_hdr_resvd_byte,
        oam_error_code_et *p_err_code
        )
{
    OAM_FUN_ENTRY_TRACE();
    UInt16 msg_len = OAM_CSPL_HDR_SIZE + OAM_MGMT_INTF_HEADER_SIZE;
    UInt16 payload_len = OAM_ZERO;
    UInt8 *payload = OAM_NULL;
    UInt8 *p_msg = OAM_NULL;



    /*OAM_REVIEW_CHANGES*/
    if(OAM_NULL == err_string || OAM_RESULT_SUCCESS == resp_result) 
    {
        payload_len = OAM_RRH_SET_RESP_PAYLOAD_LEN;
    }
    else
    {
        payload_len = OAM_RRH_SET_RESP_PAYLOAD_LEN + err_string_size;
    }

    msg_len += payload_len;

    p_msg = oam_msg_mem_get(msg_len);
    payload = p_msg;

    if(PNULL == p_msg || PNULL == payload) {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_msg failed");
        *p_err_code = OAM_MALLOC_FAIL;
        OAM_FUN_EXIT_TRACE();
        return;
    }

    /* encode CSPL header in message*/
    oam_construct_api_header(payload, oam_version,
            src_id, dst_id, OAM_DELETE_OBJECT_RESP, msg_len);
    payload += OAM_CSPL_HDR_SIZE;

    /* encode API header in message*/
    oam_construct_mgmt_api_hdr(payload, req_trans_id, src_id, dst_id,
            OAM_DELETE_OBJECT_RESP, msg_len - OAM_CSPL_HDR_SIZE,
            api_hdr_resvd_byte);
            /*SPR 17777 fix*/
    payload += OAM_MGMT_INTF_HEADER_SIZE;


    /* There will be exactly 8 bytes in the response message payload */
    /* The structure of the payload will be as follows:
     * |   1 BYTE   |  2 BYTES   | 4 BYTES  | 1 BYTE   |
     * +-----------------------------------------------+
     * |    Status  |            | Reserved | Length of|
     * | 0=SUCCESS  | Error Code | (all 0s) |   error  |
     * | 1=FAILURE  |            |          | string   |
     * +------------+------------+----------+----------+ */


    /* fill the response code */
    *payload = resp_result;
    if(OAM_RESULT_FAILURE == resp_result)
    {	
        *(payload + OAM_ONE) = (resp_error_code >> OAM_EIGHT) & OAM_ALL_8_BIT_SET;
        *(payload + OAM_TWO) =  resp_error_code  & OAM_ALL_8_BIT_SET;
        oam_memset(payload + OAM_THREE, OAM_ZERO, OAM_FOUR);
        if(OAM_NULL == err_string) {
            *(payload + OAM_SEVEN) = OAM_ZERO;
        }
        else {
            *(payload + OAM_SEVEN) = err_string_size;
            oam_memcpy(payload + OAM_EIGHT, err_string, err_string_size);
        }
    }
    else 
    {
        oam_memset(payload + OAM_ONE, OAM_ZERO, OAM_SEVEN);
    }

    /* Coverity CID 107823 Fix Start */
    if(OAM_FAILURE == oam_send_message(p_msg, dst_id, p_err_code))
    {/*SPR 17777 fix*/
        OAM_LOG(OAM, OAM_ERROR,
                "Error: Sendto failed with err_code %u;Unused %d", *p_err_code,api_version);
    }
    /* Coverity CID 107823 Fix End */

    OAM_FUN_EXIT_TRACE();

}
/******************************************************************************
 * Function Name  : oam_tr69_controll_cmd_req
 * Description    : This function handles controll command req api to tr69 
 * Inputs         : p_params_list : Pointer to Pointer to Parameter list
 *                : num_params : Number of parameters
 *                : sub_module_id :Sub module ID
 *                : dest_module_id :Dest Module ID
 *                : src_id : Source ID
 *                : dst_id : Destination ID
 *                : oam_version : OAM Version
 *                : req_trans_id : Req. transaction ID
 *                : api_version : API Version
 *                : api_hdr_resvd_byte : API Header Reserved Byte
 * Output         : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE  
 *
 ******************************************************************************/

oam_return_et 
oam_tr69_controll_cmd_req( 
        char** p_params_list,
        UInt8 num_params ,
        UInt16  sub_module_id,
        /* SPR 17777 fix code removed */
        UInt16 src_id,
        UInt16 dst_id,
        UInt8 oam_version,
        UInt16 req_trans_id,
        UInt8 api_version,
        UInt8 api_hdr_resvd_byte
        )
{

    OAM_FUN_ENTRY_TRACE();

    UInt16 msg_len = OAM_CSPL_HDR_SIZE + OAM_MGMT_INTF_HEADER_SIZE;

    UInt16 payload_len = OAM_ZERO;
    UInt8 *payload = OAM_NULL;
    UInt8 *p_msg = OAM_NULL;
    oam_error_code_et   error_code = NO_ERROR; 
    UInt32 curr_pos = OAM_ZERO;
    UInt16 transaction_id = OAM_ZERO;
    char cmd_name[STR_LEN_256] , cmd_name1[STR_LEN_256] , cmd_name2[STR_LEN_256] , cmd_name3[STR_LEN_256] , cmd_name4[STR_LEN_256]; 
    char 	param_name[STR_LEN_256]; 
    UInt16 	param_name_len= OAM_ZERO,cmd_name_len= OAM_ZERO, param_node_len= OAM_ZERO, admin_node_len= OAM_ZERO;
    UInt16 cmd_name_len1= OAM_ZERO ,cmd_name_len2= OAM_ZERO , cmd_name_len3= OAM_ZERO , cmd_name_len4= OAM_ZERO , additional_param_cmd_len = OAM_ZERO;
    UInt16 	index = OAM_ZERO;
    oam_trans_type_et trans_type = OAM_TRANS_SON;

    oam_memset(cmd_name, OAM_ZERO, STR_LEN_256);
    oam_strcpy(cmd_name, p_params_list[OAM_ZERO]);
    cmd_name_len = oam_strlen(cmd_name) + OAM_ONE;	
    cmd_name[cmd_name_len - OAM_ONE] = '\0';

    admin_node_len = cmd_name_len + OAM_TWELVE; /* legth + Reserved + No of params */

    oam_memset(cmd_name1, OAM_ZERO, STR_LEN_256);
    oam_strcpy(cmd_name1, p_params_list[OAM_ONE]);
    cmd_name_len1 = oam_strlen(cmd_name1) + OAM_ONE;	
    cmd_name1[cmd_name_len1 - OAM_ONE] = '\0';

    oam_memset(cmd_name2, OAM_ZERO, STR_LEN_256);
    oam_strcpy(cmd_name2, p_params_list[OAM_TWO]);
    cmd_name_len2 = oam_strlen(cmd_name2) + OAM_ONE;	
    cmd_name2[cmd_name_len2- OAM_ONE] = '\0';

    oam_memset(cmd_name3, OAM_ZERO, STR_LEN_256);
    oam_strcpy(cmd_name3, p_params_list[OAM_THREE]);
    cmd_name_len3 = oam_strlen(cmd_name3) + OAM_ONE;	
    cmd_name3[cmd_name_len3- OAM_ONE] = '\0';   

    oam_memset(cmd_name4, OAM_ZERO, STR_LEN_256);
    oam_strcpy(cmd_name4, p_params_list[OAM_FOUR]);
    cmd_name_len4 = oam_strlen(cmd_name4) + OAM_ONE;	
    cmd_name4[cmd_name_len4 - OAM_ONE] = '\0';


    additional_param_cmd_len = cmd_name_len1 + cmd_name_len2 + cmd_name_len3 +  cmd_name_len4 ;


    payload_len = OAM_MAX_API_TO_LOWER_LAYER + cmd_name_len ;     

    msg_len += payload_len + (OAM_FOUR *sizeof(UInt32)) + additional_param_cmd_len;  

    if(OAM_FAILURE == oam_mem_alloc(msg_len,(void*)&p_msg, &error_code))
    {
        OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to p_msg failed"
                " with Error Code:%d",
                error_code);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
    payload = p_msg; 

    /* encode CSPL header in message*/
    oam_construct_api_header(payload, oam_version,
            src_id, dst_id, TR069_CONTROL_COMMAND_REQ, msg_len); 
    payload += OAM_CSPL_HDR_SIZE;

    /* encode API header in message*/
    oam_construct_mgmt_api_hdr(payload, req_trans_id, src_id, dst_id,
            TR069_CONTROL_COMMAND_REQ, msg_len - OAM_CSPL_HDR_SIZE,
            api_hdr_resvd_byte);
            /*SPR 17777 fix*/
    payload += OAM_MGMT_INTF_HEADER_SIZE;


    /* payload encryption */

    payload[OAM_ZERO]    =  (UInt8)(((UInt32 )admin_node_len) >> OAM_EIGHT);
    payload[OAM_ONE]    =  (UInt8)(((UInt32 )admin_node_len));
    curr_pos+=OAM_TWO;

    payload[OAM_TWO]    =  OAM_ZERO; 
    payload[OAM_THREE]    =  OAM_ZERO; 
    curr_pos+=OAM_TWO;

    oam_memcpy(payload+curr_pos, cmd_name, cmd_name_len);
    curr_pos+=cmd_name_len;

    *(payload + curr_pos) = OAM_FOUR; 
    curr_pos+=OAM_EIGHT;


    for(index = OAM_ZERO; index < num_params; ++index) 
    {
        oam_memset(param_name, OAM_ZERO, STR_LEN_256);
        oam_strcpy(param_name, p_params_list[OAM_ONE + index]);
        param_name_len = oam_strlen(param_name) + OAM_ONE;
        param_node_len = param_name_len + OAM_FOUR;
        payload_len += param_node_len;
        if(payload_len > msg_len) 
        {
            OAM_LOG(OAM, OAM_WARNING, "Payload length[%u] exceeds the allowed msg "
                        "length[%u]. Skipping some parameters...", payload_len, msg_len);
             /* Coverity ID : 72337 Fix Start*/
             oam_mem_free(p_msg, &error_code);
             /* Coverity ID : 72337 Fix End*/
            return (oam_return_et)(payload_len - param_node_len);
        }

        /* Encoding parameter node */
        *(payload + curr_pos++) = (param_node_len & OAM_UPPER_EIGHT_BITS_SET) >> OAM_EIGHT;
        *(payload + curr_pos++) = param_node_len & OAM_ALL_8_BIT_SET;
        *(payload + curr_pos++) = OAM_ZERO;
        *(payload + curr_pos++) = OAM_ZERO;
        oam_memcpy(payload + curr_pos, param_name, param_name_len);
        OAM_LOG(OAM, OAM_DETAILED, "Encoding parameter at payload: %s",
                                   payload + curr_pos);
        curr_pos += param_name_len;
    }



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

    transaction_id = oam_get_new_trans_id();
    if(OAM_FAILURE ==  oam_maintain_internal_tcb(transaction_id,trans_type,msg_len,p_msg))
    {
        OAM_LOG(OAM,OAM_WARNING," Error while maintaining internal tcb ");
        /* Coverity CID 72337 Fix Start */
        oam_mem_free((void *)p_msg, &error_code);
        /* Coverity CID 72337 Fix End */
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }


    if(OAM_FAILURE == oam_send_message(p_msg, dst_id, &error_code))
    {
            /*SPR 17777 fix*/
        OAM_LOG(OAM,OAM_ERROR,"Error in sending message : %d; Unused %d",error_code,api_version);  
        OAM_FUN_ENTRY_TRACE();
        return OAM_FAILURE;
    }


    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS; 
}

/******************************************************************************
* Function Name  : oam_handle_tr069_command_resp 
* Description    : This function handles controll command response from TR069
* Inputs         : p_buf : Pointer to Buffer 
* Output         : NONE
* Returns        : OAM_SUCCESS/OAM_FAILURE 
******************************************************************************/
oam_return_et
oam_handle_tr069_command_resp
(
    UInt8 *p_buf
)
{
    UInt8 status = OAM_ZERO; 
    UInt16 error_code = OAM_ZERO;
    UInt16 counter = OAM_ZERO;
    UInt8 len_str = OAM_ZERO; 
    UInt8 err_string[STR_LEN_256]; 

    OAM_FUN_ENTRY_TRACE();

    p_buf += OAM_TR069_INTF_HEADER_SIZE;  

    oam_memcpy(&status,p_buf,sizeof(UInt8)); 
    counter += sizeof(UInt8);

    error_code = get_word_from_header((UInt8*)(p_buf) + OAM_ONE); 

    counter += sizeof(UInt16);

    counter += OAM_FOUR*(sizeof(UInt8));

    oam_memcpy(&len_str,p_buf+ counter ,sizeof(UInt8));    

    counter += sizeof(UInt8);

    oam_memcpy(err_string,p_buf+ counter ,len_str); 

    OAM_LOG(OAM,OAM_INFO,"Status : %d. "
        "Error Code : %d. "
        "Error String : %s",status, error_code, err_string); 

    oam_pm_xml_handle_file_upload_response(status);
    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS; 

}
/******************************************************************************
* Function Name  : oam_handle_file_download_ind
* Description    : This function handles file download Indication from TR069
* Inputs         : p_msg : Pointer to message Buffer
* Output         : NONE
* Returns        : OAM_SUCCESS/OAM_FAILURE
******************************************************************************/
oam_return_et
oam_handle_file_download_ind
(
    UInt8 *p_msg
)
{
    OAM_FUN_ENTRY_TRACE();

    UInt8 file_type = OAM_ZERO;
    UInt32 file_size = OAM_ZERO;
    UInt16 file_name_length = OAM_ZERO,file_des_len = OAM_ZERO;
    UInt8 file_name[STR_LEN_128]={OAM_ZERO}, file_des[STR_LEN_64]={OAM_ZERO};
    UInt32 counter = OAM_ZERO;
    FILE  *fptr=OAM_NULL;
    UInt16 transaction_id = OAM_ZERO;
    UInt16 msg_len = OAM_ZERO;
    UInt8 *p_temp_buf= OAM_NULL;
    UInt16 result = OAM_ZERO;
    oam_return_et retVal = OAM_SUCCESS;

    /*SPR_FIXED_13038_START */
    UInt8 tr069_flag    = OAM_ZERO;
     
    /*SPR_FIXED_13038_END */

    counter += OAM_TR069_INTF_HEADER_SIZE;
    transaction_id = get_word_from_header((UInt8*)(p_msg) + OAM_ZERO);

    /*getting api_size*/
    msg_len= get_word_from_header((UInt8*)(p_msg) + OAM_EIGHT);

    OAM_LOG(OAM,OAM_INFO,"Transaction ID: %d",transaction_id);

    counter+=sizeof(UInt32);
    oam_memcpy(&file_type,p_msg+counter,sizeof(UInt8));

    OAM_LOG(OAM,OAM_DETAILED,"FILE TYPE = %d",file_type);
    counter+=sizeof(UInt8);

    if((OAM_TRUE == (oam_prov_req.system_status.cell_setup_complete))&& (file_type != FIRMWARE_UPGRADE_IMAGE))
    {
        OAM_LOG(OAM,OAM_WARNING,"Cell setup is complete, Returning failure");
        /* If cell setup is complete, send failure response to TR069 Agent */
        /* Since file downloaded is supported at init time only */
        if(OAM_FAILURE == oam_send_file_download_ind_resp(OAM_RESULT_FAILURE
                    ,OAM_ERR_REQUEST_DENIED,transaction_id,
                    (UInt8 *)"Request Denied:Download at run time not supported"))
        {
            OAM_LOG(OAM,OAM_ERROR,"Error in sending Response");
            OAM_FUN_EXIT_TRACE();
            return OAM_FAILURE;
        }
        OAM_FUN_EXIT_TRACE();
        return OAM_SUCCESS;
    }

    counter+=sizeof(UInt8)
        +sizeof(UInt8)
        +sizeof(UInt8);
    file_size  |= (UInt32)((UInt8*)p_msg+counter)[OAM_ZERO] << OAM_TWENTY_FOUR;
    file_size  |= (UInt32)((UInt8*)p_msg+counter)[OAM_ONE] << OAM_SIXTEEN;
    file_size  |= (UInt32)((UInt8*)p_msg+counter)[OAM_TWO] << OAM_EIGHT;
    file_size  |= (UInt32)((UInt8*)p_msg+counter)[OAM_THREE];

    counter+=sizeof(UInt32);

    OAM_LOG(OAM,OAM_DETAILED,"FILE SIZE = %d",file_size);

    file_name_length = file_name_length |(UInt16)((p_msg+counter)[OAM_ZERO] << OAM_EIGHT);
    file_name_length = file_name_length |(UInt16)((p_msg+counter)[OAM_ONE]);

    counter+=sizeof(UInt16)+sizeof(UInt16);

    OAM_LOG(OAM,OAM_DETAILED,"FILE NAME LENGTH = %d",file_name_length);

    oam_memcpy(file_name,p_msg+counter,file_name_length);
    counter+=file_name_length;


    OAM_LOG(OAM,OAM_DETAILED,"FILE NAME = %s",file_name);

    file_des_len = p_msg[counter];
    counter+=sizeof(UInt8);
    OAM_LOG(OAM,OAM_DETAILED,"FILE DESCRIPTION LENGTH = %d",file_des_len);

    counter+=sizeof(UInt8)+sizeof(UInt16);
    oam_memcpy(file_des,p_msg+counter,file_des_len);
    counter+=file_des_len;

    if((OAM_ZERO == file_name_length))
    {
        OAM_LOG(OAM,OAM_WARNING,"Unable to access file. "
            "Trying to enable by factory settings");

        /* Failure condition */
        if(OAM_FAILURE == oam_send_file_download_ind_resp(OAM_RESULT_FAILURE
                    ,OAM_ERR_FILE_ACCESS_FAILURE,transaction_id,(UInt8 *)"File Access Failure"))
        {
            OAM_LOG(OAM,OAM_WARNING,"Error in sending Response");
            OAM_FUN_EXIT_TRACE();
            return OAM_FAILURE;

        }
    }
    else
    {
        if(file_type == FIRMWARE_UPGRADE_IMAGE) /* File Type = 1 mean "Firmware Upgrade"/ "Firmware Upgrade Image" */
        {
            OAM_LOG(OAM,OAM_INFO,"Downloaded latest TAR file for software Upgradation");

            /*Integerity Check for Downloaded file */
            result = oam_check_file_integerity(file_name);

            if(OAM_SUCCESS == result)
            {
                /* Integrity check Successful */
                /* Send File download responce = Success to TR069 */
                if(OAM_FAILURE == oam_send_file_download_ind_resp(OAM_RESULT_SUCCESS,
                            OAM_ZERO,transaction_id,(UInt8 *)""))
                {
                    OAM_LOG(OAM,OAM_WARNING,"Error in sending File Download IND Response to TR069");
                    OAM_FUN_EXIT_TRACE();
                    return OAM_FAILURE;
                }

                file_name_length = oam_strlen((char *)file_name);

                p_temp_buf = (UInt8 *)oam_construct_sdm_start_upgrade_req(file_size,file_type,
                        file_des_len,file_name_length,file_name,file_des,&msg_len);
                g_upgrade_req_retry_counter= OAM_ZERO;
                if(OAM_NULL != p_temp_buf)
		        {
                    /*  coverity 94905 fix start */
			        retVal = oam_send_sdm_start_upgrade_req(p_temp_buf);

                    if(retVal == OAM_FAILURE)
                    {
                        OAM_LOG(OAM,OAM_ERROR,"Failure In Sending Message to SDM");
                        OAM_FUN_EXIT_TRACE();
                        return OAM_FAILURE;
                    }
		        }
                OAM_LOG(OAM,OAM_INFO,"Waiting For Shutdown ACK from SDM");
                OAM_FUN_EXIT_TRACE();
                return OAM_SUCCESS;

            }
            else
            {
                /* Send Error Response to TR069 */
                if(OAM_FAILURE == oam_send_file_download_ind_resp(OAM_RESULT_FAILURE
                            ,OAM_ERR_FILE_CORRUPTED,transaction_id,(UInt8 *)"Download failure:File Corrupted"))
                {
                    OAM_LOG(OAM,OAM_WARNING,"Error in sending Response to TR069");
                    OAM_FUN_EXIT_TRACE();
                    return OAM_FAILURE;

                }
				/*spr 9283 change start */
				OAM_FUN_EXIT_TRACE();
				return  OAM_FAILURE;
				/*spr 9283 change end  */
			}

		}
		else
		{

			fptr = oam_fopen((char *)file_name,"r");
            if (fptr == OAM_NULL)
            {
                OAM_LOG(OAM,OAM_WARNING,"Unable to access file. "
                    "Trying to enable by factory settings");

                /* Failure condition */
                if(OAM_FAILURE == oam_send_file_download_ind_resp(OAM_RESULT_FAILURE
                            ,OAM_ERR_FILE_ACCESS_FAILURE,transaction_id,(UInt8 *)"File Access Failure"))
                {
                    OAM_LOG(OAM,OAM_WARNING,"Error in sending Response");
                    OAM_FUN_EXIT_TRACE();
                    return OAM_FAILURE;

                }
                return OAM_FAILURE;
            }
            /* Able to access file */
			oam_fclose(fptr);
            xmlDocPtr doc_ptr = OAM_NULL;
            doc_ptr = xmlReadFile((char *)file_name, OAM_NULL,OAM_ZERO);

            if (doc_ptr == OAM_NULL)
            {
                OAM_LOG(OAM,OAM_WARNING,"Corrupted File Received. "
                    "Trying to enable by factory settings");
                if(OAM_FAILURE == oam_send_file_download_ind_resp(OAM_RESULT_FAILURE
                            ,OAM_ERR_FILE_CORRUPTED,transaction_id,(UInt8 *)"File Corrupted"))
               {
                    OAM_LOG(OAM,OAM_ERROR,"Error in sending Response");
                    OAM_FUN_EXIT_TRACE();
                    return OAM_FAILURE;

                }
            }
            else
            {
                /*SPR_FIXED_13038_START */
                tr069_flag = oam_prov_req.oam_tr069_req.tr069_mode;
                oam_memset(&oam_prov_req,0,sizeof(oam_prov_req_t));
                oam_prov_req.oam_tr069_req.tr069_mode = tr069_flag;
                /*SPR_FIXED_13038_END */

                UInt8 tr069_file_name[STR_LEN_128]={OAM_ZERO},cp_cmd[OAM_CMD_SIZE]={OAM_ZERO};
                xmlFreeDoc(doc_ptr);

                strncpy((char *)tr069_file_name,"../cfg/tr069.xml", sizeof(tr069_file_name));

                strncpy((char *)cp_cmd,"cp " , sizeof(cp_cmd) );

                oam_strncat((char *)cp_cmd,(char *)file_name, sizeof(cp_cmd) - OAM_ONE);

                oam_strncat((char *)cp_cmd," ", sizeof(cp_cmd ) - OAM_ONE ) ;

                oam_strncat((char *)cp_cmd,(char *)tr069_file_name, sizeof(cp_cmd) - OAM_ONE);

                oam_system((char *)cp_cmd);

                OAM_LOG(OAM,OAM_DETAILED,"Event marked as OAM_FILE_DOWNLOAD_RESP_PENDING");
                oam_prov_req.system_status.event_ongoing |= OAM_FILE_DOWNLOAD_RESP_PENDING;
                oam_prov_req.system_status.p_api_buf = p_msg;
                oam_prov_req.system_status.trans_id = transaction_id;
                UInt16 trans_id = get_word_from_header((UInt8*)(oam_prov_req.system_status.p_api_buf) + OAM_ZERO);
                OAM_LOG(OAM,OAM_INFO,"Transaction ID Saved: %d",trans_id);

                if(OAM_NULL != (oam_prov_req.system_status.timer_id = oam_start_new_timer(OAM_SUB_MODULE_MIH, OAM_MODULE_ID,
                                OAM_LAYERS_CONFIG_TIMER_TYPE, OAM_LAYERS_CONFIG_TIMEOUT_DURATION * OAM_THOUSAND,
                                OAM_NULL, OAM_ZERO, OAM_FALSE)))
                {
                    OAM_LOG(OAM,OAM_DETAILED,"Layers Config Timer started successfully");
                }
                else
                {
                    OAM_LOG(OAM,OAM_WARNING,"Failed to start Layers Config Timer");
                        /* SPR 15387 Start */
                    oam_prov_req.system_status.event_ongoing &=
                        ~OAM_FILE_DOWNLOAD_RESP_PENDING;
                    /* SPR 15387 End */

                   return OAM_FAILURE;
                }
            }
        }
    }

    retVal = oam_proceed_with_init_flow(g_no_of_arguments,
                                        g_list_of_init_arguments);

    if(OAM_FAILURE == retVal) {
        OAM_LOG(OAM, OAM_CRITICAL, "Failure in initialization.");
    }

    OAM_FUN_EXIT_TRACE();
    return retVal;
}

#ifdef OAM_SON_ENABLE
/****************************************************************************
*   FUNCTION NAME   : oam_generate_instance_id_for_inter_freq_carrier_list
*   DESCRIPTION     : This function will generate instance id for inter freq 
*                     carrier list
*   Inputs          : p_instance_id : Instance ID
*                     max_entries : Max Entries
*   Outputs         : NONE
*   RETURNS         : OAM_SUCCESS/OAM_FAILURE
****************************************************************************/
oam_return_et 
oam_generate_instance_id_for_inter_freq_carrier_list(UInt16 *p_instance_id,
                                                     UInt16  max_entries,
                                                     oam_error_code_et *p_err)
{
    OAM_FUN_ENTRY_TRACE();
    SInt8  i = OAM_ZERO;
    UInt8  start_idx = OAM_ZERO;
    UInt32 is_valid = OAM_ZERO;
    oam_son_inter_freq_carrier_t *p_cell_inter_freq_carr_params = OAM_NULL;

    /* COV CID: 64247 FIX START */
    if (!p_instance_id || !p_err) {
    /* COV CID: 64247 FIX END */
        OAM_LOG(OAM, OAM_WARNING,"NULL pointer error");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    *p_instance_id = OAM_ZERO;

    /* COV CID: 64246 FIX START */
    i = oam_get_son_inter_freq_carrier_idx(
                                oam_prov_req.cell_cntxt.curr_cell_cid);
    if(OAM_ERROR_RETURN_VAL == i) {
        OAM_LOG(OAM, OAM_WARNING, "Carrier data not found");
        *p_err = OAM_INVALID_CELL_CONTEXT;

        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    p_cell_inter_freq_carr_params = 
                &SON_PROV_REQ.oam_son_inter_freq_carrier_params[i];
    /* COV CID: 64246 FIX END */

    /* Exhuast list first, and then rollover to assign instance id */
    start_idx = p_cell_inter_freq_carr_params->eutran_last_inst_id;


    for (i = start_idx; i < max_entries && i < SON_MAX_CONFIG_OBJECT; ++i) {
        is_valid = p_cell_inter_freq_carr_params->
                                      eutran_meas_entry_bitmap & (1 << i);

        if (!is_valid) {
            *p_instance_id = i + 1;
            /* Save the last added instance id */
            p_cell_inter_freq_carr_params->eutran_last_inst_id =
                                                            *p_instance_id;
            OAM_FUN_EXIT_TRACE();
    	    return OAM_SUCCESS; 
        }
    }
    /* No instance available from last added till end, then search from start 
     * till last added */

    if(OAM_ZERO == *p_instance_id) {
        for (i = 0;
             i < p_cell_inter_freq_carr_params->eutran_last_inst_id;
             ++i) {
            is_valid = p_cell_inter_freq_carr_params->
                                    eutran_meas_entry_bitmap & (1 << i);

            if (!is_valid) {
                *p_instance_id = i + 1;
                /* Save the last added instance id */
                p_cell_inter_freq_carr_params->eutran_last_inst_id =
                                                            *p_instance_id;
                OAM_FUN_EXIT_TRACE();
                return OAM_SUCCESS;
            }
        }
    }

    *p_err = OAM_RESOURCE_EXCCEED;

    OAM_FUN_EXIT_TRACE();
    return OAM_FAILURE; 
}



/****************************************************************************
*   FUNCTION NAME   : oam_generate_instance_id_for_utran_fdd_freq_list
*   DESCRIPTION     : This function will generate instance id for inter freq 
*                     carrier list
*   Inputs          : p_instance_id : Instance ID
*                     max_entries : Max Entries
*   Outputs         : NONE
*   RETURNS         : OAM_SUCCESS/OAM_FAILURE
****************************************************************************/
oam_return_et 
oam_generate_instance_id_for_utran_fdd_freq_list(UInt16 *p_instance_id,
                                                 UInt16  max_entries,
                                                 oam_error_code_et *p_err)
{
    OAM_FUN_ENTRY_TRACE();
    SInt8  i = OAM_ZERO;
    UInt8  start_idx = OAM_ZERO;
    UInt32 is_valid = OAM_ZERO;
    oam_son_inter_freq_carrier_t *p_cell_inter_freq_carr_params = OAM_NULL;

    /* COV CID: 64247 FIX START */
    if (!p_instance_id || !p_err) {
    /* COV CID: 64247 FIX END */
        OAM_LOG(OAM, OAM_WARNING, "NULL pointer error");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    *p_instance_id = OAM_ZERO;

    /* COV CID: 64246 FIX START */
    i = oam_get_son_inter_freq_carrier_idx(
                                oam_prov_req.cell_cntxt.curr_cell_cid);
    if(OAM_ERROR_RETURN_VAL == i) {
        OAM_LOG(OAM, OAM_WARNING, "UTRANFDDFreq data not found");
        *p_err = OAM_INVALID_CELL_CONTEXT;

        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    p_cell_inter_freq_carr_params = 
                &SON_PROV_REQ.oam_son_inter_freq_carrier_params[i];
    /* COV CID: 64246 FIX END */

    /* Exhuast list first, and then rollover to assign instance id */
    start_idx = p_cell_inter_freq_carr_params->utran_fdd_last_inst_id;


    for (i = start_idx; i < max_entries && i < SON_MAX_CONFIG_OBJECT; ++i) {
        is_valid = p_cell_inter_freq_carr_params->
                                      utran_fdd_meas_entry_bitmap & (1 << i);

        if (!is_valid) {
            *p_instance_id = i + 1;
            /* Save the last added instance id */
            p_cell_inter_freq_carr_params->utran_fdd_last_inst_id =
                                                            *p_instance_id;
            OAM_FUN_EXIT_TRACE();
    	    return OAM_SUCCESS; 
        }
    }
    /* No instance available from last added till end, then search from start 
     * till last added */

    if(OAM_ZERO == *p_instance_id) {
        for (i = 0;
             i < p_cell_inter_freq_carr_params->utran_fdd_last_inst_id;
             ++i) {
            is_valid = p_cell_inter_freq_carr_params->
                                    utran_fdd_meas_entry_bitmap & (1 << i);

            if (!is_valid) {
                *p_instance_id = i + 1;
                /* Save the last added instance id */
                p_cell_inter_freq_carr_params->utran_fdd_last_inst_id =
                                                            *p_instance_id;
                OAM_FUN_EXIT_TRACE();
                return OAM_SUCCESS;
            }
        }
    }

    *p_err = OAM_RESOURCE_EXCCEED;

    OAM_FUN_EXIT_TRACE();
    return OAM_FAILURE; 
}
#endif

/******************************************************************************
*   FUNCTION NAME   : oam_generate_instance_id_for_plmn_list
*   DESCRIPTION     : This function will generate instance id for plmn list
*   Inputs          : p_instance_id : Instance ID
*                     max_entries : Max Entries
*   Outputs         : NONE
*   RETURNS         : OAM_SUCCESS/OAM_FAILURE
******************************************************************************/

oam_return_et
oam_generate_instance_id_for_plmn_list(UInt16 *p_instance_id,
                                       UInt16  max_entries,
                                       oam_error_code_et *p_err)
{
    OAM_FUN_ENTRY_TRACE();
    UInt8  i = OAM_ZERO;
    UInt8  start_idx = OAM_ZERO;
    oam_rrm_cell_plmnlist_data_t *p_cell_plmnlist_data = OAM_NULL;
    UInt32 is_valid = OAM_ZERO;

    /* COV CID: 64248 FIX START */
    if (!p_instance_id || !p_err) {
    /* COV CID: 64248 FIX END */
        OAM_LOG(OAM, OAM_WARNING, "NULL pointer error");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    *p_instance_id = OAM_ZERO;
    *p_err = NO_ERROR;

    for ( ; i < oam_prov_req.num_cells_configured; ++i) {
        if (RRM_PROV_REQ.plmnlist_data[i].cid ==
                    oam_prov_req.cell_cntxt.curr_cell_cid) {
            p_cell_plmnlist_data = &RRM_PROV_REQ.plmnlist_data[i];
            break;
        }
    }

    if (i == oam_prov_req.num_cells_configured) {
        OAM_LOG(OAM, OAM_WARNING, "PLMNList data not found");
        *p_err = OAM_INVALID_CELL_CONTEXT;

        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
    
    /* Exhuast list first, and then rollover to assign instance id */
    /* Coverity 64262 Start */
    if(OAM_NULL != p_cell_plmnlist_data)
    {
    start_idx = p_cell_plmnlist_data->last_added_instance_id;

    for (i = start_idx; i < max_entries && i < RRM_OAM_MAX_NUM_PLMNS; ++i) {
        is_valid = p_cell_plmnlist_data->is_valid_bitmap & (1 << i);

        if (!is_valid) {
            *p_instance_id = i + 1;
            /* Save the last added instance id */
            p_cell_plmnlist_data->last_added_instance_id = *p_instance_id;
            OAM_FUN_EXIT_TRACE();
            return OAM_SUCCESS;
        }
    }

    /* No instance available from last added till end, then search from start 
     * till last added */
    if (OAM_ZERO == *p_instance_id) {
        for (i = 0; i < p_cell_plmnlist_data->last_added_instance_id; ++i) {
            is_valid = p_cell_plmnlist_data->is_valid_bitmap & (1 << i);
            if (!is_valid) {
                *p_instance_id = i + 1;

                /* Save the last added instance id */
                p_cell_plmnlist_data->last_added_instance_id = *p_instance_id;

                OAM_FUN_EXIT_TRACE();
                return OAM_SUCCESS;
            }
        }
    }
  }
    /* Coverity 64262 End */

    *p_err = OAM_RESOURCE_EXCCEED;

    OAM_FUN_EXIT_TRACE();
    return OAM_FAILURE;
}
/* SPR 15527 START */                                                  
/******************************************************************************
*   FUNCTION NAME   : oam_generate_instance_id_for_qos_list
*   DESCRIPTION     : This function will generate instance id for plmn list
*   Inputs          : p_instance_id : Instance ID
*                     max_entries : Max Entries
*   Outputs         : NONE
*   RETURNS         : OAM_SUCCESS/OAM_FAILURE
******************************************************************************/
oam_return_et
oam_generate_instance_id_for_qos_list(UInt16 *p_instance_id,
                                       UInt16  max_entries,
                                       oam_error_code_et *p_err)
{
    OAM_FUN_ENTRY_TRACE();
    UInt8  i = OAM_ZERO,
           start_idx = OAM_ZERO;
   
    /* SPR 17777 fix */
    OAM_LOG(OAM, OAM_DETAILEDALL, "unused parameter %p", p_err);
    /* SPR 15527 START */
    SInt8 cell_config_idx = oam_get_rrm_cell_config_idx(oam_prov_req.cell_cntxt.curr_cell_cid);
    if(OAM_ERROR_RETURN_VAL == cell_config_idx) {
        OAM_LOG(OAM, OAM_ERROR, "Invalid cell config idx");
        *p_err = OAM_INVALID_CELL_CONTEXT;
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    /* Exhuast list first, and then rollover to assign instance id */
    start_idx = (oam_prov_req.oam_rrm_req.qos_data[cell_config_idx].last_added_qos_instance_id)%OAM_MAX_QOSLIST_OBJECTS;
    for(i = start_idx; i < max_entries && i < OAM_MAX_QOSLIST_OBJECTS; i++)  //31may
    {
        if(OAM_FALSE == oam_prov_req.oam_rrm_req.qos_data[cell_config_idx].qos_obj_list[i].is_valid_entry)
        {
            *p_instance_id = (i + OAM_ONE);// Return instance id as array index + 1
            /* Save the last added instance id */
            oam_prov_req.oam_rrm_req.qos_data[cell_config_idx].last_added_qos_instance_id = *p_instance_id;
            OAM_FUN_EXIT_TRACE();
            return OAM_SUCCESS;
        }
    }
    /* No instance aviable from last added till end, then search from start till last added */
    if(*p_instance_id == OAM_ZERO)
    {
        for(i = OAM_ZERO; i < oam_prov_req.oam_rrm_req.qos_data[cell_config_idx].last_added_qos_instance_id ; i++)  //31may
        {
            if(OAM_FALSE == oam_prov_req.oam_rrm_req.qos_data[cell_config_idx].qos_obj_list[i].is_valid_entry)
            {
                *p_instance_id = (i + OAM_ONE);// Return instance id as array index + 1
                /* Save the last added instance id */
                oam_prov_req.oam_rrm_req.qos_data[cell_config_idx].last_added_qos_instance_id = *p_instance_id;
               /* SPR 15527 END */                                                  
                OAM_FUN_EXIT_TRACE();
                return OAM_SUCCESS;
            }
        }
    }
    return OAM_FAILURE;
}
/* SPR 15527 END */                                                  


#ifdef OAM_SON_ENABLE
/****************************************************************************
 *  Function Name  : oam_generate_instance_id_for_NeighborList_LTECell
 *  Description    : This function is used to generate  the isntance id of 
 *                   Neighbour list 
 *  Inputs         : add_del_flag : add/delete flag
 *                   instance_id  : insatnce id of the object name
 *  Output         : None
 *  Returns        : return OAM_SUCCESS/OAM_FAILURE
 ***************************************************************************/
oam_return_et
oam_generate_instance_id_for_NeighborList_LTECell(UInt16 *p_instance_id,
                                                  UInt16  max_entries,
                                                  oam_error_code_et *p_err)
{
    OAM_FUN_ENTRY_TRACE();
    SInt8  i = OAM_ZERO;
    UInt8  start_idx = OAM_ZERO;
    UInt32 is_valid = OAM_ZERO;
    oam_cell_neighbourlist_info_t *p_cell_neighborlist_data = OAM_NULL;

    if (!p_instance_id || !p_err) {
        OAM_LOG(OAM, OAM_WARNING, "NULL pointer error");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    *p_instance_id = OAM_ZERO;

    /* COV CID: 64244 FIX START */
    i = oam_get_son_cell_nr_list_idx(oam_prov_req.cell_cntxt.curr_cell_cid);
    if(OAM_ERROR_RETURN_VAL == i) {
        OAM_LOG(OAM, OAM_WARNING, "Carrier data not found");
        *p_err = OAM_INVALID_CELL_CONTEXT;

        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    p_cell_neighborlist_data = &SON_PROV_REQ.cell_neighbourlist_info[i];
    /* COV CID: 64244 FIX END */


    /* Exhuast list first, and then rollover to assign instance id */
    start_idx = p_cell_neighborlist_data->intra_rat_last_added_instance_id;


    for (i = start_idx;
         i < max_entries && i < SON_MAX_NO_INTRA_RAT_NEIGHBOUR_CELLS; ++i) {
        is_valid =
            p_cell_neighborlist_data->intra_rat_nr_entry_bitmap & (1 << i);

        if (!is_valid) {
            *p_instance_id = i + 1;

            /* Save the last added instance id */
            p_cell_neighborlist_data->intra_rat_last_added_instance_id =
                                                            *p_instance_id;

            OAM_FUN_EXIT_TRACE();
            return OAM_SUCCESS;
        }
    }

    /* No instance available from last added till end, then search from start 
     * till last added */

    if(OAM_ZERO == *p_instance_id) {
        for (i = 0;
             i < p_cell_neighborlist_data->intra_rat_last_added_instance_id;
             ++i) {
            is_valid =
              p_cell_neighborlist_data->intra_rat_nr_entry_bitmap & (1 << i);

            if (!is_valid) {
                *p_instance_id = i + 1;

                /* Save the last added instance id */
                p_cell_neighborlist_data->intra_rat_last_added_instance_id =
                                                              *p_instance_id;

                OAM_FUN_EXIT_TRACE();
                return OAM_SUCCESS;
            }
        }
    }

    OAM_FUN_EXIT_TRACE();
    return OAM_FAILURE;
}


/******************************************************************************
 *  Function Name  : oam_generate_instance_id_for_NeighborList_UMTS
 *  Description    : This function is used to generate the instance id of umts 
 *  Inputs         : add_del_flag : add/delete flag
 *                   instance_id  : insatnce id of the object name
 *  Output         : None
 *  Returns        : return OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_generate_instance_id_for_NeighborList_UMTS(UInt16 *p_instance_id,
                                               UInt16  max_entries,
                                               oam_error_code_et *p_err)
{
    OAM_FUN_ENTRY_TRACE();
    SInt8  i = OAM_ZERO;
    UInt8  start_idx = OAM_ZERO;
    UInt32 is_valid = OAM_ZERO;
    oam_cell_neighbourlist_info_t *p_cell_neighborlist_data = OAM_NULL;

    if (!p_instance_id || !p_err) {
        OAM_LOG(OAM, OAM_WARNING, "NULL pointer error");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    *p_instance_id = OAM_ZERO;

    /* COV CID: 64245 FIX START */
    i = oam_get_son_cell_nr_list_idx(oam_prov_req.cell_cntxt.curr_cell_cid);
    if(OAM_ERROR_RETURN_VAL == i) {
        OAM_LOG(OAM, OAM_WARNING, "Carrier data not found");
        *p_err = OAM_INVALID_CELL_CONTEXT;

        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    p_cell_neighborlist_data = &SON_PROV_REQ.cell_neighbourlist_info[i];
    /* COV CID: 64245 FIX END */

    /* Exhuast list first, and then rollover to assign instance id */
    /* SPR-13586 START */
    start_idx = p_cell_neighborlist_data->inter_rat_umts_last_added_instance_id;


    for (i = start_idx;
            /*CID 81807 START*/
            i < max_entries && i < SON_MAX_NO_UMTS_NEIGHBOUR_CELLS; ++i) {
        /*CID 81807 END*/
        is_valid =
            p_cell_neighborlist_data->inter_rat_umts_entry_bitmap & (1 << i);

        if (!is_valid) {
            *p_instance_id = i + 1;

            /* Save the last added instance id */
            p_cell_neighborlist_data->inter_rat_umts_last_added_instance_id =
                                                            *p_instance_id;

            OAM_FUN_EXIT_TRACE();
            return OAM_SUCCESS;
        }
    }

    /* No instance available from last added till end, then search from start 
     * till last added */

    if(OAM_ZERO == *p_instance_id) {
        for (i = 0;
             i < p_cell_neighborlist_data->inter_rat_umts_last_added_instance_id;
             ++i) {
            is_valid =
              p_cell_neighborlist_data->inter_rat_umts_entry_bitmap & (1 << i);

            if (!is_valid) {
                *p_instance_id = i + 1;

                /* Save the last added instance id */
                p_cell_neighborlist_data->inter_rat_umts_last_added_instance_id =
                                                              *p_instance_id;
/* SPR-13586 END */

                OAM_FUN_EXIT_TRACE();
                return OAM_SUCCESS;
            }
        }
    }

    OAM_FUN_EXIT_TRACE();
    return OAM_FAILURE;
}

/* SPR-13586 START */
/******************************************************************************
 *  Function Name  : oam_generate_instance_id_for_NeighborList_GSM
 *  Description    : This function is used to generate the instance id of umts 
 *  Inputs         : add_del_flag : add/delete flag
 *                   instance_id  : insatnce id of the object name
 *  Output         : None
 *  Returns        : return OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_generate_instance_id_for_NeighborList_GSM(UInt16 *p_instance_id,
                                               UInt16  max_entries,
                                               oam_error_code_et *p_err)
{
    OAM_FUN_ENTRY_TRACE();
    SInt8  i = OAM_ZERO;
    UInt8  start_idx = OAM_ZERO;
    UInt32 is_valid = OAM_ZERO;
    oam_cell_neighbourlist_info_t *p_cell_neighborlist_data = OAM_NULL;

    if (!p_instance_id || !p_err) {
        OAM_LOG(OAM, OAM_WARNING, "NULL pointer error");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    *p_instance_id = OAM_ZERO;

    /* COV CID: 64245 FIX START */
    i = oam_get_son_cell_nr_list_idx(oam_prov_req.cell_cntxt.curr_cell_cid);
    if(OAM_ERROR_RETURN_VAL == i) {
        OAM_LOG(OAM, OAM_WARNING, "Carrier data not found");
        *p_err = OAM_INVALID_CELL_CONTEXT;

        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    p_cell_neighborlist_data = &SON_PROV_REQ.cell_neighbourlist_info[i];
    /* COV CID: 64245 FIX END */

    /* Exhuast list first, and then rollover to assign instance id */
    start_idx = p_cell_neighborlist_data->inter_rat_gsm_last_added_instance_id;


    for (i = start_idx;
       /*CID 81807 START*/
         i < max_entries && i < SON_MAX_NO_GERAN_NEIGHBOUR_CELLS; ++i) {
        /*CID 81807 END*/
        is_valid =
            p_cell_neighborlist_data->inter_rat_gsm_entry_bitmap & (1 << i);

        if (!is_valid) {
            *p_instance_id = i + 1;

            /* Save the last added instance id */
            p_cell_neighborlist_data->inter_rat_gsm_last_added_instance_id =
                                                            *p_instance_id;

            OAM_FUN_EXIT_TRACE();
            return OAM_SUCCESS;
        }
    }

    /* No instance available from last added till end, then search from start 
     * till last added */

    if(OAM_ZERO == *p_instance_id) {
        for (i = 0;
             i < p_cell_neighborlist_data->inter_rat_gsm_last_added_instance_id;
             ++i) {
            is_valid =
              p_cell_neighborlist_data->inter_rat_gsm_entry_bitmap & (1 << i);

            if (!is_valid) {
                *p_instance_id = i + 1;

                /* Save the last added instance id */
                p_cell_neighborlist_data->inter_rat_gsm_last_added_instance_id =
                                                              *p_instance_id;

                OAM_FUN_EXIT_TRACE();
                return OAM_SUCCESS;
            }
        }
    }

    OAM_FUN_EXIT_TRACE();
    return OAM_FAILURE;
}

/* SPR-13586 END */

/****************************************************************************
 * * Function Name  : oam_process_add_delete_NeighborList_LTECell
 * * Description    : This function is used to add or delete a LTECell node
 * *                  in xml tree
 * * Inputs         : add_del_flag : add/delete flag
 * *                  p_msg        : Pointer to message
 * *                  instance_id  : insatnce id of the object name
 * *                  pointer to interal error code
 * * Output         : None
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 ***************************************************************************/
oam_return_et
oam_process_add_delete_NeighborList_LTECell(UInt8 add_del_flag,
                                            void *p_msg,
                                            UInt16 instance_id,
                                            void* p_add_data,
                                            oam_error_code_et *p_err)
{
    OAM_FUN_ENTRY_TRACE();
    oam_return_et      ret_val = OAM_SUCCESS;
    UInt16 arr_idx = instance_id - 1;
    UInt8 cid = oam_prov_req.cell_cntxt.curr_cell_cid;
    UInt16 trans_id = OAM_ZERO;
    UInt16 dest_id = OAM_ZERO;
    UInt16 msg_len = OAM_ZERO;
    oam_son_nr_delete_req_t *p_req = OAM_NULL;
    oam_cell_neighbourlist_info_t *p_cell_neighborlist_info = OAM_NULL;
    oam_cell_neighbourlist_info_t *p_local_cell_neighborlist_info = OAM_NULL;
    oam_tcb_cntxt_data_t *p_tcb_data = p_add_data;
        /* SPR 18930  FIX START */
    oam_son_neigh_list_in_use_t * p_nrt_info = OAM_NULL;
    SInt16 cgi_found_in_nliu = -1;
    

    SInt8 cell_arr_idx = OAM_ERROR_RETURN_VAL; 
    SInt8 cell_nliu_idx = OAM_ERROR_RETURN_VAL; 
        /* SPR 18930  FIX END */

    if (!p_err) {
        OAM_LOG(OAM, OAM_WARNING, "NULL pointer error");

        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    *p_err = NO_ERROR;

    cell_arr_idx = oam_get_son_cell_nr_list_idx(cid);
        /* SPR 18930  FIX START */
	cell_nliu_idx = oam_get_son_neigh_list_in_use_idx(cid);

    if((OAM_ERROR_RETURN_VAL == cell_arr_idx )||
      (OAM_ERROR_RETURN_VAL == cell_nliu_idx )){
        OAM_LOG(OAM, OAM_WARNING, "Invalid cell context[cid=%d]", cid);
        *p_err = OAM_INVALID_CELL_CONTEXT;

        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    p_cell_neighborlist_info = &SON_NEIGHBORLIST_INFO(cell_arr_idx);
    p_local_cell_neighborlist_info =
                    &LOCAL_SON_NEIGHBORLIST_INFO(cell_arr_idx);


    p_nrt_info = &SON_PROV_REQ.neigh_list_in_use[cell_nliu_idx]; 

    trans_id = get_word_from_header((UInt8*)p_msg +
            OAM_INTF_HDR_TRANS_ID_OFFSET);
    dest_id = get_word_from_header((UInt8*)p_msg +
            OAM_INTF_HDR_SRC_ID_OFFSET);
        /* SPR 18930  FIX END */


    if(OAM_ADD_OPERATION == add_del_flag) {
        OAM_LOG(OAM, OAM_INFO, "Adding LTECell object with instance-id: %d",
                     instance_id);
        p_local_cell_neighborlist_info->intra_rat_nr_entry_bitmap =
                                                            (1 << arr_idx);
    }
    else if(OAM_DELETE_OPERATION == add_del_flag) {
        OAM_LOG(OAM, OAM_INFO,
                     "Deleting LTECell object with instance-id: %d",
                     instance_id);

        if(!(p_cell_neighborlist_info->intra_rat_nr_entry_bitmap &
                                    (1 << arr_idx))) {
            OAM_LOG(OAM, OAM_DETAILED,
                    "Instance not added at SON, sending success response");
            /* Send delete object response from here only */
            oam_construct_n_send_deleteobject_resp_to_usr(OAM_RESULT_SUCCESS,
                                                          OAM_ZERO, OAM_NULL,
                                                          OAM_ZERO,
                                                          OAM_VERSION_ID,
                                                          OAM_MODULE_ID,
                                                          dest_id, trans_id,
                                                          OAM_ZERO, 
                                                          OAM_ZERO,
                                                          p_err);

            oam_prov_req.system_status.event_ongoing &=
                                ~OAM_DELETE_OBJ_CELL_RECONFIG_ONGOING;

            OAM_FUN_EXIT_TRACE();
            return OAM_SUCCESS;
        }

        p_local_cell_neighborlist_info->nr_list_query_idx = arr_idx;
  
        /* SPR 18930  FIX START */
            /* Spr 15438 Fix Start */
        /* Checking if present in NLIU */
        cgi_found_in_nliu = oam_cgi_search_in_neigh_list(
                &p_cell_neighborlist_info->oam_son_nr_add_req.nr_list.
                 intra_rat_nr_list[instance_id - OAM_ONE].nbr_cgi,
                p_nrt_info->nrt_info.nr_list.intra_rat_nr_list,
                p_nrt_info->nrt_info.nr_list.intra_rat_nr_list_size,
                &p_nrt_info->intra_rat_neigh_valid_bitmap,
                OAM_SON_NEIGH_TYPE_INTRA_RAT);

        if ((cgi_found_in_nliu == NOT_FOUND) || 
            ((cgi_found_in_nliu != NOT_FOUND)&&
            (p_nrt_info->is_detected_intra_neighbour[cgi_found_in_nliu] == OAM_TRUE)))
        {
            p_cell_neighborlist_info->intra_rat_nr_entry_bitmap &= 
                ~(OAM_ONE << (instance_id -1));


        /* SPR 18930  FIX START */
		p_cell_neighborlist_info->oam_son_nr_add_req.nr_list.intra_rat_nr_list_size--;	
        /* SPR 18930  FIX End */

            OAM_LOG(OAM,OAM_INFO,"Either Instance not added at SON Or Neighbour Is Detected By Son"
                    " send success delete response");

            oam_construct_n_send_deleteobject_resp_to_usr(OAM_RESULT_SUCCESS,
                    OAM_ZERO, OAM_NULL,
                    OAM_ZERO,
                    OAM_VERSION_ID,
                    OAM_MODULE_ID,
                    dest_id, trans_id,
                    OAM_ZERO, 
                    OAM_ZERO,
                    p_err);
            return OAM_SUCCESS;
        }
    /* Spr 15438 Fix End*/
 
        /* SPR 18930  FIX END */

        ret_val = oam_mem_alloc(sizeof(oam_son_nr_delete_req_t),
                                (void *)&p_req, p_err);

        if(OAM_FAILURE == ret_val) {
            OAM_LOG(OAM, OAM_ERROR,
                    "Memory allocation to p_req failed"
                    " with Error Code:%d",
                    *p_err);

            OAM_FUN_EXIT_TRACE();
            return ret_val;
        }
/* Coverity ID 64919 Fix Start*/
       oam_memset(p_req, 0,
                   sizeof(oam_son_nr_delete_req_t));
/* Coverity ID 64919 Fix End*/
        oam_memcpy(&p_req->src_cgi,
                   &(p_cell_neighborlist_info->oam_son_nr_add_req.src_cgi),
                   sizeof(p_req->src_cgi));

        p_req->trgt_cgi_list_size = OAM_ONE;

        p_req->trgt_cgi_list[OAM_ZERO].bitmask |=
                                        SON_INTRA_RAT_GLOBAL_CELL_ID_PRESENT;

        oam_memcpy(&p_req->trgt_cgi_list[0].intra_rat_global_cell_id,
                   &p_cell_neighborlist_info->oam_son_nr_add_req.nr_list.
                   intra_rat_nr_list[arr_idx].nbr_cgi,
                   sizeof(son_intra_rat_global_cell_id_t));
/* Start: SPR 10567 */
/* SPR 13176 FIX START */
        oam_memcpy(&LOCAL_NRT_INFO(cell_arr_idx).src_cgi,
                    &(p_cell_neighborlist_info->oam_son_nr_add_req.src_cgi),
                    sizeof(son_intra_rat_global_cell_id_t));
        LOCAL_NRT_INFO(cell_arr_idx).nrt_op = SON_NRT_OP_DELETE;
        LOCAL_NRT_INFO(cell_arr_idx).nr_list.intra_rat_nr_list_size = OAM_ONE;
        oam_memcpy(&LOCAL_NRT_INFO(cell_arr_idx).nr_list.intra_rat_nr_list[OAM_ZERO].nbr_cgi,
                      &p_cell_neighborlist_info->oam_son_nr_add_req.nr_list.
                       intra_rat_nr_list[arr_idx].nbr_cgi,
                       sizeof(son_intra_rat_global_cell_id_t));
/* SPR 13176 FIX END */
/*End: SPR 10567 */
        oam_prov_req.oam_son_req.nr_action = OAM_NR_ACTION_DELETE_EUTRAN;

        trans_id = oam_get_new_trans_id();
        ret_val = oam_son_send_son_oam_nr_delete_req(p_req, OAM_MODULE_ID,
                                                     SON_MODULE_ID,
                                                     trans_id, cid);

        if (OAM_FAILURE == ret_val) {
            OAM_LOG(OAM, OAM_ERROR, "Failed to send request to lower layer");
            *p_err = OAM_SEND_FAIL;
        }
        else {
            msg_len = get_word_from_header((UInt8*)p_msg +
                                            OAM_INTF_HDR_MSG_SIZE_OFFSET);

#ifndef OAM_UT_FRAMEWORK
            oam_maintain_tcb_ext(trans_id,
                                 OAM_TRANS_SON, msg_len,
                                 p_msg,
                                 p_tcb_data->p_orig_xml_parent,
                                 p_tcb_data->p_usr_msg_xml_parent,
                                 p_tcb_data->p_parent_xml_struct_addr);
#endif
        /* SPR 18930  FIX START */
            oam_prov_req.system_status.event_ongoing |=
                                OAM_DELETE_OBJ_CELL_RECONFIG_ONGOING;
        /* SPR 18930  FIX END */
        }

        oam_mem_free(p_req, p_err);
    }

    OAM_FUN_EXIT_TRACE();
    return ret_val;
}

/******************************************************************************
 *  Function Name  : oam_process_add_delete_NeighborList_InterRATCell_UMTS
 *  Description    : This function is used to indicate the existance of Neighbour list 
 *  Inputs         : add_del_flag : add/delete flag
 *                   instance_id  : insatnce id of the object name
 *  Output         : None
 *  Returns        : return OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_process_add_delete_NeighborList_InterRATCell_UMTS(UInt8 add_del_flag,
                                                    void *p_msg,
                                                    UInt16 instance_id,
                                                    void* p_add_data,
                                                    oam_error_code_et *p_err)
{
    OAM_FUN_ENTRY_TRACE();
    oam_return_et      ret_val = OAM_SUCCESS;
    UInt16 arr_idx = instance_id - 1;
    UInt8 cid = oam_prov_req.cell_cntxt.curr_cell_cid;
    UInt16 trans_id = OAM_ZERO;
    UInt16 dest_id = OAM_ZERO;
    UInt16 msg_len = OAM_ZERO;
    oam_son_nr_delete_req_t *p_req = OAM_NULL;
    oam_cell_neighbourlist_info_t *p_cell_neighborlist_info = OAM_NULL;
    oam_cell_neighbourlist_info_t *p_local_cell_neighborlist_info = OAM_NULL;
    oam_tcb_cntxt_data_t *p_tcb_data = p_add_data;
        /* SPR 18930  FIX START */
    SInt16 cgi_found_in_nliu = -1;
    SInt8 cell_arr_idx = OAM_ERROR_RETURN_VAL; 
    SInt8 cell_nliu_idx = OAM_ERROR_RETURN_VAL; 
    oam_son_neigh_list_in_use_t * p_nrt_info = OAM_NULL;
        /* SPR 18930  FIX END */

    if (!p_err) {
        OAM_LOG(OAM, OAM_WARNING, "NULL pointer error");

        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    *p_err = NO_ERROR;

        /* SPR 18930  FIX START */
    cell_arr_idx = oam_get_son_cell_nr_list_idx(cid);
	cell_nliu_idx = oam_get_son_neigh_list_in_use_idx(cid);

    if((OAM_ERROR_RETURN_VAL == cell_arr_idx )||
      (OAM_ERROR_RETURN_VAL == cell_nliu_idx )){
        OAM_LOG(OAM, OAM_WARNING, "Invalid cell context[cid=%d]", cid);
        *p_err = OAM_INVALID_CELL_CONTEXT;

        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }


    p_cell_neighborlist_info = &SON_NEIGHBORLIST_INFO(cell_arr_idx);
    p_local_cell_neighborlist_info =
        &LOCAL_SON_NEIGHBORLIST_INFO(cell_arr_idx);


    p_nrt_info = &SON_PROV_REQ.neigh_list_in_use[cell_nliu_idx]; 

        /* SPR 18930  FIX END */
    if(OAM_ADD_OPERATION == add_del_flag) {
        OAM_LOG(OAM, OAM_INFO, "Adding UMTS object with instance-id: %d",
                instance_id);
        /* SPR-13586 START */
        p_local_cell_neighborlist_info->inter_rat_umts_entry_bitmap =
            (1 << arr_idx);
    }
    else if(OAM_DELETE_OPERATION == add_del_flag) {
        OAM_LOG(OAM, OAM_INFO,
                "Deleting UMTS object with instance-id: %d",
                instance_id);

        if(!(p_cell_neighborlist_info->inter_rat_umts_entry_bitmap &
                    (1 << arr_idx))) {
            if(!(SON_PROV_REQ.inter_rat_umts_daho_bitmask & (1 << arr_idx)))
            {
                trans_id = get_word_from_header(p_msg +
                        OAM_INTF_HDR_TRANS_ID_OFFSET);
                dest_id = get_word_from_header(p_msg +
                        OAM_INTF_HDR_SRC_ID_OFFSET);

                OAM_LOG(OAM, OAM_DETAILED,
                        "Instance not added at SON, sending success response");
                /* Send delete object response from here only */
                oam_construct_n_send_deleteobject_resp_to_usr(OAM_RESULT_SUCCESS,
                        OAM_ZERO, OAM_NULL,
                        OAM_ZERO,
                        OAM_VERSION_ID,
                        OAM_MODULE_ID,
                        dest_id, trans_id,
                        OAM_ZERO, 
                        OAM_ZERO,
                        p_err);

                oam_prov_req.system_status.event_ongoing &=
                    ~OAM_DELETE_OBJ_CELL_RECONFIG_ONGOING;

                OAM_FUN_EXIT_TRACE();
                return OAM_SUCCESS;
            }
            else {
                *p_err = OAM_PARAMETER_NOT_FOUND;
                return OAM_FAILURE;
            }
            /* SPR-13586 END */
        }


        p_local_cell_neighborlist_info->nr_list_query_idx = arr_idx;


        /* SPR 18930  FIX START */
        cgi_found_in_nliu = oam_cgi_search_in_neigh_list(
                &p_cell_neighborlist_info->oam_son_nr_add_req.nr_list.
                inter_rat_nr_list[arr_idx].utran_neighbor.nbr_cgi,
                p_nrt_info->nrt_info.nr_list.inter_rat_nr_list,
                p_nrt_info->nrt_info.nr_list.inter_rat_nr_list_size,
                &p_nrt_info->inter_rat_umts_neigh_valid_bitmap,
                OAM_SON_NEIGH_TYPE_INTER_RAT_UTRAN);

        if ((cgi_found_in_nliu == NOT_FOUND) || 
            ((cgi_found_in_nliu != NOT_FOUND)&&
            (p_nrt_info->is_detected_utran_neighbour[cgi_found_in_nliu] == OAM_TRUE)))
        {
            trans_id = get_word_from_header(p_msg +
                        OAM_INTF_HDR_TRANS_ID_OFFSET);
                dest_id = get_word_from_header(p_msg +
                        OAM_INTF_HDR_SRC_ID_OFFSET);
            p_cell_neighborlist_info->inter_rat_umts_entry_bitmap &= 
               ~(OAM_ONE << (instance_id -1));

            OAM_LOG(OAM,OAM_INFO,"Either Instance not added at SON Or Neighbour Is Detected By Son"
                    " send success delete response");

                oam_prov_req.system_status.event_ongoing &=
                    ~OAM_DELETE_OBJ_CELL_RECONFIG_ONGOING;
            oam_construct_n_send_deleteobject_resp_to_usr(OAM_RESULT_SUCCESS,
                    OAM_ZERO, OAM_NULL,
                    OAM_ZERO,
                    OAM_VERSION_ID,
                    OAM_MODULE_ID,
                    dest_id, trans_id,
                    OAM_ZERO, 
                    OAM_ZERO,
                    p_err);
            return OAM_SUCCESS;
        }
        /* SPR 18930  FIX END */
        ret_val = oam_mem_alloc(sizeof(oam_son_nr_delete_req_t),
                (void *)&p_req, p_err);

        if(OAM_FAILURE == ret_val) {
            OAM_LOG(OAM, OAM_ERROR,
                    "Memory allocation to p_req failed"
                    " with Error Code:%d",
                    *p_err);

            OAM_FUN_EXIT_TRACE();
            return ret_val;
        }

        /* Coverity Fix 64918 start */
        oam_memset(p_req, OAM_ZERO,sizeof(oam_son_nr_delete_req_t));
        /*Coverity Fix 64918 end */

        oam_memcpy(&p_req->src_cgi,
                &(p_cell_neighborlist_info->oam_son_nr_add_req.src_cgi),
                sizeof(p_req->src_cgi));

        p_req->trgt_cgi_list_size = OAM_ONE;

        p_req->trgt_cgi_list[OAM_ZERO].bitmask |=
            SON_INTER_RAT_GLOBAL_CELL_ID_PRESENT;

        p_req->trgt_cgi_list[OAM_ZERO].inter_rat_global_cell_id.
            bitmask |= SON_UTRAN_GERAN_GLOBAL_CELL_ID_PRESENT;

        /* Start: SPR 10567 */
        /* SPR 13176 FIX START */
        oam_memcpy(&LOCAL_NRT_INFO(cell_arr_idx).src_cgi,
                &(p_cell_neighborlist_info->oam_son_nr_add_req.src_cgi),
                sizeof(son_intra_rat_global_cell_id_t));
        LOCAL_NRT_INFO(cell_arr_idx).nrt_op = SON_NRT_OP_DELETE;
        LOCAL_NRT_INFO(cell_arr_idx).nr_list.inter_rat_nr_list_size = OAM_ONE;
        LOCAL_NRT_INFO(cell_arr_idx).nr_list.inter_rat_nr_list[OAM_ZERO].bitmask |= SON_INTER_UTRAN_NEIGHBOR;
        oam_memcpy(&LOCAL_NRT_INFO(cell_arr_idx).nr_list.inter_rat_nr_list[OAM_ZERO].utran_neighbor.nbr_cgi,
                &p_cell_neighborlist_info->oam_son_nr_add_req.nr_list.inter_rat_nr_list[arr_idx].utran_neighbor.nbr_cgi,
                sizeof(son_utran_geran_global_cell_id_t));

        /* SPR 13176 FIX END */
        /*End: SPR 10567 */
        p_req->trgt_cgi_list[OAM_ZERO].inter_rat_global_cell_id.
            utran_geran_global_cell_id.rat_type = SON_UTRAN_CELL;

        oam_memcpy(&p_req->trgt_cgi_list[0].inter_rat_global_cell_id.
                utran_geran_global_cell_id,
                &p_cell_neighborlist_info->oam_son_nr_add_req.nr_list.
                inter_rat_nr_list[arr_idx].utran_neighbor.nbr_cgi,
                sizeof(son_utran_geran_global_cell_id_t));

        SON_PROV_REQ.nr_action = OAM_NR_ACTION_DELETE_UTRAN;

        trans_id = oam_get_new_trans_id();
        ret_val = oam_son_send_son_oam_nr_delete_req(p_req, OAM_MODULE_ID,
                SON_MODULE_ID,
                trans_id, cid);

        if (OAM_FAILURE == ret_val) {
            OAM_LOG(OAM, OAM_ERROR, "Failed to send request to lower layer");
            *p_err = OAM_SEND_FAIL;
        }
        else {
            msg_len = get_word_from_header((UInt8*)p_msg +
                    OAM_INTF_HDR_MSG_SIZE_OFFSET);

#ifndef OAM_UT_FRAMEWORK
            oam_maintain_tcb_ext(trans_id,
                    OAM_TRANS_SON, msg_len,
                    p_msg,
                    p_tcb_data->p_orig_xml_parent,
                    p_tcb_data->p_usr_msg_xml_parent,
                    p_tcb_data->p_parent_xml_struct_addr);
#endif
        }

        oam_mem_free(p_req, p_err);
    }

    OAM_FUN_EXIT_TRACE();
    return ret_val;
}

/* SPR-13586 START */
/******************************************************************************
 *  Function Name  : oam_process_add_delete_NeighborList_InterRATCell_GSM
 *  Description    : This function is used to indicate the existance of Neighbour list 
 *  Inputs         : add_del_flag : add/delete flag
 *                   instance_id  : insatnce id of the object name
 *  Output         : None
 *  Returns        : return OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_process_add_delete_NeighborList_InterRATCell_GSM(UInt8 add_del_flag,
                                                    void *p_msg,
                                                    UInt16 instance_id,
                                                    void* p_add_data,
                                                    oam_error_code_et *p_err)
{
    OAM_FUN_ENTRY_TRACE();
    oam_return_et      ret_val = OAM_SUCCESS;
    UInt16 arr_idx = instance_id - 1;
    UInt8 cid = oam_prov_req.cell_cntxt.curr_cell_cid;
    UInt16 trans_id = OAM_ZERO;
    UInt16 dest_id = OAM_ZERO;
    UInt16 msg_len = OAM_ZERO;
    oam_son_nr_delete_req_t *p_req = OAM_NULL;
    oam_cell_neighbourlist_info_t *p_cell_neighborlist_info = OAM_NULL;
    oam_cell_neighbourlist_info_t *p_local_cell_neighborlist_info = OAM_NULL;
    oam_tcb_cntxt_data_t *p_tcb_data = p_add_data;

    SInt8 cell_arr_idx = OAM_ERROR_RETURN_VAL; 

    if (!p_err) {
        OAM_LOG(OAM, OAM_WARNING, "NULL pointer error");

        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    *p_err = NO_ERROR;

    cell_arr_idx = oam_get_son_cell_nr_list_idx(cid);

    if(OAM_ERROR_RETURN_VAL == cell_arr_idx ){
        OAM_LOG(OAM, OAM_WARNING, "Invalid cell context[cid=%d]", cid);
        *p_err = OAM_INVALID_CELL_CONTEXT;

        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
    

    p_cell_neighborlist_info = &SON_NEIGHBORLIST_INFO(cell_arr_idx);
    p_local_cell_neighborlist_info =
                    &LOCAL_SON_NEIGHBORLIST_INFO(cell_arr_idx);



    if(OAM_ADD_OPERATION == add_del_flag) {
        OAM_LOG(OAM, OAM_INFO, "Adding GSM object with instance-id: %d",
                     instance_id);
        p_local_cell_neighborlist_info->inter_rat_gsm_entry_bitmap =
                                                            (1 << arr_idx);
    }
    else if(OAM_DELETE_OPERATION == add_del_flag) {
        OAM_LOG(OAM, OAM_INFO,
                     "Deleting GSM object with instance-id: %d",
                     instance_id);

        if(!(p_cell_neighborlist_info->inter_rat_gsm_entry_bitmap &
                                    (1 << arr_idx))) {
            if(!(SON_PROV_REQ.inter_rat_gsm_daho_bitmask & (1 << arr_idx)))
            {    
            trans_id = get_word_from_header(p_msg +
                                       OAM_INTF_HDR_TRANS_ID_OFFSET);
            dest_id = get_word_from_header(p_msg +
                                         OAM_INTF_HDR_SRC_ID_OFFSET);

            OAM_LOG(OAM, OAM_DETAILED,
                    "Instance not added at SON, sending success response");
            /* Send delete object response from here only */
            oam_construct_n_send_deleteobject_resp_to_usr(OAM_RESULT_SUCCESS,
                                                          OAM_ZERO, OAM_NULL,
                                                          OAM_ZERO,
                                                          OAM_VERSION_ID,
                                                          OAM_MODULE_ID,
                                                          dest_id, trans_id,
                                                          OAM_ZERO, 
                                                          OAM_ZERO,
                                                          p_err);

            oam_prov_req.system_status.event_ongoing &=
                                ~OAM_DELETE_OBJ_CELL_RECONFIG_ONGOING;

            OAM_FUN_EXIT_TRACE();
            return OAM_SUCCESS;
            }
            else {
                *p_err = OAM_PARAMETER_NOT_FOUND;
                return OAM_FAILURE;
            }
        }


        p_local_cell_neighborlist_info->nr_list_query_idx = arr_idx;


        ret_val = oam_mem_alloc(sizeof(oam_son_nr_delete_req_t),
                                (void *)&p_req, p_err);

        if(OAM_FAILURE == ret_val) {
            OAM_LOG(OAM, OAM_ERROR,
                    "Memory allocation to p_req failed"
                    " with Error Code:%d",
                    *p_err);

            OAM_FUN_EXIT_TRACE();
            return ret_val;
        }
        /* Coverity 85870 Fix Start*/
        oam_memset(p_req, 0,
                   sizeof(oam_son_nr_delete_req_t));
        /*Coverity 85870 Fix End*/
        oam_memcpy(&p_req->src_cgi,
                   &(p_cell_neighborlist_info->oam_son_nr_add_req.src_cgi),
                   sizeof(p_req->src_cgi));

        p_req->trgt_cgi_list_size = OAM_ONE;

        p_req->trgt_cgi_list[OAM_ZERO].bitmask |=
                            SON_INTER_RAT_GLOBAL_CELL_ID_PRESENT;

        p_req->trgt_cgi_list[OAM_ZERO].inter_rat_global_cell_id.
            bitmask |= SON_UTRAN_GERAN_GLOBAL_CELL_ID_PRESENT;

/* Start: SPR 10567 */
/* SPR 13176 FIX START */
        oam_memcpy(&LOCAL_NRT_INFO(cell_arr_idx).src_cgi,
                    &(p_cell_neighborlist_info->oam_son_nr_add_req.src_cgi),
                    sizeof(son_intra_rat_global_cell_id_t));
        LOCAL_NRT_INFO(cell_arr_idx).nrt_op = SON_NRT_OP_DELETE;
        LOCAL_NRT_INFO(cell_arr_idx).nr_list.inter_rat_nr_list_size = OAM_ONE;
        LOCAL_NRT_INFO(cell_arr_idx).nr_list.inter_rat_nr_list[OAM_ZERO].bitmask |= SON_INTER_GERAN_NEIGHBOR;
        oam_memcpy(&LOCAL_NRT_INFO(cell_arr_idx).nr_list.inter_rat_nr_list[OAM_ZERO].geran_neighbor.nbr_cgi,
                      &p_cell_neighborlist_info->oam_son_nr_add_req.nr_list.inter_rat_nr_list[arr_idx].geran_neighbor.nbr_cgi,
                       sizeof(son_utran_geran_global_cell_id_t));

/* SPR 13176 FIX END */
/*End: SPR 10567 */
        p_req->trgt_cgi_list[OAM_ZERO].inter_rat_global_cell_id.
            utran_geran_global_cell_id.rat_type = SON_GERAN_CELL;

        oam_memcpy(&p_req->trgt_cgi_list[0].inter_rat_global_cell_id.
                   utran_geran_global_cell_id,
                   &p_cell_neighborlist_info->oam_son_nr_add_req.nr_list.
                   inter_rat_nr_list[arr_idx].geran_neighbor.nbr_cgi,
                   sizeof(son_utran_geran_global_cell_id_t));

        SON_PROV_REQ.nr_action = OAM_NR_ACTION_DELETE_GERAN;

        trans_id = oam_get_new_trans_id();
        ret_val = oam_son_send_son_oam_nr_delete_req(p_req, OAM_MODULE_ID,
                                                     SON_MODULE_ID,
                                                     trans_id, cid);

        if (OAM_FAILURE == ret_val) {
            OAM_LOG(OAM, OAM_ERROR, "Failed to send request to lower layer");
            *p_err = OAM_SEND_FAIL;
        }
        else {
            msg_len = get_word_from_header((UInt8*)p_msg +
                                            OAM_INTF_HDR_MSG_SIZE_OFFSET);

#ifndef OAM_UT_FRAMEWORK
            oam_maintain_tcb_ext(trans_id,
                                 OAM_TRANS_SON, msg_len,
                                 p_msg,
                                 p_tcb_data->p_orig_xml_parent,
                                 p_tcb_data->p_usr_msg_xml_parent,
                                 p_tcb_data->p_parent_xml_struct_addr);
#endif
        }

        oam_mem_free(p_req, p_err);
    }
 
    OAM_FUN_EXIT_TRACE();
    return ret_val;
}
/* SPR-13586 END */



/****************************************************************************
 *  Function Name  : oam_interfreq_carrier_data_accessor
 *  Description    : This function returns pointers to current number of
 *                   carrier, max number of carrier, pointer to parent of
 *                   Carrier node, and pointer to first sibling of Carrier
 *                   node
 *  Inputs         : NONE

 *  Output         : 1. p_curr_entries, (Optional, use OAM_NULL if not
 *                                      required)
 *                   2. p_max_entries, (Optional, use OAM_NULL if not
 *                                      required)
 *                   3. obj_parent (Optional, use OAM_NULL if not required),
 *                   4. obj_first_sibling (Optional, use OAM_NULL if not
 *                                         required)
 *  Returns        : return OAM_SUCCESS/OAM_FAILURE
 ***************************************************************************/
        /* SPR 18930  FIX START */
oam_return_et
oam_interfreq_carrier_data_accessor(UInt16 **p_curr_entries,
                                    UInt16 *p_max_entries,
                                    xmlNode **obj_parent,
                                    xmlNode **obj_first_sibling,
/* SPR 16603 Fix Start */
                                    xmlNode *** obj_struct_parent,
                                    UInt8 *cell_idx, 
/* SPR 16603 Fix End */
                                    oam_error_code_et *p_err)
{
    UInt8 cid = oam_prov_req.cell_cntxt.curr_cell_cid;
    UInt8 idx = OAM_ZERO;
    oam_return_et retVal = OAM_SUCCESS;
    XML_struct_cell_config *p_cellconfig = OAM_NULL;

    OAM_FUN_ENTRY_TRACE();

    if (p_curr_entries || p_max_entries) {
        for ( ; idx < oam_prov_req.num_cells_configured; ++idx) {
            if (cid == SON_INTER_FREQ_CARR_PARAMS(idx).cid) {
/* SPR 16603 Fix Start */
                *cell_idx = idx;
/* SPR 16603 Fix End */
                if (p_curr_entries) {
                    *p_curr_entries = (UInt16 *)&SON_ANR_MEAS_CONFIG_REQ(idx).
                                            eutran_config_list_size;
                }

                if (p_max_entries) {
                    *p_max_entries = SON_INTER_FREQ_CARR_PARAMS(idx).
                                          MaxCarrierEntries;
                }
                break;
            }
        }
    }

    if (idx == oam_prov_req.num_cells_configured) {
        OAM_LOG(OAM, OAM_WARNING, "Carrier data not found");
        if (p_err) {
            *p_err = OAM_INVALID_CELL_CONTEXT;
        }

        retVal = OAM_FAILURE;
    }
    else if (obj_parent || obj_first_sibling) {
        p_cellconfig = oam_get_xml_cellconfig_node(cid);

        if (p_cellconfig) {
            if (obj_parent) {
                *obj_parent = p_cellconfig->InterFreq;
            }
            if (obj_first_sibling) {
                *obj_first_sibling = oam_get_xml_leaf_child_node(
                                                   p_cellconfig->InterFreq,
                                                   "CarrierNumberOfEntries");
            }
/* SPR 16603 Fix Start */
            if (obj_struct_parent) {
                *obj_struct_parent = &p_cellconfig->Carrier[0];
            }
/* SPR 16603 Fix End */
        }
        else {
            if (p_err) {
                *p_err = OAM_INVALID_CELL_CONTEXT;
            }

            retVal = OAM_FAILURE;
        }
    }

    OAM_FUN_EXIT_TRACE();
    return retVal;
}
        /* SPR 18930  FIX END */

/****************************************************************************
 *  Function Name  : oam_utran_fdd_freq_data_accessor
 *  Description    : This function returns pointers to current number of
 *                  UTRANFDDFreq objects, max number of UTRANFDDFreq objects,
                    pointer to parent of
 *                  UTRANFDDFreq object node, and pointer to first sibling of
                     UTRANFDDFreq object node
 *  Inputs         : NONE

 *  Output         : 1. p_curr_entries, (Optional, use OAM_NULL if not
 *                                      required)
 *                   2. p_max_entries, (Optional, use OAM_NULL if not
 *                                      required)
 *                   3. obj_parent (Optional, use OAM_NULL if not required),
 *                   4. obj_first_sibling (Optional, use OAM_NULL if not
 *                                         required)
 *  Returns        : return OAM_SUCCESS/OAM_FAILURE
 ***************************************************************************/
        /* SPR 18930  FIX START */
oam_return_et
oam_utran_fdd_freq_data_accessor(UInt16 **p_curr_entries,
                                 UInt16 *p_max_entries,
                                 xmlNode **obj_parent,
                                 xmlNode **obj_first_sibling,
/* SPR 16603 Fix Start */
                                 xmlNode *** obj_struct_parent,
                                 UInt8 *cell_idx, 
/* SPR 16603 Fix End */
                                 oam_error_code_et *p_err)
{
    UInt8 cid = oam_prov_req.cell_cntxt.curr_cell_cid;
    UInt8 idx = OAM_ZERO;
    oam_return_et retVal = OAM_SUCCESS;
    XML_struct_cell_config *p_cellconfig = OAM_NULL;

    OAM_FUN_ENTRY_TRACE();

    if (p_curr_entries || p_max_entries) {
        for ( ; idx < oam_prov_req.num_cells_configured; ++idx) {
            if (cid == SON_INTER_FREQ_CARR_PARAMS(idx).cid) {
/* SPR 16603 Fix Start */
                *cell_idx = idx;
/* SPR 16603 Fix End */
                if (p_curr_entries) {
                    *p_curr_entries = (UInt16 *)&SON_ANR_MEAS_CONFIG_REQ(idx).
                                       other_rat_config_list.
                                       utran_config_list.utran_fdd_list_size;
                }

                if (p_max_entries) {
                    *p_max_entries = SON_INTER_FREQ_CARR_PARAMS(idx).
                                          max_utran_fdd_freq;
                }
                break;
            }
        }
    }

    if (idx == oam_prov_req.num_cells_configured) {
        OAM_LOG(OAM, OAM_WARNING, "UTRANFDDFreq data not found");
        if (p_err) {
            *p_err = OAM_INVALID_CELL_CONTEXT;
        }

        retVal = OAM_FAILURE;
    }
    else if (obj_parent || obj_first_sibling) {
        p_cellconfig = oam_get_xml_cellconfig_node(cid);

        if (p_cellconfig) {
            if (obj_parent) {
                *obj_parent = p_cellconfig->UTRA;
            }
            if (obj_first_sibling) {
                *obj_first_sibling =
                        oam_get_xml_leaf_child_node(p_cellconfig->UTRA,
                                              "UTRANFDDFreqNumberOfEntries");
            }
/* SPR 16603 Fix Start */
            if (obj_struct_parent) {
                *obj_struct_parent = &p_cellconfig->UTRANFDDFreq[0];
            }
/* SPR 16603 Fix End */
        }
        else {
            if (p_err) {
                *p_err = OAM_INVALID_CELL_CONTEXT;
            }

            retVal = OAM_FAILURE;
        }
    }

    OAM_FUN_EXIT_TRACE();
    return retVal;
}
/* SPR 18930  FIX END */

/****************************************************************************
 *  Function Name  : oam_neighborlist_ltecell_data_accessor
 *  Description    : This function returns pointers to current number of
 *                   ltecell neighbor, max number of ltecell neighbor,
 *                   pointer to parent of
 *                   LTECell node, and pointer to first sibling of LTECell
 *                   node
 *  Inputs         : NONE

 *  Output         : 1. p_curr_entries, (Optional, use OAM_NULL if not
 *                                      required)
 *                   2. p_max_entries, (Optional, use OAM_NULL if not
 *                                      required)
 *                   3. obj_parent (Optional, use OAM_NULL if not required),
 *                   4. obj_first_sibling (Optional, use OAM_NULL if not
 *                                         required)
 *  Returns        : return OAM_SUCCESS/OAM_FAILURE
 ***************************************************************************/
/* SPR 18930  FIX START */
oam_return_et
oam_neighborlist_ltecell_data_accessor(UInt16 **p_curr_entries,
                                      UInt16 *p_max_entries,
                                      xmlNode **obj_parent,
                                      xmlNode **obj_first_sibling,
/* SPR 16603 Fix Start */
                                      xmlNode *** obj_struct_parent,
                                      UInt8 *cell_idx, 
/* SPR 16603 Fix End */
                                      oam_error_code_et *p_err)
{
    UInt8 cid = oam_prov_req.cell_cntxt.curr_cell_cid;
    UInt8 idx = OAM_ZERO;
    oam_return_et retVal = OAM_SUCCESS;
    XML_struct_cell_config *p_cellconfig = OAM_NULL;

    OAM_FUN_ENTRY_TRACE();

    
    if (p_curr_entries || p_max_entries) {
        for ( ; idx < oam_prov_req.num_cells_configured; ++idx) {
            if (cid == SON_CELL_NEIGHLIST_INFO(idx).cid) {
/* SPR 16603 Fix Start */
                *cell_idx = idx;
/* SPR 16603 Fix End */

                if (p_curr_entries) {
                    *p_curr_entries =
                        &SON_NR_LIST(idx).intra_rat_nr_list_size;
                }

                if (p_max_entries) {
                    *p_max_entries =
                        SON_CELL_NEIGHLIST_INFO(idx).max_nr_intra_entries;
                }
                break;
            }
        }
    }

    if (idx == oam_prov_req.num_cells_configured) {
        OAM_LOG(OAM, OAM_WARNING, "LTECell data not found");
        if (p_err) {
            *p_err = OAM_INVALID_CELL_CONTEXT;
        }

        retVal = OAM_FAILURE;
    }
    else if (obj_parent || obj_first_sibling) {
        p_cellconfig = oam_get_xml_cellconfig_node(cid);

        if (p_cellconfig) {
            if (obj_parent) {
                *obj_parent = p_cellconfig->NeighborList;
            }
            if (obj_first_sibling) {
                *obj_first_sibling = oam_get_xml_leaf_child_node(
                                                  p_cellconfig->NeighborList,
                                                  "LTECellNumberOfEntries");
            }
/* SPR 16603 Fix Start */
            if (obj_struct_parent) {
                *obj_struct_parent = &p_cellconfig->LTECell[0];
            }
/* SPR 16603 Fix End */
        }
        else {
            if (p_err) {
                *p_err = OAM_INVALID_CELL_CONTEXT;
            }

            retVal = OAM_FAILURE;
        }
    }

    OAM_FUN_EXIT_TRACE();
    return retVal;
}
/* SPR 18930  FIX END */

/****************************************************************************
 *  Function Name  : oam_neighborlist_interratcell_umts_data_accessor
 *  Description    : This function returns pointers to current number of
 *                   umts neighbor, max number of umts neighbor,
 *                   pointer to parent of
 *                   UMTS node, and pointer to first sibling of UMTS
 *                   node
 *  Inputs         : NONE

 *  Output         : 1. p_curr_entries, (Optional, use OAM_NULL if not
 *                                      required)
 *                   2. p_max_entries, (Optional, use OAM_NULL if not
 *                                      required)
 *                   3. obj_parent (Optional, use OAM_NULL if not required),
 *                   4. obj_first_sibling (Optional, use OAM_NULL if not 
 *                                         required)
 *  Returns        : return OAM_SUCCESS/OAM_FAILURE
 ***************************************************************************/
/* SPR 18930  FIX START */
oam_return_et
oam_neighborlist_interratcell_umts_data_accessor(UInt16 **p_curr_entries,
                                                 UInt16 *p_max_entries,
                                                 xmlNode **obj_parent,
                                                 xmlNode **obj_first_sibling,
/* SPR 16603 Fix Start */
                                                 xmlNode *** obj_struct_parent,
                                                 UInt8 *cell_idx, 
/* SPR 16603 Fix End */
                                                 oam_error_code_et *p_err)
{
    UInt8 cid = oam_prov_req.cell_cntxt.curr_cell_cid;
    UInt8 idx = OAM_ZERO;
    oam_return_et retVal = OAM_SUCCESS;
    XML_struct_cell_config *p_cellconfig = OAM_NULL;

    OAM_FUN_ENTRY_TRACE();
    
    if (p_curr_entries || p_max_entries) {
        for ( ; idx < oam_prov_req.num_cells_configured; ++idx) {
            if (cid == SON_CELL_NEIGHLIST_INFO(idx).cid) {
/* SPR 16603 Fix Start */
                *cell_idx = idx;
/* SPR 16603 Fix End */

                /* SPR-13586 START */
                if (p_curr_entries) {
                    *p_curr_entries =
                              &SON_CELL_NEIGHLIST_INFO(idx).inter_rat_umts_list_size;
                }

                if (p_max_entries) {
                    *p_max_entries =
                        SON_CELL_NEIGHLIST_INFO(idx).max_nr_umts_inter_entries;
                /* SPR-13586 END */
                }
                break;
            }
        }
    }

    if (idx == oam_prov_req.num_cells_configured) {
        OAM_LOG(OAM, OAM_WARNING, "UMTS data not found");
        if (p_err) {
            *p_err = OAM_INVALID_CELL_CONTEXT;
        }

        retVal = OAM_FAILURE;
    }
    else if (obj_parent || obj_first_sibling) {
        p_cellconfig = oam_get_xml_cellconfig_node(cid);

        if (p_cellconfig) {
            if (obj_parent) {
                *obj_parent = p_cellconfig->InterRATCell_1;
            }
            if (obj_first_sibling) {
                *obj_first_sibling = oam_get_xml_leaf_child_node(
                                                p_cellconfig->InterRATCell_1,
                                                "UMTSNumberOfEntries");
            }
/* SPR 16603 Fix Start */
            if (obj_struct_parent) {
                *obj_struct_parent = &p_cellconfig->UMTS[0];
            }
/* SPR 16603 Fix End */

        }
        else {
            OAM_LOG(OAM, OAM_WARNING, "UMTS data not found");
            if (p_err) {
                *p_err = OAM_INVALID_CELL_CONTEXT;
            }

            retVal = OAM_FAILURE;
        }
    }

    OAM_FUN_EXIT_TRACE();
    return retVal;
}
/* SPR 18930  FIX END */
/* SPR-13586 START */ 
/****************************************************************************
 *  Function Name  : oam_neighborlist_interratcell_gsm_data_accessor
 *  Description    : This function returns pointers to current number of
 *                   umts neighbor, max number of umts neighbor,
 *                   pointer to parent of
 *                   GSM node, and pointer to first sibling of GSM
 *                   node
 *  Inputs         : NONE

 *  Output         : 1. p_curr_entries, (Optional, use OAM_NULL if not
 *                                      required)
 *                   2. p_max_entries, (Optional, use OAM_NULL if not
 *                                      required)
 *                   3. obj_parent (Optional, use OAM_NULL if not required),
 *                   4. obj_first_sibling (Optional, use OAM_NULL if not 
 *                                         required)
 *  Returns        : return OAM_SUCCESS/OAM_FAILURE
 ***************************************************************************/
/* SPR 18930  FIX START */
oam_return_et
oam_neighborlist_interratcell_gsm_data_accessor(UInt16 **p_curr_entries,
                                                 UInt16 *p_max_entries,
                                                 xmlNode **obj_parent,
                                                 xmlNode **obj_first_sibling,
/* SPR 16603 Fix Start */
                                                 xmlNode ***obj_struct_parent,
                                                 UInt8 *cell_idx, 
/* SPR 16603 Fix End */
                                                 oam_error_code_et *p_err)
{
    UInt8 cid = oam_prov_req.cell_cntxt.curr_cell_cid;
    UInt8 idx = OAM_ZERO;
    oam_return_et retVal = OAM_SUCCESS;
    XML_struct_cell_config *p_cellconfig = OAM_NULL;

    OAM_FUN_ENTRY_TRACE();
    
    if (p_curr_entries || p_max_entries) {
        for ( ; idx < oam_prov_req.num_cells_configured; ++idx) {
            if (cid == SON_CELL_NEIGHLIST_INFO(idx).cid) {
/* SPR 16603 Fix Start */
                *cell_idx = idx;
/* SPR 16603 Fix End */

                if (p_curr_entries) {
                    *p_curr_entries =
/* SPR 16603 Fix Start */
                        &SON_CELL_NEIGHLIST_INFO(idx).inter_rat_gsm_list_size;
/* SPR 16603 Fix Start */
                }

                if (p_max_entries) {
                    *p_max_entries =
                        SON_CELL_NEIGHLIST_INFO(idx).max_nr_gsm_inter_entries;
                }
                break;
            }
        }
    }

    if (idx == oam_prov_req.num_cells_configured) {
        OAM_LOG(OAM, OAM_WARNING, "GSM data not found");
        if (p_err) {
            *p_err = OAM_INVALID_CELL_CONTEXT;
        }

        retVal = OAM_FAILURE;
    }
/* SPR 16603 Fix Start */
    else if (obj_parent || obj_first_sibling || obj_struct_parent) {
/* SPR 16603 Fix Start */
        p_cellconfig = oam_get_xml_cellconfig_node(cid);

        if (p_cellconfig) {
            if (obj_parent) {
                *obj_parent = p_cellconfig->InterRATCell_1;
            }
            if (obj_first_sibling) {
                *obj_first_sibling = oam_get_xml_leaf_child_node(
                                                p_cellconfig->InterRATCell_1,
                                                "GSMNumberOfEntries");
            }
/* SPR 16603 Fix Start */
            if (obj_struct_parent) {
                *obj_struct_parent = &p_cellconfig->GSM[0];
            }
/* SPR 16603 Fix End */
        }
        else {
            OAM_LOG(OAM, OAM_WARNING, "GSM data not found");
            if (p_err) {
                *p_err = OAM_INVALID_CELL_CONTEXT;
            }

            retVal = OAM_FAILURE;
        }
    }

    OAM_FUN_EXIT_TRACE();
    return retVal;
}
/* SPR 18930  FIX END */
/* SPR-13586 END */

#endif


/****************************************************************************
 *  Function Name  : oam_update_first_object_list_node
 *  Description    : This function updates the first list object address of
 *                   multi-instance object in the CellConfig strucuture
 *  Inputs         : loop: index in the object table g_object_list for 
 *                         identifying object in question
 *                   obj: new object address to be updated with the existing 
 *                   one in the CellConfig xml strucure

 *  Output         : NONE
 *  Returns        : NONE
 ***************************************************************************/
void oam_update_first_object_list_node(UInt8 loop, xmlNode *obj)
{
    xmlNode **temp = OAM_NULL;
    UInt8 cid = oam_prov_req.cell_cntxt.curr_cell_cid;
    parameter_map_element_t *p_param_tbl = OAM_NULL;
    XML_struct_cell_config *p_cellconfig_xml_st = OAM_NULL;
    SInt32 offset = OAM_ZERO;

    /* Point to parameter table of the child parameters of the object */
    p_param_tbl = g_object_list[loop].parameter_table_address;

    /* Point to base address of immediate parent of the parameters */
    temp = p_param_tbl->xml_parameter_parent;

    /* point to base address of CellConfig structure */
    /* spr_21013_fix_start */
    p_cellconfig_xml_st = &xml_struct_tr196_g.FAPService[OAM_ZERO].CellConfig;
    /* spr_21013_fix_end */

    /* Calculate offset of the actual parameter parent from 
     * CellConfig structure */
    offset = (UInt8*)temp - (UInt8*)p_cellconfig_xml_st;

    /* Find pointer to CellConfig xml structure corresponding to
     * current cell-id */
    p_cellconfig_xml_st = oam_get_xml_cellconfig_node(cid);

    if (!p_cellconfig_xml_st) {
        OAM_LOG(OAM, OAM_WARNING, "XML Cell data not found");
        return;
    }

    temp = (xmlNode**)((UInt8*)p_cellconfig_xml_st + offset);

    *temp = obj;
}

/* SPR 15527 START */
/****************************************************************************
 *  Function Name  : oam_qos_data_accessor
 *  Description    : This function returns pointers to current number of
 *                   qos entries, max number of qos entries pointer to 
 *                   parent of PLMNList node, and pointer to first sibling of 
 *                   QOS node
 *  Inputs         : NONE

 *  Output         : 1. p_curr_entries, (Optional, use OAM_NULL if not
 *                                      required)
 *                   2. p_max_entries, (Optional, use OAM_NULL if not
 *                                      required)
 *                   3. obj_parent (Optional, use OAM_NULL if not required),
 *                   4. obj_first_sibling (Optional, use OAM_NULL if not
 *                                         required)
 *  Returns        : return OAM_SUCCESS/OAM_FAILURE
 ***************************************************************************/
oam_return_et
oam_qos_data_accessor(UInt16 **p_curr_entries,
                      UInt16 *p_max_entries,
                      xmlNode **obj_parent,
                      xmlNode **obj_first_sibling,
                      xmlNode *** obj_struct_parent,
                      UInt8 *cell_idx, 
                      oam_error_code_et *p_err)
{
    UInt8 cid = oam_prov_req.cell_cntxt.curr_cell_cid;
    UInt8 idx = OAM_ZERO;
    oam_return_et retVal = OAM_SUCCESS;
    XML_struct_cell_config *p_cellconfig = OAM_NULL;
    
    if (p_curr_entries || p_max_entries) {
        for ( ; idx < oam_prov_req.num_cells_configured; ++idx) {
            if (cid == oam_prov_req.oam_rrm_req.plmnlist_data[idx].cid) {
                *cell_idx = idx;

                if (p_curr_entries) {
                    *p_curr_entries = &RRM_PROV_REQ.qos_data[idx].
                                      num_qos_entries;
                }

                if (p_max_entries) {
                    *p_max_entries = RRM_PROV_REQ.qos_data[idx].
                                     max_qos_entries;
                }
                break;
            }
        }
    }

    if (idx == oam_prov_req.num_cells_configured) {
        OAM_LOG(OAM, OAM_ERROR, "QOS data not found");
        if (p_err) {
            *p_err = OAM_INVALID_CELL_CONTEXT;
        }

        retVal = OAM_FAILURE;
    }
    else if (obj_parent || obj_first_sibling) {
        p_cellconfig = oam_get_xml_cellconfig_node(cid);

        if (p_cellconfig) {
            if (obj_parent) {
                *obj_parent = p_cellconfig->EPC;
            }
            if (obj_first_sibling) {
                *obj_first_sibling = oam_get_xml_leaf_child_node(
                                                  p_cellconfig->EPC,
                                                  "QoSNumberOfEntries");
            }
            if (obj_struct_parent) {
                *obj_struct_parent = &p_cellconfig->QoS[OAM_ZERO];
            }

        }
        else {
            if (p_err) {
                *p_err = OAM_INVALID_CELL_CONTEXT;
            }

            retVal = OAM_FAILURE;
        }
    }

    OAM_FUN_EXIT_TRACE();
    return retVal;
}
/* SPR 15527 END */                                                  

/****************************************************************************
 *  Function Name  : oam_cell_plmnlist_data_accessor
 *  Description    : This function returns pointers to current number of
 *                   plmns, max number of plmns pointer to parent of PLMNList
 *                   node, and pointer to first sibling of PLMNList node
 *  Inputs         : NONE

 *  Output         : 1. p_curr_entries, (Optional, use OAM_NULL if not
 *                                      required)
 *                   2. p_max_entries, (Optional, use OAM_NULL if not
 *                                      required)
 *                   3. obj_parent (Optional, use OAM_NULL if not required),
 *                   4. obj_first_sibling (Optional, use OAM_NULL if not
 *                                         required)
 *  Returns        : return OAM_SUCCESS/OAM_FAILURE
 ***************************************************************************/
/* SPR 18930  FIX START */
oam_return_et
oam_cell_plmnlist_data_accessor(UInt16 **p_curr_entries,
                                UInt16 *p_max_entries,
                                xmlNode **obj_parent,
                                xmlNode **obj_first_sibling,
/* SPR 16603 Fix Start */
                                xmlNode *** obj_struct_parent,
                                UInt8 *cell_idx, 
/* SPR 16603 Fix End */
                                oam_error_code_et *p_err)
{
    UInt8 cid = oam_prov_req.cell_cntxt.curr_cell_cid;
    UInt8 idx = OAM_ZERO;
    oam_return_et retVal = OAM_SUCCESS;
    XML_struct_cell_config *p_cellconfig = OAM_NULL;
    
    if (p_curr_entries || p_max_entries) {
        for ( ; idx < oam_prov_req.num_cells_configured; ++idx) {
            if (cid == oam_prov_req.oam_rrm_req.plmnlist_data[idx].cid) {
/* SPR 16603 Fix Start */
                *cell_idx = idx;
/* SPR 16603 Fix End */

                if (p_curr_entries) {
                    *p_curr_entries = &RRM_PROV_REQ.plmnlist_data[idx].
                                       curr_plmn_entries;
                }

                if (p_max_entries) {
                    *p_max_entries = RRM_PROV_REQ.plmnlist_data[idx].
                                          max_plmn_entries;
                }
                break;
            }
        }
    }

    if (idx == oam_prov_req.num_cells_configured) {
        OAM_LOG(OAM, OAM_WARNING, "PLMNList data not found");
        if (p_err) {
            *p_err = OAM_INVALID_CELL_CONTEXT;
        }

        retVal = OAM_FAILURE;
    }
    else if (obj_parent || obj_first_sibling) {
        p_cellconfig = oam_get_xml_cellconfig_node(cid);

        if (p_cellconfig) {
            if (obj_parent) {
                *obj_parent = p_cellconfig->EPC;
            }
            if (obj_first_sibling) {
                *obj_first_sibling = oam_get_xml_leaf_child_node(
                                                  p_cellconfig->EPC,
                                                  "PLMNListNumberOfEntries");
            }
/* SPR 16603 Fix Start */
            if (obj_struct_parent) {
                *obj_struct_parent = &p_cellconfig->PLMNList[0];
            }
/* SPR 16603 Fix End */

        }
        else {
            if (p_err) {
                *p_err = OAM_INVALID_CELL_CONTEXT;
            }

            retVal = OAM_FAILURE;
        }
    }

    OAM_FUN_EXIT_TRACE();
    return retVal;
}
/* SPR 18930  FIX START */
/* SPR 14153 FIX START */
/****************************************************************************
 *  Function Name  : oam_make_default_multi_instance_obj
 *  Description    : This function makes a default multi instance object and
 *                   its subtree, and returns pointer to it.
 *  Inputs         : NONE

 *  Output         : 1. object name
 *                   2. instance id of the newly made object
 *  Returns        : object pointer or OAM_NULL
 ***************************************************************************/
xmlNode* oam_make_default_multi_instance_obj(Char8* obj_name, UInt16 inst_id)
{
    xmlNode *p_obj = OAM_NULL;

    OAM_FUN_ENTRY_TRACE();

    if (!obj_name || !inst_id) {
        return p_obj;
    }

    if (!oam_strcmp(obj_name, "PLMNList")) {
        p_obj = oam_make_plmnlist_def_xml_obj(inst_id);
    }
    else if (!oam_strcmp(obj_name, "Carrier")) {
        p_obj = oam_make_carrier_def_xml_obj(inst_id);
    }
    else if (!oam_strcmp(obj_name, "UTRANFDDFreq")) {
        p_obj = oam_make_utran_fdd_freq_def_xml_obj(inst_id);
    }
    else if (!oam_strcmp(obj_name, "GERANFreqGroup")) {
        p_obj = oam_make_geran_freq_group_def_xml_obj(inst_id);
    }
    else if (!oam_strcmp(obj_name, "LTECell")) {
        p_obj = oam_make_lte_cell_def_xml_obj(inst_id);
    }
    else if (!oam_strcmp(obj_name, "UMTS")) {
        p_obj = oam_make_umts_def_xml_obj(inst_id);
    }
    else if (!oam_strcmp(obj_name, "GSM")) {
        p_obj = oam_make_gsm_def_xml_obj(inst_id);
    }

    OAM_FUN_EXIT_TRACE();

    return p_obj;
}

/****************************************************************************
 *  Function Name  : oam_make_plmnlist_def_xml_obj
 *  Description    : This function makes a default PLMNList object and
 *                   its subtree, and returns pointer to it.
 *  Inputs         : NONE

 *  Output         : 1. instance id of the newly made object
 *  Returns        : object pointer or OAM_NULL
 ***************************************************************************/
xmlNode* oam_make_plmnlist_def_xml_obj(UInt16 inst_id)
{
    xmlNode* p_obj = OAM_NULL;
    xmlNode *p_child = OAM_NULL;
    Char8   buff[OAM_OBJ_INSTANCE_NUM_STR_MAX_SIZE] = {OAM_ZERO};

    OAM_FUN_ENTRY_TRACE();

    if (!inst_id) {
        return p_obj;
    }

    p_obj = xmlNewNode(OAM_NULL, (xmlChar*)"PLMNList");

    /*Convert the interger To string*/
    oam_snprintf(buff, sizeof(buff), "%d", inst_id);

   
    /* Also add vendor specifc parameter X_VENDOR_INSTANCE_ID */
    p_child = xmlNewChild(p_obj, OAM_NULL,
                (xmlChar*)"X_VENDOR_INSTANCE_ID", (xmlChar*)buff);

    if (!p_child) {
        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;
        return p_obj;
    }

    p_child = oam_xml_add_first_child(p_obj, "Enable",
                                      PLMNLIST_ENABLE_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                                "param: Enable");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "IsPrimary",
                                       PLMNLIST_IS_PRIMARY_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                                "param: IsPrimary");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "PLMNID",
                                       PLMNLIST_PLMNID_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                                "param: PLMNID");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child,
                                 "CellReservedForOperatorUse",
                                 PLMNLIST_CELL_RESV_FOR_OP_USE_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                                "param: CellReservedForOperatorUse");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj; /* Coverity 70317 Fix */
    }

    p_obj->doc = oam_prov_req.system_status.p_xml_doc;

    p_child = p_obj->children;
    while (p_child) {
        p_child->doc = p_obj->doc;
        p_child = p_child->next;
    }

    OAM_FUN_EXIT_TRACE();

    return p_obj;
}


/****************************************************************************
 *  Function Name  : oam_make_carrier_def_xml_obj
 *  Description    : This function makes a default Carrier object and
 *                   its subtree, and returns pointer to it.
 *  Inputs         : NONE

 *  Output         : 1. instance id of the newly made object
 *  Returns        : object pointer or OAM_NULL
 ***************************************************************************/
xmlNode* oam_make_carrier_def_xml_obj(UInt16 inst_id)
{
    xmlNode* p_obj = OAM_NULL;
    xmlNode *p_child = OAM_NULL;
    Char8   buff[OAM_OBJ_INSTANCE_NUM_STR_MAX_SIZE] = {OAM_ZERO};

    OAM_FUN_ENTRY_TRACE();

    if (!inst_id) {
        return p_obj;
    }

    p_obj = xmlNewNode(OAM_NULL, (xmlChar*)"Carrier");

    /*Convert the interger To string*/
    oam_snprintf(buff, sizeof(buff), "%d", inst_id);

   
    /* Also add vendor specifc parameter X_VENDOR_INSTANCE_ID */
    p_child = xmlNewChild(p_obj, OAM_NULL,
                (xmlChar*)"X_VENDOR_INSTANCE_ID", (xmlChar*)buff);

    if (!p_child) {
        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;
        return p_obj;
    }

    p_child = oam_xml_add_first_child(p_obj, "EUTRACarrierARFCN",
                                CARRIER_EUTRA_CARRIER_ARFCN_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                                "param: EUTRACarrierARFCN");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child,
                          "X_VENDOR_MEAS_BANDWIDTH_FOR_EARFCN",
                          CARRIER_MEAS_BANDWIDTH_FOR_EARFCN_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: X_VENDOR_MEAS_BANDWIDTH_FOR_EARFCN");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child,
                          "X_VENDOR_PRESENCE_ANTENNA_PORT1",
                          CARRIER_PRESENCE_ANTENNA_PORT1_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: X_VENDOR_PRESENCE_ANTENNA_PORT1");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child,
                          "X_VENDOR_NEIGHBOR_CELL_CONFIG",
                          CARRIER_NEIGHBOR_CELL_CONFIG_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: X_VENDOR_NEIGHBOR_CELL_CONFIG");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "QOffsetFreq",
                          CARRIER_QOFFSET_FREQ_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: QOffsetFreq");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child,
                                       "QRxLevMinSIB5",
                                       CARRIER_QRXLEVMIN_SIB5_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: QRxLevMinSIB5");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "PMax",
                          CARRIER_PMAX_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: PMax");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "TReselectionEUTRA",
                          CARRIER_TRESELECTION_EUTRA_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: TReselectionEUTRA");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "TReselectionEUTRASFMedium",
                          CARRIER_TRESELECTION_EUTRA_SFMEDIUM_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: TReselectionEUTRASFMedium");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "TReselectionEUTRASFHigh",
                          CARRIER_TRESELECTION_EUTRA_SFHIGH_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: TReselectionEUTRASFHigh");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "ThreshXHigh",
                          CARRIER_THRESHX_HIGH_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: ThreshXHigh");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "ThreshXLow",
                          CARRIER_THRESHX_LOW_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: ThreshXLow");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "CellReselectionPriority",
                          CARRIER_CELL_RESELECTION_PRIORITY_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: CellReselectionPriority");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "X_VENDOR_THRESHX_HIGHQ_R9",
                          CARRIER_THRESHX_HIGHQ_R9_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: X_VENDOR_THRESHX_HIGHQ_R9");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "X_VENDOR_CM_OFFSET_FREQ",
                          CARRIER_CM_OFFSET_FREQ_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: X_VENDOR_CM_OFFSET_FREQ");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "X_VENDOR_THRESHX_LOWQ_R9",
                          CARRIER_THRESHX_LOWQ_R9_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: X_VENDOR_THRESHX_LOWQ_R9");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj; /* Coverity 70313 Fix */
    }

    p_obj->doc = oam_prov_req.system_status.p_xml_doc;

    p_child = p_obj->children;
    while (p_child) {
        p_child->doc = p_obj->doc;
        p_child = p_child->next;
    }

    OAM_FUN_EXIT_TRACE();

    return p_obj;
}


/****************************************************************************
 *  Function Name  : oam_make_utran_fdd_freq_def_xml_obj
 *  Description    : This function makes a default UTRANFDDFreq object and
 *                   its subtree, and returns pointer to it.
 *  Inputs         : NONE

 *  Output         : 1. instance id of the newly made object
 *  Returns        : object pointer or OAM_NULL
 ***************************************************************************/
xmlNode* oam_make_utran_fdd_freq_def_xml_obj(UInt16 inst_id)
{
    xmlNode* p_obj = OAM_NULL;
    xmlNode *p_child = OAM_NULL;
    Char8   buff[OAM_OBJ_INSTANCE_NUM_STR_MAX_SIZE] = {OAM_ZERO};

    OAM_FUN_ENTRY_TRACE();

    if (!inst_id) {
        return p_obj;
    }

    p_obj = xmlNewNode(OAM_NULL, (xmlChar*)"UTRANFDDFreq");

    /*Convert the interger To string*/
    oam_snprintf(buff, sizeof(buff), "%d", inst_id);

   
    /* Also add vendor specifc parameter X_VENDOR_INSTANCE_ID */
    p_child = xmlNewChild(p_obj, OAM_NULL,
                (xmlChar*)"X_VENDOR_INSTANCE_ID", (xmlChar*)buff);

    if (!p_child) {
        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;
        return p_obj;
    }

    p_child = oam_xml_add_first_child(p_obj, "UTRACarrierARFCN",
                           UTRAN_FDD_FREQ_UTRA_CARRIER_ARFCN_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                                "param: UTRACarrierARFCN");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "QRxLevMin",
                          UTRAN_FDD_FREQ_QRXLEVMIN_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: QRxLevMin");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "QQualMin",
                          UTRAN_FDD_FREQ_QQUAL_MIN_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: QQualMin");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "CellReselectionPriority",
                     UTRAN_FDD_FREQ_CELL_RESELECTION_PRIORITY_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: CellReselectionPriority");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "ThreshXHigh",
                          UTRAN_FDD_FREQ_THRESHX_HIGH_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: ThreshXHigh");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "ThreshXLow",
                          UTRAN_FDD_FREQ_THRESHX_LOW_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: ThreshXLow");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "PMaxUTRA",
                          UTRAN_FDD_FREQ_PMAX_UTRA_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: PMaxUTRA");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "X_VENDOR_THRESHX_HIGHQ_R9",
                          UTRAN_FDD_FREQ_THRESHX_HIGHQ_R9_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: X_VENDOR_THRESHX_HIGHQ_R9");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "X_VENDOR_THRESHX_LOWQ_R9",
                          UTRAN_FDD_FREQ_THRESHX_LOWQ_R9_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: X_VENDOR_THRESHX_LOWQ_R9");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj; /* Coverity 70319 Fix*/
    }

    p_obj->doc = oam_prov_req.system_status.p_xml_doc;

    p_child = p_obj->children;
    while (p_child) {
        p_child->doc = p_obj->doc;
        p_child = p_child->next;
    }

    OAM_FUN_EXIT_TRACE();

    return p_obj;
}


/****************************************************************************
 *  Function Name  : oam_make_geran_freq_group_def_xml_obj
 *  Description    : This function makes a default GERANFreqGroup object and
 *                   its subtree, and returns pointer to it.
 *  Inputs         : NONE

 *  Output         : 1. instance id of the newly made object
 *  Returns        : object pointer or OAM_NULL
 ***************************************************************************/
xmlNode* oam_make_geran_freq_group_def_xml_obj(UInt16 inst_id)
{
    xmlNode* p_obj = OAM_NULL;
    xmlNode *p_child = OAM_NULL;
    Char8   buff[OAM_OBJ_INSTANCE_NUM_STR_MAX_SIZE] = {OAM_ZERO};

    OAM_FUN_ENTRY_TRACE();

    if (!inst_id) {
        return p_obj;
    }

    p_obj = xmlNewNode(OAM_NULL, (xmlChar*)"GERANFreqGroup");

    /*Convert the interger To string*/
    oam_snprintf(buff, sizeof(buff), "%d", inst_id);

   
    /* Also add vendor specifc parameter X_VENDOR_INSTANCE_ID */
    p_child = xmlNewChild(p_obj, OAM_NULL,
                (xmlChar*)"X_VENDOR_INSTANCE_ID", (xmlChar*)buff);

    if (!p_child) {
        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;
        return p_obj;
    }

    p_child = oam_xml_add_first_child(p_obj, "BandIndicator",
                           GERAN_FREQ_GROUP_BAND_INDICATOR_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                                "param: BandIndicator");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "BCCHARFCN",
                          GERAN_FREQ_GROUP_BCCH_ARFCN_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: BCCHARFCN");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "X_VENDOR_NCC_PERMITTED",
                          GERAN_FREQ_GROUP_NCC_PERMITTED_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: X_VENDOR_NCC_PERMITTED");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "QRxLevMin",
                          GERAN_FREQ_GROUP_QRXLEV_MIN_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: QRxLevMin");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "CellReselectionPriority",
                     GERAN_FREQ_GROUP_CELL_RESELECTION_PRIORITY_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: CellReselectionPriority");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "ThreshXHigh",
                          GERAN_FREQ_GROUP_THRESHX_HIGH_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: ThreshXHigh");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "ThreshXLow",
                          GERAN_FREQ_GROUP_THRESHX_LOW_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: ThreshXLow");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }


    p_child = oam_xml_add_next_sibling(p_child, "X_VENDOR_OFFSET_FREQ",
                          GERAN_FREQ_GROUP_OFFSET_FREQ_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: X_VENDOR_OFFSET_FREQ");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj; /* Coverity 70314 Fix */
    }

    p_obj->doc = oam_prov_req.system_status.p_xml_doc;

    p_child = p_obj->children;
    while (p_child) {
        p_child->doc = p_obj->doc;
        p_child = p_child->next;
    }

    OAM_FUN_EXIT_TRACE();

    return p_obj;
}


/****************************************************************************
 *  Function Name  : oam_make_lte_cell_def_xml_obj
 *  Description    : This function makes a default LTECell object and
 *                   its subtree, and returns pointer to it.
 *  Inputs         : NONE

 *  Output         : 1. instance id of the newly made object
 *  Returns        : object pointer or OAM_NULL
 ***************************************************************************/
xmlNode* oam_make_lte_cell_def_xml_obj(UInt16 inst_id)
{
    xmlNode* p_obj = OAM_NULL;
    xmlNode *p_child = OAM_NULL;
    Char8   buff[OAM_OBJ_INSTANCE_NUM_STR_MAX_SIZE] = {OAM_ZERO};

    OAM_FUN_ENTRY_TRACE();

    if (!inst_id) {
        return p_obj;
    }

    p_obj = xmlNewNode(OAM_NULL, (xmlChar*)"LTECell");

    /*Convert the interger To string*/
    oam_snprintf(buff, sizeof(buff), "%d", inst_id);

   
    /* Also add vendor specifc parameter X_VENDOR_INSTANCE_ID */
    p_child = xmlNewChild(p_obj, OAM_NULL,
                (xmlChar*)"X_VENDOR_INSTANCE_ID", (xmlChar*)buff);

    if (!p_child) {
        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;
        return p_obj;
    }

    p_child = oam_xml_add_first_child(p_obj, "PLMNID",
                                      LTE_CELL_PLMNID_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                                "param: PLMNID");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "CID",
                                       LTE_CELL_CID_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: CID");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "EUTRACarrierARFCN",
                          LTE_CELL_EUTRA_CARRIER_ARFCN_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: EUTRACarrierARFCN");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "PhyCellID",
                          LTE_CELL_PHY_CELL_ID_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: PhyCellID");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "Blacklisted",
                     LTE_CELL_BLACKLISTED_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: Blacklisted");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "X_VENDOR_NR_STATUS",
                          LTE_CELL_NR_STATUS_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: X_VENDOR_NR_STATUS");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj; /* Coverity 70316 Fix */
    }

    p_obj->doc = oam_prov_req.system_status.p_xml_doc;

    p_child = p_obj->children;
    while (p_child) {
        p_child->doc = p_obj->doc;
        p_child = p_child->next;
    }

    OAM_FUN_EXIT_TRACE();

    return p_obj;
}


/****************************************************************************
 *  Function Name  : oam_make_umts_def_xml_obj
 *  Description    : This function makes a default UMTS object and
 *                   its subtree, and returns pointer to it.
 *  Inputs         : NONE

 *  Output         : 1. instance id of the newly made object
 *  Returns        : object pointer or OAM_NULL
 ***************************************************************************/
xmlNode* oam_make_umts_def_xml_obj(UInt16 inst_id)
{
    xmlNode* p_obj = OAM_NULL;
    xmlNode *p_child = OAM_NULL;
    Char8   buff[OAM_OBJ_INSTANCE_NUM_STR_MAX_SIZE] = {OAM_ZERO};

    OAM_FUN_ENTRY_TRACE();

    if (!inst_id) {
        return p_obj;
    }

    p_obj = xmlNewNode(OAM_NULL, (xmlChar*)"UMTS");

    /*Convert the interger To string*/
    oam_snprintf(buff, sizeof(buff), "%d", inst_id);

   
    /* Also add vendor specifc parameter X_VENDOR_INSTANCE_ID */
    p_child = xmlNewChild(p_obj, OAM_NULL,
                (xmlChar*)"X_VENDOR_INSTANCE_ID", (xmlChar*)buff);

    if (!p_child) {
        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;
        return p_obj;
    }

    p_child = oam_xml_add_first_child(p_obj, "PLMNID",
                                      UMTS_PLMNID_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                                "param: PLMNID");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "RNCID",
                                       UMTS_RNCID_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: RNCID");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "CID", UMTS_CID_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: CID");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "X_VENDOR_NR_STATUS",
                                       UMTS_NR_STATUS_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: X_VENDOR_NR_STATUS");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "X_VENDOR_HO_STATUS",
                                       UMTS_HO_STATUS_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: X_VENDOR_HO_STATUS");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "UARFCNDL",
                                       UMTS_UARFCNDL_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                                "param: UARFCNDL");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "X_VENDOR_ACCESS_MODE",
                                       UMTS_ACCESS_MODE_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: X_VENDOR_ACCESS_MODE");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj; /* Coverity 70318 Fix*/
    }

    p_obj->doc = oam_prov_req.system_status.p_xml_doc;

    p_child = p_obj->children;
    while (p_child) {
        p_child->doc = p_obj->doc;
        p_child = p_child->next;
    }

    OAM_FUN_EXIT_TRACE();

    return p_obj;
}


/****************************************************************************
 *  Function Name  : oam_make_gsm_def_xml_obj
 *  Description    : This function makes a default GSM object and
 *                   its subtree, and returns pointer to it.
 *  Inputs         : NONE

 *  Output         : 1. instance id of the newly made object
 *  Returns        : object pointer or OAM_NULL
 ***************************************************************************/
xmlNode* oam_make_gsm_def_xml_obj(UInt16 inst_id)
{
    xmlNode* p_obj = OAM_NULL;
    xmlNode *p_child = OAM_NULL;
    Char8   buff[OAM_OBJ_INSTANCE_NUM_STR_MAX_SIZE] = {OAM_ZERO};

    OAM_FUN_ENTRY_TRACE();

    if (!inst_id) {
        return p_obj;
    }

    p_obj = xmlNewNode(OAM_NULL, (xmlChar*)"GSM");

    /*Convert the interger To string*/
    oam_snprintf(buff, sizeof(buff), "%d", inst_id);

   
    /* Also add vendor specifc parameter X_VENDOR_INSTANCE_ID */
    p_child = xmlNewChild(p_obj, OAM_NULL,
                (xmlChar*)"X_VENDOR_INSTANCE_ID", (xmlChar*)buff);

    if (!p_child) {
        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;
        return p_obj;
    }

    p_child = oam_xml_add_first_child(p_obj, "PLMNID",
                                      GSM_PLMNID_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                                "param: PLMNID");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "LAC", GSM_LAC_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: LAC");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "CI", GSM_CI_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: CI");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "BandIndicator",
                                       GSM_BAND_INDICATOR_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                                "param: BandIndicator");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "BCCHARFCN",
                                       GSM_BCCH_ARFCN_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: BCCHARFCN");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "BSIC",
                                       GSM_BSIC_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: BSIC");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "X_VENDOR_NR_STATUS",
                                       GSM_NR_STATUS_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: X_VENDOR_NR_STATUS");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj;
    }

    p_child = oam_xml_add_next_sibling(p_child, "X_VENDOR_HO_STATUS",
                                       GSM_HO_STATUS_DEF_VAL_STR);

    if (!p_child) {
        OAM_LOG(OAM, OAM_WARNING, "Failed to create mandatory child "
                              "param: X_VENDOR_HO_STATUS");

        xmlFreeNode(p_obj);
        p_obj = OAM_NULL;

        return p_obj; /* Coverity 70315 Fix */
    }

    p_obj->doc = oam_prov_req.system_status.p_xml_doc;

    p_child = p_obj->children;
    while (p_child) {
        p_child->doc = p_obj->doc;
        p_child = p_child->next;
    }

    OAM_FUN_EXIT_TRACE();

    return p_obj;
}
/* SPR 14153 FIX END */
/* SPR 15604 Fix Start */
/****************************************************************************
 *  Function Name  : oam_neighborlistInUse_ltecell_data_accessor
 *  Description    : This function returns pointers to current number of
 *                   ltecell neighbor, max number of ltecell neighbor,
 *                   pointer to parent of
 *                   LTECell node, and pointer to first sibling of LTECell
 *                   node
 *  Inputs         : NONE

 *  Output         : 1. p_curr_entries, (Optional, use OAM_NULL if not
 *                                      required)
 *                   2. p_max_entries, (Optional, use OAM_NULL if not
 *                                      required)
 *                   3. obj_parent (Optional, use OAM_NULL if not required),
 *                   4. obj_first_sibling (Optional, use OAM_NULL if not
 *                                         required)
 *  Returns        : return OAM_SUCCESS/OAM_FAILURE
 ***************************************************************************/
/* SPR 18930  FIX START */
oam_return_et
oam_neighborlistInUse_ltecell_data_accessor(UInt16 **p_curr_entries,
                                      UInt16 *p_max_entries,
                                      xmlNode **obj_parent,
                                      xmlNode **obj_first_sibling,
                                      xmlNode *** obj_struct_parent,
                                      UInt8 *cell_idx,
                                      oam_error_code_et *p_err)
{
    oam_return_et retVal = OAM_SUCCESS;
    XML_struct_cell_config *p_cellconfig = OAM_NULL;
    UInt8 cid = OAM_CELL_CNTXT_STATUS(*cell_idx).cid;

    OAM_FUN_ENTRY_TRACE();


    if (p_curr_entries || p_max_entries) {

                //LOCAL_SON_NEIGHBORLISTINUSE_INFO(idx).intra_rat_nr_list_size =
                  //                  SON_NEIGHBORLISTINUSE_INFO(idx).intra_rat_nr_list_size;
                if (p_curr_entries) {
                    *p_curr_entries =
                        &NRT_INFO(*cell_idx).nr_list.intra_rat_nr_list_size;
                }

                if (p_max_entries) {
                    *p_max_entries =
                        //LOCAL_SON_CELL_NEIGHLIST_INFO(*cell_idx).max_nr_intra_entries;
                        SON_NEIGHBORLISTINUSE_INFO(*cell_idx).max_lte_cell_entries;
                         }
            }

    if (obj_parent || obj_first_sibling) {
        p_cellconfig = oam_get_xml_cellconfig_node(cid);
                OAM_LOG(OAM, OAM_DETAILED, " obj_data_accessor: p_cell_config = %p,cid = %u  ",p_cellconfig,cid);

        if (p_cellconfig) {
            if (obj_parent) {
                *obj_parent = p_cellconfig->NeighborListInUse;
            }
            if (obj_first_sibling) {
                *obj_first_sibling = oam_get_xml_leaf_child_node(
                                                  p_cellconfig->NeighborListInUse,
                                                  "LTECellNumberOfEntries");
            }
            if (obj_struct_parent) {
                *obj_struct_parent = &p_cellconfig->LTECell_NLIU[0];
            }

        }
        else {
            if (p_err) {
                *p_err = OAM_INVALID_CELL_CONTEXT;
            }

            retVal = OAM_FAILURE;
        }
    }

    OAM_FUN_EXIT_TRACE();
    return retVal;
}
/* SPR 18930  FIX END */

/****************************************************************************
 *  Function Name  : oam_neighborlistInUse_interratcell_umts_data_accessor
 *  Description    : This function returns pointers to current number of
 *                   umts neighbor, max number of umts neighbor,
 *                   pointer to parent of
 *                   UMTS node, and pointer to first sibling of UMTS
 *                   node
 *  Inputs         : NONE

 *  Output         : 1. p_curr_entries, (Optional, use OAM_NULL if not
 *                                      required)
 *                   2. p_max_entries, (Optional, use OAM_NULL if not
 *                                      required)
 *                   3. obj_parent (Optional, use OAM_NULL if not required),
 *                   4. obj_first_sibling (Optional, use OAM_NULL if not
 *                                         required)
 *  Returns        : return OAM_SUCCESS/OAM_FAILURE
 ***************************************************************************/
/* SPR 18930  FIX START */
oam_return_et
oam_neighborlistInUse_interratcell_umts_data_accessor(UInt16 **p_curr_entries,
                                                 UInt16 *p_max_entries,
                                                 xmlNode **obj_parent,
                                                 xmlNode **obj_first_sibling,
                                                 xmlNode *** obj_struct_parent,
                                                 UInt8 *cell_idx, 
                                                 oam_error_code_et *p_err)
{
    oam_return_et retVal = OAM_SUCCESS;
    XML_struct_cell_config *p_cellconfig = OAM_NULL;
    UInt8 cid = OAM_CELL_CNTXT_STATUS(*cell_idx).cid;

    OAM_FUN_ENTRY_TRACE();

    if (p_curr_entries || p_max_entries) {

                /* SPR-13586 START */
                if (p_curr_entries) {
                    *p_curr_entries =
                        (UInt16 *)&SON_NEIGHBORLISTINUSE_INFO(*cell_idx).
                                           inter_rat_umts_neigh_size;
                }

                if (p_max_entries) {
                    *p_max_entries =
                        SON_NEIGHBORLISTINUSE_INFO(*cell_idx).max_umts_entries;
                }
                /* SPR-13586 END */
        }

    if (obj_parent || obj_first_sibling) {
        p_cellconfig = oam_get_xml_cellconfig_node(cid);

        if (p_cellconfig) {
            if (obj_parent) {
                *obj_parent = p_cellconfig->InterRATCell_2;
            }
            if (obj_first_sibling) {
                *obj_first_sibling = oam_get_xml_leaf_child_node(
                                                p_cellconfig->InterRATCell_2,
                                                "UMTSNumberOfEntries");
            }

            if (obj_struct_parent) {
                *obj_struct_parent = &p_cellconfig->UMTS_NLIU[0];
            }


        }
        else {
            OAM_LOG(OAM, OAM_WARNING, "UMTS data not found");
            if (p_err) {
                *p_err = OAM_INVALID_CELL_CONTEXT;
            }

            retVal = OAM_FAILURE;
        }
    }

    OAM_FUN_EXIT_TRACE();
    return retVal;
}
/* SPR 18930  FIX END */
/* SPR 15604 Fix End */

/*SPR 15604 START */
/******************************************************************************
 *  Function Name  : oam_generate_instance_id_for_NeighborListInUse_LTECell
 *  Description    : This function is used to generate  the isntance id of Neighbour list
 *  Inputs         : add_del_flag : add/delete flag
 *                   instance_id  : insatnce id of the object name
 *  Output         : None
 *  Returns        : return OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
/* SPR 18930  FIX START */
oam_return_et
oam_generate_instance_id_for_NeighborListInUse_LTECell(UInt16 *p_instance_id,UInt16  max_entries, oam_error_code_et *p_err, UInt8 cell_arr_idx)
{

    OAM_FUN_ENTRY_TRACE();
    UInt8  i = OAM_ZERO,
           start_idx = OAM_ZERO;


    /* SPR 17777 fix */
    OAM_LOG(OAM, OAM_DETAILEDALL, "unused parameter %p", p_err);

    
    /* Exhuast list first, and then rollover to assign instance id */
    start_idx = (NR_LIST_IN_USE(cell_arr_idx).intra_rat_last_added_instance_id)%
                (NR_LIST_IN_USE(cell_arr_idx).max_lte_cell_entries);

    for(i = start_idx; i < max_entries ; i++)  //31may
    {
        if(OAM_FALSE == NR_LIST_IN_USE(cell_arr_idx).intra_rat_is_valid[i])
        {
            *p_instance_id = (i + OAM_ONE);// Return instance id as array index + 1
            /* Save the last added instance id */
            NR_LIST_IN_USE(cell_arr_idx).intra_rat_last_added_instance_id = *p_instance_id;
            OAM_FUN_EXIT_TRACE();

            return OAM_SUCCESS;
        }
    }
    /* No instance aviable from last added till end, then search from start till last added */
    if(*p_instance_id == OAM_ZERO)
    {
        for(i = OAM_ONE; i <= NR_LIST_IN_USE(cell_arr_idx).intra_rat_last_added_instance_id ; i++)  //31may
        {
            if(OAM_FALSE == NR_LIST_IN_USE(cell_arr_idx).intra_rat_is_valid[i])
            {
                *p_instance_id = (i + OAM_ONE);// Return instance id as array index + 1
                /* Save the last added instance id */
                NR_LIST_IN_USE(cell_arr_idx).intra_rat_last_added_instance_id = *p_instance_id;
                OAM_FUN_EXIT_TRACE();

                return OAM_SUCCESS;
            }
        }
    }
    OAM_FUN_EXIT_TRACE();

    return OAM_FAILURE;
}
/* SPR 18930  FIX END */


/******************************************************************************
 *  Function Name  : oam_generate_instance_id_for_NeighborListInUse_InterRATCell_UMTS
 *  Description    : This function is used to generate the instance id of umts
 *  Inputs         : add_del_flag : add/delete flag
 *                   instance_id  : insatnce id of the object name
 *  Output         : None
 *  Returns        : return OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
/* SPR 18930  FIX START */
oam_return_et
oam_generate_instance_id_for_NeighborListInUse_InterRATCell_UMTS(UInt16 *p_instance_id,UInt16  max_entries, oam_error_code_et *p_err, UInt8 cell_arr_idx)
{

    OAM_FUN_ENTRY_TRACE();
    UInt8  i = OAM_ZERO,
           start_idx = OAM_ZERO;
   
   /*  coverity 94807 fix start */
   /*  coverity 94807 fix end */

   /* SPR 17777 fix */
   OAM_LOG(OAM, OAM_DETAILEDALL, "unused parameter %p", p_err);

   /*coverity 94835 fix start*/
   /*coverity 94835 fix end*/
    /* Exhuast list first, and then rollover to assign instance id */
    start_idx = (NR_LIST_IN_USE(cell_arr_idx).inter_rat_last_added_instance_id)%
                (NR_LIST_IN_USE(cell_arr_idx).max_umts_entries);
                OAM_LOG(OAM,OAM_INFO,"start_idx is %d",start_idx);
    for(i = start_idx; i < max_entries ; i++)  //31may
    {
        if(OAM_FALSE == NR_LIST_IN_USE(cell_arr_idx).inter_rat_is_valid[i])
        {
            *p_instance_id = (i + OAM_ONE);// Return instance id as array index + 1
            /* Save the last added instance id */
           NR_LIST_IN_USE(cell_arr_idx).inter_rat_last_added_instance_id = *p_instance_id;
            OAM_FUN_EXIT_TRACE();
            return OAM_SUCCESS;
        }
    }
    /* No instance aviable from last added till end, then search from start till last added */
    if(*p_instance_id == OAM_ZERO)
    {
        for(i = OAM_ONE; i <= NR_LIST_IN_USE(cell_arr_idx).inter_rat_last_added_instance_id ; i++)  //31may
        {
            if(OAM_FALSE == NR_LIST_IN_USE(cell_arr_idx).inter_rat_is_valid[i])
            {
                *p_instance_id = (i + OAM_ONE);// Return instance id as array index + 1
                /* Save the last added instance id */
                NR_LIST_IN_USE(cell_arr_idx).inter_rat_last_added_instance_id = *p_instance_id;
                OAM_FUN_EXIT_TRACE();
                return OAM_SUCCESS;
            }
        }
    }
    OAM_FUN_EXIT_TRACE();
    return OAM_FAILURE;

}
/* SPR 18930  FIX END */

/* SPR 15527 START */
/******************************************************************************
 * * Function Name  : oam_add_qos_object
 * * Description    : This function handles add object api for QOS from tr069
 * * Inputs         : aff_obj_name : Pointer to object name
 * *                  loop 
 *                    buff
 * * Output         : None
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_add_qos_object(Char8 * add_obj_name,UInt8 loop,Char8 * buff)
{
    xmlNodePtr    new_node = xmlNewNode(OAM_NULL,(xmlChar*)add_obj_name);
    xmlNode *temp_object_node = OAM_NULL;
    xmlNode *bitrate_node = OAM_NULL;
    xmlNode *rohc_node = OAM_NULL;
    xmlNode *rohc_profiles_node = OAM_NULL;
    UInt8 param_loop = OAM_ZERO;
    Char8   *param_name = OAM_NULL;
    oam_return_et retVal = OAM_SUCCESS;
    xmlNode *object_node = OAM_NULL;
    oam_return_et countInstanceRetVal = OAM_SUCCESS;
    oam_error_code_et *p_err = NO_ERROR;
    xmlNode *p_obj_parent = OAM_NULL;
    xmlNode *p_obj_first_sibling = OAM_NULL;
    xmlNode **p_p_obj_struct_parent = OAM_NULL;
    UInt8 cell_idx = OAM_ZERO;
    UInt16 * curr_num_entries = OAM_NULL; /*to hold the address od current entries */
    UInt16 max_num_entries = OAM_ZERO;


    countInstanceRetVal = g_object_list[loop].obj_data_accessor(&curr_num_entries,
                                                   &max_num_entries,
                                                   &p_obj_parent,
                                                   &p_obj_first_sibling,
                                                   &p_p_obj_struct_parent,
                                                   &cell_idx,
                                                   p_err);

    if (OAM_FAILURE == countInstanceRetVal)
    {
        xmlNode *temp_object_node;
        temp_object_node = oam_xml_find_tag(p_obj_parent->children, (xmlChar *)"QoSNumberOfEntries");
        xmlNodePtr    new_node = xmlNewNode(OAM_NULL,(xmlChar*)add_obj_name);
        object_node = xmlAddNextSibling((xmlNode *)(temp_object_node),new_node);
    }
    else
    {
        object_node = xmlAddNextSibling((xmlNode *)(p_obj_first_sibling), new_node);
        *p_p_obj_struct_parent = new_node;
    }

    /* Loop for all parameters in object */
    for(param_loop = OAM_ZERO; param_loop < g_object_list[loop].no_of_parameter; param_loop++)
    {
        if(g_object_list[loop].parameter_table_address[param_loop].parameter_mode & OAM_WRITE_ONLY)
        {
            /*Get the Parameter Name*/
            param_name = g_object_list[loop].parameter_table_address[param_loop].tag_name;
            OAM_LOG(OAM,OAM_INFO," Object Parameter Name : %s",param_name);
            if(((oam_strcmp(param_name,"MIN_UL_BITRATE") == OAM_ZERO) ||
                    (oam_strcmp(param_name,"MIN_DL_BITRATE") == OAM_ZERO))&&
                    (bitrate_node != OAM_NULL))
            {
                /* create child parameter in dom tree */
                xmlNewTextChild (bitrate_node, NULL,
                        (const xmlChar *)param_name, NULL);
            }
            else if(((oam_strcmp(param_name,"ENABLE_ROHC") == OAM_ZERO) ||
                    (oam_strcmp(param_name,"MAX_CID") == OAM_ZERO))&&
                    (rohc_node != OAM_NULL))
            {
                /* create child parameter in dom tree */
                xmlNewTextChild (rohc_node, NULL,
                        (const xmlChar *)param_name, NULL);
            }
            else if(((oam_strstr(param_name,"ROHC_PROFILE0X0")) &&
                    (rohc_profiles_node != OAM_NULL)))
            {
                /* create child parameter in dom tree */
                xmlNewTextChild (rohc_profiles_node, NULL,
                        (const xmlChar *)param_name, NULL);
            }
            else
            {
                /* create child parameter in dom tree */
                xmlNewTextChild (object_node, NULL,
                        (const xmlChar *)param_name, NULL);
            }
        }
        if((oam_strcmp(param_name,"X_VENDOR_SUPPORTED_RAT") == OAM_ZERO))
        {
            temp_object_node = oam_xml_find_tag((xmlNode *)(object_node)->children,
                    (xmlChar *)"X_VENDOR_SUPPORTED_RAT");
            xmlNodePtr    temp_node = xmlNewNode(OAM_NULL,(xmlChar*)"X_VENDOR_BITRATE");
            bitrate_node = xmlAddNextSibling((xmlNode *)(temp_object_node),temp_node);
        }
        else if((oam_strcmp(param_name,"X_VENDOR_RLC_UM_PDCP_SN_SIZE") == OAM_ZERO))
        {
            temp_object_node = oam_xml_find_tag((xmlNode *)(object_node)->children,
                    (xmlChar *)"X_VENDOR_RLC_UM_PDCP_SN_SIZE");
            xmlNodePtr    temp_node = xmlNewNode(OAM_NULL,(xmlChar*)"X_VENDOR_ROHC_PARAMS");
            rohc_node = xmlAddNextSibling((xmlNode *)(temp_object_node),temp_node);
        }
        /* Coverity CID 85391 Fix Start */
        else if((oam_strcmp(param_name,"MAX_CID") == OAM_ZERO) && (rohc_node != OAM_NULL))
        /* Coverity CID 85391 Fix End */
        {
            temp_object_node = oam_xml_find_tag((xmlNode *)(rohc_node)->children,
                    (xmlChar *)"MAX_CID");
            xmlNodePtr    temp_node = xmlNewNode(OAM_NULL,(xmlChar*)"ROHC_PROFILES");
            rohc_profiles_node = xmlAddNextSibling((xmlNode *)(temp_object_node),temp_node);
        }


    }
    /* Also add vendor specifc parameter X_VENDOR_INSTANCE_ID */
    xmlNewChild((xmlNode *)(object_node), OAM_NULL, (xmlChar *)"X_VENDOR_INSTANCE_ID", (xmlChar *)buff);


return retVal;
}
/* SPR 15527 END */

/******************************************************************************
 * * Function Name  : oam_handle_nliu_object_addition
 * * Description    : This function triggers add object api for NLIU
 * * Inputs         : obj_name : Pointer to object name
 * *                  p_msg    : Pointer to message
 * * Output         : None
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_handle_nliu_object_addition(
    char *obj_name ,
    void *p_req,
    /* SPR 18930  FIX START */
    son_rat_type_et rat_type,
    UInt8 cell_config_idx
    /* SPR 18930  FIX END */
    )
{


/* NLIU ADD OBJECT CODE START*/

    OAM_FUN_ENTRY_TRACE();
    /* SPR 18930  FIX START */
    //UInt16 curr_num_entries = OAM_ZERO;
    UInt16 *  curr_num_entries = OAM_NULL;
    UInt16 max_num_entries = OAM_ZERO;
    UInt8 cell_idx = OAM_ZERO;
    xmlNode *p_obj_parent = OAM_NULL;
    xmlNode *p_obj_sibbling = OAM_NULL;
    xmlNode **p_p_obj_struct_parent = OAM_NULL;
    oam_return_et  retVal  = OAM_SUCCESS;


    UInt8   loop, param_loop;
    Char8   *add_obj_name = OAM_NULL, *param_name = OAM_NULL, buff[OAM_FIVE]={OAM_ZERO};
    UInt16  instance_id_value = OAM_ZERO;
    oam_bool_et valid_object_name = OAM_FALSE;
    xmlNode     *object_node = OAM_NULL;
    /*  coverity 94893 fix start */
    /*  coverity 94904 fix start */
    oam_error_code_et p_err = NO_ERROR;
    /*  coverity 94904 fix end */
    /*  coverity 94893 fix end */

    cell_idx = cell_config_idx;

    UInt8   param_name_buff[512] = {0};
    UInt8 value[STR_LEN_256] = {OAM_ZERO};

    for(loop = OAM_ZERO; loop < OAM_MAX_SUPPORTED_OBJECTS; loop++)
    {
        /* Search object name in object_map_element_t */
        /* If not found, send failure add object response */
        if(OAM_ZERO == ( oam_strncasecmp(obj_name, g_object_list[loop].object_name, oam_strlen(g_object_list[loop].object_name))))
        {
            valid_object_name = OAM_TRUE;
            /* Maximum number of instance validation */
            if (!oam_strcmp(obj_name, g_object_list[loop].object_name)) {
                retVal =
                   g_object_list[loop].obj_data_accessor(&curr_num_entries,
                                                        &max_num_entries,
                                                        &p_obj_parent,
                                                        &p_obj_sibbling,
                                                        &p_p_obj_struct_parent,
                                                        &cell_idx,
                                                        /*  coverity 94893 fix start */
                                                        /* p_err); */
                                                        &p_err);
                                                        /*  coverity 94893 fix start */
            /*  coverity 94769/94902 fix start */
            }
            /*  coverity 94769/94902 fix end */

                /* Maximum number of instance validation */
            /*  coverity 96481 indentation fix start */   

            /* Cov CID 107806 fix Start */
            if(OAM_NULL == curr_num_entries)
            {
              retVal = OAM_FAILURE;
            }
            else if (*curr_num_entries == max_num_entries) {
                OAM_LOG(OAM, OAM_WARNING, "Add operation not allowed, "
                             "Max limit[%u] of instances already achieved",
                             max_num_entries);
/*  coverity 94893 fix start */
            p_err = OAM_RESOURCE_EXCCEED;
/*  coverity 94893 fix end */

            retVal = OAM_FAILURE;
/* SPR 16603 Fix code removed */
                }
            /* Cov CID 107806 fix Start */
            /*  coverity 96481 fix end */
            /*  coverity 94769/94902 fix start */
            /* } */
            /*  coverity 94769/94902 fix end */

            /*  coverity 94769 fix start */
            /*Cov 94769 fix start*/
            if (OAM_FAILURE == retVal || PNULL == p_obj_parent) {
            /*Cov 94769 fix end*/     
                OAM_LOG(OAM, OAM_WARNING, "obj_data_accessor failed ");
                return retVal; 
            }
            /*  coverity 94902 fix start */
            else
            {
				OAM_LOG(OAM, OAM_DETAILED, "cell_idx  : %d curr_num_entries : %d  max_num_entries : %d"
						" p_obj_parent : %p   p_obj_sibbling : %p ",
						cell_idx,*curr_num_entries,max_num_entries,(xmlNode *)p_obj_parent->name,(xmlNode *)p_obj_sibbling->name);
            }
            /*  coverity 94902 fix end */
            /*  coverity 94769 fix end */

 /* Get the object name*/
            add_obj_name = g_object_list[loop].tag_name;
            OAM_LOG(OAM,OAM_INFO," Object Name : %s", add_obj_name);

            /* Create the child object in the dom tree*/
            /* Create a new node */


            xmlNodePtr    new_node = xmlNewNode(OAM_NULL,(xmlChar*)add_obj_name);
            object_node = xmlAddNextSibling((xmlNode *)(p_obj_sibbling),new_node);
            if (object_node) {
                *p_p_obj_struct_parent = new_node;
            }

            /*Generate the instance number*/
            /* SPR 17582 changes start */
	    /* SPR 19221: CID 96481 Fix Start */
            if(OAM_ZERO == (oam_strncasecmp(
                        /* spr_21013_fix_start */
					    "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.NeighborListInUse.LTECell",
					    g_object_list[loop].object_name, oam_strlen(g_object_list[loop].object_name))))
                        /* spr_21013_fix_end */
	    {
		    oam_generate_instance_id_for_NeighborListInUse_LTECell(&instance_id_value,
				    max_num_entries,
				    &p_err,
				    cell_config_idx);
	    }
	    else if(OAM_ZERO == (oam_strncasecmp(
                        /* spr_21013_fix_start */
					    "InternetGatewayDevice.Services.FAPService.CellConfig.LTE.RAN.NeighborListInUse.InterRATCell.UMTS",
					    g_object_list[loop].object_name, oam_strlen(g_object_list[loop].object_name))))
                        /* spr_21013_fix_end */
	    {
		    oam_generate_instance_id_for_NeighborListInUse_InterRATCell_UMTS(&instance_id_value,
				    max_num_entries,
				    &p_err,
				    cell_config_idx);
	    }
	    /* SPR 19221: CID 96481 Fix End */

            /* SPR 17582 changes end */
            OAM_LOG(OAM,OAM_INFO," Instance Number : %d", instance_id_value);
            oam_memcpy(param_name_buff, g_object_list[loop].object_name, oam_strlen(g_object_list[loop].object_name));

            /* Loop for all parameters in object */

            for(param_loop = OAM_ZERO; param_loop < g_object_list[loop].no_of_parameter; param_loop++)
            {
                if(g_object_list[loop].parameter_table_address[param_loop].parameter_mode & OAM_WRITE_ONLY)
                {

                    /*Get the Parameter Name*/
                    param_name = g_object_list[loop].parameter_table_address[param_loop].tag_name;
                    OAM_LOG(OAM,OAM_INFO," Object Parameter Name : %s",param_name);

                    /* create child parameter in dom tree */
                    xmlNewTextChild (object_node, NULL,
                            (const xmlChar *)param_name, NULL);
                    /* SPR 17509/17456 changes start */
                    /* SPR 17509/17456 changes end */

                }
            }

            oam_memset(buff,OAM_ZERO,sizeof(buff));
            /*Convert the interger To string*/
            snprintf(buff, sizeof(buff), "%d", instance_id_value);

            /* SPR 21628 FIXED START */
            OAM_LOG(OAM, OAM_DETAILED, "Instance_id_value %d and rat_type %d", instance_id_value,rat_type);

            OAM_LOG(OAM,OAM_INFO,"Object Parameter Name : %s",param_name);
            /* SPR 21628 FIXED END */

            /* Also add vendor specifc parameter X_VENDOR_INSTANCE_ID */
            xmlNewChild((xmlNode *)(object_node), OAM_NULL, (xmlChar *)"X_VENDOR_INSTANCE_ID", (xmlChar *)&buff);
            for(param_loop = OAM_ZERO; param_loop < g_object_list[loop].no_of_parameter; param_loop++)
            {
                if(g_object_list[loop].parameter_table_address[param_loop].parameter_mode & OAM_WRITE_ONLY)
                {
                    param_name = g_object_list[loop].parameter_table_address[param_loop].tag_name;
                    oam_validate_nliu_parameter(instance_id_value,
                            param_name,
                            param_name_buff,
                            value,
                            p_req,
                            rat_type,
                            cell_config_idx);
                }
            }

            /* Increment Instance No */
            *curr_num_entries += OAM_ONE;
            /*finding pointer to node*/
            xmlNode *p_xml_node = oam_xml_find_tag(
                    ((xmlNode *)(p_obj_parent))->children,
                    ((xmlChar *)(g_object_list[loop].current_entry)));
        /*Convert the interger To string*/
            snprintf(buff, sizeof(buff), "%d",*curr_num_entries);
            xmlNodeSetContent((xmlNode *)p_xml_node, (xmlChar *)&buff);



            break;
        }
    }
    /* SPR 18930  FIX END */
/* NLIU ADD OBJECT CODE END*/

    /*If object name found, add object response to TR069*/
    if (valid_object_name == OAM_FALSE)
    {
        retVal = OAM_FAILURE; 
        OAM_LOG(OAM, OAM_WARNING, "oam_add_object: Invalid param name.");
        return retVal;
    }

    OAM_FUN_EXIT_TRACE();
    return retVal;
}



/*****************************************************************************************
 * Function Name  : oam_validate_nliu_parameter
 * Description    : This function maps the NLIU parameter name and its value
 * Inputs           instance_id_value : instance number
 *                  tag_name          : parameter name
 *                  param_name_buff   : pointer to buffer storing the parameter name
 *                  value             : pointer to parameter value
 *                  intra_rat_nr_list : Pointer to intra rat neighbor cell
 * Output         : None
 * Returns        : None
 ****************************************************************************************/
nliu_param_et oam_validate_nliu_parameter(
    UInt16 instance_id_value,
    char *tag_name,
    UInt8 *param_name_buff,
    UInt8 *value,
    void  *p_req,
    /* SPR 18930  FIX START */
    son_rat_type_et rat_type,
    UInt8 cell_config_idx)
    /* SPR 18930  FIX END */
{
        oam_return_et retVal = OAM_FAILURE;
        /*  coverity 85414 fix start */
        nliu_param_et enum_tag_val;
        /*  coverity 85414 fix end */
       // oam_error_code_et err=NO_ERROR;
            OAM_FUN_ENTRY_TRACE();

        /* below code populating the parameter name */
        son_intra_rat_neighbor_info_t *p_intra_rat_nr_list = OAM_NULL;
        son_utran_neighbor_info_t *p_inter_rat_utran_nr_list = OAM_NULL;
        UInt8 full_param_name[512] = {0};
        oam_strcpy((char *)full_param_name,(char *)param_name_buff);
        oam_strcat(full_param_name, ".[");
        oam_sprintf((char *)(full_param_name + oam_strlen((char *)full_param_name)), "%d", instance_id_value);
        oam_strcat(full_param_name, "].");
        /* SPR 21628 FIXED START */
        if(tag_name != NULL)
        {    
            oam_strcat(full_param_name, tag_name);
        }
        else
        {
            /* Coverity 114347 Fix Start */
            return PLMNID;
            /* Coverity 114347 Fix End */
        }
        /* SPR 21628 FIXED END */

        OAM_LOG(OAM, OAM_DETAILED, "NLIU generated parameter name %s", full_param_name);
        if(OAM_NULL == p_req)
            return UNDEFINED;

        /* Function "oam_xml_search_string_to_integer" maps the string name of parameter names with an integer value */

        switch (rat_type)
        {
            case SON_EUTRAN_CELL:
                p_intra_rat_nr_list = (son_intra_rat_neighbor_info_t *)p_req;
                break;
            case SON_UTRAN_CELL:
                p_inter_rat_utran_nr_list = (son_utran_neighbor_info_t *)p_req;
                break;
            default:
                OAM_LOG(OAM, OAM_WARNING, " received unsupported "
                    "rat_type[%u]. Object addition for this cell "
                    "not supported", rat_type);
                return UNDEFINED;
        }

        enum_tag_val = (nliu_param_et)oam_xml_search_string_to_integer(tag_name, NeighborList_Mapping_Table, OAM_THIRTY_TWO); /*cov 94783 +-*/
        switch (enum_tag_val)
        {
            case PLMNID:
                if (rat_type == SON_EUTRAN_CELL)
                {
                    retVal = oam_generate_plmnid((Char8 *)value, (void *)(&p_intra_rat_nr_list->nbr_cgi.plmn_id));
                }
                else
                {
                    retVal = oam_generate_plmnid((Char8 *)value, (void *)(&p_inter_rat_utran_nr_list->nbr_cgi.plmn_id));
                }
                if (retVal == OAM_FAILURE)
                {
                    OAM_LOG(OAM, OAM_WARNING, "Error generating plmn id ");
                    OAM_FUN_EXIT_TRACE();
                    return UNDEFINED;
                }
                OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                break;
            case PhyCellID:
                oam_sprintf((char *)value, "%d",p_intra_rat_nr_list->pci);
                OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                break;

          case CID:
                {
                    if (rat_type == SON_EUTRAN_CELL)
                    {
                        UInt32 cid_value = OAM_ZERO;
                        oam_cmn_convert_byte_string_to_integer(p_intra_rat_nr_list->nbr_cgi.cell_identity,OAM_FOUR,&cid_value,
                                    OAM_MAX_INT_BITSTRING_SIZE - OAM_CELL_ID_BITSTRING_SIZE);
                        /* Coverity CID 85421 Fix Start */
                        oam_sprintf((char *)value, "%d", cid_value);
                        /* Coverity CID 85421 Fix End */
                    }
                    else
                    {
                        UInt16 cid_value = OAM_ZERO;
                        oam_extract_cid_from_cell_identity(&cid_value, p_inter_rat_utran_nr_list->nbr_cgi.cell_identity);
                        oam_sprintf((char *)value, "%d", cid_value);
                    }
                    OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                }
                break;
            case QOffset:
                /* SPR 21903 FIXED START */
                p_intra_rat_nr_list->bitmask |=  SON_CELL_OFFSET_PRESENT;
                /* SPR 21903 FIXED END*/
                oam_sprintf((char *)value, "%d",p_intra_rat_nr_list->cell_specific_offset);
                if( OAM_SUCCESS == oam_search_integer_to_interger_for_get((char *)value,
                        RAN_NeighborList_LTECell_QOffset[OAM_ZERO],OAM_THIRTY_ONE))
                OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                break;
            case CIO:
                oam_sprintf((char *)value, "%d",p_intra_rat_nr_list->cm_cell_specific_offset);
                if( OAM_SUCCESS == oam_search_integer_to_interger_for_get((char *)value,
                        RAN_NeighborList_LTECell_CIO[OAM_ZERO],OAM_THIRTY_ONE))
                OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                break;
            case EUTRACarrierARFCN:
                oam_sprintf((char *)value, "%d",p_intra_rat_nr_list->earfcn);
                OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                break;

           case X_VENDOR_TAC:
                {
                    /* Coverity 113190 Fix Start */
                    if (rat_type == SON_EUTRAN_CELL)
                    {
                        UInt32 tac_value = OAM_ZERO;
                        oam_cmn_convert_byte_string_to_short(p_intra_rat_nr_list->tac,
                                &tac_value,
                                OAM_MAX_SHORT_INT_BITSTRING_SIZE - OAM_TAC_BIT_STRING_SIZE);
                        oam_sprintf((char *)value, "%d", tac_value);
                        OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                    }
                    else
                    {
                        OAM_LOG(OAM, OAM_ERROR, "X_VENDOR_TAC is not applicable for UTRAN");
                    }
                    /* Coverity 113190 Fix End */
                }
                break;
            case X_VENDOR_CSGID:
                {
                    UInt32 csg_value = OAM_ZERO;
                    if (rat_type == SON_EUTRAN_CELL)
                    {
                        oam_cmn_convert_csgid_to_integer(p_intra_rat_nr_list->csg_identity, OAM_FOUR, &csg_value);
                    }
                    else
                    {
                        oam_cmn_convert_csgid_to_integer(p_inter_rat_utran_nr_list->csg_identity, OAM_FOUR, &csg_value);
                    }
                    oam_sprintf((char *)value, "%d", csg_value);
                    OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                }
                break;
            case Blacklisted:
                oam_sprintf((char *)value, "%d",p_intra_rat_nr_list->ho_status);
                if( OAM_SUCCESS == oam_search_integer_to_interger_for_get((char *)value,
                        NR_Blacklisted[OAM_ZERO],OAM_TWO))
                OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                break;
            case X_VENDOR_RSRP:
                /* SPR 21903 FIXED START */
                p_intra_rat_nr_list->bitmask |= SON_RSRP_PRESENT;
                /* SPR 21903 FIXED END*/
                oam_sprintf((char *)value, "%d",p_intra_rat_nr_list->rsrp);
                OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                break;
            case X_VENDOR_RSRQ:
                /* SPR 21903 FIXED START */
                p_intra_rat_nr_list->bitmask |= SON_RSRQ_PRESENT;
                /* SPR 21903 FIXED END */
                oam_sprintf((char *)value, "%d",p_intra_rat_nr_list->rsrq);
                OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                break;


 case X_VENDOR_NR_STATUS:
                if (rat_type == SON_EUTRAN_CELL)
                {
                oam_sprintf((char *)value, "%d",p_intra_rat_nr_list->nr_status);
                }
                else
                {
                    oam_sprintf((char *)value, "%d",p_inter_rat_utran_nr_list->nr_status);
                }
                OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                break;
            case X_VENDOR_BROADCAST_STATUS:
                oam_sprintf((char *)value, "%d",p_intra_rat_nr_list->broadcast_status);
                OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                break;
            case X_VENDOR_PRACH_ROOT_SEQUENCE_INDEX:
                oam_sprintf((char *)value, "%d",p_intra_rat_nr_list->prach_config.root_sequence_index);
                OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                break;
            case X_VENDOR_PRACH_ZERO_CORRELATION_ZONE_CONFIG:
                oam_sprintf((char *)value, "%d",p_intra_rat_nr_list->prach_config.zero_correlation_zone_config);
                OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                break;
            case X_VENDOR_PRACH_HIGH_SPEED_FLAG:
                oam_sprintf((char *)value, "%d",p_intra_rat_nr_list->prach_config.high_speed_flag);
                OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                break;
            case X_VENDOR_PRACH_FREQUENCY_OFFSET:
                oam_sprintf((char *)value, "%d",p_intra_rat_nr_list->prach_config.prach_frequency_offset);
                OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                break;
            case X_VENDOR_PRACH_CONFIGURATION_INDEX:
                /* SPR 21903 FIXED START */
                p_intra_rat_nr_list->prach_config.bitmask |= SON_PRACH_CONFIG_INDEX_PRESENT;
                /* SPR 21903 FIXED END */
                oam_sprintf((char *)value, "%d",p_intra_rat_nr_list->prach_config.prach_configuration_index);
                OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                break;

            case X_VENDOR_ACCESS_MODE:
                if (rat_type == SON_EUTRAN_CELL)
                {
                        oam_sprintf((char *)value, "%d",p_intra_rat_nr_list->access_mode);
                        OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                }
                else
                {
                        oam_sprintf((char *)value, "%d",p_inter_rat_utran_nr_list->access_mode);
                        OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                }
                break;
            case PCPICHScramblingCode:
                    /* SPR 17454 changes start */
                    if(p_inter_rat_utran_nr_list)
                    {
                    /* SPR 17454 changes end */            
                        if(p_inter_rat_utran_nr_list->pci.bitmask &  SON_PCI_FDD_PRESENT)   
                        {
                            oam_sprintf((char *)value, "%d",p_inter_rat_utran_nr_list->pci.pci_fdd);
                        }
                        else
                        {
                            oam_sprintf((char *)value, "%d",p_inter_rat_utran_nr_list->pci.pci_tdd);
                        }
                OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                    /* SPR 17454 changes start */
                    }
                    /* SPR 17454 changes end */
                break;
            case RNCID:
                {
                    /* SPR 17454 changes start */
                    if(p_inter_rat_utran_nr_list)
                    {
                    /* SPR 17454 changes end */
                    UInt16 rncid_val = OAM_ZERO;
                    oam_extract_rncid_from_cell_identity(&rncid_val,
                            &p_inter_rat_utran_nr_list->nbr_cgi.cell_identity[OAM_ZERO]);
                    oam_sprintf((char *)value, "%d", rncid_val);
                    OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                    /* SPR 17454 changes start */
                    }
                    /* SPR 17454 changes end */
                }
                break;
            case LAC:
                {
                    UInt32 lac_val = OAM_ZERO;
                    /* SPR 17454 changes start */
                    if(p_inter_rat_utran_nr_list)
                    {
                    /* SPR 17454 changes end */
                    /*SPR 22398 Fix Start*/
                    oam_cmn_convert_bytes_to_integer_for_LAC(&p_inter_rat_utran_nr_list->lac[OAM_ZERO], &lac_val);
                    /*SPR 22398 Fix End*/
                    oam_sprintf((char *)value, "%u", lac_val);
                    OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                    /* SPR 17454 changes start */
                    }
                    /* SPR 17454 changes end */
                }
                break;
            case RAC:
                    /* SPR 17454 changes start */
                    if(p_inter_rat_utran_nr_list)
                    {
                    /* SPR 17454 changes end */
                    oam_sprintf((char *)value, "%d", p_inter_rat_utran_nr_list->rac);
                    OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                    /* SPR 17454 changes start */
                    }
                    /* SPR 17454 changes end */
                break;
            case X_VENDOR_RSCP:
                    /* SPR 17454 changes start */
                    if(p_inter_rat_utran_nr_list)
                    {
                    /* SPR 17454 changes end */
                    oam_sprintf((char *)value, "%d", p_inter_rat_utran_nr_list->rscp);
                    OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                    /* SPR 17454 changes start */
                    }
                    /* SPR 17454 changes end */
                break;

            case X_VENDOR_CELL_SPECIFIC_OFFSET:
                    /* SPR 17454 changes start */
                    if(p_inter_rat_utran_nr_list)
                    {
                    /* SPR 17454 changes end */
                    oam_sprintf((char *)value, "%d", p_inter_rat_utran_nr_list->cell_specific_offset);
                    OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                    /* SPR 17454 changes start */
                    }
                    /* SPR 17454 changes end */
                break;
            case X_VENDOR_NUMPLMNID:
                    /* SPR 17454 changes start */
                    if(p_inter_rat_utran_nr_list)
                    {
                    /* SPR 17454 changes end */
                    oam_sprintf((char *)value, "%d", p_inter_rat_utran_nr_list->num_plmn_id);
                    OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                    /* SPR 17454 changes start */
                    }
                    /* SPR 17454 changes end */
                break;
            case X_VENDOR_PLMN_ID:
                    /* SPR 17454 changes start */
                    if(p_inter_rat_utran_nr_list)
                    {
                    /* SPR 17454 changes end */
                    oam_sprintf((char *)value, "%d", p_inter_rat_utran_nr_list->num_plmn_id);
                    OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                    /* SPR 17454 changes start */
                    }
                    /* SPR 17454 changes end */
                break;
            case X_VENDOR_PS_HO_SUPPORTED:
                    /* SPR 17454 changes start */
                    if(p_inter_rat_utran_nr_list)
                    {
                    /* SPR 17454 changes end */
                    oam_sprintf((char *)value, "%d", p_inter_rat_utran_nr_list->ps_ho_supported);
                    OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                    /* SPR 17454 changes start */
                    }
                    /* SPR 17454 changes end */
                break;
            case X_VENDOR_VOIP_CAPABLE:
                    /* SPR 17454 changes start */
                    if(p_inter_rat_utran_nr_list)
                    {
                    /* SPR 17454 changes end */
                    oam_sprintf((char *)value, "%d", p_inter_rat_utran_nr_list->voip_capable);
                    OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                    /* SPR 17454 changes start */
                    }
                    /* SPR 17454 changes end */
                break;
            case UARFCNDL:
                    /* SPR 17454 changes start */
                    if(p_inter_rat_utran_nr_list)
                    {
                    /* SPR 17454 changes end */
                    oam_sprintf((char *)value, "%d", p_inter_rat_utran_nr_list->uarfcn);
                    OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                    /* SPR 17454 changes start */
                    }
                    /* SPR 17454 changes end */
                break;
            case X_VENDOR_HO_STATUS:
                    /* SPR 17454 changes start */
                    if(p_inter_rat_utran_nr_list)
                    {
                    /* SPR 17454 changes end */
                    oam_sprintf((char *)value, "%d", p_inter_rat_utran_nr_list->ho_status);
                    OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                    /* SPR 17454 changes start */
                    }
                    /* SPR 17454 changes end */
                break;
            case X_VENDOR_ECNO:
                    /* SPR 17454 changes start */
                    if(p_inter_rat_utran_nr_list)
                    {
                    /* SPR 17454 changes end */
                    oam_sprintf((char *)value, "%d", p_inter_rat_utran_nr_list->ecno);
                    OAM_LOG(OAM, OAM_DETAILED, "%s value persisted for NLIU %s", tag_name, value);
                    /* SPR 17454 changes start */
                    }
                    /* SPR 17454 changes end */
                break;
            default:
                OAM_LOG(OAM, OAM_WARNING, "Unknown parameter[%u] received", 
                    enum_tag_val);
                break;
        }

/* SPR 17509/17456 changes start */
    xmlNode *p_orig_parent = OAM_NULL;
    xmlNode *p_req_parent = OAM_NULL;
    xmlNode **p_p_xml_struct_addr = OAM_NULL;
        /* SPR 17777 fix code removed */
        oam_error_code_et p_error_code = NO_ERROR;
                /* SPR 18930  FIX START */
        retVal = oam_update_set_parameter(/* name */ (char *)full_param_name, 
                (char *)value,
                /* p_local_req, */
                OAM_NULL,
                &p_orig_parent,
                &p_req_parent,
                &p_p_xml_struct_addr,
                /* SPR 17777 fix code removed */
                &p_error_code,
                cell_config_idx
                );


        oam_update_xml_dom(retVal, p_orig_parent,
                p_req_parent,
                p_p_xml_struct_addr);
        /* SPR 18930  FIX END */
	/*SPR 21619 Start*/
	if(p_req_parent!=OAM_NULL)
	{	
		xmlFreeNode(p_req_parent);
	}
	/*SPR 21619 End*/

        if (retVal == OAM_FAILURE)
        {
            OAM_LOG(OAM,OAM_WARNING, "parameter validation failed");
            OAM_FUN_EXIT_TRACE();
            return UNDEFINED;
         }
/* SPR 17509/17456 changes end */         
        OAM_FUN_EXIT_TRACE();
        return enum_tag_val;

}

/******************************************************************************
 * * Function Name  : oam_handle_nliu_object_deletion
 * * Description    : This function triggers delete object for a NLIU instance
 * * Inputs         : obj_name : Pointer to object name
 * *                  p_msg : Pointer to message
 * * Output         : None
 * * Returns        : return OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
/* SPR 18930  FIX START */
oam_return_et
oam_handle_nliu_object_deletion(
    char *obj_name,
    UInt32 instance_id,
    UInt8 cell_config_idx
    )
{
  OAM_FUN_ENTRY_TRACE();

/* NLIU DELETE OBJECT CODE START*/

    UInt16 *curr_num_entries = OAM_ZERO;
    UInt16 max_num_entries = OAM_ZERO;
    UInt8 cell_idx = OAM_ZERO;
    xmlNode *p_obj_parent = OAM_NULL;
    xmlNode *p_obj_sibbling = OAM_NULL;
    oam_return_et  retVal  = OAM_SUCCESS;
     UInt8 inst_id_val =  OAM_ZERO;

    /* SPR 17652 changes start */
    /*  coverity 94904 fix start */
    oam_error_code_et p_err = NO_ERROR;
    /*  coverity 94904 fix end */
    /* SPR 17652 changes end */

  UInt8   loop, instance_no = OAM_ZERO,
          *p_param_val = OAM_NULL;
  /* coverity 94820 fix start */
  Char8   temp_obj[OAM_OBJ_NAME_MAX_LEN]= {OAM_ZERO},
           *del_obj_name = OAM_NULL;
  Char8 buff[MAX_PARAMETER_NAME_LENGTH]={OAM_ZERO};
  /* coverity 94820 fix end */
  oam_bool_et   valid_object_name = OAM_FALSE, is_deleted = OAM_FALSE;

  /*  coverity 94836 fix start */
  /* code removed */
  /*  coverity 94836 fix end */

    cell_idx = cell_config_idx;

  /*Check for max no.s of obj supported*/
  for(loop = OAM_ZERO; loop < OAM_MAX_SUPPORTED_OBJECTS; loop++)
  {
    /* Search object name in object_map_element_t */
    /* If not found, send failure delete object response */
    if(OAM_ZERO == ( oam_strncasecmp(obj_name, g_object_list[loop].object_name, oam_strlen(g_object_list[loop].object_name))))
    {

                retVal =
                   g_object_list[loop].obj_data_accessor(&curr_num_entries,
                                                        &max_num_entries,
                                                        &p_obj_parent,
                                                        &p_obj_sibbling,
                                                        OAM_NULL,
                                                        &cell_idx,
                                                        /* SPR 17652 changes start */
                                                        &p_err);
                                                        /* SPR 17652 changes end */

                if (OAM_FAILURE == retVal) {
                    OAM_LOG(OAM, OAM_WARNING, "obj_data_accessor failed ");
                }


      if(OAM_ZERO == *curr_num_entries)
        break;

      valid_object_name = OAM_TRUE;
 /*Get Instance Number*/
//      oam_strncpy((UInt8*)temp_obj, (UInt8*)obj_name, sizeof(temp_obj) - 1);
      /*Cov 94759_94832 fix start*/
      oam_strncpy(temp_obj, obj_name, sizeof(temp_obj) - 1);
      /*Cov 94759_94832 fix end*/
      instance_no = instance_id;


      /* Get the object name*/
      del_obj_name = g_object_list[loop].tag_name;

      OAM_LOG(OAM,OAM_INFO," Full path Delete Object Name is %s. "
        " Delete Object Name is  = %s. "
        " Delete Instance no is  = %d", obj_name, del_obj_name, instance_no);

      /* Points to first object in parent node */
      //xmlNode   *p_curr_obj = ((xmlNode *)(*g_object_list[loop].parent_node))->children;
       xmlNode   *p_curr_obj = p_obj_parent->children;

      /* Traverse all objects present under parent object to find the object to be deleted by its instance id */
       while(OAM_NULL != p_curr_obj)
       {
           p_curr_obj = oam_xml_find_tag(p_curr_obj,
                   (xmlChar*)g_object_list[loop].tag_name);

           if(!p_curr_obj) {
               OAM_LOG(OAM, OAM_WARNING, "Object not found");
               /*Cov 94767 fix start*/
               if(p_err){
               /* SPR 17652 changes start */
               p_err = OAM_PARAMETER_NOT_FOUND;
               /* SPR 17652 changes end */
               }
               /*Cov 94767 fix end*/
               retVal = OAM_FAILURE;
               break;
           }

           p_param_val = oam_get_xml_text_node_content(p_curr_obj,
                   "X_VENDOR_INSTANCE_ID",
                   buff,
                   MAX_PARAMETER_NAME_LENGTH);

           if (p_param_val != OAM_NULL){
               inst_id_val =  oam_atoi((Char8*)p_param_val);

               if (instance_no == inst_id_val &&  (oam_strcmp((char *)p_curr_obj->name,(char *)g_object_list[loop].tag_name) == OAM_ZERO) )
               {
                   OAM_LOG(OAM,OAM_INFO," Delete object instance id in xml %d", inst_id_val);
                 is_deleted = OAM_TRUE;
                   /*delete the object using the instance number*/
                   xmlUnlinkNode((xmlNode *)(p_curr_obj));
                   xmlFreeNode((xmlNode *)(p_curr_obj));
                   break;
               }
           }
           /* Move to next instance of object */
           p_curr_obj = p_curr_obj->next;
       }/* End of internal while loop */


      if (OAM_FALSE == is_deleted)
      {
        OAM_LOG(OAM, OAM_WARNING, "oam_delete_object: instance not found.");

        return OAM_FAILURE;
      }

      *curr_num_entries -= OAM_ONE;

      /*finding pointer to node*/
      xmlNode *p_xml_node = oam_xml_find_tag(((xmlNode *)(p_obj_parent))->children ,((xmlChar *)(g_object_list[loop].current_entry)));

      /*Convert the interger To string*/
      snprintf(buff, sizeof(buff), "%d",*curr_num_entries);
      /*Set the current instance no in the xml file*/
      xmlNodeSetContent((xmlNode *)p_xml_node, (xmlChar *)&buff);

      OAM_LOG(OAM, OAM_DETAILED, "Current nu of instances after deleting object %s ", buff);

      /* If state is not active, don't send message to lower layer*/
    }/* End of if object found */
  }/* End of outer for loop */

/* NLIU DELETE OBJECT CODE END*/

  /* Search object name, If not found, send failure delete object response */
  if (valid_object_name == OAM_FALSE)  /*Checking for invalid param name path*/
  {
    OAM_LOG(OAM, OAM_WARNING, "oam_delete_object: Invalid param name.");
  }

  OAM_FUN_EXIT_TRACE();
  /*  coverity 94836 fix start */
  return retVal;
  /*  coverity 94836 fix end */
}
/* SPR 18930  FIX END */

//#endif
/*SPR 15604 END*/


