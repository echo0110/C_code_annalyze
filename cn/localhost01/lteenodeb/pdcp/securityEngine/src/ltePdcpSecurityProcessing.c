/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2011 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: ltePdcpSecurityScheduler.c $
 *
 ******************************************************************************
 *
 *  File Description : Security engine processing. 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 *
 *****************************************************************************/

/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#ifdef PDCP_ASYNC_INTERFACE
#include "ltePdcpEncSap.h"
#include "ltePdcpSecEngine.h"
#include "ltePdcpContext.h"
#include "confidentiality.h"
#include "integrity.h"
#include "ltePdcpErrors.h"
#include "ltePdcpLteDefs.h"
#include "ltePdcpProtocolTools.h"
#include "logging.h"
#include "alarm.h"
#include "ltePdcpStatistic.h"



/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static UInt32 pdcpSecToDecipherRx(LP_PdcpSecPacket secDataRx_p, 
		LP_PdcpDataInd dataInd_p, LP_PdcpSecOutPacket pdcpSecOutpacket_p
		);
static void pdcpSecIntegrityVerification(LP_PdcpSecPacket secDataRx_p, 
		LP_PdcpDataInd dataInd_p, LP_PdcpSecOutPacket pdcpSecOutpacket_p);

static UInt32 pdcpSecToCipherTx(LP_PdcpSecPacket secDataTx_p,
		/* SPR 15236 fix start */
		LP_PdcpSecOutPacket pdcpSecOutpacket_p
		/* SPR 15236 fix start */
		);

static void pdcpSecProcessedOutput( LP_PdcpSecOutPacket *pdcpSecOutpacket_p, 
		/* + Coverity 65114, 65115 */
		/* SPR 15236 fix start */
		LP_PdcpSecPacket secData_p, UInt8 direction, UInt16 ueIndex, 
		/* SPR 15236 fix end */
		UInt8 lcId );
/* - Coverity 65114, 65115 */
static void pdcpSecPollJobRingTx( UInt32 limit );
static void pdcpSecPollJobRingRx( UInt32 limit );
static  UInt32 pdcpProcessDecipher( LP_PdcpSecPacket secDataRx_p,
		LP_PdcpDataInd dataInd_p, UInt8 *msg_p , 
		UInt32 cipherSize);

/****************************************************************************
 * Private Constants
 ***************************************************************************/

/****************************************************************************
 * Exported Variables
 ***************************************************************************/
PdcpSecConfig pdcpSecConfig_g = {0};

/* SPR 15909 fix start */
extern tickType_t pdcpCurrentTime_g;
/* SPR 15909 fix end */
/* + SPR 19808 */
#ifdef LOG_PRINT_ENABLED
extern UInt8 *integrityCheckMACIbaseptr_p;
#endif
/* - SPR 19808 */
extern UInt8* pdcpProcessSrbTxMacI_p;
extern UInt8 *pdcpAsyncTxHeader_p;
extern UInt8 *pdcpAsyncRxHeader_p;

/****************************************************************************
 * Private Variables (Must be declared static)
 ***************************************************************************/

/****************************************************************************
 * Functions implementation 
 ****************************************************************************/
/****************************************************************************
 * Function Name  : pdcpSecInit
 * Inputs         : workMode - mechanism of how PDCP will receive the response
 *                             from the security engine. 
 * Outputs        : None.
 * Returns        : SUCCESS/FAILURE.
 * Description    : This API will be used for initializing the security engine. 
 ****************************************************************************/
UInt32 pdcpSecInit( UInt8 workMode)
{
	/*SPR 16529 fix start*/
    UInt16 ueId = 0;
    UInt8 lcId = 0, dir = 0;
	/*SPR 16529 fix end*/
	/* + SPR 17073 */
	/* + Coverity 93529 */
    UInt32 pdcpNumOfSupportedUsers = pdcpNumOfSupportedUsers_g;
    UInt16 srbQueueLimit = (UInt16)UPPER_POW_OF_TWO(pdcpNumOfSupportedUsers);
	/* - Coverity 93529 */
    /* The queue size is twice the number of number of UEs to accomodate two 
     * packets at a time */
    srbQueueLimit *= 2;
	/* - SPR 17073 */
    LOG_PDCP_MSG( PDCP_SEC_INIT, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__, workMode, 0,0,0, 0,0, __func__,""); 
    /* Create the pools used by security engine */
	/* + SPR 14179 */
    createMemPool( sizeof(PdcpSecPacket), ((pdcpNumOfSupportedUsers_g *
                    pdcpNumOfSupportedLC_g * MAX_NODES * 
                    pdcpDlMemFac_g)/100));
    createMemPool( sizeof(PdcpSecOutPacket), ((pdcpNumOfSupportedUsers_g *
                    pdcpNumOfSupportedLC_g * MAX_NODES * 
                    pdcpDlMemFac_g)/100));
	/* - SPR 14179 */
	/* SPR 15236 fix start */
    createMemPool( sizeof( UInt8 ) * LTE_SECURITY_KEY_LENGTH, 
            (MAX_UE_SUPPORTED * LTE_MAX_LC_ID * 
             PDCP_ENTITY_RB_BOTH ));

    for( ueId = 0; ueId < MAX_UE_SUPPORTED; ueId++ )
    {
        for( lcId = 0; lcId < LTE_MAX_LC_ID; lcId++ )
        {
            for( dir = 0; dir < PDCP_ENTITY_RB_BOTH; dir++ )
            {
                LP_PdcpSecContextInfo pdcpSecCtx_p = 
					/*SPR 16529 fix start*/
                    PDCP_SEC_CONTEXT( ueId, lcId, dir );
				/*SPR 16529 fix end*/
                pdcpSecCtx_p->oldCiphKey_p = (UInt8 *)getMemFromPool( 
                        sizeof( UInt8 ) * LTE_SECURITY_KEY_LENGTH, PNULL );
                if( PNULL == pdcpSecCtx_p->oldCiphKey_p )
                {
                    freeAllMemory( ueId, lcId, dir );
                    return PDCP_FAIL;
                }
                pdcpSecCtx_p->newCiphKey_p = (UInt8 *)getMemFromPool( 
                        sizeof( UInt8 ) * LTE_SECURITY_KEY_LENGTH, PNULL ); 
                if( PNULL == pdcpSecCtx_p->newCiphKey_p )
                {
                    freeAllMemory( ueId, lcId, dir );
                    freeMemPool( pdcpSecCtx_p->oldCiphKey_p );
                    return PDCP_FAIL;
                }
                pdcpSecCtx_p->integrityKey_p = (UInt8 *)getMemFromPool( 
                        sizeof( UInt8 ) * LTE_SECURITY_KEY_LENGTH, PNULL ); 
                if( PNULL == pdcpSecCtx_p->newCiphKey_p )
                {
                    freeAllMemory( ueId, lcId, dir );
                    freeMemPool( pdcpSecCtx_p->oldCiphKey_p );
                    freeMemPool( pdcpSecCtx_p->newCiphKey_p );
                    return PDCP_FAIL;
                }
            }
        }
    }
	/* SPR 15236 fix end */
    if( POLLING_MODE == workMode )
    {
        pdcpSecConfig_g.workMode = workMode;
        /* Init Security Context Queue for Input */
        QINIT_PDCP_SEC_INPUT_TX_Q();
        QINIT_PDCP_SEC_INPUT_RX_Q();
		/* + SPR 17073 */
        QINIT_PDCP_SEC_SRB_INPUT_TX_Q( srbQueueLimit );
        QINIT_PDCP_SEC_SRB_INPUT_RX_Q( srbQueueLimit );
		/* - SPR 17073 */
        /* Init Security Context Queue for Output */
        QINIT_PDCP_SEC_OUTPUT_TX_Q( pdcpSecConfig_g );
        QINIT_PDCP_SEC_OUTPUT_RX_Q( pdcpSecConfig_g );
        return PDCP_SUCCESS;
    }
    else if (INTERRUPT_MODE == workMode)
    {
#ifdef PDCP_ASYNC_INTR
        pdcpSecConfig_g.workMode = workMode;
        /* Init Security Context Queue for Input */
        QINIT_PDCP_SEC_INPUT_TX_Q();
        QINIT_PDCP_SEC_INPUT_RX_Q();
		/* + SPR 17073 */
        QINIT_PDCP_SEC_SRB_INPUT_TX_Q( srbQueueLimit );
        QINIT_PDCP_SEC_SRB_INPUT_RX_Q( srbQueueLimit );
		/* - SPR 17073 */
        /* Init Security Context Queue for Output */
        QINIT_PDCP_SEC_OUTPUT_TX_Q( pdcpSecConfig_g );
        QINIT_PDCP_SEC_OUTPUT_RX_Q( pdcpSecConfig_g );
#ifdef PDCP_ASYNC_INTR_TESTING
        QINIT_SEC_INTR_INPUT_RX_Q(0);
        QINIT_SEC_INTR_INPUT_TX_Q(0);
#endif
        return PDCP_SUCCESS;
#else
        return PDCP_FAIL;
#endif
    }
    else
    {
        return PDCP_FAIL;
    }
}

/****************************************************************************
 * Function Name  : pdcpCreateSecurityContext
 * Inputs         : jobRingHandler_p, 
 *                  secContext_p - Pointer to security context
 * Outputs        : None.
 * Returns        : SUCCESS/FAILURE.
 * Description    : This API will be used for updating the security engine. 
 ****************************************************************************/
UInt32 pdcpCreateSecurityContext( 
        UInt8 *jobRingHandler_p,
        LP_PdcpSecContextInfo secContext_p)
{
	if ( PNULL == secContext_p )
	{
		/* 
		   LOG_PDCP_MSG( PDCP_DATA_POINTER_NULL, LOGWARNING, PDCP_RRCOAM, 
		   pdcpCurrentTime_g, __LINE__, 0, 0, 0,0, 0,0, __func__,"");
		   */
		return PDCP_FAIL;
	}

	/*
	   LOG_PDCP_MSG( PDCP_SEC_CONTEXT_CREATE, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
	   __LINE__, secContext_p->bearer, secContext_p->direction, 0,0, 
	   0,0, __func__,""); 
	   */

	/* SPR 3276 fix start*/
	/* No need to maintain this table as call back function is already
	 * registered */
	/* mapping table maintained on the basis of lcid and direction */
	/* SPR 3276 fix end */
	return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpDeleteSecurityContext
 * Inputs         : secContext_p - Pointer to security context
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS/PDCP_FAIL.
 * Description    : This API will be used for deleting the security engine. 
 ****************************************************************************/
UInt32 pdcpDeleteSecurityContext( LP_PdcpSecContextInfo secContext_p)
{
	if (PNULL == secContext_p)
	{
		/* 
		   LOG_PDCP_MSG( PDCP_DATA_POINTER_NULL, LOGWARNING, PDCP_RRCOAM, 
		   pdcpCurrentTime_g, __LINE__, 0, 0, 0,0, 0,0, __func__,"");
		   */
		return PDCP_FAIL;
	}
	else
	{
		/*
		   LOG_PDCP_MSG( PDCP_SEC_CONTEXT_DELETE, LOGDEBUG, PDCP_RRCOAM, 
		   pdcpCurrentTime_g, __LINE__, secContext_p->bearer, 
		   secContext_p->direction, 0,0, 0,0, __func__,"");
		   */
		/* +- SPR 15236 */
	}
	return PDCP_SUCCESS;
}
/****************************************************************************
 * Function Name  : pdcpSecurityProcessPacket
 * Inputs         : secContext_p - Pointer to security context
 *                  inData_p - input data
 *                  outData_p - output data
 *                  uaCtxHandle - Pointer to meta-data.
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS/PDCP_FAIL.
 * Description    : This API will be used for sending packets to the security
 *                  engine. 
 ****************************************************************************/
UInt32 pdcpSecurityProcessPacket(
		LP_PdcpSecContextInfo secContext_p,
		void *inData_p, void *outData_p,
		uaContextHandler uaCtxHandle_p )
{
	LP_PdcpSecPacket pdcpSecData_p = PNULL;
	pdcpSecData_p = (LP_PdcpSecPacket)getMemFromPool( sizeof(PdcpSecPacket),
			PNULL);
	if ( PNULL == pdcpSecData_p  )
    {
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_TX, 
                pdcpCurrentTime_g, __LINE__, sizeof(PdcpSecPacket), 0,
                0,0,0,0, __func__,"pdcpSecData_p is NULL");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic ("Memory allocation fail for %s", __FUNCTION__);
        return PDCP_FAIL;
    }

	pdcpSecData_p->secContext_p = secContext_p;
	pdcpSecData_p->inData_p = inData_p;
	pdcpSecData_p->outData_p = outData_p;
	pdcpSecData_p->uaCtxHandle = uaCtxHandle_p ;

	if( PDCP_ENTITY_RB_TX == secContext_p->direction )
	{
		/*SPR 16529 fix start*/
		/* + Coverity 65114, 65115 */
		LP_PdcpCommDataReq commDataReq_p = PNULL;
		if( PDCP_TS_SRB_SN_SIZE == secContext_p->snSize )
		{
			LP_PdcpSrbDataReq srbDataReq_p = (LP_PdcpSrbDataReq)uaCtxHandle_p;
			commDataReq_p = &(srbDataReq_p->commDataReq); 
		}
		else
		{
			LP_PdcpDataReq dataReq_p = (LP_PdcpDataReq)uaCtxHandle_p;
			commDataReq_p = &(dataReq_p->commDataReq); 
		}
		pdcpSecData_p->ueIndex = commDataReq_p->ueIndex;
		pdcpSecData_p->lcId = commDataReq_p->lcId;
		/* - Coverity 65114, 65115 */
		/*SPR 16529 fix end*/
		/* SPR 15236 fix start */
		pdcpSecData_p->count = commDataReq_p->count;
		/* SPR 15236 fix end */

		/* Enqueue the Packets in Sec Input TX Queue */
		/* + SPR 17073 */
		if( PDCP_TS_SRB_SN_SIZE == secContext_p->snSize )
		{
			if( 0 == ENQUEUE_PDCP_SEC_SRB_INPUT_TX_Q( pdcpSecData_p ) )
			{
				freeMemPool( pdcpSecData_p );
				pdcpSecData_p = PNULL;
				return PDCP_FAIL;
			}
		}
		else
		{
			if( 0 == ENQUEUE_PDCP_SEC_INPUT_TX_Q( pdcpSecData_p ) )
			{
				freeMemPool( pdcpSecData_p );
				pdcpSecData_p = PNULL;
				return PDCP_FAIL;
			}
		}
		/* - SPR 17073 */
	}
	else
	{
		/*SPR 16529 fix start*/
		/* + Coverity 65114, 65115 */
		LP_PdcpDataInd dataInd_p = (LP_PdcpDataInd)(uaCtxHandle_p);
		pdcpSecData_p->ueIndex = dataInd_p->ueIndex;
		pdcpSecData_p->lcId = dataInd_p->lcId;
		/* SPR 15236 fix start */
		pdcpSecData_p->count = dataInd_p->count;
		/* SPR 15236 fix end */
		/* - Coverity 65114, 65115 */
		/*SPR 16529 fix end*/
		/* Enqueue the Packets in Sec Input RX Queue */
		/*SPR_16529_review_commet +*/
		/* + SPR 17073 */
		if( PDCP_TS_SRB_SN_SIZE == secContext_p->snSize )
		{
			if( 0 == ENQUEUE_PDCP_SEC_SRB_INPUT_RX_Q( pdcpSecData_p ) )
			{
				freeMemPool( pdcpSecData_p );
				pdcpSecData_p = PNULL;
				return PDCP_FAIL;
			}
		}
		else
		{
			if( 0 == ENQUEUE_PDCP_SEC_INPUT_RX_Q( pdcpSecData_p ) )
			{
				freeMemPool( pdcpSecData_p );
				pdcpSecData_p = PNULL;
				return PDCP_FAIL;
			}
			/*SPR_16529_review_commet -*/
		}
		/* - SPR 17073 */
	}
	return PDCP_SUCCESS;
}
#ifdef POLL_JOB_RING
/****************************************************************************
 * Function Name  : pdcpSecPollJobRing
 * Inputs         : jobRingHandler_p - Pointer to output queue
 *                  limit - number of packets to be retrieved
 *                  packetsNo - Number of packets submitted
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS/PDCP_FAIL.
 * Description    : This API will be used for polling the security engine for
 *                  a job ring. 
 ****************************************************************************/
UInt32 pdcpSecPollJobRing( 
        UInt8 *jobRingHandler_p,
        UInt32 limit,
        UInt32 *packetsNo)
{
	/* + SPR 17073 */
	if ( jobRingHandler_p == (UInt8 *)(&pdcpSecInputTxQ_g) || 
			jobRingHandler_p == (UInt8 *)(&pdcpSecSrbInputTxQ_g) )
		/* - SPR 17073 */
    {
        LOG_PDCP_MSG( PDCP_SEC_POLL_JOB_RING, LOGDEBUG, PDCP_TX, pdcpCurrentTime_g,
                __LINE__, limit, 0, 0,0, 0,0, __func__,"OutputTxQueue");
        /* Poll for TX procssing */
        if ( RETRIEVE_ALL_PACKETS == limit )  
        {
            limit = QCOUNT_PDCP_SEC_OUTPUT_TX_Q(pdcpSecConfig_g);
        }
		/* Coverity 69729 Fix Start*/
        if( 0 != limit )
        {
            pdcpSecPollJobRingTx( limit );
        }
		/* Coverity 69729 Fix End*/
    }
	else
    {
        LOG_PDCP_MSG( PDCP_SEC_POLL_JOB_RING, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g,
                __LINE__, limit, 0, 0,0, 0,0, __func__,"OutputRxQueue");
        /* Poll for RX procssing */
        if ( RETRIEVE_ALL_PACKETS == limit )
        {
            limit = QCOUNT_PDCP_SEC_OUTPUT_RX_Q(pdcpSecConfig_g);
        }
		/* Coverity 69729 Fix Start*/
        if( 0 != limit )
        {
            pdcpSecPollJobRingRx( limit );
        }
		/* Coverity 69729 Fix End*/
    }

	return PDCP_SUCCESS;
}
#else
/****************************************************************************
 * Function Name  : pdcpSecPoll
 * Inputs         : limit - number of packets to be retrieved
 *                  weight - weight for each job ring
 *                  packetsNo - Number of packets submitted
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS/PDCP_FAIL.
 * Description    : This API will be used for polling the security engine for
 *                  a job ring. 
 ****************************************************************************/
UInt32 pdcpSecPoll( 
        Int limit,
        UInt32 weight,
        UInt32 *packetsNo)
{
	UInt8 ctr = 0;

	if ( RETRIEVE_ALL_PACKETS == limit )
	{
		limit =  QCOUNT_PDCP_SEC_OUTPUT_TX_Q(pdcpSecConfig_g) + \
			 QCOUNT_PDCP_SEC_OUTPUT_RX_Q(pdcpSecConfig_g);
	}

	while ( ctr < limit )
	{
		/* Poll TX packets */
		/*
		   LOG_PDCP_MSG( PDCP_SEC_POLL_JOB_RING, LOGDEBUG, PDCP_TX, pdcpCurrentTime_g,
		   __LINE__, limit, 0, 0,0, 0,0, __func__,"OutputTxQueue");
		   */
		pdcpSecPollJobRingTx( weight );
		ctr += weight;

		/* Poll RX packets */
		/*
		   LOG_PDCP_MSG( PDCP_SEC_POLL_JOB_RING, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g,
		   __LINE__, limit, 0, 0,0, 0,0, __func__,"OutputRxQueue");
		   */
		pdcpSecPollJobRingRx( weight );
		ctr += weight;
	}

	return PDCP_SUCCESS;
}
#endif

/****************************************************************************
 * Function Name  : pdcpSecProcessPacketRx
 * Inputs         : secDataRx_p - Pointer to input queue node
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS/PDCP_FAIL.
 * Description    : This function will process RX processing to be done by
 *                  security engine.
 ****************************************************************************/
UInt32 pdcpSecProcessPacketRx(LP_PdcpSecPacket secDataRx_p)
{
    UInt32 retVal = PDCP_SUCCESS;
    UInt8 size = 0;
	/* + SPR 17073 */
    UInt8 snSize = 0;
	/* - SPR 17073 */
    LP_PdcpDataInd dataInd_p = (LP_PdcpDataInd)(secDataRx_p->uaCtxHandle);
    LP_PdcpSecOutPacket pdcpSecOutpacket_p = PNULL;
    /* Allocate memory for output node */
    pdcpSecProcessedOutput( &pdcpSecOutpacket_p, secDataRx_p, 
			/* + Coverity 65114, 65115 */
			/* SPR 15236 fix start */
            PDCP_ENTITY_RB_RX, secDataRx_p->ueIndex, secDataRx_p->lcId );
	/* SPR 15236 fix end */
	/* - Coverity 65114, 65115 */
    do
    {
		/* + Coverity 65114, 65115 */
		/* SPR 15236 fix start */
        LP_PdcpUeContext ueCtx_p = (LP_PdcpUeContext)PDCP_GET_UE_CONTEXT(
                secDataRx_p->ueIndex );
        if( PNULL == ueCtx_p ) 
        {
            /* Send Free notification as th context is deleted */
            pdcpSecOutpacket_p->secOutCbk = pdcpFreeRxDataFromSec; 
            break;
        }
		/* - Coverity 65114, 65115 */
        if( PNULL == PDCP_GET_VALID_ENTITY_CONTEXT( ueCtx_p, 
                    secDataRx_p->lcId, PDCP_ENTITY_RB_RX ) )
        {
            /*
               LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_RX, LOGWARNING, PDCP_RX,
               pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex, 
               dataInd_p->lcId, 0,0, 0,0, __func__,"");
               */
            /* Send Free notification as th context is deleted */
            pdcpSecOutpacket_p->secOutCbk = pdcpFreeRxDataFromSec; 
            break;
        }
		/* SPR 15236 fix end */
		/* + SPR 17073 */
        snSize = secDataRx_p->secContext_p->snSize;
		/* - SPR 17073 */
        size = GET_BYTESIZE_FROM_BITSIZE( snSize );

        /* extract the header into pdcpAsyncRxHeader_p */
        if ( 0 == msgExtract( secDataRx_p->inData_p, 0, pdcpAsyncRxHeader_p, size) )
        {
            /*
               LOG_PDCP_MSG( PDCP_MSG_EXTRACT_FAILED, LOGFATAL, PDCP_RX, 
               pdcpCurrentTime_g, __LINE__, size, dataInd_p->ueIndex,
               dataInd_p->lcId, dataInd_p->count, 0,0, __func__,
               "PDCP header cannot be extracted");
               */
            pdcpSecOutpacket_p->status = SEC_STATUS_ERROR;
            break;
        }
        /* insert the header into the output buffer */
        if (PNULL == msgInsert(secDataRx_p->outData_p, 0, pdcpAsyncRxHeader_p, size))
        {
            /*
               LOG_PDCP_MSG( PDCP_MSG_INSERT_FAILED, LOGFATAL, PDCP_RX, 
               pdcpCurrentTime_g, __LINE__, size, dataInd_p->ueIndex,
               dataInd_p->lcId, dataInd_p->count, 0,0, __func__,
               "Cannot insert header to output");
               */
            ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);            
            pdcpSecOutpacket_p->errInfo = SEC_OUT_OF_MEMORY;          
            break;
        }
        /*Remove header from input buffer so that data can be deciphered*/
        if ( 0 == msgRemove( secDataRx_p->inData_p, 0,size))
        {
            pdcpSecOutpacket_p->status = SEC_STATUS_ERROR;            
            break;
        }
        /* deciphering*/
        if (PDCP_SUCCESS != pdcpSecToDecipherRx(secDataRx_p, dataInd_p, 
                    pdcpSecOutpacket_p
                    ) )
        {
            break;
        }

        /* send for integrity check only if SRB packet */
        /* SPR 3276 changes start */
        if ( LTE_SRB_LC_ID_HIGH_BOUND >= dataInd_p->lcId )
            /* SPR 3276 changes end */
        {
            pdcpSecIntegrityVerification(secDataRx_p, dataInd_p, 
                    pdcpSecOutpacket_p);
        }
    } while (0);

    freeMemPool(secDataRx_p);
    secDataRx_p = PNULL;

    /* store the data into output RX queue*/
    {
#ifdef PDCP_ASYNC_INTR_TESTING
        ENQUEUE_SEC_INTR_INPUT_RX_Q(pdcpSecOutpacket_p);
        raise_wrapper(SIGUSR1);
#else

			/*Coverity Fix 94069 , 94091 Start*/
        retVal = ENQUEUE_PDCP_SEC_OUTPUT_RX_Q(pdcpSecConfig_g, pdcpSecOutpacket_p);
        if(PDCP_SUCCESS != retVal)
        {
            freeMemPool(pdcpSecOutpacket_p);	
            return PDCP_FAIL;
        }
			/*Coverity Fix 94069 , 94091 End*/
#endif
    }

    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpProcessDecipher
 * Inputs         : secDataRx_p - Pointer to input queue node
 *                  dataInd_p - meta-data pointer
 *                  msg_p - actual data for deciphering in non-ZCB format
 *                  dataSize - data buffer size
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS/PDCP_FAIL.
 * Description    : This function will send the packet for deciphering
 ****************************************************************************/
UInt32 pdcpProcessDecipher( LP_PdcpSecPacket secDataRx_p,
        LP_PdcpDataInd dataInd_p,
        UInt8 *msg_p,
        UInt32 dataSize
        )
{
    UInt32 retVal = PDCP_SUCCESS;
    UInt8  algoId = 0;

    if ( NORMAL_PACKET == dataInd_p->packetForReEstablishment)
    {
        algoId = secDataRx_p->secContext_p->newCiphAlgoId;
    }
    else
    {
        algoId = secDataRx_p->secContext_p->oldCiphAlgoId;
    }
    /* SPR 15342 fix start */
    /* SPR 15342 fix end */
    if ( ALGO_EEA0 != algoId )
    {
        /*
           LOG_PDCP_MSG( PDCP_DATA_TO_CIPHERING_ENGINE, LOGDEBUG, PDCP_RX, 
           pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex, 
           dataInd_p->lcId, dataInd_p->count,0, 0,0,__func__,"");

*/
        /* Check for re-establishment flag if old or new keys needs to be
           applied */
        if ( NORMAL_PACKET == dataInd_p->packetForReEstablishment)
        {
            retVal = sendToDeciphering( algoId,
                    secDataRx_p->secContext_p->bearer,
                    dataInd_p->count,PDCP_ENTITY_RB_TX,
                    secDataRx_p->secContext_p->newCiphKey_p,
                    msg_p, PNULL, msg_p, dataSize, 0
                    );
        }
        else
        {
            retVal = sendToDeciphering( algoId,
                    secDataRx_p->secContext_p->bearer,
                    dataInd_p->count,PDCP_ENTITY_RB_TX,
                    secDataRx_p->secContext_p->oldCiphKey_p,
                    msg_p, PNULL, msg_p, dataSize, 0
                    );
        }
    }    
    else
    {
        /* Update statistics data for null deciphering */
        pdcpStats.statsCiphering.numberOfPacketsDecipheredUsingEIA0 += 1;
    }
    return retVal;
}

/****************************************************************************
 * Function Name  : pdcpSecToDecipherRx
 * Inputs         : secDataRx_p - Pointer to input queue node
 *                  dataInd_p - meta-data pointer,
 *                  pdcpSecOutpacket_p
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS/PDCP_FAILURE.
 * Description    : This function will do deciphering for a packet
 ****************************************************************************/
UInt32 pdcpSecToDecipherRx(LP_PdcpSecPacket secDataRx_p, 
        LP_PdcpDataInd dataInd_p, LP_PdcpSecOutPacket pdcpSecOutpacket_p
        )
{
	UInt32 retVal = PDCP_SUCCESS;
	UInt32 size = 0;
	UInt32 cipherSize = 0;
	UInt32 msgSegCnt = 0;
	UInt8 *msg = PNULL;
	QSEGMENT segment = {0};

	msgSegCnt = msgSegCount(secDataRx_p->inData_p);
	size = msgSize(secDataRx_p->inData_p,PNULL);
	cipherSize = size;

	if ( ( SINGLE_SEGMENT == msgSegCnt) &&
			( 0 == size % CIPHER_MULTIPLE ) )
	{
		if (PNULL == msgSegNext(secDataRx_p->inData_p, 0, &segment))
		{
			/*
			   LOG_PDCP_MSG (PDCP_MSG_SEG_NEXT_FAILED, LOGFATAL, PDCP_RX, 
			   pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex,
			   dataInd_p->lcId,0,0,0,0,__func__,"");
			   */
			pdcpSecOutpacket_p->status = SEC_STATUS_ERROR;            
			return PDCP_FAIL;
		}
		msg  = segment.base;
	}
	else /* Multiple segments in the message */
	{
		if ( 0 != size % CIPHER_MULTIPLE )
		{
			cipherSize += ( CIPHER_MULTIPLE - (size % CIPHER_MULTIPLE) );
		}
		msg = (UInt8 *)getMemFromPool(cipherSize, PNULL);
		if ( PNULL == msg )
		{
			/*
			   LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RX, 
			   pdcpCurrentTime_g, __LINE__, cipherSize, 
			   dataInd_p->ueIndex, dataInd_p->lcId, dataInd_p->count,
			   0,0, __func__,"Cannot allocate memory for deciphering");
			   */
			ALARM_MSG( PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
					CRITICAL_ALARM);
			pdcpSecOutpacket_p->errInfo = SEC_OUT_OF_MEMORY;          
			return PDCP_FAIL;
		}
		if ( 0 == msgExtract(secDataRx_p->inData_p, 0, msg, size ) )
		{
			/*
			   LOG_PDCP_MSG( PDCP_MSG_EXTRACT_FAILED, LOGFATAL, PDCP_RX, 
			   pdcpCurrentTime_g, __LINE__, size, dataInd_p->ueIndex,
			   dataInd_p->lcId, dataInd_p->count, 0,0, __func__,
			   "ZCB data cannot be extracted for deciphering");
			   */
			pdcpSecOutpacket_p->status = SEC_STATUS_ERROR;            
			freeMemPool(msg);
			msg = PNULL;
			return PDCP_FAIL;
		}
	}

	/* Send the data for deciphering */
	retVal = pdcpProcessDecipher(secDataRx_p, dataInd_p, msg, 
			cipherSize );
	if (PDCP_SUCCESS != retVal)
	{
		/*
		   LOG_PDCP_MSG (PDCP_DECIPHERING_FAILED, LOGFATAL, PDCP_RX, 
		   pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex,
		   dataInd_p->lcId, dataInd_p->count, cipherSize, 
		   0,0,__func__,"");
		   */
		ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
				CRITICAL_ALARM);
		if (( SINGLE_SEGMENT == msgSegCnt) &&( 0 == size % CIPHER_MULTIPLE ))
		{
			/*no need to free as no memory allocation done for single segment*/
		}
		else
		{
			freeMemPool(msg);
			msg = PNULL;
		}
		pdcpSecOutpacket_p->errInfo = SEC_OUT_OF_MEMORY;          
		return PDCP_FAIL;
	}

	if (PNULL == msgInsert(secDataRx_p->outData_p, -1, msg, size) )
	{
		/*
		   LOG_PDCP_MSG( PDCP_MSG_INSERT_FAILED, LOGFATAL, PDCP_RX, 
		   pdcpCurrentTime_g, __LINE__, size, dataInd_p->ueIndex,
		   dataInd_p->lcId, dataInd_p->count, 0,0, __func__,
		   "data cannot be converted bach to ZCB after deciphering ");
		   */
		ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
		if (( SINGLE_SEGMENT == msgSegCnt) &&( 0 == size % CIPHER_MULTIPLE ))
		{
			/*no need to free as no memory allocation done for single segment*/
		}
		else
		{
			freeMemPool(msg);
			msg = PNULL;  
		}
		pdcpSecOutpacket_p->errInfo = SEC_OUT_OF_MEMORY; 
		return PDCP_FAIL;
	}
	if (( SINGLE_SEGMENT == msgSegCnt) &&( 0 == size % CIPHER_MULTIPLE ))
	{
		/*no need to free as no memory allocation done for single segment*/
	}
	else
	{
		freeMemPool(msg);
		msg = PNULL;
	}
	return retVal;
}

/****************************************************************************
 * Function Name  : pdcpSecIntegrityVerification
 * Inputs         : secDataRx_p - Pointer to input queue node,
 *                  dataInd_p - meta-data pointer,
 *                  pdcpSecOutpacket_p
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function will do integrity verification for a packet
 ****************************************************************************/
void pdcpSecIntegrityVerification(LP_PdcpSecPacket secDataRx_p, 
        LP_PdcpDataInd dataInd_p, LP_PdcpSecOutPacket pdcpSecOutpacket_p)
{
	UInt16 integrityResult = PDCP_FAIL;

	if ( ALGO_EIA0 != secDataRx_p->secContext_p->integrityAlgoId)
	{
		/*
		   LOG_PDCP_MSG( PDCP_DATA_TO_INTEGRITY_ENGINE, LOGDEBUG, PDCP_RX, 
		   pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex,
		   dataInd_p->lcId, dataInd_p->count,0,0,0,__func__,"");
		   */
		/*check integrity*/
		integrityResult = integrityCheckMACI(
				secDataRx_p->secContext_p->integrityAlgoId,
				secDataRx_p->secContext_p->bearer,
				dataInd_p->count,
				PDCP_ENTITY_RB_TX,
				secDataRx_p->secContext_p->integrityKey_p,
				secDataRx_p->outData_p 
				/* - SPR 18103 */
#ifndef UE_SIM_TESTING
				/* SPR 8174 Changes Start */
				,&(secDataRx_p->secContext_p->reconfigUeFlag)
				/* SPR 8174 Changes End */
#endif
				);

		/*check integrity result*/
		if ( PDCP_SUCCESS != integrityResult )
		{
			LOG_PDCP_MSG( PDCP_INTEGRITY_VERIFICATION_FAILED, LOGWARNING, PDCP_RX,
					pdcpCurrentTime_g, __LINE__,dataInd_p->ueIndex, 
					dataInd_p->lcId, dataInd_p->count,0, 0,0,__func__,"");
			/* Update statistic data */
			pdcpStats.statsIntegrity.\
				numberOfPacketsRejectedByIntegrityVerification += 1;
			pdcpSecOutpacket_p->status = SEC_STATUS_MAC_I_CHECK_FAILED;
		}
	}
	else
	{
		/* + SPR 19808 */
#ifdef LOG_PRINT_ENABLED
		UInt8 temp = 0;        /*CID 110672*/
		if( 0 != msgExtract( secDataRx_p->outData_p, -1, 
					integrityCheckMACIbaseptr_p, MACI_LEN ) )
		{
			if ( 0 != memCmp(integrityCheckMACIbaseptr_p, &temp, MACI_LEN) )
			{
				LOG_PDCP_WARNING( PDCP_RX, "Non-Zero MAC-I received for NULL "
						"integrity for UE[%u]", dataInd_p->ueIndex );
			}
		}
		else
		{
			LOG_PDCP_FATAL( PDCP_RX, "Unable to extract MAC-I value for UE[%u]",
					dataInd_p->ueIndex );
		}
#endif
		/* - SPR 19808 */
		/* Update stats for null integrity */
		pdcpStats.statsIntegrity.numberOfPacketsIntegrityVerifiedUsingEIA0 
			+= 1;
	}
	/*remove MACI from data_p*/
	if( 0 == msgRemove(secDataRx_p->outData_p, -1, MACI_LEN) )
	{
		pdcpSecOutpacket_p->status = SEC_STATUS_ERROR;            
	}
}

/****************************************************************************
 * Function Name  : pdcpSecProcessedOutput
 * Inputs         : pdcpSecOutpacket_p - Pointer to output queue node,
 *                  secData_p - Pointer to input queue node,
 *                  direction
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function will define the output queue node to be 
 *                  invoked in callback function.
 ****************************************************************************/
void pdcpSecProcessedOutput( LP_PdcpSecOutPacket *pdcpSecOutpacket_p, 
		/* + Coverity 65114, 65115 */
		/* SPR 15236 fix start */
		LP_PdcpSecPacket secData_p, UInt8 direction, UInt16 ueIndex, 
		/* SPR 15236 fix end */
		UInt8 lcId )
/* - Coverity 65114, 65115 */
{
	*pdcpSecOutpacket_p = (LP_PdcpSecOutPacket)getMemFromPool( 
			sizeof(PdcpSecOutPacket), PNULL);
	if ( PNULL == *pdcpSecOutpacket_p  )
    {
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_TX, 
                pdcpCurrentTime_g, __LINE__, sizeof(PdcpSecPacket), 0,
                0,0,0,0, __func__,"pdcpSecData_p is NULL");

        msgFree(secData_p->inData_p);
        secData_p->inData_p = PNULL;
        msgFree(secData_p->outData_p);
        secData_p->outData_p = PNULL;
        if ( PDCP_ENTITY_RB_TX == secData_p->secContext_p->direction)
        {
            if( PDCP_TS_SRB_SN_SIZE == secData_p->secContext_p->snSize)
            {
                pdcpFreeSrbTxDataReq((LP_PdcpSrbDataReq)(secData_p->uaCtxHandle)); 
            }
            else
            {
                pdcpFreeTxDataReq((LP_PdcpDataReq)(secData_p->uaCtxHandle)); 
            }
            secData_p->uaCtxHandle = PNULL;
        }
        else
        {
            pdcpFreeRxDataInd((LP_PdcpDataInd)(secData_p->uaCtxHandle));
            secData_p->uaCtxHandle = PNULL;
        }

        freeMemPool(secData_p);
        secData_p = PNULL ;

        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic ("Memory allocation fail for %s", __FUNCTION__);
        return ;
    }
	(*pdcpSecOutpacket_p)->inData_p = secData_p->inData_p;
	(*pdcpSecOutpacket_p)->outData_p = secData_p->outData_p;
	(*pdcpSecOutpacket_p)->uaCtxHandle = secData_p->uaCtxHandle;
	(*pdcpSecOutpacket_p)->status = SEC_STATUS_SUCCESS;
	(*pdcpSecOutpacket_p)->errInfo = SEC_SUCCESS;
	/* SPR 15236 fix start */
	(*pdcpSecOutpacket_p)->ueIndex = ueIndex;
	(*pdcpSecOutpacket_p)->lcId = lcId;
	/* SPR 15236 fix end */
	/* SPR 3276 changes start */
	/* + Coverity 65114, 65115 */
	if ( PDCP_ENTITY_RB_TX == direction )
	{
		if( LTE_SRB_LC_ID_HIGH_BOUND >= lcId )
		{
			(*pdcpSecOutpacket_p)->secOutCbk = pdcpProcessTxSrbPktsFrmSec; 
		}
		else
		{
			(*pdcpSecOutpacket_p)->secOutCbk = pdcpProcessTxDrbPktsFrmSec; 
		}
	}
	else
	{
		if( LTE_SRB_LC_ID_HIGH_BOUND >= lcId )
		{
			(*pdcpSecOutpacket_p)->secOutCbk = pdcpProcessRxSrbPktsFrmSec; 
		}
		else
		{
			(*pdcpSecOutpacket_p)->secOutCbk = pdcpProcessRxDrbPktsFrmSec; 
		}
	}
	/* - Coverity 65114, 65115 */
	/* SPR 3276 changes end */
}

/****************************************************************************
 * Function Name  : pdcpSecProcessPacketTx
 * Inputs         : secDataTx_p - Pointer to input queue node
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS/PDCP_FAIL.
 * Description    : This function will process TX processing to be done by
 *                  security engine.
 ****************************************************************************/
UInt32 pdcpSecProcessPacketTx(LP_PdcpSecPacket secDataTx_p)
{
    UInt8 size = 0;
    LP_PdcpSecOutPacket pdcpSecOutpacket_p = PNULL;
	/* + SPR 17073 */
    UInt8 snSize = 0;
	/* - SPR 17073 */
    pdcpSecProcessedOutput( &pdcpSecOutpacket_p, secDataTx_p,
			/* + Coverity 65114, 65115 */
			/* SPR 15236 fix start */
            PDCP_ENTITY_RB_TX,
            secDataTx_p->ueIndex, secDataTx_p->lcId );
	/* SPR 15236 fix end */
	/* - Coverity 65114, 65115 */
    do
    {
		/* + Coverity 65114, 65115 */
		/* SPR 15236 fix start */
        LP_PdcpUeContext ueCtx_p = (LP_PdcpUeContext)PDCP_GET_UE_CONTEXT(
                secDataTx_p->ueIndex );
        if( PNULL == ueCtx_p ) 
        {
            /* Send Free notification as th context is deleted */
            pdcpSecOutpacket_p->secOutCbk = pdcpFreeTxDataFromSec; 
            break;
        }
		/* - Coverity 65114, 65115 */
        if( PNULL == PDCP_GET_VALID_ENTITY_CONTEXT( ueCtx_p, 
                    secDataTx_p->lcId, PDCP_ENTITY_RB_TX ) )

        {
            /*
               LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_RX, LOGWARNING, PDCP_RX,
               pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex, 
               dataInd_p->lcId, 0,0, 0,0, __func__,"");
               */
            /* Send Free notification as th context is deleted */
            pdcpSecOutpacket_p->secOutCbk = pdcpFreeTxDataFromSec; 
            break;
        }
		/* SPR 15236 fix end */

		/* + SPR 17073 */
        snSize = secDataTx_p->secContext_p->snSize;
		/* - SPR 17073 */
        /* Allocate memory for output node */
        size = GET_BYTESIZE_FROM_BITSIZE( snSize );

        /* Send for integrity protection, only applicable for SRB packets */
		/* SPR 3276 changes start */
        if( PDCP_TS_SRB_SN_SIZE == secDataTx_p->secContext_p->snSize )
			/* SPR 3276 changes end */
        {
			/* +- SPR 15236 */
            /* Reset pdcpProcessSrbTxMacI_p to 0 */
            memSet(pdcpProcessSrbTxMacI_p, 0, MACI_LEN);

            if ( ALGO_EIA0 != secDataTx_p->secContext_p->integrityAlgoId)
            {
                /*
                   LOG_PDCP_MSG( PDCP_DATA_TO_INTEGRITY_ENGINE, LOGDEBUG, PDCP_TX,
                   pdcpCurrentTime_g, __LINE__, commData_p->ueIndex,  
                   commData_p->bearer, commData_p->count, 
                   0,0,0,__func__,"");
                   */
                /*calc MACI*/
                integrityGenerateMACI(
                        secDataTx_p->secContext_p->integrityAlgoId,
                        secDataTx_p->secContext_p->bearer,
                        /* SPR 15236 fix start */
                        secDataTx_p->count,
                        /* SPR 15236 fix end */
                        PDCP_ENTITY_RB_RX,
                        secDataTx_p->secContext_p->integrityKey_p,
                        secDataTx_p->inData_p,
                        /* - SPR 18103 */
                        pdcpProcessSrbTxMacI_p );
            }
            else
            {
                /* 6.3.4. For control plane data that are not integrity protected,
                 * the MAC-I field is still present and should be padded with
                 * padding bits set to 0.*/
                /* MAC-I is already reset to zero before checking the integrity 
                 * algo, hence no need to again reset it */
                pdcpStats.statsIntegrity.\
                    numberOfPacketsIntegrityProtectedUsingEIA0 += 1;
            }
            /*Store MACI value after DATA*/
            if(PNULL == msgInsert(secDataTx_p->inData_p, -1, 
                        pdcpProcessSrbTxMacI_p, MACI_LEN))
            {
                /*
                   LOG_PDCP_MSG( PDCP_MSG_INSERT_FAILED, LOGFATAL, PDCP_TX, pdcpCurrentTime_g,
                   __LINE__, MACI_LEN,  commData_p->ueIndex, 
                   secDataTx_p->secContext_p->bearer, commData_p->count,
                   0,0, __func__,"Cannot insert MAC-I");
                   */
                ALARM_MSG( PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                        CRITICAL_ALARM);
                pdcpSecOutpacket_p->errInfo = SEC_OUT_OF_MEMORY;
                break;
            }
        }
        /* +- SPR 15236 */
        /* extract the header into pdcpAsyncTxHeader_p */        
        if ( 0 == msgExtract( secDataTx_p->inData_p, 0, pdcpAsyncTxHeader_p, 
                    size) )
        {  
            /*
               LOG_PDCP_MSG( PDCP_MSG_EXTRACT_FAILED, LOGFATAL, PDCP_TX, pdcpCurrentTime_g,
               __LINE__, size, commData_p->ueIndex, commData_p->lcId, 
               commData_p->count, 0,0, __func__, 
               "PDCP header cannot be extracted");
               */
            pdcpSecOutpacket_p->status = SEC_STATUS_ERROR;
            break;
        }

        /* insert the header into the output buffer */
        if (PNULL == msgInsert(secDataTx_p->outData_p, 0, pdcpAsyncTxHeader_p, 
                    size) )
        {
            /*
               LOG_PDCP_MSG( PDCP_MSG_INSERT_FAILED, LOGFATAL, PDCP_TX, pdcpCurrentTime_g,
               __LINE__, size, commData_p->ueIndex, commData_p->lcId, 
               commData_p->count, 0,0, __func__,
               "Cannot insert header to output");
               */
            ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
            pdcpSecOutpacket_p->errInfo = SEC_OUT_OF_MEMORY;
            break;
        }
        /*Remove header from input buffer so that data can be ciphered*/
        if ( 0 == msgRemove( secDataTx_p->inData_p, 0,size) )
        {
            /*
               LOG_PDCP_MSG( PDCP_MSG_REMOVE_FAILED, LOGFATAL, PDCP_TX,
               pdcpCurrentTime_g, __LINE__, size, commData_p->ueIndex,
               commData_p->lcId, commData_p->count, 0,0,
               __func__, "Cannot remove PDCP Header");
               */
            pdcpSecOutpacket_p->status = SEC_STATUS_ERROR;
            break;
        }        
        /* ciphering*/
        /* SPR 15236 fix start */
        if (PDCP_SUCCESS != pdcpSecToCipherTx(secDataTx_p, 
                    /* SPR 15236 fix end */
                    pdcpSecOutpacket_p
                    ))
        {
            break;
        }
    }while( 0 );

    freeMemPool(secDataTx_p);
    secDataTx_p = PNULL;
    {
        /* store the data into output TX queue*/
#ifdef PDCP_ASYNC_INTR_TESTING
        ENQUEUE_SEC_INTR_INPUT_TX_Q(pdcpSecOutpacket_p);
        raise_wrapper(SIGUSR2);
#else
        /* + SPR 19525 */
        if( 0 == ENQUEUE_PDCP_SEC_OUTPUT_TX_Q(pdcpSecConfig_g, pdcpSecOutpacket_p) )
        {
            /* Call the call back function to free the packet */
            pdcpSecOutpacket_p->status = SEC_STATUS_ERROR;
            pdcpSecOutpacket_p->secOutCbk(
                    pdcpSecOutpacket_p->inData_p,
                    pdcpSecOutpacket_p->outData_p,
                    pdcpSecOutpacket_p->uaCtxHandle,
                    pdcpSecOutpacket_p->status,
                    pdcpSecOutpacket_p->errInfo);

            freeMemPool( pdcpSecOutpacket_p );
            pdcpSecOutpacket_p = PNULL;
        }
        /* - SPR 19525 */
#endif
    }

    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpSecToCipherTx
 * Inputs         : secDataTx_p - Pointer to input queue node,
 *                  dataReq_p - meta-data pointer,
 *                  pdcpSecOutpacket_p
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS/PDCP_FAIL.
 * Description    : This function will do ciphering for a packet
 ****************************************************************************/
UInt32 pdcpSecToCipherTx(LP_PdcpSecPacket secDataTx_p,
					/* SPR 15236 fix start */
        LP_PdcpSecOutPacket pdcpSecOutpacket_p
					/* SPR 15236 fix end */
        )
{
	/* + SPR 14179 */
    UInt32 result = PDCP_SUCCESS;
	/* - SPR 14179 */

    UInt32 size = msgSize(secDataTx_p->inData_p,PNULL);
    QSEGMENT segment ;
    UInt32 cipherSize = 0;
    UInt8 *inBuffP1_p = PNULL;
    UInt32 msgSegCnt = msgSegCount(secDataTx_p->inData_p);
    /*Ciphering (EEA1) compliance requires data to be in multiple
      of 4, i.e size (bytes) should be in multiple of 4 */
    cipherSize = size;

    /* Single segment in the message  */
    if ( ( SINGLE_SEGMENT == msgSegCnt) && ( 0 == size % CIPHER_MULTIPLE ) )
    {
        if (PNULL == msgSegNext(secDataTx_p->inData_p,0,&segment) )
        {
            /*
               LOG_PDCP_MSG( PDCP_MSG_SEG_NEXT_FAILED, LOGWARNING, PDCP_TX,
               pdcpCurrentTime_g, __LINE__, dataReq_p->ueIndex,
               dataReq_p->lcId, 0,0, 0,0, __func__, "");
               */
            pdcpSecOutpacket_p->status = SEC_STATUS_ERROR;
            return PDCP_FAIL;
        }
        inBuffP1_p = segment.base;
    }
    else /* Multiple segments in the message */
    {
        {
            if ( 0 != size % CIPHER_MULTIPLE )
            {
                cipherSize += ( CIPHER_MULTIPLE - (size % CIPHER_MULTIPLE) );
            }
        }
        inBuffP1_p = (UInt8 *)getMemFromPool(cipherSize, PNULL);
        if (PNULL == inBuffP1_p )
        {
            /*
               LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_TX,
               pdcpCurrentTime_g, __LINE__, cipherSize, dataReq_p->ueIndex,
               dataReq_p->lcId, dataReq_p->count, 0,0, __func__,
               "Cannot allocate memory for ciphering");
               */
            ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
            pdcpSecOutpacket_p->errInfo = SEC_OUT_OF_MEMORY;
            return PDCP_FAIL;
        }
        if ( 0 == msgExtract(secDataTx_p->inData_p, 0, inBuffP1_p, size ) )
        {
            /*
               LOG_PDCP_MSG( PDCP_MSG_EXTRACT_FAILED, LOGFATAL, PDCP_TX,
               pdcpCurrentTime_g, __LINE__, size, dataReq_p->ueIndex,
               dataReq_p->lcId, dataReq_p->count, 0,0, __func__,
               "ZCB data cannot be extracted for deciphering");
               */
            freeMemPool(inBuffP1_p);
            inBuffP1_p = PNULL;
            pdcpSecOutpacket_p->status = SEC_STATUS_ERROR;
            return PDCP_FAIL;
        }
    }
    if (ALGO_EEA0 != secDataTx_p->secContext_p->oldCiphAlgoId)
    {
        /*
           LOG_PDCP_MSG( PDCP_DATA_TO_CIPHERING_ENGINE, LOGDEBUG, PDCP_TX,
           pdcpCurrentTime_g, __LINE__, dataReq_p->ueIndex,
           dataReq_p->lcId, dataReq_p->count,0, 0,0,__func__,"");

*/
        result = sendToCiphering( secDataTx_p->secContext_p->oldCiphAlgoId,
                secDataTx_p->secContext_p->bearer,
                /* SPR 15236 fix start */
                secDataTx_p->count,
                /* SPR 15236 fix end */
                PDCP_ENTITY_RB_RX,
                secDataTx_p->secContext_p->oldCiphKey_p,
                inBuffP1_p, PNULL,
                inBuffP1_p, cipherSize, 0
                    );
        if (PDCP_SUCCESS != result)
        {
            /*
               LOG_PDCP_MSG (PDCP_CIPHERING_FAILED, LOGFATAL, PDCP_TX,
               pdcpCurrentTime_g, __LINE__, dataReq_p->ueIndex,
               dataReq_p->lcId, dataReq_p->count, size, 0,0,__func__,"");
               */
            ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
            if (( SINGLE_SEGMENT == msgSegCnt) &&( 0 == size % CIPHER_MULTIPLE ))
            {
                /*no need to free as no memory allocation done for single segment*/
            }
            else
            {
                freeMemPool(inBuffP1_p);
                inBuffP1_p = PNULL;
            }
            pdcpSecOutpacket_p->errInfo = SEC_OUT_OF_MEMORY;
            return PDCP_FAIL;
        }
    }
    else
    {
        /* Update stats for null ciphering */
        pdcpStats.statsCiphering.numberOfPacketsCipheredUsingEIA0 += 1;
    }
    if (PNULL == msgInsert(secDataTx_p->outData_p, -1, inBuffP1_p, size) )
    {
        /*
           LOG_PDCP_MSG( PDCP_MSG_INSERT_FAILED, LOGFATAL, PDCP_TX,
           pdcpCurrentTime_g, __LINE__, size, dataReq_p->ueIndex,
           dataReq_p->lcId, dataReq_p->count, 0,0, __func__,
           "data cannot be converted bach to ZCB after ciphering ");
           */
        if (( SINGLE_SEGMENT == msgSegCnt) &&( 0 == size % CIPHER_MULTIPLE ))
        {
            /*no need to free as no memory allocation done for single segment*/
        }
        else
        {
            freeMemPool(inBuffP1_p);
            inBuffP1_p = PNULL;
        }
        pdcpSecOutpacket_p->errInfo = SEC_OUT_OF_MEMORY;
        return PDCP_FAIL;
    }
    if (( SINGLE_SEGMENT == msgSegCnt) &&( 0 == size % CIPHER_MULTIPLE ))
    {
        /*no need to free as no memory allocation done for single segment*/
    }
    else
    {
        freeMemPool(inBuffP1_p);
        inBuffP1_p = PNULL;
    }
    return result;
}

/****************************************************************************
 * Function Name  : pdcpSecPollJobRingTx
 * Inputs         : count - max number of processed packets reqd by PDCP
 * Outputs        : None.
 * Returns        : None
 * Description    : This function will dequeue the packets from output job 
 *                  ring and calls callback function.
 ****************************************************************************/
void pdcpSecPollJobRingTx( UInt32 count )
{
	LP_PdcpSecOutPacket secOutDataTx_p = PNULL;

	while ( count-- )
	{
		DEQUEUE_PDCP_SEC_OUTPUT_TX_Q(pdcpSecConfig_g,secOutDataTx_p);
		if (PNULL == secOutDataTx_p)
        {
            LOG_PDCP_MSG (PDCP_NULL_NODE_Q, LOGWARNING, PDCP_TX, pdcpCurrentTime_g,
                    __LINE__, 0,0,0,0, 0,0, __func__, "SecOutputTxQ");
            return ;
        }
		else
        {
			/* SPR 15236 fix start */
            /* Check UE Context and LC context before calling callback function
             * to avoid dangling pointer access */
            LP_PdcpUeContext ueCtx_p = PDCP_GET_UE_CONTEXT( 
                    secOutDataTx_p->ueIndex );
            if( PNULL == ueCtx_p )
            {
                secOutDataTx_p->secOutCbk = pdcpFreeTxDataFromSec; 
            }
            else if( PNULL == PDCP_GET_VALID_ENTITY_CONTEXT( ueCtx_p, 
                        secOutDataTx_p->lcId, PDCP_ENTITY_RB_TX ) )
            {
                secOutDataTx_p->secOutCbk = pdcpFreeTxDataFromSec; 
            }
			/* SPR 15236 fix end */
            LOG_PDCP_MSG( PDCP_SEC_CALL_CALLBACK_FUNCTION, LOGDEBUG, PDCP_TX,
                    pdcpCurrentTime_g, __LINE__, PDCP_ENTITY_SRB, 
                    PDCP_ENTITY_RB_TX, secOutDataTx_p->status, 
                    secOutDataTx_p->errInfo, 0,0, __func__, "");
			/* SPR 3276 changes start */
            secOutDataTx_p->secOutCbk(
					/* SPR 3276 changes end */
                    secOutDataTx_p->inData_p,
                    secOutDataTx_p->outData_p,
                    secOutDataTx_p->uaCtxHandle,
                    secOutDataTx_p->status,
                    secOutDataTx_p->errInfo);
            freeMemPool(secOutDataTx_p);
            secOutDataTx_p = PNULL;
        }
	}
}

/****************************************************************************
 * Function Name  : pdcpSecPollJobRingRx
 * Inputs         : count - number of processed packets reqd by PDCP
 * Outputs        : None.
 * Returns        : None
 * Description    : This function will dequeue the packets from output job 
 *                  ring and calls callback function.
 ****************************************************************************/
void pdcpSecPollJobRingRx( UInt32 count )
{
	LP_PdcpSecOutPacket secOutDataRx_p = PNULL;

	while ( count-- )
	{
		DEQUEUE_PDCP_SEC_OUTPUT_RX_Q(pdcpSecConfig_g,secOutDataRx_p);
		if (PNULL == secOutDataRx_p)
		{
			/*
			   LOG_PDCP_MSG (PDCP_NULL_NODE_Q, LOGWARNING, PDCP_RX, pdcpCurrentTime_g,
			   __LINE__, 0,0,0,0, 0,0, __func__, "SecOutputRxQ");
			   */
			return ;
		}
		else
		{
			/*
			   LOG_PDCP_MSG( PDCP_SEC_CALL_CALLBACK_FUNCTION, LOGDEBUG, PDCP_RX,
			   pdcpCurrentTime_g, __LINE__, PDCP_ENTITY_SRB, 
			   PDCP_ENTITY_RB_RX, secOutDataRx_p->status, 
			   secOutDataRx_p->errInfo, 0,0, __func__, "");
			   */
			/* SPR 3276 changes start */
			secOutDataRx_p->secOutCbk(
					/* SPR 3276 changes end */
					secOutDataRx_p->inData_p,
					secOutDataRx_p->outData_p,
					secOutDataRx_p->uaCtxHandle,
					secOutDataRx_p->status,
					secOutDataRx_p->errInfo);
			freeMemPool(secOutDataRx_p);
			secOutDataRx_p = PNULL;
		}
	}
}
/* SPR 15236 fix start */
/****************************************************************************
 * Function Name  : freeAllMemory
 * Inputs         : ueId, lcId, direction
 * Outputs        : None.
 * Returns        : None.
 * Description    : This API frees the memory of the keys in the context.
 ****************************************************************************/
void freeAllMemory( UInt16 ueId, UInt8 lcId, UInt8 direction )
{
	UInt16 uIdx = 0;
	UInt8 lc = 0, dir = 0;

	/* Till before current UE-Index for which memory is allocated 
	 * successfully */
	for( uIdx = 0; uIdx < ueId; uIdx++ )
	{
		for( lc = 0; lc < LTE_MAX_LC_ID; lc++ )
		{
			for( dir = 0; dir < PDCP_ENTITY_RB_BOTH; dir++ )
			{
				LP_PdcpSecContextInfo pdcpSecCtx_p =
					/*SPR 16529 fix start*/
					PDCP_SEC_CONTEXT( uIdx, lc, dir );
				/*SPR 16529 fix end*/
				/* Free Memory */
				freeMemPool( pdcpSecCtx_p->oldCiphKey_p );
				pdcpSecCtx_p->oldCiphKey_p = PNULL;
				freeMemPool( pdcpSecCtx_p->newCiphKey_p );
				pdcpSecCtx_p->newCiphKey_p = PNULL;
				freeMemPool( pdcpSecCtx_p->integrityKey_p );
				pdcpSecCtx_p->integrityKey_p = PNULL;
			}
		}
	}

	/* For ueIndex = ueId */
	for( lc = 0; lc < lcId; lc++ )
	{
		for( dir = 0; dir < PDCP_ENTITY_RB_BOTH; dir++ )
		{
			LP_PdcpSecContextInfo pdcpSecCtx_p =
				/*SPR 16529 fix start*/
				PDCP_SEC_CONTEXT( ueId, lc, dir );
			/*SPR 16529 fix end*/
			/* Free Memory */
			freeMemPool( pdcpSecCtx_p->oldCiphKey_p );
			pdcpSecCtx_p->oldCiphKey_p = PNULL;
			freeMemPool( pdcpSecCtx_p->newCiphKey_p );
			pdcpSecCtx_p->newCiphKey_p = PNULL;
			freeMemPool( pdcpSecCtx_p->integrityKey_p );
			pdcpSecCtx_p->integrityKey_p = PNULL;
		}
	}

	/* For ueIndex = ueId and LCIndex = lcId */
	for( dir = 0; dir < direction; dir++ )
	{
		LP_PdcpSecContextInfo pdcpSecCtx_p =
			/*SPR 16529 fix start*/
			PDCP_SEC_CONTEXT( ueId, lcId, dir );
		/*SPR 16529 fix end*/
		/* Free Memory */
		freeMemPool( pdcpSecCtx_p->oldCiphKey_p );
		pdcpSecCtx_p->oldCiphKey_p = PNULL;
		freeMemPool( pdcpSecCtx_p->newCiphKey_p );
		pdcpSecCtx_p->newCiphKey_p = PNULL;
		freeMemPool( pdcpSecCtx_p->integrityKey_p );
		pdcpSecCtx_p->integrityKey_p = PNULL;
	}
}
/* SPR 15236 fix end */
#endif


