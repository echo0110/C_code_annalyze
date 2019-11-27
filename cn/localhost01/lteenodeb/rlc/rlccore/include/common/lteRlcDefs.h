/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcDefs.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:31 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *  This file defines high-level constants and definitions.  
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteRlcDefs.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:31  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:14:04  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.6.2.3  2010/07/03 11:16:28  gur19479
 * updated for macros
 *
 * Revision 1.6.2.2  2010/07/03 06:03:04  gur19479
 * updated for resegmentation
 *
 * Revision 1.6.2.1  2010/06/17 10:59:42  gur19836
 * RLC AM Base Code Merged
 *
 * Revision 1.6  2010/06/14 11:57:59  gur21148
 * RLC_UM_MERGE
 *
 * Revision 1.5  2009/04/22 04:33:35  gur19836
 * memory leak related changes
 *
 * Revision 1.4  2009/04/09 15:23:08  gur19836
 * 100 ue related fixes
 *
 * Revision 1.3  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
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

#ifndef DEFS_H 
#define DEFS_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteTypes.h"
#include "lteLog.h"
#include "lteRlcTypes.h"
#include "lteRlcBuffer.h"

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/*x * 12 == x * 8 + x * 4 == (x << 3) + (x << 2) */
#define GET_VAR_HEADER_SIZE(coutSDU)\
   ( ((coutSDU -1) << 3) + ((coutSDU -1) << 2) + 7 ) >> 3 

/* RLC window sizes */
#define UM_WINDOW_SN_5  16  /* window size for UM mode for 5-bit SN (Sequence Number)*/
#define UM_WINDOW_SN_10 512 /* window size for UM mode for 10-bit SN */
#define UM_WINDOW_SZ(snFeildLen) (((snFeildLen) == 5)? UM_WINDOW_SN_5 : UM_WINDOW_SN_10) /* window size for UM mode */
/*RLC Rel 2.0 Changes Start*/
#define AM_WINDOW_SIZE 512 /* window size for AM mode for 10-bit SN */
/*RLC Rel 2.0 Changes End*/

/* RLC window sizes */
#define UM_MOD_VAL_SN_5  32  /* modulo value for UM mode for 5-bit SN (Sequence Number)*/
#define UM_MOD_VAL_SN_10 1024 /* modulo value for UM mode for 10-bit SN */
#define UM_MOD_VAL(snFeildLen) (((snFeildLen) == 5)? UM_MOD_VAL_SN_5 : UM_MOD_VAL_SN_10 )  /* modulo value for UM mode  */
#define AM_MOD_VAL_SN_10 1024 /* modulo value for AM mode for 10-bit SN */

/* RLC  Timers  */
#define TIMER_REORDERING_DEFAULT 5 /*value is in milliseconds */
/*RLC Rel 2.0 Changes Start*/
#define TIMER_TPOLLRETRANSMIT_DEFAULT 5 /*value is in milliseconds */
#define TIMER_TSTATUSPROHIBIT_DEFAULT 5 /*value is in milliseconds */
/*RLC Rel 2.0 Changes End*/

/* Error Code for RLC */
#define RLC_ERR_BASE 100
#define RLC_ERR_DB_NOTFOUND             (RLC_ERR_BASE + 1)
#define RLC_ERR_DB_INVALID_PARAM_VAL    (RLC_ERR_BASE + 2)
#define RLC_ERR_DB_ALLOC_FAILED         (RLC_ERR_BASE + 4)

/** Rel 4.0 changes Start **/
/*RLC can store max sdu in queue*/
#ifdef UE_SIM_TESTING
#define MAX_NUMBER_OF_SDU              1024 
#else
#define MAX_NUMBER_OF_SDU              512 
#endif
/** Rel 4.0 changes End **/

/* RLC SN size */
#define RLC_AM_SN_SIZE 10

/* RLC AM E SIZE */
#define RLC_E_SIZE 1

/* RLC AM CPT SIZE */
#define RLC_AM_CPT_SIZE 3

/* RLC AM FI SIZE */
#define RLC_AM_FI_SIZE 2 

/* RLC SO START/END size */
#define RLC_AM_SO_SIZE  15
#define RLC_AM_SO_MASK 0x7fff

/* RLC SN Mask */
#define RLC_AM_SN_MASK 0x3ff

/* RLC FI Mask */
#define RLC_AM_FI_MASK 0x18

/* RLC LI MASK */
#define RLC_AM_LI_MASK 0x7ff
 
/* RLC Modulus 8 */
/* RLC Modulus 8 */
/* SPR 18122 Changes Start*/
#define RLC_AM_MOD_8(value) MODULO((value),8)
/* SPR 18122 Changes End*/

/* RLC LI SIZE */
#define RLC_AM_LI_SIZE 11

#endif  /* DEFS_H   */
