/* vim:ts=4:et:ai:ci:sw=4
*/
/*******************************************************************************
 * File Name       : oam_sdm_utils.c
 *
 * Description     : This file contains utility based functions. 
 * Revision History:
 *
 * Date         Author                 Reference            Comments
 * -----        ------                 ---------           --------
 * January,2013   Vinod Vyas     OAM Design Document   Initial Version
 *
 * Copyright (c) 2013, Aricent Inc. All Rights Reserved
 *******************************************************************************/

/*******************************************************************************
 * Includes
 *** ***************************************************************************/
#include <oam_defines.h>
#include <oam_sdm_main.h>
#include <oam_mgmt_intf_api.h>
/* SPR 14921 Fix Start*/
#include <sys/wait.h>
/* SPR 14921 Fix End */

/*******************************************************************************
 *  Global Declaration
 *******************************************************************************/

extern const QSYSOP my_os;
extern const QWAIT my_waitstruct;
extern const QSHELL my_shell;
extern UInt8 g_oam_process_number;
extern struct _oam_sdm_sim_table_t g_oam_sdm_t;
UInt32 g_sdm_poll_counter = OAM_SDM_ZERO;
extern SInt32 g_no_of_arguments;
extern Char8 **g_list_of_init_arguments;
extern UInt32 backup_counter;
extern UInt32 g_oam_sdm_event_ongoing;
extern UInt16 g_flag_for_oam_active_ind;
extern UInt8 g_bin_backup_array[MAX_NO_OF_BACKUP][MAX_BACKUP_STR_LENGTH];
extern UInt8 g_file_name[MAX_BACKUP_STR_LENGTH];

/****************************************************************************
  Private Types
 ****************************************************************************/


/******************************************************************************
 *   FUNCTION NAME  : Alloc
 *   RETURNS        : memory allocated
 *   INPUTS         : module :Module
 *                    attrsize :attribute size
 *   DESCRIPTION    : function to allocate message allocate byte size.
 ******************************************************************************/
static void *
Alloc (
		QMODULE module, 
		unsigned int attrsize
      )
{
    /* SPR 17777 fix */
    OAM_SDM_LOG (SDM, OAM_SDM_DETAILED, " unsued parameter %p ", module);

	return qvMsgAllocBySize (attrsize);
}



LOCAL const unsigned long sdmDestList[] = {
	OAM_MODULE_ID,
	OAM_SDM_ZERO
};


const QMANIFEST sdm_entity = {
	"OAM_SDM_MODULE_ID",
	OAM_SDM_MODULE_ID,
    /* SPR 21889 start */
	{OAM_SDM_ZERO, oam_sdm_init},
    /* SPR 21889 end */
	{(S32 (*)(void *, void *))oam_sdm_process_msg,(void (*)(QTIMER, void *, void *)) oam_sdm_process_timer_expiry},
	{OAM_SDM_ZERO, OAM_SDM_ZERO,(void *(*)(QMODULE, U32))Alloc},
	sdmDestList
};

/* coverity 35587 35588 35589 */



/******************************************************************************
 *   FUNCTION NAME  : oam_sdm_trace_message
 *   RETURNS        : None
 *   INPUTS         : log_level :Log level
 *                    pFile :Pointer to file
 *                    pFun : Pointer to function
 *                    Line : line
 *                    format :format.
 *   DESCRIPTION    : function to print trace message.
 ******************************************************************************/
	void
oam_sdm_trace_message (SInt32 log_level,	/* log level of this message */
		SInt8 * pFile, SInt8 * pFun, UInt16 Line, SInt8 * format,	/* format string like in printf */
		...	/* optional parameters */
		)
{
	U8 tmp_fmt[OAM_SDM_TRACE_FORMAT_SIZE];
	U8 tmp_fmt1[OAM_SDM_TRACE_FORMAT_SIZE];
	oam_memset (tmp_fmt, OAM_SDM_ZERO, sizeof (tmp_fmt));
	oam_memset (tmp_fmt1, OAM_SDM_ZERO, sizeof (tmp_fmt1));


	va_list arglist;
	OAM_SDM_VA_START (arglist, format);

	if (log_level == OAM_SDM_DETAILED)
	{
		OAM_SDM_VSPRINTF ((char *) tmp_fmt1, (const char *) format, arglist);
		oam_sdm_sprintf ((char *) tmp_fmt,
				"\n[OAM_SDM_DETAILED] %s [API:%s] [From FILE:%s] [Line No.:%d]\n",
				tmp_fmt1, pFun, pFile, Line);
		if (log_level >= prov_req.log_level)
		{
			oam_sdm_fprintf (prov_req.p_out, "%s", (const char *) tmp_fmt);
		}
		OAM_SDM_VA_END (arglist);
		return;
	}

	/* Start Fix for actual timestamp in logs */
	time_t secs_time;
	UInt8 time_string[OAM_SDM_TRACE_TEMP_STR_SIZE] = { OAM_SDM_ZERO };
	struct tm *lt, result;

	/* Current time as seconds elapsed since the Epoch. */
	secs_time = oam_sdm_time ( OAM_SDM_NULL );
	lt = oam_sdm_localtime_r (&secs_time, &result);
	/* Convert to local time format. */
	if(OAM_SDM_NULL != lt)
	{
		oam_sdm_sprintf ((char *) time_string, "%d %d %d %d:%d:%d",
				lt->tm_mday, lt->tm_mon + OAM_SDM_ONE, lt->tm_year + OAM_SDM_TRACE_YEAR_FMT,
				lt->tm_hour, lt->tm_min, lt->tm_sec);
		/* End Fix for actual timestamp in logs */
	}
	switch (log_level)
	{
		case OAM_SDM_INFO:
			{
				OAM_SDM_VSPRINTF ((char *) tmp_fmt1, (const char *) format, arglist);
				oam_sdm_sprintf ((char *) tmp_fmt,
						"\n[%s] [OAM_SDM_INFO] %s [API:%s] [From FILE:%s] [Line No.:%d]\n",
						time_string, tmp_fmt1, pFun, pFile, Line);
				break;
			}
		case OAM_SDM_WARNING:
			{
				OAM_SDM_VSPRINTF ((char *) tmp_fmt1, (const char *) format, arglist);
				oam_sdm_sprintf ((char *) tmp_fmt,
						"\n[%s] [OAM_SDM_WARNING] %s [API:%s] [From FILE:%s] [Line No.:%d]\n",
						time_string, tmp_fmt1, pFun, pFile, Line);
				break;
			}
		case OAM_SDM_ERROR:
			{
				OAM_SDM_VSPRINTF ((char *) tmp_fmt1, (const char *) format, arglist);
				oam_sdm_sprintf ((char *) tmp_fmt,
						"\n[%s] [OAM_SDM_ERROR] %s [API:%s] [From FILE:%s] [Line No.:%d]\n",
						time_string, tmp_fmt1, pFun, pFile, Line);
				break;
			}
		case OAM_SDM_CRITICAL:
			{
				OAM_SDM_VSPRINTF ((char *) tmp_fmt1, (const char *) format, arglist);
				oam_sdm_sprintf ((char *) tmp_fmt,
						"\n[%s] [OAM_SDM_CRITICAL] %s [API:%s] [From FILE:%s] [Line No.:%d]\n",
						time_string, tmp_fmt1, pFun, pFile, Line);
				break;
			}
		default:
			{
				oam_sdm_fprintf (prov_req.p_out,"\n Invalid Log level[%d]", log_level);
			}
	}

	if (log_level >= prov_req.log_level)
	{
		oam_sdm_fprintf (prov_req.p_out, "%s",(const char *) tmp_fmt);/* Coverity FIX 36002 */
	}

	OAM_SDM_VA_END (arglist);
}




/******************************************************************************
 *   FUNCTION NAME  : oam_sdm_single_thd_init
 *   DESCRIPTION    : This function creates interface thd and also responsible 
 *                    for initializing the cspl library
 *  INPUT : NONE 
 *  RETURNS         : OAM_SDM_FAILURE/OAM_SDM_SUCCESS
 ******************************************************************************/

/* + SPR 17439 */
	oam_sdm_return_et
oam_sdm_single_thd_init (void)
/* - SPR 17439 */
{

	void *listener = OAM_SDM_NULL;

	OAM_SDM_FUN_ENTRY_TRACE ();

	qvSimpleInit (&my_os);


	listener = qvOpen (&my_shell, OAM_SDM_NULL);	/* open communication channel ie socket  */

	if (listener == OAM_SDM_NULL)
	{
		OAM_SDM_LOG (SDM, OAM_SDM_ERROR,
				"oam_single_thd_init:Unable to create Listener");
		return OAM_SDM_FAILURE;
	}


	qvRegister (OAM_SDM_ZERO, OAM_SDM_ZERO, OAM_SDM_ZERO, &sdm_entity, OAM_SDM_ZERO);	/* registering module */

	qvSetDestination (OAM_MODULE_ID, &my_shell, listener, OAM_SDM_ZERO);	/* setting destination */


	qvRun (&my_shell, listener, &my_waitstruct, OAM_SDM_ZERO);


	OAM_SDM_FUN_EXIT_TRACE ();
	return OAM_SDM_SUCCESS;
}

/*****************************************************************************
 *   FUNCTION NAME  : oam_sdm_process_msg
 *   DESCRIPTION    : This function handle and process messages received 
 *                    from OAM       
 *   INPUT          : p_api_buf :Pointer to API Buffer
 *   RETURNS        : OAM_SDM_FAILURE/OAM_SDM_SUCCESS
 ******************************************************************************/
oam_sdm_return_et
oam_sdm_process_msg (
		void *p_api_buf
		)
{
	OAM_SDM_FUN_ENTRY_TRACE ();
	UInt16 api_id = OAM_SDM_ZERO;
	UInt16 src_id = OAM_SDM_ZERO;
	/* coverity 35998 */
	oam_sdm_return_et ret_val = OAM_SDM_SUCCESS;

	/* Get the src ID */
	src_id = oam_sdm_get_word_from_header ((UInt8 *) (p_api_buf) + OAM_SDM_TWO);

	/* Get the api ID */
	api_id = oam_sdm_get_word_from_header ((UInt8 *) (p_api_buf) + OAM_SDM_SIX);

	if (src_id == OAM_MODULE_ID)
	{
		switch (api_id)
		{
			case OAM_SDM_REQUEST_UPGRADE_START:
				if (OAM_SDM_FAILURE == oam_sdm_handle_upgarde_req(p_api_buf))
				{
					OAM_SDM_LOG (SDM, OAM_SDM_WARNING,
							"Error in handling upgrade request message from OAM");
					OAM_SDM_FUN_EXIT_TRACE ();
					return OAM_SDM_FAILURE;
				}
				break;
			case OAM_SDM_POLLING_RESP:
				OAM_SDM_LOG (SDM, OAM_SDM_DETAILED, "Received POll responce from OAM");
				oam_sdm_handle_polling_response (p_api_buf);
				break;
			case OAM_SDM_ACTIVE_IND:
				OAM_SDM_LOG (SDM, OAM_SDM_DETAILED,"Received Activation Indication from OAM");
				oam_sdm_handle_active_ind();
				break;
			case OAM_SDM_ACK_UPGRADE_START:
			case OAM_SDM_CONTROL_COMMAND_REQ:
				if (OAM_SDM_FAILURE == oam_sdm_start_upgrade (p_api_buf))
				{
					OAM_SDM_LOG (SDM, OAM_SDM_WARNING, "Error in Upgrading Software");
					OAM_SDM_FUN_EXIT_TRACE ();
					return OAM_SDM_FAILURE;
				}
				break;
			default:
				OAM_SDM_LOG (SDM, OAM_SDM_WARNING, "Invalid API[api_id=%d] Received for SDM", api_id);
				break;
		}

	}
	OAM_SDM_FUN_EXIT_TRACE ();
	return ret_val;
}

/****************************************************************************
 * Function Name  : oam_sdm_create_time_stamp
 * Inputs         : none 
 * Outputs        : none
 * Returns        : OAM_SDM_SUCCESS/OAM_SDM_FAILURE
 * Description    : This function Handles the time stamp
 ******************************************************************************/
	oam_sdm_return_et
oam_sdm_create_time_stamp (void)
{
	OAM_SDM_FUN_ENTRY_TRACE ();
	const struct tm *tm_ptr = OAM_SDM_NULL;
	size_t len;
	time_t now;
	now = oam_sdm_time (OAM_SDM_NULL);
	tm_ptr = oam_sdm_localtime (&now);
	if (OAM_SDM_NULL == tm_ptr)
	{
		OAM_SDM_LOG (SDM, OAM_SDM_WARNING, "tm_ptr is NULL");
		OAM_SDM_FUN_EXIT_TRACE ();
		return OAM_SDM_FAILURE;
	}
	else
	{
		len =
			strftime ((char *) time_buffer, TIME_SIZE, "%d_%m_%Y_%I-%M-%S",
					tm_ptr);
	}
	if (len == OAM_SDM_ZERO)
	{
		OAM_SDM_FUN_EXIT_TRACE ();
		return OAM_SDM_FAILURE;
	}
	else
	{
		OAM_SDM_FUN_EXIT_TRACE ();
		return OAM_SDM_SUCCESS;
	}
}

/****************************************************************************
 * Function Name  : oam_sdm_get_word_from_header
 * Inputs         : p_header :Pointer to header
 * Outputs        : none
 * Returns        : UInt16 value
 * Description    : This function finds the value of 2 bytes
 *****************************************************************************/
oam_sdm_return_et
oam_sdm_get_word_from_header (
		UInt8 * p_header
		)
{
	UInt16 value = OAM_SDM_ZERO;

	value = p_header[OAM_SDM_ZERO];
	value = value << OAM_SDM_EIGHT;
	value = value | p_header[OAM_SDM_ONE];

	/* coverity 35999 */
	return (oam_sdm_return_et)value;
}

/****************************************************************************
 * Function Name  : oam_sdm_construct_mgmt_api_hdr
 * Inputs         : p_msg :Pointer to message
 *                  trans_id :transaction identifier
 *                  src_mod_id :source module id
 *                  dst_mod_id :destination module id
 *                  api_id :API ID
 *                  api_buf_size :API Buffer size
 *                  api_version :API Version
 *                  resvd :reserved
 * Outputs        : none
 * Returns        : none
 * Description    : This function construct management api header.
 *****************************************************************************/

void
oam_sdm_construct_mgmt_api_hdr (
		UInt8 * p_msg,
		UInt16 trans_id,
		UInt16 src_mod_id,
		UInt16 dst_mod_id,
		UInt16 api_id,
		UInt16 api_buf_size,
        /* SPR 17777 fix code removed */
		UInt8 resvd
		)
{

	OAM_SDM_FUN_ENTRY_TRACE ();
	if (OAM_SDM_NULL == p_msg)
	{
		OAM_SDM_LOG (SDM, OAM_SDM_WARNING,
				"sdm_construct_mgmt_api_hdr: null pointer passed");
		return;
	}

	*(p_msg + OAM_SDM_ZERO) = (trans_id & OAM_SDM_UPPER_EIGHT_BITS_SET) >> OAM_SDM_EIGHT;
	*(p_msg + OAM_SDM_ONE) = trans_id & OAM_SDM_ALL_BIT_SET;

	*(p_msg + OAM_SDM_TWO) = (src_mod_id & OAM_SDM_UPPER_EIGHT_BITS_SET) >> OAM_SDM_EIGHT;
	*(p_msg + OAM_SDM_THREE) = src_mod_id & OAM_SDM_ALL_BIT_SET;

	*(p_msg + OAM_SDM_FOUR) = (dst_mod_id & OAM_SDM_UPPER_EIGHT_BITS_SET) >> OAM_SDM_EIGHT;
	*(p_msg + OAM_SDM_FIVE) = dst_mod_id & OAM_SDM_ALL_BIT_SET;

	*(p_msg + OAM_SDM_SIX) = (api_id & OAM_SDM_UPPER_EIGHT_BITS_SET) >> OAM_SDM_EIGHT;
	*(p_msg + OAM_SDM_SEVEN) = api_id & OAM_SDM_ALL_BIT_SET;

	*(p_msg + OAM_SDM_EIGHT) = (api_buf_size & OAM_SDM_UPPER_EIGHT_BITS_SET) >> OAM_SDM_EIGHT;
	*(p_msg + OAM_SDM_NINE) = api_buf_size & OAM_SDM_ALL_BIT_SET;

	/* Hardcoding API Version as 1 */
	*(p_msg + OAM_SDM_TEN) = OAM_SDM_API_VERSION;	/* api_version; */
	*(p_msg + OAM_SDM_ELEVEN) = resvd;
	OAM_SDM_FUN_EXIT_TRACE ();
}

/****************************************************************************
 * Function Name  : oam_sdm_encode_get_params_resp_ctrl_blk
 * Inputs         : p_ctrl_blk :Pointer to control block
 *                  resp_status :response status
 *                  err_code :Error code
 *                  resvd1 :reserved 1
 *                  resvd2 :reserved 2
 *                  err_Str_len :Error string length
 *                  err_str :Error String
 * Outputs        : none
 * Returns        : none
 * Description    : This function encode parameter response.
 *****************************************************************************/
void
oam_sdm_encode_get_params_resp_ctrl_blk(
		char *p_ctrl_blk,
		char resp_status,
		UInt16 err_code,
		UInt8 resvd1, 
		UInt32 resvd2, 
		UInt8 err_str_len, 
		char *err_str
		)
{
	OAM_SDM_FUN_ENTRY_TRACE ();
	UInt32 curr_pos = OAM_SDM_ZERO;
	*(p_ctrl_blk + curr_pos++) = resp_status;	/* packing response status */
	/* packing error code      */
	*(p_ctrl_blk + curr_pos++) = (err_code & OAM_SDM_UPPER_EIGHT_BITS_SET) >> OAM_SDM_EIGHT;
	*(p_ctrl_blk + curr_pos++) = err_code & OAM_SDM_ALL_BIT_SET;
	/*packing reserved field */
	*(p_ctrl_blk + curr_pos++) = resvd1;
	*(p_ctrl_blk + curr_pos++) = (resvd2 & 0xff0000) >> OAM_SDM_SIXTEEN;
	*(p_ctrl_blk + curr_pos++) = (resvd2 & OAM_SDM_UPPER_EIGHT_BITS_SET) >> OAM_SDM_EIGHT;
	*(p_ctrl_blk + curr_pos++) = resvd2 & OAM_SDM_ALL_BIT_SET;
	/*packing error string length */
	*(p_ctrl_blk + curr_pos++) = err_str_len;

	/* Encode error string only when there is failure */
	if (err_str_len != OAM_SDM_ZERO)
		oam_sdm_memcpy (p_ctrl_blk + curr_pos, err_str, err_str_len);
	OAM_SDM_FUN_EXIT_TRACE ();
}

/****************************************************************************
 * Function Name  : oam_sdm_construct_api_header
 * Inputs         : p_header :Pointer to CSPL header
 *                  version_id :API Version ID
 *                  src_module_id :Source Module ID
 *                  dst_module_id :Destination module Id
 *                  api_id :API Id
 *                  api_buf_size : API buffer size
 * Outputs        : none
 * Returns        : none
 * Description    : This function constructs api header from given parameters.
 ****************************************************************************/
void
oam_sdm_construct_api_header (
		UInt8 * p_header,	/* CSPL header */
		UInt8 version_id,	/* API version Id */
		UInt16 src_module_id,	/* Source module Id */
		UInt16 dst_module_id,	/* Destination module Id */
		UInt16 api_id,	/* API Id */
		UInt16 api_buf_size	/* API buffer size */
		)
{

	OAM_SDM_FUN_ENTRY_TRACE ();

	/* version */
	p_header[OAM_SDM_ZERO] = version_id;

	/* from (HI) */
	p_header[OAM_SDM_ONE] = (src_module_id & OAM_SDM_UPPER_EIGHT_BITS_SET) >> OAM_SDM_EIGHT;

	/* from (LOW) */
	p_header[OAM_SDM_TWO] = src_module_id & OAM_SDM_LOWER_EIGHT_BITS_SET;

	/* to (HI) */
	p_header[OAM_SDM_THREE] = (dst_module_id & OAM_SDM_UPPER_EIGHT_BITS_SET) >> OAM_SDM_EIGHT;

	/* to (LOW) */
	p_header[OAM_SDM_FOUR] = dst_module_id & OAM_SDM_LOWER_EIGHT_BITS_SET;

	/* api id (HI) */
	p_header[OAM_SDM_FIVE] = (api_id & OAM_SDM_UPPER_EIGHT_BITS_SET) >> OAM_SDM_EIGHT;

	/* api id (LOW) */
	p_header[OAM_SDM_SIX] = api_id & OAM_SDM_LOWER_EIGHT_BITS_SET;

	/* api size (HI) */
	p_header[OAM_SDM_SEVEN] = (api_buf_size & OAM_SDM_UPPER_EIGHT_BITS_SET) >> OAM_SDM_EIGHT;

	/* api size (LOW) */
	p_header[OAM_SDM_EIGHT] = api_buf_size & OAM_SDM_LOWER_EIGHT_BITS_SET;

	/* spare bytes */
	p_header[OAM_SDM_NINE]     = OAM_SDM_ALL_BIT_ZERO;
	p_header[OAM_SDM_TEN]      = OAM_SDM_ALL_BIT_ZERO;
	p_header[OAM_SDM_ELEVEN]   = OAM_SDM_ALL_BIT_ZERO;
	p_header[OAM_SDM_TWELVE]   = OAM_SDM_ALL_BIT_ZERO;
	p_header[OAM_SDM_THIRTEEN] = OAM_SDM_ALL_BIT_ZERO;
	p_header[OAM_SDM_FOURTEEN] = OAM_SDM_ALL_BIT_ZERO;
	p_header[OAM_SDM_FIFTEEN]  = OAM_SDM_ALL_BIT_ZERO;

	OAM_SDM_FUN_EXIT_TRACE ();
}

/****************************************************************************
 * Function Name  : oam_sdm_polling_message
 * Description    : This is used for monitoring heart beat of OAM by SDM.
 * Inputs         : p_error_code :Pointer to error code
 * Outputs        : none
 * Returns        : Pointer to void
 *****************************************************************************/
void *
oam_sdm_create_polling_message (
        /* SPR 17777 fix */
		void
		)
{
	UInt8 *p_msg = OAM_SDM_NULL;
	UInt8 *p_resp_msg = OAM_SDM_NULL;
	UInt16 msg_len = OAM_SDM_ZERO;
	SInt32 transaction_id = OAM_SDM_ZERO;

	OAM_SDM_FUN_ENTRY_TRACE ();

	msg_len = OAM_SDM_INTF_HEADER_SIZE + OAM_CSPL_HDR_SIZE;
	msg_len += sizeof(UInt32); 

	p_msg = qvMsgAlloc (OAM_SDM_NULL, OAM_SDM_ZERO, OAM_SDM_ZERO, msg_len);
	if (p_msg == OAM_SDM_NULL)
	{
		OAM_SDM_LOG(SDM,OAM_SDM_ERROR,"Memory allocation to"
				" p_msg failed");
		return OAM_SDM_NULL;
	}

	p_resp_msg = p_msg;

	oam_sdm_construct_api_header (p_resp_msg, OAM_SDM_VERSION,
			OAM_SDM_MODULE_ID, OAM_MODULE_ID,
			SDM_OAM_POLLING_REQ, msg_len);

	p_resp_msg = p_resp_msg + OAM_CSPL_HDR_SIZE;
    /* SPR 17777 fix */
	oam_sdm_construct_mgmt_api_hdr (p_resp_msg, transaction_id,
			OAM_SDM_MODULE_ID, OAM_MODULE_ID,
			SDM_OAM_POLLING_REQ, msg_len,
			OAM_SDM_ZERO);

	OAM_SDM_FUN_EXIT_TRACE ();
	return p_msg;
}

/* SPR 21889 start */
/*************************************************************************
 * Function Name  : oam_sdm_init
 * Description    : This is used for SDM initialization
 * Inputs         : p_data
 * Outputs        : none
 * Returns        : Pointer to context data allocated for the module
 ******************************************************************************/
void* oam_sdm_init(	
		void *p_data)
{
    UInt16 error_code;

    /* Creating table with only one entry for OAM process rest process when
     * started by OAM will be known to SDM in polling */
    g_oam_sdm_t.max_no_process = OAM_SDM_ONE;
    if (OAM_SDM_FAILURE == oam_sdm_mem_alloc (g_oam_sdm_t.max_no_process *
                sizeof(struct _oam_sdm_process_info_t), (void *)
                &g_oam_sdm_t.p_info, &error_code))
    {
        OAM_SDM_LOG (SDM, OAM_SDM_ERROR,
                "Error in Memory Allocation with Error Code:%d",
                error_code);
        oam_sdm_abort();
    }

    oam_sdm_strncpy(g_oam_sdm_t.p_info[OAM_SDM_ZERO].name, OAM_BIN_NAME, MAX_BIN_NAME_LEN);
    g_oam_sdm_t.p_info[OAM_SDM_ZERO].name[MAX_BIN_NAME_LEN - OAM_SDM_ONE] = '\0'; 
    g_oam_sdm_t.p_info[OAM_SDM_ZERO].pid = OAM_SDM_ZERO;

    /* Nothing to be saved in context */
    return OAM_SDM_NULL;
}
/* SPR 21889 end */

/****************************************************************************
 * Function Name  : oam_sdm_process_timer_expiry
 * Description    : This is used on expiry of timer
 * Inputs         : timer_id :Timer ID
 *                  p_timer_buf :Pointer to Timer Buffer
 *                  p_context :Pointer to context
 * Outputs        : none
 * Returns        : none
 *****************************************************************************/

void 
oam_sdm_process_timer_expiry(
		oam_sdm_timer_t timer_id, 
		void *p_timer_buf, 
		void *p_context
		)
{

	OAM_SDM_FUN_ENTRY_TRACE ();

	OAM_SDM_LOG (SDM, OAM_SDM_INFO,"Timer expired: Timer [%p] p_context [%p]"
			" p_timer_buf [%p]", 
			timer_id, p_context, p_timer_buf);

	if(OAM_SDM_NULL != p_timer_buf)
	{
		oam_timer_data_t *p_data = (oam_timer_data_t *) p_timer_buf;


		if(SDM_OAM_POLL_TIMER_TYPE == p_data->timer_type)
		{
/* SPR 14360 START */
       oam_sdm_handle_poll_timer_expiry(p_timer_buf,timer_id);
/* SPR 14360 END */
		}
		else if(SDM_OAM_ACTIVE_IND_TIMER_TYPE == p_data->timer_type)
		{
            /* SPR 17777 fix */
			oam_sdm_handle_active_ind_timer_expiry();  
		}
		qvMsgFree(p_timer_buf);
	}
	else
	{
		OAM_SDM_LOG (SDM, OAM_SDM_WARNING, "p_timer_buf is NULL");
	}
	OAM_SDM_FUN_EXIT_TRACE ();
}

/****************************************************************************
 * Function Name  : oam_start_new_timer
 * Inputs         : sub_module_id :sub module id
 *                  expiry_thread_id :Expiry thread ID
 *                  timer_type :Timer Type
 *                  timer_duration :Timer Duration
 *                  buf :Pointer to buffer
 *                  buf_size :Buffer size
 *                  is_repeated :Is repeated
 * Outputs        : none
 * Returns        : oam_sdm_timer_t
 * Description    : This function starts the timer.
 ****************************************************************************/
oam_sdm_timer_t
oam_start_new_timer (
		UInt16 sub_module_id, 
		UInt16 expiry_thread_id, 
		UInt16 timer_type, 
		UInt32 timer_duration,	/* unit: milliseconds */
		void *buf, 
		UInt16 buf_size, 
		oam_sdm_bool_et is_repeated
		)
{
	QTIME qtime;
	QMODULE module;
	oam_sdm_timer_t timer = OAM_SDM_NULL;
	oam_timer_data_t *p_timer_data = OAM_SDM_NULL;

	OAM_SDM_FUN_ENTRY_TRACE ();

	p_timer_data = qvMsgAlloc (OAM_SDM_NULL, OAM_SDM_ZERO, OAM_SDM_ZERO, sizeof (oam_timer_data_t));
	if (p_timer_data == OAM_SDM_NULL)
	{
		OAM_SDM_LOG(SDM,OAM_SDM_ERROR,"Memory allocation to"
				" p_timer_data failed");
		return OAM_SDM_NULL;
	}
	p_timer_data->module_id = sub_module_id;
	p_timer_data->timer_type = timer_type;
	p_timer_data->buf = buf;
	p_timer_data->buf_size = buf_size;
	qtime.s = timer_duration / OAM_SDM_TIME_CONV_S_TO_MS;
	qtime.us = (timer_duration % OAM_SDM_TIME_CONV_S_TO_MS) * OAM_SDM_TIME_CONV_S_TO_MS;
	if (OAM_SDM_ZERO == expiry_thread_id)
	{
		timer = qvTimerStart (&qtime, p_timer_data, is_repeated);
	}
	else
	{
		module = qvGetService (expiry_thread_id);
		timer = qvModuleTimerStart (module, &qtime, (void *) p_timer_data,
				is_repeated, OAM_SDM_NULL);
	}
	if (timer == OAM_SDM_NULL)
	{
		OAM_SDM_LOG (SDM, OAM_SDM_WARNING, "Timer Start failed for TimerType [%u]",
                timer_type);
		/* Release the memory allocated for storing the timer data */

		if (p_timer_data != OAM_SDM_NULL)
		{
			qvMsgFree (p_timer_data);
			OAM_SDM_LOG (SDM, OAM_SDM_DETAILED,
					"sdm_mem_free: memory free success");
		}
	}
	else
	{
		OAM_SDM_LOG (SDM, OAM_SDM_INFO, "Timer Started: Timer [%p], TimerType [%u],"
				" TimerDuration [%u]sec [%u]msec",
				timer, timer_type, qtime.s, qtime.us/1000);
	}
	OAM_SDM_FUN_EXIT_TRACE ();
	return timer;
}

/************************************************************************
 * Function Name  : oam_sdm_isValidIp4 
 * Inputs         : p_str :string 
 * Outputs        : none
 * Returns        : oam_sdm_return_et
 * Description    : This function Validates IP Read from cfg file.
 *************************************************************************/

oam_sdm_return_et 
oam_sdm_isValidIp4 (
		UInt8 *p_str
		) 
{
	SInt32 segs = OAM_SDM_ZERO;   /* Segment count. */
	SInt32 chcnt = OAM_SDM_ZERO;  /* Character count within segment. */
	SInt32 accum = OAM_SDM_ZERO;  /* Accumulator for segment. */


	OAM_SDM_FUN_ENTRY_TRACE ();

	/* Catch NULL pointer. */
	if (p_str == OAM_SDM_NULL)
	{
		OAM_SDM_FUN_EXIT_TRACE ();
		return OAM_SDM_FAILURE;
	}

	/* Process every character in p_string. */
	while (*p_str != OAM_SDM_ZERO)
	{            
		/* Segment changeover. */
		if (*p_str == '.')
		{    
			/* Must have some digits in segment. */

			if (chcnt == OAM_SDM_ZERO)
			{
				OAM_SDM_FUN_EXIT_TRACE ();
				return OAM_SDM_FAILURE;
			}
			/* Limit number of segments. */

			if (++segs == OAM_SDM_FOUR)
			{
				OAM_SDM_FUN_EXIT_TRACE ();
				return OAM_SDM_FAILURE;
			}
			/* Reset segment values and restart loop. */
			chcnt = accum = OAM_SDM_ZERO;
			p_str++;
			continue;
		}
		/* Check numeric. */
		if ((*p_str < '0') || (*p_str > '9'))
		{
			OAM_SDM_FUN_EXIT_TRACE ();
			return OAM_SDM_FAILURE;
		}
		/* Accumulate and check segment. */
		if ((accum = accum * OAM_SDM_TEN + *p_str - '0') > OAM_SDM_TWO_FIFTY_FIVE)
		{
			OAM_SDM_FUN_EXIT_TRACE ();
			return OAM_SDM_FAILURE;
		}
		/* Advance other segment specific stuff and continue loop. */

		/* Advance other segment specific stuff and continue loop. */

		chcnt++;
		p_str++;
	}

	/* Check enough segments and enough characters in last segment. */

	if (segs != OAM_SDM_THREE)
	{
		OAM_SDM_FUN_EXIT_TRACE ();
		return OAM_SDM_FAILURE;
	}

	if (chcnt == OAM_SDM_ZERO)
	{
		OAM_SDM_FUN_EXIT_TRACE ();
		return OAM_SDM_FAILURE;
	}
	/* Address okay. */
	OAM_SDM_LOG(SDM,OAM_SDM_DETAILED,"IP Address Validated Successfully");
	OAM_SDM_FUN_EXIT_TRACE ();
	return OAM_SDM_SUCCESS;
}



/************************************************************************
 * Function Name  : oam_sdm_handle_poll_timer_expiry
 * Inputs         : p_timer_buf :Pointer to Timer buffer
 * Outputs        : none
 * Returns        : oam_sdm_return_et
 * Description    : This function handles poll timer expiry.
 *************************************************************************/      
oam_sdm_return_et
oam_sdm_handle_poll_timer_expiry(
/* SPR 14360 START */
    void *p_timer_buf,
    oam_sdm_timer_t timer_id
/* SPR 14360 END */
		)
{
	OAM_SDM_FUN_ENTRY_TRACE ();

	oam_timer_data_t *p_data = (oam_timer_data_t *) p_timer_buf;
	SInt32 *error_code = OAM_SDM_NULL;

    /* SPR_19838: Coverity_10069 Fix Start */
    if((g_oam_sdm_event_ongoing!= OAM_TRUE)&&(g_oam_sdm_event_ongoing!=FACTORY_RESET))
    /* SPR_19838: Coverity_10069 Fix Stop */
	{
		if (g_sdm_poll_counter < MAX_NO_OF_POLL_TRY)
		{
			if (OAM_SDM_FAILURE ==
					oam_sdm_send_message (p_data->buf, OAM_MODULE_ID, error_code))
			{
				OAM_SDM_LOG (SDM, OAM_SDM_ERROR,
						"Error in sending polling message to oam : %d",
						error_code);
				OAM_SDM_FUN_EXIT_TRACE ();
				return OAM_SDM_FAILURE;
			}
			g_sdm_poll_counter = g_sdm_poll_counter + OAM_SDM_ONE;
			oam_sdm_handle_active_ind();
		}
		else
		{
			OAM_SDM_LOG (SDM, OAM_SDM_ERROR,
					"OAM Health is not good killing OAM !!!: %d", error_code);
			if(OAM_SDM_FAILURE == oam_sdm_shutdown_all())
			{
				OAM_SDM_LOG(SDM,OAM_SDM_CRITICAL,"ERROR In Killing OAM:!!!");
				OAM_SDM_FUN_EXIT_TRACE ();
				return OAM_SDM_FAILURE;
			}
/* SPR 14360 START */
            g_sdm_poll_counter = OAM_SDM_ZERO;
            if(OAM_SDM_SUCCESS == oam_sdm_stop_timer(timer_id))
            {
                OAM_SDM_LOG(SDM,OAM_SDM_INFO,"Stopped POLL Timer: Id(%d)",
                        timer_id);
            }
/* SPR 14360 END */
			OAM_SDM_LOG (SDM, OAM_SDM_INFO,"Spawning LTE OAM again");

			if (OAM_SDM_FAILURE ==
					oam_sdm_spawn_lte_oam ())
			{
				OAM_SDM_LOG (SDM, OAM_SDM_CRITICAL,
						"Error In Spawning lte_oam");
				OAM_SDM_FUN_EXIT_TRACE ();
				return OAM_SDM_FAILURE;
			}

		}
	}
	else {
		OAM_SDM_LOG (SDM, OAM_SDM_INFO,"Event Ongoing Marked True:Polling from OAM not required");
	}		
	OAM_SDM_FUN_EXIT_TRACE ();
	return OAM_SDM_SUCCESS;
}



/************************************************************************
 * Function Name  : oam_sdm_handle_active_ind_timer_expiry
 * Inputs         : p_timer_buf :Pointer to Timer buffer
 * Outputs        : none
 * Returns        : oam_sdm_return_et
 * Description    : This function handles active indication timer expiry.
 *************************************************************************/     
oam_sdm_return_et
oam_sdm_handle_active_ind_timer_expiry(
        /* SPR 17777 fix */
		void
		)
{
	OAM_SDM_FUN_ENTRY_TRACE();

	if(g_flag_for_oam_active_ind == OAM_TRUE)
	{
		OAM_SDM_LOG (SDM, OAM_SDM_INFO,"OAM is In Active State,Stop Timer for Active Ind"); 
	}
	else
	{
		char* oui_value = OAM_SDM_NULL;
		char* productClass_value                = OAM_SDM_NULL;
		char* hw_value                          = OAM_SDM_NULL;
		char* sw_value                          = OAM_SDM_NULL;
		FILE *fptr = OAM_SDM_NULL;
        /*SPR 21889 +- */
        /* SPR 14921 Fix Start */
        UInt8 pid = OAM_SDM_ZERO;
        SInt32 status = OAM_SDM_ZERO;
        /* SPR 14921 Fix End */

		UInt8 temp_file_name[MAX_BACKUP_STR_LENGTH]= {OAM_SDM_ZERO};

		OAM_SDM_LOG(SDM,OAM_SDM_ERROR,"OAM not come up In Active State,Spawning OAM with Last running software");  
        /* SPR 14921 Fix Start */
        oam_sdm_kill(g_oam_sdm_t.p_info[OAM_SDM_ZERO].pid,SIGTERM);
        pid = waitpid (g_oam_sdm_t.p_info[OAM_SDM_ZERO].pid,&status , WNOHANG);
        if(pid == g_oam_sdm_t.p_info[OAM_SDM_ZERO].pid)
        {
            OAM_SDM_LOG(SDM, OAM_SDM_INFO,
                    "Process %s having pid : %d killed successfully",
                    g_oam_sdm_t.p_info[OAM_SDM_ZERO].name ,g_oam_sdm_t.p_info[OAM_SDM_ZERO].pid);
        }
        else if(pid == OAM_SDM_ZERO)
        {
            OAM_SDM_LOG(SDM, OAM_SDM_INFO,
                    "Process %s having pid : %d no state change",
                    g_oam_sdm_t.p_info[OAM_SDM_ZERO].name ,g_oam_sdm_t.p_info[OAM_SDM_ZERO].pid);

            oam_sdm_kill(g_oam_sdm_t.p_info[OAM_SDM_ZERO].pid,SIGKILL);
            pid = waitpid(g_oam_sdm_t.p_info[OAM_SDM_ZERO].pid, &status, 0);
        	 /* Coverity_83683 Fix Start */
     
	       OAM_SDM_LOG(SDM, OAM_SDM_INFO,
                    "Process %s having pid : %d killed successfully,value of pid: %d",
                    g_oam_sdm_t.p_info[OAM_SDM_ZERO].name ,g_oam_sdm_t.p_info[OAM_SDM_ZERO].pid, pid);
	     /* Coverity_83683 Fix End */
        }
        else
        {
            OAM_SDM_LOG(SDM, OAM_SDM_WARNING,
                    "Process %s having pid : %d Error in killing",
                    g_oam_sdm_t.p_info[OAM_SDM_ZERO].name ,g_oam_sdm_t.p_info[OAM_SDM_ZERO].pid);
        }

        /*SPR 21889 +- */
        /* SPR 14921 Fix End */

        
		if(backup_counter != OAM_SDM_ZERO )
			snprintf((char *)g_file_name,MAX_BACKUP_STR_LENGTH,"../backup/%s",g_bin_backup_array[backup_counter-1]);
		g_oam_sdm_event_ongoing = OAM_SDM_ROLLBACK;
		if (OAM_SDM_FAILURE == oam_sdm_replace_software ())
		{
			OAM_SDM_LOG (SDM, OAM_SDM_ERROR, "Error in Replacing The Software");
			OAM_SDM_FUN_EXIT_TRACE ();
			return OAM_SDM_FAILURE;
		}

		fptr = oam_sdm_fopen ("../cfg/oam_version_info.txt", "w");

        /* SPR 14921 Fix Start*/
		if(backup_counter != OAM_SDM_ZERO ) /* Coverity Fix 31326 */
        oam_sdm_strcpy((char *)temp_file_name,(char *)g_bin_backup_array[backup_counter -1]);

        /* SPR 14921 Fix End */
		if (OAM_SDM_NULL != fptr)
		{
			if(temp_file_name[OAM_SDM_ZERO]=='_')
				oui_value = OAM_SDM_NULL;
			else
				oui_value=oam_sdm_strtok((char *)temp_file_name, "_");

			if (oui_value != OAM_SDM_NULL)
			{
				oam_sdm_fprintf (fptr,"ManufacturerOUI=%s\n", oui_value);
				productClass_value=oam_sdm_strtok(OAM_SDM_NULL, "_");
			}
			else
				productClass_value=oam_sdm_strtok((char *)temp_file_name, "_");

			if (productClass_value != OAM_SDM_NULL)
				oam_sdm_fprintf (fptr, "ProductClass=%s\n",productClass_value);

			hw_value = strtok(OAM_SDM_NULL, "_");

			if (hw_value != OAM_SDM_NULL)
				oam_sdm_fprintf (fptr, "HardwareVersion=%s\n",hw_value);

			sw_value = oam_sdm_strtok(OAM_SDM_NULL, "_");

			if (sw_value != OAM_SDM_NULL)
				oam_sdm_fprintf (fptr, "SoftwareVersion=%s\n",sw_value );

			oam_sdm_fclose(fptr);
		}
		else
		{
			OAM_SDM_LOG (SDM, OAM_SDM_WARNING, "Error in fillig version info");
		}
		if (OAM_SDM_FAILURE ==
				oam_sdm_spawn_lte_oam ())
		{
			OAM_SDM_LOG (SDM, OAM_SDM_CRITICAL,
					"Error In Spawning lte_oam");
			OAM_SDM_FUN_EXIT_TRACE ();
			return OAM_SDM_FAILURE;

		}
	}
    OAM_SDM_FUN_EXIT_TRACE(); /* Coverity CID 54888 FIx */
	return OAM_SDM_SUCCESS;
}



/************************************************************************
 * Function Name  : oam_sdm_shutdown_all
 * Inputs         : none
 * Outputs        : none
 * Returns        : oam_sdm_return_et
 * Description    : This function shutdown all processes.
 *************************************************************************/     
/* + SPR 17439 */
	oam_sdm_return_et
oam_sdm_shutdown_all(void)
/* + SPR 17439 */
{
	OAM_SDM_FUN_ENTRY_TRACE();

	char command[OAM_SDM_SHUTDWN_CMD_SIZE] = {OAM_SDM_ZERO};
	UInt8 process_counter = OAM_SDM_ZERO;
/* SPR 14360 START */
    UInt8 pid = OAM_SDM_ZERO;
    SInt32 status = OAM_SDM_ZERO;
/* SPR 14360 END */

	for(process_counter = OAM_SDM_ZERO ; 
			process_counter < g_oam_process_number + OAM_SDM_ONE ; process_counter++)
	{
		if (g_oam_sdm_t.p_info[process_counter].pid == OAM_SDM_ZERO)
			continue;
/* SPR 14360 START */
        oam_sdm_kill(g_oam_sdm_t.p_info[process_counter].pid,SIGTERM);
        pid = waitpid (g_oam_sdm_t.p_info[process_counter].pid,&status , WNOHANG);
        if(pid == g_oam_sdm_t.p_info[process_counter].pid)
        {
            OAM_SDM_LOG(SDM, OAM_SDM_INFO,
                    "Process %s having pid : %d killed successfully",
                    g_oam_sdm_t.p_info[process_counter].name ,g_oam_sdm_t.p_info[process_counter].pid);
        }
        else if(pid == 0)
		{
            OAM_SDM_LOG(SDM, OAM_SDM_INFO,
                    "Process %s having pid : %d no state change",
                    g_oam_sdm_t.p_info[process_counter].name ,g_oam_sdm_t.p_info[process_counter].pid);

            oam_sdm_kill(g_oam_sdm_t.p_info[process_counter].pid,SIGKILL);
            pid = waitpid(g_oam_sdm_t.p_info[process_counter].pid, &status, 0);

	    /* Coverity_81745 Fix Start */

                 OAM_SDM_LOG(SDM, OAM_SDM_INFO,
					"Process %s having pid : %d killed successfully,value of pid :%d",
					g_oam_sdm_t.p_info[process_counter].name ,g_oam_sdm_t.p_info[process_counter].pid,pid);
	        /* Coverity_81745 Fix End */
	
	       }
		else
		{
			OAM_SDM_LOG(SDM, OAM_SDM_WARNING,
                    "Process %s having pid : %d Error in killing",
					g_oam_sdm_t.p_info[process_counter].name ,g_oam_sdm_t.p_info[process_counter].pid);
			OAM_SDM_FUN_EXIT_TRACE();
			return OAM_SDM_FAILURE;
		}
	}
    snprintf(command ,OAM_SDM_SHUTDWN_CMD_SIZE ,"killall -9 ltebinread");
/* SPR 14360 END */
    if (OAM_ZERO != oam_sdm_system(command))
    {
        OAM_SDM_LOG(SDM, OAM_SDM_ERROR,
                    "Failed to execute command %s. Errno %u", command, errno);
    }

		OAM_SDM_FUN_EXIT_TRACE();
	return OAM_SDM_SUCCESS;
}
/* SPR 14360 START */
/****************************************************************************
 * Function Name  : oam_sdm_stop_timer
 * Inputs         : timer :Timer
 * Outputs        : none
 * Returns        : OAM_SUCCESS/OAM_FAILURE
 * Description    : This function stops the timer.
 ****************************************************************************/
oam_sdm_return_et
oam_sdm_stop_timer
(
 oam_sdm_timer_t timer
 )
{
    void *timer_ptr;
    if (OAM_SDM_NULL == timer)
    {
        return OAM_SDM_FAILURE ;
    }
    else
    {

        if(qvTimerRunning((QTIMER)(timer)) == OAM_SDM_ONE)
        {
		    OAM_SDM_LOG(SDM, OAM_SDM_INFO,"Stopping Timer: Id(%p)",
                        (QTIMER)(timer));
            qvTimerStop(timer, OAM_SDM_NULL);
            timer_ptr = qvTimerData(timer);
            if(OAM_SDM_NULL != timer_ptr)
            qvMsgFree(timer_ptr);
        }
    }
    return OAM_SDM_SUCCESS ;
}
/* SPR 14360 END */








