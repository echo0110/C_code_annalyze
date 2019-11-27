/* vim:ts=4:et:ai:ci:sw=4
*/
/*******************************************************************************
 *File Name :oam_xml_function.c
 *
 *Description : This file contains functions to read values from xml file
 *    
 *Revision Histroy:
 *Date       Author      		References            Comments
 *------    --------    		------------         -------------
 *May,2012  KAPIL TAWAR,        OAM Design Document  Initial Version
            BRIJESH MADOLIA    
 *September,2013 Neeraj Kumar    OAM Design Doc        Changes related to HM
 *April,2014   Ravi Gupta       SPR 10551 fix        
 *May,2014  Ekta Jain           SON bitmask fix            
 * June, 2014        Vivek K Gupta  Release 3.0         Changes for xml 
 *                                                      add/delete frwk code
 * June,2014    Avinash Kumar           SPR 12228 fix
 * Aug,2014     Aditi Singh         SPR 13065
 * Aug,2014     Ankita Jain         SPR 13074
 * Aug,2014     Ankita Jain         SPR 13171
 * Sep 2014       Vinod Vyas        Changes Related to eICIC 
 *Copyright (c) 2014, Aricent Inc. All Rights Reserved
 ******************************************************************************/

 /******************************************************************************
 * Standard Library Includes
 ******************************************************************************/
#include "oam_xml_header.h"
#include "oam_types.h"
#include "oam_defines.h"
#include "oam_proto.h"
#include <oam_sim.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <oam_sim_utils.h>
#include <oam_fault_mgmt.h>
#ifdef LTE_EMBMS_SUPPORTED            
#include "oam_embms_proto.h"
#endif
#include <libxml/xmlschemastypes.h>

#ifdef OAM_RADIO_HEAD_SUPPORT
#ifdef OAM_RRH_TYPE_1_SUPPORT
#include "oam_rcm_proto.h"
#endif
#endif

extern parameter_map_element_t g_parameter_TR_element[];
extern oam_sim_table_t g_oam_sim_t;

oam_prov_req_t g_oam_prov_req;
xmlSchemaValidCtxtPtr oam_schema_valid_ctxt;
oam_return_et oam_libxml_tr069_init(int argc,char *arg[]);
/* + SPR 17439 */
oam_return_et
oam_populate_rem_lte_param(void);
oam_return_et
oam_populate_igd_param(void);
oam_return_et
oam_populate_device_info_param(void);
oam_return_et
oam_populate_perf_mgmt_param(void);
oam_return_et
oam_populate_capabilities_param(void);
oam_return_et
oam_populate_tunnel_param(void);
oam_return_et
oam_populate_services_param(void);
oam_return_et
oam_populate_lte_tunnel_params(XML_struct_cell_config *p_cell_spec_params);
oam_return_et
oam_populate_management_server_param(void);
/* SPR_12373_CHANGES */
/* Redundant Code Removed */
/* SPR_12373_CHANGES */
oam_return_et
oam_populate_rrm_set_log_level_req_param(void);
oam_return_et
oam_populate_rrm_log_enable_disable_param(void);
oam_return_et
oam_populate_rrm_init_config_param(void);
oam_return_et
oam_populate_son_init_param(void);

/* RT SPR 9019 FIX START */
/* LINES DELETED */
/* RT SPR 9019 FIX END */
oam_return_et
oam_fill_version_id_req(void);
oam_return_et
oam_fill_load_config_req(void);

/*SPR 23209 FIXED START */
oam_return_et
oam_populate_l2_mac_init_mocn_param(XML_struct_cell_config     *p_cell_spec_params,
oam_rrm_cell_config_req_t  *p_rrm_cellconfig ,
/* SPR 23504/SES-341 Fix Start */
oam_l2_req_t               *p_mac_req, 
UInt8                      cell_cnt_index,
UInt8                      l2_instance
/* SPR 23504/SES-341 Fix End */
);

oam_return_et
oam_populate_rrm_cell_config_mocn_param(oam_rrm_cell_config_req_t  *p_rrm_cellconfig,
/* SPR 23504/SES-341 Fix Start */
oam_l2_req_t               *p_mac_req, 
UInt8                      cell_index,
UInt8                      l2_instance
/* SPR 23504/SES-341 Fix End */
);
/*SPR 23209 FIXED END */

/* - SPR 17439 */
/* FLAG_CHANGES - Code Removed Here */
/*******************************************************************************
 Private Types
 ******************************************************************************/
UInt32 flag_found= OAM_ZERO;
SInt32 lclTemp_value;
UInt8 loop_variable;
UInt32 IPInterfaceNumberOfEntries = OAM_ZERO; 

struct XML_struct_tr196 xml_struct_tr196_g;
struct XML_struct xml_struct_g;
xml_struct_nw_scan_t xml_struct_nw_scan;
extern prov_request_t prov_req;
UInt16 lte_cell_num_entries;

oam_prov_req_t oam_prov_req;
/*******************************************************************************
* Function Name  : oam_xml_find_tag
* Description    : This function is used to find the pointer of a particular 
*                  leaf node
* Inputs         : p_a_node :xmlNode type  pointer of the parent node
*		           p_tagname : Pointer to tag name of the leaf node
* Outputs        : none
* Returns        : xmlNode type pointer of the leaf node
*******************************************************************************/
xmlNode *
oam_xml_find_tag
(
    xmlNode *p_a_node,
    xmlChar *p_tagname
)
{
    xmlNode *p_cur_node = OAM_NULL;
    /*loop to find the next nodes*/
    for(p_cur_node = p_a_node;p_cur_node;p_cur_node = p_cur_node->next) {   
	    /*compare if node is of Type XML_TEXT_NODE */
        if(XML_TEXT_NODE != p_cur_node->type) {
		    /*comapre if node is equal the desired tag name*/
            if(xmlStrEqual(p_cur_node->name, p_tagname)) {
               return p_cur_node;			   
            }
        }
    }
   return p_cur_node;
}

#ifdef OAM_RADIO_HEAD_SUPPORT
/*******************************************************************************
* Function Name  : oam_populate_rrh_init_param
* Description    : This function initializes the startup parameters of RRH
*		           layer.
* Inputs         : None 
* Outputs        : None
* Returns        : OAM_SUCCESS/OAM_FAILURE
*******************************************************************************/
oam_return_et oam_populate_rrh_init_param()
{
#ifdef OAM_RRH_TYPE_1_SUPPORT
		return oam_populate_rcm_rrh_init_param();
#endif
	OAM_LOG(OAM, OAM_ERROR, "RRH is disabled");
	return OAM_FAILURE;
}
#endif



/*******************************************************************************
* Function Name  : oam_xml_find_ptr
* Description    : This function is used to calculate the pointer of a node
* Inputs         : p_a_node :Pointer to xmlNode of parent
                   p_b_Node : Pointer to unsigned int that will hold the output
                   p_str :Pointer to char str equal to the node name
* Outputs        : usigned int * b_Node
* Returns        : none
*******************************************************************************/
    /* SPR 20430 Changes Start*/
void 
oam_xml_find_ptr
(
    xmlNode * p_a_node,
    xmlNode ** p_b_Node,
    char * p_str
)
    /* SPR 20430 Changes End*/
{
    xmlNode * p_cur_node=OAM_NULL;

    if(!p_a_node || !p_b_Node || !p_str) {
        OAM_LOG(OAM, OAM_ERROR, "NULL POINTER ERROR!!!");
        return;
    }


    /*loop to find the next nodes*/
    for (p_cur_node = p_a_node; p_cur_node; p_cur_node = p_cur_node->next)
    {
        /*compare if node is of Type XML_TEXT_NODE */
        if (XML_ELEMENT_NODE == p_cur_node->type)
        {
            if(p_cur_node->children == OAM_NULL) {
               continue;
            }
            /*compare if the node is text type*/
            if(oam_strcmp((Char8*)p_cur_node->children->name, "text")) {
                /*compare if the node is the desired one*/
                if(!oam_strcmp((Char8*)p_cur_node->name, p_str)) {
    /* SPR 20430 Changes Start*/
                    *p_b_Node  = p_cur_node;
    /* SPR 20430 Changes End*/
                    flag_found = 1;
                    return;
                }
            }
        }

        if(!flag_found) {
           if(p_cur_node->children) {
               oam_xml_find_ptr(p_cur_node->children, p_b_Node, p_str);
           }
    }
    }
}

/******************************************************************************
* Function Name  : oam_xmlNodeGetContent 
* Description    : This function is used to return the content of a leaf node
* Inputs         : p_parent :Pointer to xmlNode type of the parent node
                 : p_node_tag_2 :Pointer to tag name of the leaf node
* Outputs        : none
* Returns        : Pointer to xmlChar
*******************************************************************************/
xmlChar * 
oam_xmlNodeGetContent
(
    xmlNode * p_parent,
    char * p_node_tag_2
)
{
    xmlChar * p_node_tag;
    static   xmlChar * p_temp=OAM_NULL;
    p_node_tag = (xmlChar *)p_node_tag_2;

    if(p_temp && OAM_ZERO != oam_strcmp((const char *)p_temp,(const char *)&OAM_INVALID_STR))
    {
        xmlFree(p_temp);
        p_temp = OAM_NULL;
    }      
     /* SPR 15386 START */  
    /*SPR 15335 Fix Start*/
    /*SPR 17569 Fix Start*/
    if ((OAM_NULL == p_node_tag) || (OAM_NULL == p_parent))
    /*SPR 17569 Fix End*/
    /*SPR 15335 Fix End*/
    {
    return (xmlChar *)&OAM_INVALID_STR;
    }
    /* SPR 15386 END */       

        
    p_temp = xmlNodeGetContent(
	oam_xml_find_tag(p_parent->children,p_node_tag));
    if(p_temp == OAM_NULL)
    {
	p_temp =(xmlChar *)&OAM_INVALID_STR;
    }	
	
    return 	p_temp;
}
/******************************************************************************
* Function Name  : oam_xml_str_to_ptr
* Description    : This function is used to copy the value from src ptr to dst 
* Inputs         : char *p_src
* Outputs        : void *p_dst 
* Returns        : void
*******************************************************************************/
Char8* 
oam_xml_str_to_ptr(Char8* p_src)
{
	Char8 *p_dst=OAM_NULL;
    size_t str_len;
 
       str_len = oam_strlen(p_src) + OAM_ONE;
       /* coverity 40886-2 */
	p_dst = (Char8*)oam_malloc(str_len);
       /* coverity 41025 */
       if(p_dst != NULL)
       {
        /* coverity 40890 */ 
       /* coverity 40886-1 */
		oam_strncpy((Char8*)p_dst, p_src, str_len);
       }
	return p_dst;
 
}

/******************************************************************************
* Function Name  : oam_xml_aggregation_power_offset_cc_user
* Description    : This function is used to populate parameters under tag 
*                  aggregation_power_offset_cc_user.
* Inputs         : pointer to prop_cellconfig_xml_st
* Outputs        : none
* Returns        : void
*******************************************************************************/

void
oam_xml_aggregation_power_offset_cc_user(prop_cellconfig_xml_st *p_cellconfig)
{
    UInt8  cqi_count = OAM_ZERO;
    UInt8  tuples_count = OAM_ZERO;
    /* SPR 20430 Changes Start*/
    xmlNode *cqi_cc = OAM_NULL;
    /* SPR 20430 Changes End*/
    UInt8 count = OAM_ZERO;
    /* SPR 20430 Changes Start*/
    xmlNode *temp1 = OAM_NULL;

    xmlNode *tuples = OAM_NULL;
    /* SPR 20430 Changes End*/
    UInt32 tuples_count_limit = OAM_ZERO;
    OAM_FUN_ENTRY_TRACE();
    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_cellconfig->pdcch_aggregation_power_offset,&temp1,"aggregation_power_offset_cc_user");
 
    for( count = OAM_ZERO; ((count < RRM_OAM_NUM_DCI_SIZE_CATEGORY) 
                    && ((xmlNode *)temp1 != OAM_NULL)) ;  )
    {
        if( ((xmlNode *)temp1)->type != XML_COMMENT_NODE )
        {
            if(!oam_strcmp((const char *)(((xmlNode *)temp1)->name),"aggregation_power_offset_cc_user" ))
            {
                p_cellconfig->aggregation_power_offset_cc_user_dci[count] = (xmlNode*)temp1;

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(p_cellconfig->aggregation_power_offset_cc_user_dci[count],&cqi_cc,"aggregation_power_offset_user");

                for( cqi_count = OAM_ZERO ; ((cqi_count < RRM_OAM_MAX_CQI_VALUE ) && ((xmlNode *)cqi_cc != OAM_NULL)); )
                {
            
                    if(( ((xmlNode *)cqi_cc)->type != XML_COMMENT_NODE)  &&
                        (!oam_strcmp((const char *)(((xmlNode *)cqi_cc)->name),"aggregation_power_offset_user" )) )
                    {
                         p_cellconfig->aggregation_power_offset_cc_user_cqi[count][cqi_count] = (xmlNode*)cqi_cc;

		         flag_found = OAM_ZERO;
	                 oam_xml_find_ptr(p_cellconfig->aggregation_power_offset_cc_user_cqi[count][cqi_count] ,&tuples,"aggr_pwr_offset_tuples");

                         tuples_count_limit = oam_atoi((const char *)oam_xmlNodeGetContent(
                         p_cellconfig->aggregation_power_offset_cc_user_cqi[count][cqi_count],"count"));

                         for(tuples_count = OAM_ZERO ; ((tuples_count < RRM_OAM_MAX_AGGREGATION_LEVEL_POWER_OFFSET_COUNT ) && 
                                                 ((xmlNode *)tuples != OAM_NULL) && (tuples_count < tuples_count_limit )) ; )
                         {
                             if(( ((xmlNode *)tuples)->type != XML_COMMENT_NODE)&&
                                  (!oam_strcmp((const char *)(((xmlNode *)tuples)->name),"aggr_pwr_offset_tuples" )))
                             {
                                 p_cellconfig->aggr_pwr_offset_tuples_cc_user[count][cqi_count][tuples_count] = (xmlNode*)tuples;
                                 tuples_count++;
                             }

    /* SPR 20430 Changes Start*/
                             tuples = (((xmlNode *)tuples)->next);
    /* SPR 20430 Changes End*/
                         }
                          cqi_count++;
 
                     }
                     
    /* SPR 20430 Changes Start*/
                     cqi_cc = (((xmlNode *)cqi_cc)->next);
    /* SPR 20430 Changes End*/
                 }
                 count++;
             }


         }

    /* SPR 20430 Changes Start*/
         temp1 = (((xmlNode *)temp1)->next);
    /* SPR 20430 Changes End*/

    }
    OAM_FUN_EXIT_TRACE();
}

/******************************************************************************
* Function Name  : oam_xml_aggregation_power_offset_ce_user
* Description    : This function is used to populate parameters under tag
*                  aggregation_power_offset_ce_user.
* Inputs         : pointer to prop_cellconfig_xml_st
* Outputs        : none
* Returns        : void
*******************************************************************************/

void
oam_xml_aggregation_power_offset_ce_user(prop_cellconfig_xml_st *p_cellconfig)
{
    UInt8  cqi_count = OAM_ZERO;
    UInt8  tuples_count = OAM_ZERO;
    /* SPR 20430 Changes Start*/
    xmlNode *cqi_cc = OAM_NULL;
    /* SPR 20430 Changes End*/
     OAM_FUN_ENTRY_TRACE();
    /* SPR 20430 Changes Start*/
    xmlNode *tuples = OAM_NULL;
    /* SPR 20430 Changes End*/
    UInt32 tuples_count_limit = OAM_ZERO;
    UInt8 count = OAM_ZERO;
    /* SPR 20430 Changes Start*/
    xmlNode *temp1 = OAM_NULL;
    /* SPR 20430 Changes End*/

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_cellconfig->pdcch_aggregation_power_offset,&temp1,"aggregation_power_offset_ce_user");

    for( count = OAM_ZERO; ((count < RRM_OAM_NUM_DCI_SIZE_CATEGORY) 
                                                       && ((xmlNode *)temp1 != OAM_NULL)) ;  )
    {
        if( ((xmlNode *)temp1)->type != XML_COMMENT_NODE )
        {
            if(!oam_strcmp((const char *)(((xmlNode *)temp1)->name),"aggregation_power_offset_ce_user" ))
            {
                p_cellconfig->aggregation_power_offset_ce_user_dci[count] = (xmlNode*)temp1;

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(p_cellconfig->aggregation_power_offset_ce_user_dci[count],&cqi_cc,"aggregation_power_offset_user");

                for( cqi_count = OAM_ZERO ; ((cqi_count < RRM_OAM_MAX_CQI_VALUE ) && ((xmlNode *)cqi_cc != OAM_NULL)); )
                {
            
                    if(( ((xmlNode *)cqi_cc)->type != XML_COMMENT_NODE)  &&
                        (!oam_strcmp((const char *)(((xmlNode *)cqi_cc)->name),"aggregation_power_offset_user" )) )
                    {
                         p_cellconfig->aggregation_power_offset_ce_user_cqi[count][cqi_count] = (xmlNode*)cqi_cc;

		         flag_found = OAM_ZERO;
	                 oam_xml_find_ptr(p_cellconfig->aggregation_power_offset_ce_user_cqi[count][cqi_count] ,&tuples,"aggr_pwr_offset_tuples");

                         tuples_count_limit = oam_atoi((const char *)oam_xmlNodeGetContent(
                         p_cellconfig->aggregation_power_offset_ce_user_cqi[count][cqi_count],"count"));

                         for(tuples_count = OAM_ZERO ; ((tuples_count < RRM_OAM_MAX_AGGREGATION_LEVEL_POWER_OFFSET_COUNT ) && 
                                                 ((xmlNode *)tuples != OAM_NULL) && (tuples_count < tuples_count_limit )) ; )
                         {
                             if(( ((xmlNode *)tuples)->type != XML_COMMENT_NODE)&&
                                  (!oam_strcmp((const char *)(((xmlNode *)tuples)->name),"aggr_pwr_offset_tuples" )))
                             {
                                 p_cellconfig->aggr_pwr_offset_tuples_ce_user[count][cqi_count][tuples_count] = (xmlNode*)tuples;
                                 tuples_count++;
                             }

    /* SPR 20430 Changes Start*/
                             tuples = (((xmlNode *)tuples)->next);
    /* SPR 20430 Changes End*/
                         }
                         cqi_count++;
                      }
                     
    /* SPR 20430 Changes Start*/
                     cqi_cc = (((xmlNode *)cqi_cc)->next);
    /* SPR 20430 Changes End*/
                 }
                 count++;
             }


         }

    /* SPR 20430 Changes Start*/
         temp1 = (((xmlNode *)temp1)->next);
    /* SPR 20430 Changes End*/

    }
	 OAM_FUN_EXIT_TRACE();
}

/* TDD params chnage END */


/*******************************************************************************
* Function Name  : oam_xml_struct_populate()
* Description    : This function is used to populate the global structure of
                 : xml_struct.
* Inputs         : p_node :xmlNode type  pointer of the parent node
* Outputs        : none
* Returns        : OAM_SUCCESS/OAM_FAILURE
*******************************************************************************/

oam_return_et
oam_xml_struct_populate(xmlNode  *p_node)
{
   OAM_FUN_ENTRY_TRACE();
    static int first_time = OAM_FALSE;//TRUE;
/* SPR 11001 FIX START */
    /* SPR 20430 Changes Start*/
    xmlNode *temp1 = OAM_NULL;
    xmlNode *temp2 = OAM_NULL;
    /* SPR 20430 Changes End*/
    UInt32 index = OAM_ZERO;
/* SPR 11001 FIX END */
    /* coverity 52776 52760*/
    SInt32 count= OAM_ZERO;
    /* SPR 20653 Fix Start */
    SInt32 count_rat= OAM_ZERO;
    SInt32 count_freq= OAM_ZERO;
    /* SPR 20653 Fix End */
    UInt8 cell_idx = 0;
    xmlNode *p_cell_config_tag = OAM_NULL;
    xmlNode *p_parent = OAM_NULL;
    UInt8 cell_num_entries = oam_prov_req.num_cells_configured;
    Char8 *tag_value = OAM_NULL;
    SInt32 num_entries = OAM_ZERO;
    /* SPR 20653 Fix Start */
    SInt32 num_entries_rat = OAM_ZERO;
    SInt32 num_entries_freq = OAM_ZERO;
    xmlNode *temp3 = OAM_NULL;
    /* SPR 20653 Fix End */
/* SPR 22248 MBMS Changes Start */
/* SPR 22248 MBMS Changes Stop */
#ifdef LTE_EMBMS_SUPPORTED
    xmlNode *p_result_node = OAM_NULL;
#endif

    if(first_time == OAM_TRUE)
    {
       //intializes xml_struct_g with 0
       oam_memset(&xml_struct_g,OAM_ZERO,sizeof(xml_struct_g));
       first_time = OAM_FALSE;
    }

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"eNodeBParams");
    /*SPR_16744_START*/
    if(flag_found)
    {
        xml_struct_g.eNodeBParams = (xmlNode*)temp1;
    }

#ifdef OAM_SHARED_MEM_LOGGER
    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"oam_shm_logger_cfg");
    if(flag_found)
    {
        xml_struct_g.oam_shared_mem_config = (xmlNode*)temp1;
		oam_prov_req.oam_igd_params.oam_shared_mem_conf.is_shared_mem_conf_present = OAM_TRUE;
    }
    else
    {
		oam_prov_req.oam_igd_params.oam_shared_mem_conf.is_shared_mem_conf_present = OAM_FALSE;
    }
#endif

    flag_found = OAM_ZERO;
/* SPR 11001 FIX START */
    oam_xml_find_ptr(p_node, &temp1, "load_configuration");

    if(flag_found) {
        xml_struct_g.load_configuration = (xmlNode*)temp1;

        tag_value =
              (Char8*)oam_xmlNodeGetContent(xml_struct_g.load_configuration,
                                            "Enable");

        if(!oam_strcmp(tag_value, OAM_INVALID_STR)) {
            OAM_LOG(OAM, OAM_WARNING, "Load config enable node not found, "
                         "assuming default: Disabled");
            RRM_PROV_REQ.enable_load_config = 0;
        }
        else {
            RRM_PROV_REQ.enable_load_config = oam_atoi(tag_value);
        }
    }
    else {
        OAM_LOG(OAM, OAM_WARNING, "Load configuration parameters not found,"
                     " assuming default: Disabled");
        RRM_PROV_REQ.enable_load_config = 0;
    }
/* SPR 11001 FIX END */


#ifdef ENDC_ENABLED
    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"EN_DC_Params");

    if(flag_found)
    {
        xml_struct_g.EN_DC_Params = (xmlNode*)temp1;
    }
    else
    {
        OAM_LOG(OAM , OAM_WARNING, "EN_DC_Param Tag is missing ."
                "EN_DC Config Parameters are not available ");
    }

#if 1
    flag_found = OAM_ZERO;

    UInt32 temp_peer_gnb_list_size = OAM_ZERO;

    temp_peer_gnb_list_size = oam_atoi((const char *)oam_xmlNodeGetContent(
                xml_struct_g.EN_DC_Params,"peer_gnb_pci_list_size"));
    OAM_LOG(OAM , OAM_ERROR, "peer_gnb_pci_list size %d"
                "peer_gnb_pci_list ",temp_peer_gnb_list_size);

    if ( MAX_PEER_GNB < temp_peer_gnb_list_size )
    {
        OAM_LOG(OAM, OAM_ERROR,"Number of gNB pci list[%d] "
        "exceed the max limit", temp_peer_gnb_list_size);
        oam_handle_invalid_startup_config();
    }

    oam_xml_find_ptr(xml_struct_g.EN_DC_Params,&temp1,"peer_pci_info");
    if ( flag_found )
    {
        for ( count= OAM_ZERO; count < (SInt32)temp_peer_gnb_list_size &&
            OAM_NULL != (xmlNode *)temp1; count++ )
        {
            xml_struct_g.peer_pci_info[count] = (xmlNode*)temp1;
            temp1 = (((xmlNode *)temp1)->next);
        }
    }
    if (count < (SInt32)temp_peer_gnb_list_size)
    {
        OAM_LOG(OAM, OAM_ERROR, "Value of num Peer gNB list size:%d is greater than"
                "number of peer_pci_info nodes:%d in file",
                temp_peer_gnb_list_size, count);
        oam_handle_invalid_startup_config();
    }
#endif
#endif
    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"CellConfig");

    p_cell_config_tag = (xmlNode*)temp1;
    if(flag_found)
    {
        while((cell_idx < cell_num_entries) &&
                flag_found && p_cell_config_tag) {
            if(p_cell_config_tag->type != XML_COMMENT_NODE) {
                flag_found = OAM_ZERO;
                oam_xml_find_ptr(p_cell_config_tag, &temp1, "scheduler_Params");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].scheduler_Params =
                        (xmlNode*)temp1;
                }

                /* SPR 18641_18843 fix start */
                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].scheduler_Params, 
                                  &temp1, 
                                  "tti_bundling_info_params");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].tti_bundling_info_params = (xmlNode*)temp1;
                }
                /* SPR 18641_18843 fix end */


                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].
                        scheduler_Params, &temp1,
                        "bucket_management_params");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].bucket_management_params =
                        (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].
                        scheduler_Params, &temp1,
                        "Scheduling_Weights_Params");
               if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].Scheduling_Weights_Params =
                                                              (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].
                        Scheduling_Weights_Params, &temp1,"DL");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].Scheduling_Weights_Params_DL =
                        (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].
                        Scheduling_Weights_Params, &temp1,"UL");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].Scheduling_Weights_Params_UL =
                        (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].
                        scheduler_Params, &temp1,
                        "Scheduling_Bit_Rate_Factor");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].Scheduling_Bit_Rate_Factor =
                        (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].
                        scheduler_Params, &temp1, "No_Of_UEs");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].No_Of_UEs = (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].
                        scheduler_Params, &temp1,
                        "ControlChennelConfiguration");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].ControlChennelConfiguration =
                        (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].
                        ControlChennelConfiguration,
                        &temp1, "preferredPrimaryDCIFormatForSingleTB");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].preferredPrimaryDCIFormatForSingleTB =
                        (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].
                        scheduler_Params, &temp1,
                        "additional_packet_scheduling_params");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].
                        additional_packet_scheduling_params = (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].
                        scheduler_Params, &temp1,
                        "rrm_ue_scheduling_info");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].rrm_ue_scheduling_info =
                        (xmlNode*)temp1;
                }
                /* SPR 13768 Changes Start */
                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].
                        scheduler_Params, &temp1,
                        "MAC_Params");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].MAC_Params =
                        (xmlNode*)temp1;
                }
                /* SPR 13768 Changes End */

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(p_cell_config_tag, &temp1, "dimension_params");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].dimension_params =
                        (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].
                        dimension_params, &temp1, "RNTI");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].RNTI = (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].RNTI,
                        &temp1, "RArnti");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].RArnti = (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].RNTI,
                        &temp1, "sps_crnti");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].sps_crnti = (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].RNTI, &temp1,
                        "tpc_rnti");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].tpc_rnti = (xmlNode*)temp1;
                }
                /* SPR 20653 Fix Start */
                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].dimension_params, 
                                  &temp1, 
                                  "mac_periodic");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].mac_periodic = (xmlNode*)temp1;
                }
                /* SPR 20653 Fix End */

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(p_cell_config_tag, &temp1,
                        "measurement_configuration");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].measurement_configuration = (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].
                        measurement_configuration,
                        &temp1, "csfb_tgt_selection");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].csfb_tgt_selection =
                        (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(p_cell_config_tag, &temp1, "ho_configuration");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].ho_configuration =
                        (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].
                        ho_configuration, &temp1, "ho_retry_params");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].ho_retry_params =
                        (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].
                        ho_configuration, &temp1, "ho_algo_params");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].ho_algo_params =
                        (xmlNode*)temp1;
                }
                /* SPR 20653 Fix Start */
                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].ho_configuration, 
                                  &temp1, 
                                  "ho_oscillation_params");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].ho_oscillation_params = (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].ho_configuration, 
                                  &temp1, 
                                  "ho_block_cell_params");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].ho_block_cell_params = (xmlNode*)temp1;
                }
                /* SPR 20653 Fix End */

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].
                        ho_configuration, &temp1,
                        "target_cell_selection_params");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].target_cell_selection_params =
                        (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].
                        ho_configuration, &temp1, "spid_table");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].spid_table = (xmlNode*)temp1;
                }
                /* SPR 20653 Fix Start */
                /* SPR 21088 Fix Start */
                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].
                        spid_table, &temp1, "default_rat_priority_info");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].default_rat_priority_info = (xmlNode*)temp1;
                }
                /* SPR 21088 Fix End */
                /* SPR 20653 Fix End */

                tag_value = (Char8*)oam_xmlNodeGetContent(
                        xml_struct_g.CellConfig[cell_idx].spid_table,
                        "spid_count");
                num_entries = oam_atoi(tag_value);

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].spid_table,
                        &temp1, "spid_config");

                count = 0;
                if(flag_found){
                    while(count < num_entries && temp1) {
                        if(((xmlNode*)temp1)->type != XML_COMMENT_NODE) {
                            /* SPR 20653 Fix Start */
                            xml_struct_g.CellConfig[cell_idx].spid_config[count].p_spid_config =
                                (xmlNode*)temp1;

                            tag_value = (Char8*)oam_xmlNodeGetContent(
                                    xml_struct_g.CellConfig[cell_idx].spid_config[count].p_spid_config,
                                    "rat_count");

                            num_entries_rat = oam_atoi(tag_value);

                            flag_found = OAM_ZERO;
                            oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].spid_config[count].p_spid_config,
                                    &temp2, "rat_priority");

                            count_rat = 0;
                            if(flag_found){
                                while(count_rat < num_entries_rat && temp2) {
                                    if(((xmlNode*)temp2)->type != XML_COMMENT_NODE) {
                                        xml_struct_g.CellConfig[cell_idx].spid_config[count].rat_priority[count_rat].rat_priority =
                                            (xmlNode*)temp2;

                                        tag_value = (Char8*)oam_xmlNodeGetContent(
                                                xml_struct_g.CellConfig[cell_idx].spid_config[count].rat_priority[count_rat].rat_priority,
                                                "freq_count");

                                        num_entries_freq = oam_atoi(tag_value);

                                        flag_found = OAM_ZERO;
                                        oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].spid_config[count].rat_priority[count_rat].rat_priority,
                                                &temp3, "freq_priority");

                                        count_freq = 0;
                                        if(flag_found){
                                            while(count_freq < num_entries_freq && temp3) {
                                                if(((xmlNode*)temp3)->type != XML_COMMENT_NODE) {
                                                    xml_struct_g.CellConfig[cell_idx].spid_config[count].rat_priority[count_rat].freq_priority[count_freq++].freq_priority =
                                                        (xmlNode*)temp3;
                                                }
                                                temp3 = (((xmlNode*)temp3)->next);
                                            }
                                        }
                                        count_rat++;
                                    }
                                    temp2 = (((xmlNode*)temp2)->next);
                                }
                            }

                            count++;
                            /* SPR 20653 Fix End */
                        }

                        /* SPR 20430 Changes Start*/
                        temp1 = (((xmlNode*)temp1)->next);
                        /* SPR 20430 Changes End*/
                    }
                }


                flag_found = OAM_ZERO;
                oam_xml_find_ptr(p_cell_config_tag, &temp1,
                        "admission_control_info");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].admission_control_info =
                        (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].
                        admission_control_info,
                        &temp1, "available_gbr_limit");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].available_gbr_limit =
                        (xmlNode*)temp1;
                }
                /* SPR 20653 Fix Start */
                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].
                        admission_control_info, &temp1, "prb_estimation_factor");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].prb_estimation_factor = (xmlNode*)temp1;
                }
                /* SPR 20653 Fix End */

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(p_cell_config_tag, &temp1,
                        "ue_generic_cdma2000_params");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].ue_generic_cdma2000_params =
                        (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(p_cell_config_tag, &temp1, "power_control");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].power_control =
                        (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].power_control,
                        &temp1, "power_control_enable");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].power_control_enable =
                        (xmlNode*)temp1;
                }

                /* SPR 20653 Fix Start */
                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].
                        power_control, &temp1, "uplink_power_control_dedicated");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].uplink_power_control_dedicated = (xmlNode*)temp1;
                }
                /* SPR 20653 Fix End */
                /* SPR 13976 FIX Start */
                /* code removed */
                /* SPR 13976 FIX End */

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(p_cell_config_tag, &temp1,
                        "ADDITIONAL_ICIC_params");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].ADDITIONAL_ICIC_params =
                        (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                p_parent =
                    xml_struct_g.CellConfig[cell_idx].ADDITIONAL_ICIC_params;
                oam_xml_find_ptr(p_parent, &temp1,
                        "pdcch_aggregation_power_offset");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].
                        pdcch_aggregation_power_offset = (xmlNode*)temp1;
                }

                oam_xml_aggregation_power_offset_cc_user(
                        &xml_struct_g.CellConfig[cell_idx]);
                oam_xml_aggregation_power_offset_ce_user(
                        &xml_struct_g.CellConfig[cell_idx]);

                flag_found = OAM_ZERO;
                p_parent =
                    xml_struct_g.CellConfig[cell_idx].ADDITIONAL_ICIC_params;
                oam_xml_find_ptr(p_parent, &temp1, "cqi_to_phich_power");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].cqi_to_phich_power =
                        (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                p_parent =
                    xml_struct_g.CellConfig[cell_idx].ADDITIONAL_ICIC_params;
                oam_xml_find_ptr(p_parent, &temp1,
                        "dynamic_cfi_extension_params");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].dynamic_cfi_extension_params =
                        (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(p_cell_config_tag, &temp1, "sps_params");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].sps_params = (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                p_parent = xml_struct_g.CellConfig[cell_idx].sps_params;

                oam_xml_find_ptr(p_parent, &temp2, "spsCommIntervalProfile");

                count = 0;
                if(flag_found){
                    while(count < SPS_INTERVAL_LIST && temp2) {
                        if(((xmlNode*)temp2)->type != XML_COMMENT_NODE &&
                                !oam_strcmp((Char8*)(((xmlNode*)temp2)->name),
                                    "spsCommIntervalProfile")) {
                            xml_struct_g.CellConfig[cell_idx].
                                spsCommIntervalProfile[count] = (xmlNode*)temp2;
                            flag_found = OAM_ZERO;
                            p_parent = xml_struct_g.CellConfig[cell_idx].
                                spsCommIntervalProfile[count];
                            oam_xml_find_ptr(p_parent, &temp1, "maxPdcpPduSizeIpv4");
                            if(flag_found)
                            {
                            xml_struct_g.CellConfig[cell_idx].
                                maxPdcpPduSizeIpv4[count] = (xmlNode*)temp1;
                            }

                            flag_found = OAM_ZERO;
                            oam_xml_find_ptr(p_parent, &temp1, "maxPdcpPduSizeIpv6");
                            if(flag_found)
                            {
                            xml_struct_g.CellConfig[cell_idx].
                                maxPdcpPduSizeIpv6[count] = (xmlNode*)temp1;
                            }
                            ++count;
                        }

    /* SPR 20430 Changes Start*/
                        temp2 = (((xmlNode *)temp2)->next);
    /* SPR 20430 Changes End*/
                    }
                }

                flag_found = OAM_ZERO;
                p_parent = xml_struct_g.CellConfig[cell_idx].sps_params;
                oam_xml_find_ptr(p_parent, &temp1, "supportedDlIntervalList");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].supportedDlIntervalList =
                        (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(p_parent, &temp1, "supportedUlIntervalList");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].supportedUlIntervalList =
                        (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(p_parent, &temp1, "spsDLIntervalProfile");

                count = 0;
                if(flag_found){
                    while(count < SPS_INTERVAL_LIST && temp1) {
                        if(((xmlNode*)temp1)->type != XML_COMMENT_NODE &&
                                !oam_strcmp((Char8*)(((xmlNode*)temp1)->name),
                                    "spsDLIntervalProfile")) {
                            xml_struct_g.CellConfig[cell_idx].
                                spsDLIntervalProfile[count++] = (xmlNode*)temp1;
                        }

    /* SPR 20430 Changes Start*/
                        temp1 = (((xmlNode *)temp1)->next);
    /* SPR 20430 Changes End*/
                    }
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(p_parent, &temp1, "spsULIntervalProfile");

                count = 0;
                if(flag_found){
                    while(count < SPS_INTERVAL_LIST && temp1) {
                        if(((xmlNode*)temp1)->type != XML_COMMENT_NODE &&
                                !oam_strcmp((Char8*)(((xmlNode*)temp1)->name),
                                    "spsULIntervalProfile")) {
                            xml_struct_g.CellConfig[cell_idx].
                                spsULIntervalProfile[count++] = (xmlNode*)temp1;
                        }

    /* SPR 20430 Changes Start*/
                        temp1 = (((xmlNode *)temp1)->next);
    /* SPR 20430 Changes End*/
                    }
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(p_parent, &temp1,
                        "sps_dl_scheduling_Info_per_tti");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].
                        sps_dl_scheduling_Info_per_tti = (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(p_parent, &temp1,
                        "sps_ul_scheduling_Info_per_tti");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].
                        sps_ul_scheduling_Info_per_tti = (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(p_cell_config_tag, &temp1, "mimo_mode_params");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].mimo_mode_params =
                        (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                p_parent = xml_struct_g.CellConfig[cell_idx].mimo_mode_params;
                oam_xml_find_ptr(p_parent, &temp1,
                        "mimo_codebook_subset_restriction");
                count = 0;
                if(flag_found){
                    while((count < RRM_OAM_MAX_CODEBOOK_SUBSET) && temp1) {
                        if(((xmlNode*)temp1)->type != XML_COMMENT_NODE) {
                            if(!oam_strcmp((Char8*)(((xmlNode*)temp1)->name),
                                        "mimo_codebook_subset_restriction")) {
                                xml_struct_g.CellConfig[cell_idx].
                                    mimo_codebook_subset_restriction[count++] =
                                    (xmlNode*)temp1;
                            }
                        }
    /* SPR 20430 Changes Start*/
                        temp1 = (((xmlNode *)temp1)->next);
    /* SPR 20430 Changes End*/
                    }
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(p_parent, &temp1,
                        "mimo_transmission_mode_table");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].mimo_transmission_mode_table =
                        (xmlNode*)temp1;
                }

                /* SPR 20653 Fix Start */
                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].mimo_mode_params, 
                                  &temp1, 
                                  "ul_mimo_mode_params");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].ul_mimo_mode_params = (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].mimo_mode_params, 
                                  &temp1, 
                                  "tm_mode_additional_info");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].tm_mode_additional_info = (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].tm_mode_additional_info, 
                                  &temp1, 
                                  "beam_forming_vector");
                if(flag_found)
                {
                    xml_struct_g.CellConfig[cell_idx].beam_forming_vector = (xmlNode*)temp1;
                }
                /* SPR 20653 Fix End */
                flag_found = OAM_ZERO;
                p_parent =
                    xml_struct_g.CellConfig[cell_idx].mimo_transmission_mode_table;
                oam_xml_find_ptr(p_parent, &temp1, "rrm_oam_tm_mode_table_row");
                count = OAM_ZERO;
                if(flag_found){
                    while((count < RRM_OAM_MAX_TM_MODE_TABLE_ROW) && temp1) {
                        if(((xmlNode*)temp1)->type != XML_COMMENT_NODE) {
                            if(!oam_strcmp((Char8*)(((xmlNode*)temp1)->name),
                                        "rrm_oam_tm_mode_table_row")) {
                                xml_struct_g.CellConfig[cell_idx].
                                    rrm_oam_tm_mode_table_row[count++] = (xmlNode*)temp1;
                            }
                        }
    /* SPR 20430 Changes Start*/
                        temp1 = (((xmlNode *)temp1)->next);
    /* SPR 20430 Changes End*/
                    }
                }

                /* SPR 11001 FIX START */
                if (RRM_PROV_REQ.enable_load_config) {
                    flag_found = OAM_ZERO;
                    oam_xml_find_ptr(p_cell_config_tag, &temp1, "load_config");

                    if (!flag_found) {
                        OAM_LOG(OAM, OAM_ERROR,
                                "Load configuration is enabled but no "
                                "cell load parameters are found");

                        OAM_FUN_EXIT_TRACE();
                        return OAM_FAILURE;
                    }

                    xml_struct_g.CellConfig[cell_idx].
                        load_config = (xmlNode*)temp1;


                    tag_value = (Char8*)oam_xmlNodeGetContent(
                            xml_struct_g.CellConfig[cell_idx].load_config,
                            "load_level_num_entries");

                    if (!oam_strcmp(tag_value, OAM_INVALID_STR)) {
                        OAM_LOG(OAM, OAM_ERROR,
                                "Load configuration is enabled but unable "
                                "to read tag load_level_num_entries");

                        OAM_FUN_EXIT_TRACE();
                        return OAM_FAILURE;
                    }

                    num_entries = oam_atoi(tag_value);

                    if (num_entries > MAX_LOAD_LEVEL_NUM_ENTRIES) {
                        OAM_LOG(OAM, OAM_ERROR,
                                "No more than %d load_level_info allowed",
                                num_entries);
                        OAM_FUN_EXIT_TRACE();
                        return OAM_FAILURE;
                    }

                    index = 0;
                    flag_found = OAM_ZERO;
                    oam_xml_find_ptr(
                            xml_struct_g.CellConfig[cell_idx].load_config,
                            &temp1, "load_level_info");

                    if (num_entries && !flag_found) {
                        OAM_LOG(OAM, OAM_ERROR,
                                "Expected node load_level_info[%d] not found",
                                index);
                        OAM_FUN_EXIT_TRACE();
                        return OAM_FAILURE;
                    }

                    if(flag_found){
                        for ( ; (SInt64)index < num_entries; ++index) {
                            xml_struct_g.CellConfig[cell_idx].
                                load_level_info[index] = (xmlNode*)temp1;

                            flag_found = OAM_ZERO;
                            oam_xml_find_ptr(
                                    xml_struct_g.CellConfig[cell_idx].load_level_info[index],
                                    &temp1, "load_access_class_barring_config");

                            if (flag_found) {
                                xml_struct_g.CellConfig[cell_idx].
                                    load_access_class_barring_config[index] =
                                    (xmlNode*)temp1;

                                flag_found = OAM_ZERO;
                                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].
                                        load_access_class_barring_config[index],
                                        &temp1, "acBarringForMoSignalling");

                                if (flag_found) {
                                    xml_struct_g.CellConfig[cell_idx].
                                        acBarringForMoSignalling[index] =
                                        (xmlNode*)temp1;
                                }

                                flag_found = OAM_ZERO;
                                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].
                                        load_access_class_barring_config[index],
                                        &temp1, "acBarringForMoData");
                                if (flag_found) {
                                    xml_struct_g.CellConfig[cell_idx].
                                        acBarringForMoData[index] = (xmlNode*)temp1;
                                }

                                flag_found = OAM_ZERO;
                                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].
                                        load_access_class_barring_config[index],
                                        &temp1, "ssac_barring_for_mmtel_voice_r9");

                                if (flag_found) {
                                    xml_struct_g.CellConfig[cell_idx].
                                        ssac_barring_for_mmtel_voice_r9[index] =
                                        (xmlNode*)temp1;
                                }

                                flag_found = OAM_ZERO;
                                oam_xml_find_ptr(xml_struct_g.CellConfig[cell_idx].
                                        load_access_class_barring_config[index],
                                        &temp1, "ssac_barring_for_mmtel_video_r9");

                                if (flag_found) {
                                    xml_struct_g.CellConfig[cell_idx].
                                        ssac_barring_for_mmtel_video_r9[index] =
                                        (xmlNode*)temp1;
                                }
                            }
                            /* eICIC_PHASE_1_2_CHANGES_START*/ 

                            flag_found = OAM_ZERO;
                            oam_xml_find_ptr(
                                    xml_struct_g.CellConfig[cell_idx].load_level_info[index],
                                    &temp1, "eicic_load_config");

                            if (flag_found) {
                                xml_struct_g.CellConfig[cell_idx].
                                    eicic_load_config[index] =
                                    (xmlNode*)temp1;
                            }
                            /*eICIC_PHASE_1_2_CHANGES_END */ 

    /* SPR 20430 Changes Start*/
                            temp1 = (xml_struct_g.CellConfig[cell_idx].
                                    load_level_info[index])->next;
    /* SPR 20430 Changes End*/
                        }
                    }
                }

                /*SPR 12228 FIX START*/
                /*Redundant code removed */
                /*SPR 12228 FIX END*/
            }
            /* SPR 11001 FIX END */

            p_cell_config_tag = p_cell_config_tag->next;
            ++cell_idx;
        }
    }

/*SPR 21889 +- */

/* SPR 11001 FIX START */
    /* CODE MOVED    */
/* SPR 11001 FIX END */

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"health_monitoring");
    if(flag_found)
    {
    xml_struct_g.health_monitoring = (xmlNode*)temp1;
    }
 
    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"rach_opt_params");
    if(flag_found)
    {
    xml_struct_g.rach_opt_params = (xmlNode*)temp1;
    }

/*SPR 21889 Start*/
    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"eNB_Process_Table");
    if(flag_found)
    {
        oam_populate_process_tbl(temp1);
    }
    else
    {
        OAM_LOG(OAM, OAM_ERROR,"eNB_Process_Table tag not found. Mandatory for system startupd");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
/*SPR 21889 End */

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"stack_config");
    if(flag_found)
    {
    xml_struct_g.stack_config = (xmlNode*)temp1;
    }

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"L1params");
    if(flag_found)
    {
    xml_struct_g.L1params = (xmlNode*)temp1;
    }
    
    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"L1_Power_Control_param");
    if(flag_found)
    {
    xml_struct_g.L1_Power_Control_param = (xmlNode*)temp1;
    }

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"L2params");
    if(flag_found)
    {
    xml_struct_g.L2params = (xmlNode*)temp1;
    }

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"RLCParams");
    if(flag_found)
    {
    xml_struct_g.RLCParams = (xmlNode*)temp1;
    }

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"RLC_Q_Management");
    if(flag_found)
    {
    xml_struct_g.RLC_Q_Management = (xmlNode*)temp1;
    }

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"MACParams");
    if(flag_found)
    {
    xml_struct_g.MACParams = (xmlNode*)temp1;
    }

    /* EMBMS Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
     p_result_node = oam_get_xml_nonleaf_child_node(xml_struct_g.L2params, "EGTPUParams");
    if (p_result_node)
    {
       xml_struct_g.EGTPUParams = p_result_node;
       OAM_LOG(OAM, OAM_DETAILED, "Successfully read node EGTPUParams");
    }
    else
    {
        OAM_LOG(OAM, OAM_ERROR, "Failed to read node EGTPUParams");
    }
#endif
    /* EMBMS Changes End */

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node, &temp1, "L3Params");
    if(flag_found) {
        xml_struct_g.L3Params = (xmlNode*)temp1;
    }
    else {
        OAM_LOG(OAM, OAM_ERROR, "Mandatory tag L3Params not found");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node, &temp1, "shared_mem_logging_config");
    if(flag_found) {
        xml_struct_g.shared_mem_logging_config = (xmlNode*)temp1;
    }
    else {
        OAM_LOG(OAM, OAM_ERROR,
                "Mandatory tag shared_mem_logging_config not found");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node, &temp1, "RRCparams");
    if(flag_found) {
        xml_struct_g.RRCparams = (xmlNode*)temp1;
    }
    else {
        OAM_LOG(OAM, OAM_ERROR, "Mandatory tag RRCparams not found");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node, &temp1, "Cell_Traffic_Trace_feature");
    if(flag_found) {
        xml_struct_g.Cell_Traffic_Trace_feature = (xmlNode*)temp1;
    }
    else {
        OAM_LOG(OAM, OAM_ERROR,
                "Mandatory tag Cell_Traffic_Trace_feature not found");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(xml_struct_g.RRCparams, &temp1, "guard_timers");
    if(flag_found) {
        xml_struct_g.RRCparams_guard_timers = (xmlNode*)temp1;
    }
    else {
        OAM_LOG(OAM, OAM_ERROR,
                "Mandatory tag guard_timers not found");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(xml_struct_g.RRCparams, &temp1, "procedural_timers");
    if(flag_found) {
        xml_struct_g.RRCparams_procedural_timers = (xmlNode*)temp1;
    }
    else {
        OAM_LOG(OAM, OAM_ERROR,
                "Mandatory tag procedural_timers not found");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"S1AP");
    if(flag_found)
    {
    xml_struct_g.S1AP = (xmlNode*)temp1;
    }

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"eNB_CommInfo");
    if(flag_found)
    {
    xml_struct_g.eNB_CommInfo = (xmlNode*)temp1;
    }

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(xml_struct_g.S1AP,&temp1,"procedural_timers");
    if(flag_found)
    {
    xml_struct_g.S1AP_procedural_timers = (xmlNode*)temp1;
    }

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node, &temp1, "X2AP");
    if(flag_found) {
        xml_struct_g.X2AP = (xmlNode*)temp1;
    }
    else {
        OAM_LOG(OAM, OAM_ERROR,
                "Mandatory tag X2AP not found");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
    /* EMBMS Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
   p_result_node = oam_get_xml_nonleaf_child_node(xml_struct_g.L3Params, "M2AP");
    if (p_result_node)
    {
       xml_struct_g.M2AP = p_result_node;
       OAM_LOG(OAM, OAM_DETAILED, "Successfully read node M2AP");
    }
    else
    {
        OAM_LOG(OAM, OAM_ERROR, "Failed to read node M2AP");
    }
#endif
    /* EMBMS Changes End */

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"local_eNB_CommInfo");
    if(flag_found){
    xml_struct_g.local_eNB_CommInfo= (xmlNode*)temp1;
    }

#ifdef OAM_SON_ENABLE
    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"son_comm_info");
    if(flag_found){
    xml_struct_g.son_comm_info= (xmlNode*)temp1;
    }
//#else //rm for support x2ap

    flag_found = OAM_ZERO;
    UInt32 temp_num_peer_enb = OAM_ZERO;
    temp_num_peer_enb = oam_atoi((const char *)oam_xmlNodeGetContent(
            xml_struct_g.X2AP,"numPeereNB"));
    if (MAX_PEER_ENB < temp_num_peer_enb)
    {
        OAM_LOG(OAM, OAM_ERROR, "Value of numPeereNB:%d out"
                " of range (0-%d)", temp_num_peer_enb, MAX_PEER_ENB);
        oam_handle_invalid_startup_config();
    }


    oam_xml_find_ptr(xml_struct_g.X2AP, &temp1,"peer_eNB_CommInfo");
    if(flag_found){
        /* SPR Fix 18741 Start */
        for(count= OAM_ZERO; count < (SInt32)temp_num_peer_enb && OAM_NULL != (xmlNode *)temp1; count++)
        {
        /* SPR Fix 18741 End */
            xml_struct_g.peer_eNB_CommInfo[count] = (xmlNode*)temp1;
            temp1 = (SInt32)(((xmlNode *)temp1)->next);
            if(xml_struct_g.peer_eNB_CommInfo[count]->type == XML_COMMENT_NODE)
            {
                count--;
                continue;
            }
        }
    }
    /* SPR Fix 18741 Start */
    if (count < (SInt32)temp_num_peer_enb)
    /* SPR Fix 18741 End */
    {
        OAM_LOG(OAM, OAM_ERROR, "Value of numPeereNB:%d is greater than"
                "number of peer_eNB_CommInfo nodes:%d in file",
                temp_num_peer_enb, count);
        oam_handle_invalid_startup_config();
    }
#endif

    #ifdef ENDC_ENABLED
    /* MENB CHANGES - START */
    flag_found = OAM_ZERO;
    count = OAM_ZERO;
    UInt32 temp_num_peer_gnb = OAM_ZERO;
    temp_num_peer_gnb = oam_atoi((const char *)oam_xmlNodeGetContent(
            xml_struct_g.X2AP,"numPeergNB"));
    if ( MAX_PEER_GNB < temp_num_peer_gnb )
    {
        OAM_LOG(OAM, OAM_ERROR, "Value of numPeergNB:%d out"
                " of range (0-%d)", temp_num_peer_gnb, MAX_PEER_GNB);
        oam_handle_invalid_startup_config();
    }
    oam_xml_find_ptr(xml_struct_g.X2AP, &temp1,"peer_gNB_CommInfo");
    if ( flag_found )
    {
        for ( count = OAM_ZERO; count < (SInt32)temp_num_peer_gnb &&
            OAM_NULL != (xmlNode *)temp1; count++ )
        {
            xml_struct_g.peer_gNB_CommInfo[count] = (xmlNode*)temp1;
            temp1 = (((xmlNode *)temp1)->next);
            if ( XML_COMMENT_NODE == xml_struct_g.peer_gNB_CommInfo[count]->type )
            {
                count--;
                continue;
            }
        }
    }
    
    if (count < (SInt32)temp_num_peer_gnb)
    {
        OAM_LOG(OAM, OAM_ERROR, "Value of numPeergNB:%d is greater than"
                "number of peer_gNB_CommInfo nodes:%d in file",
                temp_num_peer_gnb, count);
        oam_handle_invalid_startup_config();
    }
    /* MENB CHANGES - END */
    #endif

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(xml_struct_g.X2AP,&temp1,"guard_timers");
    if(flag_found){
    xml_struct_g.X2AP_guard_timers = (xmlNode*)temp1;
    }

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(xml_struct_g.X2AP,&temp1,"procedural_timers");
    if(flag_found){
    xml_struct_g.X2AP_procedural_timers = (xmlNode*)temp1;
    }
/* LWA changes start*/
/* LWA changes end*/
/* SPR21708 Changes start*/
    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"GTPUParams");
    if(flag_found){
    xml_struct_g.GTPUParams = (xmlNode*)temp1;
    }
/* SPR21708 Changes End*/

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"RRMparams");
    if(flag_found){
    xml_struct_g.RRMparams = (xmlNode*)temp1;
    }

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"shm_logger_cfg");
    if(flag_found){
    xml_struct_g.shm_logger_cfg = (xmlNode*)temp1;
    }

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"operator_info");
    if(flag_found){
    xml_struct_g.operator_info = (xmlNode*)temp1;
    }

#ifdef OAM_SON_ENABLE
    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"SONParams");
    if(flag_found){
    xml_struct_g.SONParams = (xmlNode*)temp1;
    }

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"son_es_params");
    if(flag_found){
    xml_struct_g.son_es_params = (xmlNode*)temp1;
    }

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"son_oam_nr_enb_add_req");
    if(flag_found){
    xml_struct_g.son_oam_nr_enb_add_req = (xmlNode*)temp1;
    }

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(xml_struct_g.son_oam_nr_enb_add_req,&temp1,"nbr_enb_list");
    if(flag_found){
    xml_struct_g.nbr_enb_list = (xmlNode*)temp1;
    }

    /*eICIC_PHASE_1_2_CHANGES_START*/
    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"son_oam_anr_neighbor_default_config_req");
    if(flag_found){
    xml_struct_g.son_oam_anr_neighbor_default_config_req = (xmlNode*)temp1;
    }

    /*eICIC_PHASE_1_2_CHANGES_END*/
    SInt32 nbr_enb_list_size = oam_atoi((const char *)oam_xmlNodeGetContent(
                xml_struct_g.nbr_enb_list,"nbr_enb_list_size"));

    if(nbr_enb_list_size > SON_MAX_NO_NEIGHBOUR_ENBS)
    {
        OAM_LOG(OAM, OAM_ERROR,"SON Number of ENB Neighbors[%u] "
        "exceed the max limit", nbr_enb_list_size);
        oam_handle_invalid_startup_config();
    }

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(xml_struct_g.nbr_enb_list,&temp1,"nbr_enb_info");
    if(flag_found){
        for(count= OAM_ZERO; count < nbr_enb_list_size; count++)
        {
            xml_struct_g.nbr_enb_info[count] = (xmlNode*)temp1;
    /* SPR 20430 Changes Start*/
            temp1 = (((xmlNode *)temp1)->next);
    /* SPR 20430 Changes End*/
        }
    }
    /*SPR_16744_END*/
#endif

    OAM_FUN_ENTRY_TRACE();
    return OAM_SUCCESS;
}

/*******************************************************************************
* Function Name  : oam_xml_struct_tr196_populate()
* Description    : This function is used to populate the global structure of 
                 : xml_struct_tr196.
* Inputs         : p_node :xmlNode type  pointer of the parent node
* Outputs        : none
* Returns        : none
*******************************************************************************/
/* As a part of SPR 21013, parameter population of FAPService has been modified
 * to add support for storing multiple FAPService instances. CellConfig strcuture
 * of n cells is changed to single CellConfig element and will now be accessed 
 * through FAPService[index]. */
oam_return_et
oam_xml_struct_tr196_populate(xmlNode *p_node)
{
    OAM_FUN_ENTRY_TRACE();
    static int first_time = OAM_FALSE;
    /* SPR 20430 Changes Start*/
    xmlNode *temp1 = OAM_NULL;    
    SInt32 count = OAM_ZERO;
    /* SPR 20430 Changes End*/
    /* spr_21013_fix_start/end Code removed */
    SInt32 max_temp = OAM_ZERO;
    /* RT SPR 9019 FIX START */
    SInt32 num_entries = 0;
    /* spr_21013_fix_start/end Code removed */
    Char8* tag_value = OAM_NULL;
    SInt32 max_entries = 0;
    /* RT SPR 9019 FIX END */
    UInt32 region=OAM_ZERO;
    xmlNode *p_cell_config_tag = OAM_NULL;
    /* spr_21013_fix_start */
    UInt8 fap_index = OAM_ZERO;
    UInt16 num_of_fap_service = OAM_ZERO;
    xmlNode *p_fap_service_tag = OAM_NULL;
    XML_struct_fap_service *p_fapService_xml_st = OAM_NULL;
    /* spr_21013_fix_end */
    UInt8 index = 0;
    oam_rrm_cell_config_req_t *p_rrm_cell_config = OAM_NULL;
    oam_return_et retVal = OAM_SUCCESS;
    /* EMBMS Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
	Char8 buf[OAM_TAG_VALUE_STR_MAX_LEN] = {OAM_ZERO};
#endif
    xmlNode *p_result_node = OAM_NULL;
    /* EMBMS Changes End */
    /* SPR 14651 START */
    UInt8 meas_count = OAM_ZERO;
    UInt8 eutran_meas_enable = OAM_ZERO;
    UInt8 utran_meas_enable = OAM_ZERO;
    UInt8 geran_meas_enable = OAM_ZERO;
    /* SPR 14651 END */

    /*SPR 23209 FIXED START */
    UInt8 operator_list_size = OAM_ZERO;
    /*SPR 23209 FIXED END */

#ifdef OAM_SON_ENABLE
    oam_son_inter_freq_carrier_t *p_oam_son_inter_freq_carrier = OAM_NULL;
#endif

    if(first_time == OAM_TRUE)
    {
       oam_memset(&xml_struct_tr196_g,OAM_ZERO,sizeof(xml_struct_tr196_g));
       first_time = OAM_FALSE;
    }
   
    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"InternetGatewayDevice");
    /*finds InternetGatewayDevice in the tree using xmlNode pointer p_node*/
    xml_struct_tr196_g.InternetGatewayDevice = (xmlNode*)temp1;
    /*populates the xmlNode Pointer in structure*/

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"X_VENDOR_NMS_FTPServerConfig");
    xml_struct_tr196_g.nms_ftp_server_params = (xmlNode*)temp1;
   
    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"Time");
    xml_struct_tr196_g.Time = (xmlNode*)temp1;
   
    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"ManagementServer");
    xml_struct_tr196_g.ManagementServer = (xmlNode*)temp1;

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"DeviceInfo");
    xml_struct_tr196_g.DeviceInfo = (xmlNode*)temp1;

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"LANDevice");
    xml_struct_tr196_g.LANDevice = (xmlNode*)temp1;

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"LAN_HostConfigManagement");
    xml_struct_tr196_g.LAN_HostConfigManagement = (xmlNode*)temp1;

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"IPInterface");

    IPInterfaceNumberOfEntries = oam_atoi((const char *)oam_xmlNodeGetContent(xml_struct_tr196_g.LAN_HostConfigManagement,"IPInterfaceNumberOfEntries"));

    for( count = OAM_ZERO; ((count < (SInt64)IPInterfaceNumberOfEntries) && (count < OAM_MAX_NUM_IP_INTRFACES)&& (OAM_NULL != (xmlNode *)temp1)) ; )
    {
        if(((xmlNode *)temp1)->type != XML_COMMENT_NODE)
        {
            xml_struct_tr196_g.IPInterface[count]= (xmlNode*)temp1;
            count++;
        }
    /* SPR 20430 Changes Start*/
        temp1 = (((xmlNode *)temp1)->next);
    /* SPR 20430 Changes End*/
    }

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"FAP");
    xml_struct_tr196_g.FAP = (xmlNode*)temp1;

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"PerfMgmt");
    xml_struct_tr196_g.PerfMgmt = (xmlNode*)temp1;

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"Config");
    xml_struct_tr196_g.Config = (xmlNode*)temp1;


    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"Tunnel");
    xml_struct_tr196_g.Tunnel = (xmlNode*)temp1;

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"CryptoProfile");
    xml_struct_tr196_g.CryptoProfile = (xmlNode*)temp1;

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"IKESA");
    xml_struct_tr196_g.IKESA = (xmlNode*)temp1;

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"VirtualInterface");
    xml_struct_tr196_g.VirtualInterface = (xmlNode*)temp1;

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"Services");
    xml_struct_tr196_g.Services = (xmlNode*)temp1;
 
    /*sets the flag to 0 each time when a new element is to be found*/
    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"FAPService");
    /* spr_21013_fix_start */
    tag_value = (Char8*)oam_xmlNodeGetContent(
                                        xml_struct_tr196_g.Services,
                                        "FAPServiceNumberOfEntries");
    num_of_fap_service = oam_atoi(tag_value);

    OAM_LOG(OAM , OAM_DETAILED ,"num_of_fap_servies = %d",num_of_fap_service);
    p_fap_service_tag = (xmlNode*)temp1;
    while((fap_index < num_of_fap_service) &&
            flag_found && p_fap_service_tag) {

        p_fapService_xml_st = &xml_struct_tr196_g.FAPService[fap_index];

        if(!fap_index)
        {
            p_fapService_xml_st->FapService = (xmlNode*)temp1;
            flag_found = OAM_ZERO;	
            oam_xml_find_ptr(p_fap_service_tag,&temp1,"Capabilities"); 
            p_fapService_xml_st->Capabilities = (xmlNode*)temp1;	

            flag_found = OAM_ZERO;		
            oam_xml_find_ptr(p_fapService_xml_st->Capabilities,&temp1,"LTE");	
            /* finds LTE in Capabilities as occurence of element 
               LTE more than one are possible*/
            p_fapService_xml_st->LTE_1 = ((xmlNode*)temp1);
            /*populates LTE in LTE_1*/

            flag_found = OAM_ZERO;	        
            oam_xml_find_ptr(p_fap_service_tag,&temp1,"FAPControl");
            p_fapService_xml_st->FAPControl = (xmlNode*)temp1;	

            flag_found = OAM_ZERO;	        
            oam_xml_find_ptr(p_fapService_xml_st->FAPControl,&temp1,"LTE");
            p_fapService_xml_st->LTE_2 = ((xmlNode*)temp1);	

            flag_found = OAM_ZERO;	        
            oam_xml_find_ptr(p_fapService_xml_st->LTE_2,&temp1,"Gateway");
            p_fapService_xml_st->Gateway = (xmlNode*)temp1;	

        }

        flag_found = OAM_ZERO;	      
        oam_xml_find_ptr(p_fap_service_tag,&temp1,"AccessMgmt");
        p_fapService_xml_st->AccessMgmt = (xmlNode*)temp1;	

        flag_found = OAM_ZERO;	        
        oam_xml_find_ptr(p_fapService_xml_st->AccessMgmt,&temp1,"LTE");
        /* SPR 20430 Changes Start*/
        if(p_node!=temp1)	
            /* SPR 20430 Changes End*/
            p_fapService_xml_st->LTE_3 = ((xmlNode*)temp1);	

        /* spr_21013_fix_start/end code removed for X_VENDOR_CELL_CONFIG_NUM_ENTRIES */

        /* SPR 14016 START */
        tag_value = (Char8*)oam_xmlNodeGetContent(p_fapService_xml_st->LTE_3,
                "X_VENDOR_CELL_TYPE");
        oam_atoi(tag_value);
        if(OAM_MACRO_ENB_TYPE == oam_atoi(tag_value)) {
            RRM_PROV_REQ.oam_rrm_enb_config_req.enb_id.
                enb_type = RRM_ENB_TYPE_MACRO;
        }
        else {
            RRM_PROV_REQ.oam_rrm_enb_config_req.enb_id.
                enb_type = RRM_ENB_TYPE_HOME;
        }
        /* SPR 14016 END */


        flag_found = OAM_ZERO;	      
        oam_xml_find_ptr(p_fap_service_tag,&temp1,"CellConfig");

        p_cell_config_tag = (xmlNode*)temp1;

        if(p_cell_config_tag->type != XML_COMMENT_NODE) {
            p_rrm_cell_config = &oam_prov_req.oam_rrm_req.
                oam_rrm_cell_config_req[index++];

#ifdef OAM_SON_ENABLE
            p_oam_son_inter_freq_carrier =
                &SON_INTER_FREQ_CARR_PARAMS(fap_index);
#endif

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_cell_config_tag, &temp1, "LTE");
            p_fapService_xml_st->CellConfig.LTE_4 = ((xmlNode*)temp1);

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.LTE_4, &temp1, "Tunnel");
            p_fapService_xml_st->CellConfig.Tunnel_2 = ((xmlNode*)temp1);

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.LTE_4, &temp1, "EPC");    
            p_fapService_xml_st->CellConfig.EPC = (xmlNode*)temp1;

            tag_value = (Char8*)oam_xmlNodeGetContent(
                    p_fapService_xml_st->CellConfig.EPC,
                    "MaxPLMNListEntries");
            max_entries = oam_atoi(tag_value);

            if(max_entries > RRM_OAM_MAX_NUM_PLMNS) {
                OAM_LOG(OAM, OAM_ERROR, "Set %u. Maximum allowed value of the"
                  " tag MaxPLMNListEntries is %d.", max_entries, RRM_OAM_MAX_NUM_PLMNS);
                OAM_FUN_EXIT_TRACE();
                return OAM_FAILURE;
            }

            tag_value = (Char8*)oam_xmlNodeGetContent(
                    p_fapService_xml_st->CellConfig.EPC,
                    "PLMNListNumberOfEntries");

            num_entries = oam_atoi(tag_value);

            if(num_entries > max_entries) {
                OAM_LOG(OAM, OAM_ERROR, "Set %u. Maximum allowed value of the"
                          " tag PLMNListNumberOfEntries is %d", num_entries, max_entries);
                OAM_FUN_EXIT_TRACE();
                return OAM_FAILURE;
            }

            flag_found = 0;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.EPC, &temp1, "PLMNList");
            count = 0;
            while((count < num_entries) && temp1) {
                if((((xmlNode*)temp1)->type != XML_COMMENT_NODE)) {
                    p_fapService_xml_st->CellConfig.PLMNList[count++] = (xmlNode*)temp1;
                }

                /* SPR 20430 Changes Start*/
                temp1 = (((xmlNode*)temp1)->next);
                /* SPR 20430 Changes End*/
            }

            /* SPR 15527 START */

            UInt8 num_qos_entries = oam_atoi((const char *)oam_xmlNodeGetContent(
                        p_fapService_xml_st->CellConfig.EPC,"QoSNumberOfEntries"));
            /* SPR 15527 END */                                                  

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.EPC, &temp1, "QoS");

            count = MIN_RRM_CELL_QCI;
            /* SPR 15527 START */
            while(( count <= num_qos_entries) && (count <= MAX_RRM_CELL_QCI) && temp1) {
                /* SPR 15527 END */
                if(((xmlNode *)temp1)->type != XML_COMMENT_NODE) {
                    p_fapService_xml_st->CellConfig.QoS[count - 1] = (xmlNode*)temp1;
                    ++count;
                }

                /* SPR 20430 Changes Start*/
                temp1 = (((xmlNode *)temp1)->next);
                /* SPR 20430 Changes End*/
            }


            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.LTE_4, &temp1, "RAN");
            p_fapService_xml_st->CellConfig.RAN = (xmlNode*)temp1;	

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.RAN, &temp1, "Common");
            p_fapService_xml_st->CellConfig.Common_1 = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.RAN,
                    &temp1, "X_VENDOR_ADDITIONAL_OPERATOR_INFO");
            p_fapService_xml_st->CellConfig.X_VENDOR_ADDITIONAL_OPERATOR_INFO =
                (xmlNode*)temp1;

            /* Coverity CID: 56081; Proj: 18_NOV_OAM_2.4.1 START */
            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.RAN,
                    &temp1, "SIB_SCHEDULING_INFO");

            num_entries = 0;
            if(flag_found) {
                while(temp1 && (num_entries < RRM_OAM_MAX_NUM_SI)) {
                    if(((xmlNode*)temp1)->type != XML_COMMENT_NODE) {
                        p_fapService_xml_st->CellConfig.SCHEDULING_INFO[num_entries++] =
                            (xmlNode*)temp1;
                    }

                    /* SPR 20430 Changes Start*/
                    temp1 = (((xmlNode*)temp1)->next);
                    /* SPR 20430 Changes End*/
                }
            }
            /* Coverity CID: 56081; Proj: 18_NOV_OAM_2.4.1 END */
            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.RAN,
                    &temp1, "X_VENDOR_EMERGENCY_SERVICE");
            p_fapService_xml_st->CellConfig.X_VENDOR_EMERGENCY_SERVICE =
                (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.RAN, &temp1, "S1AP");
            p_fapService_xml_st->CellConfig.S1AP = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.RAN, &temp1, "S1U");
            p_fapService_xml_st->CellConfig.S1U = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.S1U,
                    &temp1, "GTPUEchoInterval");
            p_fapService_xml_st->CellConfig.GTPUEchoInterval = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.RAN, &temp1, "RF");
            p_fapService_xml_st->CellConfig.RF_1 = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.RAN, &temp1, "PHY");
            p_fapService_xml_st->CellConfig.PHY = (xmlNode*)temp1;	

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.PHY,
                    &temp1, "AntennaInfo");
            p_fapService_xml_st->CellConfig.AntennaInfo = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.PHY, &temp1, "PDSCH");
            p_fapService_xml_st->CellConfig.PDSCH = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.PHY, &temp1, "SRS");
            p_fapService_xml_st->CellConfig.SRS = (xmlNode*)temp1;	

            flag_found = OAM_ZERO;	      
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.PHY, &temp1, "PRACH");
            p_fapService_xml_st->CellConfig.PRACH = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.PHY, &temp1, "PUCCH");
            p_fapService_xml_st->CellConfig.PUCCH = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.PHY, &temp1, "PUSCH");
            p_fapService_xml_st->CellConfig.PUSCH = (xmlNode*)temp1;	

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.PHY, &temp1, "ULRS");
            p_fapService_xml_st->CellConfig.ULRS = (xmlNode*)temp1;	

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.PHY,
                    &temp1, "ULPowerControl");
            p_fapService_xml_st->CellConfig.ULPowerControl = (xmlNode*)temp1;

            /* EMBMS Changes Start */
            p_result_node = oam_get_xml_nonleaf_child_node(
/* SPR 22248 MBMS Changes Start */
                                     p_fapService_xml_st->CellConfig.PHY, "MBSFN");
            if (p_result_node)
            {
/* SPR 22248 MBMS Changes Stop */
                p_fapService_xml_st->CellConfig.MBSFN = p_result_node;
            }
#ifdef LTE_EMBMS_SUPPORTED
/* SPR 22248 MBMS Changes Start */
            if (p_fapService_xml_st->CellConfig.MBSFN)
            {
                p_result_node = oam_get_xml_nonleaf_child_node(
                                            p_fapService_xml_st->CellConfig.MBSFN,
                                            "MBSFN_SERVICE_AREA_CONFIG");

                p_fapService_xml_st->CellConfig.MBSFN_SERVICE_AREA_CONFIG = p_result_node;

                oam_get_xml_text_node_content(p_fapService_xml_st->CellConfig.MBSFN,
                        "X_VENDOR_IS_SIB2_EMBMS_ENABLE",
                        buf,
                        OAM_TAG_VALUE_STR_MAX_LEN);

                if (!oam_strcmp(buf, "1"))
                {
                    oam_get_xml_text_node_content(p_fapService_xml_st->CellConfig.MBSFN,
                            /* SPR 22248 MBMS Changes Stop */
                            "MaxSFConfigListEntries",
                            buf,
                            OAM_TAG_VALUE_STR_MAX_LEN);
                    max_entries = oam_atoi(buf);

                    if (max_entries > MAX_SFCONFIG_NUM_ENTRIES)
                    {
                        OAM_LOG(OAM, OAM_CRITICAL,
                                "Maximum allowed value of the"
                                " tag MaxSFConfigListEntries is %d",
                                MAX_SFCONFIG_NUM_ENTRIES);
                        retVal = OAM_FAILURE;

                        OAM_FUN_EXIT_TRACE();
                        return retVal;
                    }
                }

            }

/* SPR 22248 MBMS Changes Start */
            oam_get_xml_text_node_content(p_fapService_xml_st->CellConfig.MBSFN,
                    /* SPR 22248 MBMS Changes Stop */
                    "SFConfigListNumberOfEntries", buf,
                    OAM_TAG_VALUE_STR_MAX_LEN);
            num_entries = oam_atoi(buf);

            if (num_entries > max_entries)
            {
                OAM_LOG(OAM, OAM_CRITICAL,
                        "Value of SFConfigListNumberOfEntries[%d] is "
                        "greater than value of MaxSFConfigListEntries[%d]",
                        num_entries, max_entries);
                retVal = OAM_FAILURE;

                OAM_FUN_EXIT_TRACE();
                return retVal;
            }
/* SPR 22248 MBMS Changes Start */
            p_result_node = oam_get_xml_nonleaf_child_node(
                                   p_fapService_xml_st->CellConfig.MBSFN,
                                   /* SPR 22248 MBMS Changes Stop */
                                   "SFConfigList");

            count = OAM_ZERO;

            if (p_result_node)
            {
                while (p_result_node && (count < num_entries) &&
                        (count < MAX_SFCONFIG_NUM_ENTRIES))
                {
                    if((p_result_node)->type != XML_COMMENT_NODE) {
                        /* SPR 22248 MBMS Changes Start */
                        p_fapService_xml_st->CellConfig.SFConfigList[count++] =
                        /* SPR 22248 MBMS Changes Stop */    
                            p_result_node;
                    }

                    p_result_node = p_result_node->next;
                }

                //oam_prov_req.oam_mbms_data.oam_m2ap_prov_req.
                //  num_mbms_sf_info = count;
            }
#endif
            /* EMBMS Changes End */



            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.PHY, &temp1, "PRS");
            p_fapService_xml_st->CellConfig.PRS = (xmlNode*)temp1;	

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.PHY, &temp1, "TDDFrame");
            p_fapService_xml_st->CellConfig.TDDFrame = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.RAN, &temp1, "MAC");
            p_fapService_xml_st->CellConfig.MAC = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.MAC, &temp1, "RACH");
            p_fapService_xml_st->CellConfig.RACH = (xmlNode*)temp1;	
            flag_found = OAM_ZERO;	        
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.MAC, &temp1, "DRX");
            p_fapService_xml_st->CellConfig.DRX = (xmlNode*)temp1;	


            flag_found = OAM_ZERO;	       
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.MAC, &temp1, "ULSCH");
            if(flag_found) {
                p_fapService_xml_st->CellConfig.ULSCH = (xmlNode*)temp1;
            }

            /* CSR 59124 Change Start*/ 
            flag_found = OAM_ZERO;	        
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.MAC, &temp1, "CDRX");
            if(flag_found) {
                p_fapService_xml_st->CellConfig.CDRX = (xmlNode*)temp1;
                p_rrm_cell_config->ran_info.mac_layer_params.
                    bitmask |= RRM_OAM_MAC_LAYER_PARAM_CDRX_PRESENT;
            }
            /* CSR 59124 Change End*/

            flag_found = OAM_ZERO;	       
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.RAN, &temp1, "RLC");
            p_fapService_xml_st->CellConfig.RLC = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.RLC, &temp1, "SRB1");
            p_fapService_xml_st->CellConfig.SRB1 = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.RLC, &temp1, "SRB2");
            p_fapService_xml_st->CellConfig.SRB2 = (xmlNode*)temp1; 

            flag_found = OAM_ZERO;	
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.RAN, &temp1, "RRCTimers");
            p_fapService_xml_st->CellConfig.RRCTimers = (xmlNode*)temp1;

            flag_found = OAM_ZERO;

            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.RAN,
                    &temp1, "CellRestriction");
            p_fapService_xml_st->CellConfig.CellRestriction = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            /* SPR 16419 Fix Start */
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.CellRestriction,
                    &temp1, "X_VENDOR_ACCESS_BARRING_INFO");
            if (flag_found) { 
                p_fapService_xml_st->CellConfig.X_VENDOR_ACCESS_BARRING_INFO =
                    (xmlNode*)temp1;
            }
            if (p_fapService_xml_st->CellConfig.X_VENDOR_ACCESS_BARRING_INFO) {

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(
                        p_fapService_xml_st->CellConfig.X_VENDOR_ACCESS_BARRING_INFO,
                        &temp1, "AC_BARRING_FOR_MO_SIGNALLING");
                if(flag_found){
                    p_fapService_xml_st->CellConfig.AC_BARRING_FOR_MO_SIGNALLING =
                        (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(
                        p_fapService_xml_st->CellConfig.X_VENDOR_ACCESS_BARRING_INFO,
                        &temp1, "AC_BARRING_FOR_MO_DATA");
                if(flag_found){
                    p_fapService_xml_st->CellConfig.AC_BARRING_FOR_MO_DATA = (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(
                        p_fapService_xml_st->CellConfig.X_VENDOR_ACCESS_BARRING_INFO,
                        &temp1, "SSAC_BARRING_FOR_MMTEL_VOICE_R9");
                if(flag_found){
                    p_fapService_xml_st->CellConfig.SSAC_BARRING_FOR_MMTEL_VOICE_R9 =
                        (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(
                        p_fapService_xml_st->CellConfig.X_VENDOR_ACCESS_BARRING_INFO,
                        &temp1, "SSAC_BARRING_FOR_MMTEL_VIDEO_R9");
                if(flag_found){
                    p_fapService_xml_st->CellConfig.SSAC_BARRING_FOR_MMTEL_VIDEO_R9 =
                        (xmlNode*)temp1;
                }
                flag_found = OAM_ZERO;
                oam_xml_find_ptr(
                        p_fapService_xml_st->CellConfig.X_VENDOR_ACCESS_BARRING_INFO,
                        &temp1, "AC_BARRING_FOR_CSFB_R10");
                if(flag_found){ 
                    p_fapService_xml_st->CellConfig.AC_BARRING_FOR_CSFB_R10 = (xmlNode*)temp1;

                }
            }
            /* SPR 16419 Fix End */
            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.RAN, &temp1, "Mobility");
            p_fapService_xml_st->CellConfig.Mobility = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.Mobility,
                    &temp1, "IdleMode");
            p_fapService_xml_st->CellConfig.IdleMode = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.IdleMode,
                    &temp1, "Common");
            p_fapService_xml_st->CellConfig.Common_2 = ((xmlNode*)temp1);

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.IdleMode,
                    &temp1, "IntraFreq");
            p_fapService_xml_st->CellConfig.IntraFreq = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.IdleMode,
                    &temp1, "InterFreq");
            p_fapService_xml_st->CellConfig.InterFreq = (xmlNode*)temp1;	

            tag_value = (Char8*)oam_xmlNodeGetContent(
                    p_fapService_xml_st->CellConfig.InterFreq,
                    "CarrierNumberOfEntries");
            num_entries = oam_atoi(tag_value);


            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.InterFreq,
                    &temp1, "Carrier");
            count = 0;
            while((count < num_entries) && temp1) {
                if(((xmlNode*)temp1)->type != XML_COMMENT_NODE) {
                    /* SPR 14651 START */
                    p_fapService_xml_st->CellConfig.Carrier[count] = (xmlNode*)temp1;
                    /* SPR 14651 END */
                }

                /* SPR 14651 START */
                eutran_meas_enable = oam_atoi((Char8*)oam_xmlNodeGetContent(p_fapService_xml_st->CellConfig.Carrier[count],
                            "Enable"));
                if(eutran_meas_enable)
                {
                    meas_count++;
                }    
                count++;
                /* SPR 14651 END */
                /* SPR 20430 Changes Start*/
                temp1 = (((xmlNode*)temp1)->next);
                /* SPR 20430 Changes End*/
            }
            /* SPR 14651 START */
            if(meas_count)
            {
                p_rrm_cell_config->ran_info.
                    bitmask |= RRM_OAM_MOBILITY_LAYER_PARAMS_PRESENT;       

                p_rrm_cell_config->ran_info. mobility_params.
                    bitmask |= RRM_OAM_IDLE_MODE_MOBILITY_PARAMS_PRESENT; 
                /* SPR 14651 END */

                p_rrm_cell_config->ran_info.mobility_params.
                    idle_mode_mobility_params.
                    bitmask |= RRM_OAM_IDLE_MODE_MOBILITY_INTER_FREQ_PRESENT;
            }

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.IdleMode, &temp1, "IRAT");
            p_fapService_xml_st->CellConfig.IRAT_1 = (xmlNode*)temp1;

            /* RT SPR 9019 FIX START */
            flag_found = 0;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.IRAT_1, &temp1, "UTRA");
            p_fapService_xml_st->CellConfig.UTRA = (xmlNode*)temp1;

            tag_value = (Char8*)oam_xmlNodeGetContent(
                    p_fapService_xml_st->CellConfig.UTRA,
                    "UTRANFDDFreqNumberOfEntries");

            num_entries = oam_atoi(tag_value);

            tag_value = (Char8*)oam_xmlNodeGetContent(
                    p_fapService_xml_st->CellConfig.UTRA,
                    "MaxUTRANFDDFreqEntries");

            max_entries = oam_atoi(tag_value);

            if(max_entries > OAM_MAX_NUM_UTRA_FDD_CARRIERS) {
                OAM_LOG(OAM, OAM_ERROR, "Set %u. Maximum allowed value of the"
                        " tag MaxUTRANFDDFreqEntries is %d",
                                        max_entries, OAM_MAX_NUM_UTRA_FDD_CARRIERS);
                OAM_FUN_EXIT_TRACE();
                return OAM_FAILURE;
            }

            if(num_entries > max_entries) {
                OAM_LOG(OAM, OAM_ERROR,
                     "Value of the tag UTRANFDDFreqNumberOfEntries[%u] "
                     "can't be greater than that of MaxUTRANFDDFreqEntries[%u]",
                     num_entries, max_entries);
                OAM_FUN_EXIT_TRACE();
                return OAM_FAILURE;
            }


        #ifdef OAM_SON_ENABLE
            p_oam_son_inter_freq_carrier->max_utran_fdd_freq = max_entries;
        #endif

            flag_found = 0;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.UTRA,
                    &temp1, "UTRANFDDFreq");

            count = 0;
            /* SPR 14651 START */
            meas_count = 0;

            /* Code Removed */
            /* SPR 14651 END */

            while((count < num_entries) && temp1) {
                if(((xmlNode*)temp1)->type != XML_COMMENT_NODE) {
                    /* SPR 14651 START */
                    p_fapService_xml_st->CellConfig.UTRANFDDFreq[count] =
                        /* SPR 14651 END */
                        (xmlNode*)temp1;

                    /* SPR 14651 START */
                    /* Code Removed */
                    /* SPR 14651 END */
                }

                /* SPR 14651 START */
                utran_meas_enable = oam_atoi((Char8*)oam_xmlNodeGetContent(
                            p_fapService_xml_st->CellConfig.UTRANFDDFreq[count],"Enable"));
                if(utran_meas_enable) 
                {
                    meas_count++;
                }    
                count++;
                /* SPR 14651 END */
                /* SPR 20430 Changes Start*/
                temp1 = (((xmlNode*)temp1)->next);
                /* SPR 20430 Changes End*/
            }

            /* SPR 14651 START */
            /* Code Removed */

            if(meas_count)
            {   
                p_rrm_cell_config->ran_info.
                    bitmask |= RRM_OAM_MOBILITY_LAYER_PARAMS_PRESENT;       

                p_rrm_cell_config->ran_info. mobility_params.
                    bitmask |= RRM_OAM_IDLE_MODE_MOBILITY_PARAMS_PRESENT;

                p_rrm_cell_config->ran_info.mobility_params.
                    idle_mode_mobility_params.
                    bitmask |= RRM_OAM_IDLE_MODE_MOBILITY_IRAT_UTRA_PRESENT;
                /* SPR 14651 END */
                p_rrm_cell_config->ran_info.mobility_params.
                    idle_mode_mobility_params.idle_mode_mobility_inter_rat_utra_params.
                    bitmask |= RRM_OAM_IRAT_EUTRAN_UTRAN_FDD_LIST_PRESENT;
            }        

            tag_value = (Char8*)oam_xmlNodeGetContent(
                    p_fapService_xml_st->CellConfig.UTRA,
                    "NUM_IRAT_EUTRAN_TO_UTRAN_TDD_CARRIERS");

            num_entries = oam_atoi(tag_value);

            if(num_entries > OAM_MAX_NUM_UTRA_TDD_CARRIERS) {
                OAM_LOG(OAM, OAM_ERROR, "Set %u. Maximum allowed value of the"
                        " tag NUM_IRAT_EUTRAN_TO_UTRAN_TDD_CARRIERS is %d",
                          num_entries, OAM_MAX_NUM_UTRA_TDD_CARRIERS);
                OAM_FUN_EXIT_TRACE();
                return OAM_FAILURE;
            }

            flag_found = 0;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.UTRA,
                    &temp1, "IRAT_EUTRAN_TO_UTRAN_TDD_CARRIERS");

            if(!flag_found && num_entries) {
                OAM_LOG(OAM, OAM_ERROR, "Mandatory tag "
                        "\"IRAT_EUTRAN_TO_UTRAN_TDD_CARRIERS\" missing");
                OAM_FUN_EXIT_TRACE();
                return OAM_FAILURE;
            }

            count = 0;


            p_rrm_cell_config->ran_info.mobility_params.
                idle_mode_mobility_params.idle_mode_mobility_inter_rat_utra_params.
                irat_eutran_to_utran_tdd_list.num_irat_eutran_to_utran_tdd_carriers = 0;
            while((count < num_entries) && temp1) {
                if((((xmlNode*)temp1)->type != XML_COMMENT_NODE)) {
                    p_fapService_xml_st->CellConfig.UTRANTDDFreq[count++] =
                        (xmlNode*)temp1;

                    ++p_rrm_cell_config->ran_info.mobility_params.
                        idle_mode_mobility_params.idle_mode_mobility_inter_rat_utra_params.
                        irat_eutran_to_utran_tdd_list.num_irat_eutran_to_utran_tdd_carriers;
                }

                /* SPR 20430 Changes Start*/
                temp1 = (((xmlNode*)temp1)->next);
                /* SPR 20430 Changes End*/
            }

            if(count) {
                p_rrm_cell_config->ran_info.mobility_params.
                    idle_mode_mobility_params.idle_mode_mobility_inter_rat_utra_params.
                    bitmask |= RRMCM_RMIF_IRAT_EUTRAN_UTRAN_TDD_LIST_PRESENT;
            }

            flag_found = 0;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.IRAT_1, &temp1, "GERAN");
            p_fapService_xml_st->CellConfig.GERAN = (xmlNode*)temp1;


            tag_value = (Char8*)oam_xmlNodeGetContent(
                    p_fapService_xml_st->CellConfig.GERAN,
                    "MaxGERANFreqGroupEntries");
            max_entries = oam_atoi(tag_value);

            if(max_entries > OAM_MAX_NUM_GERAN_CARRIERS) {
                OAM_LOG(OAM, OAM_ERROR, "Maximum allowed value of the"
                        " tag MaxGERANFreqGroupEntries is %d",
                                       max_entries, OAM_MAX_NUM_GERAN_CARRIERS);
                OAM_FUN_EXIT_TRACE();
                return OAM_FAILURE;
            }

#ifdef OAM_SON_ENABLE
            p_oam_son_inter_freq_carrier->max_geran_freq_entries = max_entries;
#endif


            tag_value = (Char8*)oam_xmlNodeGetContent(
                    p_fapService_xml_st->CellConfig.GERAN,
                    "GERANFreqGroupNumberOfEntries");

            num_entries = oam_atoi(tag_value);

            if(num_entries > max_entries) {
                OAM_LOG(OAM, OAM_ERROR, "Set %u. Maximum allowed value of the"
                        " tag GERANFreqGroupNumberOfEntries is %d",
                                num_entries, max_entries);
                OAM_FUN_EXIT_TRACE();
                return OAM_FAILURE;
            }

            flag_found = 0;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.GERAN,
                    &temp1, "GERANFreqGroup");
            count = 0;
            /* SPR 14651 START */
            meas_count = 0;
            /* SPR 14651 END */
            while((count < num_entries) && temp1) {  
                if((((xmlNode*)temp1)->type != XML_COMMENT_NODE)) {
                    /* SPR 14651 START */
                    p_fapService_xml_st->CellConfig.GERANFreqGroup[count] =
                        /* SPR 14651 END */
                        (xmlNode*)temp1;

                    /* SPR 14651 START */
                    /* Code Removed */
                    /* SPR 14651 END */
                }
                /* SPR 14651 START */
                geran_meas_enable = oam_atoi((Char8*)oam_xmlNodeGetContent(p_fapService_xml_st->CellConfig.GERANFreqGroup[count],
                            "Enable"));
                if(geran_meas_enable)
                {
                    meas_count++;
                }
                count++;
                /* SPR 14651 END */

                /* SPR 20430 Changes Start*/
                temp1 = (((xmlNode*)temp1)->next);
                /* SPR 20430 Changes End*/
            }

            /* SPR 14651 START */
            if(meas_count) {
                /* SPR 14651 END */
                p_rrm_cell_config->ran_info.
                    bitmask |= RRM_OAM_MOBILITY_LAYER_PARAMS_PRESENT;

                p_rrm_cell_config->ran_info.mobility_params.
                    bitmask |= RRM_OAM_IDLE_MODE_MOBILITY_PARAMS_PRESENT;

                p_rrm_cell_config->ran_info.mobility_params.
                    idle_mode_mobility_params.
                    bitmask |= RRM_OAM_IDLE_MODE_MOBILITY_IRAT_GERAN_PRESENT;

                p_rrm_cell_config->ran_info.mobility_params.
                    idle_mode_mobility_params.idle_mode_mobility_inter_rat_geran_params.
                    bitmask |= RRM_OAM_GERAN_CARRIER_FREQ_PARAMETER;
            }    
            /* RT SPR 9019 FIX END */

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.IRAT_1,
                    &temp1, "CDMA2000");
            p_fapService_xml_st->CellConfig.CDMA2000 = (xmlNode*)temp1;

            UInt32 num_cdma2000band = OAM_ZERO;
            tag_value = (Char8*)oam_xmlNodeGetContent(
                    p_fapService_xml_st->CellConfig.CDMA2000,
                    "X_VENDOR_NUMCDMA2000BAND");
            num_cdma2000band = oam_atoi(tag_value);


            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_node,&temp1,"CDMA2000Band");
            count = OAM_ZERO;
            while((count < (SInt64)num_cdma2000band) && temp1) {
                if(((xmlNode*)temp1)->type != XML_COMMENT_NODE) {
                    p_fapService_xml_st->CellConfig.CDMA2000Band[count++] =
                        (xmlNode*)temp1;
                }
                /* SPR 20430 Changes Start*/
                temp1 = (((xmlNode *)temp1)->next);
                /* SPR 20430 Changes End*/
            }

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.CDMA2000,
                    &temp1, "X_VENDOR_PRE_REG_INFO_HRPD");
            p_fapService_xml_st->CellConfig.X_VENDOR_PRE_REG_INFO_HRPD =
                (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.CDMA2000,
                    &temp1, "X_VENDOR_SECONDARY_LIST");
            p_fapService_xml_st->CellConfig.X_VENDOR_SECONDARY_LIST = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.CDMA2000, &temp1,
                    "X_VENDOR_MOBILITY_SIB_8_PARAMS");

            p_fapService_xml_st->CellConfig.X_VENDOR_MOBILITY_SIB_8_PARAMS =
                (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.CDMA2000, &temp1,
                    "X_VENDOR_CDMA2000_CELL_PARAM");
            p_fapService_xml_st->CellConfig.X_VENDOR_CDMA2000_CELL_PARAM =
                (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(
                    p_fapService_xml_st->CellConfig.X_VENDOR_CDMA2000_CELL_PARAM,
                    &temp1, "CDMA2000_RAND");
            p_fapService_xml_st->CellConfig.CDMA2000_RAND = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(
                    p_fapService_xml_st->CellConfig.X_VENDOR_CDMA2000_CELL_PARAM,
                    &temp1, "CELL_ID_1XRTT");
            p_fapService_xml_st->CellConfig.CELL_ID_1XRTT = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(
                    p_fapService_xml_st->CellConfig.X_VENDOR_CDMA2000_CELL_PARAM,
                    &temp1, "CELL_ID_HRPD");
            p_fapService_xml_st->CellConfig.CELL_ID_HRPD = (xmlNode*)temp1;

            /* Start: SPR 10551 */
            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.X_VENDOR_CDMA2000_CELL_PARAM,
                    &temp1, "REFERENCE_CELL_ID_1XRTT");
            p_fapService_xml_st->CellConfig.REFERENCE_CELL_ID_1XRTT = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.X_VENDOR_CDMA2000_CELL_PARAM,
                    &temp1,"REFERENCE_CELL_ID_HRPD");
            p_fapService_xml_st->CellConfig.REFERENCE_CELL_ID_HRPD = (xmlNode*)temp1;
            /*End: SPR 10551 */

            flag_found = OAM_ZERO;	       
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.Mobility,
                    &temp1, "ConnMode");
            p_fapService_xml_st->CellConfig.ConnMode = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.ConnMode, &temp1, "EUTRA");
            p_fapService_xml_st->CellConfig.EUTRA = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.ConnMode, &temp1, "IRAT");
            p_fapService_xml_st->CellConfig.IRAT_2 = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.RAN,
                    &temp1, "NeighborList");
            p_fapService_xml_st->CellConfig.NeighborList = (xmlNode*)temp1;

            /* Population of pointers for NeighborListInUse */
            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.RAN,
                    &temp1, "NeighborListInUse");
            p_fapService_xml_st->CellConfig.NeighborListInUse = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.NeighborListInUse,
                    &temp1, "InterRATCell");
            p_fapService_xml_st->CellConfig.InterRATCell_2 = (xmlNode*)temp1;	

#ifdef ENDC_ENABLED
            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.RAN,&temp1,"X_VENDOR_ENDC_PARAMS");
            if(flag_found)
            {
               p_fapService_xml_st->CellConfig.endc_params = (xmlNode*)temp1;
            }
#endif

            /*SPR 23209 FIXED START */
            flag_found = OAM_ZERO;

            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.RAN,&temp1,"X_VENDOR_MOCN_PARAMS");
            if(flag_found)
            {
               p_fapService_xml_st->CellConfig.mocn_params = (xmlNode*)temp1;
            }
            if(oam_atoi((const char *)oam_xmlNodeGetContent(
                            p_fapService_xml_st->CellConfig.mocn_params,"mocn_enable")))
            {
                flag_found = OAM_ZERO;

                oam_xml_find_ptr(p_fapService_xml_st->CellConfig.mocn_params,&temp1,"OperatorList");
                if(flag_found)
                {
                    p_fapService_xml_st->CellConfig.operator_list = (xmlNode*)temp1;
                }
                operator_list_size = oam_atoi((const char *)oam_xmlNodeGetContent(
                            p_fapService_xml_st->CellConfig.operator_list,"OperatorListNumberofEntries"));

                if(operator_list_size > MAX_MOCN_OPERATOR)
                {
                    OAM_LOG(OAM, OAM_CRITICAL, "\n <OperatorListNumberofEntries> [%d]"
                            "is invalid",operator_list_size);
                    oam_abort();
                }
                flag_found = OAM_ZERO;
                oam_xml_find_ptr(p_fapService_xml_st->CellConfig.operator_list,&temp1,"OperatorInfo");
                if(flag_found)
                {
                    for(count= OAM_ZERO; ((count < operator_list_size) &&
                                ((xmlNode*)temp1 != OAM_NULL)) ; count++)
                    {
                        p_fapService_xml_st->CellConfig.operators_info[count] = (xmlNode*)temp1;
                        temp1 = (((xmlNode *)temp1)->next);
                    }

                    if(operator_list_size>count)
                    {
                        OAM_LOG(OAM, OAM_CRITICAL, "\n Number of <OperatorInfo> tags [%d] "
                                "and <OperatorListNumberofEntries> is [%d].Configuration Mismatch Found",
                                count,operator_list_size);
                        oam_abort();
                    }
                }
            }
           
            /*SPR 23209 FIXED END */


	
            /* Population of pointers for NeighborListInUse  ends */
            /*SPR 15604 START */

            flag_found = OAM_ZERO;
            UInt32 max_num_of_lte_cells_nliu = oam_atoi((const char *)oam_xmlNodeGetContent(
                        p_fapService_xml_st->CellConfig.NeighborListInUse,"MaxLTECellEntries"));
            UInt32 num_of_lte_cells_nliu = oam_atoi((const char *)oam_xmlNodeGetContent(
                        p_fapService_xml_st->CellConfig.NeighborListInUse,"LTECellNumberOfEntries"));
            if(num_of_lte_cells_nliu > max_num_of_lte_cells_nliu)
            {
                OAM_LOG(OAM,OAM_ERROR,
                        "LTECellNumberOfEntries[%u] > MaxLTECellEntries[%u] in NeighborListInUse",
                        num_of_lte_cells_nliu, max_num_of_lte_cells_nliu);
            }
            else
            {
                oam_xml_find_ptr(p_fapService_xml_st->CellConfig.NeighborListInUse,&temp1,"LTECell");
                for(count= OAM_ZERO; ((count < (SInt64)num_of_lte_cells_nliu) &&
                            (count < OAM_MAX_LTE_NBR_CELL_NUM_ENTRIES) && (OAM_NULL != (xmlNode *)temp1))
                        ; count++)
                {
                    p_fapService_xml_st->CellConfig.LTECell_NLIU[count] = (xmlNode*)temp1;
                    /* SPR 20430 Changes Start*/
                    temp1 = (((xmlNode *)temp1)->next);
                    /* SPR 20430 Changes End*/
                    if(count < 3)
                    {
                        if( p_fapService_xml_st->CellConfig.LTECell_NLIU[count]->type == XML_COMMENT_NODE)
                        {
                            count--;
                            continue;
                        }
                    }
                }
            }

            flag_found = OAM_ZERO;
            UInt32 max_num_of_umts_cells_nliu = oam_atoi((const char *)oam_xmlNodeGetContent(
                        p_fapService_xml_st->CellConfig.InterRATCell_2,"MaxUMTSEntries"));
            UInt32 num_of_umts_cells_nliu = oam_atoi((const char *)oam_xmlNodeGetContent(
                        p_fapService_xml_st->CellConfig.InterRATCell_2,"UMTSNumberOfEntries"));
            if(num_of_umts_cells_nliu > max_num_of_umts_cells_nliu)
            {
                OAM_LOG(OAM,OAM_CRITICAL,
                        "UMTSNumberOfEntries[%u] > MaxUMTSEntries[%u] in NeighborListInUse", 
                        num_of_umts_cells_nliu, max_num_of_umts_cells_nliu);
            }

            else
            {
                oam_xml_find_ptr(p_fapService_xml_st->CellConfig.InterRATCell_2,&temp1,"UMTS");
                for(count= OAM_ZERO;(( count < (SInt64)num_of_umts_cells_nliu) && (count < OAM_MAX_UMTS_NBR_CELL_NUM_ENTRIES)
                            && ( OAM_NULL != (xmlNode *)temp1 )); count++)
                {
                    p_fapService_xml_st->CellConfig.UMTS_NLIU[count] = (xmlNode*)temp1;
                    /* SPR 20430 Changes Start*/
                    temp1 = (((xmlNode *)temp1)->next);
                    /* SPR 20430 Changes End*/
                    if(count < OAM_THREE)
                    {
                        if(p_fapService_xml_st->CellConfig.UMTS_NLIU[count]->type == XML_COMMENT_NODE)
                        {
                            count--;
                            continue;
                        }
                    }
                }
            }
            /*SPR 15604 END */



            flag_found = OAM_ZERO;
            tag_value = (Char8*)oam_xmlNodeGetContent(
                    p_fapService_xml_st->CellConfig.NeighborList,
                                        "LTECellNumberOfEntries");
            UInt32 num_of_lte_cells = oam_atoi(tag_value);
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.NeighborList,
                             &temp1, "LTECell");
            count = OAM_ZERO;
            while((count < (SInt64)num_of_lte_cells) &&
			    (count < RRM_OAM_MAX_NUM_INTRA_FREQ_CELLS) && temp1) {
		    if(((xmlNode*)temp1)->type != XML_COMMENT_NODE) {
                    p_fapService_xml_st->CellConfig.LTECell[count++] = (xmlNode*)temp1;
		    }
    /* SPR 20430 Changes Start*/
		    temp1 = (((xmlNode *)temp1)->next);
    /* SPR 20430 Changes End*/
	    }

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.NeighborList,
                             &temp1, "InterRATCell");
            p_fapService_xml_st->CellConfig.InterRATCell_1 = (xmlNode*)temp1;	


            flag_found = OAM_ZERO;
            tag_value = (Char8*)oam_xmlNodeGetContent(
                    p_fapService_xml_st->CellConfig.InterRATCell_1,
                    "UMTSNumberOfEntries");
            UInt32 num_of_umts_cells = oam_atoi(tag_value);
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.InterRATCell_1,
                    &temp1, "UMTS");
            count= OAM_ZERO;
            while((count < (SInt64)num_of_umts_cells) &&
                    (count < RRM_OAM_MAX_NUM_INTER_FREQ_CELLS) && temp1) {
                if(((xmlNode *)temp1)->type != XML_COMMENT_NODE) {
                    p_fapService_xml_st->CellConfig.UMTS[count++] = (xmlNode*)temp1;
                }
                /* SPR 20430 Changes Start*/
                temp1 = (((xmlNode *)temp1)->next);
                /* SPR 20430 Changes End*/
            }


            flag_found = OAM_ZERO;	 
            tag_value = (Char8*)oam_xmlNodeGetContent(
                    p_fapService_xml_st->CellConfig.InterRATCell_1,
                    "GSMNumberOfEntries");
            UInt32 num_of_gsm = oam_atoi(tag_value);

            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.InterRATCell_1,
                    &temp1, "GSM");
            count = OAM_ZERO;
            while((count < (SInt64)num_of_gsm) &&
                    (count < RRM_OAM_MAX_NUM_GERAN_FREQ_CELLS) && temp1) {
                if(((xmlNode *)temp1)->type != XML_COMMENT_NODE) {
                    p_fapService_xml_st->CellConfig.GSM[count++] = (xmlNode*)temp1;
                }
                /* SPR 20430 Changes Start*/
                temp1 = (((xmlNode *)temp1)->next);
                /* SPR 20430 Changes End*/
            }

            flag_found = OAM_ZERO;
            tag_value = (Char8*)oam_xmlNodeGetContent(
                    p_fapService_xml_st->CellConfig.InterRATCell_1,
                    "CDMA2000NumberOfEntries");
            UInt32 num_of_cdma2000 = oam_atoi(tag_value);

            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.InterRATCell_1,
                    &temp1, "CDMA2000");
            count = OAM_ZERO;
            while((count < (SInt64)num_of_cdma2000) &&
                    (count < RRM_OAM_NUM_CDMA2000_FREQ_CELLS ) && temp1) {
                if(((xmlNode *)temp1)->type != XML_COMMENT_NODE) {
                    p_fapService_xml_st->CellConfig.CDMA2000_2[count++] =
                        (xmlNode*)temp1;	
                }
                /* SPR 20430 Changes Start*/
                temp1 = (((xmlNode*)temp1)->next);
                /* SPR 20430 Changes End*/
            }

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_cell_config_tag, &temp1, "X_VENDOR_ICIC_PARAMS");
            p_fapService_xml_st->CellConfig.X_VENDOR_ICIC_PARAMS = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.X_VENDOR_ICIC_PARAMS,
                    &temp1, "dynamic_icic_info");
            p_fapService_xml_st->CellConfig.dynamic_icic_info = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.dynamic_icic_info,
                    &temp1, "dl_resource_partition_info");
            p_fapService_xml_st->CellConfig.dl_resource_partition_info = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            region = OAM_ZERO;
            tag_value = (Char8*)oam_xmlNodeGetContent(
                    p_fapService_xml_st->CellConfig.dl_resource_partition_info,
                    "num_of_cell_center_region");
            region = oam_atoi(tag_value);

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.dl_resource_partition_info,
                    &temp1, "cell_center_region");

            count = OAM_ZERO;
            while((count < RRM_OAM_MAX_CC_REGION) && (count < (SInt64)region) && temp1) {
                if(((xmlNode*)temp1)->type != XML_COMMENT_NODE) {
                    if(!oam_strcmp((Char8*)(((xmlNode*)temp1)->name),
                                "cell_center_region")) {
                        p_fapService_xml_st->CellConfig.dl_cell_center_region[count++] =
                            (xmlNode*)temp1;
                    }
                }

                /* SPR 20430 Changes Start*/
                temp1 = (((xmlNode*)temp1)->next);
                /* SPR 20430 Changes End*/
            }


            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.dl_resource_partition_info,
                    &temp1, "cell_edge_region");
            p_fapService_xml_st->CellConfig.dl_cell_edge_region = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.dynamic_icic_info,
                    &temp1, "ul_resource_partition_info");
            p_fapService_xml_st->CellConfig.ul_resource_partition_info = (xmlNode*)temp1;

            region = OAM_ZERO;
            tag_value = (Char8*)oam_xmlNodeGetContent(
                    p_fapService_xml_st->CellConfig.ul_resource_partition_info,
                    "num_of_cell_center_region");
            region = oam_atoi(tag_value);

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.ul_resource_partition_info,
                    &temp1, "cell_center_region");

            count = OAM_ZERO;
            while((count < RRM_OAM_MAX_CC_REGION) && (count < (SInt64)region) && temp1) {
                if(((xmlNode*)temp1)->type != XML_COMMENT_NODE) {
                    if(!oam_strcmp((Char8*)(((xmlNode*)temp1)->name),
                                "cell_center_region")) {
                        p_fapService_xml_st->CellConfig.ul_cell_center_region[count++] =
                            (xmlNode*)temp1;
                    }
                }

                /* SPR 20430 Changes Start*/
                temp1 = (((xmlNode *)temp1)->next);
                /* SPR 20430 Changes End*/
            }

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.ul_resource_partition_info,
                    &temp1, "cell_edge_region");
            p_fapService_xml_st->CellConfig.ul_cell_edge_region = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.dynamic_icic_info,
                    &temp1, "ul_power_mask");
            p_fapService_xml_st->CellConfig.ul_power_mask = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.dynamic_icic_info,
                    &temp1, "rntp_report_config_info");
            p_fapService_xml_st->CellConfig.rntp_report_config_info = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.dynamic_icic_info,
                    &temp1, "alpha_pathloss_target_sinr_map");
            p_fapService_xml_st->CellConfig.alpha_pathloss_target_sinr_map = (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(
                    p_fapService_xml_st->CellConfig.alpha_pathloss_target_sinr_map,
                    &temp1, "path_loss_target_sinr_map");
            p_fapService_xml_st->CellConfig.path_loss_target_sinr_map = (xmlNode*)temp1;

            region = OAM_ZERO;
            tag_value = (Char8*)oam_xmlNodeGetContent(
                    p_fapService_xml_st->CellConfig.path_loss_target_sinr_map,
                    "count");
            region = oam_atoi(tag_value);

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.path_loss_target_sinr_map,
                    &temp1, "path_loss_to_target_sinr_map_info");

            count = OAM_ZERO;
            while((count < RRM_OAM_MAX_NUM_PATHLOSS_TO_SINR_COUNT) && temp1) {
                if(((xmlNode*)temp1)->type != XML_COMMENT_NODE) {
                    if(!oam_strcmp((Char8*)(((xmlNode*)temp1)->name),
                                "path_loss_to_target_sinr_map_info")) {
                        p_fapService_xml_st->CellConfig.
                            path_loss_to_target_sinr_map_info[count++] =
                            (xmlNode*)temp1;
                    }
                }

                /* SPR 20430 Changes Start*/
                temp1 = (((xmlNode*)temp1)->next);
                /* SPR 20430 Changes End*/
            }

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.dynamic_icic_info,
                    &temp1, "alpha_pathloss_target_sinr_map");
            p_fapService_xml_st->CellConfig.alpha_pathloss_target_sinr_map_sps =
                (xmlNode*)temp1;

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(
                    p_fapService_xml_st->CellConfig.alpha_pathloss_target_sinr_map_sps,
                    &temp1, "path_loss_target_sinr_map");
            p_fapService_xml_st->CellConfig.path_loss_target_sinr_map_sps[OAM_ZERO] =
                (xmlNode*)temp1;

            region = OAM_ZERO;
            tag_value = (Char8*)oam_xmlNodeGetContent(
                    p_fapService_xml_st->CellConfig.path_loss_target_sinr_map_sps[0],
                    "count");
            region = oam_atoi(tag_value);

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(
                    p_fapService_xml_st->CellConfig.path_loss_target_sinr_map_sps[OAM_ZERO],
                    &temp1, "path_loss_to_target_sinr_map_info");

            count = OAM_ZERO;
            while((count < RRM_OAM_MAX_NUM_PATHLOSS_TO_SINR_COUNT) && temp1 &&
                    (count < (SInt64)region)) {
                if(((xmlNode*)temp1)->type != XML_COMMENT_NODE) {
                    if(!oam_strcmp((Char8*)(((xmlNode*)temp1)->name),
                                "path_loss_to_target_sinr_map_info")) {
                        p_fapService_xml_st->CellConfig.
                            path_loss_to_target_sinr_map_info_sps[count++] =
                            (xmlNode*)temp1;
                    }
                }

                /* SPR 20430 Changes Start*/
                temp1 = (((xmlNode *)temp1)->next);
                /* SPR 20430 Changes End*/
            }

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.dynamic_icic_info,
                    &temp1, "atb_config");
            p_fapService_xml_st->CellConfig.atb_config = (xmlNode*)temp1;

            flag_found = 0;
            oam_xml_find_ptr(p_cell_config_tag, &temp1,
                    "X_VENDOR_CARRIER_AGGREGATION_PARAMS");
            if(flag_found) {
                p_fapService_xml_st->CellConfig.ca_params = (xmlNode*)temp1;
            }
            /* SPR 20653 Fix Start */
            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.dynamic_icic_info, &temp1,
                                 "dci_params");
            if(flag_found) {
                p_fapService_xml_st->CellConfig.dci_params = (xmlNode*)temp1;
            }

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.dynamic_icic_info, &temp1,
                                 "cooloff_period");
            if(flag_found) {
                p_fapService_xml_st->CellConfig.cooloff_period = (xmlNode*)temp1;
            }

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.dynamic_icic_info, &temp1,
                                 "delta_sinr_tpc_map_pusch_info");
            if(flag_found) {
                p_fapService_xml_st->CellConfig.delta_sinr_tpc_map_pusch_info = (xmlNode*)temp1;
            }

            region = OAM_ZERO;
            tag_value = (Char8*)oam_xmlNodeGetContent(
                                 p_fapService_xml_st->CellConfig.delta_sinr_tpc_map_pusch_info,
                                 "delta_sinr_count");
            region = oam_atoi(tag_value);

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.delta_sinr_tpc_map_pusch_info,
                             &temp1, "delta_sinr_to_tpc_map_pusch_info");

            count = OAM_ZERO;
            while((count < RRM_OAM_MAX_DELTA_SINR_TO_TCP_MAP_PUSCH) && temp1) {
                if(((xmlNode*)temp1)->type != XML_COMMENT_NODE) {
                    if(!oam_strcmp((Char8*)(((xmlNode*)temp1)->name),
                                   "delta_sinr_to_tpc_map_pusch_info")) {
                        p_fapService_xml_st->CellConfig.
                            delta_sinr_to_tpc_map_pusch_info[count++] =
                                                            (xmlNode*)temp1;
                    }
                }

                temp1 = (((xmlNode*)temp1)->next);
            }

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.dynamic_icic_info, &temp1,
                                 "bler_to_tpc_map_for_pucch");
            if(flag_found) {
                p_fapService_xml_st->CellConfig.bler_to_tpc_map_for_pucch = (xmlNode*)temp1;
            }

            region = OAM_ZERO;
            tag_value = (Char8*)oam_xmlNodeGetContent(
                                 p_fapService_xml_st->CellConfig.bler_to_tpc_map_for_pucch,
                                 "bler_count");
            region = oam_atoi(tag_value);

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.bler_to_tpc_map_for_pucch,
                             &temp1, "delta_bler_tpc_map_pucch_info");

            count = OAM_ZERO;
            while((count < RRM_OAM_MAX_NUM_TPC_FOR_PUCCH) && temp1) {
                if(((xmlNode*)temp1)->type != XML_COMMENT_NODE) {
                    if(!oam_strcmp((Char8*)(((xmlNode*)temp1)->name),
                                   "delta_bler_tpc_map_pucch_info")) {
                        p_fapService_xml_st->CellConfig.
                            delta_bler_tpc_map_pucch_info[count++] =
                                                            (xmlNode*)temp1;
                    }
                }

                temp1 = (((xmlNode*)temp1)->next);
            }

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.dynamic_icic_info, &temp1,
                                 "delta_sinr_tpc_map_pucch_info");
            if(flag_found) {
                p_fapService_xml_st->CellConfig.delta_sinr_tpc_map_pucch_info = (xmlNode*)temp1;
            }

            region = OAM_ZERO;
            tag_value = (Char8*)oam_xmlNodeGetContent(
                                 p_fapService_xml_st->CellConfig.delta_sinr_tpc_map_pucch_info,
                                 "delta_sinr_pucch_count");
            region = oam_atoi(tag_value);

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.delta_sinr_tpc_map_pucch_info,
                             &temp1, "delta_sinr_to_tpc_map_pucch_info");

            count = OAM_ZERO;
            while((count < RRM_OAM_MAX_NUM_TPC_FOR_PUCCH) && temp1) {
                if(((xmlNode*)temp1)->type != XML_COMMENT_NODE) {
                    if(!oam_strcmp((Char8*)(((xmlNode*)temp1)->name),
                                   "delta_sinr_to_tpc_map_pucch_info")) {
                        p_fapService_xml_st->CellConfig.
                            delta_sinr_to_tpc_map_pucch_info[count++] =
                                                            (xmlNode*)temp1;
                    }
                }

                temp1 = (((xmlNode*)temp1)->next);
            }

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.dynamic_icic_info, &temp1,
                                 "report_config_info");
            if(flag_found) {
                p_fapService_xml_st->CellConfig.report_config_info = (xmlNode*)temp1;
            }

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.report_config_info, &temp1,
                                 "l1_meas_report_config_info");
            if(flag_found) {
                p_fapService_xml_st->CellConfig.l1_meas_report_config_info = (xmlNode*)temp1;
            }

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.report_config_info, &temp1,
                                 "l2_meas_report_config_info");
            if(flag_found) {
                p_fapService_xml_st->CellConfig.l2_meas_report_config_info = (xmlNode*)temp1;
            }

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_cell_config_tag, &temp1,
                                 "X_VENDOR_OLPC_CONFIG_PARAMS");
            if(flag_found) {
                p_fapService_xml_st->CellConfig.X_VENDOR_OLPC_CONFIG_PARAMS = (xmlNode*)temp1;
            }

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.X_VENDOR_OLPC_CONFIG_PARAMS, &temp1,
                                 "olpc_pusch_supported_category");
            if(flag_found) {
                p_fapService_xml_st->CellConfig.olpc_pusch_supported_category = (xmlNode*)temp1;
            }
            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.olpc_pusch_supported_category,
                             &temp1, "olpc_tupple");

            count = OAM_ZERO;
            while((count < RRM_OLPC_CATEGORY_TABLE_SIZE) && temp1) {
                if(((xmlNode*)temp1)->type != XML_COMMENT_NODE) {
                    if(!oam_strcmp((Char8*)(((xmlNode*)temp1)->name),
                                   "olpc_tupple")) {
                        p_fapService_xml_st->CellConfig.
                            olpc_tupple_pusch[count++] = (xmlNode*)temp1;
                    }
                }

                temp1 = (((xmlNode*)temp1)->next);
            }

            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.X_VENDOR_OLPC_CONFIG_PARAMS, &temp1,
                                 "olpc_pucch_supported_category");
            if(flag_found) {
                p_fapService_xml_st->CellConfig.olpc_pucch_supported_category = (xmlNode*)temp1;
            }
            flag_found = OAM_ZERO;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.olpc_pucch_supported_category,
                             &temp1, "olpc_tupple");

            count = OAM_ZERO;
            while((count < RRM_OLPC_CATEGORY_TABLE_SIZE) && temp1) {
                if(((xmlNode*)temp1)->type != XML_COMMENT_NODE) {
                    if(!oam_strcmp((Char8*)(((xmlNode*)temp1)->name),
                                   "olpc_tupple")) {
                        p_fapService_xml_st->CellConfig.
                            olpc_tupple_pucch[count++] = (xmlNode*)temp1;
                    }
                }

                temp1 = (((xmlNode*)temp1)->next);
            }

            /* SPR 20653 Fix End */

            flag_found = 0;
            oam_xml_find_ptr(p_fapService_xml_st->CellConfig.ca_params,
                    &temp1, "MACSchedParams");
            if(flag_found) {
                p_fapService_xml_st->CellConfig.ca_mac_sched_params = (xmlNode*)temp1;
                p_rrm_cell_config->operator_info.ca_config.
                    bitmask |= RRM_OAM_CA_MAC_SCHEDULER_PARAMS_INFO_PRESENT;
            }
            /* eICIC_PHASE_1_2_CHANGES_START */ 
            flag_found = 0;
            oam_xml_find_ptr(p_cell_config_tag, &temp1,
                    "X_VENDOR_eICIC_PARAMS");
            if(flag_found) {

                p_fapService_xml_st->CellConfig.eicic_params = (xmlNode*)temp1;
                flag_found = 0;
                oam_xml_find_ptr(p_fapService_xml_st->CellConfig.eicic_params, &temp1,
                        "AbsInfo");

                if(flag_found) {
                    p_fapService_xml_st->CellConfig.AbsInfo= (xmlNode*)temp1;
                    p_rrm_cell_config->operator_info.eicic_info.
                        bitmask |= RRM_OAM_ABS_INFO_PRESENT;

                    flag_found = 0;
                    oam_xml_find_ptr(p_fapService_xml_st->CellConfig.AbsInfo, &temp1,
                            "AbsPattern");

                    if(flag_found) {
                        p_fapService_xml_st->CellConfig.AbsPattern= (xmlNode*)temp1;
                        p_rrm_cell_config->operator_info.eicic_info.abs_info.
                            bitmask |= RRM_OAM_ABS_PATTERN_INFO_PRESENT;
                    }
                }

                flag_found = 0;
                oam_xml_find_ptr(p_fapService_xml_st->CellConfig.eicic_params, &temp1,
                        "eICICTimerInfo");

                if(flag_found) {
                    p_fapService_xml_st->CellConfig.eICICTimerInfo= (xmlNode*)temp1;
                    p_rrm_cell_config->operator_info.eicic_info.
                        bitmask |= RRM_OAM_EICIC_TIMER_INFO_PRESENT;
                }
            }

            /*eICIC_PHASE_1_2_CHANGES_END */ 
        }
        /*
         ** End cell config parameter.
         */

        if(!fap_index)
        {
            flag_found = OAM_ZERO;	       
            oam_xml_find_ptr(p_fap_service_tag,&temp1,"REM");
            p_fapService_xml_st->REM = (xmlNode*)temp1;	

            flag_found = OAM_ZERO;	        
            oam_xml_find_ptr(p_fapService_xml_st->REM,&temp1,"LTE");
            p_fapService_xml_st->LTE_5 = (xmlNode*)temp1;	

            flag_found = OAM_ZERO;	       
            oam_xml_find_ptr(p_fapService_xml_st->LTE_5, &temp1,"CarrierMeas");
            p_fapService_xml_st->CarrierMeas = (xmlNode*)temp1;	

            /*SPR_16048 Fix Start*/ 
            UInt32 num_cell_entries = OAM_ZERO;
            num_cell_entries=oam_atoi((const char *)oam_xmlNodeGetContent(
                        p_fapService_xml_st->LTE_5,"CellNumberOfEntries"));

            flag_found = OAM_ZERO;	       
            oam_xml_find_ptr(p_node,&temp1,"Cell");
            if(flag_found)
            {
                for(count = OAM_ZERO; count < (SInt32)num_cell_entries && temp1 ; count++)
                {
                    p_fapService_xml_st->Cell[count] = (xmlNode*)temp1;	
                    /* SPR 20430 Changes Start*/
                    temp1 = (((xmlNode *)temp1)->next);
                    /* SPR 20430 Changes End*/
                }
            }

            if(p_fapService_xml_st->Cell[OAM_ZERO])
            {
                flag_found = OAM_ZERO;	      
                oam_xml_find_ptr(p_fapService_xml_st->Cell[OAM_ZERO],&temp1,"RF");
                if(flag_found)
                {
                    p_fapService_xml_st->RF_2 = (xmlNode*)temp1;	
                }


                flag_found = OAM_ZERO;	      
                oam_xml_find_ptr(p_fapService_xml_st->Cell[OAM_ZERO],&temp1,"BCCH");
                if(flag_found)
                {
                    p_fapService_xml_st->BCCH = (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;
                oam_xml_find_ptr(p_fapService_xml_st->BCCH,&temp1,"PLMNList");
                if(flag_found)
                {
                    p_fapService_xml_st->PLMNList_2 = (xmlNode*)temp1;
                }

                flag_found = OAM_ZERO;	      
                oam_xml_find_ptr(p_fap_service_tag,&temp1,"Transport");
                p_fapService_xml_st->Transport = (xmlNode*)temp1;	

                flag_found = OAM_ZERO;	      
                oam_xml_find_ptr(p_fap_service_tag,&temp1,"SCTP");
                p_fapService_xml_st->SCTP = (xmlNode*)temp1;	

            }
            /*SPR_16048 Fix End*/ 
        }
        p_fap_service_tag = p_fap_service_tag->next;
        fap_index++;
    }

    oam_prov_req.num_cells_configured = fap_index;
    /* spr_21013_fix_end */
    
    /* SPR 16045 Fix Start */
    /* Code Removed */
    /* SPR 16045 Fix End */

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"FaultMgmt");
    xml_struct_tr196_g.FaultMgmt = (xmlNode*)temp1;

    max_temp=oam_atoi((const char *)oam_xmlNodeGetContent(
    xml_struct_tr196_g.FaultMgmt,"SupportedAlarmNumberOfEntries"));

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_node,&temp1,"SupportedAlarm");
    for(count = LOW_COUNT + OAM_ONE; count <= max_temp; count++)
        {
                xml_struct_tr196_g.SupportedAlarm[count - OAM_ONE] = (xmlNode*)temp1;
    /* SPR 20430 Changes Start*/
               temp1 = (((xmlNode *)temp1)->next);
    /* SPR 20430 Changes End*/
        }

OAM_FUN_EXIT_TRACE();
    return retVal;
}

/*******************************************************************************
* Function Name  : oam_xml_struct_nw_scan_populate()
* Description    : This function is used to populate the global structure of 
                 : xml_struct_tr196.
* Inputs         : xmlNode type  pointer of the parent node
* Outputs        : none
* Returns        : none
*******************************************************************************/
void
oam_xml_struct_nw_scan_populate
(
    xmlNode *p_node
)
{
    /* SPR 20430 Changes Start*/
    xmlNode *temp_node = OAM_ZERO;
    /* SPR 20430 Changes End*/

    OAM_FUN_ENTRY_TRACE();
    flag_found = 0;
    oam_xml_find_ptr(p_node,&temp_node,"InternetGatewayDevice");
    /*finds InternetGatewayDevice in the tree using xmlNode pointer p_node*/
    xml_struct_nw_scan.InternetGatewayDevice = (xmlNode*)temp_node;
     

    flag_found =0;
    oam_xml_find_ptr(xml_struct_nw_scan.InternetGatewayDevice,&temp_node,"Services");
    xml_struct_nw_scan.Services = (xmlNode*)temp_node;
    /*sets the flag to 0 each time when a new element is to be found*/
    flag_found =0;
    oam_xml_find_ptr(xml_struct_nw_scan.Services, &temp_node,"FAPService");
    /*finds FAPService in the tree using xmlNode pointer p_node*/
    xml_struct_nw_scan.FAPService = (xmlNode*)temp_node;
    /*populates the xmlNode Pointer in structure*/
    flag_found =0;
    oam_xml_find_ptr(xml_struct_nw_scan.FAPService, &temp_node,"REM");
    xml_struct_nw_scan.REM = (xmlNode*)temp_node;

    flag_found =0;
    oam_xml_find_ptr(xml_struct_nw_scan.REM,&temp_node,"LTE");
    xml_struct_nw_scan.LTE = (xmlNode*)temp_node;

    OAM_FUN_EXIT_TRACE();
}

/*******************************************************************************
 *  FUNCTION NAME    : oam_validate_tr196_schema
 *  DESCRIPTION      : This function used to validate the tr196 xml with schema
 *                     available in xml file
 *  INPUT            : p_doc2 :Pointer to pointer to xml
 *  OUTPUT           : None
 *  RETURNS          : None
*******************************************************************************/

oam_return_et
oam_validate_tr196_schema(xmlDoc **p_doc2)
{
    UInt16 validate_status; 
    Char8* filename = "Schema_log.log";
	Char8* XSDFileName = "../cfg/eNodeB_Data_Model_TR_196_based.xsd";
    oam_return_et retVal = OAM_FAILURE;
    xmlSchemaParserCtxtPtr p_oam_schema_ctxt = OAM_NULL; 
    xmlSchemaPtr  p_schema = OAM_NULL;
    OAM_FUN_ENTRY_TRACE();

    prov_req.p_schema_log = oam_fopen(filename, "w+");
    if(!prov_req.p_schema_log) {
         OAM_LOG(OAM, OAM_ERROR,
               "Unable to open Schema_log.log for xml schema related logs");
         OAM_FUN_EXIT_TRACE();
         return retVal;
    }

    xmlLineNumbersDefault(OAM_ONE);
    p_oam_schema_ctxt = xmlSchemaNewParserCtxt(XSDFileName);

    if(!p_oam_schema_ctxt) {
         OAM_LOG(OAM, OAM_ERROR, "Unable to create schema parser context");
         oam_fclose(prov_req.p_schema_log);

         OAM_FUN_EXIT_TRACE();
         return retVal;
    }
    xmlSchemaSetParserErrors(p_oam_schema_ctxt,
                            (xmlSchemaValidityErrorFunc)fprintf,
                            (xmlSchemaValidityWarningFunc)fprintf,
                            prov_req.p_schema_log);

    xmlSchemaSetParserErrors(p_oam_schema_ctxt,
                             (xmlSchemaValidityErrorFunc)fprintf,
                             OAM_NULL,
                             prov_req.p_schema_log);

    p_schema = xmlSchemaParse(p_oam_schema_ctxt);
	if (!p_schema) {
        OAM_LOG(OAM, OAM_ERROR, "Could not parse XSD schema !!!");

        oam_fclose(prov_req.p_schema_log);
        OAM_FUN_EXIT_TRACE();
        return retVal;
    }

    xmlSchemaFreeParserCtxt(p_oam_schema_ctxt);

    oam_schema_valid_ctxt = xmlSchemaNewValidCtxt(p_schema);
    xmlSchemaSetValidErrors(oam_schema_valid_ctxt,
                            (xmlSchemaValidityErrorFunc)fprintf,
                            (xmlSchemaValidityWarningFunc)fprintf,
                            prov_req.p_schema_log);

    // populate xml tree with default values
    xmlSchemaSetValidOptions(oam_schema_valid_ctxt,
                            XML_SCHEMA_VAL_VC_I_CREATE);
      

    validate_status = xmlSchemaValidateDoc(oam_schema_valid_ctxt,
                                           *p_doc2); //validate xml


	if(OAM_ZERO == validate_status) {
        OAM_LOG(OAM, OAM_INFO, "XML Validation Successful");
        retVal = OAM_SUCCESS;
    }
    else {
        OAM_LOG(OAM, OAM_ERROR, "XML Validation Fails");
    }
    
    /*SPR 18114 Start*/
    xmlSchemaFreeValidCtxt(oam_schema_valid_ctxt);
    xmlSchemaFree(p_schema);
    /*SPR 18114 End*/

    oam_fclose(prov_req.p_schema_log);
    OAM_FUN_EXIT_TRACE();
    return retVal;
}



/*******************************************************************************
 *  FUNCTION NAME    : oam_libxml_init
 *  DESCRIPTION      : This function used to inintialize the oam with parameters
 *                     available in xml file
 *  PARAMETERS       
 *      IN           : arg[1] xml file name containing non TR196 parameters
 *                     arg[2] xml file name containing TR196 parameters
 *                   
 *      OUT          : NONE
 *                   
 *  RETURNS          : OAM_SUCCESS/OAM_FAILURE
*******************************************************************************/
/*OAM_REVIEW_CHANGES*/
oam_return_et
oam_libxml_init(SInt32 argc, Char8 *arg[])
{
    OAM_FUN_ENTRY_TRACE();
    static SInt32            first_time                 = OAM_TRUE;
    xmlDoc               *p_xmldoc_prop                 = OAM_NULL;
    xmlDoc                   *p_xmldoc_tr196            = OAM_NULL;
    oam_return_et           ret_val                     = OAM_SUCCESS;
    UInt8                    cell_cnt                   = 0;
    rrc_oam_sector_info_t    *p_rrc_cell_info           = OAM_NULL;
    x2apOam_srvd_cell_arry_t *p_x2ap_cell_info          = OAM_NULL;
    XML_struct_cell_config   *p_cell_spec_params        = OAM_NULL;
    Char8 *p_tr196_filename                             = OAM_NULL;
    xmlNode *p_tr196_xml_root = OAM_NULL;
    xmlNode *p_prop_xml_root = OAM_NULL;
    prop_cellconfig_xml_st *p_prop_cellconfig = OAM_NULL;
    oam_rrm_cell_config_req_t *p_rrm_cell_config = OAM_NULL;
/* SPR 22248 MBMS Changes Start */
    XML_struct_fap_service *p_cell_specFAPService = OAM_NULL;
/* SPR 22248 MBMS Changes Stop */
    oam_l2_req_t *p_l2_req = OAM_NULL;

/* SPR 11001 FIX START */
    rrm_oam_serving_enb_cell_info_t *p_srv_enb_cell_info = OAM_NULL;
    oam_rrm_cell_plmnlist_data_t *p_plmnlist_data = OAM_NULL;
/* SPR 11001 FIX END */
    /* SPR 15527 START */
    oam_rrm_qos_data_t *p_qos_data = OAM_NULL;
    /* SPR 15527 END */                                                  

    UInt8 cid = OAM_ZERO;

    UInt16 i = OAM_ZERO;
    UInt16 max_count = OAM_ZERO;
    UInt16 tbl_size = OAM_ZERO;
    /*SPR 21096 Start*/
    UInt8 l2_instance = OAM_ZERO;
    /*SPR 21096 End*/	

#ifdef OAM_SON_ENABLE
    oam_cell_neighbourlist_info_t *p_nr_info = OAM_NULL;
    son_cell_t *p_serving_cell = OAM_NULL; 
    oam_son_inter_freq_carrier_t *p_son_inter_freq_carr_params = OAM_NULL;
    EnergySavingConfig_t *p_es_config = OAM_NULL;
    oam_son_neigh_list_in_use_t  *p_neigh_list_in_use = OAM_NULL;
/* SPR 13024 FIX START */
    oam_son_earfcn_pci_selection_t *p_earfcn_pci_selection = OAM_NULL;
/* SPR 13024 FIX END */
    oam_son_pci_selection_req_t  *p_pci_selection_req = OAM_NULL;
    oam_son_pci_reselection_req_t  *p_pci_reselection_req = OAM_NULL;

/* SPR 13065 CHANGE START */
/* SPR 13065 CHANGE END */
/* SPR 13024 FIX START */
       /* Code moved */
/* SPR 13024 FIX END */
    oam_son_carrier_freq_and_dl_tx_power_req_t
                                   *p_earfcn_selection_req = OAM_NULL;
    oam_cell_carrier_params_t *p_cell_carr_params = OAM_NULL;
#endif
#ifdef LTE_EMBMS_SUPPORTED
    /* SPR 18289 Fix Start */
    Char8* tag_value_embms = OAM_NULL;
    /* SPR 18289 Fix End */
    m2ap_cell_specific_parameters_t    *p_m2ap_cell_spec_params = OAM_NULL;
#endif

   if(argc < OAM_MIN_COMMANDLINE_ARGS || argc > OAM_MAX_COMMANDLINE_ARGS) {
       OAM_LOG(OAM, OAM_ERROR, "Invalid commandline arguments."
                               "USAGE:%s <Proprietary XML config file> "
                               "<TR196 XML config file> [Core Number]",
                               arg[0]);
       OAM_FUN_EXIT_TRACE();
       return OAM_FAILURE;
    }

    LIBXML_TEST_VERSION

    if(OAM_FALSE == first_time) {
        /* Free previous memory for xml Document */
        xmlFreeDoc(oam_prov_req.system_status.p_propriety_xml_doc);
        oam_prov_req.system_status.p_propriety_xml_doc = OAM_NULL;

        xmlFreeDoc(oam_prov_req.system_status.p_xml_doc);
        oam_prov_req.system_status.p_xml_doc = OAM_NULL;

        /* read downloaded tr196 xml file */
        p_tr196_filename = "../cfg/tr069.xml";
    }
    else {
        p_tr196_filename = arg[MANDATORY_TR196_XML_ARG_INDEX];
        first_time = OAM_FALSE;
    }

    p_xmldoc_tr196 = xmlReadFile(p_tr196_filename, OAM_NULL,
                                     XML_PARSE_NOBLANKS);
    if(!p_xmldoc_tr196) {
        OAM_LOG(OAM, OAM_ERROR,
                     "Failed to read the xml file: %s",
                     p_tr196_filename);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    oam_prov_req.system_status.p_xml_doc = p_xmldoc_tr196;

    p_xmldoc_prop = xmlReadFile(arg[NON_TR196_XML_ARG_INDEX],
                                OAM_NULL, XML_PARSE_NOBLANKS);

    if(!p_xmldoc_prop) {
        OAM_LOG(OAM, OAM_ERROR,
                          "Failed to read the xml file: %s",
                          arg[NON_TR196_XML_ARG_INDEX]);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    oam_prov_req.system_status.p_propriety_xml_doc = p_xmldoc_prop;

    if(OAM_MAX_COMMANDLINE_ARGS == argc) {
        if(!oam_isnumber(arg[--argc])) {
            OAM_LOG(OAM, OAM_ERROR,
                         "Last argument must always be core_num!!!");
            OAM_FUN_EXIT_TRACE();
            return OAM_FAILURE;
        }
    }


    /* TRI Specific code */
    p_tr196_xml_root = xmlDocGetRootElement(p_xmldoc_tr196);
    ret_val = oam_validate_tr196_schema(&p_xmldoc_tr196);

    if(OAM_FAILURE == ret_val) {
        OAM_LOG(OAM, OAM_ERROR, "TR196 schema validation failed");
        OAM_FUN_EXIT_TRACE();
        return ret_val;
    }

    ret_val = oam_xml_struct_tr196_populate(p_tr196_xml_root);

    if(OAM_FAILURE == ret_val) {
        OAM_LOG(OAM, OAM_ERROR, "TR196 xml poopulation failed");
        OAM_FUN_EXIT_TRACE();
        return ret_val;
    }

    p_prop_xml_root = xmlDocGetRootElement(p_xmldoc_prop);

    ret_val = oam_xml_struct_populate(p_prop_xml_root);
    if(OAM_FAILURE == ret_val) {
        OAM_LOG(OAM, OAM_ERROR,
                     "Population of Proprietary XML failed !!!");
        OAM_FUN_EXIT_TRACE();
        return ret_val;
    }

    if(OAM_SUCCESS == oam_populate_igd_param())
    {
        OAM_LOG(OAM, OAM_DETAILED,"parameters populated for InternetGatewayDevice Successfully");
    }

    if(OAM_SUCCESS == oam_populate_device_info_param())
    {   
        OAM_LOG(OAM, OAM_DETAILED,"parameters populated for DEVICE INFO Successfully");
    }   

    if(OAM_SUCCESS == oam_populate_rem_lte_param())
    {
        OAM_LOG(OAM, OAM_DETAILED,"parameters populated for REM_LTE Successfully");
    }

    if(OAM_SUCCESS == oam_populate_tunnel_param())
    {
        OAM_LOG(OAM, OAM_DETAILED,"parameters populated for TUNNEL Successfully");
    }

    if(OAM_SUCCESS == oam_populate_capabilities_param())
    {
        OAM_LOG(OAM, OAM_DETAILED,"parameters populated for CAPABILITIES Successfully");
    }

    if(OAM_SUCCESS == oam_populate_perf_mgmt_param())
    {
        OAM_LOG(OAM, OAM_DETAILED,"parameters populated for PERF MANAGEMENT Successfully");
    }

    if(OAM_SUCCESS == oam_populate_services_param())
    {
        OAM_LOG(OAM, OAM_DETAILED,"parameters populated for SERVICES Successfully");
    }

    if(OAM_SUCCESS == oam_populate_management_server_param())
    {
        OAM_LOG(OAM, OAM_DETAILED,"parameters populated for MANAGEMENT SERVER Successfully");
    }
    /* Populate TR069 specifc parameters end */

#ifdef OAM_RADIO_HEAD_SUPPORT
    if(OAM_SUCCESS == oam_populate_rrh_init_param()) {
        OAM_LOG(OAM, OAM_DETAILED,"parameters populated for RRH");
    }
    else {
        OAM_LOG(OAM, OAM_ERROR,
                "population failure oam_populate_rrh_init_param");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
#endif
    if(OAM_SUCCESS == oam_populate_fault_mgmt_param())
    {
        OAM_LOG(OAM, OAM_DETAILED,"TR parameters populated for FAULT MANAGEMENT Successfully");
    }
/* SPR-13171 START */
/* code removed */
/* SPR-13171 END */

#ifdef OAM_SON_ENABLE 
    /* RT SPR 8981 FIX START */
    /* LINES DELETED */
    /* RT SPR 8981 FIX END */

    ret_val = oam_populate_son_init_param();
    if(OAM_SUCCESS == ret_val) {
        OAM_LOG(OAM, OAM_INFO, "Init parameters populated for SON");
    }
    else {
        OAM_LOG(OAM, OAM_ERROR,
                     "Failed to populate init parameters for SON");

        OAM_FUN_EXIT_TRACE();
        return ret_val;
    }


    if(OAM_SUCCESS == oam_populate_son_nw_scan_params())
    {
        OAM_LOG(OAM, OAM_DETAILED,"parameters populated for son_nw_scan");
    }
    else
    {
        OAM_LOG(OAM, OAM_ERROR,
                "Unable to populate parameter for son_nw_scan");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
    if(OAM_SUCCESS == oam_populate_son_nw_scan_cfg())
    {
        OAM_LOG(OAM, OAM_DETAILED,"parameters populated for son_nw_scan_cfg");
    }
    else
    {
        OAM_LOG(OAM, OAM_ERROR,
                "Unable to populate parameter for son_nw_scan_cfg");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
     /*SPR_16636 Fix Start */
    /* code moved */
    /*SPR_16636 Fix End */

    /* SPR 15604 START */  
    /* code moved */
    /* SPR 15604 END */  


           
                    
#endif
    /* RT SPR 9019 FIX END */

    if (OAM_SUCCESS == oam_populate_nms_ftp_server_params())
    {
        OAM_LOG(OAM, OAM_DETAILED,"parameters populated for nms_ftp_server_params");
    }
    else
    {
        OAM_LOG(OAM, OAM_ERROR,
                "oam aborted due to invalid range parameter for nms_ftp_server_params");
    }

    if (OAM_SUCCESS == oam_populate_configrable_core_num())
    {
        OAM_LOG(OAM, OAM_DETAILED,"parameters populated for oam_populate_configrable_core_num");
    }
    else
    {
        OAM_LOG(OAM, OAM_ERROR,
                "oam aborted due to invalid range parameter for oam_populate_configrable_core_num");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
#ifdef OAM_SHARED_MEM_LOGGER
    if(OAM_TRUE == oam_prov_req.oam_igd_params.oam_shared_mem_conf.is_shared_mem_conf_present)
    {
        if (OAM_SUCCESS == oam_populate_shared_memory_conf())
        {
            OAM_LOG(OAM, OAM_DETAILED,"parameters populated for oam_populate_shared_memory_conf");
        }
        else
        {
            OAM_LOG(OAM, OAM_ERROR,
                    "oam aborted due to invalid range parameter for oam_populate_shared_memory_conf");
            OAM_FUN_EXIT_TRACE();
            return OAM_FAILURE;
        }
    }
#endif
    /*SPR 21096 +-*/
    /* Populate TR069 specifc parameters start */
    if(OAM_SUCCESS == oam_populate_tr069_params())
    {
        OAM_LOG(OAM, OAM_DETAILED,"parameters populated for oam_populate_tr069_params");
    }
    else
    {
        OAM_LOG(OAM, OAM_ERROR,
                "population failure for oam_populate_tr069_params");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
    /*SPR 21096 +-*/
    if(OAM_SUCCESS == oam_populate_rrm_init_config_param())
    {
        OAM_LOG(OAM, OAM_DETAILED,"parameters populated for rrm_init_config");
    }
    else
    {
        OAM_LOG(OAM, OAM_ERROR,
                "Unable to populate parameter for oam_populate_rrm_init_config_param");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
#ifdef OAM_SON_ENABLE 
    if(OAM_SUCCESS == oam_populate_son_nr_enb_add_req())
    {
        OAM_LOG(OAM, OAM_DETAILED,"parameters populated for oam_populate_son_nr_enb_add_req");
    }
    /*eICIC_PHASE_1_2_CHANGES_START*/
    if(OAM_SUCCESS == oam_populate_anr_neighbor_default_config_params())
    {
        OAM_LOG(OAM, OAM_DETAILED,"parameters populated for oam_populate_anr_neighbor_default_config_params");
    }

    /*eICIC_PHASE_1_2_CHANGES_END*/
    if (OAM_SUCCESS == oam_populate_son_mro_params())
    {
        OAM_LOG(OAM, OAM_DETAILED,"parameters populated for oam_populate_son_mro_params");
    }

    if (OAM_SUCCESS == oam_populate_son_mlb_params())
    {
        OAM_LOG(OAM, OAM_DETAILED,"parameters populated for oam_populate_son_mlb_params");
    }

    if (OAM_SUCCESS == oam_populate_son_rach_opt_params())
    {
        OAM_LOG(OAM, OAM_DETAILED,"parameters populated for oam_populate_son_rach_opt_params");
    }
#endif

    /* function for populating the data for version info */  
    oam_fill_version_id_req();

    /* function for populating the data for health info */  
    oam_populate_health_mgmt_param(); 

    if (OAM_SUCCESS == oam_populate_support_for_log_archival())
    {
        OAM_LOG(OAM, OAM_DETAILED,"parameters populated for oam_populate_support_for_log_archival");
    }
    else
    {
        OAM_LOG(OAM, OAM_DETAILED,
                "oam aborted due to invalid range parameter for oam_populate_support_for_log_archival");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
    


    ret_val = oam_populate_device_specific_s1ap_params();
    if(OAM_SUCCESS == ret_val) {
        OAM_LOG(OAM, OAM_DETAILED,
                "Device specific parameters populated for l3_s1ap");
    }
    else {
        OAM_LOG(OAM, OAM_ERROR, "Unable to populate parameter S1AP");
        OAM_FUN_EXIT_TRACE();
        return ret_val;
    }

/* SPR-13171 START */
    if(OAM_SUCCESS == oam_populate_fap_control_param())
    {
        OAM_LOG(OAM, OAM_DETAILED,"parameters populated for FAPControl Successfully");
    }
    else {
        OAM_LOG(OAM, OAM_ERROR,
                "Unable to populate parameter for FAPControl");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
/* SPR-13171 END */
   /* SPR 21889 +- */ 
/* SPR 11001 FIX START */
    if (RRM_PROV_REQ.enable_load_config) {
        oam_populate_system_spec_load_config_params();
    }
    else {
        OAM_LOG(OAM, OAM_WARNING, "Load config is disabled");
    }
/* SPR 11001 FIX END */
   
    /*SPR 21096 EGTPU HACK In the Later version the external world IP for EGTPU wil remain one for all L2 instances*/ 
    if(OAM_SUCCESS == oam_populate_lan_device(&oam_prov_req.l2_setup_info.instance_info[0].oam_l2_req)) {
        OAM_LOG(OAM, OAM_DETAILED,"parameters populated for LAN device");
    }
    else {
        OAM_LOG(OAM, OAM_ERROR,
                "population failure oam_populate_lan_device");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
/* SPR 21212 Start +- */
    
   /*SPR 21096 Start*/
    for(cell_cnt = OAM_ZERO;
		    cell_cnt < oam_prov_req.num_cells_configured ; ++cell_cnt) {

        /* spr_21013_fix_start */
	    p_cell_spec_params =
		    &xml_struct_tr196_g.FAPService[cell_cnt].CellConfig;
        /* spr_21013_fix_end */
/* SPR 22248 MBMS Changes Start */
            p_cell_specFAPService = &xml_struct_tr196_g.FAPService[cell_cnt];
/* SPR 22248 MBMS Changes Stop */
	    /*SPR 21206 Start*/
			    Char8* tag_value =
				    (Char8*)oam_xmlNodeGetContent(p_cell_spec_params->X_VENDOR_ADDITIONAL_OPERATOR_INFO,
						    "AssocLayer2Inst");
			    if(oam_strcmp(tag_value, OAM_INVALID_STR)) {
				    lclTemp_value = oam_atoi(tag_value);
				    /* SPR 21889 Start */
				    if (lclTemp_value < OAM_MAX_NUM_L2_INSTANCE)
					    /* SPR 21889 End */
                                    {
                                          l2_instance = lclTemp_value; 
                                    }
                                    else
                                    {       
                                          OAM_LOG(OAM, OAM_ERROR, "AssocLayer2Inst info should be range 0 - %u", OAM_MAX_NUM_L2_INSTANCE);
                                    }
			    }
			    else
			    {
				    OAM_LOG(OAM, OAM_ERROR,
						    "AssocLayer2Inst info not present ");
			    }
	    /*SPR 21206 End*/


	    if(OAM_SUCCESS == oam_populate_l2_mac_log_level_param(&oam_prov_req.l2_setup_info.instance_info[l2_instance].oam_l2_req))
	    {
		    OAM_LOG(OAM, OAM_DETAILED,
				    "parameters populated for l2_mac_log_level");
	    }
	    /*SPR 21096 End*/


	    /* Proprietary Specific code */
	    /*SPR 21096 +-*/
	    /* SPR 11001 FIX START */
	    OAM_LOG(OAM, OAM_DETAILED, "Reading data for CELL[%d]", cell_cnt);
	    /* SPR 11001 FIX END */

	    p_rrc_cell_info = &RRC_PROV_REQ.sector_info_list.
		    sector_info[cell_cnt];

	    p_x2ap_cell_info = &X2AP_PROV_REQ.x2ap_info.srvd_cell.
		    srvd_cell_arr[cell_cnt];
	    /*SPR 21096 +-*/

	    p_prop_cellconfig = &xml_struct_g.CellConfig[cell_cnt];

	    p_rrm_cell_config =
		    &RRM_PROV_REQ.oam_rrm_cell_config_req[cell_cnt];

        /* EMBMS Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
        oam_mbsfn_cell_data_t *p_mbsfn_cell_data = &oam_prov_req.oam_l3_req.
                                                    oam_m2ap_req.
                                                    mbsfn_cell_data[cell_cnt];
        p_m2ap_cell_spec_params = &M2AP_CELL_SPEC_PARAMS(cell_cnt);
#endif
        /* EMBMS Changes End */

	    p_plmnlist_data = &RRM_PROV_REQ.plmnlist_data[cell_cnt];

	    /* SPR 15527 START */
	    p_qos_data = &RRM_PROV_REQ.qos_data[cell_cnt];
	    /* SPR 15527 END */                                                  
/* SPR 11001 FIX START */
	    p_srv_enb_cell_info =
		    &RRM_PROV_REQ.oam_load_config_req.serv_enb_cell_info[cell_cnt];

	    /* SPR 11001 FIX END */

	    /*SPR 21096 Start*/
	    p_l2_req = &oam_prov_req.l2_setup_info.instance_info[l2_instance].oam_l2_req;
	    /*SPR 21096 End*/

#ifdef OAM_SON_ENABLE
	    p_nr_info = &SON_NEIGHBORLIST_INFO(cell_cnt);

	    p_serving_cell = &SON_CELL_INFO_IND_SERVING_CELL(cell_cnt); 

	    p_son_inter_freq_carr_params = &SON_INTER_FREQ_CARR_PARAMS(cell_cnt);
	    p_es_config = &SON_PROV_REQ.oam_energy_saving_config[cell_cnt];
	    p_neigh_list_in_use  = &NR_LIST_IN_USE(cell_cnt);
	    /* SPR 13024 FIX START */
	    p_earfcn_pci_selection = &SON_PROV_REQ.earfcn_pci_selection[cell_cnt];
	    /* SPR 13024 FIX END */
	    p_pci_selection_req =
		    &(p_earfcn_pci_selection->oam_son_pci_selection_req);
	    p_pci_reselection_req =
		    &(p_earfcn_pci_selection->oam_son_pci_reselection_req);

	    /* SPR 13065 CHANGE START */ 
	    /* SPR 13065 CHANGE END */
	    /* SPR 13024 FIX START */
	    /* Code moved */
	    /* SPR 13024 FIX END */
	    p_earfcn_selection_req =
		    &(p_earfcn_pci_selection->oam_son_carrier_freq_and_dl_tx_power_req);
	    p_cell_carr_params =
		    &(oam_prov_req.oam_params.cell_carrier_params[cell_cnt]);
	    /* SPR-13074 End*/
	    /*SPR_18663 Fix Start*/
	    /*SPR_15770 Fix start*/
	    if(OAM_ZERO == cell_cnt)
	    {
		    oam_memset(&oam_prov_req.oam_igd_params.rem_lte_params.known_earfcn_list,OAM_ZERO,
				    sizeof(oam_prov_req.oam_igd_params.rem_lte_params.known_earfcn_list));
	    }
	    /*SPR_15770 Fix end*/
	    /*SPR_18663 Fix End*/

#endif


	    /* NOTE!!! 
	     * These following function:
	     * oam_populate_rrm_cell_config_param
	     * populates most cell specific parameters and many
	     * of the functions populate their parameters directly
	     * from the rrm_cell_config structure. So this function must be
	     * called first beforre any other population function
	     */
	    ret_val = oam_populate_rrm_cell_config_param(p_rrm_cell_config,
			    p_plmnlist_data,
			    p_cell_spec_params,
/* SPR 22248 MBMS Changes Start */
			    p_cell_specFAPService,
/* SPR 22248 MBMS Changes Stop */
			    /* BCOM Compiltaion Fix */
#ifdef OAM_SON_ENABLE
			    /* SPR 17730 Fix Start */
			    p_son_inter_freq_carr_params,
			    /* SPR 17730 Fix End */
#endif
                                                     /* BCOM Compiltaion Fix */
                                                     /* spr_21013_fix_start */
                                                     p_prop_cellconfig,
                                                     /* SPR 15527 START */
                                                     cell_cnt,
                                                     p_qos_data);
                                                      /* SPR 15527 END */                                                  
                                                     /* spr_21013_fix_end */
        if(OAM_SUCCESS == ret_val) {
		    OAM_LOG(OAM, OAM_DETAILED,
				    "Parameters populated for rrm_cell_config_param");
		    cid = oam_extract_cid_from_eutran_cellidentity(
				    (OWN_CELL_CGI(cell_cnt)).cell_identity);
/* SPR 21889 start */
		    oam_prov_req.l2_setup_info.instance_info[l2_instance].l2_cell_info.cell_info[oam_prov_req.l2_setup_info.instance_info[l2_instance].l2_cell_info.num_cells].cell_idx = cid;
		    oam_prov_req.l2_setup_info.instance_info[l2_instance].l2_cell_info.num_cells++;
		    OAM_LOG(OAM, OAM_INFO,
				    "Parameters populated for rrm_cell_config_param instance[%d] cell[%d] total_cell[%d]",l2_instance,cid,oam_prov_req.l2_setup_info.instance_info[l2_instance].l2_cell_info.num_cells);
/* SPR 21889 end */
            oam_prov_req.cell_cntxt.cell_ctxt_status[cell_cnt].cid = cid;
            oam_prov_req.cell_cntxt.cell_ctxt_status[cell_cnt].
                immediate_start_needed =
                             p_rrm_cell_config->immediate_start_needed;
            RRM_PROV_REQ.plmnlist_data[cell_cnt].cid = cid;
            /*SPR 17740 Start*/
            oam_prov_req.cell_cntxt.curr_cell_cid = cid;
	    /*SPR 17740 End*/
        }
        else {
            OAM_LOG(OAM, OAM_ERROR,
                    "Unable to populate parameter for rrm_cell_config_param");
            OAM_FUN_EXIT_TRACE();
            return ret_val;
        }

            /*SPR_15770 Fix start*/
#ifdef OAM_SON_ENABLE
	    if(OAM_SUCCESS == oam_populate_son_nr_scan_params())
	    {
		    OAM_LOG(OAM, OAM_DETAILED,"parameters populated for son_nr_scan");
	    }
	    else
	    {
		    OAM_LOG(OAM, OAM_ERROR,
				    "Unable to populate parameter for son_nr_scan");
		    oam_handle_invalid_startup_config();
	    }

	    if(OAM_SUCCESS == oam_populate_son_nw_scan_params())
	    {
		    OAM_LOG(OAM, OAM_DETAILED,"parameters populated for son_nw_scan");
	    }
	    else
	    {
		    OAM_LOG(OAM, OAM_ERROR,
				    "Unable to populate parameter for son_nw_scan");
		    oam_handle_invalid_startup_config();
	    }

	    /*SPR_16636 Fix Start */
	    if(OAM_SUCCESS == oam_populate_son_periodic_nw_scan_req())
	    {
		    OAM_LOG(OAM, OAM_DETAILED,"parameters populated for son_periodic_nw_scan_req");
	    }
	    else
	    {
		    OAM_LOG(OAM, OAM_ERROR,
				    "Unable to populate parameter for son_periodic_nw_scan_req");
		    oam_handle_invalid_startup_config();
	    }
	    /*SPR_16636 Fix End */

	    if((OAM_TRUE == oam_prov_req.system_status.is_nmm_enabled)
			    &&(OAM_TRUE == oam_prov_req.oam_son_req.temp_values.scan_on_boot))
	    {
		    /* if NMM is enabled Populate NW scan Results file */

		    if (OAM_SUCCESS == oam_populate_nw_scan_results())
		    {
			    OAM_LOG(OAM, OAM_DETAILED,"parameters populated for son_nw_scan_results");
		    }
		    else
		    {
			    OAM_LOG(OAM, OAM_ERROR,
					    "Unable to populate parameter for son_nw_scan_results");
			    /* no need to abort here oam_handle_invalid_startup_config(); */
		    }
	    }
#endif
	    /*SPR_15770 Fix end*/


        /* EMBMS Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
        /* SPR 18289 Fix Start */
/* SPR 22248 MBMS Changes Start */
        tag_value_embms = (Char8*)oam_xmlNodeGetContent(p_cell_specFAPService->LTE_3,
/* SPR 22248 MBMS Changes Stop */
                "X_VENDOR_CELL_TYPE");
        if(OAM_MACRO_ENB_TYPE == oam_atoi(tag_value_embms)) 
        {
            /* SPR 18289 Fix End */
        ret_val = oam_populate_rrm_cell_specific_embms_params(p_rrm_cell_config,
                                                         p_mbsfn_cell_data,
/* SPR 22248 MBMS Changes Start */
                                                         p_cell_spec_params,
                                                         p_cell_specFAPService);
/* SPR 22248 MBMS Changes Stop */
        if (OAM_SUCCESS == ret_val) 
        {
            OAM_LOG(OAM, OAM_INFO, "eMBMS parameters populated for RRM");
            p_mbsfn_cell_data->cid = cid;
            p_mbsfn_cell_data->config_stat_resp_flag = OAM_ONE;
        }
        else 
        {
            OAM_LOG(OAM, OAM_WARNING,
                    "Unable to populate eMBMS parameters for RRM."
                    "eMBMS FEATURE MAY NOT WORK !!!!");

            ret_val = OAM_SUCCESS;
        }

        ret_val = oam_populate_l3_mbms_cell_specific_params(p_cell_spec_params,
/* SPR 22248 MBMS Changes Start */
							    p_cell_specFAPService,
/* SPR 22248 MBMS Changes Stop */
                                                            p_rrm_cell_config,
                                                    p_m2ap_cell_spec_params);

        if(OAM_SUCCESS == ret_val) {
            OAM_LOG(OAM, OAM_INFO, "eMBMS cell specific parameters populated "
                                   "for L3 for cid: %d", cid);
            ++M2AP_PROV_REQ.num_cells;
        }
        else {
            OAM_LOG(OAM, OAM_WARNING,
                    "Unable to populate eMBMS cell specific parameters for L3."
                    "eMBMS FEATURE MAY NOT WORK !!!!");
/* PLMN CHANGES Start */
            OAM_FUN_EXIT_TRACE();
            return ret_val;
/* PLMN CHANGES Start*/
        }
            /* SPR 18289 Fix Start */
        }
        else
        {
            OAM_LOG(OAM, OAM_WARNING,
                    "eMBMS FEATURE IS DISABLED AS CELL_TYPE IS FEMTO !!!!");
        }
        /* SPR 18289 Fix End */

#endif
        /* EMBMS Changes End */

	    if(!cell_cnt) {
		    oam_prov_req.cell_cntxt.curr_cell_cid =
			    oam_prov_req.cell_cntxt.cell_ctxt_status[cell_cnt].cid;
	    }
	    /*SPR 21096 Start*/
	    ret_val = oam_populate_l2_cell_specific_params(p_cell_spec_params,
			    p_prop_cellconfig,
			    p_rrm_cell_config,
			    p_l2_req, cell_cnt,l2_instance);
        /*SPR 23209 FIXED START */
        /* SPR 23504/SES-341 Fix Start */
        if (OAM_SUCCESS == oam_populate_l2_mac_init_mocn_param(p_cell_spec_params, p_rrm_cell_config, p_l2_req, cell_cnt, l2_instance))
        /* SPR 23504/SES-341 Fix End */

        {
            OAM_LOG(OAM, OAM_DETAILED,"\nparameters populated for oam_populate_l2_mac_init_mocn_param");
        }
        else
        {
            OAM_LOG(OAM, OAM_DETAILED,
                    "\noam aborted due to invalid range parameter for oam_populate_l2_mac_init_mocn_param");
            /*MOCN SPR 21985 Fix Start*/
            oam_abort();
            /*MOCN SPR 21985 Fix End*/
        }

        /* SPR 23504/SES-341 Fix Start */
        if (OAM_SUCCESS == oam_populate_rrm_cell_config_mocn_param(p_rrm_cell_config, p_l2_req, cell_cnt, l2_instance))
            /* SPR 23504/SES-341 Fix End */

        {
            OAM_LOG(OAM, OAM_DETAILED,"\nparameters populated for oam_populate_rrm_cell_config_mocn_param");
        }
        else
        {
            OAM_LOG(OAM, OAM_DETAILED,
                    "\noam aborted due to invalid range parameter for oam_populate_rrm_cell_config_mocn_param");
            /*MOCN SPR 21985 Fix Start*/
            oam_abort();
            /*MOCN SPR 21985 Fix End*/
        }
        /*SPR 23209 FIXED END */
	    /*SPR 21096 End*/
	    if(OAM_SUCCESS == ret_val) {
		    OAM_LOG(OAM, OAM_INFO,
				    "Cell specific parameters populated for l2");
	    }
	    else {
		    OAM_LOG(OAM, OAM_ERROR,
				    "Cell specific parameter population for l2 failed");
		    OAM_FUN_EXIT_TRACE();
		    return ret_val;
	    }
	    /* SPR 11952 FIX START */
	    ret_val = oam_populate_device_specific_rrc_params(p_cell_spec_params);
	    if(OAM_SUCCESS == ret_val) {
		    OAM_LOG(OAM, OAM_DETAILED,
				    "Device specific parameters populated for RRC");
	    }
	    else {
		    OAM_LOG(OAM, OAM_ERROR,
				    "Unable to populate parameter for RRC");
		    OAM_FUN_EXIT_TRACE();
		    return ret_val;
	    }
	    /* SPR 11952 FIX END */

	    ret_val = oam_populate_cell_specific_rrc_params(p_cell_spec_params,
			    p_prop_cellconfig,
			    p_rrm_cell_config,
			    p_rrc_cell_info);
	    if(OAM_SUCCESS == ret_val) {
		    OAM_LOG(OAM, OAM_DETAILED,
				    "Cell specific parameters populated for RRC");
		    ++RRC_PROV_REQ.sector_info_list.count;
	    }
	    else {
		    OAM_LOG(OAM, OAM_ERROR,
				    "Unable to populate parameter for RRC");
		    OAM_FUN_EXIT_TRACE();
		    return ret_val;
	    }

	    ret_val =
		    oam_populate_cell_specific_x2ap_params(p_cell_spec_params,
				    p_rrc_cell_info,
				    p_rrm_cell_config,
				    p_x2ap_cell_info);
	    if(OAM_SUCCESS == ret_val) {
		    OAM_LOG(OAM, OAM_DETAILED, "Parameters populated for X2AP");
		    ++X2AP_PROV_REQ.x2ap_info.srvd_cell.num_srvd_cell;
	    }
	    else {
		    OAM_LOG(OAM, OAM_ERROR,
				    "Unable to populate parameter for X2AP");
		    OAM_FUN_EXIT_TRACE();
		    return ret_val;
	    }

	    ret_val =
		    oam_populate_dynamic_icic_param(p_rrm_cell_config,
				    p_cell_spec_params,
				    p_prop_cellconfig,
                    /* SPR 21958 PUSCH RAT1 Support Start */
                    cell_cnt
                    /* SPR 21958 PUSCH RAT1 Support End */
                    );
	    if(OAM_SUCCESS == ret_val) {
		    OAM_LOG(OAM, OAM_DETAILED,
				    "Parameters populated for DYNAMIC ICIC PARAM");
	    }
	    else {
		    OAM_LOG(OAM, OAM_ERROR, "DYNAMIC ICIC param population failed");
		    OAM_FUN_EXIT_TRACE();
		    return ret_val;
	    }
	    /* SPR 20653 Fix Start */
	    ret_val =
		    oam_populate_olpc_config_params(p_cell_spec_params,
				    p_rrm_cell_config);
	    if(OAM_SUCCESS == ret_val) {
		    OAM_LOG(OAM, OAM_DETAILED,
				    "Parameters populated for OLPC CONFIG");
	    }
	    else {
		    OAM_LOG(OAM, OAM_ERROR, "OLPC CONFIG param population failed");
		    OAM_FUN_EXIT_TRACE();
		    return ret_val;
	    }
	    /* SPR 20653 Fix End */

	    /* RT SPR 9019 FIX START */
	    /* LINES DELETED */
#ifdef ENDC_ENABLED
        if( xml_struct_g.EN_DC_Params)
        {
            ret_val = oam_populate_en_dc_config_params();
            if(OAM_SUCCESS == ret_val)
            {
                OAM_LOG(OAM, OAM_DETAILED, "Parameters populated for EN_DC");
            }
            else
            {
                OAM_LOG(OAM, OAM_ERROR,
                        "Unable to populate parameter for EN_DC");
                OAM_FUN_EXIT_TRACE();
                return ret_val;
            }
        }
#endif
	    /* RT SPR 9019 FIX END */

	    /*SPR 21212 Start*/
	    ret_val = oam_populate_egtpu_init_req(p_prop_cellconfig,&oam_prov_req.l2_setup_info.instance_info[OAM_ZERO].oam_l2_req);
	    /*SPR 21212 End*/
	    if(OAM_SUCCESS == ret_val) {
		    OAM_LOG(OAM, OAM_DETAILED, "Parameters populated for egtpu");
	    }
	    else {
		    OAM_LOG(OAM, OAM_ERROR,
				    "Unable to populate parameter for EGTPU");
		    OAM_FUN_EXIT_TRACE();
		    return ret_val;
	    }

	    ret_val =
		    oam_populate_rrm_inter_rat_ncl_utran(p_rrm_cell_config,
				    p_cell_spec_params);
	    if(OAM_SUCCESS == ret_val) {
		    OAM_LOG(OAM, OAM_DETAILED,
				    "UTRAN Neighbor info parameters populated for RRM");
	    }
	    else {
		    OAM_LOG(OAM, OAM_ERROR,
				    "UTRAN Neighbor info parameters populatinon for RRM failed");
		    OAM_FUN_EXIT_TRACE();
		    return ret_val;
	    }

	    /* SPR 11001 FIX START */
	    /*SPR 12228 FIX START*/
	    /*Redundant code removed for filling ecn capacity*/
	    /*SPR 12228 FIX END*/

	    /* function for populating the data for laod config */
	    if (RRM_PROV_REQ.enable_load_config) {
		    ret_val =
			    oam_populate_cell_spec_load_config_params(p_srv_enb_cell_info,
					    p_prop_cellconfig,
					    p_rrm_cell_config);

		    if (OAM_SUCCESS == ret_val) {
			    ++RRM_PROV_REQ.oam_load_config_req.num_enb_cells;
			    OAM_LOG(OAM, OAM_DETAILED, "Cell specific parameters for cell: %d"
					    " populated for load configuration",
					    cell_cnt);
		    }
		    else {
			    OAM_LOG(OAM, OAM_ERROR, "Failed to populate cell specific"
					    " parameters for load configuration for cell: %d ",
					    cell_cnt);
			    OAM_FUN_EXIT_TRACE();
			    return ret_val;
		    }
	    }
	    /* SPR 11001 FIX END */

#ifdef OAM_SON_ENABLE

	    /*SPR 15604 START */
	    if(OAM_SUCCESS == oam_populate_rrm_inter_rat_ncl_utran_nliu(p_rrm_cell_config,p_cell_spec_params)) {
		    OAM_LOG(OAM, OAM_DETAILED,
				    "UTRAN Neighbor info parameters populated for RRM");
	    }
	    else {
		    OAM_LOG(OAM, OAM_ERROR,
				    "UTRAN Neighbor info parameters populatinon for RRM failed");
		    oam_handle_invalid_startup_config();
	    }
	    /*SPR 15604 END */



	    ret_val = oam_populate_son_anr_meas_params(p_cell_spec_params,
			    p_son_inter_freq_carr_params,
			    p_rrm_cell_config);
	    if(OAM_SUCCESS == ret_val) {
		    OAM_LOG(OAM, OAM_DETAILED,
				    "Parameters populated for SON carrier frequency measurement");
		    p_son_inter_freq_carr_params->cid = cid;
	    }
	    else {
		    OAM_LOG(OAM, OAM_ERROR, "Unable to populate SON carrier "
				    "frequency measurement params");
		    OAM_FUN_EXIT_TRACE();
		    return ret_val;
            }
            /*SPR 21702 Start*/
            oam_prov_req.oam_son_req.oam_son_cell_info_ind[cell_cnt].cell_info_size = OAM_ONE; 
            /*SPR 21702 End*/

            ret_val = oam_populate_son_cell_info_ind(p_cell_spec_params,
			    p_serving_cell,
			    p_rrm_cell_config);
	    if(OAM_SUCCESS == ret_val) {
		    OAM_LOG(OAM, OAM_DETAILED,
				    "Parameters populated for SON_CELL_INFO_IND");
	    }
	    else {
		    OAM_LOG(OAM, OAM_ERROR,
				    "Unable to populate SON_CELL_INFO_IND params");
		    OAM_FUN_EXIT_TRACE();
		    return ret_val;
	    }
	    /* SPR 13065 CHANGE START */
	    /* SPR 13065 CHANGE END */        
	    /*+ SPR 18394*/    
	    {
		    /*- SPR 18394*/
		    ret_val = oam_populate_son_career_freq_and_dl_tx_power_params(
				    p_cell_spec_params,
				    p_earfcn_selection_req,
				    p_cell_carr_params);
		    if (OAM_SUCCESS == ret_val) {
			    OAM_LOG(OAM, OAM_DETAILED, "Parameters populated for "
					    "career_freq_and_dl_tx_power_params");
		    }
		    else {
			    OAM_LOG(OAM, OAM_ERROR,
					    "Unable to populate career_freq_and_dl_tx_power_params");

			    OAM_FUN_EXIT_TRACE();
			    return ret_val;
		    }
	    }
	    /* SPR 13060 FIX START */

	    ret_val = oam_populate_son_pci_selection_params(
			    p_cell_spec_params,
			    p_pci_selection_req,
			    /* SPR 15267 START */
			    p_pci_reselection_req,
                                               /* SPR 17753 Fix Start */ 
			                        p_nr_info,
                                    p_serving_cell);
                                    /* SPR 17753 Fix End */
	    /* SPR 15267 END */
	    if(OAM_SUCCESS == ret_val) {
		    OAM_LOG(OAM, OAM_DETAILED,
				    "Parameters populated for son pci selection");
	    }
	    else {
		    OAM_LOG(OAM, OAM_ERROR,
				    "Unable to populate SON PCI reselection params");
		    OAM_FUN_EXIT_TRACE();
		    return ret_val;
	    }
	    /* SPR 13060 FIX END */

	    /* SPR 13024 FIX START */
	    p_earfcn_pci_selection->cid = cid;
	    /* SPR 13024 FIX END */
/*SPR 22442 Fix Start*/
	    ret_val =
		    oam_populate_rrm_geran_freq_cells_cell_based(p_rrm_cell_config,
				    p_cell_spec_params);
	    if(OAM_SUCCESS == ret_val) {
		    OAM_LOG(OAM, OAM_DETAILED,
				    "Parameters populated for oam_populate_rrm_geran_freq_cells");
	    }
	    else {
		    OAM_LOG(OAM, OAM_ERROR,
				    "GSM Neighbor info population for RRM failed");
		    OAM_FUN_EXIT_TRACE();
		    return ret_val;
	    }
	    ret_val = oam_populate_rrm_cdma2000_cell_based(p_rrm_cell_config,
			    p_cell_spec_params);
	    if(OAM_SUCCESS == ret_val) {
		    OAM_LOG(OAM, OAM_DETAILED,
				    "Parameters populated for oam_populate_rrm_cdma2000");
	    }
	    else {
		    OAM_LOG(OAM, OAM_ERROR,
				    "CDMA2000 Neighbor info population for RRM failed");
		    OAM_FUN_EXIT_TRACE();
		    return ret_val;
	    }
/*SPR 22442 Fix End*/
	    ret_val = oam_populate_son_nr_add_req(p_cell_spec_params,
			    p_rrm_cell_config,
			    p_nr_info);
	    if(OAM_SUCCESS == ret_val) {
		    OAM_LOG(OAM, OAM_DETAILED,
				    "Neighbor info parameters populated for SON");

		    p_nr_info->cid = cid;
	    }
	    else {
		    OAM_LOG(OAM, OAM_ERROR,
				    "Neighbor info parameters population for SON failed");
		    OAM_FUN_EXIT_TRACE();
		    return ret_val;
	    }

	    ret_val = oam_populate_son_neigh_list_in_use(p_cell_spec_params,p_neigh_list_in_use,p_rrm_cell_config);
	    if(OAM_SUCCESS == ret_val) {
		    OAM_LOG(OAM, OAM_DETAILED,
				    "Parameters populated for NeighborListInUse");
	    }
	    else {
		    OAM_LOG(OAM, OAM_ERROR, "OAM aborted due to invalid range "
				    "parameter for NeighborListInUse");
		    OAM_FUN_EXIT_TRACE();
		    return ret_val;
	    }
#endif
	    ret_val = oam_populate_lte_tunnel_params(p_cell_spec_params);
	    if(OAM_SUCCESS == ret_val) {
		    OAM_LOG(OAM, OAM_DETAILED,
				    "Parameters populated for LTE.Tunnel Successfully");
	    }
	    else {
		    OAM_LOG(OAM, OAM_ERROR,
				    "Parameters population failed for LTE.Tunnel");
		    OAM_FUN_EXIT_TRACE();
		    return ret_val;
	    }

	    ret_val = oam_populate_tr069_tr196_params(p_cell_spec_params);
	    if(OAM_SUCCESS == ret_val) {
		    OAM_LOG(OAM, OAM_DETAILED,"Parameters populated for TR069");
	    }
	    else {
		    OAM_LOG(OAM, OAM_ERROR, "Population failure for TR069");
		    OAM_FUN_EXIT_TRACE();
		    return ret_val;
	    }


	    /* The population of L3 S1AP parameters depends on value of the
	       parameter NNSFSupported which is populated in function
	       oam_populate_tr069_tr196_params, therefore the following function
	       must be called after oam_populate_tr069_tr196_params
	       */
            ret_val = oam_populate_cell_specific_s1ap_params(p_cell_spec_params,
                p_rrm_cell_config
                );
	    if(OAM_SUCCESS == ret_val) {
		    OAM_LOG(OAM, OAM_DETAILED,
				    "Cell specific parameters populated for l3_s1ap");
	    }
	    else {
		    OAM_LOG(OAM, OAM_ERROR, "Unable to populate parameter S1AP");
		    OAM_FUN_EXIT_TRACE();
		    return ret_val;
	    }
/* LWA changes start*/
/* LWA changes end*/

        /* Spr 10784 Fix Start */
	    ret_val = oam_populate_device_specific_x2ap_params();
	    if(OAM_SUCCESS == ret_val) {
		    OAM_LOG(OAM, OAM_DETAILED,
				    "Device specific parameters populated for X2AP");
	    }
	    else {
		    OAM_LOG(OAM, OAM_ERROR,
				    "Unable to populate parameter for X2AP");
		    OAM_FUN_EXIT_TRACE();
		    return ret_val;
	    }
	    /* Spr 10784 Fix End*/

#ifdef LTE_EMBMS_SUPPORTED
        /* SPR 18289 Fix Start */
/* SPR 22248 MBMS Changes Start */
        tag_value_embms = (Char8*)oam_xmlNodeGetContent(p_cell_specFAPService->LTE_3,
/* SPR 22248 MBMS Changes Stop */
                "X_VENDOR_CELL_TYPE");
        if(OAM_MACRO_ENB_TYPE == oam_atoi(tag_value_embms)) 
        {
            /* SPR 18289 Fix End */
        ret_val = oam_populate_system_specific_mbms_params();
        if(OAM_SUCCESS == ret_val) {
            OAM_LOG(OAM, OAM_INFO,
                    "Successfully populated system specific eMBMS parameters");
        }
        else {
            OAM_LOG(OAM, OAM_WARNING,
                        "Failed to populate system specific eMBMS parameters"
                        "eMBMS FEATURE MAY NOT WORK !!!!");

            ret_val = OAM_SUCCESS;
        }
            /* SPR 18289 Fix Start */
        }
        else
        {
            OAM_LOG(OAM, OAM_WARNING,
                    "eMBMS FEATURE IS DISABLED AS CELL_TYPE IS FEMTO !!!!");
        }
        /* SPR 18289 Fix End */
#endif

/*SPR 22442 Fix Start*/
	/*Code moved above*/
/*SPR 22442 Fix End*/

	    ret_val =
		    oam_populate_rrm_meas_gap_config_cell_based(p_rrm_cell_config, 
				    p_cell_spec_params);
	    if(OAM_SUCCESS == ret_val) {
		    OAM_LOG(OAM, OAM_DETAILED,
				    "Parameters populated for rrm_meas_gap_config");
	    }
	    else {
		    OAM_LOG(OAM, OAM_ERROR,
				    "rrm_meas_gap_config populatinon for RRM failed");
		    OAM_FUN_EXIT_TRACE();
		    return ret_val;
	    }

	    /* Spr 11954 Fix Start
	       Removed Redundant Code
	       Spr 11954 Fix End*/	

#ifdef OAM_SON_ENABLE
	    ret_val = oam_populate_son_es_params(p_es_config);
	    if(OAM_SUCCESS == ret_val) {
		    OAM_LOG(OAM, OAM_DETAILED, "Parameters populated for Energy Saving");
	    }
	    else {
		    OAM_LOG(OAM, OAM_ERROR,
				    "Energy Saving parameter population failed");
		    OAM_FUN_EXIT_TRACE();
		    return ret_val;
	    }
#endif
	    /*SPR 21096 Start*/
	    ret_val = oam_populate_l2_layer_specific_params(p_cell_spec_params,
			    p_prop_cellconfig,
			    p_l2_req,
			    /*SPR 21321 Start*/
			    p_rrm_cell_config,cell_cnt,
			        l2_instance
                );
	    /*SPR 21321 End*/
	    /*SPR 21096 End*/
    }
     
    /*LTE_OAM_BUG_13082_FIXED_START */ 
    if(OAM_SUCCESS == oam_populate_s1ap_enb_cfg_update_params())
    {
        OAM_LOG(OAM, OAM_DETAILED,"parameters populated for S1AP_OAM_ENB_CONFIG_UPDATE_REQ");
    }
    /*LTE_OAM_BUG_13082_FIXED_END */ 

    /*SPR 21096 +-*/

    if(OAM_SUCCESS == ret_val) {
        OAM_LOG(OAM, OAM_DETAILED,
                 "Layer specific arameters populated for l2");
    }
    else {
        OAM_LOG(OAM, OAM_ERROR,
            "Layer specific parameter populatinon for L2 failed");
        OAM_FUN_EXIT_TRACE();
        return ret_val;
    }

    ret_val = oam_populate_rrm_enb_config_params();
    if(OAM_SUCCESS == ret_val) {
        OAM_LOG(OAM, OAM_DETAILED,
                 "Parameters populated for API RRM_OAM_ENB_CONFIG_REQ");
    }
    else {
        OAM_LOG(OAM, OAM_ERROR,
            "Parameters population for API RRM_OAM_ENB_CONFIG_REQ failed");
        OAM_FUN_EXIT_TRACE();
        return ret_val;
    }

    /* Some of the AccessMgmt.LTE parameters are exeception as its direct
     * mapping depends upon the number of cells actually configured i.e.
     * single cell, or multi-cell configuration. There are two different
     * mapping of the parameter, therefore.
     * The default values of different fields of the parameter table
     * g_parameter_TR_element for these parameters are initialized 
     * according to single cell configuration. However for multi-cell
     * configuration, we need to reassign the field values as given below
     */
    /* SPR-13976 STAT */
    if (oam_prov_req.num_cells_configured != OAM_ONE)
    {    
        tbl_size = oam_find_global_table_size(g_parameter_TR_element);
        for (i = OAM_ZERO; i < tbl_size; ++i) {
            /* spr_21013_fix_start */
            /* As a part of SPR 21013 changes, InternetGatewayDevice.Services.FAPService.[1].
             * has been changed to InternetGatewayDevice.Services.FAPService. hierarchy.
             * X_VENDOR_MAX_NON_CSG_MEMBERS, X_VENDOR_MAX_CSG_MEMBERS, X_VENDOR_MAX_UES_SERVED,
             * X_VENDOR_CSGID, and X_VENDOR_ACCESSMODE elements will now be part of AccessMgmt 
             * TAG instead of CellConfig TAG */
            if(!oam_strcmp(g_parameter_TR_element[i].parameter_name,
                        "InternetGatewayDevice.Services.FAPService."
                        "AccessMgmt.LTE.MaxNonCSGMembers")) {
                g_parameter_TR_element[i].validate =
                    oam_validate_cell_specific_max_non_csg_members;

                g_parameter_TR_element[i].xml_parameter_parent =
                /* spr_21013_fix_start */
                    &xml_struct_tr196_g.FAPService[OAM_ZERO].LTE_3;
                /* spr_21013_fix_end */

                g_parameter_TR_element[i].tag_name = 
                  "MaxResourceNonCSGMembers";

                g_parameter_TR_element[i].
                    parameter_mode |= OAM_IGNORE_VALIDATION;
                ++max_count;
            }
            else if (!oam_strcmp(g_parameter_TR_element[i].parameter_name,
                        "InternetGatewayDevice.Services.FAPService."
                        "AccessMgmt.LTE.MaxCSGMembers")) {
                g_parameter_TR_element[i].validate =
                    oam_validate_cell_specific_max_csg_members;

                g_parameter_TR_element[i].xml_parameter_parent =
                /* spr_21013_fix_start */
                    &xml_struct_tr196_g.FAPService[OAM_ZERO].LTE_3;
                /* spr_21013_fix_end */

                g_parameter_TR_element[i].tag_name = 
                  "MaxCSGMembers";

                g_parameter_TR_element[i].
                    parameter_mode |= OAM_IGNORE_VALIDATION;
                ++max_count;
            }
            else if (!oam_strcmp(g_parameter_TR_element[i].parameter_name,
                        "InternetGatewayDevice.Services.FAPService."
                        "AccessMgmt.LTE.CSGID")) {
                g_parameter_TR_element[i].validate =
                    oam_validate_cell_specific_csgid;

                g_parameter_TR_element[i].xml_parameter_parent =
                /* spr_21013_fix_start */
                    &xml_struct_tr196_g.FAPService[OAM_ZERO].LTE_3;
                /* spr_21013_fix_end */

                g_parameter_TR_element[i].tag_name = 
                  "CSGID";

                g_parameter_TR_element[i].
                    parameter_mode |= OAM_IGNORE_VALIDATION;
                ++max_count;
            }
            else if (!oam_strcmp(g_parameter_TR_element[i].parameter_name,
                        "InternetGatewayDevice.Services.FAPService."
                        "AccessMgmt.LTE.MaxUEsServed")) {
                g_parameter_TR_element[i].validate =
                    oam_validate_cell_specific_max_ues_served;

                g_parameter_TR_element[i].xml_parameter_parent =
                /* spr_21013_fix_start */
                    &xml_struct_tr196_g.FAPService[OAM_ZERO].LTE_3;
                /* spr_21013_fix_end */

                g_parameter_TR_element[i].tag_name = 
                  "MaxUEsServed";

                g_parameter_TR_element[i].
                    parameter_mode |= OAM_IGNORE_VALIDATION;
                ++max_count;
            }
            else if (!oam_strcmp(g_parameter_TR_element[i].parameter_name,
                        "InternetGatewayDevice.Services.FAPService."
                        "AccessMgmt.LTE.AccessMode")) {
                g_parameter_TR_element[i].validate =
                    oam_validate_cell_specific_access_mode;

                g_parameter_TR_element[i].xml_parameter_parent =
                /* spr_21013_fix_start */
                    &xml_struct_tr196_g.FAPService[OAM_ZERO].LTE_3;
                /* spr_21013_fix_end */

                g_parameter_TR_element[i].tag_name = 
                  "AccessMode";

                g_parameter_TR_element[i].
                    parameter_mode |= OAM_IGNORE_VALIDATION;
                ++max_count;
            }
            /* spr_21013_fix_end */

            if (max_count == OAM_MAX_CELL_SPEC_ACC_MGMT_PARAMS) {
                break;
            }
        }
    }
    /* SPR-13976 End */

    return ret_val;
}


/****************************************************************************
* Function Name  : oam_get_xml_leaf_child_node
* Description    : This function is used to calculate the pointer of a leaf
*                  child node
* Inputs         : p_parent : Pointer to parent of xmlNode in question

                   p_str : Pointer to node name
* Outputs        : NONE
* Returns        : pointer to the desired node, or OAM_NULL in case it fails
****************************************************************************/
xmlNode* oam_get_xml_leaf_child_node(xmlNode * p_parent, Char8* p_name)
{
    /* COV CID: 64254 FIX START */
    xmlNode * p_cur_node = OAM_NULL;
    xmlNode * p_result_node = OAM_NULL;

    if(!p_parent || !p_name) {
        return p_result_node;
    }

    p_cur_node = p_parent->children;
    /* COV CID: 64255 FIX END */

    for ( ; p_cur_node; p_cur_node = p_cur_node->next) {
        if (p_cur_node->children &&
            XML_TEXT_NODE == p_cur_node->children->type) {
            /*compare if the node is the desired one */
            if(!oam_strcmp((Char8*)p_cur_node->name, p_name)) {
                p_result_node = p_cur_node;
                break;
            }
            else {
                p_result_node =
                    oam_get_xml_leaf_child_node(p_cur_node, p_name);
            }
        }
    }

    return p_result_node;
}


/****************************************************************************
* Function Name  : oam_get_xml_nonleaf_child_node
* Description    : This function is used to calculate the pointer of a nonleaf
*                  child node
* Inputs         : p_parent : Pointer to parent of xmlNode in question

                   p_str : Pointer to node name
* Outputs        : NONE
* Returns        : pointer to the desired node, or OAM_NULL in case it fails
****************************************************************************/
xmlNode* oam_get_xml_nonleaf_child_node(xmlNode * p_parent, Char8* p_name)
{
    /* COV CID: 64255 FIX START */
    xmlNode * p_cur_node = OAM_NULL;
    xmlNode * p_result_node = OAM_NULL;

    if(!p_parent || !p_name) {
        return p_result_node;
    }

    p_cur_node = p_parent->children;
    /* COV CID: 64255 FIX END */

    for ( ; p_cur_node; p_cur_node = p_cur_node->next) {
        if (XML_ELEMENT_NODE == p_cur_node->type &&
            p_cur_node->children &&
            XML_TEXT_NODE != p_cur_node->children->type) {
            /*compare if the node is the desired one */
            if(!oam_strcmp((Char8*)p_cur_node->name, p_name)) {
                p_result_node = p_cur_node;
                break;
            }
            else {
                p_result_node =
                    oam_get_xml_nonleaf_child_node(p_cur_node, p_name);
            }
        }
    }

    return p_result_node;
}

/****************************************************************************
* Function Name  : oam_get_xml_text_node_content
* Description    : This function is used to return the content of a leaf node
* Inputs         : p_parent : Pointer to xmlNode type of the parent node
                 : tag_name :Pointer to tag name of the leaf node
* Outputs        : none
* Returns        : Pointer to xmlChar
*******************************************************************************/
xmlChar *
oam_get_xml_text_node_content(xmlNode *p_parent,
                              Char8 *tag_name,
                              Char8 *out_buf,
                              UInt16 buf_size)
{
    xmlChar * p_content = OAM_NULL;
    xmlNode *p_tag = OAM_NULL;

    if (!out_buf || !tag_name || !p_parent) {
        return p_content;
    }

    oam_memset(out_buf, OAM_ZERO, buf_size);

    p_tag = oam_get_xml_leaf_child_node(p_parent, tag_name);

    if (p_tag) {
        p_content = xmlNodeGetContent(p_tag);

        if(p_content) {
            oam_strncpy(out_buf, (Char8*)p_content, buf_size);
            xmlFree(p_content);
            p_content = (xmlChar*)out_buf;
        }
    }
	
    return 	p_content;
}


/****************************************************************************
* Function Name  : oam_get_xml_cellconfig_node
* Description    : This function is used to return the pointer to CellConfig
*                  xml node, given a particular cid
* Inputs         : cid of the cell identifying CellConfig node in xml file

* Outputs        : none
* Returns        : Pointer to xmlNode
*******************************************************************************/
XML_struct_cell_config* oam_get_xml_cellconfig_node(UInt8 cid)
{
    UInt8 curr_cid = OAM_ZERO;
    XML_struct_cell_config   *p_xml_node_cellconfig = OAM_NULL;
            /*SPR 21240 +-*/

	/* SPR 14016 START */
	if(RRM_ENB_TYPE_HOME == RRM_PROV_REQ.oam_rrm_enb_config_req.enb_id.enb_type)
	{                   
        /* spr_21013_fix_start */
        p_xml_node_cellconfig = &xml_struct_tr196_g.FAPService[OAM_ZERO].CellConfig;        
        /* spr_21013_fix_end */
		return p_xml_node_cellconfig;
	}
	else
	{   
        curr_cid= oam_extract_cid_from_eutran_cellidentity(SON_CELL_INFO_IND_SERVING_CELL(cid).src_cgi.cell_identity);
        p_xml_node_cellconfig = &xml_struct_tr196_g.FAPService[curr_cid].CellConfig;
        if (curr_cid == cid) {
            /*SPR 21240 Start*/
            return p_xml_node_cellconfig;
        }
        return OAM_NULL;
	}
        /*SPR 21240 End */
	/* SPR 14016 END */
}

void oam_xml_copy_doc_pointer(xmlNode *to, xmlNode *from)
{
    xmlNode *to_child = OAM_NULL;
    xmlNode *from_child = OAM_NULL;

    to->doc = from->doc;
    to_child = to->children;
    from_child = from->children;
    
    while (to_child && from_child) {
        oam_xml_copy_doc_pointer(to_child, from_child);
        to_child = to_child->next;
        from_child = from_child->next;
    }
}


/****************************************************************************
 * Function Name  : oam_xmlCopyNode
 * Inputs         : pointer to xmlNode to be duplicated
 *		            copy option: 1: recursive copy, 2: if only properties 
 *		            should be copied
 * Outputs        : none
 * Returns        : pointer to new xmlNode
 * Description    : This function makes another copy of the input xmlNode
 ***************************************************************************/
xmlNode* oam_xmlCopyNode(xmlNode *node, SInt32 opt)
{
    xmlNode *new = OAM_NULL;
    new = xmlCopyNode(node, opt);

    oam_xml_copy_doc_pointer(new, node);

    return new;
}

/* SPR 21889 start */
/*******************************************************************************
 *  FUNCTION NAME    : oam_get_agent_xml_info
 *  DESCRIPTION      : This function populate xmlNode of agent xml tag
 *  PARAMETERS
 *      IN           : Pointers to p_agent_info
 *
 *      OUT          : Pointers to p_agent_xml_info, p_max_process_cnt
 *
 *  RETURNS          : OAM_SUCCESS on successful processing and OAM_FAILURE in
 *                     case of error 
 *******************************************************************************/
oam_return_et
oam_get_agent_xml_info(xmlNode *p_agent_info,
        agent_info_xml_st *p_agent_xml_info,
        UInt8   *p_max_process_cnt)
{
    Char8   *p_tag_value        = OAM_NULL;
    xmlNode *p_instance_info    = OAM_NULL;
    xmlNode *p_process_info    = OAM_NULL;
    UInt8   max_process_cnt     = OAM_ZERO;
    UInt8   i                   = OAM_ZERO;
    UInt8   j                   = OAM_ZERO;
    oam_error_code_et err       = NO_ERROR;

    OAM_FUN_ENTRY_TRACE();

    p_tag_value =
        (Char8*)oam_xmlNodeGetContent(p_agent_info, "num_of_instance");

	if(!oam_strcmp(p_tag_value, OAM_INVALID_STR)) {
        OAM_LOG(OAM, OAM_ERROR, "Failed to read node num_of_instance");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    p_agent_xml_info->num_of_instance = oam_atoi(p_tag_value);

    /* This memory to be released by parent */
    if (OAM_SUCCESS != oam_mem_alloc(sizeof(agent_inst_info_xml_st) *
                p_agent_xml_info->num_of_instance,
                (void*)&p_agent_xml_info->p_inst_xml_info, &err))
    {
        OAM_LOG(OAM, OAM_ERROR, "Failed to allocate memory for instance table");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    flag_found = OAM_FALSE;
    oam_xml_find_ptr(p_agent_info, &p_instance_info, "instance_info");
    if(!flag_found) {
        oam_mem_free(p_agent_xml_info->p_inst_xml_info, &err);
        OAM_LOG(OAM, OAM_ERROR, "Failed to find instance_info node");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    while ((OAM_NULL != p_instance_info) &&
            (i < p_agent_xml_info->num_of_instance))
    {
        if (XML_ELEMENT_NODE != p_instance_info->type)
        {
            p_instance_info = p_instance_info->next;
            continue;
        }
        p_tag_value =
            (Char8*)oam_xmlNodeGetContent(p_instance_info, "instance_id");

        if(!oam_strcmp(p_tag_value, OAM_INVALID_STR)) {
            OAM_LOG(OAM, OAM_ERROR, "Failed to read node num_of_instance");
            oam_mem_free(p_agent_xml_info->p_inst_xml_info, &err);
            OAM_FUN_EXIT_TRACE();
            return OAM_FAILURE;
        }

        p_agent_xml_info->p_inst_xml_info[i].instance_id = oam_atoi(p_tag_value);

        p_tag_value =
            (Char8*)oam_xmlNodeGetContent(p_instance_info, "num_of_process");

        if(!oam_strcmp(p_tag_value, OAM_INVALID_STR)) {
            OAM_LOG(OAM, OAM_ERROR, "Failed to read node instance_id in agent_info_list");
            oam_mem_free(p_agent_xml_info->p_inst_xml_info, &err);
            OAM_FUN_EXIT_TRACE();
            return OAM_FAILURE;
        }

        p_agent_xml_info->p_inst_xml_info[i].num_process = oam_atoi(p_tag_value);
        max_process_cnt += p_agent_xml_info->p_inst_xml_info[i].num_process;

        /* This memory to be released by parent */
        if (OAM_SUCCESS != oam_mem_alloc(sizeof(xmlNode*) *
                    p_agent_xml_info->p_inst_xml_info[i].num_process,
                    (void*)&p_agent_xml_info->p_inst_xml_info[i].process_info,
                    &err))
        {
            OAM_LOG(OAM, OAM_ERROR, "Failed to allocate memory for process table");
            oam_mem_free(p_agent_xml_info->p_inst_xml_info, &err);
            OAM_FUN_EXIT_TRACE();
            return OAM_FAILURE;
        }

        flag_found = OAM_FALSE;
        oam_xml_find_ptr(p_instance_info, &p_process_info, "process_info");

        if(!flag_found) {
            OAM_LOG(OAM, OAM_ERROR, "Failed to find process_info node for instance %u ",
                    p_agent_xml_info->p_inst_xml_info[i].instance_id);
            oam_mem_free(p_agent_xml_info->p_inst_xml_info[i].process_info, &err);
            oam_mem_free(p_agent_xml_info->p_inst_xml_info, &err);
            OAM_FUN_EXIT_TRACE();
            return OAM_FAILURE;
        }

        j = OAM_ZERO;

        while ((OAM_NULL != p_process_info) &&
                (j < p_agent_xml_info->p_inst_xml_info[i].num_process))
        {
            if (XML_ELEMENT_NODE != p_process_info->type)
            {
                p_process_info = p_process_info->next;
                continue;
            }
            p_agent_xml_info->p_inst_xml_info[i].process_info[j] = p_process_info;
            j++;
            p_process_info = p_process_info->next;
        }
        i++;
        p_instance_info = p_instance_info->next;
    }

    *p_max_process_cnt = max_process_cnt;

    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}

/*******************************************************************************
 *  FUNCTION NAME    : oam_populate_process_tbl
 *  DESCRIPTION      : This function populate process table which will be used
 *                     for spawning process on master and agent
 *  PARAMETERS
 *      IN           : Pointers to p_process_tbl_node
 *
 *      OUT          : None
 *
 *  RETURNS          : OAM_SUCCESS on successful processing and OAM_FAILURE in
 *                     case of error 
 *******************************************************************************/
oam_return_et oam_populate_process_tbl(xmlNode *p_process_tbl_node)
{
    xmlNode *p_master_list  = OAM_NULL;
    xmlNode *p_process_node = OAM_NULL;
    xmlNode *p_agent_info   = OAM_NULL;
    /*SPR 22121 Start */
    Char8   process_name[OAM_MAX_STR_LEN] = "\0";
    Char8   process_args[OAM_MAX_STR_LEN] = "\0";
    /*SPR 22121 End */
    Char8   *p_tag_value    = OAM_NULL;
    Char8   *p_args         = OAM_NULL;
    agent_info_xml_st agent_xml_info;
    UInt8   max_process_on_master       = OAM_ZERO;
    UInt8   max_process_on_all_agent    = OAM_ZERO;
    UInt8   i    = OAM_ZERO;
    UInt8   k    = OAM_ZERO;
    UInt8   m    = OAM_ZERO;
    oam_error_code_et err   = NO_ERROR;

    OAM_FUN_ENTRY_TRACE();

    p_tag_value =
        (Char8*)oam_xmlNodeGetContent(p_process_tbl_node, "username");

	if(!oam_strcmp(p_tag_value, OAM_INVALID_STR)) {
        OAM_LOG(OAM, OAM_ERROR, "Failed to read tag username");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
    oam_strncpy(g_oam_sim_t.user_name, p_tag_value, OAM_MAX_USERNAME);
    g_oam_sim_t.user_name[OAM_MAX_USERNAME - OAM_ONE] = '\0';

    p_tag_value =
        (Char8*)oam_xmlNodeGetContent(p_process_tbl_node, "password");

	if(!oam_strcmp(p_tag_value, OAM_INVALID_STR)) {
        OAM_LOG(OAM, OAM_ERROR, "Failed to read tag password");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
    oam_strncpy(g_oam_sim_t.password, p_tag_value, OAM_MAX_PASSWORD);
    g_oam_sim_t.password[OAM_MAX_PASSWORD - OAM_ONE] = '\0';

    flag_found = OAM_FALSE;
    oam_xml_find_ptr(p_process_tbl_node->children, &p_master_list, "master_list");

    if(!flag_found) {
        OAM_LOG(OAM, OAM_ERROR, "Failed to find master_list tag");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    p_tag_value =
        (Char8*)oam_xmlNodeGetContent(p_master_list, "num_of_process");

	if(!oam_strcmp(p_tag_value, OAM_INVALID_STR)) {
        OAM_LOG(OAM, OAM_ERROR, "Failed to read tag num_of_process");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }
    max_process_on_master = oam_atoi(p_tag_value);

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_process_tbl_node, &p_agent_info, "agent_info_list");

    if(!flag_found) {
        OAM_LOG(OAM, OAM_ERROR, "Failed to find agent_info_list tag");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    if (OAM_SUCCESS != oam_get_agent_xml_info(p_agent_info, &agent_xml_info, &max_process_on_all_agent))
    {
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    g_oam_sim_t.max_no_process = max_process_on_master + max_process_on_all_agent;
    if (OAM_SUCCESS != oam_mem_alloc(g_oam_sim_t.max_no_process * sizeof(oam_process_info_t), (void*) &g_oam_sim_t.p_info, &err))
    {
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    flag_found = OAM_ZERO;
    oam_xml_find_ptr(p_master_list, &p_process_node, "process_info");

    if(!flag_found) {
        OAM_LOG(OAM, OAM_ERROR, "Failed to find process_info in master_list");
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    while ((OAM_NULL != p_process_node) &&
            (i < max_process_on_master))
    {
        if (XML_ELEMENT_NODE != p_process_node->type)
        {
            p_process_node = p_process_node->next;
            continue;
        }
        p_tag_value =
            (Char8*)oam_xmlNodeGetContent(p_process_node, "process_name");

    /*SPR 22121 Start */
        if(!oam_strcmp(process_name, OAM_INVALID_STR)) {
            OAM_LOG(OAM, OAM_ERROR, "Failed to read tag num_of_process");
            OAM_FUN_EXIT_TRACE();
            return OAM_FAILURE;
        }
        oam_strncpy(process_name, p_tag_value, OAM_MAX_STR_LEN);
        process_name[OAM_MAX_STR_LEN - OAM_ONE] = '\0';

        /* Fill process name */
        oam_fill_process_name(g_oam_sim_t.p_info[i].name, p_tag_value);

        p_args =
            (Char8*)oam_xmlNodeGetContent(p_process_node, "process_args");

        if(!oam_strcmp(p_args, OAM_INVALID_STR)) {
            OAM_LOG(OAM, OAM_ERROR, "Failed to read tag process_args for process %s", process_name);
            OAM_FUN_EXIT_TRACE();
            return OAM_FAILURE;
        }
        oam_strncpy(process_args, p_args, OAM_MAX_STR_LEN);
        process_args[OAM_MAX_STR_LEN - OAM_ONE] = '\0';

        /* Fill argument for this process */
        if (OAM_SUCCESS != oam_fill_process_args(g_oam_sim_t.p_info[i].name,
                    &g_oam_sim_t.p_info[i], p_args))
        {
            OAM_LOG(OAM, OAM_ERROR, "Failed to populate argument list %s", process_args);
            OAM_FUN_EXIT_TRACE();
            return OAM_FAILURE;
        }

        if (OAM_SUCCESS != oam_fill_cksum(g_oam_sim_t.p_info[i].name, &g_oam_sim_t.p_info[i].cksum))
        {
            OAM_LOG(OAM,OAM_ERROR, "Failed to get Cksum for process %s",
                    g_oam_sim_t.p_info[i].name);
        }

        g_oam_sim_t.p_info[i].pid = OAM_ZERO;
        g_oam_sim_t.p_info[i].is_remote = OAM_FALSE;
        g_oam_sim_t.p_info[i].instance_id = OAM_ZERO; /* Initization purpose only */
        OAM_LOG(OAM, OAM_INFO, "Process[%s] Args[%s] Bin name[%s] added in process table",
                process_name, process_args,
                g_oam_sim_t.p_info[i].name);
        i++;
        p_process_node = p_process_node->next;
    }

    for (k = OAM_ZERO; k < agent_xml_info.num_of_instance; k++)
    {
        for (m = OAM_ZERO; m < agent_xml_info.p_inst_xml_info[k].num_process; m++)
        {
            if (i < g_oam_sim_t.max_no_process) /* Continue where last index was left */
            {
                p_tag_value =
                    (Char8*)oam_xmlNodeGetContent(agent_xml_info.p_inst_xml_info[k].process_info[m], "process_name");

                if(!oam_strcmp(p_tag_value, OAM_INVALID_STR)) {
                    OAM_LOG(OAM, OAM_ERROR, "Failed to read tag num_of_process");
                    OAM_FUN_EXIT_TRACE();
                    return OAM_FAILURE;
                }
                oam_strncpy(process_name, p_tag_value, OAM_MAX_STR_LEN);
                process_name[OAM_MAX_STR_LEN - OAM_ONE] = '\0';

                /* Fill process name */
                oam_fill_process_name(g_oam_sim_t.p_info[i].name, p_tag_value);

                p_args =
                    (Char8*)oam_xmlNodeGetContent(agent_xml_info.p_inst_xml_info[k].process_info[m], "process_args");

                if(!oam_strcmp(p_args, OAM_INVALID_STR)) {
                    OAM_LOG(OAM, OAM_ERROR, "Failed to read tag process_args for process %s", process_name);
                    OAM_FUN_EXIT_TRACE();
                    return OAM_FAILURE;
                }
                oam_strncpy(process_args, p_args, OAM_MAX_STR_LEN);
                process_args[OAM_MAX_STR_LEN - OAM_ONE] = '\0';

                /* Fill argument for this process */
                if (OAM_SUCCESS !=
                        oam_fill_process_args(g_oam_sim_t.p_info[i].name,
                            &g_oam_sim_t.p_info[i],  p_args))
                {
                    OAM_LOG(OAM, OAM_ERROR, "Failed to populate argument list %s", process_args);
                    OAM_FUN_EXIT_TRACE();
                    return OAM_FAILURE;
                }

                if (OAM_SUCCESS != oam_fill_cksum(g_oam_sim_t.p_info[i].name, &g_oam_sim_t.p_info[i].cksum))
                {
                    OAM_LOG(OAM,OAM_ERROR, "Failed to get Cksum for process %s",
                            g_oam_sim_t.p_info[i].name);
                }

                g_oam_sim_t.p_info[i].pid = OAM_ZERO;
                g_oam_sim_t.p_info[i].is_remote = OAM_TRUE;
                g_oam_sim_t.p_info[i].instance_id = agent_xml_info.p_inst_xml_info[k].instance_id;
                OAM_LOG(OAM, OAM_INFO, "Process[%s] Args[%s] Bin name[%s] Instance %u added in process table",
                        process_name, process_args, g_oam_sim_t.p_info[i].name,
                        g_oam_sim_t.p_info[i].instance_id);
    /*SPR 22121 End */
                i++;
            }
        }
        /* Release all memory */
        oam_mem_free(agent_xml_info.p_inst_xml_info[k].process_info, &err);
    }
    oam_mem_free(agent_xml_info.p_inst_xml_info, &err);

    OAM_FUN_EXIT_TRACE();
    return OAM_SUCCESS;
}
/* SPR 21889 end */

