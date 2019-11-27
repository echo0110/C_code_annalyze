
/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2011 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacPhyCellReconfig.c
 *
 ******************************************************************************
 *
 *  File Description : This file provide APIs for the mac phy Cell Reconfig 
 *                     updates.
 *****************************************************************************/
#include "lteMacPhyCellReconfig.h"
#include "lteMacTypes.h"
#include "lteMacParseUtil.h"
#include "lteMacFAPIApi.h"
#include "lteMacPhyInterface.h"
#include "lteMacCellConfMgr.h"
#include "lteMacULPowerControlData.h"
#include "lteMacPRSMgr.h"
#include "lteMacDLResourceInfoMgr.h"
/* Global Vairables */
/* CA Changes start */
extern CellConfigPhy *cellConfigPhy_gp[MAX_NUM_CELL];
/* This structure is used to hold PHY reconfig Parameters */
ReconfigPhyCell reconfigPhyCell_g[MAX_NUM_CELL] = {{0}};

/* This structure is used to hold Mac reconfig Parameters */
ReconfigMacCell reconfigMacCell_g[MAX_NUM_CELL] = {{0}};

/* CA Changes end */
/*SPR 16855 +-*/

/*****************************************************************************
 * Function Name : processCellReconfigUpdate
 * Inputs        : Delay to Phy
 *                 dlDelay  - The amount of delay expected in the DownLink 
 *                 internalCellIndex -  Cell-Index at MAC
 * Outputs       : Cell reconfiguration applicable from the next tick. 
 * Returns       : None
 * Description   : This funtion checks if any Cell reconfig update is present 
 *                 and updates appropriate global structures.
 *                 The check for reconfiguration is made at target SFN/SF - 1
 *                 tick, so that the updated parameters are applicable from 
 *                 the next tick.  
 *****************************************************************************/
/* CA Changes start */
void   processCellReconfigUpdate(UInt32 delayToPhy,InternalCellIndex internalCellIndex)
{
    UInt32 tgtSfn;
    UInt32 tgtSf;
    ReconfigPdschConfigCommonInfo *reconfigPdschConfigCommonInfo = PNULL;
     /* +- SPR 19066 */
    ContFreeRachTimeVal *contFreeRachTimer_p = PNULL;
    SyncSignal *syncSignal_p = PNULL;

    CellConfigParams *cellParams_p =  cellSpecificParams_g.\
                       cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
    /* Advance the global tick to sync with Phy's time which is the target
     * SFN/SF 
     */
    /*CA Changes start  */
    getSFAndSFN(&tgtSfn , &tgtSf, internalCellIndex);
    /*CA Changes end  */
    tgtSf += delayToPhy+1;

    if (tgtSf >= MAX_SUB_FRAME)
    {
        tgtSf -= MAX_SUB_FRAME;
        tgtSfn++;

        if (tgtSfn == MAX_SFN_VALUE)
        {
            tgtSfn = 0;
        }
    } 

    /* Reconfig update should be applied at 1 tick before the 
     * target SFN/SF
     */
    /* Check for Phy Reconfig Updates */
    if ( (tgtSfn == reconfigPhyCell_g[internalCellIndex].targetSfn) &&
            (tgtSf == reconfigPhyCell_g[internalCellIndex].targetSf)
       )
    {
        if (FAPI_TRUE == reconfigPhyCell_g[internalCellIndex].reconfigUpdate)
        {
            /* Update the parameters */
            reconfigPdschConfigCommonInfo = &reconfigPhyCell_g[internalCellIndex].\
					    reconfigPdschConfigCommonInfo;

            if (TRUE == reconfigPdschConfigCommonInfo->updateFlag)
            {
                /*CLPC change*/
                cellConfigPhy_gp[internalCellIndex]->rfConfig.refSignalPower = 
                               reconfigPdschConfigCommonInfo->refSignalPower;
                cellConfigPhy_gp[internalCellIndex]->pb = 
			            reconfigPdschConfigCommonInfo->pB;

                reconfigPdschConfigCommonInfo->updateFlag = FALSE;
            }

            ReconfigCommonDLPControlInfoPhy *reconFDlPow_p = 
		    &(reconfigPhyCell_g[internalCellIndex].\
				         reconfigCommonDLPowerControlInfo);
            if (TRUE == reconFDlPow_p->updateFlagPcfich)
            {
                cellConfigPhy_gp[internalCellIndex]->pcfichPowerOffset = 
			                 reconFDlPow_p->pcfichPowerOffset;
                reconFDlPow_p->updateFlagPcfich  = FALSE;
            }
            if (TRUE == reconFDlPow_p->updateFlagPhich)
            {
                cellConfigPhy_gp[internalCellIndex]->phichConfig.phichPowOffset = 
			                        reconFDlPow_p->phichPowerOffset;
                reconFDlPow_p->updateFlagPhich = FALSE;
            }
            syncSignal_p = &reconfigPhyCell_g[internalCellIndex].syncSignal;
            if (TRUE == syncSignal_p->updatePrimarySignalFlag)
            {
                cellConfigPhy_gp[internalCellIndex]->schConfig.primarySyncSignal = 
			                            syncSignal_p->primarySignal;
                syncSignal_p->updatePrimarySignalFlag = FALSE;
            }

            if (TRUE == syncSignal_p->updateSecondarySignalFlag)
            {
                cellConfigPhy_gp[internalCellIndex]->schConfig.secondarySyncSignal 
			                        = syncSignal_p->secondarySignal;
                syncSignal_p->updateSecondarySignalFlag = FALSE;
            }

            LOG_MAC_MSG(RRC_PHY_CELL_RECONFIG_REQ_ID,LOGWARNING,
                    MAC_RRC_INF,
                    getCurrentTick(),
                    reconfigPdschConfigCommonInfo->refSignalPower, 
		    PHY_RECONFIG_CELL_CNF, tgtSfn,
                    tgtSf,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,
		    "PHY_RECONFIG_CELL_REQ: Applying Reconfig At MAC");
        }
        else if (FAPI_PENDING_RESPONSE == 
			reconfigPhyCell_g[internalCellIndex].reconfigUpdate)
        {
            /* Send Fail Response to L3 */
            if( MAC_SEND_ERROR == sendPhyCellRespToRRC(PHY_RECONFIG_CELL_CNF,
                        reconfigPhyCell_g[internalCellIndex].transactionId, 
			 MAC_FAILURE, macCommonGetRRCCellIndex(internalCellIndex))
              )
            {
                ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID,
                        MAJOR_ALARM);
            }
            else
            {
                LOG_MAC_MSG(RRC_PHY_CELL_RECONFIG_REQ_ID,LOGWARNING,
                        MAC_RRC_INF,
                        getCurrentTick(),
                        RRC_MODULE_ID, PHY_RECONFIG_CELL_CNF, 
			DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        FUNCTION_NAME,"PHY_RECONFIG_CELL_REQ: Sent Fail To RRC");
            }
        }

        /* Reinitialize the flags */
        reconfigPhyCell_g[internalCellIndex].reconfigUpdate = FAPI_FALSE;
    }

    /* Check for Mac Reconfig Updates */
    if (reconfigMacCell_g[internalCellIndex].reconfigUpdate > 0)
    {

        /* GAP_VALUE_FIX_START */
        if(reconfigMacCell_g[internalCellIndex].gapValue)
        {
            cellParams_p->gapValue = reconfigMacCell_g[internalCellIndex].gapValue;
            reconfigMacCell_g[internalCellIndex].gapValue = 0;
        }
        /* GAP_VALUE_FIX_END */

        /* Check for updated parameter */
       /* +- SPR 19066 */ 
        contFreeRachTimer_p = &reconfigMacCell_g[internalCellIndex].\
			                         contFreeRachTimer; 
        if (TRUE == contFreeRachTimer_p->updateFlag)
        {
            if ( (tgtSfn == contFreeRachTimer_p->targetSfn) &&
                    (tgtSf  == contFreeRachTimer_p->targetSf)
               )
            {
                contFreeRachTimer_p->updateFlag = FALSE;
                reconfigMacCell_g[internalCellIndex].reconfigUpdate--;
                cellParams_p->contnFreeRachTimer = 
			     contFreeRachTimer_p->contFreeRachTimer;
            }
        }

        /*CLPC Change*/
        PDSCHReConfigCommonInfo *pdschReconfigCommonInfo_p = 
            &(reconfigMacCell_g[internalCellIndex].reconfigPdschConfigCommonInfo);
        if (TRUE == pdschReconfigCommonInfo_p->updateFlag)
        {
            if ((tgtSfn == pdschReconfigCommonInfo_p->targetSfn) &&
                    (tgtSf  == pdschReconfigCommonInfo_p->targetSf))
            {
                pdschReconfigCommonInfo_p->updateFlag = FALSE;
                reconfigMacCell_g[internalCellIndex].reconfigUpdate--;
                cellParams_p->pdschConfigCommonInfo.referenceSigPower = 
                    pdschReconfigCommonInfo_p->referenceSigPower;
                cellParams_p->pdschConfigCommonInfo.pB = 
			                pdschReconfigCommonInfo_p->pB;
            }
        }

        ReconfigCommonDLPControlInfoMac *reconfigCommonDLPControlInfoMac_p = 
            &(reconfigMacCell_g[internalCellIndex].reconfigCommonDLPControlInfoMac);
        if(TRUE == reconfigCommonDLPControlInfoMac_p->updateFlag)
        {
            if ((tgtSfn == reconfigCommonDLPControlInfoMac_p->targetSfn) &&
                    (tgtSf  == reconfigCommonDLPControlInfoMac_p->targetSf))
            {
                reconfigCommonDLPControlInfoMac_p->updateFlag = FALSE;
                reconfigMacCell_g[internalCellIndex].reconfigUpdate--;
                cellParams_p->commonDLPowerControlInfo.pcfichPowerOffset =
                    reconfigCommonDLPControlInfoMac_p->pcfichPowerOffset;
                cellParams_p->commonDLPowerControlInfo.phichPowerOffset = 
                    reconfigCommonDLPControlInfoMac_p->phichPowerOffset;
                cellParams_p->commonDLPowerControlInfo.pdcchPowerOffset =
                    reconfigCommonDLPControlInfoMac_p->pdcchPowerOffset;
                cellParams_p->commonDLPowerControlInfo.pbchTransmissionPower =
                    reconfigCommonDLPControlInfoMac_p->pbchTransmissionPower;
                cellParams_p->commonDLPowerControlInfo.pchTransmissionPower =
                    reconfigCommonDLPControlInfoMac_p->pchTransmissionPower;
            }
        }

        ReconfigCommonULPowerControlInfo *reconfig_p = 
            &(reconfigMacCell_g[internalCellIndex].reconfigCommonULPowerControlInfo);
        if(TRUE == reconfig_p->updateFlag)
        {
            if ((tgtSfn == reconfig_p->targetSfn) &&
                    (tgtSf  == reconfig_p->targetSf))
            {
                reconfig_p->updateFlag = FALSE;
                reconfigMacCell_g[internalCellIndex].reconfigUpdate--;
                cellParams_p->commonULPowerControlInfo.p0NominalPUSCH = 
                    reconfig_p->p0NominalPuschValue;
                cellParams_p->commonULPowerControlInfo.alpha = 
                    reconfig_p->alpha;
                if(0 == cellParams_p->commonULPowerControlInfo.alpha)
                {
                    /* ICIC changes start */
                    macPathlossToTargetSINRMapForPUSCH_p[internalCellIndex] = 
                        /*CA Changes start  */
                        (macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])-> \
                        macPathlossToTargetSINRMapForPUSCH[0];
                        /*CA Changes end  */
                    /* ICIC changes end */
                }
                else
                {
     			/* ICIC changes start */
                    macPathlossToTargetSINRMapForPUSCH_p[internalCellIndex] = 
		       (macReconfigSchedulerParamsUL_gp[internalCellIndex] + schParamsIndexUL_g[internalCellIndex])->\
		       macPathlossToTargetSINRMapForPUSCH[cellParams_p->\
		                         commonULPowerControlInfo.alpha - 3];
                    /* ICIC changes end */

                }

            }
        }
        /* SPS_CHG */
        /* Reconfig Cell for SPS is not supported */
        /* SPS_CHG */
        /*SPR 3600 Start*/
        ReconfigPuschConfigCommon *reconfigPuschConfigCommon_p =
            &(reconfigMacCell_g[internalCellIndex].reconfigPuschConfigCommon);
        if(TRUE == reconfigPuschConfigCommon_p->updateFlag)
        {
            if ((tgtSfn == reconfigPuschConfigCommon_p->targetSfn) &&
                    (tgtSf  == reconfigPuschConfigCommon_p->targetSf))
            {
                reconfigPuschConfigCommon_p->updateFlag = FALSE;
                reconfigMacCell_g[internalCellIndex].reconfigUpdate--;
                cellParams_p->macEnable64QAM =
		       	reconfigPuschConfigCommon_p->reconfigEnable64Qam;
            }
        }
        /*SPR 3600 End*/

        /* + PRS_CHANGES */

        /* If PRS is configured then saving paramters in 
         * cellParams_p->and calculating required parameters.
         */   
        PRSParseConfigInfo *prsParseConfigInfo_p = 
		&(reconfigMacCell_g[internalCellIndex].prsParseConfigInfo);

        UInt8 dlAvailableRBs = 0;
        UInt8 requiredRBs = 0;
        UInt8  *rbToResourceTypeMapDL_p = PNULL;
        UInt8 *prsReserveRBMap_p = PNULL;
	UInt8 dlPRSRATPoliciesIndex = cellParams_p->\
				      prsConfigInfo.dlPRSRATPoliciesIndex; 
        PRSResInfo *prsResInfo_p = &cellParams_p->prsConfigInfo.\
				       prsResInfo[dlPRSRATPoliciesIndex];
        if (TRUE == prsParseConfigInfo_p->updateFlag)
        {
            prsParseConfigInfo_p->updateFlag = FALSE;
            reconfigMacCell_g[internalCellIndex].reconfigUpdate--;
            if (cellParams_p->prsConfigInfo.isPRSConfigured)
            {
                if(0 !=  prsParseConfigInfo_p->isPRSConfigured)
                {
                    reconfigurePRSConfig(prsParseConfigInfo_p, internalCellIndex);
                }
                else
                {
                    cellParams_p->prsConfigInfo.isPRSConfigured = 0;
                    prsResInfo_p->prsAllowedWithICIC = 0;
                    cellParams_p->prsConfigInfo.prsSubframeCounter = 0;
                    /* + SPR_8520 */
                    cleanupPRSMgr(cellParams_p->prsConfigInfo.dlPRSMapIndex, 
                       internalCellIndex);
                    /* - SPR_8520 */
                }

            }
            else if (0 != prsParseConfigInfo_p->isPRSConfigured)
            { 
                cellParams_p->prsConfigInfo.isPRSConfigured = 1;
                prsResInfo_p->prsAllowedWithICIC = 1;
                cellParams_p->prsConfigInfo.prsBandwidth = 
                    prsParseConfigInfo_p->prsBandwidth;
                cellParams_p->prsConfigInfo.prsSubframes =
                    prsParseConfigInfo_p->prsSubframes;
                cellParams_p->prsConfigInfo.prsTransmissionPower = 
                    prsParseConfigInfo_p->prsTransmissionPower;

                initPRSMaps(*prsParseConfigInfo_p,cellParams_p->dlBandwidth, 
                              internalCellIndex);

                if (prsResInfo_p->startPRB != prsResInfo_p->endPRB)
                {

                    if (cellParams_p->prsConfigInfo.icicConfigured)
                    {
                        dlAvailableRBs  = cellParams_p->dlAvailableRBs;  
                        rbToResourceTypeMapDL_p = 
                            (dlRAInfo_gp[internalCellIndex] + dlRAInfoIndex_g[internalCellIndex])\
                                             ->rbToResourceTypeMapDL;
                        prsReserveRBMap_p = prsResInfo_p->prsReserveRBMap; 
                        CHECK_REMAINING_RES_WITH_ICIC_PRS_ON(dlAvailableRBs,requiredRBs,
                                rbToResourceTypeMapDL_p,prsReserveRBMap_p);

                        if (requiredRBs ==  MAX_RES_REQUIRED_TO_SCHEDULE_SI_PAGING)
                        { 	

                            initPRSRATPoliciesBitmaps((
                                 &((dlRAInfo_gp[internalCellIndex] + dlPRSRATPoliciesIndex)->\
                                            raPolicyInfoPRSReserved)),dlPRSRATPoliciesIndex,
                                            internalCellIndex);
                            initPRSRATPoliciesBitmaps(
                                 &((dlRAInfo_gp[internalCellIndex] + (!dlPRSRATPoliciesIndex))->\
                                        raPolicyInfoPRSReserved),dlPRSRATPoliciesIndex,
                                        internalCellIndex);
                        }
                        else
                        {
                            prsResInfo_p->prsAllowedWithICIC = FALSE;
                        }
                    }
                    else 
                    {
                        initPRSRATPoliciesBitmaps(
                               &((dlRAInfo_gp[internalCellIndex] + dlPRSRATPoliciesIndex)->\
                                    raPolicyInfoPRSReserved),dlPRSRATPoliciesIndex,
                                    internalCellIndex);
                        initPRSRATPoliciesBitmaps(
                            &((dlRAInfo_gp[internalCellIndex] + (!dlPRSRATPoliciesIndex))->\
                                    raPolicyInfoPRSReserved), dlPRSRATPoliciesIndex,
                                    internalCellIndex);
                    }
                }
            }
            else
            {
                cellParams_p->prsConfigInfo.isPRSConfigured = 0;
            }
        } 
    }     
    /* - PRS_CHANGES */
}
/* CA Changes end */
