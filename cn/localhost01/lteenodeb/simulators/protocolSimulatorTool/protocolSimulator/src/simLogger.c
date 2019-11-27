/**************************************************************************************
*
*  FILE NAME  : simLogger.c
*
*  DESCRIPTION: Simulator logger engine implementation
*
*
*   DATE    NAME    REFERENCE   REASON
*
*  Copyright 2009, Aricent Inc. Ukraine
*
*************************************************************************************/

/****************************************************************************
 * Includes
 ****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/timeb.h>
#include <stdarg.h>

#include <pthread.h>
#include <semaphore.h>

#include "simLogger.h"
#include "testStatistic.h"

/****************************************************************************
 * Local definitions
 ****************************************************************************/
#define SIM_LOGGER_MAXFILENAME 256
#define SIM_LOGGER_MAXFILESIZE (2*1024*1024)

/****************************************************************************
 * Local types
 ****************************************************************************/
typedef struct LogDetailT {
    FILE*   fd;
    UChar8  filename[SIM_LOGGER_MAXFILENAME];
    UInt32  level;
    sem_t   logLock;
} LogDetail;


/****************************************************************************
 * Local functions definitions
 ****************************************************************************/
static void loggerMakeLogFileName( UChar8* fileNameBuf );

/****************************************************************************
 * Local Variables
 ****************************************************************************/

static LogDetail simModuleLogDetail_g = { PNULL, "", SIM_LOG_NONE };
LogDetail* simModuleLogDetail_p = PNULL;

/****************************************************************************
 * Functions implementation
 ****************************************************************************/

/****************************************************************************
 * Function Name  : loggerInit
 * Inputs         : void *fileName - filename for log, PNULL if use CONSOLE_DEV
 * Outputs        : None.
 * Returns        : None.
 * Description    : Inits log context
 ****************************************************************************/
void loggerInit(void *fileName)
{
    if ( PNULL == simModuleLogDetail_p )
    {
        simModuleLogDetail_p = &simModuleLogDetail_g;
        simModuleLogDetail_p->fd = PNULL;
        simModuleLogDetail_p->level = SIM_LOG_NONE;

        sem_init(&simModuleLogDetail_p->logLock, 0, 1);

        loggerRedirectLog( fileName );
    }
}

/****************************************************************************
 * Function Name  : loggerRedirectLog
 * Inputs         : void *fileName - filename for log, PNULL if use CONSOLE_DEV
 * Outputs        : None.
 * Returns        : None.
 * Description    : redirect log output
 ****************************************************************************/
void loggerRedirectLog(void *fileName)
{
    FILE *fd = PNULL;
    UChar8 fullName[SIM_LOGGER_MAXFILENAME+64];

    if ( PNULL != simModuleLogDetail_p )
    {
        /*redirect log to other file*/
        if ( PNULL != fileName )
        {
            strcpy(simModuleLogDetail_p->filename,(UChar8*)fileName);
            strcpy(fullName, (UChar8*)fileName);

            loggerMakeLogFileName( fullName );

            if ( PNULL != simModuleLogDetail_p->fd )
            {
                fd = freopen( fullName, "w", simModuleLogDetail_p->fd );
            }
            else
            {
                fd = fopen( fullName, "w" );
            }
            if ( PNULL != fd )
            {
                simModuleLogDetail_p->fd = fd;
            }
            else
            {
                SIM_LOG( SIM_LOG_WARNING, "Unable to open file. %s", fullName );
            }
        }
        else
        {
            if (PNULL != simModuleLogDetail_p->fd )
            {
                fclose( simModuleLogDetail_p->fd );
                simModuleLogDetail_p->fd = PNULL;
            }
        }
    }
}

/****************************************************************************
 * Function Name  : loggerSetLogLevel
 * Inputs         : UInt32 level
 * Outputs        : None.
 * Returns        : None.
 * Description    : Sets level
 ****************************************************************************/
void loggerSetLogLevel(UInt32 level)
{
    if ( PNULL != simModuleLogDetail_p)
    {
        simModuleLogDetail_p->level = level;
    }
}

/****************************************************************************
 * Function Name  : loggerGetLogLevel
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : log level
 * Description    : Returns log level
 ****************************************************************************/
UInt32 loggerGetLogLevel(void)
{
    if ( PNULL != simModuleLogDetail_p )
    {
        return simModuleLogDetail_p->level;
    }
    else
    {
        return SIM_LOG_NONE;
    }
}

/****************************************************************************
 * Function Name  : loggerCleanLog
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Cleans log context
 ****************************************************************************/
void loggerCleanLog(void)
{
    /*do not clean context to preserve filename*/
    if ( PNULL != simModuleLogDetail_p )
    {
        if ( PNULL != simModuleLogDetail_p->fd )
        {
            fclose( simModuleLogDetail_p->fd );
        }
        sem_destroy(&simModuleLogDetail_p->logLock);
    }
    simModuleLogDetail_p = PNULL;
}

/****************************************************************************
 * Function Name  : loggerLog
 * Inputs         : level - Log level.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Puts log record to log storage.
 ****************************************************************************/

void loggerLog(UInt16 level, const UChar8 *format, ... )
{
    va_list         va_args;
    struct tm*      tmp = PNULL;
    struct timeb    systime;
    UChar8          prefix[0xFF]={0};

    va_start( va_args, format );

    if(SIM_LOG_WARNING==level)
    {
        statisticUpdate(SIM_STAT_LOG_WARNINGS,1);
        strcpy(prefix,"WARNING:");
    }
    if(SIM_LOG_CRITICAL==level)
    {
        statisticUpdate(SIM_STAT_LOG_ERRORS,1);
        strcpy(prefix,"CRITICAL:");
    }

    if ( PNULL == simModuleLogDetail_p )
    {
        printf("%s",prefix);
        vprintf( format, va_args );
        printf ( "\n" );
        fflush(stdout);
    }
    else
    {
        if ( simModuleLogDetail_p->level < level )
        {
            return;
        }

        sem_wait( &simModuleLogDetail_p->logLock );

        ftime( &systime );
        tmp = localtime(&systime.time);

        if ( PNULL == simModuleLogDetail_p->fd )
        {
            printf("[%02d-%02d-%02d-%06d] : ", tmp->tm_hour, tmp->tm_min, tmp->tm_sec, (Int)systime.millitm );
            printf("%s",prefix);
            vprintf( format, va_args ); /*if no file created*/
            printf ( "\n" );
            fflush(stdout);
        }
        else
        {

            fprintf( simModuleLogDetail_p->fd, "[%02d-%02d-%02d-%06d] : ", tmp->tm_hour, tmp->tm_min, tmp->tm_sec, (Int)systime.millitm );
            fprintf(simModuleLogDetail_p->fd,"%s",prefix);

            vfprintf( simModuleLogDetail_p->fd, format, va_args );
            fprintf ( simModuleLogDetail_p->fd, "\n" );

            fflush( simModuleLogDetail_p->fd );

            if( SIM_LOGGER_MAXFILESIZE < ftell(simModuleLogDetail_p->fd) )
            { /*reopen*/
                loggerRedirectLog( simModuleLogDetail_p->filename );
            }
        }

        sem_post( &simModuleLogDetail_p->logLock );
    }
    va_end( va_args );
}

/************************************************************************************
 Local functions implemenation
 ************************************************************************************/
void loggerMakeLogFileName( UChar8* fileNameBuf )
{
    struct tm*      tmp;
    struct timeb    systime;
    UChar8          buf[SIM_LOGGER_MAXFILENAME];

    if (PNULL != simModuleLogDetail_p)
    {
        ftime( &systime );
        tmp = localtime(&systime.time);

        sprintf(buf, "_%02d.%02d.%04d.%02d-%02d-%02d-%06d",
            tmp->tm_mday, tmp->tm_mon+1, tmp->tm_year+1900, tmp->tm_hour, tmp->tm_min, tmp->tm_sec, (Int)systime.millitm );

        strcat(fileNameBuf, buf );
        strcat(fileNameBuf, ".log");
    }
}
