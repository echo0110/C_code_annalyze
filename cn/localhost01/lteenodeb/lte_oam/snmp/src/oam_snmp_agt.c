/******************************************************************************
*   FILE NAME    : oam_snmp_agt.c
*
*   DESCRIPTION  : SNMP Agent Init
*
*       DATE            AUTHOR          REFERENCE       REASON
*   25 July 2013   Purshottam Gupta     ---------	Initial
*
*   Copyright (c) 2013, Aricent Inc. All Rights Reserved
*
******************************************************************************/

/*******************************************************************************
*                        Header Files
*******************************************************************************/
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <oam_enb_mib_scalars.h>
#include <oam_enb_mib_tables.h>
#include "oam_snmp_interface.h"
/* SPR 20584 changes start */
#include        <cspl.h>
#include	<oam_snmp_main.h>
/* SPR 20584 changes end */

/*******************************************************************************
*                        GLOBEL VARIBLES
*******************************************************************************/
int value=5;
extern oam_mgmt_if_param_tab_t snmp_param_table[];
extern UInt32 snmp_param_table_max_size;
char package_name[ENBSWIMAGEFILE_NAME_MAX_SIZE] = {0} ;
extern prov_request_t prov_req;
/* SPR 20584 changes start */
extern char snmp_master_ip_addr[MAX_IP_ADD_LEN_STR];
extern int snmp_master_port;
extern int tcp_flag;
/* SPR 20584 changes end */
RETSIGTYPE
/* SPR 17777 fix */
free_resources(int a) {

    /* SPR 17777 fix */
    deinitialize_table_data();
    oam_snmp_fclose(prov_req.p_out);
    oam_snmp_exit(0);
}


/* + SPR 17439 */
void init_aricentLTEeNodeBMIB(void)
/* - SPR 17439 */
{
    init_oam_enb_mib_scalars();
    init_oam_enb_mib_tables();
}


/******************************************************************************
* Function Name    : init_oam_snmp_agent
* DESCRIPTION      : It is a init function for snmp agent
* INPUT            : NONE
* OUTPUT           : NONE                       
* RETURNS          : Void
******************************************************************************/
/* + SPR 17439 */
void init_oam_snmp_agent(void)
/* - SPR 17439 */
{
    int agentx_subagent=1; /* change this if you want to be a SNMP master agent */

    /*  CID: 40450 fix */
    snmp_enable_stderrlog();

    /* we're an agentx subagent? */
    if (agentx_subagent) 
    {
        /* make us a agentx client. */
        netsnmp_ds_set_boolean(NETSNMP_DS_APPLICATION_ID, NETSNMP_DS_AGENT_ROLE, 1);
    }

    /* initialize tcpip, if necessary */
    SOCK_STARTUP;

   /* SPR 20584 changes start */
   if(tcp_flag)
   {
       char* buf = NULL;
       int retVal = oam_mgmt_mem_alloc(SNMP_ID_ADDR_AND_PORT,(void**)&buf);

       sprintf(buf, "tcp:%s:%d",snmp_master_ip_addr,snmp_master_port);
       if(OAM_SNMP_SUCCESS != retVal){
           OAM_SNMP_LOG(SNMP, OAM_SNMP_ERROR,
                   " Memory allocation to buf failed");
       }  
       netsnmp_ds_set_string(NETSNMP_DS_APPLICATION_ID,
               NETSNMP_DS_AGENT_X_SOCKET,
               buf);
       OAM_SNMP_LOG(SNMP, OAM_SNMP_DETAILED,
               " TCP buf %s",buf);
       oam_mgmt_mem_free(buf);
   }
   /* SPR 20584 changes end */

    /* initialize the agent library */
    init_agent("lte_oamSnmpAgt");

    /* initialize mib code here */

    /* mib code: init_nstAgentSubagentObject from nstAgentSubagentObject.C */
    init_aricentLTEeNodeBMIB();

    /* example-demon will be used to read example-demon.conf files. */
    init_snmp("lte_oamSnmpAgt");

    /* In case we recevie a request to stop (kill -TERM or kill -INT) */
    oam_snmp_register_signal_handler(SNMP_SIGTERM, free_resources);  /*porting changes*/
    oam_snmp_register_signal_handler(SNMP_SIGINT, free_resources); /*porting changes*/

}

/******************************************************************************
* Function Name    : oam_snmp_handle_get_request
* DESCRIPTION      : It is a handler function
* INPUT            : tab_index : table index
*                    requests  : requests
* OUTPUT           : retVal
* RETURNS          : SUCCESS/FAILURE
******************************************************************************/
int
oam_snmp_handle_get_request(netsnmp_mib_handler *handler,
                            netsnmp_handler_registration *reginfo,
                            netsnmp_request_info *request,
                            int param_type,
                            int val_type)
{
    /*Coc cid 63651 Fix */
    int retVal = OAM_SNMP_SUCCESS;
    int tab_index = oam_snmp_atoi(handler->handler_name);
    int offset = 0;
    oam_mgmt_if_req_t p_msg ;
    void *resp_buff = NULL;
    oam_mgmt_if_param_info_t *param = NULL;
    char idx[SNMP_ROW_INSTANCE_ID_MAX_LEN] = {0};
    const oid      *const suffix =
        request->requestvb->name + reginfo->rootoid_len + 1;
    netsnmp_table_request_info *table_info;
    int column = 0;
    oam_mgmt_if_get_resp_t *p_resp = NULL;
    Char8* value = NULL;
    int count = 0; 
    /*kclockwork fix */
    if(tab_index < OID_PARAM_MIN ||
            tab_index >= (SInt32)snmp_param_table_max_size) {
        return SNMP_ERR_GENERR; 
    }

    retVal = oam_mgmt_mem_alloc(sizeof(oam_mgmt_if_param_info_t),
            (void**)&p_msg.param_list);

    if(OAM_SNMP_SUCCESS != retVal || !p_msg.param_list) {
        return retVal;
    }

    retVal = oam_mgmt_mem_alloc(sizeof(oam_mgmt_if_req_t),
            (void**)&resp_buff);
    if(OAM_SNMP_SUCCESS != retVal) {
        oam_mgmt_mem_free(p_msg.param_list);
        return retVal;
    }


    if(tab_index != ENBCTRLCELLACTION_INDEX &&
            tab_index != ENBSWUPDATEACTION_INDEX &&
            tab_index != ENBSWIMAGEFILE_INDEX )  {
        /* coverity 52854 */
        oam_mgmt_memset(p_msg.param_list, 0, sizeof(oam_mgmt_if_param_info_t));
        /* KLOCWORK warning fix start */
        oam_snmp_strncpy(p_msg.parent_name,
                snmp_param_table[tab_index].parent,
                sizeof(p_msg.parent_name) - 1);

        if(SNMP_OAM_TABLE_PARAM == param_type) {
            offset = *(suffix + 1);
            /** extracts the information about the table from the request */
            table_info = netsnmp_extract_table_info(request);
            column = table_info->colnum - 1;

            /*kclockwork fix */
            if((tab_index + column < OID_PARAM_MIN) ||
                    (tab_index + column >=(SInt32)snmp_param_table_max_size)) {
                oam_mgmt_mem_free(resp_buff);
                oam_mgmt_mem_free(p_msg.param_list);
                return SNMP_ERR_GENERR; 
            }
            oam_snmp_snprintf(idx, SNMP_ROW_INSTANCE_ID_MAX_LEN, ".[%d]", offset);
            /* coverity 52858 */
            oam_snmp_strncat(p_msg.parent_name, idx, oam_snmp_strlen(idx) +1);
        }

        p_msg.num_params   = 1;

        /*  CID: 53739 fix : VKG */
        oam_snmp_strncpy(p_msg.param_list->param_name,
                snmp_param_table[tab_index + column].param,
                sizeof(p_msg.param_list->param_name) - 1);
        /* KLOCWORK warning fix end */
        p_msg.param_list->next = NULL;
        retVal = oam_snmp_get_req_handler(&p_msg, resp_buff);
    }
    else {
        if(tab_index != ENBSWIMAGEFILE_INDEX) {	
            int asn_temp_value = 0;
            snmp_set_var_typed_value(request->requestvb, ASN_INTEGER,
                    &asn_temp_value, sizeof(asn_temp_value));
        }
        else {
            snmp_set_var_typed_value(request->requestvb, ASN_OCTET_STR,
                    &package_name, oam_snmp_strlen(package_name));

        }
        oam_mgmt_mem_free(resp_buff);
        oam_mgmt_mem_free(p_msg.param_list);
        return SNMP_ERR_NOERROR;
    }

    switch(retVal) {
        case OAM_SNMP_SUCCESS:
            p_resp = (oam_mgmt_if_get_resp_t*)resp_buff;
            param = p_resp->param_list;
            if(tab_index == ENB_CONFIGURED_CID) {
                for(count = 0; param && count < p_resp->num_params; ++count) {
                    /* CID 63786 FIX START */
                    value = oam_snmp_strchr(param->param_name, '=');
                    ++value;

                    /* Copy all the CIDs returned into one single
                     * space delimited string */
                    oam_snmp_strcat(idx, value);
                    oam_snmp_strcat(idx, " ");

                    param = param->next;
                }

                value = idx;
                /* CID 63786 FIX END */
            }
            else {
                value = oam_snmp_strchr(p_resp->param_list->param_name, '=');
                /* CID 81805 FIX START */
            if(!value) {
                retVal = SNMP_ERR_GENERR;
                break;
            }
                /* CID 81805 FIX END */
                ++value;
            }


            if(ASN_INTEGER == val_type ||
                    ASN_UNSIGNED == val_type ||
                    ASN_LONG_LEN == val_type) {
                int asn_value = oam_snmp_atoi(value);
                snmp_set_var_typed_value(request->requestvb, val_type,
                        &asn_value, sizeof(asn_value));
            }
            else {
                snmp_set_var_typed_value(request->requestvb, val_type,
                        value, oam_snmp_strlen(value));
            }

            retVal = SNMP_ERR_NOERROR;
            break;

        case OAM_SNMP_NOACCESS:
            retVal = SNMP_ERR_NOACCESS;
            break;

        case OAM_SNMP_RESOURCEUNAVAILABLE:
            retVal = SNMP_ERR_RESOURCEUNAVAILABLE;
            break;

        case OAM_SNMP_NOSUCHNAME:
            retVal = SNMP_ERR_NOSUCHNAME;
            break;

        case OAM_SNMP_NOTWRITABLE:
            retVal = SNMP_ERR_NOTWRITABLE;
            break;

        default:
            retVal = SNMP_ERR_GENERR;
            break;
    }
    oam_mgmt_mem_free(resp_buff);
    oam_mgmt_mem_free(p_msg.param_list);

    return retVal;
}

/******************************************************************************
* Function Name    : oam_snmp_handle_set_request
* DESCRIPTION      : It is a handler function
* INPUT            : tab_index : table index
*                    requests  : requests
* OUTPUT           : retVal
* RETURNS          : SUCCESS/FAILURE
******************************************************************************/
int
oam_snmp_handle_set_request
(
    netsnmp_mib_handler *handler,
    netsnmp_handler_registration *reginfo,
    netsnmp_request_info *requests,
    int param_type,
    int val_type
)
{

    int retVal;
    oam_mgmt_if_req_t p_msg ;
    int tab_index = oam_snmp_atoi(handler->handler_name);
    int offset = 0;
    char idx[SNMP_ROW_INSTANCE_ID_MAX_LEN] = {0};
    const oid      *const suffix =
        requests->requestvb->name + reginfo->rootoid_len + 1;
    netsnmp_table_request_info *table_info;
    int column = 0;
    void *resp_buff = NULL;
    SInt32 rem_len = 0;


    /*kclockwork fix */
	 if((tab_index < OID_PARAM_MIN) ||
        (tab_index >= (SInt32)snmp_param_table_max_size)) {
		 return SNMP_ERR_GENERR;
	 }

    retVal = oam_mgmt_mem_alloc(sizeof(oam_mgmt_if_param_info_t),
                           (void**)&p_msg.param_list);
    if(OAM_SNMP_SUCCESS != retVal) {
        return retVal;
    }

    retVal = oam_mgmt_mem_alloc(sizeof(oam_mgmt_if_req_t),
                                (void**)&resp_buff);
    if(OAM_SNMP_SUCCESS != retVal) {
        oam_mgmt_mem_free(p_msg.param_list);
        return retVal;
    }

    oam_mgmt_memset(p_msg.param_list, 0, sizeof(oam_mgmt_if_param_info_t));
    oam_mgmt_memset(resp_buff, 0, sizeof(oam_mgmt_if_req_t));


    if(tab_index != ENBCTRLCELLACTION_INDEX &&
       tab_index != ENBSWUPDATEACTION_INDEX &&
       tab_index != ENBSWIMAGEFILE_INDEX) {
        /* KLOCWORK warning fix start */
        oam_snmp_strncpy(p_msg.parent_name,
                snmp_param_table[tab_index].parent,
                sizeof(p_msg.parent_name) - 1);

        if(SNMP_OAM_TABLE_PARAM == param_type) {
            offset = *(suffix + 1);
            /** extracts the information about the table from the request */
            table_info = netsnmp_extract_table_info(requests);
            column = table_info->colnum - 1;

            /*kclockwork fix */
            if((tab_index + column < OID_PARAM_MIN) ||
               (tab_index + column >= (SInt32)snmp_param_table_max_size)) {
                 oam_mgmt_mem_free(resp_buff);
                 oam_mgmt_mem_free(p_msg.param_list);
                 return SNMP_ERR_GENERR; 
             }
            oam_snmp_snprintf(idx, SNMP_ROW_INSTANCE_ID_MAX_LEN, ".[%d]", offset);
	    /* coverity 52859 */	
            oam_snmp_strncat(p_msg.parent_name, idx, oam_snmp_strlen(idx) +1);
        }

        OAM_SNMP_LOG(SNMP, OAM_SNMP_DETAILED,
                     "Parent name: %s", p_msg.parent_name);
        p_msg.num_params   = 1;
    }


    switch(val_type) {
        case ASN_OCTET_STR:
		    if(ENBSWIMAGEFILE_INDEX == tab_index) {
                oam_snmp_snprintf(package_name, sizeof(package_name),
                         "%s", requests->requestvb->val.string);
                oam_mgmt_mem_free(resp_buff);
                oam_mgmt_mem_free(p_msg.param_list);
                return SNMP_ERR_NOERROR;
		    }
            /*  CID: 53740 fix */
            rem_len = sizeof(p_msg.param_list->param_name);
            oam_snmp_strncpy(p_msg.param_list->param_name,
                        snmp_param_table[tab_index + column].param,
                        rem_len - 1);

            rem_len -= oam_snmp_strlen(snmp_param_table[tab_index + column].param);
			oam_snmp_strncat(p_msg.param_list->param_name, "=", rem_len - 1);

            --rem_len;
            oam_snmp_strncat(p_msg.param_list->param_name,
                    (Char8*)requests->requestvb->val.string,
                    rem_len - 1);
         /* KLOCWORK warning fix end */
            break;

        case ASN_INTEGER:
        case ASN_UNSIGNED:
        case ASN_LONG_LEN:
        case ASN_TIMETICKS:
            if(ENBCTRLCELLACTION_INDEX == tab_index) {
                switch(*(requests->requestvb->val.integer)) {
                    case ENBCTRLCELLACTION_DELETE:
                        oam_snmp_strncpy(p_msg.parent_name,
                                "Cell.Delete",
                                oam_snmp_strlen("Cell.Delete") + 1);
                        p_msg.num_params   = 1;
                        oam_snmp_sprintf(p_msg.param_list->param_name,
                                        "%s=%s", "Cell_Identity", "Current");
                        break;

                    case ENBCTRLCELLACTION_CELLSETUP:
                        oam_snmp_strncpy(p_msg.parent_name,
                                "Cell.Setup",
                                oam_snmp_strlen("Cell.Setup") + 1);
                        p_msg.num_params = 1;
                        oam_snmp_sprintf(p_msg.param_list->param_name,
                                        "%s=%s", "Cell_Identity", "Current");
                        break;

                    case ENBCTRLCELLACTION_CELLSTOP:
                        oam_snmp_strncpy(p_msg.parent_name,
                                "Cell.Stop", oam_snmp_strlen("Cell.Stop") + 1);
                        p_msg.num_params   = 1;
                        oam_snmp_sprintf(p_msg.param_list->param_name,
                                        "%s=%s", "Cell_Identity", "Current");
                        break;

                    case ENBCTRLCELLACTION_CELLSTART:
                        oam_snmp_strncpy(p_msg.parent_name,
                                "Cell.Start", oam_snmp_strlen("Cell.Start") + 1);
                        p_msg.num_params   = 1;
                        oam_snmp_sprintf(p_msg.param_list->param_name,
                                        "%s=%s", "Cell_Identity", "Current");
                        break;

                    case ENBCTRLCELLACTION_CELLBLOCK:
                        oam_snmp_strncpy(p_msg.parent_name,
                            "Cell.Block", oam_snmp_strlen("Cell.Block") + 1);
                        p_msg.num_params   = 1;
                        oam_snmp_sprintf(p_msg.param_list->param_name,
                                    "%s=%s", "Cell_Identity", "Current");
                        break;

                    case ENBCTRLCELLACTION_CELLUNBLOCK:
                        oam_snmp_strncpy(p_msg.parent_name,
                            "Cell.Unblock", oam_snmp_strlen("Cell.Unblock") + 1);
                        p_msg.num_params   = 1;
                        oam_snmp_sprintf(p_msg.param_list->param_name,
                                    "%s=%s", "Cell_Identity", "Current");
                        break;

                    case ENBCTRLCELLACTION_NOACTION:
                        return SNMP_ERR_NOERROR;

                    default:
                        return SNMP_ERR_GENERR;
                }
            }
            else if(ENBSWUPDATEACTION_INDEX == tab_index) {
                if(oam_snmp_strlen(package_name) <= 0 &&
                    (*(requests->requestvb->val.integer) != 0)) {
                    OAM_SNMP_LOG(SNMP, OAM_SNMP_WARNING,
                            "PACKAGE NAME IS NOT CONFIGURED");
                    oam_mgmt_mem_free(resp_buff);
                    oam_mgmt_mem_free(p_msg.param_list);
                    return SNMP_ERR_GENERR;		
                }

                switch(*(requests->requestvb->val.integer)) {
                    case ENBSWUPDATEACTION_DOWNLOAD:
                        oam_snmp_strncpy(p_msg.parent_name, "eNB.Download",
                                oam_snmp_strlen("eNB.Download") + 1);
                        p_msg.num_params   = 1;
                        oam_snmp_sprintf(p_msg.param_list->param_name,
                                "%s=%s", "sw_package_name", package_name);
                        break;

                    case ENBSWUPDATEACTION_SWITCH:
                        oam_snmp_strncpy(p_msg.parent_name, "eNB.Switch",
                                oam_snmp_strlen("eNB.Switch") + 1);
                        p_msg.num_params   = 1;
                        oam_snmp_sprintf(p_msg.param_list->param_name,
                                "%s=%s", "sw_package_name", package_name);
                        break;

                }
            }
            else {
                oam_snmp_sprintf(p_msg.param_list->param_name, "%s=%ld",
                                 snmp_param_table[tab_index + column].param,
                                 *(requests->requestvb->val.integer));
            }
            break;

        default:
            oam_mgmt_mem_free(resp_buff);
            oam_mgmt_mem_free(p_msg.param_list);
            return SNMP_ERR_GENERR;    
    }

    p_msg.param_list->next = NULL;

    retVal = oam_snmp_set_req_handler(&p_msg, resp_buff);

    oam_mgmt_if_resp_status_t *p_resp = (oam_mgmt_if_resp_status_t*)resp_buff;

    if(!p_resp->err_code && OAM_SNMP_SUCCESS == retVal) {
        retVal = SNMP_ERR_NOERROR;
    }
    else {
/**BUG 1233 start****/
        if(retVal == OAM_SNMP_INVALID_VALUE)
        retVal = SNMP_ERR_BADVALUE;
/**BUG 1233 end****/
        else 
        retVal = SNMP_ERR_GENERR;
    }

    oam_mgmt_mem_free(resp_buff);
    oam_mgmt_mem_free(p_msg.param_list);

    return retVal;
}
