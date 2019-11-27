/**************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 **************************************************************************
 *
 *  $Id: ltePosixMessageQ.h
 *
 **************************************************************************
 *
 *  File Description : This file contains declarations for message queue
 *                     functions.
 **************************************************************************/
/**********************************************************************
 * Standard Library Includes
 *
 *********************************************************************/
#include "mqueue.h"

/**********************************************************************
 * Project Includes
 *********************************************************************/

#include "lteTypes.h"

/***************************************************************************
 * Macros Used
 ***************************************************************************/

/*SPR 20712 Start*/
/* spr 18983 */
#define MAX_MSG_SZ   30000
/* spr 18983 */
#define MAX_MSG_NO   20
/*SPR 20712 End*/
#ifndef _MQD_T_
#define _MQD_T_
typedef  mqd_t MQD_T;
#endif
/***********************************************************************
 *  Exported Functions
 **********************************************************************/
MQD_T lteInitMessageQTx(char *qName_p);
MQD_T lteInitMessageQRx(char *qName_p);
SInt8 lteMessageQSend(MQD_T fdSend, UInt8 *buffer, UInt16 size);
SInt8 lteMessageQUnlink(char *qName_p);
SInt16 lteMessageQRecv(SInt32 fdRecv, UInt8 *buffer);
