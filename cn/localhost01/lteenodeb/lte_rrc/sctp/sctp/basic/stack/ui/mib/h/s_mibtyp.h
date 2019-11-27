/***************************************************************************
 *  FILE NAME:sm_typ.h
 *
 *  DESCRIPTION:contain all the structure and typedefine.
 *
 *    DATE     NAME              REF#               REASON
 *    -----------------------------------------------------
 *    8may01   sigtran team    HSS:40600244         Original
 *    27Sep2001  Yogesh Gahlaut                  Rel 3.0 Changes
 *
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/

#define LOG_MSG  printf

#define SCTP_MIB_PRIM_PROC_UNKNOWN 0

/* external definitions    */
typedef sctp_time_t   sctp_timestamp;
typedef sctp_U32bit   sctp_counter32;
typedef sctp_U32bit   sctp_counter64;
typedef sctp_U8bit    sctp_oct_str;

/* SCTP RTO algo used */
typedef   enum
{
    RTO_ALGO_OTHER = 1,
    RTO_ALGO_VANJ = 2
} sctpmib_rto_algo_et;

/* remote address is active or not */
typedef   enum
{
    REM_INACTIVE = 2,
    REM_ACTIVE   = 1
} sctpmib_rem_add_status_et;

/* state enumeration type,assoc state */
typedef  enum
{
    SM_CLOSED                             = 1,
    SM_COOKIE_WAIT                        = 2,
    SM_COOKIE_ECHOD                       = 3,
    SM_ESTABLISHED                        = 4,
    SM_SHUTDOWN_PENDING                   = 5,
    SM_SHUTDOWN_SENT                      = 6,
    SM_SHUTDOWN_RECEIVED                  = 7,
    SM_SHUTDOWN_ACK_SENT                  = 8,
    SM_DELETE_TCB                         = 9
} sctpmib_assoc_state_et;


/* heart beat flag is active or inactive */
typedef    enum
{
    HB_ACTIVE   = 1,
    HB_INACTIVE = 2
} sctpmib_hb_flag_et;

typedef   enum
{
    UNKNOWN = 0,
    IPv4    = 1,
    IPv6    = 2
} sctpmib_addr_et;

/* this is structure used for configuration parameters */
typedef  struct
{
    sctp_U32bit  rto_algo;
    sctp_U32bit  rto_min;
    sctp_U32bit  rto_max;
    sctp_U32bit  rto_initial;
    sctp_U32bit  max_assoc;
    sctp_U32bit  val_cookie_life ;
    sctp_U32bit  max_init_retr;
} sctpmib_config_params_st;

/* this is parameter id */
typedef enum
{
    SCTPMIB_RTO_MIN          = 0,
    SCTPMIB_RTO_MAX          = 1,
    SCTPMIB_RTO_INITIAL      = 2,
    SCTPMIB_MAX_ASSOC        = 3,
    SCTPMIB_VAL_COOKIE_LIFE  = 4,
    SCTPMIB_MAX_INIT_RETR    = 5,
    SCTPMIB_ALL_PARA         = 6
} sctpmib_config_param_et;

/* this is structure for state controlled parameters */
typedef struct
{
    sctp_counter32      curr_estab;
    sctp_counter32      active_estab;
    sctp_counter32      passive_estab;
    sctp_counter32      aborted;
    sctp_counter32      shutdown;
} sctpmib_state_stat_params_st;

typedef enum
{
    SCTPMIB_CURR_ESTAB            = 0,
    SCTPMIB_ACTIVE_ESTAB          = 1,
    SCTPMIB_PASSIVE_ESTAB         = 2,
    SCTPMIB_ABORTED               = 3,
    SCTPMIB_SHUTDOWN              = 4,
    SCTPMIB_ALL_STATCONT_PARAMS   = 5
} sctpmib_state_stat_params_et;

/* structure of all common statistical parameters */
typedef struct
{
    sctp_counter32      out_of_blue; /*rel 4.0*/
    sctp_counter32      chk_sum_errs; /*rel 4.0*/
    sctp_counter64      ctrl_sent_chunks;
    sctp_counter64      data_ord_sent_chunks;
    sctp_counter64      data_unord_sent_chunks;
    sctp_counter64      ctrl_recv_chunks;
    sctp_counter64      data_ord_recv_chunks;
    sctp_counter64      data_unord_recv_chunks;
    sctp_counter64      frag_usr_msg;
    sctp_counter64      resemble_usr_msg;
    sctp_counter64      sent_packs;
    sctp_counter64      recv_packs;
    sctp_timestamp      discontinuity_time;
} sctpmib_other_stat_params_st;

typedef enum
{
    SCTPMIB_OUT_OF_BLUE             = 0,
    SCTPMIB_CHK_SUM_ERRS            = 1,
    SCTPMIB_CTRL_CHNK_SENT          = 2,
    SCTPMIB_ORD_DATA_CHNK_SENT      = 3,
    SCTPMIB_UNORD_DATA_CHNK_SENT    = 4,
    SCTPMIB_CTRL_CHNK_RECV          = 5,
    SCTPMIB_ORD_DATA_CHNK_RECV      = 6,
    SCTPMIB_UNORD_DATA_CHNK_RECV    = 7,
    SCTPMIB_FRAG_USR_MSG            = 8,
    SCTPMIB_RESEMABE_USR_MSG        = 9,
    SCTPMIB_PACKETS_SENT            = 10,
    SCTPMIB_PACKETS_RECV            = 11,
    SCTPMIB_ALL_OTHRSTAT_PARAMS     = 12
} sctpmib_other_stat_params_et;

/* structure of association table related parameters */
typedef struct
{
    sctp_U8bit                      rem_host_name[SCTP_MAX_HOSTNAME_SIZE];
    sctp_U32bit                     local_sctp_port;
    sctp_U32bit                     rem_sctp_port;
    sctpmib_addr_et                 rem_prim_add_type;
    sctp_sockaddr_st                rem_prim_add;
    sctp_U32bit                     heart_beat_timer;
    sctpmib_assoc_state_et          assoc_state;
    sctp_U32bit                     assoc_in_streams;
    sctp_U32bit                     assoc_out_streams;
    sctp_U32bit                     assoc_max_retr;
    sctp_U32bit                     assoc_prim_process;/*rel 4.0*/
    sctp_counter32                  assoc_T1_expired;
    sctp_counter32                  assoc_T2_expired;
    sctp_counter32                  assoc_Rtx_chunks;
    sctp_timestamp                  assoc_start_time;
    sctp_timestamp                  assoc_discontinuity_time;
    sctp_timestamp                  assoc_localadd_start_time;
    sctpmib_addr_et                 rem_add_type; /* CSR 1-3307301 */
    sctp_sockaddr_st                rem_add; /* CSR 1-3307301 */
    sctpmib_rem_add_status_et       assoc_rem_add_status;
    sctpmib_hb_flag_et              assoc_rem_add_hb_flag;
    sctp_U32bit                     assoc_rem_add_rto;
    sctp_U32bit                     assoc_rem_add_max_path_retx;
    sctp_timestamp                  assoc_rem_add_start_time;
} sctpmib_assoc_table_st;


typedef enum
{
    SCTPMIB_REM_HOST_NAME                  = 0,
    SCTPMIB_LOCAL_SCTP_PORT                = 1,
    SCTPMIB_REM_SCTP_PORT                  = 2,
    SCTPMIB_REM_PRIM_ADD_TYPE              = 3,
    SCTPMIB_REM_PRIM_ADD                   = 4,
    SCTPMIB_HEART_BEAT_TIMER               = 5,
    SCTPMIB_ASSOC_STATE                    = 6,
    SCTPMIB_ASSOC_IN_STREAMS               = 7,
    SCTPMIB_ASSOC_OUT_STREAMS              = 8,
    SCTPMIB_ASSOC_MAX_RETR                 = 9,
    SCTPMIB_ASSOC_T1_EXPIRED               = 10,
    SCTPMIB_ASSOC_T2_EXPIRED               = 11,
    SCTPMIB_ASSOC_RTX_CHUNKS               = 12,
    SCTPMIB_ASSOC_START_TIME               = 13,
    SCTPMIB_ASSOC_LOCALADD_START_TIME      = 14,
    SCTPMIB_ASSOC_REM_ADD_STATUS           = 15,
    SCTPMIB_ASSOC_REM_ADD_HB_FLAG          = 16,
    SCTPMIB_ASSOC_REM_ADD_RTO              = 17,
    SCTPMIB_ASSOC_REM_ADD_MAX_PATH_RETX    = 18,
    SCTPMIB_ASSOC_REM_ADD_START_TIME       = 19,
    SCTPMIB_ASSOC_PRIM_PROCESS         = 20,
    SCTPMIB_REM_ADD_TYPE               = 21,
    SCTPMIB_REM_ADD                    = 22,
    SCTPMIB_ASSOC_ALL_PARAMS               = 23
} sctpmib_assoc_table_et;
