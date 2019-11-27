/******************************************************************************
**
**  FILE NAME     :  rrc_shell.c
**
**  DESCRIPTION   :  This file is carries the functional definitions for the
**                   encapsulating shell "CMES" code.
**
**
**  DATE        AUTHOR          REF             REASON
**  --------    -------         ---             ----------
**  1-June-09   Ritika Garg    DDv0.2          Initial
**
**     Copyright (C) 2006 Aricent Inc . All Rights Reserved
******************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "stacklayer.h"
#include "cspl.h"
#include "rrc_main.h"
#include "rrc_ext_api.h"
#include "rrc_common_utils.h"

/****************************************************************************
 * Exported Variables
****************************************************************************/
extern QMODULE        oamhModule;
extern QMODULE        cscModule;
extern QMODULE        ueccModule;
extern QMODULE        llimModule;

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/
static void readConfigFile (U8 *confFileName_p);
static S32 readIpPort (U8 *filename_p,
                          U8 *keyString_p,
                          U8 *returnString_p);


#define MY_PORT "4950"
/***********************************************************************
 * Function Name  : readIpPort
 * Input          : U8 *filename_p
 *                  U8 *keyString_p
 *                  U8 *returnString_p
 * Output         : None
 * Returns        : 0 if the keystring is found in config file
                    else returns -1
 * Description    : Searches for the key string in config file
 *                  and returns the desired string once it was found
***********************************************************************/
static S32 readIpPort (U8 *filename_p,
                          U8 *keyString_p,
                          U8 *returnString_p)
{
    FILE     *fptr_p = PNULL;
    S32   retResult = -1;
    U8    keyStringBuf[MAX_KEY_SIZE];

    RRC_ASSERT (PNULL != filename_p);
    RRC_ASSERT (PNULL != keyString_p);
    RRC_ASSERT (PNULL != returnString_p);

    if ((fptr_p = fopen_wrapper (filename_p, "r")) == (FILE *)0)
    {
        /* Here trying to display the file name also */
        /* RRC_DEBUG (("Cannot open file %s for reading ", filename_p));*/
        RRC_ASSERT (0);
    }

    while (PNULL != fgets_wrapper (returnString_p, MAX_BUFF_SIZE, fptr_p))
    {
        if (sscanf (returnString_p, "%s", keyStringBuf) <= 0)
        {
            continue;
        }
        if (!strcmp_wrapper (keyStringBuf, "#"))
        {
            fflush_wrapper(fptr_p);
            continue;
        }
          if (strlen_wrapper (keyStringBuf) == 0)
        {
            continue;
        }
        if (!strcmp_wrapper (keyString_p, keyStringBuf))
        {
            retResult = 0;
            break;
        }
    }
    rrc_fclose (fptr_p);

    return (retResult);
}



/***********************************************************************
 * Function Name   : readConfigFile
 * Input           : U8 *confFileName_p
 * Output          : None
 * Returns         : void
 * Description     : Reads each module's configuration from the file
 *                   and stores in IpcDataList global varaible.
 ****************************************************************************/
static void readConfigFile (U8 *confFileName_p)
{
    U8 temp[IPADDR_PORT_MAX_LEN];

    RRC_ASSERT (PNULL != confFileName_p);

    if(!readIpPort(confFileName_p, "RRC_RRM_IP_PORT",  temp))
    {
        RRC_ASSERT ("RRC_RRM_IP_PORT parameter not Found In Config File");
    }
    RRC_ASSERT (!(sscanf (temp, "%s%s", confLabel[RRM_IP_INDEX],
                    IpcDataList[RRM_IP_INDEX]) < 0));

    if(!readIpPort(confFileName_p, "RRC_OAM_IP_PORT",  temp))
    {
        RRC_ASSERT ("RRC_OAM_IP_PORT parameter not Found In Config File");
    }
    RRC_ASSERT (!(sscanf (temp, "%s%s", confLabel[OAM_IP_INDEX],
                    IpcDataList[OAM_IP_INDEX]) < 0));

    if(!readIpPort(confFileName_p, "RRC_S1AP_IP_PORT",  temp))
    {
        RRC_ASSERT ("RRC_S1AP_IP_PORT parameter not Found In Config File");
    }
    RRC_ASSERT (!(sscanf (temp, "%s%s", confLabel[S1AP_IP_INDEX],
                    IpcDataList[S1AP_IP_INDEX]) < 0));

    if(!readIpPort(confFileName_p, "RRC_PDCP_IP_PORT",  temp))
    {
        RRC_ASSERT ("RRC_PDCP_IP_PORT parameter not Found In Config File");
    }
    RRC_ASSERT (!(sscanf (temp, "%s%s", confLabel[PDCP_IP_INDEX],
                    IpcDataList[PDCP_IP_INDEX]) < 0));

    if(!readIpPort(confFileName_p, "RRC_RLC_IP_PORT",  temp))
    {
        RRC_ASSERT ("RRC_RLC_IP_PORT parameter not Found In Config File");
    }
    RRC_ASSERT (!(sscanf (temp, "%s%s", confLabel[RLC_IP_INDEX],
                    IpcDataList[RLC_IP_INDEX]) < 0));

    if(!readIpPort(confFileName_p, "RRC_MAC_IP_PORT",  temp))
    {
        RRC_ASSERT ("RRC_MAC_IP_PORT parameter not Found In Config File");
    }
    RRC_ASSERT (!(sscanf (temp, "%s%s", confLabel[MAC_IP_INDEX],
                    IpcDataList[MAC_IP_INDEX]) < 0));

    if(!readIpPort(confFileName_p, "RRC_PHY_IP_PORT",  temp))
    {
        RRC_ASSERT ("RRC_PHY_IP_PORT parameter not Found In Config File");
    }
    RRC_ASSERT (!(sscanf (temp, "%s%s", confLabel[PHY_IP_INDEX],
                    IpcDataList[PHY_IP_INDEX]) < 0));

    if(!readIpPort(confFileName_p, "RRC_RF_IP_PORT",  temp))
    {
        RRC_ASSERT ("RRC_RF_IP_PORT parameter not Found In Config File");
    }
    RRC_ASSERT (!(sscanf (temp, "%s%s", confLabel[RF_IP_INDEX],
                    IpcDataList[RF_IP_INDEX]) < 0));

}


/****************************************************************************
 * Function Name  : rrc_timed_wait
 * Inputs         : 1) p_cspl_listener: socket descriptor list
 *                : 2) p_timer_data: cspl Timer Data
 * Output         : none              
 * Return         : none
 * Description    : This function will be called from cspl process for every
 *                : CSPL_TIME_MICRO_SEC micro seconds.p_timer_data can be NULL
 ****************************************************************************/
void rrc_timed_wait (void *p_cspl_listener, const QTIME *p_timer_data)
{
    Listener            *listener_p = PNULL;
    struct timeval      timeout;

    RRC_ASSERT (PNULL != p_cspl_listener);

    listener_p = (Listener *)p_cspl_listener;
    RRC_ASSERT (PNULL != listener_p);

    lteFD_ZERO (&(listener_p->fdSet));
    lteFD_SET((S32)listener_p->sdRrc,&(listener_p->fdSet));

    timeout.tv_sec = 0;
    timeout.tv_usec = CSPL_TIME_MICRO_SEC;

    if (p_timer_data)
    {
        if (timeout.tv_usec > p_timer_data->us)
        {
            timeout.tv_usec = p_timer_data->us;
        }
    }

    if ((select_wrapper (FD_SETSIZE - 1, &(listener_p->fdSet), 0 , 0, &timeout)) < 0)
    {
        /* RRC_DEBUG (("ERROR : select: rrc_timed_wait"));*/
    }

    RRC_ASSERT (INVALID_FD != listener_p->sdRrc);

}


/****************************************************************************
 * Function Name  : rrc_send
 * Input          : void *p_cspl_listener
 *                  const void *message_p
 *                  QMODULE sender
 *                  QMODULE receiver
 *                  void *data_p
 * Output         : none
 * RETURNS        : none
 * Description    : Top level send function.data_p can be NULL
 ****************************************************************************/
void rrc_send (void *p_cspl_listener,
               const void *message_p, /* message to be sent */
               QMODULE sender,
               QMODULE receiver,
               void *data_p,
               U32 size /* size of data to be sent to destination */)
{
    Listener         *listener_p = PNULL;
    U16              receiverId = (U16)qvGetServiceName (receiver);
    U16              senderId = (U16)qvGetServiceName (sender);
    sockaddr         dest;
    U16              msg_len = RRC_NULL;

    /* Get the message length from EXT API HEADER */
    msg_len = ((U16)yfGetU16At(((U8 *)message_p), 8)) + EXT_API_HDRSIZE;

    RRC_ASSERT (PNULL != p_cspl_listener);
    RRC_ASSERT (PNULL != message_p);

    /* Get the EXT MODULES port Ids from the Config File */
    readConfigFile(RRC_CONFIG_FILE);

    listener_p = (Listener *)p_cspl_listener;
    RRC_ASSERT (PNULL != listener_p);

    /* Based on the ReceiverId, send the message on respective socket */
    switch (receiverId)
    {
        case RRC_RRM_MODULE_ID:
            dest.sin_family =  AF_INET;
            dest.sin_port = htons_wrapper(confLabel[RRM_IP_INDEX]);
            dest.sin_addr.s_addr = inet_addr_wrapper((U8 *)&IpcDataList[RRM_IP_INDEX]);
            sendto_wrapper(listener_p->sdRrc, (S8 *)message_p, msg_len,
                        0, (struct sockaddr *)&dest, sizeof(dest) );
            break;

        case RRC_OAM_MODULE_ID:
            dest.sin_family =  AF_INET;
            dest.sin_port = htons_wrapper(confLabel[OAM_IP_INDEX]);
            dest.sin_addr.s_addr = inet_addr_wrapper((U8 *)&IpcDataList[OAM_IP_INDEX]);
            sendto_wrapper(listener_p->sdRrc, (S8 *)message_p, msg_len,
                        0, (struct sockaddr *)&dest, sizeof(dest) );
            break;

        case RRC_S1AP_MODULE_ID:
            dest.sin_family =  AF_INET;
            dest.sin_port = htons_wrapper(confLabel[S1AP_IP_INDEX]);
            dest.sin_addr.s_addr = inet_addr_wrapper((U8 *)&IpcDataList[S1AP_IP_INDEX]);
            sendto_wrapper(listener_p->sdRrc, (S8 *)message_p, msg_len,
                        0, (struct sockaddr *)&dest, sizeof(dest) );
            break;

        case RRC_PDCP_MODULE_ID:
            dest.sin_family =  AF_INET;
            dest.sin_port = htons_wrapper(confLabel[PDCP_IP_INDEX]);
            dest.sin_addr.s_addr = inet_addr_wrapper((U8 *)&IpcDataList[PDCP_IP_INDEX]);
            sendto_wrapper(listener_p->sdRrc, (S8 *)message_p, msg_len,
                        0, (struct sockaddr *)&dest, sizeof(dest) );
            break;

        case RRC_RLC_MODULE_ID:
            dest.sin_family =  AF_INET;
            dest.sin_port = htons_wrapper(confLabel[RLC_IP_INDEX]);
            dest.sin_addr.s_addr = inet_addr_wrapper((U8 *)&IpcDataList[RLC_IP_INDEX]);
            sendto_wrapper(listener_p->sdRrc, (S8 *)message_p, msg_len,
                        0, (struct sockaddr *)&dest, sizeof(dest) );
            break;

        case RRC_MAC_MODULE_ID:
            dest.sin_family =  AF_INET;
            dest.sin_port = htons_wrapper(confLabel[MAC_IP_INDEX]);
            dest.sin_addr.s_addr = inet_addr_wrapper((U8 *)&IpcDataList[MAC_IP_INDEX]);
            sendto_wrapper(listener_p->sdRrc, (S8 *)message_p, msg_len,
                        0, (struct sockaddr *)&dest, sizeof(dest) );
            break;

        case RRC_PHY_MODULE_ID:
            dest.sin_family =  AF_INET;
            dest.sin_port = htons_wrapper(confLabel[PHY_IP_INDEX]);
            dest.sin_addr.s_addr = inet_addr_wrapper((U8 *)&IpcDataList[PHY_IP_INDEX]);
            sendto_wrapper(listener_p->sdRrc, (S8 *)message_p, msg_len,
                        0, (struct sockaddr *)&dest, sizeof(dest) );
            break;

        case RRC_RF_MODULE_ID:
            dest.sin_family =  AF_INET;
            dest.sin_port = htons_wrapper(confLabel[RF_IP_INDEX]);
            dest.sin_addr.s_addr = inet_addr_wrapper((U8 *)&IpcDataList[RF_IP_INDEX]);
            sendto_wrapper(listener_p->sdRrc, (S8 *)message_p, msg_len,
                        0, (struct sockaddr *)&dest, sizeof(dest)) ;
            break;

        default:
            RRC_ASSERT ("ERROR: Unknown Destination ");
            break;
    }
}


/****************************************************************************
 * Function Name  : rrc_receive
 * Input          : void      *p_cspl_listener
 *                  QMODULE   *sender_p
 *                  QMODULE   *receiver_p
 *                  S8     *priority_p
 * Output         : None                 
 * Returns        : none
 * Description    : This functions is called from CSPL process after some
 *                : time interval defined in rrc_timed_wait function. This
 *                : function receives the message from TCP, populates
 *                : cspl header based on the receiver modules and returns
 *                : to the CSPL process
 ****************************************************************************/
void *rrc_receive (void      *p_cspl_listener,
                  QMODULE   *sender_p,
                  QMODULE   *receiver_p,
                  S8     *priority_p)
{
    Listener            *listener_p = PNULL;
    U8                  *buf_p = PNULL;
    U8                  ext_api_hdr[EXT_API_HDRSIZE];
    EXTAPIHDR           stack_ext_api_hdr;
    STACKAPIHDR         cspl_hdr;
    U16                 msg_len = 0;
    sockaddr            from;
    socklen_t           len;

    RRC_ASSERT (PNULL != p_cspl_listener);
    RRC_ASSERT (PNULL != sender_p);
    RRC_ASSERT (PNULL != receiver_p);
    RRC_ASSERT (PNULL != priority_p);

    listener_p = (Listener *)p_cspl_listener;
    RRC_ASSERT (PNULL != listener_p);

    if((INVALID_FD != listener_p->sdRrc) &&
            (lteFD_ISSET((S32)listener_p->sdRrc, &(listener_p->fdSet))))
    {
        /* Read the Header from the socket */
        if((recvfrom_wrapper(listener_p->sdRrc,(S8 *)ext_api_hdr, EXT_API_HDRSIZE, 0,
            (struct sockaddr *)&from, &len)) <= 0)
        {
            /* RRC_DEBUG(" Nothing received");*/
        }
        else
        {
            /* Parse the EXT API HDR, and get the message length */
            rrc_parse_ext_api_hdr(ext_api_hdr, EXT_API_HDRSIZE,
                stack_ext_api_hdr);
            msg_len = EXT_API_HDRSIZE + stack_ext_api_hdr.buf_len;
            buf_p = qvMsgAllocBySize(msg_len + CV_HDRSIZE);
            RRC_ASSERT(PNULL != buf_p);

            cspl_hdr.payloadlen = 0;
            cspl_hdr.priority = 0;
            cspl_hdr.version = 0;
            cspl_hdr.from = stack_ext_api_hdr.source_id;
            cspl_hdr.paramlen = msg_len + CV_HDRSIZE;
            cspl_hdr.api = stack_ext_api_hdr.api_id;

            /* Append CSPL Header on top of buf_p */
            cvApiBuildHeader((U8 *)buf_p, &cspl_hdr);

            /* Append EXT API Header to buf_p */
            l3_memcpy_wrapper((void *)(buf_p + CV_HDRSIZE), (void *) &stack_ext_api_hdr
                , EXT_API_HDRSIZE);

            /* Append message buffer to buf_p*/
            if((recvfrom_wrapper(listener_p->sdRrc,(S8 *)(buf_p + CV_HDRSIZE +
            EXT_API_HDRSIZE), stack_ext_api_hdr.buf_len, 0, (struct sockaddr *)
                &from, &len)) <= 0)
            {
                /* RRC_DEBUG(" Void Msg received from RRM");*/
            }
            else
            {
                *sender_p = qvGetService(stack_ext_api_hdr.source_id);
                *priority_p = 0;

                if(RRC_OAM_MODULE_ID == stack_ext_api_hdr.source_id)
                {
                    cspl_hdr.to = RRC_OAMH_MODULE_ID;
                    *receiver_p = oamhModule;
                }
                else if((RRC_PDCP_MODULE_ID == stack_ext_api_hdr.source_id) ||
                   (RRC_RLC_MODULE_ID == stack_ext_api_hdr.source_id) ||
                   (RRC_MAC_MODULE_ID == stack_ext_api_hdr.source_id) ||
                   (RRC_PHY_MODULE_ID == stack_ext_api_hdr.source_id) ||
                       (RRC_RF_MODULE_ID == stack_ext_api_hdr.source_id))
                {
                    cspl_hdr.to = RRC_LLIM_MODULE_ID;
                    *receiver_p = llimModule;
                }
                else
                {
                    if((RRC_RRM_CELL_SETUP_REQ == stack_ext_api_hdr.api_id) ||
                       (RRC_RRM_CELL_DELETE_REQ == stack_ext_api_hdr.api_id) 
                       )
                    {
                        cspl_hdr.to = RRC_CSC_MODULE_ID;
                        *receiver_p = cscModule;
                    }
                    else
                    {
                        cspl_hdr.to = RRC_UECC_MODULE_ID;
                        *receiver_p = ueccModule;
                    }
                }
            }
        }
    }
    return (buf_p);
}

/***********************************************************************
 * Function Name  : rrc_make_addr
 * Input          : const U8 *str_p
 *                  sockaddr *address_p
 *                  U32 defaultAddress
 * Output         : none
 * Returns        : none
 * Description    : Setup the address_p structure with port number
 *                  and an Internet address of INADDR_ANY.
 *                  str_p contains IP & Port
 **********************************************************************/
static void rrc_make_addr(const U8 *str_p,
              sockaddr *address_p,
                          U32 defaultAddress)
{
    U16  port = 0;
    U32   ipa;

    RRC_ASSERT (PNULL != str_p);
    RRC_ASSERT (PNULL != address_p);

    memset_wrapper (address_p, 0x00, sizeof (*address_p));
    address_p->sin_family = AF_INET;

    if (strchr_wrapper (str_p, ':'))
    {
        if (sscanf (str_p, "%[^:]:%hu", ipa, &port) < 0)
        {
            RRC_ASSERT (0);
        }
        address_p->sin_port = htons_wrapper (port);
        address_p->sin_addr.s_addr = htonl_wrapper(ipa);
    }
    else
    {
        address_p->sin_port = htons_wrapper (atoi_wrapper (str_p));
        address_p->sin_addr.s_addr = defaultAddress;
    }
}

/****************************************************************************
 * Function Name  : rrc_print_addr
 * Input          : U8 *str_p
 *                  sockaddr *selfAddr
 *                  U32 len
 * Output         : none
 * Returns        : none
 * Description    : Print the address_p structure with port number and an
 *                  Internet address of INADDR_ANY
 ****************************************************************************/
static void rrc_print_addr(U8 *str_p, sockaddr *selfAddr, U32 len)
{
    RRC_ASSERT(PNULL != str_p);
    RRC_ASSERT(PNULL != selfAddr);
    RRC_ASSERT(len < IPADDR_MAX_LEN);

    U32 ipa =  ntohl_wrapper(selfAddr->sin_addr.s_addr);
    U16 port = ntohs_wrapper(selfAddr->sin_port);

    /*coverity_fix_531*/ 

    snprintf_wrapper(str_p,len,"%u, %u, %u, %u, %hu",
           YGETBYTE(ipa, 3),
           YGETBYTE(ipa, 2),
           YGETBYTE(ipa, 1),
           YGETBYTE(ipa, 0),
           port);
    /*coverity_fix_531*/ 

}



/****************************************************************************
 * Function Name  : rrc_make_server
 * Input          : const sockaddr *selfAddr_p
 * Output         : none
 * Returns        : none
 * Description    : Creates the reusable socket , bind and listens to the
 *                : incoming connections
 * Returns        : Socket discriptor
 ****************************************************************************/
static S32 rrc_make_server (const sockaddr *selfAddr_p)
{
    S32          sd = INVALID_FD;
    U8           sockName[IPADDR_MAX_LEN] = {0};
    const S32    optVal = 1;

    RRC_ASSERT (PNULL != selfAddr_p);

    rrc_print_addr (sockName, (sockaddr *)selfAddr_p, IPADDR_MAX_LEN);

    /* RRC_DEBUG (("Opening Server Socket at RRC --> %s", sockName));*/

    /* domain = AF_INET, type = SOCK_DGRAM, protocol = 0 */
    if ((sd = socket_wrapper(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
       /*RRC_DEBUG (("ENCAPS: ERROR :socket"));*/
    }

    if (setsockopt_wrapper (sd, SOL_SOCKET, SO_REUSEADDR, (S8 *)&optVal, sizeof
        (optVal)) < 0)
    {
        close_wrapper (sd);
        sd = INVALID_FD;
    }

    if (INVALID_FD != sd)
    {
        if (bind_wrapper(sd, (struct sockaddr *)selfAddr_p, sizeof (*selfAddr_p)) < 0)
        {
            close_wrapper (sd);
            sd = INVALID_FD;
        }

    }

    RRC_ASSERT (INVALID_FD != sd);
    return (sd);
}


/****************************************************************************
 * Function Name  : rrc_make_conn
 * Input          : void
 * Output         : none
 * Returns        : none
 * Description    : makes the connection (client/server) and returns the
 *                : socket descriptor
 ****************************************************************************/
static S32  rrc_make_conn (void)
{
    sockaddr        address;
    S32             tempSd = INVALID_FD;
    sockaddr        self_addr, *servinfo, *p;

    S32             sockfd;

    /* gur04564 - Support can be added to read these from a file */
    memset_wrapper(&self_addr, 0, sizeof(self_addr));
    self_addr.sin_family = AF_INET;
    self_addr.sock_type = SOCK_DGRAM;

    /* Shall we do this ..???  - Start */
    if(getsockaddr(NULL, MY_PORT, (const struct addrinfo *)&self_addr, (struct
        sockaddr **)&servinfo) != 0)
    {
        /*RRC_DEBUG("Socket open error");*/
    }

    for (p = servinfo; p != NULL; p = p->next)
    {
        if((sockfd = socket_wrapper(self_addr.sin_family, self_addr.sock_type,
                             0)) < 0)
        {
            /*RRC_DEBUG("Socket error");*/
        }

        if((bind_wrapper(sockfd, (struct sockaddr *)p, sizeof (*p))) < 0)
        {
            /*RRC_DEBUG("Socket error");*/
            close_wrapper(sockfd);
            continue;
        }
        break;
    }

    if(p == NULL)
    {
        /*RRC_DEBUG("Failed to bind to Socket");*/
        return(RRC_FAILURE);
    }

    freesockaddr(servinfo);
    /* Shall we do this ..???  - Stop */

    self_addr.sin_port = 1234; /* RRC SELF PORT */
    self_addr.sin_addr.s_addr = 0x7F000001; /* RRC SELF IP: 127.0.0.1*/

    l3_memcpy_wrapper(IpcDataList[7], &(self_addr.sin_addr.s_addr), sizeof (U32));
    IpcDataList[7][4] =  self_addr.sin_port;

    rrc_make_addr ((U8 *)IpcDataList[7],
                     &address,
                     htonl_wrapper(INADDR_ANY));

    while (1)
    {
        tempSd = rrc_make_server (&address);
        if (INVALID_FD == tempSd)
        {
        }
        else
        {
            break;
        }
        sleep_wrapper ((S32)1); /* Wait for Some Time before retrying*/
    }
    fflush_wrapper(stdout);

    RRC_ASSERT (INVALID_FD != tempSd);
    return (tempSd);
}


/****************************************************************************
 * Function Name  : rrc_open
 * Input          : void *data_p
 * Output         : none
 * Returns        : none
 * Description    : This function opens connections
 ****************************************************************************/
void *rrc_open (void *data_p)
{
    static Listener *listener_p = PNULL;
    sockaddr        from;
    socklen_t       fromLen = 0;
    S32             serverFd = INVALID_FD;

    listener_p = (Listener *)qvSysMalloc(sizeof(Listener));

    RRC_ASSERT (PNULL != listener_p);

    listener_p->sdRrc = INVALID_FD;

    serverFd = rrc_make_conn ();

    return ((void *)listener_p);
}

/****************************************************************************
 * Function Name  : rrc_close
 * Input          : void *listenerData_p
 * Output         : none
 * Returns        : none
 * Description    : closes all socket connections
 ****************************************************************************/
void rrc_close (void *listenerData_p)
{
    Listener *listener_p = PNULL;

    RRC_ASSERT (PNULL != listenerData_p);
    listener_p = (Listener *)listenerData_p;

    close_wrapper (listener_p->sdRrc);

    qvSysFree (listenerData_p);
}
