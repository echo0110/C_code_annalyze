/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteThread.h,v 1.1.1.1.16.1 2010/09/29 06:27:59 gur04640 Exp $
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
 * $Log: lteThread.h,v $
 * Revision 1.1.1.1.16.1  2010/09/29 06:27:59  gur04640
 * Function added for setting realtime priority
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.9  2009/04/10 17:22:38  gur18569
 * merged with optmization changes
 *
 * Revision 1.8  2009/01/20 16:29:29  gur11912
 * Updated for Porting
 *
 * Revision 1.7  2008/12/19 10:49:00  gur18569
 * added wappers
 *
 * Revision 1.6  2008/10/13 12:39:01  gur19836
 * Enhancements Added
 *
 * Revision 1.5  2008/09/09 06:47:46  ssinghal
 * Added comments and removed tabs from the file
 *
 * Revision 1.4  2008/09/05 04:26:59  gur19836
 * Change signature for thread create
 *
 * Revision 1.3  2008/09/01 07:44:43  ssinghal
 * thread detach and thread self support added
 *
 * Revision 1.2  2008/08/25 10:04:35  ssinghal
 * Template applied and minor changes done
 *
 *
 *
 ****************************************************************************/

#ifndef _LTE_THREAD_
#define _LTE_THREAD_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteTypes.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#include <pthread.h>
#include <errno.h>


/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/****************************************************************************
 * Function Name  : threadCleanupPush 
 * Inputs         : routine - address of function
 *                  arg - argument to routine.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : threadCleanupPush installs the routine function with
 *                  argument arg as a cleanup handler. From this point on
 *                  to the matching pthread_cleanup_pop,the function routine
 *                  will be called with arguments arg when the thread
 *                  terminates .
 *                 
 ****************************************************************************/
#define threadCleanupPush(routine, arg)\
                pthread_cleanup_push(routine,arg)


/****************************************************************************
 * Function Name  : threadCleanupPop 
 * Inputs         : execute- Non Zero: it also executes the handler
 *                           0       : the handler is only removed but 
 *                                     not executed.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : threadCleanupPop removes the most recently installed 
 *                  cleanup handler.
 ****************************************************************************/
#define threadCleanupPop(arg)\
                pthread_cleanup_pop(arg)




/* Macro for Thread Binding */
#define THREAD_TO_BIND_SET_CORE_NO(threadId,corenum) {\
    cpu_set_t cpuSetSch; \
    int temp_coreNum = 0; \
    temp_coreNum = corenum; \
    CPU_ZERO(&cpuSetSch);\
    CPU_SET(temp_coreNum ,&cpuSetSch); \
    if (pthread_setaffinity_np(threadId,sizeof(cpuSetSch),&cpuSetSch)) \
    { \
        ltePanic("Set affinity failed"); \
    } \
    else \
    { \
        LTE_LOG(LOG_INFO,PNULL, "\n Thread %x  Succefully Binded on Port no %d\n", (UInt32)threadId, temp_coreNum); \
    } \
}

#define THREAD_TO_BIND_GET_CORE_NO(threadId) {\
    cpu_set_t cpuSetSch; \
    int j = 0; \
    CPU_ZERO(&cpuSetSch);\
    if (pthread_getaffinity_np(threadId,sizeof(cpuSetSch),&cpuSetSch)) \
    { \
        ltePanic("Get affinity failed"); \
    } \
    else \
    {\
/* SPR 609 changes start */\
       LTE_LOG(LOG_INFO,PNULL, "Set returned by pthread_getaffinity_np() for Thread %lu contained:\n", threadId);\
/* SPR 609 changes end */\
       for (j = 0; j < CPU_SETSIZE; j++)\
        {\
            if (CPU_ISSET(j, &cpuSetSch))\
            {\
/* SPR 609 changes start */\
                LTE_LOG(LOG_INFO,PNULL, "Thread %lu Binded on VCPU %d\n",threadId, j);\
/* SPR 609 changes end */\
            }\
        }\
    } \
}

#define PROCESS_TO_BIND_SET_CORE_NO(pid,corenum) {\
    cpu_set_t cpuSetSch; \
    int coreNum = 0; \
    CPU_ZERO(&cpuSetSch);\
    coreNum = corenum ; \
    CPU_SET(coreNum ,&cpuSetSch); \
    if (-1 == sched_setaffinity_wrapper(pid,sizeof(cpuSetSch),&cpuSetSch)) \
    { \
        ltePanic("Set affinity failed"); \
    } \
}
#define PROCESS_TO_BIND_GET_CORE_NO(pid,corenum) {\
    cpu_set_t cpuSetSch; \
    int coreNum = 0; \
    CPU_ZERO(&cpuSetSch);\
    coreNum = corenum ; \
    CPU_SET(coreNum ,&cpuSetSch); \
    if (-1 == sched_getaffinity(pid,sizeof(cpuSetSch),&cpuSetSch)) \
    { \
        ltePanic("Get affinity failed"); \
    } \
}

/****************************************************************************
 * Exported Types
 ****************************************************************************/
enum
{
    /* thread is created in the joinable state */
    THREAD_CREATE_JOINABLE = PTHREAD_CREATE_JOINABLE, 
    /* thread is created in the detached state */
    THREAD_CREATE_DETACHED = PTHREAD_CREATE_DETACHED, 
    /* default state */
    THREAD_DEFAULT_STATE = THREAD_CREATE_JOINABLE 
};


enum
{
    /* regular,  non-realtime  scheduling */
    THREAD_SCHED_OTHER = SCHED_OTHER, 
    /* realtime,  round robin*/
    THREAD_SCHED_RR = SCHED_RR, 
    /* realtime, first-in first-out*/
    THREAD_SCHED_FIFO = SCHED_FIFO, 
    /* default scheduling*/
    THREAD_SCHED_DEFAULT = THREAD_SCHED_OTHER 
};


enum
{
    /* scheduling parameters for the newly created thread are determined by schedpolicy 
     and priority*/
    THREAD_EXPLICIT_SCHED = PTHREAD_EXPLICIT_SCHED, 
    /* Inherited  from  the  parent  thread*/
    THREAD_INHERIT_SCHED = PTHREAD_INHERIT_SCHED, 
    /* default inheritance of scheduling parameters*/
    THREAD_INHERIT_SCHED_DEFAULT = PTHREAD_EXPLICIT_SCHED 
};


enum
{
    /* threads contend for CPU time with all processes running on the machine*/
    THREAD_SCOPE_SYSTEM = PTHREAD_SCOPE_SYSTEM, 
    /* threads contend for CPU time only between the threads of the running process */
    THREAD_SCOPE_PROCESS = PTHREAD_SCOPE_PROCESS, 
    /* default thread scope */
    THREAD_SCOPE_DEFAULT = PTHREAD_SCOPE_SYSTEM
};


enum
{
    /*cancel state - enable /disable*/
    THREAD_CANCEL_ENABLE = PTHREAD_CANCEL_ENABLE,
    THREAD_CANCEL_DISABLE = PTHREAD_CANCEL_DISABLE
};
enum
{
    /* cancel type deferred /asynchronous*/
    THREAD_CANCEL_DEFERRED = PTHREAD_CANCEL_DEFERRED,
    THREAD_CANCEL_ASYNCHRONOUS = PTHREAD_CANCEL_ASYNCHRONOUS
};

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/


/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 *  function(): Description.
 *      Creates a new thread.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
SInt32  threadCreate(ULong32 *tId, SInt32 detachState, 
        SInt32    schedPol, SInt32 priority, SInt32 inheritSched, SInt32 scope,
        void * (*func)(void *), void *arg);

/****************************************************************************
 *  function(): Description.
 *      Waits for other thread to finish.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/

void    threadJoin(ULong32 tId, void **status);


/****************************************************************************
 *  function(): Description.
 *      Thread Exit API.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
void    threadExit(void *retVal);


/****************************************************************************
 *  function(): Description.
 *      Detaches the thread from the parent thread.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
void    threadDetach(ULong32 tId);


/****************************************************************************
 *  function(): Description.
 *      Returns the identifier for the current thread.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
ULong32    threadSelf(void);


/****************************************************************************
 *  function(): Description.
 *      Cancels the execution of thread with tId as thread identifier.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
SInt32    threadCancel(ULong32 tId);

/****************************************************************************
 * Function Name  : pthread_setcancelstate
 * Inputs         : state - enable /disabled.
 * Outputs        : None.
 * Returns        : 0 on success, Error code on failure.
 * Variables      : None.
 * Description    : The function  changes  the  cancellation  state  for the
 *                  calling thread -- that is, whether cancellation requests 
 *                  are ignored or not.
 ****************************************************************************/
SInt32 threadSetCancelState(UInt32 state, Int* oldState);


/****************************************************************************
 * Function Name  : threadSetCancelType 
 * Inputs         : state - deferred/asynchronous.
 * Outputs        : None.
 * Returns        : 0 on success, Error code on failure.
 * Variables      : None.
 * Description    : threadSetCancelType changes the type of responses 
 *                  to cancellation requests for the calling thread: 
 *                  asynchronous  (immediate)  or  deferred.
 *                 
 ****************************************************************************/
SInt32 threadSetCancelType(UInt32 state, Int* oldState);

/****************************************************************************
 * Function Name  : threadSetRtPriority
 * Inputs         : policy - policy to set
 *                  priority - priority to set.
 * Outputs        : None.
 * Returns        : 0 on success, Error code on failure.
 * Variables      : None.
 * Description    : This function sets the realtime priority and policy for the
 *                  calling thread.
 *                 
 ****************************************************************************/
SInt32 threadSetRtPriority(int policy, int priority);

SInt32 threadKill(ULong32 thread, SInt32 sig);
SInt32 threadEqual(ULong32 tId1, ULong32 tId2);
#endif  
