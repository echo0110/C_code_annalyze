/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_main_pk_unpk.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ******************************************************************************
 *
 *  File Description : Utility Functions for ROHC pack and unpack of context
 *                     info.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_main_pk_unpk.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.3  2009/05/28 03:26:54  gur19836
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
#ifdef ROHC_FUNC_ENHANCEMENT

#include "rohc_typ.h"
#include "rohc_def.h"
#include "rohc_ex.h"

#include "rohc_ccom_mcr.h"
#include "rohc_g_typ.h"
#include "rohc_gcom_mcr.h"
#include "rohc_c_typ.h"
#include "rohc_d_typ.h"
#include "rohc_trace_mcr.h"
#include "rohc_sl_windows.h"
 
#include "rohc_main_def.h"
#include "rohc_main_typ.h"
#include "rohc_pk_proto.h"
#include "rohc_unpk_proto.h"
#include "rohc_main_proto.h"

#include "rohc_led_mcr.h"

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

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/




/******************************************************************************
 **  FUNCTION:
 **       rohc_pack_dyn_csc_info
 *******************************************************************************
 **  DESCRIPTION:
 **     This function packs the dynamic information of compressor context for 
 **     an entity i.e.. it fills the dynamic info from compressor context 
 **     structure to the buffer to be sent in ROHC_CONTEXT_CONF.
 **     
 **  INPUT:
 **     *p_csc - pointer to compressor context of an entity.
 **     *p_buf - The pointer to buffer to be filled.
 **     max_buf_len - max buffer length to be packed.
 **     *p_ecode - pointer to error code.
 **     
 **  RETURN:
 **     ROHC_SUCCESS
 **     
 ******************************************************************************/
rohc_len_t  rohc_pack_dyn_csc_info   ROHC_PARAMS
(
    ( p_csc, p_buf,  max_buf_len, p_ecode),
    rohc_csc_t              *p_csc      _AND_
    rohc_U8bit_t            *p_buf      _AND_
    rohc_U32bit_t           max_buf_len _AND_
    rohc_error_code_t       *p_ecode
)
{
    rohc_len_t      len = 0;
    /*rohc_U32bit_t   i, j;
    rohc_U16bit_t   sn_count;*/
    rohc_U32bit_t   loop_var = 0 ;

    ROHC_ENTER_FN("rohc_pack_dyn_csc_info");

    p_buf[len++] = ((p_csc->rohc_gsw.end_index + ROHC_MAX_LENGTH_GSW) -
                         p_csc->rohc_gsw.start_index) % ROHC_MAX_LENGTH_GSW+1; 
    loop_var = (p_csc->rohc_gsw.start_index + ROHC_MAX_LENGTH_GSW-1) % 
                                                        ROHC_MAX_LENGTH_GSW;
    do  
    {
        loop_var=(loop_var+1)%ROHC_MAX_LENGTH_GSW;
        len += rohc_pk_rohc_gsw_element_t((rohc_void_tp)
                    &(p_csc->rohc_gsw.rohc_gsw_elem[loop_var]),
                    p_buf+len, max_buf_len-len, p_ecode);
    }while(loop_var!=p_csc->rohc_gsw.end_index);
    p_buf[len++] = 0;
/*    p_buf[len++] = ROHC_CSC_GET_R_CSRC_SW_NIDX(p_csc) - 
                                ROHC_CSC_GET_R_CSRC_SW_RIDX(p_csc);
    
    for(i = ROHC_CSC_GET_R_CSRC_SW_RIDX(p_csc); i < ROHC_CSC_GET_R_CSRC_SW_NIDX(p_csc); i++)
    {
        len += rohc_pk_rohc_r_sw_t((rohc_void_tp)&(p_csc->rohc_c_r_csrc_sw.r_csrc_sw[i]), p_buf+len, max_buf_len-len, p_ecode);
    }
*/
    p_buf[len++] = 0;
    /*  p_buf[len++] = ROHC_CSC_GET_R_EXT_SW_NIDX(p_csc) - ROHC_CSC_GET_R_EXT_SW_RIDX(p_csc);

    for(i = ROHC_CSC_GET_R_EXT_SW_RIDX(p_csc); i < ROHC_CSC_GET_R_EXT_SW_NIDX(p_csc); i++)
    {
        len += rohc_pk_rohc_r_sw_t((rohc_void_tp)&(p_csc->rohc_c_r_ext_hdr_sw.r_ext_hdr_sw[i]), p_buf+len, max_buf_len-len, p_ecode);
    }*/
    p_buf[len++] =0;
/*
  
  p_buf[len++] = ROHC_CSC_GET_UO_CSRC_SW_NIDX(p_csc) - ROHC_CSC_GET_UO_CSRC_SW_RIDX(p_csc);
    for(i = ROHC_CSC_GET_UO_CSRC_SW_RIDX(p_csc); i < ROHC_CSC_GET_UO_CSRC_SW_NIDX(p_csc); i++)
    {
        len += rohc_pk_rohc_uo_sw_t((rohc_void_tp)&(p_csc->rohc_c_uo_csrc_sw.uo_csrc_sw[i]), p_buf+len, max_buf_len-len, p_ecode);
    }
*/
    p_buf[len++] =0;
    /*p_buf[len++] = ROHC_CSC_GET_UO_EXT_SW_NIDX(p_csc) - ROHC_CSC_GET_UO_EXT_SW_RIDX(p_csc);

    for(i = ROHC_CSC_GET_UO_EXT_SW_RIDX(p_csc); i < ROHC_CSC_GET_UO_EXT_SW_NIDX(p_csc); i++)
    {
        len += rohc_pk_rohc_uo_sw_t((rohc_void_tp)&(p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_sw[i]), p_buf+len, max_buf_len-len, p_ecode);
    }
*/

    p_buf[len++] =0;
    /*p_buf[len++] = ROHC_CSC_GET_R_CSRC_TT_NIDX(p_csc);

    for(i = 0; i < ROHC_CSC_GET_R_CSRC_TT_NIDX(p_csc); i++)
    {
        len += rohc_pk_r_csrc_tt_t((rohc_void_tp)&(p_csc->rohc_c_r_csrc_tt.r_csrc_tt[i].r_csrc_tt), p_buf+len, max_buf_len-len, p_ecode);*/

        /* IMPORTANT: Delete the packing routine for the array sn_no_list from the func 
         * rohc_pk_r_csrc_tt_t, every time you generate the pack unpack routine */

/*        sn_count = ROHC_CSC_GET_R_CSRC_TT_SN_IDX(p_csc, i);

        if ((len+1)>max_buf_len) 
        { 
            p_ecode = ROHC_FAILURE; 
            return len; 
        } 
        p_buf[len++] = (rohc_U8bit_t) (sn_count & 0xff);


        for (j=0; j<sn_count; ++j)
        {
            if ((len+4)>max_buf_len) 
            { 
                p_ecode = ROHC_FAILURE; 
                return len; 
            } 
            ROHC_SET_16BIT((p_buf+len), (p_csc->rohc_c_r_csrc_tt.r_csrc_tt[i].r_csrc_tt.sn_no_list[j]));
            len += 2;
        }
    }
*/
    p_buf[len++] =0;/*
    p_buf[len++] = ROHC_CSC_GET_R_EXT_TT_NIDX(p_csc);

    for(i = 0; i < ROHC_CSC_GET_R_EXT_TT_NIDX(p_csc); i++)
    {
        p_buf[len++] = (rohc_U8bit_t) \
            (p_csc->rohc_c_r_ext_hdr_tt.r_ext_hdr_tt[i].r_ext_hdr_tt.known & 0xff);
*/
        /* IMPORTANT : Delete the packing routine for the function rohc_pk_r_ext_hdr_tt_t,
         * every time you generate the pack unpack routine */

        /* packing of *p_item *//*
        len += rohc_pack_p_item(p_csc->rohc_c_r_ext_hdr_tt.r_ext_hdr_tt[i].r_ext_hdr_tt.p_item, p_buf+len);

        sn_count = ROHC_CSC_GET_R_EXT_TT_SN_IDX(p_csc, i);

        if ((len+1)>max_buf_len) 
        { 
            p_ecode = ROHC_FAILURE; 
            return len; 
        } 
        p_buf[len++] = (rohc_U8bit_t) (sn_count & 0xff);

        for (j=0; j<sn_count; ++j)
        {
            if ((len+4)>max_buf_len) 
            { 
                p_ecode = ROHC_FAILURE; 
                return len; 
            } 
            ROHC_SET_16BIT((p_buf+len), (p_csc->rohc_c_r_ext_hdr_tt.r_ext_hdr_tt[i].r_ext_hdr_tt.sn_no_list[j]));
            len += 2;
        }
    }

*/
    p_buf[len++] =0;
  /*  p_buf[len++] = ROHC_CSC_GET_UO_CSRC_TT_NIDX(p_csc);

    for(i = 0; i < ROHC_CSC_GET_UO_CSRC_TT_NIDX(p_csc); i++)
    {
        len += rohc_pk_uo_csrc_tt_t((rohc_void_tp)&(p_csc->rohc_c_uo_csrc_tt.uo_csrc_tt[i]), p_buf+len, max_buf_len-len, p_ecode);

    }

*/
    p_buf[len++] =0;
        
/*p_buf[len++] = ROHC_CSC_GET_UO_EXT_TT_NIDX(p_csc);

    for(i = 0; i < ROHC_CSC_GET_UO_EXT_TT_NIDX(p_csc); i++)
    {
        p_buf[len++] = (rohc_U8bit_t) \
            (p_csc->rohc_c_uo_ext_hdr_tt.uo_ext_hdr_tt[i].known & 0xff);

        *//* IMPORTANT : Delete the packing routine for the function rohc_pk_uo_ext_hdr_tt_t,
         * every time you generate the pack unpack routine */

        /* packing of *p_item *//*
        len += rohc_pack_p_item(p_csc->rohc_c_uo_ext_hdr_tt.uo_ext_hdr_tt[i].p_item, \
               p_buf+len);

        p_buf[len++] = (rohc_U8bit_t) \
            (p_csc->rohc_c_uo_ext_hdr_tt.uo_ext_hdr_tt[i].counter & 0xff);
    }
*/
    p_buf[len] = (rohc_U8bit_t)(0 & 0xff);
    if (p_csc->static_delta & ROHC_UDP_CHECK_SUM_PRESENT)
        ROHC_ENCODE_VAL(p_buf, len, 0, 1, 1);
    if (ROHC_NULL != p_csc->field_value[ROHC_RTP_TSS_FID - ROHC_VAR_FID_BASE])
        ROHC_ENCODE_VAL(p_buf, len, 1, 1, 1);
    if (ROHC_NULL != p_csc->field_value[ROHC_RTP_TIS_FID - ROHC_VAR_FID_BASE])
        ROHC_ENCODE_VAL(p_buf, len, 2, 1, 1);
    if (ROHC_TRUE == p_csc->new_inner_rnd)
        ROHC_ENCODE_VAL(p_buf, len, 3, 1, 1);
    if (ROHC_TRUE == p_csc->new_outer_rnd)
        ROHC_ENCODE_VAL(p_buf, len, 4, 1, 1);
    if (ROHC_TRUE == p_csc->new_inner_nbo)
        ROHC_ENCODE_VAL(p_buf, len, 5, 1, 1);
    if (ROHC_TRUE == p_csc->new_outer_nbo)
        ROHC_ENCODE_VAL(p_buf, len, 6, 1, 1);
    if (ROHC_TRUE == p_csc->ack_recvd)
        ROHC_ENCODE_VAL(p_buf, len, 7, 1, 1);

    len++ ;
                                
    p_buf[len++] = (rohc_U8bit_t) (p_csc->profile & 0xff);
    p_buf[len++] = (rohc_U8bit_t) (p_csc->c_mode & 0xff);
    p_buf[len++] = (rohc_U8bit_t) (p_csc->c_state & 0xff);
    p_buf[len++] = (rohc_U8bit_t) (p_csc->c_trans_state & 0xff);

    if (ROHC_NULL != p_csc->field_value[ROHC_RTP_TSS_FID - ROHC_VAR_FID_BASE])
    {
        ROHC_SET_32BIT(p_buf + len, p_csc->field_value[ROHC_RTP_TS_STRIDE_FID - ROHC_VAR_FID_BASE]);
        len += 4;
    }
                    
    if (ROHC_NULL != p_csc->field_value[ROHC_RTP_TSS_FID - ROHC_VAR_FID_BASE])
    {
        ROHC_SET_32BIT(p_buf+len, p_csc->ts_offset);
        len += 4;
    }

    if (ROHC_NULL != p_csc->field_value[ROHC_RTP_TIS_FID - ROHC_VAR_FID_BASE])
    {
        ROHC_SET_32BIT(p_buf+len, p_csc->field_value[ROHC_RTP_TIME_STRIDE_FID - ROHC_VAR_FID_BASE]);
        len += 4;
    }
                    
                            
    if (ROHC_NULL != p_csc->field_value[ROHC_RTP_TIS_FID - ROHC_VAR_FID_BASE])
    {
        ROHC_SET_32BIT(p_buf+len, p_csc->cur_time);
        len += 4;
    }
    
    if (ROHC_NULL != p_csc->field_value[ROHC_RTP_TIS_FID - ROHC_VAR_FID_BASE])
    {
        p_buf[len++] = (rohc_U8bit_t) (p_csc->max_jitter_cd & 0xff);
    }

    p_buf[len++] = (rohc_U8bit_t) (p_csc->longest_loss_event & 0xff);
    p_buf[len++] = (rohc_U8bit_t) (p_csc->clk_resolution & 0xff);
    p_buf[len++] = (rohc_U8bit_t) (p_csc->max_jitter & 0xff);
    
    p_buf[len++] = (rohc_U8bit_t) (p_csc->rohc_c_uo_csrc_sw.uo_csrc_ref_index & 0xff);
    
    p_buf[len++] = (rohc_U8bit_t) \
        (p_csc->rohc_c_uo_csrc_sw.uo_csrc_sw[0].gen_id & 0xff);
    
    p_buf[len++] = (rohc_U8bit_t) \
        (p_csc->rohc_c_uo_csrc_sw.uo_csrc_sw[0].gen_count & 0xff);
    
    p_buf[len++] = (rohc_U8bit_t) (p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_ref_index & 0xff);

    p_buf[len++] = (rohc_U8bit_t) \
        (p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_sw[0].gen_id & 0xff);
    
    p_buf[len++] = (rohc_U8bit_t) \
        (p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_sw[0].gen_count & 0xff);
                        
    ROHC_EXIT_FN("rohc_pack_dyn_csc_info");
    return len;
    
} /* End of function rohc_pack_dyn_csc_info */

/******************************************************************************
 **  FUNCTION:
 **       rohc_pack_dyn_dsc_info
 *******************************************************************************
 **  DESCRIPTION:
 **     This function packs the dynamic information of decompressor context for 
 **     an entity i.e.. it fills the dynamic info from decompressor context 
 **     structure to the buffer to be sent in ROHC_CONTEXT_CONF.
 **     
 **  INPUT:
 **     *p_dsc - pointer to decompressor context of an entity.
 **     *p_buf - The pointer to buffer to be filled.
 **     max_buf_len - max buffer length to be packed.
 **     *p_ecode - pointer to error code.
 **     
 **  RETURN:
 **     ROHC_SUCCESS
 **     
 ******************************************************************************/
rohc_len_t  rohc_pack_dyn_dsc_info   ROHC_PARAMS
(
    ( p_dsc, p_buf,  max_buf_len, p_ecode),
    rohc_dsc_t              *p_dsc      _AND_
    rohc_U8bit_t            *p_buf      _AND_
    rohc_U32bit_t           max_buf_len _AND_
    rohc_error_code_t            *p_ecode

)
{
    rohc_len_t      len = 0;
    /*rohc_U32bit_t   i;*/
    rohc_U8bit_t    index = 0;

    ROHC_ENTER_FN("rohc_pack_dyn_dsc_info");

    p_buf[len++] = 0;
    /* Below code is commented because (Ext header list and CSCR list ) features are */
    /* are not suppoted in the release */
    /*
    p_buf[len++] = ROHC_DSC_GET_R_CSRC_SW_NIDX(p_dsc) - ROHC_DSC_GET_R_CSRC_SW_REF_IDX(p_dsc);
    
    for(i = ROHC_DSC_GET_R_CSRC_SW_REF_IDX(p_dsc); i < ROHC_DSC_GET_R_CSRC_SW_NIDX(p_dsc); i++)
    {
        len += rohc_pk_rohc_r_sw_t((rohc_void_tp)&(p_dsc->rohc_d_r_csrc_sw.d_r_csrc_sw[i]), p_buf+len, max_buf_len-len, p_ecode);
    }

*/
    p_buf[len++] = 0;/*
    p_buf[len++] = ROHC_DSC_GET_R_EXT_SW_NIDX(p_dsc) - ROHC_DSC_GET_R_EXT_SW_REF_IDX(p_dsc);

    for(i = ROHC_DSC_GET_R_EXT_SW_REF_IDX(p_dsc); i < ROHC_DSC_GET_R_EXT_SW_NIDX(p_dsc); i++)
    {
        len += rohc_pk_rohc_r_sw_t((rohc_void_tp)&(p_dsc->rohc_d_r_ext_hdr_sw.d_r_ext_hdr_sw[i]), p_buf+len, max_buf_len-len, p_ecode);
    }

*/
    p_buf[len++] = 0;/*

    p_buf[len++] = ROHC_DSC_GET_UO_CSRC_SW_NIDX(p_dsc) - ROHC_DSC_GET_UO_CSRC_SW_REF_IDX(p_dsc);

    for(i = ROHC_DSC_GET_UO_CSRC_SW_REF_IDX(p_dsc); i < ROHC_DSC_GET_UO_CSRC_SW_NIDX(p_dsc); i++)
    {
        len += rohc_pk_rohc_d_uo_sw_t((rohc_void_tp)&(p_dsc->rohc_d_uo_csrc_sw.d_uo_csrc_sw[i]), p_buf+len, max_buf_len-len, p_ecode);
    }
*/
    p_buf[len++] = 0;/*


    p_buf[len++] = ROHC_DSC_GET_UO_EXT_SW_NIDX(p_dsc) - ROHC_DSC_GET_UO_EXT_SW_REF_IDX(p_dsc); 

    for(i = ROHC_DSC_GET_UO_EXT_SW_REF_IDX(p_dsc); i < ROHC_DSC_GET_UO_EXT_SW_NIDX(p_dsc); i++)
    {
        len += rohc_pk_rohc_d_uo_sw_t((rohc_void_tp)&(p_dsc->rohc_d_uo_ext_hdr_sw.d_uo_ext_hdr_sw[i]), p_buf+len, max_buf_len-len, p_ecode);
    }
*/
    p_buf[len++] = 0;/*

    p_buf[len++] = ROHC_DSC_GET_R_CSRC_TT_NIDX(p_dsc);

    for(i = 0; i < ROHC_DSC_GET_R_CSRC_TT_NIDX(p_dsc); i++)
    {
        len += rohc_pk_rohc_decomp_csrc_tt_t((rohc_void_tp)&(p_dsc->rohc_d_r_csrc_tt.d_r_csrc_tt[i]), p_buf+len, max_buf_len-len, p_ecode);
    }
*/
    p_buf[len++] = 0;/*


    p_buf[len++] = ROHC_DSC_GET_R_EXT_TT_NIDX(p_dsc);

    for(i = 0; i < ROHC_DSC_GET_R_EXT_TT_NIDX(p_dsc); i++)
    {
        p_buf[len++] = (rohc_U8bit_t) \
            (p_dsc->rohc_d_r_ext_hdr_tt.d_r_ext_hdr_tt[i].known & 0xff);
*/
        /* IMPORTANT : Delete the packing routine for the function rohc_pk_rohc_decomp_ext_hdr_tt_t,
         * every time you generate the pack unpack routine */

        /* packing of *p_item */
/*        len += rohc_pack_p_item(p_dsc->rohc_d_r_ext_hdr_tt.d_r_ext_hdr_tt[i].p_item, \
               p_buf+len);

    }


*/
    p_buf[len++] = 0;/*
    p_buf[len++] = ROHC_DSC_GET_UO_CSRC_TT_NIDX(p_dsc);

    for(i = 0; i < ROHC_DSC_GET_UO_CSRC_TT_NIDX(p_dsc); i++)
    {
        len += rohc_pk_rohc_decomp_csrc_tt_t((rohc_void_tp)&(p_dsc->rohc_d_uo_csrc_tt.d_uo_csrc_tt[i]), p_buf+len, max_buf_len-len, p_ecode);

    }

*/
    p_buf[len++] = 0;/*

    p_buf[len++] = ROHC_DSC_GET_UO_EXT_TT_NIDX(p_dsc);

    for(i = 0; i < ROHC_DSC_GET_UO_EXT_TT_NIDX(p_dsc); i++)
    {
        p_buf[len++] = (rohc_U8bit_t) \
            (p_dsc->rohc_d_uo_ext_hdr_tt.d_uo_ext_hdr_tt[i].known & 0xff);

*/        /* IMPORTANT : Delete the packing routine for the function rohc_pk_uo_ext_hdr_tt_t,
         * every time you generate the pack unpack routine */

        /* packing of *p_item */
  /*      len += rohc_pack_p_item(p_dsc->rohc_d_uo_ext_hdr_tt.d_uo_ext_hdr_tt[i].p_item, p_buf+len);

    }*/

    p_buf[len] = (rohc_U8bit_t)(0 & 0xff);
    if (ROHC_NULL != p_dsc->udp_cksum_present)
        ROHC_ENCODE_VAL(p_buf, len, 0, 1, 1);
    if (ROHC_NULL != p_dsc->tss)
        ROHC_ENCODE_VAL(p_buf, len, 1, 1, 1);
    if (ROHC_NULL != p_dsc->tis)
        ROHC_ENCODE_VAL(p_buf, len, 2, 1, 1);
    if (ROHC_TRUE == p_dsc->rnd)
        ROHC_ENCODE_VAL(p_buf, len, 3, 1, 1);
    if (ROHC_TRUE == p_dsc->rnd2)
        ROHC_ENCODE_VAL(p_buf, len, 4, 1, 1);
    if (ROHC_TRUE == p_dsc->nbo)
        ROHC_ENCODE_VAL(p_buf, len, 5, 1, 1);
    if (ROHC_TRUE == p_dsc->nbo2)
        ROHC_ENCODE_VAL(p_buf, len, 6, 1, 1);
    if (ROHC_TRUE == p_dsc->ack_recvd)
        ROHC_ENCODE_VAL(p_buf, len, 7, 1, 1);

    len++ ;
                                
    p_buf[len++] = (rohc_U8bit_t) (p_dsc->profile & 0xff);
    p_buf[len++] = (rohc_U8bit_t) (p_dsc->d_mode & 0xff);
    p_buf[len++] = (rohc_U8bit_t) (p_dsc->d_state & 0xff);
    p_buf[len++] = (rohc_U8bit_t) (p_dsc->d_trans_state & 0xff);

    if (ROHC_NULL != p_dsc->tss)
    {
        ROHC_SET_32BIT(p_buf+len, p_dsc->ts_stride);
        len += 4;
    }
                    
    if (ROHC_NULL != p_dsc->tss)
    {
        ROHC_SET_32BIT(p_buf+len, p_dsc->ts_offset);
        len +=4;
    }

    if (ROHC_NULL != p_dsc->tis)
    {
        ROHC_SET_32BIT(p_buf+len, p_dsc->time_stride);
        len += 4;
    }
                    
    if (ROHC_NULL != p_dsc->tis)
    {
        ROHC_SET_32BIT(p_buf+len, p_dsc->arv_tm_cur_pkt);
        len += 4;
    }
    
    p_buf[len++] = (rohc_U8bit_t) (p_dsc->longest_loss_event & 0xff);
    p_buf[len++] = (rohc_U8bit_t) (p_dsc->clock_resolution & 0xff);
    p_buf[len++] = (rohc_U8bit_t) (p_dsc->max_jitter & 0xff);
    
    index = ROHC_DSC_GET_UO_CSRC_SW_NIDX(p_dsc) - 1 ;
    
    p_buf[len++] = (rohc_U8bit_t) \
        (p_dsc->rohc_d_uo_csrc_sw.d_uo_csrc_sw[index].gen_id & 0xff);
    
    index = ROHC_DSC_GET_UO_EXT_SW_NIDX(p_dsc) - 1 ; 

    p_buf[len++] = (rohc_U8bit_t) \
        (p_dsc->rohc_d_uo_ext_hdr_sw.d_uo_ext_hdr_sw[index].gen_id & 0xff);
    
    ROHC_SET_32BIT(p_buf+len, p_dsc->sn_prev);
    len+=4;
    ROHC_SET_32BIT(p_buf+len, p_dsc->sn_prev_prev);
    len+=4;
    if(p_dsc->inner_ip4 != ROHC_NULL)
    {
        ROHC_SET_16BIT(p_buf+len, p_dsc->pre_inner_ip_id);
        len+=2;
    }
    if(p_dsc->outer_ip4 != ROHC_NULL)
    {
        ROHC_SET_16BIT(p_buf+len, p_dsc->pre_outer_ip_id);
        len+=2;
    }
    ROHC_EXIT_FN("rohc_pack_dyn_dsc_info");
    return len;
}

/******************************************************************************
 **  FUNCTION:
 **       rohc_unpack_dyn_csc_info
 *******************************************************************************
 **  DESCRIPTION:
 **     This function unpacks the dynamic information of compressor context for 
 **     an entity i.e.. it fills the dynamic info received in ROHC_CONFIG_REQ 
 **     to the compressor context for an entity.
 **     
 **  INPUT:
 **     *p_csc  =  pointer to the compressor context.
 **     **p_buf - The pointer to buffer to be filled.
 **     max_buf_len - max buffer length to be packed.
 **     *p_ecode - pointer to error code.
 **     
 **  RETURN:
 **     none
 **     
 ******************************************************************************/
rohc_len_t  rohc_unpack_dyn_csc_info ROHC_PARAMS
(
    ( p_csc, p_buf,  max_buf_len, p_ecode),
    rohc_csc_t              *p_csc      _AND_
    rohc_U8bit_t            *p_buf      _AND_
    rohc_U32bit_t           max_buf_len _AND_
    rohc_error_code_t            *p_ecode
)
{
    rohc_len_t      len = 0;
    rohc_U32bit_t   i, j;
    rohc_U16bit_t   sn_count;
    rohc_U8bit_t    count;

    ROHC_ENTER_FN("rohc_unpack_dyn_csc_info");

    count = p_buf[len++] ;

    for(i = 0; i < count; i++)
    {
        len += rohc_unpk_rohc_gsw_element_t((rohc_void_tp)&(p_csc->rohc_gsw.rohc_gsw_elem[i]), p_buf+len, max_buf_len-len, p_ecode);
        if(i == 0)
        {
            p_csc->rohc_gsw.min_inner_offset_i = 
                p_csc->rohc_gsw.rohc_gsw_elem[i].inner_offset_i;
            p_csc->rohc_gsw.max_inner_offset_i = 
                p_csc->rohc_gsw.rohc_gsw_elem[i].inner_offset_i;
            p_csc->rohc_gsw.min_outer_offset_i = 
                p_csc->rohc_gsw.rohc_gsw_elem[i].outer_offset_i;
            p_csc->rohc_gsw.max_outer_offset_i = 
                p_csc->rohc_gsw.rohc_gsw_elem[i].outer_offset_i;
            p_csc->rohc_gsw.min_sn = p_csc->rohc_gsw.rohc_gsw_elem[i].sn;
            p_csc->rohc_gsw.max_sn = p_csc->rohc_gsw.rohc_gsw_elem[i].sn;
            p_csc->rohc_gsw.min_ts_scaled = 
                p_csc->rohc_gsw.rohc_gsw_elem[i].ts_scaled;
            p_csc->rohc_gsw.max_ts_scaled = 
                p_csc->rohc_gsw.rohc_gsw_elem[i].ts_scaled;
        }
        else
        {
            if(p_csc->rohc_gsw.rohc_gsw_elem[i].inner_offset_i <= 
                p_csc->rohc_gsw.min_inner_offset_i) 
            {
                p_csc->rohc_gsw.min_inner_offset_i = 
                    p_csc->rohc_gsw.rohc_gsw_elem[i].inner_offset_i;
            }
            if(p_csc->rohc_gsw.rohc_gsw_elem[i].inner_offset_i >= 
                p_csc->rohc_gsw.max_inner_offset_i) 
            {
                p_csc->rohc_gsw.max_inner_offset_i = 
                    p_csc->rohc_gsw.rohc_gsw_elem[i].inner_offset_i;
            }
            if(p_csc->rohc_gsw.rohc_gsw_elem[i].outer_offset_i <= 
                p_csc->rohc_gsw.min_outer_offset_i)
            {
                p_csc->rohc_gsw.min_outer_offset_i = 
                    p_csc->rohc_gsw.rohc_gsw_elem[i].outer_offset_i;
            }
            if(p_csc->rohc_gsw.rohc_gsw_elem[i].outer_offset_i >= 
                p_csc->rohc_gsw.max_outer_offset_i)
            {
                p_csc->rohc_gsw.max_outer_offset_i = 
                    p_csc->rohc_gsw.rohc_gsw_elem[i].outer_offset_i;
            }
            if(p_csc->rohc_gsw.rohc_gsw_elem[i].sn <= p_csc->rohc_gsw.min_sn)
                p_csc->rohc_gsw.min_sn = p_csc->rohc_gsw.rohc_gsw_elem[i].sn;
            if(p_csc->rohc_gsw.rohc_gsw_elem[i].sn >= p_csc->rohc_gsw.max_sn)
                p_csc->rohc_gsw.max_sn = p_csc->rohc_gsw.rohc_gsw_elem[i].sn;
            if(p_csc->rohc_gsw.rohc_gsw_elem[i].ts_scaled <= 
                p_csc->rohc_gsw.min_ts_scaled)
            {
                p_csc->rohc_gsw.min_ts_scaled = 
                    p_csc->rohc_gsw.rohc_gsw_elem[i].ts_scaled;
            }
            if(p_csc->rohc_gsw.rohc_gsw_elem[i].ts_scaled >= 
                p_csc->rohc_gsw.max_ts_scaled)
            {
                p_csc->rohc_gsw.max_ts_scaled = 
                    p_csc->rohc_gsw.rohc_gsw_elem[i].ts_scaled;
            }
        }
    }
    p_csc->rohc_gsw.start_index=0;
    p_csc->rohc_gsw.end_index=count-1;
    p_csc->udp_sn = p_csc->rohc_gsw.max_sn;
    p_csc->rohc_gsw.iteration_flag = 1;
    count = p_buf[len++] ;
    
    for(i = 0; i < count; i++)
    {
        len += rohc_unpk_rohc_r_sw_t((rohc_void_tp)&(p_csc->rohc_c_r_csrc_sw.r_csrc_sw[i]), p_buf+len, max_buf_len-len, p_ecode);
    }
    p_csc->rohc_c_r_csrc_sw.r_csrc_ref_index=0;
    p_csc->rohc_c_r_csrc_sw.r_csrc_next_free_index=count;

    count = p_buf[len++];

    for(i = 0; i < count; i++)
    {
         len += rohc_unpk_rohc_r_sw_t((rohc_void_tp)&(p_csc->rohc_c_r_ext_hdr_sw.r_ext_hdr_sw[i]), p_buf+len, max_buf_len-len, p_ecode);
    }
    p_csc->rohc_c_r_ext_hdr_sw.r_ext_hdr_ref_index=0;
    p_csc->rohc_c_r_ext_hdr_sw.r_ext_hdr_next_free_index=count;

    count = p_buf[len++];

    for(i = 0; i < count; i++)
    {
        len += rohc_unpk_rohc_uo_sw_t((rohc_void_tp)&(p_csc->rohc_c_uo_csrc_sw.uo_csrc_sw[i]), p_buf+len, max_buf_len-len, p_ecode);
    }
    p_csc->rohc_c_uo_csrc_sw.uo_csrc_ref_index=0;
    p_csc->rohc_c_uo_csrc_sw.uo_csrc_next_free_index=count;

    count = p_buf[len++];

    for(i = 0; i < count; i++)
    {
        len += rohc_unpk_rohc_uo_sw_t((rohc_void_tp)&(p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_sw[i]), p_buf+len, max_buf_len-len, p_ecode);
    }
    p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_ref_index=0;
    p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_next_free_index=count;

    count = p_buf[len++] ;

    for(i = 0; i < count; i++)
    {
        len += rohc_unpk_r_csrc_tt_t((rohc_void_tp)&(p_csc->rohc_c_r_csrc_tt.r_csrc_tt[i].r_csrc_tt), p_buf+len, max_buf_len-len, p_ecode);

        /* IMPORTANT: Delete the unpacking routine for the array sn_no_list from the func 
         * rohc_unpk_r_csrc_tt_t, every time you generate the pack unpack routine */

        if ((len+2)>max_buf_len) 
        { 
            p_ecode = ROHC_FAILURE; 
            return len; 
        }

        sn_count =  p_buf[len++] ;

        for (j=0; j<sn_count; ++j)
        {
            if ((len+4)>max_buf_len) 
            { 
                p_ecode = ROHC_FAILURE; 
                return len; 
            } 
            
            p_csc->rohc_c_r_csrc_tt.r_csrc_tt[i].r_csrc_tt.sn_no_list[j] =
                ROHC_GET_2BYTES(p_buf+len);
            len += 2;
        }
        p_csc->rohc_c_r_csrc_tt.r_csrc_tt[i].r_sn_csrc_tt_next_free_index=(rohc_U8bit_t)sn_count;
    }
    p_csc->rohc_c_r_csrc_tt.r_csrc_tt_next_free_index=count;

    count = p_buf[len++] ;

    for(i = 0; i < count; i++)
    {
        p_csc->rohc_c_r_ext_hdr_tt.r_ext_hdr_tt[i].r_ext_hdr_tt.known = \
            p_buf[len++] ;

        /* IMPORTANT : Delete the unpacking routine for the function 
         * rohc_unpk_r_ext_hdr_tt_t, every time you generate the pack unpack routine */

        /* unpacking of *p_item */
        len += rohc_unpack_p_item(&(p_csc->rohc_c_r_ext_hdr_tt.r_ext_hdr_tt[i].r_ext_hdr_tt.p_item), p_buf+len);

        if ((len+2)>max_buf_len) 
        { 
            p_ecode = ROHC_FAILURE; 
            return len; 
        } 
        sn_count  = p_buf[len++] ;

        for (j=0; j<sn_count; ++j)
        {
            if ((len+4)>max_buf_len) 
            { 
                p_ecode = ROHC_FAILURE; 
                return len; 
            } 
            p_csc->rohc_c_r_ext_hdr_tt.r_ext_hdr_tt[i].r_ext_hdr_tt.sn_no_list[
j] =    ROHC_GET_2BYTES(p_buf+len);
            len += 2;
        }
        p_csc->rohc_c_r_ext_hdr_tt.r_ext_hdr_tt[i].r_sn_ext_hdr_tt_next_free_index=(rohc_U8bit_t)sn_count;
    }
     p_csc->rohc_c_r_ext_hdr_tt.r_ext_hdr_tt_next_free_index=count;

    count = p_buf[len++] ;

    for(i = 0; i < count; i++)
    {
        len += rohc_unpk_uo_csrc_tt_t((rohc_void_tp)&(p_csc->rohc_c_uo_csrc_tt.uo_csrc_tt[i]), p_buf+len, max_buf_len-len, p_ecode);

    }
    p_csc->rohc_c_uo_csrc_tt.uo_csrc_tt_next_free_index=count;

    count = p_buf[len++] ;

    for(i = 0; i < count; i++)
    {
        p_csc->rohc_c_uo_ext_hdr_tt.uo_ext_hdr_tt[i].known = p_buf[len++];

        /* IMPORTANT : Delete the unpacking routine for the function 
         * rohc_unpk_uo_ext_hdr_tt_t, every time you generate the pack 
         * unpack routine */

        /* unpacking of *p_item */
        len += rohc_unpack_p_item(&(p_csc->rohc_c_uo_ext_hdr_tt.uo_ext_hdr_tt[i].p_item), p_buf+len);

        p_csc->rohc_c_uo_ext_hdr_tt.uo_ext_hdr_tt[i].counter = p_buf[len++];
    }
    p_csc->rohc_c_uo_ext_hdr_tt.uo_ext_hdr_tt_next_free_index=count;

    if(ROHC_DECODE_VAL(p_buf, len, 0, 1))
        p_csc->static_delta |= ROHC_UDP_CHECK_SUM_PRESENT;
    p_csc->field_value[ROHC_RTP_TSS_FID - ROHC_VAR_FID_BASE] = ROHC_DECODE_VAL(p_buf, len, 1, 1);
    p_csc->field_value[ROHC_RTP_TIS_FID - ROHC_VAR_FID_BASE] = ROHC_DECODE_VAL(p_buf, len, 2, 1);
    /* asuming that the context at the source RNC has alread been established */     /* and communicated to the peer side entity */
    p_csc->old_inner_rnd =
    p_csc->new_inner_rnd = ROHC_DECODE_VAL(p_buf, len, 3, 1);
    p_csc->old_outer_rnd =
    p_csc->new_outer_rnd = ROHC_DECODE_VAL(p_buf, len, 4, 1);
    p_csc->old_inner_nbo =
    p_csc->new_inner_nbo = ROHC_DECODE_VAL(p_buf, len, 5, 1);
    p_csc->old_outer_nbo =
    p_csc->new_outer_nbo = ROHC_DECODE_VAL(p_buf, len, 6, 1);
    p_csc->ack_recvd = ROHC_DECODE_VAL(p_buf, len, 7, 1);

    len++ ;
                                
    p_csc->profile  = p_buf[len++] ;
    /* Assuming here that the profile is already determined */
    p_csc->profile_determined = ROHC_TRUE;

    p_csc->c_mode = p_buf[len++] ;
    /*  Removed as according to specs the first packet which should go after context
     *  relocation should be IR and for that state should be IR */ 
    p_csc->c_state = p_buf[len++]; 
    if(p_csc->c_state == ROHC_C_ST_SO)
        p_csc->c_state = ROHC_C_ST_FO;
    /*p_csc->c_state = ROHC_C_ST_IR;*/
    p_csc->send_irdyn_flag = ROHC_TRUE;
    p_csc->c_trans_state = p_buf[len++] ;

    if (ROHC_NULL != p_csc->field_value[ROHC_RTP_TSS_FID - ROHC_VAR_FID_BASE])
    {
        p_csc->ts_stride = p_csc->field_value[ROHC_RTP_TS_STRIDE_FID - 
                        ROHC_VAR_FID_BASE] = ROHC_GET_4BYTES(p_buf+len);
        len += 4;
    }
                    
    if (ROHC_NULL != p_csc->field_value[ROHC_RTP_TSS_FID - ROHC_VAR_FID_BASE])
    {
        p_csc->ts_offset = ROHC_GET_4BYTES(p_buf + len);
        len += 4;
    }

    if (ROHC_NULL != p_csc->field_value[ROHC_RTP_TIS_FID - ROHC_VAR_FID_BASE])
    {
        p_csc->time_stride = p_csc->field_value[ROHC_RTP_TIME_STRIDE_FID - ROHC_VAR_FID_BASE] = ROHC_GET_4BYTES(p_buf + len);
        len += 4;
    }
                    
                            
    if (ROHC_NULL != p_csc->field_value[ROHC_RTP_TIS_FID - ROHC_VAR_FID_BASE])
    {
        p_csc->cur_time = ROHC_GET_4BYTES(p_buf + len);
        len += 4;
    }
    
    if (ROHC_NULL != p_csc->field_value[ROHC_RTP_TIS_FID - ROHC_VAR_FID_BASE])
    {
        p_csc->max_jitter_cd = p_buf[len++] ;
    }

    p_csc->longest_loss_event = p_buf[len++] ;
    p_csc->clk_resolution = p_buf[len++] ;
    p_csc->max_jitter = p_buf[len++] ;
    
    p_csc->rohc_c_uo_csrc_sw.uo_csrc_ref_index = p_buf[len++];
    
    p_csc->rohc_c_uo_csrc_sw.uo_csrc_sw[0].gen_id = p_buf[len++];
    
    p_csc->rohc_c_uo_csrc_sw.uo_csrc_sw[0].gen_count = p_buf[len++];
    
    p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_ref_index = p_buf[len++];

    p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_sw[0].gen_id = p_buf[len++]; 
    p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_sw[0].gen_count = p_buf[len++] ;
                        
    ROHC_EXIT_FN("rohc_pack_dyn_csc_info");
    return len;
}

/*******************************************************************************
**  FUNCTION :  rohc_unpack_dyn_dsc_info
********************************************************************************
**  DESCRIPTION :
**      This function is the entry point for the RFC3095 Compression entity. 
**
**  INPUT:
**     	*p_csc  =  pointer to the compressor context.
**     	*p_buf - The pointer to buffer to be filled.
**     	max_buf_len - max buffer length to be packed.
**     	*p_ecode - pointer to error code.
**
**  RETURNS:
**      Length
**
*******************************************************************************/
rohc_len_t  rohc_unpack_dyn_dsc_info ROHC_PARAMS
(
    ( p_dsc, p_buf,  max_buf_len, p_ecode),
    rohc_dsc_t              *p_dsc      _AND_
    rohc_U8bit_t            *p_buf      _AND_
    rohc_U32bit_t           max_buf_len _AND_
    rohc_error_code_t            *p_ecode
)
{
    rohc_len_t      len = 0;
    rohc_U32bit_t   i;
    rohc_U8bit_t    count = 0;

    ROHC_ENTER_FN("rohc_unpack_dyn_dsc_info");

    count = p_buf[len++] ;
    
    for(i = 0; i < count; i++)
    {
        len += rohc_unpk_rohc_r_sw_t((rohc_void_tp)&(p_dsc->rohc_d_r_csrc_sw.d_r_csrc_sw[i]), p_buf+len, max_buf_len-len, p_ecode);
    }

    count = p_buf[len++] ;

    for(i = 0; i < count; i++)
    {
        len += rohc_unpk_rohc_r_sw_t((rohc_void_tp)&(p_dsc->rohc_d_r_ext_hdr_sw.d_r_ext_hdr_sw[i]), p_buf+len, max_buf_len-len, p_ecode);
    }

    count = p_buf[len++] ;

    for(i = 0; i < count; i++)
    {
        len += rohc_unpk_rohc_d_uo_sw_t((rohc_void_tp)&(p_dsc->rohc_d_uo_csrc_sw.d_uo_csrc_sw[i]), p_buf+len, max_buf_len-len, p_ecode);
    }

    count = p_buf[len++] ;

    for(i = 0; i < count; i++)
    {
        len += rohc_unpk_rohc_d_uo_sw_t((rohc_void_tp)&(p_dsc->rohc_d_uo_ext_hdr_sw.d_uo_ext_hdr_sw[i]), p_buf+len, max_buf_len-len, p_ecode);
    }

    count = p_buf[len++] ;

    for(i = 0; i < count; i++)
    {
        len += rohc_unpk_rohc_decomp_csrc_tt_t((rohc_void_tp)&(p_dsc->rohc_d_r_csrc_tt.d_r_csrc_tt[i]), p_buf+len, max_buf_len-len, p_ecode);
    }

    count = p_buf[len++] ;

    for(i = 0; i < count; i++)
    {
        p_dsc->rohc_d_r_ext_hdr_tt.d_r_ext_hdr_tt[i].known = \
            p_buf[len++] ;

        /* IMPORTANT : Delete the unpacking routine for the function 
         * rohc_unpk_rohc_decomp_ext_hdr_tt_t, every time you generate 
         * the pack unpack routine */

        /* unpacking of *p_item */
        len += rohc_unpack_p_item(&(p_dsc->rohc_d_r_ext_hdr_tt.d_r_ext_hdr_tt[i].p_item), p_buf+len);

    }

    count = p_buf[len++] ;

    for(i = 0; i < count; i++)
    {
        len += rohc_unpk_rohc_decomp_csrc_tt_t((rohc_void_tp)&(p_dsc->rohc_d_uo_csrc_tt.d_uo_csrc_tt[i]), p_buf+len, max_buf_len-len, p_ecode);

    }

    count = p_buf[len++] ;

    for(i = 0; i < count; i++)
    {
        p_dsc->rohc_d_uo_ext_hdr_tt.d_uo_ext_hdr_tt[i].known = p_buf[len++] ;

        /* IMPORTANT : Delete the unpacking routine for the function
         * rohc_unpk_uo_ext_hdr_tt_t, every time you generate the 
         * pack unpack routine */

        /* unpacking of *p_item */
        len += rohc_unpack_p_item(&(p_dsc->rohc_d_uo_ext_hdr_tt.d_uo_ext_hdr_tt[i].p_item), p_buf+len);

    }

    p_dsc->udp_cksum_present = ROHC_DECODE_VAL(p_buf, len, 0, 1);
    p_dsc->tss = ROHC_DECODE_VAL(p_buf, len, 1, 1);
    p_dsc->tis = ROHC_DECODE_VAL(p_buf, len, 2, 1);
    p_dsc->rnd = ROHC_DECODE_VAL(p_buf, len, 3, 1);
    p_dsc->rnd2 = ROHC_DECODE_VAL(p_buf, len, 4, 1);
    p_dsc->nbo = ROHC_DECODE_VAL(p_buf, len, 5, 1);
    p_dsc->nbo2 = ROHC_DECODE_VAL(p_buf, len, 6, 1);
    p_dsc->ack_recvd = ROHC_DECODE_VAL(p_buf, len, 7, 1);

    if(ROHC_NULL == p_dsc->inner_ip4 )
        p_dsc->rnd =2;
    
    if(ROHC_NULL == p_dsc->outer_ip4) 
        p_dsc->rnd2 =2;

    len++ ;
    p_dsc->profile = p_buf[len++];
    p_dsc->d_mode  = p_buf[len++];
    p_dsc->d_state = p_buf[len++];
    p_dsc->d_trans_state = p_buf[len++];

    if (ROHC_NULL != p_dsc->tss)
    {
        p_dsc->ts_stride = ROHC_GET_4BYTES(p_buf + len);
        len += 4;
    }
                    
    if (ROHC_NULL != p_dsc->tss)
    {
        p_dsc->ts_offset = ROHC_GET_4BYTES(p_buf+len);
        len += 4;
    }

    if (ROHC_NULL != p_dsc->tis)
    {
        p_dsc->time_stride = ROHC_GET_4BYTES(p_buf + len);
        len += 4;
    }
                    
    if (ROHC_NULL != p_dsc->tis)
    {
        p_dsc->arv_tm_cur_pkt = ROHC_GET_4BYTES(p_buf + len);   
        len += 4;
    }
    
    p_dsc->longest_loss_event = p_buf[len++];
    p_dsc->clock_resolution = p_buf[len++];
    p_dsc->max_jitter = p_buf[len++];
    
    p_dsc->rohc_d_uo_csrc_sw.d_uo_csrc_sw[0].gen_id = p_buf[len++];
    
    p_dsc->rohc_d_uo_ext_hdr_sw.d_uo_ext_hdr_sw[0].gen_id = p_buf[len++];
    
    p_dsc->sn_prev = ROHC_GET_4BYTES(p_buf + len);
    len+=4;
    p_dsc->sn_prev_prev = ROHC_GET_4BYTES(p_buf + len);
    len+=4;
    if(p_dsc->inner_ip4 != ROHC_NULL)
    {
        p_dsc->pre_inner_ip_id = ROHC_GET_2BYTES(p_buf + len);
        len+=2;
    }   
    if(p_dsc->outer_ip4 != ROHC_NULL)
    {
        p_dsc->pre_outer_ip_id = ROHC_GET_2BYTES(p_buf + len);
        len+=2;
    }   

    ROHC_EXIT_FN("rohc_pack_dyn_dsc_info");
    return len;
}

/*******************************************************************************
**  FUNCTION :  rohc_pack_p_item
********************************************************************************
**  DESCRIPTION :
**      This function is used to pack items. 
**
**  INPUT:
**		p_item - Pointer to items
**      p_buf - Pointer to buffer
**  RETURNS:
**      Length
**
*******************************************************************************/

rohc_len_t  rohc_pack_p_item     ROHC_PARAMS
(
    ( p_item, p_buf),
    rohc_U8bit_t            *p_item     _AND_
    rohc_U8bit_t            *p_buf  
)
{
    rohc_U8bit_t    len = 0 ;
    ROHC_ENTER_FN("rohc_pack_p_item");

    p_buf[len++] = ((rohc_protocol_node_t *)p_item)->protocol;
    p_buf[len++] = ((rohc_protocol_node_t *)p_item)->length;
            
    ROHC_MEMCPY(p_buf+len, ((rohc_protocol_node_t *)p_item)->ptr, \
                    ((rohc_protocol_node_t *)p_item)->length);

    len += ((rohc_protocol_node_t *)p_item)->length;

    ROHC_EXIT_FN("rohc_pack_p_item");
    return len;
}

/*******************************************************************************
**  FUNCTION :  rohc_unpack_p_item
********************************************************************************
**  DESCRIPTION :
**      This function is used to unpack items. 
**
**  INPUT:
**		p_item - Pointer to Pointer of items
**      p_buf - Pointer to buffer
**  RETURNS:
**      Length
**
*******************************************************************************/

rohc_len_t  rohc_unpack_p_item   ROHC_PARAMS
(
    ( p_p_item, p_buf),
    rohc_U8bit_t            **p_p_item  _AND_
    rohc_U8bit_t            *p_buf  
)
{
    rohc_U8bit_t    len=0;
    rohc_protocol_node_t *p_item;
    ROHC_ENTER_FN("rohc_unpack_p_item");

    p_item = (rohc_protocol_node_t *)ROHC_MALLOC(sizeof(rohc_protocol_node_t));
    *p_p_item = (rohc_U8bit_t *)p_item;
    p_item->protocol = p_buf[len++];
    p_item->length = p_buf[len++];
    
    p_item->ptr = (rohc_U8bit_t *)ROHC_MALLOC(p_item->length);
        
    ROHC_MEMCPY(p_item->ptr, p_buf+len,p_item->length);

    len += p_item->length;
                        
    ROHC_EXIT_FN("rohc_unpack_p_item");
    return len;
}
#endif
