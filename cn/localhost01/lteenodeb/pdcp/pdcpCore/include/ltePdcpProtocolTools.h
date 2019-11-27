/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpProtocolTools.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : Functions that are used in PDCP protocol implementation
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: ltePdcpProtocolTools.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.2.1  2009/06/09 11:33:04  gur19140
 * *** empty log message ***
 *
 * Revision 1.2  2009/05/26 14:56:31  gur19140
 * *** empty log message ***
 *
 * Revision 1.1  2009/05/20 13:22:18  gur20548
 * Changed header file name
 *
 * Revision 1.3  2009/05/15 12:22:41  gur19836
 * CR Comments Incorporated
 *
 * Revision 1.4  2011/01/14 12:22:41  gur04340
 * ZCB Optimisation Changes
 *
 *
 ****************************************************************************/

#ifndef _PROTOCOL_TOOLS_DEF_
#define _PROTOCOL_TOOLS_DEF_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "ltePdcpLteFramework.h"
#include "ltePdcpTSDefs.h"
#include "ltePdcpTxQueues.h"
#include "ltePdcpRxQueues.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/*Returns number of bytes needed to store bitSize bits*/
/* SPR 18122 Changes Start*/
#define GET_BYTESIZE_FROM_BITSIZE(bitSize_par) ( (UInt16)((DIVIDE_BY_EIGHT((bitSize_par)-1))+1) )
/* SPR 18122 Changes End*/

/*Increments sn/hfn values*/
#define PDCP_INCREMENT_SN(sn_ptr,hfn_ptr,snSize_par)\
	(*(sn_ptr))++;\
	if(*(sn_ptr) > ((1<<(snSize_par))-1) )\
	{\
        PDCP_LOG(LOG_DETAIL,"Increase HFN by one current sn(%d) and hfn(%d)\n",*(sn_ptr),*(hfn_ptr));\
		*(sn_ptr)=0;\
		(*(hfn_ptr))++;\
	}

/* SPR 15520 fix start */
/* Decrements sn/hfn values */
#define PDCP_DECREMENT_SN(sn_p,hfn_p,snSize)\
    if( 0 == *sn_p ) \
{\
    if( 0 != *hfn_p ) \
    {\
        (*hfn_p)--; \
    }\
    else \
    { /* Count wraparound case */ \
        *hfn_p = ((MAX_UINT32_VALUE)>>snSize); \
    }\
    *sn_p = ((1<<(snSize))-1); \
}\
else \
{\
    (*sn_p)--; \
}
/* SPR 15520 fix end */

/*returns SN for given count, snSize*/
#define PDCP_SN_FROM_COUNT(count_par,snSize_par) ( (UInt16)((count_par) & ((1<<(snSize_par))-1)) )

/*return count value for given sn/hfn*/
#define PDCP_MAKE_COUNT(sn_par,hfn_par,snSize_par) ( ( ((UInt32)(hfn_par)) << (snSize_par) ) | (sn_par) )


/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef YTREE StorageType, *LP_StorageType;

typedef struct StorageNodeT
{
	YTNODE node;
	void *data;
} StorageNode, *LP_StorageNode;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 * Function Name  : pdcpMakePDUHeaderDRB
 * Inputs         : UInt32 count, UInt16 snSize, void *data_p
 * Outputs        : None.
 * Returns        : returns allocated PDU DRB header
 * Description    : Make PDU header for DRB packet
 ****************************************************************************/
void* pdcpMakePDUHeaderDRB( UInt32 count, UInt16 snSize, void *data_p);

/****************************************************************************
 * Function Name  : pdcpMakePDUHeaderSRB
 * Inputs         : UInt32 count, UInt16 snSize, void *data_p
 * Outputs        : None.
 * Returns        : returns allocated PDU SRB header
 * Description    : Make PDU header for SRB packet
 ****************************************************************************/
void* pdcpMakePDUHeaderSRB( UInt32 count, UInt16 snSize, void *data_p);

/****************************************************************************
 * Function Name  : pdcpLogandIgnore
 * Inputs         : ueIndex, lcId, state, event, ctx_p
 * Outputs        : None.
 * Returns        : Success
 * Description    : Logs an error message and returns
 ****************************************************************************/
UInt32 pdcpLogandIgnore(UInt16 ueIndex, UInt8 lcId, 
        UInt32 state, UInt32 event, void *context_p);


#endif /* _PROTOCOL_TOOLS_DEF_  */
