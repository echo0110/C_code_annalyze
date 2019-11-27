/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohcTimer.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the function declarations ROHC Timers
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohcTimer.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.4.2.1  2009/06/09 11:34:55  gur19140
 * first wave of Integration
 *
 * Revision 1.4  2009/05/28 04:41:35  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef __ROHCTIMER_H__
#define __ROHCTIMER_H__

/****************************************************************************
 * Includes
 ****************************************************************************/
#include "lteTypes.h"
#include "lteList.h"
#include "lteTime.h"

/****************************************************************************
 * Exported Types
 ****************************************************************************/
/* SPR 15909 fix start */
extern tickType_t pdcpCurrentTime_g ;
/* SPR 15909 fix end */

/****************************************************************************
 * Functions declarations 
 ****************************************************************************/

/****************************************************************************
 * Function Name  : initRohcTimer  
 * Inputs         : rohcTimerCallback_func func
 * Outputs        : None.
 * Returns        : None.
 * Description    : Inits all timer queues and callback
  ****************************************************************************/
/* + SPR 17439 */
void initRohcTimer(void);
/* - SPR 17439 */
/****************************************************************************
 * Function Name  : cleanRohcTimer  
 * Inputs         : None
 * Outputs        : None.
 * Returns        : None.
 * Description    : Cleans all timer queues
  ****************************************************************************/
void cleanRohcTimer(void);

/****************************************************************************
 * Function Name  : startRohcTimer  
 * Inputs         : UInt16 timeout - timeout for message
 *                  node_p : pointer to node
 * Outputs        : LTE_LIST_NODE *
 * Returns        : LTE_LIST_NODE*
 * Description    : Push message to timer queue
  ****************************************************************************/
LTE_LIST_NODE* startRohcTimer(UInt16 timeout, UInt16 timerType, void* node_p);

/****************************************************************************
 * Function Name  : stopRohcTimer
 * Inputs         : LTE_LIST_NODE* node_p - pointer to node
 *                : timertype
 * Outputs        : void
 * Returns        : void
 * Description    : Pop message to timer queue
  ****************************************************************************/
void stopRohcTimer(UInt16 timerType, void* node_p);

/****************************************************************************
 * Function Name  : rohcProcessTimers  
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : Processing queues with timers
  ****************************************************************************/
void rohcProcessTimers(void);

/****************************************************************************
 * Function Name  : removeRohcTimer  
 * Inputs         : LTE_LIST_NODE* node_p - node to remove
 * Outputs        : None
 * Returns        : None
 * Description    : Extern interface, removing rohc timer
  ****************************************************************************/
void removeRohcTimer(LTE_LIST_NODE* node_p);

#endif /* __ROHCTIMER_H__ */
