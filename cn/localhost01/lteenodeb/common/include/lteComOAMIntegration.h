/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteComOAMIntegration.h
 *
 ****************************************************************************
 *
 *  File Description : This file contains the function prototypes and 
 *                     data structures needed for O&M integration
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 ****************************************************************************/

#ifndef LTE_COM_OAMINTEGRATION_H 
#define LTE_COM_OAMINTEGRATION_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteTypes.h"

/****************************************************************************
 * Exported Types
 ****************************************************************************/
/*SPR 21653 changes start*/
UInt32 getCommandLineOptions(int argc, char *argv[]);
/*SPR21717 changes start*/
UInt32 getCommandLineOptionsSplit(int argc, char *argv[]);
/*SPR21717 changes end*/
/*SPR 21653 changes end*/

/* + SPR 17439 */
unsigned long long getCcInMs(void);

void updateElCycleCount(void);
/* - SPR 17439 */
SInt32 initComOamDistConfMsgQ(Char8* oam_config_msgQ,UInt32 strlen, Char8* str);

void* initDistSharedMemParam(UInt32 key_v,SInt32 sizeOfShm);
/*SPR 21653 changes start*/
void initAndConfigLogFramework( 
/* + SPR 17439 */
void
/* + SPR 17439 */
);
/*SPR 21653 changes end*/

void stopAllThread (UInt32 max_thread_id,ULong32 *threadId_arr);
/* + SPR 17439 */
UInt32 enableDebugTraceback(void);
/* - SPR 17439 */
/****************************************************************************
 * Exported Variables
 ****************************************************************************/
#endif
