/*******************************************************************************
 * File Name   : oam_qcli_utils.c
 *
 * Description : This file contains packet creation for every command queried
 *                over Qcli.
 *
 * Revision History:
 *
 * Date                 Author           Reference             Comments
 * -----                ------           ---------             --------
 * June,2012          Vivek K Dixit  OAM Design Document    Initial Version
 * September,2012     Rajesh Kumar                          Generic code for qcli
 * September,2013   Vinod Vyas                               Changes Related To IPV6                                                          * 
 *                                                           is added
 * April 2014         Vinods Vyas       CSR 57877 Fix
 * May 2014         Shalu Sagar         SPR 11324 FIX
 * JUne 2014        SHalu SAgar         SPR 11266 FIX
 * Aug 2014         Aditi Singh         SPR 13116
 * Copyright (c) 2012, Aricent Inc. All Rights Reserved
 *******************************************************************************/
/*******************************************************************************
 * Includes
 *******************************************************************************/
#include<oam_qcli_proto.h>
#include <oam_defines.h>
#include <oam_proto.h>
#include <oam_l2_if.h>
#include <logging.h>

/* CSR 57877 FIx Start */
#include <oam_mgmt_intf_api.h>
/* CSR 57877 FIx End*/


#define MAX_STRING_LENGTH 64
UInt16 qcli_trans_id = OAM_ZERO;
UInt8 ue_rel_flag = OAM_ZERO;

#define PRINT_CLI_SPV_RESULT_SUCCESS oam_fprintf(stdout,"\nCommand successful\n"); oam_print_line();
#define PRINT_CLI_SPV_RESULT_SUCCESS_UE_REL oam_fprintf(stdout,"\nYour Request For UE RELEASE is SENT !!\n"); oam_print_line();
#define PRINT_CLI_ADD_OBJECT_RESULT_SUCCESS  oam_fprintf(stdout,"\nADD object command successful\n"); oam_print_line();
#define PRINT_CLI_DELETE_OBJECT_RESULT_SUCCESS oam_fprintf(stdout,"\nDelete object command successful\n"); oam_print_line();
#define PRINT_CLI_GPV_RESULT_SUCCESS_NO_DATA_TO_DISPLAY oam_fprintf(stdout,"\nCommand successful: No data to display\n"); oam_print_line();
typedef struct cell_del_str_t
{
	char parent[MAX_STRING_LENGTH];
	char child[MAX_STRING_LENGTH];
}d2str_t;

UInt8 instance_no;
static UInt8 add_set_flag = 0;
UInt8 spv_after_add_status= 0;
UInt8 g_oam_cli_support_multi_instance = 0;
UInt8  kpiPdcpGetBitMap;
UInt8 Set_Flag=0;
UInt8 flag = 0;
UInt8 multi_parent_flag = 0;
static struct add_object_parameter add_params;

/*Note:- Parameters which are defined by TR-196 spec are included in this list */
/* SPR 594 FIX start*/
d2str_t param_list_warning[100]=		{{"AntennaInfo","AntennaPortsCount"},
	{"PDSCH","Pb"},
	{"SRS","SRSEnabled"},
	{"SRS","SRSBandwidthConfig"},
	{"SRS","SRSMaxUpPTS"},
	{"SRS","AckNackSRSSimultaneousTransmission"},
	{"PUCCH","DeltaPUCCHShift"},
	{"PUCCH","NRBCQI"},
	{"PUCCH","NCSAN"},
	{"PUCCH","N1PUCCHAN"},
	{"PUSCH","NSB"},
	{"PUSCH","HoppingMode"},
	{"PUSCH","HoppingOffset"},
	{"PUSCH","Enable64QAM"},
	{"ULRS","GroupHoppingEnabled"},
	{"ULRS","GroupAssignmentPUSCH"},
	{"ULRS","SequenceHoppingEnabled"},
	{"ULRS","CyclicShift"},
	{"PRACH","RootSequenceIndex"},
	{"PRACH","ConfigurationIndex"},
	{"PRACH","HighSpeedFlag"},
	{"PRACH","ZeroCorrelationZoneConfig"},
	{"PRACH","FreqOffset"},
   /*SPR 21193 FIXED START */
   {"RF","FreqBandIndicator"},
   {"RF","PBCHPowerOffset"},
   /*SPR 21193 FIXED START */
   {"RF","DLBandwidth"},
   {"RF","ULBandwidth"},
   {"RF","PhyCellID"},
   {"RF","PSCHPowerOffset"},
	{"RF","SSCHPowerOffset"},
	{"RF","ReferenceSignalPower"},
	{"RACH","NumberOfRaPreambles"},
	{"RACH","SizeOfRaGroupA"},
	{"RACH","MessageSizeGroupA"},
	{"RACH","ResponseWindowSize"},
	{"RACH","ContentionResolutionTimer"},
	{"TDD","SubFrameAssignment"},
	{"TDD","SpecialSubframePatterns"}
}; 
/* SPR 594 FIX end*/

extern prov_request_t prov_req;
oam_error_code_et error_code;
extern char wait_gb;
extern mess_send_t message_send;
extern int cache_track_index;
extern int is_cache_full;

extern mapping_cache_t mapping_cache[MAX_CACHE_LIMIT];

extern param_mapping_t g_oam_qcli_param_mapping_table[OAM_MAX_NUMBER_ELEMENT];
 /* Coverity ID 66657 Fix Start*/
#define OAM_PM_2D_TEMP_ARR_MEM_FREE(temp_arr,loop_counter, p_error_code)\
{\
       oam_counter_t loop_counter2 = OAM_ZERO;\
       for (loop_counter2 = OAM_ZERO; loop_counter2 < loop_counter; loop_counter2++)\
       {\
                  oam_mem_free(temp_arr[loop_counter2], &p_error_code);\
              }\
       oam_mem_free(temp_arr, &p_error_code);\
}
/* Coverity ID 66657 Fix End*/


/*******************************************************************************
 * Function Name  : oam_validate_special_param
 * Description    : For checking cell delete parameter
 * Inputs         : no_of_param : no of parameters 
 *                  param : param_set structure
 * Outputs        : NONE
 * Returns        : SUCCESS or FAILURE
 *******************************************************************************/

UInt8 oam_validate_special_param(char * const *V,UInt8 no_of_param,param_set *param)
{
    /* Cov cid 54875 fix */
	UInt8 loop_counter1;
    UInt32 loop_counter2,cnt=0;

	SInt32 ch, wordCount=0;
    /* Coverity 115230 Fix Start */
	char tempstr[200] = {0};
    char  *nextWordPtr = NULL;
    /* Coverity 115230 Fix End */
	SInt8 List[MAX_STRING_LENGTH][MAX_STRING_LENGTH];
	d2str_t *par_table_celldel = &param_list_warning[0];

    oam_strcpy((char *) tempstr,(char *) *V);   

    nextWordPtr = strtok(tempstr," \""); 

	while (nextWordPtr != NULL) 
	{
		oam_strcpy((char *)List[wordCount],(char *)nextWordPtr);

		wordCount++;

		nextWordPtr = strtok(NULL," \"");
	}

    /* OAM BCOM Code Comment Changes Start */
    if (OAM_ZERO==wordCount) {
      OAM_LOG(OAM, OAM_ERROR, "Returning Word count value: %d", wordCount);
 
      OAM_FUN_EXIT_TRACE();
      return OAM_TWO;
    }
    /* OAM BCOM Code Comment Changes End */

	for(loop_counter1=0;loop_counter1<no_of_param;loop_counter1++)
	{
		for(loop_counter2=0;loop_counter2<ARRSIZE(param_list_warning);loop_counter2++)
		{
            if((!oam_strcmp(param[loop_counter1].param,par_table_celldel[loop_counter2].child)) && (!oam_strcmp((char *)List[wordCount-1],par_table_celldel[loop_counter2].parent)))
			{
               oam_fprintf(stderr,"%s, ",param[loop_counter1].param);    
				cnt++;
				break;
			}  
		}
	}
	if(cnt==0)
		return OAM_ZERO;
   oam_fprintf(stderr,"This parameter change can lead to CELL Deletion, Do you want to proceed(Y/N):");
	/* Coverity Fix: Vivek */

    while((ch = OAM_GETCHAR()) == '\n');
	if((ch=='Y')||(ch=='y'))
	{
		return OAM_ZERO;
	}
	else if((ch=='N')||(ch=='n'))
	{
		return OAM_ONE;
	}
	else {
		return OAM_TWO;

	}
}


/*******************************************************************************
 * Function Name  : oam_construct_api_header
 * Description    : To fill the buffer with CSPL header
 * Inputs         : p_header : pointer to the buffer allocated
 *                  version_id : version of CSPL
 *                  src_module_id : Source id
 *                  dest_module_id : destination id
 *                  api_id : API id
 *                  api_size : total size of buffer
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
/*
   void
   oam_construct_api_header
   (
   UInt8 *p_header,
   UInt8 version_id,
   UInt16 src_module_id,
   UInt16 dest_module_id,
   UInt16 api_id,
   UInt16 api_size
   )
   {
   / * version * /

   p_header[OAM_CONST_ZERO] = 1; 
/ * from (HI) * /
   p_header[OAM_CONST_ONE] = (src_module_id & 0xFF00) >> OAM_BYTESIZE;

/ * from (LOW) * /
   p_header[OAM_CONST_TWO] = src_module_id & 0x00FF;  

/ * to (HI) * /
   p_header[OAM_CONST_THREE] = (dest_module_id & 0xFF00) >> OAM_BYTESIZE; 

/ * to (LOW) * /
p_header[OAM_CONST_FOUR] =dest_module_id  & 0x00FF;

/ * api id (HI) * /
   p_header[OAM_CONST_FIVE] = (api_id & 0xFF00) >> OAM_BYTESIZE;

/ * api id (LOW) * /
p_header[OAM_CONST_SIX] = api_id& 0x00FF;

/ * api size (HI) * /
   p_header[OAM_CONST_SEVEN] = (api_size & 0xFF00) >> OAM_BYTESIZE;

/ * api size (LOW) * /
p_header[OAM_CONST_EIGHT] = api_size & 0x00FF;

/ * spare bytes * /
p_header[OAM_CONST_NINE]     = 0x00;
p_header[OAM_CONST_TEN]    = 0x00;
p_header[OAM_CONST_ELEVEN]    = 0x00;
p_header[OAM_CONST_TWELVE]    = 0x00;
p_header[OAM_CONST_THIRTEEN]    = 0x00;
p_header[OAM_CONST_FOURTEEN]    = 0x00;
p_header[OAM_CONST_FIFTEEN]    = 0x00;
}
*/
/*******************************************************************************
 * Function Name  : oam_construct_qcli_header
 * Description    : To fill the buffer with Qcli header
 * Inputs         : p_header : pointer to the buffer allocated
 *                  id : Qcli id
 *                  src_id : Source id
 *                  CSPL_ID : OAM id
 *                  api_id : API id
 *                  trans_id : Transaction id
 * Outputs        : NONE
 * Returns        : NONE
 *******************************************************************************/
/*
   void
   oam_construct_qcli_header
   (
   UInt8 *p_header,
   UInt16 trans_id,
   UInt16 src_id,
   UInt16 oam_id,
   UInt16 api_id,
   UInt16 size,
   UInt16 id
   )
   {
   oam_qcli_header_t fill_qcli_struct;
   fill_qcli_struct.trans_id = trans_id;
   fill_qcli_struct.src_id = src_id;
   fill_qcli_struct.dest_id = oam_id;
   fill_qcli_struct.api_id = api_id;
   fill_qcli_struct.size = size;
   fill_qcli_struct.id = id;
   oam_memcpy(p_header,&fill_qcli_struct,sizeof(fill_qcli_struct));
   }
   */
/****************************************************************************
 * Function Name  : oam_get_fresh_trans_id
 * Inputs         : trans_id
 * Outputs        : none
 * Returns        : UInt16 value
 * Description    : This function returns the fresh transection id
 ****************************************************************************/
/*
   UInt16
   oam_get_fresh_trans_id
   (
   UInt16 trans_id
   )
   {
   if ((trans_id + 1) >= OAM_MAX_TRANS_ID)
   {
   / * trans-id has reached its limit, so restart the counter * /
   trans_id = 0;
   }
   return ++trans_id;
   }
   */
/*******************************************************************************
 * Function Name  : oam_fap_service_barring_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 V - Pointer to character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/

void
oam_fap_service_barring_payload
(
 UInt8 *p_header,
 rrmc_cell_barred_et rrmc_cell_barr_param
 )
{
	oam_fap_service_barr_t payload_fap_serv;
	payload_fap_serv.barrforemer = rrmc_cell_barr_param;
	oam_memcpy(p_header,&payload_fap_serv,sizeof(payload_fap_serv));
}

#ifdef OAM_SON_ENABLE
void
oam_son_log_lvl_payload
(
 UInt8 *p_header,
 setson son_log_lvl_param,
 char *V
 )
{
	oam_son_set_log_level_req_t son_level_payload;
	son_level_payload.module_id = (son_module_id_et)oam_atoi(V);
	son_level_payload.log_level = (son_oam_log_level_et)son_log_lvl_param;
	oam_memcpy(p_header,&son_level_payload,sizeof(oam_son_set_log_level_req_t));
}
#endif

void
oam_rrc_log_level_payload
(
 UInt8 *p_header,
 setloglevell3rrccommands rrc_log_level_param

 )
{
	oam_rrc_set_log_level_req_t rrc_log_lvl_payload;
	rrc_log_lvl_payload.log_level = rrc_log_level_param;
	oam_memcpy(p_header,&rrc_log_lvl_payload,sizeof(oam_rrc_set_log_level_req_t));
}

void
oam_X2AP_log_level_payload
(
 UInt8 *p_header,
 setloglevelX2APcommands X2AP_log_level_param

 )
{
	oam_X2AP_set_log_level_req_t X2AP_log_lvl_payload;
	X2AP_log_lvl_payload.log_level = X2AP_log_level_param;
	oam_memcpy(p_header,&X2AP_log_lvl_payload,sizeof(oam_X2AP_set_log_level_req_t));

}


void
oam_S1AP_log_level_payload
(
 UInt8 *p_header,
 setloglevelS1APcommands S1AP_log_level_param

 )
{
	oam_S1AP_set_log_level_req_t S1AP_log_lvl_payload;
	S1AP_log_lvl_payload.log_level = S1AP_log_level_param;
	oam_memcpy(p_header,&S1AP_log_lvl_payload,sizeof(oam_S1AP_set_log_level_req_t));
}

	void
oam_fill_rrm_log_lvl_payload(UInt8 *p_header,
		rrm_oam_log_level_et rrm_log_lvl,
		Char8 *V)
{
	oam_rrm_set_log_level_req_t log_level_payload;
	log_level_payload.module_id = (rrm_internal_module_id_et)oam_atoi(V);
	log_level_payload.log_level = rrm_log_lvl;
	oam_memcpy(p_header, &log_level_payload,
			sizeof(oam_rrm_set_log_level_req_t));
}

void
oam_enable_rrm_log_lvl_payload
(
 UInt8 *p_header,
 char *V,
 rrm_oam_log_level_et log_level
 )
{
    oam_rrm_log_enable_disable_req_t rrm_payload;
    rrm_payload.module_id = (rrm_internal_module_id_et)oam_atoi(V);
    rrm_payload.log_config.bitmask = RRM_OAM_LOG_LEVEL_PRESENT;
    rrm_payload.log_config.log_level = log_level;
    rrm_payload.log_config.log_on_off = RRM_OAM_LOG_ON;
    oam_memcpy(p_header,&rrm_payload,sizeof(rrm_payload));
}

void
oam_enable_rrm_error_payload
(
 UInt8 *p_header,
 char *V
 )
{
	oam_rrm_log_enable_disable_req_t rrm_error_payload;
	rrm_error_payload.module_id = (rrm_internal_module_id_et)oam_atoi(V);
	rrm_error_payload.log_config.bitmask = RRM_OAM_LOG_LEVEL_PRESENT;
	rrm_error_payload.log_config.log_level = RRM_OAM_ERROR;
	rrm_error_payload.log_config.log_on_off = RRM_OAM_LOG_ON;
	oam_memcpy(p_header,&rrm_error_payload,sizeof(rrm_error_payload));
}

void
oam_enable_rrm_warn_payload
(
 UInt8 *p_header,
 char *V
 )
{
	oam_rrm_log_enable_disable_req_t rrm_warn_payload;
	rrm_warn_payload.module_id = (rrm_internal_module_id_et)oam_atoi(V);
	rrm_warn_payload.log_config.bitmask = RRM_OAM_LOG_LEVEL_PRESENT;
	rrm_warn_payload.log_config.log_level = (rrm_oam_log_level_et)OAM_RRM_WARNINGLOG;
	rrm_warn_payload.log_config.log_on_off = (rrm_oam_log_on_off_et)OAM_RRM_LOG_ENABLE;
	oam_memcpy(p_header,&rrm_warn_payload,sizeof(rrm_warn_payload));
}


void
oam_enable_rrm_brief_payload
(
 UInt8 *p_header,
 char *V
 )
{
	oam_rrm_log_enable_disable_req_t rrm_brief_payload;
	rrm_brief_payload.module_id = (rrm_internal_module_id_et)oam_atoi(V);
	rrm_brief_payload.log_config.bitmask = RRM_OAM_LOG_LEVEL_PRESENT;
	/* coverity 52811 */
	rrm_brief_payload.log_config.log_level = (rrm_oam_log_level_et)OAM_RRM_BRIEFLOG;
	rrm_brief_payload.log_config.log_on_off = (rrm_oam_log_on_off_et)OAM_RRM_LOG_ENABLE;
	oam_memcpy(p_header,&rrm_brief_payload,sizeof(rrm_brief_payload));
}


void
oam_enable_rrm_detail_payload
(
 UInt8 *p_header,
 char *V
 )
{
	oam_rrm_log_enable_disable_req_t rrm_detailed_payload;
	rrm_detailed_payload.module_id = (rrm_internal_module_id_et)oam_atoi(V);
	rrm_detailed_payload.log_config.bitmask = RRM_OAM_LOG_LEVEL_PRESENT;
	rrm_detailed_payload.log_config.log_level = (rrm_oam_log_level_et)OAM_RRM_DETAILEDLOG;
	rrm_detailed_payload.log_config.log_on_off = (rrm_oam_log_on_off_et)OAM_RRM_LOG_ENABLE;
	oam_memcpy(p_header,&rrm_detailed_payload,sizeof(rrm_detailed_payload));
}

#ifdef OAM_SON_ENABLE 
/*******************************************************************************
 * Function Name  : oam_son_enable_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *                  v - Enum function value passed for subcommands
 *                  log_level - log level requested to enable at SON
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_son_enable_payload
(
 UInt8 *p_header,
 char *V,
 setson log_level
 )
{
    oam_son_log_enable_disable_req_t son_enable_log;
    son_enable_log.feature_id = SON_ALL_FEATURE;
    son_enable_log.log_configuration.bitmask = (UInt16)OAM_ZERO;
    son_enable_log.log_configuration.bitmask |= SON_OAM_MODULE_LOG_LEVEL_PRESENT;
    son_enable_log.module_id = (son_module_id_et)oam_atoi(V);
    son_enable_log.log_configuration.log_level = (son_oam_log_level_et)log_level;
    son_enable_log.log_configuration.log_on_off = (son_oam_log_on_off_et)OAM_SON_LOG_ENABLE;
    oam_memcpy(p_header,&son_enable_log,sizeof(son_enable_log));
}

/*******************************************************************************
 * Function Name  : oam_son_enable_error_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_son_enable_error_payload
(
 UInt8 *p_header,
 char *V
 )
{
	oam_son_log_enable_disable_req_t son_enable_err;
	son_enable_err.feature_id = SON_ALL_FEATURE;            
	son_enable_err.log_configuration.bitmask = (UInt16)OAM_ZERO;
	son_enable_err.log_configuration.bitmask |= SON_OAM_MODULE_LOG_LEVEL_PRESENT;
	son_enable_err.module_id = (son_module_id_et)oam_atoi(V);
	son_enable_err.log_configuration.log_level = (son_oam_log_level_et)OAM_SON_ERRORLOG;
	son_enable_err.log_configuration.log_on_off = (son_oam_log_on_off_et)OAM_SON_LOG_ENABLE;
	oam_memcpy(p_header,&son_enable_err,sizeof(son_enable_err));
}

/*******************************************************************************
 * Function Name  : oam_son_enable_warning_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_son_enable_warning_payload
(
 UInt8 *p_header,
 char *V
 )
{
	oam_son_log_enable_disable_req_t son_enable_warn;
	son_enable_warn.log_configuration.bitmask = (UInt16)OAM_ZERO;
	son_enable_warn.log_configuration.bitmask |= SON_OAM_MODULE_LOG_LEVEL_PRESENT;
	son_enable_warn.feature_id = SON_ALL_FEATURE;
	son_enable_warn.module_id = (son_module_id_et)oam_atoi(V);
	son_enable_warn.log_configuration.log_level = (son_oam_log_level_et)OAM_SON_WARNINGLOG;
	son_enable_warn.log_configuration.log_on_off = (son_oam_log_on_off_et)OAM_SON_LOG_ENABLE;
	oam_memcpy(p_header,&son_enable_warn,sizeof(son_enable_warn));
}

/*******************************************************************************
 * Function Name  : oam_son_enable_brief_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_son_enable_brief_payload
(
 UInt8 *p_header,
 char *V
 )
{
	oam_son_log_enable_disable_req_t son_enable_brief;
	son_enable_brief.feature_id = SON_ALL_FEATURE; 
	son_enable_brief.log_configuration.bitmask = (UInt16)OAM_ZERO;
	son_enable_brief.log_configuration.bitmask |= SON_OAM_MODULE_LOG_LEVEL_PRESENT;
	son_enable_brief.module_id = (son_module_id_et)oam_atoi(V);
	son_enable_brief.log_configuration.log_level = (son_oam_log_level_et)OAM_SON_BRIEFLOG;
	son_enable_brief.log_configuration.log_on_off = (son_oam_log_on_off_et)OAM_SON_LOG_ENABLE;
	oam_memcpy(p_header,&son_enable_brief,sizeof(son_enable_brief));
}

/*******************************************************************************
 * Function Name  : oam_son_enable_detailed_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_son_enable_detailed_payload
(
 UInt8 *p_header,
 char *V
 )
{
	oam_son_log_enable_disable_req_t son_enable_detail;
	son_enable_detail.feature_id = SON_ALL_FEATURE;
	son_enable_detail.log_configuration.bitmask = (UInt16)OAM_ZERO;
	son_enable_detail.log_configuration.bitmask |= SON_OAM_MODULE_LOG_LEVEL_PRESENT;  
	son_enable_detail.module_id = (son_module_id_et)oam_atoi(V);
	son_enable_detail.log_configuration.log_level = (son_oam_log_level_et)OAM_SON_DETAILEDLOG;
	son_enable_detail.log_configuration.log_on_off = (son_oam_log_on_off_et)OAM_SON_LOG_ENABLE;
	oam_memcpy(p_header,&son_enable_detail,sizeof(son_enable_detail));
}
#endif

/*******************************************************************************
 * Function Name  : oam_rrm_disable_error_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_rrm_disable_error_payload
(
 UInt8 *p_header,
 char *V
 )
{
	oam_rrm_log_enable_disable_req_t rrm_disable_err_payload;
	rrm_disable_err_payload.module_id = (rrm_internal_module_id_et)oam_atoi(V);
	rrm_disable_err_payload.log_config.bitmask = RRM_OAM_LOG_LEVEL_PRESENT;
	rrm_disable_err_payload.log_config.log_level = (rrm_oam_log_level_et)RRM_OAM_BRIEF;
	rrm_disable_err_payload.log_config.log_on_off = (rrm_oam_log_on_off_et)OAM_RRM_LOG_DISABLE;
	oam_memcpy(p_header,&rrm_disable_err_payload,sizeof(rrm_disable_err_payload));
}

/*******************************************************************************
 * Function Name  : oam_rrm_disable_warning_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_rrm_disable_warning_payload
(
 UInt8 *p_header,
 char *V
 )
{
	oam_rrm_log_enable_disable_req_t rrm_disable_warn_payload;
	rrm_disable_warn_payload.module_id = (rrm_internal_module_id_et)oam_atoi(V);
	rrm_disable_warn_payload.log_config.bitmask = RRM_OAM_LOG_LEVEL_PRESENT;
	rrm_disable_warn_payload.log_config.log_level = (rrm_oam_log_level_et)OAM_RRM_WARNINGLOG;
	rrm_disable_warn_payload.log_config.log_on_off = (rrm_oam_log_on_off_et)OAM_RRM_LOG_DISABLE;
	oam_memcpy(p_header,&rrm_disable_warn_payload,sizeof(rrm_disable_warn_payload));
}

/*******************************************************************************
 * Function Name  : oam_rrm_disable_brief_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/

void
oam_rrm_disable_brief_payload
(
 UInt8 *p_header,
 char *V
 )
{
	oam_rrm_log_enable_disable_req_t rrm_disable_brief_payload;
	rrm_disable_brief_payload.module_id = (rrm_internal_module_id_et)oam_atoi(V);
	rrm_disable_brief_payload.log_config.bitmask = RRM_OAM_LOG_LEVEL_PRESENT;
	rrm_disable_brief_payload.log_config.log_level = (rrm_oam_log_level_et)OAM_RRM_INFOLOG;
	rrm_disable_brief_payload.log_config.log_on_off = (rrm_oam_log_on_off_et)OAM_RRM_LOG_DISABLE;
	oam_memcpy(p_header,&rrm_disable_brief_payload,sizeof(rrm_disable_brief_payload));
}

/*******************************************************************************
 * Function Name  : oam_rrm_disable_detailed_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/

void
oam_rrm_disable_detailed_payload
(
 UInt8 *p_header,
 char *V
 )
{
	oam_rrm_log_enable_disable_req_t rrm_disable_detail_payload;
	rrm_disable_detail_payload.module_id = (rrm_internal_module_id_et)oam_atoi(V);
	rrm_disable_detail_payload.log_config.bitmask = RRM_OAM_LOG_LEVEL_PRESENT;
	rrm_disable_detail_payload.log_config.log_level = (rrm_oam_log_level_et)OAM_RRM_DETAILEDLOG;
	rrm_disable_detail_payload.log_config.log_on_off = (rrm_oam_log_on_off_et)OAM_RRM_LOG_DISABLE;
	oam_memcpy(p_header,&rrm_disable_detail_payload,sizeof(rrm_disable_detail_payload));
}

/*******************************************************************************
 * Function Name  : oam_son_disable_error_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
#ifdef OAM_SON_ENABLE
void
oam_son_disable_error_payload
(
 UInt8 *p_header,
 char *V
 )
{
	oam_son_log_enable_disable_req_t son_disable_err;
	son_disable_err.feature_id = SON_ALL_FEATURE;
	son_disable_err.log_configuration.bitmask = (UInt16)OAM_ZERO;
	son_disable_err.log_configuration.bitmask |= SON_OAM_MODULE_LOG_LEVEL_PRESENT;
	son_disable_err.module_id = (son_module_id_et)oam_atoi(V);
	son_disable_err.log_configuration.log_level = (son_oam_log_level_et)OAM_SON_ERRORLOG;
	son_disable_err.log_configuration.log_on_off = (son_oam_log_on_off_et)OAM_SON_LOG_DISABLE;
	oam_memcpy(p_header,&son_disable_err,sizeof(oam_son_log_enable_disable_req_t));
}
/*******************************************************************************
 * Function Name  : oam_son_disable_warning_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_son_disable_warning_payload
(
 UInt8 *p_header,
 char *V
 )
{
	oam_son_log_enable_disable_req_t son_disable_warn;
	son_disable_warn.feature_id = (son_feature_id_et)SON_ALL_FEATURE;
	son_disable_warn.log_configuration.bitmask = (UInt16)OAM_ZERO;
	son_disable_warn.module_id = (son_module_id_et)oam_atoi(V);
	son_disable_warn.log_configuration.log_level = (son_oam_log_level_et)OAM_SON_WARNINGLOG;
	son_disable_warn.log_configuration.log_on_off = (son_oam_log_on_off_et)OAM_SON_LOG_DISABLE;
	oam_memcpy(p_header,&son_disable_warn,sizeof(son_disable_warn));
}
/*******************************************************************************
 * Function Name  : oam_son_disable_brief_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_son_disable_brief_payload
(
 UInt8 *p_header,
 char *V
 )
{
	oam_son_log_enable_disable_req_t son_disable_brief;
	son_disable_brief.feature_id = (son_feature_id_et)SON_ALL_FEATURE;
	son_disable_brief.log_configuration.bitmask = (UInt16)OAM_ZERO;
	son_disable_brief.module_id = (son_module_id_et)oam_atoi(V);
	son_disable_brief.log_configuration.log_level = (son_oam_log_level_et)OAM_SON_BRIEFLOG;
	son_disable_brief.log_configuration.log_on_off = (son_oam_log_on_off_et)OAM_SON_LOG_DISABLE;
	oam_memcpy(p_header,&son_disable_brief,sizeof(son_disable_brief));
}
/*******************************************************************************
 * Function Name  : oam_son_disable_detailed_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_son_disable_detailed_payload
(
 UInt8 *p_header,
 char *V
 )
{
	oam_son_log_enable_disable_req_t son_disable_detail;
	son_disable_detail.feature_id = (son_feature_id_et)SON_ALL_FEATURE;

	son_disable_detail.log_configuration.bitmask = (UInt16)OAM_ZERO;
	son_disable_detail.module_id = (son_module_id_et)oam_atoi(V);
	son_disable_detail.log_configuration.log_level = (son_oam_log_level_et)OAM_SON_DETAILEDLOG;
	son_disable_detail.log_configuration.log_on_off = (son_oam_log_on_off_et)OAM_SON_LOG_DISABLE;
	oam_memcpy(p_header,&son_disable_detail,sizeof(son_disable_detail));
}
#endif

/*******************************************************************************
 * Function Name  : oam_call_trace_start_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *                  V1 : Pointer to character strings
 *                  V2 : Pointer to character strings
 *                  V3 : Pointer to character strings
 *                  V4 : Pointer to character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_call_trace_start_payload
(
 UInt8 *p_header,
 char *V1,
 char *V2,
 char *V3,
 char *V4,
 char *V5 /* SPR FIX 7851 */
 )
{
	UInt8* p_temp =OAM_NULL;
	UInt16 Value = OAM_ZERO;
	UInt8 count = OAM_ZERO;
	UInt32 Var5;
	oam_rrc_cell_traffic_trace_start_t oam_rrc_cell_traffic_trace_start;  
	oam_rrc_cell_traffic_trace_start.cell_index = (UInt8)oam_atoi(V1);
	oam_rrc_cell_traffic_trace_start.interface_to_trace = (UInt8)oam_atoi(V2);
	oam_rrc_cell_traffic_trace_start.trace_depth = (UInt8)oam_atoi(V3);
	oam_rrc_cell_traffic_trace_start.trace_collection_entity.length = oam_strlen(V4);
	oam_rrc_cell_traffic_trace_start.trace_collection_entity.data[OAM_ZERO] = '\0';
	/* coverity 36171 */	
	oam_rrc_cell_traffic_trace_start.traceRef[0] = '\0'; 		
	if(oam_isValidIp4((UInt8 *)V4)==OAM_ONE)
	{
		/* coverity 41021 */
		oam_rrc_cell_traffic_trace_start.trace_collection_entity.length = OAM_FOUR; 
		p_temp = (UInt8 *)oam_strtok((char *)V4,".");
		while(p_temp != OAM_NULL)
		{
			Value = oam_atoi((char *)p_temp);
			oam_rrc_cell_traffic_trace_start.trace_collection_entity.data[count++] = Value;
			p_temp=(UInt8 *)oam_strtok(OAM_NULL,".");
		}
	}
	else if (oam_isValidIp6((UInt8 *)V4)==OAM_ONE)
	{
		oam_rrc_cell_traffic_trace_start.trace_collection_entity.length = OAM_SIXTEEN;
		OAM_INET_PTON(AF_INET6,(char *)V4,oam_rrc_cell_traffic_trace_start.trace_collection_entity.data);
	}
        /*SPR FIX 11324 + */
        else
        {
           oam_rrc_cell_traffic_trace_start.trace_collection_entity.length = OAM_ZERO;
        }
         /*SPR FIX 11324 - */
	/*SPR FIX 7851 */     
    /*SPR 15196 start*/
	Var5 = (UInt32)oam_atoi(V5);
    /*SPR 15196 end*/
    /* SPR 15875 Fix Start */
    oam_rrc_cell_traffic_trace_start.traceRef[OAM_FIVE] = (UInt8) (Var5 & 0x000000FF);
    oam_rrc_cell_traffic_trace_start.traceRef[OAM_FOUR] = (UInt8) ((Var5 >> OAM_EIGHT)& 0x000000FF);
    oam_rrc_cell_traffic_trace_start.traceRef[OAM_THREE] = (UInt8) ((Var5>> OAM_SIXTEEN)& 0x000000FF);
    /* SPR 15875 Fix Stop */

	oam_memcpy(p_header, &oam_rrc_cell_traffic_trace_start,sizeof(oam_rrc_cell_traffic_trace_start_t));
}

/*******************************************************************************
 * Function Name  : oam_call_trace_stop_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *                  V : Pointer to character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_call_trace_stop_payload
(
 UInt8 *p_header,
 char *V
 )
{
	oam_call_trace_stop_t payload_call_trace_stop;
	payload_call_trace_stop.cell_index = (UInt8)oam_atoi(V);
	oam_memcpy(p_header,&payload_call_trace_stop,sizeof(payload_call_trace_stop));
}

/*******************************************************************************
 * Function Name  : oam_gtpu_incoming_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_gtpu_incoming_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_gtpu_outgoing_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_gtpu_outgoing_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}
/*******************************************************************************
 * Function Name  : oam_gtpu_fsm_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/

void
oam_gtpu_fsm_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_gtpu_error_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_gtpu_error_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_gtpu_incoming_l2all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_gtpu_incoming_l2all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_gtpu_outgoing_l2all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_gtpu_outgoing_l2all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_gtpu_fsm_l2all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_gtpu_fsm_l2all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_gtpu_error_l2all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_gtpu_error_l2all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_set_log_cat_disable_mac_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_set_log_cat_disable_mac_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_l2_disable_sys_fail_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_l2_disable_sys_fail_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_disable_lmac_l1_inf_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_disable_lmac_l1_inf_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_ul_harq_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_disable_mac_ul_harq_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_dl_harq_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_disable_mac_dl_harq_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_rach_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_disable_mac_rach_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_rrc_inf_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_disable_mac_rrc_inf_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_oam_inf_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_disable_mac_oam_inf_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_disble_mac_dl_pc_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_disble_mac_dl_pc_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_ul_pc_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_disable_mac_ul_pc_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_rrc_fatallog_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 val - Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_rrc_fatallog_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_rach_maxharq_msg_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *                  V : Pointer to character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_rach_maxharq_msg_payload
(
 UInt8 *p_header,
 char *V
 )
{
	oam_rach_maxharq_msg_t payload_rach_maxharq_msg;
	payload_rach_maxharq_msg.maxharq = (UInt8)oam_atoi(V);
	oam_memcpy(p_header,&payload_rach_maxharq_msg,sizeof(payload_rach_maxharq_msg));
}

/*******************************************************************************
 * Function Name  : oam_msg_power_offset_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *                  V : Pointer to character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_msg_power_offset_payload
(
 UInt8 *p_header,
 rrmc_group_b_msg_power_offset_et msg_pwr_val
 )
{
	oam_msg_power_t payload_msg_pwr;
	payload_msg_pwr.msg_pwr_val = msg_pwr_val;
	oam_memcpy(p_header,&payload_msg_pwr,sizeof(payload_msg_pwr));
}

/*******************************************************************************
 * Function Name  : oam_pwr_ramp_step_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *                  V : Pointer to character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_pwr_ramp_step_payload
(
 UInt8 *p_header,
 rrmc_power_ramping_step_et pwr_ramp_val
 )
{
	oam_pwr_ramp_t payload_pwr_ramp;
	payload_pwr_ramp.pwr_rmp_val = pwr_ramp_val;//(UInt8)oam_atoi(V);
	oam_memcpy(p_header,&payload_pwr_ramp,sizeof(payload_pwr_ramp));
}


/*******************************************************************************
 * Function Name  : oam_init_recv_tgt_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *                  V : Pointer to character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_init_recv_tgt_payload
(
 UInt8 *p_header,
 rrmc_preamble_init_rec_target_pow_et init_tgt_val
 )
{
	oam_init_recv_t payload_init_recv;
	payload_init_recv.init_tgt = init_tgt_val;
	oam_memcpy(p_header,&payload_init_recv,sizeof(payload_init_recv));
}

/*******************************************************************************
 * Function Name  : oam_preamble_trans_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *                  V : Pointer to character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_preamble_trans_payload
(
 UInt8 *p_header,
 rrmc_preamble_trans_max_et trans_max_val
 )
{
	oam_trans_max_t payload_trans_max;
	payload_trans_max.trans_val = trans_max_val;
	oam_memcpy(p_header,&payload_trans_max,sizeof(payload_trans_max));
}


/*******************************************************************************
 * Function Name  : oam_n310_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *                  V : Pointer to character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_n310_payload
(
 UInt8 *p_header,
 rrmc_timer_n310_et n310_val
 )
{
	oam_n310_t payload_n310;
	payload_n310.val = n310_val;
	oam_memcpy(p_header,&payload_n310,sizeof(payload_n310));
}

/*******************************************************************************
 * Function Name  : oam_n311_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *                  V : Pointer to character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_n311_payload
(
 UInt8 *p_header,
 rrmc_timer_n311_et n311_val
 )
{
	oam_n311_t payload_n311;
	payload_n311.val = n311_val;
	oam_memcpy(p_header,&payload_n311,sizeof(payload_n311));
}

/*******************************************************************************
 * Function Name  : oam_t300_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *                  V : Pointer to character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_t300_payload
(
 UInt8 *p_header,
 rrmc_timer_300_301_et t300_val
 )
{
	oam_t300_t payload_t300;
	payload_t300.val = t300_val;
	oam_memcpy(p_header,&payload_t300,sizeof(payload_t300));
}

/*******************************************************************************
 * Function Name  : oam_t301_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *                  V : Pointer to character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_t301_payload
(
 UInt8 *p_header,
 rrmc_timer_300_301_et t301_val
 )
{
	oam_t301_t payload_t300;
	payload_t300.val = t301_val;
	oam_memcpy(p_header,&payload_t300,sizeof(payload_t300));
}

/*******************************************************************************
 * Function Name  : oam_t310_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *                  V : Pointer to character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_t310_payload
(
 UInt8 *p_header,
 rrmc_timer_310_et t310_val
 )
{
	oam_t310_t payload_t310;
	payload_t310.val = t310_val;
	oam_memcpy(p_header,&payload_t310,sizeof(payload_t310));
}

/*******************************************************************************
 * Function Name  : oam_t311_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *                  V : Pointer to character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_t311_payload
(
 UInt8 *p_header,
 rrmc_timer_311_et t311_val
 )
{
	oam_t311_t payload_t311;
	payload_t311.val = t311_val;
	oam_memcpy(p_header,&payload_t311,sizeof(payload_t311));
}
/*******************************************************************************
 * Function Name  : oam_show_stats_l3_rrc_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *                  V : Pointer to character strings
 *                  bit_mask : integer set for reset value
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_show_stats_l3_rrc_payload
(
 UInt8 *p_header,
 char *V,
 char *reset_stats
 )
{
	oam_show_rrc_cell_stats_t payload_stats_rrc = {0};
	payload_stats_rrc.cell_index = (UInt8)oam_atoi(V);
	payload_stats_rrc.reset_stats = (UInt8)oam_atoi(reset_stats);

	oam_memcpy(p_header,&payload_stats_rrc,sizeof(payload_stats_rrc));
}

/*******************************************************************************
 * Function Name  : oam_show_stats_l3_rrc_reset_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *                  V1 : Pointer to character strings
 *                  V2 : Pointer to character strings
 *                  bit_mask : integer set for reset value
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_show_stats_l3_rrc_reset_payload
(
 UInt8 *p_header,
 char *V1,
 char *V2,
 UInt16 bit_mask
 )
{
	oam_show_rrc_cell_stats_t payload_stats_reset = {0};
	payload_stats_reset.bitmask = bit_mask;
	payload_stats_reset.cell_index = (UInt8)oam_atoi(V1);
	payload_stats_reset.reset_stats = (UInt8)oam_atoi(V2);
	oam_memcpy(p_header,&payload_stats_reset,sizeof(payload_stats_reset));
}



/*******************************************************************************
 * Function Name  : oam_cell_mgmt_delete_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 V - Pointer to character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_cell_mgmt_delete_payload
(
 UInt8 *p_header,
 char *V
 )
{
	oam_cell_mgmt_delete_t payload_data_delete = {0};
	payload_data_delete.cellId = (UInt32)oam_atoi(V);
	oam_memcpy(p_header,&payload_data_delete,sizeof(payload_data_delete));
}
/*******************************************************************************
 * Function Name  : oam_cellmgmt_dlpc_params_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_cellmgmt_dlpc_params_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}
/*******************************************************************************
 * Function Name  : oam_cellmgmt_sync_signals_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_cellmgmt_sync_signals_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}
/*******************************************************************************
 * Function Name  : oam_cellmgmt_mib_info_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_cellmgmt_mib_info_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_cellmgmt_sib3_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_cellmgmt_sib3_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_cellmgmt_sib4_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_cellmgmt_sib4_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_cellmgmt_sib5_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_cellmgmt_sib5_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_cellmgmt_setup_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   V - Pointer to character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_cellmgmt_setup_payload
(
 UInt8 *p_header,
 char *V
 )
{
	oam_cellmgmt_setup_t payload_data_setup;
	payload_data_setup.cellId = (UInt32)oam_atoi(V);
	oam_memcpy(p_header,&payload_data_setup,sizeof(payload_data_setup));
}
/*******************************************************************************
 * Function Name  : oam_cellmgmt_start_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   V - Pointer to character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_cellmgmt_start_payload
(
 UInt8 *p_header,
 char *V
 )
{
	oam_cellmgmt_start_t payload_data_start;
	payload_data_start.cellId = (UInt32)oam_atoi(V);
	oam_memcpy(p_header,&payload_data_start,sizeof(payload_data_start));
}

/*******************************************************************************
 * Function Name  : oam_cellmgmt_stop_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   V - Pointer to character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_cellmgmt_stop_payload
(
 UInt8 *p_header,
 char *V
 )
{
	oam_cellmgmt_stop_t payload_data_stop;
	payload_data_stop.cellId = (UInt32)oam_atoi(V);
	oam_memcpy(p_header,&payload_data_stop,sizeof(payload_data_stop));
}

/*******************************************************************************
 * Function Name  : oam_config_kpi_mac_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   V1 : Pointer to character strings
 *		   V2 : Pointer to character strings
 *		   V3 : Pointer to character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_config_kpi_mac_payload
(
 UInt8 *p_header,
 char *V1,
 char *V2,
 char *V3,
 char *V4
 )
{
	oam_config_kpi_mac_t payload_data_configkpimac;
	payload_data_configkpimac.sample_duration = (UInt16)OAM_ZERO;
	payload_data_configkpimac.periodic_Reporting = (UInt8)oam_atoi(V1);
	payload_data_configkpimac.kpitype = (UInt16)oam_atoi(V2);
	payload_data_configkpimac.duration = (UInt16)oam_atoi(V3);
	payload_data_configkpimac.duration = (UInt16)oam_atoi(V4);
	oam_memcpy(p_header,&payload_data_configkpimac,
			sizeof(payload_data_configkpimac));
}

/*******************************************************************************
 * Function Name  : oam_config_kpi_pdcp_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   V1 : Pointer to character strings
 *		   V2 : Pointer to character strings
 *		   V3 : Pointer to character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_config_kpi_pdcp_payload
(
 UInt8 *p_header,
 char *V1,
 char *V2,
 char *V3,
 char *V4
 )
{
	oam_config_kpi_pdcp_t payload_data_configkpipdcp;
	payload_data_configkpipdcp.sample_duration = (UInt16)OAM_ZERO;
	payload_data_configkpipdcp.periodic_Reporting = (UInt8)oam_atoi(V1);
	payload_data_configkpipdcp.kpitype = (UInt8)oam_atoi(V2);
	payload_data_configkpipdcp.duration = (UInt16)oam_atoi(V3);
	payload_data_configkpipdcp.duration = (UInt16)oam_atoi(V4);
	oam_memcpy(p_header,&payload_data_configkpipdcp,
			sizeof(payload_data_configkpipdcp));
}

/*******************************************************************************
 * Function Name  : oam_l2_sys_fail_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_l2_sys_fail_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}
/*******************************************************************************
 * Function Name  : oam_mac_l1_inf_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_mac_l1_inf_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_mac_ul_harq_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_mac_ul_harq_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_mac_dl_harq_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_mac_dl_harq_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_mac_rach_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_mac_rach_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_mac_rrc_inf_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_mac_rrc_inf_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_mac_oam_inf_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_mac_oam_inf_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_mac_dl_pc_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_mac_dl_pc_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_mac_ul_pc_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_mac_ul_pc_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_mac_fatallog_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_mac_fatallog_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_mac_errorlog_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_mac_errorlog_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_mac_warninglog_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_mac_warninglog_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_mac_infolog_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_mac_infolog_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_mac_debuglog_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_mac_debuglog_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_pdcp_invalidlog_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_pdcp_invalidlog_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_pdcp_fatallog_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_pdcp_fatallog_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_pdcp_errorlog_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_pdcp_errorlog_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_pdcp_warninglog_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_pdcp_warninglog_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}


/*******************************************************************************
 * Function Name  : oam_pdcp_infolog_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_pdcp_infolog_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_pdcp_debuglog_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_pdcp_debuglog_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_rlc_disabled_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_rlc_disabled_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_rlc_fatallog_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_rlc_fatallog_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_rlc_errorlog_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_rlc_errorlog_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_rlc_warninglog_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_rlc_warninglog_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_rlc_infolog_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_rlc_infolog_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_rlc_debuglog_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_rlc_debuglog_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_rrm_errorlog_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_rrm_errorlog_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_rrm_warninglog_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_rrm_warninglog_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_rrm_brieflog_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_rrm_brieflog_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_rrm_detailedlog_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_rrm_detailedlog_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

#ifdef OAM_SON_ENABLE
/*******************************************************************************
 * Function Name  : oam_son_errorlog_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_son_errorlog_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_son_warninglog_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_son_warninglog_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_son_brieflog_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_son_brieflog_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_son_detailedlog_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_son_detailedlog_all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}
#endif

/*******************************************************************************
 * Function Name  : oam_mac_fatallog_l2all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_mac_fatallog_l2all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_mac_errorlog_l2all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_mac_errorlog_l2all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_mac_warninglog_l2all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_mac_warninglog_l2all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_mac_infolog_l2all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_mac_infolog_l2all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_mac_debuglog_l2all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_mac_debuglog_l2all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}

/*******************************************************************************
 * Function Name  : oam_pdcp_invalidlog_l2all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_pdcp_invalidlog_l2all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}
/*******************************************************************************
 * Function Name  : oam_pdcp_fatallog_l2all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_pdcp_fatallog_l2all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}
/*******************************************************************************
 * Function Name  : oam_pdcp_errorlog_l2all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_pdcp_errorlog_l2all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}
/*******************************************************************************
 * Function Name  : oam_pdcp_warninglog_l2all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_pdcp_warninglog_l2all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}
/*******************************************************************************
 * Function Name  : oam_pdcp_infolog_l2all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_pdcp_infolog_l2all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}
/*******************************************************************************
 * Function Name  : oam_pdcp_debuglog_l2all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_pdcp_debuglog_l2all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}
/*******************************************************************************
 * Function Name  : oam_rlc_disabled_l2all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_rlc_disabled_l2all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}
/*******************************************************************************
 * Function Name  : oam_rlc_fatallog_l2all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_rlc_fatallog_l2all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}
/*******************************************************************************
 * Function Name  : oam_rlc_errorlog_l2all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_rlc_errorlog_l2all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}
/*******************************************************************************
 * Function Name  : oam_rlc_warninglog_l2all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_rlc_warninglog_l2all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}
/*******************************************************************************
 * Function Name  : oam_rlc_infolog_l2all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_rlc_infolog_l2all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}
/*******************************************************************************
 * Function Name  : oam_rlc_debuglog_l2all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_rlc_debuglog_l2all_payload
(
 UInt8 *p_header,
 UInt8 val
 )
{
	oam_memcpy(p_header,&val,sizeof(val));
}
/*******************************************************************************
 * Function Name  : oam_mac_log_level_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *          		log_level_param : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_mac_log_level_payload
(
 UInt8 *p_header,
 setloglevell2maccommands mac_log_level_param
 )
{

	UInt32 x = LTE_L2_LOG_CRITICAL;

	if(MAC_CRITICALLOG == mac_log_level_param)
	{
		x = LTE_L2_LOG_CRITICAL;
	}
	else if(MAC_ERRORLOG == mac_log_level_param)
	{
		x = LTE_L2_LOG_ERROR;
	}
	else if(MAC_WARNINGLOG == mac_log_level_param)
	{
		x = LTE_L2_LOG_WARNING;
	}
	else if(MAC_INFOLOG == mac_log_level_param)
	{
		x = LTE_L2_LOG_INFO;
	}
	else if(MAC_BRIEFLOG == mac_log_level_param)
	{
		x = LTE_L2_LOG_BRIEF;
	}
	else if(MAC_DETAILEDLOG == mac_log_level_param)
	{
		x = LTE_L2_LOG_DETAILED;
	}
	else if(MAC_DETAILEDTTILOG == mac_log_level_param)
	{
		x = LTE_L2_LOG_DETAILEDALL;
	}
	oam_memcpy(p_header,&x,sizeof(UInt32));
}
/*******************************************************************************
 * Function Name  : oam_rlc_log_level_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *          		log_level_param : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_rlc_log_level_payload
(
 UInt8 *p_header,
 setloglevell2rlccommands log_level_param
 )
{

	UInt32 x = LTE_L2_LOG_CRITICAL;

	if(RLC_CRITICALLOG == log_level_param)
	{
		x = LTE_L2_LOG_CRITICAL;
	}
	else if(RLC_ERRORLOG == log_level_param)
	{
		x = LTE_L2_LOG_ERROR;
	}
	else if(RLC_WARNINGLOG == log_level_param)
	{
		x = LTE_L2_LOG_WARNING;
	}
	else if(RLC_INFOLOG == log_level_param)
	{
		x = LTE_L2_LOG_INFO;
	}
	else if(RLC_BRIEFLOG == log_level_param)
	{
		x = LTE_L2_LOG_BRIEF;
	}
	else if(RLC_DETAILEDLOG == log_level_param)
	{
		x = LTE_L2_LOG_DETAILED;
	}
	else if(RLC_DETAILEDALLLOG == log_level_param)
	{
		x = LTE_L2_LOG_DETAILEDALL;
	}
	oam_memcpy(p_header,&x,sizeof(UInt32));
}
/*******************************************************************************
 * Function Name  : oam_pdcp_log_level_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *          		log_level_param : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_pdcp_log_level_payload
(
 UInt8 *p_header,
 setloglevell2pdcpcommands log_level_param
 )
{

	UInt32 x = LTE_L2_LOG_CRITICAL;

	if(PDCP_CRITICALLOG == log_level_param)
	{
		x = LTE_L2_LOG_CRITICAL;
	}
	else if(PDCP_ERRORLOG == log_level_param)
	{
		x = LTE_L2_LOG_ERROR;
	}
	else if(PDCP_WARNINGLOG == log_level_param)
	{
		x = LTE_L2_LOG_WARNING;
	}
	else if(PDCP_INFOLOG == log_level_param)
	{
		x = LTE_L2_LOG_INFO;
	}
	else if(PDCP_BRIEFLOG == log_level_param)
	{
		x = LTE_L2_LOG_BRIEF;
	}
	else if(PDCP_DETAILEDLOG == log_level_param)
	{
		x = LTE_L2_LOG_DETAILED;
	}
	else if(PDCP_DETAILEDALLLOG == log_level_param)
	{
		x = LTE_L2_LOG_DETAILEDALL;
	}
	oam_memcpy(p_header,&x,sizeof(UInt32));
}
/*******************************************************************************
 * Function Name  : oam_egtpu_log_level_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *          		log_level_param : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_egtpu_log_level_payload
(
 UInt8 *p_header,
 setloglevell2gtpucommands log_level_param
 )
{

	UInt32 x = LTE_L2_LOG_CRITICAL;

	if(GTPU_CRITICALLOG == log_level_param)
	{
		x = LTE_L2_LOG_CRITICAL;
	}
	else if(GTPU_ERRORLOG == log_level_param)
	{
		x = LTE_L2_LOG_ERROR;
	}
	else if(GTPU_WARNINGLOG == log_level_param)
	{
		x = LTE_L2_LOG_WARNING;
	}
	else if(GTPU_INFOLOG == log_level_param)
	{
		x = LTE_L2_LOG_INFO;
	}
	else if(GTPU_BRIEFLOG == log_level_param)
	{
		x = LTE_L2_LOG_BRIEF;
	}
	else if(GTPU_DETAILEDLOG == log_level_param)
	{
		x = LTE_L2_LOG_DETAILED;
	}
	else if(GTPU_DETAILEDALLLOG == log_level_param)
	{
		x = LTE_L2_LOG_DETAILEDALL;
	}
	oam_memcpy(p_header,&x,sizeof(UInt32));
}



void
oam_log_level_payload
(
 UInt8 *p_header,
 UInt32 oam_param_val
 )
{
	oam_memcpy(p_header,&oam_param_val,sizeof(UInt32));
}


/*******************************************************************************
 * Function Name  : oam_l2_log_level_all_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   val : Enum function value passed for subcommands
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_l2_log_level_all_payload
(
 UInt8 *p_header,
 oam_sys_log_level_et  sys_log_level_param
 )
{
	UInt32 x = 0x0000;

	if(SYS_CRITICAL == sys_log_level_param)
	{
		x = 0x0000;
	}
	else if(SYS_ERROR == sys_log_level_param)
	{
		x = 0x0001;
	}
	else if(SYS_WARNING == sys_log_level_param)
	{
		x = 0x0002;
	}
	else if(SYS_INFO == sys_log_level_param)
	{
		x = 0x0003;
	}
	else if(SYS_BRIEF == sys_log_level_param)
	{
		x = 0x0004;
	}
	else if(SYS_DETAILED == sys_log_level_param)
	{
		x = 0x0005;
	}
	else if(SYS_DETAILEDALL == sys_log_level_param)
	{
		x = 0x0006;
	}
	oam_memcpy(p_header,&x,sizeof(UInt32));
}
/*******************************************************************************
 * Function Name  : oam_L2All_log_level_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                   V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_All_log_level_pkt
(
 UInt16 C,
 oam_sys_log_level_et rlc_log_val,
 UInt16 api_id
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length =  OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_show_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(UInt32);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_show_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_show_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			api_id,msg_api_length);

	/* Fill QCLI header */
	p_show_msg = p_show_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_show_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			api_id,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_show_msg = p_show_msg + QCLI_HEADER_SIZE;
	oam_l2_log_level_all_payload(p_show_msg,rlc_log_val);

	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_show_kpi_mac_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   V1 : Pointer to character strings
 *		   V2 : Pointer to character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_show_kpi_mac_payload
(
 UInt8 *p_header,
 char *V1,
 char *V2
 )
{
	oam_show_kpi_mac_t 	payload_data_showkpimac;
	payload_data_showkpimac.kpi_type = (UInt16)oam_atoi(V1);
	kpiToGetBitMap = payload_data_showkpimac.kpi_type;
	payload_data_showkpimac.reset = (UInt8)oam_atoi(V2);
	oam_memcpy(p_header,&payload_data_showkpimac,
			sizeof(payload_data_showkpimac));
}
/*******************************************************************************
 * Function Name  : oam_show_kpi_pdcp_payload
 * Description    : Payload copied to the buffer allocated.
 * Inputs         : p_header : Pointer to buffer that is allocated for payload
 *		   V1 : Pointer to character strings
 *		   V2 : Pointer to character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
void
oam_show_kpi_pdcp_payload
(
 UInt8 *p_header,
 char *V1,
 char *V2
 )
{
	oam_show_kpi_pdcp_t	payload_data_showkpipdcp;
	payload_data_showkpipdcp.kpi_type = (UInt8)oam_atoi(V1);
	kpiPdcpGetBitMap = payload_data_showkpipdcp.kpi_type;
	payload_data_showkpipdcp.reset = (UInt16)oam_atoi(V2);
	oam_memcpy(p_header,&payload_data_showkpipdcp,
			sizeof(payload_data_showkpipdcp));
}

/*******************************************************************************
 * Function Name  : oam_cell_mgmt_delete_pkt
 * Description    : To delete a particular cell, based on cellid.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_cell_mgmt_delete_pkt
(
 UInt16 C,
 const char **V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */ 

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_cellmgmt_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_cell_mgmt_delete_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_cellmgmt_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_cellmgmt_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			CELLMGMT_DELETE,msg_api_length);
	/* Fill QCLI header */
	p_cellmgmt_msg = p_cellmgmt_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_cellmgmt_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			CELLMGMT_DELETE,msg_api_length);
	/* Fill Payload */
	p_cellmgmt_msg = p_cellmgmt_msg + QCLI_HEADER_SIZE;
	oam_cell_mgmt_delete_payload(p_cellmgmt_msg,(char *)V[1]);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_cellmgmt_dlpc_params_pkt
 * Description    : To reconfigure Downlink Power Control Parameters.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_cellmgmt_dlpc_params_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_cellmgmt_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(DLPC_PARAMS);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_cellmgmt_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_cellmgmt_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			CELLMGMT_RECONFIG,msg_api_length);
	/* Fill QCLI header */
	p_cellmgmt_msg = p_cellmgmt_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_cellmgmt_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			CELLMGMT_RECONFIG,msg_api_length);
	/* Fill payload */
	p_cellmgmt_msg = p_cellmgmt_msg + QCLI_HEADER_SIZE;
	oam_cellmgmt_dlpc_params_payload(p_cellmgmt_msg,DLPC_PARAMS);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;

}
/*******************************************************************************
 * Function Name  : oam_cellmgmt_sync_signals_pkt
 * Description    : To reconfigure Synchronization Signal Control Parameters.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_cellmgmt_sync_signals_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_cellmgmt_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(SYNC_SIGNALS);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);

	p_cellmgmt_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_cellmgmt_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			CELLMGMT_RECONFIG,msg_api_length);
	/* Fill QCLI header */
	p_cellmgmt_msg = p_cellmgmt_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_cellmgmt_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			CELLMGMT_RECONFIG,msg_api_length);

	/* Fill payload */
	p_cellmgmt_msg = p_cellmgmt_msg + QCLI_HEADER_SIZE;
	oam_cellmgmt_sync_signals_payload(p_cellmgmt_msg,SYNC_SIGNALS);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;

}
/*******************************************************************************
 * Function Name  : oam_cellmgmt_mib_info_pkt
 * Description    : To reconfigure mib Info parameters.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_cellmgmt_mib_info_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_cellmgmt_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(MIB_INFO);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_cellmgmt_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_cellmgmt_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			CELLMGMT_RECONFIG,msg_api_length);
	/* Fill QCLI header */
	p_cellmgmt_msg = p_cellmgmt_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_cellmgmt_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			CELLMGMT_RECONFIG,msg_api_length);

	/* Fill payload */
	p_cellmgmt_msg = p_cellmgmt_msg + QCLI_HEADER_SIZE;
	oam_cellmgmt_mib_info_payload(p_cellmgmt_msg,MIB_INFO);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;

}

/*******************************************************************************
 * Function Name  : oam_cellmgmt_sib3_pkt
 * Description    : To reconfigure sibtype3 info parameters.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_cellmgmt_sib3_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_cellmgmt_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(SIB3);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_cellmgmt_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_cellmgmt_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			CELLMGMT_RECONFIG,msg_api_length);
	/* Fill QCLI header */
	p_cellmgmt_msg = p_cellmgmt_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_cellmgmt_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			CELLMGMT_RECONFIG,msg_api_length);

	/* Fill payload */
	p_cellmgmt_msg = p_cellmgmt_msg + QCLI_HEADER_SIZE;
	oam_cellmgmt_sib3_payload(p_cellmgmt_msg,SIB3);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;

}

/*******************************************************************************
 * Function Name  : oam_cellmgmt_sib4_pkt
 * Description    : To reconfigure sibtype4 info parameters.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_cellmgmt_sib4_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_cellmgmt_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(SIB4);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_cellmgmt_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_cellmgmt_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			CELLMGMT_RECONFIG,msg_api_length);
	/* Fill QCLI header */
	p_cellmgmt_msg = p_cellmgmt_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_cellmgmt_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			CELLMGMT_RECONFIG,msg_api_length);

	/* Fill payload */
	p_cellmgmt_msg = p_cellmgmt_msg + QCLI_HEADER_SIZE;
	oam_cellmgmt_sib4_payload(p_cellmgmt_msg,SIB4);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;

}

/*******************************************************************************
 * Function Name  : oam_cellmgmt_sib5_pkt
 * Description    : To reconfigure sibtype5 info parameters.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_cellmgmt_sib5_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_cellmgmt_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(SIB5);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_cellmgmt_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_cellmgmt_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			CELLMGMT_RECONFIG,msg_api_length);
	/* Fill QCLI header */
	p_cellmgmt_msg = p_cellmgmt_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_cellmgmt_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			CELLMGMT_RECONFIG,msg_api_length);

	/* Fill payload */
	p_cellmgmt_msg = p_cellmgmt_msg + QCLI_HEADER_SIZE;
	oam_cellmgmt_sib5_payload(p_cellmgmt_msg,SIB5);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;

}
/*******************************************************************************
 * Function Name  : oam_cellmgmt_setup_pkt
 * Description    : To Setup a particular cell, based on cellid.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_cellmgmt_setup_pkt
(
 UInt16 C,
 const char **V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_cellmgmt_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_cellmgmt_setup_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_cellmgmt_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_cellmgmt_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			CELLMGMT_SETUP,msg_api_length);
	/* Fill QCLI header */
	p_cellmgmt_msg = p_cellmgmt_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_cellmgmt_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			CELLMGMT_SETUP,msg_api_length);

	/* Fill Payload */
	p_cellmgmt_msg = p_cellmgmt_msg + QCLI_HEADER_SIZE;
	oam_cellmgmt_setup_payload(p_cellmgmt_msg,(char *)V[1]);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;

}

/*******************************************************************************
 * Function Name  : oam_cellmgmt_start_pkt
 * Description    : To Start a particular cell, based on cellid.
 * Inputs         : C - Number of parameters passed,
 V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_cellmgmt_start_pkt
(
 UInt16 C,
 const char **V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_cellmgmt_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_cellmgmt_start_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_cellmgmt_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_cellmgmt_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			CELLMGMT_START,msg_api_length);
	/* Fill QCLI header */
	p_cellmgmt_msg = p_cellmgmt_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_cellmgmt_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			CELLMGMT_START,msg_api_length);

	/* Fill Payload */
	p_cellmgmt_msg = p_cellmgmt_msg + QCLI_HEADER_SIZE;
	oam_cellmgmt_start_payload(p_cellmgmt_msg,(char *)V[1]);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;

}


/*******************************************************************************
 * Function Name  : oam_cellmgmt_stop_pkt
 * Description    : To Stop a particular cell, based on cellid.
 * Inputs         : C - Number of parameters passed,
 V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_cellmgmt_stop_pkt
(
 UInt16 C,
 const char **V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_cellmgmt_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_cellmgmt_stop_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_cellmgmt_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_cellmgmt_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			CELLMGMT_STOP,msg_api_length);
	/* Fill QCLI header */
	p_cellmgmt_msg = p_cellmgmt_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_cellmgmt_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			CELLMGMT_STOP,msg_api_length);

	/* Fill Payload */
	p_cellmgmt_msg = p_cellmgmt_msg + QCLI_HEADER_SIZE;
	oam_cellmgmt_stop_payload(p_cellmgmt_msg,(char *)V[1]);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;

}

/*******************************************************************************
 * Function Name  : oam_config_kpi_mac_pkt
 * Description    : To configure the KPIs maintained at MAC.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_config_kpi_mac_pkt
(
 UInt16 C,
 const char **V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_config_kpi_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_config_kpi_mac_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_config_kpi_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_config_kpi_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_CONFIG_KPI_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_config_kpi_msg = p_config_kpi_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_config_kpi_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_CONFIG_KPI_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill Payload */
	p_config_kpi_msg = p_config_kpi_msg + QCLI_HEADER_SIZE;
	oam_config_kpi_mac_payload(p_config_kpi_msg,(char *)V[1],(char *)V[2],(char *)V[3],(char *)V[4]);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;

}


/*******************************************************************************
 * Function Name  : oam_config_kpi_pdcp_pkt
 * Description    : To configure the KPIs maintained at PDCP.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_config_kpi_pdcp_pkt
(
 UInt16 C,
 const char **V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_configkpi_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_config_kpi_pdcp_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_configkpi_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_configkpi_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_CONFIG_KPI_PDCP_REQ,msg_api_length);
	/* Fill QCLI header */
	p_configkpi_msg = p_configkpi_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_configkpi_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_CONFIG_KPI_PDCP_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill Payload */
	p_configkpi_msg = p_configkpi_msg + QCLI_HEADER_SIZE;
	oam_config_kpi_pdcp_payload(p_configkpi_msg,(char *)V[1],(char *)V[2],(char *)V[3],(char *)V[4]);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;

}

/*******************************************************************************
 * Function Name  : oam_stats_all_pkt
 * Description    : To reset Stat request of all the layers.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_stats_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_reset_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_reset_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_reset_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			RESET_STATS_ALL,msg_api_length);

	/* Fill QCLI header */
	p_reset_msg = p_reset_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_reset_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			RESET_STATS_ALL,msg_api_length);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_stats_l2_all_pkt
 * Description    : To reset Stat request of L2 layer.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_stats_l2_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_reset_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_reset_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_reset_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			RESET_STATS_L2_ALL,msg_api_length);

	/* Fill QCLI header */
	p_reset_msg = p_reset_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_reset_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			RESET_STATS_L2_ALL,msg_api_length);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_stats_l2_mac_pkt
 * Description    : To reset Stat request of Mac layer of L2.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_stats_l2_mac_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_reset_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_reset_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_reset_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			RESET_STATS_L2_MAC,msg_api_length);

	/* Fill QCLI header */
	p_reset_msg = p_reset_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_reset_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			RESET_STATS_L2_MAC,msg_api_length);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_stats_l2_pdcp_pkt
 * Description    : To reset Stat request of Pdcp layer of L2.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_stats_l2_pdcp_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_reset_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_reset_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_reset_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			RESET_STATS_L2_PDCP,msg_api_length);

	/* Fill QCLI header */
	p_reset_msg = p_reset_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_reset_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			RESET_STATS_L2_PDCP,msg_api_length);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_stats_l2_rlc_pkt
 * Description    : To reset Stat request of Rlc layer of L2.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_stats_l2_rlc_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_reset_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_reset_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_reset_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			RESET_STATS_L2_RLC,msg_api_length);

	/* Fill QCLI header */
	p_reset_msg = p_reset_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_reset_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			RESET_STATS_L2_RLC,msg_api_length);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_stats_l3_all_pkt
 * Description    : To reset Stat request of L3 layer.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_stats_l3_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_reset_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_reset_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_reset_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			RESET_STATS_L3_ALL,msg_api_length);

	/* Fill QCLI header */
	p_reset_msg = p_reset_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_reset_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			RESET_STATS_L3_ALL,msg_api_length);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_stats_l3_rrc_pkt
 * Description    : To reset Stat request of RRC of L3 layer.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_stats_l3_rrc_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_reset_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_reset_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_reset_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			RESET_STATS_L3_RRC,msg_api_length);

	/* Fill QCLI header */
	p_reset_msg = p_reset_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_reset_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			RESET_STATS_L3_RRC,msg_api_length);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_stats_l3_s1ap_pkt
 * Description    : To reset Stat request of S1AP of L3 layer.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_stats_l3_s1ap_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_reset_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_reset_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_reset_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			RESET_STATS_L3_S1AP,msg_api_length);

	/* Fill QCLI header */
	p_reset_msg = p_reset_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_reset_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			RESET_STATS_L3_S1AP,msg_api_length);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_set_log_cat_l2_mac_all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_set_log_cat_l2_mac_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat = {OAM_ZERO};
	oam_mac_enable_log_cat.logCategory |= (L2_SYS_FAIL|MAC_L1_INF|MAC_UL_HARQ|MAC_RACH|MAC_RRC_INF|MAC_OAM_INF|MAC_DL_HARQ|MAC_DL_PC|MAC_UL_PC|MAC_DL_Strategy|MAC_UL_Strategy|MAC_PDSCH|MAC_PDCCH|MAC_PUSCH|MAC_PUCCH|MAC_BCCH|MAC_PCCH|MAC_CCCH|MAC_CQI|MAC_SRS|MAC_TB_UL|MAC_TB_DL|MAC_DRX|MAC_MEAS_GAP|MAC_MEAS_HANDLER|MAC_UL_CSI|MAC_MUX|MAC_DL_SPS_STRATEGY|MAC_UL_SPS_STRATEGY|MAC_SCHEDULER|MAC_ATB|MAC_TA );
	
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_l2_sys_fail_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_l2_sys_fail_pkt
(
 UInt16 C,
 const char **V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat = {OAM_ZERO};
	oam_mac_enable_log_cat.logCategory |= L2_SYS_FAIL;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_mac_l1_inf_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_l1_inf_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat;
	oam_mac_enable_log_cat.logCategory = MAC_L1_INF;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_ul_harq_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_ul_harq_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat;
	oam_mac_enable_log_cat.logCategory = MAC_UL_HARQ;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_dl_harq_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE

 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_dl_harq_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat;
	oam_mac_enable_log_cat.logCategory = MAC_DL_HARQ;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_rach_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_rach_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat;
	oam_mac_enable_log_cat.logCategory = MAC_RACH;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_rrc_inf_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_rrc_inf_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat;
	oam_mac_enable_log_cat.logCategory = MAC_RRC_INF;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_oam_inf_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_oam_inf_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat;
	oam_mac_enable_log_cat.logCategory = MAC_OAM_INF;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*CSR:00055317_CHANGE_START*/
/*******************************************************************************
 * Function Name  : oam_mac_rrm_inf_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_rrm_inf_pkt
(
 UInt16 C,
 const char* V
 )
{
        /* SPR 17777 FIX START */
        qcliHandleUnusedParameter(C);
        qcliHandleUnusedParameter(V);
       /* SPR 17777 FIX END */

        UInt16 msg_length = OAM_ZERO;
        UInt16 msg_api_length =  OAM_ZERO;
        UInt16 new_trans_id = OAM_ZERO;

        UInt8 *p_msg, *p_set_msg;
        new_trans_id = ++qcli_trans_id;
        msg_length = sizeof(oam_mac_enable_log_category_req_t);
        msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
        p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

        if(OAM_NULL == p_msg)
        {
                /* Not enough memory */
                return OAM_FAILURE;
        }
        oam_memset(p_msg, OAM_ZERO, msg_api_length);
        p_set_msg = p_msg;
        /* Fill CSPL header */
        oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
                        OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

        /* Fill QCLI header */
        p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
        oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
                        OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

        /* Fill payload */
        p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
        oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat;
    oam_mac_enable_log_cat.logCategory = MAC_RRM_INF;
        oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
        oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
        return OAM_SUCCESS;
}
/*CSR:00055317_CHANGE_END*/

/*******************************************************************************
 * Function Name  : oam_mac_dl_pc_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_dl_pc_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat;
	oam_mac_enable_log_cat.logCategory = MAC_DL_PC;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_ul_pc_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_ul_pc_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat;
	oam_mac_enable_log_cat.logCategory = MAC_UL_PC;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/* 55317 Start */
/*******************************************************************************
 * Function Name  : oam_mac_dl_strategy_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_dl_strategy_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat;
	oam_mac_enable_log_cat.logCategory = MAC_DL_Strategy;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_ul_strategy_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_ul_strategy_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat;
	oam_mac_enable_log_cat.logCategory = MAC_UL_Strategy;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_pdsch_pkt 
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_pdsch_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat;
	oam_mac_enable_log_cat.logCategory = MAC_PDSCH;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_pdcch_pkt 
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_pdcch_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat;
	oam_mac_enable_log_cat.logCategory = MAC_PDCCH;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_pusch_pkt 
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_pusch_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat;
	oam_mac_enable_log_cat.logCategory = MAC_PUSCH;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_pucch_pkt 
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_pucch_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat;
	oam_mac_enable_log_cat.logCategory = MAC_PUCCH;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_bcch_pkt 
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_bcch_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat;
	oam_mac_enable_log_cat.logCategory = MAC_BCCH;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_pcch_pkt 
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_pcch_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(V);
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat;
	oam_mac_enable_log_cat.logCategory = MAC_PCCH;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_ccch_pkt 
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_ccch_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat;
	oam_mac_enable_log_cat.logCategory = MAC_CCCH;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_cqi_pkt 
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_cqi_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat;
	oam_mac_enable_log_cat.logCategory = MAC_CQI;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_srs_pkt 
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_srs_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat;
	oam_mac_enable_log_cat.logCategory = MAC_SRS;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_tb_ul_pkt 
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_tb_ul_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat;
	oam_mac_enable_log_cat.logCategory = MAC_TB_UL;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_tb_dl_pkt 
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_tb_dl_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat;
	oam_mac_enable_log_cat.logCategory = MAC_TB_DL;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_drx_pkt 
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_drx_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat;
	oam_mac_enable_log_cat.logCategory = MAC_DRX;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_meas_gap_pkt 
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_meas_gap_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat;
	oam_mac_enable_log_cat.logCategory = MAC_MEAS_GAP;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_meas_handler_pkt 
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_meas_handler_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat;
	oam_mac_enable_log_cat.logCategory = MAC_MEAS_HANDLER;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_ul_csi_pkt 
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_ul_csi_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat;
	oam_mac_enable_log_cat.logCategory = MAC_UL_CSI;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_mux_pkt 
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_mux_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat;
	oam_mac_enable_log_cat.logCategory = MAC_MUX;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_dl_sps_strategy_pkt 
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_dl_sps_strategy_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat = {OAM_ZERO};
	oam_mac_enable_log_cat.logCategory |= MAC_DL_SPS_STRATEGY ;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_ul_sps_strategy_pkt 
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_ul_sps_strategy_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat = {OAM_ZERO};
	oam_mac_enable_log_cat.logCategory |= MAC_UL_SPS_STRATEGY ;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_scheduler_pkt 
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_scheduler_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat = {OAM_ZERO};
	oam_mac_enable_log_cat.logCategory |= MAC_SCHEDULER ;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_atb_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_atb_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat = {OAM_ZERO};
	oam_mac_enable_log_cat.logCategory |= MAC_ATB ;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_ta_pkt 
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_ta_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_enable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_enable_log_category_req_t  oam_mac_enable_log_cat = {OAM_ZERO};
	oam_mac_enable_log_cat.logCategory |= MAC_TA ;
	oam_memcpy(p_set_msg,&oam_mac_enable_log_cat,sizeof(oam_mac_enable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_rlc_log_cat_enable_all_pkt 
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rlc_log_cat_enable_all_pkt
(
 UInt16 C,
 const char** V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 msg_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_RLC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_RLC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	Oam_LogCategory_Req_t rlc_log_enable_req = {OAM_ZERO};
	rlc_log_enable_req.logCategory |= (RLC_AM |RLC_UM | RLC_TM |RLC_TX | RLC_RX |RLC_MAC | RLC_PDCP | RLC_RRC | RLC_OAM |RLC_TIMER );
	oam_memcpy(p_set_msg,&rlc_log_enable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_rlc_am_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rlc_am_pkt
(
 UInt16 C,
 const char** V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 msg_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_RLC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_RLC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	Oam_LogCategory_Req_t rlc_log_enable_req = {OAM_ZERO};
	rlc_log_enable_req.logCategory |= RLC_AM;
	oam_memcpy(p_set_msg,&rlc_log_enable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_rlc_um_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rlc_um_pkt
(
 UInt16 C,
 const char** V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 msg_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_RLC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_RLC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	Oam_LogCategory_Req_t rlc_log_enable_req = {OAM_ZERO};
	rlc_log_enable_req.logCategory |= RLC_UM;
	oam_memcpy(p_set_msg,&rlc_log_enable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_rlc_tm_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rlc_tm_pkt
(
 UInt16 C,
 const char** V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 msg_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_RLC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_RLC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	Oam_LogCategory_Req_t rlc_log_enable_req = {OAM_ZERO};
	rlc_log_enable_req.logCategory |= RLC_TM;
	oam_memcpy(p_set_msg,&rlc_log_enable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_rlc_tx_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rlc_tx_pkt
(
 UInt16 C,
 const char** V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 msg_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_RLC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_RLC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	Oam_LogCategory_Req_t rlc_log_enable_req = {OAM_ZERO};
	rlc_log_enable_req.logCategory |= RLC_TX;
	oam_memcpy(p_set_msg,&rlc_log_enable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_rlc_rx_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rlc_rx_pkt
(
 UInt16 C,
 const char** V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 msg_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_RLC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_RLC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	Oam_LogCategory_Req_t rlc_log_enable_req = {OAM_ZERO};
	rlc_log_enable_req.logCategory |= RLC_RX;
	oam_memcpy(p_set_msg,&rlc_log_enable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_rlc_mac_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rlc_mac_pkt
(
 UInt16 C,
 const char** V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 msg_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_RLC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_RLC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	Oam_LogCategory_Req_t rlc_log_enable_req = {OAM_ZERO};
	rlc_log_enable_req.logCategory |= RLC_MAC;
	oam_memcpy(p_set_msg,&rlc_log_enable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_rlc_pdcp_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rlc_pdcp_pkt
(
 UInt16 C,
 const char** V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 msg_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_RLC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_RLC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	Oam_LogCategory_Req_t rlc_log_enable_req = {OAM_ZERO};
	rlc_log_enable_req.logCategory |= RLC_PDCP;
	oam_memcpy(p_set_msg,&rlc_log_enable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_rlc_rrc_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rlc_rrc_pkt
(
 UInt16 C,
 const char** V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 msg_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_RLC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_RLC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	Oam_LogCategory_Req_t rlc_log_enable_req = {OAM_ZERO};
	rlc_log_enable_req.logCategory |= RLC_RRC;
	oam_memcpy(p_set_msg,&rlc_log_enable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_rlc_oam_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rlc_oam_pkt
(
 UInt16 C,
 const char** V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 msg_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_RLC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_RLC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	Oam_LogCategory_Req_t rlc_log_enable_req = {OAM_ZERO};
	rlc_log_enable_req.logCategory |= RLC_OAM;
	oam_memcpy(p_set_msg,&rlc_log_enable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_rlc_timer_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rlc_timer_pkt
(
 UInt16 C,
 const char** V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 msg_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_RLC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_RLC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	Oam_LogCategory_Req_t rlc_log_enable_req = {OAM_ZERO};
	rlc_log_enable_req.logCategory |= RLC_TIMER;
	oam_memcpy(p_set_msg,&rlc_log_enable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/* 55317 End */



/*******************************************************************************
 * Function Name  : oam_pdcp_enable_log_cat_all_pkt 
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_pdcp_enable_log_cat_all_pkt
(
 UInt16 C,
 const char **V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt16 msg_length = OAM_ZERO;
	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length = msg_length+ QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_PDCP_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_PDCP_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	Oam_LogCategory_Req_t pdcp_log_enable_req = {OAM_ZERO};
	pdcp_log_enable_req.logCategory |= (PDCP_RRCOAM|PDCP_TX|PDCP_RX|PDCP_RRM_INT);
	oam_memcpy(p_set_msg,&pdcp_log_enable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_pdcp_rrcoam_pkt 
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_pdcp_rrcoam_pkt
(
 UInt16 C,
 const char **V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt16 msg_length = OAM_ZERO;
	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length = msg_length+ QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_PDCP_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_PDCP_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	Oam_LogCategory_Req_t pdcp_log_enable_req = {OAM_ZERO};
	pdcp_log_enable_req.logCategory |= PDCP_RRCOAM;
	oam_memcpy(p_set_msg,&pdcp_log_enable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_pdcp_tx_pkt 
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_pdcp_tx_pkt
(
 UInt16 C,
 const char **V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt16 msg_length = OAM_ZERO;
	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length = msg_length+ QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_PDCP_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_PDCP_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	Oam_LogCategory_Req_t pdcp_log_enable_req = {OAM_ZERO};
	pdcp_log_enable_req.logCategory |= PDCP_TX;
	oam_memcpy(p_set_msg,&pdcp_log_enable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}


/*******************************************************************************
 * Function Name  : oam_pdcp_rx_pkt 
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_pdcp_rx_pkt
(
 UInt16 C,
 const char **V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt16 msg_length = OAM_ZERO;
	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length = msg_length+ QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_PDCP_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_PDCP_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	Oam_LogCategory_Req_t pdcp_log_enable_req = {OAM_ZERO};
	pdcp_log_enable_req.logCategory |= PDCP_RX;
	oam_memcpy(p_set_msg,&pdcp_log_enable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_pdcp_rrm_int_pkt 
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_pdcp_rrm_int_pkt
(
 UInt16 C,
 const char **V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt16 msg_length = OAM_ZERO;
	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length = msg_length+ QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_PDCP_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_PDCP_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	Oam_LogCategory_Req_t pdcp_log_enable_req = {OAM_ZERO};
	pdcp_log_enable_req.logCategory |= PDCP_RRM_INT;
	oam_memcpy(p_set_msg,&pdcp_log_enable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/* SPR 55317 END */


/*******************************************************************************
 * Function Name  : oam_set_log_cat_l2_egtpu_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_set_log_cat_l2_egtpu_pkt
(
 UInt16 C,
 const char** V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  sizeof(UInt64) + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_EGTPU_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_LOG_CAT_EGTPU_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	UInt64  pdcp_log_enable_req;
	pdcp_log_enable_req = (UInt64)(oam_atoi(V[1]));
	oam_memcpy(p_set_msg,&pdcp_log_enable_req,sizeof(UInt64));

	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_fatallog_all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_fatallog_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(MAC_FATALLOG_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_fatallog_all_payload(p_set_msg,MAC_FATALLOG_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_errorlog_all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_errorlog_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(MAC_ERRORLOG_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_errorlog_all_payload(p_set_msg,MAC_ERRORLOG_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_warninglog_all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_warninglog_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(MAC_WARNINGLOG_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_warninglog_all_payload(p_set_msg,MAC_WARNINGLOG_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_infolog_all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_infolog_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(MAC_INFOLOG_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_infolog_all_payload(p_set_msg,MAC_INFOLOG_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_debuglog_all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_debuglog_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(MAC_DEBUGLOG_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_debuglog_all_payload(p_set_msg,MAC_DEBUGLOG_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_pdcp_invalidlog_all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_pdcp_invalidlog_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(PDCP_INVALIDLOG_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_pdcp_invalidlog_all_payload(p_set_msg,PDCP_INVALIDLOG_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_pdcp_fatallog_all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_pdcp_fatallog_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(PDCP_FATALLOG_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_pdcp_fatallog_all_payload(p_set_msg,PDCP_FATALLOG_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_pdcp_errorlog_all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_pdcp_errorlog_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(PDCP_ERRORLOG_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_pdcp_errorlog_all_payload(p_set_msg,PDCP_ERRORLOG_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_pdcp_warninglog_all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_pdcp_warninglog_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(PDCP_WARNINGLOG_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_pdcp_warninglog_all_payload(p_set_msg,PDCP_WARNINGLOG_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_pdcp_infolog_all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_pdcp_infolog_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(PDCP_INFOLOG_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_pdcp_infolog_all_payload(p_set_msg,PDCP_INFOLOG_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_pdcp_debuglog_all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_pdcp_debuglog_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(PDCP_DEBUGLOG_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_pdcp_debuglog_all_payload(p_set_msg,PDCP_DEBUGLOG_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_rlc_disabled_all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rlc_disabled_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(RLC_DISABLED_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_rlc_disabled_all_payload(p_set_msg,RLC_DISABLED_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_rlc_fatallog_all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rlc_fatallog_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(RLC_FATALLOG_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_rlc_fatallog_all_payload(p_set_msg,RLC_FATALLOG_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_rlc_errorlog_all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rlc_errorlog_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(RLC_ERRORLOG_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_rlc_errorlog_all_payload(p_set_msg,RLC_ERRORLOG_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_rlc_warninglog_all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rlc_warninglog_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(RLC_WARNINGLOG_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_rlc_warninglog_all_payload(p_set_msg,RLC_WARNINGLOG_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_rlc_infolog_all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rlc_infolog_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(RLC_INFOLOG_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_rlc_infolog_all_payload(p_set_msg,RLC_INFOLOG_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_rlc_debuglog_all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rlc_debuglog_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(RLC_DEBUGLOG_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_rlc_debuglog_all_payload(p_set_msg,RLC_DEBUGLOG_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_rrm_errorlog_all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rrm_errorlog_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(RRM_ERRORLOG_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_rrm_errorlog_all_payload(p_set_msg,RRM_ERRORLOG_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_rrm_warninglog_all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rrm_warninglog_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(RRM_WARNINGLOG_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_rrm_warninglog_all_payload(p_set_msg,RRM_WARNINGLOG_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_rrm_brieflog_all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rrm_brieflog_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(RRM_BRIEFLOG_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_rrm_brieflog_all_payload(p_set_msg,RRM_BRIEFLOG_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_rrm_detailedlog_all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rrm_detailedlog_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(RRM_DETAILEDLOG_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_rrm_detailedlog_all_payload(p_set_msg,RRM_DETAILEDLOG_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_son_errorlog_all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
#ifdef OAM_SON_ENABLE
oam_return_et
oam_son_errorlog_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(SON_ERRORLOG_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_son_errorlog_all_payload(p_set_msg,SON_ERRORLOG_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_son_warninglog_all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_son_warninglog_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(SON_WARNINGLOG_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_son_warninglog_all_payload(p_set_msg,SON_WARNINGLOG_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_son_brieflog_all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_son_brieflog_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(SON_BRIEFLOG_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_son_brieflog_all_payload(p_set_msg,SON_BRIEFLOG_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_son_detailedlog_all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_son_detailedlog_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(SON_DETAILEDLOG_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_son_detailedlog_all_payload(p_set_msg,SON_DETAILEDLOG_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
#endif
/*******************************************************************************
 * Function Name  : oam_mac_fatallog_l2all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_fatallog_l2all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(MAC_FATALLOG_L2ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_fatallog_l2all_payload(p_set_msg,MAC_FATALLOG_L2ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_errorlog_l2all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_errorlog_l2all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(MAC_ERRORLOG_L2ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_errorlog_l2all_payload(p_set_msg,MAC_ERRORLOG_L2ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_warninglog_l2all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_warninglog_l2all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(MAC_WARNINGLOG_L2ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_warninglog_l2all_payload(p_set_msg,MAC_WARNINGLOG_L2ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_mac_infolog_l2all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_infolog_l2all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(MAC_INFOLOG_L2ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_infolog_l2all_payload(p_set_msg,MAC_INFOLOG_L2ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_mac_debuglog_l2all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_debuglog_l2all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(MAC_DEBUGLOG_L2ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_debuglog_l2all_payload(p_set_msg,MAC_DEBUGLOG_L2ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_pdcp_invalidlog_l2all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_pdcp_invalidlog_l2all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(PDCP_INVALIDLOG_L2ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_pdcp_invalidlog_l2all_payload(p_set_msg,PDCP_INVALIDLOG_L2ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_pdcp_fatallog_l2all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_pdcp_fatallog_l2all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(PDCP_FATALLOG_L2ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_pdcp_fatallog_l2all_payload(p_set_msg,PDCP_FATALLOG_L2ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_pdcp_errorlog_l2all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_pdcp_errorlog_l2all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(PDCP_ERRORLOG_L2ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_pdcp_errorlog_l2all_payload(p_set_msg,PDCP_ERRORLOG_L2ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_pdcp_warninglog_l2all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_pdcp_warninglog_l2all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(PDCP_WARNINGLOG_L2ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_pdcp_warninglog_l2all_payload(p_set_msg,PDCP_WARNINGLOG_L2ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_pdcp_infolog_l2all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_pdcp_infolog_l2all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(PDCP_INFOLOG_L2ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_pdcp_infolog_l2all_payload(p_set_msg,PDCP_INFOLOG_L2ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_pdcp_debuglog_l2all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_pdcp_debuglog_l2all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(PDCP_DEBUGLOG_L2ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_pdcp_debuglog_l2all_payload(p_set_msg,PDCP_DEBUGLOG_L2ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_rlc_disabled_l2all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rlc_disabled_l2all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(RLC_DISABLED_L2ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_rlc_disabled_l2all_payload(p_set_msg,RLC_DISABLED_L2ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_rlc_fatallog_l2all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rlc_fatallog_l2all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(RLC_FATALLOG_L2ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_rlc_fatallog_l2all_payload(p_set_msg,RLC_FATALLOG_L2ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_rlc_errorlog_l2all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rlc_errorlog_l2all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(RLC_ERRORLOG_L2ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_rlc_errorlog_l2all_payload(p_set_msg,RLC_ERRORLOG_L2ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_rlc_warninglog_l2all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rlc_warninglog_l2all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(RLC_WARNINGLOG_L2ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_rlc_warninglog_l2all_payload(p_set_msg,RLC_WARNINGLOG_L2ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_rlc_infolog_l2all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rlc_infolog_l2all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(RLC_INFOLOG_L2ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_rlc_infolog_l2all_payload(p_set_msg,RLC_INFOLOG_L2ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_rlc_debuglog_l2all_pkt
 * Description    : To set the desired log category.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rlc_debuglog_l2all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(RLC_DEBUGLOG_L2ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_rlc_debuglog_l2all_payload(p_set_msg,RLC_DEBUGLOG_L2ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_mac_log_level_pkt
 * Description    : To set the desired log level.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_mac_log_level_pkt
(
 UInt16 C,
 setloglevell2maccommands mac_log_val
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(UInt32);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_LEVEL_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_LEVEL_MAC_REQ,msg_api_length -OAM_CSPL_HDR_SIZE );

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_log_level_payload(p_set_msg,mac_log_val);
	wait_gb = OAM_ZERO;
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}


/*******************************************************************************
 * Function Name  : oam_rlc_log_level_pkt
 * Description    : To set the desired log level.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rlc_log_level_pkt
(
 UInt16 C,
 setloglevell2rlccommands rlc_log_val
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(UInt32);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_LEVEL_RLC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_LEVEL_RLC_REQ,msg_api_length -OAM_CSPL_HDR_SIZE );

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_rlc_log_level_payload(p_set_msg, rlc_log_val);
	wait_gb = OAM_ZERO;
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}


/*******************************************************************************
 * Function Name  : oam_pdcp_log_level_pkt
 * Description    : To set the desired log level.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_pdcp_log_level_pkt
(
 UInt16 C,
 setloglevell2pdcpcommands pdcp_log_val
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(UInt32);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_LEVEL_PDCP_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_LEVEL_PDCP_REQ,msg_api_length -OAM_CSPL_HDR_SIZE );

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_pdcp_log_level_payload(p_set_msg,pdcp_log_val);
	wait_gb = OAM_ZERO;
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}


/*******************************************************************************
 * Function Name  : oam_egtpu_log_level_pkt
 * Description    : To set the desired log level.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_egtpu_log_level_pkt
(
 UInt16 C,
 setloglevell2gtpucommands gtpu_log_val
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(UInt32);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_LEVEL_GTPU_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_LEVEL_GTPU_REQ,msg_api_length -OAM_CSPL_HDR_SIZE );

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_egtpu_log_level_payload(p_set_msg,gtpu_log_val);
	wait_gb = OAM_ZERO;
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}


#ifdef OAM_SON_ENABLE
/*******************************************************************************
 * Function Name  : oam_anr_enable_pkt
 * Description    : To Enable ANR Request of SON.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_anr_enable_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_SON_ANR_ENABLE,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_SON_ANR_ENABLE,msg_api_length);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_anr_disable_pkt
 * Description    : To Disable ANR Request of SON.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_anr_disable_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_SON_ANR_DISABLE,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_SON_ANR_DISABLE,msg_api_length);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_es_enable_pkt
 * Description    : To Enable ES Request of SON.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_es_enable_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_SON_ES_ENABLE,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_SON_ES_ENABLE,msg_api_length);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_es_disable_pkt
 * Description    : To Disable ES Request of SON.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_es_disable_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_SON_ES_DISABLE,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_SON_ES_DISABLE,msg_api_length);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_switch_on_pkt
 * Description    : To Enable cell switch on request of ES of SON.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_switch_on_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_SON_ES_CELL_SWITCH_ON,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_SON_ES_CELL_SWITCH_ON,msg_api_length);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_switch_off_pkt
 * Description    : To serve cell switch off request of ES of SON.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_switch_off_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_SON_ES_CELL_SWITCH_OFF,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_SON_ES_CELL_SWITCH_OFF,msg_api_length);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
#endif


/*******************************************************************************
 * Function Name  : oam_show_log_cat_l2_mac_pkt
 * Description    : To serve the request for displaying the cat log of Mac.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_show_log_cat_l2_mac_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_show_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_show_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_show_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_GET_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_show_msg = p_show_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_show_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_GET_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_show_log_cat_l2_pdcp_pkt
 * Description    : To serve the request for displaying the cat log of Pdcp.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_show_log_cat_l2_pdcp_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_show_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_show_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_show_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_GET_LOG_CAT_PDCP_REQ,msg_api_length);

	/* Fill QCLI header */
	p_show_msg = p_show_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_show_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_GET_LOG_CAT_PDCP_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_show_log_cat_l2_rlc_pkt
 * Description    : To serve the request for displaying the cat log of Rlc.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_show_log_cat_l2_rlc_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_show_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_show_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_show_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_GET_LOG_CAT_RLC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_show_msg = p_show_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_show_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_GET_LOG_CAT_RLC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/* SPR_13116_CHANGE_START */
/*SPR 13617 START*/
/*******************************************************************************
 *Function Name  : oam_show_log_level_l3_pkt
 *Description    : To serve the request for displaying the log level of OAM.
 *Inputs         : C - Number of parameters passed,
 *V - Pointer to array of character strings
 *Outputs        : NONE
 *Returns        : OAM_SUCCESS/OAM_FAILURE
 ********************************************************************************/
oam_return_et
oam_show_log_level_l3_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

        UInt16 msg_api_length =  OAM_ZERO;
        UInt16 new_trans_id = OAM_ZERO;


        UInt8 *p_msg, *p_show_msg;
        new_trans_id = ++qcli_trans_id;
        msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
        p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

        if(OAM_NULL == p_msg)
        {
                return OAM_FAILURE;
        }
        oam_memset(p_msg, OAM_ZERO, msg_api_length);
        p_show_msg = p_msg;
         oam_construct_api_header(p_show_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
                        OAM_GET_LOG_LEVEL_L3_REQ,msg_api_length);

        p_show_msg = p_show_msg + OAM_CSPL_HDR_SIZE;
        oam_construct_qcli_header(p_show_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
                        OAM_GET_LOG_LEVEL_L3_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

        oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
        return OAM_SUCCESS;
}

/*SPR 13617 END*/

/*******************************************************************************
 *Function Name  : oam_show_log_level_l3_s1ap_pkt
 *Description    : To serve the request for displaying the log level of OAM.
 *Inputs         : C - Number of parameters passed,
 *V - Pointer to array of character strings
 *Outputs        : NONE
 *Returns        : OAM_SUCCESS/OAM_FAILURE
 ********************************************************************************/
oam_return_et
oam_show_log_level_l3_s1ap_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

        UInt16 msg_api_length =  OAM_ZERO;
        UInt16 new_trans_id = OAM_ZERO;


        UInt8 *p_msg, *p_show_msg;
        new_trans_id = ++qcli_trans_id;
        msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
        p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

        if(OAM_NULL == p_msg)
        {
                return OAM_FAILURE;
        }
        oam_memset(p_msg, OAM_ZERO, msg_api_length);
        p_show_msg = p_msg;
         oam_construct_api_header(p_show_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
                        OAM_GET_LOG_LEVEL_L3_S1AP_REQ,msg_api_length);

        p_show_msg = p_show_msg + OAM_CSPL_HDR_SIZE;
        oam_construct_qcli_header(p_show_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
                        OAM_GET_LOG_LEVEL_L3_S1AP_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

        oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
        return OAM_SUCCESS;
}

/*******************************************************************************
 *Function Name  : oam_show_log_level_l3_x2ap_pkt
 *Description    : To serve the request for displaying the log level of OAM.
 *Inputs         : C - Number of parameters passed,
 *V - Pointer to array of character strings
 *Outputs        : NONE
 *Returns        : OAM_SUCCESS/OAM_FAILURE
 ********************************************************************************/
oam_return_et
oam_show_log_level_l3_x2ap_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

        UInt16 msg_api_length =  OAM_ZERO;
        UInt16 new_trans_id = OAM_ZERO;


        UInt8 *p_msg, *p_show_msg;
        new_trans_id = ++qcli_trans_id;
        msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
        p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

        if(OAM_NULL == p_msg)
        {
                return OAM_FAILURE;
        }
        oam_memset(p_msg, OAM_ZERO, msg_api_length);
        p_show_msg = p_msg;
         oam_construct_api_header(p_show_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
                        OAM_GET_LOG_LEVEL_L3_X2AP_REQ,msg_api_length);

        p_show_msg = p_show_msg + OAM_CSPL_HDR_SIZE;
        oam_construct_qcli_header(p_show_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
                        OAM_GET_LOG_LEVEL_L3_X2AP_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

        oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
        return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_qcli_send_rrm_set_log_level_cmd
 * Description    : To qcli send rrm set log level cmd
 * Inputs         : C - Number of parameters passed,rrm_log_val,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_qcli_send_rrm_get_log_level_cmd
(
 UInt16 C,
 Char8* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

    UInt16 msg_length = OAM_ZERO;
    UInt16 msg_api_length =  OAM_ZERO;
    UInt16 new_trans_id = OAM_ZERO;

    UInt8 *p_msg, *p_set_msg;
    new_trans_id = ++qcli_trans_id;
    msg_length = sizeof(oam_rrm_get_log_level_req_t);
    msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
    p_msg = qvMsgAlloc(OAM_ZERO, OAM_ZERO, OAM_ZERO, msg_api_length);

    if(OAM_NULL == p_msg) {
       oam_fprintf(stderr, "CRITICAL: Memory allocation failure!\n");
        /* Not enough memory */
        return OAM_FAILURE;
    }
    oam_memset(p_msg, OAM_ZERO, msg_api_length);
    p_set_msg = p_msg;
    /* Fill CSPL header */
    oam_construct_api_header(p_set_msg,
            OAM_VERSION,
            QCLI_MODULE_ID,
            QCLI_INTERFACE_MODULE_ID,
            OAM_GET_LOG_LEVEL_RRM_REQ,
            msg_api_length);

    /* Fill QCLI header */
    p_set_msg += OAM_CSPL_HDR_SIZE;
    msg_api_length -= OAM_CSPL_HDR_SIZE;
    oam_construct_qcli_header(p_set_msg,
            new_trans_id,
            QCLI_MODULE_ID,
            QCLI_INTERFACE_MODULE_ID,
            OAM_GET_LOG_LEVEL_RRM_REQ,
            msg_api_length);

    /* Fill payload */
    p_set_msg += QCLI_HEADER_SIZE;
    oam_qcli_intf_send_message(p_msg, QCLI_INTERFACE_MODULE_ID, &error_code);
    return OAM_SUCCESS;
}



/*******************************************************************************
 * Function Name  : oam_show_log_level_oam_pkt
 * Description    : To serve the request for displaying the log level of OAM.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_show_log_level_oam_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

    UInt16 msg_api_length =  OAM_ZERO;
    UInt16 new_trans_id = OAM_ZERO;


    UInt8 *p_msg, *p_show_msg;
    new_trans_id = ++qcli_trans_id;
    msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
    p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

    if(OAM_NULL == p_msg)
    {
        /* Not enough memory */
        return OAM_FAILURE;
    }
    oam_memset(p_msg, OAM_ZERO, msg_api_length);
    p_show_msg = p_msg;
    /* Fill CSPL header */
    oam_construct_api_header(p_show_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
            OAM_GET_LOG_LEVEL_OAM_REQ,msg_api_length);

    /* Fill QCLI header */
    p_show_msg = p_show_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_qcli_header(p_show_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
            OAM_GET_LOG_LEVEL_OAM_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

    oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
    return OAM_SUCCESS;
}

/* SPR_13116_CHANGE_END */




/*******************************************************************************
 * Function Name  : oam_show_log_level_l2_all_pkt
 * Description    : To serve the request for displaying the log level of Mac.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_show_log_level_l2_all_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;


	UInt8 *p_msg, *p_show_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_show_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_show_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_GET_LOG_LEVEL_L2_REQ,msg_api_length);

	/* Fill QCLI header */
	p_show_msg = p_show_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_show_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_GET_LOG_LEVEL_L2_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_show_log_level_l2_mac_pkt
 * Description    : To serve the request for displaying the log level of Mac.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_show_log_level_l2_mac_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;


	UInt8 *p_msg, *p_show_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_show_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_show_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_GET_LOG_LEVEL_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_show_msg = p_show_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_show_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_GET_LOG_LEVEL_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_show_log_level_l2_pdcp_pkt
 * Description    : To serve the request for displaying the level log of pdcp.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_show_log_level_l2_pdcp_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_show_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_show_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_show_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_GET_LOG_LEVEL_PDCP_REQ,msg_api_length);

	/* Fill QCLI header */
	p_show_msg = p_show_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_show_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_GET_LOG_LEVEL_PDCP_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_show_log_level_l2_rlc_pkt
 * Description    : To serve the request for displaying the level log of Rlc.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_show_log_level_l2_rlc_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_show_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_show_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_show_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_GET_LOG_LEVEL_RLC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_show_msg = p_show_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_show_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_GET_LOG_LEVEL_RLC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*SPR 19748 FIXED START*/
/*******************************************************************************
 * Function Name  : oam_show_log_level_l2_gtpu_pkt
 * Description    : To serve the request for displaying the level log of EGTPU.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_show_log_level_l2_gtpu_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_show_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_show_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_show_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_GET_LOG_LEVEL_GTPU_REQ,msg_api_length);

	/* Fill QCLI header */
	p_show_msg = p_show_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_show_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_GET_LOG_LEVEL_GTPU_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*SPR 19748 FIXED END */
/*******************************************************************************
 * Function Name  : oam_show_stats_l2_mac_pkt
 * Description    : To get stats request of Mac L2 layer.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_show_stats_l2_mac_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_show_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_show_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_show_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			SHOW_STATS_L2_MAC,msg_api_length);

	/* Fill QCLI header */
	p_show_msg = p_show_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_show_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			SHOW_STATS_L2_MAC,msg_api_length);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_show_stats_l2_rlc_pkt
 * Description    : To get stats request of Rlc of L2 layer.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_show_stats_l2_rlc_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_show_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_show_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_show_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SHOW_STATS_L2_RLC,msg_api_length);

	/* Fill QCLI header */
	p_show_msg = p_show_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_show_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SHOW_STATS_L2_RLC,msg_api_length);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_show_stats_l2_pdcp_pkt
 * Description    : To get stats request of Pdcp of L2 layer.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_show_stats_l2_pdcp_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_show_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_show_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_show_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			SHOW_STATS_L2_PDCP,msg_api_length);

	/* Fill QCLI header */
	p_show_msg = p_show_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_show_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			SHOW_STATS_L2_PDCP,msg_api_length);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_show_stats_l2_egtpu_pkt
 * Description    : To get stats request of Egtpu of L2 layer.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_show_stats_l2_egtpu_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_show_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_show_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_show_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SHOW_STATS_L2_EGTPU,msg_api_length);

	/* Fill QCLI header */
	p_show_msg = p_show_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_show_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SHOW_STATS_L2_EGTPU,msg_api_length);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}


#ifdef OAM_SON_ENABLE
/*******************************************************************************
 * Function Name  : oam_son_enable_brief_pkt
 * Description    : To enable brief logs of son .
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_son_enable_brief_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_son_log_enable_disable_req_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_DISABLE_SON_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_DISABLE_SON_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_son_enable_brief_payload(p_set_msg,V[1]);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
#endif

/*******************************************************************************
 * Function Name  : oam_rrm_disable_error_pkt
 * Description    : To disable error logs of rrm .
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rrm_disable_error_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_rrm_log_enable_disable_req_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_DISABLE_RRM_LOG_REQ,msg_api_length);
	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_DISABLE_RRM_LOG_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_rrm_disable_error_payload(p_set_msg,V[1]);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}


/*******************************************************************************
 * Function Name  : oam_call_trace_start_pkt
 * Description    : To start call trace of FAP service.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_call_trace_start_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_rrc_cell_traffic_trace_start_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_CALL_TRACE_START,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_CALL_TRACE_START,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_call_trace_start_payload(p_set_msg,V[1],V[2],V[3],V[4],V[5]);/* SPR FIX 7851*/
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_call_trace_stop_pkt
 * Description    : To stop call trace of FAP service.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_call_trace_stop_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_call_trace_stop_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_CALL_TRACE_STOP,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_CALL_TRACE_STOP,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_call_trace_stop_payload(p_set_msg,V[1]);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_gtpu_incoming_all_pkt
 * Description    : To view all incoming logs of gtpu.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_gtpu_incoming_all_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(V);
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(GTPU_INCOMING_TRACE_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_gtpu_incoming_all_payload(p_set_msg,GTPU_INCOMING_TRACE_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_gtpu_outgoing_all_pkt
 * Description    : To view all outgoing logs of gtpu.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_gtpu_outgoing_all_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(GTPU_OUTGOING_TRACE_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_gtpu_outgoing_all_payload(p_set_msg,GTPU_OUTGOING_TRACE_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_gtpu_fsm_all_pkt
 * Description    : To view all fsm logs of gtpu.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_gtpu_fsm_all_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(GTPU_FSM_TRACE_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_gtpu_fsm_all_payload(p_set_msg,GTPU_FSM_TRACE_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_gtpu_error_all_pkt
 * Description    : To view all error logs of gtpu.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_gtpu_error_all_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(GTPU_ERROR_TRACE_ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_gtpu_error_all_payload(p_set_msg,GTPU_ERROR_TRACE_ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_gtpu_incoming_l2all_pkt
 * Description    : To view all incoming logs of gtpu of L2.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_gtpu_incoming_l2all_pkt
(
 UInt16 C,
 char* const * V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(GTPU_INCOMING_TRACE_L2ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_gtpu_incoming_l2all_payload(p_set_msg,GTPU_INCOMING_TRACE_L2ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_gtpu_outgoing_l2all_pkt
 * Description    : To view all outgoing logs of gtpu of L2.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_gtpu_outgoing_l2all_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(GTPU_OUTGOING_TRACE_L2ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_gtpu_outgoing_l2all_payload(p_set_msg,GTPU_OUTGOING_TRACE_L2ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_gtpu_fsm_l2all_pkt
 * Description    : To view all fsm logs of gtpu of L2.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_gtpu_fsm_l2all_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(GTPU_FSM_TRACE_L2ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_gtpu_fsm_l2all_payload(p_set_msg,GTPU_FSM_TRACE_L2ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_gtpu_error_l2all_pkt
 * Description    : To view all error logs of gtpu of L2.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_gtpu_error_l2all_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(GTPU_ERROR_TRACE_L2ALL);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L2_ALL,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_gtpu_error_l2all_payload(p_set_msg,GTPU_ERROR_TRACE_L2ALL);
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_set_log_cat_disable_mac_all_pkt
 * Description    : To disable mac logs.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_set_log_cat_disable_mac_all_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= (L2_SYS_FAIL|MAC_L1_INF|MAC_UL_HARQ|MAC_RACH|MAC_RRC_INF|MAC_OAM_INF|MAC_DL_HARQ|MAC_DL_PC|MAC_UL_PC|MAC_DL_Strategy|MAC_UL_Strategy|MAC_PDSCH|MAC_PDCCH|MAC_PUSCH|MAC_PUCCH|MAC_BCCH|MAC_PCCH|MAC_CCCH|MAC_CQI|MAC_SRS|MAC_TB_UL|MAC_TB_DL|MAC_DRX|MAC_MEAS_GAP|MAC_MEAS_HANDLER|MAC_UL_CSI|MAC_MUX|MAC_DL_SPS_STRATEGY|MAC_UL_SPS_STRATEGY|MAC_SCHEDULER|MAC_ATB|MAC_TA );

	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_l2_disable_sys_fail_pkt
 * Description    : To disable system failure.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_l2_disable_sys_fail_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat;
	oam_mac_disable_log_cat.logCategory = L2_SYS_FAIL;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_lmac_l1_inf_pkt
 * Description    : To disable mac information log.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_lmac_l1_inf_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO} ;
	oam_mac_disable_log_cat.logCategory |= MAC_L1_INF;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_ul_harq_pkt
 * Description    : To disable mac uplink log.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_ul_harq_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= MAC_UL_HARQ;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_dl_harq_pkt
 * Description    : To disable mac downlink log.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_dl_harq_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= MAC_DL_HARQ;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_rach_pkt
 * Description    : To disable mac rach log.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_rach_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= MAC_RACH;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_rrc_inf_pkt
 * Description    : To disable mac rrc information log.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_rrc_inf_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= MAC_RRC_INF;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_oam_inf_pkt
 * Description    : To disable mac oam information log.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_oam_inf_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= MAC_OAM_INF;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*CSR:00055317_FIX_START*/
/*******************************************************************************
 * Function Name  : oam_disable_mac_rrm_inf_pkt
 * Description    : To disable mac oam information log.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ********************************************************************************/
oam_return_et
oam_disable_mac_rrm_inf_pkt
(
 UInt16 C,
 char* const *V
 )
{
        /* SPR 17777 FIX START */
        qcliHandleUnusedParameter(C);
        qcliHandleUnusedParameter(V);
        /* SPR 17777 FIX END */

        UInt16 msg_length = OAM_ZERO;
        UInt16 msg_api_length =  OAM_ZERO;
        UInt16 new_trans_id = OAM_ZERO;

        UInt8 *p_msg, *p_set_msg;
        new_trans_id = ++qcli_trans_id;
        msg_length = sizeof(oam_mac_disable_log_category_req_t);
    msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
        p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

        if(OAM_NULL == p_msg)
        {
                /* Not enough memory */
                return OAM_FAILURE;
        }
        oam_memset(p_msg, OAM_ZERO, msg_api_length);
        p_set_msg = p_msg;
        /* Fill CSPL header */
        oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
                     OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

        /* Fill QCLI header */
        p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
        oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
                       OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

        /* Fill payload */
        p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
        oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
        oam_mac_disable_log_cat.logCategory |= MAC_RRM_INF;
        oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
        oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
        return OAM_SUCCESS;
}

/*CSR:00055317_FIX_END*/

/* SPR 55317 START */

/*******************************************************************************
 * Function Name  : oam_disable_mac_dl_strategy_pkt
 * Description    : To disable mac_dl_strategy 
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_dl_strategy_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= MAC_DL_Strategy;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_ul_strategy_pkt
 * Description    : To disable mac_ul_strategy 
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_ul_strategy_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory = MAC_UL_Strategy;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_pdsch_pkt
 * Description    : To disable mac_pdsch 
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_pdsch_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |=  MAC_PDSCH;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_pdcch_pkt
 * Description    : To disable mac_pdcch 
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_pdcch_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= MAC_PDCCH;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_pusch_pkt
 * Description    : To disable mac_pusch 
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_pusch_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= MAC_PUSCH;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_pucch_pkt
 * Description    : To disable mac_pucch 
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_pucch_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= MAC_PUCCH;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_bcch_pkt
 * Description    : To disable mac_pucch 
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_bcch_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= MAC_BCCH;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_pcch_pkt
 * Description    : To disable mac_pucch 
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_pcch_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= MAC_PCCH;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_ccch_pkt
 * Description    : To disable mac_pucch 
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_ccch_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= MAC_CCCH;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_cqi_pkt
 * Description    : To disable mac_pucch 
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_cqi_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= MAC_CQI;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_srs_pkt
 * Description    : To disable mac_pucch 
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_srs_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= MAC_SRS;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_tb_ul_pkt
 * Description    : To disable mac_pucch 
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_tb_ul_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= MAC_TB_UL;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_tb_dl_pkt
 * Description    : To disable mac_pucch 
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_tb_dl_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= MAC_TB_DL;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_drx_pkt
 * Description    : To disable mac_pucch 
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_drx_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= MAC_DRX;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_meas_gap_pkt
 * Description    : To disable mac_pucch 
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_meas_gap_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= MAC_MEAS_GAP;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_meas_handler_pkt
 * Description    : To disable mac_pucch 
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_meas_handler_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= MAC_MEAS_HANDLER;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_ul_csi_pkt
 * Description    : To disable mac_pucch 
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_ul_csi_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= MAC_UL_CSI;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}



/*******************************************************************************
 * Function Name  : oam_disable_mac_mux_pkt
 * Description    : To disable mac_pucch 
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_mux_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= MAC_MUX;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_dl_sps_strategy_pkt 
 * Description    : To disable mac_pucch 
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_dl_sps_strategy_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= MAC_DL_SPS_STRATEGY;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_ul_sps_strategy_pkt 
 * Description    : To disable mac_pucch 
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_ul_sps_strategy_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= MAC_UL_SPS_STRATEGY;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_scheduler_pkt
 * Description    : To disable mac_pucch 
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_scheduler_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= MAC_SCHEDULER;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_atb_pkt
 * Description    : To disable mac_pucch 
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_atb_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= MAC_ATB;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}


/*******************************************************************************
 * Function Name  : oam_disable_mac_ta_pkt
 * Description    : To disable mac_pucch 
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_ta_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= MAC_TA;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/* SPR 55317 END */



/*******************************************************************************
 * Function Name  : oam_disble_mac_dl_pc_pkt
 * Description    : To disable mac downlink pc log.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disble_mac_dl_pc_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= MAC_DL_PC;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_mac_ul_pc_pkt
 * Description    : To disable mac uplink pc log.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_mac_ul_pc_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_mac_disable_log_category_req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_MAC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_mac_disable_log_category_req_t oam_mac_disable_log_cat = {OAM_ZERO};
	oam_mac_disable_log_cat.logCategory |= MAC_UL_PC;
	oam_memcpy(p_set_msg,&oam_mac_disable_log_cat,sizeof(oam_mac_disable_log_category_req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/* SPR 55317 START */
/* Redundant Code Removed Here */

/*******************************************************************************
 * Function Name  : oam_set_log_cat_disable_rlc_all_pkt 
 * Description    : To disable rlc log .
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_set_log_cat_disable_rlc_all_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt16 msg_length = OAM_ZERO;
	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_RLC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_RLC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	p_set_msg = p_set_msg +OAM_MGMT_INTF_HEADER_SIZE;
	Oam_LogCategory_Req_t rlc_log_disable_req = {OAM_ZERO};
	rlc_log_disable_req.logCategory |= (RLC_AM |RLC_UM | RLC_TM |RLC_TX | RLC_RX |RLC_MAC | RLC_PDCP | RLC_RRC | RLC_OAM |RLC_TIMER );
	oam_memcpy(p_set_msg,&rlc_log_disable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}


/*******************************************************************************
 * Function Name  : oam_disable_rlc_am_pkt 
 * Description    : To disable rlc log .
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_rlc_am_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt16 msg_length = OAM_ZERO;
	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_RLC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_RLC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	p_set_msg = p_set_msg +OAM_MGMT_INTF_HEADER_SIZE;
	Oam_LogCategory_Req_t rlc_log_disable_req = {OAM_ZERO};
	rlc_log_disable_req.logCategory |= RLC_AM;
	oam_memcpy(p_set_msg,&rlc_log_disable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_rlc_um_pkt 
 * Description    : To disable rlc_um log .
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_rlc_um_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt16 msg_length = OAM_ZERO;
	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_RLC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_RLC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	p_set_msg = p_set_msg +OAM_MGMT_INTF_HEADER_SIZE;
	Oam_LogCategory_Req_t rlc_log_disable_req = {OAM_ZERO};
	rlc_log_disable_req.logCategory |= RLC_UM;
	oam_memcpy(p_set_msg,&rlc_log_disable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_rlc_tm_pkt 
 * Description    : To disable rlc_tm log .
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_rlc_tm_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt16 msg_length = OAM_ZERO;
	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_RLC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_RLC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	p_set_msg = p_set_msg +OAM_MGMT_INTF_HEADER_SIZE;
	Oam_LogCategory_Req_t rlc_log_disable_req = {OAM_ZERO};
	rlc_log_disable_req.logCategory |= RLC_TM;
	oam_memcpy(p_set_msg,&rlc_log_disable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_rlc_tx_pkt 
 * Description    : To disable rlc_tx log .
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_rlc_tx_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt16 msg_length = OAM_ZERO;
	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_RLC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_RLC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	p_set_msg = p_set_msg +OAM_MGMT_INTF_HEADER_SIZE;
	Oam_LogCategory_Req_t rlc_log_disable_req = {OAM_ZERO};
	rlc_log_disable_req.logCategory |= RLC_TX;
	oam_memcpy(p_set_msg,&rlc_log_disable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_rlc_rx_pkt 
 * Description    : To disable rlc_rx log .
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_rlc_rx_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt16 msg_length = OAM_ZERO;
	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_RLC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_RLC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	p_set_msg = p_set_msg +OAM_MGMT_INTF_HEADER_SIZE;
	Oam_LogCategory_Req_t rlc_log_disable_req = {OAM_ZERO};
	rlc_log_disable_req.logCategory |= RLC_RX;
	oam_memcpy(p_set_msg,&rlc_log_disable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_rlc_mac_pkt 
 * Description    : To disable rlc_mac log .
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_rlc_mac_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt16 msg_length = OAM_ZERO;
	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_RLC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_RLC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	p_set_msg = p_set_msg +OAM_MGMT_INTF_HEADER_SIZE;
	Oam_LogCategory_Req_t rlc_log_disable_req = {OAM_ZERO};
	rlc_log_disable_req.logCategory |= RLC_MAC;
	oam_memcpy(p_set_msg,&rlc_log_disable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_rlc_pdcp_pkt 
 * Description    : To disable rlc_pdcp log .
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_rlc_pdcp_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt16 msg_length = OAM_ZERO;
	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_RLC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_RLC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	p_set_msg = p_set_msg +OAM_MGMT_INTF_HEADER_SIZE;
	Oam_LogCategory_Req_t rlc_log_disable_req = {OAM_ZERO};
	rlc_log_disable_req.logCategory |= RLC_PDCP;
	oam_memcpy(p_set_msg,&rlc_log_disable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_rlc_rrc_pkt 
 * Description    : To disable rlc_rrc log .
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_rlc_rrc_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt16 msg_length = OAM_ZERO;
	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_RLC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_RLC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	p_set_msg = p_set_msg +OAM_MGMT_INTF_HEADER_SIZE;
	Oam_LogCategory_Req_t rlc_log_disable_req = {OAM_ZERO};
	rlc_log_disable_req.logCategory |= RLC_RRC;
	oam_memcpy(p_set_msg,&rlc_log_disable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_rlc_oam_pkt 
 * Description    : To disable rlc_oam log .
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_rlc_oam_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt16 msg_length = OAM_ZERO;
	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_RLC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_RLC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	p_set_msg = p_set_msg +OAM_MGMT_INTF_HEADER_SIZE;
	Oam_LogCategory_Req_t rlc_log_disable_req = {OAM_ZERO};
	rlc_log_disable_req.logCategory |= RLC_OAM;
	oam_memcpy(p_set_msg,&rlc_log_disable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_disable_rlc_timer_pkt 
 * Description    : To disable rlc_timer log .
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_disable_rlc_timer_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt16 msg_length = OAM_ZERO;
	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_RLC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_RLC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	p_set_msg = p_set_msg +OAM_MGMT_INTF_HEADER_SIZE;
	Oam_LogCategory_Req_t rlc_log_disable_req = {OAM_ZERO};
	rlc_log_disable_req.logCategory |= RLC_TIMER;
	oam_memcpy(p_set_msg,&rlc_log_disable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}



/* SPR 55317 END */

/*******************************************************************************
 * Function Name  : oam_set_log_cat_disable_pdcp_all_pkt 
 * Description    : To disable pdcp log .
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_set_log_cat_disable_pdcp_all_pkt
(
 UInt16 C,
 char * const*V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;
	UInt16 msg_length = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length =  msg_length+ QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_PDCP_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_PDCP_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	Oam_LogCategory_Req_t pdcp_log_disable_req = {OAM_ZERO};
	pdcp_log_disable_req.logCategory |= (PDCP_RRCOAM|PDCP_TX|PDCP_RX|PDCP_RRM_INT) ;
	oam_memcpy(p_set_msg,&pdcp_log_disable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_set_log_cat_disable_pdcp_rrcoam_pkt 
 * Description    : To disable pdcp log .
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_set_log_cat_disable_pdcp_rrcoam_pkt
(
 UInt16 C,
 char * const*V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;
	UInt16 msg_length = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length =  msg_length+ QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_PDCP_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_PDCP_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	Oam_LogCategory_Req_t pdcp_log_disable_req = {OAM_ZERO};
	pdcp_log_disable_req.logCategory |= PDCP_RRCOAM ;
	oam_memcpy(p_set_msg,&pdcp_log_disable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_set_log_cat_disable_pdcp_tx_pkt 
 * Description    : To disable pdcp log .
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_set_log_cat_disable_pdcp_tx_pkt
(
 UInt16 C,
 char * const*V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;
	UInt16 msg_length = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length =  msg_length+ QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_PDCP_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_PDCP_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	Oam_LogCategory_Req_t pdcp_log_disable_req = {OAM_ZERO};
	pdcp_log_disable_req.logCategory |= PDCP_TX ;
	oam_memcpy(p_set_msg,&pdcp_log_disable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_set_log_cat_disable_pdcp_rx_pkt 
 * Description    : To disable pdcp log .
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_set_log_cat_disable_pdcp_rx_pkt
(
 UInt16 C,
 char * const*V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;
	UInt16 msg_length = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length =  msg_length+ QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_PDCP_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_PDCP_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	Oam_LogCategory_Req_t pdcp_log_disable_req = {OAM_ZERO};
	pdcp_log_disable_req.logCategory |= PDCP_RX ;
	oam_memcpy(p_set_msg,&pdcp_log_disable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_set_log_cat_disable_pdcp_rrm_int_pkt 
 * Description    : To disable pdcp log .
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_set_log_cat_disable_pdcp_rrm_int_pkt
(
 UInt16 C,
 char * const*V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;
	UInt16 msg_length = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(Oam_LogCategory_Req_t);
	msg_api_length =  msg_length+ QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_PDCP_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_PDCP_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	Oam_LogCategory_Req_t pdcp_log_disable_req = {OAM_ZERO};
	pdcp_log_disable_req.logCategory |= PDCP_RRM_INT ;
	oam_memcpy(p_set_msg,&pdcp_log_disable_req,sizeof(Oam_LogCategory_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}


/*SPR 55317 END */
/*******************************************************************************
 * Function Name  : oam_set_log_cat_disable_gtpu_pkt
 * Description    : To disable gtpu log .
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_set_log_cat_disable_gtpu_pkt
(
 UInt16 C,
 char* const* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length = OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length = sizeof(UInt64) +  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_EGTPU_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_DISABLE_LOG_CAT_EGTPU_REQ,msg_api_length);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);

	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	UInt64 pdcp_log_disable_req;
	pdcp_log_disable_req = (UInt64)(oam_atoi(V[1]));
	oam_memcpy(p_set_msg,&pdcp_log_disable_req,sizeof(UInt64));
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_rrc_fatallog_pkt
 * Description    : To view fatal logs of rrc.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rrc_fatallog_pkt
(
 UInt16 C,
 const char* V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	/*CSR 57877 Fix Start */
	msg_length = sizeof(OAM_RRC_CRITICALLOG);
	/*CSR 57877 Fix End*/
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L3_RRC,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,OAM_MODULE_ID,
			SET_LOG_LEVEL_L3_RRC,msg_api_length);

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	/*CSR 57877 Fix Start */
	oam_rrc_fatallog_payload(p_set_msg,OAM_RRC_CRITICALLOG);
	/*CSR 57877 Fix End*/
	oam_qcli_intf_send_message(p_msg,OAM_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_rach_maxharq_msg_pkt
 * Description    : To view max harq message.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rach_maxharq_msg_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_fapservice_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_rach_maxharq_msg_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_fapservice_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_fapservice_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_FAP_CONFIG_RAN_MAC_RACH_MAXHARQ_REQ,msg_api_length);
	/* Fill QCLI header */
	p_fapservice_msg = p_fapservice_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_fapservice_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_FAP_CONFIG_RAN_MAC_RACH_MAXHARQ_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_fapservice_msg = p_fapservice_msg + QCLI_HEADER_SIZE;
	oam_rach_maxharq_msg_payload(p_fapservice_msg,V[1]);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_msg_power_offset_group_pkt
 * Description    : To view power offset group B details.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_msg_power_offset_group_pkt
(
 UInt16 C,
 rrmc_group_b_msg_power_offset_et pwr_offset
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_fapservice_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_msg_power_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_fapservice_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_fapservice_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_FAP_CONFIG_RAN_MAC_RACH_MSG_PWR_OFFSET_GRP_B_REQ,msg_api_length);
	/* Fill QCLI header */
	p_fapservice_msg = p_fapservice_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_fapservice_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_FAP_CONFIG_RAN_MAC_RACH_MSG_PWR_OFFSET_GRP_B_REQ,msg_api_length -OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_fapservice_msg = p_fapservice_msg + QCLI_HEADER_SIZE;
	oam_msg_power_offset_payload(p_fapservice_msg,pwr_offset);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}



/*******************************************************************************
 * Function Name  : oam_power_ramping_step_pkt
 * Description    : To view power ramping step details.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_power_ramping_step_pkt
(
 UInt16 C,
 rrmc_power_ramping_step_et pwr_ramp
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_fapservice_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_pwr_ramp_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_fapservice_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_fapservice_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_FAP_CONFIG_RAN_MAC_RACH_PWR_RAMP_STEP_REQ,msg_api_length);
	/* Fill QCLI header */
	p_fapservice_msg = p_fapservice_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_fapservice_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_FAP_CONFIG_RAN_MAC_RACH_PWR_RAMP_STEP_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_fapservice_msg = p_fapservice_msg + QCLI_HEADER_SIZE;
	oam_pwr_ramp_step_payload(p_fapservice_msg,pwr_ramp);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_init_recv_tgt_pkt
 * Description    : To view initial target received details.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_init_recv_tgt_pkt
(
 UInt16 C,
 rrmc_preamble_init_rec_target_pow_et init_tgt
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_fapservice_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_init_recv_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_fapservice_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_fapservice_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_FAP_CONFIG_RAN_MAC_RACH_PREAMBLE_INIT_RX_TGT_PWR_REQ,msg_api_length);
	/* Fill QCLI header */
	p_fapservice_msg = p_fapservice_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_fapservice_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_FAP_CONFIG_RAN_MAC_RACH_PREAMBLE_INIT_RX_TGT_PWR_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_fapservice_msg = p_fapservice_msg + QCLI_HEADER_SIZE;
	oam_init_recv_tgt_payload(p_fapservice_msg,init_tgt);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_preamble_trans_max_pkt
 * Description    : To view preamble trans maximum value details.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_preamble_trans_max_pkt
(
 UInt16 C,
 rrmc_preamble_trans_max_et trans_max
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_fapservice_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_trans_max_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_fapservice_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_fapservice_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_FAP_CONFIG_RAN_MAC_RACH_PREAMBLE_TX_MAX_REQ,msg_api_length);
	/* Fill QCLI header */
	p_fapservice_msg = p_fapservice_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_fapservice_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_FAP_CONFIG_RAN_MAC_RACH_PREAMBLE_TX_MAX_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_fapservice_msg = p_fapservice_msg + QCLI_HEADER_SIZE;
	oam_preamble_trans_payload(p_fapservice_msg,trans_max);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_n310_pkt
 * Description    : To view n310 rrc timer details.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_n310_pkt
(
 UInt16 C,
 rrmc_timer_n310_et timer_n310
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_fapservice_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_n310_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_fapservice_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_fapservice_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_FAP_CONFIG_RAN_RRC_TIMER_N310_REQ,msg_api_length);
	/* Fill QCLI header */
	p_fapservice_msg = p_fapservice_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_fapservice_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_FAP_CONFIG_RAN_RRC_TIMER_N310_REQ,msg_api_length -OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_fapservice_msg = p_fapservice_msg + QCLI_HEADER_SIZE;
	oam_n310_payload(p_fapservice_msg,timer_n310);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}


/*******************************************************************************
 * Function Name  : oam_n311_pkt
 * Description    : To view n311 rrc timer details.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_n311_pkt
(
 UInt16 C,
 rrmc_timer_n311_et timer_n311
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_fapservice_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_n311_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_fapservice_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_fapservice_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_FAP_CONFIG_RAN_RRC_TIMER_N311_REQ,msg_api_length);
	/* Fill QCLI header */
	p_fapservice_msg = p_fapservice_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_fapservice_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_FAP_CONFIG_RAN_RRC_TIMER_N311_REQ,msg_api_length -OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_fapservice_msg = p_fapservice_msg + QCLI_HEADER_SIZE;
	oam_n311_payload(p_fapservice_msg,timer_n311);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}


/*******************************************************************************
 * Function Name  : oam_t300_pkt
 * Description    : To view t300 rrc timer details.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_t300_pkt
(
 UInt16 C,
 rrmc_timer_300_301_et timer_300_301
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_fapservice_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_t300_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_fapservice_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_fapservice_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_FAP_CONFIG_RAN_RRC_TIMER_T300_REQ,msg_api_length);
	/* Fill QCLI header */
	p_fapservice_msg = p_fapservice_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_fapservice_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_FAP_CONFIG_RAN_RRC_TIMER_T300_REQ,msg_api_length -OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_fapservice_msg = p_fapservice_msg + QCLI_HEADER_SIZE;
	oam_t300_payload(p_fapservice_msg,timer_300_301);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_t301_pkt
 * Description    : To view t301 rrc timer details.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_t301_pkt
(
 UInt16 C,
 rrmc_timer_300_301_et timer_300_301
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_fapservice_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_t301_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_fapservice_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_fapservice_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_FAP_CONFIG_RAN_RRC_TIMER_T301_REQ,msg_api_length);
	/* Fill QCLI header */
	p_fapservice_msg = p_fapservice_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_fapservice_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_FAP_CONFIG_RAN_RRC_TIMER_T301_REQ,msg_api_length -OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_fapservice_msg = p_fapservice_msg + QCLI_HEADER_SIZE;
	oam_t301_payload(p_fapservice_msg,timer_300_301);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_t310_pkt
 * Description    : To view t310 rrc timer details.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_t310_pkt
(
 UInt16 C,
 rrmc_timer_310_et timer_t310
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_fapservice_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_t310_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_fapservice_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_fapservice_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_FAP_CONFIG_RAN_RRC_TIMER_T310_REQ,msg_api_length);
	/* Fill QCLI header */
	p_fapservice_msg = p_fapservice_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_fapservice_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_FAP_CONFIG_RAN_RRC_TIMER_T310_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_fapservice_msg = p_fapservice_msg + QCLI_HEADER_SIZE;
	oam_t310_payload(p_fapservice_msg,timer_t310);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}


/*******************************************************************************
 * Function Name  : oam_t311_pkt
 * Description    : To view t311 rrc timer details.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_t311_pkt
(
 UInt16 C,
 rrmc_timer_311_et timer_t311
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_fapservice_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_t311_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_fapservice_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_fapservice_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_FAP_CONFIG_RAN_RRC_TIMER_T311_REQ,msg_api_length);
	/* Fill QCLI header */
	p_fapservice_msg = p_fapservice_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_fapservice_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_FAP_CONFIG_RAN_RRC_TIMER_T311_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_fapservice_msg = p_fapservice_msg + QCLI_HEADER_SIZE;
	oam_t311_payload(p_fapservice_msg,timer_t311);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/****************************************************************************
 * Function Name  : oam_mac_config_kpi_stat_conf
 * Inputs         : p_api_buf, msg_len
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function provide confirmation for KPI Statistics configuration request from OAM.
 ****************************************************************************/
oam_return_et oam_qcli_mac_config_kpi_stat_conf(void* p_api_buf, U16 msg_len)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(msg_len);
    /* SPR 17777 FIX END */

	U8* p_msg   = PNULL;
	oam_mac_config_KPI_stats_conf_t *p_oam_mac_config_kpi_stats_conf = PNULL;

	p_msg      = p_api_buf + OAM_MGMT_INTF_HEADER_SIZE;
	p_oam_mac_config_kpi_stats_conf = (oam_mac_config_KPI_stats_conf_t *)p_msg;

	if (OAM_SUCCESS == p_oam_mac_config_kpi_stats_conf->responseCode)
	{

		oam_fprintf(stdout, "Response Received : SUCCESS with Reason code [%d]\n", p_oam_mac_config_kpi_stats_conf->responseCode);
	}
	else /* Failure is received from MAC for MAC_CONFIGURE_KPI_STATS_CNF */
	{
		oam_fprintf(stdout,"\nMAC_CONFIG_KPI_STATS_CNF[FAILURE] ");
	}
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_pdcp_config_kpi_stat_conf
 * Inputs         : p_api_buf, msg_len
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function provide confirmation for KPI Statistics configuration request from OAM.
 ****************************************************************************/
oam_return_et oam_qcli_pdcp_config_kpi_stat_conf(void* p_api_buf, U16 msg_len)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(msg_len);
    /* SPR 17777 FIX END */

	U8* p_msg   = PNULL;
	oam_pdcp_config_KPI_stats_conf_t *p_oam_pdcp_config_kpi_stats_conf = PNULL;

	p_msg      = p_api_buf + OAM_MGMT_INTF_HEADER_SIZE;
	p_oam_pdcp_config_kpi_stats_conf = (oam_pdcp_config_KPI_stats_conf_t *)p_msg;

	if (OAM_SUCCESS == p_oam_pdcp_config_kpi_stats_conf->responseCode)
	{

		oam_fprintf(stdout, "Response Received : SUCCESS with Reason code [%d]\n", p_oam_pdcp_config_kpi_stats_conf->responseCode);
	}
	else /* Failure is received from PDCP for PDCP_CONFIGURE_KPI_STATS_CNF */
	{
		oam_fprintf(stdout,"\nPDCP_CONFIGURE_KPI_STATS_CNF[FAILURE] ");
	}
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_mac_get_kpi_stat_conf
 * Inputs         : p_api_buf, msg_len
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function provide confirmation for KPI Statistics configuration request from OAM.
 ****************************************************************************/
oam_return_et
oam_qcli_mac_get_kpi_stat_conf
(
 void* p_api_buf
 )
{
	UInt8 *p_msg;
	p_msg  = OAM_NULL;

	oam_mac_KPI_stats_ind_t *p_oam_mac_get_kpi_stats_conf;
	p_oam_mac_get_kpi_stats_conf = OAM_NULL;
	UInt32 durationTTI     = OAM_ZERO;
	UInt32 PRB             = OAM_ZERO;
	oam_counter_t loop_counter = OAM_ZERO ;

	p_msg      = p_api_buf + OAM_MGMT_INTF_HEADER_SIZE;
	p_oam_mac_get_kpi_stats_conf = (oam_mac_KPI_stats_ind_t*)p_msg;
	durationTTI = p_oam_mac_get_kpi_stats_conf->DurationTTI;

	PRB=200;

	oam_fprintf(stdout, "\tduration : %u\n",     /*BUG_ID-104*/
			p_oam_mac_get_kpi_stats_conf->DurationTTI);
	if(p_oam_mac_get_kpi_stats_conf->DurationTTI != OAM_ZERO)
	{
		/* kpi's corrosponding to which bit is set*/
		if (kpiToGetBitMap & 1)
		{
			oam_fprintf(stdout, "\ttotalPRBUsageDL : %.02f\n",
					(float)(p_oam_mac_get_kpi_stats_conf->
						totalPRBUsageDL * 100)/(float)(PRB * durationTTI));
		}
		if (kpiToGetBitMap & 2)
		{
			oam_fprintf(stdout, "\ttotalPRBUsageUL : %.02f\n",
					(float)(p_oam_mac_get_kpi_stats_conf->
						totalPRBUsageUL * 100)/(float)(PRB * durationTTI));
		}
		/*loop for MAX_QCI */ /*BUG_ID-106*/
		for(loop_counter = OAM_ZERO; loop_counter <MAX_QCI; loop_counter++)
		{
			/*    / *BUG_ID-105* /
			      if (loop_counter < OAM_ZERO || loop_counter >= MAX_QCI)
			      {
			      oam_fprintf(stdout,"\nInvalid value for the for loop");
			      return OAM_FAILURE;

			      }*/
			if((511 == kpiToGetBitMap)||(3  < kpiToGetBitMap &&
						64 != kpiToGetBitMap && 256 != kpiToGetBitMap))
			{
				/*BUG_ID-107*/
				if (OAM_ZERO != p_oam_mac_get_kpi_stats_conf->
						totalPRBUsagePerQciDL[loop_counter] ||
						OAM_ZERO != p_oam_mac_get_kpi_stats_conf->
						totalPRBUsagePerQciUL[loop_counter] ||
						OAM_ZERO != p_oam_mac_get_kpi_stats_conf->
						numberOfActiveUEPerQciDL[loop_counter] ||
						OAM_ZERO != p_oam_mac_get_kpi_stats_conf->
						numberOfActiveUEPerQciUL[loop_counter] ||
						OAM_ZERO != p_oam_mac_get_kpi_stats_conf->
						totalPktDelayPerQciDL[loop_counter] )
				{
					oam_fprintf(stdout, "\tFor QCI : %d\n", loop_counter+1);

					if(kpiToGetBitMap & 4)
					{
						oam_fprintf(stdout, "\ttotalPRBUsagePerQciDL : %.02f\n",
								(float)(p_oam_mac_get_kpi_stats_conf->
									totalPRBUsagePerQciDL[loop_counter]* 100)/
								(float)(PRB * durationTTI));
					}
					if(kpiToGetBitMap & 8)
					{
						oam_fprintf(stdout, "\ttotalPRBUsagePerQciUL : %.02f\n",
								(float)(p_oam_mac_get_kpi_stats_conf->
									totalPRBUsagePerQciUL[loop_counter]* 100)/
								(float)(PRB * durationTTI));
					}
					if(kpiToGetBitMap & 16)
					{
						oam_fprintf(stdout, "\tnumberOfActiveUEPerQciDL : %.02f\n",
								((float)((p_oam_mac_get_kpi_stats_conf->
										numberOfActiveUEPerQciDL[loop_counter]*
										ACTIVE_USER_SAMPLING_TIMER_TTI)/durationTTI)/
								 1000.00));
					}
					if(kpiToGetBitMap & 32)
					{
						oam_fprintf(stdout, "\tnumberOfActiveUEPerQciUL : %.02f\n",
								((float)((p_oam_mac_get_kpi_stats_conf->
										numberOfActiveUEPerQciUL[loop_counter]*
										ACTIVE_USER_SAMPLING_TIMER_TTI)/durationTTI)/
								 1000.00));
					}
					if((128 & kpiToGetBitMap) && p_oam_mac_get_kpi_stats_conf->
							totalPktAckedInDLPerQci[loop_counter] != OAM_ZERO)
					{
						oam_fprintf(stdout, "\tpacketDelayInDLPerQci : %.02f\n",
								(float)(p_oam_mac_get_kpi_stats_conf->
									totalPktDelayPerQciDL[loop_counter])/
								(float)(p_oam_mac_get_kpi_stats_conf->
									totalPktAckedInDLPerQci[loop_counter]));

					}
				}

			}
		}
		if(kpiToGetBitMap & 64)
		{
			oam_fprintf(stdout, "\trcvdRandomAccessPreamblesContentionFree : %.02f\n",
					(float)(p_oam_mac_get_kpi_stats_conf->
						rcvdRandomAccessPreamblesContentionFree * 1000)/
					(float)durationTTI);
			oam_fprintf(stdout, "\trcvdRandomAccessPreamblesGroupA : %.02f\n",
					(float)(p_oam_mac_get_kpi_stats_conf->
						rcvdRandomAccessPreamblesGroupA * 1000)/
					(float)durationTTI);
			oam_fprintf(stdout, "\trcvdRandomAccessPreamblesGroupB : %.02f\n",
					(float)(p_oam_mac_get_kpi_stats_conf->
						rcvdRandomAccessPreamblesGroupB * 1000)/
					(float)durationTTI);
		}
	}

	else
	{
		oam_fprintf(stdout, "\tALL RECIEVED VALUES ARE NULL\n");
	}


	kpiToGetBitMap = 0;

	return OAM_SUCCESS;
}
/****************************************************************************
 * Function Name  : oam_pdcp_get_kpi_stat_conf
 * Inputs         : p_api_buf, msg_len
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function provide confirmation for KPI Statistics configuration request from OAM.
 ****************************************************************************/
oam_return_et oam_qcli_pdcp_get_kpi_stat_conf(void* p_api_buf, U16 msg_len)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(msg_len);
    /* SPR 17777 FIX END */

	U8* p_msg   = PNULL;
	U8  flag    = 0;
	oam_pdcp_KPI_stats_ind_t *p_oam_pdcp_get_kpi_stats_conf = PNULL;

	p_msg      = p_api_buf + OAM_MGMT_INTF_HEADER_SIZE;
	p_oam_pdcp_get_kpi_stats_conf = (oam_pdcp_KPI_stats_ind_t *)p_msg;


	UInt8 qci_index;
	oam_fprintf(stdout, "\tDurtationT : %d\n", p_oam_pdcp_get_kpi_stats_conf->durationTTI);
	for(qci_index = 0;qci_index < RRM_MAX_QCI;qci_index++)
	{
		if(p_oam_pdcp_get_kpi_stats_conf->totalSduRcvdDlperQci[qci_index] != 0)
		{
			if(kpiPdcpGetBitMap == 1 || kpiPdcpGetBitMap == 2) /* SPR 3521 and 3522 Fix */
			{
				oam_fprintf(stdout, "\tFor QCI : %d\n", qci_index+1);
			}
			if(kpiPdcpGetBitMap & 1 )
			{
				oam_fprintf(stdout, "\tPacketdiscardrateDLperQCI : %.02f\n",((float)(p_oam_pdcp_get_kpi_stats_conf->totalDlDrbDiscardRatePerQci[qci_index] )/(float)(p_oam_pdcp_get_kpi_stats_conf->totalSduRcvdDlperQci[qci_index])) * 1000000);
			}
/*spr 19296 fix start*/
        /*code deleted*/
/*spr 19296 fix end*/
			flag = 1;
		}
		if(p_oam_pdcp_get_kpi_stats_conf->totalSduRcvdUlperQci[qci_index] != 0) /* SPR 3521 and 3522 Fix */
		{
/*spr 19296 fix start*/
			if(kpiPdcpGetBitMap & 2 )
			{
				oam_fprintf(stdout, "\tFor QCI : %d\n", qci_index+1);
				oam_fprintf(stdout, "\tPacketlossRateInULperQCI : %.02f\n", ((float)(p_oam_pdcp_get_kpi_stats_conf->totalUlLossRatePerQci[qci_index] )/(float)(p_oam_pdcp_get_kpi_stats_conf->totalSduRcvdUlperQci[qci_index])) * 1000000);
				flag = 1;		
			}
/*spr 19296 fix end*/
		}
	}
	if(flag == 0)
	{
		oam_fprintf(stdout, "\tAll Stats Received in PDCP GET KPI STATS CNF are NULL");
	}

	kpiPdcpGetBitMap = 0;
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_enable_rrm_pkt
 * Inputs         : C, V
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function enable log level pkt from OAM.
 ****************************************************************************/

oam_return_et
oam_enable_rrm_pkt
(
 UInt16 C,
 char * const *V,
 rrm_oam_log_level_et log_level
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

    UInt16 msg_length = OAM_ZERO;
    UInt16 msg_api_length =  OAM_ZERO;
    UInt16 new_trans_id = OAM_ZERO;

    UInt8 *p_msg, *p_set_msg;
    new_trans_id = ++qcli_trans_id;
    msg_length = sizeof(oam_rrm_log_enable_disable_req_t);
    msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
    p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
    if(OAM_NULL == p_msg)
    {
        /* Not enough memory */
        return OAM_FAILURE;
    }
    oam_memset(p_msg, OAM_ZERO, msg_api_length);
    p_set_msg = p_msg;
    /* Fill CSPL header */
    oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
            OAM_SET_ENABLE_DISABLE_RRM_LOG_REQ,msg_api_length);
    /* Fill QCLI header */
    p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
            OAM_SET_ENABLE_DISABLE_RRM_LOG_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
    /* Fill Payload */
    p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
    oam_enable_rrm_log_lvl_payload(p_set_msg,V[1],log_level);
    oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
    return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_enable_rrm_error_pkt
 * Inputs         : C, V
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function provide enable rrm error pkt from OAM.
 ****************************************************************************/

oam_return_et
oam_enable_rrm_error_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_rrm_log_enable_disable_req_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_DISABLE_RRM_LOG_REQ,msg_api_length);
	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_DISABLE_RRM_LOG_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_enable_rrm_error_payload(p_set_msg,V[1]);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_log_enable_rrm_warn_pkt
 * Inputs         : C, V
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function provide log enable rrm warn pktfrom OAM.
 ****************************************************************************/

oam_return_et
oam_log_enable_rrm_warn_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_rrm_log_enable_disable_req_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_DISABLE_RRM_LOG_REQ,msg_api_length);
	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_DISABLE_RRM_LOG_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_enable_rrm_warn_payload(p_set_msg,V[1]);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_log_enable_rrm_brief_pkt
 * Inputs         : C, V
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function provide log enable rrm brief pkt from OAM.
 ****************************************************************************/
oam_return_et
oam_log_enable_rrm_brief_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_rrm_log_enable_disable_req_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_DISABLE_RRM_LOG_REQ,msg_api_length);
	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_DISABLE_RRM_LOG_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_enable_rrm_brief_payload(p_set_msg,V[1]);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_log_enable_rrm_detail_pkt
 * Inputs         : C, V
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function provide log enable rrm detail pkt from OAM.
 ****************************************************************************/
oam_return_et
oam_log_enable_rrm_detail_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_rrm_log_enable_disable_req_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_DISABLE_RRM_LOG_REQ,msg_api_length);
	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_DISABLE_RRM_LOG_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_enable_rrm_detail_payload(p_set_msg,V[1]);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}


#ifdef OAM_SON_ENABLE
/*******************************************************************************
 * Function Name  : oam_son_enable_pkt
 * Description    : To enable logs of son .
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et
oam_son_enable_pkt
(
 UInt16 C,
 char * const *V,
 setson log_level
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

    UInt16 msg_length = OAM_ZERO;
    UInt16 msg_api_length =  OAM_ZERO;
    UInt16 new_trans_id = OAM_ZERO;

    UInt8 *p_msg, *p_set_msg;
    new_trans_id = ++qcli_trans_id;
    msg_length = sizeof(oam_son_log_enable_disable_req_t);
    msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
    p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
    if(OAM_NULL == p_msg)
    {
        /* Not enough memory */
        return OAM_FAILURE;
    }
    oam_memset(p_msg, OAM_ZERO, msg_api_length);
    p_set_msg = p_msg;
    /* Fill CSPL header */
    oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
            OAM_SET_ENABLE_DISABLE_SON_REQ,msg_api_length);
    /* Fill QCLI header */
    p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
            OAM_SET_ENABLE_DISABLE_SON_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
    /* Fill Payload */
    p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
    oam_son_enable_payload(p_set_msg,V[1],log_level);
    oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
    return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_son_enable_error_pkt
 * Description    : To enable error logs of son .
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et
oam_son_enable_error_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_son_log_enable_disable_req_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_DISABLE_SON_REQ,msg_api_length);
	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_DISABLE_SON_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_son_enable_error_payload(p_set_msg,V[1]);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}



/*******************************************************************************
 * Function Name  : oam_son_enable_warning_pkt
 * Description    : To enable warning logs of son .
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_son_enable_warning_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_son_log_enable_disable_req_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_DISABLE_SON_REQ,msg_api_length);
	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_DISABLE_SON_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_son_enable_warning_payload(p_set_msg,V[1]);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}



/*******************************************************************************
 * Function Name  : oam_son_enable_detailed_pkt
 * Description    : To enable detailed logs of son .
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_son_enable_detailed_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_son_log_enable_disable_req_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_DISABLE_SON_REQ,msg_api_length);
	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_DISABLE_SON_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_son_enable_detailed_payload(p_set_msg,V[1]);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}


/*******************************************************************************
 * Function Name  : oam_son_disable_error_pkt
 * Description    : To disable error logs of son .
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_son_disable_error_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_son_log_enable_disable_req_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_DISABLE_SON_REQ,msg_api_length);
	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_DISABLE_SON_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_son_disable_error_payload(p_set_msg,V[1]);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
#endif
/*******************************************************************************
 * Function Name  : oam_rrm_disable_detailed_pkt
 * Description    : To disable disable detailed pkt .
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rrm_disable_detailed_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_rrm_log_enable_disable_req_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_DISABLE_RRM_LOG_REQ,msg_api_length);
	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_ENABLE_DISABLE_RRM_LOG_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_rrm_disable_detailed_payload(p_set_msg,V[1]);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_log_enable_rrc_pkt
 * Description    : To enable rrc pkt
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_log_enable_rrc_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_ENABLE_RRC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_ENABLE_RRC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_log_enable_x2ap_pkt
 * Description    : To enable x2ap pkt
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_log_enable_x2ap_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

    UInt16 msg_api_length =  OAM_ZERO;
    UInt16 new_trans_id = OAM_ZERO;

    UInt8 *p_msg, *p_set_msg;
    new_trans_id = ++qcli_trans_id;
    msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
    p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

    if(OAM_NULL == p_msg)
    {
        /* Not enough memory */
        return OAM_FAILURE;
    }
    oam_memset(p_msg, OAM_ZERO, msg_api_length);
    p_set_msg = p_msg;
    /* Fill CSPL header */
    oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
            OAM_SET_LOG_ENABLE_X2AP_REQ,msg_api_length);

    /* Fill QCLI header */
    p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
            OAM_SET_LOG_ENABLE_X2AP_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

    oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
    return OAM_SUCCESS;
}


/*******************************************************************************
 * Function Name  : oam_log_enable_s1ap_pkt
 * Description    : To enable s1ap pkt
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_log_enable_s1ap_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

    UInt16 msg_api_length =  OAM_ZERO;
    UInt16 new_trans_id = OAM_ZERO;

    UInt8 *p_msg, *p_set_msg;
    new_trans_id = ++qcli_trans_id;
    msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
    p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

    if(OAM_NULL == p_msg)
    {
        /* Not enough memory */
        return OAM_FAILURE;
    }
    oam_memset(p_msg, OAM_ZERO, msg_api_length);
    p_set_msg = p_msg;
    /* Fill CSPL header */
    oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
            OAM_SET_LOG_ENABLE_S1AP_REQ,msg_api_length);

    /* Fill QCLI header */
    p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
            OAM_SET_LOG_ENABLE_S1AP_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

    oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
    return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_log_disable_rrc_pkt
 * Description    : To disable rrc pkt
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_log_disable_rrc_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_DISABLE_RRC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_DISABLE_RRC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_log_disable_x2ap_pkt
 * Description    : To disable x2ap pkt
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_log_disable_x2ap_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

    UInt16 msg_api_length =  OAM_ZERO;
    UInt16 new_trans_id = OAM_ZERO;

    UInt8 *p_msg, *p_set_msg;
    new_trans_id = ++qcli_trans_id;
    msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
    p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

    if(OAM_NULL == p_msg)
    {
        /* Not enough memory */
        return OAM_FAILURE;
    }
    oam_memset(p_msg, OAM_ZERO, msg_api_length);
    p_set_msg = p_msg;
    /* Fill CSPL header */
    oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
            OAM_SET_LOG_DISABLE_X2AP_REQ,msg_api_length);

    /* Fill QCLI header */
    p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
            OAM_SET_LOG_DISABLE_X2AP_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

    oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
    return OAM_SUCCESS;
}


/*******************************************************************************
 * Function Name  : oam_log_disable_s1ap_pkt
 * Description    : To disable s1ap pkt
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_log_disable_s1ap_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

    UInt16 msg_api_length =  OAM_ZERO;
    UInt16 new_trans_id = OAM_ZERO;

    UInt8 *p_msg, *p_set_msg;
    new_trans_id = ++qcli_trans_id;
    msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
    p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

    if(OAM_NULL == p_msg)
    {
        /* Not enough memory */
        return OAM_FAILURE;
    }
    oam_memset(p_msg, OAM_ZERO, msg_api_length);
    p_set_msg = p_msg;
    /* Fill CSPL header */
    oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
            OAM_SET_LOG_DISABLE_S1AP_REQ,msg_api_length);

    /* Fill QCLI header */
    p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
            OAM_SET_LOG_DISABLE_S1AP_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);

    oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
    return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_rrc_log_lvl_pkt
 * Description    : To rrc log lvl pkt 
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_rrc_log_lvl_pkt
(
 UInt16 C,
 setloglevell3rrccommands rrc_log_val
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */   

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_rrc_set_log_level_req_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_LEVEL_L3_RRC_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_LEVEL_L3_RRC_REQ,msg_api_length -OAM_CSPL_HDR_SIZE );

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_rrc_log_level_payload(p_set_msg,rrc_log_val);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_X2AP_log_lvl_pkt
 * Description    : To X2AP log lvl pkt 
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_X2AP_log_lvl_pkt
(
 UInt16 C,
 setloglevelX2APcommands rrc_log_val
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_X2AP_set_log_level_req_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_LEVEL_L3_X2AP_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_LEVEL_L3_X2AP_REQ,msg_api_length -OAM_CSPL_HDR_SIZE );

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
    /* Coverity Fix 96759 Start */
	oam_X2AP_log_level_payload(p_set_msg,rrc_log_val);
    /* Coverity Fix 96759 End */
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_S1AP_log_lvl_pkt
 * Description    : To S1AP log lvl pkt 
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_S1AP_log_lvl_pkt
(
 UInt16 C,
 setloglevelS1APcommands rrc_log_val
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_S1AP_set_log_level_req_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_LEVEL_L3_S1AP_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_LEVEL_L3_S1AP_REQ,msg_api_length -OAM_CSPL_HDR_SIZE );

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
    /* Coverity 96759 fix start */
	oam_S1AP_log_level_payload(p_set_msg,rrc_log_val);
    /* Coverity 96759 fix end */
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}


/*******************************************************************************
 * Function Name  : oam_qcli_send_rrm_set_log_level_cmd
 * Description    : To qcli send rrm set log level cmd
 * Inputs         : C - Number of parameters passed,rrm_log_val,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_qcli_send_rrm_set_log_level_cmd
(
 UInt16 C,
 rrm_oam_log_level_et rrm_log_val,
 Char8* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_rrm_set_log_level_req_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO, OAM_ZERO, OAM_ZERO, msg_api_length);

	if(OAM_NULL == p_msg) {
		oam_fprintf(stderr, "CRITICAL: Memory allocation failure!\n");
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,
			OAM_VERSION,
			QCLI_MODULE_ID,
			QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_LEVEL_RRM_REQ,
			msg_api_length);

	/* Fill QCLI header */
	p_set_msg += OAM_CSPL_HDR_SIZE;
	msg_api_length -= OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,
			new_trans_id,
			QCLI_MODULE_ID,
			QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_LEVEL_RRM_REQ,
			msg_api_length);

	/* Fill payload */
	p_set_msg += QCLI_HEADER_SIZE;
	oam_fill_rrm_log_lvl_payload(p_set_msg, rrm_log_val, V[1]);
	oam_qcli_intf_send_message(p_msg, QCLI_INTERFACE_MODULE_ID, &error_code);
	return OAM_SUCCESS;
}

#ifdef OAM_SON_ENABLE

/* SPR_13116_CHANGE_START */
/*******************************************************************************
 * Function Name  : oam_son_log_lvl_pkt
 * Description    : To son log lvl pkt
 * Inputs         : C - Number of parameters passed,son_log_val,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_son_get_log_lvl_pkt
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

    UInt16 msg_length = OAM_ZERO;
    UInt16 msg_api_length =  OAM_ZERO;
    UInt16 new_trans_id = OAM_ZERO;

    UInt8 *p_msg, *p_set_msg;
    new_trans_id = ++qcli_trans_id;
    msg_length = sizeof(oam_son_get_log_level_req_t);
    msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
    p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

    if(OAM_NULL == p_msg)
    {
        /* Not enough memory */
        return OAM_FAILURE;
    }
    oam_memset(p_msg, OAM_ZERO, msg_api_length);
    p_set_msg = p_msg;
    /* Fill CSPL header */
    oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
            OAM_GET_LOG_LEVEL_SON_REQ,msg_api_length);

    /* Fill QCLI header */
    p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
    oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
            OAM_GET_LOG_LEVEL_SON_REQ,msg_api_length -OAM_CSPL_HDR_SIZE );

    /* Fill payload */


    oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
    return OAM_SUCCESS;
}
/* SPR_13116_CHANGE_END */


/*******************************************************************************
 * Function Name  : oam_son_log_lvl_pkt
 * Description    : To son log lvl pkt
 * Inputs         : C - Number of parameters passed,son_log_val,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_son_log_lvl_pkt
(
 UInt16 C,
 setson son_log_val,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_son_set_log_level_req_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_LEVEL_SON_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_LEVEL_SON_REQ,msg_api_length -OAM_CSPL_HDR_SIZE );

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_son_log_lvl_payload(p_set_msg,son_log_val,(char *)V[1]);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
#endif

/****************************************************************************
 * Function Name  : oam_qcli_get_log_cat_MAC_resp
 * Inputs         : p_api_buf, msg_len
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function provide response for log cat request from MAC.
 ****************************************************************************/
oam_return_et oam_qcli_get_log_cat_MAC_resp(void* p_api_buf, U16 msg_len)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(msg_len);
    /* SPR 17777 FIX END */

	UInt32 index = OAM_ZERO;
	UInt64 oam_get_logcategory_mac;

	UInt64 log_category = OAM_ZERO;
    /*CSR:000555317_FIX_START*/
    UInt8 disp_cat_flag = OAM_FALSE;
    /*CSR:000555317_FIX_END*/

	oam_memcpy(&oam_get_logcategory_mac,(((UInt64 *)(p_api_buf +  OAM_MGMT_INTF_HEADER_SIZE))),sizeof(UInt64));

	if(OAM_ZERO == oam_get_logcategory_mac)
	{
		oam_fprintf(stdout,"No logCategory is enabled as Received from MAC\n");
	}
	else
	{
		for(index = MAC_MIN_LOG_CAT; index <= MAC_MAX_LOG_CAT ; index++)
		{
			log_category = oam_logcategory_table[index].string_val & oam_get_logcategory_mac;
			if(log_category == oam_logcategory_table[index].string_val)
			{
				oam_fprintf(stdout, " MAC LOG CATEGORY :[ %s ]\n",oam_logcategory_table[index].string_id);
                /*CSR:000555317_FIX_START*/
                disp_cat_flag=OAM_TRUE;
                /*CSR:000555317_FIX_END*/

				/* Coverity ID : 78992 Fix Start*/
                                /* if(index <= MAC_MAX_LOG_CAT ){
				oam_fprintf(stdout, " MAC LOG CATEGORY :[ %s ]\n",oam_logcategory_table[index].string_id);
                }
				else{
					oam_fprintf(stdout, "Invalid Log Category Received from MAC\n");
					break;
				}*/
				/* Coverity ID : 78992 Fix End*/
		}
        /*CSR:000555317_FIX_START*/
        if(OAM_FALSE==disp_cat_flag)
        {
            oam_fprintf(stdout,"No logCategory is enabled as Received from MAC\n");
        }
        /*CSR:000555317_FIX_END*/


	   }
    }
	return OAM_SUCCESS;
}

/****************************************************************************
 * fUNCTION nAMe  : oam_qcli_get_log_cat_RLC_resp
 * Inputs         : p_api_buf, msg_len
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function provide response for log cat request from RLC.
 ****************************************************************************/
oam_return_et oam_qcli_get_log_cat_RLC_resp(void* p_api_buf, U16 msg_len)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(msg_len);
    /* SPR 17777 FIX END */

	UInt32 index = OAM_ZERO;
	UInt64 oam_get_logcategory_rlc;
	UInt64 log_category;
	oam_memcpy(&oam_get_logcategory_rlc,(((UInt64 *)(p_api_buf + OAM_MGMT_INTF_HEADER_SIZE))),sizeof(UInt64));


	if(OAM_ZERO == oam_get_logcategory_rlc)
	{
		oam_fprintf(stdout,"\n No logCategory is enabled as Received from RLC");
	}
	else
	{
		for(index = RLC_MIN_LOG_CAT; index <= RLC_MAX_LOG_CAT ; index++)
		{
			log_category = oam_logcategory_table[index].string_val & oam_get_logcategory_rlc;
			if (log_category == oam_logcategory_table[index].string_val)
			{
					oam_fprintf(stdout, " RLC LOG CATEGORY :[ %s ]\n", oam_logcategory_table[index].string_id);
			}
			log_category=0;
		}

	}/*end else */

	return OAM_SUCCESS;
}


/****************************************************************************
 * Function Name  : oam_qcli_get_log_cat_PDCP_resp
 * Inputs         : p_api_buf, msg_len
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function provide response for log cat request from PDCP.
 ****************************************************************************/
oam_return_et oam_qcli_get_log_cat_PDCP_resp(void* p_api_buf, U16 msg_len)
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(msg_len);
    /* SPR 17777 FIX END */

	UInt32 index = OAM_ZERO;
	UInt64 oam_get_logcategory_pdcp;
	UInt64 log_category;
	oam_memcpy(&oam_get_logcategory_pdcp,(((UInt64 *)(p_api_buf + OAM_MGMT_INTF_HEADER_SIZE))),sizeof(UInt64));

	/* check loglevel is set or not*/
	if( OAM_ZERO == oam_get_logcategory_pdcp)
	{
		oam_fprintf(stdout,"No logCategory is enabled as Received"
				" from PDCP\n");
	}
	else
	{
		for(index = PDCP_MIN_LOG_CAT; index <= PDCP_MAX_LOG_CAT; index++)
		{
			log_category = oam_logcategory_table[index].string_val & oam_get_logcategory_pdcp;
			if (log_category == oam_logcategory_table[index].string_val)
			{
				if(PDCP_MIN_LOG_CAT <= index && index <= PDCP_MAX_LOG_CAT )
					oam_fprintf(stdout, " PDCP LOG CATEGORY :[ %s ]\n",oam_logcategory_table[index].string_id);
				else
				{
					oam_fprintf(stdout, "Invalid Log Category[%llu] received from PDCP\n", 
                    				log_category);
				}
			}
			log_category=0;
		}
	}

	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_qcli_get_log_cat_GTPU_resp
 * Inputs         : p_api_buf, msg_len
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function provide response for log cat request from GTPU.
 ****************************************************************************/
oam_return_et oam_qcli_get_log_cat_GTPU_resp(void* p_api_buf, U16 msg_len)
{
    /* SPR 17777 FIX START */
    /*  coverity 96704 fix start */
    qcliHandleUnusedParameter(msg_len);
    /*  coverity 96704 fix end */
    /* SPR 17777 FIX END */

	UInt32 index = OAM_ZERO;
	UInt64 oam_get_logcategory_pr_egtpu;
	oam_memcpy(&oam_get_logcategory_pr_egtpu,(((UInt64 *)(p_api_buf + 10))),sizeof(UInt64));

	/* check loglevel is set or not*/
	if( OAM_ZERO == oam_get_logcategory_pr_egtpu)
	{
		oam_fprintf(stdout,"No logCategory is enabled as Received"
				" from GTPU\n");
	}
	else
	{

		for(index = PR_EGTPU_MIN_LOG_CAT; index <= PR_EGTPU_MAX_LOG_CAT; index++)
		{
			if(oam_logcategory_table[index].string_val & oam_get_logcategory_pr_egtpu)
			{
					oam_fprintf(stdout, " GTPU LOG CATEGORY :[ %s ]\n", oam_logcategory_table[index].string_id);
			}
		}
	}
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_cell_cfg_sib1_ifr
 * Description    : To reconfig sib1 intra frequency reselection
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_cell_cfg_sib1_ifr
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_cellmgmt_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(UInt16);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_cellmgmt_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_cellmgmt_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_CELLMGT_RECONFIG_SIB1_INTRA_FREQ_REQ,msg_api_length);
	/* Fill QCLI header */
	p_cellmgmt_msg = p_cellmgmt_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_cellmgmt_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_CELLMGT_RECONFIG_SIB1_INTRA_FREQ_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_cellmgmt_msg = p_cellmgmt_msg + QCLI_HEADER_SIZE;
	UInt16 oam_param = (UInt16)oam_atoi(V[1]);
	oam_memcpy(p_cellmgmt_msg,&oam_param,sizeof(UInt16));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
	return OAM_SUCCESS;
}


/*******************************************************************************
 * Function Name  : oam_cell_cfg_sib1_mnc
 * Description    : To reconfig sib1 mnc
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_cell_cfg_sib1_mnc
(
 UInt16 C,
 char * const*V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_cellmgmt_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(UInt16);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_cellmgmt_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_cellmgmt_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_CELLMGT_RECONFIG_SIB1_MNC_REQ,msg_api_length);
	/* Fill QCLI header */
	p_cellmgmt_msg = p_cellmgmt_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_cellmgmt_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_CELLMGT_RECONFIG_SIB1_MNC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_cellmgmt_msg = p_cellmgmt_msg + QCLI_HEADER_SIZE;
	UInt16 oam_param = (UInt16)oam_atoi(V[1]);
	oam_memcpy(p_cellmgmt_msg,&oam_param,sizeof(UInt16));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_cell_cfg_sib1_mcc
 * Description    : To reconfig sib1 mcc
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_cell_cfg_sib1_mcc
(
 UInt16 C,
 char * const*V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_cellmgmt_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(UInt16);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_cellmgmt_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_cellmgmt_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_CELLMGT_RECONFIG_SIB1_MCC_REQ,msg_api_length);
	/* Fill QCLI header */
	p_cellmgmt_msg = p_cellmgmt_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_cellmgmt_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_CELLMGT_RECONFIG_SIB1_MCC_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_cellmgmt_msg = p_cellmgmt_msg + QCLI_HEADER_SIZE;
	UInt16 oam_param = (UInt16)oam_atoi(V[1]);
	oam_memcpy(p_cellmgmt_msg,&oam_param,sizeof(UInt16));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_cell_cfg_sib2_add_spect_emi
 * Description    : To reconfig sib2 additional spectrum emission
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_cell_cfg_sib2_add_spect_emi
(
 UInt16 C,
 char * const*V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_cellmgmt_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(UInt16);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_cellmgmt_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_cellmgmt_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_CELLMGT_RECONFIG_SIB2_ADD_SPEC_EMI_REQ,msg_api_length);
	/* Fill QCLI header */
	p_cellmgmt_msg = p_cellmgmt_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_cellmgmt_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_CELLMGT_RECONFIG_SIB2_ADD_SPEC_EMI_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_cellmgmt_msg = p_cellmgmt_msg + QCLI_HEADER_SIZE;
	UInt8 oam_param = (UInt8)oam_atoi(V[1]);
	oam_memcpy(p_cellmgmt_msg,&oam_param,sizeof(UInt8));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
	return OAM_SUCCESS;
}


/*******************************************************************************
 * Function Name  : oam_cell_cfg_sib2_dflt_pag_cyl
 * Description    : To reconfig sib2 default pageing cycle
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_cell_cfg_sib2_dflt_pag_cyl
(
 UInt16 C,
 char * const*V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_cellmgmt_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(UInt16);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_cellmgmt_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_cellmgmt_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_CELLMGT_RECONFIG_SIB2_DEF_PAG_CYL_REQ,msg_api_length);
	/* Fill QCLI header */
	p_cellmgmt_msg = p_cellmgmt_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_cellmgmt_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_CELLMGT_RECONFIG_SIB2_DEF_PAG_CYL_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_cellmgmt_msg = p_cellmgmt_msg + QCLI_HEADER_SIZE;
	UInt16 oam_param = (UInt16)oam_atoi(V[1]);
	oam_memcpy(p_cellmgmt_msg,&oam_param,sizeof(UInt16));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
	return OAM_SUCCESS;
}


/*******************************************************************************
 * Function Name  : oam_cell_cfg_sib2_mod_per_coeff
 * Description    : To reconfig sib2 modification perodic coeff
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_cell_cfg_sib2_mod_per_coeff
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_cellmgmt_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(UInt16);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_cellmgmt_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_cellmgmt_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_CELLMGT_RECONFIG_SIB2_DEF_MOD_PRD_COEFF_REQ,msg_api_length);
	/* Fill QCLI header */
	p_cellmgmt_msg = p_cellmgmt_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_cellmgmt_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_CELLMGT_RECONFIG_SIB2_DEF_MOD_PRD_COEFF_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_cellmgmt_msg = p_cellmgmt_msg + QCLI_HEADER_SIZE;
	UInt16 oam_param = (UInt16)oam_atoi(V[1]);
	oam_memcpy(p_cellmgmt_msg,&oam_param,sizeof(UInt16));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_cell_cfg_sib2_nb
 * Description    : To reconfig sib2 nB
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_cell_cfg_sib2_nb
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_cellmgmt_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(UInt16);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_cellmgmt_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_cellmgmt_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_CELLMGT_RECONFIG_SIB2_NB_REQ,msg_api_length);
	/* Fill QCLI header */
	p_cellmgmt_msg = p_cellmgmt_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_cellmgmt_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_CELLMGT_RECONFIG_SIB2_NB_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_cellmgmt_msg = p_cellmgmt_msg + QCLI_HEADER_SIZE;
	UInt16 oam_param = (UInt16)oam_atoi(V[1]);
	oam_memcpy(p_cellmgmt_msg,&oam_param,sizeof(UInt16));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_cell_cfg_sib2_time_align_timer
 * Description    : To reconfig sib2 time alignment timer common
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_cell_cfg_sib2_time_align_timer
(
 UInt16 C,
 char * const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_cellmgmt_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(UInt16);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);
	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_cellmgmt_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_cellmgmt_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_CELLMGT_RECONFIG_SIB2_TIME_ALGN_TIMER_REQ,msg_api_length);
	/* Fill QCLI header */
	p_cellmgmt_msg = p_cellmgmt_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_cellmgmt_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_CELLMGT_RECONFIG_SIB2_TIME_ALGN_TIMER_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	/* Fill Payload */
	p_cellmgmt_msg = p_cellmgmt_msg + QCLI_HEADER_SIZE;
	UInt16 oam_param = (UInt16)oam_atoi(V[1]);
	oam_memcpy(p_cellmgmt_msg,&oam_param,sizeof(UInt16));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_lvl_pkt
 * Description    : To reconfig oam_lvl_pkt time alignment timer common
 * Inputs         : C - Number of parameters passed,
 *                   oam_log_val 
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_lvl_pkt
(
 UInt16 C,
 UInt32 oam_log_val
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(UInt32);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_LEVEL_OAM_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_LEVEL_OAM_REQ,msg_api_length -OAM_CSPL_HDR_SIZE );

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	oam_log_level_payload(p_set_msg,oam_log_val);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function Name  : oam_incoming_lvl_pkt
 * Description    : To reconfig oam_incoming_lvl_pkt time alignment timer common
 * Inputs         : C - Number of parameters passed,
 *                  trace_flag,trace_val
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/

oam_return_et
oam_incoming_lvl_pkt
(
 UInt16 C,
 UInt8 trace_flag,
 UInt8 trace_val
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_set_msg;
	oam_relay_Set_Trace_Lvl_Req_t gtpu_trace_req;
	new_trans_id = ++qcli_trans_id;
	msg_length = sizeof(oam_relay_Set_Trace_Lvl_Req_t);
	msg_api_length = msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_set_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_set_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_LEVEL_GTPU_REQ,msg_api_length);

	/* Fill QCLI header */
	p_set_msg = p_set_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_set_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_LEVEL_GTPU_REQ,msg_api_length -OAM_CSPL_HDR_SIZE );

	/* Fill payload */
	p_set_msg = p_set_msg + QCLI_HEADER_SIZE;
	gtpu_trace_req.traceFlag = trace_flag;
	gtpu_trace_req.traceLevel = trace_val;
	oam_memcpy(p_set_msg,&gtpu_trace_req,sizeof(oam_relay_Set_Trace_Lvl_Req_t));
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}


/*******************************************************************************
 * Function Name  : oam_set_log_level_all_pkt
 * Description    : To serve the request for displaying the log level of all modules
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_set_log_level_all_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_show_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_show_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_show_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_LEVEL_REQ,msg_api_length);

	/* Fill QCLI header */
	p_show_msg = p_show_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_show_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_LEVEL_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_set_log_level_l2_all_pkt
 * Description    : To serve the request for displaying the log level of L2.
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_set_log_level_l2_all_pkt
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(V);
    /* SPR 17777 FIX END */

	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg, *p_show_msg;
	new_trans_id = ++qcli_trans_id;
	msg_api_length =  QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_show_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_show_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_LEVEL_L2_REQ,msg_api_length);

	/* Fill QCLI header */
	p_show_msg = p_show_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_show_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_SET_LOG_LEVEL_L2_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code);
	return OAM_SUCCESS;
}

/*******************************************************************************
 * Function Name  : oam_send_shutdown_reqest
 * Description    : To shutdown OAM LTE
 * Inputs         : C - Number of parameters passed,
 *                  V - Pointer to array of character strings
 * Outputs        : NONE
 * Returns        : NA
 *******************************************************************************/
oam_return_et
oam_send_shutdown_request
(
 UInt16 C,
 char* const *V
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */


	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 *p_msg = OAM_NULL, *p_show_msg = OAM_NULL;

	oam_error_code_et   error_code = NO_ERROR;
	UInt16 cmd_name_len, admin_node_len;
	UInt8 curr_pos = OAM_ZERO;
	UInt16  resvd1=0;
	UInt8 num_params = 0;

	cmd_name_len = oam_strlen(V[0]) + 1;
	admin_node_len = oam_strlen("System=") + cmd_name_len + 12;

	new_trans_id = ++qcli_trans_id;
	msg_api_length =  OAM_MGMT_INTF_HEADER_SIZE + OAM_CSPL_HDR_SIZE + admin_node_len ;


	if (OAM_FAILURE == oam_mem_alloc(msg_api_length,(void *)&p_msg,&error_code))
	{
		/* Not enough memory */
		oam_fprintf(stderr, "\nError in Memory Allocation with error code %d\n",error_code);

		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_show_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_show_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_CONTROL_COMMAND_REQ,msg_api_length);

	p_show_msg = p_show_msg + OAM_CSPL_HDR_SIZE;
	/* Fill QCLI header */
	oam_construct_qcli_header(p_show_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			OAM_CONTROL_COMMAND_REQ,msg_api_length);

	p_show_msg = p_show_msg + OAM_MGMT_INTF_HEADER_SIZE;


	p_show_msg[0]    =  (UInt8)(((UInt32 )admin_node_len) >> 8);
	p_show_msg[1]    =  (UInt8)(((UInt32 )admin_node_len));
	curr_pos+=2;

	p_show_msg[2]    =  (UInt8)(((UInt32 )resvd1) >> 8);
	p_show_msg[3]    =  (UInt8)(((UInt32 )resvd1));
	curr_pos+=2;

	oam_memcpy(p_show_msg+curr_pos, "System=", oam_strlen("System="));
	curr_pos+= oam_strlen("System=");
	oam_memcpy(p_show_msg+curr_pos, V[0], cmd_name_len);
	curr_pos+= cmd_name_len;

	*(p_show_msg + curr_pos) = num_params;
	curr_pos+=8;

	message_send.message_send = 1;
	message_send.apiid_expected = OAM_CONTROL_COMMAND_RESP;

    /* Coverity CID 85240 Fix Start */
    if(OAM_FAILURE == oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code))
    {
        return OAM_FAILURE;
    }
    /* Coverity CID 85240 Fix End */
	return OAM_SUCCESS;
}
/******************************************************************************
 *   FUNCTION NAME     : oam_rrc_parse_erab_estab_init_fail
 *   DESCRIPTION       : This function parses the RRC UECC OAM Erab Establish
 *                       Init Fail
 *   INPUTS            : p_oam_rrc_erab_estab_init_fail :RRC UECC OAM Erab
 Establish Init Fail
 *                     : p_src :Pointer to location for reading for parsing
 *                     : length_left :Length of message left to be read
 *                     : p_length_read :Length read
 *   OUTPUTS           : p_error_code :Error Code
 *   RETURNS           : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et
oam_rrc_parse_erab_estab_init_fail
(
 oam_rrc_erab_estab_init_fail_t *p_oam_rrc_erab_estab_init_fail,
 UInt8  *p_src,
 SInt32 length_left,
 SInt32 *p_length_read,
 oam_error_code_et *p_error_code
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(p_error_code);
    /* SPR 17777 FIX END */

	oam_counter_t loop_counter;

	*p_length_read = OAM_ZERO;

	oam_memset((void *)p_oam_rrc_erab_estab_init_fail, OAM_ZERO,
			sizeof(oam_rrc_erab_estab_init_fail_t));

	/* This function parses rrc_oam_cell_status */

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32*/
	{
		for (loop_counter = OAM_ZERO; loop_counter <
				ARRSIZE(p_oam_rrc_erab_estab_init_fail->
					erab_estabInitFailNbr_cause_radioNetwork); loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)&p_oam_rrc_erab_estab_init_fail->
					erab_estabInitFailNbr_cause_radioNetwork[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_estabInitFailNbr_cause_radioNetwork[]"
					);
			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32 */
	{
		for (loop_counter = OAM_ZERO; loop_counter <
				ARRSIZE(p_oam_rrc_erab_estab_init_fail->
					erab_estabInitFailNbr_cause_transport); loop_counter++)
		{

			oam_cp_unpack_UInt32((void*)&p_oam_rrc_erab_estab_init_fail->
					erab_estabInitFailNbr_cause_transport[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_estabInitFailNbr_cause_transport[]"
					);

			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32 */
	{
		for (loop_counter = OAM_ZERO; loop_counter <
				ARRSIZE(p_oam_rrc_erab_estab_init_fail->
					erab_estabInitFailNbr_cause_nas); loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)&p_oam_rrc_erab_estab_init_fail->
					erab_estabInitFailNbr_cause_nas[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_estabInitFailNbr_cause_nas[]"
					);

			*p_length_read += sizeof(UInt32);
		}
	}
	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32 */
	{
		for (loop_counter = OAM_ZERO; loop_counter <
				ARRSIZE(p_oam_rrc_erab_estab_init_fail->
					erab_estabInitFailNbr_cause_protocol); loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)&p_oam_rrc_erab_estab_init_fail->
					erab_estabInitFailNbr_cause_protocol[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_estabInitFailNbr_cause_protocol[]"
					);

			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32 */
	{
		for (loop_counter = OAM_ZERO;
				loop_counter < ARRSIZE(p_oam_rrc_erab_estab_init_fail->
					erab_estabInitFailNbr_cause_misc); loop_counter++)
		{

			oam_cp_unpack_UInt32((void*)&p_oam_rrc_erab_estab_init_fail->
					erab_estabInitFailNbr_cause_misc[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_estabInitFailNbr_cause_misc[]"
					);
			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read > length_left)
	{
		return OAM_FAILURE;
	}

	return(OAM_SUCCESS);
}

/******************************************************************************
 *   FUNCTION NAME     : oam_rrc_parse_erab_estab_add_fail
 *   DESCRIPTION       : This function parses the RRC UECC OAM Stat Erab Setup
 *   INPUTS            : p_oam_rrc_erab_estab_add_fail :RRC UECC OAM Erab
 : Establish Add Fail
 *                     : p_src :Pointer to location for reading for parsing
 *                     : length_left :Length of message left to be read
 *                     : p_length_read :Length read
 *   OUTPUTS           : p_error_code :Error Code
 *   RETURNS           : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et
oam_rrc_parse_erab_estab_add_fail
(
 oam_rrc_erab_estab_add_fail_t *p_oam_rrc_erab_estab_add_fail,
 UInt8  *p_src,
 SInt32 length_left,
 SInt32 *p_length_read,
 oam_error_code_et *p_error_code
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(p_error_code);
    /* SPR 17777 FIX END */

	oam_counter_t loop_counter;

	*p_length_read = OAM_ZERO;

	oam_memset((void *)p_oam_rrc_erab_estab_add_fail, OAM_ZERO,
			sizeof(oam_rrc_erab_estab_add_fail_t));/*COV_FIX*/
	/* This function parses rrc_oam_cell_status */


	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32 */
	{
		for (loop_counter = OAM_ZERO; loop_counter <
				ARRSIZE(p_oam_rrc_erab_estab_add_fail->
					erab_estabAddFailNbr_cause_radioNetwork); loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)&p_oam_rrc_erab_estab_add_fail->
					erab_estabAddFailNbr_cause_radioNetwork[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_estabAddFailNbr_cause_radioNetwork[]"
					);
			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32 */
	{
		for (loop_counter = OAM_ZERO; loop_counter <
				ARRSIZE(p_oam_rrc_erab_estab_add_fail->
					erab_estabAddFailNbr_cause_transport); loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)&p_oam_rrc_erab_estab_add_fail->
					erab_estabAddFailNbr_cause_transport[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_estabAddFailNbr_cause_transport[]"
					);

			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{


		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32*/
	{
		for (loop_counter = OAM_ZERO; loop_counter <
				ARRSIZE(p_oam_rrc_erab_estab_add_fail->
					erab_estabAddFailNbr_cause_nas); loop_counter++)
		{

			oam_cp_unpack_UInt32((void*)&p_oam_rrc_erab_estab_add_fail->
					erab_estabAddFailNbr_cause_nas[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_estabAddFailNbr_cause_nas[]"
					);

			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32 */
	{
		for (loop_counter = OAM_ZERO;
				loop_counter < ARRSIZE(p_oam_rrc_erab_estab_add_fail->
					erab_estabAddFailNbr_cause_protocol); loop_counter++)
		{

			oam_cp_unpack_UInt32((void*)&p_oam_rrc_erab_estab_add_fail->
					erab_estabAddFailNbr_cause_protocol[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_estabAddFailNbr_cause_protocol[]"
					);

			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32 */
	{
		for (loop_counter = OAM_ZERO;
				loop_counter < ARRSIZE(p_oam_rrc_erab_estab_add_fail->
					erab_estabAddFailNbr_cause_misc); loop_counter++)
		{

			oam_cp_unpack_UInt32((void*)&p_oam_rrc_erab_estab_add_fail->
					erab_estabAddFailNbr_cause_misc[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_estabAddFailNbr_cause_misc[]"
					);

			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read > length_left)
	{


		return OAM_FAILURE;
	}

	return(OAM_SUCCESS);
}

/******************************************************************************
 *   FUNCTION NAME     : oam_rrc_parse_erab_rel_enb_cause
 *   DESCRIPTION       : This function parses the RRC UECC OAM Erab Release
 *                       eNodeB Cause
 *   INPUTS            : p_oam_rrc_erab_rel_enb_cause :RRC UECC OAM Erab
 Release eNodeB Cause
 *                     : p_src :Pointer to location for reading for parsing
 *                     : length_left :Length of message left to be read
 *                     : p_length_read :Length read
 *   OUTPUTS           : p_error_code :Error Code
 *   RETURNS           : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et
oam_rrc_parse_erab_rel_enb_cause
(
 oam_rrc_erab_rel_enb_cause_t *p_oam_rrc_erab_rel_enb_cause,
 UInt8  *p_src,
 SInt32 length_left,
 SInt32 *p_length_read,
 oam_error_code_et *p_error_code
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(p_error_code);
    /* SPR 17777 FIX END */

	oam_counter_t loop_counter;

	*p_length_read = OAM_ZERO;

	oam_memset((void *)p_oam_rrc_erab_rel_enb_cause, OAM_ZERO,
			sizeof(oam_rrc_erab_rel_enb_cause_t));

	/* This function parses rrc_oam_cell_status */

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{


		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of basic type elements */
	{
		for (loop_counter = OAM_ZERO;
				loop_counter < ARRSIZE(p_oam_rrc_erab_rel_enb_cause->
					erab_relEnbNbr_cause_radioNetwork);
				loop_counter++)
		{

			oam_cp_unpack_UInt32((void*)&p_oam_rrc_erab_rel_enb_cause->
					erab_relEnbNbr_cause_radioNetwork[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_relEnbNbr_cause_radioNetwork[]"
					);

			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32 */
	{
		for (loop_counter = OAM_ZERO;
				loop_counter < ARRSIZE(p_oam_rrc_erab_rel_enb_cause->
					erab_relEnbNbr_cause_transport);
				loop_counter++)
		{

			oam_cp_unpack_UInt32((void*)&p_oam_rrc_erab_rel_enb_cause->
					erab_relEnbNbr_cause_transport[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_relEnbNbr_cause_transport[]"
					);

			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32*/
	{
		for (loop_counter = OAM_ZERO;
				loop_counter < ARRSIZE(p_oam_rrc_erab_rel_enb_cause->
					erab_relEnbNbr_cause_nas);
				loop_counter++)
		{

			oam_cp_unpack_UInt32((void*)&p_oam_rrc_erab_rel_enb_cause->
					erab_relEnbNbr_cause_nas[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_relEnbNbr_cause_nas[]"
					);

			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{


		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32 */
	{
		for (loop_counter = OAM_ZERO;
				loop_counter < ARRSIZE(p_oam_rrc_erab_rel_enb_cause->
					erab_relEnbNbr_cause_protocol);
				loop_counter++)
		{

			oam_cp_unpack_UInt32((void*)&p_oam_rrc_erab_rel_enb_cause->
					erab_relEnbNbr_cause_protocol[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_relEnbNbr_cause_protocol[]"
					);

			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32 */
	{
		for (loop_counter = OAM_ZERO;
				loop_counter < ARRSIZE(p_oam_rrc_erab_rel_enb_cause->
					erab_relEnbNbr_cause_misc);
				loop_counter++)
		{

			oam_cp_unpack_UInt32((void*)&p_oam_rrc_erab_rel_enb_cause->
					erab_relEnbNbr_cause_misc[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_relEnbNbr_cause_misc[]"
					);

			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read > length_left)
	{

		return OAM_FAILURE;
	}

	return(OAM_SUCCESS);
}

/******************************************************************************
 *   FUNCTION NAME     : oam_rrc_parse_rel_fail_cause
 *   DESCRIPTION       : This function parses the RRC UECC OAM Release Fail Cause
 *   INPUTS            : p_oam_rrc_rel_fail_cause :RRC UECC OAM Release
 *                       Fail Cause
 *                     : p_src :Pointer to location for reading for parsing
 *                     : length_left :Length of message left to be read
 *                     : p_length_read :Length read
 *   OUTPUTS           : p_error_code :Error Code
 *   RETURNS           : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et
oam_rrc_parse_rel_fail_cause
(
 oam_rrc_rel_fail_cause_t *p_oam_rrc_rel_fail_cause,
 UInt8  *p_src,
 SInt32 length_left,
 SInt32 *p_length_read,
 oam_error_code_et *p_error_code
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(p_error_code);
    /* SPR 17777 FIX END */


	oam_counter_t loop_counter;

	*p_length_read = OAM_ZERO;

	oam_memset((void *)p_oam_rrc_rel_fail_cause, OAM_ZERO,
			sizeof(oam_rrc_rel_fail_cause_t));

	/* This function parses rrc_oam_cell_status */

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32 */

	{
		for (loop_counter = OAM_ZERO; loop_counter <
				ARRSIZE(p_oam_rrc_rel_fail_cause->
					erab_relFailNbr_cause_radioNetwork);
				loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)&p_oam_rrc_rel_fail_cause->
					erab_relFailNbr_cause_radioNetwork[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_relFailNbr_cause_radioNetwork[]"
					);

			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32 */
	{
		for (loop_counter = OAM_ZERO; loop_counter < ARRSIZE
				(p_oam_rrc_rel_fail_cause->
				 erab_relFailNbr_cause_transport);
				loop_counter++)
		{

			oam_cp_unpack_UInt32((void*)&p_oam_rrc_rel_fail_cause->
					erab_relFailNbr_cause_transport[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_relFailNbr_cause_transport[]"
					);

			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32 */
	{
		for (loop_counter = OAM_ZERO; loop_counter < ARRSIZE
				(p_oam_rrc_rel_fail_cause->erab_relFailNbr_cause_nas);
				loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)&p_oam_rrc_rel_fail_cause->
					erab_relFailNbr_cause_nas[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_relFailNbr_cause_nas[]"
					);

			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32 */
	{
		for (loop_counter = OAM_ZERO; loop_counter < ARRSIZE(
					p_oam_rrc_rel_fail_cause->
					erab_relFailNbr_cause_protocol);
				loop_counter++)
		{

			oam_cp_unpack_UInt32((void*)&p_oam_rrc_rel_fail_cause->
					erab_relFailNbr_cause_protocol[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_relFailNbr_cause_protocol[]"
					);

			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32 */
	{
		for (loop_counter = OAM_ZERO; loop_counter < ARRSIZE(
					p_oam_rrc_rel_fail_cause->erab_relFailNbr_cause_misc);
				loop_counter++)
		{

			oam_cp_unpack_UInt32((void*)&p_oam_rrc_rel_fail_cause->
					erab_relFailNbr_cause_misc[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_relFailNbr_cause_misc[]"
					);

			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read > length_left)
	{

		return OAM_FAILURE;
	}

	return(OAM_SUCCESS);
}

/******************************************************************************
 *   FUNCTION NAME     : oam_rrc_parse_stat_erab_mod_fail_cause
 *   DESCRIPTION       : This function parses the RRC UECC OAM Stat Erab
 *                       Modification Failure Cause
 *   INPUTS            : p_oam_rrc_stat_erab_mod_fail_cause :RRC UECC OAM
 *                       Stat Erab Modification Failure Cause
 *                     : p_src :Pointer to location for reading for parsing
 *                     : length_left :Length of message left to be read
 *                     : p_length_read :Length read
 *   OUTPUTS           : p_error_code :Error Code
 *   RETURNS           : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et
oam_rrc_parse_stat_erab_mod_fail_cause
(
 rrc_uecc_oam_stat_erab_mod_fail_cause_t *p_oam_rrc_stat_erab_mod_fail_cause,
 UInt8  *p_src,
 SInt32 length_left,
 SInt32 *p_length_read,
 oam_error_code_et *p_error_code
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(p_error_code);
    /* SPR 17777 FIX END */

	oam_counter_t loop_counter;

	*p_length_read = OAM_ZERO;

	oam_memset((void *)p_oam_rrc_stat_erab_mod_fail_cause, OAM_ZERO,
			sizeof(rrc_uecc_oam_stat_erab_mod_fail_cause_t));

	/* This function parses rrc_oam_cell_status */


	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of basic type elements */
	{
		for (loop_counter = OAM_ZERO;
				loop_counter < ARRSIZE(p_oam_rrc_stat_erab_mod_fail_cause->
					erab_modQosFailNbr_cause_radioNetwork);
				loop_counter++)
		{

			oam_cp_unpack_UInt32((void*)&p_oam_rrc_stat_erab_mod_fail_cause->
					erab_modQosFailNbr_cause_radioNetwork[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_modQosFailNbr_cause_radioNetwork[]"
					);


			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32 */
	{
		for (loop_counter = OAM_ZERO;
				loop_counter < ARRSIZE(p_oam_rrc_stat_erab_mod_fail_cause->
					erab_modQosFailNbr_cause_transport);
				loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)&p_oam_rrc_stat_erab_mod_fail_cause->
					erab_modQosFailNbr_cause_transport[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_modQosFailNbr_cause_transport[]"
					);

			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32 */
	{
		for (loop_counter = OAM_ZERO;
				loop_counter < ARRSIZE(p_oam_rrc_stat_erab_mod_fail_cause->
					erab_modQosFailNbr_cause_nas);
				loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)&p_oam_rrc_stat_erab_mod_fail_cause->
					erab_modQosFailNbr_cause_nas[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_modQosFailNbr_cause_nas[]"
					);
			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}
	/* Parse OCTET_STRING FIXED of UInt32 */
	{
		for (loop_counter = OAM_ZERO;
				loop_counter < ARRSIZE(p_oam_rrc_stat_erab_mod_fail_cause->
					erab_modQosFailNbr_cause_protocol);
				loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)&p_oam_rrc_stat_erab_mod_fail_cause->
					erab_modQosFailNbr_cause_protocol[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_modQosFailNbr_cause_protocol[]"
					);

			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32 */
	{
		for (loop_counter = OAM_ZERO;
				loop_counter < ARRSIZE(p_oam_rrc_stat_erab_mod_fail_cause->
					erab_modQosFailNbr_cause_misc);
				loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)&p_oam_rrc_stat_erab_mod_fail_cause->
					erab_modQosFailNbr_cause_misc[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_modQosFailNbr_cause_misc[]"
					);
			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read > length_left) 
	{
		return OAM_FAILURE;
	}

	return(OAM_SUCCESS);
}

/******************************************************************************
 * * *   FUNCTION NAME     : oam_rrc_parse_erab_estabTime
 * * *   DESCRIPTION       : This function parses the erab_estabTime 
 * * *   INPUTS            : p_oam_rrc_erab_estabTime 
 * * *                     : p_src :Pointer to location for reading for parsing
 * * *                     : length_left :Length of message left to be read
 * * *                     : p_length_read :Length read
 * * *   OUTPUTS           : p_error_code :Error Code
 * * *   RETURNS           : OAM_SUCCESS/OAM_FAILURE
 * * ******************************************************************************/
oam_return_et
oam_rrc_parse_erab_estabTime
(
 oam_rrc_erab_estabTime_t *p_oam_rrc_erab_estabTime,
 UInt8  *p_src,
 SInt32 length_left,
 SInt32 *p_length_read,
 oam_error_code_et *p_error_code
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(p_error_code);
    /* SPR 17777 FIX END */

	OAM_FUN_ENTRY_TRACE();

	*p_length_read = OAM_ZERO;


	oam_memset((void *)p_oam_rrc_erab_estabTime, OAM_ZERO,
			sizeof(oam_rrc_erab_estabTime_t));/*COV_FIX*/

	/* This function parses rrc_oam_cell_status */

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stderr, "\nError: Incoming message damaged!\n");
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of basic type */
	oam_cp_unpack_UInt32(&p_oam_rrc_erab_estabTime->count,
			p_src + *p_length_read
			,"count"
			);
	*p_length_read += sizeof(UInt32);
	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stderr, "\nError: Incoming message damaged!\n");
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of basic type */
	oam_cp_unpack_UInt32(&p_oam_rrc_erab_estabTime->erab_estabTime_Sum,
			p_src + *p_length_read
			, "erab_estabTime_Sum"


			);
	*p_length_read += sizeof(UInt32);

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stderr, "\nError: Incoming message damaged!\n");
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	/* Parse/Unpack parameter of basic type */
	oam_cp_unpack_UInt32(&p_oam_rrc_erab_estabTime->erab_estabTime_Max,
			p_src + *p_length_read
			,"erab_estabTime_Max"
			);
	*p_length_read += sizeof(UInt32);

	if (*p_length_read < length_left)
	{
		oam_fprintf(stderr, "\nError: Incoming message damaged!\n");
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return(OAM_SUCCESS);

}


/******************************************************************************
 *   FUNCTION NAME     : oam_rrc_parse_stat_erab_setup
 *   DESCRIPTION       : This function parses the RRC UECC OAM Stat Erab Setup
 *   INPUTS            : p_oam_rrc_stat_erab_setup :RRC UECC OAM Stat
 *                       Erab Setup
 *                     : p_src :Pointer to location for reading for parsing
 *                     : length_left :Length of message left to be read
 *                     : p_length_read :Length read
 *   OUTPUTS           : p_error_code :Error Code
 *   RETURNS           : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et
oam_rrc_parse_stat_erab_setup
(
 oam_rrc_stat_erab_setup_t *p_oam_rrc_stat_erab_setup,
 UInt8  *p_src,
 SInt32 length_left,
 SInt32 *p_length_read,
 oam_error_code_et *p_error_code
 )
{
	SInt32 length_read = OAM_ZERO;
	oam_counter_t loop_counter;
	UInt8 index  = OAM_ZERO;

	*p_length_read = OAM_ZERO;

	oam_memset((void *)p_oam_rrc_stat_erab_setup, OAM_ZERO, sizeof(oam_rrc_stat_erab_setup_t));

	/* This function parses rrc_oam_cell_status */

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32 */
	{
		for (loop_counter = OAM_ZERO;
				loop_counter < ARRSIZE(p_oam_rrc_stat_erab_setup->
					erab_estabInitAttNbr_qci); loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)&p_oam_rrc_stat_erab_setup->
					erab_estabInitAttNbr_qci[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_estabInitAttNbr_qci[]"
					);
			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		return OAM_FAILURE;
	}
	/* Parse/Unpack parameter of UInt32 */
	oam_cp_unpack_UInt32(&p_oam_rrc_stat_erab_setup->erab_estabInitAttNbr_sum,
			p_src + *p_length_read
			, "erab_estabInitAttNbr_sum"
			);
	*p_length_read += sizeof(UInt32);

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32 */
	{
		for (loop_counter = OAM_ZERO;
				loop_counter < ARRSIZE(p_oam_rrc_stat_erab_setup->
					erab_estabInitSuccNbr_qci); loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)&p_oam_rrc_stat_erab_setup->
					erab_estabInitSuccNbr_qci[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_estabInitSuccNbr_qci[]"
					);
			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		return OAM_FAILURE;
	}
	/* Parse/Unpack parameter of UInt32 */
	oam_cp_unpack_UInt32(&p_oam_rrc_stat_erab_setup->
			erab_estabInitSuccNbr_sum, p_src + *p_length_read
			, "erab_estabInitSuccNbr_sum"
			);
	*p_length_read += sizeof(UInt32);

	if (OAM_FAILURE == oam_rrc_parse_erab_estab_init_fail(
				&p_oam_rrc_stat_erab_setup->erab_estab_init_fail_stats,
				p_src + *p_length_read,
				length_left - *p_length_read,
				&length_read,
				p_error_code))
	{
		return OAM_FAILURE;
	}

	*p_length_read += length_read;


	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of UInt32 */
	oam_cp_unpack_UInt32(&p_oam_rrc_stat_erab_setup->
			erab_estabInitFailNbr_sum, p_src + *p_length_read
			, "erab_estabInitFailNbr_sum"
			);

	*p_length_read += sizeof(UInt32);

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		return OAM_FAILURE;
	}
	/* Parse OCTET_STRING FIXED of UInt32 */
	{
		for (loop_counter = OAM_ZERO; loop_counter <
				ARRSIZE(p_oam_rrc_stat_erab_setup->erab_estabAddAttNbr_qci);
				loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)&p_oam_rrc_stat_erab_setup->
					erab_estabAddAttNbr_qci[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_estabAddAttNbr_qci[]"
					);
			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		return OAM_FAILURE;
	}
	/* Parse/Unpack parameter of UInt32*/
	oam_cp_unpack_UInt32(&p_oam_rrc_stat_erab_setup->erab_estabAddAttNbr_sum,
			p_src + *p_length_read
			, "erab_estabAddAttNbr_sum"
			);
	*p_length_read += sizeof(UInt32);

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32 */
	{
		for (loop_counter = OAM_ZERO;
				loop_counter < ARRSIZE(p_oam_rrc_stat_erab_setup->
					erab_estabAddSuccNbr_qci);
				loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)&p_oam_rrc_stat_erab_setup->
					erab_estabAddSuccNbr_qci[loop_counter],
					(void*)(p_src + *p_length_read)
					, "erab_estabAddSuccNbr_qci[]"
					);
			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of UInt32 */
	oam_cp_unpack_UInt32(&p_oam_rrc_stat_erab_setup->erab_estabAddSuccNbr_sum,
			p_src + *p_length_read
			, "erab_estabAddSuccNbr_sum"
			);

	*p_length_read += sizeof(UInt32);

	if (OAM_FAILURE == oam_rrc_parse_erab_estab_add_fail(
				&p_oam_rrc_stat_erab_setup->erab_estab_add_fail_stats,
				p_src + *p_length_read,
				length_left - *p_length_read,
				&length_read,
				p_error_code))
	{
		oam_fprintf(stdout,
				"\nError in function  oam_rrc_parse_erab_estab_add_fail() :error code :%d",
				*p_error_code);

		return OAM_FAILURE;
	}

	*p_length_read += length_read;

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of UInt32 */
	oam_cp_unpack_UInt32(&p_oam_rrc_stat_erab_setup->erab_estabAddFailNbr_sum,
			p_src + *p_length_read
			, "erab_estabAddFailNbr_sum"
			);

	*p_length_read += sizeof(UInt32);

	for(index = OAM_ZERO; index < RRC_MAX_QCI_INDEX; index++)
	{


		if (OAM_FAILURE == oam_rrc_parse_erab_estabTime(
					&p_oam_rrc_stat_erab_setup->erab_estabTime_qci[index],
					p_src + *p_length_read,
					length_left - *p_length_read,
					&length_read,
					p_error_code))
		{
			oam_fprintf(stderr, "\nInternal error!!!\n");
			OAM_FUN_EXIT_TRACE();
			return OAM_FAILURE;
		}


	}

	if (*p_length_read > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}
	return(OAM_SUCCESS);
}
/******************************************************************************
 *   FUNCTION NAME     : oam_rrc_parse_stat_erab_release
 *   DESCRIPTION       : This function parses the RRC UECC OAM Stat Erab Release
 *   INPUTS            : p_oam_rrc_stat_erab_release :RRC UECC OAM Stat
 *                     : Erab Release
 *                     : p_src :Pointer to location for reading for parsing
 *                     : length_left :Length of message left to be read
 *                     : p_length_read :Length read
 *   OUTPUTS           : p_error_code :Error Code
 *   RETURNS           : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/

oam_return_et
oam_rrc_parse_stat_erab_release
(
 oam_rrc_stat_erab_release_t *p_oam_rrc_stat_erab_release,
 UInt8  *p_src,
 SInt32 length_left,
 SInt32 *p_length_read,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t loop_counter;
	SInt32 length_read = OAM_ZERO;

	*p_length_read = OAM_ZERO;

	oam_memset((void *)p_oam_rrc_stat_erab_release, OAM_ZERO,
			sizeof(oam_rrc_stat_erab_release_t));

	/* This function parses rrc_oam_cell_status */


	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32 */
	{
		for (loop_counter = OAM_ZERO;
				loop_counter < ARRSIZE(p_oam_rrc_stat_erab_release->
					erab_relEnbNbr_qci); loop_counter++)
		{

			oam_cp_unpack_UInt32((void*)&p_oam_rrc_stat_erab_release->
					erab_relEnbNbr_qci[loop_counter], (void*)(p_src + *p_length_read)
					, "erab_relEnbNbr_qci[]"
					);


			*p_length_read += sizeof(UInt32);
		}
	}
	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of UInt32 */
	oam_cp_unpack_UInt32(&p_oam_rrc_stat_erab_release->
			erab_relEnbNbr_sum, p_src + *p_length_read
			, "erab_relEnbNbr_sum"
			);


	*p_length_read += sizeof(UInt32);
	if (OAM_FAILURE == oam_rrc_parse_erab_rel_enb_cause(
				&p_oam_rrc_stat_erab_release->erab_rel_enb_cause_stats,
				p_src + *p_length_read,
				length_left - *p_length_read,
				&length_read,
				p_error_code))
	{

		oam_fprintf(stdout,
				"\nError in function oam_rrc_parse_erab_rel_enb_cause() :error code :%d",
				*p_error_code);

		return OAM_FAILURE;
	}

	*p_length_read += length_read;

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32 */
	{
		for (loop_counter = OAM_ZERO;
				loop_counter < ARRSIZE(p_oam_rrc_stat_erab_release->
					erab_relAttNbr_qci); loop_counter++)
		{

			oam_cp_unpack_UInt32((void*)&p_oam_rrc_stat_erab_release->
					erab_relAttNbr_qci[loop_counter], (void*)(p_src + *p_length_read)
					, "erab_relAttNbr_qci[]"
					);

			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of UInt32 */
	oam_cp_unpack_UInt32(&p_oam_rrc_stat_erab_release->
			erab_relAttNbr_sum, p_src + *p_length_read
			, "erab_relAttNbr_sum"
			);

	*p_length_read += sizeof(UInt32);

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32 */
	{
		for (loop_counter = OAM_ZERO;
				loop_counter < ARRSIZE(p_oam_rrc_stat_erab_release->
					erab_relSuccNbr_qci); loop_counter++)
		{

			oam_cp_unpack_UInt32((void*)&p_oam_rrc_stat_erab_release->
					erab_relSuccNbr_qci[loop_counter], (void*)(p_src + *p_length_read)
					, "erab_relSuccNbr_qci[]"
					);

			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of UInt32 */
	oam_cp_unpack_UInt32(&p_oam_rrc_stat_erab_release->
			erab_relSuccNbr_sum, p_src + *p_length_read
			, "erab_relSuccNbr_sum"
			);
	*p_length_read += sizeof(UInt32);


	if (OAM_FAILURE == oam_rrc_parse_rel_fail_cause(
				&p_oam_rrc_stat_erab_release->rel_fail_cause_stats,
				p_src + *p_length_read,
				length_left - *p_length_read,
				&length_read,
				p_error_code))
	{
		oam_fprintf(stdout,
				"\nError in function oam_rrc_parse_rel_fail_cause() :error code :%d",
				*p_error_code);

		return OAM_FAILURE;
	}

	*p_length_read += length_read;

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of UInt32 */
	oam_cp_unpack_UInt32(&p_oam_rrc_stat_erab_release->
			erab_relFailNbr_sum, p_src + *p_length_read
			, "erab_relFailNbr_sum"
			);

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32 */
	{
		for (loop_counter = OAM_ZERO;
				loop_counter < ARRSIZE(p_oam_rrc_stat_erab_release->
					erab_relActNbr_qci); loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)&p_oam_rrc_stat_erab_release->
					erab_relActNbr_qci[loop_counter], (void*)(p_src + *p_length_read)
					, "erab_relActNbr_qci[]"
					);
			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of UInt32 */
	oam_cp_unpack_UInt32(&p_oam_rrc_stat_erab_release->
			erab_relActNbr_sum, p_src + *p_length_read
			, "erab_relActNbr_sum"
			);
	*p_length_read += sizeof(UInt32);

	if (*p_length_read > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}
	return(OAM_SUCCESS);
}

/******************************************************************************
 *   FUNCTION NAME     : oam_rrc_parse_stat_erab_modification
 *   DESCRIPTION       : This function parses the RRC UECC OAM Stat Erab
 *                       Modification
 *   INPUTS            : p_oam_rrc_stat_erab_modification :RRC UECC OAM
 *                       Stat Erab Modification
 *                     : p_src :Pointer to location for reading for parsing
 *                     : length_left :Length of message left to be read
 *                     : p_length_read :Length read
 *   OUTPUTS           : p_error_code :Error Code
 *   RETURNS           : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et
oam_rrc_parse_stat_erab_modification
(
 oam_rrc_stat_erab_modification_t *p_oam_rrc_stat_erab_modification,
 UInt8  *p_src,
 SInt32 length_left,
 SInt32 *p_length_read,
 oam_error_code_et *p_error_code
 )
{
	oam_counter_t loop_counter;
	SInt32 length_read = OAM_ZERO;

	*p_length_read = OAM_ZERO;

	oam_memset((void *)p_oam_rrc_stat_erab_modification, OAM_ZERO,
			sizeof(oam_rrc_stat_erab_modification_t));

	/* This function parses rrc_oam_cell_status */

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32*/
	{
		for (loop_counter = OAM_ZERO; loop_counter < ARRSIZE(
					p_oam_rrc_stat_erab_modification->erab_modQosAttNbr_qci);
				loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)&p_oam_rrc_stat_erab_modification->
					erab_modQosAttNbr_qci[loop_counter], (void*)(p_src + *p_length_read)
					,"erab_modQosAttNbr_qci[]"
					);
			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of UInt32 */
	oam_cp_unpack_UInt32(&p_oam_rrc_stat_erab_modification->
			erab_modQosAttNbr_sum, p_src + *p_length_read
			, "erab_modQosAttNbr_sum"
			);
	*p_length_read += sizeof(UInt32);

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32 */
	{
		for (loop_counter = OAM_ZERO; loop_counter < ARRSIZE(
					p_oam_rrc_stat_erab_modification->erab_modQosSuccNbr_qci);
				loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)&p_oam_rrc_stat_erab_modification->
					erab_modQosSuccNbr_qci[loop_counter], (void*)(p_src + *p_length_read)
					, "erab_modQosSuccNbr_qci[]"
					);
			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of UInt32*/
	oam_cp_unpack_UInt32(&p_oam_rrc_stat_erab_modification->
			erab_modQosSuccNbr_sum, p_src + *p_length_read
			, "erab_modQosSuccNbr_sum"
			);
	*p_length_read += sizeof(UInt32);

	if (OAM_FAILURE == oam_rrc_parse_stat_erab_mod_fail_cause(
				&p_oam_rrc_stat_erab_modification->erab_mod_fail_cause_stats,
				p_src + *p_length_read,
				length_left - *p_length_read,
				&length_read,
				p_error_code))
	{
		oam_fprintf(stdout,
				"\nError in function oam_rrc_parse_stat_erab_mod_fail_cause() :error code :%d",
				*p_error_code);

		return OAM_FAILURE;
	}

	*p_length_read += length_read;
	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left) 
	{
		oam_fprintf(stdout, "\nIncoming message damaged!\n");

		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of UInt32 */
	oam_cp_unpack_UInt32(&p_oam_rrc_stat_erab_modification->
			erab_modQosFailNbr_sum, p_src + *p_length_read
			, "erab_modQosFailNbr_sum"
			);


	*p_length_read += sizeof(UInt32);
	if (*p_length_read > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}
	return(OAM_SUCCESS);
}

/******************************************************************************
 *   FUNCTION NAME     : oam_rrc_parse_stat_erab_number
 *   DESCRIPTION       : This function parses the RRC UECC OAM Stat Erab Number
 *   INPUTS            : p_oam_rrc_stat_erab_number :RRC OAM
 *                       Stat Erab Number
 *                     : p_src :Pointer to location for reading for parsing
 *                     : length_left :Length of message left to be read
 *                     : p_length_read :Length read
 *   OUTPUTS           : p_error_code :Error Code
 *   RETURNS           : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et
oam_rrc_parse_stat_erab_number
(
 oam_rrc_stat_erab_number_t *p_oam_rrc_stat_erab_number,
 UInt8  *p_src,
 SInt32 length_left,
 SInt32 *p_length_read,
 oam_error_code_et *p_error_code
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(p_error_code);
    /* SPR 17777 FIX END */

	OAM_FUN_ENTRY_TRACE();

	*p_length_read = OAM_ZERO;

	oam_counter_t loop_counter;

	oam_memset((void *)p_oam_rrc_stat_erab_number, OAM_ZERO,
			sizeof(oam_rrc_stat_erab_number_t));

	/* This function parses rrc_oam_cell_status */

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stderr, "\nIncoming message damaged!!!\n");
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of UInt32*/
	{
		for (loop_counter = OAM_ZERO; loop_counter < ARRSIZE(
					p_oam_rrc_stat_erab_number->erabSimNumber_qci);
				loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)&p_oam_rrc_stat_erab_number->
					erabSimNumber_qci[loop_counter], (void*)(p_src + *p_length_read)
					,"erabSimNumber_qci[]"
					);
			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stderr, "\nIncoming message damaged!!!\n");
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of UInt32 */
	oam_cp_unpack_UInt32(&p_oam_rrc_stat_erab_number->erabSimNumber_sum,
			p_src + *p_length_read
			, "erabSimNumber_sum"
			);
	*p_length_read += sizeof(UInt32);

	if (*p_length_read < length_left)
	{
		oam_fprintf(stderr, "\nIncoming message damaged!!!\n");
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	OAM_FUN_EXIT_TRACE();
	return(OAM_SUCCESS);


}

/******************************************************************************
 *   FUNCTION NAME     : oam_rrc_parse_erab_related_stats
 *   DESCRIPTION       : This function parses the RRC Erab Related Stats
 *   INPUTS            : p_oam_rrc_erab_related_stats :RRC Erab related stats
 *                     : p_src :Pointer to location for reading for parsing
 *                     : length_left :Length of message left to be read
 *                     : p_length_read :Length read
 *   OUTPUTS           : p_error_code :Error Code
 *   RETURNS           : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et
oam_rrc_parse_erab_related_stats
(
 oam_rrc_erab_related_stats_t *p_oam_rrc_erab_related_stats,
 UInt8  *p_src,
 SInt32 length_left,
 SInt32 *p_length_read,
 oam_error_code_et *p_error_code
 )
{
	SInt32 length_read = OAM_ZERO;

	*p_length_read = OAM_ZERO;

	oam_memset((void *)p_oam_rrc_erab_related_stats, OAM_ZERO,
			sizeof(oam_rrc_erab_related_stats_t));

	/* This function parses rrc_erab_related_stats */

	if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of basic type */
	oam_cp_unpack_UInt16(&p_oam_rrc_erab_related_stats->bitmask,
			p_src + *p_length_read
			, "bitmask"
			);


	*p_length_read += sizeof(UInt16);

	if(p_oam_rrc_erab_related_stats->bitmask & RRC_OAM_STATISTICS_ERAB_SETUP_STATS_PRESENT ) 
	{


		if (OAM_FAILURE == oam_rrc_parse_stat_erab_setup(
					&p_oam_rrc_erab_related_stats->erab_setup_stats,
					p_src + *p_length_read,
					length_left - *p_length_read,
					&length_read,
					p_error_code))
		{
			oam_fprintf(stdout,
					"\nError in function oam_rrc_parse_stat_erab_setup :error code :%d",
					*p_error_code);

			return OAM_FAILURE;
		}

		*p_length_read += length_read;
		length_read = OAM_ZERO; 

	}

	if(p_oam_rrc_erab_related_stats->bitmask & RRC_OAM_STATISTICS_ERAB_RELEASE_STATS_PRESENT )
	{

		if (OAM_FAILURE == oam_rrc_parse_stat_erab_release(
					&p_oam_rrc_erab_related_stats->erab_release_stats,
					p_src + *p_length_read,
					length_left - *p_length_read,
					&length_read,
					p_error_code))
		{
			oam_fprintf(stdout,
					"\nError in function oam_rrc_parse_stat_erab_release:error code :%d",
					*p_error_code);

			return OAM_FAILURE;
		}

		*p_length_read += length_read;
		length_read = OAM_ZERO; 

	}

	if(p_oam_rrc_erab_related_stats->bitmask & RRC_OAM_STATISTICS_ERAB_MODIFY_STATS_PRESENT )
	{
		if (OAM_FAILURE == oam_rrc_parse_stat_erab_modification(
					&p_oam_rrc_erab_related_stats->erab_mod_stats,
					p_src + *p_length_read,
					length_left - *p_length_read,
					&length_read,
					p_error_code))
		{
			oam_fprintf(stdout,
					"\nError in function oam_rrc_parse_stat_erab_modification :error code :%d",
					*p_error_code);

			return OAM_FAILURE;
		}

		*p_length_read += length_read;
		length_read = OAM_ZERO; 
	}

	if(p_oam_rrc_erab_related_stats->bitmask &RRC_OAM_STATISTICS_ERAB_NUMBER_STATS_PRESENT  )
	{
		if (OAM_FAILURE == oam_rrc_parse_stat_erab_number(
					&p_oam_rrc_erab_related_stats->erab_sim_num_stats,
					p_src + *p_length_read,
					length_left - *p_length_read,
					&length_read,
					p_error_code))
		{
			oam_fprintf(stdout,
					"\nError in function oam_rrc_parse_stat_erab_number :error code :%d",
					*p_error_code);

			return OAM_FAILURE;
		}

		*p_length_read += length_read;
		length_read = OAM_ZERO; 
	}
	if (*p_length_read > length_left) 
	{
		oam_fprintf(stdout, "Incoming message damaged!");
		return OAM_FAILURE;
	}


	return(OAM_SUCCESS);
}

/******************************************************************************
 *   FUNCTION NAME     : oam_rrc_parse_stat_ue_assoc_s1_connection
 *   DESCRIPTION       : This function parses the RRC UECC OAM Stat UE
 *                       Association S1 Connection
 *   INPUTS            : p_oam_rrc_stat_erab_mod_fail_cause :RRC UECC OAM
 *                       Stat UE Association S1 Connection
 *                     : p_src :Pointer to location for reading for parsing
 *                     : length_left :Length of message left to be read
 *                     : p_length_read :Length read
 *   OUTPUTS           : p_error_code :Error Code
 *   RETURNS           : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et
oam_rrc_parse_stat_ue_assoc_s1_connection
(
 oam_rrc_stat_ue_assoc_s1_connection_t
 *p_oam_rrc_stat_ue_assoc_s1_connection,
 UInt8  *p_src,
 SInt32 length_left,
 SInt32 *p_length_read,
 oam_error_code_et *p_error_code
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(p_error_code);
    /* SPR 17777 FIX END */


	*p_length_read = OAM_ZERO;

	oam_memset((void *)p_oam_rrc_stat_ue_assoc_s1_connection, OAM_ZERO,
			sizeof(oam_rrc_stat_ue_assoc_s1_connection_t));

	/* This function parses rrc_oam_cell_status */

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of UInt32 */
	oam_cp_unpack_UInt32(&p_oam_rrc_stat_ue_assoc_s1_connection->
			s1sig_connEstabAtt, p_src + *p_length_read
			, "s1sig_connEstabAtt"
			);
	*p_length_read += sizeof(UInt32);

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of UInt32 */
	oam_cp_unpack_UInt32(&p_oam_rrc_stat_ue_assoc_s1_connection->
			s1sig_connEstabSucc, p_src + *p_length_read
			, "s1sig_connEstabSucc"
			);
	*p_length_read += sizeof(UInt32);

	if (*p_length_read > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	return(OAM_SUCCESS);
}

/******************************************************************************
 *   FUNCTION NAME     : oam_rrc_parse_csc_stat_paging
 *   DESCRIPTION       : This function parses the RRC CSC OAM Stat Paging
 *   INPUTS            : p_oam_rrc_csc_stat_paging :RRC CSC OAM Stat Paging
 *                     : p_src :Pointer to location for reading for parsing
 *                     : length_left :Length of message left to be read
 *                     : p_length_read :Length read
 *   OUTPUTS           : p_error_code :Error Code
 *   RETURNS           : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et
oam_rrc_parse_csc_stat_paging
(
 oam_rrc_csc_stat_paging_t *p_oam_rrc_csc_stat_paging,
 UInt8  *p_src,
 SInt32 length_left,
 SInt32 *p_length_read,
 oam_error_code_et *p_error_code
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(p_error_code);
    /* SPR 17777 FIX END */


	*p_length_read = OAM_ZERO;

	oam_memset((void *)p_oam_rrc_csc_stat_paging, OAM_ZERO,
			sizeof(rrc_csc_oam_stat_paging_t));

	/* This function parses rrc_oam_cell_status */

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of UInt32*/
	oam_cp_unpack_UInt32(&p_oam_rrc_csc_stat_paging->pag_discardedNbr,
			p_src + *p_length_read
			, "pag_discardedNbr"
			);


	*p_length_read += sizeof(UInt32);

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of UInt32 */
	oam_cp_unpack_UInt32(&p_oam_rrc_csc_stat_paging->pag_succNbr,
			p_src + *p_length_read
			, "pag_succNbr"
			);

	*p_length_read += sizeof(UInt32);

	if (*p_length_read > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	return(OAM_SUCCESS);
}

/******************************************************************************
 *  FUNCTION NAME     : oam_rrc_parse_stat_connection_reestablishment
 *  DESCRIPTION       : This function parses the RRC UECC OAM Stat Connection
 *                      Reestablishment
 *  INPUTS            : p_oam_rrc_stat_connection_reestablishment :RRC UECC
 *                      OAM Stat Connection Establishment
 *                    : p_src :Pointer to location for reading for parsing
 *                    : length_left :Length of message left to be read
 *                    : p_length_read :Length read
 *  OUTPUTS           : p_error_code :Error Code
 *  RETURNS           : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et
oam_rrc_parse_stat_connection_reestablishment
(
 oam_rrc_stat_connection_reestablishment_t
 *p_oam_rrc_stat_connection_reestablishment,
 UInt8  *p_src,
 SInt32 length_left,
 SInt32 *p_length_read,
 oam_error_code_et *p_error_code
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(p_error_code);
    /* SPR 17777 FIX END */

	oam_counter_t loop_counter;

	*p_length_read = OAM_ZERO;

	oam_memset((void *)p_oam_rrc_stat_connection_reestablishment, OAM_ZERO,
			sizeof(oam_rrc_stat_connection_reestablishment_t));

	/* This function parses rrc_oam_cell_status */

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!\n");

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of basic type elements */
	{
		for (loop_counter = OAM_ZERO; loop_counter < ARRSIZE(
					p_oam_rrc_stat_connection_reestablishment->
					rrc_connReEstabAtt_cause);
				loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)&p_oam_rrc_stat_connection_reestablishment->
					rrc_connReEstabAtt_cause[loop_counter],
					(void*)(p_src + *p_length_read)
					, "rrc_connReEstabAtt_cause[]"
					);
			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!\n");

		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of basic type */
	oam_cp_unpack_UInt32(&p_oam_rrc_stat_connection_reestablishment->
			rrc_connReEstabAtt_sum, p_src + *p_length_read
			, "rrc_connReEstabAtt_sum"
			);
	*p_length_read += sizeof(UInt32);

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of basic type elements */
	{
		for (loop_counter = OAM_ZERO; loop_counter < ARRSIZE(
					p_oam_rrc_stat_connection_reestablishment->rrc_connReEstabSucc_cause);
				loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)
					&p_oam_rrc_stat_connection_reestablishment->
					rrc_connReEstabSucc_cause[loop_counter],
					(void*)(p_src + *p_length_read)
					, "rrc_connReEstabSucc_cause[]"
					);
			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of basic type */
	oam_cp_unpack_UInt32(&p_oam_rrc_stat_connection_reestablishment->
			rrc_connReEstabSucc_sum, p_src + *p_length_read
			, "rrc_connReEstabSucc_sum"
			);
	*p_length_read += sizeof(UInt32);
	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of basic type elements */
	{
		for (loop_counter = OAM_ZERO; loop_counter < ARRSIZE(
					p_oam_rrc_stat_connection_reestablishment->rrc_connReEstabFail_cause);
				loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)
					&p_oam_rrc_stat_connection_reestablishment->
					rrc_connReEstabFail_cause[loop_counter],
					(void*)(p_src + *p_length_read)
					, "rrc_connReEstabFail_cause[]"
					);
			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of basic type */
	oam_cp_unpack_UInt32(&p_oam_rrc_stat_connection_reestablishment->
			rrc_connReEstabFail_sum, p_src + *p_length_read
			, "rrc_connReEstabFail_sum"
			);

	*p_length_read += sizeof(UInt32);

	if (*p_length_read > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	return(OAM_SUCCESS);
}

/******************************************************************************
 *   FUNCTION NAME     : oam_rrc_parse_ue_ctxt_rel_req_fail_cause
 *   DESCRIPTION       : This function parses the RRC UECC OAM UE Context Release
 *                       Request Fail Cause
 *   INPUTS            : p_oam_rrc_ue_ctxt_rel_req_fail_cause :RRC UECC OAM UE
 *                       Context Release Request Fail Cause
 *                     : p_src :Pointer to location for reading for parsing
 *                     : length_left :Length of message left to be read
 *                     : p_length_read :Length read
 *   OUTPUTS           : p_error_code :Error Code
 *   RETURNS           : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et
oam_rrc_parse_ue_ctxt_rel_req_fail_cause
(
 oam_rrc_ue_ctxt_rel_req_fail_cause_t *p_oam_rrc_ue_ctxt_rel_req_fail_cause,
 UInt8  *p_src,
 SInt32 length_left,
 SInt32 *p_length_read,
 oam_error_code_et *p_error_code
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(p_error_code);
    /* SPR 17777 FIX END */

	oam_counter_t loop_counter;

	*p_length_read = OAM_ZERO;

	oam_memset((void *)p_oam_rrc_ue_ctxt_rel_req_fail_cause, OAM_ZERO,
			sizeof(oam_rrc_ue_ctxt_rel_req_fail_cause_t));

	/* This function parses rrc_uecc_oam_ue_ctxt_rel_req_fail_cause */

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of basic type elements */
	{
		for (loop_counter = OAM_ZERO;
				loop_counter < ARRSIZE(p_oam_rrc_ue_ctxt_rel_req_fail_cause->
					ueCtxt_relReq_cause_radioNetwork);
				loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)&p_oam_rrc_ue_ctxt_rel_req_fail_cause->
					ueCtxt_relReq_cause_radioNetwork[loop_counter],
					(void*)(p_src + *p_length_read)
					, "ueCtxt_relReq_cause_radioNetwork[]"
					);
			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of basic type elements */
	{
		for (loop_counter = OAM_ZERO;
				loop_counter < ARRSIZE(p_oam_rrc_ue_ctxt_rel_req_fail_cause->
					ueCtxt_relReq_cause_transport);
				loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)&p_oam_rrc_ue_ctxt_rel_req_fail_cause->
					ueCtxt_relReq_cause_transport[loop_counter],
					(void*)(p_src + *p_length_read)
					, "ueCtxt_relReq_cause_transport[]"
					);


			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of basic type elements */
	{
		for (loop_counter = OAM_ZERO;
				loop_counter < ARRSIZE(p_oam_rrc_ue_ctxt_rel_req_fail_cause->
					ueCtxt_relReq_cause_nas); loop_counter++)
		{
			oam_cp_unpack_UInt32(
					(void*)&p_oam_rrc_ue_ctxt_rel_req_fail_cause->
					ueCtxt_relReq_cause_nas[loop_counter],
					(void*)(p_src + *p_length_read)
					, "ueCtxt_relReq_cause_nas[]"
					);
			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}
	/* Parse OCTET_STRING FIXED of basic type elements */
	{
		for (loop_counter = OAM_ZERO;
				loop_counter < ARRSIZE(p_oam_rrc_ue_ctxt_rel_req_fail_cause->
					ueCtxt_relReq_cause_protocol); loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)&p_oam_rrc_ue_ctxt_rel_req_fail_cause->
					ueCtxt_relReq_cause_protocol[loop_counter],
					(void*)(p_src + *p_length_read)
					, "ueCtxt_relReq_cause_protocol[]"
					);
			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of basic type elements */
	{
		for (loop_counter = OAM_ZERO;
				loop_counter < ARRSIZE(p_oam_rrc_ue_ctxt_rel_req_fail_cause->
					ueCtxt_relReq_cause_misc); loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)&p_oam_rrc_ue_ctxt_rel_req_fail_cause->
					ueCtxt_relReq_cause_misc[loop_counter],
					(void*)(p_src + *p_length_read)
					, "ueCtxt_relReq_cause_misc[]"
					);

			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	return(OAM_SUCCESS);
}

/******************************************************************************
 *   FUNCTION NAME     : oam_rrc_parse_stat_ue_context_release
 *   DESCRIPTION       : This function parses the RRC UECC OAM Stat UE Context
 *                       Release
 *   INPUTS            : p_oam_rrc_stat_ue_context_release:RRC UECC OAM Stat
 *                       UE Context Release
 *                     : p_src :Pointer to location for reading for parsing
 *                     : length_left :Length of message left to be read
 *                     : p_length_read : Length read
 *   OUTPUTS           : p_error_code :Error Code
 *   RETURNS           : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et
oam_rrc_parse_stat_ue_context_release
(
 oam_rrc_stat_ue_context_release_t *p_oam_rrc_stat_ue_context_release,
 UInt8  *p_src,
 SInt32 length_left,
 SInt32 *p_length_read,
 oam_error_code_et *p_error_code
 )
{
	SInt32 length_read = OAM_ZERO;

	*p_length_read = OAM_ZERO;

	oam_memset((void *)p_oam_rrc_stat_ue_context_release, OAM_ZERO,
			sizeof(oam_rrc_stat_ue_context_release_t));



	if (OAM_FAILURE == oam_rrc_parse_ue_ctxt_rel_req_fail_cause(
				&p_oam_rrc_stat_ue_context_release->ueCtxt_relReq_cause,
				p_src + *p_length_read,
				length_left - *p_length_read,
				&length_read,
				p_error_code))
	{

		oam_fprintf(stdout,
				"\nError in function oam_rrc_parse_ue_ctxt_rel_req_fail_cause() :error code :%d",
				*p_error_code);

		return OAM_FAILURE;
	}

	*p_length_read += length_read;

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of basic type */
	oam_cp_unpack_UInt32(&p_oam_rrc_stat_ue_context_release->ueCtxt_relReq_sum
			, p_src + *p_length_read
			, "ueCtxt_relReq_sum"
			);

	*p_length_read += sizeof(UInt32);

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of basic type */
	oam_cp_unpack_UInt32(&p_oam_rrc_stat_ue_context_release->
			ueContext_RelSuccNbr, p_src + *p_length_read
			, "ueContext_RelSuccNbr"
			);


	*p_length_read += sizeof(UInt32);

	if (*p_length_read > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	return(OAM_SUCCESS);
}

/******************************************************************************
 *   FUNCTION NAME     : oam_rrc_parse_stat_connection_establishment
 *   DESCRIPTION       : This function parses the RRC UECC OAM Stat Connection
 *                       Establishment
 *   INPUTS            : p_oam_rrc_stat_connection_establishment :RRC UECC
 OAM Stat Connection Establishment
 *                     : p_src :Pointer to location for reading for parsing
 *                     : length_left :Length of message left to be read
 *                     : p_length_read :Length read
 *   OUTPUTS           : p_error_code :Error Code
 *   RETURNS           : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/

oam_return_et
oam_rrc_parse_stat_connection_establishment
(
 oam_rrc_stat_connection_establishment_t *p_oam_rrc_stat_connection_establishment,
 UInt8  *p_src,
 SInt32 length_left,
 SInt32 *p_length_read,
 oam_error_code_et *p_error_code
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(p_error_code);
    /* SPR 17777 FIX END */

	oam_counter_t loop_counter;
	*p_length_read = OAM_ZERO;

	oam_memset((void *)p_oam_rrc_stat_connection_establishment,
			OAM_ZERO, sizeof(oam_rrc_stat_connection_establishment_t));

	/* This function parses rrc_oam_cell_status */
	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of basic type elements */
	{
		for (loop_counter = OAM_ZERO;
				loop_counter < ARRSIZE(p_oam_rrc_stat_connection_establishment->
					rrc_connEstabAtt_cause); loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)&p_oam_rrc_stat_connection_establishment->
					rrc_connEstabAtt_cause[loop_counter],
					(void*)(p_src + *p_length_read)
					, "rrc_connEstabAtt_cause[]"
					);
			*p_length_read += sizeof(UInt32);
		}
	}
	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of basic type */
	oam_cp_unpack_UInt32(&p_oam_rrc_stat_connection_establishment->
			rrc_connEstabAtt_sum, p_src + *p_length_read
			, "rrc_connEstabAtt_sum"
			);
	*p_length_read += sizeof(UInt32);

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of basic type elements */
	{
		for (loop_counter = OAM_ZERO; loop_counter < ARRSIZE(
					p_oam_rrc_stat_connection_establishment->
					rrc_connEstabSucc_cause); loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)&p_oam_rrc_stat_connection_establishment->
					rrc_connEstabSucc_cause[loop_counter], (void*)
					(p_src + *p_length_read)
					, "rrc_connEstabSucc_cause[]"
					);
			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of basic type */
	oam_cp_unpack_UInt32(&p_oam_rrc_stat_connection_establishment->
			rrc_connEstabSucc_sum, p_src + *p_length_read
			, "rrc_connEstabSucc_sum"
			);
	*p_length_read += sizeof(UInt32);

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse OCTET_STRING FIXED of basic type elements */
	{
		for (loop_counter = OAM_ZERO; loop_counter < ARRSIZE(
					p_oam_rrc_stat_connection_establishment->rrc_connEstabFail_cause);
				loop_counter++)
		{
			oam_cp_unpack_UInt32((void*)
					&p_oam_rrc_stat_connection_establishment->
					rrc_connEstabFail_cause[loop_counter],
					(void*)(p_src + *p_length_read)
					, "rrc_connEstabFail_cause[]"
					);
			*p_length_read += sizeof(UInt32);
		}
	}

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of basic type */
	oam_cp_unpack_UInt32(&p_oam_rrc_stat_connection_establishment->
			rrc_connEstabFail_sum, p_src + *p_length_read
			, "rrc_connEstabFail_sum"
			);
	*p_length_read += sizeof(UInt32);

	if (*p_length_read > length_left)
	{ 
		oam_fprintf(stdout, "ANN.Incoming message damaged!\n");

		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of basic type */
	oam_cp_unpack_UInt32(&p_oam_rrc_stat_connection_establishment->
			rrc_connEstabFaileNBCause_Unspecified, p_src + *p_length_read
			, "rrc_connEstabFaileNBCause_Unspecified"
			);

	*p_length_read += sizeof(UInt32);

	if (*p_length_read > length_left)
	{
		oam_fprintf(stdout, "ANN.Incoming message damaged!\n");

		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of basic type */
	oam_cp_unpack_UInt32(&p_oam_rrc_stat_connection_establishment->
			rrc_ConnEstabFaileNBCause_Congestion, p_src + *p_length_read
			, "rrc_ConnEstabFaileNBCause_Congestion"
			);

	*p_length_read += sizeof(UInt32);

	if (*p_length_read > length_left)
	{
		oam_fprintf(stdout, "ANN.Incoming message damaged!\n");

		return OAM_FAILURE;
	}

	return(OAM_SUCCESS);
}
/******************************************************************************
 * FUNCTION NAME     : oam_rrc_parse_stat_conn_setup_time
 * DESCRIPTION       : This function parses the connEstabTime 
 * INPUTS            : p_oam_rrc_connEstabTime 
 *                   : p_src :Pointer to location for reading for parsing
 *                   : length_left :Length of message left to be read
 *                   : p_length_read :Length read
 * OUTPUTS           : p_error_code :Error Code
 * RETURNS           : OAM_SUCCESS/OAM_FAILURE
 **********************************************************************************/
oam_return_et
oam_rrc_parse_stat_conn_setup_time
(
 oam_rrc_connEstabTime_t *p_oam_rrc_connEstabTime,
 UInt8  *p_src,
 SInt32 length_left,
 SInt32 *p_length_read,
 oam_error_code_et *p_error_code
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(p_error_code);
    /* SPR 17777 FIX END */


	*p_length_read = OAM_ZERO;


	oam_memset((void *)p_oam_rrc_connEstabTime, OAM_ZERO,
			sizeof(oam_rrc_connEstabTime_t));

	/* This function parses rrc_oam_cell_status */
	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stderr, "\nIncoming message damaged!!!\n");
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of basic type */
	oam_cp_unpack_UInt32(&p_oam_rrc_connEstabTime->count,
			p_src + *p_length_read
			,"count"
			);
	*p_length_read += sizeof(UInt32);

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stderr, "\nIncoming message damaged!!!\n");
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of basic type */
	oam_cp_unpack_UInt32(&p_oam_rrc_connEstabTime->rrc_connEstabTime_Sum,
			p_src + *p_length_read
			, "rrc_connEstabTime_Sum"


			);
	*p_length_read += sizeof(UInt32);

	if (*p_length_read + (SInt32)sizeof(UInt32) > length_left)
	{
		oam_fprintf(stderr, "\nIncoming message damaged!!!\n");
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	/* Parse/Unpack parameter of basic type */
	oam_cp_unpack_UInt32(&p_oam_rrc_connEstabTime->rrc_connEstabTime_Max,
			p_src + *p_length_read
			,"rrc_connEstabTime_Max"
			);
	*p_length_read += sizeof(UInt32);

	if (*p_length_read > length_left)
	{
		oam_fprintf(stderr, "\nIncoming message damaged!\n");
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	return(OAM_SUCCESS);

}
/******************************************************************************
 *   FUNCTION NAME     : oam_rrc_parse_connection_related_stats
 *   DESCRIPTION       : This function parses the RRC Connection Related Stats
 *   INPUTS            : p_oam_rrc_connection_related_stats :RRC Connection related
 *                       stats
 *                     : p_src :Pointer to location for reading for parsing
 *                     : length_left :Length of message left to be read
 *                     : p_length_read :Length read
 *   OUTPUTS           : p_error_code :Error Code
 *   RETURNS           : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et
oam_rrc_parse_connection_related_stats
(
 rrc_oam_connection_related_stats_t *p_oam_rrc_connection_related_stats,
 UInt8  *p_src,
 SInt32 length_left,
 SInt32 *p_length_read,
 oam_error_code_et *p_error_code
 )
{   /* Cov CID 54877 Fix */
	SInt32 length_read = OAM_ZERO;
	UInt8 index  = OAM_ZERO;
	*p_length_read = OAM_ZERO;

	oam_memset((void *)p_oam_rrc_connection_related_stats,OAM_ZERO,
			sizeof(rrc_oam_connection_related_stats_t));

	/* This function parses rrc_connection_related_stats */

	if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!\n");

		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of basic type */
	oam_cp_unpack_UInt16(&p_oam_rrc_connection_related_stats->bitmask,
			p_src + *p_length_read
			, "bitmask"
			);



	*p_length_read += sizeof(UInt16);

	if (OAM_FAILURE == oam_rrc_parse_stat_connection_establishment(
				&p_oam_rrc_connection_related_stats->rrc_connection_establishment,
				p_src + *p_length_read,
				length_left - *p_length_read,
				&length_read,
				p_error_code))
	{
		oam_fprintf(stdout,
				"\nError in function oam_rrc_parse_stat_connection_establishment() :error code :%d",
				*p_error_code);

		return OAM_FAILURE;
	}

	*p_length_read += length_read;
	if (OAM_FAILURE == oam_rrc_parse_stat_connection_reestablishment(
				&p_oam_rrc_connection_related_stats->rrc_connection_reestablishment,
				p_src + *p_length_read,
				length_left - *p_length_read,
				&length_read,
				p_error_code))
	{
		oam_fprintf(stdout,
				"\nError in function oam_rrc_parse_stat_connection_reestablishment() :error code :%d",
				*p_error_code);

		return OAM_FAILURE;
	}

	*p_length_read += length_read;
	/*Parse/Unpack parameter of UInt32 */
	oam_cp_unpack_UInt32(&p_oam_rrc_connection_related_stats->
			rrcConnectionNumber, p_src + *p_length_read
			, "rrcConnectionNumber"
			);

	*p_length_read += sizeof(UInt32);
	for(index = OAM_ZERO; index <RRC_ESTABLISHMENT_CAUSE_INDEX; index++)
	{
		if (OAM_FAILURE == oam_rrc_parse_stat_conn_setup_time(
					&p_oam_rrc_connection_related_stats->rrc_connEstabTime_cause[index],
					p_src + *p_length_read,
					length_left - *p_length_read,
					&length_read,
					p_error_code))
		{
			oam_fprintf(stdout,
					"\nError in function oam_rrc_parse_stat_conn_setup_time() :error code :%d",
					*p_error_code);

			return OAM_FAILURE;
		}
	}
	*p_length_read += length_read;
	if (OAM_FAILURE == oam_rrc_parse_stat_ue_context_release(
				&p_oam_rrc_connection_related_stats->ue_contxt_rel_stats,
				p_src + *p_length_read,
				length_left - *p_length_read,
				&length_read,
				p_error_code))
	{
		oam_fprintf(stdout,
				"\nError in function oam_rrc_parse_stat_ue_context_release() :error code :%d",
				*p_error_code);

		return OAM_FAILURE;
	}

	*p_length_read += length_read;

	if (*p_length_read > length_left)
	{
		oam_fprintf(stdout, "Incoming message damaged!");

		return OAM_FAILURE;
	}

	return(OAM_SUCCESS);
}

/* OAM BCOM Code Comment Changes Start */
/******************************************************************************
 *   FUNCTION NAME     : oam_rrc_parse_get_cell_stats_resp
 *   DESCRIPTION       : This function parses the RRC OAM Get Cell Stats
 *                       Response Message
 *   INPUTS            : p_oam_rrc_get_cell_stats_resp :RRC OAM Get Cell Stats
 *                       Response
 *                     : p_src :Pointer to location for reading for parsing
 *                     : length_left :Length of message left to be read
 *                     : p_length_read :Length read
 *   OUTPUTS           : p_error_code :Error Code
 *   RETURNS           : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
oam_return_et
oam_rrc_parse_get_cell_stats_resp
(
 oam_rrc_get_cell_stats_resp_t *p_oam_rrc_get_cell_stats_resp,
 UInt8  *p_src,
 SInt32 length_left,
 SInt32 *p_length_read,
 oam_error_code_et *p_error_code
 )
{
    SInt32 length_read = OAM_ZERO;

    *p_length_read = OAM_ZERO;

    oam_memset((void *)p_oam_rrc_get_cell_stats_resp, OAM_ZERO,
            sizeof(oam_rrc_get_cell_stats_resp_t));

    /* This function parses rrc_oam_get_cell_stats_resp */


    if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
    {
        oam_fprintf(stdout, "Incoming message damaged!\n");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of UInt16*/
    oam_cp_unpack_UInt16(&p_oam_rrc_get_cell_stats_resp->bitmask,
            p_src + *p_length_read
            , "bitmask"
            );


    *p_length_read += sizeof(UInt16);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        oam_fprintf(stdout, "Incoming message damaged!\n");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of UInt8 */
    oam_cp_unpack_UInt8(&p_oam_rrc_get_cell_stats_resp->cell_index,
            p_src + *p_length_read
            , "cell_index"
            );

    *p_length_read += sizeof(UInt8);

    if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
    {
        oam_fprintf(stdout, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    /* Parse/Unpack parameter of basic type */
    oam_cp_unpack_UInt8(&p_oam_rrc_get_cell_stats_resp->response,
            p_src + *p_length_read
            , "response"
            );

    *p_length_read += sizeof(UInt8);

    if((p_oam_rrc_get_cell_stats_resp->bitmask & RRC_OAM_STATISTICS_ALL_STATS_PRESENT))
    {
    }

    if(p_oam_rrc_get_cell_stats_resp->bitmask &
            RRC_OAM_STATISTICS_CONNECTION_RELATED_STATS_PRESENT)
    {
        if (OAM_FAILURE == oam_rrc_parse_connection_related_stats(
                    &p_oam_rrc_get_cell_stats_resp->rrc_connection_related_stats,
                    p_src + *p_length_read,
                    length_left - *p_length_read,
                    &length_read,
                    p_error_code))
        {
            oam_fprintf(stdout,
                    "\nError in function oam_rrc_parse_connection_related_stats :error code :%d",
                    *p_error_code);
            return OAM_FAILURE;
        }

        *p_length_read += length_read;
    }

    if(p_oam_rrc_get_cell_stats_resp->bitmask &
            RRC_OAM_STATISTICS_ERAB_RELATED_STATS_PRESENT)
    {
        if (OAM_FAILURE == oam_rrc_parse_erab_related_stats(
                    &p_oam_rrc_get_cell_stats_resp->rrc_erab_related_stats,
                    p_src + *p_length_read,
                    length_left - *p_length_read,
                    &length_read,
                    p_error_code))
        {
            oam_fprintf(stdout,
                    "\nError in function oam_rrc_parse_erab_related_stats :error code :%d",
                    *p_error_code);
            return OAM_FAILURE;
        }

        *p_length_read += length_read;
    }

    if(p_oam_rrc_get_cell_stats_resp->bitmask &
            RRC_OAM_STATISTICS_UE_ASSOC_S1_CONN_RELATED_STATS_PRESENT)
    {
        if (OAM_FAILURE == oam_rrc_parse_stat_ue_assoc_s1_connection(
                    &p_oam_rrc_get_cell_stats_resp->rrc_ue_assoc_s1_conn_related_stats,
                    p_src + *p_length_read,
                    length_left - *p_length_read,
                    &length_read,
                    p_error_code))
        {
            oam_fprintf(stdout,
                    "\nError in function oam_rrc_parse_stat_ue_assoc_s1_connection :error code :%d",
                    *p_error_code);
            return OAM_FAILURE;
        }

        *p_length_read += length_read;
    }

    if(p_oam_rrc_get_cell_stats_resp->bitmask &
            RRC_OAM_STATISTICS_PAGING_RELATED_STATS_PRESENT)
    {
        if (OAM_FAILURE == oam_rrc_parse_csc_stat_paging(
                    &p_oam_rrc_get_cell_stats_resp->rrc_paging_related_stats,
                    p_src + *p_length_read,
                    length_left - *p_length_read,
                    &length_read,
                    p_error_code))
        {
            oam_fprintf(stdout,
                    "\nError in function oam_rrc_parse_csc_stat_paging :error code :%d",
                    *p_error_code);
            return OAM_FAILURE;
        }

        *p_length_read += length_read;
    }

    if (*p_length_read > length_left)   {
        oam_fprintf(stdout, "Incoming message damaged!");
        return OAM_FAILURE;
    }

    return(OAM_SUCCESS);
}
/* OAM BCOM Code Comment Changes End */

/*******************************************************************************
 * Function  Name : oam_qcli_handle_rlc_get_stats_cnf
 *
 *
 * Description    : OAM sends GET/SET parameters resp to user interface
 * Inputs         : p_api_buf,msg_len
 * Outputs        : p_error_code :Error Code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/

oam_return_et
oam_qcli_handle_rlc_get_stats_cnf
(
 void* p_api_buf,
 UInt16 msg_len,
 oam_error_code_et *p_error_code

 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(p_error_code);
    qcliHandleUnusedParameter(msg_len);
    /* SPR 17777 FIX END */    

	Oam_Rlc_Get_Stat_Cnf_t *p_oam_rlc_get_stats_cnf = OAM_NULL;

	oam_counter_t num_of_active_UE;


	p_api_buf = p_api_buf + OAM_MGMT_INTF_HEADER_SIZE;

	p_oam_rlc_get_stats_cnf = (Oam_Rlc_Get_Stat_Cnf_t *)p_api_buf;

	if (OAM_SUCCESS == p_oam_rlc_get_stats_cnf->responseCode)
	{
		oam_fprintf(stdout,"\nRLC_GET_STATS_CNF[SUCCESS]");
		oam_fprintf(stdout,
				"\n########################### RLC STATS ###########################\n\n");
		oam_fprintf(stdout,
				"\n-------------------------- RLC TM STATS -------------------------\n");
		oam_fprintf(stdout, "\n\tTM Rx PDU                      : %d",
				p_oam_rlc_get_stats_cnf->lteTMStat.rxTmdPdu);
		oam_fprintf(stdout, "\n\tTM Tx PDU                      : %d",
				p_oam_rlc_get_stats_cnf->lteTMStat.txTmdPdu);
		oam_fprintf(stdout,
				"\n-------------------------- RLC UM STATS -------------------------\n");
		oam_fprintf(stdout, "\n\tUM Rx SDU Discarded            : %d", 
				p_oam_rlc_get_stats_cnf->lteUMStat.rxUmdSduDiscarded);
		oam_fprintf(stdout, "\n\tUM Tx PDU                      : %d", 
				p_oam_rlc_get_stats_cnf->lteUMStat.txUmdPdu);
		oam_fprintf(stdout, "\n\tUM Rx SDU Given to Upper Layer : %d", 
				p_oam_rlc_get_stats_cnf->lteUMStat.rxUmdSduTxUL);
		oam_fprintf(stdout, "\n\tUM Rx Incomplete SDU Discarded : %d", 
				p_oam_rlc_get_stats_cnf->lteUMStat.rxIncompleteUmdSduDiscarded);
		oam_fprintf(stdout, "\n\tUM Rx PDU                      : %d", 
				p_oam_rlc_get_stats_cnf->lteUMStat.rxUmdPdu);
		oam_fprintf(stdout, "\n\tUM Rx PDU Dropped              : %d", 
				p_oam_rlc_get_stats_cnf->lteUMStat.rxUmdPduDropped);
		oam_fprintf(stdout, "\n\tUM SDU Received from PDCP      : %d", 
				p_oam_rlc_get_stats_cnf->lteUMStat.umdSduReceivedFromPdcp);
		oam_fprintf(stdout, "\n\tUM SDU Transmitted to MAC      : %d", 
				p_oam_rlc_get_stats_cnf->lteUMStat.umdSduTransmittedToMac);
		oam_fprintf(stdout,
				"\n-------------------------- RLC AM STATS -------------------------\n");
		oam_fprintf(stdout, "\n\tAM PDU Transmitted             : %d", 
				p_oam_rlc_get_stats_cnf->lteAMStat.amdPduTransmitted);
		oam_fprintf(stdout, "\n\tAM PDU Received                : %d", 
				p_oam_rlc_get_stats_cnf->lteAMStat.amdPduReceived);
		oam_fprintf(stdout, "\n\tAM PDU Dropped                 : %d", 
				p_oam_rlc_get_stats_cnf->lteAMStat.amdPduDropped);
		oam_fprintf(stdout, "\n\tAM Incomplete SDU Discarded    : %d", 
				p_oam_rlc_get_stats_cnf->lteAMStat.amdIncompSduDiscard);
		oam_fprintf(stdout, "\n\tAM SDU Received from PDCP      : %d", 
				p_oam_rlc_get_stats_cnf->lteAMStat.amdSduReceivedFromPdcp);
		oam_fprintf(stdout, "\n\tAM SDU Transmitted to PDCP     : %d", 
				p_oam_rlc_get_stats_cnf->lteAMStat.amdSduTransmittedToPdcp);
		oam_fprintf(stdout, "\n\tAM SDU Dropped                 : %d", 
				p_oam_rlc_get_stats_cnf->lteAMStat.amdSduDropped);
		oam_fprintf(stdout, "\n\tAM SDU Transmitted to MAC      : %d", 
				p_oam_rlc_get_stats_cnf->lteAMStat.amdSduTransmitedToMac);
		oam_fprintf(stdout,
				"\n-------------------------- RLC UE STATS -------------------------\n");
		oam_fprintf(stdout, "\n\tNumber of Active UE             : %d", 
				p_oam_rlc_get_stats_cnf->lteUEStat.numOfActiveUE);

		for (num_of_active_UE = 0;
				num_of_active_UE < p_oam_rlc_get_stats_cnf->lteUEStat.numOfActiveUE;
				++num_of_active_UE)
		{
			oam_fprintf(stdout, "\n\t--------------- UE [%d] ---------------",
					(num_of_active_UE + 1));

			oam_fprintf(stdout, "\n\t\tCRNTI                       : %d",
					p_oam_rlc_get_stats_cnf->lteUEStat.rlcUeStats[num_of_active_UE].crnti);
			oam_fprintf(stdout, "\n\t\tRLC SDU Received from PDCP  : %d",
					p_oam_rlc_get_stats_cnf->
					lteUEStat.rlcUeStats[num_of_active_UE].sduReceivedFromPdcp);
			oam_fprintf(stdout, "\n\t\tRLC PDU Transmitted to MAC  : %d",
					p_oam_rlc_get_stats_cnf->
					lteUEStat.rlcUeStats[num_of_active_UE].pduTransmittedToMac);
			oam_fprintf(stdout, "\n\t\tRLC SDU Transmitted to PDCP : %d",
					p_oam_rlc_get_stats_cnf->
					lteUEStat.rlcUeStats[num_of_active_UE].sduTransmittedToPdcp);
			oam_fprintf(stdout, "\n\t\tRLC PDU Received from MAC   : %d",
					p_oam_rlc_get_stats_cnf->
					lteUEStat.rlcUeStats[num_of_active_UE].pduReceivedFromMac);

		}
		oam_fprintf(stdout,
				"\n\n################################################################\n");

	}
	else
	{
		oam_fprintf(stdout,"\nRLC_GET_STATS_CNF FAILURE with cause [%d]",
				p_oam_rlc_get_stats_cnf->responseCode);
	}

	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function  Name : oam_qcli_handle_mac_get_stats_cnf
 *
 * Description    : OAM sends GET/SET parameters resp to user interface
 * Inputs         : p_api_buf,msg_len
 * Outputs        : p_error_code :Error Code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_qcli_handle_mac_get_stats_cnf
(
 void* p_api_buf,
 UInt16 msg_len,
 oam_error_code_et *p_error_code

 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(p_error_code);
    qcliHandleUnusedParameter(msg_len);
    /* SPR 17777 FIX END */

	oam_mac_stats_resp_t   *p_mac_stats_resp;

	p_mac_stats_resp = OAM_NULL;

	p_api_buf = p_api_buf + OAM_MGMT_INTF_HEADER_SIZE;

	p_mac_stats_resp  = (oam_mac_stats_resp_t *)p_api_buf;


	/* check the response code received from MAC */
	if (OAM_SUCCESS == p_mac_stats_resp->responseCode)
	{    
		oam_fprintf(stdout,"\nReceived MAC STATS Response with Status as"
				" SUCCESS");
        /* + SPR 19808 */
        oam_fprintf(stdout, "\n\tDownlink Throughput (bits per sec) : %f",
				p_mac_stats_resp->dlThroughPut);
        oam_fprintf(stdout, "\n\tUplink Throughput (bits per sec)   : %f",
				p_mac_stats_resp->ulThroughPut);
        /* - SPR 19808 */
		oam_fprintf(stdout, "\n\tDownlink Spectral Efficiency : %f",
				p_mac_stats_resp->dlSpectralEfficiency);
		oam_fprintf(stdout, "\n\tUplink Spectral Efficiency   : %f\n",
				p_mac_stats_resp->ulSpectralEfficiency);
	}    

	else /* Failure is received from MAC for MAC_STATS_CNF */
	{    
		oam_fprintf(stdout,"\nMAC_STATS_CNF[FAILURE] with cause[%d]",
				p_mac_stats_resp->responseCode);
		return OAM_FAILURE;
	} 


	return OAM_SUCCESS;
}
/*******************************************************************************
 * Function  Name : oam_qcli_parse_active_mme_ctx
 *
 * Description    : OAM sends GET/SET parameters resp to user interface
 * Inputs         : p_oam_active_mme_ctx,p_src,p_length_read
 * Outputs        : p_error_code :Error Code
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
oam_qcli_parse_active_mme_ctx
(
 oam_active_mme_ctx_t *p_oam_active_mme_ctx,
 UInt8  *p_src,
 SInt32 length_left,
 SInt32 *p_length_read,
 oam_error_code_et *p_error_code
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(p_error_code);
    /* SPR 17777 FIX END */


	*p_length_read = OAM_ZERO;

	oam_memset((void *)p_oam_active_mme_ctx, OAM_ZERO, sizeof(oam_active_mme_ctx_t));

	/* This function parses oam_active_mme_ctx */

	if (*p_length_read + (SInt32)sizeof(UInt8) > length_left)
	{
		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of UInt8 */
	oam_cp_unpack_UInt8(&p_oam_active_mme_ctx->mme_id, p_src + *p_length_read
			, "mme_id"
			);
	*p_length_read += sizeof(UInt8);

	if (*p_length_read + (SInt32)sizeof(UInt16) > length_left)
	{
		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of UInt16 */
	oam_cp_unpack_UInt16(&p_oam_active_mme_ctx->count_of_ue,
			p_src + *p_length_read
			, "count_of_ue"
			);
	*p_length_read += sizeof(UInt16);

	if (*p_length_read > length_left)
	{
		return OAM_FAILURE;
	}
	return(OAM_SUCCESS);
}
/*******************************************************************************
 * Function  Name : oam_qcli_handle_s1ap_parse_stats_resp

 * Description    : OAM sends GET/SET parameters resp to user interface
 * Inputs         : p_oam_s1ap_stats_resp,p_src,p_length_read,p_error_code
 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE 
 *******************************************************************************/
oam_return_et
oam_qcli_s1ap_parse_stats_resp
(
 oam_s1ap_status_resp_t *p_oam_s1ap_stats_resp,
 UInt8  *p_src,
 SInt32 length_left,
 SInt32 *p_length_read,
 oam_error_code_et *p_error_code
 )
{
    /* cov cid 54886 fix*/
	SInt32 length_read = OAM_ZERO;

	*p_length_read = OAM_ZERO;

	oam_memset((void *)p_oam_s1ap_stats_resp, OAM_ZERO, sizeof(oam_s1ap_status_resp_t));

	if ((*p_length_read + (SInt32)sizeof(p_oam_s1ap_stats_resp->active_mme_count)) > length_left)
	{ 
		return OAM_FAILURE;
	}

	/* Parse/Unpack parameter of UInt8*/
	oam_cp_unpack_UInt8((void *)&(p_oam_s1ap_stats_resp->active_mme_count),
			p_src + *p_length_read
			, "active_mme_count"
			);

	*p_length_read += sizeof(UInt8);

	/* Parse OCTET_STRING VARIABLE of oam_active_mme_ctx_t */
	{
		rrc_counter_t loop_counter;
		for (loop_counter = OAM_ZERO;
				loop_counter < p_oam_s1ap_stats_resp->active_mme_count;
				loop_counter++)
		{

			if (OAM_FAILURE == oam_qcli_parse_active_mme_ctx(
						&(p_oam_s1ap_stats_resp->active_mme_list[loop_counter]),
						p_src + *p_length_read,
						length_left - *p_length_read,
						&length_read,
						p_error_code))
			{
				return OAM_FAILURE;
			}

			*p_length_read += length_read;
		}
	}

	if (*p_length_read > length_left)
	{
		return OAM_FAILURE;
	}
	return(OAM_SUCCESS);
}
/*******************************************************************************
 * Function  Name : oam_qcli_handle_s1ap_get_stats_cnf
 * Description    : OAM sends GET/SET parameters resp to user interface
 * Inputs         : p_api_buf,msg_len,p_error_code

 * Outputs        : None
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/

oam_return_et
oam_qcli_handle_s1ap_get_stats_cnf
(   
 void* p_api_buf,
 UInt16 data_len,
 oam_error_code_et *p_error_code
 ) 
{
	SInt32   length         = OAM_ZERO;
	oam_s1ap_status_resp_t oam_s1ap_status_resp;
	UInt8    no_of_mme      = OAM_ZERO;
	UInt8    mme_index      = OAM_ZERO;

	p_api_buf = p_api_buf + OAM_MGMT_INTF_HEADER_SIZE;


	if (OAM_FAILURE == oam_qcli_s1ap_parse_stats_resp(
				&oam_s1ap_status_resp,
				(UInt8*)p_api_buf,
				(data_len - (OAM_MGMT_INTF_HEADER_SIZE)),
				&length,
				p_error_code))
	{
		return OAM_FAILURE;
	}
	no_of_mme = oam_s1ap_status_resp.active_mme_count;

	oam_fprintf(stdout,
			"\n######################## S1AP STATS ########################\n\n");
	oam_fprintf(stdout,"\n\tActive MME            : %d",no_of_mme);

	for (mme_index = OAM_ZERO; mme_index < no_of_mme; mme_index++)
	{
		oam_fprintf(stdout,
				"\n\t------------------------ MME[%d] ------------------------",
				(mme_index+1));
		oam_fprintf(stdout,"\n\t\tMME Id                 : %d",
				oam_s1ap_status_resp.active_mme_list[mme_index].mme_id);
		oam_fprintf(stdout,"\n\t\tNumber of Active UE    : %d\n",
				oam_s1ap_status_resp.active_mme_list[mme_index].count_of_ue);
	}

	return(OAM_SUCCESS);
}
/*******************************************************************************
 * Function  Name : oam_qcli_handle_pdcp_get_stats_cnf 
 * Description    : OAM sends GET/SET parameters resp to user interface
 * Inputs         : p_api_buf,msg_len,p_error_code
 * Outputs        : OAM_SUCCESS/OAM_FAILURE
 * Returns        : NONE
 *******************************************************************************/

oam_return_et
oam_qcli_handle_pdcp_get_stats_cnf
(
 void* p_api_buf,
 UInt16 msg_len,
 oam_error_code_et *p_error_code

 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(p_error_code);
    qcliHandleUnusedParameter(msg_len);
    /* SPR 17777 FIX END */

	oam_pdcp_stats_cnf_t *p_pdcp_stats_resp;
	p_pdcp_stats_resp  = OAM_NULL;
	oam_counter_t   loop_counter      = OAM_ZERO;
	oam_counter_t   loop_counter1     = OAM_ZERO;
	oam_counter_t   loop_counter2     = OAM_ZERO;

	p_api_buf            = p_api_buf + OAM_MGMT_INTF_HEADER_SIZE;
	p_pdcp_stats_resp    = (oam_pdcp_stats_cnf_t*)p_api_buf;


	/* check the response code received from PDCP*/
	if (OAM_SUCCESS == p_pdcp_stats_resp->responseCode)
	{
		oam_fprintf(stdout,"\n------------- INTEGRITY STATS ------------");
		oam_fprintf(stdout,"\n\tNumber Of Packets Integrity Protected"
				" Using EIA0: %u", p_pdcp_stats_resp->
				statsIntegrity.numberOfPacketsIntegrityProtectedUsingEIA0);
		oam_fprintf(stdout,"\n\tNumber Of Packets Integrity Verified"
				" Using EIA0: %u", p_pdcp_stats_resp->
				statsIntegrity.numberOfPacketsIntegrityVerifiedUsingEIA0);
		oam_fprintf(stdout,"\n\tNumber Of Packets Integrity Protected"
				" Using EIA1: %u", p_pdcp_stats_resp->
				statsIntegrity.numberOfPacketsIntegrityProtectedUsingEIA1);
		oam_fprintf(stdout,"\n\tNumber Of Packets Integrity Verified"
				" Using EIA1: %u", p_pdcp_stats_resp->
				statsIntegrity.numberOfPacketsIntegrityVerifiedUsingEIA1);
		oam_fprintf(stdout,"\n\tNumber Of Packets Integrity Protected"
				" Using EIA2: %u", p_pdcp_stats_resp->
				statsIntegrity.numberOfPacketsIntegrityProtectedUsingEIA2);
		oam_fprintf(stdout,"\n\tNumber Of Packets Integrity Verified"
				" Using  EIA2: %u", p_pdcp_stats_resp->
				statsIntegrity.numberOfPacketsIntegrityVerifiedUsingEIA2);
		oam_fprintf(stdout,"\n\tNumber Of Packets Rejected by Integrity"
				" Verification: %u", p_pdcp_stats_resp->						                                                              statsIntegrity.numberOfPacketsRejectedByIntegrityVerification);
		oam_fprintf(stdout,"\n-------------- CIPHERING STATS -------------");
		oam_fprintf(stdout,"\n\tNumber Of Packets Ciphered using EEA0: %u",
				p_pdcp_stats_resp->statsCiphering.numberOfPacketsCipheredUsingEIA0);
		oam_fprintf(stdout,"\n\tNumber Of Packets DeCiphered using EEA0: %u",
				p_pdcp_stats_resp->statsCiphering.numberOfPacketsDecipheredUsingEIA0);
		oam_fprintf(stdout,"\n\tNumber Of Packets Ciphered using EEA1: %u",
				p_pdcp_stats_resp->statsCiphering.numberOfPacketsCipheredUsingEIA1);
		oam_fprintf(stdout,"\n\tNumber Of Packets DeCiphered using EEA1: %u",
				p_pdcp_stats_resp->statsCiphering.numberOfPacketsDecipheredUsingEIA1);
		oam_fprintf(stdout,"\n\tNumber Of Packets Ciphered using EEA2: %u",
				p_pdcp_stats_resp->statsCiphering.numberOfPacketsCipheredUsingEIA2);
		oam_fprintf(stdout,"\n\tNumber Of Packets DeCiphered using EEA2: %u",
				p_pdcp_stats_resp->statsCiphering.numberOfPacketsDecipheredUsingEIA2);
		oam_fprintf(stdout,"\n--------------  ROHC STATS ------------------");
		oam_fprintf(stdout,"\n\tNumber Of IR Packets Processed: %u",
				p_pdcp_stats_resp->statsRohc.numberOfIRPacketsProcessed);
		oam_fprintf(stdout,"\n\tNumber Of IR Packets Created: %u",
				p_pdcp_stats_resp->statsRohc.numberOfIRPacketsCreated);
		oam_fprintf(stdout,"\n\tNumber Of IRDyn Packets Processed: %u",
				p_pdcp_stats_resp->statsRohc.numberOfIRDYNPacketsProcessed);
		oam_fprintf(stdout,"\n\tNumber Of IRDyn Packets Created: %u",
				p_pdcp_stats_resp->statsRohc.numberOfIRDYNPacketsCreated );
		oam_fprintf(stdout,"\n\tNumber Of ACK Processed: %u",
				p_pdcp_stats_resp->statsRohc.numberOfACKProcessed);
		oam_fprintf(stdout,"\n\tNumber Of ACK Created: %u",
				p_pdcp_stats_resp->statsRohc.numberOfACKCreated);
		oam_fprintf(stdout,"\n\tNumber Of NACK Processed: %u",
				p_pdcp_stats_resp->statsRohc.numberOfNACKProcessed);
		oam_fprintf(stdout,"\n\tNumber Of NACK Created: %u",
				p_pdcp_stats_resp->statsRohc.numberOfNACKCreated);
		oam_fprintf(stdout,"\n\tNumber Of Static NACK Processed: %u",
				p_pdcp_stats_resp->statsRohc.numberOfSNACKProcessed);
		oam_fprintf(stdout,"\n\tNumber Of Static NACK Created: %u",
				p_pdcp_stats_resp->statsRohc.numberOfSNACKCreated);
		oam_fprintf(stdout,"\n\tNumber Of R0 Packets Processed: %u",
				p_pdcp_stats_resp->statsRohc.numberOfR0PacketsProcessed);
		oam_fprintf(stdout,"\n\tNumber Of R0CRC Packets Processed: %u",
				p_pdcp_stats_resp->statsRohc.numberOfR0CRCPacketsProcessed);
		oam_fprintf(stdout,"\n\tNumber Of UO0 Packets Processed: %u",
				p_pdcp_stats_resp->statsRohc.numberOfUO0PacketsProcessed);
		oam_fprintf(stdout,"\n\tNumber Of R1 Packets Processed: %u",
				p_pdcp_stats_resp->statsRohc.numberOfR1PacketsProcessed);
		oam_fprintf(stdout,"\n\tNumber Of R1ID Packets Processed: %u",
				p_pdcp_stats_resp->statsRohc.numberOfR1IDPacketsProcessed);
		oam_fprintf(stdout,"\n\tNumber Of R1TS Packets Processed: %u",
				p_pdcp_stats_resp->statsRohc.numberOfR1TSPacketsProcessed);
		oam_fprintf(stdout,"\n\tNumber Of UO1 Packets Processed: %u",
				p_pdcp_stats_resp->statsRohc.numberOfUO1PacketsProcessed);
		oam_fprintf(stdout,"\n\tNumber Of UO1ID Packets Processed: %u",
				p_pdcp_stats_resp->statsRohc.numberOfUO1IDPacketsProcessed);                                                                                oam_fprintf(stdout,"\n\tNumber Of UO1TS Packets Processed: %u",
					p_pdcp_stats_resp->statsRohc.numberOfUO1TSPacketsProcessed);
		oam_fprintf(stdout,"\n\tNumber Of UOR2 Packets Processed: %u",
				p_pdcp_stats_resp->statsRohc.numberOfUOR2PacketsProcessed);
		oam_fprintf(stdout,"\n\tNumber Of UOR2ID Packets Processed: %u",
				p_pdcp_stats_resp->statsRohc.numberOfUOR2IDPacketsProcessed);
		oam_fprintf(stdout,"\n\tNumber Of UOR2TS Packets Processed: %u",
				p_pdcp_stats_resp->statsRohc.numberOfUOR2TSPacketsProcessed);
		oam_fprintf(stdout,"\n\tNumber Of R0 Packets Created: %u",
				p_pdcp_stats_resp->statsRohc.numberOfR0PacketsCreated);
		oam_fprintf(stdout,"\n\tNumber Of R0CRC Packets Created: %u",
				p_pdcp_stats_resp->statsRohc.numberOfR0CRCPacketsCreated);
		oam_fprintf(stdout,"\n\tNumber Of UO0 Packets Created: %u",
				p_pdcp_stats_resp->statsRohc.numberOfUO0PacketsCreated);
		oam_fprintf(stdout,"\n\tNumber Of R1 Packets Created: %u",
				p_pdcp_stats_resp->statsRohc.numberOfR1PacketsCreated);
		oam_fprintf(stdout,"\n\tNumber Of R1ID Packets Created: %u",
				p_pdcp_stats_resp->statsRohc.numberOfR1IDPacketsCreated);
		oam_fprintf(stdout,"\n\tNumber Of R1TS Packets Created: %u",
				p_pdcp_stats_resp->statsRohc.numberOfR1TSPacketsCreated);
		oam_fprintf(stdout,"\n\tNumber Of UO1 Packets Created: %u",
				p_pdcp_stats_resp->statsRohc.numberOfUO1PacketsCreated);
		oam_fprintf(stdout,"\n\tNumber Of UO1ID Packets Created: %u",
				p_pdcp_stats_resp->statsRohc.numberOfUO1IDPacketsCreated);
		oam_fprintf(stdout,"\n\tNumber Of UO1TS Packets Created: %u",
				p_pdcp_stats_resp->statsRohc.numberOfUO1TSPacketsCreated);

		oam_fprintf(stdout,"\n\tNumber Of UOR2 Packets Created: %u",
				p_pdcp_stats_resp->statsRohc.numberOfUOR2PacketsCreated);
		oam_fprintf(stdout,"\n\tNumber Of UOR2ID Packets Created: %u",
				p_pdcp_stats_resp->statsRohc.numberOfUOR2IDPacketsCreated);
		oam_fprintf(stdout,"\n\tNumber Of UOR2TS Packets Created: %u",
				p_pdcp_stats_resp->statsRohc.numberOfUOR2TSPacketsCreated);
		oam_fprintf(stdout,"\n\tNumber Of ModeTrans U2OInit: %u",
				p_pdcp_stats_resp->statsRohc.numberOfModeTransU2OInit);
		oam_fprintf(stdout,"\n\tNumber Of ModeTrans U2RInit: %u",
				p_pdcp_stats_resp->statsRohc.numberOfModeTransU2RInit);
		oam_fprintf(stdout,"\n\tNumber Of ModeTrans O2RInit: %u",
				p_pdcp_stats_resp->statsRohc.numberOfModeTransO2RInit);
		oam_fprintf(stdout,"\n\tNumber OfModeTrans O2UInit: %u",
				p_pdcp_stats_resp->statsRohc.numberOfModeTransO2UInit);
		oam_fprintf(stdout,"\n\tNumber Of ModeTrans R2UInit: %u",
				p_pdcp_stats_resp->statsRohc.numberOfModeTransR2UInit);
		oam_fprintf(stdout,"\n\tNumber Of ModeTrans R2OInit: %u",
				p_pdcp_stats_resp->statsRohc.numberOfModeTransR2OInit);
		oam_fprintf(stdout,"\n\tNumber Of Times Crc Failed: %u",
				p_pdcp_stats_resp->statsRohc.numberOfTimesCrcFailed);
		oam_fprintf(stdout,"\n\tNumber Of ModeTrans U2ORcvd: %u",
				p_pdcp_stats_resp->statsRohc.numberOfModeTransU2ORcvd);
		oam_fprintf(stdout,"\n\tNumber Of ModeTrans U2RRcvd: %u",
				p_pdcp_stats_resp->statsRohc.numberOfModeTransU2RRcvd);
		oam_fprintf(stdout,"\n\tNumber Of ModeTrans O2RRcvd: %u",
				p_pdcp_stats_resp->statsRohc.numberOfModeTransO2RRcvd);
		oam_fprintf(stdout,"\n\tNumber Of ModeTrans O2URcvd: %u",
				p_pdcp_stats_resp->statsRohc.numberOfModeTransO2URcvd);
		oam_fprintf(stdout,"\n\tNumber Of ModeTrans R2URcvd: %u",
				p_pdcp_stats_resp->statsRohc.numberOfModeTransR2URcvd);
		oam_fprintf(stdout,"\n\tNumber Of ModeTrans R2ORcvd: %u",
				p_pdcp_stats_resp->statsRohc.numberOfModeTransR2ORcvd);
		oam_fprintf(stdout,"\n\tNumber Of P0 Normal Packets Created: %u",
				p_pdcp_stats_resp->statsRohc.numberOfP0NormalPacketsCreated);
		oam_fprintf(stdout,"\n\tNumber Of P0 Normal Packets Processed: %u",
				p_pdcp_stats_resp->statsRohc.numberOfP0NormalPacketsProcessed);
		oam_fprintf(stdout,"\n\tNumber Of Active UE: %u",
				p_pdcp_stats_resp->statNumActiveUe);

		/*values corrosponding to each UE */
		for (loop_counter = OAM_ZERO; loop_counter < p_pdcp_stats_resp->
				statNumActiveUe;loop_counter++)
		{
			oam_fprintf(stdout, "\n\t------------- UE Stats [%d]--------",
					(loop_counter + 1));
			oam_fprintf(stdout, "\n\t\tCrnti: %d", p_pdcp_stats_resp->
					statsUEs_p[loop_counter].crnti);
			oam_fprintf(stdout, "\n\t\tStatNumActiveSrbLc: %d",
					p_pdcp_stats_resp->statsUEs_p[loop_counter].
					statNumActiveSrbLc);
			oam_fprintf(stdout, "\n\t\tStatNumActiveDrbLc: %d",
					p_pdcp_stats_resp->statsUEs_p[loop_counter].
					statNumActiveDrbLc);

			for (loop_counter1 = OAM_ZERO; loop_counter1 < p_pdcp_stats_resp->
					statsUEs_p[loop_counter].statNumActiveSrbLc;
					loop_counter1++)
			{
				oam_fprintf(stdout, "\n\t\t-------- SRB [%d]-------",
						(loop_counter1 + 1));
				oam_fprintf(stdout, "\n\t\t------  Downlink stats ------");
				oam_fprintf(stdout, "\n\t\t\tnumberOfDlSrbSduReceived:%u",
						p_pdcp_stats_resp->statsUEs_p[loop_counter]
						.srbs[loop_counter1].numberOfDlSrbSduReceived);
				oam_fprintf(stdout, "\n\t\t\tnumberOfDlSrbPduTransmitted:"
						" %u", p_pdcp_stats_resp->statsUEs_p[loop_counter].
						srbs[loop_counter1].numberOfDlSrbPduTransmitted);
				oam_fprintf(stdout, "\n\t\t\tnumberOfSrbDeliveryIndSent:"
						" %u",p_pdcp_stats_resp->statsUEs_p[loop_counter].
						srbs[loop_counter1].numberOfSrbDeliveryIndSent);
				oam_fprintf(stdout, "\n\t\t\tnumberOfDlSrbPduDropNackRecvd:"
						" %u",p_pdcp_stats_resp->statsUEs_p[loop_counter].
						srbs[loop_counter1].numberOfDlSrbPduDropNackRecvd);
				oam_fprintf(stdout, "\n\t\t\tnumberOfDlSrbSduDiscarded:%u",
						p_pdcp_stats_resp->statsUEs_p[loop_counter].
						srbs[loop_counter1].numberOfDlSrbSduDiscarded);
				oam_fprintf(stdout, "\n\t\t--------- Uplink stats ----------");
				oam_fprintf(stdout, "\n\t\t\tnumberOfSrbUlPduReceived: %u",
						p_pdcp_stats_resp->statsUEs_p[loop_counter].
						srbs[loop_counter1].numberOfSrbUlPduReceived);
				oam_fprintf(stdout, "\n\t\t\tnumberOfSrbUlSduTransmitted:"
						" %u",p_pdcp_stats_resp->statsUEs_p[loop_counter].
						srbs[loop_counter1].numberOfSrbUlSduTransmitted);
				oam_fprintf(stdout, "\n\t\t\tnumberOfUlSrbSduDropped: %u",
						p_pdcp_stats_resp->statsUEs_p[loop_counter].
						srbs[loop_counter1].numberOfUlSrbSduDropped);
			}
			for (loop_counter2 = OAM_ZERO; loop_counter2 < p_pdcp_stats_resp->
					statsUEs_p[loop_counter].statNumActiveDrbLc;
					loop_counter2++)
			{
				{
					oam_fprintf(stdout, "\n\t\t-------DRB [%d] ------",
							(loop_counter2 + 1));
					oam_fprintf(stdout, "\n\t\t-------Downlink stats ---------");
					oam_fprintf(stdout, "\n\t\t\tnumberOfDlDrbSduReceived:%u",
							p_pdcp_stats_resp->statsUEs_p[loop_counter].
							drbs[loop_counter2].numberOfDlDrbSduReceived);
					oam_fprintf(stdout, "\n\t\t\tnumberOfDlDrbPduTransmitted:"
							" %u",p_pdcp_stats_resp->statsUEs_p[loop_counter].
							drbs[loop_counter2].numberOfDlDrbPduTransmitted);
					oam_fprintf(stdout, "\n\t\t\tnumberOfDlDrbPduDropForNack:"
							" %u",p_pdcp_stats_resp->statsUEs_p[loop_counter].
							drbs[loop_counter2].numberOfDlDrbPduDropForNack);
					oam_fprintf(stdout, "\n\t\t\tnumberOfDlDrbPduDropForRoHC"
							" Failure:%u",p_pdcp_stats_resp->statsUEs_p[loop_counter].
							drbs[loop_counter2].numberOfDlDrbPduDropForRoHCFailure);
					oam_fprintf(stdout, "\n\t\t\tnumberOfDrbDeliveryIndSent:"
							" %u",p_pdcp_stats_resp->statsUEs_p[loop_counter].
							drbs[loop_counter2].numberOfDrbDeliveryIndSent);
					oam_fprintf(stdout, "\n\t\t\tnumberOfDlDrbSduDiscarded:"
							" %u",p_pdcp_stats_resp->statsUEs_p[loop_counter].
							drbs[loop_counter2].numberOfDlDrbSduDiscarded);
					oam_fprintf(stdout, "\n\t\t-------- Uplink stats ---------");
					oam_fprintf(stdout, "\n\t\t\tnumberOfDrbUlSduReceived:"
							" %u",p_pdcp_stats_resp->statsUEs_p[loop_counter].
							drbs[loop_counter2].numberOfDrbUlSduReceived);
					oam_fprintf(stdout, "\n\t\t\tnumberOfDrbUlPduTransmitted:"
							" %u",p_pdcp_stats_resp->statsUEs_p[loop_counter].
							drbs[loop_counter2].numberOfDrbUlPduTransmitted);
					oam_fprintf(stdout,"\n\t\t\tnumberOfDrbUlPduDropDueToRoHc"
							" Failure:%u",p_pdcp_stats_resp->statsUEs_p[loop_counter].
							drbs[loop_counter2].numberOfDrbUlPduDropDueToRoHcFailure);
					oam_fprintf(stdout,"\n\t\t\tnumberOfDrbUlPduDropAsSizeOO"
							" Range:%u", p_pdcp_stats_resp->statsUEs_p[loop_counter].
							drbs[loop_counter2].numberOfDrbUlPduDropAsSizeOORange);
				}
			}
		}
	}
	else
	{
		oam_fprintf(stdout,"\nPDCP_STATS_CNF[FAILURE] with cause[%d]\n",
				p_pdcp_stats_resp->responseCode);

		return OAM_FAILURE;
	}
	return(OAM_SUCCESS);
}
/*******************************************************************************
 * Function  Name : oam_interface_resp
 * Description    : OAM sends GET/SET parameters resp to user interface
 * Inputs         : p_api_buf 
 * Outputs        : OAM_SUCCESS/OAM_FAILURE
 * Returns        : NONE
 *******************************************************************************/
oam_return_et 
oam_interface_resp(UInt8 *p_api_buf)
{
	UInt8  status = OAM_ZERO;
	UInt16 error_code = OAM_ZERO;
	UInt8  length_error_string = OAM_ZERO;
	UInt8  error_string[80]={0};

    /* SPR 15527 START */
	UInt16  num_of_parameters = OAM_ZERO;
    /* SPR 15527 END */
	UInt16  length_parent_name = OAM_ZERO;
	UInt8  parent_node_name[256]={0};
	UInt16 length_parameter_info = OAM_ZERO; 
    /* SPR 17243 START */
	UInt8  name_val[STR_LEN_512] = {0};
    /* SPR 17243 END */
        /* SPR 19221: CID 108651 Fix Start */ 
	UInt16  name_val_length = OAM_ZERO;
        /* SPR 19221: CID 108651 Fix End */ 
	char *name = NULL;
	char *value = NULL;

	UInt16 api_id = OAM_ZERO;
	UInt8 *p_temp_buf = OAM_NULL;
	UInt8 count  = OAM_ZERO; 
	UInt8 number_of_parent = OAM_ZERO;
	UInt8 parent_loop = OAM_ZERO;  
	UInt16 counter_list=OAM_ZERO;
	/*Read common Response Info */
	p_temp_buf  = p_api_buf ;

	p_temp_buf += 2;/*trans_id*/
	p_temp_buf += 2;/*src_id*/
	p_temp_buf += 2;/*dest_id*/
	api_id  = get_word_from_header((UInt8 *)(p_temp_buf));
	p_temp_buf += 2;/*api_id*/

	p_temp_buf += 2;/*api_size*/
	p_temp_buf += 1;/*api/interface version*/
	p_temp_buf += 1;/*reserved*/

	status = *((UInt8 *) p_temp_buf);
	p_temp_buf += 1;/*status*/

	error_code = get_word_from_header((UInt8 *)(p_temp_buf));
	p_temp_buf += 2;/*error code*/
	number_of_parent = p_temp_buf[0];
	p_temp_buf += 1;/*reserved*/
	p_temp_buf += 3;/*reserved*/


	length_error_string = *((UInt8 *)(p_temp_buf));
	p_temp_buf += 1;/*length error string*/

    /* OAM BCOM Code Comment Changes Start */
    if((OAM_DELETE_OBJECT_RESP  == api_id) && (OAM_ONE == spv_after_add_status))
    {
        spv_after_add_status = OAM_ZERO;
    }
    /* OAM BCOM Code Comment Changes End */

	/*checking of failure part */
	if (OAM_ZERO != status) {
		oam_print_header();
		oam_fprintf(stdout, "\nRequest Fail with Error Code: %d\n", error_code);

		if (OAM_ADD_OBJECT_RESP == api_id) {
			add_set_flag = 0;
			g_oam_cli_support_multi_instance = 0;
		}

        /*SPR 19513 FIXED START */
        if((OAM_SET_PARAMETER_VALUES_RESP == api_id) && (OAM_ONE == spv_after_add_status))
        {
            char temp_str[STR_LEN_256] ={0};
            UInt16 new_trans_id = ++qcli_trans_id;
            SInt16 index = oam_qcli_find_index(add_params.V_add[0]);/*find index in map table*/
            if(index >= 0)
            {
                snprintf(temp_str, STR_LEN_256, "%s.[%d]",
                        g_oam_qcli_param_mapping_table[index].parent, instance_no);
            }
            oam_send_delete_obj_req(temp_str,
                    new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
                    OAM_DELETE_OBJECT_REQ);

            g_oam_cli_support_multi_instance = 0;
        }
        /*SPR 19513 FIXED END */

		if (0 < length_error_string) {
			oam_memcpy(error_string, p_temp_buf, length_error_string);
			oam_fprintf(stdout, "\nError String: %s\n", error_string);
		}

		oam_print_line();
		return OAM_FAILURE;
	}
	/*checking of success part*/
	else {
        /*SPR 19513 FIXED START */
        if(OAM_ONE == spv_after_add_status)
            spv_after_add_status = OAM_ZERO;
        /*SPR 19513 FIXED END */
		oam_print_header();
		if (OAM_SET_PARAMETER_VALUES_RESP == api_id) {
			if(ue_rel_flag == OAM_ZERO) {
				PRINT_CLI_SPV_RESULT_SUCCESS
			}
			else
			{
				PRINT_CLI_SPV_RESULT_SUCCESS_UE_REL		    
			}
		}
		else if(OAM_GET_PARAMETER_VALUES_RESP == api_id ) 
		{
			for(parent_loop=0;parent_loop<(number_of_parent==0?1:number_of_parent);parent_loop++)
			{

				p_temp_buf += 2; /*parent info length*/ 
                /* SPR 15527 START */ 
				num_of_parameters = ((UInt16)*((UInt8 *)(p_temp_buf + 3)) <<8)
                        | ((UInt16)*((UInt8 *)(p_temp_buf)));
                /* SPR 15527 END */

                /* OAM BCOM Code Comment Changes Start */
                if(num_of_parameters > 1000)
                {
                   break;
                   /* Max Allowable Parameters Reached */
                }
                /* OAM BCOM Code Comment Changes End */

				if(num_of_parameters < 1)
				{
					PRINT_CLI_GPV_RESULT_SUCCESS_NO_DATA_TO_DISPLAY
						break;
				} 
				p_temp_buf += 1; /*num of param*/
				p_temp_buf += 1; /* stars index, not used here */
				p_temp_buf += 1; /* end index, not used here */
				p_temp_buf += 1; /* reserved */
				length_parent_name = get_word_from_header((UInt8 *)p_temp_buf); 
				p_temp_buf += 2; /*length of parent name*/
				if(length_parent_name < STR_LEN_256){
					oam_memcpy(parent_node_name,p_temp_buf,length_parent_name);
					parent_node_name[length_parent_name]='\0';	   
				}
				else{
					oam_fprintf(stderr, "\n Length of parent_name= %d should be < 256 \n",length_parent_name);
					return OAM_FAILURE;
				}
				p_temp_buf += length_parent_name;
				/*Read  Parameter1/leaf level Info*/
				for(count = 0; count < num_of_parameters; count++)
				{
					length_parameter_info = get_word_from_header((UInt8 *) p_temp_buf);  
					p_temp_buf += 2;/* length of param info*/
					p_temp_buf += 2;/* reserved*/
					if(length_parameter_info > 4)
						name_val_length = length_parameter_info - 4;/* -4 means 2 for length and 2 for reserved*/
                    /* SPR 17243 START */
                    oam_memset(name_val,'\0',STR_LEN_512);
                    /* SPR 17243 END */

					/* coverity 54901 */

                    /* SPR 17243 START */
                    if(name_val_length < STR_LEN_512 && name_val_length >0)
					{
                        /* SPR 17243 END */
						oam_memcpy(name_val,p_temp_buf,name_val_length+OAM_ONE);
					}
					else{
                    /* SPR 17243 START */
						oam_fprintf(stderr, "\nLength of name_val= %d should be < 512\n",name_val_length);
                    /* SPR 17243 END */
						return OAM_FAILURE;
					}	
                    /*spr 22474 start*/
                    if((oam_strncasecmp((char * )parent_node_name,"Debug.Get.L2.MAC",oam_strlen("Debug.Get.L2.MAC")) == 0) ||
					   (oam_strncasecmp((char * )parent_node_name,"Debug.Get.L2.RLC",oam_strlen("Debug.Get.L2.RLC")) == 0) ||
					   (oam_strncasecmp((char * )parent_node_name,"Debug.Get.L2.PDCP",oam_strlen("Debug.Get.L2.PDCP")) == 0) ||
						(oam_strncasecmp((char * )parent_node_name,"Debug.Get.L2.EGTPU",oam_strlen("Debug.Get.L2.EGTPU")) == 0) ||
						(oam_strncasecmp((char * )parent_node_name,"Debug.Get.L3.S1AP",oam_strlen("Debug.Get.L3.S1AP")) == 0) ||
						(oam_strncasecmp((char * )parent_node_name,"Debug.Get.L3.X2AP",oam_strlen("Debug.Get.L3.X2AP")) == 0) ||
						(oam_strncasecmp((char * )parent_node_name,"Debug.Get.L3.RRC",oam_strlen("Debug.Get.L3.RRC")) == 0) ||
						(oam_strncasecmp((char * )parent_node_name,"Debug.Get.RRM",oam_strlen("Debug.Get.RRM")) == 0) ||
						(oam_strncasecmp((char * )parent_node_name,"Debug.Get.SON",oam_strlen("Debug.Get.SON")) == 0) ||
					    (oam_strncasecmp((char * )parent_node_name,"Debug.Get.OAM.OAM",oam_strlen("Debug.Get.OAM.OAM")) == 0) ||
					   (oam_strncasecmp((char * )parent_node_name,"Debug.Get.OAM.TR069",oam_strlen("Debug.Get.OAM.TR069")) == 0))
					{
                        oam_fprintf(stdout,"%s\n",name_val);
                        p_temp_buf += name_val_length;
                        continue;
                    }
                    /*spr 22474 end*/
 
					name=strtok((char * )name_val,"=");
					value=strtok(NULL,"=");
                    /* SPR 21415 Fix Start */
					if (oam_strncasecmp((char * )parent_node_name,"L3 RRC KPI",oam_strlen("L3 RRC KPI")) == 0 || 
							oam_strncasecmp((char * )parent_node_name,"L2 PDCP STATS",oam_strlen("L2 PDCP STATS")) == 0 ||
                            /* SPR 24124/SES-507 Fix Start */
							oam_strncasecmp((char * )parent_node_name,"Show.Stats.L2.RLC",oam_strlen("Show.Stats.L2.RLC")) == 0 ||
							oam_strncasecmp((char * )parent_node_name,"MAC KPI",oam_strlen("MAC KPI")) == 0)
                            /* SPR 24124/SES-507 Fix End */
                        /* SPR 21415 Fix End */
					{
						oam_fprintf(stdout,"%-25s\n",name);
						break;
					}
					if((NULL != name) && (NULL != value))
					{
#ifdef OAM_RADIO_HEAD_SUPPORT
#ifdef OAM_RRH_TYPE_1_SUPPORT
						else if(!oam_strncasecmp((Char8*)parent_node_name,
									"RRH", oam_strlen("RRH"))) {
							format_rrh_parameters(name, value);
						}
						else
#endif
#endif
                            /* spr_21013_fix_start */
							if(oam_strncasecmp((char * )parent_node_name,"InternetGatewayDevice.FAPService.FaultMgmt",oam_strlen("InternetGatewayDevice.FAPService.FaultMgmt")) != 0)
                            /* spr_21013_fix_end */
							{
								/*BUG ID: 535*/ 
								if (!(strncmp(name, "AdminState", oam_strlen("AdminState"))))
									oam_fprintf(stdout,"%s.%s\t=\t%-20s\n",parent_node_name,name, oam_atoi(value) == 1 ? "UNLOCKED":"LOCKED");
								else if (!(strncmp(name, "OpState", oam_strlen("OpState"))))
									oam_fprintf(stdout,"%s.%s\t=\t%-20s\n",parent_node_name,name, oam_atoi(value) == 1 ? "ENABLED":"DISABLED");
								else if (!(strncmp(name, "RFTxStatus", oam_strlen("RFTxStatus"))))
									oam_fprintf(stdout,"%s.%s\t=\t%-20s\n",parent_node_name,name, oam_atoi(value) == 1 ? "ON":"OFF");
								else if (!(strncmp(name, "AvailabilityStatus", oam_strlen("AvailabilityStatus"))))
								{
									switch (oam_atoi(value))
									{
										case 0:
											value = "INITIALIZING"; 
											break; 
										case 1:
											value = "AVAILABLE"; 
											break; 
										case 2:
											value = "DEGRADED"; 
											break; 
										case 3:
											value = "FAILED"; 
											break; 
										default:
											break; 

									}
									oam_fprintf(stdout,"%s.%s\t=\t%-20s\n",parent_node_name,name,value);
								}   
								else if (!(strncmp(name, "CellBarred", oam_strlen("CellBarred"))))
								{
									switch (oam_atoi(value))
									{

										case 0:
              									/* SPR 16407 START */
										value = "Cell-Unbarred";
										break;
										case 1:
										value = "Cell-Barred";
										/* SPR 16407 END */
										break; 
										default:
										break; 
									}
									oam_fprintf(stdout,"%s.%s\t=\t%-20s\n",parent_node_name,name,value);
								}
                                else
                                {   
                                    /* Spr 15438 Fix Start */
                                    oam_fprintf(stdout,"%s.%s  =  %-20s\n",parent_node_name,name,value);
                                }   
                                /*coverity 109018 fix start*/
                                if (!(strncmp(name, "X_VENDOR_INSTANCE_ID", oam_strlen("X_VENDOR_INSTANCE_ID"))))
                                /*coverity 109018 fix end*/
                                {
                                    oam_print_line();
                                }   
                                /* Spr 15438 Fix End*/


							}  
							else{

								if(!strncmp(name, "AlarmID", oam_strlen("AlarmID"))) 
								{
									oam_print_line();
								}
								if(!strncmp(name, "EventTime", oam_strlen("EventTime")))
								{
									Set_Flag=1;
									counter_list++;
									oam_fprintf(stdout,"\n%s[%d]\n",parent_node_name,counter_list);
								}
								else if(!strncmp(name, "AlarmIdentifier", oam_strlen("AlarmIdentifier")) && (Set_Flag==0))
								{
									counter_list++;
									oam_fprintf(stdout,"\n%s[%d]\n",parent_node_name,counter_list);
								}
								else if(!strncmp(name, "AdditionalInformation", oam_strlen("AdditionalInformation")))
									Set_Flag=0;

								oam_fprintf(stdout,"%s=%-20s\n",name,value);
							}
					}
					p_temp_buf += name_val_length;
				}
                /* Spr 15438 Fix Start */
                /* Code Removed */
                /* Spr 15438 Fix End*/
                counter_list=0;
			}
		}
		else if(OAM_DELETE_OBJECT_RESP == api_id ) 
		{
			PRINT_CLI_DELETE_OBJECT_RESULT_SUCCESS
		}
		else if(OAM_ADD_OBJECT_RESP == api_id ) 
		{
			oam_send_set_req_after_add_resp(p_api_buf);
		}
		else
		{
			oam_fprintf(stdout,"status success but apiID unknown");
			oam_print_line();
		} 
	}
	oam_print_line();
	return OAM_SUCCESS;
}


/*--------Rel:1.0 generic code for QCLI Commands  ---------------------------*/
/******************************************************************************
 *        FUNCTION NAME     :  oam_qcli_fill_param
 *        DESCRIPTION       :  fill param in given structure
 *                              
 *        INPUTS            : C (number of arguments received from qcli command
 *                          : V (Variable number of arguments)
 *                          : index (index number of map table)
 *                          : pp_param (pointer to valid param,value structure)
 *                          :
 *        OUTPUTS           : 
 *        RETURNS           : number of parameters filled
 * ********************************************************************************/
UInt16 oam_qcli_fill_param(UInt16 C,char * const *V,UInt16 index,param_set **p_param)
{
	UInt16 num_valid_param=OAM_ZERO;
	UInt16 loop_cntr=OAM_ZERO,token_index=OAM_ZERO,param_cntr=OAM_ZERO;
	char *param_token[100];
	char * temp=OAM_NULL,
	     temp_val[5],
	     temp_str[5];
	UInt16 temp_len=OAM_ZERO;
	param_set *param = NULL;
	UInt32 val;
	oam_memset(temp_val, OAM_ZERO, sizeof(temp_val));
	oam_memset(temp_str, OAM_ZERO, sizeof(temp_str));
	/*in case of OAM_GET_PARAMETER_VALUE we need only param*/
	if(g_oam_qcli_param_mapping_table[index].cmd_type == OAM_GET_PARAMETER_VALUES)
	{
        if (g_oam_cli_support_multi_instance)
        {
            /* In case of multi-instance parameters V[C-1] has the 
             * instance id number, which is not part of parameter list */
            --C;
        }
		*p_param = (param_set *) qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,
				(C-1)*sizeof(param_set));
		if(NULL==*p_param)
		{
			return OAM_FAILURE;
		}
		param=*p_param;
		for(loop_cntr=1;loop_cntr<C;loop_cntr++)
		{
			oam_strcpy(param->param,V[loop_cntr]);/*copy value */ 
			param_cntr++; 
			param++;
		}
	}
	else if((g_oam_qcli_param_mapping_table[index].cmd_type == OAM_SET_PARAMETER_VALUES) || (g_oam_qcli_param_mapping_table[index].cmd_type ==OAM_DELETE_OBJECT_REQ ) || ((g_oam_qcli_param_mapping_table[index].cmd_type) & (OAM_ADD_OBJECT_REQ)))
	{
		for(loop_cntr=1;loop_cntr<C;loop_cntr++)
		{
			/*comparing for Default value if default recieved it means invalid
			  we donot need to send that param*/
			if(oam_strcmp(V[loop_cntr],OAM_CLI_DEFAULT_PARAM_VALUE)!=0)
				num_valid_param++;
		}
		*p_param = (param_set *) qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,
				num_valid_param * sizeof(param_set));
		if(NULL==*p_param)
		{
			return OAM_FAILURE;
		}
		param = *p_param;
		temp_len =oam_strlen(g_oam_qcli_param_mapping_table[index].param_list)+1;/*+1 for null char*/
		temp = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,temp_len); 
		if(NULL==temp)
		{
			return OAM_FAILURE;
		}
		oam_memset(temp,0,temp_len);
		oam_memcpy(temp,g_oam_qcli_param_mapping_table[index].param_list,temp_len-1);
		temp[temp_len-1] = '\0';

		/* creating tokens of ',' seperated parameters string*/
		param_token[token_index]= oam_strtok(temp,",");
		while(param_token[token_index]!=NULL)
		{
			token_index++;
			param_token[token_index]=oam_strtok(NULL,",");
		}

		qvMsgFree(temp);/*free memory taken by temp*/
		for(loop_cntr=1;loop_cntr<((C-1)<=token_index?C:token_index);loop_cntr++)
		{
			/*maintaing only param,value set having valid values*/
			if(oam_strcmp(V[loop_cntr],OAM_CLI_DEFAULT_PARAM_VALUE)!=0)
			{
				oam_strcpy(param->param,param_token[loop_cntr-1]);/*copying param*/
				if((strncmp(param->param,"Action",oam_strlen(param->param)+1)==0) && flag ==1)
				{
					oam_strncpy((char *) temp_val,(char *) V[loop_cntr], sizeof(temp_val)-1); /*Cov CID 33227 Fix*/
					val= oam_atol(temp_val); 
					val += 40;
					snprintf(temp_str, 5, "%d", val);
					oam_strcpy(param->value,(char *)temp_str);
					flag=0;
				}
				else
				{
				oam_strcpy(param->value,V[loop_cntr]);/*copying value*/
				}
				param_cntr++;
				param++;
			}
		}
	}
	else
		return OAM_FAILURE;

	return param_cntr;/*returning number of valid param filled*/
}

/******************************************************************************
 *      FUNCTION NAME     :  oam_qcli_update_cache
 *      DESCRIPTION       :  update cache table 
 *                          
 *      INPUTS            :  index (array index of g_oam_qcli_param_mapping_table structure)
 *                   
 *                        : 
 *                        :
 *                        :
 *       OUTPUTS          : 
 *       RETURNS          : NOTHING
 ********************************************************************************/
void oam_qcli_update_cache(UInt16 index)
{
	/*updating cache param from main mapping table*/ 
	mapping_cache[cache_track_index].mode = g_oam_qcli_param_mapping_table[index].mode;
	mapping_cache[cache_track_index].index = index;

	/*if cache is not full increment 'is_cache_full' counter*/
	if(is_cache_full<MAX_CACHE_LIMIT)
		is_cache_full++;
	/*cache_track_index keep track where to add new entry*/
	if(cache_track_index < (MAX_CACHE_LIMIT-1))
	{
		cache_track_index++;/*incrementing cache_track_index*/
	} 
	else
	{
		cache_track_index=0;
	}
}
/******************************************************************************
 *    FUNCTION NAME     :  oam_qcli_find_index
 *    DESCRIPTION       :  find mapping table index for particular mode
 *                       
 *    INPUTS            :  current_mode (mode recieved in V[0] of qcli command)
 *                  
 *                      : 
 *                      :
 *                      :
 *    OUTPUTS           : 
 *    RETURNS           : index number
 * ******************************************************************************/
SInt16 oam_qcli_find_index(void *current_mode)
{
	UInt16 i=0;

	/* searching mode in cache table*/
	for(i=0;i<((is_cache_full<MAX_CACHE_LIMIT)?
				cache_track_index:MAX_CACHE_LIMIT);i++)
	{
		if(mapping_cache[i].mode)
		{
			/*comparing mode*/
			if(oam_strcmp(mapping_cache[i].mode,current_mode)== 0)
			{

				/*return index number of main mapping table*/
				return mapping_cache[i].index;
			}
		}
		else
			break;
	} 
	/*if mode is not found in cache table searching in main mapping table*/ 
	for(i=0;i < OAM_MAX_NUMBER_ELEMENT; i++)
	{
		if(g_oam_qcli_param_mapping_table[i].mode)
		{
			/*comparing mode*/
			if(oam_strcmp(g_oam_qcli_param_mapping_table[i].mode,current_mode)== 0)
			{
				/*updating the cache table for future*/
				oam_qcli_update_cache(i);
				return i;/*return index*/
			}
		}
		else
			break;
	}

	return -1; 
}

/******************************************************************************
 * *   FUNCTION NAME     :  oam_qcli_cli_validator
 * *   DESCRIPTION       :  compose qcli cmd and send to oam
 * *                      
 * *   INPUTS            :  V
 * *   OUTPUTS           : 
 * *   RETURNS           : 
 * ******************************************************************************/
UInt8 oam_qcli_cli_validator(char * const *V)
{
	if ((!strncmp(V[0],"CellMgmt Delete", oam_strlen("CellMgmt Delete"))) || 
			(!strncmp(V[0],"CellMgmt Setup", oam_strlen("CellMgmt Setup"))) ||
			(!strncmp(V[0],"CellMgmt Start", oam_strlen("CellMgmt Start"))) ||
			(!strncmp(V[0],"CellMgmt Stop", oam_strlen("CellMgmt Stop"))) ||
			(!strncmp(V[0],"CellMgmt Block", oam_strlen("CellMgmt Block"))) ||
			(!strncmp(V[0],"CellMgmt Unblock", oam_strlen("CellMgmt Unblock")))
	   )
	{
		if(oam_strncasecmp(V[1],"current", oam_strlen("current")))
			return OAM_ZERO;
	} 
	if ((!strncmp(V[0],"Config LoadReporting OverLoadLevel", oam_strlen("Config LoadReporting OverLoadLevel"))))
		flag = 1;
	return OAM_ONE;
}

/******************************************************************************
 * *   FUNCTION NAME     :  oam_qcli_send_command_no_param
 * *   DESCRIPTION       :  compose qcli cmd and send to oam
 * *                      
 * *   INPUTS            :  C
 * *                     :  V
 * *                     :  E
 * *                     :  T
 * *                     :  arg
 * *   OUTPUTS           : 
 * *   RETURNS           : 
 * ******************************************************************************/
char *
oam_qcli_send_command_no_param
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    qcliHandleUnusedParameter(E);
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);
    /* SPR 17777 FIX END */

    /*Cov CID 63679 Fix*/
	UInt16 new_trans_id = OAM_ZERO;

	UInt8 number_of_param=OAM_ZERO;
	Char8 *p_token = OAM_NULL;

	new_trans_id = ++qcli_trans_id;
	SInt16 index = oam_qcli_find_index(V[0]);/*find index in map table*/

	/*if no index found means entry not present in our database for that command*/
	if(index == -1)
		return OAM_ZERO;

	param_set *param = OAM_NULL;   

	if (OAM_FAILURE == oam_mem_alloc(sizeof(param_set),(void *)&param,&error_code))
	{
		/* Not enough memory */
       oam_fprintf(stderr, "\nError in Memory Allocation with error code %d\n",error_code);
		OAM_FUN_EXIT_TRACE();
		return OAM_FAILURE;
	}
	oam_memset(param, OAM_ZERO, sizeof(param_set));

	if(OAM_SET_PARAMETER_VALUES == g_oam_qcli_param_mapping_table[index].cmd_type)   
	{
		if(strstr(V[0], "AlarmHistory")) {
			p_token = oam_strrchr(V[0], ' ');
			if(!p_token) {
                    /* SPR  21280 FIXED START */
                    /* Code Remove */  
                    /*SPR  21280 FIXED END */
	                        /* Coverity ID : 72336 Fix Start*/
  	                        oam_mem_free(param,&error_code);
	                        /* Coverity ID : 72336 Fix End*/
				return OAM_ZERO;/* Coverity FIX 54275 */
			}
			++p_token;
            oam_strncpy(param->param, p_token, sizeof(param->param)-1); /* Cov CID 56657 Fix*/ 
            oam_strncpy(param->value, "0",sizeof(param->value)-1); /* Cov CID 56655 Fix*/
			number_of_param = 1;
		}
	}
/* SPR 13805 START */
    else if(OAM_GET_PARAMETER_VALUES == g_oam_qcli_param_mapping_table[index].cmd_type)
    {
        p_token = oam_strrchr(V[0], ' ');
        if(!p_token)
        {
            /*coverity 72336 fix start*/
            oam_mem_free((void *)param,&error_code);
            /*coverity 72336 fix end*/
            oam_fprintf(stderr, "\nInternal NULL pointer error\n");
            return OAM_ZERO;
        }
        ++p_token;
        oam_strncpy(param->param, p_token, sizeof(param->param)-1); 
        number_of_param = 1;
    }
/* SPR 13805 END */
	oam_send_get_set_req(number_of_param,param,g_oam_qcli_param_mapping_table[index].parent,
			new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			g_oam_qcli_param_mapping_table[index].cmd_type);

	if(OAM_FAILURE == oam_mem_free((void *)param,&error_code))
	{
       oam_fprintf(stderr, "\n Error in memory free : %d \n",error_code);
	}

	return OAM_ZERO;
}


/******************************************************************************
 *   FUNCTION NAME     :  oam_qcli_send_command
 *   DESCRIPTION       :  compose qcli cmd and send to oam
 *                      
 *   INPUTS            :  C
 *                     :  V
 *                     :  E
 *                     :  T
 *                     :  arg
 *   OUTPUTS           : 
 *   RETURNS           : 
 ******************************************************************************/

char *
oam_qcli_send_command
(
 int C,
 char *const *V,
 const char *const *E,
 const char *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(T);
    qcliHandleUnusedParameter(arg);
    /* SPR 17777 FIX END */

	UInt16 new_trans_id = OAM_ZERO;
	UInt8 number_of_param=OAM_ZERO;
	UInt8 retVal = OAM_ZERO;
	UInt8 validate_status = oam_qcli_cli_validator(V);
	if ( OAM_ZERO == validate_status)
	{
       oam_fprintf(stderr,"Parameter Validation fail\n");
		return OAM_ZERO;
	}


	new_trans_id = ++qcli_trans_id;
	SInt16 index = oam_qcli_find_index(V[0]);/*find index in map table*/

	/*if no index found means entry not present in our database for that command*/
	if(index == -1)
		return OAM_ZERO;

	param_set *param=NULL;
	ue_rel_flag = OAM_ZERO;
    if(!oam_strcmp(V[0], "RRM UE RELEASE"))
		ue_rel_flag = OAM_ONE;


	if (g_oam_qcli_param_mapping_table[index].cmd_type != OAM_DELETE_OBJECT_REQ)
	{
		/*Fill all the valid param values in 'param' address*/
        /* SPR 14217 Fix Start */
		number_of_param = oam_qcli_fill_param(C,V,index,&param);
        /* SPR 14217 Fix End */


		/*if there is no number of param then no need to send*/ 
		if(number_of_param<=0)
		{
			g_oam_cli_support_multi_instance = 0;
			/* coverity 63296 */
			if (NULL != param)
				qvMsgFree(param);/*free memory that was allocated in oam_qcli_fill_param*/
			param = NULL;

			return OAM_ZERO;
		}
	}
	if(
			((g_oam_qcli_param_mapping_table[index].cmd_type == OAM_SET_PARAMETER_VALUES) || 
			 (g_oam_qcli_param_mapping_table[index].cmd_type ==   OAM_DELETE_OBJECT_REQ)  || 
			 ((g_oam_qcli_param_mapping_table[index].cmd_type) & (OAM_ADD_OBJECT_REQ)))   && 
			(g_oam_qcli_param_mapping_table[index].cmd_type != OAM_GET_PARAMETER_VALUES) 
	  )
	{
        if(OAM_ZERO == strncmp(V[0],"SON",oam_strlen("SON")))
		{
			retVal = OAM_ZERO;
		}
        else if(OAM_ZERO == strncmp(V[0],"Config Kpi ALL",oam_strlen("Config Kpi ALL")))
		{
			retVal = OAM_ZERO;
		}
        /* SPR_19279_START */
        else if(OAM_ZERO == strncmp(V[0],"Config InternetGatewayDevice Services FAPService CellConfig LTE EPC PLMNList DELETE INFO",
                           oam_strlen("Config InternetGatewayDevice Services FAPService CellConfig LTE EPC PLMNList DELETE INFO")))
		{
			retVal = OAM_ZERO;
		}
        /* SPR_19279_END */
		else
		{
            if(OAM_NULL != param)        
			{
				retVal = oam_validate_special_param(V,number_of_param,param);
			}
			else
			{
               oam_fprintf(stderr," param equals null !");
			}

		}
		switch(retVal) 
        {
            case OAM_ZERO:
            break;
            case OAM_ONE:
           oam_fprintf(stdout,"\nCommand cancelled...\n"); 
            /* Coverity Fix 63296 start */
            if (NULL != param)
                qvMsgFree(param);/*free memory that was allocated in oam_qcli_fill_param*/
            param = NULL;
            /* Coverity Fix 63296 end */
            return OAM_ZERO;
            break;

            default:      
           oam_fprintf(stdout,"\nerror in validate\n");
            g_oam_cli_support_multi_instance = 0;
            /* Coverity Fix 63296 start */
            if (NULL != param)
                qvMsgFree(param);/*free memory that was allocated in oam_qcli_fill_param*/
            param = NULL;
            /* Coverity Fix 63296 end */
            return OAM_ZERO;
        }
	}

	if(add_set_flag && (NULL != E))
	{
		if((g_oam_qcli_param_mapping_table[index].cmd_type) &  (OAM_ADD_OBJECT_REQ))
		{
			oam_send_add_obj_req(g_oam_qcli_param_mapping_table[index].parent,
					new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
					OAM_ADD_OBJECT_REQ);
			g_oam_cli_support_multi_instance = 0;
			if (NULL != param)
				qvMsgFree(param);/*free memory that was allocated in oam_qcli_fill_param*/
			param = NULL;	
			return OAM_ZERO;
		}
	}

	if(g_oam_cli_support_multi_instance == 1)
	{
		char temp_str[256] ={0};
		if((strstr(V[0],"RF")) != NULL)
		{ 
			/*SPR 12727 START */
			snprintf(temp_str,256,"%s.[%d].RF",
					g_oam_qcli_param_mapping_table[index].parent,oam_atoi(V[C-1]));
			/*SPR 12727 END */
		}
		else if(strstr(V[0], "SON NR ") ||
                strstr(V[0], "SON ANR MEAS-CONFIG ")) {
			if(add_set_flag) {
				snprintf(temp_str, 256, "%s.[%d]",
						 g_oam_qcli_param_mapping_table[index].parent,
                         instance_no);
            }
			else {
				snprintf(temp_str, 256, "%s.[%d]",
						g_oam_qcli_param_mapping_table[index].parent,
						oam_atoi(V[C]));
            }
		}
#ifdef LTE_EMBMS_SUPPORTED
        else if (oam_strstr(V[0], "SFConfigList"))
        {
            snprintf(temp_str, 256, "%s.[%d]",
                     g_oam_qcli_param_mapping_table[index].parent,
                     oam_atoi(V[C - 1]));
        }
        /* Coverity 101114 Fix Start */
#ifdef TDD_CONFIG
        else if(oam_strstr(V[0], "MBMS Config TDD SIB2"))
        {
            snprintf(temp_str, 256, "%s.[%d]",
                     g_oam_qcli_param_mapping_table[index].parent,
                     oam_atoi(V[C]));
        }
#endif
        /* Coverity 101114 Fix End */

#endif

        /* SPR_19279_START */
        else if(strstr(V[0], "Config InternetGatewayDevice Services FAPService CellConfig LTE EPC PLMNList")) {
            if(add_set_flag) {
                snprintf(temp_str, 256, "%s.[%d]",
                        g_oam_qcli_param_mapping_table[index].parent,
                        instance_no);
            }
            else {
                snprintf(temp_str, 256, "%s.[%d]",
                        g_oam_qcli_param_mapping_table[index].parent, atoi(V[C]));  
            }
        }
        /* SPR_19279_END */
        else {
/*SPR 12727 START */
            snprintf(temp_str, 256, "%s.[%d].BCCH",
                     g_oam_qcli_param_mapping_table[index].parent,
                     oam_atoi(V[C-1]));
/*SPR 12727 END */
		}

		if((g_oam_qcli_param_mapping_table[index].
                    cmd_type == OAM_DELETE_OBJECT_REQ)) {
			oam_send_delete_obj_req(temp_str, new_trans_id,
                                    QCLI_MODULE_ID, QCLI_INTERFACE_MODULE_ID,
                                    OAM_DELETE_OBJECT_REQ);
			g_oam_cli_support_multi_instance = 0;
			return OAM_ZERO;
		}
		else {
			if (add_set_flag) {
				g_oam_qcli_param_mapping_table[index].
                              cmd_type = OAM_SET_PARAMETER_VALUES;
                /* SPR 19513 FIXED START */
                spv_after_add_status = OAM_ONE;
                /* SPR 19513 FIXED END */
			}

			if(OAM_NULL != param)                       {
				oam_send_get_set_req(number_of_param,param,temp_str,
						new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
						g_oam_qcli_param_mapping_table[index].cmd_type);
			}
			else
			{
               oam_fprintf(stderr," param equals null !");
			}
			if(add_set_flag)
				add_set_flag=0;
			g_oam_cli_support_multi_instance = 0;
			if(NULL != param)
				qvMsgFree(param);
			param = NULL;
			return OAM_ZERO;
		}
	}

	else
	{
        /*SPR 19513 FIXED START*/
        if(add_set_flag)
        {
            g_oam_qcli_param_mapping_table[index].cmd_type = OAM_SET_PARAMETER_VALUES;
            spv_after_add_status = OAM_ONE;
        }
        /*SPR 19513 FIXED END*/
		if(OAM_NULL != param)                   {
			oam_send_get_set_req(number_of_param,param,g_oam_qcli_param_mapping_table[index].parent,
					new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
					g_oam_qcli_param_mapping_table[index].cmd_type);
		}
		else
		{
           oam_fprintf(stderr," param equals null !"); 
		}
	}

	g_oam_cli_support_multi_instance = 0;
	if(NULL != param)
		qvMsgFree(param);/*free memory that was allocated in oam_qcli_fill_param*/
	param = NULL;	
	return OAM_ZERO;
}





/******************************************************************************
 *   FUNCTION NAME     :  oam_send_get_set_req
 *   DESCRIPTION       :  send GEt / SET REQ to oam 
 *
 *   INPUTS            :  no_of_param 
 *                     :  params
 *                     :  parent
 *                     :  trans_id
 *                     :  src_id
 *                     :  dest_id
 *                     :  api_id   
 *   OUTPUTS           :
 *   RETURNS           : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/

oam_return_et
oam_send_get_set_req
(
 UInt8 number_of_param,
 param_set *param, 
 char *parent,
 UInt16 trans_id,
 UInt16 src_id,
 UInt16 dest_id,
 UInt16 api_id
 )
{
	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt8 *p_msg=NULL, *p_api_msg=NULL;
	oam_error_code_et error_code = NO_ERROR;
	/* Finding Msg length*/
	msg_length = oam_qcli_message_length(number_of_param,param,parent,api_id);

	/*msg_api_length is complete msg including CSPL header QCLI header*/ 
	msg_api_length = msg_length  + OAM_CSPL_HDR_SIZE + OAM_MGMT_INTF_HEADER_SIZE ;

	/* Allocating memory for msg buffer*/
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);	
	if(NULL==p_msg)
	{			 
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_api_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_api_msg,OAM_VERSION,src_id,
			dest_id,
			api_id,msg_api_length);
	p_api_msg = p_api_msg + OAM_CSPL_HDR_SIZE;

	/* Fill QCLI header */
	oam_construct_qcli_header(p_api_msg,trans_id,src_id,
			dest_id,
			api_id,msg_api_length-OAM_CSPL_HDR_SIZE);
	p_api_msg = p_api_msg + OAM_MGMT_INTF_HEADER_SIZE ;

	/* Composing payload for msg*/
	oam_qcli_compose_payload(p_api_msg,number_of_param,param,parent,
			msg_length,api_id);

	message_send.message_send = 1;
	if(OAM_SET_PARAMETER_VALUES == api_id)
		message_send.apiid_expected = OAM_SET_PARAMETER_VALUES_RESP;
	else if(OAM_GET_PARAMETER_VALUES == api_id)
		message_send.apiid_expected = OAM_GET_PARAMETER_VALUES_RESP; 

	/* send msg to OAM*/
	if(OAM_FAILURE == oam_qcli_intf_send_message(p_msg,dest_id,&error_code))
	{
		return OAM_FAILURE;
	}
	return OAM_SUCCESS;
}
/******************************************************************************
 *   FUNCTION NAME     :  oam_qcli_message_length
 *   DESCRIPTION       :  calculate qcli msg length
 *
 *   INPUTS            :  number_of_param  (number of parameters to be sent to oam)
 *                     :  param (structure of param value pair that needs to send)
 *                     :  parent
 *                     :  api_id
 *   OUTPUTS           :
 *   RETURNS           : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
UInt16 
oam_qcli_message_length
(
 UInt8 number_of_param,
 param_set *param,
 char *parent,
 UInt16 api_id
 )
{
	UInt16 length=OAM_ZERO;
	UInt8 i = OAM_ZERO;

	length +=  sizeof(UInt16) /*length of parent info*/
		+sizeof(UInt8)  /*Number of param*/
		+sizeof(UInt8)  /*reserved*/
		+sizeof(UInt16) /*reserved*/
		+sizeof(UInt16) /*length of parent node*/
		+oam_strlen(parent)/*length of parent string*/
		+1;/*+1 if for Null char along with parent*/

	for(i=0;i<number_of_param;i++)
	{
		length += sizeof(UInt16) /*length of parameter info*/
			+sizeof(UInt16); /*Reserved*/   
		length += oam_strlen(param[i].param);/*parameter length*/
		if(OAM_SET_PARAMETER_VALUES == api_id)
		{
			length += 1;/*1 byte for '=' sign*/
			length += oam_strlen(param[i].value);/*value length*/
		}
		length += 1;/*1 for null char at end of string*/
	}
	return length;
}
/******************************************************************************
 *   FUNCTION NAME     :   oam_qcli_compose_payload
 *   DESCRIPTION       :   used to compose payload for qcli command
 *
 *   INPUTS            :   p_msg (buffer that need to fill)  
 *                     :   number_of_param 
 *                     :   param (structure of valid param,value set)
 *                     :   parent
 *                     :   msg_length (length of msg )
 *                     :   api_id 
 *   OUTPUTS           :
 *   RETURNS           : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/

void oam_qcli_compose_payload
(
 UInt8 *p_msg,
 UInt8 number_of_param,
 param_set *param,
 char * parent,
 UInt16 msg_length,
 UInt16 api_id
 )
{
	UInt16 offset = OAM_ZERO;
	UInt16 parent_len = OAM_ZERO;;
	UInt16 i = OAM_ZERO;
	UInt16 param_set_len = OAM_ZERO;
	UInt16 len_par=OAM_ZERO,len_val=OAM_ZERO;

	p_msg[offset++]=(msg_length & 0xFF00)>>8;/*length of parent information*/
	p_msg[offset++]=(msg_length & 0x00FF);
	p_msg[offset++]=number_of_param;  /*number of parameters in payload*/
	if (oam_strncasecmp(parent,"FM.Alarm.Clean",oam_strlen("FM.Alarm.Clean")) == OAM_ZERO)
	{
		p_msg[offset++]=0x00;                  /*start index*/
		p_msg[offset++]=(65535 & 0xFF00)>>8;   /*end index*/
		p_msg[offset++]=(65535 & 0x00FF);
	}
	else
	{
		p_msg[offset++]=0x00;             /*reserved*/
		p_msg[offset++]=0x00;             /*reserved*/
		p_msg[offset++]=0x00;             /*reserved*/
	}
	parent_len  =  oam_strlen(parent)/*length of parent node name*/
		+1;/*1 for null char at end of parent node name string*/
	p_msg[offset++]=(parent_len & 0xFF00)>>8;
	p_msg[offset++]=(parent_len & 0x00FF);
	oam_memcpy(p_msg+offset,parent,parent_len-1);
	p_msg[offset+parent_len]='\0';/*null cahr at the end of parent*/

	offset += parent_len;/*incrementing offset by parent_len*/

	for(i=0;i<number_of_param;i++)
	{
		param_set_len = OAM_ZERO;
		param_set_len += sizeof(UInt16)    /*length of param info*/
			+sizeof(UInt16);  /*reserved field*/

		len_par =  oam_strlen(param[i].param);/*length of param*/

		/* for OAM_SET_PARAMETER_VALUES 'param=value' pair required
		   for OAM_GET_PARAMETER_VALUES only 'param' is required*/
		if(OAM_SET_PARAMETER_VALUES == api_id)
		{
			len_val =  oam_strlen(param[i].value);/*length of value*/
			param_set_len += len_par + len_val + 1;/*1 for '=' sign*/
		}
		else/*OAM_SET_PARAMETER_VALUES*/
			param_set_len += len_par;

		param_set_len ++;  

		p_msg[offset++]=(param_set_len & 0xFF00)>>8;/*length of param info*/
		p_msg[offset++]=(param_set_len & 0x00FF);
		p_msg[offset++]=0x00;  /*reserved*/
		p_msg[offset++]=0x00;  /*reserved*/
		oam_memcpy(p_msg+offset,param[i].param,len_par);
		offset += len_par;/*increamenting offset by length of param*/
		if(OAM_SET_PARAMETER_VALUES == api_id)
		{
			p_msg[offset++]='=';/*for '=' sign in 'param=value' pair*/ 
			oam_memcpy(p_msg+offset,param[i].value,len_val);
			offset += len_val; /*incrementing offset by length of value*/
		}
		p_msg[offset++] = '\0';/*for null char at end of string*/
	}		
}

/******************************************************************************
 *    FUNCTION NAME     :  qcli_oam_hdle_rrc_config_resp
 *    DESCRIPTION       :  handle rrc config resp 
 *
 *    INPUTS            :  number_of_param  (number of parameters to be sent to oam)
 *                      :  param (structure of param value pair that needs to send)
 *                      :  parent
 *                      :  api_id
 *    OUTPUTS           :  None
 *    RETURNS           : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/


oam_return_et
qcli_oam_hdle_rrc_config_resp
(
 void *p_api
 )
{
	oam_rrc_config_stats_resp_t p_rrc_config_stats_resp;

	p_api = p_api + 10;

	/* Getting the response value */
	oam_cp_unpack_UInt8((void *)&(p_rrc_config_stats_resp.response),p_api
			, "p_rrc_config_stats_resp->response"
			); 

	if (p_rrc_config_stats_resp.response == OAM_ONE)
	{
		oam_fprintf(stdout, "Response Received : RRC CONFIG RESP SUCCESS[%d]",p_rrc_config_stats_resp.response);
	}
	else if(p_rrc_config_stats_resp.response == OAM_ZERO)
	{
		oam_fprintf(stdout, "Response Received : RRC CONFIG RESP Failure[%d]",p_rrc_config_stats_resp.response);
	}
	else
	{
		oam_fprintf(stdout, "Response Received : RRC CONFIG Recieved Response is not correct");
	}
	return OAM_SUCCESS;
}

/******************************************************************************
 *    FUNCTION NAME     :  qcli _send_rrc_config_req 
 *    DESCRIPTION       :  calculate qcli msg length
 * 
 *    INPUTS            :  number_of_param  (number of parameters to be sent to oam)
 *                      :  param (structure of param value pair that needs to send)
 *                      :  parent
 *                      :  api_id
 *    OUTPUTS           :  None
 *    RETURNS           : OAM_SUCCESS/OAM_FAILURE
 *******************************************************************************/
oam_return_et
qcli_send_rrc_config_req
(
 UInt16 recv_cli_val
 )
{
	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt16 new_trans_id = OAM_ZERO;


	UInt8 *p_msg, *p_show_msg;
	new_trans_id = ++qcli_trans_id;

	msg_length = sizeof(UInt16);

	msg_api_length =  msg_length + QCLI_HEADER_SIZE + OAM_CSPL_HDR_SIZE;

	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);

	if(OAM_NULL == p_msg)
	{
		/* Not enough memory */
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_show_msg = p_msg;
	/* Fill CSPL header */
	oam_construct_api_header(p_show_msg,OAM_VERSION,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			CONFIG_RRC_STATS_REQ,msg_api_length);

	/* Fill QCLI header */
	p_show_msg = p_show_msg + OAM_CSPL_HDR_SIZE;
	oam_construct_qcli_header(p_show_msg,new_trans_id,QCLI_MODULE_ID,QCLI_INTERFACE_MODULE_ID,
			CONFIG_RRC_STATS_REQ,msg_api_length - OAM_CSPL_HDR_SIZE);
	/* Fill payload */
	p_show_msg = p_show_msg + QCLI_HEADER_SIZE;


	oam_memcpy(p_show_msg,&recv_cli_val,sizeof(recv_cli_val));
	if( OAM_FAILURE  == oam_qcli_intf_send_message(p_msg,QCLI_INTERFACE_MODULE_ID,&error_code))
	{ 
		return OAM_FAILURE;     
	}
	return OAM_SUCCESS;
}

/******************************************************************************
 * FUNCTION NAME     :  oam_qcli_send_no_param_get_set_commands 
 * DESCRIPTION       :  compose certain qcli cmd which have no parameters and 
 *                      need specific handling. Initially implemented for SON
 *                      Specific commands but can be used elsewhere.            
 *                    
 * INPUTS            :  C : number of params
 *                   :  V : parameter array
 *                   :  E
 *                   :  T
 *                   :  arg
 * OUTPUTS           :  none
 * RETURNS           :  string
 *******************************************************************************/

/* RT SPR 8746 FIX START */
char *
oam_qcli_send_no_param_get_set_commands
(       
 int param_count,
 char *const *p_param_list_in,
 const char *const *E,
 const char *const *T,
 void *arg
 )   
{
	UInt8 size = oam_strlen(p_param_list_in[0]) + 1;
	Char8 parent_name[64] = "";
	oam_error_code_et error_code = NO_ERROR;
	Char8 *p_param_list_out[3] = {OAM_NULL};
	if (OAM_ZERO == strcasecmp(p_param_list_in[0], "SON ANR DISABLE"))
	{
		oam_strncpy(parent_name, "SON_ANR_DISABLE", sizeof(parent_name));
	}
	else if (OAM_ZERO == strcasecmp(p_param_list_in[0], "SON X2 ENABLE"))
	{
		oam_strncpy(parent_name, "SON_NR_CONFIG_X2_ENABLE",
				sizeof(parent_name));
	}
	else if (OAM_ZERO == strcasecmp(p_param_list_in[0], "SON X2 DISABLE"))
	{
		oam_strncpy(parent_name, "SON_NR_CONFIG_X2_DISABLE", sizeof(parent_name));
	}
	if(OAM_FAILURE == oam_mem_alloc(size, (void**)&p_param_list_out[0], &error_code)) 
	{
		oam_fprintf(stderr,"Memory allocation failed!!!");
		return NULL;
	}
	oam_strncpy(p_param_list_out[0], p_param_list_in[0], size);

	size = oam_strlen(parent_name) + 1;
	if(OAM_FAILURE == oam_mem_alloc(size, (void**)&p_param_list_out[1],
				&error_code)) 
	{
		oam_fprintf(stderr,"Memory allocation failed!!!");
		return NULL;
	}
	oam_memset(p_param_list_out[1], 0, size);

	size = oam_strlen("DUMMY_PARAM") + 1;
	if(OAM_FAILURE == oam_mem_alloc(size,
				(void**)&p_param_list_out[2], &error_code))
	{
		oam_fprintf(stderr,"Memory allocation failed!!!");
		return NULL;
	}
	oam_memset(p_param_list_out[2], 0, size);

	oam_strncpy(p_param_list_out[1], parent_name, 
			(oam_strlen(parent_name)));
	oam_strncpy(p_param_list_out[2], "DUMMY_PARAM",size);
	param_count++;
	return(oam_qcli_send_command(param_count,p_param_list_out,E,T,arg));
} 

/* RT SPR 8746 FIX END */

#ifdef OAM_SON_ENABLE
/******************************************************************************
 * FUNCTION NAME     :  oam_son_get_last_nw_scan_status
 * DESCRIPTION       :  compose certain qcli cmd which have no parameters and
 *                      need specific handling. Initially implemented for SON
 *                      Specific commands but can be used elsewhere.
 *
 * INPUTS            :  C : number of params
 *                   :  V : parameter array
 *                   :  E
 *                   :  T
 *                   :  arg
 * OUTPUTS           :  none
 * RETURNS           :  string
 *******************************************************************************/

Char8 *
oam_son_get_last_nw_scan_status
(
 SInt32 C,
 Char8 *const *V,
 const Char8 *const *E,
 const Char8 *const *T,
 void *arg
 )
{
	SInt32 size = OAM_ZERO;
	oam_error_code_et error_code = NO_ERROR;
	SInt32 temp = 0;
	Char8 *v[OAM_SON_NW_SCAN_STATUS_PARAMS + OAM_ONE];

	Char8 *vt[OAM_SON_NW_SCAN_STATUS_PARAMS ] = {
		"ScanStatus",
		"ErrorDetails",
		"LastScanTime",
		"MaxCellEntries",
		"CellNumberOfEntries"
	};

	C = (OAM_SON_NW_SCAN_STATUS_PARAMS+1);
	size = oam_strlen(V[0]) + 1;
	if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[0], &error_code)) {
		oam_fprintf(stderr, "\n-----------------------------------");
		oam_fprintf(stderr, "\nMEMORY ERROR!! Please try again....");
		oam_fprintf(stderr, "\n-----------------------------------");
		return OAM_NULL;
	}
	oam_strcpy(v[0], V[0]);

	for (temp = 1; temp < C; ++temp) {
		size = oam_strlen(vt[temp-1]) + 1;
		if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[temp], &error_code)) {
			oam_fprintf(stderr, "\n-----------------------------------");
			oam_fprintf(stderr, "\nMEMORY ERROR!! Please try again....");
			oam_fprintf(stderr, "\n-----------------------------------");
			return OAM_NULL;
		}
		oam_strcpy(v[temp], vt[temp-1]);
	}

	return oam_qcli_send_command(C, v, E, T, arg);
}
/******************************************************************************
 * FUNCTION NAME     :  oam_son_get_last_nw_scan_results_rf
 * DESCRIPTION       :  compose certain qcli cmd which have no parameters and
 *                      need specific handling. Initially implemented for SON
 *                      Specific commands but can be used elsewhere.
 *
 * INPUTS            :  C : number of params
 *                   :  V : parameter array
 *                   :  E
 *                   :  T
 *                   :  arg
 * OUTPUTS           :none 
 * RETURNS           :string
 *******************************************************************************/
Char8 *
oam_son_get_last_nw_scan_results_rf
(
 SInt32 C,
 Char8 *const *V,
 const Char8 *const *E,
 const Char8 *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	SInt32 size = OAM_ZERO;
	int count = OAM_ZERO;

	oam_error_code_et error_code = NO_ERROR;
	SInt32 temp = 0;
	Char8 *v[OAM_SON_NW_SCAN_RESULT_RF_PARAMS + 2];

	Char8 *vt[OAM_SON_NW_SCAN_RESULT_RF_PARAMS] = {
		"EUTRACarrierARFCN",
		"RSSI",
		"PhyCellID",
		"RSRP",
		"RSRQ"
	};
	count = (OAM_SON_NW_SCAN_RESULT_RF_PARAMS +1);
	size = oam_strlen(V[0]) + 1;
	if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[0], &error_code)) {
		oam_fprintf(stderr, "\n-----------------------------------");
		oam_fprintf(stderr, "\nMEMORY ERROR!! Please try again....");
		oam_fprintf(stderr, "\n-----------------------------------");
		return OAM_NULL;
	}
	oam_strcpy(v[0], V[0]);

	for (temp = 1; temp < count; ++temp) {
		size = oam_strlen(vt[temp -1]) + 1;
		if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[temp], &error_code)) {
			oam_fprintf(stderr, "\n-----------------------------------");
			oam_fprintf(stderr, "\nMEMORY ERROR!! Please try again....");
			oam_fprintf(stderr, "\n-----------------------------------");
			return OAM_NULL;
		}
		oam_strcpy(v[temp], vt[temp - 1]);
	}

	size = oam_strlen(V[1]) + 1;
	/*COVERITY FIX 54905*/
	if ((temp - OAM_ONE) < OAM_SON_NW_SCAN_RESULT_RF_PARAMS +1)
	{
		/*COVERITY FIX 54905*/
		if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[temp], &error_code)) {
			oam_fprintf(stderr, "\n-----------------------------------");
			oam_fprintf(stderr, "\nMEMORY ERROR!! Please try again....");
			oam_fprintf(stderr, "\n-----------------------------------");
			return OAM_NULL;
		}
		oam_strcpy(v[temp], V[1]);
	}
    g_oam_cli_support_multi_instance = 1;
    /* Cov CID 54905 Fix */
/*SPR 12727 START */
    return oam_qcli_send_command(OAM_SON_NW_SCAN_RESULT_RF_PARAMS + 2, v, E, T, arg);
/*SPR 12727 END */
}
/******************************************************************************
 * FUNCTION NAME     :  oam_son_get_last_nw_scan_results_bcch 
 * DESCRIPTION       :  compose certain qcli cmd which have no parameters and
 *                      need specific handling. Initially implemented for SON
 *                      Specific commands but can be used elsewhere.
 *
 * INPUTS            :  C : number of params
 *                   :  V : parameter array
 *                   :  E
 *                   :  T
 *                   :  arg
 * OUTPUTS           : none  
 * RETURNS           : string
 *******************************************************************************/


Char8 *
oam_son_get_last_nw_scan_results_bcch
(
 SInt32 C,
 Char8 *const *V,
 const Char8 *const *E,
 const Char8 *const *T,
 void *arg
 )
{
    /* SPR 17777 FIX START */
    qcliHandleUnusedParameter(C);
    /* SPR 17777 FIX END */

	SInt32 size = OAM_ZERO;
	int count = OAM_ZERO;
	oam_error_code_et error_code = NO_ERROR;
	SInt32 temp = 0;
	Char8 *v[OAM_SON_NW_SCAN_RESULT_BCCH_PARAMS +2];

	Char8 *vt[OAM_SON_NW_SCAN_RESULT_BCCH_PARAMS] = {
		"DLBandwidth",
		"ULBandwidth",
		"RSTxPower",
		"TAC",
		"CellID",
		"CellBarred",
		"CSGIndication",
		"CSGIdentity",
		"MaxPLMNListEntries",
		"PLMNListNumberOfEntries"
	};

	count = OAM_SON_NW_SCAN_RESULT_BCCH_PARAMS + 1;
	size = oam_strlen(V[0]) +  1;
	if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[0], &error_code)) {
		oam_fprintf(stderr, "\n-----------------------------------");
		oam_fprintf(stderr, "\nMEMORY ERROR!! Please try again....");
		oam_fprintf(stderr, "\n-----------------------------------");
		return OAM_NULL;
	}
	oam_strcpy(v[0], V[0]);

	for (temp = 1; temp < count; ++temp) {
		size = oam_strlen(vt[temp - 1]) + 1;
		if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[temp], &error_code)) {
			oam_fprintf(stderr, "\n-----------------------------------");
			oam_fprintf(stderr, "\nMEMORY ERROR!! Please try again....");
			oam_fprintf(stderr, "\n-----------------------------------");
			return OAM_NULL;
		}
		oam_strcpy(v[temp], vt[temp - 1]);
	}
	size = oam_strlen(V[1]) + 1;
	/*COVERITY FIX 54904*/
	if ((temp - OAM_ONE) < OAM_SON_NW_SCAN_RESULT_BCCH_PARAMS +1)
	{
		/*COVERITY FIX 54904*/
		if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[temp], &error_code)) {
			oam_fprintf(stderr, "\n-----------------------------------");
			oam_fprintf(stderr, "\nMEMORY ERROR!! Please try again....");
			oam_fprintf(stderr, "\n-----------------------------------");
			return OAM_NULL;
		}
		oam_strcpy(v[temp], V[1]);
	}
    g_oam_cli_support_multi_instance = 1;
    /*Cov CID 54904 Fix */
/*SPR 12727 START */
    return oam_qcli_send_command(OAM_SON_NW_SCAN_RESULT_BCCH_PARAMS +2, v, E, T, arg);
/*SPR 12727 END */
}
/******************************************************************************
 * FUNCTION NAME     :  oam_send_periodic_scan_stop 
 * DESCRIPTION       :  compose certain qcli cmd which have no parameters and 
 *                      need specific handling. Initially implemented for SON
 *                      Specific commands but can be used elsewhere.            
 *                    
 * INPUTS            :  C : number of params
 *                   :  V : parameter array
 *                   :  E
 *                   :  T
 *                   :  arg
 * OUTPUTS           : 
 * RETURNS           : 
 *******************************************************************************/

char *
oam_send_periodic_scan_stop
(       
 int param_count,
 char *const *p_param_list_in,
 const char *const *E,
 const char *const *T,
 void *arg
 )   
{
	UInt8 size = oam_strlen(p_param_list_in[0]) + 1;
	oam_error_code_et error_code = NO_ERROR;
	Char8 *p_param_list_out[2] = {OAM_NULL};


	if(OAM_FAILURE == oam_mem_alloc(size, (void**)&p_param_list_out[0], &error_code)) 
    {
		oam_fprintf(stderr,"Memory allocation failed!!!");
		return " ";
	}
	oam_strncpy((char*)p_param_list_out[0], (const char*)p_param_list_in[0], size-1);

	if(OAM_FAILURE == oam_mem_alloc(2, (void**)&p_param_list_out[1], &error_code)) 
	{
		oam_fprintf(stderr,"Memory allocation failed!!!");
		return " ";                     
	}
	snprintf(p_param_list_out[1], 2, "%d", 0);
	param_count++;
	return(oam_qcli_send_command(param_count,p_param_list_out,E,T,arg));
}


/****************************************************************************
 * FUNCTION NAME     :  qcli_validate_send_gap_config_command 
 * DESCRIPTION       :  This function handles the SON ANR MEAS CONFIG request 
 *                    
 * INPUTS            :  param_count : number of parameters passed by user
 *                   :  p_param_list_in : input parameters array
 *                   :  E : environment parameter
 *                   :  T
 *                   :  arg
 * OUTPUTS           : 
 * RETURNS           : 
 ***************************************************************************/
Char8* qcli_validate_send_optional_param_cmd(SInt32 param_count,
		Char8 *const *p_param_list_in,
		const Char8 *const *E,
		const Char8 *const *T,
		void *arg)
{
	UInt8 valid_params = OAM_ZERO;
	UInt8 cnt = OAM_ZERO;

	for(cnt = OAM_ONE; cnt < param_count; ++cnt) {
		if(oam_strcmp(p_param_list_in[cnt], OAM_CLI_DEFAULT_PARAM_VALUE)) {
			++valid_params;
		}
	}

	if(!valid_params) {
		oam_fprintf(stderr, "---------------------------------------------\n");
		oam_fprintf(stderr, "Atleast one parameter value must be provided!");
		oam_fprintf(stderr, "\n---------------------------------------------\n");
		return OAM_NULL;
	}

	return oam_qcli_send_command(param_count, p_param_list_in, E, T, arg);
}

/* OAM BCOM Code Comment Changes Start */
/****************************************************************************
 * FUNCTION NAME     :  qcli_handle_anr_meas_config_cmd
 * DESCRIPTION       :  This function handles the SON ANR MEAS CONFIG request
 *
 * INPUTS            :  param_count : number of parameters passed by user
 *                   :  p_param_list_in : input parameters array
 *                   :  E : environment parameter
 *                   :  T
 *                   :  arg
 * OUTPUTS           :
 * RETURNS           :
 ***************************************************************************/
Char8* qcli_handle_anr_meas_config_cmd(SInt32 param_count,
                                       Char8 *const *p_param_list_in,
                                       const Char8 *const *E,
                                       const Char8 *const *T,
                                       void *arg)
{
#define OAM_SON_ANR_MEAS_CONFIG_MAX_PARAM    25
/* GEERAN ADD OBJECT FIXED START */
/* SPR 14651 START */
#define ARFCN_SPACING_INDEX                  10
#define NUM_FOLLOWING_ARFCNS_INDEX           11
#define EXPLICIT_ARFNC_LIST_INDEX            12
#define ARFCN_BITMAP_INDEX                   13
/* SPR 14651 END */
/* GEERAN ADD OBJECT FIXED END */
    UInt8 size = OAM_ZERO;
    UInt8 loop = OAM_ZERO;
    oam_error_code_et error_code = NO_ERROR;
    UInt32 exclusive_param_cnt = 0;
    char *v[OAM_SON_ANR_MEAS_CONFIG_MAX_PARAM] = {OAM_NULL};

    if(strstr(p_param_list_in[0], "GERANFreqGroup")) {
        if(strcmp(p_param_list_in[ARFCN_SPACING_INDEX],
                  OAM_CLI_DEFAULT_PARAM_VALUE) ||
           strcmp(p_param_list_in[NUM_FOLLOWING_ARFCNS_INDEX],
                  OAM_CLI_DEFAULT_PARAM_VALUE)) {
            if(!(strcmp(p_param_list_in[ARFCN_SPACING_INDEX],
                        OAM_CLI_DEFAULT_PARAM_VALUE) &&
               strcmp(p_param_list_in[NUM_FOLLOWING_ARFCNS_INDEX],
                      OAM_CLI_DEFAULT_PARAM_VALUE))) {
                oam_fprintf(stderr, "-------------------------------------------"
                       " --------------------------------------\n");
                oam_fprintf(stderr, "Please specify values for both of the arfcn"
                       " spacing and number of following arfcns\n");
                oam_fprintf(stderr, "-------------------------------------------"
                       " --------------------------------------\n");
                return NULL;
            }
            ++exclusive_param_cnt;
        }

        if(strcmp(p_param_list_in[EXPLICIT_ARFNC_LIST_INDEX],
                  OAM_CLI_DEFAULT_PARAM_VALUE)) {
            ++exclusive_param_cnt;
        }

        if(strcmp(p_param_list_in[ARFCN_BITMAP_INDEX],
                  OAM_CLI_DEFAULT_PARAM_VALUE)) {
            ++exclusive_param_cnt;
        }

        if(exclusive_param_cnt > 1) {
            oam_fprintf(stderr, "-----------------------------------------------"
                    " --------------------------------------\n");
            oam_fprintf(stderr, "Please specify only one of explicit arfcn list,"
                    " equally spaced arfcns or arfcn bitmap.\n");
            oam_fprintf(stderr, "-----------------------------------------------"
                    " --------------------------------------\n");
            return NULL;
        }
    }


    /* Copy command in first index */
    /* coverity 109013 fix start*/
    size = oam_strlen(p_param_list_in[OAM_ZERO]) + 1;
    /* coverity 109013 fix end*/
    if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[OAM_ZERO],
                                    &error_code)) {
        oam_fprintf(stderr, "Memory allocation failed!!!");
        return OAM_NULL;
    }
    oam_strcpy(v[OAM_ZERO], p_param_list_in[OAM_ZERO]);

    /* Copy all other parameters as such */
    for(loop = OAM_TWO; loop < param_count; ++loop) {
        /*coverity 109013 fix start */
        size = oam_strlen(p_param_list_in[loop]) + 1;
        /*coverity 109013 fix end */
        if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[loop - OAM_ONE],
                                        &error_code)) {
            oam_fprintf(stderr, "Memory allocation failed!!!");
            return OAM_NULL;
        }
        oam_strcpy(v[loop - OAM_ONE], p_param_list_in[loop]);
    }

    /* coverity 109013 fix start */
    size = oam_strlen(p_param_list_in[OAM_ONE]) + 1;
    /* coverity 109013 fix end */
    /*SPR 12727 START */
    if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[param_count-1],
    /*SPR 12727 END */
                                    &error_code)) {
        oam_fprintf(stderr, "Memory allocation failed!!!");
        return OAM_NULL;
    }
    /*SPR 12727 START */
    oam_strcpy(v[param_count-1], p_param_list_in[OAM_ONE]);
    /*SPR 12727 END */

    g_oam_cli_support_multi_instance = 1;

    return oam_qcli_send_command(param_count, v, E, T, arg);
}

/* OAM BCOM Code Comment Changes End */

/****************************************************************************
 * FUNCTION NAME     :  oam_qcli_send_son_nr_cell_command 
 * DESCRIPTION       :  This function handles the SON NR UPDATE request 
 *                    
 * INPUTS            :  C : number of parameters passed by user
 *                   :  V : input parameters array
 *                   :  E : environment parameter
 *                   :  T
 *                   :  arg
 * OUTPUTS           : 
 * RETURNS           : 
 ***************************************************************************/
Char8* qcli_oam_son_nbr_cell_update_cmd_handler(int C, char *const *V,
                                                const char *const *E,
                                                const char *const *T,
                                                void *arg)
{
#define OAM_SON_NR_ADD_EUTRAN_CELL_MAX_PARAM    25
    UInt8 size = OAM_ZERO;
    UInt8 loop = OAM_ZERO;
    oam_error_code_et error_code = NO_ERROR;
    char *v[OAM_SON_NR_ADD_EUTRAN_CELL_MAX_PARAM + 1] = {OAM_NULL};

	/* RT SPR 8627 START */
	if(strstr(V[0], "UPDATE UTRAN")) {
		/* Check whether the value of CID and value of RNCID coform to the
		 * restriction.
		 * V[3]: value of CID
		 * V[4]: value of RNCID
		 */
		if(oam_atoi(V[3]) > 4095 && oam_atoi(V[4]) > 4095) {
			oam_fprintf(stderr, "-----------------------------------------------"
					"-------------------------------\n");
			oam_fprintf(stderr, "ERROR: The values of CID and RNCID put together"
					" must not excceed 28-bit in size\n");
			oam_fprintf(stderr, "-----------------------------------------------"
					"--------------------------------\n");
			return NULL;
		}
	}
	/* RT SPR 8627 END */

/* OAM BCOM Code Comment Changes Start */

    /* Copy command in first index */
    /*coverity 109016 fix start*/
    size = oam_strlen(V[0]) ;
    if(size > 0)
    {
        size +=1;
    if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[0], &error_code)) {
        oam_fprintf(stderr, "Memory allocation failed!!!");
        return OAM_NULL;
         }
          oam_strcpy(v[0], V[0]);
    }
    /*coverity 109016 fix end*/
    /* Copy all other parameters as such */
    for(loop = OAM_TWO; loop < C; ++loop) {
        /*coverity 109016 fix start*/
        size = oam_strlen(V[loop]) ;
        if(size > 0)
        {
            size += 1;   
        if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[loop - OAM_ONE],
                    &error_code)) {
            oam_fprintf(stderr, "Memory allocation failed!!!");
            return OAM_NULL;
        }
        oam_strcpy(v[loop - OAM_ONE], V[loop]);
        }
        /*coverity 109016 fix end*/
    }

    /*coverity 109016 fix start*/
    size = oam_strlen(V[1]) ;
    if(size >0 )
    {
        size +=1;

    --loop;
    if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[loop], &error_code)) {
        oam_fprintf(stderr, "Memory allocation failed!!!");
        return OAM_NULL;
    }
    oam_strcpy(v[loop], V[1]);
  }
    /*coverity 109016 fix end*/

    g_oam_cli_support_multi_instance = 1;

/* OAM BCOM Code Comment Changes End */
    return qcli_send_multi_instance_obj_command(C, V, E, T, arg);
}

#endif

/* SPR_19279_START */
/****************************************************************************
 * FUNCTION NAME     :  qcli_oam_epc_plmnlist_update_cmd_handler 
 * DESCRIPTION       :  This function handles the SON NR UPDATE request 
 *                    
 * INPUTS            :  C : number of parameters passed by user
 *                   :  V : input parameters array
 *                   :  E : environment parameter
 *                   :  T
 *                   :  arg
 * OUTPUTS           : 
 * RETURNS           : 
 ***************************************************************************/
Char8* qcli_oam_epc_plmnlist_update_cmd_handler(int C, char *const *V,
                                                const char *const *E,
                                                const char *const *T,
                                                void *arg)
{
    UInt8 size = OAM_ZERO;
    UInt8 loop = OAM_ZERO;
    oam_error_code_et error_code = NO_ERROR;
    char *v[24] = {OAM_NULL};

    /* Copy command in first index */
    size = strlen(V[0]) + 1;
    if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[0], &error_code)) {
        oam_fprintf(stderr, "Memory allocation failed!!!");
        return OAM_NULL;
    }
    oam_strcpy(v[0], V[0]);

    /* Copy all other parameters as such */
    for(loop = OAM_TWO; loop < C; ++loop) {
        size = strlen(V[loop]) + 1;
        if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[loop - OAM_ONE],
                    &error_code)) {
            oam_fprintf(stderr, "Memory allocation failed!!!");
            return OAM_NULL;
        }
        oam_strcpy(v[loop - OAM_ONE], V[loop]);
    }

    size = strlen(V[1]) + 1;
    --loop;
    if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[loop], &error_code)) {
        oam_fprintf(stderr, "Memory allocation failed!!!");
        return OAM_NULL;
    }
    oam_strcpy(v[loop], V[1]);

    g_oam_cli_support_multi_instance = 1;

    return qcli_send_multi_instance_obj_command(C, V, E, T, arg);
}
/* SPR_19279_END */

/******************************************************************************
 *   FUNCTION NAME     :  oam_send_delete_obj_req
 *   DESCRIPTION       :  send GEt / SET REQ to oam 
 *
 *   INPUTS            :  no_of_param 
 *                     :  params
 *                     :  parent
 *                     :  trans_id
 *                     :  src_id
 *                     :  dest_id
 *                     :  api_id   
 *   OUTPUTS           :
 *   RETURNS           : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/

oam_return_et
oam_send_delete_obj_req
(
 char *parent,
 UInt16 trans_id,
 UInt16 src_id,
 UInt16 dest_id,
 UInt16 api_id
 )
{
	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt8 *p_msg=NULL, *p_api_msg=NULL;
	oam_error_code_et error_code = NO_ERROR;
	/* Finding Msg length*/

	msg_length = sizeof(UInt32) +  /* Reserved bits */
		sizeof(UInt16) + /* Length */
		sizeof(UInt16) + /* Reserved */
		oam_strlen(parent);  /* object name */ 

	/*msg_api_length is complete msg including CSPL header QCLI header*/ 
	msg_api_length = msg_length  + OAM_CSPL_HDR_SIZE + OAM_MGMT_INTF_HEADER_SIZE ;

	/* Allocating memory for msg buffer*/
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);	
	if(NULL==p_msg)
	{			 
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_api_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_api_msg,OAM_VERSION,src_id,
			dest_id,
			api_id,msg_api_length);
	p_api_msg = p_api_msg + OAM_CSPL_HDR_SIZE;

	/* Fill QCLI header */
	oam_construct_qcli_header(p_api_msg,trans_id,src_id,
			dest_id,
			api_id,msg_api_length-OAM_CSPL_HDR_SIZE);
	p_api_msg = p_api_msg + OAM_MGMT_INTF_HEADER_SIZE ;

	/*  Composing payload for delete mesg*/
	oam_qcli_compose_delete_req(p_api_msg,parent); 

	message_send.message_send = 1;
	if(OAM_SET_PARAMETER_VALUES == api_id)
		message_send.apiid_expected = OAM_SET_PARAMETER_VALUES_RESP;
	else if(OAM_GET_PARAMETER_VALUES == api_id)
		message_send.apiid_expected = OAM_GET_PARAMETER_VALUES_RESP; 
	else if(OAM_DELETE_OBJECT_REQ == api_id)
		message_send.apiid_expected = OAM_DELETE_OBJECT_RESP; 

	/* send msg to OAM*/
	if(OAM_FAILURE == oam_qcli_intf_send_message(p_msg,dest_id,&error_code))
	{
		return OAM_FAILURE;
	}
	return OAM_SUCCESS;
}


/******************************************************************************
 *   FUNCTION NAME     :  oam_send_add_obj_req
 *   DESCRIPTION       :  send GEt / SET REQ to oam 
 *
 *   INPUTS            :  no_of_param 
 *                     :  params
 *                     :  parent
 *                     :  trans_id
 *                     :  src_id
 *                     :  dest_id
 *                     :  api_id   
 *   OUTPUTS           :
 *   RETURNS           : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/

oam_return_et
oam_send_add_obj_req
(
 char *parent,
 UInt16 trans_id,
 UInt16 src_id,
 UInt16 dest_id,
 UInt16 api_id
 )
{
	UInt16 msg_length = OAM_ZERO;
	UInt16 msg_api_length =  OAM_ZERO;
	UInt8 *p_msg=NULL, *p_api_msg=NULL;
	oam_error_code_et error_code = NO_ERROR;
	/* Finding Msg length*/

	msg_length = sizeof(UInt32) +  /* Reserved bits */
		sizeof(UInt16) + /* Length */
		sizeof(UInt16) + /* Reserved */
		oam_strlen(parent);  /* object name */ 

	/*msg_api_length is complete msg including CSPL header QCLI header*/ 
	msg_api_length = msg_length  + OAM_CSPL_HDR_SIZE + OAM_MGMT_INTF_HEADER_SIZE ;

	/* Allocating memory for msg buffer*/
	p_msg = qvMsgAlloc(OAM_ZERO,OAM_ZERO,OAM_ZERO,msg_api_length);	
	if(NULL==p_msg)
	{			 
		return OAM_FAILURE;
	}
	oam_memset(p_msg, OAM_ZERO, msg_api_length);
	p_api_msg = p_msg;

	/* Fill CSPL header */
	oam_construct_api_header(p_api_msg,OAM_VERSION,src_id,
			dest_id,
			api_id,msg_api_length);
	p_api_msg = p_api_msg + OAM_CSPL_HDR_SIZE;

	/* Fill QCLI header */
	oam_construct_qcli_header(p_api_msg,trans_id,src_id,
			dest_id,
			api_id,msg_api_length-OAM_CSPL_HDR_SIZE);
	p_api_msg = p_api_msg + OAM_MGMT_INTF_HEADER_SIZE ;

	/*  Composing payload for delete mesg*/
	oam_qcli_compose_delete_req(p_api_msg,parent); 

	message_send.message_send = 1;
	message_send.apiid_expected = OAM_ADD_OBJECT_RESP; 
	/* send msg to OAM*/
	if(OAM_FAILURE == oam_qcli_intf_send_message(p_msg,dest_id,&error_code))
	{
		return OAM_FAILURE;
	}
	return OAM_SUCCESS;
}




/******************************************************************************
 *   FUNCTION NAME     :   oam_qcli_compose_delete_req
 *   DESCRIPTION       :   used to compose payload for qcli command
 *
 *   INPUTS            :   p_msg (buffer that need to fill)  
 *                     :   number_of_param 
 *                     :   param (structure of valid param,value set)
 *                     :   parent
 *                     :   msg_length (length of msg )
 *                     :   api_id 
 *   OUTPUTS           :
 *   RETURNS           : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/
void oam_qcli_compose_delete_req
(
 UInt8 *p_payload,
 char * parent
 )
{
	UInt16 p_len= OAM_ZERO;

	p_len = oam_strlen(parent);

	*(p_payload + 0) = 0x00;   
	*(p_payload + 1) = 0x00;    
	*(p_payload + 2) = 0x00;  
	*(p_payload + 3) = 0x00; 
	*(p_payload + 4) = (p_len & 0xff00) >> 8;
	*(p_payload + 5) =  p_len & 0xff;
	*(p_payload + 6) = 0x00;
	*(p_payload + 7) = 0x00;

	oam_memcpy(p_payload + 8,parent, p_len); 

}




/******************************************************************************
 *   FUNCTION NAME     :   oam_qcli_compose_delete_req
 *   DESCRIPTION       :   used to compose payload for qcli command
 *
 *   INPUTS            :   p_msg (buffer that need to fill)  
 *                     :   number_of_param 
 *                     :   param (structure of valid param,value set)
 *                     :   parent
 *                     :   msg_length (length of msg )
 *                     :   api_id 
 *   OUTPUTS           :
 *   RETURNS           : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/ 
	void 
oam_send_set_req_after_add_resp(UInt8 *p_api_buf)
{
	int curr_pos = 12;

	unsigned char resp_status, err_len;
	unsigned char err_info[80];
	unsigned short err_code;
	int inst_no = 0;

	resp_status = *(p_api_buf + curr_pos);
	if(resp_status != 0) {
		oam_fprintf(stderr,"\nFailure SET response received!\n");
		err_code = decode_word_from_buf(p_api_buf + curr_pos + 1);
		oam_fprintf(stderr,"Error Code: %d\n", err_code);
		curr_pos += 7;
		err_len = *(p_api_buf + curr_pos);
		++curr_pos;
		oam_memset(err_info, 0, 80);
		if(err_len<80)
		{
			oam_memcpy(err_info, p_api_buf + curr_pos, err_len);
			err_info[err_len] = '\0';
		}
		oam_fprintf(stderr,"Error String Length: %d\n", err_len);
		oam_fprintf(stderr,"Error: %s\n", err_info);
		return;
	}
	else {
		inst_no = decode_word_from_buf(p_api_buf + curr_pos + 8);
		oam_fprintf(stderr,"Instance Number Generated: %d\n", inst_no);	
	}

	instance_no = inst_no;
	add_set_flag= 1;
	g_oam_cli_support_multi_instance = 1;
	oam_qcli_send_command(add_params.C_add , add_params.V_add , NULL, NULL, NULL);

}
/******************************************************************************
 *   FUNCTION NAME     :  decode_word_from_buf
 *   DESCRIPTION       :  decode word from buf to oam
 *
 *   INPUTS            :   p_buf
 *                     :  
 *                     :  
 *                     :  
 *   OUTPUTS           : none 
 *   RETURNS           : OAM_SUCCESS/OAM_FAILURE
 ******************************************************************************/


unsigned short decode_word_from_buf(unsigned char* p_buf)
{
	unsigned short word = 0;
	word = *p_buf;
	word = word << 8;
	++p_buf;
	word = word | *p_buf;
	return word;
}


/****************************************************************************
 * FUNCTION NAME     :  oam_qcli_prepare_add_object_cmd_buffer 
 * DESCRIPTION       :  This function prepares and initializes different
 *                      global variables to perform ADD_OBJECT command
 * INPUTS            :  C : number of parameters passed by user
 *                   :  V : input parameters array
 * OUTPUTS           :  NONE
 * RETURNS           :  OAM_SUCCESS/OAM_FAILURE
 ***************************************************************************/
oam_return_et
oam_qcli_prepare_add_object_cmd_buffer(SInt32 C, Char8 *const *V)
{
    oam_error_code_et err = NO_ERROR;
    oam_return_et retVal = OAM_FAILURE;
    SInt32 i = 0;
    SInt32 n = 0;

    oam_memset(&add_params, 0, sizeof(add_params));
    add_params.C_add  = C;

    retVal = oam_mem_alloc((C + 1) * sizeof(Char8*),
                           (void**)&add_params.V_add, &err);
    if(OAM_FAILURE == retVal) {
        oam_fprintf(stderr, "Memory allocation failed!!!");
        return retVal;
    }

    oam_memset(add_params.V_add, 0, (C + 1) * sizeof(Char8*));

    for(i = 0; i < C ; ++i) {
        n = oam_strlen(V[i]) + 1;

        retVal = oam_mem_alloc(n, (void**)&add_params.V_add[i], &err);
        if(OAM_FAILURE == retVal) {
            oam_fprintf(stderr, "Memory allocation failed!!!");
            return retVal;
        }

        oam_memset(add_params.V_add[i], 0, n);
        oam_strncpy(add_params.V_add[i], V[i], n);
    }

    add_set_flag = 1;
    g_oam_cli_support_multi_instance = 1;

    return retVal;
}

/*******************************************************************************
 * Function Name  : qcli_handle_anr_add_meas_config_cmd
 * Description    : Function to send add object req
 * Inputs         : C : Number of parameters passed to the command
 *                  V : The array of arguments passed to the command
 *                  E : This is the .environment. available to the command
 *                  T : This is the list of tags for the parameters passed
 *                  arg : This is the argument that was passed to qclOpen
 * Outputs        : NONE
 * Returns        : Char8*
 *******************************************************************************/
Char8* qcli_handle_anr_add_meas_config_cmd(int C, Char8 *const *V,
                                           const Char8 *const *E,
                                           const Char8 *const *T,
                                           void *arg)
{
    oam_return_et retVal = OAM_FAILURE;
    UInt16 exclusive_param_cnt = OAM_ZERO;

    if(strstr(V[0], "GERANFreqGroup")) {
        if(oam_strcmp(V[ARFCN_SPACING_INDEX],
                    OAM_CLI_DEFAULT_PARAM_VALUE) ||
                oam_strcmp(V[NUM_FOLLOWING_ARFCNS_INDEX],
                    OAM_CLI_DEFAULT_PARAM_VALUE)) {
            if(!(oam_strcmp(V[ARFCN_SPACING_INDEX],
                            OAM_CLI_DEFAULT_PARAM_VALUE) &&
                        oam_strcmp(V[NUM_FOLLOWING_ARFCNS_INDEX],
                            OAM_CLI_DEFAULT_PARAM_VALUE))) {
                oam_fprintf(stderr, "-------------------------------------------"
                        " --------------------------------------\n");
                oam_fprintf(stderr, "Please specify values for both of the arfcn"
                        " spacing and number of following arfcns\n");
                oam_fprintf(stderr, "-------------------------------------------"
                        " --------------------------------------\n");
				return NULL;
            }
            ++exclusive_param_cnt;
        }

        if(oam_strcmp(V[EXPLICIT_ARFNC_LIST_INDEX],
                    OAM_CLI_DEFAULT_PARAM_VALUE)) {
            ++exclusive_param_cnt;
        }

        if(oam_strcmp(V[ARFCN_BITMAP_INDEX],
                    OAM_CLI_DEFAULT_PARAM_VALUE)) {
            ++exclusive_param_cnt;
        }

        if(exclusive_param_cnt > 1) {
            oam_fprintf(stderr, "-----------------------------------------------"
                    " --------------------------------------\n");
            oam_fprintf(stderr, "Please specify only one of explicit arfcn list,"
                    " equally spaced arfcns or arfcn bitmap.\n");
            oam_fprintf(stderr, "-----------------------------------------------"
                    " --------------------------------------\n");
			return NULL;
        }
    }

    retVal = oam_qcli_prepare_add_object_cmd_buffer(C, V);

    if (OAM_FAILURE == retVal) {
        return OAM_NULL;
    }

	return oam_qcli_send_command(C, V, E, T, arg);
}

/****************************************************************************
 * FUNCTION NAME     :  qcli_prepare_multi_instance_obj_req_buffer 
 * DESCRIPTION       :  This function handles prepares the request buffers
                        for multi-instance-objects parameters configuration
                        command
 *                    
 * INPUTS            :  C : number of parameters passed by user
 *                   :  V : input parameters array
 * OUTPUTS           : NONE
 * RETURNS           : request buffer
***************************************************************************/
Char8**
qcli_prepare_multi_instance_obj_req_buffer(SInt32 C, Char8 *const *V)
{
	UInt8 size = OAM_ZERO;
	UInt8 loop = OAM_ZERO;
	oam_error_code_et err = NO_ERROR;
	Char8 **v = OAM_NULL;


    if(OAM_FAILURE == oam_mem_alloc(C * sizeof(Char8*), (void**)&v, &err)) {
		oam_fprintf(stderr, "Memory allocation failed!!!");
		return OAM_NULL;
	}

	/* Copy command in first index */
	size = oam_strlen(V[0]) + 1;
	if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[0], &error_code)) {
        /* Coverity CID 72336 Fix Start */
        oam_mem_free((void *)v,&error_code);
        /* Coverity CID 72336 Fix End */
		oam_fprintf(stderr, "Memory allocation failed!!!");
		return OAM_NULL;
	}
	oam_strcpy(v[0], V[0]);

	/* Copy all other parameters as such */ 
	for(loop = OAM_TWO; loop < C; ++loop) {
		size = oam_strlen(V[loop]) + 1;
		if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[loop - OAM_ONE],
					&error_code)) {
			oam_fprintf(stderr, "Memory allocation failed!!!");
           /* Coverity ID 66657 Fix Start*/
            OAM_PM_2D_TEMP_ARR_MEM_FREE(v,loop - OAM_ONE,error_code);
            /* Coverity ID 66657 Fix End*/
			return OAM_NULL;
		}
		oam_strcpy(v[loop - OAM_ONE], V[loop]);
	}

	size = oam_strlen(V[1]) + 1;
	--loop;
	if(OAM_FAILURE == oam_mem_alloc(size, (void**)&v[loop], &error_code)) {

		oam_fprintf(stderr, "Memory allocation failed!!!");
        /* Coverity ID 66657 Fix Start*/
        OAM_PM_2D_TEMP_ARR_MEM_FREE(v,loop ,error_code);
        /* Coverity ID 66657 Fix End*/
		return OAM_NULL;
	}

	oam_strcpy(v[loop], V[1]);

	g_oam_cli_support_multi_instance = 1;

    return v;
}


/****************************************************************************
 * FUNCTION NAME     :  qcli_send_multi_instance_obj_command 
 * DESCRIPTION       :  This function handles prepares the request buffers
                        for multi-instance-objects parameters configuration
                        command and send the request to OAM
 *                    
 * INPUTS            :  C : number of parameters passed by user
 *                   :  V : input parameters array
 * OUTPUTS           : NONE
 * RETURNS           : request buffer
***************************************************************************/
Char8*
qcli_send_multi_instance_obj_command(int C, char *const *V,
                                    const char *const *E,
                                    const char *const *T,
                                    void *arg)
{
	UInt8 loop = OAM_ZERO;
	Char8 **v = OAM_NULL;
    Char8 *retVal = OAM_NULL;
	oam_error_code_et err = NO_ERROR;

    v = qcli_prepare_multi_instance_obj_req_buffer(C, V);

    if (!v) {
        return retVal;
    }

	retVal = oam_qcli_send_command(--C, v, E, T, arg);

    for (loop = OAM_ZERO; loop < C; ++loop) {
        oam_mem_free(v[loop], &err);
    }

    oam_mem_free(v, &err);

    return retVal;
}
