/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2UplinkTransmitter.c,v 1.1.1.1 2010/02/11 04:51:22 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains functions used by uplink transmitter
 *                      to be usedto send data to PHY. 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteLayer2UplinkTransmitter.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3  2009/07/14 14:48:54  gur20439
 * rrc interface code change.
 *
 * Revision 1.2  2009/06/15 13:55:15  gur20439
 * modified for release 1.2.
 *
 * Revision 1.1  2009/03/30 10:20:41  gur19836
 * Initial Version
 *
 * Revision 1.42  2009/03/02 10:48:44  gur18569
 *  added code for throughput calculation
 *
 * Revision 1.41  2009/02/20 16:00:53  gur18569
 * throughput changes
 *
 * Revision 1.40  2009/02/20 14:24:25  gur18569
 * ul thoughput changes
 *
 * Revision 1.39  2009/02/17 06:48:00  gur18569
 * commented unnecessary memCpy in writeDataToPhySim func
 *
 * Revision 1.38  2009/02/06 06:12:53  gur18569
 * added sem for ulTxNode_p
 *
 * Revision 1.37  2009/01/31 12:17:44  gur19836
 * sempost added pending delete UE if UL SCH stats enabled and formatting
 *
 * Revision 1.36  2009/01/31 11:03:18  gur03939
 * Modified code for generating uplink stats info from Trasmitter
 *
 * Revision 1.35  2009/01/30 14:08:18  gur18569
 * commenting unused variables
 *
 * Revision 1.34  2009/01/29 22:41:33  gur18569
 * changes regarding ul scheduling stats
 *
 * Revision 1.33  2009/01/29 17:56:21  gur03939
 * Changes for message queue interface with PHY SIM
 *
 * Revision 1.32  2009/01/29 13:29:00  gur20052
 * Added Changed related to the Stats MAnager of Uplink
 *
 * Revision 1.31  2009/01/29 11:42:49  gur19413
 * updated
 *
 * Revision 1.30  2009/01/28 12:34:53  gur03939
 * Changes to avoid memcpy with shared memory
 *
 * Revision 1.29  2009/01/27 20:36:12  gur18569
 * removed error from buildAndSenULOutGoingMessage
 *
 * Revision 1.28  2009/01/27 20:29:00  gur18569
 * added dispatcherArrInuse flag in dispatcherNode
 *
 * Revision 1.27  2009/01/27 16:58:38  gur19413
 * UT_TESTING replace with PHYSIM_TESTING
 *
 * Revision 1.26  2009/01/27 09:42:13  gur18569
 * added warning in case Encoder is unable to send ul data
 *
 * Revision 1.25  2009/01/27 07:22:11  gur20052
 * Incorporated Vandana's Comments
 *
 * Revision 1.24  2009/01/25 11:52:27  gur20052
 * Added During Regression Testing
 *
 * Revision 1.23  2009/01/20 07:47:00  gur19836
 * Changes done for interface with RRC Layer
 *
 * Revision 1.22  2009/01/20 07:16:45  gur19413
 * updated
 *
 * Revision 1.21  2009/01/19 06:00:20  gur11447
 * Changes for 1 msec
 *
 * Revision 1.20  2009/01/17 20:40:53  gur11447
 * Changes for 1 msec
 *
 * Revision 1.19  2009/01/14 17:24:00  gur20052
 * Added a new variable ttiForUE for UT_TESTING
 *
 * Revision 1.18  2009/01/13 09:15:55  gur11447
 * Vandana's Comments incorpoarted
 *
 * Revision 1.17  2009/01/08 19:10:57  gur11447
 * *** empty log message ***
 *
 * Revision 1.16  2009/01/05 12:14:45  gur18569
 * not resetting dlSchedulerInfoFlag flag in case node is to be transmitted
 * by DL scheduler.
 *
 * Revision 1.15  2009/01/05 07:27:02  gur12905
 * Error corrected for sending res allocation for more that 1 UEs from
 *  dispatcher queue
 *
 * Revision 1.14  2009/01/01 22:27:46  gur11447
 * In UT_TESTING changed the sync time to every tick --KT
 *
 * Revision 1.13  2009/01/01 15:43:04  gur20052
 * Chnages to Avoid Overwriting ACk/NACK with resource allocation
 *
 * Revision 1.12  2008/12/31 12:08:26  gur20052
 * listCount used for looping instead of checking node_p for NULL
 *
 * Revision 1.11  2008/12/31 06:44:28  gur20052
 * changes for AllocTBSize are added
 *
 * Revision 1.10  2008/12/30 07:21:47  gur12905
 * Corrections done for sending heart beat
 *
 * Revision 1.9  2008/12/30 07:10:26  gur11447
 * Add code for UT Purpose for Timer SYNC between MAC & PHY Sim -- KT
 *
 * Revision 1.8  2008/12/22 05:14:04  gur12905
 * FreeMemPool corrected
 *
 * Revision 1.7  2008/12/19 13:19:06  gur20052
 * The pthread sys calls are replaced with wrappers
 *
 * Revision 1.6  2008/12/19 12:48:40  gur12905
 * Transmitter modified for node deletion in case if ulInfo_p is NULL and UEIdx in list
 *
 * Revision 1.5  2008/12/19 11:52:42  gur12905
 * Transmitter modified for DL node deletion
 *
 * Revision 1.4  2008/12/19 07:58:20  gur12905
 * uplinkContainer changed to UplinkContainer
 *
 * Revision 1.3  2008/12/18 12:33:54  gur12905
 * Review comments incorporated
 *
 * Revision 1.2  2008/12/10 10:37:15  gur12905
 * Updated for transmitter thread creation
 *
 * Revision 1.1  2008/12/08 13:02:21  gur12905
 * Initial Revision phase - 2
 *
 *
 ****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/
#if 0
#include "lteLayer2UplinkTransmitter.h"
#include "lteLayer2InitMac.h"
#include "lteLayer2OAM.h"
#include "lteLayer2UplinkScheduler.h"
/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
  Private Definitions
 ****************************************************************************/



/****************************************************************************
  Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
extern SInt32 sendDataToPhysical (UInt8 *bufferToSend_p, UInt32 bufferLen);
/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern UplinkContainer* containerPool_g[MAX_CONTAINER_NUM];
extern LTE_SEM  transmitterSem_g;
extern UInt32   globalTTITickCount_g;
extern LTE_SEM processCtrlQueueSem_g;
UInt32 totalUplinkBytesSend_g = 0;
UInt32 ulThroughPutEnable_g;

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/



void ulTransmitter_cleanup_handler(void *arg)
{
            printf("In Uplink Transmitter Thread, cleanup handler\n");
}

/****************************************************************************
 * Function Name  : initUplinkTransmitter()
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Variables      : 
 * Description    : This function will work on the container's 
 *                  uldispatcherToTxList and find out which uplink information
 *                  needs to be sent to PHY.
 ****************************************************************************/
void *initUplinkTransmitter(void *arg)
{
    LTE_LIST_NODE   *nodePtr_p=PNULL;
    LTE_LIST_NODE   *oldNodePtr_p=PNULL;

    UInt16 ueIndex=0;
    UInt16 qCount=0;
    UplinkContainer* cntnr_p=PNULL;
    ULResGrantInfo* ulResGrantInfo_p=PNULL;
    dispatcherArrayNode *dispatchNode_p=PNULL;
    threadSetCancelState(THREAD_CANCEL_ENABLE, NULL);
    threadSetCancelType(THREAD_CANCEL_ASYNCHRONOUS, NULL);
    threadCleanupPush(&ulTransmitter_cleanup_handler, NULL);


#ifdef UT_TESTING 
    UplinkContainer* cntnr_p_temp=PNULL;
    UInt16 qCount_temp=0;
#endif
    UInt16 qCount_old=0;
    UInt32 t1=0, t2=0, t3=0;



    //dispatcherArrayNode *tempNode_p=PNULL;
    while(1)
    {
        semWait(&transmitterSem_g);

#ifdef UT_TESTING
        cntnr_p_temp = containerPool_g[(globalTTITickCount_g-1)%8];
        qCount_temp= listCount(&(cntnr_p_temp->uldispatcherToTxList));
        if(qCount_temp!=0)
            LTE_LOG(LOG_CRITICAL,&MACModuleLogDetail_g,"globalTTITickCount_g =%ld Last container\
                    not empty qCount =%d OLD QCount=%d t1=%d, t2=%d, t3=%d",
                    globalTTITickCount_g,qCount_temp,qCount_old, t1, t2, t3);
#endif

        cntnr_p = containerPool_g[(globalTTITickCount_g)%8];
        nodePtr_p = oldNodePtr_p = getListNode(&(cntnr_p->uldispatcherToTxList),0);
        qCount = listCount(&(cntnr_p->uldispatcherToTxList));
        qCount_old=qCount;
        while( qCount != 0)
        {
            t1++;
            if(nodePtr_p == PNULL)
            {
                LTE_LOG(LOG_CRITICAL,&MACModuleLogDetail_g,"Transmitter got a NULL node freom the list");
                break;
                //continue;

            }

			if (0 == semWait(&(((ulDispatcherToTxNode*)nodePtr_p)->ulDispatcherToTxNodeSem)))
			{
                ueIndex = ((ulDispatcherToTxNode*)MEMBEROF(ulDispatcherToTxNode,
                            uldispatcherToTxNodeAnchor,oldNodePtr_p))->ueIdx;
    
                //tempNode_p = cntnr_p->dispatcherArr;
                dispatchNode_p = cntnr_p->dispatcherArr+ueIndex; 
    
                if (enableStats_g & UPLINK_STATS_ENABLED)
                {
                    ulResGrantInfo_p = &(((ulDispatcherToTxNode*)MEMBEROF(ulDispatcherToTxNode,
                                    uldispatcherToTxNodeAnchor,oldNodePtr_p))->ulResGrantInfo);
    
                    if ( ulResGrantInfo_p != PNULL  && uplinkContextInfoMap_g[ueIndex] != NULL &&
                            (!uplinkContextInfoMap_g[ueIndex]->pendingDeleteFlag) )
                    {
                        if((ulResGrantInfo_p->typeOfAllocation != ACK_UL_ALLOC_TYPE))
                        {
                            uplinkContextInfoMap_g[ueIndex]->inUseFlag |= UL_TRANSMITTER_IN_USE;
#if 0                            
                            fillULStatsInfo(uplinkContextInfoMap_g[ueIndex] ,
                                    ulResGrantInfo_p->resInfoLength  ,
                                    ulResGrantInfo_p->bsrNetTotal,
                                    ulResGrantInfo_p->typeOfAllocation );
#endif                                    
                            uplinkContextInfoMap_g[ueIndex]->inUseFlag &= UL_TRANSMITTER_NOT_IN_USE;
    
                            if(uplinkContextInfoMap_g[ueIndex]->pendingDeleteFlag)
                            {        
                                semPost(&processCtrlQueueSem_g);
                            }    
    
                        }
                    }
                }
    
                /*check if dlSchedulerInfoFlag == FALSE and ulDispatchInfo_p == NULL
                  then it means DL has already sent the information of this node 
                  . So no need to keep this node . Delete the node */
                if((dispatchNode_p->dlSchedulerInfoFlag == FALSE) && 
                        (dispatchNode_p->ulDispatchInfo_p != PNULL)
                        && dispatchNode_p->dispatcherArrInUse ==0)
                {
                    dispatchNode_p->dispatcherArrInUse |= 
                        DISPATCHER_IN_USE_BY_TRANSMITTER;
                    t2++;
                    /*remove this node and send the ulInfo to PHY*/
           		/*	if(ulThroughPutEnable_g)
           			{
                		totalUplinkBytesSend_g += dispatchNode_p->
								ulDispatchInfo_p->allocTBSize;
           			}*/
                    buildAndSendULOutGoingMessage(dispatchNode_p->ulDispatchInfo_p,1);
                    dispatchNode_p->ulDispatchInfo_p = PNULL;
                    dispatchNode_p->dispatcherArrInUse &= 
                        DISPATCHER_NOT_IN_USE_BY_TRANSMITTER;
                }
                else
                {
                    if((dispatchNode_p->dlSchedulerInfoFlag == TRUE) && 
                            (dispatchNode_p->ulDispatchInfo_p != PNULL))
                    {
                        if (dispatchNode_p->dispatcherArrInUse == DISPATCHER_IN_USE_BY_UL_SCHEDULER)
                        {
                            lteWarning("dispatcherArrInUse is set by UL scheduler,possibility of\
                                    Encoder unable to send during UL sync 0x%x",
                                    dispatchNode_p->dispatcherArrInUse);
                        }
                    }
                    /*move to next Node*/
                    //LTE_LOG(LOG_CRITICAL,&MACModuleLogDetail_g,"Transmitter got a NULL dispatchNode_p");
                    t3++;
                    /*                nodePtr_p = getNextListNode(oldNodePtr_p);
                                      listDeleteNode(&(cntnr_p->uldispatcherToTxList),oldNodePtr_p);
                                      freeMemPool(oldNodePtr_p);*/
                    //dispatchNode_p->dlSchedulerInfoFlag = FALSE;
                    //dispatchNode_p->ulDispatchInfo_p = PNULL;
                    //oldNodePtr_p = nodePtr_p;
                }
                qCount--;
                if (qCount >= 1)
                {
                    nodePtr_p = getNextListNode(oldNodePtr_p);
                    if ( (qCount_old == 1) && (0 != listCount(&(cntnr_p->uldispatcherToTxList))) )
                        ltePanic("list old count = 1 but count  != 0");
    
                }
                else
                {
                    nodePtr_p = PNULL;
                }
    	        cntnr_p->dispatcherArr[ueIndex].ulTxNode_p = PNULL;
			//    semPost(&(((ulDispatcherToTxNode*)nodePtr_p)->ulDispatcherToTxNodeSem));
                listDeleteNode(&(cntnr_p->uldispatcherToTxList),oldNodePtr_p);
                freeMemPool(oldNodePtr_p);
                oldNodePtr_p = nodePtr_p;
		    }
			else
			{
            	qCount--;
            	if (qCount >= 1)
            	{
                	nodePtr_p = getNextListNode(oldNodePtr_p);
            	}
            	else
            	{
                	nodePtr_p = PNULL;
            	}
			}
        }
        if (enableStats_g & UPLINK_STATS_ENABLED)
        {
#if 0        
            incrementULStatsTickIndex();
#endif            
        }
    }
    threadCleanupPop(1);

}
/****************************************************************************
 * Function Name  : buildAndSendULOutGoingMessage()
 * Inputs         : ulDispatcherToPhy *
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    : This function will make raw buffer from the structure and
 *                  send it to PHY
 ****************************************************************************/
void buildAndSendULOutGoingMessage(ulDispatcherToPhy *ptr_p,UInt32 type)
{
    if(ptr_p == PNULL)
    {
        return;
    }
    UInt8 *outMessage_p = PNULL;

    outMessage_p = (UInt8*)getMemFromPool(SIZE_OF_UL_GRANT_TO_PHY ,PNULL);
    //lteLog(1,"size ulDispatcherToPhy=%d",(sizeof(ulDispatcherToPhy)));

    if(PNULL == outMessage_p)
    {
        ltePanic("Unable to allocate Memory");
    }
    UInt8 *tempPtr_p=PNULL;
    tempPtr_p = outMessage_p;
    *tempPtr_p++ = UL_RESOURCE_GRANT;
    *tempPtr_p++ = (ptr_p->rnti & 0x00FF);
    *tempPtr_p++ = (ptr_p->rnti & 0x0000FF00)>>8;
    *tempPtr_p++ = (ptr_p->ttiForUE & 0x00FF) ;
    *tempPtr_p++ = (ptr_p->ttiForUE & 0x0000FF00)>>8 ;
    *tempPtr_p++ = (ptr_p->ttiForUE & 0x00FF0000)>>16;
    *tempPtr_p++ = (ptr_p->ttiForUE & 0xFF000000)>>24;

    if((ptr_p->ttiForUE <0)|| (ptr_p->ttiForUE > 4294967290ul) )
        LTE_LOG(LOG_WARNING,&MACModuleLogDetail_g,
            "\n Timer is negative or too large %u",ptr_p->ttiForUE);
    /*
     *tempPtr_p++ = ((ptr_p->rnti)>>8);
     *tempPtr_p++ = (ptr_p->rnti); 
     */
    *tempPtr_p++ = ptr_p->dciformat;
    *tempPtr_p++ = ptr_p->ackNack;
    *tempPtr_p++ = ptr_p->riv;
    *tempPtr_p++ = ptr_p->resInfoIndex;
    *tempPtr_p++ = ptr_p->resInfoLength;
    *tempPtr_p++ = ptr_p->mcs;
    *tempPtr_p++ = ptr_p->cqi;
    *tempPtr_p++ = ptr_p->srResp;
    *tempPtr_p++ = (ptr_p->allocTBSize & 0x00FF);
    *tempPtr_p++ = (ptr_p->allocTBSize & 0x0000FF00) >> 8;
    *tempPtr_p++ = (ptr_p->allocTBSize & 0x00FF0000) >> 16;
    *tempPtr_p++ = (ptr_p->allocTBSize & 0xFF000000) >> 24;
    if(ulThroughPutEnable_g)
    {
    	totalUplinkBytesSend_g += ptr_p->allocTBSize;
    }

    sendDataToPhysical(outMessage_p, SIZE_OF_UL_GRANT_TO_PHY);
    freeMemPool(outMessage_p);
    outMessage_p=PNULL;
    LTE_LOG(LOG_INFO,&MACModuleLogDetail_g,
       "Transmitter:: node Sent For RNTI %d at TTI %d :: Now Deleting Node",ptr_p->rnti,globalTTITickCount_g);
    /*printf("Transmitter:: node Sent For RNTI %d at TTI %d tbsize =%d,\
ackNack =%d dciFormat =%d, length =%d:: Now Deleting\
Node\n",ptr_p->rnti,globalTTITickCount_g,ptr_p->allocTBSize,
ptr_p->ackNack,ptr_p->dciformat, ptr_p->resInfoLength);*/

    freeMemPool(ptr_p);
    ptr_p=PNULL;
}

#endif
    
    
    
