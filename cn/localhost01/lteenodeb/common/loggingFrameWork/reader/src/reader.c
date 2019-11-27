/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2011 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: reader.c,v  2011/01/27 06.00:00 gur19170 Exp $
 ****************************************************************************
 *
 *  File Description :
 *  This file contains the implementation of reader of circular log file
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: reader.c,v $
 * Initial version
 *
 ****************************************************************************/


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
#include <stdio.h>  
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <limits.h>

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "logging.h"
#include "reader.h"

/****************************************************************************
  Private Definitions
 ****************************************************************************/
#define MAX_LOG_FILE_NAME_LEN 256
/****************************************************************************
  Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
void interpret_fapi_log(Log_FAPI_st *pfapi_logs, FILE *fptr);
/*****************************************************************************
 * Private Constants
 ****************************************************************************/




/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/
static void *p_shm_info;
static Char8 logfilename[MAX_LOG_FILE_NAME_LEN] = "";
Char8* roBuf_g=PNULL;
UInt32 roBufBaseAddr_g=0;


extern Char8* logLevelStrValues[];

extern Char8* logModuleStrValues[];
const Char8 *getLogCategoryStrValues(UInt64 logCategory);

/*****************************************************************************
 * Function Name  : reader_init
 * Inputs         : ModuleId 
 * Outputs        : 
 * Returns        : SUCCESS/FAILURE
 * Description    : This function open given file in read only mode and 
 *                  map to a shared memory object.
 *****************************************************************************/
UInt32 reader_init(Char8 *p_file_name)
{
  SInt32 shmfd;
  void *p_shmptr = NULL;
  mode_t omask;

  /*Creating shared memory object          ----shm_open()*/
  omask = umask(0);
  shmfd = shm_open(p_file_name, O_RDWR, (S_IRWXU|S_IRWXG|S_IRWXO));
  umask(omask);
  if(shmfd < 0)
 {
   PERRORMSG("In shm_open :");
   return 1;
 }

 
  /*Requesting the shared segment*/ 
  p_shmptr =  mmap(NULL, MAX_LOG_MEMORY_SIZE, PROT_READ|PROT_WRITE,
             MAP_SHARED, shmfd, 0);

  if(MAP_FAILED == p_shmptr)
  {
    shm_unlink(p_file_name);
    PERRORMSG("In mmap():");
    return(1);
  }
  /* Coverity TI Fix start */
  strncpy(logfilename, p_file_name, MAX_LOG_FILE_NAME_LEN - 1);
  logfilename[MAX_LOG_FILE_NAME_LEN - 1] = '\0';
  /* Coverity TI Fix end */
  p_shm_info = p_shmptr;
  return(0); 	
}

/*****************************************************************************
 * Function Name  : reader_init
 * Inputs         : ModuleId
 * Outputs        :
 * Returns        : SUCCESS/FAILURE
 * Description    : This function open given file in read only mode and
 *                  map to a shared memory object.
 *****************************************************************************/
UInt32 online_reader_init(ReaderConfig *rdConf, Char8 *p_file_name)
{
    SInt32 shmfd;
    void *p_shmptr = NULL;
    mode_t omask;
    
    /*Creating shared memory object          ----shm_open()*/
    //we cannot use shm_open_wrapper here, because it will link lteMisc and report more linking error while generating ltebinreader
    omask = umask(0);
    shmfd = shm_open(p_file_name, O_RDWR, (S_IRWXU|S_IRWXG|S_IRWXO));
    umask(omask);
    if(shmfd < 0)
    {
        PERRORMSG("In shm_open :");
        return 1;
    }


    /*Requesting the shared segment*/
    p_shmptr =  mmap(NULL, rdConf->shmSize, PROT_READ|PROT_WRITE,
            MAP_SHARED, shmfd, 0);

    if(MAP_FAILED == p_shmptr)
    {
        shm_unlink(p_file_name);
        PERRORMSG("In mmap():");
        return(1);
    }
    /* Coverity TI Fix start */
    strncpy(logfilename, p_file_name, MAX_LOG_FILE_NAME_LEN - 1);
    logfilename[MAX_LOG_FILE_NAME_LEN - 1] = '\0';
    /* Coverity TI Fix end */
    p_shm_info = p_shmptr;
    return(0);
}

/*****************************************************************************
 * Function Name  : get_current_offset
 * Inputs         : Shared memory pointer
 * Outputs        : None.
 * Returns        : write offset.
 * Description    : This function returns write offset available in shared 
 *                  memory object.
 *****************************************************************************/
UInt32 get_current_offset(Char8 *p_shm_pointer)
{
    return ((LogShmHeader *)p_shm_pointer)->writeOffset;
}

/*****************************************************************************
 * Function Name  : lte_cat_data
 * Inputs         : None 
 * Outputs        : None
 * Returns        : Void
 * Description    :  Function is used to print at stdout given file data
 *****************************************************************************/
/* + SPR 17439 */
void lte_cat_data(void)
/* - SPR 17439 */    
{
    UInt32 new_offset, old_offset;
    Char8 *p_temp = p_shm_info;

    LogParamValues paramValues;
    Log_FAPI_st log_fapi_st;

    /* Initialize old read offset to current write offset */
    old_offset = get_current_offset(p_temp); 
    new_offset = old_offset;

    do
    {
        /*
         * Check for the shared memory wrap around condition.
         * If remaining shm size is less than max log size 
         * then wrap around and start reading from starting of trace section.
         */
        if((MAX_LOG_MEMORY_SIZE - (old_offset)) < MAX_FAPI_LOG_LENGTH )
        {
            old_offset=LOG_FILE_HEADER_SIZE;
        }

        if(FAPI_INTF_LOG_ID == *(UInt32 *)(p_temp + old_offset))
        {
            memcpy(&log_fapi_st, p_temp + old_offset, sizeof(Log_FAPI_st));
            old_offset += sizeof(Log_FAPI_st);
            interpret_fapi_log(&log_fapi_st, stdout);
        }
        else
        {
            /* Copy the trace and Increment old read offset with trace size */ 
            memcpy(&paramValues,p_temp + old_offset, sizeof(LogReaderParamValues));
            old_offset += sizeof(LogReaderParamValues);
            if(paramValues.logId==NEW_BIN_LOG)
            {
                interpet_logs_bin_file(&paramValues,stdout);
            }    
            else
            {    
                interpret_log(&paramValues, stdout);
            }    
        }

        /*file_print(paramValues.logId,paramValues.logLevel,\
          paramValues.logCategory,paramValues.globalTick,\
          paramValues.intValues[0],paramValues.intValues[1],\
          paramValues.intValues[2],paramValues.intValues[3],\
          paramValues.intValues[4],paramValues.flValues[0],\
          paramValues.flValues[1],paramValues.strValues[0],paramValues.strValues[1]);*/

    }while (new_offset != old_offset);
}

/*****************************************************************************
 * Function Name  : lte_tail_data
 * Inputs         : wait timer 
 * Outputs        : None 
 * Returns        : Void
 * Description    : Function is used to write on stdout latest written data 
 *                  in the log file.
 *****************************************************************************/
void lte_tail_data(unsigned int timer)
{
    UInt32 new_offset, old_offset;
    Char8 *p_temp = p_shm_info;
    LogParamValues paramValues;
    Log_FAPI_st log_fapi_st;
   
    /* Initialize old read offset to current write offset */
    old_offset = get_current_offset(p_temp); 
   
    while(1)
    {
        /* wait for a configurable period of time 
         *  before checking for new logs
         */
        sleep(timer);

        /* Set current read offset = current write offset */
        new_offset = get_current_offset(p_temp);

        /*Check if old read offset is same as the current read offset */
        while (new_offset != old_offset)
        {
            /*
             * Check for the shared memory wrap around condition.
             * If remaining shm size is less than max log size 
             * flag is set, then wrap around and start reading from starting of trace section.
             */
            
            if((MAX_LOG_MEMORY_SIZE - (old_offset)) < MAX_FAPI_LOG_LENGTH)
            {
                old_offset=LOG_FILE_HEADER_SIZE;
            }
            
            if(FAPI_INTF_LOG_ID == *(UInt32 *)(p_temp + old_offset))
            {
                memcpy(&log_fapi_st, p_temp + old_offset, sizeof(Log_FAPI_st));
                old_offset += sizeof(Log_FAPI_st);
                interpret_fapi_log(&log_fapi_st, stdout);
            }
            else
            {
                /* Copy the trace and Increment old read offset with trace size */ 
                memcpy(&paramValues,p_temp + old_offset, sizeof(LogReaderParamValues));
                old_offset += sizeof(LogReaderParamValues);

                /* print the trace to stdout */ 
                interpret_log(&paramValues, stdout);
                /*console_print(paramValues.logId,paramValues.logLevel,\
                paramValues.logCategory,paramValues.globalTick,\
                paramValues.intValues[0],paramValues.intValues[1],\
                paramValues.intValues[2],paramValues.intValues[3],\
                paramValues.intValues[4],paramValues.flValues[0],\
                paramValues.flValues[1],paramValues.strValues[0],paramValues.strValues[1]);*/
            }
        }
    }/* While loop end*/
}

    /* +- SPR 17777 */
static FILE *create_binlog_file_and_open(
        ReaderConfig * rdConf)
    /* +- SPR 17777 */
{
    FILE *fp;

    if (strlen(rdConf->logFileNames[
                rdConf->curLogFileNum % rdConf->maxNumFiles]) != 0)
    {
        /* +COVERITY 10003/10004 */
        if(-1 == remove(rdConf->logFileNames[
                rdConf->curLogFileNum % rdConf->maxNumFiles]))
        { 
             //Logs to be added here
        }
    }
    snprintf(rdConf->logFileNames[
                rdConf->curLogFileNum % rdConf->maxNumFiles],
                NAME_MAX, "%s%02d.bin",
                rdConf->logFileInput, rdConf->curLogFileNum);

    fp = fopen(rdConf->logFileNames[rdConf->curLogFileNum % rdConf->maxNumFiles], "wb+");
    if (NULL == fp)
    {
        fprintf(stderr, "File open failed [%s]\n",
                rdConf->logFileNames[rdConf->curLogFileNum % rdConf->maxNumFiles]);
        return NULL;
    }
    rdConf->curLogFileNum++;

    return fp;
}

    /* +- SPR 17777 */
static FILE *create_textlog_file_and_open(
        ReaderConfig * rdConf)
    /* +- SPR 17777 */
{
    FILE *fp;

    if (strlen(rdConf->logFileNames[
                rdConf->curLogFileNum % rdConf->maxNumFiles]) != 0)
    {
        if(-1 == remove(rdConf->logFileNames[
                rdConf->curLogFileNum % rdConf->maxNumFiles]))
        {
            //LOGS to be added here
        }
        /* -COVERITY 10003/10004 */
    }
    snprintf(rdConf->logFileNames[
                rdConf->curLogFileNum % rdConf->maxNumFiles],
                NAME_MAX, "%s%02d.txt",
                rdConf->logFileInput, rdConf->curLogFileNum);

    fp = fopen(rdConf->logFileNames[rdConf->curLogFileNum % rdConf->maxNumFiles], "w+");
    if (NULL == fp)
    {
        fprintf(stderr, "File open failed [%s]\n",
                rdConf->logFileNames[rdConf->curLogFileNum % rdConf->maxNumFiles]);
        return NULL;
    }
    rdConf->curLogFileNum++;

    return fp;
}

/*****************************************************************************
 * Function Name  : lte_cat_data
 * Inputs         : None 
 * Outputs        : None
 * Returns        : Void
 * Description    :  Function is used to print at stdout given file data
 *****************************************************************************/
void lte_cat_bindata(Char8 *binfilename, Char8 *outfilename)
{
    FILE *fpbin, *fpout;
    /* + COVERITY 5.0 */    
    LogParamValues paramValues = {0,0,0,0,{0},{{{0}}}};
    /* - COVERITY 5.0 */    
    Log_FAPI_st fapiLogs = {0,0,0,0,0,{0},{0}};
   
    fpbin = fopen(binfilename, "rb+");
    if (NULL == fpbin)
    {
        fprintf(stderr, "File open failed [%s]\n", binfilename);
        return ;
    }

    if (strlen(outfilename) == 0)
    {
        fpout = stdout;
    }
    else
    {
        fpout = fopen(outfilename, "w+");
        if (NULL == fpout)
        {
            fprintf(stderr, "File open failed [%s]\n", outfilename);
            /* SPR 4010 Fix Start */
            fclose(fpbin);
            /* SPR 4010 Fix End */
            return ;
        }
    }
    
    do
    {
        UInt32 logId = INVALID_LOG_ID;
        /*Coverity 96701 fix +*/
        UInt8 items_read = fread(&logId, sizeof(UInt32), 1, fpbin);
        if(1 != items_read ){
            if (fpout != stdout)
            {
                fclose(fpout);
            }
            fclose(fpbin);
            return;
        }
        /*Coverity 96701 fix -*/
        if (FAPI_INTF_LOG_ID == logId)
        {
            fapiLogs.logId = logId;
            if (1 != fread(&(fapiLogs.timestampSec), sizeof(fapiLogs)-sizeof(logId), 1, fpbin))
            {
                /* SPR 4010 Fix Start */
                if (fpout != stdout)
                {
                    fclose(fpout);
                }
                fclose(fpbin);
                /* SPR 4010 Fix End */
                return;
            }
            interpret_fapi_log(&fapiLogs, fpout);
        }
        else
        {
            paramValues.logId  = logId;
            /* Copy the trace and Increment old read offset with trace size */ 
            if (1 != fread(&(paramValues.timestampSec), sizeof(LogReaderParamValues)-sizeof(logId), 1, fpbin))
            {
                /* SPR 4010 Fix Start */
                if (fpout != stdout)
                {
                    fclose(fpout);
                }
                fclose(fpbin);
                /* SPR 4010 Fix End */
                return;
            }

            if(paramValues.logId==NEW_BIN_LOG)
            {
                interpet_logs_bin_file(&paramValues,stdout);
            }    
            else
            {    
                interpret_log(&paramValues, stdout);
            }    
        }

    }while (1);
}

/*****************************************************************************
 * Function Name  : lte_binlog_data
 * Inputs         : wait timer 
 * Outputs        : None 
 * Returns        : Void
 * Description    : Function is used to write on stdout latest written data 
 *                  in the log file.
 *****************************************************************************/
void lte_binlog_data(ReaderConfig *rdConf, unsigned int timer)
{
    LogShmHeader *pShmHeader = NULL;
    FILE *binLogFd;
   
    pShmHeader = (LogShmHeader *) p_shm_info;

    /* create new log file and open it */
    /* +- SPR 17777 */
    if (NULL == (binLogFd = create_binlog_file_and_open(rdConf)))
    /* +- SPR 17777 */
    {
        fprintf(stderr, "log file create failed\n");
        return;
    }
   
    while(1)
    {
        /* wait for a configurable period of time 
         *  before checking for new logs
         */
        usleep(timer);

        /*Check if old read offset is same as the current read offset */
        while (pShmHeader->readOffset != pShmHeader->writeOffset)
        {
            /*
             * Check for the shared memory wrap around condition.
             * If remaining shm size is less than max log size 
             * flag is set, then wrap around and start reading from starting of trace section.
             */
            if((SInt32)(rdConf->shmSize - (pShmHeader->readOffset)) < MAX_FAPI_LOG_LENGTH)
            {
                pShmHeader->readOffset=LOG_FILE_HEADER_SIZE;
            }
            if(FAPI_INTF_LOG_ID == *(UInt32 *)((Char8 *)pShmHeader + pShmHeader->readOffset))
            {
                fwrite((Char8 *)pShmHeader + pShmHeader->readOffset, sizeof(Log_FAPI_st), 1, binLogFd);
                pShmHeader->readOffset += sizeof(Log_FAPI_st);
            }
            else
            {
                /* Copy the trace and Increment old read offset with trace size */ 
                fwrite((Char8 *)pShmHeader + pShmHeader->readOffset, sizeof(LogParamValues), 1, binLogFd);
                pShmHeader->readOffset += sizeof(LogParamValues);
            }
            if ((unsigned int)ftell(binLogFd) > rdConf->logFileSize)
            {
                fclose(binLogFd);
                /* +- SPR 17777 */
                if (NULL == (binLogFd = create_binlog_file_and_open(rdConf)))
                /* +- SPR 17777 */
                {
                    fprintf(stderr, "log file create failed\n");
                    return;
                }
            }

        }
        if((SInt32)(rdConf->shmSize - (pShmHeader->readOffset)) < MAX_FAPI_LOG_LENGTH)
        {
            pShmHeader->readOffset=LOG_FILE_HEADER_SIZE;
        }
        fflush(binLogFd);
    }/* While loop end*/
}

/*****************************************************************************
 * Function Name  : lte_binlog_data
 * Inputs         : wait timer 
 * Outputs        : None 
 * Returns        : Void
 * Description    : Function is used to write on stdout latest written data 
 *                  in the log file.
 *****************************************************************************/
void lte_textlog_data(ReaderConfig *rdConf, unsigned int timer)
{
    LogShmHeader *pShmHeader = NULL;
    FILE *textLogFd;
   
    pShmHeader = (LogShmHeader *) p_shm_info;

    /* create new log file and open it */
    /* +- SPR 17777 */
    if (NULL == (textLogFd = create_textlog_file_and_open(rdConf)))
    /* +- SPR 17777 */
    {
        fprintf(stderr, "log file create failed\n");
        return;
    }
   
    while(1)
    {
        /* wait for a configurable period of time 
         *  before checking for new logs
         */
        usleep(timer);
        while (pShmHeader->readOffset != pShmHeader->writeOffset)
        {
            /*
             * Check for the shared memory wrap around condition.
             * If remaining shm size is less than max log size 
             * flag is set, then wrap around and start reading from starting of trace section.
             */
            if((SInt32)(rdConf->shmSize - (pShmHeader->readOffset)) < MAX_FAPI_LOG_LENGTH)
            {
                pShmHeader->readOffset=LOG_FILE_HEADER_SIZE;
            }
            if(FAPI_INTF_LOG_ID == *(UInt32 *)((Char8 *)pShmHeader + pShmHeader->readOffset))
            {
                interpret_fapi_log((Log_FAPI_st *)((Char8 *)pShmHeader + pShmHeader->readOffset), textLogFd);
                pShmHeader->readOffset += sizeof(Log_FAPI_st);
            }
            else
            {

                /* Copy the trace and Increment old read offset with trace size */ 
                interpret_log((LogParamValues *)((Char8 *)pShmHeader + pShmHeader->readOffset), textLogFd);
                pShmHeader->readOffset += sizeof(LogParamValues);
            }

            if ((unsigned int)ftell(textLogFd) > rdConf->logFileSize)
            {
                fclose(textLogFd);
                /* +- SPR 17777 */
                if (NULL == (textLogFd = create_textlog_file_and_open(rdConf)))
                /* +- SPR 17777 */
                {
                    fprintf(stderr, "log file create failed\n");
                    return;
                }
            }

        }
        if((SInt32)(rdConf->shmSize - (pShmHeader->readOffset)) < MAX_FAPI_LOG_LENGTH)
        {
            pShmHeader->readOffset=LOG_FILE_HEADER_SIZE;
        }

        fflush(textLogFd);
    }/* While loop end*/
}

/*****************************************************************************
 * Function Name  : lte_binlog_data
 * Inputs         : wait timer 
 * Outputs        : None 
 * Returns        : Void
 * Description    : Function is used to write on stdout latest written data 
 *                  in the log file.
 *****************************************************************************/
void lte_consolelog_data(ReaderConfig *rdConf, unsigned int timer)
{
    LogShmHeader *pShmHeader = NULL;
    FILE *consoleLogFd;
   
    pShmHeader = (LogShmHeader *) p_shm_info;

    /* create new log file and open it */
    consoleLogFd = stdout;
   
    while(1)
    {
        /* wait for a configurable period of time 
         *  before checking for new logs
         */
        usleep(timer);

        /*Check if old read offset is same as the current read offset */
        while (pShmHeader->readOffset != pShmHeader->writeOffset)
        {
            /*
             * Check for the shared memory wrap around condition.
             * If remaining shm size is less than max log size 
             * flag is set, then wrap around and start reading from starting of trace section.
             */
            
            if((SInt32)(rdConf->shmSize - (pShmHeader->readOffset)) < MAX_FAPI_LOG_LENGTH)
            {
                pShmHeader->readOffset=LOG_FILE_HEADER_SIZE;
            }
            if(FAPI_INTF_LOG_ID == *(UInt32 *)((Char8 *)pShmHeader + pShmHeader->readOffset))
            {
                interpret_fapi_log((Log_FAPI_st *)((Char8 *)pShmHeader + pShmHeader->readOffset), consoleLogFd);
                pShmHeader->readOffset += sizeof(Log_FAPI_st);
            }
            else
            {
                /* Copy the trace and Increment old read offset with trace size */ 
                interpret_log((LogParamValues *)
                    ((Char8 *)pShmHeader + pShmHeader->readOffset), consoleLogFd);
                pShmHeader->readOffset += sizeof(LogParamValues);
            }

        }
        if((rdConf->shmSize - (pShmHeader->readOffset)) < MAX_LOG_LENGTH )
        {
            pShmHeader->readOffset=LOG_FILE_HEADER_SIZE;
        }
        fflush(consoleLogFd);
    }/* While loop end*/
}

/*****************************************************************************
 * Function Name  : readELFHeders
 * Inputs         : fd: File Descriptor
 * Outputs        : elFHeader: Parsed ELF Headers
 * Returns        : Void
 * Description    : Read ELF headers
 *****************************************************************************/
void readELFHeader(int fd,Elf32_Ehdr *elfHeader)
{
	if(elfHeader ==PNULL)
    {
        return;
    }
    if((lseek(fd, (off_t)0, SEEK_SET)) != 0)
    {
        return;
    }
    /*CID 97612 fix*/    
    if(read(fd, (void *)elfHeader, sizeof(Elf32_Ehdr)) != sizeof(Elf32_Ehdr))
    {
        return;
    }    

}

/*****************************************************************************
 * Function Name  : readShTable
 * Inputs         : fd: File Descriptior
 *                  elfHeader: Parsed ELF Header
 * Outputs        : shTable: Symbol Table 
 * Returns        : Void
 * Description    : Read Layer2 ELF String Symbol Table
 *****************************************************************************/
void readShTable(SInt32 fd, Elf32_Ehdr elfHeader, Elf32_Shdr* shTable)
{
	int count;

	if(lseek(fd, (off_t)elfHeader.e_shoff, SEEK_SET) != ((off_t)elfHeader.e_shoff))
    {
         return;
    }    

	for(count=0; count<elfHeader.e_shnum; count++) 
    {
        if(read(fd, (void *)&shTable[count], elfHeader.e_shentsize) !=  elfHeader.e_shentsize)
        {
            return;
        }    
	}

}

/*****************************************************************************
 * Function Name  : readRODATASection
 * Inputs         : fd: File Descriptior
 *                  elfHeader: ELF Header
 *                  shTable: Symbol Table
 * Outputs        : None 
 * Returns        : Void
 * Description    : Read Layer2 ELF files
 *****************************************************************************/
void readRODATASection(int fd, Elf32_Ehdr elfHeader, Elf32_Shdr* shTable)
{
    unsigned int index;
    /* CID 97642 fix*/
    char* shStr = PNULL;   /* section-header string-table is also a section. */

    shStr = malloc(shTable[elfHeader.e_shstrndx].sh_size);

/*Cocerity :99465 fix start*/
    if(shStr == PNULL) 
    {
        printf("Unable to allocate memory");
        /* CID 97655 and 97618 fix*/
        return;
    }

    /* CID 97642 fix start*/ 
    memset((char *)shStr,0,sizeof(shTable[elfHeader.e_shstrndx].sh_size)); 
    /* CID 97642 fix end*/ 
/*Cocerity :99465 fix end*/
    if(lseek(fd, (off_t)shTable[elfHeader.e_shstrndx].sh_offset, SEEK_SET) != ((off_t)shTable[elfHeader.e_shstrndx].sh_offset))
    {
        /*CID 97666 fix*/
        free(shStr);
        return;
    }
    if(read(fd, (void *)shStr, shTable[elfHeader.e_shstrndx].sh_size) != ((ssize_t )shTable[elfHeader.e_shstrndx].sh_size))
    {
        /*CID 97666 fix*/
        free(shStr);
        return;
    }

    /****** Find .rodata index***************/	
    for(index=0; index<elfHeader.e_shnum; index++) 
    {
        if(!strcmp(".rodata", (shStr + shTable[index].sh_name))) 
        {
            break;
        }
    }


    if(lseek(fd, (off_t)shTable[index].sh_offset, SEEK_SET)!=((off_t)shTable[index].sh_offset))
    {
        /*CID 97666 fix*/
        free(shStr);
         return;
    }

	roBuf_g = malloc(shTable[index].sh_size);
    roBufBaseAddr_g=(unsigned int)shTable[index].sh_addr;

    if(roBuf_g == PNULL)
    {
        /*CID 97666 fix*/
        free(shStr);
	return;
    }

    if(read(fd,roBuf_g, shTable[index].sh_size)!=((ssize_t )shTable[index].sh_size))
    {
        /*CID 97666 fix*/
		free(roBuf_g);
		roBuf_g = PNULL;
		shStr = NULL;
		return;
    }

	if (*roBuf_g == '\0' || *roBuf_g == 0) {
		printf("sh_size is zero, %s, %d\n", *roBuf_g, *roBuf_g);
		free(roBuf_g);
		roBuf_g = PNULL;
	}
    free(shStr);
}


/*****************************************************************************
 * Function Name  : readELFFile
 * Inputs         : None
 * Outputs        : None 
 * Returns        : Void
 * Description    : Read Layer2 ELF files
 *****************************************************************************/
/*SPR21717 changes Start*/
void readELFFile(char * binName)
/*SPR21717 changes End*/
{
    int fd;
    Elf32_Ehdr elfHeader;		/* elf-header is fixed size */
    Elf32_Shdr* shTable;	/* section-header table is variable size */
    /*Coverity :97614 fix start*/
    UInt32 shTableSize = 0;
    /*Coverity :97614 fix end*/
    /* CID 97651 fix start*/ 
    memset((Elf32_Ehdr *)&elfHeader,0,sizeof(Elf32_Ehdr)); 
    /* CID 97651 fix end*/
    /*SPR21717 changes Start*/
    fd = open(binName, O_RDONLY|O_SYNC);
    /*SPR21717 changes End*/
    if(fd<0)
    {
        printf("Error %d Unable to open file \n", fd);
        return;
    }

    /* ELF header : at start of file */
    readELFHeader(fd, &elfHeader);
    /*Coverity :97614 fix start*/
    shTableSize = elfHeader.e_shentsize  * elfHeader.e_shnum;
    shTable= malloc(shTableSize);
    /*Coverity :97614 fix end*/
    /* CID 97637 fix*/
    if(PNULL == shTable)
    {   
        /* CID 98838 98834 fix start */
        printf("Unable to alloc memory");
        close(fd);
        /* CID 98838 98834 fix end */
        return;
    }
    readShTable(fd,elfHeader,shTable);
    readRODATASection(fd,elfHeader,shTable);
    free(shTable);
    close(fd);
    return;
}

/*****************************************************************************
 * Function Name  : interpet_logs_bin_file
 * Inputs         :  
 * Outputs        : None 
 * Returns        : None
 * Description    :   
 *****************************************************************************/
void interpet_logs_bin_file(LogParamValues *pParamValues, FILE *fptr)
{
    Char8* logLevelStr;
    Char8* logModuleStr;
    const Char8* logCatStr;
    struct tm timestamp;
    Char8* logStr;
    Char8* fmtSpecifier;
    UInt32 fmtIndex=0;
    UInt32 offset;
    UInt32 logLevel;
    UInt16 logModule;

	if (!roBuf_g) {
		printf("roBuf_g is NULL\n");
		return;
	}

    if(NULL == localtime_r(((time_t*)&pParamValues->tm), &timestamp))
    {
        fprintf(fptr, "timestamp decoding failed\n");
    }
    logLevel=pParamValues->params.logParams.logLevel; 
    logLevelStr= logLevelStrValues[logLevel];
    logModule=pParamValues->params.logParams.logModule;
    logModuleStr= logModuleStrValues[logModule];
    logCatStr=getLogCategoryStrValues(pParamValues->params.logParams.logCategory);


    fprintf (fptr, "[%04d-%02d-%02d/%02d:%02d:%02d.%03d]",
            timestamp.tm_year+1900, (timestamp.tm_mon + 1), timestamp.tm_mday,
            timestamp.tm_hour, timestamp.tm_min, timestamp.tm_sec,
            pParamValues->timestampUsec/1000);
    fprintf (fptr, "[% 4d-%02d][%5s][%4s][%5s]", ((UInt32)((pParamValues->globalTick/10)%1024)),
            ((UInt32)(pParamValues->globalTick%10)),logLevelStr,logModuleStr,logCatStr);

    /***FILE NAME****/
    offset=pParamValues->params.logParams.fileName- roBufBaseAddr_g;
/* SPR 23260 + */
	logStr = basename(roBuf_g+offset);
    fprintf(fptr,"[%s]",logStr);/*CID 97840*/
/* SPR 23260 - */

    /***FUNC NAME****/
    offset=pParamValues->params.logParams.funcName- roBufBaseAddr_g;
    fprintf(fptr,"[%s]",roBuf_g+offset);

    /***LINE NO****/
    fprintf(fptr,"[%u] ",pParamValues->params.logParams.lineNo);

    offset=pParamValues->params.logParams.str - roBufBaseAddr_g;

    logStr=(Char8*)roBuf_g+offset;

    /**********Findng %s specifier start**************/
    fmtSpecifier=strchr(logStr,'%');

    while (fmtSpecifier!=NULL)
    {
        if(logStr[fmtSpecifier-logStr+1] == 's')
        {
            offset=pParamValues->intValues[fmtIndex] - roBufBaseAddr_g;
    /* SPR 20430 Changes Start */
	    pParamValues->intValues[fmtIndex]=(ADDR)roBuf_g+offset;
    /* SPR 20430 Changes End */
        }
        fmtIndex++;
        fmtSpecifier=strchr(fmtSpecifier+1,'%');
    }
    /**********Findng %s specifier end**************/

    fprintf(fptr,logStr,pParamValues->intValues[0],pParamValues->intValues[1],pParamValues->intValues[2],\
                   pParamValues->intValues[3],pParamValues->intValues[4]);

    if(logStr[strlen(logStr)-1]!='\n')
    {
        fprintf (fptr,"\n");
    }    
}
