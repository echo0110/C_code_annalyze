/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteSync.c,v 1.3.6.2.8.1 2010/09/21 15:45:40 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : Wrapper implementation for synchronization objects. 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteSync.c,v $
 * Revision 1.3.6.2.8.1  2010/09/21 15:45:40  gur20491
 * FAPI changes
 *
 * Revision 1.3.6.2  2010/08/02 08:57:13  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.3  2010/03/02 12:13:50  gur15697
 * optimization changes
 *
 * Revision 1.2  2010/02/13 10:29:28  gur15697
 * loking disabled at enodeb side
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.5  2008/09/18 05:25:08  gur19836
 * Review comments incorporated
 *
 * Revision 1.4  2008/09/09 11:11:53  ssinghal
 * comments added to functions
 *
 * Revision 1.3  2008/09/05 08:55:42  gur19836
 * Semaphore typedef added
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
#include "lteSync.h"
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
#ifdef MAC_AUT_TEST
extern UInt8  macAutFlag ;
#endif

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

/****************************************************************************
 * Function Name  : mutexInit 
 * Inputs         : mutex - Pointer to mutex identifier.
 * Outputs        : None. 
 * Returns        : None.
 * Variables      : None.
 * Description    : Initialises the mutex object.
 ****************************************************************************/
/* SPR 10689 changes start */
SInt32    mutexInit(UInt32 *mutex)
{
    SInt32             n =0;

    if ( (n = pthread_mutex_init((pthread_mutex_t *)mutex, PNULL)) != 0)
    {
        errno = n;
        lteWarning("mutexInit error");
    }
    return n;
}
/* SPR 10689 changes end */

/****************************************************************************
 * Function Name  : mutexLock 
 * Inputs         : mutex - Pointer to mutex identifier.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Locks the mutex.
 ****************************************************************************/
/* SPR 10689 changes start */
SInt32    mutexLock(UInt32 *mutex)
{
    SInt32             n=0;

    if ( (n = pthread_mutex_lock((pthread_mutex_t *)mutex)) != 0)
    {
        errno = n;
        lteWarning("mutexLock error");
    }
    return n;
}                                                
/* SPR 10689 changes end */

/****************************************************************************
 * Function Name  : mutexUnlock 
 * Inputs         : mutex - Pointer to mutex identifier.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Unlocks the mutex.
 ****************************************************************************/
/* SPR 10689 changes start */
SInt32    mutexUnlock(UInt32 *mutex)
{
    SInt32             n=0;

    if ( (n = pthread_mutex_unlock((pthread_mutex_t *)mutex)) != 0)
    {
        errno = n;
        lteWarning("pthread_mutex_unlock error");
    }
    return n;
}
/* SPR 10689 changes end */

/****************************************************************************
 * Function Name  : condSignal
 * Inputs         : cond - Pointer to the conditional variable.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : This function signals the conditional variable. 
 ****************************************************************************/
/* SPR 10689 changes start */
SInt32    condSignal(UInt32 *cond)
{
    SInt32             n=0;

    if ( (n = pthread_cond_signal((pthread_cond_t *)cond)) != 0)
    {
        errno = n;
        lteWarning("condSignal error");
    }
    return n;
}                                                
/* SPR 10689 changes end */

/****************************************************************************
 * Function Name  : condWait 
 * Inputs         : cond - Pointer to the conditional variable.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : This function waits for the conditional variable.
 ****************************************************************************/
/* SPR 10689 changes start */
SInt32    condWait(UInt32 *cond, UInt32 *mutex)
{
    SInt32             n=0;

    if ( (n = pthread_cond_wait((pthread_cond_t *)cond, 
                    (pthread_mutex_t *)mutex)) != 0)
    {
        errno = n;
        lteWarning("condWait error");
    }
    return n;
}    
/* SPR 10689 changes end */

/****************************************************************************
 * Function Name  : semInit 
 * Inputs         : pSem - Pointer to the semaphore variable.
 *                  value - value with which  to initialise.
 * Outputs        : None.
 * Returns        : Success or failure.
 * Variables      : None.
 * Description    : This fucntion initialises the semaphore.
 ****************************************************************************/
SInt32 semInit(LTE_SEM *pSem, UInt32 value)
{
    //return sem_init(pSem, 0, value);
	if ( pthread_mutex_init((pthread_mutex_t *)pSem, PNULL) == 0)
    {
        if ( value == 0 )
        {
        /* +COVERITY 24353 */
            if ( (pthread_mutex_lock((pthread_mutex_t *)pSem)) == 0)
        /* +COVERITY 24353 */
            return 0;
        }
        else if (value != 1 )
            ltePanic("%s() with more than 1 is not supported\n",__FUNCTION__);

        return 0;
    }
    else
        return 1;
}

/****************************************************************************
 * Function Name  : semWait 
 * Inputs         : pSem - Pointer to the semaphore variable.
 * Outputs        : None.
 * Returns        : None.
 * Variables      : None.
 * Description    : Waits for the semaphore.
 ****************************************************************************/
SInt32 semWait(LTE_SEM *pSem)
{
#ifdef MAC_AUT_TEST 
    if (macAutFlag)
    {
        return 1;
    }
#endif
    /* SPR 10689 changes start */
    SInt32             errCode=0;
    if ( (errCode = (pthread_mutex_lock((pthread_mutex_t *)pSem))) == 0)
        return 0;
    else
        lteWarning("semWait error with error number[%d]",errCode);
    return errCode;
    /* SPR 10689 changes end */
}

/****************************************************************************
 * Function Name  : semTryWait 
 * Inputs         : pSem - Pointer to the semaphore variable.
 * Outputs        : None.
 * Returns        : Success or failure.
 * Variables      : None.
 * Description    : This function tries for semaphore and is a non blocking 
 *                  call.
 ****************************************************************************/
SInt32 semTryWait(LTE_SEM *pSem)
{
    /* SPR 10689 changes start */
    SInt32             errCode=0;
    if ((errCode = (pthread_mutex_trylock((pthread_mutex_t *)pSem))) == 0)
        return 0;
    else
        lteWarning("semTryWait error with error number[%d]",errCode);
    return errCode;
    /* SPR 10689 changes end */
}

/****************************************************************************
 * Function Name  : semPost 
 * Inputs         : pSem - Pointer to the semaphore variable.
 * Outputs        : None.
 * Returns        : Success or failure.
 * Variables      : None.
 * Description    : This function releses the semaphore.
 ****************************************************************************/
SInt32 semPost(LTE_SEM *pSem)
{
    if ( (pthread_mutex_unlock((pthread_mutex_t *)pSem)) == 0)
        return 0;
    else
        return 1;
}

/****************************************************************************
 * Function Name  : semGetVal 
 * Inputs         : pSem - Pointer to the semaphore variable.
 *                  pValue - Pointer to value object that needs to be filled.
 * Outputs        : pValue has the current value of the semaphore.
 * Returns        : Success or failure.
 * Variables      : None.
 * Description    : This function fills the value fo the semaphore variable.
 ****************************************************************************/
SInt32 semGetVal(SEM_T *pSem, SInt32 *pValue)
{
#ifdef MAC_AUT_TEST 
    if (macAutFlag)
    {
        return sem_getvalue((SEM_T *)pSem, pValue);
    }
    /* +- SPR 17777 */
#else
    LTE_GCC_UNUSED_PARAM(pSem)
        LTE_GCC_UNUSED_PARAM(pValue)
    /* +- SPR 17777 */
#endif
#ifndef UE_SIM_TESTING
        return 0;
#else
    return 0;
#endif
}


/****************************************************************************
 * Function Name  : semDestroy 
 * Inputs         : pSem - Pointer to the semaphore variable.
 * Outputs        : None.
 * Returns        : Success or failure.
 * Variables      : None.
 * Description    : Thsi function destorys the semaphore object.
 ****************************************************************************/
SInt32 semDestroy(LTE_SEM *pSem)
{
    if ( (pthread_mutex_destroy((pthread_mutex_t *)pSem)) == 0)
        return 0;
    else
        return 1;
}

/****************************************************************************
 * Function Name  : semInitDefault 
 * Inputs         : pSem - Pointer to the semaphore variable.
 * Outputs        : None.
 * Returns        : Success or failure.
 * Variables      : None.
 * Description    : Thsi function destorys the semaphore object.
 ****************************************************************************/
SInt32 semInitDefault(SEM_T *pSem, SInt32 pValue)
{
    return sem_init(pSem, 0, pValue);
}

/****************************************************************************
 * Function Name  : semPostDefault 
 * Inputs         : pSem - Pointer to the semaphore variable.
 * Outputs        : None.
 * Returns        : Success or failure.
 * Variables      : None.
 * Description    : Thsi function destorys the semaphore object.
 ****************************************************************************/
SInt32 semPostDefault(SEM_T *pSem)
{
#ifdef MAC_AUT_TEST
    if (macAutFlag)
    {
        return 1;
    }
#endif
    return sem_post(pSem);
}

/****************************************************************************
 * Function Name  : semWaitDefault 
 * Inputs         : pSem - Pointer to the semaphore variable.
 * Outputs        : None.
 * Returns        : Success or failure.
 * Variables      : None.
 * Description    : Thsi function destorys the semaphore object.
 ****************************************************************************/
SInt32 semWaitDefault(SEM_T *pSem)
{
#ifdef MAC_AUT_TEST
    if (macAutFlag)
    {
        return 1;
    }
#endif
    return sem_wait(pSem);
}

