/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2Tick.h,v 1.1.1.1 2010/02/11 04:51:22 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description :  This file contains declarations for simulating the 
 *                      1 ms tick from the Physical Layer.
 *                      This file is to be included for Testing Purposes.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteLayer2Tick.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2  2009/07/14 14:45:02  gur20439
 * rrc interface code.
 *
 * Revision 1.1  2009/03/30 10:20:28  gur19836
 * Initial Version
 *
 * Revision 1.9  2009/03/23 08:32:30  gur19140
 * Integration changes first waves
 *
 * Revision 1.8  2009/01/29 18:52:54  gur03939
 * Modified nanolseep to pselect
 *
 * Revision 1.7  2009/01/29 17:48:26  gur03939
 * Changes for socket interface with PHY Simulator
 *
 * Revision 1.6  2009/01/29 11:10:37  gur19413
 * updated
 *
 * Revision 1.5  2009/01/20 07:54:00  gur19836
 * Changes done for interface with RRC Layer
 *
 * Revision 1.4  2009/01/20 07:21:29  gur19413
 * updated
 *
 * Revision 1.3  2008/12/05 13:24:04  gur18569
 * multicore changes
 *
 * Revision 1.2  2008/09/19 07:15:18  gur11912
 * review comments incorporated
 *
 * Revision 1.1  2008/09/09 05:13:56  gur11974
 * Initial Version
 *
 *
 *
 ****************************************************************************/

#ifndef LTELAYER2_TICK_H
#define LTELAYER2_TICK_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteLayer2Types.h"
#include<sys/shm.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>
#include "lteLayer2CommanTypes.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/


/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/* Typedefine the time specific structure used for nanosleep */
typedef struct timespec TimeSpec;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
     /* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
     /* SPR 15909 fix end */

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

#endif /* Included LTELAYER2_TICK_H */
