/*****************************************************************************
 * File Name   : oam_utils.c
 *
 * Description : This file contains common functions used by OAM modules
 *
 * Revision History:
 *
 * Date               Author           Reference           Comments
 * -----             ------           ---------           --------
 * April,2012       Nitin Agarwal   OAM Design Document    Initial Version
 * September 2012   Vivek K Gupta   OAM 1.0 Rel            Changes in "OAM_SET_PARAMS_RESP"
 *                                                         due to api header redesign.
 * November 2012    Sunita		    OAM 1.0	               oam_fopen() function added
 * July, 2014       Avinash		SPR 11315 fix
 * May 2014         Shalu Sagar         SPR 11324 FIX
 * July,2014     Aditi Singh           SPR 9620 Fix         Guard timer Added
 * July,2014     Aditi Singh                                Function header corrected
 * Nov 2014      Karuna Bhardwaj    eICIC Enhancement
 * Copyright (c) 2012, Aricent Inc. All Rights Reserved
 ****************************************************************************/

#include <oam_defines.h>
#include <oam_types.h>
#include <oam_proto.h>
#include <logging.h>

UInt8 time_buffer[TIME_SIZE];

extern oam_prov_req_t oam_prov_req;
/*SPR 21889 +- */
/* SPR 11175 FIX START */
/* CODE DELETED */
/* SPR 11175 FIX END */

/************************************************************************************
 * ARRAY NAME   :  oam_loglevel_table
 * DESCRIPTION  :  Array to store reset reason for OAM
 *   
 ************************************************************************************/
oam_reset_reason_msg_t oam_reset_reason_msg[] =
{
	{XML_VALIDATION_FAILURE, "Configuration file XML validation failed"},
	{NMM_NETWORK_SCAN_FAILURE, "Failed to parse NMM network scan results file (networkScanDataModel.xml)"},
	{PARAMETER_POPULATION_FAILURE, "Parameter population failure, unable to open configuration file"},
	{OAM_STARTUP_FAILURE, "OAM Startup failure"},
	{OPERATOR_INITIATED_SHUTDOWN, "Operator initiated shutdown"},
	{OAM_INTERNAL_FAILURE, "OAM Internal error"},
	{OAM_HEALTH_CHECK_FAILURE, "OAM Health Check Failure Error"}
};

/************************************************************************************
 * ARRAY NAME   :  oam_loglevel_table
 * DESCRIPTION  :  Array of structures, to store value of loglevel string and their
 *                  corresponding value
 ************************************************************************************/

oam_loglevel_id_t
oam_loglevel_table[] =
{
	{(const UInt8 *)"logfatal",      0x01},
	{(const UInt8 *)"logerror",      0x02},
	{(const UInt8 *)"logwarning",    0x04},
	{(const UInt8 *)"loginfo",       0x08},
	{(const UInt8 *)"logdebug",      0x10},
	{OAM_NULL,           0},
};

/************************************************************************************
 * ARRAY NAME   :   oam_logcategory_table
 * DESCRIPTION  :   Array of structures, to store value of logcategory string
 *
 * FIELDS       :   oam_func_id_t - structure wich consists of identifier of callback
 *                  function and pointer to callback function
 ************************************************************************************/
oam_logcategory_id_t
oam_logcategory_table[] =
{
	{(const UInt8 *)"l2_sys_fail",  L2_SYS_FAIL},
	{(const UInt8 *)"mac_l1_inf",   MAC_L1_INF},
	{(const UInt8 *)"mac_ul_harq",  MAC_UL_HARQ},
	{(const UInt8 *)"mac_rach",     MAC_RACH},
	{(const UInt8 *)"mac_rrc_inf",  MAC_RRC_INF},
	{(const UInt8 *)"mac_oam_inf",  MAC_OAM_INF},
	/*CSR:00055317_FIX_START*/
	{(const UInt8 *)"mac_rrm_inf",  MAC_RRM_INF},
	/*CSR:00055317_FIX_END*/
	/* SPR 55317 START */
	{(const UInt8 *)"mac_dl_strategy",  MAC_DL_Strategy},
	{(const UInt8 *)"mac_ul_strategy",  MAC_UL_Strategy},
	{(const UInt8 *)"mac_pdsch",    MAC_PDSCH},
	{(const UInt8 *)"mac_pdcch",    MAC_PDCCH},
	{(const UInt8 *)"mac_pusch",    MAC_PUSCH},
	{(const UInt8 *)"mac_pucch",    MAC_PUCCH},
	{(const UInt8 *)"mac_bcch",     MAC_BCCH},
	{(const UInt8 *)"mac_pcch",     MAC_PCCH},
	{(const UInt8 *)"mac_ccch",     MAC_CCCH},
	{(const UInt8 *)"mac_dl_harq",  MAC_DL_HARQ},
	{(const UInt8 *)"mac_cqi",      MAC_CQI},
	{(const UInt8 *)"mac_srs",      MAC_SRS },
	{(const UInt8 *)"mac_tb_ul",    MAC_TB_UL },
	{(const UInt8 *)"mac_tb_dl",    MAC_TB_DL },
	{(const UInt8 *)"mac_dl_pc",    MAC_DL_PC },
	{(const UInt8 *)"mac_ul_pc",    MAC_UL_PC },
	{(const UInt8 *)"mac_drx",      MAC_DRX },
	{(const UInt8 *)"mac_meas_gap", MAC_MEAS_GAP},
	{(const UInt8 *)"mac_meas_handler", MAC_MEAS_GAP},
	{(const UInt8 *)"mac_ul_csi",   MAC_UL_CSI  },
	{(const UInt8 *)"mac_mux",      MAC_MUX },
	{(const UInt8 *)"mac_dl_sps_strategy", MAC_DL_SPS_STRATEGY },
	{(const UInt8 *)"mac_ul_sps_strategy", MAC_UL_SPS_STRATEGY },
	{(const UInt8 *)"mac_scheduler",       MAC_SCHEDULER },
	{(const UInt8 *)"mac_atb",      MAC_ATB },
	{(const UInt8 *)"mac_ta",       MAC_TA },
	/* SPR 55317 END */
	{(const UInt8 *)"rlc_am",       RLC_AM},
	{(const UInt8 *)"rlc_um",       RLC_UM},
	{(const UInt8 *)"rlc_tm",       RLC_TM},
	{(const UInt8 *)"rlc_tx",       RLC_TX},
	{(const UInt8 *)"rlc_rx",       RLC_RX},
	{(const UInt8 *)"rlc_mac",      RLC_MAC},
	{(const UInt8 *)"rlc_pdcp",     RLC_PDCP},
	{(const UInt8 *)"rlc_rrc",      RLC_RRC},
	{(const UInt8 *)"rlc_oam",      RLC_OAM},
	/* SPR 55317 */
	{(const UInt8 *)"rlc_timer",    RLC_TIMER},
	{(const UInt8 *)"pdcp_rrcoam",  PDCP_RRCOAM},
	{(const UInt8 *)"pdcp_tx",      PDCP_TX},
	{(const UInt8 *)"pdcp_rx",      PDCP_RX},
	{(const UInt8 *)"pdcp_rrm_int", PDCP_RRM_INT},
	{(const UInt8 *)"pr_gtpu_control",   PR_GTPU_CONTROL},
	{(const UInt8 *)"pr_gtpu_oam",       PR_GTPU_OAM},
	{(const UInt8 *)"pr_gtpu_dl_data",   PR_GTPU_DL_DATA},
	{(const UInt8 *)"pr_gtpu_ul_data",   PR_GTPU_UL_DATA},
	{(const UInt8 *)"pr_gtpu_common",    PR_GTPU_COMMON},
	{(const UInt8 *)"pr_gtpu_ho_data",   PR_GTPU_HO_DATA},
	{OAM_NULL,            0},
};
/****************************************************************************
 * Function Name  : oam_reset_reason_code
 * Inputs         : reset_reason_code_obj
 * Outputs        : none
 * Returns        : void
 * Description    : This function logs the reset reason for OAM
 ****************************************************************************/

void oam_reset_reason_code(oam_reset_reason_code_et reset_reason_code_obj)
{
	int loop_count = OAM_ZERO;

	for(loop_count = OAM_ZERO; loop_count < RESET_REASON_COUNT; loop_count++)
	{
		if(oam_reset_reason_msg[loop_count].reset_reason_obj == reset_reason_code_obj)
		{
			OAM_LOG(OAM, OAM_WARNING, oam_reset_reason_msg[loop_count].reason_code_msg);
		}    
	}
}

/****************************************************************************
 * Function Name  : oam_get_fresh_trans_id
 * Inputs         : trans_id
 * Outputs        : none
 * Returns        : UInt16 value
 * Description    : This function returns new transaction id
 ****************************************************************************/

/* coverity warning 54871 changes start */
UInt16
oam_get_fresh_trans_id
(
 UInt16 trans_id
 )
{
	if ((trans_id + OAM_ONE) >= OAM_MAX_TRANS_ID)
	{
		/* trans-id has reached its limit, so restart the counter */
		trans_id = OAM_ZERO;
	}
	return ++trans_id;
}
/* coverity warning 54871 changes end */

/****************************************************************************
 * Function Name  : get_word_from_header
 * Inputs         : p_header
 * Outputs        : none
 * Returns        : UInt16 value
 * Description    : This function finds the value of 2 bytes
 ****************************************************************************/
UInt16 get_word_from_header
(
 UInt8 *p_header
 )
{
	UInt16 value = OAM_ZERO;

	value = p_header[OAM_ZERO];
	value = value << OAM_EIGHT;
	value = value | p_header[OAM_ONE];

	return value;
}
/****************************************************************************
 * Function Name  : oam_construct_api_header
 * Inputs         : CSPL header, API version Id, Source module Id, Destination module Id,
 *                  API Id, API buffer size  
 * Outputs        : none
 * Returns        : void
 * Description    : constructs CSPL api header
 ****************************************************************************/
void oam_construct_api_header
(
 UInt8          *p_header,      /* CSPL header */
 UInt8          version_id,     /* API version Id */
 UInt16            src_module_id,  /* Source module Id */
 UInt16            dst_module_id,  /* Destination module Id */
 UInt16         api_id,         /* API Id */
 UInt16         api_buf_size    /* API buffer size */
 )
{


	/* version */
	p_header[0] = version_id;

	/* from (HI) */
	p_header[1] = (src_module_id & 0xFF00) >> 8;

	/* from (LOW) */
	p_header[2] = src_module_id & 0x00FF;

	/* to (HI) */
	p_header[3] = (dst_module_id & 0xFF00) >> 8;

	/* to (LOW) */
	p_header[4] = dst_module_id & 0x00FF;

	/* api id (HI) */
	p_header[5] = (api_id & 0xFF00) >> 8;

	/* api id (LOW) */
	p_header[6] = api_id & 0x00FF;

	/* api size (HI) */
	p_header[7] = (api_buf_size & 0xFF00) >> 8;

	/* api size (LOW) */
	p_header[8] = api_buf_size & 0x00FF;

	/* spare bytes */
	p_header[9]     = 0x00;
	p_header[10]    = 0x00;
	p_header[11]    = 0x00;
	p_header[12]    = 0x00;
	p_header[13]    = 0x00;
	p_header[14]    = 0x00;
	p_header[15]    = 0x00;

}

/****************************************************************************
 * Function Name  : oam_qcli_intf_send_message
 * Inputs         : p_msg, dst_module, error code
 * Outputs        : none
 * Returns        : OAM_FAILURE/OAM_SUCCESS 
 * Description    : This is used for sending message to external modules
 ****************************************************************************/
oam_return_et oam_qcli_intf_send_message
(
 void            *p_msg,     /* Message that will be passed */
 rrc_module_id_t dst_module,  /* Module id for which message will be passed */
 oam_error_code_et *p_error_code
 )
{
	/* CSR 00058135 Changes Start */
	UInt16 src_id =get_word_from_header(p_msg + OAM_TWO);
	UInt16 dst_id =get_word_from_header(p_msg + OAM_FOUR);

	if((OAM_ZERO==src_id)||(OAM_ZERO==dst_id))
	{
		OAM_LOG(OAM,OAM_ERROR,"Either Source Id or Destination Id received is ZERO, src_id=%d and dst_id=%d ",src_id,dst_id);
		/*SPR 13857 START*/
		oam_mem_free(p_msg, p_error_code);
        /*  coverity 94916 fix start */
        p_msg = OAM_NULL;
        /*  coverity 94916 fix end */
		/*SPR 13857 END*/

		return OAM_FAILURE;
	}
	/* CSR 00058135 Changes End */      

	if(QCLI_MODULE_ID == dst_module)
	{    
		OAM_LOG(OAM,OAM_INFO,"Destination Module is QCLI_MODULE_ID");
	}
	else if(TR069_MODULE_ID == dst_module)
	{
		OAM_LOG(OAM,OAM_INFO,"Destination Module is TR069_MODULE_ID");
    }
#ifndef OAM_UT_FRAMEWORK
	QMODULE service = qvGetService(dst_module);
	qvSendEx(service, OAM_ZERO, p_msg,(int *)p_error_code);
#endif
	return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_rrm_send_message
 * Inputs         : pointer p_msg, dst_module
 * Outputs        : none
 * Returns        : SUCCESS/FAILURE
 * Description    : This function is used for sending message to rrm module
 ****************************************************************************/
/*SPR 20172 Fix START*/
oam_return_et oam_rrm_send_message_ex
/*SPR 20172 Fix End*/
(
 void            *p_msg,     /* Message that will be passed */
 rrc_module_id_t dst_module  /* Module id for which message will be passed */
 )
{
	UInt16 src_id =get_word_from_header(p_msg + OAM_ONE);
	UInt16 dst_id =get_word_from_header(p_msg + OAM_THREE);
	UInt16 api_id =get_word_from_header(p_msg + OAM_FIVE);
	UInt16 length =get_word_from_header(p_msg + OAM_SEVEN);

    if((QCLI_MODULE_ID != src_id) && (QCLI_INTERFACE_MODULE_ID != src_id)) 
    {
        OAM_LOG(OAM,OAM_INFO,"Sending API[%u] of length[%u] from Module[%u] to Module[%u]", api_id, length, src_id, dst_id);
    }
#ifndef OAM_UT_FRAMEWORK
	QMODULE service = qvGetService(dst_module);
	qvSend(service, OAM_ZERO, p_msg);
#endif
	return OAM_SUCCESS;
}


/****************************************************************************
 * Function Name  : oam_msg_mem_get
 * Inputs         : size
 * Outputs        : none
 * Returns        : Pointer to memory buffer or NULL in case of failure
 * Description    : This function returns memory buffer from memory pool.
 ****************************************************************************/
void* oam_msg_mem_get(size_t size)/* Size of buffer which will be allocated */
{
	void *p_buf = PNULL;

#ifdef MEM_CHECK_DOUBLE_FREE
	p_buf = (void *)malloc(size);  //temp mask
#elif OAM_UT_FRAMEWORK
	p_buf = (void *)oam_malloc(size);  //temp mask
#else
	p_buf = qvMsgAlloc(OAM_NULL, OAM_ZERO, OAM_ZERO, size);  //temp mask
#endif
	return(p_buf);
}

/****************************************************************************
 * Function Name  : oam_cp_pack_SInt32
 * Inputs         : p_dest,p_src
 * Outputs        : none
 * Returns        : SUCCESS/FAILURE
 * Description    : This function packs SInt32 variable
 ****************************************************************************/

oam_return_et
oam_cp_pack_SInt32
(
 void    *p_dest,
 void    *p_src
 ,   char    *varname
 )
{
	/* Packing SInt32 */

	((SInt8*)p_dest)[0]    = (SInt8)((*(SInt32 *)p_src) >> 24);
	((SInt8*)p_dest)[1]    = (SInt8)((*(SInt32 *)p_src) >> 16);
	((SInt8*)p_dest)[2]    = (SInt8)((*(SInt32 *)p_src) >> 8);
	((SInt8*)p_dest)[3]    = (SInt8)((*(SInt32 *)p_src));
	OAM_LOG(OAM,OAM_DETAILEDALL, "%s = %d", varname, *((SInt32 *)p_src));
	return(OAM_SUCCESS);
}

/****************************************************************************
 * Function Name  : oam_cp_pack_SInt8
 * Inputs         : p_dest,p_src
 * Outputs        : none
 * Returns        : SUCCESS/FAILURE
 * Description    : This function packs SInt8 variable
 ****************************************************************************/


oam_return_et
oam_cp_pack_SInt8
(
 void    *p_dest,
 void    *p_src
 ,   char    *varname
 )
{


	OAM_LOG(OAM,OAM_DETAILEDALL, "%s = %d", varname, *((SInt8 *)p_src));
	oam_memcpy(p_dest, p_src, sizeof(SInt8));

	return(OAM_SUCCESS);
}

/****************************************************************************
 * Function Name  : oam_mem_alloc
 * Inputs         : size
 * Outputs        : p_msg/p_error_code
 * Returns        : Pointer to memory buffer or NULL in case of failure
 * Description    : This function returns memory buffer from memory pool.
 ****************************************************************************/

oam_return_et
oam_mem_alloc
(
 size_t size,  /* Size of buffer which will be allocated */
 void **p_msg, //Nitin ---check if address is required ??
 oam_error_code_et *p_error_code
 )
{
	void *p_buf = OAM_NULL;
#ifdef MEM_CHECK_DOUBLE_FREE
	p_buf = (void *)oam_malloc(size);  //temp mask
#elif OAM_UT_FRAMEWORK
	p_buf = (void *)oam_malloc(size);  //temp mask
#else
	p_buf = qvMsgAlloc(OAM_NULL, OAM_ZERO, OAM_ZERO, size);  //temp mask
#endif

	if(p_buf == OAM_NULL)
	{
		*p_error_code = OAM_MALLOC_FAIL;

		return OAM_FAILURE;
	}
	*p_msg = p_buf;
	return OAM_SUCCESS ;
}

/****************************************************************************
 * Function Name  : oam_cp_pack_UInt16
 * Inputs         : p_dest,p_src
 * Outputs        : none
 * Returns        : SUCCESS/FAILURE
 * Description    : This function packs UInt16 variable
 ****************************************************************************/

oam_return_et
oam_cp_pack_UInt16
(
 void    *p_dest,
 void    *p_src
 ,   char     *varname
 )
{
	/* Packing UInt16 */

	((UInt8*)p_dest)[0]    = (UInt8)((*(UInt16 *)p_src) >> 8);
	((UInt8*)p_dest)[1]    = (UInt8)((*(UInt16 *)p_src));
	OAM_LOG(OAM,OAM_DETAILEDALL, "%s = %d", varname, *((UInt16 *)p_src));
	return(OAM_SUCCESS);
}

/****************************************************************************
 * Function Name  : oam_cp_pack_UInt8
 * Inputs         : p_dest,p_src
 * Outputs        : none
 * Returns        : SUCCESS/FAILURE
 * Description    : This function packs UInt8 variable
 ****************************************************************************/

oam_return_et
oam_cp_pack_UInt8
(
 void    *p_dest,
 void    *p_src
 ,   char    *varname
 )
{
	OAM_LOG(OAM,OAM_DETAILEDALL, "%s = %d", varname, *((UInt8 *)p_src));
	oam_memcpy(p_dest, p_src, sizeof(UInt8));

	return(OAM_SUCCESS);
}

/* SPR 21889 start */
/****************************************************************************
 * Function Name  : oam_cp_pack_Char8
 * Inputs         : p_dest,p_src
 * Outputs        : none
 * Returns        : SUCCESS/FAILURE
 * Description    : This function packs Char8 variable
 ****************************************************************************/

oam_return_et
oam_cp_pack_Char8
(
 void    *p_dest,
 void    *p_src
 ,   char    *varname
 )
{
	OAM_LOG(OAM,OAM_DETAILEDALL, "%s = %d", varname, *((Char8 *)p_src));
	oam_memcpy(p_dest, p_src, sizeof(Char8));

	return(OAM_SUCCESS);
}
/* SPR 21889 end */

/****************************************************************************
 * Function Name  : oam_construct_mgmt_api_hdr
 * Inputs         : p_msg, trans_id, src_mod_id, dst_mod_id, api_id,
 *                   api_buf_size, api_version, resvd
 * Outputs        : none
 * Returns        : void 
 * Description    : This function Constructs api header
 ****************************************************************************/

void oam_construct_mgmt_api_hdr 
(
 UInt8*          p_msg,
 UInt16          trans_id,
 oam_module_id_t src_mod_id,
 oam_module_id_t dst_mod_id,
 UInt16          api_id,
 UInt16          api_buf_size,
    /*SPR 17777 fix*/
 UInt8           resvd
 )
{


	if(OAM_NULL == p_msg) {
		OAM_LOG(OAM, OAM_WARNING,
				"oam_construct_mgmt_api_hdr: p_msg is NULL");
		return;
	}

	*(p_msg + 0) = (trans_id & 0xff00) >> 8;
	*(p_msg + 1) =  trans_id & 0xff;

	*(p_msg + 2) = (src_mod_id & 0xff00) >> 8;
	*(p_msg + 3) =  src_mod_id & 0xff;

	*(p_msg + 4) = (dst_mod_id & 0xff00) >> 8;
	*(p_msg + 5) =  dst_mod_id & 0xff;

	*(p_msg + 6) = (api_id & 0xff00) >> 8;
	*(p_msg + 7) =  api_id & 0xff;

	*(p_msg + 8) = (api_buf_size & 0xff00) >> 8;
	*(p_msg + 9) =  api_buf_size & 0xff;

	/* Hardcoding API Version as 1 */
	*(p_msg + 10) =  OAM_API_VERSION;//api_version;
	*(p_msg + 11) =  resvd;
}

/****************************************************************************
 * Function Name  : oam_construct_interface_api_header
 * Inputs         : p_header, transaction_id, src_module_id,
 dst_module_id, api_id, api_buf_size,cell_index
 * Outputs        : none
 * Returns        : void
 * Description    : This function constructs interface api header
 ****************************************************************************/

void
oam_construct_interface_api_header
(
 UInt8                  *p_header,      /* RRC interface header */
 UInt16                 transaction_id, /* Interface transaction identifier */
 oam_module_id_t     src_module_id,  /* Source module identifier */
 oam_module_id_t     dst_module_id,  /* Destination module identifier */
 UInt16                 api_id,         /* API identifier */
 UInt16                 api_buf_size,   /* API buffer size */
 /* Composer_parser changes +*/
 UInt8                  cell_index      /*Cell Index */
 /* Composer_parser changes -*/
 )
{
	UInt8 offset = OAM_ZERO;
	/* transaction identifier (HI) */
	/* First Byte */
	p_header[offset++] = (UInt8)((transaction_id & 0xFF00) >> 8);

	/* transaction identifier (LOW) */
	/* Second Byte */
	p_header[offset++] = (UInt8)(transaction_id & 0x00FF);

	/* from (HI) */
	/* Third Byte */
	p_header[offset++] = (UInt8)((src_module_id & 0xFF00) >> 8);

	/* from (LOW) */
	/* 4th Byte */
	p_header[offset++] = (UInt8)(src_module_id & 0x00FF);

	/* to (HI) */
	/* 5th Byte */
	p_header[offset++] = (UInt8)((dst_module_id & 0xFF00) >> 8);

	/* to (LOW) */
	/* 6th Byte */
	p_header[offset++] = (UInt8)(dst_module_id & 0x00FF);

	/* api id (HI) */
	/* 7th Byte */
	p_header[offset++] = (UInt8)((api_id & 0xFF00) >> 8);

	/* api id (LOW) */
	/* 8th Byte */
	p_header[offset++] = (UInt8)(api_id & 0x00FF);

	/*size includes length of header*/
	api_buf_size = (UInt16)(api_buf_size + LOWER_LAYER_INTERFACE_HEADER_SIZE);

	/* api size (HI) */
	/* 9th Byte */
	p_header[offset++] = (UInt8)((api_buf_size & 0xFF00) >> 8);

	/* api size (LOW) */
	/* 10th Byte */
	p_header[offset++] = (UInt8)(api_buf_size & 0x00FF);

	/*BUG TCID:TC_MC_PM_12*/
	/* Composer_parser changes +*/
	/* 11th Byte */
	p_header[offset++] = cell_index;/*oam_prov_req.cell_cntxt.curr_cell_index*/;
	/* Composer_parser changes -*/
	/* 12th Byte */
	p_header[offset++] = 0x00;
	/* 13th Byte */
	p_header[offset++] = 0x00;
	/* 14th Byte */
	p_header[offset++] = 0x00;
	/* 15th Byte */
	p_header[offset++] = 0x00;
	/* 16th Byte */
	p_header[offset++] = 0x00;
}

/****************************************************************************
 * Function Name  : oam_cp_pack_SInt16
 * Inputs         : p_dest,p_src
 * Outputs        : none
 * Returns        : SUCCESS/FAILURE
 * Description    : This function packs SInt16 variable
 ****************************************************************************/

oam_return_et
oam_cp_pack_SInt16
(
 void    *p_dest,
 void    *p_src
 ,   char    *varname
 )
{
	/* Packing SInt16 */

	((SInt8*)p_dest)[0]    = (SInt8)((*(SInt16 *)p_src) >> 8);
	((SInt8*)p_dest)[1]    = (SInt8)((*(SInt16 *)p_src));
	OAM_LOG(OAM,OAM_DETAILEDALL, "%s = %d", varname, *((SInt16 *)p_src));
	return(OAM_SUCCESS);
}
/****************************************************************************
 * Function Name  : oam_cp_unpack_SInt8
 * Inputs         : p_dest,p_src
 * Outputs        : none
 * Returns        : SUCCESS/FAILURE
 * Description    : This function unpacks SInt8 variable
 ****************************************************************************/

oam_return_et
oam_cp_unpack_SInt8
(
 void    *p_dest,
 void    *p_src
 , char    *varname
 )
{

	/* Unpacking SInt8 */

	*(SInt8 *)p_dest  = *(SInt8 *)p_src;
	OAM_LOG(OAM,OAM_DETAILEDALL, "%s = %d", varname, *((SInt8 *)p_dest));
	return(OAM_SUCCESS);
}

/****************************************************************************
 * Function Name  : oam_cp_unpack_SInt32
 * Inputs         : p_dest,p_src
 * Outputs        : none
 * Returns        : SUCCESS/FAILURE
 * Description    : This function unpacks SInt32 variable
 ****************************************************************************/

oam_return_et
oam_cp_unpack_SInt32
(
 void    *p_dest,
 void    *p_src
 ,char    *varname
 )
{

	/* Unpacking SInt32 */

	*(SInt32 *)p_dest  = 0;

	*(SInt32 *)p_dest  |= (SInt32)((UInt8*)p_src)[0] << 24;
	*(SInt32 *)p_dest  |= (SInt32)((UInt8*)p_src)[1] << 16;
	*(SInt32 *)p_dest  |= (SInt32)((UInt8*)p_src)[2] << 8;
	*(SInt32 *)p_dest  |= (SInt32)((UInt8*)p_src)[3];
	OAM_LOG(OAM,OAM_DETAILEDALL, "%s = %d", varname, *((SInt32 *)p_dest));
	return(OAM_SUCCESS);
}

/****************************************************************************
 * Function Name  : oam_cp_unpack_UInt8
 * Inputs         : p_dest,p_src
 * Outputs        : none
 * Returns        : SUCCESS/FAILURE
 * Description    : This function unpacks UInt8 variable
 ****************************************************************************/

oam_return_et
oam_cp_unpack_UInt8
(
 void    *p_dest,
 void    *p_src

 ,   char    *varname
 )
{
	oam_memcpy(p_dest, p_src, sizeof(UInt8));

	OAM_LOG(OAM,OAM_DETAILEDALL, "%s = %d", varname, *((UInt8 *)p_dest));
	return(OAM_SUCCESS);
}

/* SPR 21889 start */
/****************************************************************************
 * Function Name  : oam_cp_unpack_Char8
 * Inputs         : p_dest,p_src
 * Outputs        : none
 * Returns        : SUCCESS/FAILURE
 * Description    : This function unpacks Char8 variable
 ****************************************************************************/

oam_return_et
oam_cp_unpack_Char8
(
 void    *p_dest,
 void    *p_src
 ,   char    *varname
 )
{
	oam_memcpy(p_dest, p_src, sizeof(Char8));

	OAM_LOG(OAM,OAM_DETAILEDALL, "%s = %d", varname, *((Char8 *)p_dest));
	return(OAM_SUCCESS);
}
/* SPR 21889 end */

/****************************************************************************
 * Function Name  : oam_cp_unpack_UInt16
 * Inputs         : p_dest,p_src
 * Outputs        : none
 * Returns        : SUCCESS/FAILURE
 * Description    : This function unpacks UInt16 variable
 ****************************************************************************/

oam_return_et
oam_cp_unpack_UInt16
(
 void    *p_dest,
 void    *p_src

 ,  char    *varname
 )
{
	/* Unpacking UInt16 */

	*(UInt16 *)p_dest  = OAM_ZERO;

	*(UInt16 *)p_dest = (UInt16)(*(UInt16 *)p_dest  | (UInt16)(((UInt8*)p_src)[0] << 8));
	*(UInt16 *)p_dest = (UInt16)(*(UInt16 *)p_dest  | (UInt16)((UInt8*)p_src)[1]);

	OAM_LOG(OAM,OAM_DETAILEDALL, "%s = %d", varname, *((UInt16 *)p_dest));
	return(OAM_SUCCESS);
}
/****************************************************************************
 * Function Name  : oam_mem_free
 * Inputs         : p_buffer,p_error_code
 * Outputs        : none
 * Returns        : SUCCESS/FAILURE
 * Description    : This function frees the memory pointer 
 ****************************************************************************/

oam_return_et
oam_mem_free
(
 void *p_buffer, /* Pointer to buffer which will be freed */
 oam_error_code_et *p_error_code
 )
{
	if (p_buffer != OAM_NULL)
	{
#ifdef MEM_CHECK_DOUBLE_FREE
		oam_free(p_buffer);
#elif OAM_UT_FRAMEWORK
		oam_free(p_buffer);
#else
		qvMsgFree(p_buffer);
#endif
	}
	else
	{
		OAM_LOG(OAM,OAM_WARNING, "p_buffer is NULL (unused "
                                 "parameter, ignore = %p)", p_error_code); /* SPR1777 */
		return OAM_FAILURE;
	}

	return OAM_SUCCESS ;
}

/****************************************************************************
 * Function Name  : create_time_stamp
 * Inputs         : none
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function Handles the time stamp
 ****************************************************************************/
 /* + SPR 17439 */
oam_return_et create_time_stamp(void){
 /* - SPR 17439 */
	const struct tm *tm_ptr = OAM_NULL;                           /*COVERITY_FIX_1.0*/
	size_t len;
	time_t now;
	now = oam_time ( OAM_NULL );
	tm_ptr = oam_localtime ( &now );
	if(OAM_NULL == tm_ptr)
	{
		OAM_LOG(OAM,OAM_WARNING,"tm_ptr is NULL");
		OAM_FUN_EXIT_TRACE();                                    /*COVERITY_FIX_1.0*/
		return OAM_FAILURE;					/*COVERITY_FIX_1.0*/
	}								/*COVERITY_FIX_1.0*/
	else							/*COVERITY_FIX_1.0*/
	{								/*COVERITY_FIX_1.0*/
		len = strftime ((char *)time_buffer, TIME_SIZE, "%d_%m_%Y_%I-%M-%S", tm_ptr );
	}
	if(len == OAM_ZERO)
		return OAM_FAILURE;
	else
		return OAM_SUCCESS;
}

/****************************************************************************
 * Function Name  : oam_cp_pack_U16
 * Inputs         : p_dest,p_src
 * Outputs        : none
 * Returns        : SUCCESS/FAILURE
 * Description    : This function packs U16 variable
 ****************************************************************************/

oam_return_et
oam_cp_pack_U16
(
 void    *p_dest,
 void    *p_src
 ,   char    *varname
 )
{
	/* Packing U16 */

	((U8*)p_dest)[0]    = (U8)((*(U16 *)p_src) >> 8);
	((U8*)p_dest)[1]    = (U8)((*(U16 *)p_src));
	OAM_LOG(OAM,OAM_DETAILEDALL, "%s = %d", varname, *((U16 *)p_src));
	return(OAM_SUCCESS);
}


/****************************************************************************
 * Function Name  : oam_cp_pack_U32
 * Inputs         : p_dest,p_src
 * Outputs        : none
 * Returns        : SUCCESS/FAILURE
 * Description    : This function packs UInt32 variable
 ****************************************************************************/


oam_return_et
oam_cp_pack_U32
(
 void    *p_dest,
 void    *p_src
 ,    char    *varname
 )
{
	/* Packing U32 */

	((U8*)p_dest)[0]    = (U8)((*(U32 *)p_src) >> 24);
	((U8*)p_dest)[1]    = (U8)((*(U32 *)p_src) >> 16);
	((U8*)p_dest)[2]    = (U8)((*(U32 *)p_src) >> 8);
	((U8*)p_dest)[3]    = (U8)((*(U32 *)p_src));
	OAM_LOG(OAM,OAM_DETAILEDALL, "%s = %d", varname, *((U32 *)p_src));
	return(OAM_SUCCESS);
}

/****************************************************************************
 * Function Name  : oam_cp_pack_S8
 * Inputs         : p_dest,p_src
 * Outputs        : none
 * Returns        : SUCCESS/FAILURE
 * Description    : This function packs S8 variable
 ****************************************************************************/
oam_return_et
oam_cp_pack_S8
(
 void    *p_dest,
 void    *p_src
 ,   char    *varname
 )
{
	OAM_LOG(OAM,OAM_DETAILEDALL, "%s = %d", varname, *((S8 *)p_src));
	oam_memcpy(p_dest, p_src, sizeof(S8));

	return(OAM_SUCCESS);
}

/****************************************************************************
 * Function Name  : oam_cp_unpack_UInt32
 * Inputs         : p_dest,p_src
 * Outputs        : none
 * Returns        : SUCCESS/FAILURE
 * Description    : This function unpacks UInt32 variable
 ****************************************************************************/
oam_return_et
oam_cp_unpack_UInt32
(
 void    *p_dest,
 void    *p_src
 ,   char    *varname
 )
{
	/* Unpacking UInt32 */

	*(UInt32 *)p_dest  = 0;

	*(UInt32 *)p_dest  |= (UInt32)((UInt8*)p_src)[0] << 24;
	*(UInt32 *)p_dest  |= (UInt32)((UInt8*)p_src)[1] << 16;
	*(UInt32 *)p_dest  |= (UInt32)((UInt8*)p_src)[2] << 8;
	*(UInt32 *)p_dest  |= (UInt32)((UInt8*)p_src)[3];
	OAM_LOG(OAM,OAM_DETAILEDALL, "%s = %d", varname, *((UInt32 *)p_dest));
	return(OAM_SUCCESS);
}

/****************************************************************************
 * Function Name  : oam_cp_pack_UInt32
 * Inputs         : p_dest,p_src
 * Outputs        : none
 * Returns        : SUCCESS/FAILURE
 * Description    : This function packs UInt32 variable
 ****************************************************************************/

oam_return_et
oam_cp_pack_UInt32
(
 void    *p_dest,
 void    *p_src
 ,  char    *varname
 )
{
	/* Packing UInt32 */

	((UInt8*)p_dest)[0]    = (UInt8)((*(UInt32 *)p_src) >> 24);
	((UInt8*)p_dest)[1]    = (UInt8)((*(UInt32 *)p_src) >> 16);
	((UInt8*)p_dest)[2]    = (UInt8)((*(UInt32 *)p_src) >> 8);
	((UInt8*)p_dest)[3]    = (UInt8)((*(UInt32 *)p_src));

	OAM_LOG(OAM,OAM_DETAILEDALL, "%s = %d", varname, *((UInt32 *)p_src));
	return(OAM_SUCCESS);
}
/****************************************************************************
 * Function Name  : oam_cp_pack_U64
 * Inputs         : p_dest,p_src
 * Outputs        : none
 * Returns        : SUCCESS/FAILURE
 * Description    : This function packs U64 variable
 ****************************************************************************/
oam_return_et
oam_cp_pack_U64
(
 void    *p_dest,
 void    *p_src
 ,char    *varname
 )
{
	/* Packing U64 */

	((U8*)p_dest)[0]    = (U8)((*(U64 *)p_src) >> 56);
	((U8*)p_dest)[1]    = (U8)((*(U64 *)p_src) >> 48);
	((U8*)p_dest)[2]    = (U8)((*(U64 *)p_src) >> 40);
	((U8*)p_dest)[3]    = (U8)((*(U64 *)p_src) >> 32);
	((U8*)p_dest)[4]    = (U8)((*(U64 *)p_src) >> 24);
	((U8*)p_dest)[5]    = (U8)((*(U64 *)p_src) >> 16);
	((U8*)p_dest)[6]    = (U8)((*(U64 *)p_src) >> 8);
	((U8*)p_dest)[7]    = (U8)((*(U64 *)p_src));
	OAM_LOG(OAM, OAM_DETAILEDALL, "%s = %d", varname, *((U64 *)p_src));
	return(OAM_SUCCESS);
}

/****************************************************************************
 * Function Name  : oam_cp_unpack_U64
 * Inputs         : p_dest,p_src
 * Outputs        : none
 * Returns        : SUCCESS/FAILURE
 * Description    : This function unpacks U64 variable
 ****************************************************************************/

oam_return_et
oam_cp_unpack_U64
(
 void    *p_dest,
 void    *p_src
 ,char    *varname
 )
{
	/* Unpacking U64 */

	*(U64 *)p_dest  = OAM_ZERO;

	*(U64 *)p_dest  |= (U64)((U8*)p_src)[0] << 56;
	*(U64 *)p_dest  |= (U64)((U8*)p_src)[1] << 48;
	*(U64 *)p_dest  |= (U64)((U8*)p_src)[2] << 40;
	*(U64 *)p_dest  |= (U64)((U8*)p_src)[3] << 32;
	*(U64 *)p_dest  |= (U64)((U8*)p_src)[4] << 24;
	*(U64 *)p_dest  |= (U64)((U8*)p_src)[5] << 16;
	*(U64 *)p_dest  |= (U64)((U8*)p_src)[6] << 8;
	*(U64 *)p_dest  |= (U64)((U8*)p_src)[7];
	OAM_LOG(OAM,OAM_DETAILEDALL, "%s = %d", varname, *((U64 *)p_dest));
	return(OAM_SUCCESS);
}

/****************************************************************************
 * Function Name  : oam_construct_qcli_header
 * Inputs         : p_header, transaction_id, src_module_id,
 dst_module_id, api_id, api_buf_size
 * Outputs        : none
 * Returns        : void
 * Description    : This function constructs qcli header
 ****************************************************************************/

void
oam_construct_qcli_header
(
 unsigned char *p_header,
 UInt16 transaction_id,
 UInt16 src_module_id,
 UInt16 dst_module_id,
 UInt16 api_id,
 UInt16 api_buf_size
 )
{

	/* transaction identifier (HI) */
	p_header[0] = ((transaction_id & 0xFF00) >> 8);

	/* transaction identifier (LOW) */
	p_header[1] = (transaction_id & 0x00FF);

	/* from (HI) */
	p_header[2] = ((src_module_id & 0xFF00) >> 8);

	/* from (LOW) */
	p_header[3] = (src_module_id & 0x00FF);

	/* to (HI) */
	p_header[4] = ((dst_module_id & 0xFF00) >> 8);

	/* to (LOW) */
	p_header[5] = (dst_module_id & 0x00FF);
	/* api id (HI) */
	p_header[6] = ((api_id & 0xFF00) >> 8);

	/* api id (LOW) */
	p_header[7] = (api_id & 0x00FF);


	/* api size (HI) */
	p_header[8] = ((api_buf_size & 0xFF00) >> 8);

	/* api size (LOW) */
	p_header[9] = api_buf_size & 0x00FF;

}

/****************************************************************************
 * Function Name  : oam_isValidIp6
 * Inputs         : pointer to string containing IP address 
 * Outputs        : none
 * Returns        : 0/1
 * Description    : This function checks whether IPv6 address is in 
 *                  correct format or not
 ****************************************************************************/

int oam_isValidIp6(UInt8 *p_str)
{
	struct addrinfo hint, *res = OAM_NULL;
	int ret;
	int ret_val = OAM_ZERO;
	oam_memset(&hint, OAM_ZERO, sizeof hint);

	hint.ai_family = PF_UNSPEC;
	hint.ai_flags = AI_NUMERICHOST;

	ret = oam_getaddrinfo((char * )p_str, OAM_NULL, &hint, &res);
	if (ret) 
	{
		/*SPR FIX 11324 + */
		return ret_val;   
		/*SPR FIX 11324 - */
	}
	if (res->ai_family == AF_INET6) 
	{
		ret_val = OAM_ONE ;
	} 

	OAM_FREEADDRINFO(res);
	return ret_val;

}

/****************************************************************************
 * Function Name  : oam_isValidIp4
 * Inputs         : pointer to string containing IP address
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function checks whether IPv4 address is in
 *                  correct format or not
 ****************************************************************************/

/* coverity warning 54870 changes start */
oam_return_et oam_isValidIp4 (UInt8 *p_str)
{
	SInt32 segs = OAM_ZERO;   /* Segment count. */
	SInt32 chcnt = OAM_ZERO;  /* Character count within segment. */
	SInt32 accum = OAM_ZERO;  /* Accumulator for segment. */
	/* Catch NULL pointer. */
	if (p_str == OAM_NULL)
		return OAM_FAILURE;


	/* Process every character in p_string. */
	while (*p_str != '\0')
	{
		/* Segment changeover. */
		if (*p_str == '.')
		{
			/* Must have some digits in segment. */

			if (chcnt == 0)
				return OAM_FAILURE;

			/* Limit number of segments. */

			if (++segs == 4)
				return OAM_FAILURE;

			/* Reset segment values and restart loop. */
			chcnt = accum = 0;
			p_str++;
			continue;
		}
		/* Check numeric. */
		if ((*p_str < '0') || (*p_str > '9'))
			return OAM_FAILURE;

		/* Accumulate and check segment. */
		if ((accum = accum * 10 + *p_str - '0') > 255)
			return OAM_FAILURE;

		/* Advance other segment specific stuff and continue loop. */

		/* Advance other segment specific stuff and continue loop. */

		chcnt++;
		p_str++;
	}


	/* Check enough segments and enough characters in last segment. */

	if (segs != 3)
		return OAM_FAILURE;

	if (chcnt == 0)
		return OAM_FAILURE;

	/* Address okay. */

	return OAM_SUCCESS;
}
/* coverity warning 54870 changes end */

/* Spr 15596 Fix Start */
/****************************************************************************
 * Function Name  : oam_isValidIp4SecGWServer
 * Inputs         : pointer to string containing IP address
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function checks whether IPv4 address is in
 *                  correct format or not
 ****************************************************************************/
oam_return_et oam_isValidIp4SecGWServer (UInt8 *p_str)
{
	SInt32 segs = OAM_ZERO;   /* Segment count. */
	SInt32 chcnt = OAM_ZERO;  /* Character count within segment. */
	SInt32 accum = OAM_ZERO;  /* Accumulator for segment. */
	/* Catch NULL pointer. */
	if (p_str == OAM_NULL)
		return OAM_FAILURE;


	/* Process every character in p_string. */
	while (*p_str != '\0')
	{
		/* Segment changeover. */
		if (*p_str == '.')
		{
			/* Must have some digits in segment. */

			if (chcnt == 0)
				return OAM_FAILURE;

			/* Limit number of segments. */

			if (++segs == 4)
				return OAM_FAILURE;

			/* Reset segment values and restart loop. */
			chcnt = accum = 0;
			p_str++;
			continue;
		}
		/* Check numeric. */
		if ((*p_str < '0') || (*p_str > '9'))
			return OAM_FAILURE;

		/* Accumulate and check segment. */
		if ((accum = accum * 10 + *p_str - '0') > 255)
			return OAM_FAILURE;

		/* Advance other segment specific stuff and continue loop. */
		chcnt++;
		p_str++;
	}

	return OAM_SUCCESS;
}
/* Spr 15596 Fix End */

/****************************************************************************
 * Function Name  : oam_get_new_trans_id
 * Inputs         : none 
 * Outputs        : none
 * Returns        : new trans id
 * Description    : This function returns the new transaction id
 ****************************************************************************/
/* coverity warning 54872 changes start */
UInt16
oam_get_new_trans_id
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */

)
{
	static UInt16 current_trans_id = OAM_ZERO;

	if ((current_trans_id + OAM_ONE) >= OAM_MAX_TRANS_ID)
	{
		/* trans-id has reached its limit, so restart the counter */
		current_trans_id = OAM_ZERO;
	}
	return ++current_trans_id;
}

/* coverity warning 54872 changes end */

/****************************************************************************
 * Function Name  : oam_start_new_timer
 * Inputs         : sub_module_id,expiry_thread_id,timer_type,timer_duration,
 *                  p_timer_data,timer_data_size,is_repeated
 * Outputs        : none
 * Returns        : oam_timer_t
 * Description    : This function strats the timer.
 ****************************************************************************/
	oam_timer_t
oam_start_new_timer(UInt16       sub_module_id,
		UInt16       expiry_thread_id,
		UInt16       timer_type,
		UInt32       timer_duration, /* unit: milliseconds */
		void         *buf,
		UInt16       buf_size,
		oam_bool_et  is_repeated)
{
	QTIME qtime = {OAM_ZERO};
	QMODULE module = {OAM_ZERO};
	oam_timer_t timer  = PNULL;
	oam_timer_data_t  *p_timer_data;
	oam_error_code_et error_code = NO_ERROR;

	p_timer_data = oam_msg_mem_get(sizeof(oam_timer_data_t));
	if(p_timer_data == PNULL) {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_timer_data failed");
		return PNULL;
	}

	p_timer_data->module_id = sub_module_id;
	p_timer_data->timer_type = timer_type;
	p_timer_data->buf = buf;
	p_timer_data->buf_size = buf_size;
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
		OAM_LOG(OAM, OAM_WARNING, "Timer Start failed for TimerType [%u]",
                timer_type);
		/* Release the memory allocated for storing the timer data */
		oam_mem_free(p_timer_data, &error_code);
		oam_reset_reason_code(OAM_INTERNAL_FAILURE);
		oam_abort();
	}
	else {
		OAM_LOG(OAM, OAM_BRIEF, "Timer Started: Timer [%p], TimerType [%u],"
                " TimerDuration [%u]sec [%u]msec",
				timer, timer_type, qtime.s, qtime.us/1000);
	}

	return timer;
}

/* SPR_9620_CHANGES_START */
/****************************************************************************
 * Function Name  : oam_start_guard_test_timer
 * Inputs         : trans_id,sub_module_id,expiry_thread_id,timer_type,timer_duration,
 *                  p_timer_data,timer_data_size,is_repeated
 * Outputs        : none
 * Returns        : oam_timer_t
 * Description    : This function starts guard timer.
 ****************************************************************************/
oam_timer_t
oam_start_guard_timer(
		UInt16       trans_id,
		UInt16       sub_module_id,
		UInt16       expiry_thread_id,
		UInt16       timer_type,
		UInt32       timer_duration, /* unit: milliseconds */
		void         *buf,
		UInt16       buf_size,
		oam_bool_et  is_repeated)
{
	QTIME qtime = {OAM_ZERO};
	QMODULE module = {OAM_ZERO};
	oam_timer_t timer  = PNULL;
	oam_timer_data_t  *p_timer_data;
	oam_error_code_et error_code = NO_ERROR;

	p_timer_data = oam_msg_mem_get(sizeof(oam_timer_data_t));
	if(p_timer_data == PNULL) {
        OAM_LOG(OAM,OAM_ERROR, "Memory allocation to p_timer_data failed");
		return PNULL;
	}

	p_timer_data->trans_id = trans_id;
	p_timer_data->module_id = sub_module_id;
	p_timer_data->timer_type = timer_type;
	p_timer_data->buf = buf;
	p_timer_data->buf_size = buf_size;
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
		OAM_LOG(OAM, OAM_BRIEF, "Timer Started: Timer [%p], TimerType [%u],"
                " TimerDuration [%u]sec [%u]msec",
				timer, timer_type, qtime.s, qtime.us/1000);
	}

	return timer;
}
/* SPR_9620_CHANGES_END */

/****************************************************************************
 * Function Name  : oam_fetch_convert_seconds_to_timestamp 
 * Inputs         : now,time_buffer
 * Outputs        : none
 * Returns        : void
 * Description    : This function converts current time into proper format/
 NULL if function fails
 ****************************************************************************/

/*BUG_FIX_ID : 743*/
void oam_fetch_convert_seconds_to_timestamp
(
 time_t now,
 Char8 *time_buffer
 )
{
	const struct tm *tm_ptr;

	tm_ptr = oam_localtime ( &now ); 
	if ( OAM_NULL != tm_ptr)
	{
		oam_memset(time_buffer, OAM_ZERO , OAM_TIME_SIZE_LEN);
		strftime ( time_buffer, OAM_TIME_SIZE_LEN, "%Y-%m-%dT%I:%M:%S", tm_ptr );
	}
	else
	{
		OAM_LOG(OAM,OAM_WARNING, "Function localtime is returning NULL");
	}

	return; 
}

/****************************************************************************
 * Function Name  : oam_fetch_current_timestamp
 * Inputs         : time_buffer
 * Outputs        : none
 * Returns        : void
 * Description    : This function fetches current time
 ****************************************************************************/

void oam_fetch_current_timestamp
(
 Char8 *time_buffer
 )
{
	const struct tm *tm_ptr;
	time_t now;

	now = oam_time ( OAM_NULL );
	tm_ptr = oam_localtime ( &now );
	oam_memset(time_buffer, OAM_ZERO , OAM_TIME_SIZE_LEN);
	/* coverity 35416 */
	if ( OAM_NULL != tm_ptr)
	{
		strftime ( time_buffer, OAM_TIME_SIZE_LEN, "%Y-%m-%dT%I:%M:%S", tm_ptr );
	}
	return;
}

/*SPR 21889 +- */
/****************************************************************************
 * Function Name  : oam_get_cli_communication_info
 * Inputs         : comm_info
 * Outputs        : none
 * Returns        : void
 * Description    : This function fetches cli communication info
 ****************************************************************************/

void oam_get_cli_communication_info(oam_cli_comm_info_t * comm_info)
{
	char read_buf[100]={0};
	FILE *fp           = OAM_NULL;
	fp = oam_fopen(OAM_ENB_CONFIG_FILE_NAME, "r");
	if(fp == OAM_NULL)
	{
		OAM_LOG(OAM, OAM_WARNING,
				"Unable to open %s", OAM_ENB_CONFIG_FILE_NAME);
		OAM_EXIT(1);
	}
	while (oam_fgets((char *)read_buf,100,fp))
	{
		if(snscanf((const char *)read_buf,sizeof("ENB_OAM_IP_ADDRESS=")+MAX_IP_ADD_LEN_STR , "ENB_OAM_IP_ADDRESS=%s",comm_info->oam_ip_addr) > 0 )
		{
			if(oam_isValidIp4 ((UInt8 *)comm_info->oam_ip_addr)==0)
			{
				OAM_LOG(OAM, OAM_WARNING,"Invalid OAM IP Address [%s]",comm_info->oam_ip_addr);
				OAM_EXIT(1);
			}

		}
		else if(snscanf((const char *)read_buf,sizeof("ENB_OAM_RX_PORT=")+MAX_PORT_LENGTH,"ENB_OAM_RX_PORT=%d",&comm_info->oam_port)>0)
		{
		}
		else if(snscanf((const char *)read_buf,sizeof("ENB_OAM_CLI_INTERFACE_IP_ADDRESS=")+MAX_IP_ADD_LEN_STR,"ENB_OAM_CLI_INTERFACE_IP_ADDRESS=%s",comm_info->cli_ip_addr) > 0 )
		{
			if(oam_isValidIp4 ((UInt8 *)comm_info->cli_ip_addr)==0)
			{             
				OAM_LOG(OAM, OAM_WARNING,"Invalid CLI Interface IP Address [%s]",comm_info->cli_ip_addr);
				OAM_EXIT(1);
			} 
		}
		else if(snscanf((const char *)read_buf,sizeof("ENB_OAM_CLI_INTERFACE_RX_PORT=")+MAX_PORT_LENGTH,"ENB_OAM_CLI_INTERFACE_RX_PORT=%d",&comm_info->cli_port)>0)
		{
		}
/* SPR 21889 +- */
#ifdef OAM_RADIO_HEAD_SUPPORT
		else if(snscanf((Char8*)read_buf, sizeof("ENB_OAM_RRH_IP_ADDRESS=") +
					MAX_IP_ADD_LEN_STR,
					"ENB_OAM_RRH_IP_ADDRESS=%s",
					comm_info->rrh_ip_addr) > 0)
		{
			if(oam_isValidIp4 ((UInt8 *)comm_info->rrh_ip_addr)==0)
			{             
				oam_fprintf(stderr,"\n Invalid RRH IP Address [%s]",comm_info->rrh_ip_addr);
				OAM_EXIT(1);
			} 
		}
		else if(snscanf((const char *)read_buf,sizeof("ENB_OAM_RRH_PORT=")+MAX_PORT_LENGTH,"ENB_OAM_RRH_PORT=%d",&comm_info->rrh_port)>0)
		{
		}
#endif
		else if(snscanf((const char *)read_buf,sizeof("ENB_TR069_IP_ADDRESS=")+MAX_IP_ADD_LEN_STR,"ENB_TR069_IP_ADDRESS=%s",comm_info->tr069_ip_addr)>0)
		{
			if(oam_isValidIp4 ((UInt8 *)comm_info->tr069_ip_addr)==0)
			{
				OAM_LOG(OAM, OAM_WARNING,"Invalid TR069 ip address [%s]",comm_info->tr069_ip_addr);
				OAM_EXIT(1);
			}
		}
		/* Bug fix 1107 */
		else if(snscanf((const char *)read_buf,sizeof("ENB_TR069_RX_PORT=")+MAX_PORT_LENGTH,"ENB_TR069_RX_PORT=%d",&comm_info->tr069_port)>0) 
		{    
		}  
		else if(snscanf((const char *)read_buf,sizeof("ENB_SNMP_IP_ADDRESS=")+MAX_IP_ADD_LEN_STR,"ENB_SNMP_IP_ADDRESS=%s",comm_info->snmp_ip_addr)>0)
		{
			if(oam_isValidIp4 ((UInt8 *)comm_info->snmp_ip_addr)==0)
			{
				OAM_LOG(OAM, OAM_WARNING,"Invalid SNMP ip address [%s]",comm_info->snmp_ip_addr);
				OAM_EXIT(1);
			}
		}
		else if(snscanf((const char *)read_buf,sizeof("ENB_SNMP_RX_PORT=")+MAX_PORT_LENGTH,"ENB_SNMP_RX_PORT=%d",&comm_info->snmp_port)>0)
		{
		}
		else if(snscanf((const char *)read_buf,sizeof("ENB_RRM_IP_ADDRESS=")+MAX_IP_ADD_LEN_STR,"ENB_RRM_IP_ADDRESS=%s",comm_info->rrm_ip_addr) > 0 )
		{
			if(oam_isValidIp4 ((UInt8 *)comm_info->rrm_ip_addr)==0)
			{             
				OAM_LOG(OAM, OAM_WARNING,"Invalid RRM ip address [%s]",comm_info->rrm_ip_addr);
				OAM_EXIT(1);
			} 
		}
		else if(snscanf((const char *)read_buf,sizeof("ENB_RRM_RX_PORT=")+MAX_PORT_LENGTH,"ENB_RRM_RX_PORT=%d",&comm_info->rrm_port)>0)
		{
		}
		/*SPR 21881 Start*/
        else if(snscanf((const char *)read_buf,sizeof("ENB_SON_IP_ADDRESS=")+MAX_IP_ADD_LEN_STR,"ENB_SON_IP_ADDRESS=%s",comm_info->son_ip_addr) > 0 )
		{
		/*SPR 21881 End*/
			if(oam_isValidIp4 ((UInt8 *)comm_info->son_ip_addr)==0)
			{             
				OAM_LOG(OAM, OAM_WARNING,"Invalid SON ip address [%s]",comm_info->son_ip_addr);
				OAM_EXIT(1);
			} 
		}
		/*SPR 21881 Start*/
		else if(snscanf((const char *)read_buf,sizeof("ENB_SON_OAM_RX_PORT=")+MAX_PORT_LENGTH,"ENB_SON_OAM_RX_PORT=%d",&comm_info->son_port)>0)
		/*SPR 21881 End*/
		{
		}
		else if(snscanf((const char *)read_buf,sizeof("ENB_L3_IP_ADDRESS=")+MAX_IP_ADD_LEN_STR,"ENB_L3_IP_ADDRESS=%s",comm_info->l3_ip_addr) > 0 )
		{
			if(oam_isValidIp4 ((UInt8 *)comm_info->l3_ip_addr)==0)
			{             
				OAM_LOG(OAM, OAM_WARNING,"Invalid L3 ip address [%s]",comm_info->l3_ip_addr);
				OAM_EXIT(1);
			} 
		}
		else if(snscanf((const char *)read_buf,sizeof("ENB_L3_PORT=")+MAX_PORT_LENGTH,"ENB_L3_PORT=%d",&comm_info->l3_port)>0)
		{
		}
        /* SPR 20908 Start */
        /* code removed */
/* SPR 20908 End */
	}
	oam_fclose(fp);
}

/****************************************************************************
 * Function Name  : oam_convert_3byte_int_to_byte_string
 * Inputs         : dest, source
 * Outputs        : none
 * Returns        : void
 * Description    : This function converts 3 byte interger into byte string
 ****************************************************************************/

void oam_convert_3byte_int_to_byte_string(UInt8* dest, UInt32 source)
{
	if (OAM_NULL == dest)
	{
		OAM_LOG(OAM, OAM_WARNING, "Null Argument[dest] Passed!!");
		return;
	}
	dest[0] = (UInt8) (source & 0x000000FF);
	dest[1] = (UInt8) ((source >> 8)& 0x000000FF);
	dest[2] = (UInt8) ((source >> 16)& 0x000000FF);
}

/*SPR 11650 FIX START*/
/****************************************************************************
 * Function Name  : oam_cmn_convert_byte_string_to_integer
 * Inputs         : bytes,num_of_bytes,bitstring_size
 * Outputs        : p_value
 * Returns        : void
 * Description    : This function is used to convert byte string to integer
 ****************************************************************************/
void oam_cmn_convert_byte_string_to_integer(
		UInt8        *p_bytes,
		UInt16       num_of_bytes,
		UInt32       *p_value,
		UInt32		 bitstring_size)
{
	UInt32       dec_no = OAM_ZERO;
	UInt8        byte_shift_value = OAM_ZERO, byte_array_index = OAM_ZERO;

	for(byte_array_index = num_of_bytes; byte_array_index > OAM_ZERO; byte_array_index--)
	{
		dec_no += p_bytes[byte_array_index-OAM_ONE] << byte_shift_value;
		byte_shift_value += OAM_EIGHT;

	}/* End of for loop */
	dec_no = dec_no >> bitstring_size;
	*p_value = dec_no;

	return;
}

/*SPR 11650 FIX END*/



/****************************************************************************
 * Function Name  : oam_cmn_convert_bytes_to_integer
 * Inputs         : bytes,num_of_bytes
 * Outputs        : p_value
 * Returns        : void
 * Description    : This function is used to convert bytes to integer
 ****************************************************************************/
void oam_cmn_convert_bytes_to_integer(
		UInt8        *p_bytes,
		UInt16       num_of_bytes,
		UInt32       *p_value)
{
	UInt32       dec_no = OAM_ZERO;
	UInt8        j = OAM_ZERO, idx = OAM_ZERO;

	for(idx = OAM_ZERO; idx < num_of_bytes; idx++)
	{
		dec_no += p_bytes[idx] << j;
		j += OAM_EIGHT;

	}/* End of for loop */

	*p_value = dec_no;

	return;
}


/*SPR 22398 Fix Start*/
/* SPR 21757 Fixed Start */
/****************************************************************************
 * Function Name  : oam_cmn_convert_bytes_to_integer_for_LAC
 * Inputs         : bytes,num_of_bytes
 * Outputs        : p_value
 * Returns        : void
 * Description    : This function is used to convert bytes to integer
 ****************************************************************************/
void oam_cmn_convert_bytes_to_integer_for_LAC(
           UInt8        *p_bytes,
           UInt32       *p_value)
{

   *p_value = oam_ntohs(*((UInt16 *)p_bytes));

  return;
}
/* SPR 21757 Fixed End */
/*SPR 22398 Fix End*/

/*SPR 11315 start*/
/****************************************************************************
 * Function Name  : oam_cmn_convert_byte_string_to_short 
 * Inputs         : bytes,bitstring_size
 * Outputs        : p_value
 * Returns        : void
 * Description    : This function is used to convert byte string to short
 ****************************************************************************/
void oam_cmn_convert_byte_string_to_short(
		UInt8        *p_bytes,
		UInt32       *p_value,
		UInt32		  bitstring_size)
{
	UInt32       dec_no = OAM_ZERO;
	UInt8        j = OAM_ZERO, idx = OAM_ZERO;

	for(idx = OAM_TWO; idx > OAM_ZERO; idx--)
	{
		dec_no += p_bytes[idx - OAM_ONE] << j;
		j += OAM_EIGHT;

	}/* End of for loop */
	dec_no = dec_no >> bitstring_size;
	*p_value = dec_no;

	return;
}
/*SPR 11315 end*/
/* BITSTRING FIXES START*/
/****************************************************************************
 * Function Name  : oam_cmn_convert_csgid_to_integer
 * Inputs         : bytes,num_of_bytes
 * Outputs        : p_value
 * Returns        : void
 * Description    : This function is used to convert csgid bitstring to integer
 ****************************************************************************/
void oam_cmn_convert_csgid_to_integer(
		UInt8        *p_bytes,
		UInt16       num_of_bytes,
		UInt32       *p_value)
{
	UInt32       dec_no = OAM_ZERO;
	UInt8        byte_shift_value = OAM_ZERO, byte_array_index = OAM_ZERO;

	for(byte_array_index = num_of_bytes; byte_array_index > OAM_ZERO; byte_array_index--)
	{
		dec_no += p_bytes[byte_array_index - OAM_ONE] << byte_shift_value;
		byte_shift_value += OAM_EIGHT;

	}/* End of for loop */

	dec_no = dec_no >> OAM_FIVE;
	*p_value = dec_no;

	return;
}
/* BITSTRING FIXES END*/

/************************************************************************************
 * ** ARRAY NAME   :   oam_table_event_type
 * ** DESCRIPTION  :   Array of structures, to store event type
 * **   
 * ** FIELDS       :   event type, event type string
 * ***********************************************************************************/

oam_event_t oam_table_event_type[] =
{
	{"Communication Alarm", COMMUNICATIONS_ALARM},
	{"Processing Error Alarm", PROCESSING_ERROR_ALARM},
	{"Memory Mismatch", MEMORY_MISMATCH},
	{"Configuration or Customization Error",
		CONFIGURATION_OR_CUSTOMIZATION_ERROR},
	{"Environmental Alarm", ENVIRONMENTAL_ALARM},
	{"Quality of Service Alarm", QUALITY_OF_SERVICE_ALARM},
	{"Equipment Alarm", EQUIPMENT_ALARM},
	{"Integrity Violation", INTEGRITY_VIOLATION},
	{"Operational Violation", OPERATIONAL_VIOLATION},
	{"Physical Violation", PHYSICAL_VIOLATION},
	{"Security Service of Mechanism Violation",
		SECURITY_SERVICE_OF_MECHANISM_VIOLATION},
	{"Time Domain Violation", TIME_DOMAIN_VIOLATION},
	/*eICIC_PHASE_1_2_CHANGES_START*/
	{"Notification Type",NOTIFICATION_TYPE}
	/*eICIC_PHASE_1_2_CHANGES_END*/
};

/************************************************************************************
 * ** ARRAY NAME   :   oam_table_probable_cause
 * ** DESCRIPTION  :   Array of structures, to store event type
 * **
 * ** FIELDS       :   probable cause, probable cause string
 * ***************************************************************************************/

oam_probable_t oam_table_probable_cause[] =
{
	{"Out Of Memory", OUT_OF_MEMORY},
	{"Software Error", SOFTWARE_ERROR},
	{"Communication Transmit Failure", COMMUNICATIONS_TRANSMIT_FAILURE},
	{"Communication Receive Failure", COMMUNICATIONS_RECEIVE_FAILURE},
	{"Software Program Error", SOFTWARE_PROGRAM_ERROR},
	{"Connection Establishment Error", CONNECTION_ESTABLISHMENT_ERROR},
	{"Unavailable", UNAVAILABLE}
};

/************************************************************************************
 * ** ARRAY NAME   :   oam_table_severity
 * ** DESCRIPTION  :   Array of structures, to store event type
 * **
 * ** FIELDS       :   probable cause, probable cause string
 * **********************************************************************************/


oam_severity_t oam_table_severity[] =
{
	{"Critical", CRITICAL},
	{"Major", MAJOR},
	{"Minor", MINOR},
	{"Warning", WARNING},
	{"Indeterminate", INDETERMINATE},
	{"Cleared", CLEARED}
};


/************************************************************************************
 * ** ARRAY NAME   :   oam_table_report
 * ** DESCRIPTION  :   Array of structures, to store event type
 * **
 * ** FIELDS       :   probable cause, probable cause string
 * ***********************************************************************************/

oam_report_t oam_table_report[] =
{
	{"0 Expedited",EXPEDITED_ALARM},
	{"1 Queued",QUEUED_ALARM},
	{"2 Logged",LOGGED_ALARM},
	{"3 Disabled",DISABLED_ALARM},
	{"4 Report Mechanism Undefined",REPORT_MECHANISM_UNDEFINED},
};

/****************************************************************************
 * Function Name  : oam_get_alarm_probable_cause_id_by_cause
 * Inputs         : pointer to cause string
 * Outputs        : none
 * Returns        : probable_cause_id/OAM_FAILURE
 * Description    : This function gets  event id from event
 ****************************************************************************/

UInt8 oam_get_alarm_probable_cause_id_by_cause(char* cause)
{
	UInt16 loop = OAM_ZERO;
	for(loop = OAM_ZERO; loop<ARRSIZE(oam_table_probable_cause); loop++)
	{
		if((oam_strncmp(oam_table_probable_cause[loop].probable_cause,cause,oam_strlen(cause))) == OAM_ZERO)
		{
			return oam_table_probable_cause[loop].probable_cause_id;
		}
		continue;
	}
	return OAM_FAILURE;
}

/****************************************************************************
 * Function Name  : oam_get_alarm_event_id_by_event 
 * Inputs         : pointer to event string
 * Outputs        : none
 * Returns        : event id/OAM_FAILURE
 * Description    : This function gets alarm event id from event
 ****************************************************************************/

UInt8 oam_get_alarm_event_id_by_event(char* event)
{
	UInt8 loop = OAM_ZERO;
	for(loop = OAM_ZERO; loop<ARRSIZE(oam_table_event_type); loop++)
	{
		if((oam_strncmp(oam_table_event_type[loop].event_type,event,oam_strlen(event))) == OAM_ZERO)
		{
			return oam_table_event_type[loop].event_type_id;
		}
	}
	return OAM_FAILURE;
}

/****************************************************************************
 * Function Name  : oam_get_alarm_severity_id_by_severity
 * Inputs         : pointer to severity string
 * Outputs        : none
 * Returns        : severity id/OAM_FAILURE
 * Description    : This function gets alarm severity id from severity
 ****************************************************************************/

UInt8 oam_get_alarm_severity_id_by_severity(char* severity)
{
	UInt8 loop = OAM_ZERO;
	for(loop = OAM_ZERO; loop<ARRSIZE(oam_table_severity); loop++)
	{
		if((oam_strncmp(oam_table_severity[loop].severity,severity,oam_strlen(severity))) == OAM_ZERO)
		{
			return oam_table_severity[loop].severity_id;
		}
	}
	return OAM_FAILURE;
}

/****************************************************************************
 * Function Name  : oam_get_alarm_report_id_by_report
 * Inputs         : pointer to report string
 * Outputs        : none
 * Returns        : report id/LOGGED_ALARM
 * Description    : This function gets alarm report id from report
 ****************************************************************************/

UInt8 oam_get_alarm_report_id_by_report(char* report)
{
	UInt8 loop = OAM_ZERO;
	if (OAM_ZERO == oam_strlen(report))
	{
		return LOGGED_ALARM;
	}
	for(loop = OAM_ZERO; loop<ARRSIZE(oam_table_report); loop++)
	{
		if((oam_strncmp(oam_table_report[loop].report_mechanism,report,oam_strlen(report))) == OAM_ZERO)
		{
			return oam_table_report[loop].report_id;
		}
	}
	return LOGGED_ALARM;
}


/****************************************************************************
 * Function Name  : oam_trim_spaces
 * Inputs         : p_src: string to be trimmed
 *                  p_dest: output string
 *                  
 * Outputs        : p_dest
 * Returns        : index_dest/OAM_FAILURE 
 * Description    : This function is used to trim spaces
 ****************************************************************************/
UInt16 oam_trim_spaces(Char8* p_src, Char8* p_dest)
{
	UInt16 index_src = OAM_ZERO;
	UInt16 index_dest = OAM_ZERO;
	UInt16 len = OAM_ZERO;

	if((OAM_NULL == p_src) || (OAM_NULL == p_dest))
	{
		OAM_LOG(OAM, OAM_WARNING, "NULL argument(s) passed!! p_src=[%p], p_dest=[%p]",
                p_src, p_src);
		return OAM_FAILURE;
	}

	len = oam_strlen(p_src);
	for(index_src = OAM_ZERO; index_src < len; index_src++)
	{
		if (' ' != p_src[index_src])
		{
			p_dest[index_dest++] = p_src[index_src];
		}
	}
	p_dest[index_dest] = '\0';
	return index_dest;
}

/****************************************************************************
 * Function Name  : oam_tokenize_string_to_bitmap_array
 * Inputs         : p_str: string to be tokenized
 *                  p_delim: delimiter string
 *                  max_tokens: maximum number of tokens allowed
 * Outputs        : p_token: array of UInt8 type
 * Returns        : number of tokens successfully read
 * Description    : This function is used to convert a delimited string to an
 *                  array of UInt8s
 ****************************************************************************/
UInt8 oam_tokenize_string_to_bitmap_array(Char8* p_str,
		const Char8* p_delim,
		UInt32 max_tokens,
		UInt8 *p_tokens)
{
	OAM_FUN_ENTRY_TRACE();
	UInt32 token_count = 0;
	Char8 *p_temp = OAM_NULL;
	/*SPR 16151 Fixed Start*/
	Char8 temp_str[MAX_PARAMETER_NAME_LENGTH_VALUE] = {0};
	/*SPR 16151 Fixed End*/

	SInt32 base = 16;

	if(!p_str || !p_tokens || !max_tokens) {
		OAM_LOG(OAM, OAM_WARNING, "NULL argument(s) passed. p_str=[%p], p_tokens=[%p], max_tokens=[%d]",
                p_str, p_tokens, max_tokens);
		OAM_FUN_EXIT_TRACE();
		return token_count;
	}

	p_temp = oam_strtok(p_str, p_delim);
	if (p_temp) {
		oam_trim_spaces(p_temp, temp_str);
		p_tokens[token_count++] = oam_strtol(temp_str, OAM_NULL, base);

		while(max_tokens > token_count) {
			p_temp = oam_strtok(OAM_NULL, p_delim);
			if(!p_temp) {
				break;
			}

			oam_trim_spaces(p_temp, temp_str);
			p_tokens[token_count++] = oam_strtol(temp_str, OAM_NULL, base);
		}
	}

	OAM_FUN_EXIT_TRACE();
	return token_count;
}

/* EMBMS Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
/****************************************************************************
 * Function Name  : oam_bit_string_to_uint8_array
 * Inputs         : p_str: string to be tokenized
 *                  p_delim: delimiter string
 *                  max_tokens: maximum number of tokens allowed
 * Outputs        : p_token: array of UInt8 type
 * Returns        : number of tokens successfully read
 * Description    : This function is used to convert a delimited string to an
 *                  array of UInt8s
 ****************************************************************************/
UInt16 oam_bit_string_to_uint8_array(Char8* p_str,
         const Char8* p_delim,
                                    UInt16 max_tokens,
         UInt8* p_tokens)
{
    UInt16 token_count = OAM_ZERO;
    UInt8 bit = OAM_ZERO;
    UInt8 dec = OAM_ZERO;
    UInt8 num_bits = OAM_ZERO;
    Char8 *p_temp = OAM_NULL;
    OAM_FUN_ENTRY_TRACE();

    if (!p_str || !max_tokens)
    {
		OAM_LOG(OAM, OAM_ERROR, "Invalid arguements passed!!");
		OAM_FUN_EXIT_TRACE();
        return token_count;
    }
    if(oam_strlen(p_str) == SUB_FRAME_ALLOC_STR_MIN_LEN)
    {
        oam_sprintf((p_str + SUB_FRAME_ALLOC_STR_MIN_LEN),
                "%c%c%c%c", ',', '0', ',','0');
    }

    p_temp = oam_strtok(p_str, p_delim);
    while (p_temp && (max_tokens > token_count))
    {
        bit = oam_atoi(p_temp);
        dec = (dec << 1) | bit;
        ++num_bits;

        if (OAM_BYTE_SIZE_IN_BITS == num_bits)
        {
            p_tokens[token_count++] = dec;
            dec = OAM_ZERO;
            num_bits = OAM_ZERO;
        }
        p_temp = oam_strtok(OAM_NULL, p_delim);
    }

     OAM_FUN_EXIT_TRACE();
    return token_count;
}

/****************************************************************************
 * Function Name  : oam_uint8_array_to_bit_string
 * Inputs         : p_bitmap_arr: array of UInt
 *                  p_delim: delimiter string
 *                  p_str: output string
 *                  str_len: length of string
 * Outputs        : p_str
 * Returns        : OAM_SUCCCESS, OAM_FAILURE
 * Description    : This function is used to convert an array of UInt octets
 *                  to comma separated bit string
 ***************************************************************************/
UInt32 oam_uint8_array_to_bit_string(UInt8* p_uint8_arr,
                                    UInt16 num_elem,
                                    Char8* p_out_str,
                                    UInt16 max_str_size)
{
    OAM_FUN_ENTRY_TRACE();

    UInt32 count = OAM_ZERO;
    SInt8 i = OAM_ZERO;
    UInt8 bit = OAM_ZERO;
    SInt32 str_size = OAM_ZERO;

    if (!p_uint8_arr || !p_out_str) 
    {
		OAM_LOG(OAM, OAM_ERROR, "Invalid arguements passed!!");

		OAM_FUN_EXIT_TRACE();
        return str_size;
	}

   /*decimal to binary conversion*/ 
    for (count = OAM_ZERO; count < num_elem; ++count)
    {
        for (i = OAM_BYTE_SIZE_IN_BITS - 1; i >= OAM_ZERO; --i)
        {
            bit = p_uint8_arr[count] >> i;

            if (str_size < max_str_size - 2)
            {
                if (bit & 1)
                {
                    oam_strcat(p_out_str, "1,");
            }
            else
            {
                    oam_strcat(p_out_str, "0,");
            }

                str_size += 2;
        }
            else
            {
                if (bit & 1)
                {
                    oam_strcat(p_out_str, "1");
    }
                else
    {
                    oam_strcat(p_out_str, "0");
    }

                ++str_size;
                break;
            }
        }

        if (str_size >= max_str_size - 1)
        {
            break;
        }
 }

    p_out_str[str_size] = 0;

    OAM_FUN_EXIT_TRACE();
    return str_size;
}

#endif
/* EMBMS Changes End */
/****************************************************************************
 * Function Name  : oam_convert_bitmap_arr_to_delimited_string
 * Inputs         : p_bitmap_arr: array of octet bitmap
 *                  arr_size: size of array
 *                  delim: delimiter string
 *                  p_str: output string
 *                  str_size: size of string
 * Outputs        : p_str
 * Returns        : OAM_SUCCCESS, OAM_FAILURE
 * Description    : This function is used to convert an array of bitmap octets
 *                  to delimited string of hex numbers numbers
 ***************************************************************************/
	oam_return_et
oam_convert_bitmap_arr_to_delimited_string(UInt8* p_bitmap_arr,
		UInt8 arr_size,
		Char8 delim,
		Char8 *p_str,
		UInt32 str_size)
{
	OAM_FUN_ENTRY_TRACE();
	UInt32 token_count = 0;
	SInt32 curr_pos = 0;
	oam_return_et retVal = OAM_SUCCESS;
	SInt32 left_size = str_size - 1;

	if(!p_str || !p_bitmap_arr) {
		OAM_LOG(OAM, OAM_WARNING, "NULL argument(s) passed!!. p_str=[%p], p_bitmap_arr=[%p]",
                p_str, p_bitmap_arr);
		OAM_FUN_EXIT_TRACE();
		retVal = OAM_FAILURE;
		return retVal;
	}

	for(token_count = 0; token_count < arr_size; ++token_count) {
		snprintf(p_str + curr_pos, left_size, "%x%c",
				p_bitmap_arr[token_count], delim);
		curr_pos = oam_strlen(p_str);
		left_size = str_size - curr_pos - 1;
	}

	OAM_FUN_EXIT_TRACE();
	return retVal;
}

/****************************************************************************
 * Function Name  : oam_convert_u16_arr_to_delimited_string
 * Inputs         : p_num_arr: array of UInt16 numbers
 *                  arr_size: size of array
 *                  p_delim: delimiter string
 *                  p_str: output string
 *                  str_size
 * Outputs        : p_str
 * Returns        : OAM_SUCCCESS, OAM_FAILURE
 * Description    : This function is used to convert a UInt16 array into a
 *                  delimited string of UInt16 numbers
 ***************************************************************************/
	oam_return_et
oam_convert_u16_arr_to_delimited_string(UInt16* p_num_arr,
		UInt8 arr_size,
		Char8 delim,
		Char8 *p_str,
		UInt32 str_size)
{
	OAM_FUN_ENTRY_TRACE();
	UInt32 token_count = 0;
	SInt32 curr_pos = 0;
	oam_return_et retVal = OAM_SUCCESS;
	SInt32 left_size = str_size - 1;

	if(!p_str || !p_num_arr) {
		OAM_LOG(OAM, OAM_WARNING, "NULL arguement(s) passed!! p_str=[%p], p_num_arr=[%p]",
                p_str, p_num_arr);
		OAM_FUN_EXIT_TRACE();
		retVal = OAM_FAILURE;
		return retVal;
	}

	for(token_count = 0; token_count < arr_size; ++token_count) {
		snprintf(p_str + curr_pos, left_size, "%d%c",
				p_num_arr[token_count], delim);
		curr_pos = oam_strlen(p_str);
		left_size = str_size - curr_pos - 1;
	}

	OAM_FUN_EXIT_TRACE();
	return retVal;
}

/****************************************************************************
 * Function Name  : oam_tokenize_string_to_u16
 * Inputs         : p_str: string to be tokenized
 *                  p_delim: delimiter string
 *                  max_tokens: maximum number of tokens allowed
 * Outputs        : p_tokens: array of u16 type
 * Returns        : number of tokens successfully interpreted
 * Description    : This function is used to convert bytes to integer
 ****************************************************************************/
UInt32 oam_tokenize_string_to_u16(Char8* p_str, const Char8* p_delim,  
		UInt32 max_tokens, UInt16 *p_tokens)  
{
	OAM_FUN_ENTRY_TRACE();
	UInt32 token_count = OAM_ZERO;
	Char8 *p_temp = OAM_NULL;
	/*SPR 16151 Fixed Start*/
	Char8 temp_str[MAX_PARAMETER_NAME_LENGTH_VALUE] = {0};
	/*SPR 16151 Fixed End*/

	if(!p_str || !p_tokens || !max_tokens) {
		OAM_LOG(OAM, OAM_WARNING, "NULL argument(s) passed. p_str=[%p], p_tokens=[%p], max_tokens=[%d]",
                p_str, p_tokens, max_tokens);
		OAM_FUN_EXIT_TRACE();
		return token_count;
	}

	p_temp = oam_strtok(p_str, p_delim);
	if(p_temp) {
		oam_trim_spaces(p_temp, temp_str);
		p_tokens[token_count] = oam_atoi(temp_str);
		++token_count;
		while(max_tokens > token_count) {                     
			p_temp = oam_strtok(OAM_NULL, p_delim);
			if(!p_temp) {
				break;
			}

			oam_trim_spaces(p_temp, temp_str);
			p_tokens[token_count] = oam_atoi(temp_str);
			++token_count;
		}
	}

	OAM_FUN_EXIT_TRACE();
	return token_count;
}

/* SPR 13024 FIX START */
/****************************************************************************
 * Function Name  : oam_tokenize_string_to_u16_ext
 * Inputs         : p_str: string to be tokenized
 *                  item_sep: delimiter to separate individual tokens
 *                  range_sep: delimiter to identify a range of tokens
 *                  max_tokens: maximum number of tokens allowed
 * Outputs        : p_tokens: array of u16 type
 * Returns        : number of tokens successfully interpreted
 * Description    : This function is used to convert a character string to
 *                  a list of UInt16 integer numbers
 ****************************************************************************/
UInt32 oam_tokenize_string_to_u16_ext(const Char8* p_str,
		const Char8* item_sep,  
		const Char8* range_sep,
		UInt16 *p_tokens,
		UInt32 max_tokens,
		oam_error_code_et *p_err)
{
	OAM_FUN_ENTRY_TRACE();
	UInt32 token_count = OAM_ZERO;
	Char8 *token = OAM_NULL;
	Char8 *save_ptr = OAM_NULL;
	Char8 *save_ptr1 = OAM_NULL;
	Char8 temp_str[MAX_PARAMETER_NAME_LENGTH] = {OAM_ZERO};
	SInt32 str_len = OAM_ZERO;
	oam_return_et retVal = OAM_FAILURE;
	Char8 *list = OAM_NULL;
	Char8 *range_val = OAM_NULL;
	SInt32 start_val = OAM_ZERO;
	SInt32 end_val = OAM_ZERO;
	SInt32 count = OAM_ZERO;

	if(!p_str || !p_tokens || !p_err) {
		OAM_LOG(OAM, OAM_WARNING, "NULL argument(s) passed. p_str=[%p], p_tokens=[%p], max_tokens=[%d] p_err=[%p]",
                p_str, p_tokens, max_tokens, p_err);

		if (p_err) {
			*p_err = OAM_NULL_POINTER_ERR;
		}

		OAM_FUN_EXIT_TRACE();
		return token_count;
	}

	str_len = oam_strlen(p_str) + 1;

	retVal = oam_mem_alloc(str_len, (void**)&list, p_err);

	if (OAM_FAILURE == retVal) {
           OAM_LOG(OAM, OAM_ERROR,
                "Memory allocation to list failed"
                " with Error Code:%d",
                *p_err);
		*p_err = OAM_MALLOC_FAIL;

		OAM_FUN_EXIT_TRACE();
		return token_count;
	}

	oam_memset(list, 0, str_len);
	oam_strcpy(list, p_str);

	token = oam_strtok_r(list, item_sep, &save_ptr);
	/* SPR 13060 FIX START */
	while (token && token_count <= max_tokens) {
		/* SPR 13060 FIX END */
		oam_memset(temp_str, 0, sizeof(temp_str));
		oam_trim_spaces(token, temp_str);

		if (range_sep && oam_strstr(temp_str, range_sep)) {
			save_ptr1 = OAM_NULL;
			range_val = oam_strtok_r(temp_str, range_sep, &save_ptr1);

			if (!range_val) {
				token = oam_strtok_r(OAM_NULL, item_sep, &save_ptr);
				continue;
			}

			start_val = oam_atoi(range_val);
			range_val = oam_strtok_r(OAM_NULL, range_sep, &save_ptr1);

			if (!range_val) {
				token = oam_strtok_r(OAM_NULL, item_sep, &save_ptr);
				continue;
			}

			end_val = oam_atoi(range_val);

			for (count = start_val; count <= end_val; ++count) {
				p_tokens[token_count++] = count;
			}

			start_val = end_val = OAM_ZERO;
		}
		else {
			p_tokens[token_count++] = oam_atoi(temp_str);
		}

		token = oam_strtok_r(OAM_NULL, item_sep, &save_ptr);
	}

	oam_mem_free(list, p_err);

	OAM_FUN_EXIT_TRACE();
	return token_count;
}
/* SPR 13024 FIX END */


/****************************************************************************
 * Function Name  : oam_convert_log_level 
 * Inputs         : oam_log_level
 * Outputs        : log _level
 * Returns        : AdvL_LogLevel
 * Description    : This function is used to convert log level
 ****************************************************************************/

AdvL_LogLevel oam_convert_log_level (SInt32 oam_log_level)
{
	AdvL_LogLevel log_level;
	switch(oam_log_level)
	{
		case OAM_CRITICAL:
			log_level=LogFatal;
			break;
		case OAM_ERROR:
			log_level=LogError;
			break;
		case OAM_WARNING:
			log_level=LogWarning;
			break;
		case OAM_INFO:
			log_level=LogInfo;
			break;
		case OAM_BRIEF:
			log_level=LogBrief;
			break;
		case OAM_DETAILED:
			log_level=LogDetailed;
			break;
        case OAM_DETAILEDALL:
            log_level=LogTrace;
            break;
		default:
			log_level=LogTrace;
			break;

	}
	return log_level;
}

/* SPR 21275 changes start */
/*****************************************************************************
 * Function Name  : oam_map_log_level_to_lgr_log_level_type 
 * Inputs         : AdvL_LogLevel  log_level
 * Outputs        : None
 * Returns        : lgr_log_level_et
 * Description    : This function does the conversion of log leve from type AdvL_LogLevel
 *                  to lgr_log_level_et
 ********************************************************************************/
lgr_log_level_et oam_map_log_level_to_lgr_log_level_type (log_priority_et log_level)
{
    switch (log_level)
    {
        case OAM_CRITICAL:
        return LGR_CRITICAL;

        case OAM_ERROR:
        return LGR_ERROR;

        case OAM_WARNING:
        return LGR_WARNING;

        case OAM_INFO:
        return LGR_INFO;

        case OAM_BRIEF:
        return LGR_BRIEF;

        case OAM_DETAILED:
        return LGR_DETAILED;

        case OAM_DETAILEDALL:
        return LGR_DEBUG;

        default:
        return LGR_TRACE;

    }
}
/* SPR 21275 changes end */

/****************************************************************************
 * Function Name  : oam_init_logging
 * Inputs         : log_category, *log_data, log_level, log_size
 * Returns        : SInt32
 * Description    : This function is used to initialize the logger 
 ****************************************************************************/

SInt32 oam_init_logging
(
 oam_log_category_et log_category,
 oam_log_data_st *log_data,
 SInt32 oam_log_level,
 SInt32 log_size
 )
{
	Char8 init_data[OAM_OBJ_NAME_MAX_LEN] = {0};
	/* coverity 35620 */
	AdvL_LogTarget   log_target = LogTarget_None;
	AdvL_ContextIndex log_index = OAM_ZERO;               /* typedef unsigned short AdvL_ContextIndex */
	AdvL_LogLevel log_level;
    AdvL_LogError ret_val       = LogSuccess;

	if (OAM_NULL == log_data)
	{
		oam_fprintf(stderr, "\n Null Argument passed to oam_init_logging");
		return OAM_RESULT_FAILURE;
	}

	oam_memset(init_data, OAM_ZERO, sizeof(init_data));   

	switch(log_category)
	{
		case OAM_LOG_CATEGORY_FILE:
			log_target = LogTarget_File;
			/* KLOCWORK warning fix start */
			oam_strncpy(init_data, log_data->filepath,
					sizeof(init_data) - 1);  /*COVERITY FIX 29may*/
			/* KLOCWORK warning fix end */
			break;
		case  OAM_LOG_CATEGORY_CONSOLE:
			log_target = LogTarget_Console;
			break;
		case OAM_LOG_CATEGORY_SYSLOG:
			log_target = LogTarget_Syslog;
			/* KLOCWORK warning fix start */
			oam_strncpy(init_data, log_data->apn_name,
					sizeof(init_data) - 1);   /*COVERITY FIX 29may*/
			/* KLOCWORK warning fix end */
			break;
		case OAM_LOG_CATEGORY_NETWORK:
			log_target = LogTarget_Network;
			/* KLOCWORK warning fix start */
			oam_strncpy(init_data, log_data->network_ip_port,
					sizeof(init_data) - 1); /*COVERITY FIX 29may*/
			/* KLOCWORK warning fix end */
			break;
	}

    ret_val        = init_log(log_target, init_data,&log_index);
	if(LogSuccess != ret_val)
	{
		OAM_LOG(OAM,OAM_WARNING, "Logging initialization failed with Error Value[%d]",ret_val);
		return OAM_RESULT_FAILURE;
	}
	else
	{
		OAM_LOG(OAM,OAM_DETAILED, "Logging initialization successful");
	}

	if(OAM_LOG_CATEGORY_FILE == log_category)
	{
		set_rollover_size(log_index,log_size);
	}

	log_level = oam_convert_log_level(oam_log_level);
	set_loglevel_limit(log_index,log_level);

	return OAM_RESULT_SUCCESS;
}

/****************************************************************************
 ** Function Name  : oam_log_message
 ** Inputs         : log_level, source_file_name, calling_function, line_number
 **                  format_string, arguments
 ** Returns        : void
 ** Description    : This function is used to log a message
 *****************************************************************************/

void oam_log_message
(
 SInt32 oam_log_level,
 SInt8 *pFile,
 SInt8 *pFun,
/* SPR 15438 Start */
 UInt32 Line,
/* SPR 15438 End */
 SInt8* format, 
 ...
 )
{
	va_list arg_list;
	AdvL_LogLevel log_level;
	Char8 temp_str[OAM_MAX_LOG_MSG_SIZE];

	if (OAM_NULL == pFile || OAM_NULL == pFun)
	{
		oam_fprintf(stderr, "\n Null Argument passed to oam_init_logging");
		return; 
	}
	OAM_VA_START(arg_list,format);
	oam_memset(temp_str,0,OAM_MAX_LOG_MSG_SIZE); 

	log_level = oam_convert_log_level(oam_log_level);
	OAM_VSPRINTF(temp_str, (const Char8*)format, arg_list);
	write_log_message(LogContext_Default,log_level,(const Char8*)pFile,Line,
			(const Char8*)pFun,"OAM",0xff,0xffff,"%s", temp_str);
	OAM_VA_END(arg_list);
}
/****************************************************************************
 * Function Name  : oam_cp_unpack_SInt16
 * Inputs         : p_dest,p_src
 * Outputs        : none
 * Returns        : SUCCESS/FAILURE
 * Description    : This function unpacks SInt16 variable
 ****************************************************************************/

oam_return_et
oam_cp_unpack_SInt16
(
 void    *p_dest,
 void    *p_src
 , char    *varname
 )
{
	/* Unpacking S16 */

	*(S16 *)p_dest  = OAM_ZERO;

	*(S16 *)p_dest  |= (S16)((U8*)p_src)[OAM_ZERO] << OAM_EIGHT;
	*(S16 *)p_dest  |= (S16)((U8*)p_src)[OAM_ONE];

	OAM_LOG(OAM,OAM_DETAILEDALL, "%s = %d", varname, *((SInt16 *)p_dest));

	return(OAM_SUCCESS);
}

/*SPR FIX 7851*/
/****************************************************************************
 * Function Name  : oam_convert_int_to_byte_string_for_cell_trace 
 * Inputs         : dest :Pointer to dest
 *                  source :Source
 * Outputs        : None
 * Returns        : None
 * Description    : This function converts int into byte string
 ****************************************************************************/
void
oam_convert_int_to_byte_string_for_cell_trace(
		UInt8* dest,
		UInt32 source
		)
{
	if (OAM_NULL == dest)
	{
		OAM_LOG(OAM, OAM_WARNING, "Null Argument[dest] Passed!!");
		return;
	}
	dest[OAM_ZERO] = (UInt8) (source & 0x000000FF);
	dest[OAM_ONE] = (UInt8) ((source >> OAM_EIGHT)& 0x000000FF);
	dest[OAM_TWO] = (UInt8) ((source >> OAM_SIXTEEN)& 0x000000FF);
}

/****************************************************************************
 * Function Name  : oam_isnumber
 * Inputs         : pointer to str
 * Outputs        : checks whether the str is number or not 
 * Returns        : retVal(0/1)
 * Description    : This function checks whether input is a number or not. 
 ****************************************************************************/

SInt32 oam_isnumber(Char8 *str)
{
	SInt32 retVal = 1;
	SInt32 cnt = 0;
	if(!str) {
		OAM_LOG(OAM, OAM_WARNING, "Null pointer[str] error");
		return OAM_FAILURE;
	}

	for(cnt = 0; cnt < oam_strlen(str); ++cnt) {
		if(!OAM_ISDIGIT(str[cnt])) {
			retVal = 0;
			break;
		}
	}

	return retVal;
}
/* SPR 14016 START */
/* Code Moved to sim_utils.c */
/* SPR 14016 END */

/****************************************************************************
 * Function Name  : oam_encode_eutran_cellidentity_into_bit_stream
 * Inputs         : p_byte_arr,cell identity
 * Outputs        : p_byte_arr: encoded byte array
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function encodes the cell identity into byte array
 ****************************************************************************/
	oam_return_et
oam_encode_eutran_cellidentity_into_bit_stream(UInt8 *p_byte_arr,
		UInt32 cell_identity)
{
	UInt8 offset = EUTRAN_CID_BYTE_OFFSET;

	if(!p_byte_arr) {
		OAM_LOG(OAM, OAM_WARNING, "Null pointer error[p_byte_arr]");
		return OAM_FAILURE;
	}

	if(cell_identity > OAM_MAX_EUTRAN_CELL_IDENTITY_VALUE) {
		OAM_LOG(OAM, OAM_WARNING,
				"CellIdentity: %u value exceeds max value: %d",
				cell_identity, OAM_MAX_EUTRAN_CELL_IDENTITY_VALUE);
		return OAM_FAILURE;
	}

	oam_memset(p_byte_arr, OAM_ZERO, MAX_CELL_IDENTITY_OCTETS);

	p_byte_arr[offset] = (cell_identity & 0xf) << OAM_NIBBLE_SIZE_IN_BITS;
	OAM_LOG(OAM, OAM_DETAILED, "EUTRAN CellIdentity[%d]: %d",
			offset, p_byte_arr[offset]);
	--offset;

	p_byte_arr[offset] = (cell_identity & 0xf0) >> OAM_NIBBLE_SIZE_IN_BITS;
	cell_identity >>= OAM_BYTE_SIZE_IN_BITS;
	p_byte_arr[offset] |= (cell_identity & 0xf) << OAM_NIBBLE_SIZE_IN_BITS;
	OAM_LOG(OAM, OAM_DETAILED, "EUTRAN CellIdentity[%d]: %d",
			offset, p_byte_arr[offset]);
	--offset;

	p_byte_arr[offset] = (cell_identity & 0xf0) >> OAM_NIBBLE_SIZE_IN_BITS;
	cell_identity >>= OAM_BYTE_SIZE_IN_BITS;
	p_byte_arr[offset] |= (cell_identity & 0xf) << OAM_NIBBLE_SIZE_IN_BITS;
	OAM_LOG(OAM, OAM_DETAILED, "EUTRAN CellIdentity[%d]: %d",
			offset, p_byte_arr[offset]);
	--offset;

	p_byte_arr[offset] = (cell_identity & 0xf0) >> OAM_NIBBLE_SIZE_IN_BITS;
	cell_identity >>= OAM_BYTE_SIZE_IN_BITS;
	p_byte_arr[offset] |= (cell_identity & 0xf) << OAM_NIBBLE_SIZE_IN_BITS;
	OAM_LOG(OAM, OAM_DETAILED, "EUTRAN CellIdentity[%d]: %d",
			offset, p_byte_arr[offset]);

	return OAM_SUCCESS;
}

/* SPR 11675 FIX START */
/****************************************************************************
 * Function Name  : oam_decode_bit_stream_into_eutran_cellidentity
 * Inputs         : byte array of cell identity coming from layers
 * Outputs        : user friendly cellIdentity
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function decodes the bit stream into cell Identity
 ****************************************************************************/
	oam_return_et
oam_decode_bit_stream_into_eutran_cellidentity(UInt8 *p_byte_arr,
		UInt32 *cell_identity)
{
	UInt8 offset = 0;

	if (!p_byte_arr || !cell_identity) {
		OAM_LOG(OAM, OAM_WARNING, "NULL argument(s) passed!! p_byte_arr=[%p], cell_identity=[%p]",
                p_byte_arr, cell_identity);
		return OAM_FAILURE;
	}

	*cell_identity = 0;

	*cell_identity |= p_byte_arr[offset];
	*cell_identity <<= OAM_BYTE_SIZE_IN_BITS;
	++offset;

	*cell_identity |= p_byte_arr[offset];
	*cell_identity <<= OAM_BYTE_SIZE_IN_BITS;
	++offset;

	*cell_identity |= p_byte_arr[offset];
	*cell_identity <<= OAM_NIBBLE_SIZE_IN_BITS;
	++offset;

	*cell_identity |= ((p_byte_arr[offset] & 0xF0) >> OAM_NIBBLE_SIZE_IN_BITS);

	return OAM_SUCCESS;
}
/* SPR 11675 FIX END */
/* SPR 22304 Fix Start */
    void oam_trace_dbg(
            unsigned int source,signed int log_level,
            const char* format, ...)
{
    /* Empty Definition */
}
/* SPR 22304 Fix End */

/* + SPR 17665 + */
/****************************************************************************
 * Function Name  : oam_tokenize_string_to_u8
 * Inputs         : p_str: string to be tokenized
 *                  p_delim: delimiter string
 *                  max_tokens: maximum number of tokens allowed
 * Outputs        : p_tokens: array of u8 type
 * Returns        : number of tokens successfully interpreted
 * Description    : This function is used to convert bytes to integer
 ****************************************************************************/
UInt32 oam_tokenize_string_to_u8(Char8* p_str, const Char8* p_delim,
		UInt32 max_tokens, UInt8 *p_tokens)
{
	OAM_FUN_ENTRY_TRACE();
	UInt32 token_count = OAM_ZERO;
	Char8 *p_temp = OAM_NULL;
	Char8 temp_str[MAX_PARAMETER_NAME_LENGTH] = {0};
	if(!p_str || !p_tokens || !max_tokens) {
		OAM_LOG(OAM, OAM_ERROR, "Invalid arguements passed p_str =%p, p_tokens =%p ,max_tokens =%u ",p_str, p_tokens,max_tokens);
		OAM_FUN_EXIT_TRACE();
		return token_count;
	}

	p_temp = strtok(p_str, p_delim);
	if(p_temp) {
		oam_trim_spaces(p_temp, temp_str);
		p_tokens[token_count] = atoi(temp_str);
		++token_count;
		while(max_tokens > token_count) {
			p_temp = strtok(OAM_NULL, p_delim);
			if(!p_temp) {
				break;
			}

			oam_trim_spaces(p_temp, temp_str);
			p_tokens[token_count] = atoi(temp_str);
			++token_count;
		}
	}

	OAM_FUN_EXIT_TRACE();
	return token_count;
}
/* - SPR 17665 - */
/*spr 22474 start*/
/****************************************************************************
 * * Function Name  : oam_get_debug_mem_stats_prints
 * * Description    : This function print mempool stats
 * * Inputs         : p_mem_stats : Input message response for mempool stats
 *                       : print_arr : string for print
 * * Outputs        : None
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 * ****************************************************************************/
UInt16
oam_get_debug_mem_stats_prints
(
 L2MemPoolStats* p_mem_stats,
 UInt8*** str_arr
)
{
    /* For printing the string of print_arr[size_x][size_y] */
    UInt16 num_strings = OAM_ZERO; /* Number of strings to be printed */
    UInt8 size_y = STR_LEN_255; /* Max Number of characters in each string */
    UInt32 poolIterator = OAM_ZERO;
    UInt32 total_buffers = OAM_ZERO, total_bytes = OAM_ZERO;
    oam_error_code_et error_code = NO_ERROR;

    /*  allocate storage for an array of pointer to char for max strings including header string */
    if (OAM_FAILURE == oam_mem_alloc(((NVARPOOL + OAM_THREE) * sizeof(UInt8 *)),(void *)str_arr, &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d", error_code);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    /* for each string, allocate storage for an array of chars to be printed */
    if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)(*str_arr+num_strings), &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d", error_code);
        OAM_2D_TEMP_ARR_MEM_FREE(str_arr, num_strings, error_code);
        OAM_FUN_EXIT_TRACE();
        return num_strings;
    }
    oam_memset(*(*str_arr+num_strings), OAM_ZERO, (size_y * sizeof(UInt8)));
    
      num_strings++; /* increment the number of strings */

    /* for each string, allocate storage for an array of chars to be printed */
    if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)(*str_arr+num_strings), &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d", error_code);
        OAM_2D_TEMP_ARR_MEM_FREE(str_arr, num_strings, error_code);
        OAM_FUN_EXIT_TRACE();
        return num_strings;
    }
    oam_memset(*(*str_arr+num_strings), OAM_ZERO, (size_y * sizeof(UInt8)));

    oam_sprintf((char *)(*(*str_arr+num_strings)),
              "%8s |%10s |%14s |%9s |%15s |%12s |",
              "Pool","Buf Count","Allocated Buf","Free Buf","Peak Alloc Buf","Total Bytes");

    num_strings++; /* increment the number of strings */

    /* for each string, allocate storage for an array of chars to be printed */
    if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)(*str_arr+num_strings), &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d", error_code);
        OAM_2D_TEMP_ARR_MEM_FREE(str_arr, num_strings, error_code);
        OAM_FUN_EXIT_TRACE();
        return num_strings;
    }
    oam_memset(*(*str_arr+num_strings), OAM_ZERO, (size_y * sizeof(UInt8)));

    oam_sprintf((char *)(*(*str_arr+num_strings)),
                  "------------------------------------------------------------------");

    num_strings++; /* increment the number of strings */
    /* iterate through the pools */
    for( poolIterator=0; poolIterator<NVARPOOL; poolIterator++)
    {
        /* Check if this is a valid entry */
        if (p_mem_stats->stats[poolIterator].bufSize)
        {
            /* for each string, allocate storage for an array of chars to be printed */
            if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void*)(*str_arr+num_strings), &error_code))
            {
                /* If memory allocation fails, flush all strings and return 0 strings  */
                OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d", error_code);
                OAM_2D_TEMP_ARR_MEM_FREE(str_arr, num_strings, error_code);
                OAM_FUN_EXIT_TRACE();
                return OAM_ZERO;
            }
            oam_memset(*(*str_arr+num_strings), OAM_ZERO, (size_y * sizeof(UInt8)));

            oam_sprintf((char *)*(*str_arr+num_strings),
              "%8d |%10d |%14d |%9d |%15d |%12d |",
              p_mem_stats->stats[poolIterator].bufSize,
              p_mem_stats->stats[poolIterator].numOfBuf,
              p_mem_stats->stats[poolIterator].numOfAllocBuf,
              p_mem_stats->stats[poolIterator].numOfBuf -
              p_mem_stats->stats[poolIterator].numOfAllocBuf,
              p_mem_stats->stats[poolIterator].numOfPeakBuf,
              p_mem_stats->stats[poolIterator].bufSize*
              p_mem_stats->stats[poolIterator].numOfAllocBuf);
            
            total_bytes = p_mem_stats->stats[poolIterator].bufSize * p_mem_stats->stats[poolIterator].numOfAllocBuf; 
            total_buffers += total_bytes;        
            num_strings++; /* increment the number of strings */
        }
        else
        {
            /* no need to continue as all entries are available now in strings to be printed */
            break;
        }
    }
    oam_sprintf((char *)*(*str_arr),"=====================MEMORY POOL STATS============================"
                        "\nTotal Bytes for Memory Pool Buffers = %d"
                        "\n-------------------------------------------------------------------",total_buffers);
    return num_strings;
}


/****************************************************************************
 * * Function Name  : oam_get_debug_msg_stats_prints
 * * Description    : This function print mempool stats
 * * Inputs         : p_msg_stats : Input message response for msgpool stats
 *                       : print_arr : string for print
 * * Outputs        : None
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 * ****************************************************************************/
UInt16
oam_get_debug_msg_stats_prints
(
 L2MsgPoolStats* p_msg_stats,
 UInt8*** str_arr
)
{
    /* For printing the string of print_arr[size_x][size_y] */
    UInt16 num_strings = OAM_ZERO; /* Number of strings to be printed */
    UInt8 size_y = STR_LEN_255; /* Max Number of characters in each string */
    UInt32 poolIterator = OAM_ZERO;
    oam_error_code_et error_code = NO_ERROR;
    UInt32 total_buffers = OAM_ZERO, total_bytes = OAM_ZERO;

    /* allocate storage for an array of pointer to char for max strings including header string */
    if (OAM_FAILURE == oam_mem_alloc(((NVARPOOL + OAM_SEVEN+OAM_SIX) * sizeof(UInt8 *)),(void *)str_arr, &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d", error_code);
        OAM_FUN_EXIT_TRACE();
        return OAM_FAILURE;
    }

    /* for each string, allocate storage for an array of chars to be printed */
    if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)(*str_arr+num_strings), &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d", error_code);
        OAM_2D_TEMP_ARR_MEM_FREE(str_arr, num_strings, error_code);
        OAM_FUN_EXIT_TRACE();
        return num_strings;
    }
    oam_memset(*(*str_arr+num_strings), OAM_ZERO, (size_y * sizeof(UInt8)));
    
      num_strings++; /* increment the number of strings */

    /* for each string, allocate storage for an array of chars to be printed */
    if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)(*str_arr+num_strings), &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d", error_code);
        OAM_2D_TEMP_ARR_MEM_FREE(str_arr, num_strings, error_code);
        OAM_FUN_EXIT_TRACE();
        return num_strings;
    }
    oam_memset(*(*str_arr+num_strings), OAM_ZERO, (size_y * sizeof(UInt8)));

    oam_sprintf((char *)(*(*str_arr+num_strings)),
                  "%8s |%10s |%14s |%9s |%15s |%12s |",
                  "Pool","Buf Count","Allocated Buf","Free Buf","Peak Alloc Buf","Total Bytes");

    num_strings++; /* increment the number of strings */

    /* for each string, allocate storage for an array of chars to be printed */
    if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)(*str_arr+num_strings), &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d", error_code);
        OAM_2D_TEMP_ARR_MEM_FREE(str_arr, num_strings, error_code);
        OAM_FUN_EXIT_TRACE();
        return num_strings;
    }
    oam_memset(*(*str_arr+num_strings), OAM_ZERO, (size_y * sizeof(UInt8)));

    oam_sprintf((char *)(*(*str_arr+num_strings)),
                  "------------------------------------------------------------------");

    num_strings++; /* increment the number of strings */
    /* iterate through the pools */
    for( poolIterator=0; poolIterator<NVARPOOL; poolIterator++)
    {
        /* Check if this is a valid entry */
        if (p_msg_stats->stats[poolIterator].bufSize)
        {
            /* for each string, allocate storage for an array of chars to be printed */
            if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void*)(*str_arr+num_strings), &error_code))
            {
                /* If memory allocation fails, flush all strings and return 0 strings  */
                OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d", error_code);
                OAM_2D_TEMP_ARR_MEM_FREE(str_arr, num_strings, error_code);
                OAM_FUN_EXIT_TRACE();
                return OAM_ZERO;
            }
            oam_memset(*(*str_arr+num_strings), OAM_ZERO, (size_y * sizeof(UInt8)));

            oam_sprintf((char *)*(*str_arr+num_strings),
              "%8d |%10d |%14d |%9d |%15d |%12d |",
              p_msg_stats->stats[poolIterator].bufSize,
              p_msg_stats->stats[poolIterator].numOfBuf,
              p_msg_stats->stats[poolIterator].numOfAllocBuf,
              p_msg_stats->stats[poolIterator].numOfBuf -
              p_msg_stats->stats[poolIterator].numOfAllocBuf,
              p_msg_stats->stats[poolIterator].numOfPeakBuf,
              p_msg_stats->stats[poolIterator].bufSize*
              p_msg_stats->stats[poolIterator].numOfAllocBuf);
            
            total_bytes = p_msg_stats->stats[poolIterator].bufSize * p_msg_stats->stats[poolIterator].numOfAllocBuf; 
            total_buffers += total_bytes;        
            num_strings++; /* increment the number of strings */
        }
        else
        {
            /* no need to continue as all entries are available now in strings to be printed */
            break;
        }
    }
    oam_sprintf((char *)*(*str_arr),"=====================MESSAGE POOL STATS============================"
                        "\nTotal Bytes for Message Pool Buffers = %d"
                        "\n-------------------------------------------------------------------",total_buffers);
    /*********************************************QPCTL POOLS************************************************/
    /* for each string, allocate storage for an array of chars to be printed */
    if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)(*str_arr+num_strings), &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d", error_code);
        OAM_2D_TEMP_ARR_MEM_FREE(str_arr, num_strings, error_code);
        OAM_FUN_EXIT_TRACE();
        return num_strings;
    }
    oam_memset(*(*str_arr+num_strings), OAM_ZERO, (size_y * sizeof(UInt8)));

    oam_sprintf((char *)(*(*str_arr+num_strings)),
                  "------------------------------------------------------------------");
    num_strings++;

    /* for each string, allocate storage for an array of chars to be printed */
    if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)(*str_arr+num_strings), &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d", error_code);
        OAM_2D_TEMP_ARR_MEM_FREE(str_arr, num_strings, error_code);
        OAM_FUN_EXIT_TRACE();
        return num_strings;
    }
    oam_memset(*(*str_arr+num_strings), OAM_ZERO, (size_y * sizeof(UInt8)));
    oam_sprintf((char *)(*(*str_arr+num_strings)),
                  "=====================QPCTL POOL STATS============================");
    
     num_strings++; /* increment the number of strings */

    /* for each string, allocate storage for an array of chars to be printed */
    if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)(*str_arr+num_strings), &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d", error_code);
        OAM_2D_TEMP_ARR_MEM_FREE(str_arr, num_strings, error_code);
        OAM_FUN_EXIT_TRACE();
        return num_strings;
    }
    oam_memset(*(*str_arr+num_strings), OAM_ZERO, (size_y * sizeof(UInt8)));

    oam_sprintf((char *)(*(*str_arr+num_strings)),
                  "%8s |%10s |%14s |%9s |%15s |%12s |",
                  "Pool","Buf Count","Allocated Buf","Free Buf","Peak Alloc Buf","Total Bytes");

    num_strings++; /* increment the number of strings */

    /* for each string, allocate storage for an array of chars to be printed */
    if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)(*str_arr+num_strings), &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d", error_code);
        OAM_2D_TEMP_ARR_MEM_FREE(str_arr, num_strings, error_code);
        OAM_FUN_EXIT_TRACE();
        return num_strings;
    }
    oam_memset(*(*str_arr+num_strings), OAM_ZERO, (size_y * sizeof(UInt8)));

    oam_sprintf((char *)(*(*str_arr+num_strings)),
                  "------------------------------------------------------------------");
    num_strings++;

    /* for each string, allocate storage for an array of chars to be printed */
    if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)(*str_arr+num_strings), &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d", error_code);
        OAM_2D_TEMP_ARR_MEM_FREE(str_arr, num_strings, error_code);
        OAM_FUN_EXIT_TRACE();
        return num_strings;
    }
    oam_memset(*(*str_arr+num_strings), OAM_ZERO, (size_y * sizeof(UInt8)));

    oam_sprintf((char *)*(*str_arr+num_strings),
            "%8d |%10d |%14d |%9d |%15d |%12d |",
            p_msg_stats->qpctlStats.bufSize,
            p_msg_stats->qpctlStats.numOfBuf,
            p_msg_stats->qpctlStats.numOfAllocBuf,
            p_msg_stats->qpctlStats.numOfBuf -
            p_msg_stats->qpctlStats.numOfAllocBuf,
            p_msg_stats->qpctlStats.numOfPeakBuf,
            p_msg_stats->qpctlStats.bufSize*
            p_msg_stats->qpctlStats.numOfAllocBuf);

    num_strings++;

    if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)(*str_arr+num_strings), &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d", error_code);
        OAM_2D_TEMP_ARR_MEM_FREE(str_arr, num_strings, error_code);
        OAM_FUN_EXIT_TRACE();
        return num_strings;
    }
    oam_memset(*(*str_arr+num_strings), OAM_ZERO, (size_y * sizeof(UInt8)));

    oam_sprintf((char *)(*(*str_arr+num_strings)),
                  "------------------------------------------------------------------");
    num_strings++;

    /*********************************************QMESG POOLS************************************************/
    /* for each string, allocate storage for an array of chars to be printed */
    if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)(*str_arr+num_strings), &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d", error_code);
        OAM_2D_TEMP_ARR_MEM_FREE(str_arr, num_strings, error_code);
        OAM_FUN_EXIT_TRACE();
        return num_strings;
    }
    oam_memset(*(*str_arr+num_strings), OAM_ZERO, (size_y * sizeof(UInt8)));
    oam_sprintf((char *)(*(*str_arr+num_strings)),
                  "=====================QMESG POOL STATS============================");
    
    num_strings++; /* increment the number of strings */

    /* for each string, allocate storage for an array of chars to be printed */
    if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)(*str_arr+num_strings), &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d", error_code);
        OAM_2D_TEMP_ARR_MEM_FREE(str_arr, num_strings, error_code);
        OAM_FUN_EXIT_TRACE();
        return num_strings;
    }
    oam_memset(*(*str_arr+num_strings), OAM_ZERO, (size_y * sizeof(UInt8)));

    oam_sprintf((char *)(*(*str_arr+num_strings)),
                  "%8s |%10s |%14s |%9s |%15s |%12s |",
                  "Pool","Buf Count","Allocated Buf","Free Buf","Peak Alloc Buf","Total Bytes");

    num_strings++; /* increment the number of strings */

    /* for each string, allocate storage for an array of chars to be printed */
    if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)(*str_arr+num_strings), &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d", error_code);
        OAM_2D_TEMP_ARR_MEM_FREE(str_arr, num_strings, error_code);
        OAM_FUN_EXIT_TRACE();
        return num_strings;
    }
    oam_memset(*(*str_arr+num_strings), OAM_ZERO, (size_y * sizeof(UInt8)));

    oam_sprintf((char *)(*(*str_arr+num_strings)),
                  "------------------------------------------------------------------");
    num_strings++;

    /* for each string, allocate storage for an array of chars to be printed */
    if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)(*str_arr+num_strings), &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d", error_code);
        OAM_2D_TEMP_ARR_MEM_FREE(str_arr, num_strings, error_code);
        OAM_FUN_EXIT_TRACE();
        return num_strings;
    }
    oam_memset(*(*str_arr+num_strings), OAM_ZERO, (size_y * sizeof(UInt8)));

    oam_sprintf((char *)*(*str_arr+num_strings),
            "%8d |%10d |%14d |%9d |%15d |%12d |",
            p_msg_stats->qmsgStats.bufSize,
            p_msg_stats->qmsgStats.numOfBuf,
            p_msg_stats->qmsgStats.numOfAllocBuf,
            p_msg_stats->qmsgStats.numOfBuf -
            p_msg_stats->qmsgStats.numOfAllocBuf,
            p_msg_stats->qmsgStats.numOfPeakBuf,
            p_msg_stats->qmsgStats.bufSize*
            p_msg_stats->qmsgStats.numOfAllocBuf);


    num_strings++;

    return num_strings;
}
/****************************************************************************
 * * Function Name  : oam_get_l3_debug_mem_stats_prints
 * * Description    : This function print mempool stats
 * * Inputs         : p_mem_stats : Input message response for mempool stats
 *                       : print_arr : string for print
 * * Outputs        : None
 * * Returns        : OAM_SUCCESS/OAM_FAILURE
 * ****************************************************************************/
UInt16
oam_get_l3_debug_mem_stats_prints
(
 rrc_MemPoolStats* p_mem_stats,
 UInt8*** str_arr
)
{
    /* For printing the string of print_arr[size_x][size_y] */
    UInt16 num_strings = OAM_ZERO; /* Number of strings to be printed */
    UInt8 size_y = STR_LEN_255; /* Max Number of characters in each string */
    UInt32 poolIterator = OAM_ZERO;
    oam_error_code_et error_code = NO_ERROR;
    UInt32 total_buffers = OAM_ZERO, total_bytes = OAM_ZERO;
    /*  allocate storage for an array of pointer to char for max strings including header string */
    if (OAM_FAILURE == oam_mem_alloc(((NVARPOOL + OAM_THREE) * sizeof(UInt8 *)),(void *)str_arr, &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d", error_code);
        OAM_FUN_EXIT_TRACE();
        return OAM_ZERO;
        //return num_strings;
    }

    /* for each string, allocate storage for an array of chars to be printed */
    if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)(*str_arr+num_strings), &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d", error_code);
        OAM_2D_TEMP_ARR_MEM_FREE(str_arr, num_strings, error_code);
        OAM_FUN_EXIT_TRACE();
        return OAM_ZERO;
        //return num_strings;
    }
    oam_memset(*(*str_arr+num_strings), OAM_ZERO, (size_y * sizeof(UInt8)));
    
      num_strings++; /* increment the number of strings */

    /* for each string, allocate storage for an array of chars to be printed */
    if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)(*str_arr+num_strings), &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d", error_code);
        OAM_2D_TEMP_ARR_MEM_FREE(str_arr, num_strings, error_code);
        OAM_FUN_EXIT_TRACE();
        return OAM_ZERO;
        //return num_strings;
    }
    oam_memset(*(*str_arr+num_strings), OAM_ZERO, (size_y * sizeof(UInt8)));

    oam_sprintf((char *)(*(*str_arr+num_strings)),
              "%8s |%10s |%14s |%9s |%15s |%12s |",
              "Pool","Buf Count","Allocated Buf","Free Buf","Peak Alloc Buf","Total Bytes");

    num_strings++; /* increment the number of strings */

    /* for each string, allocate storage for an array of chars to be printed */
    if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void *)(*str_arr+num_strings), &error_code))
    {
        OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d", error_code);
        OAM_2D_TEMP_ARR_MEM_FREE(str_arr, num_strings, error_code);
        OAM_FUN_EXIT_TRACE();
        return OAM_ZERO;
        //return num_strings;
    }
    oam_memset(*(*str_arr+num_strings), OAM_ZERO, (size_y * sizeof(UInt8)));

    oam_sprintf((char *)(*(*str_arr+num_strings)),
                  "------------------------------------------------------------------");

    num_strings++; /* increment the number of strings */
    /* iterate through the pools */
    for( poolIterator=0; poolIterator<NVARPOOL; poolIterator++)
    {
        /* Check if this is a valid entry */
        if (p_mem_stats->stats[poolIterator].bufSize)
        {
            /* for each string, allocate storage for an array of chars to be printed */
            if(OAM_FAILURE == oam_mem_alloc((size_y * sizeof(UInt8)),(void*)(*str_arr+num_strings), &error_code))
            {
                /* If memory allocation fails, flush all strings and return 0 strings  */
                OAM_LOG(OAM,OAM_ERROR,"Error in Memory Allocation with error code %d", error_code);
                OAM_2D_TEMP_ARR_MEM_FREE(str_arr, num_strings, error_code);
                OAM_FUN_EXIT_TRACE();
                return OAM_ZERO;
            }
            oam_memset(*(*str_arr+num_strings), OAM_ZERO, (size_y * sizeof(UInt8)));
            oam_sprintf((char *)*(*str_arr+num_strings),
              "%8d |%10d |%14d |%9d |%15d |%12d |",
              p_mem_stats->stats[poolIterator].bufSize,
              p_mem_stats->stats[poolIterator].numOfBuf,
              p_mem_stats->stats[poolIterator].numOfAllocBuf,
              p_mem_stats->stats[poolIterator].numOfBuf -
              p_mem_stats->stats[poolIterator].numOfAllocBuf,
              p_mem_stats->stats[poolIterator].numOfPeakBuf,
              p_mem_stats->stats[poolIterator].bufSize*
              p_mem_stats->stats[poolIterator].numOfAllocBuf);
            total_bytes = p_mem_stats->stats[poolIterator].bufSize * p_mem_stats->stats[poolIterator].numOfAllocBuf; 
            total_buffers += total_bytes;        
            num_strings++; /* increment the number of strings */
        }
        else
        {
            /* no need to continue as all entries are available now in strings to be printed */
            break;
        }
    }

    oam_sprintf((char *)*(*str_arr),"=====================MESSAGE/MEMORY POOL STATS============================"
                        "\nTotal Bytes for Memory/Message Pool Buffers = %d"
                        "\n-------------------------------------------------------------------",total_buffers);
    return num_strings;
}


/*spr 22474 end*/
