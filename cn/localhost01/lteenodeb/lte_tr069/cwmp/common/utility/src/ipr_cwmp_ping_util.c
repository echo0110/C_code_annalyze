/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc . All Rights Reserved.
 *
 ******************************************************************************
 *
 * $$Id: ipr_cwmp_ping_util.c $
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
#include <unistd.h>
#include <ipr_cwmp_enb_wrapper.h>

#define PACKETSIZE  64
struct packet
{
    struct icmphdr hdr;
    UInt8 msg[PACKETSIZE-sizeof(struct icmphdr)];
};

UInt32 pid=-1;
/* SPR 23327 FIXED START*/
struct protoent *proto_variable=NULL;
/* SPR 23327 FIXED END*/

UInt32 cnt=1;

/****************************************************************************
 * Function Name  : checksum
 * Inputs         : void *b, UInt32 len
 *                 
 * Outputs        : None
 * Returns        : checksum of buffer passed
 * Description    : This function returns a checksum of the buffer passed
 *                  
 ****************************************************************************/

unsigned short checksum(void *b, UInt32 len)
{
    unsigned short *buf = b;
    UInt32 sum=0;
    unsigned short result;

    for ( sum = 0; len > 1; len -= 2 )
        sum += *buf++;
    if ( len == 1 )
        sum += *(UInt8*)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

/****************************************************************************
 * Function Name  : ping
 * Inputs         : UInt8 *adress
 *                 
 * Outputs        : None
 * Returns        : 0 if server is pingable OR 1 if not
 * Description    : This function is for checking connectivity of a 
 *                  server using icmp echo packets
 *                  
 ****************************************************************************/
/* SPR 23327 FIXED START*/
UInt32 ping(UInt8 *adress)
{
    const UInt32 value=255;
    UInt32 count;
//coverity fix
    int	 sd;
    struct packet packet_variable;
    struct sockaddr_in r_address;
    UInt32 loop_variable;
    struct hostent *hostname;
    struct sockaddr_in addr_ping_val,*addr_variable;

    pid = getpid();
    proto_variable = getprotobyname("ICMP");
    hostname = gethostbyname((const char *)adress);
    bzero(&addr_ping_val, sizeof(addr_ping_val));
    addr_ping_val.sin_family = hostname->h_addrtype;
    addr_ping_val.sin_port = 0;
    addr_ping_val.sin_addr.s_addr = *(long*)hostname->h_addr;

    addr_variable = &addr_ping_val;

    sd = socket(PF_INET, SOCK_RAW, proto_variable->p_proto);
    if ( sd < 0 )
    {
        perror("socket");
        return 1;
    }
    if ( setsockopt(sd, SOL_IP, IP_TTL, &value, sizeof(value)) != 0)
    {
        perror("Set TTL option");
	//coverity fix
	close(sd);
	return 1;
    }
    if ( fcntl(sd, F_SETFL, O_NONBLOCK) != 0 )
    {
	    perror("Request nonblocking I/O");
	    //coverity fix
	    close(sd);
        return 1;
    }

    for (loop_variable=0;loop_variable < 10; loop_variable++)
    {

        UInt32 len=sizeof(r_address);

        if ( recvfrom(sd, &packet_variable, sizeof(packet_variable), 0, (struct sockaddr*)&r_address, &len) > 0 )
        {
		//coverity fix
		close(sd);
		return 0;
        }

        bzero(&packet_variable, sizeof(packet_variable));
        packet_variable.hdr.type = ICMP_ECHO;
        packet_variable.hdr.un.echo.id = pid;
        for ( count = 0; count < sizeof(packet_variable.msg)-1; count++ )
        packet_variable.msg[count] = count+'0';
        packet_variable.msg[count] = 0;
        packet_variable.hdr.un.echo.sequence = cnt++;
        packet_variable.hdr.checksum = checksum(&packet_variable, sizeof(packet_variable));
        if ( TR069_SENDTO(sd, &packet_variable, sizeof(packet_variable), 0, (struct sockaddr*)addr_variable, sizeof(*addr_variable)) <= 0 )
            perror("sendto");

        TR069_USLEEP(300000);

    }

    //coverity fix
    close(sd);
    return 1;
}
/* SPR 23327 FIXED START*/

