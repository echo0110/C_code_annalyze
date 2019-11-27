/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteRelayS1Sap.c,v 1.2 2010/02/25 08:53:52 gur19479 Exp $
 *
 ******************************************************************************
 *
 *  File Description : S1 SAP Interface Implementation 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRelayS1Sap.c,v $
 * Revision 1.2  2010/02/25 08:53:52  gur19479
 * updated for transid array
 *
 * Revision 1.1.1.1  2010/02/11 04:51:36  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1.2.1  2009/11/25 09:57:08  cm_lteenodeb
 * code checkin
 *
 *
 *****************************************************************************/

/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/


/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteRelay.h"
#include "lteLog.h"
#include "lteRelayS1Sap.h"
#include "lteRelayCommon.h"
#include "lteMsgPool.h"
#include "egtpu_cdb.h"
#ifdef ENDC_ENABLED
#include "lteX2uRelay.c"
#endif
/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

static UInt32 allocateTransId(void);
/******************************************************************************
 * Exported Variables
 *****************************************************************************/
extern UInt8 udpSendToTG(UInt8 *respMsg, UInt16 current, Char8 interfaceType);
extern UInt32 handleGtpuSetupSapReq(relaySetupSapReq *setupSapReq_p, UInt32);
extern UInt32 handleGtpuReleaseSapReq(gtpuRelSapReq *relSapReq_p);
extern void pdcpSourceDataFwdReq(void* data_p, UInt16 size 
        /*SPR 21366 +*/
        ,UInt8 instanceId
        /*SPR 21366 -*/

        );
/* Intra-eNB HO Changes Start */
extern void pdcpSourceDataFwdReqInIntraeNB( UInt16 oldUeIndex, UInt16 newUeIndex
        /*SPR 21366 +*/
        ,UInt8 instanceId
        /*SPR 21366 -*/

        );
static inline void resetOldUeMapBasedOnUEIndexLcId_g(UInt16 ueIndex, UInt8 lcIndex);
static inline void s1uUpdateUeIndex(UInt16 ueIndex, UInt16 newUeIndex);
/* Intra-eNB HO Changes End */
/* warning removed*/
extern UInt16 getUeIdxFromRrcUeIdx(UInt16 rrcUeIdx);

/******************************************************************************
 * Global Variables
 *****************************************************************************/
UInt32 isDelayReq_g = RELAY_GTPU_NOT_IN_USE;
extern UInt8 gtpu_Pdcp_Instance_g[MAX_RRC_CELL_INDEX];
/****************************************************************************
 * Functions implementation
 ****************************************************************************/

/****************************************************************************
* Function Name  : allocateTransId
* Inputs         : None
* Outputs        : None
* Returns        : transaction id
* Description    : This funtion return unique transaction id.
****************************************************************************/

static UInt32 allocateTransId(void)
{
    static UInt32 transactionId;
   /* SPR 16425 fix start */
   UInt32 idx = 0;

   for (idx = 0; idx < RELAY_GTPU_MAX_TRANS_ID; idx++)
   {
       /*Cov 88254 + */
       transactionId++;
       if (RELAY_GTPU_MAX_TRANS_ID == transactionId)
       {
           transactionId = 0;
       }
       if( RELAY_GTPU_IN_USE != 
               relayMapBasedOnTransId_g[transactionId].inUseFlag )
       {
           break;
       }
       /*Cov 88254 - */
   }
   /* No failure expected because the transaction id is available for MAX 
    * bearers */
   if (idx == RELAY_GTPU_MAX_TRANS_ID)
   {
       /* Should not happen */
        LOG_GTPU_MSG(PR_ALL_TRANSID_USED, LOGFATAL, PR_GTPU_CONTROL,
                relayGtpuCurrentTick_g, __LINE__, RELAY_GTPU_MAX_TRANS_ID, 0, 
                0, 0, 0,0, __func__, "" );
   }
   /* SPR 16425 fix end */

    return transactionId;
}

/****************************************************************************
* Function Name  : updateTunnelIdInReconfigCnf
* Inputs         : ueIndex - UE ID, 
*                  lcId - Logical channel ID,
*                  sapFlagValue - Flag value of SAP,
*                  TunnelId
* Outputs        : None
* Returns        : None
* Description    : This function poplulates tunnelId(Normal/DL/UL) on the 
*                  of sap falg value recieved from S1-AP.
****************************************************************************/
void updateTunnelIdInReconfigCnf( UInt16 ueIndex, UInt8 lcIndex, 
                                  UInt8 sapFlagValue, UInt32 tunnelId )
{
    if (0 == (sapFlagValue & 0xF0))
    {
        s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].teid_self = tunnelId;
    }
    else if (1 == ((sapFlagValue & 0xF0) >> 5))
    {
        s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].teid_selfDlFwd = tunnelId;
    }
    else if (3 == ((sapFlagValue & 0xF0) >> 5))
    {
        s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].teid_selfUlFwd = tunnelId;
    }
#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
	else if (S1U_SN_TERMINTED_SPLIT_BEARER_SAP_FLAG == sapFlagValue)
	{
		s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].teid_self = tunnelId;
          s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].isScgTunnelPresentFlag = 0;

	}
#endif

    else
    {
        LOG_GTPU_MSG(PR_SETUP_SAP_REQ_SAP_INFO_INV, LOGWARNING, PR_GTPU_CONTROL,
                relayGtpuCurrentTick_g, __LINE__, ueIndex, lcIndex + 3, 
                sapFlagValue,0,0,0,__func__,"");
    }
}

/***********************************************************************************
* Function Name  : updateRelayMapTableBasedOnTransId
* Inputs         : ueIndex - UE ID,
*                  lcId - Logical channel ID,
*                  transIdForRsp - Trnsaction ID for response,
*                  apiIdForRsp - API ID for response,
*                  pTransId - Pointer to transaction ID,
*                  sapFlagValue - Flag value of SAP,
*                  pIsTunnelIdFlagTrue - Pointer to tunnel ID flag value,
*                  pPrevTidIndex - Pointer to prev TID index,
*                  modifyFlag - Modify Flag
* Outputs        : None
* Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE 
* Description    : This funtion populates tunnelType on the basis of sapFlagValue
*                  as input.
**********************************************************************************/
/* +- SPR 17777 */
UInt8 updateRelayMapTableBasedOnTransId(
         UInt16 ueIndex,UInt8 lcId,UInt32 transIdForRsp, UInt16 apiIdForRsp,
         UInt32 *pTransId,
         UInt8 sapFlagValue,UInt8 *pIsTunnelIdFlagTrue,
         UInt32 *pPrevTidIndex,
         UInt8 modifyFlag)
/* +- SPR 17777 */
{
    /*SPR 2952 Changes Start*/   
    if(!modifyFlag)
    {
        /* Allocate Transaction Id for a tunnel */
        *pTransId = allocateTransId();
#ifdef ENDC_ENABLED
		if ((sapFlagValue == S1U_SN_TERMINTED_SPLIT_BEARER_SAP_FLAG) &&
		        (relayMapBasedOnUeIndexLcId_g[ueIndex][lcId - 3].tunnelId  != INVALID_TUNNEL_ID))
		{
		    relayMapBasedOnUeIndexLcId_g[ueIndex][lcId - 3].dcTransId = *pTransId;
		}

#endif
        relayMapBasedOnUeIndexLcId_g[ueIndex][lcId - 3].transId = *pTransId;
        /*SPR 3736 Fix Start*/
        relayMapBasedOnTransId_g[*pTransId].inUseFlag 
            = RELAY_GTPU_IN_USE;
        relayMapBasedOnTransId_g[*pTransId].ueIndex 
            = ueIndex;
        relayMapBasedOnTransId_g[*pTransId].lcId 
            = lcId;
        /* Since Tunnel Id is unknown currently so setting it to
           default value */
        relayMapBasedOnTransId_g[*pTransId].tunnelId 
            = RELAY_GTPU_TUNNEL_ID_INVALID;
        /*SPR 3736 Fix End*/
    }
    /*SPR 2952 Changes End*/   

    /* store the SAP flag value in gloval structure*/
    relayMapBasedOnTransId_g[*pTransId].sapFlags = sapFlagValue;
    /* +- SPR 19660 */
    /* Check the tunnel type */
    if (0 == (sapFlagValue & 0xF0))
    {
        relayMapBasedOnTransId_g[*pTransId].tunnelType = NORMAL_BEARER;
        /* 08032014 604 HO Changes */
        relayMapBasedOnUeIndexLcId_g[ueIndex][lcId - 3].normalBearerTransId = *pTransId;
        /* 08032014 604 HO Changes */
        s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcId-3].isNormalTunnelPresentFlag = 1;
    }
    else if (1 == ((sapFlagValue & 0xF0) >> 5))
    {
        relayMapBasedOnTransId_g[*pTransId].tunnelType = DL_FORWARDING;
        s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcId-3].isDlTunnelPresentFlag = 1;
    }
    else if (3 == ((sapFlagValue & 0xF0) >> 5))
    {
        relayMapBasedOnTransId_g[*pTransId].tunnelType = UL_FORWARDING;
        s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcId-3].isUlTunnelPresentFlag = 1;
    }
#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
	else if (S1U_SN_TERMINTED_SPLIT_BEARER_SAP_FLAG == sapFlagValue)
	{
		relayMapBasedOnTransId_g[*pTransId].tunnelType = SCG_SPLIT_BEARER;
		s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcId-3].isScgTunnelPresentFlag = 1;
	}
#endif

    else
    {
        LOG_GTPU_MSG(PR_SETUP_SAP_REQ_SAP_INFO_INV, LOGWARNING, PR_GTPU_CONTROL,
                relayGtpuCurrentTick_g, __LINE__, ueIndex, lcId,
                sapFlagValue,0,0,0,__func__,"");
    }
    /*SPR 3736 Fix Start*/
    /*SPR 3736 Fix End*/

    /* Allocating Memory on index transId for pointer of array 
       relayUeCnxtMngmntForTransId_g */
    if (*pIsTunnelIdFlagTrue)
    {
        s1ReconfigSapCnf_g.ueIndex = relayMapBasedOnUeIndexLcId_g[ueIndex][lcId - 3].rrcUeIndex; 
        s1ReconfigSapCnf_g.transIdForRsp = transIdForRsp;
        s1ReconfigSapCnf_g.apiIdForRsp = apiIdForRsp;

        /* Holding transId index */
        *pIsTunnelIdFlagTrue = RELAY_GTPU_TUNNEL_ID_INVALID;
        *pPrevTidIndex = *pTransId;
    }
    return RELAY_GTPU_SUCCESS;
	/* - Coverity 73069 */
}

/*******************************************************************************
* Function Name  : updateRelayUeCntxMngmntBasedOnTunnelId
* Inputs         : pRelSapReq - Pointer to relayRelSapReq,
*                  lcIndex - Logical channel index,
*                  transIdForRsp - Transaction ID for response,
*                  apiIdForRsp - API ID for response,
*                  ueIndex - UE ID,
*                  teIdCount - Count of TE ID,
*                  startVal
* Outputs        : None
* Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
* Description    : This funtion populates tunnelInfo on the basis of tunnelId.
*******************************************************************************/
UInt8 updateRelayUeCntxMngmntBasedOnTunnelId(relayRelSapReq *pRelSapReq,
      UInt8 lcIndex,UInt32 transIdForRsp,UInt16 apiIdForRsp,UInt16 ueIndex,
      UInt8 teIdCount,UInt8 startVal)
{
    /* Index for TunnelId */
    if (INVALID_TUNNEL_ID !=
            relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelId)
    {
        pRelSapReq->teidSelf.tunnelId = 
            relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelId;
        relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelId =
            INVALID_TUNNEL_ID;
    }
    else if (INVALID_TUNNEL_ID !=
            relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelDlFwdId) 
    {
        pRelSapReq->teidSelf.tunnelId =
            relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelDlFwdId;
        relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelDlFwdId =
            INVALID_TUNNEL_ID;
    }                               
    else if (INVALID_TUNNEL_ID !=
            relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelUlFwdId)
    {
        pRelSapReq->teidSelf.tunnelId =
            relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelUlFwdId;
        relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelUlFwdId =
            INVALID_TUNNEL_ID;
    }

    else
    {
        LOG_GTPU_MSG(PR_SAP_REL_TUNNEL_ID_INV, LOGWARNING, 
                PR_GTPU_CONTROL, relayGtpuCurrentTick_g, __LINE__, 
                ueIndex, 0 ,0,0,0,0,__func__,"");
        return RELAY_GTPU_SUCCESS; 
    }

    s1ReconfigSapCnf_g.transIdForRsp = transIdForRsp;
    s1ReconfigSapCnf_g.apiIdForRsp     = apiIdForRsp;


    s1ReconfigSapCnf_g.ueIndex = relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].rrcUeIndex;
    if ((startVal+1) == teIdCount)
        isDelayReq_g = RELAY_GTPU_IN_USE;


    if (RELAY_GTPU_SUCCESS 
            != handleGtpuReleaseSapReq((gtpuRelSapReq*)pRelSapReq))
    { 
        LOG_GTPU_MSG(PR_GTPU_SAP_REL_FAILURE, LOGWARNING, 
                PR_GTPU_CONTROL, relayGtpuCurrentTick_g, __LINE__, 
                ueIndex, 0,0,0,0,0,__func__,"");
        isDelayReq_g = RELAY_GTPU_IN_USE;
        return RELAY_GTPU_FAILURE;
    }
    return RELAY_GTPU_SUCCESS;
}

/*******************************************************************************
* Function Name  : updateUeCntxMngmntBasedOnTunnelIdForReconfigSapRel
* Inputs         : pRelSapReq - Pointer to release SAP request,
*                  transId - Trnsaction ID,
*                  apiIdForRsp - API ID for response,
*                  ueIndex - UE ID,
*                  tunnelId - Tunnel ID                  
* Outputs        : None
* Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
* Description    : This funtion populates tunnelInfo on the basis of tunnelId.
*******************************************************************************/
/* +- SPR 17777 */
UInt8 updateUeCntxMngmntBasedOnTunnelIdForReconfigSapRel(
   relayRelSap *pRelSapReq,UInt32 transId,UInt16 apiIdForRsp,
        UInt16 ueIndex)
/* +- SPR 17777 */
{

    s1ReconfigSapCnf_g.transIdForRsp = transId;
    s1ReconfigSapCnf_g.apiIdForRsp = apiIdForRsp;
    if (RELAY_GTPU_SUCCESS 
            != handleGtpuReleaseSapReq((gtpuRelSapReq*)pRelSapReq))
    { 
        LOG_GTPU_MSG(PR_GTPU_SAP_REL_FAILURE, LOGWARNING, 
                PR_GTPU_CONTROL, relayGtpuCurrentTick_g, __LINE__, 
                ueIndex, 0,0,0,0,0,__func__,"");
        return RELAY_GTPU_FAILURE;
    }
    return RELAY_GTPU_SUCCESS;

}   

/****************************************************************************
 * Function Name  : processCreateUeRequest
 * Inputs         : setupSapReq_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process SAP UE Create setup request.
 ****************************************************************************/
UInt32 processCreateUeRequest(UInt8 *sapReq_p, UInt16 msgLen)
{
    UInt8 respMsg[20] = {0};
    /* SPR 6060 Start */
    UInt8  currentPos = 0, current = 0;   
    /* SPR 6060 End */

    UInt16 transId = 0, apiId = 0 , ueIndex , rrcUeIndex ; 

    /*  + Layer2 NON CA Changes */
    RrcCellIndex cellIndex;
    /*  - Layer2 NON CA Changes */  


    if (PNULL != sapReq_p)
    {   
        /* + coverity 56028 */
        /*  + Layer2 NON CA Changes */ 
        cellIndex = sapReq_p[API_HEADER_CELL_INDEX_OFFSET];
        /*  - Layer2 NON CA Changes */  
        /* - coverity 56028 */

        /* SPR 6060 Start */
        currentPos = API_HDR + 2; 
        /* SPR 6060 End */

        /* SPR 6060 Start */
        if ((currentPos < msgLen) && (S1U_IE_RELAY_SETUP_SAP_REQ == sapReq_p[++currentPos]))
            /* SPR 6060 End */
        {
            if (RELAY_GTPU_FAILURE == processGtpuSetupSapReq(sapReq_p, msgLen))
            {
                LOG_GTPU_MSG(PR_SETUP_SAP_REQ_FAILURE, LOGDEBUG, 
                        PR_GTPU_CONTROL, relayGtpuCurrentTick_g, __LINE__, 
                        0, 0,0,0,0,0,__func__,"");
                return RELAY_GTPU_FAILURE;
            }
        }
        else
        {
            /* Case when tunnels are not created during S1U_CREATE_UE_REQ,
             * (non-HO scenario) */
            transId = RELAY_GTPU_GET_U16BIT(&sapReq_p[current]);
            current = API_HDR;        
            rrcUeIndex = RELAY_GTPU_GET_U16BIT(&sapReq_p[current]);
            s1ReconfigSapCnf_g.ueIndex = rrcUeIndex;
            /* SPR 4558 Fix Start */
            ueIndex = getUeIdxFromRrcUeIdx(rrcUeIndex);
            /* + SPR 12979 */
#if  defined PDCP_GTPU_INTF
            if (INVALID_UE_ID == ueIndex)
            {
                ueIndex = allocateUeIdxFromRrcUeIdx( rrcUeIndex );
                /* + coverity 106461 */
                if( INVALID_UE_ID == ueIndex )
                {
                    return RELAY_GTPU_FAILURE; 
                }
                /* - coverity 106461 */
            }
#endif
            setUeIdxValid( ueIndex, UE_IDX_MAPPING_FOR_S1U );
            /* - SPR 12979 */
            /* SPR 4558 Fix End */
            s1uUEReconfigCnfInfo_g[ueIndex].cellIndex = cellIndex;

            LOG_GTPU_MSG(PR_RCVD_S1U_CREATE_UE_ENTITY_REQ, LOGINFO, 
                    PR_GTPU_CONTROL, relayGtpuCurrentTick_g, __LINE__, 
                    ueIndex, 0,0,0,0,0,__func__,"");

            apiId = S1U_CREATE_UE_ENTITY_CNF;

            RELAY_GTPU_SET_U16BIT(&respMsg[current], rrcUeIndex);

            current += 2;
            /*SPR #2340 fix Start*/
            respMsg[current] = GTPU_CAUSE_SUCCESS;  /* Cause */
            /*SPR #2340 fix End*/
            current++;

            /*SPR #2340 fix Start*/
            LOG_GTPU_MSG(PR_SEND_S1U_CREATE_UE_ENTITY_CNF, LOGINFO, 
                    PR_GTPU_CONTROL, relayGtpuCurrentTick_g, __LINE__, 
                    ueIndex, GTPU_CAUSE_SUCCESS,0,0,0,0,__func__,"");
            /*SPR #2340 fix End*/

            /* Append API Header in front of message for sending to S1-AP */

            /*  + Layer2 NON CA Changes */
            appendApiHeader(RELAY_S1AP_MODULE_ID, apiId, transId,
                    current,respMsg, s1uUEReconfigCnfInfo_g[ueIndex].cellIndex);
            /*  - Layer2 NON CA Changes */  
            /* Sending to S1-AP */
            udpSendToS1ap((Char8*)respMsg, current);

        }
        return RELAY_GTPU_SUCCESS;
    }    
    LOG_GTPU_MSG(PR_SETUP_SAP_REQ_EMPTY, LOGWARNING, PR_GTPU_CONTROL,
            relayGtpuCurrentTick_g,__LINE__,0,0,0,0,
            0,0,__func__,"");

    return RELAY_GTPU_FAILURE;
}

/* Intra-eNB HO Changes Start */
/****************************************************************************
 * Function Name  : processS1uIntraEnbDataFwdReq
 * Inputs         : setupSapReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process Intra ENB Data Fwd request.
 ****************************************************************************/
UInt32 processS1uIntraEnbDataFwdReq(UInt8 *intraDataFwdReq_p, UInt16 msgLen)
{
    UInt8 respMsg[20] = {0};
    UInt8 currentPos = API_HDR, current = 0;
    UInt16 transId = 0, apiId = 0 , ueIndex , newUeIndex ;
    UInt16 rrcUeIndex = INVALID_RRC_UE_INDEX;
    UInt16 rrcNewUeIndex = INVALID_RRC_UE_INDEX;

    if (PNULL != intraDataFwdReq_p)
    {
        transId = RELAY_GTPU_GET_U16BIT(&intraDataFwdReq_p[current]);

        /* Set the value to start of payload */
        current = API_HDR;

        /* Get the old UE index */
        rrcUeIndex = RELAY_GTPU_GET_U16BIT(&intraDataFwdReq_p[current]);
        current += 2;

        /* Get the UE index from RRX UE id */
        ueIndex = getUeIdxFromRrcUeIdx(rrcUeIndex);

        /* Get the new UE index */
        rrcNewUeIndex = RELAY_GTPU_GET_U16BIT(&intraDataFwdReq_p[current]);
        current += 2;
        /* Get the New UE index from RRC UE id */
        /*SPR 21366 +*/
        newUeIndex = getUeIdxFromRrcUeIdx(rrcNewUeIndex);
        egtpu_rab_ctxt_t *p_rab_ctxt = egtpu_db_sel_teid_key_rab_ctxt(p_global,
                relayMapBasedOnUeIndexLcId_g[ueIndex][0].tunnelId);
        if(PNULL != p_rab_ctxt)
        {
            pdcpSourceDataFwdReqInIntraeNB(ueIndex, newUeIndex
                    ,gtpu_Pdcp_Instance_g[p_rab_ctxt->teid_cell.cellIndex]);
        }
        /*SPR 21366 -*/

        /* +- SPR 17777 */
        LOG_GTPU_MSG(PR_RCVD_S1U_INTRA_ENB_DATA_FWD_REQ, LOGINFO,
                PR_GTPU_CONTROL, relayGtpuCurrentTick_g, __LINE__,
                ueIndex, newUeIndex,msgLen,0,0,0,__func__,"");

        apiId = RRC_S1U_INTRA_ENB_DATA_FWD_CNF;
        RELAY_GTPU_SET_U16BIT(&respMsg[currentPos],rrcUeIndex);
        currentPos += 2;
        respMsg[currentPos] = GTPU_CAUSE_SUCCESS;  /* Cause */
        currentPos++;

        /* Append API Header in front of message for sending to S1-AP */
        appendApiHeader(RELAY_S1AP_MODULE_ID, apiId, transId, currentPos, respMsg, 
              s1uUEReconfigCnfInfo_g[ueIndex].cellIndex);

        /* Sending to S1-AP */
        udpSendToS1ap((Char8 *)respMsg, currentPos);

        return RELAY_GTPU_SUCCESS;
    }
    LOG_GTPU_MSG(PR_RCVD_S1U_INTRA_ENB_DATA_FWD_REQ_EMPTY, LOGWARNING, PR_GTPU_CONTROL,
            relayGtpuCurrentTick_g,__LINE__,0,0,0,0,
            0,0,__func__,"");

    return RELAY_GTPU_FAILURE;
}

/****************************************************************************
 * Function Name  : processS1uLocalPathSwitchReq 
 * Inputs         : setupSapReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process Local Path Switch request.
 ****************************************************************************/
UInt32 processS1uLocalPathSwitchReq(UInt8 *localPathSwitchReq_p, UInt16 msgLen)
{
    UInt8 respMsg[20] = {0};
    UInt8 currentPos = API_HDR, current = 0;
    UInt16 transId = 0, apiId = 0 , tgtUeIndex , srcUeIndex ;
    UInt16 rrcSrcUeIndex = INVALID_RRC_UE_INDEX;
    UInt16 rrcTgtUeIndex = INVALID_RRC_UE_INDEX;

    if (PNULL != localPathSwitchReq_p)
    {    
        transId = RELAY_GTPU_GET_U16BIT(&localPathSwitchReq_p[current]);
        current = API_HDR;
        rrcTgtUeIndex = RELAY_GTPU_GET_U16BIT(&localPathSwitchReq_p[current]);
        current += 2;
        /* Get the Target UE index from RRC UE id */
        /*SPR 21366 +*/
        tgtUeIndex = getUeIdxFromRrcUeIdx(rrcTgtUeIndex);
        rrcSrcUeIndex = RELAY_GTPU_GET_U16BIT(&localPathSwitchReq_p[current]);
        current += 2;
        /* Get the Source UE index from RRC UE id */
        srcUeIndex = getUeIdxFromRrcUeIdx(rrcSrcUeIndex);
        /*SPR 21366 -*/
        /* Coverity 78968 Fix Start */
        if((srcUeIndex >= RELAY_GTPU_MAX_UE) || (tgtUeIndex >= RELAY_GTPU_MAX_UE))
        {
            return RELAY_GTPU_FAILURE;
        }
        /* Coverity 78968 Fix End */

        if (srcUeIndex == tgtUeIndex)
        {
            /*SPR 21366 +*/
            egtpu_rab_ctxt_t *p_rab_ctxt = egtpu_db_sel_teid_key_rab_ctxt(p_global,
                    relayMapBasedOnUeIndexLcId_g[srcUeIndex][0].tunnelId);
            if(PNULL != p_rab_ctxt)
            {
                pdcpSourceDataFwdReqInIntraeNB(srcUeIndex, tgtUeIndex
                        ,gtpu_Pdcp_Instance_g[p_rab_ctxt->teid_cell.cellIndex] );
            }
            /*SPR 21366 -*/
        }
        else
        {
            s1uUpdateUeIndex(srcUeIndex, tgtUeIndex);
        }
        /* +- SPR 17777 */
        LOG_GTPU_MSG(PR_RCVD_S1U_LOCAL_PATH_SWITCH_REQ, LOGINFO, 
                PR_GTPU_CONTROL, relayGtpuCurrentTick_g, __LINE__, 
                srcUeIndex, tgtUeIndex,msgLen,0,0,0,__func__,"");

        apiId = RRC_S1U_LOCAL_PATH_SWITCH_RESP;
        RELAY_GTPU_SET_U16BIT(&respMsg[currentPos], rrcTgtUeIndex);
        currentPos += 2;
        respMsg[currentPos] = GTPU_CAUSE_SUCCESS;  /* Cause */
        currentPos++;

        /* Append API Header in front of message for sending to S1-AP */
        appendApiHeader(RELAY_S1AP_MODULE_ID, apiId, transId, currentPos, respMsg, 
            s1uUEReconfigCnfInfo_g[tgtUeIndex].cellIndex);

        /* Sending to S1-AP */
        udpSendToS1ap((Char8 *)respMsg, currentPos);

        return RELAY_GTPU_SUCCESS;
    }
    LOG_GTPU_MSG(PR_RCVD_S1U_LOCAL_PATH_SWITCH_REQ_EMPTY, LOGWARNING, PR_GTPU_CONTROL,
            relayGtpuCurrentTick_g,__LINE__,0,0,0,0,
            0,0,__func__,"");

    return RELAY_GTPU_FAILURE;
}

/****************************************************************************
 * Function Name  : s1uUpdateUeIndex 
 * Inputs         : srcUeIndex- Is the Index of the UE for which Intra ENB Ho ongoing
 *                  tgtUeIndex- is the index received from upper layer which will replace the ueIndex
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion Update the Ue Index in Maps
 ****************************************************************************/
static inline void s1uUpdateUeIndex(UInt16 srcUeIndex, UInt16 tgtUeIndex)
{
    UInt8 lcIndex = 0;
    UInt32 transId  = 0; 

    for (lcIndex = 0; lcIndex < RELAY_GTPU_MAX_LC; lcIndex++)
    {
        if (RELAY_GTPU_IN_USE == relayMapBasedOnUeIndexLcId_g[srcUeIndex][lcIndex].inUseFlag)

        {
            transId = relayMapBasedOnUeIndexLcId_g[srcUeIndex][lcIndex].normalBearerTransId;
            relayMapBasedOnUeIndexLcId_g[tgtUeIndex][lcIndex].normalBearerTransId = 
                relayMapBasedOnUeIndexLcId_g[srcUeIndex][lcIndex].normalBearerTransId;
            /* Updating sourceUEIndex with targetUEIndex */
            relayMapBasedOnTransId_g[transId].ueIndex = tgtUeIndex;

            /* Updating Map Table for Target UE/LC */
            relayMapBasedOnUeIndexLcId_g[tgtUeIndex][lcIndex].inUseFlag = 
                relayMapBasedOnUeIndexLcId_g[srcUeIndex][lcIndex].inUseFlag;
            relayMapBasedOnUeIndexLcId_g[tgtUeIndex][lcIndex].numOfsapMsg = 
                relayMapBasedOnUeIndexLcId_g[srcUeIndex][lcIndex].numOfsapMsg;
            relayMapBasedOnUeIndexLcId_g[tgtUeIndex][lcIndex].EmiFlag = 
                relayMapBasedOnUeIndexLcId_g[srcUeIndex][lcIndex].EmiFlag;
            relayMapBasedOnUeIndexLcId_g[tgtUeIndex][lcIndex].isTargetENB = 
                relayMapBasedOnUeIndexLcId_g[srcUeIndex][lcIndex].isTargetENB;
            relayMapBasedOnUeIndexLcId_g[tgtUeIndex][lcIndex].isLocalEndMarkerIndRecvd = 
                relayMapBasedOnUeIndexLcId_g[srcUeIndex][lcIndex].isLocalEndMarkerIndRecvd;
            relayMapBasedOnUeIndexLcId_g[tgtUeIndex][lcIndex].tunnelId = 
                relayMapBasedOnUeIndexLcId_g[srcUeIndex][lcIndex].tunnelId;
            relayMapBasedOnUeIndexLcId_g[tgtUeIndex][lcIndex].tunnelDlFwdId = 
                INVALID_TUNNEL_ID;
            relayMapBasedOnUeIndexLcId_g[tgtUeIndex][lcIndex].tunnelUlFwdId = 
                INVALID_TUNNEL_ID;
            relayMapBasedOnUeIndexLcId_g[tgtUeIndex][lcIndex].transId = 
                relayMapBasedOnUeIndexLcId_g[srcUeIndex][lcIndex].transId;
            relayMapBasedOnUeIndexLcId_g[tgtUeIndex][lcIndex].transIdRsp = 
                relayMapBasedOnUeIndexLcId_g[srcUeIndex][lcIndex].transIdRsp;
            /* Re-setting sourceUE/LC map table */
            resetOldUeMapBasedOnUEIndexLcId_g(srcUeIndex, lcIndex);
        }
    }
    return;
}

/****************************************************************************
 * Function Name  : resetOldUeMapBasedOnUEIndexLcId_g 
 * Inputs         : ueIndex- Is the Index of the UE for which Intra ENB Ho ongoing
 *                  lcIndex-  
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : 
 ****************************************************************************/

static inline void resetOldUeMapBasedOnUEIndexLcId_g(UInt16 ueIndex, UInt8 lcIndex)
{
    memSet(&relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex],0,sizeof(TunnelIdMap));

    relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelId = INVALID_TUNNEL_ID;
    /* Initialize DL FWD tunnel and UL FWD tunnel to default value */
    relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelDlFwdId = INVALID_TUNNEL_ID;
    relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelUlFwdId = INVALID_TUNNEL_ID;
}
/* Intra-eNB HO Changes End */

/* Cyclomatic Complexity changes - starts here */
/****************************************************************************
 * Function Name  : gtpuGetRespApiId
 * Inputs         : apiIdInReq - API ID in Request,
 *                  ueIndex - UE ID,
 *                  lcId - Logical channel ID
 * Outputs        : None
 * Returns        : apiIdForRsp
 * Description    : This funtion gives GTPU Response API ID.
 ****************************************************************************/
static inline UInt16 gtpuGetRespApiId( 
    UInt16 apiIdInReq,
    UInt16 ueIndex,
    UInt16 lcId)
{
    UInt16 apiIdForRsp=0;

    if (S1U_RECONFIGURE_UE_ENTITY_REQ == apiIdInReq)
    { 
        apiIdForRsp = S1U_RECONFIGURE_UE_ENTITY_CNF;

        LOG_GTPU_MSG(PR_RECVD_RECONFIG_UE_ENTITY_REQ, LOGINFO, PR_GTPU_CONTROL,
                relayGtpuCurrentTick_g,__LINE__,
                ueIndex,lcId ,
                0,0, 0,0, __func__, "");
    }
    else if(S1U_CREATE_UE_ENTITY_REQ == apiIdInReq)
    {
        apiIdForRsp = S1U_CREATE_UE_ENTITY_CNF;

        LOG_GTPU_MSG(PR_RCVD_S1U_CREATE_UE_ENTITY_REQ, LOGINFO, PR_GTPU_CONTROL,
                relayGtpuCurrentTick_g,__LINE__,
                ueIndex,lcId ,
                0,0, 0,0, __func__, "");
    }    
    return apiIdForRsp;
}

 /****************************************************************************
 * Function Name  : checkS1UTennelId
 * Inputs         : sapReq_p - Pointer to SAP Request,
 *                  setupSap - Pointer to relaySetupSapReq,
 *                  index,
 *                  ueIndex - UE ID,
 *                  lcIndex - Logical channel ID,
 *                  modifyFlag,
 *                  transIdForRsp - Transaction ID for Response,
 *                  apiIdForRsp - API ID for Response,
 *                  msgLen - Message length
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion checks for tunnel Id.
 ****************************************************************************/
static inline UInt32 checkS1UTennelId(
    UInt8 *sapReq_p,
    relaySetupSapReq *setupSap,
    UInt16 *index, UInt16 ueIndex,
    UInt16 lcIndex,UInt8  *modifyFlag,
    UInt32 transIdForRsp,UInt16 apiIdForRsp,
    UInt16 msgLen )
{
    UInt16 current = *index;
    UInt16 length =0;
    UInt32 transId = 0, prevTidIndex = 0;
    UInt16 tunnelCount = 0;
    UInt32 ret = RELAY_GTPU_SUCCESS; 
    UInt8 isTunnelIdFlagTrue = RELAY_GTPU_TRANS_ID_TRUE;
    UInt8 totalNoOfFailure = 0;
    /* +- SPR 17777 */
    /* LIPA changes start */
    UInt32 tunnelIDSelf = 0;
    UInt8 gtpuSetupMsg[RELAY_SIZE_OF_TAG+ RELAY_SIZE_OF_LEN +RELAY_ADDR_LEN_IPV6] = {0};
    UInt8 len = 0;
    UInt8 i = 0;
#ifndef EPC_SETUP
    /* + coverity 52877 */
    UInt8* outMsgTG_p = PNULL;
    outMsgTG_p = (UInt8 *)getMemFromPool(MAX_API_LEN + API_HDR, NULL);
    if(PNULL == outMsgTG_p)
    {
        lteWarning("In %s, MEMORY ALLOC FAILURE\n",__func__);
        return RELAY_GTPU_FAILURE;
    }
    /* - coverity 52877 */

    /*  + Layer2 NON CA Changes */  
    UInt16 currentTG = API_HDR ;
    /*  - Layer2 NON CA Changes */  
#endif        
    /* LIPA changes end */

    if ( S1U_IE_TEID_SELF == RELAY_GTPU_GET_U16BIT(&sapReq_p[current]) )
    {
        setupSap->teidSelf.tag = sapReq_p[++current];
        current ++;
        setupSap->teidSelf.length = RELAY_GTPU_GET_U16BIT(&sapReq_p[current]);
        current +=2;
        setupSap->teidSelf.tunnelId = RELAY_GTPU_GET_U32BIT(&sapReq_p[current]);
        current +=4;

    }                   
    if ( S1U_IE_QOS_PROFILE == RELAY_GTPU_GET_U16BIT(&sapReq_p[current]))
    {
        setupSap->qosProfile.tag = sapReq_p[++current];			
        current ++;  
        setupSap->qosProfile.length = RELAY_GTPU_GET_U16BIT(&sapReq_p[current]);
        current +=2;
        setupSap->qosProfile.allocRetPriority = sapReq_p[current++];        
        setupSap->qosProfile.data = 
            qvAlloc(setupSap->qosProfile.length-5,PNULL); 
        if (PNULL == setupSap->qosProfile.data)
        {
            /* SPR 2030 fix Start*/
            LOG_GTPU_MSG(L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                    relayGtpuCurrentTick_g,__LINE__,
                    0,0,0,0, 0,0, __func__,"");
            ALARM_MSG(PACKET_RELAY_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID,
                    CRITICAL_ALARM);
            /* SPR 2030 fix End*/
            /* + coverity 52877 */
#ifndef EPC_SETUP
            freeMemPool(outMsgTG_p);
#endif
            /* - coverity 52877 */

            return RELAY_GTPU_FAILURE;
        }
        memCpy(setupSap->qosProfile.data,&sapReq_p[current], \
                setupSap->qosProfile.length-5);
        current += setupSap->qosProfile.length-5;  
    }
    if ( S1U_IE_SEQ_DISABLE == RELAY_GTPU_GET_U16BIT(&sapReq_p[current]) )
    {
        setupSap->seqFlag.tag = sapReq_p[++current];
        current ++;  
        setupSap->seqFlag.length = RELAY_GTPU_GET_U16BIT(&sapReq_p[current]);
        current +=2;  				
        setupSap->seqFlag.value  = sapReq_p[current++];				
    }            
    if ( S1U_IE_REORDERING_REQD == RELAY_GTPU_GET_U16BIT(&sapReq_p[current]) )
    {
        setupSap->reorderingReqd.tag = sapReq_p[++current];
        current ++;  
        setupSap->reorderingReqd.length = RELAY_GTPU_GET_U16BIT(&sapReq_p[current]);
        current +=2;  
        setupSap->reorderingReqd.value = sapReq_p[current++];
    }
    /* SPR 10355 Fix start */
    /** INTRA_ENB_HO Fix Start */
    if ( S1U_IE_PDCP_EXT_FLAG_TAG == RELAY_GTPU_GET_U16BIT(&sapReq_p[current]) )
    {
        setupSap->pdcpextFlag.tag = sapReq_p[++current];
        current ++;
        setupSap->pdcpextFlag.length = RELAY_GTPU_GET_U16BIT(&sapReq_p[current]);
        current +=2;
        setupSap->pdcpextFlag.value = sapReq_p[current++];
    }
    /** INTRA_ENB_HO Fix End */
    /* SPR 10355 Fix end */
#if defined(ENDC_ENABLED) && !(UE_SIM_TESTING)
	/*Dual Connectivity SeNB changes*/
	/*DC bearer is configured before Tunnel information*/
	UInt8 snSize = 0;
	UInt8 rlcMode = 0;
	if ( S1U_IE_DC_BEARER_TYPE_INFO == RELAY_GTPU_GET_U16BIT(&sapReq_p[current]) )
	{
	    current +=4;
	    /*store DC bearer type in relay map for LC*/
	    relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].dcBearerType =
	        (DCBearerType)sapReq_p[current++];

	   if ( S1U_IE_SAP_PARAM_TAG == RELAY_GTPU_GET_U16BIT(&sapReq_p[current]) )
	   {
	   	    current +=4;
           snSize = sapReq_p[current++];
           rlcMode = sapReq_p[current++];
           /* SCG Split tunnel created, invoked X2U Relay module API to create
            * bearer context. */
           if(SN_TERMINATED_SCG_SPLIT_BEARER == relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].dcBearerType)
           {
               if(RELAY_GTPU_FAILURE == x2uRelayCreateBearerContext(
                           ueIndex,
                           s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].lc_id,
                           snSize,
                           rlcMode))
               {
                /* coverity_<147481>_fix_start */    
                    freeMemPool(outMsgTG_p);
                 /* coverity_<147481>_fix_end */
                   return RELAY_GTPU_FAILURE;
               }
           }

	   }
	    LOG_GTPUPR_DEBUG(PR_GTPU_CONTROL,
	            "GTPU->PR, SETUP_SAP_REQ DC Bearer Type value [%u]\n",
	            relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].dcBearerType);
	}
#endif

    for (tunnelCount=0; tunnelCount < MAX_NUM_TUNNEL_PER_LC; tunnelCount++)
    {
        if (S1U_IE_TUNNEL_INFO == RELAY_GTPU_GET_U16BIT(&sapReq_p[current]) )
        {
            setupSap->sapFlag.tag = sapReq_p[++current];
            current ++; 
            setupSap->sapFlag.length = RELAY_GTPU_GET_U16BIT(&sapReq_p[current]);
            current +=2; 
            setupSap->sapFlag.value = sapReq_p[current++];
            LOG_UT(PR_SETUP_SAP_REQ_SAP_INFO, LOGDEBUG, PR_GTPU_CONTROL,
                    relayGtpuCurrentTick_g,__LINE__,
                    ueIndex,setupSap->lcId ,setupSap->sapFlag.value,0, 
                    0,0, __func__, "");

            if ( S1U_IE_TEID_PEER == RELAY_GTPU_GET_U16BIT(&sapReq_p[current]) )
            {
#if defined(ENDC_ENABLED) && !(UE_SIM_TESTING)
				/*if SAP FLag is 0x90 then S1U_RECONFIGURE is only expected in case of
				 * DC UE with SPLIT Bearer configuration or SCG Bearere with teid self for Peer update*/

				if ((setupSap->sapFlag.value == S1U_SN_TERMINTED_SPLIT_BEARER_SAP_FLAG) &&
				        (relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelId  != INVALID_TUNNEL_ID) &&
				        (relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].dcBearerType != SN_TERMINATED_SCG_SPLIT_BEARER) &&
				        (0 == setupSap->teidSelf.tunnelId))
				{
				    LOG_GTPUPR_ERROR(PR_GTPU_CONTROL,
				            "GTPU->PR, S1U_RECONFIGURE received  with wrong configuration "
				            "Tunnel ID [%u] DC Bearer Type value [%u]\n",
				            relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelId,
				            relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].dcBearerType);
#ifndef EPC_SETUP
					/*+Coverity fix 147481*/
            		freeMemPool(outMsgTG_p);
					/*-Coverity fix 147481*/
#endif

				    return RELAY_GTPU_FAILURE;
				}
#endif

                setupSap->teidPeer.tag = sapReq_p[++current];
                current ++; 
                setupSap->teidPeer.length = RELAY_GTPU_GET_U16BIT(&sapReq_p[current]);
                current +=2; 
                setupSap->teidPeer.tunnelId = RELAY_GTPU_GET_U32BIT(&sapReq_p[current]);
                current +=4;


                if(S1U_SECONDARY_TUNNEL_SAP_FLAG  != setupSap->sapFlag.value)
                {
                    updateTunnelIdInReconfigCnf(ueIndex, lcIndex,
                            setupSap->sapFlag.value,setupSap->teidPeer.tunnelId );
                    s1ReconfigSapCnf_g.setupSapCnf[lcIndex].tunnelCount++;
                }
            }			
#if   defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
            else if (setupSap->sapFlag.value == S1U_SN_TERMINTED_SPLIT_BEARER_SAP_FLAG)

            /* For SN Termincate split bearer, absence of Peer tunnel Id doesn't indicate
             * modification request from RRC. */
            {
		    /*SPR 21677 +*/
                LOG_GTPUPR_INFO(PR_GTPU_CONTROL,
                                "GTPU->PR, SETUP_SAP_CNF Tunnel Id received TEID Self [%u]\n",
                                setupSap->teidPeer.tunnelId);
		/*SPR 21677 -*/
                /* Nothing to do. This is just to ensure that we are skipping
                 * the below else condition for LWA tunnel as it will treat
                 * absence of Peer tunnel Id as modification request. */

                /* Reset the teidPeer information tag */
                setupSap->teidPeer.tag      = 0;
                setupSap->teidPeer.length   = 0;
                setupSap->teidPeer.tunnelId = 0;
            }			

#endif

            /*SPR 2952 Changes Start*/ 
            else
            {
                /*If Peer is not present, ie modify RAB req received
                 * from RRC, hence
                 * relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex] shall
                 * be in Use*/
                setupSap->teidSelf.tag = S1U_IE_TEID_SELF;
                setupSap->teidSelf.tunnelId = 
                    relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelId;
                transId = relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].transId;
                *modifyFlag = 1;
            }
            /*SPR 2952 Changes End*/ 
            if ( S1U_IE_GSN_ADDR == RELAY_GTPU_GET_U16BIT(&sapReq_p[current]) )
            {
                setupSap->transportAddr.tag = sapReq_p[++current];
                current ++;
                setupSap->transportAddr.length = RELAY_GTPU_GET_U16BIT(&sapReq_p[current]);
                current +=2;
                length = setupSap->transportAddr.length;					                    
                setupSap->transportAddr.data = qvAlloc(length - 4, PNULL);

                if (!setupSap->transportAddr.data)
                {
                    /* SPR 2030 fix Start*/
                    LOG_GTPU_MSG(L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                            relayGtpuCurrentTick_g,__LINE__,
                            0,0,0,0, 0,0, __func__,"");
                    ALARM_MSG(PACKET_RELAY_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID,
                            CRITICAL_ALARM);
                    /* SPR 2030 fix End*/
                    /* SPR 8838 fix start */
                    if (PNULL != setupSap->qosProfile.data)
                    {
                        /* SPR 22402 Fix Start */
                        freeMemPool(setupSap->qosProfile.data);
                        /* SPR 22402 Fix End */
                        /* SPR 16771 Start */
                        setupSap->qosProfile.data = PNULL;
                        /* SPR 16771 End */
                    }
                    /* SPR 8838 fix end */
                    /* + coverity 52877 */
#ifndef EPC_SETUP
                    freeMemPool(outMsgTG_p);
#endif
                    /* - coverity 52877 */
                    return RELAY_GTPU_FAILURE;
                }
                memCpy(setupSap->transportAddr.data,&sapReq_p[current], \
                        setupSap->transportAddr.length - 4);
                current += setupSap->transportAddr.length - 4;


                /* LIPA changes start */
                /* copy LIPA(secondary) tunnel GW address */
                if(S1U_SECONDARY_TUNNEL_SAP_FLAG  == setupSap->sapFlag.value)
                {
                    UInt8 startVal = 0;
                    RELAY_GTPU_SET_U16BIT(&gtpuSetupMsg[len],
                            setupSap->transportAddr.length-4);
                    len +=2;
                    for (startVal = 0; startVal <(setupSap->transportAddr.length-4); startVal++)
                    { 
                        gtpuSetupMsg[len++] = setupSap->transportAddr.data[startVal];
                    }
                } 
#if   defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
                if (setupSap->sapFlag.value == S1U_SN_TERMINTED_SPLIT_BEARER_SAP_FLAG)
                {
#ifndef EPC_SETUP
						 sendTunnelInfoToDlTg(
                	        getUeIdxFromRrcUeIdx(s1ReconfigSapCnf_g.ueIndex),
            				s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].lc_id,
            				s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].teid_self,
            				s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].teid_peer,
            				SCG_SPLIT_BEARER, snSize,
            				setupSap->transportAddr.length - 4,
            				setupSap->transportAddr.data
                				);
#endif
            	}
#endif
            }
            /* SPR 8596 Fix Start */
            else
            {
                setupSap->transportAddr.data = PNULL;
            }
            /* SPR 8596 Fix End */
            /* SPR 8073 Fix Start */   
            /* LIPA changes start */
            /* As Default Configured Tunnel is Primary Tunnel with SAP Flag value 0x0F
             * If LIPA is Supported, it acts as a Primary Tunnel and Non LIPA Tunnel is 
             * Secondary Tunnel with SAP Flag value 0x0E. 
             * */
            /* Only after primary/main tunnel creation, secondary tunnel info should be present */
            if(S1U_SECONDARY_TUNNEL_SAP_FLAG  == setupSap->sapFlag.value)
            {
                LOG_UT(PR_SETUP_SAP_REQ_SAP_INFO, LOGINFO, PR_GTPU_CONTROL,
                        relayGtpuCurrentTick_g,__LINE__,
                        ueIndex,setupSap->lcId ,
                        transId,0, 
                        0,0, __func__, "");

                if (RELAY_GTPU_SUCCESS == updateRelayMapTableOnTransIdForSecondaryTunnel(
                            ueIndex,
                            setupSap->lcId,transIdForRsp,
                            apiIdForRsp,
                            transId,setupSap->sapFlag.value,
                            &tunnelIDSelf))
                {
                    /* This function adds the transport address to the path management
                     * list for echo req/resp and also update the RAB context with secondary 
                     * tunnel address */
                    handleGtpuSecondaryTunnelId(p_global, gtpuSetupMsg, 
                            tunnelIDSelf);
#ifndef EPC_SETUP
                    UInt16 rrcUeIndex = getUeIdxFromRrcUeIdx(s1ReconfigSapCnf_g.ueIndex);
                    /* Sending Primary Tunnel Information to DL-TG*/
                    sendMessageToDlTg (rrcUeIndex,
                            relayMapBasedOnTransIdForSecTunnel_g[transId].lcId, 
                            setupSap->teidPeer.tunnelId,
                            relayMapBasedOnTransIdForSecTunnel_g[transId].tunnelId,
                            outMsgTG_p,
                            currentTG, 
                            NORMAL_BEARER, 
                            SOURCE_ENB_VALUE,
                            /*SPR 20646 fix start*/
                            length
                            /*SPR 20646 fix end*/
                            );	
#endif
                    if (current == msgLen)
                    {                                                            
                        isDelayReq_g = RELAY_GTPU_IN_USE;                           
                    }     
                    if (isDelayReq_g)                                            
                    {    
                        /* call function pointer to send the response to S1-AP Module */
                        if (RELAY_GTPU_SUCCESS !=
                                sendConfirmationRsp[RELAY_GTPU_CP_SETUP_SAP_CNF]
                                (NULL,transId,RELAY_GTPU_LEN_NOT_REQD))
                        {      
                            LOG_GTPU_MSG(PR_SEND_CNF_TO_CP_FAIL,LOGWARNING, PR_GTPU_CONTROL,
                                    relayGtpuCurrentTick_g, __LINE__,
                                    0,0,0,0, 0,0, __func__,"RELAY_GTPU_CP_SETUP_SAP_CNF"); 
                            /* SPR 8838 start */
                            if (PNULL != setupSap->qosProfile.data)
                            {
                                /* SPR 22402 Fix Start */
                                freeMemPool(setupSap->qosProfile.data);
                                /* SPR 22402 Fix End */
                                /* SPR 16771 Start */
                                setupSap->qosProfile.data = PNULL;
                                /* SPR 16771 End */
                            }
                            if (PNULL != setupSap->transportAddr.data)
                            {
                                /* SPR 22402 Fix Start */
                                freeMemPool(setupSap->qosProfile.data);
                                /* SPR 22402 Fix End */
                                /* SPR 16771 Start */
                                setupSap->transportAddr.data = PNULL;
                                /* SPR 16771 End */
                            }
                            /* SPR 8838 end */        
                            /* + coverity 52877 */
#ifndef EPC_SETUP
                            freeMemPool(outMsgTG_p);
#endif
                            /* - coverity 52877 */
                            return RELAY_GTPU_FAILURE;
                        }     
                        /*Reset the global for SETUP_SAP_CNF*/                                                      
                        for(i = 0 ; i < MAX_RAB_ID ; i++)
                            memSet(&s1ReconfigSapCnf_g.setupSapCnf[i],0,sizeof(relaySetupCnf));
                        s1ReconfigSapCnf_g.lcCount = 0;
                        isDelayReq_g = RELAY_GTPU_NOT_IN_USE;                               
                    }
                }
            }
            /* +- SPR 17777 */
            else if (RELAY_GTPU_SUCCESS == updateRelayMapTableBasedOnTransId(
                        ueIndex,setupSap->lcId,transIdForRsp,
                        apiIdForRsp,&transId,setupSap->sapFlag.value,
                        &isTunnelIdFlagTrue,&prevTidIndex,
                        /*SPR 2952 Changes Start*/
                        *modifyFlag))
                /*SPR 2952 Changes End*/
                /* +- SPR 17777 */
            {  
                if (S1U_IE_TUNNEL_INFO == RELAY_GTPU_GET_U16BIT(&sapReq_p[current]))
                {
                    s1ReconfigSapCnf_g.setupSapCnf[lcIndex].tunnelCount++;
                }
                if (current == msgLen)
                    isDelayReq_g = RELAY_GTPU_IN_USE;
                handleGtpuSetupSapReq((relaySetupSapReq *)setupSap,transId);
            } 
            /* SPR 8073 Fix End */
            /* SPR 16771 Start */
            /* freeing transportAddr.data here since it is already used */
            if (PNULL != setupSap->transportAddr.data)
            {
                /* SPR 22402 Fix Start */
                freeMemPool(setupSap->transportAddr.data);
                /* SPR 22402 Fix End */
                setupSap->transportAddr.data = PNULL;
            }
            /* SPR 16771 End */
        }
        else
        {
            if (!tunnelCount)
            {
                totalNoOfFailure += 1;
            }
            else
            {
                break;
            }    
        }
    }
    *index = current;
    /* + coverity 52877 */
#ifndef EPC_SETUP
    freeMemPool(outMsgTG_p);
#endif
    /* - coverity 52877 */
    return ret;
}
/* Cyclomatic Complexity changes - ends here */

 /****************************************************************************
 * Function Name  : processGtpuSetupSapReq
 * Inputs         : setupSapReq_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process SAP setup request.
 ****************************************************************************/
UInt32 processGtpuSetupSapReq
(
    UInt8 *sapReq_p,
    UInt16 msgLen
)
{
    UInt8 startLc = 0; 
    UInt32 tunnelId = 0, transIdForRsp = 0;
    UInt16 apiIdForRsp = 0, apiIdInReq; 
    UInt16 current = 0;
    UInt16 rrcUeIndex = 0,tunnelCount = 0 , ueIndex=0, lcId = 0, lcIndex = 0;
    /* +- SPR 19660 */
    /* Following three elements are needed for handling failure scenarios */
    UInt8 modifyFlag = 0;

    /*  + Layer2 NON CA Changes */  
    RrcCellIndex cellIndex;
    /*  - Layer2 NON CA Changes */  

    /* + coverity 55800 */
    relayGtpuSetupSapReq* setupSapReq_p = PNULL;
    setupSapReq_p = (relayGtpuSetupSapReq*)getMemFromPool(sizeof(relayGtpuSetupSapReq),NULL);
    if(PNULL == setupSapReq_p)
    {
        return RELAY_GTPU_FAILURE;
    }

    /* SPR 8838 fix start */
    memSet(setupSapReq_p, 0, sizeof(relayGtpuSetupSapReq));
    /* SPR 8838 fix end */

    if (PNULL != sapReq_p)
    {
        /* Extracting the transaction identifier */
        transIdForRsp =  RELAY_GTPU_GET_U16BIT(&sapReq_p[current]);
        /* Infering the API ID which has to go in response of the received req*/
        current +=6;
        apiIdInReq = RELAY_GTPU_GET_U16BIT(&sapReq_p[current]);  
        /* Incrementing the current by 10. starting 10 bytes will be
           API Header */
        current = API_HDR;
        /*  + Layer2 NON CA Changes */  
        cellIndex = sapReq_p[API_HEADER_CELL_INDEX_OFFSET];
        /*  - Layer2 NON CA Changes */  
        s1ReconfigSapCnf_g.cellIndex = cellIndex;/* to send responce back to RRC*/
        rrcUeIndex = setupSapReq_p->ueIndex = RELAY_GTPU_GET_U16BIT(&sapReq_p[current]);
        /* SPR 4558 Fix Start */
        ueIndex = getUeIdxFromRrcUeIdx(rrcUeIndex);
        /* + SPR 12979 */
        /* New UE is created with SAP information available (HO scenario) */
        if( S1U_CREATE_UE_ENTITY_REQ == apiIdInReq )
        {
#if  defined PDCP_GTPU_INTF
            /* +- SPR21901 , SPR21894 flags removed */
            if (INVALID_UE_ID == ueIndex)
            {
                ueIndex = allocateUeIdxFromRrcUeIdx( rrcUeIndex );
                /* + coverity 83244, 106460, 106459 */
                if( INVALID_UE_ID == ueIndex )
                {
                    freeMemPool(setupSapReq_p);
                    return RELAY_GTPU_FAILURE; 
                }
                /* - coverity 83244, 106460, 106459 */
            }
            /* +- SPR21901 , SPR21894 endif removed */
#endif
            setUeIdxValid( ueIndex, UE_IDX_MAPPING_FOR_S1U );
        }
        /* - SPR 12979 */
        /* SPR 4558 Fix End */
        /*  + Layer2 NON CA Changes */  
        if( (apiIdInReq  == S1U_RECONFIGURE_UE_ENTITY_REQ )  &&
                (s1uUEReconfigCnfInfo_g[ueIndex].cellIndex != cellIndex))
        {
            /* coverity_60439 Fix Start */
            freeMemPool(setupSapReq_p);
            /* coverity_60439 Fix End*/
            return RELAY_GTPU_FAILURE; 
        }
        else
        {
            s1uUEReconfigCnfInfo_g[ueIndex].cellIndex = cellIndex;
        }
        /*  - Layer2 NON CA Changes */  

        lcId = sapReq_p[current + 6];

        /* Cyclomatic Complexity changes - starts here */
        apiIdForRsp = gtpuGetRespApiId(apiIdInReq, setupSapReq_p->ueIndex,lcId);
        /* Cyclomatic Complexity changes - ends here */
        current +=2;

        for (startLc = 0; msgLen > current ; startLc++)
        {
            /* Check SETUP SAP TAG INFO */
            if ( S1U_IE_RELAY_SETUP_SAP_REQ == sapReq_p[++current] )
            {	
                setupSapReq_p->setupSap[startLc].setupSapReqTag = sapReq_p[current];
                current ++;
                setupSapReq_p->setupSap[startLc].setupSapReqTagLen = RELAY_GTPU_GET_U16BIT(&sapReq_p[current]);
                current +=2;
                lcId = setupSapReq_p->setupSap[startLc].lcId = sapReq_p[current++];
                /* Coverity 73011 Fix Start */
                /* Check if LCID received is not less than 3 (DRB)
                 * or more than MAX_DRB(10), return failure */
                if( ( 3 > setupSapReq_p->setupSap[startLc].lcId ) ||
                        (setupSapReq_p->setupSap[startLc].lcId -3) >= MAX_RAB_ID )
                {
                    freeMemPool(setupSapReq_p);
                    return RELAY_GTPU_FAILURE;
                }
                /* Coverity 73011 Fix End */

                lcIndex = lcId - 3;
                /* OPTION3X Changes Start */
#ifdef ENDC_ENABLED
				/*INVALID LC RESP +*/
				if(3 > lcId)
				{
				    UInt16 cause = 4; /* S1U_INV_SAP_CFG */
				    s1ReconfigSapCnf_g.ueIndex = rrcUeIndex;
				    s1ReconfigSapCnf_g.apiIdForRsp = apiIdForRsp;
				    s1ReconfigSapCnf_g.transIdForRsp = transIdForRsp;
				    if(S1U_RECONFIGURE_UE_ENTITY_CNF == apiIdForRsp)
				    {
				        s1ReconfigSapCnf_g.setupSapCnf[0].bitMask |= RELAY_MANDATORY_PRESENT;
				        s1ReconfigSapCnf_g.setupSapCnf[0].lcId = lcId;
				        s1ReconfigSapCnf_g.setupSapCnf[0].cause |= cause;
				    }

				    if (RELAY_GTPU_SUCCESS !=
				            sendConfirmationRsp[RELAY_GTPU_CP_SETUP_SAP_CNF]
				            (NULL,0,cause))
				    {
				        /* SPR 2030 fix Start*/
				        LOG_GTPU_MSG(PR_SEND_CNF_TO_CP_FAIL,LOGWARNING, PR_GTPU_CONTROL,
				                relayGtpuCurrentTick_g, __LINE__,
				                0,0,0,0, 0,0, __func__,"RELAY_GTPU_CP_SETUP_SAP_CNF");
				        /* SPR 2030 fix End*/
				        /*+ Coverity 60439*/
                    	freeMemPool(setupSapReq_p);
				        /*- Coverity 60439*/
				        return RELAY_GTPU_FAILURE;
				    }

				}
				/*INVALID LC RESP -*/
#endif
				/* OPTION3X Changes End */
                /* SPR 4631 Fix Start */
                relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].rrcUeIndex = rrcUeIndex;
                /* SPR 4631 Fix End */
                s1ReconfigSapCnf_g.lcCount++; 

                s1uUEReconfigCnfInfo_g[ueIndex].lcCount++;

                s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcIndex].lc_id = setupSapReq_p->setupSap[startLc].lcId;

                setupSapReq_p->setupSap[startLc].qosId = RELAY_GTPU_GET_U32BIT(&sapReq_p[current]);
                current +=4;            
                /* SPR 10787 fix start */
                setupSapReq_p->setupSap[startLc].cellIndex = cellIndex;
                /* SPR 10787 fix end */

                /* check whther Tunnel Id is present or not */            
                /* Cyclomatic Complexity changes - starts here */
                /* + SPR 19660 */
                if(RELAY_GTPU_FAILURE == checkS1UTennelId(sapReq_p,&setupSapReq_p->setupSap[startLc],&current,
                        ueIndex,lcIndex,&modifyFlag,transIdForRsp,apiIdForRsp,msgLen)) 
                {
                    /* + coverity 55800 */
                    freeMemPool(setupSapReq_p);
                    /* - coverity 55800 */
                    return RELAY_GTPU_FAILURE;
                }
                /* - SPR 19660 */
                /* Cyclomatic Complexity changes - ends here */

                /* Release the buffers allocated in the Setup Sap request */
                /*SPR #2434 fix Start*/
                if(NULL != setupSapReq_p->setupSap[startLc].qosProfile.data)
                    /*SPR #2434 fix End*/
                {
                    /* SPR 22402 Fix Start */
                    freeMemPool(setupSapReq_p->setupSap[startLc].qosProfile.data);
                    /* SPR 22402 Fix End */
/* SPR 16771 Start */
				    setupSapReq_p->setupSap[startLc].qosProfile.data = PNULL;
/* SPR 16771 End */
                }

/* SPR +-16771  */
                /*SPR 3288 Fix End*/
            }
            else if ( S1U_IE_RELAY_REL_SAP_REQ == sapReq_p[current] )
            {
                setupSapReq_p->releaseSap[startLc].releaseSapReqTag = sapReq_p[current];
                current ++;
                setupSapReq_p->releaseSap[startLc].releaseSapReqTagLen = RELAY_GTPU_GET_U16BIT(&sapReq_p[current]);
                current +=2;
                /* SPR 4558 Fix Start */
                ueIndex = getUeIdxFromRrcUeIdx(setupSapReq_p->ueIndex);
                /* SPR 4558 Fix End */
                /* Coverity 73011 Fix Start */
                if(ueIndex >= RELAY_GTPU_MAX_UE)
                {
                    freeMemPool(setupSapReq_p);
                    return RELAY_GTPU_FAILURE;
                }
                /* Coverity 73011 Fix End */

                lcId = setupSapReq_p->releaseSap[startLc].lcId = sapReq_p[current++];
                lcIndex = lcId - 3;

                /* OPTION3X Changes Start */
#ifdef ENDC_ENABLED
				/*INVALID LC RESP +*/
				if(3 > lcId)
				{
				    UInt16 cause = 4; /* S1U_INV_SAP_CFG */
				    s1ReconfigSapCnf_g.ueIndex = rrcUeIndex;
				    s1ReconfigSapCnf_g.apiIdForRsp = apiIdForRsp;
				    s1ReconfigSapCnf_g.transIdForRsp = transIdForRsp;
				    if(S1U_RECONFIGURE_UE_ENTITY_CNF == apiIdForRsp)
				    {
                        s1ReconfigSapCnf_g.releaseSapCnf[0].bitMask |= RELAY_MANDATORY_PRESENT;
                        s1ReconfigSapCnf_g.releaseSapCnf[0].lcId = lcId;
                        s1ReconfigSapCnf_g.releaseSapCnf[0].cause |= cause;
				    }

				    if (RELAY_GTPU_SUCCESS !=
                            sendConfirmationRsp[RELAY_GTPU_CP_RELEASE_SAP_CNF] 
                            (NULL,0,RELAY_GTPU_LEN_NOT_REQD))
				    {
				        /* SPR 2030 fix Start*/
				        LOG_GTPU_MSG(PR_SEND_CNF_TO_CP_FAIL,LOGWARNING, PR_GTPU_CONTROL,
				                relayGtpuCurrentTick_g, __LINE__,
				                0,0,0,0, 0,0, __func__,"RELAY_GTPU_CP_SETUP_SAP_CNF");
				        /* SPR 2030 fix End*/
                        /* coverity_60439_fix_start */
                        freeMemPool(setupSapReq_p);
                        /* coverity_60439_fix_start */
				        return RELAY_GTPU_FAILURE;
				    }

				}
				/*INVALID LC RESP -*/
#endif
				/* OPTION3X Changes End */
                /*If RELEASE_TUNNEL_INFO tag is present then delete the requested
                 * tunnel(in case of HO FWD TUNNELS), else  delete the RAB requested*/
                if (S1U_IE_RELEASE_TUNNEL_INFO == RELAY_GTPU_GET_U16BIT(&sapReq_p[current]) )
                {
                    s1ReconfigSapCnf_g.releaseSapCnf[lcIndex].tunnelCount++;
                    for (tunnelCount=0; tunnelCount < MAX_NUM_FWD_TUNNEL_PER_LC; tunnelCount++)
                    {
                        /* SPR 16571 fix start */
                        if( S1U_IE_RELEASE_TUNNEL_INFO != 
                                RELAY_GTPU_GET_U16BIT( &sapReq_p[current] ) )
                        {
                            /* There can be less than 2 forwarding tunnels as 
                             * UL forwarding is optional. 
                             * Tag is not RELEASE_TUNNEL_INFO, hence continue 
                             * to process further */
                            continue;
                        }                        
                        /* SPR 16571 fix end */
                        setupSapReq_p->releaseSap[tunnelCount].relSapReq.teidSelf.tag = sapReq_p[++current];
                        current ++; 
                        setupSapReq_p->releaseSap[tunnelCount].relSapReq.teidSelf.length = 
                            RELAY_GTPU_GET_U16BIT(&sapReq_p[current]);
                        current +=2;
                        tunnelId = RELAY_GTPU_GET_U32BIT(&sapReq_p[current]);
                        if (relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelDlFwdId == 
                                tunnelId)
                        {     
                            setupSapReq_p->releaseSap[tunnelCount].relSapReq.teidSelf.tunnelDlFwdId = 
                                tunnelId;
                        } 
                        else if (relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelUlFwdId == 
                                tunnelId)  
                        {
                            setupSapReq_p->releaseSap[tunnelCount].relSapReq.teidSelf.tunnelUlFwdId = 
                                tunnelId;
                        }       
#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
						else if ((relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].dcBearerType == SN_TERMINATED_SCG_SPLIT_BEARER) &&
						        (relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelId == tunnelId))
						{
						    setupSapReq_p->releaseSap[tunnelCount].relSapReq.teidSelf.tunnelId =
						        tunnelId;
						}
#endif


                        else
                        {
                            LOG_GTPU_MSG(PR_SAP_REL_TUNNEL_ID_INV, LOGWARNING,
                                    PR_GTPU_CONTROL, relayGtpuCurrentTick_g, __LINE__,
                                    ueIndex, tunnelId,0,0,0,0,__func__,"");
                            continue;
                        }         
                        /* SPR 7844 changes start */
                        if( LTE_TRUE == relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].isTargetENB )
                        {
                            relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].isTargetENB = LTE_FALSE;
                        }
                        /* SPR 7844 changes end */
                        current +=4;

                        if ( S1U_IE_GSN_ADDR == RELAY_GTPU_GET_U16BIT(&sapReq_p[current]) )
                        {
                            setupSapReq_p->releaseSap[tunnelCount].transportAddr.tag = sapReq_p[++current];
                            current ++;
                            setupSapReq_p->releaseSap[tunnelCount].transportAddr.length = 
                                RELAY_GTPU_GET_U16BIT(&sapReq_p[current]);
                            current +=2;
                            /* As transport data is not used in release, if it is received just
                             * move the pointer and ignore the transport address */
                            current += setupSapReq_p->releaseSap[tunnelCount].transportAddr.length - 4;
                        }
                        if (S1U_IE_RELEASE_TUNNEL_INFO == RELAY_GTPU_GET_U16BIT(&sapReq_p[current]))
                        {
                            relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].numOfsapMsg++;
                            s1ReconfigSapCnf_g.releaseSapCnf[lcIndex].tunnelCount++;
                        }
                        else
                        {
                            isDelayReq_g = RELAY_GTPU_IN_USE; 
                        }        
                        if (tunnelId)
                        {
                            if (current == msgLen)
                                isDelayReq_g = RELAY_GTPU_IN_USE;
                            if (RELAY_GTPU_FAILURE == updateUeCntxMngmntBasedOnTunnelIdForReconfigSapRel(
                                        &setupSapReq_p->releaseSap[tunnelCount].relSapReq,transIdForRsp,apiIdForRsp,ueIndex))
                            {
                                LOG_GTPU_MSG(PR_GTPU_SAP_REL_FAILURE, LOGWARNING, 
                                        PR_GTPU_CONTROL, relayGtpuCurrentTick_g, __LINE__, 
                                        ueIndex, 0,0,0,0,0,__func__,"");
                                /* + coverity 55800 */
                                freeMemPool(setupSapReq_p);
                                /* - coverity 55800 */
                                return RELAY_GTPU_FAILURE;
                            }
                        }
                        tunnelId = 0;
                    }
                }/*If Release_Tunnel_Info Present*/
                else
                {
                    if (current == msgLen)
                        isDelayReq_g = RELAY_GTPU_IN_USE;
                    lcIndex = lcId - 3;
                    setupSapReq_p->releaseSap[lcIndex].relSapReq.teidSelf.tunnelId = tunnelId =
                        relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelId;
                    /* +- SPR 17777 */
                    if (RELAY_GTPU_FAILURE == updateUeCntxMngmntBasedOnTunnelIdForReconfigSapRel(
                                &setupSapReq_p->releaseSap[lcIndex].relSapReq,transIdForRsp,apiIdForRsp,
                                ueIndex))
                    /* +- SPR 17777 */
                    {
                        LOG_GTPU_MSG(PR_GTPU_SAP_REL_FAILURE, LOGWARNING,
                                PR_GTPU_CONTROL, relayGtpuCurrentTick_g, __LINE__,
                                ueIndex, 0,0,0,0,0,__func__,"");
                        /* + coverity 55800 */
                        freeMemPool(setupSapReq_p);
                        /* - coverity 55800 */
                        return RELAY_GTPU_FAILURE;
                    }
                }        
            }        
        }
        /* + coverity 55800 */
        freeMemPool(setupSapReq_p);
        /* - coverity 55800 */
        return RELAY_GTPU_SUCCESS;
    }

    /* SPR 2030 fix Start*/
    LOG_GTPU_MSG(PR_SETUP_SAP_REQ_EMPTY, LOGWARNING, PR_GTPU_CONTROL,
            relayGtpuCurrentTick_g,__LINE__,0,0,0,0,
            0,0,__func__,"");
    /* SPR 2030 fix Start*/
    isDelayReq_g = RELAY_GTPU_IN_USE;
    /* + coverity 55800 */
    freeMemPool(setupSapReq_p);
    /* - coverity 55800 */
    return RELAY_GTPU_FAILURE;
}

/****************************************************************************
 * Function Name  : processGtpuDeleteUeReq
 * Inputs         : releaseSapReq_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process SAP Release Request.
 ****************************************************************************/

UInt32 processGtpuDeleteUeReq
(
 UInt8 *releaseSapReq_p,
 UInt16 msgLen
 )
{
    UInt16 current = 0, startVal = 0, rrcUeIndex ,ueIndex;
    UInt32 transIdForRsp = 0;
    UInt16 apiIdForRsp = 0; 
    UInt8 lcIndex =0;
    relayS1RelSapReq releaseReq = {0};
    UInt8 respMsg[21] = {0}, cause = GTPU_CAUSE_SUCCESS;


    transIdForRsp = RELAY_GTPU_GET_U16BIT(&releaseSapReq_p[current]);
    current +=2;
    apiIdForRsp = S1U_DELETE_UE_ENTITY_CNF;
    current = API_HDR;

    /* SPR 4995 Fix Start */
    /* Flag warning removed*/       
    UInt16 indexForTgUeIdx = 0; 
    /* SPR 4995 Fix End */

    /*  + Layer2 NON CA Changes */  
    RrcCellIndex cellIndex;

    cellIndex = releaseSapReq_p[API_HEADER_CELL_INDEX_OFFSET];

    rrcUeIndex = RELAY_GTPU_GET_U16BIT(&releaseSapReq_p[current]);
    /* SPR 4558 Fix Start */
    ueIndex = getUeIdxFromRrcUeIdx(rrcUeIndex);
    /* SPR 4558 Fix End */
    /* SPR 21117 Fix Start */
    /* s1ReconfigSapCnf_g.ueIndex needs to be initialize here. Incase the tunnels are not created then initialize s1ReconfigSapCnf_g.ueIndex
       with  rrcUeIndex otherwise it will get updated in below funtion call for updateRelayUeCntxMngmntBasedOnTunnelId.
       if Not initialize here then it can cause issue in Detetion of context at MAC when tunnels are not created as INALID value will remain
       set for second UE after DELETION of first UE */
    s1ReconfigSapCnf_g.ueIndex = rrcUeIndex;
    /* SPR 21117 Fix End */
    /* Coverity 78971 Fix Start */
    if(ueIndex >= RELAY_GTPU_MAX_UE)
    {
        return RELAY_GTPU_FAILURE;
    }
    /* Coverity 78971 Fix End */

    if(s1uUEReconfigCnfInfo_g[ueIndex].cellIndex != cellIndex)
    {
        return RELAY_GTPU_FAILURE; 
    }

    /*  - Layer2 NON CA Changes */  
    current +=2;
    /* SPR 2030 fix Start*/
    LOG_GTPU_MSG(PR_RECVD_S1U_DELETE_UE_ENTITY_REQ, LOGINFO, PR_GTPU_CONTROL,
            relayGtpuCurrentTick_g,__LINE__,
            ueIndex,msgLen,0,0,
            0,0, __func__, "");
    /* SPR 2030 fix End*/
#ifdef ENDC_ENABLED
    /*S1U_DELETE_FIX*/
    updateTickAssAtX2U(ueIndex);
#endif
    for (lcIndex = 0; lcIndex < MAX_RAB_ID; lcIndex++)
    {
        /* SPR3274 fix start */
        if(relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].inUseFlag)
        {

            if(INVALID_TUNNEL_ID != 
                    relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelId)   
                releaseReq.teidCount++;

            if(INVALID_TUNNEL_ID != 
                    relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelDlFwdId)
                /* SPR 3085 Fix Start */     
            {
                releaseReq.teidCount++;
                /* SPR 5481 Fix Start */
                /* SPR 7844 changes start */
                if( LTE_FALSE == relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].isTargetENB )
                {
                    relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].EmiFlag = 1;
                }
                else
                {
                    relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].EmiFlag = 0;
                }
                /* SPR 7844 changes end */
                relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].transIdRsp = transIdForRsp;
                /* SPR 5481 Fix End */
                /* SPR 5978 Fix Start */
                relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].cellIndex = cellIndex;
                /* SPR 5978 Fix End */
            }
            /* SPR 3085 Fix End */     
            if(INVALID_TUNNEL_ID != 
                    relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelUlFwdId)     
                releaseReq.teidCount++;

            /* SPR 3085 Fix Start */
            /* SPR 5481 Fix Start */
            if (!relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].EmiFlag || 
                    /* SPR 6758 changes start */
                    LTE_TRUE == 
                    relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].isLocalEndMarkerIndRecvd )
                /* SPR 6758 changes end */
                /* SPR 5481 Fix End */
            {
                /* Reset the flags to default value since tunnels are going to 
                 * be deleted */
                /* SPR 6758 changes start */
                relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].isLocalEndMarkerIndRecvd = LTE_FALSE;
                /* SPR 6758 changes end */
                /* SPR 16001 fix start */
                relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].EmiFlag = 
                    LTE_FALSE;
                relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].isTargetENB = 
                    LTE_FALSE;
                /* SPR 16001 fix end */
                relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].inUseFlag =
                    RELAY_GTPU_NOT_IN_USE;
                /* SPR 3085 Fix End */

                for (startVal = 0; startVal < releaseReq.teidCount; startVal++)
                {
                    if (RELAY_GTPU_FAILURE == 
                            updateRelayUeCntxMngmntBasedOnTunnelId(
                                &releaseReq.relSapReq[startVal],lcIndex,transIdForRsp,apiIdForRsp
                                ,ueIndex,releaseReq.teidCount,startVal))
                    {   
                        LOG_GTPU_MSG(PR_GTPU_SAP_REL_FAILURE, LOGWARNING, 
                                PR_GTPU_CONTROL, relayGtpuCurrentTick_g, __LINE__, 
                                ueIndex, 0,0,0,0,0,__func__,"");
                        isDelayReq_g = RELAY_GTPU_IN_USE;
                        return RELAY_GTPU_FAILURE;
                    }  
                } 
                /* SPR 3085 Fix Start */
            }
            else
            {
                return RELAY_GTPU_SUCCESS;
            }
            /* SPR 3085 Fix End */

            releaseReq.teidCount = 0;
        } 
        /* SPR3274 fix end */
    }
    /* SPR 4772 Fix start */
    /*  + Layer2 NON CA Changes */  
    current = API_HDR; 
    /*  - Layer2 NON CA Changes */  
    /* SPR 4772 Fix end */
    RELAY_GTPU_SET_U16BIT(&respMsg[current], rrcUeIndex);
    /* SPR 4995 Fix Start */
    indexForTgUeIdx = current; 
    /* SPR 4995 Fix End */
    current += 2;

    /*If a valid UE Index exists i.e Resource exists, hence
     * response cause should be SUCCESS*/
    /* SPR 4558 Fix Start */
    if( (getUeIdxFromRrcUeIdx(s1ReconfigSapCnf_g.ueIndex) ) != INVALID_UE_INDEX)
        /* SPR 4558 Fix End */
    {
        respMsg[current++] = cause;  /* Cause */
        /* + SPR 12979 */
        /* UE is deleted, hence free from the global map */
        if( 0 == setUeIdxInvalid( ueIndex, UE_IDX_MAPPING_FOR_S1U) )
        {
            freeUeIdx( ueIndex );
        }
        /* - SPR 12979 */
        s1ReconfigSapCnf_g.ueIndex = INVALID_UE_INDEX;
    }
    else
/*CID 110667 fix start*/
    {   cause = GTPU_CAUSE_RSRC_NOT_AVAILABLE;
        respMsg[current++] = cause;  /* Cause */

    }
/*CID 110667 fix end*/


    LOG_GTPU_MSG(PR_SEND_S1U_DEL_UE_ENTITY_CNF,LOGINFO,PR_GTPU_CONTROL,
            relayGtpuCurrentTick_g,__LINE__,ueIndex,
            cause,
            0,0, 0,0, __func__, "");

    /*  + Layer2 NON CA Changes */
    appendApiHeader(RELAY_S1AP_MODULE_ID, apiIdForRsp,
            transIdForRsp, current, respMsg, 
            s1uUEReconfigCnfInfo_g[ueIndex].cellIndex);
    /*  - Layer2 NON CA Changes */  


    udpSendToS1ap((Char8*)respMsg, current);

    /* SPR 3277 fix Start*/
    /* SPR 4995 Fix Start */
    RELAY_GTPU_SET_U16BIT(&respMsg[indexForTgUeIdx], ueIndex); 
    /* SPR 4995 Fix End */
#ifndef EPC_SETUP
    /* Sending ue id for release information to DL-TG */
    /*IPV6_CHANGE - START*/ 
    udpSendToTG(respMsg , current ,relayConfigInfo_g.peerTgInfAddrType);
    /* SPR 5599_128826 Fix End */
    /*IPV6_CHANGE - END*/
    /* LIPA changes start */
    udpSendToSecondaryTG(respMsg , current , relayConfigInfo_g.secPeerTgInfAddrType);
    /* LIPA changes end */
#endif

    return RELAY_GTPU_SUCCESS;
}

/****************************************************************************
 * Function Name  : processGtpuCpErrorInd
 * Inputs         : cpErrorInd_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process error indication.
 ****************************************************************************/

UInt32 processGtpuCpErrorInd
(
    UInt8 *cpErrorInd_p,
    UInt16 msgLen
)
{
    UInt16 index = CSPL_HDR;
    /* SPR_9416_Changes Start */
    UInt32 transId = 0;
    /* SPR_9416_Changes End */
    /* SPR 13378 FIX Start */
    if (RELAY_GTPU_IE_TRANSACTION_ID == cpErrorInd_p[index++])
    /* SPR 13378 FIX End */
    {
        transId =  RELAY_GTPU_GET_U32BIT(&cpErrorInd_p[index]); 
        index +=4;

        /* From TransId, Tunnel Id is extracted. but before this first 
           Check whether this structure is inuse or not */
        if( transId < RELAY_GTPU_MAX_TRANS_ID )
        {
            if (RELAY_GTPU_IN_USE  
                    == relayMapBasedOnTransId_g[transId].inUseFlag)
            {

                /* Call function handler to send the response to S1-AP */
                /* SPR_9416_Changes Start */
                if (RELAY_GTPU_SUCCESS !=
                        sendConfirmationRsp[RELAY_GTPU_CP_ERROR_IND]
                        ((UInt8 *)&transId,RELAY_GTPU_TRANS_ID_RESERVED,
                         RELAY_GTPU_LEN_NOT_REQD))
                    /* SPR_9416_Changes End */
                {
                    /* SPR 2030 fix Start*/
                    /* +- SPR 17777 */
                    LOG_GTPU_MSG(PR_SEND_CNF_TO_CP_FAIL,LOGWARNING, PR_GTPU_CONTROL,
                            relayGtpuCurrentTick_g, __LINE__,
                            msgLen,0,0,0, 0,0, __func__,"RELAY_GTPU_CP_ERROR_IND"); 
                    /* SPR 2030 fix End*/
                    return RELAY_GTPU_FAILURE;
                }

                /* SPR 2030 fix Start*/

                LOG_GTPU_MSG(PR_SENT_ERR_IND_TO_CP,LOGINFO, PR_GTPU_CONTROL,
                        relayGtpuCurrentTick_g, __LINE__,
                        0,0,0,0, 0,0, __func__,""); 
                /* SPR 2030 fix End*/
                return RELAY_GTPU_SUCCESS;
            }
        }

    }
    return RELAY_GTPU_FAILURE; 
}

/****************************************************************************
 * Function Name  : processGtpuCpPathFailureInd
 * Inputs         : cpPathFailureInd_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process CP Path Failure Indication.
 ****************************************************************************/

UInt32 processGtpuCpPathFailureInd
(
    UInt8 *cpPathFailureInd_p,
    UInt16 msgLen
)
{
    /* + COVERITY 11258 */
    UInt8 * cpPathFailureMsg_p = PNULL ;
    /*+SPR #3456*/    
    UInt16 current = CSPL_HDR,startVal = 0;
    UInt16 outMsgPos = API_HDR;
    UInt16 length = 0;

    cpPathFailureMsg_p = qvMsgAllocBySize (MAX_API_LEN + OAM_L2_API_HDR);
    if (PNULL == cpPathFailureMsg_p )
    {
        return RELAY_GTPU_FAILURE;
    }
    if (RELAY_GTPU_IE_GSN_ADDR == cpPathFailureInd_p[current])
    {
        RELAY_GTPU_SET_U16BIT(&cpPathFailureMsg_p[outMsgPos],S1U_IE_GSN_ADDR_TAG);
        current += 1;
        outMsgPos +=2;
        length =  RELAY_GTPU_GET_U16BIT(&cpPathFailureInd_p[current]) + 4;
        RELAY_GTPU_SET_U16BIT(&cpPathFailureMsg_p[outMsgPos],length);
        current +=2;
        outMsgPos += 2;
        /* Coverity 72926 Fix Start*/
        if((length-4) > (msgLen-current))
        {
            return RELAY_GTPU_FAILURE;
        }
        /* Coverity 72926 Fix End*/

        for (startVal = 0; startVal < length - 4 ; startVal++,current++,outMsgPos++)
            cpPathFailureMsg_p[outMsgPos] = cpPathFailureInd_p[current];
	/* SPR 13765 fix Start */
		/* Call function pointer to send Error Indication to OAM */
		if( RELAY_GTPU_SUCCESS != processRelayMessage[PR_GTPU_ERROR_IND]((UInt8 *)cpPathFailureMsg_p,outMsgPos))
		{
			// print log Msg
            LOG_GTPU_MSG(PR_UNABLE_TO_SEND_ERR_REPORT_TO_OAM,LOGWARNING, PR_GTPU_CONTROL,
                    relayGtpuCurrentTick_g, __LINE__,
                    0,0,0,0, 0,0, __func__,"PR_GTPU_ERROR_IND"); 
		}
	/* SPR 13765 fix END */
        /*-SPR #3456*/    

        /* Call function pointer to send the response to S1-AP Module */
        if (RELAY_GTPU_SUCCESS !=
                sendConfirmationRsp[RELAY_GTPU_CP_PATH_FAILURE_IND]
                /*+SPR #3456*/
                ((UInt8*)cpPathFailureMsg_p,RELAY_GTPU_TRANS_ID_RESERVED,outMsgPos))
            /*+SPR #3456*/
        {
            /* SPR 2030 fix Start*/
            LOG_GTPU_MSG(PR_SEND_CNF_TO_CP_FAIL,LOGWARNING, PR_GTPU_CONTROL,
                    relayGtpuCurrentTick_g, __LINE__,
                    0,0,0,0, 0,0, __func__,"RELAY_GTPU_CP_PATH_FAILURE_IND"); 
            /* SPR 2030 fix End*/
            /* SPR 22402 Fix Start */
            msgFree(cpPathFailureMsg_p);
            /* SPR 22402 Fix End */
            return RELAY_GTPU_FAILURE;
        }
        /* SPR 2030 fix Start*/
        LOG_GTPU_MSG(PR_SENT_PATH_FAILURE_IND_TO_CP,LOGWARNING, PR_GTPU_CONTROL,
                relayGtpuCurrentTick_g, __LINE__,
                0,0,0,0, 0,0, __func__,""); 
        /* SPR 2030 fix End*/
        /* SPR 22402 Fix Start */
        msgFree(cpPathFailureMsg_p);
        /* SPR 22402 Fix End */
        return RELAY_GTPU_SUCCESS;
    }  
    /* SPR 22402 Fix Start */
    msgFree(cpPathFailureMsg_p);
    /* SPR 22402 Fix End */
    /* - COVERITY 11258 */
    return RELAY_GTPU_FAILURE;
}

/*+SPR #3456*/
/****************************************************************************
 * Function Name  : processGtpuCpPathSuccessInd
 * Inputs         : cpPathSuccessInd_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process CP Path Success Indication.
 ****************************************************************************/

UInt32 processGtpuCpPathSuccessInd
(
    UInt8 *cpPathSuccessInd_p,
    UInt16 msgLen
)
{
    /* + COVERITY 11259 */
    UInt8  * cpPathSuccessMsg_p = PNULL;
    UInt16 current = CSPL_HDR,startVal = 0; 
    UInt16 currentPos = API_HDR; 
    UInt16 length = 0;
	U8bit alarm_flag = 0;

    cpPathSuccessMsg_p = qvMsgAllocBySize (MAX_API_LEN + OAM_L2_API_HDR);
    if (PNULL == cpPathSuccessMsg_p )
    {
        return RELAY_GTPU_FAILURE;
    }
	/* SPR 13765 fix START */
    if (RELAY_GTPU_IE_GSN_ADDR_WITH_FLAG == cpPathSuccessInd_p[current])
	{
		alarm_flag = cpPathSuccessInd_p[++current];
	}
    if ((RELAY_GTPU_IE_GSN_ADDR == cpPathSuccessInd_p[current])||(alarm_flag))
	/* SPR 13765 fix END */
    {
        RELAY_GTPU_SET_U16BIT(&cpPathSuccessMsg_p[currentPos],S1U_IE_GSN_ADDR_TAG);
        current +=1;
        currentPos +=2;
        length =  RELAY_GTPU_GET_U16BIT(&cpPathSuccessInd_p[current]) + RELAY_TAG_LEN + 1;
        RELAY_GTPU_SET_U16BIT(&cpPathSuccessMsg_p[currentPos],length);
        current +=2;
        currentPos +=2;
        for (startVal = 0; startVal < length - RELAY_TAG_LEN -1;startVal++,current++,currentPos++)
            cpPathSuccessMsg_p[currentPos] = cpPathSuccessInd_p[current];
		/* SPR 13765 fix START */
        if((EGTPU_SEND_ALARM_WITH_PATH_SUCCESS == alarm_flag)||(!alarm_flag))
        {
            /* SPR 13765 fix END */
            /* Call function pointer to send the response to S1-AP Module */
            if (RELAY_GTPU_SUCCESS !=
                    sendConfirmationRsp[RELAY_GTPU_CP_PATH_SUCCESS_IND]
                    ((UInt8*)cpPathSuccessMsg_p,RELAY_GTPU_TRANS_ID_RESERVED,currentPos))
            {
                /* SPR 22402 Fix Start */
                msgFree(cpPathSuccessMsg_p);
                /* SPR 22402 Fix End */
                return RELAY_GTPU_FAILURE;
            }
        }
		/* SPR 13765 fix START */
		if(alarm_flag)
        {
            RELAY_GTPU_SET_U16BIT(&cpPathSuccessMsg_p[API_HDR],S1U_IE_GSN_ADDR_RECOVERED_TAG);
			if( RELAY_GTPU_SUCCESS != processRelayMessage[PR_GTPU_ERROR_IND]((UInt8 *)cpPathSuccessMsg_p,currentPos))
			{
			    // print log Msg
           		LOG_GTPU_MSG(PR_UNABLE_TO_SEND_ERR_REPORT_TO_OAM,LOGWARNING, PR_GTPU_CONTROL,
                    relayGtpuCurrentTick_g, __LINE__,
                    msgLen,0,0,0, 0,0, __func__,"PR_GTPU_ERROR_IND"); 
			}	
        }
		/* SPR 13765 fix END */

        /* SPR 22402 Fix Start */
        msgFree(cpPathSuccessMsg_p);
        /* SPR 22402 Fix End */
        return RELAY_GTPU_SUCCESS;
    }  
    /* SPR 22402 Fix Start */
    msgFree(cpPathSuccessMsg_p);
    /* SPR 22402 Fix End */
    /* - COVERITY 11259 */
    return RELAY_GTPU_FAILURE;
}
/*-SPR #3456*/

/* SPR 16770 fix start */
/****************************************************************************
 * Function Name  : s1uPrepareForwardingReq
 * Inputs         : ueIndex, prDataFwdReq_p
 * Outputs        : None
 * Returns        : None
 * Description    : This funtion prepares and send forwarding request to PDCP
 ****************************************************************************/
void s1uPrepareForwardingReq( UInt16 ueIndex )
{
    prDataFwdReq prDataFwdReqObj = {0};
    prDataFwdReq *prDataFwdReq_p = &prDataFwdReqObj;
    UInt32 transId = 0;
    UInt8 lcIndex = 0;
    UInt8 counter = 0;
    UInt8 cause = 0;
    /*S1U_DELETE_FIX*/

    /* Fill Ue Index for Forwarding request to PDCP */
    prDataFwdReqObj.ueIndex = ueIndex;

    for( lcIndex = 0 ; lcIndex < MAX_RAB_ID ; lcIndex++)
    {
        if( RELAY_GTPU_IN_USE == 
                relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].inUseFlag )
        {
            transId = relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].transId;
            cause = s1ReconfigSapCnf_g.setupSapCnf[lcIndex].cause;

            /* Incase of Indirect Data FWD tunnel for source eNB,
             * prepare DataFwdingReq message for PDCP */
            if( (relayMapBasedOnTransId_g[transId].sapFlags & PR_EGTPU_TX_SAP) && 
                    (relayMapBasedOnTransId_g[transId].sapFlags & PR_EGTPU_FWD_SAP &&
                    !cause ) )
            {
                /* Fill forwarding for this RAB if DL forwarding tunnel is 
                 * present */
                if( INVALID_TUNNEL_ID != 
                        relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].
                        tunnelDlFwdId )
                {
                    prDataFwdReqObj.dataFwdInfo[counter].lcId = (lcIndex + 3);
                    prDataFwdReqObj.dataFwdInfo[counter].ulDataFwdFlag = LTE_FALSE;

                    /* Fill ulDataFwdFlag only if tunnelUlFwdId is valid */
                    if( INVALID_TUNNEL_ID != 
                            relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].
                            tunnelUlFwdId )
                    {
                        prDataFwdReqObj.dataFwdInfo[counter].ulDataFwdFlag = 
                            LTE_TRUE;
                    }
                    LOG_GTPU_MSG(PR_HO_SRC_DATA_FWD_IND, LOGINFO, PR_GTPU_DL_DATA,
                            relayGtpuCurrentTick_g, __LINE__, ueIndex, counter,
                            prDataFwdReqObj.dataFwdInfo[counter].lcId ,
                            prDataFwdReqObj.dataFwdInfo[counter].ulDataFwdFlag,
                            0,0,__func__,"");

                    counter++;
                }
            }
        }
    }

    /* Fill the number of LCs for which forwarding tunnels are created by L3 */
    prDataFwdReqObj.count = counter
    ;
    if( 0 != counter 
        )
    {
        /* Send Forwarding request to PDCP if for atleast 1 RAB fwding request
         * is received from L3 */
        /*SPR 21366 +*/
        egtpu_rab_ctxt_t *p_rab_ctxt = egtpu_db_sel_teid_key_rab_ctxt(p_global,
                relayMapBasedOnUeIndexLcId_g[ueIndex][0].tunnelId);
        if(PNULL != p_rab_ctxt)
        {
            if(counter)
            {

            pdcpSourceDataFwdReq((void*)prDataFwdReq_p,sizeof(prDataFwdReq)
                    ,gtpu_Pdcp_Instance_g[p_rab_ctxt->teid_cell.cellIndex] );
        }
        }
        /*SPR 21366 -*/
    }
}
/* SPR 16770 fix end */

/****************************************************************************
 * Function Name  : processGtpuSetupSapCnf
 * Inputs         : setupSapCnf_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process SAP setup confirmation.
 ****************************************************************************/
UInt32 processGtpuSetupSapCnf
(
    UInt8 *setupSapCnf_p,
    UInt16 msgLen
)
{
    UInt16 current = CSPL_HDR, ueIndex= 0,addrLen = 0;
    UInt32 transId = 0, tunnelId = 0;
    UInt8 cause = 0, isSeqNoPresent = 0, isTunnelIdPresent = 0, lcId = 0; 
    UInt16 dlSeqNo = 0, ulSeqNo = 0;
    UInt8 transportAddr[RELAY_ADDR_LEN_IPV6] = {0},i;

    /* This function is called for each SETUP_SAP request received from L3 */
    /*Parse the buffer received*/
    while(current < msgLen)
    {
        switch(setupSapCnf_p[current++])
        {
            case RELAY_GTPU_IE_CAUSE:
            cause = setupSapCnf_p[current++];
            break;

            case RELAY_GTPU_IE_TRANSACTION_ID:
            transId = RELAY_GTPU_GET_U32BIT(&setupSapCnf_p[current]);
            current+=4;
            break;

            case RELAY_GTPU_IE_TEID_DATA_I:
            tunnelId = RELAY_GTPU_GET_U32BIT(&setupSapCnf_p[current]);
            current+=4;
            isTunnelIdPresent = RELAY_GTPU_SUCCESS; 
            /*SPR 2030 fix Start*/
            LOG_GTPU_MSG(PR_SETUP_SAP_CNF_TEID_FROM_GTPU, LOGINFO, 
                    PR_GTPU_CONTROL, relayGtpuCurrentTick_g, __LINE__,
                    tunnelId,0,0,0, 0,0, __func__,""); 
            /*SPR 2030 fix End*/
            break;

            case RELAY_GTPU_IE_SEQ_NUM:
            ulSeqNo = RELAY_GTPU_GET_U16BIT(&setupSapCnf_p[current]);
            current +=3;
            dlSeqNo = RELAY_GTPU_GET_U16BIT(&setupSapCnf_p[current]);
            current +=2;
            isSeqNoPresent = RELAY_GTPU_SUCCESS;
            break;
            case RELAY_GTPU_IE_GSN_ADDR:
            addrLen = RELAY_GTPU_GET_U16BIT(&setupSapCnf_p[current]);
            current+=2;
            /* Coverity 73437 Fix Start */
            if(addrLen > (msgLen - current))
            {
                break;
            }
            /* Coverity 73437 Fix End */
            memCpy(transportAddr, &setupSapCnf_p[current], addrLen);
            current+=addrLen;
            break;

        }
    }
    /* Coverity 73437 Fix Start */
    if( transId >= RELAY_GTPU_MAX_TRANS_ID)
    {
        return RELAY_GTPU_FAILURE;
    }
    /* Coverity 73437 Fix End */
    if (RELAY_GTPU_IN_USE  ==
            relayMapBasedOnTransId_g[transId].inUseFlag)
    {
        UInt8 tunnelType = 0;
        UInt8 lcIndex;
        /*Extract the info based on transId received*/
        tunnelType = relayMapBasedOnTransId_g[transId].tunnelType;
        ueIndex = relayMapBasedOnTransId_g[transId].ueIndex;
        lcId = relayMapBasedOnTransId_g[transId].lcId;
        lcIndex = lcId - 3;
        s1ReconfigSapCnf_g.ueIndex = relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].rrcUeIndex;

        s1ReconfigSapCnf_g.setupSapCnf[lcIndex].bitMask |= RELAY_MANDATORY_PRESENT;
        s1ReconfigSapCnf_g.setupSapCnf[lcIndex].lcId = lcId;
        s1ReconfigSapCnf_g.setupSapCnf[lcIndex].cause |= cause;

        /* Maintaining req message count */
        relayTunnelInfo *tunnelInfo = &s1ReconfigSapCnf_g.setupSapCnf[lcIndex].
            tunnelInfo[tunnelType];

        /* Populate rbdirection & tunnel Info on the basis of tunnelType */
        if ((NORMAL_BEARER == tunnelType) && !cause)
        {
            tunnelInfo->rbDirection = DIR_BOTH_TX_RX;
            /*SPR 3736 Fix Start*/
            if(isTunnelIdPresent)
            {
                relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelId = tunnelId;
            }
            /*SPR 3736 Fix End*/
        }

        else if((DL_FORWARDING == tunnelType) && !cause)
        {
            tunnelInfo->rbDirection = DIR_TX;
            /*SPR 3736 Fix Start*/
            if(isTunnelIdPresent)
            {
                relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelDlFwdId = tunnelId;
            }
            /*SPR 3736 Fix End*/
        }

        else if ((UL_FORWARDING == tunnelType) && !cause)
        {
            tunnelInfo->rbDirection = DIR_RX;
            /*SPR 3736 Fix Start*/
            if(isTunnelIdPresent)
            {
                relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelUlFwdId = tunnelId;
            }
            /*SPR 3736 Fix End*/
        }

#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
        else if ((SCG_SPLIT_BEARER == tunnelType) && !cause)
        {
            tunnelInfo->rbDirection = DIR_BOTH_TX_RX;
            if(isTunnelIdPresent)
            {
                if (relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelId == INVALID_TUNNEL_ID)
                {
                    relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelId = tunnelId;
                }
                if (relayMapBasedOnUeIndexLcId_g[ueIndex][lcId - 3].dcTunnelId == INVALID_TUNNEL_ID)
                {
                    relayMapBasedOnUeIndexLcId_g[ueIndex][lcId - 3].dcTunnelId = tunnelId;
                }

            }
        }
#endif

        /* Check whether tunnel id is present or not */
        if (isTunnelIdPresent)
        { 
            /*SPR 2952 Changes Starts*/
            tunnelInfo->bitMask |= RELAY_MANDATORY_PRESENT;
            tunnelInfo->cause = cause;
            /*SPR 2952 Changes End*/
            /*if tunnel id present , transport address is also
             * present*/
            tunnelInfo->bitMask |= RELAY_TUNNEL_ID_PRESENT;
            tunnelInfo->bitMask |= RELAY_GSN_ADDR_PRESENT;
            tunnelInfo->teidSelf = tunnelId;

            /*Populate the Source transport Address*/
            tunnelInfo->addrLen = addrLen;
            /* Coverity 73437 Fix Start */
            if(addrLen > RELAY_ADDR_LEN_IPV6)
            {
                return RELAY_GTPU_FAILURE;
            }
            /* Coverity 73437 Fix End */
            for(i=0;i<addrLen;i++)
            {
                tunnelInfo->transportAddr[i] = transportAddr[i];
            }

            /*Update Other Context*/ 
            /* Self TEID*/
            relayMapBasedOnTransId_g[transId].tunnelId 
                = tunnelId;

            /* To maintain mapping for data processing */
            /* Populate tunnelId on the basis of tunnelType */
            relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].inUseFlag = RELAY_GTPU_IN_USE;
        }
        /* Check whether seq no is present or not */
        if (isSeqNoPresent)
        {
            s1ReconfigSapCnf_g.setupSapCnf[lcIndex].seqNumUplink = ulSeqNo;
            s1ReconfigSapCnf_g.setupSapCnf[lcIndex].seqNumDnlink = dlSeqNo;
            s1ReconfigSapCnf_g.setupSapCnf[lcIndex].bitMask |= RELAY_SEQ_NO_PRESENT;
        }
        /* check whether all responses are received or not */
        /* Check all SAP cnf messages received */
        if (isDelayReq_g) 
        {
            /* call function pointer to send the response to S1-AP Module */
            if (RELAY_GTPU_SUCCESS !=
                    sendConfirmationRsp[RELAY_GTPU_CP_SETUP_SAP_CNF] 
                    (NULL,transId,RELAY_GTPU_LEN_NOT_REQD))
            {
                /* SPR 2030 fix Start*/
                LOG_GTPU_MSG(PR_SEND_CNF_TO_CP_FAIL,LOGWARNING, PR_GTPU_CONTROL,
                        relayGtpuCurrentTick_g, __LINE__,
                        0,0,0,0, 0,0, __func__,"RELAY_GTPU_CP_SETUP_SAP_CNF"); 
                /* SPR 2030 fix End*/
                return RELAY_GTPU_FAILURE;
            }
            else
                    {
                /* SPR 16770 fix start */
                s1uPrepareForwardingReq( ueIndex );
                /* SPR 16770 fix end */
            }
            /*Reset the global for SETUP_SAP_CNF*/
            for(i = 0 ; i < MAX_RAB_ID ; i++)
                memSet(&s1ReconfigSapCnf_g.setupSapCnf[i],0,sizeof(relaySetupCnf));
            s1ReconfigSapCnf_g.lcCount = 0;
            isDelayReq_g = RELAY_GTPU_NOT_IN_USE;
        }
    }
    else
    {
        /*SPR 2030 fix Start*/
        LOG_GTPU_MSG(PR_SETUP_SAP_CNF_NO_UE_CTXT, LOGWARNING, 
                PR_GTPU_CONTROL, relayGtpuCurrentTick_g, __LINE__,
                0,0,0,0, 0,0, __func__,""); 

        /*SPR 2030 fix End*/
        return RELAY_GTPU_FAILURE; 
    }
    return RELAY_GTPU_SUCCESS; 
}

/****************************************************************************
 * Function Name  : processGtpuReleaseSapCnf
 * Inputs         : releaseSapCnf_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process SAP Release Confirmation.
 ****************************************************************************/

UInt32 processGtpuReleaseSapCnf
(
    UInt8 *releaseSapCnf_p,
    UInt16 msgLen
)
{
    UInt16 index = CSPL_HDR, ueIndex = 0, ulSeqNo = 0, dlSeqNo = 0;
    UInt8 cause = 0, lcId = 0, isSeqNoPresent = 0;
    UInt32 transId = 0, tunnelId = 0;
    UInt8 lcIndex =0, tunnelType = 0;
    UInt8 i;

    if (RELAY_GTPU_IE_CAUSE == releaseSapCnf_p[index++]) 
    {
        cause = releaseSapCnf_p[index++];
        /* cause identifies whether transaction id is present or
           tunnel id is present */
        if (!cause)
        {
            if (RELAY_GTPU_IE_TRANSACTION_ID == releaseSapCnf_p[index++])        
            { 
                transId = RELAY_GTPU_GET_U32BIT(&releaseSapCnf_p[index]); 
                index +=4;
                if (RELAY_GTPU_IE_SEQ_NUM == releaseSapCnf_p[index++])
                {
                    ulSeqNo = RELAY_GTPU_GET_U16BIT(&releaseSapCnf_p[index]);
                    index +=3;
                    dlSeqNo = RELAY_GTPU_GET_U16BIT(&releaseSapCnf_p[index]);
                    index +=2;
                    isSeqNoPresent = RELAY_GTPU_SUCCESS;
                }
                /* Coverity 73079 Fix Start */
                if(transId >= RELAY_GTPU_MAX_TRANS_ID)
                {
                    return RELAY_GTPU_FAILURE;

                }
                /* Coverity 73079 Fix End */

                /* checks whether transId index of relayMapBasedOnTransId_g 
                   is inuse or not*/
				/* + Coverity 73079 */
				if ( transId < RELAY_GTPU_MAX_TRANS_ID)
				{
                	if (RELAY_GTPU_IN_USE ==
                        	relayMapBasedOnTransId_g[transId].inUseFlag)
                	{
                    	/* from Mapping Array finds tunnel id from transaction identifier */
                    	tunnelId = relayMapBasedOnTransId_g[transId].tunnelId;
                    	relayMapBasedOnTransId_g[transId].inUseFlag = RELAY_GTPU_NOT_IN_USE;
                    	ueIndex = relayMapBasedOnTransId_g[transId].ueIndex;
                    	lcId = relayMapBasedOnTransId_g[transId].lcId;
                    	lcIndex = lcId - 3;
                    	tunnelType = relayMapBasedOnTransId_g[transId].tunnelType;
                    	/* loop till number of lc ids */
                    	/* Decrementing the isFinished variable by 1 */
                    	s1ReconfigSapCnf_g.ueIndex = relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].rrcUeIndex;                 
                    	s1ReconfigSapCnf_g.releaseSapCnf[lcIndex].bitMask |= RELAY_MANDATORY_PRESENT;
                    	s1ReconfigSapCnf_g.releaseSapCnf[lcIndex].lcId = lcId;
                    	s1ReconfigSapCnf_g.releaseSapCnf[lcIndex].cause |= cause;

                    	/*Populate Tunnel Info*/
                    	relayRelTunnelInfo *relTunnelInfo = 
                        	&s1ReconfigSapCnf_g.releaseSapCnf[lcIndex].relTunnelInfo[tunnelType];  
                    	relTunnelInfo->bitMask |= RELAY_TUNNEL_ID_PRESENT;
                    	relTunnelInfo->teidSelf = tunnelId;

                    	if (RELAY_GTPU_SUCCESS == isSeqNoPresent)
                    	{
                        	s1ReconfigSapCnf_g.releaseSapCnf[lcIndex].bitMask |= RELAY_SEQ_NO_PRESENT;
                        	s1ReconfigSapCnf_g.releaseSapCnf[lcIndex].seqNumUplink = ulSeqNo;
                        	s1ReconfigSapCnf_g.releaseSapCnf[lcIndex].seqNumDnlink = dlSeqNo;
                    	}

                    	/*Update the Map tables*/
                    	if (NORMAL_BEARER == tunnelType)
                    	{
                        	relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelId = INVALID_TUNNEL_ID;

                    	}
                    	else if (DL_FORWARDING == tunnelType)
                    	{
                        	relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelDlFwdId = INVALID_TUNNEL_ID;
                    	}
                    	else if (UL_FORWARDING == tunnelType)
                    	{
                        	relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelUlFwdId = INVALID_TUNNEL_ID;
                    	}

#ifdef ENDC_ENABLED
						/*Dual Connectivity SeNB changes +*/
						if((SN_TERMINATED_SCG_SPLIT_BEARER == relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].dcBearerType)
							&& (relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].x2uCtxCreated == LTE_TRUE))
						{
							relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].tunnelId = INVALID_TUNNEL_ID;
							relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].dcTunnelId = INVALID_TUNNEL_ID;
						    if(RELAY_GTPU_FAILURE == x2uRelayDeleteBearerContext(
						                s1ReconfigSapCnf_g.ueIndex,
						                s1ReconfigSapCnf_g.releaseSapCnf[lcIndex].lcId))
						    {
						        return RELAY_GTPU_FAILURE;
						    }
               				relayMapBasedOnUeIndexLcId_g[ueIndex][lcIndex].x2uCtxCreated = LTE_FALSE;
						}
#ifndef EPC_SETUP
						sendTunnelInfoToDlTg(
                		    getUeIdxFromRrcUeIdx(ueIndex),
            				lcId,
            			    0xFFFFFFFF,
            				tunnelId,
            				tunnelType, (SOURCE_ENB_VALUE<<4)|NORMAL_BEARER,
            				4,
            				NULL
                				);
#endif

						/*Dual Connectivity SeNB changes -*/
#endif


                    	relayMapBasedOnTransId_g[transId].tunnelType = INVALID_TUNNEL_TYPE; 
                	}
                }
				/* - Coverity 73079 */
                /* Check whether all response is received or not */
                if(isDelayReq_g)
                {
                    /* Call function pointer to send the response to CM */
                    if (RELAY_GTPU_SUCCESS !=
                            sendConfirmationRsp[RELAY_GTPU_CP_RELEASE_SAP_CNF]
                            (NULL,
                             RELAY_GTPU_TRANS_ID_NOT_REQD,RELAY_GTPU_LEN_NOT_REQD)) 
                    {
                        /* SPR 2030 fix Start*/
                        /* +- SPR 17777 */
                        LOG_GTPU_MSG(PR_SEND_CNF_TO_CP_FAIL,LOGWARNING, PR_GTPU_CONTROL,
                                relayGtpuCurrentTick_g, __LINE__,
                                msgLen,0,0,0, 0,0, __func__,"RELAY_GTPU_CP_RELEASE_SAP_CNF"); 
                        /* SPR 2030 fix End*/
                        return RELAY_GTPU_FAILURE;
                    }
                    for(i = 0 ; i < MAX_RAB_ID ; i++)
                        memSet(&s1ReconfigSapCnf_g.releaseSapCnf[i],0,sizeof(relayReleaseCnf));
                    s1ReconfigSapCnf_g.lcCount = 0;
                    isDelayReq_g = RELAY_GTPU_NOT_IN_USE;
                }
            } 
        } 
        else
        {
            tunnelId = RELAY_GTPU_GET_U32BIT(&releaseSapCnf_p[index]);
            index +=4;
            /* Check whether all responses are received or not */
            if(isDelayReq_g)
            {
                /* Call function pointer to send the response to CM */
                if (RELAY_GTPU_SUCCESS != 
                        sendConfirmationRsp[RELAY_GTPU_CP_RELEASE_SAP_CNF]
                        (NULL,
                         RELAY_GTPU_TRANS_ID_NOT_REQD,RELAY_GTPU_LEN_NOT_REQD))
                {
                    /* SPR 2030 fix Start*/
                    LOG_GTPU_MSG(PR_SEND_CNF_TO_CP_FAIL,LOGWARNING, PR_GTPU_CONTROL,
                            relayGtpuCurrentTick_g, __LINE__,
                            0,0,0,0, 0,0, __func__,"RELAY_GTPU_CP_RELEASE_SAP_CNF"); 
                    /* SPR 2030 fix End*/
                    return RELAY_GTPU_FAILURE;
                }
                for(i = 0 ; i < MAX_RAB_ID ; i++)
                    memSet(&s1ReconfigSapCnf_g.releaseSapCnf[i],0,sizeof(relayReleaseCnf));
                s1ReconfigSapCnf_g.lcCount = 0;
                isDelayReq_g = RELAY_GTPU_NOT_IN_USE;

            }
        }
        return RELAY_GTPU_SUCCESS;
    }
    return RELAY_GTPU_FAILURE;
}

/****************************************************************************
 * Function Name  : processGtpuCpEndMarkerInd
 * Inputs         : cpEndMarkerInd_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion processes End Maker Indication received from
 *                  eGTPu.
 ****************************************************************************/

UInt32 processGtpuCpEndMarkerInd
(
    UInt8 *cpEndMakerInd_p,
    UInt16 msgLen
)
{
    UInt16 index = CSPL_HDR;
    UInt32 transId = 0 , dlFwdTransId = 0;
    UInt16 ueId;
    /* SPR 22126 fix */
    UInt32 i;
    /* SPR 22126 fix */
    UInt8 lcId;

    if (RELAY_GTPU_IE_TRANSACTION_ID == cpEndMakerInd_p[index++])
    {
        transId =  RELAY_GTPU_GET_U32BIT(&cpEndMakerInd_p[index]); 
        index +=4;
        /* From TransId, Tunnel Id is extracted. but before this first 
           Check whether this structure is inuse or not */
        if( transId <RELAY_GTPU_MAX_TRANS_ID )
        {
            /*If the tunnel is already existing the corresponding transId must
             * be in use else END MARKER received for non-existing context*/
            if (RELAY_GTPU_IN_USE  
                    == relayMapBasedOnTransId_g[transId].inUseFlag)
            {
                /* Call function handler to send the EMI to S1-AP & PDCP*/
                if (RELAY_GTPU_SUCCESS !=
                        sendConfirmationRsp[RELAY_GTPU_CP_END_MARKER_IND]
                        (NULL, transId, RELAY_GTPU_LEN_NOT_REQD))
                {
                    /* +- SPR 17777 */
                    LOG_GTPU_MSG(PR_SEND_CNF_TO_CP_FAIL,LOGWARNING, PR_GTPU_CONTROL,
                            relayGtpuCurrentTick_g, __LINE__,
                            msgLen,0,0,0, 0,0, __func__,"RELAY_GTPU_CP_END_MARKER_IND"); 
                    return RELAY_GTPU_FAILURE;
                }

                /*Search for the transId for DL forwarding tunnel*/ 
                ueId = relayMapBasedOnTransId_g[transId].ueIndex;
                lcId = relayMapBasedOnTransId_g[transId].lcId;

                for(i = 1; i< RELAY_GTPU_MAX_TRANS_ID ;i++)
                {
                    if((ueId == relayMapBasedOnTransId_g[i].ueIndex) &&
                            (lcId == relayMapBasedOnTransId_g[i].lcId) &&
                            (DL_FORWARDING == relayMapBasedOnTransId_g[i].tunnelType))
                    {
                        dlFwdTransId = i;
                        break;
                    }
                }
                /*Call function handler to send EMI to PDCP*/
                /*Check if source or target if target no need to send EMI to PDCP*/
                if(dlFwdTransId != 0)
                {
                    if((relayMapBasedOnTransId_g[dlFwdTransId].sapFlags & 
                                PR_EGTPU_TX_SAP) &&
                            (relayMapBasedOnTransId_g[dlFwdTransId].sapFlags &
                             PR_EGTPU_FWD_SAP))
                    {
                        if(RELAY_GTPU_SUCCESS != sendConfirmationRsp
                                [RELAY_GTPU_PDCP_END_MARKER_IND]
                                (NULL,transId, RELAY_GTPU_LEN_NOT_REQD))
                        {
                            LOG_GTPU_MSG(PR_SEND_IND_TO_PDCP_FAIL,LOGWARNING, 
                                    PR_GTPU_CONTROL, relayGtpuCurrentTick_g,
                                    __LINE__,0,0,0,0, 0,0, __func__,
                                    "RELAY_GTPU_PDCP_END_MARKER_IND"); 
                            return RELAY_GTPU_FAILURE;
                        }
                    }

                }
                return RELAY_GTPU_SUCCESS;
            }
            LOG_GTPU_MSG(PR_RCVD_EMI_FOR_NONEXIST_RAB, LOGWARNING, PR_GTPU_CONTROL,
                    relayGtpuCurrentTick_g, __LINE__,transId,0,0,0, 0,0, 
                    __func__,"");
            return RELAY_GTPU_FAILURE;
        }
    }

    return RELAY_GTPU_FAILURE; 
}

/* LIPA changes start */
/****************************************************************************
* Function Name  : updateRelayMapTableOnTransIdForSecondaryTunnel
* Inputs         : ueIndex - UE ID,
*                  lcId -Logical channel ID,
*                  transIdForRsp - Transaction ID for Response,
*                  apiIdForRsp - API ID  for Response,
*                  transId - Transaction ID,
*                  sapFlagValue,
*                  tunnelIDSelf
* Outputs        : None
* Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE 
* Description    : This funtion populates tunnelType on the basis of sapFlagValue
*                  as input.
****************************************************************************/
UInt8 updateRelayMapTableOnTransIdForSecondaryTunnel(
                                   UInt16 ueIndex,
                                   UInt8 lcId,
                                   UInt32 transIdForRsp, 
                                   UInt16 apiIdForRsp,
                                   UInt32 transId,
                                   UInt8  sapFlagValue,
                                   UInt32 *tunnelIDSelf)
{
    relayMapBasedOnTransIdForSecTunnel_g[transId].inUseFlag 
        = RELAY_GTPU_IN_USE;
    relayMapBasedOnTransIdForSecTunnel_g[transId].ueIndex = ueIndex;
    relayMapBasedOnTransIdForSecTunnel_g[transId].lcId = lcId;
    /* Since Secondary Tunnel Id is unknown currently so setting it to
       default value */
    relayMapBasedOnTransIdForSecTunnel_g[transId].tunnelId 
        = relayMapBasedOnUeIndexLcId_g[ueIndex][lcId - 3].tunnelId;
    /* Storing the Tunnel ID Self for Secondary Tunnel Id */ 
    *tunnelIDSelf = relayMapBasedOnTransIdForSecTunnel_g[transId].tunnelId;
    /* store the SAP flag value for Secondary Tunnel in global structure*/
    relayMapBasedOnTransIdForSecTunnel_g[transId].sapFlags = sapFlagValue;

    relayMapBasedOnTransIdForSecTunnel_g[transId].tunnelType = NORMAL_BEARER;
    s1uUEReconfigCnfInfo_g[ueIndex].rbInfo[lcId-3].isNormalTunnelPresentFlag = 1;

    /* Allocating Memory on index transId for pointer of array 
       relayUeCnxtMngmntForTransId_g */
    s1ReconfigSapCnf_g.ueIndex = relayMapBasedOnUeIndexLcId_g[ueIndex][lcId - 3].rrcUeIndex;
    s1ReconfigSapCnf_g.transIdForRsp = transIdForRsp;
    s1ReconfigSapCnf_g.apiIdForRsp = apiIdForRsp;

    return RELAY_GTPU_SUCCESS;
}
/* LIPA changes end */


