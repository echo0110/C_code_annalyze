/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2015 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: ltePdcpUserSapCommands.c
 *
 ******************************************************************************
 *
 *  File Description : Packet Relay API message handlers implementation
 *
 *****************************************************************************/
#if  defined(PDCP_GTPU_INTF)

/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteLayer2PdcpGtpuInterface.h"
#include "ltePdcpExtScheduler.h"
#include "ltePdcpCmnLteFramework.h"
#include "ltePdcpPdcpUserSap.h"
#include "ltePdcpCmnTools.h"
#include "ltePdcpErrors.h"
#include "logging.h"
#if defined PDCP_GTPU_INTF
#include "ltePdcpSockTools.h"
#endif
 
/******************************************************************************
  Private Definitions
 *****************************************************************************/
extern tickType_t pdcpCurrentTime_g;
/*SPR 21366 +*/
/*SPR 21545 Start*/
extern UInt8 instanceL2_g;
/*SPR 21545 End*/
void sendGtpuRegisterPdcpInd(RrcCellIndex *cellIdList_p, UInt8 cellCount);
extern void SendToGtpu(UInt8 *buffer, UInt32 bufferLen, UInt16 moduleId);
extern UInt16 pdcpGtpudModulePort;
/*SPR 21366 -*/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* Packet Relay API command handlers definitions */
static void prCmdUeDataReq( UInt8* data_p, UInt16 size );
static void prCmdDataFwdReq( UInt8* data_p, UInt16 size );
static void prCmdIntraDataFwdReq( UInt8* data_p, UInt16 size );
static void prCmdTargetDlDataRelocReq( UInt8* data_p, UInt16 size );
static void prCmdTargetUlDataRelocReq( UInt8* data_p, UInt16 size );
static void prCmdEndMarkerInd( UInt8* data_p, UInt16 size );
static void pdcpProcessPdcpDataInd( UInt16 ueIndex, UInt16 lcId, void* data_p, 
        UInt16 size, UInt32 count );

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* Packet Relay API command handlers table */
TlvCommandTable prCmdTable_g[] =
{
    { { PDCP_PR_DATA_REQ                   }, prCmdUeDataReq,           },
    { { PDCP_PR_SOURCE_DATA_FWD_REQ        }, prCmdDataFwdReq,          },
    { { PDCP_PR_INTRA_SOURCE_DATA_FWD_REQ  }, prCmdIntraDataFwdReq      },
    { { PDCP_PR_TARGET_DL_DATA_RELOC_REQ   }, prCmdTargetDlDataRelocReq },
    { { PDCP_PR_TARGET_UL_DATA_RELOC_REQ   }, prCmdTargetUlDataRelocReq },
    { { PDCP_PR_END_MARKER_IND             }, prCmdEndMarkerInd         },
    { { TLV_LAST_ID                        }, PNULL,                    },
};

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/****************************************************************************
 * Functions implementation
 ****************************************************************************/


/****************************************************************************
 * Function Name  : prCmdUeDataReq
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes PDCP_PR_DATA_REQ API which shall take the
 *                  user plane data from packet relay.
 *****************************************************************************/
void prCmdUeDataReq( UInt8* data_p, UInt16 size )
{
    UInt16           errCode      = PDCP_SUCCESS;
    UInt8           *dataZcb_p    = PNULL;
    UInt8           *dataPl_p     = PNULL;
    LP_TlvMessage    message_p    = (LP_TlvMessage)data_p;
    LP_PdcpUeData    pdcpUeData_p = PNULL; 
#ifdef L2_FAST_PATH
    LP_PdcpUeContext ueCtx_p = PNULL;
    LP_PdcpEnityContext ctx_p = PNULL;
#endif
    UInt8 *freeBuff_p = data_p;
    /*Check API id */
    CHECK_EXP( PDCP_PR_DATA_REQ != message_p->header.apiId );
    
    if ( message_p->header.length != size )
    {
        /* invalid length -- reject it */
        sendFreetoGTP( 0, (void *)freeBuff_p ); 
        return;
    }
    /*Prepare pointer to message data */
    pdcpUeData_p = (LP_PdcpUeData)message_p->data;


    do
    {
        /* Get the pointer to actual data buffer from the TLV recvd */
        dataPl_p = (UInt8 *)((UInt8*)pdcpUeData_p + sizeof( PdcpUeData ));

        /*SPR 21188 +*/
        dataZcb_p = msgAlloc(PNULL, 0, 0, 0);
        if ( PNULL == dataZcb_p ) 
        {
            LOG_PDCP_MSG( PDCP_MSG_ALLOC_FAILED, LOGFATAL, PDCP_TX, 
                    pdcpCurrentTime_g, __LINE__,0,0,0, 
                    0, 0, 0, __func__,"Allocate for DL ZCB");
            sendFreetoGTP( 0, (void *)freeBuff_p ); 
            break;                                 
        }
        /* Add the memory segment from TLV buffer to ZCB segment buffer */
        if (0 == qvMsgInsertExternal(dataZcb_p, 0,
                    ( dataPl_p - PDCP_MAX_HDR_SIZE ),
                    ( pdcpUeData_p->size + PDCP_MAX_HDR_SIZE ),
                    sendFreetoGTP, (void *)freeBuff_p ) )
        {
            LOG_PDCP_MSG(PR_DL_DATA_MSG_INSERT_FAIL, LOGFATAL, PR_GTPU_DL_DATA,
                    pdcpCurrentTime_g, __LINE__, pdcpUeData_p->size, 0,0,0,
                    0,0, __func__, "ADAPTER" );
            sendFreetoGTP( 0, (void *)freeBuff_p ); 
            msgFree( dataZcb_p );
            dataZcb_p = PNULL;
            break;                                 
        }

        /* Following bytes are removed but they will actual live as HEADROOM
         * and can be used for PDCP HEADER */
        if( PDCP_MAX_HDR_SIZE != msgRemove( dataZcb_p, 0, PDCP_MAX_HDR_SIZE ) )
        {
            errCode = PDCP_FAIL;
            break;
        }
        /*SPR 21188 -*/
        /* + SPR 19169 */
        /* Communication needs to be done for RRC UEIndex with Packet Relay */
        UInt16 ueIndex = getUeIdxFromRrcUeIdx( pdcpUeData_p->ueIndex );

#ifdef L2_FAST_PATH
        ueCtx_p = PDCP_GET_UE_CONTEXT( ueIndex );
        /* - SPR 19169 */
        if( PNULL == ueCtx_p )
        {
            errCode = PDCP_FAIL;
            break;
        }
        /* Check -
         * 1. The PDCP State for DRB is PDCP_TX_STATE_CONNECTED
         * 2. DL-Forwarding Queue contents
         */
        if((PDCP_TX_STATE_CONNECTED == ueCtx_p->pdcpTxSrb2DrbState) && 
                (0 == QCOUNT_PDCP_DATA_FWDING_DL_Q(ueCtx_p)))
        {
            /* + SPR 19169 */
            ctx_p = pdcpGetEntityContext( ueIndex, pdcpUeData_p->lcId, 
                    PDCP_ENTITY_RB_TX );
            /* - SPR 19169 */
            if( PNULL == ctx_p )
            {
                errCode = PDCP_FAIL;
                break;
            }
            if (!QCOUNT_PDCP_TX_DATA_REQ_Q( ctx_p ))
            {
                /* + SPR 19169 */
                pdcpDataFpReq( ueIndex, pdcpUeData_p->lcId, ctx_p, 0xFF,
                        dataZcb_p, pdcpUeData_p->size );
            }
            else
            {
                pdcpDataReq( ueIndex, pdcpUeData_p->lcId, 0xFF, dataZcb_p,
                        pdcpUeData_p->size ,0);
            }
        }
        else
#endif
        {
            pdcpDataReq( ueIndex, pdcpUeData_p->lcId, 0xFF, dataZcb_p, 
                    pdcpUeData_p->size ,0);
            /* - SPR 19169 */
        }
    }while( 0 );

    if( PDCP_FAIL == errCode )
    {
        msgFree( dataZcb_p );
        dataZcb_p = PNULL;
    }
}

/****************************************************************************
 * Function Name  : prCmdDataFwdReq
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes PDCP_PR_SOURCE_DATA_FWD_REQ API which shall
 *                  call the forwarding API at PDCP.
 *****************************************************************************/
void prCmdDataFwdReq( UInt8* data_p, UInt16 size )
{
    LP_TlvMessage    message_p       = (LP_TlvMessage)data_p;
    LP_PdcpDataForwardReq pdcpDataFwd_p = PNULL; 
    UInt8 *freeBuff_p = data_p;

    /*Check API id */
    CHECK_EXP( PDCP_PR_SOURCE_DATA_FWD_REQ != message_p->header.apiId );
    
    if ( message_p->header.length != size )
    {
        /* invalid length -- reject it */
        sendFreetoGTP( 0, (void *)freeBuff_p ); 
        return;
    }
    /*Prepare pointer to message data */
    pdcpDataFwd_p = (LP_PdcpDataForwardReq)message_p->data;
    /* + SPR 19169 */
    /* Communication needs to be done for RRC UEIndex with Packet Relay */
    pdcpDataFwd_p->ueIndex = getUeIdxFromRrcUeIdx( pdcpDataFwd_p->ueIndex );
    /* - SPR 19169 */

    /* Call PDCP API to start the forwarding of data as per specifications */
    pdcpSourceDataFwdReq( pdcpDataFwd_p, size - sizeof(TlvHeader),0 );

    /* Send Free indication to Packet Relay after processing the message */
    sendFreetoGTP( 0, (void *)freeBuff_p ); 
}

/****************************************************************************
 * Function Name  : prCmdIntraDataFwdReq
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes PDCP_PR_INTRA_SOURCE_DATA_FWD_REQ API which
 *                  shall call the forwarding API at PDCP in case of Intra HO.
 *****************************************************************************/
void prCmdIntraDataFwdReq( UInt8* data_p, UInt16 size )
{
    LP_TlvMessage  message_p = (LP_TlvMessage)data_p;
    LP_PdcpIntraDataFwd pdcpIntraDataFwd_p = PNULL; 
    UInt8 *freeBuff_p = data_p;

    /*Check API id */
    CHECK_EXP( PDCP_PR_INTRA_SOURCE_DATA_FWD_REQ != message_p->header.apiId );
    
    if ( message_p->header.length != size )
    {
        /* invalid length -- reject it */
        sendFreetoGTP( 0, (void *)freeBuff_p ); 
        return;
    }
    /*Prepare pointer to message data */
    pdcpIntraDataFwd_p = (LP_PdcpIntraDataFwd)message_p->data;

    /* + SPR 19169 */
    /* Communication needs to be done for RRC UEIndex with Packet Relay */
    UInt16 oldUeIndex = getUeIdxFromRrcUeIdx( pdcpIntraDataFwd_p->oldUeIndex );
    UInt16 newUeIndex = getUeIdxFromRrcUeIdx( pdcpIntraDataFwd_p->newUeIndex );

    /* Call PDCP API to start the forwarding of data as per specifications 
     * in Intra eNB HO */
    {
    pdcpSourceDataFwdReqInIntraeNB( oldUeIndex, newUeIndex,0);
    /* - SPR 19169 */
    }
    
    /* Send Free indication to Packet Relay after processing the message */
    sendFreetoGTP( 0, (void *)freeBuff_p ); 
}

/****************************************************************************
 * Function Name  : prCmdTargetDlDataRelocReq
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes PDCP_PR_TARGET_DL_DATA_RELOC_REQ API which shall
 *                  take the user plane forwarded DL data from packet relay.
 *****************************************************************************/
void prCmdTargetDlDataRelocReq( UInt8* data_p, UInt16 size )
{
    LP_TlvMessage    message_p       = (LP_TlvMessage)data_p;
    LP_PdcpDataReloc pdcpTargetDlDataReloc_p = PNULL;
    UInt8 *freeBuff_p = data_p;

    /*Check API id */
    CHECK_EXP( PDCP_PR_TARGET_DL_DATA_RELOC_REQ != message_p->header.apiId );
    
    if ( message_p->header.length != size )
    { 
        /* invalid length -- reject it */
        sendFreetoGTP( 0, (void *)freeBuff_p ); 
        return;
    }
    /*Prepare pointer to message data */
    pdcpTargetDlDataReloc_p = (LP_PdcpDataReloc)message_p->data;

    do
    {
        /* Get the pointer to actual data buffer from the TLV recvd */
        UInt8 *dataPl_p = (UInt8 *)pdcpTargetDlDataReloc_p + 
            sizeof( PdcpDataReloc );
        UInt8 *dataZcb_p = msgAlloc(PNULL, 0, 0, 0);
        if ( PNULL == dataZcb_p ) 
        {
            LOG_PDCP_MSG( PDCP_MSG_ALLOC_FAILED, LOGFATAL, PDCP_TX, 
                    pdcpCurrentTime_g, __LINE__,0,0,0, 
                    0, 0, 0, __func__,"Allocate for DL ZCB");
            sendFreetoGTP( 0, (void *)freeBuff_p); 
            break;                                 
        }
        /* Add the memory segment from TLV buffer to ZCB segment buffer */
        if (0 == qvMsgInsertExternal(dataZcb_p, 0, dataPl_p,
                    pdcpTargetDlDataReloc_p->ueData.size, 
                    sendFreetoGTP, (void *)freeBuff_p ) )
        {
            LOG_PDCP_MSG(PR_DL_DATA_MSG_INSERT_FAIL, LOGFATAL, PR_GTPU_DL_DATA,
                    pdcpCurrentTime_g, __LINE__, pdcpTargetDlDataReloc_p->ueData.size, 0,0,0,
                    0,0, __func__, "ADAPTER" );
            sendFreetoGTP( 0, (void *)freeBuff_p ); 
            msgFree( dataZcb_p );
            dataZcb_p = PNULL;
            break;                                 
        }

        /* + SPR 19169 */
        /* Communication needs to be done for RRC UEIndex with Packet Relay */
        UInt16 ueIndex = getUeIdxFromRrcUeIdx( 
                pdcpTargetDlDataReloc_p->ueData.ueIndex );

        pdcpTargetDlDataRelocReq( ueIndex, 
                pdcpTargetDlDataReloc_p->ueData.lcId,
                pdcpTargetDlDataReloc_p->sn, dataZcb_p,
                pdcpTargetDlDataReloc_p->ueData.size ,0);
        /* - SPR 19169 */
    }while( 0 );
}

/****************************************************************************
 * Function Name  : prCmdTargetUlDataRelocReq
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes PDCP_PR_TARGET_UL_DATA_RELOC_REQ API which shall
 *                  take the user plane forwarded DL data from packet relay.
 *****************************************************************************/
void prCmdTargetUlDataRelocReq( UInt8* data_p, UInt16 size )
{
    LP_TlvMessage    message_p       = (LP_TlvMessage)data_p;
    LP_PdcpDataReloc pdcpTargetUlDataReloc_p = PNULL;
    UInt8 *freeBuff_p = data_p;


    /*Check API id */
    CHECK_EXP( PDCP_PR_TARGET_UL_DATA_RELOC_REQ != message_p->header.apiId );
    
    if ( message_p->header.length != size )
    {
        /* invalid length -- reject it */
        sendFreetoGTP( 0, (void *)freeBuff_p); 
        return;
    }
    /*Prepare pointer to message data */
    pdcpTargetUlDataReloc_p = (LP_PdcpDataReloc)message_p->data;

    do
    {
        /* Get the pointer to actual data buffer from the TLV recvd */
        UInt8 *dataPl_p = (UInt8 *)pdcpTargetUlDataReloc_p + 
            sizeof( PdcpDataReloc );

        UInt8 *dataZcb_p = msgAlloc(PNULL, 0, 0, 0);
        if ( PNULL == dataZcb_p ) 
        {
            LOG_PDCP_MSG( PDCP_MSG_ALLOC_FAILED, LOGFATAL, PDCP_TX, 
                    pdcpCurrentTime_g, __LINE__,0,0,0, 
                    0, 0, 0, __func__,"Allocate for DL ZCB");
            sendFreetoGTP( 0, (void *)freeBuff_p ); 
            break;                                 
        }
        /* Add the memory segment received from GTP to ZCB segment 
         * buffer */
        if (0 == qvMsgInsertExternal( dataZcb_p, 0, dataPl_p,
                    pdcpTargetUlDataReloc_p->ueData.size, 
                    sendFreetoGTP, (void *)freeBuff_p ) )
        {
            LOG_PDCP_MSG(PR_DL_DATA_MSG_INSERT_FAIL, LOGFATAL, PR_GTPU_DL_DATA,
                    pdcpCurrentTime_g, __LINE__, 
                    pdcpTargetUlDataReloc_p->ueData.size, 0,0,0,
                    0,0, __func__, "ADAPTER" );
            sendFreetoGTP( 0, (void *)freeBuff_p ); 
            msgFree( dataZcb_p );
            dataZcb_p = PNULL;
            break;                                 
        }
        /*SPR 21188 -*/
        /* + SPR 19169 */
        /* Communication needs to be done for RRC UEIndex with Packet Relay */
        UInt16 ueIndex = getUeIdxFromRrcUeIdx( 
                pdcpTargetUlDataReloc_p->ueData.ueIndex );

        pdcpTargetUlDataRelocReq( ueIndex,
                pdcpTargetUlDataReloc_p->ueData.lcId,
                pdcpTargetUlDataReloc_p->sn, dataZcb_p,
                pdcpTargetUlDataReloc_p->ueData.size,0 );
        /* - SPR 19169 */
    }while( 0 );
}

/****************************************************************************
 * Function Name  : prCmdEndMarkerInd
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes PDCP_PR_END_MARKER_IND API which shall
 *                  indicate PDCP that EMI has been received.
 *****************************************************************************/
void prCmdEndMarkerInd( UInt8* data_p, UInt16 size )
{
    LP_TlvMessage    message_p    = (LP_TlvMessage)data_p;
    LP_PdcpEmi pdcpEndMarker_p = PNULL; 
    UInt8 *freeBuff_p = data_p;

    /*Check API id */
    CHECK_EXP( PDCP_PR_END_MARKER_IND != message_p->header.apiId );
    
    if ( message_p->header.length != size )
    {
        /* invalid length -- reject it */
        sendFreetoGTP( 0, (void *)freeBuff_p ); 
        return;
    }
    /*Prepare pointer to message data */
    pdcpEndMarker_p = (LP_PdcpEmi)message_p->data;

    /* + SPR 19169 */
    /* Communication needs to be done for RRC UEIndex with Packet Relay */
    UInt16 ueIndex = getUeIdxFromRrcUeIdx( pdcpEndMarker_p->ueIndex ); 

    pdcpEndMarkerInd( ueIndex, pdcpEndMarker_p->lcId, 0 );
    /* - SPR 19169 */
    
    /* Send Free indication to Packet Relay after processing the message */
    sendFreetoGTP( 0, (void *)freeBuff_p ); 
}

/****************************************************************************
 * Function Name  : pdcpPrProcessMessage
 * Inputs         : data_p  - pointer to TLV API buffer.
 *                   size  -  TLV API buffer size.
 * Outputs        : None.
 * Returns        : None.
 * Description    : This API will be used by the PDCP layer application for
 *                  delivering Packet Relay APIs to the PDCP layer. The APIs
 *                  message will be processed immediately.
 *****************************************************************************/
void pdcpPrProcessMessage( UInt8 * data_p, UInt16 size )
{
    /* process message */
    tlvProcessCommand( data_p, size );
}

/****************************************************************************
 * Function Name  : prCmdDefaultHandler
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes unknown Packet Relay API.
 ****************************************************************************/
void prCmdDefaultHandler( UInt8* data_p, UInt16 size )
{
    /* Unknown API received, Log Message */
    
    /* Send Free Indication to Packet Relay */
    sendFreetoGTP( 0, (void *)data_p ); 
}

/****************************************************************************
 * Function Name  : pdcpFirstSubFrameInd
 * Inputs         : sysFrameNum, subFrameNum 
 * Outputs        : None
 * Returns        : None
 * Description    : This function sends the first tick received from L1 to 
 *                  packet relay.
 ****************************************************************************/
void  pdcpFirstSubFrameInd( UInt32 sysFrameNum, UInt32 subFrameNum )
{
    /* No need of sending First SFI to Relay in case of PDCP-GTP split
     * as the Relay will not do anything with tick. Timers at GTPu shall be
     * maintained using clock_gettime function */
}

/****************************************************************************
 * Function Name  : pdcpDataInd
 * Inputs         : ueIndex - UE Id,
 *                  lcId - Logical channel Id,
 *                  data_p - pointer to PDCP SDU data in Zero-copy
 *                           buffering format,
 *                  size - size of data
 * Outputs        : None
 * Returns        : None
 * Description    : The PDCP layer application has to implement this function.
 *                  It enqueues the packet to process later.
 ****************************************************************************/
void  pdcpDataInd( UInt16 ueIndex, UInt16 lcId, void* data_p, UInt16 size)
{
    pdcpProcessPdcpDataInd( ueIndex, lcId, data_p, size, 0 );
}

/****************************************************************************
 * Function Name  : pdcpProcessPdcpDataInd
 * Inputs         : ueIndex - UE Id,
 *                  lcId - Logical channel Id,
 *                  data_p - pointer to PDCP SDU data in Zero-copy
 *                           buffering format,
 *                  size - size of data
 *                  count - Number of nodes is queue
 * Outputs        : None
 * Returns        : None
 * Description    : The PDCP layer application has to implement this function.
 *                  It's used for passing SDU form the PDCP layer to the PDCP
 *                  user (upper layers).
 ****************************************************************************/
/*SPR 21188 +*/
void pdcpProcessPdcpDataInd( UInt16 ueIndex, UInt16 lcId, void* data_p, 
        UInt16 size, UInt32 count )
{
    LP_TlvMessage    message_p      = PNULL;
    LP_PdcpUeData    pdcpUeData_p = PNULL; 
    UInt8 *tempData_p  = PNULL;
    UInt8 *msgBuffer_p = PNULL;
    UInt16 totSize     = sizeof(TlvMessage) + sizeof(PdcpUeData);
    totSize += size;
    /* - SPR 19525 */
#ifdef KPI_STATS
    /* SPR 20831 Changes Start */
    UInt32 activeIndexForPdcpThpStats = gMacStats.
        pStats->lteCommonUeKPIStats.activeIndexForPdcpThpStats;
    /* SPR 20831 Changes End*/
    LtePdcpKPIThpStats *pLtePdcpKPIThpStats = &(gMacStats.pStats->lteCommonUeKPIStats.
            ltePdcpKPIThpStats[activeIndexForPdcpThpStats]);
    /* + SPR 17415 */
    /* Check if UL forwarded packets present at target eNB */
    if( 0 != pLtePdcpKPIThpStats->uLFwdedPdcpSdu[ueIndex][lcId - 
            LTE_DRB_LC_ID_LOW_BOUND] )
    {
        if( size <= pLtePdcpKPIThpStats->uLFwdedPdcpSdu[ueIndex]
                [lcId - LTE_DRB_LC_ID_LOW_BOUND] )
        {
            /* Decrement size from uLFwdedPdcpSdu as it is not to be 
             * considered for UlSduBitRate */
            pLtePdcpKPIThpStats->uLFwdedPdcpSdu[ueIndex][lcId - 
                LTE_DRB_LC_ID_LOW_BOUND] -= size;
        }
        else
        {
            UInt32 tempSize = 
                size - pLtePdcpKPIThpStats->uLFwdedPdcpSdu[ueIndex]
                [lcId - LTE_DRB_LC_ID_LOW_BOUND];
            /* Reset uLFwdedPdcpSdu */
            pLtePdcpKPIThpStats->uLFwdedPdcpSdu[ueIndex][lcId - 
                LTE_DRB_LC_ID_LOW_BOUND] = 0;
            /* Consider the remaining size from uLFwdedPdcpSdu for
             * UlSduBitRate calculation */
            pLtePdcpKPIThpStats->SharedMemoryULPDCPSDU[ueIndex]
                [lcId - LTE_DRB_LC_ID_LOW_BOUND] += tempSize;
        }
    }
    else
    {
        /* No UL fwded packet, hence, consider this size for UlSduBitRate */
        pLtePdcpKPIThpStats->SharedMemoryULPDCPSDU[ueIndex][lcId - 
            LTE_DRB_LC_ID_LOW_BOUND] += size;
    }
    /* - SPR 17415 */
#endif

    /* Memory Allocation */
    msgBuffer_p = (UInt8 *)getMemFromPool( totSize, PNULL );

    if(!msgBuffer_p)
    {
        msgFree( data_p );
        LOG_PDCP_ERROR( PDCP_RX, "Cannot Allocate Memory ");
        return;
    }
    message_p = (LP_TlvMessage)&msgBuffer_p[0];

    tempData_p = (UInt8 *)(msgBuffer_p + sizeof(TlvMessage) + 
            sizeof(PdcpUeData) ) ;

    if( 0 == msgExtract( data_p, 0, tempData_p, size ) )
    {
        LOG_PDCP_ERROR( PDCP_RX, "Unable to extract data, UE[%d] LC[%d]", 
                ueIndex, lcId ); 
        msgFree( data_p );
        freeMemPool( msgBuffer_p );
        return;
    }
    /* Prepare pointer to response message data */
    pdcpUeData_p = (LP_PdcpUeData)message_p->data;

    /* + SPR 19169 */
    /* Communication needs to be done for RRC UEIndex with Packet Relay */
    pdcpUeData_p->ueIndex = pdcpContext.ueContexts[ueIndex]->rrcUeIndex;
    /* - SPR 19169 */
    pdcpUeData_p->lcId = lcId;
    pdcpUeData_p->size = size;

    /* Prepare dataBuffer in UL for packet relay  */
    /* Do not care for the cellID as Relay does not require for this API */
    tlvPrepareMessage( message_p, 0xFF, PACKET_RELAY_MODULE_ID,
            PDCP_PR_UE_DATA_IND, sizeof(PdcpUeData), API_HEADER_CELL_ID_NIL );

    message_p->header.length += size;

    /*Convert response fields to network byte order */
    tlvPrepareHeaderForSending(message_p);
    mbSendDataPktsToPr( msgBuffer_p, totSize );
    msgFree( data_p );
}
/*SPR 21188 +*/
/****************************************************************************
 * Function Name  : pdcpEndOfDataForwardingInd
 * Inputs         : ueIndex - UE Id,
 *                  lcId    - Logical channel Id,
 *                  isEndMarkerIndRecvd - Flag for EMI Received
 * Outputs        : None
 * Returns        : None
 * Description    : The PDCP layer indicates end of forwarding SDUs in HO
 *                  Scenario.
 ****************************************************************************/
void pdcpEndOfDataForwardingInd(UInt16 ueIndex, UInt8 lcId, 
        UInt8 isEndMarkerIndRecvd )
{
    LP_TlvMessage    message_p      = PNULL;
    LP_PdcpLocalEmi pdcpLocalEmi_p = PNULL;
    UInt8 *msgBuffer_p = PNULL;
    UInt16 totSize     = sizeof(TlvMessage) + sizeof(PdcpLocalEmi);

    /* Memory Allocation */
    /*SPR 21188 +*/
    msgBuffer_p = (UInt8 *)getMemFromPool(totSize , PNULL);
    /*SPR 21188 -*/

    if(!msgBuffer_p)
    {
        LOG_PDCP_ERROR( PDCP_RX, "Cannot Allocate Memory ");
        return;
    }
    message_p = (LP_TlvMessage)&msgBuffer_p[0];

    /* Prepare pointer to response message data */
    pdcpLocalEmi_p = (LP_PdcpLocalEmi)message_p->data;

    /* + SPR 19169 */
    /* Communication needs to be done for RRC UEIndex with Packet Relay */
    pdcpLocalEmi_p->emi.ueIndex = pdcpContext.ueContexts[ueIndex]->rrcUeIndex;
    /* - SPR 19169 */
    pdcpLocalEmi_p->emi.lcId = lcId;
    pdcpLocalEmi_p->isEmiRecvd = isEndMarkerIndRecvd;

    /* Prepare message for local EMI */
    /* Do not care for the cellID as Relay does not require for this API */
    tlvPrepareMessage( message_p, 0xFF, PACKET_RELAY_MODULE_ID,
            PDCP_PR_LOCAL_EMI, sizeof(PdcpLocalEmi), API_HEADER_CELL_ID_NIL );

    /*Convert response fields to network byte order */
    tlvPrepareHeaderForSending(message_p);

    /* Send the data over mailbox */
    mbSendDataPktsToPr( msgBuffer_p, totSize );
}

/****************************************************************************
 * Function Name  : pdcpSourceDlDataRelocInd
 * Inputs         : ueIndex - UE Id,
 *                  lcId - Logical channel Id,
 *                  sn - Sequence Number,
 *                  data_p - pointer to PDCP SDU data in Zero-copy
 *                           buffering format,
 *                  size - size of payload
 * Outputs        : None
 * Returns        : None
 * Description    : The PDCP layer calls this API to forward Downlink SDUs
 *                  to upper layer at source eNodeB in case of handover.
 ****************************************************************************/
UInt16 pdcpSourceDlDataRelocInd(UInt16 ueIndex, UInt8 lcId, UInt16 sn,
        void *data_p, UInt32 size )
{
    LP_TlvMessage    message_p      = PNULL;
    LP_PdcpDataReloc pdcpDlDataReloc_p = PNULL; 
    UInt8 *msgBuffer_p = PNULL;
    UInt16 totSize     = sizeof(TlvMessage) + sizeof(PdcpDataReloc);
    UInt8 *tempData_p  = PNULL;
    totSize += size;
    /* Memory Allocation */
    /*SPR 21188 +*/
    /* Memory Allocation */
    msgBuffer_p = (UInt8 *)getMemFromPool(totSize , PNULL);
    /*SPR 21188 -*/

    if(!msgBuffer_p)
    {
        msgFree( data_p );
        LOG_PDCP_ERROR( PDCP_RX, "Cannot Allocate Memory ");
        return PDCP_FAIL;
    }
    message_p = (LP_TlvMessage)&msgBuffer_p[0];

    tempData_p = (UInt8 *)(msgBuffer_p + sizeof(TlvMessage) + 
            sizeof(PdcpDataReloc) ) ;
    /*SPR 21188 +*/
    if( 0 == msgExtract( data_p, 0, tempData_p, size ) )
    {
        LOG_PDCP_ERROR( PDCP_RX, "Unable to extract data, UE[%d] LC[%d]", 
                ueIndex, lcId ); 
        freeMemPool( msgBuffer_p );
        return PDCP_FAIL;
    }
    /*SPR 21188 -*/
#ifdef KPI_STATS
    UInt32 actIdxForPdcpThpStats = gMacStats.
        pStats->lteCommonUeKPIStats.activeIndexForPdcpThpStats;
    LtePdcpKPIThpStats *pLtePdcpKPIThpStats = &(gMacStats.pStats->
            lteCommonUeKPIStats.ltePdcpKPIThpStats[actIdxForPdcpThpStats]);

    if(pLtePdcpKPIThpStats->SharedMemoryDLPDCPSDU[ueIndex][lcId - 3] >= size)
    {
        pLtePdcpKPIThpStats->SharedMemoryDLPDCPSDU[ueIndex][lcId - 3] -= size;
    }
    else
    {
        pLtePdcpKPIThpStats->SharedMemoryDLPDCPSDU[ueIndex][lcId - 3] = 0;
    }
#endif

    /* Prepare pointer to response message data */
    pdcpDlDataReloc_p = (LP_PdcpDataReloc)message_p->data;

    /* + SPR 19169 */
    /* Communication needs to be done for RRC UEIndex with Packet Relay */
    pdcpDlDataReloc_p->ueData.ueIndex = 
        pdcpContext.ueContexts[ueIndex]->rrcUeIndex;
    /* - SPR 19169 */
    pdcpDlDataReloc_p->ueData.lcId = lcId;
    pdcpDlDataReloc_p->ueData.size = size;
    pdcpDlDataReloc_p->sn = sn;

    /* Prepare dataBuffer in UL for packet relay  */
    /* Do not care for the cellID as Relay does not require for this API */
    tlvPrepareMessage( message_p, 0xFF, PACKET_RELAY_MODULE_ID,
            PDCP_PR_SOURCE_DL_DATA_RELOC_REQ, sizeof(PdcpDataReloc), 
            API_HEADER_CELL_ID_NIL );

    message_p->header.length += size;

    /*Convert response fields to network byte order */
    tlvPrepareHeaderForSending(message_p);

    /* Send the data over mailbox */
    mbSendDataPktsToPr( msgBuffer_p, totSize );
    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpSourceUlDataRelocInd
 * Inputs         : ueIndex - UE Id,
 *                  lcId - Logical channel Id,
 *                  sn - Sequence Number,
 *                  data_p - pointer to PDCP SDU data in Zero-copy
 *                           buffering format,
 *                  size - size of payload
 * Outputs        : None
 * Returns        : None
 * Description    : The PDCP layer calls this API to forward uplink SDUs
 *                  to upper layer at source eNodeB in case of handover.
 ****************************************************************************/
UInt16 pdcpSourceUlDataRelocInd(UInt16 ueIndex, UInt8 lcId, UInt16 sn,
        void *data_p, UInt32 size
#ifdef KPI_STATS
        , InternalCellIndex internalCellIndex 
#endif
        )
{
    LP_TlvMessage    message_p      = PNULL;
    LP_PdcpDataReloc pdcpUlDataReloc_p = PNULL; 
    UInt8 *msgBuffer_p = PNULL;
    UInt16 totSize     = sizeof(TlvMessage) + sizeof(PdcpDataReloc);
    UInt8 *tempData_p  = PNULL;
    totSize += size;
    /* Memory Allocation */
    /*SPR 21188 +*/
    /* Memory Allocation */
    msgBuffer_p = (UInt8 *)getMemFromPool( totSize , PNULL );
    /*SPR 21188 -*/

    if(!msgBuffer_p)
    {
        msgFree( data_p );
        LOG_PDCP_ERROR( PDCP_RX, "Cannot Allocate Memory ");
        return PDCP_FAIL;
    }
    message_p = (LP_TlvMessage)&msgBuffer_p[0];

    tempData_p = (UInt8 *)(msgBuffer_p + sizeof(TlvMessage) + 
            sizeof(PdcpDataReloc) ) ;
    /*SPR 21188 +*/
    if( 0 == msgExtract( data_p, 0, tempData_p, size ) )
    {
        LOG_PDCP_ERROR( PDCP_RX, "Unable to extract data, UE[%d] LC[%d]", 
                ueIndex, lcId ); 
        freeMemPool( msgBuffer_p );
        return PDCP_FAIL;
    }
    /*SPR 21188 -*/

#ifdef KPI_STATS
    /* SPR 20831 Changes Start */
    UInt32 activeIndexForPdcpThpStats = gMacStats.pStats->
        lteCommonUeKPIStats.activeIndexForPdcpThpStats;
    LtePdcpKPIThpStats *pLtePdcpKPIThpStats = &(gMacStats.pStats->
            lteCommonUeKPIStats.ltePdcpKPIThpStats[
            activeIndexForPdcpThpStats]);
    /* SPR 20831 Changes End */

    /* UlSduBitRate needs to be calculated at source eNB and will not be 
     * incremented at target, hence incrementing the stats here for HO 
     * (Reference: Section 4.4.1.2 of 32.425) */
    pLtePdcpKPIThpStats->SharedMemoryULPDCPSDU[ueIndex][lcId - 
        LTE_DRB_LC_ID_LOW_BOUND] += size;
#endif

    /* Prepare pointer to response message data */
    pdcpUlDataReloc_p = (LP_PdcpDataReloc)message_p->data;

    /* + SPR 19169 */
    /* Communication needs to be done for RRC UEIndex with Packet Relay */
    pdcpUlDataReloc_p->ueData.ueIndex = 
        pdcpContext.ueContexts[ueIndex]->rrcUeIndex;
    /* - SPR 19169 */
    pdcpUlDataReloc_p->ueData.lcId = lcId;
    pdcpUlDataReloc_p->ueData.size = size;
    pdcpUlDataReloc_p->sn = sn;

    /* Prepare dataBuffer in UL for packet relay  */
    /* Do not care for the cellID as Relay does not require for this API */
    tlvPrepareMessage( message_p, 0xFF, PACKET_RELAY_MODULE_ID,
            PDCP_PR_SOURCE_UL_DATA_RELOC_REQ, sizeof(PdcpDataReloc),
            API_HEADER_CELL_ID_NIL );

    message_p->header.length += size;

    /*Convert response fields to network byte order */
    tlvPrepareHeaderForSending(message_p);

    /* Send the data over mailbox */
    mbSendDataPktsToPr( msgBuffer_p, totSize );

    return PDCP_SUCCESS;
}

/****************************************************************************
 *  Function Name  : pdcpDataStatusInd
 *  Inputs         : ueIndex - UE Id,
 *                   lcId    - Logical channel Id,
 *                   deliveryStatus - SDU delivery status
 *  Outputs        : None
 *  Returns        : None
 *  Description    : This function is for compilation
 *****************************************************************************/
void pdcpDataStatusInd( UInt16 ueIndex, UInt16 lcId, UInt16 deliveryStatus )
{ 
    /* Do Nothing*/
}


/*SPR 21366 +*/
#if defined PDCP_GTPU_INTF
/****************************************************************************
 * Function Name  : sendGtpuRegisterPdcpInd
 * Inputs         : cellIdList_p,cellCount
 * Outputs        : None
 * Returns        : None
 * Description    : This function fill the gtpu register ind msg and send to 
 *                  GTPU module
 ****************************************************************************/
void sendGtpuRegisterPdcpInd( RrcCellIndex *cellIdList_p, UInt8 cellCount )
{
    LP_TlvMessage    message_p      = PNULL;
    LP_PdcpGtpuRegistration pdcpGtpuReg_p = PNULL;
    UInt8 *msgBuffer_p = PNULL;
    UInt16 totSize = sizeof(TlvMessage) + sizeof(PdcpGtpuRegistration);
    UInt8 idx = 0;

    msgBuffer_p = (UInt8 *)getMemFromPool( totSize, PNULL );
    if(!msgBuffer_p)
    {
        LOG_PDCP_FATAL( PDCP_RRCOAM, "Memory Allocation failed" );
        return;
    }
    message_p = (LP_TlvMessage)&msgBuffer_p[0];

    /* Prepare pointer to response message data */
    pdcpGtpuReg_p = (LP_PdcpGtpuRegistration)message_p->data;
    pdcpGtpuReg_p->instanceId = instanceL2_g;
    pdcpGtpuReg_p->cellCount = cellCount + 1;
    for( ; idx < (cellCount + 1); idx++ )
    {
        pdcpGtpuReg_p->cellIdList[idx] = *cellIdList_p;
        cellIdList_p++;
    }

    tlvPrepareMessage( message_p, 0xFF, PACKET_RELAY_MODULE_ID,
            GTP_REGISTER_PDCP_IND, sizeof(PdcpGtpuRegistration),
            API_HEADER_CELL_ID_NIL );

    /*Convert response fields to network byte order */
    tlvPrepareHeaderForSending( message_p );

    /* Send the data */
    (void)SendToGtpu( msgBuffer_p, totSize, PACKET_RELAY_MODULE_ID );
    /* Free the memory */
    freeMemPool( msgBuffer_p );
}
#endif
/*SPR 21366 -*/

#endif
