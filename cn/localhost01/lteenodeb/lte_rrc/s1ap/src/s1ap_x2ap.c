/*******************************************************************************
*
*  FILE NAME   : s1ap_x2ap.c
*
*  DESCRIPTION : This file contains the function definition of 
*
*  DATE 29-Jan-2013  NAME Simarjot Kaur  REFERENCE  REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/

#include "s1ap_api.h"
#include "s1ap_timers.h"
#include "s1ap_csc.h"
#include "rrc_x2apS1ap_intf.h"


/****************************************************************************
  Private Definitions
 ****************************************************************************/

/****************************************************************************
  Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
void process_x2ap_s1ap_comm_info(    
        void            *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context);

/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/


/*****************************************************************************
 * Function Name  : s1ap_process_x2ap_messages
 * Inputs         : p_api - Pointer to input API buffer 
 *                  p_s1ap_gb_context - Pointer to the S1AP global context data
 * Outputs        : None 
 * Returns        : None
 * Description    : This function processes the messages coming from 
 *                  X2AP module of RRC
 ****************************************************************************/
void s1ap_process_x2ap_messages(    
        void            *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context)
{
    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_api);

    RRC_ASSERT(PNULL != p_s1ap_gb_context);

    switch (rrc_get_api_id((U8*)p_api))
    {
        case X2AP_S1AP_COMM_INFO:
            {
                RRC_S1AP_TRACE(RRC_INFO, "Processing X2AP Messages : "
                        "X2AP_S1AP_COMM_INFO");
                process_x2ap_s1ap_comm_info(
                        p_api,
                        p_s1ap_gb_context);
                break;
            }
        default:
            {
                RRC_S1AP_TRACE(RRC_ERROR, "Processing CSC Messages : "
                        "Unknown API : %d",rrc_get_api_id((U8*)p_api));    
                    break;
            }
    }
    RRC_S1AP_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : process_x2ap_s1ap_comm_info
 * Inputs         : p_api - Pointer to input API buffer 
 *                  p_s1ap_gb_context - Pointer to the S1AP global context data
 * Outputs        : None 
 * Returns        : none
 * Description    : This function processes the s1ap_comm_info messsages 
 *                  coming from X2AP module of RRC
 ****************************************************************************/
void process_x2ap_s1ap_comm_info(    
        void            *p_api,                
        s1ap_gb_context_t    *p_s1ap_gb_context)
{
    x2ap_s1ap_comm_info_t *p_x2ap_s1ap_comm_info = PNULL;

    RRC_S1AP_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_s1ap_gb_context);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size((U8*)p_api) != RRC_API_HEADER_SIZE + 
            sizeof(x2ap_s1ap_comm_info_t))
    {
        RRC_S1AP_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_S1AP_UT_TRACE_EXIT();
        return;
    }

    p_x2ap_s1ap_comm_info = (x2ap_s1ap_comm_info_t*)((U8 *)p_api +
                                RRC_API_HEADER_SIZE);

    l3_memcpy_wrapper(&(p_s1ap_gb_context->x2_enb_comm_info),
            &(p_x2ap_s1ap_comm_info->local_enb_info),
            sizeof(x2_enb_comm_info_t));

    RRC_S1AP_UT_TRACE_EXIT();
}
