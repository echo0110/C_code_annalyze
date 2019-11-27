/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc . All Rights Reserved.
 *
 ******************************************************************************
 *
 * $$Id: dns_client.c $
 *
 *******************************************************************************
 *
 * File Description: This file contains the function for dns_client 
 *
 *******************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   Aniruth Oblah    Aug 17, 2012   TR-069 Amendment 4   Initial Creation
 *
 ******************************************************************************/

#include <dns_client.h>

typedef int SInt32;

/* IHEMS FQDN FIXED START */
#define CONFIG_PARAM_LEN 256
extern UInt8 ABSPath[CONFIG_PARAM_LEN];

/* IHEMS FQDN FIXED END */
/****************************************************************************
 * Function Name  : ipr_cwmp_dns_client
 * Inputs         : const UInt8 *host 
 *                 
 * Outputs        : None
 * Returns        : number of IP addresses 
 * Description    : This function resolves the hostname and gives a list 
 *                  of IP address
 *                  
 ****************************************************************************/

    FILE* fp = NULL;

SInt32 ipr_cwmp_dns_client (
		const UInt8 *host  //domain name
		)
{
	TRACE ( 2 , "DNS_CLIENT", "Entering DNS Client --------- ");
	struct addrinfo hints, *res = NULL;
    /*  coverity 86638 fix start */
    struct addrinfo *temp;
    /*  coverity 86638 fix end */
	SInt32 errcode;
	UInt8 addrstr[100];
	void* p_host_addr = PNULL;
	UInt32 count = 0;

    /* SPR 16468 Fix Start */
    char * host_domain = NULL;
    char * host_port = NULL;

    UInt8 temp_domain[PERSISTENT_BUFFER_SIZE] = {'\0'};  // to store domain name
    /* Coverity 97089 + */
    strncpy((char*)temp_domain,(char*)host,PERSISTENT_BUFFER_SIZE);
    temp_domain[PERSISTENT_BUFFER_SIZE - 1] = '\0';
    /* Coverity 97089 - */
    host_domain = strtok((char*)temp_domain,":");
    TRACE ( 1 , "DNS_CLIENT", "host_domain is =%s --------- ",host_domain);
    host_port = strtok(PNULL,":");
    TRACE ( 1 , "DNS_CLIENT", "host_port is =%s--------- ",host_port);
    /* SPR 16468 Fix End */
    /* IHEMS FQDN FIXED START */
    UInt8 ABSPath1[CONFIG_PARAM_LEN] = {'\0'};
    strcpy((char*)ABSPath1,(char *)ABSPath);
    TRACE ( LOG_CRITICAL , "IP_MANAGER" , "ABSPath before concat %s ",ABSPath1 );
    strncat((char *)ABSPath1 , "/iplist.txt" , strlen("/iplist.txt"));
    TRACE ( LOG_CRITICAL , "IP_MANAGER" , "ABSPath after concat %s ",ABSPath1 );
    fp = fopen((char *)ABSPath1, "w+");



   // fp = ENB_FOPEN("iplist.txt", "w+");

    /* IHEMS FQDN FIXED END */
  	ENB_MEMSET (&hints, 0, sizeof (hints));
  	hints.ai_family = PF_UNSPEC;
  	hints.ai_socktype = SOCK_STREAM;
 	hints.ai_flags |= AI_CANONNAME;
    /* SPR 16468 Fix Start */
  	errcode = ENB_GETADDRINFO((char*)host_domain, NULL, &hints, &res);
    /* SPR 16468 Fix End */
  	if (errcode != 0)
   	{
		fflush(stdout);
      	return -1;
   	}
 /*FQDN dBL FREE FIX START */
    temp = res;
 /*FQDN dBL FREE FIX END */
  	while (res)
    {
        ENB_INET_NTOP(res->ai_family, res->ai_addr->sa_data, (char*)addrstr, 100);
      	switch (res->ai_family)
        {
        	case AF_INET:
          		p_host_addr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
          		break;
        		
			case AF_INET6:
          		p_host_addr = &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr;
          		break;
       	}
//coverity fix
        if(NULL != p_host_addr)
      	ENB_INET_NTOP(res->ai_family, p_host_addr, (char*)addrstr, 100);
        /* SPR 16468 Fix Start */
        ENB_FPRINTF(fp, "%s:%s\n", addrstr,host_port);
        /* SPR 16468 Fix End */
        /*  coverity 86638 fix start */

	/*FQDN dBL FREE FIX START */

        /* code Remove*/
	/*FQDN dBL FREE FIX END */
        res = res->ai_next;
	/*FQDN dBL FREE FIX START */
        //freeaddrinfo(temp);
	/*FQDN dBL FREE FIX END */
        /*  coverity 86638 fix end */
		count += 1; 
    }
	/*FQDN dBL FREE FIX START */
    /* SPR 22314 Start */
    TR069_FREE_ADDRINFO(temp);
    /* SPR 22314 End */
	/*FQDN dBL FREE FIX END */
	ENB_FCLOSE(fp);
  	return count;
}
