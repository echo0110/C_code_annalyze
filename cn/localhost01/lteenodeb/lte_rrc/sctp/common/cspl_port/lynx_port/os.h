/* all the os related files shall be included from this file only */
/*****************************************************************************
 *    FILE NAME:
 *        os.h
 *
 *    DESCRIPTION:
 *       This file shall include all the OS files required for solaris
 *
 *
 *    DATE           NAME          REFERENCE        REASON
 *    ----------------------------------------------------
 *    11APR05        Satyendra             -        LynxOS Porting
 *
 *     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#include    <ctype.h>
#include    <stdarg.h>
#include    <sys/types.h>
#include    <netinet/in.h>
#include    <stdio.h>
#include    <string.h>

#include    <stdlib.h>
#include    <time.h>
#include    <limits.h>
#include    <unistd.h>
#include    <fcntl.h>
#include    <sys/socket.h>
#include    <sys/times.h>
#include    <sys/time.h>
#include    <sys/select.h>
#include    <sys/stat.h>
#include    <sys/uio.h>
#include    <sys/un.h>
#include    <arpa/inet.h>
#include    <sys/ipc.h>
#include    <sys/msg.h>

#include    <netdb.h>
#include    <errno.h>

