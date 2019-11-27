/*****************************************************************************
 *    FILE NAME:     s_errhdl.c
 *
 *    DESCRIPTION:
 *     This file is part of Error Module of SCTP. This contains the Error
 *     Handler function of SCTP. SCTP stack only reports Errors, It is the
 *     responsibility of System Management to take corrective actions.This
 *     will requires some porting efforts.
 *
 *
 *    DATE          NAME           REFERENCE       REASON
 *    -----------------------------------------------------
 *
 *    02June 2000     Sandeep Mahajan  -           .Original Creation
 *
 *
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/

#include <s_cogbl.h>
#include <s_cotyps.h>

/*** Forward Declarations  ***/
void
sctp_error_handler(
    sctp_U32bit       ecode ,
    sctp_U8bit        *info_str );

/***************************************************************************
 *  FUNCTION :
 *    sctp_error_handler
 *
 *  DESCRIPTION:
 *    This function is to handle error reported by SCTP stack entity.
 *    This needs to be ported.
 *
 *  RETURN VALUE:
 *    None
 *
 ****************************************************************************/
void
sctp_error_handler(
    sctp_U32bit       ecode ,
    sctp_U8bit        *info_str )
{
    /* Print Error */
    printf( "SCTP ERROR::Error Code = %u, Diagnostic Information:%s:",
            ecode, ( char * ) info_str );
}





