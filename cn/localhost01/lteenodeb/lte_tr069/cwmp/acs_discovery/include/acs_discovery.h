/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc . All Rights Reserved.
 *
 ******************************************************************************
 *
 * $$Id: acs_discovery.h $
 *
 *******************************************************************************
 *
 * File Description:This file contains all the standarad library files  to be 
 *                  included structures, MACROS, function prototypes... for 
 *                  acs_discovery.c
 *
 *******************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   Aniruth Oblah    Aug 17, 2012   TR-069 Amendment 4   Initial Creation
 * 
******************************************************************************/

#ifndef _ACS_DISCOVERY_H_
#define _ACS_DISCOVERY_H_

/*******************************************************************************
*
*			STANDARD LIBRARY FILES
*
*******************************************************************************/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <logger.h>
#include <lteTypes.h>
#include <dhcp_client.h>
#include <ip_manager.h>
#include <ipr_cwmp_enb_wrapper.h>
#include <ManagementServerModel.h>

/*******************************************************************************
*
*				MACROS
*
*******************************************************************************/

#define SUCCESS 0
#define FAILURE 1
#define IP_BUFFER 256
#define USER_INPUT 0
#define DHCP_WAY 1

/*********************************************************************************
*			     FUNCTION PROTOTYPES
*
********************************************************************************/

UInt8* ipr_cwmp_acs_discovery (UInt8);

#endif
