/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: ltePdcpAdapterS1Sap.c,v 1.1.14.1 2010/09/21 15:53:13 gur20491 Exp $
 *
 ******************************************************************************
 *
 *  File Description : S1 SAP Interface Implementation 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: ltePdcpAdapterS1Sap.c,v $
 * Revision 1.1.14.1  2010/09/21 15:53:13  gur20491
 * FAPI changes
 *
 * Revision 1.1  2010/04/15 10:33:59  gur20619
 * .
 *
 * Revision 1.1  2010/04/15 09:10:34  gur20619
 * pdcp adapter changes for moving the relay functionality
 *
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
#include<stdio.h>
#include<stdlib.h>
#include "lteRelay.h"
//#include "lteRelayCommon.h"
#include "lteRelayS1Sap.h"
/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
//extern UInt32 handleGtpuSetupSapReq(relaySetupSapReq *setupSapReq_p);
//extern UInt32 handleGtpuReleaseSapReq(gtpuRelSapReq *relSapReq_p);
extern UInt32 processRelaySetupSapCnf( UInt8 *inMsg_p, UInt16 transId);

/****************************************************************************
 * Functions implementation
 ****************************************************************************/
UInt32 getTunnelId(void)
{
  static int tunnelIdentifier = 1;
  return tunnelIdentifier ++;
}

/****************************************************************************
 * Function Name  : processGtpuSetupSapReq
 * Inputs         : setupSapReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process SAP setup request.
 ****************************************************************************/

UInt32 processGtpuSetupSapReq
(
    UInt8 *sapReq_p
)
{
    printf("Handling processGtpuSetupSapReq\n");
    relayGtpuSetupSapReq setupSapReq = {0};
    UInt8 startLc = 0, lcCnt = 0;
    const tunnelIdTemp = 1;
    UInt32 transId = 0, prevTidIndex = 0, tunnelId = 0, transIdForRsp = 0, index = 0;
    UInt16 current = 0, length =0;
    UInt32 isTransIdFlagTrue = RELAY_GTPU_TRANS_ID_TRUE;
    UInt32 isTunnelIdFlagTrue = RELAY_GTPU_TRANS_ID_TRUE;
    /* Following three elements are needed for handling failure scenarios */
    UInt8 totalNoOfFailure = 0;
    UInt8 lcIdArr[RELAY_GTPU_MAX_LC] = {0};
    UInt32 tunnelIdArr[RELAY_GTPU_MAX_LC] = {0};
    if (PNULL != sapReq_p)
    {
        /* Extracting the transaction identifier */
        transIdForRsp 
            =  RELAY_GTPU_GET_U16BIT(&sapReq_p[current]);
        current +=2;//Ashish - What is the use of this instruction?
        /* Incrementing the current by 10. starting 10 bytes will be
           API Header */
        current  = API_HDR;
        setupSapReq.ueIndex = RELAY_GTPU_GET_U16BIT(&sapReq_p[current]);
        current +=2;
        setupSapReq.lcCount =  sapReq_p[current++];
        for (startLc = 0; startLc < setupSapReq.lcCount; startLc++)
        {
            setupSapReq.setupSap[startLc].lcId = sapReq_p[current++];
            setupSapReq.setupSap[startLc].qosId = RELAY_GTPU_GET_U32BIT(&sapReq_p[current]);
            current +=4;

            /* check whther Tunnel Id is present or not */
            if (RELAY_GTPU_IE_TEID_DATA_I == sapReq_p[current])
            {
                setupSapReq.setupSap[startLc].teidSelf.tag = sapReq_p[current++];
                setupSapReq.setupSap[startLc].teidSelf.tunnelId 
                    = RELAY_GTPU_GET_U32BIT(&sapReq_p[current]);
                current +=4;
            }
            setupSapReq.setupSap[startLc].sapFlag.tag = sapReq_p[current++];
            if (RELAY_GTPU_IE_SAP_FLAG != setupSapReq.setupSap[startLc].sapFlag.tag)
            {
                totalNoOfFailure += 1;
                lcIdArr[totalNoOfFailure] = setupSapReq.setupSap[startLc].lcId;
                printf("parameter Parsing Failed for SAP-FLAG\n");
                continue;
            }
            setupSapReq.setupSap[startLc].sapFlag.value = sapReq_p[current++];
            setupSapReq.setupSap[startLc].qosProfile.tag = sapReq_p[current++];
            if (RELAY_GTPU_IE_QOS_PROFILE 
                    != setupSapReq.setupSap[startLc].qosProfile.tag)
            {
                printf("parameter Parsing Failed for QOS Profile\n");
                lcIdArr[totalNoOfFailure] = setupSapReq.setupSap[startLc].lcId;
                totalNoOfFailure += 1;
                continue;
            }
            setupSapReq.setupSap[startLc].qosProfile.length = RELAY_GTPU_GET_U16BIT(&sapReq_p[current]);
            current +=2;
            /* Allocating memory for Qos Profile Data */
            setupSapReq.setupSap[startLc].qosProfile.data = 
                (UInt8 *)getMemFromPool((setupSapReq.setupSap[startLc].qosProfile.length), PNULL); 
            //qvAlloc(setupSapReq.setupSap[startLc].qosProfile.length,PNULL); 
            if (PNULL == setupSapReq.setupSap[startLc].qosProfile.data)
            {
                printf("Memory Allocation Failed [Error]\n");
                return RELAY_GTPU_FAILURE;
            }
            memCpy(setupSapReq.setupSap[startLc].qosProfile.data,&sapReq_p[current], \
                    setupSapReq.setupSap[startLc].qosProfile.length);
            current += setupSapReq.setupSap[startLc].qosProfile.length;
            /* check whether Seq NO Tag is present or not */
            if (RELAY_GTPU_IE_SEQ_NUM == sapReq_p[current])
            {
                setupSapReq.setupSap[startLc].seqNumUplink.tag 
                    = sapReq_p[current++];
                setupSapReq.setupSap[startLc].seqNumUplink.SquenceNo
                    = sapReq_p[current++];
                setupSapReq.setupSap[startLc].seqNumDnlink.tag 
                    = sapReq_p[current++];
                setupSapReq.setupSap[startLc].seqNumDnlink.SquenceNo 
                    = sapReq_p[current++];
            }
            if (RELAY_GTPU_IE_TEID_DATA_I == sapReq_p[current])
            {
                setupSapReq.setupSap[startLc].teidPeer.tag = sapReq_p[current++];
                setupSapReq.setupSap[startLc].teidPeer.tunnelId 
                    = RELAY_GTPU_GET_U32BIT(&sapReq_p[current]);
                fprintf(stderr,"Peer Tunnel Id Received from S1-AP is %d\n",setupSapReq.setupSap[startLc].teidPeer.tunnelId);
                current +=4;
                if (RELAY_GTPU_IE_GSN_ADDR == sapReq_p[current])
                {
                    setupSapReq.setupSap[startLc].transportAddr.tag = sapReq_p[current++];
                    length = setupSapReq.setupSap[startLc].transportAddr.length  
                        = RELAY_GTPU_GET_U16BIT(&sapReq_p[current]);
                    current +=2;
                    //                   setupSapReq.setupSap[startLc].transportAddr.data = qvAlloc(length - RELAY_TAG_LEN, PNULL);
                    setupSapReq.setupSap[startLc].transportAddr.data = (UInt8 *)getMemFromPool((length - RELAY_TAG_LEN),PNULL);
                    if (!setupSapReq.setupSap[startLc].transportAddr.data)
                    {
                        printf("Memory Allocation Failed [Error]\n");
                        return RELAY_GTPU_FAILURE;
                    }
                    memCpy(setupSapReq.setupSap[startLc].transportAddr.data,&sapReq_p[current], \
                            setupSapReq.setupSap[startLc].transportAddr.length - RELAY_TAG_LEN);
                    current += setupSapReq.setupSap[startLc].transportAddr.length - RELAY_TAG_LEN;
                }
            }
            if (RELAY_GTPU_IE_SEQ_DISABLE == sapReq_p[current])
            {
                setupSapReq.setupSap[startLc].seqFlag.tag 
                    = sapReq_p[current++];
                setupSapReq.setupSap[startLc].seqFlag.value
                    = sapReq_p[current++];
            }
            /* check whether reordering TAG is present or not */
            if (RELAY_GTPU_IE_REORDERING_REQD == sapReq_p[current])
            {
                setupSapReq.setupSap[startLc].reorderingReqd.tag = sapReq_p[current++];
                setupSapReq.setupSap[startLc].reorderingReqd.value = sapReq_p[current++];
            }

            UeContextForRsp inMsg = {0};
            UInt16 dlSeqNo = 0, ulSeqNo = 0;

            inMsg.ueIndex = setupSapReq.ueIndex;
            inMsg.lcCount = setupSapReq.lcCount;
            inMsg.tid = transIdForRsp;
            inMsg.isFinished = 0;//This is not used in S1AP

            inMsg.ueContext[startLc].bitMask = 1;
            inMsg.ueContext[startLc].lcId = setupSapReq.setupSap[startLc].lcId;
            inMsg.ueContext[startLc].transId = transIdForRsp;
            inMsg.ueContext[startLc].tunnelId = getTunnelId();

            printf("ueIndex is [%d], lcId [%d], transIdForRsp[%d]\n",setupSapReq.ueIndex,setupSapReq.setupSap[startLc].lcId, transIdForRsp);
            relayMapBasedOnTunnelId_g[inMsg.ueContext[startLc].tunnelId].inUseFlag = RELAY_GTPU_IN_USE;

            //inMsg.ueContext[startLc].cause = RELAY_GTPU_IE_CAUSE;
            inMsg.ueContext[startLc].cause = 0;
            inMsg.ueContext[startLc].ulSeqNo = ulSeqNo; 
            inMsg.ueContext[startLc].dlSeqNo = dlSeqNo;
            //Send confirmation response to S1AP 
            if(RELAY_GTPU_SUCCESS == processRelaySetupSapCnf((UInt8 *)&inMsg, transIdForRsp))
            {
                tunnelId = setupSapReq.setupSap[startLc].teidSelf.tunnelId;
                //index = GET_TUNNEL_ID(tunnelId);
                index = tunnelId;
                /* Check whether tunnelId index of array relayMapBasedOnTunnelId_g
                   is inuse or not. It should be in use. if it is  not in use then
                   it means this tunnel id is wrong */
                if (RELAY_GTPU_NOT_IN_USE ==
                        relayMapBasedOnTunnelId_g[index].inUseFlag)
                {
                    /* Assiging the ueIndex and lcId to this index */
                    relayMapBasedOnTunnelId_g[index].inUseFlag= RELAY_GTPU_IN_USE;
                    relayMapBasedOnTunnelId_g[index].ueIndex 
                        = setupSapReq.ueIndex;
                    relayMapBasedOnTunnelId_g[index].lcId 
                        = setupSapReq.setupSap[startLc].lcId;

                    //SUMIT
                    relayMapBasedOnPeerTunnelId_g[setupSapReq.ueIndex][setupSapReq.setupSap[startLc].lcId].tunnelId = 
                        setupSapReq.setupSap[startLc].teidPeer.tunnelId;
                    //SUMIT
                }
                else
                {
                    printf("Tunnel Id is wrong for ueIndex [%d] \
                            and lcId [%d]\n",setupSapReq.ueIndex,setupSapReq.setupSap[startLc].lcId);
                    tunnelIdArr[totalNoOfFailure] = tunnelId;
                    totalNoOfFailure += 1;

                }
                /* Now free the memory pointed by QOS profile Data */
                //qvFree(setupSapReq.setupSap[startLc].qosProfile.data);
                freeMemPool(setupSapReq.setupSap[startLc].qosProfile.data);
                setupSapReq.setupSap[startLc].qosProfile.data = PNULL;
                freeMemPool(setupSapReq.setupSap[startLc].transportAddr.data);
                setupSapReq.setupSap[startLc].transportAddr.data = PNULL;
            }
        }
        if (totalNoOfFailure)
        {
            if (startLc == totalNoOfFailure)
            {
                printf("All Requests are failures\n");
#if 0
                if (RELAY_GTPU_SUCCESS != 
                        prepareAndSendErrorReportToCP(&setupSapReq,transIdForRsp))
                {
                    printf("Not able to send response to S1-AP Module\n");
                    return RELAY_GTPU_FAILURE;
                }
#endif
            }
        }
        printf("Sent GTP-U SAP Request to GTP-U Module\n");
        freeMemPool(sapReq_p);
        sapReq_p = PNULL;
        return RELAY_GTPU_SUCCESS;
    }
    freeMemPool(sapReq_p);
    sapReq_p = PNULL;
    return RELAY_GTPU_FAILURE;
}

/****************************************************************************
 * Function Name  : processGtpuReleaseSapReq
 * Inputs         : releaseSapReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process SAP Release Request.
 ****************************************************************************/

UInt32 processGtpuReleaseSapReq
(
    UInt8 *releaseSapReq_p
)
{
    printf("Handling processGtpuReleaseSapReq\n");
    UInt16 current = 0, startVal = 0;
    UInt32 tunnelId = 0, transId = 0, prevIndex = 0, index = 0;
    UInt8 isTunnelIdDefined = RELAY_GTPU_SUCCESS;
    relayS1RelSapReq releaseReq = {0};
    transId = RELAY_GTPU_GET_U16BIT(&releaseSapReq_p[current]);
    current +=2;
    current = API_HDR;
    releaseReq.teidCount = releaseSapReq_p[current++];
    UeContextForRsp inMsg = {0};
    for (startVal = 0; startVal < releaseReq.teidCount; startVal++)
    {
        releaseReq.relSapReq[startVal].teidSelf.tunnelId 
            = RELAY_GTPU_GET_U32BIT(&releaseSapReq_p[current]);
#if 0
        current +=4;

        /* Call handleGtpuReleaseSapReq for further processing */
        if (RELAY_GTPU_SUCCESS 
            != handleGtpuReleaseSapReq((gtpuRelSapReq*)&releaseReq.relSapReq[startVal]))
        { 
            printf("Not Able to Process the message\n");
            return RELAY_GTPU_FAILURE;
        }
#endif        

        //Ashish
            inMsg.ueIndex = relayMapBasedOnTunnelId_g[releaseReq.relSapReq[startVal].teidSelf.tunnelId].ueIndex;
            inMsg.lcCount = releaseReq.teidCount ;
            inMsg.tid = transId;
            inMsg.isFinished = 0;//This is not used in S1AP
            inMsg.ueContext[startVal].bitMask = 1;
            inMsg.ueContext[startVal].lcId = relayMapBasedOnTunnelId_g[releaseReq.relSapReq[startVal].teidSelf.tunnelId].lcId;
            inMsg.ueContext[startVal].transId = transId;
            inMsg.ueContext[startVal].tunnelId = releaseReq.relSapReq[startVal].teidSelf.tunnelId;

            relayMapBasedOnTunnelId_g[inMsg.ueContext[startVal].tunnelId].inUseFlag = RELAY_GTPU_NOT_IN_USE;
             
            inMsg.ueContext[startVal].cause = RELAY_GTPU_IE_CAUSE;
            inMsg.ueContext[startVal].ulSeqNo = 0;//ulSeqNo; 
            inMsg.ueContext[startVal].dlSeqNo = 0;//dlSeqNo;
            if (RELAY_GTPU_IN_USE == relayMapBasedOnTunnelId_g[releaseReq.relSapReq[startVal].teidSelf.tunnelId].inUseFlag)
            {
              if(RELAY_GTPU_SUCCESS == processRelayReleaseSapCnf((UInt8 *)&inMsg, transId))
              {
                      //ueIndex = relayMapBasedOnTunnelId_g[releaseReq.relSapReq[startVal].teidSelf.tunnelId].ueIndex;
                      //lcId = relayMapBasedOnTunnelId_g[releaseReq.relSapReq[startVal].teidSelf.tunnelId].lcId;
#if 0
                      /* Deleting the mapping based on ueIndex and LcId. For that
                         First it checks whether these indexs are in use or not */
                      if (RELAY_GTPU_IN_USE
                                      == relayMapBasedOnUeIndexLcId_g[ueIndex][lcId].inUseFlag)
                      {
                              /* Deleting the mapping now */
                              relayMapBasedOnUeIndexLcId_g[ueIndex][lcId].inUseFlag
                                      = RELAY_GTPU_NOT_IN_USE;
                              relayMapBasedOnUeIndexLcId_g[ueIndex][lcId].tunnelId = 0;
                      }
#endif
                      /* Deleting the mapping based on tunnel Identifier */
                      relayMapBasedOnTunnelId_g[releaseReq.relSapReq[startVal].teidSelf.tunnelId].ueIndex = 0;
                      relayMapBasedOnTunnelId_g[releaseReq.relSapReq[startVal].teidSelf.tunnelId].lcId = 0;
                      relayMapBasedOnTunnelId_g[releaseReq.relSapReq[startVal].teidSelf.tunnelId].inUseFlag
                              = RELAY_GTPU_NOT_IN_USE;
                      printf("processGtpuReleaseSapReq>> Tunnel Id = %d freed", releaseReq.relSapReq[startVal].teidSelf.tunnelId);
              }
              else
              {
                     printf("processGtpuReleaseSapReq>> processRelayReleaseSapCnf returned Failed");
              }  
            }
            else
            {
                printf("processGtpuReleaseSapReq>> Release req received for wrong tunnelId");
            }
       #if 0 
       tunnelId = releaseReq.relSapReq[startVal].teidSelf.tunnelId;
       index = GET_TUNNEL_ID(tunnelId);
       /* this flag is required for only one time to allocate the memory for 
          pointer of relayUeCnxtMngmntForTunnelId_g */
       if(isTunnelIdDefined)
       {
           /* Check whether tunnelId index of relayUeCnxtMngmntForTunnelId_g
              is NULL or not. it should be NULL, if it is not null then it 
              means this index is alreay in use so it gives error */
           if (PNULL != relayUeCnxtMngmntForTunnelId_g[index])
           {
               printf("Duplicated Tunnel Id: Context Already exists\n");
               return RELAY_GTPU_FAILURE;
           }
           /* Now Allocating the memory for tunnelId index */
          relayUeCnxtMngmntForTunnelId_g[index] =
              qvAlloc(sizeof(UeContextForRsp),PNULL);
          /* if not able to allocate memory it returns with panic */
          if (PNULL == relayUeCnxtMngmntForTunnelId_g[index])
          {
               printf("Memory Allocation Failure\n");
               return RELAY_GTPU_FAILURE;
          }
          relayUeCnxtMngmntForTunnelId_g[index]->tid
              = transId;
          relayUeCnxtMngmntForTunnelId_g[index]->lcCount 
              = relayUeCnxtMngmntForTunnelId_g[index]->isFinished
                  = 1; 
          relayUeCnxtMngmntForTunnelId_g[index]->ueContext[startVal].tunnelId
              = tunnelId;
          isTunnelIdDefined = 0;
          /* This index on array is saved for further assignment of address to this
             chunk of memory */
          prevIndex = index;
        }
       else 
       {
           /* Now tunnelId index of array relayUeCnxtMngmntForTunnelId_g
              should be NULL */
           if ( PNULL != relayUeCnxtMngmntForTunnelId_g[index])
           {
               printf("Tunnel Id Index is inUse [Error]\n");
               return RELAY_GTPU_FAILURE;
           }
          /* Assiging address of prevIndex to current index tunnelId */
          relayUeCnxtMngmntForTunnelId_g[index]
              = relayUeCnxtMngmntForTunnelId_g[prevIndex];
          /* Incrementing the lcCount and isFinished variable by 1 */
          relayUeCnxtMngmntForTunnelId_g[index]->lcCount += 1;
          relayUeCnxtMngmntForTunnelId_g[index]->isFinished += 1;
          relayUeCnxtMngmntForTunnelId_g[index]->ueContext[startVal].tunnelId
              = tunnelId;
       }
       #endif
    }
    return RELAY_GTPU_SUCCESS;
}

/****************************************************************************
 * Function Name  : processGtpuSetupSapCnf
 * Inputs         : setupSapCnf_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process SAP setup confirmation.
 ****************************************************************************/

#if 0
UInt32 processGtpuSetupSapCnf
(
    UInt8 *setupSapCnf_p
)
{
    UInt16 current = CSPL_HDR, startVal = 0, ueIndex= 0;
    UInt32 transId = 0, tunnelId = 0;
    UInt8 cause = 0, isSeqNoPresent = 0, isTunnelIdPresent = 0, lcId = 0;
    UInt16 dlSeqNo = 0, ulSeqNo = 0;
    UInt32 index = 0;
    printf("Handling processGtpuSetupSapCnf\n");
    if (RELAY_GTPU_IE_CAUSE == setupSapCnf_p[current++]) 
    {
       cause = setupSapCnf_p[current++];
       if(RELAY_GTPU_IE_TRANSACTION_ID == setupSapCnf_p[current++])
       {
            transId =  RELAY_GTPU_GET_U32BIT(&setupSapCnf_p[current]);
            current +=4;
            if (RELAY_GTPU_IE_TEID_DATA_I == setupSapCnf_p[current++])
            {
                tunnelId = RELAY_GTPU_GET_U32BIT(&setupSapCnf_p[current]);
                fprintf(stderr,"Tunnel Id Received from GTP-U is %d\n",tunnelId);
                current +=4;
                isTunnelIdPresent = RELAY_GTPU_SUCCESS;
            }
            if (!cause)
            {
                if (RELAY_GTPU_IE_SEQ_NUM == setupSapCnf_p[current++])
                {
                    ulSeqNo = RELAY_GTPU_GET_U16BIT(&setupSapCnf_p[current]);
                    current +=2;
                }
                if (RELAY_GTPU_IE_SEQ_NUM == setupSapCnf_p[current++])
                {
                    dlSeqNo = RELAY_GTPU_GET_U16BIT(&setupSapCnf_p[current]);
                    current +=2;
                }
                isSeqNoPresent = RELAY_GTPU_SUCCESS;
            }
            if (PNULL != relayUeCnxtMngmntForTransId_g[transId])
            {
                for (startVal = 0; 
                   startVal < relayUeCnxtMngmntForTransId_g[transId]->lcCount;
                       startVal++)
                {
                   /* check whether both trans Id are same or not */
                   if(relayUeCnxtMngmntForTransId_g[transId]->ueContext[startVal].transId
                        == transId)
                   {
                       relayUeCnxtMngmntForTransId_g[transId]->isFinished -=1;
                       relayUeCnxtMngmntForTransId_g[transId]->ueContext[startVal].cause
                           = cause;
                       /* Check whether tunnel id is present or not */
                       if (isTunnelIdPresent)
                       {
                           ueIndex = relayUeCnxtMngmntForTransId_g[transId]->ueIndex;
                           lcId = relayUeCnxtMngmntForTransId_g[transId]->ueContext[startVal].lcId;
                           relayUeCnxtMngmntForTransId_g[transId]->ueContext[startVal].tunnelId
                               = relayMapBasedOnTransId_g[transId].tunnelId 
                                  = tunnelId;
                           relayUeCnxtMngmntForTransId_g[transId]->ueContext[startVal].bitMask
                              |= RELAY_TUNNEL_ID_PRESENT;
                           /* To maintain mapping for data processing */
                           relayMapBasedOnUeIndexLcId_g[ueIndex][lcId].tunnelId = tunnelId;
                           relayMapBasedOnUeIndexLcId_g[ueIndex][lcId].inUseFlag = RELAY_TUNNEL_ID_PRESENT;
                           /* To maintain reserve mapping for data processing */
                           index = GET_TUNNEL_ID(tunnelId);
                           relayMapBasedOnTunnelId_g[index].ueIndex = ueIndex;
                           relayMapBasedOnTunnelId_g[index].lcId = lcId;
                           relayMapBasedOnTunnelId_g[index].inUseFlag = RELAY_TUNNEL_ID_PRESENT;
                       }
                       /* Check whether seq no is present or not */
                       if (isSeqNoPresent)
                       {
                           relayUeCnxtMngmntForTransId_g[transId]->ueContext[startVal].dlSeqNo
                              = dlSeqNo;
                           relayUeCnxtMngmntForTransId_g[transId]->ueContext[startVal].ulSeqNo
                              = ulSeqNo;
                           relayUeCnxtMngmntForTransId_g[transId]->ueContext[startVal].bitMask
                              |= RELAY_SEQ_NO_PRESENT;
                       }
                       break;                
                   } 
                }
                /* check whether all responses are received or not */
                if (RELAY_RECV_ALL_RSP 
                    == relayUeCnxtMngmntForTransId_g[transId]->isFinished)
                {
                    /* call function pointer to send the response to S1-AP Module */
                    if (RELAY_GTPU_SUCCESS !=
                        sendConfirmationRsp[RELAY_GTPU_CP_SETUP_SAP_CNF] 
                            ((UInt8*)relayUeCnxtMngmntForTransId_g[transId],RELAY_GTPU_TRANS_ID_NOT_REQD,RELAY_GTPU_LEN_NOT_REQD))
                    {
                        printf("Not able to send response to CP\n");
                        return RELAY_GTPU_FAILURE;
                    }
                    /* free the memory allocated during request time */
                    qvFree(relayUeCnxtMngmntForTransId_g[transId]);
                    /* Resetting the pointer to NULL */
                    relayUeCnxtMngmntForTransId_g[transId] = PNULL;
                }
                else
                    /* Resetting the pointer to NULL */
                    relayUeCnxtMngmntForTransId_g[transId] = PNULL;
                return RELAY_GTPU_SUCCESS;
            }
            else
            {
                printf("UE Context was not created\n");
                return RELAY_GTPU_FAILURE; 
            }
       }
       else
       {
            tunnelId = RELAY_GTPU_GET_U32BIT(&setupSapCnf_p[current]);
            index = GET_TUNNEL_ID(tunnelId);
            current +=4;
            if (!cause)
            {
                if (RELAY_GTPU_IE_SEQ_NUM == setupSapCnf_p[current++])
                {
                    ulSeqNo = RELAY_GTPU_GET_U16BIT(&setupSapCnf_p[current]);
                    current +=2;
                }
                if (RELAY_GTPU_IE_SEQ_NUM == setupSapCnf_p[current++])
                {
                    dlSeqNo = RELAY_GTPU_GET_U16BIT(&setupSapCnf_p[current]);
                    current +=2;
                }
                isSeqNoPresent = RELAY_GTPU_SUCCESS;
            }
            /* check whether tunnelId index is valid or not */
            if (PNULL != relayUeCnxtMngmntForTunnelId_g[index])
            {
               /* loop till no of lc ids */
               for (startVal = 0; 
                   startVal < relayUeCnxtMngmntForTunnelId_g[index]->lcCount;
                       startVal++) 
               {
                   /* Check whether both tunnel ids are same or not */ 
                   if(relayUeCnxtMngmntForTunnelId_g[index]->ueContext[startVal].tunnelId
                        == tunnelId)
                   {
                       relayUeCnxtMngmntForTunnelId_g[index]->isFinished -=1;
                       relayUeCnxtMngmntForTunnelId_g[index]->ueContext[startVal].cause
                           = cause; 
                       /* check whether this flag is true, if yes then assign 
                          UL and DL Seq No and also set the bitmask */
                       if(isSeqNoPresent)
                       {
                           relayUeCnxtMngmntForTunnelId_g[index]->ueContext[startVal].dlSeqNo
                               = dlSeqNo;
                           relayUeCnxtMngmntForTunnelId_g[index]->ueContext[startVal].ulSeqNo
                               = ulSeqNo;
                           relayUeCnxtMngmntForTunnelId_g[index]->ueContext[startVal].bitMask
                               = RELAY_SEQ_NO_PRESENT;
                       }
                       break;
                   }
               }
               /* Check whether all responses are received or not */
               if (RELAY_RECV_ALL_RSP == relayUeCnxtMngmntForTunnelId_g[index]->isFinished)
               {
                   /* call function pointer to send the response to S1-AP Module */
                   if (RELAY_GTPU_SUCCESS !=
                       sendConfirmationRsp[RELAY_GTPU_CP_SETUP_SAP_CNF]
                           ((UInt8*)relayUeCnxtMngmntForTunnelId_g[index],RELAY_GTPU_TRANS_ID_NOT_REQD,RELAY_GTPU_LEN_NOT_REQD))
                   {
                       printf("Not able to send to S1-AP Modulen");
                       return RELAY_GTPU_FAILURE;
                   }
                   /* free the memory allocated at the request time */
                   qvFree(relayUeCnxtMngmntForTunnelId_g[index]);
                   /* Resetting the pointer to NULL */
                   relayUeCnxtMngmntForTunnelId_g[index] = PNULL;
               }
               else
                   /* Resetting the pointer to NULL */
                   relayUeCnxtMngmntForTunnelId_g[index] = PNULL; 
               return RELAY_GTPU_SUCCESS;
            }
            return RELAY_GTPU_FAILURE;
       } 
    }
    return RELAY_GTPU_SUCCESS; 
}


/****************************************************************************
 * Function Name  : processGtpuReleaseSapCnf
 * Inputs         : releaseSapCnf_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process SAP Release Confirmation.
 ****************************************************************************/

UInt32 processGtpuReleaseSapCnf
(
    UInt8 *releaseSapCnf_p
)
{
    printf("Handling processGtpuReleaseSapCnf\n");
    UInt16 index = CSPL_HDR, startVal = 0, ueIndex = 0;
    UInt8 cause = 0, lcId = 0;
    UInt32 transId = 0, tunnelId = 0, tunnelIdIndex = 0;
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
                /* checks whether transId index of relayMapBasedOnTransId_g 
                   is inuse or not*/
                if (RELAY_GTPU_IN_USE ==
                   relayMapBasedOnTransId_g[transId].inUseFlag)
                {
                    /* from Mapping Array finds tunnel id from transaction identifier */
                    tunnelId = relayMapBasedOnTransId_g[transId].tunnelId;
                    relayMapBasedOnTransId_g[transId].inUseFlag = RELAY_GTPU_NOT_IN_USE;
                    tunnelIdIndex = GET_TUNNEL_ID(tunnelId);
                    /* loop till number of lc ids */
                    for (startVal = 0; 
                        startVal < relayUeCnxtMngmntForTunnelId_g[tunnelIdIndex]->lcCount;
                            startVal++)
                    {
                        /* Check whether both tunnel ids are same or not */
                        if (relayUeCnxtMngmntForTunnelId_g[tunnelIdIndex]->ueContext[startVal].tunnelId
                            == tunnelId)
                        {
                            /* Decrementing the isFinished variable by 1 */
                            relayUeCnxtMngmntForTunnelId_g[tunnelIdIndex]->isFinished -=1;
                            relayUeCnxtMngmntForTunnelId_g[tunnelIdIndex]->ueContext[startVal].cause
                                = cause;
                             /* Deleting the mapping maintained at Packet relay. 
                                for that first it check whether this index is in
                                use or not */
                             if (RELAY_GTPU_IN_USE ==
                                 relayMapBasedOnTunnelId_g[tunnelIdIndex].inUseFlag)
                             {
                                 ueIndex = relayMapBasedOnTunnelId_g[tunnelIdIndex].ueIndex;
                                 lcId = relayMapBasedOnTunnelId_g[tunnelIdIndex].lcId;
                                 /* Deleting the mapping based on ueIndex and LcId. For that
                                    First it checks whether these indexs are in use or not */
                                 if (RELAY_GTPU_IN_USE
                                     == relayMapBasedOnUeIndexLcId_g[ueIndex][lcId].inUseFlag)
                                 {
                                    /* Deleting the mapping now */
                                     relayMapBasedOnUeIndexLcId_g[ueIndex][lcId].inUseFlag
                                         = RELAY_GTPU_NOT_IN_USE;
                                     relayMapBasedOnUeIndexLcId_g[ueIndex][lcId].tunnelId = 0;
                                 }
                                 /* Deleting the mapping based on tunnel Identifier */
                                 relayMapBasedOnTunnelId_g[tunnelIdIndex].ueIndex = 0;
                                 relayMapBasedOnTunnelId_g[tunnelIdIndex].lcId = 0;
                                 relayMapBasedOnTunnelId_g[tunnelIdIndex].inUseFlag
                                     = RELAY_GTPU_NOT_IN_USE;
                            }
                            break;
                        } 
                    }
                    /* Check whether all response is received or not */
                    if (RELAY_RECV_ALL_RSP 
                         == relayUeCnxtMngmntForTunnelId_g[tunnelIdIndex]->isFinished)
                    {
                        /* Call function pointer to send the response to CM */
                        if (RELAY_GTPU_SUCCESS !=
                            sendConfirmationRsp[RELAY_GTPU_CP_RELEASE_SAP_CNF]
                               ((UInt8 *)relayUeCnxtMngmntForTunnelId_g[tunnelIdIndex],RELAY_GTPU_TRANS_ID_NOT_REQD,RELAY_GTPU_LEN_NOT_REQD)) 
                        {
                            printf("Not able to send response to S1-AP Module");
                            return RELAY_GTPU_FAILURE;
                        }
                        /* Free the memory allocated at the request time */
                        qvFree(relayUeCnxtMngmntForTunnelId_g[tunnelIdIndex]);
                        /* Resetting the pointer to NULL so that tunnelId Index 
                           can be reuse of array */
                        relayUeCnxtMngmntForTunnelId_g[tunnelIdIndex] = PNULL;
                    }
                    else
                        /* Resetting the pointer to NULL so that tunnelId Index 
                           can be reuse of array */
                        relayUeCnxtMngmntForTunnelId_g[tunnelIdIndex] = PNULL;
                } 
            } 
        }
        else
        {
            tunnelId = RELAY_GTPU_GET_U32BIT(&releaseSapCnf_p[index]);
            tunnelIdIndex = GET_TUNNEL_ID(tunnelId);
            index +=4;
            /* loop till no of lc ids */
            for (startVal = 0;
                 startVal < relayUeCnxtMngmntForTunnelId_g[tunnelIdIndex]->lcCount;
                     startVal++)
            {
                /* check whether both tunnel Id is same or not */
                if (relayUeCnxtMngmntForTunnelId_g[tunnelIdIndex]->ueContext[startVal].tunnelId
                    == tunnelId)
                {
                    relayUeCnxtMngmntForTunnelId_g[tunnelIdIndex]->isFinished -= 1; 
                    relayUeCnxtMngmntForTunnelId_g[tunnelIdIndex]->ueContext[startVal].cause
                        = cause;
                    break;
                }
            }
            /* Check whether all responses are received or not */
            if (RELAY_RECV_ALL_RSP
                == relayUeCnxtMngmntForTunnelId_g[tunnelIdIndex]->isFinished)
            {
                /* Call function pointer to send the response to CM */
                if (RELAY_GTPU_SUCCESS != 
                    sendConfirmationRsp[RELAY_GTPU_CP_RELEASE_SAP_CNF]
                       ((UInt8 *)relayUeCnxtMngmntForTunnelId_g[tunnelIdIndex],RELAY_GTPU_TRANS_ID_NOT_REQD,RELAY_GTPU_LEN_NOT_REQD))
                {
                    printf("Not able to send response to S1-AP Module");
                    return RELAY_GTPU_FAILURE;
                }
                /* Free the memory allocated at the request time */
                qvFree(relayUeCnxtMngmntForTunnelId_g[tunnelIdIndex]);
                /* Reset the pointer to NULL  so that tunnelId index can be
                    reuse */
                relayUeCnxtMngmntForTunnelId_g[tunnelIdIndex] = PNULL;
            }
            else
                /* Reset the pointer to NULL  so that tunnelId index can be
                    reuse */
                relayUeCnxtMngmntForTunnelId_g[tunnelIdIndex] = PNULL;
        }
        return RELAY_GTPU_SUCCESS;
    }
    return RELAY_GTPU_FAILURE;
}


/****************************************************************************
 * Function Name  : processGtpuCpErrorInd
 * Inputs         : cpErrorInd_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process error indication.
 ****************************************************************************/
UInt32 processGtpuCpErrorInd
(
    UInt8 *cpErrorInd_p
)
{
    printf("Handling processGtpuCpErrorInd\n");
    UInt16 index = CSPL_HDR;
    UInt32 transId = 0, tunnelId = 0;
    if (RELAY_GTPU_IE_TRANSACTION_ID == cpErrorInd_p[index])
    {
        transId =  RELAY_GTPU_GET_U32BIT(&cpErrorInd_p[index]); 
        index +=4;
        /* From TransId, Tunnel Id is extracted. but before this first 
           Check whether this structure is inuse or not */
        if (RELAY_GTPU_IN_USE  
            == relayMapBasedOnTransId_g[transId].inUseFlag)
        {
            tunnelId = relayMapBasedOnTransId_g[transId].tunnelId;
            /* Call function handler to send the response to S1-AP */
            if (RELAY_GTPU_SUCCESS !=
                sendConfirmationRsp[RELAY_GTPU_CP_ERROR_IND]
                    ((UInt8 *)&tunnelId,RELAY_GTPU_TRANS_ID_RESERVED,RELAY_GTPU_LEN_NOT_REQD))
            {
                 printf("Not able to send to S1-AP Modulen");
                 return RELAY_GTPU_FAILURE;
            } 
            return RELAY_GTPU_SUCCESS;
        }
    }
    return RELAY_GTPU_FAILURE; 
}

/****************************************************************************
 * Function Name  : processGtpuCpPathFailureInd
 * Inputs         : cpPathFailureInd_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE.
 * Description    : This funtion process CP Path Failure Indication.
 ****************************************************************************/

UInt32 processGtpuCpPathFailureInd
(
    UInt8 *cpPathFailureInd_p
)
{
    printf("Handling processGtpuCpPathFailureInd\n");
    UInt8 cpPathFailureMsg[MAX_API_LEN + API_HDR] = {0};
    UInt16 current = API_HDR,startVal = 0; 
    UInt16 length = 0;
    if (RELAY_GTPU_IE_GSN_ADDR == cpPathFailureInd_p[current])
    {
        cpPathFailureMsg[current] = cpPathFailureInd_p[current];
        current +=1;
        length =  RELAY_GTPU_GET_U16BIT(&cpPathFailureInd_p[current]);
        RELAY_GTPU_SET_U16BIT(&cpPathFailureMsg[current],length);
        current +=2;
        for (startVal = 0; startVal < length - RELAY_TAG_LEN ; startVal++,current++)
            cpPathFailureMsg[current] = cpPathFailureInd_p[current];
        /* Call function pointer to send the response to S1-AP Module */
        if (RELAY_GTPU_SUCCESS !=
            sendConfirmationRsp[RELAY_GTPU_CP_PATH_FAILURE_IND]
               ((UInt8*)&cpPathFailureMsg,RELAY_GTPU_TRANS_ID_RESERVED,current))
        {
             printf("Not able to send response to CP\n");
             return RELAY_GTPU_FAILURE;
        }
        return RELAY_GTPU_SUCCESS;
    }  
    return RELAY_GTPU_FAILURE;
}
#endif
