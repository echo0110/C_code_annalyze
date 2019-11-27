/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2011 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: reader.h,v  2011/01/27 06:00:00 gur19170 Exp $
 ****************************************************************************
 *
 *  File Description :
 *  This is reader header file contains funciton prototypes
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: reader.h,v $
 * Initial version
 *
 ****************************************************************************/
#ifndef READER_H 
#define READER_H

#include <linux/limits.h>
#include <elf.h>


typedef struct ReaderConfig
{
    char            logFileInput[NAME_MAX];
    char            binName[NAME_MAX];
    char            loggerType[MAX_LOGGER_TYPE_STR_LEN];
    char            shmName[NAME_MAX];
    unsigned int    shmSize;
    unsigned int    logFileSize;
    unsigned int    maxNumFiles;
    char            logFileNames[MAX_LOG_FILES_ALLOWED][NAME_MAX];
    unsigned int    curLogFileNum;
}ReaderConfig;

unsigned int reader_init(Char8*);
unsigned int online_reader_init(ReaderConfig *, Char8*);
/* + SPR 17439 */
void lte_cat_data(void);
/* - SPR 17439 */
void lte_tail_data(unsigned int);
void lte_cat_bindata(Char8 *binfilename, Char8 *outfilename);
void lte_binlog_data(ReaderConfig *rdConf, unsigned int timer);
void lte_textlog_data(ReaderConfig *rdConf, unsigned int timer);
void lte_consolelog_data(ReaderConfig *rdConf, unsigned int timer);
void interpet_logs_bin_file(LogParamValues *pParamValues, FILE *fptr);

void readELFFile(char * binName);
void readRODATASection(int fd, Elf32_Ehdr elfHeader, Elf32_Shdr* shTable);
void readShTable(SInt32 fd, Elf32_Ehdr elfHeader, Elf32_Shdr* shTable);
void readELFHeader(int fd,Elf32_Ehdr *elfHeader);


#endif
