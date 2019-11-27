/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2011 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: ltebinread.c,v  2011/06/24 06.00:00 gur04640 Exp $
 ****************************************************************************
 *
 *  File Description :
 *  This file contains implementaion for unix cat type utility
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: ltebinread.c,v $
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
#include <getopt.h>
/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "logging.h"
#include "reader.h"
/****************************************************************************
  Private Definitions
 ****************************************************************************/

/****************************************************************************
  Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/*****************************************************************************
 * Private Constants
 ****************************************************************************/




/****************************************************************************
 * Exported Variables
 ****************************************************************************/
ReaderConfig    binReaderConfig;
/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

/*****************************************************************************
 * Function Name  : main
 * Inputs         : log file name
 * Outputs        : None
 * Returns        : SUCCESS/FAILURE
 * Description    : This function initialize reader framework and call 
 *                  ltecat utility function.
 *****************************************************************************/
int main (int argc, char **argv)
{
    struct sched_param sp = {0}; 
    int option_index = 0;
    int iterator = 0;
    int input_option;
    static struct option long_options[] = {
        {"maclogs", 1, NULL, 'f'},
        {"logger_option", 1, NULL, 'o'},
        {"log_file_size", 1, NULL, 'b'},
        {"log_num_files", 1, NULL, 'n'},
        {"shm_name", 1, NULL, 's'},
        {"shm_size", 1, NULL, 'l'},
        /*SPR21717 Changes Start */
        {"binName", 1, NULL, 'i'},
        /*SPR21717 Changes End */
        {"help", 0, NULL, 'h'},
        {0, 0, 0, 0}
    };
    sleep(1);
    sp.sched_priority = 0;
    sched_setscheduler(getpid_wrapper(), SCHED_OTHER, &sp);
    while (1) 
    {
        input_option = getopt_long(argc, argv, "",long_options, &option_index);//explain command line
        if(input_option ==-1)
            break;

        switch(input_option)
        {
            case 'f':
            strncpy(binReaderConfig.logFileInput,optarg, NAME_MAX);
            binReaderConfig.logFileInput[NAME_MAX - 1] = '\0';
            break;
            case 'o':
            strncpy(binReaderConfig.loggerType,optarg,MAX_LOGGER_TYPE_STR_LEN);
            binReaderConfig.loggerType[MAX_LOGGER_TYPE_STR_LEN - 1] = '\0';
            break;
            case 'b':
            /* +SPR_11258 Changes */
            binReaderConfig.logFileSize = atoi(optarg);
            /* -SPR_11258 Changes */
            break;
            case 'n':
            binReaderConfig.maxNumFiles = atoi(optarg); 
            if (MAX_LOG_FILES_ALLOWED < binReaderConfig.maxNumFiles)
            {
                fprintf(stderr, "Max log files allowed = %d\n",
                    MAX_LOG_FILES_ALLOWED);
                exit(0);
            }
            break;
            case 's':
            strncpy(binReaderConfig.shmName, optarg, NAME_MAX);
            binReaderConfig.shmName[NAME_MAX - 1] = '\0';
            break;
            /*SPR21717 Changes Start */
            case 'i':
            strncpy(binReaderConfig.binName, optarg, NAME_MAX);
            binReaderConfig.binName[NAME_MAX - 1] = '\0';
            /*SPR21717 Changes End */
            break;
            case 'l':
            binReaderConfig.shmSize = (atoi(optarg)* 1024 * 1024);
            break;
            case 'h':
            default:
            fprintf(stderr," Usage is %s  --maclogs=<filename>" 
                    "--logger_option=<console/text/binary>"
                    "--shm_name=<MAC_RLC_REGION/RLC_PDCP_REGION> --log_file_size=<in MB> "
                    "--log_num_files=<number of files> --help ", argv[0]);
            fprintf(stderr,"\n"
                    "--maclogs=<filename>                            ->filename of generated maclogs\n"
                    "--logger_option=<console/text/binary/offline>   ->logs format and location\n"
                    "--log_file_size=<in MB>                         ->size of log-binFiles\n"
                    "--shm_name=<MAC_RLC_REGION/RLC_PDCP_REGION>     ->shared memory region to be mapped for logs\n"
                    "--shm_size=<size in MB>                         ->size of shared memory\n"
                    "--log_num_files=<number of files>               ->no. of log-binFiles to be generated [max 100]"
                    /*SPR21717 Changes Start */
                    "--binName=<name of binary file>                 ->Name of Binary file whose logs are to be read");
                    /*SPR21717 Changes End */
            break;
        }
    }
    if (binReaderConfig.logFileSize == 0)
    {
        binReaderConfig.logFileSize = MAX_LOG_MEMORY_SIZE;
    }
    if (binReaderConfig.maxNumFiles == 0)
    {
        binReaderConfig.maxNumFiles = MAX_LOG_FILES_DEFAULT;
    }
    if (binReaderConfig.shmSize == 0)
    {
        binReaderConfig.shmSize = MAX_LOG_MEMORY_SIZE;
    }
    /*SPR21717 Changes Start */
    if (binReaderConfig.binName[0] == '\0')
    {
        fprintf(stderr,"Binary File name not provided\n");
        exit(0);
    }
    /*SPR21717 Changes End */
    binReaderConfig.curLogFileNum = 0;
    for (iterator = 0; iterator < MAX_LOG_FILES_ALLOWED; iterator++)
    {
        binReaderConfig.logFileNames[iterator][0] = '\0';
    }
    if (!strcmp(binReaderConfig.loggerType, "offline"))
    {
        /*SPR21717 Changes Start */
        readELFFile(binReaderConfig.binName);
        /*SPR21717 Changes end */
        if (strlen_wrapper(binReaderConfig.logFileInput) == 0)
        {
            fprintf(stderr, "logs file name is required\n");
            exit(0);
        }
        lte_cat_bindata(binReaderConfig.logFileInput, "");
    }
    else
    {
        if (strlen_wrapper(binReaderConfig.shmName) == 0)
        {
            fprintf(stderr, "SHM name required in online modes\n");
            exit(0);
        }
        if (strlen_wrapper(binReaderConfig.logFileInput) == 0)
        {
            /* Coverity TI fix start */
            strncpy(binReaderConfig.logFileInput,
                binReaderConfig.shmName, NAME_MAX - 1);
            /* +COVERITY 10887 */
            /* +COVERITY 10002 */
            /* Coverity TI fix end */
            binReaderConfig.logFileInput[NAME_MAX - 1]= '\0';
            /* -COVERITY 10002 */
            /* -COVERITY 10887 */
        }
        if(0 == online_reader_init(&binReaderConfig, binReaderConfig.shmName))
        {
            if (!strcmp(binReaderConfig.loggerType, "binary"))
            {
                lte_binlog_data(&binReaderConfig,500);
            }
            else if (!strcmp(binReaderConfig.loggerType, "text") || !strcmp(binReaderConfig.loggerType, "txt"))
            {
                lte_textlog_data(&binReaderConfig,500);
            }
            else if (!strcmp(binReaderConfig.loggerType, "console"))
            {
                lte_consolelog_data(&binReaderConfig,500);
            }
            else
            {
                fprintf(stderr, "Wrong reader type\n");
                exit(0);
            }
        }
        else
        {
            fprintf(stderr,"reader init failed\n");
        }
    }
    
    return 0;
}


//#ifdef LINUX_PC_TEST
 void SEND_TO_TEST(unsigned int ulMsgId, char *buf, int len)
{
	return;
}
//#endif



