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
 *  File Description : Security engine scheduler implementation. 
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
#include "ltePdcpSecEngine.h"
#include "lteThread.h"
#include "lteMisc.h"
#include "lteCommonStatsManager.h"
#include "lteMacComPorting.h"
#include "logging.h"
#include "lteLayer2CommanTypes.h"

/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static void pdcpSecurityElTxProcessing( UInt64 count );
static void pdcpSecurityElRxProcessing( UInt64 count );
/* + SPR 17073 */
static void pdcpSecurityElSrbTxProcessing( UInt64 count );
static void pdcpSecurityElSrbRxProcessing( UInt64 count );
/* - SPR 17073 */
#ifdef PDCP_ASYNC_INTR_TESTING
static void pdcpElPollSecurityEngineRx(Int);
static void pdcpElPollSecurityEngineTx(Int);
#endif

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* SPR 15909 fix start */
extern tickType_t pdcpCurrentTime_g;
/* SPR 15909 fix end */
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/****************************************************************************
 * Functions implementation 
 ****************************************************************************/
/****************************************************************************
 * Function Name  : pdcpAsyncExecutionSchedular
 * Inputs         : void * - Arguements
 * Outputs        : None
 * Returns        : void *
 * Variables      :
 * Description    : This function is initialization function for security
 *                  engine seperate thread
 ****************************************************************************/
void *pdcpAsyncExecutionSchedular( void *param )
{
    UInt64 sQcount = 0;
    InternalCellIndex internalCellIndex = 0;

#ifdef PDCP_ASYNC_INTR_TESTING
    signal_wrapper(SIGUSR1, &pdcpElPollSecurityEngineRx);
    signal_wrapper(SIGUSR2, &pdcpElPollSecurityEngineTx);
#endif
    /* Set the prority of this thread equal to PDCP thread */
#ifdef LAYER2_PHYSIM_THREAD
    threadSetRtPriority(SCHED_FIFO, 45); 
#else
    threadSetRtPriority(SCHED_FIFO, 20);
#endif
    /* Bind Security thread to different core */
    THREAD_TO_BIND_SET_CORE_NO(threadSelf(),lteLayer2CoreInfo_g.layer2ciphCoreNum);

    /* + SPR 19808 */
    /* Initialize logging framework for this thread */
    char logger_file_name[LTE_MAX_FILE_NAME_SIZE] = {0};
    UInt32 name_len = LTE_MAX_FILE_NAME_SIZE-strlen_wrapper("PDCP_ASYNC")-1;
    prepareEncoderLoggerFileName (logger_file_name, name_len, "PDCP_ASYNC", 0);
    logger_file_name[LTE_MAX_FILE_NAME_SIZE-1]= '\0';
    logger_init( LTE_L2_LOG_PDCP_ASYNC_ID, (Char8*)logger_file_name, 
            l2Input.shmSize);
    /* - SPR 19808 */
    while (1)
    {
        /* + SPR 17073 */
        sQcount = QCOUNT_PDCP_SEC_SRB_INPUT_TX_Q();
        if ( sQcount )
        {
            RDTSC_AND_CLOCK_START_FDD(PDCP_SECURITY_PROCESS_TX);
            pdcpSecurityElSrbTxProcessing( sQcount );
            RDTSC_AND_CLOCK_END_PDCP(PDCP_SECURITY_PROCESS_TX,internalCellIndex); 
        }

        sQcount = QCOUNT_PDCP_SEC_SRB_INPUT_RX_Q();
        if ( sQcount )
        {
            RDTSC_AND_CLOCK_START_FDD(PDCP_SECURITY_PROCESS_TX);
            pdcpSecurityElSrbRxProcessing( sQcount );
            RDTSC_AND_CLOCK_END_PDCP(PDCP_SECURITY_PROCESS_TX,internalCellIndex); 
        }
        /* - SPR 17073 */
        sQcount = QCOUNT_PDCP_SEC_INPUT_TX_Q();
        if ( sQcount )
        {
            RDTSC_AND_CLOCK_START_FDD(PDCP_SECURITY_PROCESS_TX);
            pdcpSecurityElTxProcessing( sQcount );
            for(internalCellIndex = 0; internalCellIndex < MAX_NUM_CELL; internalCellIndex++)
            {

                RDTSC_AND_CLOCK_END_PDCP(PDCP_SECURITY_PROCESS_TX,internalCellIndex);    
            }

        }

        sQcount = QCOUNT_PDCP_SEC_INPUT_RX_Q();
        if ( sQcount )
        {
            RDTSC_AND_CLOCK_START_FDD(PDCP_SECURITY_PROCESS_RX);
            pdcpSecurityElRxProcessing( sQcount );
            for(internalCellIndex = 0; internalCellIndex < MAX_NUM_CELL; internalCellIndex++)
            {

                RDTSC_AND_CLOCK_END_PDCP(PDCP_SECURITY_PROCESS_TX,internalCellIndex);    
            }
        }
        uSleep( 200 );
    }
}

/* + SPR 17073 */
/****************************************************************************
 * Function Name  : pdcpSecurityElSrbTxProcessing
 * Inputs         : count - Number of nodes in queue
 * Outputs        : None.
 * Returns        : None
 * Description    : EL to do SRB TX processing of the security engine.
 ****************************************************************************/
void pdcpSecurityElSrbTxProcessing( UInt64 count )
{
    LP_PdcpSecPacket secDataTx_p = PNULL;
    
    while ( count-- )
    {
        DEQUEUE_PDCP_SEC_SRB_INPUT_TX_Q(secDataTx_p);
        if (PNULL != secDataTx_p)
        {
            pdcpSecProcessPacketTx(secDataTx_p);
        }
        else
        {
            /*Nothing to process*/
            /*
            LOG_PDCP_MSG (PDCP_NULL_NODE_Q, LOGWARNING, PDCP_TX, pdcpCurrentTime_g,
                    __LINE__, 0,0,0,0, 0,0, __func__, "SecInputTxQ");
            */
            return;
        }
        secDataTx_p = PNULL;
    } 
}

/****************************************************************************
 * Function Name  : pdcpSecurityElSrbRxProcessing
 * Inputs         : count - Number of nodes in queue
 * Outputs        : None.
 * Returns        : None
 * Description    : EL to do SRB RX processing of the security engine.
 ****************************************************************************/
void pdcpSecurityElSrbRxProcessing( UInt64 count )
{
    LP_PdcpSecPacket secDataRx_p = PNULL;
    
    while ( count-- )
    {
        DEQUEUE_PDCP_SEC_SRB_INPUT_RX_Q(secDataRx_p);
        if (PNULL != secDataRx_p)
        {
            pdcpSecProcessPacketRx(secDataRx_p);
        }
        else
        {
            /*Nothing to process*/
            /*
            LOG_PDCP_MSG (PDCP_NULL_NODE_Q, LOGWARNING, PDCP_TX, pdcpCurrentTime_g,
                    __LINE__, 0,0,0,0, 0,0, __func__, "SecInputTxQ");
            */
            return;
        }
        secDataRx_p = PNULL;
    } 
}
/* - SPR 17073 */
/****************************************************************************
 * Function Name  : pdcpSecurityElTxProcessing
 * Inputs         : count - Number of nodes in queue
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS / Error code
 * Description    : EL to do TX processing of the security engine.
 ****************************************************************************/
void pdcpSecurityElTxProcessing( UInt64 count )
{
    LP_PdcpSecPacket secDataTx_p = PNULL;
    while ( count-- )
    {
        DEQUEUE_PDCP_SEC_INPUT_TX_Q(secDataTx_p);
        if (PNULL != secDataTx_p)
        {
            pdcpSecProcessPacketTx(secDataTx_p);
        }
        else
        {
            /*Nothing to process*/
            return;
        }
        /* + Coverity 69735 */
        secDataTx_p = PNULL;
        /* - Coverity 69735 */
    } 
}

/****************************************************************************
 * Function Name  : pdcpSecurityElRxProcessing
 * Inputs         : count - Number of nodes in queue
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS / Error code
 * Description    : EL to do TX processing of the security engine.
 ****************************************************************************/
void pdcpSecurityElRxProcessing( UInt64 count )
{
    LP_PdcpSecPacket secDataRx_p = PNULL;
    while ( count-- )
    {
        DEQUEUE_PDCP_SEC_INPUT_RX_Q(secDataRx_p);
        if (PNULL != secDataRx_p)
        {
            pdcpSecProcessPacketRx(secDataRx_p);
        }
        else
        {
            /*Nothing to process*/
            return;
        }
        /* + Coverity 69734 */
        secDataRx_p = PNULL;
        /* - Coverity 69734 */
    }
}

#ifdef PDCP_ASYNC_INTR_TESTING
#ifdef PDCP_ASYNC_INTR
/****************************************************************************
 * Function Name  : pdcpElSecurityEngineTxIntrHdlr
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This EL be called by the interrupt handler and will  
 *                  enqueue the data to pdcp
 ****************************************************************************/
void pdcpElSecurityEngineTxIntrHdlr(void *pkt)
{
    LP_PdcpSecOutPacket pdcpSecOutpacket_p = (LP_PdcpSecOutPacket)pkt;

    ENQUEUE_PDCP_SEC_OUTPUT_TX_Q(pdcpSecConfig_g, pdcpSecOutpacket_p);
}

/****************************************************************************
 * Function Name  : pdcpElSecurityEngineRxIntrHdlr
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This EL be called by the interrupt handler and will  
 *                  enqueue the data to pdcp
 ****************************************************************************/
void pdcpElSecurityEngineRxIntrHdlr(void *pkt)
{
    LP_PdcpSecOutPacket pdcpSecOutpacket_p = (LP_PdcpSecOutPacket)pkt;

    ENQUEUE_PDCP_SEC_OUTPUT_RX_Q(pdcpSecConfig_g, pdcpSecOutpacket_p);
}
#endif /* PDCP_ASYNC_INTR */

/****************************************************************************
 * Function Name  : pdcpElPollSecurityEngineTx
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This EL will poll the security engine to retrive back the 
 *                  processed packets by security engine.
 ****************************************************************************/
void pdcpElPollSecurityEngineTx (Int signal)
{
    UInt64 count = QCOUNT_SEC_INTR_INPUT_TX_Q();
    LP_PdcpSecOutPacket pdcpSecOutpacket_p = PNULL;
    while( count-- )
    {
        /* Dequeue the node from Input Tx queue */
        DEQUEUE_SEC_INTR_INPUT_TX_Q(pdcpSecOutpacket_p);
        if( PNULL != pdcpSecOutpacket_p )
        {
            /* Call the interrupt handler */
            pdcpElSecurityEngineTxIntrHdlr(pdcpSecOutpacket_p);
        }
        else
        {
            /*Nothing to process*/
            /*LOG_PDCP_MSG (PDCP_NULL_NODE_Q, LOGWARNING, PDCP_TX, pdcpCurrentTime_g,
                    __LINE__, 0,0,0,0, 0,0, __func__, "secIntrInputTxQ_g");*/
        }
    }
}
/****************************************************************************
 * Function Name  : pdcpElPollSecurityEngineRx
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This EL will poll the security engine to retrive back the 
 *                  processed packets by security engine.
 ****************************************************************************/
void pdcpElPollSecurityEngineRx( Int signal )
{
    UInt64 count = QCOUNT_SEC_INTR_INPUT_RX_Q();
    LP_PdcpSecOutPacket pdcpSecOutpacket_p = PNULL;
    while( count-- )
    {
        /* Dequeue the node from Input Tx queue */
        DEQUEUE_SEC_INTR_INPUT_RX_Q(pdcpSecOutpacket_p);
        if( PNULL != pdcpSecOutpacket_p )
        {
            /* Call the interrupt handler */
            pdcpElSecurityEngineRxIntrHdlr(pdcpSecOutpacket_p);
        }
        else
        {
            /*Nothing to process*/
            /*LOG_PDCP_MSG (PDCP_NULL_NODE_Q, LOGWARNING, PDCP_RX, pdcpCurrentTime_g,
                    __LINE__, 0,0,0,0, 0,0, __func__, "secIntrInputRxQ_g"); */
        }
    }
}
#endif /* PDCP_ASYNC_INTR_TESTING */
#endif
