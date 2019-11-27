/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcThread.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:40 gur20491 Exp $
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
 * $Log: lteRlcThread.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:40  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:14:04  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.8  2010/06/14 11:57:59  gur21148
 * RLC_UM_MERGE
 *
 * Revision 1.7  2009/06/04 05:20:31  gur19836
 * Optimization Changes
 *
 * Revision 1.6  2009/04/15 16:04:22  gur19140
 * rlc latency + logging related changes
 *
 * Revision 1.5  2009/04/02 14:27:51  gur19140
 * changes for intg and peer to peer testing
 *
 * Revision 1.4  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.3  2009/02/09 11:30:46  gur19140
 * Changes to make Tx Rx thread wait at Sema
 *
 * Revision 1.2  2009/01/29 05:35:22  gur20294
 * after code review comments incorporated
 *
 * Revision 1.1  2008/12/30 06:24:49  gur20294
 * RLC Layer added. INITIAL DRAFT
 *
 *
 *
 ****************************************************************************/

#ifndef RLCTHREAD_H
#define RLCTHREAD_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
/* intel specific */
#include "lteRlcTypes.h"


/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef  void * (*ThreadStartMethod)(void *);

typedef struct RlcThreadT {
    ULong32 tid ;
    SInt32 detachState ;
    SInt32 schedPol ;
    SInt32 priority ;
    SInt32 inheritSched ;
    SInt32 scope ;
    ThreadStartMethod start ;
    void *arg ;
    void *status ;
} RlcThread ;

#define RLC_THREAD_INIT {-1,THREAD_CREATE_DETACHED,0,THREAD_SCHED_DEFAULT,THREAD_INHERIT_SCHED_DEFAULT,THREAD_SCOPE_DEFAULT,0,0,0}


/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 * Exported variable
 ****************************************************************************/
extern LTE_SEM rlcRxSemlock_g ;
extern LTE_SEM rlcCurrentTickSemlock_g ;

#endif  /* INCLUDED_FILENAME_H */
