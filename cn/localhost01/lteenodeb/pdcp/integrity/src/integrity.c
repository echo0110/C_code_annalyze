/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: integrity.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ******************************************************************************
 *
 *  File Description : Integrity functions for generating and checking of MACI 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: integrity.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.6.2.3  2009/07/15 06:11:26  gur19140
 * changes during intg cycle1 for downlink
 *
 * Revision 1.6.2.2  2009/07/10 09:07:15  gur19140
 * optimization changes for zcb
 *
 * Revision 1.6.2.1  2009/06/09 11:28:26  gur19140
 * *** empty log message ***
 *
 * Revision 1.6  2009/05/26 14:56:10  gur19140
 * *** empty log message ***
 *
 * Revision 1.5  2009/05/20 14:02:18  gur20548
 * header file name changed
 *
 * Revision 1.4  2009/05/15 09:05:26  gur19140
 * bug fixes
 *
 * Revision 1.3  2009/05/14 12:38:05  gur19836
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
#include "ltePdcpLteDefs.h"
#include "ltePdcpErrors.h"

#include "integrity.h"
#include "snow3g.h"
#include "snow3g_ia.h"
#include "aes_ia.h"
#include "ltePdcpContext.h"
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
extern UInt8 *integrityCheckMACIbaseptr_p;
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/****************************************************************************
 * Functions implementation 
 ****************************************************************************/

/****************************************************************************
 * Function Name  : integrityGenerateMACI
 * Inputs         : UInt16 AlgorithmId (ALGO_EIA1/ALGO_EIA2)
 *					UInt16 lcId
 *					UInt32 count
 *					UInt16 direction - 0(downlink) or 1(uplink)
 *					UInt8 *key_p - integrity key
 *					void *data_p - ZCB with data to calc MACI for
 * Outputs        : UInt8 *MACI_p - calculated MACI will be placed here
 * Returns        : None
 * Description    : Calculates MACI
 ****************************************************************************/
void integrityGenerateMACI(UInt16 AlgorithmId, UInt16 lcId, UInt32 count,
		UInt16 direction, UInt8 *key_p, 
		/* + SPR 18103 */
		void *data_p, 
		/* - SPR 18103 */
		UInt8 *MACI_p)
{
    /* SPR_1205_FIX_START */
#ifdef UE_SIM_TESTING
    direction = PDCP_ENTITY_RB_TX;
#endif
    /* SPR_1205_FIX_END */
#ifndef PDCP_ASYNC_INTERFACE
    LOG_PDCP_MSG( PDCP_PARAMS_TO_INTEGRITY_ENGINE, LOGDEBUG, 
            (PDCP_TX | PDCP_RRCOAM), pdcpCurrentTime_g, __LINE__, lcId, count,
            direction, AlgorithmId, 0, 0, __func__, "");
#endif
    
    switch(AlgorithmId)
    {
        case ALGO_EIA1:
        {
            snow3g_eia_ZCB(key_p, lcId, count, direction, data_p,
                           msgSize(data_p,PNULL), MACI_p);
            /* Update stats */
            pdcpStats.statsIntegrity.\
                        numberOfPacketsIntegrityProtectedUsingEIA1 += 1;
            break;
        }
        case ALGO_EIA2:
        {
            aes_eia_ZCB(key_p, lcId, count, direction, data_p,
                        msgSize(data_p,PNULL), MACI_p);
            /* Update stats */
            pdcpStats.statsIntegrity.\
                        numberOfPacketsIntegrityProtectedUsingEIA2 += 1;

            break;
        }
        default:
        {
#ifndef PDCP_ASYNC_INTERFACE
            LOG_PDCP_MSG( PDCP_UNKNOWN_ALGO_ID, LOGWARNING, PDCP_TX,
                     pdcpCurrentTime_g, __LINE__, AlgorithmId, lcId, 0, 0, 
                     0,0,__func__,"Integrity Protection");
#endif
        }
    }
#ifndef PDCP_ASYNC_INTERFACE
    LOG_PDCP_MSG( PDCP_CALC_MAC_I, LOGDEBUG, (PDCP_TX | PDCP_RRCOAM), 
            pdcpCurrentTime_g, __LINE__, MACI_p[0], MACI_p[1], MACI_p[2],
            MACI_p[3], 0, 0, __func__, "");
#endif
}

/****************************************************************************
 * Function Name  : integrityCheckMACI
 * Inputs         : UInt16 AlgorithmId (ALGO_EIA1/ALGO_EIA2)
 *			UInt16 lcId
 *			UInt32 count
 *			UInt16 direction - 0(downlink) or 1(uplink)
 *			UInt8 *key_p - integrity key
 *			void *data_p - ZCB with data to calc MACI for
 *          UInt8 * flag - to check for MACI against 0 in the SMC/SMF msg.
 * Outputs        : None
 * Returns        : PDCP_SUCCESS (MACI is valid) / PDCP_FAIL (MACI is invalid)
 * Description    : Checks if integrity MACI is valid
 ****************************************************************************/
UInt16 integrityCheckMACI(UInt16 AlgorithmId, UInt16 lcId, UInt32 count,
          UInt16 direction,UInt8 *key_p,
	  /* + SPR 18103 */
	  void *data_p
	  /* - SPR 18103 */
#ifndef UE_SIM_TESTING
		  /* SPR 8174 Changes Start */
		  ,UInt8 *flag
		  /* SPR 8174 Changes End */
#endif
		  )
{
    /* SPR  3444 changes start */
    UInt8 res_MAC_I[MACI_LEN]={0};
    /* SPR  3444 changes end */
    UInt32 len = msgSize(data_p,PNULL);
    /* SPR 8174 fix start */
#ifndef UE_SIM_TESTING
    /* COVERITY FIX 69270 START */
    UInt8 temp_res_MAC_I[MACI_LEN]={0};
    /* COVERITY FIX 69270 END */
#endif

    /* SPR_1205_FIX_START */
#if (defined UE_SIM_TESTING) || defined (PDCP_UT)
    direction = PDCP_ENTITY_RB_RX;
#endif
    /* SPR_1205_FIX_END */
#ifndef PDCP_ASYNC_INTERFACE
    LOG_PDCP_MSG( PDCP_PARAMS_TO_INTEGRITY_ENGINE, LOGDEBUG, PDCP_RX, 
            pdcpCurrentTime_g, __LINE__, lcId, count, direction, AlgorithmId,
            0, 0, __func__, "");
#endif
    
    if(len <= MACI_LEN) /*do we need to support packets with data len == 0 ??? */
    {
#ifndef PDCP_ASYNC_INTERFACE
        LOG_PDCP_MSG( PDCP_INVALID_DATA_SIZE, LOGWARNING, PDCP_RX, pdcpCurrentTime_g,
                __LINE__, len, count, 0, lcId, 0,0, __func__,
                "Integrity engine receives 0 byte data");
#endif
#ifndef UE_SIM_TESTING
        /* SPR 8174 Changes Start */
        if (LTE_TRUE == *flag)
        {
            *flag = LTE_FALSE;
        }
        /* SPR 8174 Changes End */
#endif
        return PDCP_FAIL;
    }
    switch(AlgorithmId)
    {
        case ALGO_EIA1:
        {
            snow3g_eia_ZCB(key_p, lcId, count, direction, data_p,
                           len-MACI_LEN, res_MAC_I);
            /* Update stats */
            pdcpStats.statsIntegrity.numberOfPacketsIntegrityVerifiedUsingEIA1
                += 1;
            break;
        }
        case ALGO_EIA2:
        {
            aes_eia_ZCB(key_p, lcId, count, direction, data_p,
                        len-MACI_LEN, res_MAC_I);
            /* Update stats */
            pdcpStats.statsIntegrity.numberOfPacketsIntegrityVerifiedUsingEIA2
                += 1;
            break;
        }
        default:
        {
#ifndef PDCP_ASYNC_INTERFACE
            LOG_PDCP_MSG( PDCP_UNKNOWN_ALGO_ID, LOGWARNING, PDCP_RX,
                     pdcpCurrentTime_g, __LINE__, AlgorithmId, lcId, 0, 0, 
                     0,0,__func__,"Integrity Verification");
#endif
        }
    }
    /* COVERITY FIX 69270 START */
    if( PNULL == integrityCheckMACIbaseptr_p)
    {
        return PDCP_FAIL;
    }
    else
    {
    /* COVERITY FIX 69270 END */
        /* SPR_707_FIX_START */
    /* + SPR 18103 */
        if(0 == msgExtract(data_p, -1, integrityCheckMACIbaseptr_p, MACI_LEN))
    /* - SPR 18103 */
        {
#ifndef PDCP_ASYNC_INTERFACE
            LOG_PDCP_MSG( PDCP_MSG_EXTRACT_FAILED, LOGFATAL, PDCP_RX, pdcpCurrentTime_g,
                    __LINE__, MACI_LEN, 0, lcId, count, 0,0, __func__,
                    "MAC-I cannot be extracted");
#endif
            ltePanic("msgExtract error in integrityCheckMACI");
#ifndef UE_SIM_TESTING
            /* SPR 8174 Changes Start */
            if (LTE_TRUE == *flag)
            {
                *flag = LTE_FALSE;
            }
            /* SPR 8174 Changes End */
#endif    
            return PDCP_FAIL;
        }
        /* SPR_707_FIX_END */

#ifndef PDCP_ASYNC_INTERFACE
        LOG_PDCP_MSG( PDCP_CALC_MAC_I, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g, __LINE__,
                res_MAC_I[0], res_MAC_I[1], res_MAC_I[2], res_MAC_I[3], 0, 
                0, __func__, "");

        LOG_PDCP_MSG( PDCP_RECV_MAC_I, LOGDEBUG, PDCP_RX, pdcpCurrentTime_g, __LINE__,
                integrityCheckMACIbaseptr_p[0], integrityCheckMACIbaseptr_p[1] ,
                integrityCheckMACIbaseptr_p[2], integrityCheckMACIbaseptr_p[3], 
                0,0, __func__, "");
#endif    
#ifndef UE_SIM_TESTING
        /* SPR 8174 Changes Start */
        if ( (LTE_TRUE == *flag) && (0 == memCmp(integrityCheckMACIbaseptr_p, temp_res_MAC_I, MACI_LEN)) )
        {
            /* MACI is received as 0, which indicates that Integrity application failed at UE
             * so we will send as it is response to RRC, and not send integrity failure Ind.
             * Also reset the flag to 0 so that this procedure is not repeated for any other 
             * uplink message 
             */
            *flag = LTE_FALSE;
            if(ALGO_EIA1 == AlgorithmId)
            {
                /* Decrement stats which were updated above */
                pdcpStats.statsIntegrity.numberOfPacketsIntegrityVerifiedUsingEIA1
                    -= 1;
            }
            else if(ALGO_EIA2 == AlgorithmId)
            {
                /* Decrement stats which were updated above */
                pdcpStats.statsIntegrity.numberOfPacketsIntegrityVerifiedUsingEIA2
                    -= 1;
            }
            else
            {
#ifndef PDCP_ASYNC_INTERFACE
                LOG_PDCP_MSG( PDCP_UNKNOWN_ALGO_ID, LOGWARNING, PDCP_RX,
                        pdcpCurrentTime_g, __LINE__, AlgorithmId, lcId, 0, 0, 
                        0,0,__func__,"Integrity Verification");
#endif
            }
            return PDCP_SUCCESS;
        }
        /* SPR 8174 Changes End */

        /* Flag has to be resetted after processing SMC, whether in case of
         * Success/Failure 
         */ 
        *flag = LTE_FALSE;
#endif
        if ( 0 == memCmp(integrityCheckMACIbaseptr_p, res_MAC_I, MACI_LEN) )
        {
            /*MACI is valid*/
            return PDCP_SUCCESS;
        }
        /*MACI is invalid*/
        return PDCP_FAIL;
    }
}
