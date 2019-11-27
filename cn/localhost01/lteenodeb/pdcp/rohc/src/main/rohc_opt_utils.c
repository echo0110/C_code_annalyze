/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_opt_utils.c,v 1.8.26.3 2010/11/09 05:21:46 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : Contains Utility Functions for Stats and Trace api
 *                     handling.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_opt_utils.c,v $
 * Revision 1.8.26.3  2010/11/09 05:21:46  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.8.26.2  2010/10/13 03:54:06  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.8.26.1  2010/10/01 14:06:37  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.8  2010/06/09 11:52:35  gur19479
 * updated from intel branch
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.7.2.3  2009/07/13 11:07:22  gur20435
 *
 * PDCP Optimizations
 *
 * Revision 1.7.2.2  2009/07/10 09:07:20  gur19140
 * optimization changes for zcb
 *
 * Revision 1.7.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.7  2009/05/28 04:28:09  gur19836
 * Warning Removed
 *
 * Revision 1.6  2009/05/28 03:26:54  gur19836
 * File Header updated
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "rohc_def.h"
#include "rohc_opt_proto.h"
#include "rohc_com_def.h"
#include "rohc_g_typ.h"
#include "rohc_trace_mcr.h"
#include "rohc_typ.h"
#include "rohc_d_typ.h"
#include "rohc_dprm_ext_if.h"
#include "ltePdcpTxQueues.h"
#include "ltePdcpDataTransferTx.h"
#include "ltePdcpStatistic.h"
#include "rohc_stats_def.h"
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
static rohc_void_t rohcStatsUpdateIRPktTxStats (rohc_U8bit_t type);
static rohc_void_t rohcStatsUpdateUOPktTxStats (rohc_U8bit_t type);
static rohc_void_t rohcStatsUpdateRPktTxStats (rohc_U8bit_t type);
static rohc_void_t rohcStatsUpdateP6COAndRndPktTxStats (rohc_U8bit_t type);
static rohc_void_t rohcStatsUpdateP6SeqPktTxStats (rohc_U8bit_t type);

static rohc_void_t rohcStatsUpdateIRPktRxStats (rohc_U8bit_t type);
static rohc_void_t rohcStatsUpdateUOPktRxStats (rohc_U8bit_t type);
static rohc_void_t rohcStatsUpdateRPktRxStats (rohc_U8bit_t type);
static rohc_void_t rohcStatsUpdateP6COAndRndPktRxStats (rohc_U8bit_t type);
static rohc_void_t rohcStatsUpdateP6SeqPktRxStats (rohc_U8bit_t type);

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/



#ifdef ROHC_MODE_TRIGGER
rohc_return_t rohc_check_mode_transition_p1_required ROHC_PARAMS
    (  (p_rohc_entity, p_dsc),
        rohc_entity_t   *p_rohc_entity _AND_
        rohc_dsc_rtp_t      *p_dsc
    )
{
    rohc_U8bit_t ecode = ROHC_SUCCESS;
    switch(p_dsc->d_mode)
    {
        case ROHC_MODE_U:
           p_dsc->modeTransitionUtoO++;
           p_dsc->modeTransitionUtoR++;
           if(p_dsc->modeTransitionUtoO >= p_rohc_entity->modeTransitionUtoO)
           {
                ROHC_LOG(LOG_INFO, "Mode Transtion U->O is Triggered");
                p_dsc->modeTransitionUtoO = 0;
                rohc_exec_dprm_p1_md_transl
                    (p_rohc_entity, p_dsc,ROHC_MODE_O,&ecode);
           }
           else if(p_dsc->modeTransitionUtoR >= 
                                            p_rohc_entity->modeTransitionUtoR)
           {
                ROHC_LOG(LOG_INFO, "Mode Transtion U->R is Triggered");
                p_dsc->modeTransitionUtoR = 0;
                rohc_exec_dprm_p1_md_transl
                    (p_rohc_entity, p_dsc,ROHC_MODE_R,&ecode);
           }
           else
           {
                ROHC_LOG(LOG_INFO, "Mode Transtion is not required");
           }
           break;
        case ROHC_MODE_O:
           p_dsc->modeTransitionOtoU++;
           p_dsc->modeTransitionOtoR++;
           if (p_dsc->modeTransitionOtoU >= p_rohc_entity->modeTransitionOtoU)
           {
               ROHC_LOG(LOG_INFO, "Mode Transtion O->U is Triggered");
               p_dsc->modeTransitionOtoU = 0;
               rohc_exec_dprm_p1_md_transl
                   (p_rohc_entity, p_dsc,ROHC_MODE_U,&ecode);
           }
           else if (p_dsc->modeTransitionOtoR >= 
                                            p_rohc_entity->modeTransitionOtoR)
           {
               ROHC_LOG(LOG_INFO, "Mode Transtion O->R is Triggered");
               p_dsc->modeTransitionOtoR = 0;
               rohc_exec_dprm_p1_md_transl
                   (p_rohc_entity, p_dsc,ROHC_MODE_R,&ecode);
           }
           else
           {
               ROHC_LOG(LOG_INFO, "Mode Transtion is not required");
           }
           break;
        case ROHC_MODE_R:
           p_dsc->modeTransitionRtoU++;
           p_dsc->modeTransitionRtoO++;
           if (p_dsc->modeTransitionRtoU >= p_rohc_entity->modeTransitionRtoU)
           {
               ROHC_LOG(LOG_INFO, "Mode Transtion R->U is Triggered");
               p_dsc->modeTransitionRtoU = 0;
               rohc_exec_dprm_p1_md_transl
                   (p_rohc_entity, p_dsc,ROHC_MODE_U,&ecode);
           }
           else if (p_dsc->modeTransitionRtoO >= 
                                            p_rohc_entity->modeTransitionRtoO)
           {
               ROHC_LOG(LOG_INFO, "Mode Transtion R->O is Triggered");
               p_dsc->modeTransitionRtoO = 0;
               rohc_exec_dprm_p1_md_transl       
                  (p_rohc_entity, p_dsc,ROHC_MODE_O,&ecode);
           }
           else
           {
                ROHC_LOG(LOG_INFO, "Mode Transtion is not required");
           }
           break;
         default:
              ROHC_LOG(LOG_INFO, "Wrong Mode Transition is Requested");
    }
    return ROHC_SUCCESS;
}     
#endif

#ifdef ROHC_MODE_TRIGGER 

/*****************************************************************************
**      FUNCTION: rohc_check_mode_transition_p2_required
******************************************************************************
**
**      DESCRIPTION:
**              This funciton checks the mode of Transition required  
**      ARGUMENTS:
**              p_rohc_entity[IN]      Pointer to rohc entity 
**				p_dsc [IN]			   Pointer to Decompressor Context
**      RETURN VALUE:
**              none
**               
*****************************************************************************/
rohc_return_t rohc_check_mode_transition_p2_required ROHC_PARAMS
    (  (p_rohc_entity, p_dsc),
        rohc_entity_t   *p_rohc_entity _AND_
        rohc_dsc_udp_t  *p_dsc
    )
{
    rohc_U8bit_t ecode = ROHC_SUCCESS;
    switch(p_dsc->d_mode)
    {
        case ROHC_MODE_U:
           p_dsc->modeTransitionUtoO++;
           p_dsc->modeTransitionUtoR++;
           if(p_dsc->modeTransitionUtoO >= p_rohc_entity->modeTransitionUtoO)
           {
                ROHC_LOG(LOG_INFO, "Mode Transtion U->O is Triggered");
                p_dsc->modeTransitionUtoO = 0;
                rohc_exec_dprm_p2_md_transl
                    (p_rohc_entity, p_dsc,ROHC_MODE_O,&ecode);
           }
           else if(p_dsc->modeTransitionUtoR >= 
                                        p_rohc_entity->modeTransitionUtoR )
           {
                ROHC_LOG(LOG_INFO, "Mode Transtion U->R is Triggered");
                p_dsc->modeTransitionUtoR = 0;
                rohc_exec_dprm_p2_md_transl
                    (p_rohc_entity, p_dsc,ROHC_MODE_R,&ecode);
           }
           else
           {
                ROHC_LOG(LOG_INFO, "Mode Transtion is not required");
           }
           break;
        case ROHC_MODE_O:
           p_dsc->modeTransitionOtoU++;
           p_dsc->modeTransitionOtoR++;
           if (p_dsc->modeTransitionOtoU >= p_rohc_entity->modeTransitionOtoU)
           {
               ROHC_LOG(LOG_INFO, "Mode Transtion O->U is Triggered");
               p_dsc->modeTransitionOtoU = 0;
               rohc_exec_dprm_p2_md_transl
                   (p_rohc_entity, p_dsc,ROHC_MODE_U,&ecode);
           }
           else if (p_dsc->modeTransitionOtoR >= 
                                        p_rohc_entity->modeTransitionOtoR)
           {
               ROHC_LOG(LOG_INFO, "Mode Transtion O->R is Triggered");
               p_dsc->modeTransitionOtoR = 0;
               rohc_exec_dprm_p2_md_transl
                   (p_rohc_entity, p_dsc,ROHC_MODE_R,&ecode);
           }
           else
           {
               ROHC_LOG(LOG_INFO, "Mode Transtion is not required");
           }
           break;
        case ROHC_MODE_R:
           p_dsc->modeTransitionRtoU++;
           p_dsc->modeTransitionRtoO++;
           if (p_dsc->modeTransitionRtoU >= p_rohc_entity->modeTransitionRtoU)
           {
               ROHC_LOG(LOG_INFO, "Mode Transtion R->U is Triggered");
               p_dsc->modeTransitionRtoU = 0;
               rohc_exec_dprm_p2_md_transl
                   (p_rohc_entity, p_dsc,ROHC_MODE_U,&ecode);
           }
           else if (p_dsc->modeTransitionRtoO >= 
                                            p_rohc_entity->modeTransitionRtoO)
           {
               ROHC_LOG(LOG_INFO, "Mode Transtion R->O is Triggered");
               p_dsc->modeTransitionRtoO = 0;
               rohc_exec_dprm_p2_md_transl       
                  (p_rohc_entity, p_dsc,ROHC_MODE_O,&ecode);
           }
           else
           {
                ROHC_LOG(LOG_INFO, "Mode Transtion is not required");
           }
           break;
         default:
              ROHC_LOG(LOG_INFO, "Wrong Mode Transition is Requested");
    }
    return ROHC_SUCCESS;
}     

#endif

/*****************************************************************************
**      FUNCTION: rohc_send_feedback_to_pdcp
******************************************************************************
**
**      DESCRIPTION:
**              This funciton sends feedback to pdcp 
**      ARGUMENTS:
**              p_rohc_entity[IN]      Pointer to rohc entity
**      RETURN VALUE:
**              none
**               
*****************************************************************************/
rohc_U8bit_t rohc_send_feedback_to_pdcp ROHC_PARAMS
(
    (p_rohc_entity),
    rohc_entity_t   *p_rohc_entity
)
{
    rohc_void_t  *p_feedback = ROHC_NULL;
    rohc_U8bit_t *p_data = ROHC_NULL;
    p_feedback = msgAlloc(PNULL, 0, 0, 0);
    if ( PNULL == p_feedback )
    {
        ALARM_MSG (PDCP_MODULE_ID, 
                MEM_ALLOC_FAILED_ALARM_ID, 
                CRITICAL_ALARM);

        ROHC_LOG (LOG_INFO,"EID[%u:%u]:"
                " msgAlloc failed while making feedback\n",
                p_rohc_entity->rohc_entity_id.ueIndex,
                p_rohc_entity->rohc_entity_id.lcId);
        /* Klocwork Fix */
        ROHC_FREE(p_rohc_entity->fb_info.p_buff);
        p_rohc_entity->fb_info.p_buff = PNULL;

        ltePanic("%s: can't allocate buffer",__FUNCTION__);
        return ROHC_FAILURE;
    }
    p_data = msgReserve(p_feedback, 0,p_rohc_entity->fb_info.len);
    if ( PNULL == p_data )
    {
        ROHC_LOG (LOG_INFO,"EID[%u:%u]:"
                " msgAlloc failed while making feedback\n",
                p_rohc_entity->rohc_entity_id.ueIndex,
                p_rohc_entity->rohc_entity_id.lcId);

        msgFree( p_feedback);
        /* Klocwork Fix */
        ROHC_FREE(p_rohc_entity->fb_info.p_buff);
        p_rohc_entity->fb_info.p_buff = PNULL;
        ltePanic("%s: can't allocate buffer",__FUNCTION__);
        return ROHC_FAILURE;
    }
    ROHC_MEMCPY(p_data,p_rohc_entity->fb_info.p_buff,p_rohc_entity->fb_info.len);
   
    /* 
    ** SPR_1198_FIX_START: p_buff was not freed, which was causing a 
    ** memory leak.
    */
    ROHC_FREE(p_rohc_entity->fb_info.p_buff);
    p_rohc_entity->fb_info.p_buff = PNULL;
    /* SPR_1198_FIX_END */
 
    /* SRP 3608 changes start */
    /* + Coverity Id - 32623 */
    if(PNULL == pdcpStoreControlPduInd(p_rohc_entity->rohc_entity_id.ueIndex,
             p_rohc_entity->rohc_entity_id.lcId,PDCP_TX_CONTROL_PDU_IND,
              p_feedback,0x10))
    {
        /* Coverity 25198 Fix Start*/
        /*Code Removed*/
        /* Coverity 25198 Fix End*/
        return ROHC_FAILURE;
    }
    /* - Coverity Id - 32623 */
    /* SPR 3608 changes end */
    return ROHC_SUCCESS;
}

/* SPR 14084 fix start */
/* flag ROHC_MODE_TRIGGER has been removed */
/* SPR 14084 fix end */

/* SPR 5599_129036 Fix Start */
rohc_return_t rohcStatsModeTransUpdate ROHC_PARAMS 
/* SPR 5599_129036 Fix End */
    ( (type, dir),
        rohc_U8bit_t type _AND_
        rohc_U8bit_t dir
    )
{
    if(ROHC_DIR_TX == dir)
    {
        switch(type)
        {
            case ROHC_NO_OF_MODE_TRANS_U_TO_O_RCVD:
                pdcpStats.statsRohc.numberOfModeTransU2ORcvd++;
            break;
            case ROHC_NO_OF_MODE_TRANS_U_TO_R_RCVD:
                pdcpStats.statsRohc.numberOfModeTransU2RRcvd++;
            break;
            case ROHC_NO_OF_MODE_TRANS_O_TO_U_RCVD:
                pdcpStats.statsRohc.numberOfModeTransO2URcvd++;
            break;
            case ROHC_NO_OF_MODE_TRANS_O_TO_R_RCVD:
                pdcpStats.statsRohc.numberOfModeTransO2RRcvd++;
            break;
            case ROHC_NO_OF_MODE_TRANS_R_TO_U_RCVD:
                pdcpStats.statsRohc.numberOfModeTransR2URcvd++;
            break;
            case ROHC_NO_OF_MODE_TRANS_R_TO_O_RCVD:
                pdcpStats.statsRohc.numberOfModeTransR2ORcvd++;
            break;
            default:
                ROHC_LOG(LOG_INFO,
                  "Incorrect stats type passed, TYPE[%d]: DIR[%d]", type, dir);
        }
    }
    else
    {
        switch(type)
        {
            case ROHC_NO_OF_MODE_TRANS_U_TO_O_INIT:
                pdcpStats.statsRohc.numberOfModeTransU2OInit++;
            break;
            case ROHC_NO_OF_MODE_TRANS_U_TO_R_INIT:
                pdcpStats.statsRohc.numberOfModeTransU2RInit++;
            break;
            case ROHC_NO_OF_MODE_TRANS_O_TO_U_INIT:
                pdcpStats.statsRohc.numberOfModeTransO2UInit++;
            break;
             case ROHC_NO_OF_MODE_TRANS_O_TO_R_INIT:
                 pdcpStats.statsRohc.numberOfModeTransO2RInit++;
             break;
             case ROHC_NO_OF_MODE_TRANS_R_TO_U_INIT:
                 pdcpStats.statsRohc.numberOfModeTransR2UInit++;
             break;
             case ROHC_NO_OF_MODE_TRANS_R_TO_O_INIT:
                 pdcpStats.statsRohc.numberOfModeTransR2OInit++;
             break;
             default:
                ROHC_LOG(LOG_INFO,
                  "Incorrect stats type passed, TYPE[%d]: DIR[%d]", type, dir);
         }
    }
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION: rohcStatsPktTypeUpdate
******************************************************************************
**
**      DESCRIPTION:
**              This funciton updates stats 
**      ARGUMENTS:
**              type[IN]      Pointer to stats type
**              dir[IN]       Direction
**              profile[IN]   
**      RETURN VALUE:
**              none
**               
*****************************************************************************/
/* SPR 5599_129036 Fix Start */
rohc_return_t rohcStatsPktTypeUpdate ROHC_PARAMS 
/* SPR 5599_129036 Fix End */
    ( (type, dir,profile),
        rohc_U8bit_t type _AND_
        rohc_U8bit_t dir  _AND_
        rohc_U8bit_t profile
    )
{
    if(ROHC_DIR_TX == dir)
    {
        rohcStatsUpdateTxStats(type, profile);
    }
    else
    {
        rohcStatsUpdateRxStats(type, profile);
    }
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION: rohcStatsUpdateTxStats
******************************************************************************
**
**      DESCRIPTION:
**              This funciton updates Tx stats 
**      ARGUMENTS:
**              type[IN]      Pointer to stats type
**              profile[IN]   
**      RETURN VALUE:
**              none
**               
*****************************************************************************/
void rohcStatsUpdateTxStats(rohc_U8bit_t type, rohc_U8bit_t profile)
{
    if(ROHC_UNCOMP == profile)
    {
        if(ROHC_NO_OF_NORMALP0_CREATED == type)
        {
            pdcpStats.statsRohc.numberOfP0NormalPacketsCreated++;
        }
	    /* SPR 12654 fix start */
        rohcStatsUpdateIRPktTxStats(type);
	    /* SPR 12654 fix start */
    }
    else if((ROHC_IP_UDP_RTP == profile) || (ROHC_IP_UDP == profile))
    {
        rohcStatsUpdateIRPktTxStats(type);
        rohcStatsUpdateUOPktTxStats(type);
        rohcStatsUpdateRPktTxStats(type);
    }
    else
    { /* profile 6 */
        rohcStatsUpdateIRPktTxStats(type);
        rohcStatsUpdateP6COAndRndPktTxStats(type);
        rohcStatsUpdateP6SeqPktTxStats(type);
    }
}    

/*****************************************************************************
**      FUNCTION: rohcStatsUpdateRxStats
******************************************************************************
**
**      DESCRIPTION:
**              This funciton updates Rx stats
**      ARGUMENTS:
**              type[IN]      Pointer to stats type
**              profile[IN]   
**      RETURN VALUE:
**              none
**               
*****************************************************************************/
void rohcStatsUpdateRxStats(rohc_U8bit_t type, rohc_U8bit_t profile)
{
    if(ROHC_UNCOMP == profile)
    {
        /* SPR 12654 fix start */
        if(ROHC_NO_OF_NORMALP0_PROCESSED == type)
        {
        /* SPR 12654 fix end */
            pdcpStats.statsRohc.numberOfP0NormalPacketsProcessed++;
        }
        /* SPR 12654 fix start */
        rohcStatsUpdateIRPktRxStats(type);
        /* SPR 12654 fix end */
    }
    else if((ROHC_IP_UDP_RTP == profile) || (ROHC_IP_UDP == profile))
    {
        rohcStatsUpdateIRPktRxStats(type);
        rohcStatsUpdateUOPktRxStats(type);
        rohcStatsUpdateRPktRxStats(type);
    }
    else
    { /* profile 6 */
        rohcStatsUpdateIRPktRxStats(type);
        rohcStatsUpdateP6COAndRndPktRxStats(type);
        rohcStatsUpdateP6SeqPktRxStats(type);
    }
}

/*****************************************************************************
**      FUNCTION: rohcStatsUpdateUOPktTxStats
******************************************************************************
**
**      DESCRIPTION:
**              This funciton updates Tx stats for profile 1 ans 2 UO pkts
**      ARGUMENTS:
**              type[IN]      Pointer to stats type
**      RETURN VALUE:
**              none
**               
*****************************************************************************/
static rohc_void_t rohcStatsUpdateUOPktTxStats (rohc_U8bit_t type)
{
    switch(type)
    {
        case ROHC_NO_UO_0_CREATED:
            pdcpStats.statsRohc.numberOfUO0PacketsCreated++;
            break;
        case ROHC_NO_OF_UO_1_CREATED:
            pdcpStats.statsRohc.numberOfUO1PacketsCreated++;
            break;
        case ROHC_NO_UO_1_ID_CREATED:
            pdcpStats.statsRohc.numberOfUO1IDPacketsCreated++;
            break;
        case ROHC_NO_UO_1_TS_CREATED:
            pdcpStats.statsRohc.numberOfUO1TSPacketsCreated++;
            break;
        case ROHC_NO_OF_UOR_2_CREATED:
            pdcpStats.statsRohc.numberOfUOR2PacketsCreated++;
            break;
        case ROHC_NO_UOR_2_ID_CREATED:
            pdcpStats.statsRohc.numberOfUOR2IDPacketsCreated++;
            break;
        case ROHC_NO_UOR_2_TS_CREATED:
            pdcpStats.statsRohc.numberOfUOR2TSPacketsCreated++;
            break;
        default:
            break;
    }
}

/*****************************************************************************
**      FUNCTION: rohcStatsUpdateRPktTxStats
******************************************************************************
**
**      DESCRIPTION:
**              This funciton updates Tx stats for profile 1 and 2 R mode 
**      ARGUMENTS:
**              type[IN]      Pointer to stats type
**      RETURN VALUE:
**              none
**               
*****************************************************************************/
static rohc_void_t rohcStatsUpdateRPktTxStats (rohc_U8bit_t type)
{
    switch(type)
    {
        case ROHC_NO_OF_R_0_CREATED:
            pdcpStats.statsRohc.numberOfR0PacketsCreated++;
            break;
        case ROHC_NO_R_0_CRC_CREATED:
            pdcpStats.statsRohc.numberOfR0CRCPacketsCreated++;
            break;
        case ROHC_NO_OF_R_1_CREATED:
            pdcpStats.statsRohc.numberOfR1PacketsCreated++;
            break;
        case ROHC_NO_R_1_ID_CREATED:
            pdcpStats.statsRohc.numberOfR1IDPacketsCreated++;
            break;
        case ROHC_NO_R_1_TS_CREATED:
            pdcpStats.statsRohc.numberOfR1TSPacketsCreated++;
            break;
        default:
            break;
    }
}

/*****************************************************************************
**      FUNCTION: rohcStatsUpdateIRPktTxStats
******************************************************************************
**
**      DESCRIPTION:
**              This funciton updates Tx stats for IR pkts
**      ARGUMENTS:
**              type[IN]      Pointer to stats type
**      RETURN VALUE:
**              none
**               
*****************************************************************************/
static rohc_void_t rohcStatsUpdateIRPktTxStats (rohc_U8bit_t type)
{
    switch(type)
    {
        case ROHC_NO_OF_IR_CREATED:
            pdcpStats.statsRohc.numberOfIRPacketsCreated++;
            break;
        case ROHC_NO_OF_IR_DYN_CREATED:
            pdcpStats.statsRohc.numberOfIRDYNPacketsCreated++;
            break;
        case ROHC_NO_OF_IR_CR_P6_CREATED:
            pdcpStats.statsRohc.numberOfIRCRP6PacketsCreated++;
            break;
        default:
            break;
    }
}

/*****************************************************************************
**      FUNCTION: rohcStatsUpdateP6COAndRndPktTxStats
******************************************************************************
**
**      DESCRIPTION:
**              This funciton updates Tx stats for Profile6
**      ARGUMENTS:
**              type[IN]      Pointer to stats type
**      RETURN VALUE:
**              none
**               
*****************************************************************************/
static rohc_void_t rohcStatsUpdateP6COAndRndPktTxStats (rohc_U8bit_t type)
{
    switch(type)
    {
        case ROHC_NO_OF_CO_P6_CREATED:
            pdcpStats.statsRohc.numberOfCOP6PacketsCreated++;
            break;
        case ROHC_NO_OF_RND1_P6_CREATED:
            pdcpStats.statsRohc.numberOfRND1P6PacketsCreated++;
            break;
        case ROHC_NO_OF_RND2_P6_CREATED:
            pdcpStats.statsRohc.numberOfRND2P6PacketsCreated++;
            break;
        case ROHC_NO_OF_RND3_P6_CREATED:
            pdcpStats.statsRohc.numberOfRND3P6PacketsCreated++;
            break;
        case ROHC_NO_OF_RND4_P6_CREATED:
            pdcpStats.statsRohc.numberOfRND4P6PacketsCreated++;
            break;
        case ROHC_NO_OF_RND5_P6_CREATED:
            pdcpStats.statsRohc.numberOfRND5P6PacketsCreated++;
            break;
        case ROHC_NO_OF_RND6_P6_CREATED:
            pdcpStats.statsRohc.numberOfRND6P6PacketsCreated++;
            break;
        case ROHC_NO_OF_RND7_P6_CREATED:
            pdcpStats.statsRohc.numberOfRND7P6PacketsCreated++;
            break;
        case ROHC_NO_OF_RND8_P6_CREATED:
            pdcpStats.statsRohc.numberOfRND8P6PacketsCreated++;
            break;
        default:
            break;
    }
}

/*****************************************************************************
**      FUNCTION: rohcStatsUpdateP6SeqPktTxStats
******************************************************************************
**
**      DESCRIPTION:
**              This funciton updates Tx stats for profile6 seq pkts
**      ARGUMENTS:
**              type[IN]      Pointer to stats type
**      RETURN VALUE:
**              none
**               
*****************************************************************************/
static rohc_void_t rohcStatsUpdateP6SeqPktTxStats (rohc_U8bit_t type)
{
    switch(type)
    {
        case ROHC_NO_OF_SEQ1_P6_CREATED:
            pdcpStats.statsRohc.numberOfSEQ1P6PacketsCreated++;
            break;
        case ROHC_NO_OF_SEQ2_P6_CREATED:
            pdcpStats.statsRohc.numberOfSEQ2P6PacketsCreated++;
            break;
        case ROHC_NO_OF_SEQ3_P6_CREATED:
            pdcpStats.statsRohc.numberOfSEQ3P6PacketsCreated++;
            break;
        case ROHC_NO_OF_SEQ4_P6_CREATED:
            pdcpStats.statsRohc.numberOfSEQ4P6PacketsCreated++;
            break;
        case ROHC_NO_OF_SEQ5_P6_CREATED:
            pdcpStats.statsRohc.numberOfSEQ5P6PacketsCreated++;
            break;
        case ROHC_NO_OF_SEQ6_P6_CREATED:
            pdcpStats.statsRohc.numberOfSEQ6P6PacketsCreated++;
            break;
        case ROHC_NO_OF_SEQ7_P6_CREATED:
            pdcpStats.statsRohc.numberOfSEQ7P6PacketsCreated++;
            break;
        case ROHC_NO_OF_SEQ8_P6_CREATED:
            pdcpStats.statsRohc.numberOfSEQ8P6PacketsCreated++;
            break;
        default:
            break;
    }
}

/*****************************************************************************
**      FUNCTION: rohcStatsUpdateUOPktRxStats
******************************************************************************
**
**      DESCRIPTION:
**              This funciton updates Rx stats for profile 1 and 2
**      ARGUMENTS:
**              type[IN]      Pointer to stats type
**      RETURN VALUE:
**              none
**               
*****************************************************************************/
static rohc_void_t rohcStatsUpdateUOPktRxStats (rohc_U8bit_t type)
{
    switch(type)
    {
        case ROHC_NO_UO_0_PROCESSED:
            pdcpStats.statsRohc.numberOfUO0PacketsProcessed++;
            break;
        case ROHC_NO_OF_UO_1_PROCESSED:
            pdcpStats.statsRohc.numberOfUO1PacketsProcessed++;
            break;
        case ROHC_NO_UO_1_ID_PROCESSED:
            pdcpStats.statsRohc.numberOfUO1IDPacketsProcessed++;
            break;
        case ROHC_NO_UO_1_TS_PROCESSED:
            pdcpStats.statsRohc.numberOfUO1TSPacketsProcessed++;
            break;
        case ROHC_NO_OF_UOR_2_PROCESSED:
            pdcpStats.statsRohc.numberOfUOR2PacketsProcessed++;
            break;
        case ROHC_NO_UOR_2_ID_PROCESSED:
            pdcpStats.statsRohc.numberOfUOR2IDPacketsProcessed++;
            break;
        case ROHC_NO_UOR_2_TS_PROCESSED:
            pdcpStats.statsRohc.numberOfUOR2TSPacketsProcessed++;
            break;
        default:
            break;
    }
}

/*****************************************************************************
**      FUNCTION: rohcStatsUpdateRPktRxStats
******************************************************************************
**
**      DESCRIPTION:
**              This funciton updates Rx stats for profile 1 and 2 R mode
**      ARGUMENTS:
**              type[IN]      Pointer to stats type
**      RETURN VALUE:
**              none
**               
*****************************************************************************/
static rohc_void_t rohcStatsUpdateRPktRxStats (rohc_U8bit_t type)
{
    switch(type)
    {
        case ROHC_NO_OF_R_0_PROCESSED:
            pdcpStats.statsRohc.numberOfR0PacketsProcessed++;
            break;
        case ROHC_NO_R_0_CRC_PROCESSED:
            pdcpStats.statsRohc.numberOfR0CRCPacketsProcessed++;
            break;
        case ROHC_NO_OF_R_1_PROCESSED:
            pdcpStats.statsRohc.numberOfR1PacketsProcessed++;
            break;
        case ROHC_NO_R_1_ID_PROCESSED:
            pdcpStats.statsRohc.numberOfR1IDPacketsProcessed++;
            break;
        case ROHC_NO_R_1_TS_PROCESSED:
            pdcpStats.statsRohc.numberOfR1TSPacketsProcessed++;
            break;
        default:
            break;
    }
}

/*****************************************************************************
**      FUNCTION: rohcStatsUpdateIRPktRxStats
******************************************************************************
**
**      DESCRIPTION:
**              This funciton updates Rx stats for IR pkts
**      ARGUMENTS:
**              type[IN]      Pointer to stats type
**      RETURN VALUE:
**              none
**               
*****************************************************************************/
static rohc_void_t rohcStatsUpdateIRPktRxStats (rohc_U8bit_t type)
{
    switch(type)
    {
        case ROHC_NO_OF_IR_PROCESSED:
            pdcpStats.statsRohc.numberOfIRPacketsProcessed++;
            break;
        case ROHC_NO_OF_IR_DYN_PROCESSED:
            pdcpStats.statsRohc.numberOfIRDYNPacketsProcessed++;
            break;
        case ROHC_NO_OF_IR_CR_P6_PROCESSED:
            pdcpStats.statsRohc.numberOfIRCRP6PacketsProcessed++;
            break;
        default:
            break;
    }
}

/*****************************************************************************
**      FUNCTION: rohcStatsUpdateP6COAndRndPktRxStats
******************************************************************************
**
**      DESCRIPTION:
**              This funciton updates Rx stats profile6
**      ARGUMENTS:
**              type[IN]      Pointer to stats type
**      RETURN VALUE:
**              none
**               
*****************************************************************************/
static rohc_void_t rohcStatsUpdateP6COAndRndPktRxStats (rohc_U8bit_t type)
{
    switch(type)
    {
        case ROHC_NO_OF_CO_P6_PROCESSED:
            pdcpStats.statsRohc.numberOfCOP6PacketsProcessed++;
            break;
        case ROHC_NO_OF_RND1_P6_PROCESSED:
            pdcpStats.statsRohc.numberOfRND1P6PacketsProcessed++;
            break; 
        case ROHC_NO_OF_RND2_P6_PROCESSED:
            pdcpStats.statsRohc.numberOfRND2P6PacketsProcessed++;
            break; 
        case ROHC_NO_OF_RND3_P6_PROCESSED:
            pdcpStats.statsRohc.numberOfRND3P6PacketsProcessed++;
            break; 
        case ROHC_NO_OF_RND4_P6_PROCESSED:
            pdcpStats.statsRohc.numberOfRND4P6PacketsProcessed++;
            break; 
        case ROHC_NO_OF_RND5_P6_PROCESSED:
            pdcpStats.statsRohc.numberOfRND5P6PacketsProcessed++;
            break; 
        case ROHC_NO_OF_RND6_P6_PROCESSED:
            pdcpStats.statsRohc.numberOfRND6P6PacketsProcessed++;
            break; 
        case ROHC_NO_OF_RND7_P6_PROCESSED:
            pdcpStats.statsRohc.numberOfRND7P6PacketsProcessed++;
            break; 
        case ROHC_NO_OF_RND8_P6_PROCESSED:
            pdcpStats.statsRohc.numberOfRND8P6PacketsProcessed++;
            break; 
        default:
            break;
    }
}

/*****************************************************************************
**      FUNCTION: rohcStatsUpdateP6SeqPktRxStats
******************************************************************************
**
**      DESCRIPTION:
**              This funciton updates Rx stats for profile6 seq pkts
**      ARGUMENTS:
**              type[IN]      Pointer to stats type
**      RETURN VALUE:
**              none
**               
*****************************************************************************/
static rohc_void_t rohcStatsUpdateP6SeqPktRxStats (rohc_U8bit_t type)
{
    switch(type)
    {
        case ROHC_NO_OF_SEQ1_P6_PROCESSED:
            pdcpStats.statsRohc.numberOfSEQ1P6PacketsProcessed++;
            break; 
        case ROHC_NO_OF_SEQ2_P6_PROCESSED:
            pdcpStats.statsRohc.numberOfSEQ2P6PacketsProcessed++;
            break; 
        case ROHC_NO_OF_SEQ3_P6_PROCESSED:
            pdcpStats.statsRohc.numberOfSEQ3P6PacketsProcessed++;
            break; 
        case ROHC_NO_OF_SEQ4_P6_PROCESSED:
            pdcpStats.statsRohc.numberOfSEQ4P6PacketsProcessed++;
            break; 
        case ROHC_NO_OF_SEQ5_P6_PROCESSED:
            pdcpStats.statsRohc.numberOfSEQ5P6PacketsProcessed++;
            break; 
        case ROHC_NO_OF_SEQ6_P6_PROCESSED:
            pdcpStats.statsRohc.numberOfSEQ6P6PacketsProcessed++;
            break; 
        case ROHC_NO_OF_SEQ7_P6_PROCESSED:
            pdcpStats.statsRohc.numberOfSEQ7P6PacketsProcessed++;
            break; 
        case ROHC_NO_OF_SEQ8_P6_PROCESSED:
            pdcpStats.statsRohc.numberOfSEQ8P6PacketsProcessed++;
            break; 
        default:
            break;
    }
}
