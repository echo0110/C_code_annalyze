/*****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteRelayOamSap.c,v 1.1.1.1 2010/02/11 04:51:36 cm_intel Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains function definations for Packet
 *                   : Relay and OAM Interface.
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRelayOamSap.c,v $
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
#include "lteRelayAdapter.h"
#include "lteMessageQIntf.h"
#include "lteMisc.h"
#include "lteRelayOamSap.h"
#include "lteRelayCommon.h"
#include "lteRelayGtpuSap.h"

/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/

#ifdef ENDC_ENABLED
#include "lteX2uRelay.h"
#endif
/******************************************************************************
  Private Definitions
 *****************************************************************************/
/* Start of change for VARIBLE Declaration */

/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/

/* End of change for VARIBLE Declaration */

STATIC UInt32 handleGtpuSetTimerReq(relayGtpuSetTimerReq *timerReq_p);
STATIC UInt32 handleGtpuGetTimerReq(relayGtpuGetTimerReq *timerReq_p);
STATIC UInt32 handleGtpuGetMaxRetriesReq(relayGtpuGetMaxRetriesReq *retriesReq_p);
STATIC UInt32 handleGtpuSetMaxRetriesReq(relayGtpuSetMaxRetriesReq *retriesReq_p);
STATIC UInt32 handleGtpuSetTraceLvlReq(relaySetTraceLvlReq *setTraceLvlReq_p);
STATIC UInt32 handleGtpuGetTraceLvlReq(relayGetTraceLvlReq *getTraceLvlReq_p);
STATIC UInt32 handleGtpuInitTraceLvlReq(void);
STATIC UInt32 handleGtpuInitDbReq(relayGtpuInitDBReq *initDbReq_p);
/******************************************************************************
  Private Types
 *****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/
MSGPOOL MsgPool_g;
/*HSM_CHG_START*/

UInt32  gtpuHealthMonitoringTimeInterval_g;

/*HSM_CHG_END*/
/*SPR 17585 +*/
UInt8 s1UInitInd_g = LTE_FALSE;
/*SPR 17585 -*/
#define NO_OF_PACKET_RELAY_BUFFERS 5000

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
extern void initInternetSocket(LP_ConfigFileFormat configData);
    /* eMBMS Code Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
extern void initInternetSocketForEmbms(void);
#endif
    /* eMBMS Code Changes End */

extern relayConfigInfo relayConfigInfo_g;
extern LP_ConfigFileFormat configData;

/*SPR 20908 +*/
/*SPR 21545 Start*/
extern UInt8 instanceL2_g;
/*SPR 21545 End */
/*SPR 21188 +*/
#ifndef PDCP_GTPU_INTF
extern SInt32 sendL2InitIndToOAM(UInt8 *cnfBuff_p, UInt16 msgLen,SInt32 sendSocktFD_g);
#endif
/*SPR 21188 -*/
/*SPR 20908 -*/

/****************************************************************************
 * Functions implementation
 *****************************************************************************/

/****************************************************************************
 * Function Name  : processGtpuSetTimerReq
 * Inputs         : setTimerReq_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process set timer request for GTPU.
 ****************************************************************************/

UInt32 processGtpuSetTimerReq(UInt8 *setTimerReq_p, UInt16 msgLen)
{
    relayGtpuSetTimerReq gtpuSetTimerReq = {0};
    UInt16 current = 0, transId = 0;
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(msgLen)
    /* +- SPR 17777 */
    if (PNULL != setTimerReq_p)  
    {
        transId = RELAY_GTPU_GET_U16BIT(&setTimerReq_p[current]);
        current = OAM_L2_API_HDR;
/*  + Layer2 NON CA Changes */  
       transIdMappingForLM_g[LM_SET_TIMER_CNF].cellId = 
           setTimerReq_p[API_HEADER_CELL_INDEX_OFFSET];
/*  - Layer2 NON CA Changes */  
        if (TRANS_ID_DUPLICATE_REQ != transIdMappingForLM_g[LM_SET_TIMER_CNF].isValid)
        {
            gtpuSetTimerReq.timerType = setTimerReq_p[current++];
            /* Validation for time type. if it contains invalid value, no need to
            sent to GTP-U, Reject the message and send Error Report to LM */
            if (RELAY_GTPU_ECHO_RSP_TMR != gtpuSetTimerReq.timerType)
            {
                //lteLog(QVLOG_INFO,0,"validation failed => Invalid Value of IE\n");
                if (RELAY_GTPU_SUCCESS == 
                    prepareAndSendErrorReportToLM(transId,LM_SET_TIMER_CNF))
                {
                    /*This function will never return FAILURE*/
                }
                return RELAY_GTPU_SUCCESS;
            }
            gtpuSetTimerReq.timerDuration = RELAY_GTPU_GET_U32BIT(&setTimerReq_p[current]);
            current += 4;
            /* CALL handleGtpuSetTimerReq Function for further handling */
            if(RELAY_GTPU_SUCCESS != handleGtpuSetTimerReq(&gtpuSetTimerReq))
            { 
                if (RELAY_GTPU_SUCCESS == 
                    prepareAndSendErrorReportToLM(transId,LM_SET_TIMER_CNF))
                {
                    /*This function will never return FAILURE*/
                }
                return RELAY_GTPU_SUCCESS;
            }
            else
            { 
               //lteLog(QVLOG_INFO,0,"Successfully processed Set Timer Request\n"); 
               transIdMappingForLM_g[LM_SET_TIMER_CNF].transId = transId;
               transIdMappingForLM_g[LM_SET_TIMER_CNF].isValid = TRANS_ID_DUPLICATE_REQ;
            }
            return RELAY_GTPU_SUCCESS;
         }
         else
         {
            if (RELAY_GTPU_SUCCESS ==
                prepareAndSendErrorReportToLM(transId,LM_SET_TIMER_CNF))
            {
                /*This function will never return FAILURE*/
            }
        }
        return RELAY_GTPU_SUCCESS;
    }
    return RELAY_GTPU_FAILURE;
}

/****************************************************************************
 * Function Name  : processGtpuGetTimerReq
 * Inputs         : getTimerReq_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process get timer request for GTPU.
 ****************************************************************************/

UInt32 processGtpuGetTimerReq(UInt8 *getTimerReq_p, UInt16 msgLen)
{
    relayGtpuGetTimerReq gtpuTimerReq = {0};
    UInt16 current = 0, transId = 0;
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(msgLen)
    /* +- SPR 17777 */
    if (PNULL != getTimerReq_p) 
    {
        transId =  RELAY_GTPU_GET_U16BIT(&getTimerReq_p[current]);
        current = OAM_L2_API_HDR;
/*  + Layer2 NON CA Changes */  
       transIdMappingForLM_g[LM_GET_TIMER_CNF].cellId = 
           getTimerReq_p[API_HEADER_CELL_INDEX_OFFSET];
/*  - Layer2 NON CA Changes */  
        if (TRANS_ID_DUPLICATE_REQ != transIdMappingForLM_g[LM_GET_TIMER_CNF].isValid)
        {
            gtpuTimerReq.timerType = getTimerReq_p[current];
            /* Validation for time type. if it contains invalid value, no need to
               sent to GTP-U, Reject the message and send Error Report to LM */
            if (RELAY_GTPU_ECHO_RSP_TMR != gtpuTimerReq.timerType)
            { 
                if (RELAY_GTPU_SUCCESS == prepareAndSendErrorReportToLM(transId,LM_GET_TIMER_CNF))
                {
                    /* This function will never return FAILURE*/
                }
                return RELAY_GTPU_SUCCESS;
            }
            /* Call handleGtpuGetTimerReq for further processing */
            if(RELAY_GTPU_SUCCESS != handleGtpuGetTimerReq(&gtpuTimerReq))
            {
                if (RELAY_GTPU_SUCCESS ==
                    prepareAndSendErrorReportToLM(transId,LM_GET_TIMER_CNF))
                {
                   /* This function will never return FAILURE*/
                }
                return RELAY_GTPU_SUCCESS;
            }
            else
            { 
               //lteLog(QVLOG_INFO,0,"Successfully processed Get Timer Request\n");
            } 
            return RELAY_GTPU_SUCCESS;
        }
        else
        { 
            if (RELAY_GTPU_SUCCESS ==
                    prepareAndSendErrorReportToLM(transId,LM_GET_TIMER_CNF))
            {
                /* This function will never return FAILURE*/
            }
        }
        return RELAY_GTPU_SUCCESS;
    } 
    return RELAY_GTPU_FAILURE;
}
/****************************************************************************
 * Function Name  : processGtpuGetMaxRetriesReq
 * Inputs         : getMaxRetriesReq_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process get max retires request for GTPU.
 ****************************************************************************/

UInt32 processGtpuGetMaxRetriesReq(UInt8 *getMaxRetriesReq_p, UInt16 msgLen)
/* + coverity 54519 */
{
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(msgLen)
    /* +- SPR 17777 */
    relayGtpuGetMaxRetriesReq retriesReq = {0};
    UInt16 current = 0, transId = 0;
    if (PNULL != getMaxRetriesReq_p)  
    {
        transId = RELAY_GTPU_GET_U16BIT(&getMaxRetriesReq_p[current]);
        current = OAM_L2_API_HDR; 
/*  + Layer2 NON CA Changes */  
       transIdMappingForLM_g[LM_GET_MAX_RETIRES_CNF].cellId = 
           getMaxRetriesReq_p[API_HEADER_CELL_INDEX_OFFSET];
/*  - Layer2 NON CA Changes */  
        if (TRANS_ID_DUPLICATE_REQ != transIdMappingForLM_g[LM_GET_MAX_RETIRES_CNF].isValid)
        {
            retriesReq.timerType = getMaxRetriesReq_p[current];
            /* Validation for time type. if it contains invalid value, no need to
               sent to GTP-U, Reject the message and send Error Report to LM */
            if (RELAY_GTPU_ECHO_RSP_TMR != retriesReq.timerType)
            {
                //lteLog(QVLOG_INFO,0,"parsing error => Validation failed for IE\n");
                if (RELAY_GTPU_SUCCESS == 
                        prepareAndSendErrorReportToLM(transId,LM_GET_MAX_RETIRES_CNF))
                { 
                    /* This function will never return FAILURE*/
                }
                return RELAY_GTPU_SUCCESS;
            } 
            if (RELAY_GTPU_SUCCESS != handleGtpuGetMaxRetriesReq(&retriesReq))
            {   
                if (RELAY_GTPU_SUCCESS ==
                        prepareAndSendErrorReportToLM(transId,LM_GET_MAX_RETIRES_CNF))
                { 
                    /* This function will never return FAILURE*/
                }
                return RELAY_GTPU_SUCCESS;
            }
            else
            { 
                //lteLog(QVLOG_INFO,0,"Successfully processed Get Max Retries Request\n");
                transIdMappingForLM_g[LM_GET_MAX_RETIRES_CNF].transId = transId;
                transIdMappingForLM_g[LM_GET_MAX_RETIRES_CNF].isValid = TRANS_ID_DUPLICATE_REQ;
            }
            return RELAY_GTPU_SUCCESS;
        }
        else
        {
            if (RELAY_GTPU_SUCCESS ==
                    prepareAndSendErrorReportToLM(transId,LM_GET_MAX_RETIRES_CNF))
            {
                /* This function will never return FAILURE*/
            }
        }
        return RELAY_GTPU_SUCCESS;
    }
    return RELAY_GTPU_FAILURE;
}
/* - coverity 54519 */
/****************************************************************************
 * Function Name  : processGtpuSetMaxRetriesReq
 * Inputs         : setMaxRetriesReq_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process set max retries request for GTPU.
 ****************************************************************************/

UInt32 processGtpuSetMaxRetriesReq(UInt8 *setMaxRetriesReq_p, UInt16 msgLen)
{
    UInt16 current = 0, transId = 0;
    relayGtpuSetMaxRetriesReq retriesReq = {0};
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(msgLen)
    /* +- SPR 17777 */
    if (PNULL != setMaxRetriesReq_p)  
    {
        transId =  RELAY_GTPU_GET_U16BIT(&setMaxRetriesReq_p[current]);
        current = OAM_L2_API_HDR;
/*  + Layer2 NON CA Changes */  
       transIdMappingForLM_g[LM_SET_MAX_RETIRES_CNF].cellId = 
           setMaxRetriesReq_p[API_HEADER_CELL_INDEX_OFFSET];
/*  - Layer2 NON CA Changes */  
        if (TRANS_ID_DUPLICATE_REQ != transIdMappingForLM_g[LM_SET_MAX_RETIRES_CNF].isValid)
        {
            retriesReq.timerType = setMaxRetriesReq_p[current++];
            /* Validation for time type. if it contains invalid value, no need to
               sent to GTP-U, Reject the message and send Error Report to LM */
            if (RELAY_GTPU_ECHO_RSP_TMR != retriesReq.timerType)
            { 
                if (RELAY_GTPU_SUCCESS == 
                    prepareAndSendErrorReportToLM(transId,LM_SET_MAX_RETIRES_CNF))
                {
                    /* This function will never return FAILURE*/
                }
                return RELAY_GTPU_SUCCESS;
            }
            retriesReq.maxNumOfRetries 
                =  RELAY_GTPU_GET_U16BIT(&setMaxRetriesReq_p[current]);
            current += 2;
            if (RELAY_GTPU_SUCCESS != handleGtpuSetMaxRetriesReq(&retriesReq))
            { 
                if (RELAY_GTPU_SUCCESS ==
                    prepareAndSendErrorReportToLM(transId,LM_SET_MAX_RETIRES_CNF))
                {
                    /* This function will never return FAILURE*/
                }
            }
            else
            { 
               //lteLog(QVLOG_INFO,0,"Successfully processed set Max Retries Request\n");
               transIdMappingForLM_g[LM_SET_MAX_RETIRES_CNF].transId = transId;
               transIdMappingForLM_g[LM_SET_MAX_RETIRES_CNF].isValid = TRANS_ID_DUPLICATE_REQ;
            } 
            return RELAY_GTPU_SUCCESS;
        }
        else
        {
            if (RELAY_GTPU_SUCCESS ==
                prepareAndSendErrorReportToLM(transId,LM_SET_MAX_RETIRES_CNF))
            {
                /* This function will never return FAILURE*/
            }
        }
        return RELAY_GTPU_SUCCESS;
    }
    return RELAY_GTPU_FAILURE;
}
/****************************************************************************
 * Function Name  : processGtpuInitTraceLvlReq
 * Inputs         : initTraceLvlReq_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process init trace level request for GTPU.
 ****************************************************************************/

UInt32 processGtpuInitTraceLvlReq(UInt8 *initTraceLvlReq_p, UInt16 msgLen)
{
    UInt16 transId = 0;
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(msgLen)
    /* +- SPR 17777 */
    if (PNULL != initTraceLvlReq_p)  
    {
        transId =  RELAY_GTPU_GET_U16BIT(&initTraceLvlReq_p[0]);
/*  + Layer2 NON CA Changes */  
       transIdMappingForLM_g[LM_INIT_TRACE_LEVEL_CNF].cellId = 
           initTraceLvlReq_p[API_HEADER_CELL_INDEX_OFFSET];
/*  - Layer2 NON CA Changes */  
        if (TRANS_ID_DUPLICATE_REQ != transIdMappingForLM_g[LM_INIT_TRACE_LEVEL_CNF].isValid) 
        {
            if (RELAY_GTPU_SUCCESS != handleGtpuInitTraceLvlReq())
            {
                if (RELAY_GTPU_SUCCESS == prepareAndSendErrorReportToLM(transId,LM_INIT_TRACE_LEVEL_CNF))
                {
                   /* This function will never return FAILURE*/
                }
            }
            else
            {
                //lteLog(QVLOG_INFO,"Successfully processed init trace level req\n");
                transIdMappingForLM_g[LM_INIT_TRACE_LEVEL_CNF].transId = transId;
                transIdMappingForLM_g[LM_INIT_TRACE_LEVEL_CNF].isValid = TRANS_ID_DUPLICATE_REQ;
            } 
            return RELAY_GTPU_SUCCESS;
        }
        else
        {
            if (RELAY_GTPU_SUCCESS ==
                    prepareAndSendErrorReportToLM(transId,LM_INIT_TRACE_LEVEL_CNF))
            {
                 /* This function will never return FAILURE*/
            }
        }
        return RELAY_GTPU_SUCCESS;
    }
    return RELAY_GTPU_FAILURE;
}

/****************************************************************************
 * Function Name  : processGtpuGetTraceLvlReq
 * Inputs         : getTraceLvlReq_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process Get trace level request for GTPU.
 ****************************************************************************/

UInt32 processGtpuGetTraceLvlReq(UInt8 *getTraceLvlReq_p, UInt16 msgLen)
/* + coverity 54520 */
{
    relayGetTraceLvlReq getTraceLvlReq = {0};
    UInt16 current = 0, transId = 0;
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(msgLen)
    /* +- SPR 17777 */
    if (PNULL != getTraceLvlReq_p)  
    {
        transId = RELAY_GTPU_GET_U16BIT(&getTraceLvlReq_p[current]);
        current = OAM_L2_API_HDR;
/*  + Layer2 NON CA Changes */  
       transIdMappingForLM_g[LM_GET_TRACE_LEVEL_CNF].cellId = 
           getTraceLvlReq_p[API_HEADER_CELL_INDEX_OFFSET];
/*  - Layer2 NON CA Changes */  
        if (TRANS_ID_DUPLICATE_REQ != transIdMappingForLM_g[LM_GET_TRACE_LEVEL_CNF].isValid)
        {
            getTraceLvlReq.traceFlag = getTraceLvlReq_p[current++];
            /* Validation for incoming Trace. if it contains invalid value, no need
               to sent to GTP-U, Reject the message and send Error Report to LM */
            if ((RELAY_GTPU_INCOMING_TRACE != getTraceLvlReq.traceFlag) && 
                    (RELAY_GTPU_OUTGOING_TRACE != getTraceLvlReq.traceFlag) &&
                    (RELAY_GTPU_FSM_TRACE != getTraceLvlReq.traceFlag) &&
                    (RELAY_GTPU_ERROR_TRACE != getTraceLvlReq.traceFlag))
            {
                //lteLog(QVLOG_INFO,"Parameter parsing => Invalid IE Value Received\n");
                if (RELAY_GTPU_SUCCESS ==
                        prepareAndSendErrorReportToLM(transId, LM_GET_TRACE_LEVEL_CNF))
                { 
                    /* This function will never return FAILURE*/
                }
                return RELAY_GTPU_SUCCESS;
            }
            if (RELAY_GTPU_SUCCESS != handleGtpuGetTraceLvlReq(&getTraceLvlReq)) 
            {
                if (RELAY_GTPU_SUCCESS ==
                        prepareAndSendErrorReportToLM(transId, LM_GET_TRACE_LEVEL_CNF))
                {
                    /* This function will never return FAILURE*/
                }
            }
            else
            {
                //lteLog(QVLOG_INFO,"Successfully processed get trace level req\n");
                transIdMappingForLM_g[LM_GET_TRACE_LEVEL_CNF].transId = transId;
                transIdMappingForLM_g[LM_GET_TRACE_LEVEL_CNF].isValid = TRANS_ID_DUPLICATE_REQ;
            }
            return RELAY_GTPU_SUCCESS;
        }
        else
        {
            if (RELAY_GTPU_SUCCESS ==
                    prepareAndSendErrorReportToLM(transId, LM_GET_TRACE_LEVEL_CNF))
            {
                /* This function will never return FAILURE*/
            }
        }
        return RELAY_GTPU_SUCCESS;
    }
    return RELAY_GTPU_FAILURE;
}  
/* - coverity 54520 */

/****************************************************************************
 * Function Name  : processGtpuSetTraceLvlReq
 * Inputs         : setTraceLvlReq_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process set trace level request for GTPU.
 ****************************************************************************/

UInt32 processGtpuSetTraceLvlReq(UInt8 *setTraceLvlReq_p, UInt16 msgLen)
{
    relaySetTraceLvlReq setTraceLvlReq = {0};
    UInt16 current = 0, transId = 0;
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(msgLen)
    /* +- SPR 17777 */
    if (PNULL != setTraceLvlReq_p)  
    {
        transId = RELAY_GTPU_GET_U16BIT(&setTraceLvlReq_p[current]);
        current = OAM_L2_API_HDR;
/*  + Layer2 NON CA Changes */  
       transIdMappingForLM_g[LM_SET_TRACE_LEVEL_CNF].cellId = 
           setTraceLvlReq_p[API_HEADER_CELL_INDEX_OFFSET];
/*  - Layer2 NON CA Changes */  
        if (TRANS_ID_DUPLICATE_REQ != transIdMappingForLM_g[LM_SET_TRACE_LEVEL_CNF].isValid)
        {
            setTraceLvlReq.traceFlag = setTraceLvlReq_p[current++];
            /* Validation for Trace flag. if it contains invalid value, no need
              to sent to GTP-U, Reject the message and send Error Report to LM */
            if ((RELAY_GTPU_INCOMING_TRACE != setTraceLvlReq.traceFlag) && 
                (RELAY_GTPU_OUTGOING_TRACE != setTraceLvlReq.traceFlag) && 
                    (RELAY_GTPU_FSM_TRACE != setTraceLvlReq.traceFlag) &&
                        (RELAY_GTPU_ERROR_TRACE != setTraceLvlReq.traceFlag) )
            {
                //lteLog(QVLOG_INFO,"Parameter parsing => Invalid IE Value Received\n");
                /*Klockworks Warning Fix Start*/
                if (RELAY_GTPU_SUCCESS == 
                    prepareAndSendErrorReportToLM(transId, LM_SET_TRACE_LEVEL_CNF))
                {
                     /* This function will never return FAILURE*/
                }
                 /*Klockworks Warning Fix End*/
                return RELAY_GTPU_SUCCESS;
            }
            setTraceLvlReq.traceLevel = setTraceLvlReq_p[current++];
            /* Validation for incoming Trace lvl. if it contains invalid value, no
            need to sent to GTP-U, Reject the message and send Error Report to LM */
            if ((RELAY_GTPU_NO_TRACE != setTraceLvlReq.traceLevel) && 
                (RELAY_GTPU_BRIEF_TRACE != setTraceLvlReq.traceLevel) &&
                    (RELAY_GTPU_DETAILED_TRACE != setTraceLvlReq.traceLevel))
            {
                //lteLog(QVLOG_INFO,"Parameter parsing => Invalid IE Value Received\n");
                /*Klockworks Warning Fix Start*/
                if (RELAY_GTPU_SUCCESS == 
                   prepareAndSendErrorReportToLM(transId,LM_SET_TRACE_LEVEL_CNF))
                {
                    /* This function will never return FAILURE*/
                }
                return RELAY_GTPU_SUCCESS;
            }
            if (RELAY_GTPU_SUCCESS != handleGtpuSetTraceLvlReq(&setTraceLvlReq))
            {
                if (RELAY_GTPU_SUCCESS ==
                   prepareAndSendErrorReportToLM(transId,LM_SET_TRACE_LEVEL_CNF))
                {
                     /*This function will never return FAILURE*/
                }
                /*Klockworks Warning Fix End*/
            }
            else
            {
                //lteLog(QVLOG_INFO,"Successfully processed set trace level req\n");
                transIdMappingForLM_g[LM_SET_TRACE_LEVEL_CNF].transId = transId;
                transIdMappingForLM_g[LM_SET_TRACE_LEVEL_CNF].isValid = TRANS_ID_DUPLICATE_REQ;
            }
        }  
        else
        {
            /*Klockworks Warning Fix Start*/
            if (RELAY_GTPU_SUCCESS ==
                 prepareAndSendErrorReportToLM(transId,LM_SET_TRACE_LEVEL_CNF))
            {
                /* This function will never return FAILURE*/
            }
            /*Klockworks Warning Fix End*/
        }
        return RELAY_GTPU_SUCCESS;
    }
    return RELAY_GTPU_FAILURE;
}

/****************************************************************************
 * Function Name  : processGtpuInitDbReq
 * Inputs         : initDbReq_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process Init DB Req.
 ****************************************************************************/

UInt32 processGtpuInitDbReq(UInt8 *initDbReq_p, UInt16 msgLen)
/* + coverity 54521 */
{
    relayGtpuInitDBReq initDbReq;
    /*5.1.0 Warning Fix*/
    initDbReq.noRabRec.initSize = 0;
    initDbReq.noRabRec.stepSize = 0;
    initDbReq.noRabRec.maxSize = 0;
    initDbReq.noPeerRec = 0;
    initDbReq.numMyAddr4 = 0;
    initDbReq.logLevel  = 0;
    initDbReq.numMyAddr6 = 0;
    initDbReq.echoReqTimer = 0;
    initDbReq.maxRetry = 0;
    initDbReq.myAddr4.length = 0;
    initDbReq.myAddr4.value = NULL;
    initDbReq.myAddr6.length = 0;
    initDbReq.myAddr6.value = NULL;

    UInt16 current = 0, transId = 0, startVal = 0;
    /*IPV6_CHANGE - START*/UInt8 count = 0,len = 0;/*IPV6_CHANGE - END*/

    if (PNULL != initDbReq_p)  
    {
        transId = RELAY_GTPU_GET_U16BIT(&initDbReq_p[current]);
        current = OAM_L2_API_HDR;
       transIdMappingForLM_g[LM_INITDB_CNF].cellId = 
           initDbReq_p[API_HEADER_CELL_INDEX_OFFSET];

        if (TRANS_ID_DUPLICATE_REQ != transIdMappingForLM_g[LM_INITDB_CNF].isValid)
        {
            initDbReq.noRabRec.initSize 
                = RELAY_GTPU_GET_U32BIT(&initDbReq_p[current]);
            current += 4;
            initDbReq.noRabRec.stepSize  
                = RELAY_GTPU_GET_U32BIT(&initDbReq_p[current]);
            current += 4;
            initDbReq.noRabRec.maxSize 
                = RELAY_GTPU_GET_U32BIT(&initDbReq_p[current]);
            current += 4;
            initDbReq.noPeerRec 
                = RELAY_GTPU_GET_U16BIT(&initDbReq_p[current]);
            current += 2;
            initDbReq.logLevel = initDbReq_p[current++];
            /*initDbReq.numMyAddr 
              = RELAY_GTPU_GET_U16BIT(&initDbReq_p[current]);
              current +=2;*/
            /** SPR 7881 Changes Start **/
            initDbReq.numMyAddr4 = initDbReq_p[current++];
            initDbReq.numMyAddr6 = initDbReq_p[current++];
            /** SPR 7881 Changes End **/
            /*IPV6_CHANGE - START*/
            for(count=0; count<(initDbReq.numMyAddr4+initDbReq.numMyAddr6); count++)
            {
                len = RELAY_GTPU_GET_U16BIT(&initDbReq_p[current]);
                current += 2;
                if (RELAY_GTPU_IPv4_HDR == len)
                {
                    initDbReq.myAddr4.length = len;
                    initDbReq.myAddr4.value =
                        qvAlloc(initDbReq.myAddr4.length, PNULL);
                    if (PNULL == initDbReq.myAddr4.value)
                    {
                        /*SPR fix 2030 Start */
                        /* +- SPR 17777 */
                        LOG_GTPU_MSG(L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                                relayGtpuCurrentTick_g, __LINE__, 
                                initDbReq.myAddr4.length,msgLen,0,0,0,0, __func__,
                                "PacketRelay_INITDB_REQ" );
                        /*SPR fix 2030 End */

                        return RELAY_GTPU_FAILURE;
                    }
                    else
                    {
                        for (startVal = 0; startVal <initDbReq.myAddr4.length;startVal++)
                        { 
                            /*SPR #2431 fix Start*/ 
                            myAddr4[startVal] = initDbReq.myAddr4.value[startVal] = initDbReq_p[current++];
                            /*SPR #2431 fix End*/ 
                        }
                        myAddrType |= IPV4_ADDR_PRESENT;
                    }
                }
                else if(RELAY_GTPU_IPv6_HDR == len)
                {
                    initDbReq.myAddr6.length = len;
                    initDbReq.myAddr6.value =
                        qvAlloc(initDbReq.myAddr6.length, PNULL);
                    if (PNULL == initDbReq.myAddr6.value)
                    {
                        /*SPR fix 2030 Start */
                        LOG_GTPU_MSG(L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                                relayGtpuCurrentTick_g, __LINE__, 
                                initDbReq.myAddr6.length, 0,0,0,0,0, __func__,
                                "PacketRelay_INITDB_REQ" );
                        /*SPR fix 2030 End */

                        return RELAY_GTPU_FAILURE;
                    }
                    else
                    {
                        for (startVal = 0; startVal <initDbReq.myAddr6.length;startVal++)
                        { 
                            /*SPR #2431 fix Start*/ 
                            myAddr6[startVal] = initDbReq.myAddr6.value[startVal] = initDbReq_p[current++];
                            /*SPR #2431 fix End*/ 
                        }
                        myAddrType |= IPV6_ADDR_PRESENT;
                    }
                }
                else
                {
                    /*SPR fix 2030 Start */
                    LOG_GTPU_MSG(PR_GTPU_INV_IE_RCVD_FROM_OAM, LOGWARNING, PR_GTPU_OAM,
                            relayGtpuCurrentTick_g, __LINE__, 
                            0, 0,0,0,0,0, __func__,
                            "" );
                    /*SPR fix 2030 End */
                    /*Klockworks Warning Fix Start*/
                    if (RELAY_GTPU_SUCCESS ==
                            prepareAndSendErrorReportToLM(transId, LM_INITDB_CNF))
                    {
                        /* This function will never return FAILURE*/
                    }
                    /*SPR 17585 +*/
                    s1UInitInd_g = LTE_TRUE;
                    /*SPR 17585 -*/
                    return RELAY_GTPU_SUCCESS;
                }
            }
            /*IPV6_CHANGE - END*/
            /*+SPR #3456*/
            /* SPR 10597 Changes Start */
            initDbReq.echoReqTimer 
                = RELAY_GTPU_GET_U32BIT(&initDbReq_p[current]);
            current += 4;
            /* SPR 10597 Changes End */
            initDbReq.maxRetry = initDbReq_p[current++];
            /*-SPR #3456*/
            /*HSM_CHG_START*/
            gtpuHealthMonitoringTimeInterval_g = RELAY_GTPU_GET_U16BIT(&initDbReq_p[current]);
            gtpuHealthMonitoringTimeInterval_g = gtpuHealthMonitoringTimeInterval_g * 1000;
            if(0 == gtpuHealthMonitoringTimeInterval_g)
            {
                LOG_GTPU_MSG(HSM_OAM_GTPU_INIT_FAILURE, LOGFATAL, L2_SYS_FAIL,
                        relayGtpuCurrentTick_g,gtpuHealthMonitoringTimeInterval_g,
                        DEFAULT_INT_VALUE, DEFAULT_INT_VALUE,
                        DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                        DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                        __func__,"GTPU OAM Health Status Init Failure");

                return RELAY_GTPU_FAILURE;

            }

#ifdef ENDC_ENABLED
			current += 3; /*maxMspToBeBuffered -> Not present in API DOC*/

            initDbReq.dataForwardingGuardTimerInterval = RELAY_GTPU_GET_U16BIT(&initDbReq_p[current]);
			current += 2;
			
            /*thresholds for triggering DL DATA DELIVERY STATUS*/
			initDbReq.x2uDelStatusThreshold.x2uDeliveryStatusTriggerTimer = 
				                  RELAY_GTPU_GET_U16BIT(&initDbReq_p[current]);
			current += 2;
			
			initDbReq.x2uDelStatusThreshold.x2uDeliveryStatusTriggerPktCnt = 
				                  RELAY_GTPU_GET_U16BIT(&initDbReq_p[current]);
			current += 2;
			
			initDbReq.x2uDelStatusThreshold.x2uDeliveryStatusTriggerPktSize = 
				                  RELAY_GTPU_GET_U16BIT(&initDbReq_p[current]);
            current+=2;
	        initDbReq.isGtpuNRHeaderEnabled = initDbReq_p[current++];
			LOG_GTPUPR_DEBUG(PR_GTPU_OAM, 
				       "INIT_DB_REQ DataFwdTimer [%u] x2uDelStatusTimer [%u]  "
				       " x2uDelStatusPktCnt [%u] x2uDelStatusPktSize [%u] ",
				       initDbReq.dataForwardingGuardTimerInterval,
				       initDbReq.x2uDelStatusThreshold.x2uDeliveryStatusTriggerTimer,
				       initDbReq.x2uDelStatusThreshold.x2uDeliveryStatusTriggerPktCnt,
				       initDbReq.x2uDelStatusThreshold.x2uDeliveryStatusTriggerPktSize);
#endif

            /*HSM_CHG_END*/
            if (RELAY_GTPU_SUCCESS != handleGtpuInitDbReq(&initDbReq))
            {

               	/*SPR fix 2030 Start */
               	LOG_GTPU_MSG(PR_GTPU_API_PROCESSING_FAILED, LOGWARNING, 
                       	PR_GTPU_OAM,
                       	relayGtpuCurrentTick_g, __LINE__, 
                       	0,0,0,0,0,0, __func__,
                       	"LM_INITDB_REQ" );
               	/*SPR fix 2030 End */
               
               	/*Klockworks Warning Fix Start*/
               	if (RELAY_GTPU_SUCCESS ==
                       	prepareAndSendErrorReportToLM(transId, LM_INITDB_CNF))
             	{
                   	/*This function will always return RELAY_GTPU_SUCCESS*/
               	}
               	/*Klockworks Warning Fix End*/
            }
            else
            {
               	transIdMappingForLM_g[LM_INITDB_CNF].transId = transId;
               	transIdMappingForLM_g[LM_INITDB_CNF].isValid = TRANS_ID_DUPLICATE_REQ;
            }
            /*SPR #2700 fix Start*/ 
            if (initDbReq.myAddr4.value)
                /* SPR 22402 Fix Start */
                freeMemPool(initDbReq.myAddr4.value);
                /* SPR 22402 Fix End */
            if (initDbReq.myAddr6.value)
                /* SPR 22402 Fix Start */
                freeMemPool(initDbReq.myAddr6.value);
                /* SPR 22402 Fix End */
            /*SPR #2700 fix End*/ 
        }
        else
        {
            /*Klockworks Warning Fix Start*/
            if (RELAY_GTPU_SUCCESS ==
                    prepareAndSendErrorReportToLM(transId, LM_INITDB_CNF))
            {
                /*This function will always return RELAY_GTPU_SUCCESS*/
            }
            /*Klockworks Warning Fix End*/
        }
        /*SPR 17585 +*/
        s1UInitInd_g = LTE_TRUE;
        /*SPR 17585 -*/
        return RELAY_GTPU_SUCCESS;
    }  
    return RELAY_GTPU_FAILURE; 
}
/* - coverity 54521 */

/****************************************************************************
 * Function Name  : processGtpuInitDbCnf
 * Inputs         : initDbCnf_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process init db cnf.
 ****************************************************************************/

UInt32 processGtpuInitDbCnf(UInt8 *initDbCnf_p, UInt16 msgLen)
{
    UInt8 * gtpuInitDbCnf_p = PNULL;
    UInt16 current = API_HDR, index = CSPL_HDR;
    UInt16 transId = 0;
   
    gtpuInitDbCnf_p = qvMsgAllocBySize (MAX_API_LEN + OAM_L2_API_HDR);
    if (PNULL == gtpuInitDbCnf_p )
    {
        return RELAY_GTPU_FAILURE;
    }

    /* SPR 11109 Fix start */
    memSet (gtpuInitDbCnf_p, 0, (MAX_API_LEN + OAM_L2_API_HDR));
    /* SPR 11109 Fix end */

    if (PNULL != initDbCnf_p)
    {
        /* + Layer2 NON CA Changes */
        gtpuInitDbCnf_p[API_HEADER_CELL_INDEX_OFFSET] = 
            transIdMappingForLM_g[LM_INITDB_CNF].cellId;
        /* - Layer2 NON CA Changes */
        gtpuInitDbCnf_p[current++] = initDbCnf_p[index++];
        transId = transIdMappingForLM_g[LM_INITDB_CNF].transId;
        transIdMappingForLM_g[LM_INITDB_CNF].isValid = TRANS_ID_NO_REQ;
        /* Call function pointer to send the confirmation message to LM */
        if (RELAY_GTPU_SUCCESS != 
                sendConfirmationRsp[LM_INITDB_CNF]
                (gtpuInitDbCnf_p,transId,current))
        {
            /*SPR 2030 fix Start*/ 
            /* +- SPR 17777 */
            LOG_GTPU_MSG(PR_SEND_CNF_TO_OAM_FAIL,LOGWARNING, PR_GTPU_OAM,
                    relayGtpuCurrentTick_g, __LINE__,
                    msgLen,0,0,0, 0,0, __func__,"INITDB_CNF"); 
            /*SPR 2030 fix End*/ 
            /* SPR 22402 Fix Start */
            msgFree(gtpuInitDbCnf_p);
            /* SPR 22402 Fix End */
            return RELAY_GTPU_FAILURE;
        }
        /* SPR 22402 Fix Start */
        msgFree(gtpuInitDbCnf_p);
        /* SPR 22402 Fix End */
        return RELAY_GTPU_SUCCESS;
    } 
    /* SPR 22402 Fix Start */
    msgFree(gtpuInitDbCnf_p);
    /* SPR 22402 Fix End */
    /* - COVERITY 11253 */
    return RELAY_GTPU_FAILURE;
}
/****************************************************************************
 * Function Name  : processGtpuSetTimerCnf
 * Inputs         : setTimerCnf_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process set timer cnf.
 ****************************************************************************/

UInt32 processGtpuSetTimerCnf(UInt8 *setTimerCnf_p, UInt16 msgLen)
{
    /* + COVERITY 11256 */
    UInt8 * gtpuSetTimerCnf_p = PNULL ;
    UInt16 current = API_HDR, transId = 0, index = CSPL_HDR;

    gtpuSetTimerCnf_p = qvMsgAllocBySize (MAX_API_LEN + OAM_L2_API_HDR);
    if (PNULL == gtpuSetTimerCnf_p )
    {
        return RELAY_GTPU_FAILURE;
    }
 
    if (PNULL != setTimerCnf_p)
    {
        /* + Layer2 NON CA Changes */
        gtpuSetTimerCnf_p[API_HEADER_CELL_INDEX_OFFSET] = 
            transIdMappingForLM_g[LM_SET_TIMER_CNF].cellId;
        /* - Layer2 NON CA Changes */
        gtpuSetTimerCnf_p[current++] = setTimerCnf_p[index++];
        gtpuSetTimerCnf_p[current++] = setTimerCnf_p[index++];   
        transId = transIdMappingForLM_g[LM_SET_TIMER_CNF].transId;
        transIdMappingForLM_g[LM_SET_TIMER_CNF].isValid = TRANS_ID_NO_REQ; 
        /* Call function pointer to send the confirmation message to LM */
        if (RELAY_GTPU_SUCCESS != 
                sendConfirmationRsp[LM_SET_TIMER_CNF]
                (gtpuSetTimerCnf_p,transId,current))
        {
            /*SPR 2030 fix Start*/ 
            /* +- SPR 17777 */
            LOG_GTPU_MSG(PR_SEND_CNF_TO_OAM_FAIL,LOGWARNING, PR_GTPU_OAM,
                    relayGtpuCurrentTick_g, __LINE__,
                    msgLen,0,0,0, 0,0, __func__,"SET_TIMER_CNF"); 
            /*SPR 2030 fix End*/ 
            /* SPR 22402 Fix Start */
            msgFree(gtpuSetTimerCnf_p);
            /* SPR 22402 Fix End */
            return RELAY_GTPU_FAILURE;
        }
        /* SPR 22402 Fix Start */
        msgFree(gtpuSetTimerCnf_p);
        /* SPR 22402 Fix End */
        return RELAY_GTPU_SUCCESS;
    }
    /* SPR 22402 Fix Start */
     msgFree(gtpuSetTimerCnf_p);
     /* SPR 22402 Fix End */
    /* - COVERITY 11256 */
    return RELAY_GTPU_FAILURE;
}
/****************************************************************************
 * Function Name  : processGtpuGetTimerCnf
 * Inputs         : getTimerCnf_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process get timer cnf.
 ****************************************************************************/

UInt32 processGtpuGetTimerCnf(UInt8 *getTimerCnf_p, UInt16 msgLen)
{
    /* + Coverity 11251 */
    UInt8 * gtpuGetTimerCnf_p = PNULL ;
    UInt16 current = API_HDR,index = CSPL_HDR,transId = 0;
    UInt8 result = 0;
    UInt32 timerDuration = 0;
  
    gtpuGetTimerCnf_p = qvMsgAllocBySize (MAX_API_LEN + OAM_L2_API_HDR);
    if (PNULL == gtpuGetTimerCnf_p )
    {
        return RELAY_GTPU_FAILURE;
    }
  
    if (PNULL != getTimerCnf_p)
    {
        /* + Layer2 NON CA Changes */
        gtpuGetTimerCnf_p[API_HEADER_CELL_INDEX_OFFSET] = 
            transIdMappingForLM_g[LM_GET_TIMER_CNF].cellId;
        /* - Layer2 NON CA Changes */
        result = gtpuGetTimerCnf_p[current++] = getTimerCnf_p[index++];
        gtpuGetTimerCnf_p[current++] = getTimerCnf_p[index++];
        if (!result) {
            timerDuration = RELAY_GTPU_GET_U32BIT(&getTimerCnf_p[index]);
            RELAY_GTPU_SET_U32BIT(&gtpuGetTimerCnf_p[current],timerDuration);
            current +=4;
        }
        transId = transIdMappingForLM_g[LM_GET_TIMER_CNF].transId;
        transIdMappingForLM_g[LM_GET_TIMER_CNF].isValid = TRANS_ID_NO_REQ;
        /* Call function pointer to send the confirmation message to LM */
        if (RELAY_GTPU_SUCCESS !=
                sendConfirmationRsp[LM_GET_TIMER_CNF]
                (gtpuGetTimerCnf_p,transId,current))
        {
            /*SPR 2030 fix Start*/ 
            /* +- SPR 17777 */
            LOG_GTPU_MSG(PR_SEND_CNF_TO_OAM_FAIL,LOGWARNING, PR_GTPU_OAM,
                    relayGtpuCurrentTick_g, __LINE__,
                    msgLen,0,0,0, 0,0, __func__,"GET_TIMER_CNF"); 
            /*SPR 2030 fix End*/ 
            /* SPR 22402 Fix Start */
            msgFree(gtpuGetTimerCnf_p);
            /* SPR 22402 Fix End */
            return RELAY_GTPU_FAILURE;
        }
        /* SPR 22402 Fix Start */
        msgFree(gtpuGetTimerCnf_p);
        /* SPR 22402 Fix End */
        return RELAY_GTPU_SUCCESS;
    } 
    /* SPR 22402 Fix Start */
    msgFree(gtpuGetTimerCnf_p);
    /* SPR 22402 Fix End */
    /* + Coverity 11251 */
    return RELAY_GTPU_FAILURE;
}
/****************************************************************************
 * Function Name  : processGtpuGetMaxRetriesCnf
 * Inputs         : getMaxRetriesCnf_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process get max retires cnf.
 ****************************************************************************/

UInt32 processGtpuGetMaxRetriesCnf(UInt8 *getMaxRetriesCnf_p, UInt16 msgLen)
{
    /* + COVERITY 11250 */
    UInt8 * gtpuGetMaxRetriesCnf_p = PNULL;
    UInt16 current = API_HDR, transId = 0, index = CSPL_HDR;
    UInt8  cause = 0;

    gtpuGetMaxRetriesCnf_p = qvMsgAllocBySize (MAX_API_LEN + OAM_L2_API_HDR);
    if (PNULL == gtpuGetMaxRetriesCnf_p )
    {
        return RELAY_GTPU_FAILURE;
    }/* Coverity 55017 Fix*/
    else
    { 
 /* + Layer2 NON CA Changes */
        gtpuGetMaxRetriesCnf_p[API_HEADER_CELL_INDEX_OFFSET] = 
            transIdMappingForLM_g[LM_GET_MAX_RETIRES_CNF].cellId;
 /* - Layer2 NON CA Changes */
    //lteLog(QVLOG_INFO,"Handling processGtpuGetMaxRetriesCnf\n");
        cause = gtpuGetMaxRetriesCnf_p[current++] = getMaxRetriesCnf_p[index++];
        gtpuGetMaxRetriesCnf_p[current++] = getMaxRetriesCnf_p[index++];
        if (!cause)
        {
            gtpuGetMaxRetriesCnf_p[current++] = getMaxRetriesCnf_p[index++];
            gtpuGetMaxRetriesCnf_p[current++] = getMaxRetriesCnf_p[index++];
        }
        transId = transIdMappingForLM_g[LM_GET_MAX_RETIRES_CNF].transId;
        transIdMappingForLM_g[LM_GET_MAX_RETIRES_CNF].isValid = TRANS_ID_NO_REQ;
        /* Call function pointer to send the confirmation message to LM */
        if (RELAY_GTPU_SUCCESS !=
            sendConfirmationRsp[LM_GET_MAX_RETIRES_CNF]
                (gtpuGetMaxRetriesCnf_p,transId,current))
        {
           /*SPR 2030 fix Start*/ 
            /* +- SPR 17777 */
           LOG_GTPU_MSG(PR_SEND_CNF_TO_OAM_FAIL,LOGWARNING, PR_GTPU_OAM,
                   relayGtpuCurrentTick_g, __LINE__,
                   msgLen,0,0,0, 0,0, __func__,"GET_MAX_RETRY_CNF"); 
           /*SPR 2030 fix End*/ 
           /* SPR 22402 Fix Start */
            msgFree(gtpuGetMaxRetriesCnf_p);
            /* SPR 22402 Fix End */
            return RELAY_GTPU_FAILURE;
        }
        /* SPR 22402 Fix Start */
        msgFree(gtpuGetMaxRetriesCnf_p);
        /* SPR 22402 Fix End */
        return RELAY_GTPU_SUCCESS;
    }
    /* Coverity 55017 Fix*/
    /* - COVERITY 11250 */
}
/****************************************************************************
 * Function Name  : processGtpuSetMaxRetriesCnf
  Inputs         : setMaxRetriesCnf_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process set max retries cnf.
 ****************************************************************************/

UInt32 processGtpuSetMaxRetriesCnf(UInt8 *setMaxRetriesCnf_p, UInt16 msgLen)
{
    /* + COVERITY 11255 */
    UInt8 * setMaxRetriesCnf = PNULL ;
    UInt16 current = API_HDR,index = CSPL_HDR, transId = 0;
  
    setMaxRetriesCnf = qvMsgAllocBySize (MAX_API_LEN + OAM_L2_API_HDR);
    if (PNULL == setMaxRetriesCnf )
    {
        return RELAY_GTPU_FAILURE;
    }
    /*coverity-530-55017*/
    if (PNULL != setMaxRetriesCnf_p)
    /*coverity-530-55017*/
    {
        /* + Layer2 NON CA Changes */
        setMaxRetriesCnf[API_HEADER_CELL_INDEX_OFFSET] = 
            transIdMappingForLM_g[LM_SET_MAX_RETIRES_CNF].cellId;
        /* - Layer2 NON CA Changes */
        setMaxRetriesCnf[current++] = setMaxRetriesCnf_p[index++];
        setMaxRetriesCnf[current++] = setMaxRetriesCnf_p[index++];
        transId = transIdMappingForLM_g[LM_SET_MAX_RETIRES_CNF].transId;
        transIdMappingForLM_g[LM_SET_MAX_RETIRES_CNF].isValid = TRANS_ID_NO_REQ;
        /* Call function pointer to send the confirmation message to LM */
        if (RELAY_GTPU_SUCCESS !=
                sendConfirmationRsp[LM_SET_MAX_RETIRES_CNF]
                (setMaxRetriesCnf,transId,current))
        {
            /*SPR 2030 fix Start*/ 
            /* +- SPR 17777 */
            LOG_GTPU_MSG(PR_SEND_CNF_TO_OAM_FAIL,LOGWARNING, PR_GTPU_OAM,
                    relayGtpuCurrentTick_g, __LINE__,
                    msgLen,0,0,0, 0,0, __func__,"SET_MAX_RETRY_CNF"); 
            /*SPR 2030 fix End*/ 
            /* SPR 22402 Fix Start */
            msgFree(setMaxRetriesCnf);
            /* SPR 22402 Fix End */
            return RELAY_GTPU_FAILURE;
        }
        /* SPR 22402 Fix Start */
        msgFree(setMaxRetriesCnf);
        /* SPR 22402 Fix End */
        return RELAY_GTPU_SUCCESS;
    }
    /* SPR 22402 Fix Start */
    msgFree(setMaxRetriesCnf);
    /* SPR 22402 Fix End */
    /* - COVERITY 11255 */
    return RELAY_GTPU_FAILURE;

}
/****************************************************************************
 * Function Name  : processGtpuInitTraceLvlCnf
 * Inputs         : initTraceLvlCnf_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process init trace level cnf.
 ****************************************************************************/

UInt32 processGtpuInitTraceLvlCnf(UInt8 *initTraceLvlCnf_p, UInt16 msgLen)
{ 
    /* + COVERITY 11254 */
    UInt8 * gtpuInitTraceLvlCnf_p = PNULL;
    UInt16 current = API_HDR,index = CSPL_HDR,transId = 0;

    gtpuInitTraceLvlCnf_p = qvMsgAllocBySize (MAX_API_LEN + OAM_L2_API_HDR);
    if (PNULL == gtpuInitTraceLvlCnf_p )
    {
        return RELAY_GTPU_FAILURE;
    }

    if (PNULL != initTraceLvlCnf_p)
    {
        /* + Layer2 NON CA Changes */
        gtpuInitTraceLvlCnf_p[API_HEADER_CELL_INDEX_OFFSET] = 
            transIdMappingForLM_g[LM_INIT_TRACE_LEVEL_CNF].cellId;
        /* - Layer2 NON CA Changes */
        gtpuInitTraceLvlCnf_p[current++] = initTraceLvlCnf_p[index++]; 
        transId = transIdMappingForLM_g[LM_INIT_TRACE_LEVEL_CNF].transId;
        transIdMappingForLM_g[LM_INIT_TRACE_LEVEL_CNF].isValid = TRANS_ID_NO_REQ;
        /* Call function pointer to send the confirmation message to LM */
        if (RELAY_GTPU_SUCCESS !=
            sendConfirmationRsp[LM_INIT_TRACE_LEVEL_CNF]
                (gtpuInitTraceLvlCnf_p,transId,current))
        {
           /*SPR 2030 fix Start*/ 
            /* +- SPR 17777 */
           LOG_GTPU_MSG(PR_SEND_CNF_TO_OAM_FAIL,LOGWARNING, PR_GTPU_OAM,
                   relayGtpuCurrentTick_g, __LINE__,
                   msgLen,0,0,0, 0,0, __func__,"INIT_TRACE_LEVEL_CNF"); 
           /*SPR 2030 fix End*/ 
           /* SPR 22402 Fix Start */
            msgFree(gtpuInitTraceLvlCnf_p);
            /* SPR 22402 Fix End */
            return RELAY_GTPU_FAILURE;
        }
        /* SPR 22402 Fix Start */
        msgFree(gtpuInitTraceLvlCnf_p);
        /* SPR 22402 Fix End */
        return RELAY_GTPU_SUCCESS;
    }
    /* SPR 22402 Fix Start */
     msgFree(gtpuInitTraceLvlCnf_p);
     /* SPR 22402 Fix End */
    /* - COVERITY 11254 */
    return RELAY_GTPU_FAILURE;


}
/****************************************************************************
 * Function Name  : processGtpuSetTraceLvlCnf
 * Inputs         : setTraceLvlCnf_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCES/RELAY_GTPU_FAILURE
 * Description    : This funtion process set trace level cnf.
 ****************************************************************************/

UInt32 processGtpuSetTraceLvlCnf(UInt8 *setTraceLvlCnf_p, UInt16 msgLen)
{
    /* + COVERITY 11257 */
    UInt8 * gtpuSetTraceLvlCnf_p = PNULL;
    UInt16 current = API_HDR,index = CSPL_HDR, transId = 0;
     
    gtpuSetTraceLvlCnf_p = qvMsgAllocBySize (MAX_API_LEN + OAM_L2_API_HDR);
    if (PNULL == gtpuSetTraceLvlCnf_p )
    {
        return RELAY_GTPU_FAILURE;
    }

    if (PNULL != setTraceLvlCnf_p)
    {
        /* + Layer2 NON CA Changes */
        gtpuSetTraceLvlCnf_p[API_HEADER_CELL_INDEX_OFFSET] = 
            transIdMappingForLM_g[LM_SET_TRACE_LEVEL_CNF].cellId;
        /* - Layer2 NON CA Changes */
        gtpuSetTraceLvlCnf_p[current++] = setTraceLvlCnf_p[index++];
        gtpuSetTraceLvlCnf_p[current++] = setTraceLvlCnf_p[index++];
        transId = transIdMappingForLM_g[LM_SET_TRACE_LEVEL_CNF].transId;
        transIdMappingForLM_g[LM_SET_TRACE_LEVEL_CNF].isValid = TRANS_ID_NO_REQ;
        /* Call function pointer to send the confirmation message to LM */
        if (RELAY_GTPU_SUCCESS !=
                sendConfirmationRsp[LM_SET_TRACE_LEVEL_CNF]
                (gtpuSetTraceLvlCnf_p,transId,current))
        {
            /*SPR 2030 fix Start*/ 
            /* +- SPR 17777 */
            LOG_GTPU_MSG(PR_SEND_CNF_TO_OAM_FAIL,LOGWARNING, PR_GTPU_OAM,
                    relayGtpuCurrentTick_g, __LINE__,
                    msgLen,0,0,0, 0,0, __func__,"SET_TRACE_LEVEL_CNF"); 
            /*SPR 2030 fix End*/ 
            /* SPR 22402 Fix Start */
            msgFree(gtpuSetTraceLvlCnf_p);
            /* SPR 22402 Fix End */
            return RELAY_GTPU_FAILURE;
        }
        /* SPR 22402 Fix Start */
        msgFree(gtpuSetTraceLvlCnf_p);
        /* SPR 22402 Fix End */
        return RELAY_GTPU_SUCCESS;
    }
    /* SPR 22402 Fix Start */
     msgFree(gtpuSetTraceLvlCnf_p);
     /* SPR 22402 Fix End */
    /* - COVERITY 11257 */
    return RELAY_GTPU_FAILURE;
}

/****************************************************************************
 * Function Name  : processGtpuGetTraceLvlCnf
 * Inputs         : setTraceLvlCnf_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process Get trace level cnf.
 ****************************************************************************/

UInt32 processGtpuGetTraceLvlCnf(UInt8 *getTraceLvlCnf_p, UInt16 msgLen)
{
    /* + COVERITY 11252 */
    UInt8 * gtpuGetTraceLvlCnf_p = PNULL;
    UInt16 current = API_HDR, index = CSPL_HDR, transId = 0;
    UInt8 cause = 0;

    gtpuGetTraceLvlCnf_p = qvMsgAllocBySize (MAX_API_LEN + OAM_L2_API_HDR);
    if (PNULL == gtpuGetTraceLvlCnf_p )
    {
        return RELAY_GTPU_FAILURE;
    }

    if (PNULL != getTraceLvlCnf_p)
    {
        /* + Layer2 NON CA Changes */
        gtpuGetTraceLvlCnf_p[API_HEADER_CELL_INDEX_OFFSET] = 
            transIdMappingForLM_g[LM_GET_TRACE_LEVEL_CNF].cellId;
        /* - Layer2 NON CA Changes */
        cause = gtpuGetTraceLvlCnf_p[current++] = getTraceLvlCnf_p[index++];
        gtpuGetTraceLvlCnf_p[current++] = getTraceLvlCnf_p[index++];
        if (!cause){
            gtpuGetTraceLvlCnf_p[current++] = getTraceLvlCnf_p[index++];
        }
        transId = transIdMappingForLM_g[LM_GET_TRACE_LEVEL_CNF].transId;
        transIdMappingForLM_g[LM_GET_TRACE_LEVEL_CNF].isValid = TRANS_ID_NO_REQ;
        /* Call function pointer to send the confirmation message to LM */
        if (RELAY_GTPU_SUCCESS !=
                sendConfirmationRsp[LM_GET_TRACE_LEVEL_CNF]
                (gtpuGetTraceLvlCnf_p,transId,current))
        {
            /*SPR 2030 fix Start*/ 
            /* +- SPR 17777 */
            LOG_GTPU_MSG(PR_SEND_CNF_TO_OAM_FAIL,LOGWARNING, PR_GTPU_OAM,
                    relayGtpuCurrentTick_g, __LINE__,
                    msgLen,0,0,0, 0,0, __func__,"GET_TRACE_LEVEL_CNF"); 
            /*SPR 2030 fix End*/ 
            /* SPR 22402 Fix Start */
            msgFree(gtpuGetTraceLvlCnf_p);
            /* SPR 22402 Fix End */
            return RELAY_GTPU_FAILURE;
        }
        /* SPR 22402 Fix Start */
        msgFree(gtpuGetTraceLvlCnf_p);
        /* SPR 22402 Fix End */
        return RELAY_GTPU_SUCCESS;
    }
    /* SPR 22402 Fix Start */
    msgFree(gtpuGetTraceLvlCnf_p);
    /* SPR 22402 Fix End */
    /* - COVERITY 11252 */
    return RELAY_GTPU_FAILURE;
}


/****************************************************************************
 * Function Name  : handleGtpuSetTimerReq
 * Inputs         : timerReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process set timer request for GTPU.
 ****************************************************************************/

STATIC UInt32 handleGtpuSetTimerReq(relayGtpuSetTimerReq *timerReq_p)
{
    UInt8 *gtpuSetTimerReq = PNULL;
    gtpuSetTimerReq = qvMsgAllocBySize(MAX_API_LEN + CSPL_HDR);
    /*+COVERITY 5.3.0 - 32135*/
    if ( !gtpuSetTimerReq )
    {
        return RELAY_GTPU_FAILURE;    
    }
    /*-COVERITY 5.3.0 - 32135*/
    UInt16 current = CSPL_HDR;
    gtpuSetTimerReq[current++]=timerReq_p->timerType;    
    RELAY_GTPU_SET_U32BIT(&gtpuSetTimerReq[current],timerReq_p->timerDuration);
    current += 4;
    /* Appending CSPL-Header before sending packet to GTP-U */
    appendCsplHeader(EI_OAM_MODULE_ID,GTPU_SET_TIMER_REQ,current,gtpuSetTimerReq);
    //qvSend (qvGetService(GTPU_MODULE_ID), 0, gtpuSetTimerReq);
    return RELAY_GTPU_SUCCESS;
}

/****************************************************************************
 * Function Name  : handleGtpuGetTimerReq
 * Inputs         : timerReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process get timer request for GTPU.
 ****************************************************************************/

STATIC UInt32 handleGtpuGetTimerReq(relayGtpuGetTimerReq *timerReq_p)
{
    UInt8 *gtpuGetTimerReq = PNULL;
    gtpuGetTimerReq = qvMsgAllocBySize (MAX_API_LEN + CSPL_HDR);
    /* +COVERITY 32131 */
    if (!gtpuGetTimerReq)
    {
        return RELAY_GTPU_FAILURE;
    }
    /* +COVERITY 32131 */
    UInt16 current = CSPL_HDR;
    gtpuGetTimerReq[current++] = timerReq_p->timerType;  
    /* Appending CSPL-Header before sending packet to GTP-U */
    appendCsplHeader(EI_OAM_MODULE_ID,GTPU_GET_TIMER_REQ,current,gtpuGetTimerReq);
    //qvSend (qvGetService(GTPU_MODULE_ID), 0, gtpuGetTimerReq);
    return RELAY_GTPU_SUCCESS;
}

/****************************************************************************
 * Function Name  : handleGtpuGetMaxRetriesReq
 * Inputs         : retriesReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process get Max retries request for GTPU.
 ****************************************************************************/

STATIC UInt32 handleGtpuGetMaxRetriesReq(relayGtpuGetMaxRetriesReq *retriesReq_p)
{
    UInt8 *gtpuMaxRetriesReq = PNULL;
    gtpuMaxRetriesReq = qvMsgAllocBySize (MAX_API_LEN + CSPL_HDR);
    /* +COVERITY 32130 */
    if (!gtpuMaxRetriesReq)
    {
        return RELAY_GTPU_FAILURE;
    }
    /* +COVERITY 32130 */
    UInt16 current = CSPL_HDR;
    gtpuMaxRetriesReq[current++] = retriesReq_p->timerType;
    /* Appending CSPL-Header before sending packet to GTP-U */
    appendCsplHeader(EI_OAM_MODULE_ID,GTPU_GET_MAX_RETRIES_REQ,current,gtpuMaxRetriesReq);
    //qvSend (qvGetService(GTPU_MODULE_ID), 0, gtpuMaxRetriesReq);
   return RELAY_GTPU_SUCCESS;
}

/****************************************************************************
 * Function Name  : handleGtpuSetMaxRetriesReq
 * Inputs         : retriesReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process set Max retries request for GTPU.
 ****************************************************************************/

STATIC UInt32 handleGtpuSetMaxRetriesReq(relayGtpuSetMaxRetriesReq *retriesReq_p)
{
    UInt8 *gtpuMaxRetriesReq = PNULL;
    gtpuMaxRetriesReq = qvMsgAllocBySize (MAX_API_LEN + CSPL_HDR);
    /* +COVERITY 32134 */
    if (!gtpuMaxRetriesReq)
    {
        return RELAY_GTPU_FAILURE;
    }
    /* +COVERITY 32134 */
    UInt16 current = CSPL_HDR;
    gtpuMaxRetriesReq[current++] = retriesReq_p->timerType;
    RELAY_GTPU_SET_U16BIT(&gtpuMaxRetriesReq[current],retriesReq_p->maxNumOfRetries);
    current += 2;
    /* Appending CSPL-Header before sending packet to GTP-U */
    appendCsplHeader(EI_OAM_MODULE_ID,GTPU_SET_MAX_RETRIES_REQ,current,gtpuMaxRetriesReq);
    //qvSend (qvGetService(GTPU_MODULE_ID), 0, gtpuMaxRetriesReq);
   return RELAY_GTPU_SUCCESS;
}

/****************************************************************************
 * Function Name  : handleGtpuSetTraceLvlReq
 * Inputs         : setTraceLvlReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process Set Trace Lvl request for GTPU.
 ****************************************************************************/

STATIC UInt32 handleGtpuSetTraceLvlReq(relaySetTraceLvlReq *setTraceLvlReq_p)
{
    UInt8 *gtpuSetTraceLvlReq = PNULL;
    gtpuSetTraceLvlReq = qvMsgAllocBySize (MAX_API_LEN + CSPL_HDR);
    /*+COVERITY 5.3.0 - 32136*/
    if ( !gtpuSetTraceLvlReq )
    {
        return RELAY_GTPU_FAILURE;
    }
    /*-COVERITY 5.3.0 - 32136*/
    UInt16 current = CSPL_HDR;
    gtpuSetTraceLvlReq[current++] = setTraceLvlReq_p->traceFlag;
    gtpuSetTraceLvlReq[current++] = setTraceLvlReq_p->traceLevel; 
    /* Appending CSPL-Header before sending packet to GTP-U */
    appendCsplHeader(EI_OAM_MODULE_ID,GTPU_SET_TRACE_LEVEL_REQ,current,gtpuSetTraceLvlReq);
    //qvSend (qvGetService(GTPU_MODULE_ID), 0, gtpuSetTraceLvlReq);
   return RELAY_GTPU_SUCCESS;
}

/****************************************************************************
 * Function Name  : handleGtpuGetTraceLvlReq
 * Inputs         : getTraceLvlReq_p 
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process Get Trace Lvl request for GTPU.
 ****************************************************************************/

STATIC UInt32 handleGtpuGetTraceLvlReq(relayGetTraceLvlReq *getTraceLvlReq_p)
{
    UInt8 *gtpuGetTraceLvlReq = PNULL;
    gtpuGetTraceLvlReq = qvMsgAllocBySize (MAX_API_LEN + CSPL_HDR);
    /* +COVERITY 32132 */
    if (!gtpuGetTraceLvlReq)
    {
        return RELAY_GTPU_FAILURE;
    }
    /* +COVERITY 32132 */
    UInt16 current = CSPL_HDR;
    gtpuGetTraceLvlReq[current++] = getTraceLvlReq_p->traceFlag;
    /* Appending CSPL-Header before sending packet to GTP-U */
    appendCsplHeader(EI_OAM_MODULE_ID,GTPU_GET_TRACE_LEVEL_REQ,current,gtpuGetTraceLvlReq);
    //qvSend (qvGetService(GTPU_MODULE_ID), 0, gtpuGetTraceLvlReq);
   return RELAY_GTPU_SUCCESS;
}

/****************************************************************************
 * Function Name  : handleGtpuInitTraceLvlReq
 * Inputs         : None
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process Init Trace Lvl request for GTPU.
 ****************************************************************************/

STATIC UInt32 handleGtpuInitTraceLvlReq(void)
{
    UInt8 *gtpuInitTraceLvlReq = PNULL;
    gtpuInitTraceLvlReq = qvMsgAllocBySize (MAX_API_LEN + CSPL_HDR);
    /*Klocwork Fix*/
    if (PNULL == gtpuInitTraceLvlReq)
    {
        return RELAY_GTPU_FAILURE;    
    }
    /*Klocwork Fix*/
    UInt16 current = CSPL_HDR;
    /* In this API, No IE is required so passing with Appending CSPL
       Header */
    appendCsplHeader(EI_OAM_MODULE_ID,GTPU_INIT_TRACE_LEVEL_REQ,current,gtpuInitTraceLvlReq);
    //qvSend (qvGetService(GTPU_MODULE_ID), 0, gtpuInitTraceLvlReq);
   return RELAY_GTPU_SUCCESS;
}

/****************************************************************************
 * Function Name  : handleGtpuInitDbReq 
 * Inputs         : initDbReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process Init DB Request for GTPU.
 ****************************************************************************/
STATIC UInt32 handleGtpuInitDbReq(relayGtpuInitDBReq *initDbReq_p)
{
    UInt8 *gtpuInitDbReq = PNULL;
    UInt32 logLevel = 0; 
    gtpuInitDbReq = qvMsgAllocBySize (MAX_API_LEN + CSPL_HDR); 
    /* +COVERITY 32133 */
    if (!gtpuInitDbReq)
    {
        return RELAY_GTPU_FAILURE;
    }
    /* +COVERITY 32133 */
    UInt16 current = CSPL_HDR, startVal = 0;
    /*IPV6_CHANGE - START*/UInt8 count = 0,presence = 0;/*IPV6_CHANGE - END*/
/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/
    RELAY_GTPU_SET_U32BIT(&gtpuInitDbReq[current],initDbReq_p->noRabRec.initSize); 
    current += 4;
    RELAY_GTPU_SET_U32BIT(&gtpuInitDbReq[current],initDbReq_p->noRabRec.stepSize); 
    current += 4;
    RELAY_GTPU_SET_U32BIT(&gtpuInitDbReq[current],initDbReq_p->noRabRec.maxSize); 
    current += 4;
    RELAY_GTPU_SET_U16BIT(&gtpuInitDbReq[current],initDbReq_p->noPeerRec); 
    current += 2;
    gtpuInitDbReq[current++] = initDbReq_p->numMyAddr4; 
    gtpuInitDbReq[current++] = initDbReq_p->numMyAddr6; 
    /*IPV6_CHANGE - START*/
    for(count=0; count<initDbReq_p->numMyAddr4; count++)
    {
        RELAY_GTPU_SET_U16BIT(&gtpuInitDbReq[current],initDbReq_p->myAddr4.length); 
        current += 2;
        for (startVal = 0; startVal < initDbReq_p->myAddr4.length; startVal++)
        {
            gtpuInitDbReq[current++] = initDbReq_p->myAddr4.value[startVal];   
        }
       
        /* + Coverity 31402 */
        handleAdapterSockInf( relayConfigInfo_g.adapterSocketInf, 20, 
                gtpuInitDbReq, current);
        /* - Coverity 31402 */
        strNCpy(configData->adapterIP,relayConfigInfo_g.adapterSocketInf,sizeof(configData->adapterIP));
        presence = IPV4_ADDR_PRESENT;
        configData->addrPresenceSt=presence;
/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/
    }
    for(count=0; count<initDbReq_p->numMyAddr6; count++)
    {
        RELAY_GTPU_SET_U16BIT(&gtpuInitDbReq[current],initDbReq_p->myAddr6.length); 
        current += 2;
        for (startVal = 0; startVal < initDbReq_p->myAddr6.length; startVal++)
        {
            configData->adapterIP6[startVal] = gtpuInitDbReq[current++] = initDbReq_p->myAddr6.value[startVal];
        }
        presence |= IPV6_ADDR_PRESENT;
        configData->addrPresenceSt = presence;


    }

    configData->adapterRxPort=EGTPU_UDP_PORT;

/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/

/*IPV6_CHANGE - END*/
    /* + SPR 19022 */
    initInternetSocket(configData);
    /* - SPR 19022 */

    /* eMBMS Code Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
    initInternetSocketForEmbms();
#endif
    /* eMBMS Code Changes End */

    /*+SPR #3456*/
    /* SPR 10597 Changes Start */
    RELAY_GTPU_SET_U32BIT(&gtpuInitDbReq[current],initDbReq_p->echoReqTimer); 
    current += 4;
    /* SPR 10597 Changes End */
    gtpuInitDbReq[current++] = initDbReq_p->maxRetry;
    /*-SPR #3456*/

    MsgPool_g = msgPoolCreate(0, NO_OF_PACKET_RELAY_BUFFERS);

    /* Append CSPL Header before sending packet to eGTP-U */
    appendCsplHeader(EI_OAM_MODULE_ID,GTPU_INITDB_REQ,current,gtpuInitDbReq);

    logLevel = initDbReq_p->logLevel;
    l2_map_ext_loglevel_to_intern_loglevel((UInt8 *)&logLevel);
    gtpuloglevelEnabled_g = ( (2 * logLevel) -  1); 


#ifdef ENDC_ENABLED
    /*ToDo: Pass this info to X2-U Relay*/
    if(RELAY_GTPU_FAILURE == x2uRelayUpdateDataFwdTimerAndThresholds(initDbReq_p->x2uDelStatusThreshold, 
    initDbReq_p->dataForwardingGuardTimerInterval, initDbReq_p->isGtpuNRHeaderEnabled))
    {
        return RELAY_GTPU_FAILURE;
    }
#endif
    egtpu_process_msg(p_global, gtpuInitDbReq);	
    return RELAY_GTPU_SUCCESS;
}

/*SPR 2030 fix Start*/
/****************************************************************************
 * Function Name  : processGtpuSetLogLevelReq
 * Inputs         : setLogLevReq,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This function sets requested Log level for entire L2
 *                  currently, not for just EGTPU.
 ****************************************************************************/
UInt32 processGtpuSetLogLevelReq(UInt8 *setLogLevReq, UInt16 msgLen)
{
    relayGtpuSetLogLevReq logLevReq = {0};
    UInt16 current = 0;

    if (PNULL != setLogLevReq)  
    {
       current = OAM_L2_API_HDR;
       
       logLevReq.logLevel = RELAY_GTPU_GET_U32BIT(&setLogLevReq[current]);

       /*Validate params received*/
        /* Coverity 54581 */
       if( LOGTTIDEBUG < logLevReq.logLevel )
       {
           /* +- SPR 17777 */
           LOG_GTPU_MSG(PR_OAM_INV_LOG_LEV,LOGWARNING, PR_GTPU_OAM,
                   relayGtpuCurrentTick_g, __LINE__,
                   INVALID_LOG,LOGDEBUG,logLevReq.logLevel,msgLen, 0,0, __func__, 
                   "");
           return RELAY_GTPU_FAILURE;
       }
        /* Coverity 54581 */
       /*Set the Log Level for the entire L2*/
       if (INVALID_LOG == logLevReq.logLevel)
       {
           gtpuloglevelEnabled_g = INVALID_LOG;
           return RELAY_GTPU_FAILURE;
       }
       else
       {
           gtpuloglevelEnabled_g = ( (2 * logLevReq.logLevel) -  1);
       }

    }  
    return RELAY_GTPU_SUCCESS; 
}

/****************************************************************************
 * Function Name  : processGtpuGetLogLevelReq 
 * Inputs         : getLogLevReq,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This function Gets the entire L2 log level 
 *                  already set, not for just EGTPU.
 ****************************************************************************/
UInt32 processGtpuGetLogLevelReq(UInt8 *getLogLevReq, UInt16 msgLen)
{
    UInt8           *responseBuf = PNULL; /*Buffer for Response*/
    UInt16          current = 0;
    UInt16          transIdForRsp = 0;

    responseBuf = (UInt8 *)getMemFromPool((OAM_L2_API_HDR + 
                sizeof(relayGtpuGetLogLevResp)), PNULL);

    if(!responseBuf)
    {
        /* +- SPR 17777 */
        LOG_GTPU_MSG(L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                relayGtpuCurrentTick_g, __LINE__, sizeof(API_HDR) + 
                sizeof(relayGtpuGetLogLevResp),msgLen,0,0,0,0, __func__,
                "PacketRelay_GET_LOG_LEVEL_REQ response" );
        ALARM_MSG(PACKET_RELAY_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                CRITICAL_ALARM);
        return RELAY_GTPU_FAILURE; 
    }


    transIdForRsp = RELAY_GTPU_GET_U16BIT(&getLogLevReq[current]);
    current = API_HDR;
 /* + Layer2 NON CA Changes */
    /*
     * Cell Index will be in 11 byte of 
     * getLogLevReq buffer
     */

    responseBuf[API_HEADER_CELL_INDEX_OFFSET] = getLogLevReq[API_HEADER_CELL_INDEX_OFFSET];

 /* - Layer2 NON CA Changes */

    RELAY_GTPU_SET_U32BIT(&responseBuf[current],gtpuloglevelEnabled_g);

    current = OAM_L2_API_HDR +  sizeof(relayGtpuGetLogLevResp);

    if( RELAY_GTPU_SUCCESS !=
            sendConfirmationRsp[PR_GTPU_GET_LOG_LEVEL_RESP]
            (responseBuf, transIdForRsp, current))
    {
        LOG_GTPU_MSG(PR_SEND_CNF_TO_OAM_FAIL,LOGWARNING, PR_GTPU_OAM,
                relayGtpuCurrentTick_g, __LINE__,
                0,0,0,0, 0,0, __func__,"GET_LOG_LEVEL_RESP"); 
        /* + coverity 32420 */
        freeMemPool(responseBuf);
        /* - coverity 32420 */
        return RELAY_GTPU_FAILURE; 
      }

    freeMemPool(responseBuf);

    return RELAY_GTPU_SUCCESS;
}

/****************************************************************************
 * Function Name  : processGtpuEnableLogCategoryReq
 * Inputs         : enLogCatReq
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This function enables the specified log category.
 ****************************************************************************/
UInt32 processGtpuEnableLogCategoryReq(UInt8 *enLogCatReq, UInt16 msgLen)
{
    relayGtpuLogCategoryReq logCatReq = {0};
    UInt16 current = 0;
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(msgLen)
    /* +- SPR 17777 */

    if (PNULL != enLogCatReq)  
    {
        current = OAM_L2_API_HDR;
        logCatReq.logCategory = RELAY_GTPU_GET_U64BIT(&enLogCatReq[current]);
        /*Update the Global*/
        logCategoryEnabled_g |= logCatReq.logCategory;

        return RELAY_GTPU_SUCCESS; 
    } 
    else
    return RELAY_GTPU_FAILURE; 

}

/****************************************************************************
 * Function Name  : processGtpuDisableLogCategoryReq
 * Inputs         : disLogCatReq,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This function disables the specified log category.
 ****************************************************************************/
UInt32 processGtpuDisableLogCategoryReq(UInt8 *disLogCatReq, UInt16 msgLen)
{
    relayGtpuLogCategoryReq logCatReq = {0};
    UInt16 current = 0;
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(msgLen)
    /* +- SPR 17777 */

    if (PNULL != disLogCatReq)  
    {
        current = OAM_L2_API_HDR;
        logCatReq.logCategory = RELAY_GTPU_GET_U64BIT(&disLogCatReq[current]);
        /*Update the global*/  
        logCategoryEnabled_g &= ~(logCatReq.logCategory);


    return RELAY_GTPU_FAILURE; 
    }  
    return RELAY_GTPU_SUCCESS; 
}

/****************************************************************************
 * Function Name  : processGtpuGetLogCategoryReq
 * Inputs         : getLogCatReq,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This function gets the log category set.
 ****************************************************************************/
UInt32 processGtpuGetLogCategoryReq(UInt8 *getLogCatReq, UInt16 msgLen)
{
    UInt8           *responseBuf = PNULL; /*Buffer for Response*/
    UInt16          current = 0;
    UInt16          transIdForRsp = 0;

    responseBuf = (UInt8 *)getMemFromPool((OAM_L2_API_HDR + 
                sizeof(relayGtpuGetLogCategoryResp)), PNULL);

    if(!responseBuf)
    {
        /* +- SPR 17777 */
        LOG_GTPU_MSG(L2_GET_MEMFROMPOOL_FAIL_ID, LOGFATAL, L2_SYS_FAIL,
                relayGtpuCurrentTick_g, __LINE__, sizeof(API_HDR) + 
                sizeof(relayGtpuGetLogCategoryResp),msgLen,0,0,0,0, __func__,
                "PacketRelay_GET_LOG_CATEGORY_REQ response" );
        ALARM_MSG(PACKET_RELAY_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                CRITICAL_ALARM);
        return  RELAY_GTPU_FAILURE;
    }


    transIdForRsp = RELAY_GTPU_GET_U16BIT(&getLogCatReq[current]);
    current = API_HDR;

 /* + Layer2 NON CA Changes */

    /*
     * Cell Index will be in 11 byte of 
     * getLogLevReq buffer
     */

    responseBuf[API_HEADER_CELL_INDEX_OFFSET] = getLogCatReq[API_HEADER_CELL_INDEX_OFFSET];

 /* - Layer2 NON CA Changes */
    RELAY_GTPU_SET_U64BIT(&responseBuf[current],logCategoryEnabled_g);

    current = OAM_L2_API_HDR + sizeof(relayGtpuGetLogCategoryResp);

    if( RELAY_GTPU_SUCCESS !=
            sendConfirmationRsp[PR_GTPU_GET_LOG_CATEGORY_RESP]
            (responseBuf, transIdForRsp, current))
    {
        LOG_GTPU_MSG(PR_SEND_CNF_TO_OAM_FAIL,LOGWARNING, PR_GTPU_OAM,
                relayGtpuCurrentTick_g, __LINE__,
                0,0,0,0, 0,0, __func__,"GET_LOG_CATEGORY_RESP");
        /* + coverity 32419 */
        freeMemPool(responseBuf);
        /* + coverity 72788 */
        //responseBuf = PNULL;
        /* - coverity 72788 */
        /* - coverity 32419 */
        return RELAY_GTPU_FAILURE;
    }
    freeMemPool(responseBuf);

    return RELAY_GTPU_SUCCESS;
}
/*SPR 2030 fix End*/

/****************************************************************************
 * Function Name  : processGtpuGetStatsReq
 * Inputs         : getStatsReq_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process Get Stats Request
 ****************************************************************************/
UInt32 processGtpuGetStatsReq(UInt8 *getStatsReq_p, UInt16 msgLen)
{
    relayGtpuGetStatsReq getStatsReq;
    UInt16 current = 0, transId = 0;

    getStatsReq.statsType=0xFF; /* Set to invalid value */

    if (PNULL != getStatsReq_p)
    {
        transId = RELAY_GTPU_GET_U16BIT(&getStatsReq_p[current]);
        current = OAM_L2_API_HDR;
       transIdMappingForLM_g[PR_GTPU_GET_STATS_RESP].cellId = 
           getStatsReq_p[API_HEADER_CELL_INDEX_OFFSET];

        /* SPR 21030 Fix Start */
        if (transIdMappingForLM_g[PR_GTPU_GET_STATS_RESP].transId != transId)
        {
            transIdMappingForLM_g[PR_GTPU_GET_STATS_RESP].isValid = TRANS_ID_NO_REQ;
        }
        /* SPR 21030 Fix End */
        /* Checking for new request to get stats*/
        if (TRANS_ID_DUPLICATE_REQ != transIdMappingForLM_g[PR_GTPU_GET_STATS_RESP].isValid)
        {  
            getStatsReq.statsType = getStatsReq_p[current++];
            getStatsReq.resetFlag = getStatsReq_p[current++];

            /* Validate the contents */
            /* coverity fix 10050 + */
            if (getStatsReq.statsType > RELAY_MAX_STATS_TYPE)
                /* coverity fix 10050 - */
            {
                /* +- SPR 17777 */
                LOG_GTPU_MSG(PR_UNABLE_TO_SEND_ERR_REPORT_TO_OAM, LOGWARNING,
                        PR_GTPU_OAM,
                        relayGtpuCurrentTick_g, __LINE__,
                        msgLen,0,0,0,0,0, __func__,
                        "PR_GTPU_GET_STATS_REQ" );

                /* Send Failure to OAM */
                prepareAndSendErrorReportToLM(transId, PR_GTPU_GET_STATS_RESP);
                /*++klockwork warning */
                /*code removed*/
                /*--klockwork warning */

                return RELAY_GTPU_FAILURE;
            }

            /* Get Stats sent successfully to EGTPU stack for processing */ 
            /*  Message valid, now Process the content */
	    /* SPR 21441 Start */
            transIdMappingForLM_g[PR_GTPU_GET_STATS_RESP].transId = transId;
            transIdMappingForLM_g[PR_GTPU_GET_STATS_RESP].isValid = TRANS_ID_DUPLICATE_REQ;
	    /* SPR 21441 End */
            if (RELAY_GTPU_SUCCESS != handleGtpuGetStatsReq(&getStatsReq))
            {
                LOG_GTPU_MSG(PR_GTPU_API_PROCESSING_FAILED, LOGWARNING, 
                        PR_GTPU_OAM,
                        relayGtpuCurrentTick_g, __LINE__, 
                        0,0,0,0,0,0, __func__,
                        "PR_GTPU_GET_STATS_REQ" );
                /* Send Failure to OAM */
                prepareAndSendErrorReportToLM(transId, PR_GTPU_GET_STATS_RESP);
		/* SPR 21441 Start */
        	transIdMappingForLM_g[PR_GTPU_GET_STATS_RESP].isValid = TRANS_ID_NO_REQ;
		/* SPR 21441 End */
                return RELAY_GTPU_FAILURE;
            }
            /* SPR 9521 Fix start*/
            /* Updating transId and isValid flag only if processing of content is fine*/
            /* SPR 21441 +- */
            /* SPR 9521 Fix end */
        }
        else
        {
            LOG_GTPU_MSG( PR_UNABLE_TO_SEND_ERR_REPORT_TO_OAM, LOGWARNING, 
                    PR_GTPU_OAM,
                    relayGtpuCurrentTick_g, __LINE__, 
                    0,0,0,0,0,0, __func__,
                    "PR_GTPU_GET_STATS_REQ" );

            /* Send Failure to OAM */
            prepareAndSendErrorReportToLM(transId, PR_GTPU_GET_STATS_RESP);

            return RELAY_GTPU_FAILURE;
        }

        return RELAY_GTPU_SUCCESS;
    }
    return RELAY_GTPU_FAILURE; 
}

/*******************************************************************************
 * Function Name  : handleGtpuGetStatsReq
 * Inputs         : getStatsReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process Get Stats Request and call GTPU stack.
 *******************************************************************************/
UInt32 handleGtpuGetStatsReq(relayGtpuGetStatsReq *getStatsReq_p)
{
    UInt8 *gtpuGetStatsReq_p = PNULL;
    UInt16 current = CSPL_HDR, apiId = 0;

    gtpuGetStatsReq_p = (UInt8 *)qvMsgAllocBySize (MAX_API_LEN + CSPL_HDR);
    if(PNULL == gtpuGetStatsReq_p)
    {
        return RELAY_GTPU_FAILURE;
    }
    /* if stats are not to be reset */
    if (getStatsReq_p->resetFlag == RELAY_NOT_RESET)
    {
        apiId = GTPU_GET_STATS_REQ;
    }
    else /* if the stats are to be reset */
    {
        apiId = GTPU_GET_INIT_STATS_REQ;
    }

    /* Send the statsType for which statistics to be reported */
    gtpuGetStatsReq_p[current++] = getStatsReq_p->statsType;

    MsgPool_g = msgPoolCreate(0, NO_OF_PACKET_RELAY_BUFFERS);

    /* Append CSPL Header before sending packet to eGTP-U */
    appendCsplHeader(EI_OAM_MODULE_ID, apiId, current, gtpuGetStatsReq_p);

    egtpu_process_msg(p_global, gtpuGetStatsReq_p);

    return RELAY_GTPU_SUCCESS;
}

/****************************************************************************
 * Function Name  : processGtpuGetStatsResp
 * Inputs         : getStatsCnf_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process the GET STATS response.
 ****************************************************************************/
UInt32 processGtpuGetStatsResp(UInt8 *getStatsCnf_p, UInt16 msgLen)
{
    UInt16 current = API_HDR, index = CSPL_HDR, statsLength = 0;
    UInt16 transId = 0;
    UInt8 result   = 0;

    UInt8* gtpuGetStatsCnf_p = PNULL;
    gtpuGetStatsCnf_p = (UInt8 *)getMemFromPool(MAX_API_LEN + OAM_L2_API_HDR, NULL);
    if(PNULL == gtpuGetStatsCnf_p)
    {
        return RELAY_GTPU_FAILURE;
    }

    if (PNULL != getStatsCnf_p)
    {
        gtpuGetStatsCnf_p[API_HEADER_CELL_INDEX_OFFSET] = 
            transIdMappingForLM_g[PR_GTPU_GET_STATS_RESP].cellId;
        /* Copy result */
        result = getStatsCnf_p[index];
        *(gtpuGetStatsCnf_p + current++) = getStatsCnf_p[index++];

        /* Copy stats Type */
        *(gtpuGetStatsCnf_p + current++) = getStatsCnf_p[index++];

        /* Copy stats if success */
        if (EGTPU_SUCCESS == result)
        {
            /* Get length */
            statsLength = RELAY_GTPU_GET_U16BIT(&getStatsCnf_p[index]);
            index += 2;
            /* Set Length */
            RELAY_GTPU_SET_U16BIT((gtpuGetStatsCnf_p + current), statsLength);
            current += 2;
            if((SInt32)(statsLength * sizeof(UInt32)) > (MAX_API_LEN + OAM_L2_API_HDR - current ))
            {
                freeMemPool(gtpuGetStatsCnf_p);
                return RELAY_GTPU_FAILURE;
            }

            /* Copy stats */
            memCpy((gtpuGetStatsCnf_p + current), &getStatsCnf_p[index], statsLength * sizeof(UInt32)); 
            current += (statsLength * sizeof(UInt32));
        }

        transId = transIdMappingForLM_g[PR_GTPU_GET_STATS_RESP].transId;

        /* Call function pointer to send the confirmation message to LM */
        if (RELAY_GTPU_SUCCESS != sendConfirmationRsp[PR_GTPU_GET_STATS_RESP]
                (gtpuGetStatsCnf_p,transId,current))
        {
            LOG_GTPU_MSG(PR_SEND_CNF_TO_OAM_FAIL,LOGWARNING, PR_GTPU_OAM,
                    relayGtpuCurrentTick_g, __LINE__,
                    msgLen,0,0,0, 0,0, __func__,"INITDB_CNF");

            freeMemPool(gtpuGetStatsCnf_p); 

            return RELAY_GTPU_FAILURE;
        }

        /* Reseting the isValid flag once the stats response is received */ 
        transIdMappingForLM_g[PR_GTPU_GET_STATS_RESP].isValid = TRANS_ID_NO_REQ;

        freeMemPool(gtpuGetStatsCnf_p); 

        return RELAY_GTPU_SUCCESS;
    }
    freeMemPool(gtpuGetStatsCnf_p); 
    return RELAY_GTPU_FAILURE;
}

/****************************************************************************
 * Function Name  : processGtpuGetHealthMonitoringReq
 * Inputs         : hsmReq,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This function used to get health status of GTPu.
 ****************************************************************************/
UInt32 processGtpuGetHealthMonitoringReq(UInt8 *hsmReq, UInt16 msgLen)
{
    UInt16       current            = 0;
    UInt16       transIdForRsp      = 0;
    UInt8        respMsg_p[API_HDR] = {0};
#ifdef PDCP_GTPU_INTF
    /* If process is up then this message handler will be called directly, 
     * hence replying without the check which indicates that this module is 
     * up and running. Also, currently as part of GTP as a separate module, 
     * there are no multiple threads */
    UInt8        respCode           = RELAY_GTPU_SUCCESS;
#else
    tickType_t   curr_tick          = relayGtpuCurrentTick_g;
    UInt8        threadIndx         = 0;
    UInt8        respCode           = RELAY_GTPU_FAILURE;
    /* Check the state of cells, if not any cell active then always return success */
    if(INVALID_CELL_INDEX != layer2CommonGetActiveCellTick())
    {
        for (threadIndx = 0; threadIndx < GTPU_MAX_THREAD_ID; threadIndx++)
        {
            if (curr_tick - l2ThreadHealthStoreTick_g.egtpuThread[threadIndx] <
                    gtpuHealthMonitoringTimeInterval_g)
            {
                respCode = RELAY_GTPU_SUCCESS;
            }
            else
            {
                respCode = RELAY_GTPU_FAILURE;
                /* Break from loop if any thread is in fail state */
                break;
            }
        }
    }
    else
    {
        respCode = RELAY_GTPU_SUCCESS;
    }
    if(RELAY_GTPU_SUCCESS != respCode)
    {
        LOG_GTPU_MSG(HSM_GTPU_OAM_REQ_FAILURE, LOGFATAL, L2_SYS_FAIL,
                relayGtpuCurrentTick_g,threadIndx,
                msgLen, DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                __func__,"GTPU OAM Health Status Failure");
        return respCode ;
    }
#endif
    transIdForRsp = RELAY_GTPU_GET_U16BIT(&hsmReq[current]);

    appendApiHeader( RELAY_OAM_MODULE_ID, PR_GTPU_PROC_SUP_RESP,\
            transIdForRsp, API_HDR, respMsg_p, hsmReq[API_HEADER_CELL_INDEX_OFFSET]);
    
    LOG_GTPUPR_INFO( PR_GTPU_OAM, "SEND PR_GTPU_PROC_SUP_RESP to OAM");

    /* send to OAM */
    udpSendToOam((Char8*)&respMsg_p[0], API_HDR);

    return respCode;
}
/* SPR 13765 fix Start */
/**********************************************************************************
 * Function Name  : processGtpuErrorInd
 * Inputs         : gtpuErrorMsg_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This function is used to prepare and send msg of GTPU Error Indication
 ***********************************************************************************/
UInt32 processGtpuErrorInd(UInt8 *gtpuErrorMsg_p, 
                                  UInt16 msgLen)
{
	UInt16 startval = 0;
	UInt16 current  = API_HDR;
	UInt16 transId  = 0xFFFF;
	UInt8 *relayGtpuErrorIndMsgbuff = PNULL;

    /* SPR 14462 Fix Starts */
    UInt8 currentByte = 0;
    /* SPR 14462 Fix End */

	relayGtpuErrorIndMsgbuff = qvMsgAllocBySize(MAX_API_LEN + OAM_L2_API_HDR);
	if(PNULL == relayGtpuErrorIndMsgbuff)
	{
		return RELAY_GTPU_FAILURE ;
	}
	if(S1U_IE_GSN_ADDR_TAG == RELAY_GTPU_GET_U16BIT(&gtpuErrorMsg_p[current]))
	{
		RELAY_GTPU_SET_U32BIT(&relayGtpuErrorIndMsgbuff[API_HDR],S1U_LINK_FAILURE); /* SPR 13837 Fix */
	}
	else if(S1U_IE_GSN_ADDR_RECOVERED_TAG == RELAY_GTPU_GET_U16BIT(&gtpuErrorMsg_p[current]))
	{
		RELAY_GTPU_SET_U32BIT(&relayGtpuErrorIndMsgbuff[API_HDR],S1U_LINK_FAILURE_RECOVERED);  /* SPR 13837 Fix */
	}
	current = current+RELAY_FOUR ;
    /* SPR 14462 Fix Starts */
	for(startval=0; current < msgLen ; startval++ ,current++ )
	{
        /*existing gtpuErrorMsg_p will be used for preparing response 
        for L3 too hence copying current byte into local*/
        currentByte = gtpuErrorMsg_p[current];

        if(currentByte/RELAY_HUNDRED)
        {
            relayGtpuErrorIndMsgbuff[API_HDR+RELAY_FOUR+startval++] = '0'+currentByte/RELAY_HUNDRED ;
            currentByte = currentByte % RELAY_HUNDRED ;
        }

        if(currentByte/RELAY_TEN)
        {
            relayGtpuErrorIndMsgbuff[API_HDR+RELAY_FOUR+startval++] = '0'+currentByte/RELAY_TEN ;
        }
        else
        {
            if((startval)&&(relayGtpuErrorIndMsgbuff[API_HDR+RELAY_FOUR+startval-1]!='.'))
            {
                relayGtpuErrorIndMsgbuff[API_HDR+RELAY_FOUR+startval++] = '0'+ currentByte/RELAY_TEN ;
            }
        }
        currentByte = currentByte % RELAY_TEN ;
        relayGtpuErrorIndMsgbuff[API_HDR+RELAY_FOUR+startval++] = '0'+currentByte;
        relayGtpuErrorIndMsgbuff[API_HDR+RELAY_FOUR+startval] = '.';   
    }
    relayGtpuErrorIndMsgbuff[API_HDR+RELAY_FOUR+startval-1] = '\0';
    current=current+(startval-(current-(API_HDR+RELAY_FOUR)));
    /* SPR 14462 Fix End */
	appendApiHeader(RELAY_OAM_MODULE_ID,PR_GTPU_ERROR_IND,transId,current,relayGtpuErrorIndMsgbuff,API_HEADER_CELL_ID_NIL);
	LTE_LOG(LOG_INFO,"[SENDING] PR_GTPU_ERROR_IND to OAM\n");
	udpSendToOam((Char8 *)&relayGtpuErrorIndMsgbuff[0],current);
    /* SPR 22402 Fix Start */
	msgFree(relayGtpuErrorIndMsgbuff);
    /* SPR 22402 Fix End */
	return RELAY_GTPU_SUCCESS;
}
/* SPR 13765 fix END */

/*SPR 17585 +*/
/****************************************************************************
 * Function Name  : prepareS1UInitIndHeader
 * Inputs         : None
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This function sends S1U_INIT_LAYER_IND to OAM.
 ****************************************************************************/
UInt32 s1UInitIndication()
{
    UInt8 initIndBuff[MAX_INIT_BUFF_SIZE] = {0};
    
    /* Prepare header */ 
    /*SPR 20908 +*/
    appendApiHeader(RELAY_OAM_MODULE_ID, S1U_INIT_LAYER_IND,
		0xFF, OAM_L2_API_HDR, initIndBuff, API_HEADER_CELL_ID_NIL );

    LOG_GTPUPR_INFO( PR_GTPU_OAM, "Sending S1U_INIT_IND to OAM");
    /*SPR 20908 -*/
    /* send to OAM */
     if ( -1 ==
         sendto_wrapper(relayConfigInfo_g.s1ApTxFd,(Char8*)&initIndBuff[0],OAM_L2_API_HDR,0,&relayConfigInfo_g.oamToAddr,sizeof(SockAddr)))
     {
         LOG_GTPUPR_ERROR( PR_GTPU_OAM, "Not able to send UDP Packet");
         return RELAY_GTPU_FAILURE;
     }

    return RELAY_GTPU_SUCCESS;
}
/*SPR 17585 -*/

/*spr 22474 start*/
/****************************************************************************
 * Function Name  : processGtpuGetDebugInfoReq
 * Inputs		      : getDebugInfoReq_p, msgLen
 * Outputs	      : None
 * Returns	      : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description	      : This funtion process Get Debug Info Request
 ****************************************************************************/
UInt32 processGtpuGetDebugInfoReq (UInt8 *getDebugInfoReq_p, UInt16 msgLen)
{
    UInt16 current = 0;
    UInt32 respCode = RELAY_GTPU_SUCCESS;
    UInt16 debugType = 0;
    UInt8  *responseBuf_p = PNULL; /*Buffer for Response*/
    UInt16 transIdForRsp = 0;
	GtpuGetDebugInfoRespParams *gtpuGetDebugInfoRespParams_p = PNULL;

    responseBuf_p = (UInt8 *)getMemFromPool((OAM_L2_API_HDR + sizeof(GtpuGetDebugInfoRespParams)), PNULL);

    if(!responseBuf_p)
    {
        LOG_GTPUPR_FATAL(PR_GTPU_OAM, "GTPU_GET_DEBUG_INFO_RESP Failed to get memory of size:%u", 
		   	                   (OAM_L2_API_HDR + sizeof(GtpuGetDebugInfoRespParams)));

	    ALARM_MSG(PACKET_RELAY_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);

	    return RELAY_GTPU_FAILURE;
    }

    transIdForRsp = RELAY_GTPU_GET_U16BIT(&getDebugInfoReq_p[current]);
    /* Cell Index will be in 11 byte of buffer,for filling the same in response buffer */

	current = OAM_L2_API_HDR; /* Get the posiiton to payload */

    /* Get the debugType what type of information required by OAM */
    debugType = getDebugInfoReq_p[current];

    gtpuGetDebugInfoRespParams_p = (GtpuGetDebugInfoRespParams *)(responseBuf_p + current);

    switch(debugType)
    {
	    case GET_DEBUG_INFO_MEM_STATS:
	    {
            gtpuGetDebugInfoRespParams_p->debugType = GET_DEBUG_INFO_MEM_STATS;
 		    L2PrepareMemPoolStats (&(gtpuGetDebugInfoRespParams_p->u.csplMemPoolStats));
		    break;
	    }
	    case GET_DEBUG_INFO_MSG_STATS:
	    {
            gtpuGetDebugInfoRespParams_p->debugType = GET_DEBUG_INFO_MSG_STATS;
            L2PrepareMsgPoolStats (&(gtpuGetDebugInfoRespParams_p->u.csplMsgPoolStats));
		    break;
	    }
	    default:
	    {
            gtpuGetDebugInfoRespParams_p->debugType = debugType;
			respCode = RELAY_GTPU_FAILURE;
            LOG_GTPUPR_WARNING( PR_GTPU_OAM, 
				   "Invalid value of debugType:%u received in GTPU_GET_DEBUG_INFO_REQ",
				   debugType);
   
		    break;
	    }
    }

    current = OAM_L2_API_HDR + sizeof(GtpuGetDebugInfoRespParams);
    gtpuGetDebugInfoRespParams_p->responseCode = respCode;

    if( RELAY_GTPU_SUCCESS !=
			   sendConfirmationRsp[GTPU_OAM_GET_DEBUG_INFO_RESP]
			   (responseBuf_p, transIdForRsp, current))
    {
		LOG_GTPUPR_WARNING(PR_GTPU_OAM, "Fail to send confirmation to OAM"); 

		freeMemPool(responseBuf_p);

		return RELAY_GTPU_FAILURE; 
	}

    freeMemPool(responseBuf_p);

    return RELAY_GTPU_SUCCESS;
}
/*spr 22474 end*/

