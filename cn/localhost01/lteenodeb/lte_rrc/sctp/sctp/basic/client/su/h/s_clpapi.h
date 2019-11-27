/******************************************************************************
 *    FILE NAME: s_clpapi.c
 *
 *    DESCRIPTION:
 *      This file is part of client build of SCTP stack. Here we do
 *      the static parsing of API structure called by the client code.
 *
 *
 *
 *    DATE          NAME           REFERENCE       REASON
 *    -----------------------------------------------------
 *
 *    02June 2000     Sandeep Mahajan  -           .Original Creation
 *    27Now03         Vivek Bansal                 CSPL Changes
 *    11Jan04         Vivek Bansal                 Changes for giving indication
 *                                                 to application in case of IPC
 *                                                 failure.
 *    4Feb05          Amit Ray                     Rel 5.0.4
 *
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#ifndef    __FILE_sctp_parse_api_SEEN__
#define    __FILE_sctp_parse_api_SEEN__

#ifdef  __cplusplus
extern "C" {
#endif

#include <s_clco.h>

#define    SCTP_SU_CLI_MAX_DST          10
    /*********** Service User request API Id's  **************************/
#define    SCTP_MIN_SUREQUST_APIID      10    /* minimum request api id */

#define    SCTP_API_REGISTER_SU_REQ            (SCTP_MIN_SUREQUST_APIID + 0)
#define    SCTP_API_DEREGISTER_SU_REQ          (SCTP_MIN_SUREQUST_APIID + 1)
#define    SCTP_API_SOCKET_REQ                 (SCTP_MIN_SUREQUST_APIID + 2)
#define    SCTP_API_BIND_REQ                   (SCTP_MIN_SUREQUST_APIID + 3)
#define    SCTP_API_LISTEN_REQ                 (SCTP_MIN_SUREQUST_APIID + 4)
#define    SCTP_API_CONFIG_ASSOC_REQ           (SCTP_MIN_SUREQUST_APIID + 5)
#define    SCTP_API_SET_FTHRESH_REQ            (SCTP_MIN_SUREQUST_APIID + 6)
#define    SCTP_API_CONNECT_REQ                (SCTP_MIN_SUREQUST_APIID + 7)
#define    SCTP_API_CONNECT_EXT_REQ            (SCTP_MIN_SUREQUST_APIID + 8)
#define    SCTP_API_ACCEPT_REQ                 (SCTP_MIN_SUREQUST_APIID + 9)
#define    SCTP_API_ACCEPT_EXT_REQ             (SCTP_MIN_SUREQUST_APIID + 10)
#define    SCTP_API_SEND_REQ                   (SCTP_MIN_SUREQUST_APIID + 11)
#define    SCTP_API_SEND_EXT_REQ               (SCTP_MIN_SUREQUST_APIID + 12)
#define    SCTP_API_RECEIVE_REQ                (SCTP_MIN_SUREQUST_APIID + 13)
#define    SCTP_API_RECEIVE_EXT_REQ            (SCTP_MIN_SUREQUST_APIID + 14)
#define    SCTP_API_RECEIVE_PART_REQ           (SCTP_MIN_SUREQUST_APIID + 15)
#define    SCTP_API_CLOSE_REQ                  (SCTP_MIN_SUREQUST_APIID + 16)
#define    SCTP_API_HB_ACTION_REQ              (SCTP_MIN_SUREQUST_APIID + 17)
#define    SCTP_API_SET_PRIMARY_REQ            (SCTP_MIN_SUREQUST_APIID + 18)
#define    SCTP_API_GET_SRTT_REQ               (SCTP_MIN_SUREQUST_APIID + 19)
#define    SCTP_API_GET_STATUS_REQ             (SCTP_MIN_SUREQUST_APIID + 20)
#define    SCTP_API_GET_DESTSTATUS_REQ         (SCTP_MIN_SUREQUST_APIID + 21)
#define    SCTP_API_SET_RESTART_OPTION_REQ     (SCTP_MIN_SUREQUST_APIID + 22)
#define    SCTP_API_RETRIEVE_UNSENT_REQ        (SCTP_MIN_SUREQUST_APIID + 23)
#define    SCTP_API_RETRIEVE_UNACKED_REQ       (SCTP_MIN_SUREQUST_APIID + 24)
#define    SCTP_API_FLUSH_BUFFERS_REQ          (SCTP_MIN_SUREQUST_APIID + 25)
#define    SCTP_API_CONFIG_ASSOC_CTL_REQ       (SCTP_MIN_SUREQUST_APIID + 26)
#define    SCTP_API_GET_STREAM_STATUS_REQ      (SCTP_MIN_SUREQUST_APIID + 27)
#define    SCTP_API_ABORT_RETRIEVE_REQ         (SCTP_MIN_SUREQUST_APIID + 28)
#define    SCTP_API_SEND_IOVEC_REQ             (SCTP_MIN_SUREQUST_APIID + 29)
#define    SCTP_API_BINDX_REQ                  (SCTP_MIN_SUREQUST_APIID + 30)
#define    SCTP_API_CONFIG_CONG_REG_DEF_REQ    (SCTP_MIN_SUREQUST_APIID + 31)
#define    SCTP_API_GET_CONG_REG_DEF_REQ       (SCTP_MIN_SUREQUST_APIID + 32)
#define    SCTP_API_CONFIG_TXRX_NUM_FRAG_REQ   (SCTP_MIN_SUREQUST_APIID + 33)
#define    SCTP_API_GET_FREE_TR_NUM_FRAG_REQ  (SCTP_MIN_SUREQUST_APIID + 34)
#define    SCTP_API_CONNECTX_REQ                (SCTP_MIN_SUREQUST_APIID + 35)
#define    SCTP_API_CLOSE_EXT_REQ               (SCTP_MIN_SUREQUST_APIID + 36)

#define    SCTP_MAX_SUREQUST_APIID              (SCTP_API_CLOSE_EXT_REQ)

#define    SCTP_NUM_SUREQUEST_API    (SCTP_MAX_SUREQUST_APIID - SCTP_MIN_SUREQUST_APIID + 1)
    /* maximum request api id */


    /*********** Service User Response API Id's  ***************************/
#define    SCTP_MIN_SURESP_APIID           50     /* minimum response api id */
#define    SCTP_API_REGISTER_SU_RESP           (SCTP_MIN_SURESP_APIID + 0)
#define    SCTP_API_DEREGISTER_SU_RESP         (SCTP_MIN_SURESP_APIID + 1)
#define    SCTP_API_SOCKET_RESP                (SCTP_MIN_SURESP_APIID + 2)
#define    SCTP_API_BIND_RESP                  (SCTP_MIN_SURESP_APIID + 3)
#define    SCTP_API_LISTEN_RESP                (SCTP_MIN_SURESP_APIID + 4)
#define    SCTP_API_CONFIG_ASSOC_RESP          (SCTP_MIN_SURESP_APIID + 5)
#define    SCTP_API_SET_FTHRESH_RESP           (SCTP_MIN_SURESP_APIID + 6)
#define    SCTP_API_CONNECT_RESP               (SCTP_MIN_SURESP_APIID + 7)
#define    SCTP_API_CONNECT_EXT_RESP           (SCTP_MIN_SURESP_APIID + 8)
#define    SCTP_API_ACCEPT_RESP                (SCTP_MIN_SURESP_APIID + 9)
#define    SCTP_API_ACCEPT_EXT_RESP            (SCTP_MIN_SURESP_APIID + 10)
#define    SCTP_API_SEND_RESP                  (SCTP_MIN_SURESP_APIID + 11)
#define    SCTP_API_SEND_EXT_RESP              (SCTP_MIN_SURESP_APIID + 12)
#define    SCTP_API_RECEIVE_RESP               (SCTP_MIN_SURESP_APIID + 13)
#define    SCTP_API_RECEIVE_EXT_RESP           (SCTP_MIN_SURESP_APIID + 14)
#define    SCTP_API_RECEIVE_PART_RESP          (SCTP_MIN_SURESP_APIID + 15)
#define    SCTP_API_CLOSE_RESP                 (SCTP_MIN_SURESP_APIID + 16)
#define    SCTP_API_HB_ACTION_RESP             (SCTP_MIN_SURESP_APIID + 17)
#define    SCTP_API_SET_PRIMARY_RESP           (SCTP_MIN_SURESP_APIID + 18)
#define    SCTP_API_GET_SRTT_RESP              (SCTP_MIN_SURESP_APIID + 19)
#define    SCTP_API_GET_STATUS_RESP            (SCTP_MIN_SURESP_APIID + 20)
#define    SCTP_API_GET_DESTSTATUS_RESP        (SCTP_MIN_SURESP_APIID + 21)
#define    SCTP_API_SET_RESTART_OPTION_RESP    (SCTP_MIN_SURESP_APIID + 22)
#define    SCTP_API_RETRIEVE_UNSENT_RESP       (SCTP_MIN_SURESP_APIID + 23)
#define    SCTP_API_RETRIEVE_UNACKED_RESP      (SCTP_MIN_SURESP_APIID + 24)
#define    SCTP_API_FLUSH_BUFFERS_RESP         (SCTP_MIN_SURESP_APIID + 25)
#define    SCTP_API_CONFIG_ASSOC_CTL_RESP      (SCTP_MIN_SURESP_APIID + 26)
#define    SCTP_API_GET_STREAM_STATUS_RESP     (SCTP_MIN_SURESP_APIID + 27)
#define    SCTP_API_ABORT_RETRIEVE_RESP        (SCTP_MIN_SURESP_APIID + 28)
#define    SCTP_API_BINDX_RESP                 (SCTP_MIN_SURESP_APIID + 29)
#define    SCTP_API_CONFIG_CONG_REG_DEF_RESP   (SCTP_MIN_SURESP_APIID + 30)
#define    SCTP_API_GET_CONG_REG_DEF_RESP      (SCTP_MIN_SURESP_APIID + 31)
#define    SCTP_API_CONFIG_TXRX_NUM_FRAG_RESP  (SCTP_MIN_SURESP_APIID + 32)
#define    SCTP_API_GET_FREE_TR_NUM_FRAG_RESP  (SCTP_MIN_SURESP_APIID + 33)
#define    SCTP_API_CONNECTX_RESP               (SCTP_MIN_SURESP_APIID + 34)

#define    SCTP_MAX_SURESP_APIID                 SCTP_API_CONNECTX_RESP

#define    SCTP_NUM_SURESP_API (SCTP_MAX_SURESP_APIID - SCTP_MIN_SURESP_APIID + 1)
    /* maximum request api id */

    /************ Notifications ****************************/
#define SCTP_MIN_SU_NOTIF_APIID        100
#define SCTP_API_OP_ERROR_NOTF                 (SCTP_MIN_SU_NOTIF_APIID + 0)
#define SCTP_API_PEER_DATA_RECVD_IN_ERROR_NOTF (SCTP_MIN_SU_NOTIF_APIID + 1)
#define SCTP_API_DESTINATION_STATUS_CHG_NOTF   (SCTP_MIN_SU_NOTIF_APIID + 2)
#define SCTP_API_ENDPOINT_STATUS_CHG_NOTF      (SCTP_MIN_SU_NOTIF_APIID + 3)
#define SCTP_API_CONGESTION_NOTF               (SCTP_MIN_SU_NOTIF_APIID + 4)
#define SCTP_API_READY_TO_RECEIVE_NOTF         (SCTP_MIN_SU_NOTIF_APIID + 5)
#define SCTP_API_SYS_ERROR_NOTF                (SCTP_MIN_SU_NOTIF_APIID + 6)
#define SCTP_API_CONNECTION_PENDING_NOTF       (SCTP_MIN_SU_NOTIF_APIID + 7)
#define SCTP_API_DATA_PENDING_NOTF             (SCTP_MIN_SU_NOTIF_APIID + 8)
#define SCTP_API_SEND_FAILURE_NOTF             (SCTP_MIN_SU_NOTIF_APIID + 9)
#define SCTP_API_PART_DATA_PENDING_NOTF        (SCTP_MIN_SU_NOTIF_APIID + 10)
#define SCTP_API_RECV_CONGESTION_NOTF          (SCTP_MIN_SU_NOTIF_APIID + 11)
#define SCTP_API_ADAPTION_IND_NOTF             (SCTP_MIN_SU_NOTIF_APIID + 12)
#define SCTP_API_PEER_DATA_DROP_NOTF           (SCTP_MIN_SU_NOTIF_APIID + 13)
#define SCTP_API_SHUTDOWN_EVENT_NOTF           (SCTP_MIN_SU_NOTIF_APIID + 14)
#define SCTP_API_LCL_IPC_ERROR_NOTF            (SCTP_MIN_SU_NOTIF_APIID + 15)

#define SCTP_MAX_SU_NOTIF_APIID                SCTP_API_LCL_IPC_ERROR_NOTF

#define SCTP_NUM_SU_NOTIF_APIID  (SCTP_MAX_SU_NOTIF_APIID - SCTP_MIN_SU_NOTIF_APIID + 1)
    /*** other parameters *******************************************/


#define SCTP_NOTF_TRANS_ID   0

    /*
     *Change the following parameter if reqd. It denotes the max data size
     *in bytes which will be received in message based interface.
     */
#define MAX_USER_DATA_SIZE              1024

    typedef    enum
    {
        SCTP_SIMPLE_UNORDERED            = 0,
        SCTP_ORDERED                     = 1,
        SCTP_PREFERENTIAL_UNORDERED      = 3
    }
                                       sctp_order_flag_t;


    /************** Protocol Request API's Parameter list **********************/

    /* register su parameter list */
    typedef    struct
    {
        sctp_U16bit       su_id;
    } sctp_msg_register_su_st;

    /* register su resp parameter list */
    typedef    struct
    {
        sctp_return_t result;
        sctp_error_t  errorno;
        sctp_U16bit   su_id;
        sctp_U16bit   appl_id;
    } sctp_msg_register_su_res_st;


    /* deregister su parameter list    */
    typedef    struct
    {
        sctp_U16bit  appl_id;
    } sctp_msg_deregister_su_st;

    /* deregister su resp parameter list    */
    typedef    struct
    {
        sctp_return_t result;
        sctp_error_t  error;
        sctp_U16bit   appl_id;
    } sctp_msg_deregister_su_res_st;


    /* Socket API  parameter list*/
    typedef    struct
    {
        sctp_U16bit   appl_id;
        sctp_U32bit   addr_family;
    } sctp_msg_socket_st;

    /* Socket API  response parameter list*/
    typedef    struct
    {
        sctp_return_t result;
        sctp_error_t  error;
        sctp_U32bit   assoc_id;
        sctp_U16bit   appl_id;
    } sctp_msg_socket_res_st;


    /* Bind    API parameter list */
    typedef    struct
    {
        sctp_U32bit            association_id;
        sctp_U16bit            sctp_port;
        sctp_U32bit            no_of_su_addrs;
        sctp_sockaddr_list_st  list_su_addrs;
        sctp_U8bit             hostname[50];
    } sctp_msg_bind_st;

    /* Bind    API response parameter list */
    typedef    struct
    {
        sctp_return_t  result;
        sctp_error_t   error;
        sctp_U32bit    association_id;
    } sctp_msg_bind_res_st;

    /* Bind    API parameter list */
    typedef    struct
    {
        sctp_U32bit            association_id;
        sctp_bindx_flag_et     flags;
        sctp_U32bit            no_of_su_addrs;
        sctp_sockaddr_list_st  list_su_addrs;
    } sctp_msg_bindx_st;

    /* Bind    API response parameter list */
    typedef    struct
    {
        sctp_return_t  result;
        sctp_error_t   error;
        sctp_U32bit    association_id;
    } sctp_msg_bindx_res_st;

    /* Listen API parameter list */
    typedef    struct
    {
        sctp_U32bit            association_id;
        sctp_U32bit            back_log;
    } sctp_msg_listen_st;

    /* Listen API response parameter list */
    typedef    struct
    {
        sctp_return_t  result;
        sctp_error_t   error;
        sctp_U32bit    association_id;
    } sctp_msg_listen_res_st;

    /* Config Socket API parameter list */
    typedef    struct
    {
        sctp_U32bit            association_id;
        sctp_prot_params_st    protocol_parameters;
    } sctp_msg_config_assoc_st;

    /* Config Socket API resp parameter list */
    typedef    struct
    {
        sctp_return_t  result;
        sctp_error_t   error;
        sctp_U32bit    association_id;
    } sctp_msg_config_assoc_res_st;

    /* Config assoc_ctl API parameter list */
    typedef    struct
    {
        sctp_U32bit               association_id;
        sctp_prot_params_et       prot_param;
        sctp_U32bit               param_value;
    } sctp_msg_config_assoc_ctl_st;

    /* Config assoc_ctl API resp parameter list */
    typedef    struct
    {
        sctp_return_t             result;
        sctp_error_t              error;
        sctp_U32bit               association_id;
    } sctp_msg_config_assoc_ctl_res_st;


    /* set failure threshold  API parameter    list */
    typedef    struct
    {
        sctp_U32bit                association_id;
        sctp_sockaddr_st           dest_address;
        sctp_U32bit                path_max_retrans;
    } sctp_msg_set_fthresh_st;

    /* set failure threshold  API response parameter    list */
    typedef    struct
    {
        sctp_U32bit    association_id;
        sctp_return_t  result;
        sctp_error_t   error;
    } sctp_msg_set_fthresh_res_st;

    /* connect API parameter list */
    typedef    struct
    {
        sctp_U32bit                  association_id;
        sctp_U16bit                  dest_sctp_port;
        sctp_sockaddr_st             connect_dest_address;
        sctp_U32bit                  payload_prot_id;
    } sctp_msg_connect_st;

    /* connect  API response parameter    list */
    typedef    struct
    {
        sctp_U32bit    association_id;
        sctp_return_t  result;
        sctp_error_t   error;
    } sctp_msg_connect_res_st;

    /* connect extended API    parameter list */
    typedef    struct
    {
        sctp_U32bit                association_id;
        sctp_U16bit                dest_sctp_port;
        sctp_U16bit                num_out_streams;
        sctp_U16bit                num_in_streams;
        sctp_sockaddr_st           connect_dest_address;
        sctp_U32bit                payload_prot_id;
    } sctp_msg_connect_ext_st;

    /* connect  ext API response parameter    list */
    typedef    struct
    {
        sctp_U32bit               association_id;
        sctp_return_t             result;
        sctp_error_t              error;
    } sctp_msg_connect_ext_res_st;

    /* accept API parameter    list */
    typedef    struct
    {
        sctp_U32bit                association_id;
        sctp_U32bit                payload_prot_id;
    } sctp_msg_accept_st;

    /* accept API resp parameter    list */
    typedef    struct
    {
        sctp_U32bit               association_id;
        sctp_U32bit               nos_of_dest_trans_addrs;
        sctp_sockaddr_list_st     list_of_dest_trans_addrs;
        sctp_U16bit               sctp_port;
        sctp_U32bit               new_association_id;
        sctp_return_t             result;
        sctp_error_t              errorno;
    } sctp_msg_accept_res_st;


    /* accept extended API parameter list */
    typedef    struct
    {
        sctp_U32bit             association_id;
        sctp_U32bit             payload_prot_id;
        sctp_U16bit             num_out_streams;
        sctp_U16bit             num_in_streams;
    } sctp_msg_accept_ext_st;

    /* accept extended resp API parameter list */
    typedef    struct
    {
        sctp_U32bit               association_id;
        sctp_U16bit               num_out_streams;
        sctp_U16bit               num_in_streams;
        sctp_U32bit               nos_of_dest_trans_addrs;
        sctp_sockaddr_list_st     list_of_dest_trans_address;
        sctp_U16bit               sctp_port;
        sctp_U32bit               new_association_id;
        sctp_return_t             result;
        sctp_error_t              errorno;
    } sctp_msg_accept_ext_res_st;

    /* send    API parameter list */
    typedef    struct
    {
        sctp_U32bit         association_id;
        sctp_U32bit         context;
        sctp_U16bit         stream_id;
        sctp_sockaddr_st    dest_address;
        sctp_U32bit         data_size;
        sctp_U8bit          *data_payload;
    } sctp_msg_send_st;

    /* send    resp API parameter list */
    typedef    struct
    {
        sctp_U32bit     association_id;
        sctp_return_t   result;
        sctp_error_t    errorno;
    } sctp_msg_send_res_st;

    /* send    API parameter list */
    typedef    struct
    {
        sctp_U32bit             association_id;
        sctp_U32bit             context;
        sctp_sockaddr_st        dest_address;
        sctp_U32bit             data_size;
        sctp_U32bit             life_time;
        sctp_order_flag_t       order_flag;
        sctp_Boolean_t          bundle_flag;
        sctp_U32bit             payload_prot_id;
        sctp_U16bit             stream_id;
        sctp_U8bit              *data_payload;
    } sctp_msg_send_ext_st;

    typedef    struct
    {
        sctp_U32bit             association_id;
        sctp_U32bit             context;
        sctp_sockaddr_st        dest_address;
        sctp_U32bit             iovec_size;
        sctp_U32bit             life_time;
        sctp_order_flag_t       order_flag;
        sctp_Boolean_t          bundle_flag;
        sctp_U32bit             payload_prot_id;
        sctp_U16bit             stream_id;
        sctp_iovec_st           *p_iovec;
    } sctp_msg_send_iovec_st;


    /* send ext    resp API parameter list */
    typedef    struct
    {
        sctp_U32bit    association_id;
        sctp_return_t  result;
        sctp_error_t   errorno;
    } sctp_msg_send_ext_res_st;

    /* receive API parameter list */
    typedef    struct
    {
        sctp_U32bit        association_id;
    } sctp_msg_receive_st;

    /* receive API response parameter list */
    typedef    struct
    {
        sctp_U32bit   association_id;
        sctp_U32bit   protocol_id;
        sctp_U32bit   data_size;
        sctp_U16bit   stream_id;
        sctp_return_t result;
        sctp_error_t  errorno;
        sctp_U8bit    *data;
    } sctp_msg_receive_res_st;

    /* receive extended API    parameter list */
    typedef    struct
    {
        sctp_U32bit        association_id;
        sctp_Boolean_t     peek_flag;
        sctp_U16bit        stream_id;
    } sctp_msg_receive_ext_st;

    /* receive API ext response parameter list */
    typedef    struct
    {
        sctp_U32bit           association_id;
        sctp_U16bit           stream_id;
        sctp_sockaddr_st      dest_address;
        sctp_U32bit           protocol_id;
        sctp_U32bit           data_size;
        sctp_return_t         result;
        sctp_error_t          errorno;
        sctp_U8bit            *data;
    } sctp_msg_receive_ext_res_st;

    /* receive partial data    API parameter list */
    typedef    struct
    {
        sctp_U32bit        association_id;
        sctp_Boolean_t     peek_flag;
        sctp_U16bit        stream_id;
    } sctp_msg_receive_part_st;

    /* receive partial data    API response parameter list */
    typedef    struct
    {
        sctp_U32bit           association_id;
        sctp_U16bit           stream_id;
        sctp_sockaddr_st      dest_address;
        sctp_U32bit           protocol_id;
        sctp_U32bit           data_size;
        sctp_U16bit           ssn;
        sctp_U16bit           delivery_num;
        sctp_return_t         result;
        sctp_error_t          errorno;
        sctp_U8bit            *data;
    } sctp_msg_receive_part_res_st;

    /* close  API parameter    list */
    typedef    struct
    {
        sctp_U32bit            association_id;
        sctp_Boolean_t         graceful;
    } sctp_msg_close_st;

    typedef struct
    {
        sctp_U32bit     association_id;
        sctp_U16bit     cause_len;
        sctp_U8bit      *cause;
    } sctp_msg_close_ext_st;


    /* close  API resp parameter    list */
    typedef    struct
    {
        sctp_U32bit    association_id;
        sctp_return_t  result;
        sctp_error_t   errorno;
    } sctp_msg_close_res_st;

    /* Heartbeat Action API    parameter list */
    typedef    struct
    {
        sctp_U32bit            association_id;
        sctp_sockaddr_st       dest_address;
        sctp_HB_action_et      heartbeat_action;
        sctp_U32bit            heartbeat_interval;
    } sctp_msg_hb_action_st;

    /* Heartbeat Action API resp   parameter list */
    typedef    struct
    {
        sctp_U32bit    association_id;
        sctp_return_t  result;
        sctp_error_t   errorno;
    } sctp_msg_hb_action_res_st;


    /*  Set    Primary    API parameter list */
    typedef    struct
    {
        sctp_U32bit          association_id;
        sctp_sockaddr_st     dest_address;
        sctp_sockaddr_st     src_address;
    } sctp_msg_set_primary_st;

    /*  Set    Primary    API response parameter list */
    typedef    struct
    {
        sctp_U32bit          association_id;
        sctp_return_t        result;
        sctp_error_t         errorno;
    } sctp_msg_set_primary_res_st;

    /*  Get    Srtt API parameter list    */
    typedef    struct
    {
        sctp_U32bit          association_id;
        sctp_sockaddr_st     dest_address;
    } sctp_msg_get_srtt_st;

    /*  Get    Srtt API response parameter list    */
    typedef    struct
    {
        sctp_U32bit           association_id;
        sctp_sockaddr_st      dest_address;
        sctp_U32bit           srtt_value;
        sctp_return_t         result;
        sctp_error_t          errorno;
    } sctp_msg_get_srtt_res_st;

    /*  Get    Status API parameter list */
    typedef    struct
    {
        sctp_U32bit            association_id;
    } sctp_msg_get_status_st;

    /*  Get    Status API response parameter list */
    typedef    struct
    {
        sctp_U32bit                  association_id;
        sctp_return_t                result;
        sctp_error_t                 errorno;
        sctp_status_params_st        assoc_status;

    } sctp_msg_get_status_res_st;

    /*  Get    Destination Status API parameter list */
    typedef    struct
    {
        sctp_U32bit            association_id;
    } sctp_msg_get_deststatus_st;

    /*  Get    Destination Status API response parameter list */
    typedef    struct
    {
        sctp_U32bit                    association_id;
        sctp_return_t                  result;
        sctp_error_t                   errorno;
        sctp_U32bit                    nos_of_dest_trans_addrs;
        sctp_sockaddr_list_st          list_of_dest_trans_addrs;
        sctp_dest_status_param_list_st dest_param_list;
        sctp_sockaddr_st               primary_dest_transport_addr;
    } sctp_msg_get_deststatus_res_st;



    /********************************************/

    /*  Retrieve unsent datagram API parameter list */
    typedef    struct
    {
        sctp_U32bit            association_id;
    } sctp_msg_ret_unsent_st;

    /*  Retrieve unsent  API resp parameter list */
    typedef    struct
    {
        sctp_U32bit    association_id;
        sctp_return_t  result;
        sctp_error_t   errorno;
        sctp_U32bit    context;
        sctp_U32bit    data_size;
        sctp_U32bit    ppi;
        sctp_U16bit    stream;
        sctp_U8bit     *data;
    } sctp_msg_ret_unsent_res_st;


    /*  Retrieve unacked API parameter list */
    typedef    struct
    {
        sctp_U32bit            association_id;
    } sctp_msg_ret_unacked_st;

    /*  Retrieve unacked API resp parameter list */
    typedef    struct
    {
        sctp_U32bit    association_id;
        sctp_return_t  result;
        sctp_error_t   errorno;
        sctp_U32bit    context;
        sctp_U32bit    data_size;
        sctp_U32bit    ppi;
        sctp_Boolean_t incomplete_flag;
        sctp_U16bit    stream;
        sctp_U16bit    ssn;
        sctp_U8bit     *data;
    } sctp_msg_ret_unacked_res_st;

    /*  Flush association buffers */
    typedef    struct
    {
        sctp_U32bit            association_id;
    } sctp_msg_flush_buffers_st;

    /*  Retrieve unacked API resp parameter list */
    typedef    struct
    {
        sctp_U32bit     association_id;
        sctp_return_t   result;
        sctp_error_t    errorno;
    } sctp_msg_flush_buffers_res_st;


    /*  Set Restart option API parameter list */
    typedef    struct
    {
        sctp_U32bit              association_id;
        sctp_restart_options_et  restart_option;
    } sctp_msg_set_restart_option_st;

    /*  Set Restart option API resp parameter list */
    typedef    struct
    {
        sctp_U32bit    association_id;
        sctp_return_t  result;
        sctp_error_t   errorno;
    } sctp_msg_set_restart_option_res_st;

    /*  Get Stream Status API parameter list */
    typedef    struct
    {
        sctp_U32bit            association_id;
        sctp_U16bit            stream_id;
    } sctp_msg_get_stream_status_st;

    /*  Get Stream Status API resp parameter list */
    typedef    struct
    {
        sctp_U32bit    association_id;
        sctp_return_t  result;
        sctp_error_t   errorno;
        sctp_U16bit    stream_id;
        sctp_U16bit    ssn;
    } sctp_msg_get_stream_status_res_st;

    /*  Abort Retrieve API parameter list */
    typedef    struct
    {
        sctp_U32bit            association_id;
    } sctp_msg_abort_retrieve_st;

    /*  Abort Retrieve API resp parameter list */
    typedef    struct
    {
        sctp_U32bit     association_id;
        sctp_return_t   result;
        sctp_error_t    errorno;
        sctp_Boolean_t  data_pending;
    } sctp_msg_abort_retrieve_res_st;

    /*  CONFIG_CONG_REG_DEF API parameter list */
    typedef    struct
    {
        sctp_U32bit            association_id;
        sctp_cong_reg_def_st   cong_reg_def;
        sctp_Boolean_t         is_transmit;
    } sctp_msg_config_cong_reg_def_st;

    /*  CONFIG_CONG_REG_DEF API resp parameter list */
    typedef    struct
    {
        sctp_U32bit     association_id;
        sctp_return_t   result;
        sctp_error_t    errorno;
    } sctp_msg_config_cong_reg_def_resp_st;

    /*  GET_CONG_REG_DEF API parameter list */
    typedef    struct
    {
        sctp_U32bit            association_id;
        sctp_Boolean_t         is_transmit;
    } sctp_msg_get_cong_reg_def_st;

    /*  GET_CONG_REG_DEF API resp parameter list */
    typedef    struct
    {
        sctp_U32bit            association_id;
        sctp_return_t          result;
        sctp_error_t           errorno;
        sctp_cong_reg_def_st   cong_reg_def;
    } sctp_msg_get_cong_reg_def_resp_st;

    /*  CONFIG_TXRX_NUM_FRAG API parameter list */
    typedef    struct
    {
        sctp_U32bit            association_id;
        sctp_count_frags_st    count_frags;
        sctp_Boolean_t         is_transmit;
        sctp_Boolean_t         is_increment;
    } sctp_msg_config_txrx_num_frag_st;

    /*  CONFIG_TXRX_NUM_FRAG_RESP API resp parameter list */
    typedef    struct
    {
        sctp_U32bit     association_id;
        sctp_return_t   result;
        sctp_error_t    errorno;
    } sctp_msg_config_txrx_num_frag_resp_st;

    /*  GET_FREE_TXRX_NUM_FRAG API parameter list */
    typedef    struct
    {
        sctp_U32bit            association_id;
    } sctp_msg_get_free_tr_num_frag_st;

    /*  CONFIG_TXRX_NUM_FRAG_RESP API resp parameter list */
    typedef    struct
    {
        sctp_U32bit     association_id;
        sctp_return_t   result;
        sctp_error_t    errorno;
        sctp_count_frags_st    tx_num_frags;
        sctp_count_frags_st    rx_num_frags;
    } sctp_msg_get_free_tr_num_frag_resp_st;


    /*Connectx Request Structure*/
    typedef    struct
    {
        sctp_U32bit                  association_id;
        sctp_U16bit                  dest_sctp_port;
        sctp_U32bit                  num_dest_addrs;
        sctp_sockaddr_list_st        connect_dest_address;
        sctp_Boolean_t               block;
        sctp_U32bit                  payload_prot_id;
        sctp_U16bit                  p_num_ostreams;
        sctp_U16bit                  p_num_istreams;
    } sctp_msg_connectx_st;

    /* connectx response structure */
    typedef    struct
    {
        sctp_U32bit    association_id;
        sctp_return_t  result;
        sctp_error_t   error;
    } sctp_msg_connectx_res_st;


    /****************** notifications ***********************/
    typedef        struct
    {
        sctp_U32bit   association_id;
        sctp_U16bit   err_code;
        sctp_U16bit   err_length;
        sctp_U8bit    *sys_error_info;
    } sctp_msg_op_error_peer_notf_st;

    typedef        struct
    {
        sctp_U32bit                 association_id;
        sctp_sockaddr_st            destination_ip_add;
        sctp_sock_spc_event_et      dest_act_status;
    } sctp_msg_destination_status_chg_notf_st;

    typedef        struct
    {
        sctp_U32bit                 association_id;
        sctp_sock_sac_event_et      endpoint_status;
        sctp_endpoint_chg_data_ut   status_change_data;
    } sctp_msg_endpoint_status_chg_notf_st;

    typedef        struct
    {
        sctp_U32bit                 association_id;
        sctp_cong_level_et          cong_level;
    } sctp_msg_congestion_notf_st;

    typedef        struct
    {
        sctp_U32bit                 association_id;
        sctp_error_t                errorno;
    } sctp_msg_sys_error_notf_st;

    typedef        struct
    {
        sctp_U32bit                 association_id;
    } sctp_msg_connection_pending_notf_st;

    typedef        struct
    {
        sctp_U32bit                 association_id;
        sctp_U16bit                 stream_id;
#ifdef SCTP_SEND_DATA_WITH_IND
        sctp_U32bit                 ppi;
        sctp_U32bit                 size;
        sctp_U8bit                  *data;
#endif

    } sctp_msg_data_pending_notf_st;

    typedef        struct
    {
        sctp_U32bit                 association_id;
        sctp_U32bit                 context; /* void pointer as U32 bits */
        sctp_error_t                cause_code;
    } sctp_msg_send_failure_notf_st;

    typedef        struct
    {
        sctp_U32bit                 assoc_id;
        sctp_U16bit                 stream;
    } sctp_msg_partial_datapending_notf_st;

    typedef        struct
    {
        sctp_U32bit                 association_id;
        sctp_cong_level_et          cong_level;
    } sctp_msg_recv_congestion_notf_st;

    typedef    struct
    {
        sctp_U32bit        assoc_id;
        sctp_U32bit        adaption_bits;
    } sctp_msg_adaption_ind_notf_st;

    typedef        struct
    {
        sctp_U32bit         association_id;
        sctp_U16bit         ecode;
        sctp_U32bit         buffer_len;
        sctp_U8bit          buffer[SCTP_MAX_MTUSIZE];
    } sctp_msg_peer_data_drop_notf_st;

    typedef        struct
    {
        sctp_U32bit                 association_id;
    } sctp_msg_shutdown_event_notf_st;

    typedef struct
    {
        sctp_dest_id_t      dest_stk_id;
        sctp_error_t        err_code;

    } sctp_msg_lcl_ipc_error_notf_st;

    extern sctp_return_t
    sctp_chk_api_config_socket(
        sctp_Pvoid_t        p_api_elements,   /* api elements */
        sctp_Error_t        *p_ecode );         /* error code */

    extern sctp_void_t
    sctp_su_process_stack_buffer(
        sctp_U8bit *p_buffer,
        sctp_len_t nooctets );

    extern sctp_return_t
    send_to_sctp(
        sctp_trans_id_t         transaction_id,
        sctp_api_id_t           api_id,
        sctp_src_id_t           src_id,
        sctp_dest_id_t          dest_id,
        sctp_Pvoid_t            p_api_elements,
        sctp_Error_t            *p_ecode );

    extern void
    sctp_process_api_resp_from_stack(
        sctp_src_id_t,
        sctp_dest_id_t,
        sctp_trans_id_t,
        sctp_api_id_t,
        sctp_void_t * );


    extern void
    sctp_process_notf_from_stack(
        sctp_src_id_t,
        sctp_dest_id_t,
        sctp_api_id_t,
        sctp_void_t * );

#ifdef  __cplusplus
}
#endif

#endif /* __FILE_sctp_parse_api_SEEN__  */
