/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpStatistic.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : Statistic unit definition.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: ltePdcpStatistic.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.4.2.2  2009/07/13 10:25:16  gur20435
 *
 * PDCP Optimizations
 *
 * Revision 1.4.2.1  2009/06/09 11:33:04  gur19140
 * *** empty log message ***
 *
 * Revision 1.4  2009/05/28 15:28:43  gur20548
 * stats done
 *
 * Revision 1.3  2009/05/26 17:21:16  gur19479
 * modified for rohc stats
 *
 * Revision 1.2  2009/05/20 14:45:58  gur20548
 * header file name changed
 *
 * Revision 1.1  2009/05/20 13:22:18  gur20548
 * Changed header file name
 *
 * Revision 1.3  2009/05/15 12:22:41  gur19836
 * CR Comments Incorporated
 *
 *
 *
 ****************************************************************************/

#ifndef __PDCP_STATISTIC_DEF__
#define __PDCP_STATISTIC_DEF__

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <lteTypes.h>
#include "ltePdcpLteDefs.h"
#include "lteCommonStatsManager.h"
#include "ltePdcpOamInterface.h"

/****************************************************************************
 * PDCP SYSTEM STATS data structures
 ****************************************************************************/

#define LTE_SRB_LC_ID (LTE_SRB_LC_ID_HIGH_BOUND - LTE_SRB_LC_ID_LOW_BOUND + 1)
#define LTE_DRB_LC_ID (LTE_DRB_LC_ID_HIGH_BOUND - LTE_DRB_LC_ID_LOW_BOUND + 1)

/* Statistic items for Integrity protection feature */
typedef struct PdcpIntegrityStatsT
{
    UInt32 numberOfPacketsIntegrityProtectedUsingEIA0;
    /*^ M, 0 ^*/
    UInt32 numberOfPacketsIntegrityVerifiedUsingEIA0;
    /*^ M, 0 ^*/
    UInt32 numberOfPacketsIntegrityProtectedUsingEIA1;
    /*^ M, 0 ^*/
    UInt32 numberOfPacketsIntegrityVerifiedUsingEIA1;
    /*^ M, 0 ^*/
    UInt32 numberOfPacketsIntegrityProtectedUsingEIA2;
    /*^ M, 0 ^*/
    UInt32 numberOfPacketsIntegrityVerifiedUsingEIA2;
    /*^ M, 0 ^*/
    UInt32 numberOfPacketsRejectedByIntegrityVerification;
    /*^ M, 0 ^*/
}PdcpIntegrityStats;

/* Statistic items for Ciphering feature */
typedef struct PdcpCipheringStatsT
{
    UInt32 numberOfPacketsCipheredUsingEIA0;
    /*^ M, 0 ^*/
    UInt32 numberOfPacketsDecipheredUsingEIA0;
    /*^ M, 0 ^*/
    UInt32 numberOfPacketsCipheredUsingEIA1;
    /*^ M, 0 ^*/
    UInt32 numberOfPacketsDecipheredUsingEIA1;
    /*^ M, 0 ^*/
    UInt32 numberOfPacketsCipheredUsingEIA2;
    /*^ M, 0 ^*/
    UInt32 numberOfPacketsDecipheredUsingEIA2;
    /*^ M, 0 ^*/
}PdcpCipheringStats;

/* Statistic items for Ciphering feature */
typedef struct PdcpRohcStatsT
{
    UInt32 numberOfIRPacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfIRPacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfIRDYNPacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfIRDYNPacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfACKProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfACKCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfNACKProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfNACKCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfSNACKProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfSNACKCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfR0PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfR0CRCPacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfUO0PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfR1PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfR1IDPacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfR1TSPacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfUO1PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfUO1IDPacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfUO1TSPacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfUOR2PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfUOR2IDPacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfUOR2TSPacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfR0PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfR0CRCPacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfUO0PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfR1PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfR1IDPacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfR1TSPacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfUO1PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfUO1IDPacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfUO1TSPacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfUOR2PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfUOR2IDPacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfUOR2TSPacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfModeTransU2OInit;
    /*^ M, 0 ^*/
    UInt32 numberOfModeTransU2RInit;
    /*^ M, 0 ^*/
    UInt32 numberOfModeTransO2RInit;
    /*^ M, 0 ^*/
    UInt32 numberOfModeTransO2UInit;
    /*^ M, 0 ^*/
    UInt32 numberOfModeTransR2UInit;
    /*^ M, 0 ^*/
    UInt32 numberOfModeTransR2OInit;
    /*^ M, 0 ^*/
    UInt32 numberOfTimesCrcFailed;
    /*^ M, 0 ^*/
    UInt32 numberOfModeTransU2ORcvd;
    /*^ M, 0 ^*/
    UInt32 numberOfModeTransU2RRcvd;
    /*^ M, 0 ^*/
    UInt32 numberOfModeTransO2RRcvd;
    /*^ M, 0 ^*/
    UInt32 numberOfModeTransO2URcvd;
    /*^ M, 0 ^*/
    UInt32 numberOfModeTransR2URcvd;
    /*^ M, 0 ^*/
    UInt32 numberOfModeTransR2ORcvd;
    /*^ M, 0 ^*/
    UInt32 numberOfP0NormalPacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfP0NormalPacketsProcessed;
    /*^ M, 0 ^*/
    /* Profile 6 packet types stats */
    UInt32 numberOfIRCRP6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfIRCRP6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfCOP6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfCOP6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfRND1P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfRND1P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfRND2P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfRND2P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfRND3P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfRND3P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfRND4P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfRND4P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfRND5P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfRND5P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfRND6P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfRND6P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfRND7P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfRND7P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfRND8P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfRND8P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ1P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ1P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ2P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ2P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ3P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ3P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ4P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ4P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ5P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ5P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ6P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ6P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ7P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ7P6PacketsProcessed;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ8P6PacketsCreated;
    /*^ M, 0 ^*/
    UInt32 numberOfSEQ8P6PacketsProcessed;
    /*^ M, 0 ^*/
}PdcpRohcStats;

/* UE Stats data structues */
/* Group of Statistic items for SRB entity*/
typedef struct StatsSrbDataT
{
    /*SPR 3189 changes start */
    UInt8 srbId ;
    /*^ M, 0 ^*/
    /*SPR 3189 changes end */
    /* Downlink stats */
    UInt32 numberOfDlSrbSduReceived;
    /*^ M, 0 ^*/
    UInt32 numberOfDlSrbPduTransmitted;
    /*^ M, 0 ^*/
    UInt32 numberOfSrbDeliveryIndSent;
    /*^ M, 0 ^*/
    UInt32 numberOfDlSrbPduDropNackRecvd;
    /*^ M, 0 ^*/
    UInt32 numberOfDlSrbSduDiscarded;
    /*^ M, 0 ^*/
    /* Uplink stats */
    UInt32 numberOfSrbUlPduReceived;
    /*^ M, 0 ^*/
    UInt32 numberOfSrbUlSduTransmitted;
    /*^ M, 0 ^*/
    UInt32 numberOfUlSrbSduDropped;
    /*^ M, 0 ^*/
} StatsSrbData, *LP_StatsSrbData;
//} StatsSrbData;

/* Group of Statistic items for DRB entity*/
typedef struct StatsDrbDataT
{
    /*SPR 3189 changes start */
    UInt8 lcId ;
    /*^ M, 0 ^*/
    /*SPR 3189 changes end */
    /* Downlink stats */
    UInt32 numberOfDlDrbSduReceived;
    /*^ M, 0 ^*/
    UInt32 numberOfDlDrbPduTransmitted;
    /*^ M, 0 ^*/
    UInt32 numberOfDlDrbPduDropForNack;
    /*^ M, 0 ^*/
    UInt32 numberOfDlDrbPduDropForRoHCFailure;
    /*^ M, 0 ^*/
    UInt32 numberOfDrbDeliveryIndSent;
    /*^ M, 0 ^*/
    UInt32 numberOfDlDrbSduDiscarded;
    /*^ M, 0 ^*/
    /* Uplink stats */
    UInt32 numberOfDrbUlSduReceived;
    /*^ M, 0 ^*/
    UInt32 numberOfDrbUlPduTransmitted;
    /*^ M, 0 ^*/
    UInt32 numberOfDrbUlPduDropDueToRoHcFailure;
    /*^ M, 0 ^*/
    UInt32 numberOfDrbUlPduDropAsSizeOORange;
    /*^ M, 0 ^*/
    /*SPR 3811 changes start */
    UInt32 numberOfDrbUlPduDropAsReservedBitSet;
    /*^ M, 0 ^*/
    /*SPR 3811 changes end */
} StatsDrbData, *LP_StatsDrbData;

/* UE entity Statistic item */
typedef struct StatsUeT
{

    UInt16 crnti;
    /*^ M, 0 ^*/
    UInt16 statNumActiveSrbLc ; /*^ M, 0, READ ^*/
    
    UInt16 statNumActiveDrbLc ; /*^ M, 0, READ ^*/
   
    /*StatsSrbData  srbs[LTE_SRB_LC_ID_HIGH_BOUND - LTE_SRB_LC_ID_LOW_BOUND + 1];*/
    StatsSrbData  srbs[LTE_SRB_LC_ID];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, statNumActiveSrbLc ^*/
    /*StatsDrbData  drbs[LTE_DRB_LC_ID_HIGH_BOUND - LTE_DRB_LC_ID_LOW_BOUND + 1];*/
    StatsDrbData  drbs[LTE_DRB_LC_ID];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, statNumActiveDrbLc ^*/
}__attribute__ ((packed)) StatsUe, *LP_StatsUe;

typedef struct PdcpStatsT {
    PdcpIntegrityStats statsIntegrity;  /*^ M, 0 ^*/ 
    PdcpCipheringStats statsCiphering;  /*^ M, 0 ^*/ 
    PdcpRohcStats statsRohc;  /*^ M, 0 ^*/ 
    UInt16 statNumActiveUe ;  /*^ M, 0 ^*/ 
    /*CSR 40799 chg start*/ 
    StatsUe statsUEs[MAX_UE_SUPPORTED]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/ 
    /*CSR 40799 chg end*/
}__attribute__ ((packed)) PdcpStats; 

/**************************************************************************
 * PDCP GET STATS CNF response message Structure
 **************************************************************************/
#define PDCP_VAR_SIZE(x) 1
#define NUM_ACTIVE_UE 1
typedef struct PdcpStatsCnfT{
    UInt16 responseCode;
    /*^ M, 0 ^*/
    PdcpIntegrityStats statsIntegrity;
    /*^ M, 0 ^*/
    PdcpCipheringStats statsCiphering;
    /*^ M, 0 ^*/
    PdcpRohcStats statsRohc;
    /*^ M, 0 ^*/
    UInt16 statNumActiveUe ;
    /*^ M, 0 ^*/
    UInt16  startUeId;
    /*^ M, 0 ^*/
    UInt8  numOfUe;
    /*^ M, 0 ^*/
    UInt8  UesLeft;
    /*^ M, 0 ^*/
    StatsUe statsUEs_p[PDCP_VAR_SIZE(statNumActiveUe)];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}__attribute__ ((packed)) PdcpStatsCnf, *LP_PdcpStatsCnf;/*^ API, PDCP_GET_STATS_RESP ^*/

/**************************************************************************
 * PDCP UE STATUS data structures
 **************************************************************************/

typedef struct StatusSrbEntityT {
    /*SPR 3189 changes start */
    UInt8 srbId ;
    /*^ M, 0 ^*/
    /*SPR 3189 changes end */
    UInt16 rlcMode;
    /*^ M, 0 ^*/
    UInt16 direction;
    /*^ M, 0 ^*/
}StatusSrbEntity;

typedef struct StatusDrbEntityT {
    /*SPR 3189 changes start */
    UInt8 lcId ;
    /*^ M, 0 ^*/
    /*SPR 3189 changes end */
    UInt16 rlcMode;
    /*^ M, 0 ^*/
    UInt16 direction;
    /*^ M, 0 ^*/
    UInt16 discardTimerValue;
    /*^ M, 0 ^*/
    UInt16 statusReprtField;
    /*^ M, 0 ^*/
    UInt16 snFieldLength;
    /*^ M, 0 ^*/
    UInt16 rohcProfileUsed;
    /*^ M, 0 ^*/
}StatusDrbEntity;

typedef struct PdcpStatusT {
    UInt16 ueIndex ;
    /*^ M, 0 ^*/
    UInt16 crnti;
    /*^ M, 0 ^*/
    UInt16 integrityAlgoUsed;
    /*^ M, 0 ^*/
    UInt16 cipheringSrbAlgoUsed;
    /*^ M, 0 ^*/
    UInt16 cipheringDrbAlgoUsed;
    /*^ M, 0 ^*/
    UInt16 numActiveSrbEntities;
    /*^ M, 0, READ ^*/
    UInt16 numActiveDrbEntities;
    /*^ M, 0, READ ^*/
    StatusSrbEntity srbs[LTE_SRB_LC_ID];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numActiveSrbEntities ^*/
    StatusDrbEntity drbs[LTE_DRB_LC_ID];
    /*^ M, 0, OCTET_STRING, VARIABLE_COUNTER, numActiveDrbEntities ^*/
} PdcpStatus ;

/**************************************************************************
 * PDCP GET STATUS REQ/CNF messages Structures
***************************************************************************/
/* PDCP UE STATUS REQUEST message */
typedef struct PdcpStatusReqT {
    UInt16 ueStartRange ;
    /*^ M, 0 ^*/
    UInt16 ueEndRange ;
    /*^ M, 0 ^*/
}PdcpStatusReq, *LP_PdcpStatusReq;	/*^ API, PDCP_GET_STATUS_REQ ^*/


/* PDCP UE STATUS RESPONSE message */
typedef struct PdcpStatusCnfT{
    UInt16 responseCode;
    /*^ M, 0 ^*/
    UInt16 numActiveUE;
    /*^ M, 0 ^*/
    PdcpStatus data_p[PDCP_VAR_SIZE(numActiveUE)];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}PdcpUeStatusCnf, *LP_PdcpUeStatusCnf;	/*^ API, PDCP_GET_STATUS_RESP ^*/

/* SPR 15692 fix start */
/* Following index used for number of KPI values reported */
#define MAX_KPI_INDEX 80

/* Structure definition for API : PDCP_KPI_THP_STATS_IND
 * Structure name               :PdcpKpiThpStatsIndParams */
typedef struct PdcpKpiThpStatsIndParamsT 
{
    UInt32 avgDurationT;  /*^ M, 0 ^*/
    UInt16 ueIndex[ MAX_KPI_INDEX ]; /*^ M, 0, OCTET_STRING, FIXED^*/
    UInt8  qci[ MAX_KPI_INDEX ]; /*^ M, 0, OCTET_STRING, FIXED^*/
/*spr 19711 fix start*/
	/*code deleted*/
/*spr 19711 fix end*/
/*spr 19296 fix start*/
	/*code deleted*/
/*spr 19296 fix end*/
    UInt64 pdcpSduBitDl[MAX_KPI_INDEX]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    UInt64 pdcpSduBitUl[MAX_KPI_INDEX]; /*^ M, 0, OCTET_STRING, FIXED ^*/
    /* + SPR 10577 */
    UInt8 endIndicator; /*^ M, 0 ^*/
    /* padding is added for byte alignment,this is reserved and can 
     * be used for new fields in future.Its value should be ignored */
    UInt8 padding[3]; 
    /* - SPR 10577 */
} PdcpKpiThpStatsIndParams, *LP_PdcpKpiThpStatsIndParams; /*^ API, PDCP_KPI_THP_STATS_IND ^*/
/* SPR 15692 fix end */

extern PdcpStats pdcpStats;

/****************************************************************************
 * Function Name  : statsReset
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Resets all statistic data to initial values
 ****************************************************************************/
void    statsReset( void );

/****************************************************************************
 * Function Name  : pdcpInitStats
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Resets all statistic data to initial values
 ****************************************************************************/
void    pdcpInitStats ( void );

/* SPR 15692 fix start */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
/****************************************************************************
 * Function Name  : pdcpUpdateKPIBeforeLcDelete
 * Inputs         : ueIndex, lcId, qci 
 * Outputs        : None
 * Returns        : None 
 * Description    : This function will update KPIs when before deleting RAB 
 *                  of the UE. This function will be called at the time of 
 *                  cleanup of TX entity. RX entity will be deleted from there
 *                  itself without processing any new packet in this duration
 *                  since PDCP runs as a single thread.
 ****************************************************************************/
void pdcpUpdateKPIBeforeLcDelete( UInt16 ueIndex, UInt8 lcId, 
        UInt8 qci );

/****************************************************************************
 * Function Name  : pdcpNumQciEntries
 * Inputs         : moduleId 
 * Outputs        : None
 * Returns        : count - number of entries 
 * Description    : This function will return the number of entries to update
 *                  the KPI stats information for deleted RABs.
 ****************************************************************************/
UInt8 pdcpNumQciEntries( UInt8 moduleId );

/****************************************************************************
 * Function Name  : pdcpUpdateKpiThpContainerForDeleteUEs
 * Inputs         : passiveIndexForKpiThpStats - passive index of stats
 *                  moduleId - module Id
 * Outputs        : None 
 * Returns        : None
 * Description    : This function will update container for deleted UEs. 
 ****************************************************************************/
void pdcpUpdateKpiThpContainerForDeleteUEs(UInt32 passiveIndexForKpiThpStats, 
        UInt32 moduleId,InternalCellIndex internalCellIndex );

/****************************************************************************
 * Function Name  : pdcpFillStatsOfDeletedLc
 * Inputs         : moduleThpId, cnfBuff
 * Outputs        : idx_p - index till which entries for Deleted UE is filled 
 * Returns        : None
 * Description    : This function will update container for deleted RABs. 
 ****************************************************************************/
void pdcpFillStatsOfDeletedLc( UInt8 moduleThpId,
        LP_PdcpKpiThpStatsIndParams cnfBuff, UInt8 *idx_p );

/* + SPR 10577 */
/****************************************************************************
 * Function Name  : pdcpUpdateKpiThpStatsForLc
 * Inputs         : ueId - UE Identifier
 *                  lcId - LC identifier
 *                  entry - entry in response buffer
 *                  qci - QCI value for this LC
 *                  pLtePdcpKPIThpStats - shared memory container
 * Outputs        : cnfBuff - Pointer to confirmation buffer
 * Returns        : None.
 * Description    : This function fills the Throughput stats.
 ****************************************************************************/
void pdcpUpdateKpiThpStatsForLc( UInt16 ueId, UInt8 lcId, UInt8 entry,
        UInt8 qci, LtePdcpKPIThpStats *pLtePdcpKPIThpStats,
        LP_PdcpKpiThpStatsIndParams cnfBuff );
/* - SPR 10577 */
#endif
/* SPR 15692 fix end */

#endif /*__PDCP_STATISTIC_DEF__*/
