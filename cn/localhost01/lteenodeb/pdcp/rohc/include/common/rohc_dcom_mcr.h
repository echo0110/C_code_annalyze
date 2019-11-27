/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_dcom_mcr.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains GET/SET macro for DPIB/DSC
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_dcom_mcr.h,v $
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

#ifndef _ROHC_DCOM_MCR_H_
#define _ROHC_DCOM_MCR_H_

/******************************************************************************
 *  Macro Name      : ROHC_DPIB_GET_DECODED_SN
 *
 *  Description     : Returns the decoded value of SN from DPIB
 *
 *****************************************************************************/

#define ROHC_DPIB_GET_DECODED_SN(p_dpib_m)\
    (p_dpib_m->decoded_sn)
    

/******************************************************************************
 *  Macro Name      : ROHC_DPIB_SET_DECODED_SN
 *
 *  Description     : Updates the decoded value of SN in DPIB
 *
 *****************************************************************************/

#define ROHC_DPIB_SET_DECODED_SN_DPIB(p_dpib_m,sn_m)\
    (p_dpib_m->decoded_sn = sn_m)

/******************************************************************************
 *  Macro Name      : ROHC_DPIB_GET_DECODED_SN
 *
 *  Description     : Returns the decoded value of TS from DPIB
 *
 *****************************************************************************/

#define ROHC_DPIB_GET_DECODED_TS_DPIB(p_dpib_m)\
    (p_dpib_m->decoded_ts)
    

/******************************************************************************
 *  Macro Name      : ROHC_DPIB_SET_DECODED_SN
 *
 *  Description     : Updates the decoded value of TS in DPIB
 *
 *****************************************************************************/

#define ROHC_DPIB_SET_DECODED_TS(p_dpib_m,sn_m)\
    (p_dpib_m->decoded_ts = ts_m)

/******************************************************************************
 *  Macro Name      : ROHC_DPIB_GET_DECODED_IPID
 *
 *  Description     : Returns the decoded value of IPID from DPIB
 *
 *****************************************************************************/

#define ROHC_DPIB_GET_DECODED_IPID(p_dpib_m)\
    (p_dpib_m->decoded_ipid)
    

/******************************************************************************
 *  Macro Name      : ROHC_DPIB_SET_DECODED_IPID
 *
 *  Description     : Updates the decoded value of IPID in DPIB
 *
 *****************************************************************************/

#define ROHC_DPIB_SET_DECODED_IPID(p_dpib_m,ipid_m)\
    (_pdpib_m->decoded_ipid = ipid_m)

/******************************************************************************
 *  Macro Name      : ROHC_DPIB_GET_INNER_IP6_STATIC
 *
 *  Description     : Returns the pointer to inner_ip6_static from DPIB
 *
 *****************************************************************************/

#define ROHC_DPIB_GET_INNER_IP6_STATIC(p_dpib_m)\
    (p_dpib_m->inner_ip6_static)

/******************************************************************************
 *  Macro Name      : ROHC_DPIB_SET_INNER_IP6_STATIC
 *
 *  Description     : Updates the pointer to inner_ip6_static in DPIB
 *
 *****************************************************************************/

#define ROHC_DPIB_SET_INNER_IP6_STATIC(p_dpib_m,p_inner_ip6_static_m)\
    (p_dpib_m->inner_ip6_static = p_inner_ip6_static_m)

/******************************************************************************
 *  Macro Name      : ROHC_DPIB_GET_OUTER_IP6_STATIC
 *
 *  Description     : Returns the pointer to outer_ip6_static from DPIB
 *
 *****************************************************************************/

#define ROHC_DPIB_GET_OUTER_IP6_STATIC(p_dpib_m)\
    (p_dpib_m->outer_ip6_static)
    

/******************************************************************************
 *  Macro Name      : ROHC_DPIB_SET_OUTER_IP6_STATIC
 *
 *  Description     : Updates the pointer to outer_ip6_static in DPIB
 *
 *****************************************************************************/

#define ROHC_DPIB_SET_OUTER_IP6_STATIC(p_dpib_m,p_outer_ip6_static_m)\
    (p_dpib_m->outer_ip6_static = p_outer_ip6_static_m)


/******************************************************************************
 *  Macro Name      : ROHC_DPIB_GET_INNER_IP4_STATIC
 *
 *  Description     : Returns the pointer to inner_ip4_static from DPIB
 *
 *****************************************************************************/

#define ROHC_DPIB_GET_INNER_IP4_STATIC(p_dpib_m)\
    (p_dpib_m->inner_ip4_static)
    

/******************************************************************************
 *  Macro Name      : ROHC_DPIB_SET_INNER_IP4_STATIC
 *
 *  Description     : Updates the pointer to inner_ip4_static in DPIB
 *
 *****************************************************************************/

#define ROHC_DPIB_SET_INNER_IP4_STATIC(p_dpib_m,p_inner_ip5_static_m)\
    (p_dpib_m->inner_ip4_static = p_inner_ip4_static_m)

/******************************************************************************
 *  Macro Name      : ROHC_DPIB_GET_OUTER_IP4_STATIC
 *
 *  Description     : Returns the pointer to outer_ip4_static from DPIB
 *
 *****************************************************************************/

#define ROHC_DPIB_GET_OUTER_IP4_STATIC(p_dpib_m)\
    (p_dpib_m->outer_ip4_static)
    

/******************************************************************************
 *  Macro Name      : ROHC_DPIB_SET_OUTER_IP4_STATIC
 *
 *  Description     : Updates the pointer to outer_ip4_static in DPIB
 *
 *****************************************************************************/

#define ROHC_DPIB_SET_OUTER_IP4_STATIC(p_dpib_m,p_outer_ip4_static_m)\
    (p_dpib_m->outer_ip4_static = p_outer_ip4_static_m)

/**************/
/******************************************************************************
 *  Macro Name      : ROHC_DPIB_GET_INNER_IP6_DYN
 *
 *  Description     : Returns the pointer to inner_ip6_dyn from DPIB
 *
 *****************************************************************************/

#define ROHC_DPIB_GET_INNER_IP6_DYN(p_dpib_m)\
    (p_dpib_m->inner_ip6_dyn)
    
/******************************************************************************
 *  Macro Name      : ROHC_DPIB_SET_INNER_IP6_DYN
 *
 *  Description     : Updates the pointer to inner_ip6_dyn in DPIB
 *
 *****************************************************************************/

#define ROHC_DPIB_SET_INNER_IP6_DYN(p_dpib_m,p_inner_ip6_dyn_m)\
    (p_dpib_m->inner_ip6_dyn = p_inner_ip6_dyn_m)

/******************************************************************************
 *  Macro Name      : ROHC_DPIB_GET_OUTER_IP6_DYN
 *
 *  Description     : Returns the pointer to outer_ip6_dyn from DPIB
 *
 *****************************************************************************/

#define ROHC_DPIB_GET_OUTER_IP6_DYN(p_dpib_m)\
    (p_dpib_m->outer_ip6_dyn)
    

/******************************************************************************
 *  Macro Name      : ROHC_DPIB_SET_OUTER_IP6_DYN
 *
 *  Description     : Updates the pointer to outer_ip6_dyn in DPIB
 *
 *****************************************************************************/

#define ROHC_DPIB_SET_OUTER_IP6_DYN(p_dpib_m,p_outer_ip6_dyn_m)\
    (p_dpib_m->outer_ip6_dyn = p_outer_ip6_dyn_m)


/******************************************************************************
 *  Macro Name      : ROHC_DPIB_GET_INNER_IP4_DYN
 *
 *  Description     : Returns the pointer to inner_ip4_dyn from DPIB
 *
 *****************************************************************************/

#define ROHC_DPIB_GET_INNER_IP4_DYN(p_dpib_m)\
    (p_dpib_m->inner_ip4_dyn)
    

/******************************************************************************
 *  Macro Name      : ROHC_DPIB_SET_INNER_IP4_DYN
 *
 *  Description     : Updates the pointer to inner_ip4_dyn in DPIB
 *
 *****************************************************************************/

#define ROHC_DPIB_SET_INNER_IP4_DYN(p_dpib_m,p_inner_ip4_dyn_m)\
    (p_dpib_m->inner_ip4_dyn = p_inner_ip4_dyn_m)

/******************************************************************************
 *  Macro Name      : ROHC_DPIB_GET_OUTER_IP4_DYN
 *
 *  Description     : Returns the pointer to outer_ip4_dyn from DPIB
 *
 *****************************************************************************/

#define ROHC_DPIB_GET_OUTER_IP4_DYN(p_dpib_m)\
    (p_dpib_m->outer_ip4_dyn)
    

/******************************************************************************
 *  Macro Name      : ROHC_DPIB_SET_OUTER_IP4_DYN
 *
 *  Description     : Updates the pointer to outer_ip4_dyn in DPIB
 *
 *****************************************************************************/

#define ROHC_DPIB_SET_OUTER_IP4_DYN(p_dpib_m,p_outer_ip4_dyn_m)\
    (p_dpib_m->outer_ip4_dyn = p_outer_ip4_dyn_m)

/******************************************************************************
 *  Macro Name      : ROHC_DPIB_GET_UDP_STATIC
 *
 *  Description     : Returns the pointer to udp_static from DPIB
 *
 *****************************************************************************/

#define ROHC_DPIB_GET_UDP_STATIC(p_dpib_m)\
    (p_dpib_m->udp_static)
    

/******************************************************************************
 *  Macro Name      : ROHC_DPIB_SET_OUTER_IP4_DYN
 *
 *  Description     : Updates the pointer to outer_ip4_dyn in DPIB
 *
 *****************************************************************************/

#define ROHC_DPIB_SET_UDP_STATIC(p_dpib_m,p_udp_static_m)\
    (p_dpib_m->udp_static = p_udp_static_m)


#define ROHC_DPIB_GET_UDP_DYN(p_dpib_m)\
    (p_dpib_m->udp_dyn)

#define ROHC_DPIB_SET_UDP_DYN(p_dpib_m,p_udp_dyn_m)\
    (p_dpib_m->udp_dyn = p_udp_dyn_m)

#define ROHC_DPIB_GET_RTP_STATIC(p_dpib_m)\
    (p_dpib_m->rtp_static)

#define ROHC_DPIB_SET_RTP_STATIC(p_dpib_m,p_rtp_static_m)\
    (p_dpib_m->rtp_static = p_rtp_static_m)

#define ROHC_DPIB_GET_HEAD_DYN(p_dpib_m)\
    (p_dpib_m->rtp_head_dyn)

#define ROHC_DPIB_SET_HEAD_DYN(p_dpib_m,p_rtp_head_dyn_m)\
    (p_dpib_m->rtp_head_dyn = p_rtp_head_dyn_m)

#define ROHC_DPIB_GET_TAIL_DYN(p_dpib_m)\
    (p_dpib_m->rtp_tail_dyn)

#define ROHC_DPIB_SET_TAIL_DYN(p_dpib_m,p_rtp_tail_dyn_m)\
    (p_dpib_m->rtp_tail_dyn = p_rtp_tail_dyn_m)

#define ROHC_DPIB_GET_ESP_STATIC(p_dpib_m)\
    (p_dpib_m->esp_static)

#define ROHC_DPIB_SET_ESP_STATIC(p_dpib_m,p_esp_static_m)\
    (p_dpib_m->esp_static = p_esp_static_m)

#define ROHC_DPIB_GET_ESP_DYN(p_dpib_m)\
    (p_dpib_m->esp_dyn)

#define ROHC_DPIB_SET_ESP_DYN(p_dpib_m,p_esp_dyn_m)\
    (p_dpib_m->esp_dyn = p_esp_dyn_m)

#define ROHC_DPIB_GET_INNER_AH_PRT(p_dpib_m)\
    (p_dpib_m->inner_ah_ptr)

#define ROHC_DPIB_SET_INNER_AH_PTR(p_dpib_m,p_inner_ah_ptr_m)\
    (p_dpib_m->inner_ah_ptr = p_inner_ah_ptr_m)

#define ROHC_DPIB_GET_OUTER_AH_PRT(p_dpib_m)\
    (p_dpib_m->outer_ah_ptr)

#define ROHC_DPIB_SET_OUTER_AH_PTR(p_dpib_m,p_outer_ah_ptr_m)\
    (p_dpib_m->outer_ah_ptr = p_outer_ah_ptr_m)

#define ROHC_DPIB_GET_INNER_GRE_PRT(p_dpib_m)\
    (p_dpib_m->inner_gre_ptr)

#define ROHC_DPIB_SET_INNER_GRE_PTR(p_dpib_m,p_inner_gre_ptr_m)\
    (p_dpib_m->inner_gre_ptr = p_inner_gre_ptr_m)

#define ROHC_DPIB_GET_OUTER_GRE_PRT(p_dpib_m)\
    (p_dpib_m->outer_gre_ptr)

#define ROHC_DPIB_SET_OUTER_GRE_PTR(p_dpib_m,p_outer_gre_ptr_m)\
    (p_dpib_m->outer_gre_ptr = p_outer_gre_ptr_m)

#define ROHC_DPIB_GET_PAYLOAD(p_dpib_m)\
    (p_dpib_m->payload)

#define ROHC_DPIB_SET_PAYLOAD(p_dpib_m,p_payload_m)\
    (p_dpib_m->payload = p_payload_m)

#define ROHC_DPIB_GET_UDP_CKSUM(p_dpib_m)\
    (p_dpib_m->udp_ck_sum)

#define ROHC_DPIB_SET_UDP_CKSUM(p_dpib_m,p_upd_cksum_m)\
    (p_dpib_m->upd_cksum = p_upd_cksum_m)

#define ROHC_DPIB_GET_PROFILE(p_dpib_m)\
    (p_dpib_m->profile)

#define ROHC_DPIB_SET_PROFILE(p_dpib_m,p_profile_m)\
    (p_dpib_m->profile= p_profile_m)


/* Contains GET, SET Macros of DSC */

#define ROHC_DSC_GET_INNER_IP4(p_dsc_m) (p_dsc_m->inner_ip4)
#define ROHC_DSC_SET_INNER_IP4(p_dsc_m, p_innerip4_m) \
    (p_dsc_m->inner_ip4 = p_innerip4_m)

#define ROHC_DSC_GET_OUTER_IP4(p_dsc_m) (p_dsc_m->outer_ip4)
#define ROHC_DSC_SET_OUTER_IP4(p_dsc_m, p_outerip4_m) \
    (p_dsc_m->outer_ip4 = p_outerip4_m)

#define ROHC_DSC_GET_INNER_IP6(p_dsc_m) (p_dsc_m->inner_ip6)
#define ROHC_DSC_SET_INNER_IP6(p_dsc_m, p_innerip6_m) \
    (p_dsc_m->inner_ip6 = p_innerip6_m)

#define ROHC_DSC_GET_OUTER_IP6(p_dsc_m) (p_dsc_m->outer_ip6)
#define ROHC_DSC_SET_OUTER_IP6(p_dsc_m, p_outerip6_m) \
    (p_dsc_m->outer_ip6 = p_outerip6_m)

#define ROHC_DSC_GET_RTP(p_dsc_m) (p_dsc_m->rtp)
#define ROHC_DSC_SET_RTP(p_dsc_m, p_rtp_m) \
    (p_dsc_m->rtp = p_rtp_m)

#define ROHC_DSC_GET_UDP(p_dsc_m) (p_dsc_m->udp)
#define ROHC_DSC_SET_UDP(p_dsc_m, p_udp_m) \
    (p_dsc_m->udp = p_udp_m)

#define ROHC_DSC_GET_TCP(p_dsc_m) (p_dsc_m->tcp)
#define ROHC_DSC_SET_TCP(p_dsc_m, p_tcp_m) \
    (p_dsc_m->tcp = p_tcp_m)

#define ROHC_DSC_GET_ESP(p_dsc_m) (p_dsc_m->esp)
#define ROHC_DSC_SET_ESP(p_dsc_m, p_esp_m) \
    (p_dsc_m->esp = p_esp_m)

#define ROHC_DSC_GET_PAYLOAD(p_dsc_m) (p_dsc_m->payload)
#define ROHC_DSC_SET_PAYLOAD(p_dsc_m, p_payload_m) \
    (p_dsc_m->payload = p_payload_m)

#define ROHC_DSC_GET_RND(p_dsc_m) (p_dsc_m->rnd)
#define ROHC_DSC_SET_RND(p_dsc_m, rnd_m) \
    (p_dsc_m->rnd = rnd_m)

#define ROHC_DSC_GET_RND2(p_dsc_m) (p_dsc_m->rnd2)
#define ROHC_DSC_SET_RND2(p_dsc_m, rnd2_m) \
    (p_dsc_m->rnd2 = rnd2_m)

#define ROHC_DSC_GET_NBO(p_dsc_m) (p_dsc_m->nbo)
#define ROHC_DSC_SET_NBO(p_dsc_m, nbo_m) \
    (p_dsc_m->nbo = nbo_m)

#define ROHC_DSC_GET_NBO2(p_dsc_m) (p_dsc_m->nbo2)
#define ROHC_DSC_SET_NBO2(p_dsc_m, nbo2_m) \
    (p_dsc_m->nbo2 = nbo2_m)

#define ROHC_DSC_GET_DPIB(p_dsc_m) (p_dsc_m->dpib)
#define ROHC_DSC_SET_DPIB(p_dsc_m, p_dpib_m) \
    (p_dsc_m->dpib = p_dpib_m)

#define ROHC_DSC_GET_PROFILE(p_dsc_m) (p_dsc_m->profile)
#define ROHC_DSC_SET_PROFILE(p_dsc_m, profile_m) \
    (p_dsc_m->profile = profile_m)

#define ROHC_DSC_GET_STATE(p_dsc_m) (p_dsc_m->d_state)
#define ROHC_DSC_SET_STATE(p_dsc_m, state_m) \
    (p_dsc_m->d_state = state_m)

#define ROHC_DSC_GET_TRANS_STATE(p_dsc_m) (p_dsc_m->d_trans_state)
#define ROHC_DSC_SET_TRANS_STATE(p_dsc_m, tstate_m) \
    (p_dsc_m->d_trans_state = tstate_m)

#define ROHC_DSC_GET_MODE(p_dsc_m) (p_dsc_m->d_mode)
#define ROHC_DSC_SET_MODE(p_dsc_m, mode_m) \
    (p_dsc_m->d_mode = mode_m)

#define ROHC_DSC_GET_ACKED(p_dsc_m) (p_dsc_m->ack_recvd)
#define ROHC_DSC_SET_ACKED(p_dsc_m, ack_recvd_m) \
    (p_dsc_m->ack_recvd = ack_recvd_m)

#define ROHC_DSC_GET_TSS(p_dsc_m) (p_dsc_m->tss)
#define ROHC_DSC_SET_TSS(p_dsc_m, tss_m) \
    (p_dsc_m->tss = tss_m)

#define ROHC_DSC_GET_TIS(p_dsc_m) (p_dsc_m->tis)
#define ROHC_DSC_SET_TIS(p_dsc_m, tis_m) \
    (p_dsc_m->tis = tis_m)

#define ROHC_DSC_GET_AVG_PKT_ARR_TIME(p_dsc_m) (p_dsc_m->avg_pkt_arrival_time)
#define ROHC_DSC_SET_AVG_PKT_ARR_TIME(p_dsc_m, avg_pkt_arr_time_m) \
    (p_dsc_m->avg_pkt_arrival_time = avg_pkt_arr_time_m)

#define ROHC_DSC_GET_PKT_ARR_TIME(p_dsc_m) (p_dsc_m->pkt_arrival_time)
#define ROHC_DSC_SET_PKT_ARR_TIME(p_dsc_m, pkt_arr_time_m) \
    (p_dsc_m->pkt_arrival_time = pkt_arr_time_m)

#define ROHC_DSC_GET_PREV_SEQ_NO(p_dsc_m) (p_dsc_m->seq_no_prev)
#define ROHC_DSC_SET_PREV_SEQ_NO(p_dsc_m, seq_no_prev_m) \
    (p_dsc_m->seq_no_prev = seq_no_prev_m)

#define ROHC_DSC_GET_PREV_PREV_SEQ_NO(p_dsc_m) (p_dsc_m->seq_no_prev_prev)
#define ROHC_DSC_SET_PREV_PREV_SEQ_NO(p_dsc_m, seq_no_p_p_m) \
    (p_dsc_m->seq_no_prev_prev = seq_no_p_p_m)

#define ROHC_DSC_GET_UDP_CHKSUM_PRESENT(p_dsc_m) (p_dsc_m->udp_cksum_present)
#define ROHC_DSC_SET_UDP_CHKSUM_PRESENT(p_dsc_m, udp_chksum_m) \
    (p_dsc_m->udp_cksum_present = udp_chksum_m)

#define ROHC_GET_DSC_CID(p_dsc_m)\
			((p_dsc_m)->cid)


#endif /* _ROHC_DCOM_MCR_H_ */
