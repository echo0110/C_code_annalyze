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
 *  $Id: lteLogger.c,v  2013/07/16 06.00:00 gur13083 Exp $
 ****************************************************************************
 *
 *  File Description :
 *  This file contains implementation of shared memory based logging
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteLogger.c,v $
 * Initial version
 *
 ****************************************************************************/
#include <lteMisc.h>
#include <pthread.h>
#include "lteLogger.h"

static lgr_logger_info_t g_logger_info = {{LGR_ZERO}, LGR_ZERO, LGR_ZERO, LGR_FALSE};
static pthread_key_t        logger_key = LGR_ZERO;
static pthread_once_t key_create = PTHREAD_ONCE_INIT;

/****************************************************************************
* Function Name  : lgr_alloc_thrd_tbl
* Inputs         : Number of threads
* Outputs        : None
* Returns        : LGR_SUCCESS on successful allocation of thread table
*                  LGR_FAILURE failure in allocation
* Description    : Allocate thread table
****************************************************************************/
static 
lgr_return_et
lgr_alloc_thrd_tbl
(
 lgr_thread_id_t num_threads
 )
{
    U32 tbl_sz = LGR_ZERO;
    tbl_sz = sizeof(lgr_thread_info_rec_t) * num_threads;
    g_logger_info.p_thread_tbl = (lgr_thread_info_rec_t*)malloc_wrapper(tbl_sz);
    if (LGR_NULL == g_logger_info.p_thread_tbl)
    {
        return LGR_FAILURE;
    }
    memSet(g_logger_info.p_thread_tbl, LGR_ZERO, tbl_sz);
    g_logger_info.num_threads = num_threads;
    return LGR_SUCCESS;
}

/****************************************************************************
* Function Name  : lgr_get_thread_info_tbl
* Inputs         : None
* Outputs        : None
* Returns        : Pointer to the global thread table
* Description    : Return global thread table
****************************************************************************/
static  lgr_thread_info_rec_t 
/* + SPR 17439 */
*lgr_get_thread_info_tbl(void)
/* - SPR 17439 */    
{
    if (LGR_NULL != g_logger_info.p_thread_tbl)
    {
        return &g_logger_info.p_thread_tbl[LGR_ZERO];
    }
    else
    {
        return LGR_NULL;
    }
}

/****************************************************************************
* Function Name  : lgr_lib_ready
* Inputs         : Status
* Outputs        : None
* Returns        : None
* Description    : Set the status of the library
****************************************************************************/
static  void lgr_lib_ready(lgr_bool_et status)
{
    g_logger_info.lgr_lib_init = status;
}

/****************************************************************************
* Function Name  : lgr_lib_init
* Inputs         : None
* Outputs        : None
* Returns        : Status of library True when library is initialized otherwise
*                  false
* Description    : Gives status of the library
****************************************************************************/
static  lgr_bool_et lgr_lib_init(void)
{
    return g_logger_info.lgr_lib_init;
}

/****************************************************************************
* Function Name  : lgr_set_log_path
* Inputs         : p_log_path, path_len
* Outputs        : None
* Returns        : None
* Description    : Set the log path where log files will be created for logging
****************************************************************************/
 lgr_return_et
lgr_set_log_path
(
 const char *p_log_path
 )
{
    U16 path_len = strlen_wrapper(p_log_path);
    if (sizeof(g_logger_info.log_path) > path_len)
    {
        strncpy((char*)g_logger_info.log_path, (char*)p_log_path, path_len);
        g_logger_info.log_path[path_len] = '\0';
        LGR_TRACE(LGR_TRUE, p_lgr_utils_facility_name, LGR_DETAILED,
                "Logging path [%s]", g_logger_info.log_path);
        return LGR_SUCCESS;
    }
    else
    {
        LGR_TRACE(LGR_TRUE, p_lgr_utils_facility_name, LGR_ERROR,
                "Path len should be less than [%u]", sizeof(g_logger_info.log_path));
    }

    return LGR_FAILURE;
}

/****************************************************************************
* Function Name  : lgr_get_log_path
* Inputs         : None
* Outputs        : None
* Returns        : Return log path
* Description    : Set the log path where log files will be created for logging
****************************************************************************/
static  void lgr_set_cbk(
        lgr_thread_id_t id,
        call_back_pointer p_cbk,
        void *p_arg
)
{
    lgr_thread_info_rec_t   *p_thread_info_tbl  = LGR_NULL;
    p_thread_info_tbl = lgr_get_thread_info_tbl();
    if (LGR_NULL != p_thread_info_tbl)
    {
        p_thread_info_tbl[id].p_call_back = p_cbk;
        p_thread_info_tbl[id].p_cbk_arg = p_arg;
    }

    return;
}

/****************************************************************************
* Function Name  : lgr_get_log_path
* Inputs         : None
* Outputs        : None
* Returns        : Return log path
* Description    : Set the log path where log files will be created for logging
****************************************************************************/
/* + SPR 17439 */
 const char *lgr_get_log_path
(void)
/* - SPR 17439 */
{
    return (char*)g_logger_info.log_path;
}

/****************************************************************************
* Function Name  : lgr_create_filename_for_log
* Inputs         : size of filename, thread name for which file
*                  needs to be created
* Outputs        : creates files
* Returns        : LGR_SUCCESS/LGR_FAILURE
* Description    : Create filename according to the nomenclature
****************************************************************************/
lgr_return_et
lgr_create_filename_for_log(U8 *p_log_file_name, U32 size, const char *p_thread_name)
{
    struct tm currentTime;
    struct timeval tv;

    gettimeofday_wrapper(&tv, 0);
    localtime_r(&tv.tv_sec, &currentTime);
    /* Corrections to make it human readible */
    currentTime.tm_year += 1900;
    currentTime.tm_mon++;

    if (LGR_ZERO > snprintf((char*)p_log_file_name, size, "%s/%s.log.%04d.%02d.%02d_%02d.%02d.%02d.%06ld",
                            lgr_get_log_path(), p_thread_name, currentTime.tm_year, currentTime.tm_mon, currentTime.tm_mday,
                            currentTime.tm_hour, currentTime.tm_min, currentTime.tm_sec, tv.tv_usec))
    {
        p_log_file_name[LGR_ZERO] = '\0';
        return LGR_FAILURE;
    }
    /* SPR 20585 Fix End */

    return LGR_SUCCESS;
}
/****************************************************************************
* Function Name  : lgr_open_fp_for_store_log
* Inputs         : logical thread_id, file_id 
* Outputs        : None
* Returns        : File handler of new file
* Description    : Open file depending upon the logical thread id and the file
*                  id
****************************************************************************/
FILE *lgr_open_fp_for_store_log
(
 const U8 *p_log_file_name,
 const char *p_mode
 )
{
    FILE *fp;
    if ((LGR_NULL != p_log_file_name) && ('\0' != p_log_file_name[LGR_ZERO]))
    {
        fp = LGR_FOPEN((char*)p_log_file_name, p_mode);
        if (LGR_NULL == fp)
        {
            fp = LGR_DEFAULT_FS;
            LGR_TRACE(LGR_TRUE, p_lgr_utils_facility_name, LGR_ERROR,
                    "File open failed [%s], errno[%u]", log_file_name, errno);
        }
    }
    else
    {
        fp = LGR_DEFAULT_FS;
        LGR_TRACE(LGR_TRUE, p_lgr_utils_facility_name, LGR_ERROR,
                "Error in creating filename", log_file_name, errno);
    }

    LGR_TRACE(LGR_TRUE, p_lgr_utils_facility_name, LGR_BRIEF,
            "Successfully opened file [%s]", log_file_name);
    return fp;
}

/*SPR 21275 Changes Start*/
/****************************************************************************
* Function Name  : lgr_format_specifier_handler 
* Inputs         : p_buf, p_specifier, p_shm
* Outputs        : None
* Returns        : Size of format specifier 
* Description    : Function to return Size of format specifier 
****************************************************************************/
int lgr_format_specifier_handler(char *p_buf ,const char *p_specifier,lgr_sh_region_hdr_t *p_shm)
{
    switch (*p_specifier) 
    {
        case 'f':
        LGR_FPRINTF(p_shm->p_file_hndlr,"%lf", *((double*)p_buf));
        return(sizeof(double));

        case 'u':
        LGR_FPRINTF(p_shm->p_file_hndlr,"%lu", *((long unsigned int*)p_buf));
        return(sizeof(long unsigned int));

        case 'x':
        LGR_FPRINTF(p_shm->p_file_hndlr,"%lx", *((long unsigned int*)p_buf));
        return(sizeof(long unsigned int));

        case 'X':
        LGR_FPRINTF(p_shm->p_file_hndlr,"%lX", *((long unsigned int*)p_buf));
        return(sizeof(long unsigned int));

        case 'i':
        LGR_FPRINTF(p_shm->p_file_hndlr,"%li", *((long*)p_buf));
        return(sizeof(long));

        case 'd':
        LGR_FPRINTF(p_shm->p_file_hndlr,"%ld", *((long*)p_buf));
        return(sizeof(long));
    }
    return 0;
}

/****************************************************************************
* Function Name  : lgr_read_binary_data 
* Inputs         : p_buf, p_shm
* Outputs        : None
* Returns        : None
* Description    : Read the logs from the shared memory written in binary 
*                  format
****************************************************************************/
U32 lgr_read_binary_data(char *p_buf, lgr_sh_region_hdr_t *p_shm, const char *function_name, int line)
{

    U32  string_dump_size = LGR_ZERO;
    U32  string_print_size;
    lgr_ptr_node_t *ptr_node = (lgr_ptr_node_t*)p_buf;
    const char *p_specifier = ptr_node->ptr;
    p_buf += sizeof(lgr_ptr_node_t);

    while (*p_specifier)
    {
        if ('%' == *p_specifier)
        {
            p_specifier++;
            while(isdigit(*p_specifier) || (*p_specifier == '.')|| (*p_specifier == '-')) /* Skipping the formatting digit */
            {
                p_specifier++;
            }

            switch (*p_specifier) {
                case 'p':              /* pointer */
                /*SPR 22121 Start*/
                if (LGR_ZERO < (string_print_size = LGR_FPRINTF(p_shm->p_file_hndlr,"0x%x", *((int*)p_buf))))
                {
                    string_dump_size += string_print_size;
                }
                p_buf +=sizeof(int);
                break;

                /*SPR 22121 End*/
                case 'x':
                if (LGR_ZERO < (string_print_size = LGR_FPRINTF(p_shm->p_file_hndlr,"%x", *((int*)p_buf))))
                {
                    string_dump_size += string_print_size;
                }
                p_buf +=sizeof(int);
                break;

                case 'X':
                if (LGR_ZERO < (string_print_size = LGR_FPRINTF(p_shm->p_file_hndlr,"%X", *((int*)p_buf))))
                {
                    string_dump_size += string_print_size;
                }
                p_buf +=sizeof(int);
                break;


                case 'u':
                if (LGR_ZERO < (string_print_size = LGR_FPRINTF(p_shm->p_file_hndlr,"%u", *((int*)p_buf))))
                {
                    string_dump_size += string_print_size;
                }
                p_buf +=sizeof(int);
                break;

                case 'd':              /* int */
                if (LGR_ZERO < (string_print_size = LGR_FPRINTF(p_shm->p_file_hndlr,"%d", *((int*)p_buf))))
                {
                    string_dump_size += string_print_size;
                }
                p_buf +=sizeof(int);
                break;

                case 'g':
                if (LGR_ZERO < (string_print_size = LGR_FPRINTF(p_shm->p_file_hndlr,"%g", *((double*)p_buf))))
                {
                    string_dump_size += string_print_size;
                }
                p_buf +=sizeof(double);
                break;

                case 'G':
                if (LGR_ZERO < (string_print_size = LGR_FPRINTF(p_shm->p_file_hndlr,"%G", *((double*)p_buf))))
                {
                    string_dump_size += string_print_size;
                }
                p_buf +=sizeof(double);
                break;

                case 'f':
                if (LGR_ZERO < (string_print_size = LGR_FPRINTF(p_shm->p_file_hndlr,"%f", *((double*)p_buf))))
                {
                    string_dump_size += string_print_size;
                }
                p_buf +=sizeof(double);
                break;

                case 'o':
                if (LGR_ZERO < (string_print_size = LGR_FPRINTF(p_shm->p_file_hndlr,"%f", *((double*)p_buf))))
                {
                    string_dump_size += string_print_size;
                }
                p_buf +=sizeof(double);
                break;

                case 'e':
                if (LGR_ZERO < (string_print_size = LGR_FPRINTF(p_shm->p_file_hndlr,"%e", *((double*)p_buf))))
                {
                    string_dump_size += string_print_size;
                }
                p_buf +=sizeof(double);
                break;

                case 'E':
                if (LGR_ZERO < (string_print_size = LGR_FPRINTF(p_shm->p_file_hndlr,"%E", *((double*)p_buf))))
                {
                    string_dump_size += string_print_size;
                }
                p_buf +=sizeof(double);
                break;

                case 'c':              /* char */
                if (LGR_ZERO < (string_print_size = LGR_FPRINTF(p_shm->p_file_hndlr,"%c", *((char*)p_buf))))
                {
                    string_dump_size += string_print_size;
                }
                p_buf +=sizeof(char);
                break;

                case 's':              /* string */
                ptr_node = (lgr_ptr_node_t*)p_buf;
		/* SPR 21820 Start */
		if (LGR_FALSE == ptr_node->string_type)
		{
			if (LGR_ZERO < (string_print_size = LGR_FPRINTF(p_shm->p_file_hndlr,"%s", ptr_node->ptr)))
			{
				string_dump_size += string_print_size;
			}
			p_buf +=sizeof(lgr_ptr_node_t);
		}
		else
		{
			ptr_node->ptr = (char *)(p_buf + sizeof(lgr_ptr_node_t));
			if (LGR_ZERO < (string_print_size = LGR_FPRINTF(p_shm->p_file_hndlr,"%s", ptr_node->ptr)))
			{
				string_dump_size += string_print_size;
			}
			p_buf +=sizeof(lgr_ptr_node_t) + strlen(ptr_node->ptr) + 1;
		}
		/* SPR 21820 End */
                break;

                case 'l':          
                p_specifier++;
                if (*p_specifier == 'l')
                {
                    p_specifier++;
                    switch(*p_specifier)
                    {
                        case 'u':
                        if (LGR_ZERO < (string_print_size = LGR_FPRINTF(p_shm->p_file_hndlr,"%llu", *((unsigned long long*)p_buf))))
                        {
                            string_dump_size += string_print_size;
                        }
                        p_buf += sizeof(unsigned long long);
                        break;

                        case 'i':
                        if (LGR_ZERO < (string_print_size = LGR_FPRINTF(p_shm->p_file_hndlr,"%lli", *((long long*)p_buf))))
                        {
                            string_dump_size += string_print_size;
                        }
                        p_buf += sizeof(long long);
                        break;

                        case 'd':
                        if (LGR_ZERO < (string_print_size = LGR_FPRINTF(p_shm->p_file_hndlr,"%lld", *((long long*)p_buf))))
                        {
                            string_dump_size += string_print_size;
                        }
                        p_buf += sizeof(long long);
                        break;
                    }	
                }
                else
                    p_buf += lgr_format_specifier_handler(p_buf, p_specifier, p_shm);
                break;

                case 'L': 
                p_specifier++;
                switch(*p_specifier)
                {
                    case 'f':
                    if (LGR_ZERO < (string_print_size = LGR_FPRINTF(p_shm->p_file_hndlr,"%Lf", *((long double*)p_buf))))
                    {
                        string_dump_size += string_print_size;
                    }
                    p_buf += sizeof(long double);
                    break;

                }
                break;

                case 'h':        
                p_specifier++;
                switch(*p_specifier)
                {
                    case 'u':
                    if (LGR_ZERO < (string_print_size = LGR_FPRINTF(p_shm->p_file_hndlr,"%hu", *((unsigned short*)p_buf))))
                    {
                        string_dump_size += string_print_size;
                    }
                    p_buf += sizeof(unsigned short);
                    break;

                    case 'i':
                    if (LGR_ZERO < (string_print_size = LGR_FPRINTF(p_shm->p_file_hndlr,"%hi", *((short*)p_buf))))
                    {
                        string_dump_size += string_print_size;
                    }
                    p_buf +=sizeof(short);
                    break;
                }
                break;

                case '%':             
                if (LGR_ZERO < (string_print_size = LGR_FPRINTF(p_shm->p_file_hndlr,"%c", *p_specifier)))
                {
                    string_dump_size += string_print_size;
                }
                break;

                default:
                /* Should never reach here */
                abort();
                break;
            }
        }
        else
        {
            if (LGR_ZERO < (string_print_size = LGR_FPRINTF(p_shm->p_file_hndlr,"%c", *p_specifier)))
            {
                string_dump_size += string_print_size;
            }
        }
        p_specifier++;
    }
    if (LGR_ZERO < (string_print_size = LGR_FPRINTF(p_shm->p_file_hndlr,"\n")))
    {
        string_dump_size += string_print_size;
    }
    return string_dump_size;
}

/****************************************************************************
* Function Name  : lgr_dump_log_to_file
* Inputs         : p_shm, thrd_id
* Outputs        : None
* Returns        : None
* Description    : Read the logs from the shared memory and dump it in the
* corresponding thread log file
****************************************************************************/
static void 
lgr_dump_log_to_file
(
 lgr_sh_region_hdr_t *p_shm,
 lgr_thread_id_t    thrd_id
 )
{
    U32 reader_needle;
    U32 writer_needle;
    U32 chunk_cnt;
    
    lgr_generic_info_t *p_gen_info;
    
    struct tm currentTime;
    U8 *p_buf;
    lgr_thread_info_rec_t   *p_thread_info_tbl  = LGR_NULL;
    
    S32 current_file_sz = LGR_ZERO;
    

    reader_needle = p_shm->reader_needle;
    writer_needle = p_shm->writer_needle;
    
    chunk_cnt = p_shm->chunk_cnt;

    while ((writer_needle != reader_needle) && (chunk_cnt > LGR_ZERO))
    {
        /* Read size of log to be read */
        p_buf = p_shm->p_seg_start + reader_needle;

        
        /* read generic information */
        p_gen_info = (lgr_generic_info_t *)(p_buf);
        p_buf += sizeof(lgr_generic_info_t);

        if (LGR_NEG == p_gen_info->log_sz)
        {
            /* Reset to zero as their is no further log beyond this */
            LGR_TRACE(LGR_TRUE, p_lgr_utils_facility_name, LGR_DETAILED,
                    "Resetting reader_needle for thread [%u]", thrd_id);
            reader_needle = LGR_ZERO;
            p_shm->reader_needle = reader_needle;
            continue;
        }

        localtime_r(&p_gen_info->tv.tv_sec, &currentTime);
        /* Dump the actual log */
        if (LGR_ZERO < (current_file_sz = LGR_FPRINTF(p_shm->p_file_hndlr, "%02d:%02d:%02d.%06ld "
                , currentTime.tm_hour, currentTime.tm_min,
                currentTime.tm_sec,(long int)p_gen_info->tv.tv_usec)))
        {
            p_shm->cur_file_size += current_file_sz;
        }


        if (LGR_ZERO < (current_file_sz = LGR_FPRINTF(p_shm->p_file_hndlr, "%-8s %-3u %-5u %s(%u): ",
                p_gen_info->p_log_level, p_gen_info->cell_index, p_gen_info->ue_index,
                p_gen_info->p_function_name,p_gen_info->line)))
        {
            p_shm->cur_file_size += current_file_sz;
        }

        p_shm->cur_file_size += lgr_read_binary_data((char *)p_buf, p_shm, p_gen_info->p_function_name, p_gen_info->line);

        reader_needle += p_gen_info->log_sz;

        if (p_shm->file_sz_lmt <= p_shm->cur_file_size)
        {
            LGR_TRACE(LGR_TRUE, p_lgr_utils_facility_name, LGR_DETAILED,
                    "Rotating the file for thread [%u]: limit is [%u],"
                    " used[%u]", thrd_id, p_shm->file_sz_lmt,
                    p_shm->cur_file_size);

            if (LGR_ZERO !=  p_shm->log_discard_cnt)
            {
                LGR_FPRINTF(p_shm->p_file_hndlr, "[%u] logs are discarded. Increase Shared size above [%u]",
                p_shm->log_discard_cnt, p_shm->segment_size);
            }
            fClose(p_shm->p_file_hndlr);
            p_shm->cur_file_size = LGR_ZERO;
            p_thread_info_tbl = lgr_get_thread_info_tbl();
            if (LGR_NULL != p_thread_info_tbl)
            {
                p_shm->file_info.file_cntr++;
                p_shm->file_info.file_cntr = p_shm->file_info.file_cntr % p_thread_info_tbl[thrd_id].file_cnt;
                if (LGR_ZERO != LGR_FILE_REMOVE((char*)p_shm->file_info.log_file_name[p_shm->file_info.file_cntr]))
                {
                    LGR_FPRINTF(LGR_DEFAULT_FS, "Failed to remove file [%s]",
                            p_shm->file_info.log_file_name[p_shm->file_info.file_cntr]);
                }
                lgr_create_filename_for_log(p_shm->file_info.log_file_name[p_shm->file_info.file_cntr],
                        sizeof(p_shm->file_info.log_file_name[p_shm->file_info.file_cntr]),
                        p_thread_info_tbl[thrd_id].p_thread_name);
                p_shm->p_file_hndlr = lgr_open_fp_for_store_log(p_shm->file_info.log_file_name[p_shm->file_info.file_cntr], "w");
            }
            else
            {
                p_shm->p_file_hndlr = LGR_DEFAULT_FS;
                LGR_TRACE(LGR_TRUE, p_lgr_utils_facility_name, LGR_FATAL,
                        "Thread tbl not accessible !!!");

            }
        }

        if (((p_shm->segment_size - reader_needle) < sizeof(lgr_generic_info_t)) ||
            (/ == p_shm->segment_size))
        {
            LGR_TRACE(LGR_TRUE, p_lgr_utils_facility_name, LGR_DETAILED,
                    "Resetting reader_needle for thread [%u], writer_needle[%u], reader_needle[%u]", thrd_id, writer_needle, reader_needle);
            /* Reset to zero as their is no further log beyond this */
            reader_needle = LGR_ZERO;
        }

        /* Log successfully dumped to file make space for writer */
        p_shm->reader_needle = reader_needle;
        chunk_cnt--;
    }
    LGR_FFLUSH(p_shm->p_file_hndlr);

    return;
    
}

/****************************************************************************
* Function Name  : lgr_init_shm_mem
* Inputs         : thread id, log file name, shared memory size
* Outputs        : None
* Returns        : SUCCESS/FAILURE
* Description    : This function initialize the shared memory for the
                   RRM logger.
****************************************************************************/
static lgr_return_et
lgr_init_shm_mem
(
   lgr_thread_id_t      thread_id,
   const char           *p_thread_name,
   const char           *p_lgr_region_name,
   U32                  shm_size,
   U32                  file_sz,
   U32                  chunk_cnt,
   lgr_file_seq_num_t   file_cnt
)
{
    S32                     shm_fd              = LGR_ZERO;
    U32                     max_shm_size        = LGR_ZERO;
    U32                     max_log_seg_sz      = LGR_ZERO;
    void              *p_shmptr           = LGR_NULL;
    lgr_sh_region_hdr_t     *p_shm_header       = LGR_NULL;
    lgr_thread_info_rec_t   *p_thread_info_tbl  = LGR_NULL;


    /* Shared memory size provided should not be less than default size */
    if (LGR_DEFAULT_SHM_SZ > shm_size)
    {
        return LGR_FAILURE;
    }

    /* file size provided should not be less than default size */
    if (LGR_DEFAULT_FILE_SZ > file_sz)
    {
        return LGR_FAILURE;
    }

    if ((LGR_ZERO == file_cnt) || (LGR_MAX_FILE_COUNT < file_cnt))
    {
        return LGR_FAILURE;
    }

    p_thread_info_tbl = lgr_get_thread_info_tbl();
    if (LGR_NULL == p_thread_info_tbl)
    {
        return LGR_FAILURE;
    }
    strncpy(p_thread_info_tbl[thread_id].p_thread_name,p_thread_name,sizeof((p_thread_info_tbl[thread_id].p_thread_name)));
    p_thread_info_tbl[thread_id].p_thread_name[sizeof(p_thread_info_tbl[thread_id].p_thread_name)-1]='\0';

    p_thread_info_tbl[thread_id].file_cnt = file_cnt;

    max_log_seg_sz = shm_size * BYTES_IN_MB; 
    max_shm_size = max_log_seg_sz + sizeof(lgr_sh_region_hdr_t); 

    /*Creating shared memory for logger */
    shm_fd = LGR_SHM_OPEN(p_lgr_region_name, O_CREAT | O_RDWR | O_EXCL, S_IRWXU);
    if(shm_fd < LGR_ZERO)
    {
        if (errno == EEXIST)
        {
            if (LGR_ZERO != LGR_SHM_UNLINK(p_lgr_region_name))
            {
                LGR_TRACE(LGR_TRUE, p_lgr_utils_facility_name, LGR_ERROR,
                        "Failed to unlink shared memory [%s]", p_lgr_region_name);
                return LGR_FAILURE;
            }
            return lgr_init_shm_mem(thread_id, p_thread_name,
                    p_lgr_region_name, shm_size,file_sz,chunk_cnt, file_cnt);

        }
        else
        {
            LGR_TRACE(LGR_TRUE, p_lgr_utils_facility_name, LGR_ERROR,
                    "Failed to open shared memory [%s], errno[%u]", p_lgr_region_name, errno);
            return LGR_FAILURE;
        }
    }
    else
    {
        /* Fresh open */
        if(LGR_ZERO > LGR_FTRUNCATE(shm_fd, max_shm_size))
        {
            LGR_TRACE(LGR_TRUE, p_lgr_utils_facility_name, LGR_ERROR,
                    "Failed to dimension shared memory [%s], errno[%u]", p_lgr_region_name, errno);
            LGR_SHM_UNLINK(p_lgr_region_name);
            return LGR_FAILURE;
        }

        LGR_TRACE(LGR_TRUE, p_lgr_utils_facility_name, LGR_BRIEF,
                "Fresh shared memory [%s]", p_lgr_region_name);

        /*Requesting the shared segment*/
        p_shmptr =  LGR_MMAP(LGR_NULL, max_shm_size, PROT_READ|PROT_WRITE,
                MAP_SHARED, shm_fd, LGR_ZERO);

        if(MAP_FAILED == p_shmptr)
        {
            LGR_TRACE(LGR_TRUE, p_lgr_utils_facility_name, LGR_ERROR,
                    "Failed to attach shared memory [%s], errno[%u]", p_lgr_region_name, errno);
            LGR_SHM_UNLINK(p_lgr_region_name);
            return LGR_FAILURE;
        }


        p_shm_header = (lgr_sh_region_hdr_t *)p_shmptr;
        p_shm_header->p_seg_start = p_shmptr + sizeof(lgr_sh_region_hdr_t);
        p_shm_header->file_sz_lmt = file_sz * BYTES_IN_MB;
        p_shm_header->reader_needle = LGR_ZERO;
        p_shm_header->writer_needle = LGR_ZERO;
        p_shm_header->segment_size = max_log_seg_sz;
        p_shm_header->chunk_cnt = chunk_cnt;
        p_shm_header->log_discard_cnt = LGR_ZERO;
        p_shm_header->cur_file_size = LGR_ZERO;
        p_shm_header->file_info.file_cntr = LGR_ZERO;

        lgr_create_filename_for_log(p_shm_header->file_info.log_file_name[p_shm_header->file_info.file_cntr],
                sizeof(p_shm_header->file_info.log_file_name[p_shm_header->file_info.file_cntr]), p_thread_name);

        p_shm_header->p_file_hndlr = lgr_open_fp_for_store_log(p_shm_header->file_info.log_file_name[p_shm_header->file_info.file_cntr], "w");
    }

    p_thread_info_tbl[thread_id].p_sh_region = p_shmptr;
    lgr_lib_ready(LGR_TRUE);

    LGR_TRACE(LGR_TRUE, p_lgr_utils_facility_name, LGR_BRIEF,
            "Successfully initialized shared memory [%s] for thread[%u]", p_lgr_region_name, thread_id);
    return LGR_SUCCESS;
}

/****************************************************************************
* Function Name  : lgr_in_mem_logging_init
* Inputs         : num_thread, p_cfg(array of configuration), p_path
* Outputs        : None
* Returns        : SUCCESS/FAILURE
* Description    : This function initialize the logger 
****************************************************************************/
lgr_return_et
lgr_in_mem_logging_init(
        lgr_thread_id_t num_thread,
        lgr_shm_logging_config_t *p_cfg,
        const char *p_path)
{
    lgr_thread_id_t index;
    lgr_return_et   ret_val = LGR_SUCCESS;

    if (LGR_SUCCESS != lgr_alloc_thrd_tbl(num_thread))
    {
        return LGR_FAILURE;
    }

    if (LGR_SUCCESS != lgr_set_log_path(p_path))
    {
        ret_val = LGR_FAILURE;
    }
    else
    {
        for (index = LGR_ZERO; index < num_thread; index++)
        {
            lgr_set_cbk(index, p_cfg[index].p_call_back, p_cfg[index].p_cbk_arg);

            if (LGR_SUCCESS != lgr_init_shm_mem(index,p_cfg[index].thread_name,
                        p_cfg[index].lgr_region_name, p_cfg[index].shm_size,
                        p_cfg[index].log_file_sz, p_cfg[index].log_chunk_cnt,
                        p_cfg[index].file_cnt))
            {
                LGR_TRACE(LGR_TRUE, p_lgr_utils_facility_name, LGR_ERROR,
                        "Failed to initialize shared memory for thread [%u]", index);
                ret_val = LGR_FAILURE;
            }
        }
    }

    return ret_val;
}

/****************************************************************************
* Function Name  : lgr_logger_create_key
* Inputs         : None
* Outputs        : None
* Returns        : None
* Description    : Create key for storing thread information
****************************************************************************/
/* + SPR 17439 */
static void
lgr_logger_create_key
(void)
/* - SPR 17439 */
{
    if (LGR_ZERO > pthread_key_create(&logger_key, LGR_NULL))
    {
        LGR_TRACE(LGR_TRUE, p_lgr_utils_facility_name, LGR_ERROR,
                "Logger key creation failed");
    }
}

/****************************************************************************
* Function Name  : lgr_logger_register_thread
* Inputs         : Logical thread id
* Outputs        : None
* Returns        : LGR_SUCCESS on succesful registration of the thread
*                  LGR_FAILURE on error
* Description    : Register the logical thread id of the thread
****************************************************************************/
 lgr_return_et
lgr_logger_register_thread
(
 lgr_thread_id_t thread_id
 )
{
    lgr_thread_id_t *p_thread_id = (lgr_thread_id_t *)malloc_wrapper(sizeof(lgr_thread_id_t));

    if (LGR_NULL == p_thread_id)
    {
        LGR_TRACE(LGR_TRUE, p_lgr_utils_facility_name, LGR_ERROR,
                "Ctxt Allocation for Thread [0x%x] failed", thread_id);
        return LGR_FAILURE;
    }

    pthread_once(&key_create, lgr_logger_create_key);

    *p_thread_id = thread_id;

    if (LGR_ZERO > pthread_setspecific(logger_key, p_thread_id))
    {
        LGR_TRACE(LGR_TRUE, p_lgr_utils_facility_name, LGR_ERROR,
                "Thread [0x%x] registration failed", thread_id);
        return LGR_FAILURE;
    }

    return LGR_SUCCESS;
}

/****************************************************************************
* Function Name  : lgr_get_self_id
* Inputs         : None
* Outputs        : None
* Returns        : Thread id of the thread from which it is being called
* Description    : Return the logical thread id of the thread
****************************************************************************/
/* + SPR 17439 */
static  lgr_thread_id_t lgr_get_self_id(void)
/* - SPR 17439 */
{
    lgr_thread_id_t *p_thrd_id;
    p_thrd_id = (lgr_thread_id_t*)pthread_getspecific(logger_key);
    if (LGR_NULL != p_thrd_id)
    {
        return *p_thrd_id;
    }

    return LGR_ZERO;
}


/****************************************************************************
* Function Name  : lgr_loglevel_to_string
* Inputs         : Log level
* Outputs        : None
* Returns        : Return the character string of the log level
* Description    : Return the corresponding string depending upon the log level
****************************************************************************/
static const char* lgr_loglevel_to_string(U32 level)
{
    switch (level)
    {
        case LGR_CRITICAL:
            return "Fatal";
        case LGR_ERROR:
            return "Error";
        case LGR_WARNING:
            return "Warning";
        case LGR_INFO:
            return "Info";
        case LGR_BRIEF:
            return "Brief";
        case LGR_DETAILED:
            return "Detailed";
        case LGR_DEBUG:
            return "Debug";
        default:
            return "Trace";
    }
}

/****************************************************************************
* Function Name  : lgr_get_space_avl
* Inputs         : p_shm
* Outputs        : None
* Returns        : Space available in shared memory for writing log
* Description    : This function calculate the available space in shared memory
****************************************************************************/
static U32 lgr_get_space_avl(
        lgr_sh_region_hdr_t *p_shm
        )
{
    U32 space_avl = LGR_ZERO;
    U32 reader_needle;
    U32 writer_needle;

    reader_needle = p_shm->reader_needle;
    writer_needle = p_shm->writer_needle;
    
    if (writer_needle >= reader_needle)
    {
        /* Reader has read what we have dumped */
        space_avl = p_shm->segment_size - writer_needle;
    }
    else
    {
        /* One byte is used to indicate the difference between queue full
         * and queue empty condition */
        space_avl = reader_needle - writer_needle - 1;
    }
    
    if (space_avl < sizeof(lgr_generic_info_t))
    {
        return LGR_ZERO;
    }

    return space_avl;
}


/****************************************************************************
* Function Name  : lgr_print_log_in_binary 
* Inputs         : p_buf, space_avl, line, *p_specifier, ap
* Outputs        : None
* Returns        : LGR_SUCCESS/LGR_FAILURE
* Description    : Dump the log in the shared memory in binary format.
****************************************************************************/
/* SPR 21820 Start */
int lgr_print_log_in_binary(char *p_buf ,U32 space_avl , lgr_bool_et is_dynamic_str_var, const char *p_specifier, va_list ap)
/* SPR 21820 End */
{
    char *init_pos = p_buf;
    /* SPR 21820 Start */
    U32  ptr_len = LGR_ZERO;
    char *p_temp_ptr;
    /* SPR 21820 End */
    lgr_ptr_node_t *ptr_node = (lgr_ptr_node_t*)p_buf;
    if(space_avl < sizeof(lgr_ptr_node_t))
    {
        return LGR_NEG;
    }
    /* SPR 21820 Start */
    ptr_node->ptr = (char *)p_specifier;
    ptr_node->string_type = LGR_FALSE;
    /* SPR 21820 End */
    p_buf += sizeof(lgr_ptr_node_t);
    space_avl -= sizeof(lgr_ptr_node_t);

    while (*p_specifier)
    {
        if ('%' == *p_specifier)
        {
            p_specifier++;
            while(isdigit(*p_specifier) || (*p_specifier == '.') || (*p_specifier == '-')) /* Skipping the formatting digit */
            {
                p_specifier++;
            }

            switch (*p_specifier) {
                case 'x':              
                case 'X':              
                case 'd':             
                case 'p':           
                case 'u':
                if(space_avl < sizeof(int))
                {
                    return LGR_NEG;
                }
                *((int*)p_buf) = va_arg(ap, int);
                p_buf += sizeof(int);
                space_avl -= sizeof(int);
                break;

                case 'g':
                case 'G':
                case 'f':
                case 'o':
                case 'e':
                case 'E':
                if(space_avl < sizeof(double))
                {
                    return LGR_NEG;
                }
                *((double*)p_buf) = (double)va_arg(ap, double);
                p_buf += sizeof(double);
                space_avl -= sizeof(double);
                break;

                case 'c':            
                if(space_avl < sizeof(char))
                {
                    return LGR_NEG;
                }
                *((char*)p_buf) = (char) va_arg(ap, int);
                p_buf++;
                space_avl -= sizeof(char);
                break;

                case 's':          
                /* SPR 21820 Start */
		 p_temp_ptr = (char*) va_arg(ap, char*);
		if(LGR_FALSE == is_dynamic_str_var)
		{
			if(space_avl < sizeof(lgr_ptr_node_t))
			{
				return LGR_NEG;
			}
			ptr_node = (lgr_ptr_node_t*)p_buf;
			ptr_node->ptr = p_temp_ptr;
			ptr_node->string_type = LGR_FALSE;
			p_buf += sizeof(lgr_ptr_node_t);
			space_avl -= sizeof(lgr_ptr_node_t);
		}
		else
		{
			ptr_len = strlen(p_temp_ptr);
			if(space_avl < (sizeof(lgr_ptr_node_t) + ptr_len + 1))
			{
				return LGR_NEG;
			}
			ptr_node = (lgr_ptr_node_t*)p_buf;
			ptr_node->ptr = p_buf + sizeof(lgr_ptr_node_t);
			ptr_node->string_type = LGR_TRUE;
			strncpy(ptr_node->ptr, p_temp_ptr, ptr_len);
			ptr_node->ptr[ptr_len] = '\0';
			p_buf += sizeof(lgr_ptr_node_t) + ptr_len + 1;   
			space_avl = space_avl - (sizeof(lgr_ptr_node_t) + ptr_len + 1);
                }
		/* SPR 21820 End */
		break;

                case 'l':           
                p_specifier++;
                if (*p_specifier == 'l')
                {
                    p_specifier++;
                    switch(*p_specifier)
                    {
                        case 'u':
                        case 'i':
                        case 'd':
                        if(space_avl < sizeof(long long))
                        {
                            return LGR_NEG;
                        }
                        *((long long*)p_buf) = (long long)va_arg(ap, long long);
                        p_buf += sizeof(long long);
                        space_avl -= sizeof(long long);
                        break;
                        default:
                        LGR_FPRINTF(LGR_DEFAULT_FS,"l Write Could not find %c %c\n", *p_specifier, *(p_specifier + 1));
                        abort();
                    }	
                }
                else
                {
                  switch (*p_specifier) 
                  {    
                    case 'f': 
                      if(space_avl < sizeof(double))
                      {    
                        return LGR_NEG;
                      }    
                      *((double*)p_buf) = (double)va_arg(ap, double);
                      space_avl -= sizeof(double);
                      p_buf += sizeof(double);
                      break;

                    case 'u': 
                    case 'x': 
                    case 'X': 
                      if(space_avl < sizeof(long unsigned int))
                      {    
                        return LGR_NEG;
                      }    
                      *((long unsigned int*)p_buf) = (long unsigned int)va_arg(ap, long unsigned int);
                      space_avl -= sizeof(long unsigned int);
                      p_buf += sizeof(long unsigned int);
                      break;


                    case 'd': 
                    case 'i': 
                      if(space_avl < sizeof(long))
                      {
                        return LGR_NEG;
                      }
                      *((long*)p_buf) = (long)va_arg(ap, long);
                      space_avl -= sizeof(long);
                      p_buf += sizeof(long);
                      break;

                    default:
                      LGR_FPRINTF(LGR_DEFAULT_FS,"function Write Could not find %c %c\n", *p_specifier, *(p_specifier + 1));
                      abort();
                  }
                }
                break;

                case 'L':             
                p_specifier++;
                if (*p_specifier == 'f')
                {	
                    if(space_avl < sizeof(long double))
                    {
                        return LGR_NEG;
                    }
                    *((long double*)p_buf) = (long double)va_arg(ap, long double);
                    p_buf += sizeof(long double);
                    space_avl -= sizeof(long double);

                }
                break;

                case 'h':            
                p_specifier++;
                switch(*p_specifier)
                {
                    case 'u':
                    case 'i':
                    if(space_avl < sizeof(short))
                    {
                        return LGR_NEG;
                    }
                    *((short*)p_buf) = (short)va_arg(ap, int);
                    p_buf += sizeof(short);
                    space_avl -= sizeof(short);
                    break;

                    default:
                    LGR_FPRINTF(LGR_DEFAULT_FS,"h Write Could not find %c %c\n", *p_specifier, *(p_specifier + 1));
                    abort();

                }
                break;

                case '%':             
                break;

                default:
                LGR_FPRINTF(LGR_DEFAULT_FS,"Write Could not find %c %c\n", *p_specifier, *(p_specifier + 1));
                abort();
                break;
            }
        }
        p_specifier++;
    }
    return(p_buf - init_pos);

}

/****************************************************************************
* Function Name  : dump_log_message
* Inputs         : p_shm, p_buf, space_avl, p_err, logLevel, filePath, line,
*                  function_name, facility, cell_index, ue_index, format, ap
* Outputs        : None
* Returns        : LGR_SUCCESS/LGR_FAILURE
* Description    : Dump the log in the shared memory
****************************************************************************/
static lgr_return_et
dump_log_message
(
 lgr_sh_region_hdr_t *p_shm,
 U8 *p_buf,
 U32 space_avl,
 lgr_error_et *p_err,
 U32 logLevel,
 const char* filePath,
 unsigned int line,
 const char* function_name,
 const char* facility,
 U8 cell_index, 
 U16 ue_index, 
 /* SPR 21820 Start */
 lgr_bool_et is_dynamic_str_var,
 /* SPR 21820 End */
 const char* format,
 va_list ap
 )
{
	/* + coverity fix 54918 */
    S32 print_len;
	/* - coverity fix 54918 */
    U32 writer_needle;
    
    lgr_generic_info_t *p_gen_info;
    writer_needle = p_shm->writer_needle;

    if (space_avl < sizeof(lgr_generic_info_t))
    {
        LGR_TRACE(LGR_TRUE, p_lgr_utils_facility_name, LGR_ERROR,
                "No space available: SegSz[%lu]:LogSz[%lu]:SpcAvl[%lu]:Wtr[%lu]:PrevDisCrd[%u]\n",
                p_shm->segment_size, sizeof(lgr_generic_info_t), space_avl, writer_needle, p_shm->log_discard_cnt);
        *p_err = LGR_NO_SPACE_ERR;
        return LGR_FAILURE;
    }
    space_avl -= sizeof(lgr_generic_info_t);

    /* Generic Info */
    p_gen_info = (lgr_generic_info_t*)(p_buf);
    p_buf += sizeof(lgr_generic_info_t);


    /* Actual log string */
    /* SPR 21820 Start */
    print_len = lgr_print_log_in_binary((char*)p_buf, space_avl, is_dynamic_str_var, format, ap);
    /* SPR 21820 End */
    if (LGR_ZERO > print_len)
    {
        LGR_TRACE(LGR_TRUE, p_lgr_utils_facility_name, LGR_ERROR,
                "No space available: SegSz[%lu]:LogSz[%lu]:SpcAvl[%lu]:Wtr[%lu]:PrevDisCrd[%u]\n",
                p_shm->segment_size, print_len, space_avl, writer_needle, p_shm->log_discard_cnt);
        *p_err = LGR_NO_SPACE_ERR;
        p_gen_info->log_sz = LGR_NEG;
        return LGR_FAILURE;
    }

    
    p_gen_info->p_log_level = lgr_loglevel_to_string(logLevel);
    p_gen_info->line = line;
    p_gen_info->p_function_name = function_name;
    p_gen_info->cell_index = cell_index;
    p_gen_info->ue_index = ue_index;
    p_gen_info->log_sz = sizeof(lgr_generic_info_t) + print_len;
    writer_needle += p_gen_info->log_sz;


    gettimeofday_wrapper(&p_gen_info->tv, 0);
    p_shm->writer_needle = writer_needle;
    
    return LGR_SUCCESS;
}

/*************************************************************************************************
* Function Name  : print_log_message
* Inputs         : logLevel, filePath, line, function_name, facility, cell index, ue index, format
* Outputs        : None
* Returns        : None
* Description    : Print the log in the shared memory
*************************************************************************************************/
void
print_log_message
(
 U32 logLevel,
 const char* filePath,
 unsigned int line,
 const char* function_name,
 const char* facility,
 U8 cell_index,
 U16 ue_index,
/* SPR 21820 Start */
lgr_bool_et is_dynamic_str_var,
/* SPR 21820 End */
 const char* format,
 ...
 )
{
    lgr_sh_region_hdr_t *p_shm = LGR_NULL;
    U32 space_avl;
    U8 *p_buf;
	/* + coverity fix 54921 */
/* SPR 20636 Changes Start*/
#ifdef __x86_64__
    va_list ap = {{0}};
#else
    va_list ap = {0};
#endif
/* SPR 20636 Changes End*/
	/* - coverity fix 54921 */
    lgr_thread_info_rec_t   *p_thread_info_tbl;
    lgr_return_et ret_val;
    lgr_error_et err = LGR_NO_ERR;

    p_thread_info_tbl = lgr_get_thread_info_tbl();
    if ((LGR_NULL != p_thread_info_tbl) && (LGR_TRUE == lgr_lib_init()))
    {
        p_shm = p_thread_info_tbl[lgr_get_self_id()].p_sh_region;
    }

    if (LGR_NULL != p_shm)
    {
        space_avl = lgr_get_space_avl(p_shm);
        p_buf = p_shm->p_seg_start + p_shm->writer_needle;
        va_start(ap, format);
	/* SPR 21820 Start */
        ret_val = dump_log_message(p_shm, p_buf, space_avl, &err, logLevel, filePath,
                line, function_name, facility, cell_index, ue_index, is_dynamic_str_var, format, ap);
        /* SPR 21820 End */
        va_end(ap);

        if ((LGR_SUCCESS != ret_val) && (LGR_NO_SPACE_ERR == err))
        {
            /* Rewind */
            
            p_shm->writer_needle = LGR_ZERO;

            space_avl = lgr_get_space_avl(p_shm);
            if (LGR_ZERO != space_avl)
            {
                p_buf = p_shm->p_seg_start;
                va_start(ap, format);
		/* SPR 21820 Start */
                ret_val = dump_log_message(p_shm, p_buf, space_avl, &err, logLevel, filePath,
                        line, function_name, facility, cell_index, ue_index, is_dynamic_str_var, format, ap);
		/* SPR 21820 End */
                va_end(ap);
                if (LGR_SUCCESS != ret_val)
                {
                    p_shm->log_discard_cnt++;
                    fprintf(LGR_DEFAULT_FS, "Discarding log, discard count[%u]\n",
                            p_shm->log_discard_cnt);
                    return;
                }
            }
            else
            {
                /* No space to write log */
                p_shm->log_discard_cnt++;
                fprintf(LGR_DEFAULT_FS, "Discarding log, discard count[%u]\n",
                        p_shm->log_discard_cnt);
                return;
            }
        }
    }
    else
    {
        FILE* console_fd = LGR_STDOUT_FS;
                /*SPR 22121 Start*/
        struct tm currentTime;
        struct timeval      tv;
        if (logLevel == LGR_ERROR)
            console_fd = LGR_STDERR_FS;

        gettimeofday_wrapper(&tv, 0);
        localtime_r(&tv.tv_sec, &currentTime);
        /* Dump of console */
        fprintf(console_fd,"%02d:%02d:%02d.%06ld %-8s %-3u %-5u %s(%u): ",
                currentTime.tm_hour, currentTime.tm_min,
                currentTime.tm_sec, tv.tv_usec,
                /*SPR 22121 End*/
                lgr_loglevel_to_string(logLevel), 
                 cell_index, ue_index, function_name, line);

        va_start(ap, format);
        vfprintf(console_fd, format, ap);
        va_end(ap);

        fprintf(console_fd, "\n");
    }

    return;
}
/*SPR 21275 Changes End*/

/****************************************************************************
* Function Name  : lgr_process_log_segments
* Inputs         : None
* Outputs        : None
* Returns        : None 
* Description    : Traverses all the segments 
****************************************************************************/

void lgr_process_log_segments()
{
    lgr_thread_info_rec_t   *p_thread_info_tbl  = LGR_NULL;
    lgr_bool_et             log_available       = LGR_TRUE;
    U32                     writer_needle;
    U32                     reader_needle;
    lgr_thread_id_t         i;

    /* Get thread information table */
    p_thread_info_tbl = lgr_get_thread_info_tbl();

    if ((LGR_NULL != p_thread_info_tbl) && (LGR_TRUE == lgr_lib_init()))
    {
        /* Traverse the segments */
        while (log_available)
        {
            log_available = LGR_FALSE;
            for (i = LGR_ZERO; i < g_logger_info.num_threads; i++)
            {
                if (LGR_NULL != p_thread_info_tbl[i].p_sh_region)
                {
                    reader_needle = p_thread_info_tbl[i].p_sh_region->reader_needle;
                    writer_needle = p_thread_info_tbl[i].p_sh_region->writer_needle;

                    if (writer_needle != reader_needle)
                    {
                        /* Log is available in the segment read it */
                        lgr_dump_log_to_file(p_thread_info_tbl[i].p_sh_region, i);
                        log_available = LGR_TRUE;
                    }

                    /* Update health status */
                    if (LGR_NULL != p_thread_info_tbl[i].p_call_back)
                    {
                        (p_thread_info_tbl[i].p_call_back)(p_thread_info_tbl[i].p_cbk_arg);
                    }
                }
            }
        }
    }

    return;
}
