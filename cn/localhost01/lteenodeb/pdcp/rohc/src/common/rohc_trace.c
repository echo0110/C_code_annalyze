/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_trace.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the common data types which are to
 *                     be exported out of the stack.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_trace.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.3  2009/05/27 07:15:32  gur19836
 * File Headers and Function Headers updated
 *
 *
 *****************************************************************************/

#ifdef ROHC_UT_FLAG

/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "rohc_typ.h"
#include "rohc_def.h"
#include "rohc_c_typ.h"
#include "rohc_d_typ.h"
#include "rohc_g_typ.h"
#include "rohc_trace_mcr.h"
#include "rohc_sl_windows.h"
#include "rohc_db_ext_if.h"

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


/*****************************************************************************
 * Function Name  : rohc_dump_cpib 
 * Inputs         : p_cpib - Pointer to CPIB
 * Outputs        : None
 * Returns        : None
 * Description    : structure dump of CPIB
 *****************************************************************************/
rohc_void_t rohc_dump_cpib(struct rohc_cpib_t * p_cpib)
{
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,"Inner IP Extension List");    
	rohc_db_dump_list(&p_cpib->inneripextlist);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,"Inner IP Extension List");    
	rohc_db_dump_list(&p_cpib->inneripextlist);

    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," PAYLOAD = %p",p_cpib->payload);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " INNER AH POINTER = %p",p_cpib->inner_ah_ptr);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " OUTER AH POINTER = %p",p_cpib->outer_ah_ptr);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " INNER IP 4  = %p",p_cpib->inner_ip4);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " OUTER IP 4  = %p",p_cpib->outer_ip4);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " INNER IP 6  = %p",p_cpib->inner_ip6);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " OUTER IP 6  = %p",p_cpib->outer_ip6);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " UDP  = %p",p_cpib->udp);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " RTP  = %p",p_cpib->rtp);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " OUTER GRE  = %p",p_cpib->outer_gre);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " INNER GRE  = %p",p_cpib->inner_gre);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," ESP  = %p",p_cpib->esp);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," PACKET lENGTH  = %u",
        p_cpib->total_pkt_len);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," AH COUNT  = %u",p_cpib->ah_count);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " GRE COUNT  = %u",p_cpib->gre_count);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " PROFILE  = %u",p_cpib->determined_profile);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " UDP CKSUM FLAG  = %u",p_cpib->udp_cksum_present);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," IP4 COUNT  = %u",
        p_cpib->ip4_count);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," IP6 COUNT  = %u",
        p_cpib->ip6_count);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," INNER AH LENGTH  = %u",
        p_cpib->inner_ah_len);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," OUTER AH LENGTH  = %u",
        p_cpib->outer_ah_len);
}

/*****************************************************************************
 * Function Name  : rohc_dump_dpib 
 * Inputs         : p_dpib - Pointer to DPIB
 * Outputs        : None
 * Returns        : None
 * Description    : structure dump of DPIB
 *****************************************************************************/
rohc_void_t rohc_dump_dpib(rohc_dpib_t * p_dpib)
{
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,"Inner IP Extension List");    
	rohc_db_dump_list(&p_dpib->inneripextlist);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,"Outer IP Extension List");    
	rohc_db_dump_list(&p_dpib->outeripextlist);

    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," PAYLOAD = %p",p_dpib->payload);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," PROFILE= %p",p_dpib->profile);
    
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " UDP CHECKSUM = %p",p_dpib->udp_cksum);

    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " INNER AH POINTER = %p",p_dpib->inner_ah_ptr);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " OUTER AH POINTER = %p",p_dpib->outer_ah_ptr);
    
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " OUTER GRE  = %p",p_dpib->outer_gre);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " INNER GRE  = %p",p_dpib->inner_gre);
    
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " INNER IP 4 STATIC  = %p",p_dpib->inner_ip4_static);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " OUTER IP 4 STATIC  = %p",p_dpib->outer_ip4_static);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " INNER IP 4 DYNAMIC  = %p",p_dpib->inner_ip4_dyn);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " OUTER IP 4 DYNAMIC  = %p",p_dpib->outer_ip4_dyn);
    
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " INNER IP 6 STATIC = %p",p_dpib->inner_ip6_static);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " OUTER IP 6 STATIC  = %p",p_dpib->outer_ip6_static);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " INNER IP 6 DYNAMIC = %p",p_dpib->inner_ip6_dyn);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " OUTER IP 6 DYNAMIC  = %p",p_dpib->outer_ip6_dyn);
    
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " UDP STATIC = %p",p_dpib->udp_static);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " UDP DYNAMIC= %p",p_dpib->udp_dyn);
    
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " RTP STATIC = %p",p_dpib->rtp_static);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " RTP HEAD DYNAMIC = %p",p_dpib->rtp_head_dyn);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " RTP TAIL DYNAMIC = %p",p_dpib->rtp_tail_dyn);

    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
                     " ESP STATIC  = %p",p_dpib->esp_static);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
                     " ESP DYN  = %p",p_dpib->esp_dyn);

    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
                     " DECODED SN  = %p",p_dpib->decoded_sn);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
                     " DECODED TS  = %p",p_dpib->decoded_ts);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
                     " DECODED IPID  = %p",p_dpib->decoded_ipid);
}

/*****************************************************************************
 * Function Name  : rohc_dump_rohc_entity 
 * Inputs         : p_entity - Pointer to rohc entity
 * Outputs        : None
 * Returns        : None
 * Description    : structure dump of rohc entity
 *****************************************************************************/
rohc_void_t rohc_dump_entity(rohc_entity_t * p_entity)
{
    
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," K_1 = %p",p_entity->k_1);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," K_2 = %p",p_entity->k_2);
    
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," N_1 = %p",p_entity->n_1);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," N_2 = %p",p_entity->n_2);
    
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," M_1 = %p",p_entity->m_1);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," M_2 = %p",p_entity->m_2);
    
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,"max_ir_return_time  = %p",
                     p_entity->max_ir_return_time);

    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,"max_fo_return_time  = %p",
                     p_entity->max_fo_return_time);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,"max_static_info_update_counter  = %p",
                     p_entity->max_static_info_update_counter);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,"max_dynamic_info_update_counter = %p",
                     p_entity->max_dynamic_info_update_counter);
}

/*****************************************************************************
 * Function Name  : rohc_dump_csc 
 * Inputs         : p_csc - Pointer to rohc CSC
 * Outputs        : None
 * Returns        : None
 * Description    : structure dump of rohc CSC
 *****************************************************************************/
rohc_void_t rohc_dump_csc(rohc_csc_t  *p_csc)
{
    rohc_U8bit_t  i, j;

    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," UDP SN = %u\n", p_csc->udp_sn);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," OLD INNER RND = %u\n",
        p_csc->old_inner_rnd);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," NEW INNER RND = %u\n",
        p_csc->new_inner_rnd);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," OLD OUTER RND = %u\n",
        p_csc->old_outer_rnd);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," NEW OUTER RND = %u\n",
        p_csc->new_outer_rnd);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," OLD INNER NBO = %u\n",
        p_csc->old_inner_nbo);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," NEW INNER NBO = %u\n",
        p_csc->new_inner_nbo);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," OLD OUTER NBO = %u\n",
        p_csc->old_outer_nbo);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," NEW OUTER NBO = %u\n",
        p_csc->new_outer_nbo);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," TS STRIDE = %d\n", p_csc->ts_stride);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," OLD RTP TS = %d\n",
        p_csc->old_rtp_ts);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," OLD RTP SN = %d\n",
        p_csc->old_rtp_sn);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," PROFILE = %u\n", p_csc->profile);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," UDP CHECKSUM Present = %u\n",
        p_csc->udp_cksum_present);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," ACKED = %u\n", p_csc->ack_recvd);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," PRTT = %d\n", p_csc->prtt);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," RTT = %d\n", p_csc->rtt);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," MAX_JITTER_CD = %d\n",
        p_csc->max_jitter_cd);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," CURR TIME = %d\n", p_csc->cur_time);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," CLOCK RESOLUTION = %d\n",
        p_csc->clk_resolution);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," MAX JITTER = %d\n", 
        p_csc->max_jitter);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," STATIC INFO UPDATE COUNTER = %u\n",
        p_csc->static_info_update_counter);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," DYNAMIC INFO UPDATE COUNTER = %u\n",
        p_csc->dynamic_info_update_counter);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," STATE = %u\n", p_csc->c_state);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," TRANS STATE = %u\n", 
        p_csc->c_trans_state);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," MODE = %u\n", p_csc->c_mode);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," CURRENT INNER IPID INDEX= %u\n",
        p_csc->cur_inneripid_index);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," CURRENT OUTER IPID INDEX = %u\n",
        p_csc->cur_outeripid_index);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," INNER IPID\n");
    for(i = 0; i <= ROHC_PKT_BEH_NO; i++) {
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," InnerIPID[%d] = %u\t", 
            i, p_csc->inneripid[i]);
    }
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," OUTER IPID\n");
    for(i = 0; i<= ROHC_PKT_BEH_NO; i++) {
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," OuterIPID[%d] = %u\t",
            i, p_csc->outeripid[i]);
    }

    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," INNER IP6 SRC\n");
    for(i=0; i<=4; i++)
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," %d ", p_csc->innerip6_src[i]);

    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," INNER IP6 DEST\n");
    for (i=0; i<=4; i++)
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," %d ", p_csc->innerip6_dst[i]);

    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," OUTER IP6 SRC\n");
    for (i=0; i<=4; i++)
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," %d ", p_csc->outerip6_src[i]);

    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," OUTER IP6 DEST\n");
    for (i=0; i<=4; i++)
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," %d ", p_csc->outerip6_dst[i]);

    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Generic Sliding Window\n");
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Start Index = %u\n", 
        p_csc->rohc_gsw.start_index);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," End Index = %u\n",
        p_csc->rohc_gsw.end_index);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," REF IP ID = %u\n",
        p_csc->rohc_gsw.rohc_gsw_elem[p_csc->rohc_gsw.start_index].ip_id);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," REF OFFSET I = %u\n",
        p_csc->rohc_gsw.rohc_gsw_elem[p_csc->rohc_gsw.start_index].offset_i);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," REF SN = %u\n",
        p_csc->rohc_gsw.rohc_gsw_elem[p_csc->rohc_gsw.start_index].sn);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," REF TS_SCALED = %u\n",
        p_csc->rohc_gsw.rohc_gsw_elem[p_csc->rohc_gsw.start_index].ts_scaled);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," REF PKT ARR TIME = %u\n",
        p_csc->rohc_gsw.rohc_gsw_elem[p_csc->rohc_gsw.start_index].pkt_arvl_time);

    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," R Mode CSRC Sliding Window REF\n");
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," RTP SN NO = %u\n", 
        p_csc->rohc_c_r_csrc_sw.r_csrc_sw[p_csc->rohc_c_r_csrc_sw.r_csrc_ref_index].rtp_sn_no);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," COUNT = %u\n",
        p_csc->rohc_c_r_csrc_sw.r_csrc_sw[p_csc->rohc_c_r_csrc_sw.r_csrc_ref_index].icount);

    for(i=0; i<=p_csc->rohc_c_r_csrc_sw.r_csrc_sw[p_csc->rohc_c_r_csrc_sw.r_csrc_ref_index].icount; i++)
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," %u ", p_csc->rohc_c_r_csrc_sw.r_csrc_sw[p_csc->rohc_c_r_csrc_sw.r_csrc_ref_index].index_list[i]);

    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," U/O Mode CSRC Sliding Window REF\n");
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Gen ID = %u\n",
        p_csc->rohc_c_uo_csrc_sw.uo_csrc_sw[p_csc->rohc_c_uo_csrc_sw.uo_csrc_ref_index].gen_id);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Count = %u\n",
        p_csc->rohc_c_uo_csrc_sw.uo_csrc_sw[p_csc->rohc_c_uo_csrc_sw.uo_csrc_ref_index].icount);
    for(i=0; i<=p_csc->rohc_c_uo_csrc_sw.uo_csrc_sw[p_csc->rohc_c_uo_csrc_sw.uo_csrc_ref_index].icount; i++)
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," %u ", p_csc->rohc_c_uo_csrc_sw.uo_csrc_sw[p_csc->rohc_c_uo_csrc_sw.uo_csrc_ref_index].index_list[i]);

    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," R Mode EXT HDR Sliding Window REF\n");
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," RTP SN NO = %u\n",
        p_csc->rohc_c_r_ext_hdr_sw.r_ext_hdr_sw[p_csc->rohc_c_r_ext_hdr_sw.r_ext_hdr_ref_index].rtp_sn_no);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," COUNT = %u\n",
        p_csc->rohc_c_r_ext_hdr_sw.r_ext_hdr_sw[p_csc->rohc_c_r_ext_hdr_sw.r_ext_hdr_ref_index].icount);
    for(i=0; i<= p_csc->rohc_c_r_ext_hdr_sw.r_ext_hdr_sw[p_csc->rohc_c_r_ext_hdr_sw.r_ext_hdr_ref_index].icount; i++)
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," %u ", p_csc->rohc_c_r_ext_hdr_sw.r_ext_hdr_sw[p_csc->rohc_c_r_ext_hdr_sw.r_ext_hdr_ref_index].index_list[i]);


    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," U/O Mode EXT HDR Sliding Window REF\n");
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,"Gen ID = %u\n",
        p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_sw[p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_ref_index].gen_id);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," COUNT = %u\n",
        p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_sw[p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_ref_index].icount);
    for(i=0; i<= p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_sw[p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_ref_index].icount; i++)
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," %u ", p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_sw[p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_ref_index].index_list[i]);

    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Entries in R Mode CSRC TT\n");
    for (i=0; i< p_csc->rohc_c_r_csrc_tt.r_csrc_tt_next_free_index; i++)
    {
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Known flag = %u\n",
            p_csc->rohc_c_r_csrc_tt.r_csrc_tt[i].r_csrc_tt.known);
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," CSRC number = %d\n",
            p_csc->rohc_c_r_csrc_tt.r_csrc_tt[i].r_csrc_tt.item);
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," SN List = ");
        for(j=0; j<= p_csc->rohc_c_r_csrc_tt.r_csrc_tt[i].r_sn_csrc_tt_next_free_index; j++)
        {
            ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," %u ",
                p_csc->rohc_c_r_csrc_tt.r_csrc_tt[i].r_csrc_tt.sn_no_list[j]);
        }
    }

    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Entries in R Mode EXT HDR TT\n");
    for(i=0; i< p_csc->rohc_c_r_ext_hdr_tt.r_ext_hdr_tt_next_free_index; i++)
    {
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Known Flag = %u\n",
            p_csc->rohc_c_r_ext_hdr_tt.r_ext_hdr_tt[i].r_ext_hdr_tt.known);
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Item = %p\n",
            p_csc->rohc_c_r_ext_hdr_tt.r_ext_hdr_tt[i].r_ext_hdr_tt.p_item);
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," SN List = ");
        for(j=0; j<= p_csc->rohc_c_r_ext_hdr_tt.r_ext_hdr_tt[i].r_sn_ext_hdr_tt_next_free_index; j++)
        {
            ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," %u ",
                p_csc->rohc_c_r_ext_hdr_tt.r_ext_hdr_tt[i].r_ext_hdr_tt.sn_no_list[j]);
        }
    }

    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Entries in U/O Mode CSRC TT\n");
    for(i=0; i< p_csc->rohc_c_uo_csrc_tt.uo_csrc_tt_next_free_index; i++)
    {
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Known Flag = %u\n",
            p_csc->rohc_c_uo_csrc_tt.uo_csrc_tt[i].known);
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," CSRC Number = %u\n",
            p_csc->rohc_c_uo_csrc_tt.uo_csrc_tt[i].item);
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Counter = %u\n",
            p_csc->rohc_c_uo_csrc_tt.uo_csrc_tt[i].counter);
    }

    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Entries in U/O Mode EXT HDR TT\n");
    for(i=0; i< p_csc->rohc_c_uo_ext_hdr_tt.uo_ext_hdr_tt_next_free_index; i++)
    {
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Known Flag = %u\n",
            p_csc->rohc_c_uo_ext_hdr_tt.uo_ext_hdr_tt[i].known);
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Item = %p\n",
            p_csc->rohc_c_uo_ext_hdr_tt.uo_ext_hdr_tt[i].p_item);
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Counter = %u\n",
            p_csc->rohc_c_uo_ext_hdr_tt.uo_ext_hdr_tt[i].counter);
    }
}

/*****************************************************************************
 * Function Name  : rohc_dump_dsc 
 * Inputs         : p_dsc - Pointer to rohc DSC
 * Outputs        : None
 * Returns        : None
 * Description    : structure dump of rohc DSC
 *****************************************************************************/
rohc_void_t   rohc_dump_dsc(rohc_dsc_t    *p_dsc)
{
    rohc_U32bit_t i, j;

    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,"Inner IP Extension List");    
	rohc_db_dump_list(&p_dsc->inneripextlist);

    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,"Outer IP Extension List");    
	rohc_db_dump_list(&p_dsc->outeripextlist);

    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " INNER IP 4  = %p\n",p_dsc->inner_ip4);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " OUTER IP 4  = %p\n",p_dsc->outer_ip4);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " INNER IP 6  = %p\n",p_dsc->inner_ip6);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL,
        " OUTER IP 6  = %p\n",p_dsc->outer_ip6);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," RTP = %p\n",
        p_dsc->rtp);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," ESP = %p\n",
        p_dsc->esp);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," UDP = %p\n",
        p_dsc->udp);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," PAYLOAD = %p\n",
        p_dsc->payload);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," RND = %u\n",
        p_dsc->rnd);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," RND2 = %u\n",
        p_dsc->rnd2);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," NBO = %u\n",
        p_dsc->nbo);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," NBO2 = %u\n",
        p_dsc->nbo2);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Profile = %u\n",
        p_dsc->profile);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," State = %u\n",
        p_dsc->d_state);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Trans State = %u\n",
        p_dsc->d_trans_state);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Mode = %u\n",
        p_dsc->d_mode);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," ACKED = %u\n",
        p_dsc->ack_recvd);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," TSS = %d\n",
        p_dsc->tss);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," TIS = %d\n",
        p_dsc->tis);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," AVG PKT ARR TIME = %d\n",
        p_dsc->avg_pkt_arrival_time);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," PKT ARR TIME = %d\n",
        p_dsc->pkt_arrival_time);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," PREV SEQ NO = %d\n",
        p_dsc->seq_no_prev);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," PREV PREV SEQ NO = %d\n",
        p_dsc->seq_no_prev_prev);
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," UDP CHECKSUM PRESENT = %d\n",
        p_dsc->udp_cksum_present);

    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," R Mode CSRC Sliding Window\n");
    for(i=0; i< p_dsc->rohc_d_r_csrc_sw.r_csrc_sw_next_free_index; i++)
    {
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," RTP SN NO. = %u\n",
            p_dsc->rohc_d_r_csrc_sw.d_r_csrc_sw[i].rtp_sn_no);
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Count = %u\n",
            p_dsc->rohc_d_r_csrc_sw.d_r_csrc_sw[i].icount);
        for(j=0; j<= p_dsc->rohc_d_r_csrc_sw.d_r_csrc_sw[i].icount; j++)
        {
            ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," %u ", 
                p_dsc->rohc_d_r_csrc_sw.d_r_csrc_sw[i].index_list[j]);
        }
    }
    
    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," R Mode EXT HDR Sliding Window\n");
    for(i=0; i< p_dsc->rohc_d_r_ext_hdr_sw.r_ext_hdr_sw_next_free_index; i++)
    {
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," RTP SN NO. = %u\n",
            p_dsc->rohc_d_r_ext_hdr_sw.d_r_ext_hdr_sw[i].rtp_sn_no);
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Count = %u\n",
             p_dsc->rohc_d_r_ext_hdr_sw.d_r_ext_hdr_sw[i].icount);
        for(j=0; j<= p_dsc->rohc_d_r_ext_hdr_sw.d_r_ext_hdr_sw[i].icount; j++)
        {
            ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," %u ",
                p_dsc->rohc_d_r_ext_hdr_sw.d_r_ext_hdr_sw[i].index_list[j]);
        }
    }

    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," U/O Mode CSRC Sliding Window\n");
    for(i=0; i<p_dsc->rohc_d_uo_csrc_sw.uo_csrc_sw_next_free_index; i++)
    {
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Gen ID = %u\n",
            p_dsc->rohc_d_uo_csrc_sw.d_uo_csrc_sw[i].gen_id);
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Count=  %u\n",
            p_dsc->rohc_d_uo_csrc_sw.d_uo_csrc_sw[i].icount);
        for(j=0; j<= p_dsc->rohc_d_uo_csrc_sw.d_uo_csrc_sw[i].icount; j++)
        {
            ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," %u ",
                p_dsc->rohc_d_uo_csrc_sw.d_uo_csrc_sw[i].index_list[j]);
        }
    }

    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," U/O Mode EXT HDR Sliding Window\n");
    for(i=0; i<p_dsc->rohc_d_uo_ext_hdr_sw.uo_ext_hdr_sw_next_free_index; i++)
    {
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Gen ID = %u\n",
            p_dsc->rohc_d_uo_ext_hdr_sw.d_uo_ext_hdr_sw[i].gen_id);
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Count=  %u\n",
            p_dsc->rohc_d_uo_ext_hdr_sw.d_uo_ext_hdr_sw[i].icount);
        for(j=0; j<= p_dsc->rohc_d_uo_ext_hdr_sw.d_uo_ext_hdr_sw[i].icount; j++)
        {
            ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," %u ",
                p_dsc->rohc_d_uo_ext_hdr_sw.d_uo_ext_hdr_sw[i].index_list[j]);
        }
    }

    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Entries in R Mode CSRC TT\n");
    for(i=0; i<= p_dsc->rohc_d_r_csrc_tt.r_csrc_tt_next_free_index; i++)
    {
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," TT[%d] = (%u, %u)\n",
            i, p_dsc->rohc_d_r_csrc_tt.d_r_csrc_tt[i].known,
            p_dsc->rohc_d_r_csrc_tt.d_r_csrc_tt[i].item);
    }

    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Entries in R Mode EXT HDR TT\n");
    for(i=0; i<= p_dsc->rohc_d_r_ext_hdr_tt.r_ext_hdr_tt_next_free_index; i++)
    {   
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," TT[%d] = (%u, %p)\n",
            i, p_dsc->rohc_d_r_ext_hdr_tt.d_r_ext_hdr_tt[i].known,
            p_dsc->rohc_d_r_ext_hdr_tt.d_r_ext_hdr_tt[i].p_item);
    }


    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Entries in U/O Mode CSRC TT\n");
    for(i=0; i<= p_dsc->rohc_d_uo_csrc_tt.uo_csrc_tt_next_free_index; i++)
    {   
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," TT[%d] = (%u, %u)\n",
            i, p_dsc->rohc_d_uo_csrc_tt.d_uo_csrc_tt[i].known,
            p_dsc->rohc_d_uo_csrc_tt.d_uo_csrc_tt[i].item);
    }

    ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," Entries in U/O Mode EXT HDR TT\n");
    for(i=0; i<= p_dsc->rohc_d_uo_ext_hdr_tt.uo_ext_hdr_tt_next_free_index; i++)
    {   
        ROHC_STACK_PRINT(ROHC_UT_TRACE_LEVEL," TT[%d] = (%u, %p)\n",
            i, p_dsc->rohc_d_uo_ext_hdr_tt.d_uo_ext_hdr_tt[i].known,
            p_dsc->rohc_d_uo_ext_hdr_tt.d_uo_ext_hdr_tt[i].p_item);
    }

}

#endif /* ROHC_UT_FLAG */
