/**vim: ts=4:sw=4:ai:ci:si:et
*/
/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2013 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: lteLogger.h,v  2013/07/16 06.00:00 gur13083 Exp $
 ****************************************************************************
 *
 *  File Description :
 *  This file contains declaration of API, structures and defines
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteLogger.h,v $
 * Initial version
 *
 ****************************************************************************/
#ifndef _LTE_IN_MEMORY_LOGGER_H_
#define _LTE_IN_MEMORY_LOGGER_H_
#include <cspl.h>
#include <sys/mman.h>

    /*
    *   In Memory logger
    */
    
#define LGR_TRACE(is_log_enable, facility_name, log_level, format, ...) 

/* ! \def LGR_FOPEN
*/
#define LGR_FOPEN         fopen
/* ! \def LGR_SHM_OPEN
*/
#define LGR_SHM_OPEN                shm_open

/* ! \def LGR_SHM_UNLINK
*/
#define LGR_SHM_UNLINK              shm_unlink

/* ! \def LGR_FTRUNCATE
*/

#define LGR_FTRUNCATE               ftruncate

/* ! \def LGR_MMAP
*/
#define LGR_MMAP                    mmap

/* ! \def LGR_MUNMAP
*/
#define LGR_MUNMAP                    munmap

/* ! \def LGR_FPRINTF
*/
#define LGR_FPRINTF                   fprintf

/* ! \def LGR_FILE_UNLINK
*/
#define LGR_FILE_REMOVE                 remove

/* ! \def LGR_STRCPY
*/
#define LGR_STRCPY                      strcpy

/* ! \def LGR_FFLUSH
*/
#define LGR_FFLUSH                    fflush

/* ! \def BYTES_IN_MB
*/
#define BYTES_IN_MB                    1048576

/* ! \def LGR_NULL
*/
#define LGR_NULL            (void*)0

/* ! \def LGR_ZERO
*/
#define LGR_ZERO            0

/* ! \def LGR_NEG
*/
#define LGR_NEG             -1


/* ! \def LGR_MAX_LOG_PATH_STR_LEN
*/
#define LGR_MAX_LOG_PATH_STR_LEN        256

/* ! \def LGR_MAX_FILE_NAME_LEN
*/
#define LGR_MAX_FILE_NAME_LEN           (LGR_MAX_LOG_PATH_STR_LEN + 16)

/* ! \def LGR_MAX_FILE_COUNT
*/
#define LGR_MAX_FILE_COUNT              10

/* ! \def LGR_DEFAULT_WAIT_TIME
*/
#define LGR_DEFAULT_WAIT_TIME           2

/* ! \def LGR_THREAD_MAX_STR_NAME
*/
#define LGR_THREAD_MAX_STR_NAME         32

/* ! \def LGR_MAX_THREAD_NAME_LEN
*/
#define LGR_MAX_THREAD_NAME_LEN         32

/* ! \def LGR_MAX_REGION_NAME_LEN
*/
#define LGR_MAX_REGION_NAME_LEN         32

/* ! \def LGR_DEFAULT_SHM_SZ
*/
#define LGR_DEFAULT_SHM_SZ              1

/* ! \def LGR_DEFAULT_FILE_SZ
*/
#define LGR_DEFAULT_FILE_SZ             1

/* ! \def LGR_STDOUT_FS
*/
#define LGR_STDOUT_FS                   stdout

/* ! \def LGR_STDERR_FS
*/
#define LGR_STDERR_FS                   stderr

/* ! \def LGR_DEFAULT_FS
*/
#define LGR_DEFAULT_FS                  LGR_STDERR_FS

typedef void (*call_back_pointer)(void *);


/*! \ lgr_error_et
 *  \brief This enum is used for reporting the error occured during the processing
 */
typedef enum _lgr_error_et
{
    LGR_NO_ERR,
    LGR_IO_ERR,
    LGR_NO_SPACE_ERR
}lgr_error_et;

/*! \ lgr_log_level_et
 *  \brief This enum is used for log level
 */
typedef enum _lgr_log_level
{
    LGR_CRITICAL,
    LGR_ERROR,
    LGR_WARNING,
    LGR_INFO,
    LGR_BRIEF,
    LGR_DETAILED,
    LGR_DEBUG,
    LGR_TRACE
}lgr_log_level_et;

/*! \ lgr_return_et
 *  \brief This enum is used by function to indicate return status of the
 *  function
 */
typedef enum _lgr_return_et
{
    LGR_FAILURE,
    LGR_SUCCESS
}lgr_return_et;

/*! \ lgr_bool_et
 *  \brief This enum is for TRUE/FALSE value used by logger 
 */
typedef enum _lgr_bool_et
{
    LGR_FALSE,
    LGR_TRUE
}lgr_bool_et;

typedef U16     lgr_thread_id_t;
typedef U8      lgr_file_seq_num_t;

/*! \ lgr_file_info_t
 *  \brief This struct contain information about the file names
    \param file_cntr indicates the current count of file that are being written so far
    \param active_log_file_name Name of the file which is being written
    \param purge_log_file_name Name of the file that will be purged when maximum file count is reached
 */
typedef struct _lgr_file_info_t
{
    lgr_file_seq_num_t  file_cntr;
    U8                  log_file_name[LGR_MAX_FILE_COUNT][LGR_MAX_FILE_NAME_LEN];
}lgr_file_info_t;
/*! \ lgr_sh_region_hdr_t
 *  \brief This struct is used as shared memory header
 *  \param p_seg_start Indicate the start of log segment for a thread 
 *  \param p_file_hndlr This contains the file descriptor of the file being used
 *  for logging
 *  \param file_sz_lmt Indicate the maximum limit of each log file
 *  \param cur_file_size Current size of the file in bytes
 *  \param reader_needle Keeps track where logging thread is reading
 *  \param writer_needle Keeps track where writer thread has to write log
 *  \param segment_size Indicate the size of the log segment
 *  \param chunk_cnt Indicate the number of logs to be read from each segment
 *  \param log_discard_cnt Indicate the times when logs are discarded
 *  \param file_cntr Indicate the file sequence number counter
 */
typedef struct _lgr_sh_region_hdr_t
{
    U8                  *p_seg_start;
    FILE                *p_file_hndlr;
    U32                 file_sz_lmt;
    U32                 cur_file_size;
    U32                 reader_needle;
    U32                 writer_needle;
    U32                 segment_size; 
    U32                 chunk_cnt;
    U16                 log_discard_cnt; 
    lgr_file_info_t     file_info;
}lgr_sh_region_hdr_t;

/*! \ lgr_thread_info_rec_t
 *  \brief This struct is used to make the thread table which will keep
 *  information about each of the threads
 *  \param p_sh_region Shared memory of the thread
 *  \param p_call_back Pointer to the function which needs to be called after
 *  processing of each segment of a thread
 *  \param p_cbk_arg Pointer to the argument of the call back function
 *  \param file_cnt Number of log files that needs to be created for a thread
 *  \param p_thread_name Name of the thread that shall be used to create name of
 *  the log file
 */
typedef struct _lgr_thread_info_rec_t
{
    lgr_sh_region_hdr_t *p_sh_region;
    call_back_pointer   p_call_back;
    void                *p_cbk_arg;
    lgr_file_seq_num_t  file_cnt;
    char p_thread_name[LGR_THREAD_MAX_STR_NAME];
}lgr_thread_info_rec_t;

/*! \ lgr_logger_info_t
 *  \brief This struct keeps the track of path on which logs to be created and
 *  information about thread table
 *  \param log_path Log path
 *  \param num_threads Number of threads
 *  \param p_thread_tbl Thread table
 */
typedef struct _logger_info
{
    S8                      log_path[LGR_MAX_LOG_PATH_STR_LEN];
    lgr_thread_id_t         num_threads;
    lgr_thread_info_rec_t   *p_thread_tbl;
    lgr_bool_et             lgr_lib_init;
}lgr_logger_info_t;

/*! \ lgr_shm_logging_config_t
 *  \brief This struct is used to take input parameter on the interface with the
 *  application
 *  \param thread_name Thread name
 *  \param lgr_region_name Shared memory name of the thread
 *  \param p_call_back Callback function
 *  \param p_cbk_arg Argument for the callback function
 *  \param shm_size Size of the shared memory that needs to be created
 *  \param log_file_sz Size of the log file after which new file will be created
 *  \param log_chunk_cnt Number of log string that needs to be read from each of
 *  the chunk
 *  \param file_cnt Maximum file count that needs to be created for each thread
 */
typedef struct _lgr_shm_logging_config_t
{
    /* Coverity ID 59967 Fix Start*/
    char                 thread_name[LGR_MAX_THREAD_NAME_LEN + 1];
    char                 lgr_region_name[LGR_MAX_REGION_NAME_LEN + 1];
    /* Coverity ID 59967 Fix End*/
   call_back_pointer    p_call_back;
   void                 *p_cbk_arg;
   U32                  shm_size;
   U32                  log_file_sz;
   U16                  log_chunk_cnt;
   lgr_file_seq_num_t   file_cnt;
}lgr_shm_logging_config_t;

/*SPR 21275 START*/
typedef struct _lgr_generic_info_t
{
    const char          *p_log_level;
    const char          *p_function_name;
    unsigned int        line;
    U32                 log_sz;
    struct timeval      tv;
    U16                 ue_index;
    U8                  cell_index;
}lgr_generic_info_t;

typedef struct _ptr_node_t
{
    /* SPR 21820 Start */
    char          *ptr;
    U8		  string_type;
    /* SPR 21820 End */
}lgr_ptr_node_t;
/*SPR 21275 START*/
 lgr_return_et lgr_logger_register_thread (lgr_thread_id_t thread_id);
/* SPR 21820 Start */
void print_log_message (U32 logLevel, const char* filePath, unsigned int line,
        const char* function_name, const char* facility, U8 cell_index, U16 ue_index, lgr_bool_et is_dynamic_str_var, const char* format,
        ...);
/* SPR 21820 End */

lgr_return_et lgr_in_mem_logging_init(lgr_thread_id_t num_thread,
        lgr_shm_logging_config_t *p_cfg, const char *p_path);

void lgr_process_log_segments(void);
#endif
