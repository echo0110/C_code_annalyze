
/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2011 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  Id: lteMacPhyCellReconfig.h
 *
 ****************************************************************************
 *
 *  File Description : It contains macro declarations, function declarations 
 *                     sturucture declarations related to Cell reconfig 
 *             
 ***************************************************************************/
#ifndef LTE_MAC_PHY_CELL_RECONFIG_H
#define LTE_MAC_PHY_CELL_RECONFIG_H

#include "lteTypes.h"
#include "lteFapiMacPhyInterface.h"
#include "lteMacParseUtil.h"
/* + PRS_CHANGES */
#include "lteMacFAPIApi.h"
/* - PRS_CHANGES */
/* Enum Definitions */
typedef enum FAPI_PhyCellReCfg_enT
{
     FAPI_FALSE,
     FAPI_PENDING_RESPONSE,
     FAPI_TRUE
}FAPI_PhyCellReCfg_en;

/*Added for power Control*/
/*Structure for maintaining PDSCH re-configuration information*/
typedef struct PDSCHReConfigCommonInfoT
{
    /*Indicates target SFN */
    UInt16 targetSfn;
    /*Indicates target SF*/
    UInt8 targetSf;
    /*Indicates the downlink reference-signal transmit
     ** power used to derive downlink reference-signal EPRE*/
    SInt8 referenceSigPower;
    /*Indicates the ratio of PDSCH EPRE to cell-specific RS EPRE*/
    UInt8 pB;
    /*To check if it is updated or not*/
    UInt8 updateFlag;
}PDSCHReConfigCommonInfo;

/*Structure for storing the reconfiguration of common downlink 
 * power control information. This parameter is of TLV type*/ 
typedef struct ReconfigCommonDLPControlInfoPhyT
{
    /*Power per antenna of the PCFICH w.r.t reference signal*/
    UInt16 pcfichPowerOffset;
    /*The configured transmission power for PDCCH*/
    SInt8 updateFlagPcfich;
    /*Power per antenna of the PHICH w.r.t reference signal*/
    UInt16 phichPowerOffset;
    /*Configured transmission power for PBCH*/
    SInt8 updateFlagPhich;
}ReconfigCommonDLPControlInfoPhy;

/*Structure for storing the reconfiguration of common downlink power 
 * control information. This parameter is of TLV type*/
typedef struct ReconfigCommonDLPControlInfoMacT
{
    /*Indicates target SFN */
    UInt16 targetSfn;
    /*Indicates target SF*/
    UInt8 targetSf;
    /*Power per antenna of the PCFICH w.r.t reference signal*/
    UInt16 pcfichPowerOffset;
    /*Power per antenna of the PHICH w.r.t reference signal*/
    UInt16 phichPowerOffset;
    /*The configured transmission power for PDCCH*/
    UInt16 pdcchPowerOffset;
    /*Configured transmission power for PBCH*/
    UInt16 pbchTransmissionPower;
    /*Configured transmission power for PCH*/
    UInt16 pchTransmissionPower;
    /*To check if it is updated or not*/
    UInt8 updateFlag;
}ReconfigCommonDLPControlInfoMac;

/*Structure to store the reconfiguration of UL Power Control Info*/
typedef struct ReconfigCommonULPowerControlInfoT
{
    /*Indicates target SFN */
    UInt16 targetSfn;
    /*Indicates target SF*/
    UInt8 targetSf;
    /*Indicates changed P0 Nominal Pusch value */
    SInt8 p0NominalPuschValue;
    /*A 3-bit cell specific parameter for power control*/
    UInt8 alpha;
    /*Indicates whether its updated or not*/
    UInt8 updateFlag;
}ReconfigCommonULPowerControlInfo;

/*SPR 3600 Start*/
typedef struct ReconfigPuschConfigCommonT
{
    /*Indicates target SFN */
    UInt16 targetSfn;
    /*Indicates target SF*/
    UInt8 targetSf;
    /*Indicates whether its updated or not*/
    UInt8 updateFlag;
    /*Parameter to update enable64Qam*/
    UInt8 reconfigEnable64Qam;
}ReconfigPuschConfigCommon;
/*SPR 3600 End*/

/* Structure Definitions */
/*Modified for power control*/
typedef struct ReconfigPdschConfigCommonInfoT
{
    /*Contains the downlink reference-signal
     * transmit power used to derive downlink reference-signal
     * EPRE and the ratio of PDSCH EPRE to cell-specific RS EPRE
     * */
    SInt8 refSignalPower;
    /*Indicates Update of Ref Sig Power*/
    UInt8 updateFlag;/* Indicates Update of Ref Sig Power */
    /*EPRE and the ratio of PDSCH EPRE to cell-specific RS EPRE*/
    UInt8 pB;
}ReconfigPdschConfigCommonInfo;

/* +- SPR 19066 */

typedef struct contFreeRachTimeT
{
    UInt16 targetSfn;
    UInt8  targetSf;
    UInt8  contFreeRachTimer;
    UInt8 updateFlag;/* Indicates Update of contention Free Rach Timer */ 
}ContFreeRachTimeVal;

typedef struct SyncSignalT
{
    UInt16 primarySignal;
    UInt16 secondarySignal;
    UInt8 updatePrimarySignalFlag;
    UInt8 updateSecondarySignalFlag;
}SyncSignal;
typedef struct ReconfigPhyCellT
{
    UInt8  reconfigUpdate;
    UInt16 targetSfn;
    UInt8  targetSf;
    UInt16 transactionId;        
    /*Added for Power Control*/
    /*Contains the downlink reference-signal
     * transmit power used to derive downlink  reference-signal
     * EPRE and the ratio of PDSCH EPRE to cell-specific RS EPRE
     * */
    ReconfigPdschConfigCommonInfo reconfigPdschConfigCommonInfo;
    /*Contains power offsets for PCFICH, PHICH, PDCCH, PBCH and PCH*/
    ReconfigCommonDLPControlInfoPhy reconfigCommonDLPowerControlInfo;
    SyncSignal syncSignal;
    /* + PRS_CHANGES */
    UInt8 updatePrsFlag;
    PRSConfigPhy      prsPhyConfig;
    /* - PRS_CHANGES */
}ReconfigPhyCell;

typedef struct ReconfigMacCellT
{
    UInt8  reconfigUpdate;
/* GAP_VALUE_FIX_START */
    UInt8  gapValue;
/* GAP_VALUE_FIX_END */
    /* +- SPR 19066 */
    ContFreeRachTimeVal contFreeRachTimer;         
    /*Added for Power Control*/
    /*Contains changed the downlink reference-signal
     * transmit power used to derive downlink reference-signal
     * EPRE and the ratio of PDSCH EPRE to cell-specific RS EPRE
     * */
    PDSCHReConfigCommonInfo reconfigPdschConfigCommonInfo;
    /*Contains changed power offsets for PCFICH, PHICH, PDCCH, PBCH and PCH*/
    ReconfigCommonDLPControlInfoMac reconfigCommonDLPControlInfoMac;
    /*Updated value of CommonULPowerControlInfo*/
    ReconfigCommonULPowerControlInfo reconfigCommonULPowerControlInfo;
    /*SPR 3600 Start*/
    ReconfigPuschConfigCommon reconfigPuschConfigCommon;
    /*SPR 3600 End*/
    /* SPS_CHG_START */
    /* Reconfig Cell for SPS is not supported */
    /* SPS_CHG_END */
    /* + PRS_CHANGES */
    PRSParseConfigInfo prsParseConfigInfo;  
    /* - PRS_CHANGES */

}ReconfigMacCell;


/* Extern Declarations */
/* CA Changes start */
extern ReconfigPhyCell ReconfigPhyCell_g[MAX_NUM_CELL];
extern ReconfigMacCell ReconfigMacCell_g[MAX_NUM_CELL];
/* CA Changes end */
/*SPR 16855 +-*/
void   processCellReconfigUpdate( UInt32 delayToPhy , 
                    InternalCellIndex internalCellIndex);

#endif
