/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: confidentiality.c,v 1.1.2.1 2010/06/14 11:48:16 gur21010 Exp $
 *
 ******************************************************************************
 *
 *  File Description : Ciphering functions for performing encryption/decryption
 *                     of messages
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "ltePdcpLteDefs.h"
#include "ltePdcpErrors.h"
#include "ltePdcpContext.h"
#include "confidentiality.h"
#include "snow3g_ca.h"
#include "aes_ca.h"
#include "logging.h"
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
/* SPR 14909 fix start */
/* SPR 14909 fix end */
/****************************************************************************
 * Function Name  : sendToCiphering
 * Inputs         : UInt16 AlgorithmId (ALGO_EEA1/ALGO_EEA2)
 *		    UInt16 lcId
 *		    UInt32 count
 *		    UInt16 direction - 0(downlink) or 1(uplink)
 *		    UInt8 *key_p - confidentiality key
 *		    void *data_p - Data buffer to perform ciphering.
 *                  UInt32 size - length of data in bytes.
 * Outputs        : UInt8 *data_p - encrypted data will be placed here
 * Returns        : SUCCESS/FAILURE
 * Description    : Encrypts blocks of data.
 ****************************************************************************/
UInt32 sendToCiphering(
    UInt16 AlgorithmId, 
    UInt16 lcId, 
    UInt32 count,
    UInt16 direction, 
    UInt8 *key_p, 
/* + SPR 18103 */
    UInt8 *inBuffP1_p,
/* - SPR 18103 */
    UInt8 *inBuffP2_p,
    UInt8 *outBuff_p,
    UInt32 sizeOfP1,
    UInt32 sizeOfP2
    )
{
    UInt32 retVal = CIPHERING_SUCCESS;

#ifdef UE_SIM_TESTING
    direction = PDCP_ENTITY_RB_TX;
#endif
    switch(AlgorithmId)
    {
        case ALGO_EEA1:
        {
            retVal = snow3g_eea1( key_p, count, lcId, direction, inBuffP1_p,
                                  inBuffP2_p,outBuff_p,sizeOfP1,sizeOfP2);
            /* Update stats */
            pdcpStats.statsCiphering.numberOfPacketsCipheredUsingEIA1 += 1;
            break;
        }
        case ALGO_EEA2:
        {
            aes_eea2( key_p, count, lcId, direction, inBuffP1_p,inBuffP2_p,
                      outBuff_p, sizeOfP1,sizeOfP2);
            /* Update stats */
            pdcpStats.statsCiphering.numberOfPacketsCipheredUsingEIA2 += 1;
            break;
        }
        default:
    	{
#ifndef PDCP_ASYNC_INTERFACE          
            LOG_PDCP_MSG( PDCP_UNKNOWN_ALGO_ID, LOGWARNING, PDCP_TX,
                     pdcpCurrentTime_g, __LINE__, AlgorithmId, lcId, 0, 0, 
                     0,0,__func__,"Applying ciphering algorithm");
#endif
	    }
    }
    /* SPR_1210_FIX_START*/
    return retVal;
    /* SPR_1210_FIX_END*/
}

/****************************************************************************
 * Function Name  : sendToDeciphering
 * Inputs         : UInt16 AlgorithmId (ALGO_EEA1/ALGO_EEA2)
 *		    UInt16 lcId
 *		    UInt32 count
 *		    UInt16 direction - 0(downlink) or 1(uplink)
 *		    UInt8 *key_p - confidentiality key
 *		    void *data_p - Data buffer to perform deciphering.
 *                  UInt32 size - length of data in bytes.
 * Outputs        : UInt8 *data_p - decrypted data will be placed here
 * Returns        : SUCCESS/FAILURE
 * Description    : Decrypts blocks of data.
 ****************************************************************************/
UInt32 sendToDeciphering(
    UInt16 AlgorithmId, 
    UInt16 lcId, 
    UInt32 count,
    UInt16 direction,
    UInt8 *key_p,
 /* + SPR 18103 */
    UInt8 *inBuffP1_p,
 /* - SPR 18103 */
    UInt8 *inBuffP2_p,
    UInt8 *outBuff_p,
    UInt32 sizeOfP1,
    UInt32 sizeOfP2
	)
{
    UInt32 retVal = CIPHERING_SUCCESS;

#if (defined UE_SIM_TESTING) || (defined PDCP_UT)
    direction = PDCP_ENTITY_RB_RX;
#endif
    switch(AlgorithmId)
    {
        case ALGO_EEA1:
        {
            retVal = snow3g_eea1( key_p,count,lcId,direction,inBuffP1_p,\
                inBuffP2_p,outBuff_p,sizeOfP1,sizeOfP2);
            /* Update statistics data */
            pdcpStats.statsCiphering.numberOfPacketsDecipheredUsingEIA1 += 1;
            break;
        }
        case ALGO_EEA2:
        {
            aes_eea2( key_p,count,lcId,direction,inBuffP1_p,inBuffP2_p,\
                outBuff_p,sizeOfP1,sizeOfP2);
            /* Update statistics data */
            pdcpStats.statsCiphering.numberOfPacketsDecipheredUsingEIA2 += 1;
            break;
        }
        default:
	    {
#ifndef PDCP_ASYNC_INTERFACE          
            LOG_PDCP_MSG( PDCP_UNKNOWN_ALGO_ID, LOGWARNING, PDCP_RX,
                     pdcpCurrentTime_g, __LINE__, AlgorithmId, lcId, 0, 0, 
                     0,0,__func__,"Applying deciphering algorithm");
#endif
        }
    }
    /* SPR_1210_FIX_START*/
    return retVal;
    /* SPR_1210_FIX_END*/
}


