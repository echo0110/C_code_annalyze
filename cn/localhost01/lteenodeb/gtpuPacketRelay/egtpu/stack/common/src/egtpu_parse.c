
/***********************************************************************
**  FUNCTION:
**	GTP-U Parsing implementation
************************************************************************
**
**  FILE NAME:
**	egtpu_parse.c
**
**  DESCRIPTION:
**	Contains routines to decode incoming APIs
**
**  DATE	    NAME				REFERENCE	REASON
**  ----	    ----				---------	------
**  Oct 2009    Vipul Aggarwal        			Initial
**  14Jan,10    Vipul Aggarwal        			SPR FIX 7447
**
**   Copyright (C) 2009 Aricent Inc . All Rights Reserved
***********************************************************************/

#include <egtpu_glob.h>
#include <egtpu_apihdr.h>
#include <egtpu_error.h>

#include <egtpu_msgtypes.h>
#include <egtpu_ietypes.h>
#include <egtpu_stat.h>
#include <egtpu_glob_struct.h>
#include <egtpu_trace.h>
#include <egtpu_parse.h>
#include <egtpu_cdb.h>
#include <egtpu_util.h>
#include <egtpu_os_proto.h>

/****************************************************************************
 * Function Name  : __send_supp_extn_hdr_noti__
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 * Outputs        : None
 * Returns        : EGTP_SUCCESS/EGTP_FAILURE
 * Description    : This funtion sends the supported extension header
 *                  notification msg.
 ****************************************************************************/
static egtpu_void_t __send_supp_extn_hdr_noti__ (egtpu_global_t *p_global,
														egtpu_info_t *p_egtpu_info)
{
    U8bit *p_buff = p_egtpu_info->p_buff;
    U8bit           *p_start = EGTPU_API_GET_PAYLOAD_STRT_PTR (p_buff),
                    addr_len = 0;
    egtpu_return_t ret_val;

	
	EGTPU_API_SET_SRC_ID (p_buff, EGTPU_EI_GTPU);
	
    addr_len = EGTPU_GET_U16BIT(p_egtpu_info->p_peer_addr);

	
	if (addr_len == EGTPU_SIZE_OF_IPV4_ADDR)
	{
		EGTPU_API_SET_DST_ID (p_buff, EGTPU_EI_UDP);
	}
	else
	{
		EGTPU_API_SET_DST_ID (p_buff, EGTPU_EI_UDP6);
	}


    EGTPU_API_SET_API_ID (p_buff, EGTPU_UDP_UNITDATA_REQ);

    EGTPU_SET_U16BIT(p_buff + EGTPU_API_HDR_LEN + EGTPU_SIZE_OF_TYPE,
					EGTPU_UDP_PORT);

    /* start-payload and end-param are re-used in API header */
    /* GTP message length: 15 = 12 (GTP Header size -> S-flag is always set)
	** + 3 (Extension Header Type List) */
    /* SPR 18822 Fix Start */
    EGTPU_API_SET_MSG_LEN(p_buff, (p_start - p_buff + 
		EGTPU_HDR_LEN_WITH_SEQ_NUM + EGTPU_MIN_SIZE_OF_EXT_HDR_TYPE_LIST+EGTPU_SIZE_OF_TYPE));
    EGTPU_API_SET_END_PAYLOAD(p_buff, (p_start - p_buff +
		EGTPU_HDR_LEN_WITH_SEQ_NUM + EGTPU_MIN_SIZE_OF_EXT_HDR_TYPE_LIST));

    /* Fill the GTP Header: length = 7 
	** (12-8=4:seqno + 3:(Extension Header Type List)) */
    EGTPU_SET_GTP_HDR (p_start, EGTPU_UNSET_EXT_HDR_FLAG,
					EGTPU_SET_SEQ_NUM_FLAG, EGTPU_UNSET_PN_FLAG,
            		EGTPU_MSGT_SUPP_EXT_HDR_NOTI, EGTPU_MIN_SUPP_EXT_HDR_LENGTH+EGTPU_SIZE_OF_TYPE,
					EGTPU_INV_TEID_VALUE, EGTPU_MIN_SEQ_NUM,
					EGTPU_SPARE_BYTE, EGTPU_SPARE_BYTE);

    *(p_start + EGTPU_HDR_LEN_WITH_SEQ_NUM) = EGTPU_IE_EXT_HDR_TYPE_LIST;/* Tag for Extension header type list*/
    *(p_start + EGTPU_HDR_LEN_WITH_SEQ_NUM + EGTPU_SIZE_OF_TYPE) = 2;/* Number of Supported extensionsi i.e length */
    *(p_start + EGTPU_HDR_LEN_WITH_SEQ_NUM + 2) = EGTPU_PDCP_PDU_EXT_HDR;/*PDCP PDU Extension*/
    *(p_start + EGTPU_HDR_LEN_WITH_SEQ_NUM + 3) = EGTPU_UDP_EXT_HDR; /* UDDP Port Extension */
    /* SPR 18822 Fix End */



    if (addr_len == EGTPU_SIZE_OF_IPV4_ADDR)
    {
        EGTPU_SEND_FROM_GTPU (p_global,p_buff,EGTPU_API_GET_MSG_LEN(p_buff),
                              EGTPU_EI_UDP, ret_val, p_global->initdb_info.my_addr[0]);
    }
    else
    {
        EGTPU_SEND_FROM_GTPU (p_global,p_buff,EGTPU_API_GET_MSG_LEN(p_buff),
                              EGTPU_EI_UDP6, ret_val, p_global->initdb_info.my_addr6[0]);
    }
   
     /* + coverity 32074 */
     if (EGTPU_FAILURE == ret_val)
     {
         LOG_GTPU_MSG(GTPU_SEND_FAILURE, LOGWARNING, PR_GTPU_DL_DATA,
                 relayGtpuCurrentTick_g, __LINE__,
                 0,0,0,0, 0,0, __func__, "");
     }
     /* + coverity 32074 */
 
    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_UDP_UNITDATA_REQ);
    EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_SUPP_EXT_HDR_NOTI_MSG_SENT);
}

/****************************************************************************
 * Function Name  : validate_protocol_hdr
 * Inputs         : p_start - points starting point of buffer,
 *                  p_end - points end point of buffer,
 *                  p_error - Pointer to egtpu_error_t,
 *                  p_global - Pointer to global structure egtpu_global_t
 * Outputs        : None
 * Returns        : EGTP_SUCCESS/EGTP_FAILURE
 * Description    : This funtion is used to validate message received i.e its
 *                  header length ,version number etc.
 ****************************************************************************/
static egtpu_return_t validate_protocol_hdr (U8bit *p_start,
											U8bit *p_end,
											egtpu_error_t *p_error,
											egtpu_global_t *p_global)
{
    /* payload length shall atleast be GTP_MSG_HDR_MIN_LEN */
    if ((S32bit) (p_end - p_start + 1) < GTP_MSG_HDR_MIN_LEN)
	{
        *p_error = EGTPU_PERR_MSG_TOO_SHORT;
		EGTPU_UPDATE_STATS (p_global, EGTPU_STAT_ERR_GTP_MESSAGE_TOO_SHORT);
        /*SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_PAYLOAD_LESS_THAN_HDR_LEN, LOGDEBUG, PR_GTPU_DL_DATA,
             relayGtpuCurrentTick_g, __LINE__, 
        GTP_MSG_HDR_MIN_LEN,(S32bit) (p_end - p_start + 1),0,0, 0,0, __func__,
        "");
        /* SPR 2030 fix End*/
        return EGTPU_FAILURE;
    }

    /* Version Number: shall be v1 */
    if (EGTPU_GET_VER_NUM(p_start) != GTP_VERSION)
	{
        *p_error = EGTPU_PERR_VERSION_NOT_SUPP;
        /*SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_VERSION_INCORRECT, LOGDEBUG, PR_GTPU_DL_DATA,
             relayGtpuCurrentTick_g, __LINE__, 
        GTP_VERSION,EGTPU_GET_VER_NUM(p_start),0,0, 0,0, __func__,"");
        /* SPR 2030 fix End*/
        return EGTPU_FAILURE;
    }

    /* PT FLag: shall be 1 */
    if (EGTPU_GET_PT_FLAG (p_start) != EGTPU_PROTOCOL_TYPE)
	{
        *p_error = EGTPU_PERR_PROTO_TYPE_INCORRECT;
        /*SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_PROTOCOLTYPE_INCORRECT, LOGDEBUG, PR_GTPU_DL_DATA,
             relayGtpuCurrentTick_g, __LINE__, 
        EGTPU_PROTOCOL_TYPE,EGTPU_GET_PT_FLAG (p_start),0,0, 0,0, __func__,"");
        /* SPR 2030 fix End*/
        return EGTPU_FAILURE;
    }

    return EGTPU_SUCCESS;
}

/****************************************************************************
 * Function Name  : skip_extn_hdr
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t,
 *                  p_start - Points to starting point of buffer after header,
 *                  p_end - Points to end point of buffer
 * Outputs        : None
 * Returns        : EGTP_SUCCESS/EGTP_FAILURE
 * Description    : This funtion skips the extn header present in header.
 ****************************************************************************/
/* returns failure when extn header goes beyond the payload
   else it is success */
static egtpu_return_t skip_extn_hdr (egtpu_global_t *p_global,
									egtpu_info_t *p_egtpu_info,
									U8bit *p_start, U8bit *p_end)
{
    U8bit *p_tmp = p_start + EGTPU_HDR_LEN_WITH_SEQ_NUM;

    do {
        if (*(p_tmp - 1))	/* Check the Next Extn Hdr Type value */
		{
			/** Verify the length in the extn header.
			**  Length is in multiple of 4 octets */
            if (!(*p_tmp) || ((p_tmp += 4 * (*p_tmp)) > (p_end + 1)))
			{
                /* log error and discard the packet */
                EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_INV_MSG_FORMAT,
                            egtpu_entity_name_arr[EGTPU_EI_GTPU],
                            egtpu_get_msg_name (p_egtpu_info->pdu_type)));

                /* SPR 17747 FIX START */
                LOG_MSG(GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON,
                        relayGtpuCurrentTick_g, __LINE__, 
                        EGTPU_PERR_INV_MSG_FORMAT, p_egtpu_info->pdu_type,
                        0,0,0,0, __func__, egtpu_entity_name_arr[EGTPU_EI_GTPU] );
                /* SPR 17747 FIX END */

               /* SPR 2007 Fix - Part2 Start*/ 
                /*EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);*/
               /* SPR 2007 Fix - Part2 End*/
                return EGTPU_FAILURE;
            }
        }
		else
            break;
    } while(EGTPU_TRUE);

    p_egtpu_info->hdr_len += (p_tmp - (p_start + EGTPU_HDR_LEN_WITH_SEQ_NUM));
    return EGTPU_SUCCESS;
}

/*******************************************************************************
 * Function Name  : dec_extn_hdr
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t,
 *                  p_start - Points to starting point of buffer after header,
 *                  p_end - Points to end point of buffer
 * Outputs        : None
 * Returns        : EGTP_SUCCESS/EGTP_FAILURE
 * Description    : This funtion returns failure when -
 *                  extn header goes beyond the payload
 *                  not supported and comprehension reqd extn hdr type recd
 *                  else it is success
 *
 *                  in case of failure, error shall be logged, buffer is freed
 *                  in case of success, pdcp_num will be read
 *                  nd EGTPU_IE_PDCP_NUM_FLAG is set in ie_presence flag.
 *******************************************************************************/
static egtpu_return_t dec_extn_hdr (egtpu_global_t *p_global,
									egtpu_info_t *p_egtpu_info,
									U8bit *p_start, U8bit *p_end)
{
    U8bit ext_hdr, *p_tmp = p_start + EGTPU_HDR_LEN_WITH_SEQ_NUM;

    do {
        if ((ext_hdr = *(p_tmp - 1))) /* Check the Next Extn Hdr Type value */
		{
			/** Verify the length in the extn header.
			**  Length is in multiple of 4 octets */
            if (!(*p_tmp) || ((p_tmp + 4 * (*p_tmp)) > (p_end + 1)))
			{
                /* log error and discard the packet */
                EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_INV_MSG_FORMAT,
                            egtpu_entity_name_arr[EGTPU_EI_GTPU],
                            egtpu_get_msg_name (p_egtpu_info->pdu_type)));
                
                /* SPR 17747 FIX START */
                LOG_MSG( GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON,
                        relayGtpuCurrentTick_g, __LINE__,
                        EGTPU_PERR_INV_MSG_FORMAT, p_egtpu_info->pdu_type,
                        0,0,0,0,__func__, egtpu_entity_name_arr[EGTPU_EI_GTPU]);
                /* SPR 17747 FIX END */
                /* SPR 2007 Fix - Part2 Start*/
                /*EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);*/
               /* SPR 2007 Fix - Part2 End*/
                return EGTPU_FAILURE;
            }

            if (ext_hdr == EGTPU_PDCP_PDU_EXT_HDR)
			{
                if (*p_tmp != EGTPU_LEN_OF_PDCP_PDU_EXT_HDR)
				{
                    /* len is not 1, log error and discard the packet */
                    EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_INV_MSG_FORMAT,
                                egtpu_entity_name_arr[EGTPU_EI_GTPU],
                                egtpu_get_msg_name
                                (p_egtpu_info->pdu_type)));

                    /* SPR 17747 FIX START */
                    LOG_MSG( GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON,
                            relayGtpuCurrentTick_g, __LINE__,
                            EGTPU_PERR_INV_MSG_FORMAT, p_egtpu_info->pdu_type,
                            0,0,0,0,__func__, egtpu_entity_name_arr[EGTPU_EI_GTPU]);
                    /* SPR 17747 FIX END */

                    /* SPR 2007 Fix - Part2 Start*/
                    /*EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);*/
                    /* SPR 2007 Fix - Part2 End*/
                    return EGTPU_FAILURE;
                }

                p_egtpu_info->ie_presence |= EGTPU_IE_PDCP_NUM_FLAG;
                p_egtpu_info->pdcp_num = EGTPU_GET_U16BIT(p_tmp + 1);
            }
#ifdef ENDC_ENABLED
			else if (ext_hdr == EGTPU_EXT_HDR_NR_RAN)
			{
				break;
			}
#endif
			else if (ext_hdr & EGTPU_EXT_HDR_COMPREHENSION_REQD)
			{ /* comprehension reqd */
                EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_EXT_HDR_NOT_SUPP,
                            egtpu_entity_name_arr[EGTPU_EI_GTPU],
                            ext_hdr));

                /* SPR 17747 FIX START */
                LOG_MSG( GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON,
                        relayGtpuCurrentTick_g, __LINE__,
                        EGTPU_PERR_INV_MSG_FORMAT, ext_hdr,0,0,0,0,
                        __func__, egtpu_entity_name_arr[EGTPU_EI_GTPU]);
                /* SPR 17747 FIX END */

                __send_supp_extn_hdr_noti__ (p_global,p_egtpu_info);
                return EGTPU_FAILURE;
            }

            p_tmp += (4 * (*p_tmp)); /** length is in multiple of 4 octets */
        }
		else
            break;
    } while (EGTPU_TRUE);

    p_egtpu_info->hdr_len += (p_tmp - (p_start + EGTPU_HDR_LEN_WITH_SEQ_NUM));
    return EGTPU_SUCCESS;
}

/****************************************************************************
 * Function Name  : __dec_tpdu__
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t,
 *                  p_start - Points to starting point of buffer after header,
 *                  p_end - Points to end point of buffer
 * Outputs        : None
 * Returns        : EGTP_SUCCESS/EGTP_FAILURE
 * Description    : This funtion takes the API message received and extract
 *                  relevant information about seq no , PN and N_PDU no and
 *                  store these value in p_egtpu_info structure.
 ****************************************************************************/
static egtpu_return_t __dec_tpdu__ (egtpu_global_t *p_global,
									egtpu_info_t *p_egtpu_info,
									U8bit *p_start, U8bit *p_end)
{
    p_egtpu_info->teid_self = EGTPU_GET_TEID (p_start); /* extract teid from buffer*/

    if (EGTPU_GET_EXT_FLAG (p_start) &&
            dec_extn_hdr (p_global,p_egtpu_info, p_start, p_end))
	{
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_TPDU_EXT_HDR_DECODE_FAILURE, LOGWARNING, PR_GTPU_DL_DATA,
                relayGtpuCurrentTick_g, __LINE__,0,0,0,0,
                0,0, __func__,""); 
        /* SPR 2030 fix End*/
        return EGTPU_FAILURE;  /* error has been logged */
	}

    if (EGTPU_GET_SEQ_FLAG (p_start))
	{
        p_egtpu_info->seq_num_dnlk = EGTPU_GET_SEQ_NUM (p_start);
        p_egtpu_info->ie_presence |= EGTPU_IE_SEQ_NUM_FLAG;
    }

    if (EGTPU_GET_PN_FLAG (p_start))
	{
        p_egtpu_info->n_pdu_num = EGTPU_GET_NPDU_NUM (p_start);
        p_egtpu_info->ie_presence |= EGTPU_IE_NPDU_NUM_FLAG;
    }

    return EGTPU_SUCCESS;
}

/****************************************************************************
 * Function Name  : __dec_echo_req__
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t,
 *                  p_start - Points to starting point of buffer after header
 * Outputs        : None
 * Returns        : EGTP_SUCCESS/EGTP_FAILURE
 * Description    : This funtion takes the API message received for echo req
 *                  and extract downlink seq no if S flag is  set in header.
 ****************************************************************************/
static egtpu_return_t __dec_echo_req__ (egtpu_global_t *p_global,
										egtpu_info_t *p_egtpu_info,
										U8bit *p_start)
{
    if (EGTPU_GET_SEQ_FLAG (p_start))
        p_egtpu_info->seq_num_dnlk = EGTPU_GET_SEQ_NUM (p_start);
    else
	{
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_ECHO_REQ_INV_FORMAT, LOGWARNING, PR_GTPU_DL_DATA,
                relayGtpuCurrentTick_g, __LINE__,0,0,0,0,
                0,0, __func__,""); 
        /* SPR 2030 fix End*/

        /* log the error... no rsp to peer */
        EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_INV_MSG_FORMAT,
                    egtpu_entity_name_arr[EGTPU_EI_GTPU],
                    egtpu_get_msg_name (p_egtpu_info->pdu_type)));

        /* SPR 17747 FIX START */
        LOG_MSG( GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON,
                relayGtpuCurrentTick_g, __LINE__,
                EGTPU_PERR_INV_MSG_FORMAT, p_egtpu_info->pdu_type,
                0,0,0,0, __func__, egtpu_entity_name_arr[EGTPU_EI_GTPU] );
        /* SPR 17747 FIX END */
        /* SPR 2007 Fix Start */
        /* EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);*/
        /* SPR 2007 Fix End */
        return EGTPU_FAILURE;
    }

    return EGTPU_SUCCESS;
}

/****************************************************************************
 * Function Name  : __dec_echo_rsp__
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t,
 *                  p_start - Points to starting point of buffer after header,
 *                  p_end - Points to end point of buffer
 * Outputs        : None
 * Returns        : EGTP_SUCCESS/EGTP_FAILURE
 * Description    : This funtion takes the API message received for echo
 *                  response & validate message.
 ****************************************************************************/
static egtpu_return_t __dec_echo_rsp__ (egtpu_global_t *p_global,
										egtpu_info_t *p_egtpu_info,
										U8bit *p_start, U8bit *p_end)
{
    U8bit *p_ie, prev_type = EGTPU_FALSE, ie_type;
    U8bit out_of_seq = EGTPU_FALSE,
          unknown_ie = EGTPU_FALSE,
          len_insufficient = EGTPU_FALSE;
    U8bit recovery_flag = EGTPU_FALSE;
    U8bit error = EGTPU_FALSE;

    if (EGTPU_GET_EXT_FLAG (p_start) &&  
            skip_extn_hdr (p_global,p_egtpu_info, p_start, p_end))
	{
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_ECHO_RESP_EXT_HDR_DECODE_FAILURE, LOGWARNING, PR_GTPU_DL_DATA,
                relayGtpuCurrentTick_g, __LINE__,0,0,0,0,
                0,0, __func__,"");
        /* SPR 2030 fix End*/
        return EGTPU_FAILURE;  /* error has been logged */
	}

    if (!EGTPU_GET_SEQ_FLAG (p_start))
	{
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_ECHO_RESP_INV_FORMAT, LOGWARNING, PR_GTPU_DL_DATA,
                relayGtpuCurrentTick_g, __LINE__,0,0,0,0,
                0,0, __func__,""); 
        /* SPR 2030 fix End*/

        /* log the error... */
        EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_INV_MSG_FORMAT,
                    egtpu_entity_name_arr[EGTPU_EI_GTPU],
                    egtpu_get_msg_name (p_egtpu_info->pdu_type)));


        /* SPR 17747 FIX START */
        LOG_MSG( GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON,
                relayGtpuCurrentTick_g, __LINE__,
                EGTPU_PERR_INV_MSG_FORMAT, p_egtpu_info->pdu_type,
                0,0,0,0, __func__, egtpu_entity_name_arr[EGTPU_EI_GTPU] );
        /* SPR 17747 FIX END */
        /* SPR 2007 Fix - Part2 Start*/
        /*EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);*/
        /* SPR 2007 Fix - Part2 End*/
        return EGTPU_FAILURE;
    }

    p_ie = p_start + p_egtpu_info->hdr_len;
    while (p_ie < p_end)
	{
        if ((ie_type = *p_ie++) < prev_type)
		{
            out_of_seq = EGTPU_TRUE;
            break;
        }
		else
            prev_type = ie_type;

        switch (ie_type)
		{
            case EGTPU_IE_TEID_DATA_I:
                p_ie += EGTPU_SIZE_OF_TEID;
                break;

            case EGTPU_IE_GSN_ADDR:
                p_ie += (EGTPU_GET_U16BIT (p_ie) + EGTPU_SIZE_OF_LENGTH);
                break;

            case EGTPU_IE_RECOVERY:
              /* SPR 6935 Fix start: if this IE type is received than 
                 recovery flag should be set as true in all conditions*/
                 recovery_flag = EGTPU_TRUE;
                 p_ie = p_end;
              /* SPR 6935 Fix end*/
                 break;

            case EGTPU_IE_EXT_HDR_TYPE_LIST:
                p_ie += (*p_ie + 1);
                break;

            default:
                unknown_ie = EGTPU_TRUE;

                if (ie_type & 0x80)      /* unknown tlv IE */
                    p_ie += (EGTPU_GET_U16BIT(p_ie) + EGTPU_SIZE_OF_LENGTH);
                else
                    p_ie = p_end;        /* unknown tv IE */
        }
      /*SPR 6935 Fix start*/
      if (p_ie > p_end)
      {    
          len_insufficient = EGTPU_TRUE;
      }   
      /*SPR 6935 Fix end */

    }

    if (out_of_seq)
        error = EGTPU_PERR_INV_MSG_FORMAT;
    else if (!recovery_flag)
	{
        if (unknown_ie || len_insufficient)
            error = EGTPU_PERR_INV_MSG_FORMAT;
        else
            error = EGTPU_PERR_MAND_IE_MISSING;
    }

    if (error)
	{
        /* log error and discard the packet */
        EGTPU_ERROR_REPORTER (p_global,(egtpu_error_array[error],
                    egtpu_entity_name_arr[EGTPU_EI_GTPU],
                    egtpu_get_msg_name (p_egtpu_info->pdu_type)));

        /* SPR 17747 FIX START */
        LOG_MSG( GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON,
                relayGtpuCurrentTick_g, __LINE__, error, 
                p_egtpu_info->pdu_type,0,0,0,0, __func__, 
                egtpu_entity_name_arr[EGTPU_EI_GTPU] );
        /* SPR 17747 FIX END */
        /* SPR 2007 Fix - Part2 Start*/
        /*EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);*/
        /* SPR 2007 Fix - Part2 End*/
        return EGTPU_FAILURE;
    }

    return EGTPU_SUCCESS;
}

/****************************************************************************
 * Function Name  : __dec_supp_extn_hdr_noti__
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t,
 *                  p_start - Points to starting point of buffer after header,
 *                  p_end - Points to end point of buffer
 * Outputs        : None
 * Returns        : EGTP_SUCCESS/EGTP_FAILURE
 * Description    : This funtion takes the message received for supported
 *                  extension header and validates it. Also, if PDCP PDU
 *                  extn hdr is not present, then downgrades the extn header
 *                  list for the peer.
 ****************************************************************************/
static egtpu_return_t __dec_supp_extn_hdr_noti__ (egtpu_global_t *p_global,
												egtpu_info_t *p_egtpu_info,
												U8bit *p_start, U8bit *p_end)
{
    U8bit *p_ie, prev_type = EGTPU_FALSE, ie_type;
    U8bit out_of_seq = EGTPU_FALSE,
          unknown_ie = EGTPU_FALSE,
          len_insufficient = EGTPU_FALSE;
    U8bit extn_hdr_flag = EGTPU_FALSE;
    U8bit error = EGTPU_FALSE, i;
    /* + SPR 17435 */
    U8bit ext_hdr = 0;
    /* - SPR 17435 */
    /* SPR 18822 Fix Start */
    egtpu_peer_ctxt_t *p_ctxt =
        egtpu_db_sel_addr_key_peer_ctxt
        (p_egtpu_info->p_peer_addr);
    /* SPR 18822 Fix End */


    if (EGTPU_GET_EXT_FLAG (p_start) && 
            skip_extn_hdr (p_global,p_egtpu_info, p_start, p_end))
	{
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_EXT_HDR_NOTI_DECODE_FAILURE, LOGWARNING, PR_GTPU_DL_DATA,
                relayGtpuCurrentTick_g, __LINE__,0,0,0,0,
                0,0, __func__,"");
        /* SPR 2030 fix End*/
        return EGTPU_FAILURE;  /* error has been logged */
	}

    if (!EGTPU_GET_SEQ_FLAG (p_start))
	{
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_EXT_HDR_NOTI_INV_FORMAT, LOGWARNING, PR_GTPU_DL_DATA,
                relayGtpuCurrentTick_g, __LINE__,0,0,0,0,
                0,0, __func__,""); 
        /* SPR 2030 fix End*/

        /* log the error... no rsp to peer */
        EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_INV_MSG_FORMAT,
                    egtpu_entity_name_arr[EGTPU_EI_GTPU],
                    egtpu_get_msg_name (p_egtpu_info->pdu_type)));
        
        /* SPR 17747 FIX START */
        LOG_MSG( GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON,
                relayGtpuCurrentTick_g, __LINE__, EGTPU_PERR_INV_MSG_FORMAT, 
                p_egtpu_info->pdu_type,0,0,0,0, __func__, 
                egtpu_entity_name_arr[EGTPU_EI_GTPU] );
       /* SPR 17747 FIX END */
        /* SPR 2007 Fix - Part2 Start*/
        /*EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);*/
        /* SPR 2007 Fix - Part2 End*/
        return EGTPU_FAILURE;
    }

    p_ie = p_start + p_egtpu_info->hdr_len;
    while (p_ie < p_end)
	{
        if ((ie_type = *p_ie++) < prev_type)
		{
            out_of_seq = EGTPU_TRUE;
            break;
        }
		else
            prev_type = ie_type;

        switch (ie_type)
		{
            case EGTPU_IE_TEID_DATA_I:
                p_ie += EGTPU_SIZE_OF_TEID;
                break;

            case EGTPU_IE_GSN_ADDR:
                p_ie += (EGTPU_GET_U16BIT(p_ie) + EGTPU_SIZE_OF_LENGTH);
                break;

            case EGTPU_IE_RECOVERY:
                p_ie++;
                break;

            case EGTPU_IE_EXT_HDR_TYPE_LIST:
                if (!extn_hdr_flag)
				{
                    if ((p_ie + *p_ie + 1) <= (p_end + 1))
					{
                        extn_hdr_flag = EGTPU_TRUE;
                        for (i = 1; i <= *p_ie; ++i)
                        {
                            /* + SPR 17435 */
                            if (*(p_ie + i) == EGTPU_PDCP_PDU_EXT_HDR)
                            {
                                ext_hdr |= EGTPU_EXT_HDR_PDCP;
                            }
                            else if( EGTPU_UDP_EXT_HDR ==  *(p_ie + i) )
                            {
                                ext_hdr |= EGTPU_EXT_HDR_UDP;
                            }
                            else
                            {
                                /* error */
                            }
                        }
                        /* SPR 18822 Fix Start */
                        /* if any of the extension header list field is set by 
                         * peer unit */
                        if (p_ctxt)
                        {
                            /* Supported Extension Header of Peer will be updated with the
                             * Values received in Extension Header Notification.By Default
                             * We are intialising the supported Extension Headers to 0x03
                             * i.e PDCP PDU extension and UDP Port Extension */
                            p_ctxt->ext_hdr = ext_hdr;
                        }
                        else
                        {
                            EGTPU_ERROR_REPORTER (p_global,
                                    (EGTPU_TS_PERR_UNEXPECTED_MSG,
                                     egtpu_entity_name_arr[EGTPU_EI_GTPU],
                                     egtpu_get_msg_name (EGTPU_MSGT_SUPP_EXT_HDR_NOTI)));

                            /* SPR 17747 FIX START */
                            LOG_MSG( GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON,
                                    relayGtpuCurrentTick_g, __LINE__, 
                                    EGTPU_PERR_UNEXPECTED_MSG, 
                                    EGTPU_MSGT_SUPP_EXT_HDR_NOTI,0,0,0,0, __func__, 
                                    egtpu_entity_name_arr[EGTPU_EI_GTPU] );
                            /* SPR 17747 FIX END */

                            EGTPU_UPDATE_STATS (p_global,
                                    EGTPU_STAT_ERR_UNEXPECTED_GTP_SIGNALLING_MESSAGE);	
                        }	
                        /* SPR 18822 Fix End */
                        /* - SPR 17435 */
                	}
					else
                        len_insufficient = EGTPU_TRUE;
            	}

                p_ie += (*p_ie + 1);
                break;

            default:
                unknown_ie = EGTPU_TRUE;

                if (ie_type & 0x80)      /* unknown tlv IE */
                    p_ie += (EGTPU_GET_U16BIT (p_ie) + EGTPU_SIZE_OF_LENGTH);
                else
                    p_ie = p_end;        /* unknown tv IE */
        } /* end of switch case for ie_type present*/
    } /* end of while loop for buffer pointers check*/

    if (out_of_seq)
        error = EGTPU_PERR_INV_MSG_FORMAT;
    else if (!extn_hdr_flag)
	{
        if (unknown_ie || len_insufficient)
            error = EGTPU_PERR_INV_MSG_FORMAT;
        else
            error = EGTPU_PERR_MAND_IE_MISSING;
    }

    if (error)
	{ /* if error, log trace and discard the packet */
        EGTPU_ERROR_REPORTER (p_global,(egtpu_error_array[error],
                    egtpu_entity_name_arr[EGTPU_EI_GTPU],
                    egtpu_get_msg_name (p_egtpu_info->pdu_type)));

        /* SPR 17747 FIX START */
        LOG_MSG( GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON,
                relayGtpuCurrentTick_g, __LINE__, error, 
                p_egtpu_info->pdu_type,0,0,0,0, __func__, 
                egtpu_entity_name_arr[EGTPU_EI_GTPU] );
        /* SPR 17747 FIX END */

    }

    /* update stats as FSM won't be entered in this case */
    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_UNITDATA_IND);
    EGTPU_UPDATE_STATS (p_global,EGTPU_STAT_SUPP_EXT_HDR_NOTI_MSG_RCVD);
    /* SPR 2007 Fix - Part2 Start*/
    /*EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);*/
    /* SPR 2007 Fix - Part2 End*/
    return EGTPU_FAILURE; /* even in success, no need to trouble fsm */
}

/****************************************************************************
 * Function Name  : __dec_err_ind__
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t,
 *                  p_start - Points to starting point of buffer after header,
 *                  p_end - Points to end point of buffer
 * Outputs        : None
 * Returns        : EGTP_SUCCESS/EGTP_FAILURE
 * Description    : This funtion takes the API message received and increment
 *                  buffer starting point by the total length of ie_types
 *                  present in buffer.
 ****************************************************************************/
static egtpu_return_t __dec_err_ind__ (egtpu_global_t *p_global,
										egtpu_info_t *p_egtpu_info,
										U8bit *p_start, U8bit *p_end)
{
    /* SPR 5599_128826 Fix Start */
    U8bit *p_ie, /* *p_trav , */ prev_type = EGTPU_FALSE, ie_type, addr_len;
    /* SPR 5599_128826 Fix End */
    U8bit out_of_seq = EGTPU_FALSE,
          unknown_ie = EGTPU_FALSE,
          len_insufficient = EGTPU_FALSE;
    U8bit teid_flag = EGTPU_FALSE,
          addr_flag = EGTPU_FALSE;
    U8bit error = EGTPU_FALSE;
    if (EGTPU_GET_EXT_FLAG (p_start) &&
            skip_extn_hdr (p_global,p_egtpu_info, p_start, p_end))
	{
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_ERR_IND_EXT_HDR_DECODE_FAILURE, LOGWARNING, PR_GTPU_DL_DATA,
                relayGtpuCurrentTick_g, __LINE__,0,0,0,0,
                0,0, __func__,"");
        /* SPR 2030 fix End*/
        return EGTPU_FAILURE;  /* error has been logged */
	}
	
	if (!EGTPU_GET_SEQ_FLAG (p_start))
	{
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_ERR_IND_INV_FORMAT, LOGWARNING, PR_GTPU_DL_DATA,
                relayGtpuCurrentTick_g, __LINE__,0,0,0,0,
                0,0, __func__,"");
        /* SPR 2030 fix End*/
        /* log the error... */
        EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_INV_MSG_FORMAT,
                    egtpu_entity_name_arr[EGTPU_EI_GTPU],
                    egtpu_get_msg_name (p_egtpu_info->pdu_type)));

        /* SPR 17747 FIX START */
        LOG_MSG( GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON,
                relayGtpuCurrentTick_g, __LINE__, EGTPU_PERR_INV_MSG_FORMAT, 
                p_egtpu_info->pdu_type,0,0,0,0, __func__, 
                egtpu_entity_name_arr[EGTPU_EI_GTPU] );
        /* SPR 17747 FIX END */
        /* SPR 2007 Fix - Part2 Start*/
        /*EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);*/
        /* SPR 2007 Fix - Part2 End*/
        return EGTPU_FAILURE;
    }

    if (EGTPU_GET_SEQ_FLAG (p_start))
	{
        p_egtpu_info->seq_num_dnlk = EGTPU_GET_SEQ_NUM (p_start);
        p_egtpu_info->ie_presence |= EGTPU_IE_SEQ_NUM_FLAG;
    }

    p_ie = p_start + p_egtpu_info->hdr_len;
    while (p_ie < p_end)
	{
        if ((ie_type = *p_ie++) < prev_type)
		{
            out_of_seq = EGTPU_TRUE;
            break;
        }
		else
            prev_type = ie_type;

        switch (ie_type)
		{
            case EGTPU_IE_TEID_DATA_I:
                if (!teid_flag)
				{ /* see if payld is enough for teid */
                    if ((p_ie + EGTPU_SIZE_OF_TEID) <= (p_end + 1))
					{
                        teid_flag = EGTPU_TRUE;
                        p_egtpu_info->teid_peer = EGTPU_GET_U32BIT (p_ie);
                    }
					else
                        len_insufficient = EGTPU_TRUE;
                }

                p_ie += EGTPU_SIZE_OF_TEID;
                break;

            case EGTPU_IE_GSN_ADDR:
                if (!addr_flag)
				{ /* see if payld is enough for addr */
                        addr_len = EGTPU_GET_U16BIT(p_ie);
					if( (addr_len !=  EGTPU_SIZE_OF_IPV4_ADDR) && (addr_len !=  EGTPU_SIZE_OF_IPV6_ADDR) )
					{
						error = EGTPU_PERR_MAND_IE_INCORRECT;
						EGTPU_ERROR_REPORTER (p_global,(egtpu_error_array[error],
						egtpu_entity_name_arr[EGTPU_EI_GTPU],
						egtpu_get_msg_name (p_egtpu_info->pdu_type)));

                        /* SPR 17747 FIX START */
                        LOG_MSG( GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON,
                                relayGtpuCurrentTick_g, __LINE__, error, 
                                p_egtpu_info->pdu_type,0,0,0,0, __func__, 
                                egtpu_entity_name_arr[EGTPU_EI_GTPU] );
                        /* SPR 17747 FIX END */

					}

                    if((p_ie + (EGTPU_GET_U16BIT(p_ie) + EGTPU_SIZE_OF_LENGTH))
						<= (p_end + 1))
					{
                        addr_flag = EGTPU_TRUE;
                        p_egtpu_info->p_peer_addr = p_ie;
                    }
					else
                        len_insufficient = EGTPU_TRUE;
                }

                p_ie += (EGTPU_GET_U16BIT (p_ie) + EGTPU_SIZE_OF_LENGTH);
                break;

            case EGTPU_IE_RECOVERY:
                p_ie++;
                break;

            case EGTPU_IE_EXT_HDR_TYPE_LIST:
                p_ie += (*p_ie + 1);
                break;
            case EGTPU_IE_PRIVATE_EXTENSION:
                if((p_ie + (EGTPU_GET_U16BIT(p_ie) + EGTPU_SIZE_OF_LENGTH))
					<= (p_end + 1))
				{
        			p_egtpu_info->ie_presence |= EGTPU_IE_PVT_EXTN_FLAG;
                    p_egtpu_info->p_pvt_extn = p_ie;
                }
				else
                    len_insufficient = EGTPU_TRUE;

                p_ie += (EGTPU_GET_U16BIT (p_ie) + EGTPU_SIZE_OF_LENGTH);
                break;
            default:
                unknown_ie = EGTPU_TRUE;

                if (ie_type & 0x80)      /* unknown tlv IE */
                    p_ie += (EGTPU_GET_U16BIT (p_ie) + EGTPU_SIZE_OF_LENGTH);
                else
                    p_ie = p_end;        /* unknown tv IE */
        }
    }

    if (out_of_seq)
        error = EGTPU_PERR_INV_MSG_FORMAT;
    else if (!teid_flag || !addr_flag)
	{
        if (unknown_ie || len_insufficient)
            error = EGTPU_PERR_INV_MSG_FORMAT;
        else
            error = EGTPU_PERR_MAND_IE_MISSING;
    }

    if (error)
	{
        /* log error and discard the packet */
        EGTPU_ERROR_REPORTER (p_global,(egtpu_error_array[error],
                    egtpu_entity_name_arr[EGTPU_EI_GTPU],
                    egtpu_get_msg_name (p_egtpu_info->pdu_type)));

        /* SPR 17747 FIX START */
        LOG_MSG( GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON,
                relayGtpuCurrentTick_g, __LINE__, error, 
                p_egtpu_info->pdu_type,0,0,0,0, __func__, 
                egtpu_entity_name_arr[EGTPU_EI_GTPU] );
        /* SPR 17747 FIX END */

        /* SPR 2007 Fix - Part2 Start*/
        /* EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);*/
        /* SPR 2007 Fix - Part2 End*/
        return EGTPU_FAILURE;
    }

    return EGTPU_SUCCESS;
}

/****************************************************************************
 * Function Name  : __dec_end_marker__
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t,
 *                  p_start - Points to starting point of buffer after header,
 *                  p_end - Points to end point of buffer
 * Outputs        : None
 * Returns        : EGTP_SUCCESS/EGTP_FAILURE
 * Description    : This funtion takes the API message received and increment
 *                  buffer starting point by the total length of ie_types
 *                  present in buffer.
 ****************************************************************************/
static egtpu_return_t __dec_end_marker__ (egtpu_global_t *p_global,
										egtpu_info_t *p_egtpu_info,
										U8bit *p_start, U8bit *p_end)
{
    /* SPR 5599_129051 Fix Start */
    U8bit *p_ie, /* *p_trav,*/ prev_type = EGTPU_FALSE, ie_type;
    /* SPR 5599_129051 Fix End */
    U8bit out_of_seq = EGTPU_FALSE,
          len_insufficient = EGTPU_FALSE;
    U8bit error = EGTPU_FALSE;

    p_egtpu_info->teid_self = EGTPU_GET_TEID(p_start);

    if (EGTPU_GET_EXT_FLAG (p_start) &&
            skip_extn_hdr (p_global,p_egtpu_info, p_start, p_end))
	{
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_END_MARKER_EXT_HDR_DECODE_FAILURE, LOGWARNING, 
                PR_GTPU_DL_DATA,
                relayGtpuCurrentTick_g, __LINE__,0,0,0,0,
                0,0, __func__,"");
        /* SPR 2030 fix End*/
        return EGTPU_FAILURE;  /* error has been logged */
	}

    /** SPR FIX 7447 Start **/	
    if (EGTPU_GET_SEQ_FLAG (p_start))
	{
        p_egtpu_info->seq_num_dnlk = EGTPU_GET_SEQ_NUM(p_start);
        p_egtpu_info->ie_presence |= EGTPU_IE_SEQ_NUM_FLAG;
    }
    /** SPR FIX 7447 Stop **/	

    p_ie = p_start + p_egtpu_info->hdr_len;
    while (p_ie < p_end)
	{
        if ((ie_type = *p_ie++) < prev_type)
		{
            out_of_seq = EGTPU_TRUE;
            break;
        }
		else
            prev_type = ie_type;

        switch (ie_type)
		{
            case EGTPU_IE_TEID_DATA_I:
                p_ie += EGTPU_SIZE_OF_TEID;
                break;

            case EGTPU_IE_GSN_ADDR:
                p_ie += (EGTPU_GET_U16BIT(p_ie) + EGTPU_SIZE_OF_LENGTH);
                break;

            case EGTPU_IE_RECOVERY:
                p_ie++;
                break;

            case EGTPU_IE_EXT_HDR_TYPE_LIST:
                p_ie += (*p_ie + 1);
                break;

            case EGTPU_IE_PRIVATE_EXTENSION:
                if((p_ie + (EGTPU_GET_U16BIT(p_ie) + EGTPU_SIZE_OF_LENGTH))
					<= (p_end + 1))
				{
        			p_egtpu_info->ie_presence |= EGTPU_IE_PVT_EXTN_FLAG;
                    p_egtpu_info->p_pvt_extn = p_ie;
                }
				else
                    len_insufficient = EGTPU_TRUE;

                p_ie += (EGTPU_GET_U16BIT(p_ie) + EGTPU_SIZE_OF_LENGTH);
                break;

            default:

                if (ie_type & 0x80)      /* unknown tlv IE */
                    p_ie += (EGTPU_GET_U16BIT(p_ie) + EGTPU_SIZE_OF_LENGTH);
                else
                    p_ie = p_end;        /* unknown tv IE */
        }
    }

    if(out_of_seq || len_insufficient)
        error = EGTPU_PERR_INV_MSG_FORMAT;

    if (error)
	{
        /* log error and discard the packet */
        EGTPU_ERROR_REPORTER (p_global,(egtpu_error_array[error],
                    egtpu_entity_name_arr[EGTPU_EI_GTPU],
                    egtpu_get_msg_name (p_egtpu_info->pdu_type)));

        /* SPR 17747 FIX START */
        LOG_MSG( GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON,
                relayGtpuCurrentTick_g, __LINE__, error, 
                p_egtpu_info->pdu_type,0,0,0,0, __func__, 
                egtpu_entity_name_arr[EGTPU_EI_GTPU] );
        /* SPR 17747 FIX END */

        /* SPR 2007 Fix - Part2 Start*/
        /* EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);*/
        /* SPR 2007 Fix - Part2 End*/
        return EGTPU_FAILURE;
    }

    return EGTPU_SUCCESS;
}

/****************************************************************************
 * Function Name  : egtpu_parse_msg
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 * Outputs        : None
 * Returns        : EGTP_SUCCESS/EGTP_FAILURE
 * Description    : This funtion takes the API message received and extract
 *                  relevant information from the buffer and store the
 *                  information in structure egtpu_info_t.
 ****************************************************************************/
egtpu_return_t egtpu_parse_msg (egtpu_global_t *p_global,
									egtpu_info_t *p_egtpu_info)
{
	U8bit    *p_buffer = p_egtpu_info->p_buff;
        /* SPR 4357 Fix Start */
        U8bit    *p_start = NULL;/*EGTPU_API_GET_PAYLOAD_STRT_PTR (p_buffer),*/
		U8bit    *p_end = NULL;/*EGTPU_API_GET_PAYLOAD_END_PTR (p_buffer);*/
	U16bit    start = EGTPU_API_GET_START_PAYLOAD (p_buffer);
	U16bit    end = EGTPU_API_GET_END_PAYLOAD (p_buffer);
	U16bit   pdu_len, payload_len;
	egtpu_error_t error;

    p_start = (U8bit *)(p_buffer + start);
    p_end   = (U8bit *)(p_buffer + end);
	
    p_egtpu_info->pdu_type = EGTPU_GET_MSG_TYPE (p_start);
	pdu_len = EGTPU_GET_MSG_LEN (p_start) + GTP_MSG_HDR_MIN_LEN;
        /*SPR 3587*/
	/*payload_len = (U16bit) ((*p_end) - (*p_start) + 1);*/
	payload_len = (end - start) + 1;

        /* SPR 4357 Fix End */

	if (pdu_len > payload_len)
	{ /* less bytes have been rcvd */
		/* log error and discard the packet */
		EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_INV_MSG_FORMAT,
					egtpu_entity_name_arr[EGTPU_EI_GTPU],
					egtpu_get_msg_name (p_egtpu_info->pdu_type)));

        /* SPR 17747 FIX START */
        LOG_MSG( GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON,
                relayGtpuCurrentTick_g, __LINE__, EGTPU_PERR_INV_MSG_FORMAT, 
                p_egtpu_info->pdu_type,0,0,0,0, __func__, 
                egtpu_entity_name_arr[EGTPU_EI_GTPU] );
        /* SPR 17747 FIX END */

        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTPU_MSG_LESS_BYTES_RCVD, LOGWARNING, PR_GTPU_DL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                pdu_len,payload_len,0,0, 0,0, __func__,"");
        /* SPR 2030 fix End*/

		if (payload_len < GTP_MSG_HDR_MIN_LEN)
        {
            /*SPR 2030 fix Start*/
            LOG_GTPU_MSG(GTPU_PAYLOAD_LESS_THAN_HDR_LEN, LOGWARNING, PR_GTPU_DL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                GTP_MSG_HDR_MIN_LEN,payload_len,0,0, 0,0, __func__,"");
            /* SPR 2030 fix End*/

			EGTPU_UPDATE_STATS(p_global,EGTPU_STAT_ERR_GTP_MESSAGE_TOO_SHORT);
        }

                /* SPR 2007 Fix - Part2 Start*/
		/* EGTPU_FREE_MSG_BUF (p_buffer);*/
                /* SPR 2007 Fix - Part2 End*/
		return EGTPU_FAILURE;
	}
	else if (pdu_len < payload_len)
	{
		p_end = p_start + pdu_len - 1; /* discard extra bytes rcvd */

		/* set end of payload as end of PDU, fix msg length too */
		/* peer is mis-behaving */
		EGTPU_API_SET_END_PAYLOAD (p_buffer, (p_end - p_buffer));
		EGTPU_API_SET_MSG_LEN (p_buffer, (p_end - p_buffer + 1));
	}

	p_egtpu_info->pdu_len = pdu_len;

	/* minimal protocol header validation */
	if (validate_protocol_hdr (p_start, p_end, &error , p_global))
	{
		EGTPU_ERROR_REPORTER (p_global,(egtpu_error_array[error],
					egtpu_entity_name_arr[EGTPU_EI_GTPU]));
        
        /* SPR 17747 FIX START */
        LOG_MSG( GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON,
                relayGtpuCurrentTick_g, __LINE__, error,
                 p_egtpu_info->pdu_type,0,0,0,0, __func__,
                 egtpu_entity_name_arr[EGTPU_EI_GTPU] );
        /* SPR 17747 FIX END */
                /* SPR 2007 Fix - Part2 Start*/
		/* EGTPU_FREE_MSG_BUF (p_buffer);*/
                /* SPR 2007 Fix - Part2 End*/
		return EGTPU_FAILURE;
	}

	/* sender's port number */
	p_egtpu_info->port_num = EGTPU_GET_U16BIT
		(p_buffer + EGTPU_API_HDR_LEN + EGTPU_SIZE_OF_TYPE);

/*SPR 3587 Fix Start*/
	/* sender's address */
	p_egtpu_info->p_peer_addr = p_buffer + EGTPU_API_HDR_LEN +
/* SPR 4831 Fix Start */
		EGTPU_SIZE_OF_TYPE + EGTPU_SIZE_OF_PORT_NUM + EGTPU_SIZE_OF_TYPE; 
/* SPR 4831 Fix Start */
/*SPR 3587 Fix End*/

	/* hdr-len is length of entire egtpu hdr */
	/* hdr-len will atleast be 12 when either of E, S, PN flag is set */
	if (*p_start & 0x07)
	{
		p_egtpu_info->hdr_len = EGTPU_HDR_LEN_WITH_SEQ_NUM;

		if ((S32bit)(p_end - p_start + 1) < EGTPU_HDR_LEN_WITH_SEQ_NUM)
		{
           /* SPR 2030 fix Start*/
           LOG_GTPU_MSG(GTPU_INSUFFICIENT_HDR_LEN_WITH_SEQ_NUM, LOGWARNING, 
                   PR_GTPU_DL_DATA,relayGtpuCurrentTick_g, __LINE__,
                   EGTPU_HDR_LEN_WITH_SEQ_NUM,(S32bit)(p_end - p_start + 1),
                   0,0, 0,0, __func__,""); 
           /* SPR 2030 fix End*/

			/* log error and discard the packet */
			EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_INV_MSG_FORMAT,
						egtpu_entity_name_arr[EGTPU_EI_GTPU],
						egtpu_get_msg_name (p_egtpu_info->pdu_type)));

			EGTPU_UPDATE_STATS(p_global,EGTPU_STAT_ERR_GTP_MESSAGE_TOO_SHORT);
                        /* SPR 2007 Fix - Part2 Start*/
			/* EGTPU_FREE_MSG_BUF (p_buffer);*/
                        /* SPR 2007 Fix - Part2 End*/
			return EGTPU_FAILURE;
		}
	}
	else
		p_egtpu_info->hdr_len = GTP_MSG_HDR_MIN_LEN;

	switch (p_egtpu_info->pdu_type)
	{
		case EGTPU_MSGT_TPDU:
			return __dec_tpdu__ (p_global,p_egtpu_info, p_start, p_end);

		case EGTPU_MSGT_ECHO_REQUEST:
			return __dec_echo_req__ (p_global,p_egtpu_info, p_start);

		case EGTPU_MSGT_ECHO_RESPONSE:
			return __dec_echo_rsp__ (p_global,p_egtpu_info, p_start, p_end);

		case EGTPU_MSGT_SUPP_EXT_HDR_NOTI:
			return __dec_supp_extn_hdr_noti__ (p_global,p_egtpu_info,
					p_start, p_end);

		case EGTPU_MSGT_ERROR_INDICATION:
			return __dec_err_ind__ (p_global,p_egtpu_info, p_start, p_end);

		case EGTPU_MSGT_END_MARKER:
			return __dec_end_marker__ (p_global,p_egtpu_info, p_start, p_end);
	}

	EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_UNKNOWN_MSG,
				egtpu_entity_name_arr[EGTPU_EI_GTPU],
				p_egtpu_info->pdu_type));
    /* SPR 2030 Fix Start*/
    LOG_GTPU_MSG(GTPU_UNKNOWN_MSG, LOGWARNING, PR_GTPU_DL_DATA,relayGtpuCurrentTick_g,
                   __LINE__,p_egtpu_info->pdu_type,0,0,0, 0,0, __func__,"");  
    /* SPR 2030 Fix End*/

	EGTPU_UPDATE_STATS(p_global,EGTPU_STAT_ERR_UNKNOWN_GTP_SIGNALLING_MESSAGE);
	/* SPR 2007 Fix - Part2 Start*/
        /* EGTPU_FREE_MSG_BUF (p_buffer);*/
        /* SPR 2007 Fix - Part2 End*/
	return EGTPU_FAILURE;
}

/****************************************************************************
 * Function Name  : egtpu_setup_sap_req_parse
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 * Outputs        : None
 * Returns        : EGTP_SUCCESS/EGTP_FAILURE
 * Description    : This funtion takes the API message received for setup sap
 *                  req extract relevant information (IE present in message)
 *                  and store the infonmation in structure egtpu_info_t.
 ****************************************************************************/
egtpu_return_t egtpu_setup_sap_req_parse (egtpu_global_t *p_global,
											egtpu_info_t *p_egtpu_info)
{
    U8bit *p_start = p_egtpu_info->p_buff + EGTPU_API_HDR_LEN,
          *p_end = EGTPU_API_GET_END_PARAM_LIST_PTR (p_egtpu_info->p_buff),          
          *p_tmp=p_start,
          src_addr_len = 0, dst_addr_len = 0;  
          
         
    while (p_start <= p_end) {
        switch (*p_start++) {
            /* SPR 10787 fix start */
            case EGTPU_IE_CELL_INDEX_I:
                p_egtpu_info->ie_presence |= EGTPU_IE_CELL_ID_FLAG;
                p_egtpu_info->cellIndex = *p_start;
                p_start += EGTPU_SIZE_OF_CELL_ID;
                break;
            /* SPR 10787 fix end */
            case EGTPU_IE_TRANSACTION_ID:
                p_egtpu_info->ie_presence |= EGTPU_IE_TRANSACTION_ID_FLAG;
                p_egtpu_info->transaction_id = EGTPU_GET_U32BIT (p_start);

                p_start += EGTPU_SIZE_OF_TRANSACTION_ID;
                break;

            case EGTPU_IE_TEID_DATA_I:
                if ((p_egtpu_info->ie_presence &
                        EGTPU_IE_TRANSACTION_ID_FLAG) ||
                        (p_egtpu_info->ie_presence &
                         EGTPU_IE_TEID_SELF_FLAG)) {
                    p_egtpu_info->ie_presence |= EGTPU_IE_TEID_PEER_FLAG;
                    p_egtpu_info->teid_peer = EGTPU_GET_U32BIT (p_start);
                } else {
                    p_egtpu_info->ie_presence |= EGTPU_IE_TEID_SELF_FLAG;
                    p_egtpu_info->teid_self = EGTPU_GET_U32BIT (p_start);
                }

                p_start += EGTPU_SIZE_OF_TEID;
                break;

            case EGTPU_IE_SAP_FLAG:
                p_egtpu_info->sap_flag = *p_start++;
                break;
 
            case EGTPU_IE_QOS_PROFILE:
                p_tmp = p_start;
                p_start += (EGTPU_GET_U16BIT(p_start) + EGTPU_SIZE_OF_LENGTH);
                p_tmp += EGTPU_SIZE_OF_LENGTH + 5;
				/*2 is added because the length field of any IE is of 2 bytes
				and 5 is added so that to point to the 6th byte of the
				qos_profile which contains the ip_traffic*/ 
                
                p_egtpu_info->ip_traffic=(*p_tmp & 0xE0) >> 5;
				/*ip_traffic class is present at the most significant 3 bit
				positions so ANDing with 0xE0 is done to mask the remaining
				bits.right shifting is performed 5 times so that the
				most significant 3 bits get converted into
				least significant 3 bits*/ 
                if(p_egtpu_info->ip_traffic > 4)
				{ 
					/**p_trav++ = EGTPU_PERR_INV_TRAFFIC_CLASS;*/
					EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_INV_TRAFFIC_CLASS,
                    egtpu_entity_name_arr[EGTPU_EI_GTPU],p_egtpu_info->ip_traffic));
					p_egtpu_info->ip_traffic = 0;
                   /* SPR 2030 fix Start*/
                   LOG_GTPU_MSG(GTP_INV_IP_TRAFFIC_CLASS_IN_SETUP_SAP_REQ, LOGWARNING,
                           PR_GTPU_CONTROL, relayGtpuCurrentTick_g, __LINE__,
                           0,0,0,0, 0,0, __func__, "");
                   /* SPR 2030 fix End*/
				}
							
 					p_egtpu_info->qos_id = EGTPU_GET_U32BIT (p_start);
					p_start += EGTPU_SIZE_OF_QOS_ID;
				 
              
                break;

            case EGTPU_IE_SEQ_NUM:
                p_egtpu_info->ie_presence |= (EGTPU_IE_SEQ_NUM_UPLK_FLAG
                        | EGTPU_IE_SEQ_NUM_DNLK_FLAG);

                p_egtpu_info->seq_num_uplk = EGTPU_GET_U16BIT(p_start);
				p_start += EGTPU_SIZE_OF_SEQ_NUM + EGTPU_SIZE_OF_TYPE;

                p_egtpu_info->seq_num_dnlk = EGTPU_GET_U16BIT(p_start);
				p_start += EGTPU_SIZE_OF_SEQ_NUM;

                break;

            case EGTPU_IE_GSN_ADDR:
				if(p_egtpu_info->ie_presence & EGTPU_IE_TEID_PEER_FLAG)
				{
					if(p_egtpu_info->ie_presence & EGTPU_IE_TRANSPORT_ADDR_FLAG)
					{
                		p_egtpu_info->ie_presence |=
										EGTPU_IE_SOURCE_TRANSPORT_ADDR_FLAG;
                        
                    	src_addr_len=EGTPU_GET_U16BIT(p_start);						

						if( (src_addr_len != EGTPU_SIZE_OF_IPV4_ADDR) &&
                                    (src_addr_len != EGTPU_SIZE_OF_IPV6_ADDR) )					
						{
                            /* SPR 2030 fix Start*/
                            LOG_GTPU_MSG(GTP_INV_SRCIP_LEN_IN_SETUP_SAP_REQ, LOGWARNING,
                                PR_GTPU_CONTROL, relayGtpuCurrentTick_g, __LINE__,
                                src_addr_len,0,0,0, 0,0, __func__, "");
                            /* SPR 2030 fix End*/
							EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_INV_ADDR,
							egtpu_entity_name_arr[EGTPU_EI_GTPU],p_egtpu_info->p_src_addr));
							return EGTPU_FAILURE;
						}
					    p_egtpu_info->p_src_addr = p_start;
                		p_start += (EGTPU_GET_U16BIT(p_start) + 
												EGTPU_SIZE_OF_LENGTH);
					}
					else
					{
                		p_egtpu_info->ie_presence |= EGTPU_IE_TRANSPORT_ADDR_FLAG;
						dst_addr_len = EGTPU_GET_U16BIT(p_start);					
					
						if( (dst_addr_len != EGTPU_SIZE_OF_IPV4_ADDR) &&
                                    (dst_addr_len != EGTPU_SIZE_OF_IPV6_ADDR) )					
						{
                            /* SPR 2030 fix Start*/
                            LOG_GTPU_MSG(GTP_INV_DESTIP_LEN_IN_SETUP_SAP_REQ, LOGWARNING,
                                PR_GTPU_CONTROL, relayGtpuCurrentTick_g, __LINE__,
                                dst_addr_len,0,0,0, 0,0, __func__, "");
                            /* SPR 2030 fix End*/
							EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_INV_ADDR,
							egtpu_entity_name_arr[EGTPU_EI_GTPU],p_egtpu_info->p_peer_addr));
							return EGTPU_FAILURE;
						}
						p_egtpu_info->p_peer_addr = p_start;
						p_start += (EGTPU_GET_U16BIT(p_start) +
												EGTPU_SIZE_OF_LENGTH);	
                    }
				}
				else
				{
                	p_egtpu_info->ie_presence |= EGTPU_IE_SOURCE_TRANSPORT_ADDR_FLAG;
                   	src_addr_len=EGTPU_GET_U16BIT(p_start);						
					if( (src_addr_len != EGTPU_SIZE_OF_IPV4_ADDR) &&
                                    (src_addr_len != EGTPU_SIZE_OF_IPV6_ADDR) )					
					{
                        /* SPR 2030 fix Start*/
                        LOG_GTPU_MSG(GTP_INV_SRCIP_LEN_IN_SETUP_SAP_REQ, LOGWARNING,
                            PR_GTPU_CONTROL, relayGtpuCurrentTick_g, __LINE__,
                            src_addr_len,0,0,0, 0,0, __func__, "");
                        /* SPR 2030 fix End*/
						EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_INV_ADDR,
						    egtpu_entity_name_arr[EGTPU_EI_GTPU],p_egtpu_info->p_src_addr));
						return EGTPU_FAILURE;
					}
                	p_egtpu_info->p_src_addr = p_start;
                	p_start += (EGTPU_GET_U16BIT(p_start) +
												EGTPU_SIZE_OF_LENGTH);
				}
                break;
            case EGTPU_IE_SEQ_DISABLE:
                p_egtpu_info->ie_presence |= EGTPU_IE_SEQ_FLAG;
				p_egtpu_info->seq_disable = *p_start++;
                break;
    
            case EGTPU_IE_REORDERING_REQD:
                p_egtpu_info->ie_presence |= EGTPU_IE_REORDERING_FLAG;
                p_egtpu_info->reordering_reqd = *p_start++;
            /* INTRA_ENB_HO Fix Start */
               break;

            case EGTPU_IE_PDCP_EXT_FLAG:
                 p_egtpu_info->ie_presence |= EGTPU_IE_PDCP_EXT_FLAG_P;
                 p_egtpu_info->pdcpext_flag = *p_start++;
                 break;

            /* INTRA_ENB_HO Fix End */
        } /* end if switch case for ie present*/
    } /*end of while loop */

    /* Cyclomatic Complexity changes - starts here */
    if ((src_addr_len && dst_addr_len) && (src_addr_len != dst_addr_len))
        /* Cyclomatic Complexity changes - ends here */
    {
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(GTP_SRCIP_DESTIP_LEN_MISMATCH, LOGWARNING,
                PR_GTPU_CONTROL, relayGtpuCurrentTick_g, __LINE__,
                src_addr_len,dst_addr_len,0,0, 0,0, __func__, "");
        /* SPR 2030 fix End*/
        EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_PERR_INV_ADDR,
                    egtpu_entity_name_arr[EGTPU_EI_GTPU],p_egtpu_info->p_peer_addr));
        return EGTPU_FAILURE;
    }

    return EGTPU_SUCCESS;
}

/********************************************************************************
 * Function Name  : egtpu_get_ctxt_info_req_parse
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 * Outputs        : None
 * Returns        : EGTP_SUCCESS/EGTP_FAILURE
 * Description    : This funtion takes the API message received to get context
 *                  info req extract relevant information (IE present in message)
 *                  and store the infonmation in structure egtpu_info_t.
 ********************************************************************************/
egtpu_return_t egtpu_get_ctxt_info_req_parse (egtpu_global_t *p_global,
												egtpu_info_t *p_egtpu_info)
{
    U8bit *p_start = p_egtpu_info->p_buff + EGTPU_API_HDR_LEN,
          *p_end = EGTPU_API_GET_END_PARAM_LIST_PTR (p_egtpu_info->p_buff);

    p_egtpu_info->qi_info.rec_type = *p_start++;
    p_egtpu_info->qi_info.req_code = *p_start++;

    while (p_start < p_end)
	{
        switch (*p_start++)
		{
            case EGTPU_IE_TRANSACTION_ID:
                p_egtpu_info->qi_info.fltr.type = EGTPU_IE_TRANSACTION_ID;
                p_egtpu_info->qi_info.fltr.val.xid =
                    EGTPU_GET_U32BIT(p_start + EGTPU_SIZE_OF_LENGTH);

                p_start += EGTPU_SIZE_OF_LENGTH + EGTPU_SIZE_OF_TRANSACTION_ID;
                break;

            case EGTPU_IE_TEID_DATA_I:
                p_egtpu_info->qi_info.fltr.type = EGTPU_IE_TEID_DATA_I;
                p_egtpu_info->qi_info.fltr.val.teid =
                    EGTPU_GET_U32BIT (p_start + EGTPU_SIZE_OF_LENGTH);

                p_start += EGTPU_SIZE_OF_LENGTH + EGTPU_SIZE_OF_TEID;
                break;

            case EGTPU_IE_GSN_ADDR:
                p_egtpu_info->qi_info.fltr.type = EGTPU_IE_GSN_ADDR;
                p_egtpu_info->qi_info.fltr.val.addr = p_start;

                p_start += (EGTPU_GET_U16BIT(p_start) + EGTPU_SIZE_OF_LENGTH);
                break;

            case EGTPU_IE_NEXT_IDX:
                p_egtpu_info->ie_presence |= EGTPU_IE_NEXT_IDX_FLAG;
                p_egtpu_info->qi_info.next_idx = EGTPU_GET_U32BIT(p_start);

                p_start += EGTPU_SIZE_OF_NEXT_IDX_FLAG;
                break;

            case EGTPU_IE_REC_COUNT:
                p_egtpu_info->ie_presence |= EGTPU_IE_REC_COUNT_FLAG;
                p_egtpu_info->qi_info.rec_count =
                    EGTPU_GET_U32BIT(p_start);

                p_start += EGTPU_SIZE_OF_REC_COUNT_FLAG;
                break;
        }
    }
    /* GCC_4.9_WARNINGS_FIX_START */
  LTE_GCC_UNUSED_PARAM(p_global)
    /* GCC_4.9_WARNINGS_FIX_END */
    return EGTPU_SUCCESS;
}

#ifdef MAC_AUT_TEST

/********************************************************************************
 * Function Name  : local__send_supp_extn_hdr_noti__
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion used for AUT testing to send extention header 
 *                  notification 
 ********************************************************************************/
egtpu_void_t local__send_supp_extn_hdr_noti__ (egtpu_global_t *p_global,
						egtpu_info_t *p_egtpu_info)
{
 __send_supp_extn_hdr_noti__ (p_global,p_egtpu_info); 
}

/********************************************************************************
 * Function Name  : local_validate_protocol_hdr
 * Inputs         : p_start - Points to starting point of buffer after header,
 *                  p_end - Points to end point of buffer,                  
 *                  p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 * Outputs        : None
 * Returns        : egtpu_return_t 
 * Description    : This funtion used for AUT testing to get the result 
 ********************************************************************************/
egtpu_return_t local_validate_protocol_hdr (U8bit *p_start,
				        	U8bit *p_end,
					 	egtpu_error_t *p_error,
						egtpu_global_t *p_global)
{
     return validate_protocol_hdr(p_start , p_end, p_error, p_global);
}

/********************************************************************************
 * Function Name  : local_skip_extn_hdr
 * Inputs         : p_start - Points to starting point of buffer after header,
 *                  p_end - Points to end point of buffer,                  
 *                  p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 * Outputs        : None
 * Returns        : egtpu_return_t 
 * Description    : This funtion used for AUT testing to get the result 
 ********************************************************************************/
egtpu_return_t local_skip_extn_hdr(U8bit *p_start,
				        	U8bit *p_end,
					 	egtpu_info_t *p_egtpu_info,
						egtpu_global_t *p_global)
{
     return skip_extn_hdr(p_global, p_egtpu_info, p_start, p_end);
}

/********************************************************************************
 * Function Name  : local_dec_extn_hdr
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 *                  p_start - Points to starting point of buffer after header,
 *                  p_end - Points to end point of buffer
 * Outputs        : None
 * Returns        : egtpu_return_t 
 * Description    : This funtion used for AUT testing to get the result 
 ********************************************************************************/
egtpu_return_t local_dec_extn_hdr(egtpu_global_t *p_global,
					 	egtpu_info_t *p_egtpu_info,
				        	U8bit *p_start,
				        	U8bit *p_end)
{
     return dec_extn_hdr(p_global, p_egtpu_info, p_start, p_end);
}

/********************************************************************************
 * Function Name  : local__dec_tpdu__
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 *                  p_start - Points to starting point of buffer after header,
 *                  p_end - Points to end point of buffer
 * Outputs        : None
 * Returns        : egtpu_return_t 
 * Description    : This funtion used for AUT testing to get the result 
 ********************************************************************************/
egtpu_return_t local__dec_tpdu__(egtpu_global_t *p_global,
					 	egtpu_info_t *p_egtpu_info,
				        	U8bit *p_start,
				        	U8bit *p_end)
{
     return __dec_tpdu__(p_global, p_egtpu_info, p_start, p_end);
}

/********************************************************************************
 * Function Name  : local__dec_echo_req__
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 *                  p_start - Points to starting point of buffer after header
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion used for AUT testing to get the result 
 ********************************************************************************/
egtpu_return_t local__dec_echo_req__(egtpu_global_t *p_global,
					 	egtpu_info_t *p_egtpu_info,
				        	U8bit *p_start)
{
     return __dec_echo_req__(p_global, p_egtpu_info, p_start);
}

/********************************************************************************
 * Function Name  : local__dec_echo_rsp__
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 *                  p_start - Points to starting point of buffer after header,
 *                  p_end - Points to end point of buffer
 * Outputs        : None
 * Returns        : egtpu_return_t 
 * Description    : This funtion used for AUT testing to get the result 
 ********************************************************************************/
egtpu_return_t local__dec_echo_rsp__ (egtpu_global_t *p_global,
						egtpu_info_t *p_egtpu_info,
				        	U8bit *p_start,
				        	U8bit *p_end)
{
    return  __dec_echo_rsp__(p_global, p_egtpu_info,  p_start, p_end); 
}

/********************************************************************************
 * Function Name  : local__dec_supp_extn_hdr_noti__
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 *                  p_start - Points to starting point of buffer after header,
 *                  p_end - Points to end point of buffer
 * Outputs        : None
 * Returns        : egtpu_return_t 
 * Description    : This funtion used for AUT testing to get the result 
 ********************************************************************************/
egtpu_return_t local__dec_supp_extn_hdr_noti__ (egtpu_global_t *p_global,
						egtpu_info_t *p_egtpu_info,
				        	U8bit *p_start,
				        	U8bit *p_end)
{
    return  __dec_supp_extn_hdr_noti__ (p_global, p_egtpu_info,  p_start, p_end); 
}

/********************************************************************************
 * Function Name  : local__dec_err_ind__
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 *                  p_start - Points to starting point of buffer after header,
 *                  p_end - Points to end point of buffer
 * Outputs        : None
 * Returns        : egtpu_return_t 
 * Description    : This funtion used for AUT testing to get the result 
 ********************************************************************************/
egtpu_return_t local__dec_err_ind__(egtpu_global_t *p_global,
						egtpu_info_t *p_egtpu_info,
				        	U8bit *p_start,
				        	U8bit *p_end)
{
    return  __dec_err_ind__(p_global, p_egtpu_info,  p_start, p_end); 
}
 
/********************************************************************************
 * Function Name  : local__dec_end_marker__
 * Inputs         : p_global - Pointer to global structure egtpu_global_t,
 *                  p_egtpu_info - Pointer to egtpu_info_t
 *                  p_start - Points to starting point of buffer after header,
 *                  p_end - Points to end point of buffer
 * Outputs        : None
 * Returns        : egtpu_return_t 
 * Description    : This funtion used for AUT testing to get the result 
 ********************************************************************************/
egtpu_return_t local__dec_end_marker__(egtpu_global_t *p_global,
						egtpu_info_t *p_egtpu_info,
				        	U8bit *p_start,
				        	U8bit *p_end)
{
    return  __dec_end_marker__(p_global, p_egtpu_info,  p_start, p_end); 
}
 
#endif
