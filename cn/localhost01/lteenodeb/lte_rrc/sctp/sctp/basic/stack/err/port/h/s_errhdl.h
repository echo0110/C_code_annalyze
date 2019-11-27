/*****************************************************************************
 *    FILE NAME:
 *       s_errhdl.h
 *
 *    DESCRIPTION:
 *
 *       This file is part of Error Handling Module of SCTP.
 *
 *
 *     DATE            NAME           REFERENCE      REASON
 *    -----------------------------------------------------
 *
 *    02June 2000     Sandeep Mahajan  -       .Original Creation
 *
 *
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/

#ifndef  __FILE_s_errhdl_SEEN__  /* If this file has not been processed, */
#define  __FILE_s_errhdl_SEEN__  /* define it as seen--ref '#endif' below. */


#define SCTP_ERROR_HANDLER( ecode, info_str) \
    do \
    { \
        printf("SCTP	ERROR::Error Code=%u ,:%s\r\n",(sctp_U32bit)ecode, (char *) info_str);  \
    } while(0) \
         
#endif



