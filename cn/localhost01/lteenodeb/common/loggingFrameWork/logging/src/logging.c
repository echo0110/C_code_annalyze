/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2011 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: logging.c,v  2011/01/27 06.00:00 gur19170 Exp $
 ****************************************************************************
 *
 *  File Description :
 *  This file contains implementation of logging framework
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: logging.c,v $
 * Initial version
 *
 ****************************************************************************/


#include <stdio.h> 
#include <stdlib.h> 

/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/

#include <sys/time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h> 
#include <sys/timeb.h>
#include <signal.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "logging.h"
#include "lteMisc.h"
/****************************************************************************
  Private Definitions
 ****************************************************************************/
#define MAX_LOG_FILE_NAME_LEN 256
/****************************************************************************
  Private Types
 ****************************************************************************/
typedef struct LogConfig
{
    UInt32 maxShmSize;
    UInt32 maxLogFiles;
    UInt32 logLevelEnabled;
    UInt64 logCategoryEnabled;
}LogConfig;


/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/*****************************************************************************
 * Private Constants
 ****************************************************************************/
static pid_t macReaderPid_g;
static pid_t rlcReaderPid_g;
/* SPR 5178 changes starts here */
static pid_t pdcpGtpuReaderPid_g;
/*SPR 21727 Changes Start*/
#if defined RLC_PDCP_IPC_INTF 
/*SPR 21727 Changes End*/
static pid_t rlcIpcReaderPid_g;
/* SPR 5178 changes ends here */
#endif
/*SPR 8625 FIX START*/
/* + coverity 58755 Fix */
/*SPR 16855 +-*/
static pid_t macFlowpid_g;
/* - coverity 58755 Fix */
/*SPR 16855 +-*/
#ifdef DL_DATA_SEPARATION
static pid_t encoderFlowpid_g;
#endif
/*SPR 8625 FIX END*/
#ifdef RLC_STANDALONE_SETUP
static pid_t rlcSimReaderPid_g;
#endif
#ifdef LTE_EMBMS_SUPPORTED
static pid_t syncReaderPid_g;
#endif

static pid_t pdcpLogpid_g;

/*SPR 21545 Start*/
UInt8 instanceL2_g;
/*SPR 21545 End*/


/****************************************************************************
 * Exported Variables
 ****************************************************************************/
lteL2LogOption l2Input;
LogMetaData logMetaData;
UInt32 loglevelEnabled_g=0x0;
UInt32 macloglevelEnabled_g=0x0;
UInt32 pdcploglevelEnabled_g=0x0;
UInt32 rlcloglevelEnabled_g=0x0;
UInt32 syncloglevelEnabled_g=0x0;
UInt64 logCategoryEnabled_g=0x0;
UInt64 maclogCategoryEnabled_g=0x0;
UInt64 pdcplogCategoryEnabled_g=0x0;
UInt64 rlclogCategoryEnabled_g=0x0;
UInt64 synclogCategoryEnabled_g=0x0;
extern LogDefValues logCommValues[LOG_MAX_ID];
UInt32 gtpuloglevelEnabled_g=0x0;
UInt64 gtpulogCategoryEnabled_g=0x0;
/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

/*
 * Thread Specific Log Handle
 */
static log_info_t log_info[MAX_THREADS] = {{0},};

/*
 * With GNU C Thread-Local Storage, overhead to create, set and get
 * thread specifc key is avoided.
 */
#ifdef __GNUC__
    __thread log_info_t *TLS_Key;
#else
    static pthread_key_t TLS_Key = 0;
#endif

/*****************************************************************************
     * Function Name  : logger_init
     * Inputs         : ModuleId 
     * Outputs        : 
     * Returns        : 0 - On success.
     *                  1 - On failure.
     * Description    : A thread which requires it's logs to be captured should 
     *                  register by calling this function.
     *****************************************************************************/

logger_et logger_init(UInt32 thread_id, Char8* p_log_file_name,UInt32 shmSize )
{
    SInt32 shm_fd;
    void *p_shmptr = NULL;
    Char8 file_name[LTE_MAX_FILE_NAME_SIZE];
    LogShmHeader *p_file_header = NULL; 
    UInt32 maxShmSize =0 ;
	
	mysql_connect();
    /* +COVERITY 10238 */
    snprintf( file_name,LTE_MAX_FILE_NAME_SIZE, "/%s",p_log_file_name);
    /* -COVERITY 10238 */
#ifndef __GNUC__
    if (0 != pthread_key_create(&TLS_Key,NULL))
        PERRORMSG("pthread_key_create");
#endif

    /*Creating shared memory object          ----shm_open()*/
    shm_fd = shm_open_wrapper(file_name, O_CREAT | O_RDWR | O_EXCL, 0);
	if(shm_fd < 0)
    {
        if (errno == EEXIST)
        {
            shm_fd = shm_open_wrapper(file_name, O_RDWR, 0);
            if(shm_fd < 0)
            {
                PERRORMSG("In shm_open :");
                return LOGGER_FAILURE;
            }
            // Remove warning
            //is_shm_alread_exist = 1;
        }
        else
        {
            PERRORMSG("In shm_open :");
            return LOGGER_FAILURE;
        }
    }

    /*
     * In case log file size is modified from last creation
     * from adjusting mapped file size
     */
    if(shmSize != 0)
        maxShmSize = shmSize;
    else
        maxShmSize = MAX_LOG_MEMORY_SIZE;

    if(ftruncate(shm_fd,maxShmSize) == -1)
    {
        shm_unlink(file_name);
        PERRORMSG("In ftruncate()");
        return LOGGER_FAILURE;
    }

    /*Requesting the shared segment*/ 
    p_shmptr =  mmap(NULL, maxShmSize, PROT_READ|PROT_WRITE,
            MAP_SHARED, shm_fd, 0);

    if(MAP_FAILED == p_shmptr)
    {
        shm_unlink(file_name);
        PERRORMSG("In mmap():");
        /* + coverity 32177 */
        return(LOGGER_SUCCESS);
        /* - coverity 32177 */
    }

        /* Save thread specific log info */
        log_info[thread_id].file_size = maxShmSize;
        log_info[thread_id].p_shmptr = p_shmptr;
    p_file_header = (LogShmHeader *)p_shmptr;

    /* MAC_TTI_LOG Start */

    /* Added check if per TTI logs structure are not exceeding the limit */
    if(sizeof(ttiULLoggingInfo) > (sizeof(LogReaderParamValues)))
    {
        /*SPR 20636 Changes Start*/
        fprintf(stderr," size of is ttiULLoggingInfo :%zu",sizeof(ttiULLoggingInfo));
        /*SPR 20636 Changes End*/
        abort_wrapper();
    }
    else if(sizeof(ttiDciLoggingInfo) > (sizeof(LogReaderParamValues)))
    {
        /*SPR 20636 Changes Start*/
        fprintf(stderr," size of is ttiDciLoggingInfo:%zu",sizeof(ttiDciLoggingInfo));
        /*SPR 20636 Changes End*/
        abort_wrapper();
    }

    /* MAC_TTI_LOG End*/
    /* initialize trace section size */
    p_file_header->traceSectionSize = maxShmSize - LOG_FILE_HEADER_SIZE;

    /* initialize write offset */
    /*if (is_shm_alread_exist == 0)
      {*/
    /* initialize write offset to start trace section */
    p_file_header->writeOffset = LOG_FILE_HEADER_SIZE;

    /* initialize read offset to start trace section */
    p_file_header->readOffset = LOG_FILE_HEADER_SIZE;

    /* initialize rotated flag */
    p_file_header->rotatedFlag = 0;

    /* initialize header default string */
    /*strcpy(p_file_header + TRACE_SECTION_SIZE + WRITE_OFFSET_SIZE + ROTATED_FLAG_SIZE,
      FILE_HEADER_STRING);

     *(p_file_header + LOG_FILE_HEADER_SIZE-1) = '\0';*/
    /*}*/
#ifdef __GNUC__
    TLS_Key = &log_info[thread_id] ; //TLS_ptr is pointing to thread specific shm_info struct
#else
    /*Thread specific key->value storage*/
    pthread_setspecific(TLS_Key,&shmInfo[thread_id]);
#endif
    //  print_trace(0, 0, LOG_FATAL, 0x00,__FILE__,__LINE__, TRACE_START_STRING);
    return(LOGGER_SUCCESS);
} 

/*****************************************************************************
 * Function Name  : enable_log_level
 * Inputs         : 
 * Outputs        : None
 * Returns        : None
 * Description    : 
 *****************************************************************************/
void enable_log_level(UInt32 logLevel)
{
    switch(logLevel)
    {
        case LOGFATAL:
        case LOGERROR: 
        case LOGWARNING:
        case LOGINFO:
        case LOGBRIEF:
        case LOGDEBUG:
        case LOGTTIDEBUG:
        {
            loglevelEnabled_g = (logLevel*2)-1;
            macloglevelEnabled_g = (logLevel*2)-1;
            syncloglevelEnabled_g = (logLevel*2)-1;            
            break;
        }
        case INVALID_LOG:
        {
            loglevelEnabled_g = INVALID_LOG;
            macloglevelEnabled_g = INVALID_LOG;
            break;
        }
        default:
        {
            loglevelEnabled_g = (LOGFATAL*2)-1;
            macloglevelEnabled_g = (LOGFATAL*2)-1;
            break;
        }
    }
}
/*****************************************************************************
 * Function Name  : disable_log_level
 * Inputs         : 
 * Outputs        : None
 * Returns        : None
 * Description    : 
 *****************************************************************************/
/* This function would not be used by application*/
void disable_log_level(UInt32 logLevel)
{
    loglevelEnabled_g &= ~(logLevel); 
    macloglevelEnabled_g &= ~(logLevel);   
   syncloglevelEnabled_g &= ~(logLevel);   
}
/*****************************************************************************
 * Function Name  : enable_log_category
 * Inputs         : 
 * Outputs        : None
 * Returns        : None
 * Description    : 
 *****************************************************************************/
void enable_log_category(UInt64 logCategory)
{
    logCategoryEnabled_g |= logCategory; 
    maclogCategoryEnabled_g |= logCategory;
   synclogCategoryEnabled_g |=logCategory;
}
/*****************************************************************************
 * Function Name  : disable_log_category
 * Inputs         : 
 * Outputs        : None
 * Returns        : None
 * Description    : 
 *****************************************************************************/
void disable_log_category(UInt64 logCategory)
{
    logCategoryEnabled_g &= ~(logCategory); 
    maclogCategoryEnabled_g &= ~(logCategory); 
   synclogCategoryEnabled_g &= ~(logCategory); 
}

/*****************************************************************************
 * Function Name  : get_log_level
 * Inputs         : 
 * Outputs        : macloglevelEnabled_g 
 * Returns        : UInt32 
 * Description    : 
 *****************************************************************************/
UInt32 get_log_level()
{
    return macloglevelEnabled_g; 
}

/*****************************************************************************
 * Function Name  : get_log_category
 * Inputs         : 
 * Outputs        : maclogCategoryEnabled_g
 * Returns        : UInt64
 * Description    : 
 *****************************************************************************/
UInt64 get_log_category()
{
    return maclogCategoryEnabled_g; 
}

/*****************************************************************************
 * Function Name  : initlogFramework
 * Inputs         :
 * Outputs        : None
 * Returns        : None
 * Description    :
 *****************************************************************************/
void initlogFramework()
{
    /* Initialization of loglevelEnabled_g and logCategoryEnabled_g removed
     * as these are global vars and are initilaized at the time of declaration
     * itself */
    loglevelEnabled_g       |= (LOGFATAL | LOGERROR);
    logCategoryEnabled_g    |= ALL_LOG_CATEGORY_SET;
    gtpuloglevelEnabled_g   |= (LOGFATAL | LOGERROR);
    gtpulogCategoryEnabled_g|= ALL_LOG_CATEGORY_SET;
    macloglevelEnabled_g    |= (LOGFATAL | LOGERROR);
    rlcloglevelEnabled_g    |= (LOGFATAL | LOGERROR);
    maclogCategoryEnabled_g |= ALL_LOG_CATEGORY_SET;
    rlclogCategoryEnabled_g |= ALL_LOG_CATEGORY_SET;
    pdcploglevelEnabled_g   |= (LOGFATAL | LOGERROR |LOGINFO);
    pdcplogCategoryEnabled_g|= ALL_LOG_CATEGORY_SET;
    synclogCategoryEnabled_g |= ALL_LOG_CATEGORY_SET;
    syncloglevelEnabled_g   |= (LOGFATAL | LOGERROR);
}

/*****************************************************************************
 * Function Name  : initlogFramework
 * Inputs         :
 * Outputs        : None
 * Returns        : None
 * Description    :
 *****************************************************************************/
void startLogReader(lteL2LogOption *l2Input_p)
{
    char inputMacFileName[NAME_MAX] = "--maclogs=" ;
    char inputRlcFileName[NAME_MAX] = "--maclogs=" ;
    /* SPR 5178 changes starts here */
    char inputPdcpGtpuFileName[NAME_MAX] = "--maclogs=" ;
#if defined RLC_PDCP_IPC_INTF 
    char inputRlcIpcFileName[NAME_MAX] = "--maclogs=" ;
    /* SPR 5178 changes ends here */
#endif
#ifdef RLC_STANDALONE_SETUP
    char inputRlcSimFileName[NAME_MAX] = "--maclogs=" ;
#endif
#ifdef LTE_EMBMS_SUPPORTED   
   char inputSyncFileName[NAME_MAX] = "--maclogs=" ;
#endif   
    char inputLoggerOption[NAME_MAX] = "--logger_option=" ;
    char inputLogFileSize[NAME_MAX] = "--log_file_size=" ;
    char inputNumOfFiles[NAME_MAX] = "--log_num_files=" ;
    char shmName[NAME_MAX] = "--shm_name=" ;
    char binName[NAME_MAX] = "--binName=";
    char shmSize[NAME_MAX] = "--shm_size=";
    char size[MAX_LOGGER_TYPE_STR_LEN] ={0};
    /* CA Logging Changes Start */
    /*SPR 16855 +-*/
    /*SPR21717 changes start*/
    if(l2Input.l2gtpuflag)
    {
        strcat(binName,"lte_gtpu");
    }
    else
    {
        strcat(binName,"lteLayer2");
    }
    /*SPR21717 changes End*/
#ifndef DL_UL_SPLIT
    /* TDD_Warning_Fix */
    /*SPR 8625 FIX START*/
    macFlowpid_g =0;
    /*SPR 8625 FIX END*/
    /* TDD_Warning_Fix */
    char inputMacFlowFileName[NAME_MAX] = "--maclogs=" ;
    /*Coverity 10239 Fix Start*/
    /* + Coverity 32084 */
    strnCat(inputMacFlowFileName,LTE_L2_LOG_REGION_FOUR_NAME,NAME_MAX-strnlen(inputMacFlowFileName,NAME_MAX-1)-1);
    /* - Coverity 32084 */
    inputMacFlowFileName[NAME_MAX-1]='\0';
    /*Coverity 10239 Fix End*/
#endif
    /* CA Logging Changes End */

    snprintf(size,MAX_LOGGER_TYPE_STR_LEN,"%d",l2Input_p->shmSize/(1024*1024));
    size[MAX_LOGGER_TYPE_STR_LEN - 1] = '\0';
    /*+COVERITY 5.3.0 - 32084*/
    strnCat(shmSize,size,NAME_MAX-strnlen(size,MAX_LOGGER_TYPE_STR_LEN)-1);
    shmSize[NAME_MAX-1]= '\0';
    /*Klocwork Fix*/
    strnCat(inputMacFileName,l2Input_p->mac_file_name,NAME_MAX-(strnlen(inputMacFileName,NAME_MAX-1)-1));
    /*Klocwork Fix*/
    inputMacFileName[NAME_MAX-1]= '\0';
    /*Klocwork Fix*/
    strnCat(inputRlcFileName,l2Input_p->rlc_file_name,NAME_MAX-strnlen(inputRlcFileName,NAME_MAX-1)-1);
    /*Klocwork Fix*/
    inputRlcFileName[NAME_MAX-1]= '\0';
#ifdef LTE_EMBMS_SUPPORTED
   strncat(inputSyncFileName,l2Input_p->sync_file_name,NAME_MAX-strnlen(inputSyncFileName,NAME_MAX-1)-1);
   inputSyncFileName[NAME_MAX-1]= '\0';
#endif   
    /* SPR 5178 changes starts here */
    strCat(inputPdcpGtpuFileName,l2Input_p->gtpu_file_name);
    /*SPR21717 changes End*/
#ifdef RLC_STANDALONE_SETUP
    strCat(inputRlcSimFileName,l2Input_p->simulator_file_name);
#endif
    strnCat(inputLoggerOption,l2Input_p->logger_option,NAME_MAX-strnlen(l2Input_p->logger_option,MAX_LOGGER_TYPE_STR_LEN)-1);
    inputLoggerOption[NAME_MAX-1]= '\0'; 

    strnCat(inputLogFileSize,l2Input_p->logFileSize,NAME_MAX-strnlen(l2Input_p->logFileSize,MAX_LOGGER_TYPE_STR_LEN)-1);
    inputLogFileSize[NAME_MAX-1]= '\0';

    strnCat(inputNumOfFiles,l2Input_p->numFile,NAME_MAX-strnlen(l2Input_p->numFile,MAX_LOGGER_TYPE_STR_LEN)-1);
    /*-COVERITY 5.3.0 - 32084*/
    inputNumOfFiles[NAME_MAX-1]= '\0';

#ifdef RLC_STANDALONE_SETUP
    if(!l2Input.l2gtpuflag)
    {
        rlcSimReaderPid_g = fork();
        if(0 == rlcSimReaderPid_g)
        {
            strcat(shmName,"RLC_SIMULATOR_REGION");
            /*SPR21717 changes start*/
            if (-1 == execl("ltebinread", "ltebinread", shmName, shmSize, inputRlcSimFileName, inputLoggerOption, inputLogFileSize, inputNumOfFiles,binName ,NULL))
                /*SPR21717 changes end*/
            {
                fprintf(stderr, "reader spawn failed [RLC_SIMULATOR_REGION], log won't be stored\n");
                exit(1);
            }
            /******SPR 2276********/     
        }
    }
    else
    {
#endif
        /*SPR21717 changes start*/
        if(l2Input.l2gtpuflag)
        {
            pdcpGtpuReaderPid_g = fork();
            if(0 == pdcpGtpuReaderPid_g)
            {
                strcat(shmName,LTE_L2_LOG_REGION_FIVE_NAME);
                if (-1 == execl("ltebinread", "ltebinread", shmName, shmSize, inputPdcpGtpuFileName, inputLoggerOption, inputLogFileSize, inputNumOfFiles, binName,NULL))
                {
                    fprintf(stderr, "reader spawn failed [PDCP_GTPU_REGION], log won't be stored\n");
                    exit(1);
                }
                fprintf(stderr,"\n\n shmName -- GTPU_REGION Set \n");
                /******SPR 2276********/     
            }
        }
#if defined RLC_PDCP_IPC_INTF
        if(!l2Input.l2gtpuflag)
        {
            rlcIpcReaderPid_g = fork();
            if(0 == rlcIpcReaderPid_g)
            {
                strcat(shmName,"RLC_IPC_REGION");
                if (-1 == execl("ltebinread", "ltebinread", shmName, shmSize, inputRlcIpcFileName, inputLoggerOption, inputLogFileSize, inputNumOfFiles,binName, NULL))
                {
                    fprintf(stderr, "reader spawn failed [RLC_IPC_REGION], log won't be stored\n");
                    exit(1);
                }
                fprintf(stderr,"\n\n shmName -- RLC_IPC_REGION Set \n");
                /******SPR 2276********/     
            }
        }
#endif
        /* +SPR 10703 Changes */
        /*SPR 16855 +-*/
#ifdef DL_UL_SPLIT
        if(!l2Input.l2gtpuflag)
        {
            macReaderPid_g = fork();

            if (0 == macReaderPid_g)
            {

                /*SPR 21653 changes start*/
                /*code removed*/
                /*SPR 21653 changes end*/

                /* + Coverity 32084 */
                if ( strlen("MAC_RLC_REGION") > 0 )
                {
                    strnCat(shmName,"MAC_RLC_REGION",NAME_MAX-strnlen(shmName,NAME_MAX-1)-1);
                }
                else
                {
                    strnCat(shmName,"MAC_RLC_REGION",NAME_MAX-1);
                }
                /* - Coverity 32084 */
                shmName[NAME_MAX-1]= '\0';

                fprintf(stderr, "shmName[%s]\n",shmName);
                /******SPR 2276********/     
                if (-1 == execl("ltebinread", "ltebinread", shmName, shmSize, inputMacFileName, inputLoggerOption, inputLogFileSize, inputNumOfFiles,binName,  NULL))
                {
                    /* if execlreturned -> failure */
                    fprintf(stderr, "reader spawn failed [MAC_RLC_REGION], log won't be stored\n");
                    exit(1);
                }
                /******SPR 2276********/     
            }
        }
        else
#endif
        {
            /* -SPR 10703 Changes */
            if(!l2Input.l2gtpuflag)
            {
                rlcReaderPid_g = fork();

                if (0 == rlcReaderPid_g)
                {
                    /*SPR 21653 changes start*/
                    /*code removed*/
                    /*SPR 21653 changes end*/

                    /* + Coverity 32084 */
                    strnCat(shmName,"RLC_PDCP_REGION",NAME_MAX-strnlen(shmName, NAME_MAX-1)-1);
                    /* - Coverity 32084 */
                    shmName[NAME_MAX-1]= '\0';

                    /******SPR 2276********/     
                    if (-1 == execl("ltebinread", "ltebinread", shmName, shmSize, inputRlcFileName, inputLoggerOption, inputLogFileSize, binName, inputNumOfFiles, NULL))
                    {
                        /* if execlreturned -> failure */
                        fprintf(stderr, "reader spawn failed [RLC_PDCP_REGION], log won't be stored\n");
                        exit(1);
                    }
                    /******SPR 2276********/     
                }
                /******SPR 2276********/     
            }
        }
#ifdef RLC_STANDALONE_SETUP
    }
#endif
    /* CA Logging Changes Start */
    /*SPR 16855 +-*/
#ifndef DL_UL_SPLIT
    /*SPR 8625 FIX START*/
    if(!l2Input.l2gtpuflag)
    {
        macFlowpid_g = fork();
        if(macFlowpid_g == 0)
            /*SPR 8625 FIX END*/
        {
            /*SPR 21653 changes start*/
            /*code removed*/
            /*SPR 21653 changes end*/

            /* + Coverity 32084 */
            strnCat(shmName,"MAC_LP_THD_REGION",NAME_MAX-strnlen(shmName,NAME_MAX-1)-1);
            /* - Coverity 32084 */
            shmName[NAME_MAX-1]= '\0';

            /* CA Logging Changes End */
            if (-1 == execl("ltebinread", "ltebinread", shmName, shmSize, inputMacFlowFileName, inputLoggerOption, inputLogFileSize, inputNumOfFiles,binName, NULL))
            {
                fprintf(stderr, "reader spawn failed [%s], log won't be stored\n", shmName);
                exit(1);
            }
        }
    }
#endif
    /*SPR21717 changes end*/
#ifdef LTE_EMBMS_SUPPORTED
     syncReaderPid_g = fork();
     if(syncReaderPid_g == 0)
     {
         strncat(shmName,LTE_L2_SYNC_REGION_NAME,NAME_MAX-strnlen(shmName,NAME_MAX-1)-1);
         shmName[NAME_MAX-1]= '\0';
         if (-1 == execl("ltebinread", "ltebinread", shmName, shmSize, inputSyncFileName, inputLoggerOption, inputLogFileSize, inputNumOfFiles,binName,NULL))
         {
             fprintf(stderr, "reader spawn failed [%s], log won't be stored\n", shmName);
             exit(1);
         }
     }
#endif
}

void startPdcpLogReader(
        lteL2LogOption     *l2Input_p, 
        Char8*             p_log_file_name)
{
    Char8 inputPdcpFileName[NAME_MAX] = "--maclogs=" ;
    Char8 shmSize[NAME_MAX] = "--shm_size=";
    Char8 size[MAX_LOGGER_TYPE_STR_LEN] ={0};
    Char8 shmName[NAME_MAX] = "--shm_name=" ;
    Char8 inputLoggerOption[NAME_MAX] = "--logger_option=" ;
    Char8 inputLogFileSize[NAME_MAX] = "--log_file_size=" ;
    Char8 inputNumOfFiles[NAME_MAX] = "--log_num_files=" ;
    /*SPR21717 changes start*/
    char binName[NAME_MAX] = "--binName=";
    if(l2Input.l2gtpuflag)
    {
        strcat(binName,"lte_gtpu");
    }
    else
    {
        strcat(binName,"lteLayer2");
    }
    /*SPR21717 changes End*/

    pdcpLogpid_g =0;
    /* + Coverity 32084 */
    strnCat(inputPdcpFileName,p_log_file_name,NAME_MAX-strnlen(inputPdcpFileName,NAME_MAX-1)-1);
    /* - Coverity 32084 */
    inputPdcpFileName[NAME_MAX-1]='\0';

    snprintf(size,MAX_LOGGER_TYPE_STR_LEN,"%d",l2Input_p->shmSize/(1024*1024));
    size[MAX_LOGGER_TYPE_STR_LEN - 1] = '\0';

    strnCat(shmSize,size,NAME_MAX-strnlen(size,MAX_LOGGER_TYPE_STR_LEN)-1);
    shmSize[NAME_MAX-1]= '\0';

    strnCat(inputLoggerOption,l2Input_p->logger_option,NAME_MAX-strnlen(l2Input_p->logger_option,MAX_LOGGER_TYPE_STR_LEN)-1);
    inputLoggerOption[NAME_MAX-1]= '\0'; 

    strnCat(inputLogFileSize,l2Input_p->logFileSize,NAME_MAX-strnlen(l2Input_p->logFileSize,MAX_LOGGER_TYPE_STR_LEN)-1);
    inputLogFileSize[NAME_MAX-1]= '\0';

    strnCat(inputNumOfFiles,l2Input_p->numFile,NAME_MAX-strnlen(l2Input_p->numFile,MAX_LOGGER_TYPE_STR_LEN)-1);
    inputNumOfFiles[NAME_MAX-1]= '\0';

    pdcpLogpid_g = fork();
    if(pdcpLogpid_g == 0)
    {
        sprintf(shmName,"%s%s",shmName, p_log_file_name);
        if (-1 == execl("ltebinread", "ltebinread", shmName, shmSize, inputPdcpFileName, 
                    inputLoggerOption, inputLogFileSize, binName, inputNumOfFiles, NULL))
        {
            fprintf(stderr, "reader spawn failed [%s], log won't be stored\n", shmName);
            exit(1);
        }
    }

    return ;
}

/* + SPR 17439 */
void stopLogReader(void)
/* - SPR 17439 */    
{
    sleep(1);
    /*****SPR 2276******/
    if (macReaderPid_g > 0)
        kill(macReaderPid_g, SIGINT);
    if (rlcReaderPid_g > 0)
        kill(rlcReaderPid_g, SIGINT);
/*SPR 8625 FIX START*/
/* + coverity 58755 Fix */
/*SPR 16855 +-*/
/* - coverity 58755 Fix */
    if (macFlowpid_g > 0)
        kill(macFlowpid_g, SIGINT);
#ifdef DL_DATA_SEPARATION
    if (encoderFlowpid_g > 0)
        kill(encoderFlowpid_g, SIGINT);
#endif
/*SPR 8625 FIX END*/
    /******SPR 2276********/    
#ifdef RLC_STANDALONE_SETUP
    if (rlcSimReaderPid_g > 0)
        kill(rlcSimReaderPid_g, SIGINT);
#endif
    /* SPR 5178 changes starts here */
    if (pdcpGtpuReaderPid_g > 0)
        kill(pdcpGtpuReaderPid_g, SIGINT);
    /*SPR 21727 Changes Start*/
#if  defined RLC_PDCP_IPC_INTF
    /*SPR 21727 Changes End*/
    if (rlcIpcReaderPid_g > 0)
        kill(rlcIpcReaderPid_g, SIGINT);
    /* SPR 5178 changes ends here */
#endif
}

#ifdef PERSIST_FATAL_LOG
static void write_critical_log(LogParamValues *pParamValues)
{
    Char8 filename[MAX_LOG_FILE_NAME_LEN]="";
    FILE *fp;

    snprintf(filename, MAX_LOG_FILE_NAME_LEN, "logcritical.%d", getpid_wrapper());

    fp = fopen(filename, "a+");//filename  file  path name
    if (NULL == fp)
    {
        return;
    }
    interpret_log(pParamValues, fp);
    fClose(fp);
}
#endif
#ifdef SHM_PRINT_ENABLED

/* MAC_TTI_LOG Start */

/*****************************************************************************
 * Function Name  : prepare_tti_logs
 * Inputs         : log id
 * Outputs        : 
 * Returns        :  Pointer based on logid
 * Description    : A variadic function called by any thread for logging.  
 *****************************************************************************/
void * prepare_tti_logs(void) 
{
    log_info_t *p_shm_seg = NULL;
    LogParamValues *outputPtr_p = NULL;

    UInt32 write_offset = 0;
    UInt32 size = 0;
    LogShmHeader *pLogHdr = NULL;


#ifdef __GNUC__
    p_shm_seg = TLS_Key;
#else
    p_shm_seg = pthread_getspecific(TLS_Key);
#endif

        pLogHdr = (LogShmHeader *) p_shm_seg->p_shmptr;

        if (pLogHdr->writeOffset + sizeof(LogParamValues) == pLogHdr->readOffset)
        {
            pLogHdr->lostLogCount++;
            return NULL;
        }
        else if (pLogHdr->lostLogCount != 0)
        {
            /* write lost log count */
            outputPtr_p = (LogParamValues *)(p_shm_seg->p_shmptr + pLogHdr->writeOffset);
            outputPtr_p->logId= L2_LOG_LOST;
            outputPtr_p->timestampSec = (UInt32)logMetaData.timestamp.tv_sec;
            outputPtr_p->timestampUsec = (UInt32)logMetaData.timestamp.tv_usec;
            outputPtr_p->globalTick = logMetaData.currentTick;
            outputPtr_p->intValues[0] = pLogHdr->lostLogCount;
            strncpy(outputPtr_p->params.strValues[0], __func__, MAX_LOG_STR_LEN);
            /* Coverity 25387 Fix Start */
            outputPtr_p->params.strValues[0][MAX_LOG_STR_LEN - 1] = '\0';
            /* Coverity 25387 Fix End */

            if((l2Input.shmSize - (pLogHdr->writeOffset + sizeof(LogParamValues))) < MAX_LOG_LENGTH)
            {
                pLogHdr->writeOffset = LOG_FILE_HEADER_SIZE;
                pLogHdr->rotatedFlag = 1;
            }
            else
            {
                pLogHdr->writeOffset = pLogHdr->writeOffset + sizeof(LogParamValues);
            }

            /* set lost log count to 0 */
            pLogHdr->lostLogCount = 0;

            /* again check if we can write the log */
            if (pLogHdr->writeOffset + sizeof(LogParamValues) == pLogHdr->readOffset)
            {
                pLogHdr->lostLogCount++;
                return NULL;
            }
        }
        write_offset = pLogHdr->writeOffset;

        outputPtr_p = (LogParamValues *)(p_shm_seg->p_shmptr + write_offset);

        size += sizeof(LogParamValues);

        if((l2Input.shmSize - (write_offset + size)) < MAX_LOG_LENGTH)
        {
            pLogHdr->writeOffset = LOG_FILE_HEADER_SIZE;
            pLogHdr->rotatedFlag = 1;
        }
        else
        {
            pLogHdr->writeOffset = write_offset + size;
        }
    return outputPtr_p;
}

#endif

/* MAC_TTI_LOG End */

/* +- SPR 17777 */
void * prepare_FAPI_logs(UInt32 logId, SInt32 msgSize, const char* funcName) 
{
        log_info_t *p_shm_seg = NULL;
    Log_FAPI_st *outputPtr_p = NULL;

    UInt32 write_offset = 0;
    UInt32 size = 0;
    LogShmHeader *pLogHdr = NULL;


#ifdef __GNUC__
    p_shm_seg = TLS_Key;
#else
    p_shm_seg = pthread_getspecific(TLS_Key);
#endif
        pLogHdr = (LogShmHeader *) p_shm_seg->p_shmptr;

        if (pLogHdr->writeOffset + sizeof(Log_FAPI_st) == pLogHdr->readOffset)
        {
            pLogHdr->lostLogCount++;
            return NULL;
        }
        else if (pLogHdr->lostLogCount != 0)
        {
            /* write lost log count */
            LogParamValues *pParamValues = NULL;
            pParamValues = (LogParamValues *)(p_shm_seg->p_shmptr + pLogHdr->writeOffset + size);
            pParamValues->logId = L2_LOG_LOST;
            pParamValues->timestampSec = (UInt32)logMetaData.timestamp.tv_sec;
            pParamValues->timestampUsec = (UInt32)logMetaData.timestamp.tv_usec;
            pParamValues->globalTick = logMetaData.currentTick;
            pParamValues->intValues[0] = pLogHdr->lostLogCount;
            strncpy(pParamValues->params.strValues[0], __func__, MAX_LOG_STR_LEN);
            if((l2Input.shmSize - (pLogHdr->writeOffset + sizeof(LogParamValues))) < MAX_LOG_LENGTH)
            {
                pLogHdr->writeOffset = LOG_FILE_HEADER_SIZE;
                pLogHdr->rotatedFlag = 1;
            }
            else
            {
                pLogHdr->writeOffset = pLogHdr->writeOffset + sizeof(LogParamValues);;
            }

            /* set lost log count to 0 */
            pLogHdr->lostLogCount = 0;

            /* again check if we can write the log */
            if (pLogHdr->writeOffset + sizeof(Log_FAPI_st) == pLogHdr->readOffset)
            {
                pLogHdr->lostLogCount++;
                return NULL;
            }
        }

        write_offset = pLogHdr->writeOffset;

        outputPtr_p = (Log_FAPI_st*)(p_shm_seg->p_shmptr + write_offset);
        outputPtr_p->logId=logId;
        outputPtr_p->timestampSec = (UInt32)logMetaData.timestamp.tv_sec;
        outputPtr_p->timestampUsec = (UInt32)logMetaData.timestamp.tv_usec;
        outputPtr_p->globalTick = logMetaData.currentTick;
        outputPtr_p->intValue = msgSize;
        strncpy(outputPtr_p->strFuncName, funcName, MAX_LOG_STR_LEN);
        outputPtr_p->strFuncName[MAX_LOG_STR_LEN - 1] = '\0';

        size += sizeof(Log_FAPI_st);

        if((SInt32)(l2Input.shmSize - (write_offset + size)) < MAX_FAPI_LOG_LENGTH)
        {
            pLogHdr->writeOffset = LOG_FILE_HEADER_SIZE;
            pLogHdr->rotatedFlag = 1;
        }
        else
        {
            pLogHdr->writeOffset = write_offset + size;
        }

    return outputPtr_p;
}

/****************NEW LOGGING FRAMEWORK CHANGES START******************************/
/*****************************************************************************
 * Function Name  : print_log
 * Inputs         : stack id, log area, log level, trace id, file name, line num
 *                  logString to be logged in SHM 
 * Outputs        : None 
 * Returns        : None
 * Description    : A variadic function called by any thread for logging.  
 *****************************************************************************/

            void print_log(SInt8* fileName,SInt8* funcName,UInt32 lineNo,\
                    UInt8 logLevel,UInt8 logModule,UInt64 logCategory,SInt8* str,...)
{
    UInt32 size = 0;
    log_info_t *p_shm_seg = NULL;

    UInt32 write_offset;
    LogParamValues *pParamValues;
    LogShmHeader *pLogHdr;
    UInt32 index;
    va_list va;


#ifdef __GNUC__
    p_shm_seg = TLS_Key;
#else
    p_shm_seg = pthread_getspecific(TLS_Key);
#endif
        pLogHdr = (LogShmHeader *) p_shm_seg->p_shmptr;

        if (pLogHdr->writeOffset + sizeof(LogParamValues) == pLogHdr->readOffset)
        {
            pLogHdr->lostLogCount++;
            return;
        }
        else if (pLogHdr->lostLogCount != 0)
        {
            /* write lost log count */
            pParamValues = (LogParamValues *)(p_shm_seg->p_shmptr + pLogHdr->writeOffset + size);
            pParamValues->logId = L2_LOG_LOST;
            pParamValues->timestampSec = (UInt32)logMetaData.timestamp.tv_sec;
            pParamValues->timestampUsec = (UInt32)logMetaData.timestamp.tv_usec;
            pParamValues->globalTick = logMetaData.currentTick;
            pParamValues->intValues[0] = pLogHdr->lostLogCount;
            strncpy(pParamValues->params.strValues[0], __func__, MAX_LOG_STR_LEN);
            if((l2Input.shmSize - (pLogHdr->writeOffset + sizeof(LogParamValues))) < MAX_LOG_LENGTH)
            {
                pLogHdr->writeOffset = LOG_FILE_HEADER_SIZE;
                pLogHdr->rotatedFlag = 1;
            }
            else
            {
                pLogHdr->writeOffset = pLogHdr->writeOffset + sizeof(LogParamValues);;
            }

            /* set lost log count to 0 */
            pLogHdr->lostLogCount = 0;

            /* again check if we can write the log */
            if (pLogHdr->writeOffset + sizeof(LogParamValues) == pLogHdr->readOffset)
            {
                pLogHdr->lostLogCount++;
                return;
            }
        }
        /*Initialize ptr to Optional arguments list in variadic function*/
        write_offset = pLogHdr->writeOffset;

        pParamValues = (LogParamValues *)(p_shm_seg->p_shmptr + write_offset + size);
        pParamValues->logId =NEW_BIN_LOG;
        /*pParamValues->logLevel = logLevel;
          pParamValues->logCategory = logCategory;*/
        pParamValues->timestampSec = (UInt32)logMetaData.timestamp.tv_sec;
        pParamValues->timestampUsec = (UInt32)logMetaData.timestamp.tv_usec;
        pParamValues->globalTick = logMetaData.currentTick;
        /*pParamValues->flValues[0]  = floatValOne;
          pParamValues->flValues[1] = floatValTwo;*/
        va_start(va,str);
        for(index =0 ;index <5; index++)
        {    
            pParamValues->intValues[index]=(UInt32)va_arg(va,UInt32);
        }    
        va_end(va);

        /* SPR 20430 Changes Start*/
    pParamValues->params.logParams.str=(ADDR)str;
    /* SPR21717 Changes start */
	pParamValues->params.logParams.fileName=(ADDR)fileName;
    /* SPR21717 Changes end */
	pParamValues->params.logParams.funcName=(ADDR)funcName;
	pParamValues->params.logParams.lineNo=(ADDR)lineNo;

//Code Removed
    /* SPR 20430 Changes End*/

        pParamValues->params.logParams.logModule=(UInt8)logModule;
        pParamValues->params.logParams.logLevel=(UInt8)logLevel;
        pParamValues->params.logParams.logCategory=(UInt64)logCategory;

        size += sizeof(LogParamValues);
        if((l2Input.shmSize - (write_offset + size)) < MAX_LOG_LENGTH)
        {
            pLogHdr->writeOffset = LOG_FILE_HEADER_SIZE;
            pLogHdr->rotatedFlag = 1;
        }
        else
        {
            pLogHdr->writeOffset = write_offset + size;
        }

#ifdef PERSIST_FATAL_LOG
        if (LOGFATAL == logCommValues[logId].logLevel)
        {
            write_critical_log(pParamValues);
        }
#endif

}

/*****************************************************************************
 * Function Name  : console_print_log
 * Inputs         : stack id, log area, log level, trace id, file name, line num
 *                  logString to be logged in SHM 
 * Outputs        : None 
 * Returns        : None
 * Description    : A variadic function called by any thread for logging.  
 *****************************************************************************/
void console_print_log(SInt8* fileName,SInt8* funcName,UInt32 lineNo,\
                        UInt8 logLevel,UInt8 logModule,UInt16 logCategory,SInt8* str,...)
{
    Char8 *logLevelStr;
    Char8 *logModuleStr;
    struct tm timestamp;
    va_list va;

    if(NULL == localtime_r(((time_t*)&logMetaData.timestamp.tv_sec), &timestamp))
    {
        fprintf(stderr, "timestamp decoding failed\n");
    }

    logLevelStr= logLevelStrValues[logLevel];
    logModuleStr= logModuleStrValues[logModule];


    fprintf (stderr, "[%04d-%02d-%02d/%02d:%02d:%02d.%03d]",
            timestamp.tm_year+1900, (timestamp.tm_mon + 1), timestamp.tm_mday,
            timestamp.tm_hour, timestamp.tm_min, timestamp.tm_sec,
            ((UInt32)logMetaData.timestamp.tv_usec)/1000);
    fprintf (stderr, "[% 4d-%02d][%5s][%4s][%u]", ((UInt32)((logMetaData.currentTick/10)%1024)),
            ((UInt32)(logMetaData.currentTick%10)),logLevelStr,logModuleStr,logCategory);

    /***FILE NAME****/
    fprintf(stderr,"[%s]",fileName);

    /***FUNC NAME****/
    fprintf(stderr,"[%s] ",funcName);

    /***LINE NO****/
    fprintf(stderr,"[%u]\n",lineNo);

    va_start(va,str);
    vfprintf(stderr,(const char*)str,va);
    va_end(va);
}
/****************NEW LOGGING FRAMEWORK CHANGES END******************************/

short test_log(unsigned short usModId,unsigned short logLevel,char*fmt,...);
/*****************************************************************************
 * Function Name  : print_trace
 * Inputs         : stack id, log area, log level, trace id, file name, line num
 *                  logString to be logged in SHM 
 * Outputs        : None 
 * Returns        : None
 * Description    : A variadic function called by any thread for logging.  
 *****************************************************************************/

/* SPR 15909 fix start */
void print_trace(UInt32 logId,UInt64 globalTick,\
                UInt32 intValOne,UInt32 intValTwo,\
                UInt32 intValThree,UInt32 intValFour,UInt32 intValFive,\
                UDouble32 floatValOne,UDouble32 floatValTwo ,\
                const Char8 *strValOne,const Char8 *strValTwo)
        /* SPR 15909 fix end */
{
#ifdef LINUX_PC_TEST
	test_log(10,2,"print_trace %s -- %s",strValOne,strValTwo);
	return ;
#endif

    LTE_GCC_UNUSED_PARAM(globalTick);    
    LTE_GCC_UNUSED_PARAM(floatValOne);
    LTE_GCC_UNUSED_PARAM(floatValTwo);
    UInt32 size = 0;
    log_info_t *p_shm_seg = NULL;

    UInt32 write_offset;
    LogParamValues *pParamValues;
    LogShmHeader *pLogHdr;

#ifdef __GNUC__
    p_shm_seg = TLS_Key;
#else
    p_shm_seg = pthread_getspecific(TLS_Key);
#endif
        pLogHdr = (LogShmHeader *) p_shm_seg->p_shmptr;

        if (pLogHdr->writeOffset + sizeof(LogParamValues) == pLogHdr->readOffset)
        {
            pLogHdr->lostLogCount++;
            return;
        }
        else if (pLogHdr->lostLogCount != 0)
        {
            /* write lost log count */
            pParamValues = (LogParamValues *)(p_shm_seg->p_shmptr + pLogHdr->writeOffset + size);
            pParamValues->logId = L2_LOG_LOST;
            pParamValues->timestampSec = (UInt32)logMetaData.timestamp.tv_sec;
            pParamValues->timestampUsec = (UInt32)logMetaData.timestamp.tv_usec;
            pParamValues->globalTick = logMetaData.currentTick;
            pParamValues->intValues[0] = pLogHdr->lostLogCount;
            strncpy(pParamValues->params.strValues[0], __func__, MAX_LOG_STR_LEN);
            if((l2Input.shmSize - (pLogHdr->writeOffset + sizeof(LogParamValues))) < MAX_LOG_LENGTH)
            {
                pLogHdr->writeOffset = LOG_FILE_HEADER_SIZE;
                pLogHdr->rotatedFlag = 1;
            }
            else
            {
                pLogHdr->writeOffset = pLogHdr->writeOffset + sizeof(LogParamValues);;
            }

            /* set lost log count to 0 */
            pLogHdr->lostLogCount = 0;

            /* again check if we can write the log */
            if (pLogHdr->writeOffset + sizeof(LogParamValues) == pLogHdr->readOffset)
            {
                pLogHdr->lostLogCount++;
                return;
            }
        }
        /*Initialize ptr to Optional arguments list in variadic function*/
        write_offset = pLogHdr->writeOffset;

        pParamValues = (LogParamValues *)(p_shm_seg->p_shmptr + write_offset + size);
        pParamValues->logId = logId;
		strNCpy(&pParamValues->tm, &logMetaData.tm, sizeof(logMetaData.tm));
        /*pParamValues->logLevel = logLevel;
          pParamValues->logCategory = logCategory;*/
        pParamValues->timestampSec = (UInt32)logMetaData.timestamp.tv_sec;
        pParamValues->timestampUsec = (UInt32)logMetaData.timestamp.tv_usec;
        pParamValues->globalTick = logMetaData.currentTick;
        pParamValues->intValues[0] = intValOne;
        pParamValues->intValues[1] = intValTwo;
        pParamValues->intValues[2] = intValThree;
        pParamValues->intValues[3] = intValFour;
        pParamValues->intValues[4] = intValFive;
        /*pParamValues->flValues[0]  = floatValOne;
          pParamValues->flValues[1] = floatValTwo;*/

        size += sizeof(LogParamValues);

        strncpy(pParamValues->params.strValues[0], strValOne, MAX_LOG_STR_LEN);
        pParamValues->params.strValues[0][MAX_LOG_STR_LEN - 1] = '\0';

        strncpy(pParamValues->params.strValues[1], strValTwo, MAX_LOG_STR_LEN);
        pParamValues->params.strValues[1][MAX_LOG_STR_LEN - 1] = '\0';

        if((l2Input.shmSize - (write_offset + size)) < MAX_LOG_LENGTH)
        {
            pLogHdr->writeOffset = LOG_FILE_HEADER_SIZE;
            pLogHdr->rotatedFlag = 1;
        }
        else
        {
            pLogHdr->writeOffset = write_offset + size;
        }

#ifdef PERSIST_FATAL_LOG
        if (LOGFATAL == logCommValues[logId].logLevel)
        {
            write_critical_log(pParamValues);
        }
#endif
}

void print_format(const Char8 *str, ...)
{

	UInt32 size = 0, logId = LOG_MAC_FORMAT;
	log_info_t *p_shm_seg = NULL;

	UInt32 write_offset;
	LogParamValues *pParamValues;
	LogShmHeader *pLogHdr;
	UInt32 index;
    va_list va;

#ifdef __GNUC__
	p_shm_seg = TLS_Key;
#else
	p_shm_seg = pthread_getspecific(TLS_Key);
#endif
	pLogHdr = (LogShmHeader *) p_shm_seg->p_shmptr;

	if (pLogHdr->writeOffset + sizeof(LogParamValues) == pLogHdr->readOffset) {
		pLogHdr->lostLogCount++;
		return;
	} else if (pLogHdr->lostLogCount != 0) {
		/* write lost log count */
		pParamValues = (LogParamValues *)(p_shm_seg->p_shmptr + pLogHdr->writeOffset + size);
		pParamValues->logId = L2_LOG_LOST;
		pParamValues->timestampSec = (UInt32)logMetaData.timestamp.tv_sec;
		pParamValues->timestampUsec = (UInt32)logMetaData.timestamp.tv_usec;
		pParamValues->globalTick = logMetaData.currentTick;
		pParamValues->intValues[0] = pLogHdr->lostLogCount;
		strncpy(pParamValues->params.strValues[0], __func__, MAX_LOG_STR_LEN);
		if((l2Input.shmSize - (pLogHdr->writeOffset + sizeof(LogParamValues))) < MAX_LOG_LENGTH) {
			pLogHdr->writeOffset = LOG_FILE_HEADER_SIZE;
			pLogHdr->rotatedFlag = 1;
		} else {
			pLogHdr->writeOffset = pLogHdr->writeOffset + sizeof(LogParamValues);;
		}

		/* set lost log count to 0 */
		pLogHdr->lostLogCount = 0;

		/* again check if we can write the log */
		if (pLogHdr->writeOffset + sizeof(LogParamValues) == pLogHdr->readOffset) {
			pLogHdr->lostLogCount++;
			return;
		}
	}
	/*Initialize ptr to Optional arguments list in variadic function*/
	write_offset = pLogHdr->writeOffset;

	pParamValues = (LogParamValues *)(p_shm_seg->p_shmptr + write_offset + size);
	pParamValues->logId = logId;
	strNCpy(&pParamValues->tm, &logMetaData.tm, sizeof(logMetaData.tm));
	pParamValues->timestampSec = (UInt32)logMetaData.timestamp.tv_sec;
	pParamValues->timestampUsec = (UInt32)logMetaData.timestamp.tv_usec;
	pParamValues->globalTick = logMetaData.currentTick;
	va_start(va,str);
    for(index =0; index <5; index++)
        pParamValues->intValues[index]=(UInt32)va_arg(va,UInt32);
    va_end(va);


	size += sizeof(LogParamValues);

	strncpy(pParamValues->params.strValues[0], str, MAX_LOG_STR_LEN * 2);
	pParamValues->params.strValues[0][MAX_LOG_STR_LEN * 2 - 1] = '\0';

	if((l2Input.shmSize - (write_offset + size)) < MAX_LOG_LENGTH) {
		pLogHdr->writeOffset = LOG_FILE_HEADER_SIZE;
		pLogHdr->rotatedFlag = 1;
	} else {
		pLogHdr->writeOffset = write_offset + size;
	}
}

/* CA Logging Changes Start */
/*SPR 16855 +-*/
/*****************************************************************************
 * Function Name  : startExecSchedLogReader
 * Inputs         : l2Input, internalCellIndex and log_file_name.
 * Outputs        : None
 * Returns        : void
 * Description    : This function initializes the log reader, ltebinread, for
 *                  execution schedulers.
 *****************************************************************************/
void startExecSchedLogReader(
        lteL2LogOption     *l2Input_p, 
        Char8*             p_log_file_name)
{
    Char8 inputMacFlowFileName[NAME_MAX] = "--maclogs=" ;
    Char8 shmSize[NAME_MAX] = "--shm_size=";
    Char8 size[MAX_LOGGER_TYPE_STR_LEN] ={0};
    Char8 shmName[NAME_MAX] = "--shm_name=" ;
    Char8 inputLoggerOption[NAME_MAX] = "--logger_option=" ;
    Char8 inputLogFileSize[NAME_MAX] = "--log_file_size=" ;
    Char8 inputNumOfFiles[NAME_MAX] = "--log_num_files=" ;
    /*SPR21717 changes start*/
    char binName[NAME_MAX] = "--binName=";
    if(l2Input.l2gtpuflag)
    {
        strcat(binName,"lte_gtpu");
    }
    else
    {
        strcat(binName,"lteLayer2");
    }
    /*SPR21717 changes End*/

    macFlowpid_g =0;
    /* + Coverity 32084 */
    strnCat(inputMacFlowFileName,p_log_file_name,NAME_MAX-strnlen(inputMacFlowFileName,NAME_MAX-1)-1);
    /* - Coverity 32084 */
    inputMacFlowFileName[NAME_MAX-1]='\0';

    snprintf(size,MAX_LOGGER_TYPE_STR_LEN,"%d",l2Input_p->shmSize/(1024*1024));
    size[MAX_LOGGER_TYPE_STR_LEN - 1] = '\0';

    strnCat(shmSize,size,NAME_MAX-strnlen(size,MAX_LOGGER_TYPE_STR_LEN)-1);
    shmSize[NAME_MAX-1]= '\0';

    strnCat(inputLoggerOption,l2Input_p->logger_option,NAME_MAX-strnlen(l2Input_p->logger_option,MAX_LOGGER_TYPE_STR_LEN)-1);
    inputLoggerOption[NAME_MAX-1]= '\0'; 

    strnCat(inputLogFileSize,l2Input_p->logFileSize,NAME_MAX-strnlen(l2Input_p->logFileSize,MAX_LOGGER_TYPE_STR_LEN)-1);
    inputLogFileSize[NAME_MAX-1]= '\0';

    strnCat(inputNumOfFiles,l2Input_p->numFile,NAME_MAX-strnlen(l2Input_p->numFile,MAX_LOGGER_TYPE_STR_LEN)-1);
    inputNumOfFiles[NAME_MAX-1]= '\0';

    macFlowpid_g = fork();
    if(macFlowpid_g == 0)
    {
        sprintf(shmName,"%s%s",shmName, p_log_file_name);
        if (-1 == execl("ltebinread", "ltebinread", shmName, shmSize, inputMacFlowFileName, 
                    inputLoggerOption, inputLogFileSize, binName, inputNumOfFiles, NULL))
        {
            fprintf(stderr, "reader spawn failed [%s], log won't be stored\n", shmName);
            exit(1);
        }
    }
}
/*SPR 16855 +-*/
/* CA Logging Changes End */

/* +SPR_10381 Changes */
#ifdef DL_DATA_SEPARATION
/*****************************************************************************
 * Function Name  : startEncoderLogReader
 * Inputs         : l2Input, internalCellIndex and log_file_name.
 * Outputs        : None
 * Returns        : void
 * Description    : This function initializes the log reader, ltebinread, for
 *                  encoder threads.
 *****************************************************************************/
void startEncoderLogReader(
        lteL2LogOption     *l2Input_p,
        Char8*             p_log_file_name)
{
    Char8 inputEncoderFlowFileName[NAME_MAX] = "--maclogs=" ;
    Char8 shmSize[NAME_MAX] = "--shm_size=";
    Char8 size[MAX_LOGGER_TYPE_STR_LEN] ={0};
    Char8 shmName[NAME_MAX] = "--shm_name=" ;
    Char8 inputLoggerOption[NAME_MAX] = "--logger_option=" ;
    Char8 inputLogFileSize[NAME_MAX] = "--log_file_size=" ;
    Char8 inputNumOfFiles[NAME_MAX] = "--log_num_files=" ;
    /*SPR21717 changes start*/
    char binName[NAME_MAX] = "--binName=";
    if(l2Input.l2gtpuflag)
    {
        strcat(binName,"lte_gtpu");
    }
    else
    {
        strcat(binName,"lteLayer2");
    }
    /*SPR21717 changes End*/

    encoderFlowpid_g =0;
    /* + Coverity 32084 */
    /*SPR 16337 Start*/
    strnCat(inputEncoderFlowFileName, p_log_file_name, NAME_MAX-strnlen(inputEncoderFlowFileName,NAME_MAX-1)-1);
    /*SPR 16337 End*/
    /* - Coverity 32084 */
    inputEncoderFlowFileName[NAME_MAX-1]='\0';

    snprintf(size,MAX_LOGGER_TYPE_STR_LEN,"%d",l2Input_p->shmSize/(1024*1024));
    size[MAX_LOGGER_TYPE_STR_LEN - 1] = '\0';

    strnCat(shmSize,size,NAME_MAX-strnlen(size,MAX_LOGGER_TYPE_STR_LEN)-1);
    shmSize[NAME_MAX-1]= '\0';

    strnCat(inputLoggerOption,l2Input_p->logger_option,NAME_MAX-strnlen(l2Input_p->logger_option,MAX_LOGGER_TYPE_STR_LEN)-1);
    inputLoggerOption[NAME_MAX-1]= '\0';

    strnCat(inputLogFileSize,l2Input_p->logFileSize,NAME_MAX-strnlen(l2Input_p->logFileSize,MAX_LOGGER_TYPE_STR_LEN)-1);
    inputLogFileSize[NAME_MAX-1]= '\0';

    strnCat(inputNumOfFiles,l2Input_p->numFile,NAME_MAX-strnlen(l2Input_p->numFile,MAX_LOGGER_TYPE_STR_LEN)-1);
    inputNumOfFiles[NAME_MAX-1]= '\0';

    encoderFlowpid_g = fork();
    if(encoderFlowpid_g == 0)
    {
        sprintf(shmName,"%s%s",shmName, p_log_file_name);
        if (-1 == execl("ltebinread", "ltebinread", shmName, shmSize, inputEncoderFlowFileName,
                    inputLoggerOption, inputLogFileSize, inputNumOfFiles, binName, NULL))
        {
            fprintf(stderr, "reader spawn failed [%s], log won't be stored\n", shmName);
            exit(1);
        }
    }
}
#endif
/* -SPR_10381 Changes */
/****************************************************************************
 * Function Name  : l2_map_ext_loglevel_to_intern_loglevel
 * Inputs         : log_level received from OAM cfg file
 * Outputs        : log_level value used internally for MAC, RLC, PDCP, modules
 * Returns        : Nothing
 * Description    : This function maps the log_level parsed from cfg file
 *                   into log_level used for different modules.
 *                  --------------------------
 *                  LOG_LEVEL GENERIC NOTATION
 *                  --------------------------
 *                  Critical/Fatal  = 0
 *                  Error           = 1
 *                  Warning         = 2
 *                  Info            = 3
 *                  Detailed        = 4
 *                  Critical/Fatal  = Default
 *                  Internally they will be mapped as per it is used in the
 *                  respective modules
 ****************************************************************************/
/* SPR 20229 changes Start */
void l2_map_ext_loglevel_to_intern_loglevel(UInt32 *p_log_level)
{
/* SPR 20229 changes End */
    UInt8 log_level = *p_log_level;
    switch(log_level)
    {
        case LTE_LOG_CRITICAL:
        {
            *p_log_level = LOGFATAL;
            break;
        }
        case LTE_LOG_ERROR:
        {
            *p_log_level = LOGERROR;
            break;
        }
        case LTE_LOG_WARNING:
        {
            *p_log_level = LOGWARNING;
            break;
        }
        case LTE_LOG_INFO:
        {
            *p_log_level = LOGINFO;
            break;
        }
        case LTE_LOG_BRIEF:
        {
            *p_log_level = LOGBRIEF;
            break;
        }
        case LTE_LOG_DETAILED:
        {
            *p_log_level = LOGDEBUG;
            break;
        }
        case LTE_LOG_DETAILEDALL:
        {
            *p_log_level = LOGTTIDEBUG;
            break;
        }
        default:
        {
            *p_log_level = LOGERROR; 
            break;
        }
    }
}
