/******************************************************************************
*
*   FILE NAME  : sock_tools.c
*
*   DESCRIPTION: functions for transferring messages via UDP/SCTP sockets.
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc. Ukraine
*
******************************************************************************/

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "simLogger.h"

#include "sock_tools.h"
#include "userUtils.h"

#include "socket_udp_tools.h"
#include "socket_sctp_tools.h"

/******************************************************************************
 * Local definitions
 ******************************************************************************/

#define SIM_MODULE_HOST_NAME "127.0.0.1"
#define CLIENT_MODULE_HOST_NAME "127.0.0.1"

#define SIM_UDP_MODULE_PORT  5555
#define SIM_SCTP_MODULE_PORT 3223
#define CLIENT_MODULE_PORT 3333

/******************************************************************************
 * Local types
 ******************************************************************************/

/*from rrc_s1apOam_intf.h*/
#define SIM_MAX_IP_ADDRESS_LENGTH           15

typedef struct
{
    UInt8 ip_addr[SIM_MAX_IP_ADDRESS_LENGTH]; /*^ M,0,OCTET_STRING,FIXED ^*/
}sim_ip_addr_t;




/******************************************************************************
 * Local variables
 ******************************************************************************/
#ifdef LKSCTP

static fd_set sctp_fd;

#endif
#ifdef ARICENT_SCTP_IPR
static sctp_fd_set_st sctp_fd;
static UInt32 sctp_init_stack_flag = SIM_FALSE;


/******************************************************************************
 * External definitions
 ******************************************************************************/

/******************************************************************************
 Local functions definitions
 ******************************************************************************/

/******************************************************************************
 *   Function Name     : sim_l3_sctp_init_stack (copied from sctp_wrappers)
 *   Input             : addr         -      Pointer to array of IP addresses.
 *                       no_ip_addr   -      Number of Ip addresses.
 *   Output            : None.
 *   Returns           : Returns 0 on Error and 1 on Success.
 *   Description       : This Function Initialises the Aricent's SCTP stack.
 ******************************************************************************/
static UInt32 sim_l3_sctp_init_stack(
			sctp_Boolean_t cspl_already_intialised,
            sim_ip_addr_t *addr,
            UInt32 no_of_ip_addr);

#endif
/************************************************************************************
 * FUNCTION NAME    : ReadSocketsConfig
 * DESCRIPTION      : Reads sockets config (host names and port numbers) from config file
 * PARAMATERS       : UChar8 *fn - file name of config file
 * RETURN VALUE     :
 ************************************************************************************/
void ReadSocketsConfig(UChar8 *fn);

/* Performs initialization of the sockets engine. */
static UInt8 InitSocketSys(void);

/******************************************************************************
 * Local variables
 ******************************************************************************/

/* Client data*/
/*      UDP TX data*/
static UChar8 udpClientHostName[HOSTNAME_MAX_LEN] = CLIENT_MODULE_HOST_NAME;
static UInt16 udpClientPortTx = CLIENT_MODULE_PORT;
static struct sockaddr_in udpClientAddrTx;
static SocketType udpClientSocketTxFd = SOCKET_ERROR;

/* Sim data*/
/*      UDP RX data*/
static UChar8 udpSimHostName[HOSTNAME_MAX_LEN] = SIM_MODULE_HOST_NAME;
static UInt16 udpSimPortRx = SIM_UDP_MODULE_PORT;
static struct sockaddr_in udpSimAddrRx;
static SocketType udpSimSocketRxFd = SOCKET_ERROR;

/* Sim data*/
/*      SCTP data*/
static UChar8 sctpSimHostName[HOSTNAME_MAX_LEN] = SIM_MODULE_HOST_NAME;
static UInt16 sctpSimPort = SIM_SCTP_MODULE_PORT;
static struct sockaddr_in sctpSimAddr;
SocketType sctpSimSocketFd = SOCKET_ERROR;

static UInt32 sctp_no_of_ip_addr = 1;

/************************************************************************************
 Functions implementation
 ************************************************************************************/

/****************************************************************************
 * Function Name  : InitSockets
   PARAMATERS     : UChar8 *fn - file name of config file
 * Outputs        :
 * Returns        : Returns 0 or SOCKET_ERROR
 * Variables      :
 * Description    : init sockets unit.
 ****************************************************************************/
SocketType InitSockets(UChar8 *fn)
{
    ReadSocketsConfig(fn);

    if ( SIM_FALSE == InitSocketSys() )
    {
        return SOCKET_ERROR;
    }

    SIM_LOG( SIM_LOG_INFO, "SOCKET: Init TX socket...");
    udpClientSocketTxFd = udpSocketInitTx(udpClientHostName, &udpClientAddrTx, udpClientPortTx, SIM_FALSE );
    if ( SOCKET_ERROR == udpClientSocketTxFd )
    {
        return SOCKET_ERROR;
    }
    SIM_LOG(SIM_LOG_INFO, "SOCKET: udpClientSocketTxFd :%d", udpClientSocketTxFd);

    SIM_LOG( SIM_LOG_INFO, "SOCKET: Init RX socket...");
    udpSimSocketRxFd = udpSocketInitRx( udpSimHostName, &udpSimAddrRx, udpSimPortRx, SIM_FALSE );
    if ( SOCKET_ERROR == udpSimSocketRxFd )
    {
        return SOCKET_ERROR;
    }
    SIM_LOG(SIM_LOG_INFO, "SOCKET: udpSimSocketRxFd :%d", udpSimSocketRxFd);
#ifdef SSIT_SETUP
    SIM_LOG( SIM_LOG_INFO, "SOCKET: Init SCTP socket...");
    /*SPR 20633 Fix Start*/
    sctpSimSocketFd = sctpSocketInit( sctpSimHostName,sctpSimPort, SIM_FALSE );
    /*SPR 20633 Fix End*/
    if ( SOCKET_ERROR == sctpSimSocketFd )
    {
        return SOCKET_ERROR;
    }
    SIM_LOG(SIM_LOG_INFO, "SOCKET: sctpSimSocketFd :%d", sctpSimSocketFd);
#endif
    return 0;
}

/************************************************************************************
 * FUNCTION NAME    : CloseSockets
 * DESCRIPTION      : Closes sockets and deinitializes socket engine.
                      No other socket functions should be called after call to CloseSockets.
 * PARAMATERS       :
 * RETURN VALUE     :
 ************************************************************************************/
void CloseSockets(void)
{
    udpSocketClose(&udpClientSocketTxFd);
    udpSocketClose(&udpSimSocketRxFd);
    sctpSocketClose(&sctpSimSocketFd);

#ifdef WIN32
    WSACleanup();
#endif
}

/************************************************************************************
 * FUNCTION NAME    : ReadFromSocket
 * DESCRIPTION      : Reads message from socket
 * PARAMATERS       : sock_type,
 *                    buf - data from socket will be placed here. Should have SOCKET_BUFFER_SIZE length.
 *                    SInt32 timeout
 * RETURN VALUE     : Length of read message.
                      If there is no message it returns 0 (no wait for a message).
 ************************************************************************************/
UInt32 ReadFromSocket(sim_socket_type_et sock_type, UInt8 *buf, SInt32 timeout)
{
    Int bytesRead = 0;

    if (SIM_SOCKET_TYPE_UDP == sock_type)
    {
        bytesRead = udpSocketRead(udpSimSocketRxFd,buf,timeout);
    }
    else if (SIM_SOCKET_TYPE_SCTP == sock_type)
    {
        bytesRead = sctpSocketRead(buf,timeout);
    }
    else
    {
        SIM_LOG(SIM_LOG_CRITICAL, "SOCKET: invalid socket type");
        abort();
    }

    return bytesRead;
}

/************************************************************************************
 * FUNCTION NAME    : WriteToSocket
 * DESCRIPTION      : Writes information from buffer with length bufferLen to socket
 * PARAMATERS       : sock_type,buffer,bufferLen
 * RETURN VALUE     : Number of really transferred bytes. If OK should be == bufferLen
 ************************************************************************************/
UInt32 WriteToSocket(sim_socket_type_et sock_type, UInt8 *buffer, UInt32 bufferLen)
{
    Int res_len = 0;

    if (SIM_SOCKET_TYPE_UDP == sock_type)
    {
        res_len = udpSocketWrite(udpClientSocketTxFd, (struct sockaddr*)&udpClientAddrTx, buffer, bufferLen);
    }
    else if (SIM_SOCKET_TYPE_SCTP == sock_type)
    {
        res_len = sctpSocketWrite(buffer, bufferLen);
    }
    else
    {
        SIM_LOG(SIM_LOG_CRITICAL, "SOCKET: invalid socket type");
        abort();
    }
    return res_len;
}

/************************************************************************************
 * FUNCTION NAME    : WaitForConnectionOnSocket
 * DESCRIPTION      : Waits and accepts connection on socket
 * PARAMATERS       : sock_type
 * RETURN VALUE     : Returns 0 or SOCKET_ERROR
 ************************************************************************************/
UInt32 WaitForConnectionOnSocket(sim_socket_type_et sock_type)
{
    Int res = 0;
    if (SIM_SOCKET_TYPE_SCTP == sock_type)
    {
        res = sctpSocketListenAndAccept(sctpSimSocketFd);
    }
    else
    {
        SIM_LOG(SIM_LOG_CRITICAL, "SOCKET: invalid socket type");
        abort();
    }

    return res;
}

/************************************************************************************
 Local functions implemenation
 ************************************************************************************/

#ifdef ARICENT_SCTP_IPR

/******************************************************************************
 *   Function Name     : sim_l3_sctp_init_stack (copied from sctp_wrappers)
 *   Input             : addr         -      Pointer to array of IP addresses.
 *                       no_ip_addr   -      Number of Ip addresses.
 *   Output            : None.
 *   Returns           : Returns 0 on Error and 1 on Success.
 *   Description       : This Function Initialises the Aricent's SCTP stack.
 ******************************************************************************/
static UInt32 sim_l3_sctp_init_stack(
			sctp_Boolean_t cspl_already_intialised,
            sim_ip_addr_t *addr,
            UInt32 no_of_ip_addr)
{

    sctp_error_t                 sctpErrorCode = E_NOERROR;
    UInt32 counter = 0;
    sctp_init_options_st         sctpInitOptions;
    if (SIM_TRUE == sctp_init_stack_flag)
    {
        return SCTP_SUCCESS;
    }
    SCTP_FD_ZERO(&sctp_fd);

	if (SCTP_FAILURE == sctp_set_stack_envt(cspl_already_intialised, PNULL))
	{
		SIM_LOG(SIM_LOG_WARNING," SCTP Set Env Return Failure\n");
                return SCTP_FAILURE;
	}


	SIM_LOG(SIM_LOG_INFO," SCTP Set Env set\n");
    /* Setting all the Init options */
    sctpInitOptions.capability.ecn_capable            = 0;
    sctpInitOptions.capability.pmtu_capable           = 0;
    sctpInitOptions.capability.nat_capable            = 0;
    sctpInitOptions.capability.ack_policy             = 2;
    sctpInitOptions.capability.stale_cookie_policy    = 0;
    sctpInitOptions.capability.stack_type             = SCTP_OVER_INET;

    sctpInitOptions.max_appl                          = 10;
    sctpInitOptions.max_endpoint                      = 190;
    sctpInitOptions.max_tcb                           = 100;
    sctpInitOptions.rand_seed                         = 1;
    sctpInitOptions.no_of_local_addrs                 = no_of_ip_addr;

    for (counter = 0; counter < no_of_ip_addr; counter++)
    {

        sctpInitOptions.list_of_local_addrs[counter].family = SCTP_AF_INET;
        memset(&sctpInitOptions.list_of_local_addrs[counter].ip.v4.addr,0,4);
        SIM_LOG(SIM_LOG_INFO,"IP addr : %s\n",addr[counter].ip_addr);
        sctpInitOptions.list_of_local_addrs[counter].ip.v4.addr = htonl(inet_addr((char*)(addr[counter].ip_addr)));
    }

    sctpInitOptions.MaxPerAssoc.streams        = 10;
    sctpInitOptions.MaxPerAssoc.min_tx_buffers = 50;
    sctpInitOptions.MaxPerAssoc.med_tx_buffers = 200;
    sctpInitOptions.MaxPerAssoc.med_tx_buffers = 200;
    sctpInitOptions.MaxPerAssoc.max_tx_buffers = 20;
    sctpInitOptions.MaxPerAssoc.min_rx_buffers = 150;
    sctpInitOptions.MaxPerAssoc.med_rx_buffers = 70;
    sctpInitOptions.MaxPerAssoc.max_rx_buffers = 20;

    sctpInitOptions.min_reserve_buffers = 10;
    sctpInitOptions.med_reserve_buffers = 10;
    sctpInitOptions.max_reserve_buffers = 10;

    sctpInitOptions.standard_udp_port = 0 ;

    sctpInitOptions.checksum = SCTP_CRC32;

    sctpInitOptions.hostname = PNULL;

    if( SCTP_SUCCESS == sctp_api_init_stack(&sctpInitOptions, &sctpErrorCode))
    {
    	sctp_init_stack_flag = SIM_TRUE;
        return SCTP_SUCCESS;
    }
    else
    {
    	SIM_LOG(SIM_LOG_WARNING,"sctp_api_init_stack Err.code=%i\n",sctpErrorCode);
        return SCTP_FAILURE;
    }
}

#endif

/************************************************************************************
 * FUNCTION NAME    : InitSocketSys
 * DESCRIPTION      : Performs initialization of the sockets engine. Should be called
                      on program startup before using other socket functions
 * PARAMATERS       :
 * RETURN VALUE     : status (SUCCESS/FAIL)
 ************************************************************************************/
UInt8 InitSocketSys(void)
{
#ifdef WIN32
    WSADATA wsaData;

    /*Init socket lib*/
    if(WSAStartup(/*version 2.0*/ MAKEWORD(2,0 ), &wsaData)!=0)
    {
        SIM_LOG(SIM_LOG_CRITICAL,"InitSockets: error during WSAStartup");
        return SIM_FALSE;
    }
#endif

#ifdef SCTP
#ifdef ARICENT_SCTP_IPR
    if (SCTP_FAILURE == sim_l3_sctp_init_stack(0,(sim_ip_addr_t *)sctpSimHostName,sctp_no_of_ip_addr))
    {

        SIM_LOG(SIM_LOG_CRITICAL,"Error returned from l3_sctp_init_stack: %d\n",sk_errno);
    }
    else
    {
        SIM_LOG(SIM_LOG_INFO," SCTP INIT Return Success\n");
    }

#if 0
    /* Enabling SCTP stack traces for debugging**/
    sctp_trc_et    ttype = SCTP_ALL_TRC;
    sctp_error_t   ecode = TOTAL_ERRORS;

    if (SCTP_FAILURE== sctp_api_enable_assoc_trc(simSocketRxFd,
                ttype,&ecode))
    {
        printf("ERROR In setting assoc trace\n");
        abort();
    }
    /* Enabling trace**/
#endif
#endif
#endif
    return SIM_TRUE;
}

/************************************************************************************
 * FUNCTION NAME    : ReadSocketsConfig
 * DESCRIPTION      : Reads sockets config (host names and port numbers) from config file
 * PARAMATERS       : UChar8 *fn - file name of config file
 * RETURN VALUE     :
 ************************************************************************************/
void ReadSocketsConfig(UChar8 *fn)
{
    Int i=0;
    UChar8 **strings=userGetListStringsFromCfgFile(fn);
    UChar8 *val=PNULL;
    if(PNULL!=strings)
    {
        SIM_LOG(SIM_LOG_DUMP,"Reading sockets configuration from '%s'",fn);
        for(i=0; strings[i]; i++)
        {
            val=userIniKeyPresent("UDP_CLIENT_HOST_NAME",strings[i]);
            if(PNULL!=val)
            {
                memcpy(udpClientHostName, val, strlen(val));
                udpClientHostName[strlen(val)] = 0;
            }
            val=userIniKeyPresent("UDP_CLIENT_HOST_PORT",strings[i]);
            if(PNULL!=val)
            {
                udpClientPortTx = atoi(val);
            }
            val=userIniKeyPresent("UDP_SIM_HOST_NAME",strings[i]);
            if(PNULL!=val)
            {
                memcpy(udpSimHostName, val, strlen(val));
                udpSimHostName[strlen(val)] = 0;
            }
            val=userIniKeyPresent("UDP_SIM_PORT",strings[i]);
            if(PNULL!=val)
            {
                udpSimPortRx = atoi(val);
            }
            val=userIniKeyPresent("SCTP_SIM_HOST_NAME",strings[i]);
            if(PNULL!=val)
            {
                memcpy(sctpSimHostName, val, strlen(val));
                sctpSimHostName[strlen(val)] = 0;
            }
            val=userIniKeyPresent("SCTP_SIM_PORT",strings[i]);
            if(PNULL!=val)
            {
                sctpSimPort = atoi(val);
            }
        }
        userFreeListStrings(strings);
    }
}




