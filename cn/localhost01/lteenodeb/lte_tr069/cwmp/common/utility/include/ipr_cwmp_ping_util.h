/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc . All Rights Reserved.
 *
 ******************************************************************************
 *
 * $$Id: ipr_cwmp_ping_util.h $
 *
 *******************************************************************************
 *
 * File Description: This file contains functions to check connectivity of a server. 
 *      
 *
 *******************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   2013/04/08 Anant K Sharma  Initial Version 1.0
 * 
 ******************************************************************************/

#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <strings.h>
#include <lteTypes.h>


#define PACKETSIZE  64
struct packet
{
    struct icmphdr hdr;
    UInt8 msg[PACKETSIZE-sizeof(struct icmphdr)];
};

unsigned short checksum(void *b, UInt32 len);
UInt32 ping(UInt8 *adress);
