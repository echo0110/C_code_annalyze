/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_ed_utils.c,v 1.3.36.2 2010/11/09 05:12:14 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : Utility Functions for ROHC encoding decoding sub-module.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_ed_utils.c,v $
 * Revision 1.3.36.2  2010/11/09 05:12:14  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.3.36.1  2010/09/10 10:36:13  gur22059
 * Locality of reference & use of register variables for better performamce
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
#include "rohc_trace_mcr.h"
#include "rohc_ccom_mcr.h"
#include "rohc_dcom_mcr.h"
#include "rohc_gcom_mcr.h"
/* Start for SPR# 6563 */
#include "rohc_cprm_def_p1.h"
/* End for SPR# 6563 */

/******************************************************************************
  Private Definitions
 *****************************************************************************/
/* +- SPR 17777 */
extern tickType_t pdcpCurrentTime_g;
 /* +- SPR 17777 */
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
#ifdef ROHC_UT_FLAG
    extern FILE *fp;
#endif

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/




/*****************************************************************************
 * Function Name  : rohc_sn_g
 * Inputs         : v_ref - Reference Value, present in the context.
 *                  sn - new value of SN received in the current Packet.
 *                  *p_csc - pointer to CSC, needed to know the profile.
 * Outputs        : 
 * Returns        : k = Number to LSBs computed.
 * Description    : This function verifies whether the computed value of "K" 
 *                  lies in the interpretation interval. If not then it increments
 *                  the value of k. It checks for SN.
 *****************************************************************************/
rohc_U8bit_t rohc_sn_g      ROHC_PARAMS
    (
        (v_ref, sn, profile),
        rohc_U32bit_t   v_ref   _AND_
        rohc_S32bit_t   sn      _AND_
        register rohc_U8bit_t profile
    )
{
    register rohc_U8bit_t        k = 0;
    rohc_S32bit_t       p = 0;
    
    ROHC_ENTER_FN("rohc_sn_g");
    
    /* As per RFC 3095 , Section 5.7
     * For SN: The compressed RTP Sequence Number, Compressed with W-LSB.
     * The interpretation intervals, see section 4.5.1, are defined as follows:
     * p = 1                   if bits(SN) <= 4
     * p = 2^(bits(SN)-5) - 1  if bits(SN) >  4
     */

    /* As per RFC 3095 , Section 5.11
     * The ROHC UDP profile always uses p = -1 when interpreting the SN,
     * since there will be no repetitions or reordering of the compressor-
     * generated SN.  The interpretation interval thus always starts with
     * (ref_SN + 1).
     */
     
    p = ((profile == ROHC_IP_UDP) ? -1 : 1);

    for (k = 0; k < ROHC_MAX_SN_BITS; k++)
    {
        if(rohc_is_in_intval(v_ref, p, k, sn))
            break;
        if (k > 4)
            p = ((profile == ROHC_IP_UDP) ? -1 : ((ROHC_POWER_OF_2(k - 5)) - 1));
#ifdef ROHC_UT_FLAG
    fprintf(fp, "P = [%d] \n", p);
#endif
    }

    ROHC_EXIT_FN("rohc_sn_g");
    return k;
}

#ifdef ROHC_FUNC_ENHANCEMENT
/*****************************************************************************
 * Function Name  : rohc_ip_id_g
 * Inputs         : v_ref - Reference Value, present in the context.
 *                  offset_i - new value of OFFSET_I computed.
 * Outputs        : 
 * Returns        : k = Number to LSBs computed.
 * Description    : This function verifies whether the computed value of "K"
 *                  lies in the interpretation interval. If not then it increments
 *                  the value of k. It checks for IP_ID.
 *****************************************************************************/
rohc_U8bit_t rohc_ip_id_g       ROHC_PARAMS
    (
        (v_ref, offset_i),
        rohc_U32bit_t       v_ref       _AND_
        rohc_S32bit_t       offset_i
    )
{
    rohc_U8bit_t        k = 0;

    ROHC_ENTER_FN("rohc_ip_id_g");
    
    /* case when difference between SN and IP-ID is constant, K should be zero */
    if( 0 == (v_ref - offset_i))
        k = 0;
    else
        for (k = 0; k < ROHC_MAX_IP_ID_BITS; k++)
        {
            if(rohc_is_in_intval(v_ref, 0, k, offset_i))
                break;
        }

    ROHC_EXIT_FN("rohc_ip_id_g");
    return k;
}
#endif
/*****************************************************************************
 * Function Name  : rohc_ts_scaled_g
 * Inputs         : v_ref - Reference Value, present in the context.
 *                  ts_scaled - new value of ts_scaled computed.
 * Outputs        : 
 * Returns        : k = Number to LSBs computed.
 * Description    : This function verifies whether the computed value of "K" 
 *                  lies in the interpretation interval. If not then it
 *                  increments the value of k. It checks for TS.
 *****************************************************************************/
rohc_U8bit_t rohc_ts_scaled_g       ROHC_PARAMS
    (
        (v_ref, ts_scaled),
        rohc_U32bit_t   v_ref   _AND_
        rohc_U32bit_t   ts_scaled
    )
{
    rohc_U8bit_t        k = 0;
    rohc_S32bit_t       p = 0;
    
    ROHC_ENTER_FN("rohc_ts_scaled_g");
/*SPR #1393 Fix */
    if(v_ref != ts_scaled )
        for (k = 2; k < ROHC_MAX_TS_BITS; k++)
        {
            p = (ROHC_POWER_OF_2(k - 2) - 1); 
            if(rohc_is_in_intval(v_ref, p, k, ts_scaled))
                break;
        }
        
#ifdef ROHC_UT_FLAG
            fprintf(fp, "INITIAL K:[%d] \n", k);
#endif

    ROHC_EXIT_FN("rohc_ts_scaled_g");
    
    return k;
}


/*****************************************************************************
 * Function Name  : rohc_ts_p1_timer_based_g
 * Inputs         : v_ref - Reference Value, present in the context.
 *                  ts_scaled - new value of ts_scaled computed.
 *                  *p_csc - pointer to csc, for finding max_jiter_bc and max_jitter_cd.
 * Outputs        : 
 * Returns        : k = Number to LSBs computed.
 * Description    : This function verifies whether the computed value of "K" lies
 *                  in the interpretation interval. If not then it increments the
 *                  value of k. It checks for TS.
 *****************************************************************************/
rohc_U8bit_t rohc_ts_p1_timer_based_g      ROHC_PARAMS
    (
        (v_ref, ts_scaled, p_csc),
        rohc_U32bit_t       v_ref       _AND_
        rohc_S32bit_t       ts_scaled   _AND_
        rohc_csc_rtp_t      *p_csc

    )
{
    rohc_U8bit_t        k = 0;
    rohc_S32bit_t       p = 0;
    
    ROHC_ENTER_FN("rohc_ts_p1_timer_based_g");
    
    /* Compute Minimum Number of LSBs to be sent to convey TS */
    k = (rohc_U8bit_t)ROHC_CEIL(ROHC_LOG_2
                     (2 * (p_csc->max_jitter + p_csc->max_jitter_cd + 2) + 1));
        
#ifdef ROHC_UT_FLAG
            fprintf(fp, "INITIAL K:[%d] \n", k);
#endif
            
    /* If the reference falls in the interpretation interval, then return  this value,
     * else increment the value of k and recompute p and  interpretation interval */

    for((p = (ROHC_POWER_OF_2(k-1) -1));
        !rohc_is_in_intval(v_ref, p, k, ts_scaled);
        k++,(p=(ROHC_POWER_OF_2(k-1) -1)));

    ROHC_EXIT_FN("rohc_ts_p1_timer_based_g");
    
    return k;
}

/*****************************************************************************
 * Function Name  : rohc_is_within_in_intval
 * Inputs         : v_ref - Reference Value, present in the context.
 *                  p - interpretation interval shifting characteristics.
 *                  k  = number of LSBs.
 *                  new_v  = new value received in the current Packet.
 * Outputs        : 
 * Returns        : ROHC_SUCCESS if the new_v lies within the interpretation interval.
 *                  ROHC_FAILURE = otherwise.
 * Description    : This function decides whether the new value received in the
 *                  current Packet lies within the interpretation interval or not.
 *****************************************************************************/
rohc_return_t rohc_is_in_intval     ROHC_PARAMS
    (
        (v_ref, p, k, new_val),
        rohc_U32bit_t   v_ref       _AND_
        rohc_S32bit_t   p           _AND_
        rohc_U8bit_t    k           _AND_
        register rohc_S32bit_t   new_val
    )
    
{
    rohc_S32bit_t   min_v = 0, max_v = 0;
    rohc_return_t   ret_val;

    ROHC_ENTER_FN("rohc_is_in_intval");

    rohc_f(v_ref, p, k, &min_v, &max_v);
    if ((new_val <= max_v) && (new_val >= min_v)) 
        ret_val = ROHC_SUCCESS;
    else
        ret_val = ROHC_FAILURE;

#ifdef ROHC_UT_FLAG
    fprintf(fp, "MIN_V : [%d] \nMAX_V : [%d] \n", min_v, max_v);
#endif
        
    ROHC_EXIT_FN("rohc_is_in_intval");
    return ret_val;
}

/*****************************************************************************
 * Function Name  : rohc_f
 * Inputs         : v_ref, p, k, min_v, max_v
 * Outputs        : 
 * Returns        : None
 * Description    : This function finds the interpratation interval 
 *                  [vref-p, v_ref+(2^k-1)-p] for given k. Part of the LSB
 *                  calculation algorithm.
 *****************************************************************************/
rohc_void_t rohc_f      ROHC_PARAMS
    (
        (v_ref, p, k, p_min_v, p_max_v),
        register rohc_U32bit_t   v_ref  _AND_
        register rohc_S32bit_t   p      _AND_
        register rohc_U8bit_t    k      _AND_
        rohc_S32bit_t   *p_min_v        _AND_
        rohc_S32bit_t   *p_max_v
    )
{
    ROHC_ENTER_FN("rohc_f");
    *p_min_v = v_ref - p;
    *p_max_v = v_ref + (ROHC_POWER_OF_2(k) - 1) - p;    
    ROHC_EXIT_FN("rohc_f");
}

/*****************************************************************************
 * Function Name  : rohc_encode_p1_scaled_ts
 * Inputs         : p_rohc_entity, p_csc, ts, *p_en_val
 * Outputs        : 
 * Returns        : None
 * Description    : This function encodes the RTP TimeMtamp value using Scaled
 *                  RTP Timestamp encoding mechanism. The value of TS_STRIDE can
 *                  be picked from the Stream Context.
 *****************************************************************************/
rohc_void_t rohc_encode_p1_scaled_ts       ROHC_PARAMS
    (
        (p_rohc_entity, p_csc, ts, p_en_val),
        rohc_entity_t       *p_rohc_entity  _AND_
        rohc_csc_rtp_t      *p_csc          _AND_
        rohc_U32bit_t       ts              _AND_
        rohc_encoded_value_t *p_en_val
    )
{
    rohc_U32bit_t       i,v_ref_min, v_ref_max;
    rohc_U32bit_t       ts_scaled;
    rohc_U8bit_t        k = 0, k1 = 0, k2 = 0;

    ROHC_ENTER_FN("rohc_encode_p1_scaled_ts");
    if(! p_csc->ts_stride)
        return;
    /* ts_stride in updated in csc in PPM */
    ts_scaled = (ts / p_csc->ts_stride);
    
    v_ref_min = p_csc->rohc_gsw.min_ts_scaled;
    v_ref_max = p_csc->rohc_gsw.max_ts_scaled;
            
#ifdef ROHC_UT_FLAG
    fp = fopen("outfile", "a");
    fprintf(fp, "================= ENCODED SCALED TS ===============\n");
    fprintf(fp, "V_REF_MIN:[%d] \nV_REF_MAX:[%d] \nTS:[%d] \n", 
                                               v_ref_min, v_ref_max, ts);
#endif
    
    /* Compute Minimum Number of LSBs to be sent to convey TS */
    k1 = rohc_ts_scaled_g(v_ref_min, ts_scaled) ;
    k2 = rohc_ts_scaled_g(v_ref_max, ts_scaled) ;
    k = ((k1 < k2) ? k2 : k1);

    /* Update OUTPUT parameter p_en_val */
    p_en_val->value  = ts_scaled;
    p_en_val->bits  = k;

#ifdef ROHC_UT_FLAG
    fprintf(fp, "MIN_K:[%d] \nMAX_K:[%d] \nK:[%d] \n", k1, k2, k);
    fprintf(fp, "P_EN_VAL->VALUE:[%d] \nP_EN_VAL->BITS:[%d] \n", 
                                p_en_val->value, p_en_val->bits);
    fprintf(fp, "\n");
    fClose(fp);
#endif
/*SPR #1393 Start*/
    /* Update window */
     if( (p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].ts_scaled
            != p_csc->rohc_gsw.min_ts_scaled) &&
        (p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].ts_scaled
            != p_csc->rohc_gsw.max_ts_scaled) )
    {
        p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].ts_scaled 
                = ts_scaled;

        /* update the min. and max. value of TS_SCALED. It is the min. and max.\
        * value from start_index to end_index */
        p_csc->rohc_gsw.min_ts_scaled = \
        (p_csc->rohc_gsw.min_ts_scaled < ts_scaled) ? p_csc->rohc_gsw.\
                min_ts_scaled : ts_scaled;
    
        p_csc->rohc_gsw.max_ts_scaled = \
        (p_csc->rohc_gsw.max_ts_scaled > ts_scaled) ? p_csc->rohc_gsw.\
                max_ts_scaled : ts_scaled;
    }
    else if(p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].\
            ts_scaled == p_csc->rohc_gsw.min_ts_scaled)
    {
        p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].ts_scaled 
            = ts_scaled;
        p_csc->rohc_gsw.min_ts_scaled = 
            p_csc->rohc_gsw.rohc_gsw_elem[0].ts_scaled;
        for(i = 1; i< ROHC_MAX_LENGTH_GSW ; i++)
        {
            p_csc->rohc_gsw.min_ts_scaled =
                ((p_csc->rohc_gsw.min_ts_scaled < p_csc->rohc_gsw.\
                    rohc_gsw_elem[i].ts_scaled)?
                p_csc->rohc_gsw.min_ts_scaled : p_csc->rohc_gsw.\
                    rohc_gsw_elem[i].ts_scaled);
        }
         p_csc->rohc_gsw.max_ts_scaled = \
        (p_csc->rohc_gsw.max_ts_scaled > ts_scaled) ? p_csc->rohc_gsw.\
                max_ts_scaled : ts_scaled;
    }
    else
    {
        p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].ts_scaled 
            = ts_scaled;
        p_csc->rohc_gsw.max_ts_scaled = 
            p_csc->rohc_gsw.rohc_gsw_elem[0].ts_scaled;
        for(i = 1; i< ROHC_MAX_LENGTH_GSW ; i++)
        {
            p_csc->rohc_gsw.max_ts_scaled =
                ((p_csc->rohc_gsw.max_ts_scaled > p_csc->rohc_gsw.\
                    rohc_gsw_elem[i].ts_scaled)?
                p_csc->rohc_gsw.max_ts_scaled : p_csc->rohc_gsw.\
                    rohc_gsw_elem[i].ts_scaled);
        }
        p_csc->rohc_gsw.min_ts_scaled = \
        (p_csc->rohc_gsw.min_ts_scaled < ts_scaled) ? p_csc->rohc_gsw.\
                min_ts_scaled : ts_scaled;
    }
/*SPR #1393 End*/
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),ROHC_ED_GET_CUR_INDEX(p_csc),ts,k, __func__,
            "PdcpRxStateMachine");
    /* +- SPR 17777 */


    ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:CUR_ID[%u]:\
                        ENCD_SN[%u]:BITS[%u]" \
                       "SCALED TS ENCODING CALLED", \
                       p_rohc_entity->rohc_entity_id.ueIndex,
                       p_rohc_entity->rohc_entity_id.lcId, \
                       ROHC_GET_CSC_CID(p_csc),
                       ROHC_ED_GET_CUR_INDEX(p_csc), ts, k );

    ROHC_EXIT_FN("rohc_encode_p1_scaled_ts");
}

/*****************************************************************************
 * Function Name  : rohc_encode_p1_timer_based_ts
 * Inputs         : p_rohc_entity, p_csc, ts, *p_en_val
 * Outputs        : 
 * Returns        : None
 * Description    : This function encodes the RTP Timestamp value Timer based
 *                  encoding mechanism.
 *****************************************************************************/
rohc_void_t rohc_encode_p1_timer_based_ts      ROHC_PARAMS
    (
        (p_rohc_entity, p_csc, ts, p_en_val),
        rohc_entity_t           *p_rohc_entity  _AND_
        rohc_csc_rtp_t          *p_csc          _AND_
        rohc_U32bit_t           ts              _AND_
        rohc_encoded_value_t    *p_en_val
    )
{
    rohc_U32bit_t       v_ref_min_ts_scaled, v_ref_max_ts_scaled;
        
    /* rohc_U32bit_t       pkt_arvl_time; */
    rohc_U32bit_t       ts_scaled;
    
/*  rohc_S32bit_t       jitter_bc, min_jitter_bc, max_jitter_bc; */

    rohc_U8bit_t        k = 0, k1 = 0, k2 = 0;

    ROHC_ENTER_FN("rohc_encode_p1_timer_based_ts");
                
    ts_scaled = (ts / p_csc->ts_stride);
    
    /* The packet arrival time would be updated in CSC in main module 
     * after creation of csc in PPM 
    pkt_arvl_time = \
      p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].pkt_arvl_time;
    */    
    v_ref_min_ts_scaled  = p_csc->rohc_gsw.min_ts_scaled;
    v_ref_max_ts_scaled  = p_csc->rohc_gsw.max_ts_scaled;
                
        
    /* Compute Minimum Number of LSBs to be sent to convey TS */
    k1 = rohc_ts_p1_timer_based_g(v_ref_min_ts_scaled, ts_scaled, p_csc);
    k2 = rohc_ts_p1_timer_based_g(v_ref_max_ts_scaled, ts_scaled, p_csc);
    k = ((k1 < k2) ? k2 : k1);

    p_en_val->value  = ts_scaled;
    p_en_val->bits  = k;
    
    /* Update window */
    p_csc->rohc_gsw.rohc_gsw_elem[
     ROHC_ED_GET_CUR_INDEX(p_csc)].ts_scaled = ts_scaled;
    
    /* update the min. and max. value of TS_SCALED. It is the min. and max. \
     * value from start_index to end_index */
    p_csc->rohc_gsw.min_ts_scaled = \
    (p_csc->rohc_gsw.min_ts_scaled < ts_scaled) ? 
    p_csc->rohc_gsw.min_ts_scaled : ts_scaled;
    
    p_csc->rohc_gsw.max_ts_scaled = \
    (p_csc->rohc_gsw.max_ts_scaled > ts_scaled) ? 
    p_csc->rohc_gsw.max_ts_scaled : ts_scaled;

    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:CUR_ID[%u]:\
                       ENCD_SN[%u]:BITS[%u]" \
                       "TIMER BASED TS ENCODING CALLED", \
                       p_rohc_entity->rohc_entity_id.ueIndex,
                       p_rohc_entity->rohc_entity_id.lcId, \
                       ROHC_GET_CSC_CID(p_csc), 
                       ROHC_ED_GET_CUR_INDEX(p_csc), ts, k );

    ROHC_EXIT_FN("rohc_encode_p1_timer_based_ts");
}

/*****************************************************************************
 * Function Name  : rohc_decode_p1_scaled_ts
 * Inputs         : p_rohc_entity, p_dsc, *p_en_val, *p_dec_ts
 * Outputs        : 
 * Returns        : None
 * Description    : This function decodes the RTP Timestamp value using Scaled
 *                  RTP Timestamp decoding mechanism.
 *****************************************************************************/
rohc_void_t rohc_decode_p1_scaled_ts       ROHC_PARAMS
    (
        (p_rohc_entity, p_dsc, p_en_val, p_dec_ts, sn_delta),
        rohc_entity_t           *p_rohc_entity  _AND_
        rohc_dsc_rtp_t          *p_dsc          _AND_
        rohc_encoded_value_t    *p_en_val       _AND_
        rohc_U32bit_t           *p_dec_ts       _AND_
	/*Start SPR 5818*/
        rohc_U32bit_t           sn_delta
	/*End SPR 5818*/
    )
{
    rohc_U32bit_t       ts_offset, new_ts_offset;
    rohc_U32bit_t       p = 0, R=0, r = 0;
    rohc_U32bit_t       new_ts_scaled, local_tss_max;
    rohc_U32bit_t       interval_size ;
    /* + SPR 17439 */
    rohc_S32bit_t       new_v, min_v, max_v;
    /* - SPR 17439 */
    rohc_S8bit_t        adjustment_factor_N;
    rohc_U32bit_t       latest_ts_stride;
    ROHC_ENTER_FN("rohc_decode_p1_scaled_ts");
    
    /*Start SPR 6192 Fix*/
    /* Check if the TSS fields is set in RTP flags */
    if(0 == (p_dsc->dpib->field_val[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE] & ROHC_ENC_TSS_RTPFLAGP1_MASK))
    {
        latest_ts_stride = p_dsc->ts_stride;
    }
    else
    {   
        latest_ts_stride =p_dsc->dpib->field_val[ROHC_RTP_TS_STRIDE_FID -
                                    ROHC_VAR_FID_BASE];
	/* Update the ts_stride value in decompressor context */
        p_dsc->ts_stride = latest_ts_stride;
    }   
    /*End SPR 6192 Fix*/
    /*Start SPR 5818 */
    /* If TS bits set to 0, sn_delta is non-zero*/
    if (sn_delta)
    {
        /* TS is calculated on the basis of SN Diff*/
        *p_dec_ts = (sn_delta * latest_ts_stride) + p_dsc->pre_ts;
        return;
    }
    /*End SPR 5818 */

    /* SPR5815 Fix Start */
    if ((p_dsc->ts_stride == 0) ||
        ((p_dsc->dpib->field_val[ROHC_EXT3_FLAGS_FID-ROHC_VAR_FID_BASE])  &&
            (0== (p_dsc->dpib->field_val[ROHC_EXT3_FLAGS_FID-ROHC_VAR_FID_BASE] & ROHC_ENC_TSC_EXT3FLAGP1_MASK))))
    /* SPR5815 Fix end */
    {
        /* Start for SPR# 6563 */  
        if ( ROHC_P1_TS_BITS > p_en_val->bits   )
        {
    	ROHC_PUT_LSBS(new_v, p_dsc->pre_ts, 
                    p_en_val->value, p_en_val->bits);
		*p_dec_ts = new_v ;	
        }
        else
        {
            *p_dec_ts = p_en_val->value;
        }
        /* End for  SPR# 6563 */  
    	ROHC_EXIT_FN("rohc_decode_p1_scaled_ts");
		return;
    }

    p_dsc->pre_scaled_ts =  p_dsc->pre_ts/p_dsc->ts_stride;
    ROHC_PUT_LSBS(new_v, p_dsc->pre_scaled_ts, 
                    p_en_val->value, p_en_val->bits);

#ifdef ROHC_UT_FLAG
    fp = fopen("outfile", "a");
    fprintf(fp, "====================== DECODED SN ========================\n");
    fprintf(fp, "V_REF:[%d]\nENCODED_VAL:[%d]\nK:[%d]\nNEW_V:[%d]\n",
                p_dsc->pre_ts, p_en_val->value, p_en_val->bits, new_v);
#endif
                    
    /* k = p_en_val->bits */

    ts_offset = p_dsc->pre_ts %latest_ts_stride; 
    
        /*+- SPR 20899 Fix*/
    if ((rohc_U32bit_t)ROHC_ABS(p_dsc->pre_ts/p_dsc->ts_stride - new_v) >= \
            (TSS_MAX/MULTIPLY_BY_TWO(p_dsc->ts_stride)))                 /* wraparound has occurred */
        /*+- SPR 20899 Fix*/
    {
        p = (ROHC_POWER_OF_2(p_en_val->bits - 2) - 1);

        /* To find the min. and max. value of interpretation interval */
        rohc_f(p_dsc->pre_scaled_ts, p, p_en_val->bits, &min_v, &max_v);

#ifdef ROHC_UT_FLAG
    fprintf(fp, "MIN_V:[%d]\nMAX_V:[%d]\n", min_v, max_v);
#endif

        /* If new value lies in the interpretation interval,
        * then this is the decided value */
    
        if (!((new_v <= max_v) && (new_v >= min_v)))
        {
            /* As per RFC 3095, Section 5.3.2.2.2,
            * If new_v is not within this interval, an adjustment is
            * applied by adding N x interval_size to the reconstructed value so
            * that the result is brought within the interpretation interval. 
            * Note that N can be negative. */
    
            /* If new_v < min_v, N is (+)ve, and if new_v > max_v, N is (-)ve. */
            adjustment_factor_N = ((new_v > max_v ) ? -1 : 1) ;
            interval_size = ROHC_POWER_OF_2(p_en_val->bits);
            
            do
            {
                new_v = new_v + (adjustment_factor_N) * interval_size; 
            } while (!((new_v <= max_v) && (new_v >= min_v)));
        }

        R = (ROHC_POWER_OF_2(p_en_val->bits) - 1) % p_dsc->ts_stride;
        r = p_dsc->pre_scaled_ts % p_dsc->ts_stride ;
        
        if(r <= R)
            local_tss_max = TSS_MAX;
        else
            local_tss_max = TSS_MAX - 1;
    
        new_ts_scaled = (p_dsc->pre_ts + \
            ((local_tss_max - p_dsc->pre_scaled_ts + new_v + 1 )
                                * p_dsc->ts_stride)) ;
        
        new_ts_offset = new_ts_scaled % latest_ts_stride; 
            
        *p_dec_ts = (new_ts_scaled * latest_ts_stride) + new_ts_offset;
    }
    else
    {
/*SPR #1393 Fix */
        /*Coverity 72759 Fix Start*/
        if(p_en_val->bits >= 2)
            p = (ROHC_POWER_OF_2(p_en_val->bits - 2) - 1);
        else
            p = 0;
        /*Coverity 72759 Fix End*/
        
        /* To find the min. and max. value of interpretation interval */
        rohc_f(p_dsc->pre_scaled_ts, p, p_en_val->bits, &min_v, &max_v);

#ifdef ROHC_UT_FLAG
    fprintf(fp, "MIN_V:[%d]\nMAX_V:[%d]\n", min_v, max_v);
#endif
        /* If new value lies in the interpretation interval,
        * then this is the decided value */
	if(min_v > max_v && new_v <= max_v)
	{
		min_v =0; 
	}
    
        if (!((new_v <= max_v) && (new_v >= min_v)))
        {
            /* As per RFC 3095, Section 5.3.2.2.2,
            * If new_v is not within this interval, an adjustment is
            * applied by adding N x interval_size to the reconstructed value so
            * that the result is brought within the interpretation interval. 
            * Note that N can be negative. */
    
            /* If new_v < min_v, N is (+)ve, and if new_v > max_v, N is (-)ve. */
            adjustment_factor_N = ((new_v > max_v ) ? -1 : 1) ;
            interval_size = ROHC_POWER_OF_2(p_en_val->bits);
            
            do
            {
                new_v = new_v + (adjustment_factor_N) * interval_size; 
            } while (!((new_v <= max_v) && (new_v >= min_v)));
        }

        *p_dec_ts = (new_v * latest_ts_stride) + ts_offset;
    }

    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:\
                        ENCD_TS[%u]:DECODED_TS[%u]" \
                       "SCALED TS DECODING CALLED", \
                        p_rohc_entity->rohc_entity_id.ueIndex,
                        p_rohc_entity->rohc_entity_id.lcId, \
                        ROHC_GET_DSC_CID(p_dsc),
                        p_en_val->value, new_v);

    ROHC_EXIT_FN("rohc_decode_p1_scaled_ts");
}

/*****************************************************************************
 * Function Name  : rohc_decode_p1_timer_based_ts
 * Inputs         : p_rohc_entity, p_dsc, *p_en_val, *p_dec_ts
 * Outputs        : 
 * Returns        : None
 * Description    : This function decodes the RTP Timestamp value using Timer
 *                  Based RTP Timestamp decoding mechanism.
 *****************************************************************************/
rohc_void_t rohc_decode_p1_timer_based_ts  ROHC_PARAMS
    (
        (p_rohc_entity, p_dsc, p_en_val, p_dec_ts),
        rohc_entity_t           *p_rohc_entity  _AND_
        rohc_dsc_rtp_t          *p_dsc          _AND_
        rohc_encoded_value_t    *p_en_val       _AND_
        rohc_U32bit_t           *p_dec_ts
    )
{
    rohc_U32bit_t   t_approx, p, interval_size ;
    rohc_S32bit_t   new_v, min_v, max_v;
    rohc_S8bit_t    adjustment_factor_N;
    rohc_U32bit_t   diff_arv_tm ;
    rohc_U32bit_t latest_ts_stride;

    ROHC_ENTER_FN("rohc_decode_p1_timer_based_ts");
    
    if(p_dsc->dpib->field_val[ROHC_RTP_TSS_FID - ROHC_VAR_FID_BASE] != 1)
    {
        latest_ts_stride = p_dsc->ts_stride;
    }
    else
    {   
        latest_ts_stride =p_dsc->dpib->field_val[ROHC_RTP_TS_STRIDE_FID -
                                    ROHC_VAR_FID_BASE];
    }   
    ROHC_PUT_LSBS(new_v, (p_dsc->pre_ts/p_dsc->ts_stride), 
                  p_en_val->value, p_en_val->bits);

    /* As per RFC 3095, Sec 4.5.4
     * Note: Integer arithmetic is used in all equations above.  If
     * TIME_STRIDE is not equal to an integral number of clock ticks,
     * time must be normalized such that TIME_STRIDE is an integral
     * number of clock ticks.  For example, if a clock tick is 20 ms and
     * TIME_STRIDE is 30 ms, (a_n - a_ref) in 2) can be multiplied by 3
     * and TIME_STRIDE can have the value 2.
     */
        
    diff_arv_tm = (p_dsc->arv_tm_cur_pkt - p_dsc->arv_tm_pre_pkt);

    diff_arv_tm =\
        (((diff_arv_tm % latest_ts_stride) == 0) ? diff_arv_tm : 
                                (diff_arv_tm * latest_ts_stride));
        
    t_approx = p_dsc->pre_scaled_ts + (diff_arv_tm/latest_ts_stride);
    
    p = ((ROHC_POWER_OF_2(p_en_val->bits - 1)) - 1);

    /* To find the min. and max. value of interpretation interval */
    rohc_f(t_approx, p, p_en_val->bits, &min_v, &max_v);

    /* If new value lies in the interpretation interval,
    * then this is the decided value */
    
    if (!((new_v <= max_v) && (new_v >= min_v)))
    {
        /* As per RFC 3095, Section 5.3.2.2.2,
         * If new_v is not within this interval, an adjustment is
         * applied by adding N x interval_size to the reconstructed value so
         * that the result is brought within the interpretation interval. 
         * Note that N can be negative. */

        /* If new_v < min_v, N is (+)ve, and if new_v > max_v, N is (-)ve. */
        adjustment_factor_N = ((new_v > max_v ) ? -1 : 1) ;
        interval_size = ROHC_POWER_OF_2(p_en_val->bits);
        
        do
        {
            new_v = new_v + (adjustment_factor_N) * interval_size; 
        } while (!((new_v <= max_v) && (new_v >= min_v)));
    }

    *p_dec_ts = new_v * latest_ts_stride;

    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_rohc_entity->rohc_entity_id.ueIndex,p_rohc_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO, "ED:EID[%u:%u]:CID[%u]:\
                        ENCD_TS[%u]:DECODED_TS[%u]" \
                       "TIMER BASED TS DECODING CALLED", \
                        p_rohc_entity->rohc_entity_id.ueIndex,
                        p_rohc_entity->rohc_entity_id.lcId, \
                        ROHC_GET_DSC_CID(p_dsc),
                        p_en_val->value, new_v);

    ROHC_EXIT_FN("rohc_decode_p1_timer_based_ts");
}
