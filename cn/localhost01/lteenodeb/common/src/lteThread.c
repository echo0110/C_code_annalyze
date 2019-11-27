/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteThread.c,v 1.1.1.1.16.1 2010/09/29 06:27:51 gur04640 Exp $
 *
 ****************************************************************************
 *
 *  File Description : Wrapper implemention of thread related APIs. 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteThread.c,v $
 * Revision 1.1.1.1.16.1  2010/09/29 06:27:51  gur04640
 * Function added for setting realtime priority
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.9  2009/04/10 17:22:38  gur18569
 * merged with optmization changes
 *
 * Revision 1.8  2009/01/20 16:28:58  gur11912
 * Updated for Porting
 *
 * Revision 1.7  2008/12/19 10:48:43  gur18569
 * added wrappers
 *
 * Revision 1.6  2008/10/13 12:39:01  gur19836
 * Enhancements Added
 *
 * Revision 1.5  2008/09/09 11:11:53  ssinghal
 * comments added to functions
 *
 * Revision 1.4  2008/09/05 04:26:19  gur19836
 * Change signature for thread create
 *
 * Revision 1.3  2008/09/01 07:44:42  ssinghal
 * thread detach and thread self support added
 *
 * Revision 1.2  2008/08/25 10:05:03  ssinghal
 * Template applied and minor changes done
 *
 *
 ****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/


/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <sched.h>
#include <sys/syscall.h>
#include <signal.h>
#include "lteThread.h"
#include "lteLog.h"

/****************************************************************************
  Private Definitions
 ****************************************************************************/



/****************************************************************************
  Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

/****************************************************************************
 * Function Name  : threadCreate 
 * Inputs         : tId - Pointer to the thread identifier.
 *                  detachState - Whether the thread is detached or not.
 *                  schedPol - Scheduling policy to be applied.
 *                  priority - Priority of the thread.
 *                  inheritSched - Scheduling policy inherited from parent 
 *                  thread.
 *                  scope - scope of the thread.
 *                  func - entry level function for the thread.
 *                  arg - arguments required by the function if any.
 * Outputs        : None. 
 * Returns        : Success or failure.
 * Variables      : None.
 * Description    : This function creates a thread.
 ****************************************************************************/
SInt32  threadCreate(ULong32 *tId, SInt32 detachState, 
        SInt32    schedPol, SInt32 priority, SInt32 inheritSched, 
        SInt32 scope, void * (*func)(void *), void *arg)
{
    SInt32 rc=0;

    /* first create a thread attribute init object. */
    pthread_attr_t thread_attr;

    rc = pthread_attr_init(&thread_attr);

    if ( 0 == rc && detachState != THREAD_DEFAULT_STATE )
    {
        rc = pthread_attr_setdetachstate(&thread_attr, detachState);
    }

    if ( 0 == rc && schedPol != THREAD_SCHED_DEFAULT )
    {
        rc = pthread_attr_setschedpolicy(&thread_attr, schedPol);
    }

    if ( 0 == rc && priority != 0 )
    {
        struct sched_param schedParam;
        schedParam.sched_priority = priority;
        rc = pthread_attr_setschedparam(&thread_attr, &schedParam);
    }

    if ( 0 == rc && inheritSched != THREAD_INHERIT_SCHED_DEFAULT )
    {
        rc = pthread_attr_setinheritsched(&thread_attr, inheritSched);
    }

    if ( 0 == rc && scope != THREAD_SCOPE_DEFAULT )
    {
        rc = pthread_attr_setscope(&thread_attr, scope);
    }

    if( 0 != rc )
    {
        return rc;
    }
    else
    {        
        return (pthread_create(tId, &thread_attr, func, arg));
    }
}

/****************************************************************************
 * Function Name  : threadJoin 
 * Inputs         : tId - Pointer to the thread identifier.
 *                  status - returns the status of the thread.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : This function waits for other threads to complete.
 ****************************************************************************/
void    threadJoin(ULong32 tId, void **status)
{
    SInt32             n;

    if ( (n = pthread_join(tId, status)) == 0)
        return;

    errno = n;
    lteWarning("threadJoin error");
}


/****************************************************************************
 * Function Name  : threadExit 
 * Inputs         : retVal - Return value of the thread. 
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Terminates the execution of the calling thread.
 ****************************************************************************/
void    threadExit(void *retVal)
{
    pthread_exit(retVal);
}



/****************************************************************************
 * Function Name  : threadDetach 
 * Inputs         : tId - Thread identifier.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : This function puts the thread in detached state. 
 ****************************************************************************/
void    threadDetach(ULong32 tId)
{
    SInt32             n;

    if ( (n = pthread_detach(tId)) == 0)
        return;
    errno = n;
    lteWarning("threadDetach error");
}


/****************************************************************************
 * Function Name  : threadSelf 
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : The thread identifier of the calling thread.
 * Variables      : None.
 * Description    : This function returns the thread identifier of the
 *                  calling thread.
 ****************************************************************************/
ULong32    threadSelf(void)
{
    return pthread_self();
}


/****************************************************************************
 * Function Name  : threadCancel 
 * Inputs         : Thread Identifier.
 * Outputs        : None.
 * Returns        : 0 on success, Error code on failure.
 * Variables      : None.
 * Description    : This function cancels the execution of thread whose thread
 *                  identifier has been passed as an arguement.
 ****************************************************************************/
SInt32 threadCancel(ULong32 tId)
{
    return pthread_cancel(tId);
}
    
    
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
SInt32 threadSetCancelState(UInt32 state, Int* oldState)
{
    return pthread_setcancelstate(state, oldState);
}


/****************************************************************************
 * Function Name  : threadSetCancelType 
 * Inputs         : state - deferred /asynchronous.
 * Outputs        : None.
 * Returns        : 0 on success, Error code on failure.
 * Variables      : None.
 * Description    : threadSetCancelType changes the type of responses 
 *                  to cancellation requests for the calling thread: 
 *                  asynchronous  (immediate)  or  deferred.
 *                 
 ****************************************************************************/
SInt32 threadSetCancelType(UInt32 state, Int* oldState)
{
    return pthread_setcanceltype(state, oldState);
}

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
 SInt32 threadSetRtPriority(int policy, int priority)
 {
     struct sched_param sp = {0};;
     sp.sched_priority = priority;
     return sched_setscheduler(syscall(SYS_gettid), policy, &sp);
 }


SInt32 threadKill(ULong32 thread, SInt32 sig)
{
 return  pthread_kill(thread,sig);
}


SInt32 threadEqual(ULong32 tId1, ULong32 tId2)
{
 return  pthread_equal(tId1, tId2);
}
