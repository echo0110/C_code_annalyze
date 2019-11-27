/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_d_typ.h,v 1.4.36.5 2010/11/09 04:26:38 gur22059 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains DSC, DPIB
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_d_typ.h,v $
 * Revision 1.4.36.5  2010/11/09 04:26:38  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.4.36.4  2010/10/13 04:01:02  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.4.36.3  2010/10/04 03:13:18  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.4.36.2  2010/08/19 08:55:48  gur22059
 * SPR 569: Declared the variable pre_pkt_type in structure rohc_dsc_t
 *          for storing the previous received packet type.
 *
 * Revision 1.4  2009/05/28 04:41:36  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_D_TYPES_H_
#define _ROHC_D_TYPES_H_

#include "rohc_typ.h"
#include "rohc_def.h"
#include "rohc_sl_windows.h"
#include "rohc_protocol_header.h"
#include "rohc_com_def.h"
#include "rohc_c_typ.h"
/**************************************************************************/
#define ROHC_PROTO_IP4_STATIC       (100U)
#define ROHC_PROTO_IP4_DYN          (101U)
#define ROHC_PROTO_IP6_STATIC       (102U)  
#define ROHC_PROTO_IP6_DYN          (103U)
#define ROHC_PROTO_UDP_STATIC       (104U)
#define ROHC_PROTO_UDP_DYN          (105U)
#define ROHC_PROTO_RTP_STATIC       (106U)
#define ROHC_PROTO_RTP_HEAD_DYN     (107U)
#define ROHC_PROTO_RTP_TAIL_DYN     (108U)


/**************************************************************************/
typedef struct rohc_ip6_static_t
{
    rohc_U8bit_t       v_flowlabel;
#define ROHC_IP6_STATIC_VER_MASK    (0xf0U)       
#define ROHC_IP6_PROF6_STATIC_VER_MASK    (0x80U) /* For profile6, version will be stored in 1 bit, 
                                                   * with value 1 for ipv6*/
#define ROHC_IP6_STATIC_FLOWLABEL_MASK  (0x0fU)
    rohc_U8bit_t       flowlabel[2];
    rohc_U8bit_t        next_hdr;
    rohc_U8bit_t       src[16];
    rohc_U8bit_t       dst[16];
}rohc_ip6_static_t;
/**************************************************************************/
typedef struct rohc_ip6_prof6_static_flowlabel_zero_t  /* This structure will be used in prof6 if
                                                        * flow label is 0  */
{
    rohc_U8bit_t       v_res_fl_zero;
#define ROHC_IP6_PROF6_STATIC_VER_MASK    (0x80U)       
#define ROHC_IP6_PROF6_STATIC_FLOWLABEL_DISCRIM_MASK  (0x10U) /*If flow label is 0, bit at 
                                                               *this mask will be set 0 otherwise 1*/
#define ROHC_IP6_PROF6_STATIC_FLOWLABEL_ZERO_MASK  (0x0fU) /* If flow label is zero, flow label is not
                                                            * sent and thsee bits are reserved */
    rohc_U8bit_t       next_hdr;
    rohc_U8bit_t       src[16];
    rohc_U8bit_t       dst[16];
}rohc_ip6_prof6_static_flowlabel_zero_t;
/**************************************************************************/
typedef struct rohc_ip6_dyn_t
{
    rohc_U8bit_t    traffic_class;
    /* For profile 6, this field is split into DSCP and ECN fields*/
    #define ROHC_PROF6_IP6_DSCP_MASK      (0xfcU) /* dscp */
    #define ROHC_PROF6_IP6_ECN_FLAG_MASK  (0x03U) /* ecn */
    rohc_U8bit_t    hop_limit;
    /* variable length generic header extension list for Prof 1 and 2*/
}rohc_ip6_dyn_t;
/**************************************************************************/
typedef struct rohc_ip4_static_t
{
    rohc_U8bit_t    ver;
#define ROHC_IP4_STATIC_VER_MASK    (0xf0U)
#define ROHC_IP4_PROF6_STATIC_VER_MASK  (0x80U) /* For profile6, version will be stored in 1 bit, 
                                                   * with value 0 for ipv4 and remaining 7 bits are reserved*/
    rohc_U8bit_t    proto;
    rohc_U8bit_t   src[4];
    rohc_U8bit_t   dst[4];
}rohc_ip4_static_t;
/**************************************************************************/
typedef struct rohc_ip4_dyn_t
{
    rohc_U8bit_t    tos;
    rohc_U8bit_t    ttl;
    rohc_U8bit_t   id[2];
    rohc_U8bit_t    flags;
#define ROHC_IP4_DYN_DF_MASK    (0x80U) /* dont fragment flag */
#define ROHC_IP4_DYN_RND_MASK   (0x40U) /* Random */    
#define ROHC_IP4_DYN_NBO_MASK   (0x20U) /* NBO */   
}rohc_ip4_dyn_t;
/**************************************************************************/

typedef struct rohc_ip4_prof6_dyn_t
{
    rohc_U8bit_t    res_df_ipid_beh;
#define ROHC_PROF6_IP4_DYN_DF_MASK    (0x04U) /* dont fragment flag */
#define ROHC_PROF6_IP4_IPID_BEH_MASK  (0x03U) /*  ipid behaviour choice */
    rohc_U8bit_t    dscp_ecn;
#define ROHC_PROF6_IP4_DSCP_MASK      (0xfcU) /* dscp */
#define ROHC_PROF6_IP4_ECN_FLAG_MASK  (0x03U) /* ecn */
    rohc_U8bit_t    ttl_hopl;
}rohc_ip4_prof6_dyn_t;

/* ipid is either 0 or 16 bits depending on ip-id behavior
 * currently we are assuming ip-id beh as random */
typedef struct rohc_ip4_prof6_dyn_id_t
{
    rohc_U8bit_t   id[2];
}rohc_ip4_prof6_dyn_id_t;

typedef struct rohc_ip4_prof6_replicate_partial_t
{
    rohc_U8bit_t   ipidbeh_ttlflag_df;
#define ROHC_PROF6_IP4_REP_IPIDBEH_MASK    (0x0cU) /*  IP ID Behavior mask */
#define ROHC_PROF6_IP4_REP_TTLFLAG_MASK    (0x02U) /*  TTL flag Mask */
#define ROHC_PROF6_IP4_REP_DFFLAG_MASK     (0x01U) /*  DF flag Mask */
    rohc_U8bit_t   dscp_ipecnflag;
}rohc_ip4_prof6_replicate_partial_t;

typedef struct rohc_ip4_prof6_replicate_ip_id_t
{
    rohc_U16bit_t   ip_id;
} rohc_ip4_prof6_replicate_ip_id_t;

typedef struct rohc_ip4_prof6_replicate_ttlhopl_t
{
    rohc_U8bit_t   ttlhopl;
}rohc_ip4_prof6_replicate_ttlhopl_t;
/**************************************************************************/
typedef struct rohc_udp_static_t
{
    rohc_U8bit_t   src[2];
    rohc_U8bit_t   dst[2];
}rohc_udp_static_t;
/**************************************************************************/
typedef struct rohc_udp_dyn_t
{
    rohc_U8bit_t   cksum[2];
    /* 16 bit SN  present for UDP profile */
}rohc_udp_dyn_t;
/**************************************************************************/
typedef struct rohc_rtp_static_t
{
    rohc_U8bit_t   ssrc[4];
}rohc_rtp_static_t;
/**************************************************************************/
typedef struct rohc_rtp_head_dyn_t
{
    rohc_U8bit_t    ver_p_rx_cc;
#define ROHC_DYN_VER_MASK   (0xc0U)
#define ROHC_DYN_P_MASK     (0x20U)
#define ROHC_DYN_RX_MASK    (0x10U)
#define ROHC_DYN_CC_MASK    (0x0FU)
    rohc_U8bit_t    m_pt;
#define ROHC_DYN_M_MASK     (0x80U)
#define ROHC_DYN_PT_MASK    (0x7fU)
    rohc_U8bit_t   sn[2];
    rohc_U8bit_t   ts[4];
/* variable length CSRC list*/  
}rohc_rtp_head_dyn_t;
/**************************************************************************/
typedef struct rohc_rtp_tail_dyn_t
{
    rohc_U8bit_t    x_mode_tis_tss;
#define ROHC_DYN_X_MASK     (0x10U) 
#define ROHC_DYN_MODE_MASK  (0x0cU) 
#define ROHC_DYN_TIS_MASK   (0x02U) 
#define ROHC_DYN_TSS_MASK   (0x01U) 
    /*optional 1-4 octets TS_STRIDE*/
    /*optional 1-4 octets TIME_STRIDE*/
}rohc_rtp_tail_dyn_t;
/**************************************************************************/
typedef struct rohc_esp_static_t
{
    rohc_U8bit_t   spi[4];
}rohc_esp_static_t;
/**************************************************************************/
typedef struct rohc_esp_dyn_t
{
    rohc_U8bit_t   sn[4];
}rohc_esp_dyn_t;


/**************************************************************************/
typedef struct rohc_tcp_static_t
{
    rohc_U8bit_t   src[2];
    rohc_U8bit_t   dst[2];
}rohc_tcp_static_t;

/**************************************************************************/
typedef struct rohc_tcp_dyn_partial_t
{
    rohc_U8bit_t   ecnu_acksf_ackz_urpz_resr;  /*ECN Used,ack stride flag,ack zero,
                                       tcp res flags*/
#define ROHC_DYN_ECN_USED_MASK        (0x80U)
#define ROHC_DYN_ACK_STRIDE_MASK      (0x40U)
#define ROHC_DYN_ACK_ZERO_MASK        (0x20U)
#define ROHC_DYN_URP_ZERO_MASK        (0x10U)
#define ROHC_DYN_TCP_RES_MASK         (0x0FU)
    rohc_U8bit_t   ecnf_urgf_ackf_pshf_rsf;  /*tcp ecn flags,urg flag,ack flag,
                                       psh flag,rsf flag*/ 
#define ROHC_DYN_ECN_FLAG_MASK        (0xc0U)
#define ROHC_DYN_URG_FLAG_MASK        (0x20U)
#define ROHC_DYN_ACK_FLAG_MASK        (0x10U)
#define ROHC_DYN_PSH_FLAG_MASK        (0x08U)
#define ROHC_DYN_RSF_FLAG_MASK        (0x07U)
    rohc_U8bit_t   msn[2];
    rohc_U8bit_t   seq_num[4];
}rohc_tcp_dyn_partial_t;

typedef struct rohc_tcp_dyn_acknum_t
{
    rohc_U8bit_t   acknum[4];
}rohc_tcp_dyn_acknum_t;

typedef struct rohc_tcp_dyn_windncksum_t
{
    rohc_U8bit_t   window[2];
    rohc_U8bit_t   checksum[2];
}rohc_tcp_dyn_windncksum_t;

typedef struct rohc_tcp_dyn_urgptr_t
{
    rohc_U8bit_t   urg_ptr[2];
}rohc_tcp_dyn_urgptr_t;

typedef struct rohc_tcp_dyn_ackstride_t
{
    rohc_U8bit_t   ack_stride[2];
}rohc_tcp_dyn_ackstride_t;

typedef struct rohc_tcp_dyn_options_t
{
    rohc_U8bit_t   res_ps_m;
#define ROHC_DYN_PS_MASK     (0x10U)
#define ROHC_DYN_TCP_M_MASK      (0x0FU)
}rohc_tcp_dyn_options_t;

typedef struct rohc_tcp_dyn_options_XI_t
{
    rohc_U8bit_t   xi_m[16];
}rohc_tcp_dyn_options_XI_t;

typedef struct rohc_tcp_dyn_options_items_t
{
    rohc_U8bit_t   xi_items[16];
}rohc_tcp_dyn_options_items_t;

/**************************************************************************/
typedef struct rohc_tcp_replicate_partial_t
{
    rohc_U8bit_t   winp_listp_srcprt_dstprt_asf; /*window presence,list present,src port
                                    present,dst port present,ack stide flag*/ 
#define ROHC_REP_WIN_PRES_MASK            (0x40U)
#define ROHC_REP_LIS_PRES_MASK            (0x20U)
#define ROHC_REP_SRC_PORT_PRES_MASK       (0x18U)
#define ROHC_REP_DST_PORT_PRES_MASK       (0x06U)
#define ROHC_REP_ACK_STRD_FLAG_MASK       (0x01U)
    rohc_U8bit_t   ackp_urgp_urgf_ackf_pshf_rsf_ecnu;/*ack presence,urp presence,urg flag,
                                    ack flag,psh flag,rsf flags,ecn used*/ 
#define ROHC_REP_ACK_PRES_MASK            (0x80U)
#define ROHC_REP_URP_PRES_MASK            (0x40U)
#define ROHC_REP_URG_FLAG_MASK            (0x20U)
#define ROHC_REP_ACK_FLAG_MASK            (0x10U)
#define ROHC_REP_PSH_FLAG_MASK            (0x08U)
#define ROHC_REP_RSF_FLAG_MASK            (0x06U)
#define ROHC_REP_ECN_USED_MASK            (0x01U)
    rohc_U8bit_t   msn[2];
    rohc_U8bit_t   seqnum[4];
}rohc_tcp_replicate_partial_t;

typedef struct rohc_tcp_replicate_srcprt_t
{
    rohc_U8bit_t   src_port[2];
}rohc_tcp_replicate_srcprt_t;

typedef struct rohc_tcp_replicate_dstprt_t
{
    rohc_U8bit_t   dst_port[2];
}rohc_tcp_replicate_dstprt_t;

typedef struct rohc_tcp_replicate_win_t
{
    rohc_U8bit_t   win[2];
}rohc_tcp_replicate_win_t;

typedef struct rohc_tcp_replicate_urgptr_t
{
    rohc_U8bit_t   urg_ptr[2];
}rohc_tcp_replicate_urgptr_t;

typedef struct rohc_tcp_replicate_acknum_t
{
    rohc_U8bit_t   ack_num[4];
}rohc_tcp_replicate_acknum_t;

typedef struct rohc_tcp_replicate_ecn_t
{
    rohc_U8bit_t   ecn;           /*ecn padding,tcp res flags,tcp ecn flags*/ 
#define ROHC_REP_ECN_PAD_MASK            (0xc0U)
#define ROHC_REP_TCP_RES_MASK            (0x3cU)
#define ROHC_REP_TCP_ECN_MASK            (0x03U)
}rohc_tcp_replicate_ecn_t;

typedef struct rohc_tcp_replicate_chksum_t
{
    rohc_U8bit_t   chksum[2];
}rohc_tcp_replicate_chksum_t;

typedef struct rohc_tcp_replicate_ackstrd_t
{
    rohc_U8bit_t   ackstrd[2];
}rohc_tcp_replicate_ackstrd_t;

typedef struct rohc_tcp_CR_t
{
    rohc_ip4_prof6_replicate_partial_t  * ip4_replicate_partial;
    rohc_ip4_prof6_replicate_ip_id_t    * ip4_replicate_ipid;
    rohc_ip4_prof6_replicate_ttlhopl_t  * ip4_replicate_ttlhopl;
    rohc_tcp_replicate_partial_t        * tcp_replicate_partial;
    rohc_tcp_replicate_srcprt_t         * tcp_replicate_srcptr;
    rohc_tcp_replicate_dstprt_t         * tcp_replicate_dstptr;
    rohc_tcp_replicate_win_t            * tcp_replicate_win;
    rohc_tcp_replicate_urgptr_t         * tcp_replicate_urg;
    rohc_tcp_replicate_acknum_t         * tcp_replicate_acknum;
    rohc_tcp_replicate_ecn_t            * tcp_replicate_ecn;
    rohc_tcp_replicate_chksum_t         * tcp_replicate_chksum;
    rohc_tcp_replicate_ackstrd_t        * tcp_replicate_ackstride;
}rohc_tcp_CR_t;

typedef struct rohc_tcp_gen_opt_static_t
{
    rohc_U8bit_t        len;
    rohc_U8bit_t        genOptDisc;
    rohc_U8bit_t        optionString[10];
}rohc_tcp_gen_opt_static_t;


/**************************************************************************/

#define DSC_COMMON_PART_1  \
    rohc_U8bit_t d_state;  \
    rohc_U8bit_t d_mode;   \
    rohc_U8bit_t profile;  \
    rohc_U8bit_t decom_res;\
    rohc_dpib_t  *dpib;


#define DSC_COMMON_PART_2                                            \
    rohc_d_r_ext_hdr_sw_t   rohc_d_r_ext_hdr_sw;                     \
    rohc_d_uo_ext_hdr_sw_t  rohc_d_uo_ext_hdr_sw;                    \
    rohc_d_r_ext_hdr_tt_t   rohc_d_r_ext_hdr_tt;                     \
    rohc_d_uo_ext_hdr_tt_t  rohc_d_uo_ext_hdr_tt;                    \
    rohc_list_t         inneripextlist;                              \
    rohc_list_t         outeripextlist;                              \
    rohc_ip4_t          * inner_ip4;                                 \
    rohc_ip4_t          * outer_ip4;                                 \
    rohc_ip6_t          * inner_ip6;                                 \
    rohc_ip6_t          * outer_ip6;                                 \
    rohc_udp_t          * udp;                                       \
    rohc_tcp_t          * tcp;                                       \
    /* + SPR 17439 */                                                \
    rohc_rtp_t          * rtp;                                       \
    /* - SPR 17439 */                                                \
    rohc_U8bit_t        *payload;                                    \
    rohc_error_code_t   *p_tecode;                                   \
    rohc_U32bit_t   cid;                                             \
    rohc_U32bit_t   sn_prev;                                         \
    rohc_U32bit_t   sn_prev_prev;                                    \
    rohc_U16bit_t   pre_inner_ip_id;                                 \
    rohc_U16bit_t   pre_outer_ip_id;                                 \
    rohc_U8bit_t    ipid_beh_outer;                                  \
    rohc_U8bit_t    ipid_beh_inner;                                  \
    rohc_U8bit_t    rnd;                                             \
    rohc_U8bit_t    rnd2;                                            \
    rohc_U8bit_t    nbo;                                             \
    rohc_U8bit_t    nbo2;                                            \
    rohc_U16bit_t   ackstride;                                       \
    rohc_U32bit_t   msn_prev;                                        \
    rohc_U32bit_t   ttl_prev;                                        \
    rohc_U8bit_t    tcp_res_flag;                                    \
    rohc_U8bit_t    ack_stride_flag;                                 \
    rohc_U8bit_t    ecn_used;                                        \
    rohc_U8bit_t    ip_inner_ecn;                                    \
    rohc_U8bit_t    ttl_irregular_chain_flag;                        \
    rohc_U32bit_t   modeTransitionUtoO;                              \
    rohc_U32bit_t   modeTransitionUtoR;                              \
    rohc_U32bit_t   modeTransitionRtoO;                              \
    rohc_U32bit_t   modeTransitionRtoU;                              \
    rohc_U32bit_t   modeTransitionOtoU;                              \
    rohc_U32bit_t   modeTransitionOtoR;                              \
    rohc_U8bit_t    d_trans_state;                                   \
    rohc_U8bit_t    ack_recvd;                                       \
    rohc_U32bit_t   ack_recvd_prev;                                  \
    rohc_U8bit_t    udp_cksum_present;                               \
    rohc_U8bit_t    tcp_cksum_present;                               \
    rohc_U8bit_t    incor_update_sn_succeed_count;                   \
    rohc_U8bit_t    sn_lsb_succeed_count;                            \
    rohc_U8bit_t    n_1;                                             \
    rohc_U8bit_t    k_1;                                             \
    rohc_U8bit_t    n_2;                                             \
    rohc_U8bit_t    k_2;                                             \
    rohc_U8bit_t    temp_mode;                                       \
    rohc_U8bit_t    do_not_update_flag;                              \
    rohc_U8bit_t    prtt;                                            \
    rohc_U8bit_t    pkt_count;                                       \
    rohc_U8bit_t    sparse_ack_k;                                    \
    rohc_U8bit_t    r_n;                                             \
    rohc_U8bit_t    pkt_typ;                                         \
    rohc_U8bit_t    pre_pkt_type;                                    \
    rohc_U32bit_t   map_r_ext_ref_id_n_idx[ROHC_MAX_ENTRIES_IN_TT];  \
    rohc_U32bit_t   map_uo_ext_ref_id_n_idx[ROHC_MAX_ENTRIES_IN_TT]; \
    rohc_U32bit_t   ah_seq;                                          \
    rohc_U32bit_t   esp_seq;                                         \
    rohc_U32bit_t   gre_seq;                                         \
    rohc_U8bit_t    jitter;                                          \
    rohc_U8bit_t    max_jitter;                                      \
    rohc_U8bit_t    longest_loss_event;                              \
    rohc_U8bit_t    clock_resolution;                                \
    rohc_U8bit_t    bit_mask;



typedef struct rohc_dpib_t
{
    /* Decoded Inner IP extension header lists*/
    rohc_U8bit_t                inneripext_new_item_count;
    rohc_list_t                 inneripextlist;
    rohc_decomp_ext_hdr_tt_t    inneripext_tt[ROHC_MAX_ENTRIES_IN_TT];
    rohc_r_sw_t                 inneripext_r_sw;
    rohc_d_uo_sw_t              inneripext_uo_sw;

    /* Decoded Outer IP Extension header lists */
    rohc_U8bit_t                outeripext_new_item_count;
    rohc_list_t                 outeripextlist;
    rohc_decomp_ext_hdr_tt_t    outeripext_tt[ROHC_MAX_ENTRIES_IN_TT];
    rohc_r_sw_t                 outeripext_r_sw;
    rohc_d_uo_sw_t              outeripext_uo_sw;

    /* Decoded CSRC list */
    rohc_U8bit_t            csrc_new_item_count;
    rohc_U32bit_t           final_csrc_list[ROHC_MAX_ENTRIES_IN_CSRC_TT];
    rohc_decomp_csrc_tt_t   csrc_tt[ROHC_MAX_ENTRIES_IN_CSRC_TT];
    rohc_r_sw_t             csrc_r_sw;
    rohc_d_uo_sw_t          csrc_uo_sw;
    
    /* Pointer to IR static/dynamic Headers*/
    rohc_ip6_static_t   * inner_ip6_static;
    rohc_ip6_static_t   * outer_ip6_static;
    rohc_ip4_static_t   * inner_ip4_static;
    rohc_ip4_static_t   * outer_ip4_static;
    rohc_ip6_dyn_t      * inner_ip6_dyn;
    rohc_ip6_dyn_t      * outer_ip6_dyn;
    rohc_ip4_dyn_t      * inner_ip4_dyn;
    rohc_ip4_dyn_t      * outer_ip4_dyn;
    rohc_ip4_prof6_dyn_t * outer_ip4_P6_dyn;
    rohc_ip4_prof6_dyn_id_t * outer_ip4_P6_dyn_id;
    rohc_ip4_prof6_dyn_t * inner_ip4_P6_dyn;
    rohc_ip4_prof6_dyn_id_t * inner_ip4_P6_dyn_id;
    rohc_tcp_static_t   * tcp_static;
    rohc_tcp_dyn_partial_t      * tcp_dyn;
    rohc_tcp_dyn_acknum_t *tcp_dyn_acknum;
    rohc_tcp_dyn_windncksum_t * tcp_dyn_windncksum;
    rohc_tcp_dyn_urgptr_t * tcp_dyn_urgptr;
    rohc_tcp_dyn_ackstride_t * tcp_dyn_ackstride;
    rohc_tcp_dyn_options_t * tcp_dyn_options;
    rohc_tcp_dyn_options_XI_t * tcp_dyn_options_XI;
    rohc_tcp_dyn_options_items_t * tcp_dyn_options_items;
    rohc_ip4_prof6_replicate_partial_t  * ip4_replicate_partial;
    rohc_ip4_prof6_replicate_ip_id_t    * ip4_replicate_ipid;
    rohc_ip4_prof6_replicate_ttlhopl_t  * ip4_replicate_ttlhopl;
    rohc_tcp_replicate_partial_t        * tcp_replicate_partial;
    rohc_tcp_replicate_srcprt_t         * tcp_replicate_srcptr;
    rohc_tcp_replicate_dstprt_t         * tcp_replicate_dstptr;
    rohc_tcp_replicate_win_t            * tcp_replicate_win;
    rohc_tcp_replicate_urgptr_t         * tcp_replicate_urg;
    rohc_tcp_replicate_acknum_t         * tcp_replicate_acknum;
    rohc_tcp_replicate_ecn_t            * tcp_replicate_ecn;
    rohc_tcp_replicate_chksum_t         * tcp_replicate_chksum;
    rohc_tcp_replicate_ackstrd_t        * tcp_replicate_ackstride;
    rohc_U8bit_t rohc_tcp_baseCID;
    rohc_U8bit_t rohc_tcp_baseCID_CRC7;
    rohc_udp_static_t   * udp_static;
    rohc_udp_dyn_t      * udp_dyn;
    rohc_rtp_static_t   * rtp_static;
    rohc_rtp_head_dyn_t * rtp_head_dyn;
    rohc_rtp_tail_dyn_t * rtp_tail_dyn;
    rohc_esp_static_t   * esp_static;
    rohc_esp_dyn_t      * esp_dyn;
    rohc_U8bit_t                inneripext_new_index_list[ROHC_MAX_ENTRIES_IN_TT];
    rohc_U8bit_t                outeripext_new_index_list[ROHC_MAX_ENTRIES_IN_TT];
    rohc_U8bit_t                ext_hdr_ref_index;
    /* Seq number of AH, GRE and ESP after decoding */
    rohc_U32bit_t               decoded_ah_seq;
    rohc_U32bit_t               decoded_gre_seq;
    rohc_U32bit_t               decoded_esp_seq;
    rohc_U16bit_t               decoded_sport;
    rohc_U16bit_t               decoded_dport;

    rohc_U8bit_t            csrc_ref_index;
    rohc_U8bit_t            csrc_new_index_list[ROHC_MAX_ENTRIES_IN_CSRC_TT];
   
    rohc_U8bit_t  * inner_ah_ptr;
    rohc_U8bit_t  * outer_ah_ptr;
    rohc_U8bit_t  * outer_gre;
    rohc_U8bit_t  * inner_gre;
    rohc_U8bit_t  * payload;
    
    /* Field values extracted from compressed packets*/
    rohc_U32bit_t   field_val[ROHC_MAX_FIELD_TYPES 
                            + ROHC_PROF6_MAX_CTRL_FLDS -ROHC_VAR_FID_BASE];
    rohc_U32bit_t   field_len[ROHC_MAX_FIELD_TYPES 
                            + ROHC_PROF6_MAX_CTRL_FLDS -ROHC_VAR_FID_BASE];
    /* Field after LSB decoding */
    rohc_U32bit_t   decoded_sn;
    rohc_U32bit_t   decoded_msn;
    rohc_U16bit_t   decoded_win;
    rohc_U8bit_t    decoded_ttl;
    rohc_U32bit_t   decoded_ack_num;
    rohc_U32bit_t   decoded_ts;
    rohc_U16bit_t   decoded_ipid;
    rohc_U16bit_t   decoded_ipid2;
    rohc_U32bit_t   decodedTsEchoVal;
    /* Decoded TS Echo */
    rohc_U32bit_t   decodedTsEcho;
    /* Decoded SACK blocks */
    rohc_U32bit_t   decodedSackBlockStart1;
    rohc_U32bit_t   decodedSackBlockEnd1;
    rohc_U32bit_t   decodedSackBlockStart2;
    rohc_U32bit_t   decodedSackBlockEnd2;
    rohc_U32bit_t   decodedSackBlockStart3;
    rohc_U32bit_t   decodedSackBlockEnd3;
    rohc_U32bit_t   decodedSackBlockStart4;
    rohc_U32bit_t   decodedSackBlockEnd4;

    rohc_U16bit_t   udp_cksum;
    rohc_U8bit_t    sackBlockUnchanged[34];    
    rohc_U8bit_t    mss[2];    
    rohc_U8bit_t    wscale;    

    rohc_U8bit_t    profile;
    rohc_U8bit_t    rcv_pkt_typ;

    rohc_U32bit_t   payload_len;

    rohc_U16bit_t outer_ip4_tl_or_ip6_pl; /* inferred field which needs to be recomputed on receiving a compressed packet */
    rohc_U16bit_t inner_ip4_tl_or_ip6_pl; /* inferred field which needs to be recomputed on receiving a compressed packet */
    rohc_U16bit_t   outer_ip4_hdr_chksum; /* inferred field which needs to be recomputed on receiving a compressed packet */
    rohc_U16bit_t   inner_ip4_hdr_chksum; /* inferred field which needs to be recomputed on receiving a compressed packet */
#define     ROHC_MAX_GEN_OPTION_SUPP  32
    rohc_tcp_gen_opt_static_t   
        tcpOptGen[ROHC_MAX_GEN_OPTION_SUPP];
     
}rohc_dpib_t;



typedef struct rohc_dsc_t
{
    /* Sliding Window */
    rohc_d_r_csrc_sw_t      rohc_d_r_csrc_sw;
    rohc_d_r_ext_hdr_sw_t   rohc_d_r_ext_hdr_sw;
    rohc_d_uo_csrc_sw_t     rohc_d_uo_csrc_sw;
    rohc_d_uo_ext_hdr_sw_t  rohc_d_uo_ext_hdr_sw;

    /* Translation table */
    rohc_d_r_csrc_tt_t      rohc_d_r_csrc_tt;
    rohc_d_r_ext_hdr_tt_t   rohc_d_r_ext_hdr_tt;
    rohc_d_uo_csrc_tt_t     rohc_d_uo_csrc_tt;
    rohc_d_uo_ext_hdr_tt_t  rohc_d_uo_ext_hdr_tt;

    /* Extension headers */
    rohc_list_t inneripextlist;
    rohc_list_t outeripextlist;
    
    /* Pointer to Headers */
    rohc_ip4_t * inner_ip4; 
    rohc_ip4_t * outer_ip4; 
    rohc_ip6_t * inner_ip6;
    rohc_ip6_t * outer_ip6;
    rohc_udp_t * udp;
    rohc_tcp_t * tcp;
    rohc_esp_t * esp;
    rohc_rtp_t * rtp;
    rohc_U8bit_t *payload;
    
    rohc_dpib_t *dpib;
    rohc_error_code_t   *p_tecode;
    
    rohc_U32bit_t   cid;
    
    rohc_U32bit_t   tss;
    rohc_U32bit_t   tis;
    
    rohc_U32bit_t   sn_prev;
    rohc_U32bit_t   sn_prev_prev; 
    rohc_U32bit_t   arv_tm_pre_pkt; /* arrival time of previous successfully  */
                                    /* decompressed packet */
    rohc_U32bit_t   arv_tm_cur_pkt; /* arrival time of current packet */                                
    
    rohc_U32bit_t   avg_inter_pkt_arv_tm; /* Average inter packet arrival time*/ 
    rohc_U32bit_t   pre_scaled_ts;
    rohc_U32bit_t   pre_ts;

    rohc_U32bit_t   ts_stride;
    rohc_U32bit_t   ts_offset;
    rohc_U32bit_t   time_stride;
    
    rohc_U16bit_t   pre_inner_ip_id;
    rohc_U16bit_t   pre_outer_ip_id;

    rohc_U8bit_t    ipid_beh_outer;         //for Profile 6 instead on rnd and nbo we use
    rohc_U8bit_t    ipid_beh_inner;         //ipid_beh_xxxx
    rohc_U8bit_t    rnd;
    rohc_U8bit_t    rnd2;
    rohc_U8bit_t    nbo;
    rohc_U8bit_t    nbo2;

    /* control parameters for TCP */
    rohc_U16bit_t    ackstride;
    rohc_U16bit_t    msn;
    rohc_U8bit_t    tcp_res_flag;
    rohc_U8bit_t    ack_stride_flag;
    rohc_U8bit_t    ecn_used;
    
    /* parameters for mode transition */
    rohc_U32bit_t    modeTransitionUtoO;
    rohc_U32bit_t    modeTransitionUtoR;
    rohc_U32bit_t    modeTransitionRtoO;
    rohc_U32bit_t    modeTransitionRtoU;
    rohc_U32bit_t    modeTransitionOtoU;
    rohc_U32bit_t    modeTransitionOtoR;

    rohc_U8bit_t    profile;
    rohc_U8bit_t    d_state;
    rohc_U8bit_t    d_trans_state;
    rohc_U8bit_t    d_mode;
    rohc_U8bit_t    ack_recvd;
    rohc_U8bit_t    udp_cksum_present;
    rohc_U8bit_t    incor_update_sn_succeed_count;
    rohc_U8bit_t    sn_lsb_succeed_count;
    rohc_U8bit_t    decom_res; /* result of decompression attempt for */
                               /* for most recently received packet */  
#define ROHC_D_CRC_FAILURE      0x01
#define ROHC_D_PEDCM_FAILURE    0x02
#define ROHC_D_PKT_INVALID      0x04 
#define ROHC_D_SUCCESS          0x00 
                               
    rohc_U8bit_t    n_1; /* not incremented for R mode when R-0/R-1* pkt received*/
    rohc_U8bit_t    k_1; /* not incremented for R mode when R-0/R-1* pkt received*/
    rohc_U8bit_t    n_2; /* not incremented for R mode when R-0/R-1* pkt received*/
    rohc_U8bit_t    k_2; /* not incremented for R mode when R-0/R-1* pkt received*/
    rohc_U8bit_t    temp_mode; 
    rohc_U8bit_t    do_not_update_flag;
    rohc_U8bit_t    prtt;       /* number of pkt received per RTT */
    rohc_U8bit_t    pkt_count;  /* this is resent once the pattern changes or */
                                /*when   pkt_count >= prtt */
    rohc_U8bit_t    sparse_ack_k; /* number of acks per RTT */
                                /* default value is 1 */
    rohc_U8bit_t    r_n; /* first n packets that must be acked*/
                        /* default value is 2*/
                        
    rohc_U8bit_t    pkt_typ; /* last successfully decompressed pkt type */
    
    /* 
    ** START_SPR_569_FIX : Store previous received pkt type
    */
    rohc_U8bit_t    pre_pkt_type; 
    /* END_SPR_569_FIX */                            
       
    rohc_U32bit_t       map_r_ext_ref_id_n_idx[ROHC_MAX_ENTRIES_IN_TT];
    rohc_U32bit_t       map_uo_ext_ref_id_n_idx[ROHC_MAX_ENTRIES_IN_TT];
    rohc_U32bit_t       map_r_csrc_ref_id_n_idx[ROHC_MAX_ENTRIES_IN_CSRC_TT];
    rohc_U32bit_t       map_uo_csrc_ref_id_n_idx[ROHC_MAX_ENTRIES_IN_CSRC_TT];


    rohc_U32bit_t   ah_seq;
    rohc_U32bit_t   esp_seq;
    rohc_U32bit_t   gre_seq;

    rohc_U8bit_t    jitter;
    rohc_U8bit_t    max_jitter;
    rohc_U8bit_t    longest_loss_event;
    rohc_U8bit_t    clock_resolution;
    rohc_U8bit_t    bit_mask; /* to tell Jitter and clk_res present or not */
#define ROHC_UPDATE_DSC_FOR_ED 0x10 
}rohc_dsc_t;

typedef struct rohc_dsc_common_t
{
    DSC_COMMON_PART_1
    DSC_COMMON_PART_2

#define ROHC_D_CRC_FAILURE      0x01
#define ROHC_D_PEDCM_FAILURE    0x02
#define ROHC_D_PKT_INVALID      0x04
#define ROHC_D_SUCCESS          0x00
#define ROHC_UPDATE_DSC_FOR_ED  0x10

}rohc_dsc_common_t;



typedef struct rohc_dsc_tcp_t
{
    DSC_COMMON_PART_1
    DSC_COMMON_PART_2

    rohc_U32bit_t   payload_size;
    rohc_U32bit_t   pre_ts;

    rohc_U16bit_t   src_prev_prev;
    rohc_U16bit_t   src_prev;
    rohc_U16bit_t   win_prev;
    rohc_U16bit_t   dst_prev_prev;
    rohc_U16bit_t   dst_prev;
    rohc_U32bit_t   tcpOptGenStaticFlag;
    rohc_U16bit_t   snScaledPrev;
    rohc_U32bit_t   tsEchoVal;
    /* XI to option type Map */
    rohc_U8bit_t    optionTypeMap[10];
}rohc_dsc_tcp_t;

typedef struct rohc_dsc_udp_t
{
    DSC_COMMON_PART_1
    DSC_COMMON_PART_2

}rohc_dsc_udp_t;

typedef struct rohc_dsc_rtp_t
{
    DSC_COMMON_PART_1
    DSC_COMMON_PART_2

    /* Sliding Window */
    rohc_d_r_csrc_sw_t      rohc_d_r_csrc_sw;
    rohc_d_uo_csrc_sw_t     rohc_d_uo_csrc_sw;

    /* Translation table */
    rohc_d_r_csrc_tt_t      rohc_d_r_csrc_tt;
    rohc_d_uo_csrc_tt_t     rohc_d_uo_csrc_tt;

   /* +- SPR 17439 */

    rohc_U32bit_t   tss;
    rohc_U32bit_t   tis;
    
    rohc_U32bit_t   arv_tm_pre_pkt; /* arrival time of previous successfully  */
                                    /* decompressed packet */
    rohc_U32bit_t   arv_tm_cur_pkt; /* arrival time of current packet */                                
    
    rohc_U32bit_t   avg_inter_pkt_arv_tm; /* Average inter packet arrival time*/ 
    rohc_U32bit_t   pre_scaled_ts;
    rohc_U32bit_t   pre_ts;

    rohc_U32bit_t   ts_stride;
    rohc_U32bit_t   ts_offset;
    rohc_U32bit_t   time_stride;
    
    rohc_U32bit_t       map_r_csrc_ref_id_n_idx[ROHC_MAX_ENTRIES_IN_CSRC_TT];
    rohc_U32bit_t       map_uo_csrc_ref_id_n_idx[ROHC_MAX_ENTRIES_IN_CSRC_TT];

}rohc_dsc_rtp_t;

typedef struct rohc_dsc_uncomp_t
{
    DSC_COMMON_PART_1
}rohc_dsc_uncomp_t;

/* Fix for Memory Optimisation  Start */
typedef union rohc_un_dsc_t
{
    rohc_dsc_udp_t    udp_dsc;
    rohc_dsc_rtp_t    rtp_dsc;
    rohc_dsc_uncomp_t uncomp_dsc;
    rohc_dsc_tcp_t    tcp_dsc;
}rohc_un_dsc_t;
/* Fix for Memory Optimisation  End */

typedef struct rohc_dprm_updated_blk
{
    rohc_ip4_t * inner_ip4; 
    rohc_ip4_t * outer_ip4; 
    rohc_ip6_t * inner_ip6;
    rohc_ip6_t * outer_ip6;
    rohc_udp_t * udp;
    rohc_esp_t * esp;
    rohc_rtp_t * rtp;
    rohc_tcp_t * tcp;
}rohc_dprm_update_blk_t;   


#endif /* _ROHC_D_TYPES_H_ */
