/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteShmLogger.h,v 1.1.1.1.16.1 2010/10/25 10:37:30 gur23971 Exp $
 *
 ****************************************************************************
 *
 *  File Description : The file contains the necessary declarations for 
 *                     shared memory logger module.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteShmLogger.h,v $
 * Revision 1.1.1.1.16.1  2010/10/25 10:37:30  gur23971
 * warning removed
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1.2.1  2009/09/18 07:03:55  gur20439
 * integration changes for MAC1.2
 *
 * Revision 1.3.2.1  2009/06/09 11:24:46  gur19140
 * first wave of Integration
 *
 * Revision 1.2  2009/05/27 15:09:48  gur18569
 * changes in function signatures
 *
 * Revision 1.1  2009/05/25 09:33:34  gur18569
 * initial version
 *
 *
 *
 ****************************************************************************/

#ifndef SHMLOGGER_H
#define SHMLOGGER_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteTypes.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define LOGGER_THREAD_PRIORITY      0
#define SHM_HEADER_SIZE            13
#define MAX_LOG_REGION_SIZE       9194304 /* 4 MB*/
#define MAX_LOG_REGION_INDEX      9194300 /* 4 MB - 4*/
#define MAX_MODULE_NAME_SIZE      255
#define MAX_REGISTERED_MODULES     30
#define MAX_LOG_LEN              1000
#define MSG_HEADER_LENGTH           4 
#define MAX_NUM_OF_MSGS_TO_WRITE 1000



/****************************************************************************
 * Exported Types
 ****************************************************************************/
/*Whenever a registration occurs it should specify it's module , so tht 
 * it's modules properties like log level filtering can be applied to 
 * the registering thread */
enum 
{
    PDCP_MODULE,
    RLC_MODULE,
    MAC_MODULE,
    TOTAL_NUM_OF_MODULES
};

/*This structure defines the header for shm region , a TLS object is 
 * pointer of this structure*/
typedef struct shmRegionStructT
{
    /* The value in this pointer is checked against the level sent as
     * an argument to lteShmLog().This is for log filtering*/
    UInt32* level_p;

    /* The value of the offset of current write pointer*/
    UInt32 writeOffset;

    /* The value of the offset of current read pointer*/
    UInt32 readOffset;

    /* This value becomes true when while writing we have reached
     * max. value of shared memory allocated and we need to wrap
     * to begining */
    UInt8 wrap;
}shmRegionStruct;

/* This is structure maintained per registration request*/
typedef struct logRegisterInfoT
{
    /* The file descriptor the file into which logs will be flushed */
    SInt32 fd;

    /* Pointer to shared memory region which is used as buffer for 
     * logs being flushed into file*/
    shmRegionStruct * shmRegion_p;

    /* Its the name provided by registering thread and is used to
     * create file into which logs will be flushed*/
    Char8 registeredModuleName[MAX_MODULE_NAME_SIZE];
}logRegisterInfo;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
 
/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/* + SPR 17439 */
UInt32 initShmLogger(void);
void lteShmLog(UInt32 level ,Char8 * prefixSring_p ,const Char8 *logString_p, ...);
void disableLogging(void);
void stopLoggerThread(void);
/* - SPR 17439 */
UInt32 lteShmSetModuleLogLevel(UInt32 moduleId , UInt32 level);

#endif
