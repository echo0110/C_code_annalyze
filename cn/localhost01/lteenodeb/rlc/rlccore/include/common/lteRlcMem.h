/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcMem.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:34 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteRlcMem.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:34  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:14:04  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.7.28.1  2010/06/17 10:59:42  gur19836
 * RLC AM Base Code Merged
 *
 * Revision 1.7  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.6  2009/02/09 05:00:02  gur20294
 * update
 *
 * Revision 1.5  2009/02/06 15:39:33  gur20294
 * updated
 *
 * Revision 1.4  2009/02/06 15:03:49  gur20294
 * updated
 *
 * Revision 1.3  2009/01/30 13:35:56  gur20294
 * changed function name to lteRlcMem
 *
 * Revision 1.2  2009/01/29 05:35:22  gur20294
 * after code review comments incorporated
 *
 * Revision 1.1  2009/01/06 11:58:57  gur20294
 * renamed lteRlcSysInterface.h to lteRlcMem.h
 *
 * Revision 1.1  2008/12/30 06:24:49  gur20294
 * RLC Layer added. INITIAL DRAFT
 *
 *
 *
 ****************************************************************************/

#ifndef SYSINTERFACE_H
#define SYSINTERFACE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteTypes.h"
#include "lteLog.h"
#include "lteMsgPool.h"
#include "lteMemPool.h"
#include "lteMisc.h"
#include "lteList.h"
#include "lteQueue.h"
#include "ltePQueue.h"
#include "lteSQueue.h"
#include "lteThread.h"
#include "lteSync.h"


/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
 
/* return status */
#define RLC_FAILURE 0
#define RLC_SUCCESS 1
#define RLC_PARTIAL_SUCCESS 2

/*RLC Error Codes*/
#define RLC_ERR_TID_NODE_EMPTY_IN_RETX_BUFFER 	3
#define RLC_ERR_FREE_SDU_TID_FAILED		4
#define RLC_ERR_NO_TID_PRESENT_FOR_DEL_IND	5
#define RLC_ERR_FREE_RETX_BUFFER_FAILED		6
#define RLC_ERR_NON_ZERO_DC_CPT_IN_CONTROL_PDU  7
#define RLC_ERR_INVALID_ACK_RECVD		8
#define RLC_ERR_OUT_OF_WINDOW_ACK_RECVD		9
#define RLC_ERR_EMPTY_NACK_Q_IN_ENCODING	10
#define RLC_ERR_NULL_RETX_PDU_POINTER_FOUND     11

#define RLC_INVALID_SN 1025

#define RLC_NULL PNULL 
/*UInt16 max value*/
#define RLC_UNULL (0x7FFFU)

/* macro to free memory */
#define RLC_FREE(void_ptr)\
            freeMemPool(void_ptr)
/* macro to allocate memory */            
#define RLC_ALLOC(ui_sz)\
            getMemFromPool(ui_sz,RLC_NULL)
/* macro to free memory */
#define RLC_MSG_FREE(void_ptr)\
            msgFree(void_ptr)
/* macro to allocate memory */            
#define RLC_MSG_ALLOC(ui_sz)\
            msgAlloc(RLC_NULL,0,0,ui_sz)


/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef enum RlcDataTypeT {
    RLC_DATA_PDCP_UM_REQ = 0 ,
    RLC_DATA_PDCP_UM_IND ,
    RLC_DATA_PDCP_AM_REQ ,
    RLC_DATA_PDCP_AM_IND ,
    RLC_DATA_PDCP_TM_REQ ,
    RLC_DATA_PDCP_TM_IND ,
    RLC_DATA_MAC_REQ ,
    RLC_DATA_MAC_IND ,
    RLC_DATA_MAC_OPP_IND ,
    RLC_DATA_RRC_RE_EST_REQ
} RlcDataT ;


typedef struct RlcGenericT{
    LTE_LIST_NODE qN ;
    RlcDataT dataT ;
    void* data_p ;
} RlcGeneric ;

typedef struct RlcGenericQT{
    LTE_LIST *pQ ;
    UInt32 mutex ;
} RlcGenericQ ;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/* + SPR 17439 */
void rlcInitMem(void) ;
/* - SPR 17439 */
#endif  /* INCLUDED_FILENAME_H */
