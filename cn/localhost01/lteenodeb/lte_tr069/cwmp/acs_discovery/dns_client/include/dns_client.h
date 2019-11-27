/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc . All Rights Reserved.
 *
 ******************************************************************************
 *
 * $$Id: dns_client.h $
 *
 *******************************************************************************
 *
 * File Description: This file contains all the standarad library files
 *                   to be included, structures, MACROS, function
 *                   prototypes... for dns_client.c
 *
 *******************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   Aniruth Oblah    Aug 23, 2012   TR-069 Amendment 4   Initial Creation 
 * 
 ******************************************************************************/

#ifndef _DNS_CLIENT_H_
#define _DNS_CLIENT_H_

/*******************************************************************************
*
*			STANDARD LIBRARY FILES
*
*******************************************************************************/

#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<errno.h>
#include	<time.h>
#include	<netdb.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<arpa/inet.h>
#include    <logger.h>
//cov fix 
#include    <lteTypes.h>
#include    <ipr_cwmp_enb_wrapper.h>



/*******************************************************************************
*
*				MACROS
*
*******************************************************************************/

#define SUCCESS 0
#define FAILURE 1
/* SPR 16468 Fix Start */
#define PERSISTENT_BUFFER_SIZE 256
/* SPR 16468 Fix End */

/*********************************************************************************
*			     FUNCTION PROTOTYPES
*
********************************************************************************/

int ipr_cwmp_dns_client (const UInt8*);

#endif
