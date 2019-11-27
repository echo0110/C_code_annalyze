/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc . All Rights Reserved.
 *
 ******************************************************************************
 *
 * $$Id: dhcp_client.h $
 *
 *******************************************************************************
 *
 * File Description: This file contains all the standarad library files
 *                   to be included, structures, MACROS, function
 *                     prototypes... for dhcp_client.c
 *
 *******************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   Aniruth Oblah    Aug 28, 2012   TR-069 Amendment 4   Initial Creation
 *      
 ******************************************************************************/

#ifndef _DHCP_CLIENT_H_
#define _DHCP_CLIENT_H_


/*******************************************************************************
*
*			STANDARD LIBRARY FILES
*
*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <time.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/ethernet.h>
#include <string.h>
#include <errno.h>

#include <logger.h>
#include <ipr_cwmp_enb_wrapper.h>
#include <lteTypes.h>
#include <ManagementServerModel.h>

/*******************************************************************************
*
*				MACROS
*
*******************************************************************************/

/* DHCP Message Offsets */

#define F_OP         0
#define F_HTYPE      1
#define F_HLEN       2
#define F_HOPS       3
#define F_XID        4
#define F_SECS       8
#define F_FLAGS      10
#define F_CIADDR     12
#define F_YIADDR     16
#define F_SIADDR     20
#define F_GIADDR     24
#define F_CHADDR     28
#define F_SNAME      44
#define F_FILE       108
#define F_OPTIONS    236

/* standard options */
#define F_COOKIE F_OPTIONS
#define F_INFORM 240
#define F_WPAD   243
#define F_ML_H   245
#define F_ML_D   247

#define F_CUSTOM 249
#define IPR_CWMP_MSG_LEN 2048

/*********************************************************************************
*			     FUNCTION PROTOTYPES
*
********************************************************************************/

UInt8* ipr_cwmp_dhcp_client(void);

#endif
