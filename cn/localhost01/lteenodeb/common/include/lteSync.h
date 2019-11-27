/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteSync.h,v 1.2 2010/03/02 12:12:41 gur15697 Exp $
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
 * $Log: lteSync.h,v $
 * Revision 1.2  2010/03/02 12:12:41  gur15697
 * optimization changes
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.5  2008/09/18 05:25:46  gur19836
 * Review comments incorporated
 *
 * Revision 1.4  2008/09/09 06:47:46  ssinghal
 * Added comments and removed tabs from the file
 *
 * Revision 1.3  2008/09/05 08:55:42  gur19836
 * Semaphore typedef added
 *
 * Revision 1.2  2008/08/25 10:04:35  ssinghal
 * Template applied and minor changes done
 *
 *
 *
 ****************************************************************************/


#ifndef _LTE_SYNC_
#define _LTE_SYNC_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteTypes.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>


/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef pthread_mutex_t           LTE_SEM;
typedef sem_t                     SEM_T;
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
 *      Initialises the mutex object.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
/* SPR 10689 changes start */
SInt32    mutexInit(UInt32 *mutex);
/* SPR 10689 changes end */


/****************************************************************************
 *  function(): Description.
 *      Locks the mutex object.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
/* SPR 10689 changes start */
SInt32    mutexLock(UInt32 *mutex);
/* SPR 10689 changes end */


/****************************************************************************
 *  function(): Description.
 *      Unlocks the mutex object.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
/* SPR 10689 changes start */
SInt32    mutexUnlock(UInt32 *mutex);
/* SPR 10689 changes end */



/****************************************************************************
 *  function(): Description.
 *      Condiitonal signal for the conditional object.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
/* SPR 10689 changes start */
SInt32    condSignal(UInt32 *cond);
/* SPR 10689 changes end */


/****************************************************************************
 *  function(): Description.
 *      Condiitonal wait for the conditional object.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
/* SPR 10689 changes start */
SInt32    condWait(UInt32 *cond, UInt32 *mutex);
/* SPR 10689 changes end */


/****************************************************************************
 *  function(): Description.
 *      Initialises the semaphore object.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
SInt32 semInit(LTE_SEM *pSem, UInt32 value);


/****************************************************************************
 *  function(): Description.
 *      Wait for the semaphore.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
SInt32 semWait(LTE_SEM *pSem);


/****************************************************************************
 *  function(): Description.
 *      Non blocking wait for the semaphore.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
SInt32 semTryWait(LTE_SEM *pSem);


/****************************************************************************
 *  function(): Description.
 *      Posts the semaphore.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
SInt32 semPost(LTE_SEM *pSem);


/****************************************************************************
 *  function(): Description.
 *      Returns the value of the semaphore.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
SInt32 semGetVal(SEM_T *pSem, SInt32 *pValue);


/****************************************************************************
 *  function(): Description.
 *      Destroys the semaphore.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/
SInt32 semDestroy(LTE_SEM *pSem);
/****************************************************************************
 *  function(): Description.
 *      Destroys the semaphore.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/


SInt32 semInitDefault(SEM_T *pSem, SInt32 pValue);
/****************************************************************************
 *  function(): Description.
 *      Destroys the semaphore.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/

SInt32 semPostDefault(SEM_T *pSem);
/****************************************************************************
 *  function(): Description.
 *      Destroys the semaphore.
 *
 *  Parameters:
 *
 *  Return values:
 *
 ****************************************************************************/

SInt32 semWaitDefault(SEM_T *pSem);

#endif  
