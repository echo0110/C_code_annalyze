/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_sl_windows.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
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
 * $Log: rohc_sl_windows.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:34:55  gur19140
 * first wave of Integration
 *
 * Revision 1.3  2009/05/28 04:41:36  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_SL_WINDOWS_H_
#define _ROHC_SL_WINDOWS_H_

#include "rohc_typ.h"
#include "rohc_com_def.h"
/* *****************************************************
 * ************Compressor Side *************************
 * *****************************************************/

/* 
** SPR_1256_FIX_START: The window size needs to be tuned as per the
** integrated setup capabilities. As seen in our present setup,
** size 14 was inappropriate as the feedback was not getting
** handled by "rohc_ack_handling" function because the SN for
** the received feedback is never found to be in window.
*/
#define ROHC_MAX_LENGTH_GSW     (50U)  
#define ROHC_MAX_LENGTH_GSW_PROF6     (10U)  
/* SPR_1256_FIX_END */
#define ROHC_MAX_ENTRIES_IN_TT      (15U)   
#define ROHC_MAX_SN_ENTRIES  (10U)
#define ROHC_MAX_ENTRIES_IN_SW      (15U)   
#define ROHC_MAX_ENTRIES_IN_CSRC_TT (15U)
/* SPR_1256_FIX_END */
#define ROHC_MAX_ENTRIES_IN_TT      (15U)   
#define ROHC_MAX_SN_ENTRIES  (10U)
#define ROHC_MAX_ENTRIES_IN_SW      (15U)   
#define ROHC_MAX_ENTRIES_IN_CSRC_TT (15U)

/* Generic Sliding Window */

typedef struct  rohc_gsw_element
{
    rohc_U32bit_t     sn; /* for prof 1,2,6 */
    rohc_U32bit_t     ts_scaled;
    /* for prof 6 */
    rohc_U32bit_t     ack;
    rohc_U32bit_t     sn_scaled;
    rohc_U32bit_t     ack_scaled;
    rohc_U32bit_t     msn;
    rohc_U32bit_t     pkt_arvl_time;
    rohc_U16bit_t     inner_ip_id;
    rohc_U16bit_t     inner_offset_i;
    rohc_U16bit_t     outer_ip_id;
    rohc_U16bit_t     outer_offset_i;
    rohc_U8bit_t      pkt_typ;
    rohc_U32bit_t     ts_val;
    rohc_U32bit_t     ts_echo;
    rohc_U8bit_t      ts_opt_recvd;
} rohc_gsw_element_t;

typedef struct  rohc_gsw
{
    rohc_U8bit_t          start_index;
    rohc_U8bit_t          end_index;  
    rohc_gsw_element_t  rohc_gsw_elem[ROHC_MAX_LENGTH_GSW];
    /* the min and max entried added to compute k as per
     * RFC 3095, Sec 4.5.2, bullet 2 */
    rohc_U16bit_t     min_inner_offset_i;
    rohc_U16bit_t     max_inner_offset_i;
    rohc_U16bit_t     min_outer_offset_i;
    rohc_U16bit_t     max_outer_offset_i;
    rohc_U32bit_t    min_sn;
    rohc_U32bit_t    max_sn;
    rohc_U32bit_t    min_ts_scaled;
    rohc_U32bit_t    max_ts_scaled;
    rohc_U32bit_t     min_ack;
    rohc_U32bit_t     max_ack;
    rohc_U32bit_t     min_sn_scaled;
    rohc_U32bit_t     max_sn_scaled;
    rohc_U32bit_t     min_ack_scaled;
    rohc_U32bit_t     max_ack_scaled;
    rohc_U8bit_t      iteration_flag;
    rohc_U32bit_t     min_ts_val;
    rohc_U32bit_t     max_ts_val;
    rohc_U32bit_t     min_ts_echo;
    rohc_U32bit_t     max_ts_echo;
} rohc_gsw_t;

/* Structure of Sliding Window in R-Mode */

typedef struct rohc_r_mode_sw
{
        rohc_U16bit_t    rtp_sn_no;
        rohc_U16bit_t    icount;
        rohc_U8bit_t     index_list[ROHC_MAX_ENTRIES_IN_TT];
} rohc_r_sw_t;

typedef struct rohc_c_r_csrc_sw
{
    rohc_r_sw_t         r_csrc_sw [ROHC_MAX_ENTRIES_IN_SW];
    rohc_U8bit_t        r_csrc_ref_index;
    rohc_U8bit_t        r_csrc_next_free_index;
    rohc_bool_t         ref_id_present;
} rohc_c_r_csrc_sw_t;

typedef struct rohc_c_r_ext_hdr_sw
{
    rohc_r_sw_t         r_ext_hdr_sw [ROHC_MAX_ENTRIES_IN_SW];
    rohc_U8bit_t        r_ext_hdr_ref_index;
    rohc_U8bit_t        r_ext_hdr_next_free_index;
    rohc_bool_t         ref_id_present;
} rohc_c_r_ext_hdr_sw_t;

/* Structure of Sliding Window in U/O Mode */

typedef struct rohc_uo_mode_sw
{
     rohc_U8bit_t       gen_id;
     rohc_U8bit_t       gen_count;
     rohc_U16bit_t      icount;
     rohc_U8bit_t       index_list[ROHC_MAX_ENTRIES_IN_TT];
     rohc_U16bit_t      sn_no_list[ROHC_MAX_SN_ENTRIES];
} rohc_uo_sw_t;

typedef struct rohc_c_uo_csrc_sw
{
    rohc_uo_sw_t        uo_csrc_sw [ROHC_MAX_ENTRIES_IN_SW];
    rohc_U8bit_t        uo_csrc_ref_index;
    rohc_U8bit_t        uo_csrc_next_free_index;
    rohc_bool_t         ref_id_present;
} rohc_c_uo_csrc_sw_t;

typedef struct rohc_c_uo_ext_hdr_sw
{
    rohc_uo_sw_t        uo_ext_hdr_sw [ROHC_MAX_ENTRIES_IN_SW];
    rohc_U8bit_t        uo_ext_hdr_ref_index;
    rohc_U8bit_t        uo_ext_hdr_next_free_index;
    rohc_bool_t         ref_id_present;
} rohc_c_uo_ext_hdr_sw_t;

/* Structure of Translation table in R-Mode */

typedef  struct r_mode_csrc_tt_t
{
    rohc_U8bit_t      known;
    rohc_U32bit_t    item;
    rohc_U16bit_t    sn_no_list[ROHC_MAX_SN_ENTRIES];
} r_csrc_tt_t;

typedef struct rohc_c_r_csrc_tt
{
    struct c_r_mode_csrc_tt
    {
        r_csrc_tt_t         r_csrc_tt;
        rohc_U8bit_t        r_sn_csrc_tt_next_free_index;
    } r_csrc_tt[ROHC_MAX_ENTRIES_IN_CSRC_TT];
    rohc_U8bit_t            r_csrc_tt_next_free_index;
} rohc_c_r_csrc_tt_t;

typedef struct r_ext_hdr_tt
{
    rohc_U8bit_t      known;
    rohc_U8bit_t      *p_item;
    rohc_U16bit_t    sn_no_list[ROHC_MAX_SN_ENTRIES];
} r_ext_hdr_tt_t;

typedef struct rohc_c_r_ext_hdr_tt
{
    struct c_r_mode_ext_hdr_tt
    {
        r_ext_hdr_tt_t      r_ext_hdr_tt;
        rohc_U8bit_t        r_sn_ext_hdr_tt_next_free_index;
    } r_ext_hdr_tt[ROHC_MAX_ENTRIES_IN_TT];
    rohc_U8bit_t            r_ext_hdr_tt_next_free_index;
} rohc_c_r_ext_hdr_tt_t;


/* Structure of Translation table in U/O-Mode */

typedef struct c_uo_mode_csrc_tt
{
    rohc_U8bit_t          known;
    rohc_U32bit_t        item;
    rohc_U8bit_t          counter;
} uo_csrc_tt_t;

typedef struct rohc_c_uo_csrc_tt
{
    uo_csrc_tt_t         uo_csrc_tt[ROHC_MAX_ENTRIES_IN_CSRC_TT];
    rohc_U8bit_t         uo_csrc_tt_next_free_index;
} rohc_c_uo_csrc_tt_t;

typedef struct uo_ext_hdr_tt
{
    rohc_U8bit_t          known;
    rohc_U8bit_t          *p_item;
    rohc_U8bit_t          counter;
} uo_ext_hdr_tt_t;

typedef struct rohc_c_uo_ext_hdr_tt
{
    uo_ext_hdr_tt_t          uo_ext_hdr_tt[ROHC_MAX_ENTRIES_IN_TT];
    rohc_U8bit_t             uo_ext_hdr_tt_next_free_index;
} rohc_c_uo_ext_hdr_tt_t;

/* *******************************************************
 * *************Decompressor Side ************************
 * *******************************************************/

/* Structure of Sliding Window in R-Mode */

typedef struct rohc_d_r_csrc_sw
{
    rohc_r_sw_t             d_r_csrc_sw [ROHC_MAX_ENTRIES_IN_SW];
    rohc_U8bit_t            r_csrc_sw_next_free_index;
    rohc_U8bit_t            r_csrc_sw_ref_index;
} rohc_d_r_csrc_sw_t;

typedef struct rohc_d_r_ext_hdr_sw
{
    rohc_r_sw_t             d_r_ext_hdr_sw [ROHC_MAX_ENTRIES_IN_SW];
    rohc_U8bit_t            r_ext_hdr_sw_next_free_index;
    rohc_U8bit_t            r_ext_hdr_sw_ref_index;
} rohc_d_r_ext_hdr_sw_t;

/* Structure of Sliding Window in U/O-Mode */

typedef struct rohc_d_uo_mode_sw
{
     rohc_U8bit_t       gen_id;
     rohc_U16bit_t      icount;
     rohc_U8bit_t        index_list[ROHC_MAX_ENTRIES_IN_TT];
} rohc_d_uo_sw_t;

typedef struct rohc_d_uo_csrc_sw
{
    rohc_d_uo_sw_t        d_uo_csrc_sw [ROHC_MAX_ENTRIES_IN_SW];
    rohc_U8bit_t            uo_csrc_sw_next_free_index;
    rohc_U8bit_t            uo_csrc_sw_ref_index;
} rohc_d_uo_csrc_sw_t;

typedef struct rohc_d_uo_ext_hdr_sw
{
    rohc_d_uo_sw_t        d_uo_ext_hdr_sw [ROHC_MAX_ENTRIES_IN_SW];
    rohc_U8bit_t            uo_ext_hdr_sw_next_free_index;
    rohc_U8bit_t            uo_ext_hdr_sw_ref_index;
} rohc_d_uo_ext_hdr_sw_t;

/* Structure of Translation table in R-Mode */

typedef struct decomp_csrc_trans_table
{
     rohc_U8bit_t            known;
     rohc_U32bit_t          item;
}rohc_decomp_csrc_tt_t;

typedef struct decomp_ext_hdr_trans_table
{
     rohc_U8bit_t           known;
     rohc_U8bit_t           *p_item;
}rohc_decomp_ext_hdr_tt_t;

typedef struct rohc_d_r_csrc_tt
{
    rohc_decomp_csrc_tt_t   d_r_csrc_tt [ROHC_MAX_ENTRIES_IN_CSRC_TT];
    rohc_U8bit_t            r_csrc_tt_next_free_index;
} rohc_d_r_csrc_tt_t;

typedef struct rohc_d_r_ext_hdr_tt
{
    rohc_decomp_ext_hdr_tt_t    d_r_ext_hdr_tt [ROHC_MAX_ENTRIES_IN_TT];
    rohc_U8bit_t                r_ext_hdr_tt_next_free_index;
} rohc_d_r_ext_hdr_tt_t;

/* Structure of Translation table in U/O-Mode */
 
typedef struct rohc_d_uo_csrc_tt
{
    rohc_decomp_csrc_tt_t   d_uo_csrc_tt[ROHC_MAX_ENTRIES_IN_CSRC_TT];
    rohc_U8bit_t            uo_csrc_tt_next_free_index;
} rohc_d_uo_csrc_tt_t;

typedef struct rohc_d_uo_ext_hdr_tt
{
    rohc_decomp_ext_hdr_tt_t    d_uo_ext_hdr_tt [ROHC_MAX_ENTRIES_IN_TT];
    rohc_U8bit_t                uo_ext_hdr_tt_next_free_index;
} rohc_d_uo_ext_hdr_tt_t;


#endif /* __ROHC_SL_WINDOWS_H__ */
