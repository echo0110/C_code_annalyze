/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2PhysInterface.h,v 1.1.1.1 2010/02/11 04:51:22 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains declarations for the Interface of 
 *                     the LTE MAC Layer with the Physical Layer.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteLayer2PhysInterface.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3  2009/07/14 14:45:02  gur20439
 * rrc interface code.
 *
 * Revision 1.2  2009/06/15 13:52:15  gur20439
 * modifed for release 1.2.
 *
 * Revision 1.1  2009/03/30 10:20:27  gur19836
 * Initial Version
 *
 * Revision 1.23  2009/03/23 08:32:30  gur19140
 * Integration changes first waves
 *
 * Revision 1.22  2009/01/29 13:26:28  gur20052
 * Added Changed related to the Stats MAnager of Uplink
 *
 * Revision 1.21  2009/01/29 12:07:58  gur19413
 * UT_TESTING replace with PHYSIM_TESTING
 *
 * Revision 1.20  2009/01/28 12:37:57  gur03939
 * Changes to avoid memcpy with shared memory
 *
 * Revision 1.19  2009/01/27 12:29:17  gur19413
 * throughput function added
 *
 * Revision 1.18  2009/01/27 10:22:47  gur18569
 * added ueIndex in createAndSentTBToPhy
 *
 * Revision 1.17  2009/01/19 15:11:20  gur18569
 * removed UT_TESTING
 *
 * Revision 1.16  2009/01/17 20:23:04  gur11447
 * Changes for 1 msec, added code in UT_TESTING
 *
 * Revision 1.15  2009/01/16 14:28:48  gur18569
 * restored default port
 *
 * Revision 1.14  2009/01/16 14:27:36  gur18569
 * added UT_TESTING flag and removed mod scheme
 *
 * Revision 1.13  2009/01/15 05:07:46  gur20052
 * Added Support of TTI in which the particular mesaage is intended for UE under UT_TESTING Flag
 *
 * Revision 1.12  2009/01/08 19:02:08  gur18569
 * Review comments incorporated
 *
 * Revision 1.11  2008/12/23 05:13:16  gur11912
 * code review comments incorporated for Phase2
 *
 * Revision 1.10  2008/12/18 12:35:30  gur12905
 * Review comments incorporated
 *
 * Revision 1.9  2008/12/11 14:11:37  gur11912
 * Downlink Phase 2 changes
 *
 * Revision 1.8  2008/12/10 10:24:30  gur12905
 * RA_PREAMBLE_IND value updated
 *
 * Revision 1.7  2008/12/08 12:55:50  gur12905
 * Modified during Phase - 2 merging
 *
 * Revision 1.6  2008/12/05 13:23:26  gur18569
 * multicore changs
 *
 * Revision 1.5  2008/10/14 05:39:26  gur11912
 * scheduler and dispatcher files removed from include
 *
 * Revision 1.4  2008/09/27 15:46:19  gur11912
 * UT Defect Fixed
 *
 * Revision 1.3  2008/09/19 07:15:18  gur11912
 * review comments incorporated
 *
 * Revision 1.2  2008/09/09 16:10:12  gur11974
 * Updating for integration
 *
 * Revision 1.1  2008/09/09 05:13:56  gur11974
 * Initial Version
 *
 *
 *
 ****************************************************************************/

#ifndef LTELAYER2_PHYS_INTERFACE_H
#define LTELAYER2_PHYS_INTERFACE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteLayer2Types.h"
#include "lteLayer2UEContext.h"
#include "lteLayer2InitMac.h"
#include "lteLayer2CommanTypes.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define CONTROL_FLAG_INDICATOR 0x01
#define UL_FLAG_INDICATOR 0x02
//#define RA_PREAMBLE_INDICATOR 0x04
//#define CQI_CONTROL_INFO 0x01
//#define DL_ACK_NACK_INFO 0x02
//#define SR_REQUEST_INFO  0x04
//#define TIME_ADVANCE_INFO 0x08
#define TRUE  1
#define FALSE   0
//#define CONTROL_INFO_RETRANSMISSION 14
//#define THROUGHPUT_DURATION 5
/****************************************************************************
 * Exported Types
 ****************************************************************************/
#if 0

/* Logical channel info */
/*typedef struct LCInfoT 
  {
  UInt32    size;    
  UInt8*    sduData_p;
  SInt8    lcID;    
  }LCInfo;*/

/* Header information for making MAC PDU */
typedef struct SDUHeaderInfoT
{
    RLCDataBuffer*     lcInfo_p;        /* Array for storing the LCInfo pointers */
    UInt32    numOfLCMultiplexed;    /* Number of Logical Channels to be multiplexed for creating a Transport Block. */
    UInt16    paddingSize;        /* Padding Size */
    UInt8       taValue;
}SDUHeaderInfo;

/* Downlink information that needs to be passed to the Physical Layer along with the transport block data pointer */
typedef struct DownlinkPhysInfoT 
{
    UInt8 *    data_p;            /* MAC PDU data pointer */
	UInt32 dispatchedTick;
    UInt32    tbSize;            /* MAC PDU size */
    UInt32    rbBitMapValue;        /* RB bit map value. RB block to use for sending the data */
    UInt16    ueID;            /* UE Identifier*/
    UInt8    numRB;            /* Number of Resource Blocks used for the MAC PDU */
    UInt8    antennaMapping;        /* Antenna Mapping used for distinguishing between different codewords.*/
    UInt8    harqID;            /* HARQ process id. */
    UInt8    rednVersion;        /* Redundancy version used in the HARQ*/
    UInt8    newDataInd;        /* New data or re-transmission indicator */
    UInt8   mcsIndex;               /* MCS Index */ 
    UInt8    transportChannelType;    /* Transport channel type */
}DownlinkPhysInfo;


/*structure of the infromation received from PHY in case of a Random
  Access Preamble request*/
typedef struct raPreambleReqT {
    SInt8   preamble;               /*5 bit preamble received*/
    SInt8   hoppingBit;    /*frequency hopping bit*/
    SInt8   tmcs;   /*Truncated modulation and coding scheme*/
    SInt8   tpc;    /*transmission power control*/
    SInt8   ulDelay;               /*ul delay */
    UInt16   timingAdvance;      /*timing advance for time alignment*/
}raPreambleReq;



/*structure for UL information obtained from PHY*/
typedef struct ulInfoT{
    /*  UInt8   ack;*/ /*ACK/NACK info in case of uplink data transmission*/
    UInt8*  data_p; /*pointer to the uplink data arrived*/
    UInt32  length; /*Length of the uplink data arrived*/
}ulInfo;


/*structure for SUB-BAND information in CQI reports*/
#if 0
typedef struct subBandInfoT{
    UInt8       cqiIndex;   /*CQI index received*/
    UInt8       bandNum;        /*Sub band number value*/
}subBandInfo;

/*structure for the aperiodic reporting modes*/
/*structure of the  aperiodic MODE 2-0 */
typedef struct aperiodicMode20T {
    UInt16          wBandCqiIndex;  /*Wide-band index received*/
    subBandInfo         *subCqiInfo_p;  /*pointer to sub band information*/
    UInt8           subBandLength;      /*number of the sub-bands for which CQI report received*/
}apMode20;


/*structure of the  aperiodic MODE 3-0 */
typedef struct aperiodicMode30T{
    UInt16      wBandCqiIndex;  /*Wide-band index received*/
    subBandInfo     *subCqiInfo_p;  /*pointer to sub band information*/
    UInt8       subBandLength;       /*number of the sub-bands for which CQI report received*/
}apMode30;

/*structure of the  aperiodic MODE 1-2 */
typedef struct aperiodicMode12T {
    /*For future use            */
}apMode12;

/*structure of the  aperiodic MODE 2-2 */
typedef struct aperiodicMode22T {
    /*For future use            */
}apMode22;


/*structure of the  aperiodic MODE 3-1 */
typedef struct aperiodicMode31T {
    /*For future use            */
}apMode31;
/*Structure of Aperiodic CQI Reporting Information */
typedef struct aperiodicReportingInfoT {
    /*MODE type*/
    enum reportingMode{
        RM12,
        RM20,
        RM22,
        RM30,
        RM31,
        SPARE1,
        SPARE2,
        SPARE3
    }rmode;
    union aperiodicMode{
        apMode12    mode12data;
        apMode20    mode20data;
        apMode22    mode22data;
        apMode30    mode30data;
        apMode31    mode31data;
    }modeData;

}aperiodicReportingInfo;


/*structure for periodic CQI reporting MODE 1-0*/
typedef struct periodicMode10T {
    UInt16      periodicWBandCqiIndex;  /*Wide-band CQI index received*/
}periodicMode10;


/*structure for periodic CQI reporting MODE 2-0*/
typedef struct periodicMode20T {
    UInt8       reportType; /*0 for Wband/1 for Sband */
    union   bandType{
        UInt16      periodicWBandCqiIndex;  /*Wide-band CQI index received*/
        subBandInfo     *sBand_p;       /*pointer to sub-band information*/
    }bandType;
}periodicMode20;
/*structure for periodic CQI reporting MODE 1-1*/
typedef struct periodicMode11T {
    /*For future use            */
}periodicMode11;


/*structure for periodic CQI reporting MODE 2-1*/
typedef struct periodicMode21T {
    /*For future use            */
} periodicMode21;
/*Structure for Periodic CQI reporting information*/
typedef struct periodicReportingInfoT {
    enum reportingType {
        MODE10,
        MODE20,
        MODE11,
        MODE21
    }rtype;
    union periodicMode{
        periodicMode10  mode10data;
        periodicMode20  mode20data;
        periodicMode11  mode11data;
        periodicMode21  mode21data;
    }modeData;

}periodicReportingInfo;

/*CQI Reporting Information received from PHY*/
typedef struct cqiInfoT{
    UInt8   reporingType; //0 for aperiodic & 1 for periodic

    union cqiReportingInfo {

        aperiodicReportingInfo  *aperiodicReportingInfo_p;
        periodicReportingInfo   *periodicReportingInfo_p;
    }cqiRepType;
}cqiInfo;
#endif

typedef enum periodicAperiodicModeT{
    APERIODIC_MODE_20 = 1,
    APERIODIC_MODE_30,
    PERIODIC_MODE_10,
    PERIODIC_MODE_20
} periodicAperiodicMode;


#if 0
typedef struct UEReportedCQINodeInfoT{
    subBandInfo *subBandCqi_p;
    periodicAperiodicMode mode;
    UInt8   nbSubBands;
    SInt8   wideBandCqiValue;
}UEReportedCQINodeInfo;


/*structure of the control information obtained from PHY*/
typedef struct controlInfoT {
    UInt8       cqiFlag;
    /*  cqiInfo     cqiInfo*/;  /* CQI information */
    UEReportedCQINodeInfo   *cqiInfo;
    UInt8       dlAck;      /*Downlink ACK/NACK info */
    UInt8       srReq;      /* Scheduling Req flag */
    SInt8       harqId;     /*HARQ Id in case of Downlink ACK/NACK*/
    UInt8       timeAdvance;    /*timeing advance in case of RA , Provided by PHY*/
} controlInfo;


/*structure of the message received from PHY*/
typedef struct ulMessageT{
    controlInfo*    cntInfo_p;
    ulInfo*         uplinkInfo_p;
    raPreambleReq*  raPreamble_p;
    UInt16          receivedRnti;
    UInt32          tti;
    UInt8           ulAck;
}ulMessage;

#endif
/*Structure for filling res allocation info in dispatcher queue*/
typedef struct ulDispatcherToPhyT{
		LTE_SNODE   ulDispatcherToPhyNodeAnchor;
		UInt16		rnti;	/*RNTI to be used while sending resource allocation to UE*/
        /*SPR 15909 Fix Start*/
        tickType_t      ttiForUE;
        /*SPR 15909 Fix end*/
		UInt8		dciformat;	/*which of the dci format is being used*/
		UInt8		ackNack;	/*ack/nack being sent*/
		UInt8		riv;	/*indicates the resource allocation information*/
        UInt8       resInfoIndex;
        UInt8       resInfoLength;
		UInt8		mcs;	/*MCS Index used*/
		UInt8		cqi;	/*request the aperiodic request from UE*/
        UInt8       srResp;
        UInt32      allocTBSize;
}ulDispatcherToPhy;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
#define PHYSIP "127.0.0.1"

#define PHYSPORT MAC_TX_PHY_PORT

#define CONTROL_INFO_LENGTH 21

#define MAX_NUM_LC 32
#define DELIMITER_LENGTH 1

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/*SPR 15909 Fix Start*/
extern tickType_t globalTTITickCount_g;
/*SPR 15909 Fix end*/
extern LTE_SEM timerWaitForTTISem_g;
extern LTE_SEM distributorWaitForTickSem_g;
extern LTE_SEM schedulerWaitForTTISem_g;

extern SInt32 physSockFD_g;
extern SockAddr phys_addr_g;
extern UInt32 totalDwlkBytesSend_g;
extern UInt32 lastThroughputDuration_g;
/****************************************************************************
 * Exported Functions
 ****************************************************************************/

MacRetType initPhysInterface(void);

void recvDataFromPhysical (void* args_p);

UInt8* createAndSendTBToPhysical (SDUHeaderInfo* sduHeader_p, 
                                        DownlinkPhysInfo* physInfo_p,
                                        SInt8 * taOffset_p,
										SInt16 ueIndex);

//UInt32 sendDataToPhysical (DownlinkPhysInfo* physInfo_p); (For real time scenarios)
SInt32 sendDataToPhysical (UInt8 *bufferToSend_p, UInt32 bufferLen);

SInt32 sendMsgBySocket(UInt8 *buffer_p,UInt32 bufferLen);

void receiveTTITickFromPhysical (void);

void populateControlInformation(UInt8 *bufferTB_p, DownlinkPhysInfo *physInfo_p,
                        UInt16 *index_p);
void displayDwlkThroughPut();

/*New functions for Phase-2*/
//void initDistributerThread ();

//void createRntiToUeIdMap ();

//void deleteRntiFromUeIdMap (UInt16 index);
#endif 
#endif /* Included LTELAYER2_PHYS_INTERFACE_H */
