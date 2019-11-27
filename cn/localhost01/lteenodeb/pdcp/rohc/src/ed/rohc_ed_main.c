/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_ed_main.c,v 1.3.28.8 2010/11/09 05:11:58 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contain functions for encoding decoding.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_ed_main.c,v $
 * Revision 1.3.28.8  2010/11/09 05:11:58  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.3.28.7  2010/10/13 03:46:17  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.3.28.6  2010/10/01 14:10:57  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.3.28.5  2010/09/23 05:10:03  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.3.28.4  2010/09/10 10:34:45  gur22059
 * Locality of reference & use of register variables for better performamce
 *
 * Revision 1.3.28.3  2010/08/19 09:03:04  gur22059
 * SPR 577: corrected the calculation for IP-ID
 *
 * Revision 1.3.28.2  2010/06/30 05:39:00  gur21145
 * profile2_enhancements
 *
 * Revision 1.3.28.1  2010/06/25 09:12:08  gur21145
 * PROFILE@_ENHANCEMENT
 *
 * Revision 1.3  2009/05/27 15:51:52  gur19836
 * File Header and Function Headers updated
 *
 *
 *****************************************************************************/



/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "rohc_ed_def.h"
#include "rohc_ed_ext_if.h"
#include "rohc_ed_proto.h"
#include "rohc_ccom_mcr.h"
#include "rohc_dcom_mcr.h"
#include "rohc_trace_mcr.h"
#include "rohc_gcom_mcr.h"
/* + SPR 17439 */
#include "rohc_led_proto.h"
/* - SPR 17439 */
/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
 /* +- SPR 17777 */
 extern tickType_t pdcpCurrentTime_g;
 /* +- SPR 17777 */
#ifdef ROHC_UT_FLAG
    FILE *fp;
#endif

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/




/*****************************************************************************
 * Function Name  : rohc_encode_p1_sn
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to cpmressor context
 *                  sn - SN to be encoded
 *                  *p_en_val - structure to be filled, contains the sn_value
 *                          to be compressed, and it's no. of LSBs to be sent.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function encodes the sequence number received in the
 *                  packet using W-LSB encoding.  To find the value of "K" i.e..
 *                  minimum LSB of SN to be sent, it invokes the function
 *                  rohc_sn_g, which inturn calls function rohc_is_within_in_intval
 *                  and rohc_compute_k.
 *****************************************************************************/
rohc_return_t rohc_encode_p1_sn        ROHC_PARAMS
    (
        (p_rohc_entity, p_csc, sn, p_en_val, p_ecode),
        rohc_entity_t           *p_rohc_entity  _AND_
        rohc_csc_rtp_t          *p_csc          _AND_
        rohc_U32bit_t           sn              _AND_
        rohc_encoded_value_t    *p_en_val       _AND_
        rohc_error_code_t       *p_ecode
    )
{
    rohc_U32bit_t       v_ref_min, v_ref_max;
    rohc_U8bit_t        k1 = 0, k2 = 0, k = 0;

    ROHC_ENTER_FN("rohc_encode_p1_sn");
    
    /* The minimum and maximum valu of SN in GSW from start_index to end_index */
    v_ref_min = p_csc->rohc_gsw.min_sn;
    v_ref_max = p_csc->rohc_gsw.max_sn;

#ifdef ROHC_UT_FLAG
    fp = fopen("outfile", "a");
    fprintf(fp, "====================== ENCODED SN ========================\n");
    fprintf(fp, "V_REF_MIN:[%d] \nV_REF_MAX:[%d]"
                " \nSN:[%d] \n", v_ref_min, v_ref_max, sn);
#endif

    rohc_U8bit_t profile;
    profile = p_csc->profile;
        
    /* k = Minimum LSB's to be sent */
    k1 = rohc_sn_g(v_ref_min, sn, profile);
    k2 = rohc_sn_g(v_ref_max, sn, profile);
    /* As per RFC 3095 , Section 4.5.2, Buller 2, 
    the compressor chooses k = max(g(v_min, v), g(v_max, v)) */
    k = ((k1 < k2) ? k2 : k1);
    
    
    /* Update OUTPUT parameter p_en_val */
    p_en_val->value  = sn;
    p_en_val->bits  = k;
    
#ifdef ROHC_UT_FLAG
    fprintf(fp, "MIN_K:[%d] \nMAX_K:[%d] \nK:[%d] \n", k1, k2, k);
    fprintf(fp, "P_EN_VAL->VALUE:[%d] \nP_EN_VAL->BITS:[%d] \n", 
                                p_en_val->value, p_en_val->bits);
    fprintf(fp, "\n");
    fClose(fp);
#endif

    /* Update window */
    rohc_ed_p1_update_gsw_sn(p_rohc_entity,p_csc,sn);
    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"ED:EID[%u:%u]:CID[%u]:"\
                      "GSW_START_ID[%u]:"\
                      "GSW_CUR_ID[%u]:ENCD_SN[%u]:BITS[%u]" \
                      "SN ENCODING CALLED\n", \
                      p_rohc_entity->rohc_entity_id.ueIndex,
                      p_rohc_entity->rohc_entity_id.lcId, 
                      ROHC_GET_CSC_CID(p_csc),
                      p_csc->rohc_gsw.start_index,
                      ROHC_ED_GET_CUR_INDEX(p_csc), sn, k );
    ROHC_EXIT_FN("rohc_encode_p1_sn");

    return ROHC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : rohc_encode_p2_sn
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to cpmressor context
 *                  sn - SN to be encoded
 *                  *p_en_val - structure to be filled, contains the sn_value
 *                          to be compressed, and it's no. of LSBs to be sent.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function encodes the sequence number received in the
 *                  packet using W-LSB encoding.  To find the value of "K" i.e..
 *                  minimum LSB of SN to be sent, it invokes the function
 *                  rohc_sn_g, which inturn calls function rohc_is_within_in_intval
 *                  and rohc_compute_k.
 *****************************************************************************/
rohc_return_t rohc_encode_p2_sn        ROHC_PARAMS
    (
        (p_rohc_entity, p_csc, sn, p_en_val, p_ecode),
        rohc_entity_t           *p_rohc_entity  _AND_
        rohc_csc_udp_t          *p_csc          _AND_
        rohc_U32bit_t           sn              _AND_
        rohc_encoded_value_t    *p_en_val       _AND_
        rohc_error_code_t       *p_ecode
    )
{
    rohc_U32bit_t       v_ref_min, v_ref_max;
    rohc_U8bit_t        k1 = 0, k2 = 0, k = 0;

    ROHC_ENTER_FN("rohc_encode_p2_sn");
    
    /* The minimum and maximum valu of SN in GSW from start_index to end_index */
    v_ref_min = p_csc->rohc_gsw.min_sn;
    v_ref_max = p_csc->rohc_gsw.max_sn;

#ifdef ROHC_UT_FLAG
    fp = fopen("outfile", "a");
    fprintf(fp, "====================== ENCODED SN ========================\n");
    fprintf(fp, "V_REF_MIN:[%d] \nV_REF_MAX:[%d] \nSN:[%d] \n", v_ref_min, 
                                                                v_ref_max, sn);
#endif

    rohc_U8bit_t profile;
    profile = p_csc->profile;
        
    /* k = Minimum LSB's to be sent */
    k1 = rohc_sn_g(v_ref_min, sn, profile);
    k2 = rohc_sn_g(v_ref_max, sn, profile);
    /* As per RFC 3095 , Section 4.5.2, Buller 2, 
    the compressor chooses k = max(g(v_min, v), g(v_max, v)) */
    k = ((k1 < k2) ? k2 : k1);
    
    
    /* Update OUTPUT parameter p_en_val */
    p_en_val->value  = sn;
    p_en_val->bits  = k;
    
#ifdef ROHC_UT_FLAG
    fprintf(fp, "MIN_K:[%d] \nMAX_K:[%d] \nK:[%d] \n", k1, k2, k);
    fprintf(fp, "P_EN_VAL->VALUE:[%d] \nP_EN_VAL->BITS:[%d] \n", 
                                            p_en_val->value, p_en_val->bits);
    fprintf(fp, "\n");
    fClose(fp);
#endif

    /* Update window */
    rohc_ed_p2_update_gsw_sn(p_rohc_entity,p_csc,sn);
    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"ED:EID[%u:%u]:CID[%u]:"\
                                          "GSW_START_ID[%u]:"\
                                         "GSW_CUR_ID[%u]:ENCD_SN[%u]:BITS[%u]" \
                                         "SN ENCODING CALLED\n", \
                                          p_rohc_entity->rohc_entity_id.ueIndex,
                                          p_rohc_entity->rohc_entity_id.lcId, 
                                            ROHC_GET_CSC_CID(p_csc),
                                            p_csc->rohc_gsw.start_index,
                                            ROHC_ED_GET_CUR_INDEX(p_csc),
                                            sn, k );
    ROHC_EXIT_FN("rohc_encode_p2_sn");

    return ROHC_SUCCESS;
}
 
#ifdef ROHC_FUNC_ENHANCEMENT
/*****************************************************************************
 * Function Name  : rohc_encode_inner_ip_id
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to cpmressor context
 *                  ip_id - IP_ID to be encoded
 *                  *p_en_val - structure to be filled, contains the sn_value
 *                           to be compressed, and it's no. of LSBs to be sent.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function encodes the INNER IP ID received in the packet
 *                  using W-LSB encoding applied on the offset. To find the value
 *                  of "K" i.e.. minimum LSB of OFFSET ( IP_ID - SN) to be sent,
 *                  it invokes the function rohc_ip_id_g, which inturn calls
 *                  function rohc_is_within_in_intval and rohc_compute_k.
 *****************************************************************************/
rohc_return_t rohc_encode_inner_ip_id   ROHC_PARAMS
    (
        (p_rohc_entity, p_csc, ip_id, p_en_val, p_ecode),
        rohc_entity_t           *p_rohc_entity  _AND_
        rohc_csc_t              *p_csc          _AND_
        rohc_U16bit_t           ip_id           _AND_
        rohc_encoded_value_t    *p_en_val       _AND_
        rohc_error_code_t       *p_ecode
    )
{
    rohc_U16bit_t       v_ref_offset_min, v_ref_offset_max;
    rohc_U16bit_t       offset_i ;
    rohc_U8bit_t        k1 = 0, k2 = 0, k = 0;

    ROHC_ENTER_FN("rohc_encode_inner_ip_id");

    if(ROHC_TRUE == p_csc->new_inner_rnd)
    {
        /* Encoding for IP-ID is always called ofr incremantal IP-ID.
        * NO encoding to be called for random IP-ID. 
        * Refer Section 5.7, RFC 3095 for the same */
        *p_ecode = ROHC_ENCODING_CALLED_FOR_RANDOM_IP_ID;

        ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]" \
                                                "ERROR : IP-ID ENCODING CALLED for RANDOM IP-ID\n", \
                                                p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId, \
                                                ROHC_GET_CSC_CID(p_csc));

        ROHC_EXIT_FN("rohc_encode_inner_ip_id");
        return ROHC_FAILURE;
    }

    /* Find value of IP ID OFFSET in current received Packet */ 
    offset_i =  (rohc_U16bit_t)((rohc_U16bit_t)ip_id - (rohc_U16bit_t)\
                p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].sn);

    /* finding the minimum nad maximum values of offset_i in the window */
    v_ref_offset_min = p_csc->rohc_gsw.min_inner_offset_i;
    v_ref_offset_max = p_csc->rohc_gsw.max_inner_offset_i;
    
#ifdef ROHC_UT_FLAG
    fp = fopen("outfile", "a");
    fprintf(fp, "====================== ENCODED INNER IP_ID ========================\n");
    fprintf(fp, "IP-ID : [%d]\n", ip_id);
    fprintf(fp, "V_REF_MIN:[%d] \nV_REF_MAX:[%d] \nOFFSET_I:[%d] \n", \
            v_ref_offset_min, v_ref_offset_max, offset_i);
#endif

    if(offset_i < v_ref_offset_min)
        v_ref_offset_min =offset_i;
    else if(offset_i > v_ref_offset_max)
        v_ref_offset_max =offset_i;
        
    /* Compute Minimum Number of LSBs to be sent to convey IP ID OFFSET */
    k1 = rohc_ip_id_g(v_ref_offset_min, offset_i);
    k2 = rohc_ip_id_g(v_ref_offset_max, offset_i);
    
    /* As per RFC 3095 , Section 4.5.2, Buller 2, 
    the compressor chooses k = max(g(v_min, v), g(v_max, v)) */
    k = ((k1 < k2) ? k2 : k1);

    /* Update OUTPUT parameter p_en_val */
    p_en_val->value = offset_i;
    p_en_val->bits = k;
    
#ifdef ROHC_UT_FLAG
    fprintf(fp, "MIN_K:[%d] \nMAX_K:[%d] \nK:[%d] \n", k1, k2, k);
    fprintf(fp, "P_EN_VAL->VALUE:[%d] \nP_EN_VAL->BITS:[%d] \n", p_en_val->value, p_en_val->bits);
    fprintf(fp, "\n");
    fClose(fp);
#endif
    
    /* Update window */
    /* ipid*/
    p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].inner_ip_id = ip_id;
     p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].inner_offset_i
                        = offset_i;
    /*SPR #1393 Start*/
    /* ipid offset*/
    rohc_ed_update_gsw_inner_ip_id(p_rohc_entity,p_csc,ip_id);
/*SPR #1393 End*/

    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO, 
                     "ED:EID[%u:%u]:CID[%u]:GSW_START_ID[%u]:GSW_CUR_ID[%u]:"\
                     "ENCD_IP_ID[%u]:ENCD_OFFSET[%u]:BITS[%u]" \
                     "IP-ID ENCODING CALLED\n", \
                     p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId, 
                     ROHC_GET_CSC_CID(p_csc),
                     p_csc->rohc_gsw.start_index,
                     ROHC_ED_GET_CUR_INDEX(p_csc),
                     ip_id, offset_i, k );

    ROHC_EXIT_FN("rohc_encode_inner_ip_id");

    return ROHC_SUCCESS;
}

#endif
#ifdef ROHC_FUNC_ENHANCEMENT
/*****************************************************************************
 * Function Name  : rohc_encode_outer_ip_id
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to cpmressor context
 *                  ip_id - IP_ID to be encoded
 *                  *p_en_val - structure to be filled, contains the sn_value
 *                         to be compressed, and it's no. of LSBs to be sent.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function encodes the OUTER IP ID received in the packet 
 *                  using W-LSB encoding applied on the offset. To find the value
 *                  of "K" i.e.. minimum LSB of OFFSET(IP-ID - SN) to be sent,
 *                  it invokes the function rohc_ip_id_g, which inturn calls
 *                  function rohc_is_within_in_intval and rohc_compute_k.
 *****************************************************************************/
rohc_return_t rohc_encode_outer_ip_id   ROHC_PARAMS
    (
        (p_rohc_entity, p_csc, ip_id, p_en_val, p_ecode),
        rohc_entity_t           *p_rohc_entity  _AND_
        rohc_csc_t              *p_csc          _AND_
        rohc_U16bit_t           ip_id           _AND_
        rohc_encoded_value_t    *p_en_val       _AND_
        rohc_error_code_t       *p_ecode
    )
{
    rohc_U32bit_t       v_ref_offset_min, v_ref_offset_max ;
    rohc_U32bit_t       offset_i ;
    rohc_U8bit_t        k1 = 0, k2 = 0, k = 0;

    ROHC_ENTER_FN("rohc_encode_outer_ip_id");

    if(ROHC_TRUE == p_csc->new_outer_rnd)
    {
        /* Encoding for IP-ID is always called ofr incremantal IP-ID.
        * NO encoding to be called for random IP-ID. 
        * Refer Section 5.7, RFC 3095 for the same */
        *p_ecode = ROHC_ENCODING_CALLED_FOR_RANDOM_IP_ID;

        ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]" \
                                                "ERROR : IP-ID ENCODING CALLED for RANDOM IP-ID\n", \
                                                p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId, \
                                                ROHC_GET_CSC_CID(p_csc));

        ROHC_EXIT_FN("rohc_encode_outer_ip_id");
        return ROHC_FAILURE;
    }

    /* Find value of IP ID OFFSET in current received Packet */ 
    offset_i =  ROHC_ABS(ip_id - \
                p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].sn);

    /* finding the minimum nad maximum values of offset_i in the window */
    v_ref_offset_min = p_csc->rohc_gsw.min_outer_offset_i;
    v_ref_offset_max = p_csc->rohc_gsw.max_outer_offset_i;
    
#ifdef ROHC_UT_FLAG
    fp = fopen("outfile", "a");
    fprintf(fp, "====================== ENCODED OUTER IP_ID ========================\n");
    fprintf(fp, "IP-ID : [%d]\n", ip_id);
    fprintf(fp, "V_REF_MIN:[%d] \nV_REF_MAX:[%d] \nOFFSET_I:[%d] \n", \
            v_ref_offset_min, v_ref_offset_max, offset_i);
#endif

    if(offset_i < v_ref_offset_min)
    {
        /* Refer Section 4.5.5, RFC 3095, which says
         * IP-ID is assigned by 2-byte counter, thus always increased by 1.
         * RTP SN for such IPV4 stacks, will also increase by 1, and IP-ID
         * will increase by at least the same amount. Hence offset remains
         * constant or increases */
         
        *p_ecode = ROHC_OUT_OF_SEQ_IP_ID_RECD;

        ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]" \
                                                "ERROR : IP-ID ENCODING CALLED for INVALID IP-ID", \
                                                p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId, \
                                                ROHC_GET_CSC_CID(p_csc));


        ROHC_EXIT_FN("rohc_encode_outer_ip_id");
        return ROHC_FAILURE;
    }


    /* Compute Minimum Number of LSBs to be sent to convey IP ID OFFSET */
    k1 = rohc_ip_id_g(v_ref_offset_min, offset_i);
    k2 = rohc_ip_id_g(v_ref_offset_max, offset_i);

    /* As per RFC 3095 , Section 4.5.2, Buller 2, 
    the compressor chooses k = max(g(v_min, v), g(v_max, v)) */
    k = ((k1 < k2) ? k2 : k1);

    /* Update OUTPUT parameter p_en_val */
    p_en_val->value = offset_i;
    p_en_val->bits = k;
    
#ifdef ROHC_UT_FLAG
    fprintf(fp, "MIN_K:[%d] \nMAX_K:[%d] \nK:[%d] \n", k1, k2, k);
    fprintf(fp, "P_EN_VAL->VALUE:[%d] \nP_EN_VAL->BITS:[%d] \n", p_en_val->value, p_en_val->bits);
    fprintf(fp, "\n");
    fClose(fp);
#endif

    /* Update window */
    /* ipid*/
    /* ipid offset*/
    rohc_ed_update_gsw_outer_ip_id(p_rohc_entity, p_csc, ip_id);
    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO, 
                     "ED:EID[%u:%u]:CID[%u]:GSW_START_ID[%u]:GSW_CUR_ID[%u]:"\
                     "ENCD_IP_ID[%u]:ENCD_OFFSET[%u]:BITS[%u]" \
                     "IP-ID ENCODING CALLED\n", 
                     p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId, 
                     ROHC_GET_CSC_CID(p_csc),
                     p_csc->rohc_gsw.start_index, 
                     ROHC_ED_GET_CUR_INDEX(p_csc),
                     ip_id, offset_i, k);

    ROHC_EXIT_FN("rohc_encode_outer_ip_id");

    return ROHC_SUCCESS;
}
#endif

/*****************************************************************************
 * Function Name  : rohc_encode_p1_ts
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to cpmressor context
 *                  ts - TS to be encoded
 *                  *p_en_val - structure to be filled, contains the sn_value
 *                          to be compressed, and it's no. of LSBs to be sent.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This is the main routine for RTP Timestamp encoding.
 *                  It is responsible for the deciding which encoding routine to
 *                  call and based on that call the appropriate encoding routine
 *       LOGIC :
 *          If CSC contains the jitter and clock resolution of decompressor, then
 *          Call the rohc_encode_timer_based_ts routine to
 *          compress the RTP Timestamp value.
 *       Else 
 *          Call the rohc_encode_scaled_ts routine to compress
 *          the RTP Timestamp value. The field p_en_val contains 
 *          the encoded RTP Timestamp and minimum no. of LSB's
 *          of the encoded value.
 *
 *****************************************************************************/
rohc_return_t   rohc_encode_p1_ts  ROHC_PARAMS
    (
        (p_rohc_entity, p_csc, ts, p_en_val, p_ecode),
        rohc_entity_t           *p_rohc_entity  _AND_
        rohc_csc_rtp_t          *p_csc          _AND_
        rohc_U32bit_t           ts              _AND_
        rohc_encoded_value_t    *p_en_val       _AND_
        rohc_error_code_t       *p_ecode
    )
{
    ROHC_ENTER_FN("rohc_encode_p1_ts");
    
    if((p_csc->jit_clk_res_flag==(ROHC_JIT_PRESENT | ROHC_CLK_RES_PRESENT)))
    {
        ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:" \
                          "TIMER BASED TS ENCODING CALLED", \
                           p_rohc_entity->rohc_entity_id.ueIndex,
                           p_rohc_entity->rohc_entity_id.lcId, \
                           ROHC_GET_CSC_CID(p_csc));
                                  
        rohc_encode_p1_timer_based_ts(p_rohc_entity, p_csc, ts, p_en_val);
    }
    else
    {
        ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:" \
                         "SCALED TS ENCODING CALLED", \
                          p_rohc_entity->rohc_entity_id.ueIndex,
                          p_rohc_entity->rohc_entity_id.lcId, \
                          ROHC_GET_CSC_CID(p_csc));
        rohc_encode_p1_scaled_ts(p_rohc_entity, p_csc, ts, p_en_val);
    }

    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_encode_p1_ts");

    return ROHC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : rohc_ed_p1_update_window
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to cpmressor context
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function is called  to update the GSW. It incremets 
 *                  the end_index of GSW.
 *      LOGIC :
 *          Increment the end index to point to next received packet.
 *          If end_index id greater than last index, then point to 
 *          zero(th) index. This was we make GSW as circular queue.
 *****************************************************************************/
rohc_return_t   rohc_ed_p1_update_window       ROHC_PARAMS
    (
        (p_rohc_entity, p_csc),
        rohc_entity_t     *p_rohc_entity    _AND_
        rohc_csc_rtp_t    *p_csc
    )
{
    ROHC_ENTER_FN("rohc_ed_p1_update_window");
    p_csc->rohc_gsw.end_index = (p_csc->rohc_gsw.end_index+1)
                                            % ROHC_MAX_LENGTH_GSW;

    if(p_csc->rohc_gsw.iteration_flag != 0 && (p_csc->rohc_gsw.end_index == 
            p_csc->rohc_gsw.start_index) )
    {
        p_csc->rohc_gsw.start_index = (p_csc->rohc_gsw.start_index+1)
                                            % ROHC_MAX_LENGTH_GSW;
    } 
    p_csc->rohc_gsw.iteration_flag =1;  
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */

    ROHC_EXIT_FN("rohc_ed_p1_update_window");

    return ROHC_SUCCESS ;
}
/*****************************************************************************
 * Function Name  : rohc_ed_p2_update_window
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to cpmressor context
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function is called  to update the GSW. It incremets 
 *                  the end_index of GSW.
 *      LOGIC :
 *          Increment the end index to point to next received packet.
 *          If end_index id greater than last index, then point to 
 *          zero(th) index. This was we make GSW as circular queue.
 *****************************************************************************/
rohc_return_t   rohc_ed_p2_update_window       ROHC_PARAMS
    (
        (p_rohc_entity, p_csc),
        rohc_entity_t *p_rohc_entity    _AND_
        rohc_csc_udp_t    *p_csc
    )
{
    ROHC_ENTER_FN("rohc_ed_p2_update_window");
    p_csc->rohc_gsw.end_index = (p_csc->rohc_gsw.end_index+1)
                                            % ROHC_MAX_LENGTH_GSW;

    if(p_csc->rohc_gsw.iteration_flag != 0 && (p_csc->rohc_gsw.end_index == 
            p_csc->rohc_gsw.start_index) )
    {
        p_csc->rohc_gsw.start_index = (p_csc->rohc_gsw.start_index+1)
                                            % ROHC_MAX_LENGTH_GSW;
    } 
    p_csc->rohc_gsw.iteration_flag =1;  

    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_ed_p2_update_window");

    return ROHC_SUCCESS ;
}

/*****************************************************************************
 * Function Name  : rohc_ed_p1_update_gsw_sn
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to compressor context
 *                  sn - The sequence  number to be updated in GSW.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function is called  to update the SN value in GSW.
 *                  It is called when IR-DYNs are sent, thus GSW is to be updated,
 *                  but no encoding to be done.
 *****************************************************************************/
rohc_return_t   rohc_ed_p1_update_gsw_sn       ROHC_PARAMS
    (
        (p_rohc_entity, p_csc, sn),
        rohc_entity_t   *p_rohc_entity  _AND_
        rohc_csc_rtp_t  *p_csc          _AND_
        rohc_U32bit_t   sn              
    )
{
    rohc_U32bit_t loop_var=0;
    ROHC_ENTER_FN("rohc_ed_p1_update_gsw_sn");

    /* update the min. and max. value of SN. It is the min. and max. \
     * value from start_index to end_index */
    p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].sn = sn;
   
    loop_var = p_csc->rohc_gsw.start_index;
    if(loop_var == p_csc->rohc_gsw.end_index) /* only one element in the GSW */
    {
        p_csc->rohc_gsw.min_sn = p_csc->rohc_gsw.max_sn = sn;   
    }  

    if(p_csc->rohc_gsw.min_sn >  sn)
    {
      p_csc->rohc_gsw.min_sn = sn;
    }

    /* +- SPR 19801 */
        p_csc->rohc_gsw.max_sn = sn;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */

    ROHC_EXIT_FN("rohc_ed_p1_update_gsw_sn");
    return ROHC_SUCCESS ;
}
/*****************************************************************************
 * Function Name  : rohc_ed_p2_update_gsw_sn
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to compressor context
 *                  sn - The sequence  number to be updated in GSW.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function is called  to update the SN value in GSW.
 *                  It is called when IR-DYNs are sent, thus GSW is to be updated,
 *                  but no encoding to be done.
 *****************************************************************************/
rohc_return_t   rohc_ed_p2_update_gsw_sn       ROHC_PARAMS
    (
        (p_rohc_entity, p_csc, sn),
        rohc_entity_t   *p_rohc_entity  _AND_
        rohc_csc_udp_t  *p_csc          _AND_
        rohc_U32bit_t   sn              
    )
{
    rohc_U32bit_t loop_var=0;
    ROHC_ENTER_FN("rohc_ed_p2_update_gsw_sn");

    /* update the min. and max. value of SN. It is the min. and max. \
     * value from start_index to end_index */
    p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].sn = sn;
   
    loop_var = p_csc->rohc_gsw.start_index;
    if(loop_var == p_csc->rohc_gsw.end_index) /* only one element in the GSW */
    {
        p_csc->rohc_gsw.min_sn = p_csc->rohc_gsw.max_sn = sn;   
    }  

    if(p_csc->rohc_gsw.min_sn >  sn)
    {
      p_csc->rohc_gsw.min_sn = sn;
    }
    /* +- SPR 19801 */
    p_csc->rohc_gsw.max_sn = sn;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */

    ROHC_EXIT_FN("rohc_ed_p2_update_gsw_sn");
    return ROHC_SUCCESS ;
}

#ifdef ROHC_FUNC_ENHANCEMENT
/*****************************************************************************
 * Function Name  : rohc_ed_update_gsw_inner_ip_id
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to compressor context
 *                  inner_ip_id - The inner ip_id to be updated in GSW.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function is called  to update the IP_ID1 value in GSW.
 *                  It is called when IR-DYNs are sent, thus GSW is to be updated,
 *                  but no encoding to be done.
 *****************************************************************************/
rohc_return_t   rohc_ed_update_gsw_inner_ip_id      ROHC_PARAMS
    (
        (p_rohc_entity, p_csc, inner_ip_id),
        rohc_entity_t   *p_rohc_entity  _AND_
        rohc_csc_t      *p_csc          _AND_
        rohc_U16bit_t   inner_ip_id             
    )
{
    ROHC_ENTER_FN("rohc_ed_update_gsw_inner_ip_id");

    /* Update inner_ip_id in gsw */
    p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].inner_ip_id = inner_ip_id;

    rohc_ed_update_gsw_inner_offset(p_rohc_entity, p_csc,(rohc_U16bit_t)
        (ROHC_ABS(inner_ip_id - 
                  p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].sn)));
    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_ed_update_gsw_inner_ip_id");

    return ROHC_SUCCESS ;
}

#endif
#ifdef ROHC_FUNC_ENHANCEMENT
/*****************************************************************************
 * Function Name  : rohc_ed_update_gsw_outer_ip_id
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to compressor context
 *                  outer_ip_id - The outer ip_id to be updated in GSW.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function is called  to update the IP_ID2 value in GSW.
 *                  It is called when IR-DYNs are sent, thus GSW is to be updated,
 *                  but no encoding to be done.
 *****************************************************************************/
rohc_return_t   rohc_ed_update_gsw_outer_ip_id      ROHC_PARAMS
    (
        (p_rohc_entity, p_csc, outer_ip_id),
        rohc_entity_t   *p_rohc_entity  _AND_
        rohc_csc_t      *p_csc          _AND_
        rohc_U16bit_t   outer_ip_id             
    )
{
    ROHC_ENTER_FN("rohc_ed_update_gsw_outer_ip_id");

    /* Update outer_ip_id in gsw */
    p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].outer_ip_id = 
                        outer_ip_id;
    rohc_ed_update_gsw_outer_offset(p_rohc_entity, p_csc,(rohc_U16bit_t)
        (ROHC_ABS(outer_ip_id - 
            p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].sn)));
    ROHC_EXIT_FN("rohc_ed_update_gsw_outer_ip_id");

    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
    return ROHC_SUCCESS ;
}

#endif
#ifdef ROHC_FUNC_ENHANCEMENT
/*****************************************************************************
 * Function Name  : rohc_ed_update_gsw_inner_offset
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to compressor context
 *                  inner_offset - The inner offset to be updated in GSW.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function is called  to update the OFFSET_I1 value in GSW.
 *                  It is called when IR-DYNs are sent, thus GSW is to be updated,
 *                  but no encoding to be done.
 *****************************************************************************/
rohc_return_t   rohc_ed_update_gsw_inner_offset         ROHC_PARAMS
    (
        (p_rohc_entity, p_csc, inner_offset),
        rohc_entity_t   *p_rohc_entity  _AND_
        rohc_csc_t      *p_csc          _AND_
        rohc_U16bit_t   inner_offset                
    )
{
    rohc_U16bit_t loop_var;
    ROHC_ENTER_FN("rohc_ed_update_gsw_inner_offset");

    
    /* update the min. and max. value of IP-ID. It is the min. and max. \
     * value from start_index to end_index */
     p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].inner_offset_i
                        = inner_offset;
    /* ipid offset*/
    loop_var = p_csc->rohc_gsw.start_index;
    if(loop_var == p_csc->rohc_gsw.end_index) /* only one element in the GSW */
    {
        p_csc->rohc_gsw.max_inner_offset_i =
                    p_csc->rohc_gsw.min_inner_offset_i = inner_offset;  
    }   
    else /* more than one elements exist*/
    {   
        p_csc->rohc_gsw.max_inner_offset_i=p_csc->rohc_gsw.min_inner_offset_i=
                        p_csc->rohc_gsw.rohc_gsw_elem[loop_var].inner_offset_i;
        do
        {
            loop_var =(loop_var +1)%ROHC_MAX_LENGTH_GSW;
            if( p_csc->rohc_gsw.rohc_gsw_elem[loop_var].inner_offset_i < 
                                p_csc->rohc_gsw.min_inner_offset_i)
                p_csc->rohc_gsw.min_inner_offset_i =
                        p_csc->rohc_gsw.rohc_gsw_elem[loop_var].sn;
            if( p_csc->rohc_gsw.rohc_gsw_elem[loop_var].inner_offset_i > 
                                p_csc->rohc_gsw.max_inner_offset_i)
                p_csc->rohc_gsw.max_inner_offset_i = 
                        p_csc->rohc_gsw.rohc_gsw_elem[loop_var].inner_offset_i;

        }
        while(loop_var != p_csc->rohc_gsw.end_index);
    }   


    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */

    ROHC_EXIT_FN("rohc_ed_update_gsw_inner_offset");

    return ROHC_SUCCESS ;
}

#endif
#ifdef ROHC_FUNC_ENHANCEMENT
/*****************************************************************************
 * Function Name  : rohc_ed_update_gsw_outer_offset
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to compressor context
 *                  outer_offset - The outer offset to be updated in GSW.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function is called  to update the OFFSET_I2 value in GSW.
 *                  It is called when IR-DYNs are sent, thus GSW is to be updated,
 *                  but no encoding to be done.
 *****************************************************************************/
rohc_return_t   rohc_ed_update_gsw_outer_offset         ROHC_PARAMS
    (
        (p_rohc_entity, p_csc, outer_offset),
        rohc_entity_t   *p_rohc_entity  _AND_
        rohc_csc_t      *p_csc          _AND_
        rohc_U16bit_t   outer_offset                
    )
{
    rohc_U16bit_t i;
    ROHC_ENTER_FN("rohc_ed_update_gsw_outer_offset");


    /* update the min. and max. value of IP-ID. It is the min. and max. \
     * value from start_index to end_index */
    p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].inner_offset_i
                        = outer_offset;
    /* ipid offset*/
    i = p_csc->rohc_gsw.start_index;
    if(i == p_csc->rohc_gsw.end_index) /* only one element in the GSW */
    {
        p_csc->rohc_gsw.max_outer_offset_i =
                    p_csc->rohc_gsw.min_outer_offset_i = outer_offset;  
    }   
    else /* more than one elements exist*/
    {   
        p_csc->rohc_gsw.max_outer_offset_i=p_csc->rohc_gsw.min_outer_offset_i=
                        p_csc->rohc_gsw.rohc_gsw_elem[i].outer_offset_i;
        do
        {
            i =(i +1)%ROHC_MAX_LENGTH_GSW;
            if( p_csc->rohc_gsw.rohc_gsw_elem[i].outer_offset_i < 
                                p_csc->rohc_gsw.min_outer_offset_i)
                p_csc->rohc_gsw.min_outer_offset_i = 
                        p_csc->rohc_gsw.rohc_gsw_elem[i].sn;
            if( p_csc->rohc_gsw.rohc_gsw_elem[i].outer_offset_i > 
                                p_csc->rohc_gsw.max_outer_offset_i)
                p_csc->rohc_gsw.max_outer_offset_i = 
                        p_csc->rohc_gsw.rohc_gsw_elem[i].sn;

        }
        while(i != p_csc->rohc_gsw.end_index);
    }   

    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
    
    ROHC_EXIT_FN("rohc_ed_update_gsw_outer_offset");

    return ROHC_SUCCESS ;
}
#endif

/*****************************************************************************
 * Function Name  : rohc_ed_p1_update_gsw_ts
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to compressor context
 *                  ts_scaled - The ts scaled to be updated in GSW.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function is called  to update the TS_SCALED value in GSW.
 *                  It is called when IR-DYNs are sent, thus GSW is to be updated,
 *                  but no encoding to be done.
 *****************************************************************************/
rohc_return_t   rohc_ed_p1_update_gsw_ts       ROHC_PARAMS
    (
        (p_rohc_entity, p_csc, ts),
        rohc_entity_t   *p_rohc_entity  _AND_
        rohc_csc_rtp_t  *p_csc          _AND_
        rohc_U32bit_t   ts
    )
{
    rohc_U32bit_t   i,ts_scaled;        
    ROHC_ENTER_FN("rohc_ed_p1_update_gsw_ts");
    if(!p_csc->ts_stride)
        return ROHC_SUCCESS;
    ts_scaled = (ts / p_csc->ts_stride);
    p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].ts_scaled
                        = ts_scaled;
    /* ipid offset*/
    i = p_csc->rohc_gsw.start_index;
    if(i == p_csc->rohc_gsw.end_index) /* only one element in the GSW */
    {
        p_csc->rohc_gsw.max_ts_scaled =
                    p_csc->rohc_gsw.min_ts_scaled = ts_scaled;  
    }   
    else /* more than one elements exist*/
    {   
        p_csc->rohc_gsw.max_outer_offset_i=p_csc->rohc_gsw.min_ts_scaled=
                        p_csc->rohc_gsw.rohc_gsw_elem[i].ts_scaled;
        do
        {
            i =(i +1)%ROHC_MAX_LENGTH_GSW;
            if( p_csc->rohc_gsw.rohc_gsw_elem[i].ts_scaled < 
                                p_csc->rohc_gsw.min_ts_scaled)
                p_csc->rohc_gsw.min_ts_scaled = 
                        p_csc->rohc_gsw.rohc_gsw_elem[i].ts_scaled;
            if( p_csc->rohc_gsw.rohc_gsw_elem[i].ts_scaled > 
                                p_csc->rohc_gsw.max_ts_scaled)
                p_csc->rohc_gsw.max_ts_scaled = 
                        p_csc->rohc_gsw.rohc_gsw_elem[i].ts_scaled;

        }
        while(i != p_csc->rohc_gsw.end_index);
    }   
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_ed_p1_update_gsw_ts");

    return ROHC_SUCCESS ;
}

#ifdef ROHC_FUNC_ENHANCEMENT
/*****************************************************************************
 * Function Name  : rohc_ed_update_gsw_pkt_arvl_time
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to compressor context
 *                  pkt_arvl_time - The packet arrival time to be updated in GSW.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function is called  to update the PKT_ARVL_TIME value in GSW.
 *                  It is called when IR-DYNs are sent, thus GSW is to be updated,
 *                  but no encoding to be done.
 *****************************************************************************/
rohc_return_t   rohc_ed_update_gsw_pkt_arvl_time        ROHC_PARAMS
    (
        (p_rohc_entity, p_csc, pkt_arvl_time),
        rohc_entity_t   *p_rohc_entity  _AND_
        rohc_csc_t      *p_csc          _AND_
        rohc_U32bit_t   pkt_arvl_time               
    )
{
    ROHC_ENTER_FN("rohc_ed_update_gsw_pkt_arvl_time");
    /* Update pkt_arvl_time in gsw */
    p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].pkt_arvl_time 
        = pkt_arvl_time;
    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_ed_update_gsw_pkt_arvl_time");
    return ROHC_SUCCESS ;
}
#endif

/*****************************************************************************
 * Function Name  : rohc_ed_p1_update_gsw_pkt_typ
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to compressor context
 *                  pkt_typ - The packet type to be updated in GSW.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function is called  to update the pkt_typ value in GSW.
 *                  It is called when IR-DYNs are sent, thus GSW is to be updated,
 *                  but no encoding to be done.
 *****************************************************************************/
rohc_return_t   rohc_ed_p1_update_gsw_pkt_typ      ROHC_PARAMS
    (
        (p_rohc_entity, p_csc, pkt_typ),
        rohc_entity_t   *p_rohc_entity  _AND_
        rohc_csc_rtp_t  *p_csc          _AND_
        rohc_U32bit_t   pkt_typ             
    )
{
    ROHC_ENTER_FN("rohc_ed_p1_update_gsw_pkt_typ");

    /* Update sn in gsw */
    p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].pkt_typ =
                pkt_typ;

    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_ed_p1_update_gsw_pkt_typ");
    return ROHC_SUCCESS ;
}
/*****************************************************************************
 * Function Name  : rohc_ed_p2_update_gsw_pkt_typ
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to compressor context
 *                  pkt_typ - The packet type to be updated in GSW.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function is called  to update the pkt_typ value in GSW.
 *                  It is called when IR-DYNs are sent, thus GSW is to be updated,
 *                  but no encoding to be done.
 *****************************************************************************/
rohc_return_t   rohc_ed_p2_update_gsw_pkt_typ      ROHC_PARAMS
    (
        (p_rohc_entity, p_csc, pkt_typ),
        rohc_entity_t   *p_rohc_entity  _AND_
        rohc_csc_udp_t  *p_csc          _AND_
        rohc_U32bit_t   pkt_typ             
    )
{
    ROHC_ENTER_FN("rohc_ed_p2_update_gsw_pkt_typ");

    /* Update sn in gsw */
    p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].pkt_typ =
                pkt_typ;

    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_ed_p2_update_gsw_pkt_typ");
    return ROHC_SUCCESS ;
}

/*****************************************************************************
 * Function Name  : rohc_ack_handling_p1
 * Inputs         : p_csc - pointer to cpmressor context
 *                  sn - The sequence for which ACK comes.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function is called whenever a ACK is received at the
 *                  compressor side. In this function we will slide the start
 *                  index of GSW.
 *        LOGIC :
 *           Search for SN received in ACK between the start index and end index.
 *           Make the start index point to the index, where value of acked SN matches.
 *****************************************************************************/
rohc_return_t   rohc_ack_handling_p1       ROHC_PARAMS
    (
        (p_csc, sn,ack_typ),
        rohc_csc_rtp_t  *p_csc  _AND_
        rohc_U32bit_t   sn      _AND_
        rohc_U8bit_t    ack_typ
    )
{
    rohc_U32bit_t   loop_var = 0 ;
    rohc_U8bit_t    start_index = p_csc->rohc_gsw.start_index ;
    rohc_U8bit_t    end_index = p_csc->rohc_gsw.end_index ;
    rohc_U32bit_t ts_scaled =0, inner_offset_i=0, outer_offset_i=0;
    
    ROHC_ENTER_FN("rohc_ack_handling_p1");

    /*for (loop_var = start_index; loop_var < end_index; loop_var++)*/
    loop_var = (start_index + ROHC_MAX_LENGTH_GSW-1)%ROHC_MAX_LENGTH_GSW; 
    do  
    {
        loop_var=(loop_var+1)%ROHC_MAX_LENGTH_GSW;
        if(p_csc->rohc_gsw.rohc_gsw_elem[loop_var].sn == sn)
        {   
           if(ack_typ == ROHC_ACK)
           {    
            ts_scaled = p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ts_scaled;
            inner_offset_i=p_csc->rohc_gsw.rohc_gsw_elem[loop_var
                                                            ].inner_offset_i;
            outer_offset_i=p_csc->rohc_gsw.rohc_gsw_elem[loop_var
                                                            ].outer_offset_i;
            
            start_index = p_csc->rohc_gsw.start_index ;

            if(p_csc->c_mode == ROHC_MODE_R &&
               p_csc->rohc_gsw.rohc_gsw_elem[loop_var].pkt_typ == 
                                ROHC_R0CRCP1_PKTTYPE)
            {
                p_csc->r0_crc_ack_flag = ROHC_TRUE;
            }

            if(p_csc->profile == ROHC_IP_UDP_RTP && 
               (p_csc->rohc_gsw.rohc_gsw_elem[loop_var].pkt_typ >=
                                ROHC_UOR2P1_PKTTYPE) && 
               (p_csc->rohc_gsw.rohc_gsw_elem[loop_var].pkt_typ <=
                                ROHC_UOR2TSE3P1_PKTTYPE))
            { 
                p_csc->uor2_ack_flag = ROHC_TRUE;
            }
            
            p_csc->prtt = ((p_csc->rohc_gsw.end_index + ROHC_MAX_LENGTH_GSW) -
                            loop_var) % ROHC_MAX_LENGTH_GSW ;
            /* Here we need to traverse complete window and update min and
               the max value */
            
            p_csc->rohc_gsw.start_index=loop_var;
            loop_var = (start_index+ROHC_MAX_LENGTH_GSW-1)%ROHC_MAX_LENGTH_GSW; 
            do
            {
                loop_var =(loop_var +1)%ROHC_MAX_LENGTH_GSW;
                p_csc->rohc_gsw.min_sn = 
                (((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].sn)< sn)? 
                        (p_csc->rohc_gsw.rohc_gsw_elem[loop_var].sn) : sn);
                p_csc->rohc_gsw.max_sn = 
                (((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].sn) < sn)? 
                        sn : (p_csc->rohc_gsw.rohc_gsw_elem[loop_var].sn));
                
                p_csc->rohc_gsw.min_ts_scaled = 
                (((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ts_scaled) < 
                        ts_scaled)?
                        (p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ts_scaled) :
                        ts_scaled);

                p_csc->rohc_gsw.max_ts_scaled = 
                    (((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ts_scaled)
                      <ts_scaled)? 
                 ts_scaled:(p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ts_scaled));
                
                p_csc->rohc_gsw.min_inner_offset_i = 
                (((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].inner_offset_i) < 
                        inner_offset_i)?
                        (p_csc->rohc_gsw.rohc_gsw_elem[loop_var].inner_offset_i) :
                        inner_offset_i);

                p_csc->rohc_gsw.max_inner_offset_i =
                    (((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].inner_offset_i)
                      <inner_offset_i)? 
                        inner_offset_i:
                    (p_csc->rohc_gsw.rohc_gsw_elem[loop_var].inner_offset_i));
                p_csc->rohc_gsw.min_outer_offset_i = 
                (((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].outer_offset_i) < 
                        outer_offset_i)?
                        (p_csc->rohc_gsw.rohc_gsw_elem[loop_var].outer_offset_i) :
                        outer_offset_i);

                p_csc->rohc_gsw.max_outer_offset_i =
                    (((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].outer_offset_i)
                      <outer_offset_i)? 
                        outer_offset_i:
                    (p_csc->rohc_gsw.rohc_gsw_elem[loop_var].outer_offset_i));

        
            }while(loop_var != end_index);
           }        
           ROHC_EXIT_FN("rohc_ack_handling_p1");
           return ROHC_SUCCESS;
        }   
       /*  Scenario, when end_index is smaller than start_index */
       /* if (((p_csc->rohc_gsw.start_index) % ROHC_MAX_LENGTH_GSW) == 0)*/
       /*     p_csc->rohc_gsw.start_index = 0;*/

    }while(loop_var!=end_index);
        
    ROHC_EXIT_FN("rohc_ack_handling_p1");
    /* SN does exist in complete window */
    return ROHC_FAILURE ;
}
/*****************************************************************************
 * Function Name  : rohc_ack_handling_p2
 * Inputs         : p_csc - pointer to cpmressor context
 *                  sn - The sequence for which ACK comes.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function is called whenever a ACK is received at the
 *                  compressor side. In this function we will slide the start
 *                  index of GSW.
 *        LOGIC :
 *           Search for SN received in ACK between the start index and end index.
 *           Make the start index point to the index, where value of acked SN matches.
 *****************************************************************************/
rohc_return_t   rohc_ack_handling_p2       ROHC_PARAMS
    (
        (p_csc, sn,ack_typ),
        rohc_csc_udp_t      *p_csc  _AND_
        rohc_U32bit_t       sn      _AND_
        rohc_U8bit_t        ack_typ
    )
{
    rohc_U32bit_t   loop_var = 0 ;
    rohc_U8bit_t    start_index = p_csc->rohc_gsw.start_index ;
    rohc_U8bit_t    end_index = p_csc->rohc_gsw.end_index ;
    rohc_U32bit_t ts_scaled =0, inner_offset_i=0, outer_offset_i=0;
    
    ROHC_ENTER_FN("rohc_ack_handling_p2");

    /*for (loop_var = start_index; loop_var < end_index; loop_var++)*/
    loop_var = (start_index + ROHC_MAX_LENGTH_GSW-1)%ROHC_MAX_LENGTH_GSW; 
    do  
    {
        loop_var=(loop_var+1)%ROHC_MAX_LENGTH_GSW;
        if(p_csc->rohc_gsw.rohc_gsw_elem[loop_var].sn == sn)
        {   
           if(ack_typ == ROHC_ACK)
           {    
            ts_scaled = p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ts_scaled;
            inner_offset_i=p_csc->rohc_gsw.rohc_gsw_elem[loop_var
                                                            ].inner_offset_i;
            outer_offset_i=p_csc->rohc_gsw.rohc_gsw_elem[loop_var
                                                            ].outer_offset_i;
            
            start_index = p_csc->rohc_gsw.start_index ;

            if(p_csc->c_mode == ROHC_MODE_R &&
               p_csc->rohc_gsw.rohc_gsw_elem[loop_var].pkt_typ == 
                                ROHC_R0CRCP1_PKTTYPE)
            {
                p_csc->r0_crc_ack_flag = ROHC_TRUE;
            }
            if((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].pkt_typ >=
                                ROHC_UOR2P23_PKTTYPE) && 
                    (p_csc->rohc_gsw.rohc_gsw_elem[loop_var].pkt_typ <=
                                ROHC_UOR2E3P23_PKTTYPE))
            {   
                p_csc->uor2_ack_flag = ROHC_TRUE;
            }   
            
            p_csc->prtt = ((p_csc->rohc_gsw.end_index + ROHC_MAX_LENGTH_GSW) -
                            loop_var) % ROHC_MAX_LENGTH_GSW ;
            /* Here we need to traverse complete window and update min and
               the max value */
            
            p_csc->rohc_gsw.start_index=loop_var;
            loop_var = (start_index+ROHC_MAX_LENGTH_GSW-1)%ROHC_MAX_LENGTH_GSW; 
            do
            {
                loop_var =(loop_var +1)%ROHC_MAX_LENGTH_GSW;
                p_csc->rohc_gsw.min_sn = 
                (((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].sn)< sn)? 
                        (p_csc->rohc_gsw.rohc_gsw_elem[loop_var].sn) : sn);
                p_csc->rohc_gsw.max_sn = 
                (((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].sn) < sn)? 
                        sn : (p_csc->rohc_gsw.rohc_gsw_elem[loop_var].sn));
                
                p_csc->rohc_gsw.min_ts_scaled = 
                (((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ts_scaled) < 
                        ts_scaled)?
                        (p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ts_scaled) :
                        ts_scaled);

                p_csc->rohc_gsw.max_ts_scaled = 
                    (((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ts_scaled)
                      <ts_scaled)? 
                 ts_scaled:(p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ts_scaled));
                
                p_csc->rohc_gsw.min_inner_offset_i = 
                (((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].inner_offset_i) < 
                        inner_offset_i)?
                        (p_csc->rohc_gsw.rohc_gsw_elem[loop_var].inner_offset_i) :
                        inner_offset_i);

                p_csc->rohc_gsw.max_inner_offset_i =
                    (((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].inner_offset_i)
                      <inner_offset_i)? 
                        inner_offset_i:
                    (p_csc->rohc_gsw.rohc_gsw_elem[loop_var].inner_offset_i));
                p_csc->rohc_gsw.min_outer_offset_i = 
                (((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].outer_offset_i) < 
                        outer_offset_i)?
                        (p_csc->rohc_gsw.rohc_gsw_elem[loop_var].outer_offset_i) :
                        outer_offset_i);

                p_csc->rohc_gsw.max_outer_offset_i =
                    (((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].outer_offset_i)
                      <outer_offset_i)? 
                        outer_offset_i:
                    (p_csc->rohc_gsw.rohc_gsw_elem[loop_var].outer_offset_i));

        
            }while(loop_var != end_index);
           }        
           ROHC_EXIT_FN("rohc_ack_handling_p2");
           return ROHC_SUCCESS;
        }   
       /*  Scenario, when end_index is smaller than start_index */
       /* if (((p_csc->rohc_gsw.start_index) % ROHC_MAX_LENGTH_GSW) == 0)*/
       /*     p_csc->rohc_gsw.start_index = 0;*/

    }while(loop_var!=end_index);
        
    ROHC_EXIT_FN("rohc_ack_handling_p2");
    /* SN does exist in complete window */
    return ROHC_FAILURE ;
}

/*****************************************************************************
 * Function Name  : rohc_decode_p1_sn
 * Inputs         : p_rohc_entity- pointer to ROHC entity
 *                  p_dsc - pointer to decompressor context
 *                  *p_en_val - structure containing the compressed  sn_value
 *                       and no. of LSBs it is compressed in ( i.e.. K).
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function decodes the SN value using W-LSB decoding
 *                  mechanism. (This function will only work for 16 bit sn value,
 *                  not for for 32 bit sn (ESP sn).
 *****************************************************************************/
rohc_return_t rohc_decode_p1_sn    ROHC_PARAMS
    (
        (p_rohc_entity, p_dsc, p_en_val, p_dec_sn, p_ecode),
        rohc_entity_t           *p_rohc_entity  _AND_
        rohc_dsc_rtp_t          *p_dsc          _AND_
        rohc_encoded_value_t    *p_en_val       _AND_
        rohc_U32bit_t           *p_dec_sn       _AND_
        rohc_error_code_t       *p_ecode
    )
{
    rohc_U16bit_t   p, interval_size ;
    register rohc_U16bit_t   new_v;
    register rohc_U16bit_t  min_v;
    register rohc_U16bit_t max_v;
    rohc_U32bit_t sn_prev;
    rohc_U8bit_t bits;
    //rohc_S8bit_t    adjustment_factor_N;
    rohc_U8bit_t    adj_needed = ROHC_FALSE;

    ROHC_ENTER_FN("rohc_decode_p1_sn");

    /* To maintain the candidate set of last two received SN values */
    /*p_dsc->sn_prev_prev = p_dsc->sn_prev ;*/
            
    /* This macro replaces the p_en_val->bits of p_en_val->value
     * to p_dsc->sn_prev, and saves this value to new_v */
    sn_prev = p_dsc->sn_prev;
    bits = p_en_val->bits;
    ROHC_PUT_LSBS(new_v, sn_prev, p_en_val->value, bits);

#ifdef ROHC_UT_FLAG
    fp = fopen("outfile", "a");
    fprintf(fp, "====================== DECODED SN ========================\n");
    fprintf(fp, "V_REF:[%d]\nENCODED_VAL:[%d]\nK:[%d]\nNEW_V:[%d]\n",
                p_dsc->sn_prev, p_en_val->value, p_en_val->bits, new_v);
#endif
    
    /* k = p_en_val->bits */
    if(p_en_val->bits <= 4)
        p = 1;
    else
        p = ((ROHC_POWER_OF_2(p_en_val->bits - 5)) - 1);
    
    /* To find the min. and max. value of interpretation interval */
    min_v = (rohc_U16bit_t)((rohc_U16bit_t)sn_prev - p);
    max_v = (rohc_U16bit_t)((rohc_U16bit_t)sn_prev +
                            (ROHC_POWER_OF_2(bits) - 1) - p);
    
#ifdef ROHC_UT_FLAG
    fprintf(fp, "MIN_V:[%d]\nMAX_V:[%d]\n", min_v, max_v);
#endif
    
    /* If new value lies in the interpretation interval,
    * then this is the decided value */  
    if(max_v >= min_v)
    {   
        if(!((new_v <= max_v) && (new_v >= min_v)))
        {
            adj_needed = ROHC_TRUE;
        } 
    }else
    {
        if(!((new_v>=min_v) ||
                (new_v <= max_v) ))
        {
            adj_needed = ROHC_TRUE;
        }
    }

    if(adj_needed) 
    {
        /* As per RFC 3095, Section 5.3.2.2.2,
         * If new_v is not within this interval, an adjustment is
         * applied by adding N x interval_size to the reconstructed value so
         * that the result is brought within the interpretation interval. 
         * Note that N can be negative. */
        /* If new_v < min_v, N is (+)ve, and if new_v > max_v, N is (-)ve. */
        interval_size = ROHC_POWER_OF_2(bits);
        do
        {
            adj_needed = ROHC_FALSE;
            new_v = new_v + interval_size; 
            if(max_v >= min_v)
            {   
                if(!((new_v <= max_v) && (new_v >= min_v)))
                {
                    adj_needed = ROHC_TRUE;
                } 
            }else
            {
                if(!((new_v>=min_v ) ||
                (new_v <= max_v) ))
                {
                    adj_needed = ROHC_TRUE;
                }
            }
        } while (adj_needed);
    }
    
    *p_dec_sn = new_v;

#ifdef ROHC_UT_FLAG
    fprintf(fp, "DECODED_SN:[%d]\n", *p_dec_sn);
    fClose(fp);
#endif
    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */

    ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:\
                        ENCD_SN[%u]:DECODED_SN[%u]" \
                       "SN DECODING CALLED", \
                        p_rohc_entity->rohc_entity_id.ueIndex,
                        p_rohc_entity->rohc_entity_id.lcId, \
                        ROHC_GET_DSC_CID(p_dsc),
                        p_en_val->value, new_v);

    ROHC_EXIT_FN("rohc_decode_p1_sn");

    return ROHC_SUCCESS;
}
                 
/*****************************************************************************
 * Function Name  : rohc_decode_p2_sn
 * Inputs         : p_rohc_entity- pointer to ROHC entity
 *                  p_dsc - pointer to decompressor context
 *                  *p_en_val - structure containing the compressed  sn_value
 *                       and no. of LSBs it is compressed in ( i.e.. K).
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function decodes the SN value using W-LSB decoding
 *                  mechanism. (This function will only work for 16 bit sn value,
 *                  not for for 32 bit sn (ESP sn).
 *****************************************************************************/
rohc_return_t rohc_decode_p2_sn    ROHC_PARAMS
    (
        (p_rohc_entity, p_dsc, p_en_val, p_dec_sn, p_ecode),
        rohc_entity_t           *p_rohc_entity  _AND_
        rohc_dsc_udp_t          *p_dsc          _AND_
        rohc_encoded_value_t    *p_en_val       _AND_
        rohc_U32bit_t           *p_dec_sn       _AND_
        rohc_error_code_t       *p_ecode
    )
{
    rohc_U16bit_t   p, interval_size ;
    register rohc_U16bit_t   new_v;
    register rohc_U16bit_t  min_v;
    register rohc_U16bit_t max_v;
    rohc_U32bit_t sn_prev;
    rohc_U8bit_t bits;
    rohc_U8bit_t    adj_needed = ROHC_FALSE;

    ROHC_ENTER_FN("rohc_decode_p2_sn");

    /* To maintain the candidate set of last two received SN values */
    /*p_dsc->sn_prev_prev = p_dsc->sn_prev ;*/
            
    /* This macro replaces the p_en_val->bits of p_en_val->value
     * to p_dsc->sn_prev, and saves this value to new_v */
    sn_prev = p_dsc->sn_prev;
    bits = p_en_val->bits;
    ROHC_PUT_LSBS(new_v, sn_prev, p_en_val->value, bits);

#ifdef ROHC_UT_FLAG
    fp = fopen("outfile", "a");
    fprintf(fp, "====================== DECODED SN ========================\n");
    fprintf(fp, "V_REF:[%d]\nENCODED_VAL:[%d]\nK:[%d]\nNEW_V:[%d]\n",
                p_dsc->sn_prev, p_en_val->value, p_en_val->bits, new_v);
#endif
    
    /* k = p_en_val->bits */
    if(p_en_val->bits <= 4)
        p = 1;
    else
        p = ((ROHC_POWER_OF_2(p_en_val->bits - 5)) - 1);
    
    /* To find the min. and max. value of interpretation interval */
    min_v = (rohc_U16bit_t)((rohc_U16bit_t)sn_prev - p);
    max_v = (rohc_U16bit_t)((rohc_U16bit_t)sn_prev +
                            (ROHC_POWER_OF_2(bits) - 1) - p);
    
#ifdef ROHC_UT_FLAG
    fprintf(fp, "MIN_V:[%d]\nMAX_V:[%d]\n", min_v, max_v);
#endif
    
    /* If new value lies in the interpretation interval,
    * then this is the decided value */  
    if(max_v >= min_v)
    {   
        if(!((new_v <= max_v) && (new_v >= min_v)))
        {
            adj_needed = ROHC_TRUE;
        } 
    }else
    {
        if(!((new_v>=min_v) ||
                (new_v <= max_v) ))
        {
            adj_needed = ROHC_TRUE;
        }
    }

    if(adj_needed) 
    {
        /* As per RFC 3095, Section 5.3.2.2.2,
         * If new_v is not within this interval, an adjustment is
         * applied by adding N x interval_size to the reconstructed value so
         * that the result is brought within the interpretation interval. 
         * Note that N can be negative. */
        /* If new_v < min_v, N is (+)ve, and if new_v > max_v, N is (-)ve. */
        interval_size = ROHC_POWER_OF_2(bits);
        do
        {
            adj_needed = ROHC_FALSE;
            new_v = new_v + interval_size; 
            if(max_v >= min_v)
            {   
                if(!((new_v <= max_v) && (new_v >= min_v)))
                {
                    adj_needed = ROHC_TRUE;
                } 
            }else
            {
                if(!((new_v>=min_v ) ||
                (new_v <= max_v) ))
                {
                    adj_needed = ROHC_TRUE;
                }
            }
        } while (adj_needed);
    }
    
    *p_dec_sn = new_v;

#ifdef ROHC_UT_FLAG
    fprintf(fp, "DECODED_SN:[%d]\n", *p_dec_sn);
    fClose(fp);
#endif

    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:\
                          ENCD_SN[%u]:DECODED_SN[%u]" \
                            "SN DECODING CALLED", \
                                p_rohc_entity->rohc_entity_id.ueIndex,
                                p_rohc_entity->rohc_entity_id.lcId,
                                ROHC_GET_DSC_CID(p_dsc),p_en_val->value, new_v);

    ROHC_EXIT_FN("rohc_decode_p2_sn");

    return ROHC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : rohc_decode_p1_inner_ip_id
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_dsc - pointer to decompressor context
 *                  *p_en_val - structure containing the compressed ip_id1 value
 *                        and no. of LSBs it is compressed in (i.e. k).
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function decodes the IP_ID value using W-LSB decoding
 *                  mechanism.
 *****************************************************************************/
rohc_return_t rohc_decode_p1_inner_ip_id   ROHC_PARAMS
    (
        (p_rohc_entity, p_dsc, p_en_val, p_dec_ip_id, p_ecode),
        rohc_entity_t           *p_rohc_entity  _AND_
        rohc_dsc_rtp_t          *p_dsc          _AND_
        rohc_encoded_value_t    *p_en_val       _AND_
        rohc_U16bit_t           *p_dec_ip_id    _AND_
        rohc_error_code_t       *p_ecode

    )
{
    register rohc_U16bit_t   new_v;
    register rohc_U16bit_t  pre_offset;
    register rohc_U16bit_t max_v;
    rohc_U32bit_t sn_prev;
    rohc_U8bit_t bits;
    rohc_U16bit_t    interval_size ;
    //rohc_S8bit_t    adjustment_factor_N;
    rohc_U8bit_t    adj_needed = ROHC_FALSE;

    ROHC_ENTER_FN("rohc_decode_p1_inner_ip_id");

    /* The offset for the Previous Packet */
    sn_prev = p_dsc->sn_prev;
    bits = p_en_val->bits;
    pre_offset = (rohc_U16bit_t)((rohc_U16bit_t)p_dsc->pre_inner_ip_id - 
                (rohc_U16bit_t)sn_prev );

    ROHC_PUT_LSBS(new_v, pre_offset, p_en_val->value, bits);

#ifdef ROHC_UT_FLAG
    fp = fopen("outfile", "a");
    fprintf(fp, "================= DECODED INNER IP-ID ===================\n");
    fprintf(fp, "V_REF:[%d]\nENCODED_VAL:[%d]\nK:[%d]\nNEW_V:[%d]\n", 
                    pre_offset, p_en_val->value, p_en_val->bits, new_v);
#endif

    
    /* k = p_en_val->bits */
    /* To find the min. and max. value of interpretation interval */
    /* p is zero for ip id */
    /* In case of IP-ID decoding, min_v is same as pre_offset */
    max_v = pre_offset + (ROHC_POWER_OF_2(p_en_val->bits) - 1); 

#ifdef ROHC_UT_FLAG
    fprintf(fp, "MIN_V:[%d]\nMAX_V:[%d]\n", pre_offset, max_v);
#endif

    /* If new value lies in the interpretation interval,
    * then this is the decided value */
    if(max_v >= pre_offset)
    {   
        if(!((new_v <= max_v) && (new_v >= pre_offset)))
        {
            adj_needed = ROHC_TRUE;
        } 
    }else
    {
        if(!((new_v>=pre_offset) ||
                (new_v <= max_v) ))
        {
            adj_needed = ROHC_TRUE;
        }
    }
            
    /* IP -ID Adjustement Fix Start */ 
    /* Moved Ip-ID Calculation after adjustment interval Adjustment*/
    /* IP -ID Adjustement Fix End */ 
    if (adj_needed)
    {
        /* As per RFC 3095, Section 5.3.2.2.2,
         * If new_v is not within this interval, an adjustment is
         * applied by adding N x interval_size to the reconstructed value so
         * that the result is brought within the interpretation interval. 
         * Note that N can be negative. */

        /* If new_v < min_v, N is (+)ve, and if new_v > max_v, N is (-)ve. */
        /*adjustment_factor_N = ((new_v > max_v ) ? -1 : 1) ;*/
        interval_size = ROHC_POWER_OF_2(bits);

        ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:"\
                           "ENCD_IP-ID[%u]: appling adujstment" \
                           "because the received value is not"\
                           " in the interpretation interval"\
                           " (%u,%u)\n",\
                           p_rohc_entity->rohc_entity_id.ueIndex,
                           p_rohc_entity->rohc_entity_id.lcId, 
                           ROHC_GET_DSC_CID(p_dsc),
                           /* SPR 9591 fix start */
                           p_en_val->value,
                           /* SPR 9591 fix end */
                           pre_offset,new_v);

        
        do
        {
            new_v = new_v + interval_size; 
            adj_needed = ROHC_FALSE;
            if(max_v >= pre_offset)
            {   
                if(!((new_v <= max_v) && (new_v >= pre_offset)))
                {
                    adj_needed = ROHC_TRUE;
                } 
            }else
            {
                if(!((new_v>=pre_offset) ||
                        (new_v <= max_v) ))
                {
                    adj_needed = ROHC_TRUE;
                }
            }   
         } while (adj_needed);
    }
    
    /* Ip Id Needs to be Calculated After Adjustment is being done*/
    /* IP -ID Adjustement Fix Start */ 
    /* 
    ** SPR_577_FIX: LSB decoding for IP-ID fails after 30000(approx) packets
    ** for profile-2. So corrected the calculation for IP-ID
    */
    *p_dec_ip_id = (rohc_U16bit_t)((rohc_U16bit_t)p_dsc->dpib->decoded_sn + 
                    (rohc_U16bit_t)new_v);
    /* END_SPR_577 */
    /* IP -ID Adjustement Fix End */ 
#ifdef ROHC_UT_FLAG
    fprintf(fp, "DECODED INNER IP-ID:[%d]\n", *p_dec_ip_id);
    fClose(fp);
#endif

    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:\
                        ENCD_IP-ID[%u]:DECODED_IP-ID[%u]" \
                       "IP-ID DECODING CALLED", \
                        p_rohc_entity->rohc_entity_id.ueIndex,
                        p_rohc_entity->rohc_entity_id.lcId, \
                        ROHC_GET_DSC_CID(p_dsc),
                        p_en_val->value, *p_dec_ip_id);

    ROHC_EXIT_FN("rohc_decode_p1_inner_ip_id");
            
    return ROHC_SUCCESS ;
}

/*****************************************************************************
 * Function Name  : rohc_decode_p2_inner_ip_id
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_dsc - pointer to decompressor context
 *                  *p_en_val - structure containing the compressed ip_id1 value
 *                        and no. of LSBs it is compressed in (i.e. k).
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function decodes the IP_ID value using W-LSB decoding
 *                  mechanism.
 *****************************************************************************/
rohc_return_t rohc_decode_p2_inner_ip_id   ROHC_PARAMS
    (
        (p_rohc_entity, p_dsc, p_en_val, p_dec_ip_id, p_ecode),
        rohc_entity_t           *p_rohc_entity  _AND_
        rohc_dsc_udp_t          *p_dsc          _AND_
        rohc_encoded_value_t    *p_en_val       _AND_
        rohc_U16bit_t           *p_dec_ip_id    _AND_
        rohc_error_code_t       *p_ecode

    )
{
    register rohc_U16bit_t   new_v;
    register rohc_U16bit_t  pre_offset;
    register rohc_U16bit_t max_v;
    rohc_U32bit_t sn_prev;
    rohc_U8bit_t bits;
    rohc_U16bit_t    interval_size ;
    //rohc_S8bit_t    adjustment_factor_N;
    rohc_U8bit_t    adj_needed = ROHC_FALSE;

    ROHC_ENTER_FN("rohc_decode_p2_inner_ip_id");

    /* The offset for the Previous Packet */
    sn_prev = p_dsc->sn_prev;
    bits = p_en_val->bits;
    pre_offset = (rohc_U16bit_t)((rohc_U16bit_t)p_dsc->pre_inner_ip_id - 
                (rohc_U16bit_t)sn_prev );

    ROHC_PUT_LSBS(new_v, pre_offset, p_en_val->value, bits);

#ifdef ROHC_UT_FLAG
    fp = fopen("outfile", "a");
    fprintf(fp, "================== DECODED INNER IP-ID =================\n");
    fprintf(fp, "V_REF:[%d]\nENCODED_VAL:[%d]\nK:[%d]\nNEW_V:[%d]\n", 
                  pre_offset, p_en_val->value, p_en_val->bits, new_v);
#endif

    
    /* k = p_en_val->bits */
    /* To find the min. and max. value of interpretation interval */
    /* p is zero for ip id */
    /* In case of IP-ID decoding, min_v is same as pre_offset */
    max_v = pre_offset + (ROHC_POWER_OF_2(p_en_val->bits) - 1); 

#ifdef ROHC_UT_FLAG
    fprintf(fp, "MIN_V:[%d]\nMAX_V:[%d]\n", pre_offset, max_v);
#endif

    /* If new value lies in the interpretation interval,
    * then this is the decided value */
    if(max_v >= pre_offset)
    {   
        if(!((new_v <= max_v) && (new_v >= pre_offset)))
        {
            adj_needed = ROHC_TRUE;
        } 
    }else
    {
        if(!((new_v>=pre_offset) ||
                (new_v <= max_v) ))
        {
            adj_needed = ROHC_TRUE;
        }
    }
            
    /* IP -ID Adjustement Fix Start */ 
    /* Moved Ip-ID Calculation after adjustment interval Adjustment*/
    /* IP -ID Adjustement Fix End */ 
    if (adj_needed)
    {
        /* As per RFC 3095, Section 5.3.2.2.2,
         * If new_v is not within this interval, an adjustment is
         * applied by adding N x interval_size to the reconstructed value so
         * that the result is brought within the interpretation interval. 
         * Note that N can be negative. */

        /* If new_v < min_v, N is (+)ve, and if new_v > max_v, N is (-)ve. */
        /*adjustment_factor_N = ((new_v > max_v ) ? -1 : 1) ;*/
        interval_size = ROHC_POWER_OF_2(bits);

        ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:"\
                           "ENCD_IP-ID[%u]: appling adujstment" \
                           "because the received value is not"\
                           " in the interpretation interval"\
                           " (%u,%u)\n",\
                           p_rohc_entity->rohc_entity_id.ueIndex,
                           p_rohc_entity->rohc_entity_id.lcId,
                           ROHC_GET_DSC_CID(p_dsc),
                           /* SPR 9591 fix start */
                           p_en_val->value,
                           /* SPR 9591 fix end */
                           pre_offset,new_v);

        
        do
        {
            new_v = new_v + interval_size; 
            adj_needed = ROHC_FALSE;
            if(max_v >= pre_offset)
            {   
                if(!((new_v <= max_v) && (new_v >= pre_offset)))
                {
                    adj_needed = ROHC_TRUE;
                } 
            }else
            {
                if(!((new_v>=pre_offset) ||
                        (new_v <= max_v) ))
                {
                    adj_needed = ROHC_TRUE;
                }
            }   
         } while (adj_needed);
    }
    
    /* Ip Id Needs to be Calculated After Adjustment is being done*/
    /* IP -ID Adjustement Fix Start */ 
    /* 
    ** SPR_577_FIX: LSB decoding for IP-ID fails after 30000(approx) packets
    ** for profile-2. So corrected the calculation for IP-ID
    */
    *p_dec_ip_id = (rohc_U16bit_t)((rohc_U16bit_t)p_dsc->dpib->decoded_sn + 
                    (rohc_U16bit_t)new_v);
    /* END_SPR_577 */
    /* IP -ID Adjustement Fix End */ 
#ifdef ROHC_UT_FLAG
    fprintf(fp, "DECODED INNER IP-ID:[%d]\n", *p_dec_ip_id);
    fClose(fp);
#endif

    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:\
                                            ENCD_IP-ID[%u]:DECODED_IP-ID[%u]" \
                                            "IP-ID DECODING CALLED", \
                                            p_rohc_entity->rohc_entity_id.ueIndex,
                                            p_rohc_entity->rohc_entity_id.lcId, \
                                            ROHC_GET_DSC_CID(p_dsc),
                                            p_en_val->value, *p_dec_ip_id);

    ROHC_EXIT_FN("rohc_decode_p2_inner_ip_id");
            
    return ROHC_SUCCESS ;
}

/*****************************************************************************
 * Function Name  : rohc_decode_p1_outer_ip_id
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_dsc - pointer to decompressor context
 *                  *p_en_val - structure containing the compressed ip_id2 value
 *                  and no. of LSBs it is compressed in (i.e. k).
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function decodes the IP_ID value using W-LSB decoding
 *                  mechanism.
 *****************************************************************************/
rohc_return_t rohc_decode_p1_outer_ip_id   ROHC_PARAMS
    (
        (p_rohc_entity, p_dsc, p_en_val, p_dec_ip_id, p_ecode),
        rohc_entity_t           *p_rohc_entity  _AND_
        rohc_dsc_rtp_t          *p_dsc          _AND_
        rohc_encoded_value_t    *p_en_val       _AND_
        rohc_U16bit_t           *p_dec_ip_id    _AND_
        rohc_error_code_t       *p_ecode

    )
{
    register rohc_U16bit_t   new_v;
    register rohc_U16bit_t  pre_offset;
    register rohc_U16bit_t max_v;
    rohc_U32bit_t sn_prev;
    rohc_U8bit_t bits;
    rohc_U32bit_t       interval_size ;
    rohc_S8bit_t        adjustment_factor_N;

    ROHC_ENTER_FN("rohc_decode_p1_outer_ip_id");
    
    /* The offset for the Previous Packet */
    sn_prev = p_dsc->sn_prev;
    bits = p_en_val->bits;
    pre_offset = (rohc_S32bit_t)ROHC_ABS(((rohc_U32bit_t)
                            p_dsc->pre_outer_ip_id) - sn_prev);

    ROHC_PUT_LSBS(new_v, pre_offset, p_en_val->value, bits);
    
#ifdef ROHC_UT_FLAG
    fp = fopen("outfile", "a");
    fprintf(fp, "================= DECODED OUTER IP-ID ===================\n");
    fprintf(fp, "V_REF:[%d]\nENCODED_VAL:[%d]\nK:[%d]\nNEW_V:[%d]\n", 
                    pre_offset, p_en_val->value, p_en_val->bits, new_v);
#endif

    /* k = p_en_val->bits */
        
    /* To find the min. and max. value of interpretation interval */
    /* p is zero for ip id */
    /* In case of IP-ID decoding, min_v is same as pre_offset */
    max_v = pre_offset + (ROHC_POWER_OF_2(p_en_val->bits) - 1); 

#ifdef ROHC_UT_FLAG
    fprintf(fp, "MIN_V:[%d]\nMAX_V:[%d]\n", pre_offset, max_v);
#endif

    /* If new value lies in the interpretation interval,
    * then this is the decided value */
    
    if (!((new_v <= max_v) && (new_v >= pre_offset)))
    {
        /* As per RFC 3095, Section 5.3.2.2.2,
         * If new_v is not within this interval, an adjustment is
         * applied by adding N x interval_size to the reconstructed value so
         * that the result is brought within the interpretation interval. 
         * Note that N can be negative. */

        /* If new_v < min_v, N is (+)ve, and if new_v > max_v, N is (-)ve. */
        adjustment_factor_N = ((new_v > max_v ) ? -1 : 1) ;
        interval_size = ROHC_POWER_OF_2(bits);
        do
        {
            new_v = new_v + (adjustment_factor_N) * interval_size; 
        } while (!((new_v <= max_v) && (new_v >= pre_offset)));
    }
    
    if ((rohc_U32bit_t)p_dsc->pre_inner_ip_id < p_dsc->sn_prev) 
        *p_dec_ip_id = ((rohc_S32bit_t)p_dsc->dpib->decoded_sn) - new_v;
    else
        *p_dec_ip_id = ((rohc_S32bit_t)p_dsc->dpib->decoded_sn) + new_v;
    
#ifdef ROHC_UT_FLAG
        fprintf(fp, "DECODED OUTER IP-ID:[%d]\n", *p_dec_ip_id);
        fClose(fp);
#endif

        /* +- SPR 17777 */
/* SPR 20636 Changes Start */
        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
        /* +- SPR 17777 */
        ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:\
                ENCD_IP-ID[%u]:DECODED_IP-ID[%u]" \
                "IP-ID DECODING CALLED", \
                        p_rohc_entity->rohc_entity_id.ueIndex,
                        p_rohc_entity->rohc_entity_id.lcId, \
                        ROHC_GET_DSC_CID(p_dsc),
                        p_en_val->value, *p_dec_ip_id);

    ROHC_EXIT_FN("rohc_decode_p1_outer_ip_id");
            
    return ROHC_SUCCESS ;
}

/*****************************************************************************
 * Function Name  : rohc_decode_p2_outer_ip_id
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_dsc - pointer to decompressor context
 *                  *p_en_val - structure containing the compressed ip_id2 value
 *                  and no. of LSBs it is compressed in (i.e. k).
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function decodes the IP_ID value using W-LSB decoding
 *                  mechanism.
 *****************************************************************************/
rohc_return_t rohc_decode_p2_outer_ip_id   ROHC_PARAMS
    (
        (p_rohc_entity, p_dsc, p_en_val, p_dec_ip_id, p_ecode),
        rohc_entity_t           *p_rohc_entity  _AND_
        rohc_dsc_udp_t          *p_dsc          _AND_
        rohc_encoded_value_t    *p_en_val       _AND_
        rohc_U16bit_t           *p_dec_ip_id    _AND_
        rohc_error_code_t       *p_ecode

    )
{
    register rohc_U16bit_t   new_v;
    register rohc_U16bit_t  pre_offset;
    register rohc_U16bit_t max_v;
    rohc_U32bit_t sn_prev;
    rohc_U8bit_t bits;
    rohc_U32bit_t       interval_size ;
    rohc_S8bit_t        adjustment_factor_N;

    ROHC_ENTER_FN("rohc_decode_p2_outer_ip_id");
    
    /* The offset for the Previous Packet */
    sn_prev = p_dsc->sn_prev;
    bits = p_en_val->bits;
    pre_offset = (rohc_S32bit_t)ROHC_ABS(((rohc_U32bit_t)p_dsc->pre_outer_ip_id) - sn_prev);

    ROHC_PUT_LSBS(new_v, pre_offset, p_en_val->value, bits);
    
#ifdef ROHC_UT_FLAG
    fp = fopen("outfile", "a");
    fprintf(fp, "================= DECODED OUTER IP-ID ===================\n");
    fprintf(fp, "V_REF:[%d]\nENCODED_VAL:[%d]\nK:[%d]\nNEW_V:[%d]\n", 
                  pre_offset, p_en_val->value, p_en_val->bits, new_v);
#endif

    /* k = p_en_val->bits */
        
    /* To find the min. and max. value of interpretation interval */
    /* p is zero for ip id */
    /* In case of IP-ID decoding, min_v is same as pre_offset */
    max_v = pre_offset + (ROHC_POWER_OF_2(p_en_val->bits) - 1); 

#ifdef ROHC_UT_FLAG
    fprintf(fp, "MIN_V:[%d]\nMAX_V:[%d]\n", pre_offset, max_v);
#endif

    /* If new value lies in the interpretation interval,
    * then this is the decided value */
    
    if (!((new_v <= max_v) && (new_v >= pre_offset)))
    {
        /* As per RFC 3095, Section 5.3.2.2.2,
         * If new_v is not within this interval, an adjustment is
         * applied by adding N x interval_size to the reconstructed value so
         * that the result is brought within the interpretation interval. 
         * Note that N can be negative. */

        /* If new_v < min_v, N is (+)ve, and if new_v > max_v, N is (-)ve. */
        adjustment_factor_N = ((new_v > max_v ) ? -1 : 1) ;
        interval_size = ROHC_POWER_OF_2(bits);
        do
        {
            new_v = new_v + (adjustment_factor_N) * interval_size; 
        } while (!((new_v <= max_v) && (new_v >= pre_offset)));
    }
    
    if ((rohc_U32bit_t)p_dsc->pre_inner_ip_id < p_dsc->sn_prev) 
        *p_dec_ip_id = ((rohc_S32bit_t)p_dsc->dpib->decoded_sn) - new_v;
    else
        *p_dec_ip_id = ((rohc_S32bit_t)p_dsc->dpib->decoded_sn) + new_v;
    
#ifdef ROHC_UT_FLAG
        fprintf(fp, "DECODED OUTER IP-ID:[%d]\n", *p_dec_ip_id);
        fClose(fp);
#endif

        /* +- SPR 17777 */
/* SPR 20636 Changes Start */
        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
        /* +- SPR 17777 */
        ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:\
                ENCD_IP-ID[%u]:DECODED_IP-ID[%u]" \
                                            "IP-ID DECODING CALLED", \
                                            p_rohc_entity->rohc_entity_id.ueIndex,
                                            p_rohc_entity->rohc_entity_id.lcId, \
                                            ROHC_GET_DSC_CID(p_dsc),
                                            p_en_val->value, *p_dec_ip_id);

    ROHC_EXIT_FN("rohc_decode_p2_outer_ip_id");
            
    return ROHC_SUCCESS ;
}

/*****************************************************************************
 * Function Name  : rohc_decode_p1_ts
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_dsc - pointer to decompressor context
 *                  *p_en_val - structure containing the compressed ts value
 *                       and no. of LSBs it is compressed in.
 *                  *p_dec_ts - pointer to decoded RTP TS.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This is the main routine for decoding RTP Timestamp.
 *                  It is responsible for the deciding which decoding routine to
 *                  call and based on that call the appropriate decoding routine
 * LOGIC :
 *    If DSC contains the jitter and clock resolution of decompressor, then
 *    Call the rohc_decode_timer_based_ts routine to decompress the RTP Timestamp
 *    value. 
 *    Else
 *      Call the rohc_decode_scaled_ts routine to decompress the RTP Timestamp
 *      value.
 *****************************************************************************/
rohc_return_t rohc_decode_p1_ts    ROHC_PARAMS
    (
        (p_rohc_entity, p_dsc, p_en_val, p_dec_ts, p_ecode, sn_delta),
        rohc_entity_t           *p_rohc_entity      _AND_
        rohc_dsc_rtp_t          *p_dsc              _AND_
        rohc_encoded_value_t    *p_en_val           _AND_
        rohc_U32bit_t           *p_dec_ts           _AND_
        rohc_error_code_t       *p_ecode            _AND_
	/*Start SPR 5818*/
        rohc_U32bit_t           sn_delta
	/*End SPR 5818*/
    )
{
    ROHC_ENTER_FN("rohc_decode_p1_ts");

    if((p_dsc->bit_mask == (ROHC_JIT_PRESENT | ROHC_CLK_RES_PRESENT)))
    {
        /* +- SPR 17777 */
/* SPR 20636 Changes Start */
        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
        /* +- SPR 17777 */
        ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:"\
                "TIMER BASED TS DECODING CALLED",\
                           p_rohc_entity->rohc_entity_id.ueIndex,
                           p_rohc_entity->rohc_entity_id.lcId,\
                           ROHC_GET_DSC_CID(p_dsc));
                                  
        rohc_decode_p1_timer_based_ts(p_rohc_entity, p_dsc, p_en_val, p_dec_ts);
    } 
    else
    {
        /* +- SPR 17777 */
/* SPR 20636 Changes Start */
        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
        /* +- SPR 17777 */
        ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:"\
                "SCALED TS DECODING CALLED",\
                          p_rohc_entity->rohc_entity_id.ueIndex,
                          p_rohc_entity->rohc_entity_id.lcId,\
                          ROHC_GET_DSC_CID(p_dsc));
                          
        /*Start SPR 5818*/
        rohc_decode_p1_scaled_ts(p_rohc_entity, p_dsc, p_en_val, p_dec_ts, sn_delta);
        /*End SPR 5818*/
    }

    ROHC_EXIT_FN("rohc_decode_p1_ts");

    return ROHC_SUCCESS ;
    
}

/*****************************************************************************
 * Function Name  : rohc_ed_p6_update_gsw_sn
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to compressor context
 *                  sn - The sequence  number to be updated in GSW.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function is called  to update the TCP SN value in GSW.
 *                  It is called when IR-DYNs are sent, thus GSW is to be updated,
 *                  but no encoding to be done.
 *****************************************************************************/
rohc_return_t   rohc_ed_p6_update_gsw_sn       ROHC_PARAMS
    (
        (p_rohc_entity, p_csc, sn),
        rohc_entity_t   *p_rohc_entity  _AND_
        rohc_csc_tcp_t  *p_csc          _AND_
        rohc_U32bit_t   sn              
    )
{
    rohc_U32bit_t loop_var=0;
    ROHC_ENTER_FN("rohc_ed_p6_update_gsw_sn");

    /* update the min. and max. value of SN. It is the min. and max. \
     * value from start_index to end_index */
    p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].sn = sn;
   
    loop_var = p_csc->rohc_gsw.start_index;
    if(loop_var == p_csc->rohc_gsw.end_index) /* only one element in the GSW */
    {
        p_csc->rohc_gsw.min_sn = p_csc->rohc_gsw.max_sn = sn;   
    }  
    else /* more than one elements exist*/
    {   
        p_csc->rohc_gsw.min_sn=
                        p_csc->rohc_gsw.rohc_gsw_elem[loop_var].sn;
        do
        {
            loop_var =(loop_var +1)%ROHC_MAX_LENGTH_GSW_PROF6;
            if( p_csc->rohc_gsw.rohc_gsw_elem[loop_var].sn < 
                                p_csc->rohc_gsw.min_sn)
                p_csc->rohc_gsw.min_sn = 
                        p_csc->rohc_gsw.rohc_gsw_elem[loop_var].sn;
            if( p_csc->rohc_gsw.rohc_gsw_elem[loop_var].sn > 
                                p_csc->rohc_gsw.max_sn)
                p_csc->rohc_gsw.max_sn = 
                        p_csc->rohc_gsw.rohc_gsw_elem[loop_var].sn;

        }
        while(loop_var != p_csc->rohc_gsw.end_index);
    }   

    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_ed_p6_update_gsw_sn");
    return ROHC_SUCCESS ;
}


/*****************************************************************************
 * Function Name  : rohc_encode_p6_sn
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to cpmressor context
 *                  sn - SN to be encoded
 *                  *p_en_val - structure to be filled, contains the sn_value
 *                          to be compressed, and it's no. of LSBs to be sent.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function encodes the sequence number received in the
 *                  packet using W-LSB encoding.  To find the value of "K" i.e..
 *                  minimum LSB of SN to be sent, it invokes the function
 *                  rohc_sn_g, which inturn calls function rohc_is_within_in_intval
 *                  and rohc_compute_k.
 *****************************************************************************/
rohc_return_t rohc_encode_p6_sn        ROHC_PARAMS
    (
        (p_rohc_entity, p_csc, sn, p_en_val, lsb, offset_param),
        rohc_entity_t           *p_rohc_entity  _AND_
        rohc_csc_tcp_t          *p_csc          _AND_
        rohc_U32bit_t           sn              _AND_
        rohc_encoded_value_t    *p_en_val       _AND_
        rohc_U8bit_t            lsb             _AND_
        rohc_U32bit_t           offset_param    
    )
{
    rohc_U32bit_t       v_ref_min, v_ref_max;

    ROHC_ENTER_FN("rohc_encode_p6_sn");
    
    /* The minimum and maximum value of SN in GSW from start_index to end_index */
    v_ref_min = p_csc->rohc_gsw.min_sn;
    v_ref_max = p_csc->rohc_gsw.max_sn;
    if(rohc_is_in_intval(v_ref_min, offset_param, lsb, sn) &&
            rohc_is_in_intval(v_ref_max, offset_param, lsb, sn))
    {
        p_en_val->bits  = lsb;
        return ROHC_SUCCESS; 
    }
    else
    {
        /* +- SPR 17777 */
        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
                ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
        /* +- SPR 17777 */
        return ROHC_FAILURE; 
    }
}
 


/*****************************************************************************
 * Function Name  : rohc_encode_p6_scaled_sn
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to cpmressor context
 *                  scaled_sn - Scaled value to be encoded
 *                  *p_en_val - structure to be filled, contains the sn_value
 *                          to be compressed, and it's no. of LSBs to be sent.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function encodes the sequence number received in the
 *                  packet using W-LSB encoding.  To find the value of "K" i.e..
 *                  minimum LSB of SN to be sent, it invokes the function
 *                  rohc_sn_g, which inturn calls function rohc_is_within_in_intval
 *                  and rohc_compute_k.
 *****************************************************************************/
rohc_return_t rohc_encode_p6_scaled_sn        ROHC_PARAMS
    (
        (scaled_sn, p_en_val, lsb, offset_param, prev_sn, stride),
        rohc_U32bit_t           scaled_sn       _AND_
        rohc_encoded_value_t    *p_en_val       _AND_
        rohc_U8bit_t            lsb             _AND_
        rohc_U32bit_t           offset_param    _AND_
        rohc_U32bit_t           prev_sn         _AND_
        rohc_U16bit_t           stride        
        )
{
    rohc_U32bit_t       ref_val;

    ROHC_ENTER_FN("rohc_encode_p6_scaled_sn");
    
    /* The minimum and maximum value of ACK in GSW from start_index to end_index */
    ref_val = prev_sn/stride;

    if(rohc_is_in_intval(ref_val, offset_param, lsb, scaled_sn))
    {
        p_en_val->bits  = lsb;
        return ROHC_SUCCESS; 
    }
    else
    {
        return ROHC_FAILURE; 
    }
}

/*****************************************************************************
 * Function Name  : rohc_encode_p6_ttl
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to cpmressor context
 *                  
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function encodes the sequence number received in the
 *                  packet using W-LSB encoding.  To find the value of "K" i.e..
 *                  minimum LSB of SN to be sent, it invokes the function
 *                  rohc_sn_g, which inturn calls function rohc_is_within_in_intval
 *                  and rohc_compute_k.
 *****************************************************************************/
rohc_return_t rohc_encode_p6_ttl  ROHC_PARAMS
(
 (ttl, ref_val , lsb, offset_param),
 rohc_U8bit_t            ttl             _AND_
 rohc_U8bit_t            ref_val         _AND_
 rohc_U8bit_t            lsb             _AND_
 rohc_U32bit_t           offset_param    
 )
{

    ROHC_ENTER_FN("rohc_encode_p6_ttl");

    if(rohc_is_in_intval(ref_val, offset_param, lsb, ttl))
    {
        return ROHC_SUCCESS; 
    }
    else
    {
        return ROHC_FAILURE; 
    }
}

/*****************************************************************************
 * Function Name  : rohc_ed_p6_update_gsw_ack
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to compressor context
 *                  ack - The ack number to be updated in GSW.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function is called  to update the TCP ACK value in GSW.
 *                  It is called when IR-DYNs are sent, thus GSW is to be updated,
 *                  but no encoding to be done.
 *****************************************************************************/
rohc_return_t   rohc_ed_p6_update_gsw_ack       ROHC_PARAMS
    (
        (p_rohc_entity, p_csc, ack),
        rohc_entity_t   *p_rohc_entity  _AND_
        rohc_csc_tcp_t  *p_csc          _AND_
        rohc_U32bit_t   ack              
    )
{
    rohc_U32bit_t loop_var=0;
    ROHC_ENTER_FN("rohc_ed_p6_update_gsw_ack");

    /* update the min. and max. value of ACK. It is the min. and max. \
     * value from start_index to end_index */
    p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].ack = ack;
   
    loop_var = p_csc->rohc_gsw.start_index;
    if(loop_var == p_csc->rohc_gsw.end_index) /* only one element in the GSW */
    {
        p_csc->rohc_gsw.min_ack = p_csc->rohc_gsw.max_ack = ack;   
    }  

    else /* more than one elements exist*/
    {   
        p_csc->rohc_gsw.min_ack=
                        p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ack;
        do
        {
            loop_var =(loop_var +1)%ROHC_MAX_LENGTH_GSW_PROF6;
            if( p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ack < 
                                p_csc->rohc_gsw.min_ack)
                p_csc->rohc_gsw.min_ack = 
                        p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ack;
            if( p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ack > 
                                p_csc->rohc_gsw.max_ack)
                p_csc->rohc_gsw.max_ack = 
                        p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ack;

        }
        while(loop_var != p_csc->rohc_gsw.end_index);
    }   
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
     
    ROHC_EXIT_FN("rohc_ed_p6_update_gsw_ack");
    return ROHC_SUCCESS ;
}

/*****************************************************************************
 * Function Name  : rohc_encode_p6_ack
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to cpmressor context
 *                  ack - ACK to be encoded
 *                  *p_en_val - structure to be filled, contains the sn_value
 *                          to be compressed, and it's no. of LSBs to be sent.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function encodes the sequence number received in the
 *                  packet using W-LSB encoding.  To find the value of "K" i.e..
 *                  minimum LSB of SN to be sent, it invokes the function
 *                  rohc_sn_g, which inturn calls function rohc_is_within_in_intval
 *                  and rohc_compute_k.
 *****************************************************************************/
rohc_return_t rohc_encode_p6_ack        ROHC_PARAMS
    (
        (p_rohc_entity, p_csc, ack, p_en_val, lsb, offset_param),
        rohc_entity_t           *p_rohc_entity  _AND_
        rohc_csc_tcp_t          *p_csc          _AND_
        rohc_U32bit_t           ack              _AND_
        rohc_encoded_value_t    *p_en_val       _AND_
        rohc_U8bit_t            lsb             _AND_
        rohc_U32bit_t           offset_param    
    )
{
    rohc_U32bit_t       v_ref_min, v_ref_max;

    ROHC_ENTER_FN("rohc_encode_p6_ack");
    
    /* The minimum and maximum value of ACK in GSW from start_index to end_index */
    v_ref_min = p_csc->rohc_gsw.min_ack;
    v_ref_max = p_csc->rohc_gsw.max_ack;
    if(rohc_is_in_intval(v_ref_min, offset_param, lsb, ack) &&
            rohc_is_in_intval(v_ref_max, offset_param, lsb, ack))
    {
        p_en_val->bits  = lsb;
        return ROHC_SUCCESS; 
    }
    else
    {
        /* +- SPR 17777 */
        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
                ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
        /* +- SPR 17777 */
        return ROHC_FAILURE; 
    }
}
 

/*****************************************************************************
 * Function Name  : rohc_encode_p6_scaled_ack
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to cpmressor context
 *                  scaled_ack - Scaled value to be encoded
 *                  *p_en_val - structure to be filled, contains the sn_value
 *                          to be compressed, and it's no. of LSBs to be sent.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function encodes the ack number received in the
 *                  packet using W-LSB encoding.  To find the value of "K" i.e..
 *                  minimum LSB of SN to be sent, it invokes the function
 *                  rohc_sn_g, which inturn calls function rohc_is_within_in_intval
 *                  and rohc_compute_k.
 *****************************************************************************/
rohc_return_t rohc_encode_p6_scaled_ack        ROHC_PARAMS
    (
        (p_rohc_entity, p_csc, scaled_ack, p_en_val, lsb, offset_param),
        rohc_entity_t           *p_rohc_entity  _AND_
        rohc_csc_tcp_t          *p_csc          _AND_
        rohc_U32bit_t           scaled_ack       _AND_
        rohc_encoded_value_t    *p_en_val       _AND_
        rohc_U8bit_t            lsb             _AND_
        rohc_U32bit_t           offset_param    
    )
{
    rohc_U32bit_t       ref_val;

    ROHC_ENTER_FN("rohc_encode_p6_scaled_ack");
    
    ref_val = p_csc->old_tcp_ack/p_csc->field_value[ROHC_TCP_ACK_STRIDE_FID-
        ROHC_VAR_FID_BASE];

    if(rohc_is_in_intval(ref_val, offset_param, lsb, scaled_ack))
    {
        p_en_val->bits  = lsb;
        return ROHC_SUCCESS; 
    }
    else
    {
        /* +- SPR 17777 */
        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
                ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
        /* +- SPR 17777 */
        return ROHC_FAILURE; 
    }
}

/*****************************************************************************
 * Function Name  : rohc_encode_p6_ts_opt
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to cpmressor context
 *                  ts_val - TS to be encoded
 *                  *p_en_val - structure to be filled, contains the sn_value
 *                          to be compressed, and it's no. of LSBs to be sent.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function encodes the timestamp value received in the
 *                  packet using W-LSB encoding.  To find the value of "K" i.e..
 *                  minimum LSB of SN to be sent, it invokes the function
 *                  rohc_sn_g, which inturn calls function rohc_is_within_in_intval
 *                  and rohc_compute_k.
 *****************************************************************************/
rohc_return_t rohc_encode_p6_ts_opt    ROHC_PARAMS
    (
        (p_rohc_entity, ts_val, p_en_val,v_ref_min,v_ref_max),
        rohc_entity_t           *p_rohc_entity  _AND_
        rohc_U32bit_t           ts_val          _AND_
        rohc_encoded_value_t    *p_en_val       _AND_
        rohc_U32bit_t           v_ref_min       _AND_
        rohc_U32bit_t           v_ref_max      
    )
{

    ROHC_ENTER_FN("rohc_encode_p6_ts");

    /* The minimum and maximum value of SN in GSW from start_index to end_index */
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            0,0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_UT_TRACE((stderr,"V_REF_MIN:[%d] \nV_REF_MAX:[%d]"
                " \nTS:[%d] \n", v_ref_min, v_ref_max, ts_val));
    /* first check with lsb = 7 and offset = -1 */
    if(rohc_is_in_intval(v_ref_min, -1, 7, ts_val) &&
            rohc_is_in_intval(v_ref_max, -1, 7, ts_val))
    {
        p_en_val->bits  = 7;
        p_en_val->value  = ts_val;
        return ROHC_SUCCESS; 
    }
    else if(rohc_is_in_intval(v_ref_min, -1 ,14 , ts_val) &&
            rohc_is_in_intval(v_ref_max, -1, 14, ts_val))
    {
        p_en_val->bits  = 14;
        p_en_val->value  = ts_val;
        return ROHC_SUCCESS; 
    }

    else if(rohc_is_in_intval(v_ref_min, 262144 , 21 , ts_val) &&
            rohc_is_in_intval(v_ref_max, 262144, 21 , ts_val))
    {
        p_en_val->bits  = 21;
        p_en_val->value  = ts_val;
        return ROHC_SUCCESS; 
    }

    else
    {
        if (rohc_is_in_intval(v_ref_min, 67108864, 29 , ts_val) &&
                rohc_is_in_intval(v_ref_max, 67108864, 29 , ts_val))
        {
            p_en_val->bits  = 29;
            p_en_val->value  = ts_val;
            return ROHC_SUCCESS; 
        }
    }

#ifdef ROHC_UT_FLAG
    fprintf(fp, "MIN_K:[%d] \nMAX_K:[%d] \nK:[%d] \n", k1, k2, k);
    fprintf(fp, "P_EN_VAL->VALUE:[%d] \nP_EN_VAL->BITS:[%d] \n", 
            p_en_val->value, p_en_val->bits);
    fprintf(fp, "\n");
    fClose(fp);
#endif
    /* SPR 9591 fix start */
    ROHC_LOG(LOG_INFO,"ED:EID[%u:%u]:"\
            "ENCD_TS[%u]:BITS[%u]" \
            "TS ENCODING CALLED\n", \
            p_rohc_entity->rohc_entity_id.ueIndex,
            p_rohc_entity->rohc_entity_id.lcId,
            ts_val, p_en_val->bits);
    /* SPR 9591 fix end */
    ROHC_EXIT_FN("rohc_encode_p6_ts_opt");

    return ROHC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : rohc_ed_p6_update_gsw_ts_val
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to compressor context
 *                  ts_val - The timestamp value to be updated in GSW.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function is called  to update the TCP TS VAL  in GSW.
 *                  It is called when IR-DYNs are sent, thus GSW is to be updated,
 *                  but no encoding to be done.
 *****************************************************************************/
rohc_return_t   rohc_ed_p6_update_gsw_ts_val       ROHC_PARAMS
    (
        (p_rohc_entity, p_csc, ts_val),
        rohc_entity_t   *p_rohc_entity  _AND_
        rohc_csc_tcp_t  *p_csc          _AND_
        rohc_U32bit_t   ts_val              
    )
{
    rohc_U32bit_t loop_var=0;
    ROHC_ENTER_FN("rohc_ed_p6_update_gsw_ts_val");

    /* update the min. and max. value of SN. It is the min. and max. \
     * value from start_index to end_index */
    p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].ts_val = ts_val;
   
    loop_var = p_csc->rohc_gsw.start_index;
    if((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ts_opt_recvd)&&
            (loop_var == p_csc->rohc_gsw.end_index)) /* only one element in the GSW */
    {
        p_csc->rohc_gsw.min_ts_val = p_csc->rohc_gsw.max_ts_val = ts_val;   
    }  

    else /* more than one elements exist*/
    {   
        p_csc->rohc_gsw.min_ts_val = p_csc->rohc_gsw.max_ts_val = ts_val;
        do
        {
            if((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ts_opt_recvd)&&
                    (p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ts_val < 
                                p_csc->rohc_gsw.min_ts_val))
        p_csc->rohc_gsw.min_ts_val=
                        p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ts_val;
            if((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ts_opt_recvd)&&
                    (p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ts_val > 
                                p_csc->rohc_gsw.max_ts_val))
                p_csc->rohc_gsw.max_ts_val = 
                        p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ts_val;

            loop_var =(loop_var +1)%ROHC_MAX_LENGTH_GSW_PROF6;
        }
        while(loop_var != p_csc->rohc_gsw.end_index);
    }   

    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_ed_p6_update_gsw_ts_val");
    return ROHC_SUCCESS ;
}

/*****************************************************************************
 * Function Name  : rohc_ed_p6_update_gsw_ts_echo
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to compressor context
 *                  ts_echo - The timestamp echo value to be updated in GSW.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function is called  to update the TCP TS ECHO  in GSW.
 *                  It is called when IR-DYNs are sent, thus GSW is to be updated,
 *                  but no encoding to be done.
 *****************************************************************************/
rohc_return_t   rohc_ed_p6_update_gsw_ts_echo       ROHC_PARAMS
    (
        (p_rohc_entity, p_csc, ts_echo),
        rohc_entity_t   *p_rohc_entity  _AND_
        rohc_csc_tcp_t  *p_csc          _AND_
        rohc_U32bit_t   ts_echo              
    )
{
    rohc_U32bit_t loop_var=0;
    ROHC_ENTER_FN("rohc_ed_p6_update_gsw_ts_echo");

    /* update the min. and max. value of SN. It is the min. and max. \
     * value from start_index to end_index */
    p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].ts_echo = ts_echo;
   
    loop_var = p_csc->rohc_gsw.start_index;
    if((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ts_opt_recvd)&&
            (loop_var == p_csc->rohc_gsw.end_index)) /* only one element in the GSW */
    {
        p_csc->rohc_gsw.min_ts_echo = p_csc->rohc_gsw.max_ts_echo = ts_echo;   
    }  

    else /* more than one elements exist*/
    {   
        p_csc->rohc_gsw.min_ts_echo= p_csc->rohc_gsw.max_ts_echo = ts_echo;
        do
        {
            if((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ts_opt_recvd)&&
                    (p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ts_echo < 
                                p_csc->rohc_gsw.min_ts_echo))
        p_csc->rohc_gsw.min_ts_echo=
                        p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ts_echo;
            if((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ts_opt_recvd)&&
                    (p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ts_echo > 
                                p_csc->rohc_gsw.max_ts_echo))
                p_csc->rohc_gsw.max_ts_echo = 
                        p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ts_echo;

            loop_var =(loop_var +1)%ROHC_MAX_LENGTH_GSW_PROF6;
        }
        while(loop_var != p_csc->rohc_gsw.end_index);
    }   

    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_ed_p6_update_gsw_ts_echo");
    return ROHC_SUCCESS ;
}


/*****************************************************************************
 * Function Name  : rohc_ed_p6_update_window
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to cpmressor context
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function is called  to update the GSW. It increments 
 *                  the end_index of GSW.
 *      LOGIC :
 *          Increment the end index to point to next received packet.
 *          If end_index id greater than last index, then point to 
 *          zero(th) index. This was we make GSW as circular queue.
 *****************************************************************************/
rohc_return_t   rohc_ed_p6_update_window       ROHC_PARAMS
    (
        (p_rohc_entity, p_csc),
        rohc_entity_t *p_rohc_entity    _AND_
        rohc_csc_tcp_t    *p_csc
    )
{
    ROHC_ENTER_FN("rohc_ed_p6_update_window");
    p_csc->rohc_gsw.end_index = (p_csc->rohc_gsw.end_index+1)
                                            % ROHC_MAX_LENGTH_GSW_PROF6;

    if(p_csc->rohc_gsw.iteration_flag != 0 && (p_csc->rohc_gsw.end_index == 
            p_csc->rohc_gsw.start_index) )
    {
        p_csc->rohc_gsw.start_index = (p_csc->rohc_gsw.start_index+1)
                                            % ROHC_MAX_LENGTH_GSW_PROF6;
    } 
    p_csc->rohc_gsw.iteration_flag =1;  

    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_ed_p6_update_window");

    return ROHC_SUCCESS ;
}

/*****************************************************************************
 * Function Name  : rohc_ed_p6_update_gsw_msn
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_csc - pointer to compressor context
 *                  sn - The sequence  number to be updated in GSW.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function is called  to update the TCP MSN value in GSW.
 *                  It is called when IR-DYNs are sent, thus GSW is to be updated,
 *                  but no encoding to be done.
 *****************************************************************************/
rohc_return_t   rohc_ed_p6_update_gsw_msn       ROHC_PARAMS
    (
        (p_rohc_entity, p_csc, msn),
        rohc_entity_t   *p_rohc_entity  _AND_
        rohc_csc_tcp_t  *p_csc          _AND_
        rohc_U32bit_t   msn              
    )
{
    ROHC_ENTER_FN("rohc_ed_p6_update_gsw_msn");

    /* update the min. and max. value of MSN. It is the min. and max. \
     * value from start_index to end_index */
    p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].msn = msn;

    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */


    ROHC_EXIT_FN("rohc_ed_p6_update_gsw_msn");
    return ROHC_SUCCESS ;
}


/*****************************************************************************
 * Function Name  : rohc_ack_handling_p6
 * Inputs         : p_csc - pointer to cpmressor context
 *                  msn - The sequence for which ACK comes.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function is called whenever a ACK is received at the
 *                  compressor side. In this function we will slide the start
 *                  index of GSW.
 *        LOGIC :
 *           Search for MSN received in ACK between the start index and end 
 *           index.Make the start index point to the index, where value of 
 *           acked MSN matches.
 *****************************************************************************/
rohc_return_t   rohc_ack_handling_p6       ROHC_PARAMS
    (
        (p_csc, msn,ack_typ),
        rohc_csc_tcp_t  *p_csc  _AND_
        rohc_U32bit_t   msn      _AND_
        rohc_U8bit_t    ack_typ
    )
{
    rohc_U32bit_t   loop_var = 0 ;
    rohc_U8bit_t    start_index = p_csc->rohc_gsw.start_index ;
    rohc_U8bit_t    end_index = p_csc->rohc_gsw.end_index ;
    rohc_U32bit_t   sn, sn_scaled, ack, ack_scaled = 0;
    
    ROHC_ENTER_FN("rohc_ack_handling_p6");
    loop_var = (start_index + ROHC_MAX_LENGTH_GSW_PROF6-1)%
        ROHC_MAX_LENGTH_GSW_PROF6; 
    do  
    {
        loop_var=(loop_var+1)%ROHC_MAX_LENGTH_GSW_PROF6;
        if(p_csc->rohc_gsw.rohc_gsw_elem[loop_var].msn == msn)
        {   
           if(ack_typ == ROHC_ACK)
           {  
            /* + Coverity 72921*/
            //msn = p_csc->rohc_gsw.rohc_gsw_elem[loop_var].msn;
            /* - Coverity 72921*/
            sn = p_csc->rohc_gsw.rohc_gsw_elem[loop_var].sn;
            sn_scaled = p_csc->rohc_gsw.rohc_gsw_elem[loop_var].sn_scaled;
            ack = p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ack;
            ack_scaled = p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ack_scaled;
            
            /* Here we need to traverse complete window and update min and
               the max value */
            
            p_csc->rohc_gsw.start_index=loop_var;
            loop_var = (start_index+ROHC_MAX_LENGTH_GSW_PROF6-1)%ROHC_MAX_LENGTH_GSW_PROF6;
            do
            {
                loop_var =(loop_var +1)%ROHC_MAX_LENGTH_GSW_PROF6;
             
                p_csc->rohc_gsw.min_sn = 
                (((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].sn)< sn)? 
                        (p_csc->rohc_gsw.rohc_gsw_elem[loop_var].sn) : sn);
                p_csc->rohc_gsw.max_sn = 
                (((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].sn) < sn)? 
                        sn : (p_csc->rohc_gsw.rohc_gsw_elem[loop_var].sn));
                
                p_csc->rohc_gsw.min_sn_scaled = 
                (((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].sn_scaled) < 
                        sn_scaled)?
                        (p_csc->rohc_gsw.rohc_gsw_elem[loop_var].sn_scaled) :
                        sn_scaled);

                p_csc->rohc_gsw.max_sn_scaled = 
                    (((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].sn_scaled)
                      <sn_scaled)? 
                 sn_scaled:(p_csc->rohc_gsw.rohc_gsw_elem[loop_var].sn_scaled));
                
                p_csc->rohc_gsw.min_ack = 
                (((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ack)< ack)? 
                        (p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ack) : ack);
                p_csc->rohc_gsw.max_ack = 
                (((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ack) < ack)? 
                        ack : (p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ack));
                
                p_csc->rohc_gsw.min_ack_scaled = 
                (((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ack_scaled) < 
                        ack_scaled)?
                        (p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ack_scaled) :
                        ack_scaled);

                p_csc->rohc_gsw.max_ack_scaled = 
                    (((p_csc->rohc_gsw.rohc_gsw_elem[loop_var].ack_scaled)
                      <ack_scaled)? 
                 ack_scaled:(p_csc->rohc_gsw.rohc_gsw_elem[loop_var]
                     .ack_scaled));

        
            }while(loop_var != end_index);
           }        
           ROHC_EXIT_FN("rohc_ack_handling_p6");
           return ROHC_SUCCESS;
        }   

    }while(loop_var!=end_index);
        
    ROHC_EXIT_FN("rohc_ack_handling_p6");
    /* MSN does exist in complete window */
    return ROHC_FAILURE ;
}

/*****************************************************************************
 * Function Name  : rohc_decode_p6_src
 * Inputs         : p_rohc_entity- pointer to ROHC entity
 *                  p_dsc - pointer to decompressor context
 *                  *p_en_val - structure containing the compressed  sn_value
 *                       and no. of LSBs it is compressed in ( i.e.. K).
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function decodes the src value using W-LSB decoding
 *                  mechanism.
 *****************************************************************************/
rohc_return_t rohc_decode_p6_src
    (
        rohc_entity_t           *p_rohc_entity,
        rohc_dsc_tcp_t          *p_dsc,
        rohc_encoded_value_t    *p_en_val,
        rohc_U16bit_t           *p_dec_src,
        rohc_error_code_t       *p_ecode
    )
{
    rohc_U16bit_t   p;
    register rohc_U16bit_t   new_v;
    register rohc_U16bit_t  min_v;
    register rohc_U16bit_t max_v;
    rohc_U16bit_t src_prev;
    rohc_U8bit_t bits;

    ROHC_ENTER_FN("rohc_decode_p6_src");

    /* To maintain the candidate set of last two received SRC values */
    /*p_dsc->src_prev_prev = p_dsc->src_prev ;*/
            
    /* This macro replaces the p_en_val->bits of p_en_val->value
     * to p_dsc->src_prev, and saves this value to new_v */
    p_dsc->src_prev = ROHC_HTONS(p_dsc->tcp->sport);
    src_prev = p_dsc->src_prev;
    bits = p_en_val->bits;
    ROHC_PUT_LSBS(new_v, src_prev, p_en_val->value, bits);

#ifdef ROHC_UT_FLAG
    fp = fopen("outfile", "a");
    fprintf(fp, "====================== DECODED SRC ========================\n");
    fprintf(fp, "V_REF:[%d]\nENCODED_VAL:[%d]\nK:[%d]\nNEW_V:[%d]\n",
                p_dsc->src_prev, p_en_val->value, p_en_val->bits, new_v);
#endif
    
    /* k = p_en_val->bits */
    p = 8;
    
    /* To find the min. and max. value of interpretation interval */
    min_v = (rohc_U16bit_t)((rohc_U16bit_t)src_prev - p);
    max_v = (rohc_U16bit_t)((rohc_U16bit_t)src_prev +
                            (ROHC_POWER_OF_2(bits) - 1) - p);
    
#ifdef ROHC_UT_FLAG
    fprintf(fp, "MIN_V:[%d]\nMAX_V:[%d]\n", min_v, max_v);
#endif
    
    /* If new value does not lies in the interpretation interval,
    * then discard the packet */  
    if(max_v <= min_v)
    {   
        if(!((new_v>=min_v) ||
                (new_v <= max_v) ))
        {
            return ROHC_FAILURE;
        }
    }

    *p_dec_src = new_v;

#ifdef ROHC_UT_FLAG
    fprintf(fp, "DECODED_SRC:[%d]\n", *p_dec_src);
    fClose(fp);
#endif

    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:\
                          ENCD_SRC[%u]:DECODED_SRC[%u]" \
                            "SN DECODING CALLED", \
                                p_rohc_entity->rohc_entity_id.ueIndex,
                                p_rohc_entity->rohc_entity_id.lcId,
                                ROHC_GET_DSC_CID(p_dsc),p_en_val->value, new_v);

    ROHC_EXIT_FN("rohc_decode_p6_src");

    return ROHC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : rohc_decode_p6_dst
 * Inputs         : p_rohc_entity- pointer to ROHC entity
 *                  p_dsc - pointer to decompressor context
 *                  *p_en_val - structure containing the compressed  sn_value
 *                       and no. of LSBs it is compressed in ( i.e.. K).
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function decodes the dst value using W-LSB decoding
 *                  mechanism.
 *****************************************************************************/
rohc_return_t rohc_decode_p6_dst
    (
        rohc_entity_t           *p_rohc_entity,
        rohc_dsc_tcp_t          *p_dsc,
        rohc_encoded_value_t    *p_en_val,
        rohc_U16bit_t           *p_dec_dst,
        rohc_error_code_t       *p_ecode
    )
{
    rohc_U16bit_t   p;
    register rohc_U16bit_t   new_v;
    register rohc_U16bit_t  min_v;
    register rohc_U16bit_t max_v;
    rohc_U16bit_t dst_prev;
    rohc_U8bit_t bits;

    ROHC_ENTER_FN("rohc_decode_p6_src");

    /* To maintain the candidate set of last two received SRC values */
    /*p_dsc->dst_prev_prev = p_dsc->dst_prev ;*/
            
    /* This macro replaces the p_en_val->bits of p_en_val->value
     * to p_dsc->dst_prev, and saves this value to new_v */
    p_dsc->dst_prev = ROHC_HTONS(p_dsc->tcp->dport);
    dst_prev = p_dsc->dst_prev;
    bits = p_en_val->bits;
    ROHC_PUT_LSBS(new_v, dst_prev, p_en_val->value, bits);

#ifdef ROHC_UT_FLAG
    fp = fopen("outfile", "a");
    fprintf(fp, "====================== DECODED DST ========================\n");
    fprintf(fp, "V_REF:[%d]\nENCODED_VAL:[%d]\nK:[%d]\nNEW_V:[%d]\n",
                p_dsc->dst_prev, p_en_val->value, p_en_val->bits, new_v);
#endif
    
    /* k = p_en_val->bits */
    p = 8;
    
    /* To find the min. and max. value of interpretation interval */
    min_v = (rohc_U16bit_t)((rohc_U16bit_t)dst_prev - p);
    max_v = (rohc_U16bit_t)((rohc_U16bit_t)dst_prev +
                            (ROHC_POWER_OF_2(bits) - 1) - p);
    
#ifdef ROHC_UT_FLAG
    fprintf(fp, "MIN_V:[%d]\nMAX_V:[%d]\n", min_v, max_v);
#endif
    
    /* If new value does not lies in the interpretation interval,
    * then discard the packet */  
    if(max_v <= min_v)
    {   
        if(!((new_v>=min_v) ||
                (new_v <= max_v) ))
        {
            return ROHC_FAILURE;
        }
    }

    *p_dec_dst = new_v;

#ifdef ROHC_UT_FLAG
    fprintf(fp, "DECODED_SRC:[%d]\n", *p_dec_dst);
    fClose(fp);
#endif

    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:\
            ENCD_SRC[%u]:DECODED_DST[%u]" \
                            "SN DECODING CALLED", \
                                p_rohc_entity->rohc_entity_id.ueIndex,
                                p_rohc_entity->rohc_entity_id.lcId,
                                ROHC_GET_DSC_CID(p_dsc),p_en_val->value, new_v);

    ROHC_EXIT_FN("rohc_decode_p6_dst");

    return ROHC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : rohcDecodeP6SN
 * Inputs         : p_rohc_entity- pointer to ROHC entity
 *                  p_dsc - pointer to decompressor context
 *                  *p_en_val - structure containing the compressed  sn_value
 *                       and no. of LSBs it is compressed in ( i.e.. K).
 *                  p_dec_sn - pointer to decoded SN value
 *                  p_ecode  - Error code pointer
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function decodes the SN value using W-LSB decoding
 *                  mechanism.
 *****************************************************************************/
rohc_return_t rohcDecodeP6SN
    (
        rohc_entity_t           *p_rohc_entity,
        rohc_dsc_tcp_t          *p_dsc,
        rohc_encoded_value_t    *p_en_val,
        rohc_U32bit_t           *p_dec_sn,
        rohc_error_code_t       *p_ecode,
        rohc_U32bit_t           interpVal
    )
{
    register rohc_U32bit_t   new_v;
    register rohc_U32bit_t  min_v;
    register rohc_U32bit_t max_v;
    rohc_U32bit_t sn_prev;
    rohc_U8bit_t bits;
    rohc_U8bit_t    adj_needed = ROHC_FALSE;
    rohc_U32bit_t   interval_size ;

    ROHC_ENTER_FN("rohcDecodeP6SN");

            
    /* This macro replaces the p_en_val->bits of p_en_val->value
     * to p_dsc->sn_prev, and saves this value to new_v */
    sn_prev = p_dsc->sn_prev;
    bits = p_en_val->bits;
    ROHC_PUT_LSBS(new_v, sn_prev, p_en_val->value, bits);

    /* k = p_en_val->bits */
    
    /* To find the min. and max. value of interpretation interval */
    min_v = (sn_prev - interpVal );
    max_v = (sn_prev + (ROHC_POWER_OF_2(bits) - 1) - interpVal);
    
   
    /* If new value lies in the interpretation interval,
    * then this is the decided value */  
    if(max_v >= min_v)
    {   
        if(!((new_v <= max_v) && (new_v >= min_v)))
        {
            adj_needed = ROHC_TRUE;
        } 
    }else
    {
        if(!((new_v>=min_v) ||
                (new_v <= max_v) ))
        {
            adj_needed = ROHC_TRUE;
        }
    }

    if(adj_needed) 
    {
        /* As per RFC 3095, Section 5.3.2.2.2,
         * If new_v is not within this interval, an adjustment is
         * applied by adding N x interval_size to the reconstructed value so
         * that the result is brought within the interpretation interval. 
         * Note that N can be negative. */
        /* If new_v < min_v, N is (+)ve, and if new_v > max_v, N is (-)ve. */
        interval_size = ROHC_POWER_OF_2(bits);
        do
        {
            adj_needed = ROHC_FALSE;
            new_v = new_v + interval_size; 
            if(max_v >= min_v)
            {   
                if(!((new_v <= max_v) && (new_v >= min_v)))
                {
                    adj_needed = ROHC_TRUE;
                } 
            }else
            {
                if(!((new_v>=min_v ) ||
                (new_v <= max_v) ))
                {
                    adj_needed = ROHC_TRUE;
                }
            }
        } while (adj_needed);
    }
 
    *p_dec_sn = new_v;


    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:\
                          ENCD_SN[%u]:DECODED_SN[%u]" \
                            "SN DECODING CALLED", \
                                p_rohc_entity->rohc_entity_id.ueIndex,
                                p_rohc_entity->rohc_entity_id.lcId,
                                ROHC_GET_DSC_CID(p_dsc),p_en_val->value, new_v);

    ROHC_EXIT_FN("rohcDecodeP6SN");

    return ROHC_SUCCESS;
}



/*****************************************************************************
 * Function Name  : rohcDecodeP6SNScaled
 * Inputs         : p_rohc_entity- pointer to ROHC entity
 *                  p_dsc - pointer to decompressor context
 *                  *p_en_val - structure containing the compressed  sn_value
 *                       and no. of LSBs it is compressed in ( i.e.. K).
 *                  p_dec_sn - pointer to decoded SN value
 *                  p_ecode  - Error code pointer
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function decodes the SN value using W-LSB decoding
 *                  mechanism.
 *****************************************************************************/
rohc_return_t rohcDecodeP6SNScaled
    (
        rohc_entity_t           *p_rohc_entity,
        rohc_dsc_tcp_t          *p_dsc,
        rohc_encoded_value_t    *p_en_val,
        rohc_U32bit_t           *p_dec_sn,
        rohc_error_code_t       *p_ecode
    )
{
    rohc_U16bit_t   p = 7;
    register rohc_U32bit_t   new_v;
    register rohc_U32bit_t  min_v;
    register rohc_U32bit_t max_v;
    rohc_U32bit_t sn_prev;
    rohc_U8bit_t bits;
    rohc_U8bit_t    adj_needed = ROHC_FALSE;
    rohc_U32bit_t   interval_size ;

    ROHC_ENTER_FN("rohcDecodeP6SN");

    /* To maintain the candidate set of last two received SN values */
    /*p_dsc->sn_prev_prev = p_dsc->sn_prev ;*/
            
    /* This macro replaces the p_en_val->bits of p_en_val->value
     * to p_dsc->sn_prev, and saves this value to new_v */
    sn_prev = p_dsc->sn_prev/p_dsc->dpib->payload_len;
    bits = p_en_val->bits;
    ROHC_PUT_LSBS(new_v, sn_prev, p_en_val->value, bits);

    
    
    /* To find the min. and max. value of interpretation interval */
    min_v = (sn_prev - p);
    max_v = (sn_prev + (ROHC_POWER_OF_2(bits) - 1) - p);
   

    /* If new value lies in the interpretation interval,
    * then this is the decided value */  
    if(max_v >= min_v)
    {   
        if(!((new_v <= max_v) && (new_v >= min_v)))
        {
            adj_needed = ROHC_TRUE;
        } 
    }else
    {
        if(!((new_v>=min_v) ||
                (new_v <= max_v) ))
        {
            adj_needed = ROHC_TRUE;
        }
    }

    if(adj_needed) 
    {
        /* As per RFC 3095, Section 5.3.2.2.2,
         * If new_v is not within this interval, an adjustment is
         * applied by adding N x interval_size to the reconstructed value so
         * that the result is brought within the interpretation interval. 
         * Note that N can be negative. */
        /* If new_v < min_v, N is (+)ve, and if new_v > max_v, N is (-)ve. */
        interval_size = ROHC_POWER_OF_2(bits);
        do
        {
            adj_needed = ROHC_FALSE;
            new_v = new_v + interval_size; 
            if(max_v >= min_v)
            {   
                if(!((new_v <= max_v) && (new_v >= min_v)))
                {
                    adj_needed = ROHC_TRUE;
                } 
            }else
            {
                if(!((new_v>=min_v ) ||
                (new_v <= max_v) ))
                {
                    adj_needed = ROHC_TRUE;
                }
            }
        } while (adj_needed);
    }

            
    *p_dec_sn = new_v*p_dsc->dpib->payload_len + 
        (p_dsc->sn_prev%p_dsc->dpib->payload_len);

    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:\
            ENCD_SN[%u]:DECODED_SN[%u]" \
                            "SN DECODING CALLED", \
                                p_rohc_entity->rohc_entity_id.ueIndex,
                                p_rohc_entity->rohc_entity_id.lcId,
                                ROHC_GET_DSC_CID(p_dsc),p_en_val->value, new_v);

    ROHC_EXIT_FN("rohcDecodeP6SN");

    return ROHC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : rohcDecodeP6TTL
 * Inputs         : p_rohc_entity- pointer to ROHC entity
 *                  p_dsc - pointer to decompressor context
 *                  *p_en_val - structure containing the compressed msn_value
 *                   and no. of LSBs it is compressed in ( i.e.. K).
 *                  p_dec_msn - pointer to decoded MSN value
 *                  p_ecode  - Error code pointer
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function decodes the TTL value using W-LSB decoding
 *                  mechanism. 
 *****************************************************************************/
rohc_return_t rohcDecodeP6TTL
    (
        rohc_entity_t           *p_rohc_entity,
        rohc_dsc_tcp_t          *p_dsc,
        rohc_encoded_value_t    *p_en_val,
        rohc_U8bit_t           *p_dec_ttl,
        rohc_error_code_t       *p_ecode
    )
{
    register rohc_U8bit_t   new_v;
    register rohc_U8bit_t  min_v;
    register rohc_U8bit_t max_v;
    rohc_U8bit_t ttl_prev;
    rohc_U8bit_t bits;
    rohc_U8bit_t    adj_needed = ROHC_FALSE;
    rohc_U32bit_t   interval_size ;

    ROHC_ENTER_FN("rohcDecodeP6TTL");

    /* To maintain the candidate set of last two received TTL values */
            
    /* This macro replaces the p_en_val->bits of p_en_val->value
     * to p_dsc->ttl_prev, and saves this value to new_v */
    ttl_prev = p_dsc->ttl_prev;
    bits = p_en_val->bits;
    ROHC_PUT_LSBS(new_v, ttl_prev, p_en_val->value, bits);

    
    /* To find the min. and max. value of interpretation interval */
    min_v = ttl_prev - 3;
    max_v = ttl_prev + (ROHC_POWER_OF_2(bits) - 1) - 3;
    
      /* If new value lies in the interpretation interval,
    * then this is the decided value */  
    if(max_v >= min_v)
    {   
        if(!((new_v <= max_v) && (new_v >= min_v)))
        {
            adj_needed = ROHC_TRUE;
        } 
    }else
    {
        if(!((new_v>=min_v) ||
                (new_v <= max_v) ))
        {
            adj_needed = ROHC_TRUE;
        }
    }

    if(adj_needed) 
    {
        /* As per RFC 3095, Section 5.3.2.2.2,
         * If new_v is not within this interval, an adjustment is
         * applied by adding N x interval_size to the reconstructed value so
         * that the result is brought within the interpretation interval. 
         * Note that N can be negative. */
        /* If new_v < min_v, N is (+)ve, and if new_v > max_v, N is (-)ve. */
        interval_size = ROHC_POWER_OF_2(bits);
        do
        {
            adj_needed = ROHC_FALSE;
            new_v = new_v + interval_size; 
            if(max_v >= min_v)
            {   
                if(!((new_v <= max_v) && (new_v >= min_v)))
                {
                    adj_needed = ROHC_TRUE;
                } 
            }else
            {
                if(!((new_v>=min_v ) ||
                (new_v <= max_v) ))
                {
                    adj_needed = ROHC_TRUE;
                }
            }
        } while (adj_needed);
    }

    
    *p_dec_ttl = new_v;


    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:\
                          ENCD_MSN[%u]:DECODED_MSN[%u]" \
                            "MSN DECODING CALLED", \
                                p_rohc_entity->rohc_entity_id.ueIndex,
                                p_rohc_entity->rohc_entity_id.lcId,
                                ROHC_GET_DSC_CID(p_dsc),p_en_val->value, new_v);

    ROHC_EXIT_FN("rohcDecodeP6TTL");

    return ROHC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : rohcDecodeP6MSN
 * Inputs         : p_rohc_entity- pointer to ROHC entity
 *                  p_dsc - pointer to decompressor context
 *                  *p_en_val - structure containing the compressed msn_value
 *                   and no. of LSBs it is compressed in ( i.e.. K).
 *                  p_dec_msn - pointer to decoded MSN value
 *                  p_ecode  - Error code pointer
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function decodes the MSN value using W-LSB decoding
 *                  mechanism. 
 *****************************************************************************/
rohc_return_t rohcDecodeP6MSN
    (
        rohc_entity_t           *p_rohc_entity,
        rohc_dsc_tcp_t          *p_dsc,
        rohc_encoded_value_t    *p_en_val,
        rohc_U32bit_t           *p_dec_msn,
        rohc_error_code_t       *p_ecode
    )
{
    register rohc_U16bit_t   new_v;
    register rohc_U16bit_t  min_v;
    register rohc_U16bit_t max_v;
    rohc_U32bit_t msn_prev;
    rohc_U8bit_t bits;
    rohc_U8bit_t    adj_needed = ROHC_FALSE;
    rohc_U32bit_t   interval_size ;

    ROHC_ENTER_FN("rohcDecodeP6MSN");

    /* To maintain the candidate set of last two received SN values */
            
    /* This macro replaces the p_en_val->bits of p_en_val->value
     * to p_dsc->msn_prev, and saves this value to new_v */
    msn_prev = p_dsc->msn_prev;
    bits = p_en_val->bits;
    ROHC_PUT_LSBS(new_v, msn_prev, p_en_val->value, bits);

    
    /* To find the min. and max. value of interpretation interval */
    min_v = (rohc_U16bit_t)((rohc_U16bit_t)msn_prev - 4);
    max_v = (rohc_U16bit_t)((rohc_U16bit_t)msn_prev +
                            (ROHC_POWER_OF_2(bits) - 1) - 4);
    
      /* If new value lies in the interpretation interval,
    * then this is the decided value */  
    if(max_v >= min_v)
    {   
        if(!((new_v <= max_v) && (new_v >= min_v)))
        {
            adj_needed = ROHC_TRUE;
        } 
    }else
    {
        if(!((new_v>=min_v) ||
                (new_v <= max_v) ))
        {
            adj_needed = ROHC_TRUE;
        }
    }

    if(adj_needed) 
    {
        /* As per RFC 3095, Section 5.3.2.2.2,
         * If new_v is not within this interval, an adjustment is
         * applied by adding N x interval_size to the reconstructed value so
         * that the result is brought within the interpretation interval. 
         * Note that N can be negative. */
        /* If new_v < min_v, N is (+)ve, and if new_v > max_v, N is (-)ve. */
        interval_size = ROHC_POWER_OF_2(bits);
        do
        {
            adj_needed = ROHC_FALSE;
            new_v = new_v + interval_size; 
            if(max_v >= min_v)
            {   
                if(!((new_v <= max_v) && (new_v >= min_v)))
                {
                    adj_needed = ROHC_TRUE;
                } 
            }else
            {
                if(!((new_v>=min_v ) ||
                (new_v <= max_v) ))
                {
                    adj_needed = ROHC_TRUE;
                }
            }
        } while (adj_needed);
    }

    
    *p_dec_msn = new_v;


    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:\
            ENCD_MSN[%u]:DECODED_MSN[%u]" \
                            "MSN DECODING CALLED", \
                                p_rohc_entity->rohc_entity_id.ueIndex,
                                p_rohc_entity->rohc_entity_id.lcId,
                                ROHC_GET_DSC_CID(p_dsc),p_en_val->value, new_v);

    ROHC_EXIT_FN("rohcDecodeP6MSN");
    return ROHC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : rohcDecodeP6Win
 * Inputs         : p_rohc_entity- pointer to ROHC entity
 *                  p_dsc - pointer to decompressor context
 *                  *p_en_val - structure containing the compressed msn_value
 *                   and no. of LSBs it is compressed in ( i.e.. K).
 *                  p_dec_msn - pointer to decoded MSN value
 *                  p_ecode  - Error code pointer
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function decodes the MSN value using W-LSB decoding
 *                  mechanism. 
 *****************************************************************************/
rohc_return_t rohcDecodeP6Win
    (
        rohc_entity_t           *p_rohc_entity,
        rohc_dsc_tcp_t          *p_dsc,
        rohc_encoded_value_t    *p_en_val,
        rohc_U16bit_t           *p_dec_win,
        rohc_error_code_t       *p_ecode
    )
{
    register rohc_U16bit_t   new_v;
    register rohc_U16bit_t  min_v;
    register rohc_U16bit_t max_v;
    rohc_U16bit_t win_prev;
    rohc_U8bit_t bits;
    rohc_U8bit_t    adj_needed = ROHC_FALSE;
    rohc_U32bit_t   interval_size ;

    ROHC_ENTER_FN("rohcDecodeP6Win");

    /* To maintain the candidate set of last two received SN values */
            
    /* This macro replaces the p_en_val->bits of p_en_val->value
     * to p_dsc->msn_prev, and saves this value to new_v */
    win_prev = p_dsc->win_prev;
    bits = p_en_val->bits;
    ROHC_PUT_LSBS(new_v, win_prev, p_en_val->value, bits);

    
    /* To find the min. and max. value of interpretation interval */
    min_v = win_prev - 16383;
    max_v = win_prev + (ROHC_POWER_OF_2(bits) - 1) - 16383;
    
      /* If new value lies in the interpretation interval,
    * then this is the decided value */  
    if(max_v >= min_v)
    {   
        if(!((new_v <= max_v) && (new_v >= min_v)))
        {
            adj_needed = ROHC_TRUE;
        } 
    }else
    {
        if(!((new_v>=min_v) ||
                (new_v <= max_v) ))
        {
            adj_needed = ROHC_TRUE;
        }
    }

    if(adj_needed) 
    {
        /* As per RFC 3095, Section 5.3.2.2.2,
         * If new_v is not within this interval, an adjustment is
         * applied by adding N x interval_size to the reconstructed value so
         * that the result is brought within the interpretation interval. 
         * Note that N can be negative. */
        /* If new_v < min_v, N is (+)ve, and if new_v > max_v, N is (-)ve. */
        interval_size = ROHC_POWER_OF_2(bits);
        do
        {
            adj_needed = ROHC_FALSE;
            new_v = new_v + interval_size; 
            if(max_v >= min_v)
            {   
                if(!((new_v <= max_v) && (new_v >= min_v)))
                {
                    adj_needed = ROHC_TRUE;
                } 
            }else
            {
                if(!((new_v>=min_v ) ||
                (new_v <= max_v) ))
                {
                    adj_needed = ROHC_TRUE;
                }
            }
        } while (adj_needed);
    }

    
    *p_dec_win = new_v;


    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:\
            ENCD_MSN[%u]:DECODED_MSN[%u]" \
                            "Window DECODING CALLED", \
                                p_rohc_entity->rohc_entity_id.ueIndex,
                                p_rohc_entity->rohc_entity_id.lcId,
                                ROHC_GET_DSC_CID(p_dsc),p_en_val->value, new_v);

    ROHC_EXIT_FN("rohcDecodeP6Win");

    return ROHC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : rohcDecodeP6AckNum
 * Inputs         : p_rohc_entity- pointer to ROHC entity
 *                  p_dsc - pointer to decompressor context
 *                  *p_en_val - structure containing the compressed  ack_num_value
 *                       and no. of LSBs it is compressed in ( i.e.. K).
 *                  p_dec_ack_num - pointer to decoded ACK_NUM value
 *                  p_ecode  - Error code pointer
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function decodes the SN value using W-LSB decoding
 *                  mechanism.
 *****************************************************************************/
rohc_return_t rohcDecodeP6AckNum
    (
        rohc_entity_t           *p_rohc_entity,
        rohc_dsc_tcp_t          *p_dsc,
        rohc_encoded_value_t    *p_en_val,
        rohc_U32bit_t           *p_dec_ack_num,
        rohc_error_code_t       *p_ecode,
        rohc_U32bit_t           interpVal
    )
{
    register rohc_U32bit_t   new_v;
    register rohc_U32bit_t  min_v;
    register rohc_U32bit_t max_v;
    rohc_U32bit_t ack_recvd_prev;
    rohc_U8bit_t bits;
    rohc_U8bit_t    adj_needed = ROHC_FALSE;
    rohc_U32bit_t   interval_size ;

    ROHC_ENTER_FN("rohcDecodeP6AckNum");

    /* To maintain the candidate set of last two received SN values */
    /*p_dsc->sn_prev_prev = p_dsc->sn_prev ;*/
            
    /* This macro replaces the p_en_val->bits of p_en_val->value
     * to p_dsc->sn_prev, and saves this value to new_v */
    ack_recvd_prev = p_dsc->ack_recvd_prev;
    bits = p_en_val->bits;
    ROHC_PUT_LSBS(new_v, ack_recvd_prev, p_en_val->value, bits);

    
    /* To find the min. and max. value of interpretation interval */
    min_v = (ack_recvd_prev - interpVal );
    max_v = (ack_recvd_prev + (ROHC_POWER_OF_2(bits) - 1) - interpVal );
    
   
     /* If new value lies in the interpretation interval,
    * then this is the decided value */  
    if(max_v >= min_v)
    {   
        if(!((new_v <= max_v) && (new_v >= min_v)))
        {
            adj_needed = ROHC_TRUE;
        } 
    }else
    {
        if(!((new_v>=min_v) ||
                (new_v <= max_v) ))
        {
            adj_needed = ROHC_TRUE;
        }
    }

    if(adj_needed) 
    {
        /* As per RFC 3095, Section 5.3.2.2.2,
         * If new_v is not within this interval, an adjustment is
         * applied by adding N x interval_size to the reconstructed value so
         * that the result is brought within the interpretation interval. 
         * Note that N can be negative. */
        /* If new_v < min_v, N is (+)ve, and if new_v > max_v, N is (-)ve. */
        interval_size = ROHC_POWER_OF_2(bits);
        do
        {
            adj_needed = ROHC_FALSE;
            new_v = new_v + interval_size; 
            if(max_v >= min_v)
            {   
                if(!((new_v <= max_v) && (new_v >= min_v)))
                {
                    adj_needed = ROHC_TRUE;
                } 
            }else
            {
                if(!((new_v>=min_v ) ||
                (new_v <= max_v) ))
                {
                    adj_needed = ROHC_TRUE;
                }
            }
        } while (adj_needed);
    }
    *p_dec_ack_num = new_v; 
    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:\
                          ENCD_ACK_NUM[%u]:DECODED_ACK_NUM[%u]" \
                            "SN DECODING CALLED", \
                                p_rohc_entity->rohc_entity_id.ueIndex,
                                p_rohc_entity->rohc_entity_id.lcId,
                                ROHC_GET_DSC_CID(p_dsc),p_en_val->value, new_v);

    ROHC_EXIT_FN("rohcDecodeP6AckNum");

    return ROHC_SUCCESS;
}


/*****************************************************************************
 * Function Name  : rohcDecodeP6AckNumScaled
 * Inputs         : p_rohc_entity- pointer to ROHC entity
 *                  p_dsc - pointer to decompressor context
 *                  *p_en_val - structure containing the compressed  ack_num_value
 *                       and no. of LSBs it is compressed in ( i.e.. K).
 *                  p_dec_ack_num - pointer to decoded ACK_NUM value
 *                  p_ecode  - Error code pointer
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function decodes the SN value using W-LSB decoding
 *                  mechanism.
 *****************************************************************************/
rohc_return_t rohcDecodeP6AckNumScaled
    (
        rohc_entity_t           *p_rohc_entity,
        rohc_dsc_tcp_t          *p_dsc,
        rohc_encoded_value_t    *p_en_val,
        rohc_U32bit_t           *p_dec_ack_num,
        rohc_error_code_t       *p_ecode
    )
{
    register rohc_U32bit_t   new_v;
    register rohc_U32bit_t  min_v;
    register rohc_U32bit_t max_v;
    rohc_U32bit_t ack_recvd_offset, ack_recvd_prev;
    rohc_U8bit_t bits;
    rohc_U8bit_t    adj_needed = ROHC_FALSE;
    rohc_U32bit_t   interval_size ;

    ROHC_ENTER_FN("rohcDecodeP6AckNum");

    /* To maintain the candidate set of last two received SN values */
    /*p_dsc->sn_prev_prev = p_dsc->sn_prev ;*/
            
    /* This macro replaces the p_en_val->bits of p_en_val->value
     * to p_dsc->sn_prev, and saves this value to new_v */
    ack_recvd_offset = p_dsc->ack_recvd_prev % p_dsc->ackstride;
    ack_recvd_prev = p_dsc->ack_recvd_prev/p_dsc->ackstride;
    bits = p_en_val->bits;
    ROHC_PUT_LSBS(new_v, ack_recvd_prev, p_en_val->value, bits);

    
    /* To find the min. and max. value of interpretation interval */
    min_v = (ack_recvd_prev - 3);
    max_v = (ack_recvd_prev + (ROHC_POWER_OF_2(bits) - 1) - 3);
    
   
     /* If new value lies in the interpretation interval,
    * then this is the decided value */  
    if(max_v >= min_v)
    {   
        if(!((new_v <= max_v) && (new_v >= min_v)))
        {
            adj_needed = ROHC_TRUE;
        } 
    }else
    {
        if(!((new_v>=min_v) ||
                (new_v <= max_v) ))
        {
            adj_needed = ROHC_TRUE;
        }
    }

    if(adj_needed) 
    {
        /* As per RFC 3095, Section 5.3.2.2.2,
         * If new_v is not within this interval, an adjustment is
         * applied by adding N x interval_size to the reconstructed value so
         * that the result is brought within the interpretation interval. 
         * Note that N can be negative. */
        /* If new_v < min_v, N is (+)ve, and if new_v > max_v, N is (-)ve. */
        interval_size = ROHC_POWER_OF_2(bits);
        do
        {
            adj_needed = ROHC_FALSE;
            new_v = new_v + interval_size; 
            if(max_v >= min_v)
            {   
                if(!((new_v <= max_v) && (new_v >= min_v)))
                {
                    adj_needed = ROHC_TRUE;
                } 
            }else
            {
                if(!((new_v>=min_v ) ||
                (new_v <= max_v) ))
                {
                    adj_needed = ROHC_TRUE;
                }
            }
        } while (adj_needed);
    }
    *p_dec_ack_num = new_v*p_dsc->ackstride + ack_recvd_offset;

    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:\
            ENCD_ACK_NUM[%u]:DECODED_ACK_NUM[%u]" \
                            "SN DECODING CALLED", \
                                p_rohc_entity->rohc_entity_id.ueIndex,
                                p_rohc_entity->rohc_entity_id.lcId,
                                ROHC_GET_DSC_CID(p_dsc),p_en_val->value, new_v);

    ROHC_EXIT_FN("rohcDecodeP6AckNum");

    return ROHC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : rohcDecodeP6TSEchoVal
 * Inputs         : p_rohc_entity- pointer to ROHC entity
 *                  p_dsc - pointer to decompressor context
 *                  *p_en_val - structure containing the compressed  ts_value
 *                       and no. of LSBs it is compressed in ( i.e.. K).
 *                  p_dec_ts - pointer to decoded TS value
 *                  p_ecode  - Error code pointer
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function decodes the TS value using W-LSB decoding
 *                  mechanism.
 *****************************************************************************/
rohc_return_t rohcDecodeP6TSEchoVal
    (
        rohc_entity_t           *p_rohc_entity,
        rohc_dsc_tcp_t          *p_dsc,
        rohc_encoded_value_t    *p_en_val,
        rohc_U32bit_t           *p_dec_ts,
        rohc_error_code_t       *p_ecode
    )
{
    rohc_S32bit_t   p = 0;
    register rohc_U32bit_t   new_v;
    register rohc_U32bit_t  min_v;
    register rohc_U32bit_t max_v;
    rohc_U32bit_t preTsEchoVal;
    rohc_U8bit_t bits;
    rohc_U8bit_t    adj_needed = ROHC_FALSE;
    rohc_U32bit_t   interval_size ;

    ROHC_ENTER_FN("rohcDecodeP6TSEchoVal");

    /* To maintain the candidate set of last two received TS values */
    /*p_dsc->ts_prev_prev = p_dsc->ts_prev ;*/
            
    /* This macro replaces the p_en_val->bits of p_en_val->value
     * to p_dsc->ts_prev, and saves this value to new_v */
    preTsEchoVal = p_dsc->tsEchoVal;
    bits = p_en_val->bits;
    ROHC_PUT_LSBS(new_v, preTsEchoVal , p_en_val->value, bits);

    
    /* k = p_en_val->bits */
    if(( 7 == p_en_val->bits) || (14 == p_en_val->bits))
        p = -1;
    else	
    {
	    if (21 == p_en_val->bits)
	    {
		    p = 0x40000;
	    }
	    else
	    {
		    if (29 == p_en_val->bits)
			    p = 0x4000000;
	    }
    }    
    
    /* To find the min. and max. value of interpretation interval */
    min_v = preTsEchoVal- p;
    max_v = preTsEchoVal + (ROHC_POWER_OF_2(bits) - 1) - p;
    
         /* If new value lies in the interpretation interval,
    * then this is the decided value */  
    if(max_v >= min_v)
    {   
        if(!((new_v <= max_v) && (new_v >= min_v)))
        {
            adj_needed = ROHC_TRUE;
        } 
    }else
    {
        if(!((new_v>=min_v) ||
                (new_v <= max_v) ))
        {
            adj_needed = ROHC_TRUE;
        }
    }

    if(adj_needed) 
    {
        /* As per RFC 3095, Section 5.3.2.2.2,
         * If new_v is not within this interval, an adjustment is
         * applied by adding N x interval_size to the reconstructed value so
         * that the result is brought within the interpretation interval. 
         * Note that N can be negative. */
        /* If new_v < min_v, N is (+)ve, and if new_v > max_v, N is (-)ve. */
        interval_size = ROHC_POWER_OF_2(bits);
        do
        {
            adj_needed = ROHC_FALSE;
            new_v = new_v + interval_size; 
            if(max_v >= min_v)
            {   
                if(!((new_v <= max_v) && (new_v >= min_v)))
                {
                    adj_needed = ROHC_TRUE;
                } 
            }else
            {
                if(!((new_v>=min_v ) ||
                (new_v <= max_v) ))
                {
                    adj_needed = ROHC_TRUE;
                }
            }
        } while (adj_needed);
    }
 
  
    *p_dec_ts = new_v;

#ifdef ROHC_UT_FLAG
    fprintf(fp, "DECODED_TS:[%d]\n", *p_dec_ts);
    fClose(fp);
#endif

    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:\
            ENCD_TS[%u]:DECODED_ACK_NUM[%u]" \
            "TS DECODING CALLED", \
            p_rohc_entity->rohc_entity_id.ueIndex,
            p_rohc_entity->rohc_entity_id.lcId,
                                ROHC_GET_DSC_CID(p_dsc),p_en_val->value, new_v);

    ROHC_EXIT_FN("rohcDecodeP6TSEchoVal");

    return ROHC_SUCCESS;
}


/*****************************************************************************
 * Function Name  : rohcDecodeP6TS
 * Inputs         : p_rohc_entity- pointer to ROHC entity
 *                  p_dsc - pointer to decompressor context
 *                  *p_en_val - structure containing the compressed  ts_value
 *                       and no. of LSBs it is compressed in ( i.e.. K).
 *                  p_dec_ts - pointer to decoded TS value
 *                  p_ecode  - Error code pointer
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function decodes the TS value using W-LSB decoding
 *                  mechanism.
 *****************************************************************************/
rohc_return_t rohcDecodeP6TS
    (
        rohc_entity_t           *p_rohc_entity,
        rohc_dsc_tcp_t          *p_dsc,
        rohc_encoded_value_t    *p_en_val,
        rohc_U32bit_t           *p_dec_ts,
        rohc_error_code_t       *p_ecode
    )
{
    rohc_S32bit_t   p = 0;
    register rohc_U32bit_t   new_v;
    register rohc_U32bit_t  min_v;
    register rohc_U32bit_t max_v;
    rohc_U32bit_t pre_ts;
    rohc_U8bit_t bits;
    rohc_U8bit_t    adj_needed = ROHC_FALSE;
    rohc_U32bit_t   interval_size ;

    ROHC_ENTER_FN("rohcDecodeP6TS");

    /* To maintain the candidate set of last two received TS values */
    /*p_dsc->ts_prev_prev = p_dsc->ts_prev ;*/
            
    /* This macro replaces the p_en_val->bits of p_en_val->value
     * to p_dsc->ts_prev, and saves this value to new_v */
    pre_ts = p_dsc->pre_ts;
    bits = p_en_val->bits;
    ROHC_PUT_LSBS(new_v, pre_ts, p_en_val->value, bits);

    
    /* k = p_en_val->bits */
    if(( 7 == p_en_val->bits) || (14 == p_en_val->bits))
        p = -1;
    else	
    {
	    if (21 == p_en_val->bits)
	    {
		    p = 0x40000;
	    }
	    else
	    {
		    if (29 == p_en_val->bits)
			    p = 0x4000000;
	    }
    }    
    
    /* To find the min. and max. value of interpretation interval */
    min_v = pre_ts - p;
    max_v = pre_ts +(ROHC_POWER_OF_2(bits) - 1) - p;
    
    
             /* If new value lies in the interpretation interval,
    * then this is the decided value */  
    if(max_v >= min_v)
    {   
        if(!((new_v <= max_v) && (new_v >= min_v)))
        {
            adj_needed = ROHC_TRUE;
        } 
    }else
    {
        if(!((new_v>=min_v) ||
                (new_v <= max_v) ))
        {
            adj_needed = ROHC_TRUE;
        }
    }

    if(adj_needed) 
    {
        /* As per RFC 3095, Section 5.3.2.2.2,
         * If new_v is not within this interval, an adjustment is
         * applied by adding N x interval_size to the reconstructed value so
         * that the result is brought within the interpretation interval. 
         * Note that N can be negative. */
        /* If new_v < min_v, N is (+)ve, and if new_v > max_v, N is (-)ve. */
        interval_size = ROHC_POWER_OF_2(bits);
        do
        {
            adj_needed = ROHC_FALSE;
            new_v = new_v + interval_size; 
            if(max_v >= min_v)
            {   
                if(!((new_v <= max_v) && (new_v >= min_v)))
                {
                    adj_needed = ROHC_TRUE;
                } 
            }else
            {
                if(!((new_v>=min_v ) ||
                (new_v <= max_v) ))
                {
                    adj_needed = ROHC_TRUE;
                }
            }
        } while (adj_needed);
    }
    *p_dec_ts = new_v;


    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:\
                          ENCD_TS[%u]:DECODED_ACK_NUM[%u]" \
                            "TS DECODING CALLED", \
                                p_rohc_entity->rohc_entity_id.ueIndex,
                                p_rohc_entity->rohc_entity_id.lcId,
                                ROHC_GET_DSC_CID(p_dsc),p_en_val->value, new_v);

    ROHC_EXIT_FN("rohcDecodeP6TS");

    return ROHC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : rohcDecodeP6TSEchoVal
 * Inputs         : p_rohc_entity- pointer to ROHC entity
 *                  p_dsc - pointer to decompressor context
 *                  *p_en_val - structure containing the compressed  ts_value
 *                       and no. of LSBs it is compressed in ( i.e.. K).
 *                  p_dec_ts - pointer to decoded TS value
 *                  p_ecode  - Error code pointer
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function decodes the TS value using W-LSB decoding
 *                  mechanism.
 *****************************************************************************/
rohc_void_t rohcDecodeP6SackBlock
    (
        rohc_dsc_tcp_t          *p_dsc,
        rohc_encoded_value_t    *p_en_val,
        rohc_U32bit_t           *p_dec_ts,
        rohc_error_code_t       *p_ecode,
        rohc_U32bit_t           *pBaseVal
    )
{
    rohc_S32bit_t   p = -1;
    register rohc_U32bit_t   new_v;
    register rohc_U32bit_t  min_v;
    register rohc_U32bit_t max_v;
    rohc_U8bit_t bits;
    rohc_U8bit_t    adj_needed = ROHC_FALSE;
    rohc_U32bit_t   interval_size ;

    ROHC_ENTER_FN("rohcDecodeP6SackBlock");

    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,(ADDR)p_ecode,ROHC_GET_DSC_CID(p_dsc),0,0,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
    /* To maintain the candidate set of last two received TS values */
    /*p_dsc->ts_prev_prev = p_dsc->ts_prev ;*/
            
    /* This macro replaces the p_en_val->bits of p_en_val->value
     * to p_dsc->ts_prev, and saves this value to new_v */
    bits = p_en_val->bits;
    ROHC_PUT_LSBS(new_v, *pBaseVal, p_en_val->value, bits);

    
    /* k = p_en_val->bits */
    
    /* To find the min. and max. value of interpretation interval */
    min_v = (*pBaseVal - p);
    max_v = (*pBaseVal + (ROHC_POWER_OF_2(bits) - 1) - p);
    
    
               /* If new value lies in the interpretation interval,
    * then this is the decided value */  
    if(max_v >= min_v)
    {   
        if(!((new_v <= max_v) && (new_v >= min_v)))
        {
            adj_needed = ROHC_TRUE;
        } 
    }else
    {
        if(!((new_v>=min_v) ||
                (new_v <= max_v) ))
        {
            adj_needed = ROHC_TRUE;
        }
    }

    if(adj_needed) 
    {
        /* As per RFC 3095, Section 5.3.2.2.2,
         * If new_v is not within this interval, an adjustment is
         * applied by adding N x interval_size to the reconstructed value so
         * that the result is brought within the interpretation interval. 
         * Note that N can be negative. */
        /* If new_v < min_v, N is (+)ve, and if new_v > max_v, N is (-)ve. */
        interval_size = ROHC_POWER_OF_2(bits);
        do
        {
            adj_needed = ROHC_FALSE;
            new_v = new_v + interval_size; 
            if(max_v >= min_v)
            {   
                if(!((new_v <= max_v) && (new_v >= min_v)))
                {
                    adj_needed = ROHC_TRUE;
                } 
            }else
            {
                if(!((new_v>=min_v ) ||
                (new_v <= max_v) ))
                {
                    adj_needed = ROHC_TRUE;
                }
            }
        } while (adj_needed);
    }
 
    *p_dec_ts = *pBaseVal = new_v;
    
    ROHC_EXIT_FN("rohcDecodeP6TS");
}

/*****************************************************************************
 * Function Name  : rohcDecodeP6InnerIpId
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_dsc - pointer to decompressor context
 *                  *p_en_val - structure containing the compressed ip_id1 value
 *                        and no. of LSBs it is compressed in (i.e. k).
 *                  p_dec_ip_id - pointer to decoded inner IP-ID value
 *                  p_ecode  - Error code pointer
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function decodes the IP_ID value using W-LSB decoding
 *                  mechanism.
 *****************************************************************************/
rohc_return_t rohcDecodeP6InnerIpId
    (
        rohc_entity_t           *p_rohc_entity,
        rohc_dsc_tcp_t          *p_dsc,
        rohc_encoded_value_t    *p_en_val,
        rohc_U16bit_t           *p_dec_ip_id,
        rohc_error_code_t       *p_ecode
    )
{
    register rohc_U16bit_t   new_v;
    register rohc_U16bit_t  pre_offset;
    register rohc_U16bit_t min_v, max_v;
    rohc_U32bit_t msn_prev;
    rohc_U8bit_t bits, p;
    rohc_U8bit_t swap_temp_1;
    rohc_U8bit_t swap_temp_2;
    rohc_U8bit_t    adj_needed = ROHC_FALSE;
    rohc_U32bit_t   interval_size ;


    ROHC_ENTER_FN("rohcDecodeP6InnerIpId");

    /* The offset for the Previous Packet */
    msn_prev = p_dsc->msn_prev;
    bits = p_en_val->bits;
    pre_offset = (rohc_U16bit_t)((rohc_U16bit_t)p_dsc->pre_inner_ip_id - 
                (rohc_U16bit_t)msn_prev );

    if (IP_ID_BEHAVIOR_SEQUENTIAL_SWAPPED == p_dsc->ipid_beh_inner)
    {
        swap_temp_1 = (p_en_val->value << 16);
        swap_temp_2 = (p_en_val->value << 24);
        p_en_val->value = swap_temp_2;
        p_en_val->value = (p_en_val->value << 8) + swap_temp_1;
    }

    ROHC_PUT_LSBS(new_v, pre_offset, p_en_val->value, bits);

    /* k = p_en_val->bits */
    if(( 3 == bits))
    {
        p = 1;
    }
    else
    {
        p = 3;
    }

    min_v = (pre_offset - p); 
    max_v = (pre_offset + (ROHC_POWER_OF_2(p_en_val->bits) - 1) - p); 

    /* If new value lies in the interpretation interval,
    * then this is the decided value */  
    if(max_v >= min_v)
    {   
        if(!((new_v <= max_v) && (new_v >= min_v)))
        {
            adj_needed = ROHC_TRUE;
        } 
    }
    else
    {
        if(!((new_v>=min_v) ||
                (new_v <= max_v) ))
        {
            adj_needed = ROHC_TRUE;
        }
    }

    if (adj_needed)
    {
        /* As per RFC 3095, Section 5.3.2.2.2,
         * If new_v is not within this interval, an adjustment is
         * applied by adding N x interval_size to the reconstructed value so
         * that the result is brought within the interpretation interval. 
         * Note that N can be negative. */

        /* If new_v < min_v, N is (+)ve, and if new_v > max_v, N is (-)ve. */
        /*adjustment_factor_N = ((new_v > max_v ) ? -1 : 1) ;*/
        interval_size = ROHC_POWER_OF_2(bits);

        /* +- SPR 17777 */
/* SPR 20636 Changes Start */
        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
        /* +- SPR 17777 */
        ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:"\
                "ENCD_IP-ID[%u]: appling adujstment" \
                "because the received value is not"\
                " in the interpretation interval"\
                           " (%u,%u)\n",\
                           p_rohc_entity->rohc_entity_id.ueIndex,
                           p_rohc_entity->rohc_entity_id.lcId,
                           ROHC_GET_DSC_CID(p_dsc),
                           /* SPR 9591 fix start */
                           p_en_val->value,
                           /* SPR 9591 fix end */
                           pre_offset,new_v);

        
        do
        {
            new_v = new_v + interval_size; 
            adj_needed = ROHC_FALSE;
            if(max_v >= pre_offset)
            {   
                if(!((new_v <= max_v) && (new_v >= pre_offset)))
                {
                    adj_needed = ROHC_TRUE;
                } 
            }else
            {
                if(!((new_v>=pre_offset) ||
                        (new_v <= max_v) ))
                {
                    adj_needed = ROHC_TRUE;
                }
            }   
         } while (adj_needed);
    }
    
    *p_dec_ip_id = (rohc_U16bit_t)((rohc_U16bit_t)p_dsc->dpib->decoded_msn + 
                    (rohc_U16bit_t)new_v);
    /* END_SPR_577 */
    
#ifdef ROHC_UT_FLAG
    fprintf(fp, "DECODED INNER IP-ID:[%d]\n", *p_dec_ip_id);
    fClose(fp);
#endif

    ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:\
                                            ENCD_IP-ID[%u]:DECODED_IP-ID[%u]" \
                                            "IP-ID DECODING CALLED", \
                                            p_rohc_entity->rohc_entity_id.ueIndex,
                                            p_rohc_entity->rohc_entity_id.lcId, \
                                            ROHC_GET_DSC_CID(p_dsc),
                                            p_en_val->value, *p_dec_ip_id);

    ROHC_EXIT_FN("rohcDecodeP6InnerIpId");
            
    return ROHC_SUCCESS ;
}


/*****************************************************************************
 * Function Name  : rohcDecodeP6OuterIpId
 * Inputs         : p_rohc_entity - pointer to ROHC entity
 *                  p_dsc - pointer to decompressor context
 *                  *p_en_val - structure containing the compressed ip_id2 value
 *                  and no. of LSBs it is compressed in (i.e. k).
 *                  p_dec_ip_id - pointer to decoded outer IP-ID value
 *                  p_ecode  - Error code pointer
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function decodes the IP_ID value using W-LSB decoding
 *                  mechanism.
 *****************************************************************************/
#ifdef ROHC_FUNC_ENHANCEMENT  
rohc_return_t rohcDecodeP6OuterIpId
    (
        rohc_entity_t           *p_rohc_entity,
        rohc_dsc_tcp_t          *p_dsc,
        rohc_encoded_value_t    *p_en_val,
        rohc_U16bit_t           *p_dec_ip_id,
        rohc_error_code_t       *p_ecode

    )
{
    register rohc_U16bit_t   new_v;
    register rohc_U16bit_t  pre_offset;
    register rohc_U16bit_t max_v;
    rohc_U32bit_t msn_prev;
    rohc_U8bit_t bits;
    rohc_U8bit_t swap_temp_1;
    rohc_U8bit_t swap_temp_2;

    ROHC_ENTER_FN("rohcDecodeP6OuterIpId");
    
    /* The offset for the Previous Packet */
    msn_prev = p_dsc->msn_prev;
    bits = p_en_val->bits;
    pre_offset = (rohc_S32bit_t)ROHC_ABS(((rohc_U32bit_t)p_dsc->pre_outer_ip_id) - msn_prev);

    if (IP_ID_BEHAVIOR_SEQUENTIAL_SWAPPED == p_dsc->ipid_beh_inner)
    {
        swap_temp_1 = (p_en_val->value << 16);
        swap_temp_2 = (p_en_val->value << 24);
        p_en_val->value = swap_temp_2;
        p_en_val->value = (p_en_val->value << 8) + swap_temp_1;
    }

    ROHC_PUT_LSBS(new_v, pre_offset, p_en_val->value, bits);
    
#ifdef ROHC_UT_FLAG
    fp = fopen("outfile", "a");
    fprintf(fp, "================= DECODED OUTER IP-ID ===================\n");
    fprintf(fp, "V_REF:[%d]\nENCODED_VAL:[%d]\nK:[%d]\nNEW_V:[%d]\n", 
                  pre_offset, p_en_val->value, p_en_val->bits, new_v);
#endif

    /* k = p_en_val->bits */
        
    /* To find the min. and max. value of interpretation interval */
    /* p is zero for ip id */
    /* In case of IP-ID decoding, min_v is same as pre_offset */
    max_v = pre_offset + (ROHC_POWER_OF_2(p_en_val->bits) - 1); 

#ifdef ROHC_UT_FLAG
    fprintf(fp, "MIN_V:[%d]\nMAX_V:[%d]\n", pre_offset, max_v);
#endif

    /* If new value lies in the interpretation interval,
    * then this is the decided value */
    if(max_v <= pre_offset)
    {   
        if(!((new_v>=pre_offset) ||
                (new_v <= max_v) ))
        {
            return ROHC_FAILURE;
        }
    }
            
    
    if ((rohc_U32bit_t)p_dsc->pre_inner_ip_id < p_dsc->msn_prev) 
        *p_dec_ip_id = ((rohc_S32bit_t)p_dsc->dpib->decoded_msn) - new_v;
    else
        *p_dec_ip_id = ((rohc_S32bit_t)p_dsc->dpib->decoded_msn) + new_v;
    
#ifdef ROHC_UT_FLAG
        fprintf(fp, "DECODED OUTER IP-ID:[%d]\n", *p_dec_ip_id);
        fClose(fp);
#endif

    ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:\
                                            ENCD_IP-ID[%u]:DECODED_IP-ID[%u]" \
                                            "IP-ID DECODING CALLED", \
                                            p_rohc_entity->rohc_entity_id.ueIndex,
                                            p_rohc_entity->rohc_entity_id.lcId, \
                                            ROHC_GET_DSC_CID(p_dsc),
                                            p_en_val->value, *p_dec_ip_id);

    ROHC_EXIT_FN("rohcDecodeP6OuterIpId");
            
    return ROHC_SUCCESS ;
}
#endif

/*****************************************************************************
 * Function Name  : rohc_p6_decode_msn_in_feedback
 * Inputs         : p_rohc_entity- pointer to ROHC entity
 *                  p_csc - pointer to compressor context
 *                  temp_msn- lsb encoded 8 bit msn
 * Outputs        : 
 * Returns        : ROHC_SUCCESS
 * Description    : This function decodes the MSN value using W-LSB decoding
 *                  mechanism.
 *****************************************************************************/
rohc_return_t rohc_p6_decode_msn_in_feedback    ROHC_PARAMS
    (
        (p_rohc_entity, p_csc, temp_msn, p_dec_msn, p_ecode),
        rohc_entity_t           *p_rohc_entity  _AND_
        rohc_csc_tcp_t          *p_csc          _AND_
        rohc_U32bit_t           temp_msn        _AND_
        rohc_U32bit_t           *p_dec_msn       _AND_
        rohc_error_code_t       *p_ecode
    )
{
    register rohc_U16bit_t   new_v;
    register rohc_U16bit_t  min_v;
    register rohc_U16bit_t max_v;
    rohc_U32bit_t msn_prev;
    rohc_U8bit_t bits;

    rohc_U8bit_t    adj_needed = ROHC_FALSE;
    rohc_U32bit_t   interval_size ;
    ROHC_ENTER_FN("rohc_p6_decode_msn_in_feedback");

    msn_prev = p_csc->field_value[ROHC_TCP_MSN_FID-ROHC_VAR_FID_BASE];
    bits = 4;
    ROHC_PUT_LSBS(new_v, msn_prev, temp_msn, bits);

    /* To find the min. and max. value of interpretation interval */
    min_v = (rohc_U16bit_t)((rohc_U16bit_t)msn_prev - 4);
    max_v = (rohc_U16bit_t)((rohc_U16bit_t)msn_prev +
            (ROHC_POWER_OF_2(bits) - 1) - 4);
    /* If new value does not lies in the interpretation interval,
     *     * then discard the packet */
    if(max_v >= min_v)
    {   
        if(!((new_v <= max_v) && (new_v >= min_v)))
        {
            adj_needed = ROHC_TRUE;
        } 
    }else
    {
        if(!((new_v>=min_v) ||
                    (new_v <= max_v) ))
        {
            adj_needed = ROHC_TRUE;
        }
    }

    if(adj_needed) 
    {
        /* As per RFC 3095, Section 5.3.2.2.2,
         * If new_v is not within this interval, an adjustment is
         * applied by adding N x interval_size to the reconstructed value so
         * that the result is brought within the interpretation interval. 
         * Note that N can be negative. */
        /* If new_v < min_v, N is (+)ve, and if new_v > max_v, N is (-)ve. */
        interval_size = ROHC_POWER_OF_2(bits);
        do
        {
            adj_needed = ROHC_FALSE;
            new_v = new_v + interval_size; 
            if(max_v >= min_v)
            {   
                if(!((new_v <= max_v) && (new_v >= min_v)))
                {
                    adj_needed = ROHC_TRUE;
                } 
            }else
            {
                if(!((new_v>=min_v ) ||
                (new_v <= max_v) ))
                {
                    adj_needed = ROHC_TRUE;
                }
            }
        } while (adj_needed);
    }

    *p_dec_msn = new_v;

    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:\
            ENCD_MSN[%u]:DECODED_MSN[%u]" \
            "MSN DECODING CALLED ON FEEDBACK", \
            p_rohc_entity->rohc_entity_id.ueIndex,
            p_rohc_entity->rohc_entity_id.lcId, \
            ROHC_GET_CSC_CID(p_csc),
            temp_msn, new_v);

    ROHC_EXIT_FN("rohc_p6_decode_msn_in_feedback");
    return ROHC_SUCCESS;
}
