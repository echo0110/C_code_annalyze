/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcBuffer.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:30 gur20491 Exp $
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
 * $Log: lteRlcBuffer.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:30  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:14:04  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.5.2.5  2010/07/17 11:34:19  gur23054
 * Review Comments fixed
 *
 * Revision 1.5.2.4  2010/07/12 09:04:48  gur23054
 * removed structure RlcStatusBuffer
 *
 * Revision 1.5.2.3  2010/06/21 08:28:44  gur23054
 * Rlc AM Base Code Merged
 *
 * Revision 1.5.2.2  2010/06/21 08:24:22  gur23054
 * Rlc AM Base Code Merged
 *
 * Revision 1.5.2.1  2010/06/17 10:59:41  gur19836
 * RLC AM Base Code Merged
 *
 * Revision 1.5  2010/06/14 11:57:59  gur21148
 * RLC_UM_MERGE
 *
 * Revision 1.4  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.3  2009/02/05 10:59:40  gur19836
 * CR comments fixed
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

#ifndef RLCBUFFER_H
#define RLCBUFFER_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRlcMem.h"
#include "lteLayer2RlcPdcpInterface.h"
/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define  MAX_DATA_FIELD_SIZE 2047

/* return buffer size */
#define GET_BUFF_SIZE(rlcBuffer_p) \
    (rlcBuffer_p)->size

/* return buffer size used */
#define GET_BUFF_SIZE_USED(rlcBuffer_p) \
    (rlcBuffer_p)->szUsed

#define GET_BUFF_PTR(rlcBuffer_p) \
    (rlcBuffer_p).data_p

#define COPY_BUFF(rlcBufferD_p, rlcBufferS_p) {\
    (rlcBufferD_p)->szUsed = ((rlcBufferD_p)->size >= \
    (rlcBufferS_p)->size ? (rlcBufferS_p)->size : (rlcBufferD_p)->size);\
    memCpy((rlcBufferD_p)->data_p,(rlcBufferS_p)->data_p,(rlcBufferD_p)->szUsed) ;\
}

#define COPY_DATA_TO_BUFF(rlcBufferD_p, voidData_p, ui32_size) {\
    (rlcBufferD_p).szUsed = ((rlcBufferD_p).size >= (ui32_size) ? (ui32_size) : (rlcBufferD_p).size );\
    memCpy((rlcBufferD_p).data_p,(voidData_p),(rlcBufferD_p).szUsed) ;\
}

#define DUMP_DATA_FROM_BUFF(RlcBufferD_p, voidData_p, ui32_size) \
    memCpy((voidData_p),(RlcBufferD_p)->data_p,(ui32_size))

#define MOVE_BUFF(RlcBufferD_p, RlcBufferS_p){ \
    (RlcBufferD_p) = (RlcBufferS_p);\
    (RlcBufferS_p) = RLC_NULL;\
}


#define MOVE_BUFF_CONTENT(RlcBufferD_p, RlcBufferS_p){ \
    (RlcBufferD_p)->szUsed = (RlcBufferS_p)->szUsed;\
    (RlcBufferD_p)->size = (RlcBufferS_p)->size;\
    (RlcBufferD_p)->data_p = (RlcBufferS_p)->data_p;\
    (RlcBufferS_p)->data_p = RLC_NULL;\
}

/* Rlc Rel 2.0 change start */
#define RLC_MAX_STATUS_BUFFER 	2048  /*4096*/
/* RLC Rel 4.0 changes start */
#define RLC_MAX_DFE_IN_PDU   72
/* RLC Rel 4.0 changes end */
/* Rlc Rel 2.0 change end */


/* + SPR 5498 */
#define RLC_MAX_DFE_IN_PDU_UL   256
/* - SPR 5498 */
/****************************************************************************
 * Exported Types
 ****************************************************************************/

/* these are types of buffers */
typedef enum RlcBufferTypeT {
    RLC_ZERO_COPY_BUFFER = 0,
    RLC_ORIGINAL_BUFFER
} RlcBufferType;

typedef struct RlcBufferT {
    RlcBufferType copyType ;
    UInt32 size ;
    UInt32 szUsed ;
    UInt32 payloadSize;
    UInt64 headPktTS;
    UInt8 *data_p ;
/* KPI Changes Start */
#if defined(KPI_STATS) || defined(PERF_STATS)
    /* It will store the sdu count for number of  completely encoded 
     * RLC SDU and last segment of partially encoded SDU.
     * If there is any SDU in RLC PDU which is either first segment or 
     * any mid segment of SDU, that will not be counted */
    UInt8 completeEncodedSduCount;
    /* This array will be used to store the size of individual PDCP SDU */
    UInt16 pdcpSduSize[RLC_MAX_DFE_IN_PDU];
#endif
/* KPI Changes End */
} RlcBuffer ;


/*RLC Rel 2.0 Changes Start*/

typedef struct RlcAmBufferT {
    UInt32 size ;
    UInt8 *data_p ;
    UInt8 RLCRESTBFLAG;
} RlcAmBuffer ;

typedef struct RlcPduBufferT {
    UInt64 headPktTS;
    UInt8 *data_p ;
    UInt16 seqNum ;
    UInt32 payLoadSize;
    UInt32 size ;
/* RLC Rel 4.0 changes start */
#ifndef UE_SIM_TESTING
    UInt8 sduCount;
#endif
/* KPI Changes Start */
#if defined(KPI_STATS) || defined(PERF_STATS)
    /* It will store the sdu count for number of  completely encoded 
     * RLC SDU and last segment of partially encoded SDU.
     * If there is any SDU in RLC PDU which is either first segment or 
     * any mid segment of SDU, that will not be counted */
    UInt8 completeEncodedSduCount;
    /* This array will be used to store the size of individual PDCP SDU */
    UInt16 pdcpSduSize[RLC_MAX_DFE_IN_PDU];
#endif
/* KPI Changes End */
/* RLC Rel 4.0 changes start */
} RlcPduBuffer ;
/*RLC Rel 2.0 Changes End*/

/* RLC Rel 4.0 changes start */
typedef struct RlcPduHeaderInfoT
{
    UInt32  hdrSize;
    UInt16  numDfe;
    UInt8   snLen;
#define RLC_MID_SDU_SEGMENTED       0x03
#define RLC_FIRST_SDU_SEGMENTED     0x02
#define RLC_LAST_SDU_SEGMENTED      0x01
#define RLC_NO_SDU_SEGMENTED        0x00
    UInt8   fi;
    /* + SPR 5498 */
    UInt16  dfeLength[RLC_MAX_DFE_IN_PDU_UL];
    /* - SPR 5498 */
} RlcPduHeaderInfo;
/* RLC Rel 4.0 changes end */

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

RlcBuffer *rlcCreateBuffer(UInt32 size) ;

void rlcSetBuffer(UInt32 size, RlcBuffer* rlcBuff_p) ;

void rlcFreeBuffer(RlcBuffer* buff) ;

void rlcCopyBuffer(RlcBuffer *d,void *s,UInt32 size);

void rlcAppendBuffer(RlcBuffer *d,RlcBuffer *s) ;

RlcBuffer* rlcJoinBuffer(RlcBuffer** d,RlcBuffer **s) ;

RlcBuffer* rlcSplitBuffer(RlcBuffer** d,UInt32 size) ;

RlcBuffer *rlcCreateBufferNZ(UInt32 size) ;

void rlcFreeBufferNZ(RlcBuffer* buff) ;

void rlcZcopyBufferNZ(RlcBuffer *d,void *s,UInt32 size);

void rlcAppendBufferNZ(RlcBuffer *d,RlcBuffer *s) ;

RlcBuffer* rlcJoinBufferNZ(RlcBuffer** d,RlcBuffer **s) ;

RlcBuffer* rlcSplitBufferNZ(RlcBuffer** d,UInt32 size) ;


#endif  /* RLCBUFFER_H */

