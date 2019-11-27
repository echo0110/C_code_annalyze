/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteConfigFileHdlr.h
 *
 ****************************************************************************
 *
 *  File Description : This file contains the function prototypes and 
 *                     data structures needed for configuration
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 ****************************************************************************/
#ifndef LTE_CONFIG_FILE_HDLR_H 
#define LTE_CONFIG_FILE_HDLR_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteTypes.h"
#include <semaphore.h>
#define MAX_STR_LENGTH 128

#define NORMAL_MODE 1
#define LOOPBACK_MODE 0
/*IPV6_CHANGE - START*/
#define IPV4_ADDR_PRESENT	0x01
#define IPV6_ADDR_PRESENT	0x02
#define IPV4_IPV6_BOTH_PRESENT	0x03
/*IPV6_CHANGE - END*/
/****************************************************************************
 * Exported Types
 ****************************************************************************/

void lteCreateTokenString(char *enbMacRxPortStr,char *enbRlcRxPortStr,
                          char *enbPdcpRxPortStr,
/*SPR 20863 FIX*/
char *enbMacIpAddressStr,char *enbPdcpIpAddressStr
/*SPR 20863 FIX*/
,SInt32 instanceL2);
void trimSpace(char *buf,unsigned int buf_size);

Char8* isKeyPresent( Char8* string_p, Char8 *keyName_p);

UInt8 fillIpAndPortInfo(UInt16 *rxPortMAC, UInt16 *rxPortRLC,
              UInt16 *txPortOAM, 
              UInt16 *txPortRRC, UInt16 *rxPortPDCP,
              Char8 *oamIp_p, 
              Char8 *rrcIp_p,
              /*SPR 20863 FIX*/
              Char8 *macIp,Char8 *pdcpIp,Char8 *rrmIp_p, UInt16 * rrmTxPort_p,
              /*SPR 20863 FIX*/
#if defined PDCP_GTPU_INTF
              Char8 *gtpuIp_p,  /*It holds the GTPU module IP*/ 
              UInt16 *gtpuRxPort_p, /*It holds the GTPU Rx port for receiving messages from PDCP*/
              UInt16 *pdcpGtpuRxPort_p, /*It holds the PDCP Rx Port for receiving messages from GTPU */
#ifdef ENDC_ENABLED
              /*Dual Connectivity SeNB changes +*/
              UInt16 *rxX2uDataPortRlc_p,
              /*Dual Connectivity SeNB changes -*/
#endif
#endif
              /*SPR 21188 -*/
#ifdef DL_UL_SPLIT_TDD
              UInt16 *rxPortULThreadPhy, 
#endif
              Char8 *portConfigFileName, SInt32 instanceL2);
UInt8 l2CommonFillPhyIpAndPortInfo(Char8 *portConfigFileName,SInt32 instanceL2,UInt8 *phyFlag_p);
/* +- SPR 17777 */
void createTokenStringRelay(char *enbL2IpAddressStr,
                            char *enbS1uRxPortStr,SInt32 instanceL2);
/* +- SPR 17777 */

UInt32 getCfgForSocket(UInt8  *cfgFile_p, SInt8 * oamSocketInf,  
    SInt8 * s1ApSocketInf, Char8 * adapterSocketInf,
    UInt32*  oamRxPort,UInt32*  s1ApRxPort, UInt32* s1ApTxPort,
    Char8 * peerTgInf, Char8 * peerTgInfV6, Char8 * peerTgAddrType,
    Char8 * secPeerTgInf, Char8 * secPeerTgInfV6, Char8 * secPeerTgAddrType, SInt32 instanceL2
    /*SPR 21366 +*/
#if defined PDCP_GTPU_INTF
    ,UInt32* gtpuRxPort
#ifdef ENDC_ENABLED
    /*Dual Connectivity SeNB changes +*/
    ,UInt32* rlcRxPort
    /*Dual Connectivity SeNB changes -*/
#endif
#endif
    /*SPR 21366 -*/
    );


UInt8 ReadConfigFile(Char8* configFileName, Char8 * trafficGeneratorIP_p, 
    UInt16* pdcpAdapterMode_g, UInt16* pdcpAdapterDelay_g);


UInt32 buildConfiguration( UInt8* responseBuffer_p,UInt32 * Idx_p );

UInt16 getPDCPBuildInfo(Char8* buildInfo_p, UInt16 maxSize,Char8* fileName_p);

/* + Coverity 31402 */        
void handleAdapterSockInf(Char8 * adapterSocketInf_p,UInt8 size, UInt8 *gtpuInitDbReq, UInt16 current);
/* - Coverity 31402 */        

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
#endif
