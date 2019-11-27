/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2UplinkContainer.h,v 1.1.1.1 2010/02/11 04:51:22 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains information  about container
 *			structure and functions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteLayer2UplinkContainer.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2  2009/07/14 14:44:54  gur20439
 * rrc interface code.
 *
 * Revision 1.1  2009/03/30 10:20:16  gur19836
 * Initial Version
 *
 * Revision 1.14  2009/02/09 07:04:07  gur11912
 * BSR Report dataType changed to UInt32
 *
 * Revision 1.13  2009/02/06 06:18:51  gur18569
 * added ulDispatcherToTxNodeSem
 *
 * Revision 1.12  2009/01/29 13:25:44  gur20052
 * Added Changed related to the Stats MAnager of Uplink
 *
 * Revision 1.11  2009/01/27 20:46:23  gur18569
 * using max for bsrqueue as NUM_MAX_CONTAINERS
 *
 * Revision 1.10  2009/01/27 20:29:45  gur18569
 * added dispatcherArrInuse flag in dispatcherNode
 *
 * Revision 1.9  2009/01/21 08:45:44  gur18569
 * added variable in container struct for in use flag
 *
 * Revision 1.8  2009/01/08 18:57:38  gur18569
 * Review comments incorporated
 *
 * Revision 1.7  2008/12/31 06:43:28  gur20052
 * changes for AllocTBSize are added
 *
 * Revision 1.6  2008/12/24 15:10:15  gur12905
 * MAX_NUM_DEMUX_THREADS is used instead of NUM_DEMUX_THREADS
 *
 * Revision 1.5  2008/12/24 08:45:56  gur12905
 * NUM_DEMUX_THREADS changed to 16 from 4
 *
 * Revision 1.4  2008/12/23 07:48:20  gur20052
 * Changes of SR implementation incorporated
 *
 * Revision 1.3  2008/12/19 07:47:45  gur12905
 * uplinkContainer changed to UplinkContainer
 *
 * Revision 1.2  2008/12/11 04:54:00  gur19836
 * extern cleanUpRlcLayer API added
 *
 * Revision 1.1  2008/12/08 13:12:27  gur12905
 * Initial Revision phase - 2
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_UPLINK_CONTAINER_H
#define LTE_UPLINK_CONTAINER_H


#if 0
#define NUM_RARNTI_VALUES 10
#define	MAX_NUM_DEMUX_THREADS	8

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteLayer2Types.h"
#include "lteLayer2PhysInterface.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define NUM_LCG_GROUPS  4
#define NUM_OF_CONTAINERS 8
#define LCGID_INVALID_VALUE 5

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/*struct for carrying information to controller in ueContextControlInfoQueue_g queue */
typedef struct controlQueueNodeT{
		LTE_SNODE 	controlNodeAnchor;	 
		SInt8 		ack;	/*flag indication ACK.NACK for UL data arrived*/
		SInt8 		srFlag;	/*flag indication a SR */
		SInt8 		raFlag;	/*flag indicating a random access request */
		UInt16 		ueIdx;		/*Index of the UE in uplinkContextInfoMap_g	array	*/
		UInt32 		tti;		/*TTI in which data arrived at PHY*/
		raPreambleReq *randomAccess_p;	/*pointer to RA information in case of RA request else NULL */
}controlQueueNode;



/*structure for container's NACK_QUEUE node	*/	
typedef struct containerNackQueueNodeT{
		LTE_SNODE 		nackNodeAnchor;
		UInt16	ueIdx;	/*Index of the UE in uplinkContextInfoMap_g array   */
        UInt32  ttiCounter;
} containerNackQueueNode;

/*struct for container's SR_QUEUE Node	*/
typedef struct srQueueNodeT{
                LTE_SNODE   srNodeAnchor;
                UInt16      ueIdx;  /*Index of the UE in uplinkContextInfoMap_g array   */
                UInt32      ttiCounter;
} srQueueNode;
typedef struct containerSrQueueNodeT{
		LTE_SNODE 		srNodeAnchor;
		UInt16	ueIdx;	/*Index of the UE in uplinkContextInfoMap_g array   */
        UInt32  ttiCounter;
} containerSrQueueNode;


/*struct for container's RA_QUEUE node*/
typedef struct containerRaQueueNodeT{
		LTE_SNODE 		raNodeAnchor;
		UInt16 			tempRnti;	/*Temporary C-RNTI value required for RAR*/
		raPreambleReq 	*raPreamble;	
        UInt32  ttiCounter;
} containerRaQueueNode;

/*struct for container's CQI_QUEUE*/
typedef struct containerCqiQueueNodeT
{
		LTE_SNODE		cqiNodeAnchor;
		UInt16			ueIdx;	/*Index of the UE in uplinkContextInfoMap_g array   */
}containerCqiQueueNode;


/*struct of a node in container.s BSR_QUEUE	*/
typedef struct containerBsrQueueNodeT{
		LTE_SNODE	bsrNodeAnchor;
		UInt32 		bsrReport[NUM_LCG_GROUPS]; /*contain information of each LCG id.
												 Each index represents LCG id 
												 (i.e. index 0 means LCG id 0).*/
        UInt32      ttiCounter;
		UInt16 		ueIdx;	 /*Index of the UE in uplinkContextInfoMap_g array   */
		SInt8 		bsrUpdateFlag;	/*indicates the data(0) or BSR (1)arrived*/
}containerBsrQueueNode;

typedef struct  ULResGrantInfoT
{
       UInt32      bsrNetTotal;
       UInt8       typeOfAllocation;
       UInt8       resInfoLength;
}ULResGrantInfo;

typedef struct ulDispatcherToTxNodeT{
    LTE_LIST_NODE uldispatcherToTxNodeAnchor;
    UInt16                 ueIdx;
    ULResGrantInfo         ulResGrantInfo;
	LTE_SEM   ulDispatcherToTxNodeSem;
} ulDispatcherToTxNode;

typedef struct dispatcherArrayNodeT{
    UInt8   dlSchedulerInfoFlag;
	UInt8   dispatcherArrInUse;
    ulDispatcherToPhy *ulDispatchInfo_p;
    ulDispatcherToTxNode* ulTxNode_p;
}dispatcherArrayNode;

typedef struct UplinkContainerT{
		LTE_SQUEUE		cqiQueue;				/*CQI queue containing UEids for which DL requested CQI report*/
		LTE_SQUEUE 		nackQueue; 			/*NACK queue containing UEIds in case NACK for UL transmission*/
#if 0        
		LTE_SQUEUE 		srQueue;						/*SR queue containing UEId in case of scheduling request from UEs*/
#endif        
		/* NUM_RARNTI_VALUES	10 */
		LTE_SQUEUE 		raQueue[NUM_RARNTI_VALUES]; 	/*RA queue in case of random access request arrived from UEs*/
		LTE_SQUEUE 		bsrQueue[NUM_OF_CONTAINERS];	/*BSR queue containing BSR info of the UEs */
		LTE_LIST 		uldispatcherToTxList ;	/*dispatcher queue containing resource allocation responses*/
        dispatcherArrayNode *dispatcherArr; /*Array of size number of supported UEs.Initialized at init */
		UInt8	schedulerFlag ; /* if 1 means controller has left the container/ 0 means container is still owned by container	*/
} UplinkContainer;

typedef struct  nonZeroBsrListNodeT
{
    LTE_LIST_NODE   nodeAnchor;
    UInt16          ueIdx;
}nonZeroBsrListNode;

typedef struct  raResourceInfoMapT
{
    LTE_LIST_NODE   nodeAnchor;
    UInt16          raRnti;
}raResourceInfoMap ;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/


/****************************************************************************
 * Exported Variables
 ****************************************************************************/


/****************************************************************************
 * Exported Functions
 ****************************************************************************/

void initializeAllContainers ();



#endif


#endif 		/*LTE_UPLINK_CONTAINER_H 	*/
