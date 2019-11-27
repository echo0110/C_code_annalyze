/*****************************************************************************
 *    FILE NAME:
 *        sig_dbg.c
 *
 *    DESCRIPTION:
 *       This file has porting function for logging Debug information.
 *
 *    DATE           NAME          REFERENCE        REASON
 *    ----------------------------------------------------
 *    14Sept02    Sandeep Mahajan     -            .Original Creation
 *    13Sept11    Sandeep Singh    SPR-21022       Vxworks-6.8 Porting Changes
 *     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/

#include <sig_os.h>
/* Changes for SPR-21022 Starts here */
#ifdef VXWORKS_6_9_SUPPORT
#include <stdio.h>
#endif
/* Changes for SPR-21022 Ends here */

#define SIGTRAN_LOG_FILE "sig_log.txt"


FILE *sigtran_fp = NULL;

/******************************************************************************
**  FUNCTION : sigtran_dbg_log_trace
**
**  DESCRIPTION: This function is used for logging. This can either log to
**                standard output (using prinft) or it can log to a file
**                File is opended in append mode in case File pointer is
**                NULL
**
**  RETURN VALUE: Void
******************************************************************************/
void sigtran_dbg_log_trace( char *szFormat, ... )
{
     va_list   arg_list;
#ifdef SIGTRAN_LOG_TO_FILE

    if( sigtran_fp == NULL )
    {
        if( ( sigtran_fp = fopen( SIGTRAN_LOG_FILE, "a" ) ) == NULL )
        {
            return;
        }
    }

    va_start( arg_list, szFormat );
    vfprintf( sigtran_fp, szFormat, arg_list );
    va_end( arg_list );
#else

    /* Copy the formatted string into trace_str */
    va_start( arg_list, szFormat );
    vprintf( szFormat, arg_list );
    va_end( arg_list );
#endif

}















































