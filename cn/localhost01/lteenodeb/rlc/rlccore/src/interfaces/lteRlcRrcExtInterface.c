/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcRrcExtInterface.c,v 1.1.1.1.12.2.2.2 2010/10/25 07:46:35 gur21010 Exp $
 *
 ****************************************************************************
 *  File Description : This file provide APIs to receive request from RRC Layer,
 *                     send a response to RRC Layer.Presently communication 
 *                     with RRC is socket based.
 *
 *
 *
 *  1. setReadWritePort
 *  2. createRLCSockets 
 *  3. destroySockets
 *
 *
 *
 *
 *
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcRrcExtInterface.c,v $
 *
 *
 ****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/



/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRlcRrcExtInterface.h"
#include "lteRlcGlobalData.h"
#include "logging.h"
/****************************************************************************
  Private Definitions
 ****************************************************************************/
/****************************************************************************
  Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/* Socket FD for receiving messages */
SInt32 readSockFD_g;
/* Socket FD for receiving messages */
SInt32 writeSockFD_g;
SockAddr writeAddr_g;
/* Socket FD for transmitting OAM messages */
SInt32 writeOAMSockFD_g;
SockAddr writeOAMAddr_g;
UInt16 writeOAMPort_g = TXPORT_OAM;

/* read and write port number */
UInt16 readPort_g = READPORT;


#if defined(ENDC_ENABLED) &&  defined (PDCP_GTPU_INTF)
UInt16 X2uRxPort_g = GTPU_RX_RLC_PORT;
UInt16 rlcX2uRxDataPort_g = RLC_RX_X2U_PORT;
Char8 x2uModuleHostName[HOSTNAME_MAX_LEN] = GTPU_MODULE_HOST_NAME;
SockAddr writeX2uAddr_g;
/* Socket FD for receiving messages from X2U Relay */
SInt32 readSockX2uRelayFD_g;

/* Socket FD for transmitting data to X2U Relay */
SInt32 writeX2uSockFD_g;
#endif
UInt16 writePort_g = WRITEPORT;
Char8 l2ModuleHostName[HOSTNAME_MAX_LEN] = L2_MODULE_HOST_NAME;
Char8 oamModuleHostName[HOSTNAME_MAX_LEN] = OAM_MODULE_HOST_NAME;
Char8 rrcModuleHostName[HOSTNAME_MAX_LEN] = RRC_MODULE_HOST_NAME;

/* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
/* SPR 15909 fix end */
/* +- SPR 17777 */
extern tickType_t getCurrentTick(void);
/* +- SPR 17777 */
/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

/********************************************************************************
 * Function Name  : setReadWritePort 
 * Inputs         : readPort - Specify the port number for reading rrc req msg,
 *                  rrcWritePort - rrcwritePort specify the port number to
 *                                 write rrc cnf msg,
 *                  oamWritePort - oamritePort specify the port number to
 *                                 write oam cnf msg
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to seat read and write port.
 *******************************************************************************/
/* SPR 5178 changes starts here */
void setReadWritePort(UInt16 readPort, UInt16 rrcWritePort, UInt16 oamWritePort)
{
    readPort_g =  readPort;
    writePort_g = rrcWritePort;
    writeOAMPort_g = oamWritePort ;
}

/****************************************************************************
 * Function Name  : createWriteRLCSockets
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This API will create sockets for transmitting data
 *                  to RRC/OAM.
 *                   1. Creating Tx Socket for RRC Layer
 *                   2. Creating TX Socket for OAM Layer.
 ****************************************************************************/

void createWriteRLCSockets(void)
{
    /* Socket creation for Transmission of data to RRC*/
    if ((writeSockFD_g = socket_wrapper(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        LOG_RLC_MSG(LTE_RLC_SOCKET_CREATION_FAILED, LOGFATAL, RLC_RRC, globalTTITickCount_g, __LINE__,
                0,0,0,0, 0,0, __func__, "");
        ltePanic("createWriteRLCSockets sockets fails\n");
    }

    writeAddr_g.sin_family = AF_INET;
    writeAddr_g.sin_port = htons_wrapper(writePort_g);

    if (Inet_aton_wrapper(rrcModuleHostName,&writeAddr_g.sin_addr) == 0)
    {
        LOG_RLC_MSG(LTE_RLC_SOCKET_ADDRESS_CONVERSION_ERROR, LOGFATAL, RLC_RRC, globalTTITickCount_g, __LINE__,
                0,0,0,0, 0,0, __func__, "");
        ltePanic("createWriteRLCSockets Inet_aton fails\n");
    }
    /* Socket creation for Transmission of data to OAM */
    if ((writeOAMSockFD_g = socket_wrapper(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        LOG_RLC_MSG(LTE_RLC_SOCKET_CREATION_FAILED, LOGFATAL, RLC_RRC, globalTTITickCount_g, __LINE__,
                0,0,0,0, 0,0, __func__, "For Transmission of data to OAM");
        ltePanic("createWriteRLCSockets Socket fails\n");
    }

    writeOAMAddr_g.sin_family = AF_INET;
    writeOAMAddr_g.sin_port = htons_wrapper(writeOAMPort_g);
    if (Inet_aton_wrapper(oamModuleHostName,&writeOAMAddr_g.sin_addr) == 0)
    {
        LOG_RLC_MSG(LTE_RLC_SOCKET_ADDRESS_CONVERSION_ERROR, LOGFATAL, RLC_RRC, globalTTITickCount_g, __LINE__,
                0,0,0,0, 0,0, __func__, "");
        ltePanic("createWriteRLCSockets inet_aton fails\n");
    }

#if defined(ENDC_ENABLED) &&  defined (PDCP_GTPU_INTF)
    /* Socket creation for Transmission of data to OAM */
    if ((writeX2uSockFD_g = socket_wrapper(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        LOG_RLC_MSG(LTE_RLC_SOCKET_CREATION_FAILED, LOGFATAL, RLC_RRC, globalTTITickCount_g, __LINE__,
                0,0,0,0, 0,0, __func__, "For Transmission of data to X2U Relay");
        ltePanic("createWriteRLCSockets Socket fails\n");
    }

    writeX2uAddr_g.sin_family = AF_INET;
    writeX2uAddr_g.sin_port = htons_wrapper(X2uRxPort_g);
    if (Inet_aton_wrapper(x2uModuleHostName,&writeX2uAddr_g.sin_addr) == 0)
    {
        LOG_RLC_MSG(LTE_RLC_SOCKET_ADDRESS_CONVERSION_ERROR, LOGFATAL, RLC_RRC, globalTTITickCount_g, __LINE__,
                0,0,0,0, 0,0, __func__, "");
        ltePanic("createWriteRLCSockets inet_aton fails\n");
    }
#endif
}

/****************************************************************************
 * Function Name  : createRLCSockets
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This API will create sockets for receiving/transmitting data
 *                  to/from RRC.
 *                   1. Creating RX socket for RLC layer
 *                   2. Creating TX Socket for OAM Layer
 *                   3. Creating Tx Socket for RRC Layer.
 ****************************************************************************/

void createRLCSockets(void)
{
    /* Socket creation for Receiving data from RRC Layer */
    SockAddr readAddr;
    /* default setting the port if has not been set */
    if ((readSockFD_g = socket_wrapper(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        LOG_RLC_MSG(LTE_RLC_SOCKET_OPEN_ERROR, LOGFATAL, RLC_RRC, globalTTITickCount_g, __LINE__,
                0,0,0,0, 0,0, __func__, "");
        ltePanic("createRLCSockets sockets fails\n");
    }
    readAddr.sin_family = AF_INET;
    readAddr.sin_port = htons_wrapper (readPort_g);
    if (Inet_aton_wrapper(l2ModuleHostName,&readAddr.sin_addr) == 0)
    {
        LOG_RLC_MSG(LTE_RLC_SOCKET_ADDRESS_CONVERSION_ERROR, LOGFATAL, RLC_RRC, globalTTITickCount_g, __LINE__,
                0,0,0,0, 0,0, __func__, "");
        ltePanic("createRLCSockets Inet_aton fails\n");
    }

    if ((bind_wrapper(readSockFD_g, &readAddr, 
	     sizeof(readAddr))) == -1)
    {
        LOG_RLC_MSG(LTE_RLC_SOCKET_BIND_FAILED, LOGFATAL, RLC_RRC, globalTTITickCount_g, __LINE__,
                0,0,0,0, 0,0, __func__, "");
        ltePanic("createRLCSockets inet_aton fails\n");
    }


#if defined(ENDC_ENABLED) &&  defined (PDCP_GTPU_INTF)
    /* RLC socket for reading messages from X2U Relay */
    SockAddr readRlcAddr;

    /* Create socket FD for reading messages from PDCP */
    if ((readSockX2uRelayFD_g = socket_wrapper(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        LOG_RLC_MSG(LTE_RLC_SOCKET_OPEN_ERROR, LOGFATAL, RLC_PDCP, 
                    globalTTITickCount_g, __LINE__,
                    0,0,0,0, 0,0, __func__, "");

        ltePanic("createRLCSockets sockets fails\n");
    }

    /* Populate RLC socket address structure */
    readRlcAddr.sin_family = AF_INET;
    readRlcAddr.sin_port   = htons_wrapper(rlcX2uRxDataPort_g);
    if (Inet_aton_wrapper(l2ModuleHostName, &readRlcAddr.sin_addr) == 0)
    {
        LOG_RLC_MSG(LTE_RLC_SOCKET_ADDRESS_CONVERSION_ERROR, LOGFATAL, 
                    RLC_PDCP, globalTTITickCount_g, __LINE__,
                    0,0,0,0, 0,0, __func__, "");

        ltePanic("createRLCSockets Inet_aton fails\n");
    }

    /* Bind socket FD with RLC address */
    if ((bind_wrapper(readSockX2uRelayFD_g, &readRlcAddr, 
                      sizeof(readRlcAddr))) == -1)
    {
        LOG_RLC_MSG(LTE_RLC_SOCKET_BIND_FAILED, LOGFATAL, RLC_PDCP, 
                    globalTTITickCount_g, __LINE__,
                    0,0,0,0, 0,0, __func__, "");

        ltePanic("createRLCSockets inet_aton fails\n");
    }
#endif
    /* Socket creation for Transmission of data to RRC/OAM Layer */
    createWriteRLCSockets();
}

/****************************************************************************
 * Function Name  : destroySockets
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This API will destroy sockets for receiving/transmitting data
 *                  to/from RRC.
 *****************************************************************************/ 
/* + SPR 17439 */
void destroySockets(void)
/* - SPR 17439 */    
{
    /*Coverity 97476 Fix Start*/
    if( SOCKET_ERROR == close_wrapper(readSockFD_g))
    {
        LOG_MSG(L2_CLOSE_SOCKET_FAIL_ID,LOGFATAL,RLC_RRC,\
                getCurrentTick(),\
                __LINE__,0,0,0,0,\
                0.0,0.0,__FILE__,__func__);
    }
    readSockFD_g = -1;
    if(SOCKET_ERROR == close_wrapper(writeSockFD_g))
    {
        LOG_MSG(L2_CLOSE_SOCKET_FAIL_ID,LOGFATAL,RLC_RRC,\
                getCurrentTick(),\
                __LINE__,0,0,0,0,\
                0.0,0.0,__FILE__,__func__);
    }
    writeSockFD_g = - 1;
    if(SOCKET_ERROR == close_wrapper(writeOAMSockFD_g))
    {
        LOG_MSG(L2_CLOSE_SOCKET_FAIL_ID,LOGFATAL,RLC_RRC,\
                getCurrentTick(),\
                __LINE__,0,0,0,0,\
                0.0,0.0,__FILE__,__func__);
    }
    /*Coverity 97476 Fix End*/
    writeOAMSockFD_g = - 1;
/* SPR 5178 changes starts here */
/* SPR 5178 changes ends here */    
}    
