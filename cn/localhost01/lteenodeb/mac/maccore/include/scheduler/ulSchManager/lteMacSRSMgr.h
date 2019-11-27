/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacSRSMgr.h,v $
 *
 ****************************************************************************
 *
 *  File Description : This file contains all the data type information that are
 *  used by the SRS manager
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacSRSMgr.h,v $
****************************************************************************/
#ifndef LTE_MAC_SRS_MGR_H
#define LTE_MAC_SRS_MGR_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
#define SRS_MGR                                     "SRS_MGR"
#define MAX_SRS_PERIODICITY                         320
#define NUM_SF_OFFSETS_FOR_SRS_UE_CONFIG_2_MS       2 
#define MAX_TRANS_OFFSET_FOR_SRS_CELL_CONFIG        8
#define MAX_SRS_CELL_BANDWIDTH_CONFIG               8
#define MAX_SRS_DEDICATED_BANDWIDTH_CONFIG          4
#define NUM_OF_ROW_IN_SRS_CONFIG_TABLE_NOT_2MS      8
/*FDD_SRS*/
#define NUM_OF_ROW_IN_FDD_SRS_CONFIG_TABLE          9
/*FDD_SRS*/
#define RRC_UE_DELAY_FACTOR                         2
#define MAX_SRS_UE_BANDWIDTH_INDEX                  4

/*SPR 7086 Fix Start*/
#define SRS_ULSCH_OVERLAP                           1
#define SRS_ULSCH_NO_OVERLAP                        0
#define NUM_OF_SUBCARRIER_NORMAL                    12
#define NUM_OF_SUBCARRIER_EXTENDED                  14
/*SPR 7086 Fix End*/
/****************************************************************************
 * Exported Variables
 ****************************************************************************/
 /*SRS config types*/
typedef enum SRSDedicatedConfigTypeT
{
    SRS_DEDICATED_CONFIG_TYPE_RELEASE = 0,
    SRS_DEDICATED_CONFIG_TYPE_SETUP,
    /*SPR 6497 Fix Start*/
    SRS_DEDICATED_CONFIG_TYPE_SETUP_WAITING
    /*SPR 6497 Fix End*/
}SRSDedicatedConfigType; 
 
typedef enum SRSCommonConfigTypeT
{
    SRS_COMMON_CONFIG_TYPE_RELEASE = 0,
    SRS_COMMON_CONFIG_TYPE_SETUP
}SRSCommonConfigType; 

/* This strucure contains parameters that are required to be stored in the 
   SRS cell config table
   As per spec 36.211 table 5.5.3.3-2 */
typedef struct CellConfigTableEntryT
{
    /*Cell config configuration period*/
    UInt8       configPeriod;
    /*Number of transmission offsets*/
    UInt8       numTransOffset;
    /*Valid transmission offsets array*/
    UInt8       transOffsetArray[MAX_TRANS_OFFSET_FOR_SRS_CELL_CONFIG];
}CellConfigTableEntry;

#ifdef TDD_CONFIG
/* This structure contains subframe offsets corresponding to srs config 
   index used as direct indexing in SRSUEConfigTableFor2ms. This is
   used for 2ms periodicity*/
typedef struct SRSUEConfigTableEntryFor2msT
{
    UInt8       subFrameOffsetArray[NUM_SF_OFFSETS_FOR_SRS_UE_CONFIG_2_MS];
}SRSUEConfigTableEntryFor2ms;
#endif

/* This structure contains parameters required to calcultae srs periodicity and
 * subframe offset in case of periodicity > 2ms 
 As per table 8.2-2 of 36.213 */
typedef struct SRSUEConfigTableEntryForNot2msT
{
    /*Start config index*/
    UInt16      startIndex;
    /*End config index*/
    UInt16      endIndex;
    /*SRS transmission periodicity*/
    UInt16      periodicity;
    /*Start subframe offset*/
    SInt16      subFrameOffset;
}SRSUEConfigTableEntryForNot2ms;

/*SPR 7086 Fix Start*/
/* This structure contains parameters for uplink SRS bandwidth in terms of 
 * number of RBs allocated*/
typedef struct SRSULBandwidthTableEntryT
{
    UInt8       mSRS;
    UInt8       Nb;
}SRSULBandwidthTableEntry;
/*SPR 7086 Fix End*/

/* This structure contains all the parameters that are stored in the SRS Report
 * Node of a UE */
typedef struct SRSReportNodeT
{
    LTE_LIST_NODE           nodeAnchor;
    UInt16                  ueIndex;   
    UInt16                  nextUESfMapOffset;
    /*Pointer to next node (in the next valid sf offset)
    of the UE for SRS transmission*/
    struct SRSReportNodeT   *nextNode_p;
}SRSReportNode;

/* This structure shall have the UEs SRS Subframe Map List */
typedef struct SRSUESubFrameMapListT
{
    LTE_LIST srsSubFrameList;
}SRSUESubFrameMapList;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/*CA Changes start  */
extern  UInt8* srsCellConfigMap_gp[MAX_NUM_CELL];
/*CA Changes end  */
/****************************************************************************
 * Functions declaration
 ****************************************************************************/
/* HD FDD Changes */
#ifdef HD_FDD_CONFIG
void updateHDFddSemiStaticMapSRS(
    /*SPR 16855 Fix Start*/
    UInt16 ueIndex,
    /*SPR 16855 Fix End*/
    UInt16 srsPeriodicity, /* SRS peridicity */
    /* +- SPR 17777 */
    UInt8 oneShot, /* Whether infinite (non-zero) or one shot (0) SRS from UE */
    UInt8 setupRel); /* Whether SRS is setup or released */
#endif

void initSRSMgr(UInt8 srsConfigType, InternalCellIndex internalCellIndex);
void cleanupSRSMgr(InternalCellIndex internalCellIndex);
void deInitSRSMgr(InternalCellIndex internalCellIndex);

#endif /* LTE_MAC_SRS_MGR_H */
