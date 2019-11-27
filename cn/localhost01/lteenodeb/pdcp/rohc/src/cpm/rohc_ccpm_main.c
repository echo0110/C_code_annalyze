/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_ccpm_main.c,v 1.4.32.3 2010/11/09 04:55:43 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file provides implementation for C-CPM function
 *                     pointers table initialization and main entry point
 *                     function.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_ccpm_main.c,v $
 * Revision 1.4.32.3  2010/11/09 04:55:43  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.4.32.2  2010/10/13 03:35:03  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.4.32.1  2010/09/23 04:57:52  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.4  2009/05/27 07:29:45  gur19836
 * File Headers and Function Headers updated
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "rohc_ccpm_proto.h"

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
/* SPR 14084 fix start */
/* flag ROHC_MODE_TRIGGER removed */
/* SPR 14084 fix end */

     /* - SPR 17439 */
rohc_ccpm_fprtable_fpt_rtp rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U +1 ]
        [ROHC_C_TST_PENDING - ROHC_C_TST_DONE + 1 ]
        [ROHC_C_U_MAX_ET]
        [ROHC_C_ST_SO - ROHC_C_ST_IR + 1]
	/* SPR 3444 changes start */
        [ROHC_ALL_PROFILE - ROHC_UNCOMP + 1];
	/* SPR 3444 changes end */
rohc_ccpm_fprtable_fpt_udp rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U +1 ]
        [ROHC_C_TST_PENDING - ROHC_C_TST_DONE + 1 ]
        [ROHC_C_U_MAX_ET]
        [ROHC_C_ST_SO - ROHC_C_ST_IR + 1]
	/* SPR 3444 changes start */
        [ROHC_ALL_PROFILE - ROHC_UNCOMP + 1];
	/* SPR 3444 changes end */
rohc_ccpm_fprtable_fpt_tcp rohc_ccpm_fpt_tcp[ROHC_MODE_R - ROHC_MODE_U +1 ]
        [ROHC_C_TST_PENDING - ROHC_C_TST_DONE + 1 ]
        [ROHC_C_U_MAX_ET]
        [ROHC_C_ST_SO - ROHC_C_ST_IR + 1]
	/* SPR 3444 changes start */
        [ROHC_ALL_PROFILE - ROHC_UNCOMP + 1];
	/* SPR 3444 changes end */

     /* - SPR 17439 */
rohc_ccpm_fprtable rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U +1 ]
        [ROHC_C_TST_PENDING - ROHC_C_TST_DONE + 1 ]
        [ROHC_C_U_MAX_ET]
        [ROHC_C_ST_SO - ROHC_C_ST_IR + 1];
                               
rohc_U8bit_t rohc_ccpm_map_tbl[ROHC_MAX_PKT_TYPES+1][3];  

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/



/*****************************************************************************
 * Function Name  : rohc_ccpm_init_timer_data 
 * Inputs         : p_entity - ROHC Entity
 *                  p_csc - Pointer to CSC
 * Outputs        : None
 * Returns        : ROHC_SUCCESS or ROHC_FAILURE
 * Description    : This function initializes the IR return and fo return
 *                  timer data, i.e. used in U-mode
 *****************************************************************************/
rohc_return_t rohc_ccpm_init_timer_data ROHC_PARAMS(
        ( p_entity ,p_csc),
        rohc_entity_t *p_entity _AND_ 
        rohc_csc_t *p_csc 
        )
{   
    ROHC_ENTER_FN("rohc_ccpm_init_timer_data");
    
    p_csc->ir_ret_timer.tdata.timer_type = ROHC_IR_RET_TIMER;
    p_csc->ir_ret_timer.tdata.p_entity_ptr = p_entity;
    p_csc->ir_ret_timer.tdata.p_csc_ptr = p_csc;

    p_csc->fo_ret_timer.tdata.timer_type = ROHC_FO_RET_TIMER;
    p_csc->fo_ret_timer.tdata.p_entity_ptr = p_entity;
    p_csc->fo_ret_timer.tdata.p_csc_ptr = p_csc;
        
    ROHC_EXIT_FN("rohc_ccpm_init_timer_data");
    return ROHC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : rohc_ccpm_init 
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function initializes the CCPM function pointer table.
 *                  The table is of 4-dimension where 
 *                      - 1st-dim specifies compressor Mode
 *                      - 2nd-dim specifies compressor's transition state
 *                      - 3rd-dim parameter is a packet type or received
 *                        feedback type (NACK , ACK ,MODE TRANSITION)
 *                      - 4th-dim specifies compressor's  state
 *****************************************************************************/
rohc_void_t rohc_ccpm_init ROHC_NO_PARAMS()

{
    ROHC_ENTER_FN("rohc_ccpm_init");
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_u_ir_ir_dn;                           
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = ROHC_NULL;                           
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IRDYN_ET - ROHC_C_U_START]            
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IRDYN_ET - ROHC_C_U_START]            
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_u_irdyn_fo_dn;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IRDYN_ET - ROHC_C_U_START]            
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_UO_0_ET - ROHC_C_U_START]             
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                         
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_UO_0_ET - ROHC_C_U_START]             
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = ROHC_NULL;                         
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_UO_0_ET - ROHC_C_U_START]             
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = rohc_c_u_uo_0_so_dn;                
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_UO_1_ET - ROHC_C_U_START]             
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL; 
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_UO_1_ET - ROHC_C_U_START]             
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_u_irdyn_fo_dn;                         
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_UO_1_ET - ROHC_C_U_START]             
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_UO_2_ET - ROHC_C_U_START]             
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_UO_2_ET - ROHC_C_U_START]             
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_u_irdyn_fo_dn;                         
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_UO_2_ET - ROHC_C_U_START]             
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IR_RTN_TMR_ET - ROHC_C_U_START]       
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_u_ir_rtn_tmr_ir_dn;                   
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IR_RTN_TMR_ET - ROHC_C_U_START]       
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_u_ir_rtn_tmr_fo_dn;                   
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IR_RTN_TMR_ET - ROHC_C_U_START]       
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = rohc_c_u_ir_rtn_tmr_so_dn;                   
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_FO_RTN_TMR_ET - ROHC_C_U_START]       
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_u_fo_rtn_tmr_ir_dn;                   
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_FO_RTN_TMR_ET - ROHC_C_U_START]       
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_u_fo_rtn_tmr_fo_dn;                   
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_FO_RTN_TMR_ET - ROHC_C_U_START]       
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = rohc_c_u_fo_rtn_tmr_so_dn;                   
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_ACK_U_ET - ROHC_C_U_START]            
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_u_ack_u_ir_dn;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_ACK_U_ET - ROHC_C_U_START]            
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_u_ack_u_ir_dn;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_ACK_U_ET - ROHC_C_U_START]            
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = rohc_c_u_ack_u_ir_dn;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_MT_U_TO_O_ET - ROHC_C_U_START]        
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_u_mt_u_to_o_ir_dn;                    
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_MT_U_TO_O_ET - ROHC_C_U_START]        
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_u_mt_u_to_o_fo_dn;                    
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_MT_U_TO_O_ET - ROHC_C_U_START]        
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = rohc_c_u_mt_u_to_o_so_dn;                    
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_MT_U_TO_R_ET - ROHC_C_U_START]        
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_u_mt_u_to_r_ir_dn;                    
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_MT_U_TO_R_ET - ROHC_C_U_START]        
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_u_mt_u_to_r_fo_dn;                    
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_MT_U_TO_R_ET - ROHC_C_U_START]        
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = rohc_c_u_mt_u_to_r_so_dn;    
                                
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_IR_ET - ROHC_C_U_START]            
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_u_ir_ir_dn;                          
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_IR_ET - ROHC_C_U_START]            
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = ROHC_NULL;                          
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_IR_ET - ROHC_C_U_START]            
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                          
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_IRDYN_ET - ROHC_C_U_START]         
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                       
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_IRDYN_ET - ROHC_C_U_START]         
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_u_irdyn_fo_pnd;                       
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_IRDYN_ET - ROHC_C_U_START]         
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                       
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_UO_0_ET - ROHC_C_U_START]          
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_UO_0_ET - ROHC_C_U_START]          
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = ROHC_NULL;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_UO_0_ET - ROHC_C_U_START]          
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_UO_1_ET - ROHC_C_U_START]          
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_UO_1_ET - ROHC_C_U_START]          
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = ROHC_NULL;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_UO_1_ET - ROHC_C_U_START]          
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_UO_2_ET - ROHC_C_U_START]          
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_UO_2_ET - ROHC_C_U_START]          
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_u_uo_2_fo_pnd;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_UO_2_ET - ROHC_C_U_START]          
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_IR_RTN_TMR_ET - ROHC_C_U_START]    
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_u_ir_rtn_tmr_ir_dn;                  
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_IR_RTN_TMR_ET - ROHC_C_U_START]    
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_u_ir_rtn_tmr_fo_dn;                  
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_IR_RTN_TMR_ET - ROHC_C_U_START]    
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                  
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_FO_RTN_TMR_ET - ROHC_C_U_START]    
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                  
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_FO_RTN_TMR_ET - ROHC_C_U_START]    
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = ROHC_NULL;                  
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_FO_RTN_TMR_ET - ROHC_C_U_START]    
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                  
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_ACK_U_ET - ROHC_C_U_START]         
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_u_ack_u_ir_pnd;                       
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_ACK_U_ET - ROHC_C_U_START]         
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_u_ack_u_ir_pnd;                       
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_ACK_U_ET - ROHC_C_U_START]         
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                       
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_MT_U_TO_O_ET - ROHC_C_U_START]     
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                   
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_MT_U_TO_O_ET - ROHC_C_U_START]     
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = ROHC_NULL;                   
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_MT_U_TO_O_ET - ROHC_C_U_START]     
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                   
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_MT_U_TO_R_ET - ROHC_C_U_START]     
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                   
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_MT_U_TO_R_ET - ROHC_C_U_START]     
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = ROHC_NULL;                   
    rohc_ccpm_gen_fpt[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_MT_U_TO_R_ET - ROHC_C_U_START]     
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                   
    
#ifdef ROHC_MODE_TRIGGER 
    
                                                                         
    /*Funtion pointer table initialization for O-mode */                      
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_o_ir_ir_dn;                           
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = ROHC_NULL;                           
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                           
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_IRDYN_ET - ROHC_C_U_START]            
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_IRDYN_ET - ROHC_C_U_START]            
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_o_irdyn_fo_dn;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_IRDYN_ET - ROHC_C_U_START]            
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_UO_0_ET - ROHC_C_U_START]             
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                         
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_UO_0_ET - ROHC_C_U_START]             
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = ROHC_NULL;                         
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_UO_0_ET - ROHC_C_U_START]             
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = rohc_c_o_uo_0_so_dn;                         
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_UO_1_ET - ROHC_C_U_START]             
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                         
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_UO_1_ET - ROHC_C_U_START]             
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_o_irdyn_fo_dn;                         
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_UO_1_ET - ROHC_C_U_START]             
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                         
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_UO_2_ET - ROHC_C_U_START]             
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                         
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_UO_2_ET - ROHC_C_U_START]             
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_o_irdyn_fo_dn;                         
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_UO_2_ET - ROHC_C_U_START]             
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                         
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_ACK_O_ET - ROHC_C_U_START]            
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_o_ack_o_ir_dn;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_ACK_O_ET - ROHC_C_U_START]            
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_o_ack_o_fo_dn;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_ACK_O_ET - ROHC_C_U_START]            
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = rohc_c_o_ack_o_so_dn;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_STATIC_NACK_O_ET - ROHC_C_U_START]    
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_o_static_nack_o_ir_dn;                
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_STATIC_NACK_O_ET - ROHC_C_U_START]    
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_o_static_nack_o_fo_dn;                
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_STATIC_NACK_O_ET - ROHC_C_U_START]    
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = rohc_c_o_static_nack_o_fo_dn;                
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_NACK_O_ET - ROHC_C_U_START]           
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_o_nack_o_ir_dn;                       
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_NACK_O_ET - ROHC_C_U_START]           
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_o_nack_o_fo_dn;                       
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_NACK_O_ET - ROHC_C_U_START]           
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = rohc_c_o_nack_o_so_dn;                       
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_MT_O_TO_U_ET - ROHC_C_U_START]        
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_o_mt_o_to_u_ir_dn;                    
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_MT_O_TO_U_ET - ROHC_C_U_START]        
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_o_mt_o_to_u_fo_dn;                    
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_MT_O_TO_U_ET - ROHC_C_U_START]        
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = rohc_c_o_mt_o_to_u_so_dn;                    
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_MT_O_TO_R_ET - ROHC_C_U_START]        
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_o_mt_o_to_r_ir_dn;                    
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_MT_O_TO_R_ET - ROHC_C_U_START]        
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_o_mt_o_to_r_fo_dn;                    
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_MT_O_TO_R_ET - ROHC_C_U_START]        
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = rohc_c_o_mt_o_to_r_so_dn; 
                
                                   
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_IR_ET - ROHC_C_U_START]            
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_o_ir_ir_dn;                          
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_IR_ET - ROHC_C_U_START]            
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = ROHC_NULL;                          
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_IR_ET - ROHC_C_U_START]            
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                          
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_IRDYN_ET - ROHC_C_U_START]         
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                       
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_IRDYN_ET - ROHC_C_U_START]         
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_o_irdyn_fo_pnd;                       
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_IRDYN_ET - ROHC_C_U_START]         
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                       
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_UO_0_ET - ROHC_C_U_START]          
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_UO_0_ET - ROHC_C_U_START]          
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = ROHC_NULL;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_UO_0_ET - ROHC_C_U_START]          
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_UO_1_ET - ROHC_C_U_START]          
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_UO_1_ET - ROHC_C_U_START]          
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = ROHC_NULL;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_UO_1_ET - ROHC_C_U_START]          
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_UO_2_ET - ROHC_C_U_START]          
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_UO_2_ET - ROHC_C_U_START]          
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_o_irdyn_fo_pnd;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_UO_2_ET - ROHC_C_U_START]          
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_ACK_O_ET - ROHC_C_U_START]         
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_o_ack_o_ir_pnd;                       
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_ACK_O_ET - ROHC_C_U_START]         
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_o_ack_o_fo_pnd;                       
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_ACK_O_ET - ROHC_C_U_START]         
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                       
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_STATIC_NACK_O_ET - ROHC_C_U_START] 
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_o_static_nack_o_ir_pnd;               
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_STATIC_NACK_O_ET - ROHC_C_U_START] 
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_o_static_nack_o_fo_pnd;               
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_STATIC_NACK_O_ET - ROHC_C_U_START] 
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;               
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_NACK_O_ET - ROHC_C_U_START]        
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_o_nack_o_ir_pnd;                      
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_NACK_O_ET - ROHC_C_U_START]        
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_o_nack_o_fo_pnd;                      
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_NACK_O_ET - ROHC_C_U_START]        
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                      
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_MT_O_TO_U_ET - ROHC_C_U_START]     
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                   
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_MT_O_TO_U_ET - ROHC_C_U_START]     
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = ROHC_NULL;                   
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_MT_O_TO_U_ET - ROHC_C_U_START]     
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                   
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_MT_O_TO_R_ET - ROHC_C_U_START]     
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                   
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_MT_O_TO_R_ET - ROHC_C_U_START]     
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = ROHC_NULL;                   
    rohc_ccpm_gen_fpt[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_MT_O_TO_R_ET - ROHC_C_U_START]     
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                   
    
                                                                         
    /*Funtion pointer table initialization for R-mode */                      
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_r_ir_ir_dn;                           
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = ROHC_NULL;                           
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                           
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_IRDYN_ET - ROHC_C_U_START]            
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_IRDYN_ET - ROHC_C_U_START]            
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_r_irdyn_fo_dn;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_IRDYN_ET - ROHC_C_U_START]            
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_0_ET - ROHC_C_U_START]              
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                          
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_0_ET - ROHC_C_U_START]              
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = ROHC_NULL;                          
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_0_ET - ROHC_C_U_START]              
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = rohc_c_r_r_0_so_dn;                          
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_0_CRC_ET - ROHC_C_U_START]          
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                      
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_0_CRC_ET - ROHC_C_U_START]          
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = ROHC_NULL;                      
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_0_CRC_ET - ROHC_C_U_START]          
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = rohc_c_r_r_0_crc_so_dn;                      
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_1_ET - ROHC_C_U_START]              
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                          
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_1_ET - ROHC_C_U_START]              
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_r_r_1_fo_dn;                          
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_1_ET - ROHC_C_U_START]              
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                          
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_2_ET - ROHC_C_U_START]              
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                          
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_2_ET - ROHC_C_U_START]              
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_r_r_2_fo_dn;                          
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_2_ET - ROHC_C_U_START]              
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                          
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_ACK_R_ET - ROHC_C_U_START]            
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_r_ack_r_ir_dn;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_ACK_R_ET - ROHC_C_U_START]            
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_r_ack_r_fo_dn;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_ACK_R_ET - ROHC_C_U_START]            
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = rohc_c_r_ack_r_so_dn;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_STATIC_NACK_ET - ROHC_C_U_START]      
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_r_static_nack_ir_dn;        
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_STATIC_NACK_ET - ROHC_C_U_START]      
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_r_static_nack_fo_dn;                  
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_STATIC_NACK_ET - ROHC_C_U_START]      
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = rohc_c_r_static_nack_so_dn;                  
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_NACK_ET - ROHC_C_U_START]             
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_r_nack_ir_dn;                         
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_NACK_ET - ROHC_C_U_START]             
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_r_nack_fo_dn;                         
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_NACK_ET - ROHC_C_U_START]             
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = rohc_c_r_nack_so_dn;                         
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_MT_R_TO_U_ET - ROHC_C_U_START]        
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_r_mt_r_to_u_ir_dn;                    
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_MT_R_TO_U_ET - ROHC_C_U_START]        
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_r_mt_r_to_u_fo_dn;                    
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_MT_R_TO_U_ET - ROHC_C_U_START]        
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = rohc_c_r_mt_r_to_u_so_dn;                    
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_MT_R_TO_O_ET - ROHC_C_U_START]        
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_r_mt_r_to_o_ir_dn;                    
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_MT_R_TO_O_ET - ROHC_C_U_START]        
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_r_mt_r_to_o_fo_dn;                    
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_MT_R_TO_O_ET - ROHC_C_U_START]        
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = rohc_c_r_mt_r_to_o_so_dn; 
              
                                   
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_IR_ET - ROHC_C_U_START]            
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_r_ir_ir_pnd;                          
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_IR_ET - ROHC_C_U_START]            
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = ROHC_NULL;                          
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_IR_ET - ROHC_C_U_START]            
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                          
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_IRDYN_ET - ROHC_C_U_START]         
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                       
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_IRDYN_ET - ROHC_C_U_START]         
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_r_irdyn_fo_pnd;                       
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_IRDYN_ET - ROHC_C_U_START]         
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                       
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_0_ET - ROHC_C_U_START]           
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                         
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_0_ET - ROHC_C_U_START]           
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = ROHC_NULL;                         
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_0_ET - ROHC_C_U_START]           
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                         
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_0_CRC_ET - ROHC_C_U_START]       
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                     
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_0_CRC_ET - ROHC_C_U_START]       
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = ROHC_NULL;                     
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_0_CRC_ET - ROHC_C_U_START]       
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                     
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_1_ET - ROHC_C_U_START]           
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                         
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_1_ET - ROHC_C_U_START]           
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = ROHC_NULL;                         
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_1_ET - ROHC_C_U_START]           
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                         
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_2_ET - ROHC_C_U_START]           
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                         
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_2_ET - ROHC_C_U_START]           
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_r_r_2_fo_pnd;                         
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_2_ET - ROHC_C_U_START]           
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                         
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_ACK_R_ET - ROHC_C_U_START]         
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_r_ack_r_ir_pnd;                       
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_ACK_R_ET - ROHC_C_U_START]         
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_r_ack_r_ir_pnd;                       
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_ACK_R_ET - ROHC_C_U_START]         
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                       
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_STATIC_NACK_ET - ROHC_C_U_START]   
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_r_static_nack_ir_pnd;                 
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_STATIC_NACK_ET - ROHC_C_U_START]   
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_r_static_nack_fo_pnd;                 
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_STATIC_NACK_ET - ROHC_C_U_START]   
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                 
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_NACK_ET - ROHC_C_U_START]          
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = rohc_c_r_nack_ir_pnd;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_NACK_ET - ROHC_C_U_START]          
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = rohc_c_r_nack_fo_pnd;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_NACK_ET - ROHC_C_U_START]          
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                        
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_MT_R_TO_U_ET - ROHC_C_U_START]     
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                   
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_MT_R_TO_U_ET - ROHC_C_U_START]     
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = ROHC_NULL;                   
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_MT_R_TO_U_ET - ROHC_C_U_START]     
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                   
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_MT_R_TO_O_ET - ROHC_C_U_START]     
                [ROHC_C_ST_IR - ROHC_C_ST_IR] = ROHC_NULL;                   
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_MT_R_TO_O_ET - ROHC_C_U_START]     
                [ROHC_C_ST_FO - ROHC_C_ST_IR] = ROHC_NULL;                   
    rohc_ccpm_gen_fpt[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_MT_R_TO_O_ET - ROHC_C_U_START]     
                [ROHC_C_ST_SO - ROHC_C_ST_IR] = ROHC_NULL;                   
#endif

     /* - SPR 17439 */
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_u_ir_ir_dn;                           
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                           
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IRDYN_ET - ROHC_C_U_START]            
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IRDYN_ET - ROHC_C_U_START]            
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_u_irdyn_fo_dn;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IRDYN_ET - ROHC_C_U_START]            
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_UO_0_ET - ROHC_C_U_START]             
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_UO_0_ET - ROHC_C_U_START]             
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_UO_0_ET - ROHC_C_U_START]             
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_u_uo_0_so_dn;                
	rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_UO_1_ET - ROHC_C_U_START]             
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL; 
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_UO_1_ET - ROHC_C_U_START]             
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_u_irdyn_fo_dn;                         
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_UO_1_ET - ROHC_C_U_START]             
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_UO_2_ET - ROHC_C_U_START]             
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_UO_2_ET - ROHC_C_U_START]             
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_u_irdyn_fo_dn;                         
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_UO_2_ET - ROHC_C_U_START]             
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IR_RTN_TMR_ET - ROHC_C_U_START]       
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_u_ir_rtn_tmr_ir_dn;                   
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IR_RTN_TMR_ET - ROHC_C_U_START]       
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_u_ir_rtn_tmr_fo_dn;                   
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IR_RTN_TMR_ET - ROHC_C_U_START]       
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_u_ir_rtn_tmr_so_dn;                   
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_FO_RTN_TMR_ET - ROHC_C_U_START]       
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_u_fo_rtn_tmr_ir_dn;                   
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_FO_RTN_TMR_ET - ROHC_C_U_START]       
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_u_fo_rtn_tmr_fo_dn;                   
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_FO_RTN_TMR_ET - ROHC_C_U_START]       
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_u_fo_rtn_tmr_so_dn;                   
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_ACK_U_ET - ROHC_C_U_START]            
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_u_ack_u_ir_dn;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_ACK_U_ET - ROHC_C_U_START]            
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_u_ack_u_ir_dn;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_ACK_U_ET - ROHC_C_U_START]            
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_u_ack_u_ir_dn;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_MT_U_TO_O_ET - ROHC_C_U_START]        
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_u_mt_u_to_o_ir_dn;                    
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_MT_U_TO_O_ET - ROHC_C_U_START]        
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_u_mt_u_to_o_fo_dn;                    
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_MT_U_TO_O_ET - ROHC_C_U_START]        
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_u_mt_u_to_o_so_dn;                    
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_MT_U_TO_R_ET - ROHC_C_U_START]        
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_u_mt_u_to_r_ir_dn;                    
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_MT_U_TO_R_ET - ROHC_C_U_START]        
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_u_mt_u_to_r_fo_dn;                    
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_MT_U_TO_R_ET - ROHC_C_U_START]        
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_u_mt_u_to_r_so_dn;    
                                
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_IR_ET - ROHC_C_U_START]            
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_u_ir_ir_dn;                          
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_IR_ET - ROHC_C_U_START]            
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                          
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_IR_ET - ROHC_C_U_START]            
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                          
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_IRDYN_ET - ROHC_C_U_START]         
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                       
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_IRDYN_ET - ROHC_C_U_START]         
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_u_irdyn_fo_pnd;                       
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_IRDYN_ET - ROHC_C_U_START]         
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                       
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_UO_0_ET - ROHC_C_U_START]          
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_UO_0_ET - ROHC_C_U_START]          
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_UO_0_ET - ROHC_C_U_START]          
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_UO_1_ET - ROHC_C_U_START]          
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_UO_1_ET - ROHC_C_U_START]          
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_UO_1_ET - ROHC_C_U_START]          
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_UO_2_ET - ROHC_C_U_START]          
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_UO_2_ET - ROHC_C_U_START]          
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_u_uo_2_fo_pnd;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_UO_2_ET - ROHC_C_U_START]          
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_IR_RTN_TMR_ET - ROHC_C_U_START]    
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_u_ir_rtn_tmr_ir_dn;                  
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_IR_RTN_TMR_ET - ROHC_C_U_START]    
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_u_ir_rtn_tmr_fo_dn;                  
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_IR_RTN_TMR_ET - ROHC_C_U_START]    
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                  
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_FO_RTN_TMR_ET - ROHC_C_U_START]    
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                  
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_FO_RTN_TMR_ET - ROHC_C_U_START]    
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                  
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_FO_RTN_TMR_ET - ROHC_C_U_START]    
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                  
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_ACK_U_ET - ROHC_C_U_START]         
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_u_ack_u_ir_pnd;                       
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_ACK_U_ET - ROHC_C_U_START]         
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_u_ack_u_ir_pnd;                       
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_ACK_U_ET - ROHC_C_U_START]         
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                       
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_MT_U_TO_O_ET - ROHC_C_U_START]     
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_MT_U_TO_O_ET - ROHC_C_U_START]     
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_MT_U_TO_O_ET - ROHC_C_U_START]     
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_MT_U_TO_R_ET - ROHC_C_U_START]     
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_MT_U_TO_R_ET - ROHC_C_U_START]     
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_rtp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_MT_U_TO_R_ET - ROHC_C_U_START]     
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                   
    
    
#ifdef ROHC_MODE_TRIGGER 
                                                                         
    /*Funtion pointer table initialization for O-mode */                      
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_ir_ir_dn;                           
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                           
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                           
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_IRDYN_ET - ROHC_C_U_START]            
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_IRDYN_ET - ROHC_C_U_START]            
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_irdyn_fo_dn;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_IRDYN_ET - ROHC_C_U_START]            
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_UO_0_ET - ROHC_C_U_START]             
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_UO_0_ET - ROHC_C_U_START]             
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_UO_0_ET - ROHC_C_U_START]             
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_uo_0_so_dn;                         
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_UO_1_ET - ROHC_C_U_START]             
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_UO_1_ET - ROHC_C_U_START]             
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_irdyn_fo_dn;                         
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_UO_1_ET - ROHC_C_U_START]             
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_UO_2_ET - ROHC_C_U_START]             
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_UO_2_ET - ROHC_C_U_START]             
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_irdyn_fo_dn;                         
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_UO_2_ET - ROHC_C_U_START]             
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_ACK_O_ET - ROHC_C_U_START]            
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_ack_o_ir_dn;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_ACK_O_ET - ROHC_C_U_START]            
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_ack_o_fo_dn;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_ACK_O_ET - ROHC_C_U_START]            
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_ack_o_so_dn;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_STATIC_NACK_O_ET - ROHC_C_U_START]    
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_static_nack_o_ir_dn;                
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_STATIC_NACK_O_ET - ROHC_C_U_START]    
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_static_nack_o_fo_dn;                
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_STATIC_NACK_O_ET - ROHC_C_U_START]    
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_static_nack_o_fo_dn;                
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_NACK_O_ET - ROHC_C_U_START]           
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_nack_o_ir_dn;                       
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_NACK_O_ET - ROHC_C_U_START]           
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_nack_o_fo_dn;                       
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_NACK_O_ET - ROHC_C_U_START]           
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_nack_o_so_dn;                       
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_MT_O_TO_U_ET - ROHC_C_U_START]        
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_mt_o_to_u_ir_dn;                    
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_MT_O_TO_U_ET - ROHC_C_U_START]        
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_mt_o_to_u_fo_dn;                    
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_MT_O_TO_U_ET - ROHC_C_U_START]        
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_mt_o_to_u_so_dn;                    
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_MT_O_TO_R_ET - ROHC_C_U_START]        
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_mt_o_to_r_ir_dn;                    
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_MT_O_TO_R_ET - ROHC_C_U_START]        
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_mt_o_to_r_fo_dn;                    
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_MT_O_TO_R_ET - ROHC_C_U_START]        
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_mt_o_to_r_so_dn; 
                
                                   
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_IR_ET - ROHC_C_U_START]            
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_ir_ir_dn;                          
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_IR_ET - ROHC_C_U_START]            
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                          
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_IR_ET - ROHC_C_U_START]            
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                          
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_IRDYN_ET - ROHC_C_U_START]         
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                       
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_IRDYN_ET - ROHC_C_U_START]         
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_irdyn_fo_pnd;                       
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_IRDYN_ET - ROHC_C_U_START]         
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                       
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_UO_0_ET - ROHC_C_U_START]          
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_UO_0_ET - ROHC_C_U_START]          
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_UO_0_ET - ROHC_C_U_START]          
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_UO_1_ET - ROHC_C_U_START]          
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_UO_1_ET - ROHC_C_U_START]          
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_UO_1_ET - ROHC_C_U_START]          
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_UO_2_ET - ROHC_C_U_START]          
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_UO_2_ET - ROHC_C_U_START]          
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_irdyn_fo_pnd;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_UO_2_ET - ROHC_C_U_START]          
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_ACK_O_ET - ROHC_C_U_START]         
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_ack_o_ir_pnd;                       
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_ACK_O_ET - ROHC_C_U_START]         
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_ack_o_fo_pnd;                       
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_ACK_O_ET - ROHC_C_U_START]         
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                       
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_STATIC_NACK_O_ET - ROHC_C_U_START] 
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_static_nack_o_ir_pnd;               
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_STATIC_NACK_O_ET - ROHC_C_U_START] 
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_static_nack_o_fo_pnd;               
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_STATIC_NACK_O_ET - ROHC_C_U_START] 
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;               
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_NACK_O_ET - ROHC_C_U_START]        
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_nack_o_ir_pnd;                      
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_NACK_O_ET - ROHC_C_U_START]        
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_o_nack_o_fo_pnd;                      
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_NACK_O_ET - ROHC_C_U_START]        
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                      
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_MT_O_TO_U_ET - ROHC_C_U_START]     
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_MT_O_TO_U_ET - ROHC_C_U_START]     
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_MT_O_TO_U_ET - ROHC_C_U_START]     
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_MT_O_TO_R_ET - ROHC_C_U_START]     
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_MT_O_TO_R_ET - ROHC_C_U_START]     
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_rtp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_MT_O_TO_R_ET - ROHC_C_U_START]     
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                   
    
                                                                         
    /*Funtion pointer table initialization for R-mode */                      
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_ir_ir_dn;                           
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                           
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                           
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_IRDYN_ET - ROHC_C_U_START]            
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_IRDYN_ET - ROHC_C_U_START]            
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_irdyn_fo_dn;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_IRDYN_ET - ROHC_C_U_START]            
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_0_ET - ROHC_C_U_START]              
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                          
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_0_ET - ROHC_C_U_START]              
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                          
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_0_ET - ROHC_C_U_START]              
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_r_0_so_dn;                          
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_0_CRC_ET - ROHC_C_U_START]          
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                      
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_0_CRC_ET - ROHC_C_U_START]          
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                      
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_0_CRC_ET - ROHC_C_U_START]          
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_r_0_crc_so_dn;                      
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_1_ET - ROHC_C_U_START]              
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                          
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_1_ET - ROHC_C_U_START]              
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_r_1_fo_dn;                          
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_1_ET - ROHC_C_U_START]              
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                          
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_2_ET - ROHC_C_U_START]              
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                          
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_2_ET - ROHC_C_U_START]              
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_r_2_fo_dn;                          
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_2_ET - ROHC_C_U_START]              
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                          
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_ACK_R_ET - ROHC_C_U_START]            
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_ack_r_ir_dn;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_ACK_R_ET - ROHC_C_U_START]            
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_ack_r_fo_dn;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_ACK_R_ET - ROHC_C_U_START]            
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_ack_r_so_dn;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_STATIC_NACK_ET - ROHC_C_U_START]      
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_static_nack_ir_dn;        
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_STATIC_NACK_ET - ROHC_C_U_START]      
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_static_nack_fo_dn;                  
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_STATIC_NACK_ET - ROHC_C_U_START]      
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_static_nack_so_dn;                  
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_NACK_ET - ROHC_C_U_START]             
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_nack_ir_dn;                         
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_NACK_ET - ROHC_C_U_START]             
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_nack_fo_dn;                         
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_NACK_ET - ROHC_C_U_START]             
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_nack_so_dn;                         
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_MT_R_TO_U_ET - ROHC_C_U_START]        
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_mt_r_to_u_ir_dn;                    
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_MT_R_TO_U_ET - ROHC_C_U_START]        
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_mt_r_to_u_fo_dn;                    
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_MT_R_TO_U_ET - ROHC_C_U_START]        
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_mt_r_to_u_so_dn;                    
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_MT_R_TO_O_ET - ROHC_C_U_START]        
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_mt_r_to_o_ir_dn;                    
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_MT_R_TO_O_ET - ROHC_C_U_START]        
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_mt_r_to_o_fo_dn;                    
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_MT_R_TO_O_ET - ROHC_C_U_START]        
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_mt_r_to_o_so_dn; 
              
                                   
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_IR_ET - ROHC_C_U_START]            
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_ir_ir_pnd;                          
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_IR_ET - ROHC_C_U_START]            
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                          
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_IR_ET - ROHC_C_U_START]            
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                          
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_IRDYN_ET - ROHC_C_U_START]         
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                       
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_IRDYN_ET - ROHC_C_U_START]         
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_irdyn_fo_pnd;                       
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_IRDYN_ET - ROHC_C_U_START]         
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                       
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_0_ET - ROHC_C_U_START]           
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_0_ET - ROHC_C_U_START]           
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_0_ET - ROHC_C_U_START]           
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_0_CRC_ET - ROHC_C_U_START]       
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                     
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_0_CRC_ET - ROHC_C_U_START]       
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                     
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_0_CRC_ET - ROHC_C_U_START]       
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                     
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_1_ET - ROHC_C_U_START]           
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_1_ET - ROHC_C_U_START]           
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_1_ET - ROHC_C_U_START]           
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_2_ET - ROHC_C_U_START]           
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_2_ET - ROHC_C_U_START]           
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_r_2_fo_pnd;                         
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_2_ET - ROHC_C_U_START]           
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_ACK_R_ET - ROHC_C_U_START]         
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_ack_r_ir_pnd;                       
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_ACK_R_ET - ROHC_C_U_START]         
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_ack_r_ir_pnd;                       
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_ACK_R_ET - ROHC_C_U_START]         
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                       
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_STATIC_NACK_ET - ROHC_C_U_START]   
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_static_nack_ir_pnd;                 
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_STATIC_NACK_ET - ROHC_C_U_START]   
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_static_nack_fo_pnd;                 
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_STATIC_NACK_ET - ROHC_C_U_START]   
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                 
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_NACK_ET - ROHC_C_U_START]          
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_nack_ir_pnd;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_NACK_ET - ROHC_C_U_START]          
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = rohc_c_p1_r_nack_fo_pnd;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_NACK_ET - ROHC_C_U_START]          
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_MT_R_TO_U_ET - ROHC_C_U_START]     
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_MT_R_TO_U_ET - ROHC_C_U_START]     
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_MT_R_TO_U_ET - ROHC_C_U_START]     
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_MT_R_TO_O_ET - ROHC_C_U_START]     
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_MT_R_TO_O_ET - ROHC_C_U_START]     
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_rtp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_MT_R_TO_O_ET - ROHC_C_U_START]     
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP_RTP - ROHC_UNCOMP] = ROHC_NULL;                   
#endif

    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_u_ir_ir_dn;                           
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                           
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IRDYN_ET - ROHC_C_U_START]            
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IRDYN_ET - ROHC_C_U_START]            
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_u_irdyn_fo_dn;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IRDYN_ET - ROHC_C_U_START]            
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_UO_0_ET - ROHC_C_U_START]             
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_UO_0_ET - ROHC_C_U_START]             
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_UO_0_ET - ROHC_C_U_START]             
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_u_uo_0_so_dn;                
	rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_UO_1_ET - ROHC_C_U_START]             
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL; 
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_UO_1_ET - ROHC_C_U_START]             
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_u_irdyn_fo_dn;                         
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_UO_1_ET - ROHC_C_U_START]             
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_UO_2_ET - ROHC_C_U_START]             
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_UO_2_ET - ROHC_C_U_START]             
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_u_irdyn_fo_dn;                         
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_UO_2_ET - ROHC_C_U_START]             
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IR_RTN_TMR_ET - ROHC_C_U_START]       
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_u_ir_rtn_tmr_ir_dn;                   
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IR_RTN_TMR_ET - ROHC_C_U_START]       
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_u_ir_rtn_tmr_fo_dn;                   
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IR_RTN_TMR_ET - ROHC_C_U_START]       
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_u_ir_rtn_tmr_so_dn;                   
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_FO_RTN_TMR_ET - ROHC_C_U_START]       
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_u_fo_rtn_tmr_ir_dn;                   
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_FO_RTN_TMR_ET - ROHC_C_U_START]       
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_u_fo_rtn_tmr_fo_dn;                   
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_FO_RTN_TMR_ET - ROHC_C_U_START]       
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_u_fo_rtn_tmr_so_dn;                   
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_ACK_U_ET - ROHC_C_U_START]            
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_u_ack_u_ir_dn;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_ACK_U_ET - ROHC_C_U_START]            
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_u_ack_u_ir_dn;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_ACK_U_ET - ROHC_C_U_START]            
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_u_ack_u_ir_dn;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_MT_U_TO_O_ET - ROHC_C_U_START]        
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_u_mt_u_to_o_ir_dn;                    
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_MT_U_TO_O_ET - ROHC_C_U_START]        
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_u_mt_u_to_o_fo_dn;                    
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_MT_U_TO_O_ET - ROHC_C_U_START]        
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_u_mt_u_to_o_so_dn;                    
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_MT_U_TO_R_ET - ROHC_C_U_START]        
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_u_mt_u_to_r_ir_dn;                    
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_MT_U_TO_R_ET - ROHC_C_U_START]        
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_u_mt_u_to_r_fo_dn;                    
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_MT_U_TO_R_ET - ROHC_C_U_START]        
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_u_mt_u_to_r_so_dn;    
                                
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_IR_ET - ROHC_C_U_START]            
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_u_ir_ir_dn;                          
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_IR_ET - ROHC_C_U_START]            
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                          
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_IR_ET - ROHC_C_U_START]            
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                          
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_IRDYN_ET - ROHC_C_U_START]         
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                       
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_IRDYN_ET - ROHC_C_U_START]         
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_u_irdyn_fo_pnd;                       
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_IRDYN_ET - ROHC_C_U_START]         
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                       
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_UO_0_ET - ROHC_C_U_START]          
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_UO_0_ET - ROHC_C_U_START]          
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_UO_0_ET - ROHC_C_U_START]          
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_UO_1_ET - ROHC_C_U_START]          
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_UO_1_ET - ROHC_C_U_START]          
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_UO_1_ET - ROHC_C_U_START]          
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_UO_2_ET - ROHC_C_U_START]          
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_UO_2_ET - ROHC_C_U_START]          
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_u_uo_2_fo_pnd;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_UO_2_ET - ROHC_C_U_START]          
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_IR_RTN_TMR_ET - ROHC_C_U_START]    
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_u_ir_rtn_tmr_ir_dn;                  
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_IR_RTN_TMR_ET - ROHC_C_U_START]    
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_u_ir_rtn_tmr_fo_dn;                  
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_IR_RTN_TMR_ET - ROHC_C_U_START]    
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                  
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_FO_RTN_TMR_ET - ROHC_C_U_START]    
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                  
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_FO_RTN_TMR_ET - ROHC_C_U_START]    
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                  
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_FO_RTN_TMR_ET - ROHC_C_U_START]    
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                  
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_ACK_U_ET - ROHC_C_U_START]         
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_u_ack_u_ir_pnd;                       
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_ACK_U_ET - ROHC_C_U_START]         
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_u_ack_u_ir_pnd;                       
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_ACK_U_ET - ROHC_C_U_START]         
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                       
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_MT_U_TO_O_ET - ROHC_C_U_START]     
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_MT_U_TO_O_ET - ROHC_C_U_START]     
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_MT_U_TO_O_ET - ROHC_C_U_START]     
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_MT_U_TO_R_ET - ROHC_C_U_START]     
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_MT_U_TO_R_ET - ROHC_C_U_START]     
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_udp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_U_MT_U_TO_R_ET - ROHC_C_U_START]     
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                   
    
    
#ifdef ROHC_MODE_TRIGGER 
                                                                         
    /*Funtion pointer table initialization for O-mode */                      
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_ir_ir_dn;                           
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                           
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                           
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_IRDYN_ET - ROHC_C_U_START]            
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_IRDYN_ET - ROHC_C_U_START]            
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_irdyn_fo_dn;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_IRDYN_ET - ROHC_C_U_START]            
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_UO_0_ET - ROHC_C_U_START]             
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_UO_0_ET - ROHC_C_U_START]             
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_UO_0_ET - ROHC_C_U_START]             
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_uo_0_so_dn;                         
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_UO_1_ET - ROHC_C_U_START]             
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_UO_1_ET - ROHC_C_U_START]             
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_irdyn_fo_dn;                         
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_UO_1_ET - ROHC_C_U_START]             
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_UO_2_ET - ROHC_C_U_START]             
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_UO_2_ET - ROHC_C_U_START]             
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_irdyn_fo_dn;                         
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_UO_2_ET - ROHC_C_U_START]             
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_ACK_O_ET - ROHC_C_U_START]            
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_ack_o_ir_dn;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_ACK_O_ET - ROHC_C_U_START]            
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_ack_o_fo_dn;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_ACK_O_ET - ROHC_C_U_START]            
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_ack_o_so_dn;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_STATIC_NACK_O_ET - ROHC_C_U_START]    
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_static_nack_o_ir_dn;                
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_STATIC_NACK_O_ET - ROHC_C_U_START]    
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_static_nack_o_fo_dn;                
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_STATIC_NACK_O_ET - ROHC_C_U_START]    
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_static_nack_o_fo_dn;                
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_NACK_O_ET - ROHC_C_U_START]           
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_nack_o_ir_dn;                       
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_NACK_O_ET - ROHC_C_U_START]           
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_nack_o_fo_dn;                       
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_NACK_O_ET - ROHC_C_U_START]           
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_nack_o_so_dn;                       
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_MT_O_TO_U_ET - ROHC_C_U_START]        
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_mt_o_to_u_ir_dn;                    
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_MT_O_TO_U_ET - ROHC_C_U_START]        
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_mt_o_to_u_fo_dn;                    
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_MT_O_TO_U_ET - ROHC_C_U_START]        
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_mt_o_to_u_so_dn;                    
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_MT_O_TO_R_ET - ROHC_C_U_START]        
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_mt_o_to_r_ir_dn;                    
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_MT_O_TO_R_ET - ROHC_C_U_START]        
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_mt_o_to_r_fo_dn;                    
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_O_MT_O_TO_R_ET - ROHC_C_U_START]        
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_mt_o_to_r_so_dn; 
                
                                   
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_IR_ET - ROHC_C_U_START]            
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_ir_ir_dn;                          
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_IR_ET - ROHC_C_U_START]            
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                          
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_IR_ET - ROHC_C_U_START]            
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                          
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_IRDYN_ET - ROHC_C_U_START]         
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                       
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_IRDYN_ET - ROHC_C_U_START]         
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_irdyn_fo_pnd;                       
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_IRDYN_ET - ROHC_C_U_START]         
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                       
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_UO_0_ET - ROHC_C_U_START]          
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_UO_0_ET - ROHC_C_U_START]          
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_UO_0_ET - ROHC_C_U_START]          
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_UO_1_ET - ROHC_C_U_START]          
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_UO_1_ET - ROHC_C_U_START]          
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_UO_1_ET - ROHC_C_U_START]          
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_UO_2_ET - ROHC_C_U_START]          
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_UO_2_ET - ROHC_C_U_START]          
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_irdyn_fo_pnd;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_UO_2_ET - ROHC_C_U_START]          
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_ACK_O_ET - ROHC_C_U_START]         
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_ack_o_ir_pnd;                       
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_ACK_O_ET - ROHC_C_U_START]         
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_ack_o_fo_pnd;                       
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_ACK_O_ET - ROHC_C_U_START]         
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                       
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_STATIC_NACK_O_ET - ROHC_C_U_START] 
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_static_nack_o_ir_pnd;               
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_STATIC_NACK_O_ET - ROHC_C_U_START] 
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_static_nack_o_fo_pnd;               
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_STATIC_NACK_O_ET - ROHC_C_U_START] 
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;               
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_NACK_O_ET - ROHC_C_U_START]        
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_nack_o_ir_pnd;                      
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_NACK_O_ET - ROHC_C_U_START]        
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_o_nack_o_fo_pnd;                      
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_NACK_O_ET - ROHC_C_U_START]        
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                      
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_MT_O_TO_U_ET - ROHC_C_U_START]     
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_MT_O_TO_U_ET - ROHC_C_U_START]     
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_MT_O_TO_U_ET - ROHC_C_U_START]     
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_MT_O_TO_R_ET - ROHC_C_U_START]     
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_MT_O_TO_R_ET - ROHC_C_U_START]     
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_udp[ROHC_MODE_O - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_O_MT_O_TO_R_ET - ROHC_C_U_START]     
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                   
    
                                                                         
    /*Funtion pointer table initialization for R-mode */                      
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_ir_ir_dn;                           
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                           
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                           
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_IRDYN_ET - ROHC_C_U_START]            
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_IRDYN_ET - ROHC_C_U_START]            
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_irdyn_fo_dn;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_IRDYN_ET - ROHC_C_U_START]            
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_0_ET - ROHC_C_U_START]              
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                          
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_0_ET - ROHC_C_U_START]              
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                          
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_0_ET - ROHC_C_U_START]              
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_r_0_so_dn;                          
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_0_CRC_ET - ROHC_C_U_START]          
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                      
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_0_CRC_ET - ROHC_C_U_START]          
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                      
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_0_CRC_ET - ROHC_C_U_START]          
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_r_0_crc_so_dn;                      
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_1_ET - ROHC_C_U_START]              
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                          
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_1_ET - ROHC_C_U_START]              
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_r_1_fo_dn;                          
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_1_ET - ROHC_C_U_START]              
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                          
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_2_ET - ROHC_C_U_START]              
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                          
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_2_ET - ROHC_C_U_START]              
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_r_2_fo_dn;                          
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_R_2_ET - ROHC_C_U_START]              
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                          
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_ACK_R_ET - ROHC_C_U_START]            
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_ack_r_ir_dn;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_ACK_R_ET - ROHC_C_U_START]            
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_ack_r_fo_dn;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_ACK_R_ET - ROHC_C_U_START]            
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_ack_r_so_dn;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_STATIC_NACK_ET - ROHC_C_U_START]      
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_static_nack_ir_dn;        
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_STATIC_NACK_ET - ROHC_C_U_START]      
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_static_nack_fo_dn;                  
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_STATIC_NACK_ET - ROHC_C_U_START]      
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_static_nack_so_dn;                  
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_NACK_ET - ROHC_C_U_START]             
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_nack_ir_dn;                         
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_NACK_ET - ROHC_C_U_START]             
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_nack_fo_dn;                         
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_NACK_ET - ROHC_C_U_START]             
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_nack_so_dn;                         
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_MT_R_TO_U_ET - ROHC_C_U_START]        
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_mt_r_to_u_ir_dn;                    
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_MT_R_TO_U_ET - ROHC_C_U_START]        
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_mt_r_to_u_fo_dn;                    
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_MT_R_TO_U_ET - ROHC_C_U_START]        
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_mt_r_to_u_so_dn;                    
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_MT_R_TO_O_ET - ROHC_C_U_START]        
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_mt_r_to_o_ir_dn;                    
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_MT_R_TO_O_ET - ROHC_C_U_START]        
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_mt_r_to_o_fo_dn;                    
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_R_MT_R_TO_O_ET - ROHC_C_U_START]        
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_mt_r_to_o_so_dn; 
              
                                   
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_IR_ET - ROHC_C_U_START]            
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_ir_ir_pnd;                          
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_IR_ET - ROHC_C_U_START]            
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                          
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_IR_ET - ROHC_C_U_START]            
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                          
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_IRDYN_ET - ROHC_C_U_START]         
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                       
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_IRDYN_ET - ROHC_C_U_START]         
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_irdyn_fo_pnd;                       
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_IRDYN_ET - ROHC_C_U_START]         
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                       
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_0_ET - ROHC_C_U_START]           
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_0_ET - ROHC_C_U_START]           
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_0_ET - ROHC_C_U_START]           
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_0_CRC_ET - ROHC_C_U_START]       
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                     
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_0_CRC_ET - ROHC_C_U_START]       
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                     
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_0_CRC_ET - ROHC_C_U_START]       
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                     
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_1_ET - ROHC_C_U_START]           
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_1_ET - ROHC_C_U_START]           
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_1_ET - ROHC_C_U_START]           
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_2_ET - ROHC_C_U_START]           
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_2_ET - ROHC_C_U_START]           
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_r_2_fo_pnd;                         
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_R_2_ET - ROHC_C_U_START]           
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                         
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_ACK_R_ET - ROHC_C_U_START]         
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_ack_r_ir_pnd;                       
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_ACK_R_ET - ROHC_C_U_START]         
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_ack_r_ir_pnd;                       
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_ACK_R_ET - ROHC_C_U_START]         
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                       
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_STATIC_NACK_ET - ROHC_C_U_START]   
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_static_nack_ir_pnd;                 
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_STATIC_NACK_ET - ROHC_C_U_START]   
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_static_nack_fo_pnd;                 
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_STATIC_NACK_ET - ROHC_C_U_START]   
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                 
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_NACK_ET - ROHC_C_U_START]          
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_nack_ir_pnd;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_NACK_ET - ROHC_C_U_START]          
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = rohc_c_p2_r_nack_fo_pnd;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_NACK_ET - ROHC_C_U_START]          
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                        
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_MT_R_TO_U_ET - ROHC_C_U_START]     
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_MT_R_TO_U_ET - ROHC_C_U_START]     
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_MT_R_TO_U_ET - ROHC_C_U_START]     
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_MT_R_TO_O_ET - ROHC_C_U_START]     
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_MT_R_TO_O_ET - ROHC_C_U_START]     
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_udp[ROHC_MODE_R - ROHC_MODE_U]
                [ROHC_C_TST_PENDING - ROHC_C_TST_DONE] 
                [ROHC_C_R_MT_R_TO_O_ET - ROHC_C_U_START]     
                [ROHC_C_ST_SO - ROHC_C_ST_IR]
                [ROHC_IP_UDP - ROHC_UNCOMP] = ROHC_NULL;                   
#endif
    rohc_ccpm_fpt_tcp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_TCP - ROHC_UNCOMP] = rohc_c_p6_ir_ir_dn;                           
    rohc_ccpm_fpt_tcp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_TCP - ROHC_UNCOMP] = ROHC_NULL;                           
    rohc_ccpm_fpt_tcp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IR_ET - ROHC_C_U_START]               
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_TCP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_ccpm_fpt_tcp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IRDYN_ET - ROHC_C_U_START]               
                [ROHC_C_ST_IR - ROHC_C_ST_IR]
                [ROHC_IP_TCP - ROHC_UNCOMP] = ROHC_NULL;                           
    rohc_ccpm_fpt_tcp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IRDYN_ET - ROHC_C_U_START]               
                [ROHC_C_ST_FO - ROHC_C_ST_IR]
                [ROHC_IP_TCP - ROHC_UNCOMP] = rohc_c_p6_dyn_fo_dn;                           
    rohc_ccpm_fpt_tcp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IRDYN_ET - ROHC_C_U_START]               
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_TCP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_ccpm_fpt_tcp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_CO_P6_RND_ET - ROHC_C_U_START]               
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_TCP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_ccpm_fpt_tcp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_CO_P6_RND_ET - ROHC_C_U_START]               
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_TCP - ROHC_UNCOMP] = rohc_c_p6_dyn_fo_dn;
    rohc_ccpm_fpt_tcp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_CO_P6_RND_ET - ROHC_C_U_START]               
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_TCP - ROHC_UNCOMP] = rohc_c_p6_rnd_so_dn;
    rohc_ccpm_fpt_tcp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_CO_P6_COMMON_ET - ROHC_C_U_START]               
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_TCP - ROHC_UNCOMP] = ROHC_NULL;
    rohc_ccpm_fpt_tcp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_CO_P6_COMMON_ET - ROHC_C_U_START]               
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_TCP - ROHC_UNCOMP] = rohc_c_p6_dyn_fo_dn;
    rohc_ccpm_fpt_tcp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_CO_P6_COMMON_ET - ROHC_C_U_START]               
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_TCP - ROHC_UNCOMP] = rohc_c_p6_common_comp_so_dn;
    rohc_ccpm_fpt_tcp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IR_RTN_TMR_ET - ROHC_C_U_START]       
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_TCP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_tcp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IR_RTN_TMR_ET - ROHC_C_U_START]       
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_TCP - ROHC_UNCOMP] = rohc_c_p6_ir_rtn_tmr_dn;                   
    rohc_ccpm_fpt_tcp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_IR_RTN_TMR_ET - ROHC_C_U_START]       
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_TCP - ROHC_UNCOMP] = rohc_c_p6_ir_rtn_tmr_dn; 
    rohc_ccpm_fpt_tcp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_FO_RTN_TMR_ET - ROHC_C_U_START]       
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_TCP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_tcp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_FO_RTN_TMR_ET - ROHC_C_U_START]       
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_TCP - ROHC_UNCOMP] = ROHC_NULL;                   
    rohc_ccpm_fpt_tcp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_U_FO_RTN_TMR_ET - ROHC_C_U_START]       
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_TCP - ROHC_UNCOMP] = rohc_c_p6_fo_rtn_tmr_so_dn;                   
    rohc_ccpm_fpt_tcp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_ACK_ET - ROHC_C_U_START]       
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_TCP - ROHC_UNCOMP] = rohc_c_p6_ack_ir_dn;
    rohc_ccpm_fpt_tcp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_ACK_ET - ROHC_C_U_START]       
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_TCP - ROHC_UNCOMP] = rohc_c_p6_ack_fo_dn;
    rohc_ccpm_fpt_tcp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_ACK_ET - ROHC_C_U_START]       
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_TCP - ROHC_UNCOMP] = rohc_c_p6_ack_so_dn;
    rohc_ccpm_fpt_tcp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_NACK_ET - ROHC_C_U_START]       
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_TCP - ROHC_UNCOMP] = rohc_c_p6_nack_ir_dn;
    rohc_ccpm_fpt_tcp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_NACK_ET - ROHC_C_U_START]       
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_TCP - ROHC_UNCOMP] = rohc_c_p6_nack_fo_dn;
    rohc_ccpm_fpt_tcp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_NACK_ET - ROHC_C_U_START]       
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_TCP - ROHC_UNCOMP] = rohc_c_p6_nack_so_dn;
    rohc_ccpm_fpt_tcp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_STATIC_NACK_ET - ROHC_C_U_START]       
                [ROHC_C_ST_IR - ROHC_C_ST_IR] 
                [ROHC_IP_TCP - ROHC_UNCOMP] = rohc_c_p6_static_nack_ir_dn;
    rohc_ccpm_fpt_tcp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_STATIC_NACK_ET - ROHC_C_U_START]       
                [ROHC_C_ST_FO - ROHC_C_ST_IR] 
                [ROHC_IP_TCP - ROHC_UNCOMP] = rohc_c_p6_static_nack_fo_dn;
    rohc_ccpm_fpt_tcp[ROHC_MODE_U - ROHC_MODE_U]
                [ROHC_C_TST_DONE - ROHC_C_TST_DONE]
                [ROHC_C_STATIC_NACK_ET - ROHC_C_U_START]       
                [ROHC_C_ST_SO - ROHC_C_ST_IR] 
                [ROHC_IP_TCP - ROHC_UNCOMP] = rohc_c_p6_static_nack_fo_dn;
     /* - SPR 17439 */
     /* Initializing event mapping table */           
     rohc_ccpm_init_evt_tbl(); 
     
     ROHC_ENTER_FN("rohc_ccpm_init");
}

/*****************************************************************************
 * Function Name  : rohc_ccpm_exec 
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : ROHC_SUCCESS or ROHC_FAILURE
 * Description    : This is the main entry point function for CCPM. This
 *                  updates the state machine (state and transition state) and
 *                  processes the received mode transition and  ack/nack/static
 *                  nack.
 *****************************************************************************/
rohc_return_t rohc_ccpm_exec ROHC_PARAMS(
        ( p_entity ,p_csc ,pkt_typ , p_ecode ),
        rohc_entity_t *p_entity _AND_ 
        rohc_csc_t *p_csc _AND_
        rohc_U8bit_t pkt_typ _AND_
        rohc_error_code_t *p_ecode)

{
    rohc_U8bit_t event_type = 0;
    ROHC_ENTER_FN("rohc_exec_ccpm");
    if(p_csc->profile == ROHC_UNCOMP)
    {
        if( (p_csc->c_state == ROHC_CUN_ST_IR) &&
            (pkt_typ == ROHC_IRP0_PKTYPE)
            )
        {
            p_entity->p0_ir_counter++;
            if(p_entity->p0_ir_counter >= p_entity->max_p0_ir_counter)
            {
                p_csc->c_state = ROHC_CUN_ST_NORMAL;
            }   
        }
    }
    else if((pkt_typ>= ROHC_R0P1_PKTTYPE && pkt_typ <ROHC_MAX_PKT_TYPES ) &&
      ( p_csc->c_mode >= ROHC_MODE_U &&  p_csc->c_mode <= ROHC_MODE_R) &&
       ((event_type=rohc_ccpm_map_tbl[pkt_typ][p_csc->c_mode - ROHC_MODE_U])!= 
        ROHC_NULL))
    {    
        if(!rohc_ccpm_exec_direct_evt(p_entity, p_csc, event_type , p_ecode))
            return ROHC_FAILURE;  
    }
    else
    {
            *p_ecode = ROHC_INVALID_CCPM_EVENT;
            ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                      "Received Packet type[%u] is not valid \n",
                                      p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                      ROHC_GET_CSC_CID(p_csc),pkt_typ);
            ROHC_EXIT_FN("rohc_exec_ccpm");
            return ROHC_FAILURE;                                      
        
    }
    ROHC_EXIT_FN("rohc_exec_ccpm");
    return ROHC_SUCCESS;

}

/*****************************************************************************
 * Function Name  : rohc_ccpm_p0_exec 
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : ROHC_SUCCESS or ROHC_FAILURE
 * Description    : This is the main entry point function for CCPM. This
 *                  updates the state machine (state and transition state) and
 *                  processes the received mode transition and  ack/nack/static
 *                  nack.
 *****************************************************************************/
rohc_return_t rohc_ccpm_p0_exec ROHC_PARAMS(
        ( p_entity ,p_csc ,pkt_typ , p_ecode ),
        rohc_entity_t     *p_entity _AND_ 
        rohc_csc_uncomp_t *p_csc    _AND_
        rohc_U8bit_t      pkt_typ   _AND_
        rohc_error_code_t *p_ecode)

{
    ROHC_ENTER_FN("rohc_ccpm_p0_exec");
    ROHC_UT_TRACE((stderr,"In rohc_ccpm_p0_exec:Current State is [%d]: Current Mode is[%d]\n",
                       p_csc->c_state,p_csc->c_mode));

    if(p_csc->profile == ROHC_UNCOMP)
    {
        if( (p_csc->c_state == ROHC_CUN_ST_IR) &&
            (pkt_typ == ROHC_IRP0_PKTYPE)
            )
        {
            p_entity->p0_ir_counter++;
            if(p_entity->p0_ir_counter >= p_entity->max_p0_ir_counter)
            {
                p_csc->c_state = ROHC_CUN_ST_NORMAL;
            }   
        }
    }
    else
    {
            *p_ecode = ROHC_INVALID_CCPM_EVENT;
            ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                      "Received Packet type[%u] is not valid \n",
                                      p_entity->rohc_entity_id.ueIndex,
                                      p_entity->rohc_entity_id.lcId,
                                      ROHC_GET_CSC_CID(p_csc),pkt_typ);
            ROHC_EXIT_FN("rohc_ccpm_p0_exec");
            return ROHC_FAILURE;                                      
        
    }
    ROHC_EXIT_FN("rohc_ccpm_p0_exec");
    return ROHC_SUCCESS;

}

/*****************************************************************************
 * Function Name  : rohc_ccpm_p1_exec 
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : ROHC_SUCCESS or ROHC_FAILURE
 * Description    : This is the main entry point function for CCPM. This
 *                  updates the state machine (state and transition state) and
 *                  processes the received mode transition and  ack/nack/static
 *                  nack.
 *****************************************************************************/
rohc_return_t rohc_ccpm_p1_exec ROHC_PARAMS(
        ( p_entity ,p_csc ,pkt_typ , p_ecode ),
        rohc_entity_t       *p_entity   _AND_ 
        rohc_csc_rtp_t      *p_csc      _AND_
        rohc_U8bit_t        pkt_typ     _AND_
        rohc_error_code_t   *p_ecode)

{
    rohc_U8bit_t event_type = 0;
    ROHC_ENTER_FN("rohc_ccpm_p1_exec");
    ROHC_UT_TRACE((stderr,"Current State is [%d]: Current Mode is [%d]\n",
                      p_csc->c_state,p_csc->c_mode));
    if(p_csc->profile == ROHC_UNCOMP)
    {
        if( (p_csc->c_state == ROHC_CUN_ST_IR) &&
            (pkt_typ == ROHC_IRP0_PKTYPE)
            )
        {
            p_entity->p0_ir_counter++;
            if(p_entity->p0_ir_counter >= p_entity->max_p0_ir_counter)
            {
                p_csc->c_state = ROHC_CUN_ST_NORMAL;
            }   
        }
    }
    else if((pkt_typ>= ROHC_R0P1_PKTTYPE && pkt_typ <ROHC_MAX_PKT_TYPES ) &&
      ( p_csc->c_mode >= ROHC_MODE_U &&  p_csc->c_mode <= ROHC_MODE_R) &&
       ((event_type=rohc_ccpm_map_tbl[pkt_typ][p_csc->c_mode - ROHC_MODE_U])!= 
        ROHC_NULL))
    {    
        if(!rohc_ccpm_p1_exec_direct_evt(p_entity, p_csc, event_type , p_ecode))
            return ROHC_FAILURE;  
    }
    else
    {
            *p_ecode = ROHC_INVALID_CCPM_EVENT;
            ROHC_UT_TRACE((stderr,"Received Packet type[%u] is not valid \n",
                              pkt_typ));
            ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                              "Received Packet type[%u] is not valid \n",
                              p_entity->rohc_entity_id.ueIndex,
                              p_entity->rohc_entity_id.lcId,
                              ROHC_GET_CSC_CID(p_csc),pkt_typ);
            ROHC_EXIT_FN("rohc_ccpm_p1_exec");
            return ROHC_FAILURE;                                      
        
    }
    ROHC_EXIT_FN("rohc_ccpm_p1_exec");
    return ROHC_SUCCESS;

}

/*****************************************************************************
 * Function Name  : rohc_ccpm_p2_exec 
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : ROHC_SUCCESS or ROHC_FAILURE
 * Description    : This is the main entry point function for CCPM. This
 *                  updates the state machine (state and transition state) and
 *                  processes the received mode transition and  ack/nack/static
 *                  nack.
 *****************************************************************************/
rohc_return_t rohc_ccpm_p2_exec ROHC_PARAMS(
        ( p_entity ,p_csc ,pkt_typ , p_ecode ),
        rohc_entity_t     *p_entity _AND_ 
        rohc_csc_udp_t    *p_csc    _AND_
        rohc_U8bit_t      pkt_typ   _AND_
        rohc_error_code_t *p_ecode)

{
    rohc_U8bit_t event_type = 0;
    ROHC_ENTER_FN("rohc_ccpm_p2_exec");
    if(((rohc_csc_common_t *)p_csc)->profile == ROHC_UNCOMP)
    {
        if( (((rohc_csc_common_t *)p_csc)->c_state == ROHC_CUN_ST_IR) &&
            (pkt_typ == ROHC_IRP0_PKTYPE)
            )
        {
            p_entity->p0_ir_counter++;
            if(p_entity->p0_ir_counter >= p_entity->max_p0_ir_counter)
            {
                ((rohc_csc_common_t *)p_csc)->c_state = ROHC_CUN_ST_NORMAL;
            }   
        }
    }
    else if((pkt_typ>= ROHC_R0P1_PKTTYPE && pkt_typ <ROHC_MAX_PKT_TYPES ) &&
      ( p_csc->c_mode >= ROHC_MODE_U &&  p_csc->c_mode <= ROHC_MODE_R) &&
       ((event_type=rohc_ccpm_map_tbl[pkt_typ][p_csc->c_mode - ROHC_MODE_U])!= 
        ROHC_NULL))
    {    
        if(!rohc_ccpm_p2_exec_direct_evt(p_entity, p_csc, event_type , p_ecode))
            return ROHC_FAILURE;  
    }
    else
    {
            *p_ecode = ROHC_INVALID_CCPM_EVENT;
            ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                              "Received Packet type[%u] is not valid \n",
                              p_entity->rohc_entity_id.ueIndex,
                              p_entity->rohc_entity_id.lcId,
                              ROHC_GET_CSC_CID(p_csc),pkt_typ);
            ROHC_EXIT_FN("rohc_ccpm_p2_exec");
            return ROHC_FAILURE;                                      
        
    }
    ROHC_EXIT_FN("rohc_ccpm_p2_exec");
    return ROHC_SUCCESS;

}

/*****************************************************************************
 * Function Name  : rohc_ccpm_p6_exec 
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : ROHC_SUCCESS or ROHC_FAILURE
 * Description    : This is the main entry point function for CCPM. This
 *                  updates the state machine (state and transition state) and
 *                  processes the received mode transition and  ack/nack/static
 *                  nack.
 *****************************************************************************/
rohc_return_t rohc_ccpm_p6_exec ROHC_PARAMS(
        ( p_entity ,p_csc ,pkt_typ , p_ecode ),
        rohc_entity_t     *p_entity _AND_ 
        rohc_csc_tcp_t    *p_csc    _AND_
        rohc_U8bit_t      pkt_typ   _AND_
        rohc_error_code_t *p_ecode)

{
    rohc_U8bit_t event_type = 0;
    ROHC_ENTER_FN("rohc_ccpm_p6_exec");
    if(ROHC_UNCOMP == ((rohc_csc_common_t *)p_csc)->profile)
    {
        if((ROHC_CUN_ST_IR == ((rohc_csc_common_t *)p_csc)->c_state) &&
                (ROHC_IRP0_PKTYPE == pkt_typ)
          )
        {
            p_entity->p0_ir_counter++;
            if(p_entity->p0_ir_counter >= p_entity->max_p0_ir_counter)
            {
                ((rohc_csc_common_t *)p_csc)->c_state = ROHC_CUN_ST_NORMAL;
            }   
        }
    }
    else if((ROHC_CO_P6_COMMON_PKTYPE <= pkt_typ && ROHC_MAX_PKT_TYPES > pkt_typ ) &&
            (ROHC_NULL != (event_type=rohc_ccpm_map_tbl[pkt_typ][0])))
    {    
        if(!rohc_ccpm_p6_exec_direct_evt(p_entity, p_csc, event_type , p_ecode))
        {
            return ROHC_FAILURE;  
        }
    }
    else
    {
        *p_ecode = ROHC_INVALID_CCPM_EVENT;
        ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                "Received Packet type[%u] is not valid \n",
                p_entity->rohc_entity_id.ueIndex,
                p_entity->rohc_entity_id.lcId,
                ROHC_GET_CSC_CID(p_csc),pkt_typ);
        ROHC_EXIT_FN("rohc_ccpm_p6_exec");
        return ROHC_FAILURE;                                      

    }
    ROHC_EXIT_FN("rohc_ccpm_p6_exec");
    return ROHC_SUCCESS;
}

/*****************************************************************************
 * Function Name  : rohc_ccpm_exec_direct_evt 
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : ROHC_SUCCESS or ROHC_FAILURE
 * Description    : This function updates the state machine (state and
 *                  transition state) and processes the received mode transition
 *                  and ack/nack/static nack.
 *****************************************************************************/
rohc_return_t rohc_ccpm_exec_direct_evt ROHC_PARAMS(
        ( p_entity ,p_csc ,evt_type , p_ecode ),
        rohc_entity_t *p_entity _AND_ 
        rohc_csc_t *p_csc _AND_
        rohc_U8bit_t evt_type _AND_
        rohc_error_code_t *p_ecode)

{
    rohc_ccpm_fprtable temp_fptr = ROHC_NULL;
    
    ROHC_ENTER_FN("rohc_ccpm_exec_direct_evt");

    ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"\
                      "Before performing CCPM funtionality for event type[%u]"\
                      "## mode[%u], profile[%u], state[%u], "\
                      "transition-state[%u] ##\n",
                       p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                       ROHC_GET_CSC_CID(p_csc),
                       evt_type,p_csc->c_mode,p_csc->profile,
                       p_csc->c_state,p_csc->c_trans_state);
        
      
    if((evt_type>=ROHC_C_U_START && evt_type <= ROHC_C_U_MAX_ET &&
         p_csc->c_mode >= ROHC_MODE_U &&  p_csc->c_mode <= ROHC_MODE_R &&
         p_csc->c_trans_state >= ROHC_C_TST_DONE && 
         p_csc->c_trans_state <=ROHC_C_TST_PENDING &&
         p_csc->c_state >= ROHC_C_ST_IR && p_csc->c_state <= ROHC_C_ST_SO) &&
         ( p_csc->profile == ROHC_IP_UDP_RTP || p_csc->profile == ROHC_IP_UDP
         || p_csc->profile == ROHC_IP_ESP))
    {    
        temp_fptr = rohc_ccpm_gen_fpt[p_csc->c_mode - ROHC_MODE_U] 
        [p_csc->c_trans_state - ROHC_C_TST_DONE] 
        [evt_type - ROHC_C_U_START] 
        [p_csc->c_state - ROHC_C_ST_IR];
        
        if(temp_fptr != ROHC_NULL)
        {
            (temp_fptr)(p_entity, p_csc );
        }
        else
        {       
            ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                                          "Invalid operation for event type[%u]\n",
                                          p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                          ROHC_GET_CSC_CID(p_csc),evt_type);
        }
    }else
    {
        *p_ecode = ROHC_INVALID_CCPM_EVENT;
        ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                      "Event type[%u] is not valid \n",
                      p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                      ROHC_GET_CSC_CID(p_csc),evt_type );
        ROHC_EXIT_FN("rohc_ccpm_exec_direct_evt");
        return ROHC_FAILURE;                                      
        
    }
    
    ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"\
                      "After performing CCPM funtionality for event type[%u]"\
                      "## mode[%u], profile[%u], state[%u], "\
                      "transition-state[%u] ##\n",
                       p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                       ROHC_GET_CSC_CID(p_csc),
                       evt_type,p_csc->c_mode,p_csc->profile,
                       p_csc->c_state,p_csc->c_trans_state);
    ROHC_EXIT_FN("rohc_ccpm_exec_direct_evt");
    return ROHC_SUCCESS;
  
}

/*****************************************************************************
 * Function Name  : rohc_ccpm_p1_exec_direct_evt 
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : ROHC_SUCCESS or ROHC_FAILURE
 * Description    : This function updates the state machine (state and
 *                  transition state) and processes the received mode transition
 *                  and ack/nack/static nack.
 *****************************************************************************/
rohc_return_t rohc_ccpm_p1_exec_direct_evt ROHC_PARAMS(
        ( p_entity ,p_csc ,evt_type , p_ecode ),
        rohc_entity_t       *p_entity   _AND_ 
        rohc_csc_rtp_t      *p_csc      _AND_
        rohc_U8bit_t        evt_type    _AND_
        rohc_error_code_t   *p_ecode)

{
     /* + SPR 17439 */
    rohc_ccpm_fprtable_fpt_rtp temp_fptr = ROHC_NULL;
     /* - SPR 17439 */

    ROHC_ENTER_FN("rohc_ccpm_p1_exec_direct_evt");
    ROHC_UT_TRACE((stderr,"Before performing CCPM funtionality ueId[%u] lcId[%u] "\
                " ctxtId [%u] for event type[%u]"\
                "## mode[%u], profile[%u], state[%u], "\
                "transition-state[%u] ##\n",
                p_entity->rohc_entity_id.ueIndex,
                p_entity->rohc_entity_id.lcId, ROHC_GET_CSC_CID(p_csc),
                evt_type,p_csc->c_mode,p_csc->profile,
                p_csc->c_state,p_csc->c_trans_state));


    ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"\
            "Before performing CCPM funtionality for event type[%u]"\
            "## mode[%u], profile[%u], state[%u], "\
            "transition-state[%u] ##\n",
            p_entity->rohc_entity_id.ueIndex,
            p_entity->rohc_entity_id.lcId, ROHC_GET_CSC_CID(p_csc),
            evt_type,p_csc->c_mode,p_csc->profile,
            p_csc->c_state,p_csc->c_trans_state);


    if((evt_type>=ROHC_C_U_START && evt_type <= ROHC_C_R_MAX_ET &&
                p_csc->c_mode >= ROHC_MODE_U &&  p_csc->c_mode <= ROHC_MODE_R &&
                p_csc->c_trans_state >= ROHC_C_TST_DONE && 
                p_csc->c_trans_state <=ROHC_C_TST_PENDING &&
                p_csc->c_state >= ROHC_C_ST_IR && p_csc->c_state <= ROHC_C_ST_SO) &&
            ( p_csc->profile == ROHC_IP_UDP_RTP || p_csc->profile == ROHC_IP_UDP
              || p_csc->profile == ROHC_IP_ESP))
    {    
        /* + SPR 17439 */
        temp_fptr = rohc_ccpm_fpt_rtp[p_csc->c_mode - ROHC_MODE_U] 
        /* - SPR 17439 */
            [p_csc->c_trans_state - ROHC_C_TST_DONE] 
            [evt_type - ROHC_C_U_START] 
            [p_csc->c_state - ROHC_C_ST_IR]
            [p_csc->profile - ROHC_UNCOMP];

        if(temp_fptr != ROHC_NULL)
        {
            (temp_fptr)(p_entity, p_csc );
        }
        else
        {       
            ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                    "Invalid operation for event type[%u]\n",
                    p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId,
                    ROHC_GET_CSC_CID(p_csc),evt_type);
        }
    }else
    {
        *p_ecode = ROHC_INVALID_CCPM_EVENT;
        ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                "Event type[%u] is not valid \n",
                p_entity->rohc_entity_id.ueIndex,
                p_entity->rohc_entity_id.lcId,
                ROHC_GET_CSC_CID(p_csc),evt_type );
        ROHC_EXIT_FN("rohc_ccpm_p1_exec_direct_evt");
        return ROHC_FAILURE;                                      

    }
    ROHC_UT_TRACE((stderr,"After performing CCPM funtionality for event type[%u]"\
                "## mode[%u], profile[%u], state[%u], "\
                "transition-state[%u] ##\n",
                evt_type,p_csc->c_mode,p_csc->profile,
                p_csc->c_state,p_csc->c_trans_state));

    ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"\
            "After performing CCPM funtionality for event type[%u]"\
            "## mode[%u], profile[%u], state[%u], "\
            "transition-state[%u] ##\n",
            p_entity->rohc_entity_id.ueIndex,
            p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),
            evt_type,p_csc->c_mode,p_csc->profile,
            p_csc->c_state,p_csc->c_trans_state);
    ROHC_EXIT_FN("rohc_ccpm_p1_exec_direct_evt");
    return ROHC_SUCCESS;

}

/*****************************************************************************
 * Function Name  : rohc_ccpm_p2_exec_direct_evt 
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : ROHC_SUCCESS or ROHC_FAILURE
 * Description    : This function updates the state machine (state and
 *                  transition state) and processes the received mode transition
 *                  and ack/nack/static nack.
 *****************************************************************************/
rohc_return_t rohc_ccpm_p2_exec_direct_evt ROHC_PARAMS(
        ( p_entity ,p_csc ,evt_type , p_ecode ),
        rohc_entity_t     *p_entity _AND_ 
        rohc_csc_udp_t    *p_csc    _AND_
        rohc_U8bit_t      evt_type  _AND_
        rohc_error_code_t *p_ecode)

{
     /* + SPR 17439 */
    rohc_ccpm_fprtable_fpt_udp temp_fptr = ROHC_NULL;
     /* - SPR 17439 */

    ROHC_ENTER_FN("rohc_ccpm_p2_exec_direct_evt");
    ROHC_UT_TRACE((stderr,"Before performing CCPM funtionality for ueId[%u]"\
                "lcId[%u] cid[%u] event type[%u]"\
                "## mode[%u], profile[%u], state[%u], "\
                "transition-state[%u] ##\n",
                p_entity->rohc_entity_id.ueIndex,
                p_entity->rohc_entity_id.lcId, ROHC_GET_CSC_CID(p_csc),
                evt_type,p_csc->c_mode,p_csc->profile,
                p_csc->c_state,p_csc->c_trans_state));

    ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"\
            "Before performing CCPM funtionality for event type[%u]"\
            "## mode[%u], profile[%u], state[%u], "\
            "transition-state[%u] ##\n",
            p_entity->rohc_entity_id.ueIndex,
            p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),
            evt_type,p_csc->c_mode,p_csc->profile,
            p_csc->c_state,p_csc->c_trans_state);


    if((evt_type>=ROHC_C_U_START && evt_type <= ROHC_C_R_MAX_ET &&
                p_csc->c_mode >= ROHC_MODE_U &&  p_csc->c_mode <= ROHC_MODE_R &&
                p_csc->c_trans_state >= ROHC_C_TST_DONE && 
                p_csc->c_trans_state <=ROHC_C_TST_PENDING &&
                p_csc->c_state >= ROHC_C_ST_IR && p_csc->c_state <= ROHC_C_ST_SO) &&
            ( p_csc->profile == ROHC_IP_UDP_RTP || p_csc->profile == ROHC_IP_UDP
              || p_csc->profile == ROHC_IP_ESP))
    {    
       /* + SPR 17439 */
        temp_fptr = rohc_ccpm_fpt_udp[p_csc->c_mode - ROHC_MODE_U] 
       /* - SPR 17439 */
            [p_csc->c_trans_state - ROHC_C_TST_DONE] 
            [evt_type - ROHC_C_U_START] 
            [p_csc->c_state - ROHC_C_ST_IR]
            [p_csc->profile - ROHC_UNCOMP];

        if(temp_fptr != ROHC_NULL)
        {
            (temp_fptr)(p_entity, p_csc );
        }
        else
        {       
            ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                    "Invalid operation for event type[%u]\n",
                    p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId,
                    ROHC_GET_CSC_CID(p_csc),evt_type);
        }
    }else
    {
        *p_ecode = ROHC_INVALID_CCPM_EVENT;
        ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                "Event type[%u] is not valid \n",
                p_entity->rohc_entity_id.ueIndex,
                p_entity->rohc_entity_id.lcId,
                ROHC_GET_CSC_CID(p_csc),evt_type );
        ROHC_EXIT_FN("rohc_ccpm_p2_exec_direct_evt");
        return ROHC_FAILURE;                                      

    }

    ROHC_UT_TRACE((stderr,"After performing CCPM funtionality for event type[%u]"\
                "## mode[%u], profile[%u], state[%u], "\
                "transition-state[%u] ##\n",
                evt_type,p_csc->c_mode,p_csc->profile,
                p_csc->c_state,p_csc->c_trans_state));

    ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"\
            "After performing CCPM funtionality for event type[%u]"\
            "## mode[%u], profile[%u], state[%u], "\
            "transition-state[%u] ##\n",
            p_entity->rohc_entity_id.ueIndex,
            p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),
            evt_type,p_csc->c_mode,p_csc->profile,
            p_csc->c_state,p_csc->c_trans_state);
    ROHC_EXIT_FN("rohc_ccpm_p2_exec_direct_evt");
    return ROHC_SUCCESS;

}

/*****************************************************************************
 * Function Name  : rohc_ccpm_p6_exec_direct_evt 
 * Inputs         : p_entity - Pointer to rohc entity block
 *                  p_csc - Pointer to rohc compressor stream context block
 * Outputs        : None
 * Returns        : ROHC_SUCCESS or ROHC_FAILURE
 * Description    : This function updates the state machine (state and
 *                  transition state) and processes the received mode transition
 *                  and ack/nack/static nack.
 *****************************************************************************/
rohc_return_t rohc_ccpm_p6_exec_direct_evt ROHC_PARAMS(
        ( p_entity ,p_csc ,evt_type , p_ecode ),
        rohc_entity_t     *p_entity _AND_ 
        rohc_csc_tcp_t    *p_csc    _AND_
        rohc_U8bit_t      evt_type  _AND_
        rohc_error_code_t *p_ecode)

{
     /* + SPR 17439 */
    rohc_ccpm_fprtable_fpt_tcp temp_fptr = ROHC_NULL;
     /* - SPR 17439 */

    ROHC_ENTER_FN("rohc_ccpm_p6_exec_direct_evt");
    ROHC_UT_TRACE((stderr,"Before performing CCPM funtionality for ueId[%u]"\
                "lcId[%u] cid[%u] event type[%u]"\
                "## mode[%u], profile[%u], state[%u], "\
                "transition-state[%u] ##\n",
                p_entity->rohc_entity_id.ueIndex,
                p_entity->rohc_entity_id.lcId, ROHC_GET_CSC_CID(p_csc),
                evt_type,p_csc->c_mode,p_csc->profile,
                p_csc->c_state,p_csc->c_trans_state));

    ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"\
            "Before performing CCPM funtionality for event type[%u]"\
            "## mode[%u], profile[%u], state[%u], "\
            "transition-state[%u] ##\n",
            p_entity->rohc_entity_id.ueIndex,
            p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),
            evt_type,p_csc->c_mode,p_csc->profile,
            p_csc->c_state,p_csc->c_trans_state);


    if((ROHC_C_U_START <= evt_type && ROHC_C_U_MAX_ET >= evt_type &&
                ROHC_MODE_U <= p_csc->c_mode && ROHC_MODE_R >= p_csc->c_mode &&
                ROHC_C_TST_DONE <= p_csc->c_trans_state && 
                ROHC_C_TST_PENDING >= p_csc->c_trans_state &&
                ROHC_C_ST_IR <= p_csc->c_state  && ROHC_C_ST_SO >= p_csc->c_state) &&
            (ROHC_IP_UDP_RTP == p_csc->profile || ROHC_IP_UDP == p_csc->profile
             || ROHC_IP_ESP == p_csc->profile || ROHC_IP_TCP == p_csc->profile))
    {    
        /* + SPR 17439 */
        temp_fptr = rohc_ccpm_fpt_tcp[0] 
        /* - SPR 17439 */
            [p_csc->c_trans_state - ROHC_C_TST_DONE] 
            [evt_type - ROHC_C_U_START] 
            [p_csc->c_state - ROHC_C_ST_IR]
            [p_csc->profile - ROHC_UNCOMP];

        if(temp_fptr != ROHC_NULL)
        {
            (temp_fptr)(p_entity, p_csc );
        }
        else
        {       
            ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                    "Invalid operation for event type[%u]\n",
                    p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId,
                    ROHC_GET_CSC_CID(p_csc),evt_type);
        }
    }else
    {
        *p_ecode = ROHC_INVALID_CCPM_EVENT;
        ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
                "Event type[%u] is not valid \n",
                p_entity->rohc_entity_id.ueIndex,
                p_entity->rohc_entity_id.lcId,
                ROHC_GET_CSC_CID(p_csc),evt_type );
        ROHC_EXIT_FN("rohc_ccpm_p6_exec_direct_evt");
        return ROHC_FAILURE;                                      

    }

    ROHC_UT_TRACE((stderr,"After performing CCPM funtionality for event type[%u]"\
                "## mode[%u], profile[%u], state[%u], "\
                "transition-state[%u] ##\n",
                evt_type,p_csc->c_mode,p_csc->profile,
                p_csc->c_state,p_csc->c_trans_state));

    ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"\
            "After performing CCPM funtionality for event type[%u]"\
            "## mode[%u], profile[%u], state[%u], "\
            "transition-state[%u] ##\n",
            p_entity->rohc_entity_id.ueIndex,
            p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),
            evt_type,p_csc->c_mode,p_csc->profile,
            p_csc->c_state,p_csc->c_trans_state);
    ROHC_EXIT_FN("rohc_ccpm_p6_exec_direct_evt");
    return ROHC_SUCCESS;

}

/*****************************************************************************
 * Function Name  : rohc_ccpm_init_evt_tbl 
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function initializes a static table which maps the
 *                  selected packet type to a CCPM-defined events.
 *****************************************************************************/
rohc_void_t rohc_ccpm_init_evt_tbl ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_ccpm_init_evt_tbl");
    rohc_ccpm_map_tbl[ROHC_R0P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_ccpm_map_tbl[ROHC_R0P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_ccpm_map_tbl[ROHC_R0P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_R_0_ET;

    rohc_ccpm_map_tbl[ROHC_R0CRCP1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_ccpm_map_tbl[ROHC_R0CRCP1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_ccpm_map_tbl[ROHC_R0CRCP1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_R_0_CRC_ET;

    rohc_ccpm_map_tbl[ROHC_UO0P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_C_U_UO_0_ET;
    rohc_ccpm_map_tbl[ROHC_UO0P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_C_O_UO_0_ET;
    rohc_ccpm_map_tbl[ROHC_UO0P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_NULL;

    rohc_ccpm_map_tbl[ROHC_R1P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                  ROHC_NULL;          
    rohc_ccpm_map_tbl[ROHC_R1P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                  ROHC_NULL;          
    rohc_ccpm_map_tbl[ROHC_R1P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                  ROHC_C_R_R_1_ET;    

    rohc_ccpm_map_tbl[ROHC_R1E0P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                    ROHC_NULL;          
    rohc_ccpm_map_tbl[ROHC_R1E0P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                    ROHC_NULL;          
    rohc_ccpm_map_tbl[ROHC_R1E0P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                    ROHC_C_R_R_1_ET;    

    rohc_ccpm_map_tbl[ROHC_R1E1P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                    ROHC_NULL;          
    rohc_ccpm_map_tbl[ROHC_R1E1P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                    ROHC_NULL;          
    rohc_ccpm_map_tbl[ROHC_R1E1P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                    ROHC_C_R_R_1_ET;    

    rohc_ccpm_map_tbl[ROHC_R1E2P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                    ROHC_NULL;          
    rohc_ccpm_map_tbl[ROHC_R1E2P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                    ROHC_NULL;          
    rohc_ccpm_map_tbl[ROHC_R1E2P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                    ROHC_C_R_R_1_ET;    

    rohc_ccpm_map_tbl[ROHC_R1E3P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                    ROHC_NULL;          
    rohc_ccpm_map_tbl[ROHC_R1E3P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                    ROHC_NULL;          
    rohc_ccpm_map_tbl[ROHC_R1E3P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                    ROHC_C_R_R_1_ET;    

    rohc_ccpm_map_tbl[ROHC_R1IDP1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                    ROHC_NULL;          
    rohc_ccpm_map_tbl[ROHC_R1IDP1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                    ROHC_NULL;          
    rohc_ccpm_map_tbl[ROHC_R1IDP1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                    ROHC_C_R_R_1_ET;    

    rohc_ccpm_map_tbl[ROHC_R1IDE0P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;          
    rohc_ccpm_map_tbl[ROHC_R1IDE0P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;          
    rohc_ccpm_map_tbl[ROHC_R1IDE0P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_R_1_ET;    

    rohc_ccpm_map_tbl[ROHC_R1IDE1P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;          
    rohc_ccpm_map_tbl[ROHC_R1IDE1P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;          
    rohc_ccpm_map_tbl[ROHC_R1IDE1P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_R_1_ET;    

    rohc_ccpm_map_tbl[ROHC_R1IDE2P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;          
    rohc_ccpm_map_tbl[ROHC_R1IDE2P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;          
    rohc_ccpm_map_tbl[ROHC_R1IDE2P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_R_1_ET;    

    rohc_ccpm_map_tbl[ROHC_R1IDE3P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;          
    rohc_ccpm_map_tbl[ROHC_R1IDE3P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;          
    rohc_ccpm_map_tbl[ROHC_R1IDE3P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_R_1_ET;    

    rohc_ccpm_map_tbl[ROHC_R1TSP1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                    ROHC_NULL;          
    rohc_ccpm_map_tbl[ROHC_R1TSP1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                    ROHC_NULL;          
    rohc_ccpm_map_tbl[ROHC_R1TSP1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                    ROHC_C_R_R_1_ET;    

    rohc_ccpm_map_tbl[ROHC_R1TSE0P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;          
    rohc_ccpm_map_tbl[ROHC_R1TSE0P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;          
    rohc_ccpm_map_tbl[ROHC_R1TSE0P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_R_1_ET;    

    rohc_ccpm_map_tbl[ROHC_R1TSE1P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;          
    rohc_ccpm_map_tbl[ROHC_R1TSE1P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;          
    rohc_ccpm_map_tbl[ROHC_R1TSE1P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_R_1_ET;    

    rohc_ccpm_map_tbl[ROHC_R1TSE2P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;          
    rohc_ccpm_map_tbl[ROHC_R1TSE2P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;          
    rohc_ccpm_map_tbl[ROHC_R1TSE2P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_R_1_ET;    

    rohc_ccpm_map_tbl[ROHC_R1TSE3P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_ccpm_map_tbl[ROHC_R1TSE3P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_ccpm_map_tbl[ROHC_R1TSE3P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_R_1_ET;

    rohc_ccpm_map_tbl[ROHC_UO1P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] =  
                                                    ROHC_C_U_UO_1_ET;   
    rohc_ccpm_map_tbl[ROHC_UO1P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] =  
                                                    ROHC_C_O_UO_1_ET;   
    rohc_ccpm_map_tbl[ROHC_UO1P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] =  
                                                    ROHC_NULL;          

    rohc_ccpm_map_tbl[ROHC_UO1IDP1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] =  
                                                      ROHC_C_U_UO_1_ET;   
    rohc_ccpm_map_tbl[ROHC_UO1IDP1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] =  
                                                      ROHC_C_O_UO_1_ET;   
    rohc_ccpm_map_tbl[ROHC_UO1IDP1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] =  
                                                      ROHC_NULL;          

    rohc_ccpm_map_tbl[ROHC_UO1IDE0P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] =  
                                                        ROHC_C_U_UO_1_ET;   
    rohc_ccpm_map_tbl[ROHC_UO1IDE0P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] =  
                                                        ROHC_C_O_UO_1_ET;   
    rohc_ccpm_map_tbl[ROHC_UO1IDE0P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] =  
                                                        ROHC_NULL;          

    rohc_ccpm_map_tbl[ROHC_UO1IDE1P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] =  
                                                        ROHC_C_U_UO_1_ET;   
    rohc_ccpm_map_tbl[ROHC_UO1IDE1P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] =  
                                                        ROHC_C_O_UO_1_ET;   
    rohc_ccpm_map_tbl[ROHC_UO1IDE1P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] =  
                                                        ROHC_NULL;          

    rohc_ccpm_map_tbl[ROHC_UO1IDE2P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] =  
                                                        ROHC_C_U_UO_1_ET;   
    rohc_ccpm_map_tbl[ROHC_UO1IDE2P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] =  
                                                        ROHC_C_O_UO_1_ET;   
    rohc_ccpm_map_tbl[ROHC_UO1IDE2P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] =  
                                                        ROHC_NULL;          

    rohc_ccpm_map_tbl[ROHC_UO1IDE3P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] =  
                                                        ROHC_C_U_UO_1_ET;   
    rohc_ccpm_map_tbl[ROHC_UO1IDE3P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] =  
                                                        ROHC_C_O_UO_1_ET;   
    rohc_ccpm_map_tbl[ROHC_UO1IDE3P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] =  
                                                        ROHC_NULL;          

    rohc_ccpm_map_tbl[ROHC_UO1TSP1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_C_U_UO_1_ET;
    rohc_ccpm_map_tbl[ROHC_UO1TSP1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_C_O_UO_1_ET;
    rohc_ccpm_map_tbl[ROHC_UO1TSP1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_NULL;

    rohc_ccpm_map_tbl[ROHC_UOR2P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                  ROHC_C_U_UO_2_ET;     
    rohc_ccpm_map_tbl[ROHC_UOR2P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                  ROHC_C_O_UO_2_ET;     
    rohc_ccpm_map_tbl[ROHC_UOR2P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                  ROHC_C_R_R_2_ET;      

    rohc_ccpm_map_tbl[ROHC_UOR2E0P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                    ROHC_C_U_UO_2_ET;     
    rohc_ccpm_map_tbl[ROHC_UOR2E0P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                    ROHC_C_O_UO_2_ET;     
    rohc_ccpm_map_tbl[ROHC_UOR2E0P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                    ROHC_C_R_R_2_ET;      

    rohc_ccpm_map_tbl[ROHC_UOR2E1P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                    ROHC_C_U_UO_2_ET;     
    rohc_ccpm_map_tbl[ROHC_UOR2E1P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                    ROHC_C_O_UO_2_ET;     
    rohc_ccpm_map_tbl[ROHC_UOR2E1P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                    ROHC_C_R_R_2_ET;      

    rohc_ccpm_map_tbl[ROHC_UOR2E2P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                    ROHC_C_U_UO_2_ET;     
    rohc_ccpm_map_tbl[ROHC_UOR2E2P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                    ROHC_C_O_UO_2_ET;     
    rohc_ccpm_map_tbl[ROHC_UOR2E2P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                    ROHC_C_R_R_2_ET;      

    rohc_ccpm_map_tbl[ROHC_UOR2E3P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                    ROHC_C_U_UO_2_ET;     
    rohc_ccpm_map_tbl[ROHC_UOR2E3P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                    ROHC_C_O_UO_2_ET;     
    rohc_ccpm_map_tbl[ROHC_UOR2E3P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                    ROHC_C_R_R_2_ET;      

    rohc_ccpm_map_tbl[ROHC_UOR2IDP1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                    ROHC_C_U_UO_2_ET;     
    rohc_ccpm_map_tbl[ROHC_UOR2IDP1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                    ROHC_C_O_UO_2_ET;     
    rohc_ccpm_map_tbl[ROHC_UOR2IDP1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                    ROHC_C_R_R_2_ET;      

    rohc_ccpm_map_tbl[ROHC_UOR2IDE0P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_C_U_UO_2_ET;     
    rohc_ccpm_map_tbl[ROHC_UOR2IDE0P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_C_O_UO_2_ET;     
    rohc_ccpm_map_tbl[ROHC_UOR2IDE0P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_R_2_ET;      

    rohc_ccpm_map_tbl[ROHC_UOR2IDE1P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_C_U_UO_2_ET;     
    rohc_ccpm_map_tbl[ROHC_UOR2IDE1P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_C_O_UO_2_ET;     
    rohc_ccpm_map_tbl[ROHC_UOR2IDE1P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_R_2_ET;      

    rohc_ccpm_map_tbl[ROHC_UOR2IDE2P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_C_U_UO_2_ET;     
    rohc_ccpm_map_tbl[ROHC_UOR2IDE2P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_C_O_UO_2_ET;     
    rohc_ccpm_map_tbl[ROHC_UOR2IDE2P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_R_2_ET;      

    rohc_ccpm_map_tbl[ROHC_UOR2IDE3P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_C_U_UO_2_ET;     
    rohc_ccpm_map_tbl[ROHC_UOR2IDE3P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_C_O_UO_2_ET;     
    rohc_ccpm_map_tbl[ROHC_UOR2IDE3P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_R_2_ET;      

    rohc_ccpm_map_tbl[ROHC_UOR2TSP1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                    ROHC_C_U_UO_2_ET;     
    rohc_ccpm_map_tbl[ROHC_UOR2TSP1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                    ROHC_C_O_UO_2_ET;     
    rohc_ccpm_map_tbl[ROHC_UOR2TSP1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                    ROHC_C_R_R_2_ET;      

    rohc_ccpm_map_tbl[ROHC_UOR2TSE0P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_C_U_UO_2_ET;     
    rohc_ccpm_map_tbl[ROHC_UOR2TSE0P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_C_O_UO_2_ET;     
    rohc_ccpm_map_tbl[ROHC_UOR2TSE0P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_R_2_ET;      

    rohc_ccpm_map_tbl[ROHC_UOR2TSE1P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_C_U_UO_2_ET;     
    rohc_ccpm_map_tbl[ROHC_UOR2TSE1P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_C_O_UO_2_ET;     
    rohc_ccpm_map_tbl[ROHC_UOR2TSE1P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_R_2_ET;      

    rohc_ccpm_map_tbl[ROHC_UOR2TSE2P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_C_U_UO_2_ET;     
    rohc_ccpm_map_tbl[ROHC_UOR2TSE2P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_C_O_UO_2_ET;     
    rohc_ccpm_map_tbl[ROHC_UOR2TSE2P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_R_2_ET;      

    rohc_ccpm_map_tbl[ROHC_UOR2TSE3P1_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_C_U_UO_2_ET;
    rohc_ccpm_map_tbl[ROHC_UOR2TSE3P1_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_C_O_UO_2_ET;
    rohc_ccpm_map_tbl[ROHC_UOR2TSE3P1_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_R_2_ET;

    rohc_ccpm_map_tbl[ROHC_R1P23_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_ccpm_map_tbl[ROHC_R1P23_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_ccpm_map_tbl[ROHC_R1P23_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_R_1_ET;

    rohc_ccpm_map_tbl[ROHC_R1E0P23_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_ccpm_map_tbl[ROHC_R1E0P23_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_ccpm_map_tbl[ROHC_R1E0P23_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_R_1_ET;

    rohc_ccpm_map_tbl[ROHC_R1E1P23_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_ccpm_map_tbl[ROHC_R1E1P23_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_ccpm_map_tbl[ROHC_R1E1P23_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_R_1_ET;

    rohc_ccpm_map_tbl[ROHC_R1E2P23_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_ccpm_map_tbl[ROHC_R1E2P23_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_ccpm_map_tbl[ROHC_R1E2P23_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_R_1_ET;

    rohc_ccpm_map_tbl[ROHC_R1E3P23_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_ccpm_map_tbl[ROHC_R1E3P23_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_ccpm_map_tbl[ROHC_R1E3P23_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_R_1_ET;

    rohc_ccpm_map_tbl[ROHC_UO1P23_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_C_O_UO_1_ET;
    rohc_ccpm_map_tbl[ROHC_UO1P23_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_C_O_UO_1_ET;
    rohc_ccpm_map_tbl[ROHC_UO1P23_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_NULL;

    rohc_ccpm_map_tbl[ROHC_UOR2P23_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_C_U_UO_2_ET;
    rohc_ccpm_map_tbl[ROHC_UOR2P23_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_C_O_UO_2_ET;
    rohc_ccpm_map_tbl[ROHC_UOR2P23_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_R_2_ET;

    rohc_ccpm_map_tbl[ROHC_UOR2E0P23_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_C_U_UO_2_ET;
    rohc_ccpm_map_tbl[ROHC_UOR2E0P23_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_C_O_UO_2_ET;
    rohc_ccpm_map_tbl[ROHC_UOR2E0P23_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_R_2_ET;

    rohc_ccpm_map_tbl[ROHC_UOR2E1P23_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_C_U_UO_2_ET;
    rohc_ccpm_map_tbl[ROHC_UOR2E1P23_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_C_O_UO_2_ET;
    rohc_ccpm_map_tbl[ROHC_UOR2E1P23_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_R_2_ET;

    rohc_ccpm_map_tbl[ROHC_UOR2E2P23_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_C_U_UO_2_ET;
    rohc_ccpm_map_tbl[ROHC_UOR2E2P23_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_C_O_UO_2_ET;
    rohc_ccpm_map_tbl[ROHC_UOR2E2P23_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_R_2_ET;

    rohc_ccpm_map_tbl[ROHC_UOR2E3P23_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_C_U_UO_2_ET;
    rohc_ccpm_map_tbl[ROHC_UOR2E3P23_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_C_O_UO_2_ET;
    rohc_ccpm_map_tbl[ROHC_UOR2E3P23_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_R_2_ET;

    rohc_ccpm_map_tbl[ROHC_IRP0_PKTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_ccpm_map_tbl[ROHC_IRP0_PKTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_ccpm_map_tbl[ROHC_IRP0_PKTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_NULL;

    rohc_ccpm_map_tbl[ROHC_NORMALP0_PKTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_ccpm_map_tbl[ROHC_NORMALP0_PKTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_ccpm_map_tbl[ROHC_NORMALP0_PKTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_NULL;

    rohc_ccpm_map_tbl[ROHC_IR_WITHOUT_DYN_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_ccpm_map_tbl[ROHC_IR_WITHOUT_DYN_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_NULL;
    rohc_ccpm_map_tbl[ROHC_IR_WITHOUT_DYN_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_NULL;

    rohc_ccpm_map_tbl[ROHC_IR_WITH_DYN_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                      ROHC_C_U_IR_ET;
    rohc_ccpm_map_tbl[ROHC_IR_WITH_DYN_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_C_O_IR_ET;
    rohc_ccpm_map_tbl[ROHC_IR_WITH_DYN_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_IR_ET;

    rohc_ccpm_map_tbl[ROHC_IRDYN_PKTTYPE][ROHC_MODE_U - ROHC_MODE_U] = 
                                                     ROHC_C_U_IRDYN_ET;
    rohc_ccpm_map_tbl[ROHC_IRDYN_PKTTYPE][ROHC_MODE_O - ROHC_MODE_U] = 
                                                      ROHC_C_O_IRDYN_ET;
    rohc_ccpm_map_tbl[ROHC_IRDYN_PKTTYPE][ROHC_MODE_R - ROHC_MODE_U] = 
                                                      ROHC_C_R_IRDYN_ET;

    /* for profile 6 - Events for a packet in O and R mode is currently
     * set to NULL */

    rohc_ccpm_map_tbl[ROHC_CO_P6_COMMON_PKTYPE][0] = ROHC_C_U_CO_P6_COMMON_ET;

    rohc_ccpm_map_tbl[ROHC_CO_P6_RND1_PKTYPE][0] = ROHC_C_U_CO_P6_RND_ET;

    rohc_ccpm_map_tbl[ROHC_CO_P6_RND2_PKTYPE][0] = ROHC_C_U_CO_P6_RND_ET;

    rohc_ccpm_map_tbl[ROHC_CO_P6_RND3_PKTYPE][0] = ROHC_C_U_CO_P6_RND_ET;

    rohc_ccpm_map_tbl[ROHC_CO_P6_RND4_PKTYPE][0] = ROHC_C_U_CO_P6_RND_ET;

    rohc_ccpm_map_tbl[ROHC_CO_P6_RND5_PKTYPE][0] = ROHC_C_U_CO_P6_RND_ET;

    rohc_ccpm_map_tbl[ROHC_CO_P6_RND6_PKTYPE][0] =  ROHC_C_U_CO_P6_RND_ET;

    rohc_ccpm_map_tbl[ROHC_CO_P6_RND7_PKTYPE][0] = ROHC_C_U_CO_P6_RND_ET;

    rohc_ccpm_map_tbl[ROHC_CO_P6_RND8_PKTYPE][0] = ROHC_C_U_CO_P6_RND_ET;

    rohc_ccpm_map_tbl[ROHC_IRP6CR_PKTYPE][0] = ROHC_C_U_CO_IRP6CR_ET;
    ROHC_EXIT_FN("rohc_ccpm_init_evt_tbl");
}
