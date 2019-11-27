/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: ltePdcpRxQueues.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ******************************************************************************
 *
 *  File Description : PDCP RX queues implementation. 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: ltePdcpRxQueues.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.6.2.1  2009/06/09 11:33:51  gur19140
 * *** empty log message ***
 *
 * Revision 1.6  2009/05/21 07:10:46  gur19140
 * memory leak fixes
 *
 * Revision 1.5  2009/05/20 13:38:43  gur20548
 * header file name changed
 *
 * Revision 1.4  2009/05/15 18:43:24  gur19140
 * fixes
 *
 * Revision 1.3  2009/05/15 06:56:49  gur19836
 * CR Comments Incorporated
 *
 *
 *****************************************************************************/

/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "ltePdcpLteFramework.h"

#include "ltePdcpRxQueues.h"
#include "ltePdcpErrors.h"
#include "ltePdcpContext.h"
#include "logging.h"
#include "alarm.h"


/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* Cyclomatic Complexity changes - starts here */
static inline void pdcpFreeRxQueues( LP_PdcpEnityContext ctx_p );
/* Cyclomatic Complexity changes - ends here */

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* This queue will be data interface from PDCP user to the PDCP layer. */
volatile LTE_SQUEUE pdcpRxPendingDeletionQ_g;

/* SPR 15909 fix start */
extern tickType_t pdcpCurrentTime_g;
/* SPR 15909 fix end */

extern PdcpContext pdcpContext;
#ifdef PDCP_ASYNC_INTERFACE
LTE_SQUEUE pdcpRxDecipheredQ_g;
#endif
/*SPR 21188 +*/
#if  defined(PDCP_GTPU_INTF)
/*SPR 21188*/
LTE_CIRCQUEUE pdcpGtpDataIndQ_g;
#endif

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/


/****************************************************************************
 * Functions implementation 
 ****************************************************************************/

/****************************************************************************
 * Function Name  : rxQueuesCreateMemPool 
 * Inputs         :
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code.
 * Description    : Reserve memory in pools for PDCP RX queuing purposes.
 ****************************************************************************/
/* +- SPR 17777 */
UInt16 rxQueuesCreateMemPool(void)
{
    /* Start 128 UE: Memory Changes */
    createMemPool( sizeof(PdcpDataInd), ((pdcpNumOfSupportedUsers_g *
                    pdcpNumOfSupportedLC_g * MAX_PDCP_SDU_QUEUE_SIZE * 
                    pdcpUlMemFac_g)/100));
    /* End 128 UE: Memory Changes */

    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : rxQueuesInit 
 * Inputs         :     
 * Outputs        : None.
 * Returns        : None.
 * Description    : It inits the set of PDCP RX queues which are used for
 *                  data processing on TX path.
 ****************************************************************************/
void rxQueuesInit(void)
{
#ifdef PDCP_ASYNC_INTERFACE    
    QINIT_PDCP_DECIPHERED_RX_Q();
#endif
    QINIT_PDCP_RX_PENDING_DELETION_Q();
#if  defined(PDCP_GTPU_INTF)
   QINIT_PDCP_GTP_DATA_IND_Q();
#endif
}

/****************************************************************************
 * Function Name  : rxQueuesCleanup 
 * Inputs         :     
 * Outputs        : None.
 * Returns        : None.
 * Description    : It cleanup the set of PDCP RX queues which are used 
 *                  for data processing on TX path.
 ****************************************************************************/
void rxQueuesCleanup(void)
{
#ifdef PDCP_ASYNC_INTERFACE
    LP_PdcpDataInd dataInd_p = PNULL;
#endif
    LP_PdcpRxPendingDeletionReq delReq_p = PNULL;
    LP_PdcpUeContext    ueCtx_p = PNULL;
    LP_PdcpEnityContext ctx_p   = PNULL;
    UInt16              ueIndex = 0;
    UInt8               lcId    = 0;

    /* cleanup PDCP RX queue */
    while ( ueIndex < pdcpContext.numOfActiveUe)
    {
        /*
         ** activeUeInfo consists only of active UEs, hence valid UE context
         ** will always be available. Therefore, no need to check for NULL check
         */
        /* SPR 5485 changes start */
        ueCtx_p = PDCP_GET_VALID_UE_CONTEXT(pdcpContext.activeUeInfo[ueIndex]);
        /* SPR 5485 changes end */

        for (lcId = LTE_SRB_LC_ID_LOW_BOUND; lcId <= LTE_SRB_LC_ID_HIGH_BOUND;
                lcId++)
        {  
            /* SPR 5485 changes start */
            ctx_p = PDCP_GET_VALID_ENTITY_CONTEXT( ueCtx_p, lcId, 
                    PDCP_ENTITY_RB_RX );
            /* SPR 5485 changes end */
            if ( PNULL == ctx_p )
            {
                LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_RX, LOGWARNING,
                        (PDCP_RX|PDCP_RRCOAM), pdcpCurrentTime_g, __LINE__,
                        pdcpContext.activeUeInfo[ueIndex], 
                        lcId, 0,0,
                        0,0,__func__,"");
            }
            else
            { 
                /* Cyclomatic Complexity changes - starts here */
                pdcpFreeRxQueues(ctx_p);
                /* Cyclomatic Complexity changes - ends here */
            }
        }

        lcId = 0;
        while ( lcId < ueCtx_p->numOfActiveDrbLc )
        {
            /*
             ** activeLcInfo consists only of active DRBs in an UE, hence
             ** valid PDCP context will always be available. Therefore, 
             ** no need to check for NULL check
             */
            /* SPR 5485 changes start */
            ctx_p = PDCP_GET_VALID_ENTITY_CONTEXT( ueCtx_p,
                    ueCtx_p->activeLcInfo[lcId], 
                    PDCP_ENTITY_RB_RX);
            /* SPR 5485 changes end */
            /* Cyclomatic Complexity changes - starts here */
            pdcpFreeRxQueues (ctx_p);
            /* Cyclomatic Complexity changes - ends here */
            lcId++;
        }
        ueIndex++;
    }

    do
    {
        DEQUEUE_PDCP_RX_PENDING_DELETION_Q(delReq_p);
        if ( PNULL != delReq_p ) {
            freeMemPool( delReq_p );              
        }

    } while(PNULL != delReq_p);
#ifdef PDCP_ASYNC_INTERFACE
    do
    {
        DEQUEUE_PDCP_DECIPHERED_RX_Q(dataInd_p);
        if ( PNULL != dataInd_p ) 
        {
            pdcpFreeRxDataInd(dataInd_p);
        }
    } while(PNULL != dataInd_p);
#endif
}

/* Cyclomatic Complexity changes - starts here */
/****************************************************************************
 * Function Name  : pdcpFreeRxQueues 
 * Inputs         : ctx_p - Pointer to PDCP entity context    
 * Outputs        : None.
 * Returns        : None.
 * Description    : It cleanup the PDCP RX DATA IND queue and PDCP RX STORAGE 
 *                  queue used for data processing on TX path.
 ****************************************************************************/
static inline void pdcpFreeRxQueues (LP_PdcpEnityContext ctx_p)
{
	LP_PdcpDataInd dataInd_p = PNULL;
	do
	{
		DEQUEUE_PDCP_RX_DATA_IND_Q(ctx_p, dataInd_p);
		if ( PNULL != dataInd_p )
		{
			pdcpFreeRxDataInd(dataInd_p);
		}             
	} while ( PNULL != dataInd_p );
	do
	{
		DEQUEUE_PDCP_RX_STORAGE_Q(ctx_p, dataInd_p);
		if ( PNULL != dataInd_p )
		{
			pdcpFreeRxDataInd(dataInd_p);
		}             
	} while ( PNULL != dataInd_p );
}
/* Cyclomatic Complexity changes - ends here */

/****************************************************************************
 * Function Name  : pdcpFreeRxDataInd
 * Inputs         : LP_PdcpDataReq *dataReq_p   
 * Outputs        : None.
 * Returns        : None.
 * Description    : To free LP_PdcpDataReq
 ****************************************************************************/ 
void pdcpFreeRxDataInd(LP_PdcpDataInd dataInd_p)
{
    if(PNULL == dataInd_p)
    {
        LOG_PDCP_MSG (PDCP_DATA_POINTER_NULL, LOGFATAL, PDCP_RX, pdcpCurrentTime_g,
                 __LINE__, 0,0,0,0, 0,0, __func__,"dataInd_p is NULL" );
    }
    else
    {
        if(PNULL != dataInd_p->data_p)
        {
            msgFree(dataInd_p->data_p);
            dataInd_p->data_p = PNULL;
        }

        freeMemPool(dataInd_p);
    }
}

/****************************************************************************
 * Function Name  : pdcpAllocateRxDataInd 
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  sn, size, packetForReEstablishment, headPktTS, macState
 * Outputs        : None.
 * Returns        : LP_PdcpDataInd pointer on RX data indication structure or NULL.
 * Description    : To allocate and init PdcpDataInd
 ****************************************************************************/ 
LP_PdcpDataInd pdcpAllocateRxDataInd( UInt16 ueIndex, UInt16 lcId,
        UInt16 sn, UInt16 size, UInt8 packetForReEstablishment, 
        /* SPR 15909 fix start */
        tickType_t headPktTS, UInt8 macState
        /* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        ,tickType_t packetTime
        /* SPR 15909 fix end */
#endif
        /* 32.425 Events code changes end*/
        )
{
    LP_PdcpDataInd dataInd_p = PNULL;

    /* Allocate data request structure */
    dataInd_p = (LP_PdcpDataInd)getMemFromPool( sizeof(PdcpDataInd), PNULL );
    if ( PNULL != dataInd_p )
    {
        /* Init dataInd_p. */
        dataInd_p->ueIndex = ueIndex;
        dataInd_p->lcId = lcId;
        dataInd_p->data_p = PNULL;
        dataInd_p->count = 0;
        dataInd_p->sn = sn;
        dataInd_p->size = size;
        dataInd_p->packetForReEstablishment = packetForReEstablishment;
#ifdef PDCP_ASYNC_INTERFACE
        dataInd_p->discardAfterDecompression = LTE_FALSE;
#endif
        dataInd_p->headPktTS = headPktTS;
        dataInd_p->macState  = macState;
#ifdef L2_FAST_PATH
        dataInd_p->pktProcessing = LTE_PDCP_SLOW_PATH;
#endif
        /* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        dataInd_p->pktTimeStamp = packetTime ; 
#endif
        /* 32.425 Events code changes end*/
    }
    else {
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_TX, 
                 pdcpCurrentTime_g,__LINE__, sizeof(PdcpDataInd), ueIndex,
                 lcId, 0, 0,0, __func__,"dataInd_p is NULL");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
    }

    return dataInd_p;
}
