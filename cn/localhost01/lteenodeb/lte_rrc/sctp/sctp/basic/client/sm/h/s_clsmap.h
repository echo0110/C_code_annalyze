/******************************************************************************
 *  FILE NAME:  s_clsmap.h
 *
 *  DESCRIPTION:
 *  This file is part of message based SM client build of SCTP stack. Here
 *  we do the static parsing of API structure called by the client code.
 *
 *  DATE      NAME         REFERENCE       REASON
 *  -----------------------------------------------------
 *
 *  23March 2001     Vakul Garg      -         .Original Creation
 *  13Sep   2001     Yogesh Gahlaut            Rel 3.0 Changes
 *  19Sep03          Manish Rajpal             Rel 5.0 Changes
 *  27Now03          Vivek Bansal              CSPL Changes
 *  14july10         Sharad Kumar Yadav        Rel 6.2.0 SPR# 20526  For CSR# 1-7670333
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#ifndef __FILE_sctp_sm_msg_api_SEEN__
#define __FILE_sctp_sm_msg_api_SEEN__

#ifdef __cplusplus
extern "C" {
#endif

#include <s_clco.h>


    /*********** SM request API Id's  *************************************/

#define SCTP_MIN_SMREQ_APIID    200

#define SCTP_API_INIT_STACK_REQ             (SCTP_MIN_SMREQ_APIID + 0)
#define SCTP_API_GET_ENTITY_VERSION_REQ     (SCTP_MIN_SMREQ_APIID + 1)
#define SCTP_API_CONFIG_STACK_PARAMS_REQ    (SCTP_MIN_SMREQ_APIID + 2)
#define SCTP_API_INIT_STATS_REQ             (SCTP_MIN_SMREQ_APIID + 3)
#define SCTP_API_ENABLE_STATS_REQ           (SCTP_MIN_SMREQ_APIID + 4)
#define SCTP_API_DISABLE_STATS_REQ          (SCTP_MIN_SMREQ_APIID + 5)
#define SCTP_API_GET_STATS_REQ              (SCTP_MIN_SMREQ_APIID + 6)
#define SCTP_API_GET_ERROR_LEVEL_REQ        (SCTP_MIN_SMREQ_APIID + 7)
#define SCTP_API_SET_ERROR_LEVEL_REQ        (SCTP_MIN_SMREQ_APIID + 8)
#define SCTP_API_ENABLE_ASSOC_TRC_REQ       (SCTP_MIN_SMREQ_APIID + 9)
#define SCTP_API_DISABLE_ASSOC_TRC_REQ      (SCTP_MIN_SMREQ_APIID + 10)
#define SCTP_API_GET_TRACE_INFO_REQ         (SCTP_MIN_SMREQ_APIID + 11)
#define SCTP_API_CONFIG_STACK_ADDRS_REQ     (SCTP_MIN_SMREQ_APIID + 12)
#define SCTP_API_DEINIT_STACK_REQ           (SCTP_MIN_SMREQ_APIID + 13)
    /* Start SPR 20526 */
#define SCTP_API_GET_RTT_INFO_REQ           (SCTP_MIN_SMREQ_APIID +14)
    /* End SPR 20526 */

#ifdef SCTP_RED_SUPPORT

#define SCTP_MIN_REDREQ_APIID               (SCTP_MIN_SMREQ_APIID + 15)

#define SCTP_API_SET_STACK_RED_STATE_REQ    (SCTP_MIN_REDREQ_APIID + 0)
#define SCTP_API_SET_STACK_PROC_STATE_REQ   (SCTP_MIN_REDREQ_APIID + 1)
#define SCTP_API_GET_STACK_RED_DB_REQ       (SCTP_MIN_REDREQ_APIID + 2)
#define SCTP_API_SET_STACK_RED_DB_REQ       (SCTP_MIN_REDREQ_APIID + 3)
#define SCTP_API_GET_ASSOC_RED_DB_REQ       (SCTP_MIN_REDREQ_APIID + 4)
#define SCTP_API_SET_ASSOC_RED_DB_REQ       (SCTP_MIN_REDREQ_APIID + 5)
#define SCTP_API_GET_STACK_HEALTH_REQ       (SCTP_MIN_REDREQ_APIID + 6)

#define SCTP_MAX_SMREQ_APIID                SCTP_API_GET_STACK_HEALTH_REQ

#else
    /* Start SPR 20526 */
#define SCTP_MAX_SMREQ_APIID               SCTP_API_GET_RTT_INFO_REQ
    /* End SPR 20526 */

#endif /*SCTP_RED_SUPPORT*/


#define SCTP_NUM_SMREQ_API  (SCTP_MAX_SMREQ_APIID - SCTP_MIN_SMREQ_APIID + 1)


    /*********** SM Response API Id's  *************************************/
#define SCTP_MIN_SMRESP_APIID   400

#define SCTP_API_INIT_STACK_RESP            (SCTP_MIN_SMRESP_APIID + 0)
#define SCTP_API_GET_ENTITY_VERSION_RESP    (SCTP_MIN_SMRESP_APIID + 1)
#define SCTP_API_CONFIG_STACK_PARAMS_RESP   (SCTP_MIN_SMRESP_APIID + 2)
#define SCTP_API_INIT_STATS_RESP            (SCTP_MIN_SMRESP_APIID + 3)
#define SCTP_API_ENABLE_STATS_RESP          (SCTP_MIN_SMRESP_APIID + 4)
#define SCTP_API_DISABLE_STATS_RESP         (SCTP_MIN_SMRESP_APIID + 5)
#define SCTP_API_GET_STATS_RESP             (SCTP_MIN_SMRESP_APIID + 6)
#define SCTP_API_GET_ERROR_LEVEL_RESP       (SCTP_MIN_SMRESP_APIID + 7)
#define SCTP_API_SET_ERROR_LEVEL_RESP       (SCTP_MIN_SMRESP_APIID + 8)
#define SCTP_API_ENABLE_ASSOC_TRC_RESP      (SCTP_MIN_SMRESP_APIID + 9)
#define SCTP_API_DISABLE_ASSOC_TRC_RESP     (SCTP_MIN_SMRESP_APIID + 10)
#define SCTP_API_GET_TRACE_INFO_RESP        (SCTP_MIN_SMRESP_APIID + 11)
#define SCTP_API_CONFIG_STACK_ADDRS_RESP    (SCTP_MIN_SMRESP_APIID + 12)
#define SCTP_API_DEINIT_STACK_RESP          (SCTP_MIN_SMRESP_APIID + 13)
    /* Start SPR 20526 */
#define SCTP_API_GET_RTT_INFO_RESP          (SCTP_MIN_SMRESP_APIID + 14)
    /* End SPR 20526 */

#ifdef SCTP_RED_SUPPORT

#define SCTP_MIN_REDRESP_API                (SCTP_MIN_SMRESP_APIID + 15)

#define SCTP_API_SET_STACK_RED_STATE_RESP   (SCTP_MIN_REDRESP_API + 0)
#define SCTP_API_SET_STACK_PROC_STATE_RESP  (SCTP_MIN_REDRESP_API + 1)
#define SCTP_API_GET_STACK_RED_DB_RESP      (SCTP_MIN_REDRESP_API + 2)
#define SCTP_API_SET_STACK_RED_DB_RESP      (SCTP_MIN_REDRESP_API + 3)
#define SCTP_API_GET_ASSOC_RED_DB_RESP      (SCTP_MIN_REDRESP_API + 4)
#define SCTP_API_SET_ASSOC_RED_DB_RESP      (SCTP_MIN_REDRESP_API + 5)
#define SCTP_API_GET_STACK_HEALTH_RESP      (SCTP_MIN_REDRESP_API + 6)

#define SCTP_MAX_SMRESP_APIID               SCTP_API_GET_STACK_HEALTH_RESP

#else
    /* Start SPR 20526 */
#define SCTP_MAX_SMRESP_APIID               SCTP_API_GET_RTT_INFO_RESP
    /* End SPR 20526 */

#endif /*SCTP_RED_SUPPORT*/



#define SCTP_NUM_SMRESP_API (SCTP_MAX_SMRESP_APIID - SCTP_MIN_SMRESP_APIID + 1)

    /*** other parameters **********************************************/

#define SCTP_MAX_HOSTNAME_LEN       50     /* Maximum hostname length */
#define SCP_MAX_PERCENTAGE_VALUE    100    /* Max percentage */
#define SCTP_IPADDRTYPE_IPv4        0  /* address type for ip version 4*/
#define SCTP_HB_ENABLE              0
#define SCTP_HB_DISABLE             1
#define SCTP_HB_REQUEST             2

    /*
     *Change the following parameter if reqd. It denotes the max data size
     *in bytes which will be received in message based interface.
     */
#define MAX_USER_DATA_SIZE          1024

    /************************Init Stack*********************/

    typedef struct
    {
        sctp_U32bit             max_total_assoc;
        sctp_U32bit             max_endpoint;
        sctp_U32bit             max_streams_per_assoc;
        sctp_U32bit             num_tx_min_buffers;
        sctp_U32bit             num_tx_med_buffers;
        sctp_U32bit             num_tx_max_buffers;
        sctp_U32bit             num_rx_min_buffers;
        sctp_U32bit             num_rx_med_buffers;
        sctp_U32bit             num_rx_max_buffers;
        sctp_U32bit             no_of_local_addrs;
        sctp_U32bit             seed;
        sctp_U32bit             min_reserve_buffers;
        sctp_U32bit             med_reserve_buffers;
        sctp_U32bit             max_reserve_buffers;
        sctp_checksum_et        checksum;
        sctp_stack_type_et      stack_type;
        sctp_Boolean_t          pmtu_capable;
        sctp_Boolean_t          ecn_capable;
        sctp_Boolean_t          nat_capable;
        sctp_sockaddr_list_st   list_local_addrs;
        sctp_U16bit             standard_udp_port;
        sctp_U16bit             max_total_appl;
        sctp_U8bit              stale_cookie_policy;
        sctp_U8bit              ack_policy;
        sctp_U8bit              hostname[50];
    } sctp_msg_init_stack_st;


    typedef struct
    {
        sctp_return_t       result;
        sctp_error_t        errorno;
    } sctp_msg_init_stack_res_st;


    /*********************Get Version*************************/



    typedef struct
    {
        sctp_return_t       result;
        sctp_U32bit         major_ver;
        sctp_U32bit         minor_ver;
    } sctp_msg_get_entity_version_res_st;

    /********************Config Stack Params*******************/

    typedef struct
    {
        sctp_U32bit     rto_init;
        sctp_U32bit     rto_min;
        sctp_U32bit     rto_max;
        sctp_U32bit     valid_cookie_life;
        sctp_U32bit     assoc_max_retrans;
        sctp_U32bit     path_max_retrans;
        sctp_U32bit     max_init_retrans;
        sctp_U32bit     max_init_path_retrans;
        sctp_U32bit     init_timeo;
        sctp_U32bit     heartbeat_interval;
        sctp_U16bit     rto_alpha_percent;
        sctp_U16bit     rto_beta_percent;
        sctp_U16bit     num_in_streams;
        sctp_U16bit     num_out_streams;
        sctp_U16bit     hb_info_length;
        sctp_U32bit     bundling_time;
        sctp_U32bit     sack_period; /*** SACK_PERIOD ********/
        sctp_U32bit     sack_frequency; /*** SACK_FREQUENCY ********/
        sctp_U16bit     mtu_size;
        sctp_Boolean_t  pmtu_capable;
        sctp_U32bit     hb_max_burst;

#ifdef SCTP_IP_HEADER_PARAMS
        sctp_ip_header_params_st   ip_header_params;
#endif

    } sctp_msg_config_stack_params_st;


    /***********************Config Stack Params***************************/

    typedef struct
    {
        sctp_return_t       result;
        sctp_error_t        errorno;
    } sctp_msg_config_stack_params_res_st;


    /***********************Config stack addresses*************************/

    typedef    struct
    {
        sctp_bindx_flag_et     flags;
        sctp_U32bit            no_of_addrs;
        sctp_sockaddr_list_st  list_addrs;
    } sctp_msg_config_stack_addrs_st;


    typedef    struct
    {
        sctp_return_t  result;
        sctp_error_t   errorno;
    } sctp_msg_config_stack_addrs_res_st;


    /**************************Deinit stack********************************/

    typedef    struct
    {
        sctp_Boolean_t      force;
    } sctp_msg_deinit_stack_st;


    typedef    struct
    {
        sctp_return_t  result;
        sctp_error_t   errorno;
    } sctp_msg_deinit_stack_res_st;


    /***********************Init Stats***************************/


    typedef struct
    {
        sctp_stats_et       stats_type;
    } sctp_msg_init_stats_st;


    typedef struct
    {
        sctp_return_t       result;
        sctp_error_t        errorno;
    } sctp_msg_init_stats_res_st;

    /**********************Enable Stats*************************/


    typedef struct
    {
        sctp_stats_et       stats_type;
    } sctp_msg_enable_stats_st;


    typedef struct
    {
        sctp_return_t       result;
        sctp_error_t        errorno;
    } sctp_msg_enable_stats_res_st;

    /**********************Disable Stats***********************/

    typedef struct
    {
        sctp_stats_et       stats_type;
    } sctp_msg_disable_stats_st;


    typedef struct
    {
        sctp_return_t       result;
        sctp_error_t        errorno;
    } sctp_msg_disable_stats_res_st;


    /**********************Get Stats**************************/
    typedef struct
    {
        sctp_U32bit     association_id;
        sctp_stats_et   stats_type;
        sctp_Boolean_t  reset_flag;
    } sctp_msg_get_stats_st;


    typedef struct
    {
        sctp_return_t       result;
        sctp_error_t        errorno;
        sctp_U32bit         association_id;
        sctp_stats_et       stats_type;

        union
        {
            sctp_proto_internal_stats_st      prot_stats;
            sctp_global_traf_stats_st         traf_stats;
            sctp_error_stats_st               error_stats;
            sctp_assoc_stats_st               assoc_stats;
        } stats_data;
    } sctp_msg_get_stats_res_st;

    /***********************Get Error Level******************/

    typedef struct
    {
        sctp_error_flag_et       error_flag;
    } sctp_msg_get_error_level_st;


    typedef struct
    {
        sctp_return_t        result;
        sctp_error_t         errorno;
        sctp_error_level_et  error_level;
    } sctp_msg_get_error_level_res_st;


    /*********************Set Error level********************/
    typedef struct
    {
        sctp_error_flag_et       error_flag;
        sctp_error_level_et      error_level;
    } sctp_msg_set_error_level_st;


    typedef struct
    {
        sctp_return_t       result;
        sctp_error_t        errorno;
    } sctp_msg_set_error_level_res_st;

    /*********************Enable Assoc trace*****************/

    typedef struct
    {
        sctp_U32bit         assoc_id;
        sctp_trc_et         trace_flag;
    } sctp_msg_enable_assoc_trc_st;


    typedef struct
    {
        sctp_return_t       result;
        sctp_error_t        errorno;
        sctp_U32bit         assoc_id;
    } sctp_msg_enable_assoc_trc_res_st;

    /*******************Disable traces**********************/

    typedef struct
    {
        sctp_U32bit     assoc_id;
        sctp_trc_et     trace_flag;
    } sctp_msg_disable_assoc_trc_st;


    typedef struct
    {
        sctp_return_t       result;
        sctp_error_t        errorno;
        sctp_U32bit         assoc_id;
    } sctp_msg_disable_assoc_trc_res_st;

    /********************Get Trace Info**********************/

    typedef struct
    {
        sctp_U32bit     assoc_id;
    } sctp_msg_get_trace_info_st;


    typedef struct
    {
        sctp_return_t           result;
        sctp_error_t            errorno;
        sctp_U8bit              trace_info;
        sctp_U32bit             assoc_id;
    } sctp_msg_get_trace_info_res_st;


    /*****************Set Stack Red State********************/

    typedef struct
    {
        sctp_stack_red_state_et      stack_state;
    } sctp_msg_set_stack_red_state_st;


    typedef struct
    {
        sctp_return_t           result;
        sctp_error_t            errorno;
    } sctp_msg_set_stack_red_state_res_st;


    /*****************Set Stack Proc State********************/

    typedef struct
    {
        sctp_stack_proc_state_et      stack_state;
        sctp_U32bit                   stack_seed;
    } sctp_msg_set_stack_proc_state_st;


    typedef struct
    {
        sctp_return_t           result;
        sctp_error_t            errorno;
    } sctp_msg_set_stack_proc_state_res_st;


    /*****************Get Stack Red DB********************/



    typedef struct
    {
        sctp_return_t              result;
        sctp_error_t               errorno;
        sctp_red_stack_db_st       stack_red_db;
    } sctp_msg_get_stack_red_db_res_st;


    /*****************Set Stack Red DB********************/

    typedef struct
    {
        sctp_red_stack_db_st      stack_red_db;
    } sctp_msg_set_stack_red_db_st;


    typedef struct
    {
        sctp_return_t           result;
        sctp_error_t            errorno;
    } sctp_msg_set_stack_red_db_res_st;


    /*****************Get Assoc Red DB********************/

    typedef struct
    {
        sctp_U32bit             association_id;
    } sctp_msg_get_assoc_red_db_st;


    typedef struct
    {
        sctp_return_t           result;
        sctp_error_t            errorno;
        sctp_U32bit             assoc_id;
        sctp_U32bit             len;
        sctp_U8bit              *assoc_red_db;
    } sctp_msg_get_assoc_red_db_res_st;


    /*****************Set Assoc Red DB********************/

    typedef struct
    {
        sctp_U32bit             assoc_id;
        sctp_U32bit             len;
        sctp_U8bit              *assoc_red_db;
    } sctp_msg_set_assoc_red_db_st;


    typedef struct
    {
        sctp_return_t           result;
        sctp_error_t            errorno;
    } sctp_msg_set_assoc_red_db_res_st;


    /*****************Get stack Health********************/



    typedef struct
    {
        sctp_return_t              result;
        sctp_error_t               errorno;
        sctp_stack_red_state_et    red_state;
        sctp_stack_proc_state_et   proc_state;
    } sctp_msg_get_stack_health_res_st;

    /* Start SPR 20526 */
    /*****************Get Rtt info parameters******************************/

    typedef struct
    {
        sctp_U32bit       association_id;
        sctp_Boolean_t    reset_flag;
    } sctp_msg_get_rtt_info_st;


    typedef struct sctp_msg_get_rtt_info_res_st
    {
        sctp_U32bit             association_id;
        sctp_return_t           result;
        sctp_error_t            errorno;
        sctp_U32bit             nos_of_dest_trans_addrs;
        sctp_sockaddr_list_st   list_of_dest_trans_addrs;
        sctp_rtt_info_list_st   dest_rtt_param_list;
        sctp_sockaddr_st        primary_dest_transport_addr;
    } sctp_msg_get_rtt_info_res_st;

    /**********************************************************************/
    /* End SPR 20526 */




    extern sctp_return_t
    send_to_sctp_sm(
        sctp_trans_id_t     transaction_id,
        sctp_api_id_t       api_id,
        sctp_src_id_t       src_id,
        sctp_dest_id_t      dest_id,
        sctp_Pvoid_t        p_api_elements,
        sctp_Error_t        *p_ecode );

    extern sctp_void_t
    sctp_sm_process_stack_buffer(
        sctp_U8bit    *p_buffer,
        sctp_len_t    nooctets );


    extern void
    sctp_process_api_sm_resp(
        sctp_trans_id_t,
        sctp_api_id_t,
        sctp_void_t * );


#ifdef __cpluscplus
}
#endif

#endif /* __FILE_sctp_parse_api_SEEN__  */
