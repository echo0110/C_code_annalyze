#ifdef ENDC_ENABLED
/******************************************************************************
 * *
 * *  ARICENT -
 * *
 * *  Copyright (C) 2015 Aricent Inc. All Rights Reserved.
 * *
 * ******************************************************************************
 * *
 * *  File Name: lteRelayX2uSap.c
 * *
 * ******************************************************************************
 * 
 *  File Description : This file includes the functionality of handling of APIs
 *                       received from RRC at packetRelay.
 *                   1. X2U_LC_RELEASE_IND
 *                   2. X2U_SUSPEND_UE_ENTITY_REQ
 *                   3. X2U_SUSPEND_UE_ENTITY_CNF 
 * 
 * *****************************************************************************/

/******************************************************************************
 *  Standard Library Includes
 *******************************************************************************/



/******************************************************************************
 *  Project Includes
 *******************************************************************************/
#include "lteMemPool.h"
#include "lteMisc.h"
#include "lteRelayX2uSap.h"
/******************************************************************************
 * Exported Variables
 *******************************************************************************/

/******************************************************************************
 * Exported Definitions
 *******************************************************************************/
extern void startGuardTimerAndReleaseBearers(UInt16 ueIndex);
extern void suspendSplitBearers(UInt16 ueIndex);

/******************************************************************************
 *   Private Definitions
 *******************************************************************************/

/****************************************************************************
 * Function Name  : processX2uLcReleaseInd
 * Inputs         : lcReleaseInd_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process X2U_LC_RELEASE_IND
 ****************************************************************************/
UInt32 processX2uLcReleaseInd(UInt8 *lcReleaseInd_p, UInt16 msgLen)
{
    UInt8  current = 0;   

    UInt16 ueIndex , rrcUeIndex ;

    if (PNULL != lcReleaseInd_p)
    {   

            current = API_HDR;        

            rrcUeIndex = RELAY_GTPU_GET_U16BIT(&lcReleaseInd_p[current]);
            ueIndex = getUeIdxFromRrcUeIdx(rrcUeIndex);

            /*fetch X2U UE context and check validity*/ 
            /*start a guard timer for completion of DL data forwarding and 
              will release the bearer(s) in case forwarding is not completed 
              prior to expiry of guard timer bearer(s). */ 
            startGuardTimerAndReleaseBearers(ueIndex); 

            LOG_GTPU_MSG(PR_RCVD_S1U_CREATE_UE_ENTITY_REQ, LOGINFO, 
                    PR_GTPU_CONTROL, relayGtpuCurrentTick_g, __LINE__, 
                    ueIndex, 0,0,0,0,0,__func__,"");


        return RELAY_GTPU_SUCCESS;
    }    
    LOG_GTPU_MSG(PR_SETUP_SAP_REQ_EMPTY, LOGWARNING, PR_GTPU_CONTROL,
            relayGtpuCurrentTick_g,__LINE__,0,0,0,0,
            0,0,__func__,"");
    return RELAY_GTPU_FAILURE;
}

/****************************************************************************
 * Function Name  : processX2uSuspendUEEntityRequest
 * Inputs         : suspendUeEntityReq_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process X2U_SUSPEND_UE_ENTITY_REQ request.
 ****************************************************************************/
UInt32 processX2uSuspendUEEntityRequest(UInt8 *suspendUeEntityReq_p, UInt16 msgLen)
{
    UInt8 respMsg[20] = {0};
    UInt8  current = 0;   

    UInt16 transId = 0, apiId = 0 , ueIndex , rrcUeIndex ; 

    RrcCellIndex cellIndex;

    if (PNULL != suspendUeEntityReq_p)
    {   
        cellIndex = suspendUeEntityReq_p[API_HEADER_CELL_INDEX_OFFSET];


        transId = RELAY_GTPU_GET_U16BIT(&suspendUeEntityReq_p[current]);
        current = API_HDR;        

        rrcUeIndex = RELAY_GTPU_GET_U16BIT(&suspendUeEntityReq_p[current]);
        ueIndex = getUeIdxFromRrcUeIdx(rrcUeIndex);

        /*fetch X2U UE context and check validity*/ 
           
        suspendSplitBearers(ueIndex); 

        LOG_GTPUPR_INFO(PR_GTPU_CONTROL,
                           "[%s] SuspendSplitBearers of ueIndex [%u] ",__func__, ueIndex
                        );  

        apiId = X2U_SUSPEND_UE_ENTITY_CNF;

        RELAY_GTPU_SET_U16BIT(&respMsg[current], rrcUeIndex);

        current += 2;
        respMsg[current] = GTPU_CAUSE_SUCCESS;  /* Cause */
        current++;

        LOG_GTPUPR_INFO(PR_GTPU_CONTROL,
                           "[%s] Preparing X2U_SUSPEND_UE_ENTITY_CNF  ueIndex [%u] ",
                             __func__, ueIndex);  

        /* Append API Header in front of message for sending to S1-AP */

        appendApiHeader(RELAY_S1AP_MODULE_ID, apiId, transId,
                    current,respMsg, cellIndex);
        /* Sending to S1-AP */
        udpSendToS1ap((Char8*)respMsg, current);

        return RELAY_GTPU_SUCCESS;
    }    

    return RELAY_GTPU_FAILURE;
}

#endif

