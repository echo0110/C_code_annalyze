/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_main_utils.c,v 1.11.18.17 2010/12/02 04:56:11 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : Utility Functions for ROHC entity message parsing.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_main_utils.c,v $
 * Revision 1.11.18.17  2010/12/02 04:56:11  gur22059
 * SPR 576 is fixed
 *
 * Revision 1.11.18.16  2010/11/24 03:41:30  gur22059
 * Removal of conditional checks w.r.t profile1
 *
 * Revision 1.11.18.15  2010/11/09 05:21:36  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.11.18.14  2010/10/13 03:53:45  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.11.18.13  2010/10/01 14:05:46  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.11.18.12  2010/09/24 09:20:20  gur22059
 * Removed the NR flag
 *
 * Revision 1.11.18.11  2010/09/23 05:18:30  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.11.18.10  2010/08/30 08:21:24  gur22059
 * Removed enhancement that was done to bypass msgInsert
 *
 * Revision 1.11.18.9  2010/08/27 10:07:43  gur22059
 * Extra check was removed before msgInsert & msgRemove in rohc_process_data_req
 *
 * Revision 1.11.18.8  2010/08/26 05:39:58  gur22059
 * Enhancement for removing msgInsert ZCB interface from compressor
 *
 * Revision 1.11.18.7  2010/07/09 04:32:49  gur21145
 * code review comments for prof 0 , 2 enhancements
 *
 * Revision 1.11.18.6  2010/07/08 10:19:43  gur21145
 * correcting indentation
 *
 * Revision 1.11.18.5  2010/07/08 09:35:55  gur21145
 * code review comments of prof 0 , 2 enhancement activity
 *
 * Revision 1.11.18.1  2010/06/25 10:02:58  gur21145
 * profile2_ENHANCEMENTS
 *
 * Revision 1.11  2010/06/09 11:51:36  gur19479
 * updated from intel branch
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.10.2.2  2009/07/10 09:07:20  gur19140
 * optimization changes for zcb
 *
 * Revision 1.10.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.10  2009/05/29 09:37:49  gur19479
 * *** empty log message ***
 *
 * Revision 1.9  2009/05/29 09:09:43  gur19479
 * updated for mode transition
 *
 * Revision 1.8  2009/05/28 03:26:54  gur19836
 * File Header updated
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "rohc_typ.h"
#include "rohc_def.h"
#include "rohc_ex.h"

#include "rohc_stats_def.h"
#include "rohc_ccom_mcr.h"
#include "rohc_g_typ.h"
#include "rohc_gcom_mcr.h"
#include "rohc_trace_mcr.h"
#include "rohc_protocol_header.h"
#include "rohc_ed_ext_if.h"
#include "rohc_opt_proto.h" 

#include "rohc_main_def.h"
#include "rohc_main_typ.h"
#include "rohc_main_ex.h"
#include "rohc_main_proto.h"
#include "rohc_pk_proto.h"
#include "rohc_unpk_proto.h"
#include "rohc_db_proto.h"
#include "rohc_db_ext_if.h"
#include "rohc_ppm_ext_if.h"
#include "rohc_cprm_ext_if.h"
#include "rohc_dprm_ext_if.h"
#include "rohc_pbm_ext_if.h"
#include "rohc_ccpm_ext_if.h"
#include "rohc_dcpm_ext_if.h"
#include "rohc_ex.h"
#include "rohc_crc_ext_if.h"
#include "alarm.h"
#ifdef PERF_STATS 
 /* + PERF_CA */
#include "ltePdcpContext.h"
 /* - PERF_CA */
#endif

/* SPR 20149 Start */


/* SPR 20149 End */

/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
 /* + SPR 17439 */
STATIC rohc_return_t   rohc_validate_and_process_feedback   ROHC_PROTO((
 rohc_csc_common_t   *p_csc         , 
 rohc_U8bit_t        *msg           ,
 rohc_U32bit_t       in_buff_len    ,
 rohc_U8bit_t        cid_len     ,
 rohc_entity_t       *p_rohc_entity ,
 rohc_error_code_t   *p_ecode
 ));
/* - SPR 17439 */
/******************************************************************************
 * Private Constants
 *****************************************************************************/

/*
** START_SPR_576_FIX:
** This header size is taken with the assumption of max header size of 
** profile 0 & profile 2. In future release, in case of profile 1 this
** value may change.
*/
#define ROHC_MAX_HDR_SIZE 80
/* END_SPR_576_FIX */

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* +- SPR 17777 */
extern tickType_t pdcpCurrentTime_g;
/* +- SPR 17777 */
/* + Rohc Debug Log */
UInt8 rohcLogEnabled_g = ROHC_FALSE;
/* - Rohc Debug Log */
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
/* + SPR 17439 */
rohc_return_t (*rohc_process_feedback_fpt [ROHC_ALL_PROFILE - ROHC_UNCOMP])
                                          (
                                          rohc_entity_t     *p_rohc_entity, 
                                          rohc_csc_common_t       *p_csc,
                                          rohc_U8bit_t      *msg,
                                          rohc_U32bit_t     in_buff_len,
                                          rohc_error_code_t *p_ecode
                                          ) =
/* SPR 9591 fix start */
{rohc_cprm_p0_process_feedback, rohc_cprm_p1_process_feedback, rohc_cprm_p2_process_feedback, NULL,
    NULL, NULL, rohc_cprm_p6_process_feedback};
/* SPR 9591 fix end */

rohc_return_t (*rohc_process_compr_packet_fpt [ROHC_ALL_PROFILE - ROHC_UNCOMP])
                                        (
                                        rohc_entity_t     *p_rohc_entity,
                                        rohc_csc_common_t       *p_csc,
                                        rohc_U8bit_t      *msg,
                                        rohc_U32bit_t     header_len,
                                        rohc_U8bit_t      *p_in_buff,
                                        rohc_error_code_t *p_ecode
                                        ) =
{rohc_process_p0_compr_pkt, rohc_process_p1_compr_pkt, rohc_process_p2_compr_pkt, NULL, NULL,NULL,
                                                       rohc_process_p6_compr_pkt}; 

rohc_return_t (*rohc_process_decompr_packet_fpt [ROHC_ALL_PROFILE - ROHC_UNCOMP])
                                        (
                                        rohc_entity_t     *p_rohc_entity,
                                        rohc_dsc_common_t       *p_dsc,
                                        /* +- SPR 17777 */
                                        rohc_U8bit_t      *p_in_buff,
                                        rohc_api_info_t   *p_rohc_api_info,
                                        rohc_error_code_t *p_ecode
                                        ) =
{rohc_process_p0_decompr_pkt, rohc_process_p1_decompr_pkt,
    rohc_process_p2_decompr_pkt, NULL, NULL,NULL, rohcProcessP6DecomprPkt};
/* - SPR 17439 */

/*ROHC_MAX_HDR_SIZE*/
extern rohc_U8bit_t *rohc_process_data_req_buff_p;
/* 50 */
extern rohc_U8bit_t *rohc_process_p0_compr_pkt_out_buff_p;
extern rohc_U8bit_t *rohc_process_p1_compr_pkt_out_buff_p;
extern rohc_U8bit_t *rohc_process_p2_compr_pkt_out_buff_p;
extern rohc_U8bit_t *rohc_process_p6_compr_pkt_out_buff_p;

/*ROHC_MAX_HDR_SIZE*/
extern rohc_U8bit_t *rohc_process_rlc_data_ind_buff_p;

/*ROHC_MAX_UNCOM_PKT_LEN*/
extern rohc_U8bit_t *rohc_process_p1_decompr_pkt_uncom_msg_p;
extern rohc_U8bit_t *rohc_process_p2_decompr_pkt_uncom_msg_p;
extern rohc_U8bit_t *rohc_process_p6_decompr_pkt_uncom_msg_p;


/*******************************************************************************
 **  FUNCTION :  rohc_process_data_req   
 ********************************************************************************
 **  DESCRIPTION :
 **      This function is called to handle the API ROHC_DATA_REQ. The calls all 
 **      the modules of ROHC  to compress the Packet.
 **
 **  INPUT:
 **
 **  1) *p_rohc_entity  = pointer to the rfc 3095 entity, the entry for rohc entity
 **                     in db is made in PDCP. 
 **  2)  *p_in_buff  = pointer to the incoming buffer.
 **  3)  **p_p_out_buff = pointer to outgoing buffer, in which ROHC will fill data,
 **                     and return to PDCP.
 **  4)  *p_api_buff_info  = pointer to the info for different ROHC APIs.
 **  5)  *p_ecode = pointer to error code.
 **
 **  RETURN VALUES:
 **      
 **      1. ROHC_SUCCESS:        If action is success.
 **      2. ROHC_FAILURE:        If action is failure.
 **
 *******************************************************************************/


rohc_return_t   rohc_process_data_req   ROHC_PARAMS
(
 (p_rohc_entity, p_in_buff, p_p_out_buff, p_rohc_api_info, p_ecode),
 rohc_entity_t       *p_rohc_entity      _AND_
 rohc_U8bit_t        *p_in_buff          _AND_
 rohc_U8bit_t        **p_p_out_buff      _AND_
 rohc_api_info_t     *p_rohc_api_info    _AND_
 rohc_error_code_t   *p_ecode
 )
{
    rohc_U32bit_t               in_buff_len=0;   
    rohc_csc_common_t           *p_csc = ROHC_NULL;
    rohc_U32bit_t               header_len=0;
    rohc_U8bit_t                *msg = ROHC_NULL;
    /* SPR 9591 fix start */
    rohc_U8bit_t                fb_code = 0;   
    /* SPR 9591 fix end */

    /* START_SPR_576_FIX */
    rohc_U8bit_t                seg_count = 0;
    /* END_SPR_576_FIX */

    ROHC_UT_TRACE((stderr,"In rohc_process_data_req UeIndex=%u LCId=%u\n",
                    p_rohc_entity->rohc_entity_id.ueIndex,
                    p_rohc_entity->rohc_entity_id.lcId));
    ROHC_ENTER_FN("rohc_process_data_req");
    rohc_U8bit_t                cid_len;
    rohc_U8bit_t                ret_val = RO_SUCCESS;
    /*Coverity Fix 32371 start*/
    rohc_U16bit_t               cid = ROHC_INVALID_CONTEXT_ID;
    /*Coverity Fix 32371 end*/
    /*SPR 3444 change start */
    QSEGMENT                    segment = {0};


   /*SPR 3444 change start */
    if(p_rohc_entity->fb_in_process == ROHC_TRUE)
    {
        ROHC_UT_TRACE((stderr," Processing feedback data-req UeId = %d\n",
                p_rohc_entity->rohc_entity_id.ueIndex));
        ROHC_LOG(LOG_INFO,
                "MAIN:EID:[%u:%u]: Processing feedback data-req, "\
                " created by ROHC "\
                " therefore returning back to PDCP without"\
                " doing anything\n",
                p_rohc_entity->rohc_entity_id.ueIndex,
                p_rohc_entity->rohc_entity_id.lcId);
        ROHC_EXIT_FN("rohc_process_data_req");
        return RO_SUCCESS;
    }   
    in_buff_len = p_rohc_api_info->rohc_api_union.rohc_data_req.in_buff_len;

    /* 
    ** START_SPR_576_FIX:
    ** msgExtract is used instead of msgSegNext because msgSegNext sometimes
    ** does not return us the complete buffer, due to which the decomression
    ** for such packet fails and inturn that packet is dropped.  
    */

    seg_count = msgSegCount(p_in_buff);
    if (seg_count > 1)
    {
        if (0 == msgExtract(p_in_buff, 0, rohc_process_data_req_buff_p, ROHC_MAX_HDR_SIZE))
        {
            ROHC_LOG(LOG_INFO,"DMAIN :EID[%u:%u]:"
                 " msgExtract failed while extracting ZCB\n",
                 p_rohc_entity->rohc_entity_id.ueIndex,
                 p_rohc_entity->rohc_entity_id.lcId);

            /* +- SPR 17777 */
            ltePanic("%s: msgExtract failed while extracting ZCB %d",__func__,sizeof(p_p_out_buff));

            return RO_FAILURE;
        }
        msg = rohc_process_data_req_buff_p;
    }
    else
    {
        /* SPR 3444 changes start */
        if (PNULL == msgSegNext(p_in_buff, 0, &segment) )
        {
             return RO_FAILURE;
        }
        /* SPR 3444 changes end */
        msg  = segment.base;
        /*Coverity 94765 Fix Start*/ 
        if(PNULL == msg)
        {
            ROHC_LOG(LOG_INFO,"DMAIN :EID[%u:%u]:"
                    " msgSegNext failed \n",
                    p_rohc_entity->rohc_entity_id.ueIndex,
                    p_rohc_entity->rohc_entity_id.lcId);
            return RO_FAILURE;
        }
        /*Coverity 94765 Fix End*/ 
    }
    /* END_SPR_576_FIX */
    /* + Rohc Debug Log */
    if(rohcLogEnabled_g)
    {
        fprintf(stderr,"[%s] Compressor:  UE[%u] lc[%d] seg_count[%d] DL Uncompressed Packet: %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x\n",
           __func__, p_rohc_entity->rohc_entity_id.ueIndex, p_rohc_entity->rohc_entity_id.lcId, seg_count, msg[0],msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7],msg[8],msg[9],msg[10],msg[11],msg[12],msg[13],msg[14],msg[15],msg[16],msg[17],msg[18],msg[19],msg[20],msg[21],msg[22],msg[23],msg[24],msg[25],msg[26],msg[27],msg[28],msg[29],msg[30],msg[31],msg[32],msg[33],msg[34],msg[35],msg[36],msg[37],msg[38],msg[39],msg[40],msg[41],msg[42],msg[43],msg[44],msg[45],msg[46],msg[47],msg[48],msg[49],msg[50],msg[51],msg[52],msg[53],msg[54],msg[55],msg[56],msg[57],msg[58],msg[59],msg[60],msg[61],msg[62],msg[63],msg[64],msg[65],msg[66],msg[67],msg[68],msg[69]);
    }
    /* - Rohc Debug Log */
    if (ROHC_SUCCESS == 
                p_rohc_api_info->rohc_api_union.rohc_data_req.feedback_flag)
    {
        if(p_rohc_entity->large_cids == ROHC_FALSE)
        {
            cid_len = 1;
            /* SPR 9591 fix start */
            fb_code = msg[0] & ROHC_FB_CODE_MASK;
            if (fb_code)
            {
                cid = msg[1] & 0x0F;
            }
            else
            {
                cid = msg[2] & 0x0F;
            }
            /* Compressor sets CID to 0 for uncompressed profile */
            if (cid)
            {
                p_csc = (rohc_csc_common_t *)
                            rohc_db_fb_get_csc_small_cid(p_rohc_entity,cid);
            }
            else
            {
		/* SPR 13319 fix start */
		/* cid length should be set to 0 for profile 0 */
		cid_len = 0; 
		/* SPR 13319 fix end */
                p_csc = rohc_db_get_uncomp_csc(p_rohc_entity,p_ecode);
            }
            /* SPR 9591 fix end */

        } else if( ((((msg[1]) & 0x80) == 0x00)
                    ||(((msg[1]) & 0xc0) == 0x80)))
        {  
            /* Changes done for Large CID */ 
            cid = rohc_d_sdvalue_decode(msg+1, &cid_len);
            /*Coverity Fix 32371 start*/
            if ( ROHC_INVALID_CONTEXT_ID == cid )
            {
            /*Coverity Fix 32371 end*/
               ROHC_LOG (LOG_INFO,"DMAIN :EID[%u:%u]:"
                    " Invalid CID\n",
                    p_rohc_entity->rohc_entity_id.ueIndex,
                    p_rohc_entity->rohc_entity_id.lcId);
               *p_ecode = ROHC_INVALID_PKT;
               return ROHC_FAILURE;
            }
            p_csc = (rohc_csc_common_t *)
                            rohc_db_fb_get_csc_large_cid(p_rohc_entity,cid);
        }   
        else
        {
            ROHC_LOG(LOG_INFO,"DMAIN :EID[%u:%u]:"
                    " Invalid Packet recieved\n",
                    p_rohc_entity->rohc_entity_id.ueIndex,
                    p_rohc_entity->rohc_entity_id.lcId);
            *p_ecode = ROHC_INVALID_PKT;
            return ROHC_FAILURE;
        }
        if(p_csc != ROHC_NULL)
        {
            ret_val = rohc_validate_and_process_feedback(p_csc,msg,in_buff_len,
                          cid_len,p_rohc_entity,p_ecode);
            return ret_val;
        }
        else 
        {
            *p_ecode = ROHC_CSC_NOT_EXISTS_FOR_RECEIVED_FB_CID;
            /* SPR 9591 fix start */
            ROHC_LOG(LOG_INFO,
                        "MAIN:EID:%u:CSC does not exist for"
                        " the CID received in the feedback packet\n",
                        p_rohc_entity->rohc_entity_id.ueIndex);
            /* SPR 9591 fix end */
            return RO_FAILURE;
        }
    }

    /* Invoke PPM.  */
    if (ROHC_FAILURE == rohc_c_parse_ip(msg, in_buff_len, \
                p_rohc_entity, &p_csc,&header_len, p_ecode))   
    {
        if(p_csc != ROHC_NULL )
        {
            p_csc->p_cpib = ROHC_NULL;
        }   

        ROHC_LOG(LOG_INFO,
                "MAIN:EID:[%u:%u]CPPM failed while"
                "parsing the received IP buffer\n",
                p_rohc_entity->rohc_entity_id.ueIndex, 
                p_rohc_entity->rohc_entity_id.lcId);

        /* SPR 9156 fix start*/
        /* Switching to uncompressed profile in case of parsing failure/max CID
         * reached */
        p_csc = rohc_db_get_uncomp_csc(p_rohc_entity,p_ecode);

/*Coverity 83605 Fix Start */
	if(ROHC_NULL == p_csc)
	{
		return ROHC_FAILURE;
	}
/*Coverity 83605 Fix End */
        /* SPR 9156 fix stop*/
    }


    /* Invoke PRM, 
     * It fills the packet type */

    if(ROHC_FAILURE == (*rohc_process_compr_packet_fpt
                       [ROHC_CSC_GET_PROFILE(p_csc) - ROHC_UNCOMP])
                       (p_rohc_entity, p_csc, msg, header_len, p_in_buff, p_ecode))
    {
        ROHC_LOG(LOG_INFO,
                "MAIN:EID:[%u:%u]Processing failed while"
                "processing the received IP buffer\n",
                p_rohc_entity->rohc_entity_id.ueIndex, 
                p_rohc_entity->rohc_entity_id.lcId);

        return RO_FAILURE;
    }
    /* + Rohc Debug Log */
    if(rohcLogEnabled_g)
    {
        rohc_U8bit_t dummy_msg_1[50] = {0};
        rohc_U8bit_t *dummy_msg = dummy_msg_1;
        if (0 == msgExtract(p_in_buff, 0, dummy_msg, 50))
        {
	    ROHC_LOG(LOG_INFO, "MsgExtract Failed");
            return RO_FAILURE;
        }

        fprintf(stderr,"[%s] Compressor: State[%u] Profile[%u] Mode[%u] PacketType[%u] CID[%d]: DL Compressed Packet:%0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x\n\n",__func__, ROHC_CSC_GET_STATE(p_csc), ROHC_CSC_GET_PROFILE(p_csc), ROHC_CSC_GET_MODE(p_csc), p_csc->rohc_gsw.rohc_gsw_elem[p_csc->rohc_gsw.end_index].pkt_typ,ROHC_CSC_GET_CID(p_csc),dummy_msg[0],dummy_msg[1],dummy_msg[2],dummy_msg[3],dummy_msg[4],dummy_msg[5],dummy_msg[6],dummy_msg[7],dummy_msg[8],dummy_msg[9],dummy_msg[10],dummy_msg[11],dummy_msg[12],dummy_msg[13],dummy_msg[14],dummy_msg[15],dummy_msg[16],dummy_msg[17],dummy_msg[18],dummy_msg[19],dummy_msg[20],dummy_msg[21],dummy_msg[22],dummy_msg[23],dummy_msg[24],dummy_msg[25],dummy_msg[26],dummy_msg[27],dummy_msg[28],dummy_msg[29],dummy_msg[30],dummy_msg[31],dummy_msg[32],dummy_msg[33],dummy_msg[34],dummy_msg[35],dummy_msg[36],dummy_msg[37],dummy_msg[38],dummy_msg[39],dummy_msg[40],dummy_msg[41],dummy_msg[42],dummy_msg[43],dummy_msg[44],dummy_msg[45],dummy_msg[46],dummy_msg[47],dummy_msg[48],dummy_msg[49]);
    }
    /* - Rohc Debug Log */
    ROHC_EXIT_FN("rohc_process_data_req");
    return RO_SUCCESS;
}

/*******************************************************************************
 **  FUNCTION :  rohc_validate_and_process_feedback   
 ********************************************************************************
 **  DESCRIPTION :
 **      This function is called to handle the feedback.
 **
 **  INPUT:
 **
 **  1)  *p_csc = pointer to the CSC of the compressor. 
 **  2)  *msg  = pointer to the packet received by compressor. 
 **  3) in_buff_len  =  length of incoming buffer.
 **  4) cid_len  =  length of cid.
 **  5) *p_rohc_entity  = pointer to the rfc 3095 entity, the entry for
 **                       rohc entity in db is made in PDCP.
 **  6)  *p_ecode = pointer to error code.
 **
 **  RETURN VALUES:
 **      
 **      1. ROHC_SUCCESS:        If action is success.
 **      2. ROHC_FAILURE:        If action is failure.
 **
 *******************************************************************************/
STATIC rohc_return_t   rohc_validate_and_process_feedback   ROHC_PARAMS
(
 ( p_csc, msg, in_buff_len, cid_len,p_rohc_entity,p_ecode),
 rohc_csc_common_t   *p_csc         _AND_ 
 rohc_U8bit_t        *msg           _AND_ 
 rohc_U32bit_t       in_buff_len     _AND_
 rohc_U8bit_t        cid_len     _AND_
 rohc_entity_t       *p_rohc_entity _AND_
 rohc_error_code_t   *p_ecode
 )
{
    /* SPR 13319 fix start */
    rohc_return_t   ret_val;
    rohc_U8bit_t fb_code = 0; 
    /* SPR 13319 fix end */

    ROHC_ENTER_FN("rohc_validate_and_process_feedback");
    /* SPR 9591 fix start */
    if(ROHC_GET_UPPER_NIBBLE(*msg) != 0xf)
    {
        ROHC_UT_TRACE((stderr, "Resolved packet type is not a feedback"));
        return RO_FAILURE;
    }
    /* SPR 9591 fix end */
    if(ROHC_IP_TCP == ROHC_CSC_GET_PROFILE(p_csc))
    {
        /* feedback validation for profile 6 */
        if(ROHC_FAILURE == rohc_prof6_validate_fb((
                        (rohc_U8bit_t*)msg + 1),
                    in_buff_len - 1, cid_len))
        {
            ROHC_LOG(LOG_INFO,"DMAIN :EID[%u:%u]:"
                    " Feedback validation failed\n",
                    p_rohc_entity->rohc_entity_id.ueIndex
                    ,p_rohc_entity->rohc_entity_id.lcId);

            return RO_FAILURE;
        }
    }
    else
    {
    	/* SPR 13319 fix start */
        fb_code = *msg & ROHC_FB_CODE_MASK;
        if (fb_code)
        {
            /* size field is not present */
            ret_val = rohc_validate_fb(((rohc_U8bit_t*)msg + 1),
                                  in_buff_len - 1, cid_len);
        }
        else
        {
            /* size field is present, so FB data starts 1 octet later */
            ret_val = rohc_validate_fb(((rohc_U8bit_t*)msg + 2),
                                  in_buff_len - 2, cid_len);
        }
        if (ROHC_FAILURE == ret_val)
        {
	    /* SPR 13319 fix end */
            ROHC_LOG(LOG_INFO,"DMAIN :EID[%u:%u]:"
                    " Feedback validation failed\n",
                    p_rohc_entity->rohc_entity_id.ueIndex
                    ,p_rohc_entity->rohc_entity_id.lcId);

            return RO_FAILURE;
        }
    }
    if(ROHC_FAILURE == (*rohc_process_feedback_fpt
                [ROHC_CSC_GET_PROFILE(p_csc) - ROHC_UNCOMP])
            (p_rohc_entity, p_csc,msg,in_buff_len,p_ecode))
    {
        *p_ecode = ROHC_CSC_NOT_EXISTS_FOR_RECEIVED_FB_CID;
        /* SPR 9591 fix start */
        ROHC_LOG(LOG_INFO,
                "MAIN:EID:%u:CSC does not exist for"
                " the CID received in the feedback packet\n",
                p_rohc_entity->rohc_entity_id.ueIndex);
        /* SPR 9591 fix end */
        return RO_FAILURE;
    }
    return RO_SUCCESS;
}

/*******************************************************************************
 **  FUNCTION :  rohc_process_p0_compr_pkt   
 ********************************************************************************
 **  DESCRIPTION :
 **      This function is called to handle the API ROHC_DATA_REQ. The calls all 
 **      the modules of ROHC  to compress the Packet.
 **
 **  INPUT:
 **
 **  1) *p_rohc_entity  = pointer to the rfc 3095 entity, the entry for rohc entity
 **                     in db is made in PDCP. 
 **  2)  *p_csc = pointer to the CSC of the compressor. 
 **  3)  *msg  = pointer to the packet received by compressor.
 **	 4)  header_len = Length of Header 
 **  5)  *p_in_buff  = pointer to the incoming buffer.
 **  6)  *p_ecode = pointer to error code.
 **
 **  RETURN VALUES:
 **      
 **      1. ROHC_SUCCESS:        If action is success.
 **      2. ROHC_FAILURE:        If action is failure.
 **
 *******************************************************************************/
rohc_return_t   rohc_process_p0_compr_pkt   ROHC_PARAMS
(
     /* + SPR 17439 */
 (p_rohc_entity, p_csc_ctx, msg, header_len, p_in_buff, p_ecode),
 rohc_entity_t       *p_rohc_entity _AND_
 rohc_csc_common_t   *p_csc_ctx         _AND_ 
 rohc_U8bit_t        *msg           _AND_ 
 rohc_U32bit_t       header_len     _AND_
 rohc_U8bit_t        *p_in_buff     _AND_
 rohc_error_code_t   *p_ecode
 )
{
    rohc_U8bit_t                pkt_type=0;
    rohc_U32bit_t               pkt_len=0;
    rohc_U8bit_t                len_skip_from_pl=0;
    rohc_csc_uncomp_t *p_csc = NULL;
    p_csc = (rohc_csc_uncomp_t*)p_csc_ctx;
     /* - SPR 17439 */

    ROHC_ENTER_FN("rohc_process_p0_compr_pkt");


    ROHC_MEMSET(rohc_process_p0_compr_pkt_out_buff_p, 0, sizeof(rohc_U8bit_t)*50);
    /* Invoke PRM, 
     * It fills the packet type */
    if (ROHC_FAILURE == rohc_cprm_p0_resolve_pkt_typ(p_rohc_entity, 
                p_csc, &pkt_type,p_ecode))
    {
        ROHC_LOG(LOG_INFO,
                "MAIN:EID:[%u:%u]:CPRM failed while"
                "resolving the packet type\n",
                p_rohc_entity->rohc_entity_id.ueIndex, 
                p_rohc_entity->rohc_entity_id.lcId);
        return ROHC_FAILURE;
    }

    ROHC_UT_TRACE((stderr,"Resolved pkt type = %d\n",pkt_type));
    if ( ROHC_FAILURE == rohc_make_p0_rohc_compr_pkt(p_rohc_entity, p_csc, 
                pkt_type, msg,
                &pkt_len, rohc_process_p0_compr_pkt_out_buff_p,
                &len_skip_from_pl))
        /* +- SPR 17777 */
    {
        ROHC_LOG(LOG_INFO,
                "MAIN:EID:[%u:%u]:CPBM failed while"
                "creating the compressed packet type\n",
                p_rohc_entity->rohc_entity_id.ueIndex, 
                p_rohc_entity->rohc_entity_id.lcId);

        return ROHC_FAILURE;
    }
    /* The CPM is invoked with pointer to entity, pointer to csc 
       and event type */
    if(ROHC_FAILURE == rohc_ccpm_p0_exec(p_rohc_entity, p_csc , 
                                            pkt_type, p_ecode))
    {
        ROHC_LOG(LOG_INFO,
                "MAIN:EID:[%u:%u]:CCPM failed\n",
                p_rohc_entity->rohc_entity_id.ueIndex, 
                p_rohc_entity->rohc_entity_id.lcId);

        return ROHC_FAILURE;
    }
    /*  first octet is a part of packet created by PBM */
    if(pkt_type == ROHC_NORMALP0_PKTYPE)
    {   
        if(p_rohc_entity->large_cids == ROHC_TRUE)
            pkt_len-=2;
        else    
            pkt_len--;
    }

    if( 0 == msgRemove(p_in_buff,0,header_len))
    {
        ROHC_LOG(LOG_INFO,"DMAIN :EID[%u:%u]:"
             " msgRemove failed\n",
             p_rohc_entity->rohc_entity_id.ueIndex,
             p_rohc_entity->rohc_entity_id.lcId);

        ltePanic("%s: msgRemove failed",__func__);

        return ROHC_FAILURE;
    }

    if (PNULL == msgInsert(p_in_buff,0,rohc_process_p0_compr_pkt_out_buff_p,pkt_len))
    {
        ALARM_MSG (PDCP_MODULE_ID, 
             MEM_ALLOC_FAILED_ALARM_ID, 
             CRITICAL_ALARM);

        ROHC_LOG(LOG_INFO,"DMAIN :EID[%u:%u]:"
             " msgInsert failed while making compressed buffer for ROHC\n",
             p_rohc_entity->rohc_entity_id.ueIndex,
             p_rohc_entity->rohc_entity_id.lcId);

        ltePanic("%s: msgInsert failed while making compressed buffer for ROHC",
                                                                      __func__);

        return ROHC_FAILURE;
    }

    ROHC_EXIT_FN("rohc_process_p0_compr_pkt");
    return ROHC_SUCCESS;
}

/*******************************************************************************
 **  FUNCTION :  rohc_process_p1_compr_pkt   
 ********************************************************************************
 **  DESCRIPTION :
 **      This function is called to handle the API ROHC_DATA_REQ. The calls all 
 **      the modules of ROHC  to compress the Packet.
 **
 **  INPUT:
 **
 **  1) *p_rohc_entity  = pointer to the rfc 3095 entity, the entry for rohc entity
 **                     in db is made in PDCP. 
 **  2)  *p_csc = pointer to the CSC of the compressor. 
 **  3)  *msg  = pointer to the packet received by compressor. 
 **	 4)  header_len = Length of Header 
 **  5)  *p_in_buff  = pointer to the incoming buffer.
 **  6)  *p_ecode = pointer to error code.
 **
 **  RETURN VALUES:
 **      
 **      1. ROHC_SUCCESS:        If action is success.
 **      2. ROHC_FAILURE:        If action is failure.
 **
 *******************************************************************************/
rohc_return_t   rohc_process_p1_compr_pkt   ROHC_PARAMS
(
     /* + SPR 17439 */
 (p_rohc_entity, p_csc_ctx, msg, header_len, p_in_buff, p_ecode),
 rohc_entity_t       *p_rohc_entity _AND_
 rohc_csc_common_t   *p_csc_ctx         _AND_ 
 rohc_U8bit_t        *msg           _AND_ 
 rohc_U32bit_t       header_len     _AND_
 rohc_U8bit_t        *p_in_buff     _AND_
 rohc_error_code_t   *p_ecode
 )
{
    rohc_U8bit_t                pkt_type=0;
    rohc_U32bit_t               pkt_len=0;
    rohc_U8bit_t                len_skip_from_pl=0;
    rohc_csc_rtp_t *p_csc = NULL;
    p_csc = (rohc_csc_rtp_t*)p_csc_ctx;
     /* - SPR 17439 */
#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
	/* + PERF_CA */
	InternalCellIndex internalCellIndex = PDCP_GET_CELL_INDEX(p_rohc_entity->rohc_entity_id.ueIndex);
    LteCellPdcpDLPerfStats * lteCellPdcpDLPerfStats_p =
        &(gPdcpCellPerfStats_p[internalCellIndex]->lteCellPdcpDLPerfStats);
	/* - PERF_CA */
#endif


    ROHC_ENTER_FN("rohc_process_p1_compr_pkt");


    ROHC_MEMSET(rohc_process_p1_compr_pkt_out_buff_p, 0, 
                sizeof(rohc_U8bit_t) * ROHC_MAX_HDR_SIZE);
    /* Invoke PRM, 
     * It fills the packet type */

    if (ROHC_FAILURE == rohc_cprm_p1_resolve_pkt_typ(p_rohc_entity, 
                p_csc, &pkt_type,p_ecode))
    {
        p_csc->p_cpib= ROHC_NULL;
        p_csc->rohc_gsw.end_index = 
            (p_csc->rohc_gsw.end_index + ROHC_MAX_LENGTH_GSW - 1)
            %   ROHC_MAX_LENGTH_GSW;
        ROHC_LOG(LOG_INFO,
                "MAIN:EID:[%u:%u]:CPRM failed while"
                "resolving the packet type\n",
                p_rohc_entity->rohc_entity_id.ueIndex, 
                p_rohc_entity->rohc_entity_id.lcId);
        return ROHC_FAILURE;
    }
    ROHC_UT_TRACE((stderr,"Resolved pkt type = %d\n",pkt_type));

    if ( ROHC_FAILURE == rohc_make_p1_rohc_compr_pkt(p_rohc_entity, p_csc, 
                pkt_type, msg,
                &pkt_len, rohc_process_p1_compr_pkt_out_buff_p,
                &len_skip_from_pl, p_ecode))
    {
        p_csc->p_cpib= ROHC_NULL;
        ROHC_LOG(LOG_INFO,
                "MAIN:EID:[%u:%u]:CPBM failed while"
                "creating the compressed packet type\n",
                p_rohc_entity->rohc_entity_id.ueIndex, 
                p_rohc_entity->rohc_entity_id.lcId);

        return ROHC_FAILURE;
    }
    /* The CPM is invoked with pointer to entity, pointer to csc 
       and event type */
    if(ROHC_FAILURE == rohc_ccpm_p1_exec(p_rohc_entity, p_csc , 
                                            pkt_type, p_ecode))
    {
        p_csc->p_cpib= ROHC_NULL;
        ROHC_UT_TRACE((stderr,"CCPM failed\n"));
        ROHC_LOG(LOG_INFO,
                "MAIN:EID:[%u:%u]:CCPM failed\n",
                p_rohc_entity->rohc_entity_id.ueIndex, 
                p_rohc_entity->rohc_entity_id.lcId);

        return ROHC_FAILURE;
    }
    /* Start SPR 6197 Fix*/
    /* This field should always set to 0 regardless of the value sent.
     * This will always be set to 1 in next compressed packet if M bit
     * is set. Refer 5.7 of RFC 3095
     */
    p_csc->field_value[ROHC_RTP_M_FID-ROHC_VAR_FID_BASE] = 0;
    /* End SPR 6197 Fix*/

    /* Start SPR# 6563 Fix*/
    p_csc->field_value[ROHC_RTP_TSS_FID - ROHC_VAR_FID_BASE] = 0;
    /* End SPR# 6563 Fix*/
    /*  first octet is a part of packet created by PBM */
    if(pkt_type == ROHC_NORMALP0_PKTYPE)
    {   
        if(p_rohc_entity->large_cids == ROHC_TRUE)
            pkt_len-=2;
        else    
            pkt_len--;
    }

    if( 0 == msgRemove(p_in_buff,0,header_len))
    {
        ROHC_LOG(LOG_INFO,"DMAIN :EID[%u:%u]:"
             " msgRemove failed\n",
             p_rohc_entity->rohc_entity_id.ueIndex,
             p_rohc_entity->rohc_entity_id.lcId);

        ltePanic("%s: msgRemove failed",__func__);

        return ROHC_FAILURE;
    }

    if (PNULL == msgInsert(p_in_buff,0,rohc_process_p1_compr_pkt_out_buff_p,pkt_len))
    {
        ALARM_MSG (PDCP_MODULE_ID, 
             MEM_ALLOC_FAILED_ALARM_ID, 
             CRITICAL_ALARM);

        ROHC_LOG(LOG_INFO,"DMAIN :EID[%u:%u]:"
             " msgInsert failed while making compressed buffer for ROHC\n",
             p_rohc_entity->rohc_entity_id.ueIndex,
             p_rohc_entity->rohc_entity_id.lcId);

        ltePanic("%s: msgInsert failed while making compressed buffer for ROHC",
                                                                      __func__);

        return ROHC_FAILURE;
    }

#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
    if(p_rohc_entity->rohc_entity_id.qci != 0)
    {
	    /* Update KPI Counter for the Total compressed header volume that has been
	       transmitted in the downlink direction in the PDCP layer */
	    lteCellPdcpDLPerfStats_p->totalRohcCmpHdrVolDL\
		    [p_rohc_entity->rohc_entity_id.qci - 1] += pkt_len ;

	    /* Update KPI Counter for the Total uncompressed header volume that has been
	     *  received in the downlink direction in the PDCP laye */
	    lteCellPdcpDLPerfStats_p->totalRohcUnCmpHdrVolDL\
		    [p_rohc_entity->rohc_entity_id.qci - 1] += header_len;
    }
#endif 
 
    p_csc->p_cpib= ROHC_NULL;

    ROHC_EXIT_FN("rohc_process_p1_compr_pkt");
    return ROHC_SUCCESS;
}

/*******************************************************************************
 **  FUNCTION :  rohc_process_p2_compr_pkt   
 ********************************************************************************
 **  DESCRIPTION :
 **      This function is called to handle the API ROHC_DATA_REQ. The calls all 
 **      the modules of ROHC  to compress the UDP Packet.
 **
 **  INPUT:
 **  1)  *p_rohc_entity  = pointer to the rfc 3095 entity, the entry for rohc entity
 **                     in db is made in PDCP. 
 **  2)  *p_csc = pointer to the CSC of the compressor. 
 **  3)  *msg  = pointer to the packet received by compressor. 
 **	 4)  header_len = Length of Header 
 **  5)  *p_in_buff  = pointer to the incoming buffer.
 **  6)  *p_ecode = pointer to error code.
 **
 **  RETURN VALUES:
 **      
 **      1. ROHC_SUCCESS:        If action is success.
 **      2. ROHC_FAILURE:        If action is failure.
 **
 *******************************************************************************/
rohc_return_t   rohc_process_p2_compr_pkt   ROHC_PARAMS
(
     /* + SPR 17439 */
 (p_rohc_entity, p_csc_ctx, msg, header_len, p_in_buff, p_ecode),
 rohc_entity_t       *p_rohc_entity _AND_
 rohc_csc_common_t      *p_csc_ctx         _AND_ 
 rohc_U8bit_t        *msg           _AND_ 
 rohc_U32bit_t       header_len     _AND_
 rohc_U8bit_t        *p_in_buff     _AND_
 rohc_error_code_t   *p_ecode
 )
{
    rohc_U8bit_t                pkt_type=0;
    rohc_U32bit_t               pkt_len=0;
    rohc_U8bit_t                len_skip_from_pl=0;
    
#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
	/* + PERF_CA */
	InternalCellIndex internalCellIndex = PDCP_GET_CELL_INDEX(p_rohc_entity->rohc_entity_id.ueIndex);
    LteCellPdcpDLPerfStats * lteCellPdcpDLPerfStats_p =\
	    &(gPdcpCellPerfStats_p[internalCellIndex]->lteCellPdcpDLPerfStats);
	/* - PERF_CA */
#endif
    rohc_csc_udp_t *p_csc = NULL;
    p_csc = (rohc_csc_udp_t*)p_csc_ctx;
     /* - SPR 17439 */


    ROHC_ENTER_FN("rohc_process_p2_compr_pkt");


    ROHC_MEMSET(rohc_process_p2_compr_pkt_out_buff_p, 0, 
                sizeof(rohc_U8bit_t) * ROHC_MAX_HDR_SIZE);
    /* Invoke PRM, 
     * It fills the packet type */

    if (ROHC_FAILURE == rohc_cprm_p2_resolve_pkt_typ(p_rohc_entity, 
                p_csc, &pkt_type,p_ecode))
    {
        p_csc->p_cpib= ROHC_NULL;
        p_csc->rohc_gsw.end_index = 
            (p_csc->rohc_gsw.end_index + ROHC_MAX_LENGTH_GSW - 1)
            %   ROHC_MAX_LENGTH_GSW;
        ROHC_LOG(LOG_INFO,
                "MAIN:EID:[%u:%u]:CPRM failed while"
                "resolving the packet type\n",
                p_rohc_entity->rohc_entity_id.ueIndex,
                p_rohc_entity->rohc_entity_id.lcId);
        return ROHC_FAILURE;
    }
    ROHC_UT_TRACE((stderr,"Resolved pkt type = %d\n",pkt_type));

    if ( ROHC_FAILURE == rohc_make_p2_rohc_compr_pkt(p_rohc_entity, p_csc, 
                pkt_type, msg,
                &pkt_len, rohc_process_p2_compr_pkt_out_buff_p,
                &len_skip_from_pl, p_ecode))
    {
        p_csc->p_cpib= ROHC_NULL;
        ROHC_UT_TRACE((stderr,"CPBM failed while"\
                        "creating the compressed packet type\n"));
        ROHC_LOG(LOG_INFO,
                "MAIN:EID:[%u:%u]:CPBM failed while"
                "creating the compressed packet type\n",
                p_rohc_entity->rohc_entity_id.ueIndex, 
                p_rohc_entity->rohc_entity_id.lcId);

        return ROHC_FAILURE;
    }
    /* The CPM is invoked with pointer to entity, pointer to csc 
       and event type */
    if(ROHC_FAILURE == rohc_ccpm_p2_exec(p_rohc_entity, p_csc , 
                                            pkt_type, p_ecode))
    {
        p_csc->p_cpib= ROHC_NULL;
        ROHC_LOG(LOG_INFO,
                "MAIN:EID:[%u:%u]:CCPM failed\n",
                p_rohc_entity->rohc_entity_id.ueIndex, 
                p_rohc_entity->rohc_entity_id.lcId);

        return ROHC_FAILURE;
    }
    /*  first octet is a part of packet created by PBM */
    if(pkt_type == ROHC_NORMALP0_PKTYPE)
    {   
        if(p_rohc_entity->large_cids == ROHC_TRUE)
            pkt_len-=2;
        else    
            pkt_len--;
    }

    if( 0 == msgRemove(p_in_buff,0,header_len))
    {
        ROHC_LOG(LOG_INFO,"DMAIN :EID[%u:%u]:"
             " msgRemove failed\n",
             p_rohc_entity->rohc_entity_id.ueIndex,
             p_rohc_entity->rohc_entity_id.lcId);

        ltePanic("%s: msgRemove failed",__func__);

        return ROHC_FAILURE;
    }

    if (PNULL == msgInsert(p_in_buff,0,rohc_process_p2_compr_pkt_out_buff_p,pkt_len))
    {
        ALARM_MSG (PDCP_MODULE_ID, 
             MEM_ALLOC_FAILED_ALARM_ID, 
             CRITICAL_ALARM);

        ROHC_LOG(LOG_INFO,"DMAIN :EID[%u:%u]:"
             " msgInsert failed while making compressed buffer for ROHC\n",
             p_rohc_entity->rohc_entity_id.ueIndex,
             p_rohc_entity->rohc_entity_id.lcId);

        ltePanic("%s: msgInsert failed while making compressed buffer for ROHC",
                                                                      __func__);

        return ROHC_FAILURE;
    }

 #if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
    if(p_rohc_entity->rohc_entity_id.qci != 0)
    {
	    /* Update KPI Counter for the Total compressed header volume that has been
	       transmitted in the downlink direction in the PDCP layer */
	    lteCellPdcpDLPerfStats_p->totalRohcCmpHdrVolDL\
		    [p_rohc_entity->rohc_entity_id.qci - 1] += pkt_len;

	    /* Update KPI Counter for the Total uncompressed header volume that has been
	     *  received in the downlink direction in the PDCP layer */
	    lteCellPdcpDLPerfStats_p->totalRohcUnCmpHdrVolDL\
		    [p_rohc_entity->rohc_entity_id.qci - 1] += header_len;
    }
#endif 

    p_csc->p_cpib= ROHC_NULL;

    ROHC_EXIT_FN("rohc_process_p2_compr_pkt");
    return ROHC_SUCCESS;
}

/*******************************************************************************
 **  FUNCTION :  rohc_process_p6_compr_pkt   
 ********************************************************************************
 **  DESCRIPTION :
 **      This function is called to handle the API ROHC_DATA_REQ. The calls all 
 **      the modules of ROHC  to compress the UDP Packet.
 **
 **  INPUT:
 **  1)  *p_rohc_entity  = pointer to the rfc 3095 entity, the entry for rohc entity
 **                     in db is made in PDCP. 
 **  2)  *p_csc = pointer to the CSC of the compressor. 
 **  3)  *msg  = pointer to the packet received by compressor. 
 **	 4)  header_len = Length of Header 
 **  5)  *p_in_buff  = pointer to the incoming buffer.
 **  6)  *p_ecode = pointer to error code.
 **
 **  RETURN VALUES:
 **      
 **      1. ROHC_SUCCESS:        If action is success.
 **      2. ROHC_FAILURE:        If action is failure.
 **
 *******************************************************************************/
/* +- SPR 17777 */
rohc_return_t   rohc_process_p6_compr_pkt   ROHC_PARAMS
(
     /* + SPR 17439 */
 (p_rohc_entity, p_csc_ctx, msg, header_len, p_in_buff, p_ecode),
 rohc_entity_t       *p_rohc_entity _AND_
 rohc_csc_common_t      *p_csc_ctx         _AND_ 
 rohc_U8bit_t        *msg           _AND_ 
 rohc_U32bit_t       header_len     _AND_
 rohc_U8bit_t        *p_in_buff     _AND_
 rohc_error_code_t   *p_ecode
 )
{
    rohc_U8bit_t                pkt_type=0;
    rohc_U32bit_t               pkt_len=0;
    
    
#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
	/* + PERF_CA */
	InternalCellIndex internalCellIndex = PDCP_GET_CELL_INDEX(p_rohc_entity->rohc_entity_id.ueIndex);
    LteCellPdcpDLPerfStats * lteCellPdcpDLPerfStats_p =\
	    &(gPdcpCellPerfStats_p[internalCellIndex]->lteCellPdcpDLPerfStats);
	/* - PERF_CA */
#endif

    rohc_csc_tcp_t *p_csc = NULL;
    p_csc = (rohc_csc_tcp_t*)p_csc_ctx;
     /* - SPR 17439 */

    ROHC_ENTER_FN("rohc_process_p6_compr_pkt");


    ROHC_MEMSET(rohc_process_p6_compr_pkt_out_buff_p, 0, 
                sizeof(rohc_U8bit_t) * ROHC_MAX_HDR_SIZE);
    /* Invoke PRM, 
     * It fills the packet type */

    if (ROHC_FAILURE == rohc_cprm_p6_resolve_pkt_typ(p_rohc_entity, 
                p_csc, &pkt_type,p_ecode))
    {
        p_csc->p_cpib= ROHC_NULL;
        p_csc->rohc_gsw.end_index = 
            (p_csc->rohc_gsw.end_index + ROHC_MAX_LENGTH_GSW - 1)
            %   ROHC_MAX_LENGTH_GSW;
        /* +- SPR 17777 */
        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__,sizeof(msg),0,0,0,
                0,0, __func__, "");
    /* +- SPR 17777 */
        ROHC_LOG(LOG_INFO,
                "MAIN:EID:[%u:%u]:CPRM failed while"
                "resolving the packet type\n",
                p_rohc_entity->rohc_entity_id.ueIndex,
                p_rohc_entity->rohc_entity_id.lcId);
        return ROHC_FAILURE;
    }
    ROHC_UT_TRACE((stderr,"Resolved pkt type = %d\n",pkt_type));

    if ( ROHC_FAILURE == rohc_make_p6_rohc_compr_pkt(p_rohc_entity, p_csc, 
                /* +- SPR 17777 */
                pkt_type,
                &pkt_len, rohc_process_p6_compr_pkt_out_buff_p,
                p_ecode))
    {
        p_csc->p_cpib= ROHC_NULL;
        ROHC_UT_TRACE((stderr,"CPBM failed while"\
                        "creating the compressed packet type\n"));
        ROHC_LOG(LOG_INFO,
                "MAIN:EID:[%u:%u]:CPBM failed while"
                "creating the compressed packet type\n",
                p_rohc_entity->rohc_entity_id.ueIndex, 
                p_rohc_entity->rohc_entity_id.lcId);

        return ROHC_FAILURE;
    }

    /* The CPM is invoked with pointer to entity, pointer to csc 
       and event type */
    if(ROHC_FAILURE == rohc_ccpm_p6_exec(p_rohc_entity, p_csc , 
                                            pkt_type, p_ecode))
    {
        p_csc->p_cpib= ROHC_NULL;
        ROHC_LOG(LOG_INFO,
                "MAIN:EID:[%u:%u]:CCPM failed\n",
                p_rohc_entity->rohc_entity_id.ueIndex, 
                p_rohc_entity->rohc_entity_id.lcId);

        return ROHC_FAILURE;
    }


    if( 0 == msgRemove(p_in_buff,0,header_len))
    {
        ROHC_LOG(LOG_INFO,"DMAIN :EID[%u:%u]:"
             " msgRemove failed\n",
             p_rohc_entity->rohc_entity_id.ueIndex,
             p_rohc_entity->rohc_entity_id.lcId);

        ltePanic("%s: msgRemove failed ",__func__);

        return ROHC_FAILURE;
    }

    if (PNULL == msgInsert(p_in_buff,0,rohc_process_p6_compr_pkt_out_buff_p,pkt_len))
    {
        ALARM_MSG (PDCP_MODULE_ID, 
             MEM_ALLOC_FAILED_ALARM_ID, 
             CRITICAL_ALARM);

        ROHC_LOG(LOG_INFO,"DMAIN :EID[%u:%u]:"
             " msgInsert failed while making compressed buffer for ROHC\n",
             p_rohc_entity->rohc_entity_id.ueIndex,
             p_rohc_entity->rohc_entity_id.lcId);

        ltePanic("%s: msgInsert failed while making compressed buffer for" 
                "ROHC", __func__);

        return ROHC_FAILURE;
    }

 #if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
    if(p_rohc_entity->rohc_entity_id.qci != 0)
    {
	    /* Update KPI Counter for the Total compressed header volume that has been
	       transmitted in the downlink direction in the PDCP layer */
	    lteCellPdcpDLPerfStats_p->totalRohcCmpHdrVolDL\
		    [p_rohc_entity->rohc_entity_id.qci - 1] += pkt_len;

	    /* Update KPI Counter for the Total uncompressed header volume that has been
	     *  received in the downlink direction in the PDCP layer */
	    lteCellPdcpDLPerfStats_p->totalRohcUnCmpHdrVolDL\
		    [p_rohc_entity->rohc_entity_id.qci - 1] += header_len;
    }
#endif 

    
    if ( ROHC_NULL != p_csc->p_cpib )
    {
        p_csc->p_cpib= ROHC_NULL;
    }
    ROHC_EXIT_FN("rohc_process_p6_compr_pkt");
    return ROHC_SUCCESS;
}
/*******************************************************************************
 **  FUNCTION :  rohc_process_config_req 
 ********************************************************************************
 **  DESCRIPTION :
 **      This function is invoked when ROHC_CONFIG_REQ comes. This function is 
 **      responsible for filling compression info for RFC 3095 in the entity. 
 **      The entity is created in PDCP itself.
 **
 **  INPUT:
 **
 **  1)  *p_rohc_entity  = pointer to the rfc 3095 entity, the entry for rohc entity
 **                     in db is made in PDCP. 
 **  2)  *p_in_buff  = pointer to the incoming buffer.
 **  3)  **p_p_out_buff = pointer to outgoing buffer, in which ROHC will fill data,
 **                     and return to PDCP.
 **  4)  *p_rohc_api_info  = pointer to the info for different ROHC APIs.
 **  5)  *p_ecode = pointer to error code.
 **
 **  RETURN VALUES:
 **      
 **      1. ROHC_SUCCESS:        If action is success.
 **      2. ROHC_FAILURE:        If action is failure.
 **
 *******************************************************************************/
rohc_return_t   rohc_process_config_req ROHC_PARAMS
(
 (p_rohc_entity, p_in_buff, p_p_out_buff, p_rohc_api_info, p_ecode),
 rohc_entity_t   *p_rohc_entity _AND_
 rohc_U8bit_t    *p_in_buff  _AND_
 rohc_U8bit_t    **p_p_out_buff  _AND_
 rohc_api_info_t *p_rohc_api_info    _AND_
 rohc_error_code_t   *p_ecode
 )
{
    ROHC_ENTER_FN("rohc_process_config_req");

    /* The pointer to pointer to output buffer is used as pointer to entity */
    p_rohc_entity = rohc_db_create_entity();
    /* Since p_p_out_buff is now pointing to rohc_entity. PDCP will update it's record
     * for this rohc enitty by pointing to p_p_out_buff */
     /* COVERITY 63742 Fix Start */
    if (PNULL != p_rohc_entity)
    {
        *p_p_out_buff = (rohc_U8bit_t*)p_rohc_entity;

        /* update the ROHC entity parameters configured from RRC */
        p_rohc_entity->rohc_entity_id.ueIndex = p_rohc_api_info->entity_id.ueIndex ;    
        p_rohc_entity->rohc_entity_id.lcId = p_rohc_api_info->entity_id.lcId ;    

        p_rohc_entity->max_cid = p_rohc_api_info->rohc_api_union.config_req.max_cid ;
        p_rohc_entity->direction = p_rohc_api_info->rohc_api_union.config_req.direction ;
        p_rohc_entity->profilesSupported = p_rohc_api_info->rohc_api_union.config_req.profiles;
        /* update the inferred ROHC entity parameters */
        if (p_rohc_entity->max_cid > ROHC_MAX_CID)
            p_rohc_entity->large_cids = ROHC_TRUE ;
        /* SPR 1635 : Call rohc_db_fb_init after large_cid allocation */
        if (p_rohc_entity->direction == ROHC_DIR_TX)
            rohc_db_fb_init (p_rohc_entity);
        else
            rohc_db_init_decomp_stream(p_rohc_entity);
        ROHC_EXIT_FN("rohc_process_config_req");
        return RO_SUCCESS;
    }
    else
    {
    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,

                __LINE__,sizeof(p_rohc_api_info),sizeof(p_p_out_buff),sizeof(p_in_buff),
                (ADDR)p_ecode,0,0, __func__,
                "");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
        return RO_FAILURE;
    }
    /* COVERITY 63742 Fix End */
}
/*******************************************************************************
 **  FUNCTION :  rohc_process_reset_req 
 ********************************************************************************
 **  DESCRIPTION :
 **      This function is responsible for Resetting all ROHC profile component. 
 **
 **  INPUT:
 **
 **  1) *p_rohc_entity  = pointer to the rfc 3095 entity, the entry for rohc entity
 **                     in db is made in PDCP. 
 **  2)  *p_in_buff  = pointer to the incoming buffer.
 **  3)  **p_p_out_buff = pointer to outgoing buffer, in which ROHC will fill data,
 **                     and return to PDCP.
 **  4)  *p_rohc_api_info  = pointer to the info for different ROHC APIs.
 **  5)  *p_ecode = pointer to error code.
 **
 **  RETURN VALUES:
 **      
 **      1. ROHC_SUCCESS:        If action is success.
 **      2. ROHC_FAILURE:        If action is failure.
 **
 *******************************************************************************/
rohc_return_t   rohc_process_reset_req ROHC_PARAMS
(
 (p_rohc_entity, p_in_buff, p_p_out_buff, p_rohc_api_info, p_ecode),
 rohc_entity_t       *p_rohc_entity     _AND_
 rohc_U8bit_t        *p_in_buff         _AND_
 rohc_U8bit_t        **p_p_out_buff     _AND_
 rohc_api_info_t     *p_rohc_api_info   _AND_
 rohc_error_code_t   *p_ecode
 )
{
    ROHC_ENTER_FN("rohc_process_reset_req");
    
    if (p_rohc_entity->direction == ROHC_DIR_TX)
    {
        rohc_db_delete_all_csc(p_rohc_entity);
        rohc_db_fb_init (p_rohc_entity);   
    } 
    else
    {    
        rohc_db_delete_all_dsc(p_rohc_entity);
        rohc_db_init_decomp_stream(p_rohc_entity);
    }    
    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,sizeof(p_rohc_api_info),sizeof(p_p_out_buff),sizeof(p_in_buff),
            (ADDR)p_ecode,0,0, __func__,
            "");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_process_reset_req");
    return RO_SUCCESS;
}

/*******************************************************************************
 **  FUNCTION :  rohc_process_reconfig_req 
 ********************************************************************************
 **  DESCRIPTION :
 **      This function is the entry point for the RFC3095 Compression entity. 
 **
 **  INPUT:
 **
 **  1)  *p_rohc_entity  = pointer to the rfc 3095 entity, the entry for rohc entity
 **                     in db is made in PDCP. 
 **  2)  *p_in_buff  = pointer to the incoming buffer.
 **  3)  **p_p_out_buff = pointer to outgoing buffer, in which ROHC will fill data,
 **                     and return to PDCP.
 **  4)  *p_rohc_api_info  = pointer to the info for different ROHC APIs.
 **  5)  *p_ecode = pointer to error code.
 **
 **  RETURN VALUES:
 **      
 **      1. ROHC_SUCCESS:        If action is success.
 **      2. ROHC_FAILURE:        If action is failure.
 **
 *******************************************************************************/
rohc_return_t   rohc_process_reconfig_req ROHC_PARAMS
(
 (p_rohc_entity, p_in_buff, p_p_out_buff, p_rohc_api_info, p_ecode),
 rohc_entity_t       *p_rohc_entity     _AND_
 rohc_U8bit_t        *p_in_buff         _AND_
 rohc_U8bit_t        **p_p_out_buff     _AND_
 rohc_api_info_t     *p_rohc_api_info   _AND_
 rohc_error_code_t   *p_ecode
 )
{
  /* SPR 20542 Fix Start */
  if ((p_rohc_entity->profilesSupported == p_rohc_api_info->rohc_api_union.config_req.profiles) &&
      (p_rohc_entity->max_cid == p_rohc_api_info->rohc_api_union.config_req.max_cid) &&
      (p_rohc_entity->direction == p_rohc_api_info->rohc_api_union.config_req.direction))
  {
      return RO_SUCCESS;
  }
  /* SPR 20542 Fix End */
  p_rohc_entity->profilesSupported = 
                    p_rohc_api_info->rohc_api_union.config_req.profiles;

  p_rohc_entity->max_cid = p_rohc_api_info->rohc_api_union.config_req.max_cid;

  p_rohc_entity->direction = 
                    p_rohc_api_info->rohc_api_union.config_req.direction ;

  if (p_rohc_entity->max_cid > ROHC_MAX_CID)
    p_rohc_entity->large_cids = ROHC_TRUE ;

  if (p_rohc_entity->direction == ROHC_DIR_TX)
  {
      /* SPR 20542 Fix Start */
      /*
      ** Delete the CSC context from the Tree, it would be re-create again
      ** in rohc_db_create_csc_new_cid() when new ROHC packet will be received.
      */
      rohc_db_delete_all_csc(p_rohc_entity);
      rohc_db_fb_init (p_rohc_entity);
      /* SPR 20542 Fix End */
  } 
  else
  {    
      /* SPR 20542 Fix Start */
      rohc_db_delete_all_dsc(p_rohc_entity);
      rohc_db_init_decomp_stream(p_rohc_entity);
      /* SPR 20542 Fix End */
    }

  ROHC_EXIT_FN("rohc_process_reconfig_req");
  return RO_SUCCESS;
}

/*******************************************************************************
 **  FUNCTION :  rohc_process_release_req
 ********************************************************************************
 **  DESCRIPTION :
 **      This function is for deleting ROHC entity from the db.
 **
 **  INPUT:
 **
 **  1) *p_rohc_entity  = pointer to the rfc 3095 entity, the entry for rohc entity
 **                     in db is made in PDCP. 
 **  2)  *p_in_buff  = pointer to the incoming buffer.
 **  3)  **p_p_out_buff = pointer to outgoing buffer, in which ROHC will fill data,
 **                     and return to PDCP.
 **  4)  *p_rohc_api_info  = pointer to the info for different ROHC APIs.
 **  5)  *p_ecode = pointer to error code.
 **
 **  RETURN VALUES:
 **      
 **      1. ROHC_SUCCESS:        If action is success.
 **      2. ROHC_FAILURE:        If action is failure.
 **
 *******************************************************************************/
rohc_return_t   rohc_process_release_req    ROHC_PARAMS
(
 (p_rohc_entity, p_in_buff, p_p_out_buff, p_rohc_api_info, p_ecode),
 rohc_entity_t   *p_rohc_entity _AND_
 rohc_U8bit_t    *p_in_buff  _AND_
 rohc_U8bit_t    **p_p_out_buff  _AND_
 rohc_api_info_t *p_rohc_api_info    _AND_
 rohc_error_code_t   *p_ecode
 )
{
  rohc_return_t   ret_val;

  ROHC_ENTER_FN("rohc_process_release_req");

  /* delete the rohc_entity */
  ret_val = rohc_db_delete_entity(p_rohc_entity);

    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,sizeof(p_rohc_api_info),sizeof(p_p_out_buff),sizeof(p_in_buff),
            (ADDR)p_ecode,0,0, __func__,
            "");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
  ROHC_EXIT_FN("rohc_process_release_req");

  /* As ROHC_SUCCESS and PDCP_SUCCESS are different */
  return !ret_val;
}

/*******************************************************************************
 **  FUNCTION :  rohc_process_rlc_data_ind 
 ********************************************************************************
 **  DESCRIPTION :
 **      This function is the entry point for the RFC3095 Compression entity. 
 **
 **  INPUT:
 **
 **  1) *p_rohc_entity  = pointer to the rfc 3095 entity, the entry for rohc entity
 **                     in db is made in PDCP. 
 **  2)  *p_in_buff  = pointer to the incoming buffer.
 **  3)  **p_p_out_buff = pointer to outgoing buffer, in which ROHC will fill data,
 **                     and return to PDCP.
 **  4)  *p_rohc_api_info  = pointer to the info for different ROHC APIs.
 **  5)  *p_ecode = pointer to error code.
 **
 **  RETURN VALUES:
 **      
 **      1. ROHC_SUCCESS:        If action is success.
 **      2. ROHC_FAILURE:        If action is failure.
 **
 *******************************************************************************/
rohc_return_t   rohc_process_rlc_data_ind
(
 rohc_entity_t       *p_rohc_entity,
 rohc_U8bit_t        *p_in_buff,
 rohc_U8bit_t        **p_p_out_buff,
 rohc_api_info_t     *p_rohc_api_info,
 rohc_error_code_t   *p_ecode
 )
{
    rohc_dsc_common_t           *p_dsc=ROHC_NULL;
    rohc_U32bit_t               in_buff_len=0;   
    rohc_U8bit_t                *msg = ROHC_NULL;

    /* START_SPR_576_FIX */
    rohc_U8bit_t                seg_count = 0;
    /* END_SPR_576_FIX */

    ROHC_ENTER_FN("rohc_process_rlc_data_ind");


    in_buff_len = p_rohc_api_info->in_buff_len;

    /*SPR 3444 change start */
    QSEGMENT segment = {0};
    /*SPR 3444 change start */

    /* 
    ** START_SPR_576_FIX:
    ** msgExtract is used instead of msgSegNext because msgSegNext sometimes
    ** does not return us the complete buffer, due to which the decomression
    ** for such packet fails and inturn that packet is dropped.  
    */

    seg_count = msgSegCount(p_in_buff);
    if (seg_count > 1)
    {
        if (0 == msgExtract(p_in_buff, 0, rohc_process_rlc_data_ind_buff_p, 
                            ROHC_MAX_HDR_SIZE))
        {
    /* +- SPR 17777 */
            LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                    __LINE__,sizeof(p_rohc_api_info),sizeof(p_p_out_buff),sizeof(p_in_buff),
                    0,0,0, __func__,
                    "");
    /* +- SPR 17777 */
            ROHC_LOG(LOG_INFO,"DMAIN :EID[%u:%u]:"
                 " msgExtract failed while extracting ZCB\n",
                 p_rohc_entity->rohc_entity_id.ueIndex,
                 p_rohc_entity->rohc_entity_id.lcId);

            ltePanic("%s: msgExtract failed while extracting ZCB",__func__);

            return RO_FAILURE;
        }
        msg = rohc_process_rlc_data_ind_buff_p;
    }
    else
    {
        /* SPR 3444 changes start */
        if (PNULL == msgSegNext(p_in_buff, 0, &segment) )
        {
            return RO_FAILURE;
        }
        /* SPR 3444 changes end */
        msg  = segment.base;
        /*Coverity 94771 Fix Start*/ 
        if(PNULL == msg)
        {
            ROHC_LOG(LOG_INFO,"DMAIN :EID[%u:%u]:"
                    " msgSegNext failed \n",
                    p_rohc_entity->rohc_entity_id.ueIndex,
                    p_rohc_entity->rohc_entity_id.lcId);
            return RO_FAILURE;
        }
        /*Coverity 94771 Fix End*/ 
    }
    /* END_SPR_576_FIX */

    /* + Rohc Debug Log */
    if(rohcLogEnabled_g)
    {
        rohc_U8bit_t dummy_sn[4] = {0};
        rohc_U8bit_t *dummy_sn_p = dummy_sn; 
        /* Here extracting last 4 bytes of buffer which represents seq number in host environment setup */
        if (0 == msgExtract(p_in_buff, (in_buff_len-4), dummy_sn_p, 4))
        {
            ROHC_LOG(LOG_INFO,"DMAIN :EID[%u:%u]: msgExtract failed \n",
                    p_rohc_entity->rohc_entity_id.ueIndex,
                    p_rohc_entity->rohc_entity_id.lcId);
            return RO_FAILURE;
        }
        fprintf(stderr,"[%s] Decompressor: UE[%u] lc[%d] seg_count[%d] seq_num[%0x %0x %0x %0x] UL Compressed Packet: %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x\n",
            __func__, p_rohc_entity->rohc_entity_id.ueIndex, p_rohc_entity->rohc_entity_id.lcId, seg_count, dummy_sn[3],dummy_sn[2],dummy_sn[1],dummy_sn[0],msg[0],msg[1],msg[2],msg[3],msg[4],msg[5],msg[6],msg[7],msg[8],msg[9],msg[10],msg[11],msg[12],msg[13],msg[14],msg[15],msg[16],msg[17],msg[18],msg[19],msg[20],msg[21],msg[22],msg[23],msg[24],msg[25],msg[26],msg[27],msg[28],msg[29],msg[30],msg[31],msg[32],msg[33],msg[34],msg[35],msg[36],msg[37],msg[38],msg[39],msg[40],msg[41],msg[42],msg[43],msg[44],msg[45],msg[46],msg[47],msg[48],msg[49],msg[50],msg[51],msg[52],msg[53],msg[54],msg[55],msg[56],msg[57],msg[58],msg[59],msg[60],msg[61],msg[62],msg[63],msg[64],msg[65],msg[66],msg[67],msg[68],msg[69]);
    }
    /* - Rohc Debug Log */
/* SPR 20149 Start */
    /* SPR 21259 FIX START */
#ifdef PDCP_ASYNC_INTERFACE
#endif
    /* SPR 21259 FIX END */
/* SPR 20149 End */

    /* Here p_p_out is pointing to actual payload */

    /* Invoke D-PPM. 
     *
     * The first parameter points to the actual PAYLOAD ( ROHC) 
     * The second parameter the the length of actual payload
     * The third parameter is the pointer to the ROHC entity */
    if ( ROHC_FAILURE == rohc_d_decode_pkt(msg, in_buff_len,
                p_rohc_entity, &p_dsc, p_ecode))
    {   
        if(p_dsc && p_dsc->dpib)
        {
            if(*p_ecode == ROHC_INVALID_PKT)
            {
                switch(p_dsc->profile)
                {
                    case ROHC_IP_UDP_RTP :
                    {
                        /* + SPR 17439 */
                        if (ROHC_FAILURE == rohc_dcpm_p1_exec (p_rohc_entity , 
                                   (rohc_dsc_rtp_t *)p_dsc, p_dsc->dpib->rcv_pkt_typ, p_ecode))
                        /* - SPR 17439 */
                        { 
                            ROHC_UT_TRACE((stderr," DCPM failed"\
                                 "{error-code [%u]}\n",*p_ecode));
                            ROHC_LOG(LOG_INFO,
                            "MAIN:EID:[%u:%u]:DCPM failed "\
                            "{error-code [%u]}\n",
                            p_rohc_entity->rohc_entity_id.ueIndex,
                            p_rohc_entity->rohc_entity_id.lcId,*p_ecode);
                        } 
                    break;
                    }

                    case ROHC_IP_UDP :
                    {
                        /* + SPR 17439 */
                        if (ROHC_FAILURE == rohc_dcpm_p2_exec (p_rohc_entity , 
                                     (rohc_dsc_udp_t *)p_dsc, p_dsc->dpib->rcv_pkt_typ, p_ecode))
                        /* - SPR 17439 */
                        { 
                            ROHC_UT_TRACE((stderr," DCPM failed"\
                                 "{error-code [%u]}\n",*p_ecode));
                            ROHC_LOG(LOG_INFO,
                            "MAIN:EID:[%u:%u]:DCPM failed "\
                            "{error-code [%u]}\n",
                            p_rohc_entity->rohc_entity_id.ueIndex,
                            p_rohc_entity->rohc_entity_id.lcId,*p_ecode);
                        } 
                    break;
                    }

                    case ROHC_IP_TCP :
                    {
                        if (ROHC_FAILURE == rohcDcpmP6Exec (p_rohc_entity ,
                                   (rohc_dsc_tcp_t *) p_dsc,
                                   p_dsc->dpib->rcv_pkt_typ, p_ecode))
                        {
                            ROHC_UT_TRACE((stderr," DCPM failed"\
                                 "{error-code [%u]}\n",*p_ecode));
                            ROHC_LOG(LOG_INFO,
                            "MAIN:EID:[%u:%u]:DCPM failed "\
                            "{error-code [%u]}\n",
                            p_rohc_entity->rohc_entity_id.ueIndex,
                            p_rohc_entity->rohc_entity_id.lcId,*p_ecode);
                        }
                    break;
                    }
                    case ROHC_UNCOMP :
                    {
                        /* + SPR 17439 */
                        if (ROHC_FAILURE == rohc_dcpm_p0_exec (p_rohc_entity , 
                                     (rohc_dsc_uncomp_t*)p_dsc, p_dsc->dpib->rcv_pkt_typ, p_ecode))
                        /* + SPR 17439 */
                        {
                        } 
                    break;
                    }
                    default:
                            ROHC_LOG(LOG_INFO,
                            "MAIN:EID:[%u:%u]:DCPM failed "\
                            "{error-code [%u]}\n",
                            p_rohc_entity->rohc_entity_id.ueIndex,
                            p_rohc_entity->rohc_entity_id.lcId,*p_ecode);
                    break;
                }
                /* if any feedback has to be sent */
                if(p_rohc_entity->fb_info.len)
                {      
                    p_rohc_entity->fb_in_process = ROHC_TRUE;
                    rohc_send_feedback_to_pdcp(p_rohc_entity);
                    p_rohc_entity->fb_info.len =0;
                }   
            }
            else
            {
                if (ROHC_IP_TCP == p_dsc->dpib->profile)	
                {   
                    /***** feedback processing */
                    /* Coverity Fix 42193 start */
                    if(ROHC_FAILURE ==rohcDcpmP6Exec (p_rohc_entity ,
                                (rohc_dsc_tcp_t *) p_dsc,
                                p_dsc->dpib->rcv_pkt_typ, p_ecode))
                    {
                        ROHC_UT_TRACE((stderr," DCPM failed"\
                                    "{error-code [%u]}\n",*p_ecode));
                        ROHC_LOG(LOG_INFO,
                                "MAIN:EID:[%u:%u]:DCPM failed "\
                                "{error-code [%u]}\n",
                                p_rohc_entity->rohc_entity_id.ueIndex,
                                p_rohc_entity->rohc_entity_id.lcId,*p_ecode);
                    }
                    /* Coverity Fix 42193 end */


                    if(p_rohc_entity->fb_info.len)
                    {   
                        p_rohc_entity->fb_in_process = ROHC_TRUE;
                        rohc_send_feedback_to_pdcp(p_rohc_entity);
                        p_rohc_entity->fb_info.len =0;
                    }
                } 
            }   
            p_dsc->dpib = ROHC_NULL;
        }
        ROHC_UT_TRACE((stderr," DPPM failed"\
                    "{error-code [%u]}\n",*p_ecode));
        ROHC_UT_TRACE((stderr,"DPPM failed while"
                    "parsing the received ROHC-packet"\
                    " {error-code [%u]}\n",*p_ecode));
        ROHC_LOG(LOG_INFO,
                "MAIN:EID:[%u:%u]:DPPM failed while"
                "parsing the received ROHC-packet"\
                " {error-code [%u]}\n",
                p_rohc_entity->rohc_entity_id.ueIndex,
                p_rohc_entity->rohc_entity_id.lcId,*p_ecode);
        return RO_FAILURE;
    }
    if(p_dsc && p_dsc->dpib)
    {
        /* SPR 3444 changes start */
        if (!(( ROHC_IP_UDP_RTP  == p_dsc->profile)
                    || ( ROHC_UNCOMP  == p_dsc->profile )
                    || ( ROHC_IP_UDP == p_dsc->profile )
                    || ( ROHC_IP_TCP  == p_dsc->profile )))
            /* SPR 3444 changes end */
        {
            /* SPR 2655 changes start */
            p_dsc->dpib = ROHC_NULL;
            /* SPR 2655 changes end */
            return ROHC_FAILURE;
        }
        if(ROHC_FAILURE == (*rohc_process_decompr_packet_fpt
                    [p_dsc->profile - ROHC_UNCOMP])
                /* +- SPR 17777 */
                (p_rohc_entity, p_dsc,p_in_buff, 
                 p_rohc_api_info, p_ecode))
        {
            ROHC_UT_TRACE((stderr," Parsing failed while"\
                               "parsing the received ROHC-packet"\
                               "{error-code [%u]}\n",*p_ecode));
            ROHC_LOG(LOG_INFO,
                "MAIN:EID:[%u:%u]:Parsing failed while"
                "parsing the received ROHC-packet"\
                " {error-code [%u]}\n",
                p_rohc_entity->rohc_entity_id.ueIndex,
                p_rohc_entity->rohc_entity_id.lcId,*p_ecode);

           /* SPR 2655 changes start */
            p_dsc->dpib = ROHC_NULL;
           /* SPR 2655 changes end */
            return RO_FAILURE;
        }
        /* + Rohc Debug Log */
        if(rohcLogEnabled_g)
        {
            rohc_U8bit_t dummy_msg_1[70] = {0};
            rohc_U8bit_t *dummy_msg = dummy_msg_1;
            if (0 == msgExtract(p_in_buff, 0, dummy_msg,70))
            {
		ROHC_LOG(LOG_INFO, "MsgExtract Failed");
                return RO_FAILURE;
            }

            fprintf(stderr,"[%s] Decompressor: UE[%u] lc[%d] UL Uncompressed Packet:%0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x\n", __func__, p_rohc_entity->rohc_entity_id.ueIndex, p_rohc_entity->rohc_entity_id.lcId, dummy_msg[0],dummy_msg[1],dummy_msg[2],dummy_msg[3],dummy_msg[4],dummy_msg[5],dummy_msg[6],dummy_msg[7],dummy_msg[8],dummy_msg[9],dummy_msg[10],dummy_msg[11],dummy_msg[12],dummy_msg[13],dummy_msg[14],dummy_msg[15],dummy_msg[16],dummy_msg[17],dummy_msg[18],dummy_msg[19],dummy_msg[20],dummy_msg[21],dummy_msg[22],dummy_msg[23],dummy_msg[24],dummy_msg[25],dummy_msg[26],dummy_msg[27],dummy_msg[28],dummy_msg[29],dummy_msg[30],dummy_msg[31],dummy_msg[32],dummy_msg[33],dummy_msg[34],dummy_msg[35],dummy_msg[36],dummy_msg[37],dummy_msg[38],dummy_msg[39],dummy_msg[40],dummy_msg[41],dummy_msg[42],dummy_msg[43],dummy_msg[44],dummy_msg[45],dummy_msg[46],dummy_msg[47],dummy_msg[48],dummy_msg[49],dummy_msg[50],dummy_msg[51],dummy_msg[52],dummy_msg[53],dummy_msg[54],dummy_msg[55],dummy_msg[56],dummy_msg[57],dummy_msg[58],dummy_msg[59],dummy_msg[60],dummy_msg[61],dummy_msg[62],dummy_msg[63],dummy_msg[64],dummy_msg[65],dummy_msg[66],dummy_msg[67],dummy_msg[68],dummy_msg[69]);
        }
        /* - Rohc Debug Log */
    }

    /***** feedback processing */
    if(p_rohc_entity->fb_info.len)
    {   
        p_rohc_entity->fb_in_process = ROHC_TRUE;
        rohc_send_feedback_to_pdcp(p_rohc_entity);
        p_rohc_entity->fb_info.len =0;
    }   
    /* SPR 2655 changes start */
    /* SPR 3054 changes start */
    /*SPR4126_Coverity_fixes*/
    if (ROHC_NULL != p_dsc)
    {
    /*SPR4126_Coverity_fixes*/
    /* SPR 3054 changes end */
        p_dsc->dpib = ROHC_NULL;
    }
    /* SPR 2655 changes end */
    ROHC_EXIT_FN("rohc_process_rlc_data_ind");
    return RO_SUCCESS;
}

/*******************************************************************************
 **  FUNCTION :  rohc_process_p0_decompr_pkt 
 ********************************************************************************
 **  DESCRIPTION :
 **      This function is the entry point for the RFC3095 Compression entity. 
 **
 **  INPUT:
 **
 **  1) *p_rohc_entity  = pointer to the rfc 3095 entity, the entry for rohc entity
 **                     in db is made in PDCP. 
 **  2)  *p_dsc = pointer to the DSC of the de-compressor. 
 **  3)  *msg  = pointer to the packet received by de-compressor. 
 **  4)  *p_in_buff  = pointer to the incoming buffer.
 **  5)  *p_api_buff_info  = pointer to the info for different ROHC APIs.
 **  6)  *p_ecode = pointer to error code.
 **
 **  RETURN VALUES:
 **      
 **      1. ROHC_SUCCESS:        If action is success.
 **      2. ROHC_FAILURE:        If action is failure.
 **
 *******************************************************************************/
rohc_return_t   rohc_process_p0_decompr_pkt   ROHC_PARAMS
(
     /* + SPR 17439 */
 /* +- SPR 17777 */
 (p_rohc_entity, p_dsc_ctx,p_in_buff, p_rohc_api_info, p_ecode),
 rohc_entity_t       *p_rohc_entity      _AND_
 rohc_dsc_common_t   *p_dsc_ctx              _AND_ 
 rohc_U8bit_t        *p_in_buff          _AND_
 rohc_api_info_t     *p_rohc_api_info    _AND_
 rohc_error_code_t   *p_ecode
 )
{
    /* +- SPR 17777 */
    ROHC_ENTER_FN("rohc_process_p0_decompr_pkt");

    /* No Need to invoke DPRM module here as CRC verification and context update
     * is not needed for uncompressed profile*/
    rohc_dsc_uncomp_t *p_dsc = NULL;
    p_dsc = (rohc_dsc_uncomp_t*)p_dsc_ctx;
     /* - SPR 17439 */
    /* Invoke D-CPM */
    rohc_dcpm_p0_exec (p_rohc_entity, p_dsc,
            p_dsc->dpib->rcv_pkt_typ, p_ecode);
    if( 0 == msgRemove(p_in_buff,0,
                p_rohc_api_info->in_buff_len-p_dsc->dpib->payload_len))
    {
        ROHC_LOG(LOG_INFO,"DMAIN :EID[%u:%u]:"
                " msgRemove failed\n",
                p_rohc_entity->rohc_entity_id.ueIndex,
                p_rohc_entity->rohc_entity_id.lcId);

        ltePanic("%s: msgRemove failed",__func__);

        return ROHC_FAILURE;
    }
    p_dsc->dpib = ROHC_NULL;
    p_rohc_api_info->c_flag = ROHC_DATA_IND_CMP_PKT;

    ROHC_EXIT_FN("rohc_process_p0_decompr_pkt");
    return ROHC_SUCCESS;
}

/*******************************************************************************
 **  FUNCTION :  rohc_process_p1_decompr_pkt
 ********************************************************************************
 **  DESCRIPTION :
 **      This function is the entry point for the RFC3095 Compression entity. 
 **
 **  INPUT:
 **
 **  1) *p_rohc_entity  = pointer to the rfc 3095 entity, the entry for rohc entity
 **                     in db is made in PDCP. 
 **  2)  *p_dsc = pointer to the DSC of the de-compressor. 
 **  3)  *msg  = pointer to the packet received by de-compressor. 
 **  4)  *p_in_buff  = pointer to the incoming buffer.
 **  5)  *p_api_buff_info  = pointer to the info for different ROHC APIs.
 **  6)  *p_ecode = pointer to error code.
 **
 **  RETURN VALUES:
 **      
 **      1. ROHC_SUCCESS:        If action is success.
 **      2. ROHC_FAILURE:        If action is failure.
 **
 *******************************************************************************/
rohc_return_t   rohc_process_p1_decompr_pkt   ROHC_PARAMS
(
     /* + SPR 17439 */
 /* +- SPR 17777 */
 (p_rohc_entity, p_dsc_ctx, p_in_buff, p_rohc_api_info, p_ecode),
 rohc_entity_t       *p_rohc_entity      _AND_
 rohc_dsc_common_t   *p_dsc_ctx              _AND_ 
 rohc_U8bit_t        *p_in_buff          _AND_
 rohc_api_info_t     *p_rohc_api_info    _AND_
 rohc_error_code_t   *p_ecode
 )
{
    rohc_U32bit_t               pkt_len=0;

/* SPR 20149 Start */
    /* SPR 21259 FIX START */
#ifdef PDCP_ASYNC_INTERFACE
#endif    
    /* SPR 21259 FIX END */
/* SPR 20149 End */

#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
	/* + PERF_CA */
	InternalCellIndex internalCellIndex = PDCP_GET_CELL_INDEX(p_rohc_entity->rohc_entity_id.ueIndex);
    LteCellPdcpULPerfStats * lteCellPdcpULPerfStats_p = \
            &(gPdcpCellPerfStats_p[internalCellIndex]->lteCellPdcpULPerfStats);
	/* - PERF_CA */
#endif
    rohc_dsc_rtp_t *p_dsc = NULL;
    p_dsc = (rohc_dsc_rtp_t*)p_dsc_ctx;
     /* - SPR 17439 */
    ROHC_ENTER_FN("rohc_process_p1_decompr_pkt");


    ROHC_MEMSET(rohc_process_p1_decompr_pkt_uncom_msg_p, 0, sizeof(rohc_U8bit_t)*ROHC_MAX_UNCOM_PKT_LEN);

    p_dsc->n_1++;
    p_dsc->n_2++; 
    /* Invoke D-PRM */
    if (ROHC_FAILURE == rohc_dprm_p1_exec (p_rohc_entity, p_dsc, 
                    rohc_process_p1_decompr_pkt_uncom_msg_p, &pkt_len,p_ecode))
    {   
        ROHC_LOG(LOG_INFO,
                "MAIN:EID:[%u:%u]:DPRM failed while "
                "updating the DSC or computing CRC"\
                " or making ip-packets {error-code [%u]}\n",
                p_rohc_entity->rohc_entity_id.ueIndex,
                p_rohc_entity->rohc_entity_id.lcId,*p_ecode);

        if (ROHC_FAILURE == rohc_dcpm_p1_exec (p_rohc_entity , p_dsc, 
                    p_dsc->dpib->rcv_pkt_typ, p_ecode))
        { 
            ROHC_LOG(LOG_INFO,
                    "MAIN:EID:[%u:%u]:DCPM failed "\
                    "{error-code [%u]}\n",
                    p_rohc_entity->rohc_entity_id.ueIndex,
                    p_rohc_entity->rohc_entity_id.lcId,*p_ecode);
        }

        /* if any feedback has to be sent */
        if(p_rohc_entity->fb_info.len)
        {      
            p_rohc_entity->fb_in_process = ROHC_TRUE;
            rohc_send_feedback_to_pdcp(p_rohc_entity);
            p_rohc_entity->fb_info.len =0;
        } 
        p_dsc->dpib = ROHC_NULL;
        return ROHC_FAILURE;
    } 
    
    /* Invoke D-CPM */
    if (ROHC_FAILURE == rohc_dcpm_p1_exec (p_rohc_entity, p_dsc,
                                    p_dsc->dpib->rcv_pkt_typ, p_ecode))
    {   
        /* if any feedback has to be sent */
        if(p_rohc_entity->fb_info.len)
        {      
            p_rohc_entity->fb_in_process = ROHC_TRUE;
            rohc_send_feedback_to_pdcp(p_rohc_entity);
            p_rohc_entity->fb_info.len =0;
        }   
        p_dsc->dpib = ROHC_NULL;
        ROHC_LOG(LOG_INFO,"MAIN:EID:[%u:%u]:DCPM failed "\
                "{error-code [%u]}\n",
                p_rohc_entity->rohc_entity_id.ueIndex,
                p_rohc_entity->rohc_entity_id.lcId,*p_ecode);
        return ROHC_FAILURE;
    }   
#ifdef ROHC_MODE_TRIGGER
    /* check whether mode transition is required or not */
    rohc_check_mode_transition_p1_required (p_rohc_entity, p_dsc);
#endif


/* SPR 20149 Start */
    if( 0 == msgRemove(p_in_buff,0,
             p_rohc_api_info->in_buff_len-p_dsc->dpib->payload_len))
    {
        ROHC_LOG(LOG_INFO,"DMAIN :EID[%u:%u]:"
             " msgRemove failed\n",
             p_rohc_entity->rohc_entity_id.ueIndex,
             p_rohc_entity->rohc_entity_id.lcId);

        ltePanic("%s: msgRemove failed",__func__);

        return ROHC_FAILURE;
    }

    if (PNULL == msgInsert(p_in_buff,0,rohc_process_p1_decompr_pkt_uncom_msg_p,pkt_len))
    {
        ALARM_MSG (PDCP_MODULE_ID, 
             MEM_ALLOC_FAILED_ALARM_ID, 
             CRITICAL_ALARM);

        ROHC_LOG(LOG_INFO,"DMAIN :EID[%u:%u]:"
             " msgInsert failed while making compressed buffer for ROHC\n",
             p_rohc_entity->rohc_entity_id.ueIndex,
             p_rohc_entity->rohc_entity_id.lcId);

        ltePanic("%s: msgInsert failed while making compressed buffer for ROHC",
                                                                     __func__);

        return ROHC_FAILURE;
    }
/* SPR 20149 End */



#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
    if(p_rohc_entity->rohc_entity_id.qci != 0)
    {
	    /* Update KPI Counter for the Total Uncompressed header volume that has been
	       transmitted in the Uplink  direction in the PDCP layer */
	    lteCellPdcpULPerfStats_p->totalRohcUnCmpHdrVolUL\
		    [p_rohc_entity->rohc_entity_id.qci - 1] += pkt_len;
    }

#endif

    p_dsc->dpib = ROHC_NULL;
    p_rohc_api_info->c_flag = ROHC_DATA_IND_CMP_PKT;

    ROHC_EXIT_FN("rohc_process_p1_decompr_pkt");
    return ROHC_SUCCESS;
}

/*******************************************************************************
 **  FUNCTION :  rohc_process_p2_decompr_pkt 
 ********************************************************************************
 **  DESCRIPTION :
 **      This function is the entry point for the RFC3095 Compression entity. 
 **
 **  INPUT:
 **
 **  1) *p_rohc_entity  = pointer to the rfc 3095 entity, the entry for rohc entity
 **                     in db is made in PDCP. 
 **  2)  *p_dsc = pointer to the DSC of the de-compressor. 
 **  3)  *msg  = pointer to the packet received by de-compressor. 
 **  4)  *p_in_buff  = pointer to the incoming buffer.
 **  5)  *p_api_buff_info  = pointer to the info for different ROHC APIs.
 **  6)  *p_ecode = pointer to error code.
 **
 **  RETURN VALUES:
 **      
 **      1. ROHC_SUCCESS:        If action is success.
 **      2. ROHC_FAILURE:        If action is failure.
 **
 *******************************************************************************/
rohc_return_t   rohc_process_p2_decompr_pkt   ROHC_PARAMS
(
     /* + SPR 17439 */
 /* +- SPR 17777 */
 (p_rohc_entity, p_dsc_ctx, p_in_buff, p_rohc_api_info, p_ecode),
 rohc_entity_t       *p_rohc_entity      _AND_
 rohc_dsc_common_t      *p_dsc_ctx              _AND_ 
 rohc_U8bit_t        *p_in_buff          _AND_
 rohc_api_info_t     *p_rohc_api_info    _AND_
 rohc_error_code_t   *p_ecode
 )
{
    rohc_U32bit_t               pkt_len=0;

/* SPR 20149 Start */
/* SPR 20149 End */

#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
	/* + PERF_CA */
	InternalCellIndex internalCellIndex = PDCP_GET_CELL_INDEX(p_rohc_entity->rohc_entity_id.ueIndex);
    LteCellPdcpULPerfStats * lteCellPdcpULPerfStats_p = \
            &(gPdcpCellPerfStats_p[internalCellIndex]->lteCellPdcpULPerfStats);
	/* - PERF_CA */

#endif
    rohc_dsc_udp_t *p_dsc = NULL;
    p_dsc = (rohc_dsc_udp_t*)p_dsc_ctx;
     /* - SPR 17439 */
    ROHC_ENTER_FN("rohc_process_p2_decompr_pkt");


    ROHC_MEMSET(rohc_process_p2_decompr_pkt_uncom_msg_p, 0, 
                sizeof(rohc_U8bit_t)*ROHC_MAX_UNCOM_PKT_LEN);
    p_dsc->n_1++;
    p_dsc->n_2++; 
    /* Invoke D-PRM */
    if (ROHC_FAILURE == rohc_dprm_p2_exec (p_rohc_entity, p_dsc, 
                rohc_process_p2_decompr_pkt_uncom_msg_p, &pkt_len,p_ecode))
    {   
        ROHC_LOG(LOG_INFO,
                "MAIN:EID:[%u:%u]:DPRM failed while "
                "updating the DSC or computing CRC"\
                " or making ip-packets {error-code [%u]}\n",
                p_rohc_entity->rohc_entity_id.ueIndex,
                p_rohc_entity->rohc_entity_id.lcId,*p_ecode);

        if (ROHC_FAILURE == rohc_dcpm_p2_exec (p_rohc_entity , p_dsc, 
                    p_dsc->dpib->rcv_pkt_typ, p_ecode))
        { 
            ROHC_LOG(LOG_INFO,
                    "MAIN:EID:[%u:%u]:DCPM failed "\
                    "{error-code [%u]}\n",
                    p_rohc_entity->rohc_entity_id.ueIndex,
                    p_rohc_entity->rohc_entity_id.lcId,*p_ecode);
        }

        /* if any feedback has to be sent */
        if(p_rohc_entity->fb_info.len)
        {      
            p_rohc_entity->fb_in_process = ROHC_TRUE;
            rohc_send_feedback_to_pdcp(p_rohc_entity);
            p_rohc_entity->fb_info.len =0;
        } 
        p_dsc->dpib = ROHC_NULL;
        return ROHC_FAILURE;
    } 
        
    /* Invoke D-CPM */
    if (ROHC_FAILURE == rohc_dcpm_p2_exec (p_rohc_entity, p_dsc,
                                    p_dsc->dpib->rcv_pkt_typ, p_ecode))
    {   
        /* if any feedback has to be sent */
        if(p_rohc_entity->fb_info.len)
        {      
            p_rohc_entity->fb_in_process = ROHC_TRUE;
            rohc_send_feedback_to_pdcp(p_rohc_entity);
            p_rohc_entity->fb_info.len =0;
        }   
        p_dsc->dpib = ROHC_NULL;
        ROHC_LOG(LOG_INFO,"MAIN:EID:[%u:%u]:DCPM failed "\
                "{error-code [%u]}\n",
                p_rohc_entity->rohc_entity_id.ueIndex,
                p_rohc_entity->rohc_entity_id.lcId,*p_ecode);
        return ROHC_FAILURE;
    }   
#ifdef ROHC_MODE_TRIGGER
    /* check whether mode transition is required or not */
    rohc_check_mode_transition_p2_required (p_rohc_entity, p_dsc);
#endif

/* SPR 20149 Start */
    if( 0 == msgRemove(p_in_buff,0,
             p_rohc_api_info->in_buff_len-p_dsc->dpib->payload_len))
    {
        ROHC_LOG(LOG_INFO,"DMAIN :EID[%u:%u]:"
             " msgRemove failed\n",
             p_rohc_entity->rohc_entity_id.ueIndex,
             p_rohc_entity->rohc_entity_id.lcId);

        ltePanic("%s: msgRemove failed",__func__);

        return ROHC_FAILURE;
    }

    if (PNULL == msgInsert(p_in_buff,0,rohc_process_p2_decompr_pkt_uncom_msg_p,pkt_len))
    {
        ALARM_MSG (PDCP_MODULE_ID, 
             MEM_ALLOC_FAILED_ALARM_ID, 
             CRITICAL_ALARM);

        ROHC_LOG(LOG_INFO,"DMAIN :EID[%u:%u]:"
             " msgInsert failed while making compressed buffer for ROHC\n",
             p_rohc_entity->rohc_entity_id.ueIndex,
             p_rohc_entity->rohc_entity_id.lcId);

        ltePanic("%s: msgInsert failed while making compressed buffer for ROHC",
                                                                      __func__);

        return ROHC_FAILURE;
    }
/* SPR 20149 End */

#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
     if( p_rohc_entity->rohc_entity_id.qci!= 0)
    {
	    /* Update KPI Counter for the Total Uncompressed header volume that has been
	       transmitted in the Uplink  direction in the PDCP layer */
	    lteCellPdcpULPerfStats_p->totalRohcUnCmpHdrVolUL\
		    [p_rohc_entity->rohc_entity_id.qci - 1] += pkt_len;
    }

#endif

    p_dsc->dpib = ROHC_NULL;
    p_rohc_api_info->c_flag = ROHC_DATA_IND_CMP_PKT;

    ROHC_EXIT_FN("rohc_process_p2_decompr_pkt");
    return ROHC_SUCCESS;
}

/*******************************************************************************
 **  FUNCTION :  rohcProcessP6DecomprPkt
 ********************************************************************************
 **  DESCRIPTION :
 **      This function is the entry point for the RFC3095 Compression entity.
 **
 **  INPUT:
 **
 **  1) *p_rohc_entity  = pointer to the rfc 3095 entity, the entry for rohc entity
 **                     in db is made in PDCP.
 **  2)  *p_dec_ctx = pointer to Decompressor CTX
 **  3)  *msg  = pointer to the packet received by de-compressor.
 **  4)  *p_in_buff  = pointer to the incoming buffer.
 **  5)  *p_rohc_api_info  = pointer to the info for different ROHC APIs.
 **  6)  *p_ecode = pointer to error code.
 **
 **  RETURN VALUES:
 **
 **      1. ROHC_SUCCESS:        If action is success.
 **      2. ROHC_FAILURE:        If action is failure.
 **
 *******************************************************************************/
rohc_return_t   rohcProcessP6DecomprPkt
(
     /* + SPR 17439 */
 rohc_entity_t       *p_rohc_entity,
 rohc_dsc_common_t         *p_dec_ctx,
 rohc_U8bit_t        *p_in_buff,
 rohc_api_info_t     *p_rohc_api_info,
 rohc_error_code_t   *p_ecode
 )
{
    /* +- SPR 17777 */

    rohc_U32bit_t               pkt_len=0;
    rohc_dsc_tcp_t *p_dsc = NULL;

/* SPR 20149 Start */
/* SPR 20149 End */

    p_dsc = (rohc_dsc_tcp_t*)p_dec_ctx;
     /* - SPR 17439 */
#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
	/* + PERF_CA */
	InternalCellIndex internalCellIndex = PDCP_GET_CELL_INDEX(p_rohc_entity->rohc_entity_id.ueIndex);
    LteCellPdcpULPerfStats * lteCellPdcpULPerfStats_p = \
            &(gPdcpCellPerfStats_p[internalCellIndex]->lteCellPdcpULPerfStats);
	/* - PERF_CA */

#endif
    ROHC_ENTER_FN("rohcProcessP6DecomprPkt");


    ROHC_MEMSET(rohc_process_p6_decompr_pkt_uncom_msg_p, 0,
                sizeof(rohc_U8bit_t)*ROHC_MAX_UNCOM_PKT_LEN);
    p_dsc->n_1++;
    p_dsc->n_2++;

    /* Invoke D-PRM */
    if (ROHC_FAILURE == rohcDprmP6Exec (p_rohc_entity,p_dsc,
                rohc_process_p6_decompr_pkt_uncom_msg_p, &pkt_len,p_ecode))
    {
        ROHC_LOG(LOG_INFO,
                "MAIN:EID:[%u:%u]:DPRM failed while "
                "updating the DSC or computing CRC"\
                " or making ip-packets {error-code [%u]}\n",
                p_rohc_entity->rohc_entity_id.ueIndex,
                p_rohc_entity->rohc_entity_id.lcId,*p_ecode);

        if (ROHC_FAILURE == rohcDcpmP6Exec (p_rohc_entity ,
                    p_dsc,p_dsc->dpib->rcv_pkt_typ, p_ecode))
        {
            ROHC_LOG(LOG_INFO,
                    "MAIN:EID:[%u:%u]:DCPM failed "\
                    "{error-code [%u]}\n",
                    p_rohc_entity->rohc_entity_id.ueIndex,
                    p_rohc_entity->rohc_entity_id.lcId,*p_ecode);
        }

        /* if any feedback has to be sent */
        if(p_rohc_entity->fb_info.len)
        {
            p_rohc_entity->fb_in_process = ROHC_TRUE;
            rohc_send_feedback_to_pdcp(p_rohc_entity);
            p_rohc_entity->fb_info.len =0;
        }
        p_dsc->dpib = ROHC_NULL;
        return ROHC_FAILURE;
    }

    if (ROHC_FAILURE == rohcDcpmP6Exec (p_rohc_entity, p_dsc,
                p_dsc->dpib->rcv_pkt_typ, p_ecode))
    {
        /* if any feedback has to be sent */
        if(p_rohc_entity->fb_info.len)
        {
            p_rohc_entity->fb_in_process = ROHC_TRUE;
            rohc_send_feedback_to_pdcp(p_rohc_entity);
            p_rohc_entity->fb_info.len =0;
        }
        p_dsc->dpib = ROHC_NULL;
        ROHC_LOG(LOG_INFO,"MAIN:EID:[%u:%u]:DCPM failed "\
                "{error-code [%u]}\n",
                p_rohc_entity->rohc_entity_id.ueIndex,
                p_rohc_entity->rohc_entity_id.lcId,*p_ecode);
        return ROHC_FAILURE;
    }
    
/* SPR 20149 Start */
    if( 0 == msgRemove(p_in_buff,0,
             p_rohc_api_info->in_buff_len-p_dsc->
             dpib->payload_len))
    {
        ROHC_LOG(LOG_INFO,"DMAIN :EID[%u:%u]:"
             " msgRemove failed\n",
             p_rohc_entity->rohc_entity_id.ueIndex,
             p_rohc_entity->rohc_entity_id.lcId);

        ltePanic("%s: msgRemove failed",__func__);

        return ROHC_FAILURE;
    }

    if (PNULL == msgInsert(p_in_buff,0,rohc_process_p6_decompr_pkt_uncom_msg_p,
                pkt_len))
    {
        ALARM_MSG (PDCP_MODULE_ID,
             MEM_ALLOC_FAILED_ALARM_ID,
             CRITICAL_ALARM);

        ROHC_LOG(LOG_INFO,"DMAIN :EID[%u:%u]:"
             " msgInsert failed while making compressed buffer for ROHC\n",
             p_rohc_entity->rohc_entity_id.ueIndex,
             p_rohc_entity->rohc_entity_id.lcId);

        ltePanic("%s: msgInsert failed while making compressed buffer for ROHC",
                                                                      __func__);

        return ROHC_FAILURE;
    }
/* SPR 20149 End */


#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
     if( p_rohc_entity->rohc_entity_id.qci!= 0)
    {
        /* Update KPI Counter for the Total Uncompressed header volume that has
           been transmitted in the Uplink  direction in the PDCP layer */
        lteCellPdcpULPerfStats_p->totalRohcUnCmpHdrVolUL\
            [p_rohc_entity->rohc_entity_id.qci - 1] += pkt_len;
    }

#endif

    p_dsc->dpib = ROHC_NULL;
    p_rohc_api_info->c_flag = ROHC_DATA_IND_CMP_PKT;


    ROHC_EXIT_FN("rohcProcessP6DecomprPkt");
    return ROHC_SUCCESS;
}

/*******************************************************************************
 **  FUNCTION :  rohc_validate_fb   
 ********************************************************************************
 **  DESCRIPTION :
 **      This function is used to validate the feedback packet received for
 **      profile 1/2 packet.
 **
 **  INPUT:
 **
 **  1) *p_pkt  = pointer to the received packet
 **  2)  len  = length of received feedback packet
 **  3)  cid_len  = length of CID
 **
 **  RETURN VALUES:
 **      
 **      1. ROHC_SUCCESS:        If action is success.
 **      2. ROHC_FAILURE:        If action is failure.
 **
 *******************************************************************************/
rohc_return_t rohc_validate_fb ROHC_PARAMS
(
 (p_pkt,len,cid_len), 
 rohc_U8bit_t *p_pkt _AND_
 rohc_U16bit_t len _AND_
 rohc_U8bit_t cid_len
 )
{
    rohc_U8bit_t crc_req=ROHC_FALSE;
    rohc_U8bit_t crc_found=ROHC_FALSE;
    rohc_U8bit_t crc_val= 0;
    rohc_U16bit_t len_read=cid_len;
    if((len - cid_len) == 1)
    {
        return ROHC_SUCCESS;
    }   
    len_read+=(cid_len+1);
    while(len_read < len)
    {   
        switch(((*(p_pkt+(len_read++))&0xf0)>>4))
        {
            case ROHC_FBO_CRC:
                if ( len_read < len )
                {
                    crc_val = *(p_pkt+(len_read));
                    *(p_pkt+(len_read++)) = 0;
                    crc_found=ROHC_TRUE;
                }
                ROHC_LOG(LOG_INFO,
                        "MAIN:CRC option found in the received feedback "\
                        "packet\n");

                break;

            case ROHC_FBO_REJECT:
                crc_req=ROHC_TRUE;
                break;
            case ROHC_FBO_SN_NOT_VALID:
		/* SPR 13319 fix start */
                //crc_req=ROHC_TRUE;
		/* SPR 13319 fix end */
                break;
            case ROHC_FBO_SN      :
                len_read++;
                break;
            case ROHC_FBO_CLOCK   :
                len_read++;
                crc_req=ROHC_TRUE;
                break;
            case ROHC_FBO_JITTER  :
                len_read++;
                crc_req=ROHC_TRUE;
                break;
            case ROHC_FBO_LOSS    :
                len_read++;
                crc_req=ROHC_TRUE;
                break;

        }
    }   
    if(crc_req == ROHC_TRUE || crc_found == ROHC_TRUE)
    {
        if(crc_val == rohc_compute_crc(ROHC_CRC_TYPE_8,(p_pkt),
                    len,0xff))
        {
            ROHC_LOG(LOG_INFO,
                    "MAIN:CRC check is successful on the received "\
                    "feedback packet\n");
            return ROHC_SUCCESS;
        }   
        else
        {
            return ROHC_FAILURE;    
        }
    }   
    return ROHC_SUCCESS;
}   
/*end SPR #: 1391 */        
/*******************************************************************************
 **  FUNCTION :  rohc_prof6_validate_fb   
 ********************************************************************************
 **  DESCRIPTION :
 **      This function is used to validate the feedback packet received for
 **      profile 6 packet.
 **
 **  INPUT:
 **
 **  1) *p_pkt  = pointer to the received packet
 **  2)  len  = length of received feedback packet
 **  3)  cid_len  = length of CID
 **
 **  RETURN VALUES:
 **      
 **      1. ROHC_SUCCESS:        If action is success.
 **      2. ROHC_FAILURE:        If action is failure.
 **
 *******************************************************************************/
rohc_return_t rohc_prof6_validate_fb ROHC_PARAMS
(
 (p_pkt,len,cid_len), 
 rohc_U8bit_t *p_pkt _AND_
 rohc_U16bit_t len _AND_
 rohc_U8bit_t cid_len
 )
{
    rohc_U8bit_t crc_val= 0;
    rohc_U8bit_t fo_reject_opt = 0;
    rohc_U8bit_t fo_msn_not_valid_opt = 0;
    rohc_U8bit_t fo_sn_opt = 0;
    rohc_U8bit_t fo_context_mem_opt = 0;
    rohc_U16bit_t len_read=cid_len;
    if((len - cid_len) == 1)
    {
        /* Feedback 1 */
        return ROHC_SUCCESS;
    }   
    len_read+=(cid_len+1);
    crc_val = *(p_pkt+(len_read));
    *(p_pkt+(len_read++)) = 0; /* for crc computation, crc field is initialised
                                  to zero */
    while(len_read < len)
    {   
        switch(((*(p_pkt+(len_read++))&0xf0)>>4))
        {
            case ROHC_FBO_REJECT:
                if(fo_reject_opt)
                {
                    ROHC_UT_TRACE((stderr,"Failure:FBO REJECT option repeated"
                                " \n"));

                    return ROHC_FAILURE;
                }
                fo_reject_opt++;
                break;

            case ROHC_FBO_MSN_NOT_VALID: /* for prof6, this field corresponds 
                                           to msn */
                if(fo_msn_not_valid_opt || fo_sn_opt)
                    /* this option must not be repeated and must not be present
                     * alongwith SN opt type */ 
                {
                    ROHC_UT_TRACE((stderr,"MSN_NOT_VALID/MSN option already"
                                " received\n"));

                    return ROHC_FAILURE;
                }
                fo_msn_not_valid_opt++;
                break;

            case ROHC_FBO_MSN :
                if(fo_sn_opt || fo_msn_not_valid_opt)
                    /* this option must not be repeated and must not be present
                     * alongwith SN NOT VALID opt type */ 
                {
                    ROHC_UT_TRACE((stderr,"MSN_NOT_VALID/MSN option already"
                                " received\n"));

                    return ROHC_FAILURE;
                }
                len_read++;
                fo_sn_opt++;
                break;

            case ROHC_FBO_CONTEXT_MEM :
                if(fo_context_mem_opt)
                {
                    ROHC_UT_TRACE((stderr,"Failure:CONTEXT MEM option repeated"
                                "\n"));

                    return ROHC_FAILURE;
                }
                fo_context_mem_opt++;
                break;

            default:
                /* unknown option type,keep on parsing remaining elements 
                 * and ignore unknown option type */
                len_read += (*(p_pkt+(len_read - 1))& 0x0f);
        }
    }
    /* perform crc computation */
    if(crc_val == rohc_compute_crc(ROHC_CRC_TYPE_8,(p_pkt),
                len,0xff))
    {
        ROHC_UT_TRACE((stderr,
                    "MAIN:CRC check is successful on the received "\
                    "feedback packet\n"));
    }   
    else
    {
        return ROHC_FAILURE;    
    }
    return ROHC_SUCCESS;
}   

#ifdef ROHC_FUNC_ENHANCEMENT
/*******************************************************************************
 **  FUNCTION :  fill_csc_stream_packet_info
 ********************************************************************************
 **  DESCRIPTION :
 **      This function copies the static fields of CSC to the structure 
 **      stream_pkt_info_t for context Relocation. This function is called
 **      when context info is to be sent.
 **
 **  INPUT:
 **
 **  1) *p_stream_packet_info = The structure to be filled for context relocation.
 **  2) *p_csc = pointer to CSC
 **
 **  RETURN VALUES:
 **      NONE.
 **
 *******************************************************************************/
rohc_void_t fill_csc_stream_packet_info     ROHC_PARAMS
(
 (p_stream_packet_info, p_csc),
 stream_pkt_info_t   *p_stream_packet_info   _AND_
 rohc_csc_t              *p_csc
 )
{
    p_stream_packet_info->cid = p_csc->field_value[ROHC_CID_FID-
        ROHC_VAR_FID_BASE] ;
    p_stream_packet_info->info_type = ROHC_COMPR_INFO_PRESENT ;
    p_stream_packet_info->profile = p_csc->profile;

    switch(p_stream_packet_info->profile)
    {
        case ROHC_UNCOMP :
            break;

        case ROHC_IP_UDP_RTP:

            /* INNER IP info  */
            if(ROHC_INNER_IP4_PRESENT == (p_csc->static_delta & ROHC_INNER_IP4_PRESENT))
            {
                p_stream_packet_info->profile_union.profile_1.inner_ip.ip_version = \
                                                                                    ROHC_VERSION_IP4;

                p_stream_packet_info->profile_union.profile_1.inner_ip.ip_union.ip4.ver =  \
                                                                                           ROHC_VERSION_IP4;
                p_stream_packet_info->profile_union.profile_1.inner_ip.ip_union.ip4.proto = \
                                                                                            p_csc->field_value[ROHC_INNERIP4_PROTO_FID - ROHC_VAR_FID_BASE] ;
                p_stream_packet_info->profile_union.profile_1.inner_ip.ip_union.ip4.src = \
                                                                                          p_csc->field_value[ROHC_INNERIP4_SRC_FID
                                                                                          - ROHC_VAR_FID_BASE] ;
                p_stream_packet_info->profile_union.profile_1.inner_ip.ip_union.ip4.dst = \
                                                                                          p_csc->field_value[ROHC_INNERIP4_DST_FID
                                                                                          - ROHC_VAR_FID_BASE] ;
            } 
            else if(ROHC_INNER_IP6_PRESENT == (p_csc->static_delta & ROHC_INNER_IP6_PRESENT))
            {
                p_stream_packet_info->profile_union.profile_1.inner_ip.ip_version = \
                                                                                    ROHC_VERSION_IP6;

                p_stream_packet_info->profile_union.profile_1.inner_ip.ip_union.ip6.v_flowlabel_nh = \
                                                                                                     (ROHC_LSHIFT(ROHC_VERSION_IP6 , 28) \
                                                                                                      | ( p_csc->field_value[ROHC_INNERIP6_FLOWLABEL_FID - ROHC_VAR_FID_BASE] ) \
                                                                                                      |  p_csc->field_value[ROHC_INNERIP6_NHEADER_FID - ROHC_VAR_FID_BASE]);

                ROHC_MEMCPY((rohc_void_t *) \
                        p_stream_packet_info->profile_union.profile_1.inner_ip.ip_union.ip6.src,
                        (rohc_void_t *)p_csc->innerip6_src, 16);
                ROHC_MEMCPY((rohc_void_t *) \
                        p_stream_packet_info->profile_union.profile_1.inner_ip.ip_union.ip6.dst,
                        (rohc_void_t *)p_csc->innerip6_dst, 16);
            }

            /* OUTER IP info  */
            if(ROHC_OUTER_IP4_PRESENT == (p_csc->static_delta & ROHC_OUTER_IP4_PRESENT))
            {
                p_stream_packet_info->profile_union.profile_1.is_outer_present = \
                                                                                 ROHC_TRUE;

                p_stream_packet_info->profile_union.profile_1.outer_ip.ip_version = \
                                                                                    ROHC_VERSION_IP4;

                p_stream_packet_info->profile_union.profile_1.outer_ip.ip_union.ip4.ver =  \
                                                                                           ROHC_VERSION_IP4;
                p_stream_packet_info->profile_union.profile_1.outer_ip.ip_union.ip4.proto = \
                                                                                            p_csc->field_value[ROHC_OUTERIP4_PROTO_FID
                                                                                            - ROHC_VAR_FID_BASE] ;
                p_stream_packet_info->profile_union.profile_1.outer_ip.ip_union.ip4.src = \
                                                                                          p_csc->field_value[ROHC_OUTERIP4_SRC_FID
                                                                                          - ROHC_VAR_FID_BASE] ;
                p_stream_packet_info->profile_union.profile_1.outer_ip.ip_union.ip4.dst = \
                                                                                          p_csc->field_value[ROHC_OUTERIP4_DST_FID
                                                                                          - ROHC_VAR_FID_BASE] ;
            }
            else if(ROHC_OUTER_IP6_PRESENT == (p_csc->static_delta & ROHC_OUTER_IP6_PRESENT))
            {
                p_stream_packet_info->profile_union.profile_1.is_outer_present = \
                                                                                 ROHC_TRUE;

                p_stream_packet_info->profile_union.profile_1.outer_ip.ip_version = \
                                                                                    ROHC_VERSION_IP6;

                p_stream_packet_info->profile_union.profile_1.outer_ip.ip_union.ip6.v_flowlabel_nh = \
                                                                                                     (ROHC_LSHIFT(ROHC_VERSION_IP6 , 28) \
                                                                                                      | ( p_csc->field_value[ROHC_OUTERIP6_FLOWLABEL_FID - ROHC_VAR_FID_BASE] ) \
                                                                                                      |  p_csc->field_value[ROHC_OUTERIP6_NHEADER_FID - ROHC_VAR_FID_BASE]);

                ROHC_MEMCPY((rohc_void_t *) \
                        p_stream_packet_info->profile_union.profile_1.outer_ip.ip_union.ip6.src,
                        (rohc_void_t *)p_csc->outerip6_src, 16);
                ROHC_MEMCPY((rohc_void_t *) \
                        p_stream_packet_info->profile_union.profile_1.outer_ip.ip_union.ip6.dst,
                        (rohc_void_t *)p_csc->outerip6_dst, 16);
            }

            /* UDP info  */
            p_stream_packet_info->profile_union.profile_1.udp.src = 
                p_csc->field_value[ROHC_UDP_SRC_FID - ROHC_VAR_FID_BASE] ;

            p_stream_packet_info->profile_union.profile_1.udp.dst = \
                                                                    p_csc->field_value[ROHC_UDP_DST_FID
                                                                    - ROHC_VAR_FID_BASE] ;

            /* RTP info  */
            p_stream_packet_info->profile_union.profile_1.rtp.ssrc = \
                                                                     p_csc->field_value[ROHC_RTP_SSRC_FID
                                                                     -  ROHC_VAR_FID_BASE] ;

            break;
        case ROHC_IP_UDP:

            /* INNER IP info  */
            if(ROHC_INNER_IP4_PRESENT == (p_csc->static_delta & ROHC_INNER_IP4_PRESENT))
            {
                p_stream_packet_info->profile_union.profile_2.inner_ip.ip_version = \
                                                                                    ROHC_VERSION_IP4;

                p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip4.ver =  \
                                                                                           ROHC_VERSION_IP4;
                p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip4.proto = \
                                                                                            p_csc->field_value[ROHC_INNERIP4_PROTO_FID - ROHC_VAR_FID_BASE] ;
                p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip4.src = \
                                                                                          p_csc->field_value[ROHC_INNERIP4_SRC_FID - ROHC_VAR_FID_BASE] ;
                p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip4.dst = \
                                                                                          p_csc->field_value[ROHC_INNERIP4_DST_FID - ROHC_VAR_FID_BASE] ;
            } 
            else if(ROHC_INNER_IP6_PRESENT == (p_csc->static_delta & ROHC_INNER_IP6_PRESENT))
            {
                p_stream_packet_info->profile_union.profile_2.inner_ip.ip_version = \
                                                                                    ROHC_VERSION_IP6;

                p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip6.v_flowlabel_nh = \
                                                                                                     (ROHC_LSHIFT(ROHC_VERSION_IP6 , 28) \
                                                                                                      | ( p_csc->field_value[ROHC_INNERIP6_FLOWLABEL_FID - ROHC_VAR_FID_BASE] ) \
                                                                                                      |  p_csc->field_value[ROHC_INNERIP6_NHEADER_FID - ROHC_VAR_FID_BASE]);

                ROHC_MEMCPY((rohc_void_t *) \
                        p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip6.src,
                        (rohc_void_t *)p_csc->innerip6_src, 16);
                ROHC_MEMCPY((rohc_void_t *) \
                        p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip6.dst,
                        (rohc_void_t *)p_csc->innerip6_dst, 16);
            }

            /* OUTER IP info  */
            if(ROHC_OUTER_IP4_PRESENT == (p_csc->static_delta & ROHC_OUTER_IP4_PRESENT))
            {
                p_stream_packet_info->profile_union.profile_2.is_outer_present = \
                                                                                 ROHC_TRUE;

                p_stream_packet_info->profile_union.profile_2.outer_ip.ip_version = \
                                                                                    ROHC_VERSION_IP4;

                p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip4.ver =  \
                                                                                           ROHC_VERSION_IP4;
                p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip4.proto = \
                                                                                            p_csc->field_value[ROHC_OUTERIP4_PROTO_FID - ROHC_VAR_FID_BASE] ;
                p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip4.src = \
                                                                                          p_csc->field_value[ROHC_OUTERIP4_SRC_FID - ROHC_VAR_FID_BASE] ;
                p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip4.dst = \
                                                                                          p_csc->field_value[ROHC_OUTERIP4_DST_FID - ROHC_VAR_FID_BASE] ;
            }
            else if(ROHC_OUTER_IP6_PRESENT == (p_csc->static_delta & ROHC_OUTER_IP6_PRESENT))
            {
                p_stream_packet_info->profile_union.profile_2.is_outer_present = \
                                                                                 ROHC_TRUE;

                p_stream_packet_info->profile_union.profile_2.outer_ip.ip_version = \
                                                                                    ROHC_VERSION_IP6;

                p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip6.v_flowlabel_nh = \
                                                                                                     (ROHC_LSHIFT(ROHC_VERSION_IP6 , 28) \
                                                                                                      | ( p_csc->field_value[ROHC_OUTERIP6_FLOWLABEL_FID - ROHC_VAR_FID_BASE] ) \
                                                                                                      |  p_csc->field_value[ROHC_OUTERIP6_NHEADER_FID - ROHC_VAR_FID_BASE]);

                ROHC_MEMCPY((rohc_void_t *) \
                        p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip6.src,
                        (rohc_void_t *)p_csc->outerip6_src, 16);
                ROHC_MEMCPY((rohc_void_t *) \
                        p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip6.dst,
                        (rohc_void_t *)p_csc->outerip6_dst, 16);
            }

            /* UDP info  */
            p_stream_packet_info->profile_union.profile_2.udp.src = \
                                                                    p_csc->field_value[ROHC_UDP_SRC_FID - ROHC_VAR_FID_BASE] ;

            p_stream_packet_info->profile_union.profile_2.udp.dst = \
                                                                    p_csc->field_value[ROHC_UDP_DST_FID - ROHC_VAR_FID_BASE] ;

            break;

        case ROHC_IP_ESP:

            /* INNER IP info  */
            if(ROHC_INNER_IP4_PRESENT == (p_csc->static_delta & ROHC_INNER_IP4_PRESENT))
            {
                p_stream_packet_info->profile_union.profile_3.inner_ip.ip_version = \
                                                                                    ROHC_VERSION_IP4;

                p_stream_packet_info->profile_union.profile_3.inner_ip.ip_union.ip4.ver =  \
                                                                                           ROHC_VERSION_IP4;
                p_stream_packet_info->profile_union.profile_3.inner_ip.ip_union.ip4.proto = \
                                                                                            p_csc->field_value[ROHC_INNERIP4_PROTO_FID - ROHC_VAR_FID_BASE] ;
                p_stream_packet_info->profile_union.profile_3.inner_ip.ip_union.ip4.src = \
                                                                                          p_csc->field_value[ROHC_INNERIP4_SRC_FID - ROHC_VAR_FID_BASE] ;
                p_stream_packet_info->profile_union.profile_3.inner_ip.ip_union.ip4.dst = \
                                                                                          p_csc->field_value[ROHC_INNERIP4_DST_FID - ROHC_VAR_FID_BASE] ;
            } 
            else if(ROHC_INNER_IP6_PRESENT == (p_csc->static_delta & ROHC_INNER_IP6_PRESENT))
            {
                p_stream_packet_info->profile_union.profile_3.inner_ip.ip_version = \
                                                                                    ROHC_VERSION_IP6;

                p_stream_packet_info->profile_union.profile_3.inner_ip.ip_union.ip6.v_flowlabel_nh = \
                                                                                                     (ROHC_LSHIFT(ROHC_VERSION_IP6 , 28) \
                                                                                                      | ( p_csc->field_value[ROHC_INNERIP6_FLOWLABEL_FID - ROHC_VAR_FID_BASE] ) \
                                                                                                      |  p_csc->field_value[ROHC_INNERIP6_NHEADER_FID - ROHC_VAR_FID_BASE]);

                ROHC_MEMCPY((rohc_void_t *) \
                        p_stream_packet_info->profile_union.profile_3.inner_ip.ip_union.ip6.src,
                        (rohc_void_t *)p_csc->innerip6_src, 16);
                ROHC_MEMCPY((rohc_void_t *) \
                        p_stream_packet_info->profile_union.profile_3.inner_ip.ip_union.ip6.dst,
                        (rohc_void_t *)p_csc->innerip6_dst, 16);
            }

            /* OUTER IP info  */
            if(ROHC_OUTER_IP4_PRESENT == (p_csc->static_delta & ROHC_OUTER_IP4_PRESENT))
            {
                p_stream_packet_info->profile_union.profile_3.is_outer_present = \
                                                                                 ROHC_TRUE;

                p_stream_packet_info->profile_union.profile_3.outer_ip.ip_version = \
                                                                                    ROHC_VERSION_IP4;

                p_stream_packet_info->profile_union.profile_3.outer_ip.ip_union.ip4.ver =  \
                                                                                           ROHC_VERSION_IP4;
                p_stream_packet_info->profile_union.profile_3.outer_ip.ip_union.ip4.proto = \
                                                                                            p_csc->field_value[ROHC_OUTERIP4_PROTO_FID - ROHC_VAR_FID_BASE] ;
                p_stream_packet_info->profile_union.profile_3.outer_ip.ip_union.ip4.src = \
                                                                                          p_csc->field_value[ROHC_OUTERIP4_SRC_FID - ROHC_VAR_FID_BASE] ;
                p_stream_packet_info->profile_union.profile_3.outer_ip.ip_union.ip4.dst = \
                                                                                          p_csc->field_value[ROHC_OUTERIP4_DST_FID - ROHC_VAR_FID_BASE] ;
            }
            else if(ROHC_OUTER_IP6_PRESENT == (p_csc->static_delta & ROHC_OUTER_IP6_PRESENT))
            {
                p_stream_packet_info->profile_union.profile_3.is_outer_present = \
                                                                                 ROHC_TRUE;

                p_stream_packet_info->profile_union.profile_3.outer_ip.ip_version = \
                                                                                    ROHC_VERSION_IP6;

                p_stream_packet_info->profile_union.profile_3.outer_ip.ip_union.ip6.v_flowlabel_nh = \
                                                                                                     (ROHC_LSHIFT(ROHC_VERSION_IP6 , 28) \
                                                                                                      | ( p_csc->field_value[ROHC_OUTERIP6_FLOWLABEL_FID - ROHC_VAR_FID_BASE] ) \
                                                                                                      |  p_csc->field_value[ROHC_OUTERIP6_NHEADER_FID - ROHC_VAR_FID_BASE]);

                ROHC_MEMCPY((rohc_void_t *) \
                        p_stream_packet_info->profile_union.profile_3.outer_ip.ip_union.ip6.src,
                        (rohc_void_t *)p_csc->outerip6_src, 16);
                ROHC_MEMCPY((rohc_void_t *) \
                        p_stream_packet_info->profile_union.profile_3.outer_ip.ip_union.ip6.dst,
                        (rohc_void_t *)p_csc->outerip6_dst, 16);
            }

            /* ESP Info */
            p_stream_packet_info->profile_union.profile_3.esp.spi = \
                                                                    p_csc->field_value[ROHC_ESP_SPI_FID - ROHC_VAR_FID_BASE] ;

            break;
    } /* end of switch */
} /* End of function fill_csc_stream_packet_info */
#endif
#ifdef ROHC_FUNC_ENHANCEMENT
/*******************************************************************************
 **  FUNCTION :  fill_dsc_stream_packet_info
 ********************************************************************************
 **  DESCRIPTION :
 **      This function copies the static fields of DSC to the structure 
 **      stream_pkt_info_t for context Relocation. This function is called
 **      when context info is to be sent.
 **
 **  INPUT:
 **
 **  1) *p_stream_packet_info = The structure to be filled for context relocation.
 **  2) *p_dsc = pointer to DSC
 **
 **  RETURN VALUES:
 **      NONE.
 **
 *******************************************************************************/
rohc_void_t fill_dsc_stream_packet_info     ROHC_PARAMS
(
 (p_stream_packet_info, p_dsc),
 stream_pkt_info_t *p_stream_packet_info _AND_
 rohc_dsc_t           *p_dsc
 )
{
    p_stream_packet_info->cid = p_dsc->cid;
    p_stream_packet_info->info_type = ROHC_DECOMPR_INFO_PRESENT ;
    p_stream_packet_info->profile = p_dsc->profile;

    switch(p_stream_packet_info->profile)
    {
        case ROHC_UNCOMP :
            break;

        case ROHC_IP_UDP_RTP:

            /* INNER  IP info  */
            if(ROHC_NULL != p_dsc->inner_ip4 )
            {
                p_stream_packet_info->profile_union.profile_1.inner_ip.ip_version = \
                                                                                    ROHC_VERSION_IP4;

                p_stream_packet_info->profile_union.profile_1.inner_ip.ip_union.ip4.ver =  \
                                                                                           ROHC_VERSION_IP4;

                p_stream_packet_info->profile_union.profile_1.inner_ip.ip_union.ip4.proto = \
                                                                                            p_dsc->inner_ip4->proto ;

                p_stream_packet_info->profile_union.profile_1.inner_ip.ip_union.ip4.src = \
                                                                                          p_dsc->inner_ip4->src ;

                p_stream_packet_info->profile_union.profile_1.inner_ip.ip_union.ip4.dst = 
                    p_dsc->inner_ip4->dst ;
            }
            else if(ROHC_NULL != p_dsc->inner_ip6 )
            {
                p_stream_packet_info->profile_union.profile_1.inner_ip.ip_version = 
                    ROHC_VERSION_IP6;

                p_stream_packet_info->profile_union.profile_1.inner_ip.ip_union.ip6.v_flowlabel_nh = \
                                                                                                     (ROHC_LSHIFT(ROHC_VERSION_IP6 , 28) \
                                                                                                      | (p_dsc->inner_ip6->ip6_ctlun.ip6_un1.un1_flow) \
                                                                                                      | (p_dsc->inner_ip6->ip6_ctlun.ip6_un1.un1_nxt));

                ROHC_MEMCPY((rohc_void_t *) \
                        p_stream_packet_info->profile_union.profile_1.inner_ip.ip_union.ip6.src,
                        (rohc_void_t *)p_dsc->inner_ip6->src, 16);
                ROHC_MEMCPY((rohc_void_t *) \
                        p_stream_packet_info->profile_union.profile_1.inner_ip.ip_union.ip6.dst,
                        (rohc_void_t *)p_dsc->inner_ip6->dst, 16);
            }

            /* OUTER  IP info  */
            if(ROHC_NULL != p_dsc->outer_ip4 )
            {
                p_stream_packet_info->profile_union.profile_1.is_outer_present = \
                                                                                 ROHC_TRUE;

                p_stream_packet_info->profile_union.profile_1.outer_ip.ip_version = \
                                                                                    ROHC_VERSION_IP4;

                p_stream_packet_info->profile_union.profile_1.outer_ip.ip_union.ip4.ver =  \
                                                                                           ROHC_VERSION_IP4;

                p_stream_packet_info->profile_union.profile_1.outer_ip.ip_union.ip4.proto = \
                                                                                            p_dsc->outer_ip4->proto ;

                p_stream_packet_info->profile_union.profile_1.outer_ip.ip_union.ip4.src = \
                                                                                          p_dsc->outer_ip4->src ;

                p_stream_packet_info->profile_union.profile_1.outer_ip.ip_union.ip4.dst = 
                    p_dsc->outer_ip4->dst ;
            }
            else if(ROHC_NULL != p_dsc->outer_ip6 )
            {
                p_stream_packet_info->profile_union.profile_1.is_outer_present = \
                                                                                 ROHC_TRUE;

                p_stream_packet_info->profile_union.profile_1.outer_ip.ip_version = 
                    ROHC_VERSION_IP6;

                p_stream_packet_info->profile_union.profile_1.outer_ip.ip_union.ip6.v_flowlabel_nh = \
                                                                                                     (ROHC_LSHIFT(ROHC_VERSION_IP6 , 28) \
                                                                                                      | (p_dsc->outer_ip6->ip6_ctlun.ip6_un1.un1_flow) \
                                                                                                      | (p_dsc->outer_ip6->ip6_ctlun.ip6_un1.un1_nxt));

                ROHC_MEMCPY((rohc_void_t *) \
                        p_stream_packet_info->profile_union.profile_1.outer_ip.ip_union.ip6.src,
                        (rohc_void_t *)p_dsc->outer_ip6->src, 16);
                ROHC_MEMCPY((rohc_void_t *) \
                        p_stream_packet_info->profile_union.profile_1.outer_ip.ip_union.ip6.dst,
                        (rohc_void_t *)p_dsc->outer_ip6->dst, 16);
            }

            /* UDP info  */
            p_stream_packet_info->profile_union.profile_1.udp.src = p_dsc->udp->sport ;

            p_stream_packet_info->profile_union.profile_1.udp.dst = p_dsc->udp->dport ;

            /* RTP info  */
            p_stream_packet_info->profile_union.profile_1.rtp.ssrc = p_dsc->rtp->ssrc ;

            break;
        case ROHC_IP_UDP:

            /* INNER  IP info  */
            if(ROHC_NULL != p_dsc->inner_ip4 )
            {
                p_stream_packet_info->profile_union.profile_2.inner_ip.ip_version = \
                                                                                    ROHC_VERSION_IP4;

                p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip4.ver =  \
                                                                                           ROHC_VERSION_IP4;

                p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip4.proto = \
                                                                                            p_dsc->inner_ip4->proto ;

                p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip4.src = \
                                                                                          p_dsc->inner_ip4->src ;

                p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip4.dst = 
                    p_dsc->inner_ip4->dst ;
            }
            else if(ROHC_NULL != p_dsc->inner_ip6 )
            {
                p_stream_packet_info->profile_union.profile_2.inner_ip.ip_version = 
                    ROHC_VERSION_IP6;

                p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip6.v_flowlabel_nh = \
                                                                                                     (ROHC_LSHIFT(ROHC_VERSION_IP6 , 28) \
                                                                                                      | (p_dsc->inner_ip6->ip6_ctlun.ip6_un1.un1_flow) \
                                                                                                      | (p_dsc->inner_ip6->ip6_ctlun.ip6_un1.un1_nxt));

                ROHC_MEMCPY((rohc_void_t *) \
                        p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip6.src,
                        (rohc_void_t *)p_dsc->inner_ip6->src, 16);
                ROHC_MEMCPY((rohc_void_t *) \
                        p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip6.dst,
                        (rohc_void_t *)p_dsc->inner_ip6->dst, 16);
            }

      /* OUTER  IP info  */
      if(ROHC_NULL != p_dsc->outer_ip4 )
      {
        p_stream_packet_info->profile_union.profile_2.is_outer_present = \
                                                                         ROHC_TRUE;

        p_stream_packet_info->profile_union.profile_2.outer_ip.ip_version = \
                                                                            ROHC_VERSION_IP4;

        p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip4.ver =  \
                                                                                   ROHC_VERSION_IP4;

        p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip4.proto = \
                                                                                    p_dsc->outer_ip4->proto ;

        p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip4.src = \
                                                                                  p_dsc->outer_ip4->src ;

        p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip4.dst = 
          p_dsc->outer_ip4->dst ;
      }
      else if(ROHC_NULL != p_dsc->outer_ip6 )
      {
        p_stream_packet_info->profile_union.profile_2.is_outer_present = \
                                                                         ROHC_TRUE;

        p_stream_packet_info->profile_union.profile_2.outer_ip.ip_version = 
          ROHC_VERSION_IP6;

        p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip6.v_flowlabel_nh = \
                                                                                             (ROHC_LSHIFT(ROHC_VERSION_IP6 , 28) \
                                                                                              | (p_dsc->outer_ip6->ip6_ctlun.ip6_un1.un1_flow) \
                                                                                              | (p_dsc->outer_ip6->ip6_ctlun.ip6_un1.un1_nxt));

        ROHC_MEMCPY((rohc_void_t *) \
            p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip6.src,
            (rohc_void_t *)p_dsc->outer_ip6->src, 16);
        ROHC_MEMCPY((rohc_void_t *) \
            p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip6.dst,
            (rohc_void_t *)p_dsc->outer_ip6->dst, 16);
      }

      /* UDP info  */
      p_stream_packet_info->profile_union.profile_2.udp.src = p_dsc->udp->sport ;

      p_stream_packet_info->profile_union.profile_2.udp.dst = p_dsc->udp->dport ;

      break;

    case ROHC_IP_ESP:

      /* INNER  IP info  */
      if(ROHC_NULL != p_dsc->inner_ip4 )
      {
        p_stream_packet_info->profile_union.profile_2.inner_ip.ip_version = \
                                                                            ROHC_VERSION_IP4;

        p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip4.ver =  \
                                                                                   ROHC_VERSION_IP4;

        p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip4.proto = \
                                                                                    p_dsc->inner_ip4->proto ;

        p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip4.src = \
                                                                                  p_dsc->inner_ip4->src ;

        p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip4.dst = 
          p_dsc->inner_ip4->dst ;
      }
      else if(ROHC_NULL != p_dsc->inner_ip6 )
      {
        p_stream_packet_info->profile_union.profile_2.inner_ip.ip_version = 
          ROHC_VERSION_IP6;

        p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip6.v_flowlabel_nh = \
                                                                                             (ROHC_LSHIFT(ROHC_VERSION_IP6 , 28) \
                                                                                              | (p_dsc->inner_ip6->ip6_ctlun.ip6_un1.un1_flow) \
                                                                                              | (p_dsc->inner_ip6->ip6_ctlun.ip6_un1.un1_nxt));

        ROHC_MEMCPY((rohc_void_t *) \
            p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip6.src,
            (rohc_void_t *)p_dsc->inner_ip6->src, 16);
        ROHC_MEMCPY((rohc_void_t *) \
            p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip6.dst,
            (rohc_void_t *)p_dsc->inner_ip6->dst, 16);
      }

      /* OUTER  IP info  */
      if(ROHC_NULL != p_dsc->outer_ip4 )
      {
        p_stream_packet_info->profile_union.profile_3.is_outer_present = \
                                                                         ROHC_TRUE;

        p_stream_packet_info->profile_union.profile_2.outer_ip.ip_version = \
                                                                            ROHC_VERSION_IP4;

        p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip4.ver =  \
                                                                                   ROHC_VERSION_IP4;

        p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip4.proto = \
                                                                                    p_dsc->outer_ip4->proto ;

        p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip4.src = \
                                                                                  p_dsc->outer_ip4->src ;

        p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip4.dst = 
          p_dsc->outer_ip4->dst ;
      }
      else if(ROHC_NULL != p_dsc->outer_ip6 )
      {
        p_stream_packet_info->profile_union.profile_3.is_outer_present = \
                                                                         ROHC_TRUE;

        p_stream_packet_info->profile_union.profile_2.outer_ip.ip_version = 
          ROHC_VERSION_IP6;

        p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip6.v_flowlabel_nh = \
                                                                                             (ROHC_LSHIFT(ROHC_VERSION_IP6 , 28) \
                                                                                              | (p_dsc->outer_ip6->ip6_ctlun.ip6_un1.un1_flow) \
                                                                                              | (p_dsc->outer_ip6->ip6_ctlun.ip6_un1.un1_nxt));

        ROHC_MEMCPY((rohc_void_t *) \
            p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip6.src,
            (rohc_void_t *)p_dsc->outer_ip6->src, 16);
        ROHC_MEMCPY((rohc_void_t *) \
            p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip6.dst,
            (rohc_void_t *)p_dsc->outer_ip6->dst, 16);
      }


      /* ESP Info */
      p_stream_packet_info->profile_union.profile_3.esp.spi = p_dsc->esp->spi ;

      break;
  } /* end of switch */
} /*  End of function fill_dsc_stream_packet_info */

#endif

#ifdef ROHC_FUNC_ENHANCEMENT
/*******************************************************************************
 **  FUNCTION :  fill_static_csc_info
 ********************************************************************************
 **  DESCRIPTION :
 **      This function copies the structure stream_pkt_info_t to the the static 
 **      fields of CSC. This function is called while applying the context info.
 **
 **  INPUT:
 **
 **  1) *p_csc = pointer to CSC.
 **  2) *p_stream_packet_info = The structure to be copied to CSC.
 **
 **  RETURN VALUES:
 **      NONE.
 **
 *******************************************************************************/
rohc_void_t fill_static_csc_info        ROHC_PARAMS
(
 (p_csc, p_stream_packet_info),
 rohc_csc_t          *p_csc          _AND_
 stream_pkt_info_t   *p_stream_packet_info
 )
{
  p_csc->field_value[ROHC_CID_FID-ROHC_VAR_FID_BASE] =
    p_stream_packet_info->cid ;
  p_csc->profile = p_stream_packet_info->profile ;

  switch(p_csc->profile)
  {
    case ROHC_UNCOMP :
      break;

    case ROHC_IP_UDP_RTP:

      /* INNER IP info  */
      if(p_stream_packet_info->profile_union.profile_1.inner_ip.ip_version == ROHC_VERSION_IP4)
      {
        p_csc->static_delta |= ROHC_INNER_IP4_PRESENT ;

        p_csc->field_value[ROHC_INNERIP4_PROTO_FID - ROHC_VAR_FID_BASE] = \
                                                                          p_stream_packet_info->profile_union.profile_1.inner_ip.ip_union.ip4.proto;
        p_csc->field_value[ROHC_INNERIP4_SRC_FID - ROHC_VAR_FID_BASE] = \
                                                                        p_stream_packet_info->profile_union.profile_1.inner_ip.ip_union.ip4.src;
        p_csc->field_value[ROHC_INNERIP4_DST_FID - ROHC_VAR_FID_BASE] = \
                                                                        p_stream_packet_info->profile_union.profile_1.inner_ip.ip_union.ip4.dst;
      }
      else if(p_stream_packet_info->profile_union.profile_1.inner_ip.ip_version == ROHC_VERSION_IP6)
      {
        p_csc->static_delta |= ROHC_INNER_IP6_PRESENT ;

        p_csc->field_value[ROHC_INNERIP6_FLOWLABEL_FID - ROHC_VAR_FID_BASE] = \
        
        ROHC_RSHIFT(((p_stream_packet_info->profile_union.profile_1.inner_ip.ip_union.ip6.v_flowlabel_nh)\
/* SPR 4808 Fix Start */
        & ROHC_IPv6_STATIC_FLOWLABEL_MASK), 8);
/* SPR 4808 Fix End */

        p_csc->field_value[ROHC_INNERIP6_NHEADER_FID - ROHC_VAR_FID_BASE] = \
                                                                            ((p_stream_packet_info->profile_union.profile_1.inner_ip.ip_union.ip6.v_flowlabel_nh) \
                                                                             & ROHC_IPv6_STATIC_NH_MASK);

        ROHC_MEMCPY((rohc_void_t *)p_csc->innerip6_src, \
            (rohc_void_t *) \
            p_stream_packet_info->profile_union.profile_1.inner_ip.ip_union.ip6.src, 16);
        ROHC_MEMCPY((rohc_void_t *)p_csc->innerip6_dst, \
            (rohc_void_t *) \
            p_stream_packet_info->profile_union.profile_1.inner_ip.ip_union.ip6.dst,16);
      }

      if (ROHC_TRUE == p_stream_packet_info->profile_union.profile_1.is_outer_present )
      {
        /* OUTER IP info Present */
        if(p_stream_packet_info->profile_union.profile_1.outer_ip.ip_version == ROHC_VERSION_IP4)
        {
          p_csc->static_delta |= ROHC_OUTER_IP4_PRESENT ;

          p_csc->field_value[ROHC_OUTERIP4_PROTO_FID - ROHC_VAR_FID_BASE] = \
                                                                            p_stream_packet_info->profile_union.profile_1.outer_ip.ip_union.ip4.proto;
          p_csc->field_value[ROHC_OUTERIP4_SRC_FID - ROHC_VAR_FID_BASE] = \
                                                                          p_stream_packet_info->profile_union.profile_1.outer_ip.ip_union.ip4.src;
          p_csc->field_value[ROHC_OUTERIP4_DST_FID - ROHC_VAR_FID_BASE] = \
                                                                          p_stream_packet_info->profile_union.profile_1.outer_ip.ip_union.ip4.dst;
        }
        else if(p_stream_packet_info->profile_union.profile_1.outer_ip.ip_version == ROHC_VERSION_IP6)
        {
          p_csc->static_delta |= ROHC_OUTER_IP6_PRESENT ;

          p_csc->field_value[ROHC_OUTERIP6_FLOWLABEL_FID - ROHC_VAR_FID_BASE] = \
          ROHC_RSHIFT(((p_stream_packet_info->profile_union.profile_1.outer_ip.ip_union.ip6.v_flowlabel_nh)\
/* SPR 4808 Fix Start */
          & ROHC_IPv6_STATIC_FLOWLABEL_MASK), 8);
/* SPR 4808 Fix End */
          p_csc->field_value[ROHC_OUTERIP6_NHEADER_FID - ROHC_VAR_FID_BASE] = \
                                                                              ((p_stream_packet_info->profile_union.profile_1.outer_ip.ip_union.ip6.v_flowlabel_nh) \
                                                                               & ROHC_IPv6_STATIC_NH_MASK);

          ROHC_MEMCPY((rohc_void_t *)p_csc->outerip6_src, \
              (rohc_void_t *) \
              p_stream_packet_info->profile_union.profile_1.outer_ip.ip_union.ip6.src, 16);
          ROHC_MEMCPY((rohc_void_t *)p_csc->outerip6_dst, \
              (rohc_void_t *) \
              p_stream_packet_info->profile_union.profile_1.outer_ip.ip_union.ip6.dst,16);
        }
      }

      /* UDP info  */
      p_csc->field_value[ROHC_UDP_SRC_FID - ROHC_VAR_FID_BASE] = \
                                                                 p_stream_packet_info->profile_union.profile_1.udp.src;

      p_csc->field_value[ROHC_UDP_DST_FID - ROHC_VAR_FID_BASE] = \
                                                                 p_stream_packet_info->profile_union.profile_1.udp.dst;

      /* RTP info  */
      p_csc->field_value[ROHC_RTP_SSRC_FID - ROHC_VAR_FID_BASE] = \
                                                                  p_stream_packet_info->profile_union.profile_1.rtp.ssrc;

      break;
    case ROHC_IP_UDP:

      /* INNER IP info  */
      if(p_stream_packet_info->profile_union.profile_2.inner_ip.ip_version == ROHC_VERSION_IP4)
      {
        p_csc->static_delta |= ROHC_INNER_IP4_PRESENT ;

        p_csc->field_value[ROHC_INNERIP4_PROTO_FID - ROHC_VAR_FID_BASE] = \
                                                                          p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip4.proto;
        p_csc->field_value[ROHC_INNERIP4_SRC_FID - ROHC_VAR_FID_BASE] = \
                                                                        p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip4.src;
        p_csc->field_value[ROHC_INNERIP4_DST_FID - ROHC_VAR_FID_BASE] = \
                                                                        p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip4.dst;
      }
      else if(p_stream_packet_info->profile_union.profile_2.inner_ip.ip_version == ROHC_VERSION_IP6)
      {
        p_csc->static_delta |= ROHC_INNER_IP6_PRESENT ;

        p_csc->field_value[ROHC_INNERIP6_FLOWLABEL_FID - ROHC_VAR_FID_BASE] = \
        ROHC_RSHIFT(((p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip6.v_flowlabel_nh)\
/* SPR 4808 Fix Start */
        & ROHC_IPv6_STATIC_FLOWLABEL_MASK), 8);
/* SPR 4808 Fix End */

        p_csc->field_value[ROHC_INNERIP6_NHEADER_FID - ROHC_VAR_FID_BASE] = \
                                                                            ((p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip6.v_flowlabel_nh) \
                                                                             & ROHC_IPv6_STATIC_NH_MASK);

        ROHC_MEMCPY((rohc_void_t *)p_csc->innerip6_src, \
            (rohc_void_t *) \
            p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip6.src, 16);
        ROHC_MEMCPY((rohc_void_t *)p_csc->innerip6_dst, \
            (rohc_void_t *) \
            p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip6.dst,16);
      }

      if (ROHC_TRUE == p_stream_packet_info->profile_union.profile_2.is_outer_present )
      {
        /* OUTER IP info Present */
        if(p_stream_packet_info->profile_union.profile_2.outer_ip.ip_version == ROHC_VERSION_IP4)
        {
          p_csc->static_delta |= ROHC_OUTER_IP4_PRESENT ;

          p_csc->field_value[ROHC_OUTERIP4_PROTO_FID - ROHC_VAR_FID_BASE] = \
                                                                            p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip4.proto;
          p_csc->field_value[ROHC_OUTERIP4_SRC_FID - ROHC_VAR_FID_BASE] = \
                                                                          p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip4.src;
          p_csc->field_value[ROHC_OUTERIP4_DST_FID - ROHC_VAR_FID_BASE] = \
                                                                          p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip4.dst;
        }
        else if(p_stream_packet_info->profile_union.profile_2.outer_ip.ip_version == ROHC_VERSION_IP6)
        {
          p_csc->static_delta |= ROHC_OUTER_IP6_PRESENT ;

          p_csc->field_value[ROHC_OUTERIP6_FLOWLABEL_FID - ROHC_VAR_FID_BASE] = \

          ROHC_RSHIFT(((p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip6.v_flowlabel_nh)\
/* SPR 4808 Fix Start */
          & ROHC_IPv6_STATIC_FLOWLABEL_MASK), 8);

/* SPR 4808 Fix End */
          p_csc->field_value[ROHC_OUTERIP6_NHEADER_FID - ROHC_VAR_FID_BASE] = \
                                                                              ((p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip6.v_flowlabel_nh) \
                                                                               & ROHC_IPv6_STATIC_NH_MASK);

          ROHC_MEMCPY((rohc_void_t *)p_csc->outerip6_src, \
              (rohc_void_t *) \
              p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip6.src, 16);
          ROHC_MEMCPY((rohc_void_t *)p_csc->outerip6_dst, \
              (rohc_void_t *) \
              p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip6.dst,16);
        }
      }

      /* UDP info  */
      p_csc->field_value[ROHC_UDP_SRC_FID - ROHC_VAR_FID_BASE] = \
                                                                 p_stream_packet_info->profile_union.profile_2.udp.src;

      p_csc->field_value[ROHC_UDP_DST_FID - ROHC_VAR_FID_BASE] = \
                                                                 p_stream_packet_info->profile_union.profile_2.udp.dst;

      break;

    case ROHC_IP_ESP:

      /* INNER IP info  */
      if(p_stream_packet_info->profile_union.profile_3.inner_ip.ip_version == ROHC_VERSION_IP4)
      {
        p_csc->static_delta |= ROHC_INNER_IP4_PRESENT ;

        p_csc->field_value[ROHC_INNERIP4_PROTO_FID - ROHC_VAR_FID_BASE] = \
                                                                          p_stream_packet_info->profile_union.profile_3.inner_ip.ip_union.ip4.proto;
        p_csc->field_value[ROHC_INNERIP4_SRC_FID - ROHC_VAR_FID_BASE] = \
                                                                        p_stream_packet_info->profile_union.profile_3.inner_ip.ip_union.ip4.src;
        p_csc->field_value[ROHC_INNERIP4_DST_FID - ROHC_VAR_FID_BASE] = \
                                                                        p_stream_packet_info->profile_union.profile_3.inner_ip.ip_union.ip4.dst;
      }
      else if(p_stream_packet_info->profile_union.profile_3.inner_ip.ip_version == ROHC_VERSION_IP6)
      {
        p_csc->static_delta |= ROHC_INNER_IP6_PRESENT ;

        p_csc->field_value[ROHC_INNERIP6_FLOWLABEL_FID - ROHC_VAR_FID_BASE] = \
        ROHC_RSHIFT(((p_stream_packet_info->profile_union.profile_3.inner_ip.ip_union.ip6.v_flowlabel_nh)\
/* SPR 4808 Fix Start */
        & ROHC_IPv6_STATIC_FLOWLABEL_MASK), 8);
/* SPR 4808 Fix End */

        p_csc->field_value[ROHC_INNERIP6_NHEADER_FID - ROHC_VAR_FID_BASE] = \
                                                                            ((p_stream_packet_info->profile_union.profile_3.inner_ip.ip_union.ip6.v_flowlabel_nh) \
                                                                             & ROHC_IPv6_STATIC_NH_MASK);

        ROHC_MEMCPY((rohc_void_t *)p_csc->innerip6_src, \
            (rohc_void_t *) \
            p_stream_packet_info->profile_union.profile_3.inner_ip.ip_union.ip6.src, 16);
        ROHC_MEMCPY((rohc_void_t *)p_csc->innerip6_dst, \
            (rohc_void_t *) \
            p_stream_packet_info->profile_union.profile_3.inner_ip.ip_union.ip6.dst,16);
      }

      if (ROHC_TRUE == p_stream_packet_info->profile_union.profile_3.is_outer_present )
      {
        /* OUTER IP info Present */
        if(p_stream_packet_info->profile_union.profile_3.outer_ip.ip_version == ROHC_VERSION_IP4)
        {
          p_csc->static_delta |= ROHC_OUTER_IP4_PRESENT ;

          p_csc->field_value[ROHC_OUTERIP4_PROTO_FID - ROHC_VAR_FID_BASE] = \
                                                                            p_stream_packet_info->profile_union.profile_3.outer_ip.ip_union.ip4.proto;
          p_csc->field_value[ROHC_OUTERIP4_SRC_FID - ROHC_VAR_FID_BASE] = \
                                                                          p_stream_packet_info->profile_union.profile_3.outer_ip.ip_union.ip4.src;
          p_csc->field_value[ROHC_OUTERIP4_DST_FID - ROHC_VAR_FID_BASE] = \
                                                                          p_stream_packet_info->profile_union.profile_3.outer_ip.ip_union.ip4.dst;
        }
        else if(p_stream_packet_info->profile_union.profile_3.outer_ip.ip_version == ROHC_VERSION_IP6)
        {
          p_csc->static_delta |= ROHC_OUTER_IP6_PRESENT ;

          p_csc->field_value[ROHC_OUTERIP6_FLOWLABEL_FID - ROHC_VAR_FID_BASE] = \
          ROHC_RSHIFT(((p_stream_packet_info->profile_union.profile_3.outer_ip.ip_union.ip6.v_flowlabel_nh)\
/* SPR 4808 Fix Start */
          & ROHC_IPv6_STATIC_FLOWLABEL_MASK), 8);
/* SPR 4808 Fix End */
          p_csc->field_value[ROHC_OUTERIP6_NHEADER_FID - ROHC_VAR_FID_BASE] = \
                                                                              ((p_stream_packet_info->profile_union.profile_3.outer_ip.ip_union.ip6.v_flowlabel_nh) \
                                                                               & ROHC_IPv6_STATIC_NH_MASK);

          ROHC_MEMCPY((rohc_void_t *)p_csc->outerip6_src, \
              (rohc_void_t *) \
              p_stream_packet_info->profile_union.profile_3.outer_ip.ip_union.ip6.src, 16);
          ROHC_MEMCPY((rohc_void_t *)p_csc->outerip6_dst, \
              (rohc_void_t *) \
              p_stream_packet_info->profile_union.profile_3.outer_ip.ip_union.ip6.dst,16);
        }
      }

      /* ESP info  */
      p_csc->field_value[ROHC_ESP_SPI_FID - ROHC_VAR_FID_BASE]  =
        p_stream_packet_info->profile_union.profile_3.esp.spi ;

      break;
  } /* end of switch */
} /* End of function fill_static_csc_info */
#endif

#ifdef ROHC_FUNC_ENHANCEMENT
/*******************************************************************************
 **  FUNCTION :  fill_static_dsc_info
 ********************************************************************************
 **  DESCRIPTION :
 **      This function copies the structure stream_pkt_info_t to the the static 
 **      fields of DSC. This function is called while applying the context info.
 **
 **  INPUT:
 **
 **  1) *p_dsc = pointer to DSC.
 **  2) *p_stream_packet_info = The structure to be copied to CSC.
 **
 **  RETURN VALUES:
 **      NONE.
 **
 *******************************************************************************/
rohc_void_t fill_static_dsc_info        ROHC_PARAMS
(
 (p_dsc, p_stream_packet_info),
 rohc_dsc_t          *p_dsc          _AND_
 stream_pkt_info_t   *p_stream_packet_info
 )
{
  p_dsc->cid = p_stream_packet_info->cid ;

  p_dsc->profile = p_stream_packet_info->profile ;

  switch(p_dsc->profile)
  {
    case ROHC_UNCOMP :
      break;

    case ROHC_IP_UDP_RTP:

      /* INNER IP info  */
      if(p_stream_packet_info->profile_union.profile_1.inner_ip.ip_version == ROHC_VERSION_IP4)
      {
        p_dsc->inner_ip4->vhl = \
                                ((ROHC_LSHIFT(ROHC_VERSION_IP4, 4) & ROHC_IP4_STATIC_VER_MASK));

        p_dsc->inner_ip4->proto = \
                                  p_stream_packet_info->profile_union.profile_1.inner_ip.ip_union.ip4.proto;

        p_dsc->inner_ip4->src = \
                                p_stream_packet_info->profile_union.profile_1.inner_ip.ip_union.ip4.src;

        p_dsc->inner_ip4->dst = \
                                p_stream_packet_info->profile_union.profile_1.inner_ip.ip_union.ip4.dst;
      }
      else if(p_stream_packet_info->profile_union.profile_1.inner_ip.ip_version == ROHC_VERSION_IP6)
      {
        p_dsc->inner_ip6->ip6_ctlun.ip6_un1.un1_flow = \
                                                       (ROHC_LSHIFT(ROHC_VERSION_IP6, 28) | \
                                                        (p_stream_packet_info->profile_union.profile_1.inner_ip.ip_union.ip6.v_flowlabel_nh \
                                                         & ROHC_IP6_FLOWLABEL_MASK));

        ROHC_MEMCPY((rohc_void_t *)p_dsc->inner_ip6->src, \
            (rohc_void_t *) \
            p_stream_packet_info->profile_union.profile_1.inner_ip.ip_union.ip6.src, 16);
        ROHC_MEMCPY((rohc_void_t *)p_dsc->inner_ip6->dst, 
            (rohc_void_t *) \
            p_stream_packet_info->profile_union.profile_1.inner_ip.ip_union.ip6.dst,16);
      }

      if( ROHC_TRUE == p_stream_packet_info->profile_union.profile_1.is_outer_present)
      {
        /* OUTER IP info present */
        if(p_stream_packet_info->profile_union.profile_1.outer_ip.ip_version == ROHC_VERSION_IP4)
        {
          p_dsc->outer_ip4->vhl = \
                                  ((ROHC_LSHIFT(ROHC_VERSION_IP4, 4) & ROHC_IP4_STATIC_VER_MASK));

          p_dsc->outer_ip4->proto = \
                                    p_stream_packet_info->profile_union.profile_1.outer_ip.ip_union.ip4.proto;

          p_dsc->outer_ip4->src = \
                                  p_stream_packet_info->profile_union.profile_1.outer_ip.ip_union.ip4.src;

          p_dsc->outer_ip4->dst = \
                                  p_stream_packet_info->profile_union.profile_1.outer_ip.ip_union.ip4.dst;
        }
        else if(p_stream_packet_info->profile_union.profile_1.outer_ip.ip_version == ROHC_VERSION_IP6)
        {
          p_dsc->outer_ip6->ip6_ctlun.ip6_un1.un1_flow = \
                                                         (ROHC_LSHIFT(ROHC_VERSION_IP6, 28) | \
                                                          (p_stream_packet_info->profile_union.profile_1.outer_ip.ip_union.ip6.v_flowlabel_nh \
                                                           & ROHC_IP6_FLOWLABEL_MASK));

          ROHC_MEMCPY((rohc_void_t *)p_dsc->outer_ip6->src, \
              (rohc_void_t *) \
              p_stream_packet_info->profile_union.profile_1.outer_ip.ip_union.ip6.src, 16);
          ROHC_MEMCPY((rohc_void_t *)p_dsc->outer_ip6->dst, 
              (rohc_void_t *) \
              p_stream_packet_info->profile_union.profile_1.outer_ip.ip_union.ip6.dst,16);
        }
      }

      /* UDP info  */
      p_dsc->udp->sport = p_stream_packet_info->profile_union.profile_1.udp.src;

      p_dsc->udp->dport = p_stream_packet_info->profile_union.profile_1.udp.dst;

      /* RTP info  */
      p_dsc->rtp->ssrc = p_stream_packet_info->profile_union.profile_1.rtp.ssrc;

      break;
    case ROHC_IP_UDP:

      /* INNER IP info  */
      if(p_stream_packet_info->profile_union.profile_2.inner_ip.ip_version == ROHC_VERSION_IP4)
      {
        p_dsc->inner_ip4->vhl = \
                                ((ROHC_LSHIFT(ROHC_VERSION_IP4, 4) & ROHC_IP4_STATIC_VER_MASK));

        p_dsc->inner_ip4->proto = \
                                  p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip4.proto;

        p_dsc->inner_ip4->src = \
                                p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip4.src;

        p_dsc->inner_ip4->dst = \
                                p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip4.dst;
      }
      else if(p_stream_packet_info->profile_union.profile_2.inner_ip.ip_version == ROHC_VERSION_IP6)
      {
        p_dsc->inner_ip6->ip6_ctlun.ip6_un1.un1_flow = \
                                                       (ROHC_LSHIFT(ROHC_VERSION_IP6, 28) | \
                                                        (p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip6.v_flowlabel_nh \
                                                         & ROHC_IP6_FLOWLABEL_MASK));

        ROHC_MEMCPY((rohc_void_t *)p_dsc->inner_ip6->src, \
            (rohc_void_t *) \
            p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip6.src, 16);
        ROHC_MEMCPY((rohc_void_t *)p_dsc->inner_ip6->dst, 
            (rohc_void_t *) \
            p_stream_packet_info->profile_union.profile_2.inner_ip.ip_union.ip6.dst,16);
      }

      if( ROHC_TRUE == p_stream_packet_info->profile_union.profile_2.is_outer_present)
      {
        /* OUTER IP info present */
        if(p_stream_packet_info->profile_union.profile_2.outer_ip.ip_version == ROHC_VERSION_IP4)
        {
          p_dsc->outer_ip4->vhl = \
                                  ((ROHC_LSHIFT(ROHC_VERSION_IP4, 4) & ROHC_IP4_STATIC_VER_MASK));

          p_dsc->outer_ip4->proto = \
                                    p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip4.proto;

          p_dsc->outer_ip4->src = \
                                  p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip4.src;

          p_dsc->outer_ip4->dst = \
                                  p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip4.dst;
        }
        else if(p_stream_packet_info->profile_union.profile_2.outer_ip.ip_version == ROHC_VERSION_IP6)
        {
          p_dsc->outer_ip6->ip6_ctlun.ip6_un1.un1_flow = \
                                                         (ROHC_LSHIFT(ROHC_VERSION_IP6, 28) | \
                                                          (p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip6.v_flowlabel_nh \
                                                           & ROHC_IP6_FLOWLABEL_MASK));

          ROHC_MEMCPY((rohc_void_t *)p_dsc->outer_ip6->src, \
              (rohc_void_t *) \
              p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip6.src, 16);
          ROHC_MEMCPY((rohc_void_t *)p_dsc->outer_ip6->dst, 
              (rohc_void_t *) \
              p_stream_packet_info->profile_union.profile_2.outer_ip.ip_union.ip6.dst,16);
        }
      }

      /* UDP info  */
      p_dsc->udp->sport = p_stream_packet_info->profile_union.profile_2.udp.src;

      p_dsc->udp->dport = p_stream_packet_info->profile_union.profile_2.udp.dst;

      break;

    case ROHC_IP_ESP:

      /* INNER IP info  */
      if(p_stream_packet_info->profile_union.profile_3.inner_ip.ip_version == ROHC_VERSION_IP4)
      {
        p_dsc->inner_ip4->vhl = \
                                ((ROHC_LSHIFT(ROHC_VERSION_IP4, 4) & ROHC_IP4_STATIC_VER_MASK));

        p_dsc->inner_ip4->proto = \
                                  p_stream_packet_info->profile_union.profile_3.inner_ip.ip_union.ip4.proto;

        p_dsc->inner_ip4->src = \
                                p_stream_packet_info->profile_union.profile_3.inner_ip.ip_union.ip4.src;

        p_dsc->inner_ip4->dst = \
                                p_stream_packet_info->profile_union.profile_3.inner_ip.ip_union.ip4.dst;
      }
      else if(p_stream_packet_info->profile_union.profile_3.inner_ip.ip_version == ROHC_VERSION_IP6)
      {
        p_dsc->inner_ip6->ip6_ctlun.ip6_un1.un1_flow = \
                                                       (ROHC_LSHIFT(ROHC_VERSION_IP6, 28) | \
                                                        (p_stream_packet_info->profile_union.profile_3.inner_ip.ip_union.ip6.v_flowlabel_nh \
                                                         & ROHC_IP6_FLOWLABEL_MASK));

        ROHC_MEMCPY((rohc_void_t *)p_dsc->inner_ip6->src, \
            (rohc_void_t *) \
            p_stream_packet_info->profile_union.profile_3.inner_ip.ip_union.ip6.src, 16);
        ROHC_MEMCPY((rohc_void_t *)p_dsc->inner_ip6->dst, 
            (rohc_void_t *) \
            p_stream_packet_info->profile_union.profile_3.inner_ip.ip_union.ip6.dst,16);
      }

      if( ROHC_TRUE == p_stream_packet_info->profile_union.profile_3.is_outer_present)
      {
        /* OUTER IP info present */
        if(p_stream_packet_info->profile_union.profile_3.outer_ip.ip_version == ROHC_VERSION_IP4)
        {
          p_dsc->outer_ip4->vhl = \
                                  ((ROHC_LSHIFT(ROHC_VERSION_IP4, 4) & ROHC_IP4_STATIC_VER_MASK));

          p_dsc->outer_ip4->proto = \
                                    p_stream_packet_info->profile_union.profile_3.outer_ip.ip_union.ip4.proto;

          p_dsc->outer_ip4->src = \
                                  p_stream_packet_info->profile_union.profile_3.outer_ip.ip_union.ip4.src;

          p_dsc->outer_ip4->dst = \
                                  p_stream_packet_info->profile_union.profile_3.outer_ip.ip_union.ip4.dst;
        }
        else if(p_stream_packet_info->profile_union.profile_3.outer_ip.ip_version == ROHC_VERSION_IP6)
        {
          p_dsc->outer_ip6->ip6_ctlun.ip6_un1.un1_flow = \
                                                         (ROHC_LSHIFT(ROHC_VERSION_IP6, 28) | \
                                                          (p_stream_packet_info->profile_union.profile_3.outer_ip.ip_union.ip6.v_flowlabel_nh \
                                                           & ROHC_IP6_FLOWLABEL_MASK));

          ROHC_MEMCPY((rohc_void_t *)p_dsc->outer_ip6->src, \
              (rohc_void_t *) \
              p_stream_packet_info->profile_union.profile_3.outer_ip.ip_union.ip6.src, 16);
          ROHC_MEMCPY((rohc_void_t *)p_dsc->outer_ip6->dst, 
              (rohc_void_t *) \
              p_stream_packet_info->profile_union.profile_3.outer_ip.ip_union.ip6.dst,16);
        }
      }

      /* SPI info  */
      p_dsc->esp->spi = p_stream_packet_info->profile_union.profile_3.esp.spi ;

      break;
  } /* end of switch */
} /* End of function fill_static_dsc_info */
#endif
