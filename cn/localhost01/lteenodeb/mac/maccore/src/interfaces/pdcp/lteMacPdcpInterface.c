
/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacPdcpInterface.c,v  $
 *
 ****************************************************************************
 *
 *  File Description : The file contains the functionality for PDCP - MAC
 *                     interface.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacULSpsScheduler.h"
#include "lteMacTypes.h"
#include "lteMacSpsCommonTypes.h"
#include "lteMacPdcpInterface.h"

/*****************************************************************************
 Private Definitions
*****************************************************************************/

/*****************************************************************************
 Private Types
*****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
*****************************************************************************/

/****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/****************************************************************************
 * Function Name  : macUeSpsVoiceSilenceDetection
 * Inputs         : ueIndex - Index of UE,
 *                  voiceSilenceDetect - Flag for Silence/voice detection,
 *                  and spsState
 *                  internalCellIndex - cell id  
 * Outputs        : None
 * Returns        : None
 * Description    : This function will be used by PDCP layer to pass the voice/silence
 *                  packet information for SPS LC after processing UL packet.
 ****************************************************************************/
 void macUeSpsVoiceSilenceDetection( UInt16 ueIndex,
                                     /* +- SPR 17777 */
                                     UInt8 voiceSilenceDetect,
                                     UInt8 spsState,
                                     InternalCellIndex internalCellId) 
{
    LP_MacULSpsActReq    macSpsActReq_p   = PNULL;

    switch( voiceSilenceDetect )
    {
        case VOICE_PKT:
        {
            if( PDCP_MAC_SPS_DEACTIVATED ==  spsState ) 
            {
                /* Allocate memory for Activation queue node */
                macSpsActReq_p = (LP_MacULSpsActReq)getMemFromPool(
                            sizeof(MacULSpsActReq), PNULL);
                if ( PNULL == macSpsActReq_p  )
                {
                    LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                            getCurrentTick(), __LINE__, sizeof(MacULSpsActReq),
                            0,0,0, 0,0, __FILE__, __func__);
                    ALARM_MSG( MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID,
                              CRITICAL_ALARM);
                    ltePanic("getMemFromPool fails %s", __func__);
    		       /*KLOCKWORK_CHANGES_START_118 */
	        	    return;	
		           /*KLOCKWORK_CHANGES_STOP_118 */
                }
                /* Fill the activation queue node */
                macSpsActReq_p->ueIndex = ueIndex;
                macSpsActReq_p->cause = VOICE_PACKET; 
                /* Enqueue in ulSpsActQ_g */
                /*CA Changes start  */
                /* + Coverity Id - 32582 */
                if(!ENQUEUE_MAC_UL_SPS_Q( ulSpsActQ_gp[internalCellId], macSpsActReq_p ))
                {
                    freeMemSys(macSpsActReq_p);
                }
                /* - Coverity Id - 32582 */
                /* Update Q Stats */
                /* CA Stats Changes Start */
                LTE_MAC_QUEUE_USED( UL_SPS_ACT_Q, 
                        QCOUNT_MAC_UL_SPS_Q( ulSpsActQ_gp[internalCellId] ),
                        internalCellId);
                /* CA Stats Changes End */
                /*CA Changes end  */
            }
            break;
        }
        case SILENCE_PKT:
        {
            break;
        }
        default:
        {
            break;
        }
    }
}

/****************************************************************************
 * Function Name  : macUeSpsAverageSdu
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : The function sends average RLC SDU size  to MAC 
 ****************************************************************************/
 /* +- SPR 17777*/
void macUeSpsAverageSdu(void) 
{
}
/* +- SPR 17777*/
