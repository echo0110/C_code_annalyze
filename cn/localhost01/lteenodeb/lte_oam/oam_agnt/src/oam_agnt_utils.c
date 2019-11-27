
/*******************************************************************************
 * File Name       : oam_agnt_utils.c
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
#include "oam_agnt_utils.h"
#include "oam_agnt_port.h"
#include "oam_defines.h"
#include "advlogging.h"
/*******************************************************************************
 *  Global Declaration
 *******************************************************************************/
LOCAL UInt8 g_instance_id = OAM_AGNT_ZERO;

/*SPR 21889 +- */
/****************************************************************************
  Private Types
 ****************************************************************************/


/******************************************************************************
 *   FUNCTION NAME  : oam_agnt_convert_log_level
 *   RETURNS        : Corresponding log level of the advance logger
 *   INPUTS         : log_level :Log level
 *   DESCRIPTION    : Does mapping between the internal log level and he log
 *                    level of the advance logger
 ******************************************************************************/
AdvL_LogLevel oam_agnt_convert_log_level (log_priority_et agnt_log_level)
{
	AdvL_LogLevel log_level;
	switch(agnt_log_level)
	{
		case OAM_AGNT_CRITICAL:
			log_level=LogFatal;
			break;
		case OAM_AGNT_ERROR:
			log_level=LogError;
			break;
		case OAM_AGNT_WARNING:
			log_level=LogWarning;
			break;
		case OAM_AGNT_INFO:
			log_level=LogInfo;
			break;
		case OAM_AGNT_BRIEF:
			log_level=LogBrief;
			break;
		case OAM_AGNT_DETAILED:
			log_level=LogDetailed;
			break;
        case OAM_AGNT_DETAILEDALL:
            log_level=LogDebug;
            break;
		default:
			log_level=LogTrace;
			break;

	}
	return log_level;
}


/******************************************************************************
 *   FUNCTION NAME  : oam_agnt_trace_message
 *   RETURNS        : None
 *   INPUTS         : log_level :Log level
 *                    pFile :Pointer to file
 *                    pFun : Pointer to function
 *                    Line : line
 *                    format :format.
 *   DESCRIPTION    : function to print trace message.
 ******************************************************************************/
void
oam_agnt_trace_message (SInt32 agnt_log_level,	/* log level of this message */
		SInt8 * pFile, SInt8 * pFun, UInt16 Line, SInt8 * format,	/* format string like in printf */
		...	/* optional parameters */
		)
{
	va_list arg_list;
	AdvL_LogLevel log_level;
	Char8 temp_str[OAM_AGNT_TRACE_SIZE];

	if (OAM_NULL == pFile || OAM_NULL == pFun)
	{
		return; 
	}
    OAM_AGNT_VA_START (arg_list, format);

    /* CID 118134 Fix Start */
	log_level = oam_agnt_convert_log_level((log_priority_et)agnt_log_level);
    /* CID 118134 Fix End */
	OAM_AGNT_VSPRINTF(temp_str, (const Char8*)format, arg_list);
	write_log_message(LogContext_Default,log_level,(const Char8*)pFile,Line,
			(const Char8*)pFun,"AGENT",0xff,0xffff,"%s", temp_str);
    OAM_AGNT_VA_END (arg_list);
}

/*SPR 21889 +- */
/****************************************************************************
 * Function Name  : oam_agnt_get_word_from_header
 * Inputs         : p_header :Pointer to header
 * Outputs        : none
 * Returns        : UInt16 value
 * Description    : This function finds the value of 2 bytes
 *****************************************************************************/
oam_agnt_return_et
oam_agnt_get_word_from_header (
		UInt8 * p_header
		)
{
	UInt16 value = OAM_AGNT_ZERO;

	value = p_header[OAM_AGNT_ZERO];
	value = value << OAM_AGNT_EIGHT;
	value = value | p_header[OAM_AGNT_ONE];

	/* coverity 35999 */
	return (oam_agnt_return_et)value;
}

/****************************************************************************
 * Function Name  : oam_agnt_construct_mgmt_api_hdr
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
oam_agnt_construct_interface_api_header(
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

	OAM_AGNT_FUN_ENTRY_TRACE ();
	if (OAM_AGNT_NULL == p_msg)
	{
		OAM_AGNT_LOG (AGNT, OAM_AGNT_WARNING,
				"Null pointer passed");
		return;
	}

	*(p_msg + OAM_AGNT_ZERO) = (trans_id & OAM_AGNT_UPPER_EIGHT_BITS_SET) >> OAM_AGNT_EIGHT;
	*(p_msg + OAM_AGNT_ONE) = trans_id & OAM_AGNT_ALL_BIT_SET;

	*(p_msg + OAM_AGNT_TWO) = (src_mod_id & OAM_AGNT_UPPER_EIGHT_BITS_SET) >> OAM_AGNT_EIGHT;
	*(p_msg + OAM_AGNT_THREE) = src_mod_id & OAM_AGNT_ALL_BIT_SET;

	*(p_msg + OAM_AGNT_FOUR) = (dst_mod_id & OAM_AGNT_UPPER_EIGHT_BITS_SET) >> OAM_AGNT_EIGHT;
	*(p_msg + OAM_AGNT_FIVE) = dst_mod_id & OAM_AGNT_ALL_BIT_SET;

	*(p_msg + OAM_AGNT_SIX) = (api_id & OAM_AGNT_UPPER_EIGHT_BITS_SET) >> OAM_AGNT_EIGHT;
	*(p_msg + OAM_AGNT_SEVEN) = api_id & OAM_AGNT_ALL_BIT_SET;
    api_buf_size += OAM_AGNT_INTERFACE_API_HEADER_SIZE;
	*(p_msg + OAM_AGNT_EIGHT) = (api_buf_size & OAM_AGNT_UPPER_EIGHT_BITS_SET) >> OAM_AGNT_EIGHT;
	*(p_msg + OAM_AGNT_NINE) = api_buf_size & OAM_AGNT_ALL_BIT_SET;

	*(p_msg + OAM_AGNT_TEN) = resvd;/* Instance */
	OAM_AGNT_FUN_EXIT_TRACE ();
}

/****************************************************************************
 * Function Name  : oam_agnt_construct_api_header
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
oam_agnt_construct_api_header (
		UInt8 * p_header,	/* CSPL header */
		UInt8 version_id,	/* API version Id */
		UInt16 src_module_id,	/* Source module Id */
		UInt16 dst_module_id,	/* Destination module Id */
		UInt16 api_id,	/* API Id */
		UInt16 api_buf_size	/* API buffer size */
		)
{

	OAM_AGNT_FUN_ENTRY_TRACE ();

	/* version */
	p_header[OAM_AGNT_ZERO] = version_id;

	/* from (HI) */
	p_header[OAM_AGNT_ONE] = (src_module_id & OAM_AGNT_UPPER_EIGHT_BITS_SET) >> OAM_AGNT_EIGHT;

	/* from (LOW) */
	p_header[OAM_AGNT_TWO] = src_module_id & OAM_AGNT_LOWER_EIGHT_BITS_SET;

	/* to (HI) */
	p_header[OAM_AGNT_THREE] = (dst_module_id & OAM_AGNT_UPPER_EIGHT_BITS_SET) >> OAM_AGNT_EIGHT;

	/* to (LOW) */
	p_header[OAM_AGNT_FOUR] = dst_module_id & OAM_AGNT_LOWER_EIGHT_BITS_SET;

	/* api id (HI) */
	p_header[OAM_AGNT_FIVE] = (api_id & OAM_AGNT_UPPER_EIGHT_BITS_SET) >> OAM_AGNT_EIGHT;

	/* api id (LOW) */
	p_header[OAM_AGNT_SIX] = api_id & OAM_AGNT_LOWER_EIGHT_BITS_SET;

	/* api size (HI) */
	p_header[OAM_AGNT_SEVEN] = (api_buf_size & OAM_AGNT_UPPER_EIGHT_BITS_SET) >> OAM_AGNT_EIGHT;

	/* api size (LOW) */
	p_header[OAM_AGNT_EIGHT] = api_buf_size & OAM_AGNT_LOWER_EIGHT_BITS_SET;

	/* spare bytes */
	p_header[OAM_AGNT_NINE]     = OAM_AGNT_ALL_BIT_ZERO;
	p_header[OAM_AGNT_TEN]      = OAM_AGNT_ALL_BIT_ZERO;
	p_header[OAM_AGNT_ELEVEN]   = OAM_AGNT_ALL_BIT_ZERO;
	p_header[OAM_AGNT_TWELVE]   = OAM_AGNT_ALL_BIT_ZERO;
	p_header[OAM_AGNT_THIRTEEN] = OAM_AGNT_ALL_BIT_ZERO;
	p_header[OAM_AGNT_FOURTEEN] = OAM_AGNT_ALL_BIT_ZERO;
	p_header[OAM_AGNT_FIFTEEN]  = OAM_AGNT_ALL_BIT_ZERO;

	OAM_AGNT_FUN_EXIT_TRACE ();
}


/************************************************************************
 * Function Name  : oam_agnt_isValidIp4 
 * Inputs         : p_str :string 
 * Outputs        : none
 * Returns        : oam_agnt_return_et
 * Description    : This function Validates IP Read from cfg file.
 *************************************************************************/

oam_agnt_return_et 
oam_agnt_isValidIp4 (
		UInt8 *p_str
		) 
{
	SInt32 segs = OAM_AGNT_ZERO;   /* Segment count. */
	SInt32 chcnt = OAM_AGNT_ZERO;  /* Character count within segment. */
	SInt32 accum = OAM_AGNT_ZERO;  /* Accumulator for segment. */


	OAM_AGNT_FUN_ENTRY_TRACE ();

	/* Catch NULL pointer. */
	if (p_str == OAM_AGNT_NULL)
	{
		OAM_AGNT_FUN_EXIT_TRACE ();
		return OAM_AGNT_FAILURE;
	}

	/* Process every character in p_string. */
	while (*p_str != OAM_AGNT_ZERO)
	{            
		/* Segment changeover. */
		if (*p_str == '.')
		{    
			/* Must have some digits in segment. */

			if (chcnt == OAM_AGNT_ZERO)
			{
				OAM_AGNT_FUN_EXIT_TRACE ();
				return OAM_AGNT_FAILURE;
			}
			/* Limit number of segments. */

			if (++segs == OAM_AGNT_FOUR)
			{
				OAM_AGNT_FUN_EXIT_TRACE ();
				return OAM_AGNT_FAILURE;
			}
			/* Reset segment values and restart loop. */
			chcnt = accum = OAM_AGNT_ZERO;
			p_str++;
			continue;
		}
		/* Check numeric. */
		if ((*p_str < '0') || (*p_str > '9'))
		{
			OAM_AGNT_FUN_EXIT_TRACE ();
			return OAM_AGNT_FAILURE;
		}
		/* Accumulate and check segment. */
		if ((accum = accum * OAM_AGNT_TEN + *p_str - '0') > OAM_AGNT_TWO_FIFTY_FIVE)
		{
			OAM_AGNT_FUN_EXIT_TRACE ();
			return OAM_AGNT_FAILURE;
		}
		/* Advance other segment specific stuff and continue loop. */

		/* Advance other segment specific stuff and continue loop. */

		chcnt++;
		p_str++;
	}

	/* Check enough segments and enough characters in last segment. */

	if (segs != OAM_AGNT_THREE)
	{
		OAM_AGNT_FUN_EXIT_TRACE ();
		return OAM_AGNT_FAILURE;
	}

	if (chcnt == OAM_AGNT_ZERO)
	{
		OAM_AGNT_FUN_EXIT_TRACE ();
		return OAM_AGNT_FAILURE;
	}
	/* Address okay. */
	OAM_AGNT_LOG(AGNT,OAM_AGNT_DETAILED,"IP Address Validated Successfully");
	OAM_AGNT_FUN_EXIT_TRACE ();
	return OAM_AGNT_SUCCESS;
}

/*************************************************************************
 * Function Name  : oam_agnt_start_timer
 * Description    : This is used for starting the timer 
 * Inputs         : p_msg, dst_module
 * Outputs        : none
 * Returns        : OAM_AGNT_SUCCESS/OAM_AGNT_FAILURE
 ******************************************************************************/
oam_agent_timer_t
oam_agnt_start_timer 
(
 /* Duration (in seconds) of the timer to be started */
 U32         timer_duration,
 oam_agent_timer_data_t *p_timer_data,
 oam_agnt_bool_et is_repeated         /* Indicates whether timer is repeated */
 )
{
    QTIME qtime;
    void *p_timer_buf = OAM_AGNT_NULL;
    oam_agent_timer_t timer = OAM_AGNT_NULL;
    oam_agnt_error_code_et err = OAM_AGNT_NO_ERROR;

    if (OAM_AGNT_SUCCESS != oam_agnt_mem_alloc(sizeof(oam_agent_timer_data_t), &p_timer_buf, &err))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_ERROR,
                "Failed to allocate memory for timer type %u. Timer not started",
                p_timer_data->timer_type);
    /* SPR22031 Start */
        return timer;
    /* SPR22031 End */
    }

    oam_agnt_memcpy(p_timer_buf, p_timer_data, sizeof(oam_agent_timer_data_t));

    qtime.s = timer_duration;
    qtime.us = OAM_AGNT_ZERO;

    timer = qvTimerStart(&qtime, p_timer_buf, is_repeated);

    if(timer == OAM_AGNT_NULL)
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_WARNING, "Failed to start timer type %u", p_timer_data->timer_type);
        /* Release the memory allocated for storing the timer data */
        oam_agnt_mem_free(p_timer_buf, &err);
    }
    else
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_BRIEF, "Timer Started: Timer [%p], TimerType [%u], TimerDuration [%u] sec", 
                timer, p_timer_data->timer_type, timer_duration);

    }

    return timer;
}


/*************************************************************************
 * Function Name  : oam_agnt_stop_timer
 * Description    : This is used for stopping the timer started by the module
 * Inputs         : p_msg, dst_module
 * Outputs        : none
 * Returns        : OAM_AGNT_SUCCESS/OAM_AGNT_FAILURE
 ******************************************************************************/
void
oam_agnt_stop_timer(oam_agent_timer_t timer_id)
{
    void *p_data;
    oam_agnt_error_code_et err = OAM_AGNT_NO_ERROR;

    if (qvTimerRunning(timer_id))
    {
        p_data = qvTimerData(timer_id);
        if (OAM_AGNT_NULL != p_data)
        {
            oam_agnt_mem_free(p_data, &err);
        }
        qvTimerStop(timer_id, OAM_AGNT_NULL);
        OAM_AGNT_LOG(AGNT, OAM_AGNT_INFO, "Timer 0x%x stopped", timer_id);
    }
    else
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_INFO, "Timer 0x%x was not running", timer_id);
    }
}

/*************************************************************************
 * Function Name  : oam_agnt_mem_alloc
 * Inputs         : size :size
 *                  p_msg :Pointer to pointer to msg
 * Outputs        : p_error_code :Pointer to error code
 * Returns        : OAM_AGNT_SUCCESS/OAM_AGNT_FAILURE
 * Description    : This function returns memory buffer from memory pool.
 *****************************************************************************/

oam_agnt_return_et
oam_agnt_mem_alloc (size_t size,	/* Size of buffer which will be allocated */
        void **p_msg, oam_agnt_error_code_et * p_error_code)
{
    void *p_buf = OAM_AGNT_NULL;

    p_buf = qvMsgAlloc(OAM_AGNT_NULL, OAM_AGNT_ZERO, OAM_AGNT_ZERO, size);	/* temp mask */

    if (p_buf == OAM_AGNT_NULL)
    {
        OAM_AGNT_LOG (AGNT, OAM_AGNT_ERROR,
                "\noam_mem_alloc: memory allocation failed");
        *p_error_code = OAM_AGNT_MALLOC_FAIL;

        OAM_AGNT_FUN_EXIT_TRACE ();
        return OAM_AGNT_FAILURE;
    }
    *p_msg = p_buf;
    return OAM_AGNT_SUCCESS;
}
/****************************************************************************
 * Function Name  : oam_mem_free
 * Inputs         : p_buffer,p_error_code
 * Outputs        : none
 * Returns        : SUCCESS/FAILURE
 * Description    : This function frees the memory pointer
 ****************************************************************************/

oam_agnt_return_et
oam_agnt_mem_free
(
 void *p_buffer, /* Pointer to buffer which will be freed */
 oam_agnt_error_code_et *p_error_code
 )
{
    if (p_buffer != OAM_AGNT_NULL)
    {
#ifdef MEM_CHECK_DOUBLE_FREE
        oam_agnt_free(p_buffer);
#else
        qvMsgFree(p_buffer);
#endif
    }
    else
    {
        OAM_AGNT_LOG(OAM_AGNT,OAM_AGNT_WARNING,
                "p_buffer is NULL (unused ""parameter, ignore = %p)", p_error_code); /* SPR1777 */
        return OAM_AGNT_FAILURE;
    }

    return OAM_AGNT_SUCCESS ;
}

/*************************************************************************
 * Function Name  : agent_chk_file_exist
 * Description    : This is a wrapper function to check if the file exist or not
 * Inputs         : p_filename 
 * Outputs        : p_errno
 * Returns        : OAM_AGNT_SUCCESS/OAM_AGNT_FAILURE
 ******************************************************************************/
oam_agnt_bool_et
agent_chk_file_exist(Char8 *p_filename,
        SInt16 *p_errno)
{
    if (OAM_AGNT_ZERO != oam_agnt_chmod(p_filename, OAM_BIN_FILE_PERMISSION))
    {
        *p_errno = errno;
     /* SPR22031 Start */
        return OAM_AGNT_FALSE;
    }

    return OAM_AGNT_TRUE;
     /* SPR22031 End */
}

/*************************************************************************
 * Function Name  : oam_agnt_send_message
 * Description    : This is used for sending message to external modules
 * Inputs         : p_msg, dst_module
 * Outputs        : none
 * Returns        : OAM_AGNT_SUCCESS/OAM_AGNT_FAILURE
 ******************************************************************************/
oam_agnt_return_et
oam_agnt_send_message (void *p_msg,	/* Message that will be passed */
        int dst_module)	/* Module id for which message will be passed */
{
    OAM_AGNT_FUN_ENTRY_TRACE ();
    UInt16 src_id =oam_agnt_get_word_from_header((UInt8 *)p_msg + OAM_AGNT_TWO);
    UInt16 dst_id =oam_agnt_get_word_from_header((UInt8 *)p_msg + OAM_AGNT_FOUR);

    if((OAM_AGNT_ZERO==src_id)||(OAM_AGNT_ZERO==dst_id))
    {
        OAM_AGNT_LOG (AGNT, OAM_AGNT_ERROR,"\nEither Source Id or Destination Id received is ZERO");
        OAM_AGNT_LOG (AGNT, OAM_AGNT_ERROR,"\nsrc_id=%d and dst_id=%d",src_id,dst_id);
        return OAM_AGNT_FAILURE;
    }
    int error_code = 0; 
    QMODULE service = qvGetServiceEx (dst_module,(int *)&error_code);
    
    qvSend(service, OAM_AGNT_ZERO, p_msg);

    OAM_AGNT_FUN_EXIT_TRACE ();
    return OAM_AGNT_SUCCESS;
}

/*************************************************************************
 * Function Name  : oam_agnt_get_cksum
 * Description    : This is get the checksum of the file name passed
 * Inputs         : Filename
 * Outputs        : Cksum
 * Returns        : OAM_AGNT_SUCCESS/OAM_AGNT_FAILURE
 ******************************************************************************/
oam_agnt_return_et
oam_agnt_get_cksum(Char8 *p_filename, UInt64 *p_cksum)
{
    FILE *fp;
    Char8 line[OAM_AGNT_MAX_CMD_SIZE];
    Char8 cmd[OAM_AGNT_MAX_CMD_SIZE];
    SInt16 err = OAM_AGNT_ZERO;
    /* SPR 22704 Fix Start */
    *p_cksum = OAM_AGNT_ZERO;
    /* SPR 22704 Fix End */
    if (OAM_AGNT_TRUE == agent_chk_file_exist(p_filename, &err))
    {
        snprintf(cmd, OAM_AGNT_MAX_CMD_SIZE, "cksum %s", p_filename);
        /* SPR22031 Start */
        errno = OAM_AGNT_ZERO;
        fp = oam_agnt_popen(cmd, "r");
        if(NULL == fp)
        {
            OAM_AGNT_LOG (AGNT, OAM_AGNT_ERROR,"popen returned failure with errno=%d",errno);
            *p_cksum = OAM_AGNT_ZERO;
            return OAM_AGNT_FAILURE; 
        }
        /* SPR22031 End */
        if (fgets(line, sizeof(line), fp) != OAM_AGNT_NULL)
        {
            *p_cksum = oam_agnt_atoi(oam_agnt_strtok(line, " "));
        }
        oam_agnt_pclose(fp);
        return OAM_AGNT_SUCCESS;
    }
    else
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_INFO, "File %s does not exist. Errno %d",
                p_filename, err);
    }

    /* SPR 22704 Fix +- */
    return OAM_AGNT_FAILURE;
}

/*************************************************************************
 * Function Name  : oam_agnt_get_my_id
 * Description    : This is used for getting the instance id of agent
 * Inputs         : None
 * Outputs        : none
 * Returns        : Instance id of the agent
 ******************************************************************************/
UInt8
oam_agnt_get_my_id()
{
    return g_instance_id;
}

/*************************************************************************
 * Function Name  : oam_agnt_set_my_id
 * Description    : This is used for setting the instance id of the agent
 * Inputs         : Instance Id
 * Outputs        : none
 * Returns        : None
 ******************************************************************************/
void
oam_agnt_set_my_id(UInt8 inst_id)
{
    g_instance_id = inst_id;
}

oam_agnt_return_et
oam_agnt_cp_pack_UInt8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "%s = %d", varname, *((UInt8 *)p_src));
    oam_agnt_memcpy(p_dest, p_src, sizeof(UInt8));
    return(OAM_AGNT_SUCCESS);
}

oam_agnt_return_et
oam_agnt_cp_pack_Char8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "%s = %d", varname, *((Char8 *)p_src));
    oam_agnt_memcpy(p_dest, p_src, sizeof(Char8));
    return(OAM_AGNT_SUCCESS);
}

oam_agnt_return_et
oam_agnt_cp_pack_UInt16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Packing U16 */

    ((UInt8*)p_dest)[0]    = (U8)((*(UInt16 *)p_src) >> 8);
    ((UInt8*)p_dest)[1]    = (U8)((*(UInt16 *)p_src));

    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "%s = %d", varname, *((UInt16 *)p_src));
    return(OAM_AGNT_SUCCESS);
}

oam_agnt_return_et
oam_agnt_cp_pack_UInt64
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Packing U64 */

    ((U8*)p_dest)[0]    = (U8)((*(UInt64 *)p_src) >> 56);
    ((U8*)p_dest)[1]    = (U8)((*(UInt64 *)p_src) >> 48);
    ((U8*)p_dest)[2]    = (U8)((*(UInt64 *)p_src) >> 40);
    ((U8*)p_dest)[3]    = (U8)((*(UInt64 *)p_src) >> 32);
    ((U8*)p_dest)[4]    = (U8)((*(UInt64 *)p_src) >> 24);
    ((U8*)p_dest)[5]    = (U8)((*(UInt64 *)p_src) >> 16);
    ((U8*)p_dest)[6]    = (U8)((*(UInt64 *)p_src) >> 8);
    ((U8*)p_dest)[7]    = (U8)((*(UInt64 *)p_src));

    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "%s = %llu", varname, *((UInt64 *)p_src));
    return(OAM_AGNT_SUCCESS);
}

oam_agnt_return_et
oam_agnt_cp_pack_UInt32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Packing U32 */

    ((UInt8*)p_dest)[0]    = (UInt8)((*(UInt32 *)p_src) >> 24);
    ((UInt8*)p_dest)[1]    = (UInt8)((*(UInt32 *)p_src) >> 16);
    ((UInt8*)p_dest)[2]    = (UInt8)((*(UInt32 *)p_src) >> 8);
    ((UInt8*)p_dest)[3]    = (UInt8)((*(UInt32 *)p_src));

    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "%s = %d", varname, *((UInt32 *)p_src));
    return(OAM_AGNT_SUCCESS);
}

oam_agnt_return_et
oam_agnt_cp_pack_SInt8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "%s = %d", varname, *((SInt8 *)p_src));
    oam_agnt_memcpy(p_dest, p_src, sizeof(SInt8));

    return(OAM_AGNT_SUCCESS);
}

oam_agnt_return_et
oam_agnt_cp_pack_SInt16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Packing S16 */

    ((UInt8*)p_dest)[0]    = (UInt8)((*(SInt16 *)p_src) >> 8);
    ((UInt8*)p_dest)[1]    = (UInt8)((*(SInt16 *)p_src));

    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "%s = %d", varname, *((SInt16 *)p_src));

    return(OAM_AGNT_SUCCESS);
}

oam_agnt_return_et
oam_agnt_cp_pack_SInt32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Packing S32 */

    ((UInt8*)p_dest)[0]    = (UInt8)((*(SInt32 *)p_src) >> 24);
    ((UInt8*)p_dest)[1]    = (UInt8)((*(SInt32 *)p_src) >> 16);
    ((UInt8*)p_dest)[2]    = (UInt8)((*(SInt32 *)p_src) >> 8);
    ((UInt8*)p_dest)[3]    = (UInt8)((*(SInt32 *)p_src));

    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "%s = %d", varname, *((SInt32 *)p_src));

    return(OAM_AGNT_SUCCESS);
}

oam_agnt_return_et
oam_agnt_cp_unpack_UInt8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    oam_agnt_memcpy(p_dest, p_src, sizeof(UInt8));

    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "%s = %d", varname, *((UInt8 *)p_dest));

    return(OAM_AGNT_SUCCESS);
}

oam_agnt_return_et
oam_agnt_cp_unpack_Char8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    oam_agnt_memcpy(p_dest, p_src, sizeof(Char8));

    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "%s = %d", varname, *((Char8 *)p_dest));

    return(OAM_AGNT_SUCCESS);
}

oam_agnt_return_et
oam_agnt_cp_unpack_UInt16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Unpacking U16 */

    *(UInt16 *)p_dest  = 0;

    *(UInt16 *)p_dest  |= (UInt16)((U8*)p_src)[0] << 8;
    *(UInt16 *)p_dest  |= (UInt16)((U8*)p_src)[1];

    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "%s = %d", varname, *((UInt16 *)p_dest));

    return(OAM_AGNT_SUCCESS);
}

oam_agnt_return_et
oam_agnt_cp_unpack_UInt64
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Unpacking U64 */

    *(UInt64 *)p_dest  = 0;

    *(UInt64 *)p_dest  |= (UInt64)((U8*)p_src)[0] << 56;
    *(UInt64 *)p_dest  |= (UInt64)((U8*)p_src)[1] << 48;
    *(UInt64 *)p_dest  |= (UInt64)((U8*)p_src)[2] << 40;
    *(UInt64 *)p_dest  |= (UInt64)((U8*)p_src)[3] << 32;
    *(UInt64 *)p_dest  |= (UInt64)((U8*)p_src)[4] << 24;
    *(UInt64 *)p_dest  |= (UInt64)((U8*)p_src)[5] << 16;
    *(UInt64 *)p_dest  |= (UInt64)((U8*)p_src)[6] << 8;
    *(UInt64 *)p_dest  |= (UInt64)((U8*)p_src)[7];

    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "%s = %llu", varname, *((UInt64 *)p_dest));

    return(OAM_AGNT_SUCCESS);
}

oam_agnt_return_et
oam_agnt_cp_unpack_UInt32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Unpacking U32 */

    *(UInt32 *)p_dest  = 0;

    *(UInt32 *)p_dest  |= (UInt32)((UInt8*)p_src)[0] << 24;
    *(UInt32 *)p_dest  |= (UInt32)((UInt8*)p_src)[1] << 16;
    *(UInt32 *)p_dest  |= (UInt32)((UInt8*)p_src)[2] << 8;
    *(UInt32 *)p_dest  |= (UInt32)((UInt8*)p_src)[3];

    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "%s = %d", varname, *((UInt32 *)p_dest));
    return(OAM_AGNT_SUCCESS);
}

oam_agnt_return_et
oam_agnt_cp_unpack_SInt8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    oam_agnt_memcpy(p_dest, p_src, sizeof(SInt8));

    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "%s = %d", varname, *((SInt8 *)p_dest));
    return(OAM_AGNT_SUCCESS);
}

oam_agnt_return_et
oam_agnt_cp_unpack_SInt16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Unpacking S16 */

    *(SInt16 *)p_dest  = 0;

    *(SInt16 *)p_dest  |= (SInt16)((U8*)p_src)[0] << 8;
    *(SInt16 *)p_dest  |= (SInt16)((U8*)p_src)[1];

    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "%s = %d", varname, *((SInt16 *)p_dest));

    return(OAM_AGNT_SUCCESS);
}

oam_agnt_return_et
oam_agnt_cp_unpack_SInt32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
)
{
    /* Unpacking S32 */

    *(S32 *)p_dest  = 0;

    *(SInt32 *)p_dest  |= (SInt32)((U8*)p_src)[0] << 24;
    *(SInt32 *)p_dest  |= (SInt32)((U8*)p_src)[1] << 16;
    *(SInt32 *)p_dest  |= (SInt32)((U8*)p_src)[2] << 8;
    *(SInt32 *)p_dest  |= (SInt32)((U8*)p_src)[3];

    OAM_AGNT_LOG(AGNT, OAM_AGNT_COMP_PARSE, "%s = %d", varname, *((S32 *)p_dest));

    return(OAM_AGNT_SUCCESS);
}

/*************************************************************************
 * Function Name  : oam_agnt_get_api_len
 * Description    : This is used for returning value of the API len excluding all headers
 * Inputs         : Complete API buffer received
 * Outputs        : none
 * Returns        : Length of the actual API
 ******************************************************************************/
SInt32
oam_agnt_get_api_len(UInt8 *p_api_buf)
{
    return oam_agnt_get_word_from_header(p_api_buf + OAM_AGNT_MSG_SZ_HDR_IDX) -
        OAM_AGNT_INTERFACE_API_HEADER_SIZE;
}

/*************************************************************************
 * Function Name  : oam_agnt_get_api_start
 * Description    : Return the pointer from where actual API start
 * Inputs         : Complete API buffer with all headers
 * Outputs        : none
 * Returns        : Start of the API address
 ******************************************************************************/
UInt8*
oam_agnt_get_api_start(UInt8 *p_api_buf)
{
    return (p_api_buf + OAM_AGNT_INTERFACE_API_HEADER_SIZE);
}

/*************************************************************************
 * Function Name  : oam_agnt_rename_latest_img
 * Description    : Rename the binary image that is being received 
 * Inputs         : Process binary name
 * Outputs        : none
 * Returns        : Success on succesful renaming
 ******************************************************************************/
oam_agnt_return_et
oam_agnt_rename_latest_img(Char8 *p_bin_name)
{
    Char8 *p_tmp_name = OAM_AGNT_NULL;
    oam_agnt_error_code_et err = OAM_AGNT_NO_ERROR;
    SInt16 errno_err = OAM_AGNT_ZERO;

    if (OAM_AGNT_SUCCESS != oam_agnt_mem_alloc(
                oam_agnt_strlen(p_bin_name) + oam_agnt_strlen(OAM_BIN_SOFT_UPGRADE_EXT_STR),
                (void*)&p_tmp_name, &err))
    {
        OAM_AGNT_LOG(AGNT, OAM_AGNT_ERROR,
                "Failed to allocate memory for p_tmp_str. Cannot rename file %s",
                p_bin_name);
        return OAM_AGNT_FAILURE;
    }

    oam_agnt_sprintf(p_tmp_name, "%s%s", p_bin_name, OAM_BIN_SOFT_UPGRADE_EXT_STR);
    if (OAM_AGNT_TRUE == agent_chk_file_exist(p_tmp_name, &errno_err))
    {
        if (OAM_AGNT_ZERO != oam_agnt_rename(p_tmp_name, p_bin_name))
        {
            OAM_AGNT_LOG(AGNT, OAM_AGNT_ERROR, "Failed to rename %s to %s. Errno %u",
                    p_tmp_name, p_bin_name, errno_err);
            return OAM_AGNT_FAILURE;
        }
        else
        {
            OAM_AGNT_LOG(AGNT, OAM_AGNT_INFO, "Successfully renamed %s to %s",
                    p_tmp_name, p_bin_name);
        }
    }

    return OAM_AGNT_SUCCESS;
}
