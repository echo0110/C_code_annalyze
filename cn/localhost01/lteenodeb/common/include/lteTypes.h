/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteTypes.h,v 1.1.1.1.16.3 2010/10/25 10:48:04 gur23971 Exp $
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

#ifndef INCLUDED_LTE_TYPES_H
#define INCLUDED_LTE_TYPES_H

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
typedef signed int      SInt32;
typedef signed long long     SInt64;
typedef float           UDouble32;
typedef double          UDouble64;
#ifndef _PNULL_
#define _PNULL_
#define PNULL NULL
#endif /*_PNULL_*/
/* SPR 19288 change start */
/* SPR 20688 fix start*/
/* SPR 15909 fix start */
typedef UInt64 tickType_t;
/* SPR 15909 fix end */
/* SPR 20688 fix end*/
/* SPR 19288 change end */
#define EMTC_MAX_TICK_VALUE 0xFFFFFFFF
/* +- SPR 17777 */
#define LTE_GCC_UNUSED_PARAM(param) (void)param;
/* +- SPR 17777 */


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

/* SPR 20874 start */
#define SPS_QCI     1 
/* SPR 20874 end */
/* SPR 9381 changes start */
/* SPR 14759 Start */
#ifdef TDD_CONFIG
#define MAX_UL_UE_SCHEDULED ((32 > (2*MAX_UE_SUPPORTED)) ? ((2*MAX_UE_SUPPORTED)):32)
/* SPR 14759 End */
#else
/* SPR 22099 Fix + */
#define MAX_UL_UE_SCHEDULED ((16 > MAX_UE_SUPPORTED) ? MAX_UE_SUPPORTED:16)
#endif 
#define MAX_DL_UE_SCHEDULED ((16 > MAX_UE_SUPPORTED) ? MAX_UE_SUPPORTED:16)
/* SPR 22099 Fix - */
/* SPR 9381 changes end */

/* SPR 16116 fix start */
#define MAX_TOP_SCHEDULABLE_UE_LIST_SIZE (MAX_DL_UE_SCHEDULED*2)
/* SPR 16116 fix end */
#ifdef DEBUG_STATS
#define LC_1_MAX_DATA 32
#define LC_2_MAX_DATA 32
#define LC_3_MAX_DATA 1024
#define MAX_DATA_BYTES 16
typedef struct pduUlDebugInfoT
{
    UInt8 strorePduDataLC1[LC_1_MAX_DATA][MAX_DATA_BYTES];
    UInt8 strorePduDataLC2[LC_2_MAX_DATA][MAX_DATA_BYTES];
    UInt8 strorePduDataLC3[LC_3_MAX_DATA][MAX_DATA_BYTES];
    UInt32 tick1[LC_1_MAX_DATA];
    UInt32 tick2[LC_2_MAX_DATA];
    UInt32 tick3[LC_3_MAX_DATA];
    UInt16 lc1Idex;
    UInt16 lc2Idex;
    UInt16 lc3Idex;
}pduUlDebugInfo;
#endif

/* CA changes start */
typedef UInt8 RrcCellIndex;
typedef UInt8 InternalCellIndex;
typedef UInt8 ServingCellIndex;
/* Maximum number of Cells supported in Layer 2 */
#define MAX_NUM_CELL        2
#define MAX_NUM_FD_CONTAINER 2
/* Maximum number of Secondary Cells supported in Layer 2 */
#define MAX_NUM_SCELL       1
#define MAX_NUM_PCELL       1
#define MAX_FREQ_BAND_INDICATOR         64
#define MAX_EARFCN                   65535
#define PCELL_SERV_CELL_IX  0
#define MAX_SCELLS          7
#define MAX_SERVCELL ((MAX_NUM_PCELL)+(MAX_SCELLS))
#define INVALID_CELL_INDEX  0xFF
/* Ue specific APIs use this cellIndex */
/* + SPR 10767 Fix */
#define API_HEADER_DO_NOT_CARE_CELL_INDEX  0xff
/* - SPR 10767 Fix */
/* External Cell Index can be from 0...7 */
#define MAX_RRC_CELL_INDEX  7
/* External Cell Index can be from 0...1 */
#define MAX_INTERNAL_CELL_INDEX  (MAX_NUM_CELL -1)
/* e.g 11110000 for  numBits = 4 */
#define GETMASK_LEFT_ALIGNED(numBits) ((~(~0U >> (numBits))))
/* e.g 00001111 for  numBits = 4 */
#define GETMASK_RIGHT_ALIGNED(numBits) ((~(~0U << (numBits))))   

/* set given range of bits in Byte to zero*/
#define ZERO_BYTE_FIELD(byte,sBitN,numBits) ((byte) &= ~(GETMASK_RIGHT_ALIGNED(numBits) << (sBitN)))
/* set given range of bits to value */
#define SET_BYTE_FIELD(byte,value,sBitN,numBits) (ZERO_BYTE_FIELD(byte,sBitN,numBits), ((byte) |= ((value) & GETMASK_RIGHT_ALIGNED(numBits)) << (sBitN)) )

#define MAX_QCI                  9
/* following number will represent number of UEs in 
 * one batch while reporting throughput KPI */
#define MAX_UE_PER_BATCH         10
#define MAX_SPS_INTERVALS        16
#define SPS_INTERVAL_LIST        16
#define MAX_LCID_THP             8
#define MAX_LOGICAL_CHANNEL      11
#define MAX_NUMBER_OF_LOGICAL_CHANNEL      MAX_LOGICAL_CHANNEL
#define MAX_NUM_L2_INSTANCE    3

/*SPR 22325 Start*/

#if   HIGH_SCALABILITY_L2_MAX_UE
#define MAX_UE_SUPPORTED       1210
#elif HIGH_SCALABILITY_L3_MAX_UE
#define MAX_UE_SUPPORTED       3630
#else
#define MAX_UE_SUPPORTED       260
#define MAX_NB_UE_SUPPORTED    255
#endif

/*SPR 22325 End*/

#define MAX_BIN_NAME_LEN        50
#define MAX_CQI_INDEX 16

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
#define MAX_UINT32_VALUE  ((UInt32)4294967295U)
  /* SPR23838 Fix Start */
#define MAX_UINT64_VALUE  ((UInt64)18446744073709551615U)   
  /* SPR23838 Fix End */

/* SPR 18122 Changes Start */
#define DIVIDE_BY_TWO(x) ((x) >> 1)
#define DIVIDE_BY_FOUR(x) ((x) >> 2)
#define DIVIDE_BY_EIGHT(x) ((x) >> 3)
#define DIVIDE_BY_SIXTEEN(x) ((x) >> 4)
#define DIVIDE_BY_THIRTY_TWO(x) ((x) >> 5)
#define DIVIDE_BY_SIXTY_FOUR(x) ((x) >> 6)
#define DIVIDE_BY_1024(x) ((x) >> 10)

#define MULTIPLY_BY_TWO(x) ((x) << 1)
#define MULTIPLY_BY_FOUR(x) ((x) << 2)
#define MULTIPLY_BY_EIGHT(x) ((x) << 3)
#define MULTIPLY_BY_SIXTEEN(x) ((x) << 4)
#define MULTIPLY_BY_ONEZEROTWOFOUR(x) ((x) << 10)
#define MODULO(x,d) ((x)&(d-1))
/* SPR 20636 Changes Start */
#ifdef __x86_64__
#define ADDR UInt64
#else
#define ADDR UInt32
#endif
/* SPR 20636 Changes End */
/* SPR 18122 Changes End */
#define MAX_SFN_VALUE         1024  /*As per 36.331 SFN is of 10 bits*/
#define MAX_SFN MAX_SFN_VALUE
#define MAX_SUBFRAME 10
#define MAX_SUB_FRAME MAX_SUBFRAME
#define MAX_SF_VALUE  MAX_SUBFRAME
#define NUM_OF_SFN_BITS  10 
#define NUM_OF_SF_BITS    4
/*spr 22474 start*/
#define NVARPOOL    27
#define MINBUF      8
#define NVARMSGPOOL    27
#define NVARMEMPOOL    27
/* This section defines the common structure used across layer-2 modules with OAM */
/* This structure provides the POOL stats */
typedef struct Layer2PoolStatsT {
    /* the size of the buffer allocated in this pool */
    UInt32 bufSize; /*^ M, 0 ^*/
    /* the Total number of Buffers in this pool */
    UInt32 numOfBuf; /*^ M, 0 ^*/
    /* the Total number of Buffers allocated from this pool */
    UInt32 numOfAllocBuf; /*^ M, 0 ^*/
    /* the Peak of Total number of Buffers allocated from this pool during the system is running */
    UInt32 numOfPeakBuf; /*^ M, 0 ^*/
}Layer2PoolStats;

/* This structure provides the MEM POOL stats */
typedef struct L2MemPoolStatsT {
    /* This array is terminated when bufSize is 0 for an entry, other entries are not valid */
    Layer2PoolStats stats[NVARPOOL]; /*^ M, 0, N, 0, 0 ^*/
}L2MemPoolStats;

/* This structure provides the MSG POOL stats */
typedef struct L2MsgPoolStatsT {
    /* This array is terminated when bufSize is 0 for an entry, other entries are not valid */
    Layer2PoolStats stats[NVARPOOL]; /*^ M, 0 ^*/
    /* the QPCTL stats */
    Layer2PoolStats qpctlStats; /*^ M, 0 ^*/
    /* the QMSG stats */
    Layer2PoolStats qmsgStats; /*^ M, 0 ^*/
}L2MsgPoolStats;

/* This is used in debug GET Command */
void L2PrepareMemPoolStats (L2MemPoolStats *csplMemPoolStats_p);
void L2PrepareMsgPoolStats (L2MsgPoolStats *csplMsgPoolStats_p);

typedef enum GetDebugTypeE
{
    GET_DEBUG_INFO_MEM_STATS, /* Mem Pool Statistics */
    GET_DEBUG_INFO_MSG_STATS  /* Message Pool Statistics */
}L2DebugType;

/*spr 22474 end*/
/* OPTION3X Changes Start */
#ifdef ENDC_ENABLED
typedef enum
{
    /*MCG_BEARER: when RLC/MAC/PDCP all are present at Master Node,
     *   * MeNB Only. MN TERMINATED MCG BEARER. This shall be used as default bearer*/
    MCG_BEARER = 0,
    /*SN_TERMINATED_SCG_NORMAL_BEARER: when RLC/MAC/PDCP all are present at Secondary Node,
     *   * SgNB Only. SN TERMINATED SCG BEARER*/
    SN_TERMINATED_SCG_NORMAL_BEARER = 1,
    /*SCG_SPLIT_BEARER: when PDCP is present in Secendory node only and
     *   * RLC/MAC are present at Master Node and Secendory node both,
     *     * SN TERMINATED SPLIT BEARER*/
    SN_TERMINATED_SCG_SPLIT_BEARER,
    INVALID_DC_BEARER = 255
}DCBearerType;
#endif
/* OPTION3X Changes End */

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
 
/****************************************************************************
 * Exported Functions
 ****************************************************************************/

#endif  /* INCLUDED_LTE_TYPES_H */
