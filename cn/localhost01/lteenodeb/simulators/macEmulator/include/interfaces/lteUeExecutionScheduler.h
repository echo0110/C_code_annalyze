
/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteUeExecutionScheduler.h,v 1.1.1.1.6.2 2010/08/02 08:57:18 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : Physical simulator Header File
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteUeExecutionScheduler.h,v $
 * Revision 1.1.1.1.6.2  2010/08/02 08:57:18  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1.2.7.2.6  2009/12/05 13:04:18  gur19479
 * updated to remove unncessary flag
 *
 * Revision 1.1.2.7.2.5  2009/11/30 06:55:50  gur20439
 * PHY CREATE, DELETE, RECONFIGURE UE Entity req handling modified to support UUnt16 ueIndex value with RRC 0.5 code.
 *
 * Revision 1.1.2.7.2.4  2009/11/26 11:39:29  gur20439
 * API id changed as per RRC 0.5
 *
 * Revision 1.1.2.7.2.3  2009/11/09 06:58:52  gur20439
 * UT bug fix in Uplink flow.
 *
 * Revision 1.1.2.7.2.2  2009/10/23 05:43:39  gur20439
 * UT Bug fix and enhancement.
 *
 * Revision 1.1.2.7.2.1  2009/09/30 10:52:27  gur20439
 * eNodebPhy related changes for mwc demo.
 *
 * Revision 1.1.2.7  2009/09/03 06:15:11  gur19140
 * adding cycle count code
 *
 * Revision 1.1.2.6  2009/08/07 10:05:55  gur19836
 * msgQId corrected
 *
 * Revision 1.1.2.5  2009/08/06 13:26:19  gur19836
 * Graceful Exit code added
 *
 * Revision 1.1.2.4  2009/08/03 16:14:32  gur19140
 * memory leak debuugging
 *
 * Revision 1.1.2.3  2009/07/15 15:24:37  gur19836
 * UE Sim build with Adapter
 *
 * Revision 1.1.2.2  2009/06/24 06:15:27  gur19140
 * shared memory macro change
 *
 * Revision 1.1.2.1  2009/06/19 13:39:13  gur19140
 * *** empty log message ***
 *
 * Revision 1.24.2.1  2009/06/19 13:34:02  gur19140
 * Intergration second wave
 *
 * Revision 1.24  2009/04/10 17:26:26  gur18569
 * merged with optmization changes
 *
 * Revision 1.23  2009/03/02 11:07:05  gur18569
 * added ueTotalQloadsem
 *
 * Revision 1.22  2009/02/23 11:25:02  gur10445
 *  Flexible LCs Qload added
 *
 * Revision 1.21  2009/02/17 13:45:54  gur10445
 * Ul Traffic Gen
 *
 * Revision 1.20  2009/02/10 06:24:40  gur19413
 * updated
 *
 * Revision 1.19  2009/01/31 11:06:10  gur03939
 * Modified code to send DL ACK NACK through UL Dispatcher thread
 *
 * Revision 1.18  2009/01/29 18:56:42  gur03939
 * Modified code for socket interface with PHY SIM
 *
 * Revision 1.17  2009/01/29 11:37:22  gur19413
 * updated
 *
 * Revision 1.16  2009/01/28 19:24:19  gur19413
 * updated
 *
 * Revision 1.15  2009/01/25 20:48:29  gur19413
 * updated
 *
 * Revision 1.14  2009/01/25 11:54:21  gur20052
 * Added During Regression Testing
 *
 * Revision 1.13  2009/01/24 13:51:14  gur18569
 * changed bandnum and cqi index of size one byte
 *
 * Revision 1.12  2009/01/20 16:24:06  gur11912
 * Updated for Porting
 *
 * Revision 1.11  2009/01/17 20:44:14  gur11447
 * Changes for 1 msec
 *
 * Revision 1.10  2009/01/08 16:18:44  gur11447
 * *** empty log message ***
 *
 * Revision 1.8  2009/01/01 22:24:07  gur11447
 * Changes for PHY Sim --KT
 *
 * Revision 1.6  2008/12/23 14:24:01  gur19413
 * Review Comments Incorporated
 *
 * Revision 1.5  2008/12/23 13:42:27  gur11912
 * Review Comments incorportated
 *
 * Revision 1.4  2008/12/12 05:12:42  gur19413
 * Updated for Phase 2
 *
 * Revision 1.3  2008/10/22 12:17:48  gur11912
 * Throughput calculation added
 *
 * Revision 1.2  2008/09/26 06:12:52  gur11912
 * Support for Control Information Provided
 *
 * Revision 1.1  2008/09/19 11:14:28  gur11912
 * INITIAL VERSION
 *
 *
 *
 ****************************************************************************/

#ifndef PHYSIM_H
#define PHYSIM_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include "lteLayer2Types.h"
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include <sys/mman.h>
#include "phySimulatorApiHandler.h"



#define SUCCESS 1
#define RRC_API_HEADER_LEN 10
#define RRC_MODULE_ID      3 
#define PHY_MODULE_ID      8
#define PHY_CONFIG_CELL_REQ 1
#define PHY_CONFIG_CELL_CNF 2
#define PHY_CREATE_UE_ENTITY_REQ 5 
#define PHY_CREATE_UE_ENTITY_CNF 6
#define PHY_DELETE_UE_ENTITY_REQ 7
#define PHY_DELETE_UE_ENTITY_CNF 8
#define PHY_RECONFIG_UE_ENTITY_REQ 9
#define PHY_RECONFIG_UE_ENTITY_CNF 10
#define PHY_CONFIG_CELL_CNF_BUFFER_LEN RRC_API_HEADER_LEN + 2
#define PHY_CREATE_UE_ENTITY_CNF_BUFFER_LEN RRC_API_HEADER_LEN + 3
#define PHY_RECONFIG_UE_ENTITY_CNF_BUFFER_LEN RRC_API_HEADER_LEN + 5
#define PHY_DELETE_UE_ENTITY_CNF_BUFFER_LEN RRC_API_HEADER_LEN + 3 

#if 0 
#define THREAD_TO_BIND_SET_CORE_NO(threadId,corenum) {\
  cpu_set_t cpuSetSch; \
  Int coreNum = 0; \
  CPU_ZERO(&cpuSetSch);\
  coreNum = corenum ; \
  CPU_SET(coreNum ,&cpuSetSch); \
  if (pthread_setaffinity_np(threadId,sizeof(cpuSetSch),&cpuSetSch)) \
  { \
    ltePanic("Set affinity failed for thread id %d\n, threadId"); \
  } \
}

#define THREAD_TO_BIND_GET_CORE_NO(threadId,corenum) {\
  cpu_set_t cpuSetSch; \
  Int coreNum = 0; \
  CPU_ZERO(&cpuSetSch);\
  coreNum = corenum ; \
  CPU_SET(coreNum ,&cpuSetSch); \
  if (pthread_getaffinity_np(threadId,sizeof(cpuSetSch),&cpuSetSch)) \
  { \
    ltePanic("Get affinity failed for thread id %d\n, threadId"); \
  } \
}

#ifdef THREAD_BIND_ENABLE
#define THREAD_TO_BIND_SET_CORE_NO(threadId,corenum) {\
  unsigned long coreNum = 0; \
  coreNum = corenum ; \
  if (sched_setaffinity(threadId,sizeof(coreNum),&coreNum)) \
  { \
    ltePanic("Set affinity failed for thread id %d\n, threadId"); \
  } \
}
#define THREAD_TO_BIND_GET_CORE_NO(threadId,corenum) {\
  unsigned long coreNum = 0; \
  coreNum = corenum ; \
  if (sched_getaffinity(threadId,sizeof(coreNum),&coreNum)) \
  { \
    ltePanic("Get affinity failed for thread id %d\n, threadId"); \
  } \
}
#endif
#endif
/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
//void writeDataToPhySHM(UInt8* msg_p,UInt32 len,UInt32 type);

void getNackPos(UInt16  totalPackets, UInt8  nackSend, UInt8 *arr_p);
SInt32 sendMsgbySocket(UInt8 *buffer_p,UInt32 bufferLen);
/* SPR 15909 fix start */
tickType_t getCurrentglobaltick();
/* SPR 15909 fix end */
#endif/* PHYSIM_H*/
