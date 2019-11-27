/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_ccom_mcr.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
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
 * $Log: rohc_ccom_mcr.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:34:55  gur19140
 * first wave of Integration
 *
 * Revision 1.3  2009/05/28 04:41:35  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_CCOM_MCR_H_
#define _ROHC_CCOM_MCR_H_

/* Contains GET, SET Macros for CSC */

#define ROHC_CSC_GET_SN(p_csc_m) ((p_csc_m)->udp_sn)
#define ROHC_CSC_SET_SN(p_csc_m, sn_m) ((p_csc_m)->udp_sn = (sn_m))

#define ROHC_CSC_GET_TS_STRIDE(p_csc_m) ((p_csc_m)->ts_stride)
#define ROHC_CSC_SET_TS_STRIDE(p_csc_m, ts_stride_m) \
        ((p_csc_m)->ts_stride = (ts_stride_m))

#define ROHC_CSC_SET_ACK_STRIDE(p_csc_m, ack_stride_m) \
        ((p_csc_m)->ack_stride = (ack_stride_m))

#define ROHC_CSC_GET_OLD_INNER_RND(p_csc_m) ((p_csc_m)->old_inner_rnd)
#define ROHC_CSC_SET_OLD_INNER_RND(p_csc_m, old_inner_rnd_m) \
        ((p_csc_m)->old_inner_rnd = (old_inner_rnd_m))

#define ROHC_CSC_GET_NEW_INNER_RND(p_csc_m) ((p_csc_m)->new_inner_rnd)
#define ROHC_CSC_SET_NEW_INNER_RND(p_csc_m, new_inner_rnd_m) \
        ((p_csc_m)->new_inner_rnd = (new_inner_rnd_m))

#define ROHC_CSC_GET_NEW_OUTER_RND(p_csc_m) ((p_csc_m)->new_outer_rnd)
#define ROHC_CSC_SET_NEW_OUTER_RND(p_csc_m, new_outer_rnd_m) \
        ((p_csc_m)->new_outer_rnd = (new_outer_rnd_m))

#define ROHC_CSC_GET_OLD_OUTER_RND(p_csc_m) ((p_csc_m)->old_outer_rnd)
#define ROHC_CSC_SET_OLD_OUTER_RND(p_csc_m, old_outer_rnd_m) \
        ((p_csc_m)->old_outer_rnd = (old_outer_rnd_m))

#define ROHC_CSC_GET_OLD_INNER_NBO(p_csc_m) ((p_csc_m)->old_inner_nbo)
#define ROHC_CSC_SET_OLD_INNER_NBO(p_csc_m, old_inner_nbo_m) \
        ((p_csc_m)->old_inner_nbo = (old_inner_nbo_m))

#define ROHC_CSC_GET_NEW_INNER_NBO(p_csc_m) ((p_csc_m)->new_inner_nbo)
#define ROHC_CSC_SET_NEW_INNER_NBO(p_csc_m, new_inner_nbo_m) \
        ((p_csc_m)->new_inner_nbo = (new_inner_nbo_m))

#define ROHC_CSC_GET_OLD_OUTER_NBO(p_csc_m) ((p_csc_m)->old_outer_nbo)
#define ROHC_CSC_SET_OLD_OUTER_NBO(p_csc_m, old_outer_nbo_m) \
        ((p_csc_m)->old_outer_nbo = (old_outer_nbo_m))

#define ROHC_CSC_GET_NEW_OUTER_NBO(p_csc_m) ((p_csc_m)->new_outer_nbo)
#define ROHC_CSC_SET_NEW_OUTER_NBO(p_csc_m, new_outer_nbo_m) \
        ((p_csc_m)->new_outer_nbo = (new_outer_nbo_m))

#define ROHC_CSC_GET_OLD_RTP_SN(p_csc_m) ((p_csc_m)->old_rtp_sn)
#define ROHC_CSC_SET_OLD_RTP_SN(p_csc_m, old_rtp_sn_m) \
        ((p_csc_m)->old_rtp_sn = (old_rtp_sn_m))

#define ROHC_CSC_GET_OLD_RTP_TS(p_csc_m) ((p_csc_m)->old_rtp_ts)
#define ROHC_CSC_SET_OLD_RTP_TS(p_csc_m, old_rtp_ts_m) \
        ((p_csc_m)->old_rtp_ts = (old_rtp_ts_m))

#define ROHC_CSC_GET_OLD_TCP_ACK(p_csc_m) ((p_csc_m)->old_tcp_ack)
#define ROHC_CSC_SET_OLD_TCP_ACK(p_csc_m, old_tcp_ack_m) \
        ((p_csc_m)->old_tcp_ack = (old_tcp_ack_m))

#define ROHC_CSC_SET_TCP_SN_STRIDE(p_csc_m, payload_len_m) \
        ((p_csc_m)->sn_stride = (payload_len_m))
#define ROHC_CSC_SET_TCP_SN_RESIDUE(p_csc_m, temp_sn_m) \
        ((p_csc_m)->temp_sn_residue = (temp_sn_m)% (p_csc_m)->sn_stride)

#define ROHC_CSC_SET_TCP_ACK_RESIDUE(p_csc_m, temp_ack_m) \
        ((p_csc_m)->temp_ack_residue = (temp_ack_m)% (p_csc_m)->ack_stride)
#define ROHC_CSC_TCP_ECN_FLAGS(p_cpib_m) \
        ((((rohc_tcp_t*)((struct rohc_cpib_t *)p_cpib_m)->tcp)->flags\
          & ROHC_TCP_ECN_MASK)>>6)

#define ROHC_CSC_INNER_IP_ECN_FLAGS(p_cpib_m) \
        (((rohc_ip4_t*)((struct rohc_cpib_t *)p_cpib_m)->inner_ip4)->tos & \
         ROHC_PROF6_IP4_ECN_FLAG_MASK)

#define ROHC_CSC_GET_CPIB(p_csc_m) ((struct rohc_cpib_t *) ((p_csc_m)->p_cpib))
#define ROHC_CSC_SET_CPIB(p_csc_m, p_cpib_m) \
        ((p_csc_m)->p_cpib = (struct rohc_cpib_t *)(p_cpib_m))

#define ROHC_CSC_GET_CURR_INNERIPID_INDEX(p_csc_m) \
        ((p_csc_m)->cur_inneripid_index)
#define ROHC_CSC_SET_CURR_INNERIPID_INDEX(p_csc_m, cur_inneripid_idx_m) \
        ((p_csc_m)->cur_inneripid_index = (cur_inneripid_idx_m))

#define ROHC_CSC_GET_CURR_OUTERIPID_INDEX(p_csc_m) \
        ((p_csc_m)->cur_outeripid_index)
#define ROHC_CSC_SET_CURR_OUTERIPID_INDEX(p_csc_m, cur_outeripid_idx_m) \
        ((p_csc_m)->cur_outeripid_index = (cur_outeripid_idx_m))

#define ROHC_CSC_GET_INNERIPID(p_csc_m, index_m) \
        ((p_csc_m)->inneripid[index_m])
#define ROHC_CSC_SET_INNERIPID(p_csc_m, inner_ipid_m, index_m) \
        ((p_csc_m)->inneripid[index_m] = (inner_ipid_m))

#define ROHC_CSC_GET_OUTERIPID(p_csc_m, index_m) \
        ((p_csc_m)->outeripid[index_m])
#define ROHC_CSC_SET_OUTERIPID(p_csc_m, outer_ipid_m, index_m) \
        ((p_csc_m)->outeripid[index_m] = (outer_ipid_m))

#define ROHC_CSC_GET_INNERIP6_SRC(p_csc_m) ((p_csc_m)->innerip6_src)
#define ROHC_CSC_SET_INNERIP6_SRC(p_csc_m, innerip6_src_m) \
    (ROHC_MEMCPY((p_csc_m)->innerip6_src, (innerip6_src_m), sizeof((p_csc_m)->innerip6_src))

#define ROHC_CSC_GET_OUTERIP6_SRC(p_csc_m) ((p_csc_m)->outerip6_src)
#define ROHC_CSC_SET_OUTERIP6_SRC(p_csc_m, outerip6_src_m) \
    (ROHC_MEMCPY((p_csc_m)->outerip6_src, (outerip6_src_m), sizeof((p_csc_m)->outerip6_src))

#define ROHC_CSC_GET_INNERIP6_DST(p_csc_m) ((p_csc_m)->innerip6_dst)
#define ROHC_CSC_SET_INNERIP6_DST(p_csc_m, innerip6_dst_m) \
    (ROHC_MEMCPY((p_csc_m)->innerip6_dst, (innerip6_dst_m), sizeof((p_csc_m)->innerip6_dst))

#define ROHC_CSC_GET_OUTERIP6_DST(p_csc_m) ((p_csc_m)->outerip6_dst)
#define ROHC_CSC_SET_OUTERIP6_DST(p_csc_m, outerip6_dst_m) \
    (ROHC_MEMCPY((p_csc_m)->outerip6_dst, (outerip6_dst_m), sizeof((p_csc_m)->outerip6_dst))

#define ROHC_CSC_GET_PROFILE(p_csc_m) ((p_csc_m)->profile)
#define ROHC_CSC_SET_PROFILE(p_csc_m, profile_m) \
        ((p_csc_m)->profile = (profile_m))

#define ROHC_CSC_GET_UDP_CHKSUM_PRESENT(p_csc_m) \
        ((p_csc_m)->udp_cksum_present)
#define ROHC_CSC_SET_UDP_CHKSUM_PRESENT(p_csc_m, udp_cksum_present_m) \
        ((p_csc_m)->udp_cksum_present = (udp_cksum_present_m))

#define ROHC_CSC_GET_ACKED(p_csc_m) ((p_csc_m)->ack_recvd)
#define ROHC_CSC_SET_ACKED(p_csc_m, ack_recvd_m) \
        ((p_csc_m)->ack_recvd = (ack_recvd_m))

#define ROHC_CSC_GET_PRTT(p_csc_m) ((p_csc_m)->prtt)
#define ROHC_CSC_SET_PRTT(p_csc_m, prtt_m) \
        ((p_csc_m)->prtt = (prtt_m))

#define ROHC_CSC_GET_RTT(p_csc_m) ((p_csc_m)->rtt)
#define ROHC_CSC_SET_RTT(p_csc_m, rtt_m) \
        ((p_csc_m)->rtt = (rtt_m))

#define ROHC_CSC_GET_MAX_JITTER_CD(p_csc_m) ((p_csc_m)->max_jitter_cd)
#define ROHC_CSC_SET_MAX_JITTER_CD(p_csc_m, max_jitter_cd_m) \
        ((p_csc_m)->max_jitter_cd = (max_jitter_cd_m))

#define ROHC_CSC_GET_CURR_TIME(p_csc_m) ((p_csc_m)->cur_time)
#define ROHC_CSC_SET_CURR_TIME(p_csc_m, cur_time_m) \
        ((p_csc_m)->cur_time = (cur_time_m))

#define ROHC_CSC_GET_CLK_RESOLUTION(p_csc_m) ((p_csc_m)->clk_resolution)
#define ROHC_CSC_SET_CLK_RESOLUTION(p_csc_m, clk_resolution_m) \
        ((p_csc_m)->clk_resolution = (clk_resolution_m))

#define ROHC_CSC_GET_MAX_JITTER(p_csc_m) ((p_csc_m)->max_jitter)
#define ROHC_CSC_SET_MAX_JITTER(p_csc_m, max_jitter_m) \
        ((p_csc_m)->max_jitter = (max_jitter_m))

#define ROHC_CSC_GET_STATE(p_csc_m) ((p_csc_m)->c_state)
#define ROHC_CSC_SET_STATE(p_csc_m, state_m) \
        ((p_csc_m)->c_state = (state_m))

#define ROHC_CSC_GET_TRANS_STATE(p_csc_m) ((p_csc_m)->c_trans_state)
#define ROHC_CSC_SET_TRANS_STATE(p_csc_m, t_state_m) \
        ((p_csc_m)->c_trans_state = (t_state_m))

#define ROHC_CSC_GET_MODE(p_csc_m) ((p_csc_m)->c_mode)
#define ROHC_CSC_SET_MODE(p_csc_m, mode_m) \
        ((p_csc_m)->c_mode = (mode_m))

#define ROHC_CSC_GET_GSW_REF(p_csc_m) \
        ((p_csc_m)->rohc_gsw.rohc_gsw_elem[(p_csc_m)->rohc_gsw.start_index])
#define ROHC_CSC_GET_GSW_NEXT(p_csc_m) \
        ((p_csc_m)->rohc_gsw.rohc_gsw_elem[(p_csc_m)->rohc_gsw.end_index])

#define ROHC_CSC_GET_CID(p_csc_m) \
        ((p_csc_m)->field_value[ROHC_CID_FID-ROHC_VAR_FID_BASE])





#define ROHC_CPIB_GET_PAYLOAD(p_cpib_m)     (((struct rohc_cpib_t *)(p_cpib_m))\
                                            ->payload)
#define ROHC_CPIB_SET_PAYLOAD(p_cpib_m,x_m) (((struct rohc_cpib_t *)(p_cpib_m))\
                                            ->payload=(x_m))

#define ROHC_CPIB_GET_INNER_AH_PTR(p_cpib_m) (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->inner_ah_ptr)
#define ROHC_CPIB_SET_INNER_AH_PTR(p_cpib_m,x_m)    (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->inner_ah_ptr=(x_m))

#define ROHC_CPIB_GET_OUTER_AH_PTR(p_cpib_m) (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->outer_ah_ptr)
#define ROHC_CPIB_SET_OUTER_AH_PTR(p_cpib_m,x_m)    (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->outer_ah_ptr=(x_m))


#define ROHC_CPIB_GET_INNER_IP4_PTR(p_cpib_m) (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->inner_ip4)
#define ROHC_CPIB_SET_INNER_IP4_PTR(p_cpib_m,x_m)   (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->inner_ip4=(x_m))


#define ROHC_CPIB_GET_OUTER_IP4_PTR(p_cpib_m) (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->outer_ip4)
#define ROHC_CPIB_SET_OUTER_IP4_PTR(p_cpib_m,x_m)   (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->outer_ip4=(x_m))

#define ROHC_CPIB_GET_INNER_IP6_PTR(p_cpib_m) (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->inner_ip6)
#define ROHC_CPIB_SET_INNER_IP6_PTR(p_cpib_m,x_m)   (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->inner_ip6=(x_m))


#define ROHC_CPIB_GET_OUTER_IP6_PTR(p_cpib_m) (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->outer_ip6)
#define ROHC_CPIB_SET_OUTER_IP6_PTR(p_cpib_m,x_m)   (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->outer_ip6=(x_m))


#define ROHC_CPIB_GET_INNER_GRE_PTR(p_cpib_m) (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->inner_gre)
#define ROHC_CPIB_SET_INNER_GRE_PTR(p_cpib_m,x_m)   (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->inner_gre=(x_m))


#define ROHC_CPIB_GET_OUTER_GRE_PTR(p_cpib_m) (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->outer_gre)
#define ROHC_CPIB_SET_OUTER_GRE_PTR(p_cpib_m,x_m)   (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->outer_gre=(x_m))


#define ROHC_CPIB_GET_UDP_PTR(p_cpib_m) (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->udp)
#define ROHC_CPIB_SET_UDP_PTR(p_cpib_m,x_m) (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->udp=(x_m))


#define ROHC_CPIB_GET_RTP_PTR(p_cpib_m) (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->rtp)
#define ROHC_CPIB_SET_RTP_PTR(p_cpib_m,x_m) (((struct rohc_cpib_t*)\
                                              (p_cpib_m))->rtp=(x_m))
#define ROHC_CPIB_GET_TCP_PTR(p_cpib_m) (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->tcp)

#define ROHC_CPIB_GET_ESP_PTR(p_cpib_m) (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->esp)
#define ROHC_CPIB_SET_ESP_PTR(p_cpib_m,x_m) (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->esp=(x_m))

#define ROHC_CPIB_GET_TOTAL_PKT_LEN_PTR(p_cpib_m) (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->total_pkt_len)
#define ROHC_CPIB_SET_TOTAL_PKT_LEN_PTR(p_cpib_m,x_m)   (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->total_pkt_len=(x_m))

#define ROHC_CPIB_GET_AH_COUNT(p_cpib_m) (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->ah_count
#define ROHC_CPIB_SET_AH_COUNT(p_cpib_m,x_m)    (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->ah_count=(x_m))


#define ROHC_CPIB_GET_GRE_COUNT(p_cpib_m) (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->gre_count
#define ROHC_CPIB_SET_GRE_COUNT(p_cpib_m,x_m)   (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->gre_count=(x_m))


#define ROHC_CPIB_GET_PROFILE(p_cpib_m) (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->determined_profile)
#define ROHC_CPIB_SET_PROFILE(p_cpib_m,x_m) (((struct rohc_cpib_t *)\
                                        (p_cpib_m))->determined_profile=(x_m))


#define ROHC_CPIB_GET_UDP_CKSUM_PRESENT(p_cpib_m) (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->udp_cksum_present)
#define ROHC_CPIB_SET_UDP_CKSUM_PRESENT(p_cpib_m,x_m)   (((struct rohc_cpib_t *)\
                                        (p_cpib_m))->udp_cksum_present=(x_m))
#define ROHC_CPIB_SET_TCP_CKSUM_PRESENT(p_cpib_m,x_m)   (((struct rohc_cpib_t *)\
                                        (p_cpib_m))->tcp_cksum_present=(x_m))

#define ROHC_CPIB_GET_IP4_COUNT(p_cpib_m) (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->ip4_count)
#define ROHC_CPIB_SET_IP4_COUNT(p_cpib_m,x_m)   (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->ip4_count=(x_m))

#define ROHC_CPIB_GET_IP6_COUNT(p_cpib_m) (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->ip6_count)
#define ROHC_CPIB_SET_IP6_COUNT(p_cpib_m,x_m)   (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->ip6_count=(x_m))

#define ROHC_CPIB_GET_INNER_AH_LEN(p_cpib_m) (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->inner_ah_len)
#define ROHC_CPIB_SET_INNER_AH_LEN(p_cpib_m,x_m)    (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->inner_ah_len=(x_m))


#define ROHC_CPIB_GET_OUTER_AH_LEN(p_cpib_m) (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->outer_ah_len)
#define ROHC_CPIB_SET_OUTER_AH_LEN(p_cpib_m,x_m)    (((struct rohc_cpib_t *)\
                                            (p_cpib_m))->outer_ah_len=(x_m))

#define ROHC_GET_CSC_CID(p_csc_m) \
        ((p_csc_m)->field_value[ROHC_CID_FID-ROHC_VAR_FID_BASE])
#endif /* _ROHC_CCOM_MCR_H_ */
