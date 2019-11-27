/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc . All Rights Reserved.
 *
 ******************************************************************************
 *
 * $$Id: dhcp_client.c,v 1.1 2012/12/28 06:02:48 gur32338 Exp $
 *
 *******************************************************************************
 *
 * File Description: This file contains all the function definitions 
 *                    that are involved in the DHCP operation 
 *
 *******************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   2012/12/28 06:02:48  gur32338  Incorporated Coding guideline adherence changes 
 *   2012/12/27 11:21:29  gur24604  Incorporated Coding guidelines adherence changes
 *   2011/08/30 10:48:04            Initial
 ******************************************************************************/
	
#include <dhcp_client.h>
#include <ipr_cwmp_enb_wrapper.h>

static UInt8* ipr_cwmp_dhcpinform (UInt32 s2, UInt32 ipaddr, UInt32 broadcast, UInt8 *ether);
static UInt8* ipr_cwmp_display_options (UInt8 *msg, UInt32 tl, UInt8 *cookie);
/*SPR 16545 Fixed Start*/
/*  coverity 94850 fix start */
extern  S32 tr069_gettimeofday_wrapper(struct timeval *p_tv);
/*  coverity 94850 fix end */
/*SPR 16545 Fixed End*/

/****************************************************************************
 * Function Name  : ipr_cwmp_dhcp_client 
 * Inputs         :None  
 *                 
 * Outputs        : None
 * Returns        : ACS URL 
 * Description    : This function retrieves the URL of the ACS via the DHCP server 
 *                  
 *                  
 ****************************************************************************/	
	
UInt8 * ipr_cwmp_dhcp_client(void)
{
	UInt8 * URL = PNULL;
	S32 fd = 0, lfd = 0, fd1 = 0;	
	UInt32 option = 1;
	UInt8 ether[6] = {0};
	UInt32 ipaddr = 0;
	UInt32 broadcast = 0;
	struct if_nameindex *curif = {0}, *ifs = PNULL;
	struct ifreq req;
	struct sockaddr_in localaddr = {0};
	ENB_MEMSET(&req, 0, sizeof(struct ifreq));
	if ((fd = ENB_SOCKET(PF_INET, SOCK_DGRAM, 0)) != -1) 
	{
		ifs = if_nameindex();
		if (ifs) 
		{
			for (curif = ifs; curif && curif->if_name; curif++) 
			{
				/*Review comments incorporated */
                /*COVERITY 87081 FIX START*/
                if (NULL == curif->if_name)
                {
                   continue; 
                }

                ENB_MEMSET(req.ifr_name, 0, sizeof(req.ifr_name));
                /*COVERITY 88470 FIX START */
                ENB_STRNCPY(req.ifr_name, curif->if_name,
                            sizeof(req.ifr_name) - 1);
                /*COVERITY 88470 FIX END*/
                /*COVERITY 87081 FIX END*/

				if (ENB_IOCTL(fd, SIOCGIFADDR, &req) < 0)
				{
					continue;
				}
				else
				{

					if (req.ifr_addr.sa_family != AF_INET) 
					{
						continue; /* IPv4 */
					}
					if (((void*)ENB_STRSTR (req.ifr_name, "eth") == PNULL))
					{
						continue;
					}
				}
				ipaddr = ((struct sockaddr_in*)&req.ifr_addr)->sin_addr.s_addr;
				if ((ENB_IOCTL(fd,   SIOCGIFFLAGS,  &req)))
				{
					ENB_FPRINTF(stderr, "ENB_IOCTL get flags: %s\n", strerror(errno));
				}
				if ((req.ifr_flags & IFF_UP) && (req.ifr_flags & IFF_BROADCAST))
				{
					if ((ENB_IOCTL(fd, SIOCGIFHWADDR, &req)))
					{ 
						ENB_FPRINTF(stderr, "ENB_IOCTL get getmac: %s\n", strerror(errno));
					}
					ENB_MEMCPY (ether, &(((struct sockaddr)(req.ifr_hwaddr)).sa_data[0]),6);
					if (ENB_IOCTL(fd, SIOCGIFBRDADDR, &req))
					{
						ENB_FPRINTF(stderr, "ENB_IOCTL broadcast: %s\n", strerror(errno));
					}
					broadcast = ((struct sockaddr_in *) &req.ifr_broadaddr)->sin_addr.s_addr;
					ENB_FPRINTF(stderr,"%s \tip=%08X \tbroadcast=%08X \t%02X:%02X:%02X:%02X:%02X:%02X\n","det",ENB_HTONL(ipaddr),ENB_HTONL(broadcast),ether[0],ether[1],ether[2],ether[3],ether[4],ether[5]);
				}
			}

			//coverity fix
			fd1  = ENB_SOCKET(PF_INET, SOCK_DGRAM,0);
			if(-1 == fd1)
			{
				ENB_FPRINTF(stderr, " Error in socket open: %s\n", strerror(errno));
				close(fd);
				ENB_FREE(curif);
				return NULL; 
			}else
				lfd = fd1;	
			localaddr.sin_family = PF_INET;
			localaddr.sin_port = ENB_HTONS(68);
			localaddr.sin_addr.s_addr = ipaddr;
			//coverity fix
			if(ENB_SETSOCKOPT(lfd,SOL_SOCKET, SO_REUSEADDR, &option, sizeof(UInt32)))
			{
				ENB_FPRINTF(stderr, "ENB_SETSOCKOPT broadcast: %s\n", strerror(errno));
			}

			if (ENB_BIND(lfd,(struct sockaddr *) &localaddr,sizeof(struct sockaddr_in)))
			{
				ENB_FPRINTF(stderr, "bind: %s\n", strerror(errno));
			}
			if (ENB_SETSOCKOPT(lfd, SOL_SOCKET, SO_BROADCAST, (void *) &option, sizeof (UInt32)))
			{
				ENB_FPRINTF(stderr, "ENB_SETSOCKOPT broadcast: %s\n", strerror(errno));
			}
			URL = ipr_cwmp_dhcpinform(lfd,ipaddr,broadcast,ether);
			if_freenameindex(ifs);

			if (close(lfd)!=0)
			{
				ENB_FPRINTF(stderr, "close: %s\n", strerror(errno));
			}
		} 
		else
		{
			ENB_FPRINTF(stderr, "if_nameindex: %s\n", strerror(errno));
		}

		if (close(fd)!=0)
		{
			ENB_FPRINTF(stderr, "close: %s\n", strerror(errno));
		}	

	} 
	else
	{
		ENB_FPRINTF(stderr, "socket: %s\n", strerror(errno));
	}
	return URL;
}
	
/****************************************************************************
 * 
 * Function Name  : ipr_cwmp_dhcp_client
 * Inputs         : UInt32 s,UInt32 s2,UInt32 ipaddr,
 *                   UInt32 broadcast,UInt8 *ether
 *
 * Outputs        : None
 * Returns        : ACS URL
 * Description    : This function broadcasts to all machines and finds the DHCP 
 *                  server and receives an unicast response from the DHCP server 
 *                  containign the ACS URL
 *
 ****************************************************************************/

UInt8 * ipr_cwmp_dhcpinform (
	UInt32 s2,
	UInt32 ipaddr, 
	UInt32 broadcast, 
	UInt8 *ether
	)
{
	UInt32 addr = broadcast;
	 UInt8 msg[IPR_CWMP_MSG_LEN], magic_cookie[] = {99,130,83,99},
	      msg_inform[] = {53,1,8}, msg_wpad[] = {252,0}, msg_ml_h[] = {57,02 }  ;
	UInt8 dsl[] = {0x64,0x73,0x6c,0x66,0x6f,0x72,0x75,0x6d,0x2e,0x6f,0x72,0x67};
	struct timeval tv = {0};
	UInt32 xid = 0;
	UInt16 ml = 0;
	struct sockaddr_in in = {0}, msgr = {0};
	struct pollfd pfd = {0};
	UInt32  len = 0;
	int len1 = 0;
	UInt32 eom = 0;
	UInt32 code = 0;
	UInt32 pollto = 0;
	socklen_t sl = {0};
	UInt8 * URL = PNULL;
    /* coverity 94850 fix start */
	ENB_GETTIMEOFDAY(&tv);
    /* coverity 94850 fix end */
	xid = tv.tv_usec & 0xffffffff;
	ml = 4096;
	ml = ENB_HTONS(ml);
	ENB_MEMSET(msg,0,2048);            /* set options full of PAD */
	msg[F_OP] =  1;         /* boot request */
	msg[F_HTYPE] = 1;               /* ethernet */
	msg[F_HLEN] = 6;                /* mac address length */
	msg[F_HOPS] = 0;                /* no relays yet */
	ENB_MEMCPY(msg+F_XID,&xid,4);      /* transaction id */
	ENB_MEMSET(msg+F_SECS,0,2);        /* seconds since start */
	ENB_MEMSET(msg+F_FLAGS,0,2);       /* we can accept unicast responses */
	ENB_MEMCPY(msg+F_CIADDR,&ipaddr,4); /* my confirmed IP */
	ENB_MEMSET(msg+F_YIADDR,0,4);      /* offered IP */
	ENB_MEMSET(msg+F_SIADDR,0,4);      /* dhcp server IP */
	ENB_MEMSET(msg+F_GIADDR,0,4);      /* relay agent IP */
	ENB_MEMSET(msg+F_CHADDR,0,16);
	ENB_MEMCPY(msg+F_CHADDR,ether,6); /* client hardware address */
	ENB_MEMSET(msg+F_SNAME,0,64);
	ENB_MEMSET(msg+F_FILE,0,128);
	ENB_MEMCPY(msg+F_COOKIE, magic_cookie,4); /* magic cookie  */
	ENB_MEMCPY(msg+F_INFORM, msg_inform, 3);   /* operation DHCPINFORM */
	ENB_MEMCPY(msg+F_WPAD, msg_wpad, 2); /* we'd like the WPAD string */
	ENB_MEMCPY(msg+F_ML_H, msg_ml_h,2); /* max length header */
	ENB_MEMCPY(msg+F_ML_D, &ml, 2);    /* max length data */
	eom = F_CUSTOM;
	msg[eom++] = 55;
	msg[eom++] = 1;
	msg[eom++] = 43;
	msg[eom++] = 60;
	msg[eom++] = 12;
	ENB_MEMCPY(msg+eom ,&dsl,12);
	eom += 12;
	/* rfc 2131 says we should include all the options we care about (that's all of them) in the request */
	for (code=1;0 && code < 255; code++)
	{
		if ((code != 252) && (code != 57) && (code != 53) )
		{
			msg[eom++] = code;
			msg[eom++] = 0;
		}
	}
	msg[eom++] = 255;               /* end */

	/* ------------------- PACKET IS NOW SETUP  ---------------- */

	in.sin_family = AF_INET;
	in.sin_port = ENB_HTONS(67);
	ENB_MEMCPY(&in.sin_addr, &addr,sizeof (in.sin_addr));
	sl = TR069_SENDTO(s2, msg, eom, 0, (void *) &in, sizeof (struct sockaddr_in));
	if (sl < eom)
	{
		ENB_FPRINTF(stderr, "sendto: %s\n", strerror(errno));
	}

	/* ---------------- DHCPINFORM HAS BEEN SENT -------------- */

	pfd.fd = s2;
	pfd.events = POLLIN;
	pollto = 10750;         /* we'll start the timeout at 750 ms, but drop it to 250 after */

	/* we loop here because there could be multiple dhcp severs with different information that will respond to our broadcast */

	while (ENB_POLL(&pfd,1,pollto))
	{
        ENB_MEMSET(msg,'\0',2048);                /* set options full of PAD */
		sl = sizeof(struct sockaddr_in);
		len1 = ENB_RECVFROM(pfd.fd, msg, 2048, 0,(struct sockaddr *) &msgr,&sl);
		ENB_PRINTF(" bytes recieved %d ",len1);
        if(len1 <= 0)
		{
			//coverity fix
			DEALLOCATE(URL);
			return NULL;
        }
        if(len1 >= 0)
        {
			len= len1;
            msg[len] = '\0';
			URL = ipr_cwmp_display_options((UInt8 *)msg,(UInt32 )len,(UInt8 *)magic_cookie);
			pfd.revents = 0;
			pollto = 1250;
		}
}
	return URL;
}
	
/****************************************************************************
 *
 * Function Name  : ipr_cwmp_display_options
 * Inputs         : UInt8 *msg,UInt32 tl,UInt8 *cookie
 *                  
 * Outputs        : None
 * Returns        : PNULL
 * Description    : This function is involved  with the display of the various 
 *                  DHCP options 
 *
 ****************************************************************************/
	
UInt8 * ipr_cwmp_display_options (
	UInt8 *msg, 
	UInt32 tl, 
	UInt8 *cookie
	)
{
	UInt32 cntr = 0, code = 0, len = 0, counter = 0;
	UInt8 *durl = PNULL;
	UInt8 *minwaitInterval = PNULL;
	UInt8 *retryInterval = PNULL;
	if (ENB_MEMCMP(cookie, msg+F_OPTIONS, 4))
	{
		ENB_FPRINTF(stderr, "cookie mismatch: %s\n", strerror(errno));
	}
	for (cntr = F_OPTIONS+4; cntr < tl; )
	{
		code= msg[cntr];
		if ( (code == 255) || (code == 0) )
		{	
			len = 0;
			cntr++;
		}		
		else if(cntr < (tl -1) )
		{
			len = msg[cntr+1];
			cntr += 2+len;
		}
		else
		{
			perror("error");
			exit(1);
		}

		fflush (stderr);
		if (code == 255)                /* end of options */
		{
			break;
		}
		if (code == 43)
		{
			counter = 0;
			if(!((len > 0 ) && (len < 255)))
			{
				exit(1);
			}
			while (counter< len)
			{
				UInt32 suboption = msg[cntr + counter - len];
				UInt32 slen = msg[cntr + counter + 1 - len];
				UInt8 * str =(UInt8 *) ALLOCATE((slen + 1));
				if(NULL == str)
				{
					exit(0);
            }
            //coverity 
            if (slen > 0)
            {
                ENB_MEMCPY(str, msg + cntr + counter + 2 - len, slen);
                str[slen] = '\0';
            }
            ENB_FPRINTF(stdout,"%s",str);
            if (suboption == 1)
            {
                //coverity
                if (PNULL == durl)
					{
						durl = str;
					}
				}
				else if (suboption == 2)
				{
					//pcode = str;
				}
				else if (suboption == 3)
				{
					minwaitInterval = str;
					ipr_cwmp_setCWMPRetryMinimumWaitInterval(atoi((char*)minwaitInterval));
					DEALLOCATE(str);
				}
				else if (suboption == 4)
				{
					retryInterval = str;
					ipr_cwmp_setCWMPRetryIntervalMultiplier(atoi((char*)retryInterval));
					DEALLOCATE(str);
				}
				else
				{
					//coverity
					DEALLOCATE(str);
				}
				counter += slen + 2;
			}
		}
		fflush (stdout);
		fflush (stderr);
	}
	return durl;
}
