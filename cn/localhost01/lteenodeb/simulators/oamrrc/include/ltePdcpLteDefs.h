/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpLteDefs.h,v 1.1.4.2.4.2.2.1 2010/09/21 16:09:11 gur20491 Exp $
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
 * Revision 1.1.4.2.4.2.2.1  2010/09/21 16:09:11  gur20491
 * FAPI changes
 *
 * Revision 1.1.4.2  2010/08/02 08:57:18  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.1.1.1  2010/02/11 04:51:25  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1.2.3.2.2  2009/11/19 10:54:59  gur19479
 * updated for RRM Module
 *
 * Revision 1.1.2.3.2.1  2009/09/30 10:18:19  gur11318
 * Changes for oam-rrc mwc, api and module ids added.
 *
 * Revision 1.1.2.3  2009/07/12 10:14:26  gur19479
 * updated for traffic gen
 *
 * Revision 1.1.2.2  2009/07/07 06:26:45  gur19836
 * throughtput related changes
 *
 * Revision 1.1.2.1  2009/06/16 10:19:22  gur19836
 * added new header file require for oamrrc to make independent from pdcp
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
#include <lteLayer2CommanTypes.h>
/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define S1AP_MODULE_ID   3
/* RRC OAM API */
#define RRC_OAM_INIT_IND                 1
#define RRC_OAM_INIT_CNF                 2
#define RRC_OAM_COMMUNICATION_INFO_REQ   3
#define RRC_OAM_COMMUNICATION_INFO_RESP  4 
#define RRC_OAM_PROVISION_REQ            5
#define RRC_OAM_PROVISION_RESP           6
#define S1AP_OAM_INIT_IND                1281
#define S1AP_OAM_PROVISION_REQ           1283
#define S1AP_OAM_PROVISION_RESP          1284

#define MAC_CCCH_MSG_REQ        10 
/* macro added for L2 O SSI */
#define MAC_CCCH_MSG_IND        203 

#define PDCP_SRB_DATA_IND        3

/* Maximum value for ueIndex */
/* 128 UE changes start */
/* 255 UE changes start */
/* +- SPR 18268 */
#define LTE_MAX_UE_ID   265
/* +- SPR 18268 */
/* 255 UE changes end */
/* 128 UE changes end */

/* Maximum value for lcId */
#define LTE_MAX_LC_ID   10

/* The range of channels IDs supported for SRB */
#define LTE_SRB_LC_ID_LOW_BOUND     1
#define LTE_SRB_LC_ID_HIGH_BOUND    2

/* The range of channels IDs supported for DRB */
#define LTE_DRB_LC_ID_LOW_BOUND     3
#define LTE_DRB_LC_ID_HIGH_BOUND    10

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

/****************************************************************************
 * Exported Types
 ****************************************************************************/
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
} TlvHeader, *LP_TlvHeader;



/*  TLV API message definition. 
 */
typedef struct TlvMessageT
{
    TlvHeader  header;
    UInt8      data[];
} TlvMessage, *LP_TlvMessage;

/* client message*/
typedef struct clientHeader{
  UInt32 apiId;
}ClientHeader,*LP_ClientHeader;



typedef struct clientMessageT
{
      ClientHeader  header;
          UInt8      data[];
} ClientMessage, *LP_ClientMessage;

/* Throughput message struture*/
typedef struct throughputData
{
  UDouble32 ulThroughput;
  UDouble32 dlThroughput;

} ThroughputData,*LP_ThroughputData;

/*  TLV API tag (optional parameter) header definition . 
 */
typedef struct TlvTagHeaderT
{
    UInt16              tagId;
    UInt16              length;
} TlvTagHeader, *LP_TlvTagHeader;

/*  TLV API tag (optional parameter) definition. 
 */
typedef struct TlvTagT
{
    TlvTagHeader      header;
    UInt8             data[];
} TlvTag, *LP_TlvTag;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/


#endif  /* _LIB_PDCP_LTE_DEF_ */
