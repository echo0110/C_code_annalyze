/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteTypes.h,v 1.1 2012/12/28 06:02:54 gur32338 Exp $
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
 * $Log: lteTypes.h,v $
 * Revision 1.1  2012/12/28 06:02:54  gur32338
 * Incorporated Coding guideline adherence changes
 *
 * Revision 1.1.1.1.16.3  2010/10/25 10:48:04  gur23971
 * warning removed
 *
 * Revision 1.1.1.1.16.2  2010/10/25 09:11:47  gur22055
 * warning removal for multiple declaration
 *
 * Revision 1.1.1.1.16.1  2010/09/29 16:00:32  gur20491
 * updated for data types
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.5.2.1  2009/06/09 11:24:46  gur19140
 * first wave of Integration
 *
 * Revision 1.6  2009/05/06 05:39:22  gur20548
 * pdcp common merged with lte common
 *
 * Revision 1.5  2009/04/10 17:22:38  gur18569
 * merged with optmization changes
 *
 * Revision 1.4  2008/10/22 12:41:44  gur11912
 * fixed for performance changes
 *
 * Revision 1.3  2008/09/09 04:55:14  gur11974
 * Added types UChar,UDouble32 and PNULL.
 *
 * Revision 1.2  2008/08/25 10:04:35  ssinghal
 * Template applied and minor changes done
 *
 *
 *
 *
 ****************************************************************************/

#ifndef INCLUDED_MAC_TYPE_H
#define INCLUDED_MAC_TYPE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


/****************************************************************************
 * Exported Types
 ****************************************************************************/

typedef signed int     Int;
typedef unsigned char   UInt8;
typedef unsigned short  UInt16;
typedef unsigned int    UInt32;
typedef unsigned long    ULong32;
typedef unsigned long long   UInt64;
typedef unsigned char   UChar8;
typedef signed char   	SInt8;
typedef char            Char8;
typedef signed short    SInt16;
typedef signed long     SInt64;
typedef float           UDouble32;
typedef double          SDouble64;

/* SPR 20636 Changes Start */
#ifdef __x86_64__
#define ADDR UInt64
#else
#define ADDR UInt32
#endif
/* SPR 20636 Changes End */

#ifndef PNULL
#define PNULL NULL
#endif


#define LTE_FALSE                             0
#define LTE_TRUE                              1 

#define LTE_LOG_CRITICAL                      0
#define LTE_LOG_ERROR                         1
#define LTE_LOG_WARNING                       2
#define LTE_LOG_INFO                          3
#define LTE_LOG_BRIEF                         4
#define LTE_LOG_DETAILED                      5
#define LTE_LOG_DETAILEDALL                   6

#define LTE_L2_LOG_INVALID                    0
#define LTE_L2_LOG_NONE                       LTE_L2_LOG_INVALID
#define LTE_L2_LOG_CRITICAL                   (LTE_LOG_CRITICAL + 1)    /*1*/
#define LTE_L2_LOG_ERROR                      (2 * LTE_L2_LOG_CRITICAL) /*2*/
#define LTE_L2_LOG_WARNING                    (2 * LTE_L2_LOG_ERROR)    /*4*/
#define LTE_L2_LOG_INFO                       (2 * LTE_L2_LOG_WARNING)  /*8*/
#define LTE_L2_LOG_BRIEF                      (2 * LTE_L2_LOG_INFO)     /*16*/
#define LTE_L2_LOG_DETAILED                   (2 * LTE_L2_LOG_BRIEF)    /*32*/
#define LTE_L2_LOG_DETAILEDALL                (2 * LTE_L2_LOG_DETAILED) /*64*/

#define MAX_UL_UE_SCHEDULED 8
#define MAX_DL_UE_SCHEDULED 8

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
 
/****************************************************************************
 * Exported Functions
 ****************************************************************************/

#endif  /* INCLUDED_MAC_TYPE_H */
