/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpLteDefs.h,v 1.1.1.1.6.2.4.2.2.1 2010/09/21 15:49:08 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : Global LTE stack definitions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: ltePdcpLteDefs.h,v $
 * Revision 1.1.1.1.6.2.4.2.2.1  2010/09/21 15:49:08  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.6.2.2.1  2010/08/10 12:11:44  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.1.1.1.6.2  2010/08/02 08:57:18  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.2.1  2009/06/09 11:33:04  gur19140
 * *** empty log message ***
 *
 * Revision 1.2  2009/05/21 07:13:43  gur19140
 * fixes
 *
 * Revision 1.1  2009/05/20 13:22:18  gur20548
 * Changed header file name
 *
 * Revision 1.2  2009/05/15 12:22:41  gur19836
 * CR Comments Incorporated
 *
 *
 *
 ****************************************************************************/

#ifndef _LIB_PDCP_LTE_DEF_
#define _LIB_PDCP_LTE_DEF_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <lteTypes.h>
#include "lteLayer2CommanTypes.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/* LTE stack Modules identifiers */
/* + SPR_17858_389_CHANGES */
/*Coverity Fix 10194*/
#define LTE_MAX_UE_ID   MAX_UE_SUPPORTED
   /* - SPR_17858_389_CHANGES */
/* Maximum value for lcId */
#define LTE_MAX_LC_ID   10

/* Maximum number of DRBs supported*/
#define PDCP_MAX_DRB    8

/* Security keys length in bytes */
#define LTE_SECURITY_KEY_LENGTH   16
/* Integrity protection algorithm identifiers 
   (AlgorithmId field)*/
#define ALGO_EIA0 0
#define ALGO_EIA1 1
#define ALGO_EIA2 2

/* Ciphering algorithm identifiers
   {AlgorithmId field) */
#define ALGO_EEA0 0
#define ALGO_EEA1 1
#define ALGO_EEA2 2

/* MAC-I field length in octets */
#define MACI_LEN    4
/* Fixed SPR 1303 START */
#define PDCP_MAX_HDR_SIZE 2
/* Fixed SPR 1303 END  */

/* Value of count reqd for MAC-I computation during HO */
#define MAX_COUNT 0xFFFFFFFF

/* Assign Invalid SN to packet for freshly arrived SDUs */
#define PDCP_INVALID_SN 0xFFFF

/* Byte Value */
#define NUM_BITS_IN_BYTES 8

#define KILOBYTES 1000

/* Sets bits in bitmap of status report */
#define SET_BIT_1 0x80
#define SET_BIT_2 0x40
#define SET_BIT_3 0x20
#define SET_BIT_4 0x10
#define SET_BIT_5 0x08
#define SET_BIT_6 0x04
#define SET_BIT_7 0x02
#define SET_BIT_8 0x01

/* Bitmap size */
#define BITMAP_SIZE 512

/* Maximum number of PDCP SDUs, PDCP can store at a time */
#define MAX_PDCP_SDU_QUEUE_SIZE 2048
/* SPR 2073 changes start */
#define LTE_MIN_CRNTI_VALUE   0x0001
#define LTE_MAX_CRNTI_VALUE   0xFFF3
/* SPR 2073 changes end */

/* SPR 3276 changes start */
#define LTE_MIN_DRB_ID_RANGE  1
#define LTE_MAX_DRB_ID_RANGE  32
/* SPR 3276 changes end */

#define MIN_INTER_PKT_DELAY_FOR_SILENCE_DETECTION 1
#define MAX_INTER_PKT_DELAY_FOR_SILENCE_DETECTION 10

/* Number of last RLC SDUs used for calculating average */
#define NUM_RLC_SDU_FOR_RUNNING_AVG 10
/* Maximum limit when Avg SDU size needs to be sent in UL */
#define MAX_NUMBER_RLC_SDU_LIMIT    10

/* SPR 3774 change start */
#define LTE_PDCP_FAST_PATH  1
#define LTE_PDCP_SLOW_PATH  0
/* SPR 3774 change end */

/****************************************************************************
 * Exported Types
 ****************************************************************************/
/* SPR 20913 Start */
#ifndef LTE_MAC_TYPES_H 
/* SPR 20913 End */
/*  TLV API Buffer Header definition. It should be present at the beginning of each  
    TLV API Buffer.
 */
typedef struct TlvHeaderT
{
    UInt16      transactionId;
    UInt16      sourceModuleId;
    UInt16      destinationModuleId;
    UInt16      apiId;
    UInt16      length;
/* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING    
    UInt8       cellIndex;
    UInt8       reserved;
/*SPR 21653 chages start*/
    UInt32      reserved2;
/*SPR 21653 chages end*/
#endif    
/* - Layer2 NON CA Changes */
} TlvHeader, *LP_TlvHeader;

/*  TLV API message definition. 
 */
typedef struct TlvMessageT
{
    TlvHeader  header;
    UInt8      data[];
} TlvMessage, *LP_TlvMessage;

#endif
/*  TLV API tag (optional parameter) header definition . 
 */
typedef struct TlvTagHeaderT
{
    UInt16              tagId;
    UInt16              length;
/* SPR 9601 changes start */
}__attribute__((packed)) TlvTagHeader, *LP_TlvTagHeader;
/* SPR 9601 changes end */

/*  TLV API tag (optional parameter) definition. 
 */
typedef struct TlvTagT
{
    TlvTagHeader      header;
    UInt8             data[];
} TlvTag, *LP_TlvTag;
#define LTE_MAX_DRB 8
typedef struct pdcpKpiThpStatsCA_T {
        UInt64 pdcpSduBitDl[MAX_UE_SUPPORTED][LTE_MAX_DRB];
        UInt64 pdcpSduBitUl[MAX_UE_SUPPORTED][LTE_MAX_DRB];
} pdcpKpiThpStatsCA;
/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern UInt16 pdcpNumOfSupportedUsers_g;
extern UInt16 pdcpNumOfSupportedLC_g;
extern UInt8  pdcpDlMemFac_g;
extern UInt8  pdcpUlMemFac_g;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

#endif  /* _LIB_PDCP_LTE_DEF_ */
