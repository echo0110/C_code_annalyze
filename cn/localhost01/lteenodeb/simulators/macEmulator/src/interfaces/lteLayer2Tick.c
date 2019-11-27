
/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2Tick.c,v 1.1.1.1 2010/02/11 04:51:22 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file is to be used for testing purposes.
 *                     It contains the function that indicates the 1 milli - 
 *                     second tick to the dispatcher, scheduler and timer 
 *                     threads apart from updating the global tick count 
 *                     variable.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteLayer2Tick.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2  2009/07/14 14:48:54  gur20439
 * rrc interface code change.
 *
 * Revision 1.1  2009/03/30 10:20:41  gur19836
 * Initial Version
 *
 * Revision 1.24  2009/03/02 10:59:40  gur18569
 * removed funcNamp_p and replaced it with __func__
 *
 * Revision 1.23  2009/01/29 18:52:23  gur03939
 * Modified nanosleep to pselect
 *
 * Revision 1.22  2009/01/29 12:26:39  gur19413
 * remove UT_TESTING
 *
 * Revision 1.21  2009/01/29 11:10:16  gur19413
 * updated
 *
 * Revision 1.20  2009/01/25 20:26:45  gur19413
 * Updated
 *
 * Revision 1.19  2009/01/24 10:17:24  gur11912
 * Regression bugs fixed
 *
 * Revision 1.18  2009/01/20 16:18:45  gur11912
 * Updated for Porting
 *
 * Revision 1.17  2009/01/20 07:47:00  gur19836
 * Changes done for interface with RRC Layer
 *
 * Revision 1.16  2009/01/20 07:17:55  gur19413
 * updated
 *
 * Revision 1.15  2009/01/17 20:37:56  gur11447
 * Changes for 1 msec
 *
 * Revision 1.14  2009/01/02 07:46:58  gur11447
 * nanosleep guarded by a UT_TESTING flag -- KT
 *
 * Revision 1.13  2009/01/01 11:22:05  gur11447
 * Changes for NanoSleep, for proper Tick -- KT
 *
 * Revision 1.12  2008/12/26 05:56:41  gur11912
 * semaphore posted if value is 0
 *
 * Revision 1.11  2008/12/19 11:54:09  gur18569
 * replaced pthread sys calls with wrappers
 *
 * Revision 1.10  2008/12/11 14:24:12  gur11912
 * Downlink Phase 2 changes
 *
 * Revision 1.9  2008/12/11 05:09:09  gur12905
 * Updated
 *
 * Revision 1.8  2008/12/11 05:07:20  gur12905
 * Updated for transmitterSem_g and waitForULSchedularContainer_g semaphores post operation
 *
 * Revision 1.7  2008/12/05 12:41:55  gur18569
 * multicore changes
 *
 * Revision 1.6  2008/10/22 12:37:50  gur11912
 * fixed for performance changes
 *
 * Revision 1.5  2008/10/17 09:24:00  gur19836
 * Enhancement: cleanupMACLayer functionality added
 *
 * Revision 1.4  2008/10/14 06:19:29  gur11912
 * Log Level added
 *
 * Revision 1.3  2008/09/19 07:13:36  gur11912
 * review comments incorporated
 *
 * Revision 1.2  2008/09/09 16:08:41  gur11974
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
#if 0
#include <time.h>

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteLayer2Tick.h"

LTE_SEM* phySemTimer_g;
UInt32 *shMemTimer_g;

/****************************************************************************
 Private Definitions
 ****************************************************************************/
LTE_SEM timerWaitForTTISem_g;
extern LTE_SEM waitForULSchedularContainer_g;
extern LTE_SEM transmitterSem_g;
extern SInt32 sendDataToPhysical (UInt8 *bufferToSend_p, UInt32 bufferLen);

void initShMemForTimerSync();

void tick_cleanup_handler(void *arg)
{
    printf("In TTI Tick Thread - cleanup handler\n");
}

/****************************************************************************
 * Function Name  : initTickThread
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Variables      : 
 * Description    : This is the entry function for the tick thread to be used
 *                  for testing purposes in the absence of the physical layer.
 *                  It contains an infinite while loop containing the nano - 
 *                  sleep that after every 1 ms 
 *                  - increments the global tick count variable.
 *                  - posts the semaphore variables for the scheduler, 
 *                    dispatcher and timer threads.
 *                  
 ****************************************************************************/

void* initTickThread(void *args)
{
    LTE_LOG (LOG_DETAIL,"%s Entry.\n",__func__);

    struct timespec timer;
    SInt32 val =0;
    //struct timespec t1={0},t2={0};

    fd_set rfds;
    //struct timeval time_start, time_stop;
    SInt32 retval=0;


    threadSetCancelState(THREAD_CANCEL_ENABLE, NULL);
    threadSetCancelType(THREAD_CANCEL_ASYNCHRONOUS, NULL);
    threadCleanupPush(&tick_cleanup_handler, NULL);

    //SInt8 timer_ret;

    timer.tv_nsec = 1000000 * TICK_IN_MS;
    timer.tv_sec = 0;

    SockAddr timerAddr;  
    bzero(&(timerAddr),sizeof(timerAddr));
    SInt32 timerSockfd = -1;
    if ((timerSockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("Socket Failed for PHY Timer Port");
        timerSockfd = -1;
    }

    timerAddr.sin_family = AF_INET;
    timerAddr.sin_port = htons(PHY_TTI_PORT);
    timerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    timer.tv_nsec = 0;
    timer.tv_sec = 1;

    FD_ZERO (&rfds);
    FD_SET (0, &rfds);

	while (1)
	{
	/*      timer_ret=0;

		do{
			if(timer_ret==0)
			{

			}
			else
			{
				timer.tv_nsec=timer_rem.tv_nsec;
				timer.tv_sec=timer_rem.tv_sec;
			}
		timer_ret=nanosleep ((const TimeSpec *)&timer, &timer_rem);

		}while(timer_ret<0);
              */

           /* Wait up to five seconds. */

           /*gettimeofday(&time_start, NULL);*/
           retval = pselect(1, &rfds, NULL, NULL, &timer, NULL);
           /*gettimeofday(&time_stop, NULL);*/

          if (retval == -1)
          {
		perror("select failed\n");
          } 

/*           printf("start time %lu:%lu, stop time %lu:%lu\n",
                        time_start.tv_sec,time_start.tv_usec,
                        time_stop.tv_sec,time_stop.tv_usec);*/
	   
	   globalTTITickCount_g++;

	   /*semPost(&schedulerWaitForTTISem_g);
	   semPost(&timerWaitForTTISem_g);
	   semPost(&distributorWaitForTickSem_g);
	   semPost(&waitForULSchedularContainer_g);
	   semPost(&transmitterSem_g);*/

		semGetVal(&schedulerWaitForTTISem_g, &val);
		if(0 == val)
		{
			semPost(&schedulerWaitForTTISem_g);
		}

		semGetVal(&distributorWaitForTickSem_g, &val);
		if(0 == val)
		{
			semPost(&distributorWaitForTickSem_g);
		}

		semGetVal(&timerWaitForTTISem_g, &val);
		if(0 == val)
		{
			semPost(&timerWaitForTTISem_g);
		}

		semGetVal(&waitForULSchedularContainer_g, &val);
		if(0 == val)
		{
			semPost(&waitForULSchedularContainer_g);
		}

		semGetVal(&transmitterSem_g, &val);
		if(0 == val)
		{
			semPost(&transmitterSem_g);
		}
              if (timerSockfd > 0)
               {
                  UInt8 *outTimer_p = (UInt8*)getMemFromPool(TIMER_MSG_SIZE,PNULL);
                  if(PNULL == outTimer_p)
                   {
                      ltePanic("Unable to allocate Memory");
                   }
                   UInt8 *tempPtr_p=outTimer_p;
                   *tempPtr_p++ = MAC_PHY_TIMER_SYNC;
                   *tempPtr_p++ = globalTTITickCount_g & 0xFF;
                   *tempPtr_p++ = (globalTTITickCount_g & 0xFF00) >> ONE_BYTE;
                   *tempPtr_p++ = (globalTTITickCount_g & 0xFF0000) >> 2*ONE_BYTE;
                   *tempPtr_p++ = (globalTTITickCount_g & 0xFF000000) >> 3*ONE_BYTE;
                 // printf("\n Sending Tick to PHY %u\n",globalTTITickCount_g);
                   sendto(timerSockfd,outTimer_p,TIMER_MSG_SIZE, 0,
                         (struct sockaddr*)&timerAddr, sizeof(SockAddr));

                    // sendDataToPhysical(outTimer_p,5);
                   freeMemPool(outTimer_p);
                } 
	}
	threadCleanupPop(1);
	LTE_LOG (LOG_DETAIL,"%s Exit.\n",__func__);
	return PNULL;
}

/****************************************************************************
 * Function Name  : initShMemForTimerSync
 * Inputs         :
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    :
 *
 ****************************************************************************/
void initShMemForTimerSync()
{
    int shmfd;
 
    LTE_LOG(LOG_INFO," [%s] Entry ",__func__);
    phySemTimer_g = sem_open(PHY_TTI_SEM, O_CREAT, 0777, 1);
    if (phySemTimer_g == SEM_FAILED)
    {
         sem_close(phySemTimer_g);
         ltePanic("In sem_open()%d  %p",errno,phySemTimer_g);
    }
    shmfd = shm_open(SHMOBJ_TIMER_PATH, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);

    if (shmfd < 0)
    {
        ltePanic("In shm_open()");
    }
    ftruncate(shmfd, SHARED_SEG_SIZE);

    shMemTimer_g = (UInt32 *)mmap(NULL, SHARED_SEG_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    if (shMemTimer_g == NULL) {
        ltePanic("In mmap()");
    }
    LTE_LOG(LOG_INFO,"%s: Shared memory segment allocated successfully (%d bytes).\n",
             __func__, SHARED_SEG_SIZE);
    LTE_LOG(LOG_INFO," [%s] Exit ",__func__);
}

#endif
