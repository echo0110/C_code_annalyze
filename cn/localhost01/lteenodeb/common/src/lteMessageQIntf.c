/**************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 *************************************************************************
 *
 *  $Id: lteMessageQIntf.c
 *
 * ***********************************************************************
 *
 *  File Description : This file contains the functions to create POSIX
 *                     message queue and to send and receive data using it
 *
 *
 *****************************************************************************
 *  Standard Library Includes
 *****************************************************************************/

#include <stdio.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <lteLog.h>

/*****************************************************************************
 *  Project Includes
 *****************************************************************************/
#include "lteMessageQIntf.h"
#include "lteMisc.h"
/********************************************************************************
 *  Private definitions
 ********************************************************************************/
/* +SPR - 7531*/
/* SPR 20712 fix start */
/* Code deleted */
/* -SPR - 7531*/
/* SPR 20712 fix end */
#define SET_PERM     0666

/*******************************************************************************
 *  Global Types
 ********************************************************************************/
/********************************************************************************
 * Function Name    : lteInitMessageQTx
 * Inputs           : None
 * Returns          : value returned by mq_open
 * Description      : This function creates a message queue for sending
 *                    with specified set of attributes and set of permissions.
 * *********************************************************************************/
MQD_T lteInitMessageQTx(char *qName_p)
{
    MQD_T retVal = 0;
    struct mq_attr *p_attr = NULL;
    
#ifndef LINUX_PC_TEST
    struct mq_attr attr;

    attr.mq_maxmsg = MAX_MSG_NO;
    attr.mq_msgsize = MAX_MSG_SZ;
    attr.mq_flags = 0;
    p_attr = &attr;
#endif

    return mq_open_wrapper(qName_p, O_CREAT|O_RDWR|O_NONBLOCK, 0, p_attr);
}


/********************************************************************************
 * Function Name    : lteInitMessageQRx
 * Inputs           : None
 * Returns          : value returned by mq_open
 * Description      : This function creates a message queue with specified set of
 *                    attributes and set of permissions.
 * *********************************************************************************/
MQD_T lteInitMessageQRx(char *qName_p)
{
    MQD_T retVal = 0;
    mode_t omask;
    omask = umask(0);
    
#ifndef LINUX_PC_TEST
    struct mq_attr attr;

    attr.mq_maxmsg = MAX_MSG_NO;
    attr.mq_msgsize = MAX_MSG_SZ;
    attr.mq_flags = 0;

    /*Creating message queue object for receiving    ----mq_open()*/
    retVal =  mq_open(qName_p, O_CREAT|O_RDWR|O_NONBLOCK, SET_PERM, &attr);
#else
    retVal =  mq_open(qName_p, O_CREAT|O_RDWR|O_NONBLOCK, (S_IRWXU|S_IRWXG|S_IRWXO), NULL);
#endif
    umask(omask);
    return retVal;
}
/********************************************************************************
 * Function Name    : lteMessageQSend
 * Inputs           : fdSend ->FD to send,
		      buffer ->pointer to message buffer and size of message
 *                    size   -> length of the buffer
 * Returns          : value returned by mq_send
 * Description      : this function is used to call mq_send() to send the message
 *                    using message queue
 * *********************************************************************************/
SInt8 lteMessageQSend(MQD_T fdSend, UInt8 *buffer, UInt16 size)
{
    SInt8 retVal = 0;
    /*Sending data *******/
    retVal = mq_send(fdSend, (char *)buffer, size, 0); /* gcc 5.0 */
    return retVal;
}


/********************************************************************************
 * Function Name    : lteMessageQRecv
 * Inputs           : fdRecv ->FD to receive
		      buffer ->pointer to message buffer
 * Returns          : value returned by mq_receive
 * Description      : this function is used to call mq_receive() to receive the message
 *                    using message queue
 * *********************************************************************************/
/* +- SPR 17777 */
SInt16 lteMessageQRecv(SInt32 fdRecv, UInt8 *buffer)
{
    /* +- SPR 17777 */
    ssize_t retVal = 0;
    /*Receiving data **********/
    /* SPR 5650 Start */
    retVal = mq_receive((MQD_T)fdRecv, (char *)buffer, MAX_MSG_SZ, 0);
    /* SPR 5650 End */
    return retVal;
}

/***********************************************************************************
 * Function Name    :   lteMessageQUnlink
 * Inputs           :   qName_p
 * Returns          :
 * Description      :   This wrapper function is used to unlink message queue
 * *********************************************************************************/
SInt8 lteMessageQUnlink(char *qName_p)
{
    return mq_unlink(qName_p);
}
