/******************************************************
File to be used for SM interface testing for Vxworks or similar OS`
******************************************************/
#ifndef CALL__API_C__
#define CALL__API_C__

#include <s_os.h>
#include <s_cogbl.h>
#include <s_colst.h>
#include <s_cotyps.h>
#include <s_clpapi.h>
#include <s_uismty.h>
#include <s_dbg.h>
#include <s_in.h>
#include <s_err.h>
#include <s_stats.h>
#include <s_red.h>
#include <s_es.h>
#include <s_ostmr.h>
#include <s_osmem.h>
#include <s_uisusk.h>
#include <s_uisuif.h>
#include <s_cspl.h>
#define SCTP_DELIM " #=\n\0"

void call_api_sctp_set_stack_envt( sctp_Boolean_t b_alrdy_init,
                                   sctp_Boolean_t is_block );
void start_sctp( void );
void enable_sctp_traces( void );
void get_clk_res( void );
void sctp_user_task( void );
void call_api_sctp_printfack_envt( sctp_Boolean_t b_alrdy_init, sctp_Boolean_t block );
void call_sctp_api_get_stack_entity_version( void );

sctp_return_t
sctp_parse_cliaddr_into_sctp_addr(
    char *p_token,
    sctp_sockaddr_st *p_addr );

unsigned int str2ipaddr( register char *p_str_addr );

/* Changes for SPR-21022 Starts here (IPv6) */
#ifdef SCTP_INET6_SUPPORT
void call_sctp_api_init6_stack(
    sctp_U32bit       tx_buffers,
    sctp_U32bit       rx_buffers,
    sctp_U32bit       reserve_buffer,
    sctp_U16bit       standard_udp_port,
    sctp_U8bit        *local_addr );

void  test_sctp_api_bind6( sctp_U32bit sockid,
                           sctp_U16bit port,
                           char        *addr );

void call_sctp_api_init_v6_v4_stack(
    sctp_U32bit       tx_buffers,
    sctp_U32bit       rx_buffers,
    sctp_U32bit       reserve_buffer,
    sctp_U16bit       standard_udp_port,
    sctp_U8bit        *local_addr,      /* IPv6 Address */
    sctp_U8bit        *local_addrv4 );  /* IPv4 Address */

void  test_sctp_api_bind_v6_v4_addrs
( sctp_U32bit sockid,
  sctp_U16bit port,
  char        *addr,     /* IPv6 Address */
  char        *addr_v4 ); /* IPv4 Address */

#ifndef SCTP_UI_SOCK_API
void call_sctp_api_bindx6(
    sctp_U32bit            assoc_id,
    sctp_U8bit            *addr,        /* IPv6 Address */
    sctp_bindx_flag_et     flags,
    sctp_Boolean_t         block );
#endif

#endif
/* Changes for SPR-21022 Ends here */

#define SCTP_RECV_PRIORITY   50
#define SCTP_RECV_OPTIONS    VX_FP_TASK
#define SCTP_RECV_STACKSIZE  20000

typedef char                gw_boolean;
typedef unsigned long int   U32bit;
typedef unsigned short int  U16bit;
typedef unsigned char       U8bit;
typedef long int        S32bit;
typedef short int       S16bit;
typedef char            S8bit;

typedef U8bit           return_t;
typedef void           *pvoid;

#define GW_TRUE         1
#define GW_FALSE        0

#define GW_SUCCESS      GW_TRUE
#define GW_FAILURE      GW_FALSE

#define GW_ERROR            -1
#define GW_OK                0

sctp_U32bit sctp_udp_port = 7178;
int         num_local_addrs = 3;
sctp_U8bit  net_addr[3][20]  =  {"10.248.195.51", "192.168.209.62", "192.168.209.202"};
sctp_U8bit  remote_addr[16] = "10.248.195.48";

/* Changes for SPR-21022 Starts here */
sctp_U8bit *sctp_hostname = ( sctp_U8bit * ) "gsheoran.hss.hns.com";
/* Changes for SPR-21022 Ends here */
void
sctp_print_sock_addr(
    sctp_Pvoid_t   *p_addr );

sctp_U8bit th_id[25];
#ifndef VXWORKS_6_9_SUPPORT
S32bit conf_ip_converter( char *p_dottedIP );
#endif

/* begin for platform */
int sctp_user_action = 0;
sctp_U16bit application_id;
sctp_U32bit sd;
sctp_U16bit sctp_port = 10;

sctp_U16bit application_id1;
sctp_U32bit sd1;
sctp_U16bit sctp_port1 = 10;

sctp_Boolean_t block = SCTP_FALSE;
sctp_Boolean_t graceful = SCTP_FALSE;
sctp_U32bit  use_sd;

#define ES_PRINT_HEX(p_buffer,noctets) { sctp_U16bit i; for (i=0; i<noctets; i++) printf("%x ",*(p_buffer + i)); printf("\n"); }
#define DEFAULTPORT 4987

#ifndef SCTP_UI_SOCK_API

static int global_count;
void test_sctp_api_register_su( void );
void test_sctp_api_register_su1( void );
void test_sctp_api_socket( sctp_U16bit application_id );
void  test_sctp_api_socket_inet6( sctp_U16bit application_id );
void test_sctp_api_bind( sctp_U32bit sockid,
                         sctp_U16bit port,
                         char        *addr );
void  test_multi_addr_sctp_api_bind( sctp_U32bit sockid,
                                     sctp_U16bit port,
                                     int no_addr,
                                     char        *local_addr );
void test_sctp_api_accept( sctp_U32bit par_sd );
void test_sctp_api_connect( sctp_U32bit sockid,
                            sctp_U16bit port,
                            char        *addr );
void test_sctp_api_send( sctp_U32bit par_sd );

/* Changes for SPR-21022 Starts here */
void
test_sctp_api_recv(
    sctp_assoc_t                assoc_id,
    sctp_U32bit                 data_size,
    sctp_Boolean_t              block );
/* Changes for SPR-21022 Ends here */

void test_sctp_api_close( sctp_U32bit par_sd );
void test_sctp_api_listen( sctp_U32bit par_sd );
void test_sctp_api_config_txrx_num_frag( sctp_U32bit assoc_id,
                                         sctp_U32bit min_buffer, sctp_U32bit med_buffer,
                                         sctp_U32bit max_buffer, sctp_Boolean_t is_transmit,
                                         sctp_Boolean_t is_increment );
void test_sctp_get_txrx_num_frag( sctp_U32bit assoc_id );
void test_sctp_api_config_cong_reg_def( sctp_U32bit assoc_id,
                                        sctp_U8bit high_cong_lvl_onset, sctp_U8bit high_cong_lvl_abate,
                                        sctp_U8bit med_cong_lvl_onset, sctp_U8bit med_cong_lvl_abate,
                                        sctp_U8bit low_cong_lvl_onset, sctp_U8bit low_cong_lvl_abate,
                                        sctp_U8bit no_cong_lvl_onset, sctp_U8bit no_cong_lvl_abate,
                                        sctp_Boolean_t is_transmit );
void test_sctp_api_get_cong_reg_def( sctp_U32bit assoc_id,
                                     sctp_Boolean_t is_transmit );


void *callback_func_temp( sctp_U32bit event, void *event_data );
void *callback_func_temp1( sctp_U32bit event, void *event_data );


char *connection_state_s[SCTP_STATE_NUM] =
{
    "SCTPSTATE_CLOSED",
    "SCTPSTATE_COOKIEWAIT",
    "SCTPSTATE_COOKIESENT",
    "SCTPSTATE_SHUTDOWNPEND",
    "SCTPSTATE_SHUTDOWNRCVD",
    "SCTPSTATE_SHUTDOWNSENT",
    "SCTPSTATE_ESTABLISHED"
};

char *end_point_status_s[2] =
{
    "sctp_endpoint_unreachable",
    "sctp_endpoint_reachable"
};

char *event_str[11] =
{
    "SCTPNOTIFY_OPERROR",
    "SCTPNOTIFY_DESTSTATCHANGE",
    "SCTPNOTIFY_ENDSTATCHANGE",
    "SCTPNOTIFY_CONGESTION",
    "SCTPNOTIFY_SYSERROR",
    "SCTPNOTIFY_CONNPENDING",
    "SCTPNOTIFY_DATAPENDING",
    "SCTPNOTIFY_SENDFAILURE",
    "SCTPNOTIFY_PARTIAL_DATAPENDING",
    "SCTPNOTIFY_RECVCONGESTION",
    "SCTPNOTIFY_SHUTDOWN_EVENT"
};

#endif /* SCTP_UI_SOCK_API */

/* Changes for SPR-21022 Starts here */
#ifndef VXWORKS_6_9_SUPPORT
char  *sctp_error_string[TOTAL_ERRORS] =
{
    "E_NOERROR",
    "EINV_APPID",
    "EINV_ADDRS"    ,
    "EINV_PARAMS"   ,
    "E_NORESOURCES" ,
    "EINV_TYPEOFSTATS" ,
    "E_STATSDISABLED" ,
    "EINV_ERRORNUM" ,
    "EINV_ERRORLEVEL" ,
    "E_ERRORDISABLED" ,
    "EINV_TRCTYPE" ,
    "EINV_TRCLEVEL" ,
    "E_TRCDISABLED" ,
    "EINV_ASSOCID"  ,
    "E_REQ_IN_WRONG_STATE",
    "E_NOTHING_IN_QUEUE",
    "E_NO_PENDING_REQUEST"
}
#else
char  *sctp_error_string[] =
{
    "E_NOERROR",
    "EINV_APPID",
    "EINV_ADDRS",
    "EINV_PARAMS",
    "E_NORESOURCES",
    "EINV_TYPEOFSTATS",
    "E_STATSDISABLED",
    "EINV_ERRORNUM",
    "EINV_ERRORLEVEL",
    "E_ERRORDISABLED",
    "EINV_TRCTYPE",
    "EINV_TRCLEVEL",
    "E_TRCDISABLED",
    "EINV_ASSOCID",
    "E_REQ_IN_WRONG_STATE",
    "E_NOTHING_IN_QUEUE",
    "E_NO_PENDING_REQUEST",
    "E_NO_PENDING_CONNECTION",
    "E_UNKNOWN_HOST",
    "E_SCTPPROT_INVALID_TLDATAGRAM",
    "E_INVALID_CHUNKID",
    "E_INVALID_TL_SIZE",
    "E_INVALID_CHUNK_COMBINATION",
    "E_INVALID_HBINFO_TYPE",
    "E_INVALID_HBINFO_LEN",
    "E_INVALID_CHUNK_LEN",
    "E_INVALID_PARAMETER_ID",
    "E_INVALID_PARAMETER_LEN",
    "E_UNKNOWN_PARAMETER_ID",
    "E_UNKNOWN_PARAMETER_RETURN_ERROR",
    "E_UNKNOWN_PARAMETER_INIT_RETURN_ERROR",
    "E_UNKNOWN_PARAMETER_INITACK_RETURN_ERROR",
    "E_SKIP_PARAMETER",
    "E_SKIP_PARAMETER_RETURN_ERROR",
    "E_INVALID_CHUNK_SIZE",
    "E_INVALID_CHUNK_PARAMETER",
    "E_UNSUPPORTED_ADDR",
    "E_INVALID_ADDR",
    "E_INVALID_NUM_OPTIONAL_PARAMS",
    "E_INVALID_FIXED_PARAMETERS",
    "E_MALLOC_FAILED",
    "E_INIT_QUEUES",
    "E_ESHELL_INIT_FAILED",
    "E_INVALID_EVENT_FOR_STATE",
    "E_INVALID_SACK_CHUNK",
    "E_PMTU_DATABASE_OVERFLOW",
    "E_SCTP_ERROR_DISABLED",
    "E_SCTP_INVALID_ERROR_FLAG",
    "E_SCTP_INVALID_ERROR_LEVEL",
    "E_API_FAILURE",
    "E_INVALID_TSN",
    "E_DUPLICATE_TSN",
    "E_MISSING_PARAMS",
    "E_NAME_RES_FAILED",
    "E_BUFFER_TOO_SMALL",
    "E_UNKNOWN_CHUNK_ID",
    "E_UNKNOWN_CHUNK_RETURN_ERROR",
    "E_SKIP_CHUNK",
    "E_SKIP_CHUNK_RETURN_ERROR",
    "E_COOKIE_SENT_TO_RESOLVER",
    "E_INVALID_PORT_NUMBER",
    "E_INVALID_SCTP_PORT",
    "E_SU_ALREADY_REGISTRED",
    "E_INV_SU_ID",
    "E_ASSOC_DELETED",
    "E_SYSCALL_FAILURE",
    "E_DB_GET_FREELIST_FAILED",
    "E_DB_NO_FREE_ASSOCIATION",
    "E_INTERNAL_ERROR",
    "E_TMP_ECODE_FOR_SACK",
    "E_UNSUPP_ADDR_TYPE",
    "E_DB_NO_FREE_ENDPOINT",
    "E_ADDR_TYPE_MISMATCH",
    "E_FRAGMENTATION",
    "E_TIMER_INIT_FAILED",
    "E_ASSOC_EXISTS",
    "E_CLI_STK_IPC_FAILURE",
    "E_VERIFICATION_TAG_MISMATCH",
    "E_PMTU_PROCEDURE_DISABLED",
    "E_PMTU_CAPABLE_ALREADY_SET",
    "E_DB_INV_CONG_LVL_VALUE",
    "EINV_RTO_MIN_PARAMS",
    "EINV_RTO_MAX_PARAMS",
    "EINV_RTO_INIT_PARAMS",
    "EINV_RTO_COMBINATION_PARAMS",
    "EINV_COOKIE_LIFE_PARAMS",
    "EINV_HB_INTERVAL_PARAMS",
    "EINV_SACK_FREQUENCY_PARAMS",
    "EINV_SACK_PERIOD_PARAMS",
    "EINV_MTU_SIZE_PARAMS",
    "EINV_PATH_RETRANS_PARAMS",
    "EINV_STREAM_PARAMS",
    "EINV_ALPHA_BETA_PARAMS",
    "EINV_ASSOC_MAX_RETRANS_PARAMS",
    "EINV_PATH_MAX_RETRANS_PARAMS",
    "EINV_MAX_INIT_RETRANS_PARAMS",
    "EINV_CHECKSUM_PARAMS",
    "EINV_PMTUCHECKSUM_PARAMS",
    "EINV_HB_INFO_LEN",
    "TOTAL_ERRORS"
};
#endif
/* Changes for SPR-21022 Ends here */

#ifdef SCTP_UI_MESSAGEBASED
int test_sctp_init_stack_ipc( sctp_U16bit port, char *addr, sctp_port_type_et  port_type );
int test_sctp_init_stack_ipc( sctp_U16bit port, char *addr, sctp_port_type_et  port_type )
{
    sctp_return_t ret ;
    sctp_error_t errorno;
    sctp_sockaddr_st to_addr;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( addr );
    /*to avoid the warning*/

    to_addr.family = SCTP_AF_INET;
    to_addr.ip.v4.addr = 0;
    ret = sctp_init_stack_ipc( port, to_addr, port_type, &errorno );

    if( ret == SCTP_FAILURE )
    {
        printf( "ERROR !! error id = %s \r\n" , sctp_error_string[errorno] );
    }

    else
    {
        printf( "SUCCESS !! Stack IPC started" );
    }

    return ret;
}
#endif

#define  MAX_DATA_SIZE  3000

extern void *notification_handler( unsigned short event, void *event_data );
/* static unsigned  int context_value=0;*/
void debug_counter();

/* Changes for SPR-21022 Starts here */
#ifdef SCTP_INET6_SUPPORT
sctp_stack_type_et    vx_stack_type = SCTP_OVER_INET6;
#else
sctp_stack_type_et    vx_stack_type = SCTP_OVER_INET;
#endif

/* Changes for SPR-21022 Ends here */

sctp_Boolean_t        vx_raw_socket_option = SCTP_TRUE;
sctp_U32bit usec_var;

extern sctp_U16bit application_id;
extern sctp_U32bit  sd;
extern sctp_U16bit  sctp_port;
extern sctp_Boolean_t block;

void  help_sm_api( void );
void call_sctp_api_get_stack_entity_version();
void call_sctp_api_init_stats( sctp_stats_et  stats_type );
void call_sctp_api_disable_stats( sctp_stats_et  stats_type );
void call_sctp_api_enable_stats( sctp_stats_et  stats_type );
void call_sctp_api_get_stats(
    sctp_stats_et    stats_type,
    sctp_U32bit      association_id );
void call_sctp_api_get_error_level( sctp_error_flag_et    errorflag );
void call_sctp_api_set_error_level(
    sctp_error_flag_et  errorflag,
    sctp_error_level_et  error_level );
void
call_sctp_api_enable_assoc_trc(
    sctp_U32bit          assoc_id,
    sctp_Boolean_t       trc_all_assoc,
    sctp_trc_et              trace_type );
void
call_sctp_api_disable_assoc_trc(
    sctp_U32bit      assoc_id,
    sctp_Boolean_t   stop_trc_all_assoc );
void
call_sctp_api_get_trace_info(
    sctp_U32bit assoc_id );
void
call_sctp_api_init_stack(
    sctp_U32bit       tx_buffers,
    sctp_U32bit       rx_buffers,
    sctp_U32bit       reserve_buffer,
    sctp_U16bit       standard_udp_port,
    sctp_U8bit        *local_addr );

/* Changes for SPR-21022 Starts here */
void call_sctp_api_deinit_stack( sctp_Boolean_t forced );
void test_sctp_api_get_rtt_info
(
    sctp_U32bit     assoc_id,
    sctp_Boolean_t  reset_flag );
/* Changes for SPR-21022 Ends here */

void call_multi_addr_sctp_api_init_stack(
    sctp_U32bit       tx_buffers,
    sctp_U32bit       rx_buffers,
    sctp_U32bit       reserve_buffer,
    sctp_U16bit       standard_udp_port,
    sctp_U16bit       no_addr,
    sctp_U8bit        *local_addr );

#ifdef SCTP_UI_SOCK_API

void
sctp_print_sock_addr(
    sctp_Pvoid_t   *p_addr );

void sctp_sk_display_error(
    int sk_errno );

void
call_sctp_socket(
    int                  domain,
    int                  type,
    int                  protocol );

void
call_sctp_bindx(
    int                  sd,
    int                  port,
    int                  flags,
    char                 *addr );

void
call_sctp_bind(
    int                  sd,
    int                  port,
    char                 *addr ) ;

void
call_sctp_close(
    int                  sd );

void
call_sctp_listen(
    int                  sd,
    int                  backlog );

void
call_sctp_accept(
    int                  sd ) ;

void
call_sctp_connect(
    int                  sd,
    int                  port,
    char                 *addr ) ;

void
call_sctp_sendmsg_init(
    int                  sd,
    int                  port,
    char                 *addr,
    int                  out_stream,
    int                  in_stream,
    int                  max_attempts,
    int                  data_size,
    char                 *data ) ;

void
call_sctp_sendmsg(
    int                  sd,
    int                  port,
    char                 *addr,
    int                  stream,
    int                  ppid,
    int                  context,
    int                  flags,
    int                  assoc_id,
    int                  data_size,
    char                 *data ) ;

void
call_sctp_recvmsg(
    int            sd );

void call_sctp_sctp_send(
    sctp_U32bit assoc_id,
    sctp_U8bit  *p_data_payload,
    sctp_U32bit  data_size,
    sctp_U32bit  flags,
    sctp_U32bit  p_sinfo_stream_no,
    sctp_U16bit   p_sinfo_stream_seq_no,
    sctp_U16bit   p_sinfo_flags,
    sctp_U32bit   p_sinfo_ppid,
    sctp_U32bit   p_sinfo_context,
    sctp_U32bit   p_sinfo_timetolive,
    sctp_U32bit   p_sinfo_tsn );


void call_sctp_sctp_sendx(
    sctp_U32bit assoc_id,
    sctp_U8bit  *p_data_payload,
    sctp_U32bit  data_size,
    sctp_U8bit   *send_addr,
    int          port,
    sctp_U32bit  flags,
    sctp_U32bit  p_sinfo_stream_no,
    sctp_U16bit  p_sinfo_flags,
    sctp_U32bit  p_sinfo_ppid );


void
call_sctp_notification_on(
    int                  sd );

void
call_sctp_getladdrs(
    int       sd,
    int       assoc_id );


void
call_sctp_peeloff(
    int                  sd,
    int                  assoc_id );

void
call_sctp_send(
    int                  sd,
    int                  data_size,
    char                 *data );

void
call_sctp_sendto(
    int                  sd,
    int                  port,
    char                 *addr,
    int                  data_size,
    char                 *data );

void
call_sctp_recv(
    int            sd,
    int            data_size );

void
call_sctp_recvfrom(
    int            sd,
    int            data_size );

void
call_sctp_shutdown(
    int          sd,
    int          how );

void
call_sctp_setsockopt_rto(
    int         sd,
    int         level,
    int         rto_init,
    int         rto_min,
    int         rto_max,
    int         assoc_id );

void
call_sctp_setsockopt_general(
    int         sd,
    int         level,
    int         optname,
    int         value1,
    int         value2 );

void
call_sctp_setsockopt_primary(
    int         sd,
    int         level,
    int         optname,
    int         assoc_id,
    char        *addr );

void
call_sctp_getsockopt(
    int         sd,
    int         level,
    int         optname,
    int         assoc_id );

void
call_sctp_opt_info(
    int            sd,
    int            assoc_id,
    int            option,
    int            hb_interval,
    int            pathmaxrtx,
    char           *addr );

void
call_sctp_getpeername(
    int        sd );

void
call_sctp_getpaddrs(
    int       sd,
    int       assoc_id );

void
call_sctp_getsckname(
    int        sd );

void
call_sctp_getsockname(
    int        sd );

void  help_sctp_socket( void );
void  help_sctp_bind( void );
void  help_sctp_bindx( void );
void  help_sctp_connect( void );
void  help_sctp_close( void );
void  help_sctp_listen( void );
void  help_sctp_accept( void );
void  help_sctp_sendmsg( void );
void  help_sctp_sendto( void );
void  help_sctp_send( void );
void  help_sctp_recv( void );
void  help_sctp_recvfrom( void );
void  help_sctp_recvmsg( void );
void  help_sctp_shutdown( void );
void  help_sctp_peeloff( void );
void  help_sctp_getpaddrs( void );
void  help_sctp_getladdrs( void );
void  help_sctp_getsockopt( void );
void  help_sctp_setsockopt( void );
void  help_sctp_opt_info( void );
void  help_sctp_getpeername( void );
void  help_sctp_getsockname( void );
#endif

/***************************************************************************

    System Management apis from this point on

***************************************************************************/
void  help_sm_api( void )
{

    printf( "\tThe	following apis can be invoked\r\n" );
    printf( "\tWhere no parameters	are mentioned none should be passed\r\n" );
    printf( "\tcall_sctp_api_get_stack_entity_version(no parameters)\r\n" );
    printf( "\t\tExample:call_sctp_api_get_stack_entity_version\r\n\n" );
#ifdef SCTP_STATS_ENABLED
    printf( "\tcall_sctp_api_init_stats(sctp_stats_et  stats_type)\r\n" );
    printf( "\t\tExample:call_sctp_api_init_stats(1)\r\n\n" );
    printf( "\tcall_sctp_api_enable_stats(sctp_stats_et  stats_type)\r\n" );
    printf( "\t\tExample:call_sctp_api_enable_stats(2)\r\n\n" );
    printf( "\tcall_sctp_api_disable_stats(sctp_stats_et  stats_type)\r\n" );
    printf( "\t\tExample:call_sctp_api_disable_stats(2)\r\n\n" );
    printf( "\tcall_sctp_api_get_stats(sctp_stats_et stats_type,\r\n" );
    printf( "\t			    sctp_U32bit	 association_id\r\n" );
    printf( "\t\tExample:call_sctp_api_get_stats(1,2)\r\n\n" );
#endif
#ifdef SCTP_ERROR_ENABLED
    printf( "\tcall_sctp_api_get_error_level(sctp_error_flag_et errorflag)\r\n" );
    printf( "\t\tExample:call_sctp_api_get_error_level(1)\r\n\n" );
    printf( "\tcall_sctp_api_set_error_level(sctp_error_flag_et  errorflag,\r\n" );
    printf( "\t				  sctp_error_level_et error_level\r\n" );
    printf( "\t\tExample:call_sctp_api_set_error_level(1,3)\r\n\n" );
#endif
#ifdef SCTP_DEBUG_ENABLED
    printf( "\tcall_sctp_api_enable_assoc_trc(sctp_U32bit	         assoc_id,\r\n" );
    printf( "\t                               sctp_Boolean_t        trc_all_assoc,\r\n" );
    printf( "\t                               sctp_trc_et           trace_type);\r\n" );
    printf( "\t\tExample:call_sctp_api_enable_assoc_trc(2,0,1,2)\r\n\n" );
    printf( "\tcall_sctp_api_disable_assoc_trc(sctp_U32bit	 assoc_id,\r\n" );
    printf( "\t                                sctp_Boolean_t stop_trc_all_assoc\r\n" );
    printf( "\t\tExample:call_sctp_api_disable_assoc_trc(1,1);\r\n\n" );
    printf( "\tcall_sctp_api_get_trace_info(\r\n" );
    printf( "\t	 sctp_U32bit		assoc_id,\r\n" );
    printf( "\t\tExample:call_sctp_api_get_trace_info\r\n\n" );
#endif
    printf( "\tvoid call_sctp_api_init_stack(\r\n" );
    printf( "\t	 sctp_U16bit		max_total_applications,\r\n" );
    printf( "\t	 sctp_U32bit		max_total_endpoints,\r\n" );
    printf( "\t	 sctp_U32bit		max_total_associations,\r\n" );
    printf( "\t	 sctp_U32bit		max_streams_per_association,\r\n" );
    printf( "\t	 sctp_U32bit		min_tx_buffers,\r\n" );
    printf( "\t	 sctp_U32bit		med_tx_buffers,\r\n" );
    printf( "\t	 sctp_U32bit		max_tx_buffers,\r\n" );
    printf( "\t	 sctp_U32bit		min_rx_buffers,\r\n" );
    printf( "\t	 sctp_U32bit		med_rx_buffers,\r\n" );
    printf( "\t	 sctp_U32bit		max_rx_buffers,\r\n" );
    printf( "\t	 sctp_U16bit	        standard_udp_port,\r\n" );
    printf( "\t	 sctp_U8bit	        stale_cookie_policy,\r\n" );
    printf( "\t	 sctp_U8bit	        ack_policy,\r\n" );
    printf( "\t	 sctp_Boolean_t	        ecn_capable,\r\n" );
    printf( "\t	 sctp_U8bit	        *local_addr)\r\n" );
    printf( "\t\tExample:call_sctp_api_init_stack(10,10,10,100,9010,0,0,0, \"139.85.238.29\")\r\n\n" );
}

void call_sctp_api_get_stack_entity_version()
{
    sctp_ver_info_st           *ver_info;
    sctp_return_t          api_return;


    char *logthis;

    logthis = ( char * )malloc( 160 );


    printf( "Invoked sctp_api_get_stack_entity_version\n" );

    printf( "The parameter	to be passed is	*ver_info\n" );


    ver_info = ( sctp_ver_info_st * )malloc( sizeof( sctp_ver_info_st ) );

    if( ver_info != NULL )
    {
        sprintf( logthis, "ver_info->major_ver=%u ,	ver_info->minor_ver=%u\n", ver_info->major_ver, ver_info->minor_ver );
    }

    else
    {
        sprintf( logthis, "ver_info=%p\n", ver_info );
    }

    printf( "%s", logthis );


    api_return = sctp_api_get_stack_entity_version( ver_info );

    if( ver_info != NULL )
    {
        sprintf( logthis, "ver_info->major_ver=%u ,	ver_info->minor_ver=%u	, return value=%u\n\n", ver_info->major_ver, ver_info->minor_ver, api_return );
    }

    else
    {
        sprintf( logthis, "ver_info=%p , return value=%u\n\n", ver_info, api_return );
    }

    printf( "%s", logthis );

    free( logthis );

    if( ver_info != NULL )
    {
        free( ver_info );
    }
}
#ifdef SCTP_STATS_ENABLED

void call_sctp_api_init_stats( sctp_stats_et  stats_type )
{
    sctp_return_t         api_return;
    sctp_error_t          *errorno;


    char *logthis;

    logthis = ( char * )malloc( 160 );

    /* system("/bin/clear"); */
    printf( "\n\n" );

    printf( "Invoked sctp_api_init_stats\n" );

    printf( "The parameters to be passed are stats_type, *errorno\n" );


    errorno = ( sctp_error_t * )malloc( sizeof( sctp_error_t ) );



    if( errorno != NULL )
    {
        sprintf( logthis, "stats_type=%u , *errorno=%u\n", stats_type, *errorno );
    }

    else
    {
        sprintf( logthis, "stats_type=%u , errorno=%p\n", stats_type, errorno );
    }

    printf( "%s", logthis );

    api_return = sctp_api_init_stats( stats_type, errorno );

    if( errorno != NULL )
    {
        sprintf( logthis, "stats_type=%u , *errorno=%u , return value=%u\n", stats_type, *errorno, api_return );
    }

    else
    {
        sprintf( logthis, "stats_type=%u , errorno=%p , return value=%u\n", stats_type, errorno, api_return );
    }

    printf( "%s", logthis );


    free( logthis );

    if( errorno != NULL )
    {
        free( errorno );
    }

}

void call_sctp_api_enable_stats( sctp_stats_et  stats_type )
{

    sctp_return_t         api_return;

    sctp_error_t          *errorno;

    char *logthis;

    logthis = ( char * )malloc( 160 );

    /* system("/bin/clear"); */
    printf( "\n\n" );

    printf( "Invoked sctp_api_enable_stats\n" );

    printf( "The parameters to be passed are stats_type, *errorno\n" );

    errorno = ( sctp_error_t * )malloc( sizeof( sctp_error_t ) );

    if( errorno != NULL )
    {
        sprintf( logthis, "stats_type=%u , *errorno=%u\n", stats_type, *errorno );
    }

    else
    {
        sprintf( logthis, "stats_type=%u , errorno=%p\n", stats_type, errorno );
    }

    printf( "%s", logthis );

    api_return = sctp_api_enable_stats( stats_type, errorno );

    if( errorno != NULL )
    {
        sprintf( logthis, "stats_type=%u , *errorno=%u , return value=%u\n", stats_type, *errorno, api_return );
    }

    else
    {
        sprintf( logthis, "stats_type=%u , errorno=%p , return value=%u\n", stats_type, errorno, api_return );
    }

    printf( "%s", logthis );


    free( logthis );

    if( errorno != NULL )
    {
        free( errorno );
    }

}

void call_sctp_api_disable_stats( sctp_stats_et  stats_type )
{

    sctp_return_t         api_return;

    sctp_error_t          *errorno;

    char *logthis;

    logthis = ( char * )malloc( 160 );
    /* system("/bin/clear"); */
    printf( "\n\n" );

    printf( "Invoked sctp_api_disable_stats\n" );

    printf( "The parameters to be passed are stats_type, *errorno\n" );

    errorno = ( sctp_error_t * )malloc( sizeof( sctp_error_t ) );

    if( errorno != NULL )
    {
        sprintf( logthis, "stats_type=%u , *errorno=%u\n", stats_type, *errorno );
    }

    else
    {
        sprintf( logthis, "stats_type=%u , errorno=%p\n", stats_type, errorno );
    }

    printf( "%s", logthis );

    api_return = sctp_api_disable_stats( stats_type, errorno );

    if( errorno != NULL )
    {
        sprintf( logthis, "stats_type=%u , *errorno=%u , return value=%u\n", stats_type, *errorno, api_return );
    }

    else
    {
        sprintf( logthis, "stats_type=%u , errorno=%p	, return value=%u\n", stats_type, errorno, api_return );
    }

    printf( "%s", logthis );


    free( logthis );

    if( errorno != NULL )
    {
        free( errorno );
    }

}

void call_sctp_api_get_stats(
    sctp_stats_et        stats_type,
    sctp_U32bit      association_id )
{
    sctp_return_t        api_return;

    sctp_U8bit           stats_data[5000];
    sctp_error_t         errorno;

    char logthis[4096];


    /*errorno=(sctp_error_t *)malloc(sizeof(sctp_error_t));*/
    /*logthis=(char *)malloc(160);*/

    /* system("/bin/clear"); */
    printf( "\n\n" );

    printf( "Invoked sctp_api_get_stats\n" );


    printf( "The parameters to be passed are stats_type, association_id, *stats_data, *errorno\n" );

    sprintf( logthis, "stats_type=%u , association_id=%u\n", stats_type, association_id );
    printf( "%s", logthis );

    api_return = sctp_api_get_stats( stats_type, association_id, &stats_data[0], 0, &errorno );

    sprintf( logthis, "stats_type=%u , association_id=%u ,	*errorno=%u , return value=%u\n\n", stats_type, association_id, errorno, api_return );
    printf( "%s", logthis );


    /* SPR 591 :Do not print if return fails */
    if( api_return == SCTP_SUCCESS )
    {
        switch( stats_type )
        {
            case  SCTP_PROT_INTERNAL_STATS :
                {
                    sctp_proto_internal_stats_st  *base;

                    base = ( sctp_proto_internal_stats_st * )stats_data;

                    sprintf( logthis, "stats_data->no_applications_reg=%u, ->no_associations_open=%u , no_dest_marked_active=%u , ", base->no_applications_reg, base->no_associations_open, base->no_dest_marked_active );
                    printf( "%s", logthis );

                    sprintf( logthis, "->no_dest_marked_inactive=%u , ->no_endpoints_marked_up=%u, ->->no_endpoints_marked_down=%u\n\n", base->no_dest_marked_inactive, base->no_endpoints_marked_up, base->no_endpoints_marked_down );
                    printf( "%s", logthis );

                }

                break;

            case SCTP_GLOBAL_TRAFFIC_STATS  :
                {
                    sctp_global_traf_stats_st   *base;
                    base = ( sctp_global_traf_stats_st * )stats_data;

                    sprintf( logthis, "stats_data->no_assoc_accepted=%u, ->no_assoc_initiated=%u, ->no_graceful_shutdown_assocs=%u", base->no_assoc_accepted, base->no_assoc_initiated, base->no_graceful_shutdown_assocs );
                    printf( "%s", logthis );

                    sprintf( logthis, "->no_ungraceful_shutdown_assocs=%u, ->no_data_chunks_sent=%u, ->no_data_chunks_unacked=%u , ", base->no_ungraceful_shutdown_assocs, base->no_data_chunks_sent, base->no_data_chunks_unacked );
                    printf( "%s", logthis );

                    sprintf( logthis, "->no_ord_data_chunks_sent=%u, ->no_unord_data_chunks_sent=%u,", base->no_ord_data_chunks_sent, base->no_unord_data_chunks_sent );
                    printf( "%s", logthis );

                    sprintf( logthis, "->no_ord_data_chunks_recv=%u, ->no_unord_data_chunks_recv=%u,", base->no_ord_data_chunks_recv, base->no_unord_data_chunks_recv );
                    printf( "%s", logthis );

                    sprintf( logthis, "->no_data_chunks_recv=%u, ->no_bytes_sent_ok=%u, ->no_bytes_recvd_ok=%u, ->no_init_sent=%u ,", base->no_data_chunks_recv , base->no_bytes_sent_ok , base->no_bytes_recvd_ok , base->no_init_sent );
                    printf( "%s", logthis );

                    sprintf( logthis, "->no_init_recv=%u, ->no_initack_sent=%u, ->no_initack_recv=%u, ->no_cookie_sent=%u ", base->no_init_recv, base->no_initack_sent, base->no_initack_recv, base->no_cookie_sent );
                    printf( "%s", logthis );

                    sprintf( logthis, "->no_cookie_recv=%u, ->no_cookieack_sent=%u, ->no_cookieack_recv=%u, ->no_hb_sent=%u", base->no_cookie_recv, base->no_cookieack_sent, base->no_cookieack_recv, base->no_hb_sent );
                    printf( "%s", logthis );

                    sprintf( logthis, "->no_hb_recv=%u, ->no_hback_sent=%u, ->no_hback_recv=%u, ->no_sack_sent=%u, ", base->no_hb_recv, base->no_hback_sent, base->no_hback_recv, base->no_sack_sent );
                    printf( "%s", logthis );

                    sprintf( logthis, "->no_sack_recv=%u, ->no_abort_sent=%u, ->no_abort_recv=%u, ->no_shut_sent=%u, ", base->no_sack_recv, base->no_abort_sent, base->no_abort_recv, base->no_shut_sent );
                    printf( "%s", logthis );

                    sprintf( logthis, "->no_shut_recv=%u, ->no_shutack_sent=%u, ->no_shutack_recv=%u, ->no_error_sent=%u", base->no_shut_recv, base->no_shutack_sent, base->no_shutack_recv, base->no_error_sent );
                    printf( "%s", logthis );

                    sprintf( logthis, "->no_shutcom_sent=%u, ->no_shutcom_recv=%u,", base->no_shutcom_sent, base->no_shutcom_recv );
                    printf( "%s", logthis );

                    sprintf( logthis, "->no_error_recv=%u, ->no_ecne_sent=%u, ->no_ecne_recv=%u, ->no_cwr_sent=%u ", base->no_error_recv, base->no_ecne_sent, base->no_ecne_recv, base->no_cwr_sent );
                    printf( "%s", logthis );

                    sprintf( logthis, "->no_cwr_recv=%u, ->no_datagrams_recvd_in_error=%u\n\n", base->no_cwr_recv, base->no_datagrams_recvd_in_error );
                    printf( "%s", logthis );

                    sprintf( logthis, "->no_chk_sum_err_recv=%u, ->no_ootb_recv=%u\n\n", base->no_chk_sum_err_recv, base->no_ootb_recv );
                    printf( "%s", logthis );

                    sprintf( logthis, "->no_frag_usr_msg=%u, ->no_reassemble_usr_msg=%u\n\n", base->no_frag_usr_msg, base->no_reassemble_usr_msg );
                    printf( "%s", logthis );

                    sprintf( logthis, "->no_packets_sent=%u, ->no_packets_recv=%u\n\n", base->no_packets_sent, base->no_packets_recv );
                    printf( "%s", logthis );


                }

                break;


            case SCTP_ERROR_STATS      :
                {
                    sctp_error_stats_st        *base;

                    base = ( sctp_error_stats_st * )stats_data;

                    sprintf( logthis, "stats_data->API_error_stats_count=%u, ->protcol_error_stats_count=%u , ->system_error_stats_count=%u , ", base->API_error_stats_count, base->protcol_error_stats_count, base->system_error_stats_count );
                    printf( "%s", logthis );

                    sprintf( logthis, "->system_error_stats=%u, ->system_resource_error_stats_count=%u, ->internal_error_stats_count=%u\n\n", base->system_error_stats_count, base->system_resource_error_stats_count, base->internal_error_stats_count );
                    printf( "%s", logthis );

                }

                break;

            case SCTP_ASSOC_STATS     :
                {
                    sctp_assoc_stats_st          *base;
                    sctp_U32bit count;

                    base = ( sctp_assoc_stats_st * )stats_data;

                    sprintf( logthis, "stats_data->assoc_id=%u, ->no_dest_address=%u\n\n", base->assoc_id, base->no_dest_address );
                    printf( "%s", logthis );


                    for( count = 0; count < base->no_dest_address ; count++ )
                    {
                        sctp_print_sock_addr( ( ( void * )&base->dest_stats[count].dest_address ) );
                        sprintf( logthis, ".no_datagrams_sent=%u , ", base->dest_stats[count].no_datagrams_sent );
                        printf( "%s", logthis );

                        sprintf( logthis, ".no_datagrams_sent=%u , .no_datagrams_unacked=%u, .no_datagrams_recd=%u , ", base->dest_stats[count].no_datagrams_sent, base->dest_stats[count].no_datagrams_unacked, base->dest_stats[count].no_datagrams_recd );
                        printf( "%s", logthis );

                        sprintf( logthis, ".no_bytes_sent=%u, .no_bytes_recvd=%u\n", base->dest_stats[count].no_bytes_sent, base->dest_stats[count].no_bytes_recvd );
                        printf( "%s", logthis );

                    }

                }

                break;

            case SCTP_ALL_STATS  :

            default      :
                sprintf( logthis, "Unrecognized	stats_type\n" );
                printf( "%s", logthis );


        }
    }

}
#endif

#ifdef SCTP_ERROR_ENABLED

void call_sctp_api_get_error_level( sctp_error_flag_et    errorflag )
{
    sctp_return_t                api_return;
    sctp_error_level_et              *error_level;
    sctp_error_t                 *op_error;

    char *logthis;

    logthis = ( char * ) malloc( 160 );
    /* system("/bin/clear"); */
    printf( "\n\n" );

    printf( "Invoked sctp_api_get_error_level\n" );

    printf( "The parameters to be passed are errorflag, *error_level, *op_errror\n" );


    op_error = ( sctp_error_t * )malloc( sizeof( sctp_error_t ) );


    error_level = ( sctp_error_level_et * )malloc( sizeof( sctp_error_level_et ) );

    if( error_level != NULL )
    {
        sprintf( logthis, "errorflag=%u , *error_level=%u", errorflag, *error_level );
    }

    else
    {
        sprintf( logthis, "errorflag=%u , error_level=%p ", errorflag, error_level );
    }

    printf( "%s", logthis );


    if( op_error != NULL )
    {
        sprintf( logthis, "*op_error=%u\n", *op_error );
    }

    else
    {
        sprintf( logthis, "op_error=%p\n", op_error );
    }

    printf( "%s", logthis );


    api_return = sctp_api_get_error_level( errorflag, error_level, op_error );


    if( error_level != NULL )
    {
        sprintf( logthis, "errorflag=%u , *error_level=%u ", errorflag, *error_level );
    }

    else
    {
        sprintf( logthis, "errorflag=%u , error_level=%p , ", errorflag, error_level );
    }

    printf( "%s", logthis );


    if( op_error != NULL )
    {
        sprintf( logthis, "*op_error=%u , return value=%u\n\n", *op_error, api_return );
    }

    else
    {
        sprintf( logthis, "op_error=%p , return value=%u\n", op_error, api_return );
    }

    printf( "%s", logthis );


    free( logthis );

    if( op_error != NULL )
    {
        free( op_error );
    }

    if( error_level != NULL )
    {
        free( error_level );
    }

}

void
call_sctp_api_set_error_level(
    sctp_error_flag_et  errorflag,
    sctp_error_level_et  error_level )
{
    sctp_return_t                api_return;

    sctp_error_t                 *op_error;

    char *logthis;

    logthis = ( char * ) malloc( 160 );

    /* system("/bin/clear"); */
    printf( "\n\n" );

    printf( "Invoked sctp_api_set_error_level\n" );

    printf( "The parameters to be passed are errorflag, error_level, *operrror\n" );

    printf( "Pass NULL for	op_error (y/n) ? " );


    op_error = ( sctp_error_t * )malloc( sizeof( sctp_error_t ) );



    if( op_error != NULL )
    {
        sprintf( logthis, "errorflag=%u , error_level=%u , *op_error=%u\n", errorflag, error_level, *op_error );
    }

    else
    {
        sprintf( logthis, "errorflag=%u , error_level=%u , op_error=%p\n", errorflag, error_level, op_error );
    }

    printf( "%s", logthis );


    api_return = sctp_api_set_error_level( errorflag, error_level, op_error );

    if( op_error != NULL )
    {
        sprintf( logthis, "errorflag=%u , error_level=%u , *op_error=%u , return value=%u\n", errorflag, error_level, *op_error, api_return );
    }

    else
    {
        sprintf( logthis, "errorflag=%u , error_level=%u , op_error=%p , return value=%u\n", errorflag, error_level, op_error, api_return );
    }

    printf( "%s", logthis );


    free( logthis );

    if( op_error != NULL )
    {
        free( op_error );
    }

}
#endif
#ifdef SCTP_DEBUG_ENABLED

void
call_sctp_api_enable_assoc_trc(
    sctp_U32bit          assoc_id,
    sctp_Boolean_t       trc_all_assoc,
    sctp_trc_et              trace_type )
{
    sctp_return_t        api_return;

    sctp_error_t         *errorno;

    char *logthis;

    logthis = ( char * ) malloc( 160 );

    /* system("/bin/clear"); */
    printf( "\n\n" );

    printf( "Invoked sctp_api_enable_assoc_trc\n" );

    printf( "The parameters to be passed are assoc_id, trc_all_assoc, trace_type, *errorno\n" );

    errorno = ( sctp_error_t * )malloc( sizeof( sctp_error_t ) );

    sprintf( logthis, "assoc_id=%u , trc_all_assoc=%u , trace_type=%u ", assoc_id, trc_all_assoc, trace_type );
    printf( "%s", logthis );


    if( errorno != NULL )
    {
        sprintf( logthis, "*errorno=%u\n", *errorno );
    }

    else
    {
        sprintf( logthis, "errorno=%p\n", errorno );
    }

    printf( "%s", logthis );

    if( trc_all_assoc == 1 )
    {
        assoc_id = SCTP_INVALID_ENDPOINT;
    }

    api_return = sctp_api_enable_assoc_trc( assoc_id, trace_type, errorno );

    sprintf( logthis, "assoc_id=%u , trc_all_assoc=%u , trace_type=%u , ", assoc_id, trc_all_assoc, trace_type );
    printf( "%s", logthis );


    if( errorno != NULL )
    {
        sprintf( logthis, "*errorno=%u , return value=%u\n", *errorno, api_return );
    }

    else
    {
        sprintf( logthis, "errorno=%p	, return value=%u\n", errorno, api_return );
    }

    printf( "%s", logthis );


    free( logthis );

    if( errorno != NULL )
    {
        free( errorno );
    }

}

void
call_sctp_api_disable_assoc_trc(
    sctp_U32bit      assoc_id,
    sctp_Boolean_t   stop_trc_all_assoc )
{
    sctp_return_t      api_return;
    sctp_error_t       *errorno;

    char *logthis;

    logthis = ( char * )malloc( 160 );


    /* system("/bin/clear"); */
    printf( "\n\n" );

    printf( "Invoked sctp_api_disable_assoc_trc\n" );

    printf( "The parameters	to be passed are assoc_id, stop_trc_all_assoc, *errorno\n" );

    errorno = ( sctp_error_t * )malloc( sizeof( sctp_error_t ) );

    if( errorno != NULL )
    {
        sprintf( logthis, "assoc_id=%u , stop_trc_all_assoc=%u , *errorno=%u\n", assoc_id, stop_trc_all_assoc, *errorno );
    }

    else
    {
        sprintf( logthis, "assoc_id=%u , stop_trc_all_assoc=%u , errorno=%p\n", assoc_id, stop_trc_all_assoc, errorno );
    }

    printf( "%s", logthis );

    api_return = sctp_api_disable_assoc_trc( assoc_id, stop_trc_all_assoc, errorno );

    if( errorno != NULL )
    {
        sprintf( logthis, "assoc_id=%u , stop_trc_all_assoc=%u , *errorno=%u , return value=%u\n", assoc_id, stop_trc_all_assoc, *errorno, api_return );
    }

    else
    {
        sprintf( logthis, "assoc_id=%u , stop_trc_all_assoc=%u , errorno=%p ,return value=%u\n", assoc_id, stop_trc_all_assoc, errorno, api_return );
    }

    printf( "%s", logthis );


    free( logthis );

    if( errorno != NULL )
    {
        free( errorno );
    }

}


void
call_sctp_api_get_trace_info(
    sctp_U32bit assoc_id )
{

    sctp_U8bit    trace_info;
    sctp_error_t  p_ecode;

    sctp_U8bit    api_return;

    char *logthis;

    logthis = ( char * )malloc( 160 );

    printf( "\n\n" );

    printf( "Invoked sctp_get_trace_info\n" );

    api_return = sctp_api_get_trace_info( assoc_id, &trace_info, &p_ecode );

    sprintf( logthis, "return value=%i\n", api_return );
    printf( "%s", logthis );

    sprintf( logthis, "trace_ifo=%u\n", trace_info );
    printf( "%s", logthis );

    free( logthis );
}
#endif
void
call_sctp_api_init_stack(
    sctp_U32bit       tx_buffers,
    sctp_U32bit       rx_buffers,
    sctp_U32bit       reserve_buffer,
    sctp_U16bit       standard_udp_port,
    sctp_U8bit        *local_addr )
{
    sctp_return_t  ret;
    sctp_sockaddr_list_st  addr_list;
    sctp_sockaddr_st *dest_addr = ( sctp_sockaddr_st * )&addr_list;
    sctp_init_options_st init_options;
    sctp_error_t errorno;
    sctp_U8bit        stale_cookie_policy = 0;
    sctp_U8bit        ack_policy = 2;
    sctp_Boolean_t    ecn_capable = 0;
    sctp_Boolean_t    nat_capable = 0;


    __MEMSET( &init_options, 0, sizeof( init_options ) );
    init_options.MaxPerAssoc.streams = 20;
    init_options.max_appl = 5;
    init_options.max_tcb = 20;
    init_options.max_endpoint = 20;
    init_options.MaxPerAssoc.min_tx_buffers =  tx_buffers;
    init_options.MaxPerAssoc.med_tx_buffers =  tx_buffers;
    init_options.MaxPerAssoc.max_tx_buffers =  tx_buffers;

    init_options.MaxPerAssoc.min_rx_buffers = rx_buffers;
    init_options.MaxPerAssoc.med_rx_buffers = rx_buffers;
    init_options.MaxPerAssoc.max_rx_buffers = rx_buffers;

    init_options.min_reserve_buffers = reserve_buffer;
    init_options.med_reserve_buffers = reserve_buffer;
    init_options.max_reserve_buffers = reserve_buffer;

    init_options.no_of_local_addrs = 1;

    ( ( sctp_sockaddr_st * )( &dest_addr[0] ) )->family = SCTP_AF_INET;

    /* Changes for SPR-21022 Starts here */
    ( ( sctp_sockaddr_st * )( &dest_addr[0] ) )->ip.v4.addr = ntohl( inet_addr( ( const sigtran_8bit * )local_addr ) );
    /* Changes for SPR-21022 Ends here */
    init_options.hostname = NULL;
    init_options.standard_udp_port = standard_udp_port;

    __MEMCOPY( init_options.list_of_local_addrs, dest_addr, sizeof( sctp_sockaddr_list_st ) );
    init_options.capability.stale_cookie_policy = stale_cookie_policy;
    init_options.capability.ack_policy    = ack_policy;
    init_options.capability.ecn_capable = ecn_capable;
    init_options.capability.pmtu_capable = vx_raw_socket_option;
    init_options.capability.stack_type = vx_stack_type;
    init_options.capability.nat_capable = nat_capable;

    printf( "call_sctp_api_init_stack\r\n" );

    ret = sctp_api_init_stack( &init_options, &errorno );

    if( ret == SCTP_FAILURE )
    {
        printf( "ERROR Initializing SCTP !!  \r\n" );
    }

    else
    {
        printf( "SCTP Stack Initialized \r\n" );
    }
}

/* Changes for SPR-21022 Starts here (IPv4) */
void call_sctp_api_deinit_stack( sctp_Boolean_t forced )
{
    sctp_return_t retval = 0;
    sctp_error_t errorno = 0;

    retval = sctp_api_deinit_stack( forced, &errorno );

    if( SCTP_FAILURE == retval )
    {
        printf( "ERROR Deinitializing SCTP Stack\n" );
        printf( "Error Code = %s \r\n" , sctp_error_string[errorno] );
    }

    else
    {
        printf( "SCTP Stack Deinitialized Successfully !\n" );
    }
}

void test_sctp_api_get_rtt_info
(
    sctp_U32bit     assoc_id,
    sctp_Boolean_t  reset_flag )
{
    sctp_return_t           retval = 0;
    sctp_U32bit             num_daddrs = 0;
    sctp_sockaddr_list_st   list_of_dest_addrs;
    sctp_rtt_info_list_st   dest_rtt_param_list;
    sctp_sockaddr_st        dpriaddr;
    sctp_error_t            errorno;
    sctp_U32bit             counter = 0;

    retval = sctp_api_get_rtt_info( assoc_id,
                                    &num_daddrs,
                                    list_of_dest_addrs,
                                    &dest_rtt_param_list,
                                    &dpriaddr,
                                    reset_flag,
                                    &errorno );

    if( SCTP_FAILURE == retval )
    {
        printf( "\nError getting RTT Info !\n" );
        printf( "Error Code = %s \r\n" , sctp_error_string[errorno] );
    }

    else
    {
        for( counter = 0; counter < num_daddrs; counter++ )
        {
            printf( "\nRTT Info retrieved Successfully !\n" );
            printf( "min_rtt = %u\n", dest_rtt_param_list[counter].min_rtt );
            printf( "max_rtt = %u\n", dest_rtt_param_list[counter].max_rtt );
            printf( "last_recorded_rtt = %u\n", dest_rtt_param_list[counter].last_recorded_rtt );
            printf( "srtt = %u\n", dest_rtt_param_list[counter].srtt );
        }
    }
}
/* Changes for SPR-21022 Ends here (IPv4) */



void
call_multi_addr_sctp_api_init_stack(
    sctp_U32bit       tx_buffers,
    sctp_U32bit       rx_buffers,
    sctp_U32bit       reserve_buffer,
    sctp_U16bit       standard_udp_port,
    sctp_U16bit       no_addr,
    sctp_U8bit        *local_addr )
{
    sctp_return_t  ret;
    sctp_init_options_st init_options;
    sctp_error_t errorno;
    sctp_U8bit        stale_cookie_policy = 0;
    sctp_U8bit        ack_policy = 2;
    sctp_Boolean_t    ecn_capable = 0;
    sctp_Boolean_t    nat_capable = 0;
    int i_count = 0;

    sctp_sockaddr_st *p_dest_addr =
        ( sctp_sockaddr_st * ) & ( init_options.list_of_local_addrs );

    /* Changes for SPR-21022 Starts here */
    sigtran_8bit *token = NULL;
    /* Changes for SPR-21022 Ends here */

    /*init_options=(sctp_init_options_st *)malloc(sizeof(sctp_init_options_st));*/
    memset( &init_options, 0, sizeof( sctp_init_options_st ) );
    init_options.MaxPerAssoc.streams = 5;
    init_options.max_appl = 5;
    init_options.max_tcb = 5;
    init_options.max_endpoint = 5;
    init_options.MaxPerAssoc.min_tx_buffers =  tx_buffers;
    init_options.MaxPerAssoc.med_tx_buffers =  tx_buffers;
    init_options.MaxPerAssoc.max_tx_buffers =  tx_buffers;

    init_options.MaxPerAssoc.min_rx_buffers = rx_buffers;
    init_options.MaxPerAssoc.med_rx_buffers = rx_buffers;
    init_options.MaxPerAssoc.max_rx_buffers = rx_buffers;

    init_options.min_reserve_buffers = reserve_buffer;
    init_options.med_reserve_buffers = reserve_buffer;
    init_options.max_reserve_buffers = reserve_buffer;

    init_options.no_of_local_addrs = no_addr;

    /* Changes for SPR-21022 Starts here */
    token = strtok( ( sigtran_8bit * )local_addr, SCTP_DELIM );

    /* Changes for SPR-21022 Ends here */
    for( i_count = 0; i_count < no_addr; i_count++ )
    {
        if( SCTP_FAILURE == sctp_parse_cliaddr_into_sctp_addr(
                    token, &p_dest_addr[i_count] ) )
        {
            printf( "\nError:Failed to Parse Address list\n" );
        }

        token = strtok( NULL, SCTP_DELIM );
    }

    /*((sctp_sockaddr_st *)(&dest_addr[0]))->family = SCTP_AF_INET;
    ((sctp_sockaddr_st *)(&dest_addr[0]))->ip.v4.addr = ntohl(inet_addr(local_addr));*/

    init_options.hostname = NULL;
    init_options.standard_udp_port = standard_udp_port;

    /*__MEMCOPY(init_options.list_of_local_addrs, dest_addr, sizeof(sctp_sockaddr_list_st));*/
    init_options.capability.stale_cookie_policy = stale_cookie_policy;
    init_options.capability.ack_policy    = ack_policy;
    init_options.capability.ecn_capable = ecn_capable;
    init_options.capability.pmtu_capable = vx_raw_socket_option;
    init_options.capability.stack_type = vx_stack_type;
    init_options.capability.nat_capable = nat_capable;

    printf( "call_sctp_api_init_stack\r\n" );

    ret = sctp_api_init_stack( &init_options, &errorno );

    if( ret == SCTP_FAILURE )
    {
        printf( "ERROR Initializing SCTP !!  \r\n" );
    }

    else
    {
        printf( "SCTP Stack Initialized \r\n" );
    }
}
/*These two empty functions have been added for testing purpose only
and should be removed from here and implemented in the application
linked with client code*/

void
sctp_process_api_resp_from_stack(
    sctp_src_id_t    src_id,
    sctp_dest_id_t   dest_id,
    sctp_trans_id_t  tran_id,
    sctp_api_id_t    api_id,
    sctp_void_t      *resp_struct )
{
    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( src_id );
    SCTP_UNUSED_VARIABLE( dest_id );
    SCTP_UNUSED_VARIABLE( tran_id );
    SCTP_UNUSED_VARIABLE( api_id );
    SCTP_UNUSED_VARIABLE( resp_struct );
    /*to avoid the warning*/

    printf( "\nGot response from stack" );
}


void
sctp_process_notf_from_stack(
    sctp_src_id_t    src_id,
    sctp_dest_id_t   dest_id,
    sctp_api_id_t    api_id,
    sctp_void_t      *notf_struct )

{
    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( src_id );
    SCTP_UNUSED_VARIABLE( dest_id );
    SCTP_UNUSED_VARIABLE( api_id );
    SCTP_UNUSED_VARIABLE( notf_struct );
    /*to avoid the warning*/

    printf( "\nGot notification from stack" );
}
#ifdef SCTP_UI_SOCK_API

void  help_sctp_socket( void )
{

    printf( "\tcall_sctp_socket(int   domain,\r\n" );
    printf( "\t                 int   type\r\n" );
    printf( "\t                 int   protocol);\r\n" );
    printf( "\t\tExample:call_sctp_socket(2,1,132)\r\n\n" );

}
void  help_sctp_bind( void )
{

    printf( "\tcall_sctp_bind(int   sd,\r\n" );
    printf( "\t               int   port\r\n" );
    printf( "\t               char  *addr);\r\n" );
    printf( "\t\tExample:call_sctp_bind(1,3, \"192.168.209.214\")\r\n\n" );
}
void  help_sctp_bindx( void )
{

    printf( "\tcall_sctp_bindx(int   sd,\r\n" );
    printf( "\t                int   port\r\n" );
    printf( "\t                int   flag\r\n" );
    printf( "\t                char  *addr);\r\n" );
    printf( "\t\tExample:call_sctp_bindx(1,3, 0, \"192.168.209.214\")\r\n\n" );
}
void  help_sctp_connect( void )
{

    printf( "\tcall_sctp_connect(int   sd,\r\n" );
    printf( "\t                  int   port\r\n" );
    printf( "\t                  char  *addr);\r\n" );
    printf( "\t\tExample:call_sctp_connect(1,3, \"192.168.209.214\")\r\n\n" );
}
void  help_sctp_close( void )
{

    printf( "\tcall_sctp_close(int   sd);\r\n" );
    printf( "\t\tExample:call_sctp_close(1)\r\n\n" );
}
void  help_sctp_listen( void )
{

    printf( "\tcall_sctp_listen(int   sd,\r\n" );
    printf( "\t                 int   backlog);\r\n" );
    printf( "\t\tExample:call_sctp_listen(1,4)\r\n\n" );
}
void  help_sctp_accept( void )
{

    printf( "\tcall_sctp_accept(int   sd);\r\n" );
    printf( "\t\tExample:call_sctp_accept(1)\r\n\n" );
}
void  help_sctp_sendmsg( void )
{

    printf( "\tcall_sctp_sendmsg_init(int   sd,\r\n" );
    printf( "\t                       int   port,\r\n" );
    printf( "\t                       char  *addr,\r\n" );
    printf( "\t                       int   out_stream,\r\n" );
    printf( "\t                       int   in_stream,\r\n" );
    printf( "\t                       int   max_attemts,\r\n" );
    printf( "\t                       int   data_size,\r\n" );
    printf( "\t                       char  *data);\r\n" );
    printf( "\t\tExample:call_sctp_sendmsg_init(1,3,\"192.168.212.214\",10,10,8,4,\"asdf\")\r\n\n" );
    printf( "\n\n" );
    printf( "\tcall_sctp_sendmsg(int   sd,\r\n" );
    printf( "\t                  int   port,\r\n" );
    printf( "\t                  char  *addr,\r\n" );
    printf( "\t                  int   stream,\r\n" );
    printf( "\t                  int   ppid,\r\n" );
    printf( "\t                  int   context,\r\n" );
    printf( "\t                  int   flags,\r\n" );
    printf( "\t                  int   assoc_id,\r\n" );
    printf( "\t                  int   data_size,\r\n" );
    printf( "\t                  char  *data);\r\n" );
    printf( "\t\tExample:call_sctp_sendmsg(1,3,\"192.168.212.214\",1,1,0,1,4,\"asdf\")\r\n\n" );

}
void  help_sctp_sendto( void )
{

    printf( "\tcall_sctp_sendto(int   sd,\r\n" );
    printf( "\t                 int   port,\r\n" );
    printf( "\t                 char  *addr,\r\n" );
    printf( "\t                 int   data_size,\r\n" );
    printf( "\t                 char  *data);\r\n" );
    printf( "\t\tExample:call_sctp_sendto(1,3,\"192.168.212.214\",4,\"asdf\")\r\n\n" );

}
void  help_sctp_send( void )
{

    printf( "\tcall_sctp_send(int   sd,\r\n" );
    printf( "\t               int   data_size,\r\n" );
    printf( "\t               char  *data);\r\n" );
    printf( "\t\tExample:call_sctp_send(1,4,\"asdf\")\r\n\n" );

}
void  help_sctp_recv( void )
{

    printf( "\tcall_sctp_recv(int   sd,\r\n" );
    printf( "\t               int   data_size);\r\n" );
    printf( "\t\tExample:call_sctp_recv(1,100)\r\n\n" );

}
void  help_sctp_recvfrom( void )
{

    printf( "\tcall_sctp_recvfrom(int   sd,\r\n" );
    printf( "\t                   int   data_size);\r\n" );
    printf( "\t\tExample:call_sctp_recvfrom(1,100)\r\n\n" );

}
void  help_sctp_recvmsg( void )
{

    printf( "\tcall_sctp_recvmsg(int   sd);\r\n" );
    printf( "\t\tExample:call_sctp_recvmsg(1)\r\n\n" );

}
void  help_sctp_shutdown( void )
{

    printf( "\tcall_sctp_shutdown(int   sd,\r\n" );
    printf( "\t                   int   how);\r\n" );
    printf( "\t\tExample:call_sctp_shutdown(1,1)\r\n\n" );

}
void  help_sctp_peeloff( void )
{

    printf( "\tcall_sctp_peeloff(int   sd,\r\n" );
    printf( "\t                  int   assoc_id);\r\n" );
    printf( "\t\tExample:call_sctp_peeloff(1,1)\r\n\n" );

}
void  help_sctp_getpaddrs( void )
{

    printf( "\tcall_sctp_getpaddrs(int   sd,\r\n" );
    printf( "\t                    int   assoc_id);\r\n" );
    printf( "\t\tExample:call_sctp_getpaddrs(1,1)\r\n\n" );
    printf( "\t\tNote we calling sctp_freepaddr also inside this function\r\n\n" );

}
void  help_sctp_getladdrs( void )
{

    printf( "\tcall_sctp_getladdrs(int   sd,\r\n" );
    printf( "\t                    int   assoc_id);\r\n" );
    printf( "\t\tExample:call_sctp_getladdrs(1,1)\r\n\n" );
    printf( "\t\tNote we calling sctp_freeladdrs also inside this function\r\n\n" );

}
void  help_sctp_getsockopt( void )
{

    printf( "\tcall_sctp_getsockopt(int   sd,\r\n" );
    printf( "\t                     int   level,\r\n" );
    printf( "\t                     int   optname,\r\n" );
    printf( "\t                     int   assoc_id);\r\n" );
    printf( "\t\tExample:call_sctp_getsockopt(1,132,3,1)\r\n\n" );

}
void  help_sctp_setsockopt( void )
{

    printf( "\tcall_sctp_setsockopt_rto(int   sd,\r\n" );
    printf( "\t                         int   level,\r\n" );
    printf( "\t                         int   rto_init,\r\n" );
    printf( "\t                         int   rto_min,\r\n" );
    printf( "\t                         int   rto_max,\r\n" );
    printf( "\t                         int   assoc_id);\r\n" );
    printf( "\t\tExample:call_sctp_setsockopt_rto(1,132,3,1,60,1)\r\n\n" );
    printf( "\n\n" );
    printf( "\tcall_sctp_setsockopt_general(int   sd,\r\n" );
    printf( "\t                             int   level,\r\n" );
    printf( "\t                             int   optname,\r\n" );
    printf( "\t                             int   value1,\r\n" );
    printf( "\t                             int   value2\r\n" );
    printf( "\t\tExample:call_sctp_setsockopt_general(1,132,3,1,1)\r\n\n" );
    printf( "\n\n" );
    printf( "\tcall_sctp_setsockopt_primary(int   sd,\r\n" );
    printf( "\t                             int   level,\r\n" );
    printf( "\t                             int   optname,\r\n" );
    printf( "\t                             int   assoc_id,\r\n" );
    printf( "\t                             char  *addr\r\n" );
    printf( "\t\tExample:call_sctp_setsockopt_primary(1,132,9,\"192.168.209.214\")\r\n\n" );

}
void  help_sctp_opt_info( void )
{

    printf( "\tcall_sctp_opt_info(int   sd,\r\n" );
    printf( "\t                   int   assoc_id,\r\n" );
    printf( "\t                   int   option,\r\n" );
    printf( "\t                   int   hb_interval,\r\n" );
    printf( "\t                   int   pathmaxrtx,\r\n" );
    printf( "\t                   char  *addr);\r\n" );
    printf( "\t\tExample:call_sctp_opt_info(1,1,0,30,5,\"192.168.212.214\")\r\n\n" );

}
void  help_sctp_getpeername( void )
{

    printf( "\tcall_sctp_getpeername(int   sd);\r\n" );
    printf( "\t\tExample:call_sctp_getpeername(1)\r\n\n" );

}
void  help_sctp_getsockname( void )
{

    printf( "\tcall_sctp_getsockname(int   sd);\r\n" );
    printf( "\t\tExample:call_sctp_getsockname(1)\r\n\n" );

}

void
call_sctp_socket(
    int                  domain,
    int                  type,
    int                  protocol )
{
    int  ret;

    ret = sctp_socket( domain, type, protocol );

    if( ret == -1 )
    {
        printf( "ERROR creating Socket !!  \r\n" );
        sctp_sk_display_error( sk_errno );
    }

    else
    {
        printf( "Socket Created with id %d \r\n", ret );
    }
}

void
call_sctp_bind(
    int                  sd,
    int                  port,
    char                 *addr )
{
    int  ret, addr_len;
    struct sockaddr_in   *local_addr;
    sctp_sockaddr_storage_st  so_addr;

    local_addr = ( struct sockaddr_in * )&so_addr;
    local_addr->sin_family = SCTP_AF_INET;
    local_addr->sin_addr.s_addr = ( inet_addr( addr ) );
    local_addr->sin_port = htons( port );

    addr_len = sizeof( sctp_sockaddr_storage_st );
    ret = sctp_bind( sd, ( void * )local_addr, addr_len );

    if( ret == -1 )
    {
        printf( "ERROR binding Socket !!  \r\n" );
        sctp_sk_display_error( sk_errno );
    }

    else
    {
        printf( "Socket Bound Successfully \r\n" );
    }
}

void
call_sctp_bindx(
    int                  sd,
    int                  port,
    int                  flags,
    char                 *addr )
{
    int  ret, addr_len, naddr;
    struct sockaddr_in   *local_addr;
    sctp_sockaddr_storage_st  so_addr;

    local_addr = ( struct sockaddr_in * )&so_addr;
    local_addr->sin_family = SCTP_AF_INET;
    local_addr->sin_addr.s_addr = ( inet_addr( addr ) );
    local_addr->sin_port = htons( port );
    naddr = 1;

    addr_len = sizeof( sctp_sockaddr_storage_st );
    ret = sctp_bindx( sd, ( void * )local_addr, naddr, flags );

    if( ret == -1 )
    {
        printf( "ERROR in Bindx !!  \r\n" );
        sctp_sk_display_error( sk_errno );
    }

    else
    {
        printf( "Socket Bound Successfully \r\n" );
    }
}

void
call_sctp_close(
    int                  sd )
{
    int  ret;

    ret = sctp_close( sd );

    if( ret == -1 )
    {
        printf( "ERROR in Closing Socket !!  \r\n" );
        sctp_sk_display_error( sk_errno );
    }

    else
    {
        printf( "Socket Closed Successfully \r\n" );
    }
}

void
call_sctp_listen(
    int                  sd,
    int                  backlog )
{
    int  ret;

    ret = sctp_listen( sd, backlog );

    if( ret == -1 )
    {
        printf( "ERROR in Listen !!  \r\n" );
        sctp_sk_display_error( sk_errno );
    }

    else
    {
        printf( "Socket start Listening Successfully \r\n" );
    }
}

void
call_sctp_accept(
    int                  sd )
{
    /* Changes for SPR-21022 Starts here */
    sctp_return_t  ret = -1;
    sctp_socklen_t addr_len = -1;
    /* Changes for SPR-21022 Ends here */
    struct sockaddr_in   *local_addr;
    sctp_sockaddr_storage_st  so_addr;

    local_addr = ( struct sockaddr_in * )&so_addr;
    /*    local_addr->sin_family = SCTP_AF_INET;
        local_addr->sin_addr.s_addr = (inet_addr(addr));
        local_addr->sin_port = htons(port);*/

    addr_len = sizeof( sctp_sockaddr_storage_st );
    /* Changes for SPR-21022 Starts here */
    ret = sctp_accept( sd, ( void * )local_addr, ( sctp_socklen_t * )&addr_len );
    /* Changes for SPR-21022 Ends here */

    /* Changes for SPR-21022 Starts here */
    if( ret == SCTP_FAILURE )
        /* Changes for SPR-21022 Ends here */
    {
        printf( "ERROR accepting Socket !!  \r\n" );
        sctp_sk_display_error( sk_errno );
    }

    else
    {
        printf( "Socket accepted Successfully, new socket id %d\r\n", ret );
    }
}

void
call_sctp_peeloff(
    int                  sd,
    int                  assoc_id )
{
    int  ret;

    ret = sctp_peeloff( sd, assoc_id );

    if( ret == -1 )
    {
        printf( "ERROR in PeelOff Socket !!  \r\n" );
        sctp_sk_display_error( sk_errno );
    }

    else
    {
        printf( "Socket peeloffed Successfully, new socket id %d\r\n", ret );
    }
}

void
call_sctp_connect(
    int                  sd,
    int                  port,
    char                 *addr )
{
    int  ret, addr_len;
    struct sockaddr_in   *local_addr;
    sctp_sockaddr_storage_st  so_addr;

    local_addr = ( struct sockaddr_in * )&so_addr;
#ifdef SCTP_INET6_SUPPORT
    local_addr->sin_family = SCTP_AF_INET6;
#else
    local_addr->sin_family = SCTP_AF_INET;
#endif
    local_addr->sin_addr.s_addr = ( inet_addr( addr ) );
    local_addr->sin_port = htons( port );

    addr_len = sizeof( sctp_sockaddr_storage_st );
    ret = sctp_connect( sd, ( void * )local_addr, addr_len );

    if( ret == -1 )
    {
        printf( "ERROR Connecting Socket !!  \r\n" );
        sctp_sk_display_error( sk_errno );
    }

    else
    {
        printf( "Socket Connected Successfully \r\n" );
    }
}

void
call_sctp_send(
    int                  sd,
    int                  data_size,
    char                 *data )
{
    int  ret;
    int             flags = 0;

    ret = sctp_send( sd, data, data_size, flags );

    if( ret == -1 )
    {
        printf( "ERROR In Send !!  \r\n" );
        sctp_sk_display_error( sk_errno );
    }

    else
    {
        printf( "Send Successful \r\n" );
    }
}

void
call_sctp_sendto(
    int                  sd,
    int                  port,
    char                 *addr,
    int                  data_size,
    char                 *data )
{
    int  ret, addrlen;
    struct sockaddr_in   *local_addr;
    sctp_sockaddr_storage_st  so_addr;
    int             flags = 0;

    local_addr = ( struct sockaddr_in * )&so_addr;
    local_addr->sin_family = SCTP_AF_INET;
    local_addr->sin_addr.s_addr = ( inet_addr( addr ) );
    local_addr->sin_port = htons( port );

    addrlen = sizeof( sctp_sockaddr_storage_st );

    ret = sctp_sendto( sd, data, data_size, flags, ( void * )local_addr, addrlen );

    if( ret == -1 )
    {
        printf( "ERROR In Sendto !!  \r\n" );
        sctp_sk_display_error( sk_errno );
    }

    else
    {
        printf( "Sendto Successful \r\n" );
    }
}

void
call_sctp_sendmsg_init(
    int                  sd,
    int                  port,
    char                 *addr,
    int                  out_stream,
    int                  in_stream,
    int                  max_attempts,
    int                  data_size,
    char                 *data )
{
    int  ret;
    struct sockaddr_in   *local_addr;
    sctp_sockaddr_storage_st  so_addr;
    sctp_msghdr_st  message;
    struct iovec    iovec;
    struct cmsghdr  *p_cmsghdr = NULL;
    sctp_initmsg_st *p_init = NULL;
    int             msg_flags = 0;
    int             strsize = 0;

    local_addr = ( struct sockaddr_in * )&so_addr;
    local_addr->sin_family = SCTP_AF_INET;
    local_addr->sin_addr.s_addr = ( inet_addr( addr ) );
    local_addr->sin_port = htons( port );

    message.msg_name = &so_addr;
    message.msg_namelen = sizeof( sctp_sockaddr_storage_st );

    message.msg_iov = &iovec;
    message.msg_iov->iov_len = data_size;

    if( message.msg_iov->iov_len > 0 )
    {
        message.msg_iov->iov_base = malloc( message.msg_iov->iov_len + 1 );
    }

    else
    {
        message.msg_iov->iov_base = malloc( MAX_DATA_SIZE );
    }

    if( data )
    {
        strsize = strlen( data );

        /* Changes for SPR-21022 Starts here */
        if( strsize > ( sctp_S32bit )message.msg_iov->iov_len )
            /* Changes for SPR-21022 Ends here */
        {
            memcpy( message.msg_iov->iov_base, data, message.msg_iov->iov_len );
        }

        else
        {
            memcpy( message.msg_iov->iov_base, data, strsize );
        }
    }

    message.msg_iovlen = 1;

    message.msg_control = malloc( sizeof( struct cmsghdr ) + sizeof( sctp_sndrcvinfo_st ) );
    p_cmsghdr = ( struct cmsghdr * )message.msg_control;
    p_init = ( ( sctp_initmsg_st * )( ( ( struct cmsghdr * ) message.msg_control ) + 1 ) );
    /*p_init = (sctp_initmsg_st *)(message.msg_control +
                                  sizeof(struct cmsghdr)); */
    p_cmsghdr->cmsg_type = SCTP_INIT;
    p_cmsghdr->cmsg_level = 132;
    message.msg_controllen = sizeof( struct cmsghdr ) +
                             sizeof( sctp_initmsg_st );
    p_cmsghdr->cmsg_len = message.msg_controllen;

    p_init->sinit_num_ostreams = out_stream;
    p_init->sinit_max_instreams = in_stream;
    p_init->sinit_max_attempts = max_attempts;
    p_init->sinit_max_init_timeo = 0;

    ret = sctp_sendmsg( sd, &message, msg_flags );

    if( ret == -1 )
    {
        printf( "ERROR In SendMsg !!  \r\n" );
        sctp_sk_display_error( sk_errno );
    }

    else
    {
        printf( "SendMsg Successful \r\n" );
    }

    free( message.msg_iov->iov_base );
    free( message.msg_control );
}

void
call_sctp_sendmsg(
    int                  sd,
    int                  port,
    char                 *addr,
    int                  stream,
    int                  ppid,
    int                  context,
    int                  flags,
    int                  assoc_id,
    int                  data_size,
    char                 *data )
{
    int  ret;
    struct sockaddr_in   *local_addr = NULL;
    sctp_sockaddr_storage_st  so_addr;
    sctp_msghdr_st  message;
    struct iovec    iovec;
    struct cmsghdr     *p_cmsghdr = NULL;
    sctp_sndrcvinfo_st *p_sndrcv = NULL;
    int             msg_flags = 0;
    int             strsize = 0;

    local_addr = ( struct sockaddr_in * )&so_addr;
    local_addr->sin_family = SCTP_AF_INET;
    local_addr->sin_addr.s_addr = ( inet_addr( addr ) );
    local_addr->sin_port = htons( port );

    message.msg_name = &so_addr;
    message.msg_namelen = sizeof( sctp_sockaddr_storage_st );

    message.msg_iov = &iovec;
    message.msg_iov->iov_len = data_size;

    if( message.msg_iov->iov_len > 0 )
    {
        message.msg_iov->iov_base = malloc( message.msg_iov->iov_len + 1 );
    }

    else
    {
        message.msg_iov->iov_base = malloc( MAX_DATA_SIZE );
    }

    if( data )
    {
        strsize = strlen( data );

        /* Changes for SPR-21022 Starts here */
        if( strsize > ( sctp_S32bit )message.msg_iov->iov_len )
            /* Changes for SPR-21022 Ends here */
        {
            memcpy( message.msg_iov->iov_base, data, message.msg_iov->iov_len );
        }

        else
        {
            memcpy( message.msg_iov->iov_base, data, strsize );
        }
    }

    message.msg_iovlen = 1;

    message.msg_control = malloc( sizeof( struct cmsghdr ) + sizeof( sctp_sndrcvinfo_st ) );
    p_cmsghdr = ( struct cmsghdr * )message.msg_control;
    p_sndrcv = ( ( sctp_sndrcvinfo_st * )( ( ( struct cmsghdr * ) message.msg_control ) + 1 ) );
    /* p_sndrcv = (sctp_sndrcvinfo_st *)(message.msg_control +
                                  sizeof(struct cmsghdr)); */
    p_cmsghdr->cmsg_type = SCTP_SNDRCV;
    p_cmsghdr->cmsg_level = 132;
    message.msg_controllen = sizeof( struct cmsghdr ) +
                             sizeof( sctp_sndrcvinfo_st );
    p_cmsghdr->cmsg_len = message.msg_controllen;

    p_sndrcv->sinfo_stream = stream;
    p_sndrcv->sinfo_ssn = 0;
    p_sndrcv->sinfo_flags = flags;
    p_sndrcv->sinfo_ppid = ppid;
    p_sndrcv->sinfo_context = context;
    p_sndrcv->sinfo_assoc_id = assoc_id;

    ret = sctp_sendmsg( sd, &message, msg_flags );

    if( ret == -1 )
    {
        printf( "ERROR In SendMsg !!  \r\n" );
        sctp_sk_display_error( sk_errno );
    }

    else
    {
        printf( "SendMsg Successful \r\n" );
    }

    free( message.msg_iov->iov_base );
    free( message.msg_control );

}

void
call_sctp_recv(
    int            sd,
    int            data_size )
{
    sctp_U32bit    flags    = 0;
    int            ret      = 0;
    char           msg_str[5 * MAX_DATA_SIZE] = "";
    int            data_count;
    char           *data = NULL;
    sctp_U8bit     *temp = NULL;

    if( data_size > 0 )
    {
        data = malloc( data_size );
    }

    else
    {
        data = malloc( MAX_DATA_SIZE );
        data_size = MAX_DATA_SIZE;
    }

    printf( "\nReceiving data from association ID : %d", sd );

    if( ( ret = sctp_recv( sd, data, data_size, flags ) ) < 0 )
    {
        printf( "\nERROR in receiving msg" );

        sctp_sk_display_error( sk_errno );

        if( data )
        {
            free( data );
        }

        return;
    }


    /* printing the data */

    printf( "\n\nData  Received\n\n" );
    sprintf( msg_str, " DATA SIZE : %d", ret );
    temp = ( sctp_U8bit * )data;

    for( data_count = 0; data_count < ret ; data_count++ )
    {
        if( data_count % 16 == 0 )
        {
            sprintf( msg_str + strlen( msg_str ), "\n" );
        }

        if( data_count % 4 == 0 )
        {
            sprintf( msg_str + strlen( msg_str ), " " );
        }

        sprintf( msg_str + strlen( msg_str ), "%02x ",
                 temp[data_count] );
    }

    printf( "\n\n-----------------------------------------------------"
            "-\n%s\n-----------------------------------------------------"
            "-\n\n", msg_str );

    if( data )
    {
        free( data );
    }

    return;
}

void
call_sctp_recvfrom(
    int            sd,
    int            data_size )
{
    sctp_U32bit    flags    = 0;
    int            ret      = 0;
    char           msg_str[5 * MAX_DATA_SIZE] = "";
    int            data_count;
    struct sockaddr_in        *local_addr = NULL;
    sctp_sockaddr_storage_st  so_addr;
    int            addrlen = sizeof( sctp_sockaddr_storage_st );
    char           *data = NULL;
    sctp_U8bit     *temp = NULL;

    if( data_size > 0 )
    {
        data = malloc( data_size );
    }

    else
    {
        data = malloc( MAX_DATA_SIZE );
        data_size = MAX_DATA_SIZE;
    }

    local_addr = ( struct sockaddr_in * ) & ( so_addr );

    printf( "\nReceiving msg from association ID : %d", sd );

    if( ( ret = sctp_recvfrom( sd, data, data_size, flags, ( void * )local_addr,
                               ( sctp_socklen_t * )&addrlen ) ) < 0 )
    {
        printf( "\nERROR in receiving msg" );

        sctp_sk_display_error( sk_errno );

        if( data )
        {
            free( data );
        }

        return;
    }


    /* printing the data */

    printf( "\n\nData  Received\n\n" );
    sprintf( msg_str, " DATA SIZE : %d", ret );
    temp = ( sctp_U8bit * )data;

    for( data_count = 0; data_count < ret ; data_count++ )
    {
        if( data_count % 16 == 0 )
        {
            sprintf( msg_str + strlen( msg_str ), "\n" );
        }

        if( data_count % 4 == 0 )
        {
            sprintf( msg_str + strlen( msg_str ), " " );
        }

        sprintf( msg_str + strlen( msg_str ), "%02x ",
                 temp[data_count] );
    }

    printf( "\n\n-----------------------------------------------------"
            "-\n%s\n-----------------------------------------------------"
            "-\n\n", msg_str );

    printf( "\n From address " );
    sctp_print_sock_addr( ( void * ) local_addr );

    if( data )
    {
        free( data );
    }

    return;
}

void
call_sctp_recvmsg(
    int            sd )
{
    sctp_msghdr_st message;
    sctp_U32bit    flags    = 0;
    int            ret      = 0;
    struct iovec   iovec;
    struct cmsghdr *p_cmsghdr = NULL;
    sctp_notification_ut *p_ntfy = NULL;
    char           msg_str[5 * MAX_DATA_SIZE] = "";
    int            data_count;
    sctp_U8bit     *temp = NULL;

    message.msg_flags = 0;
    message.msg_name  = malloc( sizeof( sctp_sockaddr_storage_st ) );
    message.msg_namelen = sizeof( sctp_sockaddr_storage_st );
    message.msg_iovlen = 1;
    message.msg_control = malloc( sizeof( sctp_sndrcvinfo_st ) + sizeof( struct cmsghdr ) );
    message.msg_controllen = sizeof( sctp_sndrcvinfo_st ) + sizeof( struct cmsghdr );
    message.msg_iov = &iovec;
    iovec.iov_base = malloc( MAX_DATA_SIZE );
    iovec.iov_len = MAX_DATA_SIZE;

    printf( "\nReceiving msg from association ID : %d", sd );

    if( ( ret = sctp_recvmsg( sd, ( sctp_msghdr_st * )&message, flags ) ) < 0 )
    {
        printf( "\nERROR in receiving msg" );

        sctp_sk_display_error( sk_errno );
        free( message.msg_control );
        free( iovec.iov_base );

        return;
    }

    if( message.msg_flags & SCTP_MSG_NOTIFICATION )
    {
        printf( "Notification received\n" );

        p_ntfy = ( sctp_notification_ut * )message.msg_iov->iov_base;

        switch( p_ntfy->sn_header.sn_type )
        {
            case SCTP_ASSOC_CHANGE:
                printf( "SCTP_ASSOC_CHANGE: " );
                printf( "\n\tsac_type  :  %d", p_ntfy->sn_assoc_change.sac_type );
                printf( "\n\tsac_flags :  %d", p_ntfy->sn_assoc_change.sac_flags );
                printf( "\n\tsac_length:  %d", p_ntfy->sn_assoc_change.sac_length );
                printf( "\n\tsac_assoc_id: %d",
                        p_ntfy->sn_assoc_change.sac_assoc_id );
                printf( "\n\tsac_state :  %d ", p_ntfy->sn_assoc_change.sac_state );
                printf( "\n\tsac_error :  %d", p_ntfy->sn_assoc_change.sac_error );
                printf( "\n\tsac_outbound_stream : %d",
                        p_ntfy->sn_assoc_change.sac_outbound_streams );
                printf( "\n\tsac_inbound_streams: %d",
                        p_ntfy->sn_assoc_change.sac_inbound_streams );
                break;

            case SCTP_PEER_ADDR_CHANGE:
                printf( "\nSCTP_PEER_ADDR_CHANGE:" );
                printf( "\n\tspc_type        :  %d",
                        p_ntfy->sn_padr_change.spc_type );
                printf( "\n\tspc_flags       :  %d",
                        p_ntfy->sn_padr_change.spc_flags );
                printf( "\n\tspc_length      :  %d",
                        p_ntfy->sn_padr_change.spc_length );
                printf( "\n\tspc_assoc_id    :  %d",
                        p_ntfy->sn_padr_change.spc_assoc_id );
                printf( "\n\tspc_state       :  %d",
                        p_ntfy->sn_padr_change.spc_state );
                printf( "\n\tspc_error       :  %d",
                        p_ntfy->sn_padr_change.spc_error );
                printf( "\n\tspc_aaddr:      " );
                sctp_print_sock_addr( ( ( void * )&p_ntfy->sn_padr_change.spc_aaddr ) );
                break;

            case SCTP_REMOTE_ERROR:
                printf( "\nSCTP_REMOTE_ERROR:" );
                printf( "\n\tsre_type    :  %d", p_ntfy->sn_remote_error.sre_type );
                printf( "\n\tsre_flags   :  %d", p_ntfy->sn_remote_error.sre_flags );
                printf( "\n\tsre_length  :  %d",
                        p_ntfy->sn_remote_error.sre_length );
                printf( "\n\tsre_assoc_id:  %d",
                        p_ntfy->sn_remote_error.sre_assoc_id );
                printf( "\n\tsre_error   :  %d", p_ntfy->sn_remote_error.sre_error );
                printf( "\n\tsre_data:" );
                break;

            case SCTP_SEND_FAILED:
                printf( "\nSCTP_SEND_FAILED:" );
                printf( "\n\tssf_type        :  %d",
                        p_ntfy->sn_send_failed.ssf_type );
                printf( "\n\tssf_flags       :  %d",
                        p_ntfy->sn_send_failed.ssf_flags );
                printf( "\n\tssf_length      :  %d",
                        p_ntfy->sn_send_failed.ssf_length );
                printf( "\n\tssf_assoc_id    :  %d",
                        p_ntfy->sn_send_failed.ssf_assoc_id );
                printf( "\n\tssf_error       :  %d",
                        p_ntfy->sn_send_failed.ssf_error );
                printf( "\n\tssf_info        :  %d",
                        p_ntfy->sn_send_failed.ssf_info );
                printf( "\n\tssf_data        :  " );
                break;

            case SCTP_SHUTDOWN_EVENT:
                printf( "\nSCTP_SHUTDOWN_EVENT" );
                printf( "\n\tsse_type        : %d",
                        p_ntfy->sn_shutdown_event.sse_type );
                printf( "\n\tsse_flags       : %d",
                        p_ntfy->sn_shutdown_event.sse_flags );
                printf( "\n\tsse_length      : %d",
                        p_ntfy->sn_shutdown_event.sse_length );
                printf( "\n\tsse_assoc_id    : %d",
                        p_ntfy->sn_shutdown_event.sse_assoc_id );
                break;

            case SCTP_ADAPTION_EVENT:
                printf( "\nSCTP_ADAPTION_EVENT" );
                printf( "\n\tsai_type        : %d",
                        p_ntfy->sn_adaption_event.sai_type );
                printf( "\n\tsai_flags       : %d",
                        p_ntfy->sn_adaption_event.sai_flags );
                printf( "\n\tsai_length      : %d",
                        p_ntfy->sn_adaption_event.sai_length );
                printf( "\n\tsai_assoc_id    : %d",
                        p_ntfy->sn_adaption_event.sai_assoc_id );
                printf( "\n\tsai_adaption_ind: 0x%x",
                        p_ntfy->sn_adaption_event.sai_adaptation_bits );
                break;

            case SCTP_PARTIAL_DELIVERY:
                printf( "\nSCTP_ADAPTION_EVENT" );
                printf( "\n\tsse_type        : %d",
                        p_ntfy->sn_rcv_pdapi_event.pdapi_type );
                printf( "\n\tsse_flags       : %d",
                        p_ntfy->sn_rcv_pdapi_event.pdapi_flags );
                printf( "\n\tsse_length      : %d",
                        p_ntfy->sn_rcv_pdapi_event.pdapi_length );
                printf( "\n\tsse_assoc_id    : %d",
                        p_ntfy->sn_rcv_pdapi_event.pdapi_assoc_id );
                printf( "\n\tsse_adaption_ind: 0x%x",
                        p_ntfy->sn_rcv_pdapi_event.pdapi_indication );
                break;

            default:
                printf( "Invalid notification type received from  stack: error\n" );
        }

        printf( "\n" );
    }

    else
    {
        /* process the data received. */

        p_cmsghdr = ( struct cmsghdr * )message.msg_control;

        if( p_cmsghdr )
        {
            printf( "\n\tThe CMSGHDR info :\n" );

            switch( p_cmsghdr->cmsg_type )
            {
                case SCTP_INIT:
                    {
                        printf( "\n\tcmsg_level       : %d\n", p_cmsghdr->cmsg_level );
                        printf( "\n\tcmsg_type        : %d\n", p_cmsghdr->cmsg_type );

                        printf( "\n\tcmsg_data        : SCTP_INIT\n" );
                    }
                    break;

                case SCTP_SNDRCV:
                    {
                        sctp_sndrcvinfo_st *p_info =
                            ( sctp_sndrcvinfo_st * )( ( char * )message.msg_control +
                                                      sizeof( struct cmsghdr ) );
                        printf( "\n\tcmsg_level       : %d\n", p_cmsghdr->cmsg_level );
                        printf( "\n\tcmsg_type        : %d\n", p_cmsghdr->cmsg_type );

                        printf( "\n\tcmsg_data        : SCTP_SNDRCV" );
                        printf( "\n\t\tsinfo_stream  : %u", p_info->sinfo_stream );
                        printf( "\n\t\tsinfo_ssn     : %u", p_info->sinfo_ssn );
                        printf( "\n\t\tsinfo_flags   : %u", p_info->sinfo_flags );
                        printf( "\n\t\tsinfo_ppid    : %u", p_info->sinfo_ppid );
                        printf( "\n\t\tsinfo_context : %u", p_info->sinfo_context );
                        printf( "\n\t\tsinfo_assoc_id: %u\n\n",
                                p_info->sinfo_assoc_id );
                    }
                    break;

                default:
                    printf( "\n\tInvalid cmsgtype\n" );
            }
        }


        /* printing the data */

        printf( "\n\nData  Received\n\n" );
        sprintf( msg_str, " DATA SIZE : %d", iovec.iov_len );
        temp = ( sctp_U8bit * )iovec.iov_base;

        /* Changes for SPR-21022 Starts here */
        for( data_count = 0; data_count < ( sctp_S32bit )iovec.iov_len ; data_count++ )
            /* Changes for SPR-21022 Ends here */
        {
            if( data_count % 16 == 0 )
            {
                sprintf( msg_str + strlen( msg_str ), "\n" );
            }

            if( data_count % 4 == 0 )
            {
                sprintf( msg_str + strlen( msg_str ), " " );
            }

            sprintf( msg_str + strlen( msg_str ), "%02x ",
                     temp[data_count] );
        }

        printf( "\n\n----------------------------------------------------"
                "-\n%s\n----------------------------------------------------"
                "-\n\n", msg_str );

        printf( "\n\n From address " );
        sctp_print_sock_addr( ( void * ) message.msg_name );


    }

    free( message.msg_name );
    free( message.msg_control );
    free( iovec.iov_base );

    return;

}

void
call_sctp_notification_on(
    int                  sd )
{
    int  ret;
    int  optlen;
    int  level = IPPROTO_SCTP;
    sctp_event_subscribe_st sctp_events;

    sctp_events.sctp_data_io_event = 1;
    sctp_events.sctp_association_event = 1;
    sctp_events.sctp_address_event =  1;
    sctp_events.sctp_send_failure_event   = 1;
    sctp_events.sctp_peer_error_event  = 1;
    sctp_events.sctp_adaption_layer_event = 1;
    sctp_events.sctp_shutdown_event  = 1;
    sctp_events.sctp_partial_delivery_event = 1;


    optlen = sizeof( sctp_event_subscribe_st );

    ret = sctp_setsockopt( sd, level, SCTP_SO_SET_EVENTS, &sctp_events, optlen );

    if( ret == -1 )
    {
        printf( "ERROR in setting Socket call_sctp_notification_on Option !!  \r\n" );
        sctp_sk_display_error( sk_errno );
    }

    else
    {
        printf( "Socket option call_sctp_notification_on set Successfully \r\n" );
    }

}


void call_sctp_sctp_send(
    sctp_U32bit assoc_id,
    sctp_U8bit  *p_data_payload,
    sctp_U32bit  data_size,
    sctp_U32bit  flags,
    sctp_U32bit  p_sinfo_stream_no,
    sctp_U16bit   p_sinfo_stream_seq_no,
    sctp_U16bit   p_sinfo_flags,
    sctp_U32bit   p_sinfo_ppid,
    sctp_U32bit   p_sinfo_context,
    sctp_U32bit   p_sinfo_timetolive,
    sctp_U32bit   p_sinfo_tsn )


{

    sctp_sndrcvinfo_st sininfo;
    int ret;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( p_sinfo_tsn );
    /*to avoid the warning*/


    sininfo.sinfo_stream = p_sinfo_stream_no;
    sininfo.sinfo_ssn = p_sinfo_stream_seq_no;
    sininfo.sinfo_flags = p_sinfo_flags;
    sininfo.sinfo_ppid = p_sinfo_ppid;
    sininfo.sinfo_context = p_sinfo_context;
    sininfo.sinfo_timetolive = p_sinfo_timetolive;
    sininfo.sinfo_tsn = 0;
    sininfo.sinfo_assoc_id = assoc_id ;

    if( ( ret = sctp_sctp_send( assoc_id, p_data_payload, data_size,
                                &sininfo, flags ) ) < 0 )
    {
        printf( "\nsk_sctp_send: Unable to send data" );
    }

    printf( "\nData sent on Association ID : %lu", assoc_id );
}



void call_sctp_sctp_sendx(
    sctp_U32bit assoc_id,
    sctp_U8bit  *p_data_payload,
    sctp_U32bit  data_size,
    sctp_U8bit   *send_addr,
    int          port,
    sctp_U32bit  flags,
    sctp_U32bit  p_sinfo_stream_no,
    sctp_U16bit  p_sinfo_flags,
    sctp_U32bit  p_sinfo_ppid )
{
    sctp_sndrcvinfo_st sininfo;
    int ret;
    struct sockaddr_in   *local_addr;
    sctp_sockaddr_storage_st  so_addr;

    local_addr = ( struct sockaddr_in * )&so_addr;
    local_addr->sin_family = SCTP_AF_INET;
    /* Changes for SPR-21022 Starts here */
    local_addr->sin_addr.s_addr = ( inet_addr( ( sigtran_8bit * )send_addr ) );
    /* Changes for SPR-21022 Ends here */
    local_addr->sin_port = htons( port );

    sininfo.sinfo_stream = p_sinfo_stream_no;
    sininfo.sinfo_ssn = 0;
    sininfo.sinfo_flags = p_sinfo_flags;
    sininfo.sinfo_ppid = p_sinfo_ppid;
    sininfo.sinfo_context = 0;
    sininfo.sinfo_timetolive = 10;
    sininfo.sinfo_tsn = 0;
    sininfo.sinfo_assoc_id = assoc_id;

    if( ( ret = sctp_sctp_sendx( assoc_id, p_data_payload, data_size,
                                 ( void * )local_addr, 1, &sininfo, flags ) ) < 0 )
    {
        printf( "\nsctp_sendx: Unable to send data" );
    }

    printf( "\nData sent on Association ID : %lu", assoc_id );
}






void
call_sctp_shutdown(
    int          sd,
    int          how )
{
    int  ret;

    ret = sctp_shutdown( sd, how );

    if( ret == -1 )
    {
        printf( "ERROR in Shutdown Socket !!  \r\n" );
        sctp_sk_display_error( sk_errno );
    }

    else
    {
        printf( "Socket Shutdown Successfully \r\n" );
    }
}

void
call_sctp_setsockopt_rto(
    int         sd,
    int         level,
    int         rto_init,
    int         rto_min,
    int         rto_max,
    int         assoc_id )
{
    int                 ret = 0;
    int                 optname = SCTP_SO_RTOINFO;
    void                *optval = NULL;
    size_t              optlen;
    sctp_rtoinfo_st     *p_temp = NULL;

    optlen = sizeof( sctp_rtoinfo_st );
    optval = ( sctp_rtoinfo_st * ) malloc( optlen );
    p_temp = ( sctp_rtoinfo_st * )optval;
    p_temp->srto_initial = rto_init;
    p_temp->srto_max = rto_max;
    p_temp->srto_min = rto_min;
    p_temp->srto_assoc_id = assoc_id;

    if( ( ret = sctp_setsockopt( sd, level, optname, ( char * ) optval,
                                 optlen ) ) < 0 )
    {
        printf( "ERROR in setting Socket Option !!  \r\n" );

        sctp_sk_display_error( sk_errno );

        if( optval )
        {
            free( optval );
        }

        return;
    }

    printf( "\n Socket options set on socket ID %d level = %d optname = "
            "%d optlen = %d  ", sd, level, optname, optlen );

    if( optval )
    {
        free( optval );
    }

    return;
}

int user_value1, user_value2;
void
call_sctp_setsockopt_general(
    int         sd,
    int         level,
    int         optname,
    int         value1,
    int         value2 )
{
    int                 ret = 0;
    void                *optval = NULL;
    void **p_optval =   &optval;
    size_t              optlen;
    size_t              *p_optlen = &optlen;

    switch( optname )
    {

        case SCTP_SO_RTOINFO:
            {
                sctp_rtoinfo_st *p_temp = ( sctp_rtoinfo_st * )
                                          malloc( sizeof( sctp_rtoinfo_st ) );

                p_temp->srto_initial = value1;
                p_temp->srto_max     = value2;
                p_temp->srto_min = user_value1;
                user_value1 = 0;
                p_temp->srto_assoc_id = user_value2;
                user_value2 = 0;
                *p_optval = p_temp;
                *p_optlen = sizeof( sctp_rtoinfo_st );
            }
            break;

        case SCTP_SO_ASSOCINFO:
            {
                sctp_assocparams_st *p_temp = ( sctp_assocparams_st * )
                                              malloc( sizeof( sctp_assocparams_st ) );

                p_temp->sasoc_asocmaxrxt  = value1;
                p_temp->sasoc_cookie_life = value2;
                p_temp->sasoc_assoc_id    = user_value1;
                user_value1 = 0;

                *p_optval = p_temp;
                *p_optlen = sizeof( sctp_assocparams_st );
            }
            break;
            /*
            case SCTP_SO_SET_PRIMARY_ADDR:
                {
                    sctp_setprim_st *p_temp = (sctp_setprim_st*)
                        malloc(sizeof(sctp_setprim_st));

                    *p_optval = p_temp;
                    *p_optlen = sizeof(sctp_setprim_st);
                }
                break;
            case SCTP_SO_SET_PEER_PRIMARY_ADDR:
                {
                    sctp_setpeerprim_st *p_temp = (sctp_setpeerprim_st*)
                        malloc(sizeof(sctp_setpeerprim_st));

                    *p_optval = p_temp;
                    *p_optlen = sizeof(sctp_setpeerprim_st);
                }
                break;
              */

        case SCTP_SO_LINGER:
            {
                sctp_linger_st *p_temp = ( sctp_linger_st * )
                                         malloc( sizeof( sctp_linger_st ) );
                p_temp->l_onoff  = value1;
                p_temp->l_linger = value2;

                *p_optval = p_temp;
                *p_optlen = sizeof( sctp_setpeerprim_st );
            }

            break;

        case SCTP_SO_SET_EVENTS:
            {
                sctp_event_subscribe_st *p_info = malloc( sizeof( sctp_event_subscribe_st ) );
                char *options;
                options = ( char * )value1;

                if( !options )
                {
                    printf( "Events:(io/assoc/xddress/fail/erro/down/layer/part)\n" );
                    return;
                }

                p_info->sctp_data_io_event      =
                    strchr( options, 'i' ) ? 1 : 0;

                p_info->sctp_association_event  =
                    strchr( options, 'a' ) ? 1 : 0;

                p_info->sctp_address_event      =
                    strchr( options, 'x' ) ? 1 : 0;

                p_info->sctp_send_failure_event =
                    strchr( options, 'f' ) ? 1 : 0;

                p_info->sctp_peer_error_event   =
                    strchr( options, 'e' ) ? 1 : 0;

                p_info->sctp_shutdown_event     =
                    strchr( options, 'd' ) ? 1 : 0;

                p_info->sctp_adaption_layer_event   =
                    strchr( options, 'l' ) ? 1 : 0;

                p_info->sctp_partial_delivery_event =
                    strchr( options, 'p' ) ? 1 : 0;

                *p_optval = p_info;
                *p_optlen = sizeof( *p_info );
                break;
            }

        case SCTP_SO_INITMSG:
            {
                sctp_initmsg_st *p_info = malloc( sizeof( sctp_initmsg_st ) );
                printf( "SCTP_SO_INITMSG\n" );
                p_info->sinit_max_instreams = value1;
                p_info->sinit_num_ostreams = value2;
                p_info->sinit_max_attempts = user_value1;

                *p_optval = p_info;
                *p_optlen = sizeof( sctp_initmsg_st );
                break;
            }

            /***********************************/
            /*
            case SCTP_SET_PEER_ADDR_PARAMS:
            {
                sctp_paddrparams_st *p_info = malloc(sizeof(sctp_paddrparams_st));
                printf("SCTP_SET_PEER_ADDR_PARAMS\n");
                p_info->spp_hbinterval = 0;
                p_info->spp_pathmaxrxt = 0;
                p_info->spp_assoc_id   = 0;
                *p_optval = p_info;
                *p_optlen = sizeof(sctp_paddrparams_st);
            }
            break;
            */

        case SCTP_SO_SET_ADAPTION_LAYER:
            {
                sctp_setadaption_st *p_ind = malloc( sizeof( sctp_setadaption_st * ) );
                printf( "SCTP_SET_ADAPTION_LAYER\n" );

                p_ind->ssb_adaption_ind = value1;

                *p_optval = p_ind;
                *p_optlen = sizeof( sctp_setadaption_st );
            }
            break;

            /*
            case SCTP_SO_SET_STREAM_TIMEOUTS:
            {
                sctp_setstrm_timeout_st *p_info = malloc(sizeof(sctp_setstrm_timeout_st));
                printf("SCTP_SET_STREAM_TIMEOUTS\n");
                p_info->ssto_timeout = value1;
                p_info->ssto_streamid_start = value2;
                p_info->ssto_streamid_end   = user_value1;
                p_info->ssto_assoc_id       = user_value2;
                *p_optval = p_info;
                *p_optlen = sizeof(sctp_setstrm_timeout_st);
            }
            break;

            case SCTP_SO_SET_DEFAULT_SEND_PARAM:
            {
                sctp_sndrcvinfo_st *p_info = malloc(sizeof(sctp_sndrcvinfo_st));
                printf("SCTP_SO_SET_DEFAULT_SEND_PARAM\n");

                p_info->sinfo_stream     = value1;
                p_info->sinfo_context    = 0;
                p_info->sinfo_ppid       = value2;
                p_info->sinfo_flags      = 0;
                p_info->sinfo_timetolive = user_value1;
                p_info->sinfo_assoc_id   = user_value2;
                *p_optval = p_info;
                *p_optlen = sizeof(sctp_setstrm_timeout_st);
            }
            break;
            */
            /*******************************************/
        default:
            {
                int *p_on = ( int * ) malloc( sizeof( int ) );

                printf( "\nint value" );
                *p_on = value1;

                *p_optval = p_on;
                *p_optlen = sizeof( int );
            }
            break;
    }



    if( ( ret = sctp_setsockopt( sd, level, optname, ( char * ) optval,
                                 optlen ) ) < 0 )
    {
        printf( "ERROR in setting Socket Option !!  \r\n" );

        sctp_sk_display_error( sk_errno );

        if( optval )
        {
            free( optval );
        }

        return;
    }

    printf( "\n Socket options set on socket ID %d level = %d optname = "
            "%d optlen = %d  ", sd, level, optname, optlen );

    if( optval )
    {
        free( optval );
    }

    return;
}
void
call_sctp_select( int n_fds, int r, int w, int e, int time );
void
call_sctp_select( int n_fds, int r, int w, int e, int time )
{
    sctp_fd_set_st    rfds  ;
    sctp_fd_set_st    wfds;
    sctp_fd_set_st    efds;
    sctp_time_t       timeout = {0, time};
    int   ret         = 0;

    FD_ZERO( &rfds );
    FD_ZERO( &wfds );
    FD_ZERO( &efds );

    if( r )
    {
        FD_SET( r, &rfds ) ;
    }

    if( w )
    {
        FD_SET( w, &wfds ) ;
    }

    if( e )
    {
        FD_SET( e, &efds ) ;
    }

    printf( "\nSelecting for read/write/error:\n" );
    ret = sctp_select( n_fds, ( sctp_fd_set_st * )&rfds,
                       ( sctp_fd_set_st * )&wfds, ( sctp_fd_set_st * )&efds, &timeout );
}
void
call_sctp_setsockopt_primary(
    int         sd,
    int         level,
    int         optname,
    int         assoc_id,
    char        *addr )
{
    int                 ret = 0;
    void                *optval = NULL;
    size_t              optlen;


    /* struct sockaddr_in  *pri_addr; */

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( assoc_id );
    SCTP_UNUSED_VARIABLE( addr );
    /*to avoid the warning*/



    switch( optname )
    {
            /*
            case SCTP_SO_SET_PRIMARY_ADDR :
            {
                sctp_setprim_st  *p_temp = NULL;
                optlen = sizeof(sctp_setprim_st);
                optval = (sctp_setprim_st *) malloc(optlen);
                p_temp = (sctp_setprim_st *)optval;
                pri_addr = (struct sockaddr_in *)&p_temp->ssp_addr;
                pri_addr->sin_family = SCTP_AF_INET;
                pri_addr->sin_addr.s_addr = (inet_addr(addr));
                / * pri_addr->sin_port = htons(port); * /

                p_temp->ssp_assoc_id = assoc_id;
                break;
            }
            case SCTP_SO_SET_PEER_PRIMARY_ADDR :
            {
                sctp_setpeerprim_st  *p_temp = NULL;
                optlen = sizeof(sctp_setpeerprim_st);
                optval = (sctp_setpeerprim_st *) malloc(optlen);
                p_temp = (sctp_setpeerprim_st *)optval;
                pri_addr = (struct sockaddr_in *)&p_temp->sspp_addr;
                pri_addr->sin_family = SCTP_AF_INET;
                pri_addr->sin_addr.s_addr = (inet_addr(addr));
                / * pri_addr->sin_port = htons(port); * /

                p_temp->sspp_assoc_id = assoc_id;

                break;
            }
            */

    }



    if( ( ret = sctp_setsockopt( sd, level, optname, ( char * ) optval,
                                 optlen ) ) < 0 )
    {
        printf( "ERROR in setting Socket Option !!  \r\n" );

        sctp_sk_display_error( sk_errno );

        if( optval )
        {
            free( optval );
        }

        return;
    }

    printf( "\n Socket options set on socket ID %d level = %d optname = "
            "%d optlen = %d  ", sd, level, optname, optlen );

    if( optval )
    {
        free( optval );
    }

    return;
}

void
call_sctp_getsockopt(
    int         sd,
    int         level,
    int         optname,
    int         assoc_id )
{
    int                             ret = 0;
    sctp_socklen_t                  optlen;
    void                            *optval;

    printf( "\nGetting socket options" );
    optval = malloc( 1024 );
    *( int * )optval = assoc_id;

    if( ( ret = sctp_getsockopt( sd, level, optname, ( char * ) optval,
                                 &optlen ) ) < 0 )
    {
        printf( "ERROR in getting Socket Option !!  \r\n" );

        sctp_sk_display_error( sk_errno );

        if( optval )
        {
            free( optval );
        }

        return;
    }

    printf( "\n Socket options  on socket ID %d level = %d optname = "
            "%d optlen = %d  ", sd, level, optname, optlen );

    switch( optname )
    {
        case SCTP_SO_RTOINFO :
            {
                sctp_rtoinfo_st *p_temp;
                p_temp = ( sctp_rtoinfo_st * )optval;
                printf( "\n rto_initial %d\n rto_max %d\n rto_min %d\n rto_assoc_id %d\n",
                        p_temp->srto_initial, p_temp->srto_max, p_temp->srto_min,
                        p_temp->srto_assoc_id );
                break;
            }

        case SCTP_SO_ASSOCINFO :
            {
                sctp_assocparams_st *p_temp;
                p_temp = ( sctp_assocparams_st * )optval;
                printf( "ASSOCINFO\n asocmaxrxt %d\n, assoc_id %d\n, cookie_life %d\n"
                        "lrwnd %d\n npdest %d\n prwnd %d\n",
                        p_temp->sasoc_asocmaxrxt, p_temp->sasoc_assoc_id,
                        p_temp->sasoc_cookie_life, p_temp->sasoc_local_rwnd,
                        p_temp->sasoc_number_peer_destinations, p_temp->sasoc_peer_rwnd );
                break;
            }

        case SCTP_SO_INITMSG :
            {
                sctp_initmsg_st *p_temp;
                p_temp = ( sctp_initmsg_st * )optval;
                printf( "\n sinit_num_ostreams %d\n sinit_num_instreams %d\n "
                        "sinit_max_attempts %d\n sinit_max_init_timeo %d\n",
                        p_temp->sinit_num_ostreams, p_temp->sinit_max_instreams,
                        p_temp->sinit_max_attempts, p_temp->sinit_max_init_timeo );
                break;
            }

            /*
            case SCTP_SO_SET_PRIMARY_ADDR :
            {
                sctp_setprim_st *p_temp;
                p_temp = (sctp_setprim_st *)optval;
                printf("SET_PRIM ssp_assoc_id %d ", p_temp->ssp_assoc_id);
                sctp_print_sock_addr((void *)&p_temp->ssp_addr);
                break;
            }
            case SCTP_SO_SET_PEER_PRIMARY_ADDR :
            {
                sctp_setpeerprim_st *p_temp;
                p_temp = (sctp_setpeerprim_st *)optval;
                printf("PEER_PRIM sspp_assoc_id %d ", p_temp->sspp_assoc_id);
                sctp_print_sock_addr((void *)&p_temp->sspp_addr);
                break;
            }
            */
        case SCTP_SO_STATUS :
            {
                sctp_status_st *p_temp;
                p_temp = ( sctp_status_st * )optval;
                printf( "sstat_state %d\n sstat_rwnd %d\n sstat_unackdata %d,\n"
                        "stat_penddata %d\n", p_temp->sstat_state, p_temp->sstat_rwnd,
                        p_temp->sstat_unackdata, p_temp->sstat_penddata );
                sctp_print_sock_addr( ( void * )&p_temp->sstat_primary.spinfo_address );
                printf( "\n spinfo_state %d\n, spinfo_cwnd %d\n, spinfo_srtt %d\n "
                        "spinfo_rto %d\n", p_temp->sstat_primary.spinfo_state,
                        p_temp->sstat_primary.spinfo_cwnd,
                        p_temp->sstat_primary.spinfo_srtt,
                        p_temp->sstat_primary.spinfo_rto );
                printf( "sstat_instream = %d, stat_ostream = %d\n",
                        p_temp->sstat_instrms, p_temp->sstat_outstrms );
                printf( "sstat_fragmentation %d\n", p_temp->sstat_fragmentation_point );
                printf( "sstat_assoc_id %d\n", p_temp->sstat_assoc_id );
                break;
            }

        case SCTP_SO_LINGER :
            {
                sctp_linger_st *p_temp;
                p_temp = ( sctp_linger_st * )optval;
                printf( "l_onoff %d\n l_linger %d\n", p_temp->l_onoff,
                        p_temp->l_linger );
                break;
            }

        case SCTP_SO_SET_ADAPTION_LAYER:
            {
                sctp_setadaption_st *p_ind =
                    ( sctp_setadaption_st * )optval;

                printf( "ssb_adaption_ind = %d\n", p_ind->ssb_adaption_ind );
                break;

            }

            /*
            case SCTP_SO_SET_STREAM_TIMEOUTS:
            {
                sctp_setstrm_timeout_st *p_info = (sctp_setstrm_timeout_st*)optval;
                printf("timeouts: assoc %d, start %d end %d\n",p_info->ssto_assoc_id,
                    p_info->ssto_streamid_start, p_info->ssto_streamid_end, p_info->ssto_timeout);
                break;
            }
            case SCTP_SO_SET_DEFAULT_SEND_PARAM:
            {
                sctp_sndrcvinfo_st *p_info = (sctp_sndrcvinfo_st*)optval;
                printf("send parameters:\n");
                printf("context %d, flags %d, ppid, stream %d, timetolive %d\n",
                    p_info->sinfo_context, p_info->sinfo_flags, p_info->sinfo_ppid,
                    p_info->sinfo_stream, p_info->sinfo_timetolive);
                break;
            }
            case SCTP_SET_PEER_ADDR_PARAMS:
            */
        case SCTP_GET_PEER_ADDR_PARAMS:
            {
                sctp_paddrparams_st *p_info = ( sctp_paddrparams_st * )optval;
                printf( "PEER_ADDR_PARAMS:\nassoc_id %d\nhbinterval%d\npathmaxrxt%d\n",
                        p_info->spp_assoc_id, p_info->spp_hbinterval,
                        p_info->spp_pathmaxrxt );
                sctp_print_sock_addr( ( sctp_Pvoid_t * )&p_info->spp_address );
                break;
            }

        case SCTP_GET_PEER_ADDR_INFO:
            {
                sctp_paddrinfo_st *p_info = ( sctp_paddrinfo_st * )optval;

                printf( "sd %u, cwnd %d, mtu %d, rto %d, srtt %d state %d\n",
                        p_info->spinfo_assoc_id, p_info->spinfo_cwnd,
                        p_info->spinfo_mtu, p_info->spinfo_rto,
                        p_info->spinfo_srtt, p_info->spinfo_state );

                sctp_print_sock_addr( ( sctp_Pvoid_t * )&p_info->spinfo_address );
                break;
            }

        case SCTP_SO_SET_EVENTS:
            {
                sctp_event_subscribe_st *p_info =
                    ( sctp_event_subscribe_st * )optval;

                printf( "EVENTS: %s %s %s %s %s %s %s %s\n",
                        p_info->sctp_data_io_event ? "data" : "",
                        p_info->sctp_association_event ? "assoc" : "",
                        p_info->sctp_address_event ? "address" : "",
                        p_info->sctp_send_failure_event ? "failure" : "",
                        p_info->sctp_peer_error_event ? "perror" : "",
                        p_info->sctp_shutdown_event ? "shut" : "",
                        p_info->sctp_adaption_layer_event ? "adaption" : "",
                        p_info->sctp_partial_delivery_event ? "pdeliv" : "" );
                break;
            }

        case SCTP_SO_DISABLE_FRAGMENTS:
        case SCTP_SO_AUTOCLOSE:
        case SCTP_SO_NODELAY:
        case SCTP_SO_RCVBUF:
        case SCTP_SO_SNDBUF:
        case SCTP_SO_REUSEADDR:
        case SCTP_SO_NONBLOCKING:
        default:
            {
                int *p_temp;
                p_temp = ( int * )optval;
                printf( "\n optvalue %d", *p_temp );
                break;
            }

    }

    if( optval )
    {
        free( optval );
    }

    return;
}


void
call_sctp_opt_info(
    int            sd,
    int            assoc_id,
    int            option,
    int            hb_interval,
    int            pathmaxrtx,
    char           *addr )
{
    int            ret = 0;
    char           *p_optval = NULL;
    struct  sockaddr_in  *dest_addr = NULL;
    /* Changes for SPR-21022 Starts here */
    sctp_socklen_t size = -1;
    /* Changes for SPR-21022 Ends here */

    switch( option )
    {
        case SCTP_PEER_ADDR_PARAMS :
            {
                sctp_paddrparams_st  *p_temp = NULL;
                p_optval = malloc( sizeof( sctp_paddrparams_st ) );
                p_temp = ( sctp_paddrparams_st * )p_optval;
                dest_addr = ( struct sockaddr_in * )&p_temp->spp_address;
                dest_addr->sin_family = SCTP_AF_INET;
                dest_addr->sin_addr.s_addr = ( inet_addr( addr ) );
                p_temp->spp_hbinterval = hb_interval;
                p_temp->spp_pathmaxrxt = pathmaxrtx;
                p_temp->spp_assoc_id = assoc_id;

                break;
            }

        case SCTP_GET_PEER_ADDR_PARAMS :
            {
                sctp_paddrparams_st  *p_temp = NULL;
                p_optval = malloc( sizeof( sctp_paddrparams_st ) );
                p_temp = ( sctp_paddrparams_st * )p_optval;
                dest_addr = ( struct sockaddr_in * )&p_temp->spp_address;
                dest_addr->sin_family = SCTP_AF_INET;
                dest_addr->sin_addr.s_addr = ( inet_addr( addr ) );
                p_temp->spp_assoc_id = assoc_id;
                break;
            }

        case SCTP_GET_PEER_ADDR_INFO :
            {
                sctp_paddrinfo_st  *p_temp = NULL;
                p_optval = malloc( sizeof( sctp_paddrinfo_st ) );
                p_temp = ( sctp_paddrinfo_st * )p_optval;
                dest_addr = ( struct sockaddr_in * )&p_temp->spinfo_address;
                dest_addr->sin_family = SCTP_AF_INET;
                dest_addr->sin_addr.s_addr = ( inet_addr( addr ) );
                p_temp->spinfo_assoc_id = assoc_id;
                break;
            }
    }

    /* Changes for SPR-21022 Starts here */
    if( ( ret = sctp_opt_info( sd, assoc_id, option, ( void * ) p_optval, ( sctp_socklen_t * )&size ) ) < 0 )
        /* Changes for SPR-21022 Ends here */
    {
        printf( "ERROR in tetting Socket Option info!!  \r\n" );

        sctp_sk_display_error( sk_errno );

        if( p_optval )
        {
            free( p_optval );
        }

        return;
    }

    printf( "\n Socket options  on Socket ID %d assoc_id = %d option = "
            "%d ", sd, assoc_id, option );

    switch( option )
    {
        case SCTP_PEER_ADDR_PARAMS :
            {
                sctp_paddrparams_st  *p_temp;
                p_temp = ( sctp_paddrparams_st * )p_optval;
                printf( "\n spp_hbinterval: %u, spp_pathmaxrxt: %u, spp_assoc_id: %u",
                        p_temp->spp_hbinterval, p_temp->spp_pathmaxrxt, p_temp->spp_assoc_id );
                sctp_print_sock_addr( ( void * )&p_temp->spp_address );
                break;
            }

        case SCTP_GET_PEER_ADDR_PARAMS :
            {
                sctp_paddrparams_st  *p_temp;
                p_temp = ( sctp_paddrparams_st * )p_optval;
                printf( "\n spp_hbinterval: %u, spp_pathmaxrxt: %u, spp_assoc_id: %u",
                        p_temp->spp_hbinterval, p_temp->spp_pathmaxrxt, p_temp->spp_assoc_id );
                sctp_print_sock_addr( ( void * )&p_temp->spp_address );
                break;
            }

        case SCTP_GET_PEER_ADDR_INFO :
            {
                sctp_paddrinfo_st  *p_temp;
                p_temp = ( sctp_paddrinfo_st * )p_optval;
                printf( "\n spinfo_state: %u, spinfo_cwnd: %u, spinfo_srtt: %u, spinfo_rto: %u, spinfo_assoc_id: %u",
                        p_temp->spinfo_state, p_temp->spinfo_cwnd, p_temp->spinfo_srtt, p_temp->spinfo_rto, p_temp->spinfo_assoc_id );
                sctp_print_sock_addr( ( void * )&p_temp->spinfo_address );
                break;
            }
    }

    if( p_optval )
    {
        free( p_optval );
    }

    return;
}

void
call_sctp_getpeername(
    int        sd )
{
    sctp_sockaddr_storage_st        sock_storage;
    sctp_socklen_t                  addrlen = sizeof( sctp_sockaddr_storage_st );
    int                             ret = 0;

    printf( "\nGet PeerName ID : %d", sd );

    if( ( ret = sctp_getpeername( sd, ( struct sockaddr * )&sock_storage,
                                  &addrlen ) ) < 0 )
    {
        printf( "ERROR in getting peerName!!  \r\n" );

        sctp_sk_display_error( sk_errno );

        return;
    }

    printf( "\nPeerName on Association ID : %d", sd );
    sctp_print_sock_addr( ( void * ) &sock_storage );

    return;
}

void
call_sctp_getsockname(
    int        sd )
{
    sctp_sockaddr_storage_st        sock_storage;
    sctp_socklen_t                  addrlen = sizeof( sctp_sockaddr_storage_st );
    int                             ret = 0;

    printf( "\nGet SockName on socketID : %d", sd );

    if( ( ret = sctp_getsockname( sd, ( struct sockaddr * )&sock_storage,
                                  &addrlen ) ) < 0 )
    {
        printf( "ERROR in getting SockName!!  \r\n" );
        sctp_sk_display_error( sk_errno );

        return;
    }

    printf( "\nSockName on Association ID : %d", sd );
    sctp_print_sock_addr( ( void * ) &sock_storage );

    return;
}

void
call_sctp_getpaddrs(
    int       sd,
    int       assoc_id )
{
    struct sockaddr                *p_sock_storage;
    int                             count, ret = 0;

    printf( "\nGet PeerAddrs for Sock ID : %d Assoc ID %d", sd, assoc_id );

    if( ( ret = sctp_getpaddrs( sd, assoc_id, &p_sock_storage ) ) < 0 )
    {
        printf( "ERROR in getting PADDRS!!  \r\n" );
        sctp_sk_display_error( sk_errno );

        return;
    }

    printf( "\nPaddrs on socket ID : %d Assoc_id %d", sd, assoc_id );

    for( count = 0; count < ret; count++ )
    {
        sctp_print_sock_addr( ( void * )( p_sock_storage + count ) );
    }

    /* Free the paddrs */
    sctp_freepaddrs( p_sock_storage );

    return;
}

void
call_sctp_getladdrs(
    int       sd,
    int       assoc_id )
{
    struct sockaddr                *p_sock_storage;
    int                             count, ret = 0;

    printf( "\nGet LocalAddrs for Sock ID : %d AssocID %d", sd, assoc_id );

    if( ( ret = sctp_getladdrs( sd, assoc_id, &p_sock_storage ) ) < 0 )
    {
        printf( "ERROR in getting LADDRS!!  \r\n" );
        sctp_sk_display_error( sk_errno );

        return;
    }

    printf( "\nLaddrs on Sock ID : %d AssocId %d", sd, assoc_id );

    for( count = 0; count < ret; count++ )
    {
        sctp_print_sock_addr( ( void * )( p_sock_storage + count ) );
    }

    /* Free the laddrs */
    sctp_freeladdrs( ( void * )p_sock_storage );

    return;
}


void sctp_sk_display_error(
    int sk_errno )
{
    switch( sk_errno )
    {
        case SK_EAFNOSUPPORT:
            printf( "\nSK_EAFNOSUPPORT:The Address family is not supported" );
            break;

        case SK_EACCES:
            printf( "\nSK_EACCES:The process does not have access -> Not "
                    "initailized " );
            break;

        case SK_EPROTONOSUPPORT:
            printf( "\nSK_EPROTONOSUPPORT:Protocol type not supported" );
            break;

        case SK_EPROTOTYPE:
            printf( "\nSK_EPROTOTYPE:The Socket type is not supported by the "
                    "protocol" );
            break;

        case SK_ENOBUFS:
            printf( "\nSK_ENOBUFS:Insuffient buffers/resources" );
            break;

        case SK_EMFILE:
            printf( "\nSK_EMFILE:Insufficient endpoints->insuff socket "
                    "descriptors " );
            break;

        case SK_ENOTSOCK:
            printf( "\nSK_ENOTSOCK:The id passed is not a valid socket." );
            break;

        case SK_EBADF:
            printf( "\nSK_EBADF:The file descriptor passed is not valid." );
            break;

        case SK_EADDRNOTAVAIL:
            printf( "\nSK_EADDRNOTAVAIL:Address not available on the machine" );
            break;

        case SK_EADDRINUSE:
            printf( "\nSK_EADDRINUSE:Address being used by some other sock" );
            break;

        case SK_EOPNOTSUPP:
            printf( "\nSK_EOPNOTSUPP:Operation not supported: Random errors" );
            break;

        case SK_EALREADY:
            printf( "\nSK_EALREADY:Connection already exists " );
            break;

        case SK_EINPROGRESS:
            printf( "\nSK_EINPROGRESS:Connection is already in progress " );
            break;

        case SK_EISCONN:
            printf( "\nSK_EISCONN:sock is conn mode and is already connected" );
            break;

        case SK_EAGAIN:
            printf( "\nSK_EAGAIN:No Conn waiting to be accepted " );
            break;

        case SK_ENOTCONN:
            printf( "\nSK_ENOTCONN:The Socket is not connected " );
            break;

        case SK_ENOPROTOOPT:
            printf( "\nSK_ENOPROTOOPT:The option is not supported by the "
                    "protocol" );
            break;

        case SK_ECONNRESET:
            printf( "\nSK_ECONNRESET:The Peer reset the connection " );
            break;

        case SK_EDESTADDRREQ:
            printf( "\nSK_EDESTADDRREQ:The Socket is not bound and we want "
                    "to listen " );
            break;

        case SK_EINVAL:
            printf( "\nSK_EINVAL:Invalid  parameter and misc error" );
            break;

        case SK_ECONNREFUSED:
            printf( "\nSK_ECONNREFUSED:The Connection was refused " );
            break;

        case SK_ENOCONN:
            printf( "\nSK_ENOCONN:Connection doesnot exist " );
            break;

        case SK_ESHUTDOWN:
            printf( "\nSK_ESHUTDOWN:Shutdown has received " );
            break;

        default:
            printf( "\nERROR: Unknown SOCKET API Error" );
            break;
    }
}

#endif

#ifdef SCTP_LOAD_TEST

void debug_clock();
void debug_clock()
{
    sctp_time_t curr_time;
    long int nsec[200];
    int a, i;

    for( a = 0; a < 100; a++ )
    {
        for( i = 0; i < 1000; i++ )
        {

        }

        /*clock_gettime(CLOCK_REALTIME, &curr_time);
        nsec[a] = curr_time.tv_usec;*/

    }

    /*  for (a = 0; a< 100; a++)
        {
        printf("\nnsec : %d", nsec[a]);

        }*/




}

void start_task();
void start_task()
{
    /* int tid = sp((FUNCPTR)debug_counter, 0,0,0,0,0,0,0,0,0); */
    if( taskSpawn( "SCTP_DBG",
                   SCTP_RECV_PRIORITY,
                   SCTP_RECV_OPTIONS,
                   SCTP_RECV_STACKSIZE,
                   ( FUNCPTR )debug_counter,
                   0,
                   0,
                   0,
                   0,
                   0,
                   0,
                   0,
                   0,
                   0,
                   0 ) == -1 )
        /* if (tid == -1 ) */
    {
        printf( "\nFailed" );
    }

    /* taskPrioritySet(tid, 1); */

}

void debug_counter();
void debug_counter()
{
    sctp_time_t curr_time;
    struct timeval tm;
    int a = 0, i;

    for( i = 0; i < 60; i++ )
    {

        clock_gettime( CLOCK_REALTIME, &curr_time );
        printf( "\nsec	: %u	    nsec : %u", curr_time.tv_sec, curr_time.tv_usec );

    }


}

void test_debug_counter();
void test_debug_counter()
{
    sctp_time_t curr_time;
    struct timeval tm;
    int a = 0, i;

    tm.tv_sec = 0;
    tm.tv_usec = usec_var;

    for( i = 0; i < 600; i++ )
    {
        clock_gettime( CLOCK_REALTIME, &curr_time );
        printf( "\nsec	: %u	    nsec : %u", curr_time.tv_sec, curr_time.tv_usec );

    }


}
#endif
#ifdef SCTP_LOAD_TEST_VXWORKS

void load_test( int num, int size );
void load_test( int num, int size )
{
    sctp_time_t  start_time, end_time;
    unsigned long usec, count = 0;
    unsigned char  buffer[( 1024 * 1024 ) + 1];
    sctp_U32bit data_size = size;
    sctp_return_t ret;
    sctp_error_t errorno;

    __GET_ABSOLUTE_SYSTIME( &start_time );
    printf( "\nStart sec : %d,   nsec : %d", start_time.tv_sec, start_time.tv_usec );

    for( count = 0; count < num; count++ )
    {
        data_size = size;

        if( count == 0 )        /*First packet*/
        {
            sctp_buffer_set_Ulong( &buffer[0], 3 );
            data_size = 100;
        }

        else if( count == num - 1 ) /*Last packet*/
        {
            sctp_buffer_set_Ulong( &buffer[0], 4 );
            data_size = 100;
        }

        else
        {
            sctp_buffer_set_Ulong( &buffer[0], 0 );
        }

        ret = sctp_api_send( sd, 0,    0, NULL, data_size, ( sctp_Pvoid_t ) &buffer[0], &errorno );

        if( ret == SCTP_FAILURE )
        {
            printf( "ERROR !! error id =	%s \r\n" , sctp_error_string[errorno] );
        }

        else
        {
            printf( "+" );
        }
    }

    __GET_ABSOLUTE_SYSTIME( &end_time );
    printf( "\nEnd sec : %d,	nsec : %d", end_time.tv_sec, end_time.tv_usec );


    usec = ( ( end_time.tv_sec - start_time.tv_sec ) * 1000000 ) +
           ( end_time.tv_usec - start_time.tv_usec ) / 1000;

    printf( "\nTime taken to send : %u	usec\n", usec );


}
/*Vxworks 6.9 Soak test -S */
void soak_test( int num, int size, int delay );
void soak_test( int num, int size, int delay )
{
    sctp_time_t  start_time, end_time;
    unsigned long usec, count = 0;
    unsigned char      buffer[( 1024 * 1024 ) + 1];
    sctp_U32bit data_size = size;
    sctp_return_t ret;
    sctp_error_t errorno;

    __GET_ABSOLUTE_SYSTIME( &start_time );
    printf( "\nStart sec : %d,   nsec : %d", start_time.tv_sec, start_time.tv_usec );

    for( count = 0; count < num; count++ )
    {
        data_size = size;

        if( count == 0 )                /*First packet*/
        {
            sctp_buffer_set_Ulong( &buffer[0], 3 );
            data_size     = 100;
        }

        else if( count == num - 1 ) /*Last packet*/
        {
            sctp_buffer_set_Ulong( &buffer[0], 4 );
            data_size     = 100;
        }

        else
        {
            sctp_buffer_set_Ulong( &buffer[0], 0 );
        }

        ret = sctp_api_send( sd, 0,    0, NULL, data_size, ( sctp_Pvoid_t ) &buffer[0], &errorno );

        if( ret == SCTP_FAILURE )
        {
            printf( "ERROR !! error id = %s \r\n" , sctp_error_string[errorno] );
        }

        else
        {
            /*  printf("+sleep %d sec\n",delay);*/
            sleep( delay );
        }
    }

    __GET_ABSOLUTE_SYSTIME( &end_time );
    printf( "\nEnd sec : %d,    nsec : %d", end_time.tv_sec, end_time.tv_usec );


    usec = ( ( end_time.tv_sec - start_time.tv_sec ) * 1000000 ) +
           ( end_time.tv_usec - start_time.tv_usec ) / 1000;

    printf( "\nTime taken to send : %u  usec\n", usec );


}

#endif

void soak_burst_test( unsigned long num, int size, int delay, unsigned long num_iter );
void soak_burst_test( unsigned long num, int size, int delay, unsigned long num_iter )
{
    sctp_time_t  start_time, end_time;
    unsigned long usec, count = 0, count1 = 0;
    unsigned char      buffer[( 1024 * 1024 ) + 1];
    sctp_U32bit data_size = size;
    sctp_return_t ret;
    sctp_error_t errorno;

    __GET_ABSOLUTE_SYSTIME( &start_time );
    printf( "\nStart sec : %d,   nsec : %d", start_time.tv_sec, start_time.tv_usec );

    for( count1 = 0; count1 < num_iter; count1++ )
    {
        for( count = 0; count < num; count++ )
        {
            data_size = size;

            if( count == 0 )                /*First packet*/
            {
                sctp_buffer_set_Ulong( &buffer[0], 3 );
                data_size     = 100;
            }

            else if( count == num - 1 ) /*Last packet*/
            {
                sctp_buffer_set_Ulong( &buffer[0], 4 );
                data_size     = 100;
            }

            else
            {
                sctp_buffer_set_Ulong( &buffer[0], 0 );
            }

            ret = sctp_api_send( sd, 0,    0, NULL, data_size, ( sctp_Pvoid_t ) &buffer[0], &errorno );

            if( ret == SCTP_FAILURE )
            {
                printf( "ERROR !! error id = %s \r\n" , sctp_error_string[errorno] );
            }
        }

        /*  printf("+sleep %d sec\n",delay);*/
        sleep( delay );
    }

    __GET_ABSOLUTE_SYSTIME( &end_time );
    printf( "\nStart sec : %d,   nsec : %d", start_time.tv_sec, start_time.tv_usec );
    printf( "\nEnd sec : %d,    nsec : %d", end_time.tv_sec, end_time.tv_usec );


    usec = ( ( end_time.tv_sec - start_time.tv_sec ) * 1000000 ) +
           ( end_time.tv_usec - start_time.tv_usec ) / 1000;

    printf( "\nTime taken to send : %u  usec\n", usec );


}


/*Vxworks 6.9 Soak test -E */
#ifdef LOAD_TCP_TEST
void load_tcp()
{
    char server_ip[20] = "139.85.238.29";
    sctp_U32bit server_socket, connected_socket;
    unsigned num_bytes = 1000000;
    /*struct sockaddr_in my_addr;*/
    sctp_sockaddr_st addr;
    struct sockaddr remote_addr;
    int remote_addr_len = sizeof( struct sockaddr );
    char *buffer;
    sctp_return_t ret;



    /* server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (server_socket == ERROR)*/

    if( sctp_TL_create_socket( AF_INET, SOCK_STREAM, IPPROTO_TCP, &server_socket ) == SCTP_FAILURE )
    {
        perror( "\nError	creating socket	" );
        return;
    }


    printf( "\nServer socket created with ID %u", server_socket );


    /*
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(10024);
    my_addr.sin_addr.s_addr = inet_addr(&server_ip[0]);*/


    printf( "\nBinding Server socket on port 9238" );

    /*if (bind(server_socket, (struct sockaddr *)&my_addr, sizeof(my_addr)) == ERROR)*/

    addr.address_type = SCTP_IPv4_ADDR;
    addr.network_address = htonl( inet_addr( &server_ip[0] ) );

    ret  = sctp_TL_bind( server_socket, &addr, 9238 );

    if( ret == SCTP_FAILURE )

    {
        perror( "\nError	binding	socket" );
        return;
    }

    printf( "\nServer socket bound on port 9238" );

    if( listen( server_socket, 10 ) == ERROR )
    {
        perror( "\nError	listening new connections" );
        return;
    }

    printf( "\nServer socket waiting for	new connections" );

    connected_socket = accept( server_socket, ( struct sockaddr * )&remote_addr, &remote_addr_len );

    if( connected_socket == ERROR )
    {
        perror( "\nError	accepting new connection" );
        return;
    }

    printf( "\nGot new connection" );
    buffer = ( char * )malloc( num_bytes );

    while( 1 )
    {
        printf( "\nSending %u bytes ", num_bytes );

        num_bytes = send( connected_socket, ( void * )buffer, num_bytes, 0 );

        if( num_bytes == -1 )
        {
            perror( "\nError	sending	data " );
            continue;
        }

        printf( "\nNumber of bytes sent : %u", num_bytes );
    }
}
#endif /*endif LOAD_TCP*/
#endif

#ifndef VXWORKS_6_9_SUPPORT
S32bit conf_ip_converter( char *p_dottedIP )
{

    U32bit DecimalIP = 0, Component;

    /* Changes for SPR-21022 Starts here */
    sigtran_8bit *p_Component = NULL;
    /* Changes for SPR-21022 Ends here */

    for( Component = 0; Component <= 3; Component++ )
    {
        if( ( p_Component = strchr( p_dottedIP, '.' ) ) )
        {
            *p_Component = ( char )NULL;
        }

        DecimalIP += ( atoi( p_dottedIP ) << ( ( 3 - Component ) * 8 ) );
        p_dottedIP = p_Component + 1;

    }

    if( Component == 4 )
    {
        return( DecimalIP );
    }

    else
    {
        return( 0 );
    }

}
#endif
/****************************************************************
 moved form s_es.c
 ***************************************************************/

#ifndef SCTP_UI_SOCK_API

void test_sctp_api_register_su()
{
    sctp_return_t ret ;

    sctp_error_t errorno;

    printf( "test_sctp_api_register_su \r\n" );
    ret = sctp_api_register_su( &application_id, callback_func_temp, SCTP_NULL, &errorno );

    if( ret == SCTP_FAILURE )
    {
        printf( "ERROR !! error id  \r\n" );
    }

    else
    {
        printf( "SUCCESS !! Application Id = %d \r\n"   , application_id );
    }
}

void test_sctp_api_register_su1()
{
    sctp_return_t ret ;

    sctp_error_t errorno;

    printf( "test_sctp_api_register_su \r\n" );
    ret = sctp_api_register_su( &application_id1, callback_func_temp1, SCTP_NULL, &errorno );

    if( ret == SCTP_FAILURE )
    {
        printf( "ERROR !! error id  \r\n" );
    }

    else
    {
        printf( "SUCCESS !! Application Id = %d \r\n"   , application_id1 );
    }
}

void  test_sctp_api_socket( sctp_U16bit application_id )
{
    sctp_return_t ret ;
    sctp_error_t errorno;

    ret = sctp_api_socket( application_id, SCTP_AF_INET, &sd , &errorno );

    if( ret == SCTP_FAILURE )
    {
        printf( "ERROR !! error id	= %s \r\n" , sctp_error_string[errorno] );
    }

    else
    {
        printf( "SUCCESS !! Association Id	for Socket = %u \r\n ", sd );
    }
}

void  test_sctp_api_socket_inet6( sctp_U16bit application_id )
{
    sctp_return_t ret ;
    sctp_error_t errorno;

    ret = sctp_api_socket( application_id, SCTP_AF_INET6, &sd , &errorno );

    if( ret == SCTP_FAILURE )
    {
        printf( "ERROR !! error id	= %s \r\n" , sctp_error_string[errorno] );
    }

    else
    {
        printf( "SUCCESS !! Association Id	for Socket = %u \r\n ", sd );
    }
}

void  test_sctp_api_bind( sctp_U32bit sockid,
                          sctp_U16bit port,
                          char        *addr )
{
    sctp_return_t ret ;

    sctp_U32bit num_addrs;
    sctp_error_t errorno;
    sctp_sockaddr_st bind_addr_list[10];

    num_addrs = 1;

    bind_addr_list[0].family = SCTP_AF_INET;
    bind_addr_list[0].ip.v4.addr = ntohl( inet_addr( addr ) );
    ret = sctp_api_bind( sockid ,
                         port,
                         ( sctp_U32bit ) num_addrs, /* num of addrs */
                         bind_addr_list,/* addr list */
                         NULL,/* hostname */
                         &errorno );

    if( ret == SCTP_FAILURE )
    {
        printf( "ERROR !! " );
    }

    else
    {
        printf( "SUCCESS !! Bind on SCTP port = %hu \r\n ", sctp_port );
    }

}

void  test_multi_addr_sctp_api_bind( sctp_U32bit sockid,
                                     sctp_U16bit port,
                                     int no_addr,
                                     char        *local_addr )
{
    sctp_return_t ret ;

    sctp_error_t errorno;
    sctp_sockaddr_list_st bind_addr_list;
    char *token = NULL;
    int i_count = 0;

    token = strtok( local_addr, SCTP_DELIM );

    for( i_count = 0; i_count < no_addr; i_count++ )
    {
        if( SCTP_FAILURE == sctp_parse_cliaddr_into_sctp_addr(
                    token,
                    ( sctp_sockaddr_st * )&bind_addr_list[i_count] ) )
        {
            printf( "\nError:Failed to Parse Address list\n" );
        }

        token = strtok( NULL, SCTP_DELIM );
    }

    /*bind_addr_list[0].family = SCTP_AF_INET;
    bind_addr_list[0].ip.v4.addr = ntohl(inet_addr(addr));*/
    ret = sctp_api_bind( sockid ,
                         port,
                         ( sctp_U32bit ) no_addr, /* num of addrs */
                         bind_addr_list,/* addr list */
                         NULL,/* hostname */
                         &errorno );

    if( ret == SCTP_FAILURE )
    {
        printf( "ERROR !! " );
    }

    else
    {
        printf( "SUCCESS !! Bind on SCTP port = %hu \r\n ", sctp_port );
    }

}

void  test_sctp_api_accept( sctp_U32bit par_sd )
{

    sctp_return_t ret ;
    sctp_U32bit newsd,  ppi, no_of_dest_addr;
    sctp_error_t errorno;
    sctp_sockaddr_list_st addr_list;


    ppi = 1;
    ret = sctp_api_accept( par_sd,
                           ppi,
                           &no_of_dest_addr,
                           addr_list,
                           &sctp_port,
                           &newsd,
                           block,
                           &errorno );


    if( ret == SCTP_FAILURE )
    {
        printf( "ERROR !! error id	= %s \r\n" , sctp_error_string[errorno] );
    }

    else
    {
        printf( "SUCCESS !! A new connection , new	association id %u \r\n", newsd );
        sd = newsd;
    }

}

void  test_sctp_api_connect( sctp_U32bit sockid,
                             sctp_U16bit port,
                             char        *addr )
{
    sctp_return_t   ret ;
    sctp_U32bit ppi = 1;
    sctp_error_t errorno;
    sctp_sockaddr_st to_addr;

    /* Changes for SPR-21022 Starts here (IPv6) */
    sctp_Boolean_t block = 0;

    /* Changes for SPR-21022 Ends here (IPv6) */
    /*  to_addr.family = SCTP_AF_INET;
      to_addr.ip.v4.addr = ntohl(inet_addr(addr));*/
    if( SCTP_FAILURE == sctp_parse_cliaddr_into_sctp_addr(
                addr, &to_addr ) )
    {
        printf( "test_sctp_api_connect:Couldnot convert the "
                "INET6 address\n" );
    }

    ret = sctp_api_connect( sockid,
                            port,
                            NULL ,
                            NULL,
                            &to_addr,
                            ppi,
                            block,
                            &errorno );

    if( ret == SCTP_FAILURE )
        /* Changes for SPR-21022 Starts here (IPv6) */
    {
        printf( "ERROR !! error id = %s \r\n" , sctp_error_string[errorno] );
        printf( "\nconnect failed !\n" );
    }

    /* Changes for SPR-21022 Ends here (IPv6) */
    else
    {
        printf( "SUCCESS !! Connected to Peer Port %d\r\n ", sctp_port );
    }

}

void test_sctp_api_send( sctp_U32bit par_sd )
{
    sctp_return_t ret ;
    sctp_U32bit i;
    sctp_error_t errorno;
    unsigned char buffer[2048];

    /* Changes for SPR-21022 Starts here */
    strcpy( ( sigtran_8bit * )&buffer[0], "Test1 string" );
    /* Changes for SPR-21022 Ends here */


    for( i = 0; i < 100; i++ )
        if( buffer[i] == '\0' )
        {
            break;
        }

    ret = sctp_api_send( par_sd,
                         0, /* context */
                         0,     /* stream id */
                         NULL, /* dest addr    */
                         i,        /* data size */
                         ( sctp_Pvoid_t ) &buffer[0], /* data */
                         &errorno );      /*error */

    if( ret == SCTP_FAILURE )
    {
        printf( "ERROR !! error id = %s \r\n" , sctp_error_string[errorno] );
    }

    else
    {
        printf( "SUCCESS !!  data sent \r\n" );
    }

}

/* Changes for SPR-21022 Starts here */
/* Function to receive data on Vxworks-Side SCTP Stack */
void
test_sctp_api_recv(
    sctp_assoc_t                assoc_id,
    sctp_U32bit                 data_size,
    sctp_Boolean_t              block )
{
    int            retval      = 0;
    char           msg_str[5 * MAX_DATA_SIZE] = "";
    sctp_U32bit    data_count = 0;
    sctp_U8bit     *data = NULL;
    sctp_U8bit     *temp = NULL;
    sctp_U32bit     ppi;
    sctp_U16bit     stream_id;
    sctp_error_t    errorno;

    if( data_size > 0 )
    {
        data = malloc( data_size * sizeof( sctp_U8bit ) );
    }

    else
    {
        data = malloc( MAX_DATA_SIZE );
        data_size = MAX_DATA_SIZE;
    }

    printf( "\nReceiving data from association ID : %d", sd );


    retval = sctp_api_receive(
                 assoc_id,
                 &ppi,
                 &data_size,
                 &stream_id,
                 data,
                 block,
                 &errorno );
    /*
    sctp_api_receive(
        sctp_U32bit        assoc_id, [IN]
        sctp_U32bit        *p_ppi,
        sctp_U32bit        *p_data_size,    [IN]
        sctp_U16bit        *p_stream_id,
        sctp_U8bit         *p_data,
        sctp_Boolean_t     block, [IN]
        sctp_error_t       *p_ecode)
    */

    if( SCTP_FAILURE == retval )
    {
        printf( "\nsctp_api_receive() FAIL: assoc_id = %d, data_size = %d, block = %d\n",
                assoc_id, data_size, block );

        /* sctp_sk_display_error(sk_errno); */
        if( data )
        {
            free( data );
        }

        return;
    }

    /* printing the data */
    printf( "\nsctp_api_receive() SUCCESS: assoc_id = %d, data_size = %d, block = %d\n",
            assoc_id, data_size, block );

    printf( "\n\nData  Received\n\n" );
    sprintf( msg_str, " DATA SIZE : %d", data_size );
    temp = ( sctp_U8bit * )data;

    for( data_count = 0; data_count < data_size ; data_count++ )
    {
        if( data_count % 16 == 0 )
        {
            sprintf( msg_str + strlen( msg_str ), "\n" );
        }

        if( data_count % 4 == 0 )
        {
            sprintf( msg_str + strlen( msg_str ), " " );
        }

        sprintf( msg_str + strlen( msg_str ), "%02x ",
                 temp[data_count] );
        sprintf( msg_str + strlen( msg_str ), "%c ",
                 temp[data_count] );
    }

    printf( "\n\n-----------------------------------------------------"
            "-\n%s\n-----------------------------------------------------"
            "-\n\n", msg_str );

    if( data )
    {
        free( data );
    }

    return;
}

void test_sctp_config_assoc_ctl
(
    sctp_U32bit          assoc_id,
    sctp_prot_params_et  prot_parameter,
    sctp_U32bit          parameter_value );

void test_sctp_config_assoc_ctl
(
    sctp_U32bit          assoc_id,
    sctp_prot_params_et  prot_parameter,
    sctp_U32bit          parameter_value )
{
    sctp_U32bit ret_val = SCTP_FAILURE;

    sctp_error_t    errorno;

    ret_val = sctp_api_config_assoc_ctl
              ( assoc_id, prot_parameter, parameter_value, &errorno );

    if( SCTP_FAILURE == ret_val )
    {
        printf( "FAILURE !! error id = %s\n", sctp_error_string[errorno] );
    }

    else
    {
        printf( "\nsuccess\n" );
    }
}

void test_sctp_get_dest_status( sctp_assoc_t association_id );

void test_sctp_get_dest_status( sctp_assoc_t association_id )
{
    sctp_return_t                   ret_val = SCTP_FAILURE;
    sctp_U32bit                     nos_of_dest_trans_addrs;
    sctp_sockaddr_list_st           list_of_dest_trans_addrs;
    sctp_dest_status_param_list_st  dest_param_list;
    sctp_sockaddr_st                primary_dest_transport_addr;
    sctp_error_t                     errorno;
    sctp_U32bit                     counter = 0;


    ret_val = sctp_api_get_dest_addr_status
              ( association_id,
                &nos_of_dest_trans_addrs,
                list_of_dest_trans_addrs,
                ( sctp_dest_status_param_list_st * )&dest_param_list[0],
                &primary_dest_transport_addr,
                &errorno );

    if( SCTP_FAILURE == ret_val )
    {
        printf( "sctp_api_get_addr_status: FAILURE !! error id = %s\n",
                sctp_error_string[errorno] );
    }

    else
    {
        printf( "sctp_api_get_dest_addr_status: SUCCESS!!" );
        printf( "No. of dest trans addrs = %d\n", nos_of_dest_trans_addrs );

        for( counter = 0; counter < nos_of_dest_trans_addrs; counter++ )
        {
            printf( "\nlist_of_dest_trans_addrs family = %d\n",
                    list_of_dest_trans_addrs[counter].family );
            printf( "\nlist_of_dest_trans_addrs family = %u\n",
                    list_of_dest_trans_addrs[counter].ip.v4.addr );
        }

        printf( "\nprimary dest transport addr family = %d\n",
                primary_dest_transport_addr.family );
        printf( "\nprimary dest transport addr = %u\n",
                primary_dest_transport_addr.ip.v4.addr );
    }
}
/* Changes for SPR-21022 Ends here */

void test_sctp_api_close( sctp_U32bit par_sd )
{
    sctp_return_t ret ;
    sctp_error_t errorno;

    ret = sctp_api_close( par_sd,
                          graceful,
                          &errorno );

    if( ret == SCTP_FAILURE )
    {
        printf( "ERROR !! error id = %s \r\n" , sctp_error_string[errorno] );
    }

    else
    {
        printf( "SUCCESS!!" );
    }

}

int backlog = 4;
void test_sctp_api_listen( sctp_U32bit par_sd )
{
    sctp_return_t ret ;
    sctp_error_t errorno;

    ret = sctp_api_listen( par_sd,
                           backlog,
                           &errorno );

    if( ret == SCTP_FAILURE )
    {
        printf( "ERROR !! error id = %s \r\n" , sctp_error_string[errorno] );
    }

    else
    {
        printf( "SUCCESS!!" );
    }

}

void test_sctp_api_config_txrx_num_frag(
    sctp_U32bit assoc_id,
    sctp_U32bit min_buffer, sctp_U32bit med_buffer,
    sctp_U32bit max_buffer, sctp_Boolean_t is_transmit,
    sctp_Boolean_t is_increment )
{

    sctp_count_frags_st p_count_frags;
    sctp_error_t e_code;

    p_count_frags.min_num_frag = min_buffer;
    p_count_frags.med_num_frag = med_buffer;
    p_count_frags.max_num_frag = max_buffer;

    if( sctp_api_config_txrx_num_frag( assoc_id, p_count_frags,
                                       is_transmit, is_increment, &e_code ) == SCTP_FAILURE )

    {
        printf( "\nERROR: Cannot configure config_txrx_num_frag." );
        return;
    }

    else
    {
        printf( "Assoc %d Configured", assoc_id );
    }
}

void test_sctp_get_txrx_num_frag( sctp_U32bit assoc_id )
{

    sctp_count_frags_st tx_count_frags;
    sctp_count_frags_st rx_count_frags;
    sctp_error_t e_code;

    if( sctp_api_get_free_txrx_num_frag( assoc_id, &tx_count_frags,
                                         &rx_count_frags, &e_code ) == SCTP_FAILURE )
    {
        printf( "\nERROR: Cannot call config_txrx_num_frag." );
    }

    else
    {
        printf( "Information retrieved" );
    }
}


void test_sctp_api_config_cong_reg_def( sctp_U32bit assoc_id,
                                        sctp_U8bit high_cong_lvl_onset, sctp_U8bit high_cong_lvl_abate,
                                        sctp_U8bit med_cong_lvl_onset, sctp_U8bit med_cong_lvl_abate,
                                        sctp_U8bit low_cong_lvl_onset, sctp_U8bit low_cong_lvl_abate,
                                        sctp_U8bit no_cong_lvl_onset, sctp_U8bit no_cong_lvl_abate,
                                        sctp_Boolean_t is_transmit )
{
    sctp_cong_reg_def_st p_cong_reg_def;
    sctp_error_t e_code;

    /*to avoid the warning*/
    SCTP_UNUSED_VARIABLE( no_cong_lvl_onset );
    SCTP_UNUSED_VARIABLE( no_cong_lvl_abate );
    /*to avoid the warning*/

    p_cong_reg_def.high_cong_lvl_onset = high_cong_lvl_onset;
    p_cong_reg_def.high_cong_lvl_abate = high_cong_lvl_abate;
    p_cong_reg_def.med_cong_lvl_onset = med_cong_lvl_onset ;
    p_cong_reg_def.med_cong_lvl_abate = med_cong_lvl_abate ;
    p_cong_reg_def.low_cong_lvl_onset = low_cong_lvl_onset;
    p_cong_reg_def.low_cong_lvl_abate = low_cong_lvl_abate;


    if( sctp_api_config_cong_reg_def( assoc_id, p_cong_reg_def,
                                      is_transmit, &e_code ) == SCTP_FAILURE )
    {
        printf( "\nERROR: Cannot configure config_cong_reg_def." );
    }

    printf( "\nAssociation %d configured", assoc_id );
}


void test_sctp_api_get_cong_reg_def( sctp_U32bit assoc_id,
                                     sctp_Boolean_t is_transmit )
{
    sctp_cong_reg_def_st cong_reg_def;
    sctp_error_t e_code;

    if( sctp_api_get_cong_reg_def( assoc_id, &cong_reg_def, is_transmit, &e_code )
            == SCTP_FAILURE )
    {
        printf( "\nERROR: Cannot configure config_cong_reg_def." );
    }

    printf( "\nAssociation %d configured", assoc_id );

}





void *callback_func_temp1( sctp_U32bit event, void *event_data )
{

    sctp_notice_ut *notify_data = ( sctp_notice_ut * ) event_data;
    printf( "\n\n Reached the callback func temp1 !!! event: %s, global count = %d \n\n",
            event_str[event], global_count++ );

    switch( event )
    {
        case SCTPNOTIFY_OPERROR:
            break;

        case SCTPNOTIFY_DESTSTATCHANGE:
            printf( "Assoc id = %u, dest addr = %u , status = %u\n",
                    notify_data->dststatchange.assoc_id,
                    notify_data->dststatchange.destination_ip_addr.ip.v4.addr,
                    notify_data->dststatchange.status );
            break;

        case SCTPNOTIFY_ENDSTATCHANGE:
            printf( "Assoc id = %u, endpoint status = %u\n",
                    notify_data->endstatchange.assoc_id,
                    notify_data->endstatchange.status );
            break;

        case SCTPNOTIFY_CONGESTION:
            printf( "Assoc id =	%u, congestion level =	%u\n",
                    notify_data->congestion.assoc_id,
                    notify_data->congestion.cong_level );
            break;

        case SCTPNOTIFY_SYSERROR:
            break;

        case SCTPNOTIFY_CONNPENDING:
            printf( "Assoc id = %u \n",
                    notify_data->connpending.assoc_id );
            break;

        case SCTPNOTIFY_DATAPENDING:
            printf( "Assoc id = %u, stream = %d\n",
                    notify_data->datapending.assoc_id,
                    notify_data->datapending.stream );
            break;

        case SCTPNOTIFY_SENDFAILURE:
            printf( "Assoc id = %u, context = %u, cause_code = %u",
                    notify_data->sendfailure.assoc_id,
                    ( sctp_U32bit )notify_data->sendfailure.context,
                    ( sctp_U32bit )notify_data->sendfailure.cause_code );
            break;

        case SCTPNOTIFY_PARTIAL_DATAPENDING:
            printf( "Assoc id = %u, stream = %d\n",
                    notify_data->partial_datapending.assoc_id,
                    notify_data->partial_datapending.stream );
            break;

        case SCTPNOTIFY_RECVCONGESTION:
            printf( "Assoc id =	%u, congestion level =	%d\n",
                    notify_data->recv_congestion.assoc_id,
                    notify_data->recv_congestion.cong_level );
            break;

        case SCTPNOTIFY_SHUTDOWN_EVENT:
            printf( "Assoc id =	%u\n",
                    notify_data->recv_congestion.assoc_id );
            break;

    }

    return NULL;

}

void *callback_func_temp( sctp_U32bit event, void *event_data )
{

    sctp_notice_ut *notify_data = ( sctp_notice_ut * ) event_data;
    printf( "\n\n Reached the callback func !!! event: %s, global count = %d \n\n",
            event_str[event], global_count++ );

    switch( event )
    {
        case SCTPNOTIFY_OPERROR:
            break;

        case SCTPNOTIFY_DESTSTATCHANGE:
            printf( "Assoc id = %u, dest addr = %u , status = %u\n",
                    notify_data->dststatchange.assoc_id,
                    notify_data->dststatchange.destination_ip_addr.ip.v4.addr,
                    notify_data->dststatchange.status );
            break;

        case SCTPNOTIFY_ENDSTATCHANGE:
            printf( "Assoc id = %u, endpoint status = %u\n",
                    notify_data->endstatchange.assoc_id,
                    notify_data->endstatchange.status );
            break;

        case SCTPNOTIFY_CONGESTION:
            printf( "Assoc id =	%u, congestion level =	%u\n",
                    notify_data->congestion.assoc_id,
                    notify_data->congestion.cong_level );
            break;

        case SCTPNOTIFY_SYSERROR:
            break;

        case SCTPNOTIFY_CONNPENDING:
            printf( "Assoc id = %u \n",
                    notify_data->connpending.assoc_id );
            break;

        case SCTPNOTIFY_DATAPENDING:
            printf( "Assoc id = %u, stream = %d\n",
                    notify_data->datapending.assoc_id,
                    notify_data->datapending.stream );
            break;

        case SCTPNOTIFY_SENDFAILURE:
            printf( "Assoc id = %u, context = %u, cause_code = %u",
                    notify_data->sendfailure.assoc_id,
                    ( sctp_U32bit )notify_data->sendfailure.context,
                    ( sctp_U32bit )notify_data->sendfailure.cause_code );
            break;

        case SCTPNOTIFY_PARTIAL_DATAPENDING:
            printf( "Assoc id = %u, stream = %d\n",
                    notify_data->partial_datapending.assoc_id,
                    notify_data->partial_datapending.stream );
            break;

        case SCTPNOTIFY_RECVCONGESTION:
            printf( "Assoc id =	%u, congestion level =	%d\n",
                    notify_data->recv_congestion.assoc_id,
                    notify_data->recv_congestion.cong_level );
            break;

        case SCTPNOTIFY_SHUTDOWN_EVENT:
            printf( "Assoc id =	%u\n",
                    notify_data->recv_congestion.assoc_id );
            break;

    }

    return NULL;

}
/* Changes for SPR-21022 Starts here (IPv6) */

#ifdef SCTP_INET6_SUPPORT
void call_sctp_api_init6_stack(
    sctp_U32bit       tx_buffers,
    sctp_U32bit       rx_buffers,
    sctp_U32bit       reserve_buffer,
    sctp_U16bit       standard_udp_port,
    sctp_U8bit        *local_addr );

/* Function to Initialize SCTP Stack with IPv6 Address */
void call_sctp_api_init6_stack(
    sctp_U32bit       tx_buffers,
    sctp_U32bit       rx_buffers,
    sctp_U32bit       reserve_buffer,
    sctp_U16bit       standard_udp_port,
    sctp_U8bit        *local_addr )
{
    sctp_return_t  ret;
    sctp_sockaddr_st dest_addr;
    sctp_init_options_st init_options;
    sctp_error_t errorno;
    sctp_U8bit        stale_cookie_policy = 0;
    sctp_U8bit        ack_policy = 2;
    sctp_Boolean_t    ecn_capable = 0;
    sctp_Boolean_t    nat_capable = 0;


    __MEMSET( &init_options, 0, sizeof( init_options ) );
    init_options.MaxPerAssoc.streams = 20;
    init_options.max_appl = 5;
    init_options.max_tcb = 20;
    init_options.max_endpoint = 20;
    init_options.MaxPerAssoc.min_tx_buffers =  tx_buffers;
    init_options.MaxPerAssoc.med_tx_buffers =  tx_buffers;
    init_options.MaxPerAssoc.max_tx_buffers =  tx_buffers;

    init_options.MaxPerAssoc.min_rx_buffers = rx_buffers;
    init_options.MaxPerAssoc.med_rx_buffers = rx_buffers;
    init_options.MaxPerAssoc.max_rx_buffers = rx_buffers;

    init_options.min_reserve_buffers = reserve_buffer;
    init_options.med_reserve_buffers = reserve_buffer;
    init_options.max_reserve_buffers = reserve_buffer;

    init_options.no_of_local_addrs = 1;

    dest_addr.family = SCTP_AF_INET6;
    dest_addr.ip.v6.flow_info = 0;
    dest_addr.ip.v6.scope_id = 2;

    inet_pton( SCTP_AF_INET6, ( const char * )local_addr, &( dest_addr.ip.v6.addr ) );
    dest_addr.ip.v6.addr.in6_u.u6_addr32[0] = ntohl( dest_addr.ip.v6.addr.in6_u.u6_addr32[0] );
    dest_addr.ip.v6.addr.in6_u.u6_addr32[1] = ntohl( dest_addr.ip.v6.addr.in6_u.u6_addr32[1] );
    dest_addr.ip.v6.addr.in6_u.u6_addr32[2] = ntohl( dest_addr.ip.v6.addr.in6_u.u6_addr32[2] );
    dest_addr.ip.v6.addr.in6_u.u6_addr32[3] = ntohl( dest_addr.ip.v6.addr.in6_u.u6_addr32[3] );
    init_options.hostname = NULL;
    init_options.standard_udp_port = standard_udp_port;

    __MEMCOPY( init_options.list_of_local_addrs, &dest_addr, sizeof( sctp_sockaddr_list_st ) );
    init_options.capability.stale_cookie_policy = stale_cookie_policy;
    init_options.capability.ack_policy    = ack_policy;
    init_options.capability.ecn_capable = ecn_capable;
    init_options.capability.pmtu_capable = vx_raw_socket_option;
    init_options.capability.stack_type = vx_stack_type;
    init_options.capability.nat_capable = nat_capable;
    printf( "call_sctp_api_init_stack\r\n" );


    ret = sctp_api_init_stack( &init_options, &errorno );

    if( ret == SCTP_FAILURE )
    {
        printf( "ERROR Initializing SCTP !!  \r\n" );
        printf( "Error Code = %s \r\n" , sctp_error_string[errorno] );
        printf( "Error Number = %d \r\n" , errno );
    }

    else
    {
        printf( "SCTP Stack Initialized \r\n" );
    }
}

void  test_sctp_api_bind6
( sctp_U32bit sockid,
  sctp_U16bit port,
  char        *addr )
{
    sctp_return_t ret ;

    sctp_U32bit num_addrs;
    sctp_error_t errorno;
    sctp_sockaddr_st bind_addr_list[10];

    num_addrs = 1;

    bind_addr_list[0].family = SCTP_AF_INET6;
    /*    inet_pton(SCTP_AF_INET6, (const char *)local_addr, &(dest_addr.ip.v6.addr)); */
    inet_pton( SCTP_AF_INET6, ( const char * )addr, &( bind_addr_list[0].ip.v6.addr ) );
    bind_addr_list[0].ip.v6.addr.in6_u.u6_addr32[0] = ntohl( bind_addr_list[0].ip.v6.addr.in6_u.u6_addr32[0] );
    bind_addr_list[0].ip.v6.addr.in6_u.u6_addr32[1] = ntohl( bind_addr_list[0].ip.v6.addr.in6_u.u6_addr32[1] );
    bind_addr_list[0].ip.v6.addr.in6_u.u6_addr32[2] = ntohl( bind_addr_list[0].ip.v6.addr.in6_u.u6_addr32[2] );
    bind_addr_list[0].ip.v6.addr.in6_u.u6_addr32[3] = ntohl( bind_addr_list[0].ip.v6.addr.in6_u.u6_addr32[3] );

    ret = sctp_api_bind( sockid ,
                         port,
                         ( sctp_U32bit ) num_addrs, /* num of addrs */
                         bind_addr_list,/* addr list */
                         NULL,/* hostname */
                         &errorno );

    if( ret == SCTP_FAILURE )
    {
        printf( "ERROR !! " );
    }

    else
    {
        printf( "SUCCESS !! Bind on SCTP port = %hu \r\n ", port );
    }

}

/* bindx function for IPv6 Address (Function Interface) */
void call_sctp_api_bindx6(
    sctp_U32bit            assoc_id,
    sctp_U8bit            *addr,        /* IPv6 Address */
    sctp_bindx_flag_et     flags,
    sctp_Boolean_t         block )
{
    int retval = -1;
    sctp_U32bit  n_addr = 1;
    sctp_error_t errorno;
    sctp_sockaddr_list_st  sctp_addr_list;
    inet_pton( SCTP_AF_INET6, ( const char * )addr, &( sctp_addr_list[0].ip.v6.addr ) );
    sctp_addr_list[0].ip.v6.addr.in6_u.u6_addr32[0] = ntohl( sctp_addr_list[0].ip.v6.addr.in6_u.u6_addr32[0] );
    sctp_addr_list[0].ip.v6.addr.in6_u.u6_addr32[1] = ntohl( sctp_addr_list[0].ip.v6.addr.in6_u.u6_addr32[1] );
    sctp_addr_list[0].ip.v6.addr.in6_u.u6_addr32[2] = ntohl( sctp_addr_list[0].ip.v6.addr.in6_u.u6_addr32[2] );
    sctp_addr_list[0].ip.v6.addr.in6_u.u6_addr32[3] = ntohl( sctp_addr_list[0].ip.v6.addr.in6_u.u6_addr32[3] );
    inet_pton( SCTP_AF_INET6, ( const char * )addr, &( sctp_addr_list[0].ip.v6.addr ) );

    retval = sctp_api_bindx(
                 assoc_id,
                 n_addr,
                 sctp_addr_list,
                 flags,
                 block,
                 &errorno );

    if( SCTP_FAILURE == retval )
    {
        printf( "Error Code = %s \r\n" , sctp_error_string[errorno] );
    }

    else
    {
        printf( "\nbindx successful !\n" );
    }
}

/* Changes for SPR-21022 Ends here (IPv6) */

/* Changes for SPR-21022 Starts here (Multihoming) */

/* Function to configure one IPv6 & one IPv4 Address on Vxworks */
void call_sctp_api_init_v6_v4_stack(
    sctp_U32bit       tx_buffers,
    sctp_U32bit       rx_buffers,
    sctp_U32bit       reserve_buffer,
    sctp_U16bit       standard_udp_port,
    sctp_U8bit        *local_addr,      /* IPv6 Address */
    sctp_U8bit        *local_addrv4 )   /* IPv4 Address */
{
    sctp_return_t  ret;
    sctp_sockaddr_st dest_addr[2];
    sctp_init_options_st init_options;
    sctp_error_t errorno;
    sctp_U8bit        stale_cookie_policy = 0;
    sctp_U8bit        ack_policy = 2;
    sctp_Boolean_t    ecn_capable = 0;
    sctp_Boolean_t    nat_capable = 0;


    __MEMSET( &init_options, 0, sizeof( init_options ) );
    init_options.MaxPerAssoc.streams = 20;
    init_options.max_appl = 5;
    init_options.max_tcb = 20;
    init_options.max_endpoint = 20;
    init_options.MaxPerAssoc.min_tx_buffers =  tx_buffers;
    init_options.MaxPerAssoc.med_tx_buffers =  tx_buffers;
    init_options.MaxPerAssoc.max_tx_buffers =  tx_buffers;

    init_options.MaxPerAssoc.min_rx_buffers = rx_buffers;
    init_options.MaxPerAssoc.med_rx_buffers = rx_buffers;
    init_options.MaxPerAssoc.max_rx_buffers = rx_buffers;

    init_options.min_reserve_buffers = reserve_buffer;
    init_options.med_reserve_buffers = reserve_buffer;
    init_options.max_reserve_buffers = reserve_buffer;

    init_options.no_of_local_addrs = 2;     /* No. of IP Addresses */

    dest_addr[0].family = SCTP_AF_INET6;
    dest_addr[0].ip.v6.flow_info = 0;
    dest_addr[0].ip.v6.scope_id = 2;
    dest_addr[1].family = SCTP_AF_INET;

    ( ( sctp_sockaddr_st * )( &dest_addr[1] ) )->ip.v4.addr = ntohl( inet_addr( ( const sigtran_8bit * )local_addrv4 ) );

    inet_pton( SCTP_AF_INET6, ( const char * )local_addr, &( dest_addr[0].ip.v6.addr ) );
    dest_addr[0].ip.v6.addr.in6_u.u6_addr32[0] = ntohl( dest_addr[0].ip.v6.addr.in6_u.u6_addr32[0] );
    dest_addr[0].ip.v6.addr.in6_u.u6_addr32[1] = ntohl( dest_addr[0].ip.v6.addr.in6_u.u6_addr32[1] );
    dest_addr[0].ip.v6.addr.in6_u.u6_addr32[2] = ntohl( dest_addr[0].ip.v6.addr.in6_u.u6_addr32[2] );
    dest_addr[0].ip.v6.addr.in6_u.u6_addr32[3] = ntohl( dest_addr[0].ip.v6.addr.in6_u.u6_addr32[3] );

    init_options.hostname = NULL;
    init_options.standard_udp_port = standard_udp_port;

    __MEMCOPY( &init_options.list_of_local_addrs[0], &dest_addr[0], sizeof( sctp_sockaddr_st ) );
    __MEMCOPY( &init_options.list_of_local_addrs[1], &dest_addr[1], sizeof( sctp_sockaddr_st ) );

    init_options.capability.stale_cookie_policy = stale_cookie_policy;
    init_options.capability.ack_policy    = ack_policy;
    init_options.capability.ecn_capable = ecn_capable;
    init_options.capability.pmtu_capable = vx_raw_socket_option;
    init_options.capability.stack_type = vx_stack_type;  /*Stack Type must be IPv6 */
    init_options.capability.nat_capable = nat_capable;

    printf( "call_sctp_api_init_stack\r\n" );

    ret = sctp_api_init_stack( &init_options, &errorno );

    if( ret == SCTP_FAILURE )
    {
        printf( "ERROR Initializing SCTP !!  \r\n" );
        printf( "Error Code = %s \r\n" , sctp_error_string[errorno] );
        printf( "Error Number = %d \r\n" , errno );
    }

    else
    {
        printf( "SCTP Stack Initialized \r\n" );
    }
}

void  test_sctp_api_bind_v6_v4_addrs
( sctp_U32bit sockid,
  sctp_U16bit port,
  char        *addr,     /* IPv6 Address */
  char        *addr_v4 ) /* IPv4 Address */
{
    sctp_return_t ret ;

    sctp_U32bit num_addrs;      /* No. of IP Addresses */
    sctp_error_t errorno;
    sctp_sockaddr_st bind_addr_list[10];

    num_addrs = 2;

    bind_addr_list[0].family = SCTP_AF_INET6;
    bind_addr_list[1].family = SCTP_AF_INET;
    inet_pton( SCTP_AF_INET6, ( const char * )addr, &( bind_addr_list[0].ip.v6.addr ) );
    bind_addr_list[0].ip.v6.addr.in6_u.u6_addr32[0] = ntohl( bind_addr_list[0].ip.v6.addr.in6_u.u6_addr32[0] );
    bind_addr_list[0].ip.v6.addr.in6_u.u6_addr32[1] = ntohl( bind_addr_list[0].ip.v6.addr.in6_u.u6_addr32[1] );
    bind_addr_list[0].ip.v6.addr.in6_u.u6_addr32[2] = ntohl( bind_addr_list[0].ip.v6.addr.in6_u.u6_addr32[2] );
    bind_addr_list[0].ip.v6.addr.in6_u.u6_addr32[3] = ntohl( bind_addr_list[0].ip.v6.addr.in6_u.u6_addr32[3] );

    bind_addr_list[1].ip.v4.addr = ntohl( inet_addr( addr_v4 ) );
    ret = sctp_api_bind( sockid ,
                         port,
                         ( sctp_U32bit ) num_addrs, /* num of addrs */
                         bind_addr_list,/* addr list */
                         NULL,/* hostname */
                         &errorno );

    if( ret == SCTP_FAILURE )
    {
        printf( "ERROR !! " );
    }

    else
    {
        printf( "SUCCESS !! Bind on SCTP port = %hu \r\n ", port );
    }

}
#endif  /* SCTP_INET6_SUPPORT */

#ifndef SCTP_UI_SOCK_API
/* bindx function for IPv4 Address (Function Interface) */
void call_sctp_api_bindx(
    sctp_U32bit            assoc_id,
    sctp_U8bit            *addr,    /* IPv4 Address */
    sctp_bindx_flag_et     flags,
    sctp_Boolean_t         block );

/* bindx function for IPv4 Address */
void call_sctp_api_bindx(
    sctp_U32bit            assoc_id,
    sctp_U8bit            *addr,    /* IPv4 Address */
    sctp_bindx_flag_et     flags,
    sctp_Boolean_t         block )
{
    int retval = -1;
    sctp_U32bit  n_addr = 1;
    sctp_error_t errorno;
    sctp_sockaddr_list_st  sctp_addr_list;

    sctp_addr_list[0].ip.v4.addr = ntohl( inet_addr( ( const char * )addr ) );

    retval = sctp_api_bindx(
                 assoc_id,
                 n_addr,
                 sctp_addr_list,
                 flags,
                 block,
                 &errorno );

    if( SCTP_FAILURE == retval )
    {
        printf( "Error Code = %s \r\n" , sctp_error_string[errorno] );
    }

    else
    {
        printf( "\nbindx successful !\n" );
    }
}
#endif /*SCTP_UI_SOCK_API */
/* Changes for SPR-21022 Ends here (Ipv6) */
#endif /* SCTP_UI_SOCK_API */

void enable_sctp_traces( void )
{
#ifdef SCTP_DEBUG_ENABLED
    sctp_return_t ret;
    sctp_error_t errorno;
    sctp_U32bit  assoc_id = SCTP_INVALID_ENDPOINT;
    sctp_trc_et  ttype = SCTP_ALL_TRC;

    printf( "\n Calling enable trace " );
    ret = sctp_api_enable_assoc_trc(
              assoc_id ,
              ttype ,
              &errorno );

    if( ret == SCTP_SUCCESS )
    {
        printf( "\n SUCCESS" );
    }

    else
    {
        printf( "\n FAILED " );
    }

#else
    printf( "traces not enabled\n" );
#endif
    return;
}


void call_api_sctp_set_stack_envt( sctp_Boolean_t b_alrdy_init,
                                   sctp_Boolean_t is_block )
{
    stack_envt_st     stack_envt;
    stack_envt.is_blocked = 1;
    stack_envt.local_port = 13456;

    if( is_block == 1 )
    {
        if( SCTP_FAILURE == sctp_set_stack_envt( b_alrdy_init, &stack_envt ) )
        {
            printf( "\nERROR: SCTP Set Stack Envt Failure " );
            return;
        }
    }

    else
    {
        if( SCTP_FAILURE == sctp_set_stack_envt( b_alrdy_init, NULL ) )
        {
            printf( "\nERROR: SCTP Set Stack Envt Failure " );
            return;
        }
    }

    printf( "\nSCTP Set Stack Envt Succeeded" );
    return ;
}



void get_clk_res( void )
{
    struct timespec time1;

    if( ERROR == clock_getres( CLOCK_REALTIME, &time1 ) )
    {
        printf( "error in clkres \r\n" );
    }

    else
    {
        printf( "Res sec %d nsec %d \r\n",
                time1.tv_sec, time1.tv_nsec );
    }
}


void sctp_user_task( void )
{

    sctp_init_options_st init_options;
    sctp_return_t ret ;
    sctp_error_t errorno;
    sctp_sockaddr_st *dest_addr;
    sctp_U8bit *p_buffer, buffer[6];
    int count1;

    /*    SEM_ID  binary,counting; */

    printf( "sctp user task entered \r\n" );

    /*__MEMCOPY(&init_options.list_of_local_addrs, &addr_list, sizeof(sctp_sockaddr_list_st));*/
    /* init_options.list_of_local_addrs = &addr_list;*/

    memset( &init_options, 0, sizeof( init_options ) );
    init_options.MaxPerAssoc.streams = 16;

    init_options.max_appl = 10;

    init_options.max_endpoint = 10;

    init_options.max_tcb = 10;


    init_options.MaxPerAssoc.min_tx_buffers = 100;
    init_options.MaxPerAssoc.med_tx_buffers = 100;
    init_options.MaxPerAssoc.max_tx_buffers = 100;

    init_options.MaxPerAssoc.min_rx_buffers = 100;
    init_options.MaxPerAssoc.med_rx_buffers = 100;
    init_options.MaxPerAssoc.max_rx_buffers = 100;

    init_options.no_of_local_addrs = num_local_addrs;
    init_options.min_reserve_buffers = 100;
    init_options.med_reserve_buffers = 100;
    init_options.max_reserve_buffers = 100;

    printf( "sctp user task stage 1 \r\n" );

    for( count1 = 0; count1 < num_local_addrs; count1++ )
    {
        dest_addr = &init_options.list_of_local_addrs[count1];
        dest_addr->family = SCTP_AF_INET;
        /* Changes for SPR-21022 Starts here */
        dest_addr->ip.v4.addr = ntohl( inet_addr( ( const sigtran_8bit * )net_addr[count1] ) );
        /* Changes for SPR-21022 Ends here */
    }

    init_options.hostname = NULL;

    /*    printf("Standard UDP Port ..  ");
    scanf("%d",&read_in); */
    init_options.standard_udp_port = ( sctp_U16bit )sctp_udp_port;

    init_options.capability.stale_cookie_policy  = 0 ;
    init_options.capability.ack_policy = 2;
    init_options.capability.ecn_capable  = SCTP_FALSE;
    init_options.capability.nat_capable  = SCTP_TRUE;
    init_options.capability.pmtu_capable  = SCTP_FALSE;
    init_options.capability.stack_type = SCTP_OVER_INET;

    init_options.hostname = sctp_hostname;
    printf( "sctp user task stage 2 \r\n" );

    ret = sctp_api_init_stack( &init_options, &errorno );

    printf( "sctp user task stage 3 \r\n" );

    if( ret == SCTP_FAILURE )
    {
        printf( "ERROR Initializing SCTP !! \r\n" );
    }

    else
    {
        printf( "SCTP Stack Initialized \r\n" );
    }

    enable_sctp_traces();

    /* call sctp init */

    printf( "sctp_user_task called \r\n" );

    /* to talk to other sctp */
    /* routeAdd("139.85.241.0","139.85.238.29");
    routeAdd("192.168.201.0","139.85.238.29"); */

    __MEMSET( ( sctp_U8bit * )&buffer[0], 0, 6 );
    p_buffer = ( sctp_U8bit * )&buffer[0];


    for( ;; )
    {
        taskDelay( 500 );

        switch( sctp_user_action )
        {
            case 0:
                /*printf("**User task**");*/
                break;
#ifndef SCTP_UI_SOCK_API

            case 1:
                printf( "registering su \r\n" );
                test_sctp_api_register_su();
                sctp_user_action = 0;
                break;

            case 2:
                printf( "socket call \r\n" );
                test_sctp_api_socket( application_id );
                sctp_user_action = 0;
                break;

            case 3:
                printf( "Bind call \r\n" );
                test_sctp_api_bind( sd, sctp_port, "0.0.0.0" );
                sctp_user_action = 0;
                break;

            case 4:
                printf( "Accept call \r\n" );
                test_sctp_api_accept( use_sd );
                sctp_user_action = 0;
                break;

            case 5:
                printf( "Connect	call \r\n" );
                /* Changes for SPR-21022 Starts here */
                test_sctp_api_connect( sd, sctp_port, ( sigtran_8bit * )remote_addr );
                /* Changes for SPR-21022 Ends here */
                sctp_user_action = 0;
                break;

            case 6:
                printf( "Send call \r\n" );
                test_sctp_api_send( use_sd );
                sctp_user_action = 0;
                break;

            case 7:
                printf( "close call \r\n" );
                test_sctp_api_close( use_sd );
                sctp_user_action = 0;

            case 8:
                printf( "listen call \r\n" );
                test_sctp_api_listen( use_sd );
                sctp_user_action = 0;
#else

            case 7:
                printf( "sendmsg call \r\n" );
                /* Changes for SPR-21022 Starts here */
                call_sctp_sendmsg_init( 1, 5, ( sigtran_8bit * )remote_addr, 10, 10, 10, 10, "asdsad" );
                /* Changes for SPR-21022 Ends here */
                sctp_user_action = 0;


#endif /* SCTP_UI_SOCK_API */

            default:
                sctp_user_action = 0;
        }

    }
}

void start_sctp( void )
{

    /* spawn sctp_user_task */
    printf( "\n Start sctp called \r\n" );

    /*  if (-1 == sp((FUNCPTR)sctp_user_task,0,0,0,0,0,0,0,0,0)) */
    if( taskSpawn( "SCTP_MAIN",
                   SCTP_RECV_PRIORITY,
                   SCTP_RECV_OPTIONS,
                   SCTP_RECV_STACKSIZE,
                   ( FUNCPTR )sctp_user_task,
                   0,
                   0,
                   0,
                   0,
                   0,
                   0,
                   0,
                   0,
                   0,
                   0 ) == -1 )
    {
        printf( " sctp_user_task not started \r\n" );
    }

    else
    {
        printf( " sctp_user_task started \r\n" );
    }

    return;
}



/* print address */
void
sctp_print_sock_addr(
    sctp_Pvoid_t   *p_addr )
{
    unsigned char f;
#ifdef SCTP_INET6_SUPPORT
    char *buf;
#endif

    f = *( ( ( unsigned char * ) p_addr ) + 1 );

    if( f == AF_INET )
    {
        struct sockaddr_in *p_4addr = ( struct sockaddr_in * )p_addr;
        printf( "\n ADDRESS %u  ", p_4addr->sin_addr.s_addr );
    }

#ifdef SCTP_INET6_SUPPORT

    else if( f == SCTP_AF_INET6 )
    {
        struct sockaddr_in6   *p_6addr = ( struct sockaddr_in6 * )p_addr;
        struct in6_addr       ip_6addr = p_6addr->sin6_addr;

        printf( "%d %d %d %d",
                ip_6addr.s6_addr32[3],
                ip_6addr.s6_addr32[2],
                ip_6addr.s6_addr32[1],
                ip_6addr.s6_addr32[0] );
        inet_ntop( SCTP_AF_INET6, ( void * ) &ip_6addr , buf , INET6_ADDRSTRLEN );
        printf( " ADDRESS  -> %s ", buf );
    }

#endif

    return;
}

sctp_return_t
sctp_parse_cliaddr_into_sctp_addr(
    char *p_token,
    sctp_sockaddr_st *p_addr )
{
    /*if(!p_token) return SCTP_FAILURE;

    p_addr->ip.v4.addr =str2ipaddr(p_token);
    p_addr->family =SCTP_AF_INET;
    return SCTP_SUCCESS;*/
#ifdef SCTP_INET6_SUPPORT
    struct in6_addr             ip6_addr;
#endif

    if( !p_token )
    {
        return ( SCTP_FAILURE );
    }

    if( strstr( p_token , "V6" ) || strstr( p_token , "v6" ) )
    {
#ifdef SCTP_INET6_SUPPORT

        if( inet_pton( SCTP_AF_INET6, ( p_token + 3 ),
                       ( void * )&ip6_addr ) != 1 )
        {
            printf( "sctp_parse_init_params:Couldnot convert the "
                    "INET6 address\n" );

            return ( SCTP_FAILURE );
        }

        p_addr->ip.v6.addr.s_u6_addr32[0] = ntohl( ip6_addr.s6_addr32[0] );
        p_addr->ip.v6.addr.s_u6_addr32[1] = ntohl( ip6_addr.s6_addr32[1] );
        p_addr->ip.v6.addr.s_u6_addr32[2] = ntohl( ip6_addr.s6_addr32[2] );
        p_addr->ip.v6.addr.s_u6_addr32[3] = ntohl( ip6_addr.s6_addr32[3] );
        p_addr->family = SCTP_AF_INET6;
        p_addr->ip.v6.flow_info = 0;
        p_addr->ip.v6.scope_id = 0;
#endif
    }

    else if( strstr( p_token , "V4" ) || strstr( p_token , "v4" ) )
    {
        p_addr->ip.v4.addr  = str2ipaddr( p_token + 3 );
        p_addr->family = SCTP_AF_INET;
    }

    else
    {
        printf( "Address format not correct\n" );
    }

    return ( SCTP_SUCCESS );

}

unsigned int str2ipaddr( register char *p_str_addr )
{
    /*struct in_addr addr;*/
    sctp_U32bit addr;

    if( !p_str_addr || !*p_str_addr )
    {
        printf( "str2ipaddr: Null pointer" );
        return -1;
    }

    addr = inet_addr( p_str_addr );
    return ntohl( addr );
}

