/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc . All Rights Reserved.
 *
 ******************************************************************************
 *
 * $$Id: ip_manager.h $
 *
 *******************************************************************************
 *
 * File Description: This file contains all the standarad library files 
 *                   to be included, structures, MACROS, function   
 *                   prototypes... for ip_manager.c 
 *  
 *******************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   Aniruth Oblah    Aug 17, 2012   TR-069 Amendment 4   Initial Creation
 *   Vinod Vyas       Oct 2013       Ipv6 Related Changes
 *
 ******************************************************************************/

#ifndef _IP_MANAGER_H_
#define _IP_MANAGER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>
#include <limits.h>
#include <logger.h>
#include <lteTypes.h>
#include <dns_client.h>
#include <ManagementServerModel.h>
#include <ipr_cwmp_enb_wrapper.h>

#define SUCCESS 0
#define FAILURE 1
#define PERSISTENT_BUFFER_SIZE 256
#define PARTIAL_URL 100

typedef struct ip_address
{
        UInt8 *ip;
        struct ip_address * next;
}ip_address;

UInt8* ipr_cwmp_ip_manager(UInt8*, UInt8 );
UInt8* ipr_cwmp_find_host(UInt8*, UInt8*, UInt8*, UInt8*);
void ipr_cwmp_swap_values(int);
UInt32 ipr_cwmp_iprand(UInt32, UInt32);
void ipr_cwmp_srand(UInt32);
UInt32 ipr_cwmp_lengthOfU(UInt8*);

#endif
