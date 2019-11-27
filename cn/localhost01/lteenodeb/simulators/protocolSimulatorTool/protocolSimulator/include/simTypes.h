/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent. Ukraine
 *
 ****************************************************************************
 *
 *
 ****************************************************************************
 *
 *  File Description : It contains the common types.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 *
 ****************************************************************************/

#ifndef INCLUDED_SIM_TYPE_H
#define INCLUDED_SIM_TYPE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/timeb.h>

/****************************************************************************
 * Exported Includes
 ****************************************************************************/


/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


/****************************************************************************
 * Exported Types
 ****************************************************************************/

typedef int                 Int;
typedef unsigned char       UInt8;
typedef unsigned short      UInt16;
typedef unsigned int        UInt32;
typedef unsigned long long  UInt64;
typedef char                UChar8;
typedef signed char         SInt8;
typedef signed short        SInt16;
typedef signed int          SInt32;
typedef signed long long    SInt64;
typedef float               UDouble32;

#ifndef _PNULL_
#define _PNULL_
#define PNULL               ((void*)0)
#endif

/*boolean constants TRUE/FALSE*/
#define SIM_FALSE               0
#define SIM_TRUE                1
/****************************************************************************
 * Exported Constants
 ****************************************************************************/

#define SOCKET_BUFFER_SIZE  /*(16*1024)*/0xFFFE
#define HOSTNAME_MAX_LEN    64

#ifdef WIN32
    #include <winsock.h>
    typedef SOCKET SocketType;
    #define SOCKET_FLAGS 0
    #define SOCKET_FLAGS_WAIT 0
#else
    typedef Int SocketType;
    #define SOCKET_ERROR (-1)
    #define SOCKET_FLAGS 0
    #define SOCKET_FLAGS_WAIT 0
#endif

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

#endif  /* INCLUDED_SIM_TYPE_H */
