/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_cprm_mcr.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the macros definition for CPRM
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_cprm_mcr.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.2  2009/05/28 05:46:18  gur19836
 * File header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_CPRM_MCR_
#define _ROHC_CPRM_MCR_
/* bitmap manupulation macros */
#include "rohc_ccom_mcr.h"
#include "rohc_db_ext_if.h"

/* initalizing */
#define ROHC_CPRM_BIT_INIT(m_f,m_s,m_count) \
do{\
    rohc_U8bit_t i =0;\
    for(i = 0; i <m_count ;i++)\
    {\
    m_f[i] = m_s[i];\
    }\
}while(0)             



/* performng bitwise AND */
#define ROHC_CPRM_BIT_AND(m_f,m_s,m_count)\
do{\
    rohc_U8bit_t i =0;\
    for(i = 0; i <m_count ;i++)\
    {\
    m_f[i] &= m_s[i];\
    }\
}while(0)

/* performng bitwise AND */
#define ROHC_CPRM_KEEP_REMOVE(m_k,m_f,m_s,m_count)\
do{\
    rohc_U8bit_t i =0;\
    for(i = 0; i <m_count ;i++)\
    {\
    m_k[i] = (m_f[i]&(~m_s[i]));\
    m_f[i] &= m_s[i];\
    }\
}while(0)
    
/* Compute the number of packet and their id in the bitmap */   
#define ROHC_CPRM_BIT_SEL_PKT(m_bit_map, m_count, m_sel_pkt,m_num)\
do{\
    rohc_U8bit_t i =0;\
    m_num = 0;\
    for(i = 0; i < 8*m_count ;i++)\
    {\
        if(((m_bit_map[i/8]) & (1<< (i%8)) ) != 0)\
        {   \
            m_sel_pkt[m_num++] = i;\
        }\
    }\
}while(0)

#define ROHC_IS_ITH_BIT_SET(m_bit_map, m_i)\
    (((m_bit_map[m_i/8]) & (1<< (m_i%8)) ) != 0)


/* 
Not needed the number of bits set for TS (0) denotes the absence of 
ts field 
*/
/*
#define ROHC_CPRM_IS_CH_TS(p_csc_m)\
    (ROHC_CPIB_GET_RTP_PTR(ROHC_CSC_GET_CPIB(p_csc_m)) != ROHC_NULL)
*/                
#define ROHC_CPRM_IS_CH_R_PT(p_csc_m)\
    (p_csc_m->field_value[ROHC_RTP_PT_FID-ROHC_VAR_FID_BASE] != \
    (((rohc_rtp_t *)p_csc_m->p_cpib->rtp)->m_pt & ROHC_PT_MASK))

       
#define ROHC_CPRM_IS_CH_R_X(p_csc_m)\
    (p_csc_m->field_value[ROHC_RTP_X_FID-ROHC_VAR_FID_BASE] != \
    ((((rohc_rtp_t *) p_csc_m->p_cpib->rtp)->v_p_x_cc\
    & ROHC_X_MASK))>>4)


#define ROHC_CPRM_IS_CH_R_M(p_csc_m)\
    (p_csc_m->field_value[ROHC_RTP_M_FID-ROHC_VAR_FID_BASE] != \
        ((((rohc_rtp_t *)ROHC_CPIB_GET_RTP_PTR(ROHC_CSC_GET_CPIB(p_csc_m)))->m_pt\
    & ROHC_M_MASK)>>7))      


#define ROHC_CPRM_IS_CH_CSRC(p_csc_m)\
    ( 0 != \
    (((rohc_rtp_t *)ROHC_CPIB_GET_RTP_PTR(ROHC_CSC_GET_CPIB(p_csc_m)))->v_p_x_cc\
    & ROHC_CC_MASK))        


#define ROHC_CPRM_IS_CH_R_P(p_csc_m)\
    (p_csc_m->field_value[ROHC_RTP_R_P_FID-ROHC_VAR_FID_BASE] != \
        ((((rohc_rtp_t *)ROHC_CPIB_GET_RTP_PTR(ROHC_CSC_GET_CPIB(p_csc_m)))->v_p_x_cc & ROHC_PAD_MASK))>>5)

#define ROHC_CPRM_IS_CH_TSS(p_csc_m)\
    ((p_csc_m)->ts_stride != (p_csc_m)->field_value[ROHC_RTP_TS_STRIDE_FID-ROHC_VAR_FID_BASE]) 

#define ROHC_CPRM_IS_CH_TIS(p_csc_m)\
    ((p_csc_m)->time_stride != (p_csc_m)->field_value[ROHC_RTP_TIME_STRIDE_FID-ROHC_VAR_FID_BASE]) 

/* IP - inner */
#define ROHC_CPRM_IS_CH_I(p_csc_m)\
    ((p_csc_m)->p_cpib->inner_ip4 != ROHC_NULL)
 
#define ROHC_CPRM_IS_CH_TOS(p_csc_m)\
    ((p_csc_m)->field_value[ROHC_INNERIP4_TOS_FID-ROHC_VAR_FID_BASE] !=\
     ((rohc_ip4_t *)(p_csc_m)->p_cpib->inner_ip4)->tos)
    
#define ROHC_CPRM_IS_CH_TC(p_csc_m)\
    ((p_csc_m)->field_value[ROHC_INNERIP6_TCLASS_FID-ROHC_VAR_FID_BASE] !=\
     ((ROHC_NTOHL(((rohc_ip6_t *)(p_csc_m)->p_cpib->inner_ip6)->ip6_ctlun.ip6_un1.un1_flow) & ROHC_IP6_TRAFFICCLASS_MASK )>>20))
    
#define ROHC_CPRM_IS_CH_TTL(p_csc_m)\
    ((p_csc_m)->field_value[ROHC_INNERIP4_TTL_FID-ROHC_VAR_FID_BASE] !=\
     ( (rohc_ip4_t *)(p_csc_m)->p_cpib->inner_ip4)->ttl)
    
#define ROHC_CPRM_IS_CH_HL(p_csc_m)\
    ((p_csc_m)->field_value[ROHC_INNERIP6_HLIM_FID-ROHC_VAR_FID_BASE] !=\
     ((rohc_ip6_t *)(p_csc_m)->p_cpib->inner_ip6)->ip6_ctlun.ip6_un1.un1_hlim)

#define ROHC_CPRM_IS_CH_PR(p_csc_m)\
    ((p_csc_m)->field_value[ROHC_INNERIP4_PROTO_FID-ROHC_VAR_FID_BASE] !=\
     ((rohc_ip4_t *)(p_csc_m)->p_cpib->inner_ip4)->proto)
     
#define ROHC_CPRM_IS_CH_NH(p_csc_m)\
    ((p_csc_m)->field_value[ROHC_INNERIP6_NHEADER_FID-ROHC_VAR_FID_BASE] !=\
     ((rohc_ip6_t *)(p_csc_m)->p_cpib->inner_ip6)->ip6_ctlun.ip6_un1.un1_nxt)
     
#define ROHC_CPRM_IS_CH_DF(p_csc_m)\
    ((p_csc_m)->field_value[ROHC_INNERIP_DF_FID-ROHC_VAR_FID_BASE] !=\
     (((ROHC_NTOHS(((rohc_ip4_t *) ROHC_CSC_GET_CPIB(p_csc_m)->inner_ip4)->off) & ROHC_IP4_DF))>>14))
#define ROHC_CPRM_IS_CH_ECN_BEH(p_csc_m)\
    ((p_csc_m)->field_value[ROHC_TCP_ECN_USED_FID-ROHC_VAR_FID_BASE] !=\
     (p_csc_m)->ecn_used)
#define ROHC_CPRM_IS_CH_INNER_IP_ECN_FLAG(p_csc_m)\
    ((p_csc_m)->field_value[ROHC_INNERIP4_ECN_FID-ROHC_VAR_FID_BASE] !=\
      ((((rohc_ip4_t*)p_csc_m->p_cpib->inner_ip4)->tos) & ROHC_PROF6_IP4_ECN_FLAG_MASK))
            
#define ROHC_CPRM_IS_CH_INNER_IPID_BEH(p_csc_m)\
    ((p_csc_m)->field_value[ROHC_INNERIP_BEH_FID-ROHC_VAR_FID_BASE] !=\
     (p_csc_m)->inner_ipid_beh)
            
#define ROHC_CPRM_IS_CH_IPX(p_csc_m)\
    (rohc_db_count_of_list(&(p_csc_m->p_cpib->inneripextlist)) != 0 )

#define ROHC_CPRM_IS_CH_RND(p_csc_m)\
    ((p_csc_m)->new_inner_rnd!=\
     (p_csc_m)->old_inner_rnd)

#define ROHC_CPRM_IS_CH_NBO(p_csc_m)\
    ((p_csc_m)->new_inner_nbo!=\
     (p_csc_m)->old_inner_nbo)
/***/
/* outer IP */
/**/                        

#define ROHC_CPRM_IS_CH_I2(p_csc_m)\
    ((p_csc_m)->p_cpib->outer_ip4 != ROHC_NULL)
 
#define ROHC_CPRM_IS_CH_TOS2(p_csc_m)\
    ((p_csc_m)->field_value[ROHC_OUTERIP4_TOS_FID-ROHC_VAR_FID_BASE] !=\
     ((rohc_ip4_t *)(p_csc_m)->p_cpib->outer_ip4)->tos)
    
#define ROHC_CPRM_IS_CH_TC2(p_csc_m)\
    ((p_csc_m)->field_value[ROHC_OUTERIP6_TCLASS_FID-ROHC_VAR_FID_BASE] !=\
     (((ROHC_NTOHL(((rohc_ip6_t *)(p_csc_m)->p_cpib->outer_ip6)->ip6_ctlun.ip6_un1.un1_flow) & ROHC_IP6_TRAFFICCLASS_MASK))>>14))
    
#define ROHC_CPRM_IS_CH_TTL2(p_csc_m)\
    ((p_csc_m)->field_value[ROHC_OUTERIP4_TTL_FID-ROHC_VAR_FID_BASE] !=\
     ((rohc_ip4_t *)((p_csc_m)->p_cpib->outer_ip4))->ttl)
    
#define ROHC_CPRM_IS_CH_HL2(p_csc_m)\
    ((p_csc_m)->field_value[ROHC_OUTERIP6_HLIM_FID-ROHC_VAR_FID_BASE] !=\
     ((rohc_ip6_t *)(p_csc_m)->p_cpib->outer_ip6)->ip6_ctlun.ip6_un1.un1_hlim)

#define ROHC_CPRM_IS_CH_PR2(p_csc_m)\
    ((p_csc_m)->field_value[ROHC_OUTERIP4_PROTO_FID-ROHC_VAR_FID_BASE] !=\
     ((rohc_ip4_t *)(p_csc_m)->p_cpib->outer_ip4)->proto)
     
#define ROHC_CPRM_IS_CH_NH2(p_csc_m)\
    ((p_csc_m)->field_value[ROHC_OUTERIP6_NHEADER_FID-ROHC_VAR_FID_BASE] !=\
     ((rohc_ip6_t *)(p_csc_m->p_cpib->outer_ip6))->ip6_ctlun.ip6_un1.un1_nxt)
     
#define ROHC_CPRM_IS_CH_DF2(p_csc_m)\
    ((p_csc_m)->field_value[ROHC_OUTERIP_DF_FID-ROHC_VAR_FID_BASE] !=\
     ((((rohc_ip4_t *)((p_csc_m)->p_cpib->outer_ip4))->off & ROHC_IP4_DF)>>14))
            
#define ROHC_CPRM_IS_CH_IPX2(p_csc_m)\
    (rohc_db_count_of_list(&(p_csc_m->p_cpib->outeripextlist)) != 0 )

#define ROHC_CPRM_IS_CH_RND2(p_csc_m)\
    ((p_csc_m)->new_outer_rnd!=\
     (p_csc_m)->old_outer_rnd)

#define ROHC_CPRM_IS_CH_NBO2(p_csc_m)\
    ((p_csc_m)->new_outer_nbo!=\
     (p_csc_m)->old_outer_nbo)

/*************/      
                      
#define ROHC_CPRM_IS_CH_CHK_SUM(p_csc_m)\
    ( ((rohc_udp_t *)((p_csc_m)->p_cpib->udp))->cksum != 0)
    
                                                                       
/* extract a 8 Bit value starting at p_buff */                          
#define ROHC_GET_U8BIT(p_buf) (*((rohc_U8bit_t *)(p_buf)))                                                                                                  
/* extract a 16 bit value starting at p_buff */                         
#define ROHC_GET_U16BIT(p_buff)\
       ((rohc_U16bit)( ((*(rohc_U8bit*)(p_buff)) << 8) | \
       *((rohc_U8bit*)(p_buff) + 1) ))        


/*#define ROHC_GET_CSC_CID(p_csc_m)\
    p_csc_m->field_value[ROHC_CID_FID-ROHC_VAR_FID_BASE]*/
    
#define ROHC_CPRM_IS_CH_T_ACK_FLAG(p_csc_m)\
    (p_csc_m->field_value[ROHC_TCP_ACK_FLAG_FID-ROHC_VAR_FID_BASE] != \
    ((((rohc_tcp_t *)p_csc_m->p_cpib->tcp)->flags & ROHC_TCP_ACK_MASK))>>4)

#define ROHC_CPRM_IS_CH_RST_FLAG(p_csc_m)\
    (p_csc_m->field_value[ROHC_TCP_RST_FLAG_FID-ROHC_VAR_FID_BASE] != \
    (((rohc_tcp_t*)p_csc->p_cpib->tcp)->flags & ROHC_TCP_RST_MASK )>>2)

#define ROHC_CPRM_IS_CH_SYN_FLAG(p_csc_m)\
    (p_csc_m->field_value[ROHC_TCP_SYN_FLAG_FID-ROHC_VAR_FID_BASE] != \
    (((rohc_tcp_t*)p_csc_m->p_cpib->tcp)->flags & ROHC_TCP_SYN_MASK )>>1)

#define ROHC_CPRM_IS_CH_FIN_FLAG(p_csc_m)\
    ((p_csc_m->field_value[ROHC_TCP_FIN_FLAG_FID-ROHC_VAR_FID_BASE]) != \
   ((((rohc_tcp_t*)p_csc_m->p_cpib->tcp)->flags) & (ROHC_TCP_FIN_MASK)))

#define ROHC_CPRM_IS_CH_URG_FLAG(p_csc_m)\
    (p_csc_m->field_value[ROHC_TCP_URG_FLAG_FID-ROHC_VAR_FID_BASE] != \
    (((rohc_tcp_t*)p_csc_m->p_cpib->tcp)->flags & ROHC_TCP_URG_MASK )>>5)

#define ROHC_CPRM_IS_CH_URG_PTR(p_csc_m)\
     (p_csc_m->field_value[ROHC_TCP_URG_PTR_FID-ROHC_VAR_FID_BASE] !=\
     ROHC_NTOHS(((rohc_tcp_t*)p_csc_m->p_cpib->tcp)->urg_ptr))

#define ROHC_CPRM_IS_CH_SN_STRIDE(p_csc_m)\
     (p_csc_m->field_value[ROHC_TCP_SN_STRIDE_FID-ROHC_VAR_FID_BASE] !=\
     p_csc_m->sn_stride)

#define ROHC_CPRM_IS_CH_ACK_STRIDE(p_csc_m)\
     (p_csc_m->field_value[ROHC_TCP_ACK_STRIDE_FID-ROHC_VAR_FID_BASE] !=\
     p_csc_m->ack_stride)

#define ROHC_CPRM_IS_CH_SN_RESIDUE(p_csc_m)\
     (p_csc_m->sn_residue != p_csc_m->temp_sn_residue)

#define ROHC_CPRM_IS_CH_ACK_RESIDUE(p_csc_m)\
     (p_csc_m->ack_residue != p_csc_m->temp_ack_residue)
#define ROHC_CPRM_IS_CH_TCP_WINDOW(p_csc_m)\
     (p_csc_m->field_value[ROHC_TCP_WINDOW_FID-ROHC_VAR_FID_BASE] !=\
     ROHC_NTOHS(((rohc_tcp_t*)p_csc_m->p_cpib->tcp)->window))

#define ROHC_CPRM_IS_CH_TCP_SN(p_csc_m)\
     (p_csc_m->field_value[ROHC_SN_FID-ROHC_VAR_FID_BASE] !=\
     p_csc_m->p_cpib->temp_sn)

#define ROHC_CPRM_IS_CH_TCP_ACK_NO(p_csc_m)\
     (p_csc_m->field_value[ROHC_TCP_ACKNO_FID-ROHC_VAR_FID_BASE] !=\
     p_csc_m->p_cpib->temp_ack)

#define ROHC_CPRM_IS_CH_TCP_OPT_LIST(p_csc_m)\
  ( ROHC_MEMCMP( p_csc_m->p_cpib->temp_tcp_opt_kind, \
    (p_csc_m->rohc_c_tcp_opt).tcp_opt_kind,\
      ROHC_MAX_ENTRIES_TCP_OPT_IDX * sizeof(rohc_U8bit_t))) 

#define ROHC_CPRM_IS_CH_DSCP(p_csc_m)\
     (p_csc_m->field_value[ROHC_INNERIP4_DSCP_FID-ROHC_VAR_FID_BASE] !=\
       ((((rohc_ip4_t*)p_csc_m->p_cpib->inner_ip4)->tos) & ROHC_PROF6_IP4_DSCP_MASK)>>2)
#define ROHC_CPRM_IS_CH_TCP_ECN_FLAG(p_csc_m)\
    ((p_csc_m)->field_value[ROHC_TCP_ECN_FLAG_FID-ROHC_VAR_FID_BASE] !=\
       (((rohc_tcp_t*)p_csc_m->p_cpib->tcp)->flags & ROHC_TCP_ECN_MASK )>>6)
#endif /*ROHC_CPRM_MCR*/
