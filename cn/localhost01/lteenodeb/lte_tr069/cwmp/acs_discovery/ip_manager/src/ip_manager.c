/********************************************************************************
*
*		FILE NAME   : ip_select.c
*
*		DESCRIPTION : This file contains the function for ip_select 
*			      
*
*	REVISION HISTORY:
*
*	NAME		 DATE		REFERENCE	     REASON
*_______________________________________________________________________________
*
*	Aniruth Oblah    Aug 17, 2012	TR-069 Amendment 4   Initial Creation	
*       Vinod Vyas       Oct 2013       Ipv6 Related Changes 
*   Swetank Srivastava Nov 2014,	SPR 13755
*
*	All Rights Reserved. Copyright @ Aricent Group, 2012.
*
********************************************************************************/

/********************************************************************************
*
*			HEADER FILE INCLUSION
*
********************************************************************************/

#include <ip_manager.h>
#include <ipr_cwmp_enb_wrapper.h>
/* For storing IP from ACS URL */
static UInt8 g_ipr_cwmp_persistent_ip [PERSISTENT_BUFFER_SIZE] = {0};

/* Used for rand selection of ip from IP List */
static UInt64 g_ipr_cwmp_next = 1;

/* Holding List of Ip address of ACS  */
static UInt8** g_ipr_cwmp_L_IP_list = PNULL;

#define CONFIG_PARAM_LEN 256

/* SPR 17268 FIXED START */
extern UInt8 ABSPath[CONFIG_PARAM_LEN];
/* SPR 17268 FIXED END */

typedef int SInt32; 

/********************************************************************************
*
*		FUNCTION NAME : ipr_cwmp_ip_manager()		
*
*		DESCRIPTION   : This function calls the DNSCLient which resloves
*				the ACS URL and gives a set of IP addresses.
*				this function selects one IP from that.
*
*		RETURNS	      : IP of the ACS 
*
********************************************************************************/
	
	
UInt8 * ipr_cwmp_ip_manager (
	UInt8 * ACS_URL, 
	UInt8 recent_connection_status 
	)
	
{
	FILE *fp = PNULL;
	UInt8 domain_or_ip = '\0'; // Can have two values d for domain, i for IP
	TRACE ( 2 , "IP_MANAGER", "Entering IP Manager --------- ");

	SInt32 iplist_index = 0;
	static SInt32 list_size = 0;  
	UInt8 domain[PERSISTENT_BUFFER_SIZE] = {'\0'};  // to store domain name
	UInt8 url_from_domain[PERSISTENT_BUFFER_SIZE] = {'\0'};  // to store domain name
	char  Sec_type[PERSISTENT_BUFFER_SIZE] = {'\0'}; 
	char  add_url_info[PERSISTENT_BUFFER_SIZE] = {'\0'};
	// s here means, the last connection to ACS was successful
	if( 's' == recent_connection_status )
	{
		TRACE ( 3 , "IP_MANAGER", "Returning URL to ACS_Discovery --------- ");
		return g_ipr_cwmp_persistent_ip;	
	}	

	// n here means the CPE is going to connect to ACS for the very first time

	if('n' == recent_connection_status || 'e' == recent_connection_status)
	{
		//start timer for DNS 
		TRACE ( 1 , "IP_MANAGER", "Entering IP Manager for the first time --------- ");
		if ( 'n'== recent_connection_status )
		{
			/* IPV6 CHANGES */
			UInt8 * temp = ipr_cwmp_find_host(ACS_URL,(UInt8*)&domain_or_ip,(UInt8 *)&Sec_type,(UInt8*)&add_url_info); 
			if(NULL == temp)
			{
				return NULL;
			}
            /* SPR 16468 Fix Start */
            char * temp_domain = NULL;
			tr_strncpy((char*)domain, (char*)temp);
            temp_domain = strtok((char *)temp,":");
	    /*Coverity 111402 Fix Start*/
	    if(NULL != temp_domain)
            ipr_cwmp_set_acs_host_name((UInt8 *)temp_domain);
	    /*Coverity 111402 Fix End*/
            /* SPR 16468 Fix End */

			/*Free the memory allocated for hostname */
			DEALLOCATE(temp);
		}
		else
		{	
			/* IPV6 CHANGES */
			UInt8 * temp =  ipr_cwmp_find_host(ACS_URL,(UInt8*)&domain_or_ip,(UInt8 *)Sec_type,(UInt8*)add_url_info); 
			if(NULL == temp)
			{
				return NULL;
			}
			TRACE ( 3 , "IP_MANAGER", "Hostname is %s ",  temp);
			tr_strncpy((char*)domain, (char*)ipr_cwmp_get_acs_host_name());
			/*SPR 13755 Fix: code removed*/
			/*Free the memory allocated for hostname */
			DEALLOCATE(temp);
		}
		TRACE ( 3 , "IP_MANAGER", "Domain name is %s ", domain);
		if ( 'i' == domain_or_ip )
		{

			TRACE ( 3 , "IP_MANAGER", "URL contains IP --------- ");
			/*Coverity 40941 Fix Start*/
			 if(PERSISTENT_BUFFER_SIZE >=  ENB_STRLEN((char *)ACS_URL))
			 ENB_STRNCPY ((char*)g_ipr_cwmp_persistent_ip, (char*)ACS_URL , ENB_STRLEN((char *)ACS_URL));
			/*Coverity 40941 Fix End*/
			return g_ipr_cwmp_persistent_ip;
		}	
		if( 'd' == domain_or_ip )
		{
			TRACE ( 3 , "IP_MANAGER", "URL contains domain name  --------- ");
			list_size = ipr_cwmp_dns_client(domain);
			if(g_ipr_cwmp_L_IP_list != PNULL)
			{
				DEALLOCATE(g_ipr_cwmp_L_IP_list);
			}
			g_ipr_cwmp_L_IP_list = (UInt8 **)ALLOCATE (sizeof(UInt8 *)  * list_size);
			if ( -1 == list_size )
			{
				TRACE ( 1 , "IP_MANAGER", "DNS FAILED --------- ");
				return PNULL;	
			}
			if( 0 == list_size )
			{
				TRACE ( 1 , "IP_MANAGER", "DNS Failed to give an ip--------- ");
				return PNULL;
			}
			if( 0 < list_size )
			{
				//create a list to store the IP addresses

                /* SPR 17268 FIXED START */

                /* IHEMS FQDN FIXED START */
                UInt8 ABSPath1[CONFIG_PARAM_LEN] = {'\0'};
                strcpy((char*)ABSPath1,(char *)ABSPath);
                TRACE ( LOG_CRITICAL , "IP_MANAGER" , "ABSPath before concat %s ",ABSPath1 );
                strncat((char *)ABSPath1 , "/iplist.txt" , strlen("/iplist.txt"));
                TRACE ( LOG_CRITICAL , "IP_MANAGER" , "ABSPath after concat %s ",ABSPath1 );
                fp = fopen((char *)ABSPath1, "r");

                /* IHEMS FQDN FIXED START */

                /* SPR 17268 FIXED END */

				//fp = fopen("iplist.txt", "r");
				if(NULL == fp)
				{
					TRACE ( 1 , "IP_MANAGER", " File open failure --------- ");
					return PNULL;
				}
				for ( iplist_index = 0; iplist_index < list_size ; iplist_index++ )
				{
					g_ipr_cwmp_L_IP_list[iplist_index] = (UInt8 *)ALLOCATE (sizeof(UInt8) * PARTIAL_URL ); 
				}
				//copy the ip addresses from the file to the local ip list
				for(iplist_index = 0; iplist_index < list_size; iplist_index ++)
				{
					if(0 == ENB_FSCANF(fp,"%255s",g_ipr_cwmp_L_IP_list[iplist_index]))
					{
						TRACE ( 1 , "IP_MANAGER", " Fscanf early read error  --------- ");
						fclose(fp);
						return PNULL;
					}

				}
				ipr_cwmp_swap_values(list_size);
				tr_strncpy((char*)g_ipr_cwmp_persistent_ip, (char*)(g_ipr_cwmp_L_IP_list[list_size - 1 ]));
				DEALLOCATE ( g_ipr_cwmp_L_IP_list[list_size - 1] );
				g_ipr_cwmp_L_IP_list[list_size - 1] = PNULL;
				list_size = list_size - 1; 
				fclose(fp);
				/* IPV6 CHANGES */
				snprintf((char *)url_from_domain,PERSISTENT_BUFFER_SIZE,"%s//%s/%s",Sec_type,g_ipr_cwmp_persistent_ip,add_url_info);
				ENB_MEMSET ( g_ipr_cwmp_persistent_ip, 0, PERSISTENT_BUFFER_SIZE );
				tr_strncpy((char*)g_ipr_cwmp_persistent_ip, (char*)url_from_domain);
				return g_ipr_cwmp_persistent_ip;
			}	
		}
	}
	// here u means that, the IP used last time to connect to ACS was not successful and it requires a different IP
	if ( 'u' == recent_connection_status )
	{
		if (PNULL == ACS_URL)
		{
			TRACE ( 1 , "IP_MANAGER", "requesting Alternate IP--------- ");
			if (list_size > 0)
			{
				ipr_cwmp_swap_values(list_size);
				tr_strncpy((char*)g_ipr_cwmp_persistent_ip, (char*)(g_ipr_cwmp_L_IP_list[list_size - 1 ]));
				DEALLOCATE( g_ipr_cwmp_L_IP_list[list_size - 1] );
				g_ipr_cwmp_L_IP_list[list_size - 1] = PNULL;
				list_size = list_size - 1;
				/* IPV6 CHANGES */
				snprintf((char *)url_from_domain,PERSISTENT_BUFFER_SIZE,"%s//%s/%s",Sec_type,g_ipr_cwmp_persistent_ip,add_url_info);
				ENB_MEMSET ( g_ipr_cwmp_persistent_ip, 0, PERSISTENT_BUFFER_SIZE );
				tr_strncpy((char*)g_ipr_cwmp_persistent_ip, (char*)url_from_domain);
				return g_ipr_cwmp_persistent_ip;
			}	
			else if(list_size == 0)
			{
				TRACE ( 1 , "IP_MANAGER", " IP list exhausted--------- ");
				return PNULL;
			}
		}	
		if (PNULL != ACS_URL)
		{
			TRACE ( 1 , "IP_MANAGER", " URL obtained via DHCP after list exhausted--------- ");
			UInt8 * tr_temp = ipr_cwmp_find_host(ACS_URL, &domain_or_ip,(UInt8*)Sec_type,(UInt8*)add_url_info); 
			if(NULL == tr_temp)
			{
				return NULL;
			}
			tr_strncpy((char*)domain, (char*)tr_temp);
			/*Free the memory allocated for hostname */
			DEALLOCATE(tr_temp);
			if ('i' == domain_or_ip)
			{
				if(list_size > 0)
				{
					ipr_cwmp_swap_values(list_size);
					tr_strncpy((char*)g_ipr_cwmp_persistent_ip, (char*)(g_ipr_cwmp_L_IP_list[list_size - 1]));
					DEALLOCATE(g_ipr_cwmp_L_IP_list[list_size - 1]);
					g_ipr_cwmp_L_IP_list[list_size - 1] = PNULL;
					/* IPV6 CHANGES */
					snprintf((char *)url_from_domain,PERSISTENT_BUFFER_SIZE,"%s//%s/%s",Sec_type,g_ipr_cwmp_persistent_ip,add_url_info);
					ENB_MEMSET ( g_ipr_cwmp_persistent_ip, 0, PERSISTENT_BUFFER_SIZE );
					tr_strncpy((char*)g_ipr_cwmp_persistent_ip, (char*)url_from_domain);

					list_size = list_size - 1;
					return g_ipr_cwmp_persistent_ip;
				}
				else if(list_size == 0)
				{
					TRACE ( 1 , "IP_MANAGER", " IP list exhausted--------- ");
					return PNULL;
				}
			}	
			list_size = ipr_cwmp_dns_client(domain);
			g_ipr_cwmp_L_IP_list = (UInt8 **)ALLOCATE (sizeof(UInt8 *)  * list_size);
			if ( -1 == list_size )
			{	
				TRACE ( 1 , "IP_MANAGER", "DNS FAILED --------- ");
				return PNULL;	
			}
			if ( 0 == list_size )
			{	
				TRACE ( 1 , "IP_MANAGER", "DNS FAILED --------- ");
				return PNULL;	
			}
			if ( 0 < list_size )
			{
				//create a list to store the IP addresses

                /* SPR 17268 FIXED START */

                /* IHEMS FQDN FIXED START */
                UInt8 ABSPath2[CONFIG_PARAM_LEN] = {'\0'};
                strcpy((char*)ABSPath2,(char *)ABSPath);
                TRACE ( LOG_CRITICAL , "IP_MANAGER" , "ABSPath before concat %s ",ABSPath2 );
                strncat((char *)ABSPath2 , "/iplist.txt" , strlen("/iplist.txt"));
                TRACE ( LOG_CRITICAL , "IP_MANAGER" , "ABSPath after concat %s ",ABSPath2 );
                fp = fopen((char *)ABSPath2, "r");

                /* IHEMS FQDN FIXED START */

                /* SPR 17268 FIXED END */

            //	fp = fopen("iplist.txt", "r");
				if(NULL == fp)
				{
					TRACE ( 1 , "IP_MANAGER", " File open failure --------- ");
					return PNULL;
				}
				for ( iplist_index = 0; iplist_index < list_size ; iplist_index++ )
				{
					g_ipr_cwmp_L_IP_list[iplist_index] = (UInt8 *)ALLOCATE (sizeof(UInt8) * PARTIAL_URL );
				}
				//copy the ip addresses from the file to the local ip list
				for(iplist_index = 0; iplist_index < list_size; iplist_index ++)
				{
					if(0 == ENB_FSCANF(fp,"%255s",g_ipr_cwmp_L_IP_list[iplist_index]))
					{
						TRACE ( 1 , "IP_MANAGER", " Fscanf early read error  --------- ");
						fclose(fp);
						return PNULL;
					}

				}
				ipr_cwmp_swap_values(list_size);
				tr_strncpy((char*)g_ipr_cwmp_persistent_ip, (char*)(g_ipr_cwmp_L_IP_list[list_size - 1]));
				DEALLOCATE ( g_ipr_cwmp_L_IP_list[list_size - 1] );
				g_ipr_cwmp_L_IP_list[list_size - 1] = PNULL;
				list_size = list_size - 1;
				/* IPV6 CHANGES */
				snprintf((char *)url_from_domain,PERSISTENT_BUFFER_SIZE,"%s//%s/%s",Sec_type,g_ipr_cwmp_persistent_ip,add_url_info);
				ENB_MEMSET ( g_ipr_cwmp_persistent_ip, 0, PERSISTENT_BUFFER_SIZE );
				tr_strncpy((char*)g_ipr_cwmp_persistent_ip, (char*)url_from_domain);
				fclose(fp);
			}		
		}
	}
	return g_ipr_cwmp_persistent_ip;
}
	
	
	
/********************************************************************************************************************************
*
*		FUNCTION NAME : ipr_cwmp_find_host()		
*
*		DESCRIPTION   : This function extracts the domain name or ip from the URL and sends it back
*
*		RETURNS	      : domain name/ IP
*
******************************************************************************************************************************/
	
UInt8* ipr_cwmp_find_host(UInt8* URL, UInt8* domain_or_ip, UInt8* Sec_type, UInt8* add_url_info)
{

	struct addrinfo hint, *res = PNULL;
	static UInt8 TempUrl[PERSISTENT_BUFFER_SIZE] = {0};

	char Domain_name_or_ip[PERSISTENT_BUFFER_SIZE] = {0};
	UInt8* hostname=(UInt8 *)ALLOCATE ((sizeof(UInt8) * PERSISTENT_BUFFER_SIZE) );
	if(PNULL == hostname)
	{
		TRACE ( 2 , "IP_MANAGER", " Memory allcoated is NULL");
		return PNULL;
	}

	char *temp_str= PNULL;
	char *IP_Value= PNULL;
	char *sec = PNULL;
	char *add_info = PNULL;
	
    /*SPR 19393 FIXED START */
    /* Code remove */
    /*SPR 19393 FIXED START */
	memset(&hint, 0, sizeof hint);
	hint.ai_family = PF_UNSPEC;
	hint.ai_flags = AI_NUMERICHOST;
	/* IPV6 CHANGES START */
    

	/* SPR 19836 FIXED START */
	char *add_info_t = PNULL;
	UInt8 str_len = 0;
	add_info = ALLOCATE(PERSISTENT_BUFFER_SIZE);
        /*COVERITY 110593 FIXED START */
        if (NULL == add_info)
        {
            TRACE ( 2 , "IP_MANAGER", "URL Received is NULL");\
        DEALLOCATE(hostname);
	    /*Coverity 111417 Fix Start*/
             /*code removed */
	    /*Coverity 111417 Fix End*/
            return NULL;
        }
        /*COVERITY 110593 FIXED END */
	memset(add_info, 0, PERSISTENT_BUFFER_SIZE); 
	/* SPR 19836 FIXED END */

    /*SPR 19393 FIXED START */
     struct sockaddr_in sa = {0};
     UInt8 result = 0;
    /*SPR 19393 FIXED START */
	if(PNULL == URL)
	{
		TRACE ( 2 , "IP_MANAGER", "URL Received is NULL");
        /* SPR 22314 Start */
        DEALLOCATE(add_info);
        /* SPR 22314 End */
		DEALLOCATE(hostname);
		return PNULL;
	}
	else 
	{
		tr_strncpy((char*)TempUrl, (char*)URL);
		sec = strtok((char *)TempUrl,"/");
		if(NULL == sec)
		{
			TRACE ( 2 , "IP_MANAGER", "sec Received is NULL");
            /* SPR 22314 Start */
            DEALLOCATE(add_info);
            /* SPR 22314 End */
			DEALLOCATE(hostname);
			return PNULL;
		}
		tr_strncpy((char*)Sec_type, (char*)sec);
		IP_Value = strtok(PNULL,"/");
		if(NULL == IP_Value)
		{
			TRACE ( 2 , "IP_MANAGER", "sec Received is NULL");
            /* SPR 22314 Start */
            DEALLOCATE(add_info);
			DEALLOCATE(hostname);
            /* SPR 22314 End */
			return PNULL;
		}

        /* SPR 22314 Start */
        /* Code Removed */
        /* SPR 22314 End */
		/* SPR 19836 FIXED START */
		add_info_t = strtok(PNULL, "/");
		while (NULL != add_info_t)
		{
			strcat(add_info, add_info_t);
			strcat(add_info, "/");
			add_info_t = strtok(PNULL,"/");

		}
		str_len = strlen(add_info);
		add_info[str_len-1] = '\0';
		/* SPR 19836 FIXED END */
            /*COVERITY 110593 FIXED START */
            /* code removed */
            /*COVERITY 110593 FIXED END */
		
		if(PNULL != add_info) 
		{
			strncpy((char*)add_url_info, (char*)add_info,strlen(add_info)+1);
            /* SPR 21563 FIXED START */
        //DEALLOCATE(add_info);
            /* SPR 21563 FIXED END */
		}
		strncpy(Domain_name_or_ip,IP_Value,strlen(IP_Value)+1);

		if(Domain_name_or_ip[0] == '[')
		{
			temp_str = strtok(Domain_name_or_ip,"]");
			if(NULL == temp_str)
			{
				TRACE ( 2 , "IP_MANAGER", "sec Received is NULL");
				DEALLOCATE(hostname);
                /* SPR 22314 Start */
                DEALLOCATE(add_info);
                /* SPR 22314 End */
				return PNULL;
			}
			strncpy((char *)hostname,(char *)(temp_str+1),ENB_STRLEN(temp_str));
			getaddrinfo((char * )hostname, PNULL, &hint, &res);
			if (res->ai_family != AF_INET6)
			{
				TRACE ( 1 , "IP_MANAGER ERROR", "Ip is Not Valid IPV6");
                /* SPR 22314 Start */
                DEALLOCATE(add_info);
                TR069_FREE_ADDRINFO(res);
                /* SPR 22314 End */
				DEALLOCATE(hostname);
				return PNULL;
			}
			TRACE ( 1 , "IP_MANAGER", "Ipv6 Address Recieved In URL");
			*domain_or_ip = 'i';
            /* SPR 22314 Start */
            TR069_FREE_ADDRINFO(res);
            DEALLOCATE(add_info);
            /* SPR 22314 End */
			return hostname;

		}
		else 
		{
			temp_str = strtok((char *)Domain_name_or_ip,":");
			if(NULL == temp_str)
			{
				TRACE ( 2 , "IP_MANAGER", "sec Received is NULL");
            /*SPR 19393 FIXED START */
		    tr_strncpy((char *)hostname,(char *)Domain_name_or_ip);

			}
            else 
			{
			tr_strncpy((char *)hostname,(char *)temp_str);


    
			}

            result = ENB_INET_PTON(AF_INET, (const char *)hostname, &(sa.sin_addr));
            if(result == TR069_ZERO)
			{
				*domain_or_ip = 'd';

			    tr_strncpy((char *)hostname,(char *)IP_Value);
                /* SPR 22314 Start */
                DEALLOCATE(add_info);
                TR069_FREE_ADDRINFO(res);
                /* SPR 22314 End */

				return hostname;
			}
			else
			{
				getaddrinfo((char * )hostname,PNULL, &hint, &res);
				if (res->ai_family != AF_INET)
				{
					TRACE ( 1 , "IP_MANAGER ERROR", "Ip is Not Valid IPV4");
                    DEALLOCATE(hostname);
                    /* SPR 22314 Start */
                    DEALLOCATE(add_info);
                    TR069_FREE_ADDRINFO(res);
                    /* SPR 22314 End */
                    return PNULL;

				}
				*domain_or_ip = 'i';
				TRACE ( 1 , "IP_MANAGER", "Ipv4 Address Recieved In URL");
                /* SPR 22314 Start */
                TR069_FREE_ADDRINFO(res);
                /* SPR 22314 End */
 
			    tr_strncpy((char *)hostname,(char *)IP_Value);
                /* SPR 22314 Start */
                DEALLOCATE(add_info);
                /* SPR 22314 End */

				return hostname;
			}

            /*SPR 19393 FIXED END */
		}
	}
	/* IPV6 CHANGES END*/
}
	
/*********************************************************************************************************************************
*
*		FUNCTION NAME : ipr_cwmp_swap_values()		
*
*		DESCRIPTION   : This function swaps the values of the last Ip with random IP
*
*		RETURNS	      : nothing
*
********************************************************************************************************************************/
	
void ipr_cwmp_swap_values(SInt32 list_size)
{
	UInt32 index = ipr_cwmp_iprand (0, list_size);
	UInt8 temp_ip[PARTIAL_URL] = {0};
	ENB_MEMSET(temp_ip, 0, PARTIAL_URL);
	tr_strncpy((char*)temp_ip, (char*)(g_ipr_cwmp_L_IP_list[index]));
	ENB_MEMSET(g_ipr_cwmp_L_IP_list[index], 0, PARTIAL_URL);
	tr_strncpy((char*)g_ipr_cwmp_L_IP_list[index], (char*)(g_ipr_cwmp_L_IP_list[list_size - 1]));
	ENB_MEMSET(g_ipr_cwmp_L_IP_list[list_size-1], 0, PARTIAL_URL);
	tr_strncpy((char*)g_ipr_cwmp_L_IP_list[list_size - 1], (char*)temp_ip);
}
	
	
/*****************************************************************************************************************************
*
*		FUNCTION NAME : ipr_cwmp_iprand()		
*
*		DESCRIPTION   : This function uses a randomisation algorithm to find a random index
*
*		RETURNS	      : random index
*
*****************************************************************************************************************************/
	
/* rand: return pseudo-random integer on 0..32767 */
UInt32 ipr_cwmp_iprand(UInt32 min, UInt32 max)
{
	UInt32 dif = max - min;
	time_t t1 = {0};
	time(&t1);
	/* reboot 14 jan change start*/
	UInt32 number;
	g_ipr_cwmp_next = g_ipr_cwmp_next * t1 + 12345;
	if (dif == 0)
	{
		number = min;
	}
	else
	{
		number = (UInt32)(g_ipr_cwmp_next/dif) % dif;
	}
	/* reboot 14 jan change end*/
        return (min + number);

}
	
/***************************************************************************************************************************
*
*		FUNCTION NAME : ipr_cwmp_srand()		
*
*		DESCRIPTION   : This function is used by the ipr_cwmp_iprand function 
*
*		RETURNS	      : nothing
*
*****************************************************************************************************************************/
	
/* ipr_cwmp_srand: set seed for rand() */
void ipr_cwmp_srand(UInt32 seed)
{
	g_ipr_cwmp_next = seed;
}
	
	
/*********************************************************************************************************************************
*
*               FUNCTION NAME : ipr_cwmp_lengthOfU
*
*               DESCRIPTION   : This function is used to find lenght of pointer
*
*               RETURNS       : length
*
****************************************************************************************************************************/
	
UInt32 ipr_cwmp_lengthOfU(UInt8* str)
{
	UInt32 i = 0;
	while(*(str++))
    {
    	i++;
    	if(i == INT_MAX)
        	return -1;
	}
    /* COVERITY CID: 86639 FIX START */
    /* Code deleted */
    /* COVERITY CID: 86639 FIX START */
	return i;
}
