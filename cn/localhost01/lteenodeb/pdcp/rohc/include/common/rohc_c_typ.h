/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_c_typ.h,v 1.3.36.3 2010/11/09 04:26:02 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the definition of CPIB and CSC.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_c_typ.h,v $
 * Revision 1.3.36.3  2010/11/09 04:26:02  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.3.36.2  2010/10/13 04:00:39  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.3.36.1  2010/09/23 06:53:24  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.3  2009/05/28 04:41:35  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_C_TYP_H_
#define _ROHC_C_TYP_H_

#include "rohc_typ.h"
#include "rohc_limits.h"
#include "rohc_sl_windows.h"


typedef struct rohc_profile_node_t rohc_profile_node_t;
typedef struct rohc_csc_node_t rohc_csc_node_t;

#define CSC_COMMON_PART_1   \
    rohc_U8bit_t c_state;   \
    rohc_U8bit_t c_mode;    \
    rohc_U8bit_t profile;   \
    rohc_U32bit_t field_value[ROHC_MAX_FIELD_TYPES-ROHC_VAR_FID_BASE];

/* Start  for SPR#6563 added ts_wrap_flag */
#define CSC_COMMON_PART_2                                                  \
    rohc_gsw_t  rohc_gsw;                                                  \
    rohc_c_r_ext_hdr_sw_t rohc_c_r_ext_hdr_sw;                             \
    rohc_c_uo_ext_hdr_sw_t rohc_c_uo_ext_hdr_sw;                           \
    rohc_c_r_ext_hdr_tt_t   rohc_c_r_ext_hdr_tt;                           \
    rohc_c_uo_ext_hdr_tt_t  rohc_c_uo_ext_hdr_tt;                          \
    rohc_timer_info_blk_t ir_ret_timer;                                    \
    rohc_timer_info_blk_t fo_ret_timer;                                    \
    rohc_cpib_t * p_cpib;                                                  \
    rohc_profile_node_t     *p_rb_tnode;                                   \
    rohc_csc_node_t         *p_csc_tnode;                                  \
    rohc_U32bit_t       field_len[ROHC_MAX_FIELD_TYPES-ROHC_VAR_FID_BASE]; \
    rohc_U32bit_t innerip6_src[4];                                         \
    rohc_U32bit_t innerip6_dst[4];                                         \
    rohc_U32bit_t outerip6_src[4];                                         \
    rohc_U32bit_t outerip6_dst[4];                                         \
    rohc_U32bit_t prtt;                                                    \
    rohc_U32bit_t rtt;                                                     \
    rohc_U32bit_t max_jitter_cd;                                           \
    rohc_U32bit_t cur_time;                                                \
    rohc_U32bit_t clk_resolution;                                          \
    rohc_U32bit_t max_jitter;                                              \
    rohc_U32bit_t delta_field;                                             \
    rohc_U32bit_t   ah_seq;                                                \
    rohc_U32bit_t   gre_seq;                                               \
    rohc_U32bit_t   inner_ah_data_len;                                     \
    rohc_U32bit_t   outer_ah_data_len;                                     \
    rohc_U32bit_t ir_ret_time ;                                            \
    rohc_U32bit_t fo_ret_time ;                                            \
    rohc_U32bit_t no_of_pkt_recvd;                                         \
    rohc_U16bit_t inneripid[ROHC_PKT_BEH_NO];                              \
    rohc_U16bit_t outeripid[ROHC_PKT_BEH_NO];                              \
    rohc_U16bit_t inner_gre_chksum;                                        \
    rohc_U16bit_t outer_gre_chksum;                                        \
    rohc_U8bit_t old_inner_rnd;                                            \
    rohc_U8bit_t new_inner_rnd;                                            \
    rohc_U8bit_t old_outer_rnd;                                            \
    rohc_U8bit_t new_outer_rnd;                                            \
    rohc_U8bit_t old_inner_nbo;                                            \
    rohc_U8bit_t new_inner_nbo;                                            \
    rohc_U8bit_t old_outer_nbo;                                            \
    rohc_U8bit_t new_outer_nbo;                                            \
    rohc_U8bit_t  cur_inneripid_index;                                     \
    rohc_U8bit_t  cur_outeripid_index;                                     \
    rohc_U8bit_t  cur_pkt_type;                                            \
    rohc_U8bit_t  udp_cksum_present;                                       \
    rohc_U8bit_t  ack_recvd;                                               \
    rohc_U8bit_t static_info_update_counter;                               \
    rohc_U8bit_t dynamic_info_update_counter;                              \
    rohc_U8bit_t c_trans_state;                                            \
    rohc_U8bit_t sel_pkt_typ;                                              \
    rohc_U8bit_t no_r0crc_since_lastack;                                   \
    rohc_U8bit_t no_r0crc_m1;                                              \
    rohc_U8bit_t no_r0_prtt_m1;                                            \
    rohc_U8bit_t r0_crc_ack_flag;                                          \
    rohc_U8bit_t uor2_ack_flag;                                            \
    rohc_U8bit_t no_uor2_m2;                                               \
    rohc_U8bit_t no_uor1_prtt_m2;                                          \
    rohc_U8bit_t dont_update_csc_flag;                                     \
    rohc_U8bit_t ts_wrap_flag;                                             \
    rohc_U8bit_t send_irdyn_flag;                                          \
    rohc_U8bit_t jit_clk_res_flag;                                         \
    rohc_U8bit_t flags_from_ctx_info;                                      \
    rohc_U8bit_t longest_loss_event;                                       \
    rohc_U8bit_t static_delta;                                             \
    rohc_U8bit_t *p_inner_ah_data;                                         \
    rohc_U8bit_t *p_outer_ah_data;                                         \
    rohc_U8bit_t inner_gre_chksum_present;                                 \
    rohc_U8bit_t outer_gre_chksum_present;                                 \
    rohc_U8bit_t profile_determined;                                       \
    rohc_U8bit_t  crc_typ;                                                 \
    rohc_U8bit_t  crc_val;                                                 

typedef struct rohc_c_generic_opt_elem_t
{
    rohc_U8bit_t         kind;
    rohc_U8bit_t         length;
    rohc_U8bit_t         *opt_val; /* var length*/
    rohc_U8bit_t         option_static; /* control inf*/
} rohc_c_generic_opt_elem_t;

typedef struct rohc_c_generic_opt_t
{
    rohc_c_generic_opt_elem_t   rohc_generic_opt_elem[ROHC_MAX_TCP_GENERIC_OPT];
    rohc_U8bit_t                next_free_index;
} rohc_c_generic_opt_t;

typedef struct rohc_c_temp_tcp_opt_t
{
    rohc_U32bit_t sack_ptr[8];
    rohc_U8bit_t alt_cksum_req; /* This will also be treated as generic option but stored separately
                                   because its length is fixed*/
    rohc_c_generic_opt_t rohc_c_generic_opt;
    rohc_U16bit_t mss;
    rohc_U8bit_t ws;
    rohc_U32bit_t ts[2];
    rohc_U8bit_t sack_len;
    rohc_U8bit_t eol_len;
}rohc_c_temp_tcp_opt_t;

/* End for SPR#6563 */

/* Compressor side Packet Information Block */
typedef struct rohc_cpib_t
{
    rohc_list_t     inneripextlist;
    rohc_list_t     outeripextlist;
    
    /* Pointer to aunthentication data and len */
    rohc_U32bit_t   ah_data_len;
    rohc_U8bit_t  * p_ah_data;
    rohc_U8bit_t  * payload;
    rohc_U8bit_t  * inner_ah_ptr;
    rohc_U8bit_t  * outer_ah_ptr;
    rohc_U8bit_t  * inner_ip4;
    rohc_U8bit_t  * outer_ip4;
    rohc_U8bit_t  * inner_ip6;
    rohc_U8bit_t  * outer_ip6;
    rohc_U8bit_t  * udp;
    rohc_U8bit_t  * rtp;
    rohc_U8bit_t  * tcp;
    rohc_U8bit_t  * outer_gre;
    rohc_U8bit_t  * inner_gre;
    rohc_U8bit_t  * esp;
     /* always contain the value in host order and NBO*/
    rohc_U32bit_t temp_sn;
    rohc_U32bit_t temp_ack;
    rohc_U32bit_t temp_ts; 
    rohc_U16bit_t temp_ipid;
    rohc_U16bit_t temp_ipid2;
    
    rohc_U16bit_t total_pkt_len;
    rohc_U8bit_t  ah_count;
    rohc_U8bit_t  gre_count;
    rohc_U8bit_t  determined_profile; /* only standard profiles */
    rohc_U8bit_t  expected_profile; /* only standard profiles */
    rohc_U8bit_t  udp_cksum_present;
    rohc_U8bit_t  tcp_cksum_present;
    rohc_U8bit_t  ip4_count;
    rohc_U8bit_t  ip6_count;
    rohc_U8bit_t  inner_ah_len;  
    rohc_U8bit_t  outer_ah_len; 
    
    rohc_U8bit_t  gre_chksum;
    rohc_U8bit_t  gre_chksum_present;
    rohc_U8bit_t temp_tcp_opt_kind[ROHC_MAX_ENTRIES_TCP_OPT_IDX+1];
    rohc_c_temp_tcp_opt_t rohc_c_temp_tcp_opt;
}rohc_cpib_t;

/* timer info block */
typedef struct rohc_timer_info_blk
{
    rohc_timer_id_t tid;    
    rohc_timer_data_t tdata;
}rohc_timer_info_blk_t;


/* Compressor Stream context block */

typedef struct rohc_csc_t
{
    /* Sliding window */
    rohc_gsw_t  rohc_gsw;
    rohc_c_r_csrc_sw_t rohc_c_r_csrc_sw;
    rohc_c_r_ext_hdr_sw_t rohc_c_r_ext_hdr_sw;
    rohc_c_uo_csrc_sw_t  rohc_c_uo_csrc_sw;
    rohc_c_uo_ext_hdr_sw_t rohc_c_uo_ext_hdr_sw;

    /* Translation Table */
    rohc_c_r_csrc_tt_t      rohc_c_r_csrc_tt;
    rohc_c_r_ext_hdr_tt_t   rohc_c_r_ext_hdr_tt;
    rohc_c_uo_csrc_tt_t     rohc_c_uo_csrc_tt;
    rohc_c_uo_ext_hdr_tt_t  rohc_c_uo_ext_hdr_tt;
    
    /* timer block */
    rohc_timer_info_blk_t ir_ret_timer;
    rohc_timer_info_blk_t fo_ret_timer;
    
    rohc_cpib_t * p_cpib; 

    /* Node Pointer */
    rohc_profile_node_t     *p_rb_tnode;
    rohc_csc_node_t         *p_csc_tnode;

    /* to be filled with the length in bits of fields in extension */
    rohc_U32bit_t       field_len[ROHC_MAX_FIELD_TYPES-ROHC_VAR_FID_BASE];
    
    rohc_U32bit_t ts_stride; /* Current ts_stride */
    rohc_U32bit_t time_stride;
    rohc_U32bit_t ts_offset;
    rohc_U32bit_t old_rtp_ts;
    rohc_U32bit_t old_rtp_sn;
    /* IPv6 Src and Dest Address can't be stored in array as more than 32 bit*/
    rohc_U32bit_t innerip6_src[4];
    rohc_U32bit_t innerip6_dst[4];
    rohc_U32bit_t outerip6_src[4];
    rohc_U32bit_t outerip6_dst[4];

    /* arrays for generic encoding */
    rohc_U32bit_t field_value[ROHC_MAX_FIELD_TYPES-ROHC_VAR_FID_BASE];
    rohc_U32bit_t prtt;
    rohc_U32bit_t rtt;
    rohc_U32bit_t max_jitter_cd;
    rohc_U32bit_t cur_time;
    rohc_U32bit_t clk_resolution;
    rohc_U32bit_t max_jitter;
    rohc_U32bit_t delta_field; /* computed in cprm */

    /* Values of AH, GRE and ESP Sequence number used for encoding them */
    rohc_U32bit_t   ah_seq;
    rohc_U32bit_t   esp_seq;
    rohc_U32bit_t   gre_seq;
    
    /* Length of AH data */
    rohc_U32bit_t   inner_ah_data_len;
    rohc_U32bit_t   outer_ah_data_len;
    
    /*  U mode's timer related */
    rohc_U32bit_t ir_ret_time ; /* increased when ACK in received in Umode */
    #define ROHC_INCREASE_IR_RET_TIMER_DELTA 0x01
    rohc_U32bit_t fo_ret_time ;
    rohc_U32bit_t no_of_pkt_recvd;

    rohc_U16bit_t udp_sn;
    /* Fields for behavior determination : IP-ID */
    rohc_U16bit_t inneripid[ROHC_PKT_BEH_NO];
    rohc_U16bit_t outeripid[ROHC_PKT_BEH_NO];
    rohc_U16bit_t inner_gre_chksum;
    rohc_U16bit_t outer_gre_chksum;
    
    /* Behavior Determination field*/
    rohc_U8bit_t old_inner_rnd;
    rohc_U8bit_t new_inner_rnd;
    rohc_U8bit_t old_outer_rnd;
    rohc_U8bit_t new_outer_rnd;
    rohc_U8bit_t old_inner_nbo;
    rohc_U8bit_t new_inner_nbo;
    rohc_U8bit_t old_outer_nbo;
    rohc_U8bit_t new_outer_nbo;
    rohc_U8bit_t  cur_inneripid_index;
    rohc_U8bit_t  cur_outeripid_index;
    rohc_U8bit_t  profile; /* standard profiles only */
    rohc_U8bit_t  cur_pkt_type;   /* to be used by list encoding*/
    rohc_U8bit_t  udp_cksum_present;
    rohc_U8bit_t  ack_recvd;

    /* number of IR -packets sent in U-mode */ 
    rohc_U8bit_t static_info_update_counter;
    rohc_U8bit_t dynamic_info_update_counter;
    rohc_U8bit_t c_state;
    rohc_U8bit_t c_trans_state;
    rohc_U8bit_t c_mode;
    rohc_U8bit_t sel_pkt_typ; /* used in R-mode SO state */
    rohc_U8bit_t no_r0crc_since_lastack;
    rohc_U8bit_t no_r0crc_m1;
    rohc_U8bit_t no_r0_prtt_m1;
    rohc_U8bit_t r0_crc_ack_flag;
    rohc_U8bit_t uor2_ack_flag;
    rohc_U8bit_t no_uor2_m2;
    rohc_U8bit_t no_uor1_prtt_m2; 
    rohc_U8bit_t dont_update_csc_flag;
    rohc_U8bit_t ch_mode_flag;
    rohc_U8bit_t send_irdyn_flag;
    rohc_U8bit_t jit_clk_res_flag;
    rohc_U8bit_t flags_from_ctx_info;
    rohc_U8bit_t longest_loss_event;

    rohc_U8bit_t static_delta;
    #define ROHC_UDP_CHECK_SUM_PRESENT 0x01
    #define ROHC_INNER_IP4_PRESENT 0x02
    #define ROHC_INNER_IP6_PRESENT 0x04
    #define ROHC_OUTER_IP4_PRESENT 0x08
    #define ROHC_OUTER_IP6_PRESENT 0x10


    /* Pointer to Aunthentication data, which contains value only when AH data
     * needs to be send outside the list */
    rohc_U8bit_t *p_inner_ah_data;
    rohc_U8bit_t *p_outer_ah_data;
    rohc_U8bit_t inner_gre_chksum_present;
    rohc_U8bit_t outer_gre_chksum_present;

    /* used for IP ID beh Det */
    rohc_U8bit_t profile_determined;
    
    /* for crc */
    rohc_U8bit_t  crc_typ;
    rohc_U8bit_t  crc_val;
}rohc_csc_t;

typedef struct rohc_csc_common_t
{

    CSC_COMMON_PART_1
    CSC_COMMON_PART_2

    #define ROHC_INCREASE_IR_RET_TIMER_DELTA 0x01
    #define ROHC_UDP_CHECK_SUM_PRESENT 0x01
    #define ROHC_TCP_CHECK_SUM_PRESENT 0x01
    #define ROHC_INNER_IP4_PRESENT 0x02
    #define ROHC_INNER_IP6_PRESENT 0x04
    #define ROHC_OUTER_IP4_PRESENT 0x08
    #define ROHC_OUTER_IP6_PRESENT 0x10

}rohc_csc_common_t;

typedef struct rohc_csc_udp_t
{
    CSC_COMMON_PART_1
    CSC_COMMON_PART_2
    rohc_U16bit_t udp_sn; 

}rohc_csc_udp_t;

typedef struct rohc_csc_rtp_t
{
    CSC_COMMON_PART_1
    CSC_COMMON_PART_2

    /* Sliding window */
    rohc_c_r_csrc_sw_t rohc_c_r_csrc_sw;
    rohc_c_uo_csrc_sw_t  rohc_c_uo_csrc_sw;

    /* Translation Table */
    rohc_c_r_csrc_tt_t      rohc_c_r_csrc_tt;
    rohc_c_uo_csrc_tt_t     rohc_c_uo_csrc_tt;
    
    rohc_U32bit_t ts_stride; /* Current ts_stride */
    rohc_U32bit_t time_stride;
    rohc_U32bit_t ts_offset;
    rohc_U32bit_t old_rtp_ts;
    rohc_U32bit_t old_rtp_sn;

}rohc_csc_rtp_t;


typedef struct rohc_c_tcp_opt
{
    rohc_U8bit_t tcp_opt_kind[ROHC_MAX_ENTRIES_TCP_OPT_IDX+1];
    rohc_U8bit_t tcp_opt_len[MAX_OPT_WITH_VAR_LEN];
    rohc_U16bit_t mss;
    rohc_U8bit_t ws;
    rohc_U32bit_t sack_ptr[8];
    rohc_U32bit_t ts[2];
    rohc_U8bit_t alt_cksum_req; /* This will also be treated as generic option but stored separately
                                   because its length is fixed*/
    rohc_c_generic_opt_t rohc_c_generic_opt;
}rohc_c_tcp_opt_t;

typedef struct rohc_csc_tcp_t
{
    CSC_COMMON_PART_1
    CSC_COMMON_PART_2 
    rohc_U16bit_t tcp_msn;
    rohc_U16bit_t ack_stride;
    rohc_U16bit_t sn_stride;
    rohc_U32bit_t sn_residue;
    rohc_U32bit_t ack_residue;
    rohc_U32bit_t temp_sn_residue;
    rohc_U32bit_t temp_ack_residue;
    rohc_U8bit_t  ecn_used;
    rohc_U8bit_t  inner_ipid_beh;
    rohc_U32bit_t old_tcp_ack;
    rohc_U32bit_t old_tcp_sn;
    rohc_U8bit_t  old_inner_ip4_ttl;
    rohc_c_tcp_opt_t rohc_c_tcp_opt;
    rohc_U8bit_t ctrl_field_val_p6[ROHC_PROF6_MAX_CTRL_FLDS];
    rohc_U8bit_t sn_stride_change;
    rohc_U8bit_t sn_residue_change;
    rohc_U8bit_t ack_stride_change;
    rohc_U8bit_t ack_residue_change;
    rohc_U8bit_t sn_scaling_factor_estab_counter;
    rohc_U8bit_t ack_scaling_factor_estab_counter;
    /* Start SPR 8415 Fix */
    rohc_U8bit_t  num_of_co_common_pkts_sent;                        
    /* End SPR 8415 Fix */                                               
}rohc_csc_tcp_t;

typedef struct rohc_csc_uncomp_t
{
    CSC_COMMON_PART_1
}rohc_csc_uncomp_t;

/* Fix for Memory Optimisation  Start */
typedef union rohc_un_csc_t
{
    rohc_csc_udp_t    udp_csc;
    rohc_csc_rtp_t    rtp_csc;
    rohc_csc_tcp_t    tcp_csc;
    rohc_csc_uncomp_t uncomp_csc;
}rohc_un_csc_t;
/* Fix for Memory Optimisation  End */

typedef struct rohc_simple_cpib_udp_t
{
    rohc_U32bit_t ssrc;
}rohc_simple_cpib_udp_t;

typedef struct rohc_simple_cpib_esp_t
{
    rohc_U32bit_t ssrc;
    rohc_U32bit_t spi;
}rohc_simple_cpib_esp_t;

typedef struct rohc_simple_cpib_t
{
    rohc_U8bit_t flags;
#define ROHC_S_CPIB_IN_IP4_PRESENT  0x80
#define ROHC_S_CPIB_IN_IP6_PRESENT  0x40
#define ROHC_S_CPIB_UDP_PRESENT 0x20
#define ROHC_S_CPIB_ESP_PRESENT 0x10
#define ROHC_S_CPIB_ESP_RTP_FAILED  0x08
   
    union{
    rohc_U32bit_t in_src_ip4;
    rohc_U32bit_t in_src_ip6[4];
    } in_src;

    union{
    rohc_U32bit_t in_dest_ip4;
    rohc_U32bit_t in_dest_ip6[4];
    } in_dest;

    rohc_U16bit_t src_port;
    rohc_U16bit_t dest_port;

    union
    {
        rohc_simple_cpib_udp_t udp;
        rohc_simple_cpib_esp_t esp;
    } ref_cpib;
}rohc_simple_cpib_t;

typedef struct rohc_beh_det_blk_t
{
    rohc_list_node_t    node;
    rohc_simple_cpib_t  s_cpib_list;
    rohc_U8bit_t        no_of_pkt_recvd;
}rohc_beh_det_blk_t;

#endif /* _ROHC_C_TYP_H_ */
