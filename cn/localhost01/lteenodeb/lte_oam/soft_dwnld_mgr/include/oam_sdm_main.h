/*******************************************************************************
 * File Name   : oam_sdm_main.h
 *
 * Description : 
 * Revision History:
 *
 * Date            Author          Reference             Comments
 * -----           ------          ---------             --------
 * January,2013    Vinod Vyas      OAM Design Document   Initial Version
 * April 2014      Vinod Vyas      CSR 57877 Fix
 * May 2014        Shalu Sagar     Klockwork Fix
 * Copyright (c) 2013, Aricent Inc. All Rights Reserved
 *******************************************************************************/

#ifndef OAM_SDM_MAIN_H
#define OAM_SDM_MAIN_H


/*******************************************************************************
 * Includes
 *******************************************************************************/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <lteTypes.h>
#include <stacklayer.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

/*******************************************************************************
 * Type Def
 ********************************************************************************/
typedef struct _prov_req_t
{
  /* File pointer for loging */
  FILE *p_out;
  /* File pointer for loging */
  FILE *p_fp;
  /* Log level for Loging */
  UInt8 log_level;

} prov_request_t;

typedef enum _log_priority_et
{
  OAM_SDM_DETAILED,		/* Info about all the messg */
  OAM_SDM_INFO,			/* Info about the resources */
  OAM_SDM_WARNING,		/* Warning messages */
  OAM_SDM_ERROR,		/* Error messages */
  OAM_SDM_CRITICAL		/*Critical conditions, such as hard device errors */
} log_priority_et;

typedef enum
{
  OAM_SDM_FAILURE = 0,
  OAM_SDM_SUCCESS
} oam_sdm_return_et;


typedef struct __oam_timer_data_t {
    UInt16 module_id;
    UInt16 timer_type;
    void   *buf;
    UInt16 buf_size;
} oam_timer_data_t;


typedef enum
{       
/*CSR 57877 Fix Start */  
    OAM_SDM_NO_ERROR, 
/*CSR 57877 Fix End*/  
    OAM_SDM_MALLOC_FAIL,
    OAM_SDM_SEND_FAIL,
    OAM_SDM_NULL_POINTER_ERR,
    OAM_SDM_INSUFFICIENT_BUFFER,
    OAM_SDM_MODULE_DISABLED,
    OAM_SDM_INVALID_REQ,
} oam_sdm_error_code_et;

typedef struct edit_t edit_t;

typedef struct interpret_t interpret_t;

#define OAM_SDM_MAX_BIN_NAME        MAX_BIN_NAME_LEN
struct _oam_sdm_process_info_t
{
	char name[OAM_SDM_MAX_BIN_NAME];
	pid_t pid;
};
typedef QTIMER oam_sdm_timer_t;
typedef enum {
	OAM_FALSE = 0,
	OAM_TRUE
} oam_sdm_bool_et;

struct _oam_sdm_sim_table_t
{
  struct _oam_sdm_process_info_t *p_info;
  UInt8 max_no_process;
};

typedef struct _version_info_t
{
  UInt8  oui[65];
  UInt8  product_class[65];
  UInt8  hw_version[65];
  UInt8  soft_version[65];
}version_info_t;

#define MAX_PORT_LENGTH            6
#define MAX_IP_ADD_LEN_STR         64

typedef enum
{
	MODE_TCP,
	MODE_UDP
} mode_et;


typedef struct _com_params_t
{
	mode_et                 mode;
	U8                 port[MAX_PORT_LENGTH];
	U8                 ipaddr[MAX_IP_ADD_LEN_STR];
} com_params_t;

typedef struct _sdm_ext_com_config_t
{
  com_params_t    oam_com_params;
  com_params_t    sdm_com_params; 
} sdm_ext_com_config_t;

/*******************************************************************************
 * Defines
 ********************************************************************************/
#define OAM_SDM_LOG(source, log_priority, formate, ...) \
	oam_sdm_trace_message(log_priority,(SInt8 *) __FILE__,(SInt8 *)__FUNCTION__,__LINE__,(SInt8 *)formate,##__VA_ARGS__)

#ifndef OAM_EVM_TARGET
#define OAM_SDM_FUN_ENTRY_TRACE()\
    OAM_SDM_LOG(SDM, OAM_SDM_DETAILED, "Entering function: %s in file: %s\n",\
            __FUNCTION__, __FILE__)
  
#define OAM_SDM_FUN_EXIT_TRACE()\
    OAM_SDM_LOG(SDM, OAM_SDM_DETAILED, "Exiting function: %s in file: %s\n",\
            __FUNCTION__, __FILE__)
#endif

#ifdef OAM_EVM_TARGET 
#define OAM_SDM_FUN_ENTRY_TRACE()
#define OAM_SDM_FUN_EXIT_TRACE()
#endif

#define OAM_SDM_OPENDIR(X)   opendir(X)
#define OAM_SDM_READDIR(X)   readdir(X)
#define OAM_SDM_SLEEP(X)     sleep(X)
#define OAM_SDM_VA_START  va_start
#define OAM_SDM_VA_END    va_end
#define OAM_SDM_VSPRINTF(X,Y,Z)  vsprintf(X,Y,Z)
#define OAM_SDM_CPU_ZERO  CPU_ZERO
#define OAM_SDM_CPU_SET  CPU_SET

#define OAM_SDM_OPENDIR(X)   opendir(X)
#define OAM_SDM_READDIR(X)   readdir(X)
#define OAM_SDM_SLEEP(X)     sleep(X)
#define OAM_SDM_VA_START  va_start
#define OAM_SDM_VA_END    va_end
#define OAM_SDM_VSPRINTF(X,Y,Z)  vsprintf(X,Y,Z)

 
#define LOCAL                                    static
#define FORK_ERROR                              -1
#define FORK_CHILD                               0
#define OAM_SDM_NULL                          NULL
#define OAM_INVALID_RETURN                      -1
#define OAM_SDM_ZERO                             0
#define OAM_SDM_ONE                              1
#define OAM_SDM_STD_ERROR                       -1
#define OAM_SDM_POLL_TIME                       10   
#define OAM_BIT_ONE                              1
#define MAX_RX_DATA_BUFF_SIZE                11000
#define OAM_NEG_ONE                             -1
#define OAM_SDM_TIMEOUT                         10
#define TIME_SIZE                               20
#define OAM_SDM_INTF_HEADER_SIZE                12
#define OAM_SDM_MODULE_ID                       26 
#define OAM_MODULE_ID                            1
#define OAM_TIMEOUT				               132
#define SDM_OAM_ACTIVE_TIME_OUT                180  
#define OAM_NOTIFICATION                       189
#define OAM_CSPL_HDR_SIZE                       16
#define OAM_SDM_ROLLBACK                         2
/********Timer Type*******/
#define SDM_OAM_POLL_TIMER_TYPE                  1
#define SDM_OAM_ACTIVE_IND_TIMER_TYPE            2

/* CSR 57877 FIx Change */
/* Removed API */
#define OAM_SDM_RESULT_SUCCESS                    0
#define OAM_SDM_RESULT_FAILURE                    1
#define OAM_SDM_API_VERSION                       1
#define OAM_SDM_ERR_FILE_ACCESS_FAILURE        9016
#define OAM_SDM_VERSION                           1
#ifndef OAM_EVM_TARGET    
#define MAX_NO_OF_BACKUP                          5    
#else
    #define MAX_NO_OF_BACKUP                          1
#endif    
#define MAX_STR_LEN                     1024
#define MAX_BACKUP_STR_LENGTH                   256 
#define FACTORY_RESET                             2
#define SDM_SHUTDOWN_CMD                          4
#define MAX_NO_OF_POLL_TRY                        3
#define NO_OF_BACKUP_DIR                          1
#define OAM_SDM_FALSE                             0
#define OAM_SDM_TRUE                              1
#define OAM_BIT_ZERO                              0
#define OAM_BYTESIZE                              8
#define OAM_SDM_TAR_SW_CMD_SIZE                 500
#define OAM_SDM_BKP_SW_CMD_SIZE                 200
#define OAM_SDM_BKP_FILE_NAME_SIZE               65
#define OAM_SDM_ERR_STRG_SIZE                    80
#define OAM_SDM_READ_BUFF_SIZE                  100 
#define OAM_SDM_TEMP_STR_SIZE                    16 
#define OAM_SDM_TIME_CONV_S_TO_MS              1000
#define OAM_SDM_RECV_BUFFER_SIZE               5000  
#define OAM_SDM_TRACE_FORMAT_SIZE               500  
#define OAM_SDM_TRACE_TEMP_STR_SIZE              60  
#define OAM_SDM_TRACE_YEAR_FMT                 1900  
#define OAM_SDM_KILL_ALL_CMD_SIZE               100
/* SPR 14360 START */
#define OAM_SDM_SHUTDWN_CMD_SIZE                 24
/* SPR 14360 END */
#define OAM_SDM_DEST_FILE_SIZE                   64
#define OAM_SDM_TWO                               2
#define OAM_SDM_THREE                             3
#define OAM_SDM_FOUR                              4
#define OAM_SDM_FIVE                              5
#define OAM_SDM_SIX                               6
#define OAM_SDM_SEVEN                             7
#define OAM_SDM_EIGHT                             8
#define OAM_SDM_NINE                              9
#define OAM_SDM_TEN                              10
#define OAM_SDM_ELEVEN                           11
#define OAM_SDM_TWELVE                          12
#define OAM_SDM_THIRTEEN                         13
#define OAM_SDM_FOURTEEN                         14
#define OAM_SDM_FIFTEEN                          15
#define OAM_SDM_SIXTEEN                          16
#define OAM_SDM_SEVNTEEN                         17
#define OAM_SDM_EIGHTEEN                         18
#define OAM_SDM_NIGHNTEEN                        19
#define OAM_SDM_TWENTY                           20
#define OAM_SDM_TWENTY_FOUR                      24
#define OAM_SDM_TWO_FIFTY_FIVE                  255
#define OAM_SDM_UPPER_EIGHT_BITS_SET         0xFF00
#define OAM_SDM_LOWER_EIGHT_BITS_SET         0x00FF
#define OAM_SDM_ALL_BIT_SET                    0xFF
#define OAM_SDM_ALL_BIT_ZERO                   0x00
/* SPR 22371 Fix Start */
#define OAM_SDM_MAX_CMD_SIZE		       256
/* SPR 22371 Fix End */

UInt8 time_buffer[TIME_SIZE];
extern prov_request_t prov_req;

/*******************************************************************************
**  Functions Declaration
 *****************************************************************************/
pid_t oam_sdm_fork (void);

pid_t oam_sdm_getpid (void);

    /* KLOCWORK warning fix start */
Int
oam_sdm_execve (const char *filename, char *const argv[],
		                      char *const envp[]);
    /* KLOCWORK warning fix end */
/* + SPR 17439 */
oam_sdm_return_et
oam_sdm_spawn_lte_oam(void);

oam_sdm_return_et
oam_sdm_main(void);
/* - SPR 17439 */

oam_sdm_return_et
oam_sdm_single_thd_init (void);

oam_sdm_return_et
oam_sdm_process_msg (void *p_api_buf);

oam_sdm_return_et
oam_sdm_handle_upgarde_req(UInt8 *p_msg);

oam_sdm_return_et
oam_sdm_existing_software_backup (void);

oam_sdm_return_et
oam_sdm_replace_software(void);

oam_sdm_return_et
oam_sdm_create_time_stamp (void);

oam_sdm_return_et
oam_sdm_get_word_from_header (UInt8 *p_header);

SInt8
oam_sdm_system(const char *string);

void *oam_sdm_memcpy(void *dest, const void *src, size_t n);

oam_sdm_return_et 
oam_sdm_mem_alloc(size_t size, void **p_msg, UInt16 *p_error_code);

oam_sdm_return_et
oam_sdm_send_upgrade_start_resp
(
 UInt8 status,
 UInt16 error_code,
 UInt16 transaction_id,
 UInt8 *err_string
 );

void oam_sdm_encode_get_params_resp_ctrl_blk
(
 char *p_ctrl_blk,
 char resp_status,
 UInt16 err_code,
 UInt8 resvd1,
 UInt32 resvd2,
 UInt8 err_str_len,
 char *err_str
);

void oam_sdm_construct_api_header
(
 UInt8          *p_header,      
 UInt8           version_id,   
 UInt16          src_module_id,  
 UInt16          dst_module_id, 
 UInt16          api_id,         
 UInt16          api_buf_size   
);

SInt8
oam_sdm_remove(const char *string);

oam_sdm_return_et
oam_sdm_send_message (void *p_msg, int dst_module, int *p_error_code);

void oam_sdm_trace_message(
		SInt32 log_level,              /* log level of this message */
		SInt8 *pFile,
		SInt8 *pFun,
		UInt16 Line,
		SInt8* format,         /* format string like in printf */
		...                         /* optional parameters */
		);

void oam_sdm_process_timer_expiry
(
 oam_sdm_timer_t  timer_id,
 void             *p_timer_buf,
 void             *p_context
);

/* SPR 21889 start */
void* oam_sdm_init(	
		void *p_data);
/* SPR 21889 end */

oam_sdm_return_et
oam_sdm_start_upgrade(UInt8 * p_msg);

void oam_sdm_handle_polling_response(UInt8 * p_msg);

oam_sdm_return_et
sdm_get_config_info(void);

void
oam_sdm_construct_mgmt_api_hdr (UInt8 * p_msg,
        UInt16 trans_id,
        UInt16 src_mod_id,
        UInt16 dst_mod_id,
        UInt16 api_id,
        UInt16 api_buf_size,
        /* SPR 17777 fix code removed*/
        UInt8 resvd);

void *
/* SPR 17777 fix */
oam_sdm_create_polling_message (void);

oam_sdm_timer_t
oam_start_new_timer (UInt16 sub_module_id, 
                     UInt16 expiry_thread_id, 
                     UInt16 timer_type, 
                     UInt32 timer_duration,
                     void *buf, 
                     UInt16 buf_size, 
                     oam_sdm_bool_et is_repeated);
char *oam_sdm_strcpy(char *dest, const char *src);
void

oam_sdm_handle_active_ind(void);
oam_sdm_return_et

oam_sdm_get_available_backup_info(void);

void oam_sdm_abort(void);

oam_sdm_return_et oam_sdm_isValidIp4 (UInt8 *p_str);

int oam_sdm_strcmp(const char *s1, const char *s2);

int
oam_sdm_fprintf
(
    FILE *str,
    const char *format,
    ...
);


int
oam_sdm_sprintf (char *str, const char *format, ...);

char *
oam_sdm_strtok(char *str, const char *delim);

oam_sdm_return_et
oam_sdm_create_backup_file_name(Char8 *backup_file_name);

char *
oam_sdm_strcat(char *dest, const char *src);

FILE*
oam_sdm_fopen
(
    const Char8 *filename ,
    const Char8 *mode
);

void
oam_sdm_fclose
(
FILE *
);


void *
oam_memset(void *s, int c, size_t n);

oam_sdm_return_et
oam_sdm_kill
(
 UInt16 proc_id,
 UInt16 signal
);
int snscanf(const char *str, int n, const char *format, ...);
/* + SPR 17439 */
oam_sdm_return_et
oam_sdm_shutdown_all(void);
/* - SPR 17439 */
oam_sdm_return_et

/* SPR 17777 fix */
oam_sdm_handle_active_ind_timer_expiry(void);

oam_sdm_return_et
/* SPR 14360 START */
oam_sdm_handle_poll_timer_expiry(void *p_timer_buf, oam_sdm_timer_t timer_id);
/* SPR 14360 END */
void oam_sdm_signal_handler(int sig);
/* + SPR 17439 */
void oam_sdm_cleanup(void);
/* - SPR 17439 */
void oam_sdm_exit(int status);
size_t oam_sdm_strnlen(const char *s, size_t maxlen);
sighandler_t oam_sdm_signal(int signum, sighandler_t handler);
char *oam_sdm_strncpy(Char8 *s1, const Char8 *s2, size_t n);
int oam_sdm_snprintf(char *str, size_t size, const char *format, ...);
char *oam_sdm_fgets(char *s, int size, FILE *stream);
int
oam_sdm_atoi
(
    const char *str
);

time_t oam_sdm_time(time_t *t);
struct tm *oam_sdm_localtime(const time_t *timep);
struct tm *oam_sdm_localtime_r(const time_t *timep, struct tm *result);

void (*oam_sdm_register_signal_handler (int sig, void (*oam_sdm_signal_handler) (int))) (int);/* Porting changes */

void oam_sdm_exit(int status);
size_t oam_sdm_strlen(const char *s);
size_t oam_sdm_strnlen(const char *s, size_t maxlen);
char *oam_sdm_strncpy(Char8 *s1, const Char8 *s2, size_t n);
int oam_sdm_snprintf(char *str, size_t size, const char *format, ...);
char *oam_sdm_fgets(char *s, int size, FILE *stream);
int oam_sdm_atoi(const char *str);
time_t oam_sdm_time(time_t *t);
struct tm *oam_sdm_localtime(const time_t *timep);
struct tm *oam_sdm_localtime_r(const time_t *timep, struct tm *result);
int oam_sdm_sched_setaffinity(pid_t pid, size_t cpusetsize, cpu_set_t *mask);
/* SPR 22371 Fix Start */
FILE *oam_sdm_popen(const Char8 *p_cmd, const Char8 *p_mode);
int oam_sdm_pclose(FILE *p_stream);
/* SPR 22371 Fix End */

/* SPR 21889 start */ 
void* oam_sdm_init(void *p_data);
/* SPR 21889 end */ 

/* SPR 14360 START */
oam_sdm_return_et oam_sdm_stop_timer(oam_sdm_timer_t timer);
/* SPR 14360 END */

#define SDM_MALLOC              malloc /* Porting changes */
#define SDM_CALLOC              calloc /* Porting changes */
#define SDM_FREE                free /* Porting changes */
#define SDM_SIGTERM             15   /* Porting changes */
#define SDM_SIGKILL             9    /* Porting changes */
#define SDM_SIGINT              2    /* Porting changes */
#endif
