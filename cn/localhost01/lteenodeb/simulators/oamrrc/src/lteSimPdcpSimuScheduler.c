/**************************************************************************************
*
*  FILE NAME  : simuScheduler.c
*
*  DESCRIPTION: External scheduler implementation.
*
*
*	DATE	NAME	REFERENCE	REASON
*
*  Copyright 2009, Aricent Inc.
*
*************************************************************************************/

/****************************************************************************
 * Includes
 ****************************************************************************/
#ifdef NEHALEM_ONETHREAD
#define _GNU_SOURCE
#include <pthread.h>
#include <sched.h>
#endif

#include "lteSimPdcpEsLteFramework.h"

#include <ltePdcpLteDefs.h>
#include "lteSimPdcpSimuScheduler.h"
#include "lteSimPdcpSockTools.h"
#include "lteSimPdcpUserUtils.h"
#include "ltePdcpTlvUtils.h"
#include "lteOamSimulator.h"
#include "lteSimPdcpCallbacks.h"
#include "lteLayer2CommanTypes.h"
/****************************************************************************
 * Global Variables
 ****************************************************************************/
LTE_SEM     es_sem_notify; /* Sheduler notification semaphore   */
extern UChar8 *arrMACResp[50];
extern UChar8 *arrRLCResp[50];
extern void printMacStats(UInt8 *data_p,UInt16 *current,UInt16 msgLen);
extern void printRlcStats(UInt8 *data_p,UInt16 *current,UInt16 msgLen);
extern void printRlcStatus(UInt8 *data_p,UInt16 *current,UInt16 msgLen);
extern void printMacStatus(UInt8 *data_p,UInt16 *current,UInt16 msgLen);

#ifdef ZPPP_INTEGRATION
extern SInt32 sockRx_g;
extern struct sockaddr_un sockAddrTxRx_g;
extern SInt32 sockTxRx_g;

#ifdef TWO_UE_WORK

zppp_context zppp_flag_g[2];
extern SInt32 sockRx_1_g;
extern struct sockaddr_un sockAddrTxRx_1_g;
extern SInt32 sockTxRx_1_g;

#endif

#endif


/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* Execution Sheduler task for RX. It do the follwoing:
    - call to pdcpElTxProcessSocket execution leg;
 */
static void* esPdcpRxTask(void* lpParam);

/* Execution leg for processing incoming UDP messages
 */
static void pdcpElTxProcessSocket(void);
/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

static pthread_t   es_tRxId; /* Sheduler RX task Id */
static LTE_SEM     es_sem_stop; /* Stop Sheduler task semaphore   */

/* externs*/
extern UChar8 throughputFile_g[256];
extern FILE* throughputFD_g;
extern UInt16 interval_g;
extern UInt8 startCaptureThroughput_g;

/****************************************************************************
 * Function Name  : esStart 
 * Inputs         : .
 * Outputs        : None.
 * Returns        : None.
 * Description    : Runs RLC layer simulator.
 ****************************************************************************/
void    esStart()
{
#ifdef NEHALEM_ONETHREAD
    cpu_set_t cpuSetSch;
    Int res = 0 ;
    Int coreNum = 0 ;
#endif

    PDCP_LOG(LOG_DUMP, "Started RX Thread %s", __FUNCTION__);
    semInit(&es_sem_stop, 0);
    semInit(&es_sem_notify, 1);

    /* create RX task */
#ifdef NEHALEM_ONETHREAD
    threadCreate(&es_tRxId, THREAD_CREATE_DETACHED,
        THREAD_SCHED_FIFO, 0, THREAD_EXPLICIT_SCHED, THREAD_SCOPE_SYSTEM,
        &esPdcpRxTask, PNULL);
#else
    threadCreate(&es_tRxId,THREAD_DEFAULT_STATE,THREAD_SCHED_DEFAULT,0, 
        THREAD_INHERIT_SCHED_DEFAULT, THREAD_SCOPE_DEFAULT,&esPdcpRxTask, PNULL);
#endif   


#if 0
/* Rohit: Code shd be added on RMI for thread binding */
#ifdef NEHALEM_ONETHREAD
    CPU_ZERO(&cpuSetSch);
    coreNum = 1;
    CPU_SET(coreNum , &cpuSetSch);
    res = pthread_setaffinity_np(es_tRxId,
        sizeof(cpuSetSch), &cpuSetSch);
    if (res != 0 )
    {
      ltePanic(" Unable to set affinity for OAMRRC Thread.\n");
      return;
    }
    fprintf(stderr, "##OAMRRC Thread masked at coreNum %d\n", coreNum);
#endif
#endif
}

/****************************************************************************
 * Function Name  : esStop 
 * Inputs         : .
 * Outputs        : None.
 * Returns        : None.
 * Description    : Stops RLC layer simulator.
 ****************************************************************************/
void    esStop()
{
    void *status;
    semPost(&es_sem_stop);
    semPost(&es_sem_notify);
    threadJoin(es_tRxId, &status);
}

/****************************************************************************
 * Function Name  : pdcpElTxProcessSocket 
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Process incoming UDP messages
 ****************************************************************************/
void pdcpElTxProcessSocket(void)
{
    UInt8 buf[SOCKET_BUFFER_SIZE];
    SInt32 length;
    static UInt16 tickInMs = 0; 
    static UInt32 count = 0;
#ifdef ZPPP_INTEGRATION
    socklen_t addrLen = 0;
    addrLen = sizeof(sockAddrTxRx_g);
#endif
    UInt16 apiID = 0, current = 0, response = 0;
    UInt8 *data_p = PNULL ;
    UInt16 transactionId = 0, srcModId = 0, destModId = 0, msgLen = 0;
    if(startCaptureThroughput_g == 1)
    {
        if(!(tickInMs++%interval_g)) 
        {
            UChar8 reqMsg[10] ={ 0x00 ,0x0a, 0x00, 0x04, 0x00, 0x01, 0x00, 0x3e,0x00,0x0a}; // 

            if ( WriteToSocket((UInt8*)reqMsg, (UInt32)10, (UInt16)1 ) != 10 )
            {
                PDCP_LOG( LOG_WARNING, "%s:Problem sending to MAC socket", __FUNCTION__ );
            }
        }
    }

    length = ReadFromSocket( buf, OAM_MODULE_ID );
    if(length>0) /*packet length is correct according to header*/
    {
        data_p = (UInt8 *)buf;
        transactionId = LTE_GET_U16BIT(&data_p[current]);
        current += 2;
        srcModId = LTE_GET_U16BIT(&data_p[current]);
        current += 2;
        destModId = LTE_GET_U16BIT(&data_p[current]);
        current += 2;
        apiID = LTE_GET_U16BIT(&data_p[current]);
        current += 2;
        msgLen = LTE_GET_U16BIT(&data_p[current]);
        current += 2;
        data_p = (UInt8 *)buf;
        userPrintRawBuffer("OAM-MSG RECV\t", buf, length, 0);

        if(apiID == 0xFFF0 )/* this only unique id from client */
        {
	    UChar8* fileName = (UChar8*)(data_p + current);	
            PDCP_LOG(LOG_DUMP,"\t\t Client Request (%s)",fileName);
            userRunScriptCallback(1, (const UChar8**)&fileName);
            /* need to copy data_p in heap memory*/
        }

        if(MAC_MODULE_ID == srcModId)   //MAC
        {
            response = LTE_GET_U16BIT(&data_p[current]);
            current += 2;
            if (MAC_GET_STATS_CNF == apiID)
            {
//                lteLog(LOG_DUMP,PNULL," MAC RESP CODE [%u] and DESP [%s]\n",response, arrMACResp[response]);
                if (response)
                    printMacStats(data_p, &current, msgLen);
            }
            else if (MAC_GET_STATUS_CNF == apiID)
            {
//                lteLog(LOG_DUMP,PNULL," MAC RESP CODE [%u] and DESP [%s]\n",response, arrMACResp[response]);
                if (response)
                    printMacStatus(data_p, &current, msgLen);      
            }
            else if (MAC_GET_BUILD_INFO_CNF == apiID)
            {
                PDCP_LOG(LOG_DUMP,"\t\t Build Info for MAC (%s)\n",&data_p[current]);
            }
            /* To extract throughput */ 
            else if(apiID == 63){
                UInt16 throughputTag = 0;
                UDouble32 ult=0,dlt=0;
                while(current != msgLen)
                {
                    throughputTag = LTE_GET_U16BIT(&data_p[current]);
                    current += 2;
                    current += 2;
                    switch(throughputTag)
                    {
                        case MAC_DL_THROUGHPUT:
                            memCpy(&dlt, &data_p[current], 4);
                            lteLog(1,PNULL, "Downlink Throughput = %f\n", dlt);
                            current += 4;
                            break;
                        case MAC_UL_THROUGHPUT:
                            memCpy(&ult, &data_p[current], 4);
                            lteLog(1,PNULL, "Uplink Throughput = %f\n", ult);
                            current += 4;
                            break;
                    }
                    /* write to log file */
                    if (throughputFD_g != NULL){
                        fprintf(throughputFD_g,"\t\t%d,\t%6f,\t\t%6f\n",++count,dlt,ult);
                    }
                    else {
                        //PDCP_LOG(LOG_DUMP,"File Named %s is not opened\n",throughputFile_g);
                    }
                }
            }    
        }
        else if(RLC_MODULE_ID == srcModId)
        {
            response = LTE_GET_U16BIT(&data_p[current]);
            current += 2;
            msgLen -= 10;
            if (RLC_GET_STATS_CNF == apiID)
            {
                lteLog(LOG_DUMP,PNULL," RLC RESP CODE [%u] and DESP [%s]\n",response, arrRLCResp[response]);
                if (response)
                    printRlcStats(data_p, &current, msgLen);
            }
            else if (RLC_GET_STATUS_CNF == apiID)
            {
                lteLog(LOG_DUMP,PNULL," RLC RESP CODE [%u] and DESP [%s]\n",response, arrRLCResp[response]);
                if (response)
                    printRlcStatus(data_p, &current, msgLen);
            }  
            else if (RLC_GET_BUILD_INFO_CNF == apiID)
            {
                PDCP_LOG(LOG_DUMP,"\t\t Build Info for RLC (%s)\n",&data_p[current-2]);
            } 
        }
        else if(PDCP_MODULE_ID == srcModId)
        {
            response = LTE_GET_U16BIT(&data_p[current]);
            current += 2;
            //         lteLog(LOG_DUMP,PNULL," PDCP RESP CODE [%u] and DESP [%s]\n",response, arrPDCPResp[response]);
            if (PDCP_GET_STATS_CNF == apiID)
            {
                if(response)
                    printPdcpStats(data_p,&current,msgLen);
            }
            else if (PDCP_GET_STATUS_CNF == apiID)
            {
                //if(response)
                //printPdcpStatus(data_p, &current, msgLen);
            }
            else if (PDCP_GET_BUILD_INFO_CNF == apiID)
            {
                UInt8 *data_p = ((TlvMessage *)buf)->data;
                PDCP_LOG(LOG_DUMP,"\t\t BuildInfo for PDCP (%s)",data_p);
            }
        }
        PDCP_LOG(LOG_DUMP,"OAM-MSG RECV\t: AppId[%i], udpLen[%i]", apiID, length);
    }

#ifdef ZPPP_INTEGRATION
    /* receive data from zppp on unix socket for signalling packets */
    length = recvfrom(sockTxRx_g,buf,SOCKET_BUFFER_SIZE,0,(struct sockaddr *)&sockAddrTxRx_g,&addrLen);
    if ( -1 == length )
    {
        if ( errno != EAGAIN )
        {
            fprintf(stderr,"\n\nrecvfrom form sockTxRx_g fd = %d failed\n",sockTxRx_g);
            ltePanic ("Error in receive message.\n");
        }
#ifndef TWO_UE_WORK
        else
            return;
#endif
    }
    else if ( length > 0 )
    {
        fprintf(stderr,"recevied data from Ist zppp of size = %d\n",length);
        /* for attach the buffer will be 0x01 0x00 */
        if ( buf[0] && buf[1] != 2 )
        {
#ifdef TWO_UE_WORK
            if ( ( buf[0] == 0x01 && buf[1] == 0x00 )  && (zppp_flag_g[0].inUseFlag == 0)) 
            {
                zppp_flag_g[0].inUseFlag = 1;
                zppp_flag_g[0].fd = sockTxRx_g; 
		fprintf(stderr,"recevied data from Ist zppp of size = %d on fd = %d attach \n",length,sockTxRx_g);
            }
            else if ( ( buf[0] == 0x01 && buf[1] == 0x01 )  && (zppp_flag_g[0].inUseFlag == 6)) 
            {
                zppp_flag_g[0].inUseFlag = 1;
                zppp_flag_g[0].fd = sockTxRx_g; 
		fprintf(stderr,"recevied data from Ist zppp of size = %d on fd = %d\n",length,sockTxRx_g);
            }
            else 
            {
                fprintf(stderr,"\n\n\n------ Wrong state ----- flag = %d\n\n\n",zppp_flag_g[0].inUseFlag);
                return;
            }
            
#endif
            handleZpppControlPacket(buf,length);
        }
    }

#ifdef TWO_UE_WORK
    /* receive data from 2nd zppp on unix socket for signalling packets */
    length = recvfrom(sockTxRx_1_g,buf,SOCKET_BUFFER_SIZE,0,(struct sockaddr *)&sockAddrTxRx_1_g,&addrLen);
    if ( -1 == length )
    {
        if ( errno != EAGAIN )
        {
            fprintf(stderr,"\n\nrecvfrom form sockTxRx_1_g fd = %d failed\n",sockTxRx_1_g);
            ltePanic ("Error in receive message.\n");
        }
        else
            return;
    }
    else if ( length > 0 )
    {
        /* for attach the buffer will be 0x01 0x00 */
        if ( buf[0] && buf[1] != 2)
        {
            if ( ( buf[0] == 0x01 && buf[1] == 0x00 )  && (zppp_flag_g[1].inUseFlag == 0)) 
            {
                zppp_flag_g[1].inUseFlag = 1;
                zppp_flag_g[1].fd = sockTxRx_1_g;
		fprintf(stderr,"recevied data from 2nd zppp of size = %d on fd = %d attach\n",length,sockTxRx_1_g);
                 
            }
            else if ( ( buf[0] == 0x01 && buf[1] == 0x01 )  && (zppp_flag_g[1].inUseFlag == 6)) 
            {
                zppp_flag_g[1].inUseFlag = 1;
                zppp_flag_g[1].fd = sockTxRx_1_g; 
		fprintf(stderr,"recevied data from 2nd zppp of size = %d on fd = %d\n",length,sockTxRx_1_g);
            }
            else 
            {
                fprintf(stderr,"\n\n\n------ Wrong state ----- flag = %d\n\n\n",zppp_flag_g[0].inUseFlag);
                return;
            }
            handleZpppControlPacket(buf,length);
        }
        else
        {
            fprintf(stderr,"Error...!!data packet recived %d\n",buf[0]);
        }
    }

    
#endif
     
#endif
}

/****************************************************************************
 * Function Name  : esPdcpRxTask 
 * Inputs         : lpParam - not used.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Execution Sheduler task for PDCP RX Path. It do the follwoing:
    - call to pdcpElRxProcessMessage execution leg;
 ****************************************************************************/
void* esPdcpRxTask(void* lpParam)
{
    SInt32  sem_state;  
    PDCP_LOG(LOG_DUMP, "Start %s", __FUNCTION__);
    do {
    semWait(&es_sem_notify);
    pdcpElTxProcessSocket();
    semPost(&es_sem_notify);
    userSleep(1);
    /* get and check stop task condition */
    sem_state = semTryWait(&es_sem_stop);
    } while (!sem_state);
    semPost(&es_sem_stop);
    PDCP_LOG(LOG_DUMP, "Stop %s", __FUNCTION__);
    return PNULL;
}
