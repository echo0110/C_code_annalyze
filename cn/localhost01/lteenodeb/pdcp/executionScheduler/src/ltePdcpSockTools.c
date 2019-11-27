/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: ltePdcpSockTools.c,v 1.1.1.1.12.2.2.4 2010/10/25 11:02:35 gur11835 Exp $
 *
 ******************************************************************************
 *
 *  File Description : Functions for transferring messages via UDP sockets. 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: ltePdcpSockTools.c,v $
 * Revision 1.1.1.1.12.2.2.4  2010/10/25 11:02:35  gur11835
 * warning removed
 *
 * Revision 1.1.1.1.12.2.2.3  2010/10/25 08:28:11  gur21010
 * Warnings Removed
 *
 * Revision 1.1.1.1.12.2.2.2  2010/10/07 13:28:13  gur23054
 * Fix for SPR 713
 *
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:07  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:11:41  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.7.2.5.2.1  2009/09/30 12:33:38  gur20439
 * oam and rrc IP seperated for mwc demo.
 *
 * Revision 1.7.2.5  2009/07/16 05:08:54  gur19140
 * changes during dl flow fixes
 *
 * Revision 1.7.2.4  2009/07/09 02:37:48  gur19479
 * updated for port issues
 *
 * Revision 1.7.2.3  2009/07/03 14:08:36  gur19479
 * removed unused code
 *
 * Revision 1.7.2.2  2009/06/19 14:28:26  gur19836
 * Changes Done for Execution Scheduler
 *
 * Revision 1.7.2.1  2009/06/09 11:27:28  gur19140
 * first wave of Integration
 *
 * Revision 1.7  2009/05/28 13:46:27  gur19140
 * *** empty log message ***
 *
 * Revision 1.6  2009/05/21 07:06:09  gur19140
 * memory leak fixes
 *
 * Revision 1.5  2009/05/20 13:15:13  gur20548
 * Changes header files name
 *
 * Revision 1.4  2009/05/14 11:04:58  gur19836
 * CR Comments Incorporated
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "ltePdcpEsLteFramework.h"
#include <ltePdcpLteDefs.h>
#include "ltePdcpSockTools.h"
#include "lteLayer2CommanTypes.h"
#include "logging.h"
#include "alarm.h"
#include "ltePdcpErrors.h"

/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* returns UDP port number for given moduleId */
UInt16 GetModulePortNum(Int moduleId);
/* returns host name (string) for given moduleId */
Char8 *GetModuleHostName(Int moduleId);

/* returns RX socket file descriptor for given moduleId */
static SocketType GetRxSocketFD(Int moduleId);
/* Stores RX socket file descriptor for given moduleId */
static void SetRxSocketFD(Int moduleId,SocketType fd);
/* returns TX socket file descriptor for given moduleId */
static SocketType GetTxSocketFD(Int moduleId);
/* Stores TX socket file descriptor for given moduleId */
static void SetTxSocketFD(Int moduleId,SocketType fd);
/* returns blocking mode for given moduleId */
/*SPR 21366 +*/
UInt8 GetModuleBlockingMode(Int moduleId);
/*SPR 21366 -*/

/* returns pointer to RX socket address structure (sockaddr_in)
   for given moduleId */
static SockAddr *GetRxSocketAddr(Int moduleId);
/* returns pointer to TX socket address structure (sockaddr_in)
   for given moduleId */
static SockAddr *GetTxSocketAddr(Int moduleId);

/* intialize UDP socket with address/port */
static SocketType InitSocketAddress(void *host_name,SockAddr *s,
                                    UInt16 port,UInt8 isBlocking);

/******************************************************************************
 * Private Constants
 *****************************************************************************/
#define RRC_MODULE_HOST_NAME "127.0.0.1"
#define OAM_MODULE_HOST_NAME "127.0.0.1"
#define PDCP_MODULE_HOST_NAME "127.0.0.1"
/*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF
#define GTPU_MODULE_HOST_NAME "127.0.0.1"
#endif
/*SPR 21188 -*/
/*SPR 20863 FIX*/
#define RRM_MODULE_HOST_NAME "127.0.0.1"
#define RRM_MODULE_PORT 3222
/*SPR 20863 FIX*/

#define RRC_MODULE_PORT 3211
#define OAM_MODULE_PORT 3212
#define PDCP_MODULE_PORT 3210
/*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF
#define GTPU_MODULE_PORT 3213
#endif
/*SPR 21188 +*/
/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* SPR 15909 fix start */ 
extern tickType_t pdcpCurrentTime_g;
/* SPR 15909 fix end */
#ifdef MAC_AUT_TEST
UInt8 *pdcpPhyBuff_g;
UInt32 pdcpMsgLen_g;
extern UInt8 macAutFlag ;
#endif

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
static Char8 rrcModuleHostName[HOSTNAME_MAX_LEN]=RRC_MODULE_HOST_NAME;
static Char8 oamModuleHostName[HOSTNAME_MAX_LEN]=OAM_MODULE_HOST_NAME;
static Char8 pdcpModuleHostName[HOSTNAME_MAX_LEN]=PDCP_MODULE_HOST_NAME;
/*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF
static Char8 gtpuModuleHostName[HOSTNAME_MAX_LEN]=GTPU_MODULE_HOST_NAME;
#endif
/*SPR 21188 -*/
/*SPR 20863 FIX*/
static Char8 rrmModuleHostName[HOSTNAME_MAX_LEN]=RRM_MODULE_HOST_NAME;
static UInt16 rrmModulePort=RRM_MODULE_PORT;
static SocketType rrmSocketTx=SOCKET_ERROR;
/*SPR 20863 FIX*/

static UInt16 rrcModulePort=RRC_MODULE_PORT;
static UInt16 oamModulePort=OAM_MODULE_PORT;
static UInt16 pdcpModulePort=PDCP_MODULE_PORT;
/*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF
static UInt16 gtpuModulePort=GTPU_MODULE_PORT;
/*This variable holds the PDCP RX port for receiving data from GTPU*/
UInt16 pdcpGtpudModulePort=PDCP_MODULE_PORT;
#endif
/*SPR 21188 -*/
static SockAddr rrcAddrTx; 
static SockAddr rrcAddrRx;
static SockAddr oamAddrTx; 
static SockAddr oamAddrRx;
static SockAddr pdcpAddrTx; 
static SockAddr pdcpAddrRx;
/*SPR 20863 FIX*/
static SockAddr rrmAddrTx;
/*SPR 20863 FIX*/
/*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF
static SockAddr gtpuAddrTx;
#endif
/*SPR 21188 -*/
/* socket file descriptors */
static SocketType rrcSocketTx=SOCKET_ERROR;
static SocketType rrcSocketRx=SOCKET_ERROR;
static SocketType oamSocketTx=SOCKET_ERROR;
static SocketType oamSocketRx=SOCKET_ERROR;
SocketType pdcpSocketRx=SOCKET_ERROR;
/*SPR 21366 +*/
SocketType pdcpGtpuRx=SOCKET_ERROR;
/*SPR 21366 -*/
static SocketType pdcpSocketTx=SOCKET_ERROR;

/*block mode status*/
static UInt8 rrcSocketRxBlock=LTE_FALSE;
static UInt8 oamSocketRxBlock=LTE_FALSE;
static UInt8 pdcpSocketRxBlock=LTE_FALSE;
/*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF
static SocketType gtpuSocketTx=SOCKET_ERROR;
static SocketType gtpuSocketRx=SOCKET_ERROR;
#endif
/*SPR 21188 -*/

/*****************************************************************************
 * Function Name  : CloseSockets 
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : Closes sockets and deinitializes socket engine. No other
 *                  socket functions should be called after call to CloseSockets.
 *****************************************************************************/
void CloseSockets(void)
{
    CloseSocket(RRC_MODULE_ID);
    CloseSocket(OAM_MODULE_ID);
    CloseSocket(PDCP_MODULE_ID);
    /*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF
    CloseSocket(PACKET_RELAY_MODULE_ID);
#endif
    /*SPR 21188 -*/
}


/*****************************************************************************
 * Function Name  : configPdcpCommPort 
 * Inputs         : pdcpModulePort_l - PDCP Module Port
 *                  oamModulePort_l - OAM Module Port
 *                  rrcModulePort_l - RRC Module Port
 *                  pdcpModuleCmdPort_l - PDCP Module Cmd Port
 *                  oamRrcIP - OAM RRC IP
 *                  l2 IP - layer 2 IP
 * Outputs        : None
 * Returns        : None
 * Description    : Setting the pdcp comminication port.
 *****************************************************************************/
/*SPR 20863 FIX*/
void configPdcpCommPort(UInt16 pdcpModulePort_l,
        UInt16 oamModulePort_l, UInt16 rrcModulePort_l,
        Char8 *oamIp_p, Char8 *rrcIp_p,Char8 *pdcpIp_p, Char8 *rrmIp_p, UInt16 rrmModulePort_l
        /*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF
        ,Char8 *gtpuIp_p,UInt16 gtpuRxPort,UInt16 pdcpGtpuRxPort
#endif
        /*SPR 21188 -*/
        )
{
    pdcpModulePort = pdcpModulePort_l;
    oamModulePort = oamModulePort_l;
    rrcModulePort = rrcModulePort_l;
    /*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF
    gtpuModulePort = gtpuRxPort;
    pdcpGtpudModulePort = pdcpGtpuRxPort;
#endif
    /*SPR 21188 -*/
    rrmModulePort = rrmModulePort_l;
    configPdcpCommHostName(pdcpIp_p, oamIp_p, rrcIp_p, rrmIp_p
            /*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF
            ,gtpuIp_p
#endif
            /*SPR 21188 -*/
            );
}

/*****************************************************************************
 * Function Name  : configPdcpCommHostName 
 * Inputs         : pdcpModuleHN - PDCP Module Host Name
 *                  oamModuleHN - OAM Module Host Name
 *                  rrcModuleHN - RRC Module Host Name
 * Outputs        : None
 * Returns        : None
 * Description    : Setting the pdcp comminication Host Name.
 *****************************************************************************/
/*SPR 20863 FIX*/
void configPdcpCommHostName(Char8* pdcpModuleHN, Char8* oamModuleHN, 
        Char8* rrcModuleHN, Char8* rrmModuleHN
        /*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF
        ,Char8* gtpuModuleHN
#endif
        /*SPR 21188 -*/
        )
{
    if(PNULL != pdcpModuleHN){
        /* SPR 3444 changes start */
        strNCpy( pdcpModuleHostName, pdcpModuleHN, HOSTNAME_MAX_LEN);
        /* SPR 3444 changes end */
    }
    if(PNULL != oamModuleHN){
        /* SPR 3444 changes start */
        strNCpy( oamModuleHostName, oamModuleHN, HOSTNAME_MAX_LEN);
        /* SPR 3444 changes end */
    }
    if(PNULL != rrcModuleHN){
        /* SPR 3444 changes start */
        strNCpy( rrcModuleHostName,rrcModuleHN, HOSTNAME_MAX_LEN);
        /* SPR 3444 changes end */
    }
    
    if(PNULL != rrmModuleHN){
        /* SPR 3444 changes start */
        strNCpy( rrmModuleHostName,rrmModuleHN, HOSTNAME_MAX_LEN);
        /* SPR 3444 changes end */
    }
    /*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF
    if(PNULL != gtpuModuleHN){
        /* SPR 3444 changes start */
        strNCpy( gtpuModuleHostName,gtpuModuleHN, HOSTNAME_MAX_LEN);
        /* SPR 3444 changes end */
    }
#endif
    /*SPR 21188 -*/
}

/*SPR 20863 FIX*/
/*****************************************************************************
 * Function Name  : ReadFromSocket 
 * Inputs         : buf - data from socket will be placed here. Should have
 *                        RX_BUFFER_SIZE length.
 *                  moduleId - determines module to read from RX socket
 * Outputs        : None
 * Returns        : Length of read message. If there is no message it returns 0
 *                  (no wait for a message). If message length != TLV header
 *                  length it returns 0.
 * Description    : Reads message from RX UDP socket for given moduleId.
 *****************************************************************************/
UInt32 ReadFromSocket(UInt8 *buf, UInt16 moduleId)
{
    Int bytesRead = 0;
    SocketType fd = GetRxSocketFD(moduleId);

    if ( SOCKET_ERROR == fd )
    {
        return 0; /* socket is not initialized */
    }

    if(LTE_TRUE==GetModuleBlockingMode(moduleId))
    {
        /* SPR 6949 Fix Start */
        bytesRead = recvfrom_wrapper(fd, buf, SOCKET_BUFFER_SIZE, 
                          SOCKET_FLAGS_WAIT, PNULL, 0);
        /* SPR 6949 Fix End */
    }
    else
    {
        /* SPR 6949 Fix Start */
        bytesRead = recvfrom_wrapper(fd, buf, SOCKET_BUFFER_SIZE,
                          SOCKET_FLAGS, PNULL, 0);
        /* SPR 6949 Fix End */
    }

    if( bytesRead <= 0)
    {
        LOG_PDCP_MSG( L2_SOCKET_RECVFROM_FAIL_ID, LOGWARNING, PDCP_RRCOAM,
                 pdcpCurrentTime_g, __LINE__, bytesRead, 0, 0, 0, 0,0,
                 __FILE__, __func__ );
        return 0; /*if no message, bytesRead will be -1 */
    }
    return bytesRead;
}

/*****************************************************************************
 * Function Name  : WriteToSocket 
 * Inputs         : buffer - Ptr to buffer to be transmit on socket
 *                  bufferLen - Length of buffer to be sent
 *                  moduleId - determines module to write to TX socket
 * Outputs        : None
 * Returns        : Number of really transferred bytes. If OK should be
 *                  equal to bufferLen
 * Description    : Writes information from buffer with length bufferLen to
 *                  UDP socket.
 *****************************************************************************/
UInt32 WriteToSocket(UInt8 *buffer, UInt32 bufferLen, UInt16 moduleId)
{

#ifdef MAC_AUT_TEST
    if (macAutFlag)
    {
    /** CA Changes Start **/
        pdcpPhyBuff_g = MEM_ALLOC_AUT(bufferLen+1);

        MEM_COPY_AUT(pdcpPhyBuff_g,buffer,bufferLen);
    /** CA Changes End **/
        pdcpMsgLen_g = bufferLen;
        return pdcpMsgLen_g;
    }
    else
#endif
    {
        SocketType fd = GetTxSocketFD(moduleId);
        SockAddr *pSockAddr=GetTxSocketAddr(moduleId);
        if(fd != SOCKET_ERROR )
        {
            Int res_len = sendto_wrapper(fd,buffer,bufferLen,0,
                    pSockAddr,sizeof(SockAddr));
            if(res_len != (SInt64)bufferLen)
            {
                LOG_PDCP_MSG( L2_SOCKET_SENDTO_FAIL_ID, LOGWARNING, PDCP_RRCOAM,
                        pdcpCurrentTime_g, __LINE__, res_len, 0, 0, 0, 0,0,
                        __FILE__, __func__ );
                ALARM_MSG (PDCP_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
            }
            return res_len;
        }
        return PDCP_FAIL;
    }
}

/*****************************************************************************
 * Function Name  : GetModulePortNum 
 * Inputs         : moduleId - Module Id for which port number required
 * Outputs        : None
 * Returns        : port Number of the module 
 * Description    : Returns UDP port number for given moduleId.
 *****************************************************************************/
UInt16 GetModulePortNum(Int moduleId)
{
    switch(moduleId)
    {
        case RRC_MODULE_ID:
            return rrcModulePort;
        case OAM_MODULE_ID:
            return oamModulePort;
        case PDCP_MODULE_ID:
            return pdcpModulePort;
        /*SPR 20863 FIX*/
        case RRM_MODULE_ID:
            return rrmModulePort;
        /*SPR 20863 FIX*/
       /*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF
        case PACKET_RELAY_MODULE_ID:
            return gtpuModulePort;
#endif
       /*SPR 21188 -*/
        default:
        {
            LOG_PDCP_MSG( PDCP_INVALID_MODULE_ID, LOGFATAL, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, moduleId, 0, 0, 0, 0,0,
                 __FILE__, __func__ );
            return 0;
        }
    }
}

/*****************************************************************************
 * Function Name  : GetModuleHostName 
 * Inputs         : moduleId - Module Id for which host name required
 * Outputs        : None
 * Returns        : Ptr to Host Name of the module 
 * Description    : Returns host name (string) for given moduleId.
 *****************************************************************************/
Char8 *GetModuleHostName(Int moduleId)
{
    switch(moduleId)
    {
        case RRC_MODULE_ID:
            return rrcModuleHostName;
        case OAM_MODULE_ID:
            return oamModuleHostName;
        case PDCP_MODULE_ID:
            return pdcpModuleHostName;
        /*SPR 20863 FIX*/
        case RRM_MODULE_ID:
            return rrmModuleHostName;
        /*SPR 20863 FIX*/
       /*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF
        case PACKET_RELAY_MODULE_ID:
            return gtpuModuleHostName;
#endif
       /*SPR 21188 -*/

        default:
        {
            LOG_PDCP_MSG( PDCP_INVALID_MODULE_ID, LOGFATAL, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, moduleId, 0, 0, 0, 0,0,
                 __FILE__, __func__ );
            return PNULL;
        }
    }
}

/*****************************************************************************
 * Function Name  : GetRxSocketFD 
 * Inputs         : moduleId - Module Id for which socket type required
 * Outputs        : None
 * Returns        : SocketType 
 * Description    : Returns RX socket file descriptor for given moduleId.
 *****************************************************************************/
static SocketType GetRxSocketFD(Int moduleId)
{
    switch(moduleId)
    {
        case RRC_MODULE_ID:
            return rrcSocketRx;
        case OAM_MODULE_ID:
            return oamSocketRx;
        case PDCP_MODULE_ID:
            return pdcpSocketRx;
#ifdef PDCP_GTPU_INTF
        case PACKET_RELAY_MODULE_ID:
            return gtpuSocketRx;
#endif
        default:
        {
            LOG_PDCP_MSG( PDCP_INVALID_MODULE_ID, LOGFATAL, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, moduleId, 0, 0, 0, 0,0,
                 __FILE__, __func__ );
            return SOCKET_ERROR;
        }
    }
}

/*****************************************************************************
 * Function Name  : SetRxSocketFD 
 * Inputs         : moduleId - Module Id 
 *                  fd - Socket File Descriptor
 * Outputs        : None
 * Returns        : None 
 * Description    : Stores RX socket file descriptor for given moduleId.
 *****************************************************************************/
static void SetRxSocketFD(Int moduleId,SocketType fd)
{
    switch(moduleId)
    {
        case RRC_MODULE_ID:
            rrcSocketRx=fd;
            break;
        case OAM_MODULE_ID:
            oamSocketRx=fd;
            break;
        case PDCP_MODULE_ID:
            pdcpSocketRx=fd;
            break;
        default:
            LOG_PDCP_MSG( PDCP_INVALID_MODULE_ID, LOGFATAL, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, moduleId, 0, 0, 0, 0,0,
                 __FILE__, __func__ );
    }
}

/*****************************************************************************
 * Function Name  : GetTxSocketFD 
 * Inputs         : moduleId - Module Id for which socket type required
 * Outputs        : None
 * Returns        : SocketType 
 * Description    : Returns TX socket file descriptor for given moduleId.
 *****************************************************************************/
static SocketType GetTxSocketFD(Int moduleId)
{
    switch(moduleId)
    {
        case PDCP_MODULE_ID:
            return pdcpSocketTx;
        case RRC_MODULE_ID:
            return rrcSocketTx;
        case OAM_MODULE_ID:
            return oamSocketTx;
        /*SPR 20863 FIX*/
        case RRM_MODULE_ID:
            return rrmSocketTx;
        /*SPR 20863 FIX*/
       /*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF
        case PACKET_RELAY_MODULE_ID:
            return gtpuSocketTx;
#endif
       /*SPR 21188 -*/

        default:
        {
            LOG_PDCP_MSG( PDCP_INVALID_MODULE_ID, LOGFATAL, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, moduleId, 0, 0, 0, 0,0,
                 __FILE__, __func__ );
            return SOCKET_ERROR;
        }
    }
}

/*****************************************************************************
 * Function Name  : SetTxSocketFD 
 * Inputs         : moduleId - Module Id 
 *                  fd - Socket File Descriptor
 * Outputs        : None
 * Returns        : None 
 * Description    : Stores TX socket file descriptor for given moduleId.
 *****************************************************************************/
static void SetTxSocketFD(Int moduleId,SocketType fd)
{
    switch(moduleId)
    {
        case PDCP_MODULE_ID:
            pdcpSocketTx=fd;
            break;
        case RRC_MODULE_ID:
            rrcSocketTx=fd;
            break;
        case OAM_MODULE_ID:
            oamSocketTx=fd;
            break;
        /*SPR 20863 FIX*/
        case RRM_MODULE_ID:
            rrmSocketTx=fd;
            break;
        /*SPR 20863 FIX*/
       /*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF
        case PACKET_RELAY_MODULE_ID:
             gtpuSocketTx=fd;
             break;
#endif
       /*SPR 21188 -*/

        default:
            LOG_PDCP_MSG( PDCP_INVALID_MODULE_ID, LOGFATAL, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, moduleId, 0, 0, 0, 0,0,
                 __FILE__, __func__ );
    }
}

/*****************************************************************************
 * Function Name  : GetRxSocketAddr 
 * Inputs         : moduleId - Module Id 
 * Outputs        : None
 * Returns        : SockAddr - Ptr to Rx socket address 
 * Description    : Returns pointer to RX socket address structure 
 *                  (sockaddr_in) for given moduleId.
 *****************************************************************************/
static SockAddr *GetRxSocketAddr(Int moduleId)
{
    switch(moduleId)
    {
        case RRC_MODULE_ID:
            return &rrcAddrRx;
        case OAM_MODULE_ID:
            return &oamAddrRx;
        case PDCP_MODULE_ID:
            return &pdcpAddrRx;
        default:
        {
            LOG_PDCP_MSG( PDCP_INVALID_MODULE_ID, LOGFATAL, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, moduleId, 0, 0, 0, 0,0,
                 __FILE__, __func__ );
            return PNULL;
        }
    }
}

/*****************************************************************************
 * Function Name  : GetTxSocketAddr 
 * Inputs         : moduleId - Module Id 
 * Outputs        : None
 * Returns        : SockAddr - Ptr to Tx socket address 
 * Description    : Returns pointer to TX socket address structure 
 *                  (sockaddr_in) for given moduleId.
 *****************************************************************************/
static SockAddr *GetTxSocketAddr(Int moduleId)
{
    switch(moduleId)
    {
        case PDCP_MODULE_ID:
            return &pdcpAddrTx;
        case RRC_MODULE_ID:
            return &rrcAddrTx;
        case OAM_MODULE_ID:
            return &oamAddrTx;
        /*SPR 20863 FIX*/
        case RRM_MODULE_ID:
            return &rrmAddrTx;
        /*SPR 20863 FIX*/
       /*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF
        case PACKET_RELAY_MODULE_ID:
            return &gtpuAddrTx;
#endif
       /*SPR 21188 -*/

        default:
        {
            LOG_PDCP_MSG( PDCP_INVALID_MODULE_ID, LOGFATAL, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, moduleId, 0, 0, 0, 0,0,
                 __FILE__, __func__ );
            return PNULL;
        }
    }
}

/*****************************************************************************
 * Function Name  : GetModuleBlockingMode 
 * Inputs         : moduleId - Module Id 
 * Outputs        : None
 * Returns        : blocking mode 
 * Description    : Returns blocking mode for given moduleId 
 *****************************************************************************/
/*SPR 21366 +*/
UInt8 GetModuleBlockingMode(Int moduleId)
/*SPR 21366 -*/
{
    switch(moduleId)
    {
        case RRC_MODULE_ID:
            return rrcSocketRxBlock;
        case OAM_MODULE_ID:
            return oamSocketRxBlock;
        case PDCP_MODULE_ID:
            return pdcpSocketRxBlock;
        default:
        {
            LOG_PDCP_MSG( PDCP_INVALID_MODULE_ID, LOGFATAL, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, moduleId, 0, 0, 0, 0,0,
                 __FILE__, __func__ );
            return 0;
        }
    }
}
/*****************************************************************************
 * Function Name  : SetModuleBlockingMode 
 * Inputs         : moduleId - Module Id 
 *                  isBlocking - Mode to be set
 * Outputs        : None
 * Returns        : None 
 * Description    : Sets blocking mode for given moduleId 
 *****************************************************************************/
static void SetModuleBlockingMode(Int moduleId,UInt8 isBlocking)
{
    switch(moduleId)
    {
        case RRC_MODULE_ID:
            rrcSocketRxBlock=isBlocking;
            break;
        case OAM_MODULE_ID:
            oamSocketRxBlock=isBlocking;
            break;
        case PDCP_MODULE_ID:
            pdcpSocketRxBlock=isBlocking;
            break;
        default:
            LOG_PDCP_MSG( PDCP_INVALID_MODULE_ID, LOGFATAL, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, moduleId, 0, 0, 0, 0,0,
                 __FILE__, __func__ );
    }
}

/*****************************************************************************
 * Function Name  : CloseSocket 
 * Inputs         : moduleId - Module Id 
 * Outputs        : None
 * Returns        : None 
 * Description    : Close socket file descriptors (RX/TX) for given moduleId. 
 *****************************************************************************/
/* SPR 3444 changes start */
void CloseSocket(Int moduleId)
/* SPR 3444 changes end */
{
    SocketType s=SOCKET_ERROR;

    s=GetTxSocketFD(moduleId);
    if(s!=SOCKET_ERROR)
    {
        SetTxSocketFD(moduleId,SOCKET_ERROR);
        /*Coverity 95472 Fix Start*/
        if(SOCKET_ERROR == close_wrapper(s))
        {
            LOG_MSG(L2_CLOSE_SOCKET_FAIL_ID,LOGFATAL,PDCP_RRCOAM,\
                    pdcpCurrentTime_g,\
                    __LINE__,0,0,0,0,\
                    0.0,0.0,__FILE__,__func__);
        }
        /*Coverity 95472 Fix End*/
    }

    s=GetRxSocketFD(moduleId);
    if(s!=SOCKET_ERROR)
    {
        SetRxSocketFD(moduleId,SOCKET_ERROR);
        /*Coverity 95472 Fix Start*/
        if(SOCKET_ERROR == close_wrapper(s))
        {
            LOG_MSG(L2_CLOSE_SOCKET_FAIL_ID,LOGFATAL,PDCP_RRCOAM,\
                    pdcpCurrentTime_g,\
                    __LINE__,0,0,0,0,\
                    0.0,0.0,__FILE__,__func__);
        }
        /*Coverity 95472 Fix End*/
    }
}

/****************************************************************************
 * Function Name  : InitSocketAddress
 * Inputs         : Host name, sockaddr_in, Port Number, isBlocking
 * Outputs        : 
 * Returns        : Socket file descriptor or SOCKET_ERROR
 * Variables      : 
 * Description    : Intialize UDP socket with address/port
 ****************************************************************************/
static SocketType InitSocketAddress(void *host_name,SockAddr *s,
        UInt16 port,UInt8 isBlocking)
{
    SocketType sockfd=SOCKET_ERROR;
    /*As per SPR 713, change start*/
    if(PNULL == host_name)
    {
        return SOCKET_ERROR;
    }
    /*As per SPR 713, change end*/
    struct hostent *host_p=(struct hostent *)gethostbyname_wrapper(host_name);
    /*As per SPR 713, change start*/
    if (((struct hostent *)0) == host_p)
    {
        /* SPR +- 17777 */
        LOG_PDCP_MSG( L2_GET_HOSTBYNAME_FAIL_ID, LOGFATAL, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__,isBlocking, 0, 0, 0, 0,0,
                 __FILE__, __func__ );
        return SOCKET_ERROR;
    }
    /*As per SPR 713, change end*/
    if(SOCKET_ERROR == (sockfd = socket_wrapper(PF_INET, SOCK_DGRAM, 0)))
    {
        LOG_PDCP_MSG( L2_CREATE_SOCKET_FAIL_ID, LOGFATAL, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, 0, 0, 0, 0, 0,0, __FILE__,
                 __func__);
        return SOCKET_ERROR;
    }
    (*s).sin_family = PF_INET;
    (*s).sin_port = htons_wrapper(port);
    (*s).sin_addr = *((struct in_addr *)host_p->h_addr);
    memSet(&((*s).sin_zero),0,8);
    return sockfd;
}

/****************************************************************************
 * Function Name  : InitTxSocket
 * Inputs         : moduleId - module Id
 * Outputs        : 
 * Returns        : Returns socket file descriptor or SOCKET_ERROR
 * Variables      : 
 * Description    : init TX socket for given moduleId.
 ****************************************************************************/
SocketType InitTxSocket(Int moduleId)
{
    SocketType fd = SOCKET_ERROR;
    /* SPR 3444 changes start */
    SockAddr *sockAddr_p = GetTxSocketAddr(moduleId);
    if( PNULL == sockAddr_p ) 
    {
        PDCP_LOG(LOG_DUMP,"Error when getting SocketAddr for 0x%x ",moduleId);
        return SOCKET_ERROR;
    }
    fd = InitSocketAddress(GetModuleHostName(moduleId),
            sockAddr_p, GetModulePortNum(moduleId), LTE_FALSE);
    /* SPR 3444 changes end */

    /*As per SPR 713, no change done*/
    if((SOCKET_ERROR == fd) )
    {
        return SOCKET_ERROR;
    }
    SetTxSocketFD(moduleId,fd);
    return fd;
}

/****************************************************************************
 * Function Name  : InitRxSocket
 * Inputs         : moduleId - module Id, isBlocking
 * Outputs        : 
 * Returns        : Returns socket file descriptor or SOCKET_ERROR
 * Variables      : 
 * Description    : Init RX socket for given moduleId.
 ****************************************************************************/
SocketType InitRxSocket(Int moduleId, UInt8 isBlocking)
{
    SocketType fd=SOCKET_ERROR;
    SockAddr *pSockAddr = GetRxSocketAddr(moduleId);
    if(!pSockAddr) {
        PDCP_LOG(LOG_DUMP,"Error when getting SocketAddr for 0x%x ",moduleId);
        ltePanic("Error when getting SocketAddr for 0x%x ",moduleId);
        /*cov 26294*/
        return SOCKET_ERROR;
        /*cov 26294*/
    }
    SetModuleBlockingMode(moduleId,isBlocking);
    fd = InitSocketAddress(GetModuleHostName(moduleId), pSockAddr,
            GetModulePortNum(moduleId),isBlocking);
    /*As per SPR 713, no change done*/
    if(SOCKET_ERROR == fd)
    {
        return SOCKET_ERROR;
    }

    /* Bind the port to specific socket for listening */
    if(SOCKET_ERROR == 
            bind_wrapper(fd,pSockAddr,sizeof(SockAddr )))
    {
        LOG_PDCP_MSG( L2_BIND_SOCKET_FAIL_ID, LOGFATAL, PDCP_RRCOAM, pdcpCurrentTime_g,
                 __LINE__, 0, 0, 0, 0, 0,0, __FILE__, __func__);

        /*Coverity 95474 Fix Start*/
        if(SOCKET_ERROR == close_wrapper(fd))
        {
            LOG_MSG(L2_CLOSE_SOCKET_FAIL_ID,LOGWARNING,PDCP_RRCOAM,\
                    pdcpCurrentTime_g,\
                    __LINE__,0,0,0,0,\
                    0.0,0.0,__FILE__,__func__);
        }
        /*Coverity 95474 Fix End*/
        return SOCKET_ERROR;
    }
    SetRxSocketFD(moduleId,fd);    
    return fd;
}
/*SPR 21366 +*/
#if defined PDCP_GTPU_INTF
void SendToGtpu(UInt8 *buffer, UInt32 bufferLen, UInt16 moduleId)
{
    SocketType fd = pdcpGtpuRx;
    SockAddr *pSockAddr=GetTxSocketAddr(moduleId);
    if(fd != SOCKET_ERROR )
    {
        Int res_len = sendto_wrapper(fd,buffer,bufferLen,0,
                pSockAddr,sizeof(SockAddr));
        if(res_len != (SInt64)bufferLen)
        {
            LOG_PDCP_MSG( L2_SOCKET_SENDTO_FAIL_ID, LOGWARNING, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__, res_len, 0, 0, 0, 0,0,
                    __FILE__, __func__ );
            ALARM_MSG (PDCP_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
        }
        return;
    }
}
/****************************************************************************
 * Function Name  : InitGtpuPdcpRxSocket
 * Inputs         : moduleId - module Id, isBlocking
 * Outputs        : 
 * Returns        : Returns socket file descriptor or SOCKET_ERROR
 * Description    : Init PDCP RX socket for receiving messages from GTPU
 ****************************************************************************/
SocketType InitGtpuPdcpRxSocket(Int moduleId, UInt8 isBlocking)
{
    SockAddr *pSockAddr = GetRxSocketAddr(moduleId);
    if(!pSockAddr) {
        LOG_PDCP_FATAL( PDCP_RX, "Error when getting SocketAddr for 0x%x",moduleId);
        return SOCKET_ERROR;
    }
    SetModuleBlockingMode(moduleId,isBlocking);
    pdcpGtpuRx = InitSocketAddress(GetModuleHostName(moduleId), pSockAddr,
            pdcpGtpudModulePort,isBlocking);
    /*As per SPR 713, no change done*/
    if(SOCKET_ERROR == pdcpGtpuRx)
    {
        return SOCKET_ERROR;
    }

    /* Bind the port to specific socket for listening */
    if(SOCKET_ERROR == 
            bind_wrapper(pdcpGtpuRx,pSockAddr,sizeof(SockAddr )))
    {
      LOG_PDCP_ERROR( PDCP_RX, "SOCKET BINDING FAILED ");

      if(SOCKET_ERROR == close_wrapper(pdcpGtpuRx))
      {
        LOG_PDCP_ERROR( PDCP_RX, "SOCKET CLOSE FAILED ");
      }
      return SOCKET_ERROR;
    }
    return pdcpGtpuRx;
}
/*****************************************************************************
 * Function Name  : ReadFromSocket 
 * Inputs         : buf - data from socket will be placed here. Should have
 *                        RX_BUFFER_SIZE length.
 *                  moduleId - determines module to read from RX socket
 * Outputs        : None
 * Returns        : Length of read message. If there is no message it returns 0
 *                  (no wait for a message). If message length != TLV header
 *                  length it returns 0.
 * Description    : Reads message from RX UDP socket for given moduleId.
 *****************************************************************************/
UInt32 PdcpReadFromSocket(UInt8 *buf, UInt16 moduleId,SocketType fd)
{
    Int bytesRead = 0;

    if ( SOCKET_ERROR == fd )
    {
        return 0; /* socket is not initialized */
    }

    if(LTE_TRUE==GetModuleBlockingMode(moduleId))
    {
        /* SPR 6949 Fix Start */
        bytesRead = recvfrom_wrapper(fd, buf, SOCKET_BUFFER_SIZE, 
                          SOCKET_FLAGS_WAIT, PNULL, 0);
        /* SPR 6949 Fix End */
    }
    else
    {
        /* SPR 6949 Fix Start */
        bytesRead = recvfrom_wrapper(fd, buf, SOCKET_BUFFER_SIZE,
                          SOCKET_FLAGS, PNULL, 0);
        /* SPR 6949 Fix End */
    }

    if( bytesRead <= 0)
    {
        LOG_PDCP_MSG( L2_SOCKET_RECVFROM_FAIL_ID, LOGWARNING, PDCP_RRCOAM,
                 pdcpCurrentTime_g, __LINE__, bytesRead, 0, 0, 0, 0,0,
                 __FILE__, __func__ );
        return 0; /*if no message, bytesRead will be -1 */
    }

    return bytesRead;
}


#endif
/*SPR 21366 -*/
