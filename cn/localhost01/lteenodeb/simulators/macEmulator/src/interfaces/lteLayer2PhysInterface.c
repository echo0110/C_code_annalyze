
/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2PhysInterface.c,v 1.1.1.1.12.2 2010/09/20 09:07:09 gur19479 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the Interface API between the
 *                     MAC and Physical Layer in a LTE system. It performs
 *                     different functions :
 *
 *                     - Create and sends a Transport Block to the Physical
 *                       Layer.
 *                     - Receives a message from the Physical Layer
 *                     - Receive a tick indication from the Physical Layer
 *                       and sends an indication to the dispatcher, scheduler
 *                       and timer threads (Real Time Scenario)
 *                       (
 *                       For testing purposes, the message is received over 
 *                       socket instead of writing the message to the shared
 *                       memory area.
 *                       ) 
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteLayer2PhysInterface.c,v $
 * Revision 1.1.1.1.12.2  2010/09/20 09:07:09  gur19479
 * removing compilation error
 *
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3  2009/07/14 14:48:54  gur20439
 * rrc interface code change.
 *
 * Revision 1.2  2009/06/15 13:54:40  gur20439
 * modified for release 1.2.
 *
 * Revision 1.1  2009/03/30 10:20:41  gur19836
 * Initial Version
 *
 * Revision 1.38  2009/03/12 10:48:39  gur19140
 * Regarding Zero Buffer copy changes in DL
 *
 * Revision 1.37  2009/03/02 11:01:03  gur18569
 * removed funcNamp_p and replaced it with __func__
 *
 * Revision 1.36  2009/02/20 14:26:51  gur18569
 * dl throughput changes
 *
 * Revision 1.35  2009/01/29 17:56:58  gur03939
 * Changes for message queue interface with PHY SIM
 *
 * Revision 1.34  2009/01/29 11:50:57  gur03939
 * Code Changed for Uplink and Downlink throughput
 *
 * Revision 1.33  2009/01/28 12:19:55  gur03939
 * Added changes to avoid memcpy with shared memory interfaces
 *
 * Revision 1.32  2009/01/27 20:29:00  gur18569
 * added dispatcherArrInuse flag in dispatcherNode
 *
 * Revision 1.31  2009/01/27 12:22:40  gur19413
 * throughput function added
 *
 * Revision 1.30  2009/01/27 09:43:56  gur18569
 * moved code for ul sync from encoder to createAndSendTb
 *
 * Revision 1.29  2009/01/24 12:32:14  gur18569
 * replaced printf with LTE_LOG
 *
 * Revision 1.28  2009/01/24 10:17:24  gur11912
 * Regression bugs fixed
 *
 * Revision 1.27  2009/01/19 14:57:17  gur18569
 * removed UT_TESING
 *
 * Revision 1.26  2009/01/19 14:15:42  gur18569
 * adjusting rbBitMap value set or retx
 *
 * Revision 1.25  2009/01/17 20:34:06  gur11447
 * Changes done for 1 msec
 *
 * Revision 1.23  2009/01/16 14:24:35  gur18569
 * added num of lcs field in sendArqIndication
 *
 * Revision 1.22  2009/01/10 15:01:02  gur18569
 * added logs for tb verification
 *
 * Revision 1.21  2009/01/10 09:03:12  gur11912
 * Phase2 UT Defects fixed
 *
 * Revision 1.20  2009/01/08 18:56:19  gur18569
 * Review comments incorporated
 *
 * Revision 1.19  2009/01/07 17:37:45  gur11912
 * Review comments incorporated
 *
 * Revision 1.18  2009/01/07 17:19:22  gur11912
 * Review comments incorporated
 *
 * Revision 1.17  2009/01/04 07:58:40  gur11912
 * Phase 2 UT Defects Fixed
 *
 * Revision 1.16  2008/12/30 14:15:08  gur11912
 * Phase 2 UT Defect Fixed
 *
 * Revision 1.15  2008/12/26 05:50:42  gur11912
 * modified enumDataType used
 *
 * Revision 1.14  2008/12/23 12:04:18  gur11912
 * code review comments incorporated
 *
 * Revision 1.13  2008/12/23 05:06:39  gur11912
 * comment incorporatd for phase2
 *
 * Revision 1.12  2008/12/11 14:23:02  gur11912
 * Downlink Phase 2 changes
 *
 * Revision 1.11  2008/12/05 12:15:58  gur18569
 * shifted certain fields in createAndSendTBToPhysical
 *
 * Revision 1.8  2008/10/14 06:11:53  gur11912
 * Log level added
 *
 * Revision 1.7  2008/10/07 08:01:34  gur11912
 * UT Defect fixed
 *
 * Revision 1.6  2008/10/06 05:21:12  gur19836
 * UT Defect Fixed
 *
 * Revision 1.5  2008/09/27 15:45:34  gur11912
 * UT Defect Fixed
 *
 * Revision 1.4  2008/09/26 07:21:56  gur11912
 * UT Defect Fixed
 *
 * Revision 1.3  2008/09/19 07:13:36  gur11912
 * review comments incorporated
 *
 * Revision 1.2  2008/09/09 16:07:51  gur11974
 * Updated for Integration
 *
 * Revision 1.1  2008/09/09 05:12:53  gur11974
 * Initial Version
 * Rajat Budhiraja , Ravindra Patil - 0.1
 *
 ****************************************************************************/

/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/



/****************************************************************************
 * Project Includes
 ****************************************************************************/
#if 0
#include "lteLayer2PhysInterface.h"
#include "lteLayer2UplinkScheduler.h"
#include "lteLayer2UplinkTransmitter.h" 
#include "lteLayer2InitMac.h"

/****************************************************************************
 Private Definitions
 ****************************************************************************/
SInt32 physSockFD_g = 0;
SockAddr phys_addr_g = {0};

UInt32 dlStartThroughputTick_g = 0;
//#define WORKERTHREAD_PERFORMANCE
#define LOOP 1

#ifdef WORKERTHREAD_PERFORMANCE
static UInt32 loopCount = 0 ;
struct timespec tp1={0},tp2={0};
#endif
/****************************************************************************
 * Function Name  : initPhysInterface()
 * Inputs         : void
 * Outputs        : A UInt32 value indicating the return value for socket
 *                  creation.
 * Returns        : A UInt32 value indicating the return value for socket
 *                  creation.
 * Variables      :
 * Description    : This API is used to open a socket between the MAC layer
 *                  and the Physical Layer.
 *                  This function is called by the RRC interface by the 
 *                  initMACLayer().
 ****************************************************************************/
MacRetType initPhysInterface(void)
{
    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"%s Entry.\n",__func__);

    if ((physSockFD_g = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        ltePanic(" Socket creation failed.\n");
        return MAC_FAILURE;
    }
    
    phys_addr_g.sin_family = AF_INET;
    phys_addr_g.sin_port = htons(PHYSPORT);
    if (inet_aton(PHYSIP,&phys_addr_g.sin_addr) == 0)
    {
        ltePanic ("Error while converting the address.");
        return MAC_FAILURE;
    }
    
    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"%s Exit.\n",__func__);
    return MAC_SUCCESS;
}
#if 0
/****************************************************************************
 * Function Name  : createAndSendTBToPhysical
 * Inputs         : 1) sduHeader_p - A pointer to the sduHeaderInfo structure
 *                     containing information regarding the differnt logical
 *                     channels whose data need to be multiplexed while the 
 *                     formation of the Transport Block.
 *                     
 *                  2) physInfo_p - A pointer to the structure that contains
 *                     all the control information that needs to be 
 *                     forwarded to the Physical Layer along with the 
 *                     Transport Block.
 * Outputs        : A pointer to the Transport Block
 * Returns        : A pointer to the Transport Block
 * Variables      :
 * Description    : This API is used to create a Transport Block as per the 
 *                  information provided by the SDUHeaderInfo and the 
 *                  DownlinkPhysInfo structures.
 *                  It then sends the relevant information to the Physical 
 *                  Layer using the function sendDataToPhysical().
 ****************************************************************************/
UInt8* createAndSendTBToPhysical (SDUHeaderInfo* sduHeader_p, 
                                        DownlinkPhysInfo* physInfo_p,
                                        SInt8 * taOffset_p,SInt16 ueIndex)
{
    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"%s Entry.\n",__func__);
    
    //ulDispatcherToPhy *ulData_p = PNULL;
    UInt8 ulContainerIndex =0;
    SInt32 mesgBytesSent = 0;
    UInt16 index = 0, startIndex = 0;
    UInt32 transportBlockSize = 0;
    UInt32 tbBufferLen = 0;
    UInt8* bufferTB_p = PNULL;
    UInt16 tempSDUsize = 0;
    UInt16 sduPayloadInBytes = 0;
    UInt16 paddingCount = 0;
    UInt8 paddingFlag = 0;
    UInt8 count = 0;
//#define TB_BREAKUP
#ifdef TB_BREAKUP
    UInt32 tbcount = 0;
    UInt32 tempcount = 0;
#endif

    #ifdef WORKERTHREAD_PERFORMANCE
    if(0 == loopCount)
      {
       //printf("workerTh %u clock start %ld\n",workerThdCount, t1 = clock());
            clock_gettime(CLOCK_THREAD_CPUTIME_ID,&tp1);
            //printf("WORKER THREAD START updateSdu : sec %ld nsec %ld\n",tp1.tv_sec,tp1.tv_nsec);
       }
    loopCount++;
    #endif

    ulContainerIndex = globalTTITickCount_g%8;

/*    if (containerPool_g[ulContainerIndex]->dispatcherArr[ueIndex].
                                dispatcherArrInUse ==0 && ueIndex != -1)
    {
        containerPool_g[ulContainerIndex]->dispatcherArr[ueIndex]
				.dispatcherArrInUse |= DISPATCHER_IN_USE_BY_ENCODER;
        if (containerPool_g[ulContainerIndex]->dispatcherArr[ueIndex]
                    .dlSchedulerInfoFlag )
        {
            if (PNULL != (containerPool_g[ulContainerIndex]->
                dispatcherArr[ueIndex].ulDispatchInfo_p))
            {
                ulData_p = (containerPool_g[ulContainerIndex]->
                           dispatcherArr[ueIndex].ulDispatchInfo_p);
                if (ulData_p)
                {
                    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"[ENCODER][%s]synchronizing with UL in tick %u\n",
                                        __func__,globalTTITickCount_g);
                    buildAndSendULOutGoingMessage(ulData_p,2);
                    containerPool_g[ulContainerIndex]->dispatcherArr[ueIndex].
                    	ulDispatchInfo_p = PNULL;
                    containerPool_g[ulContainerIndex]->dispatcherArr[ueIndex].
                    	dlSchedulerInfoFlag = FALSE;
                }
            }
        }
        containerPool_g[ulContainerIndex]->dispatcherArr[ueIndex]
				.dispatcherArrInUse &= DISPATCHER_NOT_IN_USE_BY_ENCODER;
    }*/

    if (PNULL == physInfo_p)
    {
        lteWarning ("Received a NULL pointer for DownlinkPhysInfo.\n");
        return PNULL;
    }
    //TB Size in Bytes
    if ((physInfo_p->tbSize % ONE_BYTE) != 0)
    {
        lteWarning("The TBSize being provided is not byte - aligned.\n");
    }
    LTE_LOG (LOG_DETAIL,&MACModuleLogDetail_g,"PhysInfo TBSize %d\n", physInfo_p->tbSize);
    transportBlockSize = (physInfo_p->tbSize)/ONE_BYTE;
    tbBufferLen = CONTROL_INFO_LENGTH + DELIMITER_LENGTH + transportBlockSize;

    if(physInfo_p->newDataInd == RANDOM_ACCESS_RESPONSE ||
            physInfo_p->newDataInd == TA)
    {
        bufferTB_p = (UInt8 *)getMemFromPool(tbBufferLen ,PNULL);

        if (PNULL == bufferTB_p)
        {
            ltePanic (" [%s]Unable to allocate memory from Memory Pool RAR/TA.\n", __func__);
            return PNULL;
        }
        populateControlInformation(bufferTB_p, physInfo_p, &index);

        bufferTB_p[index++] = 0xFF;//Inserting the delimiter of all 1's 
#ifdef TB_BREAKUP
        printf("Delimiter(%d): %0x\n",index-1,bufferTB_p[index-1]);
#endif

        if(physInfo_p->newDataInd == TA)
        {
#ifdef TB_BREAKUP
        printf("TB FOR ONLY TA\n");
#endif
            if(sduHeader_p->taValue != 0)
            {    
                paddingCount = (physInfo_p->tbSize - 16)/ONE_BYTE;    
        
                if(paddingCount == 0)
                {
                    bufferTB_p[index++] = 0x1D; 
                    bufferTB_p[index++] = sduHeader_p->taValue;         
#ifdef TB_BREAKUP
        printf("Padding count ==0(%d): %0x\n",index-2,bufferTB_p[index-2]);
        printf("Padding count ==0(%d): %0x\n",index-1,bufferTB_p[index-1]);
#endif
                }
                else
                {
                    bufferTB_p[index++] = 0x3D;
                    bufferTB_p[index++] = 0x1F;
                    bufferTB_p[index++] = sduHeader_p->taValue;
#ifdef TB_BREAKUP
        printf("Padding count !=0(%d): %0x\n",index-3,bufferTB_p[index-1]);
        printf("Padding count !=0(%d): %0x\n",index-2,bufferTB_p[index-1]);
        printf("Padding count !=0(%d): %0x\n",index-1,bufferTB_p[index-1]);
        printf("\nTB_BREAKUP_END\n");
#endif
                }

            }
            else
            {
                lteWarning("[%s] TA Value Provided is Wrong\n", __func__);    
                freeMemPool((void *)bufferTB_p);
                bufferTB_p = PNULL;
                return PNULL;
            }
        }
        else
        {
#ifdef TB_BREAKUP
        printf("TB FOR RAR\n");
#endif
            memCpy((bufferTB_p + index) ,physInfo_p->data_p, /*physInfo_p->tbSize*/
                               transportBlockSize);   
#ifdef TB_BREAKUP
        for (tbcount=index; tbcount < (index+transportBlockSize); tbcount++)
        {
            LTE_LOG(LOG_INFO,&MACModuleLogDetail_g,"RAR(%d): 0x%x ",tbcount,bufferTB_p[tbcount]);
        }
        printf("\nTB_BREAKUP_END\n");
#endif    
        }

#ifdef UT_TESTING
    if (physInfo_p->dispatchedTick != globalTTITickCount_g)
    {
        LTE_LOG(1,&MACModuleLogDetail_g,"TB being sent at wrong tick . expected tick %d currenttick %d\n",
                        physInfo_p->dispatchedTick,globalTTITickCount_g);
    }
#endif
        mesgBytesSent = sendDataToPhysical (bufferTB_p,tbBufferLen);
        
        LTE_LOG(LOG_INFO,&MACModuleLogDetail_g,"[%s]tb sent of UE id %d hpid %d globaltick %d\n",
                        __func__,physInfo_p->ueID,physInfo_p->harqID,globalTTITickCount_g);

    
        if(mesgBytesSent > 0)
        {
            return bufferTB_p;    
        }
        else
        {
            lteWarning ("[%s] Unable to send TB\n", __func__);
            freeMemPool((void *)bufferTB_p);    
            bufferTB_p = PNULL;
            return PNULL;
        }
    }
   /* 
    physInfo_p->ueID = htons(physInfo_p->ueID);
    physInfo_p->tbSize = htonl(physInfo_p->tbSize);
    physInfo_p->transportChannelType = htonl(physInfo_p->transportChannelType); 
    physInfo_p->rbBitMapValue = htonl(physInfo_p->rbBitMapValue);*/

    if (physInfo_p->newDataInd)
    {
        //In case of new transmission
        if (PNULL == sduHeader_p) 
        {
            lteWarning ("Received a NULL pointer for SDUHeaderInfo \
                (New Transmission)\n");
            return PNULL;
        }
        
        if (0 == sduHeader_p->numOfLCMultiplexed)
        {
            lteWarning("The number of logical channels is 0. \n");
            return PNULL;
        }
        
        if(physInfo_p->newDataInd == NEW_RETX)
        {
            LTE_LOG(LOG_DETAIL, &MACModuleLogDetail_g,"Forming TB in case of Retransmission");        
            physInfo_p->newDataInd = RETRANSMISSION;    
        }

        if ((sduHeader_p->paddingSize % ONE_BYTE) != 0)
        {
            lteWarning("The padding size provided to Physical Interface \
                           is not byte aligned.\n");
        }
        paddingCount = (sduHeader_p->paddingSize)/ONE_BYTE;
        LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"The padding size provided to Physical Interface %d",
                        paddingCount);
         

        /*Memory Allocation for TB Buffer having length :
          control info -> (sizeof (DownlinkPhysInfo) - size for data *)
          1 -> delimiter between the control info and TB.
          transportBlockSize -> TB size.*/
    
        bufferTB_p = (UInt8 *)getMemFromPool(tbBufferLen ,PNULL);

        if (PNULL == bufferTB_p)
        {
            /*ltePanic (" [%s] Unable to allocate memory from Memory\
Pool.  New TB Buffer Length %d\n", __func__, tbBufferLen);*/
            return PNULL;
        }
        /*memSet(bufferTB_p,0,(CONTROL_INFO_LENGTH + DELIMITER_LENGTH + 
                transportBlockSize));*/
    
        /* Writing the control info starting from tbSize till end of struture
         * excluding the data pointer.*/
        
        // memCpy(&bufferTB_p[index],&(physInfo_p->tbSize),CONTROL_INFO_LENGTH);
        
        populateControlInformation(bufferTB_p, physInfo_p, &index);
        
        //index += CONTROL_INFO_LENGTH;

        bufferTB_p[index++] = 0xFF;//Inserting the delimiter of all 1's 
#ifdef TB_BREAKUP
        printf("Delimiter(%d): %0x\n",index-1,bufferTB_p[index-1]);
#endif

        if(sduHeader_p->taValue != 0)
        {
            bufferTB_p[index++] = 0x3D;   
            LTE_LOG(LOG_DETAIL, &MACModuleLogDetail_g,"[%s] TA Present in case of New TX\n", __func__);         
#ifdef TB_BREAKUP
        printf("TA_Present(%d): %0x\n",index-1,bufferTB_p[index-1]);
#endif
        }

        for (count = 0; count < sduHeader_p->numOfLCMultiplexed; count++)
        {
            if ((sduHeader_p->lcInfo_p[count].bufferLength/ONE_BYTE) <= 127)
                paddingCount++;        
        }
        
        if (paddingCount == 1)
        {
            LTE_LOG(LOG_INFO,&MACModuleLogDetail_g,"1 Padding sub-header to be applied in TB.\n");
            bufferTB_p[index++] = 0x3f;
#ifdef TB_BREAKUP
        printf("Padding count ==1(%d): %0x\n",index-1,bufferTB_p[index-1]);
#endif
        }
        else if (paddingCount == 2)
        {
            LTE_LOG(LOG_INFO,&MACModuleLogDetail_g,"2 Padding sub-headers to be applied in TB.\n");
            bufferTB_p[index++] = 0x3f;
            bufferTB_p[index++] = 0x3f;
#ifdef TB_BREAKUP
        printf("padding count ==2(%d): %0x\n",index-2,bufferTB_p[index-2]);
        printf("padding count ==2(%d): %0x\n",index-1,bufferTB_p[index-1]);
#endif
        }
        else if (paddingCount > 2)
        {
            LTE_LOG(LOG_INFO,&MACModuleLogDetail_g,"paddingFlag set,paddingCount=%d\n", paddingCount);
            paddingFlag = 1;
        }

#ifdef TB_BREAKUP
        printf("\nENCODING HEADER:\n");
        tempcount = index;
#endif
        //Encoding Headers - SDU and Padding
        for (count = 1; count <= sduHeader_p->numOfLCMultiplexed; count++)
        {
            tempSDUsize = sduHeader_p->lcInfo_p[count - 1].bufferLength/ONE_BYTE;

            bufferTB_p[index] = 0x00;
            if((count < sduHeader_p->numOfLCMultiplexed) ||
                ((count == sduHeader_p->numOfLCMultiplexed) && paddingFlag))
            {
                bufferTB_p[index] |= 0x20;
                bufferTB_p[index++] |=
                ((sduHeader_p->lcInfo_p[count - 1].lcID) & 0x1f);
            }
            else {
                bufferTB_p[index++] |=
                ((sduHeader_p->lcInfo_p[count - 1].lcID) & 0x1f);
            }

            if(tempSDUsize > 127)
            {
                bufferTB_p[index] = 0x00;
                //Setting the frame bit
                bufferTB_p[index] |= 0x80;
                bufferTB_p[index++] |= ((tempSDUsize >> 8)& 0x7f);
                bufferTB_p[index] = 0x00;
                bufferTB_p[index++] |= (tempSDUsize & 0x00ff);
            }
            else 
            {
                bufferTB_p[index] = 0x00;
                bufferTB_p[index++] |= (tempSDUsize & 0x7f);
            }
        }//end [for]

        //Setting the padding header
        if (paddingFlag) {
            bufferTB_p[index++] = 0x1f;
        }    
#ifdef TB_BREAKUP
        printf("NUM OF LCS %d\n",sduHeader_p->numOfLCMultiplexed);
        for (tbcount = tempcount; tbcount < index; tbcount++)
        {
            printf("(%d) 0x%x ",tbcount,bufferTB_p[tbcount]);
        }
        printf("\nENCODING HEADER_END\n");
#endif    
    
        if(sduHeader_p->taValue != 0)
        {
            *taOffset_p = index;
            bufferTB_p[index++] = sduHeader_p->taValue;    
            LTE_LOG(LOG_DETAIL, &MACModuleLogDetail_g,"[%s] TA %d in case of New TX\n", __func__, 
                            sduHeader_p->taValue);         
#ifdef TB_BREAKUP
        printf("TA (%d): %0x\n",index-1,bufferTB_p[index-1]);
#endif
        }

#ifdef TB_BREAKUP
        printf("\nDATA:\n");
        tempcount = index;
#endif
       UInt16 sduPayloadSize = 0; 
       UInt8 *sduFromRlc = (UInt8 *) msgAlloc(NULL, 0, 0, 0);
       if (PNULL == sduFromRlc) {
           return PNULL;
       }
        for (count = 0; count < sduHeader_p->numOfLCMultiplexed; count++)
        {
            if (PNULL == sduHeader_p->lcInfo_p[count].rlcBuffer_p)
            {
                lteWarning ("Received NULL for Logical Channel Data.\n");
                freeMemPool((void *)bufferTB_p);
                bufferTB_p = PNULL;
                return PNULL;
            }
            sduPayloadInBytes = sduHeader_p->lcInfo_p[count].bufferLength/ONE_BYTE;
            msgJoin(sduFromRlc,sduHeader_p->lcInfo_p[count].rlcBuffer_p);
            sduPayloadSize += sduPayloadInBytes;
        }

        msgExtract(sduFromRlc,0,&bufferTB_p[index],sduPayloadSize);
        index += sduPayloadSize;
        if (paddingFlag)
        {
            // TBD - memSet (&bufferTB_p[index],0,(paddingCount - 1));
            index += (paddingCount - 1);
        }

        if (index != tbBufferLen)
        {
            lteWarning("Value of index:%d, tbBufferLen:%d don't match.\n",
                            index,tbBufferLen);
        }
      

#ifdef UT_TESTING
    	if (physInfo_p->dispatchedTick != globalTTITickCount_g)
    	{
        	LTE_LOG(1,&MACModuleLogDetail_g,"TB being sent at wrong tick . expected tick %d currenttick %d\n",
                        physInfo_p->dispatchedTick,globalTTITickCount_g);
    	}
#endif
        mesgBytesSent = sendDataToPhysical (bufferTB_p,tbBufferLen);
        LTE_LOG(LOG_INFO,&MACModuleLogDetail_g,"[%s]tb sent of UE id %d hpid %d globaltick %d\n",
                        __func__,physInfo_p->ueID,physInfo_p->harqID,globalTTITickCount_g);
#ifdef TB_BREAKUP
        for (tbcount = tempcount; tbcount < tbBufferLen ; tbcount++)
        {
            printf("(%d) 0x%x ",tbcount,bufferTB_p[tbcount]);
        }
        printf("\nTB_BREAKUP_END\n");
#endif    
        LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"%s Exit.\n",__func__);
        
        if (mesgBytesSent > 0)
            return bufferTB_p;
        else
        {
            freeMemPool((void *)bufferTB_p);
            bufferTB_p = PNULL;
            physInfo_p->data_p = PNULL;
            return PNULL;
        }
    }//end [if new transmission]
    else
    {//for retransmission
        LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"Retransmitting TB");
        startIndex = 0; 
        if (physInfo_p->data_p[startIndex + CONTROL_INFO_LENGTH + DELIMITER_LENGTH - 1]
                == 0xFF)
        {
            index = startIndex + 3;
            physInfo_p->data_p[index++] = physInfo_p->dispatchedTick & 0xFF;
            physInfo_p->data_p[index++] = (physInfo_p->dispatchedTick >> ONE_BYTE) & 0xFF;
            physInfo_p->data_p[index++] = (physInfo_p->dispatchedTick >> (2*ONE_BYTE)) & 0xFF;
            physInfo_p->data_p[index++] = (physInfo_p->dispatchedTick >> (3*ONE_BYTE)) & 0xFF;
            index = startIndex + 11;
            physInfo_p->data_p[index++] = physInfo_p->rbBitMapValue & 0xFF;
            physInfo_p->data_p[index++] = (physInfo_p->rbBitMapValue >> ONE_BYTE) & 0xFF;
            physInfo_p->data_p[index++] = (physInfo_p->rbBitMapValue >> (2*ONE_BYTE)) & 0xFF;
            physInfo_p->data_p[index++] = (physInfo_p->rbBitMapValue >> (3*ONE_BYTE)) & 0xFF;
            index = startIndex + CONTROL_INFO_RETRANSMISSION + 4; // start of control info to be changed

            physInfo_p->data_p[index++] = physInfo_p->rednVersion;
            physInfo_p->data_p[index++] = physInfo_p->newDataInd;
            physInfo_p->data_p[index++] = physInfo_p->mcsIndex;

            //    memCpy(physInfo_p->data_p,&(physInfo_p->tbSize),
            //        CONTROL_INFO_LENGTH);
        }
        else
        {
            lteWarning ("The control info byte does not \
                    match while retransmission.\n");
        }
        //In case of retransmission
        if (*taOffset_p != -1)
        {
            physInfo_p->data_p[*taOffset_p] = sduHeader_p->taValue;
        }
#ifdef UT_TESTING
        if (physInfo_p->dispatchedTick != globalTTITickCount_g)
        {
            LTE_LOG(1,&MACModuleLogDetail_g,"TB being sent at wrong tick . expected tick %d currenttick %d\n",
                    physInfo_p->dispatchedTick,globalTTITickCount_g);
        }
#endif
        mesgBytesSent = sendDataToPhysical (physInfo_p->data_p,tbBufferLen);
        LTE_LOG(LOG_INFO,&MACModuleLogDetail_g,"[%s]tb sent of UE id %d hpid %d globaltick %d\n",
                __func__,physInfo_p->ueID,physInfo_p->harqID,globalTTITickCount_g);
#ifdef TB_BREAKUP
        printf("\nTB_BREAKUP_START RETX ueId %d harqId %d tb size %d taoffset %d tick %d\n",
                physInfo_p->ueID,physInfo_p->harqID,physInfo_p->tbSize,*taOffset_p,globalTTITickCount_g);
        for (; tbcount < tbBufferLen ; tbcount++)
        {
            printf("(%d) 0x%x ",tbcount,physInfo_p->data_p[tbcount]);
        }
        printf("\nTB_BREAKUP_END\n");
#endif    
        LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"%s Exit.\n",__func__);

        if (mesgBytesSent > 0)
            return physInfo_p->data_p;
        else
            return PNULL;
    }
}
/****************************************************************************
 * Function Name  : sendDataToPhysical
 * Inputs         : (For testing purposes)
 *                   bufferToSend - The buffer stream that needs to be 
 *                                  transmitted to the physical layer 
 *                                  simulator.
 *                   bufferLen    - Length in bytes of the message to be sent
 *                                  on socket.
 * Outputs        : The number of bytes sent on socket.
 * Returns        : The number of bytes sent on socket.
 * Variables      : 
 * Description    : This function is used to send the message to the physical
 *                  layer.
 *                  (For testing purposes in phase 1) - The function calls 
 *                  another method sendMsgBySocket() to send the message over
 *                  socket to the physical layer simulator. 
 ****************************************************************************/
SInt32 sendDataToPhysical (UInt8 *bufferToSend_p, UInt32 bufferLen)
{
    //This is for testing purposes where the message is send over socket to 
    //physical layer on port 5555. This needs to be replaced later by message
    //queue/shared memory interface.
    return sendMsgBySocket(bufferToSend_p,bufferLen);
}

/****************************************************************************
 * Function Name  : sendMsgBySocket
 * Inputs         : bufferToSend_p - Pointer to the buffer stream to be sent
 *                                   over socket on port 5555.
 *                  bufferLen    - Length in bytes of the message to be sent
 *                                 on socket.
 * Outputs        : The number of bytes sent on socket.
 * Returns        : The number of bytes sent on socket.
 * Variables      :
 * Description    : Function for sending the message over socket to the 
 *             physical layer on port 5555.
 ****************************************************************************/
SInt32 sendMsgBySocket(UInt8 *bufferToSend_p,UInt32 bufferLen)
{
    return sendto(physSockFD_g,(UInt8 *)bufferToSend_p,bufferLen,0,
            (const SockAddr *)&phys_addr_g,sizeof (phys_addr_g));
}

/****************************************************************************
 * Function Name  : receiveTTITickFromPhysical
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : Function for indicating the TTI start to the dispatcher,
 *                  scheduler and timer threads.(Not to be used for 
 *                  testing purposes).
 ***************************************************************************/
void receiveTTITickFromPhysical (void)
{
    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"%s Entry.\n",__func__);
    
    globalTTITickCount_g++; 
    semPost(&schedulerWaitForTTISem_g);
    semPost(&timerWaitForTTISem_g);
    semPost(&distributorWaitForTickSem_g);
    /*Fix for __SPR_637__ START */
    macSubFrameInd();
    /*Fix for __SPR_637__ END */
    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"%s Exit.\n",__func__);
}

/****************************************************************************
 * Function Name  : recvDataFromPhysical
 * Inputs         : 
 * Outputs        : 
 * Returns        : 
 * Variables      :
 * Description    : Function for receiving the data from the Physical Layer 
 *             and then writing the data in the RxDataQueue.
 *             (Not to be used during testing purposes).
 ***************************************************************************/
void recvDataFromPhysical (void* args_p)
{
    //To be populated when we have the receiver queue.
}

#endif

/****************************************************************************
 * Function Name  : populateControlInformation 
 * Inputs         : bufferTB_p - pointer to the buffer where control info is 
 *                               to be written
 *                  physInfo_p - pointer to DownlinkPhysInfo structure
 *                  index_p -  position from which control info starts
 * Outputs        : Fills control information in the buffer
 * Returns        : None
 * Variables      : 
 * Description    : This function populates the control fields of the downlink
 *                  transmission.  
 ***************************************************************************/
void populateControlInformation(UInt8 *bufferTB_p, DownlinkPhysInfo *physInfo_p,
                        UInt16 *index_p)
{
#ifdef TB_BREAKUP
    UInt32 tbcount=0;
#endif
    bufferTB_p[(*index_p)++] = physInfo_p->transportChannelType;

    bufferTB_p[(*index_p)++] = physInfo_p->ueID & 0xFF;
    bufferTB_p[(*index_p)++] = (physInfo_p->ueID >> ONE_BYTE) & 0xFF;

    bufferTB_p[(*index_p)++] = physInfo_p->dispatchedTick & 0xFF;
    bufferTB_p[(*index_p)++] = (physInfo_p->dispatchedTick >> ONE_BYTE) & 0xFF;
    bufferTB_p[(*index_p)++] = (physInfo_p->dispatchedTick >> (2*ONE_BYTE)) & 0xFF;
    bufferTB_p[(*index_p)++] = (physInfo_p->dispatchedTick >> (3*ONE_BYTE)) & 0xFF;
 
    bufferTB_p[(*index_p)++] = physInfo_p->tbSize & 0xFF;
    bufferTB_p[(*index_p)++] = (physInfo_p->tbSize >> ONE_BYTE) & 0xFF;
    bufferTB_p[(*index_p)++] = (physInfo_p->tbSize >> (2*ONE_BYTE)) & 0xFF;
    bufferTB_p[(*index_p)++] = (physInfo_p->tbSize >> (3*ONE_BYTE)) & 0xFF;
    
    bufferTB_p[(*index_p)++] = physInfo_p->rbBitMapValue & 0xFF;
    bufferTB_p[(*index_p)++] = (physInfo_p->rbBitMapValue >> ONE_BYTE) & 0xFF;
    bufferTB_p[(*index_p)++] = (physInfo_p->rbBitMapValue >> (2*ONE_BYTE)) & 0xFF;
    bufferTB_p[(*index_p)++] = (physInfo_p->rbBitMapValue >> (3*ONE_BYTE)) & 0xFF;
       
    bufferTB_p[(*index_p)++] = physInfo_p->numRB;
    bufferTB_p[(*index_p)++] = physInfo_p->antennaMapping;
    bufferTB_p[(*index_p)++] = physInfo_p->harqID;
    bufferTB_p[(*index_p)++] = physInfo_p->rednVersion;
    bufferTB_p[(*index_p)++] = physInfo_p->newDataInd;
    bufferTB_p[(*index_p)++] = physInfo_p->mcsIndex;
#ifdef TB_BREAKUP
        printf("\nTB_BREAKUP_START ueId %d harqId %d tb size %d tick %d\nCONTRL INFO\n",
                        physInfo_p->ueID,physInfo_p->harqID,physInfo_p->tbSize,globalTTITickCount_g);
        for (; tbcount < *index_p ; tbcount++)
        {
            printf("(%d) 0x%x ",tbcount,bufferTB_p[tbcount]);
        }
        printf("\n--------------\n");
#endif    
}

/*
void displayDwlkThroughPut()
{
    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"[%s] Entry",__func__);
   // LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"Total Data Read = %u, duration_g =%u",throughputBytesRead_g,duration_g);
    LTE_LOG(LOG_CRITICAL,&MACModuleLogDetail_g,"ThroughPut  =%u bits/sec", totalDwlkBytesSend_g/THROUGHPUT_DURATION);
    totalDwlkBytesSend_g = 0;
    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"[%s] Exit",__func__);
}
*/
#endif
