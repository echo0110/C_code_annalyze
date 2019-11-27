/***************************************************************************
 *  FILE NAME  : s_cotyps.h
 *
 *  DESCRIPTION: Contains the datatypes used by the stack.
 *
 *  DATE     NAME      REF#            REASON
 *  -----------------------------------
 *  4Apr00    sigtran    team          Original
 *  02jan02  hbhondwe SCTP_IPC_TCP     ipc changes for tcp
 *  27Aug02  mrajpal                   SPR 4798, 4800
 *  25Aug03  mrajpal                   Rel 5.0
 *  07Oct03  Vivek Bansal    Rel 5.0   Changes for CSPL
 *  27Nov03  Vivek Bansal    Rel 5.0   Changes for CSPL
 *  08Jul04  Nitin Khanna    Rel 5.0   SPR 6854
 *  20Oct04  Amit Ray        Rel 6.0   SCTP/FUNC-GEN-3
 *  22Oct04  Amit Ray        Rel 6.0   SCTP/FUNC-IG-14
 *  7JULY08  ShikhaKathpal   Rel 6.2.0 RFC 4960 UPGRADE
 *  25May07  External                  Changes for CSR#1-4267892
 *  15Sep08  Raza Mohamed    Rel 6.2   Changes for Redundancy.
 *  24May10  Akanskha        SPR 20406 CSR 1-8599117-Post Restart,handling incomplete
 *  12july10 Sharad Kumar Yadav  SPR 20526 Rel 6.2.0  Fix for CSR# 1-7670333
 *  25Jul10  Rajnesh         SPR Fix20540  Appid updated in Red
 *  25mar11  Akanksha        SPR 20859 Enhancement for Callback
 *  6Mar14   Rohan Pawa                CSR 69326
 *  05May14  Rohan Pawa       Bug Id 5       CSR 77081
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#ifndef __FILE_sctp_types_SEEN__
#define __FILE_sctp_types_SEEN__


#include    <s_includ.h>
#include    <s_osmutx.h>
#include    <s_cogbl.h>
#include    <s_colst.h>
#include    <s_os.h>

/* Basic data types */
typedef  __sctp_U8bit       sctp_U8bit;
typedef  __sctp_U16bit      sctp_U16bit;

typedef  __sctp_U32bit      sctp_U32bit;
typedef  __sctp_S32bit      sctp_S32bit;
typedef  sctp_U32bit        sctp_Error_t;
typedef  sctp_U32bit        sctp_stack_id_t;
typedef  sctp_U32bit        sctp_Len_t;
typedef  __sctp_void_t     *sctp_Pvoid_t;
typedef  __sctp_void_t      sctp_void_t;
typedef  sctp_U32bit       *sctp_Parameterlist_t;
typedef  sctp_U8bit         sctp_module_id_t;
typedef  sctp_U16bit        sctp_port_t;
typedef  sctp_U16bit        sctp_stream_t;
typedef  sctp_U32bit        sctp_assoc_t;
typedef  __sctp_time_t      sctp_time_t ;
typedef sigtran_thread_t    sctp_thread_id;

/* Forward declaration */
struct __sctp_tcb_st;

#ifdef SCTP_RED_SUPPORT
#ifdef SCTP_UI_MESSAGEBASED
#ifdef SCTP_CLI_RECV_COM_THREAD

typedef enum
{
    SCTP_EVT_TIMER = 0,
    SCTP_EVT_CLIENT_MSG,
    SCTP_EVT_PEER_MSG,
    SCTP_EVT_ICMP_MSG,
    SCTP_EVT_RESP_NOTF
} sctp_event_et;

#endif /* SCTP_CLI_RECV_COM_THREAD */
#endif /* SCTP_UI_MESSAGEBASED */
#endif /* SCTP_RED_SUPPORT */


typedef    enum
{
    SCTP_CLIENT_SU,
    SCTP_CLIENT_SM
} sctp_client_et;

typedef   enum
{
    SCTP_FAILURE = 0,
    SCTP_SUCCESS = 1
} sctp_return_t;

typedef   enum
{
    SCTP_FALSE = 0,
    SCTP_TRUE  = 1
} sctp_Boolean_t;

typedef   enum
{
    SCTP_CRC32    = 0,
    SCTP_ADLER32  = 1,
    SCTP_NO_CHECKSUM = 2,
    SCTP_NUM_CHECKSUM
} sctp_checksum_et;



typedef   enum
{
    SCTP_RESTARTOPTION_REQUE     = 0,
    SCTP_RESTARTOPTION_RETRIEVE  = 1
} sctp_restart_options_et;

typedef   enum
{
    SCTP_HB_ENABLE  = 0,
    SCTP_HB_DISABLE = 1,
    SCTP_HB_REQUEST = 2
} sctp_HB_action_et;


/* The SCTP Address structures. */

/* IPv6 address */
/* The IPv6 address structure. */
typedef struct __sctp_in6_addr_st
{
    union
    {
        uint8_t     u6_addr8[16];
        sctp_U16bit u6_addr16[8];
        uint32_t    u6_addr32[4];
    } in6_u;
} sctp_in6_addr_st;
#define s_u6_addr32 in6_u.u6_addr32
#define s_u6_addr16 in6_u.u6_addr16
#define s_u6_addr8  in6_u.u6_addr8
#define SCTP_HTONL htonl
#define SCTP_NTOHL ntohl

/* The Address structure in case family is SCTP_AF_INET */
typedef struct __sctp_sockaddr_in_st
{
    sctp_U32bit        addr;
} sctp_sockaddr_in_st;


/* The Address structure in case family is SCTP_AF_INET6 */
typedef struct __sctp_sockaddr_in6_st
{
    sctp_U32bit             flow_info;
    sctp_U32bit             scope_id;
    sctp_in6_addr_st        addr;
} sctp_sockaddr_in6_st;


/* Each Address element is a of type this. */
typedef struct __sctp_sockaddr_st
{
    sctp_sa_family_t   family;
    union
    {
        sctp_sockaddr_in_st     v4;
        sctp_sockaddr_in6_st    v6;
    } ip;
} sctp_sockaddr_st;

typedef   sctp_sockaddr_st  sctp_sockaddr_list_st[SCTP_MAX_TRANSPORT_ADDR];

/* CSR 69326 fix starts*/
#define SCTP_IS_INADDR_ANY(sock_addr)  sigtran_is_addr_any(sock_addr)

sigtran_U32bit sigtran_is_addr_any( sctp_sockaddr_st sock_addr );
/* CSR 69326 fix ends*/

/*************************** Error Module types ***************************/
typedef   enum
{
    SCTP_INTF_TRC   = 0x1,
    SCTP_ERR_TRC    = 0x2,
    SCTP_DBG_TRC    = 0x4,
    SCTP_ALL_TRC    = 0x7
} sctp_trc_et;

/*************************** Error Module types ***************************/
typedef   enum
{
    SCTPERROR_FLAG_SYSERROR          = 0,
    SCTPERROR_FLAG_PROTOCOLERROR     = 1,
    SCTPERROR_FLAG_APIERROR          = 2,
    SCTPERROR_FLAG_INTERNAL_ERROR    = 3,
    SCTPERROR_FLAG_ALL_ERRORS        = 4
} sctp_error_flag_et;

#define   SCTP_MIN_ERROR_FLAG     SCTPERROR_FLAG_SYSERROR
#define   SCTP_MAX_ERROR_FLAG     SCTPERROR_FLAG_INTERNAL_ERROR

typedef   enum
{
    SCTPERROR_LEVEL_NOERROR    = 0,
    SCTPERROR_LEVEL_MINOR      = 1,
    SCTPERROR_LEVEL_MAJOR      = 4,
    SCTPERROR_LEVEL_CRITICAL   = 8
} sctp_error_level_et;

/**************************** Redundancy module ****************************/
typedef    enum
{
    SCTP_STACK_RED_STATE_STANDBY = 0,
    SCTP_STACK_RED_STATE_ACTIVE  = 1
} sctp_stack_red_state_et;

typedef    enum
{
    SCTP_PROC_STATE_STOP     = 0,
    SCTP_PROC_STATE_START    = 1
} sctp_stack_proc_state_et;

#define SCTP_INVALID_U32    0xffffffff


/********************** Statistics Module types ****************************/
typedef   enum
{
    SCTP_PROT_INTERNAL_STATS    = 0,
    SCTP_GLOBAL_TRAFFIC_STATS   = 1,
    SCTP_ERROR_STATS            = 2,
    SCTP_ASSOC_STATS            = 3,
    SCTP_ALL_STATS              = 4
} sctp_stats_et;

typedef   enum
{
    SCTP_STATS_STATE_DISABLED    = 0,
    SCTP_STATS_STATE_ENABLED     = 1
} sctp_stats_state_et;

typedef   struct
{
    sctp_U32bit     no_applications_reg;
    sctp_U32bit     no_associations_open;
    sctp_U32bit     no_dest_marked_active;
    sctp_U32bit     no_dest_marked_inactive;
    sctp_U32bit     no_endpoints_marked_up;
    sctp_U32bit     no_endpoints_marked_down;
} sctp_proto_internal_stats_st;

typedef   enum
{
    SCTPSTATS_NO_APPL_REG           = 0,
    SCTPSTATS_NO_ASSOC_REG          = 1,
    SCTPSTATS_NO_DEST_ACTIVE        = 2,
    SCTPSTATS_NO_DEST_INACTIVE      = 3,
    SCTPSTATS_NO_ENDPOINT_UP        = 4,
    SCTPSTATS_NO_ENDPOINT_DOWN      = 5
} sctp_protstats_param_et;

#define  SCTP_PROT_STATS_MAX_MEMBERID    SCTPSTATS_NO_ENDPOINT_DOWN

typedef  struct
{
    sctp_U32bit  no_assoc_accepted;
    sctp_U32bit  no_assoc_initiated;

    sctp_U32bit  no_graceful_shutdown_assocs;
    sctp_U32bit  no_ungraceful_shutdown_assocs;

    sctp_U32bit  no_trans_data_chunks;
    sctp_U32bit  no_retrans_data_chunks;

    sctp_U32bit  no_data_chunks_sent;
    sctp_U32bit  no_ord_data_chunks_sent;
    sctp_U32bit  no_unord_data_chunks_sent;
    sctp_U32bit  no_data_chunks_unacked;

    sctp_U32bit  no_data_chunks_recv;
    sctp_U32bit  no_ord_data_chunks_recv;
    sctp_U32bit  no_unord_data_chunks_recv;

    sctp_U32bit  no_bytes_sent_ok;
    sctp_U32bit  no_bytes_recvd_ok;

    sctp_U32bit  no_init_sent;
    sctp_U32bit  no_init_recv;

    sctp_U32bit  no_initack_sent;
    sctp_U32bit  no_initack_recv;

    sctp_U32bit  no_cookie_sent;
    sctp_U32bit  no_cookie_recv;

    sctp_U32bit  no_cookieack_sent;
    sctp_U32bit  no_cookieack_recv;

    sctp_U32bit  no_hb_sent;
    sctp_U32bit  no_hb_recv;

    sctp_U32bit  no_hback_sent;
    sctp_U32bit  no_hback_recv;

    sctp_U32bit  no_sack_sent;
    sctp_U32bit  no_sack_recv;

    sctp_U32bit  no_abort_sent;
    sctp_U32bit  no_abort_recv;

    sctp_U32bit  no_shut_sent;
    sctp_U32bit  no_shut_recv;

    sctp_U32bit  no_shutack_sent;
    sctp_U32bit  no_shutack_recv;

    sctp_U32bit  no_shutcom_sent;
    sctp_U32bit  no_shutcom_recv;

    sctp_U32bit  no_error_sent;
    sctp_U32bit  no_error_recv;

    sctp_U32bit  no_ecne_sent;
    sctp_U32bit  no_ecne_recv;

    sctp_U32bit  no_cwr_sent;
    sctp_U32bit  no_cwr_recv;

    sctp_U32bit  no_datagrams_recvd_in_error;
    sctp_U32bit  no_chk_sum_err_recv;

    sctp_U32bit  no_ootb_recv;

    sctp_U32bit  no_frag_usr_msg;
    sctp_U32bit  no_reassemble_usr_msg;

    sctp_U32bit  no_packets_sent;
    sctp_U32bit  no_packets_recv;

    sctp_time_t  discont_time;

#ifdef SCTP_DYNAMIC_IP_SUPPORT
    sctp_U32bit  no_asconf_sent;
    sctp_U32bit  no_asconf_recv;

    sctp_U32bit  no_asconfack_sent;
    sctp_U32bit  no_asconfack_recv;
#endif
} sctp_global_traf_stats_st;

typedef   enum
{
    SCTPSTATS_NO_ASSOC_ACC                = 0,
    SCTPSTATS_NO_ASSOC_INIT               = 1,

    SCTPSTATS_NO_GRACEFUL_SHUTDOWN        = 2,
    SCTPSTATS_NO_UNGRACEFUL_SHUTDOWN      = 3,

    SCTPSTATS_NO_TRANS_DATACHUNKS         = 4,
    SCTPSTATS_NO_RETRANS_DATACHUNKS       = 5,

    SCTPSTATS_NO_DATACHUNKS_SENT          = 6,
    SCTPSTATS_NO_ORD_DATACHUNKS_SENT      = 7,
    SCTPSTATS_NO_UNORD_DATACHUNKS_SENT    = 8,
    SCTPSTATS_NO_DATACHUNKS_UNACK         = 9,

    SCTPSTATS_NO_DATACHUNKS_RECV          = 10,
    SCTPSTATS_NO_ORD_DATACHUNKS_RECV      = 11,
    SCTPSTATS_NO_UNORD_DATACHUNKS_RECV    = 12,

    SCTPSTATS_NO_DATABYTES_SENT           = 13,
    SCTPSTATS_NO_DATABYTES_RECV           = 14,

    SCTPSTATS_NO_INIT_SENT                = 15,
    SCTPSTATS_NO_INIT_RECV                = 16,

    SCTPSTATS_NO_INITACK_SENT             = 17,
    SCTPSTATS_NO_INITACK_RECV             = 18,

    SCTPSTATS_NO_COOKIE_SENT              = 19,
    SCTPSTATS_NO_COOKIE_RECV              = 20,

    SCTPSTATS_NO_COOKIEACK_SENT           = 21,
    SCTPSTATS_NO_COOKIEACK_RECV           = 22,

    SCTPSTATS_NO_HB_SENT                  = 23,
    SCTPSTATS_NO_HB_RECV                  = 24,

    SCTPSTATS_NO_HBACK_SENT               = 25,
    SCTPSTATS_NO_HBACK_RECV               = 26,

    SCTPSTATS_NO_SACK_SENT                = 27,
    SCTPSTATS_NO_SACK_RECV                = 28,

    SCTPSTATS_NO_ABORT_SENT               = 29,
    SCTPSTATS_NO_ABORT_RECV               = 30,

    SCTPSTATS_NO_SHUT_SENT                = 31,
    SCTPSTATS_NO_SHUT_RECV                = 32,

    SCTPSTATS_NO_SHUTACK_SENT             = 33,
    SCTPSTATS_NO_SHUTACK_RECV             = 34,

    SCTPSTATS_NO_SHUTCOM_SENT             = 35,
    SCTPSTATS_NO_SHUTCOM_RECV             = 36,

    SCTPSTATS_NO_ERROR_SENT               = 37,
    SCTPSTATS_NO_ERROR_RECV               = 38,

    SCTPSTATS_NO_ECNE_SENT                = 39,
    SCTPSTATS_NO_ECNE_RECV                = 40,

    SCTPSTATS_NO_CWR_SENT                 = 41,
    SCTPSTATS_NO_CWR_RECV                 = 42,

    SCTPSTATS_NO_DATACHUNKS_RECV_ERROR    = 43,
    SCTPSTATS_NO_CHK_SUM_ERR_RECV         = 44,
    SCTPSTATS_NO_OOTB_RECV                = 45,

    SCTPSTATS_NO_FRAG_USR_MSG             = 46,
    SCTPSTATS_NO_REASSEMBLE_USR_MSG       = 47,

    SCTPSTATS_NO_PACKETS_SENT             = 48,
    SCTPSTATS_NO_PACKETS_RECV             = 49,
    #ifdef SCTP_DYNAMIC_IP_SUPPORT
    SCTPSTATS_NO_ASCONF_SENT              = 50,
    SCTPSTATS_NO_ASCONF_RECV              = 51,
    SCTPSTATS_NO_ASCONFACK_SENT           = 52,
    SCTPSTATS_NO_ASCONFACK_RECV           = 53,
    SCTPSTATS_DISCONT_TIME                = 54       
#else
    SCTPSTATS_DISCONT_TIME                = 50      
#endif
} sctp_globalstats_param_et;

#define   SCTP_GLOBALSTATS_MAX_MEMBERID  SCTPSTATS_DISCONT_TIME

typedef   struct
{
    sctp_U32bit     API_error_stats_count;
    sctp_U32bit     protcol_error_stats_count;
    sctp_U32bit     system_error_stats_count;
    sctp_U32bit     system_resource_error_stats_count;
    sctp_U32bit     internal_error_stats_count;
} sctp_error_stats_st;


typedef   enum
{
    SCTPSTATS_API_ERRORS          = 0,
    SCTPSTATS_PROTOCOL_ERRORS     = 1,
    SCTPSTATS_SYSTEM_ERRORS       = 2,
    SCTPSTATS_RESOURCE_ERRORS     = 3,
    SCTPSTATS_INTERNAL_ERRORS     = 4
} sctp_errorstats_param_et;

#define   SCTP_ERRORSTATS_MAX_MEMBERID    SCTPSTATS_INTERNAL_ERRORS

typedef   struct
{
    sctp_U32bit  assoc_id;
    sctp_time_t  start_time;
    sctp_time_t  discont_time;
    sctp_U32bit  no_chunks_retrans;
    sctp_U32bit  no_T1_expired;
    sctp_U32bit  no_T2_expired;
    sctp_U32bit  no_dest_address;
    /* SCTP REL 6.2.0 changes start */
    sctp_U32bit  no_in_streams;
    sctp_U32bit  no_out_streams;
    /* SCTP REL 6.2.0 changes end */
#ifdef TORNADO_VXWORKS
    sctp_U32bit  pad[3];
#endif
    struct
    {
        sctp_sockaddr_st  dest_address;

        sctp_U32bit       no_datagrams_sent;
        sctp_U32bit       no_datagrams_unacked;
        sctp_U32bit       no_datagrams_recd;

        sctp_U32bit       no_bytes_sent;
        sctp_U32bit       no_bytes_recvd;

        sctp_time_t       dest_addr_start_time;

#ifdef TORNADO_VXWORKS
        sctp_U32bit       pad[2];
#endif
    } dest_stats[SCTP_MAX_TRANSPORT_ADDR];

    struct
    {
        sctp_sockaddr_st  local_address;
        sctp_time_t   local_addr_start_time;
    } local_addr_stats[SCTP_MAX_TRANSPORT_ADDR];

    struct
    {
        /*Number of times TX MED size buffer used instead of MIN*/
        sctp_U32bit         tx_med_utilized_instead_min;

        /*Number of times TX MAX size buffer used instead of MIN*/
        sctp_U32bit         tx_max_utilized_instead_min;

        /*Number of times TX MAX size buffer used instead of MED*/
        sctp_U32bit         tx_max_utilized_instead_med;

        /*Number of times RX MED size buffer used instead of MIN*/
        sctp_U32bit         rx_med_utilized_instead_min;

        /*Number of times RX MAX size buffer used instead of MIN*/
        sctp_U32bit         rx_max_utilized_instead_min;

        /*Number of times RX MAX size buffer used instead of MED*/
        sctp_U32bit         rx_max_utilized_instead_med;

    } buffer_stat;
    /* Rel 6.2.0 change start */
    struct
    {
        sctp_U32bit no_datachunk_sent;
        sctp_U32bit no_datachunk_resent;
        sctp_U32bit no_datachunk_send_fail;
        sctp_U32bit no_datachunk_recd;
        sctp_U32bit no_datachunk_recd_fail;
        sctp_U32bit no_of_databytes_sent;
    } stream_stats[SCTP_MAX_NUMSTREAM];
    /* Rel 6.2.0 change end */

} sctp_assoc_stats_st;


typedef   enum
{
    SCTPSTATS_ASSOC_DATA_SENT            = 1,
    SCTPSTATS_ASSOC_DATA_UNAC            = 2,
    SCTPSTATS_ASSOC_DATA_RECV            = 3,
    SCTPSTATS_ASSOC_DATABYTES_SENT       = 4,
    SCTPSTATS_ASSOC_DATABYTES_RECV       = 5,
    SCTPSTATS_ASSOC_NO_CHUNKS_RETRANS    = 6,
    SCTPSTATS_ASSOC_NO_T1_EXPIRY         = 7,
    SCTPSTATS_ASSOC_NO_T2_EXPIRY         = 8,
    SCTPSTATS_ASSOC_START_TIME           = 9,

    SCTPSTATS_ASSOC_T_MED_INSTD_MIN      = 10,
    SCTPSTATS_ASSOC_T_MAX_INSTD_MIN      = 11,
    SCTPSTATS_ASSOC_T_MAX_INSTD_MED      = 12,
    SCTPSTATS_ASSOC_R_MED_INSTD_MIN      = 13,
    SCTPSTATS_ASSOC_R_MAX_INSTD_MIN      = 14,
    SCTPSTATS_ASSOC_R_MAX_INSTD_MED      = 15,
    /* Rel 6.2.0  change start*/
    SCTPSTATS_ASSOC_STREAM_DATA_SENT      = 16,
    SCTPSTATS_ASSOC_STREAM_DATA_RESENT    = 17,
    SCTPSTATS_ASSOC_STREAM_DATA_SEND_FAIL = 18,
    SCTPSTATS_ASSOC_STREAM_DATA_RECD      = 19,
    SCTPSTATS_ASSOC_STREAM_DATA_RECD_FAIL = 20,
    SCTPSTATS_ASSOC_STREAM_DATABYTES_SENT = 21
} sctp_assoc_stats_param_st;
/* Rel 6.2.0 change end*/
typedef union
{
    sctp_proto_internal_stats_st      prot_stats;
    sctp_global_traf_stats_st         traf_stats;
    sctp_error_stats_st               error_stats;
    sctp_assoc_stats_st               assoc_stats;
} sctp_stats_data_ut;


#define   SCTP_ASSOCSTATS_MAX_MEMBERID   SCTPSTATS_ASSOC_R_MAX_INSTD_MED

/*
 * For Socket Interface.
 */
typedef   enum
{
    SCTP_SHUT_RD   = 0,
    SCTP_SHUT_WR   = 1,
    SCTP_SHUT_RDWR = 2
} sctp_sock_shutdown_type_et;

typedef   enum
{
    SCTP_INIT      = 11,
    SCTP_SNDRCV    = 12
} sctp_cmsg_type_et;

/*
 * These are used in the stack and ulp for the notifications
 * e.g. User enables ASSOC_STATE_CHANGE by giving SCTP_RECVASSOCEVNT and
 * when this event occurs then the flag set is SCTP_NTFY_ASSOC_CHANGE.
 */
typedef   enum
{
    SCTP_ASSOC_CHANGE = 0,
    SCTP_PEER_ADDR_CHANGE,
    SCTP_REMOTE_ERROR,
    SCTP_SEND_FAILED,
    SCTP_SHUTDOWN_EVENT,
    SCTP_ADAPTION_EVENT,
    SCTP_PARTIAL_DELIVERY
} sctp_sock_ntfy_type_et;

/* Used for indications of DestStatus change and ASSOC CHANGE */
typedef   enum
{
    SCTP_COMMUNICATION_UP = 0,     /* SCTP_ENDPOINT_UP */
    SCTP_COMMUNICATION_LOST,       /* SCTP_ENDPOINT_DOWN */
    SCTP_RESTART,                  /* SCTP_ENDPOINT_RESTARTED */
    SCTP_SHUTDOWN_COMPLETE,        /* New one */
    SCTP_CANT_START_ASSOC,         /* New one */
    SCTP_ENDPOINT_DOWN_RETRIEVABLE, /* Used only in HSS specific interface */
    /* Following will be not used in case SCTP_DYNAMIC_IP_SUPPORT in not defined */
    SCTP_ENDPOINT_ADDR_CHNG
} sctp_sock_sac_event_et;

/* Used for indication of dst status change notification */
typedef   enum
{
    SCTP_ADDRESS_AVAILABLE = 0,
    SCTP_ADDRESS_UNREACHABLE,
    SCTP_ADDRESS_REMOVED,
    SCTP_ADDRESS_ADDED,
    SCTP_ADDRESS_MADE_PRIM,
    SCTP_ADDRESS_CONFIRMED
} sctp_sock_spc_event_et;


/* Flags used in the SENDFAILED Indication */
typedef   enum
{
    SCTP_DATA_UNSENT = 0,
    SCTP_DATA_SENT
} sctp_sock_ssf_flag_et;

/***************************** other cons ********************************/
typedef   struct
{
    sctp_U32bit          srtt;
    sctp_U32bit          rto;
    sctp_U32bit          mtu_size;
    sctp_U32bit          cwnd;
    /* Changes for CSR#1-4267892 */
    sctp_U32bit           allow_heartbeat;
    sctp_U32bit           status;
    /* End of changes for CSR#1-4267892 */
    sctp_Boolean_t       is_confirmed;
} sctp_dest_status_param_st;

typedef   sctp_dest_status_param_st
sctp_dest_status_param_list_st[SCTP_MAX_TRANSPORT_ADDR];

/* Start SPR 20526 */
/* Structure for RTT related Information */
typedef struct
{
    sctp_U32bit           min_rtt;
    sctp_U32bit           max_rtt;
    sctp_U32bit           last_recorded_rtt;
    sctp_U32bit           srtt;
} sctp_rtt_info_st;

typedef  sctp_rtt_info_st  sctp_rtt_info_list_st[SCTP_MAX_TRANSPORT_ADDR];
/* END SPR 20526 */

#define   SCTP_MAX_INIT_TLVPARMS    10

typedef   struct
{
    sctp_U16bit           cause_code;
    sctp_U16bit           cause_len;
    union
    {
        struct
        {
            sctp_stream_t stream_id;
            sctp_U16bit   unused;
        } invalid_stream;

        struct
        {
            sctp_U32bit     num_missing_params;
            sctp_U16bit     param_id[SCTP_MAX_INIT_TLVPARMS];
        } missing_params;

        struct
        {
            sctp_U32bit     time_value;
        } stale_cookie_error;

        struct
        {
            sctp_U8bit      *addr;
        } unresolve_addr;

        struct
        {
            sctp_U8bit      *chunk;
        } unrecognize_chunk_type;

        struct
        {
            sctp_U8bit      *parameters;
        } unrecognize_parameters;

        struct
        {
            sctp_U32bit     tsn;
        } no_user_data;

        struct
        {
            sctp_U8bit     *cause;
        } user_initiated_abort;
    } cause_info;
} sctp_error_cause_st;


typedef   enum
{
    E_NOERROR  = 0,
    EINV_APPID,
    EINV_ADDRS,
    EINV_PARAMS,
    E_NORESOURCES,
    EINV_TYPEOFSTATS,
    E_STATSDISABLED,
    EINV_ERRORNUM,
    EINV_ERRORLEVEL,
    E_ERRORDISABLED,
    EINV_TRCTYPE,
    EINV_TRCLEVEL,
    E_TRCDISABLED,
    EINV_ASSOCID,
    E_REQ_IN_WRONG_STATE,
    E_NOTHING_IN_QUEUE,
    E_NO_PENDING_REQUEST,
    E_NO_PENDING_CONNECTION,
    E_UNKNOWN_HOST,
    E_SCTPPROT_INVALID_TLDATAGRAM,
    E_INVALID_CHUNKID,
    E_INVALID_TL_SIZE,
    E_INVALID_CHUNK_COMBINATION,
    E_INVALID_HBINFO_TYPE,
    E_INVALID_HBINFO_LEN,
    E_INVALID_CHUNK_LEN,
    E_INVALID_PARAMETER_ID,
    E_INVALID_PARAMETER_LEN,
    E_UNKNOWN_PARAMETER_ID,
    E_UNKNOWN_PARAMETER_RETURN_ERROR,
    E_UNKNOWN_PARAMETER_INIT_RETURN_ERROR,
    E_UNKNOWN_PARAMETER_INITACK_RETURN_ERROR,
    E_SKIP_PARAMETER,
    E_SKIP_PARAMETER_RETURN_ERROR,
    E_INVALID_CHUNK_SIZE,
    E_INVALID_CHUNK_PARAMETER,
    E_UNSUPPORTED_ADDR,
    E_INVALID_ADDR,
    E_INVALID_NUM_OPTIONAL_PARAMS,
    E_INVALID_FIXED_PARAMETERS,
    E_MALLOC_FAILED,
    E_INIT_QUEUES,
    E_ESHELL_INIT_FAILED,
    E_INVALID_EVENT_FOR_STATE,
    E_INVALID_SACK_CHUNK,
    E_PMTU_DATABASE_OVERFLOW,
    E_SCTP_ERROR_DISABLED,
    E_SCTP_INVALID_ERROR_FLAG,
    E_SCTP_INVALID_ERROR_LEVEL,
    E_API_FAILURE,
    E_INVALID_TSN,
    E_DUPLICATE_TSN,
    E_MISSING_PARAMS,
    E_NAME_RES_FAILED,
    E_BUFFER_TOO_SMALL,
    E_UNKNOWN_CHUNK_ID,
    E_UNKNOWN_CHUNK_RETURN_ERROR,
    E_SKIP_CHUNK,
    E_SKIP_CHUNK_RETURN_ERROR,
    E_COOKIE_SENT_TO_RESOLVER,
    E_INVALID_PORT_NUMBER,
    E_INVALID_SCTP_PORT,
    E_SU_ALREADY_REGISTRED,
    E_INV_SU_ID,
    E_ASSOC_DELETED,
    E_SYSCALL_FAILURE,
    E_DB_GET_FREELIST_FAILED,
    E_DB_NO_FREE_ASSOCIATION,
    E_INTERNAL_ERROR,
    E_TMP_ECODE_FOR_SACK,
    E_UNSUPP_ADDR_TYPE,
    E_DB_NO_FREE_ENDPOINT,
    E_ADDR_TYPE_MISMATCH,
    E_FRAGMENTATION,
    E_TIMER_INIT_FAILED,
    E_ASSOC_EXISTS,
    E_CLI_STK_IPC_FAILURE,
    E_VERIFICATION_TAG_MISMATCH,
    E_PMTU_PROCEDURE_DISABLED,
    E_PMTU_CAPABLE_ALREADY_SET,
    E_DB_INV_CONG_LVL_VALUE,
    EINV_RTO_MIN_PARAMS,
    EINV_RTO_MAX_PARAMS,
    EINV_RTO_INIT_PARAMS,
    EINV_RTO_COMBINATION_PARAMS,
    EINV_COOKIE_LIFE_PARAMS,
    EINV_HB_INTERVAL_PARAMS,
    EINV_SACK_FREQUENCY_PARAMS,
    EINV_SACK_PERIOD_PARAMS,
    EINV_MTU_SIZE_PARAMS,
    EINV_PATH_RETRANS_PARAMS,
    EINV_STREAM_PARAMS,
    EINV_ALPHA_BETA_PARAMS,
    EINV_ASSOC_MAX_RETRANS_PARAMS,
    EINV_PATH_MAX_RETRANS_PARAMS,
    EINV_MAX_INIT_RETRANS_PARAMS,
    EINV_CHECKSUM_PARAMS,
    EINV_PMTUCHECKSUM_PARAMS,
    EINV_HB_INFO_LEN,
    TOTAL_ERRORS
} sctp_error_t;

typedef enum
{
    SCTP_SIGNAL_UNKNOWN            = 0,
    SCTP_SIGNAL_SEGMENTATION_FAULT = 1,
    SCTP_SIGNAL_BUS_ERROR,
    SCTP_SIGNAL_SYS_ERROR,
    SCTP_SIGNAL_ILLEGAL_INSTRUCTION
} sctp_signal_et;


typedef    enum
{
    SCTP_STATE_UNINTIALISED   = 0,
    SCTP_STATE_ACTIVE         = 1
} sctp_stack_state_et;

typedef    enum
{
    SCTP_OVER_UDP    = 0,  /* SCTP Tunneled over UDP */
    SCTP_OVER_INET   = 1,  /* SCTP over Raw IPv4 sockets */
    SCTP_OVER_INET6  = 2   /* SCTP over Raw Ipv4 and IPv6 sockets */
} sctp_stack_type_et;

typedef    struct
{
    sctp_Boolean_t      ecn_capable;
    sctp_Boolean_t      pmtu_capable;
    sctp_Boolean_t      nat_capable;
    sctp_U8bit          ack_policy;
    sctp_U8bit          stale_cookie_policy;
    sctp_stack_type_et  stack_type;
} sctp_capability_st;

typedef   enum
{
    SCTP_CONGLEVEL_NOCONG     = 0,
    SCTP_CONGLEVEL_LOW        = 1,
    SCTP_CONGLEVEL_MED        = 2,
    SCTP_CONGLEVEL_MAX        = 3
} sctp_cong_level_et;

typedef struct
{
    sctp_U8bit    high_cong_lvl_onset;
    sctp_U8bit    high_cong_lvl_abate;
    sctp_U8bit    med_cong_lvl_onset;
    sctp_U8bit    med_cong_lvl_abate;
    sctp_U8bit    low_cong_lvl_onset;
    sctp_U8bit    low_cong_lvl_abate;
    sctp_U8bit    no_cong_lvl_onset;
    sctp_U8bit    no_cong_lvl_abate;
} sctp_cong_reg_def_st;

typedef   struct
{
    sctp_U32bit              connection_state;
    sctp_U32bit              num_in_streams;
    sctp_U32bit              num_out_streams;
    sctp_U32bit              curr_receiver_win_size;
    sctp_U32bit              num_data_chnks_awaiting_ack;
    sctp_U32bit              num_data_chnks_awaiting_sent;
    sctp_U32bit              last_acked_TSN;
    sctp_U32bit              last_sent_TSN;
    sctp_U32bit              num_msgs_awaiting_recv;
    sctp_sockaddr_st         primary_dest_addr;
} sctp_status_params_st;

typedef   struct
{
    sctp_U32bit          major_ver;
    sctp_U32bit          minor_ver;
} sctp_ver_info_st;

/* Structures used to initialize the stack */
typedef   struct
{
    /* Contains the features supported by the stack. */
    sctp_capability_st    capability;

    /* Maximum applications supported by the stack */
    sctp_U16bit           max_appl;

    /*
     * Max Endpoints and max TCB.
     * TCP style : max_tcb == max_endpoint
     * UDP style : max_tcb >= max_endpoint
     */
    sctp_U32bit           max_endpoint;
    sctp_U32bit           max_tcb;

    /* Seed for the random number generator. */
    sctp_U32bit           rand_seed;

    /* Local Host Address Configuration */
    sctp_U32bit           no_of_local_addrs;
    sctp_sockaddr_list_st list_of_local_addrs;

    /* Per association Max values */
    struct
    {
        sctp_U32bit       streams;
        sctp_U32bit       min_tx_buffers;
        sctp_U32bit       med_tx_buffers;
        sctp_U32bit       max_tx_buffers;
        sctp_U32bit       min_rx_buffers;
        sctp_U32bit       med_rx_buffers;
        sctp_U32bit       max_rx_buffers;
    } MaxPerAssoc;

    sctp_U32bit          min_reserve_buffers;
    sctp_U32bit          med_reserve_buffers;
    sctp_U32bit          max_reserve_buffers;

    /* Used only in case stack type is UDP: for UDP tunnelling. */
    sctp_U16bit           standard_udp_port;
    sctp_checksum_et      checksum; /*whether to use Adler32 or CRC32*/
    sctp_U8bit            *hostname;
} sctp_init_options_st;

/************* Incoming packet from the network will be stored here *******/
typedef   struct
{
    struct node        __header__;  /* To accomodate the link list pointers */

    /* src & dest address must be present */
    /* for compatibility with UDP */
    sctp_sockaddr_st   dst_addr; /* To address */
    sctp_sockaddr_st   src_addr;  /* From address */
    sctp_U32bit        packet_len;
    sctp_U32bit        CE_bit;
    sctp_U32bit        ip_header_present;
    sctp_U8bit         buffer[SCTP_MAX_MTUSIZE];
} sctp_TL_packet_st;

/************ The packet structure that contains the SCTP packet. *******/
typedef    struct
{
    /* The Source and destination port */
    sctp_port_t       src_port;
    sctp_port_t       dst_port;

    /* The Verification tag */
    sctp_U32bit       v_tag;
    /*checksum function used to verify this packet*/
    sctp_checksum_et  checksum;
    /* The TL packet for this sctp packet. */
    sctp_TL_packet_st *p_tl_packet;
} sctp_packet_st;

/************** chunk specific data types **************************/

typedef   struct
{
    sctp_U8bit          chunk_id;
    sctp_U32bit         chunk_offset;
} sctp_chunk_info_st ;

typedef   struct
{
    sctp_U16bit         parameter_id;
    sctp_U16bit         parameter_len;
    sctp_U32bit         parameter_offset;
} sctp_param_info_st;


/****************** Init/InitAck  fixed parameters ***********************/
typedef   struct
{
    sctp_U32bit           init_tag;
    sctp_U32bit           a_rwnd;
    sctp_U16bit           num_in_streams;
    sctp_U16bit           num_out_streams;
    sctp_U32bit           init_tsn;
} sctp_init_fixed_param_st;


typedef struct __sctp_suppaddr_st
{
    sctp_Boolean_t  v4;
    sctp_Boolean_t  v6;
    sctp_Boolean_t  hname;
} sctp_suppaddr_st;

/******************* Init Optional Parameters *****************************/
typedef   struct
{
    sctp_U16bit       param_type;
    sctp_U16bit       param_len;

    union
    {
        sctp_U32bit      ecn_capable; /* no to be used */
        sctp_U32bit      cookie_preservative;
        sctp_U8bit       hostname[SCTP_MAX_HOSTNAME_SIZE];
        sctp_suppaddr_st supported_addrtypes;
        sctp_U32bit      adaption_ind;
        sctp_sockaddr_st pref_primaddr; /*preffered primary address*/
    } param_data;
} sctp_init_opt_params_st;

/******************** Init Chunk ******************************************/

typedef   struct
{
    sctp_init_fixed_param_st  fixed_params;

    sctp_U32bit               num_addrs;
    sctp_sockaddr_st          addr_list[SCTP_MAX_TRANSPORT_ADDR];

    sctp_U32bit               num_opt_params;
    sctp_init_opt_params_st   opt_params[SCTP_MAX_OPTIONAL_PARAMS];
    sctp_TL_packet_st         *p_tl_packet;
} sctp_chunk_init_st;

/******************* InitAck Optional Parameters ****************/
typedef   struct
{
    sctp_U16bit       param_type;
    sctp_U16bit       param_len;
    union
    {
        sctp_U32bit     ecn_capable;
        sctp_U8bit      hostname[SCTP_MAX_HOSTNAME_SIZE];
        sctp_U32bit     adaption_ind;
        sctp_sockaddr_st pref_primaddr; /*preffered primary address*/
    } param_data;
} sctp_initack_opt_params_st;

/******************** InitAck Chunk **********************************/
typedef   struct
{
    sctp_init_fixed_param_st      fixed_params;
    sctp_U16bit                   cookie_len;
    sctp_U8bit                    *cookie;

    sctp_U32bit                   num_addrs;
    sctp_sockaddr_st              addr_list[SCTP_MAX_TRANSPORT_ADDR];

    sctp_U32bit                   num_opt_params;
    sctp_initack_opt_params_st    opt_params[SCTP_MAX_OPTIONAL_PARAMS];
} sctp_chunk_initack_st;

/******************** Cookie Chunk ***************************************/
typedef   struct
{
    sctp_init_fixed_param_st  fixed_params;

    sctp_U32bit               hostname_present; /*Whether hostname present*/
    sctp_U32bit               num_addrs;
    sctp_sockaddr_st          addr_list[SCTP_MAX_TRANSPORT_ADDR];
    sctp_U8bit                hostname[SCTP_MAX_HOSTNAME_SIZE];
    sctp_U32bit               adaption_ind;
} sctp_init_info_st;

typedef   struct
{
    sctp_U8bit           digest[SCTP_KEYSIZE]; /** MD5 Digest **/
    sctp_time_t          time_stamp;       /** time of cookie generation **/
    sctp_U32bit          life_time;       /** life time in seconds **/
    sctp_U32bit          ecn_capable;       /** ECN capable flag    **/
    sctp_init_info_st    local_init_parameters;  /** local Init parameters **/
    sctp_init_info_st    remote_init_parameters; /** remote Init parameters **/
    sctp_U32bit          local_tie_tag;       /** local tie tag **/
    sctp_U32bit          peer_tie_tag;       /** remote tie tag **/
    sctp_U16bit          this_port;     /** Source port  **/
    sctp_U16bit          dest_port;     /** Destination port **/

    /* Destination Address to which Cookie is sent.
       This is used for Path Verification
       i.e. when we receive COOKIE-ECHO, we will mark
            that_address as confirmed.
    */
    sctp_sockaddr_st     that_address;

    sctp_U16bit           unexpected_init_params_len;
} sctp_chunk_cookie_st;

/******************** Heartbeat Chunk ******************************/

typedef   struct
{
    sctp_U16bit           time_stamp_id;
    sctp_U16bit           time_stamp_len;
    sctp_time_t           time_stamp;
    sctp_U16bit           dest_addr_id;
    sctp_U16bit           dest_addr_len;
    sctp_sockaddr_st      dest_addr;
    sctp_U32bit           src_addr_id;
    sctp_U32bit           nonce[2];
} sctp_chunk_hb_st;

/************************** Sack Chunk ****************************/
typedef   struct
{
    sctp_U32bit          cumtsn;
    sctp_U32bit          arwnd;
    sctp_U16bit          numfrags;
    sctp_U8bit           *frags;
    sctp_U16bit          num_dupicate_tsn;
    sctp_U8bit           *dupicate_tsn;
} sctp_chunk_sack_st;


/****************** SACK fragments structure *****************************/
typedef   struct
{
    struct node      __header__;

    sctp_U32bit      first;
    sctp_U32bit      last;
} sctp_sackfrag_st;

/*required in prot params*/
typedef   struct sctp_sndrcvinfo
{
    sctp_stream_t      sinfo_stream;
    sctp_U16bit        sinfo_ssn;
    sctp_U16bit        sinfo_flags;
    sctp_U32bit        sinfo_ppid;
    sctp_U32bit        sinfo_context;
    sctp_U32bit        sinfo_timetolive;
    sctp_U32bit        sinfo_tsn;
    sctp_U32bit        sinfo_assoc_id;
} sctp_sndrcvinfo_st;

#ifdef SCTP_IP_HEADER_PARAMS
typedef struct
{
    sctp_U8bit      ttl;
    sctp_U8bit      tos;
#ifdef SCTP_INET6_SUPPORT
    sctp_U16bit      hoplimit;
    sctp_U16bit      tclass;
#endif

} sctp_ip_header_params_st;
#endif
/******************* sctp protocol params *********************************/
typedef   struct
{
    sctp_U32bit        rto_init;
    sctp_U32bit        rto_min;
    sctp_U32bit        rto_max;
    sctp_U32bit        max_burst;
    sctp_U32bit        valid_cookie_life;
    sctp_U32bit        assoc_max_retrans;
    sctp_U32bit        hb_interval;
    sctp_U32bit        path_max_retrans;
    sctp_U32bit        max_init_retrans;
    sctp_U32bit        max_init_path_retrans;
    sctp_U32bit        init_timeo;
    sctp_U32bit        bundling_time;
    sctp_U32bit        sack_period; /*** SACK_PERIOD ********/
    sctp_U32bit        sack_frequency; /*** SACK_FREQUENCY ********/
    sctp_U32bit        adaption_ind; /*adaptation layer indication*/
    sctp_U32bit        maxseg; /*maximum segment size*/
    sctp_U16bit        hb_info_length; /**** To set variable length HB****/
    sctp_U16bit        mtu_size;
    sctp_U16bit        rto_alpha_percent;
    sctp_U16bit        rto_beta_percent;
    sctp_U16bit        num_in_streams;
    sctp_U16bit        num_out_streams;
    sctp_Boolean_t     re_use_addr;
    sctp_Boolean_t     dont_fragment;
    sctp_Boolean_t     want_mapped;
    sctp_Boolean_t     nat_capable;
    sctp_Boolean_t     pmtu_capable;
    sctp_Boolean_t     allow_asconf;
    sctp_checksum_et   checksum_func;
    sctp_U32bit        ppi;
    sctp_U32bit        hb_max_burst;

#ifdef SCTP_IP_HEADER_PARAMS
    sctp_ip_header_params_st   ip_header_params;
#endif
#ifdef SCTP_UI_SOCK_API
    sctp_Boolean_t     sndrcvinfo_valid;
    sctp_U8bit         strm_timeout[SCTP_MAX_NUMSTREAM];
    struct sctp_sndrcvinfo  def_sndrcvinfo;
#endif
} sctp_prot_params_st;

typedef   enum
{
    RTO_INIT           = 1,
    RTO_MIN,
    RTO_MAX,
    RTO_ALPHA_PERCENT,
    RTO_BETA_PERCENT,   /* = 5*/
    VALID_COOKIE_LIFE,
    ASSOC_MAX_RETRANS,
    PATH_MAX_RETRANS,
    MAX_INIT_RETRANS,
    NUM_IN_STREAMS,
    NUM_OUT_STREAMS,    /* = 11*/
    NO_CONGESTION_LEVEL,
    LOW_CONGESTION_LEVEL,
    HIGH_CONGESTION_LEVEL,
    HB_INTERVAL,
    RE_USE_ADDR,        /* = 16*/
    /*SPR 4798*/
    BUNDLING_TIME,
    /*SPR 4798*/
    /*SPR 4800*/
    ESHELL_SELECT_TIME,
    /*SPR 4800*/
    SNDBUF,
    RCVBUF,             /* = 20*/
    ADAPTION_IND,
    DONT_FRAGMENT,
    SUPPORT_HOSTNAMES,
    WANT_MAPPED_ADDR,
    NAT_CAPABLE,
    CHECKSUM_FUNCTION,
    ALLOW_ASCONF,
    MAX_INIT_TIMEO,
    SACK_PERIOD,
    SACK_FREQUENCY,
    PMTU_CAPABLE,
    MTU_SIZE,
    HB_INFO_LENGTH,
    PAYLOAD_PROTOCOL_ID,
    HB_MAX_BURST
    /* Changes done for CSR#1-4948130 */
#ifdef SCTP_IP_HEADER_PARAMS
    ,
    IP_CONF_TTL,    /* =36*/
    IP_CONF_TOS
#ifdef SCTP_INET6_SUPPORT
    ,
    HOP_LIMIT,
    TRAFFIC_CLASS
#endif
#endif
    /* Changes done for CSR#1-4948130 Ends*/

} sctp_prot_params_et;

#define       SCTP_MAX_NUM_ACKPOLICY       2
#define       SCTP_MAX_NUM_COOKOEPOLICY    2


typedef enum
{
    /*The values are in accordance with the draft*/
    SCTP_ASCONF_PARAM_ADD_IP        = 0xC001,
    SCTP_ASCONF_PARAM_DEL_IP        = 0xC002,
    SCTP_ASCONF_PARAM_SET_PRIM      = 0xC004
} sctp_asconf_param_type_et;


#ifdef SCTP_DYNAMIC_IP_SUPPORT

typedef struct
{
    sctp_asconf_param_type_et   param_type;
    sctp_U32bit                 corr_id;
    sctp_sockaddr_st            ipaddr;
} sctp_asconf_addip_param_st;


typedef sctp_asconf_addip_param_st sctp_asconf_delip_param_st;
typedef sctp_asconf_addip_param_st sctp_asconf_setprim_param_st;

typedef struct
{
    sctp_asconf_param_type_et   param_type;
    sctp_U32bit                 ind_value;
} sctp_asconf_adaption_param_st;

typedef union
{
    sctp_asconf_param_type_et       param_type;
    sctp_asconf_addip_param_st      add_ip;
    sctp_asconf_delip_param_st      del_ip;
    sctp_asconf_setprim_param_st    set_primary;
} sctp_asconf_param_ut;

typedef struct
{
    sctp_U32bit             serial_no;
    sctp_sockaddr_st        src_addr;
    sctp_U8bit              *p_unreco_params;
    sctp_U32bit             unreco_param_len;
    sctp_U32bit             num_params; /* Number of ASCONF Parameters */
    sctp_asconf_param_ut    asconf_params[SCTP_MAX_PARAMS_PER_ASCONF];
} sctp_addr_conf_st;


typedef enum
{
    /*The values are in accordance with the draft*/
    SCTP_ASCONF_PARAM_ERR_CAUSE        = 0xC003,
    SCTP_ASCONF_PARAM_SUCCESS_REPORT   = 0xC005
} sctp_asconf_ack_param_type_et;

typedef struct
{
    sctp_asconf_ack_param_type_et       param_type;
    sctp_U32bit                         corr_id;
    struct __err_cause
    {
        sctp_U16bit            cause_code;
        sctp_U16bit           cause_len;
        sctp_asconf_param_ut  invalid_param;
    }                                   err_cause;
} sctp_asconf_ack_param_st;

typedef struct
{
    sctp_U32bit                 serial_no;
    sctp_U32bit                 num_params; /* Number of ASCONF Parameters */
    sctp_U8bit                  *p_unreco_params;
    sctp_U32bit                 unreco_param_len;
    sctp_sockaddr_st            from_addr;
    sctp_asconf_ack_param_st    asconf_params[SCTP_MAX_PARAMS_PER_ASCONF];
} sctp_addr_conf_ack_st;


typedef enum
{
    SCTP_LOCAL_ADDRESS_ADDED = 0x00,
    SCTP_LOCAL_ADDRESS_REMOVED,
    SCTP_LOCAL_ADDRESS_MADE_PRIM,
    SCTP_LOCAL_ADDRESS_ADD_FAIL = 0x100,
    SCTP_LOCAL_ADDRESS_REM_FAIL,
    SCTP_LOCAL_ADDRESS_MADE_PRIM_FAIL
} sctp_addrconf_status_et;


typedef struct
{
    sctp_Boolean_t  asconf;
    sctp_Boolean_t  addip;
    sctp_Boolean_t  delip;
    sctp_Boolean_t  setprim;
} sctp_restrictions_st;


typedef enum
{
    SCTP_ASCONF_NORESPONSE,
    SCTP_ASCONF_IMPLIED_SUCCESS,
    SCTP_ASCONF_SUCCESS,
    SCTP_ASCONF_EUNKNOWN,
    SCTP_ASCONF_ELASTIP,
    SCTP_ASCONF_ERESOURCE,
    SCTP_ASCONF_ESRCADDR,
    SCTP_ASCONF_UNRECOPARAM
} sctp_asconf_response_et;
#endif




/******************* timer buffer Structure *************************/
typedef   struct
{
    /* SPR 5130 */
    /* pointer should not be stored, insted USE ep_id, assoc_id */

    /* Pointer to the association of this timer */
    struct __sctp_tcb_st  *p_ass;

    sctp_U32bit           ep_id;
    sctp_U32bit           assoc_id;

    /* The timer event type given to FSM */
    sctp_U32bit           event;

    /* Address index: May not be used for all timer types. */
    sctp_U32bit           addr_index;
} sctp_timer_buffer_st;

/********************* Timers ***********************************/
#define   SCTP_NULL_TIMER_ID  0
typedef   enum
{
    SCTP_ASSOCTIMER_INIT        = 0,
    SCTP_ASSOCTIMER_COOKIE      = 1,
    SCTP_ASSOCTIMER_SHUT        = 2,
    SCTP_ASSOCTIMER_LIFETIME    = 3,
    SCTP_ASSOCTIMER_DELAYACK    = 4,
    SCTP_ASSOCTIMER_ECN         = 5,
    SCTP_ASSOCTIMER_HBACK       = 6,
    SCTP_ASSOCTIMER_INACTIVITY  = 7,
    SCTP_ASSOCTIMER_LINGER      = 8,
    SCTP_ASSOCTIMER_AUTOCLOSE   = 9,
#ifdef SCTP_DYNAMIC_IP_SUPPORT
    SCTP_ASSOCTIMER_ASCONF      = 10,
#endif
    /* SPR 6854 */
    /* This timer is started in case a primary path goes down, so as to
     * detect the event when it is up again
     */
    SCTP_ASSOCTIMER_LOCALHBEAT  = 11,
    /* SPR 6854 */
    SCTP_ASSOCTIMER_NUM         /*Must be the last enumeration Value*/
} sctp_assoc_timer_id_et;


typedef   enum
{
    SCTP_DESTTIMER_RTX      = 0,
    SCTP_DESTTIMER_PMTU     = 1,
    SCTP_DESTTIMER_BUNDLING = 2,
    /* This Timer is started for Path Verification
       once the Association is in Established state */
    SCTP_PATH_VERF_TIMER    = 3,
    SCTP_DESTTIMER_NUM      = 4
} sctp_dest_timer_id_et;


/***************** data buffers ************************************/

typedef   struct
{
    struct node         __header__;

    sctp_U32bit         id;

    /* The following values are extracted/inserted to/from the actual data
       chunk */
    sctp_S32bit         size;
    sctp_U32bit         ppi;
    sctp_U32bit         tsn;
    sctp_stream_t       stream;
    sctp_U16bit         ssn;

    sctp_U8bit          unordered;
    sctp_U8bit          begin;
    sctp_U8bit          end;
    sctp_U8bit            read_flag; /* This is for receive partial data */

    sctp_U32bit           lifetime;
    sctp_U32bit           context;
    sctp_U32bit           dest_addr_index;

    sctp_U8bit            sent;       /* Transmit side only -
                                         have we sent this chunk ever */
    sctp_U8bit            missed;     /* Transmit side only how many
                                         times has this fallen into an ack-gap */
    sctp_U8bit            bundle;
    sctp_U8bit            retrans_flag;   /* Retransmission flag */
    sctp_U8bit            ack_flag;       /* this flag denotes state of acknowledgement*/
    sctp_U8bit            *buffer;        /* payload */
    sctp_U32bit           ref_count;      /* the buffer pointer is with the user*/
    sctp_U32bit           buffer_read_index;    /* This mark the position from where
                                                 * the buffer had to be read. This
                                                 * is more of significance in case
                                                 * the application has provided
                                                 * insuficient buffer for reading
                                                 * the mesg.
                                                 */
} sctpbuffer_st;


typedef    struct
{
    struct node       __header__;
    sctp_U8bit          buffer[SCTP_MAX_MTUSIZE];
} sctpmtubuf_st;

/******************* Sack Buffer ********************************************/
typedef    struct
{
    struct node       __header__;

    unsigned int      first;
    unsigned int      last;
} sctpsackbuf_st;

typedef   union
{
    /* the same endpoint structure is used in RESTART option */
    struct
    {
        sctp_U32bit        num_dest_addrs;
        sctp_U16bit        num_in_streams;
        sctp_U16bit        num_out_streams;
        sctp_sockaddr_list_st list_of_addrs;
    } endpoint_up;

    struct
    {
        sctp_U32bit  last_sent_tsn;
        sctp_U32bit  last_acked_tsn;
        sctp_U16bit  cause_len;
        sctp_U8bit   *cause;
    } endpoint_retrievable;

    struct
    {
        sctp_U32bit            num_dest_addrs;
        sctp_U16bit            that_port;
        sctp_sockaddr_list_st  list_of_addrs;
        sctp_U16bit            cause_len;
        sctp_U8bit             *cause;
    } endpoint_down;

#ifdef SCTP_DYNAMIC_IP_SUPPORT
    struct
    {
        sctp_sockaddr_st        src_addr;
        sctp_addrconf_status_et status;
        sctp_asconf_response_et response;
    } endpoint_addr_chng;
#endif
} sctp_endpoint_chg_data_ut;


/************************ Notfication events *******************************/
typedef   enum
{
    SCTPNOTIFY_OPERROR             = 0,
    SCTPNOTIFY_DESTSTATCHANGE      = 1,
    SCTPNOTIFY_ENDSTATCHANGE       = 2,
    SCTPNOTIFY_CONGESTION          = 3,
    SCTPNOTIFY_SYSERROR            = 4,
    SCTPNOTIFY_CONNPENDING         = 5,
    SCTPNOTIFY_DATAPENDING         = 6,
    SCTPNOTIFY_SENDFAILURE         = 7,
    SCTPNOTIFY_PARTIAL_DATAPENDING = 8,
    SCTPNOTIFY_RECVCONGESTION      = 9,
    SCTPNOTIFY_SHUTDOWN_EVENT      = 10,
    SCTPNOTIFY_ADAPTION_IND        = 11,
    SCTPNOTIFY_PEER_DATA_DROP      = 12
} sctp_indication_et ;


/********************* su notification data   ***************************/
typedef  union   sctpnotice_u
{
    struct
    {
        sctp_U32bit         assoc_id;
        sctp_U16bit         err_code;
        sctp_U16bit         err_length;
        sctp_Pvoid_t        sys_err_info;
    } operror;

    struct
    {
        sctp_U32bit            assoc_id;
        sctp_sockaddr_st       destination_ip_addr;
        sctp_sock_spc_event_et status;
    } dststatchange;

    struct
    {
        sctp_U32bit               assoc_id;
        sctp_sock_sac_event_et    status;
        sctp_endpoint_chg_data_ut status_data;
    } endstatchange;

    struct
    {
        sctp_U32bit         assoc_id;
        sctp_cong_level_et  cong_level;
    } congestion;

    struct
    {
        sctp_U32bit        assoc_id;
        sctp_error_t       errorno;
    } syserror;

    struct
    {
        sctp_U32bit        assoc_id;
    } connpending;

    struct
    {
        sctp_U32bit        assoc_id;
        sctp_stream_t      stream;
    } datapending;

    struct
    {
        sctp_U32bit        assoc_id;
        sctp_stream_t      stream;
    } partial_datapending;

    struct
    {
        sctp_U32bit        assoc_id;
        sctp_U32bit        context;
        sctp_error_t       cause_code;
    } sendfailure;

    struct
    {
        sctp_U32bit         assoc_id;
        sctp_cong_level_et  cong_level;
    } recv_congestion;

    struct
    {
        sctp_U32bit        assoc_id;
    } shutdown_event;

    struct
    {
        sctp_U32bit        assoc_id;
        sctp_U32bit        adaption_bits;
    } adaption_ind;

    struct
    {
        sctp_U32bit         assoc_id;
        sctp_error_t        ecode;
        sctp_U32bit         packet_len;
        sctp_U8bit          buffer[SCTP_MAX_MTUSIZE];
    } peer_data_drop;

} sctp_notice_ut;


/*
 * The Callback function type
 */
typedef void
( *sctp_callback_func_t )
( sctp_U32bit      notify_type,
  sctp_notice_ut   *p_notification_data,
  sctp_Pvoid_t     p_user_info );

/* Start SPR 20859 */
/* The IP specific send function ptr table.  SPR 20859*/
typedef void ( *sctp_callback_sendmsg_fpt )(
    sctp_U8bit       *buffer,
    sctp_U32bit      size,
    sctp_Boolean_t   df_flag,
    sctp_sockaddr_st *to,
    sctp_sockaddr_st *from,
    sctp_Boolean_t   is_iovec );
/* End SPR 20859 */

typedef sctp_Boolean_t ( *user_handler_fn_pt )( sctp_signal_et, void * );

#ifdef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
/* CSR#1-4825810 Changes done for warning removal activity
 * for compilation with SUN cc */
typedef sctp_void_t ( *sctp_signal_hnd_fn_pt )( int );
#endif
/******************* Event Structure *************************************/

typedef   union
{
    struct
    {
        sctp_chunk_init_st     *init_chunk;
    } msg_init;     /* Init Chunk */

    struct
    {
        sctp_chunk_initack_st  *initack_chunk;
        sctp_Boolean_t        send_err_with_cookie;
        sctp_sockaddr_st      from_addr;
        sctp_U32bit           unrecog_param_len;
        sctp_U8bit            *unrecog_params;
    } msg_initack;   /* Initack Chunk */

    struct
    {
        sctp_chunk_cookie_st   *cookie_chunk;
        sctp_sockaddr_st      to_addr; /* SPR FIX 20406*/
    } msg_cookie;   /* Cookie Chunk */

    struct
    {
        sctp_U32bit           cumtsn;
    } msg_shutdown; /* Shutdown chunk */

    struct
    {
        sctp_chunk_hb_st *hback_chunk;
    } msg_hback;      /* heartbeat    Chunk */

    struct
    {
        sctp_chunk_sack_st    *sack_chunk;
        sctp_sockaddr_st      from_addr;
        sctp_sockaddr_st      to_addr;
        sctp_U32bit           arwnd_valid;
        sctp_U32bit           ver_tag;
        sctp_U32bit           ootb_sack;
    } msg_sack;    /* sack chunk */

    struct
    {
        sctp_Boolean_t        CE_bit;
        sctp_Boolean_t        last_chunk_flag;
        sctp_sockaddr_st      from_addr;
        sctp_U8bit            chunk_flag;
        sctp_U16bit           chunk_len;
        sctp_U8bit            *chunk_data;
        sctp_U32bit           ver_tag;
        sctp_U32bit           ootb_data; /* for out of the  blue datagram */
    } msg_data; /* data chunk */

    struct
    {
        sctp_U32bit           addr_index;
    } evt_timeout; /* any timeout*/

    struct
    {
        sctp_U32bit           addr_index;
        sctp_U16bit           path_mtu;
    } evt_pathmtu; /* new path mtu */

    struct
    {
        sctp_sockaddr_st      from_addr;
        sctp_U32bit           lowest_tsn;
    } msg_ecne;     /* ecn echo chunk */

    struct
    {
        sctp_sockaddr_st      from_addr;
        sctp_U32bit           lowest_tsn;
    } msg_cwr;    /* congestion window reduced chunk */

    struct
    {
        sctp_U32bit           stale_cookie_time;
    } msg_err_stale_cookie; /* stale cookie error chunk */

    struct
    {
        sctp_U32bit           verification_tag;
    } msg_shutack; /* shutdown-ack message */

    struct
    {
        sctp_U8bit           *cause_info;
        sctp_U16bit           cause_length;
    } cmd_abort;


    struct
    {
        sctp_U8bit           *p_chunk;
        sctp_U16bit           chunk_length;
        sctp_U8bit           *p_cause;
        sctp_U16bit           cause_length;
    } msg_abort;

#ifdef SCTP_DYNAMIC_IP_SUPPORT
    sctp_addr_conf_st         msg_addrconf;
    sctp_addr_conf_ack_st     msg_addrconfack;
#endif

} sctp_assoc_event_ut;

/**** For Events from SU Interface Module to Association State Machine ****/
typedef   union
{
    struct
    {
        sctp_U32bit           addr_index;
    } evt_cmduserdata; /* User data request from SU*/
} sctp_su_event_ut;

/***** For Events from SU Interface Module to Association State Machine ****/
typedef   union
{
    struct
    {
        sctp_U8bit            *buffer_offset;
    } evt_cmduserdata; /* User data request from SU*/
} sctp_rx_event_ut;


/**************** Partial Data info Structure *****************************/
typedef   struct
{
    sctp_U32bit        partial_data_pending;
    sctp_stream_t      stream;
    sctp_U16bit        ssn;
    sctp_U32bit        delivery_number;
    sctp_U32bit        data_assembly_in_progress;
    sctp_U32bit        expected_tsn;
    sctp_U32bit        unordered;
} sctp_partial_data_st;

/******************* ECN Info Structure  ************************************/
typedef   struct
{
    sctp_U32bit           ecn_capable;
    sctp_U32bit           ecn_sent;
    sctp_U32bit           last_recv_lowest_tsn;
    sctp_U32bit           last_sent_lowest_tsn;
} sctp_ecn_info_st;


/******************* Remote Address Structure ******************************/
typedef   struct
{
    sctp_sockaddr_st      that_address;
    sctp_U32bit           src_addr_index; /* Active Source Address */

    sctp_time_t           last_used_time; /* Last Used Time */
    sctp_U32bit           state;

    sctp_U32bit           status;

    sctp_timer_t          dest_timer_id[SCTP_DESTTIMER_NUM];

    sctp_U32bit           allow_heartbeat;
    sctp_U32bit           path_error_count;
    sctp_U32bit           path_error_threshold;

    sctp_U32bit           partial_bytes_acked;
    sctp_U32bit           lowest_tsn;
    sctp_U32bit           num_chunks_await_ack;
    sctp_U32bit           cwnd;
    sctp_U32bit           ssthresh;
    sctp_U32bit           outstanding;
    sctp_U32bit           wait_bundle;
    /* Start SPR 20526  */
    sctp_U32bit           min_rtt;
    sctp_U32bit           max_rtt;
    sctp_U32bit           last_recorded_rtt;
    /* End SPR 20526  */
    sctp_U32bit           rto;
    sctp_U32bit           srtt;
    sctp_U32bit           rttvar;
    sctp_U32bit           count;
    sctp_U16bit           path_mtu;
    /* Changes For CSR#1-5037386 */
    sctp_Boolean_t        prefered_sack_dest;
    sctp_U32bit           unsent_packet_count;
    sctp_U32bit           retrans_packet_count;
    /* Changes For CSR#1-5037386 Ends */

    /* is_confirmed signifies whether the Destination Transport Addres is confirmed or not. */
    sctp_Boolean_t        is_confirmed;
    sctp_U32bit           nonce[2]; /* this is a random 64 bit number used for path verification purpose */

    /* The Address which are marked Inactive, it should not be considered for Path Verification Probing
       using the HB.max.burst.
       perf_path_verf == SCTP_TRUE means that Path should be considered for Path Verification
       perf_path_verf == SCTP_FALSE means that Path is INACTIVE and should not be considered for
       Path Verification. Although Normal HeartBeats will be sent to it regularly.
    */
    sctp_Boolean_t        perf_path_verf;

} sctp_remote_addr_param_st;


/******************* Assoc Transmit Structure ******************************/
typedef  struct
{
    /* Congestion notification to SU */
    sctp_U32bit          congestion_event_sent;
    sctp_U32bit          tsn;
    sctp_U32bit          rwndthere;
    sctp_U32bit          total_outstanding;
    sctp_U16bit          lowest_path_mtu;
    sctp_U16bit          num_out_streams;         /*  number of out steams */
    sctp_U16bit          ssn[SCTP_MAX_NUMSTREAM]; /* SSN value to be sent */

    LIST                 transmit_queue;     /* transmit Queue */

    sctp_U32bit          src_addr_index; /* set to the index on which last data packet has arrived */

    sctp_U16bit          that_port;
    sctp_U32bit          num_remote_address;
    sctp_U32bit          configured_primary_path_index;
    sctp_U32bit          primary_address_index;

    sctp_U8bit           rem_hostname[SCTP_MAX_HOSTNAME_SIZE];
    sctp_remote_addr_param_st remote[SCTP_MAX_TRANSPORT_ADDR];
#ifdef SCTP_DYNAMIC_IP_SUPPORT
    sctp_U32bit          serial_no;     /*Serial no in ASCONF*/
#endif
    /*5036*/
    sctp_U32bit         wnd_size;
    /*5036*/

    /* Changes For CSR#1-5037386 */
    /* CHANGES FOR CONGESTION DEADLOCK RECOVERY WORKAROUND */
    sctp_U32bit         last_txq_tsn;
    sctp_U32bit         non_resp_thresh;
    /* CHANGES FOR CONGESTION DEADLOCK RECOVERY WORKAROUND ENDS*/
    sctp_U32bit         default_sack_dest_index;
    /* Changes For CSR#1-5037386 Ends */

    sctp_U32bit         fast_recovery_exit;
    sctp_U32bit         highest_outstanding;
    sctp_U32bit         highest_tsn_sent;

} sctp_transmit_st;

/******************* Assoc Receive Structure ********************************/
typedef    struct
{
    sctp_U32bit          first_data_flag;
    sctp_U32bit          congestion_event_sent;
    sctp_U32bit          state;        /* receive state */
    sctp_U32bit          cumtsn;        /* cumulative tsn received */
    sctp_Boolean_t       overrule_delayack;
    sctp_U32bit          duplicate_tsn_list[SCTP_MAXDUPLICATE_TSNS];
    sctp_U16bit          number_duplicate_tsn;

    LIST                 sacklist;
    LIST                 sackfreelist;

    /*sack fragments array */
    sctp_sackfrag_st     sackbuf[SCTP_MAX_NUMSACKFRAGS];

    sctp_U16bit          num_in_streams;       /* number incoming streams */

    struct
    {
        sctp_U16bit     nextssn;              /* next ssn value expected */
        sctp_U16bit     nextssn_notify;       /* next ssn value to notify */
        LIST            receive_queue;        /* receive queue */
    } stream[SCTP_MAX_NUMSTREAM];
#ifdef SCTP_DYNAMIC_IP_SUPPORT
    sctp_U32bit         serial_no;         /*Peer serial Number expected*/
#endif
    /*5036*/
    sctp_U32bit         wnd_size;
    /*5036*/
} sctp_receive_st;

/******************* Buffer list ******************************************/
typedef    struct
{
    LIST          list_min;
    LIST          list_med;
    LIST          list_max;
} sctp_buffer_list_st;

typedef    struct
{
    sctp_U16bit        dest_id;
    sctp_U16bit        stack_id;
    sctp_U16bit        application_id;
} sctp_client_info_st;

/****************** Application info structure *****************************/
typedef    struct
{
    sctp_U32bit           open;
    void                  *p_appl_data;
    sctp_U16bit     appl_id; /* SPR Fix 20540 */

#ifdef SCTP_UI_SU_MESSAGEBASED
    sctp_U16bit     su_id;
#else
    sctp_callback_func_t  callback;
#endif

} sctp_appl_info_st;


typedef struct
{
    struct node                 __header__;
    struct __sctp_tcb_st        *p_assoc_ref;
} sctp_verf_list_ref_tcb_st;


typedef struct
{
    sctp_U16bit  length;
    sctp_U8bit  *param_list;
} sctp_unrecog_params_list_t;

/******************* SCTP TCB Structure *********************************/
typedef   struct __sctp_tcb_st
{
    struct node                 __header__;
    sctp_U32bit                 assoc_id;
    struct __sctp_ep_st         *p_ep;

    sctp_U32bit                 open;
    sctp_U32bit                 state;

    /* protocol parameters Specific to the association */
    sctp_prot_params_st         config;

    sctp_cong_reg_def_st        trx_cong_reg_def;
    sctp_cong_reg_def_st        rcv_cong_reg_def;

    /* restart option */
    sctp_restart_options_et     restart_option;
    sctp_partial_data_st        partial_data;

    sctp_U32bit                 init_retry;
    sctp_U32bit                 init_retry_per_path;
    sctp_U32bit                 cookie_retry;
    sctp_U32bit                 shutdown_retry;
    sctp_U32bit                 shutdown_ack_retry;
    sctp_U32bit                 error_count;

    /* SPR 6854 */
    sctp_U32bit                 configured_local_primary_address_index;
    /* SPR 6854 */
    sctp_U32bit                 local_tag;
    sctp_U32bit                 remote_tag;

    sctp_U32bit                 local_virtual_tie_tag;
    sctp_U32bit                 peer_virtual_tie_tag;

    sctp_U32bit                 total_sent;

    sctp_assoc_event_ut         event;
    sctp_su_event_ut            su_event;
    sctp_rx_event_ut            rx_event;

    sctp_notice_ut              notify;

    sctp_U16bit                 cookie_size;
    /* We should not do this : we should take a buffer from the tx free list
       and store the cookie in that and when the connection is established we
       shall return the buffer to the transmit free list. The tcb structure
       shall just store the pointer to the buffer.*/
    LIST                        cookie_list;

    sctp_timer_t                assoc_timer_id[SCTP_ASSOCTIMER_NUM];

    /*    ecn info structure */
    sctp_ecn_info_st            ecn;

    sctp_transmit_st            transmit;
    sctp_receive_st             receive;
    sctp_buffer_list_st         tx_freelist;
    sctp_buffer_list_st         rx_freelist;

#ifdef SCTP_STATS_ENABLED
    sctp_assoc_stats_st         stats;
#endif
#ifdef SCTP_DYNAMIC_IP_SUPPORT
    sctp_addr_conf_st           cmd_asconf;
    sctp_U8bit                  asconf_response[SCTP_MAX_PARAMS_PER_ASCONF];
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    sctp_restrictions_st        restrict;
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    sctp_restrictions_st        sctp_restrict;
#endif
    sctp_U32bit                 buf_len;
    sctp_U8bit                  asconf_buffer[SCTP_MAX_ASCONF_MSG_SIZE];
#endif

    sctpmtubuf_st               mtu_buffer;
    sctp_U8bit                  fsm_event;

    sctp_Boolean_t              zero_win_prob_on;/*whether 0-window probing
                                                   is on or not */

    /* This will be TRUE when the Connection is Accepted by the ULP */
    sctp_Boolean_t              conn_accepted;

    /* This will contain the Unexpected parameters received in
     * INIT. Used for sending Error Chunk Bundled with CookieAck.
     */
    sctp_unrecog_params_list_t   unrecognized_params;

} sctp_tcb_st;

typedef struct
{
    sctp_U32bit              min_num_frag;
    sctp_U32bit              med_num_frag;
    sctp_U32bit              max_num_frag;
} sctp_count_frags_st;

typedef   enum
{
    SCTP_SO_RTOINFO = 0,
    SCTP_SO_ASSOCINFO,  /* = 1*/
    SCTP_SO_INITMSG,    /* = 2*/
    SCTP_SO_LINGER,
    SCTP_SO_NODELAY,
    SCTP_SO_RCVBUF,
    SCTP_SO_SNDBUF,     /* = 6*/
    SCTP_SO_AUTOCLOSE,
    SCTP_SO_PRIMARY_ADDR,
    SCTP_SO_PEER_PRIMARY_ADDR,
    SCTP_SO_STATUS,     /* = 10*/
    SCTP_SO_NONBLOCKING,
    SCTP_SO_REUSEADDR,
    SCTP_SO_SET_EVENTS,
    SCTP_SO_SET_ADAPTION_LAYER,     /* = 14*/
    SCTP_SO_SET_STREAM_TIMEOUTS,
    SCTP_SO_DISABLE_FRAGMENTS,      /* = 16*/
    SCTP_SO_DEFAULT_SEND_PARAM,
    SCTP_PEER_ADDR_PARAMS,
    SCTP_GET_PEER_ADDR_PARAMS,
    SCTP_GET_PEER_ADDR_INFO,         /* = 20*/
    SCTP_MAXSEG,
    SCTP_I_WANT_MAPPED_V4_ADDR
    /* Bug Id 5 changes Start */
#ifdef SCTP_IP_HEADER_PARAMS
    ,
    SCTP_IP_DSCP
#endif
    /* Bug Id 5 changes End*/
} sctp_sock_option_et;

#define SCTP_VALID_OPT_INFO_TYPE(type) \
    ((SCTP_SO_RTOINFO <= type) && (SCTP_GET_PEER_ADDR_INFO >= type))

typedef   enum
{
    SCTP_MSG_UNORDERED = 0x100,
    SCTP_MSG_ADDR_OVER = 0x200,
    SCTP_MSG_ABORT     = 0x400,
    SCTP_MSG_EOF       = 0x800,
    SCTP_MSG_SENDALL   = 0x010,
    SCTP_MSG_NOTIFICATION = 0x100
} sctp_sndrcvmsg_flag_et;


typedef   enum
{
    SCTP_BINDX_ADD_ADDR = 0,
    SCTP_BINDX_REM_ADDR
} sctp_bindx_flag_et;
#define SCTP_VALID_BINDX_FLAGS(flag) \
    ((SCTP_BINDX_ADD_ADDR == flag) || (SCTP_BINDX_REM_ADDR == flag))


typedef    struct
{
    sctp_U32bit        sinit_num_ostreams;
    sctp_U32bit        sinit_max_instreams;
    sctp_U32bit        sinit_max_attempts;
    sctp_U32bit        sinit_max_init_timeo;
}  sctp_initmsg_st;

typedef    struct
{
    sctp_U32bit        srto_assoc_id;
    sctp_U32bit        srto_initial;
    sctp_U32bit        srto_max;
    sctp_U32bit        srto_min;
} sctp_rtoinfo_st;

typedef struct
{
    sctp_assoc_t     spinfo_assoc_id;
    sctp_sockaddr_storage_st spinfo_address;
    sctp_S32bit      spinfo_state;
    sctp_U32bit      spinfo_cwnd;
    sctp_U32bit      spinfo_srtt;
    sctp_U32bit      spinfo_rto;
    sctp_U32bit      spinfo_mtu;
} sctp_paddrinfo_st;
/* Bug Id 5 changes starts*/
#ifdef SCTP_IP_HEADER_PARAMS
typedef struct
{
    sctp_assoc_t    sip_assoc_id;
    sctp_U8bit      sip_tos;
#ifdef SCTP_INET6_SUPPORT
    sctp_U16bit      sip_hoplimit;
    sctp_U16bit      sip_tclass;
#endif
} sctp_assoc_ip_hdr_params_st;
#endif
/* Bug Id 5  changes ends*/
typedef enum
{
    SCTP_CLOSED = 0,
    SCTP_BOUND,
    SCTP_LISTEN,
    SCTP_COOKIE_WAIT,
    SCTP_COOKIE_ECHOED,
    SCTP_ESTABLISHED,
    SCTP_SHUTDOWN_PENDING,
    SCTP_SHUTDOWN_SENT,
    SCTP_SHUTDOWN_RECEIVED,
    SCTP_SHUTDOWN_ACK_SENT
} sctp_status_state_et;

typedef    struct
{
    sctp_U32bit        sstat_assoc_id;
    sctp_U32bit        sstat_state;
    sctp_U32bit        sstat_rwnd;
    sctp_U32bit        sstat_unackdata;
    sctp_U32bit        sstat_penddata;
    /*04*/
    sctp_U16bit        sstat_instrms;
    sctp_U16bit        sstat_outstrms;
    sctp_U32bit        sstat_fragmentation_point;
    /*04*/
    sctp_paddrinfo_st  sstat_primary;
} sctp_status_st;

/*modified for socket draft 04*/
typedef struct
{
    sctp_assoc_t       sasoc_assoc_id;
    sctp_U16bit        sasoc_asocmaxrxt;
    sctp_U16bit        sasoc_number_peer_destinations;
    sctp_U32bit        sasoc_peer_rwnd;
    sctp_U32bit        sasoc_local_rwnd;
    sctp_U32bit        sasoc_cookie_life;
} sctp_assocparams_st;

typedef struct
{
    sctp_assoc_t       ssp_assoc_id;
    sctp_sockaddr_storage_st ssp_addr;
} sctp_setprim_st;

typedef struct
{
    sctp_assoc_t       sspp_assoc_id;
    sctp_sockaddr_storage_st sspp_addr;
} sctp_setpeerprim_st;

typedef struct sctp_setadaption
{
    sctp_U32bit   ssb_adaption_ind;
} sctp_setadaption_st;

#ifdef SCTP_UNUSED
typedef struct sctp_setstrm_timeout
{
    sctp_assoc_t        ssto_assoc_id;
    sctp_U32bit         ssto_timeout;
    sctp_U16bit         ssto_streamid_start;
    sctp_U16bit         ssto_streamid_end;
} sctp_setstrm_timeout_st;
#endif

typedef struct sctp_paddrparams
{
    sctp_assoc_t               spp_assoc_id;
    sctp_sockaddr_storage_st   spp_address;
    sctp_U32bit                spp_hbinterval;
    sctp_U16bit                spp_pathmaxrxt;
} sctp_paddrparams_st;

typedef struct sctp_event_subscribe
{
    sctp_U8bit sctp_data_io_event;
    sctp_U8bit sctp_association_event;
    sctp_U8bit sctp_address_event;
    sctp_U8bit sctp_send_failure_event;
    sctp_U8bit sctp_peer_error_event;
    sctp_U8bit sctp_shutdown_event;
    sctp_U8bit sctp_partial_delivery_event;
    sctp_U8bit sctp_adaption_layer_event;
} sctp_event_subscribe_st;



typedef    struct
{
    sctp_lock_t        lock;
    LIST               chain;
} sctp_hash_st;

#define SCTP_MAX_NOTF_DATA      50

typedef    struct
{
    sctp_U16bit            sac_type;
    sctp_U16bit            sac_flags;
    sctp_U32bit            sac_length;
    sctp_U16bit            sac_state;
    sctp_U16bit            sac_error;
    sctp_U16bit            sac_outbound_streams;
    sctp_U16bit            sac_inbound_streams;
    sctp_assoc_t           sac_assoc_id;
    sctp_U8bit             sac_info[SCTP_MAX_NOTF_DATA];
} sctp_assoc_change_st;

typedef    struct
{
    sctp_U16bit                  spc_type;
    sctp_U16bit                  spc_flags;
    sctp_U32bit                  spc_length;
    sctp_sockaddr_storage_st     spc_aaddr;
    sctp_U16bit                  spc_state;
    sctp_U16bit                  spc_error;
    sctp_assoc_t                 spc_assoc_id;
} sctp_paddr_change_st;

typedef    struct
{
    sctp_U16bit            sre_type;
    sctp_U16bit            sre_flags;
    sctp_U32bit            sre_length;
    sctp_U16bit            sre_error;
    sctp_U32bit            sre_assoc_id;
    sctp_U8bit             sre_data[SCTP_MAX_NOTF_DATA];
} sctp_remote_error_st;

typedef    struct
{
    sctp_U16bit            ssf_type;
    sctp_U16bit            ssf_flags;
    sctp_U32bit            ssf_length;
    sctp_U32bit            ssf_error;
    sctp_sndrcvinfo_st     ssf_info;
    sctp_U32bit            ssf_assoc_id;
    sctp_U8bit             ssf_data[SCTP_MAX_NOTF_DATA];
} sctp_send_failed_st;


typedef    struct
{
    sctp_U16bit            sse_type;
    sctp_U16bit            sse_flags;
    sctp_U32bit            sse_length;
    sctp_U32bit            sse_assoc_id;
} sctp_shutdown_event_st;

/*added for socket draft 04*/
typedef struct sctp_adaption_event
{
    sctp_U16bit sai_type;
    sctp_U16bit sai_flags;
    sctp_U32bit sai_length;
    sctp_U32bit sai_adaptation_bits;
    sctp_assoc_t sai_assoc_id;
} sctp_adaption_event_st;

typedef struct sctp_pdapi_event
{
    sctp_U16bit pdapi_type;
    sctp_U16bit pdapi_flags;
    sctp_U32bit pdapi_length;
    sctp_U32bit pdapi_indication;
    sctp_assoc_t pdapi_assoc_id;
} sctp_pdapi_event_st;

typedef   struct
{
    sctp_U32bit            l_onoff;
    sctp_S32bit            l_linger;
} sctp_linger_st;

#ifdef SCTP_UI_SOCK_API

typedef union sctp_notification
{
    struct
    {
        sctp_U16bit sn_type;             /* Notification type. */
        sctp_U16bit sn_flags;
        sctp_U32bit sn_length;
    } sn_header;
    sctp_assoc_change_st   sn_assoc_change;
    sctp_paddr_change_st   sn_padr_change;
    sctp_remote_error_st   sn_remote_error;
    sctp_send_failed_st    sn_send_failed;
    sctp_shutdown_event_st sn_shutdown_event;
    /* added */
    sctp_adaption_event_st  sn_adaption_event;
    sctp_pdapi_event_st sn_rcv_pdapi_event;
} sctp_notification_ut;


typedef   struct
{
    struct node          __header__;
    sctp_tcb_st          *p_assoc;
    sctp_U32bit          n_ind;
} sctp_data_indication_st;

typedef   struct
{
    struct node                __header__;
    sctp_U32bit                assoc_id;
    sctp_notification_ut       notification;
    sctp_sockaddr_storage_st   peer_addr;
} sctp_ulpqueue_node_st;

typedef   struct
{
    /* The Endpoint that owns this ULP queue */
    struct __sctp_ep_st  *p_ep;

    /* Notification flag: Describes which notifications are enabled. */
    sctp_U32bit          ntfy_flags;

    /* Number of Indications */
    sctp_U32bit          n_ind;
    LIST                 ntfy_queue;

    /* List for data indications */
    LIST                 data_ind_list;

} sctp_ulpqueue_st;

#endif  /* SCTP_UI_SOCK_API */


/******************* SCTP Socket Structure ******************************/
typedef   struct __sctp_ep_st
{
    struct node          __header__;

    sctp_lock_t          lock; /* Mutex */
    sctp_cond_t          cond; /* Conditional variable */
#ifdef SCTP_DYNAMIC_IP_SUPPORT
    sctp_cond_t          asconf_cond; /*block for asconf message */
#endif
    sctp_U32bit          ep_id;
    sctp_U32bit          server_ep_id;
    sctp_U32bit          num_created_ep;
    sctp_U32bit          created_ep_id[SCTP_MAX_LISTEN_BACKLOG];
    /* The Payload protocol ID */
    sctp_U32bit          ppi;

    /* protocol parameters */
    sctp_prot_params_st  sk_config;

    sctp_cong_reg_def_st        trx_cong_reg_def;
    sctp_cong_reg_def_st        rcv_cong_reg_def;

    sctp_U16bit          n_assoc;
    sctp_U16bit          last_used_assoc;
    LIST                 assoc_list;/* List of associations (sctp_tcb_st> */

    sctp_U16bit          this_port;   /* The Source port */
    sctp_U16bit          naddr;
    sctp_sockaddr_st     this_address[SCTP_MAX_TRANSPORT_ADDR];
    sctp_U8bit           hostname[SCTP_MAX_HOSTNAME_SIZE];

    sctp_U32bit          l_backlog; /* Listener Backlog */
    sctp_U32bit          flags;     /* Socket specific Flags */
    sctp_U8bit           trc_flags; /* Trace flags and Misc flags */

    sctp_U16bit          appl_id;     /* Application Id */

    sctp_sa_family_t     family;    /* Address Family */
    sctp_U8bit           sk_type;   /* Socket Type */

    sctp_linger_st       so_linger;

#ifdef SCTP_UI_SOCK_API

    /* Handles indications to the ULP. */
    sctp_ulpqueue_st     ulpqueue;

    /* Used in the select API to indicate that the endpoint is selected */
    sctp_U32bit          selected;

    /* Number of seconds of AUTO CLOSE timeout. */
    sctp_U32bit          aclose_time;

#endif /* SCTP_UI_SOCK_API */
    /* We're dying: let the user read: but he cannot write */
    sctp_U32bit          moribund;
    sctp_Boolean_t       hostname_supported;
    sctp_Boolean_t       create_from_server;
    sctp_U8bit           cong_flag; /* Congestion notification flag */
    sctp_callback_sendmsg_fpt send_func; /* Callback send Function SPR 20859*/
} sctp_ep_st;

/************ server created info structure ************/
typedef struct
{
    sctp_U16bit          this_port;   /* The Source port */
    sctp_U16bit          naddr;
    sctp_sockaddr_st     this_address[SCTP_MAX_TRANSPORT_ADDR];
} server_created_addr_st;
/*************************Redundancy module structures*****************/

/**********************For stack database*******************************/
typedef    struct
{
    sctp_U32bit                max_appl;
    sctp_U32bit                num_active_appl;

    sctp_U32bit                max_ep;
    sctp_U32bit                num_active_ep;
    sctp_U32bit                last_allocated_ep;

    sctp_U32bit                max_assoc;

    sctp_U32bit                 min_tx_buffers;
    sctp_U32bit                 med_tx_buffers;
    sctp_U32bit                 max_tx_buffers;
    sctp_U32bit                 min_rx_buffers;
    sctp_U32bit                 med_rx_buffers;
    sctp_U32bit                 max_rx_buffers;
    sctp_U32bit              min_reserve_buffers;
    sctp_U32bit              med_reserve_buffers;
    sctp_U32bit              max_reserve_buffers;

    sctp_U32bit                max_streams_per_assoc;
    sctp_U32bit                num_local_addrs;
    sctp_U32bit                active_ep_list[SCTP_MAX_TOTAL_ASSOC];

    sctp_U32bit                pmtu_timer_rem_time;
    sctp_U32bit                num_active_pmtu_nodes;

    struct
    {
        sctp_time_t         time_stamp;
        sctp_sockaddr_st    addr;
        sctp_U16bit         mtu_size;
        sctp_U16bit         spare;
    } pmtu_database[SCTP_MAX_PMTU_NODES];

#ifdef SCTP_ERROR_ENABLED
    sctp_error_level_et         sctp_error_table[SCTP_MAX_ERROR_FLAG + 1];
#endif

#ifdef SCTP_STATS_ENABLED

    sctp_stats_state_et                  sctp_stats_state;
    sctp_proto_internal_stats_st         sctp_protocol_stats;
    sctp_global_traf_stats_st            sctp_global_traffic_stats;
    sctp_error_stats_st                  sctp_error_stats;
    sctp_Boolean_t                       sctp_stats_table[SCTP_ALL_STATS];

#endif

    sctp_appl_info_st          active_appl_list[SCTP_MAX_TOTAL_APP];

    sctp_sockaddr_st           addr_list[SCTP_MAX_TRANSPORT_ADDR];
    sctp_prot_params_st        sctp_def_prot_params; /* default prot params */
    sctp_cong_reg_def_st     trx_cong_reg_def;
    sctp_cong_reg_def_st     rcv_cong_reg_def;
    sctp_capability_st         sctp_stack_init_parameters;

    sctp_U16bit                init_in_streams; /* num in stream at init */
    sctp_U16bit                init_out_streams;/* num out stream at init */
    sctp_U16bit                standard_udp_port;


#ifdef SCTP_ERROR_ENABLED
    sctp_U8bit                 sctp_error_state;
#endif


#ifdef SCTP_DEBUG_ENABLED
    sctp_U8bit                 sctp_gbl_trc;

#endif

    /*Old mac keys*/
    sctp_U8bit                 old_mac_key[SCTP_KEYSIZE];

    /*New mac keys*/
    sctp_U8bit                 current_mac_key[SCTP_KEYSIZE];

    /*
     *  No need to pass hostname as it has already been resolved and
     *  addresses stored in addr_list
     */

} sctp_red_stack_db_st;

/***********************For association database************************/


typedef struct
{
    /* Congestion notification to SU */
    sctp_U32bit          congestion_event_sent;
    sctp_U32bit          tsn;
    sctp_U32bit          rwndthere;
    sctp_U32bit          total_outstanding;
    sctp_U16bit          lowest_path_mtu;
    sctp_U16bit          num_out_streams;         /* number of out steams */
    sctp_U16bit          ssn[SCTP_MAX_NUMSTREAM]; /* SSN value to be sent */

    sctp_U32bit        src_addr_index;
    /* This field has been redefined in SCTP 6.1.5 for tracking
                      * source address index while sending sacks. Please note that
                          * a feild with same name is also part of stucture type of remote
                      * field.*/
    sctp_U16bit          that_port;
    sctp_U32bit          num_remote_address;
    sctp_U32bit          configured_primary_path_index;
    sctp_U32bit          primary_address_index;

    sctp_U32bit          num_tx_buf;

    sctp_sockaddr_st     this_address[SCTP_MAX_TRANSPORT_ADDR];
    sctp_U8bit           rem_hostname[SCTP_MAX_HOSTNAME_SIZE];

#ifdef SCTP_DYNAMIC_IP_SUPPORT
    sctp_U32bit          serial_no;     /*Serial no in ASCONF*/
#endif
    sctp_U32bit         wnd_size;

    sctp_U32bit         last_txq_tsn;
    sctp_U32bit         non_resp_thresh;

    sctp_U32bit         default_sack_dest_index;

    sctp_U32bit         fast_recovery_exit;
    sctp_U32bit         highest_outstanding;
    sctp_U32bit         highest_tsn_sent;

} sctp_red_transmit_st;


typedef   struct
{
    sctp_U32bit          first_data_flag;
    sctp_U32bit          congestion_event_sent;
    sctp_U32bit          state;        /* receive state */
    sctp_U32bit          cumtsn;        /* cumulative tsn received */
    sctp_Boolean_t       overrule_delayack;
    sctp_U32bit          duplicate_tsn_list[SCTP_MAXDUPLICATE_TSNS];
    sctp_U16bit          number_duplicate_tsn;

    sctp_U32bit          sacknum;      /*Number of sack buffers*/
    sctp_U16bit          num_in_streams;       /* number incoming streams */

    sctp_U16bit          nextssn[SCTP_MAX_NUMSTREAM];        /* next ssn value expected*/
    sctp_U16bit          nextssn_notify[SCTP_MAX_NUMSTREAM]; /* next ssn value expected*/
    sctp_U32bit          num_rx_buf[SCTP_MAX_NUMSTREAM];     /* Number of rx buffers*/

#ifdef SCTP_DYNAMIC_IP_SUPPORT
    sctp_U32bit         serial_no;
#endif
    sctp_U32bit         wnd_size;
} sctp_red_receive_st;


/***********************Assoc red db structure ************************/
typedef   struct
{
    sctp_U32bit          assoc_id;
    sctp_U32bit          open;
    sctp_U32bit          state;

    /*
     * Remaining time of the different timers to be filled in this.
     * Timer IDs should not be filled in this as in TCB structure
     */
    sctp_U32bit          remain_time[SCTP_ASSOCTIMER_NUM];
    sctp_S32bit          timer_event[SCTP_ASSOCTIMER_NUM];
    sctp_U32bit          addr_index[SCTP_ASSOCTIMER_NUM];

    /* protocol parameters */
    sctp_prot_params_st    config;

    sctp_cong_reg_def_st   trx_cong_reg_def;
    sctp_cong_reg_def_st   rcv_cong_reg_def;

    /* restart option */
    sctp_restart_options_et restart_option;

    sctp_partial_data_st   partial_data;

    sctp_U32bit          init_retry;
    sctp_U32bit          init_retry_per_path;
    sctp_U32bit          cookie_retry;
    sctp_U32bit          shutdown_retry;
    sctp_U32bit          shutdown_ack_retry;
    sctp_U32bit          error_count;

    sctp_U32bit          configured_local_primary_address_index;

    sctp_U32bit          local_tag;
    sctp_U32bit          remote_tag;

    sctp_U32bit          local_virtual_tie_tag;
    sctp_U32bit          peer_virtual_tie_tag;

    sctp_U32bit          total_sent;

    sctp_ecn_info_st       ecn;

    sctp_U16bit            cookie_size;
    LIST                   cookie_list;

    /************For transmit structure*****************************/
    sctp_red_transmit_st    transmit;

    /************For receive structure*****************************/
    sctp_red_receive_st     receive;

#ifdef SCTP_STATS_ENABLED

    sctp_assoc_stats_st     assoc_stats;

#endif
    /*Put remote address parameters*/
    /*then SACK fragments after it*/
    /*Then put transmit buffers*/
    /*Then put receive buffers*/


#ifdef SCTP_DYNAMIC_IP_SUPPORT
    sctp_addr_conf_st           cmd_asconf;
    sctp_U8bit                  asconf_response[SCTP_MAX_PARAMS_PER_ASCONF];
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    sctp_restrictions_st        restrict;
#else
    sctp_restrictions_st        sctp_restrict;
#endif
    sctp_U32bit                 buf_len;
    sctp_U8bit                  asconf_buffer[SCTP_MAX_ASCONF_MSG_SIZE];
#endif

    sctpmtubuf_st               mtu_buffer;
    sctp_U8bit                  fsm_event;


    sctp_Boolean_t   zero_win_prob_on;
    sctp_Boolean_t   conn_accepted;
} sctp_red_assoc_db_st;

/***********************Socket red db structure ************************/
typedef   struct
{
    sctp_U32bit          ep_id;
    sctp_U32bit          server_ep_id;

    sctp_U32bit          num_created_ep;
    sctp_U32bit          created_ep_id[SCTP_MAX_LISTEN_BACKLOG];

    sctp_U32bit          ppi;

    sctp_prot_params_st  sk_config;
    sctp_cong_reg_def_st trx_cong_reg_def;
    sctp_cong_reg_def_st rcv_cong_reg_def;

    sctp_U16bit          n_assoc;
    sctp_U16bit          last_used_assoc;

    sctp_U16bit          this_port;
    sctp_U16bit          naddr;
    sctp_sockaddr_st     this_address[SCTP_MAX_TRANSPORT_ADDR];
    sctp_U8bit           hostname[SCTP_MAX_HOSTNAME_SIZE];

    sctp_U32bit          l_backlog;
    sctp_U32bit          flags;
    sctp_U8bit           trc_flags;

    sctp_U16bit          appl_id;

    sctp_sa_family_t     family;
    sctp_U8bit           sk_type;
    sctp_linger_st       so_linger;

    sctp_U32bit           moribund;

    sctp_Boolean_t       hostname_supported;
    sctp_Boolean_t       create_from_server;
    sctp_U8bit           cong_flag;

} sctp_red_ep_db_st;



typedef struct
{
    sctp_sockaddr_st     *p_sockaddr;
    sctp_checksum_et     checksum;
    void                 *p_info;
} sctp_addrinfo_st;

typedef enum
{
    SCTP_MESG_PEEK              = 0x0001, /* For Message Peek option */
    SCTP_BLOCKING_RECV          = 0x0002, /* If set indicates it should be
                                           * blocking receive call.
                                           */
    SCTP_FLUSH_MESG_AFTER_READ  = 0x0004  /* Set to indicate the flush the
                                           * buffer after read. This can be used
                                           * when the message is not flushed if
                                           * the buffer passed if less that the
                                           * actual mesg size.
                                           */

} sctp_enum_recv_opr_et;

#define SCTP_STREAMID_PRESENT   0x0001
#define SCTP_REM_LENGTH_PRESENT 0x0002
#define SCTP_DEST_ADDR_PRESENT  0x0004

typedef struct
{
    sctp_U32bit         recv_struct_flag;
    /*
     * Bit mask to indicate the presence of each
     * of the parameter in the structure. It takes
     * the value as given above.
     */
    sctp_U32bit         recv_opr_flags;
    /*
     * Bit mask to indicate the opertions to be
     * perfomred with the buffer. It takes the
     * values from enum sctp_enum_recv_opr_et.
     */
    sctp_U32bit         rem_length;
    /*
     * Remaining length to indicate the
     * number of bytes which are still unread.
     * This is because the buffer passed to
     * read was insufficient.
     */
    sctp_U16bit         stream_id;
    /*
     * Indicates the stream on which the mesg
     * has to be read or the stream on which
     * the mesg has been read.
     */
    sctp_sockaddr_st    dest_address;
    /*
     * Destination address from where the mesg
     * has been received.
     */

} sctp_api_recv_struct_st;

typedef enum
{
    SCTP_PORT_TYPE_UDP = 1,
    SCTP_PORT_TYPE_TCP = 2,
    SCTP_PORT_TYPE_INTERNAL       /* User for SCTP Client only. Indicates that
                                   * SCTP Stack are running in the same process
                                   * with the message queue interface between
                                   * them */
} sctp_port_type_et;

#ifdef SCTP_INET6_SUPPORT
#define SCTP_CNVT_SIG_ADDR_TO_SCTP_ADDR(sig_addr, sctp_addr, sctp_port)\
    {\
        if(SIG_TRANS_ADDR_IPV4 == sig_addr.ip_type)\
        {\
            sctp_addr.family = AF_INET;\
            sctp_addr.ip.v4.addr = sig_addr.ip_addr_ut.addr_v4.addr;\
            sctp_port = sig_addr.ip_addr_ut.addr_v4.port;\
        }\
        else\
        {\
            sctp_addr.family = AF_INET6;\
            sctp_addr.ip.v6.flow_info = sig_addr.ip_addr_ut.addr_v6.flow_info;\
            sctp_addr.ip.v6.scope_id = sig_addr.ip_addr_ut.addr_v6.scope_id;\
            sctp_addr.ip.v6.addr.s_u6_addr32[0] = \
                                                  sig_addr.ip_addr_ut.addr_v6.ip_addr_ut.addr32[0];\
            sctp_addr.ip.v6.addr.s_u6_addr32[1] = \
                                                  sig_addr.ip_addr_ut.addr_v6.ip_addr_ut.addr32[1];\
            sctp_addr.ip.v6.addr.s_u6_addr32[2] = \
                                                  sig_addr.ip_addr_ut.addr_v6.ip_addr_ut.addr32[2];\
            sctp_addr.ip.v6.addr.s_u6_addr32[3] = \
                                                  sig_addr.ip_addr_ut.addr_v6.ip_addr_ut.addr32[3];\
            sctp_port = sig_addr.ip_addr_ut.addr_v6.port;\
        }\
    }

#define SCTP_CNVT_SCTP_ADDR_TO_SIG_ADDR(sctp_addr, sctp_port, sig_addr)\
    {\
        if(sctp_addr.family==AF_INET)\
        {\
            sig_addr.ip_type = SIG_TRANS_ADDR_IPV4;\
            sig_addr.ip_addr_ut.addr_v4.addr = sctp_addr.ip.v4.addr;\
            sig_addr.ip_addr_ut.addr_v4.port = sctp_port;\
        }\
        else\
        {\
            sig_addr.ip_type = SIG_TRANS_ADDR_IPV6;\
            sig_addr.ip_addr_ut.addr_v6.flow_info = sctp_addr.ip.v6.flow_info;\
            sig_addr.ip_addr_ut.addr_v6.scope_id = sctp_addr.ip.v6.scope_id;\
            sig_addr.ip_addr_ut.addr_v6.ip_addr_ut.addr32[0] \
                = sctp_addr.ip.v6.addr.s_u6_addr32[0];\
            sig_addr.ip_addr_ut.addr_v6.ip_addr_ut.addr32[1] \
                = sctp_addr.ip.v6.addr.s_u6_addr32[1];\
            sig_addr.ip_addr_ut.addr_v6.ip_addr_ut.addr32[2] \
                = sctp_addr.ip.v6.addr.s_u6_addr32[2];\
            sig_addr.ip_addr_ut.addr_v6.ip_addr_ut.addr32[3] \
                = sctp_addr.ip.v6.addr.s_u6_addr32[3];\
            sig_addr.ip_addr_ut.addr_v6.port = sctp_port;\
        }\
    }
#else
#define SCTP_CNVT_SIG_ADDR_TO_SCTP_ADDR(sig_addr, sctp_addr, sctp_port)\
    {\
        if(SIG_TRANS_ADDR_IPV4 == sig_addr.ip_type)\
        {\
            sctp_addr.family = AF_INET;\
            sctp_addr.ip.v4.addr = sig_addr.ip_addr_ut.addr_v4.addr;\
            sctp_port = sig_addr.ip_addr_ut.addr_v4.port;\
        }\
    }

#define SCTP_CNVT_SCTP_ADDR_TO_SIG_ADDR(sctp_addr, sctp_port, sig_addr)\
    {\
        if(sctp_addr.family==AF_INET)\
        {\
            sig_addr.ip_type = SIG_TRANS_ADDR_IPV4;\
            sig_addr.ip_addr_ut.addr_v4.addr = sctp_addr.ip.v4.addr;\
            sig_addr.ip_addr_ut.addr_v4.port = sctp_port;\
        }\
    }
#endif

#define SCTP_SEND_RECV_PORT_OFFSET  100

/* SPR 20570 Starts for CSR 1-7626869 */
#define SCTP_HB_INTERVAL_TIMEOUT    sctp_db_main.sctp_def_prot_params.hb_interval
/* SPR 20570 Ends for CSR 1-7626869 */


#endif /* __FILE_sctp_types_SEEN__     -- This MUST appear after all code! */

