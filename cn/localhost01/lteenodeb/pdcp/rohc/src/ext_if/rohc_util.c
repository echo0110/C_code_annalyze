/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_util.c,v 1.1.1.1.16.1 2010/11/03 06:55:58 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains utility functions 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_util.c,v $
 * Revision 1.1.1.1.16.1  2010/11/03 06:55:58  gur22059
 * ROHC Merge for Profile-0 and 2
 *
 * Revision 1.6  2009/05/29 00:59:26  gur19479
 * updated for timer
 *
 * Revision 1.5  2009/05/27 15:56:38  gur19836
 * File Header and Function Headers updated
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteFramework.h"
#include "rohc_com_def.h"
#include "rohc_def.h"
#include "rohc_typ.h"
#include "rohc_ex.h"
#include "rohc_ccpm_ext_if.h"
#include "rohc_ccpm_event.h"
#include "rohcTimer.h"
#include "rohc_trace_mcr.h"
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

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/




/*****************************************************************************
 * Function Name  : rohc_mem_get
 * Inputs         : size - Number of bytes required from pool
 * Outputs        : 
 * Returns        : Pointer to memory allocated
 * Description    : Gets Memory from pool
 *****************************************************************************/
rohc_void_t *rohc_mem_get   ROHC_PARAMS ((size), rohc_U32bit_t  size)
{
    void    *p_buf = ROHC_NULL;

	/* + Coverity 73152 */
	if ( size > 0 )
	{
    	p_buf = getMemFromPool(size, ROHC_NULL);
	}
	/* - Coverity 73152 */
    if(!p_buf)
    {
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
    }

    return p_buf; 

}

/*****************************************************************************
 * Function Name  : rohc_mem_free
 * Inputs         : p_buf - Pointer to buffer to be freed
 * Outputs        : 
 * Returns        : None
 * Description    : Frees memory to pool
 *****************************************************************************/
rohc_void_t rohc_mem_free   ROHC_PARAMS ((p_buf), rohc_void_tp  p_buf)
{
    if (p_buf == ROHC_NULL)
        return;
    freeMemPool(p_buf);
}


/*****************************************************************************
 * Function Name  : rohc_start_timer
 * Inputs         : 
 * Outputs        : 
 * Returns        : None
 * Description    : Starts timer 
 *****************************************************************************/
rohc_void_t rohc_start_timer ROHC_PARAMS ( (td, duration, tid_p),
                                        rohc_timer_data_t *td _AND_    
                                        rohc_U32bit_t duration _AND_
                                        rohc_timer_id_t* tid_p
                                        )
{
    *tid_p = startRohcTimer(duration, td->timer_type,td);
    if (PNULL == *tid_p)
    {
        ROHC_LOG(LOG_WARNING,"ROHC Timer could not started");
        return;
    }
}

/*****************************************************************************
 * Function Name  : rohc_stop_timer
 * Inputs         : 
 * Outputs        : 
 * Returns        : None
 * Description    : Stops Timer
 *****************************************************************************/
rohc_void_t rohc_stop_timer ROHC_PARAMS ((tdata, tid),
                                           rohc_timer_data_t tdata _AND_ void* tid )
{
    if(PNULL == tid)
        return;
    stopRohcTimer(tdata.timer_type,tid); 
}

/*****************************************************************************
 * Function Name  : rohc_timer_handler
 * Inputs         : 
 * Outputs        : 
 * Returns        : None
 * Description    : Handles timer
 *****************************************************************************/
rohc_void_t rohc_timer_handler(void *td)
{
    rohc_error_code_t ecode;
    rohc_timer_data_t *tdata = (rohc_timer_data_t*)td;

    /*
    ** START_SPR_998_FIX : Introduced the functionality of timer
    ** expiry for ROHC.
    */
    switch(((rohc_csc_common_t *)(tdata->p_csc_ptr))->profile)
    {
        case ROHC_IP_UDP_RTP:
        {    
            if(tdata->timer_type == ROHC_IR_RET_TIMER)
            {
                rohc_ccpm_p1_exec_direct_evt ((rohc_entity_t*)(tdata->p_entity_ptr),
                                              (rohc_csc_rtp_t*)(tdata->p_csc_ptr),
                                              ROHC_C_U_IR_RTN_TMR_ET, &ecode );    
            }
            else if (tdata->timer_type ==ROHC_FO_RET_TIMER)
            {
                rohc_ccpm_p1_exec_direct_evt ((rohc_entity_t*)(tdata->p_entity_ptr),
                                              (rohc_csc_rtp_t*)(tdata->p_csc_ptr),
                                              ROHC_C_U_FO_RTN_TMR_ET, &ecode );    
    
            }
            else
            {
                ROHC_LOG(LOG_INFO, "Timer data in not correct");
            }
            break;
        }

        case ROHC_IP_UDP:
        {    
            if(tdata->timer_type == ROHC_IR_RET_TIMER)
            {
                rohc_ccpm_p2_exec_direct_evt ((rohc_entity_t*)(tdata->p_entity_ptr),
                                              (rohc_csc_udp_t*)(tdata->p_csc_ptr),
                                              ROHC_C_U_IR_RTN_TMR_ET, &ecode );    
            }
            else if (tdata->timer_type ==ROHC_FO_RET_TIMER)
            {
                rohc_ccpm_p2_exec_direct_evt ((rohc_entity_t*)(tdata->p_entity_ptr),
                                              (rohc_csc_udp_t*)(tdata->p_csc_ptr),
                                              ROHC_C_U_FO_RTN_TMR_ET, &ecode );    
    
            }
            else
            {
                ROHC_LOG(LOG_INFO, "Timer data in not correct");
            }
            break;
        }

        case ROHC_IP_TCP:
        {    
            if(tdata->timer_type == ROHC_IR_RET_TIMER)
            {
                rohc_ccpm_p6_exec_direct_evt ((rohc_entity_t*)(tdata->p_entity_ptr),
                                              (rohc_csc_tcp_t*)(tdata->p_csc_ptr),
                                              ROHC_C_U_IR_RTN_TMR_ET, &ecode );    
            }
            else if (tdata->timer_type ==ROHC_FO_RET_TIMER)
            {
                rohc_ccpm_p6_exec_direct_evt ((rohc_entity_t*)(tdata->p_entity_ptr),
                                              (rohc_csc_tcp_t*)(tdata->p_csc_ptr),
                                              ROHC_C_U_FO_RTN_TMR_ET, &ecode );    
    
            }
            else
            {
                ROHC_LOG(LOG_INFO, "Timer data in not correct");
            }
            break;
        }
        case ROHC_UNCOMP:
        {
            ROHC_LOG(LOG_INFO,"RoHC Timer not started in Uncompressed profile \n");
            break;
        }
        
        default:
            ROHC_LOG(LOG_INFO, "rohc_timer_handler failed");
    }/*End of switch*/
    /* END_SPR_998_FIX */
}   
