/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpSockTools.h,v 1.1.1.1.16.1 2010/10/25 08:29:44 gur21010 Exp $
 *
 ****************************************************************************
 *
 *  File Description : Functions for transferring messages via UDP sockets. 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: ltePdcpSockTools.h,v $
 * Revision 1.1.1.1.16.1  2010/10/25 08:29:44  gur21010
 * Warnings Removed
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.2.4.2.1  2009/09/30 12:33:15  gur20439
 * oam and rrc IP seperated for mwc demo.
 *
 * Revision 1.2.2.4  2009/07/21 08:04:55  gur19140
 * Bug Fixes
 *
 * Revision 1.2.2.3  2009/07/16 05:04:01  gur19140
 * tick related problem resolve
 *
 * Revision 1.2.2.2  2009/07/09 02:38:50  gur19479
 * updated for port issues
 *
 * Revision 1.2.2.1  2009/06/09 11:27:21  gur19140
 * first wave of Integration
 *
 * Revision 1.2  2009/05/21 07:06:12  gur19140
 * memory leak fixes
 *
 * Revision 1.1  2009/05/20 13:15:56  gur20548
 * Header files name changed
 *
 * Revision 1.3  2009/05/14 11:04:51  gur19836
 * CR Comments Incorporated
 *
 *
 *
 ****************************************************************************/

#ifndef _SOCK_TOOLS_H_
#define _SOCK_TOOLS_H_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#ifdef WIN32
    #include <winsock.h>
    typedef SOCKET SocketType;
    #define SOCKET_FLAGS 0
    #define SOCKET_FLAGS_WAIT 0
#else
    typedef Int SocketType;
    #define SOCKET_ERROR (-1)
    #define SOCKET_FLAGS MSG_DONTWAIT
    #define SOCKET_FLAGS_WAIT 0
#endif


/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/*+ SPR_17858_243_CHANGES */
#ifdef LINUX_PC_TEST
    #define SOCKET_BUFFER_SIZE	(2*1024)
#else
    #define SOCKET_BUFFER_SIZE	(9*1024)
#endif

#define SOCKET_PDCP_RECV_BUF_SIZE    (2*1024)
/*- SPR_17858_243_CHANGES */
/* SPR 19288 change start */
#define HOSTNAME_MAX_LEN	64
/* SPR 19288 change end */

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

UInt8 InitSocketSys(void);
void CloseSockets(void);
SocketType InitTxSocket(Int moduleId);
SocketType InitRxSocket(Int moduleId, UInt8 isBlocking);
SocketType InitGtpuPdcpRxSocket(Int moduleId, UInt8 isBlocking);
UInt32 ReadFromSocket(UInt8 *buf, UInt16 moduleId );
UInt32 PdcpReadFromSocket(UInt8 *buf,UInt16 moduleId, SocketType fd );
UInt32 WriteToSocket(UInt8 *buffer, UInt32 bufferLen, UInt16 moduleId);
void ReadSocketsConfig(UChar8 *fn);
/*SPR 20863 FIX*/
void configPdcpCommPort(UInt16 pdcpModulePort_l,
        UInt16 oamModulePort_l, UInt16 rrcModulePort_l,
        Char8 *oamIp_p, Char8 *rrcIp_p,Char8 *pdcpIp_p, Char8 *rrmIp_p, UInt16 rrmModulePort_l
        /*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF
        ,Char8 *gtpuIp_p,UInt16 gtpuRxPort, UInt16 pdcpGtpuRxPort
#endif
        /*SPR 21188 -*/
        );
void configPdcpCommHostName(Char8* pdcpModuleHN, Char8* oamModuleHN,
        Char8* rrcModuleHN, Char8* rrmModuleHN
        /*SPR 21188 +*/
#ifdef PDCP_GTPU_INTF
        ,Char8* gtpuModuleHN
        /*SPR 21188 +*/
#endif
        );
/*SPR 20863 FIX*/
/* SPR 3444 changes start */
/* close socket file descriptors (RX/TX) for given moduleId */
void CloseSocket(Int moduleId);
/* SPR 3444 changes end */

#ifdef PDCP_GTPU_INTF
/****************************************************************************
 * Function Name  : SendToGtpu
 * Inputs         : buffer - Buffer to be sent
 *                  bufferLen - Length of buffer
 *                  moduleId - module Id
 * Outputs        : None
 * Returns        : None
 * Description    : Sends the packet to GTPu.
 ****************************************************************************/
void SendToGtpu(UInt8 *buffer, UInt32 bufferLen, UInt16 moduleId);
#endif

#endif /* _SOCK_TOOLS_H_ */
