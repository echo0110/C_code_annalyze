/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteTrafficGenRrcInterface.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteTrafficGenRrcInterface.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1.2.4.2.1  2009/09/24 21:44:39  gur19479
 * updated for video and audio processing
 *
 * Revision 1.1.2.4  2009/08/12 08:23:52  gur19836
 * Graceful shutdown of traffic generator code added
 *
 * Revision 1.1.2.3  2009/07/14 14:08:23  gur19836
 * CR Comments Disposed
 *
 * Revision 1.1.2.2  2009/07/13 07:54:53  gur19479
 * updated for traffic gen
 *
 * Revision 1.1.2.1  2009/07/12 09:37:31  gur19479
 * updated for traffic generator
 *
 *
 *
 ****************************************************************************/

#ifndef INCLUDED_LTE_TRAFFIC_GEN_RRC_INTERFACE_H
#define INCLUDED_LTE_TRAFFIC_GEN_RRC_INTERFACE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMemPool.h"
#include "lteTrafficGenDatabase.h"

/****************************************************************************
 * Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define WAIT_TIME 1
#define BUFFERSIZE 2048 
#define MAX_API_NUM 5 

#define TRAFFIC_GEN_UE_ADDED  0X01
#define TRAFFIC_GEN_UE_CONFIGURED 0X02
#define TRAFFIC_GEN_UE_UNCONFIGURED 0XFD

enum maxModeT
{
    TRAFFIC_GEN_TX_DIR,
    TRAFFIC_GEN_RX_DIR,
    TRAFFIC_GEN_RX_TX_DIR
} maxMode;

UInt32 (*processIncomingMessage[MAX_API_NUM])(UInt8 *);
UInt32 (*sendConfirmationMessage[MAX_API_NUM])(UInt16 transId, UInt16 modId, 
        UInt16 response, UInt16 ueIndex);

void initializeRxTxSocket(UInt16 rxPort,UInt16 txPort);
void getDataFromSocket();
UInt16 createTrafficGenEntity();
void trafficGenRxData();

#define SET_UE_INDEX_LC_MAPPING_INVALID(UE_INDEX,LC_ID,RB_DIR) \
           for(LC_ID = 0; LC_ID <= 10;LC_ID++) \
              for( RB_DIR = 0; RB_DIR < 2 ; RB_DIR++) \
                 ueCheckTable_g[UE_INDEX][LC_ID][RB_DIR] = -1;
                 
#define SET_LC_ID_RB_DIR_INVAILD_FOR_UE_DATA(ueInfoPtr,countLC) \
        for (; countLC < MAX_LC; countLC++ ) { \
            ueInfoPtr->UeInfoData[countLC].lcId = -1; \
            ueInfoPtr->UeInfoData[countLC].rbDirection = -1;} 
            
#define LTE_ROOT_ENV    getenv("LTE_ROOT")
#define PORT_FILE_NAME  "../cfg/lteLayer2PortConfig.cfg"

#define GET_L2_PORT_CONFIG_FILE(filename) {\
    strcpy(filename,PORT_FILE_NAME);\
}

#endif   /* INCLUDED_LTE_TRAFFIC_GEN_RRC_INTERFACE_H */
