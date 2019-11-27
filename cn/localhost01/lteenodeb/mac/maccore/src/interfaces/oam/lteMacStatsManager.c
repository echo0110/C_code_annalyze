/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteMacStatsManager.c
 *
 ****************************************************************************
 *
 *  File Description : This file contains the functionality that is invoked 
 *                     from execution scheduler to obtain downlink and uplink
 *                     stats in the file.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *$Log: lteMacStatsManager.c,v $
 *Revision 1.1.1.1.6.1.6.3  2010/10/25 08:36:31  gur23971
 *stats related changes
 *
 *Revision 1.1.1.1.6.1.6.2  2010/10/22 12:44:10  gur04640
 *Statistics code merged from branch BRFramework_RRC1_0_RLC_AM_MAC_8_8_FAPI_optimizations
 *
 *Revision 1.1.1.1.6.1.6.1  2010/10/11 12:56:56  gur10121
 *SPR #706 fix
 *Revision 1.1.1.1.6.1.8.3  2010/10/22 06:12:03  gur04640
 *FAPI and per TTI stats implemented
 *
 *Revision 1.1.1.1.6.1.10.1  2010/10/08 07:42:39  gur10121
 * SPR 706 Fix - KlockWork Warnings removed
 *Revision 1.1.1.1.6.1.8.2  2010/10/20 10:24:38  gur04640
 *FAPI specific PHY interface stats added
 *
 *Revision 1.1.1.1.6.1.8.1  2010/10/13 10:59:37  gur04640
 *Changes done for implementing Stats using shared memory
 *
 *Revision 1.1.1.1.6.1.8.1  2010/10/12 02:45:30  gur04640
 *Changes done for implementing Stats using shared memory
 *
 *Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 *FrameworkSetupWith1.0_MAC1.2.4
 *
 *Revision 1.24  2009/11/15 12:57:25  gur11912
 *LTE_MAC_UT_LOG(LOG_WARNING) used instead of lteWarning
 *
 *Revision 1.23  2009/11/11 13:02:49  gur19413
 *file name changed -> DownlinkstatsInfo to downlinkstatsInfo
 *
 *Revision 1.22  2009/11/02 05:33:40  gur11912
 *updated for RA Policy
 *
 *Revision 1.21  2009/11/01 11:42:19  gur11912
 *RBBitMap printed in Hexadecimal format
 *
 *Revision 1.20  2009/10/25 11:10:46  gur11912
 *DL and UL stats functionality separated
 *
 *Revision 1.19  2009/10/21 05:58:19  gur19413
 *optimized stats functionality
 *
 *Revision 1.18  2009/09/03 14:27:44  gur19413
 *bug fixed
 *
 *Revision 1.17  2009/09/02 19:24:55  gur19413
 *bug fixed
 *
 *Revision 1.16  2009/09/02 13:49:46  gur19413
 *bug fixed
 *
 *Revision 1.15  2009/09/02 13:32:07  gur19413
 *bug fixed for customized file name
 *
 *Revision 1.14  2009/08/28 13:33:10  gur19413
 *bug fixed
 *
 *Revision 1.13  2009/08/28 13:29:38  gur19413
 *replace older logger system to new one
 *
 *Revision 1.12  2009/08/21 10:48:06  gur19413
 *bug fixed
 *
 *Revision 1.11  2009/08/20 06:42:55  gur19413
 *added stat functionality for PRACH and PUCCH Reports
 *
 *Revision 1.10  2009/08/03 10:38:24  gur11083
 *changed for multiple UE stats for UL/DL
 *
 *
 ****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/



/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteMacCellConfMgr.h"
#include "lteMacComPorting.h"
#include "lteMacStatsManager.h"
#include "lteCommonStatsManager.h"
#include "lteFapiMacPhyInterface.h"
#include "lteMacDLSchedulerStrategy.h"
#include "lteMacULSchedulerStrategy.h"
#include "lteMacFAPIApi.h"
/* SPR 13888 fix start */
#ifdef TDD_CONFIG
#include "lteMacTddMgr.h"
#endif
/* SPR 13888 fix end */

/****************************************************************************
 Private Definitions
 ****************************************************************************/
/* Since max size of DL CONFIG msg is 65535, and the minimum size of one pdu
 * is 12, so the maximumnumber of PDUs possible in one request are
 * 65536/12 = 5462 */
#define LTE_MAC_STATS_MAX_PDU   5462

#define LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex) gMacStats.pStats->\
            phyStats[(internalCellIndex)].radioFrameStats \
            .ttiStats[lLteMacStatsRfMulFactor_g*MAX_SFN_VALUE + \
            lLteMacStatsRfSfn_g[internalCellIndex] % LTE_MAC_STATS_MAX_SFN][lLteMacStatsRfSf_g[internalCellIndex]]

/****************************************************************************
 Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/*SPR 11812 Fix Start*/
#ifdef TDD_CONFIG
extern UInt8 getDlAckNackOffset( UInt8 subframeNum,InternalCellIndex internalCellIndex);
#endif
/*SPR 11812 Fix End*/
/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

static UInt8 lLteMacPduIdxToPduType[MAX_NUM_CELL][LTE_MAC_STATS_MAX_PDU] = {{0}};

/* FAPI2.4 Changes Start */
extern void (*lteMacUpdateFapiHarqIndStatsFunction )(void *msg_p,
        InternalCellIndex internalCellIndex);
extern void (*lteMacUpdateFapiRxUlschStatsFunction)(UInt8 *pBufferToSend,
        InternalCellIndex internalCellIndex);
extern void (*lteMacUpdateFapiRachIndStatsFunction)(UInt8 *pBufferToSend,
        InternalCellIndex internalCellIndex);
extern void (*lteMacUpdateFapiCqiIndStatsFunction)(UInt8 *pBufferToSend,
        InternalCellIndex internalCellIndex);
/* FAPI2.4 Changes Start */
/* 32.425 Events code changes start*/
#ifdef KPI_STATS
/* + PERF_CA GBL */
LTE_CIRCQUEUE kpiStatsUlIpThpQueue_g[MAX_NUM_CELL];
/* - PERF_CA GBL */
ScheduledIPThroughputULT scheduledIPThroughputUL_g[MAX_UE_SUPPORTED][MAX_LOGICAL_CHANNEL - 3];
/* +- SPR 17404 */
void updateIPLatencyKpiStats(UInt8 commonIndex, UInt16 ueIndex, UInt8 lcId);
#endif
/* 32.425 Events code changes end*/


/****************************************************************************
 * Function Name  : initStatsManager 
 * Inputs         : None 
 * Outputs        : None
 * Returns        : None
 * Description    : This function initialises global variables needed to push 
 *                  pop the nodes into the global queue when global stats
 *                  array is filled and the stats info when dumped into the
 *                  file respectively with respect to stats manager.
 ****************************************************************************/
/* + SPR 17439 */
void initStatsManager(void)
/* - SPR 17439 */    
{
#ifdef KPI_STATS
	/* + PERF_CA GBL */
    QINIT_KPI_STATS_UL_IP_THP_Q();
	/* - PERF_CA GBL */
    /* +- SPR 17404 */
#endif
}


/****************************************************************************
 * Function Name  : lteMacUpdateFapiParReqStats
 * Inputs         : pBufferToSend - buffer being sent to PHY interface
 *                  bufferlength - length pf buffer in bytes.
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function updates stats for FAPI PARAM REQUEST
 *                  message.
 ****************************************************************************/
/* CA Stats Changes Start */
/* +- SPR 17777*/
static  void lteMacUpdateFapiParReqStats(InternalCellIndex internalCellIndex)
/* +- SPR 17777*/
{
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        parReqStats.numMsgs++;
}

/****************************************************************************
 * Function Name  : lteMacUpdateFapiCellConfReqStats
 * Inputs         : pBufferToSend - buffer being sent to PHY interface
 *                  bufferlength - length pf buffer in bytes.
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function updates stats for FAPI CELL CONFIG REQUEST
 *                  message.
 ****************************************************************************/
static  void lteMacUpdateFapiCellConfReqStats(UInt8 *pBufferToSend,
        /* +- SPR 17777*/
        InternalCellIndex internalCellIndex)
{
    FAPI_phyCellConfigRequest_st    *pCellConfReq =
        (FAPI_phyCellConfigRequest_st *)
        ((FAPI_l1ApiMsg_st *)pBufferToSend)->msgBody;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        cellConfReqStats.numMsgs++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |= 
        LTE_MAC_STATS_RF_TX_CELL_CONFIG_REQ;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        cellConfReqStats.numTlvs +=
        pCellConfReq->numOfTlv;
}

/****************************************************************************
 * Function Name  : lteMacUpdateFapiStartReqStats
 * Inputs         : pBufferToSend - buffer being sent to PHY interface
 *                  bufferlength - length pf buffer in bytes.
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function updates stats for FAPI START REQUEST
 *                  message.
 ****************************************************************************/
/* +- SPR 17777*/
static  void lteMacUpdateFapiStartReqStats(
        InternalCellIndex internalCellIndex)
/* +- SPR 17777*/
{
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        startReqStats.numMsgs++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |= 
        LTE_MAC_STATS_RF_TX_START_REQ;
}

/****************************************************************************
 * Function Name  : lteMacUpdateFapiStopReqStats
 * Inputs         : pBufferToSend - buffer being sent to PHY interface
 *                  bufferlength - length pf buffer in bytes.
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function updates stats for FAPI STOP REQUEST
 *                  message.
 ****************************************************************************/
/* +- SPR 17777*/
static  void lteMacUpdateFapiStopReqStats(
        InternalCellIndex internalCellIndex)
/* +- SPR 17777*/
{
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        stopReqStats.numMsgs++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |= 
        LTE_MAC_STATS_RF_TX_STOP_REQ;
}

/****************************************************************************
 * Function Name  : lteMacUpdateFapiUeConfReqStats
 * Inputs         : pUeConfReq - pointer to structure FAPI_ueConfigRequest_st
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function updates stats for FAPI UE CONFIG REQUEST
 *                  message.
 ****************************************************************************/

/*Rel_523_Coverity_10165 Fix start*/ 
static  void lteMacUpdateFapiUeConfReqStats(FAPI_ueConfigRequest_st *pUeConfReq,
        InternalCellIndex internalCellIndex)
/*Rel_523_Coverity_10165 Fix end*/ 

{
    FAPI_ueConfig_st            *pNextTlv = PNULL;
    UInt32          iterator = 0;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        ueConfReqStats.numMsgs++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |= 
        LTE_MAC_STATS_RF_TX_UE_CONFIG_REQ;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        ueConfReqStats.numTlvs += pUeConfReq->numOfTlv;

    pNextTlv = pUeConfReq->tlvs;
    for (;iterator < pUeConfReq->numOfTlv; iterator++)
    {
        if (sizeof(FAPI_cellConfig_st) < pNextTlv->tagLen)
        {
            lteWarning("Tag length in UE CONFIG REQUEST is too large [%d]\n",
                    pNextTlv->tagLen);
            return;
        }
        gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
            ueConfReqStats.numBytes += pNextTlv->tagLen;
        pNextTlv = (FAPI_ueConfig_st *)
            (((UInt8 *)pNextTlv) + pNextTlv->tagLen);
    }
}

/****************************************************************************
 * Function Name  : lteMacUpdateFapiDlConfDciStats
 * Inputs         : pPdu - PDU being sent to PHY interface,
 *                  pduLen - length of pdu in bytes.
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function updates stats for FAPI DL CONFIG REQUEST
 *                  message DCI PDU.
 ****************************************************************************/
/* +- SPR 17777*/
static  void lteMacUpdateFapiDlConfDciStats(
        UInt32 pduLen, InternalCellIndex internalCellIndex)
{
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        dlConfReqStats.dciStats.numPdu++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |= 
        LTE_MAC_STATS_RF_TX_DL_CONFIG_DCI;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        dlConfReqStats.dciStats.numBytes += pduLen;
}

/****************************************************************************
 * Function Name  : lteMacUpdateFapiDlConfBchStats
 * Inputs         : pPdu - PDU being sent to PHY interface
 *                  pduLen - length of pdu in bytes.
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function updates stats for FAPI DL CONFIG REQUEST
 *                  message BCH PDU.
 ****************************************************************************/
static  void lteMacUpdateFapiDlConfBchStats(
        FAPI_bchConfigPDUInfo_st *pPdu, UInt32 pduLen, InternalCellIndex internalCellIndex)
{
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        dlConfReqStats.bchStats.numPdu++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |= 
        LTE_MAC_STATS_RF_TX_DL_CONFIG_BCH;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        dlConfReqStats.bchStats.numBytes +=
        pduLen;
    lLteMacPduIdxToPduType[internalCellIndex][pPdu->pduIndex] = FAPI_BCH_PDU;
}

/****************************************************************************
 * Function Name  : lteMacUpdateFapiDlConfMchStats
 * Inputs         : pPdu - PDU being sent to PHY interface
 *                  pduLen - length of pdu in bytes.
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function updates stats for FAPI DL CONFIG REQUEST
 *                  message MCH PDU.
 ****************************************************************************/
static  void lteMacUpdateFapiDlConfMchStats(
        FAPI_mchConfigPDUInfo_st *pPdu, UInt32 pduLen, InternalCellIndex internalCellIndex)
{
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        dlConfReqStats.mchStats.numPdu++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |= 
        LTE_MAC_STATS_RF_TX_DL_CONFIG_MCH;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        dlConfReqStats.mchStats.numBytes +=
        pduLen;
    lLteMacPduIdxToPduType[internalCellIndex][pPdu->pduIndex] = FAPI_MCH_PDU;
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiDlConfDlschStats
* Inputs         : pPdu - PDU being sent to PHY interface
*                  pduLen - length of pdu in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI DL CONFIG REQUEST
*                  message DLSCH PDU.
****************************************************************************/
static  void lteMacUpdateFapiDlConfDlschStats(
        FAPI_dlSCHConfigPDUInfo_st *pPdu, UInt32 pduLen, InternalCellIndex internalCellIndex)
{
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        dlConfReqStats.dlschStats.numPdu++;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        dlConfReqStats.dlschStats.numBytes += pduLen;
    lLteMacPduIdxToPduType[internalCellIndex][pPdu->pduIndex] = FAPI_DLSCH_PDU;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |= 
        LTE_MAC_STATS_RF_TX_DL_CONFIG_DLSCH;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).numTb = pPdu->transportBlocks;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).mcs = pPdu->mcs;
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiDlConfPchStats
* Inputs         : pPdu - PDU being sent to PHY interface
*                  pduLen - length of pdu in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI DL CONFIG REQUEST
*                  message PCH PDU.
****************************************************************************/
static  void lteMacUpdateFapiDlConfPchStats(
        FAPI_pchPduConfigInfo_st *pPdu, UInt32 pduLen, InternalCellIndex internalCellIndex)
{
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        dlConfReqStats.pchStats.numPdu++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |= 
        LTE_MAC_STATS_RF_TX_DL_CONFIG_PCH;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        dlConfReqStats.pchStats.numBytes += pduLen;
    lLteMacPduIdxToPduType[internalCellIndex][pPdu->pduIndex] = FAPI_PCH_PDU;
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiDlConfReqStats
* Inputs         : pDlConfReq - pointer to structure FAPI_dlConfigRequest_st
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI DL CONFIG REQUEST
*                  message.
****************************************************************************/
/*Rel_523_Coverity_10162 Fix start*/ 
static  void lteMacUpdateFapiDlConfReqStats(FAPI_dlConfigRequest_st  *pDlConfReq,
        InternalCellIndex internalCellIndex)
/*Rel_523_Coverity_10162 Fix end*/ 
{
    FAPI_dlConfigPDUInfo_st     *pDlPdu = PNULL;
    UInt32      iterator = 0;

    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        dlConfReqStats.numMsgs++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |= 
        LTE_MAC_STATS_RF_TX_DL_CONFIG;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        dlConfReqStats.numBytes += pDlConfReq->length;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        dlConfReqStats.numDci += pDlConfReq->numDCI;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        dlConfReqStats.numPdu +=
        pDlConfReq->numOfPDU;

    pDlPdu = pDlConfReq->dlConfigpduInfo;
    for (; iterator < pDlConfReq->numOfPDU; iterator++)
    {
        if (sizeof(FAPI_dlConfigPDUInfo_st) < pDlPdu->pduSize)
        {
            lteWarning("Length of DL CONFIG PDU is too large [%d]\n",
                    pDlPdu->pduSize);
            return;
        }
        switch(pDlPdu->pduType)
        {
            case FAPI_DCI_DL_PDU:
            {
                lteMacUpdateFapiDlConfDciStats(
                        /* +- SPR 17777 */
                        pDlPdu->pduSize, internalCellIndex);
                break;
            }
            case FAPI_BCH_PDU:
            {
                lteMacUpdateFapiDlConfBchStats(
                        &pDlPdu->dlConfigpduInfo.BCHPdu,
                        pDlPdu->pduSize, internalCellIndex);
                break;
            }
            case FAPI_MCH_PDU:
            {
                lteMacUpdateFapiDlConfMchStats(
                        &pDlPdu->dlConfigpduInfo.MCHPdu, pDlPdu->pduSize, 
                        internalCellIndex);
                break;
            }
            case FAPI_DLSCH_PDU:
            {
                lteMacUpdateFapiDlConfDlschStats(
                        &pDlPdu->dlConfigpduInfo.DlSCHPdu,
                        pDlPdu->pduSize, internalCellIndex);
                break;
            }
            case FAPI_PCH_PDU:
            {
                lteMacUpdateFapiDlConfPchStats(
                        &pDlPdu->dlConfigpduInfo.PCHPdu,
                        pDlPdu->pduSize, internalCellIndex);
                break;
            }
            default:
            {
                lteWarning("Unknown PDU type in DL CONFIG REQUEST [%d]\n",
                        pDlPdu->pduType);
                break;
            }
        }
        pDlPdu = (FAPI_dlConfigPDUInfo_st *)
            (((UInt8 *)pDlPdu) + pDlPdu->pduSize);
    }
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiUlConfUlschStats
* Inputs         : pPdu - PDU being sent to PHY interface
*                  pduLen - length of pdu in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI UL CONFIG REQUEST
*                  message ULSCH PDU.
****************************************************************************/
static  void lteMacUpdateFapiUlConfUlschStats(
        /* +- SPR 17777*/
        UInt32 pduLen, 
        /* +- SPR 17777*/
        InternalCellIndex internalCellIndex)
{
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        ulConfReqStats.ulschStats.numPdu++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |= 
        LTE_MAC_STATS_RF_TX_UL_CONFIG_ULSCH;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        ulConfReqStats.ulschStats.numBytes += pduLen;
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiUlConfUlschCqiRiStats
* Inputs         : pPdu - PDU being sent to PHY interface
*                  pduLen - length of pdu in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI UL CONFIG REQUEST
*                  message ULSCH_CQI_RI PDU.
****************************************************************************/
static  void lteMacUpdateFapiUlConfUlschCqiRiStats(
        /* +- SPR 17777*/
        UInt32 pduLen, 
        /* +- SPR 17777*/
        InternalCellIndex internalCellIndex)
{
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.ulConfReqStats.
        ulschCqiRiStats.numPdu++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |=
        LTE_MAC_STATS_RF_TX_UL_CONFIG_ULSCH_CQI_RI;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.ulConfReqStats.
        ulschCqiRiStats.numBytes += pduLen;
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiUlConfUlschHarqStats
* Inputs         : pPdu - PDU being sent to PHY interface
*                  pduLen - length of pdu in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI UL CONFIG REQUEST
*                  message ULSCH_HARQ PDU.
****************************************************************************/
static  void lteMacUpdateFapiUlConfUlschHarqStats(
        /* +- SPR 17777*/
        UInt32 pduLen, 
        /* +- SPR 17777*/
        InternalCellIndex internalCellIndex)
{
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.ulConfReqStats.
        ulschHarqStats.numPdu++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |=
        LTE_MAC_STATS_RF_TX_UL_CONFIG_ULSCH_HARQ;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.ulConfReqStats.
        ulschHarqStats.numBytes += pduLen;
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiUlConfUlschCqiHarqRiStats
* Inputs         : pPdu - PDU being sent to PHY interface
*                  pduLen - length of pdu in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI UL CONFIG REQUEST
*                  message ULSCH_CQI_HARQ_RI.
****************************************************************************/
static  void lteMacUpdateFapiUlConfUlschCqiHarqRiStats(
        /* +- SPR 17777*/
        UInt32 pduLen, 
        /* +- SPR 17777*/
        InternalCellIndex internalCellIndex)
{
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.ulConfReqStats.
        ulschCqiHarqRiStats.numPdu++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |=
        LTE_MAC_STATS_RF_TX_UL_CONFIG_ULSCH_CQI_HARQ_RI;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.ulConfReqStats.
        ulschCqiHarqRiStats.numBytes += pduLen;
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiUlConfUciCqiStats
* Inputs         : pPdu - PDU being sent to PHY interface
*                  pduLen - length of pdu in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI UL CONFIG REQUEST
*                  message UCI_CQI PDU.
****************************************************************************/
static  void lteMacUpdateFapiUlConfUciCqiStats(
        /* +- SPR 17777*/
        UInt32 pduLen, 
        /* +- SPR 17777*/
        InternalCellIndex internalCellIndex)
{
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.ulConfReqStats.
        uciCqiStats.numPdu++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |= 
        LTE_MAC_STATS_RF_TX_UL_CONFIG_UCI_CQI;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.ulConfReqStats.
        uciCqiStats.numBytes += pduLen;
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiUlConfUciSrStats
* Inputs         : pPdu - PDU being sent to PHY interface
*                  pduLen - length of pdu in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI UL CONFIG REQUEST
*                  message UCI_SR PDU.
****************************************************************************/
static  void lteMacUpdateFapiUlConfUciSrStats(
        /* +- SPR 17777*/
        UInt32 pduLen,
        /* +- SPR 17777*/
        InternalCellIndex internalCellIndex)
{
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.ulConfReqStats.
        uciSrStats.numPdu++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |= 
        LTE_MAC_STATS_RF_TX_UL_CONFIG_UCI_SR;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.ulConfReqStats.
        uciSrStats.numBytes += pduLen;
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiUlConfUciHarqStats
* Inputs         : pPdu - PDU being sent to PHY interface
*                  pduLen - length of pdu in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI UL CONFIG REQUEST
*                  message UCI_HARQ PDU.
****************************************************************************/
static  void lteMacUpdateFapiUlConfUciHarqStats(
        /* +- SPR 17777*/
        UInt32 pduLen,
        /* +- SPR 17777*/
        InternalCellIndex internalCellIndex)
{
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.ulConfReqStats.
        uciHarqStats.numPdu++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |= 
        LTE_MAC_STATS_RF_TX_UL_CONFIG_UCI_HARQ;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.ulConfReqStats.
        uciHarqStats.numBytes += pduLen;
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiUlConfUciSrHarqStats
* Inputs         : pPdu - PDU being sent to PHY interface
*                  pduLen - length of pdu in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI UL CONFIG REQUEST
*                  message UCI_SR_HARQ PDU.
****************************************************************************/
static  void lteMacUpdateFapiUlConfUciSrHarqStats(
        /* +- SPR 17777*/
        UInt32 pduLen,
        /* +- SPR 17777*/
        InternalCellIndex internalCellIndex)
{
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.ulConfReqStats.
        uciSrHarqStats.numPdu++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |=
        LTE_MAC_STATS_RF_TX_UL_CONFIG_UCI_SR_HARQ;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.ulConfReqStats.
        uciSrHarqStats.numBytes += pduLen;
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiUlConfUciCqiHarqStats
* Inputs         : pPdu - PDU being sent to PHY interface
*                  pduLen - length of pdu in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI UL CONFIG REQUEST
*                  message UCI_CQI_HARQ PDU.
****************************************************************************/
static  void lteMacUpdateFapiUlConfUciCqiHarqStats(
        /* +- SPR 17777*/
        UInt32 pduLen, 
        /* +- SPR 17777*/
        InternalCellIndex internalCellIndex)
{
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.ulConfReqStats.
        uciCqiHarqStats.numPdu++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |=
        LTE_MAC_STATS_RF_TX_UL_CONFIG_UCI_CQI_HARQ;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.ulConfReqStats.
        uciCqiHarqStats.numBytes += pduLen;
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiUlConfUciCqiSrStats
* Inputs         : pPdu - PDU being sent to PHY interface
*                  pduLen - length of pdu in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI UL CONFIG REQUEST
*                  message UCI_CQI_SR PDU.
****************************************************************************/
static  void lteMacUpdateFapiUlConfUciCqiSrStats(
        /* +- SPR 17777*/
         UInt32 pduLen, 
         /* +- SPR 17777*/
        InternalCellIndex internalCellIndex)
{
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.ulConfReqStats.
        uciCqiSrStats.numPdu++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |=
        LTE_MAC_STATS_RF_TX_UL_CONFIG_UCI_CQI_SR;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.ulConfReqStats.
        uciCqiSrStats.numBytes += pduLen;
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiUlConfUciCqiSrHarqStats
* Inputs         : pPdu - PDU being sent to PHY interface
*                  pduLen - length of pdu in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI UL CONFIG REQUEST
*                  message UCI_CQI_SR_HARQ PDU.
****************************************************************************/
static  void lteMacUpdateFapiUlConfUciCqiSrHarqStats(
        /* +- SPR 17777*/
        UInt32 pduLen, 
        /* +- SPR 17777*/
        InternalCellIndex internalCellIndex)
{
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.ulConfReqStats.
        uciCqiSrHarqStats.numPdu++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |=
        LTE_MAC_STATS_RF_TX_UL_CONFIG_UCI_CQI_SR_HARQ;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.ulConfReqStats.
        uciCqiSrHarqStats.numBytes += pduLen;
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiUlConfSrsStats
* Inputs         : pPdu - PDU being sent to PHY interface
*                  pduLen - length of pdu in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI UL CONFIG REQUEST
*                  message SRS PDU.
****************************************************************************/
static  void lteMacUpdateFapiUlConfSrsStats(
        /* +- SPR 17777*/
        UInt32 pduLen, 
        /* +- SPR 17777*/
        InternalCellIndex internalCellIndex)
{
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.ulConfReqStats.
        srsStats.numPdu++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |= 
        LTE_MAC_STATS_RF_TX_UL_CONFIG_SRS;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        ulConfReqStats.srsStats.numBytes += pduLen;
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiUlConfReqStats
* Inputs         : pUlConfReq pointer to strucure FAPI_ulConfigRequest_st
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI UL CONFIG REQUEST
*                  message.
****************************************************************************/

/*Rel_523_Coverity_10166 Fix start*/ 
static  void lteMacUpdateFapiUlConfReqStats(FAPI_ulConfigRequest_st  *pUlConfReq,
        InternalCellIndex internalCellIndex)
/*Rel_523_Coverity_10166 Fix start*/ 
{
    FAPI_ulPDUConfigInfo_st     *pUlPdu = PNULL;
    UInt32      iterator = 0;
    UInt32      remainingLen = 0;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        ulConfReqStats.numMsgs++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |= 
        LTE_MAC_STATS_RF_TX_UL_CONFIG;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        ulConfReqStats.numPdu += pUlConfReq->numOfPdu;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        ulConfReqStats.numBytes += pUlConfReq->ulConfigLen;

    remainingLen = pUlConfReq->ulConfigLen;
    pUlPdu = (FAPI_ulPDUConfigInfo_st *)pUlConfReq->ulPduConfigInfo;
    for (; iterator < pUlConfReq->numOfPdu; iterator++)
    {
        if (remainingLen < pUlPdu->ulConfigPduSize)
        {
            LTE_MAC_UT_LOG(LOG_WARNING, OAM_STAT_MGR,"Length of UL CONFIG PDU is too large [%d]\n",
                    pUlPdu->ulConfigPduSize);
        }
        switch(pUlPdu->ulConfigPduType)
        {
            case FAPI_ULSCH:
            {
                lteMacUpdateFapiUlConfUlschStats(
                        /* +- SPR 17777 */
                        pUlPdu->ulConfigPduSize, internalCellIndex);
                break;
            }
            case FAPI_ULSCH_CQI_RI:
            {
                lteMacUpdateFapiUlConfUlschCqiRiStats(
                        /* +- SPR 17777 */
                        pUlPdu->ulConfigPduSize, internalCellIndex);
                break;
            }
            case FAPI_ULSCH_HARQ:
            {
                lteMacUpdateFapiUlConfUlschHarqStats(
                        /* +- SPR 17777 */
                        pUlPdu->ulConfigPduSize, internalCellIndex);
                break;
            }
            case FAPI_ULSCH_CQI_HARQ_RI:
            {
                lteMacUpdateFapiUlConfUlschCqiHarqRiStats(
                        /* +- SPR 17777 */
                        pUlPdu->ulConfigPduSize, internalCellIndex);
                break;
            }
            case FAPI_UCI_CQI:
            {
                lteMacUpdateFapiUlConfUciCqiStats(
                        /* +- SPR 17777 */
                        pUlPdu->ulConfigPduSize, internalCellIndex);
                break;
            }
            case FAPI_UCI_SR:
            {
                lteMacUpdateFapiUlConfUciSrStats(
                        /* +- SPR 17777 */
                        pUlPdu->ulConfigPduSize, internalCellIndex);
                break;
            }
            case FAPI_UCI_HARQ:
            {
                lteMacUpdateFapiUlConfUciHarqStats(
                        /* +- SPR 17777 */
                        pUlPdu->ulConfigPduSize, internalCellIndex);
                break;
            }
            case FAPI_UCI_SR_HARQ:
            {
                lteMacUpdateFapiUlConfUciSrHarqStats(
                        /* +- SPR 17777 */
                        pUlPdu->ulConfigPduSize, internalCellIndex);
                break;
            }
            case FAPI_UCI_CQI_HARQ:
            {
                lteMacUpdateFapiUlConfUciCqiHarqStats(
                        /* +- SPR 17777 */
                        pUlPdu->ulConfigPduSize, internalCellIndex);
                break;
            }
            case FAPI_UCI_CQI_SR:
            {
                lteMacUpdateFapiUlConfUciCqiSrStats(
                        /* +- SPR 17777 */
                        pUlPdu->ulConfigPduSize, internalCellIndex);
                break;
            }
            case FAPI_UCI_CQI_SR_HARQ:
            {
                lteMacUpdateFapiUlConfUciCqiSrHarqStats(
                        /* +- SPR 17777 */
                        pUlPdu->ulConfigPduSize, internalCellIndex);
                break;
            }
            case FAPI_SRS:
            {
                lteMacUpdateFapiUlConfSrsStats(
                        /* +- SPR 17777 */
                        pUlPdu->ulConfigPduSize, internalCellIndex);
                break;
            }
            default:
            {
                lteWarning("Unknown PDU type in UL CONFIG REQUEST [%d]\n",
                        pUlPdu->ulConfigPduType);
                break;
            }
        }
        remainingLen -= pUlPdu->ulConfigPduSize;
        pUlPdu = (FAPI_ulPDUConfigInfo_st *)
            (((UInt8 *)pUlPdu) + pUlPdu->ulConfigPduSize);
    }
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiHiDci0ReqStats
* Inputs         : pBufferToSend - buffer being sent to PHY interface,
*                  bufferLen - length pf buffer in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI HI DCI0 REQUEST
*                  message.
****************************************************************************/
static  void lteMacUpdateFapiHiDci0ReqStats(UInt8 *pBufferToSend,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex)
{
    FAPI_dlHiDCIPduInfo_st      *pHiDciMsg = (FAPI_dlHiDCIPduInfo_st *)
        ((FAPI_l1ApiMsg_st *)pBufferToSend)->msgBody;

    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        hiDci0ReqStats.numMsgs++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |= 
        LTE_MAC_STATS_RF_TX_HI_DCI0_REQ;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        hiDci0ReqStats.numHi += pHiDciMsg->numOfHI;
    if (0 < pHiDciMsg->numOfHI)
    {
        LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |= 
            LTE_MAC_STATS_RF_TX_HI_DCI0_HI;
    }
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        hiDci0ReqStats.numDci0 += pHiDciMsg->numOfDCI;
    if (0 < pHiDciMsg->numOfDCI)
    {
        LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |= 
            LTE_MAC_STATS_RF_TX_HI_DCI0_DCI0;
    }
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiTxReqDlschStats
* Inputs         : pPdu - PDU being sent to PHY interface,
*                  pduLen - length of pdu in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI TX REQUEST
*                  message DLSCH PDU.
****************************************************************************/
/* +- SPR 17777*/
static  void lteMacUpdateFapiTxReqDlschStats(
        /* +- SPR 17777*/
        UInt32 pduLen, InternalCellIndex internalCellIndex)
{
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        txReqStats.dlschStats.numPdu++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |= LTE_MAC_STATS_RF_TX_TX_REQ_DLSCH;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        txReqStats.dlschStats.numBytes += offsetof(FAPI_dlPduInfo_st, dlTLVInfo);
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        txReqStats.dlschStats.numBytes += pduLen;
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiTxReqBchStats
* Inputs         : pPdu - PDU being sent to PHY interface,
*                  pduLen - length of pdu in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI TX REQUEST
*                  message BCH PDU.
****************************************************************************/
/* +- SPR 17777*/
static  void lteMacUpdateFapiTxReqBchStats(
/* +- SPR 17777*/
        UInt32 pduLen, InternalCellIndex internalCellIndex)
{
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        txReqStats.bchStats.numPdu++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |= 
        LTE_MAC_STATS_RF_TX_TX_REQ_BCH;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        txReqStats.bchStats.numBytes += offsetof(FAPI_dlPduInfo_st, dlTLVInfo);
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        txReqStats.bchStats.numBytes += pduLen;
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiTxReqMchStats
* Inputs         : pPdu - PDU being sent to PHY interface,
*                  pduLen - length of pdu in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI TX REQUEST
*                  message MCH PDU.
*
****************************************************************************/
/* +- SPR 17777*/
static  void lteMacUpdateFapiTxReqMchStats(
/* +- SPR 17777*/
        UInt32 pduLen, InternalCellIndex internalCellIndex)
{
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        txReqStats.mchStats.numPdu++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |= 
        LTE_MAC_STATS_RF_TX_TX_REQ_MCH;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        txReqStats.mchStats.numBytes += offsetof(FAPI_dlPduInfo_st, dlTLVInfo);
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        txReqStats.mchStats.numBytes += pduLen;
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiTxReqPchStats
* Inputs         : pPdu - PDU being sent to PHY interface,
*                  pduLen - length of pdu in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI TX REQUEST
*                  message PCH PDU.
****************************************************************************/
/* +- SPR 17777*/
static  void lteMacUpdateFapiTxReqPchStats(
/* +- SPR 17777*/
        UInt32 pduLen, InternalCellIndex internalCellIndex)
{
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        txReqStats.pchStats.numPdu++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |= 
        LTE_MAC_STATS_RF_TX_TX_REQ_PCH;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        txReqStats.pchStats.numBytes += offsetof(FAPI_dlPduInfo_st, dlTLVInfo);
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        txReqStats.pchStats.numBytes +=
        pduLen;
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiTxReqStats
* Inputs         : pTxReq - pointer to structure FAPI_dlDataTxRequest_st
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI TX REQUEST
*                  message.
****************************************************************************/

/*Rel_523_Coverity_10164 Fix start*/ 
static  void lteMacUpdateFapiTxReqStats(FAPI_dlDataTxRequest_st     *pTxReq,
        InternalCellIndex internalCellIndex)
/*Rel_523_Coverity_10164 Fix start*/ 
{
    FAPI_dlPduInfo_st           *pPdu = PNULL;
    UInt32      iterator = 0;
    UInt32      dataLen = 0;

    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        txReqStats.numMsgs++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx |= 
        LTE_MAC_STATS_RF_TX_TX_REQ;
    gMacStats.pStats->phyStats[internalCellIndex].fapiTxStats.
        txReqStats.numPdu += pTxReq->numOfPDU;

    pPdu = pTxReq->dlPduInfo;
    for (; iterator < pTxReq->numOfPDU; iterator++)
    {
        /*SPR 2235 Fix Begin*/
        if (LTE_MAC_STATS_MAX_PDU <= pPdu->pduIndex) 
            /*SPR 2235 Fix End*/
        {
            lteWarning("PDU Index too large in TX REQUEST [%d]\n",
                    pPdu->pduIndex);
            return;
        }
        switch(lLteMacPduIdxToPduType[internalCellIndex][pPdu->pduIndex])
        {
            case FAPI_BCH_PDU:
            {
                /* +- SPR 17777 */
                lteMacUpdateFapiTxReqBchStats( pPdu->pduLen, 
                        internalCellIndex);
                dataLen += pPdu->pduLen;
                break;
            }
            case FAPI_MCH_PDU:
            {
                /* +- SPR 17777 */
                lteMacUpdateFapiTxReqMchStats( pPdu->pduLen, 
                        internalCellIndex);
                dataLen += pPdu->pduLen;
                break;
            }
            case FAPI_DLSCH_PDU:
            {
                /* +- SPR 17777 */
                lteMacUpdateFapiTxReqDlschStats( pPdu->pduLen, 
                        internalCellIndex);
                dataLen += pPdu->pduLen;
                break;
            }
            case FAPI_PCH_PDU:
            {
                /* +- SPR 17777 */
                lteMacUpdateFapiTxReqPchStats( pPdu->pduLen, 
                        internalCellIndex);
                dataLen += pPdu->pduLen;
                break;
            }
            default:
            {
                lteWarning("Invalid PDU type/PDU type not received in DL "
                        "CONFIG REQUEST for TX PDU [%d]\n",
                        lLteMacPduIdxToPduType[internalCellIndex][pPdu->pduIndex]);
                break;
            }
        }
        lLteMacPduIdxToPduType[internalCellIndex][pPdu->pduIndex] = 0;
        pPdu = (FAPI_dlPduInfo_st *)(((UInt8 *)pPdu) + pPdu->pduLen);
    }
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).dlSize = dataLen;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).numTxPdu = pTxReq->numOfPDU;
}

/****************************************************************************
* Function Name  : lteMacUpdatePhyFapiTxStats
* Inputs         : pBufferToSend - buffer being sent to PHY interface,
*                  bufferLen - length pf buffer in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI TX messages.
****************************************************************************/
static  void lteMacUpdatePhyFapiTxStats(UInt8 *pBufferToSend,
                /* +- SPR 17777 */
        InternalCellIndex internalCellIndex)
{
    FAPI_l1ApiMsg_st        *pL1Api = (FAPI_l1ApiMsg_st *)pBufferToSend;

    switch(pL1Api->msgId)
    {
        case PHY_PARAM_REQUEST:
        {
            /* +- SPR 17777 */
            lteMacUpdateFapiParReqStats(internalCellIndex);
            break;
        }
        case PHY_CELL_CONFIG_REQUEST:
        {
            /* +- SPR 17777 */
            lteMacUpdateFapiCellConfReqStats(pBufferToSend,
                    internalCellIndex);
            break;
        }
        case PHY_START_REQUEST:
        {
            /* +- SPR 17777 */
            lteMacUpdateFapiStartReqStats(
                    internalCellIndex);
            break;
        }
        case PHY_STOP_REQUEST:
        {
            /* +- SPR 17777 */
            lteMacUpdateFapiStopReqStats(
                    internalCellIndex);
            break;
        }
        case PHY_UE_CONFIG_REQUEST:
        {
            /*Rel_523_Coverity_10165 Fix start*/ 
            FAPI_ueConfigRequest_st     *pUeConfReq = (FAPI_ueConfigRequest_st *)pL1Api->msgBody;
            lteMacUpdateFapiUeConfReqStats(pUeConfReq, internalCellIndex);
            /*Rel_523_Coverity_10165 Fix end*/ 
            break;
        }
        case PHY_DL_CONFIG_REQUEST:
        {
            /*Rel_523_Coverity_10162 Fix start*/ 
            FAPI_dlConfigRequest_st     *pDlConfReq = (FAPI_dlConfigRequest_st *)pL1Api->msgBody;
            lteMacUpdateFapiDlConfReqStats(pDlConfReq, internalCellIndex);
            /*Rel_523_Coverity_10162 Fix end*/ 
            break;
        }
        case PHY_UL_CONFIG_REQUEST:
        {

            /*Rel_523_Coverity_10166 Fix start*/ 
            FAPI_ulConfigRequest_st     *pUlConfReq = (FAPI_ulConfigRequest_st *)pL1Api->msgBody;
            lteMacUpdateFapiUlConfReqStats(pUlConfReq, internalCellIndex);
            /*Rel_523_Coverity_10166 Fix end*/ 
            break;
        }
        case PHY_DL_HI_DCI0_REQUEST:
        {
            /* +- SPR 17777 */
            lteMacUpdateFapiHiDci0ReqStats(pBufferToSend,
                    internalCellIndex);
            break;
        }
        case PHY_DL_TX_REQUEST:
        {
            /*Rel_523_Coverity_10164 Fix start*/ 
            FAPI_dlDataTxRequest_st   *pTxReq = (FAPI_dlDataTxRequest_st *)pL1Api->msgBody;
            lteMacUpdateFapiTxReqStats(pTxReq, internalCellIndex);
            /*Rel_523_Coverity_10164 Fix end*/ 
            break;
        }
        default:
        {
            lteWarning("Invalid Msg ID in L1 API message [%d]\n",
                    pL1Api->msgId);
            break;
        }
    }
}

/****************************************************************************
* Function Name  : lteMacUpdateStatsPhyTx 
* Inputs         : pBufferToSend - buffer being sent to PHY interface,
*                  bufferLen - length pf buffer in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates the stats for data being sent to
*                  PHY interface.
****************************************************************************/
void lteMacUpdateStatsPhyTx(UInt8 *pBufferToSend, UInt32 bufferLen,
        InternalCellIndex internalCellIndex)
{
    /* If stats memory is not mapped, just return */
    if (PNULL == gMacStats.pStats)
    {
        return;
    }

    if (PNULL == pBufferToSend)
    {
        return;
    }

    /* update phy TX stats */
    gMacStats.pStats->phyStats[internalCellIndex].txStats.numMsgs++;
    gMacStats.pStats->phyStats[internalCellIndex].txStats.numBytes += bufferLen;

/* FAPI Changes */
    /* Update FAPI phy stats */
    /* +- SPR 17777 */
    lteMacUpdatePhyFapiTxStats(pBufferToSend,internalCellIndex);
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiParRespStats
* Inputs         : pBufferToSend - buffer being sent to PHY interface,
*                  bufferLen - length pf buffer in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI PARAM RESPONSE
*                  message.
****************************************************************************/
static  void lteMacUpdateFapiParRespStats(UInt8 *pBufferToSend,
        /* +- SPR 17777*/
        InternalCellIndex internalCellIndex)
/* +- SPR 17777*/
{
    FAPI_paramResponse_st       *pParResp = (FAPI_paramResponse_st *)
        ((FAPI_l1ApiMsg_st *)pBufferToSend)->msgBody;

    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        parRespStats.numMsgs++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgRx |= 
        LTE_MAC_STATS_RF_RX_PARAM_RESP;
    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        parRespStats.numTlvs += pParResp->numOfTlv;
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiCellConfRespStats
* Inputs         : pBufferToSend - buffer being sent to PHY interface,
*                  bufferLen - length pf buffer in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI CELL CONFIG RESPONSE
*                  message.
****************************************************************************/
static  void lteMacUpdateFapiCellConfRespStats(UInt8 *pBufferToSend,
/* +- SPR 17777*/
        InternalCellIndex internalCellIndex)
/* +- SPR 17777*/
{
    FAPI_phyCellConfigResp_st   *pCellConfResp = (FAPI_phyCellConfigResp_st *)
        ((FAPI_l1ApiMsg_st *)pBufferToSend)->msgBody;

    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgRx |= 
        LTE_MAC_STATS_RF_RX_CELL_CONFIG_RESP;
    if (FAPI_MSG_OK == pCellConfResp->errorCode)
    {
        gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
            cellConfRespStats.numOk++;
    }
    else
    {
        gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
            cellConfRespStats.numFail++;
        gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
            cellConfRespStats.numInvalidTlvs += pCellConfResp->numOfInvalidOrUnsupportedTLV;
        gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
            cellConfRespStats.numMissingTlvs += pCellConfResp->numOfMissingTLV;
    }
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiStopIndStats
* Inputs         : pBufferToSend - buffer being sent to PHY interface,
*                  bufferLen - length pf buffer in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI STOP INDICATION
*                  message.
****************************************************************************/
/* +- SPR 17777*/
static  void lteMacUpdateFapiStopIndStats(
        InternalCellIndex internalCellIndex)
/* +- SPR 17777*/
{
    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        stopIndStats.numMsgs++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgRx |= 
        LTE_MAC_STATS_RF_RX_STOP_IND;
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiUeConfRespStats
* Inputs         : pBufferToSend - buffer being sent to PHY interface
*                  bufferLen - length pf buffer in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI UE CONFIG RESPONSE
*                  message.
****************************************************************************/
static  void lteMacUpdateFapiUeConfRespStats(UInt8 *pBufferToSend,
/* +- SPR 17777*/
        InternalCellIndex internalCellIndex)
/* +- SPR 17777*/
{
    FAPI_phyUeConfigResp_st     *pUeConfResp = (FAPI_phyUeConfigResp_st *)
        ((FAPI_l1ApiMsg_st *)pBufferToSend)->msgBody;
    FAPI_ueConfig_st            *pTlv = PNULL;
    UInt32      iterator = 0;

    /*+ Klockwork warning removal*/
    pTlv = pUeConfResp->listOfTLV;
    /*- Klockwork warning removal*/
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgRx |= 
        LTE_MAC_STATS_RF_RX_UE_CONFIG_RESP;
    if (FAPI_MSG_OK == pUeConfResp->errorCode)
    {
        gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
            ueConfRespStats.numOk++;
    }
    else
    {
        gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
            ueConfRespStats.numFail++;
        gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
            ueConfRespStats.numInvalidTlvs += pUeConfResp->numOfInvalidOrUnsupportedTLV;
        gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
            ueConfRespStats.numMissingTlvs += pUeConfResp->numOfMissingTLV;
    }
    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        ueConfRespStats.numBytes +=
        offsetof(FAPI_phyUeConfigResp_st, listOfTLV);
    for (; iterator < pUeConfResp->numOfInvalidOrUnsupportedTLV +
            pUeConfResp->numOfMissingTLV; iterator ++)
    {
        gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
            ueConfRespStats.numBytes += pTlv->tagLen;
        pTlv = (FAPI_ueConfig_st *)(((UInt8 *)pTlv) + pTlv->tagLen);
    }
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiErrIndStats
* Inputs         : pBufferToSend - buffer being sent to PHY interface,
*                  bufferLen - length pf buffer in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI ERROR INDICATION
*                  message.
****************************************************************************/
static  void lteMacUpdateFapiErrIndStats(UInt8 *pBufferToSend,
/* +- SPR 17777*/
        InternalCellIndex internalCellIndex)
/* +- SPR 17777*/
{
    FAPI_phyErrorIndication_st  *pErrInd = (FAPI_phyErrorIndication_st *)
        ((FAPI_l1ApiMsg_st *)pBufferToSend)->msgBody;

    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        errIndStats.numMsgs++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgRx |= 
        LTE_MAC_STATS_RF_RX_ERROR_IND;
    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        errIndStats.numBytes += offsetof(FAPI_phyErrorIndication_st, errMsgBody);
    switch(pErrInd->errorCode)
    {
        case FAPI_MSG_OK:
        {
            /* This error code should never come in error indication msg */
            lteWarning("Error indication received with error code = OK\n");
            break;
        }
        case FAPI_MSG_INVALID_STATE: /* Fall through */
        case FAPI_MSG_BCH_MISSING:
        {
            /* No Message body, so no more length added */
            break;
        }
        case FAPI_MSG_INVALID_CONFIG:
        case FAPI_MSG_SUBFRAME_ERR:
        {
            gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
                errIndStats.numBytes +=
                sizeof(FAPI_errMsgBody2_st);
            break;
        }
        case FAPI_SFN_OUT_OF_SYNC: /* Fall through */
        case FAPI_MSG_INVALID_SFN:
        {
            gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
                errIndStats.numBytes +=
                sizeof(FAPI_errMsgBody1_st);
            break;
        }
        case FAPI_MSG_HI_ERR:
        {
            gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
                errIndStats.numBytes +=
                sizeof(FAPI_errMsgBody3_st);
            break;
        }
        case FAPI_MSG_TX_ERR:
        {
            gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
                errIndStats.numBytes +=
                sizeof(FAPI_errMsgBody4_st);
            break;
        }
        default:
        {
            lteWarning("Unknown error code received in ERROR IND [%d]\n", 
                    pErrInd->errorCode);
            break;
        }
    }
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiSfIndStats
* Inputs         : pBufferToSend - buffer being sent to PHY interface,
*                  bufferLen - length pf buffer in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI SUBFRAME INDICATION
*                  message.
****************************************************************************/
static  void lteMacUpdateFapiSfIndStats(UInt8 *pBufferToSend,
/* +- SPR 17777*/
        InternalCellIndex internalCellIndex)
/* +- SPR 17777*/
{
    FAPI_subFrameIndication_st      *pSfInd = (FAPI_subFrameIndication_st *)
        ((FAPI_l1ApiMsg_st *)pBufferToSend)->msgBody;

    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        sfIndStats.numMsgs++;
	/* + PERF_CA GBL */
    lLteMacStatsRfSfn_g[internalCellIndex] = GET_SYSFRAMENUM_FROM_SFNSF(pSfInd->sfnsf);
    lLteMacStatsRfSf_g[internalCellIndex] = GET_SUBFRAME_FROM_SFNSF(pSfInd->sfnsf);
#if (MAX_SFN_VALUE < LTE_MAC_STATS_MAX_SFN)
    if( (0 == lLteMacStatsRfSfn_g[internalCellIndex]) && (0 == lLteMacStatsRfSf_g[internalCellIndex]) )
	/* - PERF_CA GBL */
    {
        lLteMacStatsRfMulFactor_g =
            (lLteMacStatsRfMulFactor_g+1)%(LTE_MAC_STATS_MAX_SFN/MAX_SFN_VALUE);
    }
#endif
    /* Reset all flags for this SFN SF */
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgTx = 0;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgRx = 0;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).numTb = 0;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).mcs = 0;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).dlSize = 0;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).ulSize = 0;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).numTxPdu = 0;
#ifdef TDD_CONFIG
#ifdef DL_UL_SPLIT_TDD
    memSet(LTE_MAC_STATS_RF_TTI_STATS.macCyclesPerTick,0,MAX_EXECUTION_LEGS);
    memSet(LTE_MAC_STATS_RF_TTI_STATS.pdcpCyclesPerTick,0,MAX_PDCP_CC_FUNC);
#ifdef GETSTATS_EL_CLOCKTIME
    memSet(LTE_MAC_STATS_RF_TTI_STATS.macFuncClkTimeInMicroSecPerTick,0,MAX_EXECUTION_LEGS);
    memSet(LTE_MAC_STATS_RF_TTI_STATS.pdcpFuncClkTimeInMicroSecPerTick,0,MAX_PDCP_CC_FUNC);
#endif
#else
    memSet(LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).
            legCyclesPerTick,0,MAX_LEG_CC_FUNC);
    memSet(LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).
            legFuncClkTimeInMicroSecPerTick, 0,MAX_LEG_CC_FUNC);
#endif
#else
    memSet(LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).
            macCyclesPerTick,0,MAX_MAC_CC_FUNC);
    memSet(LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).
            macFuncClkTimeInMicroSecPerTick,0,MAX_MAC_CC_FUNC);
    memSet(LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).
            rlcCyclesPerTick,0,MAX_RLC_CC_FUNC);
    memSet(LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).
            rlcFuncClkTimeInMicroSecPerTick,0,MAX_RLC_CC_FUNC);
    memSet(LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).
            pdcpCyclesPerTick,0,MAX_PDCP_CC_FUNC);
    memSet(LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).
            pdcpFuncClkTimeInMicroSecPerTick,0,MAX_PDCP_CC_FUNC);
#endif

    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgRx |= 
        LTE_MAC_STATS_RF_RX_SF_IND;
}


/****************************************************************************
* Function Name  : lteMacUpdateFapiHarqIndStats
* Inputs         : msg_p
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI HARQ INDICATION
*                  message.
****************************************************************************/
/*Rel_523_Coverity_10162 Fix start*/ 
 void lteMacUpdateFapiHarqIndStats(void *msg_p,
        InternalCellIndex internalCellIndex)
/*Rel_523_Coverity_10162 Fix start*/ 
{

    FAPI_harqIndication_st *pHarqInd=(FAPI_harqIndication_st *)msg_p;
    FAPI_fddHarqPduIndication_st    *pPdu = PNULL;
    UInt32      iterator = 0;

    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        harqIndStats.numMsgs++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgRx |= 
        LTE_MAC_STATS_RF_RX_HARQ_IND;
    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        harqIndStats.numHarqs += pHarqInd->numOfHarq;

    pPdu = (FAPI_fddHarqPduIndication_st*)pHarqInd->harqPduInfo;
    if( PNULL == pPdu || pHarqInd->numOfHarq > 8)
    {
        return;
    }
    for (; iterator < pHarqInd->numOfHarq; iterator++)
    {

        if (pPdu->harqTB1 == FAPI_ACK)
        {
            gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
                harqIndStats.numAck++;
        }
        else
        {
            gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
                harqIndStats.numNack++;
        }
        if (pPdu->harqTB2 == FAPI_ACK)
        {
            gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
                harqIndStats.numAck++;
        }
        else
        {
            gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
                harqIndStats.numNack++;
        }
/*SPR 21001 Changes end */
        pPdu = (FAPI_fddHarqPduIndication_st *)(((UInt8 *)pPdu) +
                sizeof(FAPI_fddHarqPduIndication_st));
    }
}

/* FAPI2.4 Changes Start */
#ifdef FAPI_4_0_COMPLIANCE
/****************************************************************************
* Function Name  : lteMacUpdateFapiHarqIndStatsR8
* Inputs         : msg_p
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI HARQ INDICATION
*                  message for Release 8.
****************************************************************************/
/*Rel_523_Coverity_10162 Fix start*/ 
 void lteMacUpdateFapiHarqIndStatsR8(void *msg_p,
        InternalCellIndex internalCellIndex)
/*Rel_523_Coverity_10162 Fix start*/ 
{
    FAPI_fddHarqPduIndicationR8_st    *pPdu = PNULL;
    UInt32      iterator = 0;

    FAPI_harqIndicationR8_st *pHarqInd=(FAPI_harqIndicationR8_st *)msg_p;
    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        harqIndStats.numMsgs++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgRx |= 
        LTE_MAC_STATS_RF_RX_HARQ_IND;
    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        harqIndStats.numHarqs += pHarqInd->numOfHarq;

    pPdu = (FAPI_fddHarqPduIndicationR8_st*)pHarqInd->harqPduInfo;
    if( PNULL == pPdu || pHarqInd->numOfHarq > 8)
    {
        return;
    }
    for (; iterator < pHarqInd->numOfHarq; iterator++)
    {
        if (pPdu->release8Param.harqTB1 == FAPI_ACK)
        {
            gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
                harqIndStats.numAck++;
        }
        else
        {
            gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
                harqIndStats.numNack++;
        }
        if (pPdu->release8Param.harqTB2 == FAPI_ACK)
        {
            gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
                harqIndStats.numAck++;
        }
        else
        {
            gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
                harqIndStats.numNack++;
        }
        pPdu = (FAPI_fddHarqPduIndicationR8_st *)(((UInt8 *)pPdu) +
                sizeof(FAPI_fddHarqPduIndicationR8_st));
    }
}
/****************************************************************************
* Function Name  : lteMacUpdateFapiHarqIndStatsR9
* Inputs         : msg_p
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI HARQ INDICATION
*                  message for  Release 9/10.
****************************************************************************/
 void lteMacUpdateFapiHarqIndStatsR9(void *msg_p,
        InternalCellIndex internalCellIndex)
/*Rel_523_Coverity_10162 Fix start*/ 
{
    FAPI_fddHarqPduIndicationR9_st    *pPdu = PNULL;
    UInt32      iterator = 0,count=0;

    FAPI_harqIndicationR9_st *pHarqInd=(FAPI_harqIndicationR9_st *)msg_p;
    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        harqIndStats.numMsgs++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgRx |= 
        LTE_MAC_STATS_RF_RX_HARQ_IND;
    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        harqIndStats.numHarqs += pHarqInd->numOfHarq;

    pPdu = (FAPI_fddHarqPduIndicationR9_st*)pHarqInd->harqPduInfo;
    if( PNULL == pPdu || pHarqInd->numOfHarq > 8)
    {
        return;
    }
    for (; iterator < pHarqInd->numOfHarq; iterator++)
    {
        for (count = 0; count < pPdu->release9Param.numOfAckNack; count++)
        {
            if (pPdu->release9Param.HARQTB[count] == FAPI_ACK)
            {
                gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
                    harqIndStats.numAck++;
            }
            else
            {
                gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
                    harqIndStats.numNack++;
            }
        }

        pPdu = (FAPI_fddHarqPduIndicationR9_st *)(((UInt8 *)pPdu) +
                sizeof(FAPI_fddHarqPduIndicationR9_st));
    }
}
#endif
/* FAPI2.4 Changes End */

/****************************************************************************
* Function Name  : lteMacUpdateFapiCrcIndStats
* Inputs         : pBufferToSend - buffer being sent to PHY interface
*                  bufferLen - length pf buffer in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI CRC INDICATION
*                  message.
****************************************************************************/
static  void lteMacUpdateFapiCrcIndStats(UInt8 *pBufferToSend,
/* +- SPR 17777*/
        InternalCellIndex internalCellIndex)
/* +- SPR 17777*/
{
    FAPI_crcIndication_st      *pCrcInd = (FAPI_crcIndication_st *)
        ((FAPI_l1ApiMsg_st *)pBufferToSend)->msgBody;
    UInt32      iterator = 0;

    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        crcIndStats.numMsgs++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgRx |= 
        LTE_MAC_STATS_RF_RX_CRC_IND;
    for(; iterator < pCrcInd->numOfCrc; iterator++)
    {
        if (0 == pCrcInd->crcPduInfo[iterator].crcFlag)
        {
            gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
                crcIndStats.numOk++;
        }
        else
        {
            gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
                crcIndStats.numErr++;
        }
    }
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiRxUlschStats
* Inputs         : pBufferToSend - buffer being sent to PHY interface
*                  bufferLen - length pf buffer in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI RX ULSCH message.
****************************************************************************/
 void lteMacUpdateFapiRxUlschStats(UInt8 *pBufferToSend,
        /* +- SPR 17777*/
        InternalCellIndex internalCellIndex)
{
    FAPI_rxULSCHIndication_st   *pRxUlschInd = (FAPI_rxULSCHIndication_st *)
        ((FAPI_l1ApiMsg_st *)pBufferToSend)->msgBody;
    UInt32      iterator = 0;

    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        rxUlschStats.numMsgs++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgRx |= 
        LTE_MAC_STATS_RF_RX_RX_ULSCH_IND;
    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        rxUlschStats.numPdu += pRxUlschInd->numOfPdu;

    for(; iterator < pRxUlschInd->numOfPdu; iterator++)
    {
        gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
            rxUlschStats.numBytes += pRxUlschInd->ulDataPduInfo[iterator].length;
        LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).ulSize +=
            pRxUlschInd->ulDataPduInfo[iterator].length;
    }
}

/* FAPI2.4 Changes Start */
#ifdef FAPI_4_0_COMPLIANCE
/****************************************************************************
* Function Name  : lteMacUpdateFapiRxUlschStatsR9 
* Inputs         : pBufferToSend - buffer being sent to PHY interface
*                  bufferLen - length pf buffer in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI RX ULSCH message for
                   Release 9/10.
****************************************************************************/
 void lteMacUpdateFapiRxUlschStatsR9(UInt8 *pBufferToSend,
        /* +- SPR 17777*/
        InternalCellIndex internalCellIndex)
{
    FAPI_rxULSCHIndicationR9_st   *pRxUlschInd = (FAPI_rxULSCHIndicationR9_st *)
        ((FAPI_l1ApiMsg_st *)pBufferToSend)->msgBody;
    UInt32      iterator = 0;

    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        rxUlschStats.numMsgs++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgRx |= 
        LTE_MAC_STATS_RF_RX_RX_ULSCH_IND;
    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        rxUlschStats.numPdu += pRxUlschInd->numOfPdu;

    for(; iterator < pRxUlschInd->numOfPdu; iterator++)
    {
        gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
            rxUlschStats.numBytes += pRxUlschInd->ulDataPduInfo[iterator].length;
        LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).ulSize +=
            pRxUlschInd->ulDataPduInfo[iterator].length;
    }
}
#endif
/* FAPI2.4 Changes End */

/****************************************************************************
* Function Name  : lteMacUpdateFapiRachIndStats
* Inputs         : pBufferToSend - buffer being sent to PHY interface
*                  bufferLen - length pf buffer in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI RACH INDICATION
*                  message.
****************************************************************************/
  void lteMacUpdateFapiRachIndStats(UInt8 *pBufferToSend,
         /* +- SPR 17777*/
        InternalCellIndex internalCellIndex)
{
    FAPI_rachIndication_st      *pRachInd = (FAPI_rachIndication_st *)
        ((FAPI_l1ApiMsg_st *)pBufferToSend)->msgBody;

    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        rachIndStats.numMsgs++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgRx |= 
        LTE_MAC_STATS_RF_RX_RACH_IND;
    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        rachIndStats.numPreambles += pRachInd->numOfPreamble;
}

/* FAPI2.4 Changes Start */
#ifdef FAPI_4_0_COMPLIANCE
/****************************************************************************
* Function Name  : lteMacUpdateFapiRachIndStatsR9
* Inputs         : pBufferToSend - buffer being sent to PHY interface
*                  bufferLen - length pf buffer in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI RACH INDICATION
*                  message.
****************************************************************************/
 void lteMacUpdateFapiRachIndStatsR9(UInt8 *pBufferToSend,
        /* +- SPR 17777*/
        InternalCellIndex internalCellIndex)
{
    FAPI_rachIndicationR9_st      *pRachInd = (FAPI_rachIndicationR9_st *)
        ((FAPI_l1ApiMsg_st *)pBufferToSend)->msgBody;

    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        rachIndStats.numMsgs++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgRx |= 
        LTE_MAC_STATS_RF_RX_RACH_IND;
    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        rachIndStats.numPreambles += pRachInd->numOfPreamble;
}
#endif
/* FAPI2.4 Changes End */

/****************************************************************************
* Function Name  : lteMacUpdateFapiSrsIndStats
* Inputs         : pBufferToSend - buffer being sent to PHY interface
*                  bufferLen - length pf buffer in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI SRS INDICATION
*                  message.
****************************************************************************/
/* +- SPR 17777*/
static  void lteMacUpdateFapiSrsIndStats(
        InternalCellIndex internalCellIndex)
/* +- SPR 17777*/
{
    /* Added to remove Klockworks warning */
    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        srsIndStats.numMsgs++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgRx |= 
        LTE_MAC_STATS_RF_RX_SRS_IND;
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiRxSrIndStats
* Inputs         : pBufferToSend - buffer being sent to PHY interface
*                  bufferLen - length pf buffer in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI RX SR message.
*
****************************************************************************/
static  void lteMacUpdateFapiRxSrIndStats(UInt8 *pBufferToSend,
        /* +- SPR 17777*/
        InternalCellIndex internalCellIndex)
/* +- SPR 17777*/
{
    FAPI_rxSRIndication_st      *pRxSrInd = (FAPI_rxSRIndication_st *)
        ((FAPI_l1ApiMsg_st *)pBufferToSend)->msgBody;

    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        rxSrIndStats.numMsgs++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgRx |= 
        LTE_MAC_STATS_RF_RX_RX_SR_IND;
    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        rxSrIndStats.numSr += pRxSrInd->numOfSr;
}

/****************************************************************************
* Function Name  : lteMacUpdateFapiCqiIndStats
* Inputs         : pBufferToSend - buffer being sent to PHY interface
*                  bufferLen - length pf buffer in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI CQI INDICATION
*                  message.
*
****************************************************************************/
 void lteMacUpdateFapiCqiIndStats(UInt8 *pBufferToSend,
        /* +- SPR 17777*/
        InternalCellIndex internalCellIndex)
{
    FAPI_rxCqiIndication_st     *pCqiInd = (FAPI_rxCqiIndication_st *)
        ((FAPI_l1ApiMsg_st *)pBufferToSend)->msgBody;
    UInt32      iterator = 0;

    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        cqiIndStats.numMsgs++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgRx |= 
        LTE_MAC_STATS_RF_RX_RX_CQI_IND;
    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        cqiIndStats.numCqi += pCqiInd->numOfCqi;

    for (; iterator < pCqiInd->numOfCqi; iterator++)
    {
        gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
            cqiIndStats.numBytes += pCqiInd->cqiPduInfo[iterator].length;
    }
}

/* FAPI2.4 Changes Start */
#ifdef FAPI_4_0_COMPLIANCE
/****************************************************************************
* Function Name  : lteMacUpdateFapiCqiIndStatsR9
* Inputs         : pBufferToSend - buffer being sent to PHY interface
*                  bufferLen - length pf buffer in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI CQI INDICATION
*                  message in case of Release9/10.
*
****************************************************************************/
 void lteMacUpdateFapiCqiIndStatsR9(UInt8 *pBufferToSend,
        /* +- SPR 17777*/
        InternalCellIndex internalCellIndex)
{
    FAPI_rxCqiIndicationR9_st     *pCqiInd = (FAPI_rxCqiIndicationR9_st *)
        ((FAPI_l1ApiMsg_st *)pBufferToSend)->msgBody;
    UInt32      iterator = 0;

    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        cqiIndStats.numMsgs++;
    LTE_MAC_STATS_RF_TTI_STATS(internalCellIndex).msgRx |= 
        LTE_MAC_STATS_RF_RX_RX_CQI_IND;
    gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
        cqiIndStats.numCqi += pCqiInd->numOfCqi;

    for (; iterator < pCqiInd->numOfCqi; iterator++)
    {
        gMacStats.pStats->phyStats[internalCellIndex].fapiRxStats.
            cqiIndStats.numBytes += pCqiInd->cqiPduInfo[iterator].length;
    }
}
#endif
/* FAPI2.4 Changes End */

/****************************************************************************
* Function Name  : lteMacUpdatePhyFapiRxStats
* Inputs         : pBufferToSend - buffer being sent to PHY interface
*                  bufferLen - length pf buffer in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for FAPI RX messages.
*
****************************************************************************/
static  void lteMacUpdatePhyFapiRxStats(UInt8 *pBufferToSend,
        /* +- SPR 17777 */
        InternalCellIndex internalCellIndex)
{
    FAPI_l1ApiMsg_st        *pL1Api = (FAPI_l1ApiMsg_st *)pBufferToSend;

    switch(pL1Api->msgId)
    {
        case PHY_PARAM_RESPONSE:
        {
            /* +- SPR 17777 */
            lteMacUpdateFapiParRespStats(pBufferToSend,
                    internalCellIndex);
            break;
        }
        case PHY_CELL_CONFIG_RESPONSE:
        {
            /* +- SPR 17777 */
            lteMacUpdateFapiCellConfRespStats(pBufferToSend,
                    internalCellIndex);
            break;
        }
        case PHY_STOP_INDICATION:
        {
            /* +- SPR 17777 */
            lteMacUpdateFapiStopIndStats(
                    internalCellIndex);
            break;
        }
        case PHY_UE_CONFIG_RESPONSE:
        {
            /* +- SPR 17777 */
            lteMacUpdateFapiUeConfRespStats(pBufferToSend,
                    internalCellIndex);
            break;
        }
        case PHY_ERROR_INDICATION:
        {
            /* +- SPR 17777 */
            lteMacUpdateFapiErrIndStats(pBufferToSend,
                    internalCellIndex);
            break;
        }
        case PHY_UL_SUBFRAME_INDICATION:
        {
            /* +- SPR 17777 */
            lteMacUpdateFapiSfIndStats(pBufferToSend,
                    internalCellIndex);
            break;
        }
        /* FAPI2.4 Changes Start */
        case PHY_UL_HARQ_INDICATION:
        {
            /*Rel_523_Coverity_10163 Fix start*/ 
            (*lteMacUpdateFapiHarqIndStatsFunction)(pL1Api->msgBody, internalCellIndex);
            /*Rel_523_Coverity_10163 Fix end*/ 
            break;
        }
        /* FAPI2.4 Changes End */
        case PHY_UL_CRC_INDICATION:
        {
            /* +- SPR 17777 */
            lteMacUpdateFapiCrcIndStats(pBufferToSend,
                    internalCellIndex);
            break;
        }
        /* FAPI2.4 Changes Start */
        case PHY_UL_RX_ULSCH_INDICATION:
        {
            (*lteMacUpdateFapiRxUlschStatsFunction)(pBufferToSend,
                    internalCellIndex);
            break;
        }
        case PHY_UL_RACH_INDICATION:
        {
            (*lteMacUpdateFapiRachIndStatsFunction)(pBufferToSend,
                    internalCellIndex);
            break;
        }
        /* FAPI2.4 Changes End */
        case PHY_UL_SRS_INDICATION:
        {
            /* +- SPR 17777 */
            lteMacUpdateFapiSrsIndStats(
                    internalCellIndex);
            break;
        }
        case PHY_UL_RX_SR_INDICATION:
        {
            /* +- SPR 17777 */
            lteMacUpdateFapiRxSrIndStats(pBufferToSend,
                    internalCellIndex);
            break;
        }
        /* FAPI2.4 Changes Start */
        case PHY_UL_RX_CQI_INDICATION:
        {
            (*lteMacUpdateFapiCqiIndStatsFunction)(pBufferToSend,
                    internalCellIndex);
            break;
        }
        /* FAPI2.4 Changes End */
        default:
        {
            lteWarning("Invalid Msg ID received in L1 API message [%d]\n",
                    pL1Api->msgId);
            break;
        }
    }
}

/****************************************************************************
* Function Name  : lteMacUpdateStatsPhyRx 
* Inputs         : pBufferReceived - buffer Received from PHY interface
*                  bufferLen - length of buffer in bytes.
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates the stats for data received from
*                  PHY interface.
****************************************************************************/
void lteMacUpdateStatsPhyRx(UInt8 *pBufferReceived, UInt32 bufferLen, 
        InternalCellIndex internalCellIndex)
{
    /* If stats memory is not mapped, just return */
    if (PNULL == gMacStats.pStats)
    {
        return;
    }

    if (PNULL == pBufferReceived)
    {
        return;
    }
    /* update phy RX stats */
    gMacStats.pStats->phyStats[internalCellIndex].rxStats.numMsgs++;
    gMacStats.pStats->phyStats[internalCellIndex].rxStats.numBytes += bufferLen;
    
/* FAPI Changes */
    /* Update FAPI phy stats */
    /* +- SPR 17777 */
    lteMacUpdatePhyFapiRxStats(pBufferReceived, internalCellIndex);
    /* +- SPR 17777 */
}

/* CA Stats Changes End */
#ifdef DL_UL_SPLIT_TDD
/****************************************************************************
* Function Name  : lteMacUpdateUlStatsPhyRx 
* Inputs         : pBufferReceived - buffer Received from PHY interface
*                  bufferlength - length of buffer in bytes.
* Outputs        : none
* Returns        : none
* Description    : This function updates the stats for data received from
*                  PHY interface in UL threads.
*
****************************************************************************/
void lteMacUpdateUlStatsPhyRx(UInt8 *pBufferReceived, UInt32 bufferLen)
{
    /* If stats memory is not mapped, just return */
    if (PNULL == gMacStats.pStats)
    {
        return;
    }

    if (PNULL == pBufferReceived)
    {
        return;
    }
    /* update phy RX stats */
    gMacStats.pStats->phyStats.rxStats.numUlMsgs++;
    gMacStats.pStats->phyStats.rxStats.numUlBytes += bufferLen;
    
    /* FAPI Changes */
    /* Update FAPI phy stats */
    /* +- SPR 17777 */
    lteMacUpdatePhyFapiRxStats(pBufferReceived);
    /* +- SPR 17777 */
}
#endif

/*SPR 21179 Start*/
#ifdef SSI_DEBUG_STATS
/*SPR 21179 End*/
/****************************************************************************
* Function Name  : lteMacUpdateStatsRlcTx 
* Inputs         : 
*                  pRlcBuffer - buffer being sent to PHY interface
* Outputs        : None
* Returns        : None
* Description    : This function updates the stats for data being sent to
*                  PHY interface.
****************************************************************************/
/* +- SPR 17777 */
void lteMacUpdateStatsRlcTx(RLCUplinkData * pRlcBuffer)
/* +- SPR 17777 */
{
    UInt32      iterator = 0;
    /* If stats memory is not mapped, just return */
    if (PNULL == gMacStats.pStats)
    {
        return;
    }

    if (PNULL == pRlcBuffer)
    {
        return;
    }
    /* update rlc RX stats */
    /* PURIFY FIXES 2.4.1 start */
    for(;iterator < pRlcBuffer->numLCInfo; iterator++)
    /* PURIFY FIXES 2.4.1 end */
    {
        /*SPR 21179 Start*/
        if (iterator >= (MAX_NUM_UL_PKTS - 1) )
        /*SPR 21179 End*/
        {
            lteWarning("Data to RLC corrupted");
            return;
        }
        if (PNULL == pRlcBuffer->rlcDataArr[iterator].rlcdataBuffer)
        {
            continue;
        }
        gMacStats.pStats->rlcStats.txStats.numMsgs++;
        gMacStats.pStats->rlcStats.txStats.numBytes +=
                pRlcBuffer->rlcDataArr[iterator].dataLength;
    }
}
/*SPR 21179 Start*/
#endif 
/*SPR 21179 End*/

/****************************************************************************
* Function Name  : lteMacUpdateStatsRlcRx 
* Inputs         :
*                  pRlcBuffer - buffer reeived from RLC interface
* Outputs        : None
* Returns        : None
* Description    : This function updates the stats for data being sent to
*                  PHY interface.
****************************************************************************/
/* +- SPR 17777 */
void lteMacUpdateStatsRlcRx(RLCDataBuffer * pRlcBuffer)
/* +- SPR 17777 */
{
    /* If stats memory is not mapped, just return */
    if (PNULL == gMacStats.pStats)
    {
        return;
    }

    /* No RLC data, then just return */
    if (PNULL == pRlcBuffer || PNULL == pRlcBuffer->newRlcPdu_p)
    {
        return;
    }

    /* update rlc RX stats */
    gMacStats.pStats->rlcStats.rxStats.numMsgs++;
    gMacStats.pStats->rlcStats.rxStats.numBytes += pRlcBuffer->newPduLen;
}

/****************************************************************************
* Function Name  : lteMacUpdateStatsRlcHarqFailure 
* Inputs         : 
*                  pRlcBuffer - buffer reeived from RLC interface
* Outputs        : None
* Returns        : None
* Description    : This function updates the stats for data being sent to
*                  PHY interface.
****************************************************************************/
/* +- SPR 17777 */
void lteMacUpdateStatsRlcHarqFailure(RLCHarqFailureInfo* pRlcHarqFailureInfo)
/* +- SPR 17777 */
{
    /* If stats memory is not mapped, just return */
    if (PNULL == gMacStats.pStats)
    {
        return;
    }

    if (PNULL == pRlcHarqFailureInfo)
    {
        return;
    }
    /* update rlc RX stats */
    gMacStats.pStats->rlcStats.numHarqFailures++;
}
/* ULA_CHG */

#ifdef SSI_DEBUG_STATS

/****************************************************************************
* Function Name  : lteMacUpdateStatsPuschPerTTIPerUe
* Inputs         : numScheduledUETotal,
*                  ueIndex,
*                  transmissionType,
*                  harqIndex,
*                  numRBsAllocated,
*                  startRBPosition,
*                  ulGrantRequestType,
*                  oppGranted,
*                  frequencySeletiveScheduling,
*                  selectedMCS,
*                  allocationFailureCause,
*                  tpc and
*                  muMimoStatsPerUe_p
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates the stats for data being sent to
*                  PHY interface on per TTI per UE basis.
****************************************************************************/
void lteMacUpdateStatsPuschPerTTIPerUe(UInt8 numScheduledUETotal,
        UInt16 ueIndex,
        UInt8 transmissionType,
        UInt8 harqIndex,
        UInt8 numRBsAllocated,
        UInt8 startRBPosition,
        UInt8 ulGrantRequestType,
        UInt32 oppGranted,
        UInt8 frequencySeletiveScheduling,
        UInt8 selectedMCS,
        UInt8 allocationFailureCause,
        SInt8 tpc
        /* UL_MU_MIMO_CHG_START */
        ,MuMimoStatsPerUe *muMimoStatsPerUe_p,
        /* UL_MU_MIMO_CHG_END */ 
        /* CA Stats Changes Start */
        InternalCellIndex internalCellIndex
        /* CA Stats Changes End */
        )
{
    LteMacPuschPerTTIPerUEStats *puschStatsPerScheduledUE_p = PNULL;        
        
    if (PNULL == gMacStats.pStats)
    {
        return;
    }

    puschStatsPerScheduledUE_p = &gMacStats.pStats->puschTTIStats[internalCellIndex]\
                                 [gMacStats.pStats->writeTTIIndex[internalCellIndex]].puschStatsPerScheduledUE[numScheduledUETotal];
    
    puschStatsPerScheduledUE_p->ueIndex = ueIndex; 
    puschStatsPerScheduledUE_p->transmissionType = transmissionType; 
    puschStatsPerScheduledUE_p->harqProcessId = harqIndex;
    puschStatsPerScheduledUE_p->allocatedPRBs = numRBsAllocated;
    puschStatsPerScheduledUE_p->startRBPosition = startRBPosition;
    puschStatsPerScheduledUE_p->ulGrantType = ulGrantRequestType;
    puschStatsPerScheduledUE_p->oppGranted = oppGranted;
    puschStatsPerScheduledUE_p->frequencySeletiveScheduling = frequencySeletiveScheduling;
    puschStatsPerScheduledUE_p->selectedMCS = selectedMCS;
    puschStatsPerScheduledUE_p->allocationFailureCause = allocationFailureCause;
    puschStatsPerScheduledUE_p->tpc = tpc;

    /* UL_MU_MIMO_CHG_START */
    puschStatsPerScheduledUE_p->ulMUMIMOStatus = muMimoStatsPerUe_p->ulMUMIMOStat;
    puschStatsPerScheduledUE_p->buddyUeIndex = muMimoStatsPerUe_p->buddyIndex;
    puschStatsPerScheduledUE_p->userLocationType = muMimoStatsPerUe_p->userLocType;
    puschStatsPerScheduledUE_p->nDmrs = muMimoStatsPerUe_p->nDmrsValue;
    puschStatsPerScheduledUE_p->modulationScheme = muMimoStatsPerUe_p->modScheme;
    /* UL_MU_MIMO_CHG_END */

/* + KPI_CA:  ue is attached to which cell? */
#if defined(UTFWK_SIMULATION) && defined(KPI_STATS)
    gMacStats.pStats->lteCommonKPIStats[internalCellIndex].lteMacKPIStats.totalUplinkBytesSent += oppGranted;
#endif
/* - KPI_CA */
}
/****************************************************************************
* Function Name  : lteMacUpdateStatsWriteTTIIndex
* Inputs         : internalCellIndex
* Outputs        : None
* Returns        : None
* Description    : This function updates the stats for data being sent to
*                  PHY interface.
****************************************************************************/
/* CA Stats Changes Start */
void lteMacUpdateStatsWriteTTIIndex(InternalCellIndex internalCellIndex)
{
    if (PNULL == gMacStats.pStats)
    {
        return;
    }
    gMacStats.pStats->writeTTIIndex[internalCellIndex]++;
    if(gMacStats.pStats->writeTTIIndex[internalCellIndex] == MAX_TTI_STATS)
    {
        gMacStats.pStats->writeTTIIndex[internalCellIndex] = 0;
    }
    gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].currentTickValue[ \
          WRITE_TTI_INDEX(internalCellIndex)] = getCurrentTick() + 1;\
    INITIALIZE_TTI_STATS(internalCellIndex);
}

/****************************************************************************
* Function Name  : lteMacResetPuschStats 
* Inputs         : internalCellIndex
* Outputs        : None
* Returns        : None
* Description    : This function resets the stats in PHY interface.
****************************************************************************/
void lteMacResetPuschStats(InternalCellIndex internalCellIndex)
{
    LteMacPuschTTIStats *puschTTIStats_p = PNULL;
    UInt32 idx;
    if (PNULL == gMacStats.pStats)
    {
        return;
    }
    
    puschTTIStats_p = &gMacStats.pStats->puschTTIStats[internalCellIndex]\
                      [gMacStats.pStats->writeTTIIndex[internalCellIndex]];
    /* CA Stats Changes End */
    
    puschTTIStats_p->absoluteTTI = 0;
    puschTTIStats_p->numScheduledUERetx = 0;
    puschTTIStats_p->numScheduledUENewTx = 0;
    puschTTIStats_p->numScheduledUETotal = 0;

    /*UL_MU_MIMO_CHG_START*/
    puschTTIStats_p->totalRbShared = 0;
    puschTTIStats_p->totalBuddyPairSuccess = 0;
    puschTTIStats_p->ulMuMimoTotalUeRetx = 0;
    for(idx = 0;idx < MAX_FAILURE_CAUSE;idx++)
    {
        puschTTIStats_p->buddyPairFailurePerCases[idx] = 0;
    }
    /*UL_MU_MIMO_CHG_END*/

}
/* called everytime ue is scheduled */

/****************************************************************************
* Function Name  : lteMacUpdateStatsUeCommonInfo
* Inputs         : ueIndex - Index of UE,
*                  ueIndexPos - Position of UE Index and
*                  currentAbsoluteTTI
* Outputs        : None
* Returns        : None
* Description    : This function updates the UE Common info.
****************************************************************************/
void lteMacUpdateStatsUeCommonInfo(
        /* +- SPR 18268 */
        UInt16 ueIndex,
        /* +- SPR 18268 */
        UInt16 ueIndexPos,
        UInt64 currentAbsoluteTTI
        )
{
    UEScheduledTTIInfo *puschScheduledTTIIndex_p = PNULL;
    if (PNULL == gMacStats.pStats)
    {
        return;
    }

    puschScheduledTTIIndex_p = &gMacStats.pStats->ueCommonInfoStats[ueIndex].puschScheduledTTIIndex
        [gMacStats.pStats->ueCommonInfoStats[ueIndex].puschCurWriteIndex];
    
    puschScheduledTTIIndex_p->ueIndexPos = ueIndexPos;

    /* absolute tti value stored each time ue is scheduled */
    puschScheduledTTIIndex_p->absoluteTTI = currentAbsoluteTTI;

    gMacStats.pStats->ueCommonInfoStats[ueIndex].puschCurWriteIndex++;
    
    if(gMacStats.pStats->ueCommonInfoStats[ueIndex].puschCurWriteIndex == MAX_TTI_STATS)
    {
        gMacStats.pStats->ueCommonInfoStats[ueIndex].puschCurWriteIndex = 0;
    }
}

/*****************************************************************************
* Function Name  : updateSchConfigStats
* Inputs         : initParams_p, internalCellIndex
* Outputs        : None
* Returns        : None
* Description    : This function updates the Single Channel Config stats.
*****************************************************************************/
/* CA Changes start */
/* CA Stats Changes Start */
 void updateSchConfigStats(InitParams *initParams_p,
        InternalCellIndex internalCellIndex)
{
    /* For DL */
    gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].tknPeriodicity = 
                                initParams_p->dlSchdConfig.dlTokenPeriodicity;
    gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].noOfDLSchUE = 
                                initParams_p->dlSchdConfig.maxUEsToBeScheduledDL;
    gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].pdbWt = 
                                initParams_p->dlSchdConfig.dlQosStrategyWeights.pdbWeight;
    gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].qciWt = 
                                initParams_p->dlSchdConfig.dlQosStrategyWeights.qciWeight;
    gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].tknWt = 
                              initParams_p->dlSchdConfig.dlQosStrategyWeights.tokenWeight;
    gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].qLoadWt = 
                              initParams_p->dlSchdConfig.dlQosStrategyWeights.qLoadWeight;
    gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].mcsWt = 
                              initParams_p->dlSchdConfig.dlQosStrategyWeights.mcsWeight;
    
    /* For UL */
    gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].ulTknPeriodicity = 
                             initParams_p->ulSchdConfig.ulTokenPeriodicity;
    gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].noOfULSchUE = 
                             initParams_p->ulSchdConfig.maxUEsToBeScheduledUL;
    gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].ulPdbWt = 
                             initParams_p->ulSchdConfig.ulQosStrategyWeights.pdbWeight;
    gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].ulQciWt = 
                             initParams_p->ulSchdConfig.ulQosStrategyWeights.qciWeight;
    gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].ulTknWt = 
                             initParams_p->ulSchdConfig.ulQosStrategyWeights.tokenWeight;
    gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].ulQLoadWt = 
                             initParams_p->ulSchdConfig.ulQosStrategyWeights.qLoadWeight;
    gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].ulMcsWt = 
                             initParams_p->ulSchdConfig.ulQosStrategyWeights.mcsWeight;
}

/* CA Changes end */
/****************************************************************************
* Function Name  : lteMacQosUpdateLCPerTTIStats
* Inputs         : qLoad - Total qLoad in bytes,
*                  ueIndex and lcId
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for Qload per TTI.
****************************************************************************/
void lteMacQosUpdateQloadLCPerTTIStats(UInt32 qLoad, 
                                       UInt16 ueIndex,
                                       UInt8 lcId,
                                       InternalCellIndex internalCellIndex)
{
    gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].qoSUELCStat[ueIndex].\
        qoSLCPerTTIStat[lcId].STX[WRITE_TTI_INDEX(internalCellIndex)].qLoad = qLoad;

    gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].qoSUELCStat[ueIndex].\
        qoSLCPerTTIStat[lcId].STX[WRITE_TTI_INDEX(internalCellIndex)].numDLMissPerTick = 
        gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].qoSUELCStat[ueIndex].\
        qoSLCPerTTIStat[lcId].numDLMiss;

}
/* CA Stats Changes End */

#ifdef UTFWK_SIMULATION

/****************************************************************************
* Function Name  : lteMacQosUpdateBsrStats
* Inputs         : BsrNet, ueIndex and currentTick
* Outputs        : None
* Returns        : None
* Description    : This function updates the Qos in BSR stats.
****************************************************************************/
void lteMacQosUpdateBsrStats(UInt32 BsrNet, UInt32 ueIndex, UInt64 currentTick)
{
    gMacStats.pStats->qoSUEAndPerLCTTIStat.qoSUELCStat[ueIndex].BsrNet[currentTick%MAX_TTI_STATS] = BsrNet;
}


/****************************************************************************
* Function Name  : lteMacQosUpdateLcgStats
* Inputs         : lcgIndex, ueIndex, currentTick and lcgLoad
* Outputs        : None
* Returns        : None
* Description    : This function updates the Qos in LCG stats.
****************************************************************************/
void lteMacQosUpdateLcgStats(UInt32 lcgIndex,UInt32 ueIndex, UInt64 currentTick,UInt32 lcgLoad)
{
    gMacStats.pStats->qoSUEAndPerLCTTIStat.qoSUELCStat[ueIndex].qoSLCGPerTTIStatsUl[lcgIndex].lcgInfo[currentTick%MAX_TTI_STATS].lcgLoad = lcgLoad;
}

/****************************************************************************
* Function Name  : lteMacQosUpdateLcStats
* Inputs         : lcIndex, ueIndex, currentTick and lcLoad
* Outputs        : None
* Returns        : None
* Description    : This function updates the Qos in LC stats.
****************************************************************************/
void lteMacQosUpdateLcStats(UInt32 lcIndex,UInt32 ueIndex, UInt64 currentTick,UInt32 lcQLoad)
{
    gMacStats.pStats->qoSUEAndPerLCTTIStat.qoSUELCStat[ueIndex].qoSLCPerTTIStatsUl[lcIndex].STX[currentTick%MAX_TTI_STATS].qLoad = lcQLoad;
    gMacStats.pStats->qoSUEAndPerLCTTIStat.qoSUELCStat[ueIndex].qoSLCPerTTIStatsUl[lcIndex].TotalLCOppGranted += lcQLoad;
}

/****************************************************************************
* Function Name  : lteMacQosUpdateGrantStats
* Inputs         : ueIndex, currentTick and grant
* Outputs        : None
* Returns        : None
* Description    : This function updates the Qos in Grant stats.
****************************************************************************/
void lteMacQosUpdateGrantStats(UInt32 ueIndex, UInt64 currentTick,UInt32 grant)
{
}

/****************************************************************************
* Function Name  : lteMacQosUpdateHarqStats
* Inputs         : ueIndex, currentTick and tmpHarqId
* Outputs        : None
* Returns        : None
* Description    : This function updates the Qos in HARQ stats.
****************************************************************************/
void lteMacQosUpdateHarqStats(UInt32 ueIndex,UInt64 currentTick,UInt32 tmpHarqId)
{
	gMacStats.pStats->qoSUEAndPerLCTTIStat.qoSUELCStat[ueIndex].harqId[currentTick%MAX_TTI_STATS] = tmpHarqId;
}

/****************************************************************************
* Function Name  : lteMacQosUpdateTotalLcLoad
* Inputs         : ueId, lcId and tempGrant
* Outputs        : None
* Returns        : None
* Description    : This function updates the Qos in Total LC Load.
****************************************************************************/
void lteMacQosUpdateTotalLcLoad(UInt32 ueId,UInt32 lcId,UInt32 tempGrant)
{
    if(tempGrant > gMacStats.pStats->qoSUEAndPerLCTTIStat.qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcId].TotalLCOppGranted)
    {
        gMacStats.pStats->qoSUEAndPerLCTTIStat.qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcId].TotalLCOppGranted = 0;
    }
    else
    {
        gMacStats.pStats->qoSUEAndPerLCTTIStat.qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcId].TotalLCOppGranted -= tempGrant;
    }

}
#endif

/*****************************************************************************
* Function Name  : updateDlQosPerTTIStats
* Inputs         : internalCellIndex
* Outputs        : None
* Returns        : None
* Description    : This function updates downlink Qos as per TTI Stats.
*****************************************************************************/
/* CA Stats Changes Start */
void updateDlQosPerTTIStats(InternalCellIndex internalCellIndex)
{
    UInt32 ueIndex = 0;
    UInt8 lcId = 0;
    /* SPR 15909 fix start */
    tickType_t currentTick = getCurrentTick();            
    /* SPR 15909 fix end */
    QoSLCPerTTIStat *qoSLCPerTTIStat_p = PNULL;

    for( ueIndex=0; ueIndex < gMacStats.pStats->numActiveUe[internalCellIndex]; ueIndex++)
    {
        DLUEContext *newUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
        if ( PNULL != newUEContext_p )
        {
            DLAmbrInfo *dlAmbrInfo_p = 
                        &dlUECtxInfoArr_g[ueIndex].dlUEContext_p->dlAmbrInfo;
            DlLcQosInfo *dlLcQosInfo_p = PNULL;
            DlLcQosInfo *dlLcQosAMBRInfo_p = PNULL;

            for ( lcId = MIN_LC_VALUE ; lcId < MAX_LCID ; lcId++)
            {
                qoSLCPerTTIStat_p =
                    &(gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].\
                               qoSUELCStat[ueIndex].qoSLCPerTTIStat[lcId]);
                if(newUEContext_p->logicalChannel[lcId]\
                        .logicalChannelId != INVALID_LC_ID)
                {
                    UInt8 channelType = dlUECtxInfoArr_g[ueIndex].\
                       dlUEContext_p->logicalChannel[lcId].dlLcQosInfo.channelType;
                    if ( QOS_GBR == channelType )
                    {
                        dlLcQosInfo_p = &dlUECtxInfoArr_g[ueIndex].\
                                  dlUEContext_p->logicalChannel[lcId].dlLcQosInfo;
                        qoSLCPerTTIStat_p->STX[WRITE_TTI_INDEX(internalCellIndex)].avaiableToken = 
                                  dlLcQosInfo_p->availToken;

                        if (PNULL != dlLcQosInfo_p->dlQosStrategyLCNode_p )
                        {
                            qoSLCPerTTIStat_p->STX[WRITE_TTI_INDEX(internalCellIndex)].LCWeight =
                             dlLcQosInfo_p->dlQosStrategyLCNode_p->totLCWeight;
                        }
                        if ( (currentTick != 0) && 
                             (newUEContext_p->logicalChannel[lcId].\
                                     pktArrivalTS != 0) )
                        {
                            qoSLCPerTTIStat_p->STX[WRITE_TTI_INDEX(internalCellIndex)].headOfLinePktDelay = 
                                ( currentTick - newUEContext_p->logicalChannel[lcId].pktArrivalTS);
                        }
                        qoSLCPerTTIStat_p->STX[WRITE_TTI_INDEX(internalCellIndex)].qciPriority = 
                            dlLcQosInfo_p->priority;
                        qoSLCPerTTIStat_p->STX[WRITE_TTI_INDEX(internalCellIndex)].pdbPriority = 
                            dlLcQosInfo_p->pdbPriority;
                        qoSLCPerTTIStat_p->STX[WRITE_TTI_INDEX(internalCellIndex)].qLoadPriority =
                            dlLcQosInfo_p->qLoadPriority;
                        qoSLCPerTTIStat_p->STX[WRITE_TTI_INDEX(internalCellIndex)].tokenPriority =
                            dlLcQosInfo_p->tokenPriority;
                        qoSLCPerTTIStat_p->STX[WRITE_TTI_INDEX(internalCellIndex)].totSchdTknInWindow = 
                            dlLcQosInfo_p->totSchdTknInWindow + \
                            dlLcQosInfo_p->scheduledToken[dlLcQosInfo_p->curTknWindowInd];
                        qoSLCPerTTIStat_p->expGrntT =  dlLcQosInfo_p->lcTknTable[2];
                    }
                    else if (NON_GBR == channelType)
                    {
                        dlLcQosAMBRInfo_p = &dlUECtxInfoArr_g[ueIndex].\
                               dlUEContext_p->logicalChannel[lcId].dlLcQosInfo;
                        qoSLCPerTTIStat_p->STX[WRITE_TTI_INDEX(internalCellIndex)].avaiableToken = 
                              dlAmbrInfo_p->availAmbrToken;
                        if (dlLcQosAMBRInfo_p->dlQosStrategyLCNode_p != PNULL)
                        {
                            qoSLCPerTTIStat_p->STX[WRITE_TTI_INDEX(internalCellIndex)].LCWeight = 
                                dlLcQosAMBRInfo_p->dlQosStrategyLCNode_p->totLCWeight;
                        }
                        if ( (currentTick != 0) && 
                             (newUEContext_p->logicalChannel[lcId].pktArrivalTS != 0))
                        {
                            qoSLCPerTTIStat_p->STX[WRITE_TTI_INDEX(internalCellIndex)].headOfLinePktDelay = 
                                currentTick - \
                                newUEContext_p->logicalChannel[lcId].pktArrivalTS;
                        }
                        qoSLCPerTTIStat_p->STX[WRITE_TTI_INDEX(internalCellIndex)].qciPriority = 
                             dlLcQosAMBRInfo_p->priority;
                        qoSLCPerTTIStat_p->STX[WRITE_TTI_INDEX(internalCellIndex)].pdbPriority = 
                             dlLcQosAMBRInfo_p->pdbPriority;
                        qoSLCPerTTIStat_p->STX[WRITE_TTI_INDEX(internalCellIndex)].qLoadPriority = 
                            dlLcQosAMBRInfo_p->qLoadPriority;
                        qoSLCPerTTIStat_p->STX[WRITE_TTI_INDEX(internalCellIndex)].tokenPriority = 
                            dlLcQosAMBRInfo_p->tokenPriority;
                        qoSLCPerTTIStat_p->STX[WRITE_TTI_INDEX(internalCellIndex)].totSchdTknInWindow =
                            dlAmbrInfo_p->totSchdTknInWindow + \
                            dlAmbrInfo_p->scheduledToken[dlLcQosAMBRInfo_p->curTknWindowInd];
                        qoSLCPerTTIStat_p->expGrntT =  dlAmbrInfo_p->lcTknTable[2];
                    }
                    lteMacQosUpdateQloadLCPerTTIStats( 
                        dlUECtxInfoArr_g[ueIndex].dlUEContext_p->logicalChannel[lcId].queueLoad,
                        ueIndex, lcId, internalCellIndex);
                }
                else
                {
                    lteMacQosUpdateQloadLCPerTTIStats(0,ueIndex,lcId, internalCellIndex);
                }
            }
            gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].qoSUELCStat[ueIndex].
                mcsIndexCodeWordOne[WRITE_TTI_INDEX(internalCellIndex)] = 
                newUEContext_p->dlCQIInfo.mcsIndexCodeWordOne;
        }
    }
}

/*****************************************************************************
* Function Name     : updateUlQosPerTTIStats
* Inputs            : internalCellIndex
* Outputs           : None
* Returns           : None
* Description       : This function updates Uplink Qos as per TTI Stats.
*****************************************************************************/
void updateUlQosPerTTIStats(InternalCellIndex internalCellIndex)
{
    /* +- SPR 18268 */
    UInt16 ueId = 0;
    /* +- SPR 18268 */
    UInt8 lcIndex = 0;
    UInt64 currentTick = getCurrentTick();
    UInt8 Lcid = 0;

    for( ueId=0; ueId < gMacStats.pStats->numActiveUe[internalCellIndex]; ueId++)
    {
        if (ulUECtxInfoArr_g[ueId].ulUEContext_p)
        {
            gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].qoSUELCStat[ueId].mcs =
                ulUECtxInfoArr_g[ueId].ulUEContext_p->mcsIndex;
            /* +COVERITY 27498 */
            for (lcIndex = MIN_LC_VALUE; lcIndex < MAX_LCID; ++lcIndex)
            /* -COVERITY 27498 */
            {
                if(ulUECtxInfoArr_g[ueId].ulUEContext_p->lcInfo[lcIndex].lchId != INVALID_LCID)
                {
                    gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].\
                        qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcIndex].STX[currentTick % MAX_TTI_STATS ].availLcToken =
                        ulUECtxInfoArr_g[ueId].ulUEContext_p->lcInfo[lcIndex].ulQosInfo.availLcToken;
                    if (ulUECtxInfoArr_g[ueId].ulUEContext_p->lcInfo[lcIndex].ulQosInfo.ulQosStrategyLCNode_p !=NULL)
                    {
                        gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].\
                            qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcIndex].STX[currentTick % MAX_TTI_STATS ].ulLcWt =
                            ulUECtxInfoArr_g[ueId].ulUEContext_p->lcInfo[lcIndex].ulQosInfo.ulQosStrategyLCNode_p->totLCWght;
                        gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].\
                            qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcIndex].STX[currentTick % MAX_TTI_STATS].qLoadPriority =
                            ulUECtxInfoArr_g[ueId].ulUEContext_p->lcInfo[lcIndex].ulQosInfo.qLoadPriority;
                        gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].\
                            qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcIndex].STX[currentTick % MAX_TTI_STATS].pdbPriority =
                            ulUECtxInfoArr_g[ueId].ulUEContext_p->lcInfo[lcIndex].ulQosInfo.pdbPriority;

                        Lcid = ulUECtxInfoArr_g[ueId].ulUEContext_p->lcInfo[lcIndex].lchId;

                        gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].\
                            qoSUELCStat[ueId].BsrNet[currentTick%MAX_TTI_STATS] = ulUECtxInfoArr_g[ueId].
                            ulUEContext_p->bsrNet[Lcid];
                        gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].\
                            qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcIndex].STX[currentTick % MAX_TTI_STATS].tokenPriority =
                            ulUECtxInfoArr_g[ueId].ulUEContext_p->lcInfo[lcIndex].ulQosInfo.tokenPriority;
                        gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].\
                            qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcIndex].STX[currentTick % MAX_TTI_STATS].
                            headOfLinePktDelay = ulUECtxInfoArr_g[ueId].ulUEContext_p->lcInfo[lcIndex].ulQosInfo.headOfLinePktDelay;
                        gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].\
                            qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcIndex].STX[currentTick % MAX_TTI_STATS].
                            totSchdTknInWindow = ulUECtxInfoArr_g[ueId].ulUEContext_p->lcInfo[lcIndex].ulQosInfo.totSchdTknInWindow
                            + ulUECtxInfoArr_g[ueId].ulUEContext_p->lcInfo[lcIndex].ulQosInfo.scheduledToken[ulUECtxInfoArr_g[ueId].
                            ulUEContext_p->lcInfo[lcIndex].ulQosInfo.curTknWindowInd];
                        gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].\
                            qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcIndex].STX[currentTick % MAX_TTI_STATS].expectedGrantT =
                            ulUECtxInfoArr_g[ueId].ulUEContext_p->lcInfo[lcIndex].ulQosInfo.lcTknTable[3];
                        /*QOS_PKT_DROP_CHANGES*/
                        gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].\
                            qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcIndex].STX[currentTick % MAX_TTI_STATS].
                            numULMissPerTick = gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcIndex].numULMiss;
                        /*QOS_PKT_DROP_CHANGES*/
                        /* +COVERITY 27498 */
                        gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].\
                            qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcIndex].STX[currentTick % MAX_TTI_STATS].qLoad =
                            ulUECtxInfoArr_g[ueId].ulUEContext_p->ulLCGInfo[ulUECtxInfoArr_g[ueId].ulUEContext_p->lcInfo[lcIndex].lcGId].qLoad;
                       /* -COVERITY 27498 */
                    }
                }
            }
            lcIndex = ulUECtxInfoArr_g[ueId].ulUEContext_p->ulAmbrInfo.lcId;
            gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].\
                qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcIndex].STX[currentTick % MAX_TTI_STATS ].availLcToken =
                ulUECtxInfoArr_g[ueId].ulUEContext_p->ulAmbrInfo.availAmbrToken;


           if (ulUECtxInfoArr_g[ueId].ulUEContext_p->ulAmbrInfo.ulQosStrategyLCNode_p !=NULL)
            {
                gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].\
                    qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcIndex].STX[currentTick % MAX_TTI_STATS ].ulLcWt =
                    ulUECtxInfoArr_g[ueId].ulUEContext_p->ulAmbrInfo.ulQosStrategyLCNode_p->totLCWght;
                gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].\
                    qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcIndex].STX[currentTick % MAX_TTI_STATS].qLoadPriority =
                    ulUECtxInfoArr_g[ueId].ulUEContext_p->ulAmbrInfo.qLoadPriority;
                gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].\
                    qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcIndex].STX[currentTick % MAX_TTI_STATS].pdbPriority =
                    ulUECtxInfoArr_g[ueId].ulUEContext_p->ulAmbrInfo.pdbPriority;
                gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].\
                    qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcIndex].STX[currentTick % MAX_TTI_STATS].tokenPriority =
                    ulUECtxInfoArr_g[ueId].ulUEContext_p->ulAmbrInfo.ambrTokenPriority;
                gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].\
                    qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcIndex].STX[currentTick % MAX_TTI_STATS].headOfLinePktDelay =
                    currentTick - ulUECtxInfoArr_g[ueId].ulUEContext_p->ulAmbrInfo.headOfLinePktDelay;
                gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].\
                    qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcIndex].STX[currentTick % MAX_TTI_STATS].totSchdTknInWindow =
                    ulUECtxInfoArr_g[ueId].ulUEContext_p->ulAmbrInfo.totSchdTknInWindow
                    + ulUECtxInfoArr_g[ueId].ulUEContext_p->ulAmbrInfo.scheduledToken[ulUECtxInfoArr_g[ueId].ulUEContext_p->ulAmbrInfo.curTknWindowInd];
                gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].\
                    qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcIndex].STX[currentTick % MAX_TTI_STATS].expectedGrantT =
                    ulUECtxInfoArr_g[ueId].ulUEContext_p->ulAmbrInfo.lcTknTable[3];

                /*QOS_PKT_DROP_CHANGES*/
                gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].\
                    qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcIndex].STX[currentTick % MAX_TTI_STATS].numULMissPerTick =
                    gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcIndex].numULMiss;
                /*QOS_PKT_DROP_CHANGES*/

                gMacStats.pStats->qoSUEAndPerLCTTIStat[internalCellIndex].\
                    qoSUELCStat[ueId].qoSLCPerTTIStatsUl[lcIndex].STX[currentTick % MAX_TTI_STATS].qLoad =
                    ulUECtxInfoArr_g[ueId].ulUEContext_p->ulLCGInfo[ulUECtxInfoArr_g[ueId].ulUEContext_p->lcInfo[lcIndex].lcGId].qLoad;
            }
        }
    }
}
/* CA Stats Changes End */
#ifdef UTFWK_SIMULATION
/****************************************************************************
* Function Name  : lteMacQosUpdateOppGrantedLCPerTTIStats
* Inputs         : rQLoad - Total qLoad in bytes,
*                  ueIndex - Index of UE,
*                  currentTick and lcId
* Outputs        : None
* Returns        : None
* Description    : This function updates stats for Qload per TTI.
****************************************************************************/
void lteMacQosUpdateOppGrantedLCPerTTIStats(UInt32 rQLoad, 
                                            UInt16 ueIndex,
                                            UInt64 currentTick,
                                            SInt8 lcId)
{
    gMacStats.pStats->qoSUEAndPerLCTTIStat.qoSUELCStat[ueIndex].qoSLCPerTTIStat[lcId].STX[currentTick%MAX_TTI_STATS].OppGranted = rQLoad;

    gMacStats.pStats->qoSUEAndPerLCTTIStat.qoSUELCStat[ueIndex].qoSLCPerTTIStat[lcId].TotalLCOppGranted += rQLoad;
    
    gMacStats.pStats->qoSUEAndPerLCTTIStat.qoSUELCStat[ueIndex].TotalUeOPPGranted += rQLoad;
}
#endif

#endif /*SSI_DEBUG_STATS*/



/* 32.425 Events code changes start*/
#ifdef KPI_STATS

/****************************************************************************
* Function Name  : lteMacUpdateKpiStatsIPThpDlOnAdd 
* Inputs         : ueIndex - Index of UE,
*                  pRlcBuffer - buffer reeived from RLC interface and
*                  lcCount - Count of LC
* Outputs        : None
* Returns        : None
* Description    : This function updates the T1 and T2 Timer for 
*                   the scheduledIPThroughputDL KPI Stats.
****************************************************************************/
void lteMacUpdateKpiStatsIPThpDlOnAdd(UInt16 ueIndex, RLCDataBuffer * pRlcBuffer, UInt8 lcCount)
{
    TransmissionAndRLCData* transmissionAndRLCData_p = PNULL;
    UInt8 index =     0;
    UInt8 lcId =      INVALID_LC_ID;
    UInt8 commonIndex  = 0;

    /* SPR  13888 fix start */
    UInt8 harqId = INVALID_HARQ_ID;
    DLHARQProcess* dlHarqProcess_p = PNULL;
    transmissionAndRLCData_p = (TransmissionAndRLCData *)pRlcBuffer;
    harqId = transmissionAndRLCData_p->harqProcessId;
    dlHarqProcess_p = &dlUECtxInfoArr_g[ueIndex].dlUEContext_p->dlHarqContext_p->harqProcess[harqId];
     /* SPR  13888 fix end */

    for ( index = 0 ; index < lcCount ; index ++)
    {
        lcId = transmissionAndRLCData_p->rlcDataBufferArr[index].lcID;
	/* SPR 13888 fix start */
        if ((lcId > 2) && ((transmissionAndRLCData_p->rlcDataBufferArr[index].newPduLen !=0) ||
                            (transmissionAndRLCData_p->rlcDataBufferArr[index].retxPduSeqNum != 0)))
        {
	    /* SPR 13888 fix end */
            lcId = lcId -3;
            commonIndex = scheduledIPThroughputDL_g [ueIndex][lcId].commonIndex;
            /* SPR 15932 fix start */
            /* Following check is added to ensure that timers t1, t1prev are not updated
             * twice in case of MIMO, when two Tbs are received .Without this check t1Prev 
             * will be updated by currentTick when this function will be called for 2nd TB */
            if(scheduledIPThroughputDL_g [ueIndex][lcId].t1[commonIndex] != getCurrentTick())
            {
                /* SPR 15932 fix end */
	    /* SPR 13888 fix start */
            if (( scheduledIPThroughputDL_g [ueIndex][lcId].t2[commonIndex] == 0) &&
                (transmissionAndRLCData_p->rlcDataBufferArr[index].newPduLen !=0))
            {
	        /* SPR 13888 fix end */
                /* Store the Time when first data of data burst is transmitted to Ue */
                scheduledIPThroughputDL_g [ueIndex][lcId].t2[commonIndex] = getCurrentTick();
                /* reset the pdcpAckBytes when data burst start  */
                scheduledIPThroughputDL_g [ueIndex][lcId].pdcpAckBytes[commonIndex] = 0;
            }
            /* Keep increment this time for every packet untill data burst is complete */
            /*SPR 11812 Fix Start*/
            scheduledIPThroughputDL_g [ueIndex][lcId].t1prev[commonIndex] =  scheduledIPThroughputDL_g [ueIndex][lcId].t1[commonIndex];
            /*SPR 11812 Fix End*/
            scheduledIPThroughputDL_g [ueIndex][lcId].t1[commonIndex] = getCurrentTick();
            /* SPR  13888 fix start */
            /* Store the common Index present in the global structure in the harq process */
            /* SPR 16613 fix start */
            dlHarqProcess_p->commonIndex[lcId] = scheduledIPThroughputDL_g [ueIndex][lcId].commonIndex;
            /* SPR 16613 fix end */
            /* SPR  13888 fix end */
                /* SPR 15932 fix start */
            }
            /* SPR 15932 fix end */
        }
    }
}

/****************************************************************************
* Function Name  : lteMacUpdateKpiStatsIPThpDlOnDelete 
* Inputs         : lcId and ueIndex 
* Outputs        : None
* Returns        : None
* Description    : This function updates the common Index for the
*                  the scheduledIPThroughputDL KPI Stats when Delete Trigger.
****************************************************************************/
void lteMacUpdateKpiStatsIPThpDlOnDelete(UInt8 lcId, UInt16 ueIndex)
{
    UInt8 commonIndex =0;
    UInt8 tempLcId = INVALID_LC_ID;


    if (lcId >2 )
    {
        tempLcId = lcId -3;
        commonIndex = scheduledIPThroughputDL_g [ueIndex][tempLcId].commonIndex;
        /* IP Latency implementation start*/
        updateIPLatencyKpiStats(commonIndex, ueIndex, lcId);
        /* IP Latency implementation end*/

        /* SPR 13888 fix start */
        /* The value of scheduledIPThpTimeDL should be updated in stats only
         * when a burst is encountered.And for the traffic to be considered
         * bursty, atleast two dl packets should be received followed by a silence period.
        */
        /* SPR 13888 fix end */

        if (scheduledIPThroughputDL_g [ueIndex][tempLcId ].t1[commonIndex] - 
                scheduledIPThroughputDL_g [ueIndex][tempLcId].t2[commonIndex] >0)
        {
            scheduledIPThroughputDL_g [ueIndex][tempLcId].commonIndex++;
            if(scheduledIPThroughputDL_g [ueIndex][tempLcId].commonIndex == 30)
                scheduledIPThroughputDL_g [ueIndex][tempLcId].commonIndex =0;

        }
        else
        {
            /* Reset the the T1 and T2 when data burst is 1 sec  */
            scheduledIPThroughputDL_g [ueIndex][tempLcId].t1[commonIndex] =0;
            scheduledIPThroughputDL_g [ueIndex][tempLcId].t2[commonIndex] = 0;
            /*SPR 11812 Fix Start*/
            scheduledIPThroughputDL_g [ueIndex][tempLcId].t1prev[commonIndex] = 0;
            /*SPR 11812 Fix End*/

        }
    }
}

/****************************************************************************
* Function Name  : lteMacUpdateKpiStatsIPThpDlOnEnQueue 
* Inputs         : lcId ,ueIndex, lcCountForKpiStats, transmissionAndRLCData_p
*                  and hProcess_p
* Outputs        : None
* Returns        : None
* Description    : This function updates the common Index and pdcpBytes
*                  from the Global Struct at Harq Process for the
*                  the scheduledIPThroughputDL KPI Statsr.
****************************************************************************/
void lteMacUpdateKpiStatsIPThpDlOnEnQueue (
        UInt8 lcId,
        UInt16 ueIndex, 
        UInt8 lcCountForKpiStats,  
        TransmissionAndRLCData * transmissionAndRLCData_p,
        DLHARQProcess * hProcess_p
        )
{
    UInt32 count                      = 0;
    UInt32 sduCount                    = 0;
    UInt8 commonIndex                  = 0;
    UInt8 tempLcId = INVALID_LC_ID;

    if (lcId > 2)
    {
        tempLcId = lcId -3;
        /* get the Common Index */
        /* SPR  13888 fix start */
        /* SPR 16613 fix start */
        commonIndex = hProcess_p->commonIndex[tempLcId];
        /* SPR 16613 fix end */
        /* SPR  13888 fix end */

        /* Check for the time of the start of the data burst */

        if ( scheduledIPThroughputDL_g [ueIndex][tempLcId].t2[commonIndex])
        {
           /* +- SPR  13888 fix start */
            sduCount =   transmissionAndRLCData_p->rlcDataBufferArr[lcCountForKpiStats].completeEncodedSduCount;
            for(; count < sduCount; count++ )
            {
                /* Store every packet of data burst in the harq process */
                hProcess_p->pdcpBytes[tempLcId] +=
                    transmissionAndRLCData_p->rlcDataBufferArr[lcCountForKpiStats].pdcpSduSize[count];
            }
        }
    }
}

/****************************************************************************
* Function Name  : lteMacUpdateKpiStatsIPThpDlOnHarqAck 
* Inputs         : ueIndex, dlHarqProcess_p and rcvTTI
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function updates the Shared Memory Time and
*                  Shared Memory Data for the
*                  the scheduledIPThroughputDL KPI Stats .
****************************************************************************/
/* + KPI_CA */
void lteMacUpdateKpiStatsIPThpDlOnHarqAck (
        UInt16 ueIndex,
        DLHARQProcess* dlHarqProcess_p,
        /* SPR 15909 fix start */
        /* +- SPR 17777 */
        tickType_t rcvTTI
        /* SPR 15909 fix end */
#ifdef TDD_CONFIG
        ,InternalCellIndex internalCellIndex
#endif
        )
/* +- SPR 17777 */
{
/*spr 19296 fix start*/
    UInt32 activeIndexForMacstats = (gMacStats.
            pStats->lteCommonUeKPIStats.activeIndexForMacThpStats);
    LteMacKPIThpStats * pLteMacKPIThpStats = &(gMacStats.
            pStats->lteCommonUeKPIStats.lteMacKPIThpStats[activeIndexForMacstats]);
/*spr 19296 fix end*/
/* - KPI_CA */
   /* SPR 15909 fix start */
    tickType_t t2 = 0;
    /*SPR 11812 Fix Start*/
    tickType_t t1Prev = 0;
   /* SPR 15909 fix end */
#ifdef TDD_CONFIG
    UInt8 t1PrevDlSubFrame = 0;
#endif
    /*SPR 11812 Fix End*/
    UInt8 tempLcId = INVALID_LC_ID;

    for (tempLcId = 3; tempLcId < MAX_LOGICAL_CHANNEL; tempLcId++)
    {
        /* Get the T1 and T2 from the harq Common Index *.
         * harq Common Index has been stored in the harq during enQueue of the data */
        /* SPR 16613 fix start */
        t2 = scheduledIPThroughputDL_g [ueIndex][tempLcId - 3].t2[dlHarqProcess_p->commonIndex[tempLcId-3]];
        /*SPR 11812 Fix Start*/
        t1Prev = scheduledIPThroughputDL_g [ueIndex][tempLcId - 3].t1prev[dlHarqProcess_p->commonIndex[tempLcId - 3]];
#ifdef TDD_CONFIG
        /* SPR  13888 fix start */
        t1PrevDlSubFrame = (t1Prev + DL_ADVANCE_TICK) % MAX_SUB_FRAME;
        /* SPR  13888 fix end */
#endif
        /*SPR 11812 Fix End*/

        /* (Rcv TTI > time t1(When  last packet of the data burst is received ) +
         * Delay of ACK ) && t2 (Time of the first packet of the data burst) */
        /* SPR  13888 fix start */
#ifdef FDD_CONFIG
        if ((rcvTTI  >= (t1Prev + PHY_DL_DELAY) + 4) &&  t2 && (dlHarqProcess_p->commonIndex[tempLcId-3] != 
                                       scheduledIPThroughputDL_g [ueIndex][tempLcId - 3].commonIndex))
        /*SPR 11812 Fix Start*/     
#elif TDD_CONFIG
        if ((rcvTTI  >= (t1Prev + DL_ADVANCE_TICK) + getDlAckNackOffset(t1PrevDlSubFrame,internalCellIndex))
                &&  t2 && (dlHarqProcess_p->commonIndex[tempLcId -3] != 
                                       scheduledIPThroughputDL_g [ueIndex][tempLcId - 3].commonIndex))
#endif
        /* SPR  13888 fix end */
        /*SPR 11812 Fix End*/
        {
            /* Store the Acknoledged Bytes in the shared Memory :
             * this will exclude the bytes of the last TTI of the data burst */
/*spr 19296 fix start*/
            pLteMacKPIThpStats->scheduledIPThpVolDL[ueIndex][tempLcId - 3] +=
                scheduledIPThroughputDL_g[ueIndex][tempLcId - 3].pdcpAckBytes[dlHarqProcess_p->commonIndex[tempLcId-3]];

            /* Store the Time  in the shared Memory :
             * this will be diffrence of the when first packet of the data burst is transmitted 
             * with the time when  second last of the packet of the data burst is transmitted */
            /* This will exclude the last TTI of the data burst */
            /*SPR 11812 Fix Start*/
            /* The last data packet is not to be included while calculating
             * scheduledIPThpTimeDL for traffic burst*/
            pLteMacKPIThpStats->scheduledIPThpTimeDL[ueIndex][tempLcId - 3] += t1Prev -t2;
            /*SPR 11812 Fix End*/
/*spr 19296 fix end*/
            
            /* Reset the T1 T2 */
            scheduledIPThroughputDL_g [ueIndex][tempLcId - 3].t1[dlHarqProcess_p->commonIndex[tempLcId-3]]=0;
            scheduledIPThroughputDL_g [ueIndex][tempLcId - 3].t2[dlHarqProcess_p->commonIndex[tempLcId -3]]=0;
            /*SPR 11812 Fix Start*/
            scheduledIPThroughputDL_g [ueIndex][tempLcId - 3].t1prev[dlHarqProcess_p->commonIndex[tempLcId -3]]=0;
            /*SPR 11812 Fix End*/
        }
        else
        { 
            if (dlHarqProcess_p->pdcpBytes[tempLcId - 3])
            {
                /* data burst is not complete : store the harq Pdcp Bytes into the global structure */ 
                scheduledIPThroughputDL_g [ueIndex][tempLcId  - 3].pdcpAckBytes[dlHarqProcess_p->commonIndex[tempLcId-3]] +=
                    dlHarqProcess_p->pdcpBytes[tempLcId - 3];
            }
        /* SPR 16613 fix end */
        }
    }
}
/****************************************************************************
* Function Name  : lteMacUpdateKpiStatsIPThpDlOnHarqNack
* Inputs         : commonIndex and ueIndex
* Outputs        : None
* Returns        : None
* Description    : This function reset the global T1 , t2 and PdcpAckBytes
*                  for the the scheduledIPThroughputDL KPI Stats .
****************************************************************************/
/* + SPR 17745 */
void lteMacUpdateKpiStatsIPThpDlOnHarqNack(DLHARQProcess* dlHarqProcess_p,
        UInt16 ueIndex, UInt32 rcvTTI,InternalCellIndex internalCellIndex )
{
    /*spr 19296 fix start*/
    /* SPR 20831 Changes Start */
    UInt32 activeIdx = gMacStats.pStats->lteCommonUeKPIStats.activeIndexForMacThpStats;
    LteMacKPIThpStats * pLteMacKPIThpStats = &(gMacStats.pStats->
            lteCommonUeKPIStats.lteMacKPIThpStats[activeIdx]);
    /* SPR 20831 Changes End */
    /*spr 19296 fix end*/
    UInt32 t2 = 0;
    UInt32 t1Prev = 0;
#ifdef TDD_CONFIG
    UInt8 t1PrevDlSubFrame = 0;
#endif
    UInt8 tempLcId = INVALID_LC_ID;

    for (tempLcId = 3; tempLcId < MAX_LOGICAL_CHANNEL; tempLcId++)
    {
        /* Get the T1 and T2 from the harq Common Index which has been stored
         * in the harq during enQueue of the data */
        t2 = scheduledIPThroughputDL_g [ueIndex][tempLcId - 3].t2[
            dlHarqProcess_p->commonIndex[tempLcId-3]];
        t1Prev = scheduledIPThroughputDL_g [ueIndex][tempLcId - 3].t1prev[
            dlHarqProcess_p->commonIndex[tempLcId - 3]];
#ifdef TDD_CONFIG
        t1PrevDlSubFrame = (t1Prev + DL_ADVANCE_TICK) % MAX_SUB_FRAME;
#endif
        /* (Rcv TTI > time t1(When  last packet of the data burst is received ) +
         * Delay of ACK ) && t2 (Time of the first packet of the data burst) */
#ifdef FDD_CONFIG
        if ( (rcvTTI  >= (t1Prev + PHY_DL_DELAY) + 4) &&  t2 &&
                (dlHarqProcess_p->commonIndex[tempLcId-3] !=
                 scheduledIPThroughputDL_g [ueIndex][tempLcId - 3].commonIndex))
#elif TDD_CONFIG
            if ( (rcvTTI  >= (t1Prev + DL_ADVANCE_TICK) +
                        getDlAckNackOffset(t1PrevDlSubFrame,internalCellIndex)) &&  t2 &&
                    (dlHarqProcess_p->commonIndex[tempLcId -3] !=
                     scheduledIPThroughputDL_g [ueIndex][tempLcId - 3].commonIndex))
#endif
            {
                /* Store the Acknoledged Bytes in the shared Memory :
                 * this will exclude the bytes of the last TTI of the data burst */
/*spr 19296 fix start*/
                pLteMacKPIThpStats->scheduledIPThpVolDL[ueIndex][tempLcId - 3] +=
                    scheduledIPThroughputDL_g[ueIndex][tempLcId - 3].pdcpAckBytes[
                    dlHarqProcess_p->commonIndex[tempLcId-3]];

                /* Store the Time  in the shared Memory which will be the
                 * difference of when first packet of the data burst is transmitted
                 * with the time when second last of the packet of the data burst
                 * is transmitted. This will exclude the last TTI of the data burst
                 * */
                pLteMacKPIThpStats->scheduledIPThpTimeDL[ueIndex][tempLcId - 3] +=
                    t1Prev -t2;
/*spr 19296 fix end*/
                /* Reset pdcpAckBytes,T1 and T2 at harq Common Index  */
                scheduledIPThroughputDL_g [ueIndex][tempLcId - 3].t1[
                    dlHarqProcess_p->commonIndex[tempLcId -3]]= 0;
                scheduledIPThroughputDL_g [ueIndex][tempLcId - 3].t2[
                    dlHarqProcess_p->commonIndex[tempLcId -3]] =0;
        /*SPR 11812 Fix Start*/
                scheduledIPThroughputDL_g [ueIndex][tempLcId - 3].t1prev[
                    dlHarqProcess_p->commonIndex[tempLcId -3]] =0;
        /*SPR 11812 Fix End*/
                scheduledIPThroughputDL_g [ueIndex][tempLcId - 3].pdcpAckBytes[
                    dlHarqProcess_p->commonIndex[tempLcId -3]] =0;
            }
    }
}
/* - SPR 17745 */

/* + SPR 17404 */
/****************************************************************************
 * Function Name  : macUpdateKPIForDelTrigger 
 * Inputs         : numLcForDelTrig : Number of LC for KPI to be considered
 *                  dlUeCtx_p : DL UE Context.
 *                  delTrigLCNodeForKpi: LC's for consideration
 * Outputs        : None
 * Returns        : void
 * Description    : This function updates the the KPI for Delete Trigger
 *                  based on queueload in each qci.
 ****************************************************************************/
void macUpdateKPIForDelTrigger( UInt8 numLcForDelTrig,
        DLUEContext* dlUeCtx_p, UInt8 *delTrigLCNodeForKpi)
{
    UInt8 qci          = 0;
    UInt8 lcId         = 0;
    UInt8 lcIndex      = 0;
    for ( lcIndex = 0; lcIndex < numLcForDelTrig ; lcIndex++)
    {
        lcId = delTrigLCNodeForKpi[lcIndex];
        if(lcId != INVALID_LCID) 
        {
            qci = dlUeCtx_p->logicalChannel[lcId].dlLcQosInfo.QCI;
            if((qci) && (qci <= MAX_QCI))
            {
                /* Burst is complete so update the KPI stats */
                lteMacUpdateKpiStatsIPThpDlOnDelete( lcId, 
                        dlUeCtx_p->ueIndex);
            }
        }
    }
}
/* - SPR 17404 */

/****************************************************************************
 * Function Name  : updateIPLatencyKpiStats
 * Inputs         : commonIndex, ueIndex and lcId
 * Outputs        : None
 * Returns        : None
 * Description    : This function update the states for IP Latency.
 * ****************************************************************************/
/* + KPI_CA */
void updateIPLatencyKpiStats(UInt8 commonIndex, UInt16 ueIndex, UInt8 lcId)
{
    /* SPR 15909 fix start */
    tickType_t pdcpTickVal = 0;
    tickType_t macTickVal = 0;
    /* SPR 15909 fix end */
    /* +- SPR 13888 fix */
/*spr 19296 fix start*/
    UInt32 activeIndexForMacStats = (gMacStats.
            pStats->lteCommonUeKPIStats.activeIndexForMacThpStats);
    LteMacKPIThpStats * pLteMacKPIThpStats = &(gMacStats.pStats->lteCommonUeKPIStats.
            lteMacKPIThpStats[activeIndexForMacStats]);
/*spr 19296 fix end*/
     /* - KPI_CA */
    pdcpTickVal = scheduledIPThroughputDL_g[ueIndex][lcId - 3].pdcpTick[commonIndex];
    macTickVal = scheduledIPThroughputDL_g [ueIndex][lcId - 3].t2[commonIndex];
    /* SPR 11864 fix */
    /* SPR 13888 fix start */
    if(pdcpTickVal != 0)
    {
/*spr 19296 fix start*/
        pLteMacKPIThpStats->sharedMemoryLatSample[ueIndex][lcId - 3]++;
        pLteMacKPIThpStats->SharedMemoryLatTime[ueIndex][lcId - 3] += (macTickVal - pdcpTickVal);
        scheduledIPThroughputDL_g[ueIndex][lcId - 3].pdcpTick[commonIndex] = 0;
/*spr 19296 fix end*/
        /* SPR 13888 fix end */
        /* SPR 11864 fix */
    }
}
/****************************************************************************
* Function Name  : lteMacUpdateKpiStatsIPThpUlOnEnqueue
* Inputs         : lcgIndex, ulLCGInfo_p, ueIndex and flag
*                  internalCellIndex - cell id  
* Outputs        : None
* Returns        : None
* Description    : This function will the EnQueue the Ue id and lcId in the Circular Queue
*                  for the the scheduledIPThroughputUL KPI Stats.
****************************************************************************/
/* + PERF_CA GBL */
void lteMacUpdateKpiStatsIPThpUlOnEnqueue (UInt32 lcgIndex ,ULLCGInfo *ulLCGInfo_p,
                                           UInt16 ueIndex,/* +- SPR 17404 */
										   InternalCellIndex internalCellIndex)
{
    KPIUlIpThpQueueNode *kpiUlIpThpQueue_p = PNULL;
    UInt8 counter = 0;
    UInt8 lcIndex = 0;
    UInt8 lcId    = INVALID_LC_ID;

    /* get the LC ID from the LCG Id */
    counter =  ulLCGInfo_p[lcgIndex].numLC;

    for (;lcIndex < MAX_LOGICAL_CHANNEL && counter;)
    {
        lcId = ulLCGInfo_p[lcgIndex].lcId[lcIndex];

        if ( lcId != INVALID_LCID)
        {
            if (lcId >2 )
            {
                lcId = lcId -3;
                GET_MEM_FROM_POOL(KPIUlIpThpQueueNode,kpiUlIpThpQueue_p,
                        sizeof(KPIUlIpThpQueueNode),PNULL);
                /*+COVERITY 5.3.0 - 32098*/
                if ( PNULL == kpiUlIpThpQueue_p )
                {
                    return;    
                }
                /*-COVERITY 5.3.0 - 32098*/
                kpiUlIpThpQueue_p->ueId = ueIndex;
                kpiUlIpThpQueue_p->lcId = lcId;
                kpiUlIpThpQueue_p->timeStamp = getCurrentTick();
                /* +- SPR 17404 */
                /* Insert UE ID, LC ID   into the Q */
                /* + Coverity 32428 */
				/* + KPI_CA GBL */
//              /*6.0 DL_UL_SPLIT Fix,  correcting the first parameter*/
                /* SPR 23483 Changes Start */
                if(CIRC_FAILURE == ENQUEUE_KPI_STATS_UL_IP_THP_Q(kpiUlIpThpQueue_p,internalCellIndex))
                /* SPR 23483 Changes End */
		        {
		           freeMemPool(kpiUlIpThpQueue_p);
		           return ;
		        }
				/* - KPI_CA GBL */
                /* - Coverity 32428 */
            }
        }
        lcIndex ++;
    }

}
#ifndef UE_SIM_TESTING

/****************************************************************************
* Function Name  : lteMacUpdateKpiStatsIPThpUlOnDequeue
* Inputs         : None 
* Outputs        : None
* Returns        : None
* Description    : This function will the DeQueue the Ue id and lcId from the Circular Queue
*                  for the the scheduledIPThroughputUL KPI Stats.
****************************************************************************/
/* + KPI_CA */
/* + SPR 17439 */
void lteMacUpdateKpiStatsIPThpUlOnDequeue (void)
/* - SPR 17439 */    
{
    /* + SPR 17404 */
    KPIUlIpThpQueueNode *kpiUlIpThpQueue_p = PNULL;
/*spr 19296 fix start*/
    UInt32 activeIndexForMacstats = (gMacStats.
            pStats->lteCommonUeKPIStats.activeIndexForMacThpStats);
    LteMacKPIThpStats * pLteMacKPIThpStats = &(gMacStats.
            pStats->lteCommonUeKPIStats.lteMacKPIThpStats[activeIndexForMacstats]);
/*spr 19296 fix end*/
/* - KPI_CA */
    UInt32 sQcount = 0;
    UInt16 ueIndex = 0;
    UInt8 lcId = INVALID_LC_ID ;
    /* + KPI_CA GBL */
	InternalCellIndex cellIndex = 0;
    /* Get the Queue Count */
	for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
    {
        sQcount = QUEUE_COUNT_KPI_STATS_UL_IP_THP_Q(cellIndex);
        while(sQcount --)
        {
            DEQUEUE_KPI_STATS_UL_IP_THP_Q (kpiUlIpThpQueue_p,cellIndex);
            if(kpiUlIpThpQueue_p)
            {
                ueIndex = kpiUlIpThpQueue_p->ueId;
                lcId = kpiUlIpThpQueue_p->lcId;
                /* Update the shared Memory */
                /*SPR 11812 Fix Start*/
                /* The value of scheduledIPThpTimeUL should be updated in stats
                 * only when a burst is encountered. And for the traffic to be
                 * considered bursty, atleast two ul packets should be received
                 * followed by a silence period */
                if ( scheduledIPThroughputUL_g[ueIndex][lcId].t1 - 
                        scheduledIPThroughputUL_g[ueIndex][lcId].t2 != 0)
                {
                    /*SPR 11812 Fix End*/
                    if ( scheduledIPThroughputUL_g[ueIndex][lcId].t1prev <= 
                            kpiUlIpThpQueue_p->timeStamp )
                    {
/*spr 19296 fix start*/
                        pLteMacKPIThpStats->scheduledIPThpTimeUL[ueIndex][lcId]
                            += scheduledIPThroughputUL_g[ueIndex][lcId].t1prev -
                            scheduledIPThroughputUL_g[ueIndex][lcId].t2 + 1 ;

                        pLteMacKPIThpStats->scheduledIPThpVolUL[ueIndex][lcId]
                            += scheduledIPThroughputUL_g[ueIndex][lcId].pdcpBytes -
                            scheduledIPThroughputUL_g[ueIndex][lcId].lastPdcpSduBytes;
/*spr 19296 fix end*/
                    }
                }

                /* Reset the Global Structure */
                scheduledIPThroughputUL_g[ueIndex][lcId].t1prev = 0;
                scheduledIPThroughputUL_g[ueIndex][lcId].t1 = 0;
                scheduledIPThroughputUL_g[ueIndex][lcId].t2 = 0;
                scheduledIPThroughputUL_g[ueIndex][lcId].pdcpBytes = 0;
                scheduledIPThroughputUL_g[ueIndex][lcId].lastPdcpSduBytes = 0;

                /* Free the node */
                freeMemPool((void *)kpiUlIpThpQueue_p);
                kpiUlIpThpQueue_p = PNULL;
            }
        }
    }
    /* - SPR 17404 */
}

/****************************************************************************
* Function Name  : lteMacUpdateKpiStatsIPThpUlOnDataArrival
* Inputs         : ueIndex, lcId, size, pktTimeStamp 
* Outputs        : None
* Returns        : None
* Description    : This function will the DeQueue the Ue id and lcId from the Circular Queue
*                  for the the scheduledIPThroughputUL KPI Stats.
****************************************************************************/
/* SPR 15909 fix start */
void lteMacUpdateKpiStatsIPThpUlOnDataArrival( UInt16 ueIndex , UInt16 lcId, UInt32 size, tickType_t pktTimeStamp)
/* SPR 15909 fix end  */
{
    if (scheduledIPThroughputUL_g[ueIndex][lcId].t2)
    {
        if ( pktTimeStamp >= scheduledIPThroughputUL_g[ueIndex][lcId].t1 )
        {
            lteMacUpdateKpiStatsIPThpUlOnDequeue();

            /* store the previous packet Time stamp */
            scheduledIPThroughputUL_g[ueIndex][lcId].t1prev =
                scheduledIPThroughputUL_g[ueIndex][lcId].t1;

            /* Store the Current packet Time stamp which will increase per Packet */
            scheduledIPThroughputUL_g[ueIndex][lcId].t1 =
                pktTimeStamp;

            if (!scheduledIPThroughputUL_g[ueIndex][lcId].t2)
            {
                /* Store the time when first packet of the data burst has been received */
                scheduledIPThroughputUL_g[ueIndex][lcId].t2 = pktTimeStamp ;
            }
            /* Accumlate pdcpSDU Bytes for the data burst */
            scheduledIPThroughputUL_g[ueIndex][lcId].pdcpBytes +=  size;

            /* Store the last pdcp SDU Bytes for every Packet in the data burst */
            scheduledIPThroughputUL_g[ueIndex][lcId].lastPdcpSduBytes = size;



        }
    }

    if (!scheduledIPThroughputUL_g[ueIndex][lcId].t2)
    {
        /* Store the time when first packet of the data burst has been received */
        scheduledIPThroughputUL_g[ueIndex][lcId].t2 = pktTimeStamp ;

        /* Store the Current packet Time stamp which will increase per Packet */
        scheduledIPThroughputUL_g[ueIndex][lcId].t1 =
            pktTimeStamp;
        /* Accumlate pdcpSDU Bytes for the data burst */
        scheduledIPThroughputUL_g[ueIndex][lcId].pdcpBytes +=  size;

        /* Store the last pdcp SDU Bytes for every Packet in the data burst */
        scheduledIPThroughputUL_g[ueIndex][lcId].lastPdcpSduBytes = size;



    }
}
#endif
#endif

/* 32.425 Events code changes end*/
