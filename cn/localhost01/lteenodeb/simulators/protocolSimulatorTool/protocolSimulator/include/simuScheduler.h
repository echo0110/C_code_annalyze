/**************************************************************************************
*
*  FILE NAME  : simuScheduler.h
*
*  DESCRIPTION: External scheduler interface definition.
*
*
*   DATE    NAME    REFERENCE   REASON
*
*  Copyright 2009, Aricent Inc. Ukraine
*
*************************************************************************************/

#ifndef _SIMU_SCHEDULER_DEF_
#define _SIMU_SCHEDULER_DEF_


/****************************************************************************
 * Includes
 ****************************************************************************/

#include "simTypes.h"

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 * Function Name  : esStart
 * Inputs         : .
 * Outputs        : None.
 * Returns        : None.
 * Description    : Runs External scheduler.
 ****************************************************************************/
void    esStart();

/****************************************************************************
 * Function Name  : esStop
 * Inputs         : .
 * Outputs        : None.
 * Returns        : None.
 * Description    : Stops External scheduler.
 ****************************************************************************/
void    esStop();

/****************************************************************************
 * Function Name  : esLock
 * Inputs         : .
 * Outputs        : None.
 * Returns        : None.
 * Description    : Lock External scheduler.
 ****************************************************************************/
void    esLock();

/****************************************************************************
 * Function Name  : esUnlock
 * Inputs         : .
 * Outputs        : None.
 * Returns        : None.
 * Description    : Unlock External scheduler.
 ****************************************************************************/
void    esUnlock();

#endif /* _SIMU_SCHEDULER_DEF_ */
