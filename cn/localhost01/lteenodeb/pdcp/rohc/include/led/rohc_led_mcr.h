/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_led_mcr.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_led_mcr.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.2  2009/05/28 05:20:12  gur19836
 * File header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_LED_MCR_H_
#define _ROHC_LED_MCR_H_

/****************************************
**	Compressor Side 	*****************
****************************************/

/* R Mode EXT HDR macros for Sliding Window */

#define ROHC_CSC_GET_R_EXT_SW_NIDX(p_csc_m) \
    (p_csc_m->rohc_c_r_ext_hdr_sw.r_ext_hdr_next_free_index)
#define ROHC_CSC_SET_R_EXT_SW_NIDX(p_csc_m, index_m) \
    (p_csc_m->rohc_c_r_ext_hdr_sw.r_ext_hdr_next_free_index = index_m)

#define ROHC_CSC_GET_R_EXT_SW_RIDX(p_csc_m) \
    (p_csc_m->rohc_c_r_ext_hdr_sw.r_ext_hdr_ref_index)
#define ROHC_CSC_SET_R_EXT_SW_RIDX(p_csc_m, index_m) \
    (p_csc_m->rohc_c_r_ext_hdr_sw.r_ext_hdr_ref_index = index_m)

#define ROHC_CSC_GET_R_EXT_SW_REF(p_csc_m) \
    ((p_csc_m)->rohc_c_r_ext_hdr_sw.r_ext_hdr_sw[(p_csc_m)->rohc_c_r_ext_hdr_sw.r_ext_hdr_ref_index])
#define ROHC_CSC_GET_R_EXT_SW_NEXT(p_csc_m) \
    ((p_csc_m)->rohc_c_r_ext_hdr_sw.r_ext_hdr_sw[(p_csc_m)->rohc_c_r_ext_hdr_sw.r_ext_hdr_next_free_index])

#define ROHC_CSC_GET_R_EXT_SW(p_csc_m, index_m) \
((p_csc_m)->rohc_c_r_ext_hdr_sw.r_ext_hdr_sw[index_m])

#define ROHC_CSC_GET_R_EXT_SW_ACKED(p_csc_m) \
	(p_csc_m->rohc_c_r_ext_hdr_sw.ref_id_present)

/* R Mode EXT HDR macros for Translation Table */

#define ROHC_CSC_GET_R_EXT_TT(p_csc_m, index_m) \
    (p_csc_m->rohc_c_r_ext_hdr_tt.r_ext_hdr_tt[index_m].r_ext_hdr_tt)
#define ROHC_CSC_GET_R_EXT_TT_SN_IDX(p_csc_m, index_m) \
    (p_csc_m->rohc_c_r_ext_hdr_tt.r_ext_hdr_tt[index_m].r_sn_ext_hdr_tt_next_free_index)
#define ROHC_CSC_INCR_R_EXT_TT_SN_IDX(p_csc_m, index_m) \
    ((p_csc_m->rohc_c_r_ext_hdr_tt.r_ext_hdr_tt[index_m].r_sn_ext_hdr_tt_next_free_index)++)

#define ROHC_CSC_GET_R_EXT_TT_NIDX(p_csc_m) \
    (p_csc_m->rohc_c_r_ext_hdr_tt.r_ext_hdr_tt_next_free_index)
#define ROHC_CSC_SET_R_EXT_TT_NIDX(p_csc_m, index_m) \
	(p_csc_m->rohc_c_r_ext_hdr_tt.r_ext_hdr_tt_next_free_index = index_m)

#define ROHC_CSC_GET_R_EXT_TT_NEXT(p_csc_m) \
    ((p_csc_m)->rohc_c_r_ext_hdr_tt.r_ext_hdr_tt[(p_csc_m)->rohc_c_r_ext_hdr_tt.r_ext_hdr_tt_next_free_index].r_ext_hdr_tt)

/* R Mode CSRC macros for Sliding Window */

#define ROHC_CSC_GET_R_CSRC_SW_REF(p_csc_m) \
    ((p_csc_m)->rohc_c_r_csrc_sw.r_csrc_sw[(p_csc_m)->rohc_c_r_csrc_sw.r_csrc_ref_index])
#define ROHC_CSC_GET_R_CSRC_SW_NEXT(p_csc_m) \
    ((p_csc_m)->rohc_c_r_csrc_sw.r_csrc_sw[(p_csc_m)->rohc_c_r_csrc_sw.r_csrc_next_free_index])

#define ROHC_CSC_GET_R_CSRC_SW_RIDX(p_csc_m) \
    (p_csc_m->rohc_c_r_csrc_sw.r_csrc_ref_index)
#define ROHC_CSC_SET_R_CSRC_SW_RIDX(p_csc_m, index_m) \
    (p_csc_m->rohc_c_r_csrc_sw.r_csrc_ref_index = index_m)

#define ROHC_CSC_GET_R_CSRC_SW_NIDX(p_csc_m) \
    ((p_csc_m)->rohc_c_r_csrc_sw.r_csrc_next_free_index)
#define ROHC_CSC_SET_R_CSRC_SW_NIDX(p_csc_m, index_m) \
    (p_csc_m->rohc_c_r_csrc_sw.r_csrc_next_free_index = index_m)

#define ROHC_CSC_GET_R_CSRC_SW(p_csc_m, index_m) \
((p_csc_m)->rohc_c_r_csrc_sw.r_csrc_sw[index_m])

#define ROHC_CSC_GET_R_CSRC_SW_ACKED(p_csc_m) \
	(p_csc_m->rohc_c_r_csrc_sw.ref_id_present)

/* R Mode CSRC macros for Translation Table */

#define ROHC_CSC_GET_R_CSRC_TT(p_csc_m, index_m) \
    (p_csc_m->rohc_c_r_csrc_tt.r_csrc_tt[index_m].r_csrc_tt)
#define ROHC_CSC_GET_R_CSRC_TT_SN_IDX(p_csc_m, index_m) \
    (p_csc_m->rohc_c_r_csrc_tt.r_csrc_tt[index_m].r_sn_csrc_tt_next_free_index)
#define ROHC_CSC_INCR_R_CSRC_TT_SN_IDX(p_csc_m, index_m) \
    ((p_csc_m->rohc_c_r_csrc_tt.r_csrc_tt[index_m].r_sn_csrc_tt_next_free_index)++)

#define ROHC_CSC_GET_R_CSRC_TT_NIDX(p_csc_m) \
    (p_csc_m->rohc_c_r_csrc_tt.r_csrc_tt_next_free_index)
#define ROHC_CSC_SET_R_CSRC_TT_NIDX(p_csc_m, index_m) \
    (p_csc_m->rohc_c_r_csrc_tt.r_csrc_tt_next_free_index = index_m)

#define ROHC_CSC_GET_R_CSRC_TT_NEXT(p_csc_m) \
    ((p_csc_m)->rohc_c_r_csrc_tt.r_csrc_tt[(p_csc_m)->rohc_c_r_csrc_tt.r_csrc_tt_next_free_index].r_csrc_tt)

/* U/O Mode EXT HDR macros for Sliding Window */

#define ROHC_CSC_GET_UO_EXT_SW_NIDX(p_csc_m) \
    (p_csc_m->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_next_free_index)
#define ROHC_CSC_SET_UO_EXT_SW_NIDX(p_csc_m, index_m) \
    (p_csc_m->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_next_free_index = index_m)

#define ROHC_CSC_GET_UO_EXT_SW_RIDX(p_csc_m) \
    (p_csc_m->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_ref_index)
#define ROHC_CSC_SET_UO_EXT_SW_RIDX(p_csc_m, index_m) \
    (p_csc_m->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_ref_index = index_m)

#define ROHC_CSC_GET_UO_EXT_SW_REF(p_csc_m) \
    (p_csc_m->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_sw[p_csc_m->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_ref_index])
#define ROHC_CSC_GET_UO_EXT_SW_NEXT(p_csc_m) \
    ((p_csc_m)->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_sw[(p_csc_m)->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_next_free_index])

#define ROHC_CSC_GET_UO_EXT_SW(p_csc_m, index_m) \
((p_csc_m)->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_sw[index_m])

#define ROHC_CSC_GET_UO_EXT_SW_ACKED(p_csc_m) \
	(p_csc_m->rohc_c_uo_ext_hdr_sw.ref_id_present)
#define ROHC_CSC_SET_UO_EXT_SW_ACKED(p_csc_m) \
	(p_csc_m->rohc_c_uo_ext_hdr_sw.ref_id_present = ROHC_TRUE)

/* U/O Mode EXT HDR macros for Translation Table */

#define ROHC_CSC_GET_UO_EXT_TT(p_csc_m, index_m) \
    (p_csc_m->rohc_c_uo_ext_hdr_tt.uo_ext_hdr_tt[index_m])
#define ROHC_CSC_GET_UO_EXT_TT_NEXT(p_csc_m) \
    ((p_csc_m)->rohc_c_uo_ext_hdr_tt.uo_ext_hdr_tt[(p_csc_m)->rohc_c_uo_ext_hdr_tt.uo_ext_hdr_tt_next_free_index])

#define ROHC_CSC_GET_UO_EXT_TT_NIDX(p_csc_m) \
    (p_csc_m->rohc_c_uo_ext_hdr_tt.uo_ext_hdr_tt_next_free_index)
#define ROHC_CSC_SET_UO_EXT_TT_NIDX(p_csc_m, index_m) \
    (p_csc_m->rohc_c_uo_ext_hdr_tt.uo_ext_hdr_tt_next_free_index = index_m)



/* U/O Mode CSRC macros for Sliding Window */

#define ROHC_CSC_GET_UO_CSRC_SW_NIDX(p_csc_m) \
    (p_csc_m->rohc_c_uo_csrc_sw.uo_csrc_next_free_index)
#define ROHC_CSC_SET_UO_CSRC_SW_NIDX(p_csc_m, index_m) \
    (p_csc_m->rohc_c_uo_csrc_sw.uo_csrc_next_free_index = index_m)

#define ROHC_CSC_GET_UO_CSRC_SW_RIDX(p_csc_m) \
    (p_csc_m->rohc_c_uo_csrc_sw.uo_csrc_ref_index)
#define ROHC_CSC_SET_UO_CSRC_SW_RIDX(p_csc_m, index_m) \
    (p_csc_m->rohc_c_uo_csrc_sw.uo_csrc_ref_index = index_m)

#define ROHC_CSC_GET_UO_CSRC_SW_REF(p_csc_m) \
    ((p_csc_m)->rohc_c_uo_csrc_sw.uo_csrc_sw[(p_csc_m)->rohc_c_uo_csrc_sw.uo_csrc_ref_index])
#define ROHC_CSC_GET_UO_CSRC_SW_NEXT(p_csc_m) \
    ((p_csc_m)->rohc_c_uo_csrc_sw.uo_csrc_sw[(p_csc_m)->rohc_c_uo_csrc_sw.uo_csrc_next_free_index])

#define ROHC_CSC_GET_UO_CSRC_SW(p_csc_m, index_m) \
((p_csc_m)->rohc_c_uo_csrc_sw.uo_csrc_sw[index_m])

#define ROHC_CSC_GET_UO_CSRC_SW_ACKED(p_csc_m) \
	(p_csc_m->rohc_c_uo_csrc_sw.ref_id_present)
#define ROHC_CSC_SET_UO_CSRC_SW_ACKED(p_csc_m) \
	(p_csc_m->rohc_c_uo_csrc_sw.ref_id_present = ROHC_TRUE)

/* U/O Mode CSRC macros for Translation Table */

#define ROHC_CSC_GET_UO_CSRC_TT_NEXT(p_csc_m) \
    ((p_csc_m)->rohc_c_uo_csrc_tt.uo_csrc_tt[(p_csc_m)->rohc_c_uo_csrc_tt.uo_csrc_tt_next_free_index])
#define ROHC_CSC_GET_UO_CSRC_TT(p_csc_m, index_m) \
    (p_csc_m->rohc_c_uo_csrc_tt.uo_csrc_tt[index_m])

#define ROHC_CSC_GET_UO_CSRC_TT_NIDX(p_csc_m) \
    (p_csc_m->rohc_c_uo_csrc_tt.uo_csrc_tt_next_free_index)
#define ROHC_CSC_SET_UO_CSRC_TT_NIDX(p_csc_m, index_m) \
    (p_csc_m->rohc_c_uo_csrc_tt.uo_csrc_tt_next_free_index = index_m)

/* Get next index in generic option elements structure */
#define ROHC_CSC_GET_GENERIC_ELEM_INDEX(p_generic_opt_m) \
    ((p_generic_opt_m)->rohc_generic_opt_elem[ \
     (p_generic_opt_m)->next_free_index])

#define ROHC_CSC_SET_GENERIC_ELEM_INDEX(p_generic_opt_m) \
    ((p_generic_opt_m)->next_free_index= \
     (p_generic_opt_m)->next_free_index+1)

#define ROHC_CSC_GET_GENERIC_OPT_VAL(p_csc_m, i) \
    ((p_csc_m)->rohc_c_tcp_opt.rohc_c_generic_opt.rohc_generic_opt_elem[i].\
     opt_val)
/* Miscellaneous */


#define ROHC_SET_BIT(x, n) \
    (x | (1 << (n-1)))

#define ROHC_GET_BIT(x, n) \
    (x & (1 << (n-1)))


#define ROHC_UNSET_BIT(x, n) \
	(x & (~(1 << (n-1))))

/* SPR 3532 - Klocwork Fix */
#define ROHC_CSC_GET_SN_NO(p_csc_m) \
	(p_csc_m->field_value[ROHC_SN_FID - ROHC_VAR_FID_BASE])

/****************************************
**	DeCompressor Side 	*****************
****************************************/
/* R Mode EXT HDR macros for Sliding Window */

#define ROHC_DSC_GET_R_EXT_SW_NIDX(p_dsc_m) \
    (p_dsc_m->rohc_d_r_ext_hdr_sw.r_ext_hdr_sw_next_free_index)
#define ROHC_DSC_SET_R_EXT_SW_NIDX(p_dsc_m, index_m) \
    (p_dsc_m->rohc_d_r_ext_hdr_sw.r_ext_hdr_sw_next_free_index = index_m)

#define ROHC_DSC_GET_R_EXT_SW_NEXT(p_dsc_m) \
    ((p_dsc_m)->rohc_d_r_ext_hdr_sw.d_r_ext_hdr_sw[(p_dsc_m)->rohc_d_r_ext_hdr_sw.r_ext_hdr_sw_next_free_index])

#define ROHC_DSC_GET_R_EXT_SW(p_dsc_m, index_m) \
	((p_dsc_m)->rohc_d_r_ext_hdr_sw.d_r_ext_hdr_sw[index_m])

#define ROHC_DSC_SET_R_EXT_SW_REF_IDX(p_dsc_m, ref_idx_m) \
	((p_dsc_m)->rohc_d_r_ext_hdr_sw.r_ext_hdr_sw_ref_index = ref_idx_m)

#define ROHC_DSC_GET_R_EXT_SW_REF_IDX(p_dsc_m) \
	((p_dsc_m)->rohc_d_r_ext_hdr_sw.r_ext_hdr_sw_ref_index)

#define ROHC_DSC_GET_R_EXT_RIDX(p_dsc_m, ref_id_m) \
	((p_dsc_m)->map_r_ext_ref_id_n_idx[ref_id_m])

/* R Mode EXT HDR macros for Translation Table */

#define ROHC_DSC_GET_R_EXT_TT(p_dsc_m, index_m) \
    (p_dsc_m->rohc_d_r_ext_hdr_tt.d_r_ext_hdr_tt[index_m])

#define ROHC_DSC_GET_R_EXT_TT_NIDX(p_dsc_m) \
    (p_dsc_m->rohc_d_r_ext_hdr_tt.r_ext_hdr_tt_next_free_index)
#define ROHC_DSC_SET_R_EXT_TT_NIDX(p_dsc_m, index_m) \
	(p_dsc_m->rohc_d_r_ext_hdr_tt.r_ext_hdr_tt_next_free_index = index_m)

#define ROHC_DSC_GET_R_EXT_TT_NEXT(p_dsc_m) \
    ((p_dsc_m)->rohc_d_r_ext_hdr_tt.d_r_ext_hdr_tt[(p_dsc_m)->rohc_d_r_ext_hdr_tt.r_ext_hdr_tt_next_free_index])

/* R Mode CSRC macros for Sliding Window */

#define ROHC_DSC_GET_R_CSRC_SW_NEXT(p_dsc_m) \
    ((p_dsc_m)->rohc_d_r_csrc_sw.d_r_csrc_sw[(p_dsc_m)->rohc_d_r_csrc_sw.r_csrc_sw_next_free_index])

#define ROHC_DSC_GET_R_CSRC_SW_NIDX(p_dsc_m) \
    ((p_dsc_m)->rohc_d_r_csrc_sw.r_csrc_sw_next_free_index)
#define ROHC_DSC_SET_R_CSRC_SW_NIDX(p_dsc_m, index_m) \
    (p_dsc_m->rohc_d_r_csrc_sw.r_csrc_sw_next_free_index = index_m)

#define ROHC_DSC_GET_R_CSRC_SW(p_dsc_m, index_m) \
	((p_dsc_m)->rohc_d_r_csrc_sw.d_r_csrc_sw[index_m])

#define ROHC_DSC_SET_R_CSRC_SW_REF_IDX(p_dsc_m, ref_idx_m) \
	((p_dsc_m)->rohc_d_r_csrc_sw.r_csrc_sw_ref_index = ref_idx_m)
#define ROHC_DSC_GET_R_CSRC_SW_REF_IDX(p_dsc_m) \
	((p_dsc_m)->rohc_d_r_csrc_sw.r_csrc_sw_ref_index)

/*
#define ROHC_DSC_GET_R_CSRC_RIDX(p_dsc_m, ref_id_m) \ 
	((p_dsc_m)->map_r_csrc_ref_id_n_idx[ref_id_m])
*/

/* R Mode CSRC macros for Translation Table */

#define ROHC_DSC_GET_R_CSRC_TT(p_dsc_m, index_m) \
    (p_dsc_m->rohc_d_r_csrc_tt.d_r_csrc_tt[index_m].r_csrc_tt)

#define ROHC_DSC_GET_R_CSRC_TT_NIDX(p_dsc_m) \
    (p_dsc_m->rohc_d_r_csrc_tt.r_csrc_tt_next_free_index)
#define ROHC_DSC_SET_R_CSRC_TT_NIDX(p_dsc_m, index_m) \
    (p_dsc_m->rohc_d_r_csrc_tt.r_csrc_tt_next_free_index = index_m)

#define ROHC_DSC_GET_R_CSRC_TT_NEXT(p_dsc_m) \
    ((p_dsc_m)->rohc_d_r_csrc_tt.d_r_csrc_tt[(p_dsc_m)->rohc_d_r_csrc_tt.r_csrc_tt_next_free_index])

/* U/O Mode EXT HDR macros for Sliding Window */

#define ROHC_DSC_GET_UO_EXT_SW_NIDX(p_dsc_m) \
    (p_dsc_m->rohc_d_uo_ext_hdr_sw.uo_ext_hdr_sw_next_free_index)
#define ROHC_DSC_SET_UO_EXT_SW_NIDX(p_dsc_m, index_m) \
    (p_dsc_m->rohc_d_uo_ext_hdr_sw.uo_ext_hdr_sw_next_free_index = index_m)

#define ROHC_DSC_GET_UO_EXT_SW_NEXT(p_dsc_m) \
    ((p_dsc_m)->rohc_d_uo_ext_hdr_sw.d_uo_ext_hdr_sw[(p_dsc_m)->rohc_d_uo_ext_hdr_sw.uo_ext_hdr_sw_next_free_index])

#define ROHC_DSC_GET_UO_EXT_SW(p_dsc_m, index_m) \
	((p_dsc_m)->rohc_d_uo_ext_hdr_sw.d_uo_ext_hdr_sw[index_m])

#define ROHC_DSC_SET_UO_EXT_SW_REF_IDX(p_dsc_m, ref_idx_m) \
	((p_dsc_m)->rohc_d_uo_ext_hdr_sw.uo_ext_hdr_sw_ref_index = ref_idx_m)
#define ROHC_DSC_GET_UO_EXT_SW_REF_IDX(p_dsc_m) \
	((p_dsc_m)->rohc_d_uo_ext_hdr_sw.uo_ext_hdr_sw_ref_index)

/*
#define ROHC_DSC_GET_UO_EXT_RIDX(p_dsc_m, ref_id_m) \ 
	((p_dsc_m)->map_uo_ext_ref_id_n_idx[ref_id_m])
*/

/* U/O Mode EXT HDR macros for Translation Table */

#define ROHC_DSC_GET_UO_EXT_TT(p_dsc_m, index_m) \
    (p_dsc_m->rohc_d_uo_ext_hdr_tt.d_uo_ext_hdr_tt[index_m])
#define ROHC_DSC_GET_UO_EXT_TT_NEXT(p_dsc_m) \
    ((p_dsc_m)->rohc_d_uo_ext_hdr_tt.d_uo_ext_hdr_tt[(p_dsc_m)->rohc_d_uo_ext_hdr_tt.uo_ext_hdr_tt_next_free_index])

#define ROHC_DSC_GET_UO_EXT_TT_NIDX(p_dsc_m) \
    (p_dsc_m->rohc_d_uo_ext_hdr_tt.uo_ext_hdr_tt_next_free_index)
#define ROHC_DSC_SET_UO_EXT_TT_NIDX(p_dsc_m, index_m) \
    (p_dsc_m->rohc_d_uo_ext_hdr_tt.uo_ext_hdr_tt_next_free_index = index_m)

/* U/O Mode CSRC macros for Sliding Window */

#define ROHC_DSC_GET_UO_CSRC_SW_NIDX(p_dsc_m) \
    (p_dsc_m->rohc_d_uo_csrc_sw.uo_csrc_sw_next_free_index)
#define ROHC_DSC_SET_UO_CSRC_SW_NIDX(p_dsc_m, index_m) \
    (p_dsc_m->rohc_d_uo_csrc_sw.uo_csrc_sw_next_free_index = index_m)

#define ROHC_DSC_GET_UO_CSRC_SW_NEXT(p_dsc_m) \
    ((p_dsc_m)->rohc_d_uo_csrc_sw.d_uo_csrc_sw[(p_dsc_m)->rohc_d_uo_csrc_sw.uo_csrc_sw_next_free_index])

#define ROHC_DSC_GET_UO_CSRC_SW(p_dsc_m, index_m) \
	((p_dsc_m)->rohc_d_uo_csrc_sw.d_uo_csrc_sw[index_m])

#define ROHC_DSC_SET_UO_CSRC_SW_REF_IDX(p_dsc_m, ref_idx_m) \
	((p_dsc_m)->rohc_d_uo_csrc_sw.uo_csrc_sw_ref_index = ref_idx_m)
#define ROHC_DSC_GET_UO_CSRC_SW_REF_IDX(p_dsc_m) \
	((p_dsc_m)->rohc_d_uo_csrc_sw.uo_csrc_sw_ref_index)

#define ROHC_DSC_GET_UO_CSRC_RIDX(p_dsc_m, ref_id_m) \
	((p_dsc_m)->map_uo_csrc_ref_id_n_idx[ref_id_m])

/* U/O Mode CSRC macros for Translation Table */

#define ROHC_DSC_GET_UO_CSRC_TT_NEXT(p_dsc_m) \
    ((p_dsc_m)->rohc_d_uo_csrc_tt.d_uo_csrc_tt[(p_dsc_m)->rohc_d_uo_csrc_tt.uo_csrc_tt_next_free_index])
#define ROHC_DSC_GET_UO_CSRC_TT(p_dsc_m, index_m) \
    (p_dsc_m->rohc_d_uo_csrc_tt.d_uo_csrc_tt[index_m])

#define ROHC_DSC_GET_UO_CSRC_TT_NIDX(p_dsc_m) \
    (p_dsc_m->rohc_d_uo_csrc_tt.uo_csrc_tt_next_free_index)
#define ROHC_DSC_SET_UO_CSRC_TT_NIDX(p_dsc_m, index_m) \
    (p_dsc_m->rohc_d_uo_csrc_tt.uo_csrc_tt_next_free_index = index_m)

/* Klocwork fix */
#define ROHC_DSC_GET_SN_NO(p_dpib_m) \
	(p_dpib_m->field_val[ROHC_SN_FID - ROHC_VAR_FID_BASE])

#endif /* _ROHC_LED_MCR_H_ */
