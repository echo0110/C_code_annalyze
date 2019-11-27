
/**************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc. All Rights Reserved.
 *
 *************************************************************************
 *
 * $Id: lteMacComPorting.h
 *
 *************************************************************************
 *
 *  File Description : This file contains the function prototypes needed for
 *                     MAC porting
 *
 *************************************************************************/


#ifndef  LTE_MAC_COM_PORTING_H
#define LTE_MAC_COM_PORTING_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteTypes.h"
#include "lteMisc.h"
#include "lteCommonStatsManager.h"
#include "lteMessageQIntf.h" 
#include <semaphore.h>

/*Coverity 95473 Fix Start*/
#define SOCKET_ERROR (-1)
/*Coverity 95473 Fix End*/

/* SPR 4915 : Start */
#define MAX_MQ_NAME_LEN  256
/* SPR 4915 : End   */
#define MQ_ERROR         -1
#define RECV_MSG_FRM_HP_THREAD_Q_NAME "/RecvMsgFrmHPThd"

/****************************************************************************
 * Exported Types
 ****************************************************************************/
SInt32 initMsgQueueForICC(const char *name, SInt32 oflag);
SInt32 unlinkMsgQueueForICC(const char *name); 
SInt32 sendMsgWithIccQ(SInt32 mqdes, const char *msg_ptr, size_t msg_len, unsigned msg_prio);
SInt32 recvMsgWithIccQ(SInt32 mqdes, const void *msg_ptr);
/* + SPR 17439 */
void initComRRCAndOAMInterface(void);
/* - SPR 17439 */
/* CA Changes start */
void initComPhySocketInterface(InternalCellIndex cellId);
/* CA Changes end */
void fcntl_f_setfl(SInt32 phyRxSockFD);
/* CA changes start */
SInt32 initComGtpDataMsgQ(Char8 * buff, UInt32 strlen, Char8* str);
/* CA changes end */

void dumpULStatsToFile(UInt8 statsInfoArrIndex,
                                      UInt32 statsTickIndex,ulStatsInfo * statsULInfoArr_p,UInt32 invalid_ue_id
                                    );

void dumpDLStatsToFile( UInt8 statsInfoArrIndex,
                                      UInt32 statsTickIndex,dlStatsInfo * statsDLInfoArr_p
                                    );

/* +- SPR 17777 */

void initEncoderQueue ( SInt32 *qId, UInt8 encoderContainerCount, 
    InternalCellIndex idx,
    InternalCellIndex internalCellIndex2);
void closeULContainerNackQueue(UInt32 max_num_of_container );
void closeEncoderQueue(UInt32 max_encoder_containers);

void initComULContainerNackQueue(SInt32 *qId, UInt8 index);

void initComULContainerNackQueueForTempUE( SInt32 *qId, UInt8 index);

void initComEncoderQueue ( SInt32 *qId, UInt8 encoderContainerCount, 
    InternalCellIndex internalCellIndex
);

MQD_T initMacToRrmMsgQueue (UInt32 Id);
MQD_T initRrmToMacMsgQueue (UInt32 Id);
/* + SPR 17439 */
MQD_T initHptoLpIndicationQueue(void);
/* - SPR 17439 */

/******************************************************************************
 * Function Name: initMsgQFdFrmHpThread
 * Inputs       : None
 * Outputs      : Nome
 * Return       : Message queue FD
 * Description  : It creates message queue between LP and HP thread and 
 *                returns its FD.
******************************************************************************/
/* + SPR 17439 */
MQD_T initMsgQFdFrmHpThread(void);
/* - SPR 17439 */

extern MQD_T recvMsgFrmHPThdFd_g;

/* CA Logging Changes Start */
void prepareshmRegionName(Char8 * shmRegionName, UInt32 name_len,Char8 *str,
        SInt32 instanceL2, InternalCellIndex internalCellIndex);

/*Coverity 26339 Fix Start*/
void prepareLoggerFileName(Char8 * logger_file_name, Char8 *str, 
        SInt32 instanceL2,UInt32 size, InternalCellIndex internalCellIndex);
/*Coverity 26339 Fix End*/

void preparePdcpShmRegionName(Char8 * shmRegionName, UInt32 name_len,Char8 *str,
        SInt32 instanceL2);

void prepareFileName(Char8 * shmRegionName, UInt32 name_len,Char8 *str, 
        InternalCellIndex internalCellIndex);
/* CA Logging Changes End */

/* +SPR_10381 Changes */
/* Porting changes - start */
void prepareEncoderLoggerFileName(Char8 * logger_file_name, UInt32 name_len,Char8 *str,
        InternalCellIndex internalCellIndex);
/* Porting changes - end */
/* -SPR_10381 Changes */
/****************************************************************************
 * Exported Variables
 ****************************************************************************/
 /*SPR 16855 +-*/



/*****************************************************************************
 * Function Name: setFd
 * Inputs       : fd - FD which needs to be set
 * Outputs      : None
 * Returns      : None
 * Description  : This function sets the FD in select to receive from socket.
*****************************************************************************/
void setFd( SInt32 fd );

/*****************************************************************************
 * Function Name: setMsgQFd
 * Inputs       : fd - FD which needs to be set
 * Outputs      : None
 * Returns      : None
 * Description  : This function sets the message queue FD in select to receive
 *                from socket.
*****************************************************************************/
void setMsgQFd( MQD_T fd );
#endif
