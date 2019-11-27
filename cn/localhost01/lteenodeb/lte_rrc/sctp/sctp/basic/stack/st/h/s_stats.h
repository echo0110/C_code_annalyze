/*****************************************************************************
 *    FILE NAME:
 *      s_stats.h
 *
 *    DESCRIPTION:
 *    This file is part of SCTP statistics module. This will list
 *    Macros that will be used for incrementing/decrementing statistics
 *    counters.
 *
 *    DATE        NAME       REFERENCE       REASON
 *    -----------------------------------------------------
 *    02Jun00   samahajan       -            .Original Creation
 *    09Sep01   gsheoran        -            Rel 3.0
 *    07JULY08  Shikha Kathpal               Rel6.2.0 RFC Upgrade
 *    15july10  Sharad Kumar Yadav           Rel 6.2.0  SPR 20526 For CSR# 1-7670333
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ****************************************************************************/
#ifndef    __FILE_sctp_stats_SEEN__
#define    __FILE_sctp_stats_SEEN__

#include <s_cotyps.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef SCTP_STATS_ENABLED

    extern sctp_proto_internal_stats_st sctp_protocol_stats;

    /* Contains the Traffic related statistics of SCTP stack entity.*/
    extern sctp_global_traf_stats_st sctp_global_traffic_stats;

    /* Contains the Errors related statistics for SCTP stack entity.*/
    extern sctp_error_stats_st sctp_error_stats;

    /* Contains association specific statistics for SCTP stack entity.*/
    extern sctp_assoc_stats_st *sctp_assoc_stats;

    extern sctp_stats_state_et sctp_stats_state;

    extern sctp_Boolean_t  sctp_stats_table[SCTP_ALL_STATS];

    sctp_return_t
    sctp_init_stats_module(
        sctp_U32bit      max_num_assoc,
        sctp_error_t     *p_ecode );

    void
    sctp_incr_error_stats(
        sctp_U8bit       parameter_index,
        sctp_U32bit      count );

    void
    sctp_decr_error_stats(
        sctp_U8bit       parameter_index,
        sctp_U32bit      count );

    void
    sctp_incr_prot_int_stats(
        sctp_U8bit       parameter_index,
        sctp_U32bit      count );

    void
    sctp_decr_prot_int_stats(
        sctp_U8bit       parameter_index,
        sctp_U32bit      count );

    void
    sctp_incr_global_traf_stats(
        sctp_U8bit       parameter_index,
        sctp_U32bit      count );

    void
    sctp_decr_global_traf_stats(
        sctp_U8bit       parameter_index,
        sctp_U32bit      count );

    void
    sctp_incr_assoc_stats(
        sctp_tcb_st      *p_tcb,
        sctp_U32bit      dest_addr_idx,
        sctp_U8bit       parameter_index,
        sctp_U32bit      count );

    void
    sctp_decr_assoc_stats(
        sctp_tcb_st      *p_tcb,
        sctp_U32bit      dest_addr_idx,
        sctp_U8bit       parameter_index,
        sctp_U32bit      count );

    void
    sctp_init_assoc_stats(
        sctp_tcb_st      *p_tcb,
        sctp_Boolean_t   flag );

    void
    sctp_reinit_assoc_stats(
        sctp_tcb_st      *p_tcb,
        sctp_Boolean_t   flag );


    /* Error statistics related Macros */

#define SCTP_INCR_ERRORSTAT(parameter,count)\
    sctp_incr_error_stats(parameter, count)

#define SCTP_INCR_PROT_INTSTATS(parameter,count)\
    sctp_incr_prot_int_stats(parameter, count)

#define SCTP_DECR_PROT_INTSTATS(parameter,count)\
    sctp_decr_prot_int_stats(parameter, count)

#define SCTP_INCR_GLOBAL_TRAFFICSTAT(parameter,count)\
    sctp_incr_global_traf_stats(parameter, count)

#define SCTP_DECR_GLOBAL_TRAFFICSTAT(parameter,count)\
    sctp_decr_global_traf_stats(parameter, count)

#define SCTP_INCR_ASSOCSTAT(p_ass, dest_addr_idx, parameter,count)\
    sctp_incr_assoc_stats(p_ass, dest_addr_idx, parameter, count)

#define SCTP_DECR_ASSOCSTAT(p_ass, dest_addr_idx, parameter,count)\
    sctp_decr_assoc_stats(p_ass, dest_addr_idx, parameter, count)

#define SCTP_INIT_ASSOCSTAT(p_ass)\
    sctp_init_assoc_stats(p_ass, SCTP_FALSE)

#define SCTP_REINIT_ASSOCSTAT(p_ass)\
    sctp_reinit_assoc_stats(p_ass, SCTP_FALSE)
    /* Rel 6.2.0   change start */
#define SCTP_INCR_ASSOC_STREAM_STAT(p_ass, stream_id, parameter, count)\
    sctp_incr_assoc_stream_stats(p_ass, stream_id, parameter, count)
#else /* SCTP_STATS_ENABLED */
    /*  Rel 6.2.0   change end */
#define SCTP_DECR_ERRORSTAT(parameter, count)
#define SCTP_INCR_ERRORSTAT(parameter, count)

#define SCTP_INCR_PROT_INTSTATS(parameter, count)
#define SCTP_DECR_PROT_INTSTATS(parameter, count)

#define SCTP_INCR_GLOBAL_TRAFFICSTAT(parameter, count)
#define SCTP_DECR_GLOBAL_TRAFFICSTAT(parameter, count)

#define SCTP_INCR_ASSOCSTAT(p_ass, dest_addr_idx, parameter, count)
#define SCTP_DECR_ASSOCSTAT(p_ass, dest_addr_idx, parameter, count)
#define SCTP_INIT_ASSOCSTAT(p_ass)
#define SCTP_REINIT_ASSOCSTAT(p_ass)
    /* Rel 6.2.0  change start */
#define SCTP_INCR_ASSOC_STREAM_STAT(p_ass, stream_id, parameter,count)
#endif /* SCTP_STATS_ENABLED */
    void sctp_incr_assoc_stream_stats(
        sctp_tcb_st           *p_ass,
        sctp_U16bit           stream_id,
        sctp_U8bit            parameter_index,
        sctp_U32bit           count );
    /*  Rel 6.2.0   change end */

    sctp_return_t
    sctp_api_init_stats(
        sctp_stats_et    stats_type,
        sctp_error_t     *p_ecode );

    sctp_return_t
    sctp_api_get_stats(
        sctp_stats_et    stats_type,
        sctp_assoc_t     association_id,
        sctp_U8bit       *stats_data,
        sctp_Boolean_t       reset_flag,
        sctp_error_t     *p_ecode );

    sctp_return_t
    sctp_api_disable_stats(
        sctp_stats_et    stats_type,
        sctp_error_t     *p_ecode );

    sctp_return_t
    sctp_api_enable_stats(
        sctp_stats_et    stats_type,
        sctp_error_t     *p_ecode );

    /* Start SPR 20526 */
    sctp_return_t
    sctp_api_get_rtt_info(
        sctp_U32bit                    assoc_id,
        sctp_U32bit                    *p_num_daddrs,
        sctp_sockaddr_list_st          list_of_dest_addrs,
        sctp_rtt_info_list_st          *p_dest_rtt_param_list,
        sctp_sockaddr_st               *p_dpriaddr,
        sctp_Boolean_t                 reset_flag,
        sctp_error_t                   *p_ecode );
    /* End SPR 20526 */

#ifdef __cplusplus
}
#endif


#endif /* __FILE_sctp_stats_SEEN__ */

