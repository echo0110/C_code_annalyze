/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2PdcpGtpuInterface.h,v 2017/01/16 15:45:45 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *  This file describe the APIs between PDCP and GTPU/PR.
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 ****************************************************************************/
#ifndef PDCPGTPUINTERFACE_H 
#define PDCPGTPUINTERFACE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteTypes.h"
#include "lteLayer2CommanTypes.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

#if  defined PDCP_GTPU_INTF
/* Following macros are for APIs from Packet Relay to PDCP */
#define PDCP_PR_DATA_REQ                   1
#define PDCP_PR_SOURCE_DATA_FWD_REQ        2
#define PDCP_PR_INTRA_SOURCE_DATA_FWD_REQ  3
#define PDCP_PR_TARGET_DL_DATA_RELOC_REQ   4
#define PDCP_PR_TARGET_UL_DATA_RELOC_REQ   5
#define PDCP_PR_END_MARKER_IND             6
/* Following macros are for APIs from PDCP to Packet Relay */
#define PDCP_PR_UE_DATA_IND                7
#define PDCP_PR_SOURCE_DL_DATA_RELOC_REQ   8
#define PDCP_PR_SOURCE_UL_DATA_RELOC_REQ   9
#define PDCP_PR_LOCAL_EMI                 10
/*SPR 21366 +*/
#define GTP_REGISTER_PDCP_IND             11  
/* Maximum PDCP instances supported */
#define MAX_INSTANCE_ID    3
/*SPR 21366 -*/

/* Following structure is to send/receive the data packet to/from Packet Relay
 * / PDCP */
typedef struct PdcpUeDataT
{
    UInt16  size;
    UInt16  ueIndex; 
    UInt8   lcId;  
} PdcpUeData, *LP_PdcpUeData;

/* Following structure is to send/receive the data forwarding request during
 * Intra-eNB HO to/from Packet Relay / PDCP */
typedef struct PdcpIntraDataFwdT
{
    UInt16  oldUeIndex;
    UInt16  newUeIndex;
} PdcpIntraDataFwd, *LP_PdcpIntraDataFwd;

/* Following structure is to send/receive the forwarded packets to/from Packet
 * Relay / PDCP. This structure will be same for source and target */
typedef struct PdcpDataRelocT
{
    UInt16  sn;
    PdcpUeData ueData;
} PdcpDataReloc, *LP_PdcpDataReloc;

/* Following structure is to send/receive EMI to/from PDCP/Packet Relay */
typedef struct PdcpEmiT
{
    UInt16 ueIndex;
    UInt8  lcId;
} PdcpEmi , *LP_PdcpEmi;

/* Following structure is to send/receive local EMI to/from Packet Relay/PDCP */
typedef struct PdcpLocalEmiT
{
    PdcpEmi emi;
    UInt8 isEmiRecvd;
} PdcpLocalEmi , *LP_PdcpLocalEmi;
/*SPR 21366 +*/
/* Following structure registers sockets between PDCP and GTPu */
typedef struct PdcpGtpuRegistrationT
{
    UInt8 instanceId;
    UInt8 cellCount;
    RrcCellIndex cellIdList[MAX_NUM_CELL];
} PdcpGtpuRegistration, *LP_PdcpGtpuRegistration;
/*SPR 21366 -*/
#endif

#endif
